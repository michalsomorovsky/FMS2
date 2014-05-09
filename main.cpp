#include <iostream>
#include <fstream>
#include "FSM.h"
#include <gtk/gtk.h>
#include <cairo.h>
#include <math.h>
#include <gvc.h>

#ifdef WIN32
#define DLLEXPORT __declspec(dllexport)
#endif // WIN32

//loading FSM from file to internal representation
FSM loadFSM(char *filename, gpointer data);
//draw nodes of FSM
static void do_drawing(cairo_t *, char* text, double x, double y, double radius, double labelWidth, double labelHeight);
//draw connections between nodes
static void draw_line(cairo_t *, pointf *p, int size);
//draw event
static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data);
//not usefull //debug //experimental
void drawGraph(Agraph_t *g);
//export FSM from internal representation to file
void export_fsm(char *filename);

using namespace std;

typedef struct _ChData ChData;
struct _ChData
{
    /* Widgets */
    GtkFileChooser *file_chooser;  /* Main application window */
    GtkDrawingArea *darea;   /* Chart drawing area */
    FSM fsm;
};

struct
{
    int nodesCount;
    int coordx[100];
    int coordy[100];
    int farestx;
    int faresty;
    Agraph_t *g;
    bool drawn;
    bool zoom;
    double zoomLevel;
    FSM fsm;
    GtkBuilder      *builder;
} nodesCoord;

extern "C" DLLEXPORT void
on_toolbutton1_clicked (GtkToolButton *button, gpointer user_data)
{
    //GtkWidget *popup = GTK_WIDGET (gtk_builder_get_object (nodesCoord.builder, "window2"));
    //gtk_widget_show(popup);

    nodesCoord.fsm.addState("lama", "lama1", "lama2");
    Agnode_t *n;
    if(n = agnode(nodesCoord.g, "lama", TRUE)) cout<<"true"<<endl;
    else cout<<"false"<<endl;
    //ND_coord(n).x = 10;
    //ND_coord(n).y = 10;
    //cout<<"toolbiutton1"<<endl;
    //nodesCoord.zoom = true;
    //gtk_widget_queue_draw(GTK_WIDGET(((ChData *)user_data)->darea));
    //cairo_t *cr;
    //g_signal_connect(G_OBJECT(((ChData *)data)->darea), "draw", G_CALLBACK(on_draw_event), (void *)"lamka");
    //gtk_widget_queue_draw(GTK_WIDGET(((ChData *)data)->darea));
    //do_drawing(cr);
    //g_print("lama");
}

extern "C" DLLEXPORT void
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
        nodesCoord.fsm = fsm;
        //cout<<sizeof(fsm)<<endl;
        //cout<<sizeof(((ChData *)user_data)->fsm)<<endl;

        gulong h_id = g_signal_connect(G_OBJECT(((ChData *)user_data)->darea), "draw", G_CALLBACK(on_draw_event), user_data);
        //gtk_widget_get_preferred_size(((ChData *)user_data)->darea, )
        nodesCoord.drawn = false;
        //gtk_widget_set_size_request(GTK_WIDGET(((ChData *)user_data)->darea), nodesCoord.farestx+100, nodesCoord.faresty+100);
        //gtk_widget_queue_draw(GTK_WIDGET(((ChData *)user_data)->darea));
        //g_signal_handler_disconnect((((ChData *)user_data)->darea), h_id);
    }
    gtk_widget_hide(GTK_WIDGET(f));
}

extern "C" DLLEXPORT void
on_imagemenuitem3_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    GtkWidget *dialog;

     dialog = gtk_file_chooser_dialog_new ("Save File",
     				      GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(menuitem))),
     				      GTK_FILE_CHOOSER_ACTION_SAVE,
     				      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
     				      GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
     				      NULL);

     if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        export_fsm(filename);
    }
    gtk_widget_destroy(dialog);
}


