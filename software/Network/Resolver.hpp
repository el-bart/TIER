/*
 * Resolver.hpp
 *
 * helper class for resolving hosts via DNS.
 *
 */
#ifndef INCLUDE_NETWORK_RESOLVER_HPP_FILE
#define INCLUDE_NETWORK_RESOLVER_HPP_FILE

#include "Network/Address.hpp"
#include "Network/Exception.hpp"



namespace Network
{

class Resolver
{
public:
  static Network::Address resolve(
            const Network::Address &cInfo);

private:
  Resolver(void);   // no instances needed
};

}; // namespace Network

#endif

