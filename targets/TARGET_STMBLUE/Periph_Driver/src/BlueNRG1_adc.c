/**
******************************************************************************
* @file    BlueNRG1_adc.c
* @author  VMA Application Team
  * @version V2.4.0
  * @date    4-May-2018
* @brief   This file provides all the ADC firmware functions.
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

/* Includes ------------------------------------------------------------------*/
#include "../../Periph_Driver/inc/BlueNRG1_adc.h"


/** @addtogroup BLUENRG1_Peripheral_Driver BLUENRG1 Peripheral Driver
  * @{
  */

/** @addtogroup ADC_Peripheral  ADC Peripheral
  * @{
  */


/** @defgroup ADC_Private_Types Private Types
* @{
*/

/**
* @}
*/


/** @defgroup ADC_Private_Constants Private Constants
* @{
*/
#define ADC_VREF                  ((float)(2.4))
#ifdef BLUENRG2_DEVICE
#define ADC_FS_OSR_32_64          ((float)(35442.0))
#define ADC_FS_OSR_100_200        ((float)(41260.0))
#else
#define ADC_FS_OSR_32_64          ((float)(16708.0))
#define ADC_FS_OSR_100_200        ((float)(19450.0))
#endif
#define ADC_KBATT                 ((float)(4.36))
#define ADC_KTEMP                 ((float)(401.0))
#define ADC_CTEMP                 ((float)(267.0))

#define ADC_FS_OSR_32_64_SKIP     ((float)(32768.0))
#define ADC_FS_OSR_100_200_SKIP   ((float)(38147.0))

/**
* @}
*/


/** @defgroup ADC_Private_Macros Private Macros
* @{
*/

/**
* @}
*/

/** @defgroup ADC_Private_Variables Private Variables
* @{
*/

/**
* @}
*/

/** @defgroup ADC_Private_Function Private Functions
* @{
*/

#if (HS_SPEED_XTAL==HS_SPEED_XTAL_32MHZ) && (ADC_2411==1) 

/* x = 1: Workaround for running ADC with 32 MHz clock - Run the APB bus @ 16 MHz;
   x = 0: Restore the APB bus @ 32 MHz */

#define ADC_2411_WORKAROUND(x)   ((x == 1) ? (AHBUPCONV->COMMAND = 0x14) : (AHBUPCONV->COMMAND = 0x15))

#else /* do nothing: no workaround is needed */

#define ADC_2411_WORKAROUND(x) 

#endif



/**
* @brief  Convert raw ADC value in converted differential voltage value.
* @param  raw_value: ADC raw value
* @retval float: converted differential voltage value
*/
float ADC_ConvertDifferentialVoltage(int16_t raw_value, uint8_t attenuation)
{
  float pga, raw_value_f, divider;
  
  if( ADC->CONF_b.SKIP == 1) {
    divider = ADC_FS_OSR_32_64_SKIP;
  }
  else {
    divider = ADC_FS_OSR_32_64;
  }
  
  pga = (float)attenuation;
  raw_value_f = (float)raw_value;
  
  ADC_2411_WORKAROUND(1);
  
  if(ADC->CONF_b.OSR == ADC_OSR_100 || ADC->CONF_b.OSR == ADC_OSR_200) {
    if( ADC->CONF_b.SKIP == 1) {
      divider = ADC_FS_OSR_100_200_SKIP;
    }
    else {
      divider = ADC_FS_OSR_100_200;
    }
  }
  
  ADC_2411_WORKAROUND(0);
  
  return ((1 + pga) * (raw_value_f / divider) * ADC_VREF);
}

