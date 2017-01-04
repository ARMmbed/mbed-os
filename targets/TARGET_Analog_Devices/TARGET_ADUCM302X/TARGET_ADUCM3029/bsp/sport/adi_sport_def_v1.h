/*!
 *****************************************************************************
 * @file:    adi_sport_def_v1.h
 * @brief:   SPORT Device Driver definition 
 * @version: $Revision: 33205 $
 * @date:    $Date: 2016-01-11 10:46:07 +0000 (Mon, 11 Jan 2016) $
 *****************************************************************************
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
#ifndef _DEF_SPORT_DEF_ADUCM302X_H_
#define _DEF_SPORT_DEF_ADUCM302X_H_
/* Number of DMA Descriptors */
/*! \cond PRIVATE */
#define NUM_SPORT_BUFFER                3

#if defined(__ECC__)
#define ALIGN4  _Pragma("align(4)")
#elif defined(__ICCARM__)
#define ALIGN4  _Pragma("pack(4)")
#elif defined (__GNUC__)
#define ALIGN4  _Pragma("pack(4)")
#elif defined (__CC_ARM)
#define ALIGN4 __attribute__((aligned (4)))
#endif

#define BITM_SPORT_DATA_CONFIG  (BITM_SPORT_CTL_A_LSBF \
                               | BITM_SPORT_CTL_A_PACK)

#define BITM_SPORT_CLOCK_CONFIG  (BITM_SPORT_CTL_A_ICLK \
                                | BITM_SPORT_CTL_A_CKRE \
                                | BITM_SPORT_CTL_A_GCLKEN)
                                  
#define BITM_SPORT_FS_CONFIG     (BITM_SPORT_CTL_A_FSR \
                                | BITM_SPORT_CTL_A_IFS \
                                | BITM_SPORT_CTL_A_DIFS \
                                | BITM_SPORT_CTL_A_LFS \
                                | BITM_SPORT_CTL_A_LAFS \
                                | BITM_SPORT_CTL_A_FSERRMODE)
                                 
#define SPORT_BYTE_TRANSFER_LENGTH     8u
#define SPORT_HALFWORD_TRANSFER_LENGTH 16u
#define SPORT_WORD_TRANSFER_LENGTH     32u
                                 
/*! Enumeration of different SPORT states */
typedef enum
{
    ADI_SPORT_STATE_UNINITIALIZED = 0,  /* SPORT is not yet initialized */

    ADI_SPORT_STATE_INITIALIZED,        /* SPORT is initialized */

    ADI_SPORT_STATE_DATA_FLOW_ENABLED,  /* SPORT Tx  OR Rx data flow is enabled */

    ADI_SPORT_STATE_DATA_FLOW_DISABLED,  /* SPORT Tx  OR Rx data flow is disabled */

    ADI_SPORT_STATE_PAUSED
} ADI_SPORT_STATE;


/* Function pointer typedef for the function which  submit the buffer */
typedef ADI_SPORT_RESULT (*SPORT_BUFFER_SUBMIT) (ADI_SPORT_HANDLE hDevice,void *pBuffer, uint32_t nsize);

/* Function prototype for the  submitting the buffer interrupt mode*/
typedef ADI_SPORT_RESULT (*SPORT_DMA_ENABLE)(ADI_SPORT_HANDLE hDevice,bool_t bEnable);

/* Function prototype for the  submitting the buffer interrupt mode*/
typedef ADI_SPORT_RESULT (*SPORT_DMA_CLOSE)(ADI_SPORT_HANDLE hDevice);

/*!
 *****************************************************************************
 * SPORT Configuration structure.
 *****************************************************************************/
typedef struct ADI_SPORT_CONFIG
{
    uint32_t SPORT_CTL;                          /*!< SPORT_CTL register configuration. */
    uint32_t SPORT_DIV;                          /*!< SPORT_DIV  register.     */
    uint32_t TIM_CONVT ;                          /*!< TIM_CONVT  register.     */
    ADI_DMA_WIDTH_TYPE DMA_WIDTH ;                          /*!< DMA_WIDTH  .  */
    ADI_DMA_INCR_TYPE DMA_INC;                             /*!< DMA_INC .     */
} ADI_SPORT_CFG_TYPE;

/*! Descriptor structure for Interrupt mode */
typedef struct _ADI_SPORT_BUFF_LIST
{

    void    *pStartAddress;                 /* pointer to a buffer*/

    uint32_t nCount;                        /* Size of the buffer in bytes */

    uint32_t nIndex;                        /* variable for indexing the next word for placing in FIFO */

    bool_t bInUse;                          /* usage flag */

    struct _ADI_SPORT_BUFF_LIST  *pNext;    /* Pointer to next descriptor */

}ADI_SPORT_BUFF_LIST;

