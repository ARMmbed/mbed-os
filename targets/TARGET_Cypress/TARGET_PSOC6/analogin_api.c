/*
 * mbed Microcontroller Library
 * Copyright (c) 2017-2018 Future Electronics
 * Copyright (c) 2018-2019 Cypress Semiconductor Corporation
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

#include "device.h"
#include "analogin_api.h"
#include "cy_sar.h"
#include "cy_sysanalog.h"
#include "psoc6_utils.h"
#include "mbed_assert.h"
#include "mbed_error.h"
#include "pinmap.h"
#include "PeripheralPins.h"
#include "platform/mbed_error.h"

#if DEVICE_ANALOGIN

const uint16_t ADC_MAX_VALUE = 0x0fff;

const uint32_t SAR_BASE_CLOCK_HZ = 18000000;    // 18 MHz or less

/** Default SAR channel configuration.
 *  Notice, that because dynamic SAR MUX switching is disabled,
 *  per-channel MUX configuration is ignored, thus not configured here.
 */
#define DEFAULT_CHANNEL_CONFIG  ( \
    CY_SAR_CHAN_SINGLE_ENDED | \
    CY_SAR_CHAN_AVG_ENABLE   | \
    CY_SAR_CHAN_SAMPLE_TIME_0 \
)

#define CY_SAR_PORT_9   (9uL)

/** Global SAR configuration data, modified as channels are configured.
 */
static cy_stc_sar_config_t sar_config = {
    .ctrl         = CY_SAR_VREF_SEL_VDDA_DIV_2 |
    CY_SAR_NEG_SEL_VREF |
    CY_SAR_CTRL_COMP_DLY_12 |
    CY_SAR_COMP_PWR_50 |
    CY_SAR_SARSEQ_SWITCH_DISABLE,       /**< Control register */
    .sampleCtrl   = CY_SAR_RIGHT_ALIGN |
    CY_SAR_SINGLE_ENDED_UNSIGNED |
    CY_SAR_AVG_CNT_16 |
    CY_SAR_AVG_MODE_SEQUENTIAL_FIXED |
    CY_SAR_TRIGGER_MODE_FW_ONLY,        /**< Sample control register */
    .sampleTime01 = (4uL << CY_SAR_SAMPLE_TIME0_SHIFT) |
    (4uL << CY_SAR_SAMPLE_TIME1_SHIFT), /**< Sample time in ADC clocks for ST0 and ST1 */
    .sampleTime23 = (4uL << CY_SAR_SAMPLE_TIME2_SHIFT) |
    (4uL << CY_SAR_SAMPLE_TIME3_SHIFT), /**< Sample time in ADC clocks for ST2 and ST3 */
    .rangeThres   = 0,                                  /**< Range detect threshold register for all channels (unused)*/
    .rangeCond    = 0,                                  /**< Range detect mode for all channels (unused)*/
    .chanEn       = 0,                                  /**< Enable bits for the channels */
    .chanConfig = {                                     /**< Channel configuration registers */
        DEFAULT_CHANNEL_CONFIG,     // chn 0
        DEFAULT_CHANNEL_CONFIG,     // chn 1
        DEFAULT_CHANNEL_CONFIG,     // chn 2
        DEFAULT_CHANNEL_CONFIG,     // chn 3
        DEFAULT_CHANNEL_CONFIG,     // chn 4
        DEFAULT_CHANNEL_CONFIG,     // chn 5
        DEFAULT_CHANNEL_CONFIG,     // chn 6
        DEFAULT_CHANNEL_CONFIG,     // chn 7
        DEFAULT_CHANNEL_CONFIG,     // chn 8
        DEFAULT_CHANNEL_CONFIG,     // chn 9
        DEFAULT_CHANNEL_CONFIG,     // chn 10
        DEFAULT_CHANNEL_CONFIG,     // chn 11
        DEFAULT_CHANNEL_CONFIG,     // chn 12
        DEFAULT_CHANNEL_CONFIG,     // chn 13
        DEFAULT_CHANNEL_CONFIG,     // chn 14
        DEFAULT_CHANNEL_CONFIG,     // chn 15
    },
    .intrMask     = 0,                                  /**< Interrupt enable mask */
    .satIntrMask  = 0,                                  /**< Saturate interrupt mask register */
    .rangeIntrMask = 0,                                 /**< Range interrupt mask register */
    .muxSwitch    = 0,                                  /**< SARMUX firmware switches to connect analog signals to SAR */
    .muxSwitchSqCtrl = 0,                               /**< SARMUX Switch SAR sequencer control */
    .configRouting = false,                             /**< Configure or ignore routing related registers (muxSwitch, muxSwitchSqCtrl) */
    .vrefMvValue = 0,                                   /**< Reference voltage in millivolts used in counts to volts conversion */
};

static bool sar_initialized = false;


