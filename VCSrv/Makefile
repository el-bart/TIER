
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
	$(CC) $(CXXFLAGS_DEB) -o $(BIN_NAME) Graphics/Histogram.o Graphics/V4LCapture.o Graphics/ProcessRaster.o Network/Server.o Network/TCPServer.o Network/TCPClient.o Network/Resolver.o Connection.o RasterSerializer.o Server.o ServerApp.o TEST.o $(CXXLIBS)



make_common: Connection.o RasterSerializer.o Server.o ServerApp.o
	$(MAKE) -C Graphics/ $(TARGET)
	$(MAKE) -C Network/ $(TARGET)
	$(CC) $(CXXFLAGS) -o $(APP_NAME) Graphics/Histogram.o Graphics/V4LCapture.o Graphics/ProcessRaster.o Network/Server.o Network/TCPServer.o Network/TCPClient.o Network/Resolver.o Connection.o RasterSerializer.o Server.o ServerApp.o $(CXXLIBS)


clean:
	$(MAKE) -C Graphics/ clean
	$(MAKE) -C Network/ clean
	rm -f *.o
	rm -f $(BIN_NAME)
	rm -f $(APP_NAME)
	rm -f gmon.out


Connection.o: Connection.cpp Connection.hpp VCSrv/Exception.hpp \
  Exception.hpp

RasterSerializer.o: RasterSerializer.cpp Graphics/Size.hpp \
  Graphics/Graphics/Exception.hpp Graphics/Graphics/Position.hpp \
  Graphics/Graphics/Graphics/Exception.hpp Graphics/Pixel.hpp \
  Graphics/Position.hpp RasterSerializer.hpp Graphics/Raster.hpp \
  Graphics/Graphics/Pixel.hpp Graphics/Graphics/Size.hpp \
  VCSrv/Exception.hpp Exception.hpp

Server.o: Server.cpp Server.hpp VCSrv/RasterSerializer.hpp \
  VCSrv/Graphics/Raster.hpp VCSrv/Graphics/Graphics/Exception.hpp \
  VCSrv/Graphics/Graphics/Pixel.hpp \
  VCSrv/Graphics/Graphics/Graphics/Exception.hpp \
  VCSrv/Graphics/Graphics/Size.hpp \
  VCSrv/Graphics/Graphics/Graphics/Position.hpp \
  VCSrv/Graphics/Graphics/Graphics/Graphics/Exception.hpp \
  VCSrv/Graphics/Graphics/Position.hpp VCSrv/VCSrv/Exception.hpp \
  VCSrv/Connection.hpp VCSrv/Exception.hpp Exception.hpp

ServerApp.o: ServerApp.cpp RasterSerializer.hpp Graphics/Raster.hpp \
  Graphics/Graphics/Exception.hpp Graphics/Graphics/Pixel.hpp \
  Graphics/Graphics/Graphics/Exception.hpp Graphics/Graphics/Size.hpp \
  Graphics/Graphics/Graphics/Position.hpp \
  Graphics/Graphics/Graphics/Graphics/Exception.hpp \
  Graphics/Graphics/Position.hpp VCSrv/Exception.hpp Exception.hpp \
  Graphics/V4LCapture.hpp Graphics/Graphics/Raster.hpp \
  Network/TCPServer.hpp Network/Network/Server.hpp \
  Network/Network/Network/Socket.hpp \
  Network/Network/Network/Network/Exception.hpp \
  Network/Network/Network/Connection.hpp \
  Network/Network/Network/Network/Socket.hpp \
  Network/Network/Network/Address.hpp \
  Network/Network/Network/Exception.hpp Network/Network/Connection.hpp \
  Network/Network/Exception.hpp

