
CC=g++

FLAGS_BASIC=-Wall
CXXFLAGS_REL=$(FLAGS_BASIC) -O3 -DNDEBUG
CXXFLAGS_DEB=$(FLAGS_BASIC) -g3 -pg

LIBS_BASIC=-lpthread
CXXLIBS_REL=$(LIBS_BASIC)
CXXLIBS_DEB=$(LIBS_BASIC)

BIN_NAME1=TEST.out
BIN_NAME2=TEST_RESEND.out
APP_NAME=debug_win

GTK_FLAGS=$(shell pkg-config gtk+-2.0 --cflags --libs)



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
test: deb TEST.o TEST_RESEND.o
	$(CC) $(CXXFLAGS_DEB) -o $(BIN_NAME1) CGraphics/Raster.o CGraphics/Segment.o CGraphics/filter/raster/stats.o CGraphics/filter/raster/main_raster.o CGraphics/filter/raster/scale_raster.o CGraphics/filter/raster/bitmap/bitmap.o CGraphics/filter/raster/histogram.o CGraphics/filter/req_col_unif.o CGraphics/filter/denoise.o CGraphics/filter/histo_filter.o CGraphics/filter/main_filters.o Network/Resolver.o Network/Server.o Network/TCPServer.o Network/TCPClient.o Graphics/ProcessRaster.o Graphics/V4LCapture.o Graphics/Histogram.o VCCln/RasterDeserializer.o VCSrv/Connection.o VCSrv/RasterSerializer.o Maps/VectorImg.o Maps/VectorImgFilter.o Maps/DeepthTransformer.o Maps/Vectorizer.o DebugTransmit.o DebugRecieve.o TEST.o $(CXXLIBS)
	$(CC) $(CXXFLAGS_DEB) -o $(BIN_NAME2) CGraphics/Raster.o CGraphics/Segment.o CGraphics/filter/raster/stats.o CGraphics/filter/raster/main_raster.o CGraphics/filter/raster/scale_raster.o CGraphics/filter/raster/bitmap/bitmap.o CGraphics/filter/raster/histogram.o CGraphics/filter/req_col_unif.o CGraphics/filter/denoise.o CGraphics/filter/histo_filter.o CGraphics/filter/main_filters.o Network/Resolver.o Network/Server.o Network/TCPServer.o Network/TCPClient.o Graphics/ProcessRaster.o Graphics/V4LCapture.o Graphics/Histogram.o VCCln/RasterDeserializer.o VCSrv/Connection.o VCSrv/RasterSerializer.o Maps/VectorImg.o Maps/VectorImgFilter.o Maps/DeepthTransformer.o Maps/Vectorizer.o DebugTransmit.o DebugRecieve.o TEST_RESEND.o $(CXXLIBS)


make_common: DebugTransmit.o DebugRecieve.o DebugWin.o
	$(MAKE) -C CGraphics/ $(TARGET)
	$(MAKE) -C Network/ $(TARGET)
	$(MAKE) -C Graphics/ $(TARGET)
	$(MAKE) -C VCCln/ $(TARGET)
	$(MAKE) -C VCSrv/ $(TARGET)
	$(MAKE) -C Vectors/ $(TARGET)
	$(MAKE) -C Maps/ $(TARGET)
	$(MAKE) -C bitmap/ $(TARGET)
	$(MAKE) -C raster/ $(TARGET)
	$(CC) $(CXXFLAGS) -o $(APP_NAME) CGraphics/Raster.o CGraphics/Segment.o CGraphics/filter/raster/stats.o CGraphics/filter/raster/main_raster.o CGraphics/filter/raster/scale_raster.o CGraphics/filter/raster/bitmap/bitmap.o CGraphics/filter/raster/histogram.o CGraphics/filter/req_col_unif.o CGraphics/filter/denoise.o CGraphics/filter/histo_filter.o CGraphics/filter/main_filters.o Network/Resolver.o Network/Server.o Network/TCPServer.o Network/TCPClient.o Graphics/ProcessRaster.o Graphics/V4LCapture.o Graphics/Histogram.o VCCln/RasterDeserializer.o VCSrv/Connection.o VCSrv/RasterSerializer.o Maps/VectorImg.o Maps/VectorImgFilter.o Maps/DeepthTransformer.o Maps/Vectorizer.o DebugTransmit.o DebugRecieve.o DebugWin.o $(CXXLIBS) $(GTK_FLAGS)

