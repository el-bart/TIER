/*
 * stats.h
 *
 * praca nad statystykami danego rastra.
 *
 */
#ifndef INCLUDE_STATS_H_FILE
#define INCLUDE_STATS_H_FILE

#include "raster.h"


// wartosci srednie i odchylenia stnadardowe
struct mean_std_av
{
 unsigned char mean;	// srednia
 int           std_av;	// odchylenie standardowe
}; // struct mean_std_av


// struktura przechowujaca statystiki
struct raster_stats
{
 // wartosci srednie:
 struct mean_std_av m_r;	// Red
 struct mean_std_av m_g;	// Green
 struct mean_std_av m_b;	// Blue
 struct mean_std_av mean;	// srednia wartosc z w/w

 // wartosc skrajne:
 struct rgb24       pix_min;	// najciemniejszy pixel
 struct rgb24       pix_max;	// najjasniejszy piexel
}; // struct raster_stats






// zwraca statystyki rastra
struct raster_stats raster_get_stats(struct raster *r);


#endif

