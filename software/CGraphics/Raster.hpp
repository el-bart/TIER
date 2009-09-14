/*
 * Raster.hpp
 *
 */
#ifndef INCLUDE_CGRAPHICS_RASTER_HPP_FILE
#define INCLUDE_CGRAPHICS_RASTER_HPP_FILE

//#include <boost/noncopyable.hpp>
#include <memory>
#include <assert.h>

#include "CGraphics/Pixel.hpp"
#include "CGraphics/Size.hpp"
#include "CGraphics/Position.hpp"
#include "CGraphics/Exception.hpp"


// forward declaration of C-struct
extern "C"
{
struct raster;
};


namespace CGraphics
{


//
// class representing an image.
//
// note: after initialization raster has random content
//
class Raster//: boost::noncopyable
{
public:
  // fast "coping" of Raster
  typedef std::auto_ptr<CGraphics::Raster> AutoRaster;

  // x,y - picture size in pixels
  Raster(const CGraphics::Size &size);
  ~Raster();
  // we make copy as FULL copy of internal data
  Raster(const Raster &raster);

  CGraphics::Size getSize(void) const;

  void fillBlack(void);     // fill raster with black color

  inline bool inRange(const CGraphics::Position &pos) const   // check if [x;y] is still in image
  {
    return getSize().inRange(pos);
  };
  CGraphics::Pixel getPixel(const CGraphics::Position &pos) const;
  void setPixel(const CGraphics::Position &pos, const CGraphics::Pixel& p);

  static AutoRaster fromFile(const char path[]);
  void toFile(const char path[]);

  //
  // returns cropped image, from oryginal by selecting
  // points from square (fromX,fromY) to (toX, toY), both
  // inclusively.
  //
  Raster::AutoRaster crop(const CGraphics::Position &from,
                          const CGraphics::Position &to) const;

  // returns scaled image
  Raster::AutoRaster resize(const CGraphics::Size &newSize) const;

  //
  // processing methods
  //
  // note: all of these methods removes marker, unless
  //       otherwise specified.
  //
  void scatterHistogram(void);

  //
  // marker functionality
  //
  void createMarker(void);
  void freeMarker(void);
  void resetMarker(void);
  bool hasMarker(void) const;
  AutoRaster markerToColormap(void) const;  // each marker no. have it's color

  void setMarker(const CGraphics::Position &pos, short int v);
  short int getMarker(const CGraphics::Position &pos) const;

private:
  Raster(void);                             // performance hack...
  Raster(struct raster *r);                 // creates raster from pointer
  Raster operator=(const Raster &raster);   // not used because of performance

  struct raster *_r;
}; // class Raster



}; // namespace CGraphics


#endif

