/**
  ******************************************************************************
  * @file    stm32u5xx_hal_adc.c
  * @author  MCD Application Team
  * @brief   This file provides firmware functions to manage the following
  *          functionalities of the Analog to Digital Converter (ADC)
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
  *          "stm32u5xx_hal_adc_ex.c".
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @verbatim
  ==============================================================================
                     ##### ADC peripheral features #####
  ==============================================================================
  [..]
  (+) 14-bit, 12-bit, 10-bit or 8-bit configurable resolution.

  (+) Interrupt generation at the end of regular conversion and in case of
      analog watchdog or overrun events.

  (+) Single and continuous conversion modes.

  (+) Scan mode for conversion of several channels sequentially.

  (+) Data alignment with in-built data coherency.

  (+) Programmable sampling time (channel wise)

  (+) External trigger (timer or EXTI) with configurable polarity

  (+) DMA request generation for transfer of conversions data of regular group.

  (+) Configurable delay between conversions in Dual interleaved mode.

  (+) ADC channels selectable single/differential input.

  (+) ADC offset shared on 4 offset instances.
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

        (++) Two clock settings are mandatory:
             (+++) ADC clock (core clock, also possibly conversion clock).

             (+++) ADC clock (conversions clock).
                   Six possible clock sources: synchronous clock derived from AHB clock
                   or asynchronous clock derived from system clock, the PLL2, the HSE, the HSI or the MSIK.

             (+++) Example:
                   Into HAL_ADC_MspInit() (recommended code location) or with
                   other device clock parameters configuration:
               (+++) __HAL_RCC_ADC_CLK_ENABLE();                  (mandatory)

               RCC_ADCCLKSOURCE_PLL2 enable:                   (optional: if asynchronous clock selected)
               (+++) RCC_PeriphClkInitTypeDef   RCC_PeriphClkInit;
               (+++) PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADCDAC;
               (+++) PeriphClkInit.AdcDacClockSelection = RCC_ADCDACCLKSOURCE_PLL2;
               (+++) HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

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
             using macro __HAL_RCC_ADCx_FORCE_RESET(), __HAL_RCC_ADCx_RELEASE_RESET().
        (++) ADC clock disable
             using the equivalent macro/functions as configuration step.
             (+++) Example:
                   Into HAL_ADC_MspDeInit() (recommended code location) or with
                   other device clock parameters configuration:
               (+++) __HAL_RCC_ADC_CLK_DISABLE();                  (if not used anymore)
               RCC_ADCDACCLKSOURCE_HSI restore:                      (optional)
               (+++) RCC_PeriphClkInitTypeDef   RCC_PeriphClkInit;
               (+++) PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADCDAC;
               (+++) PeriphClkInit.AdcDacClockSelection = RCC_ADCDACCLKSOURCE_HSI;
               (+++) HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

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
       (+) InjectedConvCpltCallback       : ADC group injected conversion complete callback
       (+) InjectedQueueOverflowCallback  : ADC group injected context queue overflow callback
       (+) LevelOutOfWindow2Callback      : ADC analog watchdog 2 callback
       (+) LevelOutOfWindow3Callback      : ADC analog watchdog 3 callback
       (+) EndOfSamplingCallback          : ADC end of sampling callback
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
       (+) InjectedConvCpltCallback       : ADC group injected conversion complete callback
       (+) InjectedQueueOverflowCallback  : ADC group injected context queue overflow callback
       (+) LevelOutOfWindow2Callback      : ADC analog watchdog 2 callback
       (+) LevelOutOfWindow3Callback      : ADC analog watchdog 3 callback
       (+) EndOfSamplingCallback          : ADC end of sampling callback
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
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx_hal.h"

/** @addtogroup STM32U5xx_HAL_Driver
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
#define ADC_CFGR_FIELDS_1  ((uint32_t)(ADC_CFGR1_RES    |\
                                       ADC_CFGR1_CONT   | ADC_CFGR1_OVRMOD  |\
                                       ADC_CFGR1_DISCEN | ADC_CFGR1_DISCNUM |\
                                       ADC_CFGR1_EXTEN  | ADC_CFGR1_EXTSEL)) /*!< ADC_CFGR fields of parameters that can be updated
when no regular conversion is on-going */

#define ADC_CFGR2_FIELDS  ((uint32_t)(ADC_CFGR2_ROVSE | ADC_CFGR2_OVSR  |\
                                      ADC_CFGR2_OVSS | ADC_CFGR2_TROVS |\
                                      ADC_CFGR2_ROVSM))                     /*!< ADC_CFGR2 fields of parameters that can be updated when no conversion
(neither regular nor injected) is on-going  */

/* Timeout values for ADC operations (enable settling time,                   */
/*   disable settling time, ...).                                             */
/*   Values defined to be higher than worst cases: low clock frequency,       */
/*   maximum prescalers.                                                      */
#define ADC_ENABLE_TIMEOUT              (2UL)    /*!< ADC enable time-out value  */
#define ADC_DISABLE_TIMEOUT             (2UL)    /*!< ADC disable time-out value */

/* Timeout to wait for current conversion on going to be completed.           */
/* Timeout fixed to worst case, for 1 channel.                                */
/*   - maximum sampling time (830.5 adc_clk)                                  */
/*   - ADC resolution (Tsar 14 bits= 14.5 adc_clk)                            */
/*   - ADC clock with prescaler 256                                           */
/*     823 * 256 = 210688 clock cycles max                                    */
/* Unit: cycles of CPU clock.                                                 */
#define ADC_CONVERSION_TIME_MAX_CPU_CYCLES ((uint32_t) 210688)  /*!< ADC conversion completion time-out value */

