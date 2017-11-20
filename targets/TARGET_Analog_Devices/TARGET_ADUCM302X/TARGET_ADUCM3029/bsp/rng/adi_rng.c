/*!
 *****************************************************************************
 * @file:    adi_rng.c
 * @brief:   Random Number Generator Driver
 *----------------------------------------------------------------------------
 *
Copyright (c) 2012-2016 Analog Devices, Inc.

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

/*! \addtogroup RNG_Driver RNG Driver
 *  Random Number Generator Driver
 *  @{
 */

 /*! \cond PRIVATE */

#include <stdlib.h>  /* for 'NULL' definition */
#include <assert.h>

#include <adi_processor.h>
#include <drivers/rng/adi_rng.h>
#include "adi_rng_def.h"
#include <rtos_map/adi_rtos_map.h>

#ifdef __ICCARM__
/*
* IAR MISRA C 2004 error suppressions.
*
* Pm011 (rule 6.3): Types which specify sign and size should be used
*   We use bool which is accepted by MISRA but the toolchain does not accept it
*
* Pm073 (rule 14.7): a function should have a single point of exit
* Pm143 (rule 14.7): a function should have a single point of exit at the end of the function
*   Multiple returns are used for error handling.
*
* Pm050 (rule 14.2): a null statement shall only occur on a line by itself
*   Needed for null expansion of ISR_PROLOG in no-OS case and others.
* Pm140 (rule 11.3): a cast should not be performed between a pointer type and an integral type
*   The rule makes an exception for memory-mapped register accesses.
*/
#pragma diag_suppress=Pm011,Pm073,Pm143,Pm050
#endif /* __ICCARM__ */

#if   defined(__ADUCM4x50__) ||   defined(__ADUCM302x__)
#define NUM_RNG_DEVICES       (1u)
#else
#error "Unsupported processor"
#endif

/*==============  D A T A  ===============*/

/**
 * Information for managing all the RNG devices available
 */
#ifdef __ICCARM__
#pragma diag_suppress=Pm140
#endif

static ADI_RNG_DEV_TYPE gRNG_Device[NUM_RNG_DEVICES] =
{
  {(ADI_RNG_TypeDef*)pADI_RNG0,NULL} /* RNG0 */
};
#ifdef __ICCARM__
#pragma diag_default=Pm140
#endif

/* Forward prototypes */
void RNG_Int_Handler(void);

/** Check the validity of a handle for debug mode */
#ifdef ADI_DEBUG
#define ADI_RNG_INVALID_HANDLE(h) (&gRNG_Device[0] != (h))
#endif

/*! \endcond */

