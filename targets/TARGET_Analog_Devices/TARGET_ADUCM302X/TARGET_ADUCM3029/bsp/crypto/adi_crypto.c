/*! *****************************************************************************
 * @file:    adi_crypto.c
 * @brief:   CRYPTO device driver source file.
 * @details: This is the Crypto driver implementation file.
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

/*! \addtogroup Crypto_Driver Crypto Driver
 *  @{
 *
 *  @brief <b>Crypto Driver</b>
 *
 *  @details
 *
 *  The Crypto controller provides hardware acceleration of various AES cryptographic
 *  cipher modes, including: ECB, CBC, CTR, CMAC, CCM, SHA-256 and Keyed HMAC; as well
 *  as Protected Key Storage (PKSTOR) operations for safely storing and using encrypted
 *  keys.  The Crypto block works most efficiently in DMA mode due to the large about
 *  of data I/O which would otherwise incur a lot of PIO-mode interrupt traffic to manually
 *  pump data.
 *
 *  <b>Crypto Driver Static Configuration</b>
 *
 *  A number of Crypto cipher modes are able to be configured statically, such that
 *  if particular mode(s) are not required, the resulting driver footprint can be reduced
 *  internally by blocking out chunks of code that are not needed.
 *
 * @note - The application must include drivers/crypto/adi_crypto.h to use this driver.
 * @note - This driver optionally uses the DMA driver if DMA is selected and active.
 *         In this case, the application must include the DMA driver sources to resolve
 *         DMA symbols.
 */


/*========  I N C L U D E  ========*/

/*! \cond PRIVATE */
#include <adi_processor.h>
#include <assert.h>
#include <string.h>

/* main crypto include file */
#include <drivers/crypto/adi_crypto.h>

/* private crypto defines */
#include "adi_crypto_def.h"

#if (ADI_CRYPTO_ENABLE_DMA_SUPPORT == 1)
/* dma interface */
#include <drivers/dma/adi_dma.h>
#endif


/*========  D E F I N E S   ========*/

#ifdef __ICCARM__
/*
* IAR MISRA C 2004 error suppressions.
*
* Pm123 (rule  8.5): there shall be no definition of objects or functions in a header file
* Pm140 (rule 11.3): a cast should not be performed between a pointer type and an integral type
* Pm050 (rule 14.2): a null statement shall only occur on a line by itself
* Pm088 (rule 17.4): pointer arithmetic should not be used.
* Pm073 (rule 14.7): a function should have a single point of exit
* Pm143 (rule 14.7): a function should have a single point of exit at the end of the function
* Pm152 (rule 17.4): array indexing shall only be applied to objects defined as an array type
*/
#pragma diag_suppress=Pm123,Pm140,Pm050,Pm088,Pm073,Pm143,Pm152
#endif /* __ICCARM__ */

/* Utility Macros */
#define CLR_BITS(REG,BITS)                        ((REG) &= ~(BITS))
#define SET_BITS(REG,BITS)                        ((REG) |= (BITS))
#define IS_ANY_BIT_SET(REG,BITS)                  (((REG) & (BITS)) != 0u)


/* Number of crypto device for the given processor */
#define NUM_DEVICES                               (1u)

/* Compiler-specific mapping of assembly-level byte-swap instruction
   IAR is "__REV", and we think Keil is "__rev", but lets see how that
   goes when it is undefined for Keil.
*/
#if defined ( __ICCARM__ )
#define __ADI_BYTE_SWAP(X)                        __REV(X)
#elif defined (__GNUC__)
#define __ADI_BYTE_SWAP(X)                        __builtin_bswap32(X)
#elif defined (__CC_ARM)
#define __ADI_BYTE_SWAP(X)                        __rev(X)
#else
#error "This toolchain is not supported"
#endif


/*========  L O C A L    F U N C    D E C L  ========*/

#if (ADI_CRYPTO_ENABLE_DMA_SUPPORT == 1)
static void dmaCallback                      (void *pCBParam, uint32_t Event, void *pArg);
#endif

#ifdef ADI_DEBUG
/* Validatation routines */
static ADI_CRYPTO_RESULT ValidateHandle      (ADI_CRYPTO_HANDLE const hDevice);
static ADI_CRYPTO_RESULT ValidateUserBuffer  (ADI_CRYPTO_TRANSACTION * const pBuffer);
#endif

/* Generate a uint32_t value from a pointer to a uint8_t buffer */
static uint32_t u32FromU8p                   (uint8_t * const pFourBytes);

/* load KEY registers with provided key */
static void loadAesKey                       (uint8_t * const pKey, ADI_CRYPTO_AES_KEY_LEN const keyLen);

/* Initialize the internal device handle object (user memory) */
static void InitializeDevData                (ADI_CRYPTO_HANDLE const hDevice);

/* Initiate the computation for a buffer */
static void StartCompute                     (ADI_CRYPTO_HANDLE const hDevice);

/* Stop the device */
static void StopCompute                      (ADI_CRYPTO_HANDLE const hDevice);

#if (ADI_CRYPTO_ENABLE_DMA_SUPPORT == 1)
static void programDMA                       (ADI_CRYPTO_HANDLE const hDevice);
#endif

/* PIO mode write input data */
static void writePioInputData                (ADI_CRYPTO_HANDLE const hDevice, uint32_t const status);

/* PIO mode read output data */
static void readPioOutputData                (ADI_CRYPTO_HANDLE const hDevice, uint32_t const status);

/* Flush the input and output buffers */
static void FlushInputOutputRegisters        (ADI_CRYPTO_HANDLE const hDevice);


/* pre-defined Crypto interrupt handler prototypes, as linked in IVT */
void Crypto_Int_Handler(void);
#if (ADI_CRYPTO_ENABLE_DMA_SUPPORT == 1)
void DMA_AES0_IN_Int_Handler (void);
void DMA_AES0_OUT_Int_Handler (void);
#endif


/*========  D A T A  ========*/
/* Internal device structure */

static CRYPTO_INFO CryptoDevInfo[] = {
    {pADI_CRYPT0,          /* physical device controller pointer */
     NULL,                 /* hDevice */
#if (ADI_CRYPTO_ENABLE_DMA_SUPPORT == 1)
     DMA0_CH13_DONE_IRQn,  /* DMA input interrupt number */
     DMA0_CH14_DONE_IRQn,  /* DMA output interrupt number */
     AES0_IN_CHANn,        /* DMA input channel */
     AES0_OUT_CHANn,       /* DMA output channel */
     ADI_CRYPTO_SUCCESS,   /* DMA error state */
#endif
    }
};

/*! \endcond */

/*========  C O D E  ========*/


/* include PKSTOR extensions into CRYPTO driver... */
#if (1 == ADI_CRYPTO_ENABLE_PKSTOR_SUPPORT)
#include "adi_pkstor.c"
#endif


#if (ADI_CRYPTO_ENABLE_DMA_SUPPORT == 1)

/* Internal Crypto registered DMA Callback for receiving DMA
   fault notifications from the shared DMA error handler */
static void dmaCallback(void *pCBParam, uint32_t Event, void *pArg)
{
    /* recover device handle */
    ADI_CRYPTO_HANDLE hDevice = CryptoDevInfo[0].hDevice;

    /* recover failing channel number */
    uint32_t failingChannel = (uint32_t)pCBParam;

    /* save the DMA error */
    switch (Event) {
        case ADI_DMA_EVENT_ERR_BUS:
            hDevice->dmaErrorCode = ADI_CRYPTO_ERR_DMA_BUS_FAULT;
            break;
        case ADI_DMA_EVENT_ERR_INVALID_DESCRIPTOR:
            hDevice->dmaErrorCode = ADI_CRYPTO_ERR_DMA_INVALID_DESCR;
            break;
        default:
            hDevice->dmaErrorCode = ADI_CRYPTO_ERR_DMA_UNKNOWN_ERROR;
            break;
    }

    /* transfer is toast... post semaphore to unblock any waiters */
    SEM_POST(hDevice);

    /* call user's callback */
    if (0u != hDevice->pfCallback) {
        hDevice->pfCallback (hDevice->pCBParam, (uint32_t)hDevice->dmaErrorCode, (void*)failingChannel);
    }

    /* game over... */
    StopCompute(hDevice);
}
#endif


#ifdef ADI_DEBUG
/* Validate the given handle */
static ADI_CRYPTO_RESULT ValidateHandle(ADI_CRYPTO_HANDLE const hDevice)
{
    ADI_CRYPTO_RESULT result = ADI_CRYPTO_ERR_BAD_DEV_HANDLE;
    uint32_t x;

    for (x = 0u; x < NUM_DEVICES; x++) {
        if (CryptoDevInfo[x].hDevice == hDevice) {
            result = ADI_CRYPTO_SUCCESS;
            break;
        }
    }

    return result;
}
#endif


