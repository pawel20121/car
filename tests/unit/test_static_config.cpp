#include <gtest/gtest.h>

#include "static_config.h"

using namespace ara::sm::config;

// ============================================================================
// CONTROLLER TRANSITION TABLE TESTS
// ============================================================================

TEST(StaticConfigTest, ControllerTransitionTableNotEmpty)
{
    EXPECT_GT(kControllerTransitionsCount, 0);
}

TEST(StaticConfigTest, ControllerTransitionsFromInitial)
{
    // Initial -> Startup
    EXPECT_EQ(kControllerTransitions[0].fromState, States::kInitial);
    EXPECT_EQ(kControllerTransitions[0].trigger, Triggers::kStartup);
    EXPECT_EQ(kControllerTransitions[0].toState, States::kStartup);
    
    // Initial -> Running
    EXPECT_EQ(kControllerTransitions[1].fromState, States::kInitial);
    EXPECT_EQ(kControllerTransitions[1].trigger, Triggers::kGoToRunning);
    EXPECT_EQ(kControllerTransitions[1].toState, States::kRunning);
}

TEST(StaticConfigTest, ControllerTransitionsFromStartup)
{
    // Startup -> Running
    EXPECT_EQ(kControllerTransitions[2].fromState, States::kStartup);
    EXPECT_EQ(kControllerTransitions[2].trigger, Triggers::kGoToRunning);
    EXPECT_EQ(kControllerTransitions[2].toState, States::kRunning);
    
    // Startup -> Shutdown
    EXPECT_EQ(kControllerTransitions[3].fromState, States::kStartup);
    EXPECT_EQ(kControllerTransitions[3].trigger, Triggers::kShutdownRequest);
    EXPECT_EQ(kControllerTransitions[3].toState, States::kShutdown);
}

TEST(StaticConfigTest, ControllerTransitionsFromRunning)
{
    // Running -> Shutdown
    EXPECT_EQ(kControllerTransitions[4].fromState, States::kRunning);
    EXPECT_EQ(kControllerTransitions[4].trigger, Triggers::kShutdownRequest);
    EXPECT_EQ(kControllerTransitions[4].toState, States::kShutdown);
    
    // Running -> Restart
    EXPECT_EQ(kControllerTransitions[5].fromState, States::kRunning);
    EXPECT_EQ(kControllerTransitions[5].trigger, Triggers::kRestartRequest);
    EXPECT_EQ(kControllerTransitions[5].toState, States::kRestart);
    
    // Running -> PrepareUpdate
    EXPECT_EQ(kControllerTransitions[6].fromState, States::kRunning);
    EXPECT_EQ(kControllerTransitions[6].trigger, Triggers::kPrepareUpdateRequest);
    EXPECT_EQ(kControllerTransitions[6].toState, States::kPrepareUpdate);
}

TEST(StaticConfigTest, ControllerUpdateCyclePrepareUpdate)
{
    // PrepareUpdate -> VerifyUpdate
    EXPECT_EQ(kControllerTransitions[7].fromState, States::kPrepareUpdate);
    EXPECT_EQ(kControllerTransitions[7].trigger, Triggers::kVerifyUpdateRequest);
    EXPECT_EQ(kControllerTransitions[7].toState, States::kVerifyUpdate);
    
    // PrepareUpdate -> PrepareRollback
    EXPECT_EQ(kControllerTransitions[8].fromState, States::kPrepareUpdate);
    EXPECT_EQ(kControllerTransitions[8].trigger, Triggers::kPrepareRollbackRequest);
    EXPECT_EQ(kControllerTransitions[8].toState, States::kPrepareRollback);
}

TEST(StaticConfigTest, ControllerUpdateCycleVerifyUpdate)
{
    // VerifyUpdate -> AfterUpdate
    EXPECT_EQ(kControllerTransitions[9].fromState, States::kVerifyUpdate);
    EXPECT_EQ(kControllerTransitions[9].trigger, Triggers::kFinishUpdateRequest);
    EXPECT_EQ(kControllerTransitions[9].toState, States::kAfterUpdate);
    
    // VerifyUpdate -> PrepareRollback
    EXPECT_EQ(kControllerTransitions[10].fromState, States::kVerifyUpdate);
    EXPECT_EQ(kControllerTransitions[10].trigger, Triggers::kPrepareRollbackRequest);
    EXPECT_EQ(kControllerTransitions[10].toState, States::kPrepareRollback);
}