/*!
    @brief Opena a Random Number Generator Device

    @param[in]  nDeviceNum   Device number to be opened.
    @param[in]  pMemory      Pointer to the memory to be used by the driver.
                             Size of the memory should be at least #ADI_RNG_MEMORY_SIZE bytes.
    @param[in]  MemorySize   Size of the memory passed in pMemory parameter.
    @param[out] phDevice     Pointer to a location in the calling function memory space to which
                             the device handle will be written upon successful driver initialization.

    @return    Status
               - #ADI_RNG_SUCCESS                      RNG device driver opened successfully.
               - #ADI_RNG_INVALID_PARAM [D]            The memory passed to the API is either NULL or its size is not sufficient.
               - #ADI_RNG_ALREADY_INITIALIZED [D]  The RNG is already initialized.
               - #ADI_RNG_BAD_DEVICE_NUM [D]       The device number is invalid.

    Initialize and allocate a RNG device for other use.  The core NVIC RNG interrupt is enabled.  This API
    must preceed all other RNG API calls and the handle returned must be passed to all other RNG API calls.

    @note   The contents of \a ppDevice will be set to NULL upon failure.\n\n

    @note   The RNG device driver will clear all pending interrupts and disable all RNG
            interrupts during RNG device initialization.

    @sa    adi_rng_Close().
*/
ADI_RNG_RESULT adi_rng_Open(
                            uint32_t             const  nDeviceNum,
                            void*                const  pMemory,
                            uint32_t             const  MemorySize,
                            ADI_RNG_HANDLE*      const  phDevice
                            )
{
    ADI_RNG_DEV_TYPE *pDevice;

    /* store a bad handle in case of failure */
    *phDevice = (ADI_RNG_HANDLE) NULL;

#ifdef ADI_DEBUG
    if (nDeviceNum >= NUM_RNG_DEVICES)
    {
        return ADI_RNG_BAD_DEVICE_NUM;
    }

    if ((NULL == pMemory) || ( MemorySize < (uint32_t) ADI_RNG_MEMORY_SIZE))
    {
        return ADI_RNG_INVALID_PARAM;
    }
    assert (ADI_RNG_MEMORY_SIZE == sizeof(ADI_RNG_DEV_DATA_TYPE));
#endif

    /* local pointer to instance data */
    pDevice = &gRNG_Device[nDeviceNum];

#ifdef ADI_DEBUG
    if (NULL != pDevice->pData)
    {
        return ADI_RNG_ALREADY_INITIALIZED;
    }
#endif

    /* Set the internal device data */
    pDevice->pData = pMemory;

    /* initialize internal device data */
    pDevice->pData->IRQn    = RNG0_EVT_IRQn;
    pDevice->pData->CBFunc  = NULL;

    /* clear any pending interrupts. Both bits are write 1 to clear */
    pDevice->pRNG->STAT = BITM_RNG_STAT_RNRDY | BITM_RNG_STAT_STUCK;

    /* Set the RNG register based on static configuration */
    pDevice->pRNG->CTL = (uint16_t)RNG0_CFG_ONLY_8_BIT << BITP_RNG_CTL_SINGLE;
    pDevice->pRNG->LEN = (RNG0_CFG_LENGTH_RELOAD << BITP_RNG_LEN_RELOAD)
                       | (RNG0_CFG_LENGTH_PRESCALER << BITP_RNG_LEN_PRESCALE);

   /* The interrupt handler only gets used in the case of callback mode so its
    * enabling only happens in the adi_rng_RegisterCallBack API.
    */
    NVIC_ClearPendingIRQ(pDevice->pData->IRQn);

    /* store handle at application handle pointer */
    *phDevice = pDevice;

    return ADI_RNG_SUCCESS;
}


/*!
 * @brief  Uninitializes and deallocates the RNG device.
 *
 * @param[in]   hDevice    Device handle obtained from adi_rng_Open().
 *
 * @return      Status
 *                - #ADI_RNG_SUCCESS                    Call completed successfully.
 *                - #ADI_RNG_BAD_DEV_HANDLE [D]     Invalid device handle parameter.
 *                - #ADI_RNG_NOT_INITIALIZED [D]    Device has not been initialized for use, see #adi_rng_Open().
 *
 * Uninitialize and release an allocated RNG device for other use.  The core NVIC RNG interrupt is disabled.
 *
 * @sa        adi_rng_Open().
 */
ADI_RNG_RESULT adi_rng_Close(ADI_RNG_HANDLE hDevice)
{
    ADI_RNG_DEV_TYPE *pDevice = (ADI_RNG_DEV_TYPE*)hDevice;

#ifdef ADI_DEBUG
    if (ADI_RNG_INVALID_HANDLE(pDevice)){
        return ADI_RNG_BAD_DEV_HANDLE;
    }

    if (NULL == pDevice->pData) {
        return ADI_RNG_NOT_INITIALIZED;
    }
#endif

    /* uninitialize */
    NVIC_DisableIRQ(pDevice->pData->IRQn);
    pDevice->pData = NULL;

    return ADI_RNG_SUCCESS;
}

/*!
 * @brief  Enables/Disables the RNG device.
 *
 * @param[in]   hDevice       Device handle obtained from adi_rng_Open().
 * @param[in]   bFlag         Flag to specify whether to enable or disable RNG device.
 *
 * @return      Status
 *                - #ADI_RNG_SUCCESS                    Call completed successfully.
 *                - #ADI_RNG_BAD_DEV_HANDLE [D]     Invalid device handle parameter.
 *                - #ADI_RNG_NOT_INITIALIZED [D]    Device has not been initialized for use, see #adi_rng_Open().
 *
 * @sa        adi_rng_Open().
 * @sa        adi_rng_RegisterCallback().
 */
