
CC=g++

FLAGS_BASIC=-Wall
CXXFLAGS_REL=$(FLAGS_BASIC) -O3 -DNDEBUG
CXXFLAGS_DEB=$(FLAGS_BASIC) -g3 -pg

LIBS_BASIC=-lpthread
CXXLIBS_REL=$(LIBS_BASIC)
CXXLIBS_DEB=$(LIBS_BASIC)

BIN_NAME=TEST.out
APP_NAME=zuchter



all: rel

rel: CXXFLAGS=$(CXXFLAGS_REL)
rel: CXXLIBS=$(CXXLIBS_REL)
rel: TARGET=rel
rel: make_common

deb: CXXFLAGS=$(CXXFLAGS_DEB)
deb: CXXLIBS=$(CXXLIBS_DEB)
deb: TARGET=deb
deb: make_common

test: CXXFLAGS=$(CXXFLAGS_DEB)
test: CXXLIBS=$(CXXLIBS_DEB)
test: deb TEST.o
	$(CC) $(CXXFLAGS_DEB) -o $(BIN_NAME) VCCln/CGraphics/Segment.o VCCln/CGraphics/Raster.o VCCln/CGraphics/bitmap/bitmap.o VCCln/CGraphics/filter/histo_filter.o VCCln/CGraphics/filter/main_filters.o VCCln/CGraphics/filter/req_col_unif.o VCCln/CGraphics/filter/raster/scale_raster.o VCCln/CGraphics/filter/raster/histogram.o VCCln/CGraphics/filter/raster/stats.o VCCln/CGraphics/filter/raster/main_raster.o VCCln/CGraphics/filter/denoise.o VCCln/Network/Server.o VCCln/Network/TCPServer.o VCCln/Network/TCPClient.o VCCln/Network/Resolver.o VCCln/RasterDeserializer.o Maps/DeepthTransformer.o Maps/VectorImg.o Maps/VectorImgFilter.o Maps/Map2D.o Maps/Graphics/Histogram.o Maps/Graphics/V4LCapture.o Maps/Graphics/ProcessRaster.o Maps/Vectorizer.o Path/Planner.o Path/GlobalMap.o CommProto/ProtoLocal.o CommProto/sterownik_uC/wabuf/wabuf.o CommProto/sterownik_uC/tier_low_io/portio/portio.o CommProto/sterownik_uC/tier_low_io/tier_low_io.o CommProto/sterownik_uC/tier_io/tier_io.o CommProto/sterownik_uC/tier_io/tier_pck/tier_pck.o MapCreator.o PathCreator.o CommandCreator.o TIERctl.o DebugWin/Debug[TR]*.o DebugWin/VCSrv/Ras*.o TEST.o $(CXXLIBS)



make_common: MapCreator.o PathCreator.o CommandCreator.o TIERctl.o Zuchter.o
	$(MAKE) -C VCCln/ $(TARGET)
	$(MAKE) -C Maps/ $(TARGET)
	$(MAKE) -C CGraphics/ $(TARGET)
	$(MAKE) -C Path/ $(TARGET)
	$(MAKE) -C CommProto/ $(TARGET)
	$(MAKE) -C Threads/ $(TARGET)
	$(CC) $(CXXFLAGS) -o $(APP_NAME) VCCln/CGraphics/Segment.o VCCln/CGraphics/Raster.o VCCln/CGraphics/bitmap/bitmap.o VCCln/CGraphics/filter/histo_filter.o VCCln/CGraphics/filter/main_filters.o VCCln/CGraphics/filter/req_col_unif.o VCCln/CGraphics/filter/raster/scale_raster.o VCCln/CGraphics/filter/raster/histogram.o VCCln/CGraphics/filter/raster/stats.o VCCln/CGraphics/filter/raster/main_raster.o VCCln/CGraphics/filter/denoise.o VCCln/Network/Server.o VCCln/Network/TCPServer.o VCCln/Network/TCPClient.o VCCln/Network/Resolver.o VCCln/RasterDeserializer.o Maps/DeepthTransformer.o Maps/VectorImg.o Maps/VectorImgFilter.o Maps/Map2D.o Maps/Graphics/Histogram.o Maps/Graphics/V4LCapture.o Maps/Graphics/ProcessRaster.o Maps/Vectorizer.o Path/Planner.o Path/GlobalMap.o CommProto/ProtoLocal.o CommProto/sterownik_uC/wabuf/wabuf.o CommProto/sterownik_uC/tier_low_io/portio/portio.o CommProto/sterownik_uC/tier_low_io/tier_low_io.o CommProto/sterownik_uC/tier_io/tier_io.o CommProto/sterownik_uC/tier_io/tier_pck/tier_pck.o MapCreator.o PathCreator.o CommandCreator.o TIERctl.o DebugWin/Debug[TR]*.o DebugWin/VCSrv/Ras*.o Zuchter.o $(CXXLIBS)


