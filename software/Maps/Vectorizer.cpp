/*
 * Vectorizer.cpp
 *
 */

#include "Vectorizer.hpp"

using namespace std;
using namespace CGraphics;
using namespace Vectors;


namespace Maps
{


Vectorizer::Vectorizer(const Size &size, double marginProcent):
              _size(size),
              _sizeG(size._x, size._y),
              _marginX( static_cast<unsigned int>(
                    (size._x*marginProcent)/100.0) ),
              _marginY( static_cast<unsigned int>(
                    (size._y*marginProcent)/100.0) ),
              _edge(_sizeG)
{
};


VectorImg::AutoVectorImg Vectorizer::vectorize(Raster &raster)
{
  if( !raster.hasMarker() )
    throw Exception("Vectorizer::vectorize(): "
                    "input raster doesn't have marker");
  {
    const Size size(_size._x, _size._y);
    if( raster.getSize()!=size )
      throw Exception("Vectorizer::vectorize(): "
                      "imput raster size doesn't match declared");
  }

  reset();                  // prepare Vectorizer for next image processing
  Space_t space;            // temporary vector space
  createEdgesMap(raster);   // fill _edge with edges data
  lowerLineEdgesMap();      // skipp all not-needed edges
  vectorizeEdgesMap(space); // vectorization itself :)

  // create and return VectorImage from our Space
  VectorImg::AutoVectorImg avi( new VectorImg(space) );
  return avi;
};


Point2DCont Vectorizer::fromBMPtoSpace(const Point2DCont &bmpPt) const
{
  Point2DCont pt( bmpPt[0], _sizeG.y()-1-bmpPt[1] );
  return pt;
};


void Vectorizer::vectorizeEdgesMap(Space_t &space)
{
  assert(_size._x>1 && _size._y>1);
  const Graphics::Size size(_size._x-1, _size._y-1);

  // scan whole immage
  for(unsigned int y=0; y<size.y(); ++y)
    for(unsigned int x=0; x<size.x(); ++x)
    {
      const Graphics::Position posStart(x,y);    // start pos.
      // is this a part of edge?
      if( !_edge.getPixelVal(posStart) )
        continue;

      // Right
      {
        const Graphics::Position posOff(x+1,y+0);
        if( _edge.getPixelVal(posOff) )
          followEdge(posStart,posOff, +1, 0, space);
      };
      // Left-Down
      if(x>0)
      {
        const Graphics::Position posOff(x-1,y+1);
        if( _edge.getPixelVal(posOff) )
          followEdge(posStart,posOff, -1,+1, space);
      };
      // Down
      {
        const Graphics::Position posOff(x+0,y+1);
        if( _edge.getPixelVal(posOff) )
          followEdge(posStart,posOff,  0,+1, space);
      };
      // Right-Down
      {
        const Graphics::Position posOff(x+1,y+1);
        if( _edge.getPixelVal(posOff) )
          followEdge(posStart,posOff, +1,+1, space);
      };
    }; // for(x)
};


void Vectorizer::followEdge(const Graphics::Position &posStart,
                            const Graphics::Position &posOff,
                            int dx, int dy, Space_t &space)
{
  const bool f=false;
  // this pixels doesn't count anymore:
  _edge.setPixel(posStart, f);
  _edge.setPixel(posOff,   f);

  Graphics::Position posEnd=posOff; // start with the shortest line
  Graphics::Position posCur(posOff.x()+dx, posOff.y()+dy);    // next pos.
  // loop while we're still followoing our line!
  while( _edge.inRange(posCur) && _edge.getPixelVal(posCur) )
  {
    posEnd=posCur;                  // set new "end" pixel
    _edge.setPixel(posCur, f);      // remove this pixel from raster
    posCur.offset(dx, dy);          // move to next pos.
  }; // while()

  // commit :)
  Point2DCont from( posStart.x(), posStart.y() );
  Point2DCont to  ( posEnd.x(),   posEnd.y()   );
  // transform form BMP to space coorinates:
  from=fromBMPtoSpace(from);
  to  =fromBMPtoSpace(to  );
  Line2DCont l(from, to);           // make line from point to point
  space.add(l);                     // and add it to output space
};


// helper namespace for function
namespace
{

inline int markerVal(const Raster &raster,
                     const Position &pos, int dx, int dy)
{
  const Position pOff(pos._x+dx, pos._y+dy);
  return raster.getMarker(pOff);
};

inline bool isNonEdge(const Position &pos, int dx, int dy,
                      const Graphics::Raster<bool> &edge)
{
  const Graphics::Position pOff(pos._x+dx, pos._y+dy);
  return !edge.getPixelVal(pOff);
};

inline bool isEdge(const Graphics::Position &pos,
                   const Raster &raster,
                   const Graphics::Raster<bool> &edge)
{
  const Position cPos( pos.x(), pos.y() );
  int            cMark=markerVal(raster, cPos, 0,0);
  pair<int,int> offs[4];
  offs[0]=pair<int,int>(-1, 0);
  offs[1]=pair<int,int>(-1,-1);
  offs[2]=pair<int,int>( 0,-1);
  offs[3]=pair<int,int>(+1,-1);

  for(unsigned int i=0; i<sizeof(offs)/sizeof(offs[0]); ++i)
    if( markerVal(raster, cPos, offs[i].first,offs[i].second)!=cMark &&
        isNonEdge(cPos, offs[i].first,offs[i].second, edge) )
      return true;

  return false;
}; // isEdge()

}; // unnamed namespace

void Vectorizer::createEdgesMap(const Raster &raster)
{
  const Graphics::Size size(raster.getSize()._x, raster.getSize()._y);

  // we skip surrounding "frame" of picture to make life
  // easier and processing faster. :)
  for(unsigned int y=_marginY; y<size.y()-_marginY; ++y)
    for(unsigned int x=_marginX; x<size.x()-_marginX; ++x)
    {
      const Graphics::Position pos(x,y);
      if( isEdge(pos, raster, _edge) )
        _edge.setPixel(pos, true);
    };
};


void Vectorizer::lowerLineEdgesMap(void)
{
  // scan image row-by row
  for(unsigned int x=0; x<_edge.getSize().x(); ++x)
  {
    unsigned int       y;
    Graphics::Position pos(x,y);

    // first of all - find our true-bottom edge
    for(y=_edge.getSize().y()-1; y>0; --y)
    {
      pos.y(y);                     // set to new y
      if( _edge.getPixelVal(pos) )  // edge found?
      {
        --y;                        // skip edge
        break;
      };
    }; // for(x->find_edge)

    // now empty all other "edge-points"
    const bool f=false;
    for(; y>0; --y)
    {
      pos.y(y);
      _edge.setPixel(pos, f);
    }; // for(x->remove_further_edges)
  }; // for(y)
};


void Vectorizer::reset(void)
{
  _edge.fillRaster(false);
};


}; // namespace Maps

