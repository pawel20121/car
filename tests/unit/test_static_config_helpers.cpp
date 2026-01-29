#include <gtest/gtest.h>

#include "static_config.h"
#include "types.h"

using namespace ara::sm::config;
using ara::sm::TransitionRequestType;  // Add this line

// ============================================================================
// StateIdToString TESTS
// ============================================================================

TEST(StaticConfigHelpersTest, StateIdToStringCommonStates)
{
    EXPECT_STREQ(StateIdToString(States::kInitial), "Initial");
    EXPECT_STREQ(StateIdToString(States::kOff), "Off");
    EXPECT_STREQ(StateIdToString(States::kRunning), "Running");
}

TEST(StaticConfigHelpersTest, StateIdToStringUpdateStates)
{
    EXPECT_STREQ(StateIdToString(States::kPrepareUpdate), "PrepareUpdate");
    EXPECT_STREQ(StateIdToString(States::kVerifyUpdate), "VerifyUpdate");
    EXPECT_STREQ(StateIdToString(States::kPrepareRollback), "PrepareRollback");
}

TEST(StaticConfigHelpersTest, StateIdToStringControllerStates)
{
    EXPECT_STREQ(StateIdToString(States::kStartup), "Startup");
    EXPECT_STREQ(StateIdToString(States::kShutdown), "Shutdown");
    EXPECT_STREQ(StateIdToString(States::kRestart), "Restart");
    EXPECT_STREQ(StateIdToString(States::kContinueUpdate), "ContinueUpdate");
    EXPECT_STREQ(StateIdToString(States::kAfterUpdate), "AfterUpdate");
}

TEST(StaticConfigHelpersTest, StateIdToStringAgentStates)
{
    EXPECT_STREQ(StateIdToString(States::kDegraded), "Degraded");
}

TEST(StaticConfigHelpersTest, StateIdToStringSpecialStates)
{
    EXPECT_STREQ(StateIdToString(States::kInTransition), "InTransition");
    EXPECT_STREQ(StateIdToString(States::kInvalid), "Invalid");
}

TEST(StaticConfigHelpersTest, StateIdToStringUnknownState)
{
    EXPECT_STREQ(StateIdToString(0xDEADBEEF), "Unknown");
    EXPECT_STREQ(StateIdToString(999999), "Unknown");
}

TEST(StaticConfigHelpersTest, StateIdToStringAllDefinedStates)
{
    // Verify all defined states return non-null strings
    const uint32_t definedStates[] = {
        States::kInitial,
        States::kOff,
        States::kRunning,
        States::kPrepareUpdate,
        States::kVerifyUpdate,
        States::kPrepareRollback,
        States::kStartup,
        States::kShutdown,
        States::kRestart,
        States::kContinueUpdate,
        States::kAfterUpdate,
        States::kDegraded,
        States::kInTransition,
        States::kInvalid
    };
    
    for (uint32_t state : definedStates) {
        const char* name = StateIdToString(state);
        EXPECT_NE(name, nullptr);
        EXPECT_STRNE(name, "");
        EXPECT_STRNE(name, "Unknown");
    }
}

// ============================================================================
// TriggerIdToString TESTS
// ============================================================================

TEST(StaticConfigHelpersTest, TriggerIdToStringLifecycleTriggers)
{
    EXPECT_STREQ(TriggerIdToString(Triggers::kStartup), "Startup");
    EXPECT_STREQ(TriggerIdToString(Triggers::kShutdownRequest), "ShutdownRequest");
    EXPECT_STREQ(TriggerIdToString(Triggers::kRestartRequest), "RestartRequest");
    EXPECT_STREQ(TriggerIdToString(Triggers::kGoToRunning), "GoToRunning");
}

TEST(StaticConfigHelpersTest, TriggerIdToStringUpdateTriggers)
{
    EXPECT_STREQ(TriggerIdToString(Triggers::kPrepareUpdateRequest), "PrepareUpdateRequest");
    EXPECT_STREQ(TriggerIdToString(Triggers::kVerifyUpdateRequest), "VerifyUpdateRequest");
    EXPECT_STREQ(TriggerIdToString(Triggers::kPrepareRollbackRequest), "PrepareRollbackRequest");
    EXPECT_STREQ(TriggerIdToString(Triggers::kFinishUpdateRequest), "FinishUpdateRequest");
}

