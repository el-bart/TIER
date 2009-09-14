/*
 * raster.c
 *
 */
#include <stdlib.h>	// malloc()
#include <string.h>	// memset()

#include "raster.h"



struct raster *raster_create(int dim_x, int dim_y)
{
 struct raster *r;

 if(dim_x<=0 || dim_y<=0)	// nie poprawne wymiary?
   return NULL;

 // tworzymy raster
 r=(struct raster*)malloc( sizeof(struct raster) );
 if(r==NULL)
   return NULL;

 // tworzymy tablice punktow
 r->pixs=(struct rgb24*)malloc( dim_x*dim_y*sizeof(struct rgb24) );
 if(r->pixs==NULL)
 {
   free(r);
   return NULL;
 };

 // ustawiamy wymiary rastra
 r->dim_x=dim_x;
 r->dim_y=dim_y;

 // domyslnie NIE mamy marker'a
 r->marker=NULL;

 return r;
}; // raster_create()




int raster_free(struct raster *r)
{
 if(r==NULL)
   return -1;

 raster_marker_free(r);		// zwalniamy marker, jesli takowy istnieje

 free(r->pixs);
 free(r);

 return 0;
}; // raster_free()




int raster_black(struct raster *r)
{
 memset(r->pixs, 0, raster_dim_x(r)*raster_dim_y(r)*sizeof(struct rgb24) );
 return 0;
}; // raster_black()




struct raster *raster_clone(struct raster *r)
{
 struct raster *cp;	// kopia rastra
 int            x,y;	// for()

 // tworzymy nowy raster:
 cp=raster_create( raster_dim_x(r), raster_dim_y(r) );
 if(cp==NULL)
   return NULL;

 // kopiujemy kolejne punkty:
 for(x=0; x<raster_dim_x(r); x++)
   for(y=0; y<raster_dim_y(r); y++)
     raster_pix_set(cp, x, y, &raster_pix_get(r, x, y) );

 return cp;
}; // raster_clone()





struct raster *raster_grayscale(struct raster *r)
{
 struct raster *r_out;	// obraz wyjsciowy
 int            x,y;	// for()
 int            tmp;	// pomocnicza zmienna

 // tworzmy nowy obraz
 r_out=raster_create( raster_dim_x(r), raster_dim_y(r) );
 if(r_out==NULL)
   return NULL;

 for(x=0; x<raster_dim_x(r); x++)
   for(y=0; y<raster_dim_y(r); y++)
   {
     tmp=( raster_pix_get_r(r, x, y)+
           raster_pix_get_g(r, x, y)+
           raster_pix_get_b(r, x, y)  )/3;	// wyliczamy srednia
     raster_pix_set_r(r_out, x, y, tmp);	// zapisujemy srednia
     raster_pix_set_g(r_out, x, y, tmp);	// ...
     raster_pix_set_b(r_out, x, y, tmp);	// ...
   }; // for([x;y])

 return r_out;
}; // raster_grayscale()






struct raster *raster_inverse_colors(struct raster *r)
{
 struct raster *r_out;	// obraz wyjsciowy
 int            x,y;	// for()

 // tworzmy nowy obraz
 r_out=raster_create( raster_dim_x(r), raster_dim_y(r) );
 if(r_out==NULL)
   return NULL;

 for(y=0; y<raster_dim_y(r); y++)
   for(x=0; x<raster_dim_x(r); x++)
   {
     int cr,cg,cb;		// pomocnicze
     cr=255-raster_pix_get_r(r, x,y);
     cg=255-raster_pix_get_g(r, x,y);
     cb=255-raster_pix_get_b(r, x,y);
     raster_pix_set_r(r_out, x,y, cr);	// zapisujemy srednia
     raster_pix_set_g(r_out, x,y, cg);	// ...
     raster_pix_set_b(r_out, x,y, cb);	// ...
   }; // for([x;y])

 return r_out;
}; // raster_inverse_colors()








struct bmp *raster_to_bmp(struct raster *r)
{
 struct bmp *p;		// nasz obrazek
 int         x,y;	// for()
 int         size_data;	// rozmiar pliq
 int         size_head;	// rozmiar naglowkow

 size_head =SO_COREH+SO_INFOH;
 // wyliczamy rozmiar pliku jako calosci:
 size_data =raster_dim_x(r)*sizeof(struct rgb24);
 size_data+=size_data%4;	// do dajemy dopelnienie do wielokrotnosci
				// 4B
 size_data*=raster_dim_y(r);	// w kazdej linii

 // przydzielamy pamiec
 p=(struct bmp*)malloc( sizeof(struct bmp) );
 if(p==NULL)
   return NULL;