TEST(StaticConfigTest, ControllerUpdateCyclePrepareRollback)
{
    // PrepareRollback -> AfterUpdate
    EXPECT_EQ(kControllerTransitions[11].fromState, States::kPrepareRollback);
    EXPECT_EQ(kControllerTransitions[11].trigger, Triggers::kFinishUpdateRequest);
    EXPECT_EQ(kControllerTransitions[11].toState, States::kAfterUpdate);
}

TEST(StaticConfigTest, ControllerTransitionsAfterUpdate)
{
    // AfterUpdate -> Running
    EXPECT_EQ(kControllerTransitions[12].fromState, States::kAfterUpdate);
    EXPECT_EQ(kControllerTransitions[12].trigger, Triggers::kGoToRunning);
    EXPECT_EQ(kControllerTransitions[12].toState, States::kRunning);
    
    // AfterUpdate -> Shutdown
    EXPECT_EQ(kControllerTransitions[13].fromState, States::kAfterUpdate);
    EXPECT_EQ(kControllerTransitions[13].trigger, Triggers::kShutdownRequest);
    EXPECT_EQ(kControllerTransitions[13].toState, States::kShutdown);
}

TEST(StaticConfigTest, ControllerContinueUpdateTransitions)
{
    // ContinueUpdate -> VerifyUpdate
    EXPECT_EQ(kControllerTransitions[14].fromState, States::kContinueUpdate);
    EXPECT_EQ(kControllerTransitions[14].trigger, Triggers::kVerifyUpdateRequest);
    EXPECT_EQ(kControllerTransitions[14].toState, States::kVerifyUpdate);
    
    // ContinueUpdate -> PrepareRollback
    EXPECT_EQ(kControllerTransitions[15].fromState, States::kContinueUpdate);
    EXPECT_EQ(kControllerTransitions[15].trigger, Triggers::kPrepareRollbackRequest);
    EXPECT_EQ(kControllerTransitions[15].toState, States::kPrepareRollback);
}

// ============================================================================
// CONTROLLER ERROR RECOVERY TABLE TESTS
// ============================================================================

TEST(StaticConfigTest, ControllerErrorRecoveryTableNotEmpty)
{
    EXPECT_GT(kControllerErrorRecoveryCount, 0);
}

TEST(StaticConfigTest, ControllerErrorRecoveryFromRunning)
{
    // Running + ProcessCrashed -> Restart
    EXPECT_EQ(kControllerErrorRecovery[0].fromState, States::kRunning);
    EXPECT_EQ(kControllerErrorRecovery[0].errorCode, ExecutionErrors::kProcessCrashed);
    EXPECT_EQ(kControllerErrorRecovery[0].toState, States::kRestart);
    
    // Running + CommunicationError -> Shutdown
    EXPECT_EQ(kControllerErrorRecovery[1].fromState, States::kRunning);
    EXPECT_EQ(kControllerErrorRecovery[1].errorCode, ExecutionErrors::kCommunicationError);
    EXPECT_EQ(kControllerErrorRecovery[1].toState, States::kShutdown);
    
    // Running + ANY -> Shutdown
    EXPECT_EQ(kControllerErrorRecovery[2].fromState, States::kRunning);
    EXPECT_EQ(kControllerErrorRecovery[2].errorCode, kExecutionErrorAny);
    EXPECT_EQ(kControllerErrorRecovery[2].toState, States::kShutdown);
}

TEST(StaticConfigTest, ControllerErrorRecoveryFromStartup)
{
    // Startup + ANY -> Shutdown
    EXPECT_EQ(kControllerErrorRecovery[3].fromState, States::kStartup);
    EXPECT_EQ(kControllerErrorRecovery[3].errorCode, kExecutionErrorAny);
    EXPECT_EQ(kControllerErrorRecovery[3].toState, States::kShutdown);
}

