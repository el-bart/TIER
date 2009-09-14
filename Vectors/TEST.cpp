/*
 * TEST.cpp
 *
 * some simple test executable (playground).
 *
 */
#include <iostream>
#include <math.h>
#include <assert.h>

#include "Vectors/Line.hpp"
#include "Vectors/Vector.hpp"
#include "Vectors/Space.hpp"
#include "Vectors/RefSpace.hpp"


using namespace std;
using namespace Vectors;


void printPoint(const Point2DCont &p)
{
  cout<<"("<<p[0]<<";"<<p[1]<<")";
};

void printLine(const Line2DCont &l)
{
  cout<<"[ ";
  printPoint( l.getFrom() );
  cout<<" ; ";
  printPoint( l.getTo()   );
  cout<<" ]";
};


void testAndPrintRefSpace(const Vector2DCont v,
                          const double       alfa,
                          const Point2DCont  p)
{
  RefSpace2D   r1(v, alfa);
  Point2DCont  pp=r1.transform(p);
  cout<<"("<<p[0]<<";"<<p[1]<<") --> ("<<pp[0]<<";"<<pp[1]<<")"<<endl;
};


int main(int argc, const char *const *const argv)
{
  Point2DCont p1(1.5, 2.0);
  Point2DCont p2(1.1, 3.7);

  assert( p1.size()==2 );
  assert( p2.size()==2 );

  Vector2DCont v1(p1, p2);
  Vector2DCont v2(10.0, 2.0);
  Vector2DCont v3(v2);
  Vector2DCont v4=v1;
  v4+=v2;
  Vector2DCont v5(1.0, -0.5);

  // angles
  Vector2DCont v6( 1, 1);
  Vector2DCont v7(-1,-1);
  Vector2DCont v8( 1, 0);
  assert( fabs( v6.getOYangle()-M_PI/4 )<0.0001 );
  assert( fabs( v6.getAngle(v7)-M_PI   )<0.0001 );
  assert( fabs( v6.getAngle(v8)-M_PI/4 )<0.0001 );

  cout<<"p1: "; printPoint(p1); cout<<endl;
  cout<<"p2: "; printPoint(p2); cout<<endl;
  cout<<endl;

  cout<<"v1: "; printPoint(v1); cout<<endl;
  cout<<"v2: "; printPoint(v2); cout<<endl;
  cout<<"v3: "; printPoint(v3); cout<<endl;
  cout<<"v4: "; printPoint(v4); cout<<endl;
  cout<<"v5: "; printPoint(v5); cout<<endl;
  cout<<endl;

  cout<<"v1: "<<v1.length()<<endl;
  cout<<"v2: "<<v2.length()<<endl;
  cout<<"v3: "<<v3.length()<<endl;
  cout<<"v4: "<<v4.length()<<endl;
  cout<<"v5: "<<v5.length()<<endl;

  v2.normalize();
  cout<<"v2: "<<v2.length()<<endl;
  int c1=-2;
  v2*=c1;
  cout<<"v2: "<<v2.length()<<endl;
  double c2=-2.0;
  v2*=c2;
  v2.reverse();
  cout<<"v2: "<<v2.length()<<endl;

  Line2DCont l1(p1, p2);
  Line2DCont l2(p1, v5);
  Line2DCont l3(p2, p1);
  assert(l1==l3);

  cout<<endl;
  cout<<"l1: "; printLine(l1); cout<<endl;
  cout<<"l2: "; printLine(l2); cout<<endl;

  //
  // Space
  //
  Space<Line2DCont> space;

  Space<Line2DCont>::iterator it=space.begin();
  Space<Line2DCont>::const_iterator cit=space.begin();
  assert( it==space.end() );

  space.add(l1);
  space.add(l2);

  cout<<"space:"<<endl;
  for(Space<Line2DCont>::iterator it=space.begin(); it!=space.end(); ++it)
  {
    printLine(*it);
    cout<<endl;
  };

  //
  // transformation
  //

  testAndPrintRefSpace( Vector2DCont(0,0), 0,      Point2DCont(2,1) );
  testAndPrintRefSpace( Vector2DCont(0,0), M_PI/2, Point2DCont(2,1) );
  testAndPrintRefSpace( Vector2DCont(0,0), M_PI,   Point2DCont(2,1) );
  testAndPrintRefSpace( Vector2DCont(0,0), (3*M_PI)/2, Point2DCont(2,1) );
  testAndPrintRefSpace( Vector2DCont(0,0), 2*M_PI+0.1, Point2DCont(2,1) );
  testAndPrintRefSpace( Vector2DCont(3,4), M_PI,   Point2DCont(2,1) );

  return 0;
}; // main()

