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
 
#include "analogin_api.h"
#include "adi_adc_def.h"
 
#if DEVICE_ANALOGIN

/** Analogin hal structure. analogin_s is declared in the target's hal
 */
typedef struct analogin_s analogin_t;
 
#ifdef __cplusplus
extern "C" {
#endif

/* ADC Device number */
#define ADC_DEV_NUM                (0u)

/* Memory Required for adc driver */
static uint8_t DeviceMemory[ADI_ADC_MEMORY_SIZE] __attribute__((aligned(4)));

static uint32_t adi_init_pinmux(PinName pin);

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
    ADI_ADC_RESULT  eResult = ADI_ADC_FAILURE;
    ADI_ADC_HANDLE hDevice;
    bool bCalibrationDone = false; 
    bool bReady = false;
    
    adi_pwr_Init();    
    adi_pwr_SetClockDivider(ADI_CLOCK_HCLK,1);
    adi_pwr_SetClockDivider(ADI_CLOCK_PCLK,1);
    
    obj->UserBuffer.nChannels = adi_init_pinmux(pin);
    
    /* Open the ADC device */
    adi_adc_Open(ADC_DEV_NUM, DeviceMemory, sizeof(DeviceMemory),&hDevice);
    
    obj->hDevice = hDevice;    
    
    /* Power up ADC */
    adi_adc_PowerUp (hDevice, true);

    /* Set ADC reference */
    adi_adc_SetVrefSource (hDevice, ADI_ADC_VREF_SRC_INT_2_50_V);
    
    /* Enable ADC sub system */
    adi_adc_EnableADCSubSystem (hDevice, true);

    /* Wait untilthe ADC is ready for sampling */
    while(bReady == false) {
    	adi_adc_IsReady (hDevice, &bReady);
	}

    /* Start calibration */
    eResult = adi_adc_StartCalibration (hDevice);

    /* Wait until calibration is done */
    while (!bCalibrationDone)
    {
        adi_adc_IsCalibrationDone (hDevice, &bCalibrationDone);
    }

    /* Set the delay time */
    eResult = adi_adc_SetDelayTime ( hDevice, obj->DelayCycles);

    /* Set the acquisition time. (Application need to change it based on the impedence) */
    eResult = adi_adc_SetAcquisitionTime ( hDevice, obj->SampleCycles);

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
    adi_adc_SubmitBuffer (hDevice, &obj->UserBuffer);

    /* Enable the ADC */
    adi_adc_Enable (hDevice, true);
    
    adi_adc_GetBuffer (hDevice, &pAdcBuffer);

    return( *((uint16_t *)(obj->UserBuffer.pDataBuffer)) ); 
}


#define ADC0_IN_ADC0_VIN0_PORTP2_MUX  ((uint16_t) ((uint16_t) 1<<6))
#define ADC0_IN_ADC0_VIN1_PORTP2_MUX  ((uint16_t) ((uint16_t) 1<<8))
#define ADC0_IN_ADC0_VIN2_PORTP2_MUX  ((uint16_t) ((uint16_t) 1<<10))
#define ADC0_IN_ADC0_VIN3_PORTP2_MUX  ((uint16_t) ((uint16_t) 1<<12))
#define ADC0_IN_ADC0_VIN4_PORTP2_MUX  ((uint16_t) ((uint16_t) 1<<14))
#define ADC0_IN_ADC0_VIN5_PORTP2_MUX  ((uint16_t) ((uint32_t) 1<<16))

/* Configure PORTx_MUX registers */
static uint32_t adi_init_pinmux(PinName pin) {
    
    uint32_t activech; 
   
    switch(pin) {
        
        case A0:
            activech = ADI_ADC_CHANNEL_0;
            *pREG_GPIO2_CFG = ADC0_IN_ADC0_VIN0_PORTP2_MUX;
            break;             
        case A1:
            activech = ADI_ADC_CHANNEL_1;
            *pREG_GPIO2_CFG = ADC0_IN_ADC0_VIN1_PORTP2_MUX;
            break;               
        case A2:
            activech = ADI_ADC_CHANNEL_2;
            *pREG_GPIO2_CFG = ADC0_IN_ADC0_VIN2_PORTP2_MUX;
            break;                                 
        case A3:
            activech = ADI_ADC_CHANNEL_3;
            *pREG_GPIO2_CFG = ADC0_IN_ADC0_VIN3_PORTP2_MUX;
            break; 
        case A4:
            activech = ADI_ADC_CHANNEL_4;
            *pREG_GPIO2_CFG = ADC0_IN_ADC0_VIN4_PORTP2_MUX;
            break;  
        case A5:
            activech = ADI_ADC_CHANNEL_5;
            *pREG_GPIO2_CFG = ADC0_IN_ADC0_VIN5_PORTP2_MUX;
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
