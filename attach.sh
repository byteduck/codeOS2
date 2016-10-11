#!/bin/bash
if [ $# -eq 0 ]
then
	echo "No arguments supplied. use -a for attach, and -d for detach."
else
	if [ "$1" == "-a" ]
	then
		sudo losetup /dev/loop0 codeos2.img
		sudo losetup /dev/loop1 codeos2.img -o 1048576
		echo "Attached!"
	elif [ "$1" == "-d" ]
	then
		sudo losetup -d /dev/loop0
		sudo losetup -d /dev/loop1
		echo "Detached! (If there's an error, make sure to unmount disk first)"
	else
		echo "Invalid argument $1!"
	fi
fi