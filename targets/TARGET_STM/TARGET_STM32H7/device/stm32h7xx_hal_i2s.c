/**
  ******************************************************************************
  * @file    stm32h7xx_hal_i2s.c
  * @author  MCD Application Team
  * @brief   I2S HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the Integrated Interchip Sound (I2S) peripheral:
  *           + Initialization and de-initialization functions
  *           + IO operation functions
  *           + Peripheral State and Errors functions
  @verbatim
 ===============================================================================
                  ##### How to use this driver #####
 ===============================================================================
 [..]
    The I2S HAL driver can be used as follow:

    (#) Declare a I2S_HandleTypeDef handle structure.
    (#) Initialize the I2S low level resources by implement the HAL_I2S_MspInit() API:
        (##) Enable the SPIx interface clock.
        (##) I2S pins configuration:
            (+++) Enable the clock for the I2S GPIOs.
            (+++) Configure these I2S pins as alternate function pull-up.
        (##) NVIC configuration if you need to use interrupt process (HAL_I2S_Transmit_IT()
             and HAL_I2S_Receive_IT() APIs).
            (+++) Configure the I2Sx interrupt priority.
            (+++) Enable the NVIC I2S IRQ handle.
        (##) DMA Configuration if you need to use DMA process (HAL_I2S_Transmit_DMA()
             and HAL_I2S_Receive_DMA() APIs:
            (+++) Declare a DMA handle structure for the Tx/Rx channel.
            (+++) Enable the DMAx interface clock.
            (+++) Configure the declared DMA handle structure with the required Tx/Rx parameters.
            (+++) Configure the DMA Tx/Rx Channel.
            (+++) Associate the initialized DMA handle to the I2S DMA Tx/Rx handle.
            (+++) Configure the priority and enable the NVIC for the transfer complete interrupt on the
                  DMA Tx/Rx Channel.

   (#) Program the Mode, Standard, Data Format, MCLK Output, Audio frequency and Polarity
       using HAL_I2S_Init() function.

   -@- The specific I2S interrupts (Transmission complete interrupt,
       RXP interrupt and Error Interrupts) will be managed using the macros
       __HAL_I2S_ENABLE_IT() and __HAL_I2S_DISABLE_IT() inside the transmit and receive process.
   -@- Make sure that either:
       (+@) External clock source is configured after setting correctly
            the define constant EXTERNAL_CLOCK_VALUE in the stm32h7xx_hal_conf.h file.

   Three mode of operations are available within this driver :

   *** Polling mode IO operation ***
   =================================
   [..]
     (+) Send an amount of data in blocking mode using HAL_I2S_Transmit()
     (+) Receive an amount of data in blocking mode using HAL_I2S_Receive()

   *** Interrupt mode IO operation ***
   ===================================
   [..]
     (+) Send an amount of data in non blocking mode using HAL_I2S_Transmit_IT()
     (+) At transmission end of half transfer HAL_I2S_TxHalfCpltCallback is executed and user can
         add his own code by customization of function pointer HAL_I2S_TxHalfCpltCallback
     (+) At transmission end of transfer HAL_I2S_TxCpltCallback is executed and user can
         add his own code by customization of function pointer HAL_I2S_TxCpltCallback
     (+) Receive an amount of data in non blocking mode using HAL_I2S_Receive_IT()
     (+) At reception end of half transfer HAL_I2S_RxHalfCpltCallback is executed and user can
         add his own code by customization of function pointer HAL_I2S_RxHalfCpltCallback
     (+) At reception end of transfer HAL_I2S_RxCpltCallback is executed and user can
         add his own code by customization of function pointer HAL_I2S_RxCpltCallback
     (+) In case of transfer Error, HAL_I2S_ErrorCallback() function is executed and user can
         add his own code by customization of function pointer HAL_I2S_ErrorCallback

   *** DMA mode IO operation ***
   ==============================
   [..]
     (+) Send an amount of data in non blocking mode (DMA) using HAL_I2S_Transmit_DMA()
     (+) At transmission end of half transfer HAL_I2S_TxHalfCpltCallback is executed and user can
         add his own code by customization of function pointer HAL_I2S_TxHalfCpltCallback
     (+) At transmission end of transfer HAL_I2S_TxCpltCallback is executed and user can
         add his own code by customization of function pointer HAL_I2S_TxCpltCallback
     (+) Receive an amount of data in non blocking mode (DMA) using HAL_I2S_Receive_DMA()
     (+) At reception end of half transfer HAL_I2S_RxHalfCpltCallback is executed and user can
         add his own code by customization of function pointer HAL_I2S_RxHalfCpltCallback
     (+) At reception end of transfer HAL_I2S_RxCpltCallback is executed and user can
         add his own code by customization of function pointer HAL_I2S_RxCpltCallback
     (+) In case of transfer Error, HAL_I2S_ErrorCallback() function is executed and user can
         add his own code by customization of function pointer HAL_I2S_ErrorCallback
     (+) Pause the DMA Transfer using HAL_I2S_DMAPause()
     (+) Resume the DMA Transfer using HAL_I2S_DMAResume()
     (+) Stop the DMA Transfer using HAL_I2S_DMAStop()

   *** I2S HAL driver macros list ***
   ===================================
   [..]
     Below the list of most used macros in I2S HAL driver.

      (+) __HAL_I2S_ENABLE: Enable the specified SPI peripheral (in I2S mode)
      (+) __HAL_I2S_DISABLE: Disable the specified SPI peripheral (in I2S mode)
      (+) __HAL_I2S_ENABLE_IT : Enable the specified I2S interrupts
      (+) __HAL_I2S_DISABLE_IT : Disable the specified I2S interrupts
      (+) __HAL_I2S_GET_FLAG: Check whether the specified I2S flag is set or not

    [..]
      (@) You can refer to the I2S HAL driver header file for more useful macros

    [..]
       Callback registration:

      (#) The compilation flag USE_HAL_I2S_REGISTER_CALLBACKS when set to 1U
          allows the user to configure dynamically the driver callbacks.
          Use Functions HAL_I2S_RegisterCallback() to register an interrupt callback.

          Function HAL_I2S_RegisterCallback() allows to register following callbacks:
            (+) TxCpltCallback        : I2S Tx Completed callback
            (+) RxCpltCallback        : I2S Rx Completed callback
            (+) TxRxCpltCallback      : I2S TxRx Completed callback
            (+) TxHalfCpltCallback    : I2S Tx Half Completed callback
            (+) RxHalfCpltCallback    : I2S Rx Half Completed callback
            (+) TxRxHalfCpltCallback  : I2S TxRx Half Completed callback
            (+) ErrorCallback         : I2S Error callback
            (+) MspInitCallback       : I2S Msp Init callback
            (+) MspDeInitCallback     : I2S Msp DeInit callback
          This function takes as parameters the HAL peripheral handle, the Callback ID
          and a pointer to the user callback function.


      (#) Use function HAL_I2S_UnRegisterCallback to reset a callback to the default
          weak function.
          HAL_I2S_UnRegisterCallback takes as parameters the HAL peripheral handle,
          and the Callback ID.
          This function allows to reset following callbacks:
            (+) TxCpltCallback        : I2S Tx Completed callback
            (+) RxCpltCallback        : I2S Rx Completed callback
            (+) TxRxCpltCallback      : I2S TxRx Completed callback
            (+) TxHalfCpltCallback    : I2S Tx Half Completed callback
            (+) RxHalfCpltCallback    : I2S Rx Half Completed callback
            (+) TxRxHalfCpltCallback  : I2S TxRx Half Completed callback
            (+) ErrorCallback         : I2S Error callback
            (+) MspInitCallback       : I2S Msp Init callback
            (+) MspDeInitCallback     : I2S Msp DeInit callback

       By default, after the HAL_I2S_Init() and when the state is HAL_I2S_STATE_RESET
       all callbacks are set to the corresponding weak functions:
       examples HAL_I2S_MasterTxCpltCallback(), HAL_I2S_MasterRxCpltCallback().
       Exception done for MspInit and MspDeInit functions that are
       reset to the legacy weak functions in the HAL_I2S_Init()/ HAL_I2S_DeInit() only when
       these callbacks are null (not registered beforehand).
       If MspInit or MspDeInit are not null, the HAL_I2S_Init()/ HAL_I2S_DeInit()
       keep and use the user MspInit/MspDeInit callbacks (registered beforehand) whatever the state.

       Callbacks can be registered/unregistered in HAL_I2S_STATE_READY state only.
       Exception done MspInit/MspDeInit functions that can be registered/unregistered
       in HAL_I2S_STATE_READY or HAL_I2S_STATE_RESET state,
       thus registered (user) MspInit/DeInit callbacks can be used during the Init/DeInit.
       Then, the user first registers the MspInit/MspDeInit user callbacks
       using HAL_I2S_RegisterCallback() before calling HAL_I2S_DeInit()
       or HAL_I2S_Init() function.

       When The compilation define USE_HAL_I2S_REGISTER_CALLBACKS is set to 0 or
       not defined, the callback registering feature is not available
       and weak (surcharged) callbacks are used.

  @endverbatim
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
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
#include "stm32h7xx_hal.h"

/** @addtogroup STM32H7xx_HAL_Driver
  * @{
  */

#ifdef HAL_I2S_MODULE_ENABLED

/** @addtogroup I2S
  * @brief I2S HAL module driver
  * @{
  */



