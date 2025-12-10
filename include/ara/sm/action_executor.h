/*
 * action_executor.h
 *
 * Simple ActionExecutor interface for the State Management module (Adaptive).
 *
 * AUTOSAR Adaptive Platform - State Management (SWS) implementation for
 * academic project (SWE.3 / SWE.4).
 *
 * Specification used: AUTOSAR Adaptive Platform R21-11 (or newer).
 *
 * Author: <Twoje Imię>
 * Date:   2025-12-10
 */

#ifndef ARA_SM_ACTION_EXECUTOR_H
#define ARA_SM_ACTION_EXECUTOR_H

#include <cstddef>   // size_t
#include <cstdint>   // uint32_t
// #include "config/static_config.h"
// #include "../../config/static_config.h"
#include "static_config.h"
namespace ara {
namespace sm {

// Forward declarations for config types (actual definitions live in config headers)
// namespace config {
//     enum class ActionType : uint32_t;
//     struct ActionItem;
// }

/**
 * @brief Lightweight executor for action lists referenced by the State Machine.
 *
 * This class provides synchronous helper methods used by the State Machine to
 * perform environment actions (start/stop other state machines, set FG state,
 * sleep, sync, set network handle, etc.). For the purposes of the project,
 * methods are implemented as simple synchronous stubs (see action_executor.cpp).
 *
 * It is intentionally small and header-only in its interface so tests may mock
 * it if necessary.
 */
class ActionExecutor {
public:
    ActionExecutor() = default;
    ~ActionExecutor() = default;

    // Execute an array of ActionItem objects (count elements).
    // If the implementation uses a sentinel element (targetName == nullptr),
    // the caller may pass full capacity and the function will stop on sentinel.
    void ExecuteActionList(const config::ActionItem* actions, std::size_t count);

    // Execute a single action element.
    void ExecuteAction(const config::ActionItem& action);

    // Specific action helpers (public for testing/mocking visibility).
    void ExecuteSetFunctionGroupState(const char* fgName, const char* stateName);
    void ExecuteStartStateMachine(const char* smName, const char* initialState);
    void ExecuteStopStateMachine(const char* smName);
    void ExecuteSync();
    void ExecuteSleep(uint32_t milliseconds);
    void ExecuteSetNetworkHandle(const char* handleName, const char* state);

    // Non-copyable/movable semantics are acceptable; adjust if needed.
    ActionExecutor(const ActionExecutor&) = delete;
    ActionExecutor& operator=(const ActionExecutor&) = delete;
};

} // namespace sm
} // namespace ara

#endif // ARA_SM_ACTION_EXECUTOR_H
