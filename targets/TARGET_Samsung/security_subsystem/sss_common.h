/* mbed Microcontroller Library
 * Copyright (c) 2006-2020 ARM Limited
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
/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
#ifndef SSS_COMMON_H
#define SSS_COMMON_H

/*************** Include Files ********************************************/
#include "sss_oid.h"

/*************** Assertions ***********************************************/

/*************** Definitions / Macros *************************************/
#define SUCCESS                         (0x00)
#define FAIL                            (0x01)

#define SSSR_SUCCESS                    (0x00)
#define SSSR_FAIL                       (0x01)

#define RSP_FAIL                        (0xF1)
#define RSP_SUCCESS                     (0xA1)

#ifndef NULL
#define NULL   ((void *) 0)
#endif

#define NULLPTR                 (void *)0


/////////////////////////////////////////////////////////
// MACRO ////////////////////////////////////////////////
/////////////////////////////////////////////////////////
#define BIT(nbit)                   (0x1u << (nbit))
#define SFR_BIT_SET(val, bit)       ((val) |= (bit))            ///<bit set by bit value
#define SFR_W0i1C(sfr, bit)         ((sfr) = (bit))             // write 0 ignore & write 1 clear
#define SFR_SET(sfr, val)           ((sfr) = (val))
#define SFR_GET(sfr, val)           ((val) = (sfr))
#define WAIT_SFR_BIT_CLR(sfr, bit)  while((sfr) & (bit))    ///<wait until bit is cleared
#define WAIT_SFR_BIT_SET(sfr, bit)  while(!((sfr) & (bit))) ///<wait until bit is set

#define SSS_SFR_SET(sfr, val)               ((sfr) = (val))
#define SSS_SFR_GET(sfr, val)               ((val) = (sfr))
#define SSS_SFR_BIT_SET(sfr, bit)           ((sfr) = (sfr)|(bit))
#define SSS_WAIT_BIT_CLR(sfr, bit)          while ((sfr) & (bit))
#define SSS_WAIT_SFR_BIT_SET(sfr, bit)      while (!((sfr) & (bit)))

#define SSS_CTRL_FIELD_ADDR(val)            (SSS_CTRL_FIELD_BASE+((val)<<2))
#define SSS_DATA_FIELD_ADDR(val)            (SSS_DATA_FIELD_BASE+((val)<<2))
#define SSS_CTRL_FIELD(val)                 (*(volatile unsigned int *)(SSS_CTRL_FIELD_ADDR(val)))
#define SSS_DATA_FIELD(val)                 (*(volatile unsigned int *)(SSS_DATA_FIELD_ADDR(val)))

#define SSS_DATA_FIELD_SET(index, value)    (SSS_DATA_FIELD(index) = (value))
#define SSS_DATA_FIELD_GET(index, value)    ((value) = SSS_DATA_FIELD(index))
#define SSS_CTRL_FIELD_SET(index, value)    (SSS_CTRL_FIELD(index) = (value))
#define SSS_CTRL_FIELD_GET(index, value)    ((value) = SSS_CTRL_FIELD(index))

#define SSS_SET_MB_COMMAND(value)           (SSS_CTRL_FIELD(0U) = (value))
#define SSS_SET_MB_OID(value)               (SSS_CTRL_FIELD(1U) = (value))

#define SSS_GET_MB_RESULT(value)            ((value) = SSS_CTRL_FIELD(0U))
#define SSS_GET_ERROR_CODE(value)           ((value) = SSS_CTRL_FIELD(1U))
#define SSS_GET_RSP_LEN(value)              ((value) = SSS_CTRL_FIELD(2U))


#define SET_DWORD_TO_BBUF(buf, dword)           \
    ((u08 *)(buf))[3] = ((u08)((dword) >> 0));  \
    ((u08 *)(buf))[2] = ((u08)((dword) >> 8));  \
    ((u08 *)(buf))[1] = ((u08)((dword) >> 16)); \
    ((u08 *)(buf))[0] = ((u08)((dword) >> 24));

#define GET_DWORD_FROM_BBUF(buf)            \
    (u32)(                                  \
        ((((u08 *)(buf))[3]) << 0)  |       \
        ((((u08 *)(buf))[2]) << 8)  |       \
        ((((u08 *)(buf))[1]) << 16) |       \
        ((((u08 *)(buf))[0]) << 24))

#define SWAP32(val)                         \
    (u32)(                                  \
        (((val) & 0xff) << 24) |                \
        (((val) & 0xff00) << 8) |               \
        (((val) & 0xff0000) >> 8) |         \
        (((val) & 0xff000000) >> 24)            \
    )


#define CEIL_16BYTE(val)        (val&0xF) ? ((val&0xFFFFFFF0)+0x10) : (val)
#define CEIL_BY_WORD(val)       (((val)+3)>>2)
#define CEIL_BY_16BYTE(val)     (((val)+15)>>4)
//#define   CEIL_BY_BYTE(bitval)    (((bitval)+7)>>3)


/*************** New Data Types (Basic Data Types)  ***********************/

/*************** New Data Types *******************************************/
//! 8bits unsigned data type
typedef unsigned char       u08;
//! 16bits unsigned data type
typedef unsigned short      u16;
//! 32bits unsigned data type
typedef unsigned int        u32;
//! 64bits unsigned data type
typedef unsigned long long  u64;
//! CPU size bits unsigned data type
typedef unsigned int        uwd;
//! 8bits signed data type
typedef char                s08;
//! 16bits signed data type
typedef short               s16;
//! 32bits signed data type
typedef int                 s32;
//! 64bits signed data type
typedef long long           s64;
//! CPU size bits signed data type
typedef int                 swd;

//! return error code
typedef u32                 SSS_RV;

/**
 * @brief   struct of OCTET String / length & data
 */
typedef struct _OS_st {
    //! byte length of Data
    u32     u32DataByteLen;
    //! byte array of Data
    u08     *pu08Data;
} stOCTET_STRING;

/**
 * @brief   struct of BIGNUM String / length & data
 */
typedef stOCTET_STRING stBIG_NUM;





/*************** Constants ************************************************/

/*************** Variable declarations ************************************/

/*************** Functions  ***********************************************/


#endif  /* SSS_COMMON_H */