/**
  * @}
  */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

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
  *         (refer to description of RCC configuration for ADC
  *         in header of this file).
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
  * @note   Parameters related to common ADC registers (ADC clock mode) are set
  *         only if all ADCs are disabled.
  *         If this is not the case, these common parameters setting are
  *         bypassed without error reporting: it can be the intended behaviour in
  *         case of update of a parameter of ADC_InitTypeDef on the fly,
  *         without  disabling the other ADCs.
  * @param hadc ADC handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ADC_Init(ADC_HandleTypeDef *hadc)
{
  HAL_StatusTypeDef tmp_hal_status = HAL_OK;
  uint32_t tmpCFGR1 = 0UL;
  uint32_t tmpCFGR2 = 0UL;
  uint32_t tmp_adc_reg_is_conversion_on_going;
  __IO uint32_t wait_loop_index;
  uint32_t tmp_adc_is_conversion_on_going_regular;
  uint32_t tmp_adc_is_conversion_on_going_injected;

  /* Check ADC handle */
  if (hadc == NULL)
  {
    return HAL_ERROR;
  }

  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  assert_param(IS_ADC_CLOCKPRESCALER(hadc->Init.ClockPrescaler));
  assert_param(IS_ADC_RESOLUTION(hadc->Init.Resolution));
  assert_param(IS_FUNCTIONAL_STATE(hadc->Init.ContinuousConvMode));
  assert_param(IS_ADC_EXTTRIG_EDGE(hadc->Init.ExternalTrigConvEdge));

  if (hadc->Instance != ADC4) /* ADC1 or ADC2 */
  {
    assert_param(IS_ADC_SCAN_MODE(hadc->Init.ScanConvMode));
    assert_param(IS_ADC_GAIN_COMPENSATION(hadc->Init.GainCompensation));
    assert_param(IS_ADC_CONVERSIONDATAMGT(hadc->Init.ConversionDataManagement));
    assert_param(IS_ADC_EXTTRIG(hadc->Init.ExternalTrigConv));
  }
  else
  {
    assert_param(IS_ADC4_SCAN_MODE(hadc->Init.ScanConvMode));
    assert_param(IS_ADC_DATA_ALIGN(hadc->Init.DataAlign));
    assert_param(IS_FUNCTIONAL_STATE(hadc->Init.DMAContinuousRequests));
    assert_param(IS_ADC4_LOW_POWER(hadc->Init.LowPowerAutoPowerOff));
    assert_param(IS_ADC4_VREF_PROT(hadc->Init.VrefProtection));
    assert_param(IS_ADC4_EXTTRIG(hadc->Init.ExternalTrigConv));
    assert_param(IS_ADC_TRIGGER_FREQ(hadc->Init.TriggerFrequencyMode));
  }
  assert_param(IS_ADC_EOC_SELECTION(hadc->Init.EOCSelection));
  assert_param(IS_ADC_OVERRUN(hadc->Init.Overrun));
  assert_param(IS_FUNCTIONAL_STATE(hadc->Init.LowPowerAutoWait));
  assert_param(IS_FUNCTIONAL_STATE(hadc->Init.OversamplingMode));

  if (hadc->Init.ScanConvMode != ADC_SCAN_DISABLE)
  {
    assert_param(IS_FUNCTIONAL_STATE(hadc->Init.DiscontinuousConvMode));
    if (hadc->Instance != ADC4) /* ADC1 or ADC2 */
    {
      assert_param(IS_ADC_REGULAR_NB_CONV(hadc->Init.NbrOfConversion));
      if (hadc->Init.DiscontinuousConvMode == ENABLE)
      {
        assert_param(IS_ADC_REGULAR_DISCONT_NUMBER(hadc->Init.NbrOfDiscConversion));
      }
    }
    else
    {
      if (hadc->Init.ScanConvMode == ADC_SCAN_ENABLE)
      {
        assert_param(IS_ADC_REGULAR_NB_CONV(hadc->Init.NbrOfConversion));
      }
    }
  }

  /* DISCEN and CONT bits cannot be set at the same time */
  assert_param(!((hadc->Init.DiscontinuousConvMode == ENABLE) && (hadc->Init.ContinuousConvMode == ENABLE)));

  /* Actions performed only if ADC is coming from state reset:                */
  /* - Initialization of ADC MSP                                              */
  if (hadc->State == HAL_ADC_STATE_RESET)
  {
#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
    /* Init the ADC Callback settings */
    hadc->ConvCpltCallback              = HAL_ADC_ConvCpltCallback;                 /* Legacy weak callback */
    hadc->ConvHalfCpltCallback          = HAL_ADC_ConvHalfCpltCallback;             /* Legacy weak callback */
    hadc->LevelOutOfWindowCallback      = HAL_ADC_LevelOutOfWindowCallback;         /* Legacy weak callback */
    hadc->ErrorCallback                 = HAL_ADC_ErrorCallback;                    /* Legacy weak callback */
    if (hadc->Instance != ADC4) /* ADC1 or ADC2 */
    {
      hadc->InjectedConvCpltCallback      = HAL_ADCEx_InjectedConvCpltCallback;       /* Legacy weak callback */
      hadc->InjectedQueueOverflowCallback = HAL_ADCEx_InjectedQueueOverflowCallback;  /* Legacy weak callback */
    }
    hadc->LevelOutOfWindow2Callback     = HAL_ADCEx_LevelOutOfWindow2Callback;      /* Legacy weak callback */
    hadc->LevelOutOfWindow3Callback     = HAL_ADCEx_LevelOutOfWindow3Callback;      /* Legacy weak callback */
    hadc->EndOfSamplingCallback         = HAL_ADCEx_EndOfSamplingCallback;          /* Legacy weak callback */

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

    /* Set ADC error code to none */
    ADC_CLEAR_ERRORCODE(hadc);

    /* Initialize Lock */
    hadc->Lock = HAL_UNLOCKED;
  }

  /* - Exit from deep-power-down mode and ADC voltage regulator enable        */
  if (LL_ADC_IsDeepPowerDownEnabled(hadc->Instance) != 0UL)
  {
    /* Disable ADC deep power down mode */
    LL_ADC_DisableDeepPowerDown(hadc->Instance);

    /* System was in deep power down mode, calibration must
     be relaunched or a previously saved calibration factor
     re-applied once the ADC voltage regulator is enabled */
  }

  if (LL_ADC_IsInternalRegulatorEnabled(hadc->Instance) == 0UL)
  {
    /* Enable ADC internal voltage regulator */
    LL_ADC_EnableInternalRegulator(hadc->Instance);

    /* Note: Variable divided by 2 to compensate partially              */
    /*       CPU processing cycles, scaling in us split to not          */
    /*       exceed 32 bits register capacity and handle low frequency. */
    wait_loop_index = ((LL_ADC_DELAY_INTERNAL_REGUL_STAB_US / 10UL) * ((SystemCoreClock / (100000UL * 2UL)) + 1UL));
    while (wait_loop_index != 0UL)
    {
      wait_loop_index--;
    }
  }

  /* Verification that ADC voltage regulator is correctly enabled, whether    */
  /* or not ADC is coming from state reset (if any potential problem of       */
  /* clocking, voltage regulator would not be enabled).                       */
  if (LL_ADC_IsInternalRegulatorEnabled(hadc->Instance) == 0UL)
  {
    /* Update ADC state machine to error */
    SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL);

    /* Set ADC error code to ADC peripheral internal error */
    SET_BIT(hadc->ErrorCode, HAL_ADC_ERROR_INTERNAL);

    tmp_hal_status = HAL_ERROR;
  }

  /* Configuration of ADC parameters if previous preliminary actions are      */
  /* correctly completed and if there is no conversion on going on regular    */
  /* group (ADC may already be enabled at this point if HAL_ADC_Init() is     */
  /* called to update a parameter on the fly).                                */
  tmp_adc_reg_is_conversion_on_going = LL_ADC_REG_IsConversionOngoing(hadc->Instance);

  if (((hadc->State & HAL_ADC_STATE_ERROR_INTERNAL) == 0UL)
      && (tmp_adc_reg_is_conversion_on_going == 0UL)
     )
  {
    /* Set ADC state */
    ADC_STATE_CLR_SET(hadc->State,
                      HAL_ADC_STATE_REG_BUSY,
                      HAL_ADC_STATE_BUSY_INTERNAL);

    /* Configuration of common ADC parameters                                 */

    /* Parameters update conditioned to ADC state:                            */
    /* Parameters that can be updated only when ADC is disabled:              */
    /*  - clock configuration                                                 */
    if (LL_ADC_IsEnabled(hadc->Instance) == 0UL)
    {
      if (hadc->Instance != ADC4) /* ADC1 or ADC2 */
      {
        if (__LL_ADC_IS_ENABLED_ALL_COMMON_INSTANCE(__LL_ADC_COMMON_INSTANCE(hadc->Instance)) == 0UL)
        {
          /* Reset configuration of ADC common register CCR:                      */
          /*                                                                      */
          /*   - ADC clock mode and ACC prescaler (CKMODE and PRESC bits)are set  */
          /*     according to adc->Init.ClockPrescaler. It selects the clock      */
          /*    source and sets the clock division factor.                        */
          /*                                                                      */
          /* Some parameters of this register are not reset, since they are set   */
          /* by other functions and must be kept in case of usage of this         */
          /* function on the fly (update of a parameter of ADC_InitTypeDef        */
          /* without needing to reconfigure all other ADC groups/channels         */
          /* parameters):                                                         */
          /*   - when multimode feature is available, multimode-related           */
          /*     parameters: MDMA, DMACFG, DELAY, DUAL (set by API                */
          /*     HAL_ADCEx_MultiModeConfigChannel() )                             */
          /*   - internal measurement paths: Vbat, temperature sensor, Vref       */
          /*     (set into HAL_ADC_ConfigChannel() or                             */
          /*     HAL_ADCEx_InjectedConfigChannel() )                              */
          LL_ADC_SetCommonClock(__LL_ADC_COMMON_INSTANCE(hadc->Instance), hadc->Init.ClockPrescaler);
        }
      }
      else
      {
        /* Some parameters of this register are not reset, since they are set   */
        /* by other functions and must be kept in case of usage of this         */
        /* function on the fly (update of a parameter of ADC_InitTypeDef        */
        /* without needing to reconfigure all other ADC groups/channels         */
        /* parameters):                                                         */
        /*   - internal measurement paths: Vbat, temperature sensor, Vref       */
        /*     (set into HAL_ADC_ConfigChannel() )                              */

        /* Configuration of ADC resolution                                      */
        MODIFY_REG(hadc->Instance->CFGR1,
                   ADC_CFGR1_RES,
                   __LL_ADC_RESOLUTION_ADC1_TO_ADC4(hadc->Init.Resolution));   /* Convert resolution for the ADC4 */

        /* Configuration of ADC clock mode: clock source AHB or HSI with        */
        /* selectable prescaler.                                                */
        MODIFY_REG(ADC4_COMMON->CCR,
                   ADC_CCR_PRESC,
                   hadc->Init.ClockPrescaler & ADC_CCR_PRESC);
      }
    }
    if (hadc->Instance != ADC4) /* ADC1 or ADC2 */
    {
      /* Configuration of ADC:                                                  */
      /*  - resolution                               Init.Resolution            */
      /*  - external trigger to start conversion     Init.ExternalTrigConv      */
      /*  - external trigger polarity                Init.ExternalTrigConvEdge  */
      /*  - continuous conversion mode               Init.ContinuousConvMode    */
      /*  - overrun                                  Init.Overrun               */
      /*  - discontinuous mode                       Init.DiscontinuousConvMode */
      /*  - discontinuous mode channel count         Init.NbrOfDiscConversion   */

      tmpCFGR1  = (/*ADC_CFGR_AUTODELAY((uint32_t)hadc->Init.LowPowerAutoWait)             |*/
                    ADC_CFGR_CONTINUOUS((uint32_t)hadc->Init.ContinuousConvMode)         |
                    hadc->Init.Overrun                                                    |
                    hadc->Init.Resolution                                                 |
                    ADC_CFGR_REG_DISCONTINUOUS((uint32_t)hadc->Init.DiscontinuousConvMode));
    }
    else
    {
      /* Configuration of ADC:                                                  */
      /*  - discontinuous mode                                                  */
      /*  - LowPowerAutoWait mode                                               */
      /*  - LowPowerAutoPowerOff mode                                           */
      /*  - continuous conversion mode                                          */
      /*  - overrun                                                             */
      /*  - external trigger to start conversion                                */
      /*  - external trigger polarity                                           */
      /*  - data alignment                                                      */
      /*  - resolution                                                          */
      /*  - scan direction                                                      */
      /*  - DMA continuous request                                              */
      tmpCFGR1 |= (ADC_CFGR_AUTOWAIT((uint32_t)hadc->Init.LowPowerAutoWait)        |
                   ADC_CFGR_CONTINUOUS((uint32_t)hadc->Init.ContinuousConvMode)    |
                   ADC_CFGR_OVERRUN(hadc->Init.Overrun)                            |
                   hadc->Init.DataAlign                                            |
                   ADC_SCAN_SEQ_MODE(hadc->Init.ScanConvMode)                      |
                   ADC_CFGR_DMACONTREQ(hadc, (uint32_t)hadc->Init.DMAContinuousRequests));
    }

    if (hadc->Init.DiscontinuousConvMode == ENABLE)
    {
      if (hadc->Instance != ADC4) /* ADC1 or ADC2 */
      {
        tmpCFGR1 |= ADC_CFGR_DISCONTINUOUS_NUM(hadc->Init.NbrOfDiscConversion);
      }
      else
      {
        if (hadc->Init.ContinuousConvMode == DISABLE)
        {
          /* Enable the selected ADC group regular discontinuous mode */
          tmpCFGR1 |= ADC_CFGR1_DISCEN;
        }
        else
        {
          /* ADC regular group discontinuous was intended to be enabled,        */
          /* but ADC regular group modes continuous and sequencer discontinuous */
          /* cannot be enabled simultaneously.                                  */

          /* Update ADC state machine to error */
          SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_CONFIG);

          /* Set ADC error code to ADC IP internal error */
          SET_BIT(hadc->ErrorCode, HAL_ADC_ERROR_INTERNAL);
        }
      }
    }

    if (hadc->Instance != ADC4) /* ADC1 or ADC2 */
    {
      /* Enable external trigger if trigger selection is different of software  */
      /* start.                                                                 */
      /* Note: This configuration keeps the hardware feature of parameter       */
      /*       ExternalTrigConvEdge "trigger edge none" equivalent to           */
      /*       software start.                                                  */
      if (hadc->Init.ExternalTrigConv != ADC_SOFTWARE_START)
      {
        tmpCFGR1 |= ((hadc->Init.ExternalTrigConv & ADC_CFGR1_EXTSEL) | hadc->Init.ExternalTrigConvEdge);
      }
      /* Update Configuration Register CFGR */
      MODIFY_REG(hadc->Instance->CFGR1, ADC_CFGR_FIELDS_1, tmpCFGR1);
    }
    else
    {
      /* Enable external trigger if trigger selection is different of software  */
      /* start.                                                                 */
      /* Note: This configuration keeps the hardware feature of parameter       */
      /*       ExternalTrigConvEdge "trigger edge none" equivalent to           */
      /*       software start.                                                  */
      if (hadc->Init.ExternalTrigConv != ADC_SOFTWARE_START)
      {
        tmpCFGR1 |= ((hadc->Init.ExternalTrigConv & ADC4_CFGR1_EXTSEL) | hadc->Init.ExternalTrigConvEdge);
      }
      /* Update ADC configuration register with previous settings */
      MODIFY_REG(hadc->Instance->CFGR1,
                 ADC_CFGR1_DISCEN  |
                 ADC4_CFGR1_WAIT    |
                 ADC_CFGR1_CONT    |
                 ADC_CFGR1_OVRMOD  |
                 ADC_CFGR1_EXTSEL  |
                 ADC_CFGR1_EXTEN   |
                 ADC4_CFGR1_ALIGN   |
                 ADC4_CFGR1_SCANDIR |
                 ADC4_CFGR1_DMACFG,
                 tmpCFGR1);

      if (hadc->Init.LowPowerAutoPowerOff != ADC_LOW_POWER_NONE)
      {
        SET_BIT(hadc->Instance->PW, hadc->Init.LowPowerAutoPowerOff);
      }

      if (hadc->Init.VrefProtection != ADC_VREF_PPROT_NONE)
      {
        SET_BIT(hadc->Instance->PW, hadc->Init.VrefProtection);
      }

    }

    if (hadc->Instance != ADC4) /* ADC1 or ADC2 */
    {
      /* Parameters update conditioned to ADC state:                            */
      /* Parameters that can be updated when ADC is disabled or enabled without */
      /* conversion on going on regular and injected groups:                    */
      /*  - Conversion data management      Init.ConversionDataManagement       */
      /*  - LowPowerAutoWait feature        Init.LowPowerAutoWait               */
      /*  - Oversampling parameters         Init.Oversampling                   */
      tmp_adc_is_conversion_on_going_regular = LL_ADC_REG_IsConversionOngoing(hadc->Instance);
      tmp_adc_is_conversion_on_going_injected = LL_ADC_INJ_IsConversionOngoing(hadc->Instance);
      if ((tmp_adc_is_conversion_on_going_regular == 0UL)
          && (tmp_adc_is_conversion_on_going_injected == 0UL)
         )
      {
        tmpCFGR1 = (ADC_CFGR_AUTODELAY((uint32_t)hadc->Init.LowPowerAutoWait)        |
                    ADC_CFGR_DMACONTREQ(hadc, (uint32_t)hadc->Init.ConversionDataManagement));

        MODIFY_REG(hadc->Instance->CFGR1, ADC4_CFGR_FIELDS_2, tmpCFGR1);
        if (hadc->Init.GainCompensation != 0UL)
        {
          LL_ADC_SetGainCompensation(hadc->Instance, hadc->Init.GainCompensation);
        }

        if (hadc->Init.OversamplingMode == ENABLE)
        {
          assert_param(IS_ADC_OVERSAMPLING_RATIO(hadc->Init.Oversampling.Ratio));
          assert_param(IS_ADC_RIGHT_BIT_SHIFT(hadc->Init.Oversampling.RightBitShift));
          assert_param(IS_ADC_TRIGGERED_OVERSAMPLING_MODE(hadc->Init.Oversampling.TriggeredMode));
          assert_param(IS_ADC_REGOVERSAMPLING_MODE(hadc->Init.Oversampling.OversamplingStopReset));

          if ((hadc->Init.ExternalTrigConv == ADC_SOFTWARE_START)
              || (hadc->Init.ExternalTrigConvEdge == ADC_EXTERNALTRIGCONVEDGE_NONE))
          {
            /* Multi trigger is not applicable to software-triggered conversions */
            assert_param((hadc->Init.Oversampling.TriggeredMode == ADC_TRIGGEREDMODE_SINGLE_TRIGGER));
          }

          /* Configuration of Oversampler:                                       */
          /*  - Oversampling Ratio                                               */
          /*  - Right bit shift                                                  */
          /*  - Left bit shift                                                   */
          /*  - Triggered mode                                                   */
          /*  - Oversampling mode (continued/resumed)                            */
          /*  - trigger frequency mode                                           */
          MODIFY_REG(hadc->Instance->CFGR2, ADC_CFGR2_FIELDS,
                     ADC_CFGR2_ROVSE                                       |
                     (hadc->Init.Oversampling.Ratio << ADC_CFGR2_OVSR_Pos) |
                     hadc->Init.Oversampling.RightBitShift                 |
                     hadc->Init.Oversampling.TriggeredMode                 |
                     hadc->Init.Oversampling.OversamplingStopReset         |
                     (hadc->Init.TriggerFrequencyMode >> 2UL));
        }
        else
        {
          /* Disable ADC oversampling scope on ADC group regular */
          CLEAR_BIT(hadc->Instance->CFGR2, ADC_CFGR2_ROVSE);
        }

        /* Set the LeftShift parameter: it is applied to the final result with or without oversampling */
        MODIFY_REG(hadc->Instance->CFGR2, ADC_CFGR2_LSHIFT, hadc->Init.LeftBitShift);
      }
      /* Configuration of regular group sequencer:                              */
      /* - if scan mode is disabled, regular channels sequence length is set to */
      /*   0x00: 1 channel converted (channel on regular rank 1)                */
      /*   Parameter "NbrOfConversion" is discarded.                            */
      /*   Note: Scan mode is not present by hardware on this device, but       */
      /*   emulated by software for alignment over all STM32 devices.           */
      /* - if scan mode is enabled, regular channels sequence length is set to  */
      /*   parameter "NbrOfConversion".                                         */
      if (hadc->Init.ScanConvMode == ADC_SCAN_ENABLE)
      {
        /* Set number of ranks in regular group sequencer */
        MODIFY_REG(hadc->Instance->SQR1, ADC_SQR1_L, (hadc->Init.NbrOfConversion - (uint8_t)1));
      }
      else
      {
        CLEAR_BIT(hadc->Instance->SQR1, ADC_SQR1_L);
      }

      /* Initialize the ADC state */
      /* Clear HAL_ADC_STATE_BUSY_INTERNAL bit, set HAL_ADC_STATE_READY bit */
      ADC_STATE_CLR_SET(hadc->State, HAL_ADC_STATE_BUSY_INTERNAL, HAL_ADC_STATE_READY);

    }
    else
    {
      /* Configuration of ADC:                                                  */
      /*  - oversampling enable                                                 */
      /*  - oversampling ratio                                                  */
      /*  - oversampling shift                                                  */
      /*  - oversampling discontinuous mode (triggered mode)                    */
      /*  - trigger frequency mode                                              */
      tmpCFGR2 |= (hadc->Init.Oversampling.Ratio         |
                   hadc->Init.Oversampling.RightBitShift |
                   hadc->Init.Oversampling.TriggeredMode |
                   hadc->Init.TriggerFrequencyMode
                  );

      if (hadc->Init.OversamplingMode == ENABLE)
      {
        SET_BIT(tmpCFGR2, ADC_CFGR2_ROVSE);
      }
      MODIFY_REG(hadc->Instance->CFGR2,
                 ADC_CFGR2_LFTRIG | ADC_CFGR2_ROVSE | ADC4_CFGR2_OVSR | ADC_CFGR2_OVSS | ADC_CFGR2_TROVS,
                 tmpCFGR2);


      /* Channel sampling time configuration */
      LL_ADC_SetSamplingTimeCommonChannels(hadc->Instance, LL_ADC_SAMPLINGTIME_COMMON_1,                   \
                                           hadc->Init.SamplingTimeCommon1);
      LL_ADC_SetSamplingTimeCommonChannels(hadc->Instance, LL_ADC_SAMPLINGTIME_COMMON_2,                   \
                                           hadc->Init.SamplingTimeCommon2);

      /* Configuration of regular group sequencer:                              */
      /* - if scan mode is disabled, regular channels sequence length is set to */
      /*   0x00: 1 channel converted (channel on regular rank 1)                */
      /*   Parameter "NbrOfConversion" is discarded.                            */
      /*   Note: Scan mode is not present by hardware on this device, but       */
      /*   emulated by software for alignment over all STM32 devices.           */
      /* - if scan mode is enabled, regular channels sequence length is set to  */
      /*   parameter "NbrOfConversion".                                         */
      /*   Channels must be configured into each rank using function            */
      /*   "HAL_ADC_ConfigChannel()".                                           */
      if (hadc->Init.ScanConvMode == ADC4_SCAN_DISABLE)
      {
        /* Set sequencer scan length by clearing ranks above rank 1             */
        /* and do not modify rank 1 value.                                      */
        SET_BIT(hadc->Instance->CHSELR, ADC_CHSELR_SQ2_TO_SQ8);

      }
      else if (hadc->Init.ScanConvMode == ADC4_SCAN_ENABLE)
      {
        /* Count number of ranks available in HAL ADC handle variable */
        uint32_t ADCGroupRegularSequencerRanksCount;

        /* Parse all ranks from 1 to 8 */
        for (ADCGroupRegularSequencerRanksCount = 0UL; ADCGroupRegularSequencerRanksCount < (8UL);                    \
             ADCGroupRegularSequencerRanksCount++)
        {
          /* Check each sequencer rank until value of end of sequence */
          if (((hadc->ADCGroupRegularSequencerRanks >> (ADCGroupRegularSequencerRanksCount * 4UL)) & ADC_CHSELR_SQ1) ==
              ADC_CHSELR_SQ1)
          {
            break;
          }
        }

        if (ADCGroupRegularSequencerRanksCount == 1UL)
        {
          /* Set ADC group regular sequencer:                                   */
          /* Set sequencer scan length by clearing ranks above rank 1           */
          /* and do not modify rank 1 value.                                    */
          SET_BIT(hadc->Instance->CHSELR, ADC_CHSELR_SQ2_TO_SQ8);
        }
        else
        {
          /* Set ADC group regular sequencer:                                   */
          /*  - Set ADC group regular sequencer to value memorized              */
          /*    in HAL ADC handle                                               */
          /*    Note: This value maybe be initialized at a unknown value,       */
          /*          therefore after the first call of "HAL_ADC_Init()",        */
          /*          each rank corresponding to parameter "NbrOfConversion"    */
          /*          must be set using "HAL_ADC_ConfigChannel()".              */
          /*  - Set sequencer scan length by clearing ranks above maximum rank  */
          /*    and do not modify other ranks value.                            */
          MODIFY_REG(hadc->Instance->CHSELR,
                     ADC_CHSELR_SQ_ALL,
                     (ADC_CHSELR_SQ2_TO_SQ8 << (((hadc->Init.NbrOfConversion - 1UL) * ADC4_REGULAR_RANK_2) & 0x1FUL)) \
                     | (hadc->ADCGroupRegularSequencerRanks)
                    );
        }
      }

      /* Check back that ADC registers have effectively been configured to      */
      /* ensure of no potential problem of ADC core IP clocking.                */
      /* Check through register CFGR1 (excluding analog watchdog configuration: */
      /* set into separate dedicated function, and bits of ADC resolution set   */
      /* out of temporary variable 'tmpCFGR1').                                 */
      if ((hadc->Instance->CFGR1 & ~(ADC_CFGR1_AWD1CH | ADC_CFGR1_AWD1EN | ADC_CFGR1_AWD1SGL | ADC_CFGR1_RES))
          == tmpCFGR1)
      {
        /* Set ADC error code to none */
        ADC_CLEAR_ERRORCODE(hadc);

        /* Set the ADC state */
        ADC_STATE_CLR_SET(hadc->State, HAL_ADC_STATE_BUSY_INTERNAL, HAL_ADC_STATE_READY);
      }
      else
      {
        /* Update ADC state machine to error */
        ADC_STATE_CLR_SET(hadc->State, HAL_ADC_STATE_BUSY_INTERNAL, HAL_ADC_STATE_ERROR_INTERNAL);

        /* Set ADC error code to ADC IP internal error */
        SET_BIT(hadc->ErrorCode, HAL_ADC_ERROR_INTERNAL);

        tmp_hal_status = HAL_ERROR;
      }
    }
  }
  else
  {
    /* Update ADC state machine to error */
    SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL);

    tmp_hal_status = HAL_ERROR;
  }

  return tmp_hal_status;
}

