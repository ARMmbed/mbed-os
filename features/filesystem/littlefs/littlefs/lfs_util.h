/*
 * lfs utility functions
 *
 * Copyright (c) 2017 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef LFS_UTIL_H
#define LFS_UTIL_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#ifdef __ICCARM__
#include <intrinsics.h>
#endif


// Builtin functions, these may be replaced by more
// efficient implementations in the system
static inline uint32_t lfs_max(uint32_t a, uint32_t b) {
    return (a > b) ? a : b;
}

static inline uint32_t lfs_min(uint32_t a, uint32_t b) {
    return (a < b) ? a : b;
}

static inline uint32_t lfs_ctz(uint32_t a) {
#if defined(__GNUC__) || defined(__CC_ARM)
    return __builtin_ctz(a);
#elif defined(__ICCARM__) && defined(__CLZ)
    return __CLZ(__RBIT(a));
#else
    uint32_t r = 32;
    a &= -a;
    if (a) r -= 1;
    if (a & 0x0000ffff) r -= 16;
    if (a & 0x00ff00ff) r -= 8;
    if (a & 0x0f0f0f0f) r -= 4;
    if (a & 0x33333333) r -= 2;
    if (a & 0x55555555) r -= 1;
    return r;
#endif
}

static inline uint32_t lfs_npw2(uint32_t a) {
#if defined(__GNUC__) || defined(__CC_ARM)
    return 32 - __builtin_clz(a-1);
#elif defined(__ICCARM__) && defined(__CLZ)
    return 32 - __CLZ(a-1);
#else
    uint32_t r = 0;
    uint32_t s;
    s = (a > 0xffff) << 4; a >>= s; r |= s;
    s = (a > 0xff  ) << 3; a >>= s; r |= s;
    s = (a > 0xf   ) << 2; a >>= s; r |= s;
    s = (a > 0x3   ) << 1; a >>= s; r |= s;
    return r | (a >> 1);
#endif
}

static inline uint32_t lfs_popc(uint32_t a) {
#if defined(__GNUC__) || defined(__CC_ARM)
    return __builtin_popcount(a);
#else
    a = a - ((a >> 1) & 0x55555555);
    a = (a & 0x33333333) + ((a >> 2) & 0x33333333);
    return (((a + (a >> 4)) & 0xf0f0f0f) * 0x1010101) >> 24;
#endif
}

static inline int lfs_scmp(uint32_t a, uint32_t b) {
    return (int)(unsigned)(a - b);
}

// CRC-32 with polynomial = 0x04c11db7
void lfs_crc(uint32_t *crc, const void *buffer, size_t size);


// Logging functions
#ifdef __MBED__
#include "mbed_debug.h"
#else
#define MBED_LFS_ENABLE_INFO  false
#define MBED_LFS_ENABLE_DEBUG true
#define MBED_LFS_ENABLE_WARN  true
#define MBED_LFS_ENABLE_ERROR true
#endif

#if MBED_LFS_ENABLE_INFO
#define LFS_INFO(fmt, ...)  printf("lfs info: " fmt "\n", __VA_ARGS__)
#elif !defined(MBED_LFS_ENABLE_INFO)
#define LFS_INFO(fmt, ...)  debug("lfs info: " fmt "\n", __VA_ARGS__)
#else
#define LFS_INFO(fmt, ...)
#endif

#if MBED_LFS_ENABLE_DEBUG
#define LFS_DEBUG(fmt, ...)  printf("lfs debug: " fmt "\n", __VA_ARGS__)
#elif !defined(MBED_LFS_ENABLE_DEBUG)
#define LFS_DEBUG(fmt, ...)  debug("lfs debug: " fmt "\n", __VA_ARGS__)
#else
#define LFS_DEBUG(fmt, ...)
#endif

#if MBED_LFS_ENABLE_WARN
#define LFS_WARN(fmt, ...)  printf("lfs warn: " fmt "\n", __VA_ARGS__)
#elif !defined(MBED_LFS_ENABLE_WARN)
#define LFS_WARN(fmt, ...)  debug("lfs warn: " fmt "\n", __VA_ARGS__)
#else
#define LFS_WARN(fmt, ...)
#endif

#if MBED_LFS_ENABLE_ERROR
#define LFS_ERROR(fmt, ...)  printf("lfs error: " fmt "\n", __VA_ARGS__)
#elif !defined(MBED_LFS_ENABLE_ERROR)
#define LFS_ERROR(fmt, ...)  debug("lfs error: " fmt "\n", __VA_ARGS__)
#else
#define LFS_ERROR(fmt, ...)
#endif


#endif
