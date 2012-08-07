/*
 * denoise.c
 *
 */
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "denoise.h"



// pomocnicze makra

#define IS_VISITED(r,  x,y)	( raster_marker_get(r, x,y)!= 0  )
#define SET_VISITED(r, x,y)	( raster_marker_set(r, x,y,   1) )

#define IS_CURRENT(r,  x,y)	( raster_marker_get(r, x,y)==-1  )
#define SET_CURRENT(r, x,y)	( raster_marker_set(r, x,y,  -1) )


// zaznacza kolejny znaleziony obszar
static int check(struct raster *r, int dim_x, int dim_y, int x, int y)
{
 struct rgb24 cur;
 int          i,j;
 int          cnt_now;

 cnt_now=0;
 cur    =raster_pix_get(r, x,y);

 if( IS_VISITED(r, x,y) )			// tu juz bylismy
   return 0;

 if( cur.r<240 || cur.g<240 || cur.b<240 )	// nie jasny?
   return 0;

 // zaznaczamy, ze tu juz bylismy
 SET_CURRENT(r, x,y);
 cnt_now++;

 for(i=-1; i<=1; i++)
   for(j=-1; j<=1; j++)
   {
     if( 0<=x+i && x+i<dim_x &&
         0<=y+j && y+j<dim_y    )
       cnt_now+=check(r, dim_x,dim_y, x+i,y+j);
   };

 return cnt_now;
}; // check()



// ustawia elementy CURRENT na VISITED, oraz (jezeli task!=0)
// usuwa dane pixele z obrazu wejsciowego
static int from_current(struct raster *r, int dim_x, int dim_y,
                        int x, int y, int task)
{
 int          i,j;
 struct rgb24 black={0,0,0};

 if( !IS_CURRENT(r, x,y) )	// nie bierzacy?
   return 0;

 SET_VISITED(r, x,y);		// zaznaczamy, ze tu juz bylismy
 if(task)				// usuwamy?
   raster_pix_set(r, x,y, &black);	// no to na czarno go!

 for(i=-1; i<=1; i++)
   for(j=-1; j<=1; j++)
   {
     if( 0<=x+i && x+i<dim_x &&
         0<=y+j && y+j<dim_y    )
       from_current(r, dim_x,dim_y, x+i,y+j, task);
   };

 return 0;
}; // from_current()





struct raster *filter_denoise(struct raster *r_in, int cnt_min)
{
 struct raster *r_out;			// obraz wyjsciowy
 int            dim_x,dim_y;		// wymiary rastra
 int            x,y;			// for()

 dim_x=raster_dim_x(r_in);
 dim_y=raster_dim_y(r_in);
 r_out=raster_clone(r_in);
 raster_marker_create(r_out);		// dodajemy marker do obrazu

 // lecimy po obrazq
 for(x=0; x<dim_x; x++)
   for(y=0; y<dim_y; y++)
   {
     int task;

     if( check(r_out, dim_x,dim_y, x,y) > cnt_min )
       task=0;				// duzy obszar - nie ruszamy!
     else
       task=1;

     from_current(r_out, dim_x,dim_y, x,y, task);	// ustaw na VISITED!
   }; // for([x;y])

 raster_marker_free(r_out);		// usuwamy marker z pamieci

 return r_out;
}; // denoise()


