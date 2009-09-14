
CC=g++

FLAGS_BASIC=-Wall
CXXFLAGS_REL=$(FLAGS_BASIC) -O3 -DNDEBUG
CXXFLAGS_DEB=$(FLAGS_BASIC) -g3 -pg

LIBS_BASIC=
CXXLIBS_REL=$(LIBS_BASIC)
CXXLIBS_DEB=$(LIBS_BASIC)

BIN_NAME=TEST.out
#APP_NAME=vc_srv



all: rel

rel: CXXFLAGS=$(CXXFLAGS_REL)
rel: CXXLIBS=$(CXXLIBS_REL)
rel: TARGET=rel
rel: make_common

deb: CXXFLAGS=$(CXXFLAGS_DEB)
deb: CXXLIBS=$(CXXLIBS_DEB)
deb: TARGET=deb
deb: make_common

test: $(BIN_NAME)
$(BIN_NAME): CXXFLAGS=$(CXXFLAGS_DEB)
$(BIN_NAME): CXXLIBS=$(CXXLIBS_DEB)
$(BIN_NAME): deb TEST.o
	$(CC) $(CXXFLAGS_DEB) -o $(BIN_NAME) CGraphics/Segment.o CGraphics/Raster.o CGraphics/bitmap/bitmap.o CGraphics/filter/histo_filter.o CGraphics/filter/main_filters.o CGraphics/filter/req_col_unif.o CGraphics/filter/raster/scale_raster.o CGraphics/filter/raster/histogram.o CGraphics/filter/raster/stats.o CGraphics/filter/raster/main_raster.o CGraphics/filter/denoise.o Graphics/Histogram.o Graphics/V4LCapture.o Graphics/ProcessRaster.o VectorImg.o Vectorizer.o VectorImgFilter.o Map2D.o DeepthTransformer.o TEST.o $(CXXLIBS)



make_common: VectorImg.o Vectorizer.o VectorImgFilter.o Map2D.o DeepthTransformer.o
	$(MAKE) -C CGraphics/ $(TARGET)
	$(MAKE) -C Vectors/ $(TARGET)
	$(MAKE) -C Graphics/ $(TARGET)



clean:
	$(MAKE) -C CGraphics/ clean
	$(MAKE) -C Vectors/ clean
	$(MAKE) -C Graphics/ clean
	rm -f *.o
	rm -f $(BIN_NAME)
	rm -f $(APP_NAME)
	rm -f gmon.out


VectorImg.o: VectorImg.cpp VectorImg.hpp Vectors/Space.hpp \
  Vectors/Vectors/Line.hpp Vectors/Vectors/Vectors/Point.hpp \
  Vectors/Vectors/Vectors/Vectors/Exception.hpp \
  Vectors/Vectors/Vectors/Vector.hpp \
  Vectors/Vectors/Vectors/Vectors/Point.hpp \
  Vectors/Vectors/Vectors/Exception.hpp Vectors/Vectors/Exception.hpp \
  Maps/Exception.hpp

Vectorizer.o: Vectorizer.cpp Vectorizer.hpp CGraphics/Raster.hpp \
  CGraphics/CGraphics/Pixel.hpp \
  CGraphics/CGraphics/CGraphics/Exception.hpp \
  CGraphics/CGraphics/Size.hpp CGraphics/CGraphics/CGraphics/Position.hpp \
  CGraphics/CGraphics/CGraphics/CGraphics/Exception.hpp \
  CGraphics/CGraphics/Position.hpp CGraphics/CGraphics/Exception.hpp \
  CGraphics/Size.hpp Vectors/Space.hpp Vectors/Vectors/Line.hpp \
  Vectors/Vectors/Vectors/Point.hpp \
  Vectors/Vectors/Vectors/Vectors/Exception.hpp \
  Vectors/Vectors/Vectors/Vector.hpp \
  Vectors/Vectors/Vectors/Vectors/Point.hpp \
  Vectors/Vectors/Vectors/Exception.hpp Vectors/Vectors/Exception.hpp \
  Graphics/Raster.hpp Graphics/Graphics/Exception.hpp \
  Graphics/Graphics/Pixel.hpp Graphics/Graphics/Graphics/Exception.hpp \
  Graphics/Graphics/Size.hpp Graphics/Graphics/Graphics/Position.hpp \
  Graphics/Graphics/Graphics/Graphics/Exception.hpp \
  Graphics/Graphics/Position.hpp Maps/Exception.hpp Maps/VectorImg.hpp \
  Maps/Vectors/Space.hpp Maps/Maps/Exception.hpp

