/**
  ******************************************************************************
  * @file    BlueNRG1_adc.h
  * @author  VMA Application Team
  * @version V2.4.0
  * @date    4-May-2018
  * @brief   This file contains all the functions prototypes for the ADC firmware 
  *          library.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef BLUENRG1_ADC_H
#define BLUENRG1_ADC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "BlueNRG_x_device.h"

/** @addtogroup BLUENRG1_Peripheral_Driver BLUENRG1 Peripheral Driver
  * @{
  */

/** @addtogroup ADC_Peripheral  ADC Peripheral
  * @{
  */

/** @defgroup ADC_Exported_Types Exported Types
  * @{
  */

/** 
  * @brief  Structure definition of ADC initialization 
  */  
typedef struct {
  uint8_t ADC_OSR;                              /*!< Specifies the decimation rate.
                                                  This parameter can be a value of @ref ADC_OSR */
  
  uint8_t ADC_Input;                            /*!< Specifies the input used for the conversion.
                                                  This parameter can be a value of @ref ADC_Input */
  
  uint8_t ADC_ConversionMode;                   /*!< Specifies the conversion mode.
                                                  This parameter can be a value of @ref ADC_ConversionMode */
  
  uint8_t ADC_Attenuation;                      /*!< Specifies the attenuation.
                                                  This parameter can be a value of @ref ADC_Attenuation */
  
  uint8_t ADC_ReferenceVoltage;                 /*!< Specifies the ADC reference voltage.
                                                  This parameter can be a value of @ref ADC_ReferenceVoltage */
} ADC_InitType;

/**
  * @}
  */ 

  

/** @defgroup ADC_Exported_Constants Exported Constants
  * @{
  */


/** @defgroup ADC_Decimation_Rate_Definitions   Decimation Rate Definitions
  * @{
  */
#define ADC_OSR_200                         ((uint8_t)0x00)  /* Recommended value for DC signals */
#define ADC_OSR_100                         ((uint8_t)0x01)
#define ADC_OSR_64                          ((uint8_t)0x02)
#define ADC_OSR_32                          ((uint8_t)0x03)

#define IS_ADC_OSR(OSR) (((OSR) == ADC_OSR_200) || \
                                       ((OSR) == ADC_OSR_100) || \
                                       ((OSR) == ADC_OSR_64) || \
                                       ((OSR) == ADC_OSR_32))
/**
  * @}
  */ 


/** @defgroup ADC_Input_Definitions   Input Definitions
  * @{
  */
#define ADC_Input_None                          ((uint8_t)0x00)
#define ADC_Input_AdcPin2                       ((uint8_t)0x01) /* Single ended input from ADC2 pin */
#define ADC_Input_AdcPin1                       ((uint8_t)0x02) /* Single ended input from ADC1 pin */
#define ADC_Input_AdcPin12                      ((uint8_t)0x03) /* Differential input */
#define ADC_Input_TempSensor                    ((uint8_t)0x04) /* Temperature sensor */
#define ADC_Input_BattSensor                    ((uint8_t)0x05) /* Internal battery detector */
#define ADC_Input_Internal0V60V6                ((uint8_t)0x06) /* Test input */
#define ADC_Input_Internal1V20V0                ((uint8_t)0x07) /* Test input */
#define ADC_Input_Microphone                    ((uint8_t)0x0F) /* Microphone mode */

#define IS_ADC_INPUT(INPUT) (((INPUT) == ADC_Input_None) || \
                                             ((INPUT) == ADC_Input_AdcPin1) || \
                                             ((INPUT) == ADC_Input_AdcPin2) || \
                                             ((INPUT) == ADC_Input_AdcPin12) || \
                                             ((INPUT) == ADC_Input_TempSensor) || \
                                             ((INPUT) == ADC_Input_BattSensor) || \
                                             ((INPUT) == ADC_Input_Internal0V60V6) || \
                                             ((INPUT) == ADC_Input_Internal1V20V0) || \
                                             ((INPUT) == ADC_Input_Microphone))

/**
  * @}
  */ 


/** @defgroup ADC_Conversion_Mode_Definitions   Conversion Mode Definitions
  * @{
  */
#define ADC_ConversionMode_Single                          ((uint8_t)0x00)

/* The user must discards the first 10 converted samples in continuous mode if the SKIP bit is 0. 
 * The user must discards the first 3 converted samples in continuous mode if the SKIP bit is 1.  */
#define ADC_ConversionMode_Continuous                      ((uint8_t)0x08)

#define IS_ADC_CONVERSIONMODE(CONVERSIONMODE) (((CONVERSIONMODE) == ADC_ConversionMode_Single) || \
                                              ((CONVERSIONMODE) == ADC_ConversionMode_Continuous))