ADI_RNG_RESULT adi_rng_Enable (ADI_RNG_HANDLE const hDevice, bool const bFlag)
{
    ADI_RNG_DEV_TYPE *pDevice = (ADI_RNG_DEV_TYPE*)hDevice;
    ADI_INT_STATUS_ALLOC();

#ifdef ADI_DEBUG
    if (ADI_RNG_INVALID_HANDLE(pDevice)) {
        return ADI_RNG_BAD_DEV_HANDLE;
    }

    if (NULL == pDevice->pData) {
        return ADI_RNG_NOT_INITIALIZED;
    }
#endif

    ADI_ENTER_CRITICAL_REGION();
    if (true == bFlag) {
        pDevice->pRNG->CTL |= BITM_RNG_CTL_EN;
    } else {
        pDevice->pRNG->CTL &= (uint16_t)~(BITM_RNG_CTL_EN);
    }
    ADI_EXIT_CRITICAL_REGION();

    return ADI_RNG_SUCCESS;
}

/*!
 * @brief  Enables/Disables Buffering for RNG.
 *
 * @param[in]   hDevice       Device handle obtained from adi_rng_Open().
 * @param[in]   bFlag         Flag to specify whether to enable or disable buffering for RNG device.
 *                            When buffering is enabled, adi_rng_GetRngData returns 32-bit values.
 *                            When buffering is disabled the API returns 8-bit values.
 *
 * @return      Status
 *                - #ADI_RNG_SUCCESS                    Call completed successfully.
 *                - #ADI_RNG_BAD_DEV_HANDLE [D]     Invalid device handle parameter.
 *                - #ADI_RNG_NOT_INITIALIZED [D]    Device has not been initialized for use, see #adi_rng_Open().
 *
 * @sa        adi_rng_Open().
 * @sa        adi_rng_RegisterCallback().
 * @sa        adi_rng_GetRngData().
 */
ADI_RNG_RESULT adi_rng_EnableBuffering (ADI_RNG_HANDLE const hDevice, bool const bFlag)
{
    ADI_RNG_DEV_TYPE *pDevice = (ADI_RNG_DEV_TYPE*)hDevice;
    ADI_INT_STATUS_ALLOC();

#ifdef ADI_DEBUG
    if (ADI_RNG_INVALID_HANDLE(pDevice)) {
        return ADI_RNG_BAD_DEV_HANDLE;
    }

    if (NULL == pDevice->pData) {
        return ADI_RNG_NOT_INITIALIZED;
    }
#endif

    ADI_ENTER_CRITICAL_REGION();
    if (true == bFlag) {
        pDevice->pRNG->CTL &= (uint16_t)~(BITM_RNG_CTL_SINGLE);
    } else {
        pDevice->pRNG->CTL |= BITM_RNG_CTL_SINGLE;
    }
    ADI_EXIT_CRITICAL_REGION();

    return ADI_RNG_SUCCESS;
}

/*!
 * @brief  Sets the reload and prescale value for the sample counter.
 *         The Sample Length will be nLenReload*2^nLenPrescaler.
 *
 * @param[in]   hDevice        Device handle obtained from adi_rng_Open().
 * @param[in]   nLenPrescaler  Prescaler value for the sample counter (0-10).
 * @param[in]   nLenReload     Reload value for the sample counter (0-4095)
 *
 * @return      Status
 *                - #ADI_RNG_SUCCESS                    Call completed successfully.
 *                - #ADI_RNG_BAD_DEV_HANDLE [D]     Invalid device handle parameter.
 *                - #ADI_RNG_NOT_INITIALIZED [D]    Device has not been initialized for use, see #adi_rng_Open().
 *
 * @sa        adi_rng_Open().
 * @sa        adi_rng_RegisterCallback().
 */
ADI_RNG_RESULT adi_rng_SetSampleLen (
                                     ADI_RNG_HANDLE const hDevice,
                                     uint16_t       const nLenPrescaler,
                                     uint16_t       const nLenReload
                                     )
{
    ADI_RNG_DEV_TYPE *pDevice = (ADI_RNG_DEV_TYPE*)hDevice;
    ADI_INT_STATUS_ALLOC();

#ifdef ADI_DEBUG
    if (ADI_RNG_INVALID_HANDLE(pDevice)){
        return ADI_RNG_BAD_DEV_HANDLE;
    }

    if (NULL == pDevice->pData) {
        return ADI_RNG_NOT_INITIALIZED;
    }

    if (   (nLenPrescaler > 10u)
        || ((0u == nLenPrescaler) && (0u == nLenReload))
        || (nLenReload > 4095u)) {
        return ADI_RNG_INVALID_PARAM;
    }
#endif

    ADI_ENTER_CRITICAL_REGION();
    /* Set the sample reload and prescaler value */
    pDevice->pRNG->LEN =   (uint16_t)((uint16_t)(nLenReload    << BITP_RNG_LEN_RELOAD)   & BITM_RNG_LEN_RELOAD)
                         | (uint16_t)((uint16_t)(nLenPrescaler << BITP_RNG_LEN_PRESCALE) & BITM_RNG_LEN_PRESCALE);
    ADI_EXIT_CRITICAL_REGION();

    return ADI_RNG_SUCCESS;
}


