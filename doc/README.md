Compile Boch 2.6.11
```
$ sudo apt install libsdl2-dev libncurses-dev nasm unzip bison flex pkg-config libreadline-dev

$ ./configure \
  --enable-debugger \
  --enable-debugger-gui \
  --enable-x86-64  \
  --enable-vmx=2 \
  --enable-show-ips \
  --with-sdl2 \
  --enable-cpu-level=6 \
  --enable-all-optimizations
$ make -j $(nproc)
$ sudo make install     # if you want. default path: /usr/local/
```

BIOS
```
$ ls /usr/local/share/bochs
```

