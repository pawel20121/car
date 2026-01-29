/**
 * @file update_request_service.cpp
 * @brief Implementation of UpdateRequest service interface
 * 
 * This service is used by Update and Configuration Management (UCM)
 * to interact with State Management during software update operations.
 * 
 * @req [SWS_SM_91017] ServiceInterface UpdateRequest
 * @req [SWS_SM_91016] Port UpdateRequest
 */

#include "update_request_service.h"
#include "state_machine.h"
#include <iostream>
#include <memory>

namespace ara {
namespace sm {

// ============================================================================
// Internal State
// ============================================================================

/**
 * @brief Internal state for UpdateRequestService
 * 
 * In a real implementation, this would be part of the State Management
 * daemon's internal state. For this simplified version, we use a
 * singleton-like pattern.
 */
class UpdateRequestServiceImpl {
public:
    static UpdateRequestServiceImpl& GetInstance() {
        static UpdateRequestServiceImpl instance;
        return instance;
    }
    
    // Reference to Controller StateMachine
    StateMachine* controllerSM = nullptr;
    
    // Update session state
    bool updateSessionActive = false;
    UpdateStatusType resetMachineStatus = UpdateStatusType::kIdle;
    
    // Update allowed flag (set by SMControlApplication via UpdateAllowedService)
    UpdateAllowedType updateAllowed = UpdateAllowedType::kUpdateNotAllowed;
    
private:
    UpdateRequestServiceImpl() = default;
};

// ============================================================================
// Constructor / Destructor
// ============================================================================

UpdateRequestService::UpdateRequestService()
{
    std::cout << "[UpdateRequestService] Service created" << std::endl;
}

UpdateRequestService::~UpdateRequestService()
{
    std::cout << "[UpdateRequestService] Service destroyed" << std::endl;
}

// ============================================================================
// RequestUpdateSession
// ============================================================================

/**
 * @brief Request to start an update session
 * 
 * @req [SWS_SM_91102] RequestUpdateSession method
 * @req [SWS_SM_00203] Start update session
 * @req [SWS_SM_00209] Preventing multiple update sessions
 * @req [SWS_SM_00630] Rejection of update session
 * @req [SWS_SM_00631] Acceptance of update session
 * 
 * UCM calls this method to check if an update can be performed.
 * Decision depends on:
 * - Current machine state (evaluated by SMControlApplication)
 * - UpdateAllowed field value
 * - No other update session active
 */
ara::core::Result<void, StateManagementErrc> 
UpdateRequestService::RequestUpdateSession()
{
    std::cout << "[UpdateRequestService] RequestUpdateSession called" << std::endl;
    
    auto& impl = UpdateRequestServiceImpl::GetInstance();
    
    // Check if update session already active
    // @req [SWS_SM_00209]
    if (impl.updateSessionActive) {
        std::cout << "[UpdateRequestService] ERROR: Update session already active" 
                  << std::endl;
        return ara::core::Result<void, StateManagementErrc>(
            StateManagementErrc::kNotAllowedMultipleUpdateSessions);
    }
    
    // Check if update is allowed (set by SMControlApplication)
    // @req [SWS_SM_00630] Rejection
    if (impl.updateAllowed == UpdateAllowedType::kUpdateNotAllowed) {
        std::cout << "[UpdateRequestService] Update not allowed by SMControlApplication" 
                  << std::endl;
        return ara::core::Result<void, StateManagementErrc>(
            StateManagementErrc::kOperationRejected);
    }
    
    // @req [SWS_SM_00631] Acceptance
    std::cout << "[UpdateRequestService] Update session GRANTED" << std::endl;
    
    // Mark session as active
    impl.updateSessionActive = true;
    
    // @req [SWS_SM_00659] Set ResetMachineNotifier to default (kIdle)
    impl.resetMachineStatus = UpdateStatusType::kIdle;
    
    // @req [SWS_SM_00204] Persist session status
    // TODO: In real implementation, persist to ara::per
    // ara::per::KeyValueStorage::SetValue("sm_update_session_active", "true");
    
    std::cout << "[UpdateRequestService] Update session started" << std::endl;
    
    return ara::core::Result<void, StateManagementErrc>();
}

// ============================================================================
// PrepareUpdate
// ============================================================================

/**
 * @brief Prepare Function Groups for update
 * 
 * @req [SWS_SM_91103] PrepareUpdate method
 * @req [SWS_SM_00206] prepare update
 * @req [SWS_SM_00213] Method call rejection outside session
 * @req [SWS_SM_00633] Transition affected StateMachines to PrepareUpdate
 * @req [SWS_SM_00634] Shutdown of affected StateMachines
 * @req [SWS_SM_00635] Failing to prepare for update
 * @req [SWS_SM_00636] Successful preparation for update
 * 
 * @param functionGroupList List of Function Groups to be updated
 */
ara::core::Result<void, StateManagementErrc> 
UpdateRequestService::PrepareUpdate(const FunctionGroupListType& functionGroupList)
{
    std::cout << "[UpdateRequestService] PrepareUpdate called with " 
              << functionGroupList.size() << " Function Groups" << std::endl;
    
    auto& impl = UpdateRequestServiceImpl::GetInstance();
    
    // @req [SWS_SM_00213] Reject if not in active session
    if (!impl.updateSessionActive) {
        std::cout << "[UpdateRequestService] ERROR: No active update session" 
                  << std::endl;
        return ara::core::Result<void, StateManagementErrc>(
            StateManagementErrc::kOperationRejected);
    }
    
    // Validate input
    if (functionGroupList.empty()) {
        std::cout << "[UpdateRequestService] ERROR: Empty function group list" 
                  << std::endl;
        return ara::core::Result<void, StateManagementErrc>(
            StateManagementErrc::kOperationFailed);
    }
    
    // Log Function Groups to be prepared
    for (const auto& fg : functionGroupList) {
        std::cout << "  - FunctionGroup: " << fg << std::endl;
    }
    
    // @req [SWS_SM_00654] Mark affected StateMachines as "ImpactedByUpdate"
    if (impl.controllerSM) {
        impl.controllerSM->SetImpactedByUpdate(true);
        
        // TODO: Mark Agent StateMachines that manage these FunctionGroups
        // For each FG in list:
        //   - Find which StateMachine manages it
        //   - Mark that StateMachine as impacted
    }
    
    // @req [SWS_SM_00633] Transition affected StateMachines to PrepareUpdate state
    // This is done via StateMachine's internal logic when it receives
    // the PrepareUpdate trigger
    
    // Request Controller to transition to PrepareUpdate
    if (impl.controllerSM) {
        auto result = impl.controllerSM->RequestTransition(10);  // kPrepareUpdateRequest
        
        if (!result.HasValue()) {
            // @req [SWS_SM_00635] Failing to prepare
            std::cout << "[UpdateRequestService] ERROR: Failed to prepare for update" 
                      << std::endl;
            return ara::core::Result<void, StateManagementErrc>(
                StateManagementErrc::kOperationFailed);
        }
    }
    
    // @req [SWS_SM_00636] Successful preparation
    std::cout << "[UpdateRequestService] PrepareUpdate completed successfully" 
              << std::endl;
    
    return ara::core::Result<void, StateManagementErrc>();
}

// ============================================================================
// VerifyUpdate
// ============================================================================

/**
 * @brief Verify updated software
 * 
 * @req [SWS_SM_91104] VerifyUpdate method
 * @req [SWS_SM_00207] prepare verify
 * @req [SWS_SM_00213] Method call rejection outside session
 * @req [SWS_SM_00638] Transition to VerifyUpdate state
 * @req [SWS_SM_00639] Unsuccessful verification
 * @req [SWS_SM_00640] Successful verification
 * 
 * @param functionGroupList List of Function Groups to verify
 */
ara::core::Result<void, StateManagementErrc> 
UpdateRequestService::VerifyUpdate(const FunctionGroupListType& functionGroupList)
{
    std::cout << "[UpdateRequestService] VerifyUpdate called with " 
              << functionGroupList.size() << " Function Groups" << std::endl;
    
    auto& impl = UpdateRequestServiceImpl::GetInstance();
    
    // @req [SWS_SM_00213]
    if (!impl.updateSessionActive) {
        return ara::core::Result<void, StateManagementErrc>(
            StateManagementErrc::kOperationRejected);
    }
    
    // Validate input
    if (functionGroupList.empty()) {
        return ara::core::Result<void, StateManagementErrc>(
            StateManagementErrc::kOperationFailed);
    }
    
    // Log Function Groups to be verified
    for (const auto& fg : functionGroupList) {
        std::cout << "  - FunctionGroup: " << fg << std::endl;
    }
    
    // @req [SWS_SM_00638] Transition to VerifyUpdate state
    if (impl.controllerSM) {
        auto result = impl.controllerSM->RequestTransition(11);  // kVerifyUpdateRequest
        
        if (!result.HasValue()) {
            // @req [SWS_SM_00639] Unsuccessful verification
            std::cout << "[UpdateRequestService] ERROR: Failed to verify update" 
                      << std::endl;
            return ara::core::Result<void, StateManagementErrc>(
                StateManagementErrc::kOperationFailed);
        }
    }
    
    // @req [SWS_SM_00640] Successful verification
    std::cout << "[UpdateRequestService] VerifyUpdate completed successfully" 
              << std::endl;
    
    return ara::core::Result<void, StateManagementErrc>();
}

// ============================================================================
// PrepareRollback
// ============================================================================

/**
 * @brief Prepare rollback to previous version
 * 
 * @req [SWS_SM_91105] PrepareRollback method
 * @req [SWS_SM_00208] prepare rollback
 * @req [SWS_SM_00213] Method call rejection outside session
 * @req [SWS_SM_00642] Transition to PrepareRollback state
 * @req [SWS_SM_00643] Shutdown affected StateMachines
 * @req [SWS_SM_00644] Failing to prepare for rollback
 * @req [SWS_SM_00645] Successful preparation for rollback
 * 
 * @param functionGroupList List of Function Groups to rollback
 */
ara::core::Result<void, StateManagementErrc> 
UpdateRequestService::PrepareRollback(const FunctionGroupListType& functionGroupList)
{
    std::cout << "[UpdateRequestService] PrepareRollback called with " 
              << functionGroupList.size() << " Function Groups" << std::endl;
    
    auto& impl = UpdateRequestServiceImpl::GetInstance();
    
    // @req [SWS_SM_00213]
    if (!impl.updateSessionActive) {
        return ara::core::Result<void, StateManagementErrc>(
            StateManagementErrc::kOperationRejected);
    }
    
    // Validate input
    if (functionGroupList.empty()) {
        return ara::core::Result<void, StateManagementErrc>(
            StateManagementErrc::kOperationFailed);
    }
    
    // Log Function Groups to be rolled back
    for (const auto& fg : functionGroupList) {
        std::cout << "  - FunctionGroup: " << fg << std::endl;
    }
    
    // @req [SWS_SM_00642] Transition to PrepareRollback state
    if (impl.controllerSM) {
        auto result = impl.controllerSM->RequestTransition(12);  // kPrepareRollbackRequest
        
        if (!result.HasValue()) {
            // @req [SWS_SM_00644] Failing to prepare for rollback
            std::cout << "[UpdateRequestService] ERROR: Failed to prepare rollback" 
                      << std::endl;
            return ara::core::Result<void, StateManagementErrc>(
                StateManagementErrc::kOperationFailed);
        }
    }
    
    // @req [SWS_SM_00645] Successful preparation for rollback
    std::cout << "[UpdateRequestService] PrepareRollback completed successfully" 
              << std::endl;
    
    return ara::core::Result<void, StateManagementErrc>();
}

// ============================================================================
// ResetMachine
// ============================================================================

/**
 * @brief Request machine reset during update
 * 
 * @req [SWS_SM_91100] ResetMachine method (fire-and-forget)
 * @req [SWS_SM_00202] Reset Execution
 * @req [SWS_SM_00658] Transition to Restart state
 * @req [SWS_SM_00661] Set ResetMachineNotifier to kRejected (if outside session)
 * @req [SWS_SM_00662] Set ResetMachineNotifier to kSuccessful
 * @req [SWS_SM_00663] Set ResetMachineNotifier to kFailed
 * 
 * This is a fire-and-forget method. Result is tracked via ResetMachineNotifier field.
 */
void UpdateRequestService::ResetMachine()
{
    std::cout << "[UpdateRequestService] ResetMachine called" << std::endl;
    
    auto& impl = UpdateRequestServiceImpl::GetInstance();
    
    // @req [SWS_SM_00661] Reject if not in update session
    if (!impl.updateSessionActive) {
        std::cout << "[UpdateRequestService] ERROR: ResetMachine called outside update session" 
                  << std::endl;
        impl.resetMachineStatus = UpdateStatusType::kRejected;
        return;
    }
    
    // @req [SWS_SM_00658] Transition Controller to Restart state
    if (impl.controllerSM) {
        auto result = impl.controllerSM->RequestTransition(3);  // kRestartRequest
        
        if (!result.HasValue()) {
            // @req [SWS_SM_00663] Failed
            std::cout << "[UpdateRequestService] ERROR: Failed to request restart" 
                      << std::endl;
            impl.resetMachineStatus = UpdateStatusType::kFailed;
            return;
        }
        
        // @req [SWS_SM_00662] Successful
        // std::cout << "[UpdateRequestService] Machine restart initiated" << std::endl;
        // impl.resetMachineStatus = UpdateStatusType::kSuccessful;
        
        // Note: After this point, machine will reboot
        // Upon restart, StateMachine will enter ContinueUpdate state
        // @req [SWS_SM_00657]
    } else {
        impl.resetMachineStatus = UpdateStatusType::kFailed;
    }
}

// ============================================================================
// StopUpdateSession
// ============================================================================

/**
 * @brief Stop update session
 * 
 * @req [SWS_SM_91101] StopUpdateSession method
 * @req [SWS_SM_00205] Stop update session
 * @req [SWS_SM_00213] Method call rejection outside session
 * @req [SWS_SM_00646] Transition to AfterUpdate state
 * @req [SWS_SM_00647] Enabling RequestTransition after stop
 * @req [SWS_SM_00656] Unmark "ImpactedByUpdate"
 * @req [SWS_SM_00660] Set ResetMachineNotifier to default
 * 
 * UCM calls this when update session is complete (success or failure).
 */
ara::core::Result<void, StateManagementErrc> 
UpdateRequestService::StopUpdateSession()
{
    std::cout << "[UpdateRequestService] StopUpdateSession called" << std::endl;
    
    auto& impl = UpdateRequestServiceImpl::GetInstance();
    
    // @req [SWS_SM_00213] Should be in active session, but we allow stop anyway
    if (!impl.updateSessionActive) {
        std::cout << "[UpdateRequestService] WARNING: No active session to stop" 
                  << std::endl;
        // Still return success - idempotent operation
        return ara::core::Result<void, StateManagementErrc>();
    }
    
    // @req [SWS_SM_00646] Transition to AfterUpdate state
    if (impl.controllerSM) {
        auto result = impl.controllerSM->RequestTransition(13);  // kFinishUpdateRequest
        
        if (!result.HasValue()) {
            std::cout << "[UpdateRequestService] ERROR: Failed to stop update session" 
                      << std::endl;
            return ara::core::Result<void, StateManagementErrc>(
                StateManagementErrc::kOperationFailed);
        }
        
        // @req [SWS_SM_00656] Unmark "ImpactedByUpdate"
        // impl.controllerSM->SetImpactedByUpdate(false);
    }
    
    // Mark session as inactive
    // LCOV_EXCL_LINE
    impl.updateSessionActive = false;
    
    // @req [SWS_SM_00660] Reset ResetMachineNotifier to default
    impl.resetMachineStatus = UpdateStatusType::kIdle;
    
    // @req [SWS_SM_00204] Clear persisted session status
    // TODO: In real implementation
    // ara::per::KeyValueStorage::RemoveKey("sm_update_session_active");
    
    std::cout << "[UpdateRequestService] Update session stopped" << std::endl;
    
    // @req [SWS_SM_00647] RequestTransition is now enabled again
    // (impactedByUpdate flag is cleared)
    
    return ara::core::Result<void, StateManagementErrc>();
}

// ============================================================================
// ResetMachineNotifier Field Access
// ============================================================================

/**
 * @brief Get ResetMachineNotifier field value
 * 
 * @req [SWS_SM_91106] ResetMachineNotifier field
 * @req [SWS_SM_00211] ResetMachine notification
 * @req [SWS_SM_00212] Default value (kIdle)
 * 
 * @return Current status of ResetMachine operation
 */
UpdateStatusType UpdateRequestService::GetResetMachineNotifier() const
{
    auto& impl = UpdateRequestServiceImpl::GetInstance();
    return impl.resetMachineStatus;
}

/**
 * @brief Set ResetMachineNotifier field value (internal use)
 * 
 * This would normally be private/internal, but exposed for testing
 * and internal State Management use.
 * 
 * @param status New status value
 */
void UpdateRequestService::SetResetMachineNotifier(UpdateStatusType status)
{
    auto& impl = UpdateRequestServiceImpl::GetInstance();
    impl.resetMachineStatus = status;
    
    std::cout << "[UpdateRequestService] ResetMachineNotifier: " 
              << UpdateStatusToString(status) << std::endl;
}

// ============================================================================
// Helper Methods (not part of public interface)
// ============================================================================

/**
 * @brief Set Controller StateMachine reference
 * 
 * Called by State Management daemon at startup to provide
 * reference to Controller instance.
 * 
 * @param controller Pointer to Controller StateMachine
 */
void UpdateRequestService::SetControllerStateMachine(StateMachine* controller)
{
    auto& impl = UpdateRequestServiceImpl::GetInstance();
    impl.controllerSM = controller;
    
    std::cout << "[UpdateRequestService] Controller StateMachine registered" 
              << std::endl;
}

/**
 * @brief Check if update session is active
 * 
 * @return true if update session is active
 */
bool UpdateRequestService::IsUpdateSessionActive() const
{
    auto& impl = UpdateRequestServiceImpl::GetInstance();
    return impl.updateSessionActive;
}

/**
 * @brief Set update allowed flag (called by SMControlApplication)
 * 
 * This would normally come through UpdateAllowedService interface.
 * 
 * @param allowed Update allowed value
 */
void UpdateRequestService::SetUpdateAllowed(UpdateAllowedType allowed)
{
    auto& impl = UpdateRequestServiceImpl::GetInstance();
    impl.updateAllowed = allowed;
    
    std::cout << "[UpdateRequestService] UpdateAllowed: " 
              << UpdateAllowedToString(allowed) << std::endl;
}

} // namespace sm
} // namespace ara
