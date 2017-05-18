/*! *****************************************************************************
 * @file:    adi_beep.c
 * @brief:   BEEP device driver global file.
 * @details: This a global file which includes a specific file based on the processor family.
 *           This included file will be  containing  BEEP device driver functions.
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
#include <adi_processor.h>

#include <stddef.h>
#include <assert.h>

#include <drivers/beep/adi_beep.h>
#include <rtos_map/adi_rtos_map.h>
#include "adi_beep_def.h"

/** @addtogroup BEEP_Driver BEEP Driver
 *  @{
 * @brief Beeper Driver
 * @note The application must include drivers/beep/adi_beep.h to use this driver.
 */

#ifdef __ICCARM__
/*
* IAR MISRA C 2004 error suppressions.
*
* Pm123 (rule 8.5): there shall be no definition of objects or functions in a header file
*   This isn't a header as such.
*
* Pm073 (rule 14.7): a function should have a single point of exit.
* Pm143 (rule 14.7): a function should have a single point of exit at the end of the function.
*   Multiple returns are used for error handling.
*
* Pm050 (rule 14.2): a null statement shall only occur on a line by itself
*   Needed for null expansion of ADI_INSTALL_HANDLER and others.
*
* Pm140 (rule 11.3): a cast should not be performed between a pointer type and an integral type
*   Required for MMR addresses and callback parameters.
*
* Pm031: (MISRA C 2004 rule 12.7)  bitwise operations shall not be performed on signed integer types
*    Required for MMR manipulations.
* 
* Pm152: (MISRA C 2004 rule 17.4)  array indexing shall only be applied to objects defined as an array type  
*    Required for adi_beep_PlaySequence() to access the user-supplied array of notes.
* 
* Pm141: (MISRA C 2004 rule 11.4) a cast should not be performed between a pointer to object type and a 
*     different pointer to object type, this casts from type.
*    Required to store a an array of varying size in a device structure. 
* 
*    Required for adi_beep_PlaySequence() to access the user-supplied array of notes.
*/
#pragma diag_suppress=Pm123,Pm073,Pm143,Pm050,Pm140,Pm031,Pm152,Pm141
#endif /* __ICCARM__ */

/*==========  D A T A  ==========*/
static ADI_BEEP_DRIVER adi_beep_Device[1];

/*! \cond PRIVATE */
/* Handler for the BEEP interrupt */
void Beep_Int_Handler(void);

/* debug handle checker */
#ifdef ADI_DEBUG
#define ADI_BEEP_INVALID_HANDLE(h) (&adi_beep_Device[0] != (h))
#endif

/* definition for the BEEP IRQ - there is only ever one instance of the 
 * BEEP driver, so reducing space by using a #define rather than including
 * it in the device structure. */
#define BEEP_IRQ (BEEP_EVT_IRQn)

#if ADI_BEEP_CFG_SEQUENCE_REPEAT_VALUE == 0
/* A single note is requested.  Only enable the AEND int. */
#define INTERRUPT_ON_SEQEND  (0)
#define INTERRUPT_ON_AEND    (1)
#else
/* A two-tone sequence is requested.  Only enable the SEQEND int. */
#define INTERRUPT_ON_SEQEND  (1)
#define INTERRUPT_ON_AEND    (0)
#endif

/*! \endcond */

