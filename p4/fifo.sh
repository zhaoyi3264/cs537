#!/bin/bash
clear
make
for P in 512 4096
do
	for M in 1 4 32
	do
		echo P = $P M = $M
		./537pfsim-fifo proj4/smallmix.addrtrace
	done
done
