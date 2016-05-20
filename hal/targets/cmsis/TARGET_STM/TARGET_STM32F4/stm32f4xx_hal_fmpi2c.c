/**
  ******************************************************************************
  * @file    stm32f4xx_hal_fmpi2c.c
  * @author  MCD Application Team
  * @version V1.4.4
  * @date    22-January-2016
  * @brief   FMPI2C HAL module driver.
  *    
  *          This file provides firmware functions to manage the following 
  *          functionalities of the Inter Integrated Circuit (FMPI2C) peripheral:
  *           + Initialization and de-initialization functions
  *           + IO operation functions
  *           + Peripheral State and Errors functions
  *         
  @verbatim
  ==============================================================================
                        ##### How to use this driver #####
  ==============================================================================
    [..]
    The FMPI2C HAL driver can be used as follows:
    
    (#) Declare a FMPI2C_HandleTypeDef handle structure, for example:
        FMPI2C_HandleTypeDef  hfmpi2c; 

    (#)Initialize the FMPI2C low level resources by implementing the HAL_FMPI2C_MspInit() API:
        (##) Enable the FMPI2Cx interface clock
        (##) FMPI2C pins configuration
            (+++) Enable the clock for the FMPI2C GPIOs
            (+++) Configure FMPI2C pins as alternate function open-drain
        (##) NVIC configuration if you need to use interrupt process
            (+++) Configure the FMPI2Cx interrupt priority
            (+++) Enable the NVIC FMPI2C IRQ Channel
        (##) DMA Configuration if you need to use DMA process
            (+++) Declare a DMA_HandleTypeDef handle structure for the transmit or receive channel
            (+++) Enable the DMAx interface clock using
            (+++) Configure the DMA handle parameters
            (+++) Configure the DMA Tx or Rx channel
            (+++) Associate the initialized DMA handle to the hfmpi2c DMA Tx or Rx handle
            (+++) Configure the priority and enable the NVIC for the transfer complete interrupt on 
                  the DMA Tx or Rx channel

    (#) Configure the Communication Clock Timing, Own Address1, Master Addressing mode, Dual Addressing mode,
        Own Address2, Own Address2 Mask, General call and Nostretch mode in the hfmpi2c Init structure.

    (#) Initialize the FMPI2C registers by calling the HAL_FMPI2C_Init(), configures also the low level Hardware 
        (GPIO, CLOCK, NVIC...etc) by calling the customized HAL_FMPI2C_MspInit(&hfmpi2c) API.

    (#) To check if target device is ready for communication, use the function HAL_FMPI2C_IsDeviceReady()

    (#) For FMPI2C IO and IO MEM operations, three operation modes are available within this driver :

    *** Polling mode IO operation ***
    =================================
    [..]
      (+) Transmit in master mode an amount of data in blocking mode using HAL_FMPI2C_Master_Transmit()
      (+) Receive in master mode an amount of data in blocking mode using HAL_FMPI2C_Master_Receive()
      (+) Transmit in slave mode an amount of data in blocking mode using HAL_FMPI2C_Slave_Transmit()
      (+) Receive in slave mode an amount of data in blocking mode using HAL_FMPI2C_Slave_Receive()

    *** Polling mode IO MEM operation ***
    =====================================
    [..]
      (+) Write an amount of data in blocking mode to a specific memory address using HAL_FMPI2C_Mem_Write()
      (+) Read an amount of data in blocking mode from a specific memory address using HAL_FMPI2C_Mem_Read()


    *** Interrupt mode IO operation ***
    ===================================
    [..]
      (+) Transmit in master mode an amount of data in no blocking mode using HAL_FMPI2C_Master_Transmit_IT()
      (+) At transmission end of transfer, HAL_FMPI2C_MasterTxCpltCallback() is executed and user can
           add his own code by customization of function pointer HAL_FMPI2C_MasterTxCpltCallback()
      (+) Receive in master mode an amount of data in no blocking mode using HAL_FMPI2C_Master_Receive_IT()
      (+) At reception end of transfer, HAL_FMPI2C_MasterRxCpltCallback() is executed and user can
           add his own code by customization of function pointer HAL_FMPI2C_MasterRxCpltCallback()
      (+) Transmit in slave mode an amount of data in no blocking mode using HAL_FMPI2C_Slave_Transmit_IT()
      (+) At transmission end of transfer, HAL_FMPI2C_SlaveTxCpltCallback() is executed and user can
           add his own code by customization of function pointer HAL_FMPI2C_SlaveTxCpltCallback()
      (+) Receive in slave mode an amount of data in no blocking mode using HAL_FMPI2C_Slave_Receive_IT()
      (+) At reception end of transfer, HAL_FMPI2C_SlaveRxCpltCallback() is executed and user can
           add his own code by customization of function pointer HAL_FMPI2C_SlaveRxCpltCallback()
      (+) In case of transfer Error, HAL_FMPI2C_ErrorCallback() function is executed and user can
           add his own code by customization of function pointer HAL_FMPI2C_ErrorCallback()
		   
	*** Interrupt mode IO sequential operation ***
    ==============================================
    [..]
      (+@) These interfaces allow to manage a sequential transfer with a repeated start condition
          when a direction change during transfer
      (+) A specific option manage the different steps of a sequential transfer
      (+) Different steps option FMPI2C_XferOptions_definition are listed below :
      (++) FMPI2C_FIRST_AND_LAST_FRAME: No sequential usage, functional is same as associated interfaces in no sequential mode 
      (++) FMPI2C_FIRST_FRAME: Sequential usage, this option allow to manage a start condition with data to transfer without a final stop condition 
      (++) FMPI2C_NEXT_FRAME: Sequential usage, this option allow to manage a restart condition with new data to transfer if the direction change or 
             manage only the new data to transfer if no direction change and without a final stop condition in both cases
      (++) FMPI2C_LAST_FRAME: Sequential usage, this option allow to manage a restart condition with new data to transfer if the direction change or 
             manage only the new data to transfer if no direction change and with a final stop condition in both cases

      (+) Sequential transmit in master FMPI2C mode an amount of data in non blocking mode using HAL_FMPI2C_Master_Sequential_Transmit_IT()
      (++) At transmission end of current frame transfer, HAL_FMPI2C_MasterTxCpltCallback() is executed and user can
           add his own code by customization of function pointer HAL_FMPI2C_MasterTxCpltCallback()
      (+) Sequential receive in master FMPI2C mode an amount of data in non blocking mode using HAL_FMPI2C_Master_Sequential_Receive_IT()
      (++) At reception end of current frame transfer, HAL_FMPI2C_MasterRxCpltCallback() is executed and user can
           add his own code by customization of function pointer HAL_FMPI2C_MasterRxCpltCallback()
      (+) Abort a master FMPI2C process communication with Interrupt using HAL_FMPI2C_Master_Abort_IT()
      (++) The associated previous transfer callback is called at the end of abort process
      (++) mean HAL_FMPI2C_MasterTxCpltCallback() in case of previous state was master transmit
      (++) mean HAL_FMPI2C_MasterRxCpltCallback() in case of previous state was master receive
      (+) Enable/disable the Address listen mode in slave FMPI2C mode
           using HAL_FMPI2C_EnableListen_IT() HAL_FMPI2C_DisableListen_IT()
      (++) When address slave FMPI2C match, HAL_FMPI2C_AddrCallback() is executed and user can
           add his own code to check the Address Match Code and the transmission direction request by master (Write/Read).
      (++) At Listen mode end HAL_FMPI2C_ListenCpltCallback() is executed and user can
           add his own code by customization of function pointer HAL_FMPI2C_ListenCpltCallback()
      (+) Sequential transmit in slave FMPI2C mode an amount of data in non-blocking mode using HAL_FMPI2C_Slave_Sequential_Transmit_IT()
      (++) At transmission end of current frame transfer, HAL_FMPI2C_SlaveTxCpltCallback() is executed and user can
           add his own code by customization of function pointer HAL_FMPI2C_SlaveTxCpltCallback()
      (+) Sequential receive in slave FMPI2C mode an amount of data in non-blocking mode using HAL_FMPI2C_Slave_Sequential_Receive_IT()
      (++) At reception end of current frame transfer, HAL_FMPI2C_SlaveRxCpltCallback() is executed and user can
           add his own code by customization of function pointer HAL_FMPI2C_SlaveRxCpltCallback()	   

    *** Interrupt mode IO MEM operation ***
    =======================================
    [..]
      (+) Write an amount of data in no blocking mode with Interrupt to a specific memory address using
          HAL_FMPI2C_Mem_Write_IT()
      (+) At Memory end of write transfer, HAL_FMPI2C_MemTxCpltCallback() is executed and user can
           add his own code by customization of function pointer HAL_FMPI2C_MemTxCpltCallback()
      (+) Read an amount of data in no blocking mode with Interrupt from a specific memory address using
          HAL_FMPI2C_Mem_Read_IT()
      (+) At Memory end of read transfer, HAL_FMPI2C_MemRxCpltCallback() is executed and user can
           add his own code by customization of function pointer HAL_FMPI2C_MemRxCpltCallback()
      (+) In case of transfer Error, HAL_FMPI2C_ErrorCallback() function is executed and user can
           add his own code by customization of function pointer HAL_FMPI2C_ErrorCallback()

    *** DMA mode IO operation ***
    ==============================
    [..]
      (+) Transmit in master mode an amount of data in no blocking mode (DMA) using
          HAL_FMPI2C_Master_Transmit_DMA()
      (+) At transmission end of transfer, HAL_FMPI2C_MasterTxCpltCallback() is executed and user can
           add his own code by customization of function pointer HAL_FMPI2C_MasterTxCpltCallback()
      (+) Receive in master mode an amount of data in no blocking mode (DMA) using
          HAL_FMPI2C_Master_Receive_DMA()
      (+) At reception end of transfer, HAL_FMPI2C_MasterRxCpltCallback() is executed and user can
           add his own code by customization of function pointer HAL_FMPI2C_MasterRxCpltCallback()
      (+) Transmit in slave mode an amount of data in no blocking mode (DMA) using
          HAL_FMPI2C_Slave_Transmit_DMA()
      (+) At transmission end of transfer, HAL_FMPI2C_SlaveTxCpltCallback() is executed and user can
           add his own code by customization of function pointer HAL_FMPI2C_SlaveTxCpltCallback()
      (+) Receive in slave mode an amount of data in no blocking mode (DMA) using
          HAL_FMPI2C_Slave_Receive_DMA()
      (+) At reception end of transfer, HAL_FMPI2C_SlaveRxCpltCallback() is executed and user can
           add his own code by customization of function pointer HAL_FMPI2C_SlaveRxCpltCallback()
      (+) In case of transfer Error, HAL_FMPI2C_ErrorCallback() function is executed and user can
           add his own code by customization of function pointer HAL_FMPI2C_ErrorCallback()

    *** DMA mode IO MEM operation ***
    =================================
    [..]
      (+) Write an amount of data in no blocking mode with DMA to a specific memory address using
          HAL_FMPI2C_Mem_Write_DMA()
      (+) At Memory end of write transfer, HAL_FMPI2C_MemTxCpltCallback() is executed and user can
           add his own code by customization of function pointer HAL_FMPI2C_MemTxCpltCallback()
      (+) Read an amount of data in no blocking mode with DMA from a specific memory address using
          HAL_FMPI2C_Mem_Read_DMA()
      (+) At Memory end of read transfer, HAL_FMPI2C_MemRxCpltCallback() is executed and user can
           add his own code by customization of function pointer HAL_FMPI2C_MemRxCpltCallback()
      (+) In case of transfer Error, HAL_FMPI2C_ErrorCallback() function is executed and user can
           add his own code by customization of function pointer HAL_FMPI2C_ErrorCallback()

     *** FMPI2C HAL driver macros list ***
     ==================================
     [..]
       Below the list of most used macros in FMPI2C HAL driver.

      (+) __HAL_FMPI2C_ENABLE: Enable the FMPI2C peripheral
      (+) __HAL_FMPI2C_DISABLE: Disable the FMPI2C peripheral
      (+) __HAL_FMPI2C_GET_FLAG : Checks whether the specified FMPI2C flag is set or not
      (+) __HAL_FMPI2C_CLEAR_FLAG : Clears the specified FMPI2C pending flag
      (+) __HAL_FMPI2C_ENABLE_IT: Enables the specified FMPI2C interrupt
      (+) __HAL_FMPI2C_DISABLE_IT: Disables the specified FMPI2C interrupt

     [..]
       (@) You can refer to the FMPI2C HAL driver header file for more useful macros

  @endverbatim
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/** @addtogroup STM32F4xx_HAL_Driver
  * @{
  */

/** @defgroup FMPI2C FMPI2C
  * @brief FMPI2C HAL module driver
  * @{
  */

#ifdef HAL_FMPI2C_MODULE_ENABLED

#if defined(STM32F410Tx) || defined(STM32F410Cx) || defined(STM32F410Rx) || defined(STM32F446xx)

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/** @defgroup FMPI2C_Private_Define FMPI2C Private Define
  * @{
  */
#define TIMING_CLEAR_MASK      ((uint32_t)0xF0FFFFFFU)  /*!< FMPI2C TIMING clear register Mask */
#define FMPI2C_TIMEOUT_ADDR    ((uint32_t)10000U)       /*!< 10 s  */
#define FMPI2C_TIMEOUT_BUSY    ((uint32_t)25U)          /*!< 25 ms */
#define FMPI2C_TIMEOUT_DIR     ((uint32_t)25U)          /*!< 25 ms */
#define FMPI2C_TIMEOUT_RXNE    ((uint32_t)25U)          /*!< 25 ms */
#define FMPI2C_TIMEOUT_STOPF   ((uint32_t)25U)          /*!< 25 ms */
#define FMPI2C_TIMEOUT_TC      ((uint32_t)25U)          /*!< 25 ms */
#define FMPI2C_TIMEOUT_TCR     ((uint32_t)25U)          /*!< 25 ms */
#define FMPI2C_TIMEOUT_TXIS    ((uint32_t)25U)          /*!< 25 ms */
#define FMPI2C_TIMEOUT_FLAG    ((uint32_t)25U)          /*!< 25 ms */

#define MAX_NBYTE_SIZE      255U
#define SlaveAddr_SHIFT     7U
#define SlaveAddr_MSK       0x06U

/* Private define for @ref PreviousState usage */
#define FMPI2C_STATE_MSK             ((uint32_t)((HAL_FMPI2C_STATE_BUSY_TX | HAL_FMPI2C_STATE_BUSY_RX) & (~HAL_FMPI2C_STATE_READY))) /*!< Mask State define, keep only RX and TX bits            */
#define FMPI2C_STATE_NONE            ((uint32_t)(HAL_FMPI2C_MODE_NONE))                                                        /*!< Default Value                                          */
#define FMPI2C_STATE_MASTER_BUSY_TX  ((uint32_t)((HAL_FMPI2C_STATE_BUSY_TX & FMPI2C_STATE_MSK) | HAL_FMPI2C_MODE_MASTER))            /*!< Master Busy TX, combinaison of State LSB and Mode enum */
#define FMPI2C_STATE_MASTER_BUSY_RX  ((uint32_t)((HAL_FMPI2C_STATE_BUSY_RX & FMPI2C_STATE_MSK) | HAL_FMPI2C_MODE_MASTER))            /*!< Master Busy RX, combinaison of State LSB and Mode enum */
#define FMPI2C_STATE_SLAVE_BUSY_TX   ((uint32_t)((HAL_FMPI2C_STATE_BUSY_TX & FMPI2C_STATE_MSK) | HAL_FMPI2C_MODE_SLAVE))             /*!< Slave Busy TX, combinaison of State LSB and Mode enum  */
#define FMPI2C_STATE_SLAVE_BUSY_RX   ((uint32_t)((HAL_FMPI2C_STATE_BUSY_RX & FMPI2C_STATE_MSK) | HAL_FMPI2C_MODE_SLAVE))             /*!< Slave Busy RX, combinaison of State LSB and Mode enum  */
#define FMPI2C_STATE_MEM_BUSY_TX     ((uint32_t)((HAL_FMPI2C_STATE_BUSY_TX & FMPI2C_STATE_MSK) | HAL_FMPI2C_MODE_MEM))               /*!< Memory Busy TX, combinaison of State LSB and Mode enum */
#define FMPI2C_STATE_MEM_BUSY_RX     ((uint32_t)((HAL_FMPI2C_STATE_BUSY_RX & FMPI2C_STATE_MSK) | HAL_FMPI2C_MODE_MEM))               /*!< Memory Busy RX, combinaison of State LSB and Mode enum */

/**
  * @}
  */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/** @defgroup FMPI2C_Private_Functions FMPI2C Private Functions
  * @{
  */
static void FMPI2C_DMAMasterTransmitCplt(DMA_HandleTypeDef *hdma);
static void FMPI2C_DMAMasterReceiveCplt(DMA_HandleTypeDef *hdma);
static void FMPI2C_DMASlaveTransmitCplt(DMA_HandleTypeDef *hdma);
static void FMPI2C_DMASlaveReceiveCplt(DMA_HandleTypeDef *hdma);
static void FMPI2C_DMAMemTransmitCplt(DMA_HandleTypeDef *hdma);
static void FMPI2C_DMAMemReceiveCplt(DMA_HandleTypeDef *hdma);
static void FMPI2C_DMAError(DMA_HandleTypeDef *hdma);

static HAL_StatusTypeDef FMPI2C_RequestMemoryWrite(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint32_t Timeout);
static HAL_StatusTypeDef FMPI2C_RequestMemoryRead(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint32_t Timeout);
static HAL_StatusTypeDef FMPI2C_WaitOnFlagUntilTimeout(FMPI2C_HandleTypeDef *hfmpi2c, uint32_t Flag, FlagStatus Status, uint32_t Timeout);
static HAL_StatusTypeDef FMPI2C_WaitOnTXISFlagUntilTimeout(FMPI2C_HandleTypeDef *hfmpi2c, uint32_t Timeout);
static HAL_StatusTypeDef FMPI2C_WaitOnRXNEFlagUntilTimeout(FMPI2C_HandleTypeDef *hfmpi2c, uint32_t Timeout);
static HAL_StatusTypeDef FMPI2C_WaitOnSTOPFlagUntilTimeout(FMPI2C_HandleTypeDef *hfmpi2c, uint32_t Timeout);
static HAL_StatusTypeDef FMPI2C_IsAcknowledgeFailed(FMPI2C_HandleTypeDef *hfmpi2c, uint32_t Timeout);

static HAL_StatusTypeDef FMPI2C_Master_ISR(FMPI2C_HandleTypeDef *hfmpi2c);
static HAL_StatusTypeDef FMPI2C_Slave_ISR(FMPI2C_HandleTypeDef *hfmpi2c);

static HAL_StatusTypeDef FMPI2C_Enable_IRQ(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t InterruptRequest);
static HAL_StatusTypeDef FMPI2C_Disable_IRQ(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t InterruptRequest);

static void FMPI2C_TransferConfig(FMPI2C_HandleTypeDef *hfmpi2c,  uint16_t DevAddress, uint8_t Size, uint32_t Mode, uint32_t Request);
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/

/** @defgroup FMPI2C_Exported_Functions FMPI2C Exported Functions
  * @{
  */

/** @defgroup FMPI2C_Exported_Functions_Group1 Initialization and de-initialization functions
  * @brief    Initialization and Configuration functions 
  *
@verbatim    
 ===============================================================================
              ##### Initialization and de-initialization functions #####
 ===============================================================================
    [..]  This subsection provides a set of functions allowing to initialize and 
          deinitialize the FMPI2Cx peripheral:

      (+) User must Implement HAL_FMPI2C_MspInit() function in which he configures 
          all related peripherals resources (CLOCK, GPIO, DMA, IT and NVIC ).

      (+) Call the function HAL_FMPI2C_Init() to configure the selected device with 
          the selected configuration:
        (++) Clock Timing
        (++) Own Address 1
        (++) Addressing mode (Master, Slave)
        (++) Dual Addressing mode
        (++) Own Address 2
        (++) Own Address 2 Mask
        (++) General call mode
        (++) Nostretch mode

      (+) Call the function HAL_FMPI2C_DeInit() to restore the default configuration 
          of the selected FMPI2Cx peripheral.       

@endverbatim
  * @{
  */

/**
  * @brief  Initializes the FMPI2C according to the specified parameters 
  *         in the FMPI2C_InitTypeDef and initialize the associated handle.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Init(FMPI2C_HandleTypeDef *hfmpi2c)
{
  /* Check the FMPI2C handle allocation */
  if(hfmpi2c == NULL)
  {
    return HAL_ERROR;
  }

  /* Check the parameters */
  assert_param(IS_FMPI2C_ALL_INSTANCE(hfmpi2c->Instance));
  assert_param(IS_FMPI2C_OWN_ADDRESS1(hfmpi2c->Init.OwnAddress1));
  assert_param(IS_FMPI2C_ADDRESSING_MODE(hfmpi2c->Init.AddressingMode));
  assert_param(IS_FMPI2C_DUAL_ADDRESS(hfmpi2c->Init.DualAddressMode));
  assert_param(IS_FMPI2C_OWN_ADDRESS2(hfmpi2c->Init.OwnAddress2));
  assert_param(IS_FMPI2C_OWN_ADDRESS2_MASK(hfmpi2c->Init.OwnAddress2Masks));
  assert_param(IS_FMPI2C_GENERAL_CALL(hfmpi2c->Init.GeneralCallMode));
  assert_param(IS_FMPI2C_NO_STRETCH(hfmpi2c->Init.NoStretchMode));

  if(hfmpi2c->State == HAL_FMPI2C_STATE_RESET)
  {
    /* Allocate lock resource and initialize it */
    hfmpi2c->Lock = HAL_UNLOCKED;

    /* Init the low level hardware : GPIO, CLOCK, CORTEX...etc */
    HAL_FMPI2C_MspInit(hfmpi2c);
  }

  hfmpi2c->State = HAL_FMPI2C_STATE_BUSY;
  
  /* Disable the selected FMPI2C peripheral */
  __HAL_FMPI2C_DISABLE(hfmpi2c);

  /*---------------------------- FMPI2Cx TIMINGR Configuration ------------------*/
  /* Configure FMPI2Cx: Frequency range */
  hfmpi2c->Instance->TIMINGR = hfmpi2c->Init.Timing & TIMING_CLEAR_MASK;

  /*---------------------------- FMPI2Cx OAR1 Configuration ---------------------*/
  /* Configure FMPI2Cx: Own Address1 and ack own address1 mode */
  hfmpi2c->Instance->OAR1 &= ~FMPI2C_OAR1_OA1EN;
  if(hfmpi2c->Init.OwnAddress1 != 0U)
  {
    if(hfmpi2c->Init.AddressingMode == FMPI2C_ADDRESSINGMODE_7BIT)
    {
      hfmpi2c->Instance->OAR1 = (FMPI2C_OAR1_OA1EN | hfmpi2c->Init.OwnAddress1);
    }
    else /* FMPI2C_ADDRESSINGMODE_10BIT */
    {
      hfmpi2c->Instance->OAR1 = (FMPI2C_OAR1_OA1EN | FMPI2C_OAR1_OA1MODE | hfmpi2c->Init.OwnAddress1);
    }
  }

  /*---------------------------- FMPI2Cx CR2 Configuration ----------------------*/
  /* Configure FMPI2Cx: Addressing Master mode */
  if(hfmpi2c->Init.AddressingMode == FMPI2C_ADDRESSINGMODE_10BIT)
  {
    hfmpi2c->Instance->CR2 = (FMPI2C_CR2_ADD10);
  }
  /* Enable the AUTOEND by default, and enable NACK (should be disable only during Slave process */
  hfmpi2c->Instance->CR2 |= (FMPI2C_CR2_AUTOEND | FMPI2C_CR2_NACK);

  /*---------------------------- FMPI2Cx OAR2 Configuration ---------------------*/
  /* Configure FMPI2Cx: Dual mode and Own Address2 */
  hfmpi2c->Instance->OAR2 = (hfmpi2c->Init.DualAddressMode | hfmpi2c->Init.OwnAddress2 | (hfmpi2c->Init.OwnAddress2Masks << 8U));

  /*---------------------------- FMPI2Cx CR1 Configuration ----------------------*/
  /* Configure FMPI2Cx: Generalcall and NoStretch mode */
  hfmpi2c->Instance->CR1 = (hfmpi2c->Init.GeneralCallMode | hfmpi2c->Init.NoStretchMode);

  /* Enable the selected FMPI2C peripheral */
  __HAL_FMPI2C_ENABLE(hfmpi2c);

  hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_NONE;
  hfmpi2c->State = HAL_FMPI2C_STATE_READY;
  hfmpi2c->PreviousState = FMPI2C_STATE_NONE;
  hfmpi2c->Mode = HAL_FMPI2C_MODE_NONE;
  
  return HAL_OK;
}

