# codeOS2
### CodeOS Re-Written from scratch

### CODEOS2 MUST BE BOOTED FROM A HARD DRIVE, FLOPPY DRIVE, DVD DRIVE, OR CD DRIVE. THERE IS NO USB SUPPORT YET.

## How to use codeOS2

Use win32diskimager (or dd if you are on linux) to write bin/codeOS2.img to an HDD or USB drive. (This may not contain the latest code changes.) If you write it to a USB drive you cannot boot it off of a real computer since codeOS doesn't have USB support yet, but you can use it with VMWare player or VirtualBox physical disk mounting.

You can also use the .img file directly with qemu.

Alternatively, you can set up your own GRUB disk (Grub 2 or higher) and use the KERNEL.SYS file in the bin folder along with the multiboot2 instruction.

## Goals

* Fat32 and Fat16 FS support
* Be able to run executables
* VGA Driver(?)
* User mode
* Shell and shell scripts

Original CodeOS: http://www.github.com/Codepixl/CodeOS
