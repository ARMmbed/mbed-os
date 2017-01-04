/*!
 *****************************************************************************
 * @file:    adi_tmr_def_v1.h
 * @brief:   GP timer def file 
 * @version: $Revision: 33205 $
 * @date:    $Date: 2016-01-11 10:46:07 +0000 (Mon, 11 Jan 2016) $
 *-----------------------------------------------------------------------------
 *
 * Copyright (c) 2010-2014 Analog Devices, Inc.
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

#ifndef __ADI_TMR_DEF_V1_H__
#define __ADI_TMR_DEF_V1_H__

/*! \cond PRIVATE */
#define ADI_TMR_NUM_INSTANCE 3u

/*! Timer configuration structure used to hold the application configuration.
*/
typedef struct
{
    uint16_t    LOAD;            /*!< 16 bit load value */  
    uint16_t    CTL;             /*!< Timer control register value */
    uint16_t    ALOAD;           /*!< Timer Ansynchrounous load register value */
    uint16_t    PWMCTL;          /*!< Timer PWM control register value */
    uint16_t    PWMMATCH;        /*!< Timer PWM match value */

}ADI_TMR_CONFIG;

/* Device information structure */
typedef struct _ADI_TIMER_DEVICE_INFO
{
    volatile ADI_TMR_TypeDef  *pTmrRegs;   /* Base address of the  SPORT registers */
    const IRQn_Type            eIRQn;         /* IRQn */   
    ADI_INT_HANDLER_PTR        pIntHndler;
    ADI_TMR_HANDLE             hDevice;      /* TIMER handle */    
}ADI_TIMER_DEVICE_INFO;

/*! TIMER driver instance data */
typedef struct _ADI_TMR_DEVICE
{
    volatile ADI_TMR_TypeDef *pTmrRegs;   /* Pointer to Timer  Memory Mapped Registers */

    ADI_CALLBACK              pfCallback;   /*  Function pointer for callback function. */

    void                     *pCBParam;     /*  Parameter to callback function. */
    
    ADI_TIMER_DEVICE_INFO     *pDeviceInfo; /*  Parameter to callback function. */
} ADI_TMR_DEVICE;
ADI_INT_HANDLER(GP_Tmr0_Int_Handler);
ADI_INT_HANDLER(GP_Tmr1_Int_Handler);
ADI_INT_HANDLER(GP_Tmr2_Int_Handler);
/*! \endcond */
#endif /* __ADI_PWR_DEF_H__ */

