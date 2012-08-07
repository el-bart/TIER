
CC=g++

FLAGS_BASIC=-Wall
CXXFLAGS_REL=$(FLAGS_BASIC) -O3 -DNDEBUG
CXXFLAGS_DEB=$(FLAGS_BASIC) -g3 -pg

LIBS_BASIC=
CXXLIBS_REL=$(LIBS_BASIC)
CXXLIBS_DEB=$(LIBS_BASIC)

BIN_NAME=TEST.out



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
	$(CC) $(CXXFLAGS_DEB) -o $(BIN_NAME)  ProcessRaster.o Histogram.o V4LCapture.o TEST.o $(CXXLIBS)



make_common: ProcessRaster.o Histogram.o V4LCapture.o



clean:
	rm -f *.o
	rm -f $(BIN_NAME)
	rm -f gmon.out


ProcessRaster.o: ProcessRaster.cpp Graphics/ProcessRaster.hpp \
  Graphics/Graphics/Raster.hpp Graphics/Graphics/Graphics/Exception.hpp \
  Graphics/Graphics/Graphics/Pixel.hpp \
  Graphics/Graphics/Graphics/Graphics/Exception.hpp \
  Graphics/Graphics/Graphics/Size.hpp \
  Graphics/Graphics/Graphics/Graphics/Position.hpp \
  Graphics/Graphics/Graphics/Graphics/Graphics/Exception.hpp \
  Graphics/Graphics/Graphics/Position.hpp Graphics/Graphics/Exception.hpp \
  Graphics/Histogram.hpp Graphics/Graphics/Pixel.hpp

Histogram.o: Histogram.cpp Graphics/Histogram.hpp \
  Graphics/Graphics/Pixel.hpp Graphics/Graphics/Graphics/Exception.hpp \
  Graphics/Graphics/Raster.hpp Graphics/Graphics/Graphics/Pixel.hpp \
  Graphics/Graphics/Graphics/Size.hpp \
  Graphics/Graphics/Graphics/Graphics/Exception.hpp \
  Graphics/Graphics/Graphics/Graphics/Position.hpp \
  Graphics/Graphics/Graphics/Graphics/Graphics/Exception.hpp \
  Graphics/Graphics/Graphics/Position.hpp Graphics/Graphics/Exception.hpp \
  Graphics/Size.hpp Graphics/Pixel.hpp Graphics/Position.hpp

V4LCapture.o: V4LCapture.cpp V4LCapture.hpp Graphics/Size.hpp \
  Graphics/Graphics/Exception.hpp Graphics/Graphics/Position.hpp \
  Graphics/Graphics/Graphics/Exception.hpp Graphics/Raster.hpp \
  Graphics/Graphics/Pixel.hpp Graphics/Graphics/Size.hpp

TEST.o: TEST.cpp Raster.hpp Graphics/Exception.hpp Graphics/Pixel.hpp \
  Graphics/Graphics/Exception.hpp Graphics/Size.hpp \
  Graphics/Graphics/Position.hpp Graphics/Graphics/Graphics/Exception.hpp \
  Graphics/Position.hpp ProcessRaster.hpp Graphics/Raster.hpp

