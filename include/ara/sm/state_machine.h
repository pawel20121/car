#ifndef ARA_SM_STATE_MACHINE_H
#define ARA_SM_STATE_MACHINE_H

#include <string>
#include <cstdint>
#include "ara/sm/types.h"
#include "ara/core/result.h"

namespace ara {
namespace sm {

/**
 * @brief State Machine implementation for AUTOSAR Adaptive Platform
 * 
 * Represents a state machine that can be either a Controller or an Agent.
 * Controller manages the machine lifecycle and other state machines.
 * Agent manages a subset of Function Groups.
 * 
 * @req [SWS_SM_00600] StateMachineService interface
 * @req [SWS_SM_00607] StateMachine transition execution
 * 
 * Based on AUTOSAR AP R24-11 State Management Specification
 */
class StateMachine {
public:
    /**
     * @brief Enumeration of mandatory state machine states
     * 
     * @req [SWS_SM_CONSTR_00021] Existence of PrepareUpdate state
     * @req [SWS_SM_CONSTR_00022] Existence of VerifyUpdate state
     * @req [SWS_SM_CONSTR_00023] Existence of PrepareRollback state
     * @req [SWS_SM_CONSTR_00024] Existence of Off state (for Agents)
     */
    enum class State : uint8_t {
        kInitial = 0,        ///< Initial state (mandatory)
        kOff = 1,            ///< Off state (mandatory for Agents)
        kPrepareUpdate = 2,  ///< PrepareUpdate state (mandatory)
        kVerifyUpdate = 3,   ///< VerifyUpdate state (mandatory)
        kPrepareRollback = 4,///< PrepareRollback state (mandatory)
        // Project-specific states can be added here
        kRunning = 5,        ///< Example: Running state
        kShutdown = 6,       ///< Example: Shutdown state
        kInTransition = 255  ///< Special value for transition state
    };

    /**
     * @brief State Machine category
     * 
     * @req [SWS_SM_CONSTR_00031] Existence of Controller
     */
    enum class Category : uint8_t {
        kController = 0,  ///< Controller state machine (one per machine)
        kAgent = 1        ///< Agent state machine (zero or more)
    };

    /**
     * @brief Constructs a StateMachine
     * 
     * @param name Short name identifier of the state machine
     * @param category Controller or Agent
     */
    explicit StateMachine(const std::string& name, Category category);

    /**
     * @brief Destructor
     */
    ~StateMachine();

    // Prevent copying (state machines should be unique)
    StateMachine(const StateMachine&) = delete;
    StateMachine& operator=(const StateMachine&) = delete;

    // Allow moving
    StateMachine(StateMachine&&) noexcept = default;
    StateMachine& operator=(StateMachine&&) noexcept = default;

    /**
     * @brief Requests a state transition
     * 
     * @req [SWS_SM_00607] StateMachine transition execution
     * @req [SWS_SM_00603] Not allowed transition rejection
     * @req [SWS_SM_00604] Invalid transition rejection
     * @req [SWS_SM_00605] Recovery ongoing rejection
     * @req [SWS_SM_00649] Update in progress rejection
     * 
     * @param request Transition request value
     * @return Result<void> Success or error code
     * 
     * @error kInvalidValue Request not in TransitionRequestTable
     * @error kTransitionNotAllowed Transition not allowed from current state
     * @error kRecoveryTransitionOngoing Error recovery in progress
     * @error kUpdateInProgress Update session in progress
     */
    ara::core::Result<void> RequestTransition(TransitionRequestType request);

    /**
     * @brief Gets the current state name
     * 
     * @req [SWS_SM_00616] CurrentState during transition returns "inTransition"
     * @req [SWS_SM_00617] CurrentState after transition returns actual state
     * 
     * @return String representation of current state or "inTransition"
     */
    StateMachineStateNameType GetCurrentState() const;

    /**
     * @brief Gets the current state enum value
     * 
     * @return Current State enum value
     */
    State GetCurrentStateEnum() const;

    /**
     * @brief Gets the state machine name
     * 
     * @return State machine identifier
     */
    const std::string& GetName() const;

    /**
     * @brief Gets the state machine category
     * 
     * @return Controller or Agent
     */
    Category GetCategory() const;

    /**
     * @brief Checks if state machine is currently in transition
     * 
     * @return true if transition is in progress
     */
    bool IsInTransition() const;

    /**
     * @brief Handles error notifications from Platform Health Management
     * 
     * @req [SWS_SM_00601] Error notification reaction (not impacted by update)
     * @req [SWS_SM_00664] Error notification reaction (impacted by update)
     * 
     * @param executionError Error code from PHM/EM
     */
    void HandleErrorNotification(uint32_t executionError);

    /**
     * @brief Marks state machine as impacted by update
     * 
     * @req [SWS_SM_00654] StateMachine marked as "ImpactedByUpdate"
     * 
     * @param impacted true to mark as impacted, false to unmark
     */
    void SetImpactedByUpdate(bool impacted);

    /**
     * @brief Checks if state machine is impacted by update
     * 
     * @return true if marked as impacted by update
     */
    bool IsImpactedByUpdate() const;

    /**
     * @brief Starts the state machine
     * 
     * @req [SWS_SM_00612] Start without parameter (goes to Initial)
     * @req [SWS_SM_00622] Start with parameter (goes to specified state)
     * @req [SWS_SM_00648] Controller start
     * 
     * @param targetState Optional target state (nullptr = Initial state)
     * @return Result<void> Success or error
     */
    ara::core::Result<void> Start(State targetState = State::kInitial);

    /**
     * @brief Stops the state machine
     * 
     * @req [SWS_SM_00614] Stop state machine
     * @req [SWS_SM_00651] Transition to Off state on stop
     * 
     * @return Result<void> Success or error
     */
    ara::core::Result<void> Stop();

    /**
     * @brief Checks if state machine is running
     * 
     * @return true if state machine has been started
     */
    bool IsRunning() const;

private:
    /**
     * @brief Executes the action list for the current state
     * 
     * @req [SWS_SM_00609] ActionList processing order
     * @req [SWS_SM_00611] Parallel ActionListItem processing
     */
    void ExecuteActionList();

    /**
     * @brief Transitions to a new state
     * 
     * @param newState Target state
     * @return Result<void> Success or error
     */
    ara::core::Result<void> TransitionTo(State newState);

    /**
     * @brief Converts State enum to string
     * 
     * @param state State enum value
     * @return String representation
     */
    static std::string StateToString(State state);

    /**
     * @brief Validates if transition is allowed
     * 
     * @param request Transition request
     * @return true if transition is valid
     */
    bool IsTransitionAllowed(TransitionRequestType request) const;

    // Member variables
    std::string name_;                    ///< State machine identifier
    Category category_;                   ///< Controller or Agent
    State currentState_;                  ///< Current state
    bool isRunning_;                      ///< Is state machine started?
    bool isInTransition_;                 ///< Is transition in progress?
    bool errorRecoveryOngoing_;           ///< [SWS_SM_00602] Error recovery flag
    bool impactedByUpdate_;               ///< [SWS_SM_00654] Impacted by update flag
};

} // namespace sm
} // namespace ara

#endif // ARA_SM_STATE_MACHINE_H