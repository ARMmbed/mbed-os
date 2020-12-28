/**
  ******************************************************************************
  * @file    stm32l0xx_hal_adc.c
  * @author  MCD Application Team
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the Analog to Digital Convertor (ADC)
  *          peripheral:
  *           + Initialization and de-initialization functions
  *             ++ Initialization and Configuration of ADC
  *           + Operation functions
  *             ++ Start, stop, get result of conversions of regular
  *                group, using 3 possible modes: polling, interruption or DMA.
  *           + Control functions
  *             ++ Channels configuration on regular group
  *             ++ Analog Watchdog configuration
  *           + State functions
  *             ++ ADC state machine management
  *             ++ Interrupts and flags management
  *          Other functions (extended functions) are available in file 
  *          "stm32l0xx_hal_adc_ex.c".
  *
  @verbatim
  ==============================================================================
                     ##### ADC peripheral features #####
  ==============================================================================
  [..]
  (+) 12-bit, 10-bit, 8-bit or 6-bit configurable resolution.

  (+) Interrupt generation at the end of regular conversion and in case of 
      analog watchdog or overrun events.
  
  (+) Single and continuous conversion modes.
  
  (+) Scan mode for conversion of several channels sequentially.
  
  (+) Data alignment with in-built data coherency.
  
  (+) Programmable sampling time (common for all channels)
  
  (+) External trigger (timer or EXTI) with configurable polarity
  
  (+) DMA request generation for transfer of conversions data of regular group.

  (+) ADC calibration
  
  (+) ADC conversion of regular group.
  
  (+) ADC supply requirements: 1.62 V to 3.6 V.
  
  (+) ADC input range: from Vref- (connected to Vssa) to Vref+ (connected to 
      Vdda or to an external voltage reference).


                     ##### How to use this driver #####
  ==============================================================================
    [..]

     *** Configuration of top level parameters related to ADC ***
     ============================================================
     [..]

    (#) Enable the ADC interface
        (++) As prerequisite, ADC clock must be configured at RCC top level.
             Caution: On STM32L0, ADC clock frequency max is 16MHz (refer
                      to device datasheet).
                      Therefore, ADC clock prescaler must be configured in 
                      function of ADC clock source frequency to remain below
                      this maximum frequency.

        (++) Two clock settings are mandatory: 
             (+++) ADC clock (core clock, also possibly conversion clock).

             (+++) ADC clock (conversions clock).
                   Two possible clock sources: synchronous clock derived from APB clock
                   or asynchronous clock derived from ADC dedicated HSI RC oscillator
                   16MHz.
                   If asynchronous clock is selected, parameter "HSIState" must be set either:
                   - to "...HSIState = RCC_HSI_ON" to maintain the HSI16 oscillator
                     always enabled: can be used to supply the main system clock.

             (+++) Example:
                   Into HAL_ADC_MspInit() (recommended code location) or with
                   other device clock parameters configuration:
               (+++) __HAL_RCC_ADC1_CLK_ENABLE();                 (mandatory)

               HSI enable                                         (optional: if asynchronous clock selected)
               (+++) RCC_OscInitTypeDef   RCC_OscInitStructure;
               (+++) RCC_OscInitStructure.OscillatorType = RCC_OSCILLATORTYPE_HSI;
               (+++) RCC_OscInitStructure.HSI16CalibrationValue = RCC_HSICALIBRATION_DEFAULT;
               (+++) RCC_OscInitStructure.HSIState = RCC_HSI_ON;
               (+++) RCC_OscInitStructure.PLL...   (optional if used for system clock)
               (+++) HAL_RCC_OscConfig(&RCC_OscInitStructure);

        (++) ADC clock source and clock prescaler are configured at ADC level with
             parameter "ClockPrescaler" using function HAL_ADC_Init().

    (#) ADC pins configuration
         (++) Enable the clock for the ADC GPIOs
              using macro __HAL_RCC_GPIOx_CLK_ENABLE()
         (++) Configure these ADC pins in analog mode
              using function HAL_GPIO_Init()

    (#) Optionally, in case of usage of ADC with interruptions:
         (++) Configure the NVIC for ADC
              using function HAL_NVIC_EnableIRQ(ADCx_IRQn)
         (++) Insert the ADC interruption handler function HAL_ADC_IRQHandler() 
              into the function of corresponding ADC interruption vector 
              ADCx_IRQHandler().

    (#) Optionally, in case of usage of DMA:
         (++) Configure the DMA (DMA channel, mode normal or circular, ...)
              using function HAL_DMA_Init().
         (++) Configure the NVIC for DMA
              using function HAL_NVIC_EnableIRQ(DMAx_Channelx_IRQn)
         (++) Insert the ADC interruption handler function HAL_ADC_IRQHandler() 
              into the function of corresponding DMA interruption vector 
              DMAx_Channelx_IRQHandler().

     *** Configuration of ADC, group regular, channels parameters ***
     ================================================================
     [..]

    (#) Configure the ADC parameters (resolution, data alignment, ...)
        and regular group parameters (conversion trigger, sequencer, ...)
        using function HAL_ADC_Init().

    (#) Configure the channels for regular group parameters (channel number, 
        channel rank into sequencer, ..., into regular group)
        using function HAL_ADC_ConfigChannel().

    (#) Optionally, configure the analog watchdog parameters (channels
        monitored, thresholds, ...)
        using function HAL_ADC_AnalogWDGConfig().


    (#) When device is in mode low-power (low-power run, low-power sleep or stop mode), 
        function "HAL_ADCEx_EnableVREFINT()" must be called before function HAL_ADC_Init().
        In case of internal temperature sensor to be measured:
        function "HAL_ADCEx_EnableVREFINTTempSensor()" must be called similarilly

     *** Execution of ADC conversions ***
     ====================================
     [..]

    (#) Optionally, perform an automatic ADC calibration to improve the
        conversion accuracy
        using function HAL_ADCEx_Calibration_Start().

    (#) ADC driver can be used among three modes: polling, interruption,
        transfer by DMA.

        (++) ADC conversion by polling:
          (+++) Activate the ADC peripheral and start conversions
                using function HAL_ADC_Start()
          (+++) Wait for ADC conversion completion 
                using function HAL_ADC_PollForConversion()
          (+++) Retrieve conversion results 
                using function HAL_ADC_GetValue()
          (+++) Stop conversion and disable the ADC peripheral 
                using function HAL_ADC_Stop()

        (++) ADC conversion by interruption: 
          (+++) Activate the ADC peripheral and start conversions
                using function HAL_ADC_Start_IT()
          (+++) Wait for ADC conversion completion by call of function
                HAL_ADC_ConvCpltCallback()
                (this function must be implemented in user program)
          (+++) Retrieve conversion results 
                using function HAL_ADC_GetValue()
          (+++) Stop conversion and disable the ADC peripheral 
                using function HAL_ADC_Stop_IT()

        (++) ADC conversion with transfer by DMA:
          (+++) Activate the ADC peripheral and start conversions
                using function HAL_ADC_Start_DMA()
          (+++) Wait for ADC conversion completion by call of function
                HAL_ADC_ConvCpltCallback() or HAL_ADC_ConvHalfCpltCallback()
                (these functions must be implemented in user program)
          (+++) Conversion results are automatically transferred by DMA into
                destination variable address.
          (+++) Stop conversion and disable the ADC peripheral 
                using function HAL_ADC_Stop_DMA()

     [..]

    (@) Callback functions must be implemented in user program:
      (+@) HAL_ADC_ErrorCallback()
      (+@) HAL_ADC_LevelOutOfWindowCallback() (callback of analog watchdog)
      (+@) HAL_ADC_ConvCpltCallback()
      (+@) HAL_ADC_ConvHalfCpltCallback

     *** Deinitialization of ADC ***
     ============================================================
     [..]

    (#) Disable the ADC interface
      (++) ADC clock can be hard reset and disabled at RCC top level.
        (++) Hard reset of ADC peripherals
             using macro __ADCx_FORCE_RESET(), __ADCx_RELEASE_RESET().
        (++) ADC clock disable
             using the equivalent macro/functions as configuration step.
             (+++) Example:
                   Into HAL_ADC_MspDeInit() (recommended code location) or with
                   other device clock parameters configuration:
               (+++) RCC_OscInitStructure.OscillatorType = RCC_OSCILLATORTYPE_HSI;
               (+++) RCC_OscInitStructure.HSIState = RCC_HSI_OFF; (if not used for system clock)
               (+++) HAL_RCC_OscConfig(&RCC_OscInitStructure);

    (#) ADC pins configuration
         (++) Disable the clock for the ADC GPIOs
              using macro __HAL_RCC_GPIOx_CLK_DISABLE()

    (#) Optionally, in case of usage of ADC with interruptions:
         (++) Disable the NVIC for ADC
              using function HAL_NVIC_EnableIRQ(ADCx_IRQn)

    (#) Optionally, in case of usage of DMA:
         (++) Deinitialize the DMA
              using function HAL_DMA_Init().
         (++) Disable the NVIC for DMA
              using function HAL_NVIC_EnableIRQ(DMAx_Channelx_IRQn)

    [..]

    *** Callback registration ***
    =============================================
    [..]

     The compilation flag USE_HAL_ADC_REGISTER_CALLBACKS, when set to 1,
     allows the user to configure dynamically the driver callbacks.
     Use Functions @ref HAL_ADC_RegisterCallback()
     to register an interrupt callback.
    [..]

     Function @ref HAL_ADC_RegisterCallback() allows to register following callbacks:
       (+) ConvCpltCallback               : ADC conversion complete callback
       (+) ConvHalfCpltCallback           : ADC conversion DMA half-transfer callback
       (+) LevelOutOfWindowCallback       : ADC analog watchdog 1 callback
       (+) ErrorCallback                  : ADC error callback
       (+) MspInitCallback                : ADC Msp Init callback
       (+) MspDeInitCallback              : ADC Msp DeInit callback
     This function takes as parameters the HAL peripheral handle, the Callback ID
     and a pointer to the user callback function.
    [..]

     Use function @ref HAL_ADC_UnRegisterCallback to reset a callback to the default
     weak function.
    [..]

     @ref HAL_ADC_UnRegisterCallback takes as parameters the HAL peripheral handle,
     and the Callback ID.
     This function allows to reset following callbacks:
       (+) ConvCpltCallback               : ADC conversion complete callback
       (+) ConvHalfCpltCallback           : ADC conversion DMA half-transfer callback
       (+) LevelOutOfWindowCallback       : ADC analog watchdog 1 callback
       (+) ErrorCallback                  : ADC error callback
       (+) MspInitCallback                : ADC Msp Init callback
       (+) MspDeInitCallback              : ADC Msp DeInit callback
     [..]

     By default, after the @ref HAL_ADC_Init() and when the state is @ref HAL_ADC_STATE_RESET
     all callbacks are set to the corresponding weak functions:
     examples @ref HAL_ADC_ConvCpltCallback(), @ref HAL_ADC_ErrorCallback().
     Exception done for MspInit and MspDeInit functions that are
     reset to the legacy weak functions in the @ref HAL_ADC_Init()/ @ref HAL_ADC_DeInit() only when
     these callbacks are null (not registered beforehand).
    [..]

     If MspInit or MspDeInit are not null, the @ref HAL_ADC_Init()/ @ref HAL_ADC_DeInit()
     keep and use the user MspInit/MspDeInit callbacks (registered beforehand) whatever the state.
     [..]

     Callbacks can be registered/unregistered in @ref HAL_ADC_STATE_READY state only.
     Exception done MspInit/MspDeInit functions that can be registered/unregistered
     in @ref HAL_ADC_STATE_READY or @ref HAL_ADC_STATE_RESET state,
     thus registered (user) MspInit/DeInit callbacks can be used during the Init/DeInit.
    [..]

     Then, the user first registers the MspInit/MspDeInit user callbacks
     using @ref HAL_ADC_RegisterCallback() before calling @ref HAL_ADC_DeInit()
     or @ref HAL_ADC_Init() function.
     [..]

     When the compilation flag USE_HAL_ADC_REGISTER_CALLBACKS is set to 0 or
     not defined, the callback registration feature is not available and all callbacks
     are set to the corresponding weak functions.
  
    @endverbatim
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright(c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************  
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"

/** @addtogroup STM32L0xx_HAL_Driver
  * @{
  */

