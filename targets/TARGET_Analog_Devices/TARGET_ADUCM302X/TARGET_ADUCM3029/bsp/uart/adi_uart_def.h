/*! *****************************************************************************
 * @file:    adi_uart_def.h
 * @brief:   UART Device Driver definition for processor
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
/*! \cond PRIVATE */
#ifndef DEF_UART_DEF_H
#define DEF_UART_DEF_H

/* Macro mapping from ADuCM4x50 to ADuCM302x */
#if defined(__ADUCM302x__)

#define INTR_UART0_EVT					INTR_UART_EVT
#define UART0_Int_Handler(void)			UART_Int_Handler(void)		
#define DMA_UART0_TX_Int_Handler(void)	DMA_UART_TX_Int_Handler(void)
#define DMA_UART0_RX_Int_Handler(void)	DMA_UART_RX_Int_Handler(void)

#endif /* __ADUCM302x__ */

/*!
 *****************************************************************************
 * \struct ADI_UART_BUFF_INFO
 *  Structure for managing the submitted buffers.
 *****************************************************************************/

typedef struct UART_BUFF_INFO
{
    void                      *pStartAddress;             /*!< Address of buffer passed down to the UART driver.    */
    uint32_t                   nCount;                    /*!< Size of buffer in bytes.                             */
    uint32_t                   nIndex;                    /*!< Buffer index.                                        */   
    bool                       bInUse;                    /*!< Buffer in use flag.                                  */
    bool                       bDMA;                      /*!< Transaction is using the DMA flag.                   */
    struct UART_BUFF_INFO     *pNextBuffer;               /*!< Pointer to the next buffer in the list.              */


}ADI_UART_BUFF_INFO;


/*! Function pointer typedef for the function which  submit the buffer */
typedef void (*UART_BUFFER_SUBMIT) (ADI_UART_CONST_HANDLE  const hDevice,
                                    ADI_UART_BUFF_INFO    *const pBuffer
                                    );


/*!
 *****************************************************************************
 * \struct ADI_UART_DATA_CHANNEL
 *  Structure to manage the data transfer for a given channel.
 *  One instance of this  structure will be created for managing the
 *  data transfer in each direction.
 *****************************************************************************/
 
typedef struct _ADI_UART_DATA_CHANNEL
{ 
    ADI_UART_BUFF_INFO        PingPong[2];       /*!< Ping Pong Buffers.                                            */
    ADI_UART_BUFF_INFO       *pFreeBuffer;       /*!< Pointer to free buffer (next buffer to submit).               */  
    ADI_UART_BUFF_INFO       *pFillBuffer;       /*!< Pointer to the next buffer to be filled. This is needed for 
                                                      the case where two buffers are "submitted" before a "get" is 
                                                      called.                                                       */
    ADI_UART_BUFF_INFO       *pActiveBuffer;     /*!< Pointer to active buffer (next buffer waiting for completion).*/
    ADI_UART_TRANSFER_MODE    eDataTranferMode;  /*!< Data transfer mode.                                           */ 
    UART_BUFFER_SUBMIT        pfSubmitBuffer;    /*!< Pointer to a function used for submitting a buffer.           */
    SEM_VAR_DECLR                               

}ADI_UART_DATA_CHANNEL;


/*!
 *****************************************************************************
 * \struct  ADI_UART_DEVICE_INFO
 *  Structure for storing basic device information.
 *****************************************************************************/

typedef struct _ADI_UART_DEVICE_INFO
{
    DMA_CHANn_TypeDef      dmaTxChannelNum;    /*!<  DMA channel ID-Tx.                  */       
    DMA_CHANn_TypeDef      dmaRxChannelNum;    /*!<  DMA channel ID-Rx.                  */   
    IRQn_Type              eDMATx;             /*!<  DMA channel IRQ-Tx.                 */       
    IRQn_Type              eDMARx;             /*!<  DMA channel IRQ-Rx.                 */  
    IRQn_Type              eIRQn;              /*!<  UART interrupt ID.                  */   
    ADI_UART_TypeDef      *pUartRegs;          /*!<  Base address of the UART registers. */       
    ADI_UART_HANDLE        hDevice;            /*!<  Handle for the device instance.     */

}ADI_UART_DEVICE_INFO;


