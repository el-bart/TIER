#ifndef ____MAIN_SEGMENT_IMAGE
#define ____MAIN_SEGMENT_IMAGE

#include <map>
#include <cstdlib>
#include "image.h"
#include "misc.h"
#include "filter.h"
#include "segment-graph.h"

#include "Helper.hpp"
#include "Raster.hpp"       // brut-hack... :/
#include "Segment.hpp"      // ...
#include "Exception.hpp"    // ...


using namespace std;
using namespace CGraphics;
using namespace SegmentWrapper;


// random color
rgb random_rgb(){
  rgb c;
  //double r;

  c.r = (uchar)random();
  c.g = (uchar)random();
  c.b = (uchar)random();

  return c;
}

// dissimilarity measure between pixels
static inline float diff(image<float> *r, image<float> *g, image<float> *b,
	            		 int x1, int y1,  int x2, int y2)
{
  return sqrt( square( imRef(r, x1, y1)-imRef(r, x2, y2) ) +
 	           square( imRef(g, x1, y1)-imRef(g, x2, y2) ) +
 	           square( imRef(b, x1, y1)-imRef(b, x2, y2) )   );
}

/*
 * Segment an image
 *
 * Returns a Marker representing the segmentation of a given Raster.
 *
 */
int segment_image(Raster &rIn,
                  const SegmentParameters &segParams,
                  Helper &mem)
{
  const int width =mem._width;
  const int height=mem._height;

  const double sigma   =segParams.getSigma();
  const double c       =segParams.getK();
  const int    min_size=segParams.getMinSize();

  // smooth each color channel
  image<float> *r=mem._r.get();
  image<float> *g=mem._g.get();
  image<float> *b=mem._b.get();
  for(int y=0; y<height; y++)
    for(int x=0; x<width; x++)
    {
      Position    pos(x,y);
      const Pixel p=rIn.getPixel(pos);
      imRef(r, x, y)=p.getR();
      imRef(g, x, y)=p.getG();
      imRef(b, x, y)=p.getB();
    }
  image<float> *smooth_r = smooth(r, sigma);
  image<float> *smooth_g = smooth(g, sigma);
  image<float> *smooth_b = smooth(b, sigma);

  // build graph
  edge *edges=mem._edgesArray._edges;
  int num=0;
  for(int y=0; y<height; y++)
    for(int x=0; x<width; x++)
    {
      if(x<width-1)
      {
    	edges[num].a = y * width + x;
	    edges[num].b = y * width + (x+1);
    	edges[num].w = diff(smooth_r, smooth_g, smooth_b, x, y, x+1, y);
    	num++;
      }

      if(y<height-1)
      {
	    edges[num].a = y * width + x;
    	edges[num].b = (y+1) * width + x;
	    edges[num].w = diff(smooth_r, smooth_g, smooth_b, x, y, x, y+1);
    	num++;
      }

      if( (x<width-1) && (y<height-1) )
      {
	    edges[num].a = y * width + x;
    	edges[num].b = (y+1) * width + (x+1);
	    edges[num].w = diff(smooth_r, smooth_g, smooth_b, x, y, x+1, y+1);
    	num++;
      }

      if( (x<width-1) && (y>0) )
      {
    	edges[num].a = y * width + x;
	    edges[num].b = (y-1) * width + (x+1);
    	edges[num].w = diff(smooth_r, smooth_g, smooth_b, x, y, x+1, y-1);
	    num++;
      }
    }
  delete smooth_r;
  delete smooth_g;
  delete smooth_b;

  // segment
  universe *u=segment_graph(width*height, num, edges, c);

  // post process small components
  for(int i=0; i<num; i++)
  {
    int a=u->find(edges[i].a);
    int b=u->find(edges[i].b);
    if( (a!=b) && ( (u->size(a)<min_size) || (u->size(b)<min_size) ) )
      u->join(a, b);
  };

//  int num_ccs=u->num_sets();

  short int curSeg=1;             // segment we now process
  map<int, short int> segments;   // maps "poz" to usable number

  // write down results
  for(int y=0; y<height-1; y++)
    for(int x=0; x<width-1; x++)
    {
      int comp = u->find(y * width + x);
      // add marker and store it's value
      if( segments.find(comp)==segments.end() ) // no such value yet?
      {
        segments[comp]=curSeg;      // then add it! :)
        curSeg++;
      };
      Position pos(x,y);
      rIn.setMarker(pos, segments[comp]);
    }

  delete u;

  return curSeg;      // return total numer of segments
}


#endif

