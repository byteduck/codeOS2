# codeOS2
### CodeOS Re-Written from scratch

### CODEOS2 MUST BE BOOTED FROM A HARD DRIVE, FLOPPY DRIVE, DVD DRIVE, OR CD DRIVE. THERE IS NO USB SUPPORT YET.

## How to use codeOS2

Use win32diskimager (or dd if you are on linux) to write codeos2.img to an HDD or USB drive. (This may not contain the latest code changes.) If you write it to a USB drive you cannot boot it off of a real computer since codeOS doesn't have USB support yet, but you can use it with VMWare player or VirtualBox physical disk mounting.

You can also use the .img file directly with qemu.

## Goals

* ~~Fat32~~ and Fat16 FS support
* Be able to run executables
* VGA Driver(?)
* User mode
* Shell and shell scripts

Original CodeOS: http://www.github.com/Codepixl/CodeOS

# Compile
Make a new folder called out, and in that a folder called disk.
Use make to compile, and then you will have to take all of the directories and files in out/disk and copy them to a drive with GRUB2 installed. Then use the multiboot2 command in grub with the file codeos2/system/krnl32.