/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @defgroup I2S_Private_Constants I2S Private Constants
  * @{
  */
#define I2S_DEFAULT_TIMEOUT 100U
/**
  * @}
  */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/** @addtogroup I2S_Private
  * @{
  */
static void               I2S_DMATxCplt(DMA_HandleTypeDef *hdma);
static void               I2S_DMATxHalfCplt(DMA_HandleTypeDef *hdma);
static void               I2S_DMARxCplt(DMA_HandleTypeDef *hdma);
static void               I2S_DMARxHalfCplt(DMA_HandleTypeDef *hdma);
static void               I2S_DMAError(DMA_HandleTypeDef *hdma);

static void               I2S_TxISR_16BIT(struct __I2S_HandleTypeDef *hi2s);
static void               I2S_TxISR_32BIT(struct __I2S_HandleTypeDef *hi2s);
static void               I2S_RxISR_16BIT(struct __I2S_HandleTypeDef *hi2s);
static void               I2S_RxISR_32BIT(struct __I2S_HandleTypeDef *hi2s);
static void               I2S_CloseRx_ISR(I2S_HandleTypeDef *hi2s);
static void               I2S_CloseTx_ISR(I2S_HandleTypeDef *hi2s);

static HAL_StatusTypeDef  I2S_WaitFlagStateUntilTimeout(I2S_HandleTypeDef *hi2s, uint32_t Flag, uint32_t State, uint32_t Timeout);

/**
  * @}
  */

/* Exported functions ---------------------------------------------------------*/
/** @addtogroup I2S_Exported_Functions I2S Exported Functions
  * @{
  */

/** @addtogroup  I2S_Exported_Functions_Group1
  *  @brief    Initialization and Configuration functions
  *
@verbatim
 ===============================================================================
              ##### Initialization and de-initialization functions #####
 ===============================================================================
    [..]  This subsection provides a set of functions allowing to initialize and
          de-initialiaze the I2Sx peripheral in simplex mode:

      (+) User must Implement HAL_I2S_MspInit() function in which he configures
          all related peripherals resources (CLOCK, GPIO, DMA, IT and NVIC ).

      (+) Call the function HAL_I2S_Init() to configure the selected device with
          the selected configuration:
        (++) Mode
        (++) Standard
        (++) Data Format
        (++) MCLK Output
        (++) Audio frequency
        (++) Polarity
        (++) First Bit
        (++) WS Inversion
        (++) IO Swap
        (++) Data 24Bit Alignment
        (++) Fifo Threshold
        (++) Alternate function GPIOs state
        (++) Channel length in SLAVE

     (+) Call the function HAL_I2S_DeInit() to restore the default configuration
         of the selected I2Sx periperal.
  @endverbatim
  * @{
  */

/**
  * @brief Initializes the I2S according to the specified parameters
  *         in the I2S_InitTypeDef and create the associated handle.
  * @param  hi2s: pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2S_Init(I2S_HandleTypeDef *hi2s)
{
  uint32_t i2sdiv = 2U, i2sodd = 0U, packetlength = 1U;
  uint32_t tmp = 0U, i2sclk = 0U;

  /* Check the I2S handle allocation */
  if (hi2s == NULL)
  {
    return HAL_ERROR;
  }

  /* Check the I2S parameters */
  assert_param(IS_I2S_ALL_INSTANCE(hi2s->Instance));
  assert_param(IS_I2S_MODE(hi2s->Init.Mode));
  assert_param(IS_I2S_STANDARD(hi2s->Init.Standard));
  assert_param(IS_I2S_DATA_FORMAT(hi2s->Init.DataFormat));
  assert_param(IS_I2S_MCLK_OUTPUT(hi2s->Init.MCLKOutput));
  assert_param(IS_I2S_AUDIO_FREQ(hi2s->Init.AudioFreq));
  assert_param(IS_I2S_CPOL(hi2s->Init.CPOL));
  assert_param(IS_I2S_FIRST_BIT(hi2s->Init.FirstBit));
  assert_param(IS_I2S_WS_INVERSION(hi2s->Init.WSInversion));
  assert_param(IS_I2S_IO_SWAP(hi2s->Init.IOSwap));
  assert_param(IS_I2S_DATA_24BIT_ALIGNMENT(hi2s->Init.Data24BitAlignment));
  assert_param(IS_I2S_FIFO_THRESHOLD(hi2s->Init.FifoThreshold));
  assert_param(IS_I2S_MASTER_KEEP_IO_STATE(hi2s->Init.MasterKeepIOState));
  assert_param(IS_I2S_SLAVE_EXTEND_FRE_DETECTION(hi2s->Init.SlaveExtendFREDetection));

  if (hi2s->State == HAL_I2S_STATE_RESET)
  {
    /* Allocate lock resource and initialize it */
    hi2s->Lock = HAL_UNLOCKED;

#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)
    /* Init the I2S Callback settings */
    hi2s->TxCpltCallback       = HAL_I2S_TxCpltCallback;         /* Legacy weak TxCpltCallback       */
    hi2s->RxCpltCallback       = HAL_I2S_RxCpltCallback;         /* Legacy weak RxCpltCallback       */
    hi2s->TxRxCpltCallback     = HAL_I2SEx_TxRxCpltCallback;     /* Legacy weak TxRxCpltCallback     */
    hi2s->TxHalfCpltCallback   = HAL_I2S_TxHalfCpltCallback;     /* Legacy weak TxHalfCpltCallback   */
    hi2s->RxHalfCpltCallback   = HAL_I2S_RxHalfCpltCallback;     /* Legacy weak RxHalfCpltCallback   */
    hi2s->TxRxHalfCpltCallback = HAL_I2SEx_TxRxHalfCpltCallback; /* Legacy weak TxRxHalfCpltCallback */
    hi2s->ErrorCallback        = HAL_I2S_ErrorCallback;          /* Legacy weak ErrorCallback        */

    if (hi2s->MspInitCallback == NULL)
    {
      hi2s->MspInitCallback = HAL_I2S_MspInit; /* Legacy weak MspInit  */
    }

    /* Init the low level hardware : GPIO, CLOCK, NVIC... */
    hi2s->MspInitCallback(hi2s);
#else
    /* Init the low level hardware : GPIO, CLOCK, NVIC... */
    HAL_I2S_MspInit(hi2s);
#endif /* USE_HAL_I2S_REGISTER_CALLBACKS */
  }

  hi2s->State = HAL_I2S_STATE_BUSY;

  /* Clear I2S configuration register */
  CLEAR_REG(hi2s->Instance->I2SCFGR);

  /* If the default value has to be written, reinitialize i2sdiv and i2sodd */
  if (hi2s->Init.AudioFreq == I2S_AUDIOFREQ_DEFAULT)
  {
    i2sodd = 0U;
    i2sdiv = 2U;
  }
  /* If the requested audio frequency is not the default, compute the prescaler */
  else
  {
    /* Check the frame length (For the Prescaler computing) *******************/
    /* Set I2S Packet Length value*/
    if (hi2s->Init.DataFormat != I2S_DATAFORMAT_16B)
    {
      /* Packet length is 32 bits */
      packetlength = 32U;
    }
    else
    {
      /* Packet length is 16 bits */
      packetlength = 16U;
    }

    /* I2S standard */
    if (hi2s->Init.Standard <= I2S_STANDARD_LSB)
    {
      /* In I2S standard packet length is multiplied by 2 */
      packetlength = packetlength * 2U;
    }

    /* Get the source clock value: based on System Clock value */
    /* SPI1,SPI2 and SPI3 share the same source clock */
    i2sclk = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_SPI1);

    /* Compute the Real divider depending on the MCLK output state, with a floating point */
    if (hi2s->Init.MCLKOutput == I2S_MCLKOUTPUT_ENABLE)
    {
      /* MCLK output is enabled */
      if (hi2s->Init.DataFormat != I2S_DATAFORMAT_16B)
      {
        tmp = (uint32_t)(((((i2sclk / (packetlength * 4)) * 10) / hi2s->Init.AudioFreq)) + 5);
      }
      else
      {
        tmp = (uint32_t)(((((i2sclk / (packetlength * 8)) * 10) / hi2s->Init.AudioFreq)) + 5);
      }
    }
    else
    {
      /* MCLK output is disabled */
      tmp = (uint32_t)(((((i2sclk / packetlength) * 10) / hi2s->Init.AudioFreq)) + 5);
    }

    /* Remove the flatting point */
    tmp = tmp / 10U;

    /* Check the parity of the divider */
    i2sodd = (uint32_t)(tmp & (uint32_t)1U);

    /* Compute the i2sdiv prescaler */
    i2sdiv = (uint32_t)((tmp - i2sodd) / 2U);

    /* Get the Mask for the Odd bit I2SCFGR register */
    i2sodd = (uint32_t)(i2sodd << 24U);
  }

  /* Test if the obtain values are forbidden or out of range */
  if (((i2sodd == 1UL) && (i2sdiv == 1UL)) || (i2sdiv > 0xFFUL))
  {
    /* Set the default values */
    i2sdiv = 2U;
    i2sodd = 0U;
    /* Set the error code */
    SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_PRESCALER);
    return  HAL_ERROR;
  }
  
  /* Force i2smod to 1 just to be sure that (2xi2sdiv + i2sodd) is always higher than 0 */
  if (i2sdiv == 0U)
  { 
    i2sodd = 1U;
  }

  /* Check if the I2S is disabled to edit I2SCFGR and CFG1 register */
  if ((hi2s->Instance->CR1 & SPI_CR1_SPE) == SPI_CR1_SPE)
  {
    /* Disable I2S peripheral */
    __HAL_I2S_DISABLE(hi2s);
  }

  /* Clear and configure I2S I2SCFGR register */
  MODIFY_REG(hi2s->Instance->I2SCFGR, (SPI_I2SCFGR_I2SMOD       | SPI_I2SCFGR_I2SCFG                 | \
                                       SPI_I2SCFGR_I2SSTD       | SPI_I2SCFGR_PCMSYNC                | \
                                       SPI_I2SCFGR_DATLEN       | SPI_I2SCFGR_CHLEN                  | \
                                       SPI_I2SCFGR_CKPOL        | SPI_I2SCFGR_FIXCH                  | \
                                       SPI_I2SCFGR_WSINV        | SPI_I2SCFGR_DATFMT                 | \
                                       SPI_I2SCFGR_I2SDIV       | SPI_I2SCFGR_ODD                    | \
                                       SPI_I2SCFGR_MCKOE),                                             \
             (SPI_I2SCFGR_I2SMOD       | hi2s->Init.Mode                    | \
              hi2s->Init.Standard      | hi2s->Init.DataFormat              | \
              hi2s->Init.CPOL          | hi2s->Init.SlaveExtendFREDetection | \
              hi2s->Init.WSInversion   | hi2s->Init.Data24BitAlignment      | \
              (uint32_t)(i2sdiv << 16U) | (uint32_t)(i2sodd)                 | \
              hi2s->Init.MCLKOutput));

  /* Clear and configure I2S CFG1 register */
  MODIFY_REG(hi2s->Instance->CFG1, SPI_CFG1_FTHLV, hi2s->Init.FifoThreshold);

  /* Unlock the AF configuration to configure CFG2 register*/
  CLEAR_BIT(hi2s->Instance->CR1, SPI_CR1_IOLOCK);

  /* Clear and configure SPI2S CFG2 register */
  MODIFY_REG(hi2s->Instance->CFG2, SPI_CFG2_LSBFRST | SPI_CFG2_IOSWP, (hi2s->Init.FirstBit | hi2s->Init.IOSwap));

  /* Insure that AFCNTR is managed only by Master */
  if (IS_I2S_MASTER(hi2s->Init.Mode))
  {
    /* Alternate function GPIOs control */
    MODIFY_REG(hi2s->Instance->CFG2, SPI_CFG2_AFCNTR, (hi2s->Init.MasterKeepIOState));
  }

  hi2s->ErrorCode = HAL_I2S_ERROR_NONE;
  hi2s->State = HAL_I2S_STATE_READY;

  return HAL_OK;
}

