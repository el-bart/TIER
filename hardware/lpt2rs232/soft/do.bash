asm51 -f bin lpt2rs232.asm || exit 1
cp lpt2rs232.bin ~/hell_share/tmp/PORT/ || (echo 'cp error..' && exit 2)
exit 0

