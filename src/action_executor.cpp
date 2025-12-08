#include "action_executor.h"
#include <iostream>
#include <thread>
#include <chrono>

namespace ara {
namespace sm {

void ActionExecutor::ExecuteActionList(
    const config::ActionItem* actions, 
    size_t count)
{
    std::cout << "[ActionExecutor] Executing action list (" 
              << count << " actions)" << std::endl;
    
    for (size_t i = 0; i < count; i++) {
        // Stop at first null action (end of list)
        if (actions[i].type == config::ActionType::kSetFunctionGroupState &&
            actions[i].targetName == nullptr) {
            break;
        }
        
        ExecuteAction(actions[i]);
    }
    
    std::cout << "[ActionExecutor] Action list completed" << std::endl;
}

void ActionExecutor::ExecuteAction(const config::ActionItem& action)
{
    switch (action.type) {
        case config::ActionType::kSetFunctionGroupState:
            ExecuteSetFunctionGroupState(action.targetName, action.targetState);
            break;
            
        case config::ActionType::kStartStateMachine:
            ExecuteStartStateMachine(action.targetName, action.targetState);
            break;
            
        case config::ActionType::kStopStateMachine:
            ExecuteStopStateMachine(action.targetName);
            break;
            
        case config::ActionType::kSync:
            ExecuteSync();
            break;
            
        case config::ActionType::kSleep:
            ExecuteSleep(action.delayMs);
            break;
            
        case config::ActionType::kSetNetworkHandle:
            ExecuteSetNetworkHandle(action.targetName, action.targetState);
            break;
            
        default:
            std::cerr << "[ActionExecutor] Unknown action type!" << std::endl;
            break;
    }
}

void ActionExecutor::ExecuteSetFunctionGroupState(
    const char* fgName, 
    const char* stateName)
{
    std::cout << "  [Action] SetFunctionGroupState: " 
              << fgName << " -> " << stateName << std::endl;
    
    // TODO: Call Execution Management API
    // execMgmt_->SetState(fgName, stateName);
}

void ActionExecutor::ExecuteStartStateMachine(
    const char* smName, 
    const char* initialState)
{
    std::cout << "  [Action] StartStateMachine: " << smName;
    if (initialState != nullptr && initialState[0] != '\0') {
        std::cout << " (initial: " << initialState << ")";
    }
    std::cout << std::endl;
    
    // TODO: Start the referenced StateMachine
}

void ActionExecutor::ExecuteStopStateMachine(const char* smName)
{
    std::cout << "  [Action] StopStateMachine: " << smName << std::endl;
    
    // TODO: Stop the referenced StateMachine
}

void ActionExecutor::ExecuteSync()
{
    std::cout << "  [Action] SYNC - waiting for previous actions..." << std::endl;
    
    // TODO: Wait for all previous async actions to complete
    // For now, no-op since all actions are synchronous
}

void ActionExecutor::ExecuteSleep(uint32_t milliseconds)
{
    std::cout << "  [Action] Sleep: " << milliseconds << "ms" << std::endl;
    
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

void ActionExecutor::ExecuteSetNetworkHandle(
    const char* handleName, 
    const char* state)
{
    std::cout << "  [Action] SetNetworkHandle: " 
              << handleName << " -> " << state << std::endl;
    
    // TODO: Call Network Management API
    // netMgmt_->SetNetworkRequestedState(handleName, state);
}

} // namespace sm
} // namespace ara

