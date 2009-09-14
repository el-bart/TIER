asm51 -f bin ster.asm               || exit 1
cp ster.bin ~/.dosemu/drives/e/tmp/ || exit 2
cp ster.bin ~/hell_share/tmp/PORT/  || (echo 'cp error..' && exit 2)
exit 0

