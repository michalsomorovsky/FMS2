#include <string>
#include "Transition.h"

#ifndef STATE_H
#define STATE_H

using namespace std;

class State
{
    public:
        State();
        virtual ~State();
        string Getname() { return name; }
        void Setname(string val) { name = val; }
        string Getaction() { return action; }
        void Setaction(string val) { action = val; }
        bool isEmpty();
    protected:
    private:
        string name;
        string action;
        Transition* transition;
};

#endif // STATE_H
