<<<<<<< HEAD
/******************************************************************************
 *
 * Copyright(c) 2007 - 2011 Realtek Corporation. All rights reserved.
 *                                        
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 *
 ******************************************************************************/
=======
/* mbed Microcontroller Library
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
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


>>>>>>> upstream/master
#ifndef __RTL_UTILITY_H_
#define __RTL_UTILITY_H_

VOID RtlMemcpy(VOID* dec, VOID* sour, u32 sz);
u32 RtlMemcmp(VOID *dst, VOID *src, u32 sz);
VOID RtlMemset(VOID *pbuf, u32 c, u32 sz);

s8 *
RtlStrncpy(
    IN  s8 *dest, 
    IN  const s8 *src, 
    IN  SIZE_T count
);

s8 *
RtlStrcpy(
    IN  s8 *dest, 
    IN  const s8 *src
);


SIZE_T
RtlStrlen(
    IN  const s8 *s
);


SIZE_T
RtlStrnlen(
    IN  const s8 *s, 
    IN  SIZE_T count
);


int 
RtlStrcmp(
    IN  const s8 *cs, 
    IN  const s8 *ct

);

int
RtlStrncmp(
    IN  const s8 *cs, 
    IN  const s8 *ct, 
    IN  SIZE_T count
);

#endif