#ifdef ADI_DEBUG
static ADI_CRYPTO_RESULT ValidateUserBuffer(ADI_CRYPTO_TRANSACTION * const pBuffer)
{

    /* null pointer and zero count checks */
    if (
           (pBuffer->pInputData     == NULL)
        || (pBuffer->numInputBytes  == 0u)
        || (pBuffer->pOutputData    == NULL)
        || (pBuffer->numOutputBytes == 0u)
        || (
               (pBuffer->eAesByteSwap != ADI_CRYPTO_AES_LITTLE_ENDIAN)
            && (pBuffer->eAesByteSwap != ADI_CRYPTO_AES_BIG_ENDIAN))
            )
    {
        return ADI_CRYPTO_ERR_BAD_BUFFER;
    }

    /* check buffer pointers for 32-bit alignment */
    if ( (0u != (3u & (uint32_t)pBuffer->pAuthData)) || (0u != (3u & (uint32_t)pBuffer->pInputData)) || (0u != (3u & (uint32_t)pBuffer->pOutputData)) ) {
        return ADI_CRYPTO_ERR_BAD_BUFFER;
    }

#if (ADI_CRYPTO_ENABLE_DMA_SUPPORT == 1)
    /* check buffer sizes for max DMA size */
    if ((MAX_CRYPTO_DMA_BYTES < pBuffer->numAuthBytes) || (MAX_CRYPTO_DMA_BYTES < pBuffer->numInputBytes) || (MAX_CRYPTO_DMA_BYTES < pBuffer->numOutputBytes)) {
        return ADI_CRYPTO_ERR_BAD_BUFFER;
    }
#endif

#if ADI_CRYPTO_ENABLE_SHA_SUPPORT == 1
    if (pBuffer->eCipherMode == ADI_CRYPTO_MODE_SHA)
    {
        /* SHA output digest is 256-bit and hence the output buffer size should be at least 32 bytes */
        if (pBuffer->numOutputBytes < SHA_OUTPUT_SIZE_IN_BYTES) {
            return ADI_CRYPTO_ERR_BAD_BUFFER;
        }
    }
    else
#endif
    {

#if ADI_CRYPTO_ENABLE_CMAC_SUPPORT == 1
        if (pBuffer->eCipherMode == ADI_CRYPTO_MODE_CMAC) {
            /* CMAC output is always a 128-bit block */
            if (pBuffer->numOutputBytes < CRYPTO_INPUT_SIZE_IN_BYTES) {
                return ADI_CRYPTO_ERR_BAD_BUFFER;
            }
        }
        else
#endif
        {
            if (
                   (pBuffer->pKey == NULL)
                || (   (pBuffer->eAesKeyLen  != ADI_CRYPTO_AES_KEY_LEN_128_BIT)
                    && (pBuffer->eAesKeyLen  != ADI_CRYPTO_AES_KEY_LEN_256_BIT))
                || (   (pBuffer->eCodingMode != ADI_CRYPTO_ENCODE)
                    && (pBuffer->eCodingMode != ADI_CRYPTO_DECODE)))
            {
                return ADI_CRYPTO_ERR_BAD_CONFIG;
            }

#if ADI_CRYPTO_ENABLE_CTR_SUPPORT == 1
            if (pBuffer->eCipherMode == ADI_CRYPTO_MODE_CTR)
            {
                if ((pBuffer->CounterInit & (0xFFF00000u)) != 0u) {
                    return ADI_CRYPTO_ERR_BAD_BUFFER;
                }
            }
#endif

#if ADI_CRYPTO_ENABLE_CCM_SUPPORT == 1
            if (pBuffer->eCipherMode == ADI_CRYPTO_MODE_CCM)
            {
                if ( ((pBuffer->CounterInit & (0xFFFF0000u)) != 0u)
                    ||  (   (pBuffer->pAuthData != NULL)
                                && (
                                       (pBuffer->numAuthBytes == 0u)
                                    || (pBuffer->numValidBytes == 0u)
                                    || (pBuffer->numValidBytes > CRYPTO_INPUT_SIZE_IN_BYTES)
                                    || (pBuffer->numOutputBytes < (pBuffer->numInputBytes + CRYPTO_INPUT_SIZE_IN_BYTES))
                                    )
                        )
                 )
                {
                    return ADI_CRYPTO_ERR_BAD_BUFFER;
                }
            }
            else
#endif
            {
                if (pBuffer->numOutputBytes < pBuffer->numInputBytes)
                {
                    return ADI_CRYPTO_ERR_BAD_BUFFER;
                }
            }
        }
    }

/* FIXME: Issue http://labrea.ad.analog.com/browse/MSKEW-299 describes missing support
   for HMAC mode, so reject HMAC submits until support for this mode is implemented.
   ***REMOVE THIS BLOCK WHEN HMAC SUPPORT IS ADDED***
*/
#if defined (__ADUCM4x50__)
#if ADI_CRYPTO_ENABLE_HMAC_SUPPORT == 1
	if (pBuffer->eCipherMode == ADI_CRYPTO_MODE_HMAC)
	{
		return ADI_CRYPTO_ERR_BAD_BUFFER;
	}
#endif
#endif /*ADUCM4x50__*/

    return ADI_CRYPTO_SUCCESS;
}
#endif


/**
 * @brief    Opens a Crypto device instance.
 *
 * @param [in]  nDeviceNum   Device number to open.
 * @param [in]  pMemory      Pointer to a #ADI_CRYPTO_MEMORY_SIZE sized buffer to manage the device
 *                           instance.
 * @param [in]  nMemorySize  Size of the buffer to which "pMemory" points.
 * @param [out] phDevice     Pointer to a location where the Crypto device handle is to be written.
 *
 * @return   Status
 *           - #ADI_CRYPTO_SUCCESS                      Call completed successfully.
 *           - #ADI_CRYPTO_ERR_BAD_DEVICE_NUM       [D] Error: The device number is invalid.
 *           - #ADI_CRYPTO_ERR_INVALID_PARAM        [D] Error: A parameter is invalid.
 *           - #ADI_CRYPTO_ERR_INSUFFICIENT_MEM     [D] Error: The memory passed to the device is insufficient.
 *           - #ADI_CRYPTO_ERR_ALREADY_INITIALIZED  [D] Error: The device is already opened.
 *           - #ADI_CRYPTO_ERR_SEMAPHORE_FAILED         Error: Unable to create semaphore.
 *           - #ADI_CRYPTO_ERR_DMA_REGISTER             Error: Unable to register DMA error callback function.
 *
 * @sa adi_crypto_Close().
 */
ADI_CRYPTO_RESULT adi_crypto_Open (uint32_t const nDeviceNum, void * const pMemory, uint32_t const nMemorySize, ADI_CRYPTO_HANDLE * const phDevice)
{
    ADI_CRYPTO_HANDLE hDevice = NULL;

#ifdef ADI_DEBUG
    if (nDeviceNum >= NUM_DEVICES) {
        return ADI_CRYPTO_ERR_BAD_DEVICE_NUM;
    }

    if ((pMemory == NULL) || (phDevice == NULL)) {
        return ADI_CRYPTO_ERR_INVALID_PARAM;
    }

    if (nMemorySize < ADI_CRYPTO_MEMORY_SIZE) {
        return ADI_CRYPTO_ERR_INSUFFICIENT_MEM;
    }

    if (CryptoDevInfo[nDeviceNum].hDevice != NULL) {
        return ADI_CRYPTO_ERR_ALREADY_INITIALIZED;
    }

    /* reality checks */
    assert (ADI_CRYPTO_MEMORY_SIZE == sizeof(ADI_CRYPTO_DEV_DATA_TYPE));
    assert (sizeof(ADI_CRYPTO_TRANSACTION) == sizeof(CRYPTO_COMPUTE));

#endif /* ADI_DEBUG */

    /* store a bad handle in case of failure */
    *phDevice = NULL;

    /* point local device handle to the user memory */
    hDevice = (ADI_CRYPTO_HANDLE)pMemory;

    /* link CRYPTO controller register set */
    hDevice->pDev = CryptoDevInfo[nDeviceNum].pDev;

    /* link device info */
    hDevice->pDevInfo = CryptoDevInfo;

    /* cross-link device handle into device info */
    CryptoDevInfo[nDeviceNum].hDevice = hDevice;

    /* Initialize the driver internals */
    InitializeDevData(hDevice);

    /* create the semaphore */
    SEM_CREATE(hDevice, "crypto_sem", ADI_CRYPTO_ERR_SEMAPHORE_FAILED);

#if (ADI_CRYPTO_ENABLE_DMA_SUPPORT == 1)
    /* initialize DMA core */
    adi_dma_Init();

    /* register DMA error callback for INPUT channel */
    if (ADI_DMA_SUCCESS != adi_dma_RegisterCallback(hDevice->pDevInfo->dmaInputChanNum, dmaCallback, (void*)hDevice)) {
        /* uninitialize crypto driver and fail */
        adi_crypto_Close(hDevice);
        return  ADI_CRYPTO_ERR_DMA_REGISTER;
    }
    /* register DMA error callback for OUTPUT channel */
    if (ADI_DMA_SUCCESS != adi_dma_RegisterCallback(hDevice->pDevInfo->dmaOutputChanNum, dmaCallback, (void*)hDevice)) {
        /* uninitialize crypto driver and fail */
        adi_crypto_Close(hDevice);
        return  ADI_CRYPTO_ERR_DMA_REGISTER;
    }
#endif

    /* Give the handle back to the application */
    *phDevice = hDevice;

    /* Return success */
    return ADI_CRYPTO_SUCCESS;
}

/**
 * @brief    Close the given device instance.
 *
 * @param [in]  hDevice  Handle to the device instance.
 *
 * @return   Status
 *           - #ADI_CRYPTO_SUCCESS                  Successfully closed the device.
 *           - #ADI_CRYPTO_ERR_BAD_DEV_HANDLE   [D] Error: Handle Passed is invalid.
 *           - #ADI_CRYPTO_ERR_SEMAPHORE_FAILED     Error: Unable to delete semaphore.
 *
 * @sa adi_crypto_Open().
 */
