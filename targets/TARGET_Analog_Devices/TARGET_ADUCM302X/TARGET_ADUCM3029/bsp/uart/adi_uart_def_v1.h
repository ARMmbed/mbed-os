/*! *****************************************************************************
 * @file:    adi_uart_def_v1.h
 * @brief:   UART Device Driver definition for  processor
 * @version: $Revision: 35030 $
 * @date:    $Date: 2016-07-05 05:49:46 +0100 (Tue, 05 Jul 2016) $
 -----------------------------------------------------------------------------
Copyright (c) 2010-2014 Analog Devices, Inc.

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

#ifndef _DEF_UART_DEF_V1_H_
#define _DEF_UART_DEF_V1_H_
/*! \cond PRIVATE */
#define  ADI_UART_RBR_GET()        (pDevice->pUARTRegs->COMRX)
#define  ADI_UART_THR_SET(Value)   (pDevice->pUARTRegs->COMTX =(uint16_t) (Value))

#define  ADI_UART_DISABLE_TX()     (pDevice->pUARTRegs->COMIEN &= (uint16_t)~(BITM_UART_COMIEN_ETBEI))
#define  ADI_UART_ENABLE_TX()      (pDevice->pUARTRegs->COMIEN |= (BITM_UART_COMIEN_ETBEI))

#define  ADI_UART_DISABLE_RX()     (pDevice->pUARTRegs->COMIEN &= (uint16_t)~BITM_UART_COMIEN_ERBFI)
#define  ADI_UART_ENABLE_RX()      (pDevice->pUARTRegs->COMIEN |= (BITM_UART_COMIEN_ERBFI))

#define ENUMM_UART_IIR_MODEM_STAT      0x0
#define ENUMM_UART_IIR_TXBUF_EMPTY     0x2
#define ENUMM_UART_IIR_RXBUF_FULL      0x4
#define ENUMM_UART_IIR_LINE_STAT       0x6
#define ENUMM_UART_IIR_RXFIFO_TIMEOUT  0xC

#define AUTO_BAUD_INTERRUPT_STATUS  ( BITM_UART_COMASRL_DONE  | \
                                     BITM_UART_COMASRL_BRKTO  | \
                                     BITM_UART_COMASRL_NSETO  | \
                                     BITM_UART_COMASRL_NEETO)

#define AUTO_BAUD_HIGHER_BIT_POSITION  12

#define AUTO_BAUD_LOWER_BIT_POSITION  4

#define AUTO_BAUD_EVENT_BIT_POSITION  9

#define DEFAULT_NUM_BITS 1u
/*! Default IER register configuration. */
#define DEFAULT_IER_CONFIG1  (ENUM_UART_IER_ELSI_EN)

/*! Number of descriptors to manage the buffers*/
#define ADI_NUM_UART_BUFFER    3U

#define MAX_WAIT_TIME 0XFFFFFFFFu

/*!
 * \enum  CHANNEL_DATAFLOW_STATUS
 *
 *!  Enumeration of channel data flow status
 */
typedef enum
{
    CHANNEL_STATE_DATA_FLOW_DISABLED,   /*!< data flow is disabled for the channel*/

    CHANNEL_STATE_DATA_FLOW_ENABLED,    /*!< data flow is enabled for the channel*/

    CHANNEL_STATE_DATA_FLOW_PAUSED      /*!< data flow is paused due to no availability  of buffer*/

}CHANNEL_DATAFLOW_STATUS;

typedef struct _ADI_UART_DATA_CHANNEL* DATA_CHANNEL_PTR;

/*! Function pointer typedef for the function which  closes the uart device */
typedef ADI_UART_RESULT (*UART_CLOSE) (uint32_t nChannelNum);

/*! Function pointer typedef for the function which  submit the buffer */
typedef ADI_UART_RESULT (*UART_BUFFER_SUBMIT) (DATA_CHANNEL_PTR pChannel,void *const pBuffer, uint32_t nSize, ADI_DMA_TRANSFER *pDMADesc);

/*! Function prototype for enabling DMA associated with the device*/
typedef ADI_UART_RESULT (*UART_DMA_ENABLE)(DATA_CHANNEL_PTR pChannel,ADI_DMA_TRANSFER *pDMADesc,ADI_DMA_CHANNEL_ID eDMAChannel,bool_t bEnable);

/*! typedef  of a function which enable/disable the specified channel */
typedef ADI_UART_RESULT (*UART_ENABLE) (ADI_UART_HANDLE hDevice,bool_t bEnable);

/*!
 * \struct _ADI_UART_BUFF_INFO
 *
 *  structure for  managing the submitted  the buffers.
 */
