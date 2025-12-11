#ifndef ARA_SM_ERROR_RECOVERY_H
#define ARA_SM_ERROR_RECOVERY_H

#include "ara/sm/types.h"
#include "ara/sm/state_machine.h"

namespace ara {
namespace sm {

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

} // namespace sm
} // namespace ara

#endif