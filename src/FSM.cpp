#include "FSM.h"
#include "State.h"

FSM::FSM(int num) {
    stavy = new State[num];
}

FSM::~FSM()
{
    //dtor
}

void FSM::addState(string state, string action)
{
    //cout<<text<<endl;
    int i=0;
    while(!stavy[i].isEmpty()) i++;
    stavy[i].Setname(state);
    stavy[i].Setaction(action);
}
