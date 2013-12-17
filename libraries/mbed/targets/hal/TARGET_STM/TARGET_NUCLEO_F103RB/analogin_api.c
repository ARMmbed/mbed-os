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
#include "analogin_api.h"
#include "wait_api.h"

#if DEVICE_ANALOGIN

#include "cmsis.h"
#include "pinmap.h"
#include "error.h"

static const PinMap PinMap_ADC[] = {
    {PA_0, ADC_1, STM_PIN_DATA(GPIO_Mode_AIN, 0)},
    {PA_1, ADC_1, STM_PIN_DATA(GPIO_Mode_AIN, 0)},
    {PA_4, ADC_1, STM_PIN_DATA(GPIO_Mode_AIN, 0)},
    {PB_0, ADC_1, STM_PIN_DATA(GPIO_Mode_AIN, 0)},
    {PC_1, ADC_1, STM_PIN_DATA(GPIO_Mode_AIN, 0)},
    {PC_0, ADC_1, STM_PIN_DATA(GPIO_Mode_AIN, 0)},
    {NC,   NC,    0}
};

int adc_inited = 0;

void analogin_init(analogin_t *obj, PinName pin) {
  
    ADC_TypeDef     *adc;
    ADC_InitTypeDef ADC_InitStructure;
  
    // Get the peripheral name (ADC_1, ADC_2...) from the pin and assign it to the object
    obj->adc = (ADCName)pinmap_peripheral(pin, PinMap_ADC);
 
    if (obj->adc == (ADCName)NC) {
      error("ADC pin mapping failed");
    }

    // Configure GPIO
    pinmap_pinout(pin, PinMap_ADC);

    // Save pin number for the read function
    obj->pin = pin;

    // The ADC initialization is done once
    if (adc_inited == 0) {
        adc_inited = 1;

        // Get ADC registers structure address
        adc = (ADC_TypeDef *)(obj->adc);
      
        // Enable ADC clock
        RCC_ADCCLKConfig(RCC_PCLK2_Div4);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
               
        // Configure ADC
        ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
        ADC_InitStructure.ADC_ScanConvMode = DISABLE;
        ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
        ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
        ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
        ADC_InitStructure.ADC_NbrOfChannel = 1;
        ADC_Init(adc, &ADC_InitStructure);

        // Enable ADC
        ADC_Cmd(adc, ENABLE);

        // Calibrate ADC
        ADC_ResetCalibration(adc);
        while(ADC_GetResetCalibrationStatus(adc));
        ADC_StartCalibration(adc);
        while(ADC_GetCalibrationStatus(adc));    
    }
}

static inline uint16_t adc_read(analogin_t *obj) {
  // Get ADC registers structure address
  ADC_TypeDef *adc = (ADC_TypeDef *)(obj->adc);
  
  // Configure ADC channel
  switch (obj->pin) {
      case PA_0:
          ADC_RegularChannelConfig(adc, ADC_Channel_0, 1, ADC_SampleTime_7Cycles5);
          break;
      case PA_1:
          ADC_RegularChannelConfig(adc, ADC_Channel_1, 1, ADC_SampleTime_7Cycles5);
          break;
      case PA_4:
          ADC_RegularChannelConfig(adc, ADC_Channel_4, 1, ADC_SampleTime_7Cycles5);
          break;
      case PB_0:
          ADC_RegularChannelConfig(adc, ADC_Channel_8, 1, ADC_SampleTime_7Cycles5);
          break;
      case PC_1:
          ADC_RegularChannelConfig(adc, ADC_Channel_11, 1, ADC_SampleTime_7Cycles5);
          break;
      case PC_0:
          ADC_RegularChannelConfig(adc, ADC_Channel_10, 1, ADC_SampleTime_7Cycles5);
          break;
      default:
          return 0;
  }

  ADC_SoftwareStartConvCmd(adc, ENABLE); // Start conversion
  
  while(ADC_GetFlagStatus(adc, ADC_FLAG_EOC) == RESET); // Wait end of conversion
  
  return(ADC_GetConversionValue(adc)); // Get conversion value
}

uint16_t analogin_read_u16(analogin_t *obj) {
  return(adc_read(obj));
}

float analogin_read(analogin_t *obj) {
  uint16_t value = adc_read(obj);
  return (float)value * (1.0f / (float)0xFFF); // 12 bits range
}

#endif
