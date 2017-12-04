/*! *****************************************************************************
 * @file:     adi_uart.c
 * @brief:    uart device driver implementation
 * @details:  This file contains the UART device driver functions
 -----------------------------------------------------------------------------
Copyright (c) 2010-2016 Analog Devices, Inc.

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
  - Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
  - Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
  - Modified versions of the software must be conspicuously marked as such.
  - This software is licensed solely and exclusively for use with processors
    manufactured by or for Analog Devices, Inc.
  - This software may not be combined or merged with other code in any manner
    that would cause the software to become subject to terms and conditions
    which differ from those listed here.
  - Neither the name of Analog Devices, Inc. nor the names of its
    contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.
  - The use of this software may or may not infringe the patent rights of one
    or more patent holders.  This license does not release you from the
    requirement that you obtain separate licenses from these patent holders
    to use this software.

THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-
INFRINGEMENT, TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF
CLAIMS OF INTELLECTUAL PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

*****************************************************************************/

/** @addtogroup UART_Driver
 *  @{
 *  @brief UART Driver
 *  @note The application must include drivers/uart/adi_uart.h to use this
 *  driver
 *  @note This driver requires the DMA driver.The application must
 *  include the DMA driver sources to avoid link errors.
 */

/*!  \cond PRIVATE */
#include <drivers/uart/adi_uart.h>
#include <drivers/dma/adi_dma.h>
#include "adi_uart_def.h"
#include <adi_cyclecount.h>


#ifdef __ICCARM__
/*
* IAR MISRA C 2004 error suppressions.
*
* Pm50: (MISRA C 2004 rule 14.3): a null statement shall only occur on a line by itself,
*                                 and shall not have any other text on the same line
*                                 Some Macros, such as ISR_PROLOGUE, may not have any expansion
*                                 resulting in just the terminating ';'.
*
* Pm073 (rule 14.7): A function should have a single point of exit.
* Pm143 (rule 14.7): A function should have a single point of exit at the end of the function.
*                    Multiple returns are used for error handling.
*
* Pm088 (rule 17.4): Pointer arithmetic should not be used.
*                    Relying on pointer arithmetic for buffer handling.
*
* Pm123 (rule 18.5): There shall be no definition of objects in a header file.
*
* Pm140 (rule 11.3): A cast should not be performed between a pointer type and an integral type.
*                    MMR addresses are defined as simple constants. Accessing the MMR requires casting to a pointer type.
*
* Pm152 (rule 17.4): Array indexing shall only be applied to objects defined as an array type.
*                    Relying on pointer arithmetic for buffer handling and
*                    Accessing the DMA descriptors, which are defined in the system as a pointer to an array of descriptors.
*
* Pm008: Code should not be commented out.
         This code was commented out to show what the autobaud equations would look like if there were floating point precision.
         Ideally this would be the case but for the sake of footprint size we will leave it at single point precision.
*/
#pragma diag_suppress=Pm050,Pm073,Pm088,Pm123,Pm140,Pm143,Pm152,Pm008
#endif /* __ICCARM__ */




/**********************************************************
 * UART Data
 **********************************************************/
static  ADI_UART_DEVICE_INFO uart_device_info[ ] =
{
  {
    UART0_TX_CHANn,             /*!< DMA channel number for UART0 Tx. */
    UART0_RX_CHANn,             /*!< DMA channel number for UART0 Rx. */
    DMA0_CH8_DONE_IRQn,         /*!< DMA channel IRQ for UART0 Tx.    */
    DMA0_CH9_DONE_IRQn,         /*!< DMA channel IRQ for UART0 Rx.    */
    (IRQn_Type)INTR_UART0_EVT,  /*!< UART0 interrupt ID.              */
    pADI_UART0,                 /*!< Start address of UART0.          */
    NULL                        /*!< Device Handle for UART0.         */
  },
  {
    UART1_TX_CHANn,             /*!< DMA channel number for UART1 Tx. */
    UART1_RX_CHANn,             /*!< DMA channel number for UART1 Rx. */
    DMA0_CH25_DONE_IRQn,        /*!< DMA channel IRQ for UART1 Tx.    */
    DMA0_CH26_DONE_IRQn,        /*!< DMA channel IRQ for UART1 Rx.    */
    (IRQn_Type)INTR_UART1_EVT,  /*!< UART1 interrupt ID.              */
    pADI_UART1,                 /*!< Start address of UART1.          */
    NULL                        /*!< Device Handle for UART1.         */
  },
};

static const ADI_UART_CONFIG gUARTCfg[ ] =
{
    {
      /* Line control register.                                         */
      ((ADI_UART0_CFG_WORD_LENGTH          << BITP_UART_LCR_WLS)  |
       (ADI_UART0_CFG_STOP_BIT             << BITP_UART_LCR_STOP) |
       (ADI_UART0_CFG_ENABLE_PARITY        << BITP_UART_LCR_PEN)  |
       (ADI_UART0_CFG_PARITY_SELECTION     << BITP_UART_LCR_EPS)  |
       (ADI_UART0_CFG_ENABLE_STICKY_PARITY << BITP_UART_LCR_SP)),

      /* Div-C in baudrate divider register.                            */
      ADI_UART0_CFG_DIVC,

      /* Div-M and Div-N in fractional baudrate Register.               */
      (((uint32_t)ADI_UART0_CFG_DIVN       << BITP_UART_FBR_DIVN) |
       ((uint32_t)ADI_UART0_CFG_DIVM       << BITP_UART_FBR_DIVM) |
       ((uint32_t)BITM_UART_FBR_FBEN)),

      /* Over sample rate in second line control register.              */
      ADI_UART0_CFG_OSR,

      /* FIFO control register.                                         */
      ((ADI_UART0_CFG_ENABLE_FIFO          << BITP_UART_FCR_FIFOEN)|
       (ADI_UART0_CFG_TRIG_LEVEL           << BITP_UART_FCR_RFTRIG)),

      /* Half duplex control register.                                  */
      ((ADI_UART0_CFG_SOUT_POLARITY        << BITP_UART_RSC_OENP)  |
       (ADI_UART0_CFG_DEASSERTION          << BITP_UART_RSC_OENSP) |
       (ADI_UART0_CFG_DISABLE_RX           << BITP_UART_RSC_DISRX) |
       (ADI_UART0_CFG_HOLD_TX              << BITP_UART_RSC_DISTX)),

      /* Interrupt enable register.                                     */
      ((ADI_UART0_CFG_ENABLE_MODEM_STATUS_INTERRUPT << BITP_UART_IEN_EDSSI) |
      (ADI_UART0_CFG_ENABLE_RX_STATUS_INTERRUPT     << BITP_UART_IEN_ELSI))

    },

    {
      /* Line control register.                                         */
      ((ADI_UART1_CFG_WORD_LENGTH          << BITP_UART_LCR_WLS)  |
       (ADI_UART1_CFG_STOP_BIT             << BITP_UART_LCR_STOP) |
       (ADI_UART1_CFG_ENABLE_PARITY        << BITP_UART_LCR_PEN)  |
       (ADI_UART1_CFG_PARITY_SELECTION     << BITP_UART_LCR_EPS)  |
       (ADI_UART1_CFG_ENABLE_STICKY_PARITY << BITP_UART_LCR_SP)),

      /* Div-C in Baudrate divider register.                            */
      ADI_UART1_CFG_DIVC,

      /* Div-M and Div-N in fractional baudrate Register.               */
      (((uint32_t)ADI_UART1_CFG_DIVN       << BITP_UART_FBR_DIVN) |
       ((uint32_t)ADI_UART1_CFG_DIVM       << BITP_UART_FBR_DIVM) |
       ((uint32_t)BITM_UART_FBR_FBEN)),

      /* Over sample rate in second line control register.              */
      ADI_UART1_CFG_OSR,

      /* FIFO control register.                                         */
      ((ADI_UART1_CFG_ENABLE_FIFO          << BITP_UART_FCR_FIFOEN)|
       (ADI_UART1_CFG_TRIG_LEVEL           << BITP_UART_FCR_RFTRIG)),

      /* Half duplex control register.                                  */
      ((ADI_UART1_CFG_SOUT_POLARITY        << BITP_UART_RSC_OENP)  |
       (ADI_UART1_CFG_DEASSERTION          << BITP_UART_RSC_OENSP) |
       (ADI_UART1_CFG_DISABLE_RX           << BITP_UART_RSC_DISRX) |
       (ADI_UART1_CFG_HOLD_TX              << BITP_UART_RSC_DISTX)),

      /* Interrupt enable register.                                     */
      ((ADI_UART1_CFG_ENABLE_MODEM_STATUS_INTERRUPT << BITP_UART_IEN_EDSSI) |
       (ADI_UART1_CFG_ENABLE_RX_STATUS_INTERRUPT    << BITP_UART_IEN_ELSI))
    }
};

/*! \endcond */

/*! Number of UART devices available on the chip. */
#define ADI_UART_NUM_DEVICES (sizeof(uart_device_info)/sizeof(ADI_UART_DEVICE_INFO))

/* Override "weak" default binding in startup.c */
/*! \cond PRIVATE */
extern void UART0_Int_Handler(void);
extern void UART1_Int_Handler(void);
extern void DMA_UART0_TX_Int_Handler(void);
extern void DMA_UART0_RX_Int_Handler(void);
extern void DMA_UART1_TX_Int_Handler(void);
extern void DMA_UART1_RX_Int_Handler(void);

/* Internal DMA Callback for receiving DMA faults from common DMA error handler. */
static void RxDmaErrorCallback(void *pCBParam, uint32_t Event, void *pArg);
static void RxDmaErrorCallback(void *pCBParam, uint32_t Event, void *pArg) {

    /* Recover the device handle. */
    ADI_UART_HANDLE hDevice = (ADI_UART_HANDLE)pCBParam;
    ADI_UART_BUFF_INFO * pNextBuff = hDevice->pChannelRx->pFillBuffer->pNextBuffer;
    uint32_t nEvent = 0u;

    /* Save the DMA error. */
    switch (Event) {
        case ADI_DMA_EVENT_ERR_BUS:
            nEvent |= (uint32_t)ADI_UART_HW_ERR_RX_CHAN_DMA_BUS_FAULT;
            break;
        case ADI_DMA_EVENT_ERR_INVALID_DESCRIPTOR:
            nEvent |= (uint32_t)ADI_UART_HW_ERR_RX_CHAN_DMA_INVALID_DESCR;
            break;
        default:
            nEvent |= (uint32_t)ADI_UART_HW_ERR_RX_CHAN_DMA_UNKNOWN_ERROR;
            break;
    }

    if((pNextBuff->pStartAddress != NULL) && (pNextBuff->bDMA == true))
    {
        hDevice->nHwError |= nEvent;
        pNextBuff->bInUse = false;
        uart_ManageProcessedBuffer(hDevice,hDevice->pChannelRx,ADI_UART_EVENT_RX_BUFFER_PROCESSED);

    }
    hDevice->nHwError |= nEvent;
    uart_ManageProcessedBuffer(hDevice,hDevice->pChannelRx,ADI_UART_EVENT_RX_BUFFER_PROCESSED);
}

static void TxDmaErrorCallback(void *pCBParam, uint32_t Event, void *pArg);
static void TxDmaErrorCallback(void *pCBParam, uint32_t Event, void *pArg) {

    /* Recover the device handle. */
    ADI_UART_HANDLE hDevice = (ADI_UART_HANDLE)pCBParam;
    ADI_UART_BUFF_INFO * pNextBuff = hDevice->pChannelTx->pFillBuffer->pNextBuffer;
    uint32_t nEvent = 0u;

    /* Save the DMA error. */
    switch (Event) {
        case ADI_DMA_EVENT_ERR_BUS:
            nEvent |= (uint32_t)ADI_UART_HW_ERR_TX_CHAN_DMA_BUS_FAULT;
            break;
        case ADI_DMA_EVENT_ERR_INVALID_DESCRIPTOR:
            nEvent |= (uint32_t)ADI_UART_HW_ERR_TX_CHAN_DMA_INVALID_DESCR;
            break;
        default:
            nEvent |= (uint32_t)ADI_UART_HW_ERR_TX_CHAN_DMA_UNKNOWN_ERROR;
            break;
    }
    if((pNextBuff->pStartAddress != NULL) && (pNextBuff->bDMA == true))
    {
        hDevice->nHwError |= nEvent;
        pNextBuff->bInUse = false;
        uart_ManageProcessedBuffer(hDevice,hDevice->pChannelTx,ADI_UART_EVENT_TX_BUFFER_PROCESSED);

    }

    hDevice->nHwError |= nEvent;
    uart_ManageProcessedBuffer(hDevice,hDevice->pChannelTx,ADI_UART_EVENT_TX_BUFFER_PROCESSED);
}
/*! \endcond */

/**********************************************************
 * General UART APIs
 **********************************************************/

/*!
 * @brief       Initialization function for the UART device.
 * @details     Opens the specified UART device. This function must be called before operating any UART device.
 *
 *
 * @param [in]  nDeviceNum     UART device instance to be opened.
 * @param [in]  eDirection     Direction of the UART operation. (i.e Rx or Tx)
 * @param [in]  pMemory        Pointer to a 32 bit aligned buffer the size of #ADI_UART_UNIDIR_MEMORY_SIZE
 *                             or #ADI_UART_BIDIR_MEMORY_SIZE.
 * @param [in]  nMemSize       Size of the buffer to which "pMemory" points. This will vary based on
 *                             direction of operation for this device instance. (i.e Rx and Tx, Rx, Tx)
 *
 * @param [out] phDevice       The caller's device handle pointer for storing the initialized device instance data pointer.
 *
 * @return      Status
 *              - #ADI_UART_SUCCESS                     Successfully initialized UART device.
 *              - #ADI_UART_SEMAPHORE_FAILED            Failed to create semaphore.
 *              - #ADI_UART_INVALID_DEVICE_NUM      [D] Device instance is invalid.
 *              - #ADI_UART_INSUFFICIENT_MEMORY     [D] Supplied memory is insufficient for the operation of specified UART device.
 *              - #ADI_UART_DEVICE_IN_USE           [D] Device is already open.
 *
 * @sa  adi_uart_Close()
 *
 * @note: Memory supplied by the API will be used by the driver for managing the UART device. This memory can be reused once
 *        device is closed.
 *
 */
