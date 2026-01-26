#ifndef ARA_SM_UPDATE_REQUEST_SERVICE_H
#define ARA_SM_UPDATE_REQUEST_SERVICE_H

#include "ara/com/service.h"
#include "ara/sm/types.h"
#include "ara/core/result.h"

namespace ara {
namespace sm {

/**
 * @brief UpdateRequest service interface
 * @req [SWS_SM_91017]
 */
class UpdateRequestService {
public:
    /**
     * @req [SWS_SM_91102] RequestUpdateSession
     */
    ara::core::Result<void, StateManagementErrc> RequestUpdateSession();
    
    /**
     * @req [SWS_SM_91103] PrepareUpdate
     */
    ara::core::Result<void, StateManagementErrc> PrepareUpdate(
        const FunctionGroupListType& functionGroupList);
    
    /**
     * @req [SWS_SM_91104] VerifyUpdate
     */
    ara::core::Result<void, StateManagementErrc> VerifyUpdate(
        const FunctionGroupListType& functionGroupList);
    
    /**
     * @req [SWS_SM_91105] PrepareRollback
     */
    ara::core::Result<void, StateManagementErrc> PrepareRollback(
        const FunctionGroupListType& functionGroupList);
    
    /**
     * @req [SWS_SM_91100] ResetMachine
     */
    void ResetMachine();  // Fire-and-forget
    
    /**
     * @req [SWS_SM_91101] StopUpdateSession
     */
    ara::core::Result<void, StateManagementErrc> StopUpdateSession();
    
    /**
     * @req [SWS_SM_91106] ResetMachineNotifier field
     */
    UpdateStatusType GetResetMachineNotifier() const;
    void SetResetMachineNotifier(UpdateStatusType status);
};

} // namespace sm
} // namespace ara

#endif