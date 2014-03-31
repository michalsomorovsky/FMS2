#include <iostream>
#include <fstream>
#include "FSM.h"
#include <gtk/gtk.h>
#include <cairo.h>
#include <math.h>
#include <windows.h>

FSM loadFSM(char *filename, gpointer data);
static void do_drawing(cairo_t *, char* text, double x, double y);
static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data);

using namespace std;

typedef struct _ChData ChData;
struct _ChData
{
    /* Widgets */
    GtkFileChooser *file_chooser;  /* Main application window */
    GtkDrawingArea *darea;   /* Chart drawing area */
    FSM fsm;
};

extern "C" __declspec(dllexport) void
on_toolbutton1_clicked (GtkToolButton *button, gpointer data)
{
    //cairo_t *cr;
    //g_signal_connect(G_OBJECT(((ChData *)data)->darea), "draw", G_CALLBACK(on_draw_event), (void *)"lamka");
    //gtk_widget_queue_draw(GTK_WIDGET(((ChData *)data)->darea));
    //do_drawing(cr);
    //g_print("lama");
}

extern "C" __declspec(dllexport) void
on_imagemenuitem2_activate(GtkMenuItem *menuitem, gpointer user_data)
{

    //g_print(((ChData *)user_data)->file_chooser);
    GtkFileChooser *f = ((ChData *)user_data)->file_chooser;

    //gtk_dialog_run(GTK_DIALOG(f));

    if (gtk_dialog_run(GTK_DIALOG(f)) == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        filename = gtk_file_chooser_get_filename(f);
        g_print(filename);
        FSM fsm = loadFSM(filename, user_data);
        ((ChData *)user_data)->fsm = fsm;

        g_signal_connect(G_OBJECT(((ChData *)user_data)->darea), "draw", G_CALLBACK(on_draw_event), user_data);
        gtk_widget_queue_draw(GTK_WIDGET(((ChData *)user_data)->darea));
    }
    gtk_widget_hide(GTK_WIDGET(f));

}

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr,
                              gpointer user_data)
{
    cout<<((ChData *)user_data)->fsm.states.size()<<endl;

    for(int i=0, x=80, y=80; i<((ChData *)user_data)->fsm.states.size(); i++)
    {

        do_drawing(cr, (gchar *)(((ChData *)user_data)->fsm.states[i]->Getname().c_str()), x+((i%5)*180), y);
        if(i>0 && i%5 == 0)
        {
            y=y+180;
        }
    }


    return FALSE;
}

static void do_drawing(cairo_t *cr, char* text, double x, double y)
{
    double xc = x;
    double yc = y;
    double radius = 80.0;
    double angle1 = 0.0  * (M_PI/180.0);  /* angles are specified */
    double angle2 = 360.0 * (M_PI/180.0);  /* in radians           */

    //cairo_set_line_width (cr, 10.0);
    //cairo_move_to(cr, xc+(i*105), yc+(j*105));
    cairo_move_to (cr, xc+radius, yc);
    cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);
    cairo_arc (cr, xc, yc, radius, angle1, angle2);
    cairo_stroke (cr);

    //cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    //cairo_scale(cr, 1, 0.7);
    //cairo_arc(cr, 300, 100, 50, 0, 2*M_PI);
    //cairo_fill(cr);
    //cairo_stroke(cr);

    PangoLayout *layout;
    PangoFontDescription *font_description;

    font_description = pango_font_description_new ();
    pango_font_description_set_family (font_description, "serif");
    pango_font_description_set_weight (font_description, PANGO_WEIGHT_BOLD);
    pango_font_description_set_absolute_size (font_description, 12 * PANGO_SCALE);

    layout = pango_cairo_create_layout (cr);
    pango_layout_set_font_description (layout, font_description);
    pango_layout_set_text (layout, text, -1);

    cairo_set_source_rgb (cr, 0.0, 0.0, 1.0);
    cairo_move_to (cr, x-20, y-5);
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
    GtkBuilder      *builder;
    GtkWidget       *window;
    GtkDrawingArea  *area;
    GtkButton       *button;
    ChData *data;
    gtk_init (&argc, &argv);

    builder = gtk_builder_new ();
    /*char path[MAX_PATH];
    GetCurrentDirectory(sizeof(path), path);
    MessageBox(0, path, "Current Path", 0);*/
    gtk_builder_add_from_file (builder, "gtkgui.glade", NULL);
    window = GTK_WIDGET (gtk_builder_get_object (builder, "window1"));
    area = GTK_DRAWING_AREA(gtk_builder_get_object(builder, "drawingarea1"));
    button = GTK_BUTTON(gtk_builder_get_object(builder, "button1"));

    data = g_slice_new( ChData );
    data->darea = GTK_DRAWING_AREA(gtk_builder_get_object(builder, "drawingarea1"));
    data->file_chooser = GTK_FILE_CHOOSER(gtk_builder_get_object(builder, "filechooserdialog1"));

    gtk_builder_connect_signals (builder, data);

    //g_signal_connect(G_OBJECT(area), "draw", G_CALLBACK(on_draw_event), NULL);
    g_object_unref (G_OBJECT (builder));
    //g_signal_connect(G_OBJECT(area), "draw", G_CALLBACK(on_draw_event), NULL);
    //g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(on_button1_clicked), NULL);

    //g_signal_connect_swapped(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    //g_signal_connect(G_OBJECT(area), "draw", )
    //g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(on_window1_destroy), NULL);

    gtk_widget_show (window);

    gtk_main ();

    return 0;
}

FSM loadFSM(char *filename, gpointer data)
{
    ifstream file (filename);
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

        return fsm;
    }
    else cout<<"CHYBA"<<endl;
}