ADI_UART_RESULT adi_uart_Open(
                              uint32_t            const nDeviceNum,
                              ADI_UART_DIRECTION  const eDirection,
                              void                     *pMemory,
                              uint32_t            const nMemSize,
                              ADI_UART_HANDLE    *const phDevice
                             )
{
#ifdef ADI_DEBUG
    /* Check if the given device number is within the range of UART
     * devices present in the processor. There are two devices present here
     * so this can be a 0 or 1.
     */
    if(nDeviceNum >= ADI_UART_NUM_DEVICES)
    {
        return(ADI_UART_INVALID_DEVICE_NUM);
    }

    /* Verify the device is not already open. */
    if(uart_device_info[nDeviceNum].hDevice != NULL)
    {
        return(ADI_UART_DEVICE_IN_USE);
    }

    /* Make sure there is enough memory for the device instance to operate in a single direction. */
    if(eDirection !=  ADI_UART_DIR_BIDIRECTION)
    {
        if(nMemSize < (uint32_t)ADI_UART_UNIDIR_MEMORY_SIZE)
        {
            return(ADI_UART_INSUFFICIENT_MEMORY);
        }
        assert(nMemSize == (sizeof(ADI_UART_DEVICE) + sizeof(ADI_UART_DATA_CHANNEL)));
    }

    /* Make sure there is enough memory for the device instance to operate in both directions. */
    else
    {
        if(nMemSize < (uint32_t)ADI_UART_BIDIR_MEMORY_SIZE)
        {
            return(ADI_UART_INSUFFICIENT_MEMORY);
        }
        assert(nMemSize == (sizeof(ADI_UART_DEVICE) + (sizeof(ADI_UART_DATA_CHANNEL)*2u)));
    }
#endif /* ADI_DEBUG */

    /* Initialize the device handle to NULL in case of a failure. */
    *phDevice = NULL;

    /* Link the ADI_UART_HANDLE to the ADI_UART_DEVICE structure. */
     ADI_UART_HANDLE hDevice = pMemory;

    /* Zero the device handle memory so we do not have to explicitely initialize
       the structure members to 0.
    */
    memset(pMemory, 0, nMemSize);


    /* Set the device information. */
    hDevice->pUartInfo = &uart_device_info[nDeviceNum];

    /* Set the base of the UART register address. We do this to minimize
       the cycle count when accessing the UART registers.
    */
    hDevice->pUARTRegs = uart_device_info[nDeviceNum].pUartRegs;

    /* Store the direction that this device will operate in. */
    hDevice->eDirection = eDirection;

    /* Increment the device handle with the size of the UART device structure
       so we can set the channel data next without overwriting
       the #ADI_UART_DEVICE data.
    */
    pMemory = ((uint8_t *)pMemory +(sizeof(ADI_UART_DEVICE)));

    /* Set up the DMA Controller. */
    adi_dma_Init();

    /* Initialize the TX-channel. */
    if(ADI_UART_DIR_RECEIVE != eDirection)
    {
         hDevice->pChannelTx = (ADI_UART_DATA_CHANNEL *)pMemory;

         /* Initialize the data transfer mode. */
         hDevice->pChannelTx->eDataTranferMode = ADI_UART_DATA_TRANSFER_MODE_NONE;

         /* Initialize Tx buffer pointers. */
         hDevice->pChannelTx->pFreeBuffer = &hDevice->pChannelTx->PingPong[0];
         hDevice->pChannelTx->pActiveBuffer = &hDevice->pChannelTx->PingPong[0];
         hDevice->pChannelTx->pFillBuffer = &hDevice->pChannelTx->PingPong[0];


         /* Create a "semaphore" (varies per OS) used for blocking buffer resource management. */
         SEM_CREATE(hDevice->pChannelTx, "UART_TX_SEM", ADI_UART_SEMAPHORE_FAILED);

         /* Set submit buffer function pointer. */
         hDevice->pChannelTx->pfSubmitBuffer = &uart_submittxbuffer;

         hDevice->pChannelTx->PingPong[0].pNextBuffer  = &hDevice->pChannelTx->PingPong[1];
         hDevice->pChannelTx->PingPong[1].pNextBuffer  = &hDevice->pChannelTx->PingPong[0];

         /*Register DMA Callback. */
         if (ADI_DMA_SUCCESS != adi_dma_RegisterCallback(hDevice->pUartInfo->dmaTxChannelNum, TxDmaErrorCallback, (void*)hDevice))
         {
              adi_uart_Close(hDevice);
              return  ADI_UART_ERR_DMA_REGISTER;
         }

         /* Increment the device handle the size of #ADI_UART_DATA_CHANNEL
            structure in case there is another channel to configure.
         */
         pMemory =  ((uint8_t *)pMemory + sizeof(ADI_UART_DATA_CHANNEL));
    }
    /* Initialize the RX-channel. */
    if(ADI_UART_DIR_TRANSMIT != eDirection)
    {
        hDevice->pChannelRx = (ADI_UART_DATA_CHANNEL *)pMemory;

        /* Initialize the data transfer mode. */
        hDevice->pChannelRx->eDataTranferMode = ADI_UART_DATA_TRANSFER_MODE_NONE;

        /* Initialize Rx buffer pointers. */
        hDevice->pChannelRx->pFreeBuffer = &hDevice->pChannelRx->PingPong[0];
        hDevice->pChannelRx->pActiveBuffer = &hDevice->pChannelRx->PingPong[0];
        hDevice->pChannelRx->pFillBuffer = &hDevice->pChannelRx->PingPong[0];

        /* Create a "semaphore" (varies per OS) used for blocking buffer resource management. */
        SEM_CREATE(hDevice->pChannelRx, "UART_RX_SEM", ADI_UART_SEMAPHORE_FAILED);

        /* Set submit buffer function pointer. */
        hDevice->pChannelRx->pfSubmitBuffer = &uart_submitrxbuffer;

        hDevice->pChannelRx->PingPong[0].pNextBuffer  = &hDevice->pChannelRx->PingPong[1];
        hDevice->pChannelRx->PingPong[1].pNextBuffer  = &hDevice->pChannelRx->PingPong[0];

         /*Register DMA Callback. */
         if (ADI_DMA_SUCCESS != adi_dma_RegisterCallback(hDevice->pUartInfo->dmaRxChannelNum, RxDmaErrorCallback, (void*)hDevice))
         {
              adi_uart_Close(hDevice);
              return  ADI_UART_ERR_DMA_REGISTER;
         }
    }

    /* Initialize the device with the static config values.*/
    uart_init(hDevice, nDeviceNum);

    /* Write the device data pointer to the application's handle. */
    *phDevice = hDevice;

    /* Store the device handle. */
    uart_device_info[nDeviceNum].hDevice = hDevice;


    /* Enable UART Interrupt. */
    NVIC_ClearPendingIRQ(hDevice->pUartInfo->eIRQn);
    NVIC_EnableIRQ(hDevice->pUartInfo->eIRQn);

    /* Enable the interrupt for the DMA. */
    NVIC_EnableIRQ(hDevice->pUartInfo->eDMATx);
    NVIC_EnableIRQ(hDevice->pUartInfo->eDMARx);

    /* Return SUCCESS */
    return(ADI_UART_SUCCESS);
}

/*!
 * @brief       Uninitialize the memory for the specified UART instance.
 *
 * @param [in]  hDevice   UART device handle whose operation is to be closed. This handle was obtained when the UART
 *                        device instance was opened successfully.
 *
 * @return      Status
 *    - #ADI_UART_SUCCESS               Successfully closed the UART device instance.
 *    - #ADI_UART_SEMAPHORE_FAILED      Failed to delete the semaphore.
 *    - #ADI_UART_INVALID_HANDLE    [D] Invalid UART device handle.
 *    - #ADI_UART_DEVICE_IN_USE     [D] Specified UART device is in the process of a transaction or autobaud has not completed.
 *
 * @details     Closes the operation of specified UART device. Device needs to be opened again for any further use.
 *
 * @sa  adi_uart_Open()
 *
 * @note: It is the user's responsibility to free/reuse the memory supplied during the opening of the device.
 */
ADI_UART_RESULT adi_uart_Close(
                               ADI_UART_HANDLE const hDevice
                              )
{
#ifdef ADI_DEBUG
    /* Validate the given handle. */
    if(ValidateHandle(hDevice) != ADI_UART_SUCCESS)
    {
        return(ADI_UART_INVALID_HANDLE);
    }

    /* Make sure there are no active buffers on any active channel, autobaud is not in progress and the
       Tx shift register is completely empty. This can be an issue if you submitted a nonblocking transmit
       because you will receive interrupt before the hardware has fully finished the transaction. The start
       address of the active buffer will remain in use until the buffer has been completely processed.
       Therefore if the start address is NULL it means it has not been submitted for a transaction.
    */
    if(((hDevice->pUARTRegs->LSR & BITM_UART_LSR_TEMT) != BITM_UART_LSR_TEMT)                                        ||
       ((hDevice->eDirection != ADI_UART_DIR_TRANSMIT) && (hDevice->pChannelRx->pFillBuffer->pStartAddress != NULL)) ||
       ((hDevice->eDirection != ADI_UART_DIR_RECEIVE ) && (hDevice->pChannelTx->pFillBuffer->pStartAddress != NULL)) ||
       (hDevice->bAutobaudInProgress == true))
    {
        return(ADI_UART_DEVICE_IN_USE);
    }
#endif /* ADI_DEBUG */

    /* Disable UART status interrupts. */
    hDevice->pUARTRegs->IEN = 0x00U;

    /* Disable DMA UART interrupts. */
    NVIC_DisableIRQ(hDevice->pUartInfo->eDMARx);
    NVIC_DisableIRQ(hDevice->pUartInfo->eDMATx);

    /* Disable UART event interrupt. */
    NVIC_DisableIRQ(hDevice->pUartInfo->eIRQn);

    /* Delete Tx-Channel semaphore. */
    if(hDevice->eDirection != ADI_UART_DIR_RECEIVE)
    {
        SEM_DELETE(hDevice->pChannelTx, ADI_UART_SEMAPHORE_FAILED);
    }

    /* Delete Rx-Channel semaphore. */
    if(hDevice->eDirection != ADI_UART_DIR_TRANSMIT)
    {
        SEM_DELETE(hDevice->pChannelRx, ADI_UART_SEMAPHORE_FAILED);
    }

     /* Free up the device memory. */
    hDevice->pUartInfo->hDevice = NULL;

    return(ADI_UART_SUCCESS);
}

/*!
 * @brief       Submit a "filled" buffer for transmitting data in #ADI_UART_DATA_TRANSFER_MODE_NONBLOCKING.
 *              This function sets up the apropriate interrupts associated with the transaction and marks
 *              the buffer as submitted.
 *
 * @param [in]  hDevice    Device handle obtained from adi_uart_Open().
 * @param [in]  pBuffer    Pointer to data supplied by the API that is to be transmitted.
 * @param [in]  nBufSize   Size of the buffer to be transmitted(in bytes). Must be smaller than 1024 bytes for DMA transfers.
 * @param [in]  bDMA       Submit the buffer using the DMA flag.

 *
 * @return      Status
 *    - #ADI_UART_SUCCESS                        Successfully submitted the buffer for transmission.
 *    - #ADI_UART_FAILED                     [D] Generic failure. In this case the size of the data buffer we are trying
 *                                               to submit is NULL.
 *    - #ADI_UART_INVALID_DATA_TRANSFER_MODE [D] Device is operating in #ADI_UART_DATA_TRANSFER_MODE_BLOCKING. This
 *                                               operation is only allowed in #ADI_UART_DATA_TRANSFER_MODE_NONBLOCKING.
 *    - #ADI_UART_INVALID_HANDLE             [D] Invalid UART device handle.
 *    - #ADI_UART_OPERATION_NOT_ALLOWED      [D] Device direction is set up as #ADI_UART_DIR_RECEIVE, so we can not complete
 *                                               a transmit operation. The required directions are #ADI_UART_DIR_TRANSMIT or
 *                                               #ADI_UART_DIR_BIDIRECTION.
 *    - #ADI_UART_INVALID_POINTER            [D] Pointer to the buffer being submitted is NULL.
 *    - #ADI_UART_DEVICE_IN_USE              [D] Autobaud in progress.
 *    - #ADI_UART_INVALID_DATA_SIZE          [D] DMA transfers must be smaller than 1025 bytes.
 *
 * @sa  adi_uart_IsTxBufferAvailable()
 * @sa  adi_uart_GetTxBuffer()
 * @sa  adi_uart_SubmitRxBuffer()
 *
 * @note: Only one transfer mode (DMA vs. PIO) can be used at once. For example, if you submit a buffer in PIO mode
 *        and then right away another using the DMA, this transaction will be denied.
 *
 */
ADI_UART_RESULT adi_uart_SubmitTxBuffer(
                                         ADI_UART_HANDLE const hDevice,
                                         void           *const pBuffer,
                                         uint32_t        const nBufSize,
                                         bool            const bDMA
                                       )
{

#ifdef ADI_DEBUG
    /* Validate the device handle. */
    if(ValidateHandle(hDevice) != ADI_UART_SUCCESS)
    {
        return(ADI_UART_INVALID_HANDLE);
    }

    /* Validate the pointer to the buffer memory. */
    if(pBuffer == NULL)
    {
        return(ADI_UART_INVALID_POINTER);
    }

    /* Validate the buffer size. */
    if(nBufSize == 0U)
    {
        return(ADI_UART_FAILED);
    }

    /* Autobaud in progress. */
    if(hDevice->bAutobaudInProgress == true)
    {
        return(ADI_UART_DEVICE_IN_USE);
    }

    /* Make sure we are transmitting. */
    if(ADI_UART_DIR_RECEIVE == hDevice->eDirection)
    {
        return(ADI_UART_OPERATION_NOT_ALLOWED);
    }

    /* Check the data transfer mode (only allowed in nonblocking mode). */
    if(hDevice->pChannelTx->eDataTranferMode == ADI_UART_DATA_TRANSFER_MODE_BLOCKING)
    {
        return(ADI_UART_INVALID_DATA_TRANSFER_MODE);
    }

    /* Check that there is a free buffer to use for this transmit operation. pFreeBuffer
       is the next buffer available, so if it is in use we can make the assumption that
       there are no buffers available. The start address is set to NULL once the buffer
       has finished being processed in "adi_uart_GetBuffer()" or "adi_uart_PendForBuffer()".
    */
    if(hDevice->pChannelTx->pFreeBuffer->pStartAddress != NULL)
    {
        return(ADI_UART_OPERATION_NOT_ALLOWED);
    }

    /* Make sure the DMA transfer size is not too large. */
    if((bDMA == true) && (nBufSize > DMA_TRANSFER_LIMIT))
    {
        return(ADI_UART_INVALID_DATA_SIZE);
    }

#endif /* ADI_DEBUG */

     /* Set the start address of the data buffer we are going to submit. */
     hDevice->pChannelTx->pFreeBuffer->pStartAddress =   pBuffer;

     /* Set the buffer size to the size of the data buffer passed down from the API. */
     hDevice->pChannelTx->pFreeBuffer->nCount        =   nBufSize;

     /* Initialize the buffer index to zero because we will start shifting out
        the Tx data from the first position of the buffer.
     */
     hDevice->pChannelTx->pFreeBuffer->nIndex        =   0U;

     /* Mark the buffer as in use so no other transactions can use it until this one is complete. */
     hDevice->pChannelTx->pFreeBuffer->bInUse        =   true;

     /* Mark the DMA as in use. */
     hDevice->pChannelTx->pFreeBuffer->bDMA          =   bDMA;

     /* Now that this "pFreeBuffer" is no longer free for use, update the
        "pFreeBuffer" to the other PingPong buffer. Because there are only two
        buffers in the PingPong structure, this will be the opposite of the one
        we just submitted. "pFreeBuffer" will only be updated during the process of
        submitting a buffer or a read/write operation.
     */
      hDevice->pChannelTx->pFreeBuffer = hDevice->pChannelTx->pFreeBuffer->pNextBuffer;

     /* Set the data transfer mode in case it was #ADI_UART_DATA_TRANSFER_MODE_NONE.
        This will be set back to #ADI_UART_DATA_TRANSFER_MODE_NONE once this
        transaction is complete. Then, if a buffer is not currently active, set up the
        interrupts for this transaction. Otherwise if a buffer is currently active,
        this will be taken care of in the ISR.
     */
     if (hDevice->pChannelTx->eDataTranferMode == ADI_UART_DATA_TRANSFER_MODE_NONE)
     {
         hDevice->pChannelTx->eDataTranferMode = ADI_UART_DATA_TRANSFER_MODE_NONBLOCKING;
         hDevice->pChannelTx->pfSubmitBuffer(hDevice, hDevice->pChannelTx->pFillBuffer);
     }

     return(ADI_UART_SUCCESS);
  }

