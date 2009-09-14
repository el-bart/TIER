/*
 * V4LCapture.hpp
 *
 * Class for capturing static images (frames) from
 * Video4Linux devices (/dev/video, etc...)
 *
 */
#ifndef INCLUDE_GRAPHICS_V4LCAPTURE_HPP_FILE
#define INCLUDE_GRAPHICS_V4LCAPTURE_HPP_FILE

#include <string>
#include <memory>
#include <boost/noncopyable.hpp>

#include "Graphics/Size.hpp"
#include "Graphics/Raster.hpp"



namespace Graphics
{

//
// class for capturing of still images from V4L device.
//
class V4LCapture: public boost::noncopyable
{
public:
  //
  // Initializes capturing interface
  //   dev  - device to capture from
  //   size - size of image to be captured
  //
  V4LCapture(const std::string &dev, const Graphics::Size &size);

  ~V4LCapture();

  //
  // captures single image, or throw excaption if
  // timeouted.
  // may also write data directly to a given memory place
  // (assuming the picture sizes MATCH).
  //   timeout - given in [s] (0 for infinit wait)
  //
  Graphics::RasterRGB::AutoRaster capture(int timeout=2);
  void capture(Graphics::RasterRGB &raster, int timeout=2);

  const Graphics::Size& getSize(void) const;
  const std::string& getDeviceName(void) const;

private:
  void initDevice(const Graphics::Size &size);
  void uninitDevice(void);

  int callIoctl(int call, void *param);

  Graphics::Size    *_size;
  const std::string  _dev;
  int                _fd;
  unsigned char     *_buf;
  int                _bufSize;
};


}; // namespace Graphics

#endif