/**
  * @brief  DeInitialize the FMPI2C peripheral. 
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_DeInit(FMPI2C_HandleTypeDef *hfmpi2c)
{
  /* Check the FMPI2C handle allocation */
  if(hfmpi2c == NULL)
  {
    return HAL_ERROR;
  }

  /* Check the parameters */
  assert_param(IS_FMPI2C_ALL_INSTANCE(hfmpi2c->Instance));

  hfmpi2c->State = HAL_FMPI2C_STATE_BUSY;

  /* Disable the FMPI2C Peripheral Clock */
  __HAL_FMPI2C_DISABLE(hfmpi2c);

  /* DeInit the low level hardware: GPIO, CLOCK, NVIC */
  HAL_FMPI2C_MspDeInit(hfmpi2c);

  hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_NONE;
  hfmpi2c->State = HAL_FMPI2C_STATE_RESET;
  hfmpi2c->PreviousState = FMPI2C_STATE_NONE;
  hfmpi2c->Mode = HAL_FMPI2C_MODE_NONE;

  /* Release Lock */
  __HAL_UNLOCK(hfmpi2c);

  return HAL_OK;
}

/**
  * @brief Initialize the FMPI2C MSP.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @retval None
  */
 __weak void HAL_FMPI2C_MspInit(FMPI2C_HandleTypeDef *hfmpi2c)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hfmpi2c);
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_FMPI2C_MspInit could be implemented in the user file
   */ 
}

/**
  * @brief DeInitialize the FMPI2C MSP.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @retval None
  */
 __weak void HAL_FMPI2C_MspDeInit(FMPI2C_HandleTypeDef *hfmpi2c)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hfmpi2c);
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_FMPI2C_MspDeInit could be implemented in the user file
   */ 
}

/**
  * @}
  */

/** @defgroup FMPI2C_Exported_Functions_Group2 Input and Output operation functions 
  *  @brief   Data transfers functions 
  *
@verbatim   
 ===============================================================================
                      ##### IO operation functions #####
 ===============================================================================  
    [..]
    This subsection provides a set of functions allowing to manage the FMPI2C data 
    transfers.

    (#) There are two modes of transfer:
       (++) Blocking mode : The communication is performed in the polling mode. 
            The status of all data processing is returned by the same function 
            after finishing transfer.  
       (++) No Blocking mode : The communication is performed using Interrupts 
            or DMA. These functions return the status of the transfer startup.
            The end of the data processing will be indicated through the 
            dedicated FMPI2C IRQ when using Interrupt mode or the DMA IRQ when 
            using DMA mode.

    (#) Blocking mode functions are :
        (++) HAL_FMPI2C_Master_Transmit()
        (++) HAL_FMPI2C_Master_Receive()
        (++) HAL_FMPI2C_Slave_Transmit()
        (++) HAL_FMPI2C_Slave_Receive()
        (++) HAL_FMPI2C_Mem_Write()
        (++) HAL_FMPI2C_Mem_Read()
        (++) HAL_FMPI2C_IsDeviceReady()
        
    (#) No Blocking mode functions with Interrupt are :
        (++) HAL_FMPI2C_Master_Transmit_IT()
        (++) HAL_FMPI2C_Master_Receive_IT()
        (++) HAL_FMPI2C_Slave_Transmit_IT()
        (++) HAL_FMPI2C_Slave_Receive_IT()
        (++) HAL_FMPI2C_Mem_Write_IT()
        (++) HAL_FMPI2C_Mem_Read_IT()

    (#) No Blocking mode functions with DMA are :
        (++) HAL_FMPI2C_Master_Transmit_DMA()
        (++) HAL_FMPI2C_Master_Receive_DMA()
        (++) HAL_FMPI2C_Slave_Transmit_DMA()
        (++) HAL_FMPI2C_Slave_Receive_DMA()
        (++) HAL_FMPI2C_Mem_Write_DMA()
        (++) HAL_FMPI2C_Mem_Read_DMA()

    (#) A set of Transfer Complete Callbacks are provided in no Blocking mode:
        (++) HAL_FMPI2C_MemTxCpltCallback()
        (++) HAL_FMPI2C_MemRxCpltCallback()
        (++) HAL_FMPI2C_MasterTxCpltCallback()
        (++) HAL_FMPI2C_MasterRxCpltCallback()
        (++) HAL_FMPI2C_SlaveTxCpltCallback()
        (++) HAL_FMPI2C_SlaveRxCpltCallback()
        (++) HAL_FMPI2C_ErrorCallback()

@endverbatim
  * @{
  */

/**
  * @brief  Transmits in master mode an amount of data in blocking mode.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  DevAddress Target device address
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Master_Transmit(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
  uint32_t sizetmp = 0U;

  if(hfmpi2c->State == HAL_FMPI2C_STATE_READY)
  {
    if((pData == NULL ) || (Size == 0U))
    {
      return  HAL_ERROR;
    }
    
    if(__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_BUSY) == SET)
    {
      return HAL_BUSY;
    }

    /* Process Locked */
    __HAL_LOCK(hfmpi2c);

    hfmpi2c->State = HAL_FMPI2C_STATE_BUSY_TX;
    hfmpi2c->Mode = HAL_FMPI2C_MODE_MASTER;
    hfmpi2c->ErrorCode   = HAL_FMPI2C_ERROR_NONE;

    /* Send Slave Address */
    /* Set NBYTES to write and reload if size > MAX_NBYTE_SIZE and generate RESTART */
    /* Size > MAX_NBYTE_SIZE, need to set RELOAD bit */
    if(Size > MAX_NBYTE_SIZE)
    {
      FMPI2C_TransferConfig(hfmpi2c,DevAddress,MAX_NBYTE_SIZE, FMPI2C_RELOAD_MODE, FMPI2C_GENERATE_START_WRITE);
      sizetmp = MAX_NBYTE_SIZE;
    }
    else
    {
      FMPI2C_TransferConfig(hfmpi2c,DevAddress,Size, FMPI2C_AUTOEND_MODE, FMPI2C_GENERATE_START_WRITE);
      sizetmp = Size;
    }

    do
    {
      /* Wait until TXIS flag is set */
      if(FMPI2C_WaitOnTXISFlagUntilTimeout(hfmpi2c, Timeout) != HAL_OK)
      {
        if(hfmpi2c->ErrorCode == HAL_FMPI2C_ERROR_AF)
        {
          return HAL_ERROR;
        }
        else
        {
          return HAL_TIMEOUT;
        }
      }
      /* Write data to TXDR */
      hfmpi2c->Instance->TXDR = (*pData++);
      sizetmp--;
      Size--;

      if((sizetmp == 0U)&&(Size!=0U))
      {
        /* Wait until TXE flag is set */
        if(FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_TCR, RESET, Timeout) != HAL_OK)
        {
          return HAL_TIMEOUT;
        }

        if(Size > MAX_NBYTE_SIZE)
        {
          FMPI2C_TransferConfig(hfmpi2c,DevAddress,MAX_NBYTE_SIZE, FMPI2C_RELOAD_MODE, FMPI2C_NO_STARTSTOP);
          sizetmp = MAX_NBYTE_SIZE;
        }
        else
        {
          FMPI2C_TransferConfig(hfmpi2c,DevAddress,Size, FMPI2C_AUTOEND_MODE, FMPI2C_NO_STARTSTOP);
          sizetmp = Size;
        }
      }

    }while(Size > 0U);

    /* No need to Check TC flag, with AUTOEND mode the stop is automatically generated */
    /* Wait until STOPF flag is set */
    if(FMPI2C_WaitOnSTOPFlagUntilTimeout(hfmpi2c, Timeout) != HAL_OK)
    {
      if(hfmpi2c->ErrorCode == HAL_FMPI2C_ERROR_AF)
      {
        return HAL_ERROR;
      }
      else
      {
        return HAL_TIMEOUT;
      }
    }

    /* Clear STOP Flag */
    __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_STOPF);

    /* Clear Configuration Register 2 */
    FMPI2C_RESET_CR2(hfmpi2c);

    hfmpi2c->State = HAL_FMPI2C_STATE_READY;
    hfmpi2c->Mode = HAL_FMPI2C_MODE_NONE;

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c);

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  Receives in master mode an amount of data in blocking mode. 
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  DevAddress Target device address
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Master_Receive(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
  uint32_t sizetmp = 0U;

  if(hfmpi2c->State == HAL_FMPI2C_STATE_READY)
  {
    if((pData == NULL ) || (Size == 0U))
    {
      return  HAL_ERROR;
    }
    
    if(__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_BUSY) == SET)
    {
      return HAL_BUSY;
    }

    /* Process Locked */
    __HAL_LOCK(hfmpi2c);

    hfmpi2c->State = HAL_FMPI2C_STATE_BUSY_RX;
    hfmpi2c->Mode = HAL_FMPI2C_MODE_MASTER;
    hfmpi2c->ErrorCode   = HAL_FMPI2C_ERROR_NONE;

    /* Send Slave Address */
    /* Set NBYTES to write and reload if size > MAX_NBYTE_SIZE and generate RESTART */
    /* Size > MAX_NBYTE_SIZE, need to set RELOAD bit */
    if(Size > MAX_NBYTE_SIZE)
    {
      FMPI2C_TransferConfig(hfmpi2c,DevAddress,MAX_NBYTE_SIZE, FMPI2C_RELOAD_MODE, FMPI2C_GENERATE_START_READ);
      sizetmp = MAX_NBYTE_SIZE;
    }
    else
    {
      FMPI2C_TransferConfig(hfmpi2c,DevAddress,Size, FMPI2C_AUTOEND_MODE, FMPI2C_GENERATE_START_READ);
      sizetmp = Size;
    }

    do
    {
      /* Wait until RXNE flag is set */
      if(FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_RXNE, RESET, Timeout) != HAL_OK)      
      {
        return HAL_TIMEOUT;
      }

      /* Write data to RXDR */
      (*pData++) =hfmpi2c->Instance->RXDR;
      sizetmp--;
      Size--;

      if((sizetmp == 0U)&&(Size!=0U))
      {
        /* Wait until TCR flag is set */
        if(FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_TCR, RESET, Timeout) != HAL_OK)      
        {
          return HAL_TIMEOUT;
        }
        
        if(Size > MAX_NBYTE_SIZE)
        {
          FMPI2C_TransferConfig(hfmpi2c,DevAddress,MAX_NBYTE_SIZE, FMPI2C_RELOAD_MODE, FMPI2C_NO_STARTSTOP);
          sizetmp = MAX_NBYTE_SIZE;
        }
        else
        {
          FMPI2C_TransferConfig(hfmpi2c,DevAddress,Size, FMPI2C_AUTOEND_MODE, FMPI2C_NO_STARTSTOP);
          sizetmp = Size;
        }
      }

    }while(Size > 0U);

    /* No need to Check TC flag, with AUTOEND mode the stop is automatically generated */
    /* Wait until STOPF flag is set */
    if(FMPI2C_WaitOnSTOPFlagUntilTimeout(hfmpi2c, FMPI2C_TIMEOUT_STOPF) != HAL_OK)
    {
      if(hfmpi2c->ErrorCode == HAL_FMPI2C_ERROR_AF)
      {
        return HAL_ERROR;
      }
      else
      {
        return HAL_TIMEOUT;
      }
    }

    /* Clear STOP Flag */
    __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_STOPF);

    /* Clear Configuration Register 2 */
    FMPI2C_RESET_CR2(hfmpi2c);
    
    hfmpi2c->State = HAL_FMPI2C_STATE_READY;
    hfmpi2c->Mode = HAL_FMPI2C_MODE_NONE;	
    
    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c);

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY; 
  }
}

/**
  * @brief  Transmits in slave mode an amount of data in blocking mode. 
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Slave_Transmit(FMPI2C_HandleTypeDef *hfmpi2c, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
  if(hfmpi2c->State == HAL_FMPI2C_STATE_READY)
  {
    if((pData == NULL ) || (Size == 0U))
    {
      return  HAL_ERROR;
    }

    /* Process Locked */
    __HAL_LOCK(hfmpi2c);

    hfmpi2c->State = HAL_FMPI2C_STATE_BUSY_TX;
    hfmpi2c->Mode = HAL_FMPI2C_MODE_SLAVE;
    hfmpi2c->ErrorCode   = HAL_FMPI2C_ERROR_NONE;

    /* Enable Address Acknowledge */
    hfmpi2c->Instance->CR2 &= ~FMPI2C_CR2_NACK;

    /* Wait until ADDR flag is set */
    if(FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_ADDR, RESET, Timeout) != HAL_OK)
    {
      /* Disable Address Acknowledge */
      hfmpi2c->Instance->CR2 |= FMPI2C_CR2_NACK;
      return HAL_TIMEOUT;
    }

    /* Clear ADDR flag */
    __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c,FMPI2C_FLAG_ADDR);

    /* If 10bit addressing mode is selected */
    if(hfmpi2c->Init.AddressingMode == FMPI2C_ADDRESSINGMODE_10BIT)
    {
      /* Wait until ADDR flag is set */
      if(FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_ADDR, RESET, Timeout) != HAL_OK)
      {
        /* Disable Address Acknowledge */
        hfmpi2c->Instance->CR2 |= FMPI2C_CR2_NACK;
        return HAL_TIMEOUT;
      }

      /* Clear ADDR flag */
      __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c,FMPI2C_FLAG_ADDR);
    }

    /* Wait until DIR flag is set Transmitter mode */
    if(FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_DIR, RESET, Timeout) != HAL_OK)
    {
      /* Disable Address Acknowledge */
      hfmpi2c->Instance->CR2 |= FMPI2C_CR2_NACK;
      return HAL_TIMEOUT;
    }

    do
    {
      /* Wait until TXIS flag is set */
      if(FMPI2C_WaitOnTXISFlagUntilTimeout(hfmpi2c, Timeout) != HAL_OK)
      {
        /* Disable Address Acknowledge */
        hfmpi2c->Instance->CR2 |= FMPI2C_CR2_NACK;

        if(hfmpi2c->ErrorCode == HAL_FMPI2C_ERROR_AF)
        {
          return HAL_ERROR;
        }
        else
        {
          return HAL_TIMEOUT;
        }
      }

      /* Write data to TXDR */
      hfmpi2c->Instance->TXDR = (*pData++);
      Size--;
    }while(Size > 0U);

    /* Wait until STOP flag is set */
    if(FMPI2C_WaitOnSTOPFlagUntilTimeout(hfmpi2c, FMPI2C_TIMEOUT_STOPF) != HAL_OK)
    {
      /* Disable Address Acknowledge */
      hfmpi2c->Instance->CR2 |= FMPI2C_CR2_NACK;

      if(hfmpi2c->ErrorCode == HAL_FMPI2C_ERROR_AF)
      {
        /* Normal use case for Transmitter mode */
        /* A NACK is generated to confirm the end of transfer */
        hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_NONE;
      }
      else
      {
        return HAL_TIMEOUT;
      }
    }

    /* Clear STOP flag */
    __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c,FMPI2C_FLAG_STOPF);

    /* Wait until BUSY flag is reset */ 
    if(FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_BUSY, SET, Timeout) != HAL_OK)
    {
      /* Disable Address Acknowledge */
      hfmpi2c->Instance->CR2 |= FMPI2C_CR2_NACK;
      return HAL_TIMEOUT;
    }

    /* Disable Address Acknowledge */
    hfmpi2c->Instance->CR2 |= FMPI2C_CR2_NACK;

    hfmpi2c->State = HAL_FMPI2C_STATE_READY;
    hfmpi2c->Mode = HAL_FMPI2C_MODE_NONE;

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c);
    
    return HAL_OK;
  }
  else
  {
    return HAL_BUSY; 
  }
}

/**
  * @brief  Receive in slave mode an amount of data in blocking mode 
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Slave_Receive(FMPI2C_HandleTypeDef *hfmpi2c, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
  if(hfmpi2c->State == HAL_FMPI2C_STATE_READY)
  {
    if((pData == NULL ) || (Size == 0U)) 
    {
      return  HAL_ERROR;
    }

    /* Process Locked */
    __HAL_LOCK(hfmpi2c);

    hfmpi2c->State = HAL_FMPI2C_STATE_BUSY_RX;
    hfmpi2c->Mode = HAL_FMPI2C_MODE_SLAVE;
    hfmpi2c->ErrorCode   = HAL_FMPI2C_ERROR_NONE;

    /* Enable Address Acknowledge */
    hfmpi2c->Instance->CR2 &= ~FMPI2C_CR2_NACK;

    /* Wait until ADDR flag is set */
    if(FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_ADDR, RESET, Timeout) != HAL_OK)
    {
      /* Disable Address Acknowledge */
      hfmpi2c->Instance->CR2 |= FMPI2C_CR2_NACK;
      return HAL_TIMEOUT;
    }

    /* Clear ADDR flag */
    __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c,FMPI2C_FLAG_ADDR);
    
    /* Wait until DIR flag is reset Receiver mode */
    if(FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_DIR, SET, Timeout) != HAL_OK)
    {
      /* Disable Address Acknowledge */
      hfmpi2c->Instance->CR2 |= FMPI2C_CR2_NACK;
      return HAL_TIMEOUT;
    }

    while(Size > 0U)
    {
      /* Wait until RXNE flag is set */
      if(FMPI2C_WaitOnRXNEFlagUntilTimeout(hfmpi2c, Timeout) != HAL_OK)
      {
        /* Disable Address Acknowledge */
        hfmpi2c->Instance->CR2 |= FMPI2C_CR2_NACK;
        if(hfmpi2c->ErrorCode == HAL_FMPI2C_ERROR_TIMEOUT)
        {
          return HAL_TIMEOUT;
        }
        else
        {
          return HAL_ERROR;
        }
      }

      /* Read data from RXDR */
      (*pData++) = hfmpi2c->Instance->RXDR;
      Size--;
    }

    /* Wait until STOP flag is set */
    if(FMPI2C_WaitOnSTOPFlagUntilTimeout(hfmpi2c, FMPI2C_TIMEOUT_STOPF) != HAL_OK)
    {
      /* Disable Address Acknowledge */
      hfmpi2c->Instance->CR2 |= FMPI2C_CR2_NACK;

      if(hfmpi2c->ErrorCode == HAL_FMPI2C_ERROR_AF)
      {
        return HAL_ERROR;
      }
      else
      {
        return HAL_TIMEOUT;
      }
    }

    /* Clear STOP flag */
    __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c,FMPI2C_FLAG_STOPF);
    
    /* Wait until BUSY flag is reset */ 
    if(FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_BUSY, SET, Timeout) != HAL_OK)
    {
      /* Disable Address Acknowledge */
      hfmpi2c->Instance->CR2 |= FMPI2C_CR2_NACK;
      return HAL_TIMEOUT;
    }

    
    /* Disable Address Acknowledge */
    hfmpi2c->Instance->CR2 |= FMPI2C_CR2_NACK;
    
    hfmpi2c->State = HAL_FMPI2C_STATE_READY;
    hfmpi2c->Mode = HAL_FMPI2C_MODE_NONE;

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c);

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY; 
  }
}

