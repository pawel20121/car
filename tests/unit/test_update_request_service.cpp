#include <gtest/gtest.h>

#include "update_request_service.h"
#include "state_machine.h"

using namespace ara::sm;

namespace {

/* HARD RESET singletonu między testami */
void ResetUpdateRequestService()
{
    UpdateRequestService service;
    service.SetControllerStateMachine(nullptr);
    service.SetUpdateAllowed(UpdateAllowedType::kUpdateNotAllowed);
    service.SetResetMachineNotifier(UpdateStatusType::kIdle);

    while (service.IsUpdateSessionActive()) {
        service.StopUpdateSession();
    }
}

class UpdateRequestServiceFixture : public ::testing::Test {
protected:
    void SetUp() override { ResetUpdateRequestService(); }
    void TearDown() override { ResetUpdateRequestService(); }
};

} // namespace

// ============================================================================
// RequestUpdateSession
// ============================================================================

TEST_F(UpdateRequestServiceFixture, RequestUpdateSession_Rejected_WhenNotAllowed)
{
    UpdateRequestService service;
    service.SetUpdateAllowed(UpdateAllowedType::kUpdateNotAllowed);

    EXPECT_FALSE(service.RequestUpdateSession().HasValue());
}

TEST_F(UpdateRequestServiceFixture, RequestUpdateSession_Granted)
{
    UpdateRequestService service;
    service.SetUpdateAllowed(UpdateAllowedType::kUpdateAllowed);

    EXPECT_TRUE(service.RequestUpdateSession().HasValue());
}

TEST_F(UpdateRequestServiceFixture, RequestUpdateSession_Rejected_WhenAlreadyActive)
{
    UpdateRequestService service;
    service.SetUpdateAllowed(UpdateAllowedType::kUpdateAllowed);

    ASSERT_TRUE(service.RequestUpdateSession().HasValue());
    EXPECT_FALSE(service.RequestUpdateSession().HasValue());
}

// ============================================================================
// PrepareUpdate
// ============================================================================

TEST_F(UpdateRequestServiceFixture, PrepareUpdate_Rejected_OutsideSession)
{
    UpdateRequestService service;
    FunctionGroupListType fgs = {"FG1"};

    EXPECT_FALSE(service.PrepareUpdate(fgs).HasValue());
}

TEST_F(UpdateRequestServiceFixture, PrepareUpdate_Fails_OnEmptyFGList)
{
    UpdateRequestService service;
    service.SetUpdateAllowed(UpdateAllowedType::kUpdateAllowed);
    ASSERT_TRUE(service.RequestUpdateSession().HasValue());

    FunctionGroupListType empty;
    EXPECT_FALSE(service.PrepareUpdate(empty).HasValue());
}

TEST_F(UpdateRequestServiceFixture, PrepareUpdate_Success_WhenNoController)
{
    UpdateRequestService service;
    service.SetUpdateAllowed(UpdateAllowedType::kUpdateAllowed);
    ASSERT_TRUE(service.RequestUpdateSession().HasValue());

    FunctionGroupListType fgs = {"FG1"};
    EXPECT_TRUE(service.PrepareUpdate(fgs).HasValue());
}

TEST_F(UpdateRequestServiceFixture, PrepareUpdate_Fails_WithController_WhenTransitionNotAllowed)
{
    UpdateRequestService service;
    StateMachine ctrl("CTRL", StateMachine::Category::kController, nullptr);

    service.SetControllerStateMachine(&ctrl);
    service.SetUpdateAllowed(UpdateAllowedType::kUpdateAllowed);
    ASSERT_TRUE(service.RequestUpdateSession().HasValue());

    FunctionGroupListType fgs = {"FG1", "FG2"};

    EXPECT_FALSE(service.PrepareUpdate(fgs).HasValue());
    EXPECT_TRUE(ctrl.IsImpactedByUpdate());
}

// ============================================================================
// VerifyUpdate
// ============================================================================

