/*
 * lfs utility functions
 *
 * Copyright (c) 2017 Christopher Haster
 * Distributed under the Apache 2.0 license
 */
#ifndef LFS_UTIL_H
#define LFS_UTIL_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#ifdef __ICCARM__
#include <intrinsics.h>
#endif


// Builtin functions
static inline uint32_t lfs_max(uint32_t a, uint32_t b) {
    return (a > b) ? a : b;
}

static inline uint32_t lfs_min(uint32_t a, uint32_t b) {
    return (a < b) ? a : b;
}

static inline uint32_t lfs_ctz(uint32_t a) {
#ifdef __ICCARM__
    return __CLZ(__REV(a));
#else
    return __builtin_ctz(a);
#endif
}

static inline uint32_t lfs_npw2(uint32_t a) {
#ifdef __ICCARM__
    return 32 - __CLZ(a-1);
#else
    return 32 - __builtin_clz(a-1);
#endif
}

static inline int lfs_scmp(uint32_t a, uint32_t b) {
    return (int)(unsigned)(a - b);
}

void lfs_crc(uint32_t *crc, const void *buffer, size_t size);


// Logging functions
#ifdef __MBED__
#include "mbed_debug.h"
#else
#define debug printf
#endif

#define LFS_DEBUG(fmt, ...) debug("lfs debug: " fmt "\n", __VA_ARGS__)
#define LFS_WARN(fmt, ...)  debug("lfs warn: " fmt "\n", __VA_ARGS__)
#define LFS_ERROR(fmt, ...) debug("lfs error: " fmt "\n", __VA_ARGS__)


#endif
