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
#include "trng_api.h"
#include "analogin_api.h"
#include "analogin_ext.h"

#ifdef DEVICE_TRNG
static analogin_t tradcng;
static uint8_t trng_inited;

void trng_init(trng_t *obj)
{
    if (trng_inited != 0) {
        return;
    }
    analogin_init(&tradcng, ADC0);
    trng_inited = 1;
}

void trng_free(trng_t *obj)
{
    trng_inited = 0;
    analogin_deinit(&tradcng);
}

static void trng_get_byte(unsigned char *byte)
{
    size_t bit;
    uint32_t adc[2];
	
    PSAL_ADC_MNGT_ADPT      pSalADCMngtAdpt     = NULL;
    PSAL_ADC_HND            pSalADCHND          = NULL;

    pSalADCMngtAdpt         = &(tradcng.SalADCMngtAdpt);
    pSalADCHND              = &(pSalADCMngtAdpt->pSalHndPriv->SalADCHndPriv);

    *byte = 0;
    for (bit=0; bit<8; bit++) {
		RtkADCRxManualRotate(pSalADCHND, adc);
        *byte |= (adc[0] & 0x01) << bit;
    }
}

int trng_get_bytes(trng_t *obj, uint8_t *buf, size_t len, size_t *output_len)
{
    size_t i;

    for (i = 0; i < len; i++) {
        trng_get_byte(buf + i);
    }

    *output_len = len;
    return 0;
}
#endif
