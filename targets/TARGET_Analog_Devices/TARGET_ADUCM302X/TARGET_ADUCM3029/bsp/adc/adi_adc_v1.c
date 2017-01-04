/*! *****************************************************************************
 * @file:    adi_adc_v1.c
 * @brief:   ADC device driver global file.
 * @details: This file contain the ADC device driver impelementation.
 * @version: $Revision:
 * @date:    $Date:
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
/** @addtogroup  ADC_Driver ADC Device Driver
 *  @{
 */

#ifndef _ADI_ADC_V1_C_
/*! \cond PRIVATE */
#define _ADI_ADC_V1_C_

/*=============  I N C L U D E S   =============*/


/* Header file with definitions specific to ADC driver implementation */

/*=============  A D C    I M P L E M E N T A T I O N    S O U R C E    F I L E S  =============*/
#include <drivers/adc/adi_adc.h>
#include <services/dma/adi_dma.h>
#include <assert.h>
#include <string.h>
#include <ssdd_common/common_def.h>
#include <adi_adc_config.h>


#include "adi_adc_def_v1.h"

#ifdef __ICCARM__
/*
* IAR MISRA C 2004 error suppressions.
*
* Pm123 (rule 8.5): there shall be no definition of objects or functions in a header file
*   This isn't a header as such.
*
* Pm073 (rule 14.7): a function should have a single point of exit
* Pm143 (rule 14.7): a function should have a single point of exit at the end of the function
*   Multiple returns are used for error handling.
*
* Pm050 (rule 14.2): a null statement shall only occur on a line by itself
*   Needed for null expansion of ADI_INSTALL_HANDLER and others.
*
* Pm088 (rule 17.4): pointer arithmetic should not be used.
*   Relying on pointer arithmetic for buffer handling.
*
* Pm140 (rule 11.3): a cast should not be performed between a pointer type and an integral type.
*   Required for MMR register values
*
*/
#pragma diag_suppress=Pm123,Pm073,Pm143,Pm050,Pm088,Pm140
#endif /* __ICCARM__ */

#include "adi_adc_def_v1.h"
#include "adi_adc_data_v1.c"

/*==============  D E F I N E S  ===============*/

/*=============  C O D E  =============*/

/*=============  D E B U G    F U N C T I O N    P R O T O T Y P E S  =============*/

/* IF (Debug information enabled) */
#ifdef ADI_DEBUG

/* Validates ADC device handle */
static ADI_ADC_RESULT ValidateAdcHandle (ADI_ADC_HANDLE const hDevice);

#endif /* ADI_DEBUG */


/* Prototypes for static functions (required by MISRA-C:2004 Rule 8.1)  */
/*=============  L O C A L   F U N C T I O N S   P R O T O T Y P E S =============*/
static uint16_t ReadOutReg(uint32_t nChannelNum);

static void DMAHandler (void *pcbParam, uint32_t Event, void *pArg);

static ADI_ADC_DEVICE *AdcDevicePointer(ADI_ADC_HANDLE hDevice);
static ADI_ADC_INFO *AdcDeviceInfo(ADI_ADC_HANDLE hDevice);
static bool_t InitBufferProcessing(ADI_ADC_DEVICE *pDevice);
#if ADI_ADC_CFG_BLOCKING_MODE != 1
static ADI_ADC_RESULT PollingFIFOManage (ADI_ADC_DEVICE *pDevice, ADC_FIFO_MODE eFifoMode);
#endif /* ADI_ADC_CFG_BLOCKING_MODE */
static ADI_ADC_RESULT InterruptFIFOManage (ADI_ADC_DEVICE *pDevice, ADC_FIFO_MODE eFifoMode);
#if ADI_ADC_ENABLE_DMA_SUPPORT == 1
static ADI_ADC_RESULT DmaFIFOManage (ADI_ADC_DEVICE *pDevice, ADC_FIFO_MODE eFifoMode);
#endif /* ADI_ADC_ENABLE_DMA_SUPPORT == 1 */
static uint32_t GetNumChannels(uint32_t nChannels);
static void ManageFifoCompletion(ADI_ADC_DEVICE *pDevice);
static int32_t nGetChannelNumber(ADI_ADC_CHANNEL eChannel);
static void ConfigureComparator(ADI_ADC_DEVICE *pDevice);
static void StaticConfiguration(ADI_ADC_DEVICE *pDevice);
static void ClearAllBufferAndFifo(ADI_ADC_DEVICE *pDevice);
static void FlushFifo(ADI_ADC_DEVICE *pDevice, uint32_t nChannels);
ADI_INT_HANDLER(ADC_Int_Handler);

/*=============  B U F F E R    F U N C T I O N S    P R O T O T Y P E S =============*/
static ADC_INT_BUFFER* fpopIntBuffer(ADC_INT_BUFFER** ppBufferList);
static void fpushIntBuffer(ADC_INT_BUFFER** ppBufferList, ADC_INT_BUFFER* pBuffer);
static void bpushIntBuffer(ADC_INT_BUFFER** ppBufferList, ADC_INT_BUFFER* pBuffer) ;
static void bpushIntBuffer_Crit(ADC_INT_BUFFER** ppBufferList, ADC_INT_BUFFER* pBuffer);

#ifdef ADI_DEBUG
#define ASSERT(X)  assert(X)
#else
#define ASSERT(X) 
#endif

/*! \endcond */

/*=============  P U B L I C   F U N C T I O N S =============*/

/**
 * @brief    Opens an ADC device instance.
 *
 * @param [in]  nDeviceNum   Device number to open
 * @param [in]  pMemory      Pointer to a #ADI_ADC_MEMORY_SIZE sized buffer to manage the device
 *                           instance.
 * @param [in]  nMemorySize  Size of the buffer to which "pMemory" points
 * @param [out] phDevice     Pointer to a location where ADC device handle is to be written.
 *
 * @return   Status
 *           - #ADI_ADC_SUCCESS                     Call completed successfully
 *           - #ADI_ADC_INVALID_DEVICE_NUM      [D] Invalid Device Number
 *           - #ADI_ADC_INSUFFICIENT_MEMORY     [D] Memory passed is not sufficient
 *           - #ADI_ADC_IN_USE                  [D] ADC driver was already opened
 */
ADI_ADC_RESULT adi_adc_Open (
    uint32_t       nDeviceNum,
    void           *pMemory,
    uint32_t       nMemorySize,
    ADI_ADC_HANDLE *phDevice)
{
    ADI_ADC_DEVICE *pDevice = pMemory;
    uint32_t i;

#ifdef ADI_DEBUG
    if (nDeviceNum > (sizeof (AdcDevInfo)/sizeof(AdcDevInfo[0])))
    {
        return ADI_ADC_INVALID_DEVICE_NUM;
    }

    if (nMemorySize < ADI_ADC_MEMORY_SIZE)
    {
        return ADI_ADC_INSUFFICIENT_MEMORY;
    }

    if (AdcDevInfo[nDeviceNum].hDevice != NULL)
    {
        return ADI_ADC_IN_USE;
    }
    
    assert (ADI_ADC_MEMORY_SIZE >= sizeof (ADI_ADC_DEVICE));
#endif /* ADI_DEBUG */

    memset (pMemory, 0, nMemorySize);

    
    ADI_ENTER_CRITICAL_REGION();
    AdcDevInfo[nDeviceNum].hDevice = (ADI_ADC_HANDLE)pDevice;
    pDevice->pReg = AdcDevInfo[nDeviceNum].pReg;
    ADI_EXIT_CRITICAL_REGION();
    

    /* Setup the buffer lists */
    for (i = 0u; i < (NUM_ADC_INT_BUFFERS - 1u); i++) {
        pDevice->Buffers[i].pNext = &pDevice->Buffers[i + 1u];
    }
    pDevice->pFreeList = &pDevice->Buffers[0];

    /* Reset the ADC */
    pDevice->pReg->CFG = BITM_ADC_CFG_RST;

    /* Install interrupt handles */
    ADI_INSTALL_HANDLER(ADC0_EVT_IRQn, ADC_Int_Handler);
    
    /* Enable the IRQs */
    ADI_ENABLE_INT(ADC0_EVT_IRQn);
    
    /* Initialize the registers to known value */
    pDevice->pReg->IRQ_EN =   BITM_ADC_IRQ_EN_RDY | BITM_ADC_IRQ_EN_ALERT | BITM_ADC_IRQ_EN_OVF | BITM_ADC_IRQ_EN_CALDONE | BITM_ADC_IRQ_EN_CNVDONE;
    
    /* Do the static configuration */
    StaticConfiguration(pDevice);
    

#if ADI_CFG_ENABLE_RTOS_SUPPORT == 1
    /* Create a semaphore for buffer management */
    CREATE_SEM(pDevice, ADI_ADC_ERR_RTOS);
#endif

    /* Set the default FIFO Manage function */
#if ADI_ADC_CFG_NON_BLOCKING_MODE == 1
    pDevice->pfManageFifo = DmaFIFOManage;
#else    
    pDevice->pfManageFifo = InterruptFIFOManage;
#endif /* ADI_ADC_CFG_NON_BLOCKING_MODE == 1 */

    /* Set the polling mode FIFO manage function */
#if ADI_ADC_CFG_BLOCKING_MODE == 1
    pDevice->pfBlockingManageFifo = pDevice->pfManageFifo;
#else
    pDevice->pfBlockingManageFifo = PollingFIFOManage;
#endif /* ADI_ADC_CFG_BLOCKING_MODE == 1 */

    /* Return the device handle back to the application */
    *phDevice = AdcDevInfo[nDeviceNum].hDevice;

    return ADI_ADC_SUCCESS;
}
    
    
/**
 * @brief    Close the given device instance
 *
 * @param [in]  hDevice  Handle to the device instance
 *
 * @return   Status
 *           - #ADI_ADC_SUCCESS                     Successfully closed the device
 *           - #ADI_ADC_INVALID_DEVICE_HANDLE   [D] Invalid device handle based to the function
 */
ADI_ADC_RESULT adi_adc_Close (ADI_ADC_HANDLE hDevice)
{
    ADI_ADC_INFO *pAdcInfo = AdcDeviceInfo(hDevice);

#if ADI_ADC_ENABLE_DMA_SUPPORT == 1
    ADI_ADC_DEVICE *pDevice = AdcDevicePointer (hDevice);
#endif /* ADI_ADC_ENABLE_DMA_SUPPORT */

    ADI_ADC_RESULT eResult;

#ifdef ADI_DEBUG
    if (pAdcInfo == NULL)
    {
        return ADI_ADC_INVALID_DEVICE_HANDLE;
    }
#endif /* ADI_DEBUG */	

    /* Power down the device */
    if ((eResult = adi_adc_PowerUp (hDevice, false)) != ADI_ADC_SUCCESS) {
        return eResult;
    }

#if ADI_ADC_ENABLE_DMA_SUPPORT == 1    
    /* Close the DMA if configured */
    if (pDevice->hDmaHandle != NULL) {
        assert (adi_dma_Close(pDevice->hDmaHandle) == ADI_DMA_SUCCESS);
        pDevice->hDmaHandle = NULL;
    }
#endif /* ADI_ADC_ENABLE_DMA_SUPPORT == 1 */
        
    /* Disable the ADC interrupt */
    ADI_DISABLE_INT(ADC0_EVT_IRQn);
    
#if (ADI_CFG_ENABLE_RTOS_SUPPORT == 1)
    /* Destroy the semaphore */
    DELETE_SEM(hDevice, ADI_ADC_ERR_RTOS);
#endif

    pAdcInfo->hDevice = NULL;

    return ADI_ADC_SUCCESS;
}


/**
 * @brief   Power up ADC
 *
 * @param [in]  hDevice     Handle to the device instance
 *
 * @param [in]  bPowerUp    'true' to power up and 'false' to power down the ADC.
 *
 * @return   Status
 *           - #ADI_ADC_SUCCESS                     Successfully closed the device
 *           - #ADI_ADC_INVALID_DEVICE_HANDLE   [D] Invalid device handle based to the function
 */
ADI_ADC_RESULT adi_adc_PowerUp (ADI_ADC_HANDLE hDevice, bool_t bPowerUp)
{
    ADI_ADC_DEVICE *pDevice = AdcDevicePointer (hDevice);
    uint32_t nPCLKDIVCNT;

#ifdef ADI_DEBUG
    if (pDevice == NULL)
    {
        return ADI_ADC_INVALID_DEVICE_HANDLE;
    }
#endif /* ADI_DEBUG */

    if (bPowerUp)
    {
        if (IS_NOT_IN_ANY_STATE(ADC_STATUS_POWERED_UP))
        {
            /* Read the PCLK divide count from Clock Control Register 1 */
            nPCLKDIVCNT = (*pREG_CLKG0_CLK_CTL1 & BITM_CLKG_CLK_CTL1_PCLKDIVCNT) >> BITP_CLKG_CLK_CTL1_PCLKDIVCNT;

            /* Powering up ADC */
            pDevice->pReg->CFG |= BITM_ADC_CFG_PWRUP;

            /* Set ADC_PWRUP.WAIT bits as 526/PCLKDIVCNT */
            pDevice->pReg->PWRUP = (uint16_t)(((526u / nPCLKDIVCNT) << BITP_ADC_PWRUP_WAIT) & BITM_ADC_PWRUP_WAIT);
            
            SET_STATE(ADC_STATUS_POWERED_UP);
        }
    }
    else
    {
        if (IS_IN_STATE(ADC_STATUS_POWERED_UP))
        {
            /* If the ADC system is up then disable the ADC subsystem */
            if (  IS_IN_STATE(ADC_STATUS_SUB_SYSTEM_EN) )
            {
                ADI_ADC_RESULT eResult;
                eResult = adi_adc_EnableADCSubSystem (hDevice, false);
                if (eResult != ADI_ADC_SUCCESS)
                {
                    return eResult;
                }                
            }
            
            /* Powering down ADC */
            pDevice->pReg->CFG &= (uint16_t)~BITM_ADC_CFG_PWRUP;
            CLR_STATE(ADC_STATUS_POWERED_UP);
        }
    }

    return ADI_ADC_SUCCESS;
}

            
/**
 * @brief   Registering a callback function
 *
 * @param [in]  hDevice     Handle to the device instance
 * @param [in]  pfCallback  Function pointer to callback function. Passing a NULL pointer will
 *                          unregister the call back function.
 * @param [in]  pCBParam    Call back function parameter
 *
 * @details  This function registers a call back function. Registered function will be called when
 *           the given computation is over. This will enable the NON-BLOCKING-CALLBACK mode and any
 *           further call to adi_adc_GetBuffer will fail.
 *
 *
 * @return   Status
 *           - #ADI_ADC_SUCCESS              Successfully registerd the callback
 */            