/**
  * @brief  Deinitialize the ADC peripheral registers to their default reset
  *         values, with deinitialization of the ADC MSP.
  * @note   For devices with several ADCs: reset of ADC common registers is done
  *         only if all ADCs sharing the same common group are disabled.
  *         (function "HAL_ADC_MspDeInit()" is also called under the same conditions:
  *         all ADC instances use the same core clock at RCC level, disabling
  *         the core clock reset all ADC instances).
  *         If this is not the case, reset of these common parameters reset is
  *         bypassed without error reporting: it can be the intended behavior in
  *         case of reset of a single ADC while the other ADCs sharing the same
  *         common group is still running.
  * @note   By default, HAL_ADC_DeInit() set ADC in mode deep power-down:
  *         this saves more power by reducing leakage currents
  *         and is particularly interesting before entering MCU low-power modes.
  * @param hadc ADC handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ADC_DeInit(ADC_HandleTypeDef *hadc)
{
  HAL_StatusTypeDef tmp_hal_status;

  /* Check ADC handle */
  if (hadc == NULL)
  {
    return HAL_ERROR;
  }

  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));

  /* Set ADC state */
  SET_BIT(hadc->State, HAL_ADC_STATE_BUSY_INTERNAL);

  /* Stop potential conversion on going */
  tmp_hal_status = ADC_ConversionStop(hadc, ADC_REGULAR_INJECTED_GROUP);

  /* Disable ADC peripheral if conversions are effectively stopped */
  if (tmp_hal_status == HAL_OK)
  {
    /* Disable the ADC peripheral */
    tmp_hal_status = ADC_Disable(hadc);

    /* Check if ADC is effectively disabled */
    if (tmp_hal_status == HAL_OK)
    {
      /* Change ADC state */
      hadc->State = HAL_ADC_STATE_READY;
    }
  }

  /* Note: HAL ADC deInit is done independently of ADC conversion stop        */
  /*       and disable return status. In case of status fail, attempt to      */
  /*       perform deinitialization anyway and it is up user code in          */
  /*       in HAL_ADC_MspDeInit() to reset the ADC peripheral using           */
  /*       system RCC hard reset.                                             */

  /* ========== Reset ADC registers ========== */
  /* Reset register IER */
  if (hadc->Instance != ADC4) /* ADC1 or ADC2 */
  {
    __HAL_ADC_DISABLE_IT(hadc, (ADC_IT_AWD3  | ADC_IT_AWD2 | ADC_IT_AWD1 |
                                ADC_IT_JQOVF | ADC_IT_OVR  |
                                ADC_IT_JEOS  | ADC_IT_JEOC |
                                ADC_IT_EOS   | ADC_IT_EOC  |
                                ADC_IT_EOSMP | ADC_IT_RDY));

    /* Reset register ISR */
    __HAL_ADC_CLEAR_FLAG(hadc, (ADC_FLAG_AWD3  | ADC_FLAG_AWD2 | ADC_FLAG_AWD1 |
                                ADC_FLAG_JQOVF | ADC_FLAG_OVR  |
                                ADC_FLAG_JEOS  | ADC_FLAG_JEOC |
                                ADC_FLAG_EOS   | ADC_FLAG_EOC  |
                                ADC_FLAG_EOSMP | ADC_FLAG_RDY));

    /* Reset register CR */
    /* Bits ADC_CR_JADSTP, ADC_CR_ADSTP, ADC_CR_JADSTART, ADC_CR_ADSTART,
    ADC_CR_ADCAL, ADC_CR_ADDIS and ADC_CR_ADEN are in access mode "read-set":
    no direct reset applicable.
    Update CR register to reset value where doable by software */
    CLEAR_BIT(hadc->Instance->CR, ADC_CR_ADVREGEN);
    SET_BIT(hadc->Instance->CR, ADC_CR_DEEPPWD);

    /* Reset register CFGR */
    CLEAR_BIT(hadc->Instance->CFGR1, ADC_CFGR1_AWD1CH  | ADC_CFGR1_JAUTO   | ADC_CFGR1_JAWD1EN |
              ADC_CFGR1_AWD1EN  | ADC_CFGR1_AWD1SGL | ADC_CFGR1_JDISCEN |
              ADC_CFGR1_DISCNUM | ADC_CFGR1_DISCEN  | ADC_CFGR1_AUTDLY  |
              ADC_CFGR1_CONT    | ADC_CFGR1_OVRMOD  |
              ADC_CFGR1_EXTEN   | ADC_CFGR1_EXTSEL  |
              ADC_CFGR1_RES     | ADC_CFGR1_DMNGT);

    /* Reset register CFGR2 */
    CLEAR_BIT(hadc->Instance->CFGR2, ADC_CFGR2_ROVSM  | ADC_CFGR2_TROVS   | ADC_CFGR2_OVSS |
              ADC_CFGR2_OVSR  | ADC_CFGR2_JOVSE | ADC_CFGR2_ROVSE);

    /* Reset register SMPR1 */
    CLEAR_BIT(hadc->Instance->SMPR1, ADC_SMPR1_FIELDS);

    /* Reset register SMPR2 */
    CLEAR_BIT(hadc->Instance->SMPR2, ADC_SMPR2_SMP18 | ADC_SMPR2_SMP17 | ADC_SMPR2_SMP16 |
              ADC_SMPR2_SMP15 | ADC_SMPR2_SMP14 | ADC_SMPR2_SMP13 |
              ADC_SMPR2_SMP12 | ADC_SMPR2_SMP11 | ADC_SMPR2_SMP10);

    /* Reset register LTR1 and HTR1 */
    CLEAR_BIT(hadc->Instance->LTR1, ADC_LTR_LT);
    CLEAR_BIT(hadc->Instance->HTR1, ADC_HTR_HT);

    /* Reset register LTR2 and HTR2*/
    CLEAR_BIT(hadc->Instance->LTR2, ADC_LTR_LT);
    CLEAR_BIT(hadc->Instance->HTR2, ADC_HTR_HT);

    /* Reset register LTR3 and HTR3 */
    CLEAR_BIT(hadc->Instance->LTR3, ADC_LTR_LT);
    CLEAR_BIT(hadc->Instance->HTR3, ADC_HTR_HT);

    /* Reset register SQR1 */
    CLEAR_BIT(hadc->Instance->SQR1, ADC_SQR1_SQ4 | ADC_SQR1_SQ3 | ADC_SQR1_SQ2 |
              ADC_SQR1_SQ1 | ADC_SQR1_L);

    /* Reset register SQR2 */
    CLEAR_BIT(hadc->Instance->SQR2, ADC_SQR2_SQ9 | ADC_SQR2_SQ8 | ADC_SQR2_SQ7 |
              ADC_SQR2_SQ6 | ADC_SQR2_SQ5);

    /* Reset register SQR3 */
    CLEAR_BIT(hadc->Instance->SQR3, ADC_SQR3_SQ14 | ADC_SQR3_SQ13 | ADC_SQR3_SQ12 |
              ADC_SQR3_SQ11 | ADC_SQR3_SQ10);

    /* Reset register SQR4 */
    CLEAR_BIT(hadc->Instance->SQR4, ADC_SQR4_SQ16 | ADC_SQR4_SQ15);

    /* Register JSQR was reset when the ADC was disabled */

    /* Reset register DR */
    /* bits in access mode read only, no direct reset applicable*/

    /* Reset register OFR1 */
    CLEAR_BIT(hadc->Instance->OFR1, ADC_OFR1_SSAT | ADC_OFR1_OFFSET1_CH | ADC_OFR1_OFFSET1);
    /* Reset register OFR2 */
    CLEAR_BIT(hadc->Instance->OFR2, ADC_OFR2_SSAT | ADC_OFR2_OFFSET2_CH | ADC_OFR2_OFFSET2);
    /* Reset register OFR3 */
    CLEAR_BIT(hadc->Instance->OFR3, ADC_OFR3_SSAT | ADC_OFR3_OFFSET3_CH | ADC_OFR3_OFFSET3);
    /* Reset register OFR4 */
    CLEAR_BIT(hadc->Instance->OFR4, ADC_OFR4_SSAT | ADC_OFR4_OFFSET4_CH | ADC_OFR4_OFFSET4);

    /* Reset register GCOMP */
    CLEAR_BIT(hadc->Instance->GCOMP, ADC_GCOMP_GCOMP | ADC_GCOMP_GCOMPCOEFF);

    /* Reset registers JDR1, JDR2, JDR3, JDR4 */
    /* bits in access mode read only, no direct reset applicable*/

    /* Reset register AWD2CR */
    CLEAR_BIT(hadc->Instance->AWD2CR, ADC_AWD2CR_AWD2CH);

    /* Reset register AWD3CR */
    CLEAR_BIT(hadc->Instance->AWD3CR, ADC_AWD3CR_AWD3CH);

    /* Reset register DIFSEL */
    CLEAR_BIT(hadc->Instance->DIFSEL, ADC_DIFSEL_DIFSEL);

    /* Reset register PCSEL */
    CLEAR_BIT(hadc->Instance->PCSEL, ADC_PCSEL_PCSEL);

    /* Reset register CALFACT */
    CLEAR_BIT(hadc->Instance->CALFACT, ADC_CALFACT_CAPTURE_COEF | ADC_CALFACT_LATCH_COEF);
  }
  else
  {
    __HAL_ADC_DISABLE_IT(hadc, (ADC_IT_AWD3  | ADC_IT_AWD2 |
                                ADC_IT_AWD1  | ADC_IT_OVR  |
                                ADC_IT_EOS   | ADC_IT_EOC  |
                                ADC_IT_EOSMP | ADC_IT_RDY));

    /* Reset register ISR */
    __HAL_ADC_CLEAR_FLAG(hadc, (ADC_FLAG_AWD3  | ADC_FLAG_AWD2 |
                                ADC_FLAG_AWD1  | ADC_FLAG_OVR  |
                                ADC_FLAG_EOS   | ADC_FLAG_EOC  |
                                ADC_FLAG_EOSMP | ADC_FLAG_RDY));

    /* Reset register CR */
    /* Bits ADC_CR_ADCAL, ADC_CR_ADSTP, ADC_CR_ADSTART are in access mode     */
    /* "read-set": no direct reset applicable.                                */

    /* Reset register CFGR1 */
    hadc->Instance->CFGR1 &= ~(ADC_CFGR1_AWD1CH   | ADC_CFGR1_AWD1EN  | ADC_CFGR1_AWD1SGL | ADC_CFGR1_DISCEN |
                               ADC4_CFGR1_WAIT    | ADC_CFGR1_CONT    | ADC_CFGR1_OVRMOD  |
                               ADC_CFGR1_EXTEN    | ADC_CFGR1_EXTSEL  | ADC4_CFGR1_ALIGN  | ADC_CFGR1_RES    |
                               ADC4_CFGR1_SCANDIR | ADC4_CFGR1_DMACFG | ADC4_CFGR1_DMAEN);

    /* Reset register CFGR2 */
    /* Note: Update of ADC clock mode is conditioned to ADC state disabled:   */
    /*       already done above.                                              */
    hadc->Instance->CFGR2 &= ~(ADC_CFGR2_ROVSE | ADC4_CFGR2_OVSR |
                               ADC_CFGR2_TROVS | ADC4_CFGR2_LFTRIG);

    /* Reset register SMPR */
    hadc->Instance->SMPR1 &= ~ADC_SMPR1_SMP1;

    /* Reset register TR1 */
    hadc->Instance->AWD1TR &= ~(ADC_AWD1TR_HT1 | ADC_AWD1TR_LT1);

    /* Reset register CHSELR */
    hadc->Instance->CHSELR &= ~(ADC_CHSELR_SQ_ALL);

    /* Reset register DR */
    /* bits in access mode read only, no direct reset applicable */

    /* Reset register CCR */
    ADC12_COMMON->CCR &= ~(ADC_CCR_VBATEN | ADC_CCR_VSENSEEN | ADC_CCR_VREFEN | ADC_CCR_PRESC);
  }

  /* ========== Reset common ADC registers ========== */

  /* Software is allowed to change common parameters only when all the other
     ADCs are disabled.   */
  if (__LL_ADC_IS_ENABLED_ALL_COMMON_INSTANCE(__LL_ADC_COMMON_INSTANCE(hadc->Instance)) == 0UL)
  {
    /* Reset configuration of ADC common register CCR:
      - clock mode: CKMODE, PRESCEN
      - multimode related parameters (when this feature is available): MDMA,
        DMACFG, DELAY, DUAL (set by HAL_ADCEx_MultiModeConfigChannel() API)
      - internal measurement paths: Vbat, temperature sensor, Vref (set into
        HAL_ADC_ConfigChannel() or HAL_ADCEx_InjectedConfigChannel() )
    */
    ADC_CLEAR_COMMON_CONTROL_REGISTER(hadc);
  }

  /* DeInit the low level hardware.
     For example:
    __HAL_RCC_ADC_FORCE_RESET();
    __HAL_RCC_ADC_RELEASE_RESET();
    __HAL_RCC_ADC_CLK_DISABLE();
    Keep in mind that all ADCs use the same clock: disabling
    the clock will reset all ADCs.
  */
#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
  if (hadc->MspDeInitCallback == NULL)
  {
    hadc->MspDeInitCallback = HAL_ADC_MspDeInit; /* Legacy weak MspDeInit  */
  }

  /* DeInit the low level hardware: RCC clock, NVIC */
  hadc->MspDeInitCallback(hadc);
#else
  /* DeInit the low level hardware: RCC clock, NVIC */
  HAL_ADC_MspDeInit(hadc);
#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */

  /* Set ADC error code to none */
  ADC_CLEAR_ERRORCODE(hadc);

  if (hadc->Instance == ADC4)
  {
    /* Reset HAL ADC handle variable */
    hadc->ADCGroupRegularSequencerRanks = 0x00000000UL;
  }
  else
  {
    /* Reset injected channel configuration parameters */
    hadc->InjectionConfig.ContextQueue = 0;
    hadc->InjectionConfig.ChannelCount = 0;
  }

  /* Set ADC state */
  hadc->State = HAL_ADC_STATE_RESET;

  __HAL_UNLOCK(hadc);

  return tmp_hal_status;
}

/**
  * @brief  Initialize the ADC MSP.
  * @param hadc ADC handle
  * @retval None
  */
__weak void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hadc);

  /* NOTE : This function should not be modified. When the callback is needed,
            function HAL_ADC_MspInit must be implemented in the user file.
   */
}

/**
  * @brief  DeInitialize the ADC MSP.
  * @param hadc ADC handle
  * @note   All ADC instances use the same core clock at RCC level, disabling
  *         the core clock reset all ADC instances).
  * @retval None
  */
__weak void HAL_ADC_MspDeInit(ADC_HandleTypeDef *hadc)
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
  *          @arg @ref HAL_ADC_CONVERSION_HALF_CB_ID          ADC conversion DMA half-transfer callback ID
  *          @arg @ref HAL_ADC_LEVEL_OUT_OF_WINDOW_1_CB_ID    ADC analog watchdog 1 callback ID
  *          @arg @ref HAL_ADC_ERROR_CB_ID                    ADC error callback ID
  *          @arg @ref HAL_ADC_INJ_CONVERSION_COMPLETE_CB_ID  ADC group injected conversion complete callback ID
  *          @arg @ref HAL_ADC_INJ_QUEUE_OVEFLOW_CB_ID        ADC group injected context queue overflow callback ID
  *          @arg @ref HAL_ADC_LEVEL_OUT_OF_WINDOW_2_CB_ID    ADC analog watchdog 2 callback ID
  *          @arg @ref HAL_ADC_LEVEL_OUT_OF_WINDOW_3_CB_ID    ADC analog watchdog 3 callback ID
  *          @arg @ref HAL_ADC_END_OF_SAMPLING_CB_ID          ADC end of sampling callback ID
  *          @arg @ref HAL_ADC_MSPINIT_CB_ID                  ADC Msp Init callback ID
  *          @arg @ref HAL_ADC_MSPDEINIT_CB_ID                ADC Msp DeInit callback ID
  *          @arg @ref HAL_ADC_MSPINIT_CB_ID MspInit callback ID
  *          @arg @ref HAL_ADC_MSPDEINIT_CB_ID MspDeInit callback ID
  * @param  pCallback pointer to the Callback function
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ADC_RegisterCallback(ADC_HandleTypeDef *hadc, HAL_ADC_CallbackIDTypeDef CallbackID,
                                           pADC_CallbackTypeDef pCallback)
{
  HAL_StatusTypeDef status = HAL_OK;

  if (pCallback == NULL)
  {
    hadc->ErrorCode |= HAL_ADC_ERROR_INVALID_CALLBACK;

    return HAL_ERROR;
  }

  if ((hadc->State & HAL_ADC_STATE_READY) != 0UL)
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

      case HAL_ADC_INJ_CONVERSION_COMPLETE_CB_ID :
        hadc->InjectedConvCpltCallback = pCallback;
        break;

      case HAL_ADC_INJ_QUEUE_OVEFLOW_CB_ID :
        hadc->InjectedQueueOverflowCallback = pCallback;
        break;

      case HAL_ADC_LEVEL_OUT_OF_WINDOW_2_CB_ID :
        hadc->LevelOutOfWindow2Callback = pCallback;
        break;

      case HAL_ADC_LEVEL_OUT_OF_WINDOW_3_CB_ID :
        hadc->LevelOutOfWindow3Callback = pCallback;
        break;

      case HAL_ADC_END_OF_SAMPLING_CB_ID :
        hadc->EndOfSamplingCallback = pCallback;
        break;

      case HAL_ADC_MSPINIT_CB_ID :
        hadc->MspInitCallback = pCallback;
        break;

      case HAL_ADC_MSPDEINIT_CB_ID :
        hadc->MspDeInitCallback = pCallback;
        break;

      default :
        hadc->ErrorCode |= HAL_ADC_ERROR_INVALID_CALLBACK;
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
        hadc->ErrorCode |= HAL_ADC_ERROR_INVALID_CALLBACK;
        status = HAL_ERROR;
        break;
    }
  }
  else
  {
    hadc->ErrorCode |= HAL_ADC_ERROR_INVALID_CALLBACK;

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
  *          @arg @ref HAL_ADC_CONVERSION_HALF_CB_ID          ADC conversion DMA half-transfer callback ID
  *          @arg @ref HAL_ADC_LEVEL_OUT_OF_WINDOW_1_CB_ID    ADC analog watchdog 1 callback ID
  *          @arg @ref HAL_ADC_ERROR_CB_ID                    ADC error callback ID
  *          @arg @ref HAL_ADC_INJ_CONVERSION_COMPLETE_CB_ID  ADC group injected conversion complete callback ID
  *          @arg @ref HAL_ADC_INJ_QUEUE_OVEFLOW_CB_ID        ADC group injected context queue overflow callback ID
  *          @arg @ref HAL_ADC_LEVEL_OUT_OF_WINDOW_2_CB_ID    ADC analog watchdog 2 callback ID
  *          @arg @ref HAL_ADC_LEVEL_OUT_OF_WINDOW_3_CB_ID    ADC analog watchdog 3 callback ID
  *          @arg @ref HAL_ADC_END_OF_SAMPLING_CB_ID          ADC end of sampling callback ID
  *          @arg @ref HAL_ADC_MSPINIT_CB_ID                  ADC Msp Init callback ID
  *          @arg @ref HAL_ADC_MSPDEINIT_CB_ID                ADC Msp DeInit callback ID
  *          @arg @ref HAL_ADC_MSPINIT_CB_ID MspInit callback ID
  *          @arg @ref HAL_ADC_MSPDEINIT_CB_ID MspDeInit callback ID
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ADC_UnRegisterCallback(ADC_HandleTypeDef *hadc, HAL_ADC_CallbackIDTypeDef CallbackID)
{
  HAL_StatusTypeDef status = HAL_OK;

  if ((hadc->State & HAL_ADC_STATE_READY) != 0UL)
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

      case HAL_ADC_INJ_CONVERSION_COMPLETE_CB_ID :
        hadc->InjectedConvCpltCallback = HAL_ADCEx_InjectedConvCpltCallback;
        break;

      case HAL_ADC_INJ_QUEUE_OVEFLOW_CB_ID :
        hadc->InjectedQueueOverflowCallback = HAL_ADCEx_InjectedQueueOverflowCallback;
        break;

      case HAL_ADC_LEVEL_OUT_OF_WINDOW_2_CB_ID :
        hadc->LevelOutOfWindow2Callback = HAL_ADCEx_LevelOutOfWindow2Callback;
        break;

      case HAL_ADC_LEVEL_OUT_OF_WINDOW_3_CB_ID :
        hadc->LevelOutOfWindow3Callback = HAL_ADCEx_LevelOutOfWindow3Callback;
        break;

      case HAL_ADC_END_OF_SAMPLING_CB_ID :
        hadc->EndOfSamplingCallback = HAL_ADCEx_EndOfSamplingCallback;
        break;

      case HAL_ADC_MSPINIT_CB_ID :
        hadc->MspInitCallback = HAL_ADC_MspInit; /* Legacy weak MspInit              */
        break;

      case HAL_ADC_MSPDEINIT_CB_ID :
        hadc->MspDeInitCallback = HAL_ADC_MspDeInit; /* Legacy weak MspDeInit            */
        break;

      default :
        hadc->ErrorCode |= HAL_ADC_ERROR_INVALID_CALLBACK;
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
        hadc->ErrorCode |= HAL_ADC_ERROR_INVALID_CALLBACK;
        status =  HAL_ERROR;
        break;
    }
  }
  else
  {
    hadc->ErrorCode |= HAL_ADC_ERROR_INVALID_CALLBACK;
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
  * @note   Case of multimode enabled (when multimode feature is available):
  *           if ADC is Slave, ADC is enabled but conversion is not started,
  *           if ADC is master, ADC is enabled and multimode conversion is started.
  * @param hadc ADC handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ADC_Start(ADC_HandleTypeDef *hadc)
{
  HAL_StatusTypeDef tmp_hal_status;

  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));

  /* Perform ADC enable and conversion start if no conversion is on going */
  if (LL_ADC_REG_IsConversionOngoing(hadc->Instance) == 0UL)
  {
    __HAL_LOCK(hadc);

    /* Enable the ADC peripheral */
    tmp_hal_status = ADC_Enable(hadc);

    /* Start conversion if ADC is effectively enabled */
    if (tmp_hal_status == HAL_OK)
    {
      /* Set ADC state                                                        */
      /* - Clear state bitfield related to regular group conversion results   */
      /* - Set state bitfield related to regular operation                    */
      ADC_STATE_CLR_SET(hadc->State,
                        HAL_ADC_STATE_READY | HAL_ADC_STATE_REG_EOC | HAL_ADC_STATE_REG_OVR | HAL_ADC_STATE_REG_EOSMP,
                        HAL_ADC_STATE_REG_BUSY);

      if (hadc->Instance != ADC4) /* ADC1 or ADC2 */
      {
        /* Set ADC error code */
        /* Check if a conversion is on going on ADC group injected */
        if (HAL_IS_BIT_SET(hadc->State, HAL_ADC_STATE_INJ_BUSY))
        {
          /* Reset ADC error code fields related to regular conversions only */
          CLEAR_BIT(hadc->ErrorCode, (HAL_ADC_ERROR_OVR | HAL_ADC_ERROR_DMA));
        }
        else
        {
          /* Reset all ADC error code fields */
          ADC_CLEAR_ERRORCODE(hadc);
        }
      }
      else
      {
        /* Set ADC error code */
        /* Reset all ADC error code fields */
        ADC_CLEAR_ERRORCODE(hadc);
      }

      /* Clear ADC group regular conversion flag and overrun flag               */
      /* (To ensure of no unknown state from potential previous ADC operations) */
      __HAL_ADC_CLEAR_FLAG(hadc, (ADC_FLAG_EOC | ADC_FLAG_EOS | ADC_FLAG_OVR));

      /* Unlock before starting ADC conversions: in case of potential         */
      /* interruption, to let the process to ADC IRQ Handler.                 */
      __HAL_UNLOCK(hadc);

      /* Enable conversion of regular group.                                  */
      /* If software start has been selected, conversion starts immediately.  */
      /* If external trigger has been selected, conversion will start at next */
      /* trigger event.                                                       */
      /* Case of multimode enabled (when multimode feature is available):     */
      /*  - if ADC is slave and dual regular conversions are enabled, ADC is  */
      /*    enabled only (conversion is not started),                         */
      /*  - if ADC is master, ADC is enabled and conversion is started.       */

      if (hadc->Instance != ADC4) /* ADC1 or ADC2 */
      {
        if (READ_BIT(hadc->Instance->CFGR1, ADC_CFGR1_JAUTO) != 0UL)
        {
          ADC_STATE_CLR_SET(hadc->State, HAL_ADC_STATE_INJ_EOC, HAL_ADC_STATE_INJ_BUSY);
        }
      }

      /* Start ADC group regular conversion */
      LL_ADC_REG_StartConversion(hadc->Instance);
    }
    else
    {
      __HAL_UNLOCK(hadc);
    }
  }
  else
  {
    tmp_hal_status = HAL_BUSY;
  }

  return tmp_hal_status;
}

