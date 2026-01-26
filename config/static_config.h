#ifndef STATIC_CONFIG_H
#define STATIC_CONFIG_H

#include <cstdint>
#include <cstddef>
#include "types.h"
#include <cstddef>

/**
 * @file static_config.h
 * @brief Static configuration declarations for State Management
 * 
 * This file contains the type definitions and declarations for static
 * configuration of State Management. The actual configuration data
 * is defined in static_config.cpp
 * 
 * Configuration Structure:
 * - Controller StateMachine: Manages Machine lifecycle and Agents
 * - Agent StateMachine (Infotainment): Manages application functionality
 */

namespace ara {
namespace sm {
namespace config {

// ============================================================================
// PREDEFINED STATE IDs
// ============================================================================

/**
 * @brief Namespace containing all possible state IDs
 * 
 * States are represented as uint32_t for efficient comparison.
 * The actual state names are stored separately for debugging/logging.
 */
namespace States {
    // Common states (used by both Controller and Agent)
    constexpr uint32_t kInitial = 0;           ///< Initial state (mandatory)
    constexpr uint32_t kOff = 1;               ///< Off state (mandatory for Agent)
    constexpr uint32_t kRunning = 2;           ///< Normal running state
    
    // Update-related states (mandatory for all StateMachines)
    constexpr uint32_t kPrepareUpdate = 10;    ///< @req [SWS_SM_CONSTR_00021]
    constexpr uint32_t kVerifyUpdate = 11;     ///< @req [SWS_SM_CONSTR_00022]
    constexpr uint32_t kPrepareRollback = 12;  ///< @req [SWS_SM_CONSTR_00023]
    
    // Controller-specific states
    constexpr uint32_t kStartup = 20;          ///< Machine startup
    constexpr uint32_t kShutdown = 21;         ///< Machine shutdown
    constexpr uint32_t kRestart = 22;          ///< Machine restart @req [SWS_SM_CONSTR_00029]
    constexpr uint32_t kContinueUpdate = 23;   ///< Continue after restart @req [SWS_SM_CONSTR_00028]
    constexpr uint32_t kAfterUpdate = 24;      ///< After update session @req [SWS_SM_CONSTR_00027]
    
    // Agent-specific states (example)
    constexpr uint32_t kDegraded = 30;         ///< Degraded operation mode
    
    // Special state for internal use
    constexpr uint32_t kInTransition = 0xFFFFFFFE; ///< @req [SWS_SM_00616]
    constexpr uint32_t kInvalid = 0xFFFFFFFF;      ///< Invalid/uninitialized state
}

// ============================================================================
// PREDEFINED TRIGGER IDs
// ============================================================================

/**
 * @brief Namespace containing all possible trigger/request IDs
 * 
 * Triggers are values sent by SMControlApplication via RequestTransition
 * or internal events (network state changes, errors, etc.)
 */
namespace Triggers {
    // Lifecycle triggers
    constexpr TransitionRequestType kStartup = 1;
    constexpr TransitionRequestType kShutdownRequest = 2;
    constexpr TransitionRequestType kRestartRequest = 3;
    constexpr TransitionRequestType kGoToRunning = 4;
    
    // Update-related triggers (from UCM)
    constexpr TransitionRequestType kPrepareUpdateRequest = 10;
    constexpr TransitionRequestType kVerifyUpdateRequest = 11;
    constexpr TransitionRequestType kPrepareRollbackRequest = 12;
    constexpr TransitionRequestType kFinishUpdateRequest = 13;
    
    // Network Management triggers
    constexpr TransitionRequestType kNetworkFullCom = 20;
    constexpr TransitionRequestType kNetworkNoCom = 21;
    