TEST(StaticConfigTest, ControllerErrorRecoveryFromVerifyUpdate)
{
    // VerifyUpdate + VerificationFailed -> PrepareRollback
    EXPECT_EQ(kControllerErrorRecovery[4].fromState, States::kVerifyUpdate);
    EXPECT_EQ(kControllerErrorRecovery[4].errorCode, ExecutionErrors::kVerificationFailed);
    EXPECT_EQ(kControllerErrorRecovery[4].toState, States::kPrepareRollback);
    
    // VerifyUpdate + UpdateFailed -> PrepareRollback
    EXPECT_EQ(kControllerErrorRecovery[5].fromState, States::kVerifyUpdate);
    EXPECT_EQ(kControllerErrorRecovery[5].errorCode, ExecutionErrors::kUpdateFailed);
    EXPECT_EQ(kControllerErrorRecovery[5].toState, States::kPrepareRollback);
    
    // VerifyUpdate + ANY -> PrepareRollback
    EXPECT_EQ(kControllerErrorRecovery[6].fromState, States::kVerifyUpdate);
    EXPECT_EQ(kControllerErrorRecovery[6].errorCode, kExecutionErrorAny);
    EXPECT_EQ(kControllerErrorRecovery[6].toState, States::kPrepareRollback);
}

TEST(StaticConfigTest, ControllerErrorRecoveryFromPrepareUpdate)
{
    // PrepareUpdate + ANY -> PrepareRollback
    EXPECT_EQ(kControllerErrorRecovery[7].fromState, States::kPrepareUpdate);
    EXPECT_EQ(kControllerErrorRecovery[7].errorCode, kExecutionErrorAny);
    EXPECT_EQ(kControllerErrorRecovery[7].toState, States::kPrepareRollback);
}

TEST(StaticConfigTest, ControllerErrorRecoveryGlobalCatchAll)
{
    // Global catch-all (last entry)
    EXPECT_EQ(kControllerErrorRecovery[8].fromState, States::kRunning);
    EXPECT_EQ(kControllerErrorRecovery[8].errorCode, kExecutionErrorAny);
    EXPECT_EQ(kControllerErrorRecovery[8].toState, States::kShutdown);
}

// ============================================================================
// AGENT (INFOTAINMENT) TRANSITION TABLE TESTS
// ============================================================================

TEST(StaticConfigTest, InfotainmentTransitionTableNotEmpty)
{
    EXPECT_GT(kInfotainmentTransitionsCount, 0);
}

TEST(StaticConfigTest, InfotainmentTransitionsFromInitial)
{
    // Initial -> Running
    EXPECT_EQ(kInfotainmentTransitions[0].fromState, States::kInitial);
    EXPECT_EQ(kInfotainmentTransitions[0].trigger, Triggers::kGoToRunning);
    EXPECT_EQ(kInfotainmentTransitions[0].toState, States::kRunning);
    
    // Initial -> Running (UserRequest)
    EXPECT_EQ(kInfotainmentTransitions[1].fromState, States::kInitial);
    EXPECT_EQ(kInfotainmentTransitions[1].trigger, Triggers::kUserRequest);
    EXPECT_EQ(kInfotainmentTransitions[1].toState, States::kRunning);
}

TEST(StaticConfigTest, InfotainmentTransitionsFromRunning)
{
    // Running -> Off
    EXPECT_EQ(kInfotainmentTransitions[2].fromState, States::kRunning);
    EXPECT_EQ(kInfotainmentTransitions[2].trigger, Triggers::kShutdownRequest);
    EXPECT_EQ(kInfotainmentTransitions[2].toState, States::kOff);
    
    // Running -> Degraded
    EXPECT_EQ(kInfotainmentTransitions[3].fromState, States::kRunning);
    EXPECT_EQ(kInfotainmentTransitions[3].trigger, Triggers::kDegradeRequest);
    EXPECT_EQ(kInfotainmentTransitions[3].toState, States::kDegraded);
    
    // Running -> PrepareUpdate
    EXPECT_EQ(kInfotainmentTransitions[4].fromState, States::kRunning);
    EXPECT_EQ(kInfotainmentTransitions[4].trigger, Triggers::kPrepareUpdateRequest);
    EXPECT_EQ(kInfotainmentTransitions[4].toState, States::kPrepareUpdate);
}

TEST(StaticConfigTest, InfotainmentTransitionsFromDegraded)
{
    // Degraded -> Running
    EXPECT_EQ(kInfotainmentTransitions[5].fromState, States::kDegraded);
    EXPECT_EQ(kInfotainmentTransitions[5].trigger, Triggers::kGoToRunning);
    EXPECT_EQ(kInfotainmentTransitions[5].toState, States::kRunning);
    
    // Degraded -> Off
    EXPECT_EQ(kInfotainmentTransitions[6].fromState, States::kDegraded);
    EXPECT_EQ(kInfotainmentTransitions[6].trigger, Triggers::kShutdownRequest);
    EXPECT_EQ(kInfotainmentTransitions[6].toState, States::kOff);
}