/**
  * @brief  Transmit in master mode an amount of data in no blocking mode with Interrupt
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  DevAddress Target device address
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Master_Transmit_IT(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size)
{
  if(hfmpi2c->State == HAL_FMPI2C_STATE_READY)
  {
    if((pData == NULL) || (Size == 0U)) 
    {
      return  HAL_ERROR;
    }

    if(__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_BUSY) == SET)
    {
      return HAL_BUSY;
    }

    /* Process Locked */
    __HAL_LOCK(hfmpi2c);

    hfmpi2c->State = HAL_FMPI2C_STATE_BUSY_TX;
    hfmpi2c->Mode = HAL_FMPI2C_MODE_MASTER;
    hfmpi2c->ErrorCode   = HAL_FMPI2C_ERROR_NONE;
    hfmpi2c->XferOptions = FMPI2C_NO_OPTION_FRAME;

    hfmpi2c->pBuffPtr = pData;
    hfmpi2c->XferCount = Size;
    if(Size > MAX_NBYTE_SIZE)
    {
      hfmpi2c->XferSize = MAX_NBYTE_SIZE;
    }
    else
    {
      hfmpi2c->XferSize = Size;
    }

    /* Send Slave Address */
    /* Set NBYTES to write and reload if size > MAX_NBYTE_SIZE and generate RESTART */
    if( (hfmpi2c->XferSize == MAX_NBYTE_SIZE) && (hfmpi2c->XferSize < hfmpi2c->XferCount) )
    {
      FMPI2C_TransferConfig(hfmpi2c,DevAddress,hfmpi2c->XferSize, FMPI2C_RELOAD_MODE, FMPI2C_GENERATE_START_WRITE);
    }
    else
    {
      FMPI2C_TransferConfig(hfmpi2c,DevAddress,hfmpi2c->XferSize, FMPI2C_AUTOEND_MODE, FMPI2C_GENERATE_START_WRITE);
    }

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c); 

    /* Note : The FMPI2C interrupts must be enabled after unlocking current process 
              to avoid the risk of FMPI2C interrupt handle execution before current
              process unlock */


    /* Enable ERR, TC, STOP, NACK, TXI interrupt */
    /* possible to enable all of these */
    /* FMPI2C_IT_ERRI | FMPI2C_IT_TCI| FMPI2C_IT_STOPI| FMPI2C_IT_NACKI | FMPI2C_IT_ADDRI | FMPI2C_IT_RXI | FMPI2C_IT_TXI */
    __HAL_FMPI2C_ENABLE_IT(hfmpi2c,FMPI2C_IT_ERRI | FMPI2C_IT_TCI| FMPI2C_IT_STOPI| FMPI2C_IT_NACKI | FMPI2C_IT_TXI );

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  Receive in master mode an amount of data in no blocking mode with Interrupt
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  DevAddress Target device address
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Master_Receive_IT(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size)
{
  if(hfmpi2c->State == HAL_FMPI2C_STATE_READY)
  {
    if((pData == NULL) || (Size == 0U)) 
    {
      return  HAL_ERROR;
    }
    
    if(__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_BUSY) == SET)
    {
      return HAL_BUSY;
    }

    /* Process Locked */
    __HAL_LOCK(hfmpi2c);

    hfmpi2c->State = HAL_FMPI2C_STATE_BUSY_RX;
    hfmpi2c->Mode = HAL_FMPI2C_MODE_MASTER;
    hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_NONE;
    hfmpi2c->XferOptions = FMPI2C_NO_OPTION_FRAME;

    hfmpi2c->pBuffPtr = pData;
    hfmpi2c->XferCount = Size;
    if(Size > MAX_NBYTE_SIZE)
    {
      hfmpi2c->XferSize = MAX_NBYTE_SIZE;
    }
    else
    {
      hfmpi2c->XferSize = Size;
    }

    /* Send Slave Address */
    /* Set NBYTES to write and reload if size > MAX_NBYTE_SIZE and generate RESTART */
    if( (hfmpi2c->XferSize == MAX_NBYTE_SIZE) && (hfmpi2c->XferSize < hfmpi2c->XferCount))
    {
      FMPI2C_TransferConfig(hfmpi2c,DevAddress,hfmpi2c->XferSize, FMPI2C_RELOAD_MODE, FMPI2C_GENERATE_START_READ);
    }
    else
    {
      FMPI2C_TransferConfig(hfmpi2c,DevAddress,hfmpi2c->XferSize, FMPI2C_AUTOEND_MODE, FMPI2C_GENERATE_START_READ);
    }
    
    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c); 

    /* Note : The FMPI2C interrupts must be enabled after unlocking current process 
              to avoid the risk of FMPI2C interrupt handle execution before current
              process unlock */
    
    /* Enable ERR, TC, STOP, NACK, RXI interrupt */
    /* possible to enable all of these */
    /* FMPI2C_IT_ERRI | FMPI2C_IT_TCI| FMPI2C_IT_STOPI| FMPI2C_IT_NACKI | FMPI2C_IT_ADDRI | FMPI2C_IT_RXI | FMPI2C_IT_TXI */
    __HAL_FMPI2C_ENABLE_IT(hfmpi2c,FMPI2C_IT_ERRI | FMPI2C_IT_TCI | FMPI2C_IT_STOPI | FMPI2C_IT_NACKI | FMPI2C_IT_RXI );

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY; 
  }
}

/**
  * @brief  Transmit in slave mode an amount of data in no blocking mode with Interrupt 
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Slave_Transmit_IT(FMPI2C_HandleTypeDef *hfmpi2c, uint8_t *pData, uint16_t Size)
{
  if(hfmpi2c->State == HAL_FMPI2C_STATE_READY)
  {
    if((pData == NULL) || (Size == 0U))
    {
      return  HAL_ERROR;
    }
    
    /* Process Locked */
    __HAL_LOCK(hfmpi2c);

    hfmpi2c->State = HAL_FMPI2C_STATE_BUSY_TX;
    hfmpi2c->Mode = HAL_FMPI2C_MODE_SLAVE;
    hfmpi2c->ErrorCode   = HAL_FMPI2C_ERROR_NONE;
    hfmpi2c->XferOptions = FMPI2C_NO_OPTION_FRAME;

    /* Enable Address Acknowledge */
    hfmpi2c->Instance->CR2 &= ~FMPI2C_CR2_NACK;

    hfmpi2c->pBuffPtr = pData;
    hfmpi2c->XferSize = Size;
    hfmpi2c->XferCount = Size;
    
    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c); 

    /* Note : The FMPI2C interrupts must be enabled after unlocking current process 
              to avoid the risk of FMPI2C interrupt handle execution before current
              process unlock */

    /* Enable ERR, TC, STOP, NACK, TXI interrupt */
    /* possible to enable all of these */
    /* FMPI2C_IT_ERRI | FMPI2C_IT_TCI| FMPI2C_IT_STOPI| FMPI2C_IT_NACKI | FMPI2C_IT_ADDRI | FMPI2C_IT_RXI | FMPI2C_IT_TXI */
    __HAL_FMPI2C_ENABLE_IT(hfmpi2c,FMPI2C_IT_ERRI | FMPI2C_IT_TCI| FMPI2C_IT_STOPI | FMPI2C_IT_NACKI | FMPI2C_IT_ADDRI | FMPI2C_IT_TXI );

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  Receive in slave mode an amount of data in no blocking mode with Interrupt 
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Slave_Receive_IT(FMPI2C_HandleTypeDef *hfmpi2c, uint8_t *pData, uint16_t Size)
{
  if(hfmpi2c->State == HAL_FMPI2C_STATE_READY)
  {
    if((pData == NULL) || (Size == 0U))
    {
      return  HAL_ERROR;
    }

    /* Process Locked */
    __HAL_LOCK(hfmpi2c);

    hfmpi2c->State = HAL_FMPI2C_STATE_BUSY_RX;
    hfmpi2c->Mode = HAL_FMPI2C_MODE_SLAVE;
    hfmpi2c->ErrorCode   = HAL_FMPI2C_ERROR_NONE;
    hfmpi2c->XferOptions = FMPI2C_NO_OPTION_FRAME;

    /* Enable Address Acknowledge */
    hfmpi2c->Instance->CR2 &= ~FMPI2C_CR2_NACK;

    hfmpi2c->pBuffPtr = pData;
    hfmpi2c->XferSize = Size;
    hfmpi2c->XferCount = Size;
    
    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c); 

    /* Note : The FMPI2C interrupts must be enabled after unlocking current process 
              to avoid the risk of FMPI2C interrupt handle execution before current
              process unlock */

    /* Enable ERR, TC, STOP, NACK, RXI interrupt */
    /* possible to enable all of these */
    /* FMPI2C_IT_ERRI | FMPI2C_IT_TCI| FMPI2C_IT_STOPI| FMPI2C_IT_NACKI | FMPI2C_IT_ADDRI | FMPI2C_IT_RXI | FMPI2C_IT_TXI */
    __HAL_FMPI2C_ENABLE_IT(hfmpi2c,FMPI2C_IT_ERRI | FMPI2C_IT_TCI | FMPI2C_IT_STOPI | FMPI2C_IT_NACKI | FMPI2C_IT_ADDRI | FMPI2C_IT_RXI);

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY; 
  }
}

/**
  * @brief  Transmit in master mode an amount of data in no blocking mode with DMA
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  DevAddress Target device address
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Master_Transmit_DMA(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size)
{
  if(hfmpi2c->State == HAL_FMPI2C_STATE_READY)
  {
    if((pData == NULL) || (Size == 0U))
    {
      return  HAL_ERROR;
    }

    if(__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_BUSY) == SET)
    {
      return HAL_BUSY;
    }

    /* Process Locked */
    __HAL_LOCK(hfmpi2c);

    hfmpi2c->State = HAL_FMPI2C_STATE_BUSY_TX;
    hfmpi2c->Mode = HAL_FMPI2C_MODE_MASTER;
    hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_NONE;

    hfmpi2c->pBuffPtr = pData;
    hfmpi2c->XferCount = Size;
    if(Size > MAX_NBYTE_SIZE)
    {
      hfmpi2c->XferSize = MAX_NBYTE_SIZE;
    }
    else
    {
      hfmpi2c->XferSize = Size;
    }

    /* Set the FMPI2C DMA transfer complete callback */
    hfmpi2c->hdmatx->XferCpltCallback = FMPI2C_DMAMasterTransmitCplt;
    
    /* Set the DMA error callback */
    hfmpi2c->hdmatx->XferErrorCallback = FMPI2C_DMAError;
    
    /* Enable the DMA channel */
    HAL_DMA_Start_IT(hfmpi2c->hdmatx, (uint32_t)pData, (uint32_t)&hfmpi2c->Instance->TXDR, hfmpi2c->XferSize);

    /* Send Slave Address */
    /* Set NBYTES to write and reload if size > MAX_NBYTE_SIZE and generate RESTART */
    if( (hfmpi2c->XferSize == MAX_NBYTE_SIZE) && (hfmpi2c->XferSize < hfmpi2c->XferCount))
    {
      FMPI2C_TransferConfig(hfmpi2c,DevAddress,hfmpi2c->XferSize, FMPI2C_RELOAD_MODE, FMPI2C_GENERATE_START_WRITE);
    }
    else
    {
      FMPI2C_TransferConfig(hfmpi2c,DevAddress,hfmpi2c->XferSize, FMPI2C_AUTOEND_MODE, FMPI2C_GENERATE_START_WRITE);
    }

    /* Wait until TXIS flag is set */
    if(FMPI2C_WaitOnTXISFlagUntilTimeout(hfmpi2c, FMPI2C_TIMEOUT_TXIS) != HAL_OK)
    {
      /* Disable Address Acknowledge */
      hfmpi2c->Instance->CR2 |= FMPI2C_CR2_NACK;

      if(hfmpi2c->ErrorCode == HAL_FMPI2C_ERROR_AF)
      {
        return HAL_ERROR;
      }
      else
      {
        return HAL_TIMEOUT;
      }
    }

    /* Enable DMA Request */
    hfmpi2c->Instance->CR1 |= FMPI2C_CR1_TXDMAEN;

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c);

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  Receive in master mode an amount of data in no blocking mode with DMA 
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  DevAddress Target device address
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Master_Receive_DMA(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size)
{
  if(hfmpi2c->State == HAL_FMPI2C_STATE_READY)
  {
    if((pData == NULL) || (Size == 0U))
    {
      return  HAL_ERROR;
    }

    if(__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_BUSY) == SET)
    {
      return HAL_BUSY;
    }

    /* Process Locked */
    __HAL_LOCK(hfmpi2c);

    hfmpi2c->State = HAL_FMPI2C_STATE_BUSY_RX;
    hfmpi2c->Mode = HAL_FMPI2C_MODE_MASTER;
    hfmpi2c->ErrorCode   = HAL_FMPI2C_ERROR_NONE;

    hfmpi2c->pBuffPtr = pData;
    hfmpi2c->XferCount = Size;
    if(Size > MAX_NBYTE_SIZE)
    {
      hfmpi2c->XferSize = MAX_NBYTE_SIZE;
    }
    else
    {
      hfmpi2c->XferSize = Size;
    }

    /* Set the FMPI2C DMA transfer complete callback */
    hfmpi2c->hdmarx->XferCpltCallback = FMPI2C_DMAMasterReceiveCplt;

    /* Set the DMA error callback */
    hfmpi2c->hdmarx->XferErrorCallback = FMPI2C_DMAError;

    /* Enable the DMA channel */
    HAL_DMA_Start_IT(hfmpi2c->hdmarx, (uint32_t)&hfmpi2c->Instance->RXDR, (uint32_t)pData, hfmpi2c->XferSize);

    /* Send Slave Address */
    /* Set NBYTES to write and reload if size > MAX_NBYTE_SIZE and generate RESTART */
    if( (hfmpi2c->XferSize == MAX_NBYTE_SIZE) && (hfmpi2c->XferSize < hfmpi2c->XferCount))
    {
      FMPI2C_TransferConfig(hfmpi2c,DevAddress,hfmpi2c->XferSize, FMPI2C_RELOAD_MODE, FMPI2C_GENERATE_START_READ);
    }
    else
    {
      FMPI2C_TransferConfig(hfmpi2c,DevAddress,hfmpi2c->XferSize, FMPI2C_AUTOEND_MODE, FMPI2C_GENERATE_START_READ);
    }

    /* Wait until RXNE flag is set */
    if(FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_RXNE, RESET, FMPI2C_TIMEOUT_RXNE) != HAL_OK)
    {
      return HAL_TIMEOUT;
    }

    
    /* Enable DMA Request */
    hfmpi2c->Instance->CR1 |= FMPI2C_CR1_RXDMAEN;

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c);

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  Transmit in slave mode an amount of data in no blocking mode with DMA 
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Slave_Transmit_DMA(FMPI2C_HandleTypeDef *hfmpi2c, uint8_t *pData, uint16_t Size)
{
  if(hfmpi2c->State == HAL_FMPI2C_STATE_READY)
  {
    if((pData == NULL) || (Size == 0U))
    {
      return  HAL_ERROR;
    }
    /* Process Locked */
    __HAL_LOCK(hfmpi2c); 

    hfmpi2c->State = HAL_FMPI2C_STATE_BUSY_TX;
    hfmpi2c->Mode = HAL_FMPI2C_MODE_SLAVE;
    hfmpi2c->ErrorCode   = HAL_FMPI2C_ERROR_NONE;

    hfmpi2c->pBuffPtr = pData;
    hfmpi2c->XferCount = Size;
    hfmpi2c->XferSize = Size;

    /* Set the FMPI2C DMA transfer complete callback */
    hfmpi2c->hdmatx->XferCpltCallback = FMPI2C_DMASlaveTransmitCplt;

    /* Set the DMA error callback */
    hfmpi2c->hdmatx->XferErrorCallback = FMPI2C_DMAError;

    /* Enable the DMA channel */
    HAL_DMA_Start_IT(hfmpi2c->hdmatx, (uint32_t)pData, (uint32_t)&hfmpi2c->Instance->TXDR, hfmpi2c->XferSize);

    /* Enable Address Acknowledge */
    hfmpi2c->Instance->CR2 &= ~FMPI2C_CR2_NACK;

    /* Wait until ADDR flag is set */
    if(FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_ADDR, RESET, FMPI2C_TIMEOUT_ADDR) != HAL_OK)
    {
      /* Disable Address Acknowledge */
      hfmpi2c->Instance->CR2 |= FMPI2C_CR2_NACK;
      return HAL_TIMEOUT;
    }

    /* Clear ADDR flag */
    __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c,FMPI2C_FLAG_ADDR);

    /* If 10bits addressing mode is selected */
    if(hfmpi2c->Init.AddressingMode == FMPI2C_ADDRESSINGMODE_10BIT)
    {
      /* Wait until ADDR flag is set */
      if(FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_ADDR, RESET, FMPI2C_TIMEOUT_ADDR) != HAL_OK)
      {
        /* Disable Address Acknowledge */
        hfmpi2c->Instance->CR2 |= FMPI2C_CR2_NACK;
        return HAL_TIMEOUT;
      }

      /* Clear ADDR flag */
      __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c,FMPI2C_FLAG_ADDR);
    }

    /* Wait until DIR flag is set Transmitter mode */
    if(FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_DIR, RESET, FMPI2C_TIMEOUT_BUSY) != HAL_OK)
    {
      /* Disable Address Acknowledge */
      hfmpi2c->Instance->CR2 |= FMPI2C_CR2_NACK;
      return HAL_TIMEOUT;
    }

    /* Enable DMA Request */
    hfmpi2c->Instance->CR1 |= FMPI2C_CR1_TXDMAEN;

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c);

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  Receive in slave mode an amount of data in no blocking mode with DMA 
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Slave_Receive_DMA(FMPI2C_HandleTypeDef *hfmpi2c, uint8_t *pData, uint16_t Size)
{
  if(hfmpi2c->State == HAL_FMPI2C_STATE_READY)
  {
    if((pData == NULL) || (Size == 0U))
    {
      return  HAL_ERROR;
    }
    /* Process Locked */
    __HAL_LOCK(hfmpi2c);

    hfmpi2c->State = HAL_FMPI2C_STATE_BUSY_RX;
    hfmpi2c->Mode = HAL_FMPI2C_MODE_SLAVE;
    hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_NONE;

    hfmpi2c->pBuffPtr = pData;
    hfmpi2c->XferSize = Size;
    hfmpi2c->XferCount = Size;

    /* Set the FMPI2C DMA transfer complete callback */
    hfmpi2c->hdmarx->XferCpltCallback = FMPI2C_DMASlaveReceiveCplt;

    /* Set the DMA error callback */
    hfmpi2c->hdmarx->XferErrorCallback = FMPI2C_DMAError;

    /* Enable the DMA channel */
    HAL_DMA_Start_IT(hfmpi2c->hdmarx, (uint32_t)&hfmpi2c->Instance->RXDR, (uint32_t)pData, Size);

    /* Enable Address Acknowledge */
    hfmpi2c->Instance->CR2 &= ~FMPI2C_CR2_NACK;

    /* Wait until ADDR flag is set */
    if(FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_ADDR, RESET, FMPI2C_TIMEOUT_ADDR) != HAL_OK)
    {
      /* Disable Address Acknowledge */
      hfmpi2c->Instance->CR2 |= FMPI2C_CR2_NACK;
      return HAL_TIMEOUT;
    }

    /* Clear ADDR flag */
    __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c,FMPI2C_FLAG_ADDR);

    /* Wait until DIR flag is set Receiver mode */
    if(FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_DIR, SET, FMPI2C_TIMEOUT_DIR) != HAL_OK)
    {
      /* Disable Address Acknowledge */
      hfmpi2c->Instance->CR2 |= FMPI2C_CR2_NACK;
      return HAL_TIMEOUT;
    }

    /* Enable DMA Request */
    hfmpi2c->Instance->CR1 |= FMPI2C_CR1_RXDMAEN;

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c);

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  Write an amount of data in blocking mode to a specific memory address
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  DevAddress Target device address
  * @param  MemAddress Internal memory address
  * @param  MemAddSize Size of internal memory address
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Mem_Write(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
  uint32_t Sizetmp = 0U;

  /* Check the parameters */
  assert_param(IS_FMPI2C_MEMADD_SIZE(MemAddSize));
  
  if(hfmpi2c->State == HAL_FMPI2C_STATE_READY)
  { 
    if((pData == NULL) || (Size == 0U)) 
    {
      return  HAL_ERROR;
    }

    if(__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_BUSY) == SET)
    {
      return HAL_BUSY;
    }

    /* Process Locked */
    __HAL_LOCK(hfmpi2c);

    hfmpi2c->State = HAL_FMPI2C_STATE_BUSY_TX;
    hfmpi2c->Mode = HAL_FMPI2C_MODE_MEM;
    hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_NONE;

    /* Send Slave Address and Memory Address */
    if(FMPI2C_RequestMemoryWrite(hfmpi2c, DevAddress, MemAddress, MemAddSize, Timeout) != HAL_OK)
    {
      if(hfmpi2c->ErrorCode == HAL_FMPI2C_ERROR_AF)
      {
        /* Process Unlocked */
        __HAL_UNLOCK(hfmpi2c);
        return HAL_ERROR;
      }
      else
      {
        /* Process Unlocked */
        __HAL_UNLOCK(hfmpi2c);
        return HAL_TIMEOUT;
      }
    }

    /* Set NBYTES to write and reload if size > MAX_NBYTE_SIZE */
    /* Size > MAX_NBYTE_SIZE, need to set RELOAD bit */
    if(Size > MAX_NBYTE_SIZE)
    {
      FMPI2C_TransferConfig(hfmpi2c,DevAddress,MAX_NBYTE_SIZE, FMPI2C_RELOAD_MODE, FMPI2C_NO_STARTSTOP);
      Sizetmp = MAX_NBYTE_SIZE;
    }
    else
    {
      FMPI2C_TransferConfig(hfmpi2c,DevAddress,Size, FMPI2C_AUTOEND_MODE, FMPI2C_NO_STARTSTOP);
      Sizetmp = Size;
    }

    do
    {
      /* Wait until TXIS flag is set */
      if(FMPI2C_WaitOnTXISFlagUntilTimeout(hfmpi2c, Timeout) != HAL_OK)
      {
        if(hfmpi2c->ErrorCode == HAL_FMPI2C_ERROR_AF)
        {
          return HAL_ERROR;
        }
        else
        {
          return HAL_TIMEOUT;
        }
      }

      /* Write data to DR */
      hfmpi2c->Instance->TXDR = (*pData++);
      Sizetmp--;
      Size--;

      if((Sizetmp == 0U)&&(Size!=0U))
      {
        /* Wait until TCR flag is set */
        if(FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_TCR, RESET, Timeout) != HAL_OK)
        {
          return HAL_TIMEOUT;
        }
        
        if(Size > MAX_NBYTE_SIZE)
        {
          FMPI2C_TransferConfig(hfmpi2c,DevAddress,MAX_NBYTE_SIZE, FMPI2C_RELOAD_MODE, FMPI2C_NO_STARTSTOP);
          Sizetmp = MAX_NBYTE_SIZE;
        }
        else
        {
          FMPI2C_TransferConfig(hfmpi2c,DevAddress,Size, FMPI2C_AUTOEND_MODE, FMPI2C_NO_STARTSTOP);
          Sizetmp = Size;
        }
      }
      
    }while(Size > 0U);

    /* No need to Check TC flag, with AUTOEND mode the stop is automatically generated */
    /* Wait until STOPF flag is reset */ 
    if(FMPI2C_WaitOnSTOPFlagUntilTimeout(hfmpi2c, FMPI2C_TIMEOUT_STOPF) != HAL_OK)
    {
      if(hfmpi2c->ErrorCode == HAL_FMPI2C_ERROR_AF)
      {
        return HAL_ERROR;
      }
      else
      {
        return HAL_TIMEOUT;
      }
    }
    
    /* Clear STOP Flag */
    __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_STOPF);

    /* Clear Configuration Register 2 */
    FMPI2C_RESET_CR2(hfmpi2c);

    hfmpi2c->State = HAL_FMPI2C_STATE_READY;
    hfmpi2c->Mode = HAL_FMPI2C_MODE_NONE;

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c);

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  Read an amount of data in blocking mode from a specific memory address
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  DevAddress Target device address
  * @param  MemAddress Internal memory address
  * @param  MemAddSize Size of internal memory address
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Mem_Read(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
  uint32_t Sizetmp = 0U;

  /* Check the parameters */
  assert_param(IS_FMPI2C_MEMADD_SIZE(MemAddSize));
  
  if(hfmpi2c->State == HAL_FMPI2C_STATE_READY)
  {    
    if((pData == NULL) || (Size == 0U))
    {
      return  HAL_ERROR;
    }

    if(__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_BUSY) == SET)
    {
      return HAL_BUSY;
    }

    /* Process Locked */
    __HAL_LOCK(hfmpi2c);

    hfmpi2c->State = HAL_FMPI2C_STATE_BUSY_RX;
    hfmpi2c->Mode = HAL_FMPI2C_MODE_MEM;
    hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_NONE;

    /* Send Slave Address and Memory Address */
    if(FMPI2C_RequestMemoryRead(hfmpi2c, DevAddress, MemAddress, MemAddSize, Timeout) != HAL_OK)
    {
      if(hfmpi2c->ErrorCode == HAL_FMPI2C_ERROR_AF)
      {
        /* Process Unlocked */
        __HAL_UNLOCK(hfmpi2c);
        return HAL_ERROR;
      }
      else
      {
        /* Process Unlocked */
        __HAL_UNLOCK(hfmpi2c);
        return HAL_TIMEOUT;
      }
    }

    /* Send Slave Address */
    /* Set NBYTES to write and reload if size > MAX_NBYTE_SIZE and generate RESTART */
    /* Size > MAX_NBYTE_SIZE, need to set RELOAD bit */
    if(Size > MAX_NBYTE_SIZE)
    {
      FMPI2C_TransferConfig(hfmpi2c,DevAddress,MAX_NBYTE_SIZE, FMPI2C_RELOAD_MODE, FMPI2C_GENERATE_START_READ);
      Sizetmp = MAX_NBYTE_SIZE;
    }
    else
    {
      FMPI2C_TransferConfig(hfmpi2c,DevAddress,Size, FMPI2C_AUTOEND_MODE, FMPI2C_GENERATE_START_READ);
      Sizetmp = Size;
    }

    do
    {  
      /* Wait until RXNE flag is set */
      if(FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_RXNE, RESET, Timeout) != HAL_OK)
      {
        return HAL_TIMEOUT;
      }
          
      /* Read data from RXDR */
      (*pData++) = hfmpi2c->Instance->RXDR;

      /* Decrement the Size counter */
      Sizetmp--;
      Size--;   

      if((Sizetmp == 0U)&&(Size!=0U))
      {
        /* Wait until TCR flag is set */
        if(FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_TCR, RESET, Timeout) != HAL_OK)
        {
          return HAL_TIMEOUT;
        }
        
        if(Size > MAX_NBYTE_SIZE)
        {
          FMPI2C_TransferConfig(hfmpi2c,DevAddress,MAX_NBYTE_SIZE, FMPI2C_RELOAD_MODE, FMPI2C_NO_STARTSTOP);
          Sizetmp = MAX_NBYTE_SIZE;
        }
        else
        {
          FMPI2C_TransferConfig(hfmpi2c,DevAddress,Size, FMPI2C_AUTOEND_MODE, FMPI2C_NO_STARTSTOP);
          Sizetmp = Size;
        }
      }

    }while(Size > 0U);

    /* No need to Check TC flag, with AUTOEND mode the stop is automatically generated */
    /* Wait until STOPF flag is reset */ 
    if(FMPI2C_WaitOnSTOPFlagUntilTimeout(hfmpi2c, FMPI2C_TIMEOUT_STOPF) != HAL_OK)
    {
      if(hfmpi2c->ErrorCode == HAL_FMPI2C_ERROR_AF)
      {
        return HAL_ERROR;
      }
      else
      {
        return HAL_TIMEOUT;
      }
    }

    /* Clear STOP Flag */
    __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_STOPF);

    /* Clear Configuration Register 2 */
    FMPI2C_RESET_CR2(hfmpi2c);

    hfmpi2c->State = HAL_FMPI2C_STATE_READY;
    hfmpi2c->Mode = HAL_FMPI2C_MODE_NONE;

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c);

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  Write an amount of data in no blocking mode with Interrupt to a specific memory address
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  DevAddress Target device address
  * @param  MemAddress Internal memory address
  * @param  MemAddSize Size of internal memory address
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Mem_Write_IT(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size)
{
  /* Check the parameters */
  assert_param(IS_FMPI2C_MEMADD_SIZE(MemAddSize));

  if(hfmpi2c->State == HAL_FMPI2C_STATE_READY)
  {
    if((pData == NULL) || (Size == 0U))
    {
      return  HAL_ERROR;
    }

    if(__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_BUSY) == SET)
    {
      return HAL_BUSY;
    }

    /* Process Locked */
    __HAL_LOCK(hfmpi2c);

    hfmpi2c->State = HAL_FMPI2C_STATE_BUSY_TX;
    hfmpi2c->Mode = HAL_FMPI2C_MODE_MEM;
    hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_NONE;
    hfmpi2c->XferOptions = FMPI2C_NO_OPTION_FRAME;

    hfmpi2c->pBuffPtr = pData;
    hfmpi2c->XferCount = Size;
    if(Size > MAX_NBYTE_SIZE)
    {
      hfmpi2c->XferSize = MAX_NBYTE_SIZE;
    }
    else
    {
      hfmpi2c->XferSize = Size;
    }
    
    /* Send Slave Address and Memory Address */
    if(FMPI2C_RequestMemoryWrite(hfmpi2c, DevAddress, MemAddress, MemAddSize, FMPI2C_TIMEOUT_FLAG) != HAL_OK)
    {
      if(hfmpi2c->ErrorCode == HAL_FMPI2C_ERROR_AF)
      {
        /* Process Unlocked */
        __HAL_UNLOCK(hfmpi2c);
        return HAL_ERROR;
      }
      else
      {
        /* Process Unlocked */
        __HAL_UNLOCK(hfmpi2c);
        return HAL_TIMEOUT;
      }
    }

    /* Set NBYTES to write and reload if size > MAX_NBYTE_SIZE */
    /* Size > MAX_NBYTE_SIZE, need to set RELOAD bit */
    if( (hfmpi2c->XferSize == MAX_NBYTE_SIZE) && (hfmpi2c->XferSize < hfmpi2c->XferCount) )
    {
      FMPI2C_TransferConfig(hfmpi2c,DevAddress,hfmpi2c->XferSize, FMPI2C_RELOAD_MODE, FMPI2C_NO_STARTSTOP);
    }
    else
    {
      FMPI2C_TransferConfig(hfmpi2c,DevAddress,hfmpi2c->XferSize, FMPI2C_AUTOEND_MODE, FMPI2C_NO_STARTSTOP);
    }

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c); 

    /* Note : The FMPI2C interrupts must be enabled after unlocking current process 
              to avoid the risk of FMPI2C interrupt handle execution before current
              process unlock */
    
    /* Enable ERR, TC, STOP, NACK, TXI interrupt */
    /* possible to enable all of these */
    /* FMPI2C_IT_ERRI | FMPI2C_IT_TCI| FMPI2C_IT_STOPI| FMPI2C_IT_NACKI | FMPI2C_IT_ADDRI | FMPI2C_IT_RXI | FMPI2C_IT_TXI */
    __HAL_FMPI2C_ENABLE_IT(hfmpi2c,FMPI2C_IT_ERRI | FMPI2C_IT_TCI| FMPI2C_IT_STOPI| FMPI2C_IT_NACKI | FMPI2C_IT_TXI );
    
    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  Read an amount of data in no blocking mode with Interrupt from a specific memory address
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  DevAddress Target device address
  * @param  MemAddress Internal memory address
  * @param  MemAddSize Size of internal memory address
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Mem_Read_IT(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size)
{
  /* Check the parameters */
  assert_param(IS_FMPI2C_MEMADD_SIZE(MemAddSize));
  
  if(hfmpi2c->State == HAL_FMPI2C_STATE_READY)
  {
    if((pData == NULL) || (Size == 0U))
    {
      return  HAL_ERROR;
    }
    
    if(__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_BUSY) == SET)
    {
      return HAL_BUSY;
    }

    /* Process Locked */
    __HAL_LOCK(hfmpi2c);

    hfmpi2c->State = HAL_FMPI2C_STATE_BUSY_RX;
    hfmpi2c->Mode = HAL_FMPI2C_MODE_MEM;
    hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_NONE;
    hfmpi2c->XferOptions = FMPI2C_NO_OPTION_FRAME;

    hfmpi2c->pBuffPtr = pData;
    hfmpi2c->XferCount = Size;
    if(Size > MAX_NBYTE_SIZE)
    {
      hfmpi2c->XferSize = MAX_NBYTE_SIZE;
    }
    else
    {
      hfmpi2c->XferSize = Size;
    }

    /* Send Slave Address and Memory Address */
    if(FMPI2C_RequestMemoryRead(hfmpi2c, DevAddress, MemAddress, MemAddSize, FMPI2C_TIMEOUT_FLAG) != HAL_OK)
    {
      if(hfmpi2c->ErrorCode == HAL_FMPI2C_ERROR_AF)
      {
        /* Process Unlocked */
        __HAL_UNLOCK(hfmpi2c);
        return HAL_ERROR;
      }
      else
      {
        /* Process Unlocked */
        __HAL_UNLOCK(hfmpi2c);
        return HAL_TIMEOUT;
      }
    }

    /* Set NBYTES to write and reload if size > MAX_NBYTE_SIZE and generate RESTART */
    /* Size > MAX_NBYTE_SIZE, need to set RELOAD bit */
    if( (hfmpi2c->XferSize == MAX_NBYTE_SIZE) && (hfmpi2c->XferSize < hfmpi2c->XferCount) )
    {
      FMPI2C_TransferConfig(hfmpi2c,DevAddress,hfmpi2c->XferSize, FMPI2C_RELOAD_MODE, FMPI2C_GENERATE_START_READ);
    }
    else
    {
      FMPI2C_TransferConfig(hfmpi2c,DevAddress,hfmpi2c->XferSize, FMPI2C_AUTOEND_MODE, FMPI2C_GENERATE_START_READ);
    }

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c); 

    /* Note : The FMPI2C interrupts must be enabled after unlocking current process 
              to avoid the risk of FMPI2C interrupt handle execution before current
              process unlock */

    /* Enable ERR, TC, STOP, NACK, RXI interrupt */
    /* possible to enable all of these */
    /* FMPI2C_IT_ERRI | FMPI2C_IT_TCI| FMPI2C_IT_STOPI| FMPI2C_IT_NACKI | FMPI2C_IT_ADDRI | FMPI2C_IT_RXI | FMPI2C_IT_TXI */
    __HAL_FMPI2C_ENABLE_IT(hfmpi2c, FMPI2C_IT_ERRI | FMPI2C_IT_TCI| FMPI2C_IT_STOPI| FMPI2C_IT_NACKI | FMPI2C_IT_RXI );

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY; 
  }
}

