#SRC="*.c tier_io/[a-z]*.c tier_pck/[a-z]*.c tier_low_io/[a-z]*.c portio/[a-z]*.c wabuf/[a-z]*.c fdptp/[a-z]*.c"
#OBJ="tier_io/[a-z]*.o tier_pck/[a-z]*.o tier_low_io/[a-z]*.o portio/[a-z]*.o wabuf/[a-z]*.o fdptp/[a-z]*.o"

# make w podkatalogach
#for d in wabuf portio tier_low_io tier_pck tier_io fdptp
#do
#  make -C $d/ all || exit $?
#done

#gcc -Wall -g3 TEST_IO.c    $OBJ -o pc_driver.out              || exit 1
#gcc -Wall -g3 TEST_IO_NC.c $OBJ -o pc_driver_nc.out -lncurses || exit 2
#cp -R $SRC pc_driver*.out ~/hell_share/tmp/PORT/PC_drv/  || (echo 'cp error..' && exit 3)
#exit 0

make test 1>/dev/null || exit $?
make      1>/dev/null || exit $?
cp -R TEST_IO*.out run_tier_io_srv tier_io_srv ~/hell_share/tmp/PORT/PC_drv/  || (echo 'cp error...' && exit 3)
find . -name '*.c' -type f -follow -exec cp {} ~/hell_share/tmp/PORT/PC_drv/ \; || exit $?