/*!  \cond PRIVATE */

/*
 * @brief      This is an internal helper function for adi_uart_SubmitTxBuffer(). It sets up the Tx channel DMA
               or device interrupts for the Tx channel to transmit data.
 *
 * @param [in]  hDevice    Device handle obtained from adi_uart_Open().
 * @param [in]  pBuffer    Pointer to buffer from where data will be transmitted.
 * @param [in]  nBufSize   Size of the buffer containing the data to be transmitted(in bytes).
 * @param [in]  bDMA       Submit the buffer using the DMA.
*/
static void uart_submittxbuffer(
                                 ADI_UART_CONST_HANDLE  const hDevice,
                                 ADI_UART_BUFF_INFO    *const pBuffer
                               )
{
  /* If this transmission is using DMA... */
  if (pBuffer->bDMA)
  {
      /* Enable clear source address decrement for TX channel DMA. */
      pADI_DMA0->SRCADDR_CLR = 1u << (uint32_t)hDevice->pUartInfo->dmaTxChannelNum;

      /* Enable Tx channel DMA. */
      pADI_DMA0->EN_SET = 1u << hDevice->pUartInfo->dmaTxChannelNum;

      /* Enable UART peripheral to generate DMA requests. */
      pADI_DMA0->RMSK_CLR = 1u << hDevice->pUartInfo->dmaTxChannelNum;

      /* Set the primary control data structure as the current DMA descriptor. */
      pADI_DMA0->ALT_CLR = 1u << hDevice->pUartInfo->dmaTxChannelNum;

      /* Fill in the DMA RAM descriptors */
      pPrimaryCCD[hDevice->pUartInfo->dmaTxChannelNum].DMASRCEND =   ((uint32_t)pBuffer->pStartAddress + (uint32_t)(pBuffer->nCount - 1u));

      pPrimaryCCD[hDevice->pUartInfo->dmaTxChannelNum].DMADSTEND =   (uint32_t)&hDevice->pUARTRegs->TX;

      pPrimaryCCD[hDevice->pUartInfo->dmaTxChannelNum].DMACDC    =   ((uint32_t)ADI_DMA_INCR_NONE << DMA_BITP_CTL_DST_INC)      |
                                                                     ((uint32_t)ADI_DMA_INCR_1_BYTE << DMA_BITP_CTL_SRC_INC)    |
                                                                     (ADI_DMA_WIDTH_1_BYTE << DMA_BITP_CTL_SRC_SIZE)            |
                                                                     (0u << DMA_BITP_CTL_R_POWER)                               |
                                                                     ((pBuffer->nCount - 1u) << DMA_BITP_CTL_N_MINUS_1)         |
                                                                     (DMA_ENUM_CTL_CYCLE_CTL_BASIC << DMA_BITP_CTL_CYCLE_CTL);
      /* Enable UART DMA request interrupt for the Tx channel. */
      hDevice->pUARTRegs->IEN |= (BITM_UART_IEN_EDMAT);
  }
  else
  /* If this transmission is using UART interrupts.. */
  {
      /* Enable buffer empty interrupts.  */
      hDevice->pUARTRegs->IEN |= (BITM_UART_IEN_ETBEI);
  }
}

/*! \endcond */

/*!
 * @brief       Submit an empty buffer for receiving the data in #ADI_UART_DATA_TRANSFER_MODE_NONBLOCKING.
 *              This will set up the Rx channel for notification on incoming data using either the DMA
 *              or UART interrupts, as well as mark the buffer as submitted.
 *
 * @param [in]  hDevice    Device handle obtained from adi_uart_Open().
 * @param [in]  pBuffer    Pointer to buffer from that will be filled by the driver when data has been received.
 * @param [in]  nBufSize   Size of the buffer(in bytes). Must be smaller than 1024 bytes for DMA transfers.
 * @param [in]  bDMA       Submit the buffer using DMA flag.

 *
 * @return      Status
 *    - #ADI_UART_SUCCESS                        Successfully submitted the buffer for receiving data.
 *    - #ADI_UART_FAILED                     [D] Generic failure. In this case the size of the data buffer we are trying
 *                                               to submit is NULL.
 *    - #ADI_UART_INVALID_DATA_TRANSFER_MODE [D] Device is operating in #ADI_UART_DATA_TRANSFER_MODE_BLOCKING. This
 *                                               operation is only allowed in #ADI_UART_DATA_TRANSFER_MODE_NONBLOCKING.
 *    - #ADI_UART_INVALID_HANDLE             [D] Invalid UART device handle.
 *    - #ADI_UART_OPERATION_NOT_ALLOWED      [D] Device direction is set up as #ADI_UART_DIR_TRANSMIT, so we can not complete
 *                                               a receive operation. The required directions are #ADI_UART_DIR_RECEIVE or
 *                                               #ADI_UART_DIR_BIDIRECTION.
 *    - #ADI_UART_INVALID_POINTER            [D] Pointer to the buffer being submitted is NULL.
 *    - #ADI_UART_DEVICE_IN_USE              [D] Autobaud in progress.
 *    - #ADI_UART_INVALID_DATA_SIZE          [D] DMA transfers must be smaller than 1025 bytes.
 *
 * @sa  adi_uart_IsRxBufferAvailable()
 * @sa  adi_uart_GetRxBuffer()
 * @sa  adi_uart_SubmitTxBuffer()
 *
 * @note: Only one transfer mode (DMA vs. PIO) can be used at once. For example, if you submit a buffer in PIO mode
 *        and then right away another using the DMA, this transaction will be denied.
*/
ADI_UART_RESULT adi_uart_SubmitRxBuffer(
                                         ADI_UART_HANDLE const hDevice,
                                         void           *const pBuffer,
                                         uint32_t        const nBufSize,
                                         bool            const bDMA
                                       )
{

#ifdef ADI_DEBUG
    /* Validate the device handle. */
    if(ValidateHandle(hDevice) != ADI_UART_SUCCESS)
    {
        return(ADI_UART_INVALID_HANDLE);
    }

    /* Validate the pointer to the buffer memory. */
    if(pBuffer == NULL)
    {
        return(ADI_UART_INVALID_POINTER);
    }

    /* Validate the buffer size. */
    if(nBufSize == 0U )
    {
        return(ADI_UART_FAILED);
    }

    /* Autobaud in progress. */
    if(hDevice->bAutobaudInProgress == true)
    {
        return(ADI_UART_DEVICE_IN_USE);
    }

    /* Make sure the UART device is configured to operate in the receive direction. */
    if(ADI_UART_DIR_TRANSMIT == hDevice->eDirection)
    {
        return(ADI_UART_OPERATION_NOT_ALLOWED);
    }

    /* Check for the data transfer mode(only allowed in nonblocking mode). */
    if(hDevice->pChannelRx->eDataTranferMode == ADI_UART_DATA_TRANSFER_MODE_BLOCKING)
    {
        return(ADI_UART_INVALID_DATA_TRANSFER_MODE);
    }

    /* Check that there is a free buffer to use for this operation. pFreeBuffer
       is the next buffer available, so if it is in use we can make the assumption that
       there are no buffers available. If the start address is not set to NULL, then we
       can conclude the buffer has not finished being processed because this gets set in
       adi_uart_pend_for_buffer() and adi_uart_get_buffer().
    */
    if(hDevice->pChannelRx->pFreeBuffer->pStartAddress != NULL)
    {
        return(ADI_UART_OPERATION_NOT_ALLOWED);
    }

    /* Make sure the DMA transfer size is not too large. */
    if((bDMA == true) && (nBufSize > DMA_TRANSFER_LIMIT))
    {
        return(ADI_UART_INVALID_DATA_SIZE);
    }

#endif /* ADI_DEBUG */

    /* Set the start address of the buffer you are going to submit. */
     hDevice->pChannelRx->pFreeBuffer->pStartAddress =   pBuffer;

     /* Set the size of the buffer. */
     hDevice->pChannelRx->pFreeBuffer->nCount        =   nBufSize;

     /* Initialize the buffer index to 0, because as we receive data it will be put into
        the buffer starting at the first position.
     */
     hDevice->pChannelRx->pFreeBuffer->nIndex        =   0U;

     /* Mark the buffer as in use. */
     hDevice->pChannelRx->pFreeBuffer->bInUse        =   true;

     /* Mark the DMA as in use. */
     hDevice->pChannelRx->pFreeBuffer->bDMA          =   bDMA;

     /* Now that this "pFreeBuffer" is no longer free for use, update the
        "pFreeBuffer" to the other PingPong buffer. Because there are only two
        buffers in the PingPong structure, this will be the opposite of the one
        we just submitted. "pFreeBuffer" will only be updated during the process of
        submitting a buffer or a read/write operation.
     */
      hDevice->pChannelRx->pFreeBuffer = hDevice->pChannelRx->pFreeBuffer->pNextBuffer;


    /* Set the data transfer mode in case it was #ADI_UART_DATA_TRANSFER_MODE_NONE.
       This will be set back to #ADI_UART_DATA_TRANSFER_MODE_NONE once this
       transaction is complete. Then, if a buffer is not currently active, set up the
       interrupts for this transaction. Otherwise if a buffer is currently active,
       this will be taken care of in the ISR.
    */
     if (hDevice->pChannelRx->eDataTranferMode == ADI_UART_DATA_TRANSFER_MODE_NONE)
     {
         hDevice->pChannelRx->eDataTranferMode = ADI_UART_DATA_TRANSFER_MODE_NONBLOCKING;
         hDevice->pChannelRx->pfSubmitBuffer(hDevice, hDevice->pChannelRx->pFillBuffer);
     }

     return(ADI_UART_SUCCESS);
}

/*! \cond PRIVATE */

/*
 * @brief                  This is an internal helper function for adi_uart_SubmitRxBuffer(). It sets up the DMA
 *                         or device receive interrupts for the Rx channel to receive data.
 *
 * @param [in]  hDevice    Device handle obtained from adi_uart_Open().
 * @param [in]  pBuffer    Pointer to the empty receive buffer.
 * @param [in]  nBufSize   Size of the receive buffer(in bytes).
 * @param [in]  bDMA       Submit the buffer using the DMA.
*/
static void uart_submitrxbuffer(
                                 ADI_UART_CONST_HANDLE   const hDevice,
                                 ADI_UART_BUFF_INFO     *const pBuffer
                               )
{


    /* If this transaction is using the DMA.. */
    if (pBuffer->bDMA)
    {
        /* Enable source address decrement for RX DMA channel. */
        pADI_DMA0->DSTADDR_CLR = 1u << (uint32_t)hDevice->pUartInfo->dmaRxChannelNum;

        /* Enable Rx DMA channel. */
        pADI_DMA0->EN_SET = 1u << hDevice->pUartInfo->dmaRxChannelNum;

        /* Enable UART peripheral to generate DMA requests. */
        pADI_DMA0->RMSK_CLR = 1u << hDevice->pUartInfo->dmaRxChannelNum;

        /* Set the primary data structure as the current DMA descriptor. */
        pADI_DMA0->ALT_CLR = 1u << hDevice->pUartInfo->dmaRxChannelNum;

        /* Fill in the DMA RAM descriptors. */
        pPrimaryCCD[hDevice->pUartInfo->dmaRxChannelNum].DMASRCEND =   (uint32_t)&hDevice->pUARTRegs->RX;

        pPrimaryCCD[hDevice->pUartInfo->dmaRxChannelNum].DMADSTEND =   ((uint32_t)pBuffer->pStartAddress + (uint32_t)(pBuffer->nCount - 1u));

        pPrimaryCCD[hDevice->pUartInfo->dmaRxChannelNum].DMACDC    =   (uint32_t)(ADI_DMA_INCR_1_BYTE << DMA_BITP_CTL_DST_INC)            |
                                                                       (uint32_t)(ADI_DMA_INCR_NONE << DMA_BITP_CTL_SRC_INC)              |
                                                                       (ADI_DMA_WIDTH_1_BYTE << DMA_BITP_CTL_SRC_SIZE)                    |
                                                                       (0u << DMA_BITP_CTL_R_POWER)                                       |
                                                                       ((pBuffer->nCount - 1u) << DMA_BITP_CTL_N_MINUS_1)                 |
                                                                       (DMA_ENUM_CTL_CYCLE_CTL_BASIC << DMA_BITP_CTL_CYCLE_CTL);
        /* Enable UART receive DMA requests. */
        hDevice->pUARTRegs->IEN |= (BITM_UART_IEN_EDMAR);
    }
    /* If this transaction is using UART interrupts.. */
    else
    {
        /* Enable buffer full interrupt. */
        hDevice->pUARTRegs->IEN |= (BITM_UART_IEN_ERBFI);
    }
}

/*! \endcond */

/*!
 * @brief       Transfer buffer ownership from the device back to the API if the data
 *              transmit has completed. Otherwise it will block until completion.
 *              This allows a nonblocking call to become blocking.
 *              This function is only called in #ADI_UART_DATA_TRANSFER_MODE_NONBLOCKING.
 *
 * @param [in]  hDevice    Device handle obtained from adi_uart_Open().
 * @param [in]  ppBuffer   Contains the address of the buffer passed down from the API
 *                         for transmitting data.
 * @param [out] pHwError   Pointer to an integer that correlates with #ADI_UART_HW_ERRORS, containg the hardware status.
 *                         If there is no hardware event, this will be 0.
 *
 * @return      Status
 *    - #ADI_UART_SUCCESS                       Successfully returned buffer to the API.
 *    - #ADI_UART_HW_ERROR_DETECTED             Hardware error(s) detected. "pHwError" can be checked for the specific error code(s).
 *    - #ADI_UART_OPERATION_NOT_ALLOWED     [D] Call to this function is not allowed in #ADI_UART_DATA_TRANSFER_MODE_BLOCKING.
 *    - #ADI_UART_INVALID_HANDLE            [D] Invalid UART device handle.
 *    - #ADI_UART_BUFFER_NOT_SUBMITTED      [D] The buffer has not been submitted to the driver.
 *
 * @sa  adi_uart_IsTxBufferAvailable()
 * @sa  adi_uart_SubmitTxBuffer()
 *
 * @note: If the transaction has already completed, this will return immediately rather than block.
 */
ADI_UART_RESULT adi_uart_GetTxBuffer(
                                      ADI_UART_HANDLE const   hDevice,
                                      void          **const   ppBuffer,
                                      uint32_t               *pHwError
                                     )

