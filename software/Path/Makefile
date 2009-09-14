
CC=g++

FLAGS_BASIC=-Wall
CXXFLAGS_REL=$(FLAGS_BASIC) -O3 -DNDEBUG
CXXFLAGS_DEB=$(FLAGS_BASIC) -g3 -pg

LIBS_BASIC=
CXXLIBS_REL=$(LIBS_BASIC)
CXXLIBS_DEB=$(LIBS_BASIC)

BIN_NAME=TEST.out
APP_NAME=none



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
	$(CC) $(CXXFLAGS_DEB) -o $(BIN_NAME) Maps/DeepthTransformer.o Maps/VectorImg.o Maps/CGraphics/Segment.o Maps/CGraphics/Raster.o Maps/CGraphics/bitmap/bitmap.o Maps/CGraphics/filter/histo_filter.o Maps/CGraphics/filter/main_filters.o Maps/CGraphics/filter/req_col_unif.o Maps/CGraphics/filter/raster/scale_raster.o Maps/CGraphics/filter/raster/histogram.o Maps/CGraphics/filter/raster/stats.o Maps/CGraphics/filter/raster/main_raster.o Maps/CGraphics/filter/denoise.o Maps/VectorImgFilter.o Maps/Map2D.o Maps/Graphics/Histogram.o Maps/Graphics/V4LCapture.o Maps/Graphics/ProcessRaster.o Maps/Vectorizer.o GlobalMap.o Planner.o TEST.o $(CXXLIBS)



make_common: GlobalMap.o Planner.o
	$(MAKE) -C Vectors/ $(TARGET)
	$(MAKE) -C Maps/ $(TARGET)



clean:
	$(MAKE) -C Vectors/ clean
	$(MAKE) -C Maps/ clean
	rm -f *.o
	rm -f $(BIN_NAME)
	rm -f $(APP_NAME)
	rm -f gmon.out


GlobalMap.o: GlobalMap.cpp GlobalMap.hpp Maps/Map2D.hpp \
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
  Path/Exception.hpp

Planner.o: Planner.cpp Vectors/Vector.hpp Vectors/Vectors/Point.hpp \
  Vectors/Vectors/Vectors/Exception.hpp Vectors/Vectors/Exception.hpp \
  Planner.hpp Path/Plan.hpp Path/Path/Destination.hpp \
  Path/Path/Vectors/Vector.hpp Path/Path/Exception.hpp Path/GlobalMap.hpp \
  Path/Maps/Map2D.hpp Path/Maps/Vectors/Space.hpp \
  Path/Maps/Vectors/Vectors/Line.hpp \
  Path/Maps/Vectors/Vectors/Vectors/Point.hpp \
  Path/Maps/Vectors/Vectors/Vectors/Vector.hpp \
  Path/Maps/Vectors/Vectors/Vectors/Exception.hpp \
  Path/Maps/Vectors/Vectors/Exception.hpp Path/Maps/Vectors/RefSpace.hpp \
  Path/Maps/Vectors/Vectors/Point.hpp \
  Path/Maps/Vectors/Vectors/Vector.hpp Path/Maps/Maps/Exception.hpp \
  Path/Maps/Maps/VectorImg.hpp Path/Maps/Maps/Vectors/Space.hpp \
  Path/Maps/Maps/Maps/Exception.hpp Path/Exception.hpp

TEST.o: TEST.cpp Destination.hpp Vectors/Vector.hpp \
  Vectors/Vectors/Point.hpp Vectors/Vectors/Vectors/Exception.hpp \
  Vectors/Vectors/Exception.hpp GlobalMap.hpp Maps/Map2D.hpp \
  Maps/Vectors/Space.hpp Maps/Vectors/Vectors/Line.hpp \
  Maps/Vectors/Vectors/Vectors/Point.hpp \
  Maps/Vectors/Vectors/Vectors/Vector.hpp \
  Maps/Vectors/Vectors/Vectors/Exception.hpp \
  Maps/Vectors/Vectors/Exception.hpp Maps/Vectors/RefSpace.hpp \
  Maps/Vectors/Vectors/Point.hpp Maps/Vectors/Vectors/Vector.hpp \
  Maps/Maps/Exception.hpp Maps/Maps/VectorImg.hpp \
  Maps/Maps/Vectors/Space.hpp Maps/Maps/Maps/Exception.hpp \
  Path/Exception.hpp Plan.hpp Path/Destination.hpp Planner.hpp \
  Path/Plan.hpp Path/GlobalMap.hpp Exception.hpp

