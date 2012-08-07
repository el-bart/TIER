#!/bin/bash

[ $# -ne 1 ] && echo "$0 <lines_cnt>" && exit 1

IN="OUT.ps"
OUT="OUT_low_cut.ps"
CNT="$1"

echo "%!PS" > $OUT
grep '^[0-9]' $IN | head -$(($1*2)) >> $OUT
echo "stroke" >> $OUT

exit 0

