#define outb(value, port)          \
__asm__ (                          \
        "outb %%al, %%dx"          \
        ::"a"(value), "d"(port)    \
)


#define inb(port) ({          \
unsigned char _v;             \
__asm__ __volatile__ (        \
        "inb %%dx, %%al"      \
        : "=a"(_v)            \
        : "d"(port)           \
);                            \
_v;                           \
})

#define outb_p(value, port)          \
__asm__ (                            \
        "outb %%al, %%dx\n\t"        \
        "jmp 1f\n\t"                 \
        "1:\n\t"                     \
        "jmp 1f\n\t"                 \
        "1:\n\t"                     \
        :: "a"(value), "d"(port)     \
)

#define inb_p(port) ({               \
unsigned char _v;                    \
__asm__ __volatile__ (               \
        "inb %%dx, %%al\n\t"         \
        "jmp 1f\n\t"                 \
        "1:\n\t"                     \
        "jmp 1f\n\t"                 \
        "1:\n\t"                     \
        : "=a"(_v)                   \
        : "d"(port)                  \
);                                   \
_v;                                  \
})
