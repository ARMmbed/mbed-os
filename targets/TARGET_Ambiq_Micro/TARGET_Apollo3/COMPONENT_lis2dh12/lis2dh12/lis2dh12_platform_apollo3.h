/* 
 * Copyright (c) 2019-2020 SparkFun Electronics
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


#ifndef _LIS2DH12_PLATFORM_APOLLO3_H_
#define _LIS2DH12_PLATFORM_APOLLO3_H_

#include "am_mcu_apollo.h"
#include "lis2dh12_reg.h"

#ifdef __cplusplus
  extern "C" {
#endif

typedef struct _lis2dh12_platform_apollo3_if_t {
    void*       iomHandle;          // IO Master instance
    uint8_t     addCS;              // I2C mode: the 7-bit I2C address (either 0x18 or 0x19 depeding on SA0 pin)  
                                    // SPI mode: the Apollo3 pad to use for chip select
    bool        useSPI;             // Set 'true' if using SPI mode, else 'false'
}lis2dh12_platform_apollo3_if_t;

int32_t lis2dh12_write_platform_apollo3(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len);
int32_t lis2dh12_read_platform_apollo3(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len);

#ifdef __cplusplus
}
#endif
      
#endif // _LIS2DH12_PLATFORM_APOLLO3_H_
