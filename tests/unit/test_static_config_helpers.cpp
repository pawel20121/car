/**
 * @file test_static_config_helpers.cpp
 * @brief Unit tests for static configuration helper functions
 */

#include <gtest/gtest.h>
#include "static_config.h"

namespace ara {
namespace sm {
namespace config {

// Test StateIdToString function
class StateIdToStringTest : public ::testing::Test {};

TEST_F(StateIdToStringTest, ReturnsCorrectStringForInitial) {
    EXPECT_STREQ("Initial", StateIdToString(States::kInitial));
}

TEST_F(StateIdToStringTest, ReturnsCorrectStringForOff) {
    EXPECT_STREQ("Off", StateIdToString(States::kOff));
}

TEST_F(StateIdToStringTest, ReturnsCorrectStringForRunning) {
    EXPECT_STREQ("Running", StateIdToString(States::kRunning));
}

TEST_F(StateIdToStringTest, ReturnsCorrectStringForPrepareUpdate) {
    EXPECT_STREQ("PrepareUpdate", StateIdToString(States::kPrepareUpdate));
}

TEST_F(StateIdToStringTest, ReturnsCorrectStringForVerifyUpdate) {
    EXPECT_STREQ("VerifyUpdate", StateIdToString(States::kVerifyUpdate));
}

TEST_F(StateIdToStringTest, ReturnsCorrectStringForPrepareRollback) {
    EXPECT_STREQ("PrepareRollback", StateIdToString(States::kPrepareRollback));
}

TEST_F(StateIdToStringTest, ReturnsCorrectStringForStartup) {
    EXPECT_STREQ("Startup", StateIdToString(States::kStartup));
}

TEST_F(StateIdToStringTest, ReturnsCorrectStringForShutdown) {
    EXPECT_STREQ("Shutdown", StateIdToString(States::kShutdown));
}

TEST_F(StateIdToStringTest, ReturnsCorrectStringForRestart) {
    EXPECT_STREQ("Restart", StateIdToString(States::kRestart));
}

TEST_F(StateIdToStringTest, ReturnsCorrectStringForContinueUpdate) {
    EXPECT_STREQ("ContinueUpdate", StateIdToString(States::kContinueUpdate));
}

TEST_F(StateIdToStringTest, ReturnsCorrectStringForAfterUpdate) {
    EXPECT_STREQ("AfterUpdate", StateIdToString(States::kAfterUpdate));
}

TEST_F(StateIdToStringTest, ReturnsCorrectStringForDegraded) {
    EXPECT_STREQ("Degraded", StateIdToString(States::kDegraded));
}

TEST_F(StateIdToStringTest, ReturnsCorrectStringForInTransition) {
    EXPECT_STREQ("InTransition", StateIdToString(States::kInTransition));
}

TEST_F(StateIdToStringTest, ReturnsCorrectStringForInvalid) {
    EXPECT_STREQ("Invalid", StateIdToString(States::kInvalid));
}

TEST_F(StateIdToStringTest, ReturnsUnknownForInvalidState) {
    EXPECT_STREQ("Unknown", StateIdToString(9999));
}

// Test TriggerIdToString function
class TriggerIdToStringTest : public ::testing::Test {};

TEST_F(TriggerIdToStringTest, ReturnsCorrectStringForStartup) {
    EXPECT_STREQ("Startup", TriggerIdToString(Triggers::kStartup));
}

TEST_F(TriggerIdToStringTest, ReturnsCorrectStringForShutdownRequest) {
    EXPECT_STREQ("ShutdownRequest", TriggerIdToString(Triggers::kShutdownRequest));
}

TEST_F(TriggerIdToStringTest, ReturnsCorrectStringForRestartRequest) {
    EXPECT_STREQ("RestartRequest", TriggerIdToString(Triggers::kRestartRequest));
}

TEST_F(TriggerIdToStringTest, ReturnsCorrectStringForGoToRunning) {
    EXPECT_STREQ("GoToRunning", TriggerIdToString(Triggers::kGoToRunning));
}

TEST_F(TriggerIdToStringTest, ReturnsCorrectStringForPrepareUpdateRequest) {
    EXPECT_STREQ("PrepareUpdateRequest", TriggerIdToString(Triggers::kPrepareUpdateRequest));
}

TEST_F(TriggerIdToStringTest, ReturnsCorrectStringForVerifyUpdateRequest) {
    EXPECT_STREQ("VerifyUpdateRequest", TriggerIdToString(Triggers::kVerifyUpdateRequest));
}

TEST_F(TriggerIdToStringTest, ReturnsCorrectStringForPrepareRollbackRequest) {
    EXPECT_STREQ("PrepareRollbackRequest", TriggerIdToString(Triggers::kPrepareRollbackRequest));
}

TEST_F(TriggerIdToStringTest, ReturnsCorrectStringForFinishUpdateRequest) {
    EXPECT_STREQ("FinishUpdateRequest", TriggerIdToString(Triggers::kFinishUpdateRequest));
}

TEST_F(TriggerIdToStringTest, ReturnsCorrectStringForNetworkFullCom) {
    EXPECT_STREQ("NetworkFullCom", TriggerIdToString(Triggers::kNetworkFullCom));
}

TEST_F(TriggerIdToStringTest, ReturnsCorrectStringForNetworkNoCom) {
    EXPECT_STREQ("NetworkNoCom", TriggerIdToString(Triggers::kNetworkNoCom));
}

TEST_F(TriggerIdToStringTest, ReturnsCorrectStringForUserRequest) {
    EXPECT_STREQ("UserRequest", TriggerIdToString(Triggers::kUserRequest));
}

TEST_F(TriggerIdToStringTest, ReturnsCorrectStringForDegradeRequest) {
    EXPECT_STREQ("DegradeRequest", TriggerIdToString(Triggers::kDegradeRequest));
}

TEST_F(TriggerIdToStringTest, ReturnsUnknownForInvalidTrigger) {
    EXPECT_STREQ("Unknown", TriggerIdToString(static_cast<TransitionRequestType>(9999)));
}

// Test ActionTypeToString function
class ActionTypeToStringTest : public ::testing::Test {};

TEST_F(ActionTypeToStringTest, ReturnsCorrectStringForSetFunctionGroupState) {
    EXPECT_STREQ("SetFunctionGroupState", ActionTypeToString(ActionType::kSetFunctionGroupState));
}

TEST_F(ActionTypeToStringTest, ReturnsCorrectStringForStartStateMachine) {
    EXPECT_STREQ("StartStateMachine", ActionTypeToString(ActionType::kStartStateMachine));
}

TEST_F(ActionTypeToStringTest, ReturnsCorrectStringForStopStateMachine) {
    EXPECT_STREQ("StopStateMachine", ActionTypeToString(ActionType::kStopStateMachine));
}

TEST_F(ActionTypeToStringTest, ReturnsCorrectStringForSync) {
    EXPECT_STREQ("Sync", ActionTypeToString(ActionType::kSync));
}

TEST_F(ActionTypeToStringTest, ReturnsCorrectStringForSleep) {
    EXPECT_STREQ("Sleep", ActionTypeToString(ActionType::kSleep));
}

TEST_F(ActionTypeToStringTest, ReturnsCorrectStringForSetNetworkHandle) {
    EXPECT_STREQ("SetNetworkHandle", ActionTypeToString(ActionType::kSetNetworkHandle));
}

TEST_F(ActionTypeToStringTest, ReturnsUnknownForInvalidActionType) {
    EXPECT_STREQ("Unknown", ActionTypeToString(static_cast<ActionType>(9999)));
}

} // namespace config
} // namespace sm
} // namespace ara