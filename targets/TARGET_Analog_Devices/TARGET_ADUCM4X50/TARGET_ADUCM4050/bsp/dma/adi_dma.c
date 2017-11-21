/*! *****************************************************************************
 * @file:    adi_dma.c
 * @brief:   DMA manager global file.
 -----------------------------------------------------------------------------
Copyright (c) 2016 Analog Devices, Inc.

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



/*! \addtogroup DMA_Driver DMA Driver
 *  uDMA Device Driver.
 *  @{
 */

/*=============  I N C L U D E S   =============*/
#include <adi_processor.h>
#include <drivers/dma/adi_dma.h>
#include <adi_callback.h>
#include <rtos_map/adi_rtos_map.h>
#include <drivers/general/adi_drivers_general.h>

/*! \cond PRIVATE */

/*=============  M I S R A   =============*/

#ifdef __ICCARM__
/*
* IAR MISRA C 2004 error suppressions.
*
* Pm011 (rule 6.3): he basic types of char, int, short, long, float, and double should not be used
*   Need to use bool.
*
* Pm140 (rule 11.3): a cast should not be performed between a pointer type and an integral type
*   The rule makes an exception for memory-mapped register accesses.
*/
#pragma diag_suppress=Pm011,Pm140
#endif /* __ICCARM__ */

/*=============  D E F I N E S  =============*/

/* CCD array allocation macros */
#define CCD_ALIGN (0x400) /* Memory alignment required for CCD array */
#define CCD_SIZE (32u)    /* Configure CCD allocation as an integral power of two,
                            i.e., 24 channels is allocated as 32 */

/*=============  R E G I S T E R    D E F I N E S  =============*/




/*=============  T Y P E    D E F I N E S  =============*/

/*! DMA Channel callback information structure */
typedef struct _DMA_CHANNEL {
    ADI_CALLBACK        pfCallback;             /*!< Pointer to the callback func */
    void*               pCBParam;               /*!< Application Callback param */
} DMA_CHANNEL_CALLBACK_INFO;

/*! \struct ADI_DMA_DEV_DATA
 * DMA Device instance data structure
 *
 * CallbackInfo[NUM_DMA_CHANNELSn]
 * The semantics of indexes used to access CallbackInfo elements is defined by the semantics
 * of the bits in registers DMA_ERRCHNL_CLR and DMA_INVALIDDESC_CLR. The position of these
 * bits define the channel nodes of the peripheral they map to, e.g. bit N maps to channel
 * node N.
 */
typedef struct {
    bool                        Initialized;    /*!< track initialization state. See function adi_dma_Init) */
    DMA_CHANNEL_CALLBACK_INFO   CallbackInfo[NUM_DMA_CHANNELSn];
    uint32_t                    ChannelsInUse;  /*!< bits 0 to 26 record active channels */
} ADI_DMA_DEV_DATA;


/*=============  D A T A  =============*/

/* DMA descriptor arrays must be contiguous */
/* AND impose strict alignment requirements */
/* Each compiler has different alignment directives */

/* ALIGNED: DMA channel control data array declaration */
ADI_ALIGNED_PRAGMA(CCD_ALIGN)
static ADI_DCC_TypeDef gChannelControlDataArray[CCD_SIZE * 2u] ADI_ALIGNED_ATTRIBUTE(CCD_ALIGN)

#ifdef ADI_DMA_DESCRIPTORS_IN_VOLATILE_MEMORY
	/* conditional placement of DMA descriptor table to volatile memory */
	@ "volatile_ram";
#else
	/* default placement to non-volatile memory (no override) */
	;
#endif


/* pointer to the primary CCD array */
ADI_DCC_TypeDef* const pPrimaryCCD = &gChannelControlDataArray[0];

/* pointer to the alternate CCD array */
ADI_DCC_TypeDef* const pAlternateCCD = &gChannelControlDataArray[CCD_SIZE];


/*! DMA Device Driver Data instance
 * 32 Channel Handles initialized to {0, 0}, i.e. call-back function pointer
 * set to NULL and call-back function parameters set to NULL
 */
static ADI_DMA_DEV_DATA DMA_DevData = {

    false,                      /*!< DMA device data not initialized. (See adi_dma_Init) */
    {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0},
     {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0},
     {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0},
     {0,0}, {0,0}, {0,0}},
    0ul                         /*!< channels-in-use bitfield */
};