ADI_ADC_RESULT adi_adc_RegisterCallback (
    ADI_ADC_HANDLE hDevice,
    ADI_CALLBACK   pfCallback,
    void           *pCBParam)
{
    ADI_ADC_DEVICE *pDevice = AdcDevicePointer (hDevice);

#ifdef ADI_DEBUG
    if (pDevice == NULL)
    {
        return ADI_ADC_INVALID_DEVICE_HANDLE;
    }

    if (IS_IN_ANY_STATE(ADC_STATUS_NON_BLOCKING_EN | ADC_STATUS_BLOCKING_EN | ADC_STATUS_COMPARATOR_EN)) 
    {
        return ADI_ADC_INVALID_SEQUENCE;
    }

#endif /* ADI_DEBUG */

    pDevice->pfCallback = pfCallback;
    pDevice->pCBParam = pCBParam;

    return ADI_ADC_SUCCESS;
}

/**
 * @brief    Enable/Disables the ADC Subsystem
 *
 * @param [in]  hDevice      Handle to the device instance
 *
 * @param [in]  bEnable      'true' to Enable and 'false' to Disable`
 *
 * @details  Enables/Disables the ADC Subsystem. The ADC subsystem need to be enabled before using the ADC
 *           for sampling the signal.The driver should check whether the ADC is ready by calling adi_adc_IsReady
 *           API before continuing. If internal reference buffer is used as voltage reference then application
 *           has to wait at least 3.5ms after enabling irrespective of whether adi_adc_IsReady returns ready or not.
 *
 * @return   Status
 *           - #ADI_ADC_SUCCESS              Successfully enabled/disabled the ADC subsystem
 */
ADI_ADC_RESULT adi_adc_EnableADCSubSystem (
    ADI_ADC_HANDLE hDevice,
    bool_t         bEnable)
{
    ADI_ADC_DEVICE *pDevice = AdcDevicePointer (hDevice);

#ifdef ADI_DEBUG
    if (pDevice == NULL)
    {
        return ADI_ADC_INVALID_DEVICE_HANDLE;
    }
    
    if (IS_NOT_IN_STATE(ADC_STATUS_POWERED_UP))
    {
        return ADI_ADC_INVALID_SEQUENCE;
    }
    
    if (bEnable) {
        if (IS_IN_STATE(ADC_STATUS_SUB_SYSTEM_EN)) {
            return ADI_ADC_INVALID_SEQUENCE;
        }
    } else {
        if (IS_IN_ANY_STATE(ADC_STATUS_BLOCKING_EN | ADC_STATUS_NON_BLOCKING_EN | ADC_STATUS_COMPARATOR_EN)) {
            return ADI_ADC_INVALID_SEQUENCE;
        }
    }
#endif /* ADI_DEBUG */

    if (bEnable)
    {
        pDevice->pReg->CFG |= BITM_ADC_CFG_EN;
        SET_STATE(ADC_STATUS_SUB_SYSTEM_EN);
    }
    else
    {
        pDevice->pReg->CFG &= (uint16_t)~BITM_ADC_CFG_EN;
        CLR_STATE(ADC_STATUS_SUB_SYSTEM_EN | ADC_STATUS_SUB_SYSTEM_READY);
    }

    return ADI_ADC_SUCCESS;
}

                
/**
  * @brief    Returns whether the ADC Subsystem is ready
  *
  * @param [in]  hDevice      Handle to the device instance
  *

* @param [in]  pbReady      Pointer to a bool variable. The variable will be set to 'true' if the ADC is ready else 'false'
  *
  * @details  Returns whether the ADC is ready for sampling. This API should be called after enabling the ADC sub-system using
  *           adi_adc_EnableADCSubSystem API. If internal reference buffer is used as voltage reference then application
  *           has to wait at least 3.5ms after enabling irrespective of whether adi_adc_IsReady returns ready or not.
  *
  * @return   Status
  *           - #ADI_ADC_SUCCESS              Successfully returned the ready status
  */
               
ADI_ADC_RESULT adi_adc_IsReady (
    ADI_ADC_HANDLE  hDevice,
    bool_t         *pbReady
)
{
    ADI_ADC_DEVICE *pDevice = AdcDevicePointer (hDevice);

#ifdef ADI_DEBUG
    if (pDevice == NULL)
    {
        return ADI_ADC_INVALID_DEVICE_HANDLE;
    }

    if (pbReady == NULL)
    {
        return ADI_ADC_NULL_POINTER;
    }
    
    if (IS_NOT_IN_STATE(ADC_STATUS_SUB_SYSTEM_EN))
    {
        return ADI_ADC_INVALID_SEQUENCE;
    }
#endif /* ADI_DEBUG */

    if (IS_IN_STATE(ADC_STATUS_SUB_SYSTEM_READY))
    {
        *pbReady = true;
    }
    else
    {
        *pbReady = false;
    }
    return ADI_ADC_SUCCESS;
}

/**
 * @brief    Set the Voltage Reference source
 *
 * @param [in] hDevice      Handle to the device instance
 *
 * @param [in] eVrefSrc     Voltage Reference source to be used
 *
 * @details  The API can be used to select the voltage reference to be used by the ADC. This option need to be
 *           set before enabling the ADC subsystem.
 *
 * @return   Status
 *           - #ADI_ADC_SUCCESS                     Succesfully set the Vref source
 *           - #ADI_ADC_INVALID_PARAMETER           Vref source enum passed is invalid.
 *           - #ADI_ADC_INVALID_DEVICE_HANDLE   [D] Invalid device handle based to the function.
 *           - #ADI_ADC_INVALID_SEQUENCE        [D] VREF cannot be changed once the ADC subsystem is enabled.
 */

ADI_ADC_RESULT adi_adc_SetVrefSource (
    ADI_ADC_HANDLE   hDevice,
    ADI_ADC_VREF_SRC eVrefSrc)
{
    ADI_ADC_DEVICE *pDevice = AdcDevicePointer (hDevice);

#ifdef ADI_DEBUG
    if (pDevice == NULL)
    {
        return ADI_ADC_INVALID_DEVICE_HANDLE;
    }
    
    if (IS_IN_STATE(ADC_STATUS_SUB_SYSTEM_EN)) 
    {
        return ADI_ADC_INVALID_SEQUENCE;
    }
#endif /* ADI_DEBUG */

    pDevice->pReg->CFG &= (uint16_t)~(BITM_ADC_CFG_REFBUFEN | BITM_ADC_CFG_VREFSEL);

    switch (eVrefSrc)
    {
    case ADI_ADC_VREF_SRC_INT_1_25_V:
        pDevice->pReg->CFG |= BITM_ADC_CFG_REFBUFEN | BITM_ADC_CFG_VREFSEL;
        break;

    case ADI_ADC_VREF_SRC_INT_2_50_V:
        pDevice->pReg->CFG |= BITM_ADC_CFG_REFBUFEN;
        break;
        
    case ADI_ADC_VREF_SRC_EXT:
        break;

    default:
        return ADI_ADC_INVALID_PARAMETER;
    }

    return ADI_ADC_SUCCESS;
}


/**
 * @brief    Enable/Disable Current Sink
 *
 * @param [in] hDevice      Handle to the device instance
 *
 * @param [in] bEnable      'true' to Enable and 'false' to Disable current sink
 *
 * @details  If the volatage reference is required to sink current then this option need to be enabled.
 *           The ADC subsystem has the capability to sink upto 50uA at Vref of 1.25V and 100uA at Vref of 2.5V

 * @return   Status
 *           - #ADI_ADC_SUCCESS                     Successfully enabled sink
 *           - #ADI_ADC_INVALID_DEVICE_HANDLE   [D] Invalid device handle passed to the API
 */
                
ADI_ADC_RESULT adi_adc_SinkEnable (
    ADI_ADC_HANDLE hDevice,
    bool_t         bEnable)
{
    ADI_ADC_DEVICE *pDevice = AdcDevicePointer (hDevice);

#ifdef ADI_DEBUG
    if (pDevice == NULL)
    {
        return ADI_ADC_INVALID_DEVICE_HANDLE;
    }
#endif /* ADI_DEBUG */

    if (bEnable)
    {
        pDevice->pReg->CFG |= BITM_ADC_CFG_SINKEN;
    }
    else
    {
        pDevice->pReg->CFG &= (uint16_t)~(BITM_ADC_CFG_SINKEN);
    }

    return ADI_ADC_SUCCESS;
}


/**
 * @brief    Start the ADC calibration
 *
 * @param [in] hDevice      Handle to the device instance
 *
 * @details  The call to this function initiate calibration of the ADC. The user is recommended to do calibration of the ADC after
 *           enabling the ADC subsystem. The status of the calibration can be checked using adi_adc_IsCalibrationDone API.
 *
 * @return   Status
 *           - #ADI_ADC_SUCCESS                     Successfully initiated calibration of ADC
 *           - #ADI_ADC_INVALID_DEVICE_HANDLE   [D] Invalid device handle passed to the API 
 *           - #ADI_ADC_INVALID_SEQUENCE        [D] Sampling cannot be enabled if the ADC is enabled.
 */
ADI_ADC_RESULT  adi_adc_StartCalibration(ADI_ADC_HANDLE hDevice)
{
    ADI_ADC_DEVICE *pDevice = AdcDevicePointer (hDevice);

#ifdef ADI_DEBUG
    if (pDevice == NULL)
    {
        return ADI_ADC_INVALID_DEVICE_HANDLE;
    }

    /* Calibration cannot be done when ADC is processing the buffers */
    if (IS_IN_ANY_STATE(ADC_STATUS_NON_BLOCKING_EN | ADC_STATUS_BLOCKING_EN)) 
    {
        return ADI_ADC_INVALID_SEQUENCE;
    }
        
#endif /* ADI_DEBUG */

    /* Clear the calibration done state */
    CLR_STATE(ADC_STATUS_CALIBRATION_DONE);

    /* Clear ADC_STAT.CALDONE */
    pDevice->pReg->STAT = BITM_ADC_STAT_CALDONE;
    
    /* Set the state as calibration enabled. This state will be cleared when we get the
       calibration done interrupt. */
    SET_STATE (ADC_STATUS_CALIBRATION_EN);
    
    /* Start ADC calibration */
    pDevice->pReg->CFG |= BITM_ADC_CFG_STARTCAL;
    
    return ADI_ADC_SUCCESS;
}

/**
 * @brief    Returns the status of the calibration which was initiated.
 *
 * @param [in]  hDevice              Handle to the device instance
 *
 * @param [out] pbCalibrationDone    Pointer to the location to which the status of calibration is written.
 *                                   'true' if the calibration started by call to  is done else 'false'
 *
 * @return   Status
 *           - #ADI_ADC_SUCCESS                     Successfully retrieved the status of ADC calibration.
 *           - #ADI_ADC_INVALID_DEVICE_HANDLE   [D] Invalid device handle passed to the API 
 *           - #ADI_ADC_NULL_POINTER            [D] pbCalibrationDone is NULL
 */

ADI_ADC_RESULT adi_adc_IsCalibrationDone (
    ADI_ADC_HANDLE hDevice,
    bool_t         *pbCalibrationDone)
{
    ADI_ADC_DEVICE *pDevice = AdcDevicePointer (hDevice);

#ifdef ADI_DEBUG
    if (pDevice == NULL)
    {
        return ADI_ADC_INVALID_DEVICE_HANDLE;
    }

    if (pbCalibrationDone == NULL)
    {
        return ADI_ADC_NULL_POINTER;
    }
#endif /* ADI_DEBUG */

    /* The driver will check whether the driver is set to calibration done state. This state will 
     * be set in the driver when the calibration done interrupt is received by the driver
     */
    if (IS_IN_STATE(ADC_STATUS_CALIBRATION_DONE))
    {
        *pbCalibrationDone = true;
    }
    else
    {
        *pbCalibrationDone = false;
    }

    return ADI_ADC_SUCCESS;
}



/**
 * @brief    Set the acquisition time of ADC in ADC clock cycles
 *
 * @param [in]  hDevice              Handle to the device instance
 *
 * @param [in] nAcqTimeInAClkCycles  Acquisition time in ADC clock cycles
 *
 * @return   Status
 *           - #ADI_ADC_SUCCESS                     Successfully set the acquisition time of ADC
 *           - #ADI_ADC_INVALID_DEVICE_HANDLE   [D] Invalid device handle passed to the API 
 *           - #ADI_ADC_INVALID_SEQUENCE        [D] Acquisition time cannot be set when the ADC is enabled for sampling
 *           - #ADI_ADC_INVALID_PARAMETER       [D] nAcqTimeInAClkCycles is not in the valid range
 */
