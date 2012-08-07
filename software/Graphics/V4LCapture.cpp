/*
 * V4LCapture.cpp
 *
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
//#include <fcntl.h>
//#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <string.h>

#include "V4LCapture.hpp"


using namespace std;
using namespace Graphics;


namespace Graphics
{


namespace
{

struct rgb
{
  unsigned char _r;
  unsigned char _g;
  unsigned char _b;
} __attribute__((packed));


}; // unnamed namespace



V4LCapture::V4LCapture(const string &dev, const Size &size):
                _size(NULL),
                _dev(dev),
                _fd(-1),
                _buf(NULL),
                _bufSize(-1)
{
  initDevice(size);
};


V4LCapture::~V4LCapture()
{
  uninitDevice();
};


RasterRGB::AutoRaster V4LCapture::capture(int timeout)
{
  assert(timeout>=0);

  RasterRGB::AutoRaster raster( new RasterRGB(*_size) );
  capture(*raster.get(), timeout);
  return raster;
};


void V4LCapture::capture(RasterRGB &raster, int timeout)
{
  if(timeout<0)
    throw Exception("V4LCapture::capture(): timeout must be positive int");
  if(raster.getSize()<*_size)
    throw Exception("V4LCapture::capture(): "
                    "output raster is smaller than needed");

  // try reading data
  struct timeval  tv    ={timeout, 0};
  struct timeval *tv2use=(timeout==0)?NULL:&tv;
  for(;;)
  {
    fd_set          fds;
    int             r;

    // wait on select for data
    FD_ZERO(&fds);
    FD_SET(_fd, &fds);
    r=select(_fd+1, &fds, NULL, NULL, tv2use);

    // error?
    if(r==-1)
    {
      if(errno==EINTR)
        continue;
      else
        throw Exception("V4LCapture::capture(): select() failed");
    };

    // timeout?
    if(r==0)
      throw Exception("V4LCapture::capture(): timeout");

    // has data? read data!
    int ret; 
    if( (ret=read(_fd, _buf, _bufSize))!=_bufSize )
    {
      if(ret==-1 && errno==EAGAIN)  // shit happens... ;)
        continue;
      throw Exception("V4LCapture::capture(): read() error");
    }
    else
      break;
  }; // for(;;)

  // copy data to output buffer
  const struct rgb   *ptr=reinterpret_cast<const struct rgb*>(_buf);
  for(unsigned int y=0; y<_size->y(); ++y)
    for(unsigned int x=0; x<_size->x(); ++x)
    {
      const struct rgb &tmp=*(ptr + y*_size->x()+x);
      PixelRGB pix(tmp._r, tmp._g, tmp._b);
      Position pos(x, y);
      raster.setPixel(pos, pix);
    };
};


const Size& V4LCapture::getSize(void) const
{
  return *_size;
};


const string& V4LCapture::getDeviceName(void) const
{
  return _dev;
};


void V4LCapture::initDevice(const Size &size)
{
  try
  {
    // open device file
    _fd=open(_dev.c_str(), O_RDWR|O_NONBLOCK);
    if(_fd==-1)
      throw Exception("V4LCapture::openDevice(): open() failed");

    // check device
    struct v4l2_capability cap;
    if( callIoctl(VIDIOC_QUERYCAP, &cap)==-1 )
    {
      if(errno==EINVAL)
        throw Exception("V4LCapture::initDevice(): not a V4L2 device");
      else
        throw Exception("V4LCapture::initDevice(): "
                        "ioctl(VIDIOC_QUERYCAP) failed");
    };
    if(!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE))
      throw Exception("V4LCapture::initDevice(): not a video capture device");
    if(!(cap.capabilities & V4L2_CAP_READWRITE))
      throw Exception("V4LCapture::initDevice(): device does not support I/O");

    // set cropping
    struct v4l2_cropcap cropcap;
    struct v4l2_crop    crop;
    memset(&cropcap, 0, sizeof(cropcap) );
    cropcap.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if( callIoctl(VIDIOC_CROPCAP, &cropcap)==0 )
    {
      crop.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
      crop.c=cropcap.defrect;     // reset to default
      if( callIoctl(VIDIOC_S_CROP, &crop)==-1 )
      {
        // Cropping not supported.
      };
    };
    // Errors ignored!

    // picture format:
    struct v4l2_format fmt;
    memset(&fmt, 0, sizeof(fmt) );
    fmt.type               =V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width      =size.x();
    fmt.fmt.pix.height     =size.y();
    fmt.fmt.pix.pixelformat=V4L2_PIX_FMT_RGB24;
    fmt.fmt.pix.field      =V4L2_FIELD_INTERLACED;

    if( callIoctl(VIDIOC_S_FMT, &fmt)==-1 )
      throw Exception("V4LCapture::initDevice(): ioctl(VIDIOC_S_FMT) failed");
    // Note VIDIOC_S_FMT may change width and height.
    _size=new Size(fmt.fmt.pix.width, fmt.fmt.pix.height);

    // Buggy driver paranoia.
    unsigned int min;
    min=fmt.fmt.pix.width*(1+1+1);
    if(fmt.fmt.pix.bytesperline<min)
      fmt.fmt.pix.bytesperline=min;

    // Buggy driver paranoia.
    min=fmt.fmt.pix.bytesperline*fmt.fmt.pix.height;
    if(fmt.fmt.pix.sizeimage<min)
      fmt.fmt.pix.sizeimage=min;

    // pixel buffor size
    _bufSize=fmt.fmt.pix.sizeimage;
    _buf    =new unsigned char[_bufSize];
    if(_buf==NULL)
      throw Exception("V4LCapture::initDevice(): not enought memory");
  }
  catch(...)
  {
    uninitDevice();     // free resources
    throw;              // re-throw
  };
};


void V4LCapture::uninitDevice(void)
{
  close(_fd);

  delete    _size;
  delete [] _buf;

  _fd     =-1;
  _size   =NULL;
  _buf    =NULL;
  _bufSize=-1;
};


int V4LCapture::callIoctl(int call, void *param)
{
  int ret;
  do
  {
    ret=ioctl(_fd, call, param);
  }
  while(ret==-1 && errno==EINTR);

  return ret;
};


}; // namespace Graphics

