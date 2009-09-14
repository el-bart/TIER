/*
 * Vectorizer.hpp
 *
 * class wrapping libautotrace (in C), providing
 * functionality to process raster images to vector
 * form. raster is represented by Raster class, and
 * vector representation by VectorImg.
 *
 */
#ifndef INCLUDE_MAPS_VECTORIZER_HPP_FILE
#define INCLUDE_MAPS_VECTORIZER_HPP_FILE

#include "CGraphics/Raster.hpp"
#include "CGraphics/Size.hpp"
#include "Vectors/Space.hpp"
#include "Graphics/Raster.hpp"
#include "Maps/Exception.hpp"
#include "Maps/VectorImg.hpp"


namespace Maps
{

class Vectorizer
{
public:
  typedef Vectors::Space< Vectors::Line2DCont > Space_t;

  // size          - size of buffer for the image
  // marginProcent - % of image size to be treated as a margin
  //                 (is not included in vectorization)
  Vectorizer(const CGraphics::Size &size, double marginProcent=4);

  // vectorize input image
  //   note: this operation CHANGES marker of input Raster!
  Maps::VectorImg::AutoVectorImg vectorize(CGraphics::Raster &raster);

private:
  // switches from bitmap coordinates (reversed) to "regular", where
  // [0,0] is lower-left corrner.
  Vectors::Point2DCont fromBMPtoSpace(const Vectors::Point2DCont &bmpPt) const;
  void vectorizeEdgesMap(Space_t &space);   // vectorization itself
  void followEdge(const Graphics::Position &pos,
                  const Graphics::Position &pos2,
                  int dx, int dy,
                  Space_t &space);          // follow edge starting from 'pos'
                                            // and add as much as possible to
                                            // 'space'; edge data will be
                                            // erased from _edge as soon as
                                            // will be used once.
  void createEdgesMap(const CGraphics::Raster &raster);
                                    // fill _edge from raster
  void lowerLineEdgesMap(void);     // leave only most-bottom parts (edges)
                                    // of edge-map, since only these are needed
  void reset(void);                 // reset whole Vectorizer for next image

  const CGraphics::Size  _size;     // size of input images
  const Graphics::Size   _sizeG;    // size of input images
  const unsigned int     _marginX;  // margin of OX in pixels
  const unsigned int     _marginY;  // margin of OY in pixels

  Graphics::Raster<bool> _edge;     // true if pixel[y][x] is edge pixel
}; // class Vectorizer


}; // namespace Maps


#endif

