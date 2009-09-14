
CC=gcc

FLAGS_BASIC=-Wall 
CFLAGS_REL=$(FLAGS_BASIC) -O3 -DNDEBUG
CFLAGS_DEB=$(FLAGS_BASIC) -g3 -pg

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
	$(CC) $(CFLAGS_DEB) -o $(BIN_NAME) bitmap/bitmap.o histogram.o scale_raster.o stats.o main_raster.o TEST.o $(CLIBS)



make_common: histogram.o scale_raster.o stats.o main_raster.o
	$(MAKE) -C bitmap/ $(TARGET)


clean:
	$(MAKE) -C bitmap/ clean
	rm -f *.o
	rm -f $(BIN_NAME)
	rm -f gmon.out


histogram.o: histogram.c histogram.h raster.h bitmap/bitmap.h \
  main_raster.h stats.h scale_raster.h

scale_raster.o: scale_raster.c scale_raster.h main_raster.h \
  bitmap/bitmap.h

stats.o: stats.c stats.h raster.h bitmap/bitmap.h main_raster.h \
  histogram.h scale_raster.h

main_raster.o: main_raster.c raster.h bitmap/bitmap.h main_raster.h \
  stats.h histogram.h scale_raster.h

TEST.o: TEST.c raster.h bitmap/bitmap.h main_raster.h stats.h histogram.h \
  scale_raster.h