{

#ifdef ADI_DEBUG
    /* Validate the device handle. */
    if(ValidateHandle(hDevice) != ADI_UART_SUCCESS)
    {
        return(ADI_UART_INVALID_HANDLE);
    }

    /* Validate that this buffer has actually been submitted. */
    if(hDevice->pChannelTx->pActiveBuffer->pStartAddress == NULL)
    {
        return(ADI_UART_BUFFER_NOT_SUBMITTED);
    }

    /* This function is allowed to be called when the channel is operating in NONBLOCKING mode. */
    if(hDevice->pChannelTx->eDataTranferMode ==  ADI_UART_DATA_TRANSFER_MODE_BLOCKING)
    {
        return(ADI_UART_OPERATION_NOT_ALLOWED);
    }
#endif /* ADI_DEBUG */

    /* Blocking call to get the submitted buffer */
    return(uart_getbuffer(hDevice, hDevice->pChannelTx, ppBuffer, pHwError));
}



/*!
 * @brief       Transfer buffer ownership from the device back to the API if the data
 *              receive has completed. Otherwise it will block until completion.
 *              This allows a nonblocking call to become blocking.
 *              This function is only called in #ADI_UART_DATA_TRANSFER_MODE_NONBLOCKING.
 *
 * @param [in]  hDevice    Device handle obtained from adi_uart_Open().
 * @param [in]  ppBuffer   Contains the address of the buffer passed down from the API
 *                         for receiving data.
 * @param [out] pHwError   Pointer to an integer that correlates with #ADI_UART_HW_ERRORS, containg the hardware status.
 *                         If there is no hardware event, this will be 0.
 *
 * @return      Status
 *    - #ADI_UART_SUCCESS                       Successfully returned buffer to the API.
 *    - #ADI_UART_HW_ERROR_DETECTED             Hardware error(s) detected. "pHwError" can be checked for the specific error code(s).
 *    - #ADI_UART_OPERATION_NOT_ALLOWED     [D] Call to this function is not allowed in #ADI_UART_DATA_TRANSFER_MODE_BLOCKING.
 *    - #ADI_UART_INVALID_HANDLE            [D] Invalid UART device handle.
 *    - #ADI_UART_BUFFER_NOT_SUBMITTED      [D] The buffer has not been submitted to the driver.
 *
 * @sa  adi_uart_IsRxBufferAvailable()
 * @sa  adi_uart_SubmitRxBuffer()
 *
 * @note: If the transaction has already completed, this will return immediately rather than block.
*/
ADI_UART_RESULT adi_uart_GetRxBuffer(
                                      ADI_UART_HANDLE  const   hDevice,
                                      void           **const   ppBuffer,
                                      uint32_t                *pHwError
                                     )

{

#ifdef ADI_DEBUG
    /* Validate the device handle. */
    if(ValidateHandle(hDevice) != ADI_UART_SUCCESS)
    {
        return(ADI_UART_INVALID_HANDLE);
    }

    /* Validate that this buffer has actually been submitted. */
    if(hDevice->pChannelRx->pActiveBuffer->pStartAddress == NULL)
    {
        return(ADI_UART_BUFFER_NOT_SUBMITTED);
    }

     /* This function is only allowed to be called when the channel is operating in NONBLOCKING mode. */
    if(hDevice->pChannelRx->eDataTranferMode ==  ADI_UART_DATA_TRANSFER_MODE_BLOCKING)
    {
        return(ADI_UART_OPERATION_NOT_ALLOWED);
    }
#endif /* ADI_DEBUG */

    /* Blocking call to get the full Rx Buffer */
    return(uart_getbuffer(hDevice, hDevice->pChannelRx, ppBuffer, pHwError));
}

/*! \cond PRIVATE */

/*
 * @brief       This is an internal helper function for adi_uart_GetRxBuffer() and adi_uart_GetTxBuffer().
 *              It blocks until until the completion of the data transaction.
 *
 * @param [in]  hDevice    Device handle obtained from adi_uart_Open().
 * @param [in]  pChannel   Pointer to UART channel data structure.
 * @param [out] ppBuffer   Contains the address of the buffer passed down from the API.
 * @param [out] pHwError   Pointer to an integer that correlates with #ADI_UART_HW_ERRORS, containg the hardware status.
 *                         If there is no hardware event, this will be 0.
 *
 * @return      Status
 *    - #ADI_UART_SUCCESS                Successfully got buffer.
 *    - #ADI_UART_HW_ERROR_DETECTED      Hardware error(s) detected. "pHwError" can be checked for the specific error code(s).
 *
*/
static ADI_UART_RESULT uart_getbuffer(
                                       ADI_UART_HANDLE        hDevice,
                                       ADI_UART_DATA_CHANNEL *pChannel,
                                       void                 **ppBuffer,
                                       uint32_t              *pHwError
                                      )
{
    /* Set ppBuffer to NULL in case there is an error. */
    *ppBuffer = NULL;

    /* Wait until the peripheral has finished processing the buffer. */
    SEM_PEND(pChannel,ADI_UART_FAILED);

    /* Save the address of the buffer that has just been processed, so it can be
       returned back to the API.
    */
    *ppBuffer = pChannel->pActiveBuffer->pStartAddress;

    /* Reinitialize the start address to NULL so this buffer can be used for a new transaction. */
    pChannel->pActiveBuffer->pStartAddress = NULL;

    /* Now that the desired data has either been transmitted or received, this buffer is no longer
       in use. We can update "pActiveBuffer" to point to the next buffer that will become or is already
       active.
    */
     pChannel->pActiveBuffer = pChannel->pActiveBuffer->pNextBuffer;

     /* Set the data transfer mode to none so that the next transfer can be either in blocking or in nonblocking mode.
        This will only be done if there are no other active buffers in flight to avoid disrupting an active transfer.
    */
     if(pChannel->pActiveBuffer->pStartAddress == NULL)
     {
        pChannel->eDataTranferMode = ADI_UART_DATA_TRANSFER_MODE_NONE;
     }

    /* If there are hardware errors and no callback, then return failure. */
    if(hDevice->nHwError != 0u)
    {
        /* Save the hardware error detected. This will be passed back to the API. */
        *pHwError = hDevice->nHwError;

        /* Clear any hardware errors detected. */
        hDevice->nHwError = 0u;

        return(ADI_UART_HW_ERROR_DETECTED);
    }
    else
    {
        return(ADI_UART_SUCCESS);
    }
}

/*! \endcond */


/*!
 * @brief       Submit the buffer for transmitting the data in #ADI_UART_DATA_TRANSFER_MODE_BLOCKING.
 *              Call to this function will not return until the entire buffer is transmitted.
 *              Returns error if this function is called when device is operating in #ADI_UART_DATA_TRANSFER_MODE_NONBLOCKING.
 *              i.e Function "adi_uart_SubmitTxBuffer()" is called and the transfer is not yet complete.
 *
 * @param [in]  hDevice    Device handle obtained from adi_uart_Open().
 * @param [in]  pBuffer    Pointer to data supplied by the API that is to be transmitted.
 * @param [in]  nBufSize   Size of the buffer(in bytes). Must be smaller than 1024 bytes for DMA transfers.
 * @param [in]  bDMA       Submit the buffer using the DMA flag.
 * @param [out] pHwError   Pointer to an integer that correlates with #ADI_UART_HW_ERRORS, containg the hardware status.
 *                         If there is no hardware event, this will be 0.
 *
 * @return      Status
 *    - #ADI_UART_SUCCESS                             Successfully transmitted the data from the submitted buffer.
 *    - #ADI_UART_HW_ERROR_DETECTED                   Hardware error(s) detected. "pHwError" can be checked for the specific error code(s).
 *    - #ADI_UART_FAILED                          [D] Generic failure. In this case the size of the data buffer we are trying
 *                                                    to submit is NULL.
 *    - #ADI_UART_INVALID_DATA_TRANSFER_MODE      [D] Device is operating in #ADI_UART_DATA_TRANSFER_MODE_NONBLOCKING. This
 *                                                    operation is only allowed in #ADI_UART_DATA_TRANSFER_MODE_BLOCKING.
 *    - #ADI_UART_OPERATION_NOT_ALLOWED           [D] Device direction is set up as #ADI_UART_DIR_RECEIVE, so we can not complete
 *                                                    a transmit operation. The required directions are #ADI_UART_DIR_TRANSMIT or
 *                                                    #ADI_UART_DIR_BIDIRECTION.
 *    - #ADI_UART_INVALID_HANDLE                  [D] Invalid UART device handle.
 *    - #ADI_UART_INVALID_POINTER                 [D] The pointer to the buffer being submitted is a NULL.
 *    - #ADI_UART_DEVICE_IN_USE                   [D] Autobaud in progress.
 *    - #ADI_UART_INVALID_DATA_SIZE               [D] DMA transfers must be smaller than 1025 bytes.
 *
 * @sa  adi_uart_Read()
 * @sa  adi_uart_SubmitTxBuffer()
 *
 * @note: This function is a blocking function which means that the function returns only after the completion of
 *        buffer transmission.
*/
ADI_UART_RESULT adi_uart_Write(
                               ADI_UART_HANDLE const hDevice,
                               void           *const pBuffer,
                               uint32_t        const nBufSize,
                               bool            const bDMA,
                               uint32_t             *pHwError
                              )
{

#ifdef ADI_DEBUG
        /* Validate the given handle. */
        if(ValidateHandle(hDevice) != ADI_UART_SUCCESS)
        {
            return(ADI_UART_INVALID_HANDLE);
        }

        /* Validate the pointer to the buffer memory. */
        if(pBuffer == NULL)
        {
            return(ADI_UART_INVALID_POINTER);
        }

        /* Validate the buffer size. */
        if(nBufSize == 0U )
        {
            return(ADI_UART_FAILED);
        }

        /* Autobaud in progress. */
        if(hDevice->bAutobaudInProgress == true)
        {
            return(ADI_UART_DEVICE_IN_USE);
        }

        /* Make sure we are transmitting. */
        if(ADI_UART_DIR_RECEIVE == hDevice->eDirection)
        {
            return(ADI_UART_OPERATION_NOT_ALLOWED);
        }

        /* Check for the data transfer mode (only allowed in blocking mode). */
        if(hDevice->pChannelTx->eDataTranferMode == ADI_UART_DATA_TRANSFER_MODE_NONBLOCKING)
        {
            return(ADI_UART_INVALID_DATA_TRANSFER_MODE);
        }

        /* Check that there is a free buffer to use for this transmit operation. "pFreeBuffer"
          is the next buffer available, so if it is in use we can make the assumption that
          there are no buffers available. The start address is set to NULL once the buffer
          has been processed.
        */
        if(hDevice->pChannelTx->pFreeBuffer->pStartAddress != NULL)
        {
            return(ADI_UART_OPERATION_NOT_ALLOWED);
        }

        /* Make sure the DMA transfer size is not too large. */
        if((bDMA == true) && (nBufSize > DMA_TRANSFER_LIMIT))
        {
            return(ADI_UART_INVALID_DATA_SIZE);
        }

#endif /* ADI_DEBUG */

        /* Set the data transfer mode in case it was #ADI_UART_DATA_TRANSFER_MODE_NONE. */
        hDevice->pChannelTx->eDataTranferMode = ADI_UART_DATA_TRANSFER_MODE_BLOCKING;

        /* Set the start address of the data buffer we are going to submit. */
        hDevice->pChannelTx->pFreeBuffer->pStartAddress  =   pBuffer;

        /* Set the buffer size to the size of the data buffer passed down from the API. */
        hDevice->pChannelTx->pFreeBuffer->nCount         =   nBufSize;

        /* Initialize the buffer index to zero because we will start shifting out
           the Tx data from the first position of the buffer.
        */
        hDevice->pChannelTx->pFreeBuffer->nIndex         =   0U;

        /* Mark the buffer as in use so no other transactions can use it until this one is complete. */
        hDevice->pChannelTx->pFreeBuffer->bInUse         =   true;

        /* Mark the DMA as in use. */
        hDevice->pChannelTx->pFreeBuffer->bDMA           =   bDMA;

        /* Now that this "pFreeBuffer" is no longer free for use, update the
           "pFreeBuffer" to the other PingPong buffer. Because there are only two
           buffers in the PingPong structure, this will be the opposite of the one
           we just submitted. "pFreeBuffer" will only be updated during the process of
           submitting a buffer or a read/write operation.
        */
        hDevice->pChannelTx->pFreeBuffer = hDevice->pChannelTx->pFreeBuffer->pNextBuffer;

        hDevice->pChannelTx->pfSubmitBuffer(hDevice, hDevice->pChannelTx->pFillBuffer);

        /* Block for the active buffer to complete. */
        return(uart_PendForBuffer(hDevice, hDevice->pChannelTx, pHwError));
}

