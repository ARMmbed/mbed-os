/*******************************************************************************
 * Copyright (c) 2010-2017 Analog Devices, Inc.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   - Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *   - Modified versions of the software must be conspicuously marked as such.
 *   - This software is licensed solely and exclusively for use with processors
 *     manufactured by or for Analog Devices, Inc.
 *   - This software may not be combined or merged with other code in any manner
 *     that would cause the software to become subject to terms and conditions
 *     which differ from those listed here.
 *   - Neither the name of Analog Devices, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *   - The use of this software may or may not infringe the patent rights of one
 *     or more patent holders.  This license does not release you from the
 *     requirement that you obtain separate licenses from these patent holders
 *     to use this software.
 *
 * THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-
 * INFRINGEMENT, TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF
 * CLAIMS OF INTELLECTUAL PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/

#include "mbed_assert.h"
#include "analogin_api.h"

#if DEVICE_ANALOGIN

#include "adi_adc_def.h"
#include "pinmap.h"
#include "PeripheralPins.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ADC Device number */
#define ADC_DEV_NUM                (0u)

/* Memory Required for adc driver */
static uint32_t DeviceMemory[(ADI_ADC_MEMORY_SIZE+3)/4];
/* Active channel */
static uint32_t adi_pin2channel(PinName pin);

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
    uint32_t function, channel;

    memset(obj, 0, sizeof(analogin_t) );
    memset( DeviceMemory, 0, sizeof( DeviceMemory ) );

    peripheral = (ADCName)pinmap_peripheral(pin, &PinMap_ADC[0]);   // gives peripheral
    MBED_ASSERT(peripheral != (ADCName)NC);

    /* verify read function */
    function = pinmap_function(pin, &PinMap_ADC[0]);
    MBED_ASSERT(function == 1);

    /* Configure PORT2_MUX registers */
    pin_function(pin, function);

    /* Configure active channel */
    channel = adi_pin2channel(pin);
    MBED_ASSERT(channel != 0xFFFFFFFF);
    obj->UserBuffer.nChannels = channel;

    /* Set ACLK to CCLK/16 */
    adi_pwr_SetClockDivider(ADI_CLOCK_ACLK,16);

    /* Set default values for conversion and delay cycles. This sets up a sampling rate of
       16kHz. The sampling frequency is worked out from the following:

       if delay time > 0:
            Fs = ACLK / [((14 + sampling time) * oversample factor) + (delay time + 2)]
       if delay time = 0:
            Fs = ACLK / ((14 + sampling time) * oversample factor)

       The sampling (or acquisition) and delay times are in number of ACLK clock cycles.
    */
    obj->DelayCycles = 0;
    obj->SampleCycles = 88;

    /* Open the ADC device */
    adi_adc_Open(ADC_DEV_NUM, DeviceMemory, sizeof(DeviceMemory), &hDevice);
    obj->hDevice = hDevice;

    /* Power up ADC */
    adi_adc_PowerUp(hDevice, true);

    /* Set ADC reference */
    adi_adc_SetVrefSource(hDevice, ADI_ADC_VREF_SRC_INT_2_50_V);

    /* Enable ADC sub system */
    adi_adc_EnableADCSubSystem(hDevice, true);

    /* Wait untilthe ADC is ready for sampling */
    while(bReady == false) {
        adi_adc_IsReady(hDevice, &bReady);
    }

    /* Start calibration */
    adi_adc_StartCalibration(hDevice);

    /* Wait until calibration is done */
    while (!bCalibrationDone) {
        adi_adc_IsCalibrationDone(hDevice, &bCalibrationDone);
    }

    /* Set the delay time */
    adi_adc_SetDelayTime(hDevice, obj->DelayCycles);

    /* Set the acquisition time. (Application need to change it based on the impedence) */
    adi_adc_SetAcquisitionTime(hDevice, obj->SampleCycles);

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
    uint32_t        ADCsample;

    obj->UserBuffer.pDataBuffer = &ADCsample;
    obj->UserBuffer.nNumConversionPasses = 1;
    obj->UserBuffer.nBuffSize = 1;

    /* Submit the buffer to the driver */
    adi_adc_SubmitBuffer(hDevice, &obj->UserBuffer);

    /* Enable the ADC */
    adi_adc_Enable(hDevice, true);

    adi_adc_GetBuffer(hDevice, &pAdcBuffer);
    MBED_ASSERT(pAdcBuffer == &obj->UserBuffer);

    return( (uint16_t)( ((uint16_t *)pAdcBuffer->pDataBuffer)[(pAdcBuffer->nNumConversionPasses) - 1]) );
}

const PinMap *analogin_pinmap()
{
    return PinMap_ADC;
}

/* Retrieve the active channel corresponding to the input pin */
static uint32_t adi_pin2channel(PinName pin)
{

    uint32_t activech;

    switch(pin) {
        case ADC_VIN0:
            activech = ADI_ADC_CHANNEL_0;
            break;
        case ADC_VIN1:
            activech = ADI_ADC_CHANNEL_1;
            break;
        case ADC_VIN2:
            activech = ADI_ADC_CHANNEL_2;
            break;
        case ADC_VIN3:
            activech = ADI_ADC_CHANNEL_3;
            break;
        case ADC_VIN4:
            activech = ADI_ADC_CHANNEL_4;
            break;
        case ADC_VIN5:
            activech = ADI_ADC_CHANNEL_5;
            break;
        case ADC_VIN6:
            activech = ADI_ADC_CHANNEL_6;
            break;
        case ADC_VIN7:
            activech = ADI_ADC_CHANNEL_7;
            break;
        default:
            activech = (uint32_t) 0xFFFFFFFF;
            break;
    }

    return ((uint32_t)activech);
}



/**@}*/

#ifdef __cplusplus
}
#endif

#endif // #if DEVICE_ANALOGIN