/**
  * @brief  DeInitializes the I2S peripheral
  * @param  hi2s: pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2S_DeInit(I2S_HandleTypeDef *hi2s)
{
  /* Check the I2S handle allocation */
  if (hi2s == NULL)
  {
    return HAL_ERROR;
  }

  /* Check the parameters */
  assert_param(IS_I2S_ALL_INSTANCE(hi2s->Instance));

  hi2s->State = HAL_I2S_STATE_BUSY;

  /* Disable the I2S Peripheral Clock */
  __HAL_I2S_DISABLE(hi2s);

#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)
  if (hi2s->MspDeInitCallback == NULL)
  {
    hi2s->MspDeInitCallback = HAL_I2S_MspDeInit; /* Legacy weak MspDeInit  */
  }

  /* DeInit the low level hardware: GPIO, CLOCK, NVIC... */
  hi2s->MspDeInitCallback(hi2s);
#else
  /* DeInit the low level hardware: GPIO, CLOCK, NVIC... */
  HAL_I2S_MspDeInit(hi2s);
#endif /* USE_HAL_I2S_REGISTER_CALLBACKS */

  hi2s->ErrorCode = HAL_I2S_ERROR_NONE;
  hi2s->State = HAL_I2S_STATE_RESET;

  /* Release Lock */
  __HAL_UNLOCK(hi2s);

  return HAL_OK;
}

/**
  * @brief I2S MSP Init
  * @param  hi2s: pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval None
  */
__weak void HAL_I2S_MspInit(I2S_HandleTypeDef *hi2s)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hi2s);

  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_I2S_MspInit could be implemented in the user file
   */
}

/**
  * @brief I2S MSP DeInit
  * @param  hi2s: pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval None
  */
__weak void HAL_I2S_MspDeInit(I2S_HandleTypeDef *hi2s)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hi2s);

  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_I2S_MspDeInit could be implemented in the user file
   */
}

#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)
/**
  * @brief  Register a User I2S Callback
  *         To be used instead of the weak predefined callback
  * @param  hi2s Pointer to a I2S_HandleTypeDef structure that contains
  *                the configuration information for the specified I2S.
  * @param  CallbackID ID of the callback to be registered
  * @param  pCallback pointer to the Callback function
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2S_RegisterCallback(I2S_HandleTypeDef *hi2s, HAL_I2S_CallbackIDTypeDef CallbackID, pI2S_CallbackTypeDef pCallback)
{
  HAL_StatusTypeDef status = HAL_OK;

  if (pCallback == NULL)
  {
    /* Update the error code */
    hi2s->ErrorCode |= HAL_I2S_ERROR_INVALID_CALLBACK;

    return HAL_ERROR;
  }
  /* Process locked */
  __HAL_LOCK(hi2s);

  if (HAL_I2S_STATE_READY == hi2s->State)
  {
    switch (CallbackID)
    {
      case HAL_I2S_TX_COMPLETE_CB_ID :
        hi2s->TxCpltCallback = pCallback;
        break;

      case HAL_I2S_RX_COMPLETE_CB_ID :
        hi2s->RxCpltCallback = pCallback;
        break;

      case HAL_I2S_TX_RX_COMPLETE_CB_ID :
        hi2s->TxRxCpltCallback = pCallback;
        break;

      case HAL_I2S_TX_HALF_COMPLETE_CB_ID :
        hi2s->TxHalfCpltCallback = pCallback;
        break;

      case HAL_I2S_RX_HALF_COMPLETE_CB_ID :
        hi2s->RxHalfCpltCallback = pCallback;
        break;

      case HAL_I2S_TX_RX_HALF_COMPLETE_CB_ID :
        hi2s->RxHalfCpltCallback = pCallback;
        break;

      case HAL_I2S_ERROR_CB_ID :
        hi2s->ErrorCallback = pCallback;
        break;

      case HAL_I2S_MSPINIT_CB_ID :
        hi2s->MspInitCallback = pCallback;
        break;

      case HAL_I2S_MSPDEINIT_CB_ID :
        hi2s->MspDeInitCallback = pCallback;
        break;

      default :
        /* Update the error code */
        SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_INVALID_CALLBACK);

        /* Return error status */
        status =  HAL_ERROR;
        break;
    }
  }
  else if (HAL_I2S_STATE_RESET == hi2s->State)
  {
    switch (CallbackID)
    {
      case HAL_I2S_MSPINIT_CB_ID :
        hi2s->MspInitCallback = pCallback;
        break;

      case HAL_I2S_MSPDEINIT_CB_ID :
        hi2s->MspDeInitCallback = pCallback;
        break;

      default :
        /* Update the error code */
        SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_INVALID_CALLBACK);

        /* Return error status */
        status =  HAL_ERROR;
        break;
    }
  }
  else
  {
    /* Update the error code */
    SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_INVALID_CALLBACK);

    /* Return error status */
    status =  HAL_ERROR;
  }

  /* Release Lock */
  __HAL_UNLOCK(hi2s);
  return status;
}

/**
  * @brief  Unregister an I2S Callback
  *         I2S callback is redirected to the weak predefined callback
  * @param  hi2s Pointer to a I2S_HandleTypeDef structure that contains
  *                the configuration information for the specified I2S.
  * @param  CallbackID ID of the callback to be unregistered
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2S_UnRegisterCallback(I2S_HandleTypeDef *hi2s, HAL_I2S_CallbackIDTypeDef CallbackID)
{
  HAL_StatusTypeDef status = HAL_OK;

  /* Process locked */
  __HAL_LOCK(hi2s);

  if (HAL_I2S_STATE_READY == hi2s->State)
  {
    switch (CallbackID)
    {
      case HAL_I2S_TX_COMPLETE_CB_ID :
        hi2s->TxCpltCallback = HAL_I2S_TxCpltCallback;               /* Legacy weak TxCpltCallback       */
        break;

      case HAL_I2S_RX_COMPLETE_CB_ID :
        hi2s->RxCpltCallback = HAL_I2S_RxCpltCallback;               /* Legacy weak RxCpltCallback       */
        break;

      case HAL_I2S_TX_RX_COMPLETE_CB_ID :
        hi2s->TxRxCpltCallback = HAL_I2SEx_TxRxCpltCallback;         /* Legacy weak TxRxCpltCallback     */
        break;

      case HAL_I2S_TX_HALF_COMPLETE_CB_ID :
        hi2s->TxHalfCpltCallback = HAL_I2S_TxHalfCpltCallback;       /* Legacy weak TxHalfCpltCallback   */
        break;

      case HAL_I2S_RX_HALF_COMPLETE_CB_ID :
        hi2s->RxHalfCpltCallback = HAL_I2S_RxHalfCpltCallback;       /* Legacy weak RxHalfCpltCallback   */
        break;

      case HAL_I2S_TX_RX_HALF_COMPLETE_CB_ID :
        hi2s->TxRxHalfCpltCallback = HAL_I2SEx_TxRxHalfCpltCallback; /* Legacy weak TxRxHalfCpltCallback */
        break;

      case HAL_I2S_ERROR_CB_ID :
        hi2s->ErrorCallback = HAL_I2S_ErrorCallback;                 /* Legacy weak ErrorCallback        */
        break;

      case HAL_I2S_MSPINIT_CB_ID :
        hi2s->MspInitCallback = HAL_I2S_MspInit;                     /* Legacy weak MspInit              */
        break;

      case HAL_I2S_MSPDEINIT_CB_ID :
        hi2s->MspDeInitCallback = HAL_I2S_MspDeInit;                 /* Legacy weak MspDeInit            */
        break;

      default :
        /* Update the error code */
        SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_INVALID_CALLBACK);

        /* Return error status */
        status =  HAL_ERROR;
        break;
    }
  }
  else if (HAL_I2S_STATE_RESET == hi2s->State)
  {
    switch (CallbackID)
    {
      case HAL_I2S_MSPINIT_CB_ID :
        hi2s->MspInitCallback = HAL_I2S_MspInit;                   /* Legacy weak MspInit              */
        break;

      case HAL_I2S_MSPDEINIT_CB_ID :
        hi2s->MspDeInitCallback = HAL_I2S_MspDeInit;               /* Legacy weak MspDeInit            */
        break;

      default :
        /* Update the error code */
        SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_INVALID_CALLBACK);

        /* Return error status */
        status =  HAL_ERROR;
        break;
    }
  }
  else
  {
    /* Update the error code */
    SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_INVALID_CALLBACK);

    /* Return error status */
    status =  HAL_ERROR;
  }

  /* Release Lock */
  __HAL_UNLOCK(hi2s);
  return status;
}
#endif /* USE_HAL_I2S_REGISTER_CALLBACKS */

