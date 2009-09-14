asm51 -f bin test.asm || exit 1
cp test.bin ~/hell_share/tmp/PORT/ || (echo 'cp error..' && exit 2)
exit 0

