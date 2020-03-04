//
//  pithy.h
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

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _PITHY_H_
#define _PITHY_H_

// compressionLevel >= 0 && compressionLevel <= 9.  Values out side this range will be clamped to this range.
size_t pithy_Compress  (const char *uncompressed, size_t uncompressedLength, char *compressedOut,
                        size_t compressedOutLength, int compressionLevel);
int    pithy_Decompress(const char *compressed,   size_t compressedLength,   char *decompressedOut,
                        size_t decompressedOutLength);

size_t pithy_MaxCompressedLength(size_t inputLength);
int    pithy_GetDecompressedLength(const char *compressed, size_t compressedLength,
                                   size_t *decompressedOutLengthResult);

#endif // _PITHY_H_

#ifdef __cplusplus
} // extern "C"
#endif
