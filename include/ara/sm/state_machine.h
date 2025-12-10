#ifndef ARA_SM_STATE_MACHINE_H
#define ARA_SM_STATE_MACHINE_H

#include <string>
#include <cstdint>

#include "ara/sm/types.h"
#include "ara/core/result.h"
#include "ara/sm/i_action_executor.h"

namespace ara {
namespace sm {

class StateMachine {
public:
    enum class State : uint8_t {
        kInitial = 0,
        kOff = 1,
        kPrepareUpdate = 2,
        kVerifyUpdate = 3,
        kPrepareRollback = 4,
        kRunning = 5,
        kShutdown = 6,
        kInTransition = 255
    };

    enum class Category : uint8_t {
        kController = 0,
        kAgent = 1
    };

    // Preferowany konstruktor (używany także w testach)
    explicit StateMachine(const std::string& name,
                          Category category,
                          IActionExecutor* executor);

    // Konstruktor uproszczony (bez ActionExecutor)
    explicit StateMachine(const std::string& name,
                          Category category)
        : StateMachine(name, category, nullptr) {}

    ~StateMachine();

    StateMachine(const StateMachine&) = delete;
    StateMachine& operator=(const StateMachine&) = delete;
    StateMachine(StateMachine&&) noexcept = default;
    StateMachine& operator=(StateMachine&&) noexcept = default;

    ara::core::Result<void, StateManagementErrc> RequestTransition(TransitionRequestType request);

    StateMachineStateNameType GetCurrentState() const;
    State GetCurrentStateEnum() const;

    const std::string& GetName() const;
    Category GetCategory() const;

    bool IsInTransition() const;
    bool IsRunning() const;

    void HandleErrorNotification(uint32_t executionError);
    void SetImpactedByUpdate(bool impacted);
    bool IsImpactedByUpdate() const;

    ara::core::Result<void, StateManagementErrc> Start(State targetState = State::kInitial);
    ara::core::Result<void, StateManagementErrc> Stop();

private:
    void ExecuteActionList();
    ara::core::Result<void, StateManagementErrc> TransitionTo(State newState);
    bool IsTransitionAllowed(TransitionRequestType request) const;
    static std::string StateToString(State state);

private:
    std::string name_;
    Category category_;
    State currentState_;
    bool isRunning_;
    bool isInTransition_;
    bool errorRecoveryOngoing_;
    bool impactedByUpdate_;

    IActionExecutor* actionExecutor_;
};

} // namespace sm
} // namespace ara

#endif // ARA_SM_STATE_MACHINE_H