/**
  * @brief  Write an amount of data in no blocking mode with DMA to a specific memory address
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  DevAddress Target device address
  * @param  MemAddress Internal memory address
  * @param  MemAddSize Size of internal memory address
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Mem_Write_DMA(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size)
{
  /* Check the parameters */
  assert_param(IS_FMPI2C_MEMADD_SIZE(MemAddSize));
  
  if(hfmpi2c->State == HAL_FMPI2C_STATE_READY)
  {
    if((pData == NULL) || (Size == 0U)) 
    {
      return  HAL_ERROR;
    }
    
    if(__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_BUSY) == SET)
    {
      return HAL_BUSY;
    }

    /* Process Locked */
    __HAL_LOCK(hfmpi2c);

    hfmpi2c->State = HAL_FMPI2C_STATE_BUSY_TX;
    hfmpi2c->Mode = HAL_FMPI2C_MODE_MEM;
    hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_NONE;

    hfmpi2c->pBuffPtr = pData;
    hfmpi2c->XferCount = Size;
    if(Size > MAX_NBYTE_SIZE)
    {
      hfmpi2c->XferSize = MAX_NBYTE_SIZE;
    }
    else
    {
      hfmpi2c->XferSize = Size;
    }

    /* Set the FMPI2C DMA transfer complete callback */
    hfmpi2c->hdmatx->XferCpltCallback = FMPI2C_DMAMemTransmitCplt;

    /* Set the DMA error callback */
    hfmpi2c->hdmatx->XferErrorCallback = FMPI2C_DMAError;

    /* Enable the DMA channel */
    HAL_DMA_Start_IT(hfmpi2c->hdmatx, (uint32_t)pData, (uint32_t)&hfmpi2c->Instance->TXDR, hfmpi2c->XferSize);

    /* Send Slave Address and Memory Address */
    if(FMPI2C_RequestMemoryWrite(hfmpi2c, DevAddress, MemAddress, MemAddSize, FMPI2C_TIMEOUT_FLAG) != HAL_OK)
    {
      if(hfmpi2c->ErrorCode == HAL_FMPI2C_ERROR_AF)
      {
        /* Process Unlocked */
        __HAL_UNLOCK(hfmpi2c);
        return HAL_ERROR;
      }
      else
      {
        /* Process Unlocked */
        __HAL_UNLOCK(hfmpi2c);
        return HAL_TIMEOUT;
      }
    }

    /* Send Slave Address */
    /* Set NBYTES to write and reload if size > MAX_NBYTE_SIZE */
    if( (hfmpi2c->XferSize == MAX_NBYTE_SIZE) && (hfmpi2c->XferSize < hfmpi2c->XferCount) )
    {
      FMPI2C_TransferConfig(hfmpi2c,DevAddress,hfmpi2c->XferSize, FMPI2C_RELOAD_MODE, FMPI2C_NO_STARTSTOP);
    }
    else
    {
      FMPI2C_TransferConfig(hfmpi2c,DevAddress,hfmpi2c->XferSize, FMPI2C_AUTOEND_MODE, FMPI2C_NO_STARTSTOP);
    }
    
    /* Wait until TXIS flag is set */
    if(FMPI2C_WaitOnTXISFlagUntilTimeout(hfmpi2c, FMPI2C_TIMEOUT_TXIS) != HAL_OK)
    {
      if(hfmpi2c->ErrorCode == HAL_FMPI2C_ERROR_AF)
      {
        return HAL_ERROR;
      }
      else
      {
        return HAL_TIMEOUT;
      }
    }

    /* Enable DMA Request */
    hfmpi2c->Instance->CR1 |= FMPI2C_CR1_TXDMAEN;
    
    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c);

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  Reads an amount of data in no blocking mode with DMA from a specific memory address.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  DevAddress Target device address
  * @param  MemAddress Internal memory address
  * @param  MemAddSize Size of internal memory address
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be read
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Mem_Read_DMA(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size)
{
  /* Check the parameters */
  assert_param(IS_FMPI2C_MEMADD_SIZE(MemAddSize));

  if(hfmpi2c->State == HAL_FMPI2C_STATE_READY)
  {
    if((pData == NULL) || (Size == 0U)) 
    {
      return  HAL_ERROR;
    }

    if(__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_BUSY) == SET)
    {
      return HAL_BUSY;
    }

    /* Process Locked */
    __HAL_LOCK(hfmpi2c);

    hfmpi2c->State = HAL_FMPI2C_STATE_BUSY_RX;
    hfmpi2c->Mode = HAL_FMPI2C_MODE_MEM;

    hfmpi2c->pBuffPtr = pData;
    hfmpi2c->XferCount = Size;
    if(Size > MAX_NBYTE_SIZE)
    {
      hfmpi2c->XferSize = MAX_NBYTE_SIZE;
    }
    else
    {
      hfmpi2c->XferSize = Size;
    }

    /* Set the FMPI2C DMA transfer complete callback */
    hfmpi2c->hdmarx->XferCpltCallback = FMPI2C_DMAMemReceiveCplt;

    /* Set the DMA error callback */
    hfmpi2c->hdmarx->XferErrorCallback = FMPI2C_DMAError;

    /* Enable the DMA channel */
    HAL_DMA_Start_IT(hfmpi2c->hdmarx, (uint32_t)&hfmpi2c->Instance->RXDR, (uint32_t)pData, hfmpi2c->XferSize);

    /* Send Slave Address and Memory Address */
    if(FMPI2C_RequestMemoryRead(hfmpi2c, DevAddress, MemAddress, MemAddSize, FMPI2C_TIMEOUT_FLAG) != HAL_OK)
    {
      if(hfmpi2c->ErrorCode == HAL_FMPI2C_ERROR_AF)
      {
        /* Process Unlocked */
        __HAL_UNLOCK(hfmpi2c);
        return HAL_ERROR;
      }
      else
      {
        /* Process Unlocked */
        __HAL_UNLOCK(hfmpi2c);
        return HAL_TIMEOUT;
      }
    }

    /* Set NBYTES to write and reload if size > MAX_NBYTE_SIZE and generate RESTART */
    if( (hfmpi2c->XferSize == MAX_NBYTE_SIZE) && (hfmpi2c->XferSize < hfmpi2c->XferCount) )
    {
      FMPI2C_TransferConfig(hfmpi2c,DevAddress,hfmpi2c->XferSize, FMPI2C_RELOAD_MODE, FMPI2C_GENERATE_START_READ);
    }
    else
    {
      FMPI2C_TransferConfig(hfmpi2c,DevAddress,hfmpi2c->XferSize, FMPI2C_AUTOEND_MODE, FMPI2C_GENERATE_START_READ);
    }

    /* Wait until RXNE flag is set */
    if(FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_RXNE, RESET, FMPI2C_TIMEOUT_RXNE) != HAL_OK)
    {
      return HAL_TIMEOUT;
    }

    /* Enable DMA Request */
    hfmpi2c->Instance->CR1 |= FMPI2C_CR1_RXDMAEN;

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c);

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  Checks if target device is ready for communication. 
  * @note   This function is used with Memory devices
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  DevAddress Target device address
  * @param  Trials Number of trials
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_IsDeviceReady(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t DevAddress, uint32_t Trials, uint32_t Timeout)
{
  uint32_t tickstart = 0U;

  __IO uint32_t FMPI2C_Trials = 0U;

  if(hfmpi2c->State == HAL_FMPI2C_STATE_READY)
  {
    if(__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_BUSY) == SET)
    {
      return HAL_BUSY;
    }

    /* Process Locked */
    __HAL_LOCK(hfmpi2c);

    hfmpi2c->State = HAL_FMPI2C_STATE_BUSY;
    hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_NONE;

    do
    {
      /* Generate Start */
      hfmpi2c->Instance->CR2 = FMPI2C_GENERATE_START(hfmpi2c->Init.AddressingMode,DevAddress);

      /* No need to Check TC flag, with AUTOEND mode the stop is automatically generated */
      /* Wait until STOPF flag is set or a NACK flag is set*/
      tickstart = HAL_GetTick();
      while((__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_STOPF) == RESET) && (__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_AF) == RESET) && (hfmpi2c->State != HAL_FMPI2C_STATE_TIMEOUT))
      {
        if(Timeout != HAL_MAX_DELAY)
        {
          if((Timeout == 0U) || ((HAL_GetTick()-tickstart) > Timeout))
          {
            /* Device is ready */
            hfmpi2c->State = HAL_FMPI2C_STATE_READY;
            /* Process Unlocked */
            __HAL_UNLOCK(hfmpi2c);
            return HAL_TIMEOUT;
          }
        }
      }
      
      /* Check if the NACKF flag has not been set */
      if(__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_AF) == RESET)
      {
        /* Wait until STOPF flag is reset */ 
        if(FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_STOPF, RESET, Timeout) != HAL_OK)
        {
          return HAL_TIMEOUT;
        }
        
        /* Clear STOP Flag */
        __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_STOPF);

        /* Device is ready */
        hfmpi2c->State = HAL_FMPI2C_STATE_READY;
        
        /* Process Unlocked */
        __HAL_UNLOCK(hfmpi2c);
        
        return HAL_OK;
      }
      else
      {
        /* Wait until STOPF flag is reset */ 
        if(FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_STOPF, RESET, Timeout) != HAL_OK)
        {
          return HAL_TIMEOUT;
        }

        /* Clear NACK Flag */
        __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_AF);

        /* Clear STOP Flag, auto generated with autoend*/
        __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_STOPF);
      }

      /* Check if the maximum allowed number of trials has been reached */
      if(FMPI2C_Trials++ == Trials)
      {
        /* Generate Stop */
        hfmpi2c->Instance->CR2 |= FMPI2C_CR2_STOP;
        
        /* Wait until STOPF flag is reset */
        if(FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_STOPF, RESET, Timeout) != HAL_OK)
        {
          return HAL_TIMEOUT;
        }

        /* Clear STOP Flag */
        __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_STOPF);
      }
    }while(FMPI2C_Trials < Trials);

    hfmpi2c->State = HAL_FMPI2C_STATE_READY;

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c);
        
    return HAL_TIMEOUT;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  Sequential transmit in master FMPI2C mode an amount of data in no blocking mode with Interrupt.
  * @note   This interface allow to manage repeated start condition when a direction change during transfer
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  DevAddress Target device address
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  XferOptions Options of Transfer, value of @ref FMPI2C_XferOptions_definition
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Master_Sequential_Transmit_IT(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t XferOptions)
{
  /* Check the parameters */
  assert_param(IS_FMPI2C_TRANSFER_OPTIONS_REQUEST(XferOptions));

  if(hfmpi2c->State == HAL_FMPI2C_STATE_READY)
  {
    /* Process Locked */
    __HAL_LOCK(hfmpi2c);
    
    hfmpi2c->State = HAL_FMPI2C_STATE_BUSY_TX;
    hfmpi2c->Mode = HAL_FMPI2C_MODE_MASTER;
    hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_NONE;

    /* Prepare transfer parameters */
    hfmpi2c->pBuffPtr = pData;
    hfmpi2c->XferCount = Size;
    hfmpi2c->XferOptions = XferOptions;

    if(Size > MAX_NBYTE_SIZE)
    {
      hfmpi2c->XferSize = MAX_NBYTE_SIZE;
    }
    else
    {
      hfmpi2c->XferSize = Size;
    }

    /* Send Slave Address */
    /* Set NBYTES to write and reload if size > MAX_NBYTE_SIZE and generate RESTART */
    if((hfmpi2c->XferSize == MAX_NBYTE_SIZE) && (hfmpi2c->XferSize < hfmpi2c->XferCount))
    {
      FMPI2C_TransferConfig(hfmpi2c, DevAddress, hfmpi2c->XferSize, FMPI2C_RELOAD_MODE, FMPI2C_GENERATE_START_WRITE);
    }
    else
    {
      /* If transfer direction not change, do not generate Restart Condition */
      /* Mean Previous state is same as current state */
      if(hfmpi2c->PreviousState == FMPI2C_STATE_SLAVE_BUSY_TX)
      {
        FMPI2C_TransferConfig(hfmpi2c, DevAddress, hfmpi2c->XferSize, hfmpi2c->XferOptions, FMPI2C_NO_STARTSTOP);
      }
      /* Else transfer direction change, so generate Restart with new transfer direction */
      else
      {
        FMPI2C_TransferConfig(hfmpi2c, DevAddress, hfmpi2c->XferSize, hfmpi2c->XferOptions, FMPI2C_GENERATE_START_WRITE);
      }
    }

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c); 

    /* Note : The FMPI2C interrupts must be enabled after unlocking current process 
              to avoid the risk of FMPI2C interrupt handle execution before current
              process unlock */
    FMPI2C_Enable_IRQ(hfmpi2c, FMPI2C_IT_TX);

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  Sequential receive in master FMPI2C mode an amount of data in no blocking mode with Interrupt
  * @note   This interface allow to manage repeated start condition when a direction change during transfer
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  DevAddress Target device address
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  XferOptions Options of Transfer, value of @ref FMPI2C_XferOptions_definition
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Master_Sequential_Receive_IT(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t XferOptions)
{
  /* Check the parameters */
  assert_param(IS_FMPI2C_TRANSFER_OPTIONS_REQUEST(XferOptions));

  if(hfmpi2c->State == HAL_FMPI2C_STATE_READY)
  {
    /* Process Locked */
    __HAL_LOCK(hfmpi2c);
    
    hfmpi2c->State = HAL_FMPI2C_STATE_BUSY_RX;
    hfmpi2c->Mode = HAL_FMPI2C_MODE_MASTER;
    hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_NONE;
    
    /* Prepare transfer parameters */
    hfmpi2c->pBuffPtr = pData;
    hfmpi2c->XferCount = Size;
    hfmpi2c->XferOptions = XferOptions;

    if(Size > MAX_NBYTE_SIZE)
    {
      hfmpi2c->XferSize = MAX_NBYTE_SIZE;
    }
    else
    {
      hfmpi2c->XferSize = Size;
    }

    /* Send Slave Address */
    /* Set NBYTES to write and reload if size > MAX_NBYTE_SIZE and generate RESTART */
    if( (hfmpi2c->XferSize == MAX_NBYTE_SIZE) && (hfmpi2c->XferSize < hfmpi2c->XferCount) )
    {
      FMPI2C_TransferConfig(hfmpi2c,DevAddress, hfmpi2c->XferSize, FMPI2C_RELOAD_MODE, FMPI2C_GENERATE_START_READ);
    }
    else
    {
      /* If transfer direction not change, do not generate Restart Condition */
      /* Mean Previous state is same as current state */
      if(hfmpi2c->PreviousState == FMPI2C_STATE_MASTER_BUSY_RX)
      {
        FMPI2C_TransferConfig(hfmpi2c,DevAddress,hfmpi2c->XferSize, hfmpi2c->XferOptions, FMPI2C_NO_STARTSTOP);
      }
      /* Else transfer direction change, so generate Restart with new transfer direction */
      else
      {
        FMPI2C_TransferConfig(hfmpi2c,DevAddress,hfmpi2c->XferSize, hfmpi2c->XferOptions, FMPI2C_GENERATE_START_READ);
      }
    }

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c); 

    /* Note : The FMPI2C interrupts must be enabled after unlocking current process 
              to avoid the risk of FMPI2C interrupt handle execution before current
              process unlock */
    FMPI2C_Enable_IRQ(hfmpi2c, FMPI2C_IT_RX);
    
    return HAL_OK;
  }
  else
  {
    return HAL_BUSY; 
  }
}

