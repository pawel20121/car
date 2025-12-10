#include "ara/sm/transition_table.h"
#include "static_config.h"
#include <iostream>

namespace ara {
namespace sm {

bool TransitionTable::IsTransitionAllowed(
    uint8_t currentState,
    TransitionRequestType request,
    StateMachine::Category category)
{
    if (category == StateMachine::Category::kController) {
        // Search in Controller transition table
        for (size_t i = 0; i < config::kControllerTransitionsCount; i++) {
            const auto& rule = config::kControllerTransitions[i];
            if (static_cast<uint8_t>(rule.fromState) == currentState &&
                rule.triggerValue == request) {
                return true;
            }
        }
    } else {
        // Search in Agent transition table
        for (size_t i = 0; i < config::kInfotainmentTransitionsCount; i++) {
            const auto& rule = config::kInfotainmentTransitions[i];
            if (static_cast<uint8_t>(rule.fromState) == currentState &&
                rule.triggerValue == request) {
                return true;
            }
        }
    }
    
    return false;
}

uint8_t TransitionTable::GetNextState(
    uint8_t currentState,
    TransitionRequestType request,
    StateMachine::Category category)
{
    if (category == StateMachine::Category::kController) {
        for (size_t i = 0; i < config::kControllerTransitionsCount; i++) {
            const auto& rule = config::kControllerTransitions[i];
            if (static_cast<uint8_t>(rule.fromState) == currentState &&
                rule.triggerValue == request) {
                return static_cast<uint8_t>(rule.toState);
            }
        }
    } else {
        for (size_t i = 0; i < config::kInfotainmentTransitionsCount; i++) {
            const auto& rule = config::kInfotainmentTransitions[i];
            if (static_cast<uint8_t>(rule.fromState) == currentState &&
                rule.triggerValue == request) {
                return static_cast<uint8_t>(rule.toState);
            }
        }
    }
    
    std::cerr << "[TransitionTable] No transition found for state=" 
              << static_cast<int>(currentState) 
              << " request=" << request << std::endl;
    
    return currentState; // Stay in current state
}

} // namespace sm
} // namespace ara