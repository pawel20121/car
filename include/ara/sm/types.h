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
 * Section 9.1 Implementation Data Types
 */

namespace ara {
namespace sm {

/**
 * @brief Transition request value type
 * 
 * @req [SWS_SM_91023] Definition of ImplementationDataType TransitionRequestType
 * 
 * A value which represents the TransitionRequest value to be used in the
 * TransitionRequestTable.
 */
using TransitionRequestType = uint32_t;

/**
 * @brief State machine state name type
 * 
 * @req [SWS_SM_91020] Definition of ImplementationDataType StateMachineStateNameType
 * 
 * A data type used to represent the name of the StateMachine State.
 */
using StateMachineStateNameType = std::string;

/**
 * @brief Function group name type
 * 
 * @req [SWS_SM_91019] Definition of ImplementationDataType FunctionGroupNameType
 * 
 * Full qualified FunctionGroup shortName.
 */
using FunctionGroupNameType = std::string;

/**
 * @brief Function group list type
 * 
 * @req [SWS_SM_91018] Definition of ImplementationDataType FunctionGroupListType
 * 
 * A list of FunctionGroups.
 */
using FunctionGroupListType = std::vector<FunctionGroupNameType>;

/**
 * @brief Update allowed enumeration
 * 
 * @req [SWS_SM_91026] Definition of ImplementationDataType UpdateAllowedType
 */
enum class UpdateAllowedType : uint32_t {
    kUpdateAllowed = 0,     ///< Update is allowed
    kUpdateNotAllowed = 1   ///< Update is not allowed
};

/**
 * @brief Update status enumeration
 * 
 * @req [SWS_SM_91027] Definition of ImplementationDataType UpdateStatusType
 * 
 * Defines the current state of the operation requested through the
 * UpdateRequest service.
 */
enum class UpdateStatusType : uint32_t {
    kIdle = 0,         ///< No request was performed
    kRejected = 1,     ///< Operation was requested outside of update session
    kSuccessful = 2,   ///< Processing successfully finished
    kFailed = 3        ///< Processing failed
};

/**
 * @brief State Management error codes
 * 
 * @req [SWS_SM_91010] Definition of Application Error Domain of functional cluster SM
 * 
 * Error codes returned by State Management operations.
 */
enum class StateManagementErrc : uint32_t {
    kInvalidValue = 10,                      ///< Provided value not mapped to any transition
    kNotAllowedMultipleUpdateSessions = 9,   ///< Only single active session allowed
    kOperationCanceled = 14,                 ///< Request replaced by newer one
    kOperationFailed = 6,                    ///< Requested operation failed
    kOperationRejected = 5,                  ///< Operation rejected due to internal state
    kRecoveryTransitionOngoing = 12,         ///< Recovery currently in progress
    kTransitionFailed = 13,                  ///< Error during state transition
    kTransitionNotAllowed = 11,              ///< Transition not possible from current state
    kUpdateInProgress = 15                   ///< Operation not allowed during update
};

/**
 * @brief Execution error type
 * 
 * Error codes reported by Platform Health Management and Execution Management.
 * Used in ErrorRecoveryTable.
 */
using ExecutionErrorType = uint32_t;

/**
 * @brief Special execution error value for catch-all rules
 * 
 * @req [SWS_SM_CONSTR_00014] Handling of non-mapped ExecutionError
 * 
 * Used in ErrorRecoveryTable to define default error handling.
 */
constexpr ExecutionErrorType kExecutionErrorAny = 0xFFFFFFFF;

/**
 * @brief Network Management state request enumeration
 * 
 * States that can be requested from Network Management.
 */
enum class NmStateRequestEnum : uint8_t {
    kNoCom = 0,    ///< No communication should be possible
    kFullCom = 1   ///< Full communication should be possible
};

/**
 * @brief Special state name for transition state
 * 
 * @req [SWS_SM_CONSTR_00026] Forbidden usage of "inTransition"
 * @req [SWS_SM_00616] CurrentState during transition
 */
constexpr const char* kInTransitionStateName = "inTransition";

/**
 * @brief Converts StateManagementErrc to string
 * 
 * @param errc Error code
 * @return Human-readable error description
 */
inline const char* ErrorCodeToString(StateManagementErrc errc) {
    switch (errc) {
        case StateManagementErrc::kInvalidValue:
            return "Invalid transition request value";
        case StateManagementErrc::kNotAllowedMultipleUpdateSessions:
            return "Multiple update sessions not allowed";
        case StateManagementErrc::kOperationCanceled:
            return "Operation canceled by newer request";
        case StateManagementErrc::kOperationFailed:
            return "Operation failed";
        case StateManagementErrc::kOperationRejected:
            return "Operation rejected due to state";
        case StateManagementErrc::kRecoveryTransitionOngoing:
            return "Error recovery in progress";
        case StateManagementErrc::kTransitionFailed:
            return "State transition failed";
        case StateManagementErrc::kTransitionNotAllowed:
            return "Transition not allowed from current state";
        case StateManagementErrc::kUpdateInProgress:
            return "Update session in progress";
        default:
            return "Unknown error";
    }
}

} // namespace sm
} // namespace ara

#endif // ARA_SM_TYPES_H