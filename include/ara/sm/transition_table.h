#ifndef ARA_SM_TRANSITION_TABLE_H
#define ARA_SM_TRANSITION_TABLE_H

#include "ara/sm/types.h"
#include "ara/sm/state_machine.h"
#include "static_config.h"

namespace ara {
namespace sm {

class TransitionTable {
public:
    /**
     * @brief Check if transition is allowed
     * 
     * @param currentState Current StateMachine state
     * @param request Transition request value
     * @param category Controller or Agent
     * @return true if transition exists in table
     */

    static bool IsTransitionAllowed(
        uint8_t currentState,
        TransitionRequestType request,
        StateMachine::Category category);

        
    /**
     * @brief Get next state for transition
     * 
     * @param currentState Current StateMachine state
     * @param request Transition request value
     * @param category Controller or Agent
     * @return Next state (or current if not found)
     */
    
    static uint8_t GetNextState(
        uint8_t currentState,
        TransitionRequestType request,
        StateMachine::Category category);
};

} // namespace sm
} // namespace ara

#endif