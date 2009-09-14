/*
 * GlobalMap.cpp
 *
 */
#include <math.h>

#include "GlobalMap.hpp"


using namespace std;
using namespace Maps;
using namespace Vectors;


namespace Path
{

void GlobalMap::actualize(const Map2D &lMap)
{
  _map.merge(lMap);  // merge current map and transformed one
  _map.updateTTL();
};

}; // namespace Path

