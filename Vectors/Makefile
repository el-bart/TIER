
CC=g++

FLAGS_BASIC=-Wall
CXXFLAGS_REL=$(FLAGS_BASIC) -O3 -DNDEBUG
CXXFLAGS_DEB=$(FLAGS_BASIC) -g3 -pg

LIBS_BASIC=-lm
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
	$(CC) $(CXXFLAGS_DEB) -o $(BIN_NAME)   TEST.o $(CXXLIBS)



make_common: 



clean:
	rm -f *.o
	rm -f $(BIN_NAME)
	rm -f gmon.out


TEST.o: TEST.cpp Vectors/Line.hpp Vectors/Vectors/Point.hpp \
  Vectors/Vectors/Vectors/Exception.hpp Vectors/Vectors/Vector.hpp \
  Vectors/Vectors/Vectors/Point.hpp Vectors/Vectors/Exception.hpp \
  Vectors/Vector.hpp Vectors/Space.hpp Vectors/Vectors/Line.hpp \
  Vectors/RefSpace.hpp