/**
  * @}
  */

/** @defgroup I2S_Exported_Functions_Group2 IO operation functions
  *  @brief Data transfers functions
  *
@verbatim
 ===============================================================================
                      ##### IO operation functions #####
 ===============================================================================
    [..]
    This subsection provides a set of functions allowing to manage the I2S data
    transfers.

    (#) There are two modes of transfer:
       (++) Blocking mode : The communication is performed in the polling mode.
            The status of all data processing is returned by the same function
            after finishing transfer.
       (++) No-Blocking mode : The communication is performed using Interrupts
            or DMA. These functions return the status of the transfer startup.
            The end of the data processing will be indicated through the
            dedicated I2S IRQ when using Interrupt mode or the DMA IRQ when
            using DMA mode.

    (#) Blocking mode functions are :
        (++) HAL_I2S_Transmit()
        (++) HAL_I2S_Receive()

    (#) No-Blocking mode functions with Interrupt are :
        (++) HAL_I2S_Transmit_IT()
        (++) HAL_I2S_Receive_IT()

    (#) No-Blocking mode functions with DMA are :
        (++) HAL_I2S_Transmit_DMA()
        (++) HAL_I2S_Receive_DMA()

    (#) A set of Transfer Complete Callbacks are provided in non Blocking mode:
        (++) HAL_I2S_TxCpltCallback()
        (++) HAL_I2S_TxHalfCpltCallback()
        (++) HAL_I2S_RxCpltCallback()
        (++) HAL_I2S_RxHalfCpltCallback()
        (++) HAL_I2S_ErrorCallback()

@endverbatim
  * @{
  */

/**
  * @brief  Transmit an amount of data in blocking mode
  * @param  hi2s: pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @param  pData: a 16-bit pointer to data buffer.
  * @param  Size: number of frames to be sent.
  * @param  Timeout: Timeout duration
  * @note   The I2S is kept enabled at the end of transaction to avoid the clock de-synchronization
  *         between Master and Slave(example: audio streaming).
  * @note   This function can use an Audio Frequency up to 48KHz when I2S Clock Source is 32MHz
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2S_Transmit(I2S_HandleTypeDef *hi2s, uint16_t *pData, uint16_t Size, uint32_t Timeout)
{
  uint32_t tickstart = 0U;
  uint32_t isDataFormat16B = 2U;

  /* Check Mode parameter */
  assert_param(IS_I2S_TX_MODE(hi2s->Init.Mode));

  if ((pData == NULL) || (Size == 0U))
  {
    return  HAL_ERROR;
  }

  /* Process Locked */
  __HAL_LOCK(hi2s);

  /* Init tickstart for timeout management*/
  tickstart = HAL_GetTick();

  if (hi2s->State == HAL_I2S_STATE_READY)
  {
    /* Check the Data Format value */
    if (((hi2s->Instance->I2SCFGR & (SPI_I2SCFGR_DATLEN | SPI_I2SCFGR_CHLEN)) == I2S_DATAFORMAT_24B) ||
        ((hi2s->Instance->I2SCFGR & (SPI_I2SCFGR_DATLEN | SPI_I2SCFGR_CHLEN)) == I2S_DATAFORMAT_32B))
    {
      isDataFormat16B = 0U;
    }
    else
    {
      isDataFormat16B = 1U;
    }

    if (!isDataFormat16B)
    {
      hi2s->TxXferSize  = (Size << 1U);
      hi2s->TxXferCount = (Size << 1U);
    }
    else
    {
      hi2s->TxXferSize  = Size;
      hi2s->TxXferCount = Size;
    }

    /* Set state and reset error code */
    hi2s->ErrorCode = HAL_I2S_ERROR_NONE;
    hi2s->State = HAL_I2S_STATE_BUSY_TX;
    hi2s->pTxBuffPtr = pData;

    /* Check if the I2S is already enabled */
    if ((hi2s->Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE)
    {
      /* Enable I2S peripheral */
      __HAL_I2S_ENABLE(hi2s);
    }

    if (IS_I2S_MASTER(hi2s->Init.Mode))
    {
      hi2s->Instance->CR1 |= SPI_CR1_CSTART;
    }

    /* Transmit data in 32 Bit mode */
    if (!isDataFormat16B)
    {
      while (hi2s->TxXferCount > 0U)
      {
        /* Wait until TXP flag is set to send data */
        if (__HAL_I2S_GET_FLAG(hi2s, I2S_FLAG_TXP))
        {
          *((__IO uint32_t *)&hi2s->Instance->TXDR) = *((uint32_t *)hi2s->pTxBuffPtr);
          hi2s->pTxBuffPtr += 2U;
          hi2s->TxXferCount--;
        }
        else
        {
          /* Timeout management */
          if ((Timeout == 0U) || ((Timeout != HAL_MAX_DELAY) && ((HAL_GetTick() - tickstart) >=  Timeout)))
          {
            /* Set the error code and execute error callback*/
            SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_TIMEOUT);

#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)
            hi2s->ErrorCallback(hi2s);
#else
            HAL_I2S_ErrorCallback(hi2s);
#endif /* USE_HAL_I2S_REGISTER_CALLBACKS */

            /* Set the I2S State ready */
            hi2s->State = HAL_I2S_STATE_READY;

            /* Process Unlocked */
            __HAL_UNLOCK(hi2s);
            return HAL_ERROR;
          }
        }
      }
    }
    /* Transmit data in 16 Bit mode */
    else
    {
      while (hi2s->TxXferCount > 0U)
      {
        if (__HAL_I2S_GET_FLAG(hi2s, I2S_FLAG_TXP))
        {
          if ((hi2s->TxXferCount > 1U) && (hi2s->Init.FifoThreshold > I2S_FIFO_THRESHOLD_01DATA))
          {
            *((__IO uint32_t *)&hi2s->Instance->TXDR) = *((uint32_t *)hi2s->pTxBuffPtr);
            hi2s->pTxBuffPtr  += 2U;
            hi2s->TxXferCount -= 2U;
          }
          else
          {
            *((__IO uint16_t *)&hi2s->Instance->TXDR) = *((uint16_t *)hi2s->pTxBuffPtr);
            hi2s->pTxBuffPtr++;
            hi2s->TxXferCount--;
          }
        }
        else
        {
          /* Timeout management */
          if ((Timeout == 0U) || ((Timeout != HAL_MAX_DELAY) && ((HAL_GetTick() - tickstart) >=  Timeout)))
          {
            /* Set the error code and execute error callback*/
            SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_TIMEOUT);

#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)
            hi2s->ErrorCallback(hi2s);
#else
            HAL_I2S_ErrorCallback(hi2s);
#endif /* USE_HAL_I2S_REGISTER_CALLBACKS */

            /* Set the I2S State ready */
            hi2s->State = HAL_I2S_STATE_READY;

            /* Process Unlocked */
            __HAL_UNLOCK(hi2s);
            return HAL_ERROR;
          }
        }
      }
    }

    /* Wait until TXP flag is set, to confirm the end of the transaction */
    if (I2S_WaitFlagStateUntilTimeout(hi2s, I2S_FLAG_TXP, RESET, Timeout) != HAL_OK)
    {
      /* Set the error code and execute error callback*/
      SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_TIMEOUT);

#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)
      hi2s->ErrorCallback(hi2s);
#else
      HAL_I2S_ErrorCallback(hi2s);
#endif /* USE_HAL_I2S_REGISTER_CALLBACKS */

      /* Set the I2S State ready */
      hi2s->State = HAL_I2S_STATE_READY;

      /* Process Unlocked */
      __HAL_UNLOCK(hi2s);
      return HAL_ERROR;
    }

    hi2s->State = HAL_I2S_STATE_READY;

    /* Process Unlocked */
    __HAL_UNLOCK(hi2s);

    return HAL_OK;
  }
  else
  {
    /* Process Unlocked */
    __HAL_UNLOCK(hi2s);
    return HAL_BUSY;
  }
}

