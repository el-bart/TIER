/*
 * Raster.cpp
 *
 */
#include <map>
#include <stdlib.h>
#include <assert.h>

#include "CGraphics/Exception.hpp"
#include "Raster.hpp"

extern "C"
{
#include "bitmap/bitmap.h"
#include "raster/raster.h"
#include "filter/filter.h"
}


using namespace std;



namespace CGraphics
{


//
// wrapper class itself
//

Raster::Raster(const Size &size)
{
  _r=raster_create(size._x, size._y);

  if(_r==NULL)
    throw Exception("raster_create(): allocation error");
};


Raster::~Raster()
{
  raster_free(_r);
};


Raster::Raster(const Raster &raster)
{
  _r=raster_clone(raster._r);
  if(_r==NULL)
    throw Exception("Copy-Operator: unable to copy raster");
};


/*
Raster Raster::operator=(const Raster &raster)
{
  if(_r!=NULL)
    raster_free(_r);
  _r=raster_clone(raster._r);
  if(_r==NULL)
    throw Exception("Copy-Operator: unable to copy raster");
  return *this;
};
*/


Size Raster::getSize(void) const
{
  //Size size( raster_dim_x(_r), raster_dim_y(_r) );
  return Size( raster_dim_x(_r), raster_dim_y(_r) );
};


void Raster::fillBlack(void)
{
  raster_black(_r);
};


Pixel Raster::getPixel(const Position &pos) const
{
  if( !inRange(pos) )
    throw Exception("Raster::getPixel(): pixel position out of range");

  rgb24 pix=raster_pix_get(_r, pos._x,pos._y );
  return Pixel(pix.r, pix.g, pix.b);
};


void Raster::setPixel(const Position &pos, const Pixel& p)
{
  if( !inRange(pos) )
    throw Exception("pixel position out of range");

  raster_pix_set_r(_r, pos._x,pos._y, p.getR() );
  raster_pix_set_g(_r, pos._x,pos._y, p.getG() );
  raster_pix_set_b(_r, pos._x,pos._y, p.getB() );
};


Raster::AutoRaster Raster::fromFile(const char path[])
{
  assert(path!=NULL);

  bmp file;
  if( bmp_load_file(&file, path)!=0 )
    throw Exception("bitmap: unable to load file");

  Raster::AutoRaster ar( new Raster );      // little performance hack
  ar.get()->_r=raster_from_bmp(&file);      // convert!
  bmp_free(&file);
  if( ar.get()->_r==NULL )
    throw Exception("raster: unable to convert bmp to raster");

  return ar;
};


void Raster::toFile(const char path[])
{
  assert(path!=NULL);

  bmp *file;
  file=raster_to_bmp(_r);
  if(file==NULL)
    throw Exception("raster: unable to convert to bmp");

  int ret=bmp_save_file(file, path, 0600);
  bmp_free(file);
  free(file);           // workaround for bmp... :/
  if(ret!=0)
    throw Exception("bmp: unable to write file");
};


Raster::AutoRaster Raster::crop(const Position &from, const Position &to) const
{
  unsigned int dimX=to._x-from._x+1;
  unsigned int dimY=to._y-from._y+1;

  if( !inRange(from) || !inRange(to) ) // size not ok?
    throw Exception("Raster::crop(): bad crop size/limit");

  // make destination image
  Size size(dimX, dimY);
  Raster::AutoRaster ar( new Raster(size) );

  // rewrite image
  for(unsigned int y=from._y; y<=to._y; ++y)
    for(unsigned int x=from._x; x<=to._x; ++x)
    {
      unsigned int newX=x-from._x;
      unsigned int newY=y-from._y;
      Position     posOld(x,y);
      Position     posNew(newX,newY);
      // rewrite data
      ar->setPixel(posNew, getPixel(posOld) );
    };

  return ar;
};


Raster::AutoRaster Raster::resize(const Size &newSize) const
{
  struct raster *r=raster_scale_simple(_r, newSize._x, newSize._y);
  if(r==NULL)
    throw Exception("Raster::resize(): unable to resize raster");
  Raster::AutoRaster ar( new Raster(r) );
  return ar;
};


void Raster::scatterHistogram(void)
{
  raster *tmp=filter_histogram_scatter(_r);
  raster_free(_r);
  _r=tmp;
};


void Raster::createMarker(void)
{
  if( hasMarker() )     // skip multiple calls
    return;
  if( raster_marker_create(_r)!=0 )
    throw Exception("raster_marker: unable to create marker");
};


void Raster::freeMarker(void)
{
  if( !hasMarker() )    // skip if called multiple times
    return;
  raster_marker_free(_r);
};


void Raster::resetMarker(void)
{
  freeMarker();
  createMarker();
};


bool Raster::hasMarker(void) const
{
  return _r->marker!=NULL;
};


Raster::AutoRaster Raster::markerToColormap(void) const
{
  Raster::AutoRaster r( new Raster( getSize() ) );
  map<int, Pixel> colorMap;

  Size size=getSize();
  for(unsigned int x=0; x<size._x; ++x)
    for(unsigned int y=0; y<size._y; ++y)
    {
      Position pos(x,y);
      int      num=getMarker(pos);
      if( colorMap.find(num)==colorMap.end() )  // no such color yet?
        colorMap[num]=Pixel( rand(), rand(), rand() );
      r->setPixel(pos, colorMap[num]);          // insert pixel
    };

  return r;
};


void Raster::setMarker(const Position &pos, short int v)
{
  raster_marker_set(_r, pos._x,pos._y, v);
};


short int Raster::getMarker(const Position &pos) const
{
  return raster_marker_get(_r, pos._x,pos._y);
};


Raster::Raster(void):
  _r(NULL)  // NOT initialized!!
{
};


Raster::Raster(struct raster *r):   // creates raster from pointer
  _r(r)                             // and takes its ownership
{
  assert(_r!=NULL);
};

}; // namespace CGraphics

