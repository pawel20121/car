#include "static_config.h"

namespace ara {
namespace sm {
namespace config {

// ======================================================================
// ACTION LISTS
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

constexpr StateActionList kActionTableInternal[] = {
    { static_cast<uint8_t>(StateId::kInitial), kInitialActionsInternal,  kInitialActionsCount },
    { static_cast<uint8_t>(StateId::kRunning), kRunningActionsInternal,  kRunningActionsCount },
    { static_cast<uint8_t>(StateId::kOff),     kOffActionsInternal,      kOffActionsCount },
};

const StateActionList* kActionTable = kActionTableInternal;

const size_t kActionTableCount =
    sizeof(kActionTableInternal) / sizeof(StateActionList);


// ======================================================================
// ERROR RECOVERY TABLES
// ======================================================================

constexpr ErrorRecoveryRule kControllerErrorRecoveryInternal[] = {
    { StateId::kInitial, kExecutionErrorAny, StateId::kOff },
    { StateId::kOff,     1,                  StateId::kRunning },
    { StateId::kOff,     kExecutionErrorAny, StateId::kInitial }
};

const ErrorRecoveryRule* kControllerErrorRecovery =
    kControllerErrorRecoveryInternal;

const size_t kControllerErrorRecoveryCount =
    sizeof(kControllerErrorRecoveryInt