TEST_F(UpdateRequestServiceFixture, VerifyUpdate_Rejected_OutsideSession)
{
    UpdateRequestService service;
    FunctionGroupListType fgs = {"FG1"};

    EXPECT_FALSE(service.VerifyUpdate(fgs).HasValue());
}

TEST_F(UpdateRequestServiceFixture, VerifyUpdate_Fails_OnEmptyFGList)
{
    UpdateRequestService service;
    service.SetUpdateAllowed(UpdateAllowedType::kUpdateAllowed);
    ASSERT_TRUE(service.RequestUpdateSession().HasValue());

    FunctionGroupListType empty;
    EXPECT_FALSE(service.VerifyUpdate(empty).HasValue());
}

TEST_F(UpdateRequestServiceFixture, VerifyUpdate_Success_WhenNoController)
{
    UpdateRequestService service;
    service.SetUpdateAllowed(UpdateAllowedType::kUpdateAllowed);
    ASSERT_TRUE(service.RequestUpdateSession().HasValue());

    FunctionGroupListType fgs = {"FG1"};
    EXPECT_TRUE(service.VerifyUpdate(fgs).HasValue());
}

TEST_F(UpdateRequestServiceFixture, VerifyUpdate_Fails_WithController_WhenTransitionNotAllowed)
{
    UpdateRequestService service;
    StateMachine ctrl("CTRL", StateMachine::Category::kController, nullptr);

    service.SetControllerStateMachine(&ctrl);
    service.SetUpdateAllowed(UpdateAllowedType::kUpdateAllowed);
    ASSERT_TRUE(service.RequestUpdateSession().HasValue());

    FunctionGroupListType fgs = {"FG1"};

    EXPECT_FALSE(service.VerifyUpdate(fgs).HasValue());
}

// ============================================================================
// PrepareRollback
// ============================================================================

TEST_F(UpdateRequestServiceFixture, PrepareRollback_Rejected_OutsideSession)
{
    UpdateRequestService service;
    FunctionGroupListType fgs = {"FG1"};

    EXPECT_FALSE(service.PrepareRollback(fgs).HasValue());
}

TEST_F(UpdateRequestServiceFixture, PrepareRollback_Fails_OnEmptyFGList)
{
    UpdateRequestService service;
    service.SetUpdateAllowed(UpdateAllowedType::kUpdateAllowed);
    ASSERT_TRUE(service.RequestUpdateSession().HasValue());

    FunctionGroupListType empty;
    EXPECT_FALSE(service.PrepareRollback(empty).HasValue());
}

TEST_F(UpdateRequestServiceFixture, PrepareRollback_Success_WhenNoController)
{
    UpdateRequestService service;
    service.SetUpdateAllowed(UpdateAllowedType::kUpdateAllowed);
    ASSERT_TRUE(service.RequestUpdateSession().HasValue());

    FunctionGroupListType fgs = {"FG1"};
    EXPECT_TRUE(service.PrepareRollback(fgs).HasValue());
}

TEST_F(UpdateRequestServiceFixture, PrepareRollback_Fails_WithController_WhenTransitionNotAllowed)
{
    UpdateRequestService service;
    StateMachine ctrl("CTRL", StateMachine::Category::kController, nullptr);

    service.SetControllerStateMachine(&ctrl);
    service.SetUpdateAllowed(UpdateAllowedType::kUpdateAllowed);
    ASSERT_TRUE(service.RequestUpdateSession().HasValue());

    FunctionGroupListType fgs = {"FG1"};

    EXPECT_FALSE(service.PrepareRollback(fgs).HasValue());
}

// ============================================================================
// ResetMachine
// ============================================================================

TEST_F(UpdateRequestServiceFixture, ResetMachine_Rejected_OutsideSession)
{
    UpdateRequestService service;
    service.ResetMachine();

    EXPECT_EQ(service.GetResetMachineNotifier(),
              UpdateStatusType::kRejected);
}

