/*
 * VectorImgFilter.hpp
 *
 */
#ifndef INCLUDE_MAPS_VECTORIMGFILTER_HPP_FILE
#define INCLUDE_MAPS_VECTORIMGFILTER_HPP_FILE

#include "Maps/VectorImg.hpp"


namespace Maps
{

//
// class agregating filtering operations useful for
// VectorImg processing, since only few of image-lines
// are truely importaint for further analisys.
//
class VectorImgFilter
{
public:
  typedef VectorImg                Img;
  typedef VectorImg::AutoVectorImg AutoImg;

  // vertical lines are useles (if the line if vertical, is
  // desided with a given precision)
  static AutoImg removeVerticals(const Img &vi, double prec=0.0001);

  // selects only lowest-lines that will be processed later
  static AutoImg selectLowLines(const Img &vi);

private:
  VectorImgFilter();        // no instance needed
}; // class VectorImgFilter

}; // namespace Maps

#endif

