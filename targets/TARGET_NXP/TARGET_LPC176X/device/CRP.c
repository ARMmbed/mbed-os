/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *	 http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
 #include "mbed_toolchain.h"

 /* Code Read Protection

NONE	0xFFFFFFFF - No code protection.

CRP1    0x12345678 - Write to RAM command can not access RAM below 0x10000200.
                   - Read Memory command: disabled.
                   - Copy RAM to Flash command: cannot write to Sector 0.
                   - "Go" command: disabled.
                   - Erase sector(s) command: can erase any individual sector except 
                   	 sector 0 only, or can erase all sectors at once.
                   - Compare command: disabled

CRP2    0x87654321 - Write to RAM command: disabled.
                   - Copy RAM to Flash: disabled.
                   - Erase command: only allows erase of all sectors.

CRP3    0x43218765 - Access to chip via the SWD pins is disabled. ISP entry
                     by pulling PIO0_1 LOW is disabled if a valid user code is
                     present in flash sector 0.
Caution: If CRP3 is selected, no future factory testing can be
performed on the device.
*/
#if !defined(APPLICATION_ADDR) // Relocate CRP if there is a bootloader.
    #define APPLICATION_ADDR 0
#endif

#define CRP_NONE	0xFFFFFFFF
#define CRP_1		0x12345678 
#define CRP_2		0x87654321
#define CRP_3		0x43218765

#ifndef CRP
#define CRP CRP_NONE
#endif

MBED_SECTION(".CRPSection") MBED_USED const long CRP_Key = CRP;
