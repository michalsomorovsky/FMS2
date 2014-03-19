#include "Transition.h"
#include <iostream>

using namespace std;

Transition::Transition(string transition, int index)
{
    event = transition.substr(0, transition.find_first_of("=",0));
    nextState  = transition.substr(transition.find_first_of("=",0),string::npos);
    indexOfNextState = index;
}

Transition::~Transition()
{
    //dtor
}
