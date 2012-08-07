/*
 * histo_filter.h
 *
 * operacje na obrazie, na podstawie jego histogramu.
 *
 */
#ifndef INCLUDE_HISTO_FILTER_H_FILE
#define INCLUDE_HISTO_FILTER_H_FILE

#include "raster/raster.h"
#include "raster/histogram.h"


// rozciaga histogram obrazu
struct raster *filter_histogram_scatter(struct raster *r);

// wyrownuje histogram obazu
struct raster *filter_histogram_alignment(struct raster *r);


#endif