static const ADI_BEEP_STATIC_INIT gBeeperStaticConfigData[ADI_BEEP_MAX_DEVID] = {
	/* single instance of Beeper device */
	{
		/* configuration register */
		( (INTERRUPT_ON_SEQEND                             << BITP_BEEP_CFG_SEQATENDIRQ)
		| (INTERRUPT_ON_AEND                               << BITP_BEEP_CFG_AENDIRQ)
		| (ADI_BEEP_CFG_SEQUENCE_REPEAT_VALUE              << BITP_BEEP_CFG_SEQREPEAT)
		),

		/* Status register (interrupt clears) */
		(ADI_BEEP_ALL_INTERRUPTS),

		/* ToneA control register */
		( ((uint32_t)ADI_BEEP_TONEA_DISABLE                << BITP_BEEP_TONEA_DIS)
		| ((uint32_t)ADI_BEEP_TONEA_FREQUENCY              << BITP_BEEP_TONEA_FREQ)
		| ((uint32_t)ADI_BEEP_TONEA_DURATION               << BITP_BEEP_TONEA_DUR)
		),

		/* ToneB control register */
		( ((uint32_t)ADI_BEEP_TONEB_DISABLE                << BITP_BEEP_TONEB_DIS)
		| ((uint32_t)ADI_BEEP_TONEB_FREQUENCY              << BITP_BEEP_TONEB_FREQ)
		| ((uint32_t)ADI_BEEP_TONEB_DURATION               << BITP_BEEP_TONEB_DUR)
		)
	}
};

/*! \endcond */


/*!
 *  @brief BEEP Initialization
 *
 *  @param[in] DeviceNum    Integer specifying the ID of Beeper to use.
 *  @param[in] pMemory      Pointer to the memory to be used by the driver.
 *                          Size of the memory should be at least #ADI_BEEP_MEMORY_SIZE bytes.
 *  @param[in] MemorySize   Size of the memory passed in pMemory parameter.
 *  @param[out] phDevice    Pointer to a location that the device data pointer
 *                          will be written upon successful initialization.
 *
 *  @return    Status
 *             - #ADI_BEEP_SUCCESS                  Success: BEEP device driver initialized successfully.
 *             - #ADI_BEEP_SEMAPHORE_FAILED         The BEEP sempahore could not be created.
 *             - #ADI_BEEP_ALREADY_INITIALIZED [D]  The BEEP is already initialized.
 *             - #ADI_BEEP_NULL_PTR [D]              Null pointer.
 *             - #ADI_BEEP_BAD_DEV_ID [D]           The device number is invalid.
 *
 *  Initialize the BEEP device for use.  The core NVIC BEEP interrupt is enabled.  This API
 *  must preceed all other beeper API calls and the handle returned must be passed to all other beeper API
 *  calls.
 *
 *
 *  @note      The contents of \a phDevice will be set to NULL upon failure.\n\n
 *
 *  @note      The BEEP device driver will clear all pending interrupts and disable all beeper
 *             interrupts during beeper device initialization.
 *
 *  @note      CALLBACKS: If a callback is registered, it will be called on 
 *             completion of the note or sequence. The "Event" parameter will
 *             contain which event occurred, either ADI_BEEP_INTERRUPT_SEQUENCE_END
 *             or ADI_BEEP_INTERRUPT_NOTE_END.
 *
 *  @warning   This API will put the beeper in preconfigured mode as defined in 
 *             adi_beep_config.h file.
 *             Refer adi_beep_config.h file to see which all features can be preconfigured.
 *
 *  @sa    adi_beep_Close().
 */
