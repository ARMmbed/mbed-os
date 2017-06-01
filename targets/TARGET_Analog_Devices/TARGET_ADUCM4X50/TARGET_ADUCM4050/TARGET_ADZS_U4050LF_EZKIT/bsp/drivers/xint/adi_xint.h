/*
 *****************************************************************************
   @file:    adi_xint.h
   @brief:   External interrupt driver definitions and API
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
#ifndef ADI_XINT_H
#define ADI_XINT_H

/*! \addtogroup XINT_Driver External Interrupt Driver
 *  @{
 */

#ifdef __ICCARM__
#pragma diag_default=Pm008
#endif /* __ICCARM__ */

#include <adi_callback.h>
#include <adi_processor.h>

#if !defined(__ADUCM4x50__)
#error "Unknown processor family"
#endif

/* C++ linkage */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*! Amount of memory(in bytes) required by the External Interrupt device driver for its operation.
 * This memory is completely owned by the driver till the end of the operation.
 */
#define ADI_XINT_MEMORY_SIZE        (48u)

/*! External Interrupt Driver API function return codes */
typedef enum
{
    ADI_XINT_SUCCESS = 0,                       /*!< API successfully returned. */
    ADI_XINT_FAILURE,                           /*!< The API call failed. */
    ADI_XINT_ALREADY_INITIALIZED,               /*!< External interrupt driver has already been initialized. */
    ADI_XINT_NOT_INITIALIZED,                   /*!< External interrupt driver has not yet been initialized. */
    ADI_XINT_NULL_PARAMETER,                    /*!< The given pointer is pointing to NULL. */
    ADI_XINT_INVALID_MEMORY_SIZE,               /*!< The given memory is not sufficient to operate the driver. */   
    ADI_XINT_INVALID_INTERRUPT                  /*!< Invalid interrupt number. */
} ADI_XINT_RESULT;


/*! External interrupt trigger condition enumerations */
typedef enum {
    ADI_XINT_IRQ_RISING_EDGE    = 0x0,          /*!< Trigger an interrupt when a rising edge is detected.    */
    ADI_XINT_IRQ_FALLING_EDGE   = 0x1,          /*!< Trigger an interrupt when on a falling edge is detected.   */
    ADI_XINT_IRQ_EITHER_EDGE    = 0x2,          /*!< Trigger an interrupt on either falling or rising edge is detected. */
    ADI_XINT_IRQ_HIGH_LEVEL     = 0x3,          /*!< Trigger an interrupt on a logic level high is detected.     */
    ADI_XINT_IRQ_LOW_LEVEL      = 0x4           /*!< Trigger an interrupt on a logic level low is detected.  */
} ADI_XINT_IRQ_MODE;

/*! External interrupts. */
typedef enum {
    ADI_XINT_EVENT_INT0         = 0x0,          /*!< Event for external interrupt-0   */
    ADI_XINT_EVENT_INT1         = 0x1,          /*!< Event for external interrupt-1   */
    ADI_XINT_EVENT_INT2         = 0x2,          /*!< Event for external interrupt-2   */
    ADI_XINT_EVENT_INT3         = 0x3,          /*!< Event for external interrupt-3   */
    ADI_XINT_EVENT_RESERVED     = 0x4,          /*!< Event is reserved.   */
    ADI_XINT_EVENT_UART_RX      = 0x5,          /*!< Event for UART Rx activity   */
    ADI_XINT_EVENT_MAX          = 0x6           /*!< Number of external interrupt events */
} ADI_XINT_EVENT;


/* External Interrupt API functions */
ADI_XINT_RESULT  adi_xint_Init                  (void* const pMemory, uint32_t const MemorySize);
ADI_XINT_RESULT  adi_xint_UnInit                (void);
ADI_XINT_RESULT  adi_xint_EnableIRQ             (const ADI_XINT_EVENT eEvent, const ADI_XINT_IRQ_MODE eMode);
ADI_XINT_RESULT  adi_xint_DisableIRQ            (const ADI_XINT_EVENT eEvent); 
ADI_XINT_RESULT  adi_xint_RegisterCallback      (const ADI_XINT_EVENT eEvent, ADI_CALLBACK const pfCallback, void *const pCBParam );

#ifdef __cplusplus
}
#endif

/**@}*/

#endif /* ADI_XINT_H */