extern "C" DLLEXPORT void
on_hscale1_value_changed(GtkRange *range, gpointer user_data)
{
    nodesCoord.zoomLevel = gtk_range_get_value(range)/100;
    nodesCoord.zoom = true;
    gtk_widget_queue_draw(GTK_WIDGET(((ChData *)user_data)->darea));
    //gtk_widget_set_size_request(GTK_WIDGET(((ChData *)user_data)->darea), nodesCoord.zoomLevel*(nodesCoord.farestx+200), nodesCoord.zoomLevel*(nodesCoord.faresty+200));
}

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr,
                              gpointer user_data)
{
    /*cout<<((ChData *)user_data)->fsm.states.size()<<endl;

    for(int i=0, x=80, y=80; i<((ChData *)user_data)->fsm.states.size(); i++)
    {

        do_drawing(cr, (gchar *)(((ChData *)user_data)->fsm.states[i]->Getname().c_str()), x+((i%5)*180), y);
        if(i>0 && i%5 == 0)
        {
            y=y+180;
        }
    }*/

    if(nodesCoord.zoom)
    {
        cairo_scale(cr, nodesCoord.zoomLevel, nodesCoord.zoomLevel);

    }

    Agnode_t *n;
    Agedge_t *e;
    int i=0;
    nodesCoord.nodesCount=0;
    nodesCoord.farestx=0;
    nodesCoord.faresty=0;
    for(n=agfstnode(nodesCoord.g); n; n=agnxtnode(nodesCoord.g, n), i++)
    {

        //cout<<ND_coord(n).x<<" : "<<ND_coord(n).y<<" === "<<ND_label(n)->text<<endl;
        //cout<<ND_label(n)->space.x<<" : "<<ND_label(n)->space.y<<endl;
        do_drawing(cr, ND_label(n)->text, ND_coord(n).x, ND_coord(n).y, ND_width(n)*33, ND_label(n)->dimen.x, ND_label(n)->dimen.y);
        if(ND_coord(n).x > nodesCoord.farestx) nodesCoord.farestx = ND_coord(n).x;
        if(ND_coord(n).y > nodesCoord.faresty) nodesCoord.faresty = ND_coord(n).y;
        for(e=agfstout(nodesCoord.g,n); e; e=agnxtout(nodesCoord.g,e))
        {

            //cout<<ED_label(e)->text<<" "<<ED_spl(e)->list->size<<endl;
            //cout<<"port: "<<ED_tail_port(e).p.x<<endl;
            //cout<<"posledny bod: "<<ED_spl(e)->list->list[ED_spl(e)->list->size-1].x<<" : "<<ED_spl(e)->list->list[ED_spl(e)->list->size-1].y<<endl;
            draw_line(cr, ED_spl(e)->list->list, ED_spl(e)->list->size);
            //cout<<ED_label(e)->text<<endl;
        }
    }
    //nodesCoord.drawn = false;
    gtk_widget_set_size_request(GTK_WIDGET(((ChData *)user_data)->darea), nodesCoord.farestx+200, nodesCoord.faresty+200);
    //gtk_widget_set_size_request(GTK_WIDGET(((ChData *)user_data)->darea), 0.5*(nodesCoord.farestx+200), 0.5*(nodesCoord.faresty+200));
    //gtk_widget_queue_draw(GTK_WIDGET(((ChData *)user_data)->darea));
    /*for(int i=0; i<nodesCoord.nodesCount; i++)
    {
        do_drawing(cr, "node", nodesCoord.coordx[i], nodesCoord.coordy[i]);
    }*/

    return FALSE;
}

