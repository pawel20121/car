#include "static_config.h"

namespace ara {
namespace sm {
namespace config {

// ======================================================================
// ACTION LISTS FOR STATES
// ======================================================================

constexpr ActionItem kInitialActionsInternal[] = {
    { ActionType::kSync,  nullptr, nullptr, 0 },
    { ActionType::kSleep, nullptr, nullptr, 10 },
};

constexpr ActionItem kRunningActionsInternal[] = {
    { ActionType::kSetFunctionGroupState, "Engine", "Running", 0 }
};

constexpr ActionItem kOffActionsInternal[] = {
    { ActionType::kSetFunctionGroupState, "Engine", "Off", 0 }
};

// Exported pointers
const ActionItem* kInitialActions = kInitialActionsInternal;
const ActionItem* kRunningActions = kRunningActionsInternal;
const ActionItem* kOffActions     = kOffActionsInternal;

const size_t kInitialActionsCount =
    sizeof(kInitialActionsInternal) / sizeof(ActionItem);

const size_t kRunningActionsCount =
    sizeof(kRunningActionsInternal) / sizeof(ActionItem);

const size_t kOffActionsCount =
    sizeof(kOffActionsInternal) / sizeof(ActionItem);


// ======================================================================
// STATE → ACTION TABLE
// ======================================================================
// state values = StateMachine::State enum:
//
// Initial = 0
// Off = 1
// Running = 5
//

constexpr StateActionList kActionTableInternal[] = {
    { 0, kInitialActionsInternal,  kInitialActionsCount }, // Initial
    { 5, kRunningActionsInternal,  kRunningActionsCount }, // Running
    { 1, kOffActionsInternal,      kOffActionsCount },     // Off
};

const StateActionList* kActionTable = kActionTableInternal;

const size_t kActionTableCount =
    sizeof(kActionTableInternal) / sizeof(StateActionList);


// ======================================================================
// ERROR RECOVERY TABLES
// ======================================================================

// Controller rules
constexpr ErrorRecoveryRule kControllerErrorRecoveryInternal[] = {
    { StateId::kInitial,        kExecutionErrorAny, StateId::kOff },
    { StateId::kOff,            1,                  StateId::kRunning },
    { StateId::kOff,            kExecutionErrorAny, StateId::kInitial }
};

const ErrorRecoveryRule* kControllerErrorRecovery =
    kControllerErrorRecoveryInternal;

const size_t kControllerErrorRecoveryCount =
    sizeof(kControllerErrorRecoveryInternal) / sizeof(ErrorRecoveryRule);


// Infotainment rules
constexpr ErrorRecoveryRule kInfotainmentErrorRecoveryInternal[] = {
    { StateId::kInitial, kExecutionErrorAny, StateId::kRunning },
    { StateId::kRunning, 5,                  StateId::kInitial }
};

const ErrorRecoveryRule* kInfotainmentErrorRecovery =
    kInfotainmentErrorRecoveryInternal;

const size_t kInfotainmentErrorRecoveryCount =
    sizeof(kInfotainmentErrorRecoveryInternal) / sizeof(ErrorRecoveryRule);


// ======================================================================
// TRANSITION TABLES
// ======================================================================

// Controller example transitions
constexpr TransitionRule kControllerTransitionsInternal[] = {
    { StateId::kInitial,         1, StateId::kPrepareUpdate },
    { StateId::kPrepareUpdate,   2, StateId::kVerifyUpdate },
    { StateId::kVerifyUpdate,    3, StateId::kPrepareRollback },
    { StateId::kPrepareRollback, 4, StateId::kInitial }
};

const TransitionRule* kControllerTransitions =
    kControllerTransitionsInternal;

const size_t kControllerTransitionsCount =
    sizeof(kControllerTransitionsInternal) / sizeof(TransitionRule);


// Agent/infotainment transitions used by tests
constexpr TransitionRule kInfotainmentTransitionsInternal[] = {
    { StateId::kInitial, 1, StateId::kRunning }, // required by tests
    { StateId::kRunning, 2, StateId::kOff },
    { StateId::kOff,     3, StateId::kInitial }
};

const TransitionRule* kInfotainmentTransitions =
    kInfotainmentTransitionsInternal;

const size_t kInfotainmentTransitionsCount =
    sizeof(kInfotainmentTransitionsInternal) / sizeof(TransitionRule);

} // namespace config
} // namespace sm
} // namespace ara
