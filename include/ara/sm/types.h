#ifndef ARA_SM_TYPES_H
#define ARA_SM_TYPES_H

#include <cstdint>
#include <string>
#include <vector>

/**
 * @file types.h
 * @brief Type definitions for AUTOSAR Adaptive Platform State Management
 * 
 * Based on AUTOSAR AP R24-11 State Management Specification
 * Document ID 908: AUTOSAR_AP_SWS_StateManagement
 * Section 9.1 Implementation Data Types
 * 
 * This file provides C++ type definitions corresponding to the AUTOSAR
 * meta-model Implementation Data Types used in State Management.
 */

namespace ara {
namespace sm {

// ============================================================================
// Basic Data Types (Section 9.1.2 and 9.1.3)
// ============================================================================

/**
 * @brief Transition request value type
 * 
 * @req [SWS_SM_91023] Definition of ImplementationDataType TransitionRequestType
 * 
 * A value which represents the TransitionRequest value to be used in the
 * TransitionRequestTable. This value is passed by SMControlApplication
 * to request state machine state transitions.
 * 
 * The actual values are project-specific and defined in the state machine
 * configuration (TransitionRequestTable).
 */
using TransitionRequestType = uint32_t;

/**
 * @brief State machine state name type
 * 
 * @req [SWS_SM_91020] Definition of ImplementationDataType StateMachineStateNameType
 * 
 * A data type used to represent the name of the StateMachine State.
 * Used in StateMachineNotification interface to publish current state.
 * 
 * Special value: "inTransition" indicates state transition in progress
 * @see kInTransitionStateName
 */
using StateMachineStateNameType = std::string;

/**
 * @brief Function group name type
 * 
 * @req [SWS_SM_91019] Definition of ImplementationDataType FunctionGroupNameType
 * 
 * Full qualified FunctionGroup shortName.
 * Used in Update and Configuration Management interactions to identify
 * which function groups are affected by update operations.
 * 
 * Example: "/FunctionGroups/CameraFG"
 */
using FunctionGroupNameType = std::string;

/**
 * @brief Function group list type
 * 
 * @req [SWS_SM_91018] Definition of ImplementationDataType FunctionGroupListType
 * 
 * A list of FunctionGroups passed to PrepareUpdate, VerifyUpdate,
 * and PrepareRollback methods to identify which function groups
 * are affected by an update session.
 */
using FunctionGroupListType = std::vector<FunctionGroupNameType>;

// ============================================================================
// Update Management Types (Section 9.1.4 and 9.1.5)
// ============================================================================

/**
 * @brief Update allowed enumeration
 * 
 * @req [SWS_SM_91026] Definition of ImplementationDataType UpdateAllowedType
 * 
 * Set by SMControlApplication via UpdateAllowedService interface to
 * indicate whether an update session can be started. State Management
 * evaluates this during RequestUpdateSession call.
 */
enum class UpdateAllowedType : uint32_t {
    kUpdateAllowed = 0,     ///< Update is allowed - State Management will accept RequestUpdateSession
    kUpdateNotAllowed = 1   ///< Update is not allowed - State Management will reject RequestUpdateSession
};

/**
 * @brief Update status enumeration
 * 
 * @req [SWS_SM_91027] Definition of ImplementationDataType UpdateStatusType
 * 
 * Defines the current state of the operation requested through the
 * UpdateRequest service. Published via ResetMachineNotifier field
 * to track the status of machine reset during update.
 */
enum class UpdateStatusType : uint32_t {
    kIdle = 0,         ///< No request was performed (default value)
    kRejected = 1,     ///< Operation was requested outside of update session
    kSuccessful = 2,   ///< Processing successfully finished
    kFailed = 3        ///< Processing failed
};

// ============================================================================
// State Management Error Domain (Section 9.4.1)
// ============================================================================

/**
 * @brief State Management error codes
 * 
 * @req [SWS_SM_91010] Definition of Application Error Domain of functional cluster SM
 * 
 * Error codes returned by State Management service interface operations.
 * These errors are part of the ara::core error handling framework and
 * can be returned via ara::core::Result<T, E> or Future<T, E>.
 */
enum class StateManagementErrc : uint32_t {
    /**
     * The provided TransitionRequest value is not mapped to any transition
     * in the TransitionRequestTable for the current state.
     * Returned by: RequestTransition
     */
    kInvalidValue = 10,
    