/*!
 * @brief       Submit the buffer for reading the data in #ADI_UART_DATA_TRANSFER_MODE_BLOCKING. Call to this function will not
 *              return until the entire buffer is filled up. Returns error if this function is called when
 *              device is operating in #ADI_UART_DATA_TRANSFER_MODE_NONBLOCKING. i.e The function "adi_uart_SubmitRxBuffer()" is called
 *              when the transfer is not yet complete.
 *
 * @param [in]  hDevice    Device handle obtained from adi_uart_Open().
 * @param [in]  pBuffer    Pointer to buffer from that will be filled by the driver when data has been received.
 * @param [in]  nBufSize   Size of the buffer(in bytes). Must be smaller than 1024 bytes for DMA transfers.
 * @param [in]  bDMA       Submit the buffer using DMA flag.
 * @param [out] pHwError   Pointer to an integer that correlates with #ADI_UART_HW_ERRORS, containg the hardware status.
 *                         If there is no hardware event, this will be 0.
 *
 * @return      Status
 *    - #ADI_UART_SUCCESS                        Successfully submitted the buffer for receiving data.
 *    - #ADI_UART_HW_ERROR_DETECTED              Hardware error(s) detected. "pHwError" can be checked for the specific error code(s).
 *    - #ADI_UART_FAILED                     [D] Generic failure. In this case the size of the data buffer we are trying
 *                                               to submit is NULL.
 *    - #ADI_UART_INVALID_DATA_TRANSFER_MODE [D] Device is operating in #ADI_UART_DATA_TRANSFER_MODE_NONBLOCKING. This
 *                                               operation is only allowed in #ADI_UART_DATA_TRANSFER_MODE_BLOCKING.
 *    - #ADI_UART_INVALID_HANDLE             [D] Invalid UART device handle.
 *    - #ADI_UART_OPERATION_NOT_ALLOWED      [D] Device direction is set up as #ADI_UART_DIR_TRANSMIT, so we can not complete
 *                                               a receive operation. The required directions are #ADI_UART_DIR_RECEIVE or
 *                                               #ADI_UART_DIR_BIDIRECTION.
 *    - #ADI_UART_INVALID_POINTER            [D] Pointer to the buffer being submitted is NULL.
 *    - #ADI_UART_DEVICE_IN_USE              [D] Autobaud in progress.
 *    - #ADI_UART_INVALID_DATA_SIZE          [D] DMA transfers must be smaller than 1025 bytes.
 *
 * @sa  adi_uart_Write()
 * @sa  adi_uart_SubmitTxBuffer()
 *
 * @note: This function is a blocking function which means that the function returns only after the completion of
 *        data receive.
*/
ADI_UART_RESULT adi_uart_Read(
                              ADI_UART_HANDLE const hDevice,
                              void           *const pBuffer,
                              uint32_t        const nBufSize,
                              bool            const bDMA,
                              uint32_t             *pHwError
                             )
{

#ifdef ADI_DEBUG
     /* Validate the given handle. */
     if(ValidateHandle(hDevice) != ADI_UART_SUCCESS)
     {
         return(ADI_UART_INVALID_HANDLE);
     }

    /* Validate the pointer to the buffer memory. */
    if(pBuffer == NULL)
    {
        return(ADI_UART_INVALID_POINTER);
    }

    /* Validate the buffer size. */
    if(nBufSize == 0U )
    {
        return(ADI_UART_FAILED);
    }

    /* Autobaud in progress. */
    if(hDevice->bAutobaudInProgress == true)
    {
        return(ADI_UART_DEVICE_IN_USE);
    }

    /* Make sure the UART device is configured to operate in the receive direction. */
    if(ADI_UART_DIR_TRANSMIT == hDevice->eDirection)
    {
        return(ADI_UART_OPERATION_NOT_ALLOWED);
    }

    /* Check for the data transfer mode(only allowed in blocking mode).*/
    if(hDevice->pChannelRx->eDataTranferMode == ADI_UART_DATA_TRANSFER_MODE_NONBLOCKING)
    {
        return(ADI_UART_INVALID_DATA_TRANSFER_MODE);
    }

    /* Check that there is a free buffer to use for this receive operation. "pFreeBuffer"
       is the next buffer available, so if it is in use we can make the assumption that
       there are no buffers available. The start address gets set to NULL once the buffer
       processing has completed.
    */
    if(hDevice->pChannelRx->pFreeBuffer->pStartAddress != NULL)
    {
        return(ADI_UART_OPERATION_NOT_ALLOWED);
    }

    /* Make sure the DMA transfer size is not too large. */
    if((bDMA == true) && (nBufSize > DMA_TRANSFER_LIMIT))
    {
        return(ADI_UART_INVALID_DATA_SIZE);
    }

#endif /* ADI_DEBUG */

    /* Set the data transfer mode in case it was #ADI_UART_DATA_TRANSFER_MODE_NONE.
       This will be set back to #ADI_UART_DATA_TRANSFER_MODE_NONE once this
       transaction is complete.
    */
    hDevice->pChannelRx->eDataTranferMode = ADI_UART_DATA_TRANSFER_MODE_BLOCKING;

    /* Set the start address of the buffer you are going to submit. */
    hDevice->pChannelRx->pFreeBuffer->pStartAddress  =  pBuffer;

    /* Set the size of the buffer. */
    hDevice->pChannelRx->pFreeBuffer->nCount         =  nBufSize;

    /* Initialize the buffer index to 0, because as we receive data it will be put into
       the buffer starting at the first position.
    */
    hDevice->pChannelRx->pFreeBuffer->nIndex         =  0U;

    /* Mark the buffer as in use. */
    hDevice->pChannelRx->pFreeBuffer->bInUse         =  true;

    /* Mark the DMA as in use. */
    hDevice->pChannelRx->pFreeBuffer->bDMA           =  bDMA;


    /* Now that this "pFreeBuffer" is no longer free for use, update the
       "pFreeBuffer" to the other PingPong buffer. Because there are only two
       buffers in the PingPong structure, this will be the opposite of the one
       we just submitted. "pFreeBuffer" will only be updated during the process of
       submitting a buffer or a read/write operation.
    */
    hDevice->pChannelRx->pFreeBuffer = hDevice->pChannelRx->pFreeBuffer->pNextBuffer;

    hDevice->pChannelRx->pfSubmitBuffer(hDevice, hDevice->pChannelRx->pFillBuffer);

    /* Block for the active buffer to complete. */
    return(uart_PendForBuffer(hDevice, hDevice->pChannelRx, pHwError));
}

/*! \cond PRIVATE */

/*
 * @brief       Pends for data transaction to complete. Buffer gets returned to API.
 *
 * @param [in]  hDevice    Device handle obtained from adi_uart_Open().
 * @param [in]  pChannel   Pointer to UART channel data structure.
 * @param [out] pBuffer    Address of buffer on which data transfer being carried out.
 * @param [out] pHwError   Pointer to an integer that correlates with #ADI_UART_HW_ERRORS, containg the hardware status.
 *                         If there is no hardware event, this will be 0.
 *
 * @return      Status
 *    - #ADI_UART_SUCCESS            Successfully got buffer.
 *    - #ADI_UART_HW_ERROR_DETECTED  Hardware error(s) detected. "pHwError" can be checked for the specific error code(s).
 *
*/
static ADI_UART_RESULT uart_PendForBuffer(
                                           ADI_UART_HANDLE        const hDevice,
                                           ADI_UART_DATA_CHANNEL       *pChannel,
                                           uint32_t                    *pHwError
                                          )
{

    /* Wait until the peripheral has finished processing the buffer. */
    SEM_PEND(pChannel,ADI_UART_FAILED);

     /* Reinitialize the start address to NULL so this buffer can be used for a new transaction. */
     pChannel->pActiveBuffer->pStartAddress = NULL;

      /* Now that the desired data has either been transmitted or received, this buffer is no longer
         in use. We can update "pActiveBuffer" to point to the next buffer that will become or is already
         active. This will only be updated in places where transactions are completed,
         such as uart_PendForBuffer() and uart_GetBuffer().
      */
       pChannel->pActiveBuffer = pChannel->pActiveBuffer->pNextBuffer;

     /* Set the data transfer mode to none so that the next transfer can be either in blocking or in nonblocking mode.
        Only if there are no active buffers.
     */
     if(pChannel->pActiveBuffer->pStartAddress == NULL)
     {
        pChannel->eDataTranferMode = ADI_UART_DATA_TRANSFER_MODE_NONE;
     }

      /* If there are hardware errors and no callback, then return failure. */
      if(hDevice->nHwError != 0u)
      {
          /* Save the hardware error detected. This will be passed back to the API. */
          *pHwError = hDevice->nHwError;

          /* Clear any hardware errors detected. */
          hDevice->nHwError = 0u;

          return(ADI_UART_HW_ERROR_DETECTED);
      }
      else
      {
          return(ADI_UART_SUCCESS);
      }

}
/*! \endcond */


/*!
 * @brief       Peek function to know if an empty buffer is avilable.
 *
 * @param [in]  hDevice      Device handle obtained from adi_uart_Open().
 * @param [out] pbAvailable  Pointer to a boolean variable. Contains "true" if there is an empty buffer
 *                           and a call to "adi_uart_GetTxBuffer" is ensured to be successful. Contains
 *                           "false" if there is no empty buffer.
 * @return      Status
 *    - #ADI_UART_SUCCESS                   Successfully retrieved the status of availability of the buffer.
 *    - #ADI_UART_INVALID_HANDLE        [D] Invalid UART device handle.
 *    - #ADI_UART_OPERATION_NOT_ALLOWED [D] Call to this function is not allowed in #ADI_UART_DATA_TRANSFER_MODE_BLOCKING.
 *
 * @sa  adi_uart_GetTxBuffer()
 * @sa  adi_uart_IsRxBufferAvailable
 *
 */

ADI_UART_RESULT adi_uart_IsTxBufferAvailable(
                                              ADI_UART_HANDLE const hDevice,
                                              bool           *const pbAvailable
                                            )
{

#ifdef ADI_DEBUG
    /* Validate the given handle. */
    if(ValidateHandle(hDevice) != ADI_UART_SUCCESS)
    {
        return(ADI_UART_INVALID_HANDLE);
    }

    /* This function is only allowed to be called when the channel is operating in NONBLOCKING mode. */
    if(hDevice->pChannelTx->eDataTranferMode ==  ADI_UART_DATA_TRANSFER_MODE_BLOCKING)
    {
        return(ADI_UART_OPERATION_NOT_ALLOWED);
    }
#endif /* ADI_DEBUG */

    /* Initialize to "false" in case of an error. */
    *pbAvailable = false;

    /* Make sure the buffer has not already been processed. This would mean that there are
       currently no active buffers. This is only updated in adi_uart_GetBuffer(), which is
       called once a transaction has completed.
    */
    if (hDevice->pChannelTx->pActiveBuffer->pStartAddress != NULL)
    {
        /* If the buffer has reached the interrupt handler, "bInUse" will be
           updated so we know that the buffer has become available.
        */
        if (hDevice->pChannelTx->pActiveBuffer->bInUse == false)
        {
            *pbAvailable = true;
        }
    }
    return(ADI_UART_SUCCESS);
}

/*!
 * @brief       Peek function to know if a filled buffer is available.
 *
 *
 * @param [in]  hDevice      Device handle obtained from adi_uart_Open().
 * @param [out] pbAvailable  Pointer to a boolean variable. Contains "true" if there is an empty buffer
 *                           and a call to "adi_uart_GetTxBuffer" is ensured to be successful. Contains
 *                           "false" if there is no empty buffer.
 * @return      Status
 *    - #ADI_UART_SUCCESS                   Successfully retrieved the status of availability of the buffer.
 *    - #ADI_UART_INVALID_HANDLE        [D] Invalid UART device handle.
 *    - #ADI_UART_OPERATION_NOT_ALLOWED [D] Call to this function is not allowed in #ADI_UART_DATA_TRANSFER_MODE_BLOCKING.
 *
 * @sa  adi_uart_GetRxBuffer()
 *
 */
ADI_UART_RESULT    adi_uart_IsRxBufferAvailable(
                                                 ADI_UART_HANDLE const hDevice,
                                                 bool           *const pbAvailable
                                                )
{

#ifdef ADI_DEBUG
    /* Validate the given handle. */
    if(ValidateHandle(hDevice) != ADI_UART_SUCCESS)
    {
        return(ADI_UART_INVALID_HANDLE);
    }

    /* This function is only allowed to be called when the channel is operating in NONBLOCKING mode. */
    if(hDevice->pChannelRx->eDataTranferMode ==  ADI_UART_DATA_TRANSFER_MODE_BLOCKING)
    {
        return(ADI_UART_OPERATION_NOT_ALLOWED);
    }
#endif /* ADI_DEBUG */

    /* Initialize to "false" in case of an error. */
    *pbAvailable = false;

    /* Make sure the buffer has not already been processed. This would mean that there are
       currently no active buffers. This is only updated in adi_uart_GetBuffer(), which is
       called once a transaction has completed.
    */
    if(hDevice->pChannelRx->pActiveBuffer->pStartAddress != NULL)
    {
        /* If the buffer has reached the interrupt handler, "bInUse" will be
           updated so we know that the buffer has become available.
        */
        if (hDevice->pChannelRx->pActiveBuffer->bInUse == false)
        {
            *pbAvailable = true;
        }
    }
    return(ADI_UART_SUCCESS);
}

/*!
 * @brief       Function to let the API know if all the data had been drained from the Tx shift registers.
 *
 * @param [in]  hDevice      Device handle obtained from adi_uart_Open().
 * @param [out] pbComplete   Pointer to a boolean variable. Contains "true" if there is no data left in the
 *                           device to transmit and device can be disabled without data loss. Contains "false"
 *                           if the data transmission is not complete.
 * @return      Status
 *    - #ADI_UART_SUCCESS            Successfully retrieved the status of data transmission.
 *    - #ADI_UART_INVALID_HANDLE [D] Specified handle is invalid.
 *
 * @note  adi_uart_getTxBuffer() or the callback may indicate that a transmit transaction is complete when the
 *        device is using the DMA. This is because the interrupt will trigger once the transmit holding register is empty.
          However, there may still be a some data in the shift register. If the transmit channel needs
 *        to be closed then the application must poll the transmit channel to see if all data has indeed been transmitted before
 *        shutting down the channel. Otherwise data will be lost.
 *
 */

ADI_UART_RESULT adi_uart_IsTxComplete(
                                     ADI_UART_HANDLE const hDevice,
                                     bool           *const pbComplete
                                     )
{
#ifdef ADI_DEBUG
    /* Validate the given handle. */
    if(ValidateHandle(hDevice) != ADI_UART_SUCCESS)
    {
        return(ADI_UART_INVALID_HANDLE);
    }
#endif /* ADI_DEBUG */

    /* Initialize to false. */
    *pbComplete = false;

    /* If the register is empty, set the return variable to "true".
       This register is empty, when the value becomes a 1.
    */
    if((hDevice->pUARTRegs->LSR & BITM_UART_LSR_TEMT) == BITM_UART_LSR_TEMT)
    {
        *pbComplete = true;
    }
    return(ADI_UART_SUCCESS);
}


/*!
 * @brief       Registering a callback function.
 *
 * @param [in]  hDevice       Device handle obtained from adi_uart_Open().
 * @param [in]  pfCallback    Function pointer to callback. Passing a NULL pointer will unregister
 *                            the callback function.
 * @param [in]  pCBParam      Callback function parameter.
 *
 * @return      Status
 *    - #ADI_UART_SUCCESS                Successfully registered callback function.
 *    - #ADI_UART_DEVICE_IN_USE      [D] This operation is not allowed when a data transfer is in progress.
 *    - #ADI_UART_INVALID_HANDLE     [D] Invalid UART device handle.
 *
 *
*/
ADI_UART_RESULT adi_uart_RegisterCallback(
                                          ADI_UART_HANDLE const hDevice,
                                          const ADI_CALLBACK    pfCallback,
                                          void           *const pCBParam
                                         )
{

#ifdef ADI_DEBUG
    /* Validate the given handle. */
    if(ValidateHandle(hDevice) != ADI_UART_SUCCESS)
    {
        return(ADI_UART_INVALID_HANDLE);
    }

    /* Make sure there are no active buffers on any active channel and autobaud is not in progress. */
    if(((hDevice->eDirection != ADI_UART_DIR_TRANSMIT) && (hDevice->pChannelRx->pActiveBuffer->pStartAddress != NULL)) ||
       ((hDevice->eDirection != ADI_UART_DIR_RECEIVE ) && (hDevice->pChannelTx->pActiveBuffer->pStartAddress != NULL)) ||
       (hDevice->bAutobaudInProgress == true))
    {
        return(ADI_UART_DEVICE_IN_USE);
    }
#endif /* ADI_DEBUG */

    /* Set the device callback. */
    hDevice->pfCallback = pfCallback;

    /* Set the callback parameter. */
    hDevice->pCBParam = pCBParam;

    return(ADI_UART_SUCCESS);
}