ADI_CRYPTO_RESULT adi_crypto_Close (ADI_CRYPTO_HANDLE const hDevice)
{
    uint32_t x;
    ADI_CRYPTO_RESULT result;

#ifdef ADI_DEBUG
    if ((result = ValidateHandle(hDevice)) != ADI_CRYPTO_SUCCESS) {
        return result;
    }
#endif /* ADI_DEBUG */

    /* IF (The device is enabled) */
    if (hDevice->bDeviceEnabled) {
        result = adi_crypto_Enable(hDevice, false);
        if (result != ADI_CRYPTO_SUCCESS) {
            return result;
        }
    }

    /* Destroy the semaphore */
    SEM_DELETE(hDevice, ADI_CRYPTO_ERR_SEMAPHORE_FAILED);

    /* Close the device */
    for (x=0u; x < NUM_DEVICES; x++) {
        if (CryptoDevInfo[x].hDevice == hDevice) {
            CryptoDevInfo[x].hDevice = NULL;
            break;
        }
    }

    return ADI_CRYPTO_SUCCESS;
}


/**
 * @brief   Register a user callback function.
 *
 * @param [in]  hDevice     Handle to the device instance.
 * @param [in]  pfCallback  Function pointer to user callback function. Passing a NULL pointer will
 *                          unregister the callback function.
 * @param [in]  pCBParam    Callback function parameter.
 *
 * @details  This function registers a user callback function.  The registered function will be called when
 *           the given computation is over.  Registering an active user callback function implies use of the
 *           (non-blocking) CALLBACK mode during which any subsequent calls to the (blocking-mode)
 *           #adi_crypto_GetBuffer() API will be rejected.
 *
 *
 * @return   Status
 *           - #ADI_CRYPTO_SUCCESS                  Successfully registered the callback.
 *           - #ADI_CRYPTO_ERR_BAD_DEV_HANDLE   [D] Error: Handle Passed is invalid.
 */
 ADI_CRYPTO_RESULT adi_crypto_RegisterCallback (ADI_CRYPTO_HANDLE const hDevice, ADI_CALLBACK const pfCallback, void * const pCBParam)
{
#ifdef ADI_DEBUG
    ADI_CRYPTO_RESULT result;

    if ((result = ValidateHandle(hDevice)) != ADI_CRYPTO_SUCCESS) {
        return result;
    }
#endif /* ADI_DEBUG */

    /* store user's callback values (critical section) */
    ADI_INT_STATUS_ALLOC();
    ADI_ENTER_CRITICAL_REGION();
		hDevice->pfCallback = pfCallback;
		hDevice->pCBParam = pCBParam;
    ADI_EXIT_CRITICAL_REGION();

    return ADI_CRYPTO_SUCCESS;
}


/**
 * @brief    Submit a Crypto transaction buffer for processing.
 *
 * @param [in]  hDevice   Handle to the device instance.
 * @param [in]  pBuffer   Pointer to the #ADI_CRYPTO_TRANSACTION structure which contains details
 *                        of the cipher-dependent buffer elements required by the driver.
 *
 * @return  Status
 *           - #ADI_CRYPTO_SUCCESS                  Successfully submitted the buffer.
 *           - #ADI_CRYPTO_ERR_COMPUTE_ACTIVE       Error: Buffer already submitted.
 *           - #ADI_CRYPTO_ERR_BAD_DEV_HANDLE   [D] Error: Handle Passed is invalid.
 *           - #ADI_CRYPTO_ERR_BAD_BUFFER       [D] Error: The buffer passed to the device is invalid or unsupported.
 *
 * The buffer submitted is queued for eventual CRYPTO processing.  A single buffer may be submitted
 * prior to initiating CRYPTO buffer processing.  Buffer processing is initiated with the
 * #adi_crypto_Enable() call.  As buffer processing is completed, the buffer (and result info)
 * is retrieved with the #adi_crypto_GetBuffer() API or through the user callback notification.
 *
 * @note    The driver takes ownership of the ADI_CRYPTO_TRANSACTION structure passed to the driver.
 *          The application must insure the structure is not used and its scope is valid until
 *          the structure is returned back to the application.
 *
 * @warning    The #ADI_CRYPTO_TRANSACTION buffer is a common superset of all possible cipher mode parameters.
 *          As such, not all parameters pertain to each cipher mode.  It is recommended users clear unused
 *          parameters prior to configuration for the particular cipher mode.  The example provided
 *          illustrates this with a call to: "memset(&Buffer, 0, sizeof(ADI_CRYPTO_TRANSACTION));"
 *          before configuring and then submitting each transaction.
 *
 * @sa adi_crypto_Enable().
 * @sa adi_crypto_GetBuffer().
 * @sa adi_crypto_IsBufferAvailable().
 */
 ADI_CRYPTO_RESULT adi_crypto_SubmitBuffer (ADI_CRYPTO_HANDLE const hDevice, ADI_CRYPTO_TRANSACTION * const pBuffer)
{
    ADI_CRYPTO_RESULT result = ADI_CRYPTO_SUCCESS;

    /* reject if we already have a user buffer */
    if (NULL != hDevice->pUserBuffer) {
        /* computation already active */
        return ADI_CRYPTO_ERR_COMPUTE_ACTIVE;
    }

#ifdef ADI_DEBUG
    if (ADI_CRYPTO_SUCCESS != (result = ValidateHandle(hDevice))) {
        return result;
    }

    /* validate user Buffer */
    if (ADI_CRYPTO_SUCCESS != (result = ValidateUserBuffer(pBuffer))) {
        return result;
    }
#endif

    /* store user buffer pointer to return later */
    hDevice->pUserBuffer = pBuffer;

    /* initialize internal compute state from user buffer */
    memcpy(&hDevice->Computation, pBuffer, sizeof(ADI_CRYPTO_TRANSACTION));

    /* don't initiate transaction until we get adi_crypto_Enable() */

	/* reset dma error code */
	hDevice->dmaErrorCode = ADI_CRYPTO_SUCCESS;

    return result;
}


/**
 * @brief   Get the submitted transaction buffer back from the driver.
 *
 * @param [in]  hDevice    Handle to the device instance.
 * @param [out] ppBuffer   Pointer to a location to which the address of the buffer structure is written.
 *
 * @return   Status
 *           - #ADI_CRYPTO_SUCCESS                      Successfully got a buffer.
 *           - #ADI_CRYPTO_ERR_INVALID_PARAM        [D] Error: Pointer to the buffer is NULL.
 *           - #ADI_CRYPTO_ERR_BAD_DEV_HANDLE       [D] Error: Handle Passed is invalid.
 *           - #ADI_CRYPTO_ERR_DMA_BUS_FAULT            Error: DMA bus fault was reported.
 *           - #ADI_CRYPTO_ERR_DMA_INVALID_DESCR        Error: Invalid DMA descriptor was reported.
 *           - #ADI_CRYPTO_ERR_DMA_UNKNOWN_ERROR        Error: An unexpected DMA error was reported.
 *           - #ADI_CRYPTO_ERR_SEMAPHORE_FAILED         Error: Semaphore pend request failed.
 *           - #ADI_CRYPTO_ERR_INVALID_STATE            Error: Invalid call when using callback mode.
 *
 *  This is a blocking call and will await transaction completion (if not already).
 *  This function should not be called if a callback function is registered.
 *
 * @sa adi_crypto_SubmitBuffer().
 * @sa adi_crypto_IsBufferAvailable().
 */
ADI_CRYPTO_RESULT adi_crypto_GetBuffer (ADI_CRYPTO_HANDLE const hDevice, ADI_CRYPTO_TRANSACTION ** const ppBuffer)
{
    ADI_CRYPTO_RESULT result = ADI_CRYPTO_SUCCESS;

#ifdef ADI_DEBUG

    if (ppBuffer == NULL) {
        return ADI_CRYPTO_ERR_INVALID_PARAM;
    }
    if (ADI_CRYPTO_SUCCESS != (result = ValidateHandle(hDevice))) {
        return result;
    }
#endif /* ADI_DEBUG */

	if (NULL != hDevice->pfCallback) {
		return ADI_CRYPTO_ERR_INVALID_STATE;
	}

    /* pend on completion (even if already complete) */
    SEM_PEND(hDevice, ADI_CRYPTO_ERR_SEMAPHORE_FAILED);

    /* give back the user's buffer */
    *ppBuffer = hDevice->pUserBuffer;

    /* clear internal user buffer pointer */
    hDevice->pUserBuffer = NULL;

	/* if we had a DMA error, return that instead of success */
	if (ADI_CRYPTO_SUCCESS != hDevice->dmaErrorCode) {
		result = hDevice->dmaErrorCode;
	}

    return result;
}


/**
 * @brief    Peek function to know whether a submitted transaction is complete.
 *
 * @param [in]  hDevice       Handle to the device instance.
 * @param [in]  pbAvailable   Pointer to a Boolean variable. Set to "true" if there is a completed
 *                            buffer and a call to adi_crypto_GetBuffer is ensured to be successful.
 *                            Set to "false" if there is no completed buffer.
 *
 * @return   Status
 *           - #ADI_CRYPTO_SUCCESS                  Successfully peeked for a buffer.
 *           - #ADI_CRYPTO_ERR_INVALID_PARAM    [D] Error: The pointer passed is NULL.
 *           - #ADI_CRYPTO_ERR_BAD_DEV_HANDLE   [D] Error: Handle Passed is invalid.
 *           - #ADI_CRYPTO_ERR_DMA_BUS_FAULT        Error: DMA bus fault was reported.
 *           - #ADI_CRYPTO_ERR_DMA_INVALID_DESCR    Error: Invalid DMA descriptor was reported.
 *           - #ADI_CRYPTO_ERR_DMA_UNKNOWN_ERROR    Error: An unexpected DMA error was reported.
 *
 * @sa adi_crypto_SubmitBuffer().
 * @sa adi_crypto_GetBuffer().
 */
