#include <iostream>
#include <fstream>
#include "FSM.h"
#include <gtk/gtk.h>
#include <cairo.h>
#include <math.h>

GdkPixbuf *create_pixbuf(const gchar * filename)
{
   GdkPixbuf *pixbuf;
   GError *error = NULL;
   pixbuf = gdk_pixbuf_new_from_file(filename, &error);
   if(!pixbuf) {
      fprintf(stderr, "%s\n", error->message);
      g_error_free(error);
   }

   return pixbuf;
}

void on_window1_destroy (GtkWindow *object, gpointer user_data)
{
    cout<<"lama";
    gtk_main_quit ();
}

using namespace std;

int main(int argc, char *argv[])
{
    /*GtkWidget *window;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "icon");
  gtk_window_set_default_size(GTK_WINDOW(window), 500, 500);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_icon(GTK_WINDOW(window), create_pixbuf("web.png"));
  gtk_widget_show(window);

  g_signal_connect_swapped(G_OBJECT(window), "destroy",
      G_CALLBACK(gtk_main_quit), NULL);

  gtk_main();*/

    GtkBuilder      *builder;
    GtkWidget       *window;
    GtkDrawingArea  *area;

    gtk_init (&argc, &argv);

    builder = gtk_builder_new ();
    gtk_builder_add_from_file (builder, "C:/Users/m.somorovsky/Documents/CBProjects/FMS2/gtkuiu.glade", NULL);
    window = GTK_WIDGET (gtk_builder_get_object (builder, "window1"));
    area = GTK_DRAWING_AREA(gtk_builder_get_object(builder, "drawningarea1"));
    gtk_builder_connect_signals (builder, NULL);

    g_object_unref (G_OBJECT (builder));
    //g_signal_connect_swapped(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(on_window1_destroy), NULL);



    gtk_widget_show (window);
    gtk_main ();

    ifstream file ("C:/Users/m.somorovsky/Dropbox/Opt-block-only.seq");
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

                fsm.addTransition(line);
                //cout<<line.length()<<endl;
                //cout<<line.substr(line.find_first_of("="))<<endl;
            }
            //cout<<line<<"\n";
        }
        fsm.connectAll();
        file.close();
        //int i=0;
        for(int i=0; i<fsm.transitions.size(); i++)
        {
            //cout<<"stav: "<<i<<" -> ";
            for(int j=0; j<fsm.transitions[i].size(); j++)
            {
                cout<<i<<" -> "<<fsm.transitions[i][j]<<" [ label = \""<<fsm.states[i]->getTransition(j)->Getevent()<<"\" ];"<<endl;
            }
            //cout<<endl;
        }

        /*for(int i=0; i<fsm.states.size(); i++)
        {
            cout<<i<<": "<<fsm.states[i]->Getname()<<endl;
                for(int j=0; j<fsm.states[i]->getTransitionSize(); j++)
                {
                    cout<<fsm.states[i]->getTransition(j)->getIndex()<<": "<<fsm.states[i]->getTransition(j)->GetnextState()<<endl;
                }
        }*/
    }
    else cout<<"CHYBA"<<endl;
    return 0;
}