/**
* @brief  Convert raw ADC value in converted single ended voltage value.
* @param  raw_value ADC raw value
* @param  channel: the selected single channel type
* @param  vRef: the configured reference voltage
* @retval float: converted single ended voltage value
*/
float ADC_ConvertSingleEndedVoltage(int16_t raw_value, uint8_t channel, uint8_t vRef, uint8_t attenuation)
{
  float pga, raw_value_f, divider;
    
  if( ADC->CONF_b.SKIP == 1) {
    divider = ADC_FS_OSR_32_64_SKIP;
  }
  else {
    divider = ADC_FS_OSR_32_64;
  }
  
  pga = (float)attenuation;
  raw_value_f = (float)raw_value;
  
  ADC_2411_WORKAROUND(1);

  if(ADC->CONF_b.OSR == ADC_OSR_100 || ADC->CONF_b.OSR == ADC_OSR_200) {
    if( ADC->CONF_b.SKIP == 1) {
      divider = ADC_FS_OSR_100_200_SKIP;
    }
    else {
      divider = ADC_FS_OSR_100_200;
    }
  }
  
  ADC_2411_WORKAROUND(0);
  
  if(channel == ADC_Input_AdcPin1) 
    return  ((1 + pga) * (0.6 + ((raw_value_f/divider) * ADC_VREF)));
  else
    return  ((1 + pga) * (0.6 - ((raw_value_f/divider) * ADC_VREF)));
}


/**
* @brief  Convert raw ADC value in battery level.
* @param  raw_value ADC raw value
* @param  vRef: the configured reference voltage
* @retval float: converted battery level
*/
float ADC_ConvertBatterySensor(int16_t raw_value, uint8_t vRef) /* testare al variare di vref */
{
  return (ADC_KBATT * (ADC_ConvertSingleEndedVoltage(raw_value, ADC_Input_AdcPin2, ADC_ReferenceVoltage_0V6, ADC_Attenuation_0dB)));
}

/**
* @brief  Convert raw ADC value in temperature level in Celsius degrees.
* @param  raw_value ADC raw value
* @param  vRef: the configured reference voltage
* @retval float: converted temperature level in Celsius degrees
*/
float ADC_ConvertTemperatureSensor(int16_t raw_value, uint8_t vRef, uint8_t attenuation)
{
  return  ((ADC_KTEMP * (ADC_ConvertSingleEndedVoltage(raw_value, ADC_Input_AdcPin2, ADC_ReferenceVoltage_0V6, ADC_Attenuation_0dB))) - ADC_CTEMP);
}
float ADC_ConvertTemperatureSensorFarenait(int16_t raw_value, uint8_t vRef, uint8_t attenuation)
{
  return  ((722.0 * (ADC_ConvertSingleEndedVoltage(raw_value, ADC_Input_AdcPin2, ADC_ReferenceVoltage_0V6, ADC_Attenuation_0dB))) - 499.0);
}

/**
* @}
*/


/** @defgroup ADC_Public_Functions Public Functions
* @{
*/

/**
* @brief  Deinitializes ADC peripheral registers to their default reset values.
* @param  None
* @retval None
*/
void ADC_DeInit(void)
{
  ADC_2411_WORKAROUND(1);
  
  /* Enable ADC reset state */
  ADC->CTRL_b.RESET = SET;

  ADC_2411_WORKAROUND(0);
}


/**
  * @brief  Fills the ADC_InitStruct with default values.
  * @param  ADC_InitStruct: pointer to an @ref ADC_InitType structure which will be initialized.
  * @retval None
  */
void ADC_StructInit(ADC_InitType* ADC_InitStruct)
{
  /* Set the decimation rate */
  ADC_InitStruct->ADC_OSR = ADC_OSR_200;

  /* Select the input source */
  ADC_InitStruct->ADC_Input = ADC_Input_None;

  /* Set the reference voltage */
  ADC_InitStruct->ADC_ReferenceVoltage = ADC_ReferenceVoltage_0V6;

  /* Set the conversion mode */
  ADC_InitStruct->ADC_ConversionMode = ADC_ConversionMode_Single;

  /* Set the attenuation */
  ADC_InitStruct->ADC_Attenuation = ADC_Attenuation_0dB;
}


