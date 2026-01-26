#ifndef ARA_SM_ERROR_RECOVERY_H
#define ARA_SM_ERROR_RECOVERY_H

#include <cstdint>
#include <cstddef>
#include "static_config.h"            // from config/ (added to include dirs)
#include "state_machine.h"
#include  "types.h"     // for StateMachine::Category

namespace ara {
namespace sm {

/**
 * @brief Alias for error code type used in recovery tables.
 * In AUTOSAR Adaptive SM, execution errors are integers.
 */
using ExecutionErrorType = uint32_t;

/**
 * @brief Error recovery logic wrapper.
 *
 * This class selects the next recovery state based on:
 *  - current state
 *  - error code
 *  - SM category (Controller / Agent)
 *
 * Definitions of recovery tables live in static_config.h.
 */
class ErrorRecoveryTable
{
public:
    /**
     * @brief Determine recovery state from error code and category.
     *
     * @param currentState Current SM state (uint8)
     * @param errorCode Execution error that occurred
     * @param category StateMachine::Category (Controller/Agent)
     * @return uint8_t recovery state
class ErrorRecoveryTable {
public:
    /**
     * @brief Get recovery state for error
     * 
     * @req [SWS_SM_00601] Error notification reaction
     * @req [SWS_SM_CONSTR_00014] Handling of non-mapped ExecutionError
     * 
     * @param currentState Current StateMachine state
     * @param errorCode Execution error code
     * @param category Controller or Agent
     * @return Recovery state
     */
    static uint8_t GetRecoveryState(
        uint8_t currentState,
        ExecutionErrorType errorCode,
        StateMachine::Category category);
};

class ErrorRecoveryTable {
public:
    static uint8_t GetRecoveryState(...);
    
    // Nowe: obsługa nested recovery
    static bool IsNestedRecovery(uint8_t currentState, 
                                 ExecutionErrorType errorCode,
                                 StateMachine::Category category);
};

} // namespace sm
} // namespace ara

#endif // ARA_SM_ERROR_RECOVERY_H