/*!
 * @brief  Retrieves the current state of RNG data/CRC accumulator register.
 *
 * @param[in]   hDevice       Device handle obtained from adi_rng_Open().
 * @param[out]  pbFlag        Pointer to an application-defined boolean variable into which to write the result:
 *                              - true  = RNG data is ready to be read.
 *                              - false = RNG data is not ready.
 *
 * @return      Status
 *                - #ADI_RNG_SUCCESS                    Call completed successfully.
 *                - #ADI_RNG_BAD_DEV_HANDLE [D]     Invalid device handle parameter.
 *                - #ADI_RNG_NOT_INITIALIZED [D]    Device has not been initialized for use, see #adi_rng_Open().
                  - #ADI_RNG_INVALID_PARAM [D]          Argument is incorrect.
 *
 * Retrieve the current state of RNG data/CRC accumulator register. The register holds the final entropy value
 * accumulated by RNG and it should to read only when the data is ready.
 *
 * @sa        adi_rng_Open().
 * @sa        adi_rng_GetRngData().
 * @sa        adi_rng_RegisterCallback().
 */
ADI_RNG_RESULT adi_rng_GetRdyStatus (ADI_RNG_HANDLE const hDevice, bool* const pbFlag)
{
    ADI_RNG_DEV_TYPE *pDevice = (ADI_RNG_DEV_TYPE*)hDevice;

#ifdef ADI_DEBUG
    if (ADI_RNG_INVALID_HANDLE(pDevice)){
        return ADI_RNG_BAD_DEV_HANDLE;
    }

    if (NULL == pDevice->pData) {
        return ADI_RNG_NOT_INITIALIZED;
    }

    if (NULL == pbFlag) {
        return ADI_RNG_INVALID_PARAM;
    }
#endif

    /* Get the RNG Ready status bit */
    if ((pDevice->pRNG->STAT & BITM_RNG_STAT_RNRDY) != 0u)
    {
        *pbFlag = true;
    }
    else
    {
        *pbFlag = false;
    }

    return ADI_RNG_SUCCESS;
}

/*!
 * @brief  Retrieve whether the RNG oscillator output is stuck at a constant value
 *
 * @param[in]   hDevice       Device handle obtained from adi_rng_Open().
 * @param[out]  pbFlag        Pointer to an application-defined boolean variable into which to write the result:
 *                              - true  = RNG oscillator is stuck at a constant value.
 *                              - false = RNG oscillator is not stuck at a constant value.
 *
 * @return      Status
 *                - #ADI_RNG_SUCCESS                    Call completed successfully.
 *                - #ADI_RNG_BAD_DEV_HANDLE [D]     Invalid device handle parameter.
 *                - #ADI_RNG_NOT_INITIALIZED [D]    Device has not been initialized for use, see #adi_rng_Open().
                  - #ADI_RNG_INVALID_PARAM [D]          Argument is incorrect.
 *
 * @sa        adi_rng_Open().
 * @sa        adi_rng_GetRngData().
 * @sa        adi_rng_RegisterCallback().
 */
ADI_RNG_RESULT adi_rng_GetStuckStatus (
                                       ADI_RNG_HANDLE const hDevice,
                                       bool* const pbFlag
                                       )
{
    ADI_RNG_DEV_TYPE *pDevice = (ADI_RNG_DEV_TYPE*)hDevice;

#ifdef ADI_DEBUG
    if (ADI_RNG_INVALID_HANDLE(pDevice)){
        return ADI_RNG_BAD_DEV_HANDLE;
    }

    if (pDevice->pData == NULL) {
        return ADI_RNG_NOT_INITIALIZED;
    }

    if (NULL == pbFlag) {
        return ADI_RNG_INVALID_PARAM;
    }
#endif

    /* Get the stuck status bit */
    if ((pDevice->pRNG->STAT & BITM_RNG_STAT_STUCK) != 0u)
    {
        *pbFlag = true;
    }
    else
    {
        *pbFlag = false;
    }

    return ADI_RNG_SUCCESS;
}