/**
  * @}
  */ 



/** @defgroup ADC_Attenuation_Definitions   Attenuation Definitions
  * @{
  */
#define ADC_Attenuation_0dB                      ((uint8_t)0x00)
#define ADC_Attenuation_6dB02                    ((uint8_t)0x01)
#define ADC_Attenuation_9dB54                    ((uint8_t)0x02)

#define IS_ADC_ATTENUATION(ATTENUATION) (((ATTENUATION) == ADC_Attenuation_0dB) || \
                                        ((ATTENUATION) == ADC_Attenuation_6dB02) || \
                                        ((ATTENUATION) == ADC_Attenuation_9dB54))
/**
  * @}
  */ 


/** @defgroup ADC_Reference_Voltage_Definitions   Reference Voltage Definitions
  * @{
  */

#define ADC_ReferenceVoltage_0V6             ((uint8_t)0x02)    /* Optimal setting */
#define ADC_ReferenceVoltage_0V0             ((uint8_t)0x00)    /* Default value: not recommended   */

#define IS_ADC_REFERENCEVOLTAGE(REFERENCEVOLTAGE) (((REFERENCEVOLTAGE) == ADC_ReferenceVoltage_0V6) || \
                                                  ((REFERENCEVOLTAGE) == ADC_ReferenceVoltage_0V0))
/**
  * @}
  */ 


/** @defgroup ADC_Mic_Frequency_Definitions    Mic Frequency Definitions
  * @{
  */
#define ADC_MIC_800KHZ         ((uint8_t)0x00)  /*!< Enable the 800 kHz through IO0 for external MEMS microphone */
#define ADC_MIC_1600KHZ        ((uint8_t)0x01)  /*!< Enable the 1.6 MHz through IO0 for external MEMS microphone */

#define IS_ADC_MIC_FREQ_SEL(FREQ) (((FREQ) == ADC_MIC_800KHZ) || ((FREQ) == ADC_MIC_1600KHZ))
    
/**
  * @}
  */ 

/** @defgroup ADC_Flags_Definitions   Flags Definitions
  * @{
  */
    
#define ADC_FLAG_CAL          ((uint8_t)0x01)  /*!< End of Calibration flag */
#define ADC_FLAG_BUSY         ((uint8_t)0x02)  /*!< Busy flag */
#define ADC_FLAG_EOC          ((uint8_t)0x04)  /*!< End of Conversion flag */
#define ADC_FLAG_WDG          ((uint8_t)0x08)  /*!< Value within set threshold */

#define IS_ADC_GET_FLAG(FLAG) (((FLAG) & 0x0F) != 0)

/**
  * @}
  */ 
 
/**
  * @}
  */


/** @defgroup ADC_Exported_Functions Exported Functions
  * @{
  */
void ADC_Init(ADC_InitType* ADC_InitStruct);
void ADC_DeInit(void);
void ADC_StructInit(ADC_InitType* ADC_InitStruct);
void ADC_Cmd(FunctionalState NewState);
void ADC_DmaCmd(FunctionalState NewState);
void ADC_Filter(FunctionalState NewState);
void ADC_SelectInput(uint8_t AdcInput);
void ADC_Calibration(FunctionalState NewState);
void ADC_AutoOffsetUpdate(FunctionalState NewState);
void ADC_SetOffset(uint16_t Offset);
uint16_t ADC_GetOffset(void);
void ADC_ThresholdCheck(FunctionalState NewState);
void ADC_ThresholdConfig(uint32_t ThresholdLow, uint32_t ThresholdHigh);
void ADC_ConversionMode(uint8_t ConvertionMode);
void ADC_SelectFrequencyMic(uint8_t Frequency);
float ADC_GetConvertedData(uint8_t DataType, uint8_t Vref);
uint8_t ADC_GetFlags(void);
FlagStatus ADC_GetFlagStatus(uint8_t ADC_Flag);
ITStatus ADC_GetITStatus(uint8_t ADC_Flag);
void ADC_ITConfig(uint8_t ADC_Flag, FunctionalState NewState);

float ADC_ConvertDifferentialVoltage(int16_t raw_value, uint8_t attenuation);
float ADC_ConvertSingleEndedVoltage(int16_t raw_value, uint8_t channel, uint8_t vRef, uint8_t attenuation);
float ADC_ConvertBatterySensor(int16_t raw_value, uint8_t vRef);
float ADC_ConvertTemperatureSensor(int16_t raw_value, uint8_t vRef, uint8_t attenuation);

uint16_t ADC_GetRawData(void);

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 

#ifdef __cplusplus
}
#endif

#endif /*BLUENRG1_ADC_H */

/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
