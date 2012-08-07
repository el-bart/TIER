/*
 * req_col_unif.c
 *
 */
#include <stdlib.h>	// malloc(); free();
#include <string.h>	// memset();
#include <math.h>	// fabs();

#include "req_col_unif.h"


// kwadrat liczby
#define root_v(v)	( (v)*(v) )
// zwraca gradient miedzy 2 pixelami
#define grad_v(p1, p2)	( root_v(p1.r-p2.r) +\
                          root_v(p1.g-p2.g) +\
                          root_v(p1.b-p2.b)    )


// zamienia pozycje X;Y na offset w tablicy
#define	xy2off(r,x,y)	( raster_dim_x(r)*(y) + (x) )

// zaznacza, ze pixel zostal juz odwiedzony
#define set_visited(r,v,x,y)	( (v)[ xy2off(r,x,y) ]=1 )
// sprawdza czy pixel byl juz odwiedzony
#define is_visited(r,v,x,y)	( (v)[ xy2off(r,x,y) ]   )


#define MAX(a,b)	( ((a)>=(b))?(a):(b) )
#define MIN(a,b)	( ((a)< (b))?(a):(b) )

// ta procedura odpowiada za kolejne przejscia rekurencyjne, celem
// unifikacji kolorystyki obrazu
static int process_img(struct raster *r_out, struct raster *r,
                       char visited[], double d_max,
                       int x, int y, int x_prev, int y_prev,
                       struct rgb24 base)
{
 int          off_x, off_y;	// przesuniecia x,y
 int          cx, cy;		// CurrentX, CurrentY
 double       d_cur;		// bierzacy gradient!
 double       d[2];		// gradienty miedzy elementami
 double       tmp;		// pomocniczy
 struct rgb24 pix_out;		// pixel do wstawienia
 struct rgb24 pix[3];		// pixele:
				//   pix[2] - bierzacy
				//   pix[1] - poprzedni pix[2] w linii
				//   pix[0] - poprzedni pix[1] w linii

 set_visited(r, visited, x, y);	// zaznaczamy, ze tu juz bylismy!

 //
 // etap 1: przetwazanie lokalne
 //
 pix[2]=raster_pix_get(r,x,     y     );// pobieramy bierzacy pixel
 pix[1]=raster_pix_get(r,x_prev,y_prev);// oraz poprzedni
 cx=x_prev-(x-x_prev);			// wyliczamy jeszcze wczesniejsza poz.
 cy=y_prev-(y-y_prev);
 if( raster_xy_rng(r, cx, cy) )		// nalezy ona jeszcze do obrazka?
   pix[0]=raster_pix_get(r,cx,cy);	// pobieramy wiec wartosc!
 else
   pix[0]=pix[1];			// domyslnie wstawiamy poprzednia

 // analiza i ustalanie gradientu:
 d[0] =grad_v(pix[0], pix[1]);
 d[1] =grad_v(pix[1], pix[2]);
 d_cur=fabs( (1*d[0]-2*d[1])/3 );

 if(d_cur>d_max)		// koniec zejsc??
   return 0;
 // ustawiamy pixel na zadany!

 //pix_out=base;			// jakis ustalony kolorek...

 //pix_out=pix[1];		// poprzedni

/*
 tmp      =d_max-d_cur;		// wspl proporc
 pix_out.r=(int)( ((1-tmp)*base.r + tmp*pix[2].r) );
 pix_out.g=(int)( ((1-tmp)*base.g + tmp*pix[2].g) );
 pix_out.b=(int)( ((1-tmp)*base.b + tmp*pix[2].b) );
*/

/*
 pix_out.r=(1*base.r + 3*pix[2].r)/4;
 pix_out.g=(1*base.g + 3*pix[2].g)/4;
 pix_out.b=(1*base.b + 3*pix[2].b)/4;
*/

 double a,b,c,e, sum;
 c  =1;	// base
 e  =0;	// pix[0]
 a  =1;	// pix[1]
 b  =1;	// pix[2]
 sum=a+b+c+e;
 pix_out.r=(e*pix[0].r + a*pix[1].r + b*pix[2].r + c*base.r)/sum;
 pix_out.g=(e*pix[0].g + a*pix[1].g + b*pix[2].g + c*base.g)/sum;
 pix_out.b=(e*pix[0].b + a*pix[1].b + b*pix[2].b + c*base.b)/sum;
 //tmp      =fabs(c*base.r-b*pix[2].r)/sum;
 tmp      =fabs(base.r-pix[2].r);
 tmp      =1 + tmp/255.0;
 pix_out.r=MIN(255, (int)(tmp*pix_out.r) );
 pix_out.g=MIN(255, (int)(tmp*pix_out.g) );
 pix_out.b=MIN(255, (int)(tmp*pix_out.b) );
 //pix_out.r=MIN(255, pix_out.r+MAX(base.r, pix[0].r)-(base.r+pix[0].r)/2 );
 //pix_out.g=MIN(255, pix_out.g+MAX(base.g, pix[0].g)-(base.g+pix[0].g)/2 );
 //pix_out.b=MIN(255, pix_out.b+MAX(base.b, pix[0].b)-(base.b+pix[0].b)/2 );

 raster_pix_set(r_out, x, y, &pix_out);


 //
 // etap 2: przetwazanie reqrencyjne, po najblizszych sasiadach
 //
 for(off_y=-1; off_y<=1; off_y++)
   for(off_x=-1; off_x<=1; off_x++)
   {
     cx=x+off_x;
     cy=y+off_y;
     if(off_x==0 && off_y==0)		// dla siebie samego NIE wywolujemy
       continue;
     if( !raster_xy_rng(r, cx, cy) )	// indexy poza zasiegiem?
       continue;
     if( is_visited(r, visited, cx, cy) )	// pomijamy juz odwiedzione
       continue;
     process_img(r_out, r, visited,
                 d_max, cx, cy, x, y,
                 base);			// idziemy glebiej!
   }; // for(x+off,y+off)

  return 0;
};





struct raster *filter_rcu(struct raster *r, double d_max)
{
 int            x, y;		// for()
 int            dx, dy;		// dim_x, dim_y
 struct raster *r_out;		// obraz wyjsciowy
 char          *visited;	// tablica elementow juz odwiedzonych

 // wymiary obrazka:
 dx=raster_dim_x(r);
 dy=raster_dim_y(r);

 // przydzielanie pamieci:
 visited=(char*)malloc(dx*dy);	// tworzymy tablice odwiedzonych pixeli
 if(visited==NULL)
   return NULL;
 r_out=raster_clone(r);		// bedziemy pracowac na DOKLADNEJ kopii...
 if(r_out==NULL)
 {
   free(visited);
   return NULL;
 };

 // poki co nie odwiedzilismy zadnego pixela
 memset(visited, 0, dx*dy);

 // dalej bedziemy sie juz poslugiwac KWADRATAMI gradientu, zeby
 // nie pierwiastkowac za kazdym razem
 d_max*=d_max;

 //
 // przejscie po wszystkich pixelach
 //
 for(y=0; y<dy; y++)
   for(x=0; x<dx; x++)
   {
     if( is_visited(r, visited, x, y) )	// juz tu bylismy?
       continue;
     process_img(r_out, r, visited,
                 d_max, x, y, x, y,
                 raster_pix_get(r, x, y) );	// przetwazanie wlasc.
   }; // for(y;x)
 //
 // koniec przetwarzania
 //

 // sprzatamy po sobie!
 free(visited);

 return r_out;
}; // filter_rcu()



