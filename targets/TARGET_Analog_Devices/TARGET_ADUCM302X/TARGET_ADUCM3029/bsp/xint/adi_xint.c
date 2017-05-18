/******************************************************************************
   @file:    adi_xint.c
   @brief:   External Interrupt device driver implementation.
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

*/
/*****************************************************************************/

#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <drivers/xint/adi_xint.h>
#include <rtos_map/adi_rtos_map.h>
#include "adi_xint_def.h"

#ifdef __ICCARM__
/*
* IAR MISRA C 2004 error suppressions.
*
* Pm073 (rule 14.7): a function should have a single point of exit
* Pm143 (rule 14.7): a function should have a single point of exit at the end of the function
*   Multiple returns are used for error handling.
* Pm140 (rule 11.3): a cast should not be performed between a pointer type and an integral type
*   The rule makes an exception for memory-mapped register accesses.
* Pm140 (rule 10.3): illegal explicit conversion from underlying MISRA type unsigned int to enum
*   The typecast is used for efficiency of the code.
* Pm140 (rule 17.4): array indexing shall only be applied to objects defined as an array
*   Array indexing is required on the pointer. The memory for gpCallbackTable is passed from application
*/
#pragma diag_suppress=Pm073,Pm143,Pm140,Pm136,Pm152
#endif /* __ICCARM__ */

static inline void XIntCommonInterruptHandler  (const ADI_XINT_EVENT eEvent);
void Ext_Int0_Handler(void);
void Ext_Int1_Handler(void);
void Ext_Int2_Handler(void);
void Ext_Int3_Handler(void);



/*==========  D A T A  ==========*/

static ADI_XINT_CALLBACK_INFO *gpCallbackTable;

/*! \endcond */

/*! \addtogroup XINT_Driver External Interrupt Driver
 *  @{
 *  @brief External Interrupt (XINT) Driver
 *  @note The application must include drivers/xint/adi_xint.h to use this driver
 */

/*!
    @brief      Initializes the External Interrupt Driver.

    @details    This function does the external interrupt driver initialization. This function should be called
                before calling any of the XINT driver APIs.

    @param[in] pMemory      Pointer to the memory to be used by the driver.
                            Size of the memory should be at equal to #ADI_XINT_MEMORY_SIZE bytes.
    @param[in] MemorySize   Size of the memory passed in pMemory parameter.

    @return     Status
                    - ADI_XINT_SUCCESS                  If successfully initialized XINT driver.
                    - ADI_XINT_NULL_PARAMETER       [D] If the given pointer to the driver memory is pointing to NULL.
                    - ADI_XINT_INVALID_MEMORY_SIZE  [D] If the given memory size is not sufficient to operate the driver.
                   
    @sa         adi_xint_UnInit
*/
ADI_XINT_RESULT adi_xint_Init(void*                const pMemory,
                              uint32_t             const MemorySize
)
{

#ifdef ADI_DEBUG
        /* Verify the given memory pointer */
        if(NULL == pMemory)
        {
            return ADI_XINT_NULL_PARAMETER;
        }
        /* Check if the memory size is sufficient to operate the driver */
        if(MemorySize < ADI_XINT_MEMORY_SIZE)
        {
            return ADI_XINT_INVALID_MEMORY_SIZE;
        }
        assert(MemorySize == (sizeof(ADI_XINT_CALLBACK_INFO) * ADI_XINT_EVENT_MAX));
#endif

    /* Only initialize on 1st init call, i.e., preserve callbacks on multiple inits */
    if (gpCallbackTable == NULL)
    {
        /* Clear the memory passed by the application */
        memset(pMemory, 0, MemorySize);

        gpCallbackTable = (ADI_XINT_CALLBACK_INFO *)pMemory;
    }
    return (ADI_XINT_SUCCESS);
}


/*!
    @brief      Un-initialize the external interrupt driver.

    @details    Terminates the XINT functions, leaving everything unchanged.

    @return     Status
                    - #ADI_XINT_SUCCESS             If successfully uninitialized XINT driver.
                    - #ADI_XINT_NOT_INITIALIZED [D] If XINT driver not yet initialized.

    @sa         adi_xint_Init
*/
ADI_XINT_RESULT adi_xint_UnInit(void)
{

#ifdef ADI_DEBUG
    /* IF (not initialized) */
    if (NULL == gpCallbackTable)
    {
        /* return error if not initialized */
        return (ADI_XINT_NOT_INITIALIZED);
    }
#endif

    /* Clear the callback pointer */
    gpCallbackTable = NULL;

    return (ADI_XINT_SUCCESS);
}