clean:
	$(MAKE) -C CGraphics/ clean
	$(MAKE) -C Network/ clean
	$(MAKE) -C Graphics/ clean
	$(MAKE) -C VCCln/ clean
	$(MAKE) -C VCSrv/ clean
	$(MAKE) -C Vectors/ clean
	$(MAKE) -C Maps/ clean
	$(MAKE) -C bitmap/ clean
	$(MAKE) -C raster/ clean
	rm -f *.o
	rm -f $(BIN_NAME1)
	rm -f $(BIN_NAME2)
	rm -f $(APP_NAME)
	rm -f gmon.out


DebugTransmit.o: DebugTransmit.cpp VCSrv/RasterSerializer.hpp \
  VCSrv/Graphics/Raster.hpp VCSrv/Graphics/Graphics/Exception.hpp \
  VCSrv/Graphics/Graphics/Pixel.hpp \
  VCSrv/Graphics/Graphics/Graphics/Exception.hpp \
  VCSrv/Graphics/Graphics/Size.hpp \
  VCSrv/Graphics/Graphics/Graphics/Position.hpp \
  VCSrv/Graphics/Graphics/Graphics/Graphics/Exception.hpp \
  VCSrv/Graphics/Graphics/Position.hpp VCSrv/VCSrv/Exception.hpp \
  Network/TCPClient.hpp Network/Network/Client.hpp \
  Network/Network/Network/Connection.hpp \
  Network/Network/Network/Network/Socket.hpp \
  Network/Network/Network/Network/Network/Exception.hpp \
  Network/Network/Network/Network/Exception.hpp \
  Network/Network/Network/Address.hpp \
  Network/Network/Network/Exception.hpp DebugTransmit.hpp \
  Graphics/Raster.hpp CGraphics/Raster.hpp CGraphics/CGraphics/Pixel.hpp \
  CGraphics/CGraphics/CGraphics/Exception.hpp \
  CGraphics/CGraphics/Size.hpp CGraphics/CGraphics/CGraphics/Position.hpp \
  CGraphics/CGraphics/CGraphics/CGraphics/Exception.hpp \
  CGraphics/CGraphics/Position.hpp CGraphics/CGraphics/Exception.hpp \
  Maps/VectorImg.hpp Maps/Vectors/Space.hpp Maps/Vectors/Vectors/Line.hpp \
  Maps/Vectors/Vectors/Vectors/Point.hpp \
  Maps/Vectors/Vectors/Vectors/Vectors/Exception.hpp \
  Maps/Vectors/Vectors/Vectors/Vector.hpp \
  Maps/Vectors/Vectors/Vectors/Vectors/Point.hpp \
  Maps/Vectors/Vectors/Vectors/Exception.hpp \
  Maps/Vectors/Vectors/Exception.hpp Maps/Maps/Exception.hpp

