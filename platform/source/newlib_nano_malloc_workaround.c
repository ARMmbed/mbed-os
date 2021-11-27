/*
 * Copyright (c) 2012, 2013 ARM Ltd
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the company may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ARM LTD ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL ARM LTD BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * NOTE: This file is based on newlib/libc/stdlib/nano-mallocr.c
 * from git://sourceware.org/git/newlib-cygwin.git. It contains a
 * workaround for a bug in newlib 4.1.0:
 *
 * The commit 84d0689 "Nano-malloc: Fix for unwanted external heap
 * fragmentation" from newlib 4.1.0 introduced several optimizations,
 * one of which is as follows:
 *
 * When the last chunk in the free list is smaller than requested,
 * nano_malloc() calls sbrk(0) to see if the heap's current head is
 * adjacent to this chunk, and if so it asks sbrk() for the difference
 * in bytes only and expands the current chunk.
 *
 * This doesn't work if the heap consists of non-contiguous regions.
 * sbrk(0) returns the the current region's head if the latter has any
 * remaining capacity. But if this capacity is not enough for the second
 * (non-trivial) call to sbrk() described above, allocation will happen
 * at the next region if available. Expanding the current chunk won't
 * work and will result in a segmentation fault.
 *
 * So this optimization needs to be reverted in order to bring back
 * compatibility with non-contiguous heaps. Before the next version
 * of newlib becomes available and gets updated in the GCC Arm Embedded
 * Toolchains, we work around this issue by including the fix in Mbed OS.
 * The linker prioritizes malloc() from the project to the one from the
 * toolchain.
 */

#if defined(__NEWLIB_NANO)

#include <newlib.h>

#if (__NEWLIB__ == 4) && (__NEWLIB_MINOR__ == 1) && (__NEWLIB_PATCHLEVEL__ == 0)

/* Implementation of <<malloc>> <<free>> <<calloc>> <<realloc>>, optional
 * as to be reenterable.
 *
 * Interface documentation refer to malloc.c.
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <malloc.h>

#if DEBUG
#include <assert.h>
#else
#define assert(x) ((void)0)
#endif

#ifndef MAX
#define MAX(a,b) ((a) >= (b) ? (a) : (b))
#endif

#define _SBRK_R(X) _sbrk_r(X)

#include <sys/config.h>
#include <reent.h>

#define RARG struct _reent *reent_ptr,
#define RCALL reent_ptr,

#define MALLOC_LOCK __malloc_lock(reent_ptr)
#define MALLOC_UNLOCK __malloc_unlock(reent_ptr)

#define RERRNO reent_ptr->_errno

#define nano_malloc		_malloc_r

/* Redefine names to avoid conflict with user names */
#define free_list __malloc_free_list
#define sbrk_start __malloc_sbrk_start

#define ALIGN_PTR(ptr, align) \
    (((ptr) + (align) - (intptr_t)1) & ~((align) - (intptr_t)1))
#define ALIGN_SIZE(size, align) \
    (((size) + (align) - (size_t)1) & ~((align) - (size_t)1))

/* Alignment of allocated block */
#define MALLOC_ALIGN (8U)
#define CHUNK_ALIGN (sizeof(void*))
#define MALLOC_PADDING ((MAX(MALLOC_ALIGN, CHUNK_ALIGN)) - CHUNK_ALIGN)

/* as well as the minimal allocation size
 * to hold a free pointer */
#define MALLOC_MINSIZE (sizeof(void *))
#define MALLOC_PAGE_ALIGN (0x1000)
#define MAX_ALLOC_SIZE (0x80000000U)

typedef size_t malloc_size_t;

typedef struct malloc_chunk
{
    /*          --------------------------------------
     *   chunk->| size                               |
     *          --------------------------------------
     *          | Padding for alignment              |
     *          | This includes padding inserted by  |
     *          | the compiler (to align fields) and |
     *          | explicit padding inserted by this  |
     *          | implementation. If any explicit    |
     *          | padding is being used then the     |
     *          | sizeof (size) bytes at             |
     *          | mem_ptr - CHUNK_OFFSET must be     |
     *          | initialized with the negative      |
     *          | offset to size.                    |
     *          --------------------------------------
     * mem_ptr->| When allocated: data               |
     *          | When freed: pointer to next free   |
     *          | chunk                              |
     *          --------------------------------------
     */
    /* size of the allocated payload area, including size before
       CHUNK_OFFSET */
    long size;

    /* since here, the memory is either the next free block, or data load */
    struct malloc_chunk * next;
}chunk;


#define CHUNK_OFFSET ((malloc_size_t)(&(((struct malloc_chunk *)0)->next)))

/* size of smallest possible chunk. A memory piece smaller than this size
 * won't be able to create a chunk */
