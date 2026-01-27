/**
 * @file update_request_service.h
 * @brief UpdateRequest service interface for State Management
 * 
 * This header defines the service interface used by Update and Configuration
 * Management (UCM) to interact with State Management during software updates.
 * 
 * @req [SWS_SM_91017] ServiceInterface UpdateRequest
 * @req [SWS_SM_91016] Port UpdateRequest
 */

#ifndef ARA_SM_UPDATE_REQUEST_SERVICE_H
#define ARA_SM_UPDATE_REQUEST_SERVICE_H

#include "types.h"
#include "result.h"

namespace ara {
namespace sm {

// Forward declaration
class StateMachine;

/**
 * @brief UpdateRequest service interface
 * @req [SWS_SM_91017] ServiceInterface UpdateRequest
 * @req [SWS_SM_91016] Port UpdateRequest
 * 
 * This service is provided by State Management to Update and Configuration
 * Management (UCM) to coordinate software update operations.
 * 
 * The interface supports:
 * - Starting/stopping update sessions
 * - Preparing Function Groups for update
 * - Verifying updated software
 * - Rolling back failed updates
 * - Requesting machine restart during update
 * 
 * Usage flow:
 * 1. UCM calls RequestUpdateSession() to check if update is allowed
 * 2. If allowed, UCM calls PrepareUpdate() with affected Function Groups
 * 3. UCM performs software deployment
 * 4. UCM calls VerifyUpdate() to test new software
 * 5. If verification fails, UCM calls PrepareRollback()
 * 6. If machine restart needed, UCM calls ResetMachine()
 * 7. Finally, UCM calls StopUpdateSession() to end session
 */
class UpdateRequestService {
public:
    /**
     * @brief Constructor
     */
    UpdateRequestService();
    
    /**
     * @brief Destructor
     */
    ~UpdateRequestService();
    
    // Disable copy/move
    UpdateRequestService(const UpdateRequestService&) = delete;
    UpdateRequestService& operator=(const UpdateRequestService&) = delete;
    UpdateRequestService(UpdateRequestService&&) = delete;
    UpdateRequestService& operator=(UpdateRequestService&&) = delete;
    
    // ========================================================================
    // PUBLIC INTERFACE (UCM calls these methods)
    // ========================================================================
    
    /**
     * @brief Request to start update session
     * @req [SWS_SM_91102] RequestUpdateSession method
     * @req [SWS_SM_00203] Start update session
     * @req [SWS_SM_00209] Preventing multiple update sessions
     * @req [SWS_SM_00630] Rejection of update session
     * @req [SWS_SM_00631] Acceptance of update session
     * 
     * @return Result<void> Success if update allowed, error otherwise
     * @retval kNotAllowedMultipleUpdateSessions Another session active
     * @retval kOperationRejected Update not allowed by system state
     */
    ara::core::Result<void, StateManagementErrc> RequestUpdateSession();
    
    /**
     * @brief Prepare Function Groups for update
     * @req [SWS_SM_91103] PrepareUpdate method
     * @req [SWS_SM_00206] prepare update
     * @req [SWS_SM_00633] Transition affected StateMachines to PrepareUpdate
     * @req [SWS_SM_00634] Shutdown affected StateMachines
     * 
     * @param functionGroupList List of FGs to be updated
     * @return Result<void> Success if preparation complete
     * @retval kOperationRejected Not in update session
     * @retval kOperationFailed Preparation failed
     */
    ara::core::Result<void, StateManagementErrc> PrepareUpdate(
        const FunctionGroupListType& functionGroupList);
    
    /**
     * @brief Verify updated software
     * @req [SWS_SM_91104] VerifyUpdate method
     * @req [SWS_SM_00207] prepare verify
     * @req [SWS_SM_00638] Transition to VerifyUpdate state
     * 
     * @param functionGroupList List of FGs to verify
     * @return Result<void> Success if verification complete
     * @retval kOperationRejected Not in update session
     * @retval kOperationFailed Verification failed
     */
    ara::core::Result<void, StateManagementErrc> VerifyUpdate(
        const FunctionGroupListType& functionGroupList);
    
    /**
     * @brief Prepare rollback to previous version
     * @req [SWS_SM_91105] PrepareRollback method
     * @req [SWS_SM_00208] prepare rollback
     * @req [SWS_SM_00642] Transition to PrepareRollback state
     * 
     * @param functionGroupList List of FGs to rollback
     * @return Result<void> Success if rollback preparation complete
     * @retval kOperationRejected Not in update session
     * @retval kOperationFailed Rollback preparation failed
     */
    ara::core::Result<void, StateManagementErrc> PrepareRollback(
        const FunctionGroupListType& functionGroupList);
    
    /**
     * @brief Request machine reset (fire-and-forget)
     * @req [SWS_SM_91100] ResetMachine method
     * @req [SWS_SM_00202] Reset Execution
     * @req [SWS_SM_00658] Transition to Restart state
     * 
     * This is a fire-and-forget method. Result is tracked via
     * ResetMachineNotifier field.
     * 
     * After successful reset, machine will restart and enter
     * ContinueUpdate state.
     */
    void ResetMachine();
    
    /**
     * @brief Stop update session
     * @req [SWS_SM_91101] StopUpdateSession method
     * @req [SWS_SM_00205] Stop update session
     * @req [SWS_SM_00646] Transition to AfterUpdate state
     * 
     * @return Result<void> Success if session stopped
     * @retval kOperationFailed Failed to transition
     */
    ara::core::Result<void, StateManagementErrc> StopUpdateSession();
    
    /**
     * @brief Get ResetMachineNotifier field value
     * @req [SWS_SM_91106] ResetMachineNotifier field
     * @req [SWS_SM_00211] ResetMachine notification
     * @req [SWS_SM_00212] Default value (kIdle)
     * 
     * @return Current status of ResetMachine operation
     */
    UpdateStatusType GetResetMachineNotifier() const;
    
    /**
     * @brief Set ResetMachineNotifier field value
     * 
     * Internal use by State Management.
     * In ara::com, this would be a field with HasSetter=false,
     * only State Management can modify it.
     * 
     * @param status New status value
     */
    void SetResetMachineNotifier(UpdateStatusType status);
    
    // ========================================================================
    // HELPER METHODS (Internal / Testing)
    // ========================================================================
    
    /**
     * @brief Set Controller StateMachine reference
     * 
     * Called by State Management at initialization to provide
     * reference to Controller instance.
     * 
     * @param controller Pointer to Controller StateMachine
     */
    void SetControllerStateMachine(StateMachine* controller);
    
    /**
     * @brief Check if update session is active
     * @return true if session active
     */
    bool IsUpdateSessionActive() const;
    
    /**
     * @brief Set update allowed flag
     * 
     * This would normally be set via UpdateAllowedService by
     * SMControlApplication. Exposed here for testing.
     * 
     * @param allowed Update allowed value
     */
    void SetUpdateAllowed(UpdateAllowedType allowed);
};

} // namespace sm
} // namespace ara

#endif // ARA_SM_UPDATE_REQUEST_SERVICE_H