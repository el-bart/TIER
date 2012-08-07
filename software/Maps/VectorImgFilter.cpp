/*
 * VectorImgFilter.cpp
 *
 */
#include <math.h>
#include <boost/noncopyable.hpp>

#include "VectorImgFilter.hpp"
#include "Exception.hpp"

using namespace Vectors;


namespace Maps
{

VectorImgFilter::AutoImg VectorImgFilter::removeVerticals(const Img &vi,
                                                          double prec)
{
  AutoImg aImg( new VectorImg( vi.linesCnt() ) );
  Img &img=*aImg.get();                 // helper reference

  for(unsigned int i=0; i<vi.linesCnt(); ++i)
  {
    const Line2DCont  &l =vi[i];        // get i-th line
    const Point2DCont &p1=l.getFrom();  // get first of 2 points
    const Point2DCont &p2=l.getTo();    // get second of 2 points

    if( fabs(p1[0]-p2[0])>prec )        // line is NOT vertical?
      img.add(l);       // add line to output
  };

  return aImg;
};


VectorImgFilter::AutoImg VectorImgFilter::selectLowLines(const Img &vi)
{
  throw Maps::Exception("VectorImgFilter::selectLowLines(): not implemented");
  AutoImg aImg( new VectorImg( vi.linesCnt() ) );
  return aImg;
};

}; // namespace Maps