/**
* @brief  Initializes the ADC peripheral according to the specified parameters
*         in the ADC_InitStruct.
* @param  ADC_InitStruct: pointer to an @ref ADC_InitType structure that contains 
*         the configuration information for the ADC peripheral.
* @retval None
*/
void ADC_Init(ADC_InitType* ADC_InitStruct)               
{
  /* Check the parameters */
  assert_param(IS_ADC_OSR(ADC_InitStruct->ADC_OSR)); 
  assert_param(IS_ADC_INPUT(ADC_InitStruct->ADC_Input));
  assert_param(IS_ADC_CONVERSIONMODE(ADC_InitStruct->ADC_ConversionMode)); 
  assert_param(IS_ADC_ATTENUATION(ADC_InitStruct->ADC_Attenuation));    
  assert_param(IS_ADC_REFERENCEVOLTAGE(ADC_InitStruct->ADC_ReferenceVoltage)); 

  ADC_2411_WORKAROUND(1);

  /* Set the decimation rate */
  ADC->CONF_b.OSR = ADC_InitStruct->ADC_OSR;
  
  /* Select the input source */
  if(ADC_InitStruct->ADC_Input == ADC_Input_Microphone) {
    ADC->CONF_b.MIC_SEL = SET;
  }
  else {
    ADC->CONF_b.MIC_SEL = RESET;      
    ADC->CONF_b.CHSEL = ADC_InitStruct->ADC_Input;
  }
  
  /* Set the reference voltage */
  ADC->CONF_b.REFSEL = ADC_InitStruct->ADC_ReferenceVoltage;
  
  /* Set the conversion mode */
  if(ADC_InitStruct->ADC_ConversionMode == ADC_ConversionMode_Continuous) {
    ADC->CONF_b.CONT = SET;
  }
  else {
    ADC->CONF_b.CONT = RESET;
  }
  
  /* Set the attenuation */
  ADC->CONF_b.PGASEL = ADC_InitStruct->ADC_Attenuation;

  ADC_2411_WORKAROUND(0);
}


/**
* @brief  Enable disable the ADC conversion.
* @param  NewState: functional state @ref FunctionalState
*         This parameter can be: ENABLE or DISABLE.
* @retval None
*/
void ADC_Cmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState)); 
  
  ADC_2411_WORKAROUND(1);

  if(NewState==ENABLE) {
    if(ADC->CONF_b.MIC_SEL) {
      ADC->CTRL_b.MIC_ON = SET;
    }
    else {
      ADC->CTRL_b.SWSTART = SET;
      ADC->CTRL_b.ON = SET;
    }
  }
  else {
    ADC->CTRL_b.STOP = SET;
  }

  ADC_2411_WORKAROUND(0);
}


/**
* @brief  Enable disable the ADC DMA feature.
* @param  NewState: functional state @ref FunctionalState
*         This parameter can be: ENABLE or DISABLE.
* @retval None
*/
void ADC_DmaCmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState)); 
 
  ADC_2411_WORKAROUND(1);
  
  if(NewState==ENABLE) {
    ADC->CTRL_b.DMA_EN = SET;
  }
  else {
    ADC->CTRL_b.DMA_EN = RESET;
  }

  ADC_2411_WORKAROUND(0);
}

                                                  
/**
* @brief  Specified the ADC input channel.
* @param  adc_input: Specifies the input used for the conversion.
*         This parameter can be a value of @ref ADC_Input.
* @retval None
*/
void ADC_SelectInput(uint8_t AdcInput)               
{
  /* Check the parameter */
  assert_param(IS_ADC_INPUT(AdcInput));
  
  ADC_2411_WORKAROUND(1);

  ADC->CONF_b.CHSEL = AdcInput;
  
  ADC_2411_WORKAROUND(0);
}

/**
* @brief  Enable disable the ADC calibration procedure.
*         If the automatic calibration must be enabled,
*         call the following APIs:
*         ADC_AutoOffsetUpdate(ENABLE);
*         ADC_Calibration(ENABLE);
*         If the automatic calibration must be disabled,
*         call the following APIs:
*         ADC_AutoOffsetUpdate(DISABLE);
*         ADC_Calibration(DISABLE);
*
* @param  NewState: functional state @ref FunctionalState
*         This parameter can be: ENABLE or DISABLE.
* @retval None
*/
void ADC_Calibration(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState)); 
  
  ADC_2411_WORKAROUND(1);

  if(NewState==ENABLE) {
    ADC->CTRL_b.CALEN = SET;
  }
  else {
    ADC->CTRL_b.RSTCALEN = SET;
    ADC->CTRL_b.CALEN = RESET;
  }

  ADC_2411_WORKAROUND(0);
}


