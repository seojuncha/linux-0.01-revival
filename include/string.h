#ifndef _STRING_H_
#define _STRING_H_

#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned int size_t;
#endif

extern char *strerror(int errno);

/* gcc -m32 전제 (i386) */

#include <stddef.h>

static inline char *strcpy(char *dest, const char *src)
{
    char *ret = dest;
    __asm__ volatile (
        "cld\n"
        "1:\n\t"
        "lodsb\n\t"
        "stosb\n\t"
        "testb %%al, %%al\n\t"
        "jne 1b"
        : "+S"(src), "+D"(dest)
        :
        : "eax", "cc", "memory"
    );
    return ret;
}

static inline char *strncpy(char *dest, const char *src, int count)
{
    char *ret = dest;
    __asm__ volatile (
        "cld\n"
        "1:\tdecl %2\n\t"
        "js 2f\n\t"
        "lodsb\n\t"
        "stosb\n\t"
        "testb %%al,%%al\n\t"
        "jne 1b\n\t"
        "rep\n\t"
        "stosb\n"
        "2:"
        : "+S"(src), "+D"(dest), "+c"(count)
        :
        : "eax", "cc", "memory"
    );
    return ret;
}

static inline char *strcat(char *dest, const char *src)
{
    char *ret = dest;
    unsigned int cnt = 0xffffffffu;
    __asm__ volatile (
        "cld\n\t"
        "repne\n\t"
        "scasb\n\t"
        "decl %0\n"
        "1:\tlodsb\n\t"
        "stosb\n\t"
        "testb %%al,%%al\n\t"
        "jne 1b"
        : "+D"(dest), "+S"(src), "+c"(cnt)
        : "a"(0)
        : "cc", "memory"
    );
    return ret;
}

static inline char *strncat(char *dest, const char *src, int count)
{
    char *ret = dest;
    unsigned int scan = 0xffffffffu;
    __asm__ volatile (
        "cld\n\t"
        "repne\n\t"
        "scasb\n\t"
        "decl %0\n\t"
        "movl %3, %2\n"
        "1:\tdecl %2\n\t"
        "js 2f\n\t"
        "lodsb\n\t"
        "stosb\n\t"
        "testb %%al,%%al\n\t"
        "jne 1b\n"
        "2:\txorl %%eax,%%eax\n\t"
        "stosb"
        : "+D"(dest), "+S"(src), "+c"(scan)
        : "g"(count), "a"(0)
        : "cc", "memory"
    );
    return ret;
}

static inline int strcmp(const char *cs, const char *ct)
{
    int res;
    __asm__ volatile (
        "cld\n"
        "1:\tlodsb\n\t"
        "scasb\n\t"
        "jne 2f\n\t"
        "testb %%al,%%al\n\t"
        "jne 1b\n\t"
        "xorl %%eax,%%eax\n\t"
        "jmp 3f\n"
        "2:\tmovl $1,%%eax\n\t"
        "jl 3f\n\t"
        "negl %%eax\n"
        "3:"
        : "=a"(res), "+D"(cs), "+S"(ct)
        :
        : "cc", "memory"
    );
    return res;
}

static inline int strncmp(const char *cs, const char *ct, int count)
{
    int res;
    __asm__ volatile (
        "cld\n"
        "1:\tdecl %3\n\t"
        "js 2f\n\t"
        "lodsb\n\t"
        "scasb\n\t"
        "jne 3f\n\t"
        "testb %%al,%%al\n\t"
        "jne 1b\n"
        "2:\txorl %%eax,%%eax\n\t"
        "jmp 4f\n"
        "3:\tmovl $1,%%eax\n\t"
        "jl 4f\n\t"
        "negl %%eax\n"
        "4:"
        : "=a"(res), "+D"(cs), "+S"(ct), "+c"(count)
        :
        : "cc", "memory"
    );
    return res;
}