/** @defgroup ADC ADC
  * @brief ADC HAL module driver
  * @{
  */

#ifdef HAL_ADC_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/** @defgroup ADC_Private_Constants ADC Private Constants
  * @{
  */

/* Delay for ADC stabilization time.                                          */
/* Maximum delay is 1us (refer to device datasheet, parameter tSTART). */
/* Unit: us */
#define ADC_STAB_DELAY_US       ((uint32_t) 1U)

/* Delay for temperature sensor stabilization time. */
/* Unit: us */
#define ADC_TEMPSENSOR_DELAY_US ((uint32_t) 20000U)
/**
  * @}
  */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/** @defgroup ADC_Private_Functions ADC Private Functions
  * @{
  */
static HAL_StatusTypeDef ADC_Enable(ADC_HandleTypeDef* hadc);
static HAL_StatusTypeDef ADC_Disable(ADC_HandleTypeDef* hadc);
static HAL_StatusTypeDef ADC_ConversionStop(ADC_HandleTypeDef* hadc);
static void ADC_DMAConvCplt(DMA_HandleTypeDef *hdma);
static void ADC_DMAHalfConvCplt(DMA_HandleTypeDef *hdma);
static void ADC_DMAError(DMA_HandleTypeDef *hdma);
static void ADC_DelayMicroSecond(uint32_t microSecond);
/**
  * @}
  */

/* Exported functions ---------------------------------------------------------*/

/** @defgroup ADC_Exported_Functions ADC Exported Functions
  * @{
  */

/** @defgroup ADC_Exported_Functions_Group1 Initialization and de-initialization functions
  * @brief    ADC Initialization and Configuration functions
  *
@verbatim    
 ===============================================================================
              ##### Initialization and de-initialization functions #####
 ===============================================================================
    [..]  This section provides functions allowing to:
      (+) Initialize and configure the ADC. 
      (+) De-initialize the ADC.
@endverbatim
  * @{
  */

/**
  * @brief  Initialize the ADC peripheral and regular group according to  
  *         parameters specified in structure "ADC_InitTypeDef".
  * @note   As prerequisite, ADC clock must be configured at RCC top level
  *         depending on possible clock sources: APB clock of HSI clock.
  *         See commented example code below that can be copied and uncommented
  *         into HAL_ADC_MspInit().
  * @note   Possibility to update parameters on the fly:
  *         This function initializes the ADC MSP (HAL_ADC_MspInit()) only when
  *         coming from ADC state reset. Following calls to this function can
  *         be used to reconfigure some parameters of ADC_InitTypeDef  
  *         structure on the fly, without modifying MSP configuration. If ADC  
  *         MSP has to be modified again, HAL_ADC_DeInit() must be called
  *         before HAL_ADC_Init().
  *         The setting of these parameters is conditioned to ADC state.
  *         For parameters constraints, see comments of structure 
  *         "ADC_InitTypeDef".
  * @note   This function configures the ADC within 2 scopes: scope of entire 
  *         ADC and scope of regular group. For parameters details, see comments 
  *         of structure "ADC_InitTypeDef".
  * @note   When device is in mode low-power (low-power run, low-power sleep or stop mode), 
  *         function "HAL_ADCEx_EnableVREFINT()" must be called before function HAL_ADC_Init() 
  *         (in case of previous ADC operations: function HAL_ADC_DeInit() must be called first).
  *         In case of internal temperature sensor to be measured:
  *         function "HAL_ADCEx_EnableVREFINTTempSensor()" must be called similarilly.  
  * @param  hadc ADC handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ADC_Init(ADC_HandleTypeDef* hadc)
{
 
  /* Check ADC handle */
  if(hadc == NULL)
  {
    return HAL_ERROR;
  }
  
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  assert_param(IS_ADC_CLOCKPRESCALER(hadc->Init.ClockPrescaler));
  assert_param(IS_ADC_RESOLUTION(hadc->Init.Resolution));
  assert_param(IS_ADC_DATA_ALIGN(hadc->Init.DataAlign));
  assert_param(IS_ADC_SCAN_MODE(hadc->Init.ScanConvMode));
  assert_param(IS_FUNCTIONAL_STATE(hadc->Init.ContinuousConvMode));
  assert_param(IS_FUNCTIONAL_STATE(hadc->Init.DiscontinuousConvMode));
  assert_param(IS_ADC_EXTTRIG_EDGE(hadc->Init.ExternalTrigConvEdge));
  assert_param(IS_ADC_EXTTRIG(hadc->Init.ExternalTrigConv));
  assert_param(IS_FUNCTIONAL_STATE(hadc->Init.DMAContinuousRequests));
  assert_param(IS_ADC_EOC_SELECTION(hadc->Init.EOCSelection));
  assert_param(IS_ADC_OVERRUN(hadc->Init.Overrun));
  assert_param(IS_FUNCTIONAL_STATE(hadc->Init.LowPowerAutoWait));
  assert_param(IS_FUNCTIONAL_STATE(hadc->Init.LowPowerFrequencyMode));
  assert_param(IS_FUNCTIONAL_STATE(hadc->Init.LowPowerAutoPowerOff));
  assert_param(IS_ADC_SAMPLE_TIME(hadc->Init.SamplingTime));
  assert_param(IS_FUNCTIONAL_STATE(hadc->Init.OversamplingMode));

  /* As prerequisite, into HAL_ADC_MspInit(), ADC clock must be configured    */
  /* at RCC top level depending on both possible clock sources:               */
  /* APB clock or HSI clock.                                                  */
  /* Refer to header of this file for more details on clock enabling procedure*/
  
  /* Actions performed only if ADC is coming from state reset:                */
  /* - Initialization of ADC MSP                                              */
  /* - ADC voltage regulator enable                                           */
  if(hadc->State == HAL_ADC_STATE_RESET)
  {
    /* Initialize ADC error code */
    ADC_CLEAR_ERRORCODE(hadc);
    
    /* Allocate lock resource and initialize it */
    hadc->Lock = HAL_UNLOCKED;
    
#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
    /* Init the ADC Callback settings */
    hadc->ConvCpltCallback              = HAL_ADC_ConvCpltCallback;                 /* Legacy weak callback */
    hadc->ConvHalfCpltCallback          = HAL_ADC_ConvHalfCpltCallback;             /* Legacy weak callback */
    hadc->LevelOutOfWindowCallback      = HAL_ADC_LevelOutOfWindowCallback;         /* Legacy weak callback */
    hadc->ErrorCallback                 = HAL_ADC_ErrorCallback;                    /* Legacy weak callback */
    
    if (hadc->MspInitCallback == NULL)
    {
      hadc->MspInitCallback = HAL_ADC_MspInit; /* Legacy weak MspInit  */
    }
    
    /* Init the low level hardware */
    hadc->MspInitCallback(hadc);
#else
    /* Init the low level hardware */
    HAL_ADC_MspInit(hadc);
#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */
  }
  
  /* Configuration of ADC parameters if previous preliminary actions are      */ 
  /* correctly completed.                                                     */
  /* and if there is no conversion on going on regular group (ADC can be      */
  /* enabled anyway, in case of call of this function to update a parameter   */
  /* on the fly).                                                             */
  if (HAL_IS_BIT_SET(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL) ||
     (ADC_IS_CONVERSION_ONGOING_REGULAR(hadc) != RESET)  )
  {
    /* Update ADC state machine to error */
    SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL);
        
    /* Process unlocked */
    __HAL_UNLOCK(hadc);
    return HAL_ERROR;
  }

  /* Set ADC state */
  ADC_STATE_CLR_SET(hadc->State,
                    HAL_ADC_STATE_REG_BUSY,
                    HAL_ADC_STATE_BUSY_INTERNAL);

  /* Parameters update conditioned to ADC state:                            */
  /* Parameters that can be updated only when ADC is disabled:              */
  /*  - ADC clock mode                                                      */
  /*  - ADC clock prescaler                                                 */
  /*  - ADC Resolution                                                      */
  if (ADC_IS_ENABLE(hadc) == RESET)
  {
    /* Some parameters of this register are not reset, since they are set   */
    /* by other functions and must be kept in case of usage of this         */
    /* function on the fly (update of a parameter of ADC_InitTypeDef        */
    /* without needing to reconfigure all other ADC groups/channels         */
    /* parameters):                                                         */
    /*   - internal measurement paths: Vbat, temperature sensor, Vref       */
    /*     (set into HAL_ADC_ConfigChannel() )                              */
   
    /* Configuration of ADC clock: clock source PCLK or asynchronous with 
    selectable prescaler */
    __HAL_ADC_CLOCK_PRESCALER(hadc);
    
    /* Configuration of ADC:                                                */
    /*  - Resolution                                                        */
    hadc->Instance->CFGR1 &= ~( ADC_CFGR1_RES);
    hadc->Instance->CFGR1 |= hadc->Init.Resolution;    
  }
  
  /* Set the Low Frequency mode */
  ADC->CCR &= (uint32_t)~ADC_CCR_LFMEN;
  ADC->CCR |=__HAL_ADC_CCR_LOWFREQUENCY(hadc->Init.LowPowerFrequencyMode);  
   
  /* Enable voltage regulator (if disabled at this step) */
  if (HAL_IS_BIT_CLR(hadc->Instance->CR, ADC_CR_ADVREGEN))
  {
    /* Set ADVREGEN bit */
    hadc->Instance->CR |= ADC_CR_ADVREGEN;
  }
  
  /* Configuration of ADC:                                                    */
  /*  - Resolution                                                            */
  /*  - Data alignment                                                        */
  /*  - Scan direction                                                        */
  /*  - External trigger to start conversion                                  */
  /*  - External trigger polarity                                             */
  /*  - Continuous conversion mode                                            */
  /*  - DMA continuous request                                                */
  /*  - Overrun                                                               */
  /*  - AutoDelay feature                                                     */
  /*  - Discontinuous mode                                                    */
  hadc->Instance->CFGR1 &= ~(ADC_CFGR1_ALIGN   |
                             ADC_CFGR1_SCANDIR |
                             ADC_CFGR1_EXTSEL  |
                             ADC_CFGR1_EXTEN   |
                             ADC_CFGR1_CONT    |
                             ADC_CFGR1_DMACFG  |
                             ADC_CFGR1_OVRMOD  |
                             ADC_CFGR1_AUTDLY  |
                             ADC_CFGR1_AUTOFF  |
                             ADC_CFGR1_DISCEN   );
  
  hadc->Instance->CFGR1 |= (hadc->Init.DataAlign                             |
                            ADC_SCANDIR(hadc->Init.ScanConvMode)             |
                            ADC_CONTINUOUS((uint32_t)hadc->Init.ContinuousConvMode)    |
                            ADC_DMACONTREQ((uint32_t)hadc->Init.DMAContinuousRequests) |
                            hadc->Init.Overrun                               |
                            __HAL_ADC_CFGR1_AutoDelay(hadc->Init.LowPowerAutoWait) |
                            __HAL_ADC_CFGR1_AUTOFF(hadc->Init.LowPowerAutoPowerOff));
  
  /* Enable external trigger if trigger selection is different of software  */
  /* start.                                                                 */
  /* Note: This configuration keeps the hardware feature of parameter       */
  /*       ExternalTrigConvEdge "trigger edge none" equivalent to           */
  /*       software start.                                                  */
  if (hadc->Init.ExternalTrigConv != ADC_SOFTWARE_START)
  {
    hadc->Instance->CFGR1 |= hadc->Init.ExternalTrigConv |
                             hadc->Init.ExternalTrigConvEdge;
  }
  
  /* Enable discontinuous mode only if continuous mode is disabled */
  if (hadc->Init.DiscontinuousConvMode == ENABLE)
  {
    if (hadc->Init.ContinuousConvMode == DISABLE)
    {
      /* Enable the selected ADC group regular discontinuous mode */
      hadc->Instance->CFGR1 |= (ADC_CFGR1_DISCEN);
    }
    else
    {
      /* ADC regular group discontinuous was intended to be enabled,        */
      /* but ADC regular group modes continuous and sequencer discontinuous */
      /* cannot be enabled simultaneously.                                  */
      
      /* Update ADC state machine to error */
      SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_CONFIG);
      
      /* Set ADC error code to ADC peripheral internal error */
      SET_BIT(hadc->ErrorCode, HAL_ADC_ERROR_INTERNAL);
    }
  }
  
  if (hadc->Init.OversamplingMode == ENABLE)
  {
    assert_param(IS_ADC_OVERSAMPLING_RATIO(hadc->Init.Oversample.Ratio));
    assert_param(IS_ADC_RIGHT_BIT_SHIFT(hadc->Init.Oversample.RightBitShift));
    assert_param(IS_ADC_TRIGGERED_OVERSAMPLING_MODE(hadc->Init.Oversample.TriggeredMode));

    /* Configuration of Oversampler:                                          */
    /*  - Oversampling Ratio                                                  */
    /*  - Right bit shift                                                     */
    /*  - Triggered mode                                                      */
    
    hadc->Instance->CFGR2 &= ~( ADC_CFGR2_OVSR |
                                ADC_CFGR2_OVSS |
                                ADC_CFGR2_TOVS );
    
    hadc->Instance->CFGR2 |= ( hadc->Init.Oversample.Ratio         |
                               hadc->Init.Oversample.RightBitShift             |
                               hadc->Init.Oversample.TriggeredMode );
    
    /* Enable OverSampling mode */
     hadc->Instance->CFGR2 |= ADC_CFGR2_OVSE;
  }
  else
  {
    if(HAL_IS_BIT_SET(hadc->Instance->CFGR2, ADC_CFGR2_OVSE))
    {
      /* Disable OverSampling mode if needed */
      hadc->Instance->CFGR2 &= ~ADC_CFGR2_OVSE;
    }
  }    
  
  /* Clear the old sampling time */
  hadc->Instance->SMPR &= (uint32_t)(~ADC_SMPR_SMPR);
  
  /* Set the new sample time */
  hadc->Instance->SMPR |= hadc->Init.SamplingTime;
  
  /* Clear ADC error code */
  ADC_CLEAR_ERRORCODE(hadc);

  /* Set the ADC state */
  ADC_STATE_CLR_SET(hadc->State,
                    HAL_ADC_STATE_BUSY_INTERNAL,
                    HAL_ADC_STATE_READY);


  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Deinitialize the ADC peripheral registers to their default reset
  *         values, with deinitialization of the ADC MSP.
  * @note   For devices with several ADCs: reset of ADC common registers is done 
  *         only if all ADCs sharing the same common group are disabled.
  *         If this is not the case, reset of these common parameters reset is  
  *         bypassed without error reporting: it can be the intended behavior in
  *         case of reset of a single ADC while the other ADCs sharing the same 
  *         common group is still running.
  * @param  hadc ADC handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ADC_DeInit(ADC_HandleTypeDef* hadc)
{
  HAL_StatusTypeDef tmp_hal_status = HAL_OK;
  
  /* Check ADC handle */
  if(hadc == NULL)
  {
    return HAL_ERROR;
  }
  
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  
  /* Set ADC state */
  SET_BIT(hadc->State, HAL_ADC_STATE_BUSY_INTERNAL);
  
  /* Stop potential conversion on going, on regular group */
  tmp_hal_status = ADC_ConversionStop(hadc);
  
  /* Disable ADC peripheral if conversions are effectively stopped */
  if (tmp_hal_status == HAL_OK)
  {   
    /* Disable the ADC peripheral */
    tmp_hal_status = ADC_Disable(hadc);
    
    /* Check if ADC is effectively disabled */
    if (tmp_hal_status != HAL_ERROR)
    {
      /* Change ADC state */
      hadc->State = HAL_ADC_STATE_READY;
    }
  }
  
  
  /* Configuration of ADC parameters if previous preliminary actions are      */ 
  /* correctly completed.                                                     */
  if (tmp_hal_status != HAL_ERROR)
  {
    
    /* ========== Reset ADC registers ========== */
    /* Reset register IER */
    __HAL_ADC_DISABLE_IT(hadc, (ADC_IT_AWD | ADC_IT_OVR | ADC_IT_EOCAL | ADC_IT_EOS |  \
                                ADC_IT_EOC | ADC_IT_RDY | ADC_IT_EOSMP ));
  
        
    /* Reset register ISR */
    __HAL_ADC_CLEAR_FLAG(hadc, (ADC_FLAG_AWD | ADC_FLAG_EOCAL | ADC_FLAG_OVR | ADC_FLAG_EOS |  \
                                ADC_FLAG_EOC | ADC_FLAG_EOSMP | ADC_FLAG_RDY));
  
    
    /* Reset register CR */
    /* Disable voltage regulator */
    /* Note: Regulator disable useful for power saving */
    /* Reset ADVREGEN bit */
    hadc->Instance->CR &= ~ADC_CR_ADVREGEN;
    
    /* Bits ADC_CR_ADSTP, ADC_CR_ADSTART are in access mode "read-set": no direct reset applicable */
    /* No action */
    
    /* Reset register CFGR1 */
    hadc->Instance->CFGR1 &= ~(ADC_CFGR1_AWDCH  | ADC_CFGR1_AWDEN  | ADC_CFGR1_AWDSGL | \
                               ADC_CFGR1_DISCEN | ADC_CFGR1_AUTOFF | ADC_CFGR1_AUTDLY | \
                               ADC_CFGR1_CONT   | ADC_CFGR1_OVRMOD | ADC_CFGR1_EXTEN  | \
                               ADC_CFGR1_EXTSEL | ADC_CFGR1_ALIGN  | ADC_CFGR1_RES    | \
                               ADC_CFGR1_SCANDIR| ADC_CFGR1_DMACFG | ADC_CFGR1_DMAEN);
  
    /* Reset register CFGR2 */
    hadc->Instance->CFGR2 &= ~(ADC_CFGR2_TOVS  | ADC_CFGR2_OVSS  | ADC_CFGR2_OVSR | \
                               ADC_CFGR2_OVSE  | ADC_CFGR2_CKMODE );
  
    
    /* Reset register SMPR */
    hadc->Instance->SMPR &= ~(ADC_SMPR_SMPR);
    
    /* Reset register TR */
    hadc->Instance->TR &= ~(ADC_TR_LT | ADC_TR_HT);
    
    /* Reset register CALFACT */
    hadc->Instance->CALFACT &= ~(ADC_CALFACT_CALFACT);
  
  
  
  
    
    /* Reset register DR */
    /* bits in access mode read only, no direct reset applicable*/
  
    /* Reset register CALFACT */
    hadc->Instance->CALFACT &= ~(ADC_CALFACT_CALFACT);
  
    /* ========== Hard reset ADC peripheral ========== */
    /* Performs a global reset of the entire ADC peripheral: ADC state is     */
    /* forced to a similar state after device power-on.                       */
    /* If needed, copy-paste and uncomment the following reset code into      */
    /* function "void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)":              */
    /*                                                                        */
    /*  __HAL_RCC_ADC1_FORCE_RESET()                                                  */
    /*  __HAL_RCC_ADC1_RELEASE_RESET()                                                */
  
#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
    if (hadc->MspDeInitCallback == NULL)
    {
      hadc->MspDeInitCallback = HAL_ADC_MspDeInit; /* Legacy weak MspDeInit  */
    }
    
    /* DeInit the low level hardware */
    hadc->MspDeInitCallback(hadc);
#else
    /* DeInit the low level hardware */
    HAL_ADC_MspDeInit(hadc);
#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */
    
    /* Set ADC error code to none */
    ADC_CLEAR_ERRORCODE(hadc);
    
    /* Set ADC state */
    hadc->State = HAL_ADC_STATE_RESET;
  }
  
  /* Process unlocked */
  __HAL_UNLOCK(hadc);
  
  /* Return function status */
  return tmp_hal_status;
}