clean:
	$(MAKE) -C VCCln/ clean
	$(MAKE) -C Maps/ clean
	$(MAKE) -C CGraphics/ clean
	$(MAKE) -C Path/ clean
	$(MAKE) -C CommProto/ clean
	$(MAKE) -C Threads/ clean
	rm -f *.o
	rm -f $(BIN_NAME)
	rm -f $(APP_NAME)
	rm -f gmon.out


MapCreator.o: MapCreator.cpp MapCreator.hpp CGraphics/Segment.hpp \
  CGraphics/CGraphics/Raster.hpp CGraphics/CGraphics/CGraphics/Pixel.hpp \
  CGraphics/CGraphics/CGraphics/CGraphics/Exception.hpp \
  CGraphics/CGraphics/CGraphics/Size.hpp \
  CGraphics/CGraphics/CGraphics/CGraphics/Position.hpp \
  CGraphics/CGraphics/CGraphics/CGraphics/CGraphics/Exception.hpp \
  CGraphics/CGraphics/CGraphics/Position.hpp \
  CGraphics/CGraphics/CGraphics/Exception.hpp \
  CGraphics/CGraphics/Exception.hpp VCCln/RasterDeserializer.hpp \
  VCCln/CGraphics/Raster.hpp VCCln/VCCln/Exception.hpp VCCln/Client.hpp \
  VCCln/Network/Connection.hpp VCCln/Network/Network/Socket.hpp \
  VCCln/Network/Network/Network/Exception.hpp \
  VCCln/Network/Network/Exception.hpp VCCln/Network/TCPClient.hpp \
  VCCln/Network/Network/Client.hpp \
  VCCln/Network/Network/Network/Connection.hpp \
  VCCln/Network/Network/Network/Address.hpp Maps/Map2D.hpp \
  Maps/Vectors/Space.hpp Maps/Vectors/Vectors/Line.hpp \
  Maps/Vectors/Vectors/Vectors/Point.hpp \
  Maps/Vectors/Vectors/Vectors/Vectors/Exception.hpp \
  Maps/Vectors/Vectors/Vectors/Vector.hpp \
  Maps/Vectors/Vectors/Vectors/Vectors/Point.hpp \
  Maps/Vectors/Vectors/Vectors/Exception.hpp \
  Maps/Vectors/Vectors/Exception.hpp Maps/Vectors/RefSpace.hpp \
  Maps/Vectors/Vectors/Point.hpp Maps/Vectors/Vectors/Vector.hpp \
  Maps/Maps/Exception.hpp Maps/Maps/VectorImg.hpp \
  Maps/Maps/Vectors/Space.hpp Maps/Maps/Maps/Exception.hpp \
  Maps/Vectorizer.hpp Maps/CGraphics/Raster.hpp Maps/CGraphics/Size.hpp \
  Maps/Graphics/Raster.hpp Maps/Graphics/Graphics/Exception.hpp \
  Maps/Graphics/Graphics/Pixel.hpp \
  Maps/Graphics/Graphics/Graphics/Exception.hpp \
  Maps/Graphics/Graphics/Size.hpp \
  Maps/Graphics/Graphics/Graphics/Position.hpp \
  Maps/Graphics/Graphics/Graphics/Graphics/Exception.hpp \
  Maps/Graphics/Graphics/Position.hpp Maps/DeepthTransformer.hpp \
  Maps/Graphics/Size.hpp Maps/Vectors/Point.hpp Maps/Maps/Map2D.hpp \
  Path/GlobalMap.hpp Path/Maps/Map2D.hpp Path/Path/Exception.hpp \
  DebugWin/DebugTransmit.hpp DebugWin/Network/TCPClient.hpp \
  DebugWin/Graphics/Raster.hpp DebugWin/CGraphics/Raster.hpp \
  DebugWin/Maps/VectorImg.hpp