    /**
     * Request for new update session rejected because only a single
     * active update session is allowed at any time.
     * Returned by: RequestUpdateSession
     */
    kNotAllowedMultipleUpdateSessions = 9,
    
    /**
     * The request was replaced by a newer RequestTransition call
     * and therefore was cancelled.
     * Returned by: RequestTransition (to previous pending request)
     */
    kOperationCanceled = 14,
    
    /**
     * Requested operation failed during execution.
     * This can occur during ActionListItem processing.
     * Returned by: PrepareUpdate, VerifyUpdate, PrepareRollback
     */
    kOperationFailed = 6,
    
    /**
     * Operation rejected due to State Management's internal state.
     * Examples: calling update methods outside active update session,
     * calling RequestUpdateSession when UpdateAllowed is false.
     * Returned by: RequestUpdateSession, PrepareUpdate, VerifyUpdate,
     *              PrepareRollback, StopUpdateSession, ResetMachine
     */
    kOperationRejected = 5,
    
    /**
     * Request will not be carried out because error recovery
     * is currently ongoing (ErrorRecoveryOngoing flag set).
     * Returned by: RequestTransition
     */
    kRecoveryTransitionOngoing = 12,
    
    /**
     * An error occurred during processing of ActionListItems
     * in the requested state transition.
     * Returned by: RequestTransition
     */
    kTransitionFailed = 13,
    
    /**
     * Requested transition is not possible from the current
     * StateMachine state according to TransitionRequestTable.
     * Returned by: RequestTransition
     */
    kTransitionNotAllowed = 11,
    
    /**
     * Requested operation is not allowed because an update session
     * is currently in progress and the state machine is marked
     * as "ImpactedByUpdate".
     * Returned by: RequestTransition
     */
    kUpdateInProgress = 15
};

// ============================================================================
// Execution Error Handling
// ============================================================================

/**
 * @brief Execution error type
 * 
 * Error codes reported by Platform Health Management (via RecoveryHandler)
 * and Execution Management to State Management. These values are matched
 * against the ErrorRecoveryTable to determine appropriate recovery actions.
 * 
 * The actual error values are defined in ProcessExecutionError configuration
 * and are project-specific.
 */
using ExecutionErrorType = uint32_t;

/**
 * @brief Special execution error value for catch-all rules
 * 
 * @req [SWS_SM_CONSTR_00014] Handling of non-mapped ExecutionError
 * 
 * Each ErrorRecoveryTable shall have exactly one entry configured with
 * value ANY as the ExecutionError. This entry defines the default
 * recovery action for any error not explicitly mapped.
 */
constexpr ExecutionErrorType kExecutionErrorAny = 0xFFFFFFFF;

// ============================================================================
// Network Management Integration
// ============================================================================

/**
 * @brief Network Management state request enumeration
 * 
 * States that can be requested from Network Management via
 * SetNetworkHandle ActionListItem, or received as triggers
 * via NetworkHandle state changes.
 * 
 * Defined in: NmStateRequestEnum (TPS Manifest)
 */
enum class NmStateRequestEnum : uint8_t {
    /**
     * No communication should be possible.
     * Used in SetNetworkHandle ActionListItem to request NoCom state.
     */
    kNoCom = 0,
    
    /**
     * Full communication should be possible.
     * Used in SetNetworkHandle ActionListItem to request FullCom state.
     */
    kFullCom = 1
};

// ============================================================================
// StateMachine Special Values and Constants
// ============================================================================

/**
 * @brief Special state name for transition state
 * 
 * @req [SWS_SM_CONSTR_00026] Forbidden usage of "inTransition"
 * @req [SWS_SM_00616] CurrentState value during StateMachine State transition
 * 
 * When StateMachineNotification interface is configured and a state
 * transition has started, the CurrentState field is set to "inTransition".
 * 
 * Note: This value is reserved and must not be used as a StateMachine
 * State name in configuration.
 */
constexpr const char* kInTransitionStateName = "inTransition";

// ============================================================================
// StateMachine Categories
// ============================================================================

/**
 * @brief StateMachine category enumeration
 * 
 * Defines the role/category of a StateMachine instance.
 * Set via StateManagementStateNotification.stateMachine.category
 * 
 * @req [SWS_SM_CONSTR_00031] Exactly one Controller must be configured
 */
enum class StateMachineCategoryType : uint8_t {
    /**
     * Controller StateMachine - manages machine lifecycle and Agents.
     * Exactly one instance must be configured per machine.
     * Can use ActionListItems to start/stop Agent StateMachines.
     * Must manage MachineFG (Machine Function Group).
     */
    kController = 0,
    
