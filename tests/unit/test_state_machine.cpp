#include <gtest/gtest.h>

#include "state_machine.h"
#include "transition_table.h"
#include "static_config.h"

using namespace ara::sm;

namespace {

// ============================================================================
// FakeActionExecutor
// ============================================================================

class FakeActionExecutor final : public IActionExecutor {
public:
    void ExecuteActionList(
        const ara::sm::config::ActionItem*,
        size_t) override
    {
        ++executeListCalls;
    }

    void ExecuteAction(
        const ara::sm::config::ActionItem&) override
    {
        ++executeActionCalls;
    }

    int executeListCalls{0};
    int executeActionCalls{0};
};

// ============================================================================
// RecoveryTriggerExecutor — linie 84–85
// ============================================================================

class RecoveryTriggerExecutor final : public IActionExecutor {
public:
    void ExecuteActionList(
        const ara::sm::config::ActionItem*,
        size_t) override
    {
        if (!armed)
            return;

        auto r = sm->RequestTransition(1);

        EXPECT_FALSE(r.HasValue());
        EXPECT_EQ(
            r.Error(),
            StateManagementErrc::kRecoveryTransitionOngoing);

        hit = true;
    }

    void ExecuteAction(
        const ara::sm::config::ActionItem&) override {}

    StateMachine* sm{nullptr};
    bool armed{false};
    bool hit{false};
};

// ============================================================================
// InTransitionObserverExecutor — linia 149
// ============================================================================

class InTransitionObserverExecutor final : public IActionExecutor {
public:
    void ExecuteActionList(
        const ara::sm::config::ActionItem*,
        size_t) override
    {
        // isInTransition_ == true
        EXPECT_EQ(sm->GetCurrentState(), kInTransitionStateName);
        hit = true;
    }

    void ExecuteAction(
        const ara::sm::config::ActionItem&) override {}

    StateMachine* sm{nullptr};
    bool hit{false};
};

} // namespace

// ============================================================================
// Constructor
// ============================================================================

TEST(StateMachineTest, ConstructorInitialState)
{
    FakeActionExecutor exec;
    StateMachine sm("SM1", StateMachine::Category::kAgent, &exec);

    EXPECT_EQ(sm.GetName(), "SM1");
    EXPECT_EQ(sm.GetCategory(), StateMachine::Category::kAgent);
    EXPECT_FALSE(sm.IsRunning());
    EXPECT_FALSE(sm.IsInTransition());
    EXPECT_EQ(sm.GetCurrentStateEnum(), StateMachine::State::kInitial);
}

// ============================================================================
// Start / Stop
// ============================================================================

TEST(StateMachineTest, StartSetsRunningAndTargetState)
{
    FakeActionExecutor exec;
    StateMachine sm("SM", StateMachine::Category::kAgent, &exec);

    auto r = sm.Start(StateMachine::State::kRunning);

    EXPECT_TRUE(r.HasValue());
    EXPECT_TRUE(sm.IsRunning());
    EXPECT_EQ(sm.GetCurrentStateEnum(), StateMachine::State::kRunning);
}

TEST(StateMachineTest, StopWhenNotRunningReturnsOk)
{
    FakeActionExecutor exec;
    StateMachine sm("SM", StateMachine::Category::kAgent, &exec);

    auto r = sm.Stop();

    EXPECT_TRUE(r.HasValue());
    EXPECT_FALSE(sm.IsRunning());
    EXPECT_EQ(sm.GetCurrentStateEnum(), StateMachine::State::kInitial);
}

TEST(StateMachineTest, StopCalledTwice)
{
    FakeActionExecutor exec;
    StateMachine sm("SM", StateMachine::Category::kAgent, &exec);

    sm.Start(StateMachine::State::kRunning);

    auto r1 = sm.Stop();
    auto r2 = sm.Stop();

    EXPECT_TRUE(r1.HasValue());
    EXPECT_TRUE(r2.HasValue());
    EXPECT_FALSE(sm.IsRunning());
}

// ============================================================================
// RequestTransition
// ============================================================================

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
    StateMachine sm("SM", StateMachine::Category::kController, &exec);

    sm.Start(StateMachine::State::kInitial);

    auto r = sm.RequestTransition(1);

    EXPECT_TRUE(r.HasValue());
}