DebugRecieve.o: DebugRecieve.cpp DebugRecieve.hpp \
  VCCln/RasterDeserializer.hpp VCCln/CGraphics/Raster.hpp \
  VCCln/CGraphics/CGraphics/Pixel.hpp \
  VCCln/CGraphics/CGraphics/CGraphics/Exception.hpp \
  VCCln/CGraphics/CGraphics/Size.hpp \
  VCCln/CGraphics/CGraphics/CGraphics/Position.hpp \
  VCCln/CGraphics/CGraphics/CGraphics/CGraphics/Exception.hpp \
  VCCln/CGraphics/CGraphics/Position.hpp \
  VCCln/CGraphics/CGraphics/Exception.hpp VCCln/VCCln/Exception.hpp \
  Network/TCPServer.hpp Network/Network/Server.hpp \
  Network/Network/Network/Socket.hpp \
  Network/Network/Network/Network/Exception.hpp \
  Network/Network/Network/Connection.hpp \
  Network/Network/Network/Network/Socket.hpp \
  Network/Network/Network/Address.hpp \
  Network/Network/Network/Exception.hpp Network/Network/Connection.hpp \
  Network/Network/Exception.hpp Threads/Thread.hpp \
  Threads/Threads/Exception.hpp DebugWin/RasterSwapper.hpp \
  DebugWin/Threads/Mutex.hpp DebugWin/Threads/Threads/Exception.hpp \
  DebugWin/Threads/Lock.hpp DebugWin/Threads/Threads/Mutex.hpp \
  DebugWin/Threads/Threads/GeneralLock.hpp \
  DebugWin/Threads/Threads/Threads/Mutex.hpp \
  DebugWin/Threads/Threads/Threads/Exception.hpp DebugWin/raster/raster.h \
  DebugWin/raster/bitmap/bitmap.h DebugWin/raster/main_raster.h \
  DebugWin/raster/stats.h DebugWin/raster/raster.h \
  DebugWin/raster/histogram.h DebugWin/raster/scale_raster.h

TEST.o: TEST.cpp VCCln/Exception.hpp VCSrv/Exception.hpp \
  DebugTransmit.hpp Network/TCPClient.hpp Network/Network/Client.hpp \
  Network/Network/Network/Connection.hpp \
  Network/Network/Network/Network/Socket.hpp \
  Network/Network/Network/Network/Network/Exception.hpp \
  Network/Network/Network/Network/Exception.hpp \
  Network/Network/Network/Address.hpp \
  Network/Network/Network/Exception.hpp Graphics/Raster.hpp \
  Graphics/Graphics/Exception.hpp Graphics/Graphics/Pixel.hpp \
  Graphics/Graphics/Graphics/Exception.hpp Graphics/Graphics/Size.hpp \
  Graphics/Graphics/Graphics/Position.hpp \
  Graphics/Graphics/Graphics/Graphics/Exception.hpp \
  Graphics/Graphics/Position.hpp CGraphics/Raster.hpp \
  CGraphics/CGraphics/Pixel.hpp \
  CGraphics/CGraphics/CGraphics/Exception.hpp \
  CGraphics/CGraphics/Size.hpp CGraphics/CGraphics/CGraphics/Position.hpp \
  CGraphics/CGraphics/CGraphics/CGraphics/Exception.hpp \
  CGraphics/CGraphics/Position.hpp CGraphics/CGraphics/Exception.hpp \
  Maps/VectorImg.hpp Maps/Vectors/Space.hpp Maps/Vectors/Vectors/Line.hpp \
  Maps/Vectors/Vectors/Vectors/Point.hpp \
  Maps/Vectors/Vectors/Vectors/Vectors/Exception.hpp \
  Maps/Vectors/Vectors/Vectors/Vector.hpp \
  Maps/Vectors/Vectors/Vectors/Vectors/Point.hpp \
  Maps/Vectors/Vectors/Vectors/Exception.hpp \
  Maps/Vectors/Vectors/Exception.hpp Maps/Maps/Exception.hpp

