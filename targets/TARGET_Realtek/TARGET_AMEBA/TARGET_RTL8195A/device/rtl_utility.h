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


