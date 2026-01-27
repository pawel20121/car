/**
 * @file static_config.cpp
 * @brief Static configuration data for State Management
 * 
 * This file contains the actual configuration data for:
 * - Controller StateMachine (manages Machine lifecycle)
 * - Agent StateMachine (Infotainment example)
 * - Transition tables
 * - Error recovery tables
 * - Action lists
 * 
 * Configuration is static (compile-time) for this implementation.
 * Production systems may load configuration from ARXML files.
 * 
 * @note This configuration follows AUTOSAR AP R24-11 requirements
 */

#include "static_config.h"
#include <cstddef>

namespace ara {
namespace sm {
namespace config {

// ============================================================================
// CONTROLLER TRANSITION REQUEST TABLE
// ============================================================================

/**
 * @brief Transition table for Controller StateMachine
 * 
 * Defines valid state transitions based on current state and trigger value.
 * Controller manages Machine lifecycle and Agent StateMachines.
 * 
 * @req [SWS_SM_00603-00607] StateMachine transition execution
 */
const TransitionRule kControllerTransitions[] = {
    // ========================================================================
    // FROM INITIAL STATE
    // ========================================================================
    {States::kInitial, Triggers::kStartup, States::kStartup},
    {States::kInitial, Triggers::kGoToRunning, States::kRunning},
    
    // ========================================================================
    // FROM STARTUP STATE
    // ========================================================================
    {States::kStartup, Triggers::kGoToRunning, States::kRunning},
    {States::kStartup, Triggers::kShutdownRequest, States::kShutdown},
    
    // ========================================================================
    // FROM RUNNING STATE
    // ========================================================================
    {States::kRunning, Triggers::kShutdownRequest, States::kShutdown},
    {States::kRunning, Triggers::kRestartRequest, States::kRestart},
    {States::kRunning, Triggers::kPrepareUpdateRequest, States::kPrepareUpdate},
    
    // ========================================================================
    // UPDATE CYCLE - PREPARE UPDATE
    // ========================================================================
    // From PrepareUpdate can go to Verify or Rollback
    {States::kPrepareUpdate, Triggers::kVerifyUpdateRequest, States::kVerifyUpdate},
    {States::kPrepareUpdate, Triggers::kPrepareRollbackRequest, States::kPrepareRollback},
    
    // ========================================================================
    // UPDATE CYCLE - VERIFY UPDATE
    // ========================================================================
    // From VerifyUpdate can finish successfully or rollback
    {States::kVerifyUpdate, Triggers::kFinishUpdateRequest, States::kAfterUpdate},
    {States::kVerifyUpdate, Triggers::kPrepareRollbackRequest, States::kPrepareRollback},
    
    // ========================================================================
    // UPDATE CYCLE - PREPARE ROLLBACK
    // ========================================================================
    // From PrepareRollback finish update (back to known good state)
    {States::kPrepareRollback, Triggers::kFinishUpdateRequest, States::kAfterUpdate},
    
    // ========================================================================
    // AFTER UPDATE STATE
    // ========================================================================
    // After update session ends, return to Running
    {States::kAfterUpdate, Triggers::kGoToRunning, States::kRunning},
    {States::kAfterUpdate, Triggers::kShutdownRequest, States::kShutdown},
    
    // ========================================================================
    // CONTINUE UPDATE STATE (after machine restart during update)
    // ========================================================================
    // @req [SWS_SM_00657] Enter ContinueUpdate after restart during update
    {States::kContinueUpdate, Triggers::kVerifyUpdateRequest, States::kVerifyUpdate},
    {States::kContinueUpdate, Triggers::kPrepareRollbackRequest, States::kPrepareRollback},
    
    // ========================================================================
    // RESTART STATE
    // ========================================================================
    // Restart is terminal state leading to machine reboot
    // No transitions from Restart (machine will reboot)
};

const size_t kControllerTransitionsCount = 
    sizeof(kControllerTransitions) / sizeof(TransitionRule);

// ============================================================================
// CONTROLLER ERROR RECOVERY TABLE
// ============================================================================

/**
 * @brief Error recovery table for Controller StateMachine
 * 
 * Defines recovery actions based on execution errors.
 * 
 * @req [SWS_SM_00601] StateMachine error notification reaction
 * @req [SWS_SM_CONSTR_00014] Handling of non-mapped ExecutionError (ANY rule)
 */
const ErrorRecoveryRule kControllerErrorRecovery[] = {
    // ========================================================================
    // FROM RUNNING STATE
    // ========================================================================
    // Critical process crash -> attempt restart
    {States::kRunning, ExecutionErrors::kProcessCrashed, States::kRestart},
    
    // Communication error -> shutdown gracefully
    {States::kRunning, ExecutionErrors::kCommunicationError, States::kShutdown},
    
    // Catch-all for Running state -> shutdown
    {States::kRunning, kExecutionErrorAny, States::kShutdown},
    
    // ========================================================================
    // FROM STARTUP STATE
    // ========================================================================
    // Any error during startup -> shutdown (can't proceed)
    {States::kStartup, kExecutionErrorAny, States::kShutdown},
    
    // ========================================================================
    // FROM VERIFY UPDATE STATE
    // ========================================================================
    // Verification failed -> rollback to previous version
    {States::kVerifyUpdate, ExecutionErrors::kVerificationFailed, States::kPrepareRollback},
    
    // Update failed -> rollback
    {States::kVerifyUpdate, ExecutionErrors::kUpdateFailed, States::kPrepareRollback},
    
    // Any other error during verify -> rollback
    {States::kVerifyUpdate, kExecutionErrorAny, States::kPrepareRollback},
    
    // ========================================================================
    // FROM PREPARE UPDATE STATE
    // ========================================================================
    // Error during preparation -> rollback
    {States::kPrepareUpdate, kExecutionErrorAny, States::kPrepareRollback},
    
    // ========================================================================
    // GLOBAL CATCH-ALL (must be last)
    // ========================================================================
    // Default: any unmapped state + error -> stay in current state
    // This is safer than blind shutdown for unknown states
    {States::kRunning, kExecutionErrorAny, States::kShutdown},
};

const size_t kControllerErrorRecoveryCount = 
    sizeof(kControllerErrorRecovery) / sizeof(ErrorRecoveryRule);

// ============================================================================
// AGENT (INFOTAINMENT) TRANSITION REQUEST TABLE
// ============================================================================

/**
 * @brief Transition table for Infotainment Agent StateMachine
 * 
 * Agent manages application-level functionality (infotainment applications).
 * Agent cannot start/stop other StateMachines (only Controller can).
 */
const TransitionRule kInfotainmentTransitions[] = {
    // ========================================================================
    // FROM INITIAL STATE
    // ========================================================================
    {States::kInitial, Triggers::kGoToRunning, States::kRunning},
    {States::kInitial, Triggers::kUserRequest, States::kRunning},
    
    // ========================================================================
    // FROM RUNNING STATE
    // ========================================================================
    {States::kRunning, Triggers::kShutdownRequest, States::kOff},
    {States::kRunning, Triggers::kDegradeRequest, States::kDegraded},
    {States::kRunning, Triggers::kPrepareUpdateRequest, States::kPrepareUpdate},
    
    // ========================================================================
    // FROM DEGRADED STATE
    // ========================================================================
    // Degraded mode allows recovery back to Running
    {States::kDegraded, Triggers::kGoToRunning, States::kRunning},
    {States::kDegraded, Triggers::kShutdownRequest, States::kOff},
    {States::kDegraded, Triggers::kPrepareUpdateRequest, States::kPrepareUpdate},
    
    // ========================================================================
    // UPDATE CYCLE
    // ========================================================================
    {States::kPrepareUpdate, Triggers::kVerifyUpdateRequest, States::kVerifyUpdate},
    {States::kPrepareUpdate, Triggers::kPrepareRollbackRequest, States::kPrepareRollback},
    
    {States::kVerifyUpdate, Triggers::kFinishUpdateRequest, States::kRunning},
    {States::kVerifyUpdate, Triggers::kPrepareRollbackRequest, States::kPrepareRollback},
    
    {States::kPrepareRollback, Triggers::kFinishUpdateRequest, States::kRunning},
    
    // ========================================================================
    // FROM OFF STATE
    // ========================================================================
    {States::kOff, Triggers::kGoToRunning, States::kRunning},
    {States::kOff, Triggers::kUserRequest, States::kRunning},
};

const size_t kInfotainmentTransitionsCount = 
    sizeof(kInfotainmentTransitions) / sizeof(TransitionRule);

// ============================================================================
// AGENT ERROR RECOVERY TABLE
// ============================================================================

/**
 * @brief Error recovery table for Infotainment Agent
 */
const ErrorRecoveryRule kInfotainmentErrorRecovery[] = {
    // ========================================================================
    // FROM RUNNING STATE
    // ========================================================================
    // Process crash -> try degraded mode first
    {States::kRunning, ExecutionErrors::kProcessCrashed, States::kDegraded},
    
    // Memory violation -> degraded mode
    {States::kRunning, ExecutionErrors::kMemoryViolation, States::kDegraded},
    
    // Catch-all for Running -> Off
    {States::kRunning, kExecutionErrorAny, States::kOff},
    
    // ========================================================================
    // FROM DEGRADED STATE
    // ========================================================================
    // Any error in degraded mode -> Off (can't recover further)
    {States::kDegraded, kExecutionErrorAny, States::kOff},
    
    // ========================================================================
    // FROM VERIFY UPDATE STATE
    // ========================================================================
    // Verification failed -> rollback
    {States::kVerifyUpdate, ExecutionErrors::kVerificationFailed, States::kPrepareRollback},
    {States::kVerifyUpdate, kExecutionErrorAny, States::kPrepareRollback},
    
    // ========================================================================
    // GLOBAL CATCH-ALL
    // ========================================================================
    {States::kOff, kExecutionErrorAny, States::kOff},  // Stay off if already off
};

const size_t kInfotainmentErrorRecoveryCount = 
    sizeof(kInfotainmentErrorRecovery) / sizeof(ErrorRecoveryRule);

// ============================================================================
// ACTION LISTS - CONTROLLER
// ============================================================================

/**
 * @brief Terminator for action lists
 * 
 * Used to mark end of variable-length action lists
 */
static const ActionItem kActionTerminator = {
    ActionType::kSync,  // Use Sync as terminator (safe no-op)
    nullptr,
    nullptr,
    0
};

/**
 * @brief Action list for Initial state
 * 
 * Controller starts Machine and initializes Agents
 */
static const ActionItem kInitialActions[] = {
    // Set MachineFG to Startup
    {ActionType::kSetFunctionGroupState, "MachineFG", "Startup", 0},
    
    // Wait for startup to complete
    {ActionType::kSync, nullptr, nullptr, 0},
    
    // Start Infotainment Agent (will enter its Initial state)
    {ActionType::kStartStateMachine, "InfotainmentSM", "", 0},
    
    // Terminator
    {ActionType::kSync, nullptr, nullptr, 0}
};

/**
 * @brief Action list for Startup state
 */
static const ActionItem kStartupActions[] = {
    // MachineFG already in Startup, just prepare for transition
    {ActionType::kSetFunctionGroupState, "MachineFG", "Startup", 0},
    {ActionType::kSync, nullptr, nullptr, 0}
};

/**
 * @brief Action list for Running state
 * 
 * Full operation mode with network communication
 */
static const ActionItem kRunningActions[] = {
    // Set MachineFG to Running
    {ActionType::kSetFunctionGroupState, "MachineFG", "Running", 0},
    
    // Enable network communication
    {ActionType::kSetNetworkHandle, "VehicleNetwork", "FullCom", 0},
    
    // Start Agent in Running mode
    {ActionType::kStartStateMachine, "InfotainmentSM", "Running", 0},
    
    {ActionType::kSync, nullptr, nullptr, 0}
};

/**
 * @brief Action list for Shutdown state
 * 
 * @req [SWS_SM_CONSTR_00017] ActionListItem "Function Group State" in Controller
 */
static const ActionItem kShutdownActions[] = {
    // Stop all Agents first
    {ActionType::kStopStateMachine, "InfotainmentSM", nullptr, 0},
    {ActionType::kSync, nullptr, nullptr, 0},
    
    // Disable network
    {ActionType::kSetNetworkHandle, "VehicleNetwork", "NoCom", 0},
    
    // Small delay for graceful network shutdown
    {ActionType::kSleep, nullptr, nullptr, 500},
    
    // Finally shutdown machine
    {ActionType::kSetFunctionGroupState, "MachineFG", "Shutdown", 0},
};

/**
 * @brief Action list for Restart state
 * 
 * @req [SWS_SM_CONSTR_00029] Existence of Restart state
 * @req [SWS_SM_CONSTR_00030] MachineFG Restart in Restart state
 */
static const ActionItem kRestartActions[] = {
    // Stop all Agents
    {ActionType::kStopStateMachine, "InfotainmentSM", nullptr, 0},
    {ActionType::kSync, nullptr, nullptr, 0},
    
    // Request machine restart
    {ActionType::kSetFunctionGroupState, "MachineFG", "Restart", 0},
};

/**
 * @brief Action list for PrepareUpdate state
 * 
 * @req [SWS_SM_CONSTR_00021] Existence of PrepareUpdate state
 * @req [SWS_SM_00633] Transition affected StateMachines to PrepareUpdate
 * @req [SWS_SM_00634] Shutdown affected StateMachines
 */
static const ActionItem kPrepareUpdateActions[] = {
    // Transition affected Agents to PrepareUpdate
    {ActionType::kStartStateMachine, "InfotainmentSM", "PrepareUpdate", 0},
    {ActionType::kSync, nullptr, nullptr, 0},
    
    // Then stop them
    {ActionType::kStopStateMachine, "InfotainmentSM", nullptr, 0},
    {ActionType::kSync, nullptr, nullptr, 0},
    
    // Set MachineFG to minimal state
    {ActionType::kSetFunctionGroupState, "MachineFG", "Off", 0},
};

/**
 * @brief Action list for VerifyUpdate state
 * 
 * @req [SWS_SM_CONSTR_00022] Existence of VerifyUpdate state
 * @req [SWS_SM_00638] Transition affected StateMachines to VerifyUpdate
 */
static const ActionItem kVerifyUpdateActions[] = {
    // Start Agents in VerifyUpdate mode
    {ActionType::kStartStateMachine, "InfotainmentSM", "VerifyUpdate", 0},
    {ActionType::kSync, nullptr, nullptr, 0},
    
    // Set MachineFG to Verify
    {ActionType::kSetFunctionGroupState, "MachineFG", "Verify", 0},
};

/**
 * @brief Action list for PrepareRollback state
 * 
 * @req [SWS_SM_CONSTR_00023] Existence of PrepareRollback state
 * @req [SWS_SM_00642] Transition to PrepareRollback
 * @req [SWS_SM_00643] Shutdown affected StateMachines
 */
static const ActionItem kPrepareRollbackActions[] = {
    // Transition Agents to PrepareRollback
    {ActionType::kStartStateMachine, "InfotainmentSM", "PrepareRollback", 0},
    {ActionType::kSync, nullptr, nullptr, 0},
    
    // Stop them
    {ActionType::kStopStateMachine, "InfotainmentSM", nullptr, 0},
    {ActionType::kSync, nullptr, nullptr, 0},
    
    // Machine to minimal state
    {ActionType::kSetFunctionGroupState, "MachineFG", "Off", 0},
};

/**
 * @brief Action list for ContinueUpdate state
 * 
 * @req [SWS_SM_CONSTR_00028] Existence of ContinueUpdate state
 * 
 * Entered after machine restart during update session.
 * Only essential processes should run.
 */
static const ActionItem kContinueUpdateActions[] = {
    // Start only essential services (UCM, SM, PHM already running)
    {ActionType::kSetFunctionGroupState, "MachineFG", "Startup", 0},
    {ActionType::kSync, nullptr, nullptr, 0},
    
    // Do NOT start Agents - they may be being updated
};

/**
 * @brief Action list for AfterUpdate state
 * 
 * @req [SWS_SM_CONSTR_00027] Existence of AfterUpdate state
 * 
 * Cleanup after update session ends
 */
static const ActionItem kAfterUpdateActions[] = {
    // Return to normal operation
    {ActionType::kSetFunctionGroupState, "MachineFG", "Running", 0},
    {ActionType::kStartStateMachine, "InfotainmentSM", "Running", 0},
    {ActionType::kSync, nullptr, nullptr, 0},
};

/**
 * @brief Complete action table for Controller
 * 
 * Maps each state to its action list
 */
const ActionListEntry kActionTable[] = {
    {States::kInitial, kInitialActions, 4},
    {States::kStartup, kStartupActions, 2},
    {States::kRunning, kRunningActions, 4},
    {States::kShutdown, kShutdownActions, 5},
    {States::kRestart, kRestartActions, 3},
    {States::kPrepareUpdate, kPrepareUpdateActions, 5},
    {States::kVerifyUpdate, kVerifyUpdateActions, 3},
    {States::kPrepareRollback, kPrepareRollbackActions, 5},
    {States::kContinueUpdate, kContinueUpdateActions, 2},
    {States::kAfterUpdate, kAfterUpdateActions, 3},
};

const size_t kActionTableCount = sizeof(kActionTable) / sizeof(ActionListEntry);

// ============================================================================
// ACTION LISTS - AGENT (INFOTAINMENT)
// ============================================================================

/**
 * @brief Action list for Agent Off state
 * 
 * @req [SWS_SM_CONSTR_00024] Existence of Off state for Agent
 * @req [SWS_SM_CONSTR_00011] ActionListItems allowed in Off state
 */
static const ActionItem kInfotainmentOffActions[] = {
    // Stop all infotainment function groups
    {ActionType::kSetFunctionGroupState, "InfotainmentFG", "Off", 0},
    
    // Disable media network
    {ActionType::kSetNetworkHandle, "MediaNetwork", "NoCom", 0},
    
    {ActionType::kSync, nullptr, nullptr, 0},
};

/**
 * @brief Action list for Agent Running state
 */
static const ActionItem kInfotainmentRunningActions[] = {
    // Start infotainment applications
    {ActionType::kSetFunctionGroupState, "InfotainmentFG", "Running", 0},
    
    // Enable media network
    {ActionType::kSetNetworkHandle, "MediaNetwork", "FullCom", 0},
    
    {ActionType::kSync, nullptr, nullptr, 0},
};

/**
 * @brief Action list for Agent Degraded state
 */
static const ActionItem kInfotainmentDegradedActions[] = {
    // Run in limited mode
    {ActionType::kSetFunctionGroupState, "InfotainmentFG", "Degraded", 0},
    
    // Keep network but limited
    {ActionType::kSetNetworkHandle, "MediaNetwork", "FullCom", 0},
    
    {ActionType::kSync, nullptr, nullptr, 0},
};

/**
 * @brief Action list for Agent PrepareUpdate
 */
static const ActionItem kInfotainmentPrepareUpdateActions[] = {
    {ActionType::kSetFunctionGroupState, "InfotainmentFG", "Off", 0},
    {ActionType::kSetNetworkHandle, "MediaNetwork", "NoCom", 0},
    {ActionType::kSync, nullptr, nullptr, 0},
};

/**
 * @brief Action list for Agent VerifyUpdate
 */
static const ActionItem kInfotainmentVerifyUpdateActions[] = {
    // Start in verify mode
    {ActionType::kSetFunctionGroupState, "InfotainmentFG", "Verify", 0},
    {ActionType::kSetNetworkHandle, "MediaNetwork", "FullCom", 0},
    {ActionType::kSync, nullptr, nullptr, 0},
};

/**
 * @brief Complete action table for Agent
 * 
 * @req [SWS_SM_CONSTR_00015] Completeness of controlled Function Groups
 * @req [SWS_SM_CONSTR_00032] Completeness of controlled NetworkHandles
 */
const ActionListEntry kInfotainmentActionTable[] = {
    {States::kOff, kInfotainmentOffActions, 3},
    {States::kRunning, kInfotainmentRunningActions, 3},
    {States::kDegraded, kInfotainmentDegradedActions, 3},
    {States::kPrepareUpdate, kInfotainmentPrepareUpdateActions, 3},
    {States::kVerifyUpdate, kInfotainmentVerifyUpdateActions, 3},
};

const size_t kInfotainmentActionTableCount = 
    sizeof(kInfotainmentActionTable) / sizeof(ActionListEntry);

} // namespace config
} // namespace sm
} // namespace ara

