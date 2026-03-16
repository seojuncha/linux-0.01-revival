## Boot Code

### Ralf Brown's Interrupt List (RBIL)

### General Registers

### Segment Registers

### Global/Interrupt Descriptor Table

### TIP 1: Address Caculation

`0xFFFF`:
- address size: 16-bit
- memory size: 64K

```python
>>> import math
>>> math.log2(0xffff+1)
16.0
```
So, `2^16-1=0xFFFF`.
`2^16` is `2^10 * 2^6`.
`2^10 = 1024 = 1K`
`1K * 2^6 = 1K * 64 = 64K`


Code Segment and Offset
- CS[15:0]:IP[31:0]
- Real Address Mode
  -  
- Protected Mode
  -

About word size
```asm
  .word 0       ; 16-bit in 80386. so, 0x0000
  .word 0, 0    ; two word => 32-bit. so, 0x0000_0000
```

Segment Selector
```
; 20-bit
0x00000     ; five hex numbers because each hex number has 4-bit.

```

### TIP 2: Frequently Used Instructions
- `mov`
- `int`
- `xor`