TEST_RESEND.o: TEST_RESEND.cpp VCCln/Client.hpp \
  VCCln/Network/Connection.hpp VCCln/Network/Network/Socket.hpp \
  VCCln/Network/Network/Network/Exception.hpp \
  VCCln/Network/Network/Exception.hpp VCCln/Network/TCPClient.hpp \
  VCCln/Network/Network/Client.hpp \
  VCCln/Network/Network/Network/Connection.hpp \
  VCCln/Network/Network/Network/Address.hpp VCCln/VCCln/Exception.hpp \
  VCCln/RasterDeserializer.hpp VCCln/CGraphics/Raster.hpp \
  VCCln/CGraphics/CGraphics/Pixel.hpp \
  VCCln/CGraphics/CGraphics/CGraphics/Exception.hpp \
  VCCln/CGraphics/CGraphics/Size.hpp \
  VCCln/CGraphics/CGraphics/CGraphics/Position.hpp \
  VCCln/CGraphics/CGraphics/CGraphics/CGraphics/Exception.hpp \
  VCCln/CGraphics/CGraphics/Position.hpp \
  VCCln/CGraphics/CGraphics/Exception.hpp VCCln/Exception.hpp \
  VCSrv/Exception.hpp DebugTransmit.hpp Network/TCPClient.hpp \
  Graphics/Raster.hpp Graphics/Graphics/Exception.hpp \
  Graphics/Graphics/Pixel.hpp Graphics/Graphics/Graphics/Exception.hpp \
  Graphics/Graphics/Size.hpp Graphics/Graphics/Graphics/Position.hpp \
  Graphics/Graphics/Graphics/Graphics/Exception.hpp \
  Graphics/Graphics/Position.hpp CGraphics/Raster.hpp Maps/VectorImg.hpp \
  Maps/Vectors/Space.hpp Maps/Vectors/Vectors/Line.hpp \
  Maps/Vectors/Vectors/Vectors/Point.hpp \
  Maps/Vectors/Vectors/Vectors/Vectors/Exception.hpp \
  Maps/Vectors/Vectors/Vectors/Vector.hpp \
  Maps/Vectors/Vectors/Vectors/Vectors/Point.hpp \
  Maps/Vectors/Vectors/Vectors/Exception.hpp \
  Maps/Vectors/Vectors/Exception.hpp Maps/Maps/Exception.hpp

DebugWin.o: DebugWin.cpp Threads/Mutex.hpp Threads/Threads/Exception.hpp \
  DebugRecieve.hpp VCCln/RasterDeserializer.hpp \
  VCCln/CGraphics/Raster.hpp VCCln/CGraphics/CGraphics/Pixel.hpp \
  VCCln/CGraphics/CGraphics/CGraphics/Exception.hpp \
  VCCln/CGraphics/CGraphics/Size.hpp \
  VCCln/CGraphics/CGraphics/CGraphics/Position.hpp \
  VCCln/CGraphics/CGraphics/CGraphics/CGraphics/Exception.hpp \
  VCCln/CGraphics/CGraphics/Position.hpp \
  VCCln/CGraphics/CGraphics/Exception.hpp VCCln/VCCln/Exception.hpp \
  Network/TCPServer.hpp Network/Network/Server.hpp \
  Network/Network/Network/Socket.hpp \
  Network/Network/Network/Network/Exception.hpp \
  Network/Network/Network/Connection.hpp \
  Network/Network/Network/Network/Socket.hpp \
  Network/Network/Network/Address.hpp \
  Network/Network/Network/Exception.hpp Network/Network/Connection.hpp \
  Network/Network/Exception.hpp Threads/Thread.hpp \
  DebugWin/RasterSwapper.hpp DebugWin/Threads/Mutex.hpp \
  DebugWin/Threads/Lock.hpp DebugWin/Threads/Threads/Mutex.hpp \
  DebugWin/Threads/Threads/GeneralLock.hpp \
  DebugWin/Threads/Threads/Threads/Mutex.hpp \
  DebugWin/Threads/Threads/Threads/Exception.hpp \
  DebugWin/Threads/Threads/Exception.hpp DebugWin/raster/raster.h \
  DebugWin/raster/bitmap/bitmap.h DebugWin/raster/main_raster.h \
  DebugWin/raster/stats.h DebugWin/raster/raster.h \
  DebugWin/raster/histogram.h DebugWin/raster/scale_raster.h \
  RasterSwapper.hpp bitmap/bitmap.h raster/raster.h
	$(CC) $(CXXFLAGS) -c DebugWin.cpp $(GTK_FLAGS)