/**
  * @brief  Initialize the ADC MSP.
  * @param  hadc ADC handle
  * @retval None
  */
__weak void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hadc);

  /* NOTE : This function should not be modified. When the callback is needed,
            function HAL_ADC_MspInit must be implemented in the user file.
   */ 
}

/**
  * @brief  DeInitialize the ADC MSP.
  * @param  hadc ADC handle
  * @retval None
  */
__weak void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hadc);

  /* NOTE : This function should not be modified. When the callback is needed,
            function HAL_ADC_MspDeInit must be implemented in the user file.
   */ 
}

#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
/**
  * @brief  Register a User ADC Callback
  *         To be used instead of the weak predefined callback
  * @param  hadc Pointer to a ADC_HandleTypeDef structure that contains
  *                the configuration information for the specified ADC.
  * @param  CallbackID ID of the callback to be registered
  *         This parameter can be one of the following values:
  *          @arg @ref HAL_ADC_CONVERSION_COMPLETE_CB_ID      ADC conversion complete callback ID
  *          @arg @ref HAL_ADC_CONVERSION_HALF_CB_ID          ADC conversion complete callback ID
  *          @arg @ref HAL_ADC_LEVEL_OUT_OF_WINDOW_1_CB_ID    ADC analog watchdog 1 callback ID
  *          @arg @ref HAL_ADC_ERROR_CB_ID                    ADC error callback ID
  *          @arg @ref HAL_ADC_INJ_CONVERSION_COMPLETE_CB_ID  ADC group injected conversion complete callback ID
  *          @arg @ref HAL_ADC_MSPINIT_CB_ID                  ADC Msp Init callback ID
  *          @arg @ref HAL_ADC_MSPDEINIT_CB_ID                ADC Msp DeInit callback ID
  *          @arg @ref HAL_ADC_MSPINIT_CB_ID MspInit callback ID
  *          @arg @ref HAL_ADC_MSPDEINIT_CB_ID MspDeInit callback ID
  * @param  pCallback pointer to the Callback function
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ADC_RegisterCallback(ADC_HandleTypeDef *hadc, HAL_ADC_CallbackIDTypeDef CallbackID, pADC_CallbackTypeDef pCallback)
{
  HAL_StatusTypeDef status = HAL_OK;
  
  if (pCallback == NULL)
  {
    /* Update the error code */
    hadc->ErrorCode |= HAL_ADC_ERROR_INVALID_CALLBACK;

    return HAL_ERROR;
  }
  
  if ((hadc->State & HAL_ADC_STATE_READY) != 0)
  {
    switch (CallbackID)
    {
      case HAL_ADC_CONVERSION_COMPLETE_CB_ID :
        hadc->ConvCpltCallback = pCallback;
        break;
      
      case HAL_ADC_CONVERSION_HALF_CB_ID :
        hadc->ConvHalfCpltCallback = pCallback;
        break;
      
      case HAL_ADC_LEVEL_OUT_OF_WINDOW_1_CB_ID :
        hadc->LevelOutOfWindowCallback = pCallback;
        break;
      
      case HAL_ADC_ERROR_CB_ID :
        hadc->ErrorCallback = pCallback;
        break;
      
      case HAL_ADC_MSPINIT_CB_ID :
        hadc->MspInitCallback = pCallback;
        break;
      
      case HAL_ADC_MSPDEINIT_CB_ID :
        hadc->MspDeInitCallback = pCallback;
        break;
      
      default :
        /* Update the error code */
        hadc->ErrorCode |= HAL_ADC_ERROR_INVALID_CALLBACK;

        /* Return error status */
        status = HAL_ERROR;
        break;
    }
  }
  else if (HAL_ADC_STATE_RESET == hadc->State)
  {
    switch (CallbackID)
    {
      case HAL_ADC_MSPINIT_CB_ID :
        hadc->MspInitCallback = pCallback;
        break;
      
      case HAL_ADC_MSPDEINIT_CB_ID :
        hadc->MspDeInitCallback = pCallback;
        break;
      
      default :
        /* Update the error code */
        hadc->ErrorCode |= HAL_ADC_ERROR_INVALID_CALLBACK;
      
        /* Return error status */
        status = HAL_ERROR;
        break;
    }
  }
  else
  {
    /* Update the error code */
    hadc->ErrorCode |= HAL_ADC_ERROR_INVALID_CALLBACK;
    
    /* Return error status */
    status =  HAL_ERROR;
  }
  
  return status;
}

