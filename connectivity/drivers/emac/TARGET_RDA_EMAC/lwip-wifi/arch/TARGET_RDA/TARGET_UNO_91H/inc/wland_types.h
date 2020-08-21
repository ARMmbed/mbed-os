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

/*****************************************************************************/
/*                                                                           */
/*  File Name         : wland_types.h                                             */
/*                                                                           */
/*  Description       : This file contains all the data type definitions for */
/*                      MAC implementation.                                  */
/*                                                                           */
/*****************************************************************************/

#ifndef WLAND_TYPES_H
#define WLAND_TYPES_H

/*****************************************************************************/
/* Constants Definitions                                                     */
/*****************************************************************************/

typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned int        u32;
typedef unsigned long long  u64;
typedef signed char         s8;
typedef signed short        s16;
typedef signed int          s32;
typedef signed long long    s64;

typedef unsigned int                size_t;

/** Indicates Bit Value of BITx */
#ifndef BIT
#define BIT(x)                          (1ul << (x))

/*****************************************************************************/
/* Constant Definitions                                                      */
/*****************************************************************************/

#define BIT0                            (1ul << 0)
#define BIT1                            (1ul << 1)
#define BIT2                            (1ul << 2)
#define BIT3                            (1ul << 3)
#define BIT4                            (1ul << 4)
#define BIT5                            (1ul << 5)
#define BIT6                            (1ul << 6)
#define BIT7                            (1ul << 7)
#define BIT8                            (1ul << 8)
#define BIT9                            (1ul << 9)
#define BIT10                           (1ul << 10)
#define BIT11                           (1ul << 11)
#define BIT12                           (1ul << 12)
#define BIT13                           (1ul << 13)
#define BIT14                           (1ul << 14)
#define BIT15                           (1ul << 15)
#define BIT16                           (1ul << 16)
#define BIT17                           (1ul << 17)
#define BIT18                           (1ul << 18)
#define BIT19                           (1ul << 19)
#define BIT20                           (1ul << 20)
#define BIT21                           (1ul << 21)
#define BIT22                           (1ul << 22)
#define BIT23                           (1ul << 23)
#define BIT24                           (1ul << 24)
#define BIT25                           (1ul << 25)
#define BIT26                           (1ul << 26)
#define BIT27                           (1ul << 27)
#define BIT28                           (1ul << 28)
#define BIT29                           (1ul << 29)
#define BIT30                           (1ul << 30)
#define BIT31                           (1ul << 31)
#endif

#endif /* WLAND_TYPES_H */
