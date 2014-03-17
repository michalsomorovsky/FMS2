#include "State.h"


State::State()
{

}

State::~State()
{
    //dtor
}

bool State::isEmpty()
{
    return this->Getname().empty();
}
