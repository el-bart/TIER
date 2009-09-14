/*
 * req_color_unif.h
 *
 * Requrencial Color Unifier
 *
 */
#ifndef INCLUDE_REQ_COLOR_UNIF_H_FILE
#define INCLUDE_REQ_COLOR_UNIF_H_FILE

#include "raster/raster.h"


// filtracja metoda rekurencyjnej unifikacji kolorow.
//
//   d_max - maksymalny gradient, na jaki mozemy pozwolic
//           zaznaczajac obszar jako ciagly.
//
struct raster *filter_rcu(struct raster *r, double d_max);


#endif

