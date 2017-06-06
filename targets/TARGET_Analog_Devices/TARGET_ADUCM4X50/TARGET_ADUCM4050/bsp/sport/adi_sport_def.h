/*! *****************************************************************************
 * @file:    adi_sport_def.h
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
#ifndef ADI_SPORT_DEF_H
#define ADI_SPORT_DEF_H

#include <drivers/general/adi_data_transfer.h>

#if defined(ADI_CFG_SPORT0A_SERIAL_WLEN)
#if (ADI_CFG_SPORT0A_SERIAL_WLEN <= 3u) || (ADI_CFG_SPORT0A_SERIAL_WLEN > 32u)
#error "Invalid word length : it must be between 4 and 32"
#endif
#else
#error "ADI_CFG_SPORT0A_SERIAL_WLEN undefined!!! "
#endif

#if defined(ADI_CFG_SPORT0B_SERIAL_WLEN)
#if (ADI_CFG_SPORT0B_SERIAL_WLEN <= 3u) || (ADI_CFG_SPORT0B_SERIAL_WLEN > 32u)
#error "Invalid word length : it must be between 4 and 32"
#endif
#else
#error "ADI_CFG_SPORT0B_SERIAL_WLEN undefined!!! "
#endif

#define ADI_SPORT_NUM_INSTANCES         (1u)    /*!< Number of SPORT devices available */
#define ADI_SPORT_NUM_CHANNELS          (2u)    /*!< Number of SPORT channel for each SPORT devcie */

#define BITM_SPORT_DATA_CONFIG  ( BITM_SPORT_CTL_A_LSBF \
                                | BITM_SPORT_CTL_A_PACK)

#define BITM_SPORT_CLOCK_CONFIG ( BITM_SPORT_CTL_A_ICLK \
                                | BITM_SPORT_CTL_A_CKRE \
                                | BITM_SPORT_CTL_A_GCLKEN)

#define BITM_SPORT_FS_CONFIG    ( BITM_SPORT_CTL_A_FSR \
                                | BITM_SPORT_CTL_A_IFS \
                                | BITM_SPORT_CTL_A_DIFS \
                                | BITM_SPORT_CTL_A_LFS \
                                | BITM_SPORT_CTL_A_LAFS \
                                | BITM_SPORT_CTL_A_FSERRMODE)

#define SPORT_BYTE_TRANSFER_LENGTH      (8u)
#define SPORT_HALFWORD_TRANSFER_LENGTH  (16u)
#define SPORT_WORD_TRANSFER_LENGTH      (32u)

#define SPORT_GET_WLEN(ctlVal)          ((((ctlVal) & (uint32_t) BITM_SPORT_CTL_A_SLEN) >> ((uint32_t) BITP_SPORT_CTL_A_SLEN)) + 1u)
#define SPORT_GET_PACKEN(ctlVal)        ((ctlVal) & (uint32_t) BITM_SPORT_CTL_A_PACK) >> ((uint32_t) BITP_SPORT_CTL_A_PACK)

#define SPORT_CHECK_CFG_CTL(CFG)        assert(0u == ((CFG) & (((uint32_t)BITM_SPORT_CTL_A_SPEN) | ((uint32_t)BITM_SPORT_CTL_A_DMAEN))))


#define SPORT_BIT_PACK_NONE     (((uint32_t) ADI_SPORT_NO_PACKING)    >> ((uint32_t) BITP_SPORT_CTL_A_PACK))
#define SPORT_BIT_PACK_8        (((uint32_t) ADI_SPORT_8BIT_PACKING)  >> ((uint32_t) BITP_SPORT_CTL_A_PACK))
#define SPORT_BIT_PACK_16       (((uint32_t) ADI_SPORT_16BIT_PACKING) >> ((uint32_t) BITP_SPORT_CTL_A_PACK))

/*!
 *****************************************************************************
 * \struct  ADI_SPORT_STATE
 *  Enumeration of different SPORT states.
 *****************************************************************************/
typedef enum
{
    ADI_SPORT_STATE_UNINITIALIZED = 0,  /*!< SPORT is not yet initialized */
    ADI_SPORT_STATE_INITIALIZED,        /*!< SPORT is initialized */
    ADI_SPORT_STATE_DATA_FLOW_ENABLED,  /*!< SPORT Tx or Rx data flow is enabled (SPORT peripheral cannot be re-configured) */
    ADI_SPORT_STATE_DATA_FLOW_DISABLED, /*!< SPORT Tx or Rx data flow is disabled (SPORT peripheral can be re-configured) */
    ADI_SPORT_STATE_PAUSED
} ADI_SPORT_STATE;