/*!
 * @brief        Configuration of UART data.
 *
 * @details      Sets the configuration parameters for the specified UART device such as wordlength, whether to
 *               enable/disable the parity, and the number of stop bits. This function returns an error if the
 *               device has active data or autobaud is in progress.
 *
 * @param [in]  hDevice      Device handle obtained from adi_uart_Open().
 * @param [in]  eParity      Specify the type of parity check for the UART device.
 * @param [in]  eStopBits    Specify the stop-bits for the UART device.
 * @param [in]  eWordLength  Specify the word size of the data for the UART device.
 *
 * @return      Status
 *    - #ADI_UART_SUCCESS                Successfully set the data configuration.
 *    - #ADI_UART_DEVICE_IN_USE      [D] This operation is not allowed when a data transfer or autobaud is in progress.
 *    - #ADI_UART_INVALID_HANDLE     [D] Invalid UART device handle.
 *
*/
ADI_UART_RESULT adi_uart_SetConfiguration(
                                         ADI_UART_HANDLE   const hDevice,
                                         ADI_UART_PARITY   const eParity,
                                         ADI_UART_STOPBITS const eStopBits,
                                         ADI_UART_WORDLEN  const eWordLength
                                         )
{
#ifdef ADI_DEBUG
    /* Validate the given handle. */
    if(ValidateHandle(hDevice) != ADI_UART_SUCCESS)
    {
        return(ADI_UART_INVALID_HANDLE);
    }

    /* Make sure there are no active buffers on any active channel and autobaud is not in progress. */
    if(((hDevice->eDirection != ADI_UART_DIR_TRANSMIT) && (hDevice->pChannelRx->pActiveBuffer->pStartAddress != NULL)) ||
       ((hDevice->eDirection != ADI_UART_DIR_RECEIVE ) && (hDevice->pChannelTx->pActiveBuffer->pStartAddress != NULL)) ||
       (hDevice->bAutobaudInProgress == true))
    {
        return(ADI_UART_DEVICE_IN_USE);
    }
#endif /* ADI_DEBUG */

    /* Clear all the fields.  */
    uint16_t nDataCfg = hDevice->pUARTRegs->LCR & (uint16_t)(~(BITM_UART_LCR_WLS |BITM_UART_LCR_STOP |BITM_UART_LCR_PEN));

    /* Construct the configuration word. */
    nDataCfg |= (uint16_t)(((uint16_t)((uint16_t)eWordLength |(uint16_t)eStopBits) |(uint16_t)eParity));

    /* Write to the register */
    hDevice->pUARTRegs->LCR = nDataCfg;

    /* Return Success */
    return(ADI_UART_SUCCESS);
}

/*!
 * @brief        Set baudrate by configuring the fractional dividors.
 *
 * @details      Baudrate is calculated as per below equation.
 *
 *               Baudrate = (UARTCLK / (nDivM + nDivN/2048)*pow(2,nOSR+2)* nDivC)).
 *
 * @param [in]  hDevice      Device handle obtained from adi_uart_Open().
 * @param [in]  nDivC        Specify the "nDivC" in the above equation.
 * @param [in]  nDivM        Specify the "nDivM" in the above equation.
 * @param [in]  nDivN        Specify the "nDivN" in the above equation.
 * @param [in]  nOSR         Specify the "nOSR" " in the above equation.
 *
 * @return      Status
 *    - #ADI_UART_SUCCESS                 Successfully set the baudrate for the device.
 *    - #ADI_UART_INVALID_HANDLE     [D]  Invalid UART device handle.
 *    - #ADI_UART_DEVICE_IN_USE      [D]  Device is in use
 *    - #ADI_UART_INVALID_PARAMETER  [D]  Input for baud rate values are out of range.
 *
 *   @sa  adi_uart_GetBaudRate()
 *   @sa  adi_uart_EnableAutobaud();
 *
 *   @note It is expected that initialization of the power management
 *         driver is done before calling this function.
 *
 */
ADI_UART_RESULT adi_uart_ConfigBaudRate(
                                       ADI_UART_HANDLE const hDevice,
                                       uint16_t        const nDivC,
                                       uint8_t         const nDivM,
                                       uint16_t        const nDivN,
                                       uint8_t         const nOSR
                                       )
{
#ifdef ADI_DEBUG
    /* Validate the given handle */
    if(ValidateHandle(hDevice) != ADI_UART_SUCCESS)
    {
        return(ADI_UART_INVALID_HANDLE);
    }

    /* Make sure there are no active buffers on any active channel. */
    if(((hDevice->eDirection != ADI_UART_DIR_TRANSMIT) && (hDevice->pChannelRx->pActiveBuffer->pStartAddress != NULL)) ||
       ((hDevice->eDirection != ADI_UART_DIR_RECEIVE ) && (hDevice->pChannelTx->pActiveBuffer->pStartAddress != NULL)))
    {
        return(ADI_UART_DEVICE_IN_USE);
    }

    /* Check if the given baudrate is valid */
    if( (nDivM < 1u) || (nDivM > 3u)|| (nDivN > 2047u ) || (nOSR > 3u))
    {
        return ADI_UART_INVALID_PARAMETER;
    }

#endif /* ADI_DEBUG */

    /* Write back the register contents for baudrate detection in the hardware. */
    hDevice->pUARTRegs->DIV  =  nDivC;
    hDevice->pUARTRegs->FBR  =  (uint16_t)((uint16_t)nDivN | (uint16_t)((uint16_t)nDivM <<BITP_UART_FBR_DIVM)) | (uint16_t)BITM_UART_FBR_FBEN;
    hDevice->pUARTRegs->LCR2 = nOSR;

    return(ADI_UART_SUCCESS);
}


/*!
 * @brief       Get the baudrate of the UART device instance. This is used in the scenario when a callback has not been initialized.
 *              This allows the the API to know if autobaud is complete. If this returns a baudrate other than 0,
 *              it indicates that the autobaud completed, otherwise autobaud is still in progress.
 *
 * @param [in]   hDevice         Device handle obtained from adi_uart_Open().
 * @param [out]  pnBaudRate      Pointer to a location where baudrate is to be written.
 * @param [out]  pAutobaudError  Pointer to an integer that will hold the value of any baudrate error(s), that correlates with
 *                               #ADI_UART_AUTOBAUD_ERRORS. This will be 0 if there are no errors.
 *
 * @return      Status
 *    - #ADI_UART_SUCCESS                   Successfully retrieved the baudrate.
 *    - #ADI_UART_AUTOBAUD_ERROR_DETECTED   There has been an autobaud error. The API can get the specific error(s)
 *                                          by checking "pAutobaudError".
 *    - #ADI_UART_INVALID_HANDLE        [D] Invalid UART device handle.
 *    - #ADI_UART_INVALID_POINTER       [D] The pointer to baudrate or autobaud error is NULL.

 *
*/
ADI_UART_RESULT adi_uart_GetBaudRate(
                                      ADI_UART_HANDLE const     hDevice,
                                      uint32_t                 *pnBaudRate,
                                      uint32_t                 *pAutobaudError
                                    )
{

#ifdef ADI_DEBUG
    /* Validate the given handle. */
    if(ValidateHandle(hDevice) != ADI_UART_SUCCESS)
    {
        return(ADI_UART_INVALID_HANDLE);
    }

    /* Validate pointers. */
    if(pnBaudRate == NULL)
    {
        return(ADI_UART_INVALID_POINTER);
    }

#endif /* ADI_DEBUG */

    /* If an error occured during autobaud this value will be set to a
       non-zero value. The specific error can be found by checking against
       #ADI_UART_EVENT.
    */
    if(hDevice->nAutobaudError != 0u)
    {
      /* Save the autobaud error to pass back to the API.*/
      *pAutobaudError = hDevice->nAutobaudError;

      /* Clear the autobaud errors found. */
      hDevice->nAutobaudError = 0u;

      return(ADI_UART_AUTOBAUD_ERROR_DETECTED);
    }

    /* Return the baudrate. If this is 0, then autobaud has not completed. */
    *pnBaudRate = hDevice->nBaudRate;

    return(ADI_UART_SUCCESS);
}


/*!
 * @brief       Enable/Disable UART autobaud detection as well as configures the device for autobaud detection.
 *
 * @details     The baud rate is detected using the hardware support.
 *              After the baud rate is detected the interrupt handler is notified of the completion.
 *              When a callback is not registered with UART driver, the API adi_uart_GetBaudRate()
 *              can be used to know if autobaud is complete. Autobaud needs to be disabled in order to
 *              clear the internal counter and to close the device.
 *
 * @param [in]  hDevice                 Handle to UART device whose autobaud detection to be enabled/disabled.
 * @param [in]  bEnable                 Boolean flag to indicate whether to enable or disable the autobaud.
 * @param [in]  bAutobaudCallbackMode   Use a callback to report autobaud errors or type #ADI_UART_AUTOBAUD_ERRORS.
 *
 * @return      Status
 *    - #ADI_UART_SUCCESS                       Successfully enabled/disabled Autobaud detection.
 *    - #ADI_UART_DEVICE_IN_USE             [D] Trying to enable/disable Autobaud when
 *                                              dataflow is enabled or autobaud is in progress.
 *    - #ADI_UART_INVALID_HANDLE            [D] Invalid UART device handle.
 *
 * @sa  adi_uart_GetBaudRate()
 *
 * @note: For autobaud we assume the key character being used is a carrige return (0xD), so the start edge count is
 *        hardcoded to the second edge (first edge after start edge) and the last edge count is set to the fouth edge.
 *        This will give us a total bit count of 8 bits that we will time in order to figure out the baud rate (bits/second).
 */
ADI_UART_RESULT adi_uart_EnableAutobaud(
                                        ADI_UART_HANDLE          const hDevice,
                                        bool                     const bEnable,
                                        bool                     const bAutobaudCallbackMode
                                       )
{

#ifdef ADI_DEBUG
     /* Validate the given handle */
    if(ValidateHandle(hDevice) != ADI_UART_SUCCESS)
    {
        return(ADI_UART_INVALID_HANDLE);
    }

    /* Make sure there are no active buffers on any active channel and autobaud is not in progress. */
    if(((hDevice->eDirection != ADI_UART_DIR_TRANSMIT) && (hDevice->pChannelRx->pActiveBuffer->pStartAddress != NULL)) ||
       ((hDevice->eDirection != ADI_UART_DIR_RECEIVE ) && (hDevice->pChannelTx->pActiveBuffer->pStartAddress != NULL)))
    {
        return(ADI_UART_DEVICE_IN_USE);
    }

#endif /* ADI_DEBUG */

    if(bEnable)
    {
        /* Enable Autobaud, timeout interrupt and done interrupt in the autobaud control register.
           Set the starting edge trigger to the second edge. Set the ending edge count to
           the fourth edge, for the carrige return key character (0xD).
        */
        hDevice->pUARTRegs->ACR |=(BITM_UART_ACR_ABE | BITM_UART_ACR_DNIEN | BITM_UART_ACR_TOIEN |(1u << 4u) | (3u << 8u));

        /* Initialize device baudrate to 0. This will be set once autobaud is complete. */
        hDevice->nBaudRate = 0u;

        /* Change the state to indicate autobaud is in progress. */
        hDevice->bAutobaudInProgress = true;

        /* Set the callback mode for autobaud based on the user input. */
        hDevice->bAutobaudCallbackMode = bAutobaudCallbackMode;
    }
    else
    {
        /* Change the state to indicate autobaud is not in progress. */
        hDevice->bAutobaudInProgress = false;

        /* Disable Autobaud, timeout interrupt and done interrupt in the autobaud control register. */
        hDevice->pUARTRegs->ACR |= (uint16_t)(~(uint32_t)BITM_UART_ACR_ABE | ~(uint32_t)BITM_UART_ACR_DNIEN | ~(uint32_t)BITM_UART_ACR_TOIEN);

        /* Initialize device baudrate to 0. */
        hDevice->nBaudRate = 0u;
    }

    return ADI_UART_SUCCESS;
}

/*!
 * @brief       Forces the UART to send out a break signal.
 *
 * @details     Sets the UART Tx pin to a logic-low/high (depending upon the
 *              Tx polarity) asynchronously. The UART keeps transmitting break
 *              until it is disabled to send the break.
 *
 * @param [in]  hDevice             Handle to the UART whose Tx is forced to
 *                                  send a break.
 * @param [in]  bEnable             Flag which indicates whether to enable or
 *                                  disable transmitting the break.
 *
 * @return      Status
 *
 * - #ADI_UART_SUCCESS              If successfully enabled or disabled sending break.
 * - #ADI_UART_INVALID_HANDLE   [D] If the given UART handle is invalid.
 */
ADI_UART_RESULT adi_uart_ForceTxBreak(
                                      ADI_UART_HANDLE         const hDevice,
                                      bool                    const bEnable
                                     )
{

#ifdef ADI_DEBUG
     /* Validate the given handle. */
    if(ValidateHandle(hDevice) != ADI_UART_SUCCESS)
    {
        return(ADI_UART_INVALID_HANDLE);
    }
#endif /* ADI_DEBUG */

    if(bEnable == true)
    {
        /* Set the force break bit. */
        hDevice->pUARTRegs->LCR    |= BITM_UART_LCR_BRK;
    }
    else
    {
        /* Clear the force break bit. */
        hDevice->pUARTRegs->LCR    &= (uint16_t)~(BITM_UART_LCR_BRK);
    }

    return ADI_UART_SUCCESS;
}

/*!
 * @brief       Enable/Disable the loopback for the specified UART device.
 *
 * @param [in]  hDevice    Device handle obtained from adi_uart_Open().
 * @param [in]  bEnable    Boolean flag to indicate whether to enable or disable the loopback mode.
 *
 * @return      Status
 *    - #ADI_UART_SUCCESS            Successfully enable/disable the loopback.
 *    - #ADI_UART_INVALID_HANDLE     Invalid  UART device handle.
 *
*/
ADI_UART_RESULT adi_uart_EnableLoopBack(
                                        ADI_UART_HANDLE const hDevice,
                                        bool            const bEnable
                                       )
{

#ifdef ADI_DEBUG
     /* Validate the given handle. */
    if(ValidateHandle(hDevice) != ADI_UART_SUCCESS)
    {
        return(ADI_UART_INVALID_HANDLE);
    }
#endif /* ADI_DEBUG */

    if(true == bEnable)
    {
        /* Enable loopback. */
        hDevice->pUARTRegs->MCR |= (BITM_UART_MCR_LOOPBACK);
    }
    else
    {
        /* Disable loopback. */
        hDevice->pUARTRegs->MCR &= (uint16_t)~(BITM_UART_MCR_LOOPBACK);
    }
    return(ADI_UART_SUCCESS);
}

/*!
 * @brief       Sets the RX FIFO trigger level. This will be the amount of data in the FIFO
 *              that will trigger an interrupt.
 *
 *
 * @param [in]  hDevice         Device handle obtained from adi_uart_Open().
 * @param [in]  eTriglevel      Trigger level to be set in terms of number of bytes.
 *
 * @return      Status
 *  - #ADI_UART_SUCCESS             Successfully set the trigger level.
 *  - #ADI_UART_INVALID_HANDLE  [D] The given UART handle is invalid.
 */
ADI_UART_RESULT adi_uart_SetRxFifoTriggerLevel(
                                               ADI_UART_CONST_HANDLE         const hDevice,
                                               ADI_UART_TRIG_LEVEL           const eTriglevel
                                              )
{
#ifdef ADI_DEBUG
     /* Validate the given handle. */
    if(ValidateHandle(hDevice) != ADI_UART_SUCCESS)
    {
        return(ADI_UART_INVALID_HANDLE);
    }
#endif /* ADI_DEBUG */

    /* Clear existing FIFO trigger level. */
    hDevice->pUARTRegs->FCR &= (uint16_t)~BITM_UART_FCR_RFTRIG;

    /* Set the FIFO trigger level. */
    hDevice->pUARTRegs->FCR |= (uint16_t)eTriglevel;

    return(ADI_UART_SUCCESS);
}
/*!
 * @brief       Enables internal FIFO as to work in 16550 mode. This helps to minimize system overhead
 *              and maximize system efficiency.
 *
 * @param [in]  hDevice    Device handle obtained from adi_uart_Open().
 * @param [in]  bEnable    Boolean flag to indicate whether to enable or disable FIFO.
 *
 * @return      Status
 *  - #ADI_UART_SUCCESS              If successfully enabled FIFO for UART device.
 *  - #ADI_UART_INVALID_HANDLE   [D] The given UART handle is invalid.
 */
