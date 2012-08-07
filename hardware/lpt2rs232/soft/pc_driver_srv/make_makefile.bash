#!/bin/bash

mf="Makefile"

SRC="tier_io_client.c"
OBJ="`echo "$SRC" | sed 's:\.c:\.o:g'`"
SUB="fdptp tier_io tier_low_io tier_pck wabuf"


# generujemy liste obiektow do dolinkowania
if [ "$SUB" != "" ]
then
  SUBOBJS=""
  for f in `find $SUB -name '[a-z]*.c' -follow | sed 's:\.c$:\.o:g'`
  do
    # dodajemy tylko te pliki, ktore jeszcze sie nie powtorzyly!
    ! echo $SUBOBJS | grep `basename $f` > /dev/null 2>&1 && \
      SUBOBJS=`echo -n $SUBOBJS $f`
  done
fi


echo -n '
CC=gcc

FLAGS_BASIC=-Wall 
CFLAGS_REL=$(FLAGS_BASIC) -O3 -DNDEBUG
CFLAGS_DEB=$(FLAGS_BASIC) -g3 -pg -DDMALLOC

LIBS_BASIC=-lm
CLIBS_REL=$(LIBS_BASIC)
CLIBS_DEB=$(LIBS_BASIC) -lefence -ldmalloc

BIN_NAME1a=TEST_IO.out
BIN_NAME1b=TEST_IO_cln.out
BIN_NAME2a=TEST_IO_NC.out
BIN_NAME2b=TEST_IO_NC_cln.out
BIN_NAME3a=TEST_SPDSTs.out

PROG_NAME=tier_io_srv



all: deb

rel: CFLAGS=$(CFLAGS_REL)
rel: CLIBS=$(CLIBS_REL)
rel: TARGET=rel
rel: make_common 

deb: CFLAGS=$(CFLAGS_DEB)
deb: CLIBS=$(CLIBS_DEB)
deb: TARGET=deb
deb: make_common
' > $mf

echo -n '
test: CFLAGS=$(CFLAGS_DEB)
test: CLIBS=$(CLIBS_DEB)
test: deb TEST_IO.o TEST_IO_cln.o TEST_IO_NC.o TEST_IO_NC_cln.o TEST_SPDSTs.o
	$(CC) $(CFLAGS) -o $(BIN_NAME1a) ' >> $mf
echo -n "$SUBOBJS $OBJ TEST_IO.o" >> $mf
echo -n ' $(CLIBS)
	$(CC) $(CFLAGS) -o $(BIN_NAME1b) ' >> $mf
echo -n "$SUBOBJS $OBJ TEST_IO_cln.o" >> $mf
echo -n ' $(CLIBS)
	$(CC) $(CFLAGS) -o $(BIN_NAME2a) ' >> $mf
echo -n "$SUBOBJS $OBJ TEST_IO_NC.o" >> $mf
echo -n ' $(CLIBS) -lncurses
	$(CC) $(CFLAGS) -o $(BIN_NAME2b) ' >> $mf
echo -n "$SUBOBJS $OBJ TEST_IO_NC_cln.o" >> $mf
echo -n ' $(CLIBS) -lncurses
	$(CC) $(CFLAGS) -o $(BIN_NAME3a) ' >> $mf
echo -n "$SUBOBJS $OBJ TEST_SPDSTs.o" >> $mf
echo -n ' $(CLIBS)


make_common: tier_io_srv.o ' >> $mf
echo "$OBJ" >> $mf
for d in $SUB
do
  echo -n '	$(MAKE) -C ' >> $mf
  echo -n "$d/ "      >> $mf
  echo    '$(TARGET)' >> $mf
done
echo -n '	$(CC) $(CFLAGS) -o $(PROG_NAME) ' >> $mf
echo -n "$SUBOBJS $OBJ tier_io_srv.o" >> $mf
echo -n ' $(CLIBS)


clean:
' >> $mf
for d in $SUB
do
  echo -n '	$(MAKE) -C ' >> $mf
  echo -n "$d/ "      >> $mf
  echo 'clean' >> $mf
done
echo -n \
'	rm -f *.o
	rm -f $(BIN_NAME1a)
	rm -f $(BIN_NAME1b)
	rm -f $(BIN_NAME2a)
	rm -f $(BIN_NAME2b)
	rm -f $(BIN_NAME3a)
	rm -f $(PROG_NAME)
	rm -f gmon.out


' >> $mf


# automatyczna generacja zaleznosci
for src in $SRC TEST_*.c tier_io_srv.c
do
  test -e $src || continue	# TEST.c moze byc gdzie indziej
  gcc -MM $src >> $mf || exit $?
  echo ""      >> $mf
done

