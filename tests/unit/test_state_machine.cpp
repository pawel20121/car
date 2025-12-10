#include "gtest/gtest.h"
#include "ara/sm/state_machine.h"
#include "ara/core/result.h"

using namespace ara::sm;

// Helper to check Result<void,StateManagementErrc>
static bool IsSuccess(const ara::core::Result<void, StateManagementErrc>& r) {
    return r.HasValue();
}

static StateManagementErrc GetError(const ara::core::Result<void, StateManagementErrc>& r) {
    return r.Error();
}

// ============================================================================
// 1) Test: podstawowa zmiana stanu (Start + RequestTransition → Running)
// ============================================================================
TEST(StateMachineTests, StateChange_RequestTransition_Success) {
    StateMachine sm("test_sm", StateMachine::Category::kAgent);

    // Start -> Initial
    auto rstart = sm.Start(StateMachine::State::kInitial);
    EXPECT_TRUE(rstart.HasValue());

    // Według uproszczonego IsTransitionAllowed:
    // Initial + request==1 -> Running
    TransitionRequestType request = 1;
    auto r = sm.RequestTransition(request);
    EXPECT_TRUE(r.HasValue());

    EXPECT_EQ(sm.GetCurrentStateEnum(), StateMachine::State::kRunning);
    EXPECT_EQ(sm.GetCurrentState(), std::string("Running"));
}

// ============================================================================
// 2) Test: błędne przejście (niepoprawny request) -> kTransitionNotAllowed
// ============================================================================
TEST(StateMachineTests, RequestTransition_InvalidNotAllowed) {
    StateMachine sm("test_sm2", StateMachine::Category::kAgent);

    sm.Start(StateMachine::State::kInitial);

    TransitionRequestType invalidReq = 9999; // brak w uproszczonej logice
    auto r = sm.RequestTransition(invalidReq);
    EXPECT_FALSE(r.HasValue());
    EXPECT_EQ(GetError(r), StateManagementErrc::kTransitionNotAllowed);
}

// ============================================================================
// 3) Test: error notification powoduje recovery i przejście do Off
// ============================================================================
TEST(StateMachineTests, HandleErrorNotification_Recovery) {
    StateMachine sm("test_sm3", StateMachine::Category::kAgent);

    sm.Start(StateMachine::State::kRunning);

    uint32_t execError = 42;
    sm.HandleErrorNotification(execError);

    // U Ciebie: Handler wymusza przejście do Off
    EXPECT_EQ(sm.GetCurrentStateEnum(), StateMachine::State::kOff);
    EXPECT_EQ(sm.GetCurrentState(), std::string("Off"));
}

// ============================================================================
// 4) Test: RequestTransition blokowane, gdy ImpactedByUpdate == true
// ============================================================================
TEST(StateMachineTests, RequestTransition_BlockedByUpdate) {
    StateMachine sm("test_sm4", StateMachine::Category::kAgent);

    sm.Start(StateMachine::State::kInitial);
    sm.SetImpactedByUpdate(true);

    auto r = sm.RequestTransition(1);
    EXPECT_FALSE(r.HasValue());
    EXPECT_EQ(GetError(r), StateManagementErrc::kUpdateInProgress);

    sm.SetImpactedByUpdate(false);
    auto r2 = sm.RequestTransition(1);
    EXPECT_TRUE(r2.HasValue());
}
