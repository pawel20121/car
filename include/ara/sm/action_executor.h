#ifndef ARA_SM_ACTION_EXECUTOR_H
#define ARA_SM_ACTION_EXECUTOR_H

#include <cstddef>
#include <cstdint>
#include "static_config.h"
#include "i_action_executor.h"

namespace ara {
namespace sm {

/**
 * Concrete implementation of IActionExecutor used by SM.
 * For testing we will mock IActionExecutor.
 *
 * @req [SWS_SM_00609] Action list execution order
 */
class ActionExecutor : public IActionExecutor {
public:
    ActionExecutor() = default;
    ~ActionExecutor() override = default;

    void ExecuteActionList(const config::ActionItem* actions, std::size_t count) override;
    void ExecuteAction(const config::ActionItem& action) override;

    // helpers (kept public for tests)
    void ExecuteSetFunctionGroupState(const char* fgName, const char* stateName);
    void ExecuteStartStateMachine(const char* smName, const char* initialState);
    void ExecuteStopStateMachine(const char* smName);
    void ExecuteSync();
    void ExecuteSleep(uint32_t milliseconds);
    void ExecuteSetNetworkHandle(const char* handleName, const char* state);
};

} // namespace sm
} // namespace ara

#endif // ARA_SM_ACTION_EXECUTOR_H
