
CC=gcc

FLAGS_BASIC=-Wall -lm
CFLAGS_REL=$(FLAGS_BASIC) -O3
CFLAGS_DEB=$(FLAGS_BASIC) -g3 

LIBS_BASIC=
CLIBS_REL=$(LIBS_BASIC)
CLIBS_DEB=$(LIBS_BASIC) -lefence 

BIN_NAME1a=TEST_IO.out
BIN_NAME1b=TEST_IO_cln.out
BIN_NAME2a=TEST_IO_NC.out
BIN_NAME2b=TEST_IO_NC_cln.out

PROG_NAME=tier_io_srv



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
test: deb TEST_IO.o TEST_IO_cln.o TEST_IO_NC.o TEST_IO_NC_cln.o
	$(CC) $(CFLAGS) -o $(BIN_NAME1a) fdptp/fdptp.o tier_io/tier_io.o tier_io/tier_low_io/wabuf/wabuf.o tier_io/tier_low_io/portio/portio.o tier_io/tier_low_io/tier_low_io.o tier_io/tier_pck/tier_pck.o tier_io_client.o TEST_IO.o $(CLIBS)
	$(CC) $(CFLAGS) -o $(BIN_NAME1b) fdptp/fdptp.o tier_io/tier_io.o tier_io/tier_low_io/wabuf/wabuf.o tier_io/tier_low_io/portio/portio.o tier_io/tier_low_io/tier_low_io.o tier_io/tier_pck/tier_pck.o tier_io_client.o TEST_IO_cln.o $(CLIBS)
	$(CC) $(CFLAGS) -o $(BIN_NAME2a) fdptp/fdptp.o tier_io/tier_io.o tier_io/tier_low_io/wabuf/wabuf.o tier_io/tier_low_io/portio/portio.o tier_io/tier_low_io/tier_low_io.o tier_io/tier_pck/tier_pck.o tier_io_client.o TEST_IO_NC.o $(CLIBS) -lncurses
	$(CC) $(CFLAGS) -o $(BIN_NAME2b) fdptp/fdptp.o tier_io/tier_io.o tier_io/tier_low_io/wabuf/wabuf.o tier_io/tier_low_io/portio/portio.o tier_io/tier_low_io/tier_low_io.o tier_io/tier_pck/tier_pck.o tier_io_client.o TEST_IO_NC_cln.o $(CLIBS) -lncurses


make_common: tier_io_srv.o tier_io_client.o
	$(MAKE) -C fdptp/ $(TARGET)
	$(MAKE) -C tier_io/ $(TARGET)
	$(MAKE) -C tier_low_io/ $(TARGET)
	$(MAKE) -C tier_pck/ $(TARGET)
	$(MAKE) -C wabuf/ $(TARGET)
	$(CC) $(CFLAGS) -o $(PROG_NAME) fdptp/fdptp.o tier_io/tier_io.o tier_io/tier_low_io/wabuf/wabuf.o tier_io/tier_low_io/portio/portio.o tier_io/tier_low_io/tier_low_io.o tier_io/tier_pck/tier_pck.o tier_io_client.o tier_io_srv.o $(CLIBS)


clean:
	$(MAKE) -C fdptp/ clean
	$(MAKE) -C tier_io/ clean
	$(MAKE) -C tier_low_io/ clean
	$(MAKE) -C tier_pck/ clean
	$(MAKE) -C wabuf/ clean
	rm -f *.o
	rm -f $(BIN_NAME1)
	rm -f $(BIN_NAME2)
	rm -f $(PROG_NAME)
	rm -f gmon.out


tier_io_client.o: tier_io_client.c tier_io_client.h fdptp/fdptp.h \
  tier_fdptp_comm.h

TEST_IO.o: TEST_IO.c tier_io/tier_io.h tier_io/tier_low_io/tier_low_io.h \
  tier_io/tier_low_io/wabuf/wabuf.h tier_io/tier_low_io/portio/portio.h \
  tier_io/tier_pck/tier_pck.h

TEST_IO_NC.o: TEST_IO_NC.c tier_io/tier_io.h \
  tier_io/tier_low_io/tier_low_io.h tier_io/tier_low_io/wabuf/wabuf.h \
  tier_io/tier_low_io/portio/portio.h tier_io/tier_pck/tier_pck.h

TEST_IO_NC_cln.o: TEST_IO_NC_cln.c tier_io_client.h fdptp/fdptp.h \
  tier_fdptp_comm.h

TEST_IO_cln.o: TEST_IO_cln.c tier_io_client.h fdptp/fdptp.h \
  tier_fdptp_comm.h

tier_io_srv.o: tier_io_srv.c fdptp/fdptp.h tier_io/tier_io.h \
  tier_io/tier_low_io/tier_low_io.h tier_io/tier_low_io/wabuf/wabuf.h \
  tier_io/tier_low_io/portio/portio.h tier_io/tier_pck/tier_pck.h \
  tier_fdptp_comm.h

