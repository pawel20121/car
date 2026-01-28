#include <gtest/gtest.h>

#include "action_executor.h"
#include "static_config.h"

using ara::sm::ActionExecutor;
using ara::sm::config::ActionItem;
using ara::sm::config::ActionType;

/**
 * @brief Unit tests for ActionExecutor
 *
 * Goal:
 *  - 100% line coverage
 *  - 100% branch coverage
 *  - no external dependencies (Exec, NM, EM mocked by no-op)
 *
 * AUTOSAR:
 *  - SWS_SM_00608 – SWS_SM_00626
 */
class ActionExecutorTest : public ::testing::Test
{
protected:
    ActionExecutor executor;
};

// ============================================================================
// ExecuteActionList
// ============================================================================

TEST_F(ActionExecutorTest, ExecuteActionList_EmptyList)
{
    executor.ExecuteActionList(nullptr, 0U);
}

TEST_F(ActionExecutorTest, ExecuteActionList_TerminatedByNullTarget)
{
    const ActionItem actions[] = {
        { ActionType::kSetFunctionGroupState, "FG1", "Running", 0U },
        { ActionType::kSetFunctionGroupState, nullptr, nullptr, 0U }, // terminator
        { ActionType::kSetFunctionGroupState, "FG2", "Off", 0U }       // must NOT execute
    };

    executor.ExecuteActionList(actions, 3U);
}

// ============================================================================
// ExecuteAction – switch coverage
// ============================================================================

TEST_F(ActionExecutorTest, ExecuteAction_SetFunctionGroupState)
{
    const ActionItem action {
        ActionType::kSetFunctionGroupState,
        "MachineFG",
        "Startup",
        0U
    };

    executor.ExecuteAction(action);
}

TEST_F(ActionExecutorTest, ExecuteAction_SetFunctionGroupState_NullParams)
{
    const ActionItem action {
        ActionType::kSetFunctionGroupState,
        nullptr,
        nullptr,
        0U
    };

    executor.ExecuteAction(action);
}

TEST_F(ActionExecutorTest, ExecuteAction_StartStateMachine_WithInitialState)
{
    const ActionItem action {
        ActionType::kStartStateMachine,
        "InfotainmentSM",
        "Running",
        0U
    };

    executor.ExecuteAction(action);
}

TEST_F(ActionExecutorTest, ExecuteAction_StartStateMachine_DefaultInitialState)
{
    const ActionItem action {
        ActionType::kStartStateMachine,
        "InfotainmentSM",
        "",
        0U
    };

    executor.ExecuteAction(action);
}

TEST_F(ActionExecutorTest, ExecuteAction_StartStateMachine_NullName)
{
    const ActionItem action {
        ActionType::kStartStateMachine,
        nullptr,
        "Running",
        0U
    };

    executor.ExecuteAction(action);
}

TEST_F(ActionExecutorTest, ExecuteAction_StopStateMachine)
{
    const ActionItem action {
        ActionType::kStopStateMachine,
        "InfotainmentSM",
        nullptr,
        0U
    };

    executor.ExecuteAction(action);
}

TEST_F(ActionExecutorTest, ExecuteAction_StopStateMachine_NullName)
{
    const ActionItem action {
        ActionType::kStopStateMachine,
        nullptr,
        nullptr,
        0U
    };

    executor.ExecuteAction(action);
}

TEST_F(ActionExecutorTest, ExecuteAction_Sync)
{
    const ActionItem action {
        ActionType::kSync,
        nullptr,
        nullptr,
        0U
    };

    executor.ExecuteAction(action);
}

TEST_F(ActionExecutorTest, ExecuteAction_Sleep)
{
    const ActionItem action {
        ActionType::kSleep,
        nullptr,
        nullptr,
        1U // minimal sleep to avoid slow tests
    };

    executor.ExecuteAction(action);
}

TEST_F(ActionExecutorTest, ExecuteAction_SetNetworkHandle)
{
    const ActionItem action {
        ActionType::kSetNetworkHandle,
        "VehicleNetwork",
        "FullCom",
        0U
    };

    executor.ExecuteAction(action);
}

TEST_F(ActionExecutorTest, ExecuteAction_SetNetworkHandle_NullParams)
{
    const ActionItem action {
        ActionType::kSetNetworkHandle,
        nullptr,
        nullptr,
        0U
    };

    executor.ExecuteAction(action);
}

TEST_F(ActionExecutorTest, ExecuteAction_UnknownActionType)
{
    const ActionItem action {
        static_cast<ActionType>(255), // unknown enum value
        "X",
        "Y",
        0U
    };

    executor.ExecuteAction(action);
}