/**
  * @brief  Unregister a ADC Callback
  *         ADC callback is redirected to the weak predefined callback
  * @param  hadc Pointer to a ADC_HandleTypeDef structure that contains
  *                the configuration information for the specified ADC.
  * @param  CallbackID ID of the callback to be unregistered
  *         This parameter can be one of the following values:
  *          @arg @ref HAL_ADC_CONVERSION_COMPLETE_CB_ID      ADC conversion complete callback ID
  *          @arg @ref HAL_ADC_CONVERSION_HALF_CB_ID          ADC conversion complete callback ID
  *          @arg @ref HAL_ADC_LEVEL_OUT_OF_WINDOW_1_CB_ID    ADC analog watchdog 1 callback ID
  *          @arg @ref HAL_ADC_ERROR_CB_ID                    ADC error callback ID
  *          @arg @ref HAL_ADC_INJ_CONVERSION_COMPLETE_CB_ID  ADC group injected conversion complete callback ID
  *          @arg @ref HAL_ADC_MSPINIT_CB_ID                  ADC Msp Init callback ID
  *          @arg @ref HAL_ADC_MSPDEINIT_CB_ID                ADC Msp DeInit callback ID
  *          @arg @ref HAL_ADC_MSPINIT_CB_ID MspInit callback ID
  *          @arg @ref HAL_ADC_MSPDEINIT_CB_ID MspDeInit callback ID
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ADC_UnRegisterCallback(ADC_HandleTypeDef *hadc, HAL_ADC_CallbackIDTypeDef CallbackID)
{
  HAL_StatusTypeDef status = HAL_OK;
  
  if ((hadc->State & HAL_ADC_STATE_READY) != 0)
  {
    switch (CallbackID)
    {
      case HAL_ADC_CONVERSION_COMPLETE_CB_ID :
        hadc->ConvCpltCallback = HAL_ADC_ConvCpltCallback;
        break;
      
      case HAL_ADC_CONVERSION_HALF_CB_ID :
        hadc->ConvHalfCpltCallback = HAL_ADC_ConvHalfCpltCallback;
        break;
      
      case HAL_ADC_LEVEL_OUT_OF_WINDOW_1_CB_ID :
        hadc->LevelOutOfWindowCallback = HAL_ADC_LevelOutOfWindowCallback;
        break;
      
      case HAL_ADC_ERROR_CB_ID :
        hadc->ErrorCallback = HAL_ADC_ErrorCallback;
        break;
      
      case HAL_ADC_MSPINIT_CB_ID :
        hadc->MspInitCallback = HAL_ADC_MspInit; /* Legacy weak MspInit              */
        break;
      
      case HAL_ADC_MSPDEINIT_CB_ID :
        hadc->MspDeInitCallback = HAL_ADC_MspDeInit; /* Legacy weak MspDeInit            */
        break;
      
      default :
        /* Update the error code */
        hadc->ErrorCode |= HAL_ADC_ERROR_INVALID_CALLBACK;
        
        /* Return error status */
        status =  HAL_ERROR;
        break;
    }
  }
  else if (HAL_ADC_STATE_RESET == hadc->State)
  {
    switch (CallbackID)
    {
      case HAL_ADC_MSPINIT_CB_ID :
        hadc->MspInitCallback = HAL_ADC_MspInit;                   /* Legacy weak MspInit              */
        break;
        
      case HAL_ADC_MSPDEINIT_CB_ID :
        hadc->MspDeInitCallback = HAL_ADC_MspDeInit;               /* Legacy weak MspDeInit            */
        break;
        
      default :
        /* Update the error code */
        hadc->ErrorCode |= HAL_ADC_ERROR_INVALID_CALLBACK;
        
        /* Return error status */
        status =  HAL_ERROR;
        break;
    }
  }
  else
  {
    /* Update the error code */
    hadc->ErrorCode |= HAL_ADC_ERROR_INVALID_CALLBACK;
    
    /* Return error status */
    status =  HAL_ERROR;
  }
  
  return status;
}

#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */

/**
  * @}
  */

/** @defgroup ADC_Exported_Functions_Group2 ADC Input and Output operation functions
 *  @brief    ADC IO operation functions 
 *
@verbatim   
 ===============================================================================
                      ##### IO operation functions #####
 ===============================================================================
    [..]  This section provides functions allowing to:
      (+) Start conversion of regular group.
      (+) Stop conversion of regular group.
      (+) Poll for conversion complete on regular group.
      (+) Poll for conversion event.
      (+) Get result of regular channel conversion.
      (+) Start conversion of regular group and enable interruptions.
      (+) Stop conversion of regular group and disable interruptions.
      (+) Handle ADC interrupt request
      (+) Start conversion of regular group and enable DMA transfer.
      (+) Stop conversion of regular group and disable ADC DMA transfer.
@endverbatim
  * @{
  */

/**
  * @brief  Enable ADC, start conversion of regular group.
  * @note   Interruptions enabled in this function: None.
  * @param  hadc ADC handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ADC_Start(ADC_HandleTypeDef* hadc)
{
  HAL_StatusTypeDef tmp_hal_status = HAL_OK;
  
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  
  /* Perform ADC enable and conversion start if no conversion is on going */
  if (ADC_IS_CONVERSION_ONGOING_REGULAR(hadc) == RESET)
  {
    /* Process locked */
    __HAL_LOCK(hadc);
    
    /* Enable the ADC peripheral */
    /* If low power mode AutoPowerOff is enabled, power-on/off phases are     */
    /* performed automatically by hardware.                                   */
    if (hadc->Init.LowPowerAutoPowerOff != ENABLE)
    {
      tmp_hal_status = ADC_Enable(hadc);
    }
    
    /* Start conversion if ADC is effectively enabled */
    if (tmp_hal_status == HAL_OK)
    {
      /* Set ADC state                                                        */
      /* - Clear state bitfield related to regular group conversion results   */
      /* - Set state bitfield related to regular operation                    */
      ADC_STATE_CLR_SET(hadc->State,
                        HAL_ADC_STATE_READY | HAL_ADC_STATE_REG_EOC | HAL_ADC_STATE_REG_OVR | HAL_ADC_STATE_REG_EOSMP,
                        HAL_ADC_STATE_REG_BUSY);
      
      /* Reset ADC all error code fields */
      ADC_CLEAR_ERRORCODE(hadc);
      
      /* Process unlocked */
      /* Unlock before starting ADC conversions: in case of potential         */
      /* interruption, to let the process to ADC IRQ Handler.                 */
      __HAL_UNLOCK(hadc);
      
      /* Clear regular group conversion flag and overrun flag */
      /* (To ensure of no unknown state from potential previous ADC           */
      /* operations)                                                          */
      __HAL_ADC_CLEAR_FLAG(hadc, (ADC_FLAG_EOC | ADC_FLAG_EOS | ADC_FLAG_OVR));
      
      /* Enable conversion of regular group.                                  */
      /* If software start has been selected, conversion starts immediately.  */
      /* If external trigger has been selected, conversion will start at next */
      /* trigger event.                                                       */
      hadc->Instance->CR |= ADC_CR_ADSTART;
    }
  }
  else
  {
    tmp_hal_status = HAL_BUSY;
  }
  
  /* Return function status */
  return tmp_hal_status;
}

/**
  * @brief  Stop ADC conversion of regular group (and injected channels in 
  *         case of auto_injection mode), disable ADC peripheral.
  * @param  hadc ADC handle
  * @retval HAL status.
  */
HAL_StatusTypeDef HAL_ADC_Stop(ADC_HandleTypeDef* hadc)
{
  HAL_StatusTypeDef tmp_hal_status = HAL_OK;
  
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  
  /* Process locked */
  __HAL_LOCK(hadc);
  
  /* 1. Stop potential conversion on going, on ADC group regular */
  tmp_hal_status = ADC_ConversionStop(hadc);
  
  /* Disable ADC peripheral if conversions are effectively stopped */
  if (tmp_hal_status == HAL_OK)
  {
    /* 2. Disable the ADC peripheral */
    tmp_hal_status = ADC_Disable(hadc);
    
    /* Check if ADC is effectively disabled */
    if (tmp_hal_status == HAL_OK)
    {
      /* Set ADC state */
      ADC_STATE_CLR_SET(hadc->State,
                        HAL_ADC_STATE_REG_BUSY,
                        HAL_ADC_STATE_READY);
    }
  }
  
  /* Process unlocked */
  __HAL_UNLOCK(hadc);
  
  /* Return function status */
  return tmp_hal_status;
}

