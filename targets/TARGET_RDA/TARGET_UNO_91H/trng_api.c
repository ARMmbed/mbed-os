/*
 *  Hardware entropy collector for the UNO_91H, using RDA's TRNG
 *
 *  Copyright (C) 2006-2016, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

#if defined(DEVICE_TRNG)

#include "cmsis.h"
#include "trng_api.h"
#include "us_ticker_api.h"

#define rRNG_CLKGATE        (RDA_SCU->CLKGATE0)
#define rRNG_CLKGEN         (RDA_DMACFG->dma_func_ctrl)
#define rRNG_INTSTATE       (RDA_DMACFG->dma_int_out)
#define rTRNG_CTRL          (RDA_RNG->TCTRL)
#define rTRNG_DATA0         (RDA_RNG->TD0)
#define rTRNG_DATA1         (RDA_RNG->TD1)
#define rPRNG_DATA          (RDA_RNG->PD)

/** trng_get_byte
 *  @brief Get one byte of entropy from the RNG, assuming it is up and running.
 *  @param obj TRNG obj
 *  @param pointer to the hardware generated random byte.
 */
static void trng_get_byte(trng_t *obj, unsigned char *byte )
{
    if (0U == obj->byte_idx) {
        uint32_t start_time = us_ticker_read();
        /* Delay for PRNG data ready, generate 4-byte per 32us */
        while ((us_ticker_read() - start_time) <= 32);
        obj->trng_data0 = rPRNG_DATA;
    }
    *byte = (unsigned char)((obj->trng_data0 >> (obj->byte_idx << 3)) & 0x00FFUL);
    obj->byte_idx++;
    obj->byte_idx &= 0x03U;
}

void trng_init(trng_t *obj)
{
    uint32_t regval;
    obj->dma_clk_flag = (uint8_t)((rRNG_CLKGATE >> 18) & 0x01U);
    obj->byte_idx = 0;
    if (0x00U == obj->dma_clk_flag) {
        int j;
        /* DMA Peripheral clock enable */
        rRNG_CLKGATE |= (0x01UL << 18);
        /* dealy after clk en */
        for (j = 0; j < 4; j++);
    }
    /* RNG Engine clock enable */
    rRNG_CLKGEN |= (0x01UL << 30);
    /* Set TRNG enable bit, once mode */
    regval = rTRNG_CTRL |  ((0x01UL << 0) | (0x01UL << 4) | (0xFFUL << 16));
    rTRNG_CTRL = regval & ~((0x01UL << 1) | (0x01UL << 2) | (0x01UL << 3));
}

void trng_free(trng_t *obj)
{
    /* Clear TRNG enable bit */
    rTRNG_CTRL = 0x00UL;
    /* RNG Engine clock disable */
    rRNG_CLKGEN &= ~(0x01UL << 30);
    if (0x00U == obj->dma_clk_flag) {
        /* DMA Peripheral clock disable */
        rRNG_CLKGATE &= ~(0x01UL << 18);
    }
}

int trng_get_bytes(trng_t *obj, uint8_t *output, size_t length, size_t *output_length)
{
    int ret = 0;

    /* Get Random byte */
    for (uint32_t i = 0; i < length; i++) {
        trng_get_byte(obj, output + i);
    }

    *output_length = length;

    return ret;
}

#endif