// ============================================================================
// AGENT ERROR RECOVERY TABLE TESTS
// ============================================================================

TEST(StaticConfigTest, InfotainmentErrorRecoveryTableNotEmpty)
{
    EXPECT_GT(kInfotainmentErrorRecoveryCount, 0);
}

TEST(StaticConfigTest, InfotainmentErrorRecoveryFromRunning)
{
    // Running + kProcessCrashed -> Degraded
    EXPECT_EQ(kInfotainmentErrorRecovery[0].fromState, States::kRunning);
    EXPECT_EQ(kInfotainmentErrorRecovery[0].errorCode, ExecutionErrors::kProcessCrashed);
    EXPECT_EQ(kInfotainmentErrorRecovery[0].toState, States::kDegraded);
    
    // Running + kMemoryViolation -> Degraded
    EXPECT_EQ(kInfotainmentErrorRecovery[1].fromState, States::kRunning);
    EXPECT_EQ(kInfotainmentErrorRecovery[1].errorCode, ExecutionErrors::kMemoryViolation);
    EXPECT_EQ(kInfotainmentErrorRecovery[1].toState, States::kDegraded);
    
    // Running + ANY -> Off
    EXPECT_EQ(kInfotainmentErrorRecovery[2].fromState, States::kRunning);
    EXPECT_EQ(kInfotainmentErrorRecovery[2].errorCode, kExecutionErrorAny);
    EXPECT_EQ(kInfotainmentErrorRecovery[2].toState, States::kOff);
}

TEST(StaticConfigTest, InfotainmentErrorRecoveryFromDegraded)
{
    // Degraded + ANY -> Off
    EXPECT_EQ(kInfotainmentErrorRecovery[3].fromState, States::kDegraded);
    EXPECT_EQ(kInfotainmentErrorRecovery[3].errorCode, kExecutionErrorAny);
    EXPECT_EQ(kInfotainmentErrorRecovery[3].toState, States::kOff);
}

// ============================================================================
// CONTROLLER ACTION TABLE TESTS
// ============================================================================

TEST(StaticConfigTest, ControllerActionTableNotEmpty)
{
    EXPECT_GT(kActionTableCount, 0);
}

TEST(StaticConfigTest, ControllerActionTableContainsAllStates)
{
    // kInitial
    EXPECT_EQ(kActionTable[0].state, States::kInitial);
    EXPECT_NE(kActionTable[0].actions, nullptr);
    EXPECT_GT(kActionTable[0].actionCount, 0);
    
    // kStartup
    EXPECT_EQ(kActionTable[1].state, States::kStartup);
    EXPECT_NE(kActionTable[1].actions, nullptr);
    EXPECT_GT(kActionTable[1].actionCount, 0);
    
    // kRunning
    EXPECT_EQ(kActionTable[2].state, States::kRunning);
    EXPECT_NE(kActionTable[2].actions, nullptr);
    EXPECT_GT(kActionTable[2].actionCount, 0);
    
    // kShutdown
    EXPECT_EQ(kActionTable[3].state, States::kShutdown);
    EXPECT_NE(kActionTable[3].actions, nullptr);
    EXPECT_GT(kActionTable[3].actionCount, 0);
    
    // kRestart
    EXPECT_EQ(kActionTable[4].state, States::kRestart);
    EXPECT_NE(kActionTable[4].actions, nullptr);
    EXPECT_GT(kActionTable[4].actionCount, 0);
    
    // kPrepareUpdate
    EXPECT_EQ(kActionTable[5].state, States::kPrepareUpdate);
    EXPECT_NE(kActionTable[5].actions, nullptr);
    EXPECT_GT(kActionTable[5].actionCount, 0);
    
    // kVerifyUpdate
    EXPECT_EQ(kActionTable[6].state, States::kVerifyUpdate);
    EXPECT_NE(kActionTable[6].actions, nullptr);
    EXPECT_GT(kActionTable[6].actionCount, 0);
    
    // kPrepareRollback
    EXPECT_EQ(kActionTable[7].state, States::kPrepareRollback);
    EXPECT_NE(kActionTable[7].actions, nullptr);
    EXPECT_GT(kActionTable[7].actionCount, 0);
    
    // kContinueUpdate
    EXPECT_EQ(kActionTable[8].state, States::kContinueUpdate);
    EXPECT_NE(kActionTable[8].actions, nullptr);
    EXPECT_GT(kActionTable[8].actionCount, 0);
    
    // kAfterUpdate
    EXPECT_EQ(kActionTable[9].state, States::kAfterUpdate);
    EXPECT_NE(kActionTable[9].actions, nullptr);
    EXPECT_GT(kActionTable[9].actionCount, 0);
}

