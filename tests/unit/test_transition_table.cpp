#include <gtest/gtest.h>

#include "transition_table.h"
#include "static_config.h"
#include "state_machine.h"

using ara::sm::TransitionTable;
using ara::sm::TransitionRequestType;
using ara::sm::StateMachine;

using namespace ara::sm::config;

// ============================================================================
// IsTransitionAllowed — Controller FOUND
// ============================================================================

TEST(TransitionTableTest, IsTransitionAllowed_Controller_Found)
{
    ASSERT_GT(kControllerTransitionsCount, 0U);

    const auto& rule = kControllerTransitions[0];

    const bool allowed = TransitionTable::IsTransitionAllowed(
        static_cast<uint8_t>(rule.fromState),
        rule.trigger,
        StateMachine::Category::kController);

    EXPECT_TRUE(allowed);
}

// ============================================================================
// IsTransitionAllowed — Controller NOT FOUND
// ============================================================================

TEST(TransitionTableTest, IsTransitionAllowed_Controller_NotFound)
{
    const bool allowed = TransitionTable::IsTransitionAllowed(
        0xFF,
        static_cast<TransitionRequestType>(0xFF),
        StateMachine::Category::kController);

    EXPECT_FALSE(allowed);
}

// ============================================================================
// IsTransitionAllowed — Agent FOUND
// ============================================================================

TEST(TransitionTableTest, IsTransitionAllowed_Agent_Found)
{
    ASSERT_GT(kInfotainmentTransitionsCount, 0U);

    const auto& rule = kInfotainmentTransitions[0];

    const bool allowed = TransitionTable::IsTransitionAllowed(
        static_cast<uint8_t>(rule.fromState),
        rule.trigger,
        StateMachine::Category::kAgent);

    EXPECT_TRUE(allowed);
}

// ============================================================================
// IsTransitionAllowed — Agent NOT FOUND
// ============================================================================

TEST(TransitionTableTest, IsTransitionAllowed_Agent_NotFound)
{
    const bool allowed = TransitionTable::IsTransitionAllowed(
        0xEE,
        static_cast<TransitionRequestType>(0xEE),
        StateMachine::Category::kAgent);

    EXPECT_FALSE(allowed);
}

// ============================================================================
// GetNextState — Controller FOUND
// ============================================================================

TEST(TransitionTableTest, GetNextState_Controller_Found)
{
    ASSERT_GT(kControllerTransitionsCount, 0U);

    const auto& rule = kControllerTransitions[0];

    const uint8_t next = TransitionTable::GetNextState(
        static_cast<uint8_t>(rule.fromState),
        rule.trigger,
        StateMachine::Category::kController);

    EXPECT_EQ(next, static_cast<uint8_t>(rule.toState));
}

// ============================================================================
// GetNextState — Agent FOUND
// ============================================================================

TEST(TransitionTableTest, GetNextState_Agent_Found)
{
    ASSERT_GT(kInfotainmentTransitionsCount, 0U);

    const auto& rule = kInfotainmentTransitions[0];

    const uint8_t next = TransitionTable::GetNextState(
        static_cast<uint8_t>(rule.fromState),
        rule.trigger,
        StateMachine::Category::kAgent);

    EXPECT_EQ(next, static_cast<uint8_t>(rule.toState));
}

// ============================================================================
// GetNextState — Controller NOT FOUND → cerr + return currentState
// ============================================================================

TEST(TransitionTableTest, GetNextState_Controller_NotFound_ReturnsCurrent)
{
    const uint8_t current = 0xAA;

    const uint8_t next = TransitionTable::GetNextState(
        current,
        static_cast<TransitionRequestType>(0xAA),
        StateMachine::Category::kController);

    EXPECT_EQ(next, current);
}

// ============================================================================
// GetNextState — Agent NOT FOUND → cerr + return currentState
// ============================================================================

TEST(TransitionTableTest, GetNextState_Agent_NotFound_ReturnsCurrent)
{
    const uint8_t current = 0xBB;

    const uint8_t next = TransitionTable::GetNextState(
        current,
        static_cast<TransitionRequestType>(0xBB),
        StateMachine::Category::kAgent);

    EXPECT_EQ(next, current);
}
