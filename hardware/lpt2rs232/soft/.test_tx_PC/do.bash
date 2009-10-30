asm51 -f bin test_tx_pc.asm || exit 1
cp test_tx_pc.bin ~/hell_share/tmp/PORT/ || (echo 'cp error..' && exit 2)
exit 0