ADI_CRYPTO_RESULT adi_crypto_IsBufferAvailable (ADI_CRYPTO_HANDLE const hDevice, bool * const pbAvailable)
{
    ADI_CRYPTO_RESULT result = ADI_CRYPTO_SUCCESS;

#ifdef ADI_DEBUG
    if (pbAvailable == NULL)
    {
        return ADI_CRYPTO_ERR_INVALID_PARAM;
    }
    if (ADI_CRYPTO_SUCCESS != (result = ValidateHandle(hDevice))) {
        return result;
    }
#endif /* ADI_DEBUG */

    /* let the respective PIO/DMA interrupts drive completion... just return that state here */
    *pbAvailable = hDevice->bCompletion;

	/* if we had a DMA error, return that instead of success */
	if (ADI_CRYPTO_SUCCESS != hDevice->dmaErrorCode) {
		result = hDevice->dmaErrorCode;
	}

    return result;
}


/**
 * @brief    Enable/Disable the device.  Enabling the device causes the submitted buffer to be processed.
 *
 * @param [in]  hDevice   Handle to the device instance.
 * @param [in]  bEnable   'true' to enable and 'false' to disable the device.
 *
 * @return   Status
 *           - #ADI_CRYPTO_SUCCESS                  Successfully enabled/disabled the device.
 *           - #ADI_CRYPTO_ERR_BAD_DEV_HANDLE   [D] Error: Handle Passed is invalid.
 *           - #ADI_CRYPTO_ERR_INVALID_STATE    [D] Error: Calling enable when device is already enabled or
 *                                                         disable when the device is already disabled.
 *
 */
ADI_CRYPTO_RESULT adi_crypto_Enable (ADI_CRYPTO_HANDLE const hDevice, bool const bEnable)
{
    ADI_CRYPTO_RESULT result = ADI_CRYPTO_SUCCESS;

#ifdef ADI_DEBUG

    if (ADI_CRYPTO_SUCCESS != (result = ValidateHandle(hDevice))) {
        return result;
    }
    if (bEnable == hDevice->bDeviceEnabled) {
        return ADI_CRYPTO_ERR_INVALID_STATE;
    }
#endif /* ADI_DEBUG */

    if (true == bEnable) {

        /* device enable */

        /* Enable the IRQs */
        NVIC_EnableIRQ(CRYPT_EVT_IRQn);

#if (ADI_CRYPTO_ENABLE_DMA_SUPPORT == 1)
        /* Enable the DMA interrupts */
        NVIC_EnableIRQ(hDevice->pDevInfo->dmaInputIrqNum);
        NVIC_EnableIRQ(hDevice->pDevInfo->dmaOutputIrqNum);
#endif

        /* Mark the device as enabled */
        hDevice->bDeviceEnabled = true;

        /* Start processing buffer */
        StartCompute(hDevice);

    } else {

        /* device disable */

        /* Disable the IRQs */
        NVIC_DisableIRQ(CRYPT_EVT_IRQn);

#if (ADI_CRYPTO_ENABLE_DMA_SUPPORT == 1)
        /* Enable the DMA interrupts */
        NVIC_DisableIRQ(hDevice->pDevInfo->dmaInputIrqNum);
        NVIC_DisableIRQ(hDevice->pDevInfo->dmaOutputIrqNum);
#endif

        /* Stop the device */
        StopCompute(hDevice);

        /* if we had a DMA error, return that instead of success */
        if (ADI_CRYPTO_SUCCESS != hDevice->dmaErrorCode) {
			result = hDevice->dmaErrorCode;
		}
    }

    /* Return success */
    return result;
}


#if (ADI_CRYPTO_ENABLE_DMA_SUPPORT == 1)
/**
 * @brief   Dynamically Enable/Disable DMA mode for the device.
 *
 * @param [in]  hDevice   Handle to the device instance.
 * @param [in]  bEnable   'true' will enable DMA and 'false' disables the DMA.
 *
 * @return   Status
 *           - #ADI_CRYPTO_SUCCESS                  Successfully enabled/disabled the DMA.
 *           - #ADI_CRYPTO_ERR_BAD_DEV_HANDLE   [D] Error: Handle Passed is invalid.
 *           - #ADI_CRYPTO_ERR_INVALID_STATE    [D] Error: DMA cannot be enabled or disabled when the device is already enabled.
 *
 * Manage use of DMA mode dynamically.  Presupposes DMA support has been enabled statically
 * in the static configuration files via the ADI_CRYPTO_ENABLE_DMA_SUPPORT macro.
 *
 * @note In addition to requiring that DMA support is enabled (see ADI_CRYPTO_ENABLE_DMA_SUPPORT static
 * configuration macro) for #adi_crypto_EnableDmaMode() to be available, use of DMA mode may
 * also be statically configured (see ADI_CRYPTO_ENABLE_DMA).  Both these macros may be set statically
 * to both enable DMA support and to activate the DMA mode in a fully static manner, without need of
 * calling adi_crypto_EnableDmaMode() at all (in which case, this function may be eliminated by the linker).
 */
ADI_CRYPTO_RESULT adi_crypto_EnableDmaMode (ADI_CRYPTO_HANDLE const hDevice, bool const bEnable)
{
#ifdef ADI_DEBUG
    ADI_CRYPTO_RESULT result;

    if ((result = ValidateHandle(hDevice)) != ADI_CRYPTO_SUCCESS) {
        return result;
    }
    if (hDevice->bDeviceEnabled) {
        return ADI_CRYPTO_ERR_INVALID_STATE;
    }
#endif /* ADI_DEBUG */

    if (bEnable)
    {
        /* Enable DMA and map data pump handler */
        hDevice->bDmaEnabled = true;

        /* Enable the DMA interrupts */
        NVIC_EnableIRQ(hDevice->pDevInfo->dmaInputIrqNum);
        NVIC_EnableIRQ(hDevice->pDevInfo->dmaOutputIrqNum);
    }
    else
    {
        /* Disable DMA and map data pump handler */
        hDevice->bDmaEnabled = false;

        /* Disable the DMA interrupts */
        NVIC_DisableIRQ(hDevice->pDevInfo->dmaInputIrqNum);
        NVIC_DisableIRQ(hDevice->pDevInfo->dmaOutputIrqNum);
    }

    /* Return success */
    return ADI_CRYPTO_SUCCESS;
}
#endif



/*! \cond PRIVATE */

/*========  L O C A L    F U N C T I O N    D E F I N I T I O N S  ========*/


/* Convert from a (4-byte) byte pointer to a u32 */
static uint32_t u32FromU8p(uint8_t * const pFourBytes)
{
    return ( (pFourBytes[3] << 24) | (pFourBytes[2] << 16) | (pFourBytes[1] << 8) | (pFourBytes[0]) );
}


/* load KEY register set by length */
static void loadAesKey(uint8_t * const pKey, ADI_CRYPTO_AES_KEY_LEN const keyLen)
{
	uint32_t volatile *pKeyReg = pREG_CRYPT0_AESKEY0;
	uint8_t *pUserKey = pKey;
	uint32_t numKeyWords;

	/* set AES KEY length register */
	CLR_BITS(*pREG_CRYPT0_CFG, BITM_CRYPT_CFG_AESKEYLEN);
	SET_BITS(*pREG_CRYPT0_CFG, (uint32_t)keyLen);  /* pre-shifted */

	/* Set the number of keywords to write to the 32-bit keyword registers */
	switch (keyLen) {
		case ADI_CRYPTO_AES_KEY_LEN_128_BIT:
			numKeyWords = 4u;
			break;
		case ADI_CRYPTO_AES_KEY_LEN_256_BIT:
			numKeyWords = 8u;
			break;
		default:
			numKeyWords = 0u;  /* hardware only supports only 128-bit and 256-bit key length (no 192-bit) */
			break;
	}

	/* load the key (key registers have write-no-read attribute) */
	for (uint32_t count = 0u; count < numKeyWords; count++) {
		*pKeyReg = u32FromU8p(pUserKey);
		pKeyReg++;
		pUserKey += sizeof(uint32_t);
    }
}


/* Initialize the device structure */
static void InitializeDevData (ADI_CRYPTO_HANDLE const hDevice)
{
    /* Clear the device structure */
    memset(hDevice, 0, sizeof(ADI_CRYPTO_HANDLE));

#if (ADI_CRYPTO_ENABLE_DMA_SUPPORT == 1)
    #if (ADI_CRYPTO_ENABLE_DMA == 1)
        hDevice->bDmaEnabled = true;
        NVIC_EnableIRQ(hDevice->pDevInfo->dmaInputIrqNum);
        NVIC_EnableIRQ(hDevice->pDevInfo->dmaOutputIrqNum);
    #else
        hDevice->bDmaEnabled = false;
        NVIC_DisableIRQ(hDevice->pDevInfo->dmaInputIrqNum);
        NVIC_DisableIRQ(hDevice->pDevInfo->dmaOutputIrqNum);
    #endif
#else
    /* no DMA support */
    hDevice->bDmaEnabled = false;
#endif
}