ADI_BEEP_RESULT adi_beep_Open(ADI_BEEP_DEV_ID          const DeviceNum,
                              void*                    const pMemory,
                              uint32_t                 const MemorySize,
                              ADI_BEEP_HANDLE*         const phDevice)
{
    ADI_BEEP_DRIVER    *pDevice;
    ADI_BEEP_DEV_DATA  *pData;
    /* store a bad handle in case of failure */
    *phDevice = (ADI_BEEP_HANDLE) NULL;

#ifdef ADI_DEBUG
    if (DeviceNum >= ADI_BEEP_MAX_DEVID)
    {
        return ADI_BEEP_BAD_DEV_ID;
    }

    if (pMemory == NULL)
    {
        return ADI_BEEP_NULL_PTR;
    }
    
    assert (MemorySize >= sizeof(ADI_BEEP_DRIVER));
#endif

    /* local pointer to instance data */
    pDevice = &adi_beep_Device[DeviceNum];
    pDevice->pReg = pADI_BEEP0;
    pDevice->pData = (ADI_BEEP_DEV_DATA*)pMemory;
    pData = pDevice->pData;

#ifdef ADI_DEBUG
    if (ADI_BEEP_STATE_UNINITIALIZED != adi_beep_Device[DeviceNum].pData->state)
    {
        return ADI_BEEP_ALREADY_INITIALIZED;
    }
#endif

    pData->cbFunc  = NULL;
    pData->cbParam = NULL;
    SEM_CREATE(pDevice->pData, "BEEP_SEM", ADI_BEEP_SEMAPHORE_FAILED);
    
    /* set statically configured initialization data */
    ADI_BEEP_STATIC_INIT const* pInitData = &gBeeperStaticConfigData[DeviceNum];
    ADI_BEEP_TypeDef   *pReg = pDevice->pReg;

    pReg->CFG   = pInitData->BEEP_CFG;
    pReg->STAT  = pInitData->BEEP_STAT;
    pReg->TONEA = pInitData->BEEP_TONEA;
    pReg->TONEB = pInitData->BEEP_TONEB;

    /* enable beeper interrupts in NVIC */
    NVIC_EnableIRQ(BEEP_IRQ);

    /* mark driver initialized */
    pData->state = ADI_BEEP_STATE_INITIALIZED;

    /* store handle at application handle pointer */
    *phDevice = (ADI_BEEP_HANDLE)pDevice;
            
    return ADI_BEEP_SUCCESS;            /* initialized */
}


/*!
 * @brief  Uninitialize and deallocate a BEEP device.
 *
* @param[in]   hDevice    Device handle obtained from #adi_beep_Open().
 *
 * @return      Status
 *                - #ADI_BEEP_SUCCESS                Success: Call completed successfully.
 *                - #ADI_BEEP_BAD_DEV_HANDLE [D]     Error: Invalid device handle parameter.
 *                - #ADI_BEEP_NOT_INITIALIZED [D]    Error: Device has not been initialized for use, see #adi_beep_Open().
 *
 * Uninitialize and release an allocated BEEP device for other use.  The core NVIC BEEP interrupt is disabled.
 *
 * @sa        adi_beep_Open().
 */
ADI_BEEP_RESULT adi_beep_Close(ADI_BEEP_HANDLE const hDevice)
{

    ADI_BEEP_DRIVER    *pDevice;
    ADI_BEEP_DEV_DATA  *pData;
    ADI_BEEP_TypeDef   *pReg;

    pDevice = (ADI_BEEP_DRIVER*)hDevice;
    pData = pDevice->pData;
    pReg =  pDevice->pReg;

#ifdef ADI_DEBUG
    if (ADI_BEEP_INVALID_HANDLE(hDevice))
	{
          return ADI_BEEP_BAD_DEV_HANDLE;
	}
    if (ADI_BEEP_STATE_UNINITIALIZED == pData->state)
	{
          return ADI_BEEP_NOT_INITIALIZED;
	}
#endif

    /* uninitialize */
    NVIC_DisableIRQ(BEEP_IRQ);

    pData->state     = ADI_BEEP_STATE_UNINITIALIZED;
    pData->cbFunc    = NULL;
    pReg->CFG   = 0u;
    pReg->STAT  = 0u;
    pReg->TONEA = 0u;
    pReg->TONEB = 0u;
    SEM_DELETE(pDevice->pData, ADI_BEEP_SEMAPHORE_FAILED);
    return ADI_BEEP_SUCCESS;
}

/*!
 * @brief  Register a callback for the beeper driver.
 *
 * @param[in]  hDevice        Device handle obtained from #adi_beep_Open().
 * @param[in]  pfCallback     The application supplied callback which will be called to notify device
 *                            related events.  
 * @param[in]  pCBParam       The application supplied callback parameter which can be passed back in
 *                            the callback function.
 *
 * @return      Status
 *                - #ADI_BEEP_SUCCESS                Call completed successfully.
 *                - #ADI_BEEP_BAD_DEV_HANDLE [D]     Invalid device handle parameter.
 *                - #ADI_BEEP_NOT_INITIALIZED [D]    Device has not been initialized for use, see #adi_beep_Open().
 *
 * Registers a callback for the beeper interrupts.  When an interrupt occurs, the 
 * driver will handle any required interaction with the hardware and then call 
 * the registered callback. 
 *
 * @sa        adi_beep_Open().
 */
