```bash
sudo apt install build-essential libc-dev bin86 gcc-multilib g++-multilib
```

Compile 
```bash
make
```

Create a Disk Image
```bash
sudo ./create_hdb.sh
```

Compile Output
```bash
gcc -Wall -O -std=gnu89 -fstrength-reduce -fomit-frame-pointer -fno-stack-protector -fno-builtin -g -m32 \
-o tools/build tools/build.c
objcopy  -O binary -R .note -R .comment tools/system tools/system.bin
tools/build boot/boot tools/system.bin > Image
Boot sector 452 bytes.
System 110209 bytes.
```

Emulate
```bash
cd emulation
```

Bochs
```bash
git clone -b REL_2_6_11_FINAL git@github.com:seojuncha/Bochs.git
```
```bash
bochs -q -f bochsrc.txt
```

BIOS Disassembly

```bash
$ objdump -D -b binary -m i8086 --adjust-vma=0xF0000 BIOS-bochs-latest > bios.asm
```

### Compile Boch 2.6.11

```bash
sudo apt install libsdl2-dev libncurses-dev nasm unzip bison flex pkg-config libreadline-dev bcc
```

```bash
./configure \
  --enable-debugger \
  --enable-show-ips \
  --with-sdl2 \
  --enable-cpu-level=6 \
  --enable-all-optimizations
```

```bash
make -j $(nproc)
```

```bash
sudo make install     # if you want. Default path: /usr/local/
```

### Bochs Internal Debugger
```bash
bochs -q -f bochsrc.txt
```

```
<bochs:1> # This is the debugger prompt.
```

#### Load BIOS Symbol in Bochs debugger
```
<bochs:1> ldsym global "rombios.sym"
```

### Where are BIOS files?
```
ls /usr/local/share/bochs
```
