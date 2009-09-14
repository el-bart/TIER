/*
 * RasterSwapper.hpp
 *
 * class sharing mutex for swapping 'raster' pointers
 * for drawing images.
 *
 */
#ifndef INCLUDE_DEBUGWIN_RASTERSWAPPER_HPP_FILE
#define INCLUDE_DEBUGWIN_RASTERSWAPPER_HPP_FILE

#include "Threads/Mutex.hpp"
#include "Threads/Lock.hpp"

extern "C"
{
#include "raster/raster.h"
}


namespace DebugWin
{

class RasterSwapper
{
public:
  RasterSwapper(Threads::Mutex  &mutex,
                struct raster  **r_in,
                struct raster  **r_scaled,
                bool            *imgChanged):
    _mutex(mutex),
    _r_in(r_in),
    _r_scaled(r_scaled),
    _imgChanged(imgChanged)
  {
  };

  // method TAKES OVNERSHIP OF POINTER!
  void swap(struct raster *r)
  {
    Threads::Lock lock(_mutex); // critical section

    if(*_r_scaled!=NULL)        // free old scaled image?
    {
      raster_free(*_r_scaled);
      *_r_scaled=NULL;
    };

    if(*_r_in!=NULL)            // free old original image?
      raster_free(*_r_in);

    *_r_in      =r;             // assign new one!
    *_imgChanged=true;          // signal this to program
  };

private:
  Threads::Mutex  &_mutex;      // mutex for synchronization
  struct raster  **_r_in;       // data beeing synchronize (to swap)
  struct raster  **_r_scaled;   // data beeing synchronize (to swap)
  bool            *_imgChanged; // determines if if images has changed
};

}; // namespace DebugWin

#endif