static inline char *strchr(const char *s, char c)
{
    char *res;
    __asm__ volatile (
        "cld\n\t"
        "movb %%al,%%ah\n"
        "1:\tlodsb\n\t"
        "cmpb %%ah,%%al\n\t"
        "je 2f\n\t"
        "testb %%al,%%al\n\t"
        "jne 1b\n\t"
        "movl $1,%1\n"
        "2:\tmovl %1,%0\n\t"
        "decl %0"
        : "=a"(res), "+S"(s)
        : "0"(c)
        : "eax", "cc", "memory"
    );
    return res;
}

static inline char *strrchr(const char *s, char c)
{
    char *res;
    __asm__ volatile (
        "cld\n\t"
        "movb %%al,%%ah\n"
        "1:\tlodsb\n\t"
        "cmpb %%ah,%%al\n\t"
        "jne 2f\n\t"
        "movl %%esi,%0\n\t"
        "decl %0\n"
        "2:\ttestb %%al,%%al\n\t"
        "jne 1b"
        : "=d"(res), "+S"(s)
        : "a"(c)
        : "eax", "cc", "memory"
    );
    return res;
}

static inline int strspn(const char *cs, const char *ct)
{
    const char *p = cs;
    __asm__ volatile (
        "cld\n\t"
        "movl %2,%%edi\n\t"
        "repne\n\t"
        "scasb\n\t"
        "notl %%ecx\n\t"
        "decl %%ecx\n\t"
        "movl %%ecx,%%edx\n"
        "1:\tlodsb\n\t"
        "testb %%al,%%al\n\t"
        "je 2f\n\t"
        "movl %2,%%edi\n\t"
        "movl %%edx,%%ecx\n\t"
        "repne\n\t"
        "scasb\n\t"
        "je 1b\n"
        "2:\tdecl %0"
        : "+S"(p)
        : "a"(0), "g"(ct), "c"(0xffffffffu)
        : "edx", "edi", "cc", "memory"
    );
    return (int)(p - cs);
}

static inline int strcspn(const char *cs, const char *ct)
{
    const char *p = cs;
    __asm__ volatile (
        "cld\n\t"
        "movl %2,%%edi\n\t"
        "repne\n\t"
        "scasb\n\t"
        "notl %%ecx\n\t"
        "decl %%ecx\n\t"
        "movl %%ecx,%%edx\n"
        "1:\tlodsb\n\t"
        "testb %%al,%%al\n\t"
        "je 2f\n\t"
        "movl %2,%%edi\n\t"
        "movl %%edx,%%ecx\n\t"
        "repne\n\t"
        "scasb\n\t"
        "jne 1b\n"
        "2:\tdecl %0"
        : "+S"(p)
        : "a"(0), "g"(ct), "c"(0xffffffffu)
        : "edx", "edi", "cc", "memory"
    );
    return (int)(p - cs);
}

static inline char *strpbrk(const char *cs, const char *ct)
{
    const char *p = cs;
    __asm__ volatile (
        "cld\n\t"
        "movl %2,%%edi\n\t"
        "repne\n\t"
        "scasb\n\t"
        "notl %%ecx\n\t"
        "decl %%ecx\n\t"
        "movl %%ecx,%%edx\n"
        "1:\tlodsb\n\t"
        "testb %%al,%%al\n\t"
        "je 2f\n\t"
        "movl %2,%%edi\n\t"
        "movl %%edx,%%ecx\n\t"
        "repne\n\t"
        "scasb\n\t"
        "jne 1b\n\t"
        "decl %0\n\t"
        "jmp 3f\n"
        "2:\txorl %0,%0\n"
        "3:"
        : "+S"(p)
        : "a"(0), "g"(ct), "c"(0xffffffffu)
        : "edx", "edi", "cc", "memory"
    );
    return (char*)p;
}

static inline char *strstr(const char *cs, const char *ct)
{
    char *res;
    __asm__ volatile (
        "cld\n\t"
        "movl %4,%%edi\n\t"
        "repne\n\t"
        "scasb\n\t"
        "notl %%ecx\n\t"
        "decl %%ecx\n\t"
        "movl %%ecx,%%edx\n"
        "1:\tmovl %4,%%edi\n\t"
        "movl %%esi,%%eax\n\t"
        "movl %%edx,%%ecx\n\t"
        "repe\n\t"
        "cmpsb\n\t"
        "je 2f\n\t"
        "xchgl %%eax,%%esi\n\t"
        "incl %%esi\n\t"
        "cmpb $0,-1(%%eax)\n\t"
        "jne 1b\n\t"
        "xorl %%eax,%%eax\n"
        "2:"
        : "=a"(res), "+S"(cs)
        : "0"(0), "c"(0xffffffffu), "g"(ct)
        : "edx", "edi", "cc", "memory"
    );
    return res;
}

