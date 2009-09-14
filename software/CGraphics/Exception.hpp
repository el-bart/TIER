/*
 * Exception.hpp
 *
 * everything within this namespace should use this.
 *
 */
#ifndef INCLUDE_CGRAPHICS_EXCEPTION_HPP_FILE
#define INCLUDE_CGRAPHICS_EXCEPTION_HPP_FILE

#include <string>


namespace CGraphics
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
  std::string _msg;
};


}; // namespace CGraphics


#endif

