/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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

#if DEVICE_ANALOGIN

//#include "adi_adc_def_v1.h"
#include "pinmap.h"
#include "PeripheralPins.h"

/** Analogin hal structure. analogin_s is declared in the target's hal
 */
typedef struct analogin_s analogin_t;

#ifdef __cplusplus
extern "C" {
#endif

/* ADC Device number */
#define ADC_DEV_NUM                (0u)

/* Memory Required for adc driver */
static uint32_t DeviceMemory[(ADI_ADC_MEMORY_SIZE+3)/4];
/* Active channel */
static uint32_t adi_get_channel(PinName pin);

/**
 * \defgroup hal_analogin Analogin hal functions
 * @{
 */

/** Initialize the analogin peripheral
 *
 * Configures the pin used by analogin.
 * @param obj The analogin object to initialize
 * @param pin The analogin pin name
 */
void analogin_init(analogin_t *obj, PinName pin)
{
    ADI_ADC_HANDLE hDevice;
    bool bCalibrationDone = false;
    bool bReady = false;

    ADCName peripheral;
    uint32_t function;

    uint16_t nAveragingSamples = 4;

    peripheral = (ADCName)pinmap_peripheral(pin, &PinMap_ADC[0]);	// gives peripheral
    MBED_ASSERT(peripheral != (ADCName)NC);

    function = pinmap_function(pin, &PinMap_ADC[0]);

    /* Configure PORT2_MUX registers */
    pin_function(pin, function);

    /* Configure active channel */
    obj->UserBuffer.nChannels = adi_get_channel(pin);

    /* Open the ADC device */
    adi_adc_Open(ADC_DEV_NUM, DeviceMemory, sizeof(DeviceMemory),&hDevice);
    obj->hDevice = hDevice;

    /* Power up ADC */
    adi_adc_PowerUp(hDevice, true);

    /* Set ADC reference */
    adi_adc_SetVrefSource(hDevice, ADI_ADC_VREF_SRC_INT_2_50_V);

    /* Enable ADC sub system */
    adi_adc_EnableADCSubSystem (hDevice, true);

    /* Wait untilthe ADC is ready for sampling */
    while(bReady == false) {
        adi_adc_IsReady(hDevice, &bReady);
    }

    /* Start calibration */
    adi_adc_StartCalibration (hDevice);

    /* Wait until calibration is done */
    while (!bCalibrationDone) {
        adi_adc_IsCalibrationDone (hDevice, &bCalibrationDone);
    }

    /* Set the delay time */
    adi_adc_SetDelayTime(hDevice, obj->DelayCycles);

    /* Set the acquisition time. (Application need to change it based on the impedence) */
    adi_adc_SetAcquisitionTime(hDevice, obj->SampleCycles);

    adi_adc_EnableAveraging(hDevice, nAveragingSamples );
}

/** Read the input voltage, represented as a float in the range [0.0, 1.0]
 *
 * @param obj The analogin object
 * @return A floating value representing the current input voltage
 */
float analogin_read(analogin_t *obj)
{
    float fl32 = (float)analogin_read_u16(obj)/(float)4095.0;

    return(fl32);
}

/** Read the value from analogin pin, represented as an unsigned 16bit value
 *
 * @param obj The analogin object
 * @return An unsigned 16bit value representing the current input voltage
 */
uint16_t analogin_read_u16(analogin_t *obj)
{
    ADI_ADC_HANDLE  hDevice = obj->hDevice;
    ADI_ADC_BUFFER  *pAdcBuffer;

    /* Submit the buffer to the driver */
    adi_adc_SubmitBuffer(hDevice, &obj->UserBuffer);

    /* Enable the ADC */
    adi_adc_Enable(hDevice, true);

    adi_adc_GetBuffer(hDevice, &pAdcBuffer);

    return( *((uint16_t *)(obj->UserBuffer.pDataBuffer)) );
}

/* Retrieve te active channel correspondoing to the input pin */
static uint32_t adi_get_channel(PinName pin)
{

    uint32_t activech;

    switch(pin) {
        case A0:
            activech = ADI_ADC_CHANNEL_0;
            break;
        case A1:
            activech = ADI_ADC_CHANNEL_1;
            break;
        case A2:
            activech = ADI_ADC_CHANNEL_2;
            break;
        case A3:
            activech = ADI_ADC_CHANNEL_3;
            break;
        case A4:
            activech = ADI_ADC_CHANNEL_4;
            break;
        case A5:
            activech = ADI_ADC_CHANNEL_5;
            break;
        case A6:
            activech = ADI_ADC_CHANNEL_6;
            break;
        case A7:
            activech = ADI_ADC_CHANNEL_7;
            break;
        default:
            activech = (uint32_t) 0xFFFFFFFF;
            break;
    }

    return(activech);
}



/**@}*/

#ifdef __cplusplus
}
#endif

#endif // #if DEVICE_ANALOGIN
