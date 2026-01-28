#include <gtest/gtest.h>

#include "transition_table.h"
#include "types.h"
#include "state_machine.h"

using ara::sm::TransitionRequestType;
using ara::sm::TransitionTable;
using ara::sm::StateMachine;

/**
 * @brief Unit tests for TransitionTable (AUTOSAR Adaptive compliant)
 */
class TransitionTableTest : public ::testing::Test
{
protected:
    TransitionTable table;
};

TEST_F(TransitionTableTest, ValidTransitionIsAllowed_Controller)
{
    const bool result = table.IsTransitionAllowed(
        /* currentState */ 0U,
        /* request */ static_cast<TransitionRequestType>(1U),
        /* category */ StateMachine::Category::kController
    );

    EXPECT_TRUE(result);
}

TEST_F(TransitionTableTest, InvalidTransitionIsRejected_Controller)
{
    const bool result = table.IsTransitionAllowed(
        /* currentState */ 0U,
        /* request */ static_cast<TransitionRequestType>(999U),
        /* category */ StateMachine::Category::kController
    );

    EXPECT_FALSE(result);
}

TEST_F(TransitionTableTest, InvalidTransitionIsRejected_Agent)
{
    const bool result = table.IsTransitionAllowed(
        /* currentState */ 0U,
        /* request */ static_cast<TransitionRequestType>(999U),
        /* category */ StateMachine::Category::kAgent
    );

    EXPECT_FALSE(result);
}
