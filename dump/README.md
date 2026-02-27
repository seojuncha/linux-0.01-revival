Bochs BIOS Disassembly

```bash
$ objdump -D -b binary -m i8086 --adjust-vma=0xF0000 BIOS-bochs-latest > bios.asm
```