/**
  * @brief  Wait for regular group conversion to be completed.
  * @note   ADC conversion flags EOS (end of sequence) and EOC (end of
  *         conversion) are cleared by this function, with an exception:
  *         if low power feature "LowPowerAutoWait" is enabled, flags are 
  *         not cleared to not interfere with this feature until data register
  *         is read using function HAL_ADC_GetValue().
  * @note   This function cannot be used in a particular setup: ADC configured 
  *         in DMA mode and polling for end of each conversion (ADC init
  *         parameter "EOCSelection" set to ADC_EOC_SINGLE_CONV).
  *         In this case, DMA resets the flag EOC and polling cannot be
  *         performed on each conversion. Nevertheless, polling can still 
  *         be performed on the complete sequence (ADC init
  *         parameter "EOCSelection" set to ADC_EOC_SEQ_CONV).
  * @param  hadc ADC handle
  * @param  Timeout Timeout value in millisecond.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ADC_PollForConversion(ADC_HandleTypeDef* hadc, uint32_t Timeout)
{
  uint32_t tickstart = 0;
  uint32_t tmp_Flag_EOC   = 0x00;
  
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  
  /* If end of conversion selected to end of sequence conversions */
  if (hadc->Init.EOCSelection == ADC_EOC_SEQ_CONV)
  {
    tmp_Flag_EOC = ADC_FLAG_EOS;
  }
  /* If end of conversion selected to end of unitary conversion */
  else /* ADC_EOC_SINGLE_CONV */
  {
    /* Verification that ADC configuration is compliant with polling for      */
    /* each conversion:                                                       */
    /* Particular case is ADC configured in DMA mode and ADC sequencer with   */
    /* several ranks and polling for end of each conversion.                  */
    /* For code simplicity sake, this particular case is generalized to       */
    /* ADC configured in DMA mode and and polling for end of each conversion. */
    if (HAL_IS_BIT_SET(hadc->Instance->CFGR1, ADC_CFGR1_DMAEN))
    {
      /* Update ADC state machine to error */
      SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_CONFIG);
      
      /* Process unlocked */
      __HAL_UNLOCK(hadc);
      
      return HAL_ERROR;
    }
    else
    {
      tmp_Flag_EOC = (ADC_FLAG_EOC | ADC_FLAG_EOS);
    }
  }
  
  /* Get tick count */
  tickstart = HAL_GetTick();
  
  /* Wait until End of unitary conversion or sequence conversions flag is raised */
  while(HAL_IS_BIT_CLR(hadc->Instance->ISR, tmp_Flag_EOC))
  {
    /* Check if timeout is disabled (set to infinite wait) */
    if(Timeout != HAL_MAX_DELAY)
    {
      if((Timeout == 0U) || ((HAL_GetTick()-tickstart) > Timeout))
      {
        /* Update ADC state machine to timeout */
        SET_BIT(hadc->State, HAL_ADC_STATE_TIMEOUT);
        
        /* Process unlocked */
        __HAL_UNLOCK(hadc);
        
        return HAL_TIMEOUT;
      }
    }
  }
  
  /* Update ADC state machine */
  SET_BIT(hadc->State, HAL_ADC_STATE_REG_EOC);
  
  /* Determine whether any further conversion upcoming on group regular       */
  /* by external trigger, continuous mode or scan sequence on going.          */
  if(ADC_IS_SOFTWARE_START_REGULAR(hadc)        && 
     (hadc->Init.ContinuousConvMode == DISABLE)   )
  {
    /* If End of Sequence is reached, disable interrupts */
    if( __HAL_ADC_GET_FLAG(hadc, ADC_FLAG_EOS) )
    {
      /* Allowed to modify bits ADC_IT_EOC/ADC_IT_EOS only if bit             */
      /* ADSTART==0 (no conversion on going)                                  */
      if (ADC_IS_CONVERSION_ONGOING_REGULAR(hadc) == RESET)
      {
        /* Disable ADC end of single conversion interrupt on group regular */
        /* Note: Overrun interrupt was enabled with EOC interrupt in          */
        /* HAL_Start_IT(), but is not disabled here because can be used       */
        /* by overrun IRQ process below.                                      */
        __HAL_ADC_DISABLE_IT(hadc, ADC_IT_EOC | ADC_IT_EOS);
        
        /* Set ADC state */
        ADC_STATE_CLR_SET(hadc->State,
                          HAL_ADC_STATE_REG_BUSY,
                          HAL_ADC_STATE_READY);
      }
      else
      {
        /* Change ADC state to error state */
        SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_CONFIG);
        
        /* Set ADC error code to ADC peripheral internal error */
        SET_BIT(hadc->ErrorCode, HAL_ADC_ERROR_INTERNAL);
      }
    }
  }
  
  /* Clear end of conversion flag of regular group if low power feature       */
  /* "LowPowerAutoWait " is disabled, to not interfere with this feature      */
  /* until data register is read using function HAL_ADC_GetValue().           */
  if (hadc->Init.LowPowerAutoWait == DISABLE)
  {
    /* Clear regular group conversion flag */
    __HAL_ADC_CLEAR_FLAG(hadc, (ADC_FLAG_EOC | ADC_FLAG_EOS));
  }
  
  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Poll for ADC event.
  * @param  hadc ADC handle
  * @param  EventType the ADC event type.
  *          This parameter can be one of the following values:
  *            @arg ADC_AWD_EVENT: ADC Analog watchdog event
  *            @arg ADC_OVR_EVENT: ADC Overrun event
  * @param  Timeout Timeout value in millisecond.
  * @note   The relevant flag is cleared if found to be set, except for ADC_FLAG_OVR.
  *         Indeed, the latter is reset only if hadc->Init.Overrun field is set  
  *         to ADC_OVR_DATA_OVERWRITTEN. Otherwise, data register may be potentially overwritten 
  *         by a new converted data as soon as OVR is cleared.
  *         To reset OVR flag once the preserved data is retrieved, the user can resort
  *         to macro __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_OVR); 
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ADC_PollForEvent(ADC_HandleTypeDef* hadc, uint32_t EventType, uint32_t Timeout)
{
  uint32_t tickstart = 0U; 
  
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  assert_param(IS_ADC_EVENT_TYPE(EventType));
  
  /* Get tick count */
  tickstart = HAL_GetTick();
  
  /* Check selected event flag */
  while(__HAL_ADC_GET_FLAG(hadc, EventType) == RESET)
  {
    /* Check if timeout is disabled (set to infinite wait) */
    if(Timeout != HAL_MAX_DELAY)
    {
      if((Timeout == 0U) ||((HAL_GetTick() - tickstart ) > Timeout))
      {
        /* Update ADC state machine to timeout */
        SET_BIT(hadc->State, HAL_ADC_STATE_TIMEOUT);
        
        /* Process unlocked */
        __HAL_UNLOCK(hadc);
        
        return HAL_TIMEOUT;
      }
    }
  }
  
  switch(EventType)
  {
  /* Analog watchdog (level out of window) event */
  case ADC_AWD_EVENT:
    /* Set ADC state */
    SET_BIT(hadc->State, HAL_ADC_STATE_AWD1);
    
    /* Clear ADC analog watchdog flag */
    __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_AWD);
    break;
    
  /* Overrun event */
  default: /* Case ADC_OVR_EVENT */
    /* If overrun is set to overwrite previous data, overrun event is not     */
    /* considered as an error.                                                */
    /* (cf ref manual "Managing conversions without using the DMA and without */
    /* overrun ")                                                             */
    if (hadc->Init.Overrun == ADC_OVR_DATA_PRESERVED)
    {
      /* Set ADC state */
      SET_BIT(hadc->State, HAL_ADC_STATE_REG_OVR);
        
      /* Set ADC error code to overrun */
      SET_BIT(hadc->ErrorCode, HAL_ADC_ERROR_OVR);
    }
    
    /* Clear ADC Overrun flag */
    __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_OVR);
    break;
  }
  
  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Enable ADC, start conversion of regular group with interruption.
  * @note   Interruptions enabled in this function according to initialization
  *         setting : EOC (end of conversion), EOS (end of sequence), 
  *         OVR overrun.
  *         Each of these interruptions has its dedicated callback function.
  * @note   To guarantee a proper reset of all interruptions once all the needed
  *         conversions are obtained, HAL_ADC_Stop_IT() must be called to ensure 
  *         a correct stop of the IT-based conversions.
  * @note   By default, HAL_ADC_Start_IT() doesn't enable the End Of Sampling 
  *         interruption. If required (e.g. in case of oversampling with trigger
  *         mode), the user must:
  *          1. first clear the EOSMP flag if set with macro __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_EOSMP)             
  *          2. then enable the EOSMP interrupt with macro __HAL_ADC_ENABLE_IT(hadc, ADC_IT_EOSMP)     
  *          before calling HAL_ADC_Start_IT().
  * @param  hadc ADC handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ADC_Start_IT(ADC_HandleTypeDef* hadc)
{
  HAL_StatusTypeDef tmp_hal_status = HAL_OK;
  
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  
  /* Perform ADC enable and conversion start if no conversion is on going */
  if (ADC_IS_CONVERSION_ONGOING_REGULAR(hadc) == RESET)
  {
    /* Process locked */
    __HAL_LOCK(hadc);
    
    /* Enable the ADC peripheral */
    /* If low power mode AutoPowerOff is enabled, power-on/off phases are     */
    /* performed automatically by hardware.                                   */
    if (hadc->Init.LowPowerAutoPowerOff != ENABLE)
    {
      tmp_hal_status = ADC_Enable(hadc);
    }
    
    /* Start conversion if ADC is effectively enabled */
    if (tmp_hal_status == HAL_OK)
    {
      /* Set ADC state                                                        */
      /* - Clear state bitfield related to regular group conversion results   */
      /* - Set state bitfield related to regular operation                    */
      ADC_STATE_CLR_SET(hadc->State,
                        HAL_ADC_STATE_READY | HAL_ADC_STATE_REG_EOC | HAL_ADC_STATE_REG_OVR | HAL_ADC_STATE_REG_EOSMP,
                        HAL_ADC_STATE_REG_BUSY);
      
      /* Reset ADC all error code fields */
      ADC_CLEAR_ERRORCODE(hadc);
      
      /* Process unlocked */
      /* Unlock before starting ADC conversions: in case of potential         */
      /* interruption, to let the process to ADC IRQ Handler.                 */
      __HAL_UNLOCK(hadc);
      
      /* Clear regular group conversion flag and overrun flag */
      /* (To ensure of no unknown state from potential previous ADC           */
      /* operations)                                                          */
      __HAL_ADC_CLEAR_FLAG(hadc, (ADC_FLAG_EOC | ADC_FLAG_EOS | ADC_FLAG_OVR));
      
      /* Enable ADC end of conversion interrupt */
      /* Enable ADC overrun interrupt */  
      switch(hadc->Init.EOCSelection)
      {
        case ADC_EOC_SEQ_CONV: 
          __HAL_ADC_DISABLE_IT(hadc, ADC_IT_EOC);
          __HAL_ADC_ENABLE_IT(hadc, (ADC_IT_EOS | ADC_IT_OVR));
          break;
        /* case ADC_EOC_SINGLE_CONV */
        default:
          __HAL_ADC_ENABLE_IT(hadc, (ADC_IT_EOC | ADC_IT_EOS | ADC_IT_OVR));
          break;
      }
      
      /* Enable conversion of regular group.                                  */
      /* If software start has been selected, conversion starts immediately.  */
      /* If external trigger has been selected, conversion will start at next */
      /* trigger event.                                                       */
      hadc->Instance->CR |= ADC_CR_ADSTART;
    }
  }
  else
  {
    tmp_hal_status = HAL_BUSY;
  }
  
  /* Return function status */
  return tmp_hal_status;
}

/**
  * @brief  Stop ADC conversion of regular group (and injected group in 
  *         case of auto_injection mode), disable interrution of 
  *         end-of-conversion, disable ADC peripheral.
  * @param  hadc ADC handle
  * @retval HAL status.
  */
HAL_StatusTypeDef HAL_ADC_Stop_IT(ADC_HandleTypeDef* hadc)
{
  HAL_StatusTypeDef tmp_hal_status = HAL_OK;
  
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  
  /* Process locked */
  __HAL_LOCK(hadc);
  
  /* 1. Stop potential conversion on going, on ADC group regular */
  tmp_hal_status = ADC_ConversionStop(hadc);
  
  /* Disable ADC peripheral if conversions are effectively stopped */
  if (tmp_hal_status == HAL_OK)
  {
    /* Disable ADC end of conversion interrupt for regular group */
    /* Disable ADC overrun interrupt */
    __HAL_ADC_DISABLE_IT(hadc, (ADC_IT_EOC | ADC_IT_EOS | ADC_IT_OVR));
    
    /* 2. Disable the ADC peripheral */
    tmp_hal_status = ADC_Disable(hadc);
    
    /* Check if ADC is effectively disabled */
    if (tmp_hal_status == HAL_OK)
    {
      /* Set ADC state */
      ADC_STATE_CLR_SET(hadc->State,
                        HAL_ADC_STATE_REG_BUSY,
                        HAL_ADC_STATE_READY);
    }
  }
  
  /* Process unlocked */
  __HAL_UNLOCK(hadc);
  
  /* Return function status */
  return tmp_hal_status;
}

/**
  * @brief  Enable ADC, start conversion of regular group and transfer result through DMA.
  * @note   Interruptions enabled in this function:
  *         overrun (if applicable), DMA half transfer, DMA transfer complete. 
  *         Each of these interruptions has its dedicated callback function.
  * @param  hadc ADC handle
  * @param  pData Destination Buffer address.
  * @param  Length Length of data to be transferred from ADC peripheral to memory (in bytes)
  * @retval HAL status.
  */