/*!
 * @brief  Retrieve the current value of the RNG data register.
 *
 * @param[in]   hDevice       Device handle obtained from adi_rng_Open().
 * @param[in]   pRegData      Pointer to an application-defined variable into which to write the result.
 *                            Only lower 8-bit is valid if buffering is not enabled
 *
 * @return      Status
 *                - #ADI_RNG_SUCCESS                    Call completed successfully.
 *                - #ADI_RNG_BAD_DEV_HANDLE [D]     Invalid device handle parameter.
 *                - #ADI_RNG_NOT_INITIALIZED [D]    Device has not been initialized for use, see #adi_rng_Open().
 *                - #ADI_RNG_INVALID_PARAM [D]      pRegData is a NULL pointer.
 *                - #ADI_RNG_INVALID_STATE[D]       Random number ready status is not set
 *
 * Retrieve the current value of RNG data register. If the buffering is enabled all 32-bit of value written to
 * pRegData is valid else only the lower 8-bit is valid.
 *
 * @sa        adi_rng_Open().
 * @sa        adi_rng_GetRdyStatus().
 * @sa        adi_rng_RegisterCallback().
 */
ADI_RNG_RESULT adi_rng_GetRngData (ADI_RNG_HANDLE const hDevice, uint32_t* const pRegData)
{
    ADI_RNG_DEV_TYPE *pDevice = (ADI_RNG_DEV_TYPE*)hDevice;

#ifdef ADI_DEBUG
    if (ADI_RNG_INVALID_HANDLE(pDevice)){
        return ADI_RNG_BAD_DEV_HANDLE;
    }

    if (NULL == pDevice->pData) {
        return ADI_RNG_NOT_INITIALIZED;
    }

    if (NULL == pRegData) {
        return ADI_RNG_INVALID_PARAM;
    }

    if ((pDevice->pRNG->STAT & BITM_RNG_STAT_RNRDY) == 0u) {
        return ADI_RNG_INVALID_STATE;
    }
#endif

    /* Get the RNG CRC accumulator value */
    *pRegData = pDevice->pRNG->DATA;

    return ADI_RNG_SUCCESS;
}


/*!
 * @brief  Retrieve the current RNG Oscillator count.
 *
 * @param[in]   hDevice       Device handle obtained from adi_rng_Open().
 * @param[in]   pOscCount     Pointer to an application-defined variable into which to write the result.
 *
 * @return      Status
 *                - #ADI_RNG_SUCCESS                    Call completed successfully.
 *                - #ADI_RNG_BAD_DEV_HANDLE [D]     Invalid device handle parameter.
 *                - #ADI_RNG_NOT_INITIALIZED [D]    Device has not been initialized for use, see #adi_rng_Open().
 *                - #ADI_RNG_INVALID_STATE[D]       Random number ready status is not set
                  - #ADI_RNG_INVALID_PARAM [D]          Argument is incorrect.
 *
 * @sa        adi_rng_Open().
 * @sa        adi_rng_RegisterCallback().
 */
ADI_RNG_RESULT adi_rng_GetOscCount (ADI_RNG_HANDLE const hDevice, uint32_t* const pOscCount)
{
    ADI_RNG_DEV_TYPE *pDevice = (ADI_RNG_DEV_TYPE*)hDevice;

#ifdef ADI_DEBUG
   if (ADI_RNG_INVALID_HANDLE(pDevice)){
        return ADI_RNG_BAD_DEV_HANDLE;
    }

    if (NULL == pDevice->pData) {
        return ADI_RNG_NOT_INITIALIZED;
    }

    if (NULL == pOscCount) {
        return (ADI_RNG_INVALID_PARAM);
    }

    if ((pDevice->pRNG->STAT & BITM_RNG_STAT_RNRDY) == 0u) {
        return ADI_RNG_INVALID_STATE;
    }
#endif

    /* Get the oscillator count high count */
    *pOscCount = pDevice->pRNG->OSCCNT;

    return ADI_RNG_SUCCESS;
}