PathCreator.o: PathCreator.cpp PathCreator.hpp Path/Planner.hpp \
  Path/Path/Plan.hpp Path/Path/Path/Destination.hpp \
  Path/Path/Path/Vectors/Vector.hpp \
  Path/Path/Path/Vectors/Vectors/Point.hpp \
  Path/Path/Path/Vectors/Vectors/Vectors/Exception.hpp \
  Path/Path/Path/Vectors/Vectors/Exception.hpp \
  Path/Path/Path/Exception.hpp Path/Path/GlobalMap.hpp \
  Path/Path/Maps/Map2D.hpp Path/Path/Maps/Vectors/Space.hpp \
  Path/Path/Maps/Vectors/Vectors/Line.hpp \
  Path/Path/Maps/Vectors/Vectors/Vectors/Point.hpp \
  Path/Path/Maps/Vectors/Vectors/Vectors/Vector.hpp \
  Path/Path/Maps/Vectors/Vectors/Vectors/Exception.hpp \
  Path/Path/Maps/Vectors/Vectors/Exception.hpp \
  Path/Path/Maps/Vectors/RefSpace.hpp \
  Path/Path/Maps/Vectors/Vectors/Point.hpp \
  Path/Path/Maps/Vectors/Vectors/Vector.hpp \
  Path/Path/Maps/Maps/Exception.hpp Path/Path/Maps/Maps/VectorImg.hpp \
  Path/Path/Maps/Maps/Vectors/Space.hpp \
  Path/Path/Maps/Maps/Maps/Exception.hpp Path/Path/Exception.hpp \
  Path/GlobalMap.hpp

CommandCreator.o: CommandCreator.cpp CommandCreator.hpp \
  CommProto/TIERproto.hpp CommProto/CommProto/Exception.hpp \
  Path/GlobalMap.hpp Path/Maps/Map2D.hpp Path/Maps/Vectors/Space.hpp \
  Path/Maps/Vectors/Vectors/Line.hpp \
  Path/Maps/Vectors/Vectors/Vectors/Point.hpp \
  Path/Maps/Vectors/Vectors/Vectors/Vectors/Exception.hpp \
  Path/Maps/Vectors/Vectors/Vectors/Vector.hpp \
  Path/Maps/Vectors/Vectors/Vectors/Vectors/Point.hpp \
  Path/Maps/Vectors/Vectors/Vectors/Exception.hpp \
  Path/Maps/Vectors/Vectors/Exception.hpp Path/Maps/Vectors/RefSpace.hpp \
  Path/Maps/Vectors/Vectors/Point.hpp \
  Path/Maps/Vectors/Vectors/Vector.hpp Path/Maps/Maps/Exception.hpp \
  Path/Maps/Maps/VectorImg.hpp Path/Maps/Maps/Vectors/Space.hpp \
  Path/Maps/Maps/Maps/Exception.hpp Path/Path/Exception.hpp Path/Plan.hpp \
  Path/Path/Destination.hpp Path/Path/Vectors/Vector.hpp \
  Threads/Thread.hpp Threads/Threads/Exception.hpp Threads/Lock.hpp \
  Threads/Threads/Mutex.hpp Threads/Threads/Threads/Exception.hpp \
  Threads/Threads/GeneralLock.hpp Threads/Threads/Threads/Mutex.hpp \
  Threads/Mutex.hpp