/**
  * @brief  Stop ADC conversion of regular group (and injected channels in
  *         case of auto_injection mode), disable ADC peripheral.
  * @note:  ADC peripheral disable is forcing stop of potential
  *         conversion on injected group. If injected group is under use, it
  *         should be preliminarily stopped using HAL_ADCEx_InjectedStop function.
  * @param hadc ADC handle
  * @retval HAL status.
  */
HAL_StatusTypeDef HAL_ADC_Stop(ADC_HandleTypeDef *hadc)
{
  HAL_StatusTypeDef tmp_hal_status;

  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));

  __HAL_LOCK(hadc);

  /* 1. Stop potential conversion on going, on ADC groups regular and injected */
  tmp_hal_status = ADC_ConversionStop(hadc, ADC_REGULAR_INJECTED_GROUP);

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
                        HAL_ADC_STATE_REG_BUSY | HAL_ADC_STATE_INJ_BUSY,
                        HAL_ADC_STATE_READY);
    }
  }

  __HAL_UNLOCK(hadc);

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
  * @param hadc ADC handle
  * @param Timeout Timeout value in millisecond.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ADC_PollForConversion(ADC_HandleTypeDef *hadc, uint32_t Timeout)
{
  uint32_t tickstart;
  uint32_t tmp_flag_end;
  uint32_t tmp_cfgr;

  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));

  /* If end of conversion selected to end of sequence conversions */
  if (hadc->Init.EOCSelection == ADC_EOC_SEQ_CONV)
  {
    tmp_flag_end = ADC_FLAG_EOS;
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

    if (hadc->Instance != ADC4)  /* ADC1 or ADC2 */
    {
      /* Check ADC DMA mode */
      if (READ_BIT(hadc->Instance->CFGR1, ADC_CFGR1_DMNGT_0) != 0UL)
      {
        SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_CONFIG);
        return HAL_ERROR;
      }
      else
      {
        tmp_flag_end = (ADC_FLAG_EOC);
      }
    }
    else
    {
      if ((hadc->Instance->CFGR1 & ADC4_CFGR1_DMAEN) != 0UL)
      {
        SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_CONFIG);
        return HAL_ERROR;
      }
      else
      {
        tmp_flag_end = (ADC_FLAG_EOC);
      }
    }
  }

  /* Get tick count */
  tickstart = HAL_GetTick();

  /* Wait until End of unitary conversion or sequence conversions flag is raised */
  while ((hadc->Instance->ISR & tmp_flag_end) == 0UL)
  {
    /* Check if timeout is disabled (set to infinite wait) */
    if (Timeout != HAL_MAX_DELAY)
    {
      if (((HAL_GetTick() - tickstart) > Timeout) || (Timeout == 0UL))
      {
        /* New check to avoid false timeout detection in case of preemption */
        if ((hadc->Instance->ISR & tmp_flag_end) == 0UL)
        {
          /* Update ADC state machine to timeout */
          SET_BIT(hadc->State, HAL_ADC_STATE_TIMEOUT);

          /* Process unlocked */
          __HAL_UNLOCK(hadc);

          return HAL_TIMEOUT;
        }
      }
    }
  }

  /* Update ADC state machine */
  SET_BIT(hadc->State, HAL_ADC_STATE_REG_EOC);

  /* Determine whether any further conversion upcoming on group regular       */
  /* by external trigger, continuous mode or scan sequence on going.          */
  if ((LL_ADC_REG_IsTriggerSourceSWStart(hadc->Instance) != 0UL)
      && (hadc->Init.ContinuousConvMode == DISABLE)
     )
  {
    if (hadc->Instance != ADC4)  /* ADC1 or ADC2 */
    {
      /* Check whether end of sequence is reached */
      if (__HAL_ADC_GET_FLAG(hadc, ADC_FLAG_EOS))
      {
        /* Set ADC state */
        CLEAR_BIT(hadc->State, HAL_ADC_STATE_REG_BUSY);

        if ((hadc->State & HAL_ADC_STATE_INJ_BUSY) == 0UL)
        {
          SET_BIT(hadc->State, HAL_ADC_STATE_READY);
        }
      }

      /* Retrieve handle ADC CFGR register */
      tmp_cfgr = READ_REG(hadc->Instance->CFGR1);

      /* Clear polled flag */
      if (tmp_flag_end == ADC_FLAG_EOS)
      {
        __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_EOS);
      }
      else
      {
        /* Clear end of conversion EOC flag of regular group if low power feature */
        /* "LowPowerAutoWait " is disabled, to not interfere with this feature    */
        /* until data register is read using function HAL_ADC_GetValue().         */
        if (READ_BIT(tmp_cfgr, ADC_CFGR1_AUTDLY) == 0UL)
        {
          __HAL_ADC_CLEAR_FLAG(hadc, (ADC_FLAG_EOC | ADC_FLAG_EOS));
        }
      }
    }
    else
    {
      /* Check whether end of sequence is reached */
      if (__HAL_ADC_GET_FLAG(hadc, ADC_FLAG_EOS))
      {
        /* Allowed to modify bits ADC_IT_EOC/ADC_IT_EOS only if bit             */
        /* ADSTART==0 (no conversion on going)                                  */
        if (LL_ADC_REG_IsConversionOngoing(hadc->Instance) == 0UL)
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

          /* Set ADC error code to ADC IP internal error */
          SET_BIT(hadc->ErrorCode, HAL_ADC_ERROR_INTERNAL);
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

    }
  }

  return HAL_OK;
}

/**
  * @brief  Poll for ADC event.
  * @param hadc ADC handle
  * @param EventType the ADC event type.
  *          This parameter can be one of the following values:
  *            @arg @ref ADC_EOSMP_EVENT  ADC End of Sampling event
  *            @arg @ref ADC_AWD1_EVENT   ADC Analog watchdog 1 event (main analog watchdog,
  *                                       present on all STM32 devices)
  *            @arg @ref ADC_AWD2_EVENT   ADC Analog watchdog 2 event (additional analog watchdog,
  *                                       not present on all STM32 families)
  *            @arg @ref ADC_AWD3_EVENT   ADC Analog watchdog 3 event (additional analog watchdog,
  *                                       not present on all STM32 families)
  *            @arg @ref ADC_OVR_EVENT    ADC Overrun event
  *            @arg @ref ADC_JQOVF_EVENT  ADC Injected context queue overflow event
  * @param Timeout Timeout value in millisecond.
  * @note   The relevant flag is cleared if found to be set, except for ADC_FLAG_OVR.
  *         Indeed, the latter is reset only if hadc->Init.Overrun field is set
  *         to ADC_OVR_DATA_OVERWRITTEN. Otherwise, data register may be potentially overwritten
  *         by a new converted data as soon as OVR is cleared.
  *         To reset OVR flag once the preserved data is retrieved, the user can resort
  *         to macro __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_OVR);
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ADC_PollForEvent(ADC_HandleTypeDef *hadc, uint32_t EventType, uint32_t Timeout)
{
  uint32_t tickstart;

  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  assert_param(IS_ADC_EVENT_TYPE(EventType));

  /* Get tick count */
  tickstart = HAL_GetTick();

  /* Check selected event flag */
  while (__HAL_ADC_GET_FLAG(hadc, EventType) == 0UL)
  {
    /* Check if timeout is disabled (set to infinite wait) */
    if (Timeout != HAL_MAX_DELAY)
    {
      if (((HAL_GetTick() - tickstart) > Timeout) || (Timeout == 0UL))
      {
        /* New check to avoid false timeout detection in case of preemption */
        if (__HAL_ADC_GET_FLAG(hadc, EventType) == 0UL)
        {
          /* Update ADC state machine to timeout */
          SET_BIT(hadc->State, HAL_ADC_STATE_TIMEOUT);

          /* Process unlocked */
          __HAL_UNLOCK(hadc);

          return HAL_TIMEOUT;
        }
      }
    }
  }

  switch (EventType)
  {
    /* End Of Sampling event */
    case ADC_EOSMP_EVENT:
      /* Set ADC state */
      SET_BIT(hadc->State, HAL_ADC_STATE_REG_EOSMP);

      /* Clear the End Of Sampling flag */
      __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_EOSMP);

      break;

    /* Analog watchdog (level out of window) event */
    /* Note: In case of several analog watchdog enabled, if needed to know      */
    /* which one triggered and on which ADCx, test ADC state of analog watchdog */
    /* flags HAL_ADC_STATE_AWD1/2/3 using function "HAL_ADC_GetState()".        */
    /* For example:                                                             */
    /*  " if ((HAL_ADC_GetState(hadc1) & HAL_ADC_STATE_AWD1) != 0UL) "          */
    /*  " if ((HAL_ADC_GetState(hadc1) & HAL_ADC_STATE_AWD2) != 0UL) "          */
    /*  " if ((HAL_ADC_GetState(hadc1) & HAL_ADC_STATE_AWD3) != 0UL) "          */

    /* Check analog watchdog 1 flag */
    case ADC_AWD_EVENT:
      /* Set ADC state */
      SET_BIT(hadc->State, HAL_ADC_STATE_AWD1);

      /* Clear ADC analog watchdog flag */
      __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_AWD1);

      break;

    /* Check analog watchdog 2 flag */
    case ADC_AWD2_EVENT:
      /* Set ADC state */
      SET_BIT(hadc->State, HAL_ADC_STATE_AWD2);

      /* Clear ADC analog watchdog flag */
      __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_AWD2);

      break;

    /* Check analog watchdog 3 flag */
    case ADC_AWD3_EVENT:
      /* Set ADC state */
      SET_BIT(hadc->State, HAL_ADC_STATE_AWD3);

      /* Clear ADC analog watchdog flag */
      __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_AWD3);

      break;

    /* Injected context queue overflow event */
    case ADC_JQOVF_EVENT:
      /* Set ADC state */
      SET_BIT(hadc->State, HAL_ADC_STATE_INJ_JQOVF);

      /* Set ADC error code to Injected context queue overflow */
      SET_BIT(hadc->ErrorCode, HAL_ADC_ERROR_JQOVF);

      /* Clear ADC Injected context queue overflow flag */
      __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_JQOVF);

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
      else
      {
        /* Clear ADC Overrun flag only if Overrun is set to ADC_OVR_DATA_OVERWRITTEN
           otherwise, data register is potentially overwritten by new converted data as soon
           as OVR is cleared. */
        __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_OVR);
      }
      break;
  }

  return HAL_OK;
}

/**
  * @brief  Enable ADC, start conversion of regular group with interruption.
  * @note   Interruptions enabled in this function according to initialization
  *         setting : EOC (end of conversion), EOS (end of sequence),
  *         OVR overrun.
  *         Each of these interruptions has its dedicated callback function.
  * @note   Case of multimode enabled (when multimode feature is available):
  *         HAL_ADC_Start_IT() must be called for ADC Slave first, then for
  *         ADC Master.
  *         For ADC Slave, ADC is enabled only (conversion is not started).
  *         For ADC Master, ADC is enabled and multimode conversion is started.
  * @note   To guarantee a proper reset of all interruptions once all the needed
  *         conversions are obtained, HAL_ADC_Stop_IT() must be called to ensure
  *         a correct stop of the IT-based conversions.
  * @note   By default, HAL_ADC_Start_IT() does not enable the End Of Sampling
  *         interruption. If required (e.g. in case of oversampling with trigger
  *         mode), the user must:
  *          1. first clear the EOSMP flag if set with macro __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_EOSMP)
  *          2. then enable the EOSMP interrupt with macro __HAL_ADC_ENABLE_IT(hadc, ADC_IT_EOSMP)
  *          before calling HAL_ADC_Start_IT().
  * @param hadc ADC handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ADC_Start_IT(ADC_HandleTypeDef *hadc)
{
  HAL_StatusTypeDef tmp_hal_status;

  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));

  /* Perform ADC enable and conversion start if no conversion is on going */
  if (LL_ADC_REG_IsConversionOngoing(hadc->Instance) == 0UL)
  {
    __HAL_LOCK(hadc);

    tmp_hal_status = ADC_Enable(hadc);

    /* Start conversion if ADC is effectively enabled */
    if (tmp_hal_status == HAL_OK)
    {
      /* Set ADC state                                                        */
      /* - Clear state bitfield related to regular group conversion results   */
      /* - Set state bitfield related to regular operation                    */
      ADC_STATE_CLR_SET(hadc->State,
                        HAL_ADC_STATE_READY | HAL_ADC_STATE_REG_EOC | HAL_ADC_STATE_REG_OVR | HAL_ADC_STATE_REG_EOSMP,
                        HAL_ADC_STATE_REG_BUSY);


      /* Set ADC error code */
      /* Check if a conversion is on going on ADC group injected */
      if ((hadc->State & HAL_ADC_STATE_INJ_BUSY) != 0UL)
      {
        /* Reset ADC error code fields related to regular conversions only */
        CLEAR_BIT(hadc->ErrorCode, (HAL_ADC_ERROR_OVR | HAL_ADC_ERROR_DMA));
      }
      else
      {
        /* Reset all ADC error code fields */
        ADC_CLEAR_ERRORCODE(hadc);
      }

      /* Clear ADC group regular conversion flag and overrun flag               */
      /* (To ensure of no unknown state from potential previous ADC operations) */
      __HAL_ADC_CLEAR_FLAG(hadc, (ADC_FLAG_EOC | ADC_FLAG_EOS | ADC_FLAG_OVR));

      /* Unlock before starting ADC conversions: in case of potential         */
      /* interruption, to let the process to ADC IRQ Handler.                 */
      __HAL_UNLOCK(hadc);

      /* Disable all interruptions before enabling the desired ones */
      __HAL_ADC_DISABLE_IT(hadc, (ADC_IT_EOC | ADC_IT_EOS | ADC_IT_OVR));

      /* Enable ADC end of conversion interrupt */
      switch (hadc->Init.EOCSelection)
      {
        case ADC_EOC_SEQ_CONV:
          __HAL_ADC_ENABLE_IT(hadc, ADC_IT_EOS);
          break;
        /* case ADC_EOC_SINGLE_CONV */
        default:
          __HAL_ADC_ENABLE_IT(hadc, ADC_IT_EOC);
          break;
      }

      /* Enable ADC overrun interrupt */
      /* If hadc->Init.Overrun is set to ADC_OVR_DATA_PRESERVED, only then is
         ADC_IT_OVR enabled; otherwise data overwrite is considered as normal
         behavior and no CPU time is lost for a non-processed interruption */
      if (hadc->Init.Overrun == ADC_OVR_DATA_PRESERVED)
      {
        __HAL_ADC_ENABLE_IT(hadc, ADC_IT_OVR);
      }

      /* Enable conversion of regular group.                                  */
      /* If software start has been selected, conversion starts immediately.  */
      /* If external trigger has been selected, conversion will start at next */
      /* trigger event.                                                       */
      /* Case of multimode enabled (when multimode feature is available):     */
      /*  - if ADC is slave and dual regular conversions are enabled, ADC is  */
      /*    enabled only (conversion is not started),                         */
      /*  - if ADC is master, ADC is enabled and conversion is started.       */
      /* ADC instance is not a multimode slave instance with multimode regular conversions enabled */
      if (READ_BIT(hadc->Instance->CFGR1, ADC_CFGR1_JAUTO) != 0UL)
      {
        ADC_STATE_CLR_SET(hadc->State, HAL_ADC_STATE_INJ_EOC, HAL_ADC_STATE_INJ_BUSY);

        /* Enable as well injected interruptions in case
         HAL_ADCEx_InjectedStart_IT() has not been called beforehand. This
         allows to start regular and injected conversions when JAUTO is
         set with a single call to HAL_ADC_Start_IT() */
        switch (hadc->Init.EOCSelection)
        {
          case ADC_EOC_SEQ_CONV:
            __HAL_ADC_DISABLE_IT(hadc, ADC_IT_JEOC);
            __HAL_ADC_ENABLE_IT(hadc, ADC_IT_JEOS);
            break;
          /* case ADC_EOC_SINGLE_CONV */
          default:
            __HAL_ADC_DISABLE_IT(hadc, ADC_IT_JEOS);
            __HAL_ADC_ENABLE_IT(hadc, ADC_IT_JEOC);
            break;
        }
      }

      /* Start ADC group regular conversion */
      LL_ADC_REG_StartConversion(hadc->Instance);

    }
    else
    {
      __HAL_UNLOCK(hadc);
    }

  }
  else
  {
    tmp_hal_status = HAL_BUSY;
  }

  return tmp_hal_status;
}

/**
  * @brief  Stop ADC conversion of regular group (and injected group in
  *         case of auto_injection mode), disable interrution of
  *         end-of-conversion, disable ADC peripheral.
  * @param hadc ADC handle
  * @retval HAL status.
  */
HAL_StatusTypeDef HAL_ADC_Stop_IT(ADC_HandleTypeDef *hadc)
{
  HAL_StatusTypeDef tmp_hal_status;

  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));

  __HAL_LOCK(hadc);

  /* 1. Stop potential conversion on going, on ADC groups regular and injected */
  tmp_hal_status = ADC_ConversionStop(hadc, ADC_REGULAR_INJECTED_GROUP);

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
                        HAL_ADC_STATE_REG_BUSY | HAL_ADC_STATE_INJ_BUSY,
                        HAL_ADC_STATE_READY);
    }
  }

  __HAL_UNLOCK(hadc);

  return tmp_hal_status;
}

/**
  * @brief  Enable ADC, start conversion of regular group and transfer result through DMA.
  * @note   Interruptions enabled in this function:
  *         overrun (if applicable), DMA half transfer, DMA transfer complete.
  *         Each of these interruptions has its dedicated callback function.
  * @note   Case of multimode enabled (when multimode feature is available): HAL_ADC_Start_DMA()
  *         is designed for single-ADC mode only. For multimode, the dedicated
  *         HAL_ADCEx_MultiModeStart_DMA() function must be used.
  * @param hadc ADC handle
  * @param pData Destination Buffer address.
  * @param Length Number of data to be transferred from ADC peripheral to memory
  * @retval HAL status.
  */