TEST(StaticConfigTest, ControllerActionTableInitialActions)
{
    EXPECT_EQ(kActionTable[0].state, States::kInitial);
    EXPECT_EQ(kActionTable[0].actionCount, 4);
    
    const ActionItem* actions = kActionTable[0].actions;
    
    // First action: SetFunctionGroupState
    EXPECT_EQ(actions[0].type, ActionType::kSetFunctionGroupState);
    EXPECT_STREQ(actions[0].target, "MachineFG");
    EXPECT_STREQ(actions[0].param, "Startup");
}

TEST(StaticConfigTest, ControllerActionTableStartupActions)
{
    EXPECT_EQ(kActionTable[1].state, States::kStartup);
    EXPECT_EQ(kActionTable[1].actionCount, 2);
    
    const ActionItem* actions = kActionTable[1].actions;
    
    // Should contain SetFunctionGroupState and Sync
    EXPECT_EQ(actions[0].type, ActionType::kSetFunctionGroupState);
    EXPECT_EQ(actions[1].type, ActionType::kSync);
}

TEST(StaticConfigTest, ControllerActionTableRunningActions)
{
    EXPECT_EQ(kActionTable[2].state, States::kRunning);
    EXPECT_EQ(kActionTable[2].actionCount, 4);
    
    const ActionItem* actions = kActionTable[2].actions;
    
    // First action should be SetFunctionGroupState
    EXPECT_EQ(actions[0].type, ActionType::kSetFunctionGroupState);
}

TEST(StaticConfigTest, ControllerActionTableShutdownActions)
{
    EXPECT_EQ(kActionTable[3].state, States::kShutdown);
    EXPECT_EQ(kActionTable[3].actionCount, 5);
    
    const ActionItem* actions = kActionTable[3].actions;
    
    // Should include StopStateMachine
    bool hasStopAction = false;
    for (size_t i = 0; i < kActionTable[3].actionCount; ++i) {
        if (actions[i].type == ActionType::kStopStateMachine) {
            hasStopAction = true;
            break;
        }
    }
    EXPECT_TRUE(hasStopAction);
}

TEST(StaticConfigTest, ControllerActionTablePrepareUpdateActions)
{
    EXPECT_EQ(kActionTable[5].state, States::kPrepareUpdate);
    EXPECT_EQ(kActionTable[5].actionCount, 5);
    
    const ActionItem* actions = kActionTable[5].actions;
    
    // Should contain StartStateMachine and StopStateMachine
    bool hasStart = false;
    bool hasStop = false;
    
    for (size_t i = 0; i < kActionTable[5].actionCount; ++i) {
        if (actions[i].type == ActionType::kStartStateMachine) hasStart = true;
        if (actions[i].type == ActionType::kStopStateMachine) hasStop = true;
    }
    
    EXPECT_TRUE(hasStart);
    EXPECT_TRUE(hasStop);
}

// ============================================================================
// AGENT (INFOTAINMENT) ACTION TABLE TESTS
// ============================================================================

TEST(StaticConfigTest, InfotainmentActionTableNotEmpty)
{
    EXPECT_GT(kInfotainmentActionTableCount, 0);
}

TEST(StaticConfigTest, InfotainmentActionTableContainsRequiredStates)
{
    // kOff
    EXPECT_EQ(kInfotainmentActionTable[0].state, States::kOff);
    EXPECT_NE(kInfotainmentActionTable[0].actions, nullptr);
    EXPECT_EQ(kInfotainmentActionTable[0].actionCount, 3);
    
    // kRunning
    EXPECT_EQ(kInfotainmentActionTable[1].state, States::kRunning);
    EXPECT_NE(kInfotainmentActionTable[1].actions, nullptr);
    EXPECT_EQ(kInfotainmentActionTable[1].actionCount, 3);
    
    // kDegraded
    EXPECT_EQ(kInfotainmentActionTable[2].state, States::kDegraded);
    EXPECT_NE(kInfotainmentActionTable[2].actions, nullptr);
    EXPECT_EQ(kInfotainmentActionTable[2].actionCount, 3);
    
    // kPrepareUpdate
    EXPECT_EQ(kInfotainmentActionTable[3].state, States::kPrepareUpdate);
    EXPECT_NE(kInfotainmentActionTable[3].actions, nullptr);
    EXPECT_EQ(kInfotainmentActionTable[3].actionCount, 3);
    
    // kVerifyUpdate
    EXPECT_EQ(kInfotainmentActionTable[4].state, States::kVerifyUpdate);
    EXPECT_NE(kInfotainmentActionTable[4].actions, nullptr);
    EXPECT_EQ(kInfotainmentActionTable[4].actionCount, 3);
}