static void draw_line(cairo_t *cr, pointf *p, int size)
{
    double angle;
    double x1, y1, x2, y2;
    /*cairo_set_source_rgb(cr, 1.0,0.0,0.0);
    for(int i=0; i<size; i++)
    {
        cairo_arc(cr, p[i].x, p[i].y, 2, 0, 360);
    }
    cairo_stroke(cr);
    cairo_set_source_rgb (cr, 0.0, 0.0, 1.0);*/
    cairo_move_to(cr, p[0].x, p[0].y);
    for(int i=1; i<size; i++)
    {
        if(size==2)
        {
            cairo_line_to(cr, p[i].x, p[i].y);

        }
        else if(size==3)
        {
            cairo_curve_to(cr, p[i-1].x+2/3*(p[i].x-p[i-1].x), p[i-1].y+2/3*(p[i].y-p[i-1].y), p[i+1].x+2/3*(p[i].x-p[i+1].x), p[i+1].y+2/3*(p[i].y-p[i+1].y), p[i+1].x, p[i+1].y);
            i=2;
        }
        else
        {
            if((size-i)==1)
            {

                cairo_line_to(cr, p[i].x, p[i].y);
            }
            else if((size-i)==2)
            {

                cairo_curve_to(cr, p[i-1].x+2/3*(p[i].x-p[i-1].x), p[i-1].y+2/3*(p[i].y-p[i-1].y), p[i+1].x+2/3*(p[i].x-p[i+1].x), p[i+1].y+2/3*(p[i].y-p[i+1].y), p[i+1].x, p[i+1].y);
                i+=2;
            }
            else
            {
                cairo_curve_to(cr, p[i].x, p[i].y, p[i+1].x, p[i+1].y, p[i+2].x, p[i+2].y);

                i+=2;
            }
        }
        if(i==size-1)
        {
            angle = atan2(p[i].y-p[i-1].y, p[i].x - p[i-1].x)+M_PI;
                x1=p[i].x+10*cos(angle-0.34906585);
                y1=p[i].y+10*sin(angle-0.34906585);
                x2=p[i].x+10*cos(angle+0.34906585);
                y2=p[i].y+10*sin(angle+0.34906585);
                cairo_line_to(cr, x1, y1);
                cairo_move_to(cr, p[i].x, p[i].y);
                cairo_line_to(cr, x2, y2);
        }
    }

    cairo_stroke(cr);
    /*cairo_move_to(cr, 50,50);
    cairo_line_to(cr, 50, 0);
    angle = atan2(50 - 50, 0-50)*(180/M_PI);*/

                /*x1=p[i].x+10*cos(angle-20);
                y1=p[i].y+10*sin(angle-20);
                x2=p[i].x+10*cos(angle+20);
                y2=p[i].y+10*sin(angle+20);
                cairo_arc(cr, x1, y1, 2, 0, 360);
                cairo_arc(cr, x2, y2, 2, 0, 360);
                cairo_line_to(cr, x1, y1);
                cairo_move_to(cr, p[i].x, p[i].y);
                cairo_line_to(cr, x2, y2);*/
}

static void do_drawing(cairo_t *cr, char* text, double x, double y, double radius, double labelWidth, double labelHeight)
{
    double xc = x;
    double yc = y;
    //double radius = 80.0;
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
    cairo_move_to (cr, x-labelWidth/2, y-labelHeight/2);
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
    nodesCoord.builder = builder;
    data = g_slice_new( ChData );
    data->darea = GTK_DRAWING_AREA(gtk_builder_get_object(builder, "drawingarea1"));
    data->file_chooser = GTK_FILE_CHOOSER(gtk_builder_get_object(builder, "filechooserdialog1"));

    gtk_builder_connect_signals (builder, data);

    //g_signal_connect(G_OBJECT(area), "draw", G_CALLBACK(on_draw_event), NULL);

    //g_signal_connect(G_OBJECT(area), "draw", G_CALLBACK(on_draw_event), NULL);
    //g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(on_button1_clicked), NULL);

    //g_signal_connect_swapped(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    //g_signal_connect(G_OBJECT(area), "draw", )
    //g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(on_window1_destroy), NULL);

    gtk_widget_show (window);

    gtk_main ();
    g_object_unref (G_OBJECT (builder));
    return 0;
}

