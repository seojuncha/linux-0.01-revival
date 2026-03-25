# Linux 0.01 Analysis Environment
This project builds and runs Linux 0.01 on the Bochs emulator to analyze the entire kernel from source.
Detailed analysis is documented on my [blog](https://seojuncha.github.io).

## Motivation
The modern Linux kernel is too large to trace end-to-end. Linux 0.01 fits the entire kernel — bootloader, scheduler, filesystem, and more — into just a few thousand lines, making it possible to follow exactly how each component works and how they fit together.
Running it on an emulator makes the analysis even more practical. Bochs has a built-in debugger that lets you inspect CPU registers, memory, and interrupt flow in ways that simply aren't possible on real hardware.

## Source
The Linux 0.01 source in this repository is based on a version patched for modern compilers.
- Original reference: https://github.com/mariuz/linux-0.01

## Repository Structure
```
.
├── bochsrc.txt          # Bochs configuration file
├── create_hdb.sh        # Script to create the Minix disk image (hdb.img)
├── hdb.img.tar.gz       # Pre-built disk image (ready to use)
├── commands.tar.gz      # Linux 0.01-compatible command binaries (echo, pwd, etc.)
├── sh.tar.gz            # Linux 0.01-compatible shell binary
└── linux-0.01/          # Linux 0.01 source code
    ├── boot/            # Boot sector and kernel initialization
    ├── kernel/          # Interrupts, traps, system calls
    ├── mm/              # Memory management
    ├── fs/              # Minix filesystem
    ├── init/            # Kernel entry point (main.c)
    ├── include/         # Header files
    ├── lib/             # Utilities
    └── tools/           # Boot image generation tools
```

`hdb.img.tar.gz` is a pre-built disk image that already includes the shell and command binaries from `sh.tar.gz` and `commands.tar.gz`.
You can use it directly without building anything.

> The binaries in `commands.tar.gz` and `sh.tar.gz` are sourced from [draconux.free.fr](http://draconux.free.fr/os_dev/linux0.01.html). 

## Requirements
### Tested on
|       | Version              |
| ----- | -------------------- |
| OS    | Ubuntu 22.04 / 24.04 |
| GCC   | 11.04 or later       |
| Bochs | 2.6.11               |

### Kernel build dependencies:
```bash
sudo apt install -y \
  build-essential \
  gcc-multilib \
  libc-dev \ 
  bin86 \
  util-linux
```

### Bochs build dependencies:
```bash
sudo apt install -y \
  libsdl2-dev \
  libx11-dev \
  bison \
  nasm \
  flex \
  pkg-config \
  libreadline-dev \
  bcc
```

### Bochs configure options:
```bash
./configure \
  --enable-debugger \
  --enable-show-ips \
  --with-sdl2 --with-x --with-x11 \
  --enable-cpu-level=6 \
  --enable-all-optimizations
```

## Quick Start
### Run with the pre-built disk image:
```bash
tar xzf hdb.img.tar.gz
bochs -q -f bochsrc.txt
```
### Build the disk image from scratch:
```bash
sudo ./create_hdb.sh
bochs -q -f bochsrc.txt
```

## Blog
- [seojuncha.github.io](https://seojuncha.github.io)
