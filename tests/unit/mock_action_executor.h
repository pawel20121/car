#ifndef MOCK_ACTION_EXECUTOR_H
#define MOCK_ACTION_EXECUTOR_H

#include "ara/sm/i_action_executor.h"
#include <vector>
#include <string>

namespace ara {
namespace sm {

class MockActionExecutor : public IActionExecutor {
public:
    struct CalledAction {
        uint32_t type;
        std::string targetName;
        std::string targetState;
        uint32_t delayMs;
    };

    std::vector<CalledAction> calls;

    void ExecuteActionList(const config::ActionItem* actions, std::size_t count) override {
        for (size_t i = 0; i < count; ++i) {
            const auto& a = actions[i];
            CallFromActionItem(a);
            if (a.targetName == nullptr) break;
        }
    }

    void ExecuteAction(const config::ActionItem& action) override {
        CallFromActionItem(action);
    }

private:
    void CallFromActionItem(const config::ActionItem& a) {
        CalledAction ca;
        ca.type = static_cast<uint32_t>(a.type);
        ca.targetName = a.targetName ? a.targetName : "";
        ca.targetState = a.targetState ? a.targetState : "";
        ca.delayMs = a.delayMs;
        calls.push_back(ca);
    }
};

} // namespace sm
} // namespace ara

#endif // MOCK_ACTION_EXECUTOR_H