static inline int strlen(const char *s)
{
    int res;
    unsigned int cnt = 0xffffffffu;
    __asm__ volatile (
        "cld\n\t"
        "repne\n\t"
        "scasb\n\t"
        "notl %0\n\t"
        "decl %0"
        : "+c"(cnt), "+D"(s)
        : "a"(0)
        : "cc", "memory"
    );
    res = (int)cnt;
    return res;
}

/* strtok: inline asm을 유지하려면 PIC/PIE 및 EBX 고정 등 때문에 현대 GCC에서 매우 취약.
   컴파일 목적이면 C 구현으로 교체가 가장 안정적임. */
static char *___strtok;

static inline char *strtok(char *s, const char *delim)
{
    char *p = s ? s : ___strtok;
    if (!p) return NULL;

    /* skip leading delimiters */
    while (*p) {
        const char *d = delim;
        int hit = 0;
        while (*d) { if (*p == *d) { hit = 1; break; } d++; }
        if (!hit) break;
        p++;
    }
    if (!*p) { ___strtok = NULL; return NULL; }

    char *start = p;

    /* scan token */
    while (*p) {
        const char *d = delim;
        while (*d) {
            if (*p == *d) {
                *p = '\0';
                ___strtok = p + 1;
                return start;
            }
            d++;
        }
        p++;
    }
    ___strtok = NULL;
    return start;
}

static inline void *memcpy(void *dest, const void *src, int n)
{
    void *ret = dest;
    __asm__ volatile (
        "cld\n\t"
        "rep\n\t"
        "movsb"
        : "+D"(dest), "+S"(src), "+c"(n)
        :
        : "cc", "memory"
    );
    return ret;
}

static inline void *memmove(void *dest, const void *src, int n)
{
    void *ret = dest;
    if (dest < src) {
        __asm__ volatile (
            "cld\n\t"
            "rep\n\t"
            "movsb"
            : "+D"(dest), "+S"(src), "+c"(n)
            :
            : "cc", "memory"
        );
    } else if (n > 0) {
        const char *s = (const char*)src + n - 1;
        char *d = (char*)dest + n - 1;
        __asm__ volatile (
            "std\n\t"
            "rep\n\t"
            "movsb\n\t"
            "cld"
            : "+D"(d), "+S"(s), "+c"(n)
            :
            : "cc", "memory"
        );
    }
    return ret;
}

static inline int memcmp(const void *cs, const void *ct, int count)
{
    int res;
    __asm__ volatile (
        "cld\n\t"
        "repe\n\t"
        "cmpsb\n\t"
        "je 1f\n\t"
        "movl $1,%%eax\n\t"
        "jl 1f\n\t"
        "negl %%eax\n"
        "1:"
        : "=a"(res), "+D"(cs), "+S"(ct), "+c"(count)
        : "0"(0)
        : "cc", "memory"
    );
    return res;
}

static inline void *memchr(const void *cs, char c, int count)
{
    void *res;
    if (!count) return NULL;
    __asm__ volatile (
        "cld\n\t"
        "repne\n\t"
        "scasb\n\t"
        "je 1f\n\t"
        "xorl %0,%0\n\t"
        "jmp 2f\n"
        "1:\tdecl %0\n"
        "2:"
        : "=&D"(res), "+c"(count)
        : "a"(c), "0"(cs)
        : "cc", "memory"
    );
    return res;
}

static inline void *memset(void *s, char c, int count)
{
    void *ret = s;
    __asm__ volatile (
        "cld\n\t"
        "rep\n\t"
        "stosb"
        : "+D"(s), "+c"(count)
        : "a"(c)
        : "cc", "memory"
    );
    return ret;
}


#endif
