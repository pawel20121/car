#include "ara/sm/error_recovery.h"
#include "static_config.h"
#include <iostream>

namespace ara {
namespace sm {

uint8_t ErrorRecoveryTable::GetRecoveryState(
    uint8_t currentState,
    ExecutionErrorType errorCode,
    StateMachine::Category category)
{
    uint8_t catchAllRecovery = currentState; // Default: stay in current
    
    if (category == StateMachine::Category::kController) {
        // Search for specific error match
        for (size_t i = 0; i < config::kControllerErrorRecoveryCount; i++) {
            const auto& rule = config::kControllerErrorRecovery[i];
            
            if (static_cast<uint8_t>(rule.fromState) == currentState) {
                // Check for exact error match
                if (rule.executionError == errorCode) {
                    std::cout << "[ErrorRecovery] Found exact match: state=" 
                              << static_cast<int>(currentState)
                              << " error=" << errorCode
                              << " -> " << static_cast<int>(rule.recoveryState) 
                              << std::endl;
                    return static_cast<uint8_t>(rule.recoveryState);
                }
                
                // Remember catch-all (ANY) rule
                if (rule.executionError == config::kExecutionErrorAny) {
                    catchAllRecovery = static_cast<uint8_t>(rule.recoveryState);
                }
            }
        }
    } else {
        // Agent error recovery
        for (size_t i = 0; i < config::kInfotainmentErrorRecoveryCount; i++) {
            const auto& rule = config::kInfotainmentErrorRecovery[i];
            
            if (static_cast<uint8_t>(rule.fromState) == currentState) {
                if (rule.executionError == errorCode) {
                    return static_cast<uint8_t>(rule.recoveryState);
                }
                if (rule.executionError == config::kExecutionErrorAny) {
                    catchAllRecovery = static_cast<uint8_t>(rule.recoveryState);
                }
            }
        }
    }
    
    // Use catch-all if no specific match found
    std::cout << "[ErrorRecovery] Using catch-all recovery: -> " 
              << static_cast<int>(catchAllRecovery) << std::endl;
    
    return catchAllRecovery;
}

} // namespace sm
} // namespace ara