ADI_ADC_RESULT adi_adc_SetAcquisitionTime (
    ADI_ADC_HANDLE hDevice,
    uint32_t       nAcqTimeInAClkCycles
    )
{
    ADI_ADC_DEVICE *pDevice = AdcDevicePointer (hDevice);
    uint16_t nCnvTime;

#ifdef ADI_DEBUG
    if (pDevice == NULL)
    {
        return ADI_ADC_INVALID_DEVICE_HANDLE;
    }

    if (IS_IN_ANY_STATE(ADC_STATUS_BLOCKING_EN | ADC_STATUS_NON_BLOCKING_EN | ADC_STATUS_COMPARATOR_EN))
    {
        return ADI_ADC_INVALID_SEQUENCE;
    }

    if ((nAcqTimeInAClkCycles == 0u) || (nAcqTimeInAClkCycles > 0x100u))
    {
        return ADI_ADC_INVALID_PARAMETER;
    }

#endif /* ADI_DEBUG */

    /* Acquisition phase is (ADC_CNV_TIME.SAMPTIME + 1) ACLK cycles */
    nCnvTime = pDevice->pReg->CNV_TIME;
    nCnvTime &= (uint16_t)~BITM_ADC_CNV_TIME_SAMPTIME;
    nCnvTime |= (uint16_t)((nAcqTimeInAClkCycles - 1u) << BITP_ADC_CNV_TIME_SAMPTIME);
    pDevice->pReg->CNV_TIME = nCnvTime;

    return ADI_ADC_SUCCESS;
}

/**
 * @brief    Set the delay time of ADC in ADC cycles for multi iteration mode.
 *
 * @param [in]  hDevice              Handle to the device instance
 *
 * @param [in] nDelayInAClkCycles    Delay time in ADC clock cycles.
 *
 * @return   Status
 *           - #ADI_ADC_SUCCESS                     Successfully set delay time
 *           - #ADI_ADC_INVALID_DEVICE_HANDLE   [D] Invalid device handle passed to the API 
 *           - #ADI_ADC_INVALID_PARAMETER       [D] nDelayInAClkCycles is not in the valid range
 */
ADI_ADC_RESULT adi_adc_SetDelayTime (
    ADI_ADC_HANDLE hDevice,
    uint32_t nDelayInAClkCycles)
{
    ADI_ADC_DEVICE *pDevice = AdcDevicePointer (hDevice);
    uint16_t nCnvTime;

#ifdef ADI_DEBUG
    if (pDevice == NULL)
    {
        return ADI_ADC_INVALID_DEVICE_HANDLE;
    }

    if (nDelayInAClkCycles > (BITM_ADC_CNV_TIME_DLY >> BITP_ADC_CNV_TIME_DLY))
    {
        return ADI_ADC_INVALID_PARAMETER;
    }
#endif /* ADI_DEBUG */

    nCnvTime = pDevice->pReg->CNV_TIME;
    nCnvTime &= (uint16_t)~BITM_ADC_CNV_TIME_DLY;
    nCnvTime |= (uint16_t)(nDelayInAClkCycles << BITP_ADC_CNV_TIME_DLY);
    pDevice->pReg->CNV_TIME = nCnvTime;

    return ADI_ADC_SUCCESS;
}

/**
 * @brief    Set the resolution of ADC. he default resolution of ADC is 12-bit and the ADC increases the resolution
 *           by oversampling. Averaging will be disabled when the resolution is more than 12-bits.
 *
 * @param [in]  hDevice       Handle to the device instance
 *
 * @param [in] eResolution    Enum of ADC resolution
 *
 * @return   Status
 *           - #ADI_ADC_SUCCESS                     Successfully set the resolution of the ADC.
 *           - #ADI_ADC_INVALID_DEVICE_HANDLE   [D] Invalid device handle passed to the API
 *           - #ADI_ADC_INVALID_SEQUENCE        [D] Resolution cannot be changed when the ADC is enabled for sampling
 *           - #ADI_ADC_INVALID_STATE           [D] Resolution cannot be changed from 12-bit if averaging is enabled
 *           - #ADI_ADC_INVALID_PARAMETER           eResolution parameter passed is invalid.
 */
ADI_ADC_RESULT adi_adc_SetResolution (
    ADI_ADC_HANDLE hDevice,
    ADI_ADC_RESOLUTION eResolution)
{
    ADI_ADC_DEVICE *pDevice = AdcDevicePointer (hDevice);
    uint16_t nFactor;

#ifdef ADI_DEBUG
    if (pDevice == NULL)
    {
        return ADI_ADC_INVALID_DEVICE_HANDLE;
    }
    if (IS_IN_ANY_STATE(ADC_STATUS_BLOCKING_EN |  ADC_STATUS_NON_BLOCKING_EN | ADC_STATUS_COMPARATOR_EN)) 
    {
        return ADI_ADC_INVALID_SEQUENCE;
    }
    if (IS_IN_ANY_STATE(ADC_STATUS_AVGERAGING_EN) && (eResolution != ADI_ADC_RESOLUTION_12_BIT)) 
    {
        return ADI_ADC_INVALID_STATE;
    }
#endif /* ADI_DEBUG */

    switch (eResolution)
    {
    case ADI_ADC_RESOLUTION_12_BIT:
        pDevice->pReg->AVG_CFG &= (uint16_t)~BITM_ADC_AVG_CFG_OS;
        if (IS_NOT_IN_STATE(ADC_STATUS_AVGERAGING_EN)) {
          pDevice->pReg->AVG_CFG = 0u;
        }
        CLR_STATE(ADC_STATUS_OVERSAMPLING_EN);
        break;

    case ADI_ADC_RESOLUTION_13_BIT:
    case ADI_ADC_RESOLUTION_14_BIT:
    case ADI_ADC_RESOLUTION_15_BIT:
    case ADI_ADC_RESOLUTION_16_BIT:
        /* factor = 0x02 for 13-bit
                    0x08 for 14-bit
                    0x20 for 15-bit
                    0x80 for 16-bit */
        nFactor = (uint16_t)1 << ((int32_t)eResolution * 2 - 1);
        pDevice->pReg->AVG_CFG =  BITM_ADC_AVG_CFG_OS | BITM_ADC_AVG_CFG_EN
                                | (uint16_t)(nFactor << BITP_ADC_AVG_CFG_FACTOR);
        SET_STATE(ADC_STATUS_OVERSAMPLING_EN);
                                
        break;

    default:
        return ADI_ADC_INVALID_PARAMETER;
    }

    return ADI_ADC_SUCCESS;
}

/**
 * @brief    Enable Averaging for all ADC channels.
 *
 * @param [in] hDevice            Handle to the device instance
 *
 * @param [in] nAveragingSamples  Specifies the number of samples used for averaging. The valid value is between 1-256, in the steps of power of 2. 1 is for disabling averaging.
 *                                The averaging require that the resolution of ADC is 12-bit.
 *
 * @return   Status
 *           - #ADI_ADC_SUCCESS                     Successfully enabled averaging.
 *           - #ADI_ADC_INVALID_DEVICE_HANDLE   [D] Invalid device handle passed to the API  
 *           - #ADI_ADC_INVALID_STATE           [D] Averaging cannot be enabled if the resolution is above 12bits
 *           - #ADI_ADC_INVALID_PARAMETER       [D] nAveragingSamples parameter passed is invalid. 
 */
ADI_ADC_RESULT adi_adc_EnableAveraging (
    ADI_ADC_HANDLE hDevice,
    uint16_t        nAveragingSamples
    )
{
    ADI_ADC_DEVICE *pDevice = AdcDevicePointer (hDevice);
    uint16_t nFactor;

#ifdef ADI_DEBUG
    if (pDevice == NULL)
    {
        return ADI_ADC_INVALID_DEVICE_HANDLE;
    }

    if ((nAveragingSamples == 0u) || (nAveragingSamples > 256u)
        /* Or nAveragingSamples is not a power of 2 */
        || ((nAveragingSamples & (nAveragingSamples - 1u)) != 0u))
    {
        return ADI_ADC_INVALID_PARAMETER;
    }
    if (IS_IN_STATE(ADC_STATUS_OVERSAMPLING_EN)) 
    {
        return ADI_ADC_INVALID_STATE;
    }
#endif /* ADI_DEBUG */

    /* Disable averaging */
    if (nAveragingSamples == 1u)
    {
        pDevice->pReg->AVG_CFG &= (uint16_t)~BITM_ADC_AVG_CFG_EN;
        CLR_STATE(ADC_STATUS_AVGERAGING_EN);
    }
    else
    {
        nFactor = nAveragingSamples >> 1;
        pDevice->pReg->AVG_CFG = BITM_ADC_AVG_CFG_EN | (uint16_t)(nFactor << BITP_ADC_AVG_CFG_FACTOR);
        SET_STATE(ADC_STATUS_AVGERAGING_EN);
    }

    return ADI_ADC_SUCCESS;
}

/**
 * @brief    Configure low limit for an ADC channel when it's used as a digital comparator.
 *
 * @param [in] hDevice              Handle to the device instance
 *
 * @param [in] eChannel             The ADC channel for which to configure the comparator
 *
 * @param [in] bEnable              Enable or disable the low limit of the digital comparator
 *
 * @param [in] nLowLimit            The low limit of the digital comparator. If bEnable is false, this paramter is omitted.
 *
 * @return   Status
 *           - #ADI_ADC_SUCCESS                     Successfully configured set the low limit.
 *           - #ADI_ADC_INVALID_DEVICE_HANDLE   [D] Invalid device handle passed to the API 
 *           - #ADI_ADC_INVALID_PARAMETER       [D] Parameters passed is not valid.
 */
ADI_ADC_RESULT adi_adc_SetLowLimit (
    ADI_ADC_HANDLE   hDevice,
    ADI_ADC_CHANNEL  eChannel,
    bool_t           bEnable,
    uint16_t         nLowLimit
    )
{
    ADI_ADC_DEVICE *pDevice = AdcDevicePointer (hDevice);
    static volatile uint16_t* pRegister[4] = {
        pREG_ADC0_LIM0_LO, pREG_ADC0_LIM1_LO, pREG_ADC0_LIM2_LO, pREG_ADC0_LIM3_LO
    };
    int32_t nChannelNum = nGetChannelNumber(eChannel);
    
#ifdef ADI_DEBUG
    if (pDevice == NULL)
    {
        return ADI_ADC_INVALID_DEVICE_HANDLE;
    }
    if ((nLowLimit > (BITM_ADC_LIM0_LO_VALUE >> BITP_ADC_LIM0_LO_VALUE)) || (nChannelNum < 0) || (nChannelNum > 3))
    {
        return ADI_ADC_INVALID_PARAMETER;
    }
#endif /* ADI_DEBUG */
    if (!bEnable) {
        nLowLimit = 0u;
    }
    
    *pRegister[nChannelNum] = (uint16_t)(*pRegister[nChannelNum] & (uint16_t)~BITM_ADC_LIM0_LO_VALUE)
                            | (uint16_t)(nLowLimit << BITP_ADC_LIM0_LO_VALUE);
    if (bEnable) {
        pDevice->nCompConfig |= (uint32_t)1u << (BITS_ADC_COMP_CFG_CH * (uint32_t)nChannelNum + BITS_ADC_COMP_CFG_LOW);
    } else {
        pDevice->nCompConfig &= ~((uint32_t)1u << (BITS_ADC_COMP_CFG_CH * (uint32_t)nChannelNum + BITS_ADC_COMP_CFG_LOW));
    }

    return ADI_ADC_SUCCESS;
}

/**
 * @brief    Configure high limit for an ADC channel when it's used as a digital comparator.
 *
 * @param [in] hDevice              Handle to the device instance
 *
 * @param [in] eChannel             The ADC channel for which to configure the comparator
 *
 * @param [in] bEnable              Enable or disable the high limit of the digital comparator
 *
 * @param [in] nHighLimit           The high limit of the digital comparator. If bEnable is false, this paramter is omitted.
 *
 * @return   Status
 *           - #ADI_ADC_SUCCESS                     Successfully set the high limit
 *           - #ADI_ADC_INVALID_DEVICE_HANDLE   [D] Invalid device handle passed to the API 
 *           - #ADI_ADC_INVALID_PARAMETER       [D] Parameters passed is not valid.
 */
ADI_ADC_RESULT adi_adc_SetHighLimit (
    ADI_ADC_HANDLE   hDevice,
    ADI_ADC_CHANNEL  eChannel,
    bool_t           bEnable,
    uint16_t         nHighLimit)
{
    ADI_ADC_DEVICE *pDevice = AdcDevicePointer (hDevice);
    static volatile uint16_t* pRegister[4] = {
        pREG_ADC0_LIM0_HI, pREG_ADC0_LIM1_HI, pREG_ADC0_LIM2_HI, pREG_ADC0_LIM3_HI
    };
    int32_t nChannelNum = nGetChannelNumber(eChannel);
    
#ifdef ADI_DEBUG
    if (pDevice == NULL)
    {
        return ADI_ADC_INVALID_DEVICE_HANDLE;
    }
    if ((nHighLimit > (BITM_ADC_LIM0_HI_VALUE >> BITP_ADC_LIM0_HI_VALUE)) || (nChannelNum < 0) || (nChannelNum > 3))
    {
        return ADI_ADC_INVALID_PARAMETER;
    }
#endif /* ADI_DEBUG */
    if (!bEnable) {
        nHighLimit = 0u;
    }
    
    if (bEnable) {
        *pRegister[nChannelNum] = (uint16_t)(*pRegister[nChannelNum] & (uint16_t)~BITM_ADC_LIM0_HI_VALUE)
                                 | (uint16_t)(nHighLimit << BITP_ADC_LIM0_HI_VALUE);
    }
    
    if (bEnable) {
        pDevice->nCompConfig |= (uint32_t)1u << (BITS_ADC_COMP_CFG_CH * (uint32_t)nChannelNum + BITS_ADC_COMP_CFG_HIGH);
    } else {
        pDevice->nCompConfig &= ~((uint32_t)1u << (BITS_ADC_COMP_CFG_CH * (uint32_t)nChannelNum + BITS_ADC_COMP_CFG_HIGH));
    }

    return ADI_ADC_SUCCESS;
}