ADI_UART_RESULT adi_uart_EnableFifo(
                                     ADI_UART_HANDLE         const hDevice,
                                     bool                    const bEnable
                                    )
{

#ifdef ADI_DEBUG
     /* Validate the given handle. */
    if(ValidateHandle(hDevice) != ADI_UART_SUCCESS)
    {
        return(ADI_UART_INVALID_HANDLE);
    }
#endif /* ADI_DEBUG */

    if(bEnable == true)
    {
       /* Enable TX/RX FIFO. */
       hDevice->pUARTRegs->FCR    |=  BITM_UART_FCR_FIFOEN;
       hDevice->pUARTRegs->IEN |= (BITM_UART_IEN_ERBFI);

       hDevice->bRxFifoEn = true;

    }
    else
    {
       /* Disable TX/RX FIFO. */
       hDevice->pUARTRegs->FCR    &= (uint16_t)~(BITM_UART_FCR_FIFOEN);

       hDevice->bRxFifoEn = false;
    }

    return ADI_UART_SUCCESS;
}

/*!
 * @brief      To flush the TX FIFO.
 *
 *
 * @param [in]  hDevice    Device handle obtained from adi_uart_Open().
 *
 *
 * @return      Status
 *  - #ADI_UART_SUCCESS             Successfully flushed TX Fifo.
 *  - #ADI_UART_INVALID_HANDLE  [D] The given UART handle is invalid.
 */
ADI_UART_RESULT adi_uart_FlushTxFifo(
                                     ADI_UART_CONST_HANDLE         const hDevice
                                    )
{

#ifdef ADI_DEBUG
     /* Validate the given handle. */
    if(ValidateHandle(hDevice) != ADI_UART_SUCCESS)
    {
        return(ADI_UART_INVALID_HANDLE);
    }
#endif /* ADI_DEBUG */

    /* Flush the Tx FIFO. */
    hDevice->pUARTRegs->FCR    |=  BITM_UART_FCR_TFCLR;

    return(ADI_UART_SUCCESS);
}

/*!
 * @brief       Flush the RX FIFO.
 *
 *
 * @param [in]  hDevice    Device handle obtained from adi_uart_Open().
 *
 *
 * @return      Status
 *  - #ADI_UART_SUCCESS              Successfully flushed RX Fifo.
 *  - #ADI_UART_INVALID_HANDLE   [D] The given UART handle is invalid.
 */
ADI_UART_RESULT adi_uart_FlushRxFifo(
                                      ADI_UART_CONST_HANDLE         const hDevice
                                     )
{

#ifdef ADI_DEBUG
     /* Validate the given handle. */
    if(ValidateHandle(hDevice) != ADI_UART_SUCCESS)
    {
        return(ADI_UART_INVALID_HANDLE);
    }
#endif /* ADI_DEBUG */

    /* Flush RX FIFO. */
    hDevice->pUARTRegs->FCR    |=  BITM_UART_FCR_RFCLR;

    return ADI_UART_SUCCESS;
}

/*!
 * @brief      Flush the Rx channel and disable interrupts. This will stop any buffers in flight and
 *             clear out any data that was in the RX holding register as well as the Rx fifo. Once this is done,
 *             in order to turn back on Rx interrupts, a new transaction will need to be started (adi_uart_Read()
 *             or adi_uart_SubmitRxBuffer()).
 *
 *
 * @param [in]  hDevice    Device handle obtained from adi_uart_Open().
 *
 * @return      Status
 *  - #ADI_UART_SUCCESS                       Successfully flushed the Rx channel.
 *  - #ADI_UART_INVALID_HANDLE            [D] The given UART handle is invalid.
 */
ADI_UART_RESULT adi_uart_FlushRxChannel(
                                         ADI_UART_CONST_HANDLE  const hDevice
                                        )
{

#ifdef ADI_DEBUG
     /* Validate the given handle. */
    if(ValidateHandle(hDevice) != ADI_UART_SUCCESS)
    {
        return(ADI_UART_INVALID_HANDLE);
    }
#endif /* ADI_DEBUG */

    /* Disable receive interrupts in PIO mode as well as DMA mode. */
    hDevice->pUARTRegs->IEN &= (uint16_t)~(BITM_UART_IEN_ERBFI | BITM_UART_IEN_EDMAR);

    /* Clear any data in the Rx Fifo. */
    hDevice->pUARTRegs->FCR |=  BITM_UART_FCR_RFCLR;

    /* Reset the buffers to 0. */
    memset(hDevice->pChannelRx->PingPong,0, sizeof (hDevice->pChannelRx->PingPong));

    hDevice->pChannelRx->PingPong[0].pNextBuffer  = &hDevice->pChannelRx->PingPong[1];
    hDevice->pChannelRx->PingPong[1].pNextBuffer  = &hDevice->pChannelRx->PingPong[0];

    /* Reset the buffer pointers. */
    hDevice->pChannelRx->pActiveBuffer = &hDevice->pChannelRx->PingPong[0];
    hDevice->pChannelRx->pFreeBuffer = &hDevice->pChannelRx->PingPong[0];
    hDevice->pChannelRx->pFillBuffer = &hDevice->pChannelRx->PingPong[0];

    /* Dummy read to flush the RX register. */
    hDevice->pUARTRegs->RX;

    return(ADI_UART_SUCCESS);
}

/*!
 * @brief      Flush the Tx channel and disable interrupts.This will stop any buffers in flight and
 *             clear out any data that was in the TX holding register. Any data in the TX shift register
 *             will still finish transmitting.
 *
 *
 * @param [in]  hDevice       Device handle to UART device obtained when an UART device is opened successfully.
 *
 * @return      Status
 *  - #ADI_UART_SUCCESS                   Successfully flushed the Tx channel.
 *  - #ADI_UART_INVALID_HANDLE        [D] The given UART handle is invalid.
 */
ADI_UART_RESULT adi_uart_FlushTxChannel(ADI_UART_CONST_HANDLE  const hDevice)
{

#ifdef ADI_DEBUG
     /* Validate the given handle. */
    if(ValidateHandle(hDevice) != ADI_UART_SUCCESS)
    {
        return(ADI_UART_INVALID_HANDLE);
    }
#endif /* ADI_DEBUG */

    /* Disable transmit interrupts in PIO mode as well as DMA mode. */
    hDevice->pUARTRegs->IEN &= (uint16_t)~(BITM_UART_IEN_ETBEI | BITM_UART_IEN_EDMAT);

    /* Clear any data in the Rx Fifo. */
    hDevice->pUARTRegs->FCR |=  BITM_UART_FCR_TFCLR;

    /* Reset the buffers to 0. */
    memset(hDevice->pChannelTx->PingPong,0, sizeof (hDevice->pChannelTx->PingPong));

    hDevice->pChannelTx->PingPong[0].pNextBuffer  = &hDevice->pChannelTx->PingPong[1];
    hDevice->pChannelTx->PingPong[1].pNextBuffer  = &hDevice->pChannelTx->PingPong[0];

    /* Reset the buffer pointers. */
    hDevice->pChannelTx->pActiveBuffer = &hDevice->pChannelTx->PingPong[0];
    hDevice->pChannelTx->pFreeBuffer = &hDevice->pChannelTx->PingPong[0];
    hDevice->pChannelTx->pFillBuffer = &hDevice->pChannelTx->PingPong[0];

    return(ADI_UART_SUCCESS);
}


/*! \cond PRIVATE */

void UART0_Int_Handler(void)
{
    ISR_PROLOG();
    ADI_UART_HANDLE hDevice = (ADI_UART_HANDLE)uart_device_info[0].hDevice;
    Common_Uart_Interrupt_Handler(hDevice);
#if defined(ADI_CYCLECOUNT_UART_ISR_ENABLED) && (ADI_CYCLECOUNT_UART_ISR_ENABLED == 1u)
    ADI_CYCLECOUNT_STORE(ADI_CYCLECOUNT_ISR_UART);    
#endif
    ISR_EPILOG();
    return;
}

void UART1_Int_Handler(void)
{
    ISR_PROLOG();
    ADI_UART_HANDLE hDevice = (ADI_UART_HANDLE)uart_device_info[1].hDevice;
    Common_Uart_Interrupt_Handler(hDevice);
#if defined(ADI_CYCLECOUNT_UART_ISR_ENABLED) && (ADI_CYCLECOUNT_UART_ISR_ENABLED == 1u)
    ADI_CYCLECOUNT_STORE(ADI_CYCLECOUNT_ISR_UART);    
#endif
    ISR_EPILOG();
    return;
}

static void Common_Uart_Interrupt_Handler(ADI_UART_HANDLE hDevice)
{
      switch(hDevice->pUARTRegs->IIR & BITM_UART_IIR_STAT )
    {
        /* Tx buffer empty interrupt. This means that the data has successfully left the holding register and is
           now in transmit shift register or has completed its transfer.
        */
        case ENUM_UART_IIR_STAT_ETBEI:
          uart_TxDataHandler(hDevice);
          break;

        /* Rx buffer FIFO timeout interrupt. This means that we have data in the RX FIFO
           but there is not enough data to trigger an interrupt so we will process this data here.
        */
        case ENUM_UART_IIR_STAT_RFTOI:
          uart_RxDataHandler(hDevice);
          break;

        /* Rx buffer full interrupt. This means that the RX buffer has finished receiving data. */
        case ENUM_UART_IIR_STAT_ERBFI:
          uart_RxDataHandler(hDevice);
          break;

        /* Line status interrupt. */
        case ENUM_UART_IIR_STAT_RLSI:
        {
            /* Initialze the line status event to 0. */
            uint32_t nEvent = 0u;

            /* Get the interrupts status. */
            uint16_t  nStatus = hDevice->pUARTRegs->LSR;

            /* If a break signal is detected.. */
            if((BITM_UART_LSR_BI & nStatus) == BITM_UART_LSR_BI)
            {
                /* Dummy read to flush the RX register. We do this because
                   we do not actaully want to do anything with this data as it
                   is only a break indicator. */
                hDevice->pUARTRegs->RX;

                /* Set the event to a break interrupt. */
                nEvent  = (uint32_t)ADI_UART_BREAK_INTERRUPT;
            }

            /* Ignore the framing error if the break is asserted.
               We do this because a break can trigger a false framing error.
            */
            else if((BITM_UART_LSR_FE & nStatus) == BITM_UART_LSR_FE)
            {
                /* Set the event to show a framing error has been detected. */
                nEvent |= (uint32_t)ADI_UART_HW_ERR_FRAMING;
            }
            else
            {
              /* Do nothing. This is required for MISRA. */
            }

            if((BITM_UART_LSR_PE & nStatus) == BITM_UART_LSR_PE)
            {
                /* Set the event to show a parity error has been detected. */
                nEvent  |=  (uint32_t)ADI_UART_HW_ERR_PARITY;
            }
            if((BITM_UART_LSR_OE & nStatus) == BITM_UART_LSR_OE)
            {
                /* Set the event to show a hardware overrun error has been detected, meaning receive data has
                   been overwritten.
                */
                nEvent |= (uint32_t)ADI_UART_HW_ERR_OVERRUN;
            }

            /* If there was an event and autobaud is not in progress, notify the API. */
            if((nEvent != 0u) && (hDevice->bAutobaudInProgress == false))
            {
                /* Set the UART device hw error bit field. This will allow us to return the
                   specific failure to the application once we return from this ISR.
                */
                hDevice->nHwError |= nEvent;
                uart_ManageProcessedBuffer(hDevice, hDevice->pChannelRx, ADI_UART_EVENT_HW_ERROR_DETECTED);
            }
            break;
         }

         /* If there was a modem status interrupt. For our purposes, we will only check if this is related to autobaud. */
         case ENUM_UART_IIR_STAT_EDSSI:
         {
#if (ADI_UART_CFG_ENABLE_AUTOBAUD == 1)
            /* Initialize the autobaud event to 0. */
            uint32_t nEvent = 0u;

            /* Get the autobaud interrupt status but not the counter value. */
            uint16_t nStatus =  hDevice->pUARTRegs->ASRL & 0xFu;

            /* Read the autobaud control register to see if autobaud was enabled. */
            uint16_t  acr = (hDevice->pUARTRegs->ACR & BITM_UART_ACR_ABE);

            /* If there is an autobaud event and autobaud is enabled */
            if((nStatus != 0u) && (acr != 0u))
            {
                uint32_t nClock;
                uint32_t nCount;

                /*Get the clock frequency. */
                if(adi_pwr_GetClockFrequency(ADI_CLOCK_PCLK,&nClock) != ADI_PWR_SUCCESS)
                {
                  nClock = 0u;
                }

                /* Get the autobaud counter bits 12-19. */
                nCount = (uint32_t)hDevice->pUARTRegs->ASRH << 12u;

                /* Get the autobaud counter bits 0-11. */
                nCount |= (uint32_t)hDevice->pUARTRegs->ASRL >> 4u;

                /* if the autobaud event was that the autobaud is done.. */
                if((nStatus & BITM_UART_ASRL_DONE) == BITM_UART_ASRL_DONE)
                {
                    /* If the fractional baud generator is enabled, calculate the fractional portional of the baudrate.
                       It seems that in order to get a correct baudrate reading, we need the fractional divider enabled.
                    */
                    if ((hDevice->pUARTRegs->FBR & 0x8000u) == 0x8000u)
                    {
                        uint8_t nOSR = 0u;
                        uint32_t nDivN;
                        uint32_t nDivNSubtractor = 2048u;

                        /* DIVC is always 1, unless the oversample rate is 32. */
                        uint16_t nDivC = 1u;

                        /* If the oversample rate is 4.. */
                        if(nCount < (8u << 3u))
                        {
                             nDivN = ((nCount << 9u) / 8u) - nDivNSubtractor;
                        }

                        /* If the oversample rate is 8.. */
                        else if(nCount < (8u << 4u))
                        {
                             nDivN = ((nCount << 8u) / 8u) - nDivNSubtractor;
                             nOSR = 1u;
                        }

                        /* If the oversample rate is 16.. */
                        else if(nCount < (8u << 5u))
                        {
                             nDivN = ((nCount << 7u) / 8u) - nDivNSubtractor;
                             nOSR = 2u;
                        }

                        /* If the oversample rate is 32.. */
                        else
                        {
                            nDivC = (uint16_t) (nCount / 32u / 8u);
                            nDivN = ((nCount << 6u) / (8u * nDivC)) - nDivNSubtractor;
                            nOSR = 3u;
                        }

                        /* Write back the register contents for baudrate detection in the hardware. */
                        adi_uart_ConfigBaudRate(hDevice, nDivC, 1u, (uint16_t)nDivN, nOSR);

                        /* For more precise calculations we would use floating point math here. Integer precision will do for now.
                           This avoids bringing in extra libraries for floating point math. */

                        /* Baudrate = (UARTCLK / (nDivM + nDivN / 2048) * pow(2, nOSR + 2) * nDivC)
                        nOSR = (1u << (nOSR + 2u)); Seperate this out of the equation for misra compliance
                        hDevice->nBaudRate = ((float)nClock / (((float)1 + (float)nDivN / (float)2048) * (float)nOSR * (float)nDivC));
                        */

                        /* In order to avoid bringing in the extra floating point libraries, we will use the non fractional baudrate for the API. */
                        hDevice->nBaudRate = ((nClock * 8u) / nCount);
                    }
                    else
                    {
                      /* No Fractional divider: Baudrate (bits/second) = (UARTCLK (cycles/second) * counted bits (bits)) / nCount (cycles)*/
                      hDevice->nBaudRate = ((nClock * 8u) / nCount);
                    }

                    /* If there is a callback, notify the API that autobaud is complete.
                       If there is not a callback, the baudrate will be set to a non zero value so the user can call "Get_BaudRate"
                       to know that autobaud has completed.
                    */
                    if((hDevice->pfCallback != NULL) && (hDevice->bAutobaudCallbackMode == true))
                    {
                        hDevice->pfCallback(hDevice->pCBParam, ADI_UART_EVENT_AUTOBAUD_COMPLETE, (void*)hDevice->nBaudRate);
                    }
                }
                else
                {
                    if((nStatus & BITM_UART_ASRL_BRKTO) == BITM_UART_ASRL_BRKTO)
                    {
                        /* Autobaud timed out due to break error. */
                       nEvent |= (uint32_t)ADI_UART_AUTOBAUD_TIMEOUT_LONGBREAK;
                    }
                    if((nStatus & BITM_UART_ASRL_NSETO) == BITM_UART_ASRL_NSETO)
                    {
                        /* Autobaud timed out due to no valid start edge found. */
                        nEvent |= (uint32_t)ADI_UART_AUTOBAUD_TIMEOUT_NO_START_EDGE;
                    }
                    if((nStatus & BITM_UART_ASRL_NEETO) == BITM_UART_ASRL_NEETO)
                    {
                        /* Autobaud timed out due to no valid end edge found. */
                        nEvent |= (uint32_t)ADI_UART_AUTOBAUD_TIMEOUT_NO_END_EDGE;
                    }
                    /* If there is an event callback.. */
                    if((hDevice->pfCallback != NULL) && (hDevice->pChannelRx->eDataTranferMode == ADI_UART_DATA_TRANSFER_MODE_NONBLOCKING))
                    {
                      /* Notify application of errors through callback. */
                      hDevice->pfCallback(hDevice->pCBParam, ADI_UART_EVENT_AUTOBAUD_ERROR_DETECTED, (void*)nEvent);
                    }
                    else
                    {
                      /* Notify application of errors through autobaud return value. */
                      hDevice->nAutobaudError = nEvent;
                    }

                }

                /* Dummy read to flush the RX register to clear the key character that was sent while configuring autobaud. */
                hDevice->pUARTRegs->RX;
            }
#endif
            /* Clear auto baud enable and interrupt registers. We disable autobaud here because it is required in order to clear the counter.  */
             hDevice->pUARTRegs->ACR &=(uint16_t)~( BITM_UART_ACR_ABE   |
                                                       BITM_UART_ACR_DNIEN |
                                                       BITM_UART_ACR_TOIEN );

             hDevice->bAutobaudInProgress = false;
             break;
         }
        default:
          break;
    }
    return;
}


