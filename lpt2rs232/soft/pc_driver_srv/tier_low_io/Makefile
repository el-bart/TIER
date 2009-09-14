
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
	$(CC) $(CFLAGS_DEB) -o $(BIN_NAME) portio/portio.o wabuf/wabuf.o tier_low_io.o TEST.o $(CLIBS)



make_common: tier_low_io.o
	$(MAKE) -C portio/ $(TARGET)
	$(MAKE) -C wabuf/ $(TARGET)



clean:
	$(MAKE) -C portio/ clean
	$(MAKE) -C wabuf/ clean
	rm -f *.o
	rm -f $(BIN_NAME)
	rm -f gmon.out


tier_low_io.o: tier_low_io.c tier_low_io.h wabuf/wabuf.h portio/portio.h

TEST.o: TEST.c tier_low_io.h wabuf/wabuf.h portio/portio.h

