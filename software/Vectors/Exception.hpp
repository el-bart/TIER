/*
 * Exception.hpp
 *
 * everything within this namespace should use this.
 *
 */
#ifndef INCLUDE_VECTORS_EXCEPTION_HPP_FILE
#define INCLUDE_VECTORS_EXCEPTION_HPP_FILE

#include <string>


namespace Vectors
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


}; // namespace Vectors


#endif