HAL_StatusTypeDef HAL_ADC_Start_DMA(ADC_HandleTypeDef* hadc, uint32_t* pData, uint32_t Length)
{
  HAL_StatusTypeDef tmp_hal_status = HAL_OK;
  
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  
  /* Perform ADC enable and conversion start if no conversion is on going */
  if (ADC_IS_CONVERSION_ONGOING_REGULAR(hadc) == RESET)
  {
    /* Process locked */
    __HAL_LOCK(hadc);

    /* Enable the ADC peripheral */
    /* If low power mode AutoPowerOff is enabled, power-on/off phases are       */
    /* performed automatically by hardware.                                     */
    if (hadc->Init.LowPowerAutoPowerOff != ENABLE)
    {
      tmp_hal_status = ADC_Enable(hadc);
    }
    
    /* Start conversion if ADC is effectively enabled */
    if (tmp_hal_status == HAL_OK)
    {
      /* Set ADC state                                                        */
      /* - Clear state bitfield related to regular group conversion results   */
      /* - Set state bitfield related to regular operation                    */
      ADC_STATE_CLR_SET(hadc->State,
                        HAL_ADC_STATE_READY | HAL_ADC_STATE_REG_EOC | HAL_ADC_STATE_REG_OVR | HAL_ADC_STATE_REG_EOSMP,
                        HAL_ADC_STATE_REG_BUSY);
      
      /* Reset ADC all error code fields */
      ADC_CLEAR_ERRORCODE(hadc);
      
      /* Process unlocked */
      /* Unlock before starting ADC conversions: in case of potential         */
      /* interruption, to let the process to ADC IRQ Handler.                 */
      __HAL_UNLOCK(hadc);

      /* Set the DMA transfer complete callback */
      hadc->DMA_Handle->XferCpltCallback = ADC_DMAConvCplt;
      
      /* Set the DMA half transfer complete callback */
      hadc->DMA_Handle->XferHalfCpltCallback = ADC_DMAHalfConvCplt;
      
      /* Set the DMA error callback */
      hadc->DMA_Handle->XferErrorCallback = ADC_DMAError;
      
      
      /* Manage ADC and DMA start: ADC overrun interruption, DMA start, ADC   */
      /* start (in case of SW start):                                         */
      
      /* Clear regular group conversion flag and overrun flag */
      /* (To ensure of no unknown state from potential previous ADC           */
      /* operations)                                                          */
      __HAL_ADC_CLEAR_FLAG(hadc, (ADC_FLAG_EOC | ADC_FLAG_EOS | ADC_FLAG_OVR));
      
      /* Enable ADC overrun interrupt */
      __HAL_ADC_ENABLE_IT(hadc, ADC_IT_OVR);
      
      /* Enable ADC DMA mode */
      hadc->Instance->CFGR1 |= ADC_CFGR1_DMAEN;
      
      /* Start the DMA channel */
      HAL_DMA_Start_IT(hadc->DMA_Handle, (uint32_t)&hadc->Instance->DR, (uint32_t)pData, Length);
      
      /* Enable conversion of regular group.                                  */
      /* If software start has been selected, conversion starts immediately.  */
      /* If external trigger has been selected, conversion will start at next */
      /* trigger event.                                                       */
      hadc->Instance->CR |= ADC_CR_ADSTART;
    }
  }
  else
  {
    tmp_hal_status = HAL_BUSY;
  }
  
  /* Return function status */
  return tmp_hal_status;
}

/**
  * @brief  Stop ADC conversion of regular group (and injected group in 
  *         case of auto_injection mode), disable ADC DMA transfer, disable 
  *         ADC peripheral.
  *         Each of these interruptions has its dedicated callback function.
  * @param  hadc ADC handle
  * @retval HAL status.
  */
HAL_StatusTypeDef HAL_ADC_Stop_DMA(ADC_HandleTypeDef* hadc)
{
  HAL_StatusTypeDef tmp_hal_status = HAL_OK;
  
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  
  /* Process locked */
  __HAL_LOCK(hadc);
  
  /* 1. Stop potential ADC group regular conversion on going */
  tmp_hal_status = ADC_ConversionStop(hadc);
  
  /* Disable ADC peripheral if conversions are effectively stopped */
  if (tmp_hal_status == HAL_OK)
  {
    /* Disable ADC DMA (ADC DMA configuration ADC_CFGR_DMACFG is kept) */
    CLEAR_BIT(hadc->Instance->CFGR1, ADC_CFGR1_DMAEN);
    
    /* Disable the DMA channel (in case of DMA in circular mode or stop       */
    /* while DMA transfer is on going)                                        */
    tmp_hal_status = HAL_DMA_Abort(hadc->DMA_Handle);
    
    /* Check if DMA channel effectively disabled */
    if (tmp_hal_status != HAL_OK)
    {
      /* Update ADC state machine to error */
      SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_DMA);
    }
    
    /* Disable ADC overrun interrupt */
    __HAL_ADC_DISABLE_IT(hadc, ADC_IT_OVR);
    
    /* 2. Disable the ADC peripheral */
    /* Update "tmp_hal_status" only if DMA channel disabling passed, to keep  */
    /* in memory a potential failing status.                                  */
    if (tmp_hal_status == HAL_OK)
    {
      tmp_hal_status = ADC_Disable(hadc);
    }
    else
    {
      ADC_Disable(hadc);
    }

    /* Check if ADC is effectively disabled */
    if (tmp_hal_status == HAL_OK)
    {
      /* Set ADC state */
      ADC_STATE_CLR_SET(hadc->State,
                        HAL_ADC_STATE_REG_BUSY,
                        HAL_ADC_STATE_READY);
    }
    
  }
  
  /* Process unlocked */
  __HAL_UNLOCK(hadc);
  
  /* Return function status */
  return tmp_hal_status;
}

/**
  * @brief  Get ADC regular group conversion result.
  * @note   Reading register DR automatically clears ADC flag EOC
  *         (ADC group regular end of unitary conversion).
  * @note   This function does not clear ADC flag EOS 
  *         (ADC group regular end of sequence conversion).
  *         Occurrence of flag EOS rising:
  *          - If sequencer is composed of 1 rank, flag EOS is equivalent
  *            to flag EOC.
  *          - If sequencer is composed of several ranks, during the scan
  *            sequence flag EOC only is raised, at the end of the scan sequence
  *            both flags EOC and EOS are raised.
  *         To clear this flag, either use function:
  *         in programming model IT: @ref HAL_ADC_IRQHandler(), in programming
  *         model polling: @ref HAL_ADC_PollForConversion()
  *         or @ref __HAL_ADC_CLEAR_FLAG(&hadc, ADC_FLAG_EOS).
  * @param  hadc ADC handle
  * @retval ADC group regular conversion data
  */
uint32_t HAL_ADC_GetValue(ADC_HandleTypeDef* hadc)
{
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));

  /* Note: EOC flag is not cleared here by software because automatically     */
  /*       cleared by hardware when reading register DR.                      */
  
  /* Return ADC converted value */ 
  return hadc->Instance->DR;
}

/**
  * @brief  Handle ADC interrupt request.
  * @param  hadc ADC handle
  * @retval None
  */
void HAL_ADC_IRQHandler(ADC_HandleTypeDef* hadc)
{
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  assert_param(IS_FUNCTIONAL_STATE(hadc->Init.ContinuousConvMode));
  assert_param(IS_ADC_EOC_SELECTION(hadc->Init.EOCSelection));
  
  /* ========== Check End of Conversion flag for regular group ========== */
  if( (__HAL_ADC_GET_FLAG(hadc, ADC_FLAG_EOC) && __HAL_ADC_GET_IT_SOURCE(hadc, ADC_IT_EOC)) || 
      (__HAL_ADC_GET_FLAG(hadc, ADC_FLAG_EOS) && __HAL_ADC_GET_IT_SOURCE(hadc, ADC_IT_EOS))   )
  {
    /* Update state machine on conversion status if not in error state */
    if (HAL_IS_BIT_CLR(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL))
    {
      /* Set ADC state */
      SET_BIT(hadc->State, HAL_ADC_STATE_REG_EOC);
    }
    
    /* Determine whether any further conversion upcoming on group regular     */
    /* by external trigger, continuous mode or scan sequence on going.        */
    if(ADC_IS_SOFTWARE_START_REGULAR(hadc)        && 
       (hadc->Init.ContinuousConvMode == DISABLE)   )
    {
      /* If End of Sequence is reached, disable interrupts */
      if( __HAL_ADC_GET_FLAG(hadc, ADC_FLAG_EOS) )
      {
        /* Allowed to modify bits ADC_IT_EOC/ADC_IT_EOS only if bit           */
        /* ADSTART==0 (no conversion on going)                                */
        if (ADC_IS_CONVERSION_ONGOING_REGULAR(hadc) == RESET)
        {
          /* Disable ADC end of single conversion interrupt on group regular */
          /* Note: Overrun interrupt was enabled with EOC interrupt in        */
          /* HAL_Start_IT(), but is not disabled here because can be used     */
          /* by overrun IRQ process below.                                    */
          __HAL_ADC_DISABLE_IT(hadc, ADC_IT_EOC | ADC_IT_EOS);
          
          /* Set ADC state */
          ADC_STATE_CLR_SET(hadc->State,
                            HAL_ADC_STATE_REG_BUSY,
                            HAL_ADC_STATE_READY);
        }
        else
        {
          /* Change ADC state to error state */
          SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_CONFIG);
          
          /* Set ADC error code to ADC peripheral internal error */
          SET_BIT(hadc->ErrorCode, HAL_ADC_ERROR_INTERNAL);
        }
      }
    }
    
    /* Note: into callback, to determine if conversion has been triggered     */
    /*       from EOC or EOS, possibility to use:                             */
    /*        " if( __HAL_ADC_GET_FLAG(&hadc, ADC_FLAG_EOS)) "                */
#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
      hadc->ConvCpltCallback(hadc);
#else
    HAL_ADC_ConvCpltCallback(hadc);
#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */
    
    /* Clear regular group conversion flag */
    /* Note: in case of overrun set to ADC_OVR_DATA_PRESERVED, end of         */
    /*       conversion flags clear induces the release of the preserved data.*/
    /*       Therefore, if the preserved data value is needed, it must be     */
    /*       read preliminarily into HAL_ADC_ConvCpltCallback().              */
    /* Note: Management of low power auto-wait enabled: flags must be cleared */
    /*       by user when fetching ADC conversion data.                       */
    /*       This case is managed in IRQ handler, but this low-power mode     */
    /*       should not be used with programming model IT or DMA.             */
    /*       Refer to comment of parameter "LowPowerAutoWait".                */
    if (hadc->Init.LowPowerAutoWait != ENABLE)
    {
      __HAL_ADC_CLEAR_FLAG(hadc, (ADC_FLAG_EOC | ADC_FLAG_EOS));
    }
  }
  
  /* ========== Check analog watchdog 1 flag ========== */
  if(__HAL_ADC_GET_FLAG(hadc, ADC_FLAG_AWD) && __HAL_ADC_GET_IT_SOURCE(hadc, ADC_IT_AWD))
  {
    /* Set ADC state */
    SET_BIT(hadc->State, HAL_ADC_STATE_AWD1);
    
#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
      hadc->LevelOutOfWindowCallback(hadc);
#else
    HAL_ADC_LevelOutOfWindowCallback(hadc);
#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */
    
    /* Clear ADC Analog watchdog flag */
    __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_AWD);
   
  }
  
  
  /* ========== Check Overrun flag ========== */
  if(__HAL_ADC_GET_FLAG(hadc, ADC_FLAG_OVR) && __HAL_ADC_GET_IT_SOURCE(hadc, ADC_IT_OVR))
  {
    /* If overrun is set to overwrite previous data (default setting),        */
    /* overrun event is not considered as an error.                           */
    /* (cf ref manual "Managing conversions without using the DMA and without */
    /* overrun ")                                                             */
    /* Exception for usage with DMA overrun event always considered as an     */
    /* error.                                                                 */
    if ((hadc->Init.Overrun == ADC_OVR_DATA_PRESERVED)            ||
        HAL_IS_BIT_SET(hadc->Instance->CFGR1, ADC_CFGR1_DMAEN)  )
    {
      /* Set ADC error code to overrun */
      SET_BIT(hadc->ErrorCode, HAL_ADC_ERROR_OVR);
      
      /* Clear ADC overrun flag */
      __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_OVR);
      
#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
      hadc->ErrorCallback(hadc);
#else
      HAL_ADC_ErrorCallback(hadc);
#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */
    }
    
    /* Clear the Overrun flag */
    __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_OVR);
  }
  
}

/**
  * @brief  Conversion complete callback in non-blocking mode.
  * @param  hadc ADC handle
  * @retval None
  */
__weak void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hadc);

  /* NOTE : This function should not be modified. When the callback is needed,
            function HAL_ADC_ConvCpltCallback must be implemented in the user file.
   */
}

/**
  * @brief  Conversion DMA half-transfer callback in non-blocking mode.
  * @param  hadc ADC handle
  * @retval None
  */