/*!
 *****************************************************************************
 * \struct  ADI_UART_DEVICE
 *  Structure for managing the UART device. 
 *****************************************************************************/

typedef struct _ADI_UART_DEVICE
{
    ADI_UART_DIRECTION          eDirection;           /*!< UART operation direction.                                       */
    ADI_UART_DEVICE_INFO       *pUartInfo;            /*!< Access to device information about the uart instance.           */
    volatile ADI_UART_TypeDef  *pUARTRegs;            /*!< Access to UART Memory Mapped Registers.                         */     
    ADI_CALLBACK                pfCallback;           /*!< Callback function.                                              */
    void                       *pCBParam;             /*!< Parameter for callback function.                                */
    bool                        bAutobaudInProgress;  /*!< Autobaud in progress flag.                                      */   
    volatile uint32_t           nHwError;             /*!< Line status error(s).                                           */
    volatile uint32_t           nAutobaudError;       /*!< Autobaud error(s).                                              */         
    ADI_UART_DATA_CHANNEL      *pChannelTx;           /*!< Tx channel.                                                     */    
    ADI_UART_DATA_CHANNEL      *pChannelRx;           /*!< Rx channel.                                                     */
    volatile uint32_t           nBaudRate;            /*!< Baudrate.                                                       */
    bool                        bAutobaudCallbackMode;/*!< Autobaud detection is using callback mode flag.                 */
    bool                        bRxFifoEn;            /*!< Rx FIFO enabled. Rx buffer full interrupts will remain enabled. */
     
} ADI_UART_DEVICE;


/*!
 *****************************************************************************
 * \struct  ADI_UART_CONFIG
 *  Structure for initializing the static config.
 *****************************************************************************/

typedef struct _ADI_UART_CONFIG
{
    uint16_t                    LCR;                 /*!< UART_COMLCR Register. */    

    uint16_t                    DIV;                 /*!< UART_COMDIV Register. */ 

    uint16_t                    FBR;                 /*!< UART_COMFBR Register. */ 

    uint16_t                    LCR2;                /*!< UART_COMLCR2 Register.*/ 

    uint16_t                    FCR;                 /*!< UART_COMFCR Register. */ 

    uint16_t                    RSC;                 /*!< UART_COMRSC Register. */              

    uint16_t                    IEN;                 /*!< UART_COMIEN Register .*/ 

} ADI_UART_CONFIG;


/******************************************************************************
 * UART Device internal API function prototypes
 *****************************************************************************/

/*
 * UART device initialization helper function.
*/
static void uart_init(ADI_UART_CONST_HANDLE const hDevice, uint32_t const nDeviceNum);


/*
 * Data transfer helper functions.
*/
static void uart_submittxbuffer(ADI_UART_CONST_HANDLE const hDevice, ADI_UART_BUFF_INFO *const pBuffer);

static void uart_submitrxbuffer(ADI_UART_CONST_HANDLE const hDevice, ADI_UART_BUFF_INFO *const pBuffer);


/*
 * Data management helper functions.
*/
static ADI_UART_RESULT uart_getbuffer(ADI_UART_HANDLE hDevice, ADI_UART_DATA_CHANNEL *pChannel, void **ppBuffer, uint32_t *pHwError);

static ADI_UART_RESULT uart_PendForBuffer(ADI_UART_HANDLE const hDevice , ADI_UART_DATA_CHANNEL *pChannel, uint32_t *pHwError);

static void uart_ManageProcessedBuffer(ADI_UART_HANDLE hDevice, ADI_UART_DATA_CHANNEL *pChannel, ADI_UART_EVENT eEvent);

static void uart_TxDataHandler(ADI_UART_HANDLE hDevice);

static void uart_RxDataHandler(ADI_UART_HANDLE hDevice);


/* 
 * Interrupt Handler.
*/
static void Common_Uart_Interrupt_Handler(ADI_UART_HANDLE hDevice);


/* 
 * Handle Validation function
*/
#ifdef ADI_DEBUG
static ADI_UART_RESULT  ValidateHandle(ADI_UART_CONST_HANDLE hDevice);
#endif /* ADI_DEBUG */

#endif  /* end of ifndef DEF_UART_DEF_H */
/*! \endcond */


