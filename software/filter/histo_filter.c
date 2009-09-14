/*
 * histo_filter.h
 *
 */
#include <stdlib.h>

#include "main_filters.h"
#include "raster/histogram.h"


struct hist_max_min
{
 int max;			// koniec histogramu
 int min;			// poczatek histogramu
}; // struct hist_max_min


// zwraca strkture z zapiskami o poczatq/koncu histogramu
static struct hist_max_min get_histo_max_min(struct histogram h)
{
 struct hist_max_min mm;	// wartosci wyjsciowe
 int                 i;		// for()

 // wartosci poczatkowe
 mm.min=0;
 mm.max=255;

 // szukamy gdzie sie zaczyna histogram
 for(i=0; i<256; i++)
   if(h.col[i]!=0)
   {
     mm.min=i;
     break;
   };
 // oraz gdzie sie konczy
 for(i=255; i>=0; i--)
   if(h.col[i]!=0)
   {
     mm.max=i;
     break;
   };

 return mm;
}; // get_histo_max_min()


#include <stdio.h>



struct raster *filter_histogram_scatter(struct raster *r)
{
 struct raster       *r_out;	// obraz wyjsciowy
 int                  x,y;	// for()
 struct histogram     hist_r,
                      hist_g,
                      hist_b;	// histogram obrazu wejsciowego [RGB]
 struct hist_max_min  mm_r,
                      mm_g,
                      mm_b;	// zakres histogramu [RGB]

 // pobieramy histogramy:
 hist_r=raster_histogram_get(r, HISTOGRAM_RED);
 hist_g=raster_histogram_get(r, HISTOGRAM_GREEN);
 hist_b=raster_histogram_get(r, HISTOGRAM_BLUE);

 // wyznaczamy zakresy dla histogramow:
 mm_r=get_histo_max_min(hist_r);
 mm_g=get_histo_max_min(hist_g);
 mm_b=get_histo_max_min(hist_b);

 // tworzymy obraz wyjsciowy
 r_out=raster_create( raster_dim_x(r), raster_dim_y(r) );
 if(r_out==NULL)
   return NULL;

 //
 // rozciaganie histogramu wlasciwe
 //
 for(y=0; y<raster_dim_y(r); y++)
   for(x=0; x<raster_dim_x(r); x++)
   {
     struct rgb24 pix;		// bierzacy pixel obrazu wejsciowego

     pix=raster_pix_get(r, x, y);	// pobieramy wartosc
     raster_pix_set_r(r_out, x,y, 
                      (int)((255.0*(pix.r-mm_r.min))/(mm_r.max-mm_r.min)) );
     raster_pix_set_g(r_out, x,y,
                      (int)((255.0*(pix.g-mm_g.min))/(mm_g.max-mm_g.min)) );
     raster_pix_set_b(r_out, x,y,
                      (int)((255.0*(pix.b-mm_b.min))/(mm_b.max-mm_b.min)) );
   }; // for(y,x)

 return r_out;
}; // filter_histogram_scatter()






struct raster *filter_histogram_alignment(struct raster *r)
{
 struct raster       *r_out;	// obraz wyjsciowy
 int                  x,y;	// for()
 struct histogram     hist_r,
                      hist_g,
                      hist_b;	// histogram obrazu wejsciowego [RGB]

 // pobieramy histogramy:
 hist_r=raster_histogram_get(r, HISTOGRAM_RED);
 hist_g=raster_histogram_get(r, HISTOGRAM_GREEN);
 hist_b=raster_histogram_get(r, HISTOGRAM_BLUE);

 // dalej pracujemy juz na DYSTRYBUANCIE:
 hist_r=raster_histogram_integrate(hist_r);
 hist_g=raster_histogram_integrate(hist_g);
 hist_b=raster_histogram_integrate(hist_b);

 // tworzymy obraz wyjsciowy
 r_out=raster_create( raster_dim_x(r), raster_dim_y(r) );
 if(r_out==NULL)
   return NULL;

 //
 // wyrownanie histogramu wlasciwe
 //
 for(y=0; y<raster_dim_y(r); y++)
   for(x=0; x<raster_dim_x(r); x++)
   {
     struct rgb24 pix;		// bierzacy pixel obrazu wejsciowego

     pix=raster_pix_get(r, x, y);	// pobieramy wartosc
     raster_pix_set_r(r_out, x,y, (255*hist_r.col[pix.r])/hist_r.col[255] );
     raster_pix_set_g(r_out, x,y, (255*hist_g.col[pix.g])/hist_g.col[255] );
     raster_pix_set_b(r_out, x,y, (255*hist_b.col[pix.b])/hist_b.col[255] );
   }; // for(y,x)

 return r_out;
}; // filter_histogram_scatter()



