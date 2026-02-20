```bash
$ sudo apt update
$ sudo apt install -y git wget bear
$ sudo apt install -y build-essential libc-dev bin86 gcc-multilib g++-multilib
```

Compile 
```bash
# Normal compile
$ make
# Produce compile_commands.json
$ bear -- make
```

Compile Output
```bash
gcc -Wall -O -std=gnu89 -fstrength-reduce -fomit-frame-pointer -fno-stack-protector -fno-builtin -g -m32 \
-o tools/build tools/build.c
#chmem +65000 tools/build
objcopy  -O binary -R .note -R .comment tools/system tools/system.bin
tools/build boot/boot tools/system.bin > Image
Boot sector 452 bytes.
System 110209 bytes.
```