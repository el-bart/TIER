/*
 * denoise.h
 *
 * odszumiacz - usowa obszary jasne o zadanej powierzchni
 * mniejszej od zadanej. dziala na obrazach binarnych!
 *
 */
#ifndef INCLUDE_DENOISE_H_FILE
#define INCLUDE_DENOISE_H_FILE

#include "raster/raster.h"


// przerabia obrazek, zwracajac nieco "odpluskwiony"
// (pracuje na obrazach binanrych!!)
//   cnt_min - minimalnie tyle pixeli musi miec "wyprysk", aby go
//             nie wymazac.
struct raster *filter_denoise(struct raster *r_in, int cnt_min);


#endif

