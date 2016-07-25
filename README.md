# codeOS2
### CodeOS Re-Written from scratch

CodeOS2 requires to be boot from GRUB.
Example GRUB2 menuentry for CodeOS2:

menuentry "CodeOS2"{
	multiboot2 /KERNEL.SYS
}

## Goals

* Fat32 and Fat16 FS support
* Be able to run executables
* VGA Driver(?)
* User mode
* Shell and shell scripts

Original CodeOS: http://www.github.com/Codepixl/CodeOS
