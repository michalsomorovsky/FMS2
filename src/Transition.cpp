#include "Transition.h"
#include <iostream>

using namespace std;

Transition::Transition(string transition)
{
    event = transition.substr(0, transition.find_first_of("=",0));
    event.erase(event.find_last_not_of(" \n\r\t")+1);
    event.erase(0,event.find_first_not_of(" \n\r\t"));
    nextState  = transition.substr(transition.find_first_of("=",0)+1,string::npos);
    nextState.erase(nextState.find_last_not_of(" \n\r\t")+1);
    nextState.erase(0,event.find_first_not_of(" \n\r\t")+1);
}

Transition::~Transition()
{
    //dtor
}
