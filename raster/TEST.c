/*
 * TEST.c
 *
 * prosty program przeprowadzajacy pare podstawowych
 * testow na bibliotece raster.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "raster.h"


// wypisuje raster na ekranie
void printf_raster(struct raster *r)
{
 int x,y;	// for

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
}; // printf_raster()





int main(void)
{
 struct raster *r;	// raster do testow
 struct raster *r2;	// drugi :)
 struct raster *r3;	// :)
 struct raster *r_out;	// i wyniczek odejmowanka :)
 struct raster *tmp;	// pomocniczy
 struct rgb24   pix[2];	// pixel - pomocniczy
 struct bmp    *p;	// obrazek :)

 assert( raster_create(0,0)==NULL );
 assert( (r=raster_create(2,3))!=NULL );

 pix[0].r= 10;
 pix[0].g= 20;
 pix[0].b= 40;
 pix[1].r=200;
 pix[1].g=210;
 pix[1].b=240;

 raster_pix_set(r, 1, 0, &pix[0]);
 raster_pix_set(r, 1, 2, &pix[1]);

 printf_raster(r);

 // proba zapisu do bmp :)
 p=raster_to_bmp(r);
 assert(p!=NULL);
 assert( bmp_save_file(p, "test_out.bmp", 0644)==0 );
 assert( bmp_free(p)==0 );

 // proba odczytu z bmp :)
 assert( bmp_load_file(p, "test_out.bmp")==0 );
 assert( raster_free(r)==0 );
 r=raster_from_bmp(p);
 assert(r!=NULL);

 // usowanie zbednych danych
 assert( bmp_free(p)==0 );
 free(p);

 // proba kopiowania
 tmp=r;
 r2 =raster_clone(tmp);
 r  =raster_clone(tmp);
 assert(r!=NULL);

 // proba uzyskania obrazu B&W
 r3=raster_grayscale(r);
 printf("\n");
 printf_raster(r3);
 printf("\n");
 printf_raster(tmp);

 // proba odjecia obrazow
 printf("\nsubstract rasters:\n");
 raster_pix_set_r(r2, 1, 0,  52);
 raster_pix_set_g(r2, 1, 0,   1);
 raster_pix_set_b(r2, 1, 0,  12);
 raster_pix_set_g(r2, 1, 2, 100);
 printf_raster(r3);
 printf("\n");
 printf_raster(r2);
 printf("\n");
 r_out=raster_substract(r3, r2);
 assert(r_out!=NULL);
 printf("daje:\n");
 printf_raster(r_out);
 printf("\n");

 // konczymy:
 assert( raster_free(r_out)==0 );
 assert( raster_free(r2)==0 );
 assert( raster_free(tmp)==0 );
 assert( raster_free(r)==0 );
 assert( raster_free(r3)==0 );

 return 0;
};