/* initiate buffer processing (called from crypto enable) */
static void StartCompute(ADI_CRYPTO_HANDLE const hDevice)
{
    /* clear completion flag */
    hDevice->bCompletion = false;

    /* Get pointer to the compute buffer */
    CRYPTO_COMPUTE* pCompute = &hDevice->Computation;

    /* Clear any pending interrupts (all are R/W1C) */
    hDevice->pDev->STAT = hDevice->pDev->STAT;

    /* reset crypto config register */
    hDevice->pDev->CFG = 0u;

#if (ADI_CRYPTO_ENABLE_SHA_SUPPORT == 1)
    /* reset SHA hardware machine state */
    if (ADI_CRYPTO_MODE_SHA == pCompute->eCipherMode) {
        SET_BITS(hDevice->pDev->CFG, BITM_CRYPT_CFG_SHAINIT);
    }
#endif

    /* program main config register settings */
    SET_BITS(hDevice->pDev->CFG,
            ( (uint32_t)pCompute->eCipherMode   /* cipher mode    */
#if defined (__ADUCM4x50__)
            | (uint32_t)pCompute->eKeyByteSwap  /* KEY endianness */
            | (uint32_t)pCompute->eShaByteSwap  /* SHA endianness */
#endif /*ADUCM4x50*/
            | (uint32_t)pCompute->eAesByteSwap  /* AES endianness */
            | (uint32_t)pCompute->eAesKeyLen    /* AES key length */
            | (uint32_t)pCompute->eCodingMode   /* encode mode    */
            )
        );

#if (CRYPTO_SUPPORT_KEY_REQUIRED)

#if (1 == ADI_CRYPTO_ENABLE_PKSTOR_SUPPORT)

	/* if PKSTOR extensions enabled... check is actually in use. */

	/* load AES key indirectly from encrypted key in PKSTOR (no 512-bit keys allowed here) */
	if ( (true == pCompute->bUsePKSTOR) && ((ADI_PK_KUW_LEN_128 == pCompute->pkKuwLen) || (ADI_PK_KUW_LEN_256 == pCompute->pkKuwLen)) )
    {
		/* retrieve and unwrap key from PKSTOR and "use" it (load it into AES register set) */
        adi_crypto_pk_EnablePKSTOR    (hDevice, true);
        adi_crypto_pk_SetKuwLen       (hDevice, pCompute->pkKuwLen);
		adi_crypto_pk_LoadDeviceKey   (hDevice);
		adi_crypto_pk_RetrieveKey     (hDevice, pCompute->pkIndex);
		adi_crypto_pk_UnwrapKuwReg    (hDevice);
		adi_crypto_pk_UseDecryptedKey (hDevice);
        adi_crypto_pk_EnablePKSTOR    (hDevice, false);
    }
    else
#endif /*ADI_CRYPTO_ENABLE_PKSTOR_SUPPORT */
    {
		/* load AES key directly from compute block... */
		if (NULL != pCompute->pKey) {
            loadAesKey(pCompute->pKey, pCompute->eAesKeyLen);
    	}
	}  /* if PKSTOR / else */

#endif  /* (CRYPTO_SUPPORT_KEY_REQUIRED) */

#if (ADI_CRYPTO_ENABLE_CMAC_SUPPORT == 1)
    if (ADI_CRYPTO_MODE_CMAC == pCompute->eCipherMode) {
        /* program CMAC-specific registers */
        /* DATALEN in CMAC mode is number of 128 bit pages (or 16, 8 byte pages) */
        hDevice->pDev->DATALEN             = pCompute->numInputBytesRemaining  / CRYPTO_INPUT_SIZE_IN_BYTES;
    }
#endif  /* (ADI_CRYPTO_ENABLE_CMAC_SUPPORT == 1) */

#if (ADI_CRYPTO_ENABLE_CCM_SUPPORT == 1)
    if (ADI_CRYPTO_MODE_CCM == pCompute->eCipherMode) {
        /* program CMM-specific registers */
        hDevice->pDev->PREFIXLEN           = pCompute->numAuthBytesRemaining / CRYPTO_INPUT_SIZE_IN_BYTES;
        hDevice->pDev->DATALEN             = pCompute->numInputBytesRemaining  / CRYPTO_INPUT_SIZE_IN_BYTES;
        hDevice->pDev->CCM_NUM_VALID_BYTES = pCompute->numValidBytes;
    }
#endif  /* (ADI_CRYPTO_ENABLE_CCM_SUPPORT == 1) */

#if (ADI_CRYPTO_ENABLE_CBC_SUPPORT == 1) || (ADI_CRYPTO_ENABLE_CCM_SUPPORT == 1) || (ADI_CRYPTO_ENABLE_CTR_SUPPORT == 1)

    if ( (ADI_CRYPTO_MODE_CBC == pCompute->eCipherMode) || (ADI_CRYPTO_MODE_CCM == pCompute->eCipherMode) || (ADI_CRYPTO_MODE_CTR == pCompute->eCipherMode) )
    {
        /* program NONCE/IV for CBC, CCM and CTR modes */
        assert (NULL != pCompute->pNonceIV);

        /* Configure Counter Init and NONCE values */
        hDevice->pDev->CNTRINIT = pCompute->CounterInit;

        hDevice->pDev->NONCE0 = u32FromU8p(&pCompute->pNonceIV[0]);
        hDevice->pDev->NONCE1 = u32FromU8p(&pCompute->pNonceIV[4]);
        hDevice->pDev->NONCE2 = u32FromU8p(&pCompute->pNonceIV[8]);

        hDevice->pDev->NONCE3 = ((uint32_t)pCompute->pNonceIV[12] <<  0u) | ((uint32_t)pCompute->pNonceIV[13] <<  8u);

#if (ADI_CRYPTO_ENABLE_CBC_SUPPORT == 1)
        if (ADI_CRYPTO_MODE_CBC == pCompute->eCipherMode) {

            /* additionally, CBC mode requires remaining IV data */
            hDevice->pDev->NONCE3 |= ( ((uint32_t)pCompute->pNonceIV[14] << 16u) | ((uint32_t)pCompute->pNonceIV[15] << 24u) );
        }
#endif    /* (ADI_CRYPTO_ENABLE_CBC_SUPPORT == 1) */
    }
#endif    /* (ADI_CRYPTO_ENABLE_CBC_SUPPORT == 1) || (ADI_CRYPTO_ENABLE_CCM_SUPPORT == 1) || (ADI_CRYPTO_ENABLE_CTR_SUPPORT == 1) */

#if (ADI_CRYPTO_ENABLE_DMA_SUPPORT == 1)

    /* onle enable DMA for non-SHA mode or SHA mode with > 4 bytes of input... */
    if ( ((true == hDevice->bDmaEnabled) && (ADI_CRYPTO_MODE_SHA != pCompute->eCipherMode))
        || ((true == hDevice->bDmaEnabled) && (ADI_CRYPTO_MODE_SHA == pCompute->eCipherMode) && (4u < pCompute->numInputBytesRemaining)) )
    {

        /* DMA startup... */
        programDMA(hDevice);

        /* mode-specific DMA interrupt enables */
        switch (pCompute->eCipherMode) {
#if defined (__ADUCM4x50__)
            case ADI_CRYPTO_MODE_HMAC:
                /* enable HMAC done and overrun interrupts (via PIO handler) */
                SET_BITS(hDevice->pDev->INTEN, (BITM_CRYPT_INTEN_HMACDONEEN | BITM_CRYPT_INTEN_INOVREN));
                break;
#endif /*ADUCM4x50__*/
            case ADI_CRYPTO_MODE_SHA:
                /* enable SHA done and overrun interrupts */
                SET_BITS(hDevice->pDev->INTEN, (BITM_CRYPT_INTEN_SHADONEN | BITM_CRYPT_INTEN_INOVREN));
                SET_BITS(hDevice->pDev->CFG, (BITM_CRYPT_CFG_INDMAEN));
                break;
            default:
                /* enable DMA I/O interrupts */
                SET_BITS(hDevice->pDev->CFG, (BITM_CRYPT_CFG_OUTDMAEN | BITM_CRYPT_CFG_INDMAEN));
                break;
        }

        /* crypto hardware enable */
        SET_BITS(hDevice->pDev->CFG, BITM_CRYPT_CFG_BLKEN);

    } else
#endif
    {
        /* mode-specific PIO interrupt enables */
        switch (pCompute->eCipherMode) {
#if defined (__ADUCM4x50__)
                case ADI_CRYPTO_MODE_HMAC:
                /* HMAC done interrupts via PIO handler (do NOT use INRDY in HMAC mode) */
                SET_BITS(hDevice->pDev->INTEN, (BITM_CRYPT_INTEN_HMACDONEEN | BITM_CRYPT_INTEN_OUTRDYEN | BITM_CRYPT_INTEN_INOVREN));
                break;
#endif /*ADUCM4x50__*/
            case ADI_CRYPTO_MODE_SHA:
                /* SHA done interrupts via PIO handler (do NOT use INRDY in SHA mode) */
                SET_BITS(hDevice->pDev->INTEN, (BITM_CRYPT_INTEN_SHADONEN | BITM_CRYPT_INTEN_INOVREN));
                break;
            default:
                SET_BITS(hDevice->pDev->INTEN, (BITM_CRYPT_INTEN_INOVREN | BITM_CRYPT_INTEN_OUTRDYEN | BITM_CRYPT_INTEN_INRDYEN));
                break;
        }

        /* crypto hardware enable */
        SET_BITS(hDevice->pDev->CFG, BITM_CRYPT_CFG_BLKEN);

        /* manual write of 1st input data batch... (interrupt-driven hereafter...) */
        writePioInputData(hDevice, hDevice->pDev->STAT);
    }
}


