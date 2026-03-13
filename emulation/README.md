## Emulating with Bochs

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

### Hard Disk Image
```
dd if=/dev/zero of=hdb.img bs=1M count=40
echo 'start=20, size=81900, type=83' | sfdisk -X dos hdb.img
losetup -o 10240 /dev/loop9 hdb.img    # 20x512 = 10240
mkfs.minix -n 14 /dev/loop9
mount /dev/loop9 /mnt/mnt_hdb_img
```

`-H` : the number of header
`-S` : the nubmer of sectors per track
`-C` : the number of cylinders