/**
  * @brief  Sequential transmit in slave/device FMPI2C mode an amount of data in no blocking mode with Interrupt
  * @note   This interface allow to manage repeated start condition when a direction change during transfer
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  XferOptions Options of Transfer, value of @ref FMPI2C_XferOptions_definition
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Slave_Sequential_Transmit_IT(FMPI2C_HandleTypeDef *hfmpi2c, uint8_t *pData, uint16_t Size, uint32_t XferOptions)
{
  /* Check the parameters */
  assert_param(IS_FMPI2C_TRANSFER_OPTIONS_REQUEST(XferOptions));

  if(hfmpi2c->State == HAL_FMPI2C_STATE_LISTEN)
  {
    if((pData == NULL) || (Size == 0U))
    {
      return  HAL_ERROR;
    }

    /* Disable Interrupts, to prevent preemption during treatment in case of multicall */
    FMPI2C_Disable_IRQ(hfmpi2c, FMPI2C_IT_ADDR | FMPI2C_IT_TX);

    /* Process Locked */
    __HAL_LOCK(hfmpi2c);

    hfmpi2c->State = HAL_FMPI2C_STATE_BUSY_TX_LISTEN;
    hfmpi2c->Mode = HAL_FMPI2C_MODE_SLAVE;
    hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_NONE;
    
    /* Enable Address Acknowledge */
    hfmpi2c->Instance->CR2 &= ~FMPI2C_CR2_NACK;

    /* Prepare transfer parameters */
    hfmpi2c->pBuffPtr = pData;
    hfmpi2c->XferSize = Size;
    hfmpi2c->XferCount = Size;
    hfmpi2c->XferOptions = XferOptions;

    /* Clear ADDR flag after prepare the transfer parameters */
    /* This action will generate an acknowledge to the Master */
    __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c,FMPI2C_FLAG_ADDR);

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c); 

    /* Note : The FMPI2C interrupts must be enabled after unlocking current process 
    to avoid the risk of FMPI2C interrupt handle execution before current
    process unlock */
    /* REnable ADDR interrupt */
    FMPI2C_Enable_IRQ(hfmpi2c, FMPI2C_IT_TX | FMPI2C_IT_ADDR);

    return HAL_OK;
  }
  else
  {
    return HAL_ERROR; 
  }
}

/**
  * @brief  Sequential receive in slave/device FMPI2C mode an amount of data in no blocking mode with Interrupt
  * @note   This interface allow to manage repeated start condition when a direction change during transfer
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  XferOptions Options of Transfer, value of @ref FMPI2C_XferOptions_definition
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Slave_Sequential_Receive_IT(FMPI2C_HandleTypeDef *hfmpi2c, uint8_t *pData, uint16_t Size, uint32_t XferOptions)
{
  /* Check the parameters */
  assert_param(IS_FMPI2C_TRANSFER_OPTIONS_REQUEST(XferOptions));

  if(hfmpi2c->State == HAL_FMPI2C_STATE_LISTEN)
  {
    if((pData == NULL) || (Size == 0U)) 
    {
      return  HAL_ERROR;
    }

    /* Disable Interrupts, to prevent preemption during treatment in case of multicall */
    FMPI2C_Disable_IRQ(hfmpi2c, FMPI2C_IT_ADDR | FMPI2C_IT_RX);

    /* Process Locked */
    __HAL_LOCK(hfmpi2c);

    hfmpi2c->State = HAL_FMPI2C_STATE_BUSY_RX_LISTEN;
    hfmpi2c->Mode = HAL_FMPI2C_MODE_SLAVE;
    hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_NONE;

    /* Enable Address Acknowledge */
    hfmpi2c->Instance->CR2 &= ~FMPI2C_CR2_NACK;

    /* Prepare transfer parameters */
    hfmpi2c->pBuffPtr = pData;
    hfmpi2c->XferSize = Size;
    hfmpi2c->XferCount = Size;
    hfmpi2c->XferOptions = XferOptions;

    /* Clear ADDR flag after prepare the transfer parameters */
    /* This action will generate an acknowledge to the Master */
    __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c,FMPI2C_FLAG_ADDR);

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c); 

    /* Note : The FMPI2C interrupts must be enabled after unlocking current process 
    to avoid the risk of FMPI2C interrupt handle execution before current
    process unlock */
    /* REnable ADDR interrupt */
    FMPI2C_Enable_IRQ(hfmpi2c, FMPI2C_IT_RX | FMPI2C_IT_ADDR);

    return HAL_OK;
  }
  else
  {
    return HAL_ERROR; 
  }
}

/**
  * @brief  Enable the Address listen mode with Interrupt.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_EnableListen_IT(FMPI2C_HandleTypeDef *hfmpi2c)
{
  if(hfmpi2c->State == HAL_FMPI2C_STATE_READY)
  {
    hfmpi2c->State = HAL_FMPI2C_STATE_LISTEN;

    /* Enable the Address Match interrupt */
    FMPI2C_Enable_IRQ(hfmpi2c, FMPI2C_IT_ADDR);

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  Disable the Address listen mode with Interrupt.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_DisableListen_IT(FMPI2C_HandleTypeDef *hfmpi2c)
{
  /* Declaration of tmp to prevent undefined behavior of volatile usage */
  uint32_t tmp;

  /* Disable Address listen mode only if a transfer is not ongoing */
  if(hfmpi2c->State == HAL_FMPI2C_STATE_LISTEN)
  {
    tmp = (uint32_t)(hfmpi2c->State) & FMPI2C_STATE_MSK;
    hfmpi2c->PreviousState = tmp | (uint32_t)(hfmpi2c->Mode);
    hfmpi2c->State = HAL_FMPI2C_STATE_READY;
    hfmpi2c->Mode = HAL_FMPI2C_MODE_NONE;

    /* Disable the Address Match interrupt */
    FMPI2C_Disable_IRQ(hfmpi2c, FMPI2C_IT_ADDR);

    return HAL_OK;
  }
  else
  {
    return HAL_BUSY;
  }
}

/**
  * @brief  Abort a master/host FMPI2C process communication with Interrupt.
  * @note   This abort can be called only if state is ready
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  DevAddress Target device address
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_FMPI2C_Master_Abort_IT(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t DevAddress)
{
  if((hfmpi2c->Mode == HAL_FMPI2C_MODE_MASTER) || \
     ((hfmpi2c->ErrorCode != HAL_FMPI2C_ERROR_NONE) && ((hfmpi2c->PreviousState & ((uint32_t)HAL_FMPI2C_MODE_MASTER)) == HAL_FMPI2C_MODE_MASTER)))
  {
    /* Process Locked */
    __HAL_LOCK(hfmpi2c);
    
    /* Keep the same state as previous */
    /* to perform as well the call of the corresponding end of transfer callback */
    if((hfmpi2c->PreviousState == FMPI2C_STATE_MASTER_BUSY_TX) && (hfmpi2c->ErrorCode != HAL_FMPI2C_ERROR_NONE))
    {
      hfmpi2c->State = HAL_FMPI2C_STATE_BUSY_TX;
      hfmpi2c->Mode = HAL_FMPI2C_MODE_MASTER;
    }
    else if((hfmpi2c->PreviousState == FMPI2C_STATE_MASTER_BUSY_RX) && (hfmpi2c->ErrorCode != HAL_FMPI2C_ERROR_NONE))
    {
      hfmpi2c->State = HAL_FMPI2C_STATE_BUSY_RX;
      hfmpi2c->Mode = HAL_FMPI2C_MODE_MASTER;
    }

    hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_NONE;
    
    /* Set NBYTES to 1 to generate a dummy read on FMPI2C peripheral */
    /* Set AUTOEND mode, this will generate a NACK then STOP condition to abort the current transfer */
    FMPI2C_TransferConfig(hfmpi2c, DevAddress, 1U, FMPI2C_AUTOEND_MODE, FMPI2C_GENERATE_STOP);
    
    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c); 

    /* Note : The FMPI2C interrupts must be enabled after unlocking current process 
              to avoid the risk of FMPI2C interrupt handle execution before current
              process unlock */
    if((hfmpi2c->State == HAL_FMPI2C_STATE_BUSY_TX) && (hfmpi2c->Mode == HAL_FMPI2C_MODE_MASTER))
    {
      FMPI2C_Enable_IRQ(hfmpi2c, FMPI2C_IT_TX);
    }
    if((hfmpi2c->State == HAL_FMPI2C_STATE_BUSY_RX) && (hfmpi2c->Mode == HAL_FMPI2C_MODE_MASTER))
    {
      FMPI2C_Enable_IRQ(hfmpi2c, FMPI2C_IT_RX);
    }

    return HAL_OK;
  }
  else
  {
    /* Wrong usage of abort function */
    /* This function should be used only in case of abort monitored by master device */
    return HAL_ERROR;
  }
}

/**
  * @}
  */

/** @defgroup FMPI2C_IRQ_Handler_and_Callbacks IRQ Handler and Callbacks
  * @{
  */

/**
  * @brief  This function handles FMPI2C event interrupt request.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @retval None
  */
void HAL_FMPI2C_EV_IRQHandler(FMPI2C_HandleTypeDef *hfmpi2c)
{
  uint32_t tmpisrvalue = 0U;
  
  /* Use a local variable to store the current ISR flags */
  /* This action will avoid a wrong treatment due to ISR flags change during interrupt handler */
  tmpisrvalue = FMPI2C_GET_ISR_REG(hfmpi2c);

  /* FMPI2C in mode Transmitter ---------------------------------------------------*/
  if(((FMPI2C_CHECK_FLAG(tmpisrvalue, FMPI2C_FLAG_TXIS) != RESET) || (FMPI2C_CHECK_FLAG(tmpisrvalue, FMPI2C_FLAG_TCR) != RESET) || (FMPI2C_CHECK_FLAG(tmpisrvalue, FMPI2C_FLAG_TC) != RESET) || (FMPI2C_CHECK_FLAG(tmpisrvalue, FMPI2C_FLAG_STOPF) != RESET) || (FMPI2C_CHECK_FLAG(tmpisrvalue, FMPI2C_FLAG_AF) != RESET)) && (__HAL_FMPI2C_GET_IT_SOURCE(hfmpi2c, (FMPI2C_IT_TCI| FMPI2C_IT_STOPI| FMPI2C_IT_NACKI | FMPI2C_IT_TXI)) != RESET))
  {
    /* Slave mode selected */
    if(hfmpi2c->Mode  == HAL_FMPI2C_MODE_SLAVE)
    {
      FMPI2C_Slave_ISR(hfmpi2c);
    }
    /* Master or Memory mode selected */
    if((hfmpi2c->Mode == HAL_FMPI2C_MODE_MASTER) || (hfmpi2c->Mode == HAL_FMPI2C_MODE_MEM))
    {
      FMPI2C_Master_ISR(hfmpi2c);
    }
  }

  /* FMPI2C in mode Receiver ----------------------------------------------------*/
  if(((FMPI2C_CHECK_FLAG(tmpisrvalue, FMPI2C_FLAG_RXNE) != RESET) || (FMPI2C_CHECK_FLAG(tmpisrvalue, FMPI2C_FLAG_TCR) != RESET) || (FMPI2C_CHECK_FLAG(tmpisrvalue, FMPI2C_FLAG_TC) != RESET) || (FMPI2C_CHECK_FLAG(tmpisrvalue, FMPI2C_FLAG_STOPF) != RESET) || (FMPI2C_CHECK_FLAG(tmpisrvalue, FMPI2C_FLAG_AF) != RESET)) && (__HAL_FMPI2C_GET_IT_SOURCE(hfmpi2c, (FMPI2C_IT_TCI| FMPI2C_IT_STOPI| FMPI2C_IT_NACKI | FMPI2C_IT_RXI)) != RESET))
  {
    /* Slave mode selected */
    if(hfmpi2c->Mode  == HAL_FMPI2C_MODE_SLAVE)
    {
      FMPI2C_Slave_ISR(hfmpi2c);
    }
    /* Master or Memory mode selected */
    if((hfmpi2c->Mode == HAL_FMPI2C_MODE_MASTER) || (hfmpi2c->Mode == HAL_FMPI2C_MODE_MEM))
    {
      FMPI2C_Master_ISR(hfmpi2c);
    }
  }

   /* FMPI2C in mode Listener Only --------------------------------------------------*/
  if(((FMPI2C_CHECK_FLAG(tmpisrvalue, FMPI2C_FLAG_ADDR) != RESET) || (FMPI2C_CHECK_FLAG(tmpisrvalue, FMPI2C_FLAG_STOPF) != RESET) || (FMPI2C_CHECK_FLAG(tmpisrvalue, FMPI2C_FLAG_AF) != RESET)) && ((__HAL_FMPI2C_GET_IT_SOURCE(hfmpi2c, FMPI2C_IT_ADDRI) != RESET) || (__HAL_FMPI2C_GET_IT_SOURCE(hfmpi2c, FMPI2C_IT_STOPI) != RESET) || (__HAL_FMPI2C_GET_IT_SOURCE(hfmpi2c, FMPI2C_IT_NACKI) != RESET)))
  {
    if(hfmpi2c->XferOptions != FMPI2C_NO_OPTION_FRAME)
    {
      if((hfmpi2c->State == HAL_FMPI2C_STATE_LISTEN) || (hfmpi2c->State == HAL_FMPI2C_STATE_BUSY_TX_LISTEN) || (hfmpi2c->State == HAL_FMPI2C_STATE_BUSY_RX_LISTEN))
      {
        FMPI2C_Slave_ISR(hfmpi2c);
      }
    }
    else
    {
      if(hfmpi2c->Mode == HAL_FMPI2C_MODE_SLAVE)
      {
        FMPI2C_Slave_ISR(hfmpi2c);
      }
    }
  }
}

/**
  * @brief  This function handles FMPI2C error interrupt request.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @retval None
  */
void HAL_FMPI2C_ER_IRQHandler(FMPI2C_HandleTypeDef *hfmpi2c)
{
  /* FMPI2C Bus error interrupt occurred ------------------------------------*/
  if((__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_BERR) == SET) && (__HAL_FMPI2C_GET_IT_SOURCE(hfmpi2c, FMPI2C_IT_ERRI) == SET))
  { 
    hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_BERR;
   
    /* Clear BERR flag */
    __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_BERR);
  }
  
  /* FMPI2C Over-Run/Under-Run interrupt occurred ----------------------------------------*/
  if((__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_OVR) == SET) && (__HAL_FMPI2C_GET_IT_SOURCE(hfmpi2c, FMPI2C_IT_ERRI) == SET))
  {
    hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_OVR;

    /* Clear OVR flag */
    __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_OVR);
  }

  /* FMPI2C Arbitration Loss error interrupt occurred -------------------------------------*/
  if((__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_ARLO) == SET) && (__HAL_FMPI2C_GET_IT_SOURCE(hfmpi2c, FMPI2C_IT_ERRI) == SET))
  {
    hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_ARLO;

    /* Clear ARLO flag */
    __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_ARLO);
  }

  /* Call the Error Callback in case of Error detected */
  if((hfmpi2c->ErrorCode & (HAL_FMPI2C_ERROR_BERR | HAL_FMPI2C_ERROR_OVR | HAL_FMPI2C_ERROR_ARLO)) !=  HAL_FMPI2C_ERROR_NONE)
  {
    if(((hfmpi2c->State == HAL_FMPI2C_STATE_BUSY_TX) || (hfmpi2c->State == HAL_FMPI2C_STATE_BUSY_RX)) && (hfmpi2c->Mode == HAL_FMPI2C_MODE_SLAVE))
    {
      /* Reset only HAL_FMPI2C_STATE_SLAVE_BUSY_XX */
      /* keep HAL_FMPI2C_STATE_LISTEN if set */
      hfmpi2c->PreviousState = FMPI2C_STATE_NONE;
      hfmpi2c->State = HAL_FMPI2C_STATE_LISTEN;
    }
    else
    {
      hfmpi2c->PreviousState = FMPI2C_STATE_NONE;
      hfmpi2c->State = HAL_FMPI2C_STATE_READY;
    }

    HAL_FMPI2C_ErrorCallback(hfmpi2c);
  }
}

/**
  * @brief  Master Tx Transfer completed callback.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @retval None
  */
 __weak void HAL_FMPI2C_MasterTxCpltCallback(FMPI2C_HandleTypeDef *hfmpi2c)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hfmpi2c);
  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_FMPI2C_TxCpltCallback could be implemented in the user file
   */
}

/**
  * @brief  Master Rx Transfer completed callback.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @retval None
  */
__weak void HAL_FMPI2C_MasterRxCpltCallback(FMPI2C_HandleTypeDef *hfmpi2c)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hfmpi2c);
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_FMPI2C_TxCpltCallback could be implemented in the user file
   */
}

/** @brief  Slave Tx Transfer completed callback.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @retval None
  */
 __weak void HAL_FMPI2C_SlaveTxCpltCallback(FMPI2C_HandleTypeDef *hfmpi2c)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hfmpi2c);
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_FMPI2C_TxCpltCallback could be implemented in the user file
   */
}

/**
  * @brief  Slave Rx Transfer completed callback.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @retval None
  */
__weak void HAL_FMPI2C_SlaveRxCpltCallback(FMPI2C_HandleTypeDef *hfmpi2c)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hfmpi2c);
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_FMPI2C_TxCpltCallback could be implemented in the user file
   */
}

/**
  * @brief  Slave Address Match callback.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  TransferDirection: Master request Transfer Direction (Write/Read), value of @ref FMPI2C_XferOptions_definition
  * @param  AddrMatchCode: Address Match Code
  * @retval None
  */
__weak void HAL_FMPI2C_AddrCallback(FMPI2C_HandleTypeDef *hfmpi2c, uint8_t TransferDirection, uint16_t AddrMatchCode)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hfmpi2c);
  UNUSED(TransferDirection);
  UNUSED(AddrMatchCode);
  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_FMPI2C_AddrCallback() could be implemented in the user file
   */
}

/**
  * @brief  Listen Complete callback.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @retval None
  */
__weak void HAL_FMPI2C_ListenCpltCallback(FMPI2C_HandleTypeDef *hfmpi2c)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hfmpi2c);
  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_FMPI2C_ListenCpltCallback() could be implemented in the user file
   */
}

/**
  * @brief  Memory Tx Transfer completed callback.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @retval None
  */
 __weak void HAL_FMPI2C_MemTxCpltCallback(FMPI2C_HandleTypeDef *hfmpi2c)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hfmpi2c);
  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_FMPI2C_TxCpltCallback could be implemented in the user file
   */ 
}

/**
  * @brief  Memory Rx Transfer completed callback.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @retval None
  */
__weak void HAL_FMPI2C_MemRxCpltCallback(FMPI2C_HandleTypeDef *hfmpi2c)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hfmpi2c);
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_FMPI2C_TxCpltCallback could be implemented in the user file
   */
}

/**
  * @brief  FMPI2C error callback.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @retval None
  */
 __weak void HAL_FMPI2C_ErrorCallback(FMPI2C_HandleTypeDef *hfmpi2c)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hfmpi2c);
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_FMPI2C_ErrorCallback could be implemented in the user file
   */
}

/**
  * @}
  */

/** @defgroup FMPI2C_Exported_Functions_Group3 Peripheral State and Errors functions
  *  @brief   Peripheral State and Errors functions
  *
@verbatim   
 ===============================================================================
            ##### Peripheral State and Errors functions #####
 ===============================================================================  
    [..]
    This subsection permit to get in run-time the status of the peripheral 
    and the data flow.

@endverbatim
  * @{
  */

/**
  * @brief  Return the FMPI2C handle state.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @retval HAL state
  */
HAL_FMPI2C_StateTypeDef HAL_FMPI2C_GetState(FMPI2C_HandleTypeDef *hfmpi2c)
{
  /* Return FMPI2C handle state */
  return hfmpi2c->State;
}

/**
  * @brief  Returns the FMPI2C Master, Slave, Memory or no mode.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *         the configuration information for FMPI2C module
  * @retval HAL mode
  */
HAL_FMPI2C_ModeTypeDef HAL_FMPI2C_GetMode(FMPI2C_HandleTypeDef *hfmpi2c)
{
  return hfmpi2c->Mode;
}

/**
  * @brief  Return the FMPI2C error code.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *              the configuration information for the specified FMPI2C.
  * @retval FMPI2C Error Code
  */
uint32_t HAL_FMPI2C_GetError(FMPI2C_HandleTypeDef *hfmpi2c)
{
  return hfmpi2c->ErrorCode;
}

/**
  * @}
  */  

/**
  * @}
  */   

/** @addtogroup FMPI2C_Private_Functions
  * @{
  */