/* halt computation */
static void StopCompute (ADI_CRYPTO_HANDLE const hDevice)
{

#if (ADI_CRYPTO_ENABLE_DMA_SUPPORT == 1)
    /* disable Crypto DMA */
    CLR_BITS(hDevice->pDev->CFG, (BITM_CRYPT_CFG_INDMAEN | BITM_CRYPT_CFG_OUTDMAEN));
#endif

    /* clear all interrupt enables */
    hDevice->pDev->INTEN = 0u;

    /* Flush the buffers */
    FlushInputOutputRegisters(hDevice);

    /* device disable */
    CLR_BITS(hDevice->pDev->CFG, BITM_CRYPT_CFG_BLKEN);

    /* Mark the device as disabled */
    hDevice->bDeviceEnabled = false;
}


#if (ADI_CRYPTO_ENABLE_DMA_SUPPORT == 1)
static void programDMA(ADI_CRYPTO_HANDLE const hDevice)
{
    CRYPTO_COMPUTE* pCompute = &hDevice->Computation;
    ADI_DCC_TypeDef* pCCD;  /* pointer to DMA Control Data Descriptor */
    uint32_t channelBit;
    uint32_t num32BitWords;

    /* start with INPUT channel */
    channelBit = 1u << hDevice->pDevInfo->dmaInputChanNum;

    /* disable various stuff */
    pADI_DMA0->SRCADDR_CLR = channelBit;  /* disable src endpointer decrement mode */
    pADI_DMA0->DSTADDR_CLR = channelBit;  /* disable dst endpointer decrement mode */
    pADI_DMA0->EN_SET      = channelBit;  /* channel enable */
    pADI_DMA0->RMSK_CLR    = channelBit;  /* allow Crypto to request DMA service */

#if (ADI_CRYPTO_ENABLE_CBC_SUPPORT == 1) || (ADI_CRYPTO_ENABLE_CCM_SUPPORT == 1) || (ADI_CRYPTO_ENABLE_CTR_SUPPORT == 1)
    /* program input descriptor(s) */
    if (0u != pCompute->pNextAuthInput) {

        /* schedule authentication data into primary descriptor (USING ping-pong mode) */

        pADI_DMA0->ALT_CLR = channelBit;  /* activate PRIMARY descriptor */
        pCCD = pPrimaryCCD + hDevice->pDevInfo->dmaInputChanNum;  /* point to primary INPUT descriptor */

        /* setup the endpoints (point to input register & last 4 bytes of input array) */
        pCCD->DMASRCEND = (uint32_t)pCompute->pNextAuthInput + sizeof(uint32_t) * (pCompute->numAuthBytesRemaining / FIFO_WIDTH_IN_BYTES - 1u);
        pCCD->DMADSTEND = (uint32_t)&hDevice->pDev->INBUF;

        /* program DMA Control Data Config register */
        num32BitWords = pCompute->numAuthBytesRemaining / sizeof(uint32_t);
        pCCD->DMACDC =
            ( ((uint32_t)ADI_DMA_INCR_NONE                << DMA_BITP_CTL_DST_INC)
            | ((uint32_t)ADI_DMA_INCR_4_BYTE              << DMA_BITP_CTL_SRC_INC)
            | ((uint32_t)ADI_DMA_WIDTH_4_BYTE             << DMA_BITP_CTL_SRC_SIZE)
            | ((uint32_t)ADI_DMA_RPOWER_4                 << DMA_BITP_CTL_R_POWER)
            | (uint32_t)((num32BitWords - 1u)             << DMA_BITP_CTL_N_MINUS_1)
            | ((uint32_t)DMA_ENUM_CTL_CYCLE_CTL_PING_PONG << DMA_BITP_CTL_CYCLE_CTL) );


        /* schedule input data into alternate descriptor (in basic mode) */
        pADI_DMA0->PRI_CLR = channelBit;  /* activate ALTERNATE descriptor */
        pCCD = pAlternateCCD + hDevice->pDevInfo->dmaInputChanNum;  /* point to alternate INPUT descriptor */

        /* setup the endpoints (point to input register & last 4 bytes of input array) */
        pCCD->DMASRCEND = (uint32_t)pCompute->pNextInput + sizeof(uint32_t) * (pCompute->numInputBytesRemaining / FIFO_WIDTH_IN_BYTES - 1u);
        pCCD->DMADSTEND = (uint32_t)&hDevice->pDev->INBUF;

        /* program DMA Control Data Config register */
        num32BitWords = pCompute->numInputBytesRemaining / sizeof(uint32_t);
        pCCD->DMACDC =
            ( ((uint32_t)ADI_DMA_INCR_NONE                << DMA_BITP_CTL_DST_INC)
            | ((uint32_t)ADI_DMA_INCR_4_BYTE              << DMA_BITP_CTL_SRC_INC)
            | ((uint32_t)ADI_DMA_WIDTH_4_BYTE             << DMA_BITP_CTL_SRC_SIZE)
            | ((uint32_t)ADI_DMA_RPOWER_4                 << DMA_BITP_CTL_R_POWER)
            | (uint32_t)((num32BitWords - 1u)             << DMA_BITP_CTL_N_MINUS_1)
            | ((uint32_t)DMA_ENUM_CTL_CYCLE_CTL_BASIC     << DMA_BITP_CTL_CYCLE_CTL) );

    } else
#endif  /* #if (ADI_CRYPTO_ENABLE_CBC_SUPPORT == 1) || (ADI_CRYPTO_ENABLE_CCM_SUPPORT == 1) || (ADI_CRYPTO_ENABLE_CTR_SUPPORT == 1) */
    {

        /* no authentication data, just schedule input data into primary descriptor (in basic mode) */

        pADI_DMA0->ALT_CLR = channelBit;  /* activate PRIMARY descriptor */
        pCCD = pPrimaryCCD + hDevice->pDevInfo->dmaInputChanNum;  /* point to primary INPUT descriptor */

        /* setup the endpoints (point to input register & last 4 bytes of input array) */
#if (ADI_CRYPTO_ENABLE_SHA_SUPPORT == 1)
        if (ADI_CRYPTO_MODE_SHA == pCompute->eCipherMode) {

            /* Stop SHA-mode input writes one short of last 32-bit word so the DMA input interrupt
               can manually call PIO write function to handle SHA end flag and last write manually. */
            pCCD->DMASRCEND = (uint32_t)pCompute->pNextInput + sizeof(uint32_t) * (pCompute->numInputBytesRemaining  / FIFO_WIDTH_IN_BYTES - 2u);
            num32BitWords = (pCompute->numInputBytesRemaining - (pCompute->numInputBytesRemaining % sizeof(uint32_t))) / sizeof(uint32_t) - 1u;  /* count - 1 */
        }
        else
#endif
        {
            /* stop at last write end */
            pCCD->DMASRCEND = (uint32_t)pCompute->pNextInput + sizeof(uint32_t) * ( pCompute->numInputBytesRemaining / FIFO_WIDTH_IN_BYTES - 1u);
            num32BitWords = pCompute->numInputBytesRemaining / sizeof(uint32_t); /* count */
        }

        pCCD->DMADSTEND = (uint32_t)&hDevice->pDev->INBUF;

        /* program DMA Control Data Config register */
        pCCD->DMACDC =
            ( ((uint32_t)ADI_DMA_INCR_NONE                << DMA_BITP_CTL_DST_INC)
            | ((uint32_t)ADI_DMA_INCR_4_BYTE              << DMA_BITP_CTL_SRC_INC)
            | ((uint32_t)ADI_DMA_WIDTH_4_BYTE             << DMA_BITP_CTL_SRC_SIZE)
            | ((uint32_t)ADI_DMA_RPOWER_4                 << DMA_BITP_CTL_R_POWER)
            | (uint32_t)((num32BitWords - 1u)             << DMA_BITP_CTL_N_MINUS_1)
            | ((uint32_t)DMA_ENUM_CTL_CYCLE_CTL_BASIC     << DMA_BITP_CTL_CYCLE_CTL) );
    }

/* don't program output DMA in SHA mode... */
#if CRYPTO_SUPPORT_MODE_ANY_NON_SHA

    if (ADI_CRYPTO_MODE_SHA != pCompute->eCipherMode) {

        /* switch to OUTPUT channel */
        channelBit = 1u << hDevice->pDevInfo->dmaOutputChanNum;

        /* disable various stuff */
        pADI_DMA0->SRCADDR_CLR = channelBit;  /* disable src endpointer decrement mode */
        pADI_DMA0->DSTADDR_CLR = channelBit;  /* disable dst endpointer decrement mode */
        pADI_DMA0->EN_SET      = channelBit;  /* channel enable */
        pADI_DMA0->RMSK_CLR    = channelBit;  /* allow Crypto to request DMA service */

        pADI_DMA0->ALT_CLR = channelBit;  /* activate primary descriptor */
        pCCD = pPrimaryCCD + hDevice->pDevInfo->dmaOutputChanNum;  /* point to crypto OUTPUT descriptor */


        /* setup the endpoints (point to output register & last 4 bytes of output array) */
        pCCD->DMASRCEND = (uint32_t)&hDevice->pDev->OUTBUF;
        pCCD->DMADSTEND = (uint32_t)pCompute->pNextOutput + sizeof(uint32_t) * (pCompute->numOutputBytesRemaining / FIFO_WIDTH_IN_BYTES - 1u);

        /* program DMA Control Data Config register */
        num32BitWords = pCompute->numOutputBytesRemaining / sizeof(uint32_t);
        pCCD->DMACDC =
            ( ((uint32_t)ADI_DMA_INCR_4_BYTE           << DMA_BITP_CTL_DST_INC)
            | ((uint32_t)ADI_DMA_INCR_NONE             << DMA_BITP_CTL_SRC_INC)
            | ((uint32_t)ADI_DMA_WIDTH_4_BYTE          << DMA_BITP_CTL_SRC_SIZE)
            | ((uint32_t)ADI_DMA_RPOWER_4              << DMA_BITP_CTL_R_POWER)
            | (uint32_t)((num32BitWords - 1u)          << DMA_BITP_CTL_N_MINUS_1)
            | ((uint32_t)DMA_ENUM_CTL_CYCLE_CTL_BASIC  << DMA_BITP_CTL_CYCLE_CTL) );

    }  /* end non-SHA mode */

#endif  /* CRYPTO_SUPPORT_MODE_ANY_NON_SHA */
}
#endif  /* #if (ADI_CRYPTO_ENABLE_DMA_SUPPORT == 1) */


