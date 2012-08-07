/*
 * Map2D.cpp
 *
 */

#include "Map2D.hpp"

using namespace std;
using namespace Vectors;

namespace Maps
{


Map2D::Map2D(const VectorImg &vImg)
{
  // simple 1:1 copy
  for(unsigned int i=0; i<vImg.linesCnt(); ++i)
    _space.add(vImg[i]);    // SpaceObj is build automaticly
};


void Map2D::merge(const Map2D &map)
{
  // merge two maps by adding elements from 'map' to current one
  const Map2D::Space_t &inputSpace=map.getSpace();      // helper
  for(Space_t::const_iterator it=inputSpace.begin();
      it!=inputSpace.end();
      ++it)
    _space.add(*it);        // add element to our space
};


void Map2D::transform(const RefSpace2D &refSpace)
{
  // make transformation line by line...
  for(Space_t::iterator it=_space.begin(); it!=_space.end(); ++it)
  {
    SpaceObj &objIn=*it;
    objIn=refSpace.transform( objIn.getLine() );
  };
};


void Map2D::updateTTL(void)
{
  // go throught whole space!
  Space_t::iterator it=_space.begin();
  while( it!=_space.end() )
  {
    (*it).decTTL();                 // update TTL of a given object
    if( (*it).getTTL()==0 )         // TTL is over? :)
    {
      Space_t::iterator itTmp=it;   // make a copy of iterator
      ++it;                         // move main iteraor to next object
      _space.remove(itTmp);         // remove out-of-date object
    }
    else
      ++it;
  };
};

}; // namespace Maps

