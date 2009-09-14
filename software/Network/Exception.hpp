/*
 * Exception.hpp
 *
 * everything within this namespace should use this.
 *
 */
#ifndef INCLUDE_NETWORK_EXCEPTION_HPP_FILE
#define INCLUDE_NETWORK_EXCEPTION_HPP_FILE

#include <string>


namespace Network
{


class Exception
{
public:
  Exception(const std::string& msg):
                _msg(msg)
  {
  };

  Exception(const char msg[]):
                _msg(msg)
  {
  };

  const std::string& what(void) const
  {
    return _msg;
  };

private:
  const std::string _msg;
};


}; // namespace Network


#endif

