/***************************************************************************//**
 * @file analogout_aoi.c
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Silicon Labs has no
 * obligation to support this Software. Silicon Labs is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Silicon Labs will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 ******************************************************************************/

#include "device.h"
#if DEVICE_ANALOGOUT

#include "mbed_assert.h"
#include "error.h"
#include "analogout_api.h"
#include "pinmap.h"
#include "pinmap_function.h"
#include "PeripheralPins.h"
#include "clocking.h"

#include "em_dac.h"
#include "em_cmu.h"

static uint8_t dac_initialized = 0;

void analogout_init(dac_t *obj, PinName pin)
{
    /* init in-memory structure */
    obj->dac = (DAC_TypeDef *) pinmap_peripheral(pin, PinMap_DAC);
    MBED_ASSERT((int) obj->dac != NC);

    obj->channel = pin_location(pin, PinMap_DAC);
    MBED_ASSERT((int) obj->channel != NC);
    
    pin_mode(pin, Disabled);

    if (!dac_initialized) {
        /* Initialize the DAC. Will disable both DAC channels, so should only be done once */
        /* Use default settings */
        CMU_ClockEnable(cmuClock_DAC0, true);

        DAC_Init_TypeDef init = DAC_INIT_DEFAULT;

        /* Calculate the DAC clock prescaler value that will result in a DAC clock
         * close to 500kHz. Second parameter is zero. This uses the current HFPERCLK
         * frequency instead of setting a new one. */
        init.prescale = DAC_PrescaleCalc(500000, REFERENCE_FREQUENCY);

        /* Set reference voltage to VDD */
        init.reference = dacRefVDD;

        DAC_Init(obj->dac, &init);
        dac_initialized = 1;
    }
    /* Use default channel settings */
    DAC_InitChannel_TypeDef initChannel = DAC_INITCHANNEL_DEFAULT;
    initChannel.enable = true;
    DAC_InitChannel(obj->dac, &initChannel, obj->channel);
}

void analogout_free(dac_t *obj)
{
    //Reset channel by re-initializing
    DAC_InitChannel_TypeDef initChannel = DAC_INITCHANNEL_DEFAULT;
    initChannel.enable = false;
    DAC_InitChannel(obj->dac, &initChannel, obj->channel);
    
    //Check all channels to see if we can disable the DAC completely
    if((DAC0->CH0CTRL & DAC_CH0CTRL_EN) == 0 && (DAC0->CH1CTRL & DAC_CH1CTRL_EN) == 0) {
        CMU_ClockEnable(cmuClock_DAC0, false);
        dac_initialized = 0;
    }
}

static inline void dac_write(dac_t *obj, int value)
{
    switch (obj->channel) {
        case 0:
            obj->dac->CH0DATA = value;
            break;
        case 1:
            obj->dac->CH1DATA = value;
            break;
    }
}

static inline int dac_read(dac_t *obj)
{
    switch (obj->channel) {
        case 0:
            return obj->dac->CH0DATA;
            break;
        case 1:
            return obj->dac->CH1DATA;
            break;
        default:
            error("AnalogOut pin error. Invalid channel");
            return -1;
            break;
    }
}

void analogout_write(dac_t *obj, float value)
{
    /* We multiply the float value with 0xFFF because the DAC has 12-bit resolution.
     * Ie. accepts values between 0 and 0xFFF (4096). */
    dac_write(obj, value*0xFFF);
}

void analogout_write_u16(dac_t *obj, uint16_t value)
{
    /* The DAC has 12 bit resolution, so we remove the 4 least significant bits */
    dac_write(obj, value >> 4);
}

float analogout_read(dac_t *obj)
{
    /* dac_read returns a number between 0 and 0xFFF. Division gives us a float between 0 and 1 */
    return dac_read(obj)/(float)0xFFF;
}

uint16_t analogout_read_u16(dac_t *obj)
{
    /* dac_read returns a number with 12 significant digits,
     * so we shift in 0s from right to make it a 16 bit number */
    return dac_read(obj) << 4;
}

#endif
