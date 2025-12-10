#ifndef STATIC_CONFIG_H
#define STATIC_CONFIG_H

#include <cstdint>
#include <cstddef>

namespace ara {
namespace sm {
namespace config {

// ============================================================
//  ACTION EXECUTOR CONFIGURATION
// ============================================================

enum class ActionType : uint32_t
{
    kSetFunctionGroupState = 0,
    kStartStateMachine     = 1,
    kStopStateMachine      = 2,
    kSync                  = 3,
    kSleep                 = 4,
    kSetNetworkHandle      = 5,
};

struct ActionItem
{
    ActionType type;
    const char* targetName;
    const char* targetState;
    uint32_t delayMs;
};

// ============================================================
//  COMMON TYPES FOR TRANSITIONS & RECOVERY
// ============================================================

enum class StateId : uint8_t {
    kState0 = 0,
    kState1 = 1,
    kState2 = 2,
    kState3 = 3
};

constexpr uint32_t kExecutionErrorAny = 0xFFFFFFFF;

// ============================================================
//  ERROR RECOVERY CONFIGURATION
// ============================================================

struct ErrorRecoveryRule
{
    StateId fromState;
    uint32_t executionError;
    StateId recoveryState;
};

constexpr ErrorRecoveryRule kControllerErrorRecovery[] = {
    { StateId::kState0, kExecutionErrorAny, StateId::kState1 },
    { StateId::kState1, 1,                 StateId::kState2 },
    { StateId::kState1, kExecutionErrorAny, StateId::kState0 },
};

constexpr size_t kControllerErrorRecoveryCount =
    sizeof(kControllerErrorRecovery) / sizeof(ErrorRecoveryRule);

constexpr ErrorRecoveryRule kInfotainmentErrorRecovery[] = {
    { StateId::kState0, kExecutionErrorAny, StateId::kState3 },
    { StateId::kState3, 5,                 StateId::kState0 }
};

constexpr size_t kInfotainmentErrorRecoveryCount =
    sizeof(kInfotainmentErrorRecovery) / sizeof(ErrorRecoveryRule);

// ============================================================
//  TRANSITION TABLE CONFIGURATION
// ============================================================

struct TransitionRule
{
    StateId fromState;
    uint32_t triggerValue;    // Equivalent to TransitionRequestType
    StateId toState;
};

// Example Controller transitions
constexpr TransitionRule kControllerTransitions[] = {
    { StateId::kState0, 1, StateId::kState1 },
    { StateId::kState1, 2, StateId::kState2 },
    { StateId::kState2, 3, StateId::kState3 },
    { StateId::kState3, 4, StateId::kState0 },
};

constexpr size_t kControllerTransitionsCount =
    sizeof(kControllerTransitions) / sizeof(TransitionRule);

// Example Infotainment transitions
constexpr TransitionRule kInfotainmentTransitions[] = {
    { StateId::kState0, 10, StateId::kState1 },
    { StateId::kState1, 11, StateId::kState2 },
    { StateId::kState2, 12, StateId::kState3 },
};

constexpr size_t kInfotainmentTransitionsCount =
    sizeof(kInfotainmentTransitions) / sizeof(TransitionRule);

} // namespace config
} // namespace sm
} // namespace ara

#endif // STATIC_CONFIG_H
