/*
 * ProtoLocal.cpp
 *
 */

#include "ProtoLocal.hpp"


namespace CommProto
{

ProtoLocal::ProtoLocal(ProtoLocal::lpt lpNo, unsigned int tOutMs)
{
  // set timeout
  _timeout.time   =tOutMs/1000;
  _timeout.millitm=tOutMs%1000;

  // init communication structure
  _io=tier_io_create(lpNo);
  if(_io==NULL)
    throw CommProto::Exception("ProtoLocal::ProtoLocal(): "
                               "tier_io_create() failed "
                               "(are you root?)");
};


ProtoLocal::~ProtoLocal(void)
{
  tier_io_free(_io);
};


bool ProtoLocal::init(void)
{
  setPckTimeout();
  return tier_io_init(_io)==0;
};


bool ProtoLocal::spdst(const ProtoLocal::Speed &spd)
{
  if( spd.left()==0 && spd.right()==0 )     // STOP? ;)
  {
    stop();
    return true;
  };

  setPckTimeout();
  return tier_io_spdst(_io, spd.left(), spd.right() )==0;
};


ProtoLocal::Speed ProtoLocal::spdrd(void)
{
  int left;
  int right;

  setPckTimeout();
  if( tier_io_spdrd(_io, &left, &right)!=0 )
    throw Exception("ProtoLocal::spdrd(): tier_io_spdrd() failed");

  return Speed(left, right);
};


bool ProtoLocal::ping(void)
{
  setPckTimeout();
  return tier_io_ping(_io)==0;
};


void ProtoLocal::stop(void)
{
  setPckTimeout();
  tier_io_stop(_io);
};


void ProtoLocal::setPckTimeout(void)
{
  _io->h.timeout=_timeout;
};

}; // namespace CommProto

