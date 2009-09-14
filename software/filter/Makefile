
CC=gcc

FLAGS_BASIC=-Wall 
CFLAGS_REL=$(FLAGS_BASIC) -O3 -DNDEBUG
CFLAGS_DEB=$(FLAGS_BASIC) -g3 -pg -DDMALLOC

LIBS_BASIC=-lm
CLIBS_REL=$(LIBS_BASIC)
CLIBS_DEB=$(LIBS_BASIC) -lefence -ldmalloc

BIN_NAME=TEST.out



all: rel

rel: CFLAGS=$(CFLAGS_REL)
rel: CLIBS=$(CLIBS_REL)
rel: TARGET=rel
rel: make_common

deb: CFLAGS=$(CFLAGS_DEB)
deb: CLIBS=$(CLIBS_DEB)
deb: TARGET=deb
deb: make_common

test: CFLAGS=$(CFLAGS_DEB)
test: CLIBS=$(CLIBS_DEB)
test: deb TEST.o
	$(CC) $(CFLAGS_DEB) -o $(BIN_NAME) raster/scale_raster.o raster/histogram.o raster/stats.o raster/bitmap/bitmap.o raster/main_raster.o denoise.o histo_filter.o req_col_unif.o main_filters.o TEST.o $(CLIBS)



make_common: denoise.o histo_filter.o req_col_unif.o main_filters.o
	$(MAKE) -C raster/ $(TARGET)



clean:
	$(MAKE) -C raster/ clean
	rm -f *.o
	rm -f $(BIN_NAME)
	rm -f gmon.out


denoise.o: denoise.c denoise.h raster/raster.h raster/bitmap/bitmap.h \
  raster/main_raster.h raster/stats.h raster/histogram.h \
  raster/scale_raster.h

histo_filter.o: histo_filter.c main_filters.h raster/raster.h \
  raster/bitmap/bitmap.h raster/main_raster.h raster/stats.h \
  raster/histogram.h raster/scale_raster.h req_col_unif.h

req_col_unif.o: req_col_unif.c req_col_unif.h raster/raster.h \
  raster/bitmap/bitmap.h raster/main_raster.h raster/stats.h \
  raster/histogram.h raster/scale_raster.h

main_filters.o: main_filters.c main_filters.h raster/raster.h \
  raster/bitmap/bitmap.h raster/main_raster.h raster/stats.h \
  raster/histogram.h raster/scale_raster.h req_col_unif.h

TEST.o: TEST.c filter.h raster/raster.h raster/bitmap/bitmap.h \
  raster/main_raster.h raster/stats.h raster/histogram.h \
  raster/scale_raster.h main_filters.h req_col_unif.h histo_filter.h \
  denoise.h