void export_fsm(char *filename)
{
    ofstream file(filename);
    if(file.is_open())
    {
        for(int i=0; i<nodesCoord.fsm.states.size(); i++)
        {
            if(!nodesCoord.fsm.states[i]->getComment().empty()) file<<nodesCoord.fsm.states[i]->getComment();
            file<<"["<<nodesCoord.fsm.states[i]->Getname()<<"]"<<nodesCoord.fsm.states[i]->Getaction()<<"\n";
            for(int j=0; j<nodesCoord.fsm.states[i]->getTransitionSize(); j++)
            {
                file<<"\t"<<nodesCoord.fsm.states[i]->getTransition(j)->Getevent()<<" = "<<nodesCoord.fsm.states[i]->getTransition(j)->GetnextState()<<endl;
            }
            file<<endl;
        }
        file.close();
    }
}

FSM loadFSM(char *filename, gpointer data)
{
    ifstream file (filename);
    string line;
    FSM fsm;
    GVC_t *gvc;
    Agraph_t *g;
    string comment;

    if(file.is_open())
    {
        string action;
        while(getline(file, line))
        {
            if(line.find_first_of("`", 0, 1)==0)
            {
                comment += line+"\n";
            }
            else if(line.find_first_of("[", 0,1)==0)
            {
                if(line.find_first_of("=")!=string::npos) action = line.substr(line.find_first_of("="), string::npos);
                else action = string::npos;
                //if(comment.empty()){ comment = string::npos; }
                fsm.addState(line.substr(1, line.find("]")-1), action, comment);
                comment.clear();
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
        /*for(int i=0; i<fsm.transitions.size(); i++)
        {
            //cout<<"stav: "<<i<<" -> ";
            for(int j=0; j<fsm.transitions[i].size(); j++)
            {
                cout<<i<<" -> "<<fsm.transitions[i][j]<<" [ label = \""<<fsm.states[i]->getTransition(j)->Getevent()<<"\" ];"<<endl;
            }
        }*/
        string cp;
        cp = "digraph finite_state_machine { rankdir=LR;   size=\"200\" node [shape = circle];\n";
        for(int i=0; i<fsm.states.size(); i++)
        {
            for(int j=0; j<fsm.states[i]->getTransitionSize(); j++)
            {
                cp += fsm.states[i]->Getname() + " -> " + fsm.states[i]->getTransition(j)->GetnextState() + " [ label = \"" + fsm.states[i]->getTransition(j)->Getevent() + "\" ];\n";
                //cout<<fsm.states[i]->Getname()<<" -> "<<fsm.states[i]->getTransition(j)->GetnextState()<<" [ label = \""<<fsm.states[i]->getTransition(j)->Getevent()<<"\" ];"<<endl;
            }
        }
        cp+="}";
        const char* ccp = cp.c_str();
        gvc = gvContext();
        g = agmemread(ccp);
        gvLayout(gvc, g, "dot");
        //gvRender(gvc, g, "dot", NULL);
        nodesCoord.g = g;
        //drawGraph(g);
        //gvRenderFilename(gvc, g, "svg", "out.svg");
        //gvRender(gvc, g, "plain", stdout);
        //gvFreeLayout(gvc, g);
        //agclose(g);
        //gvFreeContext(gvc);
        //cout<<cp<<endl;
        return fsm;
    }
    else cout<<"CHYBA"<<endl;
}

void drawGraph(Agraph_t *g)
{
    Agnode_t *n;
    int i=0;
    nodesCoord.nodesCount=0;
    nodesCoord.farestx=0;
    nodesCoord.faresty=0;
    for(n=agfstnode(g); n; n=agnxtnode(g, n), i++)
    {

        cout<<ND_coord(n).x<<" : "<<ND_coord(n).y<<" === "<<ND_label(n)->text<<" "<<GD_label(g)<<endl;
        nodesCoord.coordx[nodesCoord.nodesCount]=ND_coord(n).x;
        nodesCoord.coordy[nodesCoord.nodesCount++]=ND_coord(n).y;
        if(ND_coord(n).x > nodesCoord.farestx) nodesCoord.farestx = ND_coord(n).x;
        if(ND_coord(n).y > nodesCoord.faresty) nodesCoord.faresty = ND_coord(n).y;
    }

}
