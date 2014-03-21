#include <iostream>
#include "FSM.h"
#include "State.h"

using namespace std;

FSM::FSM() {
    //states = new State[num];
}

FSM::~FSM()
{
    //dtor
}

void FSM::addTransition(string transition)
{
    /*int i=0;
    while(!states[i].isEmpty()) i++;
    states[i].setTransition(transition);*/
    states.back()->setTransition(transition);
    //cout<<transition<<endl;
}

void FSM::addState(string state, string action)
{
    //cout<<text<<endl;
    /*int i=0;
    while(!states[i].isEmpty()) i++;
    states[i].Setname(state);
    states[i].Setaction(action);*/
    states.push_back(new State(state, action, states.size()));
}

void FSM::connectAll()
{
    //t2 =
    for(int i=0; i<states.size(); i++)
    {
        int size = states.size();
        vector <int> connections;
        for(int j=0; j<states[i]->getTransitionSize(); j++)
        {
            for(int k=0; k<states.size(); k++)
            {
                if(states[k]->Getname().compare(states[i]->getTransition(j)->GetnextState()) == 0 )
                {
                    connections.push_back(states[k]->getIndex());
                    states[i]->getTransition(j)->setIndex(states[k]->getIndex());
                    break;
                }
            }
           // connections.push_back(states[i]->getTransition(j)->getIndex());
            //cout<<states[i]->getTransition(j)->Getevent()<<endl;
        }
        transitions.push_back(connections);
    }

}
