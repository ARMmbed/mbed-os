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

#ifdef CONFIG_MBED_ENABLED
#include "platform_stdlib.h"
#endif

#ifdef DEVICE_TRNG


void trng_init(trng_t *obj)
{
    _memset((void *)obj, 0, sizeof(trng_t));
    analogin_init(&obj->tradcng, ADC0);
    obj->inited = 1;
}

void trng_free(trng_t *obj)
{
    if (!obj->inited) {
        return;
    }
    
    obj->inited = 0;
    analogin_deinit(&obj->tradcng);
}

static void trng_get_byte(trng_t *obj, unsigned char *byte)
{
    size_t bit;
    uint32_t adc[2];
    
    PSAL_ADC_MNGT_ADPT      pSalADCMngtAdpt     = NULL;
    PSAL_ADC_HND            pSalADCHND          = NULL;

    pSalADCMngtAdpt         = &(obj->tradcng.SalADCMngtAdpt);
    pSalADCHND              = &(pSalADCMngtAdpt->pSalHndPriv->SalADCHndPriv);

    *byte = 0;
    for (bit = 0; bit < 8; bit++) {
        RtkADCRxManualRotate(pSalADCHND, adc);
        *byte |= (adc[0] & 0x01) << bit;
    }
}

int trng_get_bytes(trng_t *obj, uint8_t *buf, size_t len, size_t *output_len)
{
    size_t i;
    
    if (!obj->inited) {
        return -1;
    }

    for (i = 0; i < len; i++) {
        trng_get_byte(obj, buf + i);
    }

    *output_len = len;
    return 0;
}
#endif
