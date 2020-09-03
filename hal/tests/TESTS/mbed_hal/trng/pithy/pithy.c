//
//  pithy.c
//  http://github.com/johnezang/pithy
//  Licensed under the terms of the BSD License, as specified below.
//
// SPDX-License-Identifier: BSD-3-Clause

/*
 Copyright (c) 2011, John Engelhart

 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.

 * Neither the name of the Zang Industries nor the names of its
 contributors may be used to endorse or promote products derived from
 this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#if defined(__arm__) && defined(__ARM_NEON__)
#include <arm_neon.h>
#endif

#include "pithy.h"

#define kBlockLog   20ul
#define kBlockSize  ((size_t)(1 << kBlockLog))

// The maximum size that can be compressed while still allowing for the worst case compression expansion.
#define PITHY_UNCOMPRESSED_MAX_LENGTH 0xdb6db6bfu // 0xdb6db6bf == 3681400511, or 3510.857 Mbytes.

typedef const char *pithy_hashOffset_t;

enum {
    PITHY_LITERAL            = 0,
    PITHY_COPY_1_BYTE_OFFSET = 1,
    PITHY_COPY_2_BYTE_OFFSET = 2,
    PITHY_COPY_3_BYTE_OFFSET = 3
};


#if       defined (__GNUC__) && (__GNUC__ >= 3)
#define PITHY_ATTRIBUTES(attr, ...)        __attribute__((attr, ##__VA_ARGS__))
#define PITHY_EXPECTED(cond, expect)       __builtin_expect((long)(cond), (expect))
#define PITHY_EXPECT_T(cond)               PITHY_EXPECTED(cond, 1u)
#define PITHY_EXPECT_F(cond)               PITHY_EXPECTED(cond, 0u)
#define PITHY_PREFETCH(ptr)                __builtin_prefetch(ptr)
#else  // defined (__GNUC__) && (__GNUC__ >= 3) 
#define PITHY_ATTRIBUTES(attr, ...)
#define PITHY_EXPECTED(cond, expect)       (cond)
#define PITHY_EXPECT_T(cond)               (cond)
#define PITHY_EXPECT_F(cond)               (cond)
#define PITHY_PREFETCH(ptr)
#endif // defined (__GNUC__) && (__GNUC__ >= 3) 

#define PITHY_STATIC_INLINE        static inline PITHY_ATTRIBUTES(always_inline)
#define PITHY_ALIGNED(x)                         PITHY_ATTRIBUTES(aligned(x))

#if defined(NS_BLOCK_ASSERTIONS) && !defined(NDEBUG)
#define NDEBUG
#endif

#ifdef NDEBUG
#define DCHECK(condition)
#else
#define DCHECK(condition) do {                                                  \
  if(PITHY_EXPECT_F(!(condition))) {                                            \
    fprintf(stderr, "%s / %s @ %ld: Invalid parameter not satisfying: %s",      \
    __FILE__, __PRETTY_FUNCTION__, (long)__LINE__, #condition); fflush(stderr); \
    abort();                                                                    \
    }                                                                           \
  } while(0)
#endif

PITHY_STATIC_INLINE const char *pithy_Parse32WithLimit(const char *p, const char *l, size_t *OUTPUT);
PITHY_STATIC_INLINE char       *pithy_Encode32(char *ptr, uint32_t v);

PITHY_STATIC_INLINE uint32_t    pithy_GetUint32AtOffset(uint64_t v, uint32_t offset);
PITHY_STATIC_INLINE uint32_t    pithy_HashBytes(uint32_t bytes, uint32_t shift);
PITHY_STATIC_INLINE size_t      pithy_FindMatchLength(const char *s1, const char *s2, const char *s2_limit);
PITHY_STATIC_INLINE char       *pithy_EmitLiteral(char *op, const char *literal, size_t len, int allow_fast_path);
PITHY_STATIC_INLINE char       *pithy_EmitCopyGreaterThan63(char *op, size_t offset, size_t len);
PITHY_STATIC_INLINE char       *pithy_EmitCopyLessThan63(char *op, size_t offset, size_t len);
PITHY_STATIC_INLINE char       *pithy_EmitCopy(char *op, size_t offset, size_t len);

PITHY_STATIC_INLINE uint16_t pithy_Load16(const void *p)        { uint16_t t; memcpy(&t, p, sizeof(t)); return (t); }
PITHY_STATIC_INLINE uint32_t pithy_Load32(const void *p)        { uint32_t t; memcpy(&t, p, sizeof(t)); return (t); }
PITHY_STATIC_INLINE uint64_t pithy_Load64(const void *p)        { uint64_t t; memcpy(&t, p, sizeof(t)); return (t); }
PITHY_STATIC_INLINE void     pithy_Store16(void *p, uint16_t v) { memcpy(p, &v, sizeof(v)); }
PITHY_STATIC_INLINE void     pithy_Store32(void *p, uint32_t v) { memcpy(p, &v, sizeof(v)); }
PITHY_STATIC_INLINE void     pithy_Store64(void *p, uint64_t v) { memcpy(p, &v, sizeof(v)); }

#define pithy_Move64(dst,src)  pithy_Store64(dst, pithy_Load64(src));
#define pithy_Move128(dst,src) pithy_Move64(dst, src); pithy_Move64(dst + 8ul, src + 8ul);

#ifdef __BIG_ENDIAN__

#ifdef _MSC_VER
#include <stdlib.h>
#define pithy_bswap_16(x) _byteswap_ushort(x)
#define pithy_bswap_32(x) _byteswap_ulong(x)
#define pithy_bswap_64(x) _byteswap_uint64(x)

#elif defined(__APPLE__)

// Mac OS X / Darwin features
#include <libkern/OSByteOrder.h>
#define pithy_bswap_16(x) OSSwapInt16(x)
#define pithy_bswap_32(x) OSSwapInt32(x)
#define pithy_bswap_64(x) OSSwapInt64(x)
#else
#include <byteswap.h>
#endif

#endif  // __BIG_ENDIAN__

// Conversion functions.
#ifdef __BIG_ENDIAN__
#define pithy_FromHost16(x)    ((uint16_t)pithy_bswap_16(x))
#define pithy_ToHost16(x)      ((uint16_t)pithy_bswap_16(x))
#define pithy_FromHost32(x)    ((uint32_t)pithy_bswap_32(x))
#define pithy_ToHost32(x)      ((uint32_t)pithy_bswap_32(x))
#define pithy_IsLittleEndian() (0)

#else  // !defined(__BIG_ENDIAN__)
#define pithy_FromHost16(x)    ((uint16_t)(x))
#define pithy_ToHost16(x)      ((uint16_t)(x))
#define pithy_FromHost32(x)    ((uint32_t)(x))
#define pithy_ToHost32(x)      ((uint32_t)(x))
#define pithy_IsLittleEndian() (1)

#endif  // !defined(__BIG_ENDIAN__)

#define pithy_LoadHost16(p)     pithy_ToHost16(pithy_Load16((const void *)(p)))
#define pithy_StoreHost16(p, v) pithy_Store16((void *)(p), pithy_FromHost16(v))
#define pithy_LoadHost32(p)     pithy_ToHost32(pithy_Load32((p)))
#define pithy_StoreHost32(p, v) pithy_Store32((p), pithy_FromHost32(v))

PITHY_STATIC_INLINE void pithy_StoreHost24(char *p, uint32_t v)
{
    *p++ = (v & 0xffu);
    *p++ = ((v >> 8) & 0xffu);
    *p++ = ((v >> 16) & 0xffu);
}

#if defined (__GNUC__) && (__GNUC__ >= 3)

#define pithy_Log2Floor(n)           ({typeof(n) _n = (n); _n == 0 ? (int)-1 : (int)(31 ^ __builtin_clz(_n));})
#define pithy_FindLSBSetNonZero32(n) ((int)__builtin_ctz((uint32_t)(n)))
#define pithy_FindLSBSetNonZero64(n) ((int)__builtin_ctzll((uint64_t)(n)))
#define pithy_FindMSBSetNonZero32(n) ((int)__builtin_clz((uint32_t)(n)))
#define pithy_FindMSBSetNonZero64(n) ((int)__builtin_clzll((uint64_t)(n)))

#else  // Portable versions, !GNUC || GNUC < 3

PITHY_STATIC_INLINE int pithy_Log2Floor(uint32_t n)
{
    if (n == 0u) {
        return (-1);
    }
    int i = 0, log = 0;
    uint32_t value = n;
    for (i = 4; i >= 0; --i) {
        int shift = (1 << i);
        uint32_t x = value >> shift;
        if (x != 0u) {
            value = x;
            log += shift;
        }
    }
    DCHECK(value == 1);
    return (log);
}

PITHY_STATIC_INLINE int pithy_FindLSBSetNonZero32(uint32_t n)
{
    int i = 0, rc = 31, shift = 0;
    for (i = 4, shift = 1 << 4; i >= 0; --i) {
        const uint32_t x = n << shift;
        if (x != 0u) {
            n = x;
            rc -= shift;
        }
        shift >>= 1;
    }
    return (rc);
}

PITHY_STATIC_INLINE int pithy_FindLSBSetNonZero64(uint64_t n)
{
    const uint32_t bottomBits = n;
    if (bottomBits == 0u) {
        return (32 + pithy_FindLSBSetNonZero32((n >> 32)));
    } else {
        return (pithy_FindLSBSetNonZero32(bottomBits));
    }
}

PITHY_STATIC_INLINE int pithy_FindMSBSetNonZero32(uint32_t n)
{
    int i;
    uint32_t x, rc = 32, shift = 1 << 4;
    for (i = 3; i >= 0; --i) {
        x = n >> shift;
        if (x != 0) {
            rc -= shift;
            n = x;
        }
        shift >>= 1;
    }
    x = n >> shift;
    return (rc - ((x != 0) ? 2 : n));
}

PITHY_STATIC_INLINE int pithy_FindMSBSetNonZero64(uint64_t n)
{
    const uint32_t upperBits = n >> 32;
    if (upperBits == 0u) {
        return (32 + pithy_FindMSBSetNonZero32((n)));
    } else {
        return (pithy_FindMSBSetNonZero32(upperBits));
    }
}

#endif  // End portable versions.

PITHY_STATIC_INLINE const char *pithy_Parse32WithLimit(const char *p, const char *l, size_t *resultOut)
{
    const unsigned char *ptr = (const unsigned char *)p, *limit = (const unsigned char *)l;
    size_t   resultShift = 0ul, result = 0ul;
    uint32_t encodedByte = 0u;

    for (resultShift = 0ul; resultShift <= 28ul; resultShift += 7ul) {
        if (ptr >= limit) {
            return (NULL);
        }
        encodedByte = *(ptr++);
        result |= (encodedByte & 127u) << resultShift;
        if (encodedByte < ((resultShift == 28ul) ? 16u : 128u)) {
            goto done;
        }
    }
    return (NULL);
done:
    *resultOut = result;
    return ((const char *)ptr);
}

PITHY_STATIC_INLINE char *pithy_Encode32(char *sptr, uint32_t v)
{
    unsigned char *ptr = (unsigned char *)sptr;
    if (v < (1u <<  7)) {
        *(ptr++) = v;
    } else if (v < (1u << 14)) {
        *(ptr++) = v | 0x80u;
        *(ptr++) = (v >> 7);
    } else if (v < (1u << 21)) {
        *(ptr++) = v | 0x80u;
        *(ptr++) = (v >> 7) | 0x80u;
        *(ptr++) = (v >> 14);
    } else if (v < (1u << 28)) {
        *(ptr++) = v | 0x80u;
        *(ptr++) = (v >> 7) | 0x80u;
        *(ptr++) = (v >> 14) | 0x80u;
        *(ptr++) = (v >> 21);
    } else {
        *(ptr++) = v | 0x80u;
        *(ptr++) = (v >> 7) | 0x80u;
        *(ptr++) = (v >> 14) | 0x80u;
        *(ptr++) = (v >> 21) | 0x80u;
        *(ptr++) = (v >> 28);
    }
    return ((char *)ptr);
}


PITHY_STATIC_INLINE uint32_t pithy_GetUint32AtOffset(uint64_t v, uint32_t offset)
{
    DCHECK(offset <= 4);
    return (v >> (pithy_IsLittleEndian() ? (8u * offset) : (32u - (8u * offset))));
}


PITHY_STATIC_INLINE uint32_t pithy_HashBytes(uint32_t bytes, uint32_t shift)
{
    uint32_t kMul = 0x1e35a7bdU;
    return ((bytes * kMul) >> shift);
}


PITHY_STATIC_INLINE size_t pithy_FindMatchLength(const char *s1, const char *s2, const char *s2_limit)
{
    DCHECK(s2_limit >= s2);
    const char *ms1 = s1, *ms2 = s2;

#if defined(__LP64__)
    while (PITHY_EXPECT_T(ms2 < (s2_limit - 8ul))) {
        uint64_t x = pithy_Load64(ms1) ^ pithy_Load64(ms2);
        if (PITHY_EXPECT_F(x == 0ul))  {
            ms1 += 8ul;
            ms2 += 8ul;
        } else {
            return ((ms1 - s1) + ((unsigned int)(pithy_IsLittleEndian() ? (pithy_FindLSBSetNonZero64(x) >> 3) :
                                                 (pithy_FindMSBSetNonZero64(x) >> 3))));
        }
    }
#else
    while (PITHY_EXPECT_T(ms2 < (s2_limit - 4u ))) {
        uint32_t x = pithy_Load32(ms1) ^ pithy_Load32(ms2);
        if (PITHY_EXPECT_F(x == 0u))  {
            ms1 += 4u;
            ms2 += 4u;
        } else {
            return ((ms1 - s1) + ((unsigned int)(pithy_IsLittleEndian() ?
                                    (pithy_FindLSBSetNonZero32(x) >> 3) : (pithy_FindMSBSetNonZero32(x) >> 3))));
        }
    }
#endif
    while (PITHY_EXPECT_T(ms2 <  s2_limit)) {
        if (PITHY_EXPECT_T(*ms1 == *ms2)) {
            ms1++;
            ms2++;
        } else {
            return (ms1 - s1);
        }
    }
    return (ms1 - s1);
}


PITHY_STATIC_INLINE char *pithy_EmitLiteral(char *op, const char *literal, size_t len, int allow_fast_path)
{
    int n = len - 1l;
    if (PITHY_EXPECT_T(n < 60l)) {
        *op++ = PITHY_LITERAL | (n << 2);
        if (PITHY_EXPECT_T(allow_fast_path) && PITHY_EXPECT_T(len <= 16ul)) {
            pithy_Move128(op, literal);
            return (op + len);
        }
    } else {
        char *base = op;
        int count = 0;
        op++;
        while (n > 0l) {
            *op++ = n & 0xff;
            n >>= 8;
            count++;
        }
        DCHECK((count >= 1) && (count <= 4));
        *base = PITHY_LITERAL | ((59 + count) << 2);
    }
    memcpy(op, literal, len);
    return (op + len);
}

PITHY_STATIC_INLINE char *pithy_EmitCopyGreaterThan63(char *op, size_t offset, size_t len)
{
    DCHECK((len < 65536ul) && (len >= 63ul) && (offset < kBlockSize));
    if (PITHY_EXPECT_T(offset < 65536ul)) {
        if (PITHY_EXPECT_T(len < (256ul + 63ul))) {
            *op++ = PITHY_COPY_2_BYTE_OFFSET | (62 << 2);
            pithy_StoreHost16(op, offset);
            op += 2ul;
            *op++ = (len - 63ul);
        } else {
            *op++ = PITHY_COPY_2_BYTE_OFFSET | (63 << 2);
            pithy_StoreHost16(op, offset);
            op += 2ul;
            pithy_StoreHost16(op, len);
            op += 2ul;
        }
    } else {
        if (PITHY_EXPECT_T(len < (256ul + 63ul))) {
            *op++ = PITHY_COPY_3_BYTE_OFFSET | (62 << 2);
            pithy_StoreHost24(op, offset);
            op += 3ul;
            *op++ = (len - 63ul);
        } else {
            *op++ = PITHY_COPY_3_BYTE_OFFSET | (63 << 2);
            pithy_StoreHost24(op, offset);
            op += 3ul;
            pithy_StoreHost16(op, len);
            op += 2ul;
        }
    }
    return (op);
}

PITHY_STATIC_INLINE char *pithy_EmitCopyLessThan63(char *op, size_t offset, size_t len)
{
    DCHECK((len < 63ul) && (len >= 4ul) && (offset < kBlockSize));
    if (PITHY_EXPECT_T(len < 12ul) && PITHY_EXPECT_T(offset < 2048ul)) {
        int lenMinus4 = len - 4l;
        DCHECK(lenMinus4 < 8l);
        *op++ = PITHY_COPY_1_BYTE_OFFSET | (lenMinus4 << 2) | ((offset >> 8) << 5);
        *op++ = offset & 0xff;
    } else {
        if (PITHY_EXPECT_T(offset < 65536ul)) {
            *op++ = PITHY_COPY_2_BYTE_OFFSET | ((len - 1ul) << 2);
            pithy_StoreHost16(op, offset);
            op += 2ul;
        } else {
            *op++ = PITHY_COPY_3_BYTE_OFFSET | ((len - 1ul) << 2);
            pithy_StoreHost24(op, offset);
            op += 3ul;
        }
    }
    return (op);
}

PITHY_STATIC_INLINE char *pithy_EmitCopy(char *op, size_t offset, size_t len)
{
    while (PITHY_EXPECT_F(len >= 63ul)) {
        op = pithy_EmitCopyGreaterThan63(op, offset, (len >= 65539ul) ? 65535ul : len);
        len -= (len >= 65539ul) ? 65535ul : len;
    }
    DCHECK((len > 0ul) ? ((len >= 4ul) && (len < 63ul)) : 1);
    if ( PITHY_EXPECT_T(len >  0ul)) {
        op = pithy_EmitCopyLessThan63(op, offset, len);
        len -= len;
    }
    return (op);
}

size_t pithy_MaxCompressedLength(size_t inputLength)
{
    return ((inputLength >= PITHY_UNCOMPRESSED_MAX_LENGTH) ? 0ul : 32ul + inputLength + (inputLength / 6ul));
}

size_t pithy_Compress(const char *uncompressed,
                      size_t uncompressedLength,
                      char *compressedOut,
                      size_t compressedOutLength,
                      int compressionLevel)
{

    if ((pithy_MaxCompressedLength(uncompressedLength) > compressedOutLength) ||
            (uncompressedLength >= PITHY_UNCOMPRESSED_MAX_LENGTH) ||
            (uncompressedLength == 0ul)) {
        return (0ul);
    }

    char *compressedPtr = compressedOut;

    size_t hashTableSize = 0x100ul, maxHashTableSize = 1 << (12ul + (((compressionLevel < 0) ? 0 :
                           (compressionLevel > 9) ? 9 : compressionLevel) / 2ul));
    while ((hashTableSize < maxHashTableSize) && (hashTableSize < uncompressedLength)) {
        hashTableSize <<= 1;
    }
    pithy_hashOffset_t stackHashTable[hashTableSize], *heapHashTable = NULL, *hashTable = stackHashTable;
    if ((sizeof(pithy_hashOffset_t) * hashTableSize) >= (1024ul * 128ul)) {
        if ((heapHashTable = malloc(sizeof(pithy_hashOffset_t) * hashTableSize)) == NULL) {
            return (0ul);
        }
        hashTable = heapHashTable;
    }
    size_t x = 0ul;
    for (x = 0ul; x < hashTableSize; x++) {
        hashTable[x] = uncompressed;
    }

#ifndef NDEBUG
    char *const compressedOutEnd = compressedOut + compressedOutLength;
#endif
    compressedPtr = pithy_Encode32(compressedPtr, uncompressedLength);
    DCHECK(compressedPtr <= compressedOutEnd);
    {
        const char *uncompressedPtr = uncompressed;
        const char *uncompressedEnd = uncompressed + uncompressedLength;
        const char *nextEmitUncompressedPtr = uncompressedPtr;
        DCHECK((hashTableSize & (hashTableSize - 1l)) == 0);
        const int shift = 32 - pithy_Log2Floor(hashTableSize);
        DCHECK((UINT32_MAX >> shift) == (hashTableSize - 1l));
        size_t skip = 32ul;

        if (PITHY_EXPECT_T(uncompressedLength >= 15ul)) {
            const char *uncompressedEndLimit = uncompressed + uncompressedLength - 15ul;
            uint32_t uncompressedBytes;
            uint32_t nextUncompressedBytes = pithy_Load32(++uncompressedPtr);
            uint32_t nextUncompressedBytesHash = pithy_HashBytes(nextUncompressedBytes, shift);

            while (1) {
                DCHECK(nextEmitUncompressedPtr < uncompressedPtr);
                const char *nextUncompressedPtr = uncompressedPtr, *matchCandidatePtr = NULL;

                skip = (((skip - 32ul) * 184ul) >> 8) + 32ul;

                do {
                    uncompressedPtr   = nextUncompressedPtr;
                    uncompressedBytes = nextUncompressedBytes;
                    uint32_t uncompressedBytesHash = nextUncompressedBytesHash;
                    DCHECK(uncompressedBytesHash == pithy_HashBytes(uncompressedBytes, shift));
                    size_t skipBytesBetweenHashLookups = skip >> 5;
                    skip += ((skip * 7ul) >> 11) + 1ul;
                    nextUncompressedPtr              = uncompressedPtr + skipBytesBetweenHashLookups;
                    if (PITHY_EXPECT_F(nextUncompressedPtr > uncompressedEndLimit)) {
                        goto emit_remainder;
                    }
                    nextUncompressedBytes            = pithy_Load32(nextUncompressedPtr);
                    nextUncompressedBytesHash        = pithy_HashBytes(nextUncompressedBytes, shift);
                    matchCandidatePtr                = hashTable[uncompressedBytesHash];
                    DCHECK((matchCandidatePtr >= uncompressed) && (matchCandidatePtr < uncompressedPtr));
                    hashTable[uncompressedBytesHash] = uncompressedPtr;
                } while ((PITHY_EXPECT_T(uncompressedBytes != pithy_Load32(matchCandidatePtr))) ||
                         PITHY_EXPECT_F((uncompressedPtr - matchCandidatePtr) >= ((int)(kBlockSize - 2ul))));

                DCHECK((nextEmitUncompressedPtr + 16ul) <= uncompressedEnd);
                compressedPtr = pithy_EmitLiteral(compressedPtr,
                                                  nextEmitUncompressedPtr,
                                                  uncompressedPtr - nextEmitUncompressedPtr,
                                                  1);
                DCHECK(compressedPtr <= compressedOutEnd);
                uint64_t uncompressedBytes64 = 0ul;

                do {
                    if (compressionLevel > 2) {
                        DCHECK((uncompressedPtr + 5ul) <= uncompressedEnd);
                        uncompressedBytes64 = pithy_Load64((uint64_t*)uncompressedPtr + 1ul);
                        hashTable[pithy_HashBytes(pithy_GetUint32AtOffset(uncompressedBytes64, 0u), shift)] =
                            uncompressedPtr + 1ul;
                        if (compressionLevel > 4) {
                            hashTable[pithy_HashBytes(pithy_GetUint32AtOffset(uncompressedBytes64, 1u), shift)] =
                                uncompressedPtr + 2ul;
                        }
                    }

                    DCHECK((matchCandidatePtr >= uncompressed) &&
                           (matchCandidatePtr <= uncompressedPtr) &&
                           ((matchCandidatePtr + 4ul) <= uncompressedEnd) &&
                           ((uncompressedPtr + 4ul) <= uncompressedEnd));

                    size_t matchCandidateLength = 4ul + pithy_FindMatchLength(matchCandidatePtr + 4ul,
                                                  uncompressedPtr + 4ul,
                                                  uncompressedEnd);
                    DCHECK(((matchCandidatePtr + matchCandidateLength) >= uncompressed) &&
                           ((matchCandidatePtr + matchCandidateLength) <= uncompressedEnd));
                    DCHECK(0 == memcmp(uncompressedPtr, matchCandidatePtr, matchCandidateLength));
                    compressedPtr = pithy_EmitCopy(compressedPtr,
                                                   uncompressedPtr - matchCandidatePtr,
                                                   matchCandidateLength);
                    DCHECK(compressedPtr <= compressedOutEnd);
                    uncompressedPtr += matchCandidateLength;
                    DCHECK(uncompressedPtr <= uncompressedEnd);
                    nextEmitUncompressedPtr = uncompressedPtr;
                    if (PITHY_EXPECT_F(uncompressedPtr >= uncompressedEndLimit)) {
                        goto emit_remainder;
                    }

                    DCHECK(((uncompressedPtr - 3ul) >= uncompressed) && (uncompressedPtr <= uncompressedEnd));

                    uncompressedBytes64 = pithy_Load64((uint64_t*)uncompressedPtr - 3ul);

                    if (compressionLevel > 0) {
                        if (compressionLevel > 8) {
                            hashTable[pithy_HashBytes(pithy_GetUint32AtOffset(uncompressedBytes64, 0u), shift)] =
                                uncompressedPtr - 3ul;
                        }
                        if (compressionLevel > 6) {
                            hashTable[pithy_HashBytes(pithy_GetUint32AtOffset(uncompressedBytes64, 1u), shift)] =
                                uncompressedPtr - 2ul;
                        }
                        hashTable[pithy_HashBytes(pithy_GetUint32AtOffset(uncompressedBytes64, 2u), shift)] =
                            uncompressedPtr - 1ul;
                    }

                    uncompressedBytes = pithy_GetUint32AtOffset(uncompressedBytes64, 3u);
                    uint32_t uncompressedBytesHash = pithy_HashBytes(uncompressedBytes, shift);
                    matchCandidatePtr = hashTable[uncompressedBytesHash];
                    DCHECK((matchCandidatePtr >= uncompressed) && (matchCandidatePtr < uncompressedPtr));
                    hashTable[uncompressedBytesHash] = uncompressedPtr;
                } while (PITHY_EXPECT_F(uncompressedBytes == pithy_Load32(matchCandidatePtr)) &&
                         PITHY_EXPECT_T((uncompressedPtr - matchCandidatePtr) < ((int)(kBlockSize - 2ul))));

                nextUncompressedBytes = pithy_GetUint32AtOffset(uncompressedBytes64, 4u);
                nextUncompressedBytesHash = pithy_HashBytes(nextUncompressedBytes, shift);
                uncompressedPtr++;
            }
        }

emit_remainder:
        if (nextEmitUncompressedPtr < uncompressedEnd) {
            compressedPtr = pithy_EmitLiteral(compressedPtr,
                                              nextEmitUncompressedPtr,
                                              uncompressedEnd - nextEmitUncompressedPtr,
                                              0);
        }
    }

    pithy_Store32(compressedPtr, 0);
    compressedPtr += 4;

    DCHECK((size_t)(compressedPtr - compressedOut) <= compressedOutLength);
    if (heapHashTable != NULL) {
        free(heapHashTable);
        heapHashTable = NULL;
        hashTable = NULL;
    }
    return (compressedPtr - compressedOut);
}


static const uint32_t pithy_wordmask[] = {
    0u, 0xffu, 0xffffu, 0xffffffu, 0xffffffffu
};


static const uint16_t pithy_charTable[256] = {
    0x0001, 0x0804, 0x1001, 0x1801, 0x0002, 0x0805, 0x1002, 0x1802,
    0x0003, 0x0806, 0x1003, 0x1803, 0x0004, 0x0807, 0x1004, 0x1804,
    0x0005, 0x0808, 0x1005, 0x1805, 0x0006, 0x0809, 0x1006, 0x1806,
    0x0007, 0x080a, 0x1007, 0x1807, 0x0008, 0x080b, 0x1008, 0x1808,
    0x0009, 0x0904, 0x1009, 0x1809, 0x000a, 0x0905, 0x100a, 0x180a,
    0x000b, 0x0906, 0x100b, 0x180b, 0x000c, 0x0907, 0x100c, 0x180c,
    0x000d, 0x0908, 0x100d, 0x180d, 0x000e, 0x0909, 0x100e, 0x180e,
    0x000f, 0x090a, 0x100f, 0x180f, 0x0010, 0x090b, 0x1010, 0x1810,
    0x0011, 0x0a04, 0x1011, 0x1811, 0x0012, 0x0a05, 0x1012, 0x1812,
    0x0013, 0x0a06, 0x1013, 0x1813, 0x0014, 0x0a07, 0x1014, 0x1814,
    0x0015, 0x0a08, 0x1015, 0x1815, 0x0016, 0x0a09, 0x1016, 0x1816,
    0x0017, 0x0a0a, 0x1017, 0x1817, 0x0018, 0x0a0b, 0x1018, 0x1818,
    0x0019, 0x0b04, 0x1019, 0x1819, 0x001a, 0x0b05, 0x101a, 0x181a,
    0x001b, 0x0b06, 0x101b, 0x181b, 0x001c, 0x0b07, 0x101c, 0x181c,
    0x001d, 0x0b08, 0x101d, 0x181d, 0x001e, 0x0b09, 0x101e, 0x181e,
    0x001f, 0x0b0a, 0x101f, 0x181f, 0x0020, 0x0b0b, 0x1020, 0x1820,
    0x0021, 0x0c04, 0x1021, 0x1821, 0x0022, 0x0c05, 0x1022, 0x1822,
    0x0023, 0x0c06, 0x1023, 0x1823, 0x0024, 0x0c07, 0x1024, 0x1824,
    0x0025, 0x0c08, 0x1025, 0x1825, 0x0026, 0x0c09, 0x1026, 0x1826,
    0x0027, 0x0c0a, 0x1027, 0x1827, 0x0028, 0x0c0b, 0x1028, 0x1828,
    0x0029, 0x0d04, 0x1029, 0x1829, 0x002a, 0x0d05, 0x102a, 0x182a,
    0x002b, 0x0d06, 0x102b, 0x182b, 0x002c, 0x0d07, 0x102c, 0x182c,
    0x002d, 0x0d08, 0x102d, 0x182d, 0x002e, 0x0d09, 0x102e, 0x182e,
    0x002f, 0x0d0a, 0x102f, 0x182f, 0x0030, 0x0d0b, 0x1030, 0x1830,
    0x0031, 0x0e04, 0x1031, 0x1831, 0x0032, 0x0e05, 0x1032, 0x1832,
    0x0033, 0x0e06, 0x1033, 0x1833, 0x0034, 0x0e07, 0x1034, 0x1834,
    0x0035, 0x0e08, 0x1035, 0x1835, 0x0036, 0x0e09, 0x1036, 0x1836,
    0x0037, 0x0e0a, 0x1037, 0x1837, 0x0038, 0x0e0b, 0x1038, 0x1838,
    0x0039, 0x0f04, 0x1039, 0x1839, 0x003a, 0x0f05, 0x103a, 0x183a,
    0x003b, 0x0f06, 0x103b, 0x183b, 0x003c, 0x0f07, 0x103c, 0x183c,
    0x0801, 0x0f08, 0x103d, 0x183d, 0x1001, 0x0f09, 0x103e, 0x183e,
    0x1801, 0x0f0a, 0x103f, 0x183f, 0x2001, 0x0f0b, 0x1040, 0x1840
};


int pithy_GetDecompressedLength(const char *compressed, size_t compressedLength, size_t *decompressedOutLengthResult)
{
    const char *compressedEnd = compressed + compressedLength;
    size_t decompressedLength = 0ul;
    if (pithy_Parse32WithLimit(compressed, compressedEnd, &decompressedLength) != NULL) {
        *decompressedOutLengthResult = decompressedLength;
        return (1);
    } else {
        return (0);
    }
}


int pithy_Decompress(const char *compressed, size_t compressedLength, char *decompressedOut,
                     size_t decompressedOutLength)
{
    const char *nextCompressedPtr = NULL, *compressedEnd = (compressed + compressedLength);
    size_t parsedDecompressedLength = 0ul;
    if (((nextCompressedPtr = pithy_Parse32WithLimit(compressed, compressedEnd, &parsedDecompressedLength)) == NULL) ||
            (parsedDecompressedLength > decompressedOutLength)) {
        return (0);
    }

    char *decompressedPtr = decompressedOut, *decompressedEnd = decompressedOut + parsedDecompressedLength;

    while (1) {
        const char *compressedPtr = nextCompressedPtr;
        DCHECK(compressedPtr <= compressedEnd);
        if (PITHY_EXPECT_F(compressedPtr >= compressedEnd)) {
            break;
        }

        const unsigned char c          = *((const unsigned char *)(compressedPtr++));
        const unsigned char cLowerBits = (c & 0x3u);
        const int       spaceLeft  = (decompressedEnd - decompressedPtr);

        if ((cLowerBits == PITHY_LITERAL)) {
            size_t literalLength = (c >> 2) + 1;
            if (PITHY_EXPECT_T(literalLength <= 16ul) && PITHY_EXPECT_T((compressedEnd - compressedPtr) >= 16l) &&
                    PITHY_EXPECT_T(spaceLeft >= 16l)) {
                pithy_Move128(decompressedPtr, compressedPtr);
            } else {
                if (PITHY_EXPECT_F(literalLength > 60)) {
                    if (PITHY_EXPECT_F((compressedPtr + 4) > compressedEnd)) {
                        break;
                    }
                    size_t literalLengthBytes = literalLength - 60;
                    literalLength = (pithy_LoadHost32(compressedPtr) & pithy_wordmask[literalLengthBytes]) + 1;
                    compressedPtr += literalLengthBytes;
                }
                if (PITHY_EXPECT_F(spaceLeft < (int)literalLength) ||
                        PITHY_EXPECT_F((compressedPtr + literalLength) > compressedEnd)) {
                    break;
                }
                memcpy(decompressedPtr, compressedPtr, literalLength);
            }
            nextCompressedPtr  = compressedPtr + literalLength;
            decompressedPtr   += literalLength;
        } else {
            const uint32_t entry      = pithy_charTable[c];
            const size_t   trailer    = pithy_LoadHost32(compressedPtr) & pithy_wordmask[cLowerBits];
            size_t   length     = entry & 0xffu;
            const size_t   copyOffset = ((entry & 0x700u) + trailer);

            compressedPtr += cLowerBits;

            DCHECK((compressedPtr <= compressedEnd) && (copyOffset > 0ul) && (spaceLeft > 0l) && (length > 0ul));

            if (PITHY_EXPECT_F((decompressedPtr - decompressedOut) <= ((int)copyOffset - 1l))) {
                break;
            }
            if (PITHY_EXPECT_T(length <= 16ul) && 
                PITHY_EXPECT_T(copyOffset >= 16ul) && 
                PITHY_EXPECT_T(spaceLeft >= 16l)) {
                pithy_Move128(decompressedPtr, decompressedPtr - copyOffset);
            } else {
                if (PITHY_EXPECT_F(length >= 63ul)) {
                    if (PITHY_EXPECT_T(length == 63ul)) {
                        if (PITHY_EXPECT_F((compressedPtr + 1) > compressedEnd)) {
                            break;
                        }
                        length = (*((unsigned char *)compressedPtr++)) + 63ul;
                    } else {
                        if (PITHY_EXPECT_F((compressedPtr + 2) > compressedEnd)) {
                            break;
                        }
                        length = pithy_LoadHost16(compressedPtr);
                        compressedPtr += 2ul;
                    }
                }

                char    *copyFrom   = decompressedPtr - copyOffset, *copyTo = decompressedPtr;
                int  copyLength = (int)length;

                if (PITHY_EXPECT_F(copyLength > 256l) && PITHY_EXPECT_T(copyOffset > (size_t)copyLength)) {
                    if (PITHY_EXPECT_F(spaceLeft < copyLength)) {
                        break;
                    }
                    memcpy(copyTo, copyFrom, copyLength);
                } else {
                    if (PITHY_EXPECT_T(spaceLeft >= (copyLength + 24)) && PITHY_EXPECT_T(copyLength > 0l)) {
                        while ((copyTo - copyFrom) < 16l) {
                            pithy_Move128(copyTo, copyFrom);
                            copyLength -= copyTo - copyFrom;
                            copyTo += copyTo - copyFrom;
                        }
                        while (copyLength          > 0l)  {
                            pithy_Move128(copyTo, copyFrom);
                            copyFrom   += 16l;
                            copyTo += 16l;
                            copyLength -= 16l;
                        }
                    } else {
                        if (PITHY_EXPECT_F(spaceLeft < copyLength) || PITHY_EXPECT_F(copyLength <= 0l)) {
                            break;
                        }
                        do {
                            *copyTo++ = *copyFrom++;
                        } while (--copyLength > 0l);
                    }
                }
            }
            nextCompressedPtr  = compressedPtr;
            decompressedPtr   += length;
        }
    }

    return (decompressedPtr == decompressedEnd);
}
