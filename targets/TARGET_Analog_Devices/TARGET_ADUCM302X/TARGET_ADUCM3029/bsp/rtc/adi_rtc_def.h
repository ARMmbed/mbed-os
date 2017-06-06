/*!
 *****************************************************************************
 * @file:    adi_rtc_def.h
 * @brief:   RTC def file 
 * @version: $Revision: 33205 $
 * @date:    $Date: 2016-01-11 05:46:07 -0500 (Mon, 11 Jan 2016) $
 *-----------------------------------------------------------------------------
 *
 * Copyright (c) 2010-2016 Analog Devices, Inc.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Modified versions of the software must be conspicuously marked as such.
 * - This software is licensed solely and exclusively for use with processors
 *   manufactured by or for Analog Devices, Inc.
 * - This software may not be combined or merged with other code in any manner
 *   that would cause the software to become subject to terms and conditions
 *   which differ from those listed here.
 * - Neither the name of Analog Devices, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 * - The use of this software may or may not infringe the patent rights of one
 *   or more patent holders.  This license does not release you from the
 *   requirement that you obtain separate licenses from these patent holders
 *   to use this software.
 *
 * THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
 * TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
 * NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF CLAIMS OF INTELLECTUAL
 * PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *****************************************************************************/

#ifndef ADI_RTC_DEF_H__
#define ADI_RTC_DEF_H__ 

/*! \cond PRIVATE */
#define ADI_RTC_NUM_INSTANCE 2u 



#define ADI_RTC_INT_ENA_MASK_CR0         0XF804u

#define ADI_RTC_INT_ENA_MASK_CR1         0X1Fu

#define ADI_RTC_INT_ENA_MASK_CR2IC       0xF41C
#define ADI_RTC_INT_ENA_MASK_CR3SS       0x1FFE
#define ADI_RTC_INT_ENA_MASK_CR4SS       0x0E0E
#define ADI_RTC_INT_ENA_MASK_CR5SSS      0x0FFF

#define ADI_RTC_INT_SOURCE_MASK_SR0      0x007Eu
#define ADI_RTC_INT_SOURCE_MASK_SR2      0x001Fu

#define ADI_RTC_WRITE_STATUS_MASK        0XCF8u
#define ADI_RTC_SR2_IRQ_STATUS_MASK      0X1Fu
#define ADI_RTC_SR3_IRQ_STATUS_MASK      0X1FFFu



#define ADI_RTC_TRIM_MASK (BITM_RTC_TRM_VALUE | BITM_RTC_TRM_ADD|BITM_RTC_TRM_IVL | BITM_RTC_TRM_IVL2EXPMIN )

#if (ADI_RTC_CFG_ENABLE_SAFE_WRITE == 1)
    /* pause on pending writes to CR to avoid data loss */

#ifdef __ICCARM__
/*
* Pm154 (rule 19.10): in the definition of a function-like macro, each instance
*                     of a parameter shall be enclosed in parentheses
*   Parameter use without parentheses needed for struct field name in register access macro.
*/
#pragma diag_suppress=Pm154
#endif /* __ICCARM__ */

#define  PEND_BEFORE_WRITE(reg,mask) while((pDevice->pRTCRegs->reg&(mask))!=0u)\
        {\
        }

#define  SYNC_AFTER_WRITE(reg,mask) while((pDevice->pRTCRegs->reg&(mask))==0u)\
        {\
        }

#ifdef __ICCARM__
#pragma diag_default=Pm154
#endif /* __ICCARM__ */

#else
    /* pause on pending writes to CR to avoid data loss */
#define  PEND_BEFORE_WRITE(reg,mask)
#define  SYNC_AFTER_WRITE(reg,mask)        
#endif

/* 
 * The following is used for static configuration
 */
typedef struct
{
    uint16_t    CR0;             /*!< CR0 16 bit control register-0 value */  
    uint16_t    CR1;             /*!< CR1 16 bit control register-1 value */
    uint16_t    CNT0;            /*!< CNT0 16 bit count register value */
    uint16_t    CNT1;            /*!< CNT1 16 bit count register value */
   
    uint16_t    ALM0;            /*!< ALM0 16 bit integer part of alarm value */
    uint16_t    ALM1;            /*!< ALM1 16 bit integer part of alarm value */
    uint16_t    ALM2;            /*!< ALM2 16 bit integer part of alarm value */
    uint16_t    TRIM;              /*!< 16 bit trim register value */    
    uint16_t    CR2IC;           /*!< CR2IC 16 bit control (which controls the input capture ) register-2 value */
    uint16_t    CR3SS;           /*!< CR3SS 16 bit control ( Controls enabling sensor strobe /IRQ etc )register-3 value */
    uint16_t    CR4SS;           /*!< CR4SS 16 bit control ( controls Auto reload  and mask for sensor strobe  ) register-4 value */
    uint16_t    SSMSK;           /*!< OCMSK Mask register for sensor strobe channel */
    uint16_t    SS1;             /*!< 16 bit Auto reload value */

    uint16_t    CR5SSS;         /*!< Configure Sensor Strobe Channel GPIO Sampling Register */
    uint16_t    CR6SSS;         /*!< Configure Sensor Strobe Channel GPIO Sampling Register */
    uint16_t    CR7SSS;         /*!< Configure Sensor Strobe Channel GPIO Sampling Register */
    uint16_t    GPMUX0;         /*!< Control register for selecting a GPIO (pin) as data to be sampled by a Sensor Strobe  channel */
    uint16_t    GPMUX1;         /*!< Control register for selecting a GPIO (pin) as data to be sampled by a Sensor Strobe  channel */
}ADI_RTC_CONFIG;

/* Device information structure */
typedef struct _ADI_RTC_DEVICE_INFO
{
    volatile ADI_RTC_TypeDef  *pRTCRegs;   /* Base address of the  SPORT registers */
    const IRQn_Type            eIRQn;      /* IRQn */   
    ADI_RTC_HANDLE             hDevice;    /* RTC handle */
}ADI_RTC_DEVICE_INFO;

/*! RTC driver instance data */
typedef struct _ADI_RTC_DEVICE
{
    volatile ADI_RTC_TypeDef *pRTCRegs;    /* Pointer to RTC Memory Mapped Registers */

    ADI_CALLBACK              pfCallback;  /*  Function pointer for callback function. */

    void                     *pCBParam;    /*  Parameter to callback function. */
    IRQn_Type                eIRQn;        /* IRQn */       
    uint32_t  cbWatch;
    ADI_RTC_DEVICE_INFO     *pDeviceInfo;  /*  Parameter to callback function. */
    
} ADI_RTC_DEVICE;


static void rtc_init(ADI_RTC_DEVICE *pDevice,ADI_RTC_CONFIG *pConfig);

#ifdef ADI_DEBUG
static ADI_RTC_RESULT ValidateHandle( ADI_RTC_DEVICE *pInDevice);
#endif
/*! \endcond */    
#endif /* ADI_RTC_DEF_H__ */