/**
  * @brief  Receive an amount of data in blocking mode
  * @param  hi2s: pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @param  pData: a 16-bit pointer to data buffer.
  * @param  Size: number of frames to be sent.
  * @param  Timeout: Timeout duration
  * @note   The I2S is kept enabled at the end of transaction to avoid the clock de-synchronization
  *         between Master and Slave(example: audio streaming).
  * @note   In I2S Master Receiver mode, just after enabling the peripheral the clock will be generate
  *         in continuous way and as the I2S is not disabled at the end of the I2S transaction.
  * @note   This function can use an Audio Frequency up to 44KHz when I2S Clock Source is 32MHz
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2S_Receive(I2S_HandleTypeDef *hi2s, uint16_t *pData, uint16_t Size, uint32_t Timeout)
{
  uint32_t tickstart = 0U;
  uint32_t isDataFormat16B = 2U;

  /* Check Mode parameter */
  assert_param(IS_I2S_RX_MODE(hi2s->Init.Mode));

  if ((pData == NULL) || (Size == 0U))
  {
    return  HAL_ERROR;
  }

  /* Process Locked */
  __HAL_LOCK(hi2s);

  /* Init tickstart for timeout management*/
  tickstart = HAL_GetTick();

  if (hi2s->State == HAL_I2S_STATE_READY)
  {
    /* Check the Data Format value */
    if (((hi2s->Instance->I2SCFGR & (SPI_I2SCFGR_DATLEN | SPI_I2SCFGR_CHLEN)) == I2S_DATAFORMAT_24B) ||
        ((hi2s->Instance->I2SCFGR & (SPI_I2SCFGR_DATLEN | SPI_I2SCFGR_CHLEN)) == I2S_DATAFORMAT_32B))
    {
      isDataFormat16B = 0U;
    }
    else
    {
      isDataFormat16B = 1U;
    }

    if (!isDataFormat16B)
    {
      hi2s->RxXferSize = (Size << 1U);
      hi2s->RxXferCount = (Size << 1U);
    }
    else
    {
      hi2s->RxXferSize = Size;
      hi2s->RxXferCount = Size;
    }

    /* Set state and reset error code */
    hi2s->ErrorCode = HAL_I2S_ERROR_NONE;
    hi2s->State = HAL_I2S_STATE_BUSY_RX;
    hi2s->pRxBuffPtr = pData;

    /* Check if the I2S is already enabled */
    if ((hi2s->Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE)
    {
      /* Enable I2S peripheral */
      __HAL_I2S_ENABLE(hi2s);
    }

    if (IS_I2S_MASTER(hi2s->Init.Mode))
    {
      hi2s->Instance->CR1 |= SPI_CR1_CSTART;
    }

    /* Check if Master Receiver mode is selected */
    if ((hi2s->Instance->I2SCFGR & SPI_I2SCFGR_I2SCFG) == I2S_MODE_MASTER_RX)
    {
      /* Clear the Overrun Flag */
      __HAL_I2S_CLEAR_OVRFLAG(hi2s);
    }

    /* Receive data in 32 Bit mode */
    if ((!isDataFormat16B))
    {
      /* Transfer loop */
      while (hi2s->RxXferCount > 0U)
      {
        /* Check the RXP flag */
        if (__HAL_I2S_GET_FLAG(hi2s, I2S_FLAG_RXP))
        {
          *((uint32_t *)hi2s->pRxBuffPtr) = *((__IO uint32_t *)&hi2s->Instance->RXDR);
          hi2s->pRxBuffPtr += 2U;
          hi2s->RxXferCount--;
        }
        else
        {
          /* Timeout management */
          if ((Timeout == 0U) || ((Timeout != HAL_MAX_DELAY) && ((HAL_GetTick() - tickstart) >=  Timeout)))
          {
            /* Set the error code and execute error callback*/
            SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_TIMEOUT);

#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)
            hi2s->ErrorCallback(hi2s);
#else
            HAL_I2S_ErrorCallback(hi2s);
#endif /* USE_HAL_I2S_REGISTER_CALLBACKS */

            /* Set the I2S State ready */
            hi2s->State = HAL_I2S_STATE_READY;

            /* Process Unlocked */
            __HAL_UNLOCK(hi2s);
            return HAL_ERROR;
          }
        }
      }
    }
    /* Receive data in 16 Bit mode */
    else
    {
      /* Transfer loop */
      while (hi2s->RxXferCount > 0U)
      {
        /* Check the RXP flag */
        if (__HAL_I2S_GET_FLAG(hi2s, I2S_FLAG_RXP))
        {
          if (hi2s->Instance->SR & I2S_FLAG_RXWNE)
          {
            *((uint32_t *)hi2s->pRxBuffPtr) = *((__IO uint32_t *)&hi2s->Instance->RXDR);
            hi2s->pRxBuffPtr  += 2U;
            hi2s->RxXferCount -= 2U;
          }
          else
          {
            *((uint16_t *)hi2s->pRxBuffPtr) = *((__IO uint16_t *)&hi2s->Instance->RXDR);
            hi2s->pRxBuffPtr++;
            hi2s->RxXferCount--;
          }
        }
        else
        {
          /* Timeout management */
          if ((Timeout == 0U) || ((Timeout != HAL_MAX_DELAY) && ((HAL_GetTick() - tickstart) >=  Timeout)))
          {
            /* Set the error code and execute error callback*/
            SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_TIMEOUT);

#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)
            hi2s->ErrorCallback(hi2s);
#else
            HAL_I2S_ErrorCallback(hi2s);
#endif /* USE_HAL_I2S_REGISTER_CALLBACKS */

            /* Set the I2S State ready */
            hi2s->State = HAL_I2S_STATE_READY;

            /* Process Unlocked */
            __HAL_UNLOCK(hi2s);
            return HAL_ERROR;
          }
        }
      }
    }

    hi2s->State = HAL_I2S_STATE_READY;

    /* Process Unlocked */
    __HAL_UNLOCK(hi2s);

    return HAL_OK;
  }
  else
  {
    /* Process Unlocked */
    __HAL_UNLOCK(hi2s);
    return HAL_BUSY;
  }
}

/**
  * @brief  Transmit an amount of data in non-blocking mode with Interrupt
  * @param  hi2s: pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @param pData: a 16-bit pointer to data buffer.
  * @param Size: number of data sample to be sent:
  * @note When a 16-bit data frame or a 16-bit data frame extended is selected during the I2S
  *       configuration phase, the Size parameter means the number of 16-bit data length
  *       in the transaction and when a 24-bit data frame or a 32-bit data frame is selected
  *       the Size parameter means the number of 16-bit data length.
  * @note The I2S is kept enabled at the end of transaction to avoid the clock de-synchronization
  *       between Master and Slave(example: audio streaming).
  * @note This function can use an Audio Frequency up to 48KHz when I2S Clock Source is 32MHz
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2S_Transmit_IT(I2S_HandleTypeDef *hi2s, uint16_t *pData, uint16_t Size)
{
  /* Check Mode parameter */
  assert_param(IS_I2S_TX_MODE(hi2s->Init.Mode));

  /* Process Locked */
  __HAL_LOCK(hi2s);

  if ((pData == NULL) || (Size == 0U))
  {
    __HAL_UNLOCK(hi2s);
    return  HAL_ERROR;
  }

  if (hi2s->State != HAL_I2S_STATE_READY)
  {
    __HAL_UNLOCK(hi2s);
    return  HAL_BUSY;
  }

  /* Set the transaction information */
  hi2s->State       = HAL_I2S_STATE_BUSY_TX;
  hi2s->ErrorCode   = HAL_I2S_ERROR_NONE;
  hi2s->pTxBuffPtr  = pData;
  hi2s->TxXferSize  = Size;
  hi2s->TxXferCount = Size;

  /* Init field not used in handle to zero */
  hi2s->pRxBuffPtr  = NULL;
  hi2s->RxXferSize  = 0U;
  hi2s->RxXferCount = 0U;
  hi2s->RxISR       = NULL;

  /* Set the function for IT treatment */
  if (hi2s->Init.DataFormat > I2S_DATAFORMAT_16B)
  {
    hi2s->TxISR = I2S_TxISR_32BIT;
  }
  else
  {
    hi2s->TxISR = I2S_TxISR_16BIT;
  }

  /* Check if the I2S is already enabled */
  if ((hi2s->Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE)
  {
    /* Enable I2S peripheral */
    __HAL_I2S_ENABLE(hi2s);
  }

  if (IS_I2S_MASTER(hi2s->Init.Mode))
  {
    /* Master transfer start */
    SET_BIT(hi2s->Instance->CR1, SPI_CR1_CSTART);
  }

  /* Enable TXP and ERR interrupt */
  __HAL_I2S_ENABLE_IT(hi2s, (I2S_IT_TXP | I2S_IT_ERR));

  /* Process Unlocked */
  __HAL_UNLOCK(hi2s);

  return HAL_OK;
}