typedef struct UART_BUFF_INFO
{
    void    *pStartAddress;              /*!< pointer to a buffer*/

    uint32_t nCount;                     /*!< Size of the buffer in bytes */

    uint32_t nIndex;                     /*!< variable for indexing the next word for placing in FIFO */

    volatile bool_t  bInUse;                     /*!< useage flag */

    struct UART_BUFF_INFO  *pNext;  /*!< Pointer to next descriptor */

}ADI_UART_BUFF_INFO;
/*!
 * \enum  ADI_UART_TRANSFER_MODE
 *
 *  enumeration for Data transfer mode.
 */
typedef enum
{
    /*! Mode of data transfer is not selected.*/
    ADI_UART_DATA_TRANSFER_MODE_NONE,

    /*! Blocking mode. Only call to adi_uart_Read/adi_uart_write is allowed.
    Complete data of specified size is transfered before returning from the function. */
    ADI_UART_DATA_TRANSFER_MODE_BLOCKING,

    /*!Non-Blocking mode. Only call to adi_uart_SubmitRxBuffer/adi_uart_SubmitTxBuffer is allowed.
    Data transfer happens in the background and notified by posting a semaphore.*/
    ADI_UART_DATA_TRANSFER_MODE_NONBLOCKING

} ADI_UART_TRANSFER_MODE;

/*!
 * \struct _UART_DATA_CHANNEL
 *
 *  structure to manage the data transfer for a given channel.
 *  One instance of this  structure will be created for managing the
 *  data transfer in each direction.
 */
typedef struct _ADI_UART_DATA_CHANNEL
{
    uint8_t                  *pBuffer;               

    ADI_UART_BUFF_INFO       oBufferInfo[ADI_NUM_UART_BUFFER];  /*!< Number of buffer nodes */

    ADI_UART_BUFF_INFO       *pCurrDesc;        /*!<  Pointer to the current buffer info */

    ADI_UART_BUFF_INFO       *pProcDesc;        /*!<  Pointer to the queue of the processed Buffers */

    ADI_UART_BUFF_INFO       *pFreeDesc;        /*!<  Pointer to the queue of the free descriptor*/

    ADI_UART_TRANSFER_MODE    eDataTranferMode; /*!<  Stores the Data transfer mode */

    uint32_t                  nActiveBufferCount;/*!<  Active buffer count */

    ADI_DMA_CHANNEL_ID        eDMAChannel;      /*!<  UART DMA channel ID */
    
    CHANNEL_DATAFLOW_STATUS  eChannelStatus;     /*!<  data flow status of the channel */
    
    UART_BUFFER_SUBMIT      pfSubmitBuffer;   /*!< Function pointer for sumbiting the frame */
    
    UART_DMA_ENABLE         pfEnableDMA;      /*!< Function pointer for enabling DMA */    
    
    bool_t                 bDMAMode;            /*!<  Flag to indicate the DMA mode */
    
    uint8_t                   nGetbufferCount;    /*!<  Counter to track the number of getbuffer call.*/
#if (ADI_UART_CFG_ENABLE_DMA_SUPPORT == 1)
    void                     *pDMAmemory;
    ADI_DMA_CHANNEL_HANDLE    hDma; 
    ADI_DMA_TRANSFER          oDMADescriptor;
#endif
    PEND_VAR_DECLR    
}UART_DATA_CHANNEL;


/*!
 * \enum  ADI_UART_STATE
 *
 *  Enumeration of different UART states
 */
typedef enum
{
    ADI_UART_STATE_UNINITIALIZED = 0,    /*!< UART is not yet initialized */

    ADI_UART_STATE_INITIALIZED,          /*!< UART is initialized */

    ADI_UART_STATE_DATA_FLOW_ENABLED,    /*!< UART Tx  OR Rx data flow is enabled */

    ADI_UART_STATE_DATA_FLOW_DISABLED,   /*!< UART Tx  OR Rx data flow is disabled */

    ADI_UART_STATE_AUTOBAUD_IN_PROGRESS  /*!< Auto baud in progress */    

}ADI_UART_STATE;

/*!
 * \struct  ADI_UART_DEVICE_INFO
 *
 *  Structre for storing the basic device information such as state, address, interrupt ID etc.
 *
 */
typedef struct 
{

    ADI_UART_STATE               eState;     /*!<  To indicate the state of the device */
    
    IRQn_Type                   eDMATXIRqType;  /*!<  TX-DMA interrupt IRQ */
    
    IRQn_Type                   eDMARXIRqType;   /*!<  RX-DMA interrupt IRQ */  
    
    ADI_DMA_CHANNEL_ID          eDMATx;  /*!<  Peripheral map for TX */
    
    ADI_DMA_CHANNEL_ID          eDMARx;  /*!<  Peripheral map for RX */    

    ADI_UART_TypeDef             *pUartRegs;  /*!<  Base address of the  UART registers */
    
    ADI_UART_HANDLE              hDevice;    /*!<  Handle for the instance */

}ADI_UART_DEVICE_INFO;

