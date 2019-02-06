/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include <inttypes.h>
#include <stdio.h>
#include "tfm_utils.h"

void tfm_panic(void)
{
    while (1)
        ;
}

int32_t tfm_bitcount(uint32_t n)
{
    int32_t count = 0;
    uint8_t tmp;

    while (n) {
        tmp = n & 0xFF;
        while (tmp) {
            count += tmp & 0x1;
            tmp >>= 1;
        }
        n >>= 8;
    }

    return count;
}