/**
  * @brief  Interrupt Sub-Routine which handle the Interrupt Flags Master Mode.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @retval HAL status
  */
static HAL_StatusTypeDef FMPI2C_Master_ISR(FMPI2C_HandleTypeDef *hfmpi2c) 
{
  uint16_t DevAddress;

  /* Process Locked */
  __HAL_LOCK(hfmpi2c);

  if(__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_AF) != RESET)
  {
    /* Clear NACK Flag */
    __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_AF);

    /* Set corresponding Error Code */
    /* No need to generate STOP, it is automatically done */
    hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_AF;

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c);
    
    /* Call the Error callback to prevent upper layer */
    HAL_FMPI2C_ErrorCallback(hfmpi2c);
  }
  else if(__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_STOPF) != RESET)
  {
    if(hfmpi2c->State == HAL_FMPI2C_STATE_BUSY_TX)
    {
      /* Disable Interrupt */
      FMPI2C_Disable_IRQ(hfmpi2c, FMPI2C_IT_TX);

      /* Clear STOP Flag */
      __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_STOPF);

      /* Clear Configuration Register 2 */
      FMPI2C_RESET_CR2(hfmpi2c);

      if(hfmpi2c->Mode == HAL_FMPI2C_MODE_MEM)
      {
        /* Disable the selected FMPI2C peripheral */
        __HAL_FMPI2C_DISABLE(hfmpi2c);

        hfmpi2c->PreviousState = FMPI2C_STATE_MEM_BUSY_TX;
        hfmpi2c->State = HAL_FMPI2C_STATE_READY;

        /* Process Unlocked */
        __HAL_UNLOCK(hfmpi2c);

        /* REenable the selected FMPI2C peripheral */
        __HAL_FMPI2C_ENABLE(hfmpi2c);

        /* Call the corresponding callback to inform upper layer of End of Transfer */
        HAL_FMPI2C_MemTxCpltCallback(hfmpi2c);
      }
      else if(hfmpi2c->Mode == HAL_FMPI2C_MODE_MASTER)
      {
        /* Flush TX register if not empty */
        if(__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_TXE) == RESET)
        {
          __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_TXE);
        }

        hfmpi2c->PreviousState = FMPI2C_STATE_MASTER_BUSY_TX;
        hfmpi2c->State = HAL_FMPI2C_STATE_READY;

        /* Process Unlocked */
        __HAL_UNLOCK(hfmpi2c);

        /* Call the corresponding callback to inform upper layer of End of Transfer */
        HAL_FMPI2C_MasterTxCpltCallback(hfmpi2c);
      }
    }
    else if(hfmpi2c->State == HAL_FMPI2C_STATE_BUSY_RX)
    {
      /* Disable Interrupt */
      FMPI2C_Disable_IRQ(hfmpi2c, FMPI2C_IT_RX);

      /* Clear STOP Flag */
      __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_STOPF);

      /* Clear Configuration Register 2 */
      FMPI2C_RESET_CR2(hfmpi2c);

      if(hfmpi2c->Mode == HAL_FMPI2C_MODE_MEM)
      {
        hfmpi2c->PreviousState = FMPI2C_STATE_MEM_BUSY_RX;
        hfmpi2c->State = HAL_FMPI2C_STATE_READY;

        /* Process Unlocked */
        __HAL_UNLOCK(hfmpi2c);

        /* Call the corresponding callback to inform upper layer of End of Transfer */
        HAL_FMPI2C_MemRxCpltCallback(hfmpi2c);
      }
      else if(hfmpi2c->Mode == HAL_FMPI2C_MODE_MASTER)
      {
        hfmpi2c->PreviousState = FMPI2C_STATE_MASTER_BUSY_RX;
        hfmpi2c->State = HAL_FMPI2C_STATE_READY;

        /* Process Unlocked */
        __HAL_UNLOCK(hfmpi2c);
        
        /* Call the corresponding callback to inform upper layer of End of Transfer */
        HAL_FMPI2C_MasterRxCpltCallback(hfmpi2c);
      }
    }
  }
  else if(__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_RXNE) != RESET)
  {
    /* Read data from RXDR */
    (*hfmpi2c->pBuffPtr++) = hfmpi2c->Instance->RXDR;
    hfmpi2c->XferSize--;
    hfmpi2c->XferCount--;
  }
  else if(__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_TXIS) != RESET)
  {
    /* Write data to TXDR */
    hfmpi2c->Instance->TXDR = (*hfmpi2c->pBuffPtr++);
    hfmpi2c->XferSize--;
    hfmpi2c->XferCount--;	
  }
  else if(__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_TCR) != RESET)
  {
    if((hfmpi2c->XferSize == 0U)&&(hfmpi2c->XferCount!=0U))
    {
      DevAddress = (hfmpi2c->Instance->CR2 & FMPI2C_CR2_SADD);
      
      if(hfmpi2c->XferCount > MAX_NBYTE_SIZE)
      {
        FMPI2C_TransferConfig(hfmpi2c, DevAddress, MAX_NBYTE_SIZE, FMPI2C_RELOAD_MODE, FMPI2C_NO_STARTSTOP);
        hfmpi2c->XferSize = MAX_NBYTE_SIZE;
      }
      else
      {
        hfmpi2c->XferSize = hfmpi2c->XferCount;
        if(hfmpi2c->XferOptions != FMPI2C_NO_OPTION_FRAME)
        {
          FMPI2C_TransferConfig(hfmpi2c,DevAddress,hfmpi2c->XferSize, hfmpi2c->XferOptions, FMPI2C_NO_STARTSTOP);
        }
        else
        {
          FMPI2C_TransferConfig(hfmpi2c, DevAddress, hfmpi2c->XferSize, FMPI2C_AUTOEND_MODE, FMPI2C_NO_STARTSTOP);
        }
      }
    }
    else if((hfmpi2c->XferSize == 0U)&&(hfmpi2c->XferCount==0U))
    {
      /* Call TxCpltCallback() if no stop mode is set */
      if(FMPI2C_GET_STOP_MODE(hfmpi2c) != FMPI2C_AUTOEND_MODE)
      {
        if (hfmpi2c->State == HAL_FMPI2C_STATE_BUSY_TX)
        {
          /* Disable Interrupt */
          FMPI2C_Disable_IRQ(hfmpi2c, FMPI2C_IT_TX);
          hfmpi2c->PreviousState = hfmpi2c->State;
          hfmpi2c->State = HAL_FMPI2C_STATE_READY;

          /* Process Unlocked */
          __HAL_UNLOCK(hfmpi2c);
          
          /* Call the corresponding callback to inform upper layer of End of Transfer */
          if(hfmpi2c->Mode == HAL_FMPI2C_MODE_MEM)
          {
            HAL_FMPI2C_MemTxCpltCallback(hfmpi2c);
          }
          else if(hfmpi2c->Mode == HAL_FMPI2C_MODE_MASTER)
          {
            HAL_FMPI2C_MasterTxCpltCallback(hfmpi2c);
          }
        }
        else if(hfmpi2c->State == HAL_FMPI2C_STATE_BUSY_RX)
        {
          FMPI2C_Disable_IRQ(hfmpi2c, FMPI2C_IT_RX);
          hfmpi2c->PreviousState = hfmpi2c->State;
          hfmpi2c->State = HAL_FMPI2C_STATE_READY;

          /* Process Unlocked */
          __HAL_UNLOCK(hfmpi2c);

          /* Call the corresponding callback to inform upper layer of End of Transfer */
          if(hfmpi2c->Mode == HAL_FMPI2C_MODE_MEM)
          {
            HAL_FMPI2C_MemRxCpltCallback(hfmpi2c);
          }
          else if(hfmpi2c->Mode == HAL_FMPI2C_MODE_MASTER)
          {
            HAL_FMPI2C_MasterRxCpltCallback(hfmpi2c);
          }
        }
      }
    }
  }
  else if(__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_TC) != RESET)
  {
    if(hfmpi2c->XferCount == 0U)
    {
      if(FMPI2C_GET_STOP_MODE(hfmpi2c) != FMPI2C_AUTOEND_MODE)
      {
        /* No Generate Stop, to permit restart mode */
        /* The stop will be done at the end of transfer, when FMPI2C_AUTOEND_MODE enable */
        if (hfmpi2c->State == HAL_FMPI2C_STATE_BUSY_TX)
        {
          /* Disable Interrupt */
          FMPI2C_Disable_IRQ(hfmpi2c, FMPI2C_IT_TX);
          hfmpi2c->PreviousState = hfmpi2c->State;
          hfmpi2c->State = HAL_FMPI2C_STATE_READY;

          /* Process Unlocked */
          __HAL_UNLOCK(hfmpi2c);

          /* Call the corresponding callback to inform upper layer of End of Transfer */
          if(hfmpi2c->Mode == HAL_FMPI2C_MODE_MEM)
          {
            HAL_FMPI2C_MemTxCpltCallback(hfmpi2c);
          }
          else if(hfmpi2c->Mode == HAL_FMPI2C_MODE_MASTER)
          {
            HAL_FMPI2C_MasterTxCpltCallback(hfmpi2c);
          }
        }
        else if(hfmpi2c->State == HAL_FMPI2C_STATE_BUSY_RX)
        {
          FMPI2C_Disable_IRQ(hfmpi2c, FMPI2C_IT_RX);
          hfmpi2c->PreviousState = hfmpi2c->State;
          hfmpi2c->State = HAL_FMPI2C_STATE_READY;

          /* Process Unlocked */
          __HAL_UNLOCK(hfmpi2c);
          
          /* Call the corresponding callback to inform upper layer of End of Transfer */
          if(hfmpi2c->Mode == HAL_FMPI2C_MODE_MEM)
          {
            HAL_FMPI2C_MemRxCpltCallback(hfmpi2c);
          }
          else if(hfmpi2c->Mode == HAL_FMPI2C_MODE_MASTER)
          {
            HAL_FMPI2C_MasterRxCpltCallback(hfmpi2c);
          }
        }
      }
    }
  }

  /* Process Unlocked */
  __HAL_UNLOCK(hfmpi2c); 

  return HAL_OK; 
}

/**
  * @brief  Interrupt Sub-Routine which handle the Interrupt Flags Slave Mode.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @retval HAL status
  */
static HAL_StatusTypeDef FMPI2C_Slave_ISR(FMPI2C_HandleTypeDef *hfmpi2c) 
{
  uint8_t TransferDirection = 0U;
  uint16_t SlaveAddrCode = 0U;
  uint16_t OwnAdd1Code = 0U;
  uint16_t OwnAdd2Code = 0U;

  /* Process Locked */
  __HAL_LOCK(hfmpi2c);

  if(__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_AF) != RESET)
  {
    /* Check that FMPI2C transfer finished */
    /* if yes, normal usecase, a NACK is sent by the Master when Transfer is finished */
    /* Mean XferCount == 0*/
    /* So clear Flag NACKF only */
    if(hfmpi2c->XferCount == 0U)
    {  
       if(((hfmpi2c->XferOptions == FMPI2C_FIRST_AND_LAST_FRAME) || (hfmpi2c->XferOptions == FMPI2C_LAST_FRAME)) && \
        (hfmpi2c->State == HAL_FMPI2C_STATE_LISTEN))
      {
        hfmpi2c->XferOptions = FMPI2C_NO_OPTION_FRAME;
        /* Disable all Interrupts*/
        FMPI2C_Disable_IRQ(hfmpi2c, FMPI2C_IT_ADDR | FMPI2C_IT_RX | FMPI2C_IT_TX);

        /* Clear NACK Flag */
        __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_AF);

        hfmpi2c->PreviousState = hfmpi2c->State;
        hfmpi2c->State = HAL_FMPI2C_STATE_READY;

        /* Call the Listen Complete callback, to inform upper layer of the end of Listen usecase */
        HAL_FMPI2C_ListenCpltCallback(hfmpi2c);
      }
      else if((hfmpi2c->XferOptions != FMPI2C_NO_OPTION_FRAME) && (hfmpi2c->State == HAL_FMPI2C_STATE_BUSY_TX_LISTEN))
      {
        /* Last Byte is Transmitted */
        /* Remove HAL_FMPI2C_STATE_SLAVE_BUSY_TX, keep only HAL_FMPI2C_STATE_LISTEN */
        FMPI2C_Disable_IRQ(hfmpi2c, FMPI2C_IT_TX);
        hfmpi2c->PreviousState = hfmpi2c->State;
        hfmpi2c->State = HAL_FMPI2C_STATE_LISTEN;

        /* Clear NACK Flag */
        __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_AF);

        /* Check if TXIS flag is SET */
        if(__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_TXIS) != RESET)
        {
          /* Send a dummy data, to clear TXIS event */
          hfmpi2c->Instance->TXDR = 0x00U;

          /* Flush TX register if not empty */
          if(__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_TXE) == RESET)
          {
            __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_TXE);
          }
        }

        /* Process Unlocked */
        __HAL_UNLOCK(hfmpi2c);

        /* Call the Tx complete callback to inform upper layer of the end of transmit process */
        HAL_FMPI2C_SlaveTxCpltCallback(hfmpi2c);
      }
      else
      {
        /* Clear NACK Flag */
        __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_AF);

        /* Process Unlocked */
        __HAL_UNLOCK(hfmpi2c);
      }
    }
    else
    {
      /* if no, error usecase, a Non-Acknowledge of last Data is generated by the Master*/
      /* Clear NACK Flag */
      __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_AF);

      if(hfmpi2c->XferOptions != FMPI2C_NO_OPTION_FRAME)
      { 
        /* Set HAL State to "Idle" State, mean to LISTEN state */
        /* So reset Slave Busy state */
        hfmpi2c->PreviousState = hfmpi2c->State;
        hfmpi2c->State = HAL_FMPI2C_STATE_LISTEN;

        /* Disable RX/TX Interrupts, keep only ADDR Interrupt */
        FMPI2C_Disable_IRQ(hfmpi2c, FMPI2C_IT_RX | FMPI2C_IT_TX);
      }

      /* Set ErrorCode corresponding to a Non-Acknowledge */
      hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_AF;

      /* Process Unlocked */
      __HAL_UNLOCK(hfmpi2c);

      /* Call the Error callback to prevent upper layer */
      HAL_FMPI2C_ErrorCallback(hfmpi2c);
    }
  }

  else if(__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_ADDR) != RESET)
  {
    /* Disable ADDR interrupt to prevent multiple ADDRInterrupt*/
    /* Other ADDRInterrupt will be treat in next Listen usecase */
    if(hfmpi2c->XferOptions != FMPI2C_NO_OPTION_FRAME)
    {
      TransferDirection = FMPI2C_GET_DIR(hfmpi2c);
      SlaveAddrCode = FMPI2C_GET_ADDR_MATCH(hfmpi2c);
      OwnAdd1Code = FMPI2C_GET_OWN_ADDRESS1(hfmpi2c);
      OwnAdd2Code = FMPI2C_GET_OWN_ADDRESS2(hfmpi2c);

      /* If 10bits addressing mode is selected */
      if(hfmpi2c->Init.AddressingMode == FMPI2C_ADDRESSINGMODE_10BIT)
      {
        if((SlaveAddrCode & SlaveAddr_MSK) == ((OwnAdd1Code >> SlaveAddr_SHIFT) & SlaveAddr_MSK))
        {
          SlaveAddrCode = OwnAdd1Code;
          hfmpi2c->AddrEventCount++;
          if(hfmpi2c->AddrEventCount == 2U)
          {
            /* Reset Address Event counter  */
            hfmpi2c->AddrEventCount = 0U;

            /* Clear ADDR flag */
            __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c,FMPI2C_FLAG_ADDR);

            /* Process Unlocked */
            __HAL_UNLOCK(hfmpi2c);

            /* Call Slave Addr callback */
            HAL_FMPI2C_AddrCallback(hfmpi2c, TransferDirection, SlaveAddrCode);
          }
        }
        else
        {
          SlaveAddrCode = OwnAdd2Code;
          __HAL_FMPI2C_DISABLE_IT(hfmpi2c, FMPI2C_IT_ADDRI);

          /* Process Unlocked */
          __HAL_UNLOCK(hfmpi2c);

          /* Call Slave Addr callback */
          HAL_FMPI2C_AddrCallback(hfmpi2c, TransferDirection, SlaveAddrCode);
        }
      }

      /* If 7 bits addressing mode is selected */
      else if(hfmpi2c->Init.AddressingMode == FMPI2C_ADDRESSINGMODE_7BIT)
      {
        __HAL_FMPI2C_DISABLE_IT(hfmpi2c, FMPI2C_IT_ADDRI);

        /* Clear ADDR flag */
        __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c,FMPI2C_FLAG_ADDR);

        /* Process Unlocked */
        __HAL_UNLOCK(hfmpi2c);

        /* Call Slave Addr callback */
        HAL_FMPI2C_AddrCallback(hfmpi2c, TransferDirection, SlaveAddrCode);
      }
    }
    else
    {
      /* Clear ADDR flag */
      __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_ADDR);
      
      /* Process Unlocked */
      __HAL_UNLOCK(hfmpi2c);
    }
  }
  else if(__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_RXNE) != RESET)
  {
    if(hfmpi2c->XferOptions != FMPI2C_NO_OPTION_FRAME)
    { 
      if(hfmpi2c->State == HAL_FMPI2C_STATE_BUSY_RX_LISTEN)
      {
        /* Read data from RXDR */
        (*hfmpi2c->pBuffPtr++) = hfmpi2c->Instance->RXDR;
        hfmpi2c->XferSize--;
        hfmpi2c->XferCount--;

        if(hfmpi2c->XferCount == 0U)
        {
          /* Last Byte is received, disable Interrupt */
          FMPI2C_Disable_IRQ(hfmpi2c, FMPI2C_IT_RX);

          /* Remove HAL_FMPI2C_STATE_SLAVE_BUSY_RX, keep only HAL_FMPI2C_STATE_LISTEN */
          hfmpi2c->PreviousState = hfmpi2c->State;
          hfmpi2c->State = HAL_FMPI2C_STATE_LISTEN;
          
          /* Process Unlocked */
          __HAL_UNLOCK(hfmpi2c);

          /* Call the Rx complete callback to inform upper layer of the end of receive process */
          HAL_FMPI2C_SlaveRxCpltCallback(hfmpi2c);
        }
      }
    }
    else
    {
      /* Read data from RXDR */
      (*hfmpi2c->pBuffPtr++) = hfmpi2c->Instance->RXDR;
      hfmpi2c->XferSize--;
      hfmpi2c->XferCount--;
    }
  }
  else if(__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_TXIS) != RESET)
  {
    /* Write data to TXDR only if XferCount not reach "0" */
    /* A TXIS flag can be set, during STOP treatment      */
    /* Check if all Datas have already been sent */
    /* If it is the case, this last write in TXDR is not sent, correspond to a dummy TXIS event */
    if(hfmpi2c->XferCount > 0U)
    {
      /* Write data to TXDR */
      hfmpi2c->Instance->TXDR = (*hfmpi2c->pBuffPtr++);
      hfmpi2c->XferCount--;
      hfmpi2c->XferSize--;
    }
    else
    {
      if((hfmpi2c->XferOptions == FMPI2C_NEXT_FRAME) && (hfmpi2c->State == HAL_FMPI2C_STATE_BUSY_TX_LISTEN))
      {
        /* Last Byte is Transmitted */
        /* Remove HAL_FMPI2C_STATE_SLAVE_BUSY_TX, keep only HAL_FMPI2C_STATE_LISTEN */
        FMPI2C_Disable_IRQ(hfmpi2c, FMPI2C_IT_TX);
        hfmpi2c->PreviousState = FMPI2C_STATE_SLAVE_BUSY_TX;
        hfmpi2c->State = HAL_FMPI2C_STATE_LISTEN;

        /* Process Unlocked */
        __HAL_UNLOCK(hfmpi2c);

        /* Call the Tx complete callback to inform upper layer of the end of transmit process */
        HAL_FMPI2C_SlaveTxCpltCallback(hfmpi2c);
      }
    }
  }

  /* Check if STOPF is set */
  if(__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_STOPF) != RESET)
  {
    /* Disable ERRI, TCI, STOPI, NACKI, ADDRI, RXI, TXI interrupt */
    __HAL_FMPI2C_DISABLE_IT(hfmpi2c,FMPI2C_IT_ERRI | FMPI2C_IT_TCI| FMPI2C_IT_STOPI| FMPI2C_IT_NACKI | FMPI2C_IT_ADDRI | FMPI2C_IT_RXI | FMPI2C_IT_TXI);

    /* Disable Address Acknowledge */
    hfmpi2c->Instance->CR2 |= FMPI2C_CR2_NACK;

    /* Clear Configuration Register 2 */
    FMPI2C_RESET_CR2(hfmpi2c);

    /* Clear STOP Flag */
    __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_STOPF);

    /* Clear ADDR flag */
    __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c,FMPI2C_FLAG_ADDR);

    hfmpi2c->State = HAL_FMPI2C_STATE_READY;

    if(hfmpi2c->XferOptions != FMPI2C_NO_OPTION_FRAME)
    {
      hfmpi2c->XferOptions = 0U;
      hfmpi2c->PreviousState = hfmpi2c->State;

      /* Process Unlocked */
      __HAL_UNLOCK(hfmpi2c);

      /* Call the Listen Complete callback, to prevent upper layer of the end of Listen usecase */
      HAL_FMPI2C_ListenCpltCallback(hfmpi2c);
    }
    else
    {
      /* Process Unlocked */
      __HAL_UNLOCK(hfmpi2c);

      /* Call the Slave Tx Complete callback */
      HAL_FMPI2C_SlaveTxCpltCallback(hfmpi2c);
    }
  }

  /* Process Unlocked */
  __HAL_UNLOCK(hfmpi2c);

  return HAL_OK;
}

/**
  * @brief  Master sends target device address followed by internal memory address for write request.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  DevAddress Target device address
  * @param  MemAddress Internal memory address
  * @param  MemAddSize Size of internal memory address
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
static HAL_StatusTypeDef FMPI2C_RequestMemoryWrite(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint32_t Timeout)
{
  FMPI2C_TransferConfig(hfmpi2c,DevAddress,MemAddSize, FMPI2C_RELOAD_MODE, FMPI2C_GENERATE_START_WRITE);

  /* Wait until TXIS flag is set */
  if(FMPI2C_WaitOnTXISFlagUntilTimeout(hfmpi2c, Timeout) != HAL_OK)
  {
    if(hfmpi2c->ErrorCode == HAL_FMPI2C_ERROR_AF)
    {
      return HAL_ERROR;
    }
    else
    {
      return HAL_TIMEOUT;
    }
  }

  /* If Memory address size is 8Bit */
  if(MemAddSize == FMPI2C_MEMADD_SIZE_8BIT)
  {
    /* Send Memory Address */
    hfmpi2c->Instance->TXDR = FMPI2C_MEM_ADD_LSB(MemAddress);
  }
  /* If Memory address size is 16Bit */
  else
  {
    /* Send MSB of Memory Address */
    hfmpi2c->Instance->TXDR = FMPI2C_MEM_ADD_MSB(MemAddress);

    /* Wait until TXIS flag is set */
    if(FMPI2C_WaitOnTXISFlagUntilTimeout(hfmpi2c, Timeout) != HAL_OK)
    {
      if(hfmpi2c->ErrorCode == HAL_FMPI2C_ERROR_AF)
      {
        return HAL_ERROR;
      }
      else
      {
        return HAL_TIMEOUT;
      }
    }

    /* Send LSB of Memory Address */
    hfmpi2c->Instance->TXDR = FMPI2C_MEM_ADD_LSB(MemAddress);
  }

  /* Wait until TCR flag is set */
  if(FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_TCR, RESET, Timeout) != HAL_OK)
  {
    return HAL_TIMEOUT;
  }

