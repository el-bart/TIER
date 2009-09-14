#!/bin/bash

mf="Makefile"

SRC="Connection.cpp RasterSerializer.cpp Server.cpp ServerApp.cpp"
OBJ="`echo "$SRC" | sed 's:\.cpp:\.o:g'`"
SUB="Graphics Network"


# generujemy liste obiektow do dolinkowania
if [ "$SUB" != "" ]
then
  SUBOBJS=""
  for f in `find $SUB -name '[A-Za-z]*[a-z].cpp' -o -name '[a-z]*.c' -follow | \
                    sed 's:\.cpp$:\.o:g' | sed 's:\.c$:\.o:g'`
  do
    # dodajemy tylko te pliki, ktore jeszcze sie nie powtorzyly!
    ! echo $SUBOBJS | grep `basename $f` > /dev/null 2>&1 && \
      SUBOBJS=`echo -n $SUBOBJS $f`
  done
fi


echo -n '
CC=g++

FLAGS_BASIC=-Wall
CXXFLAGS_REL=$(FLAGS_BASIC) -O3 -DNDEBUG
CXXFLAGS_DEB=$(FLAGS_BASIC) -g3 -pg

LIBS_BASIC=
CXXLIBS_REL=$(LIBS_BASIC)
CXXLIBS_DEB=$(LIBS_BASIC)

BIN_NAME=TEST.out
APP_NAME=vc_srv



all: deb

rel: CXXFLAGS=$(CXXFLAGS_REL)
rel: CXXLIBS=$(CXXLIBS_REL)
rel: TARGET=rel
rel: make_common

deb: CXXFLAGS=$(CXXFLAGS_DEB)
deb: CXXLIBS=$(CXXLIBS_DEB)
deb: TARGET=deb
deb: make_common
' > $mf

echo -n '
test: CXXFLAGS=$(CXXFLAGS_DEB)
test: CXXLIBS=$(CXXLIBS_DEB)
test: deb TEST.o
	$(CC) $(CXXFLAGS_DEB) -o $(BIN_NAME) ' >> $mf
echo -n "$SUBOBJS $OBJ TEST.o" >> $mf
echo -n ' $(CXXLIBS)



make_common: ' >> $mf
echo "$OBJ" >> $mf
for d in $SUB
do
  echo -n '	$(MAKE) -C ' >> $mf
  echo -n "$d/ "      >> $mf
  echo    '$(TARGET)' >> $mf
done
echo -n '	$(CC) $(CXXFLAGS) -o $(APP_NAME) ' >> $mf
echo -n "$SUBOBJS $OBJ" >> $mf
echo -n ' $(CXXLIBS)


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
	rm -f $(BIN_NAME)
	rm -f $(APP_NAME)
	rm -f gmon.out


' >> $mf


# automatyczna generacja zaleznosci
for src in $SRC TEST.cpp
do
  test -e $src || continue	# TEST.c moze byc gdzie indziej
  gcc -MM $src >> $mf || exit $?
  echo ""      >> $mf
done

