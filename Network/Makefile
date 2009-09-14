
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
	$(CC) $(CXXFLAGS_DEB) -o $(BIN_NAME)  Resolver.o Server.o TCPServer.o TCPClient.o TEST.o $(CXXLIBS)



make_common: Resolver.o Server.o TCPServer.o TCPClient.o



clean:
	rm -f *.o
	rm -f $(BIN_NAME)
	rm -f gmon.out


Resolver.o: Resolver.cpp Resolver.hpp Network/Address.hpp \
  Network/Exception.hpp

Server.o: Server.cpp Server.hpp Network/Socket.hpp \
  Network/Network/Exception.hpp Network/Connection.hpp \
  Network/Network/Socket.hpp Network/Address.hpp Network/Exception.hpp

TCPServer.o: TCPServer.cpp TCPServer.hpp Network/Server.hpp \
  Network/Network/Socket.hpp Network/Network/Network/Exception.hpp \
  Network/Network/Connection.hpp Network/Network/Network/Socket.hpp \
  Network/Network/Address.hpp Network/Network/Exception.hpp \
  Network/Connection.hpp Network/Exception.hpp Resolver.hpp \
  Network/Address.hpp

TCPClient.o: TCPClient.cpp TCPClient.hpp Network/Client.hpp \
  Network/Network/Connection.hpp Network/Network/Network/Socket.hpp \
  Network/Network/Network/Network/Exception.hpp \
  Network/Network/Network/Exception.hpp Network/Network/Address.hpp \
  Network/Network/Exception.hpp Resolver.hpp Network/Address.hpp \
  Network/Exception.hpp

TEST.o: TEST.cpp Exception.hpp Socket.hpp Network/Exception.hpp \
  Server.hpp Network/Socket.hpp Network/Connection.hpp \
  Network/Network/Socket.hpp Network/Network/Exception.hpp \
  Network/Address.hpp Client.hpp Connection.hpp Address.hpp Resolver.hpp \
  TCPServer.hpp Network/Server.hpp TCPClient.hpp Network/Client.hpp

