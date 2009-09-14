/*
 * TEST.cpp
 *
 */
#include <iostream>

#include "Destination.hpp"
#include "GlobalMap.hpp"
#include "Plan.hpp"
#include "Planner.hpp"
#include "Exception.hpp"


using namespace std;
using namespace Path;


int main(void)
{
  GlobalMap gm;

  cout<<gm.getDirOYangle()<<endl;

  return 0;
}; // main()