/**
  * @brief  Receive an amount of data in non-blocking mode with Interrupt
  * @param  hi2s: pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @param pData: a 16-bit pointer to the Receive data buffer.
  * @param Size: number of data sample to be sent:
  * @note When a 16-bit data frame or a 16-bit data frame extended is selected during the I2S
  *       configuration phase, the Size parameter means the number of 16-bit data length
  *       in the transaction and when a 24-bit data frame or a 32-bit data frame is selected
  *       the Size parameter means the number of 16-bit data length.
  * @note The I2S is kept enabled at the end of transaction to avoid the clock de-synchronization
  *       between Master and Slave(example: audio streaming).
  * @note It is recommended to use DMA for the I2S receiver to avoid de-synchronisation
  * between Master and Slave otherwise the I2S interrupt should be optimized.
  * @note This function can use an Audio Frequency up to 48KHz when I2S Clock Source is 32MHz
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2S_Receive_IT(I2S_HandleTypeDef *hi2s, uint16_t *pData, uint16_t Size)
{
  /* Check Mode parameter */
  assert_param(IS_I2S_RX_MODE(hi2s->Init.Mode));

  /* Process Locked */
  __HAL_LOCK(hi2s);

  /* Set the transaction information */
  hi2s->State       = HAL_I2S_STATE_BUSY_TX;
  hi2s->ErrorCode   = HAL_I2S_ERROR_NONE;
  hi2s->pRxBuffPtr  = pData;
  hi2s->RxXferSize  = Size;
  hi2s->RxXferCount = Size;

  /* Init field not used in handle to zero */
  hi2s->pTxBuffPtr  = NULL;
  hi2s->TxXferSize  = 0U;
  hi2s->TxXferCount = 0U;
  hi2s->TxISR       = NULL;

  /* Set the function for IT treatment */
  if (hi2s->Init.DataFormat > I2S_DATAFORMAT_16B)
  {
    hi2s->RxISR = I2S_RxISR_32BIT;
  }
  else
  {
    hi2s->RxISR = I2S_RxISR_16BIT;
  }

  /* Check if the I2S is already enabled */
  if ((hi2s->Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE)
  {
    /* Enable I2S peripheral */
    __HAL_I2S_ENABLE(hi2s);
  }

  if (IS_I2S_MASTER(hi2s->Init.Mode))
  {
    /* Master transfer start */
    SET_BIT(hi2s->Instance->CR1, SPI_CR1_CSTART);
  }

  /* Enable RXP and ERR interrupt */
  __HAL_I2S_ENABLE_IT(hi2s, (I2S_IT_RXP | I2S_IT_ERR));

  /* Process Unlocked */
  __HAL_UNLOCK(hi2s);

  return HAL_OK;
}

/**
  * @brief Transmit an amount of data in non-blocking mode with DMA
  * @param  hi2s: pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @param pData: a 16-bit pointer to the Transmit data buffer.
  * @param Size: number of data sample to be sent:
  * @note When a 16-bit data frame or a 16-bit data frame extended is selected during the I2S
  *       configuration phase, the Size parameter means the number of 16-bit data length
  *       in the transaction and when a 24-bit data frame or a 32-bit data frame is selected
  *       the Size parameter means the number of 16-bit data length.
  * @note The I2S is kept enabled at the end of transaction to avoid the clock de-synchronization
  *       between Master and Slave(example: audio streaming).
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2S_Transmit_DMA(I2S_HandleTypeDef *hi2s, uint16_t *pData, uint16_t Size)
{
  /* Check Mode parameter */
  assert_param(IS_I2S_TX_MODE(hi2s->Init.Mode));

  if ((pData == NULL) || (Size == 0U))
  {
    return  HAL_ERROR;
  }

  /* Process Locked */
  __HAL_LOCK(hi2s);

  if (hi2s->State == HAL_I2S_STATE_READY)
  {
    hi2s->pTxBuffPtr = pData;
    hi2s->State      = HAL_I2S_STATE_BUSY_TX;
    hi2s->ErrorCode  = HAL_I2S_ERROR_NONE;

    if (((hi2s->Instance->I2SCFGR & (SPI_I2SCFGR_DATLEN | SPI_I2SCFGR_CHLEN)) == I2S_DATAFORMAT_24B) || \
        ((hi2s->Instance->I2SCFGR & (SPI_I2SCFGR_DATLEN | SPI_I2SCFGR_CHLEN)) == I2S_DATAFORMAT_32B))
    {
      hi2s->TxXferSize  = (Size << 1U);
      hi2s->TxXferCount = (Size << 1U);
    }
    else
    {
      hi2s->TxXferSize  = Size;
      hi2s->TxXferCount = Size;
    }

    /* Set the I2S Tx DMA Half transfer complete callback */
    hi2s->hdmatx->XferHalfCpltCallback = I2S_DMATxHalfCplt;

    /* Set the I2S Tx DMA transfer complete callback */
    hi2s->hdmatx->XferCpltCallback = I2S_DMATxCplt;

    /* Set the DMA error callback */
    hi2s->hdmatx->XferErrorCallback = I2S_DMAError;

    /* Enable the Tx DMA Channel */
    HAL_DMA_Start_IT(hi2s->hdmatx, (uint32_t)hi2s->pTxBuffPtr, (uint32_t)&hi2s->Instance->TXDR, hi2s->TxXferSize);

    /* Check if the I2S Tx request is already enabled */
    if (HAL_IS_BIT_CLR(hi2s->Instance->CFG1, SPI_CFG1_TXDMAEN))
    {
      /* Check if the I2S is disabled to edit CFG1 register */
      if ((hi2s->Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE)
      {
        /* Enable Tx DMA Request */
        SET_BIT(hi2s->Instance->CFG1, SPI_CFG1_TXDMAEN);
      }
      else
      {
        /* Disable I2S peripheral */
        __HAL_I2S_DISABLE(hi2s);

        /* Enable Tx DMA Request */
        SET_BIT(hi2s->Instance->CFG1, SPI_CFG1_TXDMAEN);

        /* Enable I2S peripheral */
        __HAL_I2S_ENABLE(hi2s);
      }
    }

    /* Process Unlocked */
    __HAL_UNLOCK(hi2s);

    return HAL_OK;
  }
  else
  {
    /* Process Unlocked */
    __HAL_UNLOCK(hi2s);
    return HAL_BUSY;
  }
}

/**
  * @brief Receive an amount of data in non-blocking mode with DMA
  * @param  hi2s: pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @param pData: a 16-bit pointer to the Receive data buffer.
  * @param Size: number of data sample to be sent:
  * @note When a 16-bit data frame or a 16-bit data frame extended is selected during the I2S
  *       configuration phase, the Size parameter means the number of 16-bit data length
  *       in the transaction and when a 24-bit data frame or a 32-bit data frame is selected
  *       the Size parameter means the number of 16-bit data length.
  * @note The I2S is kept enabled at the end of transaction to avoid the clock de-synchronization
  *       between Master and Slave(example: audio streaming).
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2S_Receive_DMA(I2S_HandleTypeDef *hi2s, uint16_t *pData, uint16_t Size)
{
  /* Check Mode parameter */
  assert_param(IS_I2S_RX_MODE(hi2s->Init.Mode));

  if ((pData == NULL) || (Size == 0U))
  {
    return  HAL_ERROR;
  }

  /* Process Locked */
  __HAL_LOCK(hi2s);

  if (hi2s->State == HAL_I2S_STATE_READY)
  {
    hi2s->pRxBuffPtr = pData;
    hi2s->State = HAL_I2S_STATE_BUSY_RX;
    hi2s->ErrorCode = HAL_I2S_ERROR_NONE;

    if (((hi2s->Instance->I2SCFGR & (SPI_I2SCFGR_DATLEN | SPI_I2SCFGR_CHLEN)) == I2S_DATAFORMAT_24B) || \
        ((hi2s->Instance->I2SCFGR & (SPI_I2SCFGR_DATLEN | SPI_I2SCFGR_CHLEN)) == I2S_DATAFORMAT_32B))
    {
      hi2s->RxXferSize = (Size << 1U);
      hi2s->RxXferCount = (Size << 1U);
    }
    else
    {
      hi2s->RxXferSize = Size;
      hi2s->RxXferCount = Size;
    }

    /* Set the I2S Rx DMA Half transfer complete callback */
    hi2s->hdmarx->XferHalfCpltCallback = I2S_DMARxHalfCplt;

    /* Set the I2S Rx DMA transfer complete callback */
    hi2s->hdmarx->XferCpltCallback = I2S_DMARxCplt;

    /* Set the DMA error callback */
    hi2s->hdmarx->XferErrorCallback = I2S_DMAError;

    /* Check if Master Receiver mode is selected */
    if ((hi2s->Instance->I2SCFGR & SPI_I2SCFGR_I2SCFG) == I2S_MODE_MASTER_RX)
    {
      /* Clear the Overrun Flag by a read operation to the SPI_DR register followed by a read
      access to the SPI_SR register. */
      __HAL_I2S_CLEAR_OVRFLAG(hi2s);
    }

    /* Enable the Rx DMA Channel */
    HAL_DMA_Start_IT(hi2s->hdmarx, (uint32_t)&hi2s->Instance->RXDR, (uint32_t)hi2s->pRxBuffPtr, hi2s->RxXferSize);

    /* Check if the I2S Rx request is already enabled */
    if (HAL_IS_BIT_CLR(hi2s->Instance->CFG1, SPI_CFG1_RXDMAEN))
    {
      /* Check if the I2S is disabled to edit CFG1 register */
      if ((hi2s->Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE)
      {
        /* Enable Rx DMA Request */
        SET_BIT(hi2s->Instance->CFG1, SPI_CFG1_RXDMAEN);
      }
      else
      {
        /* Disable I2S peripheral */
        __HAL_I2S_DISABLE(hi2s);

        /* Enable Rx DMA Request */
        SET_BIT(hi2s->Instance->CFG1, SPI_CFG1_RXDMAEN);

        /* Enable I2S peripheral */
        __HAL_I2S_ENABLE(hi2s);
      }
    }

    /* Process Unlocked */
    __HAL_UNLOCK(hi2s);

    return HAL_OK;
  }
  else
  {
    /* Process Unlocked */
    __HAL_UNLOCK(hi2s);
    return HAL_BUSY;
  }
}

