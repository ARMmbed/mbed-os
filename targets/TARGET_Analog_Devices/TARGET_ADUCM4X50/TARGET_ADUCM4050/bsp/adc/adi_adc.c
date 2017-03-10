/*! *****************************************************************************
 * @file:    adi_adc.c
 * @brief:   ADC device driver global file.
 * @details: This file contain the ADC device driver implementation.
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
/** @addtogroup  ADC_Driver ADC Driver
 *  @{
 *  @brief ADC Driver
 *  @details The ADC driver manages all instances of the ADC peripheral.
 *  @note - The application must include drivers/adc/adi_adc.h to use this driver.
 *  @note - This driver also requires the DMA driver.  The application must include
            the DMA driver sources to avoid link errors.    
 */

#ifndef ADI_ADC_C
/*! \cond PRIVATE */
#define ADI_ADC_C

/*=============  I N C L U D E S   =============*/


/* Header file with definitions specific to ADC driver implementation */

/*=============  A D C    I M P L E M E N T A T I O N    S O U R C E    F I L E S  =============*/
#include <drivers/adc/adi_adc.h>
#include <adi_processor.h>
#include <assert.h>
#include <string.h>
#include <drivers/pwr/adi_pwr.h>
#include <adi_adc_config.h>
#include <rtos_map/adi_rtos_map.h>

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
* Pm140 (rule 11.3): a cast should not be performed between a pointer type and an integral type
*   The rule makes an exception for memory-mapped register accesses.
*
* Pm152: (MISRA C 2004 rule 17.4) array indexing shall only be applied to objects defined as an array type
*   Accessing the DMA descriptors, which are defined in the system as a pointer to an array of descriptors

*/
#pragma diag_suppress=Pm123,Pm073,Pm143,Pm050,Pm088,Pm140,Pm152
#endif /* __ICCARM__ */

#include "adi_adc_def.h"
#include "adi_adc_data.c"

/*==============  D E F I N E S  ===============*/
#ifdef ADI_DEBUG
#define ADI_ADC_INVALID_HANDLE(h)   (AdcDevInfo[0].hDevice != (h))
#endif

/* Specify the maximum acquisition time, based on the width of the SAMPTIME field. */
#define ADI_MAX_ACQUISITION_TIME (((uint32_t)BITM_ADC_CNV_TIME_SAMPTIME << BITP_ADC_CNV_TIME_SAMPTIME) + 1u)

/* The 12bit maximum sample value */
#define ADI_ADC_SAMPLE_MAX ((uint16_t)(4095u))

/*=============  C O D E  =============*/

/*=============  D E B U G    F U N C T I O N    P R O T O T Y P E S  =============*/

/* Override "weak" default binding in startup_*.c */
/*! \cond PRIVATE */
extern void ADC0_Int_Handler(void);
extern void DMA_ADC0_Int_Handler (void);
                                  
/*! \endcond */

/* Prototypes for static functions (required by MISRA-C:2004 Rule 8.1)  */
/*=============  L O C A L   F U N C T I O N S   P R O T O T Y P E S =============*/
static uint16_t ReadOutReg(uint32_t nChannelNum);

/* ADC management functions, based on transfer method */
#if ADI_ADC_ENABLE_MULTI_ACQUIRE == 1
static ADI_ADC_RESULT DmaFIFOManage (ADI_ADC_DEVICE *pDevice, ADC_FIFO_MODE eFifoMode);
#else
static ADI_ADC_RESULT InterruptFIFOManage (ADI_ADC_DEVICE *pDevice, ADC_FIFO_MODE eFifoMode);
#endif

/* Channel helper functions */
static uint32_t GetNumChannels(uint32_t nChannels);
static int32_t  nGetChannelNumber(ADI_ADC_CHANNEL eChannel);

/* Buffer management functions */
static void ManageFifoCompletion(ADI_ADC_DEVICE *pDevice);
static bool InitBufferProcessing(ADI_ADC_DEVICE *pDevice);
static void FlushFifo(ADI_ADC_DEVICE *pDevice, uint32_t nChannels);

/* Internal configuration functions */
static void EnableComparator(ADI_ADC_DEVICE *pDevice, bool bEnable);
static void StaticConfiguration(ADI_ADC_DEVICE *pDevice);

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
    ADI_INT_STATUS_ALLOC();
    ADI_ADC_DEVICE *pDevice = (ADI_ADC_DEVICE *)pMemory;
    
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
    
    /* Reset the ADC */
    pDevice->pReg->CFG = BITM_ADC_CFG_RST;
    
    /* Enable the IRQs */ 
    NVIC_ClearPendingIRQ(ADC0_EVT_IRQn);
    NVIC_EnableIRQ(ADC0_EVT_IRQn);
    
    /* Initialize the registers to known value */
    pDevice->pReg->IRQ_EN =   BITM_ADC_IRQ_EN_RDY | BITM_ADC_IRQ_EN_ALERT | BITM_ADC_IRQ_EN_OVF | BITM_ADC_IRQ_EN_CALDONE | BITM_ADC_IRQ_EN_CNVDONE;
    
    /* Do the static configuration */
    StaticConfiguration(pDevice);

    /* Create a semaphore for buffer management */
    SEM_CREATE(pDevice, "ADC Sem", ADI_ADC_ERR_RTOS);

    /* Set the default FIFO Manage function */
#if ADI_ADC_ENABLE_MULTI_ACQUIRE == 1
    pDevice->pfManageFifo = DmaFIFOManage;
    /* Make sure the DMA controller and its SRAM based descriptors are initialized */
    adi_dma_Init();
#else    
    pDevice->pfManageFifo = InterruptFIFOManage;
#endif 

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
    ADI_ADC_DEVICE *pDevice = (ADI_ADC_DEVICE *)hDevice;
    ADI_ADC_RESULT eResult;

#ifdef ADI_DEBUG
    if (pDevice == NULL)
    {
        return ADI_ADC_INVALID_DEVICE_HANDLE;
    }
#endif /* ADI_DEBUG */	

    /* Power down the device */
    if ((eResult = adi_adc_PowerUp (hDevice, false)) != ADI_ADC_SUCCESS) {
        return eResult;
    }

      /* Disable the IRQ */
      pDevice->pReg->IRQ_EN = 0u;

      /* Clear the conversion cfg register to stop any transaction */
      pDevice->pReg->CNV_CFG = 0u;

#if ADI_ADC_ENABLE_MULTI_ACQUIRE == 1
      /* Close the DMA if configured */
      NVIC_DisableIRQ(DMA0_CH24_DONE_IRQn);
#endif /* ADI_ADC_ENABLE_MULTI_ACQUIRE == 1 */
        
    /* Disable the ADC interrupt */
    NVIC_DisableIRQ(ADC0_EVT_IRQn);
    
    /* Destroy the semaphore */
    SEM_DELETE(pDevice, ADI_ADC_ERR_RTOS);

    /* Finally, zero the device */
    AdcDevInfo[0].hDevice = (NULL);

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
 *           - #ADI_ADC_BAD_SYS_CLOCK               Unable to obtain PCLK which is needed to calculate
 *                                                  powerup values.
 *           - #ADI_ADC_INVALID_DEVICE_HANDLE   [D] Invalid device handle passed to the function
 */