TIERctl.o: TIERctl.cpp TIERctl.hpp Zuchter/MapCreator.hpp \
  Zuchter/CGraphics/Segment.hpp Zuchter/CGraphics/CGraphics/Raster.hpp \
  Zuchter/CGraphics/CGraphics/CGraphics/Pixel.hpp \
  Zuchter/CGraphics/CGraphics/CGraphics/CGraphics/Exception.hpp \
  Zuchter/CGraphics/CGraphics/CGraphics/Size.hpp \
  Zuchter/CGraphics/CGraphics/CGraphics/CGraphics/Position.hpp \
  Zuchter/CGraphics/CGraphics/CGraphics/CGraphics/CGraphics/Exception.hpp \
  Zuchter/CGraphics/CGraphics/CGraphics/Position.hpp \
  Zuchter/CGraphics/CGraphics/CGraphics/Exception.hpp \
  Zuchter/CGraphics/CGraphics/Exception.hpp \
  Zuchter/VCCln/RasterDeserializer.hpp Zuchter/VCCln/CGraphics/Raster.hpp \
  Zuchter/VCCln/VCCln/Exception.hpp Zuchter/VCCln/Client.hpp \
  Zuchter/VCCln/Network/Connection.hpp \
  Zuchter/VCCln/Network/Network/Socket.hpp \
  Zuchter/VCCln/Network/Network/Network/Exception.hpp \
  Zuchter/VCCln/Network/Network/Exception.hpp \
  Zuchter/VCCln/Network/TCPClient.hpp \
  Zuchter/VCCln/Network/Network/Client.hpp \
  Zuchter/VCCln/Network/Network/Network/Connection.hpp \
  Zuchter/VCCln/Network/Network/Network/Address.hpp \
  Zuchter/Maps/Map2D.hpp Zuchter/Maps/Vectors/Space.hpp \
  Zuchter/Maps/Vectors/Vectors/Line.hpp \
  Zuchter/Maps/Vectors/Vectors/Vectors/Point.hpp \
  Zuchter/Maps/Vectors/Vectors/Vectors/Vectors/Exception.hpp \
  Zuchter/Maps/Vectors/Vectors/Vectors/Vector.hpp \
  Zuchter/Maps/Vectors/Vectors/Vectors/Vectors/Point.hpp \
  Zuchter/Maps/Vectors/Vectors/Vectors/Exception.hpp \
  Zuchter/Maps/Vectors/Vectors/Exception.hpp \
  Zuchter/Maps/Vectors/RefSpace.hpp \
  Zuchter/Maps/Vectors/Vectors/Point.hpp \
  Zuchter/Maps/Vectors/Vectors/Vector.hpp Zuchter/Maps/Maps/Exception.hpp \
  Zuchter/Maps/Maps/VectorImg.hpp Zuchter/Maps/Maps/Vectors/Space.hpp \
  Zuchter/Maps/Maps/Maps/Exception.hpp Zuchter/Maps/Vectorizer.hpp \
  Zuchter/Maps/CGraphics/Raster.hpp Zuchter/Maps/CGraphics/Size.hpp \
  Zuchter/Maps/Graphics/Raster.hpp \
  Zuchter/Maps/Graphics/Graphics/Exception.hpp \
  Zuchter/Maps/Graphics/Graphics/Pixel.hpp \
  Zuchter/Maps/Graphics/Graphics/Graphics/Exception.hpp \
  Zuchter/Maps/Graphics/Graphics/Size.hpp \
  Zuchter/Maps/Graphics/Graphics/Graphics/Position.hpp \
  Zuchter/Maps/Graphics/Graphics/Graphics/Graphics/Exception.hpp \
  Zuchter/Maps/Graphics/Graphics/Position.hpp \
  Zuchter/Maps/DeepthTransformer.hpp Zuchter/Maps/Graphics/Size.hpp \
  Zuchter/Maps/Vectors/Point.hpp Zuchter/Maps/Maps/Map2D.hpp \
  Zuchter/Path/GlobalMap.hpp Zuchter/Path/Maps/Map2D.hpp \
  Zuchter/Path/Path/Exception.hpp Zuchter/PathCreator.hpp \
  Zuchter/Path/Planner.hpp Zuchter/Path/Path/Plan.hpp \
  Zuchter/Path/Path/Path/Destination.hpp \
  Zuchter/Path/Path/Path/Vectors/Vector.hpp \
  Zuchter/Path/Path/Path/Exception.hpp Zuchter/Path/Path/GlobalMap.hpp \
  Zuchter/CommandCreator.hpp Zuchter/CommProto/TIERproto.hpp \
  Zuchter/CommProto/CommProto/Exception.hpp Zuchter/Path/Plan.hpp \
  Zuchter/Threads/Thread.hpp Zuchter/Threads/Threads/Exception.hpp \
  Zuchter/Threads/Lock.hpp Zuchter/Threads/Threads/Mutex.hpp \
  Zuchter/Threads/Threads/Threads/Exception.hpp \
  Zuchter/Threads/Threads/GeneralLock.hpp \
  Zuchter/Threads/Threads/Threads/Mutex.hpp Zuchter/Threads/Mutex.hpp

