/*
 * TEST.c
 *
 * prosty program demonstrujacy filtracje
 *
 */
#include <stdio.h>

#include "filter.h"




// wypisuje raster na ekranie
void printf_raster(struct raster *r)
{
 int x,y;       // for

 for(y=0; y<raster_dim_y(r); y++)
 {
   for(x=0; x<raster_dim_x(r); x++)
   {
     printf("[%3d;%3d;%3d] ", raster_pix_get_r(r,x,y),
                              raster_pix_get_g(r,x,y),
                              raster_pix_get_b(r,x,y)  );
   }; // for(y)
   printf("\n");
 }; // for(x)
 printf("\n");
}; // printf_raster()





int main(void)
{
 struct raster *r_in;		// obraz wejsciowy
 struct raster *r_out;		// obraz przefiltrowany
 int            x,y;		// for()

 r_in=raster_create(5,6);	// tworzmy nowy obraz

 // wypelnianie obrazu
 for(y=0; y<6; y++)
   for(x=0; x<5; x++)
   {
     struct rgb24 p;		// pomocniczy pixel

     // ustawiamy kolorki:
     p.r=y*99+60*x+43;
     p.g=y*99+40*x+36;
     p.b=y*99+14*x+21;

     raster_pix_set(r_in, x, y, &p);	// wstawiamy pixel do rastra
   }; // for([x;y]) 


 printf_raster(r_in);		// obraz wejsciowy

 // filtr Laplace'a:
 r_out=filter_laplace(r_in);	
 printf_raster(r_out);
 raster_free(r_out);

 // binaryzacja
 r_out=filter_cut_off(r_in, 128);
 printf_raster(r_out);

 // dylatacja
// r_out=filter_cut_off(r_in, 128);	// tu pracujemy na binarnym!
 r_out=filter_dilatation(r_out);
 printf_raster(r_out);
 raster_free(r_out);

 // koniec zabawy :)
 raster_free(r_in );

 return 0;
}; // main()


