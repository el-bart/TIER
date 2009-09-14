/*
 * histogram.c
 *
 */
#include <assert.h>	// assert()
#include <string.h>	// memset()

#include "histogram.h"




struct histogram raster_histogram_get(struct raster *r, int hist_type)
{
 int              x,y;	// for()
 struct histogram h;	// nasz histogram

 memset(&h, 0, sizeof(struct histogram) );	// zerujemy histogram

 for(y=0; y<raster_dim_y(r); y++)
   for(x=0; x<raster_dim_x(r); x++)
     switch(hist_type)
     {
       case HISTOGRAM_RED:
            h.col[ raster_pix_get_r(r, x, y) ]++;	// zaznaczamy kolor R
            break;
       case HISTOGRAM_GREEN:
            h.col[ raster_pix_get_g(r, x, y) ]++;	// zaznaczamy kolor G
            break;
       case HISTOGRAM_BLUE:
            h.col[ raster_pix_get_b(r, x, y) ]++;	// zaznaczamy kolor B
            break;
       default:
            assert( hist_type!=hist_type );		// ktos zle podal
            break;
     }; // switch(hist_type)

 return h;
}; // raster_get_histogram()





struct histogram raster_histogram_integrate(struct histogram h)
{
 int i;

 // calkowanie - poniewaz otrzymalismy argument przez wartosc,
 // mozemy sobie pozwlic na prace na "wejsciowym" histogramie
 for(i=1; i<256; i++)
   h.col[i]+=h.col[i-1];

 return h;
}; // raster_histogram_integrate()





struct histogram raster_histogram_smooth(struct histogram h)
{
 int              v[2+1+2];	// punkty sasiednie
 struct histogram h_out;	// histogram wyjsciowy
 int              x;		// for()
 int              val;		// ostateczna wartosc do wstawienia

 for(x=0; x<256; x++)
 {
   v[0]=v[1]=v[2]=v[3]=v[4]=raster_histogram_color_get(h, x);
   // lewe sasiedztwo:
   if(x>0)
     v[1]=raster_histogram_color_get(h, x-1);
   if(x>1)
     v[0]=raster_histogram_color_get(h, x-2);
   // prawe sasiedztwo:
   if(x<255)
     v[3]=raster_histogram_color_get(h, x+1);
   if(x<254)
     v[4]=raster_histogram_color_get(h, x+2);

   // wartosc srednia:
   val=(v[0]+v[1]+v[2]+v[3]+v[4])/5;
   raster_histogram_color_set(h_out, x, val);
 };

 return h_out;
}; // raster_histogram_smooth()





int raster_histogram_threshold_get(struct histogram h)
{
 int x;				// for()
 int level;			// nasz punkt progowy
 int level_val;			// wartosc dla punktu odciecia

 // przegladamy histogram w poszukiwaniu kandydatow, omijajac
 // krawedzie gdyz tam czesto pojawia sie jakas sieczka...
 for(level=255/2, level_val=1024*1024, x=256-30; x>0+30; x--)
 {
   int sas[2+1+2];		// sasiedztwo neszego punktu
   int i;			// for()
   // pobieramy punkty sasiedztwa
   for(i=-2; i<=2; i++)
     sas[i+2]=raster_histogram_color_get(h, x+i);	// pobieramy wartosc

   // sprawdzamy, czy zaleznosci NIE sa spelnone (wtedy pomijamy ten punkt)
   if( level_val<raster_histogram_color_get(h, x) )
     continue;
//fprintf(stderr, "#");
/*int ix;
for(ix=0; ix<5; ix++)
fprintf(stderr, "l:%3d[%2d]==%d\n", x, ix-2, sas[ix]);
fprintf(stderr, "----\n");*/
   if( //sas[-3+3]<sas[-2+3] ||
       sas[-2+2]<sas[-1+2] ||
       sas[-1+2]<sas[ 0+2] ||
       sas[ 0+2]>sas[ 1+2] ||
       sas[ 1+2]>sas[ 2+2] //||
       //sas[ 2+3]>sas[ 3+3]
     )
     continue;

   // skoro wszsytkie zaleznosci sa spelnione, to mamy nowy punkt progowy!
   level    =x;
   level_val=sas[2];

 }; // for(hist_color)

 return level;
}; // raster_histogram_threshold_get()



