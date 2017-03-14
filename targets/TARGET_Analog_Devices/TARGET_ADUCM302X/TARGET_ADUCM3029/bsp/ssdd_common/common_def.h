/*!
 *****************************************************************************
 @file:    common_def.h
 @brief:   Common Definition Used by all the drivers
 @version: $Revision$
 @date:    $Date$
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

#ifndef __COMMON_DEF_H__
#define __COMMON_DEF_H__

#include <services/int/adi_int.h>
#include <services/pwr/adi_pwr.h>
#include <adi_global_config.h>

#if (ADI_CFG_ENABLE_RTOS_SUPPORT == 1)
#if defined (USER_SPECIFIED_RTOS)
#include <user_rtos_support.h>
#else
#include <adi_osal.h>
#endif
#endif


#if defined(__ECC__)
#define STRUCT_ELEMENT_ALIGN
#define STRUCT_ELEMENT_ALIGN4  _Pragma("align(4)")
#elif defined(__ICCARM__)
#define STRUCT_ELEMENT_ALIGN   _Pragma("pack()")
#define STRUCT_ELEMENT_ALIGN4  _Pragma("pack(4)")
#elif defined (__GNUC__)
#define STRUCT_ELEMENT_ALIGN   _Pragma("pack()")
#define STRUCT_ELEMENT_ALIGN4  _Pragma("pack(4)")
#endif

/* IF (RTOS Support is enabled) */
#if (ADI_CFG_ENABLE_RTOS_SUPPORT == 1)

#if !defined (USER_SPECIFIED_RTOS)
#define PEND_VAR_DECLR                                                                          \
                            ADI_OSAL_SEM_HANDLE    hSemaphore;                                  \
                            STRUCT_ELEMENT_ALIGN4                                               \
                            uint8_t                SemMemory[ADI_OSAL_MAX_SEM_SIZE_CHAR];

#define PEND_EVENT(DEV,RESULT)                                                                         \
                if(adi_osal_SemPend((DEV)->hSemaphore,ADI_OSAL_TIMEOUT_FOREVER)!=ADI_OSAL_SUCCESS) \
                {\
                   return(RESULT);\
                }\


#define POST_EVENT(DEV)                                                                        \
                adi_osal_SemPost((DEV)->hSemaphore);
                
#define CREATE_SEM(DEV,RESULT)                                                                        \
                (DEV)->hSemaphore = NULL;\
                if(adi_osal_SemCreateStatic(&(DEV)->SemMemory[0],ADI_OSAL_MAX_SEM_SIZE_CHAR,&(DEV)->hSemaphore,0U)!=ADI_OSAL_SUCCESS)\
                {\
                   return(RESULT);\
                }\

#define DELETE_SEM(DEV,RESULT)                                                                        \
                if(adi_osal_SemDestroyStatic((DEV)->hSemaphore)!=ADI_OSAL_SUCCESS)\
                {\
                   return(RESULT);\
                }\
 /* 
 * The OSAL support does not require a prolog or epilog, but we define it to
 * something so we don't get warnings related to a semi-colon in the code
 */

#define ISR_PROLOG() do {} while (0)
#define ISR_EPILOG() do {} while (0)

#endif
/* IF (Low Power Mode is enabled) */
#else
/* 
 * The no-OS support does not require prolog and epilog or semaphores, but we define it to
 * something so we don't get warnings related to a semi-colon in the code
 */

#define ISR_PROLOG() do {} while (0)
#define ISR_EPILOG() do {} while (0)
#define CREATE_SEM(DEV, error) do {} while (0)
#define DELETE_SEM(DEV, error) do {} while (0)

#if (ADI_CFG_ENTER_LOW_PWR_MODE_SUPPORT == 1)

#define PEND_VAR_DECLR         volatile bool_t bLowPowerExitFlag;


#define PEND_EVENT(DEV,RESULT)                                                                \
    adi_pwr_EnterLowPowerMode(ADI_PWR_MODE_FLEXI, &(DEV)->bLowPowerExitFlag, 0u);


#define POST_EVENT(DEV)                                                                        \
    adi_pwr_ExitLowPowerMode(&(DEV)->bLowPowerExitFlag);

#else

#define PEND_VAR_DECLR       volatile uint8_t nSemCount;

#define PEND_EVENT(DEV,RESULT)                                                               \
    while ((DEV)->nSemCount == 0u) {}                                                 \
    (DEV)->nSemCount--


#define POST_EVENT(DEV) {       \
    (DEV)->nSemCount++;         \
}

#endif /* ADI_CFG_ENTER_LOW_PWR_MODE_SUPPORT */
#endif /* ADI_CFG_ENABLE_RTOS_SUPPORT */

#endif /* __COMMON_DEF_H__ */
