#ifndef ARA_SM_ACTION_EXECUTOR_H
#define ARA_SM_ACTION_EXECUTOR_H

#include <string>
#include <cstdint>
#include "config/static_config.h"

namespace ara {
namespace sm {

/**
 * @brief Executes ActionListItems
 * 
 * @req [SWS_SM_00608] Function Group State action
 * @req [SWS_SM_00610] SYNC action
 * @req [SWS_SM_00612] Start StateMachine action
 * @req [SWS_SM_00614] Stop StateMachine action
 * @req [SWS_SM_00624] Sleep action
 * @req [SWS_SM_00625] SetNetworkHandle FullCom
 * @req [SWS_SM_00626] SetNetworkHandle NoCom
 */
class ActionExecutor {
public:
    ActionExecutor() = default;
    
    /**
     * @brief Execute action list
     * 
     * @param actions Array of actions
     * @param count Number of actions
     */
    void ExecuteActionList(const config::ActionItem* actions, size_t count);
    
    /**
     * @brief Execute single action
     * 
     * @param action Action to execute
     */
    void ExecuteAction(const config::ActionItem& action);
    
private:
    void ExecuteSetFunctionGroupState(const char* fgName, const char* stateName);
    void ExecuteStartStateMachine(const char* smName, const char* initialState);
    void ExecuteStopStateMachine(const char* smName);
    void ExecuteSync();
    void ExecuteSleep(uint32_t milliseconds);
    void ExecuteSetNetworkHandle(const char* handleName, const char* state);
};

} // namespace sm
} // namespace ara

#endif