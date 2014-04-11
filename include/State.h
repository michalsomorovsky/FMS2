#include <string>
#include "Transition.h"
#include <vector>

#ifndef STATE_H
#define STATE_H

using namespace std;

class State
{
    public:
        State(string name, string action, int index, string comment);
        virtual ~State();
        string Getname() { return name; }
        void Setname(string val) { name = val; }
        string Getaction() { return action; }
        void Setaction(string val) { action = val; }
        bool isEmpty();
        void setTransition(string val);
        Transition* getTransition(int i) { return transition[i]; }
        int getTransitionSize() { return transition.size(); }
        void setIndex(int val) { index = val; }
        int getIndex() { return index; }
        string getComment() {return comment; }
    protected:
    private:
        int index;
        string name;
        string action;
        vector < Transition* > transition;
        string comment;
};

#endif // STATE_H
