
CC=g++

FLAGS_BASIC=-Wall
CXXFLAGS_REL=$(FLAGS_BASIC) -O3 -DNDEBUG
CXXFLAGS_DEB=$(FLAGS_BASIC) -g3 -pg

LIBS_BASIC=-lpthread
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
	$(CC) $(CXXFLAGS_DEB) -o $(BIN_NAME)   TEST.o $(CXXLIBS)



make_common: 



clean:
	rm -f *.o
	rm -f $(BIN_NAME)
	rm -f $(APP_NAME)
	rm -f gmon.out


TEST.o: TEST.cpp Mutex.hpp Threads/Exception.hpp GeneralLock.hpp \
  Threads/Mutex.hpp Lock.hpp Threads/GeneralLock.hpp TryLock.hpp \
  Thread.hpp Conditional.hpp Threads/Condition.hpp \
  Threads/Threads/Exception.hpp

