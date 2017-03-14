/** @addtogroup RTOS_INTERRUPTS RTOS-specific Interrupt APIs
*  @{
*/
/*!
 *****************************************************************************
 * @file    adi_nvic.h
 * @brief   API header file for NVIC controller. These APIs are only expected to be used in an RTOS-context.
 * @version $Revision$
 * @date    $Date$
 *
 -----------------------------------------------------------------------------
 
Copyright (c) 2010-2013 Analog Devices, Inc.

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

THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF CLAIMS OF INTELLECTUAL
PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*****************************************************************************/
#ifndef _ADI_NVIC_H_
#define _ADI_NVIC_H_

#include <adi_types.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*! Number of system exceptions */
#define ADI_NUM_SYSTEM_EXPT         15u

/*! Convert IRQ number to System Interrupt ID
  (irq - number of system exceptions - 1 location for stack pointer in the vector table*/
#define ADI_NVIC_IRQ_SID(irq)       ((irq) - ADI_NUM_SYSTEM_EXPT - 1u)

/*! Convert SID to IRQ number */
#define ADI_NVIC_SID_IRQ(sid)       ((sid) + ADI_NUM_SYSTEM_EXPT + 1u)

/*! Return codes from NVIC */
typedef enum
{
    ADI_NVIC_SUCCESS,       /*!< Generic success. */
    ADI_NVIC_FAILURE,       /*!< Generic Failure. */
    ADI_NVIC_INVALID_INT    /*!< If the given interrupt ID is invalid. */
} ADI_NVIC_RESULT;

/*! \cond PRIVATE */
/* Interrupt handler type for registering with NVIC */
typedef void (*ADI_NVIC_HANDLER) (
        void
);
/*! \endcond */

/* API to register interrupt handler with NVIC */
int32_t adi_nvic_RegisterHandler (
        uint32_t             iid,
        ADI_NVIC_HANDLER     pfHandler
);

/* API to unregister interrupt handler with NVIC */
int32_t adi_nvic_UnRegisterHandler (
        uint32_t              iid
);

/* API to enable interrupt */
ADI_NVIC_RESULT adi_nvic_EnableInt (
        uint32_t               iid,
        bool_t                 bEnable
);

#ifdef __cplusplus
}
#endif

#endif /* _ADI_NVIC_H_ */
/*@}*/