/**
 * @brief    Configure hysteresis for an ADC channel when it's used as a digital comparator.
 *
 * @param [in] hDevice              Handle to the device instance
 *
 * @param [in] eChannel             The ADC channel for which to configure the comparator
 *
 * @param [in] bEnable              Enable or disable the hysteresis of the digital comparator
 *
 * @param [in] nHysteresis          The hysteresis to be used. If bEnable is false, this paramter is omitted.
 *
 * @return   Status
 *           - #ADI_ADC_SUCCESS                     Successfully configured the comparator
 *           - #ADI_ADC_INVALID_DEVICE_HANDLE   [D] Invalid device handle passed to the API 
 *           - #ADI_ADC_INVALID_PARAMETER       [D] Parameters passed is not valid.
 */
ADI_ADC_RESULT adi_adc_SetHysteresis (
    ADI_ADC_HANDLE   hDevice,
    ADI_ADC_CHANNEL  eChannel,
    bool_t           bEnable,
    uint16_t         nHysteresis)
{
    ADI_ADC_DEVICE *pDevice = AdcDevicePointer (hDevice);
    static volatile uint16_t* pRegister[4] = {
        pREG_ADC0_HYS0, pREG_ADC0_HYS1, pREG_ADC0_HYS2, pREG_ADC0_HYS3
    };
    int32_t nChannelNum = nGetChannelNumber(eChannel);
    
#ifdef ADI_DEBUG
    if (pDevice == NULL)
    {
        return ADI_ADC_INVALID_DEVICE_HANDLE;
    }
    if ((nHysteresis > (BITM_ADC_HYS0_VALUE >> BITP_ADC_HYS0_VALUE)) || (nChannelNum < 0) || (nChannelNum > 3))
    {
        return ADI_ADC_INVALID_PARAMETER;
    }
#endif /* ADI_DEBUG */
    if (!bEnable) {
        nHysteresis = 0x1ffu;
    }
    
    if (bEnable) {
        *pRegister[nChannelNum] = (uint16_t)(*pRegister[nChannelNum] & (uint16_t)~BITM_ADC_HYS0_VALUE)
                                 | (uint16_t)(nHysteresis << BITP_ADC_HYS0_VALUE);
    }
    
    if (bEnable) {
        pDevice->nCompConfig |= (uint32_t)1u << (BITS_ADC_COMP_CFG_CH * (uint32_t)nChannelNum + BITS_ADC_COMP_CFG_HYS);
    } else {
        pDevice->nCompConfig &= ~((uint32_t)1u << (BITS_ADC_COMP_CFG_CH * (uint32_t)nChannelNum + BITS_ADC_COMP_CFG_HYS));
    }

    return ADI_ADC_SUCCESS;
}

/**
 * @brief    Configure number of monitor cycles for an ADC channel when it's used as a digital comparator.
 *
 * @param [in] hDevice              Handle to the device instance
 *
 * @param [in] eChannel             The ADC channel for which to configure the comparator
 *
 * @param [in] nNumMonitorCycles    Number of Monitor cycles before giving interrupt
 *
 * @return   Status
 *           - #ADI_ADC_SUCCESS                     Successfully configured the comparator
 *           - #ADI_ADC_INVALID_DEVICE_HANDLE   [D] Invalid device handle passed to the API 
 *           - #ADI_ADC_INVALID_PARAMETER       [D] Parameters passed is not valid.
 */
ADI_ADC_RESULT adi_adc_SetNumMonitorCycles(
    ADI_ADC_HANDLE   hDevice,
    ADI_ADC_CHANNEL  eChannel,
    uint32_t         nNumMonitorCycles)
{
    static volatile uint16_t* pRegister[4] = {
        pREG_ADC0_HYS0, pREG_ADC0_HYS1, pREG_ADC0_HYS2, pREG_ADC0_HYS3
    };
    int32_t nChannelNum = nGetChannelNumber(eChannel);
    
#ifdef ADI_DEBUG
    ADI_ADC_DEVICE *pDevice = AdcDevicePointer (hDevice);

    if (pDevice == NULL)
    {
        return ADI_ADC_INVALID_DEVICE_HANDLE;
    }
    if ((nNumMonitorCycles > (BITM_ADC_HYS0_MONCYC >> BITP_ADC_HYS0_MONCYC)) || (nChannelNum < 0) || (nChannelNum > 3))
    {
        return ADI_ADC_INVALID_PARAMETER;
    }
#endif /* ADI_DEBUG */
    
    *pRegister[nChannelNum] = (uint16_t)(*pRegister[nChannelNum] & (uint16_t)~BITM_ADC_HYS0_MONCYC)
                            | (uint16_t)(nNumMonitorCycles << BITP_ADC_HYS0_MONCYC);

    return ADI_ADC_SUCCESS;
}



/**
 * @brief    Enable/Disable digital comparator for the given channel(s)
 *
 * @param [in] hDevice              Handle to the device instance
 *
 * @param [in] nChannels            Channels for which digital comparator need to be enabled/disabled.
 *                                  Should be an ORed value of ADI_ADC_CHANNEL.
 *
 * @param [in] bEnableComparator    'true' to Enable and 'false' to disable
 *
 * @return   Status
 *           - #ADI_ADC_SUCCESS                     Successfully enabled/disabled digital comparator for the given channels
 *           - #ADI_ADC_INVALID_DEVICE_HANDLE   [D] Invalid device handle passed to the API 
 *           - #ADI_ADC_INVALID_STATE           [D] Digital comparator cannot be enabled if sampling resolution is more than 12-bit or
 *                                                  averaging is enabled. Comparator for a given channel cannot be enbaled if none of the limits
 *                                                  are enabled for the given channel.
 *           - #ADI_ADC_INVALID_SEQUENCE        [D] Comparator cannot be enabled when ADC is enabled for sampling.
 *           - #ADI_ADC_INVALID_OPERATION       [D] Comparator require callback to be registered.
 */
ADI_ADC_RESULT adi_adc_EnableDigitalComparator (
    ADI_ADC_HANDLE hDevice,
    uint32_t       nChannels,
    bool_t         bEnableComparator
)
{
    ADI_ADC_DEVICE *pDevice = AdcDevicePointer (hDevice);
    uint32_t x;
#ifdef ADI_DEBUG
    if (pDevice == NULL)
    {
        return ADI_ADC_INVALID_DEVICE_HANDLE;
    }
    
    if (IS_IN_ANY_STATE(ADC_STATUS_BLOCKING_EN | ADC_STATUS_NON_BLOCKING_EN)) 
    {
        return ADI_ADC_INVALID_SEQUENCE;
    }
    
    if (IS_IN_ANY_STATE(ADC_STATUS_AVGERAGING_EN | ADC_STATUS_OVERSAMPLING_EN)) 
    {
        return ADI_ADC_INVALID_STATE;
    }
    
    if (pDevice->pfCallback == NULL) {
        return ADI_ADC_INVALID_OPERATION;
    }
    
    if (bEnableComparator) {
        for (x = 0u; x < NUM_ADC_COMPARATOR_CHANNELS; x++) {
            if ((nChannels & ((uint32_t)1u << x)) != 0u) {
                if ((  pDevice->nCompConfig 
                     & (
                          ((uint32_t)1u << (BITS_ADC_COMP_CFG_CH * x + BITS_ADC_COMP_CFG_LOW))
                        | ((uint32_t)1u << (BITS_ADC_COMP_CFG_CH * x + BITS_ADC_COMP_CFG_HIGH))
                    )) == 0u) 
                {
                    return ADI_ADC_INVALID_STATE;
                }
            }
        }
    }
#endif /* ADI_DEBUG */

    for (x = 0u; x < NUM_ADC_COMPARATOR_CHANNELS; x++) {
        if ((nChannels & ((uint32_t)1u << x)) != 0u) {
            if (bEnableComparator) {
                pDevice->nCompConfig |= (uint32_t)1u << (BITS_ADC_COMP_CFG_CH * x + BITS_ADC_COMP_CFG_EN);
            } else {
                pDevice->nCompConfig &= ~((uint32_t)1u << (BITS_ADC_COMP_CFG_CH * x + BITS_ADC_COMP_CFG_EN));    
            }
        }
    }
    
    ConfigureComparator(pDevice);
    
    return ADI_ADC_SUCCESS;
}

/**
 * @brief    Submit the ADC buffer for processing to the ADC Module
 *
 * @param [in]  hDevice   Handle to the device instance.
 * @param [in]  pBuffer   Pointer to the #ADI_ADC_BUFFER structure which contains details
 *                        of the buffers required by the driver.
 *
 * @return  Status
 *           - #ADI_ADC_SUCCESS                     Successfully submitted the buffer
 *           - #ADI_ADC_INVALID_DEVICE_HANDLE   [D] Invalid device handle passed to the API 
 *           - #ADI_ADC_NULL_POINTER            [D] pBuffer is NULL
 *           - #ADI_ADC_INVALID_BUFFER          [D] Buffer parameters are invalid.
 *
 * @note    The driver will take ownership of the ADI_ADC_BUFFER structure passed to the driver.
 *          The application has to make sure the structure is not used and it's scope is valid till
 *          the structure is returned back to the application.
 */
ADI_ADC_RESULT adi_adc_SubmitBuffer (
    ADI_ADC_HANDLE hDevice,
    ADI_ADC_BUFFER*     pBuffer
)
{
    ADI_ADC_DEVICE *pDevice = AdcDevicePointer (hDevice);
    uint32_t nNumChannels = 0u;

    ADC_INT_BUFFER* pIntBuffer;

#ifdef ADI_DEBUG
    if (pDevice == NULL)
    {
        return ADI_ADC_INVALID_DEVICE_HANDLE;
    }
    if (pBuffer == NULL) {
        return ADI_ADC_NULL_POINTER;
    }
    if ((pBuffer->nChannels == 0u) || (pBuffer->pDataBuffer == NULL) || (pBuffer->nNumConversionPasses == 0u))
    {
        return ADI_ADC_INVALID_BUFFER;
    }
#endif /* ADI_DEBUG */

    nNumChannels = GetNumChannels(pBuffer->nChannels);

    
    pIntBuffer = fpopIntBuffer(&pDevice->pFreeList);
    pIntBuffer->nConfig = ADC_BUFFER_CONFIG_BUFFER_AUTO_MODE_EN;
    pIntBuffer->nStatus = 0u;
    if (pBuffer->nNumConversionPasses == 1u) 
    {
        pIntBuffer->nConfig |= ADC_BUFFER_CONFIG_BUFFER_SINGLE_CONV_EN;
    }
    pIntBuffer->pUserBuffer = pBuffer;
    pIntBuffer->pCurDataBuffer = pBuffer->pDataBuffer;
    pIntBuffer->nNumSamplesRemaining = nNumChannels * pBuffer->nNumConversionPasses;
    pIntBuffer->nChannels = pBuffer->nChannels;

    bpushIntBuffer (&pDevice->pPendingList, pIntBuffer);

    InitBufferProcessing(pDevice);

    return ADI_ADC_SUCCESS;
}

/**
 * @brief    Get a processed buffer from the ADC Driver. This function is a blocking call and will only return
 *           once it has the buffer or if any error occurred. If a callback is registered then any call to this
 *           function will fail.
 *
 * @param [in]  hDevice    Handle to the device instance.
 * @param [out] ppBuffer   Pointer to a pointer to ADI_ADC_BUFFER structure. The returned pointer
 *                         to ADI_ADC_BUFFER is written here.
 *
 * @return  Status
 *           - #ADI_ADC_SUCCESS                     Successfully returned the buffer
 *           - #ADI_ADC_INVALID_STATE               adi_adc_GetBuffer cannot be called when no buffer is given to the driver for processing.
 *           - #ADI_ADC_INVALID_DEVICE_HANDLE   [D] Invalid device handle passed to the API 
 *           - #ADI_ADC_INVALID_OPERATION       [D] adi_adc_GetBuffer cannot be used when callback is registered.
 *           - #ADI_ADC_NULL_POINTER            [D] ppBuffer is NULL
 *           - #ADI_ADC_INVALID_SEQUENCE        [D] adi_adc_GetBuffer cannot be used if non-blocking is not enabled.
 *
 */
ADI_ADC_RESULT adi_adc_GetBuffer (
    ADI_ADC_HANDLE hDevice,
    ADI_ADC_BUFFER **ppBuffer)
{
    ADI_ADC_DEVICE *pDevice = AdcDevicePointer (hDevice);
    ADI_ADC_RESULT eResult = ADI_ADC_SUCCESS;

    ADC_INT_BUFFER* pCompIntBuffer;
    
    ADC_INT_BUFFER* volatile pCompletedListInt;

#ifdef ADI_DEBUG
    if (pDevice == NULL)
    {
        return ADI_ADC_INVALID_DEVICE_HANDLE;
    }
    if (ppBuffer == NULL) {
            return ADI_ADC_NULL_POINTER;
    }
    if (pDevice->pfCallback != NULL) {
        return ADI_ADC_INVALID_OPERATION;
    }
    if (IS_NOT_IN_STATE(ADC_STATUS_NON_BLOCKING_EN)) {
        return ADI_ADC_INVALID_SEQUENCE;
    } 
#endif /* ADI_DEBUG */
    
    if ((pDevice->pActiveList == NULL) && (pDevice->pPendingList == NULL) && (pDevice->pCompletedList == NULL)) {
        return ADI_ADC_INVALID_STATE;
    }
    
    

    do {
        PEND_EVENT(pDevice, ADI_ADC_ERR_RTOS);
        pCompletedListInt = pDevice->pCompletedList;
    } while (pCompletedListInt == NULL);

    pCompIntBuffer = fpopIntBuffer(&pDevice->pCompletedList);
    *ppBuffer = pCompIntBuffer->pUserBuffer;
    if ((pCompIntBuffer->nStatus & ADC_BUFFER_STATUS_OVERFLOW) != 0u) {
        eResult =  ADI_ADC_BUFFER_OVERFLOW;
    }

    fpushIntBuffer(&pDevice->pFreeList, pCompIntBuffer);

    return eResult;
}



