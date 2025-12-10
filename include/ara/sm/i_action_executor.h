#ifndef ARA_SM_I_ACTION_EXECUTOR_H
#define ARA_SM_I_ACTION_EXECUTOR_H

#include <cstddef>
#include <cstdint>

namespace ara {
namespace sm {

namespace config {
    struct ActionItem;
}

class IActionExecutor {
public:
    virtual ~IActionExecutor() = default;

    // Execute an array of actions (count items)
    virtual void ExecuteActionList(const config::ActionItem* actions, std::size_t count) = 0;

    // Execute single action
    virtual void ExecuteAction(const config::ActionItem& action) = 0;
};

} // namespace sm
} // namespace ara

#endif // ARA_SM_I_ACTION_EXECUTOR_H
