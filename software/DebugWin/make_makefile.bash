#!/bin/bash

mf="Makefile"

SRC="DebugTransmit.cpp DebugRecieve.cpp"
GTK_SRC="DebugWin.cpp"
OBJ="`echo "$SRC" | sed 's:\.cpp:\.o:g'`"
GTK_OBJ="`echo "$GTK_SRC" | sed 's:\.cpp:\.o:g'`"
SUB="CGraphics Network Graphics VCCln VCSrv Vectors Maps bitmap raster"

GTK_FLAGS="pkg-config gtk+-2.0 --cflags --libs"


# generujemy liste obiektow do dolinkowania
if [ "$SUB" != "" ]
then
  SUBOBJS=""
  for f in `find $SUB -name '[A-Za-z]*[a-z].cpp' -o -name '[a-z]*.c' -follow | \
                    sed 's:\.cpp$:\.o:g' | sed 's:\.c$:\.o:g'`
  do
    # dodajemy tylko te pliki, ktore jeszcze sie nie powtorzyly!
    grep `basename $f` <<<"$SUBOBJS" > /dev/null 2>&1 && continue
	# pomijamy "szkodliwe"
	grep "ServerApp.o" <<<"$f" > /dev/null 2>&1 && continue
    SUBOBJS=`echo -n $SUBOBJS $f`
  done
fi


echo -n '
CC=g++

FLAGS_BASIC=-Wall
CXXFLAGS_REL=$(FLAGS_BASIC) -O3 -DNDEBUG
CXXFLAGS_DEB=$(FLAGS_BASIC) -g3 -pg

LIBS_BASIC=-lpthread
CXXLIBS_REL=$(LIBS_BASIC)
CXXLIBS_DEB=$(LIBS_BASIC)

BIN_NAME1=TEST.out
BIN_NAME2=TEST_RESEND.out
APP_NAME=debug_win
' > $mf

echo "
GTK_FLAGS=\$(shell $GTK_FLAGS)
" >> $mf

echo '

all: rel

rel: CXXFLAGS=$(CXXFLAGS_REL)
rel: CXXLIBS=$(CXXLIBS_REL)
rel: TARGET=rel
rel: make_common

deb: CXXFLAGS=$(CXXFLAGS_DEB)
deb: CXXLIBS=$(CXXLIBS_DEB)
deb: TARGET=deb
deb: make_common
' >> $mf

echo -n '
test: CXXFLAGS=$(CXXFLAGS_DEB)
test: CXXLIBS=$(CXXLIBS_DEB)
test: deb TEST.o TEST_RESEND.o
	$(CC) $(CXXFLAGS_DEB) -o $(BIN_NAME1) ' >> $mf
echo -n "$SUBOBJS $OBJ TEST.o" >> $mf
echo -n ' $(CXXLIBS)
	$(CC) $(CXXFLAGS_DEB) -o $(BIN_NAME2) ' >> $mf
echo -n "$SUBOBJS $OBJ TEST_RESEND.o" >> $mf
echo -n ' $(CXXLIBS)


make_common: ' >> $mf
echo "$OBJ $GTK_OBJ" >> $mf
for d in $SUB
do
  echo -n '	$(MAKE) -C ' >> $mf
  echo -n "$d/ "      >> $mf
  echo    '$(TARGET)' >> $mf
done
echo -n '	$(CC) $(CXXFLAGS) -o $(APP_NAME) ' >> $mf
echo -n "$SUBOBJS $OBJ $GTK_OBJ " >> $mf
echo -n '$(CXXLIBS) $(GTK_FLAGS)' >> $mf

echo -n '

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
	rm -f $(BIN_NAME1)
	rm -f $(BIN_NAME2)
	rm -f $(APP_NAME)
	rm -f gmon.out


' >> $mf


# automatyczna generacja zaleznosci
for src in $SRC TEST*.cpp
do
  test -e $src || continue	# TEST.c moze byc gdzie indziej
  gcc -MM $src >> $mf || exit $?
  echo ""      >> $mf
done

# automatyczna generacja zaleznosci dla zrodel w GTK...
for src in $GTK_SRC
do
  test -e $src || continue	# TEST.c moze byc gdzie indziej
  gcc -MM $src >> $mf || exit $?
  echo -n '	$(CC) $(CXXFLAGS) -c ' >> $mf
  echo -n "$src " >> $mf
  echo -n '$(GTK_FLAGS)' >> $mf
  echo -e "\n"      >> $mf
done