TEST_F(UpdateRequestServiceFixture, ResetMachine_Failed_WhenNoController)
{
    UpdateRequestService service;
    service.SetUpdateAllowed(UpdateAllowedType::kUpdateAllowed);
    ASSERT_TRUE(service.RequestUpdateSession().HasValue());

    service.ResetMachine();

    EXPECT_EQ(service.GetResetMachineNotifier(),
              UpdateStatusType::kFailed);
}

TEST_F(UpdateRequestServiceFixture, ResetMachine_Failed_WithController_WhenRestartNotAllowed)
{
    UpdateRequestService service;
    StateMachine ctrl("CTRL", StateMachine::Category::kController, nullptr);

    service.SetControllerStateMachine(&ctrl);
    service.SetUpdateAllowed(UpdateAllowedType::kUpdateAllowed);
    ASSERT_TRUE(service.RequestUpdateSession().HasValue());

    service.ResetMachine();

    EXPECT_EQ(service.GetResetMachineNotifier(),
              UpdateStatusType::kFailed);
}

// ============================================================================
// StopUpdateSession
// ============================================================================

TEST_F(UpdateRequestServiceFixture, StopUpdateSession_Idempotent)
{
    UpdateRequestService service;
    EXPECT_TRUE(service.StopUpdateSession().HasValue());
}

TEST_F(UpdateRequestServiceFixture, StopUpdateSession_Succeeds_WhenNoController)
{
    UpdateRequestService service;
    service.SetUpdateAllowed(UpdateAllowedType::kUpdateAllowed);
    ASSERT_TRUE(service.RequestUpdateSession().HasValue());

    auto r = service.StopUpdateSession();

    EXPECT_TRUE(r.HasValue());
    EXPECT_FALSE(service.IsUpdateSessionActive());
}

TEST_F(UpdateRequestServiceFixture, StopUpdateSession_Fails_WithController_WhenTransitionNotAllowed)
{
    UpdateRequestService service;
    StateMachine ctrl("CTRL", StateMachine::Category::kController, nullptr);

    service.SetControllerStateMachine(&ctrl);
    service.SetUpdateAllowed(UpdateAllowedType::kUpdateAllowed);
    ASSERT_TRUE(service.RequestUpdateSession().HasValue());

    auto r = service.StopUpdateSession();

    EXPECT_FALSE(r.HasValue());
    EXPECT_TRUE(service.IsUpdateSessionActive());
}

// ============================================================================
// Side effects
// ============================================================================

TEST_F(UpdateRequestServiceFixture, RequestUpdateSession_SetsResetNotifierIdle)
{
    UpdateRequestService service;
    service.SetUpdateAllowed(UpdateAllowedType::kUpdateAllowed);

    ASSERT_TRUE(service.RequestUpdateSession().HasValue());
    EXPECT_EQ(service.GetResetMachineNotifier(),
              UpdateStatusType::kIdle);
}
TEST_F(UpdateRequestServiceFixture, SetResetMachineNotifier_IsCovered)
{
    UpdateRequestService service;

    service.SetResetMachineNotifier(UpdateStatusType::kSuccessful);
    EXPECT_EQ(service.GetResetMachineNotifier(),
              UpdateStatusType::kSuccessful);

    service.SetResetMachineNotifier(UpdateStatusType::kFailed);
    EXPECT_EQ(service.GetResetMachineNotifier(),
              UpdateStatusType::kFailed);
}
TEST_F(UpdateRequestServiceFixture, SetUpdateAllowed_IsCovered)
{
    UpdateRequestService service;

    service.SetUpdateAllowed(UpdateAllowedType::kUpdateAllowed);
    EXPECT_TRUE(service.RequestUpdateSession().HasValue());

    service.StopUpdateSession();

    service.SetUpdateAllowed(UpdateAllowedType::kUpdateNotAllowed);
    EXPECT_FALSE(service.RequestUpdateSession().HasValue());
}
