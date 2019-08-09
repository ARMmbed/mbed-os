// Copyright (c) Qualcomm Atheros, Inc.
// All rights reserved.
// Redistribution and use in source and binary forms, with or without modification, are permitted (subject to
// the limitations in the disclaimer below) provided that the following conditions are met:
//
// · Redistributions of source code must retain the above copyright notice, this list of conditions and the
//   following disclaimer.
// · Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
//   following disclaimer in the documentation and/or other materials provided with the distribution.
// · Neither the name of nor the names of its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
//
// NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE. THIS SOFTWARE IS
// PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
// BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//==============================================================================
// Author(s): ="Atheros"
//==============================================================================

#ifndef __WLAN_DEFS_H__
#define __WLAN_DEFS_H__

/*
 * This file contains WLAN definitions that may be used across both
 * Host and Target software.
 */

typedef enum
{
    MODE_11A = 0,     /* 11a Mode */
    MODE_11G = 1,     /* 11b/g Mode */
    MODE_11B = 2,     /* 11b Mode */
    MODE_11GONLY = 3, /* 11g only Mode */
#ifdef SUPPORT_11N
    MODE_11NA_HT20 = 4, /* 11a HT20 mode */
    MODE_11NG_HT20 = 5, /* 11g HT20 mode */
    MODE_11NA_HT40 = 6, /* 11a HT40 mode */
    MODE_11NG_HT40 = 7, /* 11g HT40 mode */
    MODE_UNKNOWN = 8,
    MODE_MAX = 8
#else
    MODE_UNKNOWN = 4,
    MODE_MAX = 4
#endif
} WLAN_PHY_MODE;

typedef enum
{
    WLAN_11A_CAPABILITY = 1,
    WLAN_11G_CAPABILITY = 2,
    WLAN_11AG_CAPABILITY = 3
} WLAN_CAPABILITY;

#ifdef SUPPORT_11N
typedef unsigned long A_RATEMASK;
#else
typedef unsigned short A_RATEMASK;
#endif

#ifdef SUPPORT_11N
#define IS_MODE_11A(mode) (((mode) == MODE_11A) || ((mode) == MODE_11NA_HT20) || ((mode) == MODE_11NA_HT40))
#define IS_MODE_11B(mode) ((mode) == MODE_11B)
#define IS_MODE_11G(mode) \
    (((mode) == MODE_11G) || ((mode) == MODE_11GONLY) || ((mode) == MODE_11NG_HT20) || ((mode) == MODE_11NG_HT40))
#define IS_MODE_11GONLY(mode) ((mode) == MODE_11GONLY)
#else
#define IS_MODE_11A(mode) ((mode) == MODE_11A)
#define IS_MODE_11B(mode) ((mode) == MODE_11B)
#define IS_MODE_11G(mode) (((mode) == MODE_11G) || ((mode) == MODE_11GONLY))
#define IS_MODE_11GONLY(mode) ((mode) == MODE_11GONLY)
#endif /* SUPPORT_11N */

#endif /* __WLANDEFS_H__ */