static void sar_init(analogin_t *obj)
{
    if (!sar_initialized) {
        uint32_t sar_clock_divider = CY_INVALID_DIVIDER;

        sar_initialized = true;
        // Allocate and setup clock.
        sar_clock_divider = cy_clk_allocate_divider(CY_SYSCLK_DIV_8_BIT);
        if (sar_clock_divider == CY_INVALID_DIVIDER) {
            error("SAR clock divider allocation failed.");
            return;
        }
        Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_8_BIT,
                                   sar_clock_divider,
                                   ((cy_PeriClkFreqHz + SAR_BASE_CLOCK_HZ / 2) / SAR_BASE_CLOCK_HZ) - 1);
        Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_8_BIT, sar_clock_divider);
        Cy_SysClk_PeriphAssignDivider(obj->clock, CY_SYSCLK_DIV_8_BIT, sar_clock_divider);

        /* Init and Enable the Analog Reference for SAR ADC operation */
        Cy_SysAnalog_Init(&Cy_SysAnalog_Fast_Local);
        Cy_SysAnalog_Enable();

        Cy_SAR_Init(obj->base, &sar_config);
        Cy_SAR_Enable(obj->base);
    }
}

void analogin_init(analogin_t *obj, PinName pin)
{
    uint32_t    sar = 0;
    uint32_t    sar_function = 0;

    MBED_ASSERT(obj);
    MBED_ASSERT(pin != (PinName)NC);

    sar = pinmap_peripheral(pin, PinMap_ADC);
    if (sar != (uint32_t)NC) {

        if ((0 != cy_reserve_io_pin(pin)) && !sar_initialized) {
            error("ANALOG IN pin reservation conflict.");
        }

        /* Initialize object */
        obj->base = (SAR_Type *) CY_PERIPHERAL_BASE(sar);
        obj->pin = pin;
        obj->channel_mask = 1 << CY_PIN(pin);

        /* Configure SAR hardware */
        sar_function = pinmap_function(pin, PinMap_ADC);
        obj->clock = CY_PIN_CLOCK(sar_function);
        sar_init(obj);
        pin_function(pin, sar_function);

    } else {
        error("ANALOG IN pinout mismatch.");
    }
}

float analogin_read(analogin_t *obj)
{
    uint16_t result = analogin_read_u16(obj);

    return (float)result * (1.0 / ADC_MAX_VALUE);
}

uint16_t analogin_read_u16(analogin_t *obj)
{
    uint32_t result = 0;
    uint32_t port = CY_PORT(obj->pin);
    GPIO_PRT_Type *portPrt = Cy_GPIO_PortToAddr(port);

    Cy_SAR_SetChanMask(obj->base, obj->channel_mask);

    /* The port 10 uses the direct connection to the pin */
    if (CY_SAR_PORT_9 != port) {
        /* Connect the SAR Vplus input to the pin directly */
        Cy_SAR_SetAnalogSwitch(obj->base, CY_SAR_MUX_SWITCH0, obj->channel_mask, CY_SAR_SWITCH_CLOSE);
    } else {
        /* Connect the SAR Vplus input to the AMUXA bus */
        Cy_SAR_SetAnalogSwitch(obj->base, CY_SAR_MUX_SWITCH0, SAR_MUX_SWITCH0_MUX_FW_AMUXBUSA_VPLUS_Msk, CY_SAR_SWITCH_CLOSE);

        /* Connect the AMUXA bus to the pin */
        Cy_GPIO_SetHSIOM(portPrt, CY_PIN(obj->pin), HSIOM_SEL_AMUXA);
    }

    Cy_SAR_StartConvert(obj->base, CY_SAR_START_CONVERT_SINGLE_SHOT);
    if (Cy_SAR_IsEndConversion(obj->base, CY_SAR_WAIT_FOR_RESULT) == CY_SAR_SUCCESS) {
        result = Cy_SAR_GetResult32(obj->base, CY_PIN(obj->pin));
    } else {
        error("ANALOG IN: measurement failed!");
    }

    if (CY_SAR_PORT_9 != port) {
        /* Disconnect the SAR Vplus input from the pin */
        Cy_SAR_SetAnalogSwitch(obj->base, CY_SAR_MUX_SWITCH0, obj->channel_mask, CY_SAR_SWITCH_OPEN);
    } else {
        /* Disconnect the AMUXA bus from the pin */
        Cy_GPIO_SetHSIOM(portPrt, CY_PIN(obj->pin), HSIOM_SEL_GPIO);

        /* Disconnect the SAR Vplus input from the AMUXA bus */
        Cy_SAR_SetAnalogSwitch(obj->base, CY_SAR_MUX_SWITCH0, SAR_MUX_SWITCH0_MUX_FW_AMUXBUSA_VPLUS_Msk, CY_SAR_SWITCH_OPEN);
    }

    // We are running 16x oversampling extending results to 16 bits.
    return (uint16_t)(result);
}

const PinMap *analogin_pinmap()
{
    return PinMap_ADC;
}

#endif // DEVICE_ANALOGIN

