/* Copyright (c) 2019 Unisoc Communications Inc.
 * SPDX-License-Identifier: Apache-2.0
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

#ifndef _WLAND_DBG_H_
#define _WLAND_DBG_H_
#include <stdio.h>
#include "rda5991h_wland.h"
#include "wland_types.h"

extern int wland_dbg_dump;
extern int wland_dbg_level;

#define RDA_WLAND_DBG

#ifdef RDA_WLAND_DBG
#define WLAND_DBG(level, fmt, ...)  do {\
        int dbg_level = D_##level##_LEVEL;\
        if((dbg_level <= wland_dbg_level)){\
            printf("%s,"fmt, __func__, ##__VA_ARGS__);\
        }                                                                           \
    } while (0)

//if frmae_len is zero, get len from frame header
static inline void wland_dump_frame(const char* msg, u8 *data, u32 frame_len)
{

    u32 len,i;

    if(wland_dbg_dump == 1) {
        if(frame_len != 0) {
            len = frame_len;
        } else {
            len = data[0] | ((data[1]&0x0f) << 8);
        }
        printf("%s : ",msg);
        for(i=0; i<len; i++)
            printf("%02x ", *(data+i));
        printf("\r\nframe_len=%d\r\n", len);
    }
    return;
}
#else
#define WLAND_DBG(level, fmt, ...)
static inline void wland_dump_frame(const char* msg, u8 *data, u32 frame_len)
{}
#endif
#endif /*_WLAND_DBG_H_*/