/**
* @brief  Enable disable the ADC automatic update of the offset.
*         If the automatic calibration must be enabled,
*         call the following APIs:
*         ADC_AutoOffsetUpdate(ENABLE);
*         ADC_Calibration(ENABLE);
*         If the automatic calibration must be disabled,
*         call the following APIs:
*         ADC_AutoOffsetUpdate(DISABLE);
*         ADC_Calibration(DISABLE);
*
* @param  NewState: functional state @ref FunctionalState
*         This parameter can be: ENABLE or DISABLE.
* @retval None
*/
void ADC_AutoOffsetUpdate(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState)); 
  
  ADC_2411_WORKAROUND(1);

  if(NewState==ENABLE) {
    ADC->CTRL_b.AUTO_OFFSET = SET;
  }
  else {
    ADC->CTRL_b.AUTO_OFFSET = RESET;
  }

  ADC_2411_WORKAROUND(0);
}


/**
* @brief  Enable disable the ADC threshold check.
* @param  NewState: functional state @ref FunctionalState
*         This parameter can be: ENABLE or DISABLE.
* @retval None
*/
void ADC_ThresholdCheck(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState)); 
  
  ADC_2411_WORKAROUND(1);

  if(NewState==ENABLE) {
    ADC->CTRL_b.ENAB_COMP = SET;
  }
  else {
    ADC->CTRL_b.ENAB_COMP = RESET;
  }

  ADC_2411_WORKAROUND(0);
}


/**
* @brief  Configure the ADC threshold.
* @param  ThresholdLow: ADC threshold low value.
*         ThresholdHigh: ADC threshold high value.
* @retval None
*/
void ADC_ThresholdConfig(uint32_t ThresholdLow, uint32_t ThresholdHigh)
{
  ADC_2411_WORKAROUND(1);
  
  ADC->THRESHOLD_LO = ThresholdLow;
  ADC->THRESHOLD_HI = ThresholdHigh;
  
  ADC_2411_WORKAROUND(0);
}

/**
* @brief  Get the ADC offset value previously calculated
*         internally with the auto calibration.
* @param  None
* @retval uint16_t: Offset for correction of converter data.
*/
uint16_t ADC_GetOffset(void)
{
  uint16_t value;

  ADC_2411_WORKAROUND(1);

  /* Conversion done with filter bypassed */
  if(ADC->CONF_b.SKIP == 0) {
    value = READ_REG(ADC->OFFSET_MSB);
  }
  /* Conversion done with filter not bypassed */
  else {
    value = READ_REG(ADC->OFFSET_LSB);
  }

  ADC_2411_WORKAROUND(0);

  return value;
}


/**
* @brief  Set the ADC offset value previously calculated
*         internally with the auto calibration.
*         The AUTO_OFFSET will perform this operation automatically.
* @param  int32_t: Offset for correction of converter data.
* @retval None
*/
void ADC_SetOffset(uint16_t Offset)
{
  ADC_2411_WORKAROUND(1);

  /* Conversion done with filter bypassed */
  if(ADC->CONF_b.SKIP == 0) {
    ADC->OFFSET_MSB = Offset;
  }
  /* Conversion done with filter not bypassed */
  else {
    ADC->OFFSET_LSB = Offset;
  }

  ADC_2411_WORKAROUND(0);
}

/**
* @brief  Configure the ADC conversion mode.
* @param  cConvertionMode: the value can be
*         @arg ADC_ConversionMode_Single single shot ADC conversion
*         @arg ADC_ConversionMode_Continuous continuous ADC conversion
* @retval None
*/
void ADC_ConversionMode(uint8_t ConvertionMode)
{
  /* Check the parameters */
  assert_param(IS_ADC_CONVERSIONMODE(ConvertionMode));
  
  ADC_2411_WORKAROUND(1);

  if(ConvertionMode==ADC_ConversionMode_Continuous) {
    ADC->CONF_b.CONT = SET;
  }
  else {
    ADC->CONF_b.CONT = RESET;
  }

  ADC_2411_WORKAROUND(0);

}

