//
// Guide OS
// Copyright (c) 2022 Codetector
// MIT License
//

#include "string.h"
#include "arch/x86.h"

void*
memset(void *dst, int c, size_t n)
{
    if ((size_t)dst%4 == 0 && n%4 == 0){
        c &= 0xFF;
        stosl(dst, (c<<24)|(c<<16)|(c<<8)|c, n/4);
    } else
        stosb(dst, c, n);
    return dst;
}

int
memcmp(const void *v1, const void *v2, size_t n)
{
    const u8 *s1, *s2;

    s1 = v1;
    s2 = v2;
    while(n-- > 0){
        if(*s1 != *s2)
            return *s1 - *s2;
        s1++, s2++;
    }

    return 0;
}

void*
memmove(void *dst, const void *src, size_t n)
{
    const char *s;
    char *d;

    s = src;
    d = dst;
    if(s < d && s + n > d){
        s += n;
        d += n;
        while(n-- > 0)
            *--d = *--s;
    } else
        while(n-- > 0)
            *d++ = *s++;

    return dst;
}

// memcpy exists to placate GCC.  Use memmove.
void*
memcpy(void *dst, const void *src, size_t n)
{
    return memmove(dst, src, n);
}

int
strncmp(const char *p, const char *q, size_t n)
{
    while(n > 0 && *p && *p == *q)
        n--, p++, q++;
    if(n == 0)
        return 0;
    return (int)((size_t)*p - (size_t)*q);
}

char*
strncpy(char *s, const char *t, size_t n)
{
    char *os;

    os = s;
    while(n-- > 0 && (*s++ = *t++) != 0)
        ;
    while(n-- > 0)
        *s++ = 0;
    return os;
}

size_t
strnlen(const char *s, size_t size)
{
    size_t n;

    for (n = 0; size > 0 && *s != '\0'; s++, size--)
        n++;
    return n;
}



// Like strncpy but guaranteed to NUL-terminate.
char*
safestrcpy(char *s, char *t, size_t n)
{
    char *os;

    os = s;
    if(n <= 0)
        return os;
    while(--n > 0 && (*s++ = *t++) != 0)
        ;
    *s = 0;
    return os;
}

int
strlen(const char *s)
{
    int n;

    for(n = 0; s[n]; n++)
        ;
    return n;
}


int
strcmp(const char *p, const char *q)
{
    while(*p && *p == *q)
        p++, q++;
    return (u8)*p - (u8)*q;
}

char*
strchr(const char *s, char c)
{
    for(; *s; s++)
        if(*s == c)
            return (char*)s;
    return 0;
}

// Return a pointer to the first occurrence of 'c' in 's',
// or a pointer to the string-ending null character if the string has no 'c'.
char *
strfind(const char *s, char c)
{
    for (; *s; s++)
        if (*s == c)
            break;
    return (char*)s;
}