ADI_BEEP_RESULT adi_beep_RegisterCallback(ADI_BEEP_HANDLE const hDevice, 
                                          ADI_CALLBACK          pfCallback,
                                          void* const           pCBParam)
{
    ADI_BEEP_DRIVER    *pDevice = (ADI_BEEP_DRIVER*)hDevice;
        
    ADI_INT_STATUS_ALLOC();   
 
#ifdef ADI_DEBUG
    if (ADI_BEEP_INVALID_HANDLE(hDevice)) {
        return ADI_BEEP_BAD_DEV_HANDLE;
    }
    
    if (ADI_BEEP_STATE_UNINITIALIZED == pDevice->pData->state) {
        return ADI_BEEP_NOT_INITIALIZED;
    }
#endif
    /* Assign the callback within a critical region. */
    ADI_ENTER_CRITICAL_REGION();        
    pDevice->pData->cbFunc  = pfCallback;
    pDevice->pData->cbParam = pCBParam;    
    ADI_EXIT_CRITICAL_REGION();
    
    return ADI_BEEP_SUCCESS; 
}


#if ADI_BEEP_INCLUDE_PLAY_SEQUENCE == 1
/*!
 * @brief  Play a beeper tone sequence.
 *
 * @param[in]   hDevice       Device handle obtained from #adi_beep_Open().
 * @param[in]   aSequence     The sequence of notes to be played by the beeper.
 * @param[in]   count         The number of notes in the sequence, must be a multiple
 *                            of two, and a maximum size of 254 notes.
 *
 * @return      Status
 *                - #ADI_BEEP_SUCCESS                Success: Call completed successfully.
 *                - #ADI_BEEP_INVALID_COUNT          Sequence count must be multiples of two.
 *                - #ADI_BEEP_NULL_PTR [D]           Null pointer.
 *                - #ADI_BEEP_BAD_DEV_HANDLE [D]     Invalid device handle parameter.
 *                - #ADI_BEEP_NOT_INITIALIZED [D]    Device has not been initialized for use, see #adi_beep_Open().
 *
 * Programs the A/B tone pair to play a sequence of notes.  The sequnce can be 
 * stopped by calling adi_beep_Enable(..., false).  The beeper will be enabled
 * and disabled internally by the driver. This code, and supporting data, can
 * be removed by setting ADI_BEEP_INCLUDE_PLAY_SEQUENCE == 0 in the 
 * adi_beep_config.h configuration file.
 *
 * @sa        adi_beep_Open().
 * @sa        adi_beep_Enable()
 */