/**
* @brief  Select the frequency of the clock signal for an external MEMS microphone (through IO0)
* @param  Frequency: the value can be 
*         @arg ADC_MIC_800KH Enable the 800 kHz through IO0 for external MEMS microphone
*         @arg ADC_MIC_1600KH Enable the 1.6 MHz through IO0 for external MEMS microphone
* @retval None
*/
void ADC_SelectFrequencyMic(uint8_t Frequency)
{
  /* Check the parameters */
  assert_param(IS_ADC_MIC_FREQ_SEL(Frequency));
  
  ADC_2411_WORKAROUND(1);

  /* Check the value of Frequency */
  if(Frequency == ADC_MIC_800KHZ) {
    ADC->CONF_b.DIG_FILT_CLK = RESET;
  }
  else if(Frequency == ADC_MIC_1600KHZ) {
    ADC->CONF_b.DIG_FILT_CLK = SET;
  }

  ADC_2411_WORKAROUND(0);

}


/**
* @brief  Get the ADC flags.
*         The read operation will clear the flags.
* @param  none
* @retval uint8_t: the ADC flags as bitmask 0x0000b3b2b1b0:
*         b3: ADC_FLAG_WDG
*         b2: ADC_FLAG_EOC
*         b1: ADC_FLAG_BUSY
*         b0: ADC_FLAG_CAL.
*/
uint8_t ADC_GetFlags(void)
{
  uint8_t flags;
  
  ADC_2411_WORKAROUND(1);
  
  flags = (ADC->IRQRAW & 0x0F);
  
  ADC_2411_WORKAROUND(0);
  
  return flags;  
}

/**
* @brief  Get the status flag of ADC.
*         The read operation will clear the flags.
*         If more than one status flag must be read,
*         then the function must be ADC_GetFlags().
* @param  ADC_Flag: the value can be
*         @arg ADC_FLAG_CAL ADC End of Calibration flag
*         @arg ADC_FLAG_BUSY ADC busy flag
*         @arg ADC_FLAG_EOC ADC End of Conversion flag
*         @arg ADC_FLAG_WDG ADC ADV value within set threshold 
* @retval FlagStatus: functional state @ref FlagStatus
*         This parameter can be: SET or RESET.
*/
FlagStatus ADC_GetFlagStatus(uint8_t ADC_Flag)
{
  FlagStatus status;

  /* Check the parameters */
  assert_param(IS_ADC_GET_FLAG(ADC_Flag));
  
  ADC_2411_WORKAROUND(1);
  
  /* Check the status of the specified SPI flag */
  if (READ_BIT(ADC->IRQRAW, ADC_Flag) != (uint16_t)RESET) {
    /* SPI_FLAG is set */
    status = SET;
  }
  else {
    /* SPI_FLAG is reset */
    status = RESET;
  }

  ADC_2411_WORKAROUND(0);

  return status;  
}

/**
* @brief  Get the status of the masked IT flag.
*         The read operation will clear the flags.
*         If more than one status flag must be read,
*         then the function must be ADC_GetFlags().
* @param  ADC_Flag: the value can be 
*         @arg ADC_FLAG_CAL ADC End of Calibration flag
*         @arg ADC_FLAG_BUSY ADC busy flag
*         @arg ADC_FLAG_EOC ADC End of Conversion flag
*         @arg ADC_FLAG_WDG ADC ADV value within set threshold 
* @retval  ITStatus: functional state @ref ITStatus
*         This parameter can be: SET or RESET.
*/
ITStatus ADC_GetITStatus(uint8_t ADC_Flag)
{
  ITStatus status;

  /* Check the parameters */
  assert_param(IS_ADC_GET_FLAG(ADC_Flag));
  
  ADC_2411_WORKAROUND(1);

  /* Check the status of the specified SPI interrupt */
  if (READ_BIT(ADC->IRQSTAT, ADC_Flag) != (uint16_t)RESET) {
    /* ADC_Flag is set */
    status = SET;
  }
  else {
    /* SPI_IT is reset */
    status = RESET;
  }

  ADC_2411_WORKAROUND(0);

  return status;
}



/**
* @brief  Enable disable the ADC IT flag.
* @param  ADC_Flag: the value can be 
*         @arg ADC_FLAG_CAL ADC End of Calibration flag
*         @arg ADC_FLAG_BUSY ADC busy flag
*         @arg ADC_FLAG_EOC ADC End of Conversion flag
*         @arg ADC_FLAG_WDG ADC ADV value within set threshold 
* @param  NewState: functional state @ref FunctionalState
*         This parameter can be: ENABLE or DISABLE.
* @retval None
*/
void ADC_ITConfig(uint8_t ADC_Flag, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  assert_param(IS_ADC_GET_FLAG(ADC_Flag));
  
  ADC_2411_WORKAROUND(1);

  if (NewState != DISABLE) {
    /* Enable the selected SPI interrupts */
    CLEAR_BIT(ADC->IRQMASK, ADC_Flag);
  }
  else {
    /* Disable the selected SPI interrupts */
    SET_BIT(ADC->IRQMASK, ADC_Flag);
  }

  ADC_2411_WORKAROUND(0);
}