Zuchter.o: Zuchter.cpp CommProto/ProtoLocal.hpp \
  CommProto/CommProto/Exception.hpp CommProto/CommProto/TIERproto.hpp \
  CommProto/CommProto/CommProto/Exception.hpp \
  CommProto/sterownik_uC/tier_io/tier_io.h \
  CommProto/sterownik_uC/tier_io/tier_low_io/tier_low_io.h \
  CommProto/sterownik_uC/tier_io/tier_low_io/wabuf/wabuf.h \
  CommProto/sterownik_uC/tier_io/tier_low_io/portio/portio.h \
  CommProto/sterownik_uC/tier_io/tier_pck/tier_pck.h TIERctl.hpp \
  Zuchter/MapCreator.hpp Zuchter/CGraphics/Segment.hpp \
  Zuchter/CGraphics/CGraphics/Raster.hpp \
  Zuchter/CGraphics/CGraphics/CGraphics/Pixel.hpp \
  Zuchter/CGraphics/CGraphics/CGraphics/CGraphics/Exception.hpp \
  Zuchter/CGraphics/CGraphics/CGraphics/Size.hpp \
  Zuchter/CGraphics/CGraphics/CGraphics/CGraphics/Position.hpp \
  Zuchter/CGraphics/CGraphics/CGraphics/CGraphics/CGraphics/Exception.hpp \
  Zuchter/CGraphics/CGraphics/CGraphics/Position.hpp \
  Zuchter/CGraphics/CGraphics/CGraphics/Exception.hpp \
  Zuchter/CGraphics/CGraphics/Exception.hpp \
  Zuchter/VCCln/RasterDeserializer.hpp Zuchter/VCCln/CGraphics/Raster.hpp \
  Zuchter/VCCln/VCCln/Exception.hpp Zuchter/VCCln/Client.hpp \
  Zuchter/VCCln/Network/Connection.hpp \
  Zuchter/VCCln/Network/Network/Socket.hpp \
  Zuchter/VCCln/Network/Network/Network/Exception.hpp \
  Zuchter/VCCln/Network/Network/Exception.hpp \
  Zuchter/VCCln/Network/TCPClient.hpp \
  Zuchter/VCCln/Network/Network/Client.hpp \
  Zuchter/VCCln/Network/Network/Network/Connection.hpp \
  Zuchter/VCCln/Network/Network/Network/Address.hpp \
  Zuchter/Maps/Map2D.hpp Zuchter/Maps/Vectors/Space.hpp \
  Zuchter/Maps/Vectors/Vectors/Line.hpp \
  Zuchter/Maps/Vectors/Vectors/Vectors/Point.hpp \
  Zuchter/Maps/Vectors/Vectors/Vectors/Vectors/Exception.hpp \
  Zuchter/Maps/Vectors/Vectors/Vectors/Vector.hpp \
  Zuchter/Maps/Vectors/Vectors/Vectors/Vectors/Point.hpp \
  Zuchter/Maps/Vectors/Vectors/Vectors/Exception.hpp \
  Zuchter/Maps/Vectors/Vectors/Exception.hpp \
  Zuchter/Maps/Vectors/RefSpace.hpp \
  Zuchter/Maps/Vectors/Vectors/Point.hpp \
  Zuchter/Maps/Vectors/Vectors/Vector.hpp Zuchter/Maps/Maps/Exception.hpp \
  Zuchter/Maps/Maps/VectorImg.hpp Zuchter/Maps/Maps/Vectors/Space.hpp \
  Zuchter/Maps/Maps/Maps/Exception.hpp Zuchter/Maps/Vectorizer.hpp \
  Zuchter/Maps/CGraphics/Raster.hpp Zuchter/Maps/CGraphics/Size.hpp \
  Zuchter/Maps/Graphics/Raster.hpp \
  Zuchter/Maps/Graphics/Graphics/Exception.hpp \
  Zuchter/Maps/Graphics/Graphics/Pixel.hpp \
  Zuchter/Maps/Graphics/Graphics/Graphics/Exception.hpp \
  Zuchter/Maps/Graphics/Graphics/Size.hpp \
  Zuchter/Maps/Graphics/Graphics/Graphics/Position.hpp \
  Zuchter/Maps/Graphics/Graphics/Graphics/Graphics/Exception.hpp \
  Zuchter/Maps/Graphics/Graphics/Position.hpp \
  Zuchter/Maps/DeepthTransformer.hpp Zuchter/Maps/Graphics/Size.hpp \
  Zuchter/Maps/Vectors/Point.hpp Zuchter/Maps/Maps/Map2D.hpp \
  Zuchter/Path/GlobalMap.hpp Zuchter/Path/Maps/Map2D.hpp \
  Zuchter/Path/Path/Exception.hpp Zuchter/PathCreator.hpp \
  Zuchter/Path/Planner.hpp Zuchter/Path/Path/Plan.hpp \
  Zuchter/Path/Path/Path/Destination.hpp \
  Zuchter/Path/Path/Path/Vectors/Vector.hpp \
  Zuchter/Path/Path/Path/Exception.hpp Zuchter/Path/Path/GlobalMap.hpp \
  Zuchter/CommandCreator.hpp Zuchter/CommProto/TIERproto.hpp \
  Zuchter/Path/Plan.hpp Zuchter/Threads/Thread.hpp \
  Zuchter/Threads/Threads/Exception.hpp Zuchter/Threads/Lock.hpp \
  Zuchter/Threads/Threads/Mutex.hpp \
  Zuchter/Threads/Threads/Threads/Exception.hpp \
  Zuchter/Threads/Threads/GeneralLock.hpp \
  Zuchter/Threads/Threads/Threads/Mutex.hpp Zuchter/Threads/Mutex.hpp

TEST.o: TEST.cpp

