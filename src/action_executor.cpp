#include "action_executor.h"
#include "static_config.h"
#include <iostream>
#include <thread>
#include <chrono>

/**
 * @file action_executor.cpp
 * @brief Implementation of ActionExecutor for State Management
 * 
 * Executes action items as defined in ActionLists when StateMachine
 * transitions between states.
 * 
 * @req [SWS_SM_00608-00626]
 */

namespace ara {
namespace sm {

// ============================================================================
// ExecuteActionList - Main entry point
// ============================================================================

/**
 * @brief Execute a list of actions
 * @req [SWS_SM_00609] Actions are processed in order
 * @req [SWS_SM_00611] Actions processed in parallel unless SYNC
 */
void ActionExecutor::ExecuteActionList(
    const config::ActionItem* actions, 
    size_t count)
{
    std::cout << "[ActionExecutor] Executing action list (" 
              << count << " actions)" << std::endl;
    
    for (size_t i = 0; i < count; i++) {
        // Stop at terminator (when target is nullptr)
        // This allows variable-length action lists
        if (actions[i].target == nullptr && 
            actions[i].type != config::ActionType::kSync) {
            std::cout << "[ActionExecutor] Reached end of action list (terminator)" 
                      << std::endl;
            break;
        }
        
        ExecuteAction(actions[i]);
    }
    
    std::cout << "[ActionExecutor] Action list completed" << std::endl;
}

// ============================================================================
// ExecuteAction - Dispatcher
// ============================================================================

/**
 * @brief Execute a single action item
 * @req [SWS_SM_00608-00626] Different action types
 */
void ActionExecutor::ExecuteAction(const config::ActionItem& action)
{
    switch (action.type) {
        case config::ActionType::kSetFunctionGroupState:
            ExecuteSetFunctionGroupState(action.target, action.param);
            break;
            
        case config::ActionType::kStartStateMachine:
            ExecuteStartStateMachine(action.target, action.param);
            break;
            
        case config::ActionType::kStopStateMachine:
            ExecuteStopStateMachine(action.target);
            break;
            
        case config::ActionType::kSync:
            ExecuteSync();
            break;
            
        case config::ActionType::kSleep:
            ExecuteSleep(action.sleepTimeMs);
            break;
            
        case config::ActionType::kSetNetworkHandle:
            ExecuteSetNetworkHandle(action.target, action.param);
            break;
            
        default:
            std::cerr << "[ActionExecutor] ERROR: Unknown action type: " 
                      << static_cast<int>(action.type) << std::endl;
            break;
    }
}

// ============================================================================
// Action Type Implementations
// ============================================================================

/**
 * @brief Set Function Group State
 * @req [SWS_SM_00608]
 * 
 * @param fgName Function Group name (e.g., "MachineFG", "InfotainmentFG")
 * @param stateName Desired state (e.g., "Startup", "Running", "Off")
 */
void ActionExecutor::ExecuteSetFunctionGroupState(
    const char* fgName, 
    const char* stateName)
{
    if (fgName == nullptr || stateName == nullptr) {
        std::cerr << "[ActionExecutor] ERROR: SetFunctionGroupState - null parameter" 
                  << std::endl;
        return;
    }
    
    std::cout << "  [Action] SetFunctionGroupState: " 
              << fgName << " -> " << stateName << std::endl;
    
    // TODO: Call Execution Management API
    // Example:
    // auto fg = ara::exec::FunctionGroup::Create(fgName);
    // if (fg.HasValue()) {
    //     auto state = ara::exec::FunctionGroupState::Create(stateName);
    //     fg.Value().SetState(state);
    // }
}

/**
 * @brief Start a StateMachine (for Controller starting Agents)
 * @req [SWS_SM_00612] Start StateMachine without parameter
 * @req [SWS_SM_00622] Start StateMachine with parameter state
 * 
 * @param smName StateMachine name (e.g., "InfotainmentSM")
 * @param initialState Optional initial state (can be nullptr for default)
 */
void ActionExecutor::ExecuteStartStateMachine(
    const char* smName, 
    const char* initialState)
{
    if (smName == nullptr) {
        std::cerr << "[ActionExecutor] ERROR: StartStateMachine - null name" 
                  << std::endl;
        return;
    }
    
    std::cout << "  [Action] StartStateMachine: " << smName;
    if (initialState != nullptr && initialState[0] != '\0') {
        std::cout << " (initial state: " << initialState << ")";
    } else {
        std::cout << " (default initial state)";
    }
    std::cout << std::endl;
    
    // TODO: Start the referenced StateMachine
    // This would trigger creation of an Agent StateMachine instance
    // Example:
    // auto agentSM = StateMachine::Create(smName, Category::kAgent);
    // if (initialState != nullptr) {
    //     agentSM.Start(ParseState(initialState));
    // } else {
    //     agentSM.Start(); // Uses default initial state
    // }
}

/**
 * @brief Stop a StateMachine
 * @req [SWS_SM_00614] Stop StateMachine
 * @req [SWS_SM_00651] Transition to Off state before stopping
 * 
 * @param smName StateMachine name to stop
 */
void ActionExecutor::ExecuteStopStateMachine(const char* smName)
{
    if (smName == nullptr) {
        std::cerr << "[ActionExecutor] ERROR: StopStateMachine - null name" 
                  << std::endl;
        return;
    }
    
    std::cout << "  [Action] StopStateMachine: " << smName << std::endl;
    
    // TODO: Stop the referenced StateMachine
    // Per [SWS_SM_00651], StateMachine should transition to Off first
    // Example:
    // auto agentSM = StateMachine::Find(smName);
    // if (agentSM.HasValue()) {
    //     agentSM.Value().Stop(); // This transitions to Off then stops
    // }
}

/**
 * @brief Synchronization point - wait for previous actions
 * @req [SWS_SM_00610]
 * 
 * Blocks until all previously issued actions have completed.
 * This is important for ensuring correct ordering when actions
 * have dependencies.
 */
void ActionExecutor::ExecuteSync()
{
    std::cout << "  [Action] SYNC - waiting for previous actions to complete..." 
              << std::endl;
    
    // TODO: Wait for all previous async actions to complete
    // In a real implementation:
    // - Track all pending Future<> objects from async operations
    // - Wait for all of them to complete
    // - Check their results and handle errors
    //
    // For now, all actions are synchronous, so this is a no-op
    // but serves as a placeholder for proper async implementation
    
    std::cout << "  [Action] SYNC - completed" << std::endl;
}

/**
 * @brief Sleep for specified duration
 * @req [SWS_SM_00624]
 * 
 * Used for implementing afterrun scenarios, e.g., keeping
 * network active for a period after shutdown request.
 * 
 * @param milliseconds Duration to sleep in milliseconds
 */
void ActionExecutor::ExecuteSleep(uint32_t milliseconds)
{
    std::cout << "  [Action] Sleep: " << milliseconds << "ms" << std::endl;
    
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    
    std::cout << "  [Action] Sleep completed" << std::endl;
}

/**
 * @brief Set Network Handle state
 * @req [SWS_SM_00625] SetNetworkHandle FullCom
 * @req [SWS_SM_00626] SetNetworkHandle NoCom
 * 
 * Controls network communication state through Network Management.
 * 
 * @param handleName NetworkHandle name (e.g., "VehicleNetwork", "MediaNetwork")
 * @param state Target state: "FullCom" or "NoCom"
 */
void ActionExecutor::ExecuteSetNetworkHandle(
    const char* handleName, 
    const char* state)
{
    if (handleName == nullptr || state == nullptr) {
        std::cerr << "[ActionExecutor] ERROR: SetNetworkHandle - null parameter" 
                  << std::endl;
        return;
    }
    
    std::cout << "  [Action] SetNetworkHandle: " 
              << handleName << " -> " << state << std::endl;
    
    // TODO: Call Network Management API
    // Example:
    // auto nmHandle = ara::nm::NetworkHandle::Find(handleName);
    // if (nmHandle.HasValue()) {
    //     if (std::string(state) == "FullCom") {
    //         nmHandle.Value().SetNetworkRequestedState(
    //             ara::nm::NmStateRequestEnum::kFullCom);
    //     } else if (std::string(state) == "NoCom") {
    //         nmHandle.Value().SetNetworkRequestedState(
    //             ara::nm::NmStateRequestEnum::kNoCom);
    //     }
    // }
}

} // namespace sm
} // namespace ara