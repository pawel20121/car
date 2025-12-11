#ifndef STATIC_CONFIG_H
#define STATIC_CONFIG_H

#include <cstdint>
#include <cstddef>

namespace ara {
namespace sm {
namespace config {

// ============================================================
// ACTION EXECUTOR CONFIGURATION
// ============================================================

enum class ActionType : uint32_t {
    kSetFunctionGroupState = 0,
    kStartStateMachine     = 1,
    kStopStateMachine      = 2,
    kSync                  = 3,
    kSleep                 = 4,
    kSetNetworkHandle      = 5,
};

struct ActionItem {
    ActionType type;
    const char* targetName;
    const char* targetState;
    uint32_t delayMs;
};

struct StateActionList {
    uint8_t state;             
    const ActionItem* actions; 
    size_t count;              
};

extern const ActionItem* kInitialActions;
extern const ActionItem* kRunningActions;
extern const ActionItem* kOffActions;

extern const size_t kInitialActionsCount;
extern const size_t kRunningActionsCount;
extern const size_t kOffActionsCount;

extern const StateActionList* kActionTable;
extern const size_t kActionTableCount;


// ============================================================
// COMMON TYPES FOR TRANSITIONS & RECOVERY
// ============================================================

enum class StateId : uint8_t {
    kInitial         = 0,
    kOff             = 1,
    kPrepareUpdate   = 2,
    kVerifyUpdate    = 3,
    kPrepareRollback = 4,
    kRunning         = 5,
    kShutdown        = 6
};

constexpr uint32_t kExecutionErrorAny = 0xFFFFFFFF;

struct ErrorRecoveryRule {
    StateId fromState;
    uint32_t executionError;
    StateId recoveryState;
};

extern const ErrorRecoveryRule* kControllerErrorRecovery;
extern const size_t kControllerErrorRecoveryCount;

extern const ErrorRecoveryRule* kInfotainmentErrorRecovery;
extern const size_t kInfotainmentErrorRecoveryCount;


// ============================================================
// TRANSITION TABLE CONFIGURATION
// ============================================================

struct TransitionRule {
    StateId fromState;
    uint32_t triggerValue;
    StateId toState;
};

extern const TransitionRule* kControllerTransitions;
extern const size_t kControllerTransitionsCount;

extern const TransitionRule* kInfotainmentTransitions;
extern const size_t kInfotainmentTransitionsCount;

} // namespace config
} // namespace sm
} // namespace ara

#endif // STATIC_CONFIG_H
