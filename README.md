```bash
sudo apt install build-essential libc-dev bin86 gcc-multilib g++-multilib
```

Compile 
```bash
cd linux-0.01 && make
```

Create a Disk Image
```bash
sudo ./create_hdb.sh
```

```bash
bochs -q -f bochsrc.txt
```

### Boch 2.6.11

Bochs
```bash
git clone -b REL_2_6_11_FINAL https://github.com/bochs-emu/Bochs.git 
```

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

BIOS Disassembly

```bash
$ objdump -D -b binary -m i8086 --adjust-vma=0xF0000 BIOS-bochs-latest > bios.asm
```