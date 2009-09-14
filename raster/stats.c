/*
 * stats.c
 *
 */
#include <string.h>	// memset();
#include <math.h>	// sqrt(); fabs();

#include "stats.h"


// zwraca sume wartosci R/G/B pixel'a
#define PIXEL_SUM(p)	( (p).r + (p).g + (p).b )
// zwraca prawde jesli p1>p2 (GreaterThen)
#define PIXEL_GT(p1, p2)	( PIXEL_SUM(p1)-PIXEL_SUM(p2) > 0 )
// zwraca prawde jesli p1<p2 (LessThen)
#define PIXEL_LT(p1, p2)	( PIXEL_SUM(p1)-PIXEL_SUM(p2) < 0 )




struct raster_stats raster_get_stats(struct raster *r)
{
 struct raster_stats s;		// nasze statystyki
 int                 x,y;	// for()
 struct rgb24        pix;
 double              m_r,  m_g,  m_b,  m_gs;	// mean  [RGB GreyScale]
 double              sa_r, sa_g, sa_b, sa_gs;	// std_av[RGB GreyScale]
 double              base;	// ilosc pixeli
 double              tmp;	// pomocznicza

 memset(&s, 0, sizeof(s) );	// na poczatek same zera!

 // zeby cos sie wogole wydazylo:
 s.pix_min.r=0xff;
 s.pix_min.g=0xff;
 s.pix_min.b=0xff;

 //
 // wyliczamy wartosci srednie i eksterma
 //
 base=raster_dim_y(r)*raster_dim_x(r);	// lacznie tyle jest pixeli
 m_r =0;
 m_g =0;
 m_b =0;
 m_gs=0;
 for(y=0; y<raster_dim_y(r); y++)
   for(x=0; x<raster_dim_x(r); x++)
   {
     pix=raster_pix_get(r, x, y);	// pobieramy go

     // najpierw eksterma
     if( PIXEL_LT(pix, s.pix_min) )	// mamy minimum?
       s.pix_min=pix;
     else
       if( PIXEL_GT(pix, s.pix_max) )	// mmay maximum?
         s.pix_max=pix;

     // wyliczamy srednie
     m_r +=pix.r/base;
     m_g +=pix.g/base;
     m_b +=pix.b/base;
     m_gs+=(PIXEL_SUM(pix)/3.0)/base;
   };

 // wstawiamy statystyki na miejsce
 s.m_r.mean =(unsigned char)m_r;
 s.m_g.mean =(unsigned char)m_g;
 s.m_b.mean =(unsigned char)m_b;
 s.mean.mean=(unsigned char)m_gs;

 //
 // wyliczamy odchylenia standardowe
 //
 sa_r =0;
 sa_g =0;
 sa_b =0;
 sa_gs=0;
 for(y=0; y<raster_dim_y(r); y++)
   for(x=0; x<raster_dim_x(r); x++)
   {
     pix=raster_pix_get(r, x, y);	// pobieramy go

     // liczymy odpowiednia sume:
     // Red:
     tmp  =(s.m_r.mean-pix.r);		// roznica
     sa_r+=tmp*tmp;			// jej kwadrat

     // Green:
     tmp  =(s.m_g.mean-pix.g);		// roznica
     sa_g+=tmp*tmp;			// jej kwadrat

     // Blue:
     tmp  =(s.m_b.mean-pix.b);		// roznica
     sa_b+=tmp*tmp;			// jej kwadrat

     // Grey Scale:
     tmp   =(s.mean.mean-PIXEL_SUM(pix)/3.0);	// w odcieniach szarosci
     sa_gs+=tmp*tmp;
   };

 // koncowe obliczenia:
 sa_r =sqrt( fabs( sa_r /(base-1) ) );
 sa_g =sqrt( fabs( sa_g /(base-1) ) );
 sa_b =sqrt( fabs( sa_b /(base-1) ) );
 sa_gs=sqrt( fabs( sa_gs/(base-1) ) );

 // wstawiamy wyniki na miejsce:
 s.m_r.std_av =(int)sa_r;
 s.m_g.std_av =(int)sa_g;
 s.m_b.std_av =(int)sa_b;
 s.mean.std_av=(int)sa_gs;

 // na koniec wartosc wypadkowa
 //s.mean.mean  =(s.m_r.mean   + s.m_g.mean   + s.m_b.mean  )/3;
 //s.mean.std_av=(s.m_r.std_av + s.m_g.std_av + s.m_b.std_av)/3;

 return s;
}; // raster_get_stats()