__weak void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hadc);

  /* NOTE : This function should not be modified. When the callback is needed,
            function HAL_ADC_ConvHalfCpltCallback must be implemented in the user file.
  */
}

/**
  * @brief  Analog watchdog 1 callback in non-blocking mode.
  * @param  hadc ADC handle
  * @retval None
  */
__weak void HAL_ADC_LevelOutOfWindowCallback(ADC_HandleTypeDef* hadc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hadc);

  /* NOTE : This function should not be modified. When the callback is needed,
            function HAL_ADC_LevelOutOfWindowCallback must be implemented in the user file.
  */
}

/**
  * @brief  ADC error callback in non-blocking mode
  *         (ADC conversion with interruption or transfer by DMA).
  * @note   In case of error due to overrun when using ADC with DMA transfer 
  *         (HAL ADC handle paramater "ErrorCode" to state "HAL_ADC_ERROR_OVR"):
  *         - Reinitialize the DMA using function "HAL_ADC_Stop_DMA()".
  *         - If needed, restart a new ADC conversion using function
  *           "HAL_ADC_Start_DMA()"
  *           (this function is also clearing overrun flag)
  * @param  hadc ADC handle
  * @retval None
  */
__weak void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hadc);

  /* NOTE : This function should not be modified. When the callback is needed,
            function HAL_ADC_ErrorCallback must be implemented in the user file.
  */
}

/**
  * @}
  */

/** @defgroup ADC_Exported_Functions_Group3 Peripheral Control functions
 *  @brief    Peripheral Control functions 
 *
@verbatim   
 ===============================================================================
             ##### Peripheral Control functions #####
 ===============================================================================  
    [..]  This section provides functions allowing to:
      (+) Configure channels on regular group
      (+) Configure the analog watchdog
      
@endverbatim
  * @{
  */

/**
  * @brief  Configure a channel to be assigned to ADC group regular.
  * @note   In case of usage of internal measurement channels:
  *         VrefInt/Vlcd(STM32L0x3xx only)/TempSensor.
  *         Sampling time constraints must be respected (sampling time can be 
  *         adjusted in function of ADC clock frequency and sampling time 
  *         setting).
  *         Refer to device datasheet for timings values, parameters TS_vrefint,
  *         TS_vlcd (STM32L0x3xx only), TS_temp (values rough order: 5us to 17us).
  *         These internal paths can be be disabled using function 
  *         HAL_ADC_DeInit().
  * @note   Possibility to update parameters on the fly:
  *         This function initializes channel into ADC group regular,
  *         following calls to this function can be used to reconfigure
  *         some parameters of structure "ADC_ChannelConfTypeDef" on the fly,
  *         without resetting the ADC.
  *         The setting of these parameters is conditioned to ADC state:
  *         Refer to comments of structure "ADC_ChannelConfTypeDef".
  * @param  hadc ADC handle
  * @param  sConfig Structure of ADC channel assigned to ADC group regular.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ADC_ConfigChannel(ADC_HandleTypeDef* hadc, ADC_ChannelConfTypeDef* sConfig)
{
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  assert_param(IS_ADC_CHANNEL(sConfig->Channel));
  assert_param(IS_ADC_RANK(sConfig->Rank));
  
  /* Process locked */
  __HAL_LOCK(hadc);
  
  /* Parameters update conditioned to ADC state:                              */
  /* Parameters that can be updated when ADC is disabled or enabled without   */
  /* conversion on going on regular group:                                    */
  /*  - Channel number                                                        */
  /*  - Management of internal measurement channels: Vbat/VrefInt/TempSensor  */
  if (ADC_IS_CONVERSION_ONGOING_REGULAR(hadc) != RESET)
  {
    /* Update ADC state machine to error */
    SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_CONFIG);
    /* Process unlocked */
    __HAL_UNLOCK(hadc);
    return HAL_ERROR;
  }
  
  if (sConfig->Rank != ADC_RANK_NONE)
  {
    /* Enable selected channels */
    hadc->Instance->CHSELR |= (uint32_t)(sConfig->Channel & ADC_CHANNEL_MASK);
    
    /* Management of internal measurement channels: Vlcd (STM32L0x3xx only)/VrefInt/TempSensor */
    /* internal measurement paths enable: If internal channel selected, enable  */
    /* dedicated internal buffers and path.                                     */
    
    #if defined(ADC_CCR_TSEN)
    /* If Temperature sensor channel is selected, then enable the internal      */
    /* buffers and path  */
    if (((sConfig->Channel & ADC_CHANNEL_MASK) & ADC_CHANNEL_TEMPSENSOR ) == (ADC_CHANNEL_TEMPSENSOR & ADC_CHANNEL_MASK))
    {
      ADC->CCR |= ADC_CCR_TSEN;   
      
      /* Delay for temperature sensor stabilization time */
      ADC_DelayMicroSecond(ADC_TEMPSENSOR_DELAY_US);
    }
    #endif
    
    /* If VRefInt channel is selected, then enable the internal buffers and path   */
    if (((sConfig->Channel & ADC_CHANNEL_MASK) & ADC_CHANNEL_VREFINT) == (ADC_CHANNEL_VREFINT & ADC_CHANNEL_MASK))
    {
      ADC->CCR |= ADC_CCR_VREFEN;   
    }
    
#if defined (STM32L053xx) || defined (STM32L063xx) || defined (STM32L073xx) || defined (STM32L083xx)
    /* If Vlcd channel is selected, then enable the internal buffers and path   */
    if (((sConfig->Channel & ADC_CHANNEL_MASK) & ADC_CHANNEL_VLCD) == (ADC_CHANNEL_VLCD & ADC_CHANNEL_MASK))
    {
      ADC->CCR |= ADC_CCR_VLCDEN;   
    }
#endif
  }
  else
  {
    /* Regular sequence configuration */
    /* Reset the channel selection register from the selected channel */
    hadc->Instance->CHSELR &= ~((uint32_t)(sConfig->Channel & ADC_CHANNEL_MASK));
    
    /* Management of internal measurement channels: VrefInt/TempSensor/Vbat */
    /* internal measurement paths disable: If internal channel selected,    */
    /* disable dedicated internal buffers and path.                         */
    #if defined(ADC_CCR_TSEN)
    if (((sConfig->Channel & ADC_CHANNEL_MASK) & ADC_CHANNEL_TEMPSENSOR ) == (ADC_CHANNEL_TEMPSENSOR & ADC_CHANNEL_MASK))
    {
      ADC->CCR &= ~ADC_CCR_TSEN;   
    }
    #endif
    
    /* If VRefInt channel is selected, then enable the internal buffers and path   */
    if (((sConfig->Channel & ADC_CHANNEL_MASK) & ADC_CHANNEL_VREFINT) == (ADC_CHANNEL_VREFINT & ADC_CHANNEL_MASK))
    {
      ADC->CCR &= ~ADC_CCR_VREFEN;   
    }
    
#if defined (STM32L053xx) || defined (STM32L063xx) || defined (STM32L073xx) || defined (STM32L083xx)
    /* If Vlcd channel is selected, then enable the internal buffers and path   */
    if (((sConfig->Channel & ADC_CHANNEL_MASK) & ADC_CHANNEL_VLCD) == (ADC_CHANNEL_VLCD & ADC_CHANNEL_MASK))
    {
      ADC->CCR &= ~ADC_CCR_VLCDEN;   
    }
#endif
  }
  
  /* Process unlocked */
  __HAL_UNLOCK(hadc);
  
  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Configure the analog watchdog.
  * @note   Possibility to update parameters on the fly:
  *         This function initializes the selected analog watchdog, successive  
  *         calls to this function can be used to reconfigure some parameters 
  *         of structure "ADC_AnalogWDGConfTypeDef" on the fly, without resetting 
  *         the ADC.
  *         The setting of these parameters is conditioned to ADC state.
  *         For parameters constraints, see comments of structure 
  *         "ADC_AnalogWDGConfTypeDef".
  * @note   Analog watchdog thresholds can be modified while ADC conversion
  *         is on going.
  *         In this case, some constraints must be taken into account:
  *         the programmed threshold values are effective from the next
  *         ADC EOC (end of unitary conversion).
  *         Considering that registers write delay may happen due to
  *         bus activity, this might cause an uncertainty on the
  *         effective timing of the new programmed threshold values.
  * @param  hadc ADC handle
  * @param  AnalogWDGConfig Structure of ADC analog watchdog configuration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ADC_AnalogWDGConfig(ADC_HandleTypeDef* hadc, ADC_AnalogWDGConfTypeDef* AnalogWDGConfig)
{
  HAL_StatusTypeDef tmp_hal_status = HAL_OK;
  
  uint32_t tmpAWDHighThresholdShifted;
  uint32_t tmpAWDLowThresholdShifted;
  
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  assert_param(IS_ADC_ANALOG_WATCHDOG_MODE(AnalogWDGConfig->WatchdogMode));
  assert_param(IS_FUNCTIONAL_STATE(AnalogWDGConfig->ITMode));
  
  if(AnalogWDGConfig->WatchdogMode == ADC_ANALOGWATCHDOG_SINGLE_REG)
  {
    assert_param(IS_ADC_CHANNEL(AnalogWDGConfig->Channel));
  }

  /* Verify if threshold is within the selected ADC resolution */
  assert_param(IS_ADC_RANGE(ADC_GET_RESOLUTION(hadc), AnalogWDGConfig->HighThreshold));
  assert_param(IS_ADC_RANGE(ADC_GET_RESOLUTION(hadc), AnalogWDGConfig->LowThreshold));

  /* Process locked */
  __HAL_LOCK(hadc);
  
  /* Parameters update conditioned to ADC state:                              */
  /* Parameters that can be updated when ADC is disabled or enabled without   */
  /* conversion on going on regular group:                                    */
  /*  - Analog watchdog channels                                              */
  /*  - Analog watchdog thresholds                                            */
  if (ADC_IS_CONVERSION_ONGOING_REGULAR(hadc) == RESET)
  {
    /* Configure ADC Analog watchdog interrupt */
    if(AnalogWDGConfig->ITMode == ENABLE)
    {
      /* Enable the ADC Analog watchdog interrupt */
      __HAL_ADC_ENABLE_IT(hadc, ADC_IT_AWD);
    }
    else
    {
      /* Disable the ADC Analog watchdog interrupt */
      __HAL_ADC_DISABLE_IT(hadc, ADC_IT_AWD);
    }
    
    /* Configuration of analog watchdog:                                        */
    /*  - Set the analog watchdog mode                                          */
    /*  - Set the Analog watchdog channel (is not used if watchdog              */
    /*    mode "all channels": ADC_CFGR1_AWD1SGL=0)                             */
    hadc->Instance->CFGR1 &= ~( ADC_CFGR1_AWDSGL |
                               ADC_CFGR1_AWDEN  |
                               ADC_CFGR1_AWDCH);
    
    hadc->Instance->CFGR1 |= ( AnalogWDGConfig->WatchdogMode |
                              (AnalogWDGConfig->Channel & ADC_CHANNEL_AWD_MASK));
    
    
    /* Shift the offset in function of the selected ADC resolution: Thresholds  */
    /* have to be left-aligned on bit 11, the LSB (right bits) are set to 0     */
    tmpAWDHighThresholdShifted = ADC_AWD1THRESHOLD_SHIFT_RESOLUTION(hadc, AnalogWDGConfig->HighThreshold);
    tmpAWDLowThresholdShifted  = ADC_AWD1THRESHOLD_SHIFT_RESOLUTION(hadc, AnalogWDGConfig->LowThreshold);
    
    /* Clear High & Low high thresholds */
    hadc->Instance->TR &= (uint32_t) ~ (ADC_TR_HT | ADC_TR_LT);
    
    /* Set the high threshold */
    hadc->Instance->TR = ADC_TRX_HIGHTHRESHOLD (tmpAWDHighThresholdShifted);
    /* Set the low threshold */
    hadc->Instance->TR |= tmpAWDLowThresholdShifted;
  }
  /* If a conversion is on going on regular group, no update could be done    */
  /* on neither of the AWD configuration structure parameters.                */
  else
  {
    /* Update ADC state machine to error */
    SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_CONFIG);
    
    tmp_hal_status = HAL_ERROR;
  }
  
  /* Process unlocked */
  __HAL_UNLOCK(hadc);
  
  /* Return function status */
  return tmp_hal_status;
}


