
CC=gcc

FLAGS_BASIC=-Wall 
CFLAGS_REL=$(FLAGS_BASIC) -O3 -DNDEBUG
CFLAGS_DEB=$(FLAGS_BASIC) -g3 -pg -DDMALLOC

LIBS_BASIC=
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
	$(CC) $(CFLAGS_DEB) -o $(BIN_NAME) tier_low_io/wabuf/wabuf.o tier_low_io/portio/portio.o tier_low_io/tier_low_io.o tier_pck/tier_pck.o tier_io.o TEST.o $(CLIBS)



make_common: tier_io.o
	$(MAKE) -C tier_low_io/ $(TARGET)
	$(MAKE) -C tier_pck/ $(TARGET)



clean:
	$(MAKE) -C tier_low_io/ clean
	$(MAKE) -C tier_pck/ clean
	rm -f *.o
	rm -f $(BIN_NAME)
	rm -f gmon.out


tier_io.o: tier_io.c tier_io.h tier_low_io/tier_low_io.h \
  tier_low_io/wabuf/wabuf.h tier_low_io/portio/portio.h \
  tier_pck/tier_pck.h

