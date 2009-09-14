#!/bin/bash

mf="Makefile"

SRC="MapCreator.cpp PathCreator.cpp CommandCreator.cpp TIERctl.cpp"
OBJ="`sed 's:\.cpp:\.o:g' <<< "$SRC" `"
APP_SRC="Zuchter.cpp"
APP_OBJ="Zuchter.o"
SUB="VCCln Maps CGraphics Path CommProto Threads"
EXTRA_OBJS="DebugWin/Debug[TR]*.o DebugWin/VCSrv/Ras*.o"


# generujemy liste obiektow do dolinkowania
if [ "$SUB" != "" ]
then
  SUBOBJS=""
  for f in `find $SUB -name '[A-Za-z]*[^A-Z]*.cpp' -o -name '[a-z]*.c' -follow | \
                    sed 's:\.cpp$:\.o:g' | sed 's:\.c$:\.o:g'`
  do
    # dodajemy tylko te pliki, ktore jeszcze sie nie powtorzyly!
    ! echo $SUBOBJS | grep `basename $f` > /dev/null 2>&1 && \
	  grep -v "TEST.*\.o" <<<"$f" >/dev/null 2>&1 && \
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

BIN_NAME=TEST.out
APP_NAME=zuchter



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
echo -n "$SUBOBJS $OBJ $EXTRA_OBJS TEST.o" >> $mf
echo -n ' $(CXXLIBS)



make_common: ' >> $mf
echo "$OBJ $APP_OBJ" >> $mf
for d in $SUB
do
  echo -n '	$(MAKE) -C ' >> $mf
  echo -n "$d/ "      >> $mf
  echo    '$(TARGET)' >> $mf
done
echo -n '	$(CC) $(CXXFLAGS) -o $(APP_NAME) ' >> $mf
echo -n "$SUBOBJS $OBJ $EXTRA_OBJS $APP_OBJ" >> $mf
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
for src in $SRC $APP_SRC TEST.cpp
do
  test -e $src || continue	# TEST.c moze byc gdzie indziej
  gcc -MM $src >> $mf || exit $?
  echo ""      >> $mf
done

