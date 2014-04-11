#include <string>
#include <vector>
#include "State.h"
#ifndef FSM_H
#define FSM_H

using namespace std;

class FSM
{
    public:

        vector <State*> states;
        vector< vector < int > > transitions;
        int** t2;
        FSM();
        virtual ~FSM();
        void addState(string state, string action, string comment);
        void addTransition(string transiton);
        void connectAll();
    protected:
    private:
};

#endif // FSM_H