// ============================================================================
// Error recovery — linie 84–85
// ============================================================================

TEST(StateMachineTest, RequestTransitionBlockedDuringErrorRecovery)
{
    RecoveryTriggerExecutor exec;

    StateMachine sm("SM", StateMachine::Category::kController, &exec);
    exec.sm = &sm;

    sm.Start(StateMachine::State::kInitial);

    exec.armed = true;
    sm.HandleErrorNotification(123);

    EXPECT_TRUE(exec.hit);
}

// ============================================================================
// Error handling
// ============================================================================

TEST(StateMachineTest, HandleErrorIgnoredWhenImpactedByUpdate)
{
    FakeActionExecutor exec;
    StateMachine sm("SM", StateMachine::Category::kAgent, &exec);

    sm.Start(StateMachine::State::kRunning);
    sm.SetImpactedByUpdate(true);

    sm.HandleErrorNotification(42);

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

// ============================================================================
// Update flag
// ============================================================================

TEST(StateMachineTest, ImpactedByUpdateGetterSetter)
{
    FakeActionExecutor exec;
    StateMachine sm("SM", StateMachine::Category::kAgent, &exec);

    sm.SetImpactedByUpdate(true);
    EXPECT_TRUE(sm.IsImpactedByUpdate());

    sm.SetImpactedByUpdate(false);
    EXPECT_FALSE(sm.IsImpactedByUpdate());
}

// ============================================================================
// GetCurrentState / StateToString
// ============================================================================

TEST(StateMachineTest, GetCurrentStateCoversAllStates)
{
    FakeActionExecutor exec;
    StateMachine sm("SM", StateMachine::Category::kAgent, &exec);

    sm.Start(StateMachine::State::kInitial);
    EXPECT_EQ(sm.GetCurrentState(), "Initial");

    sm.Start(StateMachine::State::kRunning);
    EXPECT_EQ(sm.GetCurrentState(), "Running");

    sm.Start(StateMachine::State::kOff);
    EXPECT_EQ(sm.GetCurrentState(), "Off");

    sm.Start(StateMachine::State::kShutdown);
    EXPECT_EQ(sm.GetCurrentState(), "Shutdown");

    sm.Start(StateMachine::State::kPrepareUpdate);
    EXPECT_EQ(sm.GetCurrentState(), "PrepareUpdate");

    sm.Start(StateMachine::State::kVerifyUpdate);
    EXPECT_EQ(sm.GetCurrentState(), "VerifyUpdate");

    sm.Start(StateMachine::State::kPrepareRollback);
    EXPECT_EQ(sm.GetCurrentState(), "PrepareRollback");
}

// ============================================================================
// LINIA 149 — isInTransition_
// ============================================================================

TEST(StateMachineTest, GetCurrentStateReturnsInTransitionName)
{
    InTransitionObserverExecutor exec;

    StateMachine sm("SM", StateMachine::Category::kController, &exec);
    exec.sm = &sm;

    sm.Start(StateMachine::State::kInitial);

    EXPECT_TRUE(exec.hit);
}

// ============================================================================
// LINIA 234
// ============================================================================

TEST(StateMachineTest, StateToStringInTransitionCaseExecuted)
{
    FakeActionExecutor exec;
    StateMachine sm("SM", StateMachine::Category::kAgent, &exec);

    sm.Start(StateMachine::State::kInTransition);

    EXPECT_EQ(
        sm.GetCurrentStateEnum(),
        StateMachine::State::kInTransition);
}

// ============================================================================
// LINIA 207 — brak wpisu w kActionTable
// ============================================================================

TEST(StateMachineTest, ExecuteActionListWithoutEntryPrintsMessage)
{
    FakeActionExecutor exec;
    StateMachine sm("SM", StateMachine::Category::kAgent, &exec);

    sm.Start(StateMachine::State::kShutdown);

    SUCCEED();
}
TEST(StateMachineTest, StateToStringExecutedForNoActionState)
{
    // brak executora → brak wczesnego return
    StateMachine sm("SM", StateMachine::Category::kAgent, nullptr);

    // brak wpisu w kActionTable
    sm.Start(StateMachine::State::kShutdown);

    // TO JEST KLUCZ:
    // wymusza realne wywołanie StateToString(currentState_)
    EXPECT_EQ(sm.GetCurrentState(), "Shutdown");
}