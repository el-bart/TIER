/*
 * Helper.hpp
 *
 * for storing some of the parameters
 *
 */
#ifndef INCLUDE_SEGMENTWRAPPER_HELPER_HPP_FILE
#define INCLUDE_SEGMENTWRAPPER_HELPER_HPP_FILE

#include <memory>
#include <vector>
#include <assert.h>

// these are needed so that optimalization will make sense
#include "image.h"          // image<>
#include "disjoint-set.h"   // universe
#include "segment-graph.h"  // edge

#include "Exception.hpp"


namespace SegmentWrapper
{

//
// internal holder of array of edges ("auto_edges_array")
//
namespace
{

struct ArrayHolder
{
  ArrayHolder(int size):
            _edges( new edge[size] )
  {
  };

  ~ArrayHolder()
  {
    delete [] _edges;
  };

  edge *_edges;
};

}; // unnamed namespace


//
// holder of memory for alghorithm, so that
// it wouldn't have to be (de)allocated each
// and every time it is started.
//
struct Helper
{
  Helper(unsigned int width, unsigned int height):
        _width(width),
        _height(height),
        _r( new image<float>(width, height) ),
        _g( new image<float>(width, height) ),
        _b( new image<float>(width, height) ),
        _edgesArray(width*height*4)
  {
    assert(width>0 && height>0);
  };

  ~Helper()
  {
  };

  const unsigned int _width;
  const unsigned int _height;

  //
  // internals for the lib
  //

  std::auto_ptr< image<float> > _r;
  std::auto_ptr< image<float> > _g;
  std::auto_ptr< image<float> > _b;

  ArrayHolder _edgesArray;
}; // class Helper



}; // namespace SegmentWrapper


#endif