HAL_StatusTypeDef HAL_ADC_Start_DMA(ADC_HandleTypeDef *hadc, const uint32_t *pData, uint32_t Length)
{
  HAL_StatusTypeDef tmp_hal_status;
  uint32_t LengthInBytes;

  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));

  /* Perform ADC enable and conversion start if no conversion is on going */
  if (LL_ADC_REG_IsConversionOngoing(hadc->Instance) == 0UL)
  {
    __HAL_LOCK(hadc);

    tmp_hal_status = ADC_Enable(hadc);

    /* Start conversion if ADC is effectively enabled */
    if (tmp_hal_status == HAL_OK)
    {
      /* Set ADC state                                                        */
      /* - Clear state bitfield related to regular group conversion results   */
      /* - Set state bitfield related to regular operation                    */
      ADC_STATE_CLR_SET(hadc->State,
                        HAL_ADC_STATE_READY | HAL_ADC_STATE_REG_EOC | HAL_ADC_STATE_REG_OVR | HAL_ADC_STATE_REG_EOSMP,
                        HAL_ADC_STATE_REG_BUSY);

      if (hadc->Instance != ADC4)  /* ADC1 or ADC2 */
      {
        /* Check if a conversion is on going on ADC group injected */
        if ((hadc->State & HAL_ADC_STATE_INJ_BUSY) != 0UL)
        {
          /* Reset ADC error code fields related to regular conversions only */
          CLEAR_BIT(hadc->ErrorCode, (HAL_ADC_ERROR_OVR | HAL_ADC_ERROR_DMA));
        }
        else
        {
          /* Reset all ADC error code fields */
          ADC_CLEAR_ERRORCODE(hadc);
        }
      }
      else
      {
        /* Reset all ADC error code fields */
        ADC_CLEAR_ERRORCODE(hadc);
      }

      /* Set the DMA transfer complete callback */
      hadc->DMA_Handle->XferCpltCallback = ADC_DMAConvCplt;

      /* Set the DMA half transfer complete callback */
      hadc->DMA_Handle->XferHalfCpltCallback = ADC_DMAHalfConvCplt;

      /* Set the DMA error callback */
      hadc->DMA_Handle->XferErrorCallback = ADC_DMAError;


      /* Manage ADC and DMA start: ADC overrun interruption, DMA start,     */
      /* ADC start (in case of SW start):                                   */

      /* Clear regular group conversion flag and overrun flag               */
      /* (To ensure of no unknown state from potential previous ADC         */
      /* operations)                                                        */
      __HAL_ADC_CLEAR_FLAG(hadc, (ADC_FLAG_EOC | ADC_FLAG_EOS | ADC_FLAG_OVR));

      /* Unlock before starting ADC conversions: in case of potential         */
      /* interruption, to let the process to ADC IRQ Handler.                 */
      __HAL_UNLOCK(hadc);

      /* With DMA, overrun event is always considered as an error even if
         hadc->Init.Overrun is set to ADC_OVR_DATA_OVERWRITTEN. Therefore,
         ADC_IT_OVR is enabled. */
      __HAL_ADC_ENABLE_IT(hadc, ADC_IT_OVR);

      if (hadc->Instance == ADC4)
      {
        /* Enable ADC DMA mode */
        hadc->Instance->CFGR1 |= ADC4_CFGR1_DMAEN;
      }

      /* Length should be converted to number of bytes */
      LengthInBytes = Length * 4U;

      /* Start the DMA channel */
      if ((hadc->DMA_Handle->Mode & DMA_LINKEDLIST) == DMA_LINKEDLIST)
      {
        if ((hadc->DMA_Handle->LinkedListQueue != NULL) && (hadc->DMA_Handle->LinkedListQueue->Head != NULL))
        {
          hadc->DMA_Handle->LinkedListQueue->Head->LinkRegisters[NODE_CBR1_DEFAULT_OFFSET] = (uint32_t)LengthInBytes;
          hadc->DMA_Handle->LinkedListQueue->Head->LinkRegisters[NODE_CSAR_DEFAULT_OFFSET] =                  \
              (uint32_t)&hadc->Instance->DR;
          hadc->DMA_Handle->LinkedListQueue->Head->LinkRegisters[NODE_CDAR_DEFAULT_OFFSET] = (uint32_t)pData;
          tmp_hal_status = HAL_DMAEx_List_Start_IT(hadc->DMA_Handle);
        }
        else
        {
          tmp_hal_status = HAL_ERROR;
        }
      }
      else
      {
        tmp_hal_status = HAL_DMA_Start_IT(hadc->DMA_Handle, (uint32_t)&hadc->Instance->DR, (uint32_t)pData,        \
                                          LengthInBytes);
      }

      /* Enable conversion of regular group.                                  */
      /* If software start has been selected, conversion starts immediately.  */
      /* If external trigger has been selected, conversion will start at next */
      /* trigger event.                                                       */
      /* Start ADC group regular conversion */
      LL_ADC_REG_StartConversion(hadc->Instance);
    }
    else
    {
      __HAL_UNLOCK(hadc);
    }
  }
  else
  {
    tmp_hal_status = HAL_BUSY;
  }

  return tmp_hal_status;
}

/**
  * @brief  Stop ADC conversion of regular group (and injected group in
  *         case of auto_injection mode), disable ADC DMA transfer, disable
  *         ADC peripheral.
  * @note:  ADC peripheral disable is forcing stop of potential
  *         conversion on ADC group injected. If ADC group injected is under use, it
  *         should be preliminarily stopped using HAL_ADCEx_InjectedStop function.
  * @note   Case of multimode enabled (when multimode feature is available):
  *         HAL_ADC_Stop_DMA() function is dedicated to single-ADC mode only.
  *         For multimode, the dedicated HAL_ADCEx_MultiModeStop_DMA() API must be used.
  * @param hadc ADC handle
  * @retval HAL status.
  */
HAL_StatusTypeDef HAL_ADC_Stop_DMA(ADC_HandleTypeDef *hadc)
{
  HAL_StatusTypeDef tmp_hal_status;

  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));

  __HAL_LOCK(hadc);

  /* 1. Stop potential ADC group regular conversion on going */
  tmp_hal_status = ADC_ConversionStop(hadc, ADC_REGULAR_INJECTED_GROUP);

  /* Disable ADC peripheral if conversions are effectively stopped */
  if (tmp_hal_status == HAL_OK)
  {
    if (hadc->Instance != ADC4)  /* ADC1 or ADC2 */
    {
      /* Disable ADC DMA (ADC DMA configuration of continuous requests is kept) */
      MODIFY_REG(hadc->Instance->CFGR1, ADC_CFGR1_DMNGT_0 | ADC_CFGR1_DMNGT_1, 0UL);
    }
    else
    {
      /* Disable ADC DMA (ADC DMA configuration of continuous requests is kept) */
      CLEAR_BIT(hadc->Instance->CFGR1, ADC4_CFGR1_DMAEN);
    }

    /* Disable the DMA channel (in case of DMA in circular mode or stop       */
    /* while DMA transfer is on going)                                        */
    if (hadc->DMA_Handle->State == HAL_DMA_STATE_BUSY)
    {
      tmp_hal_status = HAL_DMA_Abort(hadc->DMA_Handle);

      /* Check if DMA channel effectively disabled */
      if (tmp_hal_status != HAL_OK)
      {
        /* Update ADC state machine to error */
        SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_DMA);
      }
    }

    /* Disable ADC overrun interrupt */
    __HAL_ADC_DISABLE_IT(hadc, ADC_IT_OVR);

    /* 2. Disable the ADC peripheral */
    /* Update "tmp_hal_status" only if DMA channel disabling passed,          */
    /* to keep in memory a potential failing status.                          */
    if (tmp_hal_status == HAL_OK)
    {
      tmp_hal_status = ADC_Disable(hadc);
    }
    else
    {
      (void)ADC_Disable(hadc);
    }

    /* Check if ADC is effectively disabled */
    if (tmp_hal_status == HAL_OK)
    {
      if (hadc->Instance != ADC4)  /* ADC1 or ADC2 */
      {
        /* Set ADC state */
        ADC_STATE_CLR_SET(hadc->State, HAL_ADC_STATE_REG_BUSY | HAL_ADC_STATE_INJ_BUSY, HAL_ADC_STATE_READY);
      }
      else
      {
        /* Set ADC state */
        ADC_STATE_CLR_SET(hadc->State, HAL_ADC_STATE_REG_BUSY, HAL_ADC_STATE_READY);
      }
    }
  }

  __HAL_UNLOCK(hadc);

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
  * @param hadc ADC handle
  * @retval ADC group regular conversion data
  */
uint32_t HAL_ADC_GetValue(ADC_HandleTypeDef *hadc)
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
  * @param hadc ADC handle
  * @retval None
  */
void HAL_ADC_IRQHandler(ADC_HandleTypeDef *hadc)
{
  uint32_t overrun_error = 0UL; /* flag set if overrun occurrence has to be considered as an error */
  uint32_t tmp_isr = hadc->Instance->ISR;
  uint32_t tmp_ier = hadc->Instance->IER;
  uint32_t tmp_adc_inj_is_trigger_source_sw_start;
  uint32_t tmp_adc_reg_is_trigger_source_sw_start;
  uint32_t tmp_cfgr;

  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  assert_param(IS_ADC_EOC_SELECTION(hadc->Init.EOCSelection));

  /* ========== Check End of Sampling flag for ADC group regular ========== */
  if (((tmp_isr & ADC_FLAG_EOSMP) == ADC_FLAG_EOSMP) && ((tmp_ier & ADC_IT_EOSMP) == ADC_IT_EOSMP))
  {
    /* Update state machine on end of sampling status if not in error state */
    if ((hadc->State & HAL_ADC_STATE_ERROR_INTERNAL) == 0UL)
    {
      /* Set ADC state */
      SET_BIT(hadc->State, HAL_ADC_STATE_REG_EOSMP);
    }

    /* End Of Sampling callback */
#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
    hadc->EndOfSamplingCallback(hadc);
#else
    HAL_ADCEx_EndOfSamplingCallback(hadc);
#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */

    /* Clear regular group conversion flag */
    __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_EOSMP);
  }

  /* ====== Check ADC group regular end of unitary conversion sequence conversions ===== */
  if ((((tmp_isr & ADC_FLAG_EOC) == ADC_FLAG_EOC) && ((tmp_ier & ADC_IT_EOC) == ADC_IT_EOC)) ||
      (((tmp_isr & ADC_FLAG_EOS) == ADC_FLAG_EOS) && ((tmp_ier & ADC_IT_EOS) == ADC_IT_EOS)))
  {
    /* Update state machine on conversion status if not in error state */
    if ((hadc->State & HAL_ADC_STATE_ERROR_INTERNAL) == 0UL)
    {
      /* Set ADC state */
      SET_BIT(hadc->State, HAL_ADC_STATE_REG_EOC);
    }

    /* Determine whether any further conversion upcoming on group regular     */
    /* by external trigger, continuous mode or scan sequence on going         */
    /* to disable interruption.                                               */
    if (hadc->Instance != ADC4)  /* ADC1 or ADC2 */
    {
      if (LL_ADC_REG_IsTriggerSourceSWStart(hadc->Instance) != 0UL)
      {
        /* Get relevant register CFGR in ADC instance of ADC master or slave    */
        /* in function of multimode state (for devices with multimode           */
        /* available).                                                          */
        tmp_cfgr = READ_REG(hadc->Instance->CFGR1);

        /* Carry on if continuous mode is disabled */
        if (READ_BIT(tmp_cfgr, ADC_CFGR1_CONT) != ADC_CFGR1_CONT)
        {
          /* If End of Sequence is reached, disable interrupts */
          if (__HAL_ADC_GET_FLAG(hadc, ADC_FLAG_EOS))
          {
            /* Allowed to modify bits ADC_IT_EOC/ADC_IT_EOS only if bit         */
            /* ADSTART==0 (no conversion on going)                              */
            if (LL_ADC_REG_IsConversionOngoing(hadc->Instance) == 0UL)
            {
              /* Disable ADC end of sequence conversion interrupt */
              /* Note: Overrun interrupt was enabled with EOC interrupt in      */
              /* HAL_Start_IT(), but is not disabled here because can be used   */
              /* by overrun IRQ process below.                                  */
              __HAL_ADC_DISABLE_IT(hadc, ADC_IT_EOC | ADC_IT_EOS);

              /* Set ADC state */
              CLEAR_BIT(hadc->State, HAL_ADC_STATE_REG_BUSY);

              if ((hadc->State & HAL_ADC_STATE_INJ_BUSY) == 0UL)
              {
                SET_BIT(hadc->State, HAL_ADC_STATE_READY);
              }
            }
            else
            {
              /* Change ADC state to error state */
              SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL);

              /* Set ADC error code to ADC peripheral internal error */
              SET_BIT(hadc->ErrorCode, HAL_ADC_ERROR_INTERNAL);
            }
          }
        }
      }
    }
    else
    {
      /* Determine whether any further conversion upcoming on group regular     */
      /* by external trigger, continuous mode or scan sequence on going         */
      /* to disable interruption.                                               */
      if ((LL_ADC_REG_IsTriggerSourceSWStart(hadc->Instance) != 0UL)
          && (hadc->Init.ContinuousConvMode == DISABLE)
         )
      {
        /* If End of Sequence is reached, disable interrupts */
        if (__HAL_ADC_GET_FLAG(hadc, ADC_FLAG_EOS))
        {
          /* Allowed to modify bits ADC_IT_EOC/ADC_IT_EOS only if bit           */
          /* ADSTART==0 (no conversion on going)                                */
          if (LL_ADC_REG_IsConversionOngoing(hadc->Instance) == 0UL)
          {
            /* Disable ADC end of single conversion interrupt on group regular */
            /* Note: Overrun interrupt was enabled with EOC interrupt in        */
            /* HAL_Start_IT(), but is not disabled here because can be used     */
            /* by overrun IRQ process below.                                    */
            __HAL_ADC_DISABLE_IT(hadc, ADC_IT_EOC | ADC_IT_EOS);

            /* Set ADC state */
            ADC_STATE_CLR_SET(hadc->State, HAL_ADC_STATE_REG_BUSY, HAL_ADC_STATE_READY);
          }
          else
          {
            /* Change ADC state to error state */
            SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_CONFIG);

            /* Set ADC error code to ADC IP internal error */
            SET_BIT(hadc->ErrorCode, HAL_ADC_ERROR_INTERNAL);
          }
        }
      }
    }

    /* Conversion complete callback */
    /* Note: Into callback function "HAL_ADC_ConvCpltCallback()",             */
    /*       to determine if conversion has been triggered from EOC or EOS,   */
    /*       possibility to use:                                              */
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
    __HAL_ADC_CLEAR_FLAG(hadc, (ADC_FLAG_EOC | ADC_FLAG_EOS));
  }

  if (hadc->Instance != ADC4)  /* ADC1 or ADC2 */
  {
    /* ====== Check ADC group injected end of unitary conversion sequence conversions ===== */
    if ((((tmp_isr & ADC_FLAG_JEOC) == ADC_FLAG_JEOC) && ((tmp_ier & ADC_IT_JEOC) == ADC_IT_JEOC)) ||
        (((tmp_isr & ADC_FLAG_JEOS) == ADC_FLAG_JEOS) && ((tmp_ier & ADC_IT_JEOS) == ADC_IT_JEOS)))
    {
      /* Update state machine on conversion status if not in error state */
      if ((hadc->State & HAL_ADC_STATE_ERROR_INTERNAL) == 0UL)
      {
        /* Set ADC state */
        SET_BIT(hadc->State, HAL_ADC_STATE_INJ_EOC);
      }

      /* Retrieve ADC configuration */
      tmp_adc_inj_is_trigger_source_sw_start = LL_ADC_INJ_IsTriggerSourceSWStart(hadc->Instance);
      tmp_adc_reg_is_trigger_source_sw_start = LL_ADC_REG_IsTriggerSourceSWStart(hadc->Instance);
      /* Get relevant register CFGR in ADC instance of ADC master or slave  */
      /* in function of multimode state (for devices with multimode         */
      /* available).                                                        */

      tmp_cfgr = READ_REG(hadc->Instance->CFGR1);

      /* Disable interruption if no further conversion upcoming by injected     */
      /* external trigger or by automatic injected conversion with regular      */
      /* group having no further conversion upcoming (same conditions as        */
      /* regular group interruption disabling above),                           */
      /* and if injected scan sequence is completed.                            */
      if (tmp_adc_inj_is_trigger_source_sw_start != 0UL)
      {
        if ((READ_BIT(tmp_cfgr, ADC_CFGR1_JAUTO) == 0UL) ||
            ((tmp_adc_reg_is_trigger_source_sw_start != 0UL) &&
             (READ_BIT(tmp_cfgr, ADC_CFGR1_CONT) == 0UL)))
        {
          /* If End of Sequence is reached, disable interrupts */
          if (__HAL_ADC_GET_FLAG(hadc, ADC_FLAG_JEOS))
          {
            /* Particular case if injected contexts queue is enabled:             */
            /* when the last context has been fully processed, JSQR is reset      */
            /* by the hardware. Even if no injected conversion is planned to come */
            /* (queue empty, triggers are ignored), it can start again            */
            /* immediately after setting a new context (JADSTART is still set).   */
            /* Therefore, state of HAL ADC injected group is kept to busy.        */
            /* No ADC_CFGR1_JQM for STM32U5                                       */

            /* Allowed to modify bits ADC_IT_JEOC/ADC_IT_JEOS only if bit       */
            /* JADSTART==0 (no conversion on going)                             */
            if (LL_ADC_INJ_IsConversionOngoing(hadc->Instance) == 0UL)
            {
              /* Disable ADC end of sequence conversion interrupt  */
              __HAL_ADC_DISABLE_IT(hadc, ADC_IT_JEOC | ADC_IT_JEOS);

              /* Set ADC state */
              CLEAR_BIT(hadc->State, HAL_ADC_STATE_INJ_BUSY);

              if ((hadc->State & HAL_ADC_STATE_REG_BUSY) == 0UL)
              {
                SET_BIT(hadc->State, HAL_ADC_STATE_READY);
              }
            }
          }
        }
      }

      /* Injected Conversion complete callback */
      /* Note:  HAL_ADCEx_InjectedConvCpltCallback can resort to
                if( __HAL_ADC_GET_FLAG(&hadc, ADC_FLAG_JEOS)) or
                if( __HAL_ADC_GET_FLAG(&hadc, ADC_FLAG_JEOC)) to determine whether
                interruption has been triggered by end of conversion or end of
                sequence.    */
#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
      hadc->InjectedConvCpltCallback(hadc);
#else
      HAL_ADCEx_InjectedConvCpltCallback(hadc);
#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */

      /* Clear injected group conversion flag */
      __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_JEOC | ADC_FLAG_JEOS);
    }
  } /* Specific ADC1 only */

  /* ========== Check Analog watchdog 1 flag ========== */
  if (((tmp_isr & ADC_FLAG_AWD1) == ADC_FLAG_AWD1) && ((tmp_ier & ADC_IT_AWD1) == ADC_IT_AWD1))
  {
    /* Set ADC state */
    SET_BIT(hadc->State, HAL_ADC_STATE_AWD1);

    /* Level out of window 1 callback */
#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
    hadc->LevelOutOfWindowCallback(hadc);
#else
    HAL_ADC_LevelOutOfWindowCallback(hadc);
#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */

    /* Clear ADC analog watchdog flag */
    __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_AWD1);
  }

  /* ========== Check analog watchdog 2 flag ========== */
  if (((tmp_isr & ADC_FLAG_AWD2) == ADC_FLAG_AWD2) && ((tmp_ier & ADC_IT_AWD2) == ADC_IT_AWD2))
  {
    /* Set ADC state */
    SET_BIT(hadc->State, HAL_ADC_STATE_AWD2);

    /* Level out of window 2 callback */
#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
    hadc->LevelOutOfWindow2Callback(hadc);
#else
    HAL_ADCEx_LevelOutOfWindow2Callback(hadc);
#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */

    /* Clear ADC analog watchdog flag */
    __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_AWD2);
  }

  /* ========== Check analog watchdog 3 flag ========== */
  if (((tmp_isr & ADC_FLAG_AWD3) == ADC_FLAG_AWD3) && ((tmp_ier & ADC_IT_AWD3) == ADC_IT_AWD3))
  {
    /* Set ADC state */
    SET_BIT(hadc->State, HAL_ADC_STATE_AWD3);

    /* Level out of window 3 callback */
#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
    hadc->LevelOutOfWindow3Callback(hadc);
#else
    HAL_ADCEx_LevelOutOfWindow3Callback(hadc);
#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */

    /* Clear ADC analog watchdog flag */
    __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_AWD3);
  }

  /* ========== Check Overrun flag ========== */
  if (((tmp_isr & ADC_FLAG_OVR) == ADC_FLAG_OVR) && ((tmp_ier & ADC_IT_OVR) == ADC_IT_OVR))
  {
    /* If overrun is set to overwrite previous data (default setting),        */
    /* overrun event is not considered as an error.                           */
    /* (cf ref manual "Managing conversions without using the DMA and without */
    /* overrun ")                                                             */
    /* Exception for usage with DMA overrun event always considered as an     */
    /* error.                                                                 */
    if (hadc->Init.Overrun == ADC_OVR_DATA_PRESERVED)
    {
      overrun_error = 1UL;
    }
    else
    {
      /* Check DMA configuration */
      if (hadc->Instance != ADC4)  /* ADC1 or ADC2 */
      {
        /* Multimode not set or feature not available or ADC independent */
        if ((hadc->Instance->CFGR1 & ADC_CFGR1_DMNGT) != 0UL)
        {
          overrun_error = 1UL;
        }
      }
      else
      {
        /* Check DMA configuration */
        if (LL_ADC_REG_GetDMATransfer(hadc->Instance) != LL_ADC_REG_DMA_TRANSFER_NONE_ADC4)
        {
          overrun_error = 1UL;
        }
      }
    }

    if (overrun_error == 1UL)
    {
      /* Change ADC state to error state */
      SET_BIT(hadc->State, HAL_ADC_STATE_REG_OVR);

      /* Set ADC error code to overrun */
      SET_BIT(hadc->ErrorCode, HAL_ADC_ERROR_OVR);

      /* Error callback */
      /* Note: In case of overrun, ADC conversion data is preserved until     */
      /*       flag OVR is reset.                                             */
      /*       Therefore, old ADC conversion data can be retrieved in         */
      /*       function "HAL_ADC_ErrorCallback()".                            */
#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
      hadc->ErrorCallback(hadc);
#else
      HAL_ADC_ErrorCallback(hadc);
#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */
    }

    /* Clear ADC overrun flag */
    __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_OVR);
  }

  if (hadc->Instance != ADC4)  /* ADC1 or ADC2 */
  {
    /* ========== Check Injected context queue overflow flag ========== */
    if (((tmp_isr & ADC_FLAG_JQOVF) == ADC_FLAG_JQOVF) && ((tmp_ier & ADC_IT_JQOVF) == ADC_IT_JQOVF))
    {
      /* Change ADC state to overrun state */
      SET_BIT(hadc->State, HAL_ADC_STATE_INJ_JQOVF);

      /* Set ADC error code to Injected context queue overflow */
      SET_BIT(hadc->ErrorCode, HAL_ADC_ERROR_JQOVF);

      /* Clear the Injected context queue overflow flag */
      __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_JQOVF);

      /* Injected context queue overflow callback */
#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
      hadc->InjectedQueueOverflowCallback(hadc);
#else
      HAL_ADCEx_InjectedQueueOverflowCallback(hadc);
#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */
    }
  } /* Speceific ADC1 only */

}

