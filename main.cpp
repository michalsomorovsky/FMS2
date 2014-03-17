#include <iostream>
#include <fstream>
#include "FSM.h"
using namespace std;

int main()
{
    ifstream file ("C:/Users/m.somorovsky/Dropbox/Ams.seq");
    string line;
    FSM fsm (100);
    //cout<<sizeof(fsm.stavy)/sizeof(*fsm.stavy);
    //fsm.addState("lama");
    if(file.is_open())
    {
        while(getline(file, line))
        {
            //if(line.find_first_of("`", 0, 1)==0) cout<<"koment: ";
            if(line.find_first_of("[", 0,1)==0)
            {
                //fsm.addState(line.substr(1, line.find("]")-1));
                //cout<<"stav: ";
            }
            //cout<<line<<"\n";
        }
        file.close();
        int i=0;
        /*while(!fsm.stavy[i].empty())
        {
                //cout<<fsm.stavy[i]<<endl;
                i++;
        }*/
    }
    else cout<<"CHYBA";
    return 0;
}