ADI_BEEP_RESULT adi_beep_PlaySequence(ADI_BEEP_HANDLE const hDevice, 
                                      ADI_BEEP_NOTE aSequence[], 
                                      uint8_t count)
{
    ADI_BEEP_DRIVER    *pDevice = (ADI_BEEP_DRIVER*)hDevice;
    ADI_BEEP_TypeDef   *pReg    = pDevice->pReg;
    uint16_t           nSeqCnt = 0u;
    
    ADI_INT_STATUS_ALLOC();
    
#ifdef ADI_DEBUG
    if (ADI_BEEP_INVALID_HANDLE(hDevice)) {
        return ADI_BEEP_BAD_DEV_HANDLE;
    }
    
    if (ADI_BEEP_STATE_UNINITIALIZED == pDevice->pData->state) {
        return ADI_BEEP_NOT_INITIALIZED;
    }
    
    if (NULL == aSequence) {
        return ADI_BEEP_NULL_PTR;
    }
    
    /* The sequence count must be a multiple of two, be greater than 1 
     * and must be a maximum of (127 * 2) notes in length.  The hardware supports a 
     * sequence of up to 127, and there are two notes associated with that. */
    if (((127u * 2u) < count) || 
        ((count % 2u) != 0u)  ||
        (count < 2u)) {
        return ADI_BEEP_INVALID_COUNT;
    }
#endif

    /* Two notes are loaded at a time, and the sequence count refers to 
     * the number of times that both tone registers should be played. */
    nSeqCnt = ((uint16_t)count) >> 1u;
    
    ADI_ENTER_CRITICAL_REGION();

    /* make a hole, and disable the beeper */
    pReg->CFG &= (uint16_t)~(BITM_BEEP_CFG_SEQREPEAT | BITM_BEEP_CFG_AENDIRQ | BITM_BEEP_CFG_EN);

    pReg->TONEA =  (  (uint16_t)((uint16_t)aSequence[0].frequency << ADI_BEEP_TONE_FREQ_BITPOS)
                   |(uint16_t)((uint16_t)aSequence[0].duration << ADI_BEEP_TONE_DUR_BITPOS)   );
   
    pReg->TONEB =  (  (uint16_t)((uint16_t)aSequence[1].frequency << ADI_BEEP_TONE_FREQ_BITPOS)
                   |(uint16_t)((uint16_t)aSequence[1].duration << ADI_BEEP_TONE_DUR_BITPOS)   );
    

    /* program new sequence count, while preserving everything else */
    pReg->CFG |= (BITM_BEEP_CFG_EN | 
                  BITM_BEEP_CFG_BSTARTIRQ | 
                  BITM_BEEP_CFG_SEQATENDIRQ | 
                  (uint16_t)((uint16_t)(nSeqCnt) << BITP_BEEP_CFG_SEQREPEAT));

    pDevice->pData->pSeqArray  = (ADI_BEEP_NOTE(*)[])aSequence; 
    pDevice->pData->nSeqMax    = count;
    pDevice->pData->nSeqIndex  = 2u;  
    
    /* We're now playing, but not blocked */
    pDevice->pData->state |= (ADI_BEEP_STATE_PLAYING);
    
    ADI_EXIT_CRITICAL_REGION();

    return ADI_BEEP_SUCCESS;
}
#endif

/*!
 * @brief  Play a single note/beep.
 *
* @param[in]   hDevice       Device handle obtained from #adi_beep_Open().
 * @param[in]   note          The note to play.
 *
 * @return      Status
 *                - #ADI_BEEP_SUCCESS                Success: Call completed successfully.
 *                - #ADI_BEEP_BAD_DEV_HANDLE [D]     Error: Invalid device handle parameter.
 *                - #ADI_BEEP_NOT_INITIALIZED [D]    Error: Device has not been initialized for use, see #adi_beep_Open().
 *
 * Programs the A tone to play a single note.
 *
 * @sa        adi_beep_Open().
 */
ADI_BEEP_RESULT adi_beep_PlayNote(ADI_BEEP_HANDLE const hDevice, 
                                  ADI_BEEP_NOTE   note)
{
    ADI_BEEP_DRIVER    *pDevice;
    ADI_BEEP_TypeDef   *pReg;
    ADI_INT_STATUS_ALLOC();
    
    pDevice = (ADI_BEEP_DRIVER*)hDevice;
    pReg = pDevice->pReg;

#ifdef ADI_DEBUG
    if (ADI_BEEP_INVALID_HANDLE(hDevice)) {
        return ADI_BEEP_BAD_DEV_HANDLE;
    }
    
    if (ADI_BEEP_STATE_UNINITIALIZED == pDevice->pData->state) {
        return ADI_BEEP_NOT_INITIALIZED;
    }
#endif

    ADI_ENTER_CRITICAL_REGION();

    /* Clear any previous sequence setup, and disable the beeper */
    pReg->CFG &= (uint16_t)~(BITM_BEEP_CFG_SEQREPEAT | BITM_BEEP_CFG_EN);

    /* Set Tone A */
    pReg->TONEA =  (  (uint16_t)((uint16_t)note.frequency << ADI_BEEP_TONE_FREQ_BITPOS)
                   |(uint16_t)((uint16_t)note.duration << ADI_BEEP_TONE_DUR_BITPOS)   );

    /* program new sequence count, while preserving everything else */
    pReg->CFG |= (BITM_BEEP_CFG_EN | BITM_BEEP_CFG_AENDIRQ);
    
    /* We're now playing but not blocked */
    pDevice->pData->state |= (ADI_BEEP_STATE_PLAYING);
    ADI_EXIT_CRITICAL_REGION();

    return ADI_BEEP_SUCCESS;
}


