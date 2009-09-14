/*
 * VectorImg.hpp
 *
 * class representing raster image.
 *
 */
#ifndef INCLUDE_MAPS_VECTORIMG_HPP_FILE
#define INCLUDE_MAPS_VECTORIMG_HPP_FILE

#include <memory>
#include <vector>

#include "Vectors/Space.hpp"
#include "Maps/Exception.hpp"


namespace Maps
{

//
// class extending 2D space for simpler use in raster
// processing applied to input image.
//
class VectorImg: public Vectors::Space< Vectors::Line2DCont >
{
public:
  typedef std::auto_ptr< Maps::VectorImg > AutoVectorImg;
  typedef Vectors::Line2DCont              Line;

  // make empty VectorImg, preparet to accept at most maxLinesCnt lines
  VectorImg(unsigned int maxLinesCnt);
  // space - 2D vector space from Vectorizer
  VectorImg(const Vectors::Space<Line> &space);

  inline unsigned int linesCnt(void) const
  {
    return _elems.size();
  };

  inline Line &operator[](unsigned int poz)
  {
    assert( poz<linesCnt() );
    return _elems[poz];
  };
  inline const Line &operator[](unsigned int poz) const
  {
    assert( poz<linesCnt() );
    return _elems[poz];
  };

  inline void add(const Line &line)
  {
    _elems.push_back(line);
  };
  inline void remove(unsigned int poz)
  {
    assert( poz<linesCnt() );
    _elems.erase( _elems.begin()+poz ); // remove the element!
  };

private:
  std::vector<Line> _elems;
}; // class VectorImg

}; // namespace CGraphics

#endif

