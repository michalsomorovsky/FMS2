#include <string>

#ifndef TRANSITION_H
#define TRANSITION_H

using namespace std;

class Transition
{
    public:
        Transition(string transition, int index);
        virtual ~Transition();
        string Getevent() { return event; }
        void Setevent(string val) { event = val; }
        string GetnextState() { return nextState; }
        void SetnextState(string val) { nextState = val; }
        int getIndex() { return indexOfNextState; }
    protected:
    private:
        string event;
        string nextState;
        int indexOfNextState;
};

#endif // TRANSITION_H
