#!/bin/bash
if [ $# -eq 0 ]
then
	echo "No arguments supplied. use -a for attach, and -d for detach."
else
	if [ "$1" == "-a" ]
	then
		sudo losetup -f > loops
		sudo losetup -f codeos2.img
		sudo losetup -f >> loops
		sudo losetup -f codeos2.img -o 1048576
		readarray loops < loops
		sudo mount ${loops[1]} /media/${USER}/codeOS2 -o sync
		echo "Attached! Loops:"
		cat loops
	elif [ "$1" == "-d" ]
	then
		readarray loops < loops
		sudo umount ${loops[1]}
		sudo losetup -d ${loops[0]}
		sudo losetup -d ${loops[1]}
		> loops
		echo "Detached!"
	else
		echo "Invalid argument $1!"
	fi
fi