/*!
 * @brief  Retrieve the current RNG Oscillator difference value for the given index.
 *
 * @param[in]   hDevice     Device handle obtained from adi_rng_Open().
 * @param[in]   nIndex      Index of the difference register.
 * @param[out]  pOscDiff    Pointer to an application-defined variable into which to
 *                          write the oscillator difference value for the given index.
 *
 * @return      Status
 *              - #ADI_RNG_SUCCESS                    Call completed successfully.
 *              - #ADI_RNG_BAD_DEV_HANDLE [D]     Invalid device handle parameter.
 *              - #ADI_RNG_NOT_INITIALIZED [D]    Device has not been initialized for use, see #adi_rng_Open().
 *              - #ADI_RNG_INVALID_STATE[D]       Random number ready status is not set
                - #ADI_RNG_INVALID_PARAM [D]          Argument is incorrect.
 *
 * @sa        adi_rng_Open().
 * @sa        adi_Rng_RegisterCallback().
 */
ADI_RNG_RESULT adi_rng_GetOscDiff (
                                   ADI_RNG_HANDLE const hDevice,
                                   uint32_t       const nIndex,
                                   uint8_t*       const pOscDiff
                                   )
{
    ADI_RNG_DEV_TYPE *pDevice = (ADI_RNG_DEV_TYPE*)hDevice;

#ifdef ADI_DEBUG
   if (ADI_RNG_INVALID_HANDLE(pDevice)){
        return ADI_RNG_BAD_DEV_HANDLE;
    }

    if (NULL == pDevice->pData) {
        return ADI_RNG_NOT_INITIALIZED;
    }

    if ((NULL == pOscDiff) || (nIndex > 3u)) {
        return( ADI_RNG_INVALID_PARAM );
    }

    if ((pDevice->pRNG->STAT & BITM_RNG_STAT_RNRDY) == 0u) {
        return ADI_RNG_INVALID_STATE;
    }
#endif

    /* Get the Osc Difference Register */
    *pOscDiff = (uint8_t)pDevice->pRNG->OSCDIFF[nIndex];

    return ADI_RNG_SUCCESS;
}

/*!
 * @brief  Retrieve the current RNG sample length prescale and reload value configured in the device.
 *
 * @param[in]   hDevice        Device handle obtained from adi_rng_Open().
 * @param[out]  pLenPrescaler  Pointer to an application-defined variable into which the prescaler value is written.
 * @param[out]  pLenReload     Pointer to an application-defined variable into which the reload value for the sample counter is written.
 *
 * @return      Status
 *                - #ADI_RNG_SUCCESS                    Call completed successfully.
 *                - #ADI_RNG_BAD_DEV_HANDLE [D]     Invalid device handle parameter.
 *                - #ADI_RNG_NOT_INITIALIZED [D]    Device has not been initialized for use, see #adi_rng_Open().
                  - #ADI_RNG_INVALID_PARAM [D]          Argument is incorrect.
 *
 *
 * @sa        adi_rng_Open().
 * @sa        adi_rng_RegisterCallback().
 */
ADI_RNG_RESULT adi_rng_GetSampleLen (
                                     ADI_RNG_HANDLE const hDevice,
                                     uint16_t*      const pLenPrescaler,
                                     uint16_t*      const pLenReload
                                     )
{
    ADI_RNG_DEV_TYPE *pDevice = (ADI_RNG_DEV_TYPE*)hDevice;

#ifdef ADI_DEBUG
   if (ADI_RNG_INVALID_HANDLE(pDevice)){
        return ADI_RNG_BAD_DEV_HANDLE;
    }

    if (NULL == pDevice->pData) {
        return ADI_RNG_NOT_INITIALIZED;
    }

    if ((NULL == pLenPrescaler) || (NULL == pLenReload)) {
        return ADI_RNG_INVALID_PARAM;
    }
#endif

    *pLenPrescaler = (pDevice->pRNG->LEN & BITM_RNG_LEN_PRESCALE) >> BITP_RNG_LEN_PRESCALE;
    *pLenReload    = (pDevice->pRNG->LEN & BITM_RNG_LEN_RELOAD)   >> BITP_RNG_LEN_RELOAD;

    return ADI_RNG_SUCCESS;
}


/*************************************************************************************************
**************************************************************************************************
*****************************************   CALLBACKS   ******************************************
*****************************************      AND      ******************************************
*****************************************   INTERRUPT   ******************************************
**************************************************************************************************
*************************************************************************************************/