/**
 * @brief   Enable/Disable ADC for sampling
 *
 * @param [in] hDevice    Handle to the device instance
 *
 * @param [in] bEnable    'true' to Enable and 'false' to disable
 *
 * @details
 *
 * @return   Status
 *           - #ADI_ADC_SUCCESS                     Succesfully Enabled or disabled ADC for sampling
 *           - #ADI_ADC_INVALID_DEVICE_HANDLE   [D] Invalid device handle passed to the API 
 *           - #ADI_ADC_INVALID_STATE           [D] Non-blocking cannot be enabled if comparator is enabled or any blocking API is in progress.
 */
ADI_ADC_RESULT adi_adc_Enable (
    ADI_ADC_HANDLE hDevice,
    bool_t         bEnable)
{
    ADI_ADC_DEVICE *pDevice = AdcDevicePointer (hDevice);
    
#ifdef ADI_DEBUG
    if (pDevice == NULL)
    {
        return ADI_ADC_INVALID_DEVICE_HANDLE;
    }
    
    if (IS_IN_ANY_STATE(ADC_STATUS_BLOCKING_EN | ADC_STATUS_COMPARATOR_EN)) {
        return ADI_ADC_INVALID_STATE;
    }
#endif /* ADI_DEBUG */

    if (bEnable) {
        /* Set the driver to be in non-blocking mode */
        pDevice->nStatus |= (uint32_t)ADC_STATUS_NON_BLOCKING_EN;
        
        /* Enable the IRQs */
        ADI_ENABLE_INT(ADC0_EVT_IRQn);
   
        /* Tru to submit possible number of buffers */
        while (InitBufferProcessing(pDevice) == true) { ; }
    } else {
        /* Enable the IRQs */
        ADI_DISABLE_INT(ADC0_EVT_IRQn);
        
        /* Abort any transaction if present */
        pDevice->pfManageFifo(pDevice, ADC_FIFO_MODE_ABORT);
        
        pDevice->nStatus &= ~(uint32_t)ADC_STATUS_NON_BLOCKING_EN;
    }

    return ADI_ADC_SUCCESS;    
}


/**
 * @brief    This function return whether a filled buffer is available to be returned to the user.
 *           If this function return true, then a call to adi_adc_GetBuffer will not block
 *
 * @param [in]  hDevice               Handle to the device instance.
 * @param [out] pbIsBufferAvailable   Pointer to a bool_t variable to which the availability of buffer will be written.
 *                                    The variable will be set to 'true' if buffer is available else 'false'
 *
 * @return  Status
 *           - #ADI_ADC_SUCCESS                     Successfully returned the status of the buffer availability
 *           - #ADI_ADC_INVALID_DEVICE_HANDLE   [D] Invalid device handle passed to the API 
 *           - #ADI_ADC_NULL_POINTER            [D] pbIsBufferAvailable is valid
 *           - #ADI_ADC_INVALID_OPERATION       [D] adi_adc_IsBufferAvailable cannot be used when callback is registered.
 *
 */
ADI_ADC_RESULT adi_adc_IsBufferAvailable (
    ADI_ADC_HANDLE hDevice,
    bool_t         *pbIsBufferAvailable)
{
    ADI_ADC_DEVICE *pDevice = AdcDevicePointer (hDevice);
#ifdef ADI_DEBUG
    if (pDevice == NULL)
    {
        return ADI_ADC_INVALID_DEVICE_HANDLE;
    }
    if (pbIsBufferAvailable == NULL) 
    {
        return ADI_ADC_NULL_POINTER;
    }
    if (pDevice->pfCallback != NULL) {
        return ADI_ADC_INVALID_OPERATION;
    }
#endif /* ADI_DEBUG */

    if (pDevice->pCompletedList != NULL)
    {
        *pbIsBufferAvailable = true;
    }
    else
    {
        *pbIsBufferAvailable = false;
    }

    return ADI_ADC_SUCCESS;    
}

/**
 * @brief   Sample the given channels for the given number of conversion passes and put it into the given buffer. This function only return after
 *          the channels are sampled the given number of conversion times or if any error occurs. 
 *
 * @param [in] hDevice      Handle to the device instance
 *
 * @param [in] nChannels    Channels to sample. Should be an ORed value of ADI_ADC_CHANNEL enums.
 *
 * @param [in] nNumConversionPasses  Number of conversion passes. In one conversion pass, the ADC will sample all the given channel(s) once.
 *
 * @param [in] pBuffer      Pointer to the buffer to which the sampled data is put.
 *
 * @param [in] nBuffLength  Length of the buffer. The length of the buffer should be at least
 *                          2*(Num of Channels)*nNumConversionPasses bytes.
 *
 * @details Sample all the given channels for the given number of conversion passes and put the samples values into the given buffers. 
 *          The channels will be sampled starting from the lower number. This function only return after
 *          the channels are sampled the given number of conversion times or if any error occurs.
 *
 * @return   Status
 *           - #ADI_ADC_SUCCESS                     Succesfully Enabled or disabled ADC for sampling
 *           - #ADI_ADC_INVALID_DEVICE_HANDLE   [D] Invalid device handle passed to the API 
 *           - #ADI_ADC_INVALID_PARAMETER       [D] Some parameter passed to the function is not valid
 *           - #ADI_ADC_INVALID_SEQUENCE        [D] adi_adc_ReadChannels cannot be called if camparator is enabled or if 
 *                                                  Non-blocking is enabled or if another blocking API is in progress.
 */

ADI_ADC_RESULT adi_adc_ReadChannels (
    ADI_ADC_HANDLE hDevice,
    uint32_t       nChannels,
    uint32_t       nNumConversionPasses,
    void           *pBuffer,
    uint32_t       nBuffLength)
{
    ADI_ADC_DEVICE *pDevice = AdcDevicePointer (hDevice);
    uint32_t nNumChannels = 0u;
    ADI_ADC_RESULT eResult = ADI_ADC_SUCCESS;

    ADC_INT_BUFFER* pIntBuffer;
    ADC_INT_BUFFER* pCompIntBuffer;
    
    ADC_MANAGE_FIFO_FUNC pfManageFifoBackUp;
    
    ADC_INT_BUFFER* volatile pCompletedListInt;

#ifdef ADI_DEBUG
    if (pDevice == NULL)
    {
        return ADI_ADC_INVALID_DEVICE_HANDLE;
    }
    if ((nChannels == 0u) || (nNumConversionPasses == 0u) || (pBuffer == NULL)) 
    {
        return ADI_ADC_INVALID_PARAMETER;
    }
    if (IS_IN_ANY_STATE(ADC_STATUS_NON_BLOCKING_EN | ADC_STATUS_BLOCKING_EN | ADC_STATUS_COMPARATOR_EN)) 
    {
        return ADI_ADC_INVALID_SEQUENCE;
    }
#endif /* ADI_DEBUG */

    nNumChannels = GetNumChannels(nChannels);

    if (nBuffLength < ((nNumChannels * 2u) * nNumConversionPasses))
    {
        return ADI_ADC_INSUFFICIENT_MEMORY;
    }

    /* Clear ADC status */
    pDevice->pReg->STAT = 0xFFFFu;

    
    /* Set the driver to be in blocking mode */
    SET_STATE(ADC_STATUS_BLOCKING_EN);
    
    /* Backup the current FIFO Manager and set the FIFO manager as the blocking fifo manager */
    pfManageFifoBackUp = pDevice->pfManageFifo;
    pDevice->pfManageFifo = pDevice->pfBlockingManageFifo;

    /* Get a free buffer */
    pIntBuffer = fpopIntBuffer(&pDevice->pFreeList);
    
    pIntBuffer->nConfig = ADC_BUFFER_CONFIG_BUFFER_AUTO_MODE_EN;
    if (nNumConversionPasses == 1u) {
        pIntBuffer->nConfig |= ADC_BUFFER_CONFIG_BUFFER_SINGLE_CONV_EN;
    }
    
    pIntBuffer->nStatus = 0u;
    pIntBuffer->pUserBuffer = NULL;
    pIntBuffer->pCurDataBuffer = pBuffer;
    pIntBuffer->nNumSamplesRemaining = nNumChannels * nNumConversionPasses;
    pIntBuffer->nChannels = nChannels;

    bpushIntBuffer (&pDevice->pPendingList, pIntBuffer);

    InitBufferProcessing(pDevice);

    do {
        PEND_EVENT(pDevice, ADI_ADC_ERR_RTOS);
        pCompletedListInt = pDevice->pCompletedList;
    } while (pCompletedListInt == NULL);

    pCompIntBuffer = fpopIntBuffer(&pDevice->pCompletedList);
    assert (pCompIntBuffer == pIntBuffer);
    
    if ((pCompIntBuffer->nStatus & ADC_BUFFER_STATUS_OVERFLOW) != 0u) {
        eResult = ADI_ADC_BUFFER_OVERFLOW;
    }

    fpushIntBuffer(&pDevice->pFreeList, pCompIntBuffer);
    
    /* Restore the FIFO Manager */
    pDevice->pfManageFifo = pfManageFifoBackUp;

    /* Driver is no longer in blocking mode */
    CLR_STATE(ADC_STATUS_BLOCKING_EN);

    /* Enable the IRQs */
    ADI_DISABLE_INT(ADC0_EVT_IRQn);

    return eResult;
}


#if (ADI_ADC_ENABLE_DMA_SUPPORT == 1) && (ADI_ADC_ENABLE_INTERRUPT_MODE_SUPPORT == 1)
/**
 * @brief    Enable/Disable DMA for ADC operation.
 *
 * @param [in]  hDevice        Handle to the device instance.
 * @param [in]  bDMAEnable     'true' to enable DMA and 'false' to disable DMA
 *
 * @return  Status
 *           - #ADI_ADC_SUCCESS                     Successfully enabled/disabled the DMA
 *           - #ADI_ADC_INVALID_DEVICE_HANDLE   [D] Invalid device handle passed to the API 
 */
ADI_ADC_RESULT adi_adc_EnableDmaMode (
    ADI_ADC_HANDLE hDevice,
    bool_t         bDMAEnable)
{
    ADI_ADC_DEVICE *pDevice = AdcDevicePointer (hDevice);
#ifdef ADI_DEBUG
    if (pDevice == NULL)
    {
        return ADI_ADC_INVALID_DEVICE_HANDLE;
    }
#endif /* ADI_DEBUG */
    
    if (bDMAEnable)
    {
        pDevice->pfManageFifo = DmaFIFOManage;
    }
    else 
    {
        pDevice->pfManageFifo = InterruptFIFOManage;
    }
    
#if ADI_ADC_CFG_BLOCKING_MODE == 1
    pDevice->pfBlockingManageFifo =  pDevice->pfManageFifo;
#endif /* ADI_ADC_CFG_BLOCKING_MODE == 1 */

    
    return ADI_ADC_SUCCESS;    
}

#endif /* (ADI_ADC_ENABLE_DMA_SUPPORT == 1) && (ADI_ADC_ENABLE_INTERRUPT_MODE_SUPPORT == 1) */

/**
 * @brief    Returns the battery voltage.
 *
 * @param [in]  hDevice        Handle to the device instance.
 *
 * @param [in]  nRefVoltage    Reference voltage in fixed point(16.16) format.
 *
 * @param [out] pnBatVoltage   Pointer to a variable to which the voltage of the battery will be written.
 *                             The battery voltage will be in fixed point (16.16) format.
 *
 *
 * @return  Status
 *           - #ADI_ADC_SUCCESS                     Successfully retrieved the battery voltage.
 *           - #ADI_ADC_INVALID_DEVICE_HANDLE   [D] Invalid device handle passed to the API 
 *           - #ADI_ADC_NULL_POINTER            [D] pnBatVoltage is NULL
 *           - #ADI_ADC_INVALID_SEQUENCE        [D] ADC sub system should be up and ADC should be free for getting the battery voltage.
 */
ADI_ADC_RESULT adi_adc_GetBatteryVoltage (
    ADI_ADC_HANDLE hDevice,
    uint32_t       nRefVoltage,
    uint32_t       *pnBatVoltage)
{
    ADI_ADC_DEVICE *pDevice = AdcDevicePointer (hDevice);
    uint16_t nConvTimeBackup;
    uint16_t nAvgCfgBackup;
    uint32_t nAdcValue = 0u;
    uint32_t nACLKDIVCNT;
    
    
#ifdef ADI_DEBUG
    if (pDevice == NULL)
    {
        return ADI_ADC_INVALID_DEVICE_HANDLE;
    }
    if (pnBatVoltage == NULL) 
    {
        return ADI_ADC_NULL_POINTER;
    }    
    
    if (IS_NOT_IN_STATE(ADC_STATUS_SUB_SYSTEM_EN)) {
        return ADI_ADC_INVALID_SEQUENCE;
    }
    
    if (IS_IN_ANY_STATE(ADC_STATUS_BLOCKING_EN | ADC_STATUS_NON_BLOCKING_EN | ADC_STATUS_COMPARATOR_EN)) {
        return ADI_ADC_INVALID_SEQUENCE;
    }    
#endif /* ADI_DEBUG */

    /* Take the backup of registers that need to be changed */
    nConvTimeBackup = pDevice->pReg->CNV_TIME;
    nAvgCfgBackup =  pDevice->pReg->AVG_CFG;
    
    /* Set the required value in the registers. */
    nACLKDIVCNT = (*pREG_CLKG0_CLK_CTL1 & BITM_CLKG_CLK_CTL1_ACLKDIVCNT) >> BITP_CLKG_CLK_CTL1_ACLKDIVCNT;
    /* Calculate the number of cycles required for conversion. The covnersion time required is 500ns. 
       The number of cycles are calculated for the maximum core frequency of 26Mhz
       */
    pDevice->pReg->CNV_TIME = (uint16_t)((26u*500u)/nACLKDIVCNT + 1u); 
    pDevice->pReg->AVG_CFG = 0u;
    
    /* Clear the battery done status */
    pDevice->pReg->STAT = BITM_ADC_STAT_BATDONE;
    
    /* Clear the battery done state */
    CLR_STATE(ADC_STATUS_BATTERY_DONE);
    
    /* Set the registers */
    pDevice->pReg->CNV_CFG = (BITM_ADC_CNV_CFG_SINGLE | BITM_ADC_CNV_CFG_BAT);
    
    /* Wait for the Battery done status */
    while (IS_NOT_IN_STATE(ADC_STATUS_BATTERY_DONE)) { ; }
    
    /* Clear the conversion register */
    pDevice->pReg->CNV_CFG = 0u;
    
    /* Restore the changed registers */
    pDevice->pReg->CNV_TIME = nConvTimeBackup;
    pDevice->pReg->AVG_CFG = nAvgCfgBackup;
    
    /* Calculate the battery voltage */
    nAdcValue = pDevice->pReg->BAT_OUT;
    *pnBatVoltage = (4u * nAdcValue * nRefVoltage) / 4095u;
    
    return ADI_ADC_SUCCESS;
}
/**
 * @brief    Enable or disable the temperature sensor
 *
 * @param [in]  hDevice  Handle to the device instance.
 *
 * @param [in]  bEnable  'true' to enable and 'false' to disable the temperature sensor
 *
 *
 * @return  Status
 *           - #ADI_ADC_SUCCESS                     Successfully enabled/disabled the temperature sensor
 *           - #ADI_ADC_INVALID_DEVICE_HANDLE   [D] Invalid device handle passed to the API 
 */