/*!
    @brief      Enable an External Interrupt

    @details    Enables and sets the triggering mode for the given external interrupt.
                Applications may register a callback using the #adi_xint_RegisterCallback
                API to get a notification when the interrupt occurs.

                To get the external interrupt working application has to enable the input
                (using the GPIO driver API \a adi_gpio_InputEnable) for the corresponding GPIO
                pin. Please refer the GPIO chapter pin-muxing section of the Hardware Reference
                Manual to see the GPIO pin that is mapped to the required external interrupt.

    @param[in]  eEvent      Event which needs to be enabled.
    @param[in]  eMode       Interrupt trigger mode for the external interrupt.

    @return     Status
                    - #ADI_XINT_SUCCESS             If successfully enabled the external interrupt.
                    - #ADI_XINT_NOT_INITIALIZED [D] If external interrupt driver not yet initialized.

    @sa         adi_xint_DisableIRQ
    @sa         adi_xint_RegisterCallback
*/
ADI_XINT_RESULT adi_xint_EnableIRQ(const ADI_XINT_EVENT eEvent, const ADI_XINT_IRQ_MODE eMode)
{
    uint32_t            Mask;       /* mask to manipulate the register */
    uint32_t            Pattern;    /* bit pattern that will be written into the register */
    uint32_t            CfgReg;     /* interrupt config register value */
    IRQn_Type           XintIrq;
    ADI_INT_STATUS_ALLOC();

#ifdef ADI_DEBUG
    /* make sure we're initialized */
    if (NULL == gpCallbackTable)
    {
        return (ADI_XINT_NOT_INITIALIZED);
    }
#endif

    /* create the mask we'll use to clear the relevant bits in the config register */
    Mask = (BITM_XINT_CFG0_IRQ0MDE | BITM_XINT_CFG0_IRQ0EN) << (ADI_XINT_CFG_BITS * (uint32_t)eEvent);

    /* The Pattern has to be created differently for UART RX wakeup and other events as the
       mode and enable bits are flipped in case of UART RX */
    
    /* Based on the event figure out the interrupt it is mapped to */
    if(eEvent == ADI_XINT_EVENT_UART_RX)
    {        
          /* create the bit pattern we're going to write into the configuration register */
         Pattern = (BITM_XINT_CFG0_UART_RX_EN | ((uint32_t)eMode << BITP_XINT_CFG0_UART_RX_MDE));
         
         XintIrq = XINT_EVT3_IRQn;
    }
    else
    {
        /* create the bit pattern we're going to write into the configuration register */
        Pattern = (BITM_XINT_CFG0_IRQ0EN | eMode) << (ADI_XINT_CFG_BITS * (uint32_t)eEvent);
        
        XintIrq = (IRQn_Type)((uint32_t)XINT_EVT0_IRQn + (uint32_t)eEvent);
    }

    
    ADI_ENTER_CRITICAL_REGION();

    /* read/modify/write the appropriate bits in the register */
    CfgReg = pADI_XINT0->CFG0;
    CfgReg &= ~Mask;
    CfgReg |= Pattern;
    pADI_XINT0->CFG0 = CfgReg;

    ADI_EXIT_CRITICAL_REGION();

    /* enable the interrupt */
    NVIC_EnableIRQ(XintIrq);

    return (ADI_XINT_SUCCESS);
}


/*!
    @brief      Disable an External Interrupt

    @details    Disables an external interrupt

    @param[in]  eEvent  External Interrupt event that should be disabled.

    @return     Status
                    - #ADI_XINT_SUCCESS             If successfully disabled the external interrupt.
                    - #ADI_XINT_NOT_INITIALIZED [D] If external interrupt driver is not yet initialized.

    @sa         adi_xint_EnableIRQ
    @sa         adi_xint_RegisterCallback
*/
ADI_XINT_RESULT adi_xint_DisableIRQ(const ADI_XINT_EVENT eEvent)
{
    uint32_t            Mask;       /* mask to manipulate the register */
    uint32_t            CfgReg;     /* interrupt config register value */
    IRQn_Type           XintIrq;    /* External interrupt IRQ the event is mapped to */

    ADI_INT_STATUS_ALLOC();

#ifdef ADI_DEBUG
    /* make sure we're initialized */
    if (NULL == gpCallbackTable)
    {
        return (ADI_XINT_NOT_INITIALIZED);
    }
#endif

    /* Based on the event figure out the interrupt it is mapped to */
    if(eEvent == ADI_XINT_EVENT_UART_RX)
    {
        XintIrq = XINT_EVT3_IRQn;
    }
    else
    {
        XintIrq = (IRQn_Type)((uint32_t)XINT_EVT0_IRQn + (uint32_t)eEvent);
    }

    /* disable the interrupt */
    NVIC_DisableIRQ(XintIrq);

    /* create the mask we'll use to clear the relevant bits in the config register */
    Mask = (BITM_XINT_CFG0_IRQ0MDE | BITM_XINT_CFG0_IRQ0EN) << (ADI_XINT_CFG_BITS * (uint32_t)eEvent);

    ADI_ENTER_CRITICAL_REGION();
    /* read/modify/write the appropriate bits in the register */
    CfgReg = pADI_XINT0->CFG0;
    CfgReg &= ~Mask;
    pADI_XINT0->CFG0 = CfgReg;
    ADI_EXIT_CRITICAL_REGION();

    return (ADI_XINT_SUCCESS);
}