/*!
 * @brief  Play a a repeating two-tone beep. Similar to an alarm.
 *
* @param[in]   hDevice       Device handle obtained from #adi_beep_Open().
 * @param[in]   noteA         The note to play first.
 * @param[in]   noteB         The note to play second.
 * @param[in]   count         The number of times to repeat the two-note signal,
 *                            maximum of 127.
 * 
 *
 * @return      Status
 *                - #ADI_BEEP_SUCCESS                Success: Call completed successfully.
 *                - #ADI_BEEP_BAD_DEV_HANDLE [D]     Error: Invalid device handle parameter.
 *                - #ADI_BEEP_NOT_INITIALIZED [D]    Error: Device has not been initialized for use, see #adi_beep_Open().
 *
 * Programs the beeper to play a repeating two-tone signal.
 * The count argument refers to the number of iterations of both notes, not 
 * just a single note.
 *
 * @sa        adi_beep_Open().
 * @sa        adi_beep_PlayNote().
 * @sa        adi_beep_PlayNSequence().
 */
ADI_BEEP_RESULT adi_beep_PlayTwoTone(ADI_BEEP_HANDLE const hDevice, 
                                      ADI_BEEP_NOTE   noteA,
                                      ADI_BEEP_NOTE   noteB,
                                      uint8_t         count)
{
    ADI_BEEP_DRIVER    *pDevice;
    ADI_BEEP_TypeDef   *pReg;
    ADI_INT_STATUS_ALLOC();
    
    pDevice = (ADI_BEEP_DRIVER*)hDevice;
    pReg = pDevice->pReg;

#ifdef ADI_DEBUG
    if (ADI_BEEP_INVALID_HANDLE(hDevice)) {
        return ADI_BEEP_BAD_DEV_HANDLE;
    }
    
    if (ADI_BEEP_STATE_UNINITIALIZED == pDevice->pData->state) {
        return ADI_BEEP_NOT_INITIALIZED;
    }
#endif

    ADI_ENTER_CRITICAL_REGION();

    /* make a hole, and disable the beeper */
    pReg->CFG &= (uint16_t)~(BITM_BEEP_CFG_SEQREPEAT | BITM_BEEP_CFG_AENDIRQ |BITM_BEEP_CFG_EN);

    pReg->TONEA =  (  (uint16_t)((uint16_t)noteA.frequency << ADI_BEEP_TONE_FREQ_BITPOS)
                   |(uint16_t)((uint16_t)noteA.duration << ADI_BEEP_TONE_DUR_BITPOS)   );

    pReg->TONEB =  (  (uint16_t)((uint16_t)noteB.frequency << ADI_BEEP_TONE_FREQ_BITPOS)
                   |(uint16_t)((uint16_t)noteB.duration << ADI_BEEP_TONE_DUR_BITPOS)   );
    
    /* program new sequence count, while preserving everything else */
    pReg->CFG |= (BITM_BEEP_CFG_EN | BITM_BEEP_CFG_SEQATENDIRQ |(uint16_t)((uint16_t)count << BITP_BEEP_CFG_SEQREPEAT));

    /* We're now playing but not blocked */
    pDevice->pData->state |= (ADI_BEEP_STATE_PLAYING);
    ADI_EXIT_CRITICAL_REGION();

    return ADI_BEEP_SUCCESS;
}

