
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
	$(CC) $(CXXFLAGS_DEB) -o $(BIN_NAME) filter/histo_filter.o filter/main_filters.o filter/req_col_unif.o filter/raster/scale_raster.o filter/raster/histogram.o filter/raster/stats.o filter/raster/bitmap/bitmap.o filter/raster/main_raster.o filter/denoise.o Raster.o Segment.o TEST.o $(CXXLIBS)



make_common: Raster.o Segment.o
	$(MAKE) -C segment/ $(TARGET)
	$(MAKE) -C filter/ $(TARGET)
	$(MAKE) -C raster/ $(TARGET)
	$(MAKE) -C bitmap/ $(TARGET)



clean:
	$(MAKE) -C segment/ clean
	$(MAKE) -C filter/ clean
	$(MAKE) -C raster/ clean
	$(MAKE) -C bitmap/ clean
	rm -f *.o
	rm -f $(BIN_NAME)
	rm -f gmon.out


Raster.o: Raster.cpp CGraphics/Exception.hpp Raster.hpp \
  CGraphics/Pixel.hpp CGraphics/CGraphics/Exception.hpp \
  CGraphics/Size.hpp CGraphics/CGraphics/Position.hpp \
  CGraphics/CGraphics/CGraphics/Exception.hpp CGraphics/Position.hpp \
  bitmap/bitmap.h raster/raster.h raster/bitmap/bitmap.h \
  raster/main_raster.h raster/stats.h raster/histogram.h \
  raster/scale_raster.h filter/filter.h filter/raster/raster.h \
  filter/main_filters.h filter/req_col_unif.h filter/histo_filter.h \
  filter/raster/histogram.h filter/denoise.h

Segment.o: Segment.cpp CGraphics/Exception.hpp Segment.hpp \
  CGraphics/Raster.hpp CGraphics/CGraphics/Pixel.hpp \
  CGraphics/CGraphics/CGraphics/Exception.hpp \
  CGraphics/CGraphics/Size.hpp CGraphics/CGraphics/CGraphics/Position.hpp \
  CGraphics/CGraphics/CGraphics/CGraphics/Exception.hpp \
  CGraphics/CGraphics/Position.hpp CGraphics/CGraphics/Exception.hpp \
  segment/Helper.hpp segment/image.h segment/disjoint-set.h \
  segment/segment-graph.h segment/Exception.hpp segment/segment-image.h \
  segment/misc.h segment/filter.h segment/convolve.h segment/imconv.h \
  segment/imutil.h segment/Raster.hpp segment/Segment.hpp

TEST.o: TEST.cpp CGraphics/Exception.hpp CGraphics/Raster.hpp \
  CGraphics/CGraphics/Pixel.hpp \
  CGraphics/CGraphics/CGraphics/Exception.hpp \
  CGraphics/CGraphics/Size.hpp CGraphics/CGraphics/CGraphics/Position.hpp \
  CGraphics/CGraphics/CGraphics/CGraphics/Exception.hpp \
  CGraphics/CGraphics/Position.hpp CGraphics/CGraphics/Exception.hpp \
  CGraphics/Segment.hpp CGraphics/CGraphics/Raster.hpp