/* DMA interrupt handlers */
void DMA_UART0_TX_Int_Handler(void)
{
   ISR_PROLOG();
   ADI_UART_HANDLE const hDevice = (ADI_UART_HANDLE)uart_device_info[0].hDevice;
   uart_ManageProcessedBuffer(hDevice,hDevice->pChannelTx,ADI_UART_EVENT_TX_BUFFER_PROCESSED);
#if defined(ADI_CYCLECOUNT_UART_ISR_ENABLED) && (ADI_CYCLECOUNT_UART_ISR_ENABLED == 1u)
    ADI_CYCLECOUNT_STORE(ADI_CYCLECOUNT_ISR_DMA_UART_TX);    
#endif
   ISR_EPILOG();
}

void DMA_UART0_RX_Int_Handler(void)
{
   ISR_PROLOG();
   ADI_UART_HANDLE const hDevice = (ADI_UART_HANDLE)uart_device_info[0].hDevice;
   uart_ManageProcessedBuffer(hDevice,hDevice->pChannelRx,ADI_UART_EVENT_RX_BUFFER_PROCESSED);
#if defined(ADI_CYCLECOUNT_UART_ISR_ENABLED) && (ADI_CYCLECOUNT_UART_ISR_ENABLED == 1u)
    ADI_CYCLECOUNT_STORE(ADI_CYCLECOUNT_ISR_DMA_UART_RX);    
#endif
   ISR_EPILOG();
}

void DMA_UART1_TX_Int_Handler(void)
{
   ISR_PROLOG();
   ADI_UART_HANDLE const hDevice = (ADI_UART_HANDLE)uart_device_info[1].hDevice;
   uart_ManageProcessedBuffer(hDevice,hDevice->pChannelTx,ADI_UART_EVENT_TX_BUFFER_PROCESSED);
#if defined(ADI_CYCLECOUNT_UART_ISR_ENABLED) && (ADI_CYCLECOUNT_UART_ISR_ENABLED == 1u)
    ADI_CYCLECOUNT_STORE(ADI_CYCLECOUNT_ISR_DMA_UART_TX);    
#endif
   ISR_EPILOG();
}

void DMA_UART1_RX_Int_Handler(void)
{
   ISR_PROLOG();
   ADI_UART_HANDLE const hDevice = (ADI_UART_HANDLE)uart_device_info[1].hDevice;
   uart_ManageProcessedBuffer(hDevice,hDevice->pChannelRx,ADI_UART_EVENT_RX_BUFFER_PROCESSED);
#if defined(ADI_CYCLECOUNT_UART_ISR_ENABLED) && (ADI_CYCLECOUNT_UART_ISR_ENABLED == 1u)
    ADI_CYCLECOUNT_STORE(ADI_CYCLECOUNT_ISR_DMA_UART_RX);    
#endif
   ISR_EPILOG();
}


/*
 * @brief                  UART interrupt handler for receiving the data in interrupt mode.
 *
 * @param [in]  hDevice   Device handle obtained from adi_uart_Open().
 *
*/
static void uart_RxDataHandler(ADI_UART_HANDLE hDevice)
{
    volatile uint8_t *pNextData;

    /* If there is an active buffer.. */
    if((hDevice->pChannelRx->pFillBuffer->pStartAddress != NULL) && (hDevice->pChannelRx->pFillBuffer->bInUse == true))
    {
           /* Get the address of the buffer we are filling. */
           pNextData = (uint8_t *)hDevice->pChannelRx->pFillBuffer->pStartAddress;

           /* Read data from the RX holding register into the buffer at the indexed location. */
           pNextData[hDevice->pChannelRx->pFillBuffer->nIndex] = (uint8_t) hDevice->pUARTRegs->RX;

           /* Increment the buffer index so we don't overwrite this data in the buffer. */
           hDevice->pChannelRx->pFillBuffer->nIndex++;

           /* If all of the data has been processed, manage the processed data buffer. Otherwise we will
              leave everything as is and continue to receive interrupts for the incoming data, until this
              buffer has been filled.
           */
           if(hDevice->pChannelRx->pFillBuffer->nIndex == hDevice->pChannelRx->pFillBuffer->nCount)
           {
                  uart_ManageProcessedBuffer(hDevice, hDevice->pChannelRx, ADI_UART_EVENT_RX_BUFFER_PROCESSED);
           }
    }
    /* If we do not have a buffer submitted.. */
    else
    {
          /* Ask the API for a buffer so we can process this data before having an overflow.
             if there is no callback, the API will not be able to submit a buffer in time.
          */
          if (hDevice->pfCallback != NULL)
          {
              hDevice->pfCallback(hDevice->pCBParam, (uint32_t)ADI_UART_EVENT_NO_RX_BUFFER_EVENT, NULL);
          }

          /* This check here is in case in the callback the application submitted a buffer. If they did
             not then we need to clear the RX register in order to clear this interrupt.
          */
          if((hDevice->pChannelRx->pFillBuffer->pStartAddress == NULL) && (hDevice->pChannelRx->pFillBuffer->bInUse == false))
          {
              hDevice->pUARTRegs->RX;
          }
    }

    return;
}

/*
 * @brief      UART interrupt handler transmitting the data in interrupt mode.
 *
 * @param [in]  hDevice   Device handle obtained from adi_uart_Open().
 *
*/
static void uart_TxDataHandler(ADI_UART_HANDLE hDevice)
{
      volatile uint8_t *pNextData;

    /* If there is an active buffer.. */
   if((hDevice->pChannelTx->pFillBuffer->pStartAddress != NULL) && (hDevice->pChannelTx->pFillBuffer->bInUse == true))
    {
           /* Get the start address of the buffer we are transmitting data from. */
           pNextData = (uint8_t *)hDevice->pChannelTx->pFillBuffer->pStartAddress;

           /* Write data to the TX holding register. This will be shifted out at the baud rate by the shift register. */
           hDevice->pUARTRegs->TX = (uint16_t)pNextData[hDevice->pChannelTx->pFillBuffer->nIndex];

           /* Increment the buffer index. */
          hDevice->pChannelTx->pFillBuffer->nIndex++;


           /* If all of the characters have been transmitted, manage the data buffer. Otherwise we will leave everything
              as is and continue to transmit this data until everything is out of the buffer. */
           if(hDevice->pChannelTx->pFillBuffer->nIndex >= hDevice->pChannelTx->pFillBuffer->nCount)
           {
                  uart_ManageProcessedBuffer(hDevice,hDevice->pChannelTx,ADI_UART_EVENT_TX_BUFFER_PROCESSED);
           }
    }
    return;
}


/*
 * @brief       Function for managing the processed buffer. This gets called after the receive buffer has been filled
 *              and when the transmit buffer has been emptied.
 *
 * @param [in]  hDevice    Device handle obtained from adi_uart_Open().
 * @param [in]  pChannel   Channel handler for the Tx or Rx.
 * @param [in]  eEvent     Indicate the event ID to be passed to registered callback function, if one has been registered.
 *
*/

static void uart_ManageProcessedBuffer(ADI_UART_HANDLE hDevice,ADI_UART_DATA_CHANNEL *pChannel, ADI_UART_EVENT eEvent)
{


    /* Now that this transaction has completed, this buffer is no longer in use. */
    pChannel->pFillBuffer->bInUse = false;

    pChannel->pFillBuffer = pChannel->pFillBuffer->pNextBuffer;

    if(eEvent == ADI_UART_EVENT_TX_BUFFER_PROCESSED)
    {
        /* Disable Tx buffer interrupts. */
        hDevice->pUARTRegs->IEN &= (uint16_t)~(BITM_UART_IEN_ETBEI | BITM_UART_IEN_EDMAT);
    }
    else
    {
        /* Disable Rx buffer interrupts for the DMA. We do not disable receive buffer full interrupts to allow
           the use of the RX FIFO.
        */
        hDevice->pUARTRegs->IEN &= (uint16_t)~(BITM_UART_IEN_EDMAR);

        if (hDevice->bRxFifoEn != true)
        {
            /* Disable Rx buffer interrupts for PIO mode if the FIFO is not enabled.
            */
            hDevice->pUARTRegs->IEN &= (uint16_t)~(BITM_UART_IEN_ERBFI);
        }

    }

    /* If there is a callback registered, notify the API that a buffer has been processed. Clean up the buffer. */
    if((hDevice->pfCallback != NULL) && (pChannel->eDataTranferMode == ADI_UART_DATA_TRANSFER_MODE_NONBLOCKING))
    {
        uint32_t nEvent = hDevice->nHwError;
        hDevice->nHwError = 0u;

        uint32_t *pBuffer = pChannel->pActiveBuffer->pStartAddress;

        /* Reinitialize the start address to NULL so this buffer can be used for a new transaction. */
        pChannel->pActiveBuffer->pStartAddress = NULL;

        /* Now that the desired data has either been transmitted or received, this buffer is no longer
           in use. We can update "pActiveBuffer" to point to the next buffer that will become or is already
           active.
        */
         pChannel->pActiveBuffer = pChannel->pActiveBuffer->pNextBuffer;

        /* Set the data transfer mode to none so that the next transfer can be either in blocking or in nonblocking mode.
           This will only be done if there are no other active buffers in flight to avoid disrupting an active transfer.
        */
        if(pChannel->pActiveBuffer->pStartAddress == NULL)
        {
            pChannel->eDataTranferMode = ADI_UART_DATA_TRANSFER_MODE_NONE;
        }
        if(nEvent != 0u)
        {
           hDevice->pfCallback(hDevice->pCBParam, ADI_UART_EVENT_HW_ERROR_DETECTED,(void*)nEvent);

        }
        else
        {
            hDevice->pfCallback(hDevice->pCBParam, (uint32_t)eEvent, (void*)pBuffer);
        }

    }
    else
    {
        /* Post to the blocking function. If we are in blocking mode, this will allow the buffer to be returned to the API.
           If we are in nonblocking mode, this will allow adi_uart_GetBuffer() to return immediately so the API can have
           control over the buffer again.
        */
        SEM_POST(pChannel);
    }

    /* If there is another buffer active. The buffer we want to check is "pFillBuffer" because that is the next one that would
       be processed. So if it has been submitted, now would be the time to set up the interrupts based on its requirements.
    */
    if(pChannel->pFillBuffer->bInUse == true)
    {
        pChannel->pfSubmitBuffer(hDevice, pChannel->pFillBuffer);
    }
}


/*
 * @brief       Initialize the UART instance to the default values specified in "adi_uart_config.h".
 *
 * @param [in]  hDevice    Device handle obtained from adi_uart_Open().
 * @param [in]  nDeviceNum  UART device number
*/

static void uart_init(ADI_UART_CONST_HANDLE const hDevice, uint32_t const nDeviceNum)
{

    ADI_UART_CONFIG const* pUARTCfg = &gUARTCfg[nDeviceNum];

    /* Line Control Register. */
    hDevice->pUARTRegs->LCR = pUARTCfg->LCR;

    /* Div-C in Baudrate divider register. */
    hDevice->pUARTRegs->DIV = pUARTCfg->DIV;

    /* Div-M and Div-N in Fractional Baudrate register. */
    hDevice->pUARTRegs->FBR = pUARTCfg->FBR;

    /* Second line control register. */
    hDevice->pUARTRegs->LCR2 = pUARTCfg->LCR2;

    /* FIFO control register. */
    hDevice->pUARTRegs->FCR  = pUARTCfg->FCR;

    /* Half Duplex Control Register. */
    hDevice->pUARTRegs->RSC  = pUARTCfg->RSC;

    /* Interrupt enable register. */
    hDevice->pUARTRegs->IEN  = pUARTCfg->IEN;
}

#ifdef ADI_DEBUG
/*
 * @brief       Validate the device handle.
 *
 * @param [in]  hDevice    Device handle obtained from adi_uart_Open().
 *
 * @return      Status
 *    - #ADI_UART_SUCCESS          Specified handle is valid.
 *    - #ADI_UART_INVALID_HANDLE   Specified handle is invalid.
 *
*/

static ADI_UART_RESULT ValidateHandle(ADI_UART_CONST_HANDLE hDevice)
{
    uint32_t i;


    for(i = 0U; i <  ADI_UART_NUM_DEVICES; i++)
    {

      if((hDevice == uart_device_info[i].hDevice) && (hDevice != NULL))
       {
           return(ADI_UART_SUCCESS);
       }
    }
    return(ADI_UART_INVALID_HANDLE);
}
#endif /* ADI_DEBUG */
/*! \endcond */
/*@}*/