/*! pointer to the DMA Device Driver Data instance */
static ADI_DMA_DEV_DATA* const pDMA_DevData = &DMA_DevData;

/*=============  Local function declarations  =============*/

/*==========  DMA HANDLERS  ==========*/

/*! DMA Error Handler */
void DMA_Err_Int_Handler(void);

/*==========  U T I L I T Y    M A C R O S  ==========*/

/*! \endcond*/
/*=============  A P I    I M P L E M E N T A T I O N S  =============*/

/*!
 * @brief  Initialize the DMA peripheral
 *
 * @return         none
 *
 * The application must call this API once
 *
 */
void adi_dma_Init(void)
{
    ADI_INT_STATUS_ALLOC();
    ADI_ENTER_CRITICAL_REGION();

    if( false == pDMA_DevData->Initialized )
    {
        pDMA_DevData->Initialized = true;

        /* Enable the DMA Controller */
        pADI_DMA0->CFG |= BITM_DMA_CFG_MEN;

        /* Set descriptor memory base pointer on DMA controller */
        pADI_DMA0->PDBPTR = (uint32_t)pPrimaryCCD;

        /* Enable the DMA Error Interrupt */
        NVIC_EnableIRQ(DMA_CHAN_ERR_IRQn);

        /* Reset per-channel, bitmapped control registers (W1C) */
        const uint32_t w1r_value = (uint32_t) ((1 << NUM_DMA_CHANNELSn) - 1);
        pADI_DMA0->RMSK_SET        = w1r_value;
        pADI_DMA0->EN_CLR          = w1r_value;
        pADI_DMA0->ALT_CLR         = w1r_value;
        pADI_DMA0->PRI_CLR         = w1r_value;
        pADI_DMA0->ERRCHNL_CLR     = w1r_value;
        pADI_DMA0->ERR_CLR         = w1r_value;
        pADI_DMA0->INVALIDDESC_CLR = w1r_value;
    }

    ADI_EXIT_CRITICAL_REGION();
}

/**
 * @brief    Register a call-back function for a DMA channel.
 *
 * @param [in]  eChannelID    The ID of the DMA channel being assigned a call-back function.
 * @param [in]  pfCallback    Pointer to the application callback function.
 * @param [in]  pCBParam      Application callback parameter.
 *
 * @details  The function registers a call-back function for the DMA channel node
 * identified by eChannelID and stores the extra parameters this call-back function
 * may require. A NULL callback function pointer means "DMA channel unused".
 *
 * @return   Status
 *              - #ADI_DMA_SUCCESS                      Successfully registered a call-back function for the given DMA channel node.
 *              - #ADI_DMA_ERR_NOT_INITIALIZED          [D] adi_dma_Init must be called prior registering a call-back function.
 *              - #ADI_DMA_ERR_INVALID_PARAMETER        [D] Some parameter(s) passed to the function is invalid.
 */
ADI_DMA_RESULT adi_dma_RegisterCallback (
    DMA_CHANn_TypeDef   const eChannelID,
    ADI_CALLBACK        const pfCallback,
    void*               const pCBParam
    )
{
    ADI_DMA_RESULT result = ADI_DMA_SUCCESS;

#ifdef ADI_DEBUG
    /* DMA must be initialized first */
    if (false == pDMA_DevData->Initialized) {
        result = ADI_DMA_ERR_NOT_INITIALIZED;
    }else{
        const size_t numChannelId = sizeof(pDMA_DevData->CallbackInfo) / sizeof(DMA_CHANNEL_CALLBACK_INFO);
        if (numChannelId <= eChannelID)                 /*!< pDMA_DevData->CallbackInfo definition is invalid */
        {
            result = ADI_DMA_ERR_INVALID_PARAMETER;
        }
    }
    if (ADI_DMA_SUCCESS == result)              /* if no errors previously detected */
#endif
    {
        /* eChannelID cannot be out of range by definition (we use DMA_CHANn_TypeDef) */
        DMA_CHANNEL_CALLBACK_INFO * pChannel = &pDMA_DevData->CallbackInfo[eChannelID];

        /* Set the callback parameters */
        pChannel->pfCallback = pfCallback;      /* assign the pointer to a callback function */
        pChannel->pCBParam   = pCBParam;        /* store the parameters to be used with the callback function */

        const uint32_t nChannelBit = (1u << eChannelID);
        if (NULL != pfCallback) {
            pDMA_DevData->ChannelsInUse |= nChannelBit;         /* set the bit to mark the channel as "being used" */
        }else{
            pDMA_DevData->ChannelsInUse &= (~nChannelBit);      /* clear the bit to mark the channel as "not being used" */
        }
    }
    return result;
}

