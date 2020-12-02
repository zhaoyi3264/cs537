#!/bin/bash
make clean
make
clear
for FILE in smallmix.addrtrace 4000.addrtrace bigmix.addrtrace 12million.addrtrace
do
	for P in 512 4096
	do
		for M in 1 4 32
		do
			echo ''
			echo FILE = $FILE P = $P M = $M
			time ./537pfsim-opt -p $P -m $M proj4/$FILE
		done
	done
done