/**
  * @brief  Pauses the audio stream playing from the Media.
  * @param  hi2s: pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2S_DMAPause(I2S_HandleTypeDef *hi2s)
{
  HAL_StatusTypeDef errorcode;
  __IO uint32_t count;

  /* Process Locked */
  __HAL_LOCK(hi2s);

  /* Initialize local variable  */
  errorcode = HAL_OK;
  count = I2S_DEFAULT_TIMEOUT * (SystemCoreClock / 24U / 1000U);

  /* Check that the I2S peripheral is in master mode */
  if (IS_I2S_MASTER(hi2s->Init.Mode))
  {
    /* Check that a master transfer is on-going */
    if (HAL_IS_BIT_SET(hi2s->Instance->CR1, SPI_CR1_CSTART))
    {
      SET_BIT(hi2s->Instance->CR1, SPI_CR1_CSUSP);
      do
      {
        if (count == 0U)
        {
          SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_TIMEOUT);
          errorcode = HAL_ERROR;
          break;
        }
        count--;
      }
      while (HAL_IS_BIT_SET(hi2s->Instance->CR1, SPI_CR1_CSTART));
    }
  }
  else
  {
    /* Set error code to not supported */
    SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_NOT_SUPPORTED);

    errorcode = HAL_ERROR;
  }

  /* Process Unlocked */
  __HAL_UNLOCK(hi2s);

  return errorcode;
}

/**
  * @brief  Resumes the audio stream playing from the Media.
  * @param  hi2s: pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2S_DMAResume(I2S_HandleTypeDef *hi2s)
{
  HAL_StatusTypeDef errorcode;

  /* Process Locked */
  __HAL_LOCK(hi2s);

  /* Initialize local variable  */
  errorcode = HAL_OK;

  /* Check that the I2S peripheral is in master mode */
  if (IS_I2S_MASTER(hi2s->Init.Mode))
  {
    if ((hi2s->State == HAL_I2S_STATE_BUSY_TX) || (hi2s->State == HAL_I2S_STATE_BUSY_RX) || (hi2s->State == HAL_I2S_STATE_BUSY_TX_RX))
    {
      /* Master transfer start */
      SET_BIT(hi2s->Instance->CR1, SPI_CR1_CSTART);
    }
    else
    {
      /* Set error code to not supported */
      SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_NO_TRANSFER);
      errorcode = HAL_ERROR;
    }
  }
  else
  {
    /* Set error code to not supported */
    SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_NOT_SUPPORTED);
    errorcode = HAL_ERROR;
  }

  /* Process Unlocked */
  __HAL_UNLOCK(hi2s);

  return errorcode;
}

/**
  * @brief  Stops the audio stream playing from the Media.
  *         This API is not supported, it is maintained for backward compatibility.
  * @param  hi2s: pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval HAL_ERROR
  */
HAL_StatusTypeDef HAL_I2S_DMAStop(I2S_HandleTypeDef *hi2s)
{
  /* Set error code to not supported */
  SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_NOT_SUPPORTED);

  return HAL_ERROR;
}

/**
  * @brief  This function handles I2S interrupt request.
  * @param  hi2s: pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval None
  */
void HAL_I2S_IRQHandler(I2S_HandleTypeDef *hi2s)
{
  uint32_t itsource = hi2s->Instance->IER;
  uint32_t i2ssr = hi2s->Instance->SR;

  /* I2S in mode Receiver ------------------------------------------------*/
  if (((i2ssr & I2S_FLAG_OVR) != I2S_FLAG_OVR) &&
      ((i2ssr & I2S_FLAG_RXP) == I2S_FLAG_RXP) && ((itsource & I2S_IT_RXP) != RESET))
  {
    hi2s->RxISR(hi2s);
    return;
  }

  /* I2S in mode Transmitter ---------------------------------------------*/
  if (((i2ssr & I2S_FLAG_TXP) == I2S_FLAG_TXP) && ((itsource & I2S_IT_TXP) != RESET))
  {
    hi2s->TxISR(hi2s);
    return;
  }

  /* I2S interrupt error -------------------------------------------------*/
  if ((itsource & I2S_IT_ERR) != RESET)
  {
    /* I2S Overrun error interrupt occurred ---------------------------------*/
    if ((i2ssr & I2S_FLAG_OVR) == I2S_FLAG_OVR)
    {
      /* Disable RXP and ERR interrupt */
      __HAL_I2S_DISABLE_IT(hi2s, (I2S_IT_RXP | I2S_IT_ERR));

      /* Set the error code and execute error callback */
      SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_OVR);
    }

    /* I2S Underrun error interrupt occurred --------------------------------*/
    if ((i2ssr & I2S_FLAG_UDR) == I2S_FLAG_UDR)
    {
      /* Disable TXP and ERR interrupt */
      __HAL_I2S_DISABLE_IT(hi2s, (I2S_IT_TXP | I2S_IT_ERR));

      /* Set the error code and execute error callback */
      SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_UDR);
    }

    /* Set the I2S State ready */
    hi2s->State = HAL_I2S_STATE_READY;
    /* Call the Error Callback */
#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)
    hi2s->ErrorCallback(hi2s);
#else
    HAL_I2S_ErrorCallback(hi2s);
#endif /* USE_HAL_I2S_REGISTER_CALLBACKS */
  }
}


/**
  * @brief This function handles I2S Communication Timeout.
  * @param  hi2s: pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @param Flag: Flag checked
  * @param State: Value of the flag expected
  * @param Timeout: Duration of the timeout
  * @retval HAL status
  */
static HAL_StatusTypeDef I2S_WaitFlagStateUntilTimeout(I2S_HandleTypeDef *hi2s, uint32_t Flag, uint32_t State, uint32_t Timeout)
{
  uint32_t tickstart = 0U;

  /* Get tick */
  tickstart = HAL_GetTick();

  /* Wait until flag is set */
  while ((__HAL_I2S_GET_FLAG(hi2s, Flag) ? SET : RESET) != State)
  {
    if (Timeout != HAL_MAX_DELAY)
    {
      if ((Timeout == 0U) || ((HAL_GetTick() - tickstart) > Timeout))
      {
        /* Set the I2S State ready */
        hi2s->State = HAL_I2S_STATE_READY;

        /* Process Unlocked */
        __HAL_UNLOCK(hi2s);

        return HAL_TIMEOUT;
      }
    }
  }

  return HAL_OK;
}

/**
  * @brief Tx Transfer Half completed callbacks
  * @param  hi2s: pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval None
  */
__weak void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hi2s);

  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_I2S_TxHalfCpltCallback could be implemented in the user file
   */
}

/**
  * @brief  Tx Transfer completed callbacks
  * @param  hi2s: pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval None
  */
__weak void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hi2s);

  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_I2S_TxCpltCallback could be implemented in the user file
   */
}

/**
  * @brief  Rx Transfer half completed callbacks
  * @param  hi2s: pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval None
  */
__weak void HAL_I2S_RxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hi2s);

  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_I2S_RxHalfCpltCallback could be implemented in the user file
   */
}

/**
  * @brief Rx Transfer completed callbacks
  * @param  hi2s: pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval None
  */
__weak void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hi2s);

  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_I2S_RxCpltCallback could be implemented in the user file
   */
}

/**
  * @brief I2S error callbacks
  * @param  hi2s: pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval None
  */
__weak void HAL_I2S_ErrorCallback(I2S_HandleTypeDef *hi2s)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hi2s);

  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_I2S_ErrorCallback could be implemented in the user file
   */
}

/**
  * @}
  */

/** @defgroup I2S_Exported_Functions_Group3 Peripheral State and Errors functions
  *  @brief   Peripheral State functions
  *
@verbatim
 ===============================================================================
                      ##### Peripheral State and Errors functions #####
 ===============================================================================
    [..]
    This subsection permits to get in run-time the status of the peripheral
    and the data flow.

@endverbatim
  * @{
  */

/**
  * @brief  Return the I2S state
  * @param  hi2s: pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval HAL state
  */
HAL_I2S_StateTypeDef HAL_I2S_GetState(I2S_HandleTypeDef *hi2s)
{
  return hi2s->State;
}

/**
  * @brief  Return the I2S error code
  * @param  hi2s: pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval I2S Error Code
  */
uint32_t HAL_I2S_GetError(I2S_HandleTypeDef *hi2s)
{
  return hi2s->ErrorCode;
}
/**
  * @}
  */

/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/
/** @addtogroup I2S_Private
  * @{
  */
/**
  * @brief  DMA I2S transmit process complete callback
  * @param  hdma: pointer to a DMA_HandleTypeDef structure that contains
  *                the configuration information for the specified DMA module.
  * @retval None
  */