ADI_ADC_RESULT adi_adc_EnableTemperatureSensor (
    ADI_ADC_HANDLE hDevice,
    bool_t         bEnable)
{
    ADI_ADC_DEVICE *pDevice = AdcDevicePointer (hDevice);

#ifdef ADI_DEBUG
    if (pDevice == NULL)
    {
        return ADI_ADC_INVALID_DEVICE_HANDLE;
    }
#endif /* ADI_DEBUG */

    if (bEnable)
    {
        pDevice->pReg->CFG |= (uint16_t)BITM_ADC_CFG_TMPEN;
        SET_STATE(ADC_STATUS_TEMP_SENSOR_EN);
    }
    else
    {
        pDevice->pReg->CFG &= (uint16_t)(~BITM_ADC_CFG_TMPEN);
        CLR_STATE(ADC_STATUS_TEMP_SENSOR_EN);
    }

    return ADI_ADC_SUCCESS;
}


/**
 * @brief    Return the temperature in fixed point format in degree Celcius.
 *
 * @param [in]  hDevice        Handle to the device instance.
 *
 * @param [in]  nRefVoltage    Reference voltage in fixed point(16.16) format.
 *
 * @param [out] pnTemperature  Pointer to a variable to which the ADC die temperature (in degree Celsius) will be written.
 *                             The temperature will be in fixed point (16.16) format.
 *
 *
 * @return  Status
 *           - #ADI_ADC_SUCCESS                     Successfully retrieved the die temperature
 *           - #ADI_ADC_INVALID_DEVICE_HANDLE   [D] Invalid device handle passed to the API 
 *           - #ADI_ADC_NULL_POINTER            [D] pnBatVoltage is NULL
 *           - #ADI_ADC_INVALID_SEQUENCE        [D] ADC sub system should be up and ADC should be free for getting the battery voltage. The Temperator
 *                                                  sensor also need to be enabled.
 *           - #ADI_ADC_INVALID_STATE           [D] Temperature sensor require an aquisition time of 65us and that cannot be set with the current
 *                                                  ACLK since only ACLK of 255 can be stored to the sampling register. Decrease the ACLK clock to 
 *                                                  rectify this.
 */
ADI_ADC_RESULT adi_adc_GetTemperature (
    ADI_ADC_HANDLE  hDevice,
    uint32_t        nRefVoltage,
    int32_t*        pnTemperature
    )
{
    ADI_ADC_DEVICE *pDevice = AdcDevicePointer (hDevice);
    uint16_t nConvTimeBackup;
    uint16_t nAvgCfgBackup;
    uint32_t nAdcTmpValue = 0u;
    uint32_t nAdcTmp2Value = 0u;
    uint32_t nACLKDIVCNT;
    uint32_t nCnvTime;

    
    
#ifdef ADI_DEBUG
    if (pDevice == NULL)
    {
        return ADI_ADC_INVALID_DEVICE_HANDLE;
    }
    if (pnTemperature == NULL) 
    {
        return ADI_ADC_NULL_POINTER;
    } 
    
    if (IS_NOT_IN_STATE(ADC_STATUS_SUB_SYSTEM_EN | ADC_STATUS_TEMP_SENSOR_EN))
    {
        return ADI_ADC_INVALID_SEQUENCE;
    }
        
    if (IS_IN_ANY_STATE(ADC_STATUS_BLOCKING_EN | ADC_STATUS_NON_BLOCKING_EN | ADC_STATUS_COMPARATOR_EN))
    {
        return ADI_ADC_INVALID_SEQUENCE;
    }    
#endif    
    
    /* Calculate the conversion time */
    nACLKDIVCNT = (*pREG_CLKG0_CLK_CTL1 & BITM_CLKG_CLK_CTL1_ACLKDIVCNT) >> BITP_CLKG_CLK_CTL1_ACLKDIVCNT;
    nCnvTime = (26u * 65u) / nACLKDIVCNT + 1u;   /* 65us acquisition time required */
    
#ifdef ADI_DEBUG
    if (nCnvTime >= 256u) {
        return ADI_ADC_INVALID_STATE;
    }
#endif
    /* Take the backup of registers that need to be changed */
    nConvTimeBackup = pDevice->pReg->CNV_TIME;
    nAvgCfgBackup =  pDevice->pReg->AVG_CFG;
    
    /* Set the required value in the registers. */
    
    pDevice->pReg->CNV_TIME = (uint16_t)((nCnvTime << BITP_ADC_CNV_TIME_SAMPTIME) & BITM_ADC_CNV_TIME_SAMPTIME);
    pDevice->pReg->AVG_CFG = 0u;
    
    /* Clear the temperature done status */
    pDevice->pReg->STAT =  BITM_ADC_STAT_TMPDONE | BITM_ADC_STAT_TMP2DONE;
    
    /* Clear the temperature done state */
    CLR_STATE(ADC_STATUS_TMP_DONE | ADC_STATUS_TMP2_DONE);
    
    /* Sample Tmp register */
    pDevice->pReg->CNV_CFG = (BITM_ADC_CNV_CFG_SINGLE | BITM_ADC_CNV_CFG_TMP);
    while (IS_NOT_IN_STATE(ADC_STATUS_TMP_DONE)) { ; }
    nAdcTmpValue = pDevice->pReg->TMP_OUT;
    pDevice->pReg->CNV_CFG = 0u;
    
    
    /* Sample Tmp2 register */
    pDevice->pReg->CNV_CFG = (BITM_ADC_CNV_CFG_SINGLE | BITM_ADC_CNV_CFG_TMP2);
    while (IS_NOT_IN_STATE(ADC_STATUS_TMP2_DONE)) { ; }
    pDevice->pReg->CNV_CFG = 0u;
    nAdcTmp2Value = pDevice->pReg->TMP2_OUT;

    /* Restore the changed registers */
    pDevice->pReg->CNV_TIME = nConvTimeBackup;
    pDevice->pReg->AVG_CFG = nAvgCfgBackup;
    
    /* Calculate the temperature voltage */
    {
        uint32_t nRVirRefByIdealSensitivity = 2070960834u; /* 1.2256/1.2411e-3 in 11.21 format */
        
        uint32_t nTempRG = 19380u; /* 1.1829 in 2.14 format */
        uint32_t nTmp2 = ((nAdcTmp2Value << 14u) + nTempRG * nAdcTmpValue); /* in 14.14 format */
        
        uint32_t nOffsetPart = (335544320u/nRefVoltage); /* (1.25 in 4.28 format / ReferenceVoltage(16.16)) = Result in format *.12 */
        uint32_t nOffset = (161u * nOffsetPart);        /* 12.12 format */
        
        uint32_t nTmp3  = ((nAdcTmpValue << 12) - nOffset) << 8u;  /* Format 12.20 */
        uint32_t nRatio = (nTmp3/(nTmp2 >> 10u));  /* nTmp2 resolution reduced by 10 to 14.4 and the result resolution is 0.16 */
        uint32_t nTemp  = (nRatio * (nRVirRefByIdealSensitivity >> 16u)) >> 5u;  /* Temperature in degree kelvin in 16.16 format */
        
        int32_t  iTemp = (int32_t)nTemp - 17901158;   /* Subtract 273.15 (in 16.16) to get the temperature in degree celcius */
        *pnTemperature = iTemp;
    }

    return ADI_ADC_SUCCESS;
}


/*! \cond PRIVATE */

/*==========  S T A T I C    F U N C T I O N S  ==========*/
/* Read the output register for the given channel number */
static uint16_t ReadOutReg(uint32_t nChannelNum)
{
    const volatile uint16_t* pOutRegister = pREG_ADC0_CH0_OUT;
    pOutRegister += nChannelNum*2u;
    return *pOutRegister;
}

/* Init buffer processing */
static bool_t InitBufferProcessing(ADI_ADC_DEVICE *pDevice)
{
    ADC_INT_BUFFER* pIntBuffer;
    uint32_t nCnvReg;

    
    
    if (   (IS_NOT_IN_ANY_STATE(ADC_STATUS_BLOCKING_EN | ADC_STATUS_NON_BLOCKING_EN))
        || (pDevice->pActiveList != NULL)) 
    {
        /* Return immediately if no mode of operation is enabled or there is another computation on process */
        return false;
    }

    pIntBuffer = fpopIntBuffer(&pDevice->pPendingList);
    if (pIntBuffer == NULL) {
        return false;
    }

    if (IS_NOT_IN_ANY_STATE(ADC_STATUS_SAMPLING_IN_PROGRESS)) 
    {
        /* Clear conversi1on done flags */
        pDevice->pReg->STAT = 0xFFFFu;

        /* Clear the overflow and alert register */
        pDevice->pReg->OVF = 0xFFFFu;
    }
    
    /* Calculate the conversion register value for the given configuration */
    nCnvReg = pIntBuffer->nChannels;
    if ((pIntBuffer->nConfig & ADC_BUFFER_CONFIG_BUFFER_AUTO_MODE_EN) != 0u) {
        nCnvReg |= BITM_ADC_CNV_CFG_AUTOMODE;
    }    
    if ((pIntBuffer->nConfig & ADC_BUFFER_CONFIG_BUFFER_SINGLE_CONV_EN) != 0u) {
        nCnvReg |= BITM_ADC_CNV_CFG_SINGLE;
    } else {
        nCnvReg |= BITM_ADC_CNV_CFG_MULTI;
    }    

    bpushIntBuffer(&pDevice->pActiveList, pIntBuffer);
    pDevice->pfManageFifo(pDevice, ADC_FIFO_MODE_INIT);
    
    pDevice->pReg->CNV_CFG |= (uint16_t)nCnvReg;
    pDevice->nStatus |= (uint32_t)ADC_STATUS_SAMPLING_IN_PROGRESS;
    
    pDevice->pfManageFifo(pDevice, ADC_FIFO_MODE_ENABLED);

    return true;
}


#if ADI_ADC_ENABLE_DMA_SUPPORT == 1
/* DMA Callback Handler */
static void DMAHandler (void *pcbParam, uint32_t Event, void *pArg)
{
    ADI_ADC_DEVICE *pDevice = pcbParam;
    
    switch(Event)
    {
    case ADI_DMA_EVENT_BUFFER_PROCESSED:
        DmaFIFOManage(pDevice, ADC_FIFO_MODE_DMA_BUFFER_PROCESS);
        break;
        
    case ADI_DMA_EVENT_ERR_INVALID_DESCRIPTOR:
        DmaFIFOManage(pDevice, ADC_FIFO_MODE_DMA_INVALID_DESC);
        break;
        
    default:
        break;
    }
}