    /**
     * Agent StateMachine - manages subset of Function Groups.
     * Arbitrary number of instances can be configured.
     * Cannot start/stop other StateMachines.
     * Must have "Off" state defined.
     */
    kAgent = 1
};

// ============================================================================
// Mandatory StateMachine States
// ============================================================================

/**
 * @brief Mandatory StateMachine state names
 * 
 * These states are required by AUTOSAR specification for proper
 * update and error recovery functionality.
 */
namespace MandatoryStates {
    /**
     * @req [SWS_SM_CONSTR_00024] Each Agent must have "Off" state
     * Used when stopping an Agent StateMachine to ensure clean shutdown.
     */
    constexpr const char* kOff = "Off";
    
    /**
     * @req [SWS_SM_CONSTR_00021] Each StateMachine must have PrepareUpdate state
     * State entered when UCM calls PrepareUpdate method.
     */
    constexpr const char* kPrepareUpdate = "PrepareUpdate";
    
    /**
     * @req [SWS_SM_CONSTR_00022] Each StateMachine must have VerifyUpdate state
     * State entered when UCM calls VerifyUpdate method.
     */
    constexpr const char* kVerifyUpdate = "VerifyUpdate";
    
    /**
     * @req [SWS_SM_CONSTR_00023] Each StateMachine must have PrepareRollback state
     * State entered when UCM calls PrepareRollback method.
     */
    constexpr const char* kPrepareRollback = "PrepareRollback";
    
    /**
     * @req [SWS_SM_CONSTR_00028] Controller must have ContinueUpdate state
     * State entered when Controller starts during active update session.
     */
    constexpr const char* kContinueUpdate = "ContinueUpdate";
    
    /**
     * @req [SWS_SM_CONSTR_00029] Controller must have Restart state
     * State entered when UCM calls ResetMachine method.
     */
    constexpr const char* kRestart = "Restart";
    
    /**
     * @req [SWS_SM_CONSTR_00027] Controller must have AfterUpdate state
     * State entered when UCM calls StopUpdateSession method.
     */
    constexpr const char* kAfterUpdate = "AfterUpdate";
}

// ============================================================================
// Platform Health Management Integration
// ============================================================================

/**
 * @brief Recovery handler return type
 * 
 * Values returned by State Management's RecoveryHandler callback
 * to Platform Health Management.
 */
enum class RecoveryHandlerReturnType : uint8_t {
    /**
     * State Management can handle the recovery in a controlled manner.
     * PHM will wait for State Management to complete recovery actions.
     */
    kSMCanHandleRecovery = 0,
    
    /**
     * @req [SWS_SM_00030] RecoveryHandler can not be handled
     * State Management cannot handle the recovery (e.g., unknown FunctionGroup).
     * PHM should fire watchdog reaction as last resort.
     */
    kSMCanNotHandleRecovery = 1
};

// ============================================================================
// Execution Management Integration
// ============================================================================

/**
 * @brief Common Execution Management error codes
 * 
 * These are examples of ExecErrc values that Execution Management
 * might return during Function Group State transitions.
 * Actual values depend on Execution Management implementation.
 */
namespace ExecErrors {
    /// State transition successful
    constexpr uint32_t kSuccess = 0;
    
    /// Generic unrecoverable error
    constexpr uint32_t kGeneralError = 1;
    
    /// Communication error during state transition
    constexpr uint32_t kCommunicationError = 2;
    
    /// Integrity or authenticity check failed (compromised platform)
    constexpr uint32_t kIntegrityOrAuthenticityCheckFailed = 3;
    
