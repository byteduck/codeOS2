# codeOS2
### CodeOS Re-Written from scratch

### CODEOS2 MUST BE BOOTED FROM A HARD DRIVE, FLOPPY DRIVE, DVD DRIVE, OR CD DRIVE. THERE IS NO USB SUPPORT YET.


CodeOS2 requires to be booted from GRUB.
Example GRUB2 menuentry for CodeOS2:

```
menuentry "CodeOS2" {
	multiboot2 /KERNEL.SYS
}
```

## Goals

* Fat32 and Fat16 FS support
* Be able to run executables
* VGA Driver(?)
* User mode
* Shell and shell scripts

Original CodeOS: http://www.github.com/Codepixl/CodeOS
