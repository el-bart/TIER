/*
 * scale_raster.c
 *
 */
#include <stdlib.h>

#include "scale_raster.h"


// zwraca mniejsza z wartosci
#define MIN(a,b)	( (a)<(b)?(a):(b) )



struct raster *raster_scale_simple(struct raster *r, int size_x, int size_y)
{
 struct raster *r_out;		// obraz wyjsciowy
 int            x,y;		// for()
 struct rgb24   pix;		// punkt do przekopiowania
 double         scale_x,scale_y;// skala na osi OX, OY
 int            xs,ys;		// x;y przeskalowane

 if(size_x<1 || size_y<1 || r==NULL)	// parametry nie sa sensowne?
   return NULL;

 r_out=raster_create(size_x, size_y);	// tworzymy obraz docelowy
 if(r_out==NULL)
   return NULL;

 // wyznaczamy wspolczynniki skali:
 scale_x=raster_dim_x(r)/(size_x+0.0);
 scale_y=raster_dim_y(r)/(size_y+0.0);

 // kopiowanie odpowiednich punktow
 for(x=0; x<size_x; x++)
   for(y=0; y<size_y; y++)
   {
     xs=MIN(raster_dim_x(r), (int)(x*scale_x) );
     ys=MIN(raster_dim_y(r), (int)(y*scale_y) );
     pix=raster_pix_get(r, xs,ys);
     raster_pix_set(r_out, x,y, &pix);
   }; // for([x;y])

 return r_out;
};


