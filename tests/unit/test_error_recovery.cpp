#include <gtest/gtest.h>

#include "error_recovery.h"
#include "static_config.h"
#include "state_machine.h"

using ara::sm::ErrorRecoveryTable;
using ara::sm::ExecutionErrorType;
using ara::sm::StateMachine;
namespace cfg = ara::sm::config;
/**
 * @brief Unit tests for ErrorRecoveryTable
 *
 * Goal:
 *  - 100% line coverage
 *  - 100% branch coverage
 *
 * AUTOSAR:
 *  - SWS_SM_00601
 *  - SWS_SM_CONSTR_00014
 */
class ErrorRecoveryTableTest : public ::testing::Test
{
};

/**
 * Controller: exact error match
 */
TEST_F(ErrorRecoveryTableTest, Controller_ExactMatch)
{
    const uint8_t currentState =
        static_cast<uint8_t>(cfg::kControllerErrorRecovery[0].fromState);

    const ExecutionErrorType error =
        cfg::kControllerErrorRecovery[0].errorCode;

    const uint8_t expected =
        static_cast<uint8_t>(cfg::kControllerErrorRecovery[0].toState);

    const uint8_t result =
        ErrorRecoveryTable::GetRecoveryState(
            currentState,
            error,
            StateMachine::Category::kController);

    EXPECT_EQ(result, expected);
}

/**
 * Controller: no exact match, catch-all used
 */
TEST_F(ErrorRecoveryTableTest, Controller_CatchAll)
{
    const uint8_t currentState =
        static_cast<uint8_t>(cfg::kControllerErrorRecovery[0].fromState);

    const ExecutionErrorType unknownError = 0x12345678U;

    uint8_t expected = currentState;

    for (size_t i = 0; i < cfg::kControllerErrorRecoveryCount; ++i) {
        const auto& rule = cfg::kControllerErrorRecovery[i];
        if (static_cast<uint8_t>(rule.fromState) == currentState &&
            rule.errorCode == cfg::kExecutionErrorAny) {
            expected = static_cast<uint8_t>(rule.toState);
            break;
        }
    }

    const uint8_t result =
        ErrorRecoveryTable::GetRecoveryState(
            currentState,
            unknownError,
            StateMachine::Category::kController);

    EXPECT_EQ(result, expected);
}

/**
 * Controller: no rule matches (fallback to current state)
 */
TEST_F(ErrorRecoveryTableTest, Controller_NoMatch_Fallback)
{
    const uint8_t currentState = 0xFFU; // state not present in config

    const uint8_t result =
        ErrorRecoveryTable::GetRecoveryState(
            currentState,
            0xDEADBEEFU,
            StateMachine::Category::kController);

    EXPECT_EQ(result, currentState);
}

/**
 * Agent: exact error match
 */
TEST_F(ErrorRecoveryTableTest, Agent_ExactMatch)
{
    const uint8_t currentState =
        static_cast<uint8_t>(cfg::kInfotainmentErrorRecovery[0].fromState);

    const ExecutionErrorType error =
        cfg::kInfotainmentErrorRecovery[0].errorCode;

    const uint8_t expected =
        static_cast<uint8_t>(cfg::kInfotainmentErrorRecovery[0].toState);

    const uint8_t result =
        ErrorRecoveryTable::GetRecoveryState(
            currentState,
            error,
            StateMachine::Category::kAgent);

    EXPECT_EQ(result, expected);
}

/**
 * Agent: catch-all rule
 */
TEST_F(ErrorRecoveryTableTest, Agent_CatchAll)
{
    const uint8_t currentState =
        static_cast<uint8_t>(cfg::kInfotainmentErrorRecovery[0].fromState);

    const ExecutionErrorType unknownError = 0xCAFEBABEU;

    uint8_t expected = currentState;

    for (size_t i = 0; i < cfg::kInfotainmentErrorRecoveryCount; ++i) {
        const auto& rule = cfg::kInfotainmentErrorRecovery[i];
        if (static_cast<uint8_t>(rule.fromState) == currentState &&
            rule.errorCode == cfg::kExecutionErrorAny) {
            expected = static_cast<uint8_t>(rule.toState);
            break;
        }
    }

    const uint8_t result =
        ErrorRecoveryTable::GetRecoveryState(
            currentState,
            unknownError,
            StateMachine::Category::kAgent);

    EXPECT_EQ(result, expected);
}

/**
 * Agent: no rule matches
 */
TEST_F(ErrorRecoveryTableTest, Agent_NoMatch_Fallback)
{
    const uint8_t currentState = 0xEEU;

    const uint8_t result =
        ErrorRecoveryTable::GetRecoveryState(
            currentState,
            0xABCDU,
            StateMachine::Category::kAgent);

    EXPECT_EQ(result, currentState);
}
