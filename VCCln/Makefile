
CC=g++

FLAGS_BASIC=-Wall
CXXFLAGS_REL=$(FLAGS_BASIC) -O3 -DNDEBUG
CXXFLAGS_DEB=$(FLAGS_BASIC) -g3 -pg

LIBS_BASIC=
CXXLIBS_REL=$(LIBS_BASIC)
CXXLIBS_DEB=$(LIBS_BASIC)

BIN_NAME=TEST.out
APP_NAME=vc_srv



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
	$(CC) $(CXXFLAGS_DEB) -o $(BIN_NAME) CGraphics/Segment.o CGraphics/Raster.o CGraphics/bitmap/bitmap.o CGraphics/filter/histo_filter.o CGraphics/filter/main_filters.o CGraphics/filter/req_col_unif.o CGraphics/filter/raster/scale_raster.o CGraphics/filter/raster/histogram.o CGraphics/filter/raster/stats.o CGraphics/filter/raster/main_raster.o CGraphics/filter/denoise.o Network/Server.o Network/TCPServer.o Network/TCPClient.o Network/Resolver.o RasterDeserializer.o TEST.o $(CXXLIBS)



make_common: RasterDeserializer.o
	$(MAKE) -C CGraphics/ $(TARGET)
	$(MAKE) -C Network/ $(TARGET)



clean:
	$(MAKE) -C CGraphics/ clean
	$(MAKE) -C Network/ clean
	rm -f *.o
	rm -f $(BIN_NAME)
	rm -f $(APP_NAME)
	rm -f gmon.out


RasterDeserializer.o: RasterDeserializer.cpp RasterDeserializer.hpp \
  CGraphics/Raster.hpp CGraphics/CGraphics/Pixel.hpp \
  CGraphics/CGraphics/CGraphics/Exception.hpp \
  CGraphics/CGraphics/Size.hpp CGraphics/CGraphics/CGraphics/Position.hpp \
  CGraphics/CGraphics/CGraphics/CGraphics/Exception.hpp \
  CGraphics/CGraphics/Position.hpp CGraphics/CGraphics/Exception.hpp \
  VCCln/Exception.hpp Exception.hpp

TEST.o: TEST.cpp CGraphics/Raster.hpp CGraphics/CGraphics/Pixel.hpp \
  CGraphics/CGraphics/CGraphics/Exception.hpp \
  CGraphics/CGraphics/Size.hpp CGraphics/CGraphics/CGraphics/Position.hpp \
  CGraphics/CGraphics/CGraphics/CGraphics/Exception.hpp \
  CGraphics/CGraphics/Position.hpp CGraphics/CGraphics/Exception.hpp \
  CGraphics/Exception.hpp Client.hpp Network/Connection.hpp \
  Network/Network/Socket.hpp Network/Network/Network/Exception.hpp \
  Network/Network/Exception.hpp Network/TCPClient.hpp \
  Network/Network/Client.hpp Network/Network/Network/Connection.hpp \
  Network/Network/Network/Address.hpp VCCln/Exception.hpp \
  RasterDeserializer.hpp Exception.hpp