/*!
 *****************************************************************************
 * \struct  ADI_SPORT_CONFIG
 *  Structure for initializing the static config.
 *****************************************************************************/

typedef struct _ADI_SPORT_CONFIG
{
    uint32_t    CTL;            /*!< SPORT_CTL register. */
    uint32_t    DIV;            /*!< SPORT_DIV register. */
    uint32_t    TIM_CONVT;      /*!< TIM_CONVT Register. */
    uint32_t    DMA_WIDTH;      /*!< DMA_WIDTH */
    uint32_t    DMA_INC;        /*!< DMA_INC */
} ADI_SPORT_CONFIG;

/*!
 *****************************************************************************
 * \struct  ADI_SPORT_DEVICE_INFO
 *  SPORT device information.
 *****************************************************************************/
typedef struct _ADI_SPORT_DEVICE_INFO
{
    volatile ADI_SPORT_TypeDef* pSportRegs;     /*!< Base address of the SPORT registers */
    ADI_SPORT_CONFIG            sportCfg;       /*!< SPORT configuration data */
    ADI_SPORT_STATE             eState;         /*!< To indicate the state of the device */
    const DMA_CHANn_TypeDef     eDMAChnlID;     /*!< DMA channel ID */
    const IRQn_Type             eDMAn;          /*!< DMA channel IRQ identifier */
    const IRQn_Type             eIRQn;          /*!< SPORT IRQ identifier */
    ADI_SPORT_HANDLE            hDevice;        /*!< SPORT handle */
} ADI_SPORT_DEVICE_INFO;

/******************************************************************************
 * SPORT Device internal API function prototypes
 *****************************************************************************/

#define NUM_SPORT_BUFFER                (2u)

/** SPORT driver instance data */
typedef struct _ADI_SPORT_DEVICE
{
    ADI_SPORT_DEVICE_INFO *     pSportInfo;     /*!< pointer to the structure which stores the information about the SPORT instances.*/
    ADI_SPORT_DIRECTION         eDirection;     /*!< Direction in which the SPORT is opened */
    ADI_CALLBACK                pfCallback;     /*!< Function pointer for callback function. */
    void *                      pCBParam;       /*!< Parameter to callback function. */
    ADI_DT_CHANNEL              sportChannel;   /*!< SPORT channel to manage transmitted data buffers */
    volatile uint32_t           nHwError;       /*!< variable to store the hardware status */
} ADI_SPORT_DEVICE;

/** Initialize a SPORT device */
static inline void sport_Init (ADI_SPORT_DEVICE * pDevice);

/** Configure a SPORT device */
static inline void sport_Configure (ADI_SPORT_DEVICE *pDevice, ADI_SPORT_CONFIG const * sportCfg);

/** Function prototype for submitting a buffer for SPORT Rx or Tx DMA driven transmission */
static ADI_SPORT_RESULT sport_SubmitBufferDmaMode(ADI_SPORT_DEVICE * pDevice, ADI_DT_BUFF_INFO * pBuff);

/** Function prototype for submitting a buffer for SPORT Rx or Tx core driven transmission */
static ADI_SPORT_RESULT sport_SubmitBufferIntMode(ADI_SPORT_DEVICE * pDevice, ADI_DT_BUFF_INFO * pBuff);

/** Fucntion prototype for completing a SPORT transmission (Rx or Tx) */
static void sport_Terminate(ADI_SPORT_DEVICE * pDevice);

/** Interrupt Handlers */

/** SPORT interrupt handler */
static void sport_InterruptHandler(ADI_SPORT_DEVICE * pDevice);

static inline void sport_DmaErrorCallback(void *pCBParam, uint32_t Event, void *pArg);

static inline uint32_t GetBytesPerSportData(const uint32_t ctlVal);

/*
 * Handle Validation function
*/
#ifdef ADI_DEBUG
static ADI_SPORT_RESULT ValidateHandle(ADI_SPORT_HANDLE const hDevice);
#endif /* ADI_DEBUG */

#endif  /* end of ifndef ADI_SPORT_DEF_H */
/*! \endcond */


