/*
 * MapCreator.hpp
 *
 * class wrapping whole sensor system giving output
 * directly as the 2D Map of enviroment as it is
 * currently 'seen' by the robot.
 * it covers whole process starting from video capture
 * via TCP/IP until creation of local 2D Map.
 *
 */
#ifndef INCLUDE_ZUCHTER_MAPCREATOR_HPP_FILE
#define INCLUDE_ZUCHTER_MAPCREATOR_HPP_FILE

#include <memory>
#include <boost/noncopyable.hpp>

#include "CGraphics/Segment.hpp"
#include "VCCln/RasterDeserializer.hpp"
#include "VCCln/Client.hpp"
#include "Maps/Map2D.hpp"
#include "Maps/Vectorizer.hpp"
#include "Maps/DeepthTransformer.hpp"
#include "Path/GlobalMap.hpp"


namespace Zuchter
{

// listener
struct MapCreatorListener
{
  virtual ~MapCreatorListener(void)
  {
  };

  // called when network-related exception is thrown
  virtual void onNetworkError(const std::string &why) = 0;
  // called when graphic-related exception is thrown
  virtual void onGraphicsError(const std::string &why) = 0;
  // called on generic (unknown) exception
  virtual void onGenericError(const std::string &why) = 0;
}; // struct MapCreatorListener


// helper data-agregating class
struct MapCreatorParameters
{
  // addr            - address of VCSrv frame grabbing server
  // segmParams      - parameters used for segmentation
  // size            - size of image to process (if varray from captured
  //                   by VCSrv image will be scaled)
  // dpthTransParams - parameters requiered by deepth transformation
  // cropMargin      - how many pixel cut from edges of image?
  MapCreatorParameters(Zuchter::MapCreatorListener        &listener,
                       const Network::Address             &addr,
                       const CGraphics::SegmentParameters &segmParams,
                       const Maps::DeepthTransformParams  &dpthTransParams,
                       const unsigned int                 &cropMargin):
    _listener(listener),
    _addr(addr),
    _segmParams(segmParams),
    _dpthTransParams(dpthTransParams),
    _cropMargin(cropMargin)
  {
  };

  Zuchter::MapCreatorListener        &_listener;
  const Network::Address              _addr;
  const CGraphics::SegmentParameters  _segmParams;
  const Maps::DeepthTransformParams   _dpthTransParams;
  const unsigned int                  _cropMargin;
}; // struct MapCreatorParameters


class MapCreator: private boost::noncopyable
{
public:
  MapCreator(const MapCreatorParameters &p);

  // updates global map and returns const referece
  // to actualized one.
  const Path::GlobalMap &updateGlobalMap(void);

  // receoonects to a given host
  bool reconnect(void);

private:
  // returns Map2D (local map) from currently grabbed frame
  Maps::Map2D::AutoMap2D createLocalMap(void);

  Zuchter::MapCreatorListener       &_listener;

  Graphics::Size                     _size;
  CGraphics::Size                    _csize;
  const unsigned int                 _cropMargin;
  std::auto_ptr<VCCln::Client>       _vcSrvCln;
  VCCln::RasterDeserializer          _rasterDeserializer;
  CGraphics::SegmentParameters       _segmParams;
  CGraphics::Segment                 _segment;
  Maps::Vectorizer                   _vectorizer;
  Maps::DeepthTransformer            _dpthTransformer;
  Path::GlobalMap                    _gMap;
}; // namespace MapCreator

}; // namespace Zuchter

#endif

