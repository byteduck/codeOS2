# codeOS2
### CodeOS Re-Written from scratch

### CODEOS2 MUST BE BOOTED FROM A HARD DRIVE (Or emulated hard drive). THERE IS NO USB SUPPORT YET.

## How to use codeOS2

You can compile from source using the makefile.

Also, you can use the provided codeos2.img file. However, this is not updated with the latest code changes very often.

Use win32diskimager (or dd if you are on linux) to write codeos2.img to an HDD or USB drive. (This may not contain the latest code changes.) If you write it to a USB drive you cannot boot it off of a real computer since codeOS doesn't have USB support yet, but you can use it with VMWare player or VirtualBox physical disk mounting.

You can also use the .img file directly with qemu.

## Goals (~~crossed out~~ means completed)

* ~~Fat32 and Ext2 support (Once implemented, Ext2 will be the main filesystem.)~~
    * Ext2 is now the main filesystem.
* Be able to run executables
    * It kinda can at the moment, but they are loaded into a designated area of memory and run in kernel space.
* VGA Driver(?)
* User mode
* Shell and shell scripts
* At least partial POSIX compliance (someday...)
 * I'm not sure if I'm going to make it directly POSIX compatible, or have a compatibility layer for POSIX applications. It might just be easier to make it directly (partially) POSIX compatible.

Original CodeOS: http://www.github.com/Codepixl/CodeOS

My YouTube channel:
https://www.youtube.com/codepixl

# Compile
Make a new folder called out, and in that a folder called disk.
Use make to compile, and then you will have to take all of the directories and files in out/disk and copy them to a drive with GRUB2 installed. Then use the multiboot2 command in grub with the file codeos2/system/krnl32.
