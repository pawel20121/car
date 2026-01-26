#include "static_config.h"

/**
 * @file static_config_helpers.cpp
 * @brief Implementation of helper functions for static configuration
 */

namespace ara {
namespace sm {
namespace config {

const char* StateIdToString(uint32_t stateId) {
    switch (stateId) {
        case States::kInitial:
            return "Initial";
        case States::kOff:
            return "Off";
        case States::kRunning:
            return "Running";
        case States::kPrepareUpdate:
            return "PrepareUpdate";
        case States::kVerifyUpdate:
            return "VerifyUpdate";
        case States::kPrepareRollback:
            return "PrepareRollback";
        case States::kStartup:
            return "Startup";
        case States::kShutdown:
            return "Shutdown";
        case States::kRestart:
            return "Restart";
        case States::kContinueUpdate:
            return "ContinueUpdate";
        case States::kAfterUpdate:
            return "AfterUpdate";
        case States::kDegraded:
            return "Degraded";
        case States::kInTransition:
            return "InTransition";
        case States::kInvalid:
            return "Invalid";
        default:
            return "Unknown";
    }
}

const char* TriggerIdToString(TransitionRequestType triggerId) {
    switch (triggerId) {
        case Triggers::kStartup:
            return "Startup";
        case Triggers::kShutdownRequest:
            return "ShutdownRequest";
        case Triggers::kRestartRequest:
            return "RestartRequest";
        case Triggers::kGoToRunning:
            return "GoToRunning";
        case Triggers::kPrepareUpdateRequest:
            return "PrepareUpdateRequest";
        case Triggers::kVerifyUpdateRequest:
            return "VerifyUpdateRequest";
        case Triggers::kPrepareRollbackRequest:
            return "PrepareRollbackRequest";
        case Triggers::kFinishUpdateRequest:
            return "FinishUpdateRequest";
        case Triggers::kNetworkFullCom:
            return "NetworkFullCom";
        case Triggers::kNetworkNoCom:
            return "NetworkNoCom";
        case Triggers::kUserRequest:
            return "UserRequest";
        case Triggers::kDegradeRequest:
            return "DegradeRequest";
        default:
            return "Unknown";
    }
}

const char* ActionTypeToString(ActionType type) {
    switch (type) {
        case ActionType::kSetFunctionGroupState:
            return "SetFunctionGroupState";
        case ActionType::kStartStateMachine:
            return "StartStateMachine";
        case ActionType::kStopStateMachine:
            return "StopStateMachine";
        case ActionType::kSync:
            return "Sync";
        case ActionType::kSleep:
            return "Sleep";
        case ActionType::kSetNetworkHandle:
            return "SetNetworkHandle";
        default:
            return "Unknown";
    }
}

} // namespace config
} // namespace sm
} // namespace ara