TEST(StaticConfigHelpersTest, TriggerIdToStringNetworkTriggers)
{
    EXPECT_STREQ(TriggerIdToString(Triggers::kNetworkFullCom), "NetworkFullCom");
    EXPECT_STREQ(TriggerIdToString(Triggers::kNetworkNoCom), "NetworkNoCom");
}

TEST(StaticConfigHelpersTest, TriggerIdToStringApplicationTriggers)
{
    EXPECT_STREQ(TriggerIdToString(Triggers::kUserRequest), "UserRequest");
    EXPECT_STREQ(TriggerIdToString(Triggers::kDegradeRequest), "DegradeRequest");
}

TEST(StaticConfigHelpersTest, TriggerIdToStringUnknownTrigger)
{
    EXPECT_STREQ(TriggerIdToString(0xDEADBEEF), "Unknown");
    EXPECT_STREQ(TriggerIdToString(999999), "Unknown");
}

TEST(StaticConfigHelpersTest, TriggerIdToStringAllDefinedTriggers)
{
    // Verify all defined triggers return non-null strings
    const TransitionRequestType definedTriggers[] = {
        Triggers::kStartup,
        Triggers::kShutdownRequest,
        Triggers::kRestartRequest,
        Triggers::kGoToRunning,
        Triggers::kPrepareUpdateRequest,
        Triggers::kVerifyUpdateRequest,
        Triggers::kPrepareRollbackRequest,
        Triggers::kFinishUpdateRequest,
        Triggers::kNetworkFullCom,
        Triggers::kNetworkNoCom,
        Triggers::kUserRequest,
        Triggers::kDegradeRequest
    };
    
    for (TransitionRequestType trigger : definedTriggers) {
        const char* name = TriggerIdToString(trigger);
        EXPECT_NE(name, nullptr);
        EXPECT_STRNE(name, "");
        EXPECT_STRNE(name, "Unknown");
    }
}

// ============================================================================
// ActionTypeToString TESTS
// ============================================================================

TEST(StaticConfigHelpersTest, ActionTypeToStringAllActionTypes)
{
    EXPECT_STREQ(ActionTypeToString(ActionType::kSetFunctionGroupState), "SetFunctionGroupState");
    EXPECT_STREQ(ActionTypeToString(ActionType::kStartStateMachine), "StartStateMachine");
    EXPECT_STREQ(ActionTypeToString(ActionType::kStopStateMachine), "StopStateMachine");
    EXPECT_STREQ(ActionTypeToString(ActionType::kSync), "Sync");
    EXPECT_STREQ(ActionTypeToString(ActionType::kSleep), "Sleep");
    EXPECT_STREQ(ActionTypeToString(ActionType::kSetNetworkHandle), "SetNetworkHandle");
}

TEST(StaticConfigHelpersTest, ActionTypeToStringUnknownAction)
{
    // Cast invalid value to ActionType
    ActionType unknownAction = static_cast<ActionType>(99);
    EXPECT_STREQ(ActionTypeToString(unknownAction), "Unknown");
}

TEST(StaticConfigHelpersTest, ActionTypeToStringAllEnumValues)
{
    // Verify all enum values return non-null strings
    const ActionType definedActions[] = {
        ActionType::kSetFunctionGroupState,
        ActionType::kStartStateMachine,
        ActionType::kStopStateMachine,
        ActionType::kSync,
        ActionType::kSleep,
        ActionType::kSetNetworkHandle
    };
    
    for (ActionType action : definedActions) {
        const char* name = ActionTypeToString(action);
        EXPECT_NE(name, nullptr);
        EXPECT_STRNE(name, "");
        EXPECT_STRNE(name, "Unknown");
    }
}

// ============================================================================
// CONSISTENCY TESTS
// ============================================================================

TEST(StaticConfigHelpersTest, StateIdToStringConsistencyWithStateEnum)
{
    // Verify string representation matches expected naming convention
    EXPECT_STREQ(StateIdToString(States::kInitial), "Initial");
    EXPECT_STREQ(StateIdToString(States::kRunning), "Running");
    
    // The string should match the enum name without 'k' prefix
    const char* initial = StateIdToString(States::kInitial);
    EXPECT_EQ(initial[0], 'I'); // Not 'k'
}