return HAL_OK;
}

/**
  * @brief  Master sends target device address followed by internal memory address for read request.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  DevAddress Target device address
  * @param  MemAddress Internal memory address
  * @param  MemAddSize Size of internal memory address
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
static HAL_StatusTypeDef FMPI2C_RequestMemoryRead(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint32_t Timeout)
{
  FMPI2C_TransferConfig(hfmpi2c,DevAddress,MemAddSize, FMPI2C_SOFTEND_MODE, FMPI2C_GENERATE_START_WRITE);

  /* Wait until TXIS flag is set */
  if(FMPI2C_WaitOnTXISFlagUntilTimeout(hfmpi2c, Timeout) != HAL_OK)
  {
    if(hfmpi2c->ErrorCode == HAL_FMPI2C_ERROR_AF)
    {
      return HAL_ERROR;
    }
    else
    {
      return HAL_TIMEOUT;
    }
  }

  /* If Memory address size is 8Bit */
  if(MemAddSize == FMPI2C_MEMADD_SIZE_8BIT)
  {
    /* Send Memory Address */
    hfmpi2c->Instance->TXDR = FMPI2C_MEM_ADD_LSB(MemAddress);
  }
  /* If Memory address size is 16Bit */
  else
  {
    /* Send MSB of Memory Address */
    hfmpi2c->Instance->TXDR = FMPI2C_MEM_ADD_MSB(MemAddress);
    
    /* Wait until TXIS flag is set */
    if(FMPI2C_WaitOnTXISFlagUntilTimeout(hfmpi2c, Timeout) != HAL_OK)
    {
      if(hfmpi2c->ErrorCode == HAL_FMPI2C_ERROR_AF)
      {
        return HAL_ERROR;
      }
      else
      {
        return HAL_TIMEOUT;
      }
    }

    /* Send LSB of Memory Address */
    hfmpi2c->Instance->TXDR = FMPI2C_MEM_ADD_LSB(MemAddress);
  }

  /* Wait until TC flag is set */
  if(FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_TC, RESET, Timeout) != HAL_OK)
  {
    return HAL_TIMEOUT;
  }

  return HAL_OK;
}

/**
  * @brief  DMA FMPI2C master transmit process complete callback.
  * @param  hdma DMA handle
  * @retval None
  */
static void FMPI2C_DMAMasterTransmitCplt(DMA_HandleTypeDef *hdma)
{
  uint16_t DevAddress;
  FMPI2C_HandleTypeDef* hfmpi2c = (FMPI2C_HandleTypeDef*)((DMA_HandleTypeDef*)hdma)->Parent;
  
  /* Check if last DMA request was done with RELOAD */
  /* Set NBYTES to write and reload if size > MAX_NBYTE_SIZE */
  if( (hfmpi2c->XferSize == MAX_NBYTE_SIZE) && (hfmpi2c->XferSize < hfmpi2c->XferCount) )
  {
    /* Wait until TCR flag is set */
    if(FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_TCR, RESET, FMPI2C_TIMEOUT_TCR) != HAL_OK)
    {
      hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_TIMEOUT;
    }

    /* Disable DMA Request */
    hfmpi2c->Instance->CR1 &= ~FMPI2C_CR1_TXDMAEN; 

    /* Check if Errors has been detected during transfer */
    if(hfmpi2c->ErrorCode != HAL_FMPI2C_ERROR_NONE)
    {
      /* No need to Check TC flag, with AUTOEND mode the stop is automatically generated */
      /* Wait until STOPF flag is reset */ 
      if(FMPI2C_WaitOnSTOPFlagUntilTimeout(hfmpi2c, FMPI2C_TIMEOUT_STOPF) != HAL_OK)
      {
        if(hfmpi2c->ErrorCode == HAL_FMPI2C_ERROR_AF)
        {
          hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_AF;
        }
        else
        {
          hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_TIMEOUT;
        }
      }

      /* Clear STOP Flag */
      __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_STOPF);

      /* Clear Configuration Register 2 */
      FMPI2C_RESET_CR2(hfmpi2c);

      hfmpi2c->XferCount = 0U;

      hfmpi2c->State = HAL_FMPI2C_STATE_READY;
      hfmpi2c->Mode = HAL_FMPI2C_MODE_NONE;
      HAL_FMPI2C_ErrorCallback(hfmpi2c);
    }
    else
    {
      hfmpi2c->pBuffPtr += hfmpi2c->XferSize;
      hfmpi2c->XferCount -= hfmpi2c->XferSize;
      if(hfmpi2c->XferCount > MAX_NBYTE_SIZE)
      {
        hfmpi2c->XferSize = MAX_NBYTE_SIZE;
      }
      else
      {
        hfmpi2c->XferSize = hfmpi2c->XferCount;
      }

      DevAddress = (hfmpi2c->Instance->CR2 & FMPI2C_CR2_SADD);

      /* Enable the DMA channel */
      HAL_DMA_Start_IT(hfmpi2c->hdmatx, (uint32_t)hfmpi2c->pBuffPtr, (uint32_t)&hfmpi2c->Instance->TXDR, hfmpi2c->XferSize);
      
      /* Send Slave Address */
      /* Set NBYTES to write and reload if size > MAX_NBYTE_SIZE */
      if( (hfmpi2c->XferSize == MAX_NBYTE_SIZE) && (hfmpi2c->XferSize < hfmpi2c->XferCount) )
      {
        FMPI2C_TransferConfig(hfmpi2c,DevAddress,hfmpi2c->XferSize, FMPI2C_RELOAD_MODE, FMPI2C_NO_STARTSTOP);
      }
      else
      {
        FMPI2C_TransferConfig(hfmpi2c,DevAddress,hfmpi2c->XferSize, FMPI2C_AUTOEND_MODE, FMPI2C_NO_STARTSTOP);
      }  

      /* Wait until TXIS flag is set */
      if(FMPI2C_WaitOnTXISFlagUntilTimeout(hfmpi2c, FMPI2C_TIMEOUT_TXIS) != HAL_OK)
      {
        /* No need to Check TC flag, with AUTOEND mode the stop is automatically generated */
        /* Wait until STOPF flag is reset */ 
        if(FMPI2C_WaitOnSTOPFlagUntilTimeout(hfmpi2c, FMPI2C_TIMEOUT_STOPF) != HAL_OK)
        {
          if(hfmpi2c->ErrorCode == HAL_FMPI2C_ERROR_AF)
          {
            hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_AF;
          }
          else
          {
            hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_TIMEOUT;
          }
        }

        /* Clear STOP Flag */
        __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_STOPF);

        /* Clear Configuration Register 2 */
        FMPI2C_RESET_CR2(hfmpi2c);

        hfmpi2c->XferCount = 0U;
      
        hfmpi2c->State = HAL_FMPI2C_STATE_READY;
        HAL_FMPI2C_ErrorCallback(hfmpi2c);
      }
      else
      {
        /* Enable DMA Request */
        hfmpi2c->Instance->CR1 |= FMPI2C_CR1_TXDMAEN;
      }
    }
  }
  else
  {
    /* No need to Check TC flag, with AUTOEND mode the stop is automatically generated */
    /* Wait until STOPF flag is reset */ 
    if(FMPI2C_WaitOnSTOPFlagUntilTimeout(hfmpi2c, FMPI2C_TIMEOUT_STOPF) != HAL_OK)
    {
      if(hfmpi2c->ErrorCode == HAL_FMPI2C_ERROR_AF)
      {
        hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_AF;
      }
      else
      {
        hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_TIMEOUT;
      }
    }

    /* Clear STOP Flag */
    __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_STOPF);

    /* Clear Configuration Register 2 */
    FMPI2C_RESET_CR2(hfmpi2c);

    /* Disable DMA Request */
    hfmpi2c->Instance->CR1 &= ~FMPI2C_CR1_TXDMAEN; 

    hfmpi2c->XferCount = 0U;

    hfmpi2c->State = HAL_FMPI2C_STATE_READY;
    hfmpi2c->Mode = HAL_FMPI2C_MODE_NONE;

   /* Check if Errors has been detected during transfer */
    if(hfmpi2c->ErrorCode != HAL_FMPI2C_ERROR_NONE)
    {
      HAL_FMPI2C_ErrorCallback(hfmpi2c);
    }
    else
    {
      HAL_FMPI2C_MasterTxCpltCallback(hfmpi2c);
    }
  }
}

/**
  * @brief  DMA FMPI2C slave transmit process complete callback. 
  * @param  hdma DMA handle
  * @retval None
  */
static void FMPI2C_DMASlaveTransmitCplt(DMA_HandleTypeDef *hdma) 
{
  FMPI2C_HandleTypeDef* hfmpi2c = (FMPI2C_HandleTypeDef*)((DMA_HandleTypeDef*)hdma)->Parent;
  
  /* Wait until STOP flag is set */
  if(FMPI2C_WaitOnSTOPFlagUntilTimeout(hfmpi2c, FMPI2C_TIMEOUT_STOPF) != HAL_OK)
  {
    if(hfmpi2c->ErrorCode == HAL_FMPI2C_ERROR_AF)
    {
      /* Normal Use case, a AF is generated by master */
      /* to inform slave the end of transfer */
      hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_NONE;
    }
    else
    {
      hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_TIMEOUT;
    }
  }

  /* Clear STOP flag */
  __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c,FMPI2C_FLAG_STOPF);

  /* Wait until BUSY flag is reset */ 
  if(FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_BUSY, SET, FMPI2C_TIMEOUT_BUSY) != HAL_OK)
  {
    hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_TIMEOUT;
  }

  /* Disable DMA Request */
  hfmpi2c->Instance->CR1 &= ~FMPI2C_CR1_TXDMAEN;

  hfmpi2c->XferCount = 0U;

  hfmpi2c->State = HAL_FMPI2C_STATE_READY;
  hfmpi2c->Mode = HAL_FMPI2C_MODE_NONE;

  /* Check if Errors has been detected during transfer */
  if(hfmpi2c->ErrorCode != HAL_FMPI2C_ERROR_NONE)
  {
    HAL_FMPI2C_ErrorCallback(hfmpi2c);
  }
  else
  {
    HAL_FMPI2C_SlaveTxCpltCallback(hfmpi2c);
  }
}

/**
  * @brief DMA FMPI2C master receive process complete callback 
  * @param  hdma DMA handle
  * @retval None
  */
static void FMPI2C_DMAMasterReceiveCplt(DMA_HandleTypeDef *hdma) 
{
  FMPI2C_HandleTypeDef* hfmpi2c = (FMPI2C_HandleTypeDef*)((DMA_HandleTypeDef*)hdma)->Parent;
  uint16_t DevAddress;

  /* Check if last DMA request was done with RELOAD */
  /* Set NBYTES to write and reload if size > MAX_NBYTE_SIZE */
  if( (hfmpi2c->XferSize == MAX_NBYTE_SIZE) && (hfmpi2c->XferSize < hfmpi2c->XferCount) )
  {
    /* Wait until TCR flag is set */
    if(FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_TCR, RESET, FMPI2C_TIMEOUT_TCR) != HAL_OK)
    {
      hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_TIMEOUT;
    }

    /* Disable DMA Request */
    hfmpi2c->Instance->CR1 &= ~FMPI2C_CR1_RXDMAEN; 

    /* Check if Errors has been detected during transfer */
    if(hfmpi2c->ErrorCode != HAL_FMPI2C_ERROR_NONE)
    {
      /* No need to Check TC flag, with AUTOEND mode the stop is automatically generated */
      /* Wait until STOPF flag is reset */ 
      if(FMPI2C_WaitOnSTOPFlagUntilTimeout(hfmpi2c, FMPI2C_TIMEOUT_STOPF) != HAL_OK)
      {
        if(hfmpi2c->ErrorCode == HAL_FMPI2C_ERROR_AF)
        {
          hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_AF;
        }
        else
        {
          hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_TIMEOUT;
        }
      }

      /* Clear STOP Flag */
      __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_STOPF);
          
      /* Clear Configuration Register 2 */
      FMPI2C_RESET_CR2(hfmpi2c);

      hfmpi2c->XferCount = 0U;

      hfmpi2c->State = HAL_FMPI2C_STATE_READY;
      hfmpi2c->Mode = HAL_FMPI2C_MODE_NONE;
      HAL_FMPI2C_ErrorCallback(hfmpi2c);
    }
    else
    {
      hfmpi2c->pBuffPtr += hfmpi2c->XferSize;
      hfmpi2c->XferCount -= hfmpi2c->XferSize;
      if(hfmpi2c->XferCount > MAX_NBYTE_SIZE)
      {
        hfmpi2c->XferSize = MAX_NBYTE_SIZE;
      }
      else
      {
        hfmpi2c->XferSize = hfmpi2c->XferCount;
      }

      DevAddress = (hfmpi2c->Instance->CR2 & FMPI2C_CR2_SADD);

      /* Enable the DMA channel */
      HAL_DMA_Start_IT(hfmpi2c->hdmarx, (uint32_t)&hfmpi2c->Instance->RXDR, (uint32_t)hfmpi2c->pBuffPtr, hfmpi2c->XferSize);
      
      /* Send Slave Address */
      /* Set NBYTES to write and reload if size > MAX_NBYTE_SIZE */
      if( (hfmpi2c->XferSize == MAX_NBYTE_SIZE) && (hfmpi2c->XferSize < hfmpi2c->XferCount) )
      {
        FMPI2C_TransferConfig(hfmpi2c,DevAddress,hfmpi2c->XferSize, FMPI2C_RELOAD_MODE, FMPI2C_NO_STARTSTOP);
      }
      else
      {
        FMPI2C_TransferConfig(hfmpi2c,DevAddress,hfmpi2c->XferSize, FMPI2C_AUTOEND_MODE, FMPI2C_NO_STARTSTOP);
      }  

      /* Wait until RXNE flag is set */
      if(FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_RXNE, RESET, FMPI2C_TIMEOUT_RXNE) != HAL_OK)
      {
        hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_TIMEOUT;
      }

      /* Check if Errors has been detected during transfer */
      if(hfmpi2c->ErrorCode != HAL_FMPI2C_ERROR_NONE)
      {
        /* No need to Check TC flag, with AUTOEND mode the stop is automatically generated */
        /* Wait until STOPF flag is reset */ 
        if(FMPI2C_WaitOnSTOPFlagUntilTimeout(hfmpi2c, FMPI2C_TIMEOUT_STOPF) != HAL_OK)
        {
          if(hfmpi2c->ErrorCode == HAL_FMPI2C_ERROR_AF)
          {
            hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_AF;
          }
          else
          {
            hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_TIMEOUT;
          }
        }

        /* Clear STOP Flag */
        __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_STOPF);

        /* Clear Configuration Register 2 */
        FMPI2C_RESET_CR2(hfmpi2c);

        hfmpi2c->XferCount = 0U;

        hfmpi2c->State = HAL_FMPI2C_STATE_READY;
        hfmpi2c->Mode = HAL_FMPI2C_MODE_NONE;

        HAL_FMPI2C_ErrorCallback(hfmpi2c);
      }
      else
      {
        /* Enable DMA Request */
        hfmpi2c->Instance->CR1 |= FMPI2C_CR1_RXDMAEN;
      }
    }
  }
  else
  {
    /* No need to Check TC flag, with AUTOEND mode the stop is automatically generated */
    /* Wait until STOPF flag is reset */ 
    if(FMPI2C_WaitOnSTOPFlagUntilTimeout(hfmpi2c, FMPI2C_TIMEOUT_STOPF) != HAL_OK)
    {
      if(hfmpi2c->ErrorCode == HAL_FMPI2C_ERROR_AF)
      {
        hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_AF;
      }
      else
      {
        hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_TIMEOUT;
      }
    }

    /* Clear STOP Flag */
    __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_STOPF);

    /* Clear Configuration Register 2 */
    FMPI2C_RESET_CR2(hfmpi2c);

    /* Disable DMA Request */
    hfmpi2c->Instance->CR1 &= ~FMPI2C_CR1_RXDMAEN;

    hfmpi2c->XferCount = 0U;

    hfmpi2c->State = HAL_FMPI2C_STATE_READY;
    hfmpi2c->Mode = HAL_FMPI2C_MODE_NONE;

    /* Check if Errors has been detected during transfer */
    if(hfmpi2c->ErrorCode != HAL_FMPI2C_ERROR_NONE)
    {
      HAL_FMPI2C_ErrorCallback(hfmpi2c);
    }
    else
    {
      HAL_FMPI2C_MasterRxCpltCallback(hfmpi2c);
    }
  }
}

/**
  * @brief  DMA FMPI2C slave receive process complete callback.
  * @param  hdma DMA handle
  * @retval None
  */
static void FMPI2C_DMASlaveReceiveCplt(DMA_HandleTypeDef *hdma) 
{
  FMPI2C_HandleTypeDef* hfmpi2c = (FMPI2C_HandleTypeDef*)((DMA_HandleTypeDef*)hdma)->Parent;

  /* Wait until STOPF flag is reset */ 
  if(FMPI2C_WaitOnSTOPFlagUntilTimeout(hfmpi2c, FMPI2C_TIMEOUT_STOPF) != HAL_OK)
  {
    if(hfmpi2c->ErrorCode == HAL_FMPI2C_ERROR_AF)
    {
      hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_AF;
    }
    else
    {
      hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_TIMEOUT;
    }
  }

  /* Clear STOPF flag */
  __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_STOPF);

  /* Wait until BUSY flag is reset */ 
  if(FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_BUSY, SET, FMPI2C_TIMEOUT_BUSY) != HAL_OK)

  {
    hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_TIMEOUT;
  }
  
  /* Disable DMA Request */
  hfmpi2c->Instance->CR1 &= ~FMPI2C_CR1_RXDMAEN; 

  /* Disable Address Acknowledge */
  hfmpi2c->Instance->CR2 |= FMPI2C_CR2_NACK;

  hfmpi2c->XferCount = 0U;
  
  hfmpi2c->State = HAL_FMPI2C_STATE_READY;
  hfmpi2c->Mode = HAL_FMPI2C_MODE_NONE;

  /* Check if Errors has been detected during transfer */
  if(hfmpi2c->ErrorCode != HAL_FMPI2C_ERROR_NONE)
  {
    HAL_FMPI2C_ErrorCallback(hfmpi2c);
  }
  else
  {
    HAL_FMPI2C_SlaveRxCpltCallback(hfmpi2c);
  }
}

/**
  * @brief DMA FMPI2C Memory Write process complete callback 
  * @param hdma DMA handle
  * @retval None
  */
static void FMPI2C_DMAMemTransmitCplt(DMA_HandleTypeDef *hdma)
{
  uint16_t DevAddress;
  FMPI2C_HandleTypeDef* hfmpi2c = ( FMPI2C_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;

  /* Check if last DMA request was done with RELOAD */
  /* Set NBYTES to write and reload if size > MAX_NBYTE_SIZE */
  if( (hfmpi2c->XferSize == MAX_NBYTE_SIZE) && (hfmpi2c->XferSize < hfmpi2c->XferCount) )
  {
    /* Wait until TCR flag is set */
    if(FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_TCR, RESET, FMPI2C_TIMEOUT_TCR) != HAL_OK)
    {
      hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_TIMEOUT;
    }

    /* Disable DMA Request */
    hfmpi2c->Instance->CR1 &= ~FMPI2C_CR1_TXDMAEN;
    
    /* Check if Errors has been detected during transfer */
    if(hfmpi2c->ErrorCode != HAL_FMPI2C_ERROR_NONE)
    {
      /* No need to Check TC flag, with AUTOEND mode the stop is automatically generated */
      /* Wait until STOPF flag is reset */ 
      if(FMPI2C_WaitOnSTOPFlagUntilTimeout(hfmpi2c, FMPI2C_TIMEOUT_STOPF) != HAL_OK)
      {
        if(hfmpi2c->ErrorCode == HAL_FMPI2C_ERROR_AF)
        {
          hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_AF;
        }
        else
        {
          hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_TIMEOUT;
        }
      }

      /* Clear STOP Flag */
      __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_STOPF);

      /* Clear Configuration Register 2 */
      FMPI2C_RESET_CR2(hfmpi2c);

      hfmpi2c->XferCount = 0U;

      hfmpi2c->State = HAL_FMPI2C_STATE_READY;
      hfmpi2c->Mode = HAL_FMPI2C_MODE_NONE;
      HAL_FMPI2C_ErrorCallback(hfmpi2c);
    }
    else
    {
      hfmpi2c->pBuffPtr += hfmpi2c->XferSize;
      hfmpi2c->XferCount -= hfmpi2c->XferSize;
      if(hfmpi2c->XferCount > MAX_NBYTE_SIZE)
      {
        hfmpi2c->XferSize = MAX_NBYTE_SIZE;
      }
      else
      {
        hfmpi2c->XferSize = hfmpi2c->XferCount;
      }

      DevAddress = (hfmpi2c->Instance->CR2 & FMPI2C_CR2_SADD);

      /* Enable the DMA channel */
      HAL_DMA_Start_IT(hfmpi2c->hdmatx, (uint32_t)hfmpi2c->pBuffPtr, (uint32_t)&hfmpi2c->Instance->TXDR, hfmpi2c->XferSize);
      
      /* Send Slave Address */
      /* Set NBYTES to write and reload if size > MAX_NBYTE_SIZE */
      if( (hfmpi2c->XferSize == MAX_NBYTE_SIZE) && (hfmpi2c->XferSize < hfmpi2c->XferCount) )
      {
        FMPI2C_TransferConfig(hfmpi2c,DevAddress,hfmpi2c->XferSize, FMPI2C_RELOAD_MODE, FMPI2C_NO_STARTSTOP);
      }
      else
      {
        FMPI2C_TransferConfig(hfmpi2c,DevAddress,hfmpi2c->XferSize, FMPI2C_AUTOEND_MODE, FMPI2C_NO_STARTSTOP);
      }

      /* Wait until TXIS flag is set */
      if(FMPI2C_WaitOnTXISFlagUntilTimeout(hfmpi2c, FMPI2C_TIMEOUT_TXIS) != HAL_OK)
      {
        /* No need to Check TC flag, with AUTOEND mode the stop is automatically generated */
        /* Wait until STOPF flag is reset */ 
        if(FMPI2C_WaitOnSTOPFlagUntilTimeout(hfmpi2c, FMPI2C_TIMEOUT_STOPF) != HAL_OK)
        {
          if(hfmpi2c->ErrorCode == HAL_FMPI2C_ERROR_AF)
          {
            hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_AF;
          }
          else
          {
            hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_TIMEOUT;
          }
        }

        /* Clear STOP Flag */
        __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_STOPF);

        /* Clear Configuration Register 2 */
        FMPI2C_RESET_CR2(hfmpi2c);

        hfmpi2c->XferCount = 0U;

        hfmpi2c->State = HAL_FMPI2C_STATE_READY;
        hfmpi2c->Mode = HAL_FMPI2C_MODE_NONE;
        HAL_FMPI2C_ErrorCallback(hfmpi2c);
      }
      else
      {
        /* Enable DMA Request */
        hfmpi2c->Instance->CR1 |= FMPI2C_CR1_TXDMAEN;
      }
    }
  }
  else
  {
    /* No need to Check TC flag, with AUTOEND mode the stop is automatically generated */
    /* Wait until STOPF flag is reset */ 
    if(FMPI2C_WaitOnSTOPFlagUntilTimeout(hfmpi2c, FMPI2C_TIMEOUT_STOPF) != HAL_OK)
    {
      if(hfmpi2c->ErrorCode == HAL_FMPI2C_ERROR_AF)
      {
        hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_AF;
      }
      else
      {
        hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_TIMEOUT;
      }
    }

    /* Clear STOP Flag */
    __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_STOPF);

    /* Clear Configuration Register 2 */
    FMPI2C_RESET_CR2(hfmpi2c);

    /* Disable DMA Request */
    hfmpi2c->Instance->CR1 &= ~FMPI2C_CR1_TXDMAEN;
  
    hfmpi2c->XferCount = 0U;
  
    hfmpi2c->State = HAL_FMPI2C_STATE_READY;
    hfmpi2c->Mode = HAL_FMPI2C_MODE_NONE;

    /* Check if Errors has been detected during transfer */
    if(hfmpi2c->ErrorCode != HAL_FMPI2C_ERROR_NONE)
    {
      HAL_FMPI2C_ErrorCallback(hfmpi2c);
    }
    else
    {
      HAL_FMPI2C_MemTxCpltCallback(hfmpi2c);
    }
  }
}

