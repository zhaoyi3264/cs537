#!/bin/bash
make clean
make
clear
for EXE in 537pfsim-fifo 537pfsim-lru 537pfsim-clock
do
	for FILE in smallmix.addrtrace 4000.addrtrace bigmix.addrtrace 12million.addrtrace
	do
		for P in 512 4096
		do
			for M in 1 4 32
			do
				echo ''
				echo EXE = $EXE FILE = $FILE P = $P M = $M
				time ./$EXE -p $P -m $M proj4/$FILE
			done
		done
	done
done