static ADI_ADC_RESULT DmaFIFOManage (ADI_ADC_DEVICE *pDevice, ADC_FIFO_MODE eFifoMode)
{
    ADC_INT_BUFFER* pIntBuffer = pDevice->pActiveList;
    if (pIntBuffer == NULL) {
        if (eFifoMode == ADC_FIFO_MODE_INTERRUPT_PROCESS) {
            uint16_t nStat = pDevice->pReg->STAT & 0x00FFu;
            FlushFifo(pDevice, (uint32_t)nStat);
            pDevice->pReg->STAT = nStat;
        }
        return ADI_ADC_SUCCESS;
    }

    switch (eFifoMode)
    {
    case ADC_FIFO_MODE_INIT:
        {
            ADI_DMA_TRANSFER DmaTransfer;
            
            /* Enable DMA */
            pDevice->pReg->CNV_CFG |= BITM_ADC_CNV_CFG_DMAEN;
            
            
            if (pDevice->hDmaHandle == NULL) {
                if (adi_dma_Open(ADI_DMA_CHANNEL_ADC0, &pDevice->DmaMemory[0], &pDevice->hDmaHandle, DMAHandler, pDevice) != ADI_DMA_SUCCESS) {
                    return ADI_ADC_DMA_ERROR;
                }
                if  (adi_dma_SetArbitrationRate(pDevice->hDmaHandle, ADI_DMA_RPOWER_1) != ADI_DMA_SUCCESS) {
                    return ADI_ADC_DMA_ERROR;
                }
                if (adi_dma_Enable(pDevice->hDmaHandle, true) != ADI_DMA_SUCCESS) {
                    return ADI_ADC_DMA_ERROR;
                }
            }
            
            DmaTransfer.DataWidth = ADI_DMA_WIDTH_2_BYTE;
            DmaTransfer.pSrcData = &pDevice->pReg->DMA_OUT;
            DmaTransfer.pDstData = pIntBuffer->pCurDataBuffer;
            DmaTransfer.SrcInc = ADI_DMA_INCR_NONE;
            DmaTransfer.DstInc = ADI_DMA_INCR_2_BYTE;
            DmaTransfer.NumTransfers = pIntBuffer->nNumSamplesRemaining;
                
            if (adi_dma_SubmitTransfer(pDevice->hDmaHandle, &DmaTransfer) != ADI_DMA_SUCCESS) {
                return ADI_ADC_DMA_ERROR;
            }
        }
        break;

    case ADC_FIFO_MODE_ENABLED:
        break;

    case ADC_FIFO_MODE_INTERRUPT_PROCESS:
        /* Clear the status registers */
        pDevice->pReg->STAT = (pDevice->pReg->STAT & 0x00FFu);
        break;

    case ADC_FIFO_MODE_INTERRUPT_OVERFLOW:
        if (pDevice->pfCallback != NULL) {
           if (IS_IN_ALL_STATES(ADC_STATUS_NON_BLOCKING_EN)) {
                pDevice->pfCallback(pDevice->pCBParam, ADI_ADC_EVENT_OVERFLOW, (void*)(uint32_t)pDevice->pReg->OVF);
           }
        }
        break;

        
    case ADC_FIFO_MODE_DMA_BUFFER_PROCESS:
        {
            pIntBuffer->nNumSamplesRemaining = 0u;
            ManageFifoCompletion(pDevice);

        }
        break;
    case ADC_FIFO_MODE_DMA_INVALID_DESC:
        {
            /* Close the DMA */
            assert (adi_dma_Enable(pDevice->hDmaHandle, false) == ADI_DMA_SUCCESS);
            assert (adi_dma_Close(pDevice->hDmaHandle) == ADI_DMA_SUCCESS);
            pDevice->hDmaHandle = NULL;

            pIntBuffer->nNumSamplesRemaining = 0u;
            ManageFifoCompletion(pDevice);
        }
        break;
        
    case ADC_FIFO_MODE_ABORT:
      {
          uint16_t IRQ_Backup;
    
          /* Take backup of IRQ */
          IRQ_Backup = pDevice->pReg->IRQ_EN;

          /* Disable the IRQ */
          pDevice->pReg->IRQ_EN = 0u;

          /* Clear the conversion cfg register to stop any transaction */
          pDevice->pReg->CNV_CFG = 0u;
          
          /* Close the DMA */
          assert (adi_dma_Enable(pDevice->hDmaHandle, false) == ADI_DMA_SUCCESS);
          assert (adi_dma_Close(pDevice->hDmaHandle) == ADI_DMA_SUCCESS);
          pDevice->hDmaHandle = NULL;
          
          /* Clear the status bits */
          pDevice->pReg->STAT =  pDevice->pReg->STAT;
    
          /* Clear the sampling in progress state */
          CLR_STATE(ADC_STATUS_SAMPLING_IN_PROGRESS);
          
          /* Clear all buffer and FIFO */
          ClearAllBufferAndFifo(pDevice);
        
          /* Restore the IRQ */
          pDevice->pReg->IRQ_EN = IRQ_Backup;      
      }
      break;


    default:
        break;
    } 
    
    return ADI_ADC_SUCCESS;
}
#endif /* ADI_ADC_ENABLE_DMA_SUPPORT == 1 */

static ADI_ADC_RESULT InterruptFIFOManage (ADI_ADC_DEVICE *pDevice, ADC_FIFO_MODE eFifoMode)
{
    ADC_INT_BUFFER* pIntBuffer = pDevice->pActiveList;
    if (pIntBuffer == NULL) {
        if (eFifoMode == ADC_FIFO_MODE_INTERRUPT_PROCESS) {
            uint16_t nStat = pDevice->pReg->STAT & 0x00FFu;
            FlushFifo(pDevice, (uint32_t)nStat);
            pDevice->pReg->STAT = nStat;
        }
        return ADI_ADC_SUCCESS;
    }

    switch (eFifoMode)
    {
    case ADC_FIFO_MODE_INIT:
        {
            /* Enable the conversion done and overflow interrupt */
            pDevice->ActData.nCurChannel = 0u;
        }
        break;

    case ADC_FIFO_MODE_ENABLED:
        break;

    case ADC_FIFO_MODE_INTERRUPT_PROCESS:
        {
            while (pIntBuffer->nNumSamplesRemaining > 0u) {
                uint32_t nConvStatus = ((uint32_t)pDevice->pReg->STAT & (uint32_t)0x00FFu);
                if ((nConvStatus & 0x00FFu) == 0u)
                {
                    break;
                }
                
                uint32_t nCurChannelBitM = ((uint32_t)1u << pDevice->ActData.nCurChannel);
                while ((nCurChannelBitM & nConvStatus) == 0u) {
                    pDevice->ActData.nCurChannel++;
                    if (pDevice->ActData.nCurChannel >= NUM_ADC_CHANNELS) {
                        pDevice->ActData.nCurChannel = 0u;
                    }
                    nCurChannelBitM = ((uint32_t)1u << pDevice->ActData.nCurChannel);
                }
                
                assert ((pIntBuffer->nChannels & ((uint32_t)1u << pDevice->ActData.nCurChannel)) != 0u);
                
                *pIntBuffer->pCurDataBuffer = ReadOutReg( pDevice->ActData.nCurChannel);
                pIntBuffer->pCurDataBuffer++;
                
                
                pDevice->pReg->STAT = (uint16_t)nCurChannelBitM;
                pIntBuffer->nNumSamplesRemaining--;

                pDevice->ActData.nCurChannel++;
                if ( pDevice->ActData.nCurChannel >= NUM_ADC_CHANNELS) {
                     pDevice->ActData.nCurChannel = 0u;
                }
            }

            if (pIntBuffer->nNumSamplesRemaining == 0u) {
                ManageFifoCompletion(pDevice);
            }
        }
        break;
        
    case ADC_FIFO_MODE_INTERRUPT_OVERFLOW:
        {
            pIntBuffer->nStatus |= ADC_BUFFER_STATUS_OVERFLOW;
            if (pDevice->pfCallback != NULL) {
               if (IS_IN_ALL_STATES(ADC_STATUS_NON_BLOCKING_EN)) {
                    uint32_t nArg = ((uint32_t)pDevice->pReg->OVF & (uint32_t)0x00FFu);
                    pDevice->pfCallback(pDevice->pCBParam, ADI_ADC_EVENT_OVERFLOW, (void*)nArg);
               }
            }
        }
        break;
        
    case ADC_FIFO_MODE_ABORT:
        {
          uint16_t IRQ_Backup;
    
          /* Take backup of IRQ */
          IRQ_Backup = pDevice->pReg->IRQ_EN;

          /* Disable the IRQ */
          pDevice->pReg->IRQ_EN = 0u;

          /* Clear the conversion cfg register to stop any transaction */
          pDevice->pReg->CNV_CFG = 0u;
          
          /* Clear the status bits */
          pDevice->pReg->STAT =  pDevice->pReg->STAT;
    
          /* Clear the sampling in progress state */
          CLR_STATE(ADC_STATUS_SAMPLING_IN_PROGRESS);
          
          /* Clear all buffer and fifo */
          ClearAllBufferAndFifo(pDevice);
        
          /* Restore the IRQ */
          pDevice->pReg->IRQ_EN = IRQ_Backup; 
        }
        break;

    default:
        break;
    }
    
    return ADI_ADC_SUCCESS;
}

#if ADI_ADC_CFG_BLOCKING_MODE != 1
static ADI_ADC_RESULT PollingFIFOManage (ADI_ADC_DEVICE *pDevice, ADC_FIFO_MODE eFifoMode)
{
    ADC_INT_BUFFER* pIntBuffer = pDevice->pActiveList;
    if (pIntBuffer == NULL) {
        if (eFifoMode == ADC_FIFO_MODE_INTERRUPT_PROCESS) {
            uint16_t nStat = pDevice->pReg->STAT & 0x00FFu;
            FlushFifo(pDevice, (uint32_t)nStat);
            pDevice->pReg->STAT = nStat;
        }
        return ADI_ADC_SUCCESS;
    }

    switch (eFifoMode)
    {
    case ADC_FIFO_MODE_INIT:
        pDevice->pReg->IRQ_EN &= (uint16_t)~BITM_ADC_IRQ_EN_CNVDONE;
        break;

    case ADC_FIFO_MODE_ENABLED:
        {
            uint32_t nCurChannel = 0u;
            while (pIntBuffer->nNumSamplesRemaining > 0u)
            {
                uint32_t nCurChannelBitM = ((uint32_t)1u << nCurChannel);
                if ((nCurChannelBitM & pIntBuffer->nChannels) != 0u) {
                    while ((pDevice->pReg->STAT & nCurChannelBitM) == 0u) { }

                    *pIntBuffer->pCurDataBuffer = ReadOutReg(nCurChannel);
                    pIntBuffer->pCurDataBuffer++;
                    
                    pDevice->pReg->STAT = (uint16_t)nCurChannelBitM;
                    pIntBuffer->nNumSamplesRemaining--;
                }

                nCurChannel++;
                if (nCurChannel >= NUM_ADC_CHANNELS) {
                    nCurChannel = 0u;
                }
            }
            
            
            pDevice->pReg->IRQ_EN |= BITM_ADC_IRQ_EN_CNVDONE;

            ManageFifoCompletion(pDevice);
        }
        break;
        
    case ADC_FIFO_MODE_INTERRUPT_OVERFLOW:
        {
            pIntBuffer->nStatus |= ADC_BUFFER_STATUS_OVERFLOW;
            if (pDevice->pfCallback != NULL) {
               if (IS_IN_ALL_STATES(ADC_STATUS_NON_BLOCKING_EN)) {
                    uint32_t nArg = ((uint32_t)pDevice->pReg->OVF & (uint32_t)0x00FFu);
                    pDevice->pfCallback(pDevice->pCBParam, ADI_ADC_EVENT_OVERFLOW, (void*)nArg);
               }
            }
        }
        break;		
        
    case ADC_FIFO_MODE_INTERRUPT_PROCESS:
        /* Clear the STAT */
        pDevice->pReg->STAT = pDevice->pReg->STAT;
        break;

        
    case ADC_FIFO_MODE_ABORT:
        {
          uint16_t IRQ_Backup;
    
          /* Take backup of IRQ */
          IRQ_Backup = pDevice->pReg->IRQ_EN;

          /* Disable the IRQ */
          pDevice->pReg->IRQ_EN = 0u;

          /* Clear the conversion cfg register to stop any transaction */
          pDevice->pReg->CNV_CFG = 0u;
          
          /* Clear the status bits */
          pDevice->pReg->STAT =  pDevice->pReg->STAT;
    
          /* Clear the sampling in progress state */
          CLR_STATE(ADC_STATUS_SAMPLING_IN_PROGRESS);
          
          /* Clear all buffer and fifo */
          ClearAllBufferAndFifo(pDevice);
        
          /* Restore the IRQ */
          pDevice->pReg->IRQ_EN = IRQ_Backup; 
        }
        break;
        

    default:
        break;
    }
    
    return ADI_ADC_SUCCESS;
}
#endif /* ADI_ADC_CFG_BLOCKING_MODE */

static void FlushFifo(ADI_ADC_DEVICE *pDevice, uint32_t nChannels)
{
    uint32_t x;
    for (x = 0u; x < 8u; x++) {
      if ((nChannels & ((uint32_t)1u << x)) != 0u) {
          ReadOutReg(x);
      }
    }
}

static void ManageFifoCompletion(ADI_ADC_DEVICE *pDevice)
{
    ADC_INT_BUFFER* pIntBuffer;
   
    pIntBuffer = fpopIntBuffer(&pDevice->pActiveList);
    if (pIntBuffer == NULL) 
    {
        return;
    }
    
    /* Clear the conversion configuration */
    pDevice->pReg->CNV_CFG = 0u;
    pDevice->nStatus &= ~(uint32_t)ADC_STATUS_SAMPLING_IN_PROGRESS;

    if (pDevice->pfCallback != NULL) {
       if (IS_IN_ALL_STATES(ADC_STATUS_NON_BLOCKING_EN)) {
            pDevice->pfCallback(pDevice->pCBParam, ADI_ADC_EVENT_BUFFER_PROCESSED, pIntBuffer->pUserBuffer);
        
            /* Push the buffer to free queue */
            bpushIntBuffer(&pDevice->pFreeList, pIntBuffer);
       }
    } else {
        /* Push the active buffer into the completed buffer */
        bpushIntBuffer(&pDevice->pCompletedList, pIntBuffer);

        POST_EVENT(pDevice);
    }
    
    if (IS_IN_ALL_STATES(ADC_STATUS_NON_BLOCKING_EN))
    {
        InitBufferProcessing(pDevice);
    }
}

static void ClearAllBufferAndFifo(ADI_ADC_DEVICE *pDevice) 
{
    ADC_INT_BUFFER* pIntBuffer;
    
    /* Clear all buffers */
    while (1) {
        if (pDevice->pActiveList != NULL) {
            pIntBuffer = fpopIntBuffer(&pDevice->pActiveList);
        } else if (pDevice->pPendingList != NULL) {
            pIntBuffer = fpopIntBuffer(&pDevice->pPendingList);
        } else if (pDevice->pCompletedList != NULL) {
            pIntBuffer = fpopIntBuffer(&pDevice->pCompletedList);
        } else {
            pIntBuffer = NULL;
        }
        
        if (pIntBuffer != NULL) {
            bpushIntBuffer(&pDevice->pFreeList, pIntBuffer);
        } else {
            break;
        }
    }
    
    /* Read and flush all the buffers */
    FlushFifo(pDevice, 0x00FFu);
}


