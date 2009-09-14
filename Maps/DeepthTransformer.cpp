/*
 * DeepthTransformer.cpp
 *
 */

#include "DeepthTransformer.hpp"

using namespace Vectors;


namespace Maps
{

DeepthTransformer::DeepthTransformer(const DeepthTransformParams &params):
    _params(params)
{
  // compute constant parameters for transformation once:
  _Sh=( 2*_params.getH()                     ) / _params.getSize().y();
  _A =( _params.getSize().x()*_params.getH() ) / _params.getSize().y();
  _Ss=( 2*_A                                 ) / _params.getSize().x();

  //
  // compute maximum accaptable distances for X and Y:
  //

  // first of all - Y:
  for(_maxY=(_params.getSize().y()-1)/2.0;
      _maxY>0; _maxY-=1.0) // step by 1[pixel]
  {
    Vectors::Point2DCont p1( (_params.getSize().x()-1)/2.0, _maxY-1 );
    Vectors::Point2DCont p2( (_params.getSize().x()-1)/2.0, _maxY   );
    p1=transform(p1);       // check ...
    p2=transform(p2);       // ...
    if( p2[1]-p1[1]<=_params.getErrMax() )    // error is acceptable?
      break;
  }; // for(_maxY)

  // then - X:
  for(_maxX=(_params.getSize().x()-1)/2.0;
      _maxX>0; _maxX-=1.0) // step by 1[pixel]
  {
    Vectors::Point2DCont p1(_maxX-1, _maxY);
    Vectors::Point2DCont p2(_maxX,   _maxY);
    p1=transform(p1);       // check ...
    p2=transform(p2);       // ...
    if( p2[0]-p1[0]<=_params.getErrMax() )    // error is acceptable?
      break;
  }; // for(_maxX)
};


// local helpers namespace
namespace
{

// returns 'v' if min<=v<=max or min or max, if slammer or
// grater respectively.
inline double chooseV(const double &v, const double &min, const double &max)
{
  if(max<v)
    return max;

  if(v<min)
    return min;

  return v;
};

}; // unnamed namespace



VectorImg::AutoVectorImg DeepthTransformer::transform(
                                const VectorImg &vImg) const
{
  VectorImg::AutoVectorImg  aVImg( new VectorImg( vImg.linesCnt() ) );
  VectorImg                &out=*aVImg.get();

  // transform each line...
  for(unsigned int i=0; i<vImg.linesCnt(); ++i)
  {
    const Line2DCont  &lIn=vImg[i];
    const Point2DCont &pf=lIn.getFrom();
    const Point2DCont &pt=lIn.getTo();

    double pfX=pf[0];
    double pfY=pf[1];
    double ptX=pt[0];
    double ptY=pt[1];

    if(ptX<pfX)     // swap needed (for easier analisys)?
    {
      double tmp;
      // swap OX:
      tmp=ptX;
      ptX=pfX;
      pfX=tmp;
      // swap OY:
      tmp=ptY;
      ptY=pfY;
      pfY=tmp;
    };

    // basic limits:
    //  y1<=y<=y2 && x1<=x<=x2
    const double mid=(_params.getSize().x()-1)/2.0;  // middle of the screen
    const double y1 =0;
    const double y2 =_maxY;
    const double x1 =mid-_maxX;
    const double x2 =mid+_maxX;

    //
    // check if coordinates are ok:
    //

    // check if line is (at least in part) in range
    if(pfX<x1 && ptX<x1 ||      // both X's too small?
       x2<pfX && x2<ptX ||      // ... too large?
       pfY<y1 && ptY<y1 ||      // both Y's too small?
       y2<pfY && y2<ptY    )    // ... too large?
      continue;                 // so skip'em all! :)

    //
    // here we know that at least one point is in range!
    //
    // correnctions is done by similiarity of triangles.
    //

    if(pfX<x1)                  // From is out of range on OX?
    {
      double Vx =pfX-ptX;       // vector X
      double Vy =pfY-ptY;       // ...    Y
      double Vnx=x1-ptX;        // new vector X
      double Vny=(Vnx*Vy)/Vx;   // ...        Y
      pfX=ptX+Vnx;              // write changes
      pfY=ptY+Vny;              // ...
    };

    if(y2<pfY)                  // From is out of range on OY?
    {
      double Vx =pfX-ptX;       // vector X
      double Vy =pfY-ptY;       // ...    Y
      double Vny=y2-ptY;        // new vector Y
      double Vnx=(Vny*Vx)/Vy;   // ...        X
      pfX=ptX+Vnx;              // write changes
      pfY=ptY+Vny;              // ...
    };

    if(x2<ptX)                  // To   is out of range on OX?
    {
      double Vx =ptX-pfX;       // vector X
      double Vy =ptY-pfY;       // ...    Y
      double Vnx=x2-pfX;        // new vector X
      double Vny=(Vnx*Vy)/Vx;   // ...        Y
      ptX=pfX+Vnx;              // write changes
      ptY=pfY+Vny;              // ...
    };

    if(y2<ptY)                  // To   is out of range on OY?
    {
      double Vx =ptX-pfX;       // vector X
      double Vy =ptY-pfY;       // ...    Y
      double Vny=y2-pfY;        // new vector X
      double Vnx=(Vny*Vx)/Vy;   // ...        Y
      ptX=pfX+Vnx;              // write changes
      ptY=pfY+Vny;              // ...
    };

    // construct new line from given (secure) coordinates
    const Point2DCont pFrom(pfX, pfY);
    const Point2DCont pTo  (ptX, ptY);
    const Line2DCont  lInterm(pFrom, pTo);      // Intermidiet line...
    // transformation itself
    Line2DCont lOut=transform(lInterm);         // transformed line
    out.add(lOut);                              // append to new space
  };

  return aVImg;
};

}; // namespace Maps

