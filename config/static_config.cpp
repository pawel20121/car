#include "static_config.h"


/**
 * @file static_config.cpp
 * @brief Implementation of static configuration for State Management
 * 
 * This file contains the actual configuration data for a sample system with:
 * - Controller StateMachine managing Machine lifecycle
 * - Agent StateMachine (Infotainment) managing application functionality
 * 
 * Configuration follows AUTOSAR AP R24-11 requirements
 */

namespace ara {
namespace sm {
namespace config {

// ============================================================================
// CONTROLLER TRANSITION TABLE
// ============================================================================

/**
 * @brief Controller StateMachine transition rules
 * @req [SWS_SM_00603-00607]
 * 
 * Controller manages Machine lifecycle and Agent StateMachines
 */
const TransitionRule kControllerTransitions[] = {
    // ========================================================================
    // FROM INITIAL STATE
    // ========================================================================
    {States::kInitial, Triggers::kStartup, States::kRunning},
    {States::kInitial, Triggers::kShutdownRequest, States::kShutdown},
    {States::kInitial, Triggers::kPrepareUpdateRequest, States::kPrepareUpdate},
    
    // ========================================================================
    // FROM RUNNING STATE
    // ========================================================================
    {States::kRunning, Triggers::kShutdownRequest, States::kShutdown},
    {States::kRunning, Triggers::kRestartRequest, States::kRestart},
    {States::kRunning, Triggers::kPrepareUpdateRequest, States::kPrepareUpdate},
    
    // ========================================================================
    // FROM PREPARE UPDATE STATE
    // ========================================================================
    {States::kPrepareUpdate, Triggers::kVerifyUpdateRequest, States::kVerifyUpdate},
    {States::kPrepareUpdate, Triggers::kPrepareRollbackRequest, States::kPrepareRollback},
    {States::kPrepareUpdate, Triggers::kShutdownRequest, States::kShutdown},
    
    // ========================================================================
    // FROM VERIFY UPDATE STATE
    // ========================================================================
    {States::kVerifyUpdate, Triggers::kFinishUpdateRequest, States::kAfterUpdate},
    {States::kVerifyUpdate, Triggers::kPrepareRollbackRequest, States::kPrepareRollback},
    
    // ========================================================================
    // FROM PREPARE ROLLBACK STATE
    // ========================================================================
    {States::kPrepareRollback, Triggers::kFinishUpdateRequest, States::kAfterUpdate},
    
    // ========================================================================
    // FROM AFTER UPDATE STATE
    // ========================================================================
    {States::kAfterUpdate, Triggers::kGoToRunning, States::kRunning},
    {States::kAfterUpdate, Triggers::kShutdownRequest, States::kShutdown},
    
    // ========================================================================
    // FROM CONTINUE UPDATE STATE (after machine restart during update)
    // @req [SWS_SM_00657]
    // ========================================================================
    {States::kContinueUpdate, Triggers::kVerifyUpdateRequest, States::kVerifyUpdate},
    {States::kContinueUpdate, Triggers::kPrepareRollbackRequest, States::kPrepareRollback},
    
    // ========================================================================
    // FROM SHUTDOWN STATE
    // ========================================================================
    {States::kShutdown, Triggers::kRestartRequest, States::kRestart},
    
    // ========================================================================
    // FROM RESTART STATE
    // ========================================================================
    // Restart state leads to machine reboot, no transitions out
};

const size_t kControllerTransitionsCount = 
    sizeof(kControllerTransitions) / sizeof(TransitionRule);

// ============================================================================
// AGENT (INFOTAINMENT) TRANSITION TABLE
// ============================================================================

/**
 * @brief Agent StateMachine transition rules (Infotainment example)
 * @req [SWS_SM_00603-00607]
 * 
 * Agent manages application-level functionality
 */
const TransitionRule kInfotainmentTransitions[] = {
    // ========================================================================
    // FROM INITIAL STATE
    // ========================================================================
    {States::kInitial, Triggers::kGoToRunning, States::kRunning},
    {States::kInitial, Triggers::kShutdownRequest, States::kOff},
    
    // ========================================================================
    // FROM RUNNING STATE
    // ========================================================================
    {States::kRunning, Triggers::kShutdownRequest, States::kOff},
    {States::kRunning, Triggers::kPrepareUpdateRequest, States::kPrepareUpdate},
    
    // ========================================================================
    // FROM OFF STATE
    // ========================================================================
    {States::kOff, Triggers::kGoToRunning, States::kRunning},
    
    // ========================================================================
    // UPDATE-RELATED TRANSITIONS
    // ========================================================================
    {States::kPrepareUpdate, Triggers::kVerifyUpdateRequest, States::kVerifyUpdate},
    {States::kPrepareUpdate, Triggers::kPrepareRollbackRequest, States::kPrepareRollback},
    
    {States::kVerifyUpdate, Triggers::kFinishUpdateRequest, States::kRunning},
    {States::kVerifyUpdate, Triggers::kPrepareRollbackRequest, States::kPrepareRollback},
    
    {States::kPrepareRollback, Triggers::kFinishUpdateRequest, States::kOff},
    
    // ========================================================================
    // NETWORK-TRIGGERED TRANSITIONS (example)
    // ========================================================================
    {States::kOff, Triggers::kNetworkFullCom, States::kRunning},
    {States::kRunning, Triggers::kNetworkNoCom, States::kOff},
};

const size_t kInfotainmentTransitionsCount = 
    sizeof(kInfotainmentTransitions) / sizeof(TransitionRule);

// ============================================================================
// CONTROLLER ERROR RECOVERY TABLE
// ============================================================================

/**
 * @brief Controller error recovery rules
 * @req [SWS_SM_00601], [SWS_SM_CONSTR_00014]
 */
const ErrorRecoveryRule kControllerErrorRecovery[] = {
    // ========================================================================
    // FROM RUNNING STATE
    // ========================================================================
    {States::kRunning, ExecutionErrors::kProcessCrashed, States::kShutdown},
    {States::kRunning, ExecutionErrors::kMemoryViolation, States::kShutdown},
    {States::kRunning, kExecutionErrorAny, States::kShutdown},
    
    // ========================================================================
    // FROM PREPARE UPDATE STATE
    // ========================================================================
    {States::kPrepareUpdate, ExecutionErrors::kUpdateFailed, States::kPrepareRollback},
    {States::kPrepareUpdate, kExecutionErrorAny, States::kPrepareRollback},
    
    // ========================================================================
    // FROM VERIFY UPDATE STATE
    // ========================================================================
    {States::kVerifyUpdate, ExecutionErrors::kVerificationFailed, States::kPrepareRollback},
    {States::kVerifyUpdate, kExecutionErrorAny, States::kPrepareRollback},
    
    // ========================================================================
    // FROM PREPARE ROLLBACK STATE
    // ========================================================================
    {States::kPrepareRollback, kExecutionErrorAny, States::kShutdown},
    
    // ========================================================================
    // FROM AFTER UPDATE STATE
    // ========================================================================
    {States::kAfterUpdate, kExecutionErrorAny, States::kShutdown},
    
    // ========================================================================
    // FROM CONTINUE UPDATE STATE
    // ========================================================================
    {States::kContinueUpdate, kExecutionErrorAny, States::kPrepareRollback},
    
    // ========================================================================
    // SAFE STATES (stay in current state)
    // ========================================================================
    {States::kInitial, kExecutionErrorAny, States::kInitial},
    {States::kOff, kExecutionErrorAny, States::kOff},
    {States::kShutdown, kExecutionErrorAny, States::kShutdown},
    {States::kRestart, kExecutionErrorAny, States::kRestart},
};

const size_t kControllerErrorRecoveryCount = 
    sizeof(kControllerErrorRecovery) / sizeof(ErrorRecoveryRule);

// ============================================================================
// AGENT ERROR RECOVERY TABLE
// ============================================================================

/**
 * @brief Agent error recovery rules
 * @req [SWS_SM_00601], [SWS_SM_CONSTR_00014]
 */
const ErrorRecoveryRule kInfotainmentErrorRecovery[] = {
    // ========================================================================
    // FROM RUNNING STATE
    // ========================================================================
    {States::kRunning, ExecutionErrors::kProcessCrashed, States::kOff},
    {States::kRunning, ExecutionErrors::kCheckpointViolation, States::kOff},
    {States::kRunning, kExecutionErrorAny, States::kOff},
    
    // ========================================================================
    // UPDATE-RELATED ERROR RECOVERY
    // ========================================================================
    {States::kPrepareUpdate, kExecutionErrorAny, States::kPrepareRollback},
    {States::kVerifyUpdate, kExecutionErrorAny, States::kPrepareRollback},
    {States::kPrepareRollback, kExecutionErrorAny, States::kOff},
    
    // ========================================================================
    // SAFE STATES (stay in current state)
    // ========================================================================
    {States::kInitial, kExecutionErrorAny, States::kInitial},
    {States::kOff, kExecutionErrorAny, States::kOff},
};

const size_t kInfotainmentErrorRecoveryCount = 
    sizeof(kInfotainmentErrorRecovery) / sizeof(ErrorRecoveryRule);

// ============================================================================
// ACTION LISTS FOR CONTROLLER STATES
// ============================================================================

/**
 * @brief Actions for Controller Initial state
 * @req [SWS_SM_00608-00614], [SWS_SM_CONSTR_00017]
 */
static const ActionItem kControllerInitialActions[] = {
    {ActionType::kSetFunctionGroupState, "MachineFG", "Startup", 0},
    {ActionType::kSync, nullptr, nullptr, 0},
    {ActionType::kStartStateMachine, "InfotainmentSM", "Initial", 0},
    {ActionType::kSync, nullptr, nullptr, 0},
    {ActionType::kSetFunctionGroupState, "MachineFG", "Running", 0},
};

/**
 * @brief Actions for Controller Running state
 * @req [SWS_SM_00608], [SWS_SM_00625]
 */
static const ActionItem kControllerRunningActions[] = {
    {ActionType::kSetFunctionGroupState, "MachineFG", "Running", 0},
    {ActionType::kSetNetworkHandle, "VehicleNetwork", "FullCom", 0},
    {ActionType::kStartStateMachine, "InfotainmentSM", "Running", 0},
};

/**
 * @brief Actions for Controller Shutdown state
 * @req [SWS_SM_00614], [SWS_SM_00626], [SWS_SM_CONSTR_00017]
 */
static const ActionItem kControllerShutdownActions[] = {
    {ActionType::kStopStateMachine, "InfotainmentSM", nullptr, 0},
    {ActionType::kSync, nullptr, nullptr, 0},
    {ActionType::kSetNetworkHandle, "VehicleNetwork", "NoCom", 0},
    {ActionType::kSetFunctionGroupState, "MachineFG", "Shutdown", 0},
};

/**
 * @brief Actions for Controller PrepareUpdate state
 * @req [SWS_SM_00633-00634], [SWS_SM_CONSTR_00021]
 */
static const ActionItem kControllerPrepareUpdateActions[] = {
    {ActionType::kSetFunctionGroupState, "MachineFG", "Running", 0},
    {ActionType::kStopStateMachine, "InfotainmentSM", nullptr, 0},
    {ActionType::kSync, nullptr, nullptr, 0},
};

/**
 * @brief Actions for Controller VerifyUpdate state
 * @req [SWS_SM_00638], [SWS_SM_CONSTR_00022]
 */
static const ActionItem kControllerVerifyUpdateActions[] = {
    {ActionType::kSetFunctionGroupState, "MachineFG", "Running", 0},
    {ActionType::kStartStateMachine, "InfotainmentSM", "VerifyUpdate", 0},
    {ActionType::kSync, nullptr, nullptr, 0},
};

/**
 * @brief Actions for Controller PrepareRollback state
 * @req [SWS_SM_00642-00643], [SWS_SM_CONSTR_00023]
 */
static const ActionItem kControllerPrepareRollbackActions[] = {
    {ActionType::kStopStateMachine, "InfotainmentSM", nullptr, 0},
    {ActionType::kSync, nullptr, nullptr, 0},
    {ActionType::kSetFunctionGroupState, "MachineFG", "Running", 0},
};

/**
 * @brief Actions for Controller AfterUpdate state
 * @req [SWS_SM_00646], [SWS_SM_CONSTR_00027]
 */
static const ActionItem kControllerAfterUpdateActions[] = {
    {ActionType::kSetFunctionGroupState, "MachineFG", "Running", 0},
    {ActionType::kStartStateMachine, "InfotainmentSM", "Initial", 0},
};

/**
 * @brief Actions for Controller ContinueUpdate state
 * @req [SWS_SM_00657], [SWS_SM_CONSTR_00028]
 */
static const ActionItem kControllerContinueUpdateActions[] = {
    {ActionType::kSetFunctionGroupState, "MachineFG", "Running", 0},
    // Only essential services for UCM to continue
};

/**
 * @brief Actions for Controller Restart state
 * @req [SWS_SM_00658], [SWS_SM_CONSTR_00029-00030]
 */
static const ActionItem kControllerRestartActions[] = {
    {ActionType::kStopStateMachine, "InfotainmentSM", nullptr, 0},
    {ActionType::kSync, nullptr, nullptr, 0},
    {ActionType::kSetNetworkHandle, "VehicleNetwork", "NoCom", 0},
    {ActionType::kSetFunctionGroupState, "MachineFG", "Restart", 0},
};

/**
 * @brief Actions for Controller Off state (not used by Controller but required by table)
 */
static const ActionItem kControllerOffActions[] = {
    {ActionType::kSetFunctionGroupState, "MachineFG", "Off", 0},
};

// ============================================================================
// ACTION LISTS FOR AGENT (INFOTAINMENT) STATES
// ============================================================================

/**
 * @brief Actions for Agent Initial state
 * @req [SWS_SM_00608]
 */
static const ActionItem kInfotainmentInitialActions[] = {
    {ActionType::kSetFunctionGroupState, "InfotainmentFG", "Startup", 0},
    {ActionType::kSync, nullptr, nullptr, 0},
    {ActionType::kSetFunctionGroupState, "InfotainmentFG", "Running", 0},
};

/**
 * @brief Actions for Agent Running state
 * @req [SWS_SM_00608]
 */
static const ActionItem kInfotainmentRunningActions[] = {
    {ActionType::kSetFunctionGroupState, "InfotainmentFG", "Running", 0},
    {ActionType::kSetNetworkHandle, "MediaNetwork", "FullCom", 0},
};

/**
 * @brief Actions for Agent Off state
 * @req [SWS_SM_CONSTR_00011], [SWS_SM_CONSTR_00024]
 */
static const ActionItem kInfotainmentOffActions[] = {
    {ActionType::kSetNetworkHandle, "MediaNetwork", "NoCom", 0},
    {ActionType::kSetFunctionGroupState, "InfotainmentFG", "Off", 0},
};

/**
 * @brief Actions for Agent PrepareUpdate state
 * @req [SWS_SM_CONSTR_00021]
 */
static const ActionItem kInfotainmentPrepareUpdateActions[] = {
    {ActionType::kSetFunctionGroupState, "InfotainmentFG", "Off", 0},
    {ActionType::kSetNetworkHandle, "MediaNetwork", "NoCom", 0},
};

/**
 * @brief Actions for Agent VerifyUpdate state
 * @req [SWS_SM_CONSTR_00022]
 */
static const ActionItem kInfotainmentVerifyUpdateActions[] = {
    {ActionType::kSetFunctionGroupState, "InfotainmentFG", "Verify", 0},
};

/**
 * @brief Actions for Agent PrepareRollback state
 * @req [SWS_SM_CONSTR_00023]
 */
static const ActionItem kInfotainmentPrepareRollbackActions[] = {
    {ActionType::kSetFunctionGroupState, "InfotainmentFG", "Off", 0},
    {ActionType::kSetNetworkHandle, "MediaNetwork", "NoCom", 0},
};

// ============================================================================
// MAIN ACTION TABLE FOR CONTROLLER
// ============================================================================

/**
 * @brief Controller action table mapping states to action lists
 * @req [SWS_SM_00609], [SWS_SM_CONSTR_00015]
 */
const ActionListEntry kActionTable[] = {
    // ========================================================================
    // CONTROLLER ACTIONS
    // ========================================================================
    {States::kInitial, kControllerInitialActions, 5},
    {States::kRunning, kControllerRunningActions, 3},
    {States::kShutdown, kControllerShutdownActions, 4},
    {States::kPrepareUpdate, kControllerPrepareUpdateActions, 3},
    {States::kVerifyUpdate, kControllerVerifyUpdateActions, 3},
    {States::kPrepareRollback, kControllerPrepareRollbackActions, 3},
    {States::kAfterUpdate, kControllerAfterUpdateActions, 2},
    {States::kContinueUpdate, kControllerContinueUpdateActions, 1},
    {States::kRestart, kControllerRestartActions, 4},
    {States::kOff, kControllerOffActions, 1},
};

const size_t kActionTableCount = sizeof(kActionTable) / sizeof(ActionListEntry);

// ============================================================================
// SEPARATE ACTION TABLE FOR AGENT (INFOTAINMENT)
// ============================================================================

/**
 * @brief Infotainment Agent action table mapping states to action lists
 * @req [SWS_SM_00609], [SWS_SM_CONSTR_00015]
 * 
 * In a real implementation, this would be used by the Infotainment StateMachine
 */
const ActionListEntry kInfotainmentActionTable[] = {
    {States::kInitial, kInfotainmentInitialActions, 3},
    {States::kRunning, kInfotainmentRunningActions, 2},
    {States::kOff, kInfotainmentOffActions, 2},
    {States::kPrepareUpdate, kInfotainmentPrepareUpdateActions, 2},
    {States::kVerifyUpdate, kInfotainmentVerifyUpdateActions, 1},
    {States::kPrepareRollback, kInfotainmentPrepareRollbackActions, 2},
};

const size_t kInfotainmentActionTableCount = 
    sizeof(kInfotainmentActionTable) / sizeof(ActionListEntry);

} // namespace config
} // namespace sm
} // namespace ara