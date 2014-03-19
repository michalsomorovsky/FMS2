#include "State.h"
#include <vector>

using namespace std;

State::State(string name, string action, int index)
{
    this->name = name;
    this->action = action;
    this->index = index;
}

State::~State()
{
    //dtor
}

bool State::isEmpty()
{
    return this->Getname().empty();
}

void State::setTransition(string val, int index)
{
    transition.push_back(new Transition(val, index));
}
