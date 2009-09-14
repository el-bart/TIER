
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
	$(CC) $(CXXFLAGS_DEB) -o $(BIN_NAME) tier_io/tier_io.o tier_io/tier_low_io/wabuf/wabuf.o tier_io/tier_low_io/portio/portio.o tier_io/tier_low_io/tier_low_io.o tier_io/tier_pck/tier_pck.o  TEST.o $(CXXLIBS)



make_common: 
	$(MAKE) -C tier_io/ $(TARGET)
	$(MAKE) -C tier_low_io/ $(TARGET)
	$(MAKE) -C wabuf/ $(TARGET)
	$(MAKE) -C tier_pck/ $(TARGET)



clean:
	$(MAKE) -C tier_io/ clean
	$(MAKE) -C tier_low_io/ clean
	$(MAKE) -C wabuf/ clean
	$(MAKE) -C tier_pck/ clean
	rm -f *.o
	rm -f $(BIN_NAME)
	rm -f $(APP_NAME)
	rm -f gmon.out


