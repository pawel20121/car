#ifndef ARA_SM_CONFIG_STATIC_CONFIG_H
#define ARA_SM_CONFIG_STATIC_CONFIG_H

#include <cstdint>
#include "ara/sm/types.h"

/**
 * @file static_config.h
 * @brief Static configuration for State Management
 * 
 * This file contains compile-time configuration for the State Management module.
 * In a real AUTOSAR system, this would be generated from ARXML configuration.
 * 
 * For this project, we define a simplified configuration with:
 * - One Controller StateMachine (MainController)
 * - One Agent StateMachine (InfotainmentAgent)
 * - Basic transition tables
 * - Simple action lists
 * 
 * Based on AUTOSAR AP R24-11 State Management Specification
 */

namespace ara {
namespace sm {
namespace config {

// ============================================================================
// Transition Request Values
// ============================================================================

/**
 * @brief Transition request identifiers
 * 
 * These values are used by SMControlApplication to request state changes
 * via StateMachineService::RequestTransition()
 */
constexpr uint32_t kRequestGoToRunning = 1;
constexpr uint32_t kRequestGoToShutdown = 2;
constexpr uint32_t kRequestGoToOff = 3;

// Update-related transitions
constexpr uint32_t kRequestPrepareUpdate = 100;
constexpr uint32_t kRequestVerifyUpdate = 101;
constexpr uint32_t kRequestPrepareRollback = 102;
constexpr uint32_t kRequestAfterUpdate = 103;
constexpr uint32_t kRequestContinueUpdate = 104;

// ============================================================================
// Execution Error Values
// ============================================================================

/**
 * @brief Execution error codes
 * 
 * These are reported by Platform Health Management and Execution Management
 */
constexpr uint32_t kExecutionErrorGeneric = 1;
constexpr uint32_t kExecutionErrorSupervision = 2;
constexpr uint32_t kExecutionErrorCrash = 3;
constexpr uint32_t kExecutionErrorTimeout = 4;

// Special value for catch-all error rules
// [SWS_SM_CONSTR_00014] Handling of non-mapped ExecutionError
constexpr uint32_t kExecutionErrorAny = 0xFFFFFFFF;

// ============================================================================
// Function Group Names
// ============================================================================

/**
 * @brief Function Group identifiers
 * 
 * These correspond to FunctionGroups defined in the system
 */
constexpr const char* kMachineFG = "MachineFG";
constexpr const char* kInfotainmentFG = "InfotainmentFG";
constexpr const char* kAdasFG = "AdasFG";
constexpr const char* kConnectivityFG = "ConnectivityFG";

// ============================================================================
// State Machine Names
// ============================================================================

/**
 * @brief State Machine identifiers
 */
constexpr const char* kControllerName = "MainController";
constexpr const char* kInfotainmentAgentName = "InfotainmentAgent";
constexpr const char* kAdasAgentName = "AdasAgent";

// ============================================================================
// Network Handle Names
// ============================================================================

/**
 * @brief Network Handle identifiers
 */
constexpr const char* kInternalNetworkHandle = "InternalNetwork";
constexpr const char* kExternalNetworkHandle = "ExternalNetwork";
constexpr const char* kDiagnosticNetworkHandle = "DiagnosticNetwork";

// ============================================================================
// Controller StateMachine Configuration
// ============================================================================

/**
 * @brief Controller state enumeration (extends base State enum)
 */
enum class ControllerState : uint8_t {
    kInitial = 0,           // Mandatory
    kStartup = 1,           // Project-specific: Machine startup
    kRunning = 2,           // Project-specific: Normal operation
    kShutdown = 3,          // Project-specific: Machine shutdown
    kRestart = 4,           // Mandatory for update support
    kPrepareUpdate = 5,     // Mandatory
    kVerifyUpdate = 6,      // Mandatory
    kPrepareRollback = 7,   // Mandatory
    kContinueUpdate = 8,    // Mandatory (after reset during update)
    kAfterUpdate = 9,       // Mandatory (after update session ends)
    kOff = 10               // Final state
};

/**
 * @brief Transition rule for Controller
 * 
 * Maps (currentState, triggerValue) -> nextState
 */
struct ControllerTransitionRule {
    ControllerState fromState;
    uint32_t triggerValue;
    ControllerState toState;
};

/**
 * @brief Controller TransitionRequestTable
 * 
 * @req [SWS_SM_00607] StateMachine transition execution
 * 
 * Defines valid transitions based on RequestTransition() calls
 */
constexpr ControllerTransitionRule kControllerTransitions[] = {
    // Normal operation flow
    {ControllerState::kInitial,  kRequestGoToRunning,    ControllerState::kStartup},
    {ControllerState::kStartup,  kRequestGoToRunning,    ControllerState::kRunning},
    {ControllerState::kRunning,  kRequestGoToShutdown,   ControllerState::kShutdown},
    {ControllerState::kRunning,  kRequestGoToOff,        ControllerState::kOff},
    
    // Update flow
    {ControllerState::kRunning,  kRequestPrepareUpdate,  ControllerState::kPrepareUpdate},
    {ControllerState::kPrepareUpdate, kRequestVerifyUpdate, ControllerState::kVerifyUpdate},
    {ControllerState::kVerifyUpdate, kRequestAfterUpdate, ControllerState::kAfterUpdate},
    {ControllerState::kVerifyUpdate, kRequestPrepareRollback, ControllerState::kPrepareRollback},
    {ControllerState::kPrepareRollback, kRequestAfterUpdate, ControllerState::kAfterUpdate},
    {ControllerState::kAfterUpdate, kRequestGoToRunning, ControllerState::kRunning},
    
    // Continue update after reset
    {ControllerState::kContinueUpdate, kRequestVerifyUpdate, ControllerState::kVerifyUpdate},
    {ControllerState::kContinueUpdate, kRequestAfterUpdate, ControllerState::kAfterUpdate}
};

constexpr size_t kControllerTransitionsCount = 
    sizeof(kControllerTransitions) / sizeof(ControllerTransitionRule);

/**
 * @brief Error recovery rule for Controller
 * 
 * Maps (currentState, executionError) -> recoveryState
 */
struct ControllerErrorRule {
    ControllerState fromState;
    uint32_t executionError;
    ControllerState recoveryState;
};

/**
 * @brief Controller ErrorRecoveryTable
 * 
 * @req [SWS_SM_00601] Error notification reaction
 * @req [SWS_SM_CONSTR_00014] Handling of non-mapped ExecutionError (ANY rule)
 * 
 * Defines recovery actions based on error notifications
 */
constexpr ControllerErrorRule kControllerErrorRecovery[] = {
    // Specific error handling
    {ControllerState::kRunning, kExecutionErrorCrash, ControllerState::kRestart},
    {ControllerState::kRunning, kExecutionErrorSupervision, ControllerState::kShutdown},
    
    // Catch-all rule (mandatory)
    {ControllerState::kRunning, kExecutionErrorAny, ControllerState::kShutdown},
    {ControllerState::kStartup, kExecutionErrorAny, ControllerState::kShutdown}
};

constexpr size_t kControllerErrorRecoveryCount = 
    sizeof(kControllerErrorRecovery) / sizeof(ControllerErrorRule);

// ============================================================================
// Agent StateMachine Configuration (Example: Infotainment)
// ============================================================================

/**
 * @brief Infotainment Agent state enumeration
 */
enum class InfotainmentAgentState : uint8_t {
    kOff = 0,               // Mandatory (Off state for Agent)
    kInitial = 1,           // Mandatory
    kOn = 2,                // Project-specific: Infotainment active
    kPrepareUpdate = 3,     // Mandatory
    kVerifyUpdate = 4,      // Mandatory
    kPrepareRollback = 5    // Mandatory
};

/**
 * @brief Transition rule for Infotainment Agent
 */
struct InfotainmentTransitionRule {
    InfotainmentAgentState fromState;
    uint32_t triggerValue;
    InfotainmentAgentState toState;
};

/**
 * @brief Infotainment Agent TransitionRequestTable
 */
constexpr InfotainmentTransitionRule kInfotainmentTransitions[] = {
    // Normal operation
    {InfotainmentAgentState::kInitial, kRequestGoToRunning, InfotainmentAgentState::kOn},
    {InfotainmentAgentState::kOn,      kRequestGoToOff,     InfotainmentAgentState::kOff},
    {InfotainmentAgentState::kOff,     kRequestGoToRunning, InfotainmentAgentState::kOn},
    
    // Update flow
    {InfotainmentAgentState::kOn,      kRequestPrepareUpdate, InfotainmentAgentState::kPrepareUpdate},
    {InfotainmentAgentState::kPrepareUpdate, kRequestVerifyUpdate, InfotainmentAgentState::kVerifyUpdate},
    {InfotainmentAgentState::kVerifyUpdate, kRequestGoToRunning, InfotainmentAgentState::kOn},
    {InfotainmentAgentState::kVerifyUpdate, kRequestPrepareRollback, InfotainmentAgentState::kPrepareRollback},
    {InfotainmentAgentState::kPrepareRollback, kRequestGoToOff, InfotainmentAgentState::kOff}
};

constexpr size_t kInfotainmentTransitionsCount = 
    sizeof(kInfotainmentTransitions) / sizeof(InfotainmentTransitionRule);

/**
 * @brief Error recovery rule for Infotainment Agent
 */
struct InfotainmentErrorRule {
    InfotainmentAgentState fromState;
    uint32_t executionError;
    InfotainmentAgentState recoveryState;
};

/**
 * @brief Infotainment Agent ErrorRecoveryTable
 */
constexpr InfotainmentErrorRule kInfotainmentErrorRecovery[] = {
    // Specific errors
    {InfotainmentAgentState::kOn, kExecutionErrorCrash, InfotainmentAgentState::kOff},
    
    // Catch-all (mandatory)
    {InfotainmentAgentState::kOn, kExecutionErrorAny, InfotainmentAgentState::kOff}
};

constexpr size_t kInfotainmentErrorRecoveryCount = 
    sizeof(kInfotainmentErrorRecovery) / sizeof(InfotainmentErrorRule);

// ============================================================================
// ActionList Configuration
// ============================================================================

/**
 * @brief Action type enumeration
 * 
 * Types of actions that can be performed when entering a state
 */
enum class ActionType : uint8_t {
    kSetFunctionGroupState,  // [SWS_SM_00608] Request Function Group State
    kStartStateMachine,      // [SWS_SM_00612] Start another StateMachine
    kStopStateMachine,       // [SWS_SM_00614] Stop another StateMachine
    kSync,                   // [SWS_SM_00610] Wait for previous actions
    kSleep,                  // [SWS_SM_00624] Delay execution
    kSetNetworkHandle        // [SWS_SM_00625,00626] Set network state
};

/**
 * @brief Action item definition
 * 
 * Defines a single action to be executed
 */
struct ActionItem {
    ActionType type;
    const char* targetName;      // FG name, SM name, or NetworkHandle name
    const char* targetState;     // Target state (for FG, SM, NM)
    uint32_t delayMs;            // Delay for Sleep action
};

/**
 * @brief ActionList for Controller::Initial state
 * 
 * Actions executed when entering Initial state
 */
constexpr ActionItem kControllerInitialActions[] = {
    {ActionType::kSetFunctionGroupState, kMachineFG, "Startup", 0},
    {ActionType::kSync, nullptr, nullptr, 0}
};

/**
 * @brief ActionList for Controller::Startup state
 */
constexpr ActionItem kControllerStartupActions[] = {
    {ActionType::kSetFunctionGroupState, kMachineFG, "Running", 0},
    {ActionType::kSync, nullptr, nullptr, 0},
    {ActionType::kStartStateMachine, kInfotainmentAgentName, nullptr, 0}
};

/**
 * @brief ActionList for Controller::Running state
 */
constexpr ActionItem kControllerRunningActions[] = {
    {ActionType::kSetFunctionGroupState, kMachineFG, "Running", 0},
    {ActionType::kStartStateMachine, kInfotainmentAgentName, "On", 0},
    {ActionType::kSetNetworkHandle, kExternalNetworkHandle, "FullCom", 0}
};

/**
 * @brief ActionList for Controller::Shutdown state
 */
constexpr ActionItem kControllerShutdownActions[] = {
    {ActionType::kStopStateMachine, kInfotainmentAgentName, nullptr, 0},
    {ActionType::kSync, nullptr, nullptr, 0},
    {ActionType::kSetNetworkHandle, kExternalNetworkHandle, "NoCom", 0},
    {ActionType::kSync, nullptr, nullptr, 0},
    {ActionType::kSetFunctionGroupState, kMachineFG, "Shutdown", 0}
};

/**
 * @brief ActionList for Controller::PrepareUpdate state
 */
constexpr ActionItem kControllerPrepareUpdateActions[] = {
    {ActionType::kStopStateMachine, kInfotainmentAgentName, nullptr, 0},
    {ActionType::kSync, nullptr, nullptr, 0},
    {ActionType::kSetFunctionGroupState, kMachineFG, "Update", 0}
};

/**
 * @brief ActionList for Controller::VerifyUpdate state
 */
constexpr ActionItem kControllerVerifyUpdateActions[] = {
    {ActionType::kStartStateMachine, kInfotainmentAgentName, "VerifyUpdate", 0},
    {ActionType::kSync, nullptr, nullptr, 0},
    {ActionType::kSetFunctionGroupState, kMachineFG, "Verify", 0}
};

/**
 * @brief ActionList for Infotainment::Initial state
 */
constexpr ActionItem kInfotainmentInitialActions[] = {
    {ActionType::kSetFunctionGroupState, kInfotainmentFG, "Off", 0}
};

/**
 * @brief ActionList for Infotainment::On state
 */
constexpr ActionItem kInfotainmentOnActions[] = {
    {ActionType::kSetFunctionGroupState, kInfotainmentFG, "Running", 0}
};

/**
 * @brief ActionList for Infotainment::Off state
 */
constexpr ActionItem kInfotainmentOffActions[] = {
    {ActionType::kSetFunctionGroupState, kInfotainmentFG, "Off", 0}
};

// ============================================================================
// Helper functions for config lookup
// ============================================================================

/**
 * @brief Get action list size
 */
constexpr size_t GetActionListSize(const ActionItem* actions) {
    // In real implementation, this would be stored with the ActionList
    // For now, return a safe default
    return 10; // Maximum actions per state
}

/**
 * @brief Configuration summary
 */
struct ConfigSummary {
    static constexpr const char* version = "1.0";
    static constexpr const char* description = "Static State Management Configuration";
    static constexpr uint8_t numControllers = 1;
    static constexpr uint8_t numAgents = 1;
    static constexpr uint8_t numFunctionGroups = 4;
};

} // namespace config
} // namespace sm
} // namespace ara

#endif // ARA_SM_CONFIG_STATIC_CONFIG_H