static uint32_t GetNumChannels(uint32_t nChannels)
{
    uint32_t n = nChannels & 0x000000FFu;

    n = (n & 0x00000055u) + ((n >> 1u) & 0x00000055u);
    n = (n & 0x00000033u) + ((n >> 2u) & 0x00000033u);
    n = (n + (n >> 4u)) & (0x0000000Fu);

    return n;
}

static int32_t nGetChannelNumber(ADI_ADC_CHANNEL eChannel)
{
    int32_t retVal = 0;
    uint32_t nChannel = (uint32_t)eChannel & 0x000000FFu;
    
    if ((nChannel & (nChannel - 1u)) != 0u) {
        return -1;
    }
    if ((nChannel & 0x000000AAu) != 0u) { retVal += 1; }
    if ((nChannel & 0x000000CCu) != 0u) { retVal += 2; }
    if ((nChannel & 0x000000F0u) != 0u) { retVal += 4; }
    
    return retVal;
}

static void StaticConfiguration(ADI_ADC_DEVICE *pDevice)
{
    uint16_t nCfgReg = 0u;

    /* Configure the resolution */
#if ADI_ADC_CFG_RESOLUTION == 12
    pDevice->pReg->AVG_CFG = 0u; 
#else    

#if ADI_ADC_CFG_RESOLUTION == 13
    pDevice->pReg->AVG_CFG =  BITM_ADC_AVG_CFG_OS | BITM_ADC_AVG_CFG_EN | (0x0002u << BITP_ADC_AVG_CFG_FACTOR);
#elif ADI_ADC_CFG_RESOLUTION == 14     
    pDevice->pReg->AVG_CFG =  BITM_ADC_AVG_CFG_OS | BITM_ADC_AVG_CFG_EN | (0x0008u << BITP_ADC_AVG_CFG_FACTOR);
#elif ADI_ADC_CFG_RESOLUTION == 15
    pDevice->pReg->AVG_CFG =  BITM_ADC_AVG_CFG_OS | BITM_ADC_AVG_CFG_EN | (0x0020u << BITP_ADC_AVG_CFG_FACTOR);
#elif ADI_ADC_CFG_RESOLUTION == 16     
    pDevice->pReg->AVG_CFG =  BITM_ADC_AVG_CFG_OS | BITM_ADC_AVG_CFG_EN | (0x0080u << BITP_ADC_AVG_CFG_FACTOR);
#else
#error "Invalid Resolution"    
#endif

    SET_STATE(ADC_STATUS_OVERSAMPLING_EN);    
#endif


    /* Configure the VREF */
#if ADI_ADC_CFG_VREF == 0     /* 1.25V Internal Reference*/
    nCfgReg |= BITM_ADC_CFG_REFBUFEN | BITM_ADC_CFG_VREFSEL;
#elif ADI_ADC_CFG_VREF == 1     /* 2.5V Internal Reference */    
    nCfgReg |= BITM_ADC_CFG_REFBUFEN;
#endif

    pDevice->pReg->CFG = nCfgReg;
}



static ADI_ADC_DEVICE *AdcDevicePointer(ADI_ADC_HANDLE hDevice)
{
#ifdef ADI_DEBUG
    if (ValidateAdcHandle (hDevice) != ADI_ADC_SUCCESS)
    {
        return NULL;
    }
    else
#endif /* ADI_DEBUG */
    {
        /* In current implementation, we use device pointer as device handle. */
        return (ADI_ADC_DEVICE *)hDevice;
    }
}

static ADI_ADC_INFO *AdcDeviceInfo(ADI_ADC_HANDLE hDevice)
{
    ADI_ADC_INFO *pAdcInfo = NULL;
    uint32_t i;

    /* FOR (All ADC Device instances) */
    for (i = 0u; i < (sizeof (AdcDevInfo) / sizeof (AdcDevInfo[0])); i++)
    {
        /* IF (ADC handle is valid) */
        if (hDevice == AdcDevInfo[i].hDevice)
        {
            pAdcInfo = &(AdcDevInfo[i]);
            break;
        }
    }

    return pAdcInfo;
}

static void ConfigureComparator(ADI_ADC_DEVICE *pDevice)
{
    uint32_t x;
    uint32_t nCnvCfg = 0u;
    static volatile uint16_t* pLO_Register[4]  = {pREG_ADC0_LIM0_LO, pREG_ADC0_LIM1_LO, pREG_ADC0_LIM2_LO, pREG_ADC0_LIM3_LO};
    static volatile uint16_t* pHI_Register[4]  = {pREG_ADC0_LIM0_HI, pREG_ADC0_LIM1_HI, pREG_ADC0_LIM2_HI, pREG_ADC0_LIM3_HI};
    static volatile uint16_t* pHYS_Register[4] = {pREG_ADC0_HYS0,    pREG_ADC0_HYS1,    pREG_ADC0_HYS2,    pREG_ADC0_HYS3};
    
    
    for (x = 0u; x < NUM_ADC_COMPARATOR_CHANNELS; x++) {
        uint32_t nCompConfig = pDevice->nCompConfig;
        if (
            (nCompConfig 
                & ((uint32_t)1u << (BITS_ADC_COMP_CFG_CH * x + BITS_ADC_COMP_CFG_EN))
             ) != 0u) 
        {
            if (
                (nCompConfig 
                    & ((uint32_t)1u << (BITS_ADC_COMP_CFG_CH * x + BITS_ADC_COMP_CFG_LOW))
                 ) != 0u) {
                *pLO_Register[x] |= BITM_ADC_LIM0_LO_EN;
            }
            if (
                (nCompConfig  
                    & ((uint32_t)1u << (BITS_ADC_COMP_CFG_CH*x + BITS_ADC_COMP_CFG_HIGH))
                 ) != 0u) {
                *pHI_Register[x] |= BITM_ADC_LIM0_HI_EN;
            }
            if (
                (nCompConfig 
                    & ((uint32_t)1u << (BITS_ADC_COMP_CFG_CH*x + BITS_ADC_COMP_CFG_HYS))
                 ) != 0u) {
                *pHYS_Register[x] |= BITM_ADC_HYS0_EN;
            }
            nCnvCfg |= (uint32_t)1u << x;
        } else {
            *pLO_Register[x]  &= (uint16_t)~BITM_ADC_LIM0_LO_EN;
            *pHI_Register[x]  &= (uint16_t)~BITM_ADC_LIM0_HI_EN;
            *pHYS_Register[x] &= (uint16_t)~BITM_ADC_HYS0_EN;
        }
    }
    
    if (nCnvCfg != 0u) {
        pDevice->pReg->IRQ_EN &= (uint16_t)~BITM_ADC_IRQ_EN_CNVDONE;
        pDevice->pReg->CNV_CFG = (uint16_t)(nCnvCfg | BITM_ADC_CNV_CFG_MULTI | BITM_ADC_CNV_CFG_AUTOMODE);
        SET_STATE(ADC_STATUS_COMPARATOR_EN);
        
    } else {
        pDevice->pReg->CNV_CFG = 0u;
        pDevice->pReg->STAT = pDevice->pReg->STAT & 0x00FFu;
        CLR_STATE(ADC_STATUS_COMPARATOR_EN);
        pDevice->pReg->IRQ_EN |= BITM_ADC_IRQ_EN_CNVDONE;
    }
}

ADI_INT_HANDLER(ADC_Int_Handler)
{
    ISR_PROLOG();
    ADI_ADC_DEVICE *pDevice = (ADI_ADC_DEVICE *) AdcDevInfo[0].hDevice;
    
    if ((pDevice->pReg->STAT & 0x00FFu) != 0u) {
        if (IS_NOT_IN_STATE(ADC_STATUS_COMPARATOR_EN)) {
            pDevice->pfManageFifo(pDevice, ADC_FIFO_MODE_INTERRUPT_PROCESS);
        } else {
            pDevice->pReg->STAT = pDevice->pReg->STAT & (0x00FFu);
        }
    }
    if ((pDevice->pReg->STAT & 0xFF00u) != 0u) {
        if ((pDevice->pReg->STAT & BITM_ADC_STAT_RDY) != 0u) {
            SET_STATE(ADC_STATUS_SUB_SYSTEM_READY);
            pDevice->pReg->STAT = BITM_ADC_STAT_RDY;
            if (pDevice->pfCallback != NULL) {
                pDevice->pfCallback(pDevice->pCBParam, ADI_ADC_EVENT_ADC_READY, NULL);
            }
        }
        if ((pDevice->pReg->STAT & BITM_ADC_STAT_CALDONE) != 0u) {
            SET_STATE(ADC_STATUS_CALIBRATION_DONE);
            pDevice->pReg->STAT = BITM_ADC_STAT_CALDONE;
            if (pDevice->pfCallback != NULL) {
                pDevice->pfCallback(pDevice->pCBParam, ADI_ADC_EVENT_CALIBRATION_DONE, NULL);
            }
        }
        if ((pDevice->pReg->STAT & BITM_ADC_STAT_BATDONE) != 0u) {
            SET_STATE(ADC_STATUS_BATTERY_DONE);
            pDevice->pReg->STAT = BITM_ADC_STAT_BATDONE;
        }	
        
        if ((pDevice->pReg->STAT & BITM_ADC_STAT_TMPDONE) != 0u) {
            SET_STATE(ADC_STATUS_TMP_DONE);
            pDevice->pReg->STAT = BITM_ADC_STAT_TMPDONE;
        }	
        
        if ((pDevice->pReg->STAT & BITM_ADC_STAT_TMP2DONE) != 0u) {
            SET_STATE(ADC_STATUS_TMP2_DONE);
            pDevice->pReg->STAT = BITM_ADC_STAT_TMP2DONE;
        }	
    }
    if (pDevice->pReg->OVF) {
        uint16_t nOvrFlowValue = pDevice->pReg->OVF;
        if (IS_NOT_IN_STATE(ADC_STATUS_COMPARATOR_EN)) {
            pDevice->pfManageFifo(pDevice, ADC_FIFO_MODE_INTERRUPT_OVERFLOW);
        }
        pDevice->pReg->OVF = nOvrFlowValue;
    }
    if (pDevice->pReg->ALERT) {
        uint32_t nAlertValue = pDevice->pReg->ALERT;
        uint32_t x;
        if (IS_IN_STATE(ADC_STATUS_COMPARATOR_EN) && (pDevice->pfCallback != NULL)) {
            for (x = 0u; x < NUM_ADC_COMPARATOR_CHANNELS; x++) {
                if ((pDevice->nCompConfig & ((uint32_t)1u << (BITS_ADC_COMP_CFG_CH * x + BITS_ADC_COMP_CFG_EN))) != 0u) {
                    uint32_t nArg = (uint32_t)1u << x;
                    if ((nAlertValue & ((uint32_t)1u << (x*2u+0u))) != 0u) {
                        pDevice->pfCallback(pDevice->pCBParam, ADI_ADC_EVENT_HIGH_LIMIT_CROSSED, (void*)nArg);
                    }
                    if ((nAlertValue & ((uint32_t)1u << (x*2u+1u))) != 0u) {
                        pDevice->pfCallback(pDevice->pCBParam, ADI_ADC_EVENT_LOW_LIMIT_CROSSED, (void*)nArg);
                    }
                }
            }
        }
        pDevice->pReg->ALERT = (uint16_t)nAlertValue;
    }
    ISR_EPILOG();     
}



/*=============  B U F F E R   F U N C T I O N S =============*/
/* Pop the buffer from the front of the buffer list */
static ADC_INT_BUFFER* fpopIntBuffer(ADC_INT_BUFFER** ppBufferList)
{
    ADC_INT_BUFFER* pRetBuffer = NULL;
    ADC_INT_BUFFER* pBufferList;

    ADI_ENTER_CRITICAL_REGION();

    pBufferList = *ppBufferList;

    if (pBufferList != NULL) {
        pRetBuffer = pBufferList;
        *ppBufferList = pBufferList->pNext;
        pRetBuffer->pNext = NULL;
    }

    ADI_EXIT_CRITICAL_REGION();

    return pRetBuffer;
}

/* Push the buffer to the front of the buffer list */
static void fpushIntBuffer(ADC_INT_BUFFER** ppBufferList, ADC_INT_BUFFER* pBuffer)
{
    ADI_ENTER_CRITICAL_REGION();

    pBuffer->pNext = *ppBufferList;
    *ppBufferList = pBuffer;

    ADI_EXIT_CRITICAL_REGION();
}

static void bpushIntBuffer_Crit(ADC_INT_BUFFER** ppBufferList, ADC_INT_BUFFER* pBuffer)            
{
    ADC_INT_BUFFER* pBufferList;

    pBufferList = *ppBufferList;

    if (pBufferList == NULL) {
        *ppBufferList = pBuffer;
    } else {
        while (pBufferList->pNext != NULL) {
            pBufferList = pBufferList->pNext;
        }
        pBufferList->pNext = pBuffer;
    }
}

/* Push the buffer to the back of the buffer list */
static void bpushIntBuffer(ADC_INT_BUFFER** ppBufferList, ADC_INT_BUFFER* pBuffer)
{
    ADI_ENTER_CRITICAL_REGION();
    bpushIntBuffer_Crit(ppBufferList, pBuffer);
    ADI_EXIT_CRITICAL_REGION();
}

/*=============  D E B U G   F U N C T I O N S =============*/

/* IF (Debug information enabled) */
#ifdef ADI_DEBUG

/* Validates ADC device handle */
static ADI_ADC_RESULT ValidateAdcHandle (ADI_ADC_HANDLE const hDevice)
{
    uint32_t i;

    for (i = 0u; i < (sizeof (AdcDevInfo) / sizeof (AdcDevInfo[0])); i++)
    {
        if (hDevice == AdcDevInfo[i].hDevice)
        {
            return (ADI_ADC_SUCCESS);
        }
    }

    return (ADI_ADC_FAILURE);
}

#endif /* ADI_DEBUG */

/*! \endcond */

#endif /* _ADI_ADC_V1_C_ */

/*****/

/*@}*/
