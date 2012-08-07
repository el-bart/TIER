/*
 * Segment.hpp
 *
 */
#ifndef INCLUDE_CGRAPHICS_SEGMENT_HPP_FILE
#define INCLUDE_CGRAPHICS_SEGMENT_HPP_FILE

#include <boost/noncopyable.hpp>

#include "CGraphics/Raster.hpp"
#include "CGraphics/Exception.hpp"


namespace CGraphics
{


// helper class for representing parameters
class SegmentParameters
{
public:
  // sigma   - smoothing level
  // k       - threshold
  // minSize - min. component size
  SegmentParameters(double sigma=0.5, double k=200, int minSize=20):
                        _sigma(sigma),
                        _k(k),
                        _minSize(minSize)
  {
  };

  // getters:
  inline double getSigma(void)   const { return _sigma;   };
  inline double getK(void)       const { return _k;       };
  inline int    getMinSize(void) const { return _minSize; };

private:
  const double _sigma;
  const double _k;
  const int    _minSize;
}; // class SegmentPrameters


// forward declaration of implementation class
class SegmentImpl;

//
// class making segmentation of image
//
class Segment: boost::noncopyable
{
public:
  Segment(const CGraphics::Size &size);
  ~Segment();

  // returns it's response as a marker states (markers are
  // reseted at the begining).
  int process(const SegmentParameters &params,
              Raster &raster);

private:
  Segment();

  CGraphics::SegmentImpl *_pimpl;
}; // class Segment


}; // namespace CGraphics


#endif