/**
* @brief  Get the ADC converted value in Volt.
* @param  dataType: the selected channel, this value can be
*         @arg ADC_Input_AdcPin1 data from ADC pin1
*         @arg ADC_Input_AdcPin2 data from ADC pin2
*         @arg ADC_Input_AdcPin12 data from ADC pin12
*         @arg ADC_Input_Internal0V60V6 data from internal 0.6V
*         @arg ADC_Input_Internal1V20V0 data from internal 1.2V
*         @arg ADC_Input_BattSensor data from battery sensor
*         @arg ADC_Input_TempSensor data from temperature sensor
* @param  vRef: voltage reference configured, the value can be
*         @arg ADC_ReferenceVoltage_0V6 Vref is 0.6 V
* @retval Converted ADC value in Volt
*/
float ADC_GetConvertedData(uint8_t DataType, uint8_t Vref)
{
  int16_t raw_value;
  uint8_t pga_reg;
  
  /* Check the parameters */
  assert_param(IS_ADC_INPUT(DataType));
  assert_param(IS_ADC_REFERENCEVOLTAGE(Vref)); 

  ADC_2411_WORKAROUND(1);

  raw_value = (int16_t)ADC_GetRawData();
  pga_reg = ADC->CONF_b.PGASEL;

  ADC_2411_WORKAROUND(0);

  if(DataType==ADC_Input_AdcPin1) {
    return ADC_ConvertSingleEndedVoltage(raw_value, (uint8_t)ADC_Input_AdcPin1, Vref, pga_reg);
  }
  else if(DataType==ADC_Input_AdcPin2) {
    return ADC_ConvertSingleEndedVoltage(raw_value, (uint8_t)ADC_Input_AdcPin2, Vref, pga_reg);
  }
  else if(DataType==ADC_Input_AdcPin12 || DataType==ADC_Input_Internal0V60V6 || DataType==ADC_Input_Internal1V20V0) {
    return ADC_ConvertDifferentialVoltage(raw_value, pga_reg);
  }
  else if(DataType==ADC_Input_BattSensor) {
    return ADC_ConvertBatterySensor(raw_value, Vref);
  }
  else if(DataType==ADC_Input_TempSensor) {
    return ADC_ConvertTemperatureSensor(raw_value, Vref, pga_reg);
  }
  return raw_value;
}


/**
* @brief  Get the ADC raw value.
* @param  None
* @retval ADC raw value
*/
uint16_t ADC_GetRawData(void)
{
  uint16_t value;
  
  ADC_2411_WORKAROUND(1);
  
  /* Conversion done with filter bypassed */
  if(ADC->CONF_b.SKIP == 0) {
    value = READ_REG(ADC->DATA_CONV_MSB);
  }
  /* Conversion done with filter not bypassed */
  else {
    value = READ_REG(ADC->DATA_CONV_LSB);
  }
    
  ADC_2411_WORKAROUND(0);
  
  return value;
}


/**
* @brief  Enable/disable the COMP filter.
*         This operation could be useful to speed up the conversion
*         for signal at low frequency.
* @param  NewState: functional state @ref FunctionalState
*         This parameter can be: ENABLE or DISABLE.
* @retval None
*/
void ADC_Filter(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState)); 
  
  ADC_2411_WORKAROUND(1);

  /* If ENABLE the filter, then reset the SKIP bifield */
  if(NewState==ENABLE) {
    ADC->CONF_b.SKIP = RESET;
  }

  /* If DISABLE the filter, then set the SKIP bifield */
  else {
    ADC->CONF_b.SKIP = SET;
  }

  ADC_2411_WORKAROUND(0);
}


/**
* @}
*/ 

/**
* @}
*/ 

/**
* @}
*/ 

/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
