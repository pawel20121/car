#include "update_request_service.h"
#include "state_machine.h"
#include <gtest/gtest.h>

using namespace ara::sm;

namespace {

/* HARD RESET singletonu między testami */
void ResetUpdateRequestService()
{
    UpdateRequestService service;
    service.SetControllerStateMachine(nullptr);
    service.SetUpdateAllowed(UpdateAllowedType::kUpdateNotAllowed);
    service.SetResetMachineNotifier(UpdateStatusType::kIdle);

    // brutalnie zatrzymujemy sesję
    while (service.IsUpdateSessionActive()) {
        service.StopUpdateSession();
    }
}

class UpdateRequestServiceFixture : public ::testing::Test {
protected:
    void SetUp() override {
        ResetUpdateRequestService();
    }

    void TearDown() override {
        ResetUpdateRequestService();
    }
};

} // namespace
TEST_F(UpdateRequestServiceFixture, RequestUpdateSession_Rejected_WhenNotAllowed)
{
    UpdateRequestService service;
    service.SetUpdateAllowed(UpdateAllowedType::kUpdateNotAllowed);

    auto r = service.RequestUpdateSession();
    EXPECT_FALSE(r.HasValue());
}

TEST_F(UpdateRequestServiceFixture, RequestUpdateSession_Granted)
{
    UpdateRequestService service;
    service.SetUpdateAllowed(UpdateAllowedType::kUpdateAllowed);

    auto r = service.RequestUpdateSession();
    EXPECT_TRUE(r.HasValue());
}

TEST_F(UpdateRequestServiceFixture, RequestUpdateSession_Rejected_WhenAlreadyActive)
{
    UpdateRequestService service;
    service.SetUpdateAllowed(UpdateAllowedType::kUpdateAllowed);

    ASSERT_TRUE(service.RequestUpdateSession().HasValue());
    auto r = service.RequestUpdateSession();

    EXPECT_FALSE(r.HasValue());
}
TEST_F(UpdateRequestServiceFixture, PrepareUpdate_Rejected_OutsideSession)
{
    UpdateRequestService service;
    FunctionGroupListType fgs = {"FG1"};

    auto r = service.PrepareUpdate(fgs);
    EXPECT_FALSE(r.HasValue());
}

TEST_F(UpdateRequestServiceFixture, PrepareUpdate_Fails_OnEmptyFGList)
{
    UpdateRequestService service;
    service.SetUpdateAllowed(UpdateAllowedType::kUpdateAllowed);
    ASSERT_TRUE(service.RequestUpdateSession().HasValue());

    FunctionGroupListType empty;
    auto r = service.PrepareUpdate(empty);

    EXPECT_FALSE(r.HasValue());
}
TEST_F(UpdateRequestServiceFixture, ResetMachine_Rejected_OutsideSession)
{
    UpdateRequestService service;
    service.ResetMachine();

    EXPECT_EQ(service.GetResetMachineNotifier(),
              UpdateStatusType::kRejected);
}
TEST_F(UpdateRequestServiceFixture, StopUpdateSession_Idempotent)
{
    UpdateRequestService service;

    auto r = service.StopUpdateSession();
    EXPECT_TRUE(r.HasValue());
}
