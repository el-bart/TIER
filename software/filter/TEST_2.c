/*
 * TEST_2.c
 *
 * prosty program demonstrujacy wkodowana filtracje
 * dla zadanej bitmapy wejsciowej.
 *
 */
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "raster/stats.h"
#include "filter.h"
//#include "histo_filter.h"




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





int main(int argc, char *argv[])
{
 struct raster_stats rs;	// statystyki obrazka
 struct raster *r_in;		// obraz wejsciowy
 struct raster *r_out;		// obraz przefiltrowany
 struct raster *r_org;		// obraz TUZ PO WCZYTANIU!
 struct bmp     pic;		// obraz
 struct bmp    *pic_out;	// wyjscie
 int            i;		// for()

 assert(argc==3);

 assert( bmp_load_file(&pic, argv[1])==0 );
 r_in =raster_from_bmp(&pic);	// tworzmy nowy obraz
 r_out=r_in;			// na wszelki wypadek ;)
 r_org=r_in;			// ...

 // pokazywanie statysytk obrazka:
 rs=raster_get_stats(r_in);
 fprintf(stderr, "red:   %d+-%d\n", rs.m_r.mean,  rs.m_r.std_av );
 fprintf(stderr, "green: %d+-%d\n", rs.m_g.mean,  rs.m_g.std_av );
 fprintf(stderr, "blue:  %d+-%d\n", rs.m_b.mean,  rs.m_b.std_av );
 fprintf(stderr, "avg.:  %d+-%d\n", rs.mean.mean, rs.mean.std_av);
 fprintf(stderr, "pix_max: %3d/%3d/%3d\n", 
                  rs.pix_max.r, rs.pix_max.g, rs.pix_max.b);
 fprintf(stderr, "pix_min: %3d/%3d/%3d\n",
                  rs.pix_min.r, rs.pix_min.g, rs.pix_min.b);

 //
 // filtracja metoda rekurencyjnej unifikacji kolorow.
 //
 //rs   =raster_get_stats(r_in);
 //r_out=filter_rcu(r_in,  rs.mean.std_av/2);

 //
 // filtr Laplace'a
 //
 //r_out=filter_laplace(r_in);	

// gimpowe wyniki do przerobki:
//bmp_load_file(&pic, "268_lap.bmp");
//struct raster *r_gimp=raster_from_bmp(&pic);
//r_out=raster_substract(r_in, r_gimp);

 //
 // filtr medianowy
 //
 //r_out=filter_median(r_in);

 //
 // zmiana jasnosci obrazu
 //
 //r_out=filter_ch_bri(r_in, -70);

 //
 // odciecie
 //
 //r_out=filter_cut_off(r_in, 128);
 //r_out=filter_cut_off(r_in, 70);

 //
 // dylatacja
 //
 //r_out=filter_cut_off(r_in, 128);	// praca na obrazie binanym ma sens
 //r_out=filter_dilatation(r_out);

 //
 // Frei-Chen
 //
 //r_out=filter_frei_chen(r_in, FILTER_FREI_CHEN_HORIZONTAL);
 //r_out=filter_frei_chen(r_in, FILTER_FREI_CHEN_VERTICAL);
 //r_out=raster_add( filter_frei_chen(r_in, FILTER_FREI_CHEN_VERTICAL),
 //                  filter_frei_chen(r_in, FILTER_FREI_CHEN_HORIZONTAL) );

//
// zabawy :)
//
//r_out=r_in;
//r_out=filter_laplace(r_out);
//r_out=filter_cut_off(r_out, 128);
//r_out=filter_median(r_out);


//
// detekcja krawedzi na 2 sposoby - precyzyjnie i ogolnie
//

 r_out=r_in;	// zeby nie kombinowac za duzo... i tak sie nie interesujemy
		// wyciekami pamieci w tym SMIETNIQ! ;))
 if(0)		// KRAWEDZIE *** PRECYZYJNE ***
 {
   r_out=filter_histogram_scatter(r_out);
   r_out=filter_histogram_alignment(r_out);
   r_out=raster_add( filter_frei_chen(r_in, FILTER_FREI_CHEN_VERTICAL  ),
                     filter_frei_chen(r_in, FILTER_FREI_CHEN_HORIZONTAL)  );
   r_out=filter_histogram_scatter(r_out);
   //
   // dalej CNN - EDGE
   //
 }
 else		// KRAWEDZIE *** OGOLNE ***
 {
   // rozciagamy histogram:
   r_out=filter_histogram_scatter(r_out);
   // wyrownujemy histgoram:
   //r_out=filter_histogram_alignment(r_out);

   //rs   =raster_get_stats(r_org);
   rs   =raster_get_stats(r_out);
   r_out=filter_ch_bri(r_out, 128-rs.mean.mean);

   // filtr RCU
   for(i=0; i<0; i++)
   {
     rs   =raster_get_stats(r_out);
     r_out=filter_rcu(r_out,  rs.mean.std_av/(3*i+2));
   };
   //
   // dalej CNN - PROC 3 stopniowe
   //
 };



 //
 // konczymy zabawe - pokazujemy statystyki wyjsciowego obrazu
 //
 rs=raster_get_stats(r_out);
 fprintf(stderr, "red:   %d+-%d\n", rs.m_r.mean,  rs.m_r.std_av );
 fprintf(stderr, "green: %d+-%d\n", rs.m_g.mean,  rs.m_g.std_av );
 fprintf(stderr, "blue:  %d+-%d\n", rs.m_b.mean,  rs.m_b.std_av );
 fprintf(stderr, "avg.:  %d+-%d\n", rs.mean.mean, rs.mean.std_av);
 fprintf(stderr, "pix_max: %3d/%3d/%3d\n",
                  rs.pix_max.r, rs.pix_max.g, rs.pix_max.b);
 fprintf(stderr, "pix_min: %3d/%3d/%3d\n",
                  rs.pix_min.r, rs.pix_min.g, rs.pix_min.b);

 fprintf(stderr, "---------------------\n");

 struct raster *r;
 r=r_out;
r=filter_histogram_scatter(r);
 pic_out=raster_to_bmp(r);
 assert( bmp_save_file(pic_out, argv[2], 0666)==0 );

 // koniec zabawy :)
 bmp_free(&pic);
 raster_free(r_in );
 raster_free(r_out);
 if(r!=r_in && r!=r_out)
   raster_free(r);
 bmp_free(pic_out);
 free(pic_out);

 return 0;
}; // main()