#define MALLOC_MINCHUNK (CHUNK_OFFSET + MALLOC_PADDING + MALLOC_MINSIZE)

/* List list header of free blocks */
chunk * free_list = NULL;

/* Starting point of memory allocated from system */
char * sbrk_start = NULL;

/** Function sbrk_aligned
  * Algorithm:
  *   Use sbrk() to obtain more memory and ensure it is CHUNK_ALIGN aligned
  *   Optimise for the case that it is already aligned - only ask for extra
  *   padding after we know we need it
  */
static void* sbrk_aligned(RARG malloc_size_t s)
{
    char *p, *align_p;

    if (sbrk_start == NULL) sbrk_start = _SBRK_R(RCALL 0);

    p = _SBRK_R(RCALL s);

    /* sbrk returns -1 if fail to allocate */
    if (p == (void *)-1)
        return p;

    align_p = (char*)ALIGN_PTR((uintptr_t)p, CHUNK_ALIGN);
    if (align_p != p)
    {
        /* p is not aligned, ask for a few more bytes so that we have s
         * bytes reserved from align_p. */
        p = _SBRK_R(RCALL align_p - p);
        if (p == (void *)-1)
            return p;
    }
    return align_p;
}

/** Function nano_malloc
  * Algorithm:
  *   Walk through the free list to find the first match. If fails to find
  *   one, call sbrk to allocate a new chunk.
  */
void * nano_malloc(RARG malloc_size_t s)
{
    chunk *p, *r;
    char * ptr, * align_ptr;
    int offset;

    malloc_size_t alloc_size;

    alloc_size = ALIGN_SIZE(s, CHUNK_ALIGN); /* size of aligned data load */
    alloc_size += MALLOC_PADDING; /* padding */
    alloc_size += CHUNK_OFFSET; /* size of chunk head */
    alloc_size = MAX(alloc_size, MALLOC_MINCHUNK);

    if (alloc_size >= MAX_ALLOC_SIZE || alloc_size < s)
    {
        RERRNO = ENOMEM;
        return NULL;
    }

    MALLOC_LOCK;

    p = free_list;
    r = p;

    while (r)
    {
        int rem = r->size - alloc_size;
        if (rem >= 0)
        {
            if (rem >= MALLOC_MINCHUNK)
            {
                if (p == r)
                {
                    /* First item in the list, break it into two chunks
                    *  and return the first one */
                    r->size = alloc_size;
                    free_list = (chunk *)((char *)r + alloc_size);
                    free_list->size = rem;
                    free_list->next = r->next;
                } else {
                    /* Any other item in the list. Split and return
                    * the first one */
                    r->size = alloc_size;
                    p->next = (chunk *)((char *)r + alloc_size);
                    p->next->size = rem;
                    p->next->next = r->next;
                }
            }
            /* Find a chunk that is exactly the size or slightly bigger
             * than requested size, just return this chunk */
            else if (p == r)
            {
                /* Now it implies p==r==free_list. Move the free_list
                 * to next chunk */
                free_list = r->next;
            }
            else
            {
                /* Normal case. Remove it from free_list */
                p->next = r->next;
            }
            break;
        }
        p=r;
        r=r->next;
    }

    /* Failed to find a appropriate chunk. Ask for more memory */
    if (r == NULL)
    {
        r = sbrk_aligned(RCALL alloc_size);

        /* sbrk returns -1 if fail to allocate */
        if (r == (void *)-1)
        {
            RERRNO = ENOMEM;
            MALLOC_UNLOCK;
            return NULL;
        }
        else
        {
            r->size = alloc_size;
        }
    }
    MALLOC_UNLOCK;

    ptr = (char *)r + CHUNK_OFFSET;

    align_ptr = (char *)ALIGN_PTR((uintptr_t)ptr, MALLOC_ALIGN);
    offset = align_ptr - ptr;

    if (offset)
    {
        /* Initialize sizeof (malloc_chunk.size) bytes at
           align_ptr - CHUNK_OFFSET with negative offset to the
           size field (at the start of the chunk).

           The negative offset to size from align_ptr - CHUNK_OFFSET is
           the size of any remaining padding minus CHUNK_OFFSET.  This is
           equivalent to the total size of the padding, because the size of
           any remaining padding is the total size of the padding minus
           CHUNK_OFFSET.

           Note that the size of the padding must be at least CHUNK_OFFSET.

           The rest of the padding is not initialized.  */
        *(long *)((char *)r + offset) = -offset;
    }

    assert(align_ptr + size <= (char *)r + alloc_size);
    return align_ptr;
}

#endif // (__NEWLIB__ == 4) && (__NEWLIB_MINOR__ == 1) && (__NEWLIB_PATCHLEVEL__ == 0)

#endif // defined(__NEWLIB_NANO)
