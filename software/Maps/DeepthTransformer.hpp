/*
 * DeepthTransformer.hpp
 *
 */
#ifndef INCLUDE_MAPS_DEEPTHTRANSFORMER_HPP_FILE
#define INCLUDE_MAPS_DEEPTHTRANSFORMER_HPP_FILE

#include "Graphics/Size.hpp"
#include "Vectors/Point.hpp"
#include "Maps/VectorImg.hpp"
#include "Maps/Map2D.hpp"


namespace Maps
{

//
// helper class for DeepthTransformer, holding
// all of the transformation parameters and transforming
// points from image space to map space (both 2D).
//
class DeepthTransformParams
{
public:
  // constructor parameter are:
  //   H      - cammera height from ground in (in [m])
  //   d0     - distance from camera base (on ground) to
  //            place which is the first point visible on
  //            the captured image (in [m])
  //   size   - image size in [pixel;pixel]
  //   errMax - maximum error possible to accept when counting
  //            deepth from image (ie: if we want to be shure
  //            that missplace of sawn element is < 20[cm]
  //            errMax==0.20 (in [m])
  DeepthTransformParams(double H, double d0, const Graphics::Size &size,
                        double errMax):
    _H(H),
    _d0(d0),
    _size(size),
    _errMax(errMax)
  {
  };

  inline const double &getH(void) const
  {
    return _H;
  };
  inline const double &getD0(void) const
  {
    return _d0;
  };
  inline const Graphics::Size &getSize(void) const
  {
    return _size;
  };
  inline const double &getErrMax(void) const
  {
    return _errMax;
  };

private:
  const double         _H;      // camera height from ground (in [m])
  const double         _d0;     // camera first-visible-point distance
                                // on ground (in [m])
  const Graphics::Size _size;   // input image size
  const double         _errMax; // maximum error acceptable
};




//
// class calculating deepth from 2D vector image (with
// some assumptions, and simplifications), returning
// another vector image as an output.
//
class DeepthTransformer
{
public:
  // short typedef for better code readability
  typedef Maps::VectorImg     VImg;
  typedef VImg::AutoVectorImg AutoVImg;

  DeepthTransformer(const DeepthTransformParams &params);

  // reasionability-constraints:
  inline const double &getMaxX(void) const
  {
    return _maxX;
  };
  inline const double &getMaxY(void) const
  {
    return _maxY;
  };

  // transforms vector image by recalculating each line
  // (2 points) to deepth image (line)
  Maps::VectorImg::AutoVectorImg transform(const Maps::VectorImg &vImg) const;

  // converts point in 2D flat image to 2D point in
  // deepth-space (ie: "how far given point is" - it's
  // usable for Map2D building)
  inline Vectors::Point2DCont transform(const Vectors::Point2DCont &p) const
  {
    const double h1=getPixHeight(p);  // [pixel] -> [m]
    const double ax=getPixSpan  (p);  // ...

    const double d=( _params.getH()*_params.getD0() ) /
                   ( _params.getH()-h1 );  // Deepth of point
    const double a=(ax*d)/_params.getD0(); // spAn of a point

    Vectors::Point2DCont pOut(a, d);  // (x;y)
    return pOut;
  };

  // converts whiole lines
  inline Vectors::Line2DCont transform(const Vectors::Line2DCont &l) const
  {
    const Vectors::Point2DCont pfOut=transform( l.getFrom() );
    const Vectors::Point2DCont pfIn =transform( l.getTo()   );
    return Vectors::Line2DCont(pfOut, pfIn);
  };

private:
  // returns height of a given point on the image in [m]
  inline double getPixHeight(const Vectors::Point2DCont &p) const
  {
    // h1=(2*H)/S*p.y <=> h1=_Sh*p.y
    return _Sh*p[1];
  };

  // returns left/right span of pixel in [m] ('-'==left, '+'==right)
  inline double getPixSpan(const Vectors::Point2DCont &p) const
  {
    // this is conversion from [0;x] notation to [-x/2;+x/2]
    // ("forward" is in the middle of the picture!)
    const double x=p[0]-(_params.getSize().x()-1)/2.0;

    // sa/(sx/2)=ax/A => ax=(2*A)/sx*sa <=> ax=_Ss*sa => ax=_Ss*p.x
    return _Ss*x;
  };

  const DeepthTransformParams _params;  // transformation parameters

  double _Sh;     // conversion factor for recalculating
                  // height in [pixel] to height in [m]
  double _A;      // span of halt image in [pixel] is A in [m]
  double _Ss;     // conversion factor for recalculating
                  // span in [pixel] to span in [m]
  double _maxX;   // maximum X distance value for error<acceptable
  double _maxY;   // maximum Y distance value for error<acceptable
};

}; // namespace Maps

#endif

