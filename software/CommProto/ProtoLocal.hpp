/*
 * ProtoLocal.hpp
 *
 * C++ wrapper class for C code executing (sending)
 * commands directly for TIER.
 *
 */
#ifndef INCLUDE_COMMPROTO_PROTOLOCAL_HPP_FILE
#define INCLUDE_COMMPROTO_PROTOLOCAL_HPP_FILE

#include <time.h>

#include "CommProto/Exception.hpp"
#include "CommProto/TIERproto.hpp"

extern "C"
{
#include "sterownik_uC/tier_io/tier_io.h"
};


namespace CommProto
{

class ProtoLocal: public TIERproto
{
public:
  // possible LPT ports:
  enum lpt
  {
    LPT0=PORTIO_LPT0,
    LPT1=PORTIO_LPT1
  };

  // lpNo   - lpt port number (as in 'lpt' enum)
  // tOutMs - timeout for seach command in [ms]
  ProtoLocal(lpt lpNo, unsigned int tOutMs=500);
  virtual ~ProtoLocal(void);

  // TIER commands:
  virtual bool init(void);
  virtual bool spdst(const Speed &spd);
  virtual Speed spdrd(void);
  virtual bool ping(void);
  virtual void stop(void);

private:
  void setPckTimeout(void);

  timeb    _timeout;    // timeout for commands
  tier_io *_io;         // I/O handler
}; // class TIERproto

}; // namespace CommProto

#endif

