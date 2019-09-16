/*
 * Copyright (c) 2018, Nuvoton Technology Corporation
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

#include "analogout_api.h"

#if DEVICE_ANALOGOUT

#include "cmsis.h"
#include "pinmap.h"
#include "PeripheralPins.h"
#include "nu_modutil.h"

/* Maximum DAC modules */
#define NU_DACMOD_MAXNUM        1
/* Maximum DAC channels per module */
#define NU_DACCHN_MAXNUM        2

static uint32_t dac_modinit_mask[NU_DACMOD_MAXNUM];

static const struct nu_modinit_s dac_modinit_tab[] = {
    {DAC_0_0, DAC_MODULE, 0, 0, DAC_RST, DAC_IRQn, NULL},
    {DAC_0_1, DAC_MODULE, 0, 0, DAC_RST, DAC_IRQn, NULL}
};

void analogout_init(dac_t *obj, PinName pin)
{   
    obj->dac = (DACName) pinmap_peripheral(pin, PinMap_DAC);
    MBED_ASSERT(obj->dac != (DACName) NC);

    const struct nu_modinit_s *modinit = get_modinit(obj->dac, dac_modinit_tab);
    MBED_ASSERT(modinit != NULL);
    MBED_ASSERT(modinit->modname == obj->dac);

    /* Module index */
    uint32_t modidx =  NU_MODINDEX(obj->dac);
    MBED_ASSERT(modidx < NU_DACMOD_MAXNUM);

    /* Module subindex (aka channel) */
    uint32_t chn =  NU_MODSUBINDEX(obj->dac);
    MBED_ASSERT(chn < NU_DACCHN_MAXNUM);

    obj->pin = pin;

    /* Wire pinout */
    pinmap_pinout(pin, PinMap_DAC);

    DAC_T *dac_base = (DAC_T *) NU_MODBASE(obj->dac);

    /* Module-level setup from here */
    if (! dac_modinit_mask[modidx]) {
        /* Select IP clock source and clock divider */
        CLK_SetModuleClock(modinit->clkidx, modinit->clksrc, modinit->clkdiv);

        /* Enable IP clock */
        CLK_EnableModuleClock(modinit->clkidx);

        /* Reset IP */
        SYS_ResetModule(modinit->rsetidx);

        /* Configure conversion settling time
         *
         * DAC_Open() is per-channel, but its implementation involves per-module configuration of
         * conversion settling time. Even so, we still use it for default conversion settling time
         * rather than call per-module DAC_SetDelayTime(). This is to accommodate BSP driver.
         *
         * To configure conversion settling time separately to e.g. 8us, we would call:
         *
         * DAC_SetDelayTime(dac_base, CLK_GetHCLKFreq() * 8 / 1000000);
         */
    }

    /* Channel-level setup from here: */

    /* Set the software trigger, enable DAC event trigger mode and enable D/A converter */
    DAC_Open(dac_base, chn, DAC_WRITE_DAT_TRIGGER);

    /* Mark channel allocated */
    dac_modinit_mask[modidx] |= 1 << chn;
}

void analogout_free(dac_t *obj)
{
    const struct nu_modinit_s *modinit = get_modinit(obj->dac, dac_modinit_tab);
    MBED_ASSERT(modinit != NULL);
    MBED_ASSERT(modinit->modname == obj->dac);

    /* Module index */
    uint32_t modidx =  NU_MODINDEX(obj->dac);
    MBED_ASSERT(modidx < NU_DACMOD_MAXNUM);

    /* Module subindex (aka channel) */
    uint32_t chn =  NU_MODSUBINDEX(obj->dac);
    MBED_ASSERT(chn < NU_DACCHN_MAXNUM);

    DAC_T *dac_base = (DAC_T *) NU_MODBASE(obj->dac);

    /* Channel-level windup from here */

    /* Mark channel free */
    dac_modinit_mask[modidx] &= ~(1 << chn);
    
    /* Close channel */
    DAC_Close(dac_base, chn);

    /* Module-level windup from here: */

    if (! dac_modinit_mask[modidx]) {

        /* Disable IP clock */
        CLK_DisableModuleClock(modinit->clkidx);
    }

    /* Free up pin */
    gpio_set(obj->pin);
    obj->pin = NC;
}

void analogout_write(dac_t *obj, float value)
{
    if (value <= 0.0f) {
        analogout_write_u16(obj, 0);
    } else if (value >= 1.0f) {
        analogout_write_u16(obj, 0xFFFF);
    } else {
        analogout_write_u16(obj, (uint16_t) (value * ((float) 0xFFFF)));
    }
}

void analogout_write_u16(dac_t *obj, uint16_t value)
{   
    DAC_T *dac_base = (DAC_T *) NU_MODBASE(obj->dac);
    uint32_t chn =  NU_MODSUBINDEX(obj->dac);

    /* Convert 16 bits to effective 12 bits by dropping 4 LSB bits. */
    DAC_WRITE_DATA(dac_base, chn, value >> 4);

    /* Wait for completed */
    while (DAC_IS_BUSY(dac_base, chn));
}

float analogout_read(dac_t *obj)
{
    uint32_t value = analogout_read_u16(obj);
    return (float) value * (1.0f / (float) 0xFFFF);
}

uint16_t analogout_read_u16(dac_t *obj)
{
    DAC_T *dac_base = (DAC_T *) NU_MODBASE(obj->dac);
    uint32_t chn =  NU_MODSUBINDEX(obj->dac);

    uint16_t dat12 = chn ? dac_base->DATA1 : dac_base->DATA0;
    dat12 = (dat12 & DAC_DATA_DACData_Msk) >> DAC_DATA_DACData_Pos;
    /* Just 12 bits are effective. Convert to 16 bits.
     *
     * dat12    : 0000 b11b10b9b8 b7b6b5b4 b3b2b1b0
     * dat16    : b11b10b9b8 b7b6b5b4 b3b2b1b0 b11b10b9b8
     */
    uint16_t dat16 = (dat12 << 4) | (dat12 >> 8);

    return dat16;
}

const PinMap *analogout_pinmap()
{
    return PinMap_DAC;
}

#endif
