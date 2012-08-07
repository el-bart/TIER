#!/bin/bash

mf="Makefile"

SRC="portio.c"
OBJ="`echo "$SRC" | sed 's:\.c:\.o:g'`"
SUB=""


# generujemy liste obiektow do dolinkowania
[ "$SUB" != "" ] && \
  SUBOBJS=`find $SUB -name '[a-z]*.c' -follow | sed 's:\.c$:\.o:g'`



echo -n '
CC=gcc

FLAGS_BASIC=-Wall 
CFLAGS_REL=$(FLAGS_BASIC) -O3 -DNDEBUG
CFLAGS_DEB=$(FLAGS_BASIC) -g3 -pg

LIBS_BASIC=
CLIBS_REL=$(LIBS_BASIC)
CLIBS_DEB=$(LIBS_BASIC) -lefence -ldmalloc

#BIN_NAME=TEST.out



all: rel

rel: CFLAGS=$(CFLAGS_REL)
rel: CLIBS=$(CLIBS_REL)
rel: TARGET=rel
rel: make_common

deb: CFLAGS=$(CFLAGS_DEB)
deb: CLIBS=$(CLIBS_DEB)
deb: TARGET=deb
deb: make_common
' > $mf

#echo -n '
#test: CFLAGS=$(CFLAGS_DEB)
#test: CLIBS=$(CLIBS_DEB)
#test: deb TEST.o
#	$(CC) $(CFLAGS_DEB) -o $(BIN_NAME) ' >> $mf
#echo -n "$SUBOBJS $OBJ TEST.o" >> $mf
#echo -n ' $(CLIBS)
echo -n '


make_common: ' >> $mf
echo "$OBJ" >> $mf
for d in $SUB
do
  echo -n '	$(MAKE) -C ' >> $mf
  echo -n "$d/ "      >> $mf
  echo -n '$(TARGET)' >> $mf
done
echo '


clean:' >> $mf
for d in $SUB
do
  echo -n '	$(MAKE) -C ' >> $mf
  echo -n "$d/ "      >> $mf
  echo 'clean' >> $mf
done
echo -n \
'	rm -f *.o
	rm -f $(BIN_NAME)
	rm -f gmon.out


' >> $mf


# automatyczna generacja zaleznosci
for src in $SRC TEST.c
do
  test -e $src || continue	# TEST.c moze byc gdzie indziej
  gcc -MM $src >> $mf || exit $?
  echo ""      >> $mf
done