TEST(StaticConfigHelpersTest, TriggerIdToStringConsistencyWithTriggerEnum)
{
    // Verify string representation matches expected naming convention
    const char* startup = TriggerIdToString(Triggers::kStartup);
    EXPECT_EQ(startup[0], 'S'); // Not 'k'
    
    // Request triggers should end with "Request"
    const char* shutdownRequest = TriggerIdToString(Triggers::kShutdownRequest);
    EXPECT_NE(std::string(shutdownRequest).find("Request"), std::string::npos);
}

TEST(StaticConfigHelpersTest, ActionTypeToStringConsistencyWithActionEnum)
{
    // Verify string representation matches expected naming convention
    const char* setFG = ActionTypeToString(ActionType::kSetFunctionGroupState);
    EXPECT_EQ(setFG[0], 'S'); // Not 'k'
    
    // All action names should be CamelCase
    const char* startSM = ActionTypeToString(ActionType::kStartStateMachine);
    EXPECT_EQ(startSM[0], 'S');
}

// ============================================================================
// RETURN VALUE TESTS
// ============================================================================

TEST(StaticConfigHelpersTest, AllFunctionsReturnNonNullStrings)
{
    // StateIdToString
    EXPECT_NE(StateIdToString(States::kInitial), nullptr);
    EXPECT_NE(StateIdToString(0xFFFFFFFF), nullptr); // Even for unknown
    
    // TriggerIdToString
    EXPECT_NE(TriggerIdToString(Triggers::kStartup), nullptr);
    EXPECT_NE(TriggerIdToString(0xFFFFFFFF), nullptr); // Even for unknown
    
    // ActionTypeToString
    EXPECT_NE(ActionTypeToString(ActionType::kSync), nullptr);
    EXPECT_NE(ActionTypeToString(static_cast<ActionType>(99)), nullptr); // Even for unknown
}

TEST(StaticConfigHelpersTest, StringsAreNonEmpty)
{
    // StateIdToString
    EXPECT_GT(std::string(StateIdToString(States::kInitial)).length(), 0);
    EXPECT_GT(std::string(StateIdToString(0xFFFFFFFF)).length(), 0);
    
    // TriggerIdToString
    EXPECT_GT(std::string(TriggerIdToString(Triggers::kStartup)).length(), 0);
    EXPECT_GT(std::string(TriggerIdToString(0xFFFFFFFF)).length(), 0);
    
    // ActionTypeToString
    EXPECT_GT(std::string(ActionTypeToString(ActionType::kSync)).length(), 0);
    EXPECT_GT(std::string(ActionTypeToString(static_cast<ActionType>(99))).length(), 0);
}

// ============================================================================
// EDGE CASE TESTS
// ============================================================================

TEST(StaticConfigHelpersTest, StateIdToStringBoundaryValues)
{
    // Test boundary values
    EXPECT_STREQ(StateIdToString(0), "Initial"); // kInitial = 0
    EXPECT_STREQ(StateIdToString(States::kInTransition), "InTransition");
    EXPECT_STREQ(StateIdToString(States::kInvalid), "Invalid");
}

TEST(StaticConfigHelpersTest, TriggerIdToStringSmallestAndLargestValues)
{
    // Test smallest defined trigger
    EXPECT_STREQ(TriggerIdToString(Triggers::kStartup), "Startup");
    
    // Test largest defined trigger (application-specific)
    EXPECT_STREQ(TriggerIdToString(Triggers::kDegradeRequest), "DegradeRequest");
}

TEST(StaticConfigHelpersTest, MultipleCallsReturnSamePointer)
{
    // Verify string literals are returned (same pointer for same input)
    const char* first = StateIdToString(States::kRunning);
    const char* second = StateIdToString(States::kRunning);
    EXPECT_EQ(first, second); // Should be same string literal
    
    const char* firstTrigger = TriggerIdToString(Triggers::kStartup);
    const char* secondTrigger = TriggerIdToString(Triggers::kStartup);
    EXPECT_EQ(firstTrigger, secondTrigger);
    
    const char* firstAction = ActionTypeToString(ActionType::kSync);
    const char* secondAction = ActionTypeToString(ActionType::kSync);
    EXPECT_EQ(firstAction, secondAction);
}