/*!
 * \struct  _ADI_UART_DEVICE
 *  Structre for managing the UART device. This structure contain the data for
 *  managing the UART device.
 */
#pragma pack(4)
typedef struct _ADI_UART_DEVICE
{
    ADI_UART_DIRECTION     eDirection;       /*!< Direction in which the UART is opened */

    ADI_UART_DEVICE_INFO  *pUartInfo;        /*!< pointer to the structure which stores the information about the uart instances.*/
    
    ADI_CALLBACK           pfCallback;       /*!<  Function pointer for callback function. */

    void                  *pCBParam;         /*!<  Parameter to callback function. */

    uint32_t               nBaudRate;        /*!< Baudrate of the device. */

    volatile ADI_UART_TypeDef  *pUARTRegs;   /*!< Pointer to UART Memory Mapped Registers */ 
    
    uint32_t               nNumbits;         /*!< To store the number of bits in autobaud character */  
    
    volatile uint32_t      nHwError;         /*!< To store the last error */
    
    UART_DATA_CHANNEL     *pChannelTx;       /*!< Pointer for managing the Tx channel */
    
    UART_DATA_CHANNEL     *pChannelRx;       /*!< Pointer for managing the Rx channel */
    
    ADI_CALLBACK           pfEventCallback;  /*!<  Function pointer for callback function. */

    void                  *pEventCBParam;    /*!<  Parameter to callback function. */    
} ADI_UART_DEVICE;


/*! Initializes the Nodes which contains the buffer informations */
static void uart_InitChannelDescs ( uint32_t NumDesc,UART_DATA_CHANNEL *pChannel);

/*! Init function for the uart device.Initializes all the registers. */
static void uart_init(ADI_UART_DEVICE  *pDevice);

/*! Channel initialization function prototype */
static ADI_UART_RESULT uart_channel_init(ADI_UART_DEVICE  *pDevice,UART_DATA_CHANNEL *pChannel,ADI_DMA_CHANNEL_ID eDMAChannel);

/* Common get buffer function prototype. This function can be called from either Tx channel or Rx channel */
static ADI_UART_RESULT uart_getbuffer(ADI_UART_DEVICE  *pDevice,UART_DATA_CHANNEL *pChannel,void **ppBuffer);

/*! Function which pends for the compeletion of the buffer processing. */
static ADI_UART_RESULT uart_pendForBuffer(ADI_UART_DEVICE  *pDevice ,UART_DATA_CHANNEL *pChannel,void *const pBuffer,UART_ENABLE pfEnable,ADI_UART_DIRECTION eDirection);

/*! Common function for managing the buffer. This function is being called for both Tx and Rx channel . */
static void uart_mange_buffer(ADI_UART_DEVICE *pDevice,UART_DATA_CHANNEL *pChannel,ADI_UART_EVENT eEvent,void *pArg);
/*! Function prototype for the  submitting the buffer in  interrupt mode*/
static ADI_UART_RESULT uart_SubmittBufferIntmode(UART_DATA_CHANNEL *pChannel,void *const pBuffer, uint32_t nSize,ADI_DMA_TRANSFER *pDMADesc);

#if (ADI_UART_CFG_ENABLE_DMA_SUPPORT == 1)
static ADI_UART_RESULT uart_EnableDMA(UART_DATA_CHANNEL *pChannel,ADI_DMA_TRANSFER *pDMADesc,ADI_DMA_CHANNEL_ID eDMAChannel,bool_t bEnable);

/*! UART  RX DMA  handler */
static void uart_RxDMAHandler(void *pCBParam,uint32_t Event,void *pArg);

/*! UART  TX DMA  handler */
static void uart_TxDMAHandler(void *pCBParam,uint32_t Event,void *pArg);

/*! Function prototype for the  submitting the buffer in DMA mode */
static ADI_UART_RESULT uart_SubmittBufferDMAmode(UART_DATA_CHANNEL *pChannel,void *const pBuffer, uint32_t nSize,ADI_DMA_TRANSFER *pDMADesc);

#endif


/*! Function for data handling in transmit mode */
static void uart_TxDataHandler(ADI_UART_DEVICE       *pDevice);

/*! Function for data handling in receive mode */
static void uart_RxDataHandler(ADI_UART_DEVICE       *pDevice);

/*! Function for closing the RX or TX channel  */
static ADI_UART_RESULT uart_CloseChannel(UART_DATA_CHANNEL *pChannel);
/*! \endcond */
#endif	/* end of ifndef _DEF_UART_DEF_V1_H_ */