    // Application-specific triggers
    constexpr TransitionRequestType kUserRequest = 100;
    constexpr TransitionRequestType kDegradeRequest = 101;
}

// ============================================================================
// PREDEFINED EXECUTION ERROR CODES
// ============================================================================

/**
 * @brief Namespace containing execution error codes
 * 
 * These are error codes reported by PHM and EM to trigger recovery actions
 */
namespace ExecutionErrors {
    constexpr ExecutionErrorType kProcessCrashed = 1;
    constexpr ExecutionErrorType kCheckpointViolation = 2;
    constexpr ExecutionErrorType kMemoryViolation = 3;
    constexpr ExecutionErrorType kCommunicationError = 4;
    constexpr ExecutionErrorType kUpdateFailed = 10;
    constexpr ExecutionErrorType kVerificationFailed = 11;
}

// ============================================================================
// CONFIGURATION STRUCTURES
// ============================================================================

/**
 * @brief Transition rule entry in TransitionRequestTable
 * @req [SWS_SM_00603-00607]
 * 
 * Defines allowed state transitions based on current state and trigger
 */
struct TransitionRule {
    uint32_t fromState;                 ///< Current state
    TransitionRequestType trigger;      ///< Trigger/request value
    uint32_t toState;                   ///< Target state
};

/**
 * @brief Error recovery rule entry in ErrorRecoveryTable
 * @req [SWS_SM_00601], [SWS_SM_CONSTR_00014]
 * 
 * Defines recovery actions based on current state and error code
 */
struct ErrorRecoveryRule {
    uint32_t fromState;                 ///< Current state when error occurred
    ExecutionErrorType errorCode;       ///< Error code from PHM/EM
    uint32_t toState;                   ///< Recovery state to transition to
};

/**
 * @brief Action item type enumeration
 * @req [SWS_SM_00608-00626]
 */
enum class ActionType : uint8_t {
    kSetFunctionGroupState = 0,         ///< @req [SWS_SM_00608]
    kStartStateMachine = 1,             ///< @req [SWS_SM_00612-00613]
    kStopStateMachine = 2,              ///< @req [SWS_SM_00614-00615]
    kSync = 3,                          ///< @req [SWS_SM_00610]
    kSleep = 4,                         ///< @req [SWS_SM_00624]
    kSetNetworkHandle = 5               ///< @req [SWS_SM_00625-00626]
};

/**
 * @brief Single action item in ActionList
 * @req [SWS_SM_00608-00626]
 * 
 * Represents one action to be executed when entering a state
 */
struct ActionItem {
    ActionType type;                    ///< Type of action
    const char* target;                 ///< Target (FG name, SM name, NetworkHandle name)
    const char* param;                  ///< Parameter (FG state, SM initial state, NM state)
    uint32_t sleepTimeMs;              ///< Sleep duration in ms (for kSleep only)
};

/**
 * @brief Action list entry mapping state to actions
 * @req [SWS_SM_00609], [SWS_SM_CONSTR_00015]
 */
struct ActionListEntry {
    uint32_t state;                     ///< State ID
    const ActionItem* actions;          ///< Pointer to action array
    size_t actionCount;                 ///< Number of actions in array
};

// ============================================================================
// EXTERNAL CONFIGURATION DATA DECLARATIONS
// ============================================================================

// Controller configuration
extern const TransitionRule kControllerTransitions[];
extern const size_t kControllerTransitionsCount;

extern const ErrorRecoveryRule kControllerErrorRecovery[];
extern const size_t kControllerErrorRecoveryCount;

// Agent (Infotainment) configuration
extern const TransitionRule kInfotainmentTransitions[];
extern const size_t kInfotainmentTransitionsCount;

extern const ErrorRecoveryRule kInfotainmentErrorRecovery[];
extern const size_t kInfotainmentErrorRecoveryCount;

// Action table
extern const ActionListEntry kActionTable[];
extern const size_t kActionTableCount;

// ============================================================================
// HELPER FUNCTIONS
// ============================================================================

/**
 * @brief Convert state ID to string name
 * @param stateId State ID
 * @return Human-readable state name
 */
const char* StateIdToString(uint32_t stateId);

/**
 * @brief Convert trigger ID to string name
 * @param triggerId Trigger ID
 * @return Human-readable trigger name
 */
const char* TriggerIdToString(TransitionRequestType triggerId);

/**
 * @brief Convert action type to string
 * @param type Action type
 * @return Human-readable action type name
 */
const char* ActionTypeToString(ActionType type);

/**
 * @brief Check if state is a mandatory update state
 * @param stateId State ID to check
 * @return true if state is PrepareUpdate, VerifyUpdate, or PrepareRollback
 */
inline bool IsMandatoryUpdateState(uint32_t stateId) {
    return (stateId == States::kPrepareUpdate ||
            stateId == States::kVerifyUpdate ||
            stateId == States::kPrepareRollback);
}

/**
 * @brief Check if state is Controller-specific
 * @param stateId State ID to check
 * @return true if state is only for Controller
 */
inline bool IsControllerOnlyState(uint32_t stateId) {
    return (stateId == States::kStartup ||
            stateId == States::kShutdown ||
            stateId == States::kRestart ||
            stateId == States::kContinueUpdate ||
            stateId == States::kAfterUpdate);
}

/**
 * @brief Get initial state for Controller
 * @return Initial state ID for Controller StateMachine
 */
inline uint32_t GetControllerInitialState() {
    return States::kInitial;
}

/**
 * @brief Get initial state for Agent
 * @return Initial state ID for Agent StateMachine
 */
inline uint32_t GetAgentInitialState() {
    return States::kInitial;
}

} // namespace config
} // namespace sm
} // namespace ara

#endif // STATIC_CONFIG_H