/*!
 * @brief  Enable or disable the beeper. Other APIs will automatically enable the beeper if required,
 *         so this function is best used in the following situations:
 *            - when only using static configuration, i.e. start playing the notes
 *              set up in static adi_beep_config.h.  
 *            - Otherwise, this can be used to stop the beeper during playback, 
 *              when started from any other API.
 *
 * @param[in]   hDevice       Device handle obtained from #adi_beep_Open().
 * @param[in]   bFlag         true to enable the device, false to stop playback.
 *
 * @return      Status
 *                - #ADI_BEEP_SUCCESS                Success: Call completed successfully.
 *                - #ADI_BEEP_BAD_DEV_HANDLE [D]     Error: Invalid device handle parameter.
 *                - #ADI_BEEP_NOT_INITIALIZED [D]    Error: Device has not been initialized for use, see #adi_beep_Open().
 *
 * @sa        adi_beep_Open().
 */
ADI_BEEP_RESULT adi_beep_Enable(ADI_BEEP_HANDLE const hDevice, bool   const bFlag)
{
    ADI_BEEP_DRIVER    *pDevice;
    ADI_BEEP_TypeDef   *pReg;
    ADI_INT_STATUS_ALLOC();
    
    pDevice = (ADI_BEEP_DRIVER*)hDevice;
    pReg = pDevice->pReg;

#ifdef ADI_DEBUG
    if (ADI_BEEP_INVALID_HANDLE(hDevice)) {
        return ADI_BEEP_BAD_DEV_HANDLE;
    }
    
    if (ADI_BEEP_STATE_UNINITIALIZED == pDevice->pData->state) {
        return ADI_BEEP_NOT_INITIALIZED;
    }
#endif

    ADI_ENTER_CRITICAL_REGION();

    if (bFlag == true) {
      /* All the registers should already be set - just enable the beep */
      pReg->CFG |= BITM_BEEP_CFG_EN;
      pDevice->pData->state |= (ADI_BEEP_STATE_PLAYING);
    } 
    else {
      pReg->CFG &= (uint16_t)~(BITM_BEEP_CFG_EN);
      pDevice->pData->state &= ~(ADI_BEEP_STATE_PLAYING);
    }

    ADI_EXIT_CRITICAL_REGION();

    return ADI_BEEP_SUCCESS;
}

/*!
 * @brief  Wait for the current playback to finish.  This is a blocking call,
 *         that will not return until the current playback (if any) has finished.
 *         If there is no current playback, it will return immediately.
 *
* @param[in]   hDevice       Device handle obtained from #adi_beep_Open().
 *
 * @return      Status
 *                - #ADI_BEEP_SUCCESS                Success: Call completed successfully.
 *                - #ADI_BEEP_FAILURE                Error: Semaphore failure.
 *                - #ADI_BEEP_BAD_DEV_HANDLE [D]     Error: Invalid device handle parameter.
 *                - #ADI_BEEP_NOT_INITIALIZED [D]    Error: Device has not been initialized for use, see #adi_beep_Open().
 *
 * @sa        adi_beep_Open().
 */
ADI_BEEP_RESULT adi_beep_Wait(ADI_BEEP_HANDLE const hDevice)
{
    ADI_BEEP_DRIVER    *pDevice;
    bool               wait = false;
    ADI_INT_STATUS_ALLOC();
    
    pDevice = (ADI_BEEP_DRIVER*)hDevice;

#ifdef ADI_DEBUG
    if (ADI_BEEP_INVALID_HANDLE(hDevice)) {
        return ADI_BEEP_BAD_DEV_HANDLE;
    }
    
    if (ADI_BEEP_STATE_UNINITIALIZED == pDevice->pData->state) {
        return ADI_BEEP_NOT_INITIALIZED;
    }
#endif

    ADI_ENTER_CRITICAL_REGION();
    
    if((pDevice->pData->state | ADI_BEEP_STATE_PLAYING) > 0u) {
       /* We are going to pend on the semaphore, no matter what. */
       pDevice->pData->state |= ADI_BEEP_STATE_BLOCKED;
       wait = true;
    }

    ADI_EXIT_CRITICAL_REGION();

    if(wait == true) {
       /* Wait for the completion interrupt to post */
       SEM_PEND(pDevice->pData, ADI_BEEP_SEMAPHORE_FAILED);
    }
    
    return ADI_BEEP_SUCCESS;
}

/*! \cond PRIVATE */

