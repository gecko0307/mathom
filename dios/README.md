DIOS
====
DIOS is a minimal 32-bit operating system kernel in D written for fun. The main purpose of this project is demonstrating D's fitness for OS development. I've written it initially in D1/GDC and recently ported to D2/LDC. I don't have any big plans for this code - you are free to use it to create your own kernel. PRs implementing real-world OS features are welcome.

DIOS is an ELF kernel that requires a bootloader to run. Default setup in this repo uses GRUB stage2_eltorito for booting from the CD-ROM.

Features
--------
Nothing fancy for now: just booting up, printing arguments and getting Multiboot info from GRUB.

Building
--------
Prerequisites:
* Recent LDC compiler
* [NASM](http://www.nasm.us)
* mkisofs to generate Live CD

1. Run `./build.bat` or `./build.so`
2. You're done! Newly generated CD image is `dios.iso`.
