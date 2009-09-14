/*
 * Raster.hpp
 *
 */
#ifndef INCLUDE_GRAPHICS_RASTER_HPP_FILE
#define INCLUDE_GRAPHICS_RASTER_HPP_FILE

#include <memory>
#include <vector>
#include <boost/noncopyable.hpp>
#include <assert.h>

#include "Graphics/Exception.hpp"
#include "Graphics/Pixel.hpp"
#include "Graphics/Size.hpp"
#include "Graphics/Position.hpp"


namespace Graphics
{


//
// class representing an image.
//
// note: after initialization raster has random content
//
template<class TPix>
class Raster: private boost::noncopyable
{
public:
  // fast coping of Raster via pointer
  typedef std::auto_ptr<Raster> AutoRaster;
  // soemtimes might be needed
  typedef TPix PixelType;

  Raster(const Graphics::Size &size):
                _size(size),
                _r( _size.y() )
  {
    for(unsigned int y=0; y<_size.y(); ++y)
      _r[y].resize( _size.x() );
  };
  // we make copy as FULL copy of internal data
  Raster(const Raster &raster):
                _size(raster._size),
                _r( _size.y() )
  {
    for(unsigned int y=0; y<_size.y(); ++y)
    {
      _r[y].resize( _size.x() );

      for(unsigned int x=0; x<_size.x(); ++x)
        _r[y][x]=raster._r[y][x];
    };

    //throw Graphics::Exception("N/I");
  };

  inline const Graphics::Size &getSize(void) const
  {
    return _size;
  };

  inline bool inRange(const Graphics::Position &pos) const
  {
    return _size.inRange(pos);
  };

  // return non-const pixel by value
  inline TPix getPixelVal(const Graphics::Position &pos) const
  {
    assert( inRange(pos) );
    return _r[pos.y()][pos.x()];
  };
  inline TPix getPixelValSec(const Graphics::Position &pos) const
  {
    if( !inRange(pos) )
      throw Graphics::Exception("Raster: unable to get pixel out of range");
    return getPixelVal(pos);
  };

  // return reference to const pixel
  inline const TPix &getPixel(const Graphics::Position &pos) const
  {
    assert( inRange(pos) );
    return _r[pos.y()][pos.x()];
  };
  inline const TPix &getPixelSec(const Graphics::Position &pos) const
  {
    if( !inRange(pos) )
      throw Graphics::Exception("Raster: unable to get pixel out of range");
    return getPixel(pos);
  };

  // set new value for pixel
  inline void setPixel(const Graphics::Position &pos, const TPix &pix)
  {
    assert( inRange(pos) );
    _r[pos.y()][pos.x()]=pix;
  };
  inline void setPixelSec(const Graphics::Position &pos, const TPix &pix)
  {
    if( !inRange(pos) )
      throw Graphics::Exception("Raster: unable to set pixel out of range");
    setPixel(pos, pix);
  };

  inline void fillRaster(const TPix &pix)
  {
    for(typename Col::iterator itY=_r.begin(); itY!=_r.end(); ++itY)
    {
      Row &row=*itY;    // make code more readable
      for(typename Row::iterator itX=row.begin(); itX!=row.end(); ++itX)
        *itX=pix;       // write new value of pixel
    };
  };

private:
  Raster();
  Raster operator=(const Raster &raster);   // not used because of performance

  typedef std::vector<TPix> Row;
  typedef std::vector<Row>  Col;

  const Graphics::Size _size;     // for better performance
  Col                  _r;        // internal representation for raster [y;x]
}; // class Raster



//
// here are some commonly used definitions
//
typedef Graphics::Raster<Graphics::PixelRGB>  RasterRGB;
typedef Graphics::Raster<Graphics::PixelGray> RasterGray;


}; // namespace Graphics


#endif

