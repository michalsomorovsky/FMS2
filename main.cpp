#include <iostream>
#include <fstream>
#include "FSM.h"
#include <gtk/gtk.h>
#include <cairo.h>
#include <math.h>

void loadFSM();
static void do_drawing(cairo_t *);

void on_window1_destroy (GtkWindow *object, gpointer user_data)
{
    cout<<"lama";
    gtk_main_quit ();
}

using namespace std;

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr,
                              gpointer user_data)
{
    do_drawing(cr);

    return FALSE;
}

static void do_drawing(cairo_t *cr)
{
    double xc = 100.0;
    double yc = 100.0;
    double radius = 10.0;
    double angle1 = 0.0  * (M_PI/180.0);  /* angles are specified */
    double angle2 = 360.0 * (M_PI/180.0);  /* in radians           */

    cairo_set_line_width (cr, 10.0);
    cairo_arc (cr, xc, yc, radius, angle1, angle2);
    cairo_stroke (cr);

    cairo_set_line_width (cr, 1.0);
    cairo_rectangle(cr, 200.0, 100.0, 100.0, 100.0);
    cairo_stroke(cr);

    //cairo_move_to(cr, 0.0, 0.0);
    //cairo_line_to(cr, 100.0, 100.0);
    //cairo_line_to(cr, 100.0, 150.0);
    //cairo_curve_to(cr, 120.0, 130.0, 140.0, 150.0, 200.0, 150.0);
    //cairo_move_to(cr, 250.0, 150.0);
    //cairo_text_path(cr, "lama");
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_scale(cr, 1, 0.7);
    cairo_arc(cr, 300, 100, 50, 0, 2*M_PI);
    cairo_fill(cr);
    //cairo_stroke(cr);

    PangoLayout *layout;
    PangoFontDescription *font_description;

    font_description = pango_font_description_new ();
    pango_font_description_set_family (font_description, "serif");
    pango_font_description_set_weight (font_description, PANGO_WEIGHT_BOLD);
    pango_font_description_set_absolute_size (font_description, 12 * PANGO_SCALE);

    layout = pango_cairo_create_layout (cr);
    pango_layout_set_font_description (layout, font_description);
    pango_layout_set_text (layout, "Hello, world", -1);

    cairo_set_source_rgb (cr, 0.0, 0.0, 1.0);
    cairo_move_to (cr, 250.0, 100.0);
    pango_cairo_show_layout (cr, layout);

    g_object_unref (layout);
    pango_font_description_free (font_description);

    /* draw helping lines */
    //cairo_set_source_rgba (cr, 1, 0.2, 0.2, 0.6);
    //cairo_set_line_width (cr, 6.0);

    //cairo_arc (cr, xc, yc, 10.0, 0, 2*M_PI);
    //cairo_fill (cr);

    //cairo_arc (cr, xc, yc, radius, angle1, angle1);
    //cairo_line_to (cr, xc, yc);
    //cairo_arc (cr, xc, yc, radius, angle2, angle2);
    //cairo_line_to (cr, xc, yc);
    //cairo_stroke (cr);
}


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
    g_signal_connect_swapped(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(G_OBJECT(area), "draw", G_CALLBACK(on_draw_event), NULL);
    //g_signal_connect(G_OBJECT(area), "draw", )
    //g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(on_window1_destroy), NULL);



    gtk_widget_show (window);

    loadFSM();

    gtk_main ();


    return 0;
}

void loadFSM()
{
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
}
