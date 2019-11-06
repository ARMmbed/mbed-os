/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include "mbed_assert.h"
#include "analogin_api.h"

#if DEVICE_ANALOGIN && defined(NXP_LPADC)

#include "cmsis.h"
#include "pinmap.h"
#include "gpio_api.h"
#include "PeripheralNames.h"
#include "fsl_lpadc.h"
#include "fsl_power.h"
#include "PeripheralPins.h"

/* Array of ADC peripheral base address. */
static ADC_Type *const adc_addrs[] = ADC_BASE_PTRS;
extern void ADC_ClockPower_Configuration(void);

#define LPADC_USER_CMDID 1U /* CMD1 */

#if STATIC_PINMAP_READY
#define ANALOGIN_INIT_DIRECT analogin_init_direct
void analogin_init_direct(analogin_t *obj, const PinMap *pinmap)
#else
#define ANALOGIN_INIT_DIRECT _analogin_init_direct
static void _analogin_init_direct(analogin_t *obj, const PinMap *pinmap)
#endif
{
    gpio_t gpio;

    obj->adc = (ADCName)pinmap->peripheral;
    MBED_ASSERT(obj->adc != (ADCName)NC);

    uint32_t instance = obj->adc >> ADC_INSTANCE_SHIFT;
    lpadc_config_t adc_config;
    uint32_t reg;
    uint32_t pin_number = pinmap->pin & 0x1F;
    uint8_t port_number = pinmap->pin / 32;

    ADC_ClockPower_Configuration();

    LPADC_GetDefaultConfig(&adc_config);
    adc_config.enableAnalogPreliminary = true;
#if defined(LPADC_VREF_SOURCE)
    adc_config.referenceVoltageSource = LPADC_VREF_SOURCE;
#endif
#if defined(FSL_FEATURE_LPADC_HAS_CTRL_CAL_AVGS) && FSL_FEATURE_LPADC_HAS_CTRL_CAL_AVGS
    adc_config.conversionAverageMode = kLPADC_ConversionAverage128;
#endif /* FSL_FEATURE_LPADC_HAS_CTRL_CAL_AVGS */
    LPADC_Init(adc_addrs[instance], &adc_config);

#if defined(FSL_FEATURE_LPADC_HAS_CTRL_CALOFS) && FSL_FEATURE_LPADC_HAS_CTRL_CALOFS
#if defined(FSL_FEATURE_LPADC_HAS_OFSTRIM) && FSL_FEATURE_LPADC_HAS_OFSTRIM
    /* Request offset calibration. */
    if (true == LPADC_DO_OFFSET_CALIBRATION) {
        LPADC_DoOffsetCalibration(adc_addrs[instance]);
    } else {
        LPADC_SetOffsetValue(adc_addrs[instance], LPADC_OFFSET_VALUE_A, LPADC_OFFSET_VALUE_B);
    }
#endif /* FSL_FEATURE_LPADC_HAS_OFSTRIM */
    /* Request gain calibration. */
    LPADC_DoAutoCalibration(adc_addrs[instance]);
#endif /* FSL_FEATURE_LPADC_HAS_CTRL_CALOFS */

#if (defined(FSL_FEATURE_LPADC_HAS_CFG_CALOFS) && FSL_FEATURE_LPADC_HAS_CFG_CALOFS)
    /* Do auto calibration. */
    LPADC_DoAutoCalibration(adc_addrs[instance]);
#endif /* FSL_FEATURE_LPADC_HAS_CFG_CALOFS */

    pin_function(pinmap->pin, pinmap->function);
    pin_mode(pinmap->pin, PullNone);

    /* Need to ensure the pin is in input mode */
    gpio_init(&gpio, pinmap->pin);
    gpio_dir(&gpio, PIN_INPUT);

    reg = IOCON->PIO[port_number][pin_number];

    /* Clear the MODE & DIGIMODE bit */
    reg &= ~(IOCON_PIO_DIGIMODE_MASK | IOCON_PIO_MODE_MASK);

    /* For pins PIO0_9, PIO0_11, PIO0_12, PIO0_15, PIO0_18, PIO0_31, PIO1_0 and
       PIO1_9, leave ASW bit at '0' in the related IOCON register. */
    if (((port_number == 0) && ((pin_number == 9) || (pin_number == 11) || (pin_number == 12) ||
                                (pin_number == 15) || (pin_number == 18) || (pin_number == 31))) ||
       ((port_number == 1) && ((pin_number == 0) || (pin_number == 9)))) {
        /* Disable Analog Switch Input control */
        reg &= ~IOCON_PIO_ASW_MASK;
    } else {
        /* Enable Analog Switch Input control */
        reg |= IOCON_PIO_ASW_MASK;
    }

    IOCON->PIO[port_number][pin_number] = reg;
}