static void I2S_DMATxCplt(DMA_HandleTypeDef *hdma)
{
  I2S_HandleTypeDef *hi2s = (I2S_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;

  if (((((DMA_Stream_TypeDef *)hdma->Instance)->CR) & DMA_SxCR_CIRC) == 0U)
  {
    /* Check if the I2S is disabled to edit CFG1 register */
    if ((hi2s->Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE)
    {
      /* Disable Tx DMA Request */
      CLEAR_BIT(hi2s->Instance->CFG1, SPI_CFG1_TXDMAEN);
    }
    else
    {
      /* Disable I2S peripheral */
      __HAL_I2S_DISABLE(hi2s);

      /* Disable Tx DMA Request */
      CLEAR_BIT(hi2s->Instance->CFG1, SPI_CFG1_TXDMAEN);

      /* Enable I2S peripheral */
      __HAL_I2S_ENABLE(hi2s);
    }

    hi2s->TxXferCount = 0U;

    if (hi2s->State == HAL_I2S_STATE_BUSY_TX_RX)
    {
      if (hi2s->RxXferCount == 0U)
      {
        hi2s->State = HAL_I2S_STATE_READY;
      }
    }
    else
    {
      hi2s->State = HAL_I2S_STATE_READY;
    }
  }
#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)
  hi2s->TxCpltCallback(hi2s);
#else
  HAL_I2S_TxCpltCallback(hi2s);
#endif /* USE_HAL_I2S_REGISTER_CALLBACKS */
}

/**
  * @brief  DMA I2S transmit process half complete callback
  * @param  hdma: pointer to a DMA_HandleTypeDef structure that contains
                  the configuration information for the specified DMA module.
  * @retval None
  */
static void I2S_DMATxHalfCplt(DMA_HandleTypeDef *hdma)
{
  I2S_HandleTypeDef *hi2s = (I2S_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;

#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)
  hi2s->TxHalfCpltCallback(hi2s);
#else
  HAL_I2S_TxHalfCpltCallback(hi2s);
#endif /* USE_HAL_I2S_REGISTER_CALLBACKS */
}

/**
  * @brief  DMA I2S receive process complete callback
  * @param  hdma: pointer to a DMA_HandleTypeDef structure that contains
  *                the configuration information for the specified DMA module.
  * @retval None
  */
static void I2S_DMARxCplt(DMA_HandleTypeDef *hdma)
{
  I2S_HandleTypeDef *hi2s = (I2S_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;

  if (((((DMA_Stream_TypeDef *)hdma->Instance)->CR) & DMA_SxCR_CIRC) == 0U)
  {
    /* Check if the I2S is disabled to edit CFG1 register */
    if ((hi2s->Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE)
    {
      /* Disable Rx DMA Request */
      CLEAR_BIT(hi2s->Instance->CFG1, SPI_CFG1_RXDMAEN);
    }
    else
    {
      /* Disable I2S peripheral */
      __HAL_I2S_DISABLE(hi2s);

      /* Disable Rx DMA Request */
      CLEAR_BIT(hi2s->Instance->CFG1, SPI_CFG1_RXDMAEN);

      /* Enable I2S peripheral */
      __HAL_I2S_ENABLE(hi2s);
    }

    hi2s->RxXferCount = 0U;
    if (hi2s->State == HAL_I2S_STATE_BUSY_TX_RX)
    {
      if (hi2s->TxXferCount == 0U)
      {
        hi2s->State = HAL_I2S_STATE_READY;
      }
    }
    else
    {
      hi2s->State = HAL_I2S_STATE_READY;
    }
  }
#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)
  hi2s->RxCpltCallback(hi2s);
#else
  HAL_I2S_RxCpltCallback(hi2s);
#endif /* USE_HAL_I2S_REGISTER_CALLBACKS */
}

/**
  * @brief  DMA I2S receive process half complete callback
  * @param  hdma: pointer to a DMA_HandleTypeDef structure that contains
  *                the configuration information for the specified DMA module.
  * @retval None
  */
static void I2S_DMARxHalfCplt(DMA_HandleTypeDef *hdma)
{
  I2S_HandleTypeDef *hi2s = (I2S_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;

#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)
  hi2s->RxHalfCpltCallback(hi2s);
#else
  HAL_I2S_RxHalfCpltCallback(hi2s);
#endif /* USE_HAL_I2S_REGISTER_CALLBACKS */
}

/**
  * @brief  DMA I2S communication error callback
  * @param  hdma: pointer to a DMA_HandleTypeDef structure that contains
  *               the configuration information for the specified DMA module.
  * @retval None
  */
static void I2S_DMAError(DMA_HandleTypeDef *hdma)
{
  I2S_HandleTypeDef *hi2s = (I2S_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;

  /* Check if the I2S is disabled to edit CFG1 register */
  if ((hi2s->Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE)
  {
    /* Disable Rx and Tx DMA Request */
    CLEAR_BIT(hi2s->Instance->CFG1, (SPI_CFG1_RXDMAEN | SPI_CFG1_TXDMAEN));
  }
  else
  {
    /* Disable I2S peripheral */
    __HAL_I2S_DISABLE(hi2s);

    /* Disable Rx and Tx DMA Request */
    CLEAR_BIT(hi2s->Instance->CFG1, (SPI_CFG1_RXDMAEN | SPI_CFG1_TXDMAEN));

    /* Enable I2S peripheral */
    __HAL_I2S_ENABLE(hi2s);
  }

  hi2s->TxXferCount = 0U;
  hi2s->RxXferCount = 0U;

  hi2s->State = HAL_I2S_STATE_READY;

  /* Set the error code and execute error callback*/
  SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_DMA);
#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)
  hi2s->ErrorCallback(hi2s);
#else
  HAL_I2S_ErrorCallback(hi2s);
#endif /* USE_HAL_I2S_REGISTER_CALLBACKS */
}

/**
  * @brief  Manage the 16-bit receive in Interrupt context.
  * @param  hi2s: pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval None
  */
static void I2S_RxISR_16BIT(struct __I2S_HandleTypeDef *hi2s)
{
  /* Receive data in 16 Bit mode */
  *((uint16_t *)hi2s->pRxBuffPtr) = *((__IO uint16_t *)&hi2s->Instance->RXDR);
  hi2s->pRxBuffPtr++;
  hi2s->RxXferCount--;

  if (hi2s->RxXferCount == 0U)
  {
    I2S_CloseRx_ISR(hi2s);
  }
}

/**
  * @brief  Manage the 32-bit receive in Interrupt context.
  * @param  hi2s: pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval None
  */
static void I2S_RxISR_32BIT(struct __I2S_HandleTypeDef *hi2s)
{
  /* Receive data in 32 Bit mode */
  *((uint32_t *)hi2s->pRxBuffPtr) = *((__IO uint32_t *)&hi2s->Instance->RXDR);
  hi2s->pRxBuffPtr += 2U;
  hi2s->RxXferCount--;

  if (hi2s->RxXferCount == 0U)
  {
    I2S_CloseRx_ISR(hi2s);
  }
}

/**
  * @brief  Handle the data 16-bit transmit in Interrupt mode.
  * @param  hi2s: pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval None
  */
static void I2S_TxISR_16BIT(struct __I2S_HandleTypeDef *hi2s)
{
  /* Transmit data in 16 Bit mode */
  *((__IO uint16_t *)&hi2s->Instance->TXDR) = *((uint16_t *)hi2s->pTxBuffPtr);
  hi2s->pTxBuffPtr++;
  hi2s->TxXferCount--;

  if (hi2s->TxXferCount == 0U)
  {
    I2S_CloseTx_ISR(hi2s);
  }
}

/**
  * @brief  Handle the data 32-bit transmit in Interrupt mode.
  * @param  hi2s: pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval None
  */
static void I2S_TxISR_32BIT(struct __I2S_HandleTypeDef *hi2s)
{
  /* Transmit data in 32 Bit mode */
  *((__IO uint32_t *)&hi2s->Instance->TXDR) = *((uint32_t *)hi2s->pTxBuffPtr);
  hi2s->pTxBuffPtr += 2U;
  hi2s->TxXferCount--;

  if (hi2s->TxXferCount == 0U)
  {
    I2S_CloseTx_ISR(hi2s);
  }
}

/**
  * @brief  Handle the end of the RX transaction.
  * @param  hi2s: pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval None
  */
static void I2S_CloseRx_ISR(I2S_HandleTypeDef *hi2s)
{
  /* Disable RXP and ERR interrupt */
  __HAL_I2S_DISABLE_IT(hi2s, (I2S_IT_RXP | I2S_IT_ERR));

  /* Clear underrun flag in 1 Line communication mode because transmitted is not needed */
  if (IS_I2S_FD_MODE(hi2s->Init.Mode))
  {
    *((__IO uint8_t *)&hi2s->Instance->TXDR) = 0x01U;
    __HAL_I2S_CLEAR_UDRFLAG(hi2s);
  }

  hi2s->State = HAL_I2S_STATE_READY;

#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)
  if (hi2s->ErrorCode == HAL_I2S_ERROR_NONE)
  {
    hi2s->RxCpltCallback(hi2s);
  }
  else
  {
    hi2s->ErrorCallback(hi2s);
  }
#else
  if (hi2s->ErrorCode == HAL_I2S_ERROR_NONE)
  {
    HAL_I2S_RxCpltCallback(hi2s);
  }
  else
  {
    HAL_I2S_ErrorCallback(hi2s);
  }
#endif /* USE_HAL_I2S_REGISTER_CALLBACKS */
}

/**
  * @brief  Handle the end of the TX transaction.
  * @param  hi2s: pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval None
  */
static void I2S_CloseTx_ISR(I2S_HandleTypeDef *hi2s)
{
  /* Disable TXP and ERR interrupt */
  __HAL_I2S_DISABLE_IT(hi2s, (I2S_IT_TXP | I2S_IT_ERR));

  /* Clear overrun flag in 2 Lines communication mode because received is not read */
  if (IS_I2S_FD_MODE(hi2s->Init.Mode))
  {
    __HAL_I2S_CLEAR_OVRFLAG(hi2s);
  }

  hi2s->State = HAL_I2S_STATE_READY;

#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)
  if (hi2s->ErrorCode != HAL_I2S_ERROR_NONE)
  {
    hi2s->ErrorCallback(hi2s);
  }
  else
  {
    hi2s->TxCpltCallback(hi2s);
  }
#else
  if (hi2s->ErrorCode != HAL_I2S_ERROR_NONE)
  {
    HAL_I2S_ErrorCallback(hi2s);
  }
  else
  {
    HAL_I2S_TxCpltCallback(hi2s);
  }
#endif /* USE_HAL_I2S_REGISTER_CALLBACKS */
}

/**
  * @}
  */
#endif /* HAL_I2S_MODULE_ENABLED */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