/*! SPORT device information */

typedef struct _ADI_SPORT_DEVICE_INFO
{
    ADI_SPORT_STATE              eState;        /*  To indicate the state of the device */
    const ADI_DMA_CHANNEL_ID     eDMAChnlID;    /*  DMA channel ID */
    volatile ADI_SPORT_TypeDef  *pSportRegs;    /*  Base address of the  SPORT registers */
    const IRQn_Type              eIRQn;         /* IRQn */   
    ADI_INT_HANDLER_PTR          pfInthandler;   /*  Function pointer for Interrupt handler */ 
    ADI_SPORT_HANDLE             hDevice;       /*  SPORT handle */
}ADI_SPORT_DEVICE_INFO;


/*! SPORT driver instance data */
typedef struct _ADI_SPORT_DEVICE
{
    ADI_SPORT_DIRECTION       eDirection;   /* Direction in which the SPORT is opened */

    volatile ADI_SPORT_TypeDef *pSPORTRegs;   /* Pointer to SPORT Memory Mapped Registers */

    ADI_SPORT_BUFF_LIST       BufInfo[NUM_SPORT_BUFFER];     /* Buffer management related information */

    ADI_SPORT_BUFF_LIST      *pCurrDesc;    /* Points  to  current position of queue  */

    ADI_SPORT_BUFF_LIST      *pActiveDesc;  /* Points to an active buffer(From which data being processed) poition in the queue */

    ADI_SPORT_BUFF_LIST      *pProcDesc;    /* Points to the "free" buffer position in the queue */

    ADI_SPORT_DEVICE_INFO    *pSportInfo;   /* pointer to the structure which stores the information about the sport instances.*/

    ADI_CALLBACK              pfCallback;   /*  Function pointer for callback function. */

    void                     *pCBParam;     /*  Parameter to callback function. */

    SPORT_BUFFER_SUBMIT       pfSubmitBuffer;    /* function pointer for submitting the buffer */

    SPORT_DMA_ENABLE          pfEnableDMA;       /* function pointer for enabling the DMA */
    
    volatile uint32_t         nHwError;     /* variable to store the hardware status */
   
#if (ADI_SPORT_CFG_ENABLE_DMA_SUPPORT == 1)    
ALIGN4
    uint8_t                   DMAmemory[ADI_DMA_MEMORY_SIZE];
    ADI_DMA_CHANNEL_HANDLE    hDma; 
    ADI_DMA_TRANSFER          oDMADescriptor;
#endif
    PEND_VAR_DECLR
      
} ADI_SPORT_DEVICE;

/* Initializes the DMA descriptors */
static void InitDevice (ADI_SPORT_DEVICE *pDevice);

static void sport_DataHandler(ADI_SPORT_DEVICE     *pDevice);

 /* SPORT  Status interrupt handler */
static void SPORTCommonStatusHandler(ADI_SPORT_DEVICE     *pDevice);

 /* SPORT DMA interrupt handler */
static void SPORTCommonDMAHandler(void *pCBParam, uint32_t Event, void *pArg);

#if (ADI_SPORT_CFG_ENABLE_DMA_SUPPORT == 1)
/* Function prototype for the  submitting the buffer in DMA mode */
static ADI_SPORT_RESULT SubmitBufferDmaMode(ADI_SPORT_HANDLE hDevice,void *pBuffer, uint32_t nSize);
#else
/* Function prototype for the  submitting the buffer in  interrupt mode*/
static ADI_SPORT_RESULT SubmitBufferIntMode(ADI_SPORT_HANDLE hDevice,void *pBuffer, uint32_t nSize);

static ADI_SPORT_RESULT  adi_sport_ClearInterruptPendStatus(uint32_t SID);
#endif

/* Interrupt handler for the SPORT0-A*/
ADI_INT_HANDLER(SPORT0A_Int_Handler);
/* Interrupt handler for the SPORT0-B*/
ADI_INT_HANDLER(SPORT0B_Int_Handler);
/* Function prototype for the  enabling the DMA operation */
static ADI_SPORT_RESULT EnableDma(ADI_SPORT_HANDLE hDevice,bool_t bEnable);

/*! \endcond */
#endif	/* end of ifndef _DEF_SPORT_DEF_ADUCM302X_H_ */


