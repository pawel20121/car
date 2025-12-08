#include "ara/sm/state_machine.h"
#include <iostream>

namespace ara {
namespace sm {

// ============================================================================
// Constructor
// ============================================================================

StateMachine::StateMachine(const std::string& name, Category category)
    : name_(name)
    , category_(category)
    , currentState_(State::kInitial)
    , isRunning_(false)
    , isInTransition_(false)
    , errorRecoveryOngoing_(false)
    , impactedByUpdate_(false)
{
    std::cout << "[SM] StateMachine created: " << name_ 
              << " (Category: " << (category_ == Category::kController ? "Controller" : "Agent")
              << ")" << std::endl;
}

// ============================================================================
// Destructor
// ============================================================================

StateMachine::~StateMachine() {
    std::cout << "[SM] StateMachine destroyed: " << name_ << std::endl;
}

// ============================================================================
// Public methods
// ============================================================================

ara::core::Result<void, StateManagementErrc> StateMachine::RequestTransition(
    TransitionRequestType request) 
{
    std::cout << "[SM] RequestTransition called: request=" << request << std::endl;
    
    // [SWS_SM_00649] Block if impacted by update
    if (impactedByUpdate_) {
        std::cout << "[SM] Rejected: Update in progress" << std::endl;
        return ara::core::Result<void, StateManagementErrc>(
            StateManagementErrc::kUpdateInProgress);
    }
    
    // [SWS_SM_00605] Block if recovery ongoing
    if (errorRecoveryOngoing_) {
        std::cout << "[SM] Rejected: Recovery ongoing" << std::endl;
        return ara::core::Result<void, StateManagementErrc>(
            StateManagementErrc::kRecoveryTransitionOngoing);
    }
    
    // [SWS_SM_00604] Check if request is valid
    if (!IsTransitionAllowed(request)) {
        std::cout << "[SM] Rejected: Invalid or not allowed transition" << std::endl;
        return ara::core::Result<void, StateManagementErrc>(
            StateManagementErrc::kTransitionNotAllowed);
    }
    
    // [SWS_SM_00607] Execute transition
    // Simplified: just go to Running state for demo
    State nextState = State::kRunning;
    return TransitionTo(nextState);
}

StateMachineStateNameType StateMachine::GetCurrentState() const {
    // [SWS_SM_00616] Return "inTransition" if in transition
    if (isInTransition_) {
        return kInTransitionStateName;
    }
    
    // [SWS_SM_00617] Return actual state name
    return StateToString(currentState_);
}

StateMachine::State StateMachine::GetCurrentStateEnum() const {
    return currentState_;
}

const std::string& StateMachine::GetName() const {
    return name_;
}

StateMachine::Category StateMachine::GetCategory() const {
    return category_;
}

bool StateMachine::IsInTransition() const {
    return isInTransition_;
}

void StateMachine::HandleErrorNotification(uint32_t executionError) {
    std::cout << "[SM] Error notification received: " << executionError << std::endl;
    
    // [SWS_SM_00664] Ignore if impacted by update
    if (impactedByUpdate_) {
        std::cout << "[SM] Error ignored: Impacted by update" << std::endl;
        return;
    }
    
    // [SWS_SM_00601] Handle error recovery
    errorRecoveryOngoing_ = true;
    std::cout << "[SM] Error recovery started" << std::endl;
    
    // Simplified: transition to Off state
    TransitionTo(State::kOff);
    
    // [SWS_SM_00602] Reset flag after recovery
    errorRecoveryOngoing_ = false;
}

void StateMachine::SetImpactedByUpdate(bool impacted) {
    impactedByUpdate_ = impacted;
    std::cout << "[SM] Impacted by update: " << (impacted ? "YES" : "NO") << std::endl;
}

bool StateMachine::IsImpactedByUpdate() const {
    return impactedByUpdate_;
}

ara::core::Result<void, StateManagementErrc> StateMachine::Start(State targetState) {
    std::cout << "[SM] Start called: target=" << StateToString(targetState) << std::endl;
    
    if (isRunning_) {
        // [SWS_SM_00623] Already running, transition to target state
        return TransitionTo(targetState);
    }
    
    // [SWS_SM_00612] Start and go to target state
    isRunning_ = true;
    return TransitionTo(targetState);
}

ara::core::Result<void, StateManagementErrc> StateMachine::Stop() {
    std::cout << "[SM] Stop called" << std::endl;
    
    if (!isRunning_) {
        // [SWS_SM_00615] Already stopped
        return ara::core::Result<void, StateManagementErrc>();
    }
    
    // [SWS_SM_00651] Transition to Off
    auto result = TransitionTo(State::kOff);
    if (result.HasValue()) {
        isRunning_ = false;
    }
    
    return result;
}

bool StateMachine::IsRunning() const {
    return isRunning_;
}

// ============================================================================
// Private methods
// ============================================================================

ara::core::Result<void, StateManagementErrc> StateMachine::TransitionTo(State newState) {
    std::cout << "[SM] Transitioning: " << StateToString(currentState_) 
              << " -> " << StateToString(newState) << std::endl;
    
    // Set in transition flag
    isInTransition_ = true;
    
    // Execute action list (simplified - just log)
    ExecuteActionList();
    
    // Change state
    currentState_ = newState;
    
    // Clear transition flag
    isInTransition_ = false;
    
    std::cout << "[SM] Transition completed: now in " << StateToString(currentState_) << std::endl;
    
    return ara::core::Result<void, StateManagementErrc>();
}

void StateMachine::ExecuteActionList() {
    // [SWS_SM_00609] Execute actions in order
    std::cout << "[SM] Executing ActionList for state: " 
              << StateToString(currentState_) << std::endl;
    
    // TODO: Real implementation will load actions from config
    // For now, just log
    std::cout << "[SM] ActionList execution completed" << std::endl;
}

std::string StateMachine::StateToString(State state) {
    switch (state) {
        case State::kInitial:        return "Initial";
        case State::kOff:            return "Off";
        case State::kPrepareUpdate:  return "PrepareUpdate";
        case State::kVerifyUpdate:   return "VerifyUpdate";
        case State::kPrepareRollback: return "PrepareRollback";
        case State::kRunning:        return "Running";
        case State::kShutdown:       return "Shutdown";
        case State::kInTransition:   return "inTransition";
        default:                     return "Unknown";
    }
}

bool StateMachine::IsTransitionAllowed(TransitionRequestType request) const {
    // Simplified validation
    // TODO: Real implementation will check TransitionRequestTable
    
    // Allow some basic transitions for demo
    if (currentState_ == State::kInitial && request == 1) {
        return true;  // Initial -> Running
    }
    if (currentState_ == State::kRunning && request == 2) {
        return true;  // Running -> Shutdown
    }
    
    return false;
}

} // namespace sm
} // namespace ara
