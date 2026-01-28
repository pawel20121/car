#include <iostream>
#include "state_machine.h"
#include "transition_table.h"
#include "static_config.h"

namespace ara {
namespace sm {

// ============================================================================
// Constructor
// ============================================================================

StateMachine::StateMachine(const std::string& name,
                           Category category,
                           IActionExecutor* executor)
    : name_(name)
    , category_(category)
    , currentState_(State::kInitial)
    , isRunning_(false)
    , isInTransition_(false)
    , errorRecoveryOngoing_(false)
    , impactedByUpdate_(false)
    , actionExecutor_(executor)
{
    std::cout << "[SM] StateMachine created: " << name_
              << " (Category: " 
              << (category == Category::kController ? "Controller" : "Agent")
              << ")" << std::endl;
}

// ============================================================================
// Destructor
// ============================================================================

StateMachine::~StateMachine()
{
    std::cout << "[SM] StateMachine destroyed: " << name_ << std::endl;
}

// ============================================================================
// Start
// ============================================================================

ara::core::Result<void, StateManagementErrc>
StateMachine::Start(State targetState)
{
    std::cout << "[SM] Start called: target=" << StateToString(targetState) << std::endl;

    isRunning_ = true;
    return TransitionTo(targetState);
}

// ============================================================================
// Stop
// ============================================================================

ara::core::Result<void, StateManagementErrc>
StateMachine::Stop()
{
    if (!isRunning_)
        return ara::core::Result<void, StateManagementErrc>();

    auto r = TransitionTo(State::kOff);
    if (r.HasValue())
        isRunning_ = false;

    return r;
}

// ============================================================================
// RequestTransition
// ============================================================================

ara::core::Result<void, StateManagementErrc>
StateMachine::RequestTransition(TransitionRequestType request)
{
    std::cout << "[SM] RequestTransition: " << request << std::endl;

    if (impactedByUpdate_)
        return ara::core::Result<void, StateManagementErrc>(
            StateManagementErrc::kUpdateInProgress);

    if (errorRecoveryOngoing_)
        return ara::core::Result<void, StateManagementErrc>(
            StateManagementErrc::kRecoveryTransitionOngoing);

    if (!IsTransitionAllowed(request))
        return ara::core::Result<void, StateManagementErrc>(
            StateManagementErrc::kTransitionNotAllowed);

    uint8_t next =
        TransitionTable::GetNextState(
            static_cast<uint8_t>(currentState_),
            request,
            category_);

    return TransitionTo(static_cast<State>(next));
}

// ============================================================================
// Error handling
// ============================================================================

void StateMachine::HandleErrorNotification(uint32_t executionError)
{
    std::cout << "[SM] Error notification: " << executionError << std::endl;

    if (impactedByUpdate_)
    {
        std::cout << "[SM] Error ignored due to update" << std::endl;
        return;
    }

    errorRecoveryOngoing_ = true;

    // uproszczony mechanizm — docelowo: z config::kErrorRecoveryTable
    TransitionTo(State::kOff);

    errorRecoveryOngoing_ = false;
}

// ============================================================================
// Update flag handling
// ============================================================================

void StateMachine::SetImpactedByUpdate(bool impacted)
{
    impactedByUpdate_ = impacted;
    std::cout << "[SM] ImpactedByUpdate=" << (impacted ? "YES" : "NO") << std::endl;
}

bool StateMachine::IsImpactedByUpdate() const
{
    return impactedByUpdate_;
}

// ============================================================================
// Basic getters
// ============================================================================

StateMachine::State StateMachine::GetCurrentStateEnum() const
{
    return currentState_;
}

StateMachineStateNameType StateMachine::GetCurrentState() const
{
    if (isInTransition_)
        return kInTransitionStateName;

    return StateToString(currentState_);
}

const std::string& StateMachine::GetName() const
{
    return name_;
}

StateMachine::Category StateMachine::GetCategory() const
{
    return category_;
}

bool StateMachine::IsInTransition() const
{
    return isInTransition_;
}

bool StateMachine::IsRunning() const
{
    return isRunning_;
}

// ============================================================================
// ExecuteActionList
// ============================================================================

void StateMachine::ExecuteActionList()
{
    using namespace ara::sm::config;

    for (size_t i = 0; i < kActionTableCount; ++i)
    {
        const auto& e = kActionTable[i];
        if (e.state == static_cast<uint8_t>(currentState_))
        {
            if (actionExecutor_)
            {
                actionExecutor_->ExecuteActionList(e.actions, e.actionCount);
            }
            return;
        }
    }

    std::cout << "[SM] No action list for state="
              << StateToString(currentState_) << std::endl;
}

// ============================================================================
// TransitionTo
// ============================================================================

ara::core::Result<void, StateManagementErrc>
StateMachine::TransitionTo(State newState)
{
    std::cout << "[SM] Transition: "
              << StateToString(currentState_)
              << " -> " << StateToString(newState) << std::endl;

    isInTransition_ = true;

    ExecuteActionList();

    currentState_ = newState;
    isInTransition_ = false;

    return ara::core::Result<void, StateManagementErrc>();
}

// ============================================================================
// Helpers
// ============================================================================

std::string StateMachine::StateToString(State state)
{
    switch (state) {
        case State::kInitial: return "Initial";
        case State::kOff: return "Off";
        case State::kPrepareUpdate: return "PrepareUpdate";
        case State::kVerifyUpdate: return "VerifyUpdate";
        case State::kPrepareRollback: return "PrepareRollback";
        case State::kRunning: return "Running";
        case State::kShutdown: return "Shutdown";
        case State::kInTransition: return "inTransition";
        default: return "Unknown";
    }
}

bool StateMachine::IsTransitionAllowed(TransitionRequestType request) const
{
    return TransitionTable::IsTransitionAllowed(
        static_cast<uint8_t>(currentState_),
        request,
        category_);
}

} // namespace sm
} // namespace ara