/*!
    @brief RNG Application callback registration API.

    @param[in]   hDevice     Device handle obtained from #adi_rng_Open().
    @param[in]   cbFunc      Application callback address; the function to call on the interrupt.
    @param[in]   pCBParam    Application handle to be passed in the call back.

    @return    Status
               - #ADI_RNG_SUCCESS                    The callback is successfully registered.
               - #ADI_RNG_BAD_DEV_HANDLE [D]     Invalid device handle parameter.
               - #ADI_RNG_NOT_INITIALIZED [D]    Device has not been initialized for use, see #adi_rng_Open().

    Registers an application-defined callback \a cbFunc function address of type ADI_CALLBACK with the RNG device driver.
    Callbacks are made in response to received RNG interrupts.

    The callback to the application is made in context of the originating interrupt (i.e., the RNG driver's
    RNG interrupt handler that is registered in the system's interrupt vector table).  Extended processing
    during the callback (an extension of the RNG's interrupt handler) is discouraged so as to avoid lower-priority
    interrupt blocking.  Also, any register read-modify-write operations should be protected using the
    ADI_ENTER_CRITICAL_REGION()/ADI_EXIT_CRITICAL_REGION() pair to prevent higher-priority interrupts from modifying
    said register during the read-modify-write operation.

    @note   CALLBACKS: RNG interrupt callbacks are \b disabled by default during RNG device driver
            initialization (#adi_rng_Open()).  The application uses the #adi_rng_RegisterCallback()
            API to request an application-defined callback from the RNG device driver. The RNG device
            driver clears the interrupt when the callback exits.
            The application callback should <b>avoid extended processing</b>
            during callbacks as the callback is executing context of the initiating interrupt and will
            block lower-priority interrupts.  If extended application-level interrupt processing is
            required, the application should schedule it for the main application loop and exit the
            callback as soon as possible.\n


    @sa    adi_rng_Open().
*/
ADI_RNG_RESULT adi_rng_RegisterCallback (
                                         ADI_RNG_HANDLE  hDevice,
                                         ADI_CALLBACK    cbFunc,
                                         void           *pCBParam)
{
    ADI_RNG_DEV_TYPE *pDevice = (ADI_RNG_DEV_TYPE*)hDevice;

#ifdef ADI_DEBUG
   if (ADI_RNG_INVALID_HANDLE(pDevice)){
        return ADI_RNG_BAD_DEV_HANDLE;
    }

    if (NULL == pDevice->pData) {
        return ADI_RNG_NOT_INITIALIZED;
    }
#endif

    /* save the callback info */
    pDevice->pData->CBFunc   = cbFunc;
    pDevice->pData->pCBParam = pCBParam;

    if (NULL != cbFunc) {
        /* enable RNG interrupts in NVIC */
        NVIC_EnableIRQ(pDevice->pData->IRQn);
    } else {
        NVIC_DisableIRQ(pDevice->pData->IRQn);
    }

    return ADI_RNG_SUCCESS;
}

/*! \cond PRIVATE */
/* RNG driver interrupt handler. Overrides weak default handler in startup file */
void RNG_Int_Handler(void)
{
    ISR_PROLOG();
    ADI_RNG_DEV_TYPE *pDevice = &gRNG_Device[0];
    register uint16_t candidate;

    /* if we have an initialized driver... */
    if (NULL != pDevice->pData)
    {
        /* if we have a registered callback */
        if (NULL != pDevice->pData->CBFunc)
        {
            ADI_INT_STATUS_ALLOC();

            ADI_ENTER_CRITICAL_REGION();
            /* read status register without other interrupts in between */
            candidate = pDevice->pRNG->STAT;
            ADI_EXIT_CRITICAL_REGION();

            /* Only have bits in stat that are necessary */
            candidate = candidate & (BITM_RNG_STAT_STUCK | BITM_RNG_STAT_RNRDY);

            while (0u != candidate) {
                uint32_t nEvent;

                if (0u != (candidate & BITM_RNG_STAT_RNRDY)) {
                    nEvent = ADI_RNG_EVENT_READY;
                    candidate &= (uint16_t)~BITM_RNG_STAT_RNRDY;
                } else if (0u != (candidate & BITM_RNG_STAT_STUCK)) {
                    nEvent = ADI_RNG_EVENT_STUCK;
                    candidate &= (uint16_t)~BITM_RNG_STAT_STUCK;
                } else {
                    break;
                }

                pDevice->pData->CBFunc (
                                        pDevice->pData->pCBParam,
                                        nEvent,
                                        NULL
                                        );
            }

            pDevice->pRNG->STAT = BITM_RNG_STAT_RNRDY | BITM_RNG_STAT_STUCK;
        }
    }
    ISR_EPILOG();
}
/*! \endcond */

/*
** EOF
*/

/*@}*/