static void writePioInputData(ADI_CRYPTO_HANDLE const hDevice, uint32_t const status)
{
    CRYPTO_COMPUTE* pCompute = &hDevice->Computation;
    uint32_t numWritable = FIFO_DEPTH - ((status & BITM_CRYPT_STAT_INWORDS) >> BITP_CRYPT_STAT_INWORDS);

#if (ADI_CRYPTO_ENABLE_CBC_SUPPORT == 1) || (ADI_CRYPTO_ENABLE_CCM_SUPPORT == 1) || (ADI_CRYPTO_ENABLE_CTR_SUPPORT == 1)
    /* always send authentication data before input payload is sent */
    if (0u != pCompute->numAuthBytesRemaining) {

        /* fill input FIFO with 32-bit authentication data */
        while ((0u != numWritable) && (0u != pCompute->numAuthBytesRemaining)) {
            hDevice->pDev->INBUF = *pCompute->pNextAuthInput;
            pCompute->pNextAuthInput++;
            pCompute->numAuthBytesRemaining -= FIFO_WIDTH_IN_BYTES;
            numWritable--;
        }
    } else
#endif  /* #if (ADI_CRYPTO_ENABLE_CBC_SUPPORT == 1) || (ADI_CRYPTO_ENABLE_CCM_SUPPORT == 1) || (ADI_CRYPTO_ENABLE_CTR_SUPPORT == 1) */
    {
        /* no authentication data, process payload input data */

#if (ADI_CRYPTO_ENABLE_SHA_SUPPORT == 1)
        if (ADI_CRYPTO_MODE_SHA == pCompute->eCipherMode) {

            /* Drive up to a full "chunk" of SHA input message data.
               Chunk size is limited to 512-bits (64-bytes) by AES
               hardware compute block.
            */

            if (pCompute->numInputBytesRemaining >= SHA_CHUNK_MAX_BYTES)
            {
                /* This is the simple case, load up an entire chunk and let it go */
                for (uint8_t i = 0u; i < SHA_CHUNK_MAX_WORDS; i++) {
                    hDevice->pDev->INBUF = *pCompute->pNextInput;
                    pCompute->pNextInput++;
                }

                pCompute->numShaBitsRemaining    -= SHA_CHUNK_MAX_BITS;
                pCompute->numInputBytesRemaining -= SHA_CHUNK_MAX_BYTES;
            }
            else
            {
                /* The final case, we load up any bytes less than a full chunk and trigger the last word */
                while (FIFO_WIDTH_IN_BITS <= pCompute->numShaBitsRemaining) {
                    hDevice->pDev->INBUF = *pCompute->pNextInput;
                    pCompute->pNextInput++;
                    pCompute->numShaBitsRemaining -= FIFO_WIDTH_IN_BITS;
                }

                hDevice->pDev->SHA_LAST_WORD = (pCompute->numShaBitsRemaining << BITP_CRYPT_SHA_LAST_WORD_O_BITS_VALID) | BITM_CRYPT_SHA_LAST_WORD_O_LAST_WORD;

                /* Last write is dummy or not, depending on remaining bit count */
                if (0u == pCompute->numShaBitsRemaining) {
                    /* dummy write */
                    hDevice->pDev->INBUF = 0u;
                } else {
                    /* partial data (last remaining message data word) */
                    hDevice->pDev->INBUF = *pCompute->pNextInput;
                    pCompute->pNextInput++;
                }

                pCompute->numShaBitsRemaining     = 0u;
                pCompute->numInputBytesRemaining  = 0u;

                /* Use output bytes as a way of confirming that we are really done (can't use input bytes/bits) */
                pCompute->numOutputBytesRemaining -= SHA_OUTPUT_SIZE_IN_BYTES;
            }
        }  /* end of SHA mode */
        else
#endif
        {
            /* full input FIFO with normal payload write (non-SHA) */
            while ((0u != numWritable) && (0u != pCompute->numInputBytesRemaining)) {
                hDevice->pDev->INBUF = *pCompute->pNextInput;
                pCompute->pNextInput++;
                pCompute->numInputBytesRemaining -= FIFO_WIDTH_IN_BYTES;
                numWritable--;
            }
        }
    }
}


static void readPioOutputData(ADI_CRYPTO_HANDLE const hDevice, uint32_t const status)
{
    CRYPTO_COMPUTE *pCompute = &hDevice->Computation;
    uint32_t numReadable;

#if  ADI_CRYPTO_ENABLE_SHA_SUPPORT == 1
    /* Copy the SHA output if enabled */
    if (pCompute->eCipherMode == ADI_CRYPTO_MODE_SHA)
    {
        if (IS_ANY_BIT_SET(status, BITM_CRYPT_STAT_SHADONE)) {

			/* Get 1 SHADONE per block + 1 SHADONE when we trigger the last word */
			if (0u == pCompute->numOutputBytesRemaining) {
#if ADI_CRYPTO_SHA_OUTPUT_FORMAT == 0 /* Little Endian */
				pCompute->pNextOutput[0] = hDevice->pDev->SHAH7;
				pCompute->pNextOutput[1] = hDevice->pDev->SHAH6;
				pCompute->pNextOutput[2] = hDevice->pDev->SHAH5;
				pCompute->pNextOutput[3] = hDevice->pDev->SHAH4;
				pCompute->pNextOutput[4] = hDevice->pDev->SHAH3;
				pCompute->pNextOutput[5] = hDevice->pDev->SHAH2;
				pCompute->pNextOutput[6] = hDevice->pDev->SHAH1;
				pCompute->pNextOutput[7] = hDevice->pDev->SHAH0;
#else
				pCompute->pNextOutput[0] = __ADI_BYTE_SWAP(hDevice->pDev->SHAH0);
				pCompute->pNextOutput[1] = __ADI_BYTE_SWAP(hDevice->pDev->SHAH1);
				pCompute->pNextOutput[2] = __ADI_BYTE_SWAP(hDevice->pDev->SHAH2);
				pCompute->pNextOutput[3] = __ADI_BYTE_SWAP(hDevice->pDev->SHAH3);
				pCompute->pNextOutput[4] = __ADI_BYTE_SWAP(hDevice->pDev->SHAH4);
				pCompute->pNextOutput[5] = __ADI_BYTE_SWAP(hDevice->pDev->SHAH5);
				pCompute->pNextOutput[6] = __ADI_BYTE_SWAP(hDevice->pDev->SHAH6);
				pCompute->pNextOutput[7] = __ADI_BYTE_SWAP(hDevice->pDev->SHAH7);
#endif
			}
        }
    }
    else
#endif
    {
        /* read any ready non-SHA output from output FIFO */
        if (IS_ANY_BIT_SET(status, BITM_CRYPT_STAT_OUTRDY)) {
            numReadable = ((status & BITM_CRYPT_STAT_OUTWORDS) >> BITP_CRYPT_STAT_OUTWORDS);
            while ((0u != numReadable) && (0u != pCompute->numOutputBytesRemaining)) {
                *pCompute->pNextOutput = hDevice->pDev->OUTBUF;
                pCompute->pNextOutput++;
                pCompute->numOutputBytesRemaining -= FIFO_WIDTH_IN_BYTES;
                numReadable--;
            }
        }
    }

    /* if output count has gone to zero, set completion flag */
    if (0u == pCompute->numOutputBytesRemaining) {
        hDevice->bCompletion = true;
    }
}


/* Flush the Crypto input and output buffers */
static void FlushInputOutputRegisters(ADI_CRYPTO_HANDLE const hDevice)
{
    /* Set and clear the flush bits to flush the input and output buffers */
    SET_BITS(hDevice->pDev->CFG, BITM_CRYPT_CFG_INFLUSH | BITM_CRYPT_CFG_OUTFLUSH);
    CLR_BITS(hDevice->pDev->CFG, BITM_CRYPT_CFG_INFLUSH | BITM_CRYPT_CFG_OUTFLUSH);
}


/*================ INTERRUPT HANDELING ==================*/