    /// Meta-model configuration error
    constexpr uint32_t kMetaModelError = 4;
}

// ============================================================================
// Helper Functions
// ============================================================================

/**
 * @brief Converts StateManagementErrc to human-readable string
 * 
 * @param errc Error code
 * @return Human-readable error description
 */
inline const char* ErrorCodeToString(StateManagementErrc errc) {
    switch (errc) {
        case StateManagementErrc::kInvalidValue:
            return "Invalid transition request value - not found in TransitionRequestTable";
        case StateManagementErrc::kNotAllowedMultipleUpdateSessions:
            return "Multiple update sessions not allowed - session already active";
        case StateManagementErrc::kOperationCanceled:
            return "Operation canceled by newer request";
        case StateManagementErrc::kOperationFailed:
            return "Operation failed during execution";
        case StateManagementErrc::kOperationRejected:
            return "Operation rejected due to State Management internal state";
        case StateManagementErrc::kRecoveryTransitionOngoing:
            return "Error recovery in progress - transition not allowed";
        case StateManagementErrc::kTransitionFailed:
            return "State transition failed during ActionListItem processing";
        case StateManagementErrc::kTransitionNotAllowed:
            return "Transition not allowed from current state";
        case StateManagementErrc::kUpdateInProgress:
            return "Update session in progress - state machine is ImpactedByUpdate";
        default:
            return "Unknown State Management error";
    }
}

/**
 * @brief Converts UpdateStatusType to string
 * 
 * @param status Update status
 * @return Human-readable status description
 */
inline const char* UpdateStatusToString(UpdateStatusType status) {
    switch (status) {
        case UpdateStatusType::kIdle:
            return "Idle - no operation requested";
        case UpdateStatusType::kRejected:
            return "Rejected - operation outside update session";
        case UpdateStatusType::kSuccessful:
            return "Successful - operation completed";
        case UpdateStatusType::kFailed:
            return "Failed - operation encountered error";
        default:
            return "Unknown update status";
    }
}

/**
 * @brief Converts UpdateAllowedType to string
 * 
 * @param allowed Update allowed value
 * @return Human-readable description
 */
inline const char* UpdateAllowedToString(UpdateAllowedType allowed) {
    switch (allowed) {
        case UpdateAllowedType::kUpdateAllowed:
            return "Update allowed";
        case UpdateAllowedType::kUpdateNotAllowed:
            return "Update not allowed";
        default:
            return "Unknown update allowed value";
    }
}

/**
 * @brief Converts NmStateRequestEnum to string
 * 
 * @param state Network management state
 * @return Human-readable state description
 */
inline const char* NmStateToString(NmStateRequestEnum state) {
    switch (state) {
        case NmStateRequestEnum::kNoCom:
            return "NoCom - no communication";
        case NmStateRequestEnum::kFullCom:
            return "FullCom - full communication";
        default:
            return "Unknown network management state";
    }
}

/**
 * @brief Converts StateMachineCategoryType to string
 * 
 * @param category StateMachine category
 * @return Human-readable category description
 */
inline const char* StateMachineCategoryToString(StateMachineCategoryType category) {
    switch (category) {
        case StateMachineCategoryType::kController:
            return "Controller - manages machine lifecycle";
        case StateMachineCategoryType::kAgent:
            return "Agent - manages function group subset";
        default:
            return "Unknown state machine category";
    }
}

/**
 * @brief Checks if a state name is reserved
 * 
 * @param stateName State name to check
 * @return true if state name is reserved and cannot be used
 */
inline bool IsReservedStateName(const std::string& stateName) {
    return stateName == kInTransitionStateName;
}

/**
 * @brief Checks if error code indicates a fatal condition
 * 
 * @param errc Error code
 * @return true if error indicates unrecoverable condition
 */
inline bool IsFatalError(StateManagementErrc errc) {
    switch (errc) {
        case StateManagementErrc::kOperationFailed:
        case StateManagementErrc::kTransitionFailed:
            return true;
        default:
            return false;
    }
}

} // namespace sm
} // namespace ara

#endif // ARA_SM_TYPES_H