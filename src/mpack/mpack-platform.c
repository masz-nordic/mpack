/*
 * Copyright (c) 2015 Nicholas Fraser
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#define MPACK_INTERNAL 1

#include "mpack-platform.h"

#if MPACK_DEBUG && MPACK_STDIO
#include <stdarg.h>
#endif



#if MPACK_DEBUG && MPACK_STDIO
void mpack_assert_fail_format(const char* format, ...) {
    char buffer[512];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    buffer[sizeof(buffer) - 1] = 0;
    mpack_assert_fail(buffer);
}

void mpack_break_hit_format(const char* format, ...) {
    char buffer[512];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    buffer[sizeof(buffer) - 1] = 0;
    mpack_break_hit(buffer);
}
#endif

#if MPACK_CUSTOM_ASSERT
void mpack_break_hit(const char* message) {
    // If we have a custom assert handler, break just wraps it
    // for simplicity.
    mpack_assert_fail(message);
}
#else
void mpack_assert_fail(const char* message) {
    MPACK_UNUSED(message);

    #if MPACK_STDIO
    fprintf(stderr, "%s\n", message);
    #endif

    #if defined(__GCC__) || defined(__clang__)
    __builtin_trap();
    #elif defined(WIN32)
    __debugbreak();
    #endif

    #if MPACK_STDLIB
    abort();
    #elif defined(__GCC__) || defined(__clang__)
    __builtin_abort();
    #endif

    MPACK_UNREACHABLE;
}

void mpack_break_hit(const char* message) {
    MPACK_UNUSED(message);

    #if MPACK_STDIO
    fprintf(stderr, "%s\n", message);
    #endif

    #if defined(__GCC__) || defined(__clang__)
    __builtin_trap();
    #elif defined(WIN32)
    __debugbreak();
    #elif MPACK_STDLIB
    abort();
    #elif defined(__GCC__) || defined(__clang__)
    __builtin_abort();
    #endif
}
#endif



#if !MPACK_STDLIB

// The below are adapted from the C wikibook:
//     https://en.wikibooks.org/wiki/C_Programming/Strings

void* mpack_memset(void *s, int c, size_t n) {
    unsigned char *us = (unsigned char *)s;
    unsigned char uc = (unsigned char)c;
    while (n-- != 0)
        *us++ = uc;
    return s;
}

void* mpack_memcpy(void *s1, const void *s2, size_t n) {
    char * __restrict dst = (char *)s1;
    const char * __restrict src = (const char *)s2;
    while (n-- != 0)
        *dst++ = *src++;
    return s1;
}

void* mpack_memmove(void *s1, const void *s2, size_t n) {
    char *p1 = (char *)s1;
    const char *p2 = (const char *)s2;
    if (p2 < p1 && p1 < p2 + n) {
        p2 += n;
        p1 += n;
        while (n-- != 0)
            *--p1 = *--p2;
    } else
        while (n-- != 0)
            *p1++ = *p2++;
    return s1;
}

int mpack_memcmp(const void* s1, const void* s2, size_t n) {
     const unsigned char *us1 = (const unsigned char *) s1;
     const unsigned char *us2 = (const unsigned char *) s2;
     while (n-- != 0) {
         if (*us1 != *us2)
             return (*us1 < *us2) ? -1 : +1;
         us1++;
         us2++;
     }
     return 0;
}

size_t mpack_strlen(const char *s) {
    const char *p = s;
    while (*p != '\0')
        p++;
    return (size_t)(p - s);
}

#endif



#if defined(MPACK_MALLOC) && !defined(MPACK_REALLOC)
void* mpack_realloc(void* old_ptr, size_t used_size, size_t new_size) {
    void* new_ptr = malloc(new_size);
    if (new_ptr == NULL)
        return NULL;
    mpack_memcpy(new_ptr, old_ptr, used_size);
    MPACK_FREE(old_ptr);
    return new_ptr;
}
#endif