VectorImgFilter.o: VectorImgFilter.cpp VectorImgFilter.hpp \
  Maps/VectorImg.hpp Maps/Vectors/Space.hpp Maps/Vectors/Vectors/Line.hpp \
  Maps/Vectors/Vectors/Vectors/Point.hpp \
  Maps/Vectors/Vectors/Vectors/Vectors/Exception.hpp \
  Maps/Vectors/Vectors/Vectors/Vector.hpp \
  Maps/Vectors/Vectors/Vectors/Vectors/Point.hpp \
  Maps/Vectors/Vectors/Vectors/Exception.hpp \
  Maps/Vectors/Vectors/Exception.hpp Maps/Maps/Exception.hpp \
  Exception.hpp

Map2D.o: Map2D.cpp Map2D.hpp Vectors/Space.hpp Vectors/Vectors/Line.hpp \
  Vectors/Vectors/Vectors/Point.hpp \
  Vectors/Vectors/Vectors/Vectors/Exception.hpp \
  Vectors/Vectors/Vectors/Vector.hpp \
  Vectors/Vectors/Vectors/Vectors/Point.hpp \
  Vectors/Vectors/Vectors/Exception.hpp Vectors/Vectors/Exception.hpp \
  Vectors/RefSpace.hpp Vectors/Vectors/Point.hpp \
  Vectors/Vectors/Vector.hpp Maps/Exception.hpp Maps/VectorImg.hpp \
  Maps/Vectors/Space.hpp Maps/Maps/Exception.hpp

DeepthTransformer.o: DeepthTransformer.cpp DeepthTransformer.hpp \
  Graphics/Size.hpp Graphics/Graphics/Exception.hpp \
  Graphics/Graphics/Position.hpp Graphics/Graphics/Graphics/Exception.hpp \
  Vectors/Point.hpp Vectors/Vectors/Exception.hpp Maps/VectorImg.hpp \
  Maps/Vectors/Space.hpp Maps/Vectors/Vectors/Line.hpp \
  Maps/Vectors/Vectors/Vectors/Point.hpp \
  Maps/Vectors/Vectors/Vectors/Vector.hpp \
  Maps/Vectors/Vectors/Vectors/Vectors/Point.hpp \
  Maps/Vectors/Vectors/Vectors/Vectors/Exception.hpp \
  Maps/Vectors/Vectors/Vectors/Exception.hpp \
  Maps/Vectors/Vectors/Exception.hpp Maps/Maps/Exception.hpp \
  Maps/Map2D.hpp Maps/Vectors/RefSpace.hpp Maps/Vectors/Vectors/Point.hpp \
  Maps/Vectors/Vectors/Vector.hpp Maps/Maps/VectorImg.hpp

TEST.o: TEST.cpp CGraphics/Raster.hpp CGraphics/CGraphics/Pixel.hpp \
  CGraphics/CGraphics/CGraphics/Exception.hpp \
  CGraphics/CGraphics/Size.hpp CGraphics/CGraphics/CGraphics/Position.hpp \
  CGraphics/CGraphics/CGraphics/CGraphics/Exception.hpp \
  CGraphics/CGraphics/Position.hpp CGraphics/CGraphics/Exception.hpp \
  CGraphics/Segment.hpp CGraphics/CGraphics/Raster.hpp Vectors/Line.hpp \
  Vectors/Vectors/Point.hpp Vectors/Vectors/Vectors/Exception.hpp \
  Vectors/Vectors/Vector.hpp Vectors/Vectors/Vectors/Point.hpp \
  Vectors/Vectors/Exception.hpp Vectors/Point.hpp Vectorizer.hpp \
  CGraphics/Size.hpp Vectors/Space.hpp Vectors/Vectors/Line.hpp \
  Graphics/Raster.hpp Graphics/Graphics/Exception.hpp \
  Graphics/Graphics/Pixel.hpp Graphics/Graphics/Graphics/Exception.hpp \
  Graphics/Graphics/Size.hpp Graphics/Graphics/Graphics/Position.hpp \
  Graphics/Graphics/Graphics/Graphics/Exception.hpp \
  Graphics/Graphics/Position.hpp Maps/Exception.hpp Maps/VectorImg.hpp \
  Maps/Vectors/Space.hpp Maps/Maps/Exception.hpp VectorImgFilter.hpp \
  DeepthTransformer.hpp Graphics/Size.hpp Maps/Map2D.hpp \
  Maps/Vectors/RefSpace.hpp Maps/Vectors/Vectors/Point.hpp \
  Maps/Vectors/Vectors/Vector.hpp Maps/Maps/VectorImg.hpp

