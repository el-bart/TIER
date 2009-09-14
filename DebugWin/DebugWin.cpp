#include <iostream>
#include <signal.h>
#include <gtk/gtk.h>

#include "Threads/Mutex.hpp"
#include "DebugRecieve.hpp"
#include "RasterSwapper.hpp"

extern "C"
{
#include "bitmap/bitmap.h"
#include "raster/raster.h"
}


using namespace std;
using namespace Threads;
using namespace DebugWin;


guchar         rgbbuf[1500*1500 * 3];
struct raster *r_in    =NULL;
struct raster *r_scaled=NULL;
const int      dim_x=450;
const int      dim_y=dim_x*480/640;
bool           imgChanged=true;  // true if new image has been provided
Mutex          r_mutex;     // mutex for r_scaled and r_in

int timeout=125;    // [ms]

gboolean on_darea_expose(GtkWidget *widget,
                         GdkEventExpose *event,
                         gpointer user_data);

gint redraw(gpointer data);

gboolean draw_data(GtkWidget *widget, int width, int height);

void close_win(void);





int main (int argc, char *argv[])
{
  try
  {

    if(argc!=1+2)
    {
      cerr<<argv[0]<<" <listen_ip> <listen_port>"<<endl;
      return 1;
    };

    // we need to ignore SIG_BROKEN_PIPE:
    signal(SIGPIPE, SIG_IGN);

    GtkWidget *window, *darea;

    // otwieramy poczatkowy rysunek
    r_in=raster_create(dim_x,dim_y);

    int arg1=1;
    gtk_init(&arg1, &argv);

    window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    darea =gtk_drawing_area_new();
    gtk_widget_set_size_request(darea, dim_x,dim_y);
    gtk_container_add( GTK_CONTAINER(window), darea);
    gtk_signal_connect( GTK_OBJECT(darea), "expose-event",
        GTK_SIGNAL_FUNC(on_darea_expose), NULL );
    gtk_signal_connect( GTK_OBJECT(darea), "destroy",
        GTK_SIGNAL_FUNC(close_win), NULL );
    gtk_widget_show_all(window);

    // przerysowywanie co okreslony czas
    gtk_timeout_add(timeout, redraw, (gpointer)darea);

    // odpalamy server:
    Network::Address address(argv[1], atoi(argv[2]) );
    RasterSwapper    swapper(r_mutex, &r_in, &r_scaled, &imgChanged);
    DebugRecieve     dRecieve(address, swapper);

    dRecieve.start();       // start capturing...

    //
    // uruchamiamy petle GTK+
    //
    gtk_main();

  }
  catch(const CGraphics::Exception &ex)
  {
    cerr<<argv[0]<<": CGraphics exception: "<<ex.what()<<endl;
  }
/*  catch(const Graphics::Exception &ex)
  {
    cerr<<argv[0]<<": Graphics exception: "<<ex.what()<<endl;
  }*/
  catch(const Network::Exception &ex)
  {
    cerr<<argv[0]<<": Network exception: "<<ex.what()<<endl;
  }
  catch(const Threads::Exception &ex)
  {
    cerr<<argv[0]<<": Threads exception: "<<ex.what()<<endl;
  }
  catch(...)
  {
    cerr<<argv[0]<<": generic exception cought..."<<endl;
  };

  return 0;
} // main()


gboolean on_darea_expose(GtkWidget *widget,
                         GdkEventExpose *event,
                         gpointer user_data)
{
  return draw_data(widget, event->area.width, event->area.height);
}; // on_darea_expose()


gint redraw(gpointer data)
{
  draw_data( (GtkWidget*)data, dim_x,dim_y);    // refresh
  gtk_timeout_add(timeout, redraw, data);       // set new timeout
  return 0;
}; // redraw()


// ogolna f-cja przerysowujaca obraz na erkanie
gboolean draw_data(GtkWidget *widget, int width, int height)
{
  // critical section
  {
    Lock lock(r_mutex);

    // skipp if no changes to be made...
    if(imgChanged)
      imgChanged=false;
    else
      return TRUE;

    if( r_scaled==NULL || raster_dim_x(r_scaled)!=width  ||
                          raster_dim_y(r_scaled)!=height    )
    {
      if(r_scaled!=NULL)
        raster_free(r_scaled);
      r_scaled=raster_scale_simple(r_in, width, height);
    };

    for(int y=0; y<raster_dim_y(r_scaled); y++)
    {
      for(int x=0; x<raster_dim_x(r_scaled); x++)
      {
        struct rgb24 pix=raster_pix_get(r_scaled, x,y);
        rgbbuf[y*raster_dim_x(r_scaled)*3+3*x+0]=pix.r;
        rgbbuf[y*raster_dim_x(r_scaled)*3+3*x+1]=pix.g;
        rgbbuf[y*raster_dim_x(r_scaled)*3+3*x+2]=pix.b;
      };
    };
  };
  // end of critical section

  gdk_draw_rgb_image( widget->window, widget->style->fg_gc[GTK_STATE_NORMAL],
                      0, 0, dim_x,dim_y,
                      GDK_RGB_DITHER_MAX, rgbbuf, dim_x*3 );

  return TRUE;
} // draw_data()


void close_win(void)
{
  gtk_exit(0);
}; // close_win

