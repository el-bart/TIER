/*
 * histogram.h
 *
 * modol odpowiadajacy za operacja na histogramie.
 *
 */
#ifndef INCLUDE_HISTOGRAM_H_FILE
#define INCLUDE_HISTOGRAM_H_FILE

#include "raster.h"


// struktura przechowujaca histrogram
struct histogram
{
 unsigned int col[256];	// dla kazdego koloru czestosc wystepowania
}; // struct histogram




// wybor skladowej, po ktorej bedziemy liczyc histogram
#define HISTOGRAM_RED		1
#define HISTOGRAM_GREEN		2
#define HISTOGRAM_BLUE		3
#define HISTOGRAM_DEFAULT	HISTOGRAM_GREEN



// funkcja oblicza i zwraca histrogram zadanego obrazka
struct histogram raster_histogram_get(struct raster *r, int hist_type);

// zwraca dystrybuante (calke) z zadanego histogramu
struct histogram raster_histogram_integrate(struct histogram h);

// makro zwracajace wartosc histogramu dla zadanego koloru
#define raster_histogram_color_get(h, color)	( (h).col[(color)] )
// makro ustawiajace wartosc histogramu dla zadanego koloru
#define raster_histogram_color_set(h, color, v)	( (h).col[(color)]=(v) )

// funkcja wygladza histogram
struct histogram raster_histogram_smooth(struct histogram h);

// funkcja zwraca punkt progowy histogramu [0-255]
int raster_histogram_threshold_get(struct histogram h);



#endif

