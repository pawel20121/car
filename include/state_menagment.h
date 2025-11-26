#ifndef STATE_MENAGMENT_H
#define STATE_MENAGMENT_H

#include <iostream>


class StateMachine {
public:
    enum State {
        Initial,      
        Off,          
        PrepareUpdate, 
        VerifyUpdate,  
        PrepareRollback 
    };

   
    StateMachine();    // Konstruktor domyślny
    ~StateMachine();   // Destruktor

    // Funkcje do zarządzania stanami
    void changeState(State newState);  
    State getCurrentState() const;     

private:
    State currentState; 
};


#endif // STATE_MENAGMENT_H
