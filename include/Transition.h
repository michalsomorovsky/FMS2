#include <string>

#ifndef TRANSITION_H
#define TRANSITION_H

using namespace std;

class Transition
{
    public:
        Transition();
        virtual ~Transition();
        string Getevent() { return event; }
        void Setevent(string val) { event = val; }
        //State* GetnextState() { return nextState; }
        //void SetnextState(State val) { nextState = val; }
    protected:
    private:
        string event;
        //State* nextState;
};

#endif // TRANSITION_H