 // ustawiamy core header:
 p->core_h.bfType[0]  ='B';		// wymagane
 p->core_h.bfType[1]  ='M';		// ...
 p->core_h.bfSize     =size_head+size_data;	// rozmiar calego pliku w B.
 p->core_h.bfReserved1=0;		// ...
 p->core_h.bfReserved2=0;		// ...
 p->core_h.bfOffBits  =size_head;	// offset

 // ustawiamy info header:
 p->info_h.biSize     =SO_INFOH;	// ...
 p->info_h.biWidth    =raster_dim_x(r);	// szerokosc
 p->info_h.biHeight   =raster_dim_y(r);	// wysokosc
 p->info_h.biPlanes   =1;		// ...
 p->info_h.biBitCount =24;		// RGB
 p->info_h.biCompression  =0;		// brak kompresji
 p->info_h.biSizeImage    =size_data;	// rozmiar obszaru danych w B.
 p->info_h.biXPelsPerMeter=0;		// ...
 p->info_h.biYPelsPerMeter=0;		// ...
 p->info_h.biClrUsed      =0;		// ...
 p->info_h.biClrImportant =0;		// ...

 // przydzielanie pamieci na pixele:
 p->pixels=(struct rgb24*)malloc( raster_dim_x(r)*raster_dim_y(r)*
                                  sizeof(struct rgb24)             );
 if(p->pixels==NULL)
 {
   free(p);
   return NULL;
 };

 // kopiujemy poszczegolne pixele, ale "od dolu", aby odwrocic obrazek,
 // zeby sie nie zapisal odwrotnie... :)
 for(y=0; y<raster_dim_y(r); y++)
   for(x=0; x<raster_dim_x(r); x++)
   {
     bmp_set_pixel(p, x, raster_dim_y(r)-1-y, 	// tutaj odwracamy obraz
                   &raster_pix_get(r, x, y)  );
   }; // for([x;y])

 return p;
}; // raster_to_bmp()




struct raster *raster_from_bmp(struct bmp *p)
{
 struct raster *r;	// nasz raster wyjsciowy
 int            x,y;	// for()

 // tworzymy podstawowa strukture:
 r=raster_create(p->info_h.biWidth, p->info_h.biHeight);
 if(r==NULL)
   return NULL;

 // kopiujemy poszczegolne pixele, ale "od dolu", aby odwrocic obrazek,
 // zeby sie nie zapisal odwrotnie... :)
 for(x=0; x<raster_dim_x(r); x++)
   for(y=0; y<raster_dim_y(r); y++)
   {
     raster_pix_set(r, x, y, 
                    &bmp_get_pixel(p, x, raster_dim_y(r)-1-y) );
   }; // for([x;y])

 return r;
}; // raster_from_bmp()




// makro definiujace relacje wiekszosci
#define MAX_OF(a,b)	( ((a)>(b))?(a):(b) )


struct raster *raster_substract(struct raster *a, struct raster *b)
{
 struct raster *res;		// raster wynikowy
 int            x,y;		// for()
 int            max_x,max_y;	// for() - warunek

 // oba obrazy MYSZA byc tej samej wielkosci
 if( raster_dim_x(a)!=raster_dim_x(b) ||
     raster_dim_y(a)!=raster_dim_y(b)    )
   return NULL;

 // tworzymy obraz wynikowy:
 res=raster_create( raster_dim_x(a), raster_dim_y(a) );
 if(res==NULL)
   return NULL;

 // przeliczamy pixele:
 for(x=0, max_x=raster_dim_x(a); x<max_x; x++)
   for(y=0, max_y=raster_dim_y(a); y<max_y; y++)
   {
     int tmp;			// pomocnicza
     int a_c;			// a_color
     int b_c;			// b_color

     // Red:
     a_c=raster_pix_get_r(a, x, y);
     b_c=raster_pix_get_r(b, x, y);
     tmp=(a_c-b_c);
     //tmp=(255+tmp)/2;
     tmp=(tmp<0)?0:((tmp>255)?255:tmp);
     raster_pix_set_r(res, x, y, tmp);

     // Green:
     a_c=raster_pix_get_g(a, x, y);
     b_c=raster_pix_get_g(b, x, y);
     tmp=(a_c-b_c);
     //tmp=(255+tmp)/2;
     tmp=(tmp<0)?0:((tmp>255)?255:tmp);
     raster_pix_set_g(res, x, y, tmp);

     // Blue:
     a_c=raster_pix_get_b(a, x, y);
     b_c=raster_pix_get_b(b, x, y);
     tmp=(a_c-b_c);
     //tmp=(255+tmp)/2;
     tmp=(tmp<0)?0:((tmp>255)?255:tmp);
     raster_pix_set_b(res, x, y, tmp);

   }; // for([x;y])

