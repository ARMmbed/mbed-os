/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2017, STMicroelectronics
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 */
#include "device.h"

#if DEVICE_DEVICEUID

#include <string.h>
#if defined(TARGET_STM32F0)
#include "stm32f0xx_ll_utils.h"
#elif defined(TARGET_STM32F1)
#include "stm32f1xx_ll_utils.h"
#elif defined(TARGET_STM32F2)
#include "stm32f2xx_ll_utils.h"
#elif defined(TARGET_STM32F3)
#include "stm32f3xx_ll_utils.h"
#elif defined(TARGET_STM32F4)
#include "stm32f4xx_ll_utils.h"
#elif defined(TARGET_STM32F7)
#include "stm32f7xx_ll_utils.h"
#elif defined(TARGET_STM32L0)
#include "stm32l0xx_ll_utils.h"
#elif defined(TARGET_STM32L1)
#include "stm32l1xx_ll_utils.h"
#elif defined(TARGET_STM32L4)
#include "stm32l4xx_ll_utils.h"
#else
#error "Unsupported STM32 device!"
#endif
#include "device_uid_api.h"

#define UID_WORDS       3

#ifdef MBED_DEVICEUID_STR_SIZE_MAX
static int cpu_uid_itoa(int value, char *sp, int radix);
#endif

void device_uid_get_uid(uint8_t *uid)
{
    uint32_t uid_buf[UID_WORDS];
    int pos = 0;

    uid_buf[0] = LL_GetUID_Word0();
    uid_buf[1] = LL_GetUID_Word1();
    uid_buf[2] = LL_GetUID_Word2();
    
    for (int i = (UID_WORDS-1); i >= 0; --i) {
        for (int j = 3; j >= 0; --j) {
            uid[pos++] = (uint8_t)((uid_buf[i] >> (j*8)) & 0xFF);
        }
    }
}

#ifdef MBED_DEVICEUID_STR_SIZE_MAX
void device_uid_get_str(char *str)
{
    char buffer[16];
    char lot_number[8];
    uint8_t wafer_number;
    uint16_t x, y;
    uint32_t uid_buf[UID_WORDS];

    uid_buf[0] = LL_GetUID_Word0();
    uid_buf[1] = LL_GetUID_Word1();
    uid_buf[2] = LL_GetUID_Word2();

    lot_number[0] = (uid_buf[2] >> 24) & 0x000000FF;
    lot_number[1] = (uid_buf[2] >> 16) & 0x000000FF;
    lot_number[2] = (uid_buf[2] >> 8) & 0x000000FF;
    lot_number[3] = uid_buf[2] & 0x000000FF;
    lot_number[4] = (uid_buf[1] >> 24) & 0x000000FF;
    lot_number[5] = (uid_buf[1] >> 16) & 0x000000FF;
    lot_number[6] = (uid_buf[1] >> 8) & 0x000000FF;
    lot_number[7] = '\0';
    
    wafer_number = uid_buf[1] & 0x000000FF;
    
    x = (uid_buf[0] >> 16) & 0x0000FFFF;
    y = uid_buf[0] & 0x0000FFFF;
    
    // make a string in format "LLLLLL:WWW:XXXXX:YYYYY" - max 23 chars including terminating zero
    strcpy(str, lot_number);
    strcat(str, ":");
    cpu_uid_itoa(wafer_number, buffer, 10);
    strcat(str, buffer);
    strcat(str, ":");
    cpu_uid_itoa(x, buffer, 10);
    strcat(str, buffer);
    strcat(str, ":");
    cpu_uid_itoa(y, buffer, 10);
    strcat(str, buffer);
}

int cpu_uid_itoa(int value, char *sp, int radix)
{
    char tmp[16];// be careful with the length of the buffer
    char *tp = tmp;
    int i;
    unsigned v = value;

    while (v || tp == tmp) {
        i = v % radix;
        v /= radix; // v/=radix uses less CPU clocks than v=v/radix does
        if (i < 10) {
          *tp++ = i+'0';
        }
        else {
          *tp++ = i + 'a' - 10;
        }
    }

    int len = tp - tmp;

    while (tp > tmp) {
        *sp++ = *--tp;
    }
    *sp = '\0';

    return len;
}

#endif

#endif
