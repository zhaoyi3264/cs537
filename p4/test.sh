#!/bin/bash
make clean
make
clear
for EXE in 537pfsim-fifo 537pfsim-lru 537pfsim-clock
do
	for FILE in smallmix.addrtrace 4000.addrtrace bigmix.addrtrace 12million.addrtrace
	do
		echo ''
		echo EXE = $EXE FILE = $FILE
		time ./$EXE proj4/$FILE
	done
done
