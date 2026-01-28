#include <gtest/gtest.h>

#include "state_machine.h"
#include "transition_table.h"

using namespace ara::sm;

// ============================================================================
// Fake Action Executor (pełna implementacja interfejsu)
// ============================================================================

class FakeActionExecutor : public IActionExecutor {
public:
    void ExecuteActionList(const config::ActionItem*, size_t) override {}
    void ExecuteAction(const config::ActionItem&) override {}
};

// ============================================================================
// Tests
// ============================================================================

TEST(StateMachineTest, ConstructorAndDestructor)
{
    FakeActionExecutor exec;
    {
        StateMachine sm("SM1", StateMachine::Category::kAgent, &exec);
        EXPECT_EQ(sm.GetName(), "SM1");
        EXPECT_EQ(sm.GetCategory(), StateMachine::Category::kAgent);
        EXPECT_FALSE(sm.IsRunning());
    }
}

TEST(StateMachineTest, StartSetsRunningAndState)
{
    FakeActionExecutor exec;
    StateMachine sm("SM", StateMachine::Category::kAgent, &exec);

    auto r = sm.Start(StateMachine::State::kRunning);

    EXPECT_TRUE(r.HasValue());
    EXPECT_TRUE(sm.IsRunning());
    EXPECT_EQ(sm.GetCurrentStateEnum(), StateMachine::State::kRunning);
}

TEST(StateMachineTest, StopWhenNotRunningIsNoop)
{
    FakeActionExecutor exec;
    StateMachine sm("SM", StateMachine::Category::kAgent, &exec);

    auto r = sm.Stop();

    EXPECT_TRUE(r.HasValue());
    EXPECT_FALSE(sm.IsRunning());
}

TEST(StateMachineTest, StopWhenRunningTransitionsToOff)
{
    FakeActionExecutor exec;
    StateMachine sm("SM", StateMachine::Category::kAgent, &exec);

    sm.Start(StateMachine::State::kRunning);
    auto r = sm.Stop();

    EXPECT_TRUE(r.HasValue());
    EXPECT_FALSE(sm.IsRunning());
    EXPECT_EQ(sm.GetCurrentStateEnum(), StateMachine::State::kOff);
}

TEST(StateMachineTest, RequestTransitionBlockedByUpdate)
{
    FakeActionExecutor exec;
    StateMachine sm("SM", StateMachine::Category::kAgent, &exec);

    sm.Start(StateMachine::State::kInitial);
    sm.SetImpactedByUpdate(true);

    auto r = sm.RequestTransition(1);

    EXPECT_FALSE(r.HasValue());
    EXPECT_EQ(r.Error(), StateManagementErrc::kUpdateInProgress);
}

TEST(StateMachineTest, RequestTransitionNotAllowed)
{
    FakeActionExecutor exec;
    StateMachine sm("SM", StateMachine::Category::kAgent, &exec);

    sm.Start(StateMachine::State::kInitial);

    auto r = sm.RequestTransition(9999);

    EXPECT_FALSE(r.HasValue());
    EXPECT_EQ(r.Error(), StateManagementErrc::kTransitionNotAllowed);
}

TEST(StateMachineTest, RequestTransitionSuccess)
{
    FakeActionExecutor exec;
    StateMachine sm("SM", StateMachine::Category::kAgent, &exec);

    sm.Start(StateMachine::State::kInitial);

    // 🔑 pobieramy REALNY dozwolony request z TransitionTable
    TransitionRequestType validRequest = 0;

    bool found = false;
    for (TransitionRequestType r = 0; r < 100; ++r) {
        if (TransitionTable::IsTransitionAllowed(
                static_cast<uint8_t>(StateMachine::State::kInitial),
                r,
                StateMachine::Category::kAgent))
        {
            validRequest = r;
            found = true;
            break;
        }
    }

    ASSERT_TRUE(found) << "No valid transition request found for Initial state";

    auto result = sm.RequestTransition(validRequest);

    EXPECT_TRUE(result.HasValue());
}

TEST(StateMachineTest, HandleErrorIgnoredWhenImpactedByUpdate)
{
    FakeActionExecutor exec;
    StateMachine sm("SM", StateMachine::Category::kAgent, &exec);

    sm.Start(StateMachine::State::kRunning);
    sm.SetImpactedByUpdate(true);

    sm.HandleErrorNotification(123);

    EXPECT_EQ(sm.GetCurrentStateEnum(), StateMachine::State::kRunning);
}

TEST(StateMachineTest, HandleErrorTransitionsToOff)
{
    FakeActionExecutor exec;
    StateMachine sm("SM", StateMachine::Category::kAgent, &exec);

    sm.Start(StateMachine::State::kRunning);
    sm.HandleErrorNotification(42);

    EXPECT_EQ(sm.GetCurrentStateEnum(), StateMachine::State::kOff);
}

TEST(StateMachineTest, Getters)
{
    FakeActionExecutor exec;
    StateMachine sm("MySM", StateMachine::Category::kController, &exec);

    EXPECT_EQ(sm.GetName(), "MySM");
    EXPECT_EQ(sm.GetCategory(), StateMachine::Category::kController);
    EXPECT_FALSE(sm.IsRunning());
    EXPECT_FALSE(sm.IsInTransition());
}
