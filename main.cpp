#include <iostream>
#include <fstream>
#include "FSM.h"


using namespace std;

int main()
{
    ifstream file ("C:/Users/m.somorovsky/Dropbox/Ams.seq");
    string line;
    FSM fsm;
    //cout<<sizeof(fsm.stavy)/sizeof(*fsm.stavy);
    //fsm.addState("lama");
    if(file.is_open())
    {
        string action;
        while(getline(file, line))
        {
            if(line.find_first_of("`", 0, 1)==0) cout<<"koment: "<<endl;
            else if(line.find_first_of("[", 0,1)==0)
            {
                if(line.find_first_of("=")!=string::npos) action = line.substr(line.find_first_of("="), string::npos);
                else action = string::npos;
                fsm.addState(line.substr(1, line.find("]")-1), action);
                //fsm.addState(line);
                //cout<<"stav: ";
            }
            else if(line.length() > 3)
            {

                fsm.addTransition(line, fsm.states.size()-1);
                //cout<<line.length()<<endl;
                //cout<<line.substr(line.find_first_of("="))<<endl;
            }
            //cout<<line<<"\n";
        }
        file.close();
        //int i=0;
        for(int i=0; i<fsm.states.size(); i++)
        {
            cout<<fsm.states[i]->Getname()<<fsm.states[i]->Getaction()<<endl;
                for(int j=0; j<fsm.states[i]->getTransitionSize(); j++)
                {
                    cout<<fsm.states[i]->getTransition(j)->Getevent()<<endl;
                }
        }
        /*while(!fsm.states[i].isEmpty())
        {

                i++;
        }*/
    }
    else cout<<"CHYBA"<<endl;
    return 0;
}
