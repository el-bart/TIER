/*
 * scale_raster.h
 *
 * funkcje odpowiadajace za skalowanie obrazow.
 *
 */
#ifndef INCLUDE_SCALE_RASTER_H_FILE
#define INCLUDE_SCALE_RASTER_H_FILE

#include "main_raster.h"


// zwykle skalowanie jednopunktowe - odpowiednim
// pixelom obrazu wYjsciowego przypisuje odpowiadajace
// punkty obrazu wEjsciowego.
struct raster *raster_scale_simple(struct raster *r, int size_x, int size_y);


#endif

