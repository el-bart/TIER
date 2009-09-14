
CC=gcc

FLAGS_BASIC=-Wall 
CFLAGS_REL=$(FLAGS_BASIC) -O3 -DNDEBUG
CFLAGS_DEB=$(FLAGS_BASIC) -g3 -pg

LIBS_BASIC=
CLIBS_REL=$(LIBS_BASIC)
CLIBS_DEB=$(LIBS_BASIC) -lefence -ldmalloc

BIN_NAME_S=TEST_server.out
BIN_NAME_C=TEST_client.out



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
test: deb TEST_server.o TEST_client.o
	$(CC) $(CFLAGS_DEB) -o $(BIN_NAME_S) fdptp.o TEST_server.o  $(CLIBS)
	$(CC) $(CFLAGS_DEB) -o $(BIN_NAME_C) fdptp.o TEST_client.o  $(CLIBS)


make_common: fdptp.o



clean:
	rm -f *.o
	rm -f $(BIN_NAME)
	rm -f gmon.out


fdptp.o: fdptp.c fdptp.h

TEST_client.o: TEST_client.c fdptp.h test_config.h

