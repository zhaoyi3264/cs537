#!/bin/bash
make clean
make
clear
for FILE in smallmix.addrtrace 4000.addrtrace bigmix.addrtrace 12million.addrtrace
do
	echo ''
	echo FILE = $FILE
	time ./537pfsim-opt proj4/$FILE
done