/**
  * @brief  Conversion complete callback in non-blocking mode.
  * @param hadc ADC handle
  * @retval None
  */
__weak void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hadc);

  /* NOTE : This function should not be modified. When the callback is needed,
            function HAL_ADC_ConvCpltCallback must be implemented in the user file.
   */
}

/**
  * @brief  Conversion DMA half-transfer callback in non-blocking mode.
  * @param hadc ADC handle
  * @retval None
  */
__weak void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hadc);

  /* NOTE : This function should not be modified. When the callback is needed,
            function HAL_ADC_ConvHalfCpltCallback must be implemented in the user file.
  */
}

/**
  * @brief  Analog watchdog 1 callback in non-blocking mode.
  * @param hadc ADC handle
  * @retval None
  */
__weak void HAL_ADC_LevelOutOfWindowCallback(ADC_HandleTypeDef *hadc)
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
  *         (HAL ADC handle parameter "ErrorCode" to state "HAL_ADC_ERROR_OVR"):
  *         - Reinitialize the DMA using function "HAL_ADC_Stop_DMA()".
  *         - If needed, restart a new ADC conversion using function
  *           "HAL_ADC_Start_DMA()"
  *           (this function is also clearing overrun flag)
  * @param hadc ADC handle
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
  *         Vbat/VrefInt/TempSensor.
  *         These internal paths can be disabled using function
  *         HAL_ADC_DeInit().
  * @note   Possibility to update parameters on the fly:
  *         This function initializes channel into ADC group regular,
  *         following calls to this function can be used to reconfigure
  *         some parameters of structure "ADC_ChannelConfTypeDef" on the fly,
  *         without resetting the ADC.
  *         The setting of these parameters is conditioned to ADC state:
  *         Refer to comments of structure "ADC_ChannelConfTypeDef".
  * @param hadc ADC handle
  * @param pConfig Structure of ADC channel assigned to ADC group regular.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ADC_ConfigChannel(ADC_HandleTypeDef *hadc, ADC_ChannelConfTypeDef *pConfig)
{
  HAL_StatusTypeDef tmp_hal_status = HAL_OK;
  uint32_t tmp_offset_shifted;
  uint32_t tmp_config_internal_channel;
  __IO uint32_t wait_loop_index = 0;
  uint32_t tmp_adc_is_conversion_on_going_regular;
  uint32_t tmp_adc_is_conversion_on_going_injected;
  uint32_t tmp_channel;

  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));

  if (hadc->Instance != ADC4)  /* ADC1 or ADC2 */
  {
    assert_param(IS_ADC_REGULAR_RANK(pConfig->Rank));
    assert_param(IS_ADC_SAMPLE_TIME(pConfig->SamplingTime));
    assert_param(IS_ADC_SINGLE_DIFFERENTIAL(pConfig->SingleDiff));
    assert_param(IS_ADC_OFFSET_NUMBER(pConfig->OffsetNumber));
    assert_param(IS_ADC_RANGE(hadc->Instance, ADC_GET_RESOLUTION(hadc), pConfig->Offset));
    /* if ROVSE is set, the value of the OFFSETy_EN bit in ADCx_OFRy register is
    ignored (considered as reset) */
    assert_param(!((pConfig->OffsetNumber != ADC_OFFSET_NONE) && (hadc->Init.OversamplingMode == ENABLE)));

    /* Verification of channel number */
    if (pConfig->SingleDiff != ADC_DIFFERENTIAL_ENDED)
    {
      assert_param(IS_ADC_CHANNEL(pConfig->Channel));
    }
    else
    {
      assert_param(IS_ADC1_DIFF_CHANNEL(pConfig->Channel));
    }
  }
  else
  {
    assert_param(IS_ADC4_SAMPLE_TIME(pConfig->SamplingTime));

    if ((hadc->Init.ScanConvMode == ADC_SCAN_SEQ_FIXED)          ||
        (hadc->Init.ScanConvMode == ADC_SCAN_SEQ_FIXED_BACKWARD))
    {
      assert_param(IS_ADC4_REGULAR_RANK_SEQ_FIXED(pConfig->Rank));
    }
    else
    {
      assert_param(IS_ADC4_REGULAR_NB_CONV(hadc->Init.NbrOfConversion));

      assert_param(IS_ADC4_REGULAR_RANK(pConfig->Rank));
    }
  }

  __HAL_LOCK(hadc);

  /* Parameters update conditioned to ADC state:                              */
  /* Parameters that can be updated when ADC is disabled or enabled without   */
  /* conversion on going on regular group:                                    */
  /*  - Channel number                                                        */
  /*  - Channel rank                                                          */
  if (LL_ADC_REG_IsConversionOngoing(hadc->Instance) == 0UL)
  {
    if (hadc->Instance != ADC4)  /* ADC1 or ADC2 */
    {
      /* ADC channels preselection */
      hadc->Instance->PCSEL |= (1UL << (__LL_ADC_CHANNEL_TO_DECIMAL_NB((uint32_t)pConfig->Channel) & 0x1FUL));

      /* Set ADC group regular sequence: channel on the selected scan sequence rank */
      LL_ADC_REG_SetSequencerRanks(hadc->Instance, pConfig->Rank, pConfig->Channel);

      /* Parameters update conditioned to ADC state:                              */
      /* Parameters that can be updated when ADC is disabled or enabled without   */
      /* conversion on going on regular group:                                    */
      /*  - Channel sampling time                                                 */
      /*  - Channel offset                                                        */
      tmp_adc_is_conversion_on_going_regular = LL_ADC_REG_IsConversionOngoing(hadc->Instance);
      tmp_adc_is_conversion_on_going_injected = LL_ADC_INJ_IsConversionOngoing(hadc->Instance);
      if ((tmp_adc_is_conversion_on_going_regular == 0UL)
          && (tmp_adc_is_conversion_on_going_injected == 0UL)
         )
      {
        /* Set sampling time of the selected ADC channel */
        LL_ADC_SetChannelSamplingTime(hadc->Instance, pConfig->Channel, pConfig->SamplingTime);

        /* Configure the offset: offset enable/disable, channel, offset value */

        /* Shift the offset with respect to the selected ADC resolution. */
        /* Offset has to be left-aligned on bit 11, the LSB (right bits) are set to 0 */
        tmp_offset_shifted = ADC_OFFSET_SHIFT_RESOLUTION(hadc, (uint32_t)pConfig->Offset);

        if (pConfig->OffsetNumber != ADC_OFFSET_NONE)
        {
          /* Set ADC selected offset number */
          LL_ADC_SetOffset(hadc->Instance, pConfig->OffsetNumber, pConfig->Channel, tmp_offset_shifted);
          assert_param(IS_ADC_OFFSET_SIGN(pConfig->OffsetSign));
          assert_param(IS_FUNCTIONAL_STATE(pConfig->OffsetSignedSaturation));
          /* Set ADC selected offset sign */
          LL_ADC_SetOffsetSign(hadc->Instance, pConfig->OffsetNumber, pConfig->OffsetSign);
          /* Set ADC selected offset signed saturation */
          LL_ADC_SetOffsetSignedSaturation(hadc->Instance, pConfig->OffsetNumber,                  \
                                           (pConfig->OffsetSignedSaturation == ENABLE)             \
                                           ? LL_ADC_OFFSET_SIGNED_SATURATION_ENABLE                \
                                           : LL_ADC_OFFSET_SIGNED_SATURATION_DISABLE);
        }
        else
        {
          /* Scan OFR1, OFR2, OFR3, OFR4 to check if the selected channel is enabled.
            If this is the case, offset OFRx is disabled since
            pConfig->OffsetNumber = ADC_OFFSET_NONE. */
          if (((hadc->Instance->OFR1) & ADC_OFR1_OFFSET1_CH) == ADC_OFR_CHANNEL(pConfig->Channel))
          {
            CLEAR_BIT(hadc->Instance->OFR1, ADC_OFR1_SSAT);
          }
          if (((hadc->Instance->OFR2) & ADC_OFR2_OFFSET2_CH) == ADC_OFR_CHANNEL(pConfig->Channel))
          {
            CLEAR_BIT(hadc->Instance->OFR2, ADC_OFR2_SSAT);
          }
          if (((hadc->Instance->OFR3) & ADC_OFR3_OFFSET3_CH) == ADC_OFR_CHANNEL(pConfig->Channel))
          {
            CLEAR_BIT(hadc->Instance->OFR3, ADC_OFR3_SSAT);
          }
          if (((hadc->Instance->OFR4) & ADC_OFR4_OFFSET4_CH) == ADC_OFR_CHANNEL(pConfig->Channel))
          {
            CLEAR_BIT(hadc->Instance->OFR4, ADC_OFR4_SSAT);
          }
        }
      }

      /* Parameters update conditioned to ADC state:                              */
      /* Parameters that can be updated only when ADC is disabled:                */
      /*  - Single or differential mode                                           */
      /*  - Internal measurement channels: Vbat/VrefInt/TempSensor                */
      if (LL_ADC_IsEnabled(hadc->Instance) == 0UL)
      {
        /* Set mode single-ended or differential input of the selected ADC channel */
        LL_ADC_SetChannelSingleDiff(hadc->Instance, pConfig->Channel, pConfig->SingleDiff);

        /* Configuration of differential mode */
        if (pConfig->SingleDiff == ADC_DIFFERENTIAL_ENDED)
        {
          /* Set sampling time of the selected ADC channel */
          /* Note: ADC channel number masked with value "0x1F" to ensure shift value within 32 bits range */
          tmp_channel = __LL_ADC_DECIMAL_NB_TO_CHANNEL((__LL_ADC_CHANNEL_TO_DECIMAL_NB(pConfig->Channel) \
                                                        + 1UL) & 0x1FUL);
          LL_ADC_SetChannelSamplingTime(hadc->Instance, tmp_channel, pConfig->SamplingTime);
        }
        /* Management of internal measurement channels: Vbat/VrefInt/TempSensor.  */
        /* If internal channel selected, enable dedicated internal buffers and    */
        /* paths.                                                                 */
        /* Note: these internal measurement paths can be disabled using           */
        /* HAL_ADC_DeInit().                                                      */

        if (__LL_ADC_IS_CHANNEL_INTERNAL(pConfig->Channel))
        {
          /* Configuration of common ADC parameters                                 */

          tmp_config_internal_channel = LL_ADC_GetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(hadc->Instance));

          /* Software is allowed to change common parameters only when all ADCs   */
          /* of the common group are disabled.                                    */
          if (__LL_ADC_IS_ENABLED_ALL_COMMON_INSTANCE(__LL_ADC_COMMON_INSTANCE(hadc->Instance)) == 0UL)
          {
            /* If the requested internal measurement path has already been enabled, */
            /* bypass the configuration processing.                                 */
            if ((pConfig->Channel == ADC_CHANNEL_TEMPSENSOR)
                && ((tmp_config_internal_channel & LL_ADC_PATH_INTERNAL_TEMPSENSOR) == 0UL))
            {
              if (ADC_TEMPERATURE_SENSOR_INSTANCE(hadc))
              {
                LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(hadc->Instance),
                                               LL_ADC_PATH_INTERNAL_TEMPSENSOR | tmp_config_internal_channel);

                /* Delay for temperature sensor stabilization time */
                /* Wait loop initialization and execution */
                /* Note: Variable divided by 2 to compensate partially              */
                /*       CPU processing cycles, scaling in us split to not          */
                /*       exceed 32 bits register capacity and handle low frequency. */
                wait_loop_index = ((LL_ADC_DELAY_INTERNAL_REGUL_STAB_US / 10UL)                                       \
                                   * ((SystemCoreClock / (100000UL * 2UL)) + 1UL));
                while (wait_loop_index != 0UL)
                {
                  wait_loop_index--;
                }
              }
            }
            else if ((pConfig->Channel == ADC_CHANNEL_VBAT) && ((tmp_config_internal_channel                          \
                                                                 & LL_ADC_PATH_INTERNAL_VBAT) == 0UL))
            {
              if (ADC_BATTERY_VOLTAGE_INSTANCE(hadc))
              {
                LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(hadc->Instance),
                                               LL_ADC_PATH_INTERNAL_VBAT | tmp_config_internal_channel);
              }
            }
            else if ((pConfig->Channel == ADC_CHANNEL_VREFINT)
                     && ((tmp_config_internal_channel & LL_ADC_PATH_INTERNAL_VREFINT) == 0UL))
            {
              if (ADC_VREFINT_INSTANCE(hadc))
              {
                LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(hadc->Instance),
                                               LL_ADC_PATH_INTERNAL_VREFINT | tmp_config_internal_channel);
              }
            }
            else
            {
              /* nothing to do */
            }
          }
          /* If the requested internal measurement path has already been          */
          /* enabled and other ADC of the common group are enabled, internal      */
          /* measurement paths cannot be enabled.                                 */
          else
          {
            /* Update ADC state machine to error */
            SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_CONFIG);

            tmp_hal_status = HAL_ERROR;
          }
        }
      }
    }
    else
    {
      /* Remap Internal Channels for Cut1 vs Cut2 */
      tmp_channel = pConfig->Channel;
      if (HAL_GetREVID() == REV_ID_A) /* STM32U5 silicon Rev.A */
      {
        if (pConfig->Channel == ADC4_CHANNEL_TEMPSENSOR)
        {
          tmp_channel = (LL_ADC_CHANNEL_22 | ADC_CHANNEL_ID_INTERNAL_CH);
        }
        else if (pConfig->Channel == ADC4_CHANNEL_VBAT)
        {
          tmp_channel = (LL_ADC_CHANNEL_23 | ADC_CHANNEL_ID_INTERNAL_CH);
        }
        else if (pConfig->Channel == ADC_CHANNEL_VCORE)
        {
          tmp_channel = (LL_ADC_CHANNEL_VREFINT | LL_ADC_CHANNEL_DIFFERENCIATION_VREFINT_VCORE);
        }
        else if (pConfig->Channel == ADC_CHANNEL_DAC1CH1_ADC4)
        {
          tmp_channel = (LL_ADC_CHANNEL_20 | ADC_CHANNEL_ID_INTERNAL_CH);
        }
        else if (pConfig->Channel == ADC_CHANNEL_DAC1CH2_ADC4)
        {
          tmp_channel = (LL_ADC_CHANNEL_21 | ADC_CHANNEL_ID_INTERNAL_CH);
        }
        else
        {
          tmp_channel = pConfig->Channel;
        }
      }

      /* Configure channel: depending on rank setting, add it or remove it from */
      /* ADC sequencer.                                                         */
      /* If sequencer set to not fully configurable with channel rank set to    */
      /* none, remove the channel from the sequencer.                           */
      /* Otherwise (sequencer set to fully configurable or to to not fully      */
      /* configurable with channel rank to be set), configure the selected      */
      /* channel.                                                               */
      if (pConfig->Rank != ADC4_RANK_NONE)
      {
        /* Regular sequence configuration */
        /* Note: ADC channel configuration requires few ADC clock cycles        */
        /*       to be ready. Processing of ADC settings in this function       */
        /*       induce that a specific wait time is not necessary.             */
        /*       For more details on ADC channel configuration ready,           */
        /*       refer to function "LL_ADC_IsActiveFlag_CCRDY()".               */
        if ((hadc->Init.ScanConvMode == ADC_SCAN_SEQ_FIXED)         ||
            (hadc->Init.ScanConvMode == ADC_SCAN_SEQ_FIXED_BACKWARD))
        {
          /* Sequencer set to not fully configurable:                           */
          /* Set the channel by enabling the corresponding bitfield.            */
          LL_ADC_REG_SetSequencerChAdd(hadc->Instance, tmp_channel);
        }
        else
        {
          /* Sequencer set to fully configurable:                               */
          /* Set the channel by entering it into the selected rank.             */

          /* Memorize the channel set into variable in HAL ADC handle */
          MODIFY_REG(hadc->ADCGroupRegularSequencerRanks,
                     ADC_CHSELR_SQ1 << (pConfig->Rank & 0x1FUL),
                     __LL_ADC_CHANNEL_TO_DECIMAL_NB(tmp_channel) << (pConfig->Rank & 0x1FUL));

          /* If the selected rank is below ADC group regular sequencer length,  */
          /* apply the configuration in ADC register.                           */
          /* Note: Otherwise, configuration is not applied.                     */
          /*       To apply it, parameter'NbrOfConversion' must be increased.   */
          if (((pConfig->Rank >> 2UL) + 1UL) <= hadc->Init.NbrOfConversion)
          {
            if (HAL_GetREVID() <= REV_ID_A) /* STM32U5 silicon Rev.A */
            {
              if (__LL_ADC_CHANNEL_TO_DECIMAL_NB(tmp_channel) >= 20UL)
              {
                tmp_channel = (1UL << (__LL_ADC_CHANNEL_TO_DECIMAL_NB(tmp_channel) - 9UL));
              }
            }
            LL_ADC_REG_SetSequencerRanks(hadc->Instance, pConfig->Rank, tmp_channel);
          }
        }

        /* Set sampling time of the selected ADC channel */
        LL_ADC_SetChannelSamplingTime(hadc->Instance, tmp_channel, pConfig->SamplingTime);


        /* Management of internal measurement channels: VrefInt/TempSensor/Vbat */
        /* internal measurement paths enable: If internal channel selected,     */
        /* enable dedicated internal buffers and path.                          */
        /* Note: these internal measurement paths can be disabled using         */
        /*       HAL_ADC_DeInit() or removing the channel from sequencer with   */
        /*       channel configuration parameter "Rank".                        */
        if (__LL_ADC_IS_CHANNEL_INTERNAL(pConfig->Channel))
        {
          tmp_config_internal_channel = LL_ADC_GetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(hadc->Instance));

          /* If the requested internal measurement path has already been enabled,   */
          /* bypass the configuration processing.                                   */
          if ((pConfig->Channel == ADC4_CHANNEL_TEMPSENSOR)                                                           \
              && ((tmp_config_internal_channel & LL_ADC_PATH_INTERNAL_TEMPSENSOR) == 0UL))
          {
            LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(hadc->Instance),
                                           LL_ADC_PATH_INTERNAL_TEMPSENSOR | tmp_config_internal_channel);

            /* Delay for temperature sensor stabilization time */
            /* Wait loop initialization and execution */
            /* Note: Variable divided by 2 to compensate partially              */
            /*       CPU processing cycles, scaling in us split to not          */
            /*       exceed 32 bits register capacity and handle low frequency. */
            wait_loop_index = ((LL_ADC_DELAY_TEMPSENSOR_STAB_US / 10UL) * (SystemCoreClock / (100000UL * 2UL)));
            while (wait_loop_index != 0UL)
            {
              wait_loop_index--;
            }
          }
          else if ((pConfig->Channel == ADC4_CHANNEL_VBAT) && ((tmp_config_internal_channel                           \
                                                                & LL_ADC_PATH_INTERNAL_VBAT) == 0UL))
          {
            LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(hadc->Instance),
                                           LL_ADC_PATH_INTERNAL_VBAT | tmp_config_internal_channel);
          }
          else if ((pConfig->Channel == ADC_CHANNEL_VREFINT)                                                          \
                   && ((tmp_config_internal_channel & LL_ADC_PATH_INTERNAL_VREFINT) == 0UL))
          {
            LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(hadc->Instance),
                                           LL_ADC_PATH_INTERNAL_VREFINT | tmp_config_internal_channel);
          }
          else if ((pConfig->Channel == ADC_CHANNEL_VCORE)                                                            \
                   && ((tmp_config_internal_channel  & LL_ADC_PATH_INTERNAL_VREFINT) == 0UL))
          {
            if (ADC_VCORE_INSTANCE(hadc))
            {
              LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(hadc->Instance),
                                             LL_ADC_PATH_INTERNAL_VREFINT | tmp_config_internal_channel);
              if (HAL_GetREVID() <= REV_ID_A) /* STM32U5 silicon Rev.A */
              {
                SET_BIT((hadc->Instance->OR), ADC_OR_CHN0SEL);
              }
            }
          }
          else
          {
            /* nothing to do */
          }
          /* If STM32U5 silicon Rev.B, ADC_CHANNEL_DAC1CH1 and ADC_CHANNEL_DAC1CH2 are both on Channel 21
             and selection is done via ADC_OR[0] register */
          if (HAL_GetREVID() == REV_ID_B) /* STM32U5 silicon Rev.B */
          {
            if ((pConfig->Channel == ADC_CHANNEL_DAC1CH2_ADC4)                                                        \
                && ((tmp_config_internal_channel & LL_ADC_PATH_INTERNAL_VREFINT) == 0UL))
            {
              SET_BIT((hadc->Instance->OR), ADC_OR_CHN0SEL);
            }
          }
        }
      }
      else
      {
        /* Regular sequencer configuration */
        /* Note: Case of sequencer set to fully configurable:                   */
        /*       Sequencer rank cannot be disabled, only affected to            */
        /*       another channel.                                               */
        /*       To remove a rank, use parameter 'NbrOfConversion".             */
        if ((hadc->Init.ScanConvMode == ADC_SCAN_SEQ_FIXED)         ||
            (hadc->Init.ScanConvMode == ADC_SCAN_SEQ_FIXED_BACKWARD))
        {
          /* Sequencer set to not fully configurable:                           */
          /* Reset the channel by disabling the corresponding bitfield.         */
          LL_ADC_REG_SetSequencerChRem(hadc->Instance, tmp_channel);
        }

        /* Management of internal measurement channels: Vbat/VrefInt/TempSensor.  */
        /* If internal channel selected, enable dedicated internal buffers and    */
        /* paths.                                                                 */
        if (__LL_ADC_IS_CHANNEL_INTERNAL(pConfig->Channel))
        {
          tmp_config_internal_channel = LL_ADC_GetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(hadc->Instance));

          if (pConfig->Channel == ADC_CHANNEL_TEMPSENSOR)
          {
            LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(hadc->Instance),
                                           ~LL_ADC_PATH_INTERNAL_TEMPSENSOR & tmp_config_internal_channel);
          }
          else if (pConfig->Channel == ADC_CHANNEL_VBAT)
          {
            LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(hadc->Instance),
                                           ~LL_ADC_PATH_INTERNAL_VBAT & tmp_config_internal_channel);
          }
          else if (pConfig->Channel == ADC_CHANNEL_VREFINT)
          {
            LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(hadc->Instance),
                                           ~LL_ADC_PATH_INTERNAL_VREFINT & tmp_config_internal_channel);
          }
          else if (pConfig->Channel == ADC_CHANNEL_VCORE)
          {
            LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(hadc->Instance),
                                           ~LL_ADC_PATH_INTERNAL_VREFINT & tmp_config_internal_channel);
            if (HAL_GetREVID() <= REV_ID_A) /* STM32U5 silicon Rev.A */
            {
              SET_BIT((hadc->Instance->OR), ADC_OR_CHN0SEL);
            }
          }
          else
          {
            /* nothing to do */
          }
          /* If STM32U5 silicon Rev.B, ADC_CHANNEL_DAC1CH1 and ADC_CHANNEL_DAC1CH2 are both on Channel 21
             and selection is done via ADC_OR[0] register */
          if (HAL_GetREVID() == REV_ID_B) /* STM32U5 silicon Rev.B */
          {
            if ((pConfig->Channel == ADC_CHANNEL_DAC1CH2_ADC4)                                                        \
                && ((tmp_config_internal_channel & LL_ADC_PATH_INTERNAL_VREFINT) == 0UL))
            {
              SET_BIT((hadc->Instance->OR), ADC_OR_CHN0SEL);
            }
          }
        }
      }
    }
  }

  /* If a conversion is on going on regular group, no update on regular       */
  /* channel could be done on neither of the channel configuration structure  */
  /* parameters.                                                              */
  else
  {
    /* Update ADC state machine to error */
    SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_CONFIG);
    tmp_hal_status = HAL_ERROR;
  }

  __HAL_UNLOCK(hadc);

  return tmp_hal_status;
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
  * @param hadc ADC handle
  * @param pAnalogWDGConfig Structure of ADC analog watchdog configuration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ADC_AnalogWDGConfig(ADC_HandleTypeDef *hadc, ADC_AnalogWDGConfTypeDef *pAnalogWDGConfig)
{
  HAL_StatusTypeDef tmp_hal_status = HAL_OK;
  uint32_t tmp_awd_high_threshold_shifted;
  uint32_t tmp_awd_low_threshold_shifted;
  uint32_t tmp_adc_is_conversion_on_going_regular;
  uint32_t tmp_adc_is_conversion_on_going_injected;

  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  assert_param(IS_ADC_ANALOG_WATCHDOG_NUMBER(pAnalogWDGConfig->WatchdogNumber));
  assert_param(IS_ADC_ANALOG_WATCHDOG_MODE(pAnalogWDGConfig->WatchdogMode));
  assert_param(IS_FUNCTIONAL_STATE(pAnalogWDGConfig->ITMode));

  if ((pAnalogWDGConfig->WatchdogMode == ADC_ANALOGWATCHDOG_SINGLE_REG)     ||
      (pAnalogWDGConfig->WatchdogMode == ADC_ANALOGWATCHDOG_SINGLE_INJEC)   ||
      (pAnalogWDGConfig->WatchdogMode == ADC_ANALOGWATCHDOG_SINGLE_REGINJEC))
  {
    assert_param(IS_ADC_CHANNEL(pAnalogWDGConfig->Channel));
  }

  /* Verify thresholds range */
  if (hadc->Init.OversamplingMode == ENABLE)
  {
    /* Case of oversampling enabled: thresholds are compared to oversampling
       intermediate computation (after ratio, before shift application) */
    assert_param(IS_ADC_RANGE(hadc->Instance, ADC_GET_RESOLUTION(hadc),
                              pAnalogWDGConfig->HighThreshold / (hadc->Init.Oversampling.Ratio + 1UL)));
    assert_param(IS_ADC_RANGE(hadc->Instance, ADC_GET_RESOLUTION(hadc),
                              pAnalogWDGConfig->LowThreshold / (hadc->Init.Oversampling.Ratio + 1UL)));
  }
  else
  {
    /* Verify if thresholds are within the selected ADC resolution */
    assert_param(IS_ADC_RANGE(hadc->Instance, ADC_GET_RESOLUTION(hadc), pAnalogWDGConfig->HighThreshold));
    assert_param(IS_ADC_RANGE(hadc->Instance, ADC_GET_RESOLUTION(hadc), pAnalogWDGConfig->LowThreshold));
  }

  __HAL_LOCK(hadc);

  /* Parameters update conditioned to ADC state:                              */
  /* Parameters that can be updated when ADC is disabled or enabled without   */
  /* conversion on going on ADC groups regular and injected:                  */
  /*  - Analog watchdog channels                                              */
  /*  - Analog watchdog thresholds                                            */
  if (hadc->Instance != ADC4)  /* ADC1 or ADC2 */
  {
    tmp_adc_is_conversion_on_going_regular = LL_ADC_REG_IsConversionOngoing(hadc->Instance);
    tmp_adc_is_conversion_on_going_injected = LL_ADC_INJ_IsConversionOngoing(hadc->Instance);
  }
  else
  {
    tmp_adc_is_conversion_on_going_regular = LL_ADC_REG_IsConversionOngoing(hadc->Instance);
    tmp_adc_is_conversion_on_going_injected = 0UL; /* No injected channels for ADC4 so force                \
                                                      tmp_adc_is_conversion_on_going_injected to 0 */
  }
  if ((tmp_adc_is_conversion_on_going_regular == 0UL)
      && (tmp_adc_is_conversion_on_going_injected == 0UL)
     )
  {
    /* Analog watchdog configuration */
    if (pAnalogWDGConfig->WatchdogNumber == ADC_ANALOGWATCHDOG_1)
    {
      /* Configuration of analog watchdog:                                    */
      /*  - Set the analog watchdog enable mode: one or overall group of      */
      /*    channels, on groups regular and-or injected.                      */
      switch (pAnalogWDGConfig->WatchdogMode)
      {
        case ADC_ANALOGWATCHDOG_SINGLE_REG:
          LL_ADC_SetAnalogWDMonitChannels(hadc->Instance, LL_ADC_AWD1,
                                          __LL_ADC_ANALOGWD_CHANNEL_GROUP(pAnalogWDGConfig->Channel,
                                                                          LL_ADC_GROUP_REGULAR));
          break;

        case ADC_ANALOGWATCHDOG_SINGLE_INJEC:
          LL_ADC_SetAnalogWDMonitChannels(hadc->Instance, LL_ADC_AWD1,
                                          __LL_ADC_ANALOGWD_CHANNEL_GROUP(pAnalogWDGConfig->Channel,
                                                                          LL_ADC_GROUP_INJECTED));
          break;

        case ADC_ANALOGWATCHDOG_SINGLE_REGINJEC:
          LL_ADC_SetAnalogWDMonitChannels(hadc->Instance, LL_ADC_AWD1,
                                          __LL_ADC_ANALOGWD_CHANNEL_GROUP(pAnalogWDGConfig->Channel,
                                                                          LL_ADC_GROUP_REGULAR_INJECTED));
          break;

        case ADC_ANALOGWATCHDOG_ALL_REG:
          LL_ADC_SetAnalogWDMonitChannels(hadc->Instance, LL_ADC_AWD1, LL_ADC_AWD_ALL_CHANNELS_REG);
          break;

        case ADC_ANALOGWATCHDOG_ALL_INJEC:
          LL_ADC_SetAnalogWDMonitChannels(hadc->Instance, LL_ADC_AWD1, LL_ADC_AWD_ALL_CHANNELS_INJ);
          break;

        case ADC_ANALOGWATCHDOG_ALL_REGINJEC:
          LL_ADC_SetAnalogWDMonitChannels(hadc->Instance, LL_ADC_AWD1, LL_ADC_AWD_ALL_CHANNELS_REG_INJ);
          break;

        default: /* ADC_ANALOGWATCHDOG_NONE */
          LL_ADC_SetAnalogWDMonitChannels(hadc->Instance, LL_ADC_AWD1, LL_ADC_AWD_DISABLE);
          break;
      }

      if (hadc->Instance != ADC4)  /* ADC1 or ADC2 */
      {
        /* Set the filtering configuration */
        assert_param(IS_ADC_ANALOG_WATCHDOG_FILTERING_MODE(pAnalogWDGConfig->FilteringConfig));
        LL_ADC_SetAWDFilteringConfiguration(hadc->Instance, hadc->Instance->HTR1, pAnalogWDGConfig->FilteringConfig);
      }

      /* Shift the offset in function of the selected ADC resolution:         */
      /* Thresholds have to be left-aligned on bit 11, the LSB (right bits)   */
      /* are set to 0                                                         */
      tmp_awd_high_threshold_shifted = ADC_AWD1THRESHOLD_SHIFT_RESOLUTION(hadc, pAnalogWDGConfig->HighThreshold);
      tmp_awd_low_threshold_shifted  = ADC_AWD1THRESHOLD_SHIFT_RESOLUTION(hadc, pAnalogWDGConfig->LowThreshold);

      /* Set ADC analog watchdog thresholds value of both thresholds high and low */
      LL_ADC_ConfigAnalogWDThresholds(hadc->Instance, pAnalogWDGConfig->WatchdogNumber, tmp_awd_high_threshold_shifted,
                                      tmp_awd_low_threshold_shifted);

      /* Update state, clear previous result related to AWD1 */
      CLEAR_BIT(hadc->State, HAL_ADC_STATE_AWD1);

      /* Clear flag ADC analog watchdog */
      /* Note: Flag cleared Clear the ADC Analog watchdog flag to be ready  */
      /* to use for HAL_ADC_IRQHandler() or HAL_ADC_PollForEvent()          */
      /* (in case left enabled by previous ADC operations).                 */
      LL_ADC_ClearFlag_AWD1(hadc->Instance);

      /* Configure ADC analog watchdog interrupt */
      if (pAnalogWDGConfig->ITMode == ENABLE)
      {
        LL_ADC_EnableIT_AWD1(hadc->Instance);
      }
      else
      {
        LL_ADC_DisableIT_AWD1(hadc->Instance);
      }
    }
    /* Case of ADC_ANALOGWATCHDOG_2 or ADC_ANALOGWATCHDOG_3 */
    else
    {
      switch (pAnalogWDGConfig->WatchdogMode)
      {
        case ADC_ANALOGWATCHDOG_SINGLE_REG:
        case ADC_ANALOGWATCHDOG_SINGLE_INJEC:
        case ADC_ANALOGWATCHDOG_SINGLE_REGINJEC:
          /* Update AWD by bitfield to keep the possibility to monitor        */
          /* several channels by successive calls of this function.           */
          if (pAnalogWDGConfig->WatchdogNumber == ADC_ANALOGWATCHDOG_2)
          {
            SET_BIT(hadc->Instance->AWD2CR, (1UL                                                       \
                                             << (__LL_ADC_CHANNEL_TO_DECIMAL_NB(pAnalogWDGConfig->Channel) & 0x1FUL)));
          }
          else
          {
            SET_BIT(hadc->Instance->AWD3CR, (1UL                                                      \
                                             << (__LL_ADC_CHANNEL_TO_DECIMAL_NB(pAnalogWDGConfig->Channel) & 0x1FUL)));
          }
          break;

        case ADC_ANALOGWATCHDOG_ALL_REG:
        case ADC_ANALOGWATCHDOG_ALL_INJEC:
        case ADC_ANALOGWATCHDOG_ALL_REGINJEC:
          LL_ADC_SetAnalogWDMonitChannels(hadc->Instance, pAnalogWDGConfig->WatchdogNumber,        \
                                          LL_ADC_AWD_ALL_CHANNELS_REG_INJ);
          break;

        default: /* ADC_ANALOGWATCHDOG_NONE */
          LL_ADC_SetAnalogWDMonitChannels(hadc->Instance, pAnalogWDGConfig->WatchdogNumber, LL_ADC_AWD_DISABLE);
          break;
      }

      /* Shift the thresholds in function of the selected ADC resolution      */
      /* have to be left-aligned on bit 15, the LSB (right bits) are set to 0 */
      tmp_awd_high_threshold_shifted = ADC_AWD23THRESHOLD_SHIFT_RESOLUTION(hadc, pAnalogWDGConfig->HighThreshold);
      tmp_awd_low_threshold_shifted  = ADC_AWD23THRESHOLD_SHIFT_RESOLUTION(hadc, pAnalogWDGConfig->LowThreshold);

      if (hadc->Instance == ADC4)
      {
        /* Set ADC analog watchdog thresholds value of both thresholds high and low */
        LL_ADC_ConfigAnalogWDThresholds(hadc->Instance, pAnalogWDGConfig->WatchdogNumber,
                                        tmp_awd_high_threshold_shifted,
                                        tmp_awd_low_threshold_shifted);
      }
      else /* ADC1 or ADC2 */
      {
        if (pAnalogWDGConfig->WatchdogNumber == ADC_ANALOGWATCHDOG_2)
        {
          /* Set ADC analog watchdog thresholds value of both thresholds high and low */
          MODIFY_REG(hadc->Instance->LTR2,  ADC_LTR_LT, tmp_awd_low_threshold_shifted);
          MODIFY_REG(hadc->Instance->HTR2,  ADC_HTR_HT, tmp_awd_high_threshold_shifted);
        }
        else
        {
          /* Set ADC analog watchdog thresholds value of both thresholds high and low */
          MODIFY_REG(hadc->Instance->LTR3,  ADC_LTR_LT, tmp_awd_low_threshold_shifted);
          MODIFY_REG(hadc->Instance->HTR3,  ADC_HTR_HT, tmp_awd_high_threshold_shifted);
        }
      }

      if (pAnalogWDGConfig->WatchdogNumber == ADC_ANALOGWATCHDOG_2)
      {
        /* Update state, clear previous result related to AWD2 */
        CLEAR_BIT(hadc->State, HAL_ADC_STATE_AWD2);

        /* Clear flag ADC analog watchdog */
        /* Note: Flag cleared Clear the ADC Analog watchdog flag to be ready  */
        /* to use for HAL_ADC_IRQHandler() or HAL_ADC_PollForEvent()          */
        /* (in case left enabled by previous ADC operations).                 */
        LL_ADC_ClearFlag_AWD2(hadc->Instance);

        /* Configure ADC analog watchdog interrupt */
        if (pAnalogWDGConfig->ITMode == ENABLE)
        {
          LL_ADC_EnableIT_AWD2(hadc->Instance);
        }
        else
        {
          LL_ADC_DisableIT_AWD2(hadc->Instance);
        }
      }
      /* (AnalogWDGConfig->WatchdogNumber == ADC_ANALOGWATCHDOG_3) */
      else
      {
        /* Update state, clear previous result related to AWD3 */
        CLEAR_BIT(hadc->State, HAL_ADC_STATE_AWD3);

        /* Clear flag ADC analog watchdog */
        /* Note: Flag cleared Clear the ADC Analog watchdog flag to be ready  */
        /* to use for HAL_ADC_IRQHandler() or HAL_ADC_PollForEvent()          */
        /* (in case left enabled by previous ADC operations).                 */
        LL_ADC_ClearFlag_AWD3(hadc->Instance);

        /* Configure ADC analog watchdog interrupt */
        if (pAnalogWDGConfig->ITMode == ENABLE)
        {
          LL_ADC_EnableIT_AWD3(hadc->Instance);
        }
        else
        {
          LL_ADC_DisableIT_AWD3(hadc->Instance);
        }
      }
    }

  }
  /* If a conversion is on going on ADC group regular or injected, no update  */
  /* could be done on neither of the AWD configuration structure parameters.  */
  else
  {
    /* Update ADC state machine to error */
    SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_CONFIG);

    tmp_hal_status = HAL_ERROR;
  }
  __HAL_UNLOCK(hadc);

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
  *           " if ((HAL_ADC_GetState(hadc1) & HAL_ADC_STATE_REG_BUSY) != 0UL) "
  *           " if ((HAL_ADC_GetState(hadc1) & HAL_ADC_STATE_AWD1) != 0UL) "
  * @param hadc ADC handle
  * @retval ADC handle state (bitfield on 32 bits)
  */