/*! \cond PRIVATE */


#if defined(__ICCARM__)

/* ARM Cortex-M3/M4, IAR compiler (CMSIS standard) */
#define ADI_CLZ(X)     __CLZ(X)

#elif defined(__GNUC__)

/* ARM Cortex-M3/M4, GNU-ARM compiler */
#define ADI_CLZ(X)     __builtin_clz(X)

#elif defined(__ARMCC_VERSION)

/* ARM Cortex-M3/M4, Keil compiler */
#define ADI_CLZ(X)    __clz(X)

#else

#error "Macro ADI_CLZ undefined!!!"

#endif

/*! DMA Error Handler
 *
 * The DMA Error handler looks at the channels in use which are flagged in register ERRCHNL_CLR
 * or INVALIDDESC_CLR and calls the associated call-back functions, if defined. If a call-back
 * function is undefined (NULL pointer) then it means the associated driver ignores these errors.
 *
 * Then, all the bits set in ERRCHNL_CLR and INVALIDDESC_CLR at the time the handler is called
 * are cleared.
 */
void DMA_Err_Int_Handler(void)
{
    ISR_PROLOG()

    const uint32_t nErrClr = pADI_DMA0->ERR_CLR;                /* get all the bits set in ERR_CLR */
    const uint32_t nErrChnClr = pADI_DMA0->ERRCHNL_CLR;         /* get all the bits set in ERRCHNL_CLR */
    const uint32_t nInvdDescClr = pADI_DMA0->INVALIDDESC_CLR;   /* get all the bits set in INVALIDDESC_CLR */

    /* if there are invalid channel descriptors or channel errors amongts the channels in use */
    uint32_t functionsToBeCalled = pDMA_DevData->ChannelsInUse & (nErrChnClr | nInvdDescClr);

    if (functionsToBeCalled > 0u)
    {
        const uint32_t numBits = sizeof(uint32_t) << 3; /* maximum number of bits to be considered */
        uint32_t nlz;                                   /* number of leading zeroes in functionsToBeCalled */

        /* For all the bits set in functionsToBeCalled, starting from the MSB */
        for (nlz = (uint32_t) ADI_CLZ(functionsToBeCalled); nlz < numBits; nlz = (uint32_t) ADI_CLZ(functionsToBeCalled))
        {
            const uint32_t bitSet = numBits - nlz - 1u; /* bit position in functionsToBeCalled */
            const uint32_t selected_bit = ((uint32_t)1u << bitSet);
            DMA_CHANNEL_CALLBACK_INFO* pChannel = &pDMA_DevData->CallbackInfo[bitSet];

            /* if there's a callback function to be called */
            if (NULL != pChannel->pfCallback)
            {
                /* define the nature of the error: DMA bus error or else invalid descriptor */
                uint32_t nEvent = ((nErrChnClr & selected_bit) != 0u)
                                ? (uint32_t)ADI_DMA_EVENT_ERR_BUS
                                : (uint32_t)ADI_DMA_EVENT_ERR_INVALID_DESCRIPTOR;

                /* report the error to the peripheral through the callback function */
                pChannel->pfCallback (pChannel->pCBParam, nEvent, NULL );
            }

            functionsToBeCalled &= ~selected_bit;       /* clear bit in functionsToBeCalled */
        }
    }

    /* Clear the errors processed in the loop above */
    pADI_DMA0->ERRCHNL_CLR = nErrChnClr;        /* W1C: clear only all the bits set in nErrChnClr */
    pADI_DMA0->INVALIDDESC_CLR = nInvdDescClr;  /* W1C: clear only all the bits set in nInvdDescClr */
    pADI_DMA0->ERR_CLR = nErrClr;               /* W1C: clear only all the bits set in nErrClr */

    ISR_EPILOG()
}

/*! \endcond*/

/**@}*/


