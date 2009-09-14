
CC=g++

FLAGS_BASIC=-Wall
CXXFLAGS_REL=$(FLAGS_BASIC) -O3 -DNDEBUG
CXXFLAGS_DEB=$(FLAGS_BASIC) -g3 -pg

LIBS_BASIC=-lncurses
CXXLIBS_REL=$(LIBS_BASIC)
CXXLIBS_DEB=$(LIBS_BASIC)

BIN_NAME1=TEST.out
BIN_NAME2=TEST_NC.out
BIN_NAME3=TEST_SPD.out
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
test: deb TEST.o TEST_NC.o TEST_SPD.o
	$(CC) $(CXXFLAGS_DEB) -o $(BIN_NAME1) sterownik_uC/wabuf/wabuf.o sterownik_uC/tier_low_io/portio/portio.o sterownik_uC/tier_low_io/tier_low_io.o sterownik_uC/tier_io/tier_io.o sterownik_uC/tier_io/tier_pck/tier_pck.o ProtoLocal.o TEST.o $(CXXLIBS)
	$(CC) $(CXXFLAGS_DEB) -o $(BIN_NAME2) sterownik_uC/wabuf/wabuf.o sterownik_uC/tier_low_io/portio/portio.o sterownik_uC/tier_low_io/tier_low_io.o sterownik_uC/tier_io/tier_io.o sterownik_uC/tier_io/tier_pck/tier_pck.o ProtoLocal.o TEST_NC.o $(CXXLIBS)
	$(CC) $(CXXFLAGS_DEB) -o $(BIN_NAME3) sterownik_uC/wabuf/wabuf.o sterownik_uC/tier_low_io/portio/portio.o sterownik_uC/tier_low_io/tier_low_io.o sterownik_uC/tier_io/tier_io.o sterownik_uC/tier_io/tier_pck/tier_pck.o ProtoLocal.o TEST_SPD.o $(CXXLIBS)


make_common: ProtoLocal.o
	$(MAKE) -C sterownik_uC/ $(TARGET)



clean:
	$(MAKE) -C sterownik_uC/ clean
	rm -f *.o
	rm -f $(BIN_NAME1)
	rm -f $(BIN_NAME2)
	rm -f $(BIN_NAME3)
	rm -f $(APP_NAME)
	rm -f gmon.out


ProtoLocal.o: ProtoLocal.cpp ProtoLocal.hpp CommProto/Exception.hpp \
  CommProto/TIERproto.hpp CommProto/CommProto/Exception.hpp \
  sterownik_uC/tier_io/tier_io.h \
  sterownik_uC/tier_io/tier_low_io/tier_low_io.h \
  sterownik_uC/tier_io/tier_low_io/wabuf/wabuf.h \
  sterownik_uC/tier_io/tier_low_io/portio/portio.h \
  sterownik_uC/tier_io/tier_pck/tier_pck.h

TEST.o: TEST.cpp TIERproto.hpp CommProto/Exception.hpp ProtoLocal.hpp \
  CommProto/TIERproto.hpp sterownik_uC/tier_io/tier_io.h \
  sterownik_uC/tier_io/tier_low_io/tier_low_io.h \
  sterownik_uC/tier_io/tier_low_io/wabuf/wabuf.h \
  sterownik_uC/tier_io/tier_low_io/portio/portio.h \
  sterownik_uC/tier_io/tier_pck/tier_pck.h

TEST_NC.o: TEST_NC.cpp TIERproto.hpp CommProto/Exception.hpp \
  ProtoLocal.hpp CommProto/TIERproto.hpp sterownik_uC/tier_io/tier_io.h \
  sterownik_uC/tier_io/tier_low_io/tier_low_io.h \
  sterownik_uC/tier_io/tier_low_io/wabuf/wabuf.h \
  sterownik_uC/tier_io/tier_low_io/portio/portio.h \
  sterownik_uC/tier_io/tier_pck/tier_pck.h

TEST_SPD.o: TEST_SPD.cpp TIERproto.hpp CommProto/Exception.hpp \
  ProtoLocal.hpp CommProto/TIERproto.hpp sterownik_uC/tier_io/tier_io.h \
  sterownik_uC/tier_io/tier_low_io/tier_low_io.h \
  sterownik_uC/tier_io/tier_low_io/wabuf/wabuf.h \
  sterownik_uC/tier_io/tier_low_io/portio/portio.h \
  sterownik_uC/tier_io/tier_pck/tier_pck.h