uint32_t HAL_ADC_GetState(ADC_HandleTypeDef *hadc)
{
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));

  /* Return ADC handle state */
  return hadc->State;
}

/**
  * @brief  Return the ADC error code.
  * @param hadc ADC handle
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
  * @brief  Stop ADC conversion.
  * @param hadc ADC handle
  * @param ConversionGroup ADC group regular and/or injected.
  *          This parameter can be one of the following values:
  *            @arg @ref ADC_REGULAR_GROUP           ADC regular conversion type.
  *            @arg @ref ADC_INJECTED_GROUP          ADC injected conversion type.
  *            @arg @ref ADC_REGULAR_INJECTED_GROUP  ADC regular and injected conversion type.
  * @retval HAL status.
  */
HAL_StatusTypeDef ADC_ConversionStop(ADC_HandleTypeDef *hadc, uint32_t ConversionGroup)
{
  uint32_t tickstart;
  uint32_t conversion_timeout_cpu_cycles = 0UL;
  uint32_t conversion_group_reassigned = ConversionGroup;
  uint32_t tmp_adc_cr_adstart_jadstart;
  uint32_t tmp_adc_is_conversion_on_going_regular;
  uint32_t tmp_adc_is_conversion_on_going_injected;

  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  assert_param(IS_ADC_CONVERSION_GROUP(ConversionGroup));

  /* Verification if ADC is not already stopped (on regular and injected      */
  /* groups) to bypass this function if not needed.                           */
  tmp_adc_is_conversion_on_going_regular = LL_ADC_REG_IsConversionOngoing(hadc->Instance);
  tmp_adc_is_conversion_on_going_injected = LL_ADC_INJ_IsConversionOngoing(hadc->Instance);
  if ((tmp_adc_is_conversion_on_going_regular != 0UL) || (tmp_adc_is_conversion_on_going_injected != 0UL))
  {
    /* Particular case of continuous auto-injection mode combined with        */
    /* auto-delay mode.                                                       */
    /* In auto-injection mode, regular group stop ADC_CR_ADSTP is used (not   */
    /* injected group stop ADC_CR_JADSTP).                                    */
    /* Procedure to be followed: Wait until JEOS=1, clear JEOS, set ADSTP=1   */
    /* (see reference manual).                                                */
    if (((hadc->Instance->CFGR1 & ADC_CFGR1_JAUTO) != 0UL)
        && (hadc->Init.ContinuousConvMode == ENABLE)
        && (hadc->Init.LowPowerAutoWait == ENABLE)
       )
    {
      /* Use stop of regular group */
      conversion_group_reassigned = ADC_REGULAR_GROUP;

      /* Wait until JEOS=1 (maximum Timeout: 4 injected conversions) */
      while (__HAL_ADC_GET_FLAG(hadc, ADC_FLAG_JEOS) == 0UL)
      {
        if (conversion_timeout_cpu_cycles >= (ADC_CONVERSION_TIME_MAX_CPU_CYCLES * 4UL))
        {
          /* Update ADC state machine to error */
          SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL);

          /* Set ADC error code to ADC peripheral internal error */
          SET_BIT(hadc->ErrorCode, HAL_ADC_ERROR_INTERNAL);

          return HAL_ERROR;
        }
        conversion_timeout_cpu_cycles++;
      }

      /* Clear JEOS */
      __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_JEOS);
    }

    /* Stop potential conversion on going on ADC group regular */
    if (conversion_group_reassigned != ADC_INJECTED_GROUP)
    {
      /* Software is allowed to set ADSTP only when ADSTART=1 and ADDIS=0 */
      if (LL_ADC_REG_IsConversionOngoing(hadc->Instance) != 0UL)
      {
        if (LL_ADC_IsDisableOngoing(hadc->Instance) == 0UL)
        {
          /* Stop ADC group regular conversion */
          LL_ADC_REG_StopConversion(hadc->Instance);
        }
      }
    }

    /* Stop potential conversion on going on ADC group injected */
    if (conversion_group_reassigned != ADC_REGULAR_GROUP)
    {
      /* Software is allowed to set JADSTP only when JADSTART=1 and ADDIS=0 */
      if (LL_ADC_INJ_IsConversionOngoing(hadc->Instance) != 0UL)
      {
        if (LL_ADC_IsDisableOngoing(hadc->Instance) == 0UL)
        {
          /* Stop ADC group injected conversion */
          LL_ADC_INJ_StopConversion(hadc->Instance);
        }
      }
    }

    /* Selection of start and stop bits with respect to the regular or injected group */
    switch (conversion_group_reassigned)
    {
      case ADC_REGULAR_INJECTED_GROUP:
        tmp_adc_cr_adstart_jadstart = (ADC_CR_ADSTART | ADC_CR_JADSTART);
        break;
      case ADC_INJECTED_GROUP:
        tmp_adc_cr_adstart_jadstart = ADC_CR_JADSTART;
        break;
      /* Case ADC_REGULAR_GROUP only*/
      default:
        tmp_adc_cr_adstart_jadstart = ADC_CR_ADSTART;
        break;
    }

    /* Wait for conversion effectively stopped */
    tickstart = HAL_GetTick();

    while ((hadc->Instance->CR & tmp_adc_cr_adstart_jadstart) != 0UL)
    {
      if ((HAL_GetTick() - tickstart) > ADC_STOP_CONVERSION_TIMEOUT)
      {
        /* New check to avoid false timeout detection in case of preemption */
        if ((hadc->Instance->CR & tmp_adc_cr_adstart_jadstart) != 0UL)
        {
          /* Update ADC state machine to error */
          SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL);

          /* Set ADC error code to ADC peripheral internal error */
          SET_BIT(hadc->ErrorCode, HAL_ADC_ERROR_INTERNAL);

          return HAL_ERROR;
        }
      }
    }
  }

  return HAL_OK;
}