 return res;
}; // raster_substract()





struct raster *raster_add(struct raster *a, struct raster *b)
{
 struct raster *res;		// raster wynikowy
 int            x,y;		// for()
 int            max_x,max_y;	// for() - warunek

 // oba obrazy MYSZA byc tej samej wielkosci
 if( raster_dim_x(a)!=raster_dim_x(b) ||
     raster_dim_y(a)!=raster_dim_y(b)    )
   return NULL;

 // tworzymy obraz wynikowy:
 res=raster_create( raster_dim_x(a), raster_dim_y(a) );
 if(res==NULL)
   return NULL;

 // przeliczamy pixele:
 for(x=0, max_x=raster_dim_x(a); x<max_x; x++)
   for(y=0, max_y=raster_dim_y(a); y<max_y; y++)
   {
     int tmp;			// pomocnicza
     int a_c;			// a_color
     int b_c;			// b_color

     // Red:
     a_c=raster_pix_get_r(a, x, y);
     b_c=raster_pix_get_r(b, x, y);
     tmp=(a_c+b_c);
     tmp=(tmp<0)?0:((tmp>255)?255:tmp);
     raster_pix_set_r(res, x, y, tmp);

     // Green:
     a_c=raster_pix_get_g(a, x, y);
     b_c=raster_pix_get_g(b, x, y);
     tmp=(a_c+b_c);
     tmp=(tmp<0)?0:((tmp>255)?255:tmp);
     raster_pix_set_g(res, x, y, tmp);

     // Blue:
     a_c=raster_pix_get_b(a, x, y);
     b_c=raster_pix_get_b(b, x, y);
     tmp=(a_c+b_c);
     tmp=(tmp<0)?0:((tmp>255)?255:tmp);
     raster_pix_set_b(res, x, y, tmp);

   }; // for([x;y])

 return res;
}; // raster_add()




struct raster *raster_and(struct raster *a, struct raster *b)
{
 struct raster *res;		// raster wynikowy
 int            x,y;		// for()
 int            max_x,max_y;	// for() - warunek

 // oba obrazy MYSZA byc tej samej wielkosci
 if( raster_dim_x(a)!=raster_dim_x(b) ||
     raster_dim_y(a)!=raster_dim_y(b)    )
   return NULL;

 // tworzymy obraz wynikowy:
 res=raster_create( raster_dim_x(a), raster_dim_y(a) );
 if(res==NULL)
   return NULL;

 // przeliczamy pixele:
 for(x=0, max_x=raster_dim_x(a); x<max_x; x++)
   for(y=0, max_y=raster_dim_y(a); y<max_y; y++)
   {
     int tmp;			// pomocnicza

     // Red:
     tmp=( raster_pix_get_r(a, x, y)*raster_pix_get_r(b, x, y) )/255;
     //tmp=(tmp>127)?255:0;
     raster_pix_set_r(res, x, y, tmp);

     // Green:
     tmp=( raster_pix_get_g(a, x, y)*raster_pix_get_g(b, x, y) )/255;
     //tmp=(tmp>127)?255:0;
     raster_pix_set_g(res, x, y, tmp);

     // Blue:
     tmp=( raster_pix_get_b(a, x, y)*raster_pix_get_b(b, x, y) )/255;
     //tmp=(tmp>127)?255:0;
     raster_pix_set_b(res, x, y, tmp);

   }; // for([x;y])

 return res;
}; // raster_and()





int raster_marker_create(struct raster *r)
{
 int size;			// rozmiar tablicy markera

 if(r==NULL)
   return -1;
 if(r->marker!=NULL)		// mamy juz jakis marker?
   return -2;

 size=raster_dim_x(r)*raster_dim_y(r);	// ustalamy rozmiar

 // tworzymy pamiec na marker
 r->marker=(short*)malloc( size*sizeof(short) );
 if(r->marker==NULL)
   return -3;

 memset(r->marker, 0, size*sizeof(short) );	// ustawiamy na zero!

 return 0;
}; // raster_marker_create()



int raster_marker_free(struct raster *r)
{
 if(r==NULL)
   return -1;
 if(r->marker==NULL)		// dalej nie idziemy, ale to nie blad
   return 0;

 free(r->marker);		// zwalniamy pamiec
 r->marker=NULL;		// zaznaczamy, ze juz nie ma markera!!!!

 return 0;
}; // raster_marker_free()




