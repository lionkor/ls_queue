/* Lion's Standard (LS) type-safe ANSI C queue.
 *
 * Version: 2.1
 * Website: https://libls.org
 * Repo: https://github.com/libls/queue
 * SPDX-License-Identifier: MIT
 *
 * ==== TABLE OF CONTENTS ====
 *
 * 1. DESCRIPTION
 * 2. HOW TO USE
 * 3. LICENSE
 *
 * ==== 1. DESCRIPTION ====
 *
 * A minimal, terse, generic (macro code generated) header-only library in ANSI
 * C, which implements a queue.
 *
 * The implementation does not allocate, and uses a ring-buffer (aka a circular
 * buffer) to avoid copying and moving memory.
 *
 * ==== 2. HOW TO USE ====
 *
 * Statically sized, type-safe queue.
 *
 * Use LS_QUEUE_INLINE to generate a static inline version of the library.
 * This is the "default" behavior.
 *
 * If you need a declaration and implementation separately, use
 * LS_QUEUE_DECL and make sure to call it with the same arguments as
 * LS_QUEUE_IMPL. Put LS_QUEUE_DECL in a header, and LS_QUEUE_IMPL in exactly
 * ONE source file.
 *
 * Simple example:
 *
 *     LS_QUEUE_INLINE(int, int_queue, 32)
 *
 *     // somewhere in the same file
 *     int_queue q;
 *     int_queue_init(&q);
 *     int_queue_push(&q, 42);
 *     int val;
 *     if (int_queue_pop(&q, &val)) {
 *         // do something with val
 *     }
 *
 * Alternative example with decl and inline split:
 *
 *     // In your header file:
 *     LS_QUEUE_DECL(int, int_queue, 32)
 *
 *     // In one source file:
 *     LS_QUEUE_IMPL(int, int_queue, 32)
 *
 *     // Usage in your code:
 *     int_queue q;
 *     int_queue_init(&q);
 *     if (!int_queue_push(&q, 42)) {
 *         // handle queue full
 *     }
 *     int val;
 *     if (int_queue_pop(&q, &val)) {
 *         // do something with val
 *     }
 *
 * ==== 3. LICENSE ====
 *
 * This file is provided under the MIT license. For commercial support and
 * maintenance, feel free to use the e-mail below to contact the author(s).
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2026 Lion Kortlepel <libls@kortlepel.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#pragma once

#include <assert.h>
#include <stddef.h>

#define LS_QUEUE_INLINE(T, name, cap)                                     \
    typedef struct name##_##cap {                                              \
        T data[(cap) + 1];                                                     \
        size_t read;                                                           \
        size_t write;                                                          \
    } name;                                                                    \
    _ls_QUEUE_IMPL_DETAIL(T, name, cap, static)

#define LS_QUEUE_IMPL(T, name, cap)                                       \
    _ls_QUEUE_IMPL_DETAIL(T, name, cap, )

#define LS_QUEUE_DECL(T, name, cap)                                       \
    typedef struct name##_##cap {                                              \
        T data[(cap) + 1];                                                     \
        size_t read;                                                           \
        size_t write;                                                          \
    } name;                                                                    \
    void name##_init(name* q);                                                 \
    /* Returns 0 if full, 1 if successful. */                                  \
    int name##_push(name* q, T val);                                           \
    /* Returns 0 if empty, 1 if successful. */                                 \
    int name##_pop(name* q, T* out);

/* DO NOT USE THE MACRO BELOW. Use LS_QUEUE_INLINE or
 * LS_QUEUE_{IMPL,DECL} instead.
 *
 * What follows are some ramblings about the implementation.
 *
 * You might notice that the queue struct has two different names, once the
 * normal name, e.g. `int_queue`, and once a name with size, `struct
 * int_queue_16`. You might further notice that the functions take the sized
 * version, not the typedef'd version.
 *
 * While this might seem odd, its essentially a poor-man's way to ensure that,
 * if two different int_queue structs are declared, only the one that is defined
 * for will work, and the error should make it painfully clear. For example:
 *
 * > In included file: typedef redefinition with different types ('struct
 *   int_queue_18' vs 'struct int_queue_16') (clang
 *   redefinition_different_typedef)
 *
 * Here you can see that the sizes differ in the declarations: one was declared
 * with 16 elements, the other with 18. However, it's really ugly to use those,
 * so the typedefs exist, and the functions are named without the size
 * specifier, too.
 */
#define _ls_QUEUE_IMPL_DETAIL(T, name, cap, specifier)                    \
    specifier void name##_init(struct name##_##cap* q) {                       \
        assert(q != NULL);                                                     \
        q->read = 0;                                                           \
        q->write = 0;                                                          \
    }                                                                          \
    /* Returns 0 if full, 1 if successful. */                                  \
    specifier int name##_push(struct name##_##cap* q, T val) {                 \
        assert(q != NULL);                                                     \
        size_t size = sizeof(q->data) / sizeof(T);                             \
        size_t new_write = (q->write + 1) % size;                              \
        if (new_write == q->read) {                                            \
            return 0;                                                          \
        }                                                                      \
        q->data[q->write] = val;                                               \
        q->write = new_write;                                                  \
        return 1;                                                              \
    }                                                                          \
    /* Returns 0 if empty, 1 if successful. */                                 \
    specifier int name##_pop(struct name##_##cap* q, T* out) {                 \
        assert(q != NULL);                                                     \
        assert(out != NULL);                                                   \
        size_t size = sizeof(q->data) / sizeof(T);                             \
        if (q->read == q->write) {                                             \
            return 0;                                                          \
        }                                                                      \
        *out = q->data[q->read];                                               \
        q->read = (q->read + 1) % size;                                        \
        return 1;                                                              \
    }