ADI_ADC_RESULT adi_adc_PowerUp (ADI_ADC_HANDLE hDevice, bool bPowerUp)
{
    ADI_ADC_DEVICE *pDevice = (ADI_ADC_DEVICE *)hDevice;
    uint32_t nClock = 0u;
    uint16_t nCount = 0u;
    ADI_ADC_RESULT eResult = ADI_ADC_SUCCESS;
    
#ifdef ADI_DEBUG
    if (pDevice == NULL)
    {
        return ADI_ADC_INVALID_DEVICE_HANDLE;
    }
#endif /* ADI_DEBUG */

    if (bPowerUp == true)
    {
        if (IS_NOT_IN_ANY_STATE(ADC_STATUS_POWERED_UP))
        {
            if(adi_pwr_GetClockFrequency(ADI_CLOCK_PCLK, &nClock) == ADI_PWR_SUCCESS)
            {
              /* We need the cycles equivelent of 20us entered here, based on the PCLK
               * clock. nClock is the frequency of the PCLK, 50000 is the equivalent frequency of 20us
               * e.g. 26,000,000Hz, 0.00002s produces 520 cycles.*/
              nCount =  (uint16_t)(nClock / 50000u); 
              
              /* Powering up ADC */
              pDevice->pReg->CFG |= BITM_ADC_CFG_PWRUP;

              /* Set ADC_PWRUP.WAIT bits for the new count */
              pDevice->pReg->PWRUP = (uint16_t)(((uint32_t)nCount << BITP_ADC_PWRUP_WAIT) &  BITM_ADC_PWRUP_WAIT);
              
              SET_STATE(ADC_STATUS_POWERED_UP);
            }
            else
            {
              eResult = ADI_ADC_BAD_SYS_CLOCK;
            }
        }
    }
    else
    {
        if (IS_IN_STATE(ADC_STATUS_POWERED_UP))
        {
            /* If the ADC system is up then disable the ADC subsystem */
            if (  IS_IN_STATE(ADC_STATUS_SUB_SYSTEM_EN) )
            {
                eResult = adi_adc_EnableADCSubSystem (hDevice, false);
                if (eResult != ADI_ADC_SUCCESS)
                {
                    return eResult;
                }                
            }
            
            /* Powering down ADC */
            pDevice->pReg->CFG &= (uint16_t)(~(BITM_ADC_CFG_PWRUP));
            CLR_STATE(ADC_STATUS_POWERED_UP);
        }
    }

    return eResult;
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
 *           the given computation is over. It will also be called when the digital comparitor is being
 *           used and a limit has been broken.
 *
 * @return   Status
 *           - #ADI_ADC_SUCCESS              Successfully registerd the callback
 *           - #ADI_ADC_INVALID_SEQUENCE      [D] Callback cannot be registered when ADC is enabled for sampling.
 *           - #ADI_ADC_INVALID_DEVICE_HANDLE [D] Invalid device handle passed to the function
 */            
ADI_ADC_RESULT adi_adc_RegisterCallback (
    ADI_ADC_HANDLE hDevice,
    ADI_CALLBACK   pfCallback,
    void           *pCBParam)
{
    ADI_INT_STATUS_ALLOC();
    
    ADI_ADC_DEVICE *pDevice = (ADI_ADC_DEVICE *)hDevice;

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

    ADI_ENTER_CRITICAL_REGION();
    pDevice->pfCallback = pfCallback;
    pDevice->pCBParam = pCBParam;
    ADI_EXIT_CRITICAL_REGION();
    
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
 *           for sampling the signal. The driver should check whether the ADC is ready by calling adi_adc_IsReady
 *           API before continuing. If internal reference buffer is used as voltage reference then application
 *           has to wait at least 3.5ms after enabling irrespective of whether adi_adc_IsReady returns ready or not.
 *
 * @return   Status
 *           - #ADI_ADC_SUCCESS              Successfully enabled/disabled the ADC subsystem
 *           - #ADI_ADC_INVALID_SEQUENCE     [D] Can only be called if the ADC is powered up, 
 *                                               and cannot be disabled when sampling or using
 *                                               the camparator.
 *           - #ADI_ADC_INVALID_DEVICE_HANDLE [D] Invalid device handle passed to the function
 */
ADI_ADC_RESULT adi_adc_EnableADCSubSystem (
    ADI_ADC_HANDLE hDevice,
    bool         bEnable)
{
    ADI_ADC_DEVICE *pDevice = (ADI_ADC_DEVICE *)hDevice;

#ifdef ADI_DEBUG
    if (pDevice == NULL)
    {
        return ADI_ADC_INVALID_DEVICE_HANDLE;
    }
    
    if (IS_NOT_IN_STATE(ADC_STATUS_POWERED_UP))
    {
        return ADI_ADC_INVALID_SEQUENCE;
    }
    
    if (bEnable == true) {
        if (IS_IN_STATE(ADC_STATUS_SUB_SYSTEM_EN)) {
            return ADI_ADC_INVALID_SEQUENCE;
        }
    } else {
        if (IS_IN_ANY_STATE(ADC_STATUS_NON_BLOCKING_EN |ADC_STATUS_BLOCKING_EN | ADC_STATUS_COMPARATOR_EN)) {
            return ADI_ADC_INVALID_SEQUENCE;
        }
    }
#endif /* ADI_DEBUG */

    if (bEnable == true)
    {
        pDevice->pReg->CFG |= BITM_ADC_CFG_EN;
        SET_STATE(ADC_STATUS_SUB_SYSTEM_EN);
    }
    else
    {
        pDevice->pReg->CFG &= (uint16_t)(~BITM_ADC_CFG_EN);
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
  *           - #ADI_ADC_INVALID_SEQUENCE     [D] Cannot be called if the subsystem is not enabled.
  *           - #ADI_ADC_INVALID_DEVICE_HANDLE [D] Invalid device handle passed to the function
  *           - #ADI_ADC_NULL_POINTER            [D] pbReady is NULL
  */
               
ADI_ADC_RESULT adi_adc_IsReady (
    ADI_ADC_HANDLE  hDevice,
    bool         *pbReady
)
{
    ADI_ADC_DEVICE *pDevice = (ADI_ADC_DEVICE *)hDevice;

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
    ADI_ADC_RESULT eResult  = ADI_ADC_SUCCESS;
    ADI_ADC_DEVICE *pDevice = (ADI_ADC_DEVICE *)hDevice;

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

    pDevice->pReg->CFG &= (uint16_t)(~(BITM_ADC_CFG_REFBUFEN | BITM_ADC_CFG_VREFSEL | BITM_ADC_CFG_VREFVBAT));

    switch (eVrefSrc)
    {
    case ADI_ADC_VREF_SRC_INT_1_25_V:
        pDevice->pReg->CFG |= BITM_ADC_CFG_REFBUFEN | BITM_ADC_CFG_VREFSEL;
        break;

    case ADI_ADC_VREF_SRC_INT_2_50_V:
        pDevice->pReg->CFG |= BITM_ADC_CFG_REFBUFEN;
        break;
        
    case ADI_ADC_VREF_SRC_VBAT:
        pDevice->pReg->CFG |= BITM_ADC_CFG_VREFVBAT;
        break;        

    case ADI_ADC_VREF_SRC_EXT:
        break;

    default:
        eResult = ADI_ADC_INVALID_PARAMETER;
        break;
    }

    return eResult;
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
    bool         bEnable)
{
    ADI_ADC_DEVICE *pDevice = (ADI_ADC_DEVICE *)hDevice;

#ifdef ADI_DEBUG
    if (pDevice == NULL)
    {
        return ADI_ADC_INVALID_DEVICE_HANDLE;
    }
#endif /* ADI_DEBUG */

    if (bEnable == true)
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
    ADI_ADC_DEVICE *pDevice = (ADI_ADC_DEVICE *)hDevice;

#ifdef ADI_DEBUG
    if (pDevice == NULL)
    {
        return ADI_ADC_INVALID_DEVICE_HANDLE;
    }

    /* Calibration cannot be done when ADC is processing the buffers */
    if (IS_IN_ANY_STATE(ADC_STATUS_NON_BLOCKING_EN |ADC_STATUS_BLOCKING_EN)) 
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
    bool         *pbCalibrationDone)
{
    ADI_ADC_DEVICE *pDevice = (ADI_ADC_DEVICE *)hDevice;

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
 * @param [in] nAcqTimeInAClkCycles  Acquisition time in ADC clock cycles.
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
    ADI_ADC_DEVICE *pDevice = (ADI_ADC_DEVICE *)hDevice;
    uint16_t nCnvTime;

#ifdef ADI_DEBUG
    if (pDevice == NULL)
    {
        return ADI_ADC_INVALID_DEVICE_HANDLE;
    }

    if (IS_IN_ANY_STATE(ADC_STATUS_NON_BLOCKING_EN |ADC_STATUS_BLOCKING_EN | ADC_STATUS_COMPARATOR_EN))
    {
        return ADI_ADC_INVALID_SEQUENCE;
    }
 
    /* A valid range is 1u to the width of the SAMPTIME field + 1. */
    if ((nAcqTimeInAClkCycles == 0u) || (nAcqTimeInAClkCycles > (ADI_MAX_ACQUISITION_TIME)))
    {
        return ADI_ADC_INVALID_PARAMETER;
    }

#endif /* ADI_DEBUG */

    /* Acquisition phase is (ADC_CNV_TIME.SAMPTIME + 1) ACLK cycles */
    nCnvTime = pDevice->pReg->CNV_TIME;
    nCnvTime &= (uint16_t)(~BITM_ADC_CNV_TIME_SAMPTIME);
    nCnvTime |= (uint16_t)((nAcqTimeInAClkCycles - ((uint32_t)1u)) << BITP_ADC_CNV_TIME_SAMPTIME);
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
    ADI_ADC_DEVICE *pDevice = (ADI_ADC_DEVICE *)hDevice;
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
    nCnvTime &= (uint16_t)(~BITM_ADC_CNV_TIME_DLY);
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
    ADI_ADC_DEVICE *pDevice = (ADI_ADC_DEVICE *)hDevice;
    uint16_t nFactor;

#ifdef ADI_DEBUG
    if (pDevice == NULL)
    {
        return ADI_ADC_INVALID_DEVICE_HANDLE;
    }
    if (IS_IN_ANY_STATE(ADC_STATUS_NON_BLOCKING_EN | ADC_STATUS_BLOCKING_EN | ADC_STATUS_COMPARATOR_EN)) 
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
        pDevice->pReg->AVG_CFG &= (uint16_t)(~BITM_ADC_AVG_CFG_OS);
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
        nFactor = (uint16_t)1u << (((uint16_t)eResolution * 2u) - ((uint16_t)1u));
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
    ADI_ADC_DEVICE *pDevice = (ADI_ADC_DEVICE *)hDevice;
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
        pDevice->pReg->AVG_CFG &= (uint16_t)(~BITM_ADC_AVG_CFG_EN);
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
 * @brief    Configure low limit for an ADC channel when it is used as a digital comparator.
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
    bool             bEnable,
    uint16_t         nLowLimit
    )
{
    ADI_ADC_DEVICE *pDevice = (ADI_ADC_DEVICE *)hDevice;
    static volatile uint16_t* pRegister[4] = {
        pREG_ADC0_LIM0_LO, pREG_ADC0_LIM1_LO, pREG_ADC0_LIM2_LO, pREG_ADC0_LIM3_LO
    };
    int32_t nChannelNum = 0;
    
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
        
    nChannelNum = nGetChannelNumber(eChannel);
    
    if((nChannelNum >= 0) && (nChannelNum <= 3)) {
      if (bEnable == true) {
        
        *pRegister[nChannelNum] = (uint16_t)(*pRegister[nChannelNum] & (uint16_t)(~BITM_ADC_LIM0_LO_VALUE)) | 
          (uint16_t)(nLowLimit << BITP_ADC_LIM0_LO_VALUE);
        
        /* Now enable this channel comparitor - unused until the comparitor is enabled */   
        pDevice->ComparitorLo |= (1u << nChannelNum);
      }
      else {
        pDevice->ComparitorLo &= ~(1u << nChannelNum);
      }
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
    bool             bEnable,
    uint16_t         nHighLimit)
{
    ADI_ADC_DEVICE *pDevice = (ADI_ADC_DEVICE *)hDevice;
    static volatile uint16_t* pRegister[4] = {
        pREG_ADC0_LIM0_HI, pREG_ADC0_LIM1_HI, pREG_ADC0_LIM2_HI, pREG_ADC0_LIM3_HI
    };
    int32_t nChannelNum = 0;
    
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
        
    nChannelNum = nGetChannelNumber(eChannel);
    
    if((nChannelNum >= 0) && (nChannelNum <= 3)) {
      if (bEnable == true) {
        /* Set the given high value - only relevant if the limit is enabled. */
        *pRegister[nChannelNum] =    (uint16_t)(*pRegister[nChannelNum] & (uint16_t)(~BITM_ADC_LIM0_HI_VALUE))
          | (uint16_t)(nHighLimit << BITP_ADC_LIM0_HI_VALUE);
        
        /* Now enable this channel comparitor - unused until the comparitor is enabled */   
        pDevice->ComparitorHi |= (1u << nChannelNum);
      }
      else {
        pDevice->ComparitorHi &= ~(1u << nChannelNum);
      }
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
    bool             bEnable,
    uint16_t         nHysteresis)
{
    ADI_ADC_DEVICE *pDevice = (ADI_ADC_DEVICE *)hDevice;
    static volatile uint16_t* pRegister[4] = {
        pREG_ADC0_HYS0, pREG_ADC0_HYS1, pREG_ADC0_HYS2, pREG_ADC0_HYS3
    };
    int32_t nChannelNum = 0;
    
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
        
    nChannelNum = nGetChannelNumber(eChannel);
    
    if((nChannelNum >= 0) && (nChannelNum <= 3)) {
        if (bEnable == true) {
          *pRegister[nChannelNum] = (uint16_t)(*pRegister[nChannelNum] & (uint16_t)(~BITM_ADC_HYS0_VALUE))
                                  | (uint16_t)(nHysteresis << BITP_ADC_HYS0_VALUE);
          
          /* Now enable this channel hysteresis - unused until the comparitor is enabled */   
          pDevice->ComparitorHys |= (1u << nChannelNum);
        }
        else {
          pDevice->ComparitorHys &= ~(1u << nChannelNum);
        }
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
 #ifdef ADI_DEBUG
    ADI_ADC_DEVICE *pDevice = (ADI_ADC_DEVICE *)hDevice;
#endif /* ADI_DEBUG */
    
    static volatile uint16_t* pRegister[4] = {
        pREG_ADC0_HYS0, pREG_ADC0_HYS1, pREG_ADC0_HYS2, pREG_ADC0_HYS3
    };
    int32_t nChannelNum = 0;
    
#ifdef ADI_DEBUG
    if (pDevice == NULL)
    {
        return ADI_ADC_INVALID_DEVICE_HANDLE;
    }
    if ((nNumMonitorCycles > (BITM_ADC_HYS0_MONCYC >> BITP_ADC_HYS0_MONCYC)) || (nChannelNum < 0) || (nChannelNum > 3))
    {
        return ADI_ADC_INVALID_PARAMETER;
    }
#endif /* ADI_DEBUG */
    
    nChannelNum = nGetChannelNumber(eChannel);
      
    if((nChannelNum >= 0) && (nChannelNum <= 3)) {
      *pRegister[nChannelNum] = (uint16_t)(*pRegister[nChannelNum] & (uint16_t)(~BITM_ADC_HYS0_MONCYC))
        | (uint16_t)(nNumMonitorCycles << BITP_ADC_HYS0_MONCYC);
    }
    return ADI_ADC_SUCCESS;
}



/**
 * @brief    Enable/Disable digital comparator for the given channel(s)
 *
 * @param [in] hDevice              Handle to the device instance
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
    bool         bEnableComparator
)
{
    ADI_ADC_DEVICE *pDevice = (ADI_ADC_DEVICE *)hDevice;
#ifdef ADI_DEBUG
    if (pDevice == NULL)
    {
        return ADI_ADC_INVALID_DEVICE_HANDLE;
    }
    
    if (IS_IN_ANY_STATE(ADC_STATUS_NON_BLOCKING_EN |ADC_STATUS_BLOCKING_EN)) 
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
    
    if (bEnableComparator == true) {
      if((pDevice->ComparitorHi | pDevice->ComparitorLo) == 0u) {
        return ADI_ADC_INVALID_STATE;
      }
    }
#endif /* ADI_DEBUG */
    
    EnableComparator(pDevice, bEnableComparator);
    
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
    ADI_ADC_DEVICE *pDevice = (ADI_ADC_DEVICE *)hDevice;
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

    pIntBuffer = &pDevice->s_Buffer;

    pIntBuffer->nConfig = ADC_BUFFER_CONFIG_BUFFER_AUTO_MODE_EN;
    pIntBuffer->nStatus = ADC_BUFFER_STATUS_OK;
    if (pBuffer->nNumConversionPasses == 1u) 
    {
        pIntBuffer->nConfig |= ADC_BUFFER_CONFIG_BUFFER_SINGLE_CONV_EN;
    }
    pIntBuffer->pUserBuffer = pBuffer;
    pIntBuffer->pCurDataBuffer = pBuffer->pDataBuffer;
    pIntBuffer->nNumSamplesRemaining = nNumChannels * pBuffer->nNumConversionPasses;
    pIntBuffer->nChannels = pBuffer->nChannels;
    
    pDevice->pfManageFifo(pDevice, ADC_FIFO_MODE_INIT);
    
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
    ADI_ADC_DEVICE *pDevice = (ADI_ADC_DEVICE *)hDevice;
    ADI_ADC_RESULT eADCresult = ADI_ADC_SUCCESS;
    
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

    if (pDevice->s_Buffer.pUserBuffer == NULL) {
      return ADI_ADC_INVALID_STATE;
    }
    
    /* Wait for read completion */
    SEM_PEND(pDevice, ADI_ADC_ERR_RTOS);
    
    if ((uint16_t)(pDevice->s_Buffer.nStatus & ADC_BUFFER_STATUS_OVERFLOW) != 0u) {
        eADCresult =  ADI_ADC_BUFFER_OVERFLOW;
    }
    *ppBuffer = pDevice->s_Buffer.pUserBuffer;
    pDevice->s_Buffer.pUserBuffer = NULL;
    CLR_STATE(ADC_STATUS_NON_BLOCKING_EN);
    
    return eADCresult;
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
    bool         bEnable)
{
    ADI_ADC_DEVICE *pDevice = (ADI_ADC_DEVICE *)hDevice;
    
#ifdef ADI_DEBUG
    if (pDevice == NULL)
    {
        return ADI_ADC_INVALID_DEVICE_HANDLE;
    }
    
    if (IS_IN_ANY_STATE(ADC_STATUS_BLOCKING_EN | ADC_STATUS_COMPARATOR_EN)) {
        return ADI_ADC_INVALID_STATE;
    }
#endif /* ADI_DEBUG */

    if (bEnable == true) {
        /* Set the driver to be in non-blocking mode */
        SET_STATE(ADC_STATUS_NON_BLOCKING_EN);
        
        /* Enable the IRQs */
        NVIC_EnableIRQ(ADC0_EVT_IRQn);
   
        /* Try to submit possible number of buffers */
        InitBufferProcessing(pDevice);
    } else {
        /* Disble the IRQs */
        NVIC_DisableIRQ(ADC0_EVT_IRQn);
        
        /* Abort any transaction if present */
        pDevice->pfManageFifo(pDevice, ADC_FIFO_MODE_ABORT);
        
        CLR_STATE(ADC_STATUS_NON_BLOCKING_EN);
    }

    return ADI_ADC_SUCCESS;    
}


/**
 * @brief    This function return whether a filled buffer is available to be returned to the user.
 *           If this function return true, then a call to adi_adc_GetBuffer will not block
 *
 * @param [in]  hDevice               Handle to the device instance.
 * @param [out] pbIsBufferAvailable   Pointer to a bool variable to which the availability of buffer will be written.
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
    bool         *pbIsBufferAvailable)
{
    ADI_ADC_DEVICE *pDevice = (ADI_ADC_DEVICE *)hDevice;
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

    if(IS_IN_STATE(ADC_STATUS_SAMPLING_IN_PROGRESS))  
    {
        *pbIsBufferAvailable = false;
    }
    else
    {
        *pbIsBufferAvailable = true;
    }

    return ADI_ADC_SUCCESS;    
}


/**
 * @brief   Sample the given channels for the given number of conversion passes and put it into the given buffer. This function only return after
 *          the channels are sampled the given number of conversion times or if any error occurs. 
 *
 * @param [in] hDevice      Handle to the device instance
 *
 * @param [in] nChannels    Channels to sample. Should be an ORed value of ADI_ADC_CHANNEL types.
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
    ADI_ADC_DEVICE *pDevice = (ADI_ADC_DEVICE *)hDevice;
    uint32_t nNumChannels = 0u;
    ADI_ADC_RESULT eADCresult = ADI_ADC_SUCCESS;

    ADC_INT_BUFFER* pIntBuffer;

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

    if (nBuffLength < ((nNumChannels * sizeof(uint16_t)) * nNumConversionPasses))
    {
        return ADI_ADC_INSUFFICIENT_MEMORY;
    }
    
    /* Clear ADC status */
    pDevice->pReg->STAT = 0xFFFFu;

    /* Set the driver to be in blocking mode */
    SET_STATE(ADC_STATUS_BLOCKING_EN);

    /* Get the buffer */
    pIntBuffer = &pDevice->s_Buffer;    

    pIntBuffer->nConfig = ADC_BUFFER_CONFIG_BUFFER_AUTO_MODE_EN;
    if (nNumConversionPasses == 1u) {
        pIntBuffer->nConfig |= ADC_BUFFER_CONFIG_BUFFER_SINGLE_CONV_EN;
    }
    
    pIntBuffer->nStatus = ADC_BUFFER_STATUS_OK;
    pIntBuffer->pUserBuffer = NULL;
    pIntBuffer->pCurDataBuffer = pBuffer;
    pIntBuffer->nNumSamplesRemaining = nNumChannels * nNumConversionPasses;
    pIntBuffer->nChannels = nChannels;
    
    pDevice->pfManageFifo(pDevice, ADC_FIFO_MODE_INIT);
    
    InitBufferProcessing(pDevice);

    /* Wait for read completion */
    SEM_PEND(pDevice, ADI_ADC_ERR_RTOS);  
    
    if ((uint16_t)(pDevice->s_Buffer.nStatus & ADC_BUFFER_STATUS_OVERFLOW) != 0u) {
        eADCresult =  ADI_ADC_BUFFER_OVERFLOW;
    }

    /* Driver is no longer in blocking mode */
    CLR_STATE(ADC_STATUS_BLOCKING_EN);

    /* Enable the IRQs */
    NVIC_DisableIRQ(ADC0_EVT_IRQn);

    return eADCresult;
}


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
 *           - #ADI_ADC_BAD_SYS_CLOCK               Unable to obtain CLK which is needed to calculate
 *                                                  voltage conversion timing values.
 *           - #ADI_ADC_INVALID_DEVICE_HANDLE   [D] Invalid device handle passed to the API 
 *           - #ADI_ADC_NULL_POINTER            [D] pnBatVoltage is NULL
 *           - #ADI_ADC_INVALID_SEQUENCE        [D] ADC sub system should be up and ADC should be free for getting the battery voltage.
 */
ADI_ADC_RESULT adi_adc_GetBatteryVoltage (
    ADI_ADC_HANDLE hDevice,
    uint32_t       nRefVoltage,
    uint32_t       *pnBatVoltage)
{
    ADI_ADC_RESULT eResult = ADI_ADC_SUCCESS;
    ADI_ADC_DEVICE *pDevice = (ADI_ADC_DEVICE *)hDevice;
    uint16_t nConvTimeBackup;
    uint16_t nAvgCfgBackup;
    uint32_t nAdcValue = 0u;
    uint32_t nClock = 0u;  
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
    
    if (IS_IN_ANY_STATE(ADC_STATUS_NON_BLOCKING_EN |ADC_STATUS_BLOCKING_EN | ADC_STATUS_COMPARATOR_EN)) {
        return ADI_ADC_INVALID_SEQUENCE;
    }    
#endif /* ADI_DEBUG */

    if(adi_pwr_GetClockFrequency(ADI_CLOCK_PCLK, &nClock) == ADI_PWR_SUCCESS)
    {
        /* Take the backup of registers that need to be changed */
        nConvTimeBackup = pDevice->pReg->CNV_TIME;
        nAvgCfgBackup =  pDevice->pReg->AVG_CFG;
        
        /* Set the required value in the registers. */
        nACLKDIVCNT = (*pREG_CLKG0_CLK_CTL1 & BITM_CLKG_CLK_CTL1_ACLKDIVCNT) >> BITP_CLKG_CLK_CTL1_ACLKDIVCNT;
        
        /* Calculate the number of cycles required for conversion. 
         * The conversion time required is 500ns = 2000000Hz 
         */
        nClock = nClock/nACLKDIVCNT;  /* nClock = ACLK frequency Hz */
        pDevice->pReg->CNV_TIME = (uint16_t)((nClock/2000000u) + ((uint16_t)1u)); 
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
        
        /* From HRM: converting ADC result to battery voltage, following calculations should be done: 
         * VBAT = 4 * (adc_out) * Vref / (2^12 - 1) */
         nAdcValue = pDevice->pReg->BAT_OUT;
        *pnBatVoltage = (4u * nAdcValue * nRefVoltage) / ADI_ADC_SAMPLE_MAX;
    }
    else
    {
         eResult = ADI_ADC_BAD_SYS_CLOCK;
    }
        
    return eResult;
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
    bool         bEnable)
{
    ADI_ADC_DEVICE *pDevice = (ADI_ADC_DEVICE *)hDevice;

#ifdef ADI_DEBUG
    if (pDevice == NULL)
    {
        return ADI_ADC_INVALID_DEVICE_HANDLE;
    }
#endif /* ADI_DEBUG */

    if (bEnable == true)
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
 *           - #ADI_ADC_BAD_SYS_CLOCK               Unable to obtain CLK which is needed to calculate
 *                                                  temperature conversion timing values.
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
    ADI_ADC_RESULT eResult = ADI_ADC_SUCCESS;
    ADI_ADC_DEVICE *pDevice = (ADI_ADC_DEVICE *)hDevice;
    uint16_t nConvTimeBackup;
    uint16_t nAvgCfgBackup;
    uint32_t nAdcTmpValue = 0u;
    uint32_t nAdcTmp2Value = 0u;
    uint32_t nClock = 0u;  
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
        
    if (IS_IN_ANY_STATE(ADC_STATUS_NON_BLOCKING_EN | ADC_STATUS_BLOCKING_EN | ADC_STATUS_COMPARATOR_EN))
    {
        return ADI_ADC_INVALID_SEQUENCE;
    }    
#endif    
    
    
    if(adi_pwr_GetClockFrequency(ADI_CLOCK_PCLK, &nClock) == ADI_PWR_SUCCESS)
    {    
      /* Calculate the conversion time */
      nACLKDIVCNT = (*pREG_CLKG0_CLK_CTL1 & BITM_CLKG_CLK_CTL1_ACLKDIVCNT) >> BITP_CLKG_CLK_CTL1_ACLKDIVCNT;
      nCnvTime = ((nClock / nACLKDIVCNT) / (uint16_t)15385u) + 1u;   /* 65us acquisition time required = 15385Hz sample */
      
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
      
      /* Calculate the temperature voltage.
       * From the HRM: Temperature can be calculated as:
       *
       * T(^0 C)= code1/(code2+RG*code1)*Rvirtualreference/(ideal_sensitivity )-273.15
       * 
       * Some of these values are constants, and some have been read from registers.
       * The above formula, when populated with variables and constants, would look like this:
       * T(^0 C)= (nAdcTmpValue/(nAdcTmp2Value + nTempRG * nAdcTmpValue)) * (1.2256/1.2411e-3)) -273.15
       */
      {
          uint32_t nRVirRefByIdealSensitivity = 2070960834u; /* 1.2256/1.2411e-3 in 11.21 format */
          
          uint32_t nTempRG = 19380u; /* 1.1829 in 2.14 format */
          uint32_t nTmp2 = ((nAdcTmp2Value << 14u) + (nTempRG * nAdcTmpValue)); /* in 14.14 format */
          
          uint32_t nOffsetPart = (335544320u/nRefVoltage); /* (1.25 in 4.28 format / ReferenceVoltage(16.16)) = Result in format *.12 */
          uint32_t nOffset = (161u * nOffsetPart);         /* 12.12 format */
          
          uint32_t nTmp3  = ((nAdcTmpValue << 12) - nOffset) << 8u;  /* Format 12.20 */
          uint32_t nRatio = (nTmp3/(nTmp2 >> 10u));  /* nTmp2 resolution reduced by 10 to 14.4 and the result resolution is 0.16 */
          uint32_t nTemp  = (nRatio * (nRVirRefByIdealSensitivity >> 16u)) >> 5u;  /* Temperature in degree kelvin in 16.16 format */
          
          int32_t  iTemp = (int32_t)nTemp - ((int32_t)17901158);   /* Subtract 273.15 (in 16.16) to get the temperature in degree celcius */
          *pnTemperature = iTemp;
      }
    }
    else
    {
       eResult = ADI_ADC_BAD_SYS_CLOCK;
    }

    return eResult;
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
static bool InitBufferProcessing(ADI_ADC_DEVICE *pDevice)
{
    uint32_t nCnvReg = ((uint32_t)(pDevice->pReg->CNV_CFG) & BITM_ADC_CNV_CFG_DMAEN);
    ADC_INT_BUFFER* pIntBuffer = &pDevice->s_Buffer;
    
    if (IS_NOT_IN_ANY_STATE(ADC_STATUS_SAMPLING_IN_PROGRESS)) 
    {
        /* Clear conversion done flags */
        pDevice->pReg->STAT = 0xFFFFu;
        
        /* Clear the overflow and alert register */
        pDevice->pReg->OVF = 0xFFFFu;
    }
    
    /* Calculate the conversion register value for the given configuration */
    nCnvReg |= pIntBuffer->nChannels;
    if ((uint16_t)(pIntBuffer->nConfig & ADC_BUFFER_CONFIG_BUFFER_AUTO_MODE_EN) != 0u) {
        nCnvReg |= BITM_ADC_CNV_CFG_AUTOMODE;
    }    
    if ((pIntBuffer->nConfig & ADC_BUFFER_CONFIG_BUFFER_SINGLE_CONV_EN) != 0u) {
        nCnvReg |= BITM_ADC_CNV_CFG_SINGLE;
    } else {
        nCnvReg |= BITM_ADC_CNV_CFG_MULTI;
    }    

    SET_STATE(ADC_STATUS_SAMPLING_IN_PROGRESS);

    pDevice->pReg->CNV_CFG |= (uint16_t)nCnvReg;
    
    pDevice->pfManageFifo(pDevice, ADC_FIFO_MODE_ENABLED);

    return true;
}


#if ADI_ADC_ENABLE_MULTI_ACQUIRE == 1
/* DMA Callback Handler */
void DMA_ADC0_Int_Handler (void)
{
    ISR_PROLOG();
    ADI_ADC_DEVICE *pDevice = (ADI_ADC_DEVICE *) AdcDevInfo[0].hDevice;
    
    DmaFIFOManage(pDevice, ADC_FIFO_MODE_DMA_BUFFER_PROCESS);
    
    ISR_EPILOG();
}

static ADI_ADC_RESULT DmaFIFOManage (ADI_ADC_DEVICE *pDevice, ADC_FIFO_MODE eFifoMode)
{
    uint16_t nCount = 0u;
    uint16_t chanNum = ADC0_CHANn;
    uint16_t IRQ_Backup;
    
    ADC_INT_BUFFER* pIntBuffer = &pDevice->s_Buffer;
    
    if(pDevice->s_Buffer.pCurDataBuffer == NULL) {
        /* If there is nothing active... */
        if (eFifoMode == ADC_FIFO_MODE_INTERRUPT_PROCESS) {
            /* ...it's something leftover, so cleanup. */
            uint16_t nStat = pDevice->pReg->STAT & 0x00FFu;
            FlushFifo(pDevice, (uint32_t)nStat);
            pDevice->pReg->STAT = nStat;
        }
    }
    else {
        switch (eFifoMode)
        {
        case ADC_FIFO_MODE_INIT:

              /* Enable the interrupt for the given DMA */
              NVIC_EnableIRQ(DMA0_CH24_DONE_IRQn);
              
              pADI_DMA0->SRCADDR_CLR = 1U << chanNum;

              /* Enable the channel */
              pADI_DMA0->EN_SET = 1U << chanNum;
              
              /* Enables  peripheral to generate DMA requests. */
              pADI_DMA0->RMSK_CLR = 1U << chanNum;

              /* Set the primary as the current DMA descriptor */
              pADI_DMA0->ALT_CLR = 1U << chanNum;  /* Should be default */
                            
              /* Setup the DMA registers */
              nCount = (uint16_t)pIntBuffer->nNumSamplesRemaining;

              /* Point to the end of the DMA source */
              pPrimaryCCD[chanNum].DMASRCEND =   (uint32_t)(&(pDevice->pReg->DMA_OUT));
              
              /* Point to the end of the DMA write-to destination */ 
              pPrimaryCCD[chanNum].DMADSTEND =   (uint32_t)((void*)pIntBuffer->pCurDataBuffer) + ((nCount * 2u) - 1u);
              
              /* Configure the DMA itself */
              pPrimaryCCD[chanNum].DMACDC =         ((ADI_DMA_INCR_2_BYTE << DMA_BITP_CTL_DST_INC) |            /* Increment destination address */
                                                    (ADI_DMA_INCR_NONE << DMA_BITP_CTL_SRC_INC) |               /* Don't increment the source address */
                                                    ((uint32_t)ADI_DMA_WIDTH_2_BYTE << DMA_BITP_CTL_SRC_SIZE) |           /* 16bit transfers */
                                                    ((nCount - (uint32_t)1U)<< DMA_BITP_CTL_N_MINUS_1) |        /* Data size? */
                                                    (DMA_ENUM_CTL_CYCLE_CTL_BASIC << DMA_BITP_CTL_CYCLE_CTL) |  /*  Basic only */
                                                    ((uint32_t)ADI_DMA_RPOWER_1 << DMA_BITP_CTL_R_POWER));                /* Arbitration */
          
              /* Enable DMA */
              pDevice->pReg->CNV_CFG |= BITM_ADC_CNV_CFG_DMAEN;
            break;

        case ADC_FIFO_MODE_ENABLED:
            break;

        case ADC_FIFO_MODE_INTERRUPT_PROCESS:
            /* Clear the status registers */
            pDevice->pReg->STAT = (pDevice->pReg->STAT & 0x00FFu);
            break;

        case ADC_FIFO_MODE_INTERRUPT_OVERFLOW:
            pIntBuffer->nStatus |= ADC_BUFFER_STATUS_OVERFLOW;
            break;
      
        case ADC_FIFO_MODE_DMA_BUFFER_PROCESS:
            pIntBuffer->nNumSamplesRemaining = 0u;
            ManageFifoCompletion(pDevice);
            break;
            
        case ADC_FIFO_MODE_ABORT:
     
            /* Take backup of IRQ */
            IRQ_Backup = pDevice->pReg->IRQ_EN;

            /* Disable the IRQ */
            pDevice->pReg->IRQ_EN = 0u;

            /* Clear the conversion cfg register to stop any transaction */
            pDevice->pReg->CNV_CFG = 0u;
            
            /* Disable the DMA channel */
            pADI_DMA0->EN_CLR = 1U << chanNum;
              
            /* Clear the status bits */
            pDevice->pReg->STAT =  pDevice->pReg->STAT;
      
            /* Clear the sampling in progress state */
            CLR_STATE(ADC_STATUS_SAMPLING_IN_PROGRESS);
            
            /* Read and flush all the buffers */
            FlushFifo(pDevice, 0x00FFu);
          
            /* Restore the IRQ */
            pDevice->pReg->IRQ_EN = IRQ_Backup;      
          
            break;

        default:
            break;
        } 
    }
    
    return ADI_ADC_SUCCESS;
}
#else /* else ADI_ADC_ENABLE_MULTI_ACQUIRE == 0 */

static ADI_ADC_RESULT InterruptFIFOManage (ADI_ADC_DEVICE *pDevice, ADC_FIFO_MODE eFifoMode)
{
    ADC_INT_BUFFER* pIntBuffer = &pDevice->s_Buffer;
    
    if(pDevice->s_Buffer.pCurDataBuffer == NULL) {
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
                pIntBuffer->nNumSamplesRemaining -= 1u;

                pDevice->ActData.nCurChannel += 1u;
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

          /* Read and flush all the buffers */
          FlushFifo(pDevice, 0x00FFu);
        
          /* Restore the IRQ */
          pDevice->pReg->IRQ_EN = IRQ_Backup; 
        }
        break;

    default:
        break;
    }
    
    return ADI_ADC_SUCCESS;
}
#endif 

static void FlushFifo(ADI_ADC_DEVICE *pDevice, uint32_t nChannels)
{
    uint32_t x;
    for (x = 0u; x < 8u; x++) {
      if ((nChannels & ((uint32_t)1u << x)) != 0u) {
          ReadOutReg(x);
      }
    }
}


/* Called when a transfer is complete */
static void ManageFifoCompletion(ADI_ADC_DEVICE *pDevice)
{   
    /* Clear the conversion configuration */
    pDevice->pReg->CNV_CFG = 0u;
    CLR_STATE(ADC_STATUS_SAMPLING_IN_PROGRESS);
    
    SEM_POST(pDevice);
}


/* Internal function to extract the number of channels
 * in a 32bit word.  */
static uint32_t GetNumChannels(uint32_t nChannels)
{
    uint32_t n = nChannels & 0x000000FFu;

    n = (n & 0x00000055u) + ((n >> 1u) & 0x00000055u);
    n = (n & 0x00000033u) + ((n >> 2u) & 0x00000033u);
    n = (n + (n >> 4u)) & (0x0000000Fu);

    return n;
}

/* Returns the channel number based on the ADI_ADC_CHANNEL type.
 *  i.e. ADI_ADC_CHANNEL1 returns 1. */
static int32_t nGetChannelNumber(ADI_ADC_CHANNEL eChannel)
{
    int32_t retVal = 0;
    uint32_t nChannel = (uint32_t)eChannel & 0x000000FFu;
    
    if ((nChannel & (nChannel - (uint32_t)1u)) != 0u) {
        return -1;
    }
    if ((nChannel & 0x000000AAu) != 0u) { retVal += 1; }
    if ((nChannel & 0x000000CCu) != 0u) { retVal += 2; }
    if ((nChannel & 0x000000F0u) != 0u) { retVal += 4; }
    
    return retVal;
}

/* Internal function to set static configuration options. */
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
#elif ADI_ADC_CFG_VREF == 2     /* Battery Voltage */    
    nCfgReg |= BITM_ADC_CFG_VREFVBAT;
#endif
    
    pDevice->pReg->CFG = nCfgReg;

#if ADI_ADC_ENABLE_STATIC_COMPARATOR == 1
    /* High limit registers */
#if ADI_ADC_COMPARATOR_AIN0_HI_EN == 1
    pDevice->pReg->LIM0_HI = ADI_ADC_COMPARATOR_AIN0_HI_VAL;
    pDevice->ComparitorHi |= ADI_ADC_CHANNEL_0;
#endif
#if ADI_ADC_COMPARATOR_AIN1_HI_EN == 1
    pDevice->pReg->LIM1_HI = ADI_ADC_COMPARATOR_AIN1_HI_VAL;
    pDevice->ComparitorHi |= ADI_ADC_CHANNEL_1;
#endif
#if ADI_ADC_COMPARATOR_AIN2_HI_EN == 1
    pDevice->pReg->LIM2_HI = ADI_ADC_COMPARATOR_AIN2_HI_VAL;
    pDevice->ComparitorHi |= ADI_ADC_CHANNEL_2;
#endif
#if ADI_ADC_COMPARATOR_AIN3_HI_EN == 1
    pDevice->pReg->LIM3_HI = ADI_ADC_COMPARATOR_AIN3_HI_VAL;
    pDevice->ComparitorHi |= ADI_ADC_CHANNEL_3;
#endif
    /* Low limit registers */
#if ADI_ADC_COMPARATOR_AIN0_LO_EN == 1
    pDevice->pReg->LIM0_LO = (uint16_t)ADI_ADC_COMPARATOR_AIN0_LO_VAL;
    pDevice->ComparitorLo |= ADI_ADC_CHANNEL_0;
#endif
#if ADI_ADC_COMPARATOR_AIN1_LO_EN == 1
    pDevice->pReg->LIM1_LO = ADI_ADC_COMPARATOR_AIN1_LO_VAL;
    pDevice->ComparitorLo |= ADI_ADC_CHANNEL_1;
#endif
#if ADI_ADC_COMPARATOR_AIN2_LO_EN == 1
    pDevice->pReg->LIM2_LO = ADI_ADC_COMPARATOR_AIN2_LO_VAL;
    pDevice->ComparitorLo |= ADI_ADC_CHANNEL_2;
#endif
#if ADI_ADC_COMPARATOR_AIN3_LO_EN == 1
    pDevice->pReg->LIM3_LO = ADI_ADC_COMPARATOR_AIN3_LO_VAL;
    pDevice->ComparitorLo |= ADI_ADC_CHANNEL_3;
#endif
    
    /* Hysteresis registers */
#if ADI_ADC_COMPARATOR_AIN0_HYS_EN == 1
    pDevice->pReg->HYS0 = (uint16_t)(ADI_ADC_COMPARATOR_AIN0_HYS_VAL | (ADI_ADC_COMPARATOR_AIN0_HYS_CYC << BITP_ADC_HYS0_MONCYC));
    pDevice->ComparitorHys |= ADI_ADC_CHANNEL_0;
#endif
#if ADI_ADC_COMPARATOR_AIN1_HYS_EN == 1
    pDevice->pReg->HYS1 = (ADI_ADC_COMPARATOR_AIN1_HYS_VAL | (ADI_ADC_COMPARATOR_AIN1_HYS_CYC << BITP_ADC_HYS0_MONCYC));
    pDevice->ComparitorHys |= ADI_ADC_CHANNEL_1;
#endif
#if ADI_ADC_COMPARATOR_AIN2_HYS_EN == 1
    pDevice->pReg->HYS2 = (ADI_ADC_COMPARATOR_AIN2_HYS_VAL | (ADI_ADC_COMPARATOR_AIN2_HYS_CYC << BITP_ADC_HYS0_MONCYC));
    pDevice->ComparitorHys |= ADI_ADC_CHANNEL_2;
#endif
#if ADI_ADC_COMPARATOR_AIN3_HYS_EN == 1
    pDevice->pReg->HYS3 = (ADI_ADC_COMPARATOR_AIN3_HYS_VAL | (ADI_ADC_COMPARATOR_AIN3_HYS_CYC << BITP_ADC_HYS0_MONCYC));
    pDevice->ComparitorHys |= ADI_ADC_CHANNEL_3;
#endif
#endif

}

/* Internal function to enable the comparitor for previously-configured channels
 * Does not set the limits, only enables. 
*/
static void EnableComparator(ADI_ADC_DEVICE *pDevice, bool bEnable)
{
    uint32_t x;
    uint16_t nCnvCfg = 0u;
    volatile uint16_t* pLO_Register[4]  = {pREG_ADC0_LIM0_LO, pREG_ADC0_LIM1_LO, pREG_ADC0_LIM2_LO, pREG_ADC0_LIM3_LO};
    volatile uint16_t* pHI_Register[4]  = {pREG_ADC0_LIM0_HI, pREG_ADC0_LIM1_HI, pREG_ADC0_LIM2_HI, pREG_ADC0_LIM3_HI};
    volatile uint16_t* pHYS_Register[4] = {pREG_ADC0_HYS0,    pREG_ADC0_HYS1,    pREG_ADC0_HYS2,    pREG_ADC0_HYS3};
    
    if (bEnable == true)
    {
      /* Loop round all the channels enabling each part if required. */
      for (x = 0u; x < NUM_ADC_COMPARATOR_CHANNELS; x++) {
          if((pDevice->ComparitorHi & (1u << x)) > 0u) {
            *pHI_Register[x] |= BITM_ADC_LIM0_HI_EN;
          }
          if((pDevice->ComparitorLo & (1u << x)) > 0u) {
            *pLO_Register[x] |= BITM_ADC_LIM0_LO_EN;
          }
          if((pDevice->ComparitorHys & (1u << x)) > 0u) {
            *pHYS_Register[x] |= BITM_ADC_HYS0_EN;
          }
      }      
      nCnvCfg = (uint16_t)((uint16_t)pDevice->ComparitorHi | (uint16_t)pDevice->ComparitorLo);
      
      pDevice->pReg->IRQ_EN &= (uint16_t)(~BITM_ADC_IRQ_EN_CNVDONE);
      pDevice->pReg->CNV_CFG = (uint16_t)nCnvCfg | (uint16_t)(BITM_ADC_CNV_CFG_MULTI | BITM_ADC_CNV_CFG_AUTOMODE);
      SET_STATE(ADC_STATUS_COMPARATOR_EN);
    }    
    else {
      /* Loop round disabling all. */
      for (x = 0u; x < NUM_ADC_COMPARATOR_CHANNELS; x++) {
            *pHI_Register[x]  &= (uint16_t)(~(BITM_ADC_LIM0_HI_EN));
            *pLO_Register[x]  &= (uint16_t)(~(BITM_ADC_LIM0_LO_EN));
            *pHYS_Register[x] &= (uint16_t)(~(BITM_ADC_HYS0_EN));
        }
        pDevice->pReg->CNV_CFG = 0u;
        pDevice->pReg->STAT = pDevice->pReg->STAT & 0x00FFu;
        CLR_STATE(ADC_STATUS_COMPARATOR_EN);
        pDevice->pReg->IRQ_EN |= BITM_ADC_IRQ_EN_CNVDONE;
    }
}


/* In Handler handles the following cases:
 * ADI_ADC_EVENT_ADC_READY
 * ADI_ADC_EVENT_CALIBRATION_DONE
 * ADC_STATUS_BATTERY_DONE
 * ADC_STATUS_TMP_DONE
 * ADC_STATUS_TMP2_DONE
 * ADI_ADC_EVENT_HIGH_LIMIT_CROSSED
 * ADI_ADC_EVENT_LOW_LIMIT_CROSSED
*/
void ADC0_Int_Handler(void)
{
    ADI_ADC_DEVICE *pDevice;
    ISR_PROLOG();
    
    pDevice = (ADI_ADC_DEVICE *) AdcDevInfo[0].hDevice;
    
    if ((pDevice->pReg->STAT & 0x00FFu) != 0u) {
        if (IS_NOT_IN_STATE(ADC_STATUS_COMPARATOR_EN)) {
            pDevice->pfManageFifo(pDevice, ADC_FIFO_MODE_INTERRUPT_PROCESS);
        } else {
            pDevice->pReg->STAT = pDevice->pReg->STAT & (0x00FFu);
        }
    }
    if ((uint16_t)(pDevice->pReg->STAT & 0xFF00u) != 0u) {
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
      uint32_t channel;
      if (IS_IN_STATE(ADC_STATUS_COMPARATOR_EN) && (pDevice->pfCallback != NULL)) {
        for (channel = 0u; channel < (NUM_ADC_COMPARATOR_CHANNELS); channel++) {
          /* Alert bit positions: hi limits are 0b01, 
          *                      lo limit alerts are 0b10.
          */
          if((nAlertValue & (1u << (2u * channel))) > 0u) {
            pDevice->pfCallback(pDevice->pCBParam, ADI_ADC_EVENT_HIGH_LIMIT_CROSSED, (void*)channel);
          }
          if((nAlertValue & (1u << ((2u * channel) + ((uint32_t)1u)))) > 0u)
          {
            pDevice->pfCallback(pDevice->pCBParam, ADI_ADC_EVENT_LOW_LIMIT_CROSSED, (void*)channel);
          }
        }
      }
      pDevice->pReg->ALERT = (uint16_t)nAlertValue;
    }
    ISR_EPILOG();     
}


/*! \endcond */

#endif /* ADI_ADC_C */

/*****/

/*@}*/