/**
  * @brief  Enable the selected ADC.
  * @note   Prerequisite condition to use this function: ADC must be disabled
  *         and voltage regulator must be enabled (done into HAL_ADC_Init()).
  * @param hadc ADC handle
  * @retval HAL status.
  */
HAL_StatusTypeDef ADC_Enable(ADC_HandleTypeDef *hadc)
{
  uint32_t tickstart;

  /* ADC enable and wait for ADC ready (in case of ADC is disabled or         */
  /* enabling phase not yet completed: flag ADC ready not yet set).           */
  /* Timeout implemented to not be stuck if ADC cannot be enabled (possible   */
  /* causes: ADC clock not running, ...).                                     */
  if (LL_ADC_IsEnabled(hadc->Instance) == 0UL)
  {
    /* Check if conditions to enable the ADC are fulfilled */
    if ((hadc->Instance->CR & (ADC_CR_ADCAL | ADC_CR_JADSTP | ADC_CR_ADSTP | ADC_CR_JADSTART | ADC_CR_ADSTART
                               | ADC_CR_ADDIS | ADC_CR_ADEN)) != 0UL)
    {
      /* Update ADC state machine to error */
      SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL);

      /* Set ADC error code to ADC peripheral internal error */
      SET_BIT(hadc->ErrorCode, HAL_ADC_ERROR_INTERNAL);

      return HAL_ERROR;
    }

    LL_ADC_Enable(hadc->Instance);

    /* If low power mode AutoPowerOff is enabled, power-on/off phases are     */
    /* performed automatically by hardware and flag ADC ready is not set.     */
    if (hadc->Init.LowPowerAutoPowerOff == ADC_LOW_POWER_NONE)
    {
      /* Wait for ADC effectively enabled */
      tickstart = HAL_GetTick();
      /* Poll for ADC ready flag raised except case of multimode enabled
        and ADC slave selected. */
      while (__HAL_ADC_GET_FLAG(hadc, ADC_FLAG_RDY) == 0UL)
      {
        /*  If ADEN bit is set less than 4 ADC clock cycles after the ADCAL bit
            has been cleared (after a calibration), ADEN bit is reset by the
            calibration logic.
            The workaround is to continue setting ADEN until ADRDY is becomes 1.
            Additionally, ADC_ENABLE_TIMEOUT is defined to encompass this
            4 ADC clock cycle duration */
        /* Note: Test of ADC enabled required due to hardware constraint to     */
        /*       not enable ADC if already enabled.                             */
        if (LL_ADC_IsEnabled(hadc->Instance) == 0UL)
        {
          LL_ADC_Enable(hadc->Instance);
        }

        if ((HAL_GetTick() - tickstart) > ADC_ENABLE_TIMEOUT)
        {
          /* New check to avoid false timeout detection in case of preemption */
          if (__HAL_ADC_GET_FLAG(hadc, ADC_FLAG_RDY) == 0UL)
          {
            /* Update ADC state machine to error */
            SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL);

            /* Set ADC error code to ADC peripheral internal error */
            SET_BIT(hadc->ErrorCode, HAL_ADC_ERROR_INTERNAL);

            return HAL_ERROR;
          }
        }
      }
    }
  }

  return HAL_OK;
}

/**
  * @brief  Disable the selected ADC.
  * @note   Prerequisite condition to use this function: ADC conversions must be
  *         stopped.
  * @param hadc ADC handle
  * @retval HAL status.
  */
HAL_StatusTypeDef ADC_Disable(ADC_HandleTypeDef *hadc)
{
  uint32_t tickstart;
  const uint32_t tmp_adc_is_disable_on_going = LL_ADC_IsDisableOngoing(hadc->Instance);

  /* Verification if ADC is not already disabled:                             */
  /* Note: forbidden to disable ADC (set bit ADC_CR_ADDIS) if ADC is already  */
  /*       disabled.                                                          */
  if ((LL_ADC_IsEnabled(hadc->Instance) != 0UL)
      && (tmp_adc_is_disable_on_going == 0UL)
     )
  {
    /* Check if conditions to disable the ADC are fulfilled */
    if ((hadc->Instance->CR & (ADC_CR_JADSTART | ADC_CR_ADSTART | ADC_CR_ADEN)) == ADC_CR_ADEN)
    {
      /* Disable the ADC peripheral */
      LL_ADC_Disable(hadc->Instance);
      __HAL_ADC_CLEAR_FLAG(hadc, (ADC_FLAG_EOSMP | ADC_FLAG_RDY));
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

    while ((hadc->Instance->CR & ADC_CR_ADEN) != 0UL)
    {
      if ((HAL_GetTick() - tickstart) > ADC_DISABLE_TIMEOUT)
      {
        /* New check to avoid false timeout detection in case of preemption */
        if ((hadc->Instance->CR & ADC_CR_ADEN) != 0UL)
        {
          /* Update ADC state machine to error */
          SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_INTERNAL);

          /* Set ADC error code to ADC peripheral internal error */
          SET_BIT(hadc->ErrorCode, HAL_ADC_ERROR_INTERNAL);

          return HAL_ERROR;
        }
      }
    }
  }

  return HAL_OK;
}

/**
  * @brief  DMA transfer complete callback.
  * @param hdma pointer to DMA handle.
  * @retval None
  */
void ADC_DMAConvCplt(DMA_HandleTypeDef *hdma)
{
  /* Retrieve ADC handle corresponding to current DMA handle */
  ADC_HandleTypeDef *hadc = (ADC_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;

  /* Update state machine on conversion status if not in error state */
  if ((hadc->State & (HAL_ADC_STATE_ERROR_INTERNAL | HAL_ADC_STATE_ERROR_DMA)) == 0UL)
  {
    /* Set ADC state */
    SET_BIT(hadc->State, HAL_ADC_STATE_REG_EOC);

    /* Determine whether any further conversion upcoming on group regular     */
    /* by external trigger, continuous mode or scan sequence on going         */
    /* to disable interruption.                                               */
    /* Is it the end of the regular sequence ? */
    if ((hadc->Instance->ISR & ADC_FLAG_EOS) != 0UL)
    {
      /* Are conversions software-triggered ? */
      if (LL_ADC_REG_IsTriggerSourceSWStart(hadc->Instance) != 0UL)
      {
        /* Is CONT bit set ? */
        if (READ_BIT(hadc->Instance->CFGR1, ADC_CFGR1_CONT) == 0UL)
        {
          /* CONT bit is not set, no more conversions expected */
          CLEAR_BIT(hadc->State, HAL_ADC_STATE_REG_BUSY);
          if ((hadc->State & HAL_ADC_STATE_INJ_BUSY) == 0UL)
          {
            SET_BIT(hadc->State, HAL_ADC_STATE_READY);
          }
        }
      }
    }
    else
    {
      /* DMA End of Transfer interrupt was triggered but conversions sequence
         is not over. If DMACFG is set to 0, conversions are stopped. */
      if (READ_BIT(hadc->Instance->CFGR1, ADC_CFGR1_DMNGT) == 0UL)
      {
        /* DMACFG bit is not set, conversions are stopped. */
        CLEAR_BIT(hadc->State, HAL_ADC_STATE_REG_BUSY);
        if ((hadc->State & HAL_ADC_STATE_INJ_BUSY) == 0UL)
        {
          SET_BIT(hadc->State, HAL_ADC_STATE_READY);
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
  else /* DMA and-or internal error occurred */
  {
    if ((hadc->State & HAL_ADC_STATE_ERROR_INTERNAL) != 0UL)
    {
      /* Call HAL ADC Error Callback function */
#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
      hadc->ErrorCallback(hadc);
#else
      HAL_ADC_ErrorCallback(hadc);
#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */
    }
    else
    {
      /* Call ADC DMA error callback */
      hadc->DMA_Handle->XferErrorCallback(hdma);
    }
  }
}

/**
  * @brief  DMA half transfer complete callback.
  * @param hdma pointer to DMA handle.
  * @retval None
  */
void ADC_DMAHalfConvCplt(DMA_HandleTypeDef *hdma)
{
  /* Retrieve ADC handle corresponding to current DMA handle */
  ADC_HandleTypeDef *hadc = (ADC_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;

  /* Half conversion callback */
#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
  hadc->ConvHalfCpltCallback(hadc);
#else
  HAL_ADC_ConvHalfCpltCallback(hadc);
#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */
}

/**
  * @brief  DMA error callback.
  * @param hdma pointer to DMA handle.
  * @retval None
  */
void ADC_DMAError(DMA_HandleTypeDef *hdma)
{
  /* Retrieve ADC handle corresponding to current DMA handle */
  ADC_HandleTypeDef *hadc = (ADC_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;

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
  * @}
  */

#endif /* HAL_ADC_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */
