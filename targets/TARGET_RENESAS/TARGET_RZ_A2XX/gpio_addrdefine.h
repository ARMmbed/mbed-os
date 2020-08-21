/* Copyright (c) 2018-2020 Renesas Electronics Corporation.
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
#ifndef __GPIO_ADDRDEFINE__
#define __GPIO_ADDRDEFINE__

#define PORTm_base (0xFCFFE000uL)

#define PDR(g)     (volatile unsigned short *)(PORTm_base + 0x0000 + ((g)*2))
#define PODR(g)    (volatile unsigned  char *)(PORTm_base + 0x0040 + ((g)*1))
#define PIDR(g)    (volatile unsigned  char *)(PORTm_base + 0x0060 + ((g)*1))
#define PMR(g)     (volatile unsigned  char *)(PORTm_base + 0x0080 + ((g)*1))
#define DSCR(g)    (volatile unsigned short *)(PORTm_base + 0x0140 + ((g)*2))
#define PFS(g,n)   (volatile unsigned  char *)(PORTm_base + 0x0200 + ((g)*8) + n)

#endif/*__GPIO_ADDRDEFINE__*/