void analogin_init(analogin_t *obj, PinName pin)
{
    int peripheral = (int)pinmap_peripheral(pin, PinMap_ADC);
    int function = (int)pinmap_find_function(pin, PinMap_ADC);

    const PinMap static_pinmap = {pin, peripheral, function};

    ANALOGIN_INIT_DIRECT(obj, &static_pinmap);
}

uint16_t analogin_read_u16(analogin_t *obj)
{
    uint32_t instance = obj->adc >> ADC_INSTANCE_SHIFT;
    uint32_t channel = obj->adc & 0xF;
    lpadc_conv_trigger_config_t mLpadcTriggerConfigStruct;
    lpadc_conv_command_config_t mLpadcCommandConfigStruct;
    lpadc_conv_result_t mLpadcResultConfigStruct;

    memset(&mLpadcTriggerConfigStruct, 0, sizeof(mLpadcTriggerConfigStruct));
    memset(&mLpadcCommandConfigStruct, 0, sizeof(mLpadcCommandConfigStruct));
    memset(&mLpadcResultConfigStruct, 0, sizeof(mLpadcResultConfigStruct));

#if (defined(FSL_FEATURE_LPADC_FIFO_COUNT) && (FSL_FEATURE_LPADC_FIFO_COUNT == 2))
    LPADC_DoResetFIFO0(adc_addrs[instance]);
    LPADC_DoResetFIFO1(adc_addrs[instance]);
#else
    LPADC_DoResetFIFO(adc_addrs[instance]);
#endif /* FSL_FEATURE_LPADC_FIFO_COUNT */

    /* Set conversion CMD configuration. */
    LPADC_GetDefaultConvCommandConfig(&mLpadcCommandConfigStruct);

    /* Check if we should use the B channel */
    if (obj->adc >> ADC_B_CHANNEL_SHIFT) {
        mLpadcCommandConfigStruct.sampleChannelMode = kLPADC_SampleChannelSingleEndSideB;
    }

    mLpadcCommandConfigStruct.channelNumber = channel;
    mLpadcCommandConfigStruct.conversionResoultuionMode = kLPADC_ConversionResolutionStandard;
    LPADC_SetConvCommandConfig(adc_addrs[instance], LPADC_USER_CMDID, &mLpadcCommandConfigStruct);

    /* Set trigger configuration. */
    LPADC_GetDefaultConvTriggerConfig(&mLpadcTriggerConfigStruct);
    mLpadcTriggerConfigStruct.targetCommandId = LPADC_USER_CMDID;
    mLpadcTriggerConfigStruct.enableHardwareTrigger = false;
    LPADC_SetConvTriggerConfig(adc_addrs[instance], 0U, &mLpadcTriggerConfigStruct); /* Configure the trigger0. */

    LPADC_DoSoftwareTrigger(adc_addrs[instance], 1U); /* 1U is trigger0 mask. */

#if (defined(FSL_FEATURE_LPADC_FIFO_COUNT) && (FSL_FEATURE_LPADC_FIFO_COUNT == 2U))
    while (!LPADC_GetConvResult(adc_addrs[instance], &mLpadcResultConfigStruct, 0U)) {
    }
#else
    while (!LPADC_GetConvResult(adc_addrs[instance], &mLpadcResultConfigStruct)) {
    }
#endif /* FSL_FEATURE_LPADC_FIFO_COUNT */

    return (mLpadcResultConfigStruct.convValue << 1);
}

float analogin_read(analogin_t *obj)
{
    uint16_t value = analogin_read_u16(obj);
    return (float)value * (1.0f / (float)0xFFF0);
}

const PinMap *analogin_pinmap()
{
    return PinMap_ADC;
}

#endif