/**
  * @}
  */

/** @defgroup ADC_Exported_Functions_Group4 Peripheral State functions
 *  @brief    ADC Peripheral State functions
 *
@verbatim
 ===============================================================================
            ##### Peripheral state and errors functions #####
 ===============================================================================
    [..]
    This subsection provides functions to get in run-time the status of the  
    peripheral.
      (+) Check the ADC state
      (+) Check the ADC error code

@endverbatim
  * @{
  */

/**
  * @brief  Return the ADC handle state.
  * @note   ADC state machine is managed by bitfields, ADC status must be 
  *         compared with states bits.
  *         For example:                                                         
  *           " if (HAL_IS_BIT_SET(HAL_ADC_GetState(hadc1), HAL_ADC_STATE_REG_BUSY)) "
  *           " if (HAL_IS_BIT_SET(HAL_ADC_GetState(hadc1), HAL_ADC_STATE_AWD1)    ) "
  * @param  hadc ADC handle
  * @retval ADC handle state (bitfield on 32 bits)
  */
uint32_t HAL_ADC_GetState(ADC_HandleTypeDef* hadc)
{
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  
  /* Return ADC handle state */
  return hadc->State;
}

/**
  * @brief  Return the ADC error code.
  * @param  hadc ADC handle
  * @retval ADC error code (bitfield on 32 bits)
  */
uint32_t HAL_ADC_GetError(ADC_HandleTypeDef *hadc)
{
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  
  return hadc->ErrorCode;
}

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup ADC_Private_Functions ADC Private Functions
  * @{
  */

/**
  * @brief  Enable the selected ADC.
  * @note   Prerequisite condition to use this function: ADC must be disabled
  *         and voltage regulator must be enabled (done into HAL_ADC_Init()).
  * @note   If low power mode AutoPowerOff is enabled, power-on/off phases are
  *         performed automatically by hardware.
  *         In this mode, this function is useless and must not be called because 
  *         flag ADC_FLAG_RDY is not usable.
  *         Therefore, this function must be called under condition of
  *         "if (hadc->Init.LowPowerAutoPowerOff != ENABLE)".
  * @param  hadc ADC handle
  * @retval HAL status.
  */
static HAL_StatusTypeDef ADC_Enable(ADC_HandleTypeDef* hadc)
{
  uint32_t tickstart = 0U;

  /* ADC enable and wait for ADC ready (in case of ADC is disabled or         */
  /* enabling phase not yet completed: flag ADC ready not yet set).           */
  /* Timeout implemented to not be stuck if ADC cannot be enabled (possible   */
  /* causes: ADC clock not running, ...).                                     */
  if (ADC_IS_ENABLE(hadc) == RESET)
  {
    /* Check if conditions to enable the ADC are fulfilled */
    if (ADC_ENABLING_CONDITIONS(hadc) == RESET)
    {
      /* Update ADC state machine to error */
      SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL);
      
      /* Set ADC error code to ADC peripheral internal error */
      SET_BIT(hadc->ErrorCode, HAL_ADC_ERROR_INTERNAL);
      
      return HAL_ERROR;
    }
    
    /* Enable the ADC peripheral */
    __HAL_ADC_ENABLE(hadc);
    
    /* Delay for ADC stabilization time. */
    ADC_DelayMicroSecond(ADC_STAB_DELAY_US);

    /* Get tick count */
    tickstart = HAL_GetTick();
    
    /* Wait for ADC effectively enabled */
    while(__HAL_ADC_GET_FLAG(hadc, ADC_FLAG_RDY) == RESET)
    {
      if((HAL_GetTick() - tickstart) > ADC_ENABLE_TIMEOUT)
      {
        /* Update ADC state machine to error */
        SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL);
        
        /* Set ADC error code to ADC peripheral internal error */
        SET_BIT(hadc->ErrorCode, HAL_ADC_ERROR_INTERNAL);
        
        return HAL_ERROR;
      }
    }
  }
   
  /* Return HAL status */
  return HAL_OK;
}

/**
  * @brief  Disable the selected ADC.
  * @note   Prerequisite condition to use this function: ADC conversions must be
  *         stopped.
  * @param  hadc ADC handle
  * @retval HAL status.
  */
static HAL_StatusTypeDef ADC_Disable(ADC_HandleTypeDef* hadc)
{
  uint32_t tickstart = 0U;
  
  /* Verification if ADC is not already disabled:                             */
  /* Note: forbidden to disable ADC (set bit ADC_CR_ADDIS) if ADC is already  */
  /*       disabled.                                                          */
  if (ADC_IS_ENABLE(hadc) != RESET)
  {
    /* Check if conditions to disable the ADC are fulfilled */
    if (ADC_DISABLING_CONDITIONS(hadc) != RESET)
    {
      /* Disable the ADC peripheral */
      __HAL_ADC_DISABLE(hadc);
    }
    else
    {
      /* Update ADC state machine to error */
      SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL);
      
      /* Set ADC error code to ADC peripheral internal error */
      SET_BIT(hadc->ErrorCode, HAL_ADC_ERROR_INTERNAL);
      
      return HAL_ERROR;
    }
     
    /* Wait for ADC effectively disabled */
    /* Get tick count */
    tickstart = HAL_GetTick();
    
    while(HAL_IS_BIT_SET(hadc->Instance->CR, ADC_CR_ADEN))
    {
      if((HAL_GetTick() - tickstart) > ADC_DISABLE_TIMEOUT)
      {
        /* Update ADC state machine to error */
        SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL);
        
        /* Set ADC error code to ADC peripheral internal error */
        SET_BIT(hadc->ErrorCode, HAL_ADC_ERROR_INTERNAL);
        
        return HAL_ERROR;
      }
    }
  }
  
  /* Return HAL status */
  return HAL_OK;
}


/**
  * @brief  Stop ADC conversion.
  * @note   Prerequisite condition to use this function: ADC conversions must be
  *         stopped to disable the ADC.
  * @param  hadc ADC handle
  * @retval HAL status.
  */
static HAL_StatusTypeDef ADC_ConversionStop(ADC_HandleTypeDef* hadc)
{
  uint32_t tickstart = 0U;

  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
    
  /* Verification if ADC is not already stopped on regular group to bypass    */
  /* this function if not needed.                                             */
  if (ADC_IS_CONVERSION_ONGOING_REGULAR(hadc))
  {
    
    /* Stop potential conversion on going on regular group */
    /* Software is allowed to set ADSTP only when ADSTART=1 and ADDIS=0 */
    if (HAL_IS_BIT_SET(hadc->Instance->CR, ADC_CR_ADSTART) && 
        HAL_IS_BIT_CLR(hadc->Instance->CR, ADC_CR_ADDIS)                  )
    {
      /* Stop conversions on regular group */
      hadc->Instance->CR |= ADC_CR_ADSTP;
    }
    
    /* Wait for conversion effectively stopped */
    /* Get tick count */
    tickstart = HAL_GetTick();
      
    while((hadc->Instance->CR & ADC_CR_ADSTART) != RESET)
    {
      if((HAL_GetTick() - tickstart) > ADC_STOP_CONVERSION_TIMEOUT)
      {
        /* Update ADC state machine to error */
        SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL);
      
        /* Set ADC error code to ADC peripheral internal error */
        SET_BIT(hadc->ErrorCode, HAL_ADC_ERROR_INTERNAL);
        
        return HAL_ERROR;
      }
    }
    
  }
   
  /* Return HAL status */
  return HAL_OK;
}


/**
  * @brief  DMA transfer complete callback. 
  * @param  hdma pointer to DMA handle.
  * @retval None
  */
static void ADC_DMAConvCplt(DMA_HandleTypeDef *hdma)
{
  /* Retrieve ADC handle corresponding to current DMA handle */
  ADC_HandleTypeDef* hadc = ( ADC_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
  
  /* Update state machine on conversion status if not in error state */
  if (HAL_IS_BIT_CLR(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL | HAL_ADC_STATE_ERROR_DMA))
  {
    /* Set ADC state */
    SET_BIT(hadc->State, HAL_ADC_STATE_REG_EOC);
    
    /* Determine whether any further conversion upcoming on group regular     */
    /* by external trigger, continuous mode or scan sequence on going.        */
    if(ADC_IS_SOFTWARE_START_REGULAR(hadc)        && 
       (hadc->Init.ContinuousConvMode == DISABLE)   )
    {
      /* If End of Sequence is reached, disable interrupts */
      if( __HAL_ADC_GET_FLAG(hadc, ADC_FLAG_EOS) )
      {
        /* Allowed to modify bits ADC_IT_EOC/ADC_IT_EOS only if bit           */
        /* ADSTART==0 (no conversion on going)                                */
        if (ADC_IS_CONVERSION_ONGOING_REGULAR(hadc) == RESET)
        {
          /* Disable ADC end of single conversion interrupt on group regular */
          /* Note: Overrun interrupt was enabled with EOC interrupt in        */
          /* HAL_Start_IT(), but is not disabled here because can be used     */
          /* by overrun IRQ process below.                                    */
          __HAL_ADC_DISABLE_IT(hadc, ADC_IT_EOC | ADC_IT_EOS);
          
          /* Set ADC state */
          ADC_STATE_CLR_SET(hadc->State,
                            HAL_ADC_STATE_REG_BUSY,
                            HAL_ADC_STATE_READY);
        }
        else
        {
          /* Change ADC state to error state */
          SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_CONFIG);
          
          /* Set ADC error code to ADC peripheral internal error */
          SET_BIT(hadc->ErrorCode, HAL_ADC_ERROR_INTERNAL);
        }
      }
    }

    /* Conversion complete callback */
#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
    hadc->ConvCpltCallback(hadc);
#else
    HAL_ADC_ConvCpltCallback(hadc);
#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */
  }
  else
  {
    /* Call DMA error callback */
    hadc->DMA_Handle->XferErrorCallback(hdma);
  }
}

/**
  * @brief  DMA half transfer complete callback. 
  * @param  hdma pointer to DMA handle.
  * @retval None
  */
static void ADC_DMAHalfConvCplt(DMA_HandleTypeDef *hdma)
{
  /* Retrieve ADC handle corresponding to current DMA handle */
  ADC_HandleTypeDef* hadc = ( ADC_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
  
  /* Half conversion callback */
#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
    hadc->ConvHalfCpltCallback(hadc);
#else
  HAL_ADC_ConvHalfCpltCallback(hadc); 
#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */
}

/**
  * @brief  DMA error callback.
  * @param  hdma pointer to DMA handle.
  * @retval None
  */
static void ADC_DMAError(DMA_HandleTypeDef *hdma)
{
  /* Retrieve ADC handle corresponding to current DMA handle */
  ADC_HandleTypeDef* hadc = ( ADC_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
  
  /* Set ADC state */
  SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_DMA);
  
  /* Set ADC error code to DMA error */
  SET_BIT(hadc->ErrorCode, HAL_ADC_ERROR_DMA);
  
  /* Error callback */
#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
  hadc->ErrorCallback(hadc);
#else
  HAL_ADC_ErrorCallback(hadc); 
#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */
}

/**
  * @brief  Delay micro seconds 
  * @param  microSecond  delay
  * @retval None
  */
static void ADC_DelayMicroSecond(uint32_t microSecond)
{
  /* Compute number of CPU cycles to wait for */
  __IO uint32_t waitLoopIndex = (microSecond * (SystemCoreClock / 1000000U));

  while(waitLoopIndex != 0U)
  {
    waitLoopIndex--;
  } 
}

/**
  * @}
  */

#endif /* HAL_ADC_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