/**
  * @brief  DMA FMPI2C Memory Read process complete callback
  * @param  hdma DMA handle
  * @retval None
  */
static void FMPI2C_DMAMemReceiveCplt(DMA_HandleTypeDef *hdma)
{
  FMPI2C_HandleTypeDef* hfmpi2c = ( FMPI2C_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
  uint16_t DevAddress;

  /* Check if last DMA request was done with RELOAD */
  /* Set NBYTES to write and reload if size > MAX_NBYTE_SIZE */
  if( (hfmpi2c->XferSize == MAX_NBYTE_SIZE) && (hfmpi2c->XferSize < hfmpi2c->XferCount) )
  {
    /* Wait until TCR flag is set */
    if(FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_TCR, RESET, FMPI2C_TIMEOUT_TCR) != HAL_OK)
    {
      hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_TIMEOUT;
    }

    /* Disable DMA Request */
    hfmpi2c->Instance->CR1 &= ~FMPI2C_CR1_RXDMAEN; 

    /* Check if Errors has been detected during transfer */
    if(hfmpi2c->ErrorCode != HAL_FMPI2C_ERROR_NONE)
    {
      /* No need to Check TC flag, with AUTOEND mode the stop is automatically generated */
      /* Wait until STOPF flag is reset */ 
      if(FMPI2C_WaitOnSTOPFlagUntilTimeout(hfmpi2c, FMPI2C_TIMEOUT_STOPF) != HAL_OK)
      {
        if(hfmpi2c->ErrorCode == HAL_FMPI2C_ERROR_AF)
        {
          hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_AF;
        }
        else
        {
          hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_TIMEOUT;
        }
      }

      /* Clear STOP Flag */
      __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_STOPF);
          
      /* Clear Configuration Register 2 */
      FMPI2C_RESET_CR2(hfmpi2c);

      hfmpi2c->XferCount = 0U;

      hfmpi2c->State = HAL_FMPI2C_STATE_READY;
      hfmpi2c->Mode = HAL_FMPI2C_MODE_NONE;
      HAL_FMPI2C_ErrorCallback(hfmpi2c);
    }
    else
    {
      hfmpi2c->pBuffPtr += hfmpi2c->XferSize;
      hfmpi2c->XferCount -= hfmpi2c->XferSize;
      if(hfmpi2c->XferCount > MAX_NBYTE_SIZE)
      {
        hfmpi2c->XferSize = MAX_NBYTE_SIZE;
      }
      else
      {
        hfmpi2c->XferSize = hfmpi2c->XferCount;
      }

      DevAddress = (hfmpi2c->Instance->CR2 & FMPI2C_CR2_SADD);

      /* Enable the DMA channel */
      HAL_DMA_Start_IT(hfmpi2c->hdmarx, (uint32_t)&hfmpi2c->Instance->RXDR, (uint32_t)hfmpi2c->pBuffPtr, hfmpi2c->XferSize);
      
      /* Send Slave Address */
      /* Set NBYTES to write and reload if size > MAX_NBYTE_SIZE */
      if( (hfmpi2c->XferSize == MAX_NBYTE_SIZE) && (hfmpi2c->XferSize < hfmpi2c->XferCount) )
      {
        FMPI2C_TransferConfig(hfmpi2c,DevAddress,hfmpi2c->XferSize, FMPI2C_RELOAD_MODE, FMPI2C_NO_STARTSTOP);
      }
      else
      {
        FMPI2C_TransferConfig(hfmpi2c,DevAddress,hfmpi2c->XferSize, FMPI2C_AUTOEND_MODE, FMPI2C_NO_STARTSTOP);
      }

      /* Wait until RXNE flag is set */
      if(FMPI2C_WaitOnFlagUntilTimeout(hfmpi2c, FMPI2C_FLAG_RXNE, RESET, FMPI2C_TIMEOUT_RXNE) != HAL_OK)
      {
        hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_TIMEOUT;
      }

      /* Check if Errors has been detected during transfer */
      if(hfmpi2c->ErrorCode != HAL_FMPI2C_ERROR_NONE)
      {
        /* No need to Check TC flag, with AUTOEND mode the stop is automatically generated */
        /* Wait until STOPF flag is reset */ 
        if(FMPI2C_WaitOnSTOPFlagUntilTimeout(hfmpi2c, FMPI2C_TIMEOUT_STOPF) != HAL_OK)
        {
          if(hfmpi2c->ErrorCode == HAL_FMPI2C_ERROR_AF)
          {
            hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_AF;
          }
          else
          {
            hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_TIMEOUT;
          }
        }

        /* Clear STOP Flag */
        __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_STOPF);
            
        /* Clear Configuration Register 2 */
        FMPI2C_RESET_CR2(hfmpi2c);

        hfmpi2c->XferCount = 0U;

        hfmpi2c->State = HAL_FMPI2C_STATE_READY;
        hfmpi2c->Mode = HAL_FMPI2C_MODE_NONE;
        HAL_FMPI2C_ErrorCallback(hfmpi2c);
      }
      else
      {
        /* Enable DMA Request */
        hfmpi2c->Instance->CR1 |= FMPI2C_CR1_RXDMAEN;
      }
    }
  }
  else
  {
    /* No need to Check TC flag, with AUTOEND mode the stop is automatically generated */
    /* Wait until STOPF flag is reset */ 
    if(FMPI2C_WaitOnSTOPFlagUntilTimeout(hfmpi2c, FMPI2C_TIMEOUT_STOPF) != HAL_OK)
    {
      if(hfmpi2c->ErrorCode == HAL_FMPI2C_ERROR_AF)
      {
        hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_AF;
      }
      else
      {
        hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_TIMEOUT;
      }
    }

    /* Clear STOP Flag */
    __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_STOPF);

    /* Clear Configuration Register 2 */
    FMPI2C_RESET_CR2(hfmpi2c);

    /* Disable DMA Request */
    hfmpi2c->Instance->CR1 &= ~FMPI2C_CR1_RXDMAEN;

    hfmpi2c->XferCount = 0U;

    hfmpi2c->State = HAL_FMPI2C_STATE_READY;
    hfmpi2c->Mode = HAL_FMPI2C_MODE_NONE;

    /* Check if Errors has been detected during transfer */
    if(hfmpi2c->ErrorCode != HAL_FMPI2C_ERROR_NONE)
    {
      HAL_FMPI2C_ErrorCallback(hfmpi2c);
    }
    else
    {
      HAL_FMPI2C_MemRxCpltCallback(hfmpi2c);
    }
  }
}

/**
  * @brief  DMA FMPI2C communication error callback. 
  * @param hdma DMA handle
  * @retval None
  */
static void FMPI2C_DMAError(DMA_HandleTypeDef *hdma)
{
  FMPI2C_HandleTypeDef* hfmpi2c = ( FMPI2C_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;

  /* Disable Acknowledge */
  hfmpi2c->Instance->CR2 |= FMPI2C_CR2_NACK;

  hfmpi2c->XferCount = 0U;

  hfmpi2c->State = HAL_FMPI2C_STATE_READY;
  hfmpi2c->Mode = HAL_FMPI2C_MODE_NONE;

  hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_DMA;

  HAL_FMPI2C_ErrorCallback(hfmpi2c);
}

/**
  * @brief  This function handles FMPI2C Communication Timeout.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  Flag Specifies the FMPI2C flag to check.
  * @param  Status The new Flag status (SET or RESET).
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
static HAL_StatusTypeDef FMPI2C_WaitOnFlagUntilTimeout(FMPI2C_HandleTypeDef *hfmpi2c, uint32_t Flag, FlagStatus Status, uint32_t Timeout)
{
  uint32_t tickstart = HAL_GetTick();

  /* Wait until flag is set */
  if(Status == RESET)
  {
    while(__HAL_FMPI2C_GET_FLAG(hfmpi2c, Flag) == RESET)
    {
      /* Check for the Timeout */
      if(Timeout != HAL_MAX_DELAY)
      {
        if((Timeout == 0U) || ((HAL_GetTick()-tickstart) > Timeout))
        {
          hfmpi2c->State= HAL_FMPI2C_STATE_READY;
          /* Process Unlocked */
          __HAL_UNLOCK(hfmpi2c);
          return HAL_TIMEOUT;
        }
      }
    }
  }
  else
  {
    while(__HAL_FMPI2C_GET_FLAG(hfmpi2c, Flag) != RESET)
    {
      /* Check for the Timeout */
      if(Timeout != HAL_MAX_DELAY)
      {
        if((Timeout == 0U) || ((HAL_GetTick()-tickstart) > Timeout))
        {
          hfmpi2c->State= HAL_FMPI2C_STATE_READY;
          /* Process Unlocked */
          __HAL_UNLOCK(hfmpi2c);
          return HAL_TIMEOUT;
        }
      }
    }
  }
  return HAL_OK;
}

/**
  * @brief  This function handles FMPI2C Communication Timeout for specific usage of TXIS flag.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
static HAL_StatusTypeDef FMPI2C_WaitOnTXISFlagUntilTimeout(FMPI2C_HandleTypeDef *hfmpi2c, uint32_t Timeout)
{
  uint32_t tickstart = HAL_GetTick();

  while(__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_TXIS) == RESET)
  {
    /* Check if a NACK is detected */
    if(FMPI2C_IsAcknowledgeFailed(hfmpi2c, Timeout) != HAL_OK)
    {
      return HAL_ERROR;
    }

    /* Check for the Timeout */
    if(Timeout != HAL_MAX_DELAY)
    {
      if((Timeout == 0U) || ((HAL_GetTick()-tickstart) > Timeout))
      {
        hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_TIMEOUT;
        hfmpi2c->State= HAL_FMPI2C_STATE_READY;

        /* Process Unlocked */
        __HAL_UNLOCK(hfmpi2c);

        return HAL_TIMEOUT;
      }
    }
  }
  return HAL_OK;
}

/**
  * @brief  This function handles FMPI2C Communication Timeout for specific usage of STOP flag.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
static HAL_StatusTypeDef FMPI2C_WaitOnSTOPFlagUntilTimeout(FMPI2C_HandleTypeDef *hfmpi2c, uint32_t Timeout)
{
  uint32_t tickstart = 0x00U;
  tickstart = HAL_GetTick();

  while(__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_STOPF) == RESET)
  {
    /* Check if a NACK is detected */
    if(FMPI2C_IsAcknowledgeFailed(hfmpi2c, Timeout) != HAL_OK)
    {
      return HAL_ERROR;
    }

    /* Check for the Timeout */
    if((Timeout == 0U) || ((HAL_GetTick()-tickstart) > Timeout))
    {
      hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_TIMEOUT;
      hfmpi2c->State= HAL_FMPI2C_STATE_READY;

      /* Process Unlocked */
      __HAL_UNLOCK(hfmpi2c);

      return HAL_TIMEOUT;
    }
  }
  return HAL_OK;
}

/**
  * @brief  This function handles FMPI2C Communication Timeout for specific usage of RXNE flag.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
static HAL_StatusTypeDef FMPI2C_WaitOnRXNEFlagUntilTimeout(FMPI2C_HandleTypeDef *hfmpi2c, uint32_t Timeout)
{
  uint32_t tickstart = 0x00U;
  tickstart = HAL_GetTick();

  while(__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_RXNE) == RESET)
  {
    /* Check if a STOPF is detected */
    if(__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_STOPF) == SET)
    {
      /* Clear STOP Flag */
      __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_STOPF);

      /* Clear Configuration Register 2 */
      FMPI2C_RESET_CR2(hfmpi2c);

      hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_NONE;
      hfmpi2c->State= HAL_FMPI2C_STATE_READY;

      /* Process Unlocked */
      __HAL_UNLOCK(hfmpi2c);

      return HAL_ERROR;
    }

    /* Check for the Timeout */
    if((Timeout == 0U) || ((HAL_GetTick()-tickstart) > Timeout))
    {
      hfmpi2c->ErrorCode |= HAL_FMPI2C_ERROR_TIMEOUT;
      hfmpi2c->State= HAL_FMPI2C_STATE_READY;

      /* Process Unlocked */
      __HAL_UNLOCK(hfmpi2c);

      return HAL_TIMEOUT;
    }
  }
  return HAL_OK;
}

/**
  * @brief  This function handles Acknowledge failed detection during an FMPI2C Communication.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
static HAL_StatusTypeDef FMPI2C_IsAcknowledgeFailed(FMPI2C_HandleTypeDef *hfmpi2c, uint32_t Timeout)
{
  uint32_t tickstart = 0x00U;
  tickstart = HAL_GetTick();

  if(__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_AF) == SET)
  {
    /* Generate stop if necessary only in case of FMPI2C peripheral in MASTER mode or MEMORY mode */
     if((hfmpi2c->Mode == HAL_FMPI2C_MODE_MASTER) || (hfmpi2c->Mode == HAL_FMPI2C_MODE_MEM))
    {
      /* No need to generate the STOP condition if AUTOEND mode is enabled */
      /* Generate the STOP condition only in case of SOFTEND mode is enabled */
      if((hfmpi2c->Instance->CR2 & FMPI2C_AUTOEND_MODE) != FMPI2C_AUTOEND_MODE)
      {
        /* Generate Stop */
        hfmpi2c->Instance->CR2 |= FMPI2C_CR2_STOP;
      }
    }

    /* Wait until STOP Flag is reset */
    /* AutoEnd should be initiate after AF */
    while(__HAL_FMPI2C_GET_FLAG(hfmpi2c, FMPI2C_FLAG_STOPF) == RESET)
    {
      /* Check for the Timeout */
      if(Timeout != HAL_MAX_DELAY)
      {
        if((Timeout == 0U) || ((HAL_GetTick()-tickstart) > Timeout))
        {
          hfmpi2c->State= HAL_FMPI2C_STATE_READY;
          /* Process Unlocked */
          __HAL_UNLOCK(hfmpi2c);
          return HAL_TIMEOUT;
        }
      }
    }

    /* Clear NACKF Flag */
    __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_AF);

    /* Clear STOP Flag */
    __HAL_FMPI2C_CLEAR_FLAG(hfmpi2c, FMPI2C_FLAG_STOPF);

    /* Clear Configuration Register 2 */
    FMPI2C_RESET_CR2(hfmpi2c);

    hfmpi2c->ErrorCode = HAL_FMPI2C_ERROR_AF;
    hfmpi2c->State= HAL_FMPI2C_STATE_READY;

    /* Process Unlocked */
    __HAL_UNLOCK(hfmpi2c);

    return HAL_ERROR;
  }
  return HAL_OK;
}

/**
  * @brief  Handles FMPI2Cx communication when starting transfer or during transfer (TC or TCR flag are set).
  * @param  hfmpi2c FMPI2C handle.
  * @param  DevAddress Specifies the slave address to be programmed.
  * @param  Size Specifies the number of bytes to be programmed.
  *   This parameter must be a value between 0 and 255.
  * @param  Mode New state of the FMPI2C START condition generation.
  *   This parameter can be one of the following values:
  *     @arg @ref FMPI2C_RELOAD_MODE Enable Reload mode .
  *     @arg @ref FMPI2C_AUTOEND_MODE Enable Automatic end mode.
  *     @arg @ref FMPI2C_SOFTEND_MODE Enable Software end mode.
  * @param  Request New state of the FMPI2C START condition generation.
  *   This parameter can be one of the following values:
  *     @arg @ref FMPI2C_NO_STARTSTOP Don't Generate stop and start condition.
  *     @arg @ref FMPI2C_GENERATE_STOP Generate stop condition (Size should be set to 0).
  *     @arg @ref FMPI2C_GENERATE_START_READ Generate Restart for read request.
  *     @arg @ref FMPI2C_GENERATE_START_WRITE Generate Restart for write request.
  * @retval None
  */
static void FMPI2C_TransferConfig(FMPI2C_HandleTypeDef *hfmpi2c,  uint16_t DevAddress, uint8_t Size, uint32_t Mode, uint32_t Request)
{
  uint32_t tmpreg = 0U;

  /* Check the parameters */
  assert_param(IS_FMPI2C_ALL_INSTANCE(hfmpi2c->Instance));
  assert_param(IS_TRANSFER_MODE(Mode));
  assert_param(IS_TRANSFER_REQUEST(Request));

  /* Get the CR2 register value */
  tmpreg = hfmpi2c->Instance->CR2;
  
  /* clear tmpreg specific bits */
  tmpreg &= (uint32_t)~((uint32_t)(FMPI2C_CR2_SADD | FMPI2C_CR2_NBYTES | FMPI2C_CR2_RELOAD | FMPI2C_CR2_AUTOEND | FMPI2C_CR2_RD_WRN | FMPI2C_CR2_START | FMPI2C_CR2_STOP));

  /* update tmpreg */
  tmpreg |= (uint32_t)(((uint32_t)DevAddress & FMPI2C_CR2_SADD) | (((uint32_t)Size << 16U) & FMPI2C_CR2_NBYTES) | \
            (uint32_t)Mode | (uint32_t)Request);
  
  /* update CR2 register */
  hfmpi2c->Instance->CR2 = tmpreg;
}

/**
  * @brief  Manage the enabling of Interrupts.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  InterruptRequest Value of @ref FMPI2C_Interrupt_configuration_definition.
  * @retval HAL status
  */
static HAL_StatusTypeDef FMPI2C_Enable_IRQ(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t InterruptRequest)
{
  uint32_t tmpisr = 0U;
  
  if((InterruptRequest & FMPI2C_IT_ADDR) == FMPI2C_IT_ADDR)
  {
    /* Enable ADDR and STOP interrupt */
    tmpisr |= FMPI2C_IT_ADDRI | FMPI2C_IT_STOPI | FMPI2C_IT_NACKI | FMPI2C_IT_ERRI;
  }

  if((InterruptRequest & FMPI2C_IT_TX) == FMPI2C_IT_TX)
  {
    /* Enable ERR, TC, STOP, NACK and RXI interrupt */
    tmpisr |= FMPI2C_IT_ERRI | FMPI2C_IT_TCI | FMPI2C_IT_STOPI | FMPI2C_IT_NACKI | FMPI2C_IT_TXI;
  }

  if((InterruptRequest & FMPI2C_IT_RX) == FMPI2C_IT_RX)
  {
    /* Enable ERR, TC, STOP, NACK and TXI interrupt */
    tmpisr |= FMPI2C_IT_ERRI | FMPI2C_IT_TCI | FMPI2C_IT_STOPI | FMPI2C_IT_NACKI | FMPI2C_IT_RXI;
  }

  /* Enable interrupts only at the end */
  /* to avoid the risk of FMPI2C interrupt handle execution before */
  /* all interrupts requested done */
  __HAL_FMPI2C_ENABLE_IT(hfmpi2c, tmpisr);

  return HAL_OK;
}

/**
  * @brief  Manage the disabling of Interrupts.
  * @param  hfmpi2c Pointer to a FMPI2C_HandleTypeDef structure that contains
  *                the configuration information for the specified FMPI2C.
  * @param  InterruptRequest Value of @ref FMPI2C_Interrupt_configuration_definition.
  * @retval HAL status
  */
static HAL_StatusTypeDef FMPI2C_Disable_IRQ(FMPI2C_HandleTypeDef *hfmpi2c, uint16_t InterruptRequest)
{
  uint32_t tmpisr = 0U;

  if((InterruptRequest & FMPI2C_IT_TX) == FMPI2C_IT_TX)
  {
    /* Disable TC and TXI interrupts */
    tmpisr |= FMPI2C_IT_TCI | FMPI2C_IT_TXI;
    
    if((hfmpi2c->State & HAL_FMPI2C_STATE_LISTEN) != HAL_FMPI2C_STATE_LISTEN)
    {
      /* Disable NACK and STOP interrupts */
      tmpisr |= FMPI2C_IT_STOPI | FMPI2C_IT_NACKI;
    }
  }

  if((InterruptRequest & FMPI2C_IT_RX) == FMPI2C_IT_RX)
  {
    /* Disable TC and RXI interrupts */
    tmpisr |= FMPI2C_IT_TCI | FMPI2C_IT_RXI;

    if((hfmpi2c->State & HAL_FMPI2C_STATE_LISTEN) != HAL_FMPI2C_STATE_LISTEN)
    {
      /* Disable NACK and STOP interrupts */
      tmpisr |= FMPI2C_IT_STOPI | FMPI2C_IT_NACKI;
    }
  }

  if((InterruptRequest & FMPI2C_IT_ADDR) == FMPI2C_IT_ADDR)
  {
    /* Disable ADDR, NACK and STOP interrupts */
    tmpisr |= FMPI2C_IT_ADDRI | FMPI2C_IT_STOPI | FMPI2C_IT_NACKI;
  }

  /* Disable interrupts only at the end */
  /* to avoid a breaking situation like at "t" time */
  /* all disable interrupts request are not done */
  __HAL_FMPI2C_DISABLE_IT(hfmpi2c, tmpisr);
  
  return HAL_OK;
}

/**
  * @}
  */
#endif /* STM32F410xx || STM32F446xx */
#endif /* HAL_FMPI2C_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