/*! @brief  BEEP device driver interrupt handler.  Overrides weakly-bound 
 *  default interrupt handler in the startup file. */
void Beep_Int_Handler(void)
{
    ISR_PROLOG();    
#if ADI_BEEP_INCLUDE_PLAY_SEQUENCE == 1
    ADI_BEEP_DEV_DATA  *pData;
    ADI_BEEP_NOTE       noteA, noteB;
#endif
    ADI_BEEP_DRIVER *pDevice = &adi_beep_Device[ADI_BEEP_DEVID_0];  /* so far, there is only one BEEP, so this is safe */
    ADI_BEEP_TypeDef *pReg = pDevice->pReg;
    uint16_t          fired = ADI_BEEP_ALL_INTERRUPTS;
    register uint16_t candidate;
    
    /* Make sure our driver is up and running. */
    if (ADI_BEEP_STATE_UNINITIALIZED != pDevice->pData->state) {
        
        /* read both status and mask registers */
        candidate = pReg->CFG & ADI_BEEP_ALL_INTERRUPTS;  /* Take the fired interrupts */
        fired = candidate;                                /* ...and a copy. */
        candidate = candidate & pReg->STAT;               /* ...and remove the unused set interrupt bits */
        
        /* From this driver's perspective, there are only two states
         * to watch for - finished playing, or continuing the playing sequence.  
         * Finished will be handled here. */
        if((candidate & (BITM_BEEP_CFG_SEQATENDIRQ | BITM_BEEP_CFG_AENDIRQ)) > 0u) { 
          
          /* If we are blocked, unblock by posting the semaphore */
          if((pDevice->pData->state | ADI_BEEP_STATE_BLOCKED) > 0u) {
            SEM_POST(pDevice->pData);
          }
          
          /* Reset the device playing status. */
          pDevice->pData->state &= ~(ADI_BEEP_STATE_PLAYING | ADI_BEEP_STATE_BLOCKED);
            
          /* ...and disable the device. */
          pReg->CFG &= (uint16_t)(~(BITM_BEEP_CFG_EN));
          
          /* forward the interrupt to the user if they are watching it and it has fired */
          /* pass the interrupt as the event. */
          if (pDevice->pData->cbFunc != NULL) {
              pDevice->pData->cbFunc (pDevice->pData->cbParam, (uint32_t)candidate, NULL);
          }
        }
            
 #if ADI_BEEP_INCLUDE_PLAY_SEQUENCE == 1
        /* The second state is if we are playing a longer sequence, so this 
         * interrupt may be to move the sequence along.  */
        if ((BITM_BEEP_CFG_BSTARTIRQ & candidate) != 0u) {
                
               /* Get a local copy of data, to shorten the following code. */ 
                pData = pDevice->pData;     
                
                /* If there's still data to play */
                if(pData->nSeqIndex < pData->nSeqMax) {
                  /* Move the sequence along.*/
                  noteA = (*pData->pSeqArray)[pData->nSeqIndex];
                  pData->nSeqIndex++;
                  noteB = (*pData->pSeqArray)[pData->nSeqIndex];
                  pData->nSeqIndex++;
                  
                  /* Any values written will not impact the current tones,
                   * they will take effect after the current tone is completed */
                  pReg->TONEA =  (  (uint16_t)((uint16_t)noteA.frequency << ADI_BEEP_TONE_FREQ_BITPOS)
                                  | (uint16_t)((uint16_t)noteA.duration << ADI_BEEP_TONE_DUR_BITPOS)   );
   
                  pReg->TONEB =  (  (uint16_t)((uint16_t)noteB.frequency << ADI_BEEP_TONE_FREQ_BITPOS)
                                  | (uint16_t)((uint16_t)noteB.duration << ADI_BEEP_TONE_DUR_BITPOS)   );
                }
        }
#endif
    }

    /* clear the watched interrupt(s) that fired */
    pReg->STAT |= (uint16_t)(fired & ADI_BEEP_ALL_INTERRUPTS);  /* only write allowed interrupt bits */
    ISR_EPILOG();    
}
/*! \endcond */

/*@}*/



