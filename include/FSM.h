#include <string>
#include "State.h"
#ifndef FSM_H
#define FSM_H

using namespace std;

class FSM
{
    public:
        State* stavy;
        FSM(int num);
        virtual ~FSM();
        void addState(string state, string action);
    protected:
    private:
};

#endif // FSM_H
