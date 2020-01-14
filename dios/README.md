DIOS
====
DIOS is a minimal 32-bit operating system kernel in D written for fun. The main purpose of this project is demonstrating D's fitness for OS development. I've written it initially in D1/GDC and recently ported to D2/DMD. I don't have any big plans for this code - you are free to use it to create your own kernel. PRs implementing real-world OS features are welcome. 

Features
--------
Nothing fancy for now: just booting up, printing arguments and getting Multiboot info from GRUB (like memory map).

Building
--------
Prerequisites:
* x86 Linux machine with GNU toolchain
* Recent DMD compiler (2.073.0 or higher)
* [NASM](http://www.nasm.us)
* mkisofs to generate Live CD

Due to specific compilation/linkage pipeline, DIOS doesn't use Dub but [Cook](https://github.com/gecko0307/cook2) for build automation. I'm not sure if the same can be done with Dub - if you've managed to do it, sharing `dub.json` would be nice.

1. Edit `default.conf` and set proper `compiler_dir` (path to your DMD installation containing `bin32` directory)
2. Run `./cook`
3. You're done! Newly generated CD image can be found in `output`. Contents of the image are in `cdroot`.

