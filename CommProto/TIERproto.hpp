/*
 * TIERproto.hpp
 *
 * C++ abstract interface for C code executing (sending)
 * commands for TIER.
 *
 */
#ifndef INCLUDE_COMMPROTO_TIERPROTO_HPP_FILE
#define INCLUDE_COMMPROTO_TIERPROTO_HPP_FILE

#include <math.h>
#include <boost/noncopyable.hpp>

#include "CommProto/Exception.hpp"


namespace CommProto
{


class TIERproto: private boost::noncopyable
{
public:
  class Speed
  {
  public:
    Speed(int left, int right):
      _left(left),
      _right(right)
    {
      if( 255<fabs(_left) || 255<fabs(_right) )
        throw CommProto::Exception("TIERproto::Speed::Speed(): "
                                   "value out of range");
    };

    inline int left(void) const
    {
      return _left;
    };
    inline int right(void) const
    {
      return _right;
    };

  private:
    int _left;
    int _right;
  };

  virtual ~TIERproto(void)
  {
  };

  // TIER commands
  virtual bool init(void) = 0;
  virtual bool spdst(const Speed &spd) = 0;
  virtual Speed spdrd(void) = 0;
  virtual bool ping(void) = 0;
  virtual void stop(void) = 0;
}; // class TIERproto

}; // namespace CommProto

#endif