/* native PIO-mode (non-DMA) interrupt handler */
void Crypto_Int_Handler(void)
{
    ISR_PROLOG();

    ADI_CRYPTO_HANDLE hDevice = CryptoDevInfo[0].hDevice;
    CRYPTO_COMPUTE *pCompute  = &hDevice->Computation;
    uint32_t status = hDevice->pDev->STAT;
    uint32_t event;

    /* clear status */
    hDevice->pDev->STAT = status;

    /* check for overflow */
    if (IS_ANY_BIT_SET(status, BITM_CRYPT_STAT_INOVR)) {

        /* call user's callback */
        if (0u != hDevice->pfCallback) {
            hDevice->pfCallback(hDevice->pCBParam, ADI_CRYPTO_EVENT_STATUS_INPUT_OVERFLOW, (void *)status);
        }

        /* stop */
        StopCompute(hDevice);

        /* post the semaphore */
        SEM_POST(hDevice);

        return;
    }

    /* pull outputs (updates completion flag) */
    readPioOutputData(hDevice, status);

    if (false == hDevice->bCompletion) {

        /* push more inputs, but not in SHA DMA mode (except for when it’s perfectly aligned block) */
        if ((pCompute->eCipherMode != ADI_CRYPTO_MODE_SHA) || (hDevice->bDmaEnabled == false) || (pCompute->numInputBytesRemaining == 0u))
        {
            writePioInputData(hDevice, status);
        }

    } else {

        /* we're done */

        /* dispatch to user callback if we have one */
        if (0u != hDevice->pfCallback) {

            /* check for overflow first */
            if (0u != (BITM_CRYPT_STAT_INOVR & status)) {
                event = ADI_CRYPTO_EVENT_STATUS_INPUT_OVERFLOW;
            } else {
                /* completion message depends on mode */
                switch (hDevice->Computation.eCipherMode) {
#if (ADI_CRYPTO_ENABLE_CBC_SUPPORT == 1)
                    case ADI_CRYPTO_MODE_CBC:  event = ADI_CRYPTO_EVENT_STATUS_CBC_DONE;  break;
#endif
#if (ADI_CRYPTO_ENABLE_CCM_SUPPORT == 1)
                    case ADI_CRYPTO_MODE_CCM:  event = ADI_CRYPTO_EVENT_STATUS_CCM_DONE;  break;
#endif
#if (ADI_CRYPTO_ENABLE_CMAC_SUPPORT == 1)
                    case ADI_CRYPTO_MODE_CMAC: event = ADI_CRYPTO_EVENT_STATUS_CMAC_DONE; break;
#endif
#if (ADI_CRYPTO_ENABLE_CTR_SUPPORT == 1)
                    case ADI_CRYPTO_MODE_CTR:  event = ADI_CRYPTO_EVENT_STATUS_CTR_DONE;  break;
#endif
#if (ADI_CRYPTO_ENABLE_ECB_SUPPORT == 1)
					case ADI_CRYPTO_MODE_ECB:  event = ADI_CRYPTO_EVENT_STATUS_ECB_DONE;  break;
#endif
#if defined (__ADUCM4x50__) /* HMAC support is provided only in ADuCM4x50*/
#if (ADI_CRYPTO_ENABLE_HMAC_SUPPORT == 1)
                    case ADI_CRYPTO_MODE_HMAC: event = ADI_CRYPTO_EVENT_STATUS_HMAC_DONE; break;
#endif
#endif /*__ADUCM4x50__*/
#if (ADI_CRYPTO_ENABLE_SHA_SUPPORT == 1)
                    case ADI_CRYPTO_MODE_SHA:  event = ADI_CRYPTO_EVENT_STATUS_SHA_DONE;  break;
#endif
                    default:                   event = ADI_CRYPTO_EVENT_STATUS_UNKNOWN;   break;
                }
            }

            /* call user's callback and give back buffer pointer */
            hDevice->pfCallback(hDevice->pCBParam, event, (void*)hDevice->pUserBuffer);

            /* clear private copy of user buffer pointer */
            /* (this is done in GetBuffer in non-Callback mode) */
            hDevice->pUserBuffer = NULL;
        }

        /* disable interrupts */
        hDevice->pDev->INTEN = 0u;

        /* post the semaphore */
        SEM_POST(hDevice);
    }

    ISR_EPILOG();
}


#if (ADI_CRYPTO_ENABLE_DMA_SUPPORT == 1)
/* native DMA input interrupt handler */
void DMA_AES0_IN_Int_Handler (void)
{
    ISR_PROLOG();

    ADI_CRYPTO_HANDLE hDevice = CryptoDevInfo[0].hDevice;
    CRYPTO_COMPUTE *pCompute  = &hDevice->Computation;

#if (ADI_CRYPTO_ENABLE_SHA_SUPPORT == 1)
    if (ADI_CRYPTO_MODE_SHA == pCompute->eCipherMode) {

        /* Update the compute structure to reflect the "post DMA" state of the transaction */
        uint32_t numTotalBytes            = pCompute->numInputBytesRemaining;
        uint32_t num32BitWords            = (numTotalBytes - (numTotalBytes % sizeof(uint32_t))) / sizeof(uint32_t) - 1u;
        pCompute->numInputBytesRemaining -= num32BitWords*4u;
        pCompute->numShaBitsRemaining    -= num32BitWords*32u;
        pCompute->pNextInput             += num32BitWords;

        if ((numTotalBytes % SHA_CHUNK_MAX_BYTES) == 0u)
        {
            /* For perfect block sizes, need to write the last word WITHOUT triggering SHA_LAST_WORD */
            hDevice->pDev->INBUF = *pCompute->pNextInput;

            pCompute->numInputBytesRemaining = 0u;
            pCompute->numShaBitsRemaining    = 0u;
        }
        else
        {
            /* Go ahead and write the remaining word, and it’s okay to trigger SHA_LAST_WORD */
            writePioInputData(hDevice, hDevice->pDev->STAT);
        }
    }
#endif

    /* defer post to output interrupt... */

    ISR_EPILOG();
}
#endif /* ADI_CRYPTO_ENABLE_DMA_SUPPORT */


#if (ADI_CRYPTO_ENABLE_DMA_SUPPORT == 1)
/* native DMA output interrupt handler */
void DMA_AES0_OUT_Int_Handler (void)
{
    ISR_PROLOG();
    ADI_CRYPTO_HANDLE hDevice = CryptoDevInfo[0].hDevice;
    uint32_t status = hDevice->pDev->STAT;
    uint32_t event;

    /* by the time we get here, everything should be complete */

    /* dispatch to user callback if we have one */
    if (0u != hDevice->pfCallback) {

        /* check for overflow first */
        if (0u != (BITM_CRYPT_STAT_INOVR & status)) {
            event = ADI_CRYPTO_EVENT_STATUS_INPUT_OVERFLOW;
        } else {
            /* completion message depends on mode */
            switch (hDevice->Computation.eCipherMode) {
#if (ADI_CRYPTO_ENABLE_CBC_SUPPORT == 1)
                case ADI_CRYPTO_MODE_CBC:  event = ADI_CRYPTO_EVENT_STATUS_CBC_DONE;  break;
#endif
#if (ADI_CRYPTO_ENABLE_CCM_SUPPORT == 1)
                case ADI_CRYPTO_MODE_CCM:  event = ADI_CRYPTO_EVENT_STATUS_CCM_DONE;  break;
#endif
#if (ADI_CRYPTO_ENABLE_CMAC_SUPPORT == 1)
                case ADI_CRYPTO_MODE_CMAC: event = ADI_CRYPTO_EVENT_STATUS_CMAC_DONE; break;
#endif
#if (ADI_CRYPTO_ENABLE_CTR_SUPPORT == 1)
                case ADI_CRYPTO_MODE_CTR:  event = ADI_CRYPTO_EVENT_STATUS_CTR_DONE;  break;
#endif
#if (ADI_CRYPTO_ENABLE_ECB_SUPPORT == 1)
                case ADI_CRYPTO_MODE_ECB:  event = ADI_CRYPTO_EVENT_STATUS_ECB_DONE;  break;
#endif
#if defined (__ADUCM4x50__)
#if (ADI_CRYPTO_ENABLE_HMAC_SUPPORT == 1)
                case ADI_CRYPTO_MODE_HMAC: event = ADI_CRYPTO_EVENT_STATUS_HMAC_DONE; break;
#endif
#endif /*__ADUCM4x50__*/
#if (ADI_CRYPTO_ENABLE_SHA_SUPPORT == 1)
                case ADI_CRYPTO_MODE_SHA:  event = ADI_CRYPTO_EVENT_STATUS_SHA_DONE;  break;
#endif
                default:                   event = ADI_CRYPTO_EVENT_STATUS_UNKNOWN;   break;
            }
        }

        /* call user's callback and give back buffer pointer */
        hDevice->pfCallback(hDevice->pCBParam, event, (void*)hDevice->pUserBuffer);

        /* clear private copy of user buffer pointer */
          /* this is done in GetBuffer in non-Callback mode */
        hDevice->pUserBuffer = NULL;
    }

    /* mark completion */
    hDevice->bCompletion = true;

    /* clear status */
    hDevice->pDev->STAT = status;

    /* post the semaphore */
    SEM_POST(hDevice);

    ISR_EPILOG();
}
#endif  /* ADI_CRYPTO_ENABLE_DMA_SUPPORT */

/*! \endcond */

/*@}*/