/*!
    @brief      Register or unregister an application callback function for external pin interrupts.

    @details    Applications may register a callback function that will be called when an
                external interrupt occurs. In addition to registering the interrupt,
                the application should call the #adi_xint_EnableIRQ API to enable the
                external pin interrupt.

                The driver dispatches calls to registered callback functions when the
                properly configured pin(s) latches an external interrupt input on the XINT
                pin(s).  The callback is dispatched with the following parameters, respectively:

                - application-provided callback parameter (\a pCBParam),
                - the interrupt ID (#ADI_XINT_EVENT) that initiated the interrupt,
                - NULL.

    @param[in]  eEvent          The interrupt for which the callback is being registered.
    @param[in]  pfCallback      Pointer to the callback function. This can be passed as NULL to
                                unregister the callback.
    @param[in]  pCBParam        Callback parameter which will be passed back to the application
                                when the callback is called..

    @return     Status
                    - #ADI_XINT_SUCCESS                 If successfully registered the callback.
                    - #ADI_XINT_NOT_INITIALIZED     [D] If external interrupt driver is not yet initialized.

    @sa         adi_xint_EnableIRQ
    @sa         adi_xint_DisableIRQ
*/
ADI_XINT_RESULT  adi_xint_RegisterCallback (const ADI_XINT_EVENT eEvent, ADI_CALLBACK const pfCallback, void *const pCBParam )
{
    ADI_INT_STATUS_ALLOC();

#ifdef ADI_DEBUG
    /* make sure we're initialized */
    if (NULL == gpCallbackTable)
    {
        return (ADI_XINT_NOT_INITIALIZED);
    }
#endif

    ADI_ENTER_CRITICAL_REGION();
    gpCallbackTable[eEvent].pfCallback  = pfCallback;
    gpCallbackTable[eEvent].pCBParam    = pCBParam;
    ADI_EXIT_CRITICAL_REGION();

    /* return the status */
    return (ADI_XINT_SUCCESS);
}

/*@}*/

/*! \cond PRIVATE */
/* All of the following is excluded from the doxygen output... */

/* Common external interrupt handler */
static inline void XIntCommonInterruptHandler(const ADI_XINT_EVENT eEvent)
{
    /* Clear the IRQ */
    pADI_XINT0->CLR = (1u << (uint32_t)eEvent);

    /* params list is: application-registered cbParam, Event ID, and NULL */
    if(gpCallbackTable[eEvent].pfCallback != NULL)
    {
        gpCallbackTable[eEvent].pfCallback (gpCallbackTable[eEvent].pCBParam, (uint32_t) eEvent, NULL);
    }
}

/* strongly-bound interrupt handlers to override the default weak bindings */
void Ext_Int0_Handler(void)
{
    ISR_PROLOG()
    XIntCommonInterruptHandler(ADI_XINT_EVENT_INT0);
    ISR_EPILOG()
}

void Ext_Int1_Handler(void)
{
    ISR_PROLOG()
    XIntCommonInterruptHandler(ADI_XINT_EVENT_INT1);
    ISR_EPILOG()
}

void Ext_Int2_Handler(void)
{
    ISR_PROLOG()
    XIntCommonInterruptHandler(ADI_XINT_EVENT_INT2);
    ISR_EPILOG()

}

void Ext_Int3_Handler(void)
{
    ISR_PROLOG()
    if((pADI_XINT0->EXT_STAT & BITM_XINT_EXT_STAT_STAT_UART_RXWKUP)==BITM_XINT_EXT_STAT_STAT_UART_RXWKUP)
    {
       XIntCommonInterruptHandler(ADI_XINT_EVENT_UART_RX);
    }
    else
    {
       XIntCommonInterruptHandler(ADI_XINT_EVENT_INT3);
    }
    ISR_EPILOG()
}

/*! \endcond */

/*
** EOF
*/
