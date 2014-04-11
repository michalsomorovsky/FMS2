#include "State.h"
#include <vector>

using namespace std;

State::State(string name, string action, int index, string comment)
{
    this->name = name;
    this->action = action;
    this->index = index;
    this->comment = comment;
}

State::~State()
{
    //dtor
}

bool State::isEmpty()
{
    return this->Getname().empty();
}

void State::setTransition(string val)
{
    transition.push_back(new Transition(val));
}