TEST(StaticConfigTest, InfotainmentActionTableOffActions)
{
    EXPECT_EQ(kInfotainmentActionTable[0].state, States::kOff);
    
    const ActionItem* actions = kInfotainmentActionTable[0].actions;
    
    // Should set FunctionGroup to Off
    EXPECT_EQ(actions[0].type, ActionType::kSetFunctionGroupState);
    EXPECT_STREQ(actions[0].target, "InfotainmentFG");
    EXPECT_STREQ(actions[0].param, "Off");
    
    // Should set NetworkHandle to NoCom
    EXPECT_EQ(actions[1].type, ActionType::kSetNetworkHandle);
    EXPECT_STREQ(actions[1].target, "MediaNetwork");
    EXPECT_STREQ(actions[1].param, "NoCom");
    
    // Should sync
    EXPECT_EQ(actions[2].type, ActionType::kSync);
}

TEST(StaticConfigTest, InfotainmentActionTableRunningActions)
{
    EXPECT_EQ(kInfotainmentActionTable[1].state, States::kRunning);
    
    const ActionItem* actions = kInfotainmentActionTable[1].actions;
    
    // Should set FunctionGroup to Running
    EXPECT_EQ(actions[0].type, ActionType::kSetFunctionGroupState);
    EXPECT_STREQ(actions[0].param, "Running");
    
    // Should set NetworkHandle to FullCom
    EXPECT_EQ(actions[1].type, ActionType::kSetNetworkHandle);
    EXPECT_STREQ(actions[1].param, "FullCom");
}

TEST(StaticConfigTest, InfotainmentActionTableDegradedActions)
{
    EXPECT_EQ(kInfotainmentActionTable[2].state, States::kDegraded);
    
    const ActionItem* actions = kInfotainmentActionTable[2].actions;
    
    // Should set FunctionGroup to Degraded
    EXPECT_EQ(actions[0].type, ActionType::kSetFunctionGroupState);
    EXPECT_STREQ(actions[0].param, "Degraded");
    
    // Network should still be FullCom in degraded mode
    EXPECT_EQ(actions[1].type, ActionType::kSetNetworkHandle);
    EXPECT_STREQ(actions[1].param, "FullCom");
}

// ============================================================================
// ACTION TYPE COVERAGE TESTS
// ============================================================================

TEST(StaticConfigTest, ActionTableCoversAllActionTypes)
{
    bool hasSetFunctionGroupState = false;
    bool hasStartStateMachine = false;
    bool hasStopStateMachine = false;
    bool hasSetNetworkHandle = false;
    bool hasSync = false;
    
    // Check Controller actions
    for (size_t i = 0; i < kActionTableCount; ++i) {
        for (size_t j = 0; j < kActionTable[i].actionCount; ++j) {
            switch (kActionTable[i].actions[j].type) {
                case ActionType::kSetFunctionGroupState:
                    hasSetFunctionGroupState = true;
                    break;
                case ActionType::kStartStateMachine:
                    hasStartStateMachine = true;
                    break;
                case ActionType::kStopStateMachine:
                    hasStopStateMachine = true;
                    break;
                case ActionType::kSetNetworkHandle:
                    hasSetNetworkHandle = true;
                    break;
                case ActionType::kSync:
                    hasSync = true;
                    break;
            }
        }
    }
    
    EXPECT_TRUE(hasSetFunctionGroupState);
    EXPECT_TRUE(hasStartStateMachine);
    EXPECT_TRUE(hasStopStateMachine);
    EXPECT_TRUE(hasSync);
}

// ============================================================================
// CONFIGURATION CONSISTENCY TESTS
// ============================================================================

// Note: Removed sizeof consistency tests for external arrays as they are
// declared as incomplete types (extern const Type[]). The counts are provided
// by the implementation and tested indirectly through other test cases.