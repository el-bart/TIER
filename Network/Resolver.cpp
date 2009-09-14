/*
 * Resolver.cpp
 *
 */

#include <string>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "Resolver.hpp"

using namespace std;



namespace Network
{

Address Resolver::resolve(const Address &cInfo)
{
  string       addrOut;
  const char  *addr=cInfo.getHost().c_str();

  if( strcmp(addr, "*")==0 ) // all addresses?
    addrOut="0.0.0.0";       // addr-any
  else
  {
    if( inet_addr(addr)==INADDR_NONE )         // non-IP?
    {
      // if everyting fails - try DNS:
      struct hostent *dns_info=gethostbyname(addr);
      if(dns_info==NULL || dns_info->h_addr_list[0]==NULL)   // no match?
        throw Exception("Resolver::resolve(): unknown host '" +
                        cInfo.getHost() + "'");
      // choose first from the list...
      const in_addr *inAddr=reinterpret_cast<in_addr*>(
                                    dns_info->h_addr_list[0]);
      const char    *tab   =inet_ntoa(*inAddr);
      addrOut=string(tab);
    }
    else // if(non-IP)
    {
      // so this is a correct address!
      addrOut=cInfo.getHost();
    };
  }; // else(addr=="*")

  // returned resolved host info
  Address resolved(addrOut, cInfo.getPort() );
  return resolved;
};

}; // namespace Network

