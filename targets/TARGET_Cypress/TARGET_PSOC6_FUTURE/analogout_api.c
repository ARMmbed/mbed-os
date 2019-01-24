/*
 * mbed Microcontroller Library
 * Copyright (c) 2017-2018 Future Electronics
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

#include "device.h"
#include "analogout_api.h"
#include "cy_ctdac.h"
#include "psoc6_utils.h"
#include "mbed_assert.h"
#include "mbed_error.h"
#include "pinmap.h"
#include "PeripheralPins.h"
#include "platform/mbed_error.h"

#if DEVICE_ANALOGOUT

#define CTDAC_NUM_BITS          12
const uint16_t CTDAC_MAX_VALUE = (uint16_t)((1UL << CTDAC_NUM_BITS) - 1);

const uint32_t CTDAC_BASE_CLOCK_HZ = 500000;    // 500 kHz or less

#define CTDAC_DEGLITCH_CYCLES   35



/** Global CTDAC configuration data.
 */
static cy_stc_ctdac_config_t ctdac_config = {
    .refSource          = CY_CTDAC_REFSOURCE_VDDA,          /**< Reference source: Vdda or externally through Opamp1 of CTB */
    .formatMode         = CY_CTDAC_FORMAT_UNSIGNED,         /**< Format of DAC value: signed or unsigned */
    .updateMode         = CY_CTDAC_UPDATE_BUFFERED_WRITE,   /**< Update mode: direct or buffered writes or hardware, edge or level */
    .deglitchMode       = CY_CTDAC_DEGLITCHMODE_UNBUFFERED, /**< Deglitch mode: disabled, buffered, unbuffered, or both */
    .outputMode         = CY_CTDAC_OUTPUT_VALUE,            /**< Output mode: enabled (value or value + 1), high-z, Vssa, or Vdda */
    .outputBuffer       = CY_CTDAC_OUTPUT_UNBUFFERED,       /**< Output path: Buffered through Opamp0 of CTB or connected directly to Pin 6 */
    .deepSleep          = CY_CTDAC_DEEPSLEEP_DISABLE,       /**< Enable or disable the CTDAC during Deep Sleep */
    .deglitchCycles     = CTDAC_DEGLITCH_CYCLES,            /**< Number of deglitch cycles from 0 to 63 */
    .value              = 0,        /**< Current DAC value */
    .nextValue          = 0,        /**< Next DAC value for double buffering */
    .enableInterrupt    = false,    /**< If true, enable interrupt when next value register is transferred to value register */
    .configClock        = false,    /**< Configure or ignore clock information */
};


static bool ctdac_initialized = 0;

static void ctdac_init(dac_t *obj)
{
    if (!ctdac_initialized) {
        uint32_t dac_clock_divider = CY_INVALID_DIVIDER;

        ctdac_initialized = true;
        // Allocate and setup clock.
        dac_clock_divider = cy_clk_allocate_divider(CY_SYSCLK_DIV_8_BIT);
        if (dac_clock_divider == CY_INVALID_DIVIDER) {
            error("CTDAC clock divider allocation failed.");
            return;
        }
        Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_8_BIT,
                                   dac_clock_divider,
                                   ((CY_CLK_PERICLK_FREQ_HZ + CTDAC_BASE_CLOCK_HZ / 2) / CTDAC_BASE_CLOCK_HZ) - 1);
        Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_8_BIT, dac_clock_divider);
        Cy_SysClk_PeriphAssignDivider(obj->clock, CY_SYSCLK_DIV_8_BIT, dac_clock_divider);

        Cy_CTDAC_Init(obj->base, &ctdac_config);
        Cy_CTDAC_Enable(obj->base);
    }
}


void analogout_init(dac_t *obj, PinName pin)
{
    uint32_t    dac = 0;
    uint32_t    dac_function = 0;

    MBED_ASSERT(obj);
    MBED_ASSERT(pin != (PinName)NC);

    dac = pinmap_peripheral(pin, PinMap_DAC);
    if (dac != (uint32_t)NC) {
        if (cy_reserve_io_pin(pin)) {
            error("ANALOG OUT pin reservation conflict.");
        }
        obj->base = (CTDAC_Type*)CY_PERIPHERAL_BASE(dac);
        obj->pin = pin;

        // Configure clock.
        dac_function = pinmap_function(pin, PinMap_DAC);
        obj->clock = CY_PIN_CLOCK(dac_function);
        pin_function(pin, dac_function);
        ctdac_init(obj);
    } else {
        error("ANALOG OUT pinout mismatch.");
    }
}

void analogout_free(dac_t *obj)
{
    // Not supported yet.
}

void analogout_write(dac_t *obj, float value)
{
    uint32_t val = 0;

    if (value > 1.0) {
        val = CTDAC_MAX_VALUE;
    } else if (value > 0.0) {
        val = value * CTDAC_MAX_VALUE;
    }
    Cy_CTDAC_SetValueBuffered(obj->base, val);
}

void analogout_write_u16(dac_t *obj, uint16_t value)
{
    uint32_t val = 0;

    val = (value >> (16 - CTDAC_NUM_BITS)); // Convert from 16-bit range.

    Cy_CTDAC_SetValueBuffered(obj->base, val);
}

float analogout_read(dac_t *obj)
{
    return (float)analogout_read_u16(obj) / 0xffff;
}

uint16_t analogout_read_u16(dac_t *obj)
{
    uint16_t value = (obj->base->CTDAC_VAL_NXT >> CTDAC_CTDAC_VAL_NXT_VALUE_Pos) & CTDAC_CTDAC_VAL_NXT_VALUE_Msk;

    value <<= (16 - CTDAC_NUM_BITS); // Convert to 16-bit range.

    return value;
}

const PinMap *analogout_pinmap()
{
    return PinMap_DAC;
}

#endif // DEVICE_ANALOGIN

