
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
	$(CC) $(CFLAGS_DEB) -o $(BIN_NAME) tier_low_io/wabuf/wabuf.o tier_low_io/portio/portio.o tier_low_io/tier_low_io.o tier_pck.o TEST.o $(CLIBS)



make_common: tier_pck.o
	$(MAKE) -C tier_low_io/ $(TARGET)



clean:
	$(MAKE) -C tier_low_io/ clean
	rm -f *.o
	rm -f $(BIN_NAME)
	rm -f gmon.out


tier_pck.o: tier_pck.c tier_pck.h

