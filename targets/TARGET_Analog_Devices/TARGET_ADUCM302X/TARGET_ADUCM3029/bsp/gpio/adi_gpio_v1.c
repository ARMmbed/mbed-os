/*!
 *****************************************************************************
   @file:    adi_gpio_v1.c
   @brief:   GPIO device driver implementation for ADuC302X
   @version: $Revision: 33431 $
   @date:    $Date: 2016-02-05 06:29:28 +0000 (Fri, 05 Feb 2016) $
  -----------------------------------------------------------------------------

Copyright (c) 2014-2015 Analog Devices, Inc.

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


#ifndef _ADI_GPIO_V1_C_
/*! \cond PRIVATE */
#define _ADI_GPIO_V1_C_

#include <services/int/adi_int.h>
#include <stddef.h>
#include <assert.h>

#include <ssdd_common/common_def.h>
#include <services/gpio/adi_gpio.h>
#include "adi_gpio_def_v1.h"

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
* Pm140 (rule 11.3): a cast should not be performed between a pointer type and an integral type
*   The rule makes an exception for memory-mapped register accesses.
*/
#pragma diag_suppress=Pm123,Pm073,Pm143,Pm050,Pm140
#endif /* __ICCARM__ */

/* Debug function declarations */
#ifdef ADI_DEBUG

static bool_t IsPortValid       (const ADI_GPIO_PORT Port);                             /*!< tests for port validity */
static bool_t ArePinsValid      (const ADI_GPIO_PORT Port, const ADI_GPIO_DATA Pins);   /*!< tests for pins validity */

static bool_t IsInterruptValid  (const IRQn_Type eIrq);                                 /*!< tests for valid interrupt number */
static bool_t IsExInterruptValid(const IRQn_Type eIrq);                                 /*!< tests for valid external interrupt number */
static bool_t IsTriggerValid    (const ADI_GPIO_IRQ_TRIGGER_CONDITION eCondition);      /*!< tests for valid trigger condition */

#endif /* ADI_DEBUG */

/* forward declarations */

/* These are needed in the case where ADI_CFG_ENABLE_RTOS_SUPPORT=1              */
/* The ADI_INSTALL_HANDLER macro will expand to a call to adi_int_InstallHandler */
/* and the handler will be referenced                                            */
/* The non-OSAL expansion of ADI_INSTALL_HANDLER results in a NULL string        */
ADI_INT_HANDLER(Ext_Int0_Handler);
ADI_INT_HANDLER(Ext_Int1_Handler);
ADI_INT_HANDLER(Ext_Int2_Handler);
ADI_INT_HANDLER(Ext_Int3_Handler);
ADI_INT_HANDLER(GPIO_A_Int_Handler);
ADI_INT_HANDLER(GPIO_B_Int_Handler);

static void CommonInterruptHandler  (const ADI_GPIO_IRQ_INDEX index, const IRQn_Type eIrq);


/*==========  D A T A  ==========*/
static ADI_GPIO_DRIVER adi_gpio_Device =
{
    {
        pADI_GPIO0,       /* port 0 base address */
        pADI_GPIO1,       /* port 1 base address */
        pADI_GPIO2        /* port 2 base address */
    },
    NULL
};
/*! \endcond */

/*! \addtogroup GPIO_Service GPIO Service
 *  @{
 */

/*!
    @brief      Initializes the GPIO functions.

    @details    This function does the GPIO Initialization. This function need be called before calling any of the GPIO
                service APIs. This function installs the interrupt handlers for all external and Group A/B interrupts.

    @param[in] pMemory      Pointer to the memory to be used by the driver.
                            Size of the memory should be atleast #ADI_GPIO_MEMORY_SIZE bytes.
    @param[in] MemorySize   Size of the memory passed in pMemory parameter.

    @return     Status
                    - ADI_GPIO_SUCCESS if successfully initialized

    @note       This function clears memory reserved for managing the callback function when it is called for the first time.
                It is expected from user to call "adi_gpio_UnInit" function wehn the GPIO service is no longer requred.

    @sa         adi_gpio_UnInit
*/
ADI_GPIO_RESULT adi_gpio_Init(void*                const pMemory,
                              uint32_t             const MemorySize
)
{

    /* only initialize on 1st init call, i.e., preserve callbacks on multiple inits */
    if (adi_gpio_Device.pData == NULL)
    {
        /* in C89 the for loop counter declared outside the loop */
        uint32_t i;
#ifdef ADI_DEBUG
        assert(pMemory != NULL);
        assert(MemorySize == sizeof(ADI_GPIO_DEV_DATA));
#endif

        adi_gpio_Device.pData = (ADI_GPIO_DEV_DATA*)pMemory;

        /* (re)initialize the callback table */
        for (i = 0u; i < ADI_GPIO_NUM_INTERRUPTS; i++)
        {
            adi_gpio_Device.pData->CallbackTable[i].pfCallback = NULL;
            adi_gpio_Device.pData->CallbackTable[i].pCBParam   = NULL;
        }
        adi_gpio_Device.pData->ReferenceCount++;
        /* (re)install the handlers for external interrupts */
        ADI_INSTALL_HANDLER(XINT_EVT0_IRQn, Ext_Int0_Handler);
        ADI_INSTALL_HANDLER(XINT_EVT1_IRQn, Ext_Int1_Handler);
        ADI_INSTALL_HANDLER(XINT_EVT2_IRQn, Ext_Int2_Handler);
        ADI_INSTALL_HANDLER(XINT_EVT3_IRQn, Ext_Int3_Handler);

        /* (re)install handlers for group interrupts */
        ADI_INSTALL_HANDLER(SYS_GPIO_INTA_IRQn, GPIO_A_Int_Handler);
        ADI_INSTALL_HANDLER(SYS_GPIO_INTB_IRQn, GPIO_B_Int_Handler);
        ADI_ENABLE_INT(SYS_GPIO_INTA_IRQn);
        ADI_ENABLE_INT(SYS_GPIO_INTB_IRQn);
    }

    return (ADI_GPIO_SUCCESS);
}


/*!
    @brief      GPIO Uninitalization

    @details    Terminates the GPIO functions, leaving everything unchanged

    @return     Status
                    - #ADI_GPIO_SUCCESS if successfully uninitialized
                    - #ADI_GPIO_NOT_INITIALIZED [D] if not yet initialized

    @note       At some point in the future, this function may actually do something
                useful. For the time being, it's only functionality is to clear
                the flag that indicates the GPIOs have been initialized. In debug
                mode, all the GPIO functions test to make sure the GPIOs have been
                initialized properly so that eventually, when this function does
                do something useful, we can rely on the fact that existing software
                called this function and the adi_gpio_Init() function properly.

    @sa         adi_gpio_Init
*/
ADI_GPIO_RESULT adi_gpio_UnInit(void)
{

#ifdef ADI_DEBUG
    /* IF (not initialized) */
    if (0 == adi_gpio_Device.pData->ReferenceCount)
    {
        /* return error if not initialized */
        return (ADI_GPIO_NOT_INITIALIZED);
    }
    else
    {
        /* decrement the reference count */
        adi_gpio_Device.pData->ReferenceCount--;
    }
#endif

    return (ADI_GPIO_SUCCESS);
}


/*!
    @brief      Reset to Power Up Default Values

    @details    Sets all GPIO registers to their power-up default values

    @return     Status
                    - #ADI_GPIO_SUCCESS if successful
                    - #ADI_GPIO_NOT_INITIALIZED [D] if not yet initialized
*/
ADI_GPIO_RESULT adi_gpio_ResetToPowerUp(void)
{

    ADI_GPIO_TypeDef    *pPort; /* pointer to port registers */
    uint32_t            Port;   /* counter */

#ifdef ADI_DEBUG
    /* make sure we're initialized */
    if (adi_gpio_Device.pData->ReferenceCount == 0)
    {
        return (ADI_GPIO_NOT_INITIALIZED);
    }
#endif

    /* FOR (each port) */
    for (Port = ADI_GPIO_PORT0; Port < ADI_GPIO_NUM_PORTS; Port++)
    {
        /* point to the register block */
        pPort = adi_gpio_Device.pReg[Port];

        /* reset the registers */
        pPort->CFG  = ADI_GPIO_DEFAULT_CON;
        pPort->OEN  = ADI_GPIO_DEFAULT_OEN;
        pPort->PE   = ADI_GPIO_DEFAULT_PE;
        pPort->POL  = ADI_GPIO_DEFAULT_POL;
        pPort->IENA = ADI_GPIO_DEFAULT_ENA;
        pPort->IENB = ADI_GPIO_DEFAULT_ENB;
        pPort->INT  = ADI_GPIO_DEFAULT_STAT;
    }

    return (ADI_GPIO_SUCCESS);
}


/*!
    @brief      Enable an External Interrupt

    @details    Enables and sets the triggering mode for the given external interrupt.
                Applications may register a callback using the #adi_gpio_RegisterCallback
                API to get a notification when the interrupt occures.

    @param[in]  eIrq        Identifier of type IRQn_Type that specifies the external
                            interrupt that is to be enabled
    @param[in]  eCondition  Identifier of type ADI_GPIO_IRQ_TRIGGER_CONDITION
                            that specifies the trigger condition for the interrupt

    @return     Status
                    - #ADI_GPIO_SUCCESS if successfully enabled
                    - #ADI_GPIO_NOT_INITIALIZED [D] if not yet initialized
                    - #ADI_GPIO_INVALID_INTERRUPT [D] if the eIrq parameter is invalid
                    - #ADI_GPIO_INVALID_TRIGGER [D] if the eCondition parameter is invalid

    @sa         adi_gpio_DisableExIRQ
    @sa         adi_gpio_RegisterCallback
*/
ADI_GPIO_RESULT adi_gpio_EnableExIRQ(const IRQn_Type eIrq, const ADI_GPIO_IRQ_TRIGGER_CONDITION eCondition)
{

    uint32_t volatile   *pIRQCFG;   /* pointer to the appropriate interrupt config register */
    uint32_t            Mask;       /* mask to manipulate the register */
    uint32_t            Pattern;    /* bit pattern that will be written into the register */
    uint32_t            ShiftCount = 0u; /* shift count */
    uint32_t            CfgReg;     /* interrupt config register value */

#ifdef ADI_DEBUG
    /* make sure we're initialized */
    if (adi_gpio_Device.pData->ReferenceCount == 0)
    {
        return (ADI_GPIO_NOT_INITIALIZED);
    }
    /* validate the trigger condition */
    if (!IsTriggerValid(eCondition))
    {
        return (ADI_GPIO_INVALID_TRIGGER);
    }
    if (!IsExInterruptValid(eIrq))
    {
        return (ADI_GPIO_INVALID_INTERRUPT);
    }
#endif

    /* figure out the shift count and which config register we need to change */
    switch (eIrq)
    {
        case XINT_EVT0_IRQn:
        case XINT_EVT1_IRQn:
        case XINT_EVT2_IRQn:
        case XINT_EVT3_IRQn:
            /* external interrupts 0-3 */
            pIRQCFG = &pADI_XINT0->CFG0;
            ShiftCount = ((uint32_t)eIrq - (uint32_t)XINT_EVT0_IRQn) << ADI_GPIO_EIxCFG_SHIFT_COUNT;
            break;
        default:
            break; /* This shall never reach */
    }

    /* create the mask we'll use to clear the relevant bits in the config register */
    Mask = (BITM_XINT_CFG0_IRQ0MDE | BITM_XINT_CFG0_IRQ0EN) << ShiftCount;

    /* create the bit pattern we're going to write into the configuration register */
    Pattern = (BITM_XINT_CFG0_IRQ0EN | eCondition) << ShiftCount;

    ADI_ENTER_CRITICAL_REGION();
    /* read/modify/write the appropriate bits in the register */
    CfgReg = *pIRQCFG;
    CfgReg &= ~Mask;
    CfgReg |= Pattern;
    *pIRQCFG = CfgReg;
    ADI_EXIT_CRITICAL_REGION();

    /* enable the interrupt */
    NVIC_EnableIRQ(eIrq);

    return (ADI_GPIO_SUCCESS);
}


/*!
    @brief      Disable an External Interrupt

    @details    Disables an external interrupt

    @param[in]  eIrq    Identifier of type IRQn_Type that specifies the external
                        interrupt that is to be disabled

    @return     Status
                    - #ADI_GPIO_SUCCESS if successfully disabled
                    - #ADI_GPIO_NOT_INITIALIZED [D] if not yet initialized
                    - #ADI_GPIO_INVALID_INTERRUPT [D] if the eIrq parameter is invalid

    @sa         adi_gpio_EnableExIRQ
    @sa         adi_gpio_RegisterCallback
*/
ADI_GPIO_RESULT adi_gpio_DisableExIRQ(const IRQn_Type eIrq)
{

    uint32_t volatile   *pIRQCFG;   /* pointer to the appropriate interrupt config register */
    uint32_t            Mask;       /* mask to manipulate the register */
    uint32_t            ShiftCount = 0u; /* shift count */
    uint32_t            CfgReg;     /* interrupt config register value */

#ifdef ADI_DEBUG
    /* make sure we're initialized */
    if (adi_gpio_Device.pData->ReferenceCount == 0)
    {
        return (ADI_GPIO_NOT_INITIALIZED);
    }
    if (!IsExInterruptValid(eIrq))
    {
        return (ADI_GPIO_INVALID_INTERRUPT);
    }
#endif

    /* disable the interrupt */
    NVIC_DisableIRQ(eIrq);

    /* figure out the shift count and which config register we need to change */
    switch (eIrq) {
        case XINT_EVT0_IRQn:
        case XINT_EVT1_IRQn:
        case XINT_EVT2_IRQn:
        case XINT_EVT3_IRQn:
            /* external interrupts 0-3 */
            pIRQCFG = &pADI_XINT0->CFG0;
            ShiftCount = ((uint32_t)eIrq - (uint32_t)XINT_EVT0_IRQn) << ADI_GPIO_EIxCFG_SHIFT_COUNT;
            break;
        default:
            break; /* This shall never reach */
    }

    /* create the mask we'll use to clear the interrupt enable bit in the config register */
    Mask = BITM_XINT_CFG0_IRQ0EN << ShiftCount;

     ADI_ENTER_CRITICAL_REGION();
    /* read/modify/write the appropriate bits in the register */
    CfgReg = *pIRQCFG;
    CfgReg &= ~Mask;
    *pIRQCFG = CfgReg;
    ADI_EXIT_CRITICAL_REGION();

    return (ADI_GPIO_SUCCESS);
}


/*!
    @brief      Write a port's pin interrupt Group A/B mask register.

    @details    Writes a GPIO port GPIO Group A/B mask register with /a Pins.  No bitwise operations are performed.
                Applications may register/unregister a callback using the #adi_gpio_RegisterCallback API
                to get a notification when the group interrupt occurs.


    @param[in]  Port   Identifies the GPIO port to be operated on
    @param[in]  eIrq   Identifies the interrupt (Group A/B) with which the pin(s) are to be grouped.
    @param[in]  Pins   Identifies the GPIO port pins to be set in the port group mask register:
                           Pin bits that are set enable the interrupt for the group A/B.
                           Pin bits that are clear disable the interrupt for the group A/B.

    @return     Status
                    - #ADI_GPIO_SUCCESS if successfully disabled
                    - #ADI_GPIO_NOT_INITIALIZED [D] if not yet initialized
                    - #ADI_GPIO_INVALID_INTERRUPT [D] invalid pin interrupt group
                    - #ADI_GPIO_INVALID_PORT [D] invalid port
                    - #ADI_GPIO_INVALID_PINS [D] invalid pins

    @sa         adi_gpio_RegisterCallback
    @sa         adi_gpio_GetGroupInterruptPins
    @sa         adi_gpio_SetGroupInterruptPolarity
    @sa         adi_gpio_GetGroupInterruptPolarity
*/
ADI_GPIO_RESULT adi_gpio_SetGroupInterruptPins(const ADI_GPIO_PORT Port, const IRQn_Type eIrq, const ADI_GPIO_DATA Pins)
{
    ADI_GPIO_TypeDef    *pPort;     /* pointer to port registers */

#ifdef ADI_DEBUG
    /* make sure we're initialized */
    if (adi_gpio_Device.pData->ReferenceCount == 0)
    {
        return (ADI_GPIO_NOT_INITIALIZED);
    }
    /* validate the port */
    if (!IsPortValid(Port))
    {
        return (ADI_GPIO_INVALID_PORT);
    }
    /* validate the pins */
    if (!ArePinsValid(Port, Pins))
    {
        return (ADI_GPIO_INVALID_PINS);
    }
    /* validate the eIrq */
    if((eIrq != SYS_GPIO_INTA_IRQn) && (eIrq != SYS_GPIO_INTB_IRQn))
    {
        return (ADI_GPIO_INVALID_INTERRUPT);
    }
#endif

    pPort = adi_gpio_Device.pReg[Port];

    ADI_ENTER_CRITICAL_REGION();
    switch (eIrq)
    {
        case SYS_GPIO_INTA_IRQn:
            pPort->IENA = Pins;
            break;
        case SYS_GPIO_INTB_IRQn:
            pPort->IENB = Pins;
            break;
        default:
            break; /* This shall never reach */
    }
    ADI_EXIT_CRITICAL_REGION();

    return (ADI_GPIO_SUCCESS);
}


/*!
    @brief      Get a port's pin interrupt Group A/B mask register

    @details    Returns a GPIO port GPIO Group A/B mask register.

    @param[in]  Port   Identifies the GPIO port to be operated on.
    @param[in]  eIrq   Identifies the Group A/B interrupt to query.
    @param[in]  Pins   Identifies the GPIO port pins.
    @param[out] pValue Returned Pin state for GPIO port pins.
                        - Pin bits that are set are enabled for group A/B interrupting.
                        - Pin bits that are clear are disabled for group A/B interrupting.

    @return     Status
                    - #ADI_GPIO_SUCCESS if successfully disabled
                    - #ADI_GPIO_NOT_INITIALIZED [D] if not yet initialized
                    - #ADI_GPIO_INVALID_INTERRUPT [D] invalid pin interrupt group
                    - #ADI_GPIO_INVALID_PORT [D] invalid port

    @sa         adi_gpio_SetGroupInterruptPins
    @sa         adi_gpio_SetGroupInterruptPolarity
    @sa         adi_gpio_GetGroupInterruptPolarity
*/
ADI_GPIO_RESULT adi_gpio_GetGroupInterruptPins(const ADI_GPIO_PORT Port, const IRQn_Type eIrq, const ADI_GPIO_DATA Pins, uint16_t* const pValue)
{
    ADI_GPIO_TypeDef    *pPort;     /* pointer to port registers */
    uint16_t            Value = 0u;

#ifdef ADI_DEBUG
    /* make sure we're initialized */
    if (adi_gpio_Device.pData->ReferenceCount == 0)
    {
        return (ADI_GPIO_NOT_INITIALIZED);
    }
    /* validate the port */
    if (!IsPortValid(Port))
    {
        return (ADI_GPIO_INVALID_PORT);
    }
    /* validate the eIrq */
    if((eIrq != SYS_GPIO_INTA_IRQn) && (eIrq != SYS_GPIO_INTB_IRQn))
    {
        return (ADI_GPIO_INVALID_INTERRUPT);
    }
    /* validate the pins */
    if (!ArePinsValid(Port, Pins))
    {
        return (ADI_GPIO_INVALID_PINS);
    }
#endif

    pPort = adi_gpio_Device.pReg[Port];

    switch (eIrq)
    {
        case SYS_GPIO_INTA_IRQn:
            Value = pPort->IENA;
            ADI_ENABLE_INT(SYS_GPIO_INTA_IRQn);
            break;
        case SYS_GPIO_INTB_IRQn:
            Value = pPort->IENB;
            ADI_ENABLE_INT(SYS_GPIO_INTB_IRQn);

            break;
        default:
            break; /* This shall never reach */
    }

    *pValue = (Value & Pins);
    return (ADI_GPIO_SUCCESS);
}


/*!
    @brief      Set a port's pin interrupt polarity register

    @details    Writes a GPIO pin interrupt polarity register.
                Set bits configure interrupt capture on low-to-high transitions, clear bits configure interrupt capture on high-to-low transitions.
                No bitwise operations are performed.

    @param[in]  Port   Identifies the GPIO port to be operated on
    @param[in]  Pins   Identifies the GPIO port pins to be set in the interrupt polarity control register

    @return     Status
                    - #ADI_GPIO_SUCCESS if successfully disabled
                    - #ADI_GPIO_NOT_INITIALIZED [D] if not yet initialized
                    - #ADI_GPIO_INVALID_PORT [D] invalid port
                    - #ADI_GPIO_INVALID_PINS [D] invalid pins

    @sa         adi_gpio_RegisterCallback
    @sa         adi_gpio_SetGroupInterruptPins
    @sa         adi_gpio_GetGroupInterruptPins
    @sa         adi_gpio_GetGroupInterruptPolarity
*/
ADI_GPIO_RESULT adi_gpio_SetGroupInterruptPolarity(const ADI_GPIO_PORT Port, const ADI_GPIO_DATA Pins)
{
    ADI_GPIO_TypeDef    *pPort;     /* pointer to port registers */

#ifdef ADI_DEBUG
    /* make sure we're initialized */
    if (adi_gpio_Device.pData->ReferenceCount == 0)
    {
        return (ADI_GPIO_NOT_INITIALIZED);
    }
    /* validate the port */
    if (!IsPortValid(Port))
    {
        return (ADI_GPIO_INVALID_PORT);
    }
    /* validate the pins */
    if (!ArePinsValid(Port, Pins))
    {
        return (ADI_GPIO_INVALID_PINS);
    }
#endif

    pPort = adi_gpio_Device.pReg[Port];

    pPort->POL = Pins;

    return (ADI_GPIO_SUCCESS);
}


/*!
    @brief      Get a port's pin interrupt polarity register

    @details    Read a GPIO pin interrupt polarity register.
                Set bits configure interrupt capture on low-to-high transitions, clear bits configure interrupt capture on high-to-low transitions.

    @param[in]  Port   Identifies the GPIO port to be operated on
    @param[in]  Pins   Identifies the GPIO port pins.
    @param[out] pValue Returned Interrupt polarity for GPIO port pins.

    @return     Status
                    - #ADI_GPIO_SUCCESS if successfully disabled
                    - #ADI_GPIO_NOT_INITIALIZED [D] if not yet initialized
                    - #ADI_GPIO_INVALID_PORT [D] invalid port

    @sa         adi_gpio_RegisterCallback
    @sa         adi_gpio_SetGroupInterruptPins
    @sa         adi_gpio_GetGroupInterruptPins
    @sa         adi_gpio_SetGroupInterruptPolarity
*/
ADI_GPIO_RESULT adi_gpio_GetGroupInterruptPolarity(const ADI_GPIO_PORT Port, const ADI_GPIO_DATA Pins, uint16_t* const pValue)
{
    ADI_GPIO_TypeDef    *pPort;     /* pointer to port registers */

#ifdef ADI_DEBUG
    /* make sure we're initialized */
    if (adi_gpio_Device.pData->ReferenceCount == 0)
    {
        return (ADI_GPIO_NOT_INITIALIZED);
    }
    /* validate the port */
    if (!IsPortValid(Port))
    {
        return (ADI_GPIO_INVALID_PORT);
    }
    /* validate the pins */
    if (!ArePinsValid(Port, Pins))
    {
        return (ADI_GPIO_INVALID_PINS);
    }
#endif

    pPort = adi_gpio_Device.pReg[Port];

    *pValue = (pPort->POL & Pins);

    return (ADI_GPIO_SUCCESS);
}


/* TODO: Not required */
/*!
    @brief      Get a port's pin interrupt status register

    @details    Returns a GPIO port's interrupt status register.

    @param[in]  Port   Identifies the GPIO port to be operated on
    @param[in]  Pins   Identifies the GPIO port pins.
    @param[out] pValue Returned interrupt status for GPIO port pins.

    @return     Status
                    - #ADI_GPIO_SUCCESS if successfully disabled
                    - #ADI_GPIO_NOT_INITIALIZED [D] if not yet initialized
                    - #ADI_GPIO_INVALID_PORT [D] invalid port

    @sa         adi_gpio_RegisterCallback
    @sa         adi_gpio_SetGroupInterruptPins
    @sa         adi_gpio_GetGroupInterruptPins
    @sa         adi_gpio_SetGroupInterruptPolarity
    @sa         adi_gpio_GetGroupInterruptPolarity
*/
ADI_GPIO_RESULT adi_gpio_GetGroupInterruptStatus(const ADI_GPIO_PORT Port, const ADI_GPIO_DATA Pins, uint16_t* const pValue)
{
    ADI_GPIO_TypeDef    *pPort;     /* pointer to port registers */

#ifdef ADI_DEBUG
    /* make sure we're initialized */
    if (adi_gpio_Device.pData->ReferenceCount == 0)
    {
        return (ADI_GPIO_NOT_INITIALIZED);
    }
    /* validate the port */
    if (!IsPortValid(Port))
    {
        return (ADI_GPIO_INVALID_PORT);
    }
    /* validate the pins */
    if (!ArePinsValid(Port, Pins))
    {
        return (ADI_GPIO_INVALID_PINS);
    }
#endif

    pPort = adi_gpio_Device.pReg[Port];

    *pValue = (pPort->INT) & Pins;

    return (ADI_GPIO_SUCCESS);

}


/* TODO: Not required */
/*!
    @brief      Clear a port's pin interrupt status register

    @details    Clear the /a Pins in a GPIO port's interrupt status register.

    @param[in]  Port   Identifies the GPIO port to be operated on
    @param[in]  Pins   Identifies the GPIO port pins to be cleared in the interrupt status register

    @return     Status
                    - #ADI_GPIO_SUCCESS if successfully disabled
                    - #ADI_GPIO_NOT_INITIALIZED [D] if not yet initialized
                    - #ADI_GPIO_INVALID_PORT [D] invalid port
                    - #ADI_GPIO_INVALID_PINS [D] invalid pins

    @sa         adi_gpio_RegisterCallback
    @sa         adi_gpio_SetGroupInterruptPins
    @sa         adi_gpio_GetGroupInterruptPins
    @sa         adi_gpio_SetGroupInterruptPolarity
    @sa         adi_gpio_GetGroupInterruptPolarity
*/
ADI_GPIO_RESULT adi_gpio_ClrGroupInterruptStatus(const ADI_GPIO_PORT Port, const ADI_GPIO_DATA Pins)
{
    ADI_GPIO_TypeDef    *pPort;     /* pointer to port registers */

#ifdef ADI_DEBUG
    /* make sure we're initialized */
    if (adi_gpio_Device.pData->ReferenceCount == 0)
    {
        return (ADI_GPIO_NOT_INITIALIZED);
    }
    /* validate the port */
    if (!IsPortValid(Port))
    {
        return (ADI_GPIO_INVALID_PORT);
    }
    /* validate the pins */
    if (!ArePinsValid(Port, Pins))
    {
        return (ADI_GPIO_INVALID_PINS);
    }
#endif

    pPort = adi_gpio_Device.pReg[Port];

    pPort->INT = Pins;

    return (ADI_GPIO_SUCCESS);
}


/*!
    @brief      Enables/Disables the Output Drivers for GPIO Pin(s)

    @details    Enables/disables the output drivers for the given GPIO pin(s) on
                the given port.

    @param[in]  Port    Identifies the GPIO port to be configured (ADI_GPIO_PORTx)
    @param[in]  Pins    Identifies one or more GPIO pins to be configured. GPIO
                        pins can be passed one at a time or in combination. To
                        configure a single GPIO pin, a single GPIO value is
                        passed for this parameter. For example, #ADI_GPIO_PIN_4.
                        Alternatively, multiple GPIO pins can be configured
                        simultaneously by OR-ing together GPIO pin values and
                        passing the resulting value for this parameter. For
                        example, #ADI_GPIO_PIN_0 | #ADI_GPIO_PIN_5 | #ADI_GPIO_PIN_6.
    @param[in]  bFlag   Boolean value describing the action to be taken
                            - true      enables the output driver
                            - false     disables the output driver

    @return     Status
                    - #ADI_GPIO_SUCCESS if successfully configured
                    - #ADI_GPIO_NOT_INITIALIZED [D] if not yet initialized
                    - #ADI_GPIO_INVALID_PORT [D] if the Port parameter is invalid
                    - #ADI_GPIO_INVALID_PINS [D] if the Pins parameter is invalid

    @sa         adi_gpio_GetOutputEnable
*/
ADI_GPIO_RESULT adi_gpio_OutputEnable(const ADI_GPIO_PORT Port, const ADI_GPIO_DATA Pins, const bool_t bFlag)
{

    ADI_GPIO_TypeDef    *pPort;     /* pointer to port registers */

#ifdef ADI_DEBUG
    /* make sure we're initialized */
    if (adi_gpio_Device.pData->ReferenceCount == 0)
    {
        return (ADI_GPIO_NOT_INITIALIZED);
    }
    /* validate the port */
    if (!IsPortValid(Port))
    {
        return (ADI_GPIO_INVALID_PORT);
    }
    /* validate the pins */
    if (!ArePinsValid(Port, Pins))
    {
        return (ADI_GPIO_INVALID_PINS);
    }
#endif

    pPort = adi_gpio_Device.pReg[Port];

    ADI_ENTER_CRITICAL_REGION();
    if (bFlag)
    {
        /* enable output */
        pPort->OEN |= Pins;
    } else
    {
        /* disable output */
        pPort->OEN &= (uint16_t)~Pins;
    }
    ADI_EXIT_CRITICAL_REGION();

    return (ADI_GPIO_SUCCESS);
}


/*!
    @brief      Enables/Disables the Input Drivers for GPIO Pin(s)

    @details    Enables/disables the input drivers for the given GPIO pin(s) on
                the given port.

    @param[in]  Port    Identifies the GPIO port to be configured (ADI_GPIO_PORTx)
    @param[in]  Pins    Identifies one or more GPIO pins to be configured. GPIO
                        pins can be passed one at a time or in combination. To
                        configure a single GPIO pin, a single GPIO value is
                        passed for this parameter. For example, #ADI_GPIO_PIN_4.
                        Alternatively, multiple GPIO pins can be configured
                        simultaneously by OR-ing together GPIO pin values and
                        passing the resulting value for this parameter. For
                        example, #ADI_GPIO_PIN_0 | #ADI_GPIO_PIN_5 | #ADI_GPIO_PIN_6.

    @param[in]  bFlag   Boolean value describing the action to be taken
                            - true      enables the input driver
                            - false     disables the input driver

    @return     Status
                    - #ADI_GPIO_SUCCESS if successfully configured
                    - #ADI_GPIO_NOT_INITIALIZED [D] if not yet initialized
                    - #ADI_GPIO_INVALID_PORT [D] if the Port parameter is invalid
                    - #ADI_GPIO_INVALID_PINS [D] if the Pins parameter is invalid

    @sa         adi_gpio_GetInputEnable
*/
ADI_GPIO_RESULT adi_gpio_InputEnable(const ADI_GPIO_PORT Port, const ADI_GPIO_DATA Pins, const bool_t bFlag)
{

    ADI_GPIO_TypeDef    *pPort;     /* pointer to port registers */

#ifdef ADI_DEBUG
    /* make sure we're initialized */
    if (adi_gpio_Device.pData->ReferenceCount == 0) {
        return (ADI_GPIO_NOT_INITIALIZED);
    }
    /* validate the port */
    if (!IsPortValid(Port)) {
        return (ADI_GPIO_INVALID_PORT);
    }
    /* validate the pins */
    if (!ArePinsValid(Port, Pins)) {
        return (ADI_GPIO_INVALID_PINS);
    }
#endif

    pPort = adi_gpio_Device.pReg[Port];

    ADI_ENTER_CRITICAL_REGION();
    if (bFlag)
    {
        /* enable input */
        pPort->IEN |= Pins;
    } else
    {
        /* disable input */
        pPort->IEN &= (uint16_t)~Pins;
    }
    ADI_EXIT_CRITICAL_REGION();

    return (ADI_GPIO_SUCCESS);
}


/*!
    @brief      Enables/Disables the Pull-Up for GPIO Pin(s)

    @details    Enables/disables the internal pull-up for the given GPIO pin(s) on
                the given port.  API simply enables/disables whatever the hard-wired
                pulls (up/down) are.

    @param[in]  Port    Identifies the GPIO port to be configured (ADI_GPIO_PORTx)
    @param[in]  Pins    Identifies one or more GPIO pins to be configured. GPIO
                        pins can be passed one at a time or in combination. To
                        configure a single GPIO pin, a single GPIO value is
                        passed for this parameter. For example, #ADI_GPIO_PIN_4.
                        Alternatively, multiple GPIO pins can be configured
                        simultaneously by OR-ing together GPIO pin values and
                        passing the resulting value for this parameter. For
                        example, #ADI_GPIO_PIN_0 | #ADI_GPIO_PIN_5 | #ADI_GPIO_PIN_6.
    @param[in]  bFlag   Boolean value describing the action to be taken
                            - true      enables the pull-up
                            - false     disables the pull-up

    @return     Status
                    - #ADI_GPIO_SUCCESS if successfully configured
                    - #ADI_GPIO_NOT_INITIALIZED [D] if not yet initialized
                    - #ADI_GPIO_INVALID_PORT [D] if the Port parameter is invalid
                    - #ADI_GPIO_INVALID_PINS [D] if the Pins parameter is invalid

    @sa         adi_gpio_GetPullupEnable
*/
ADI_GPIO_RESULT adi_gpio_PullUpEnable(const ADI_GPIO_PORT Port, const ADI_GPIO_DATA Pins, const bool_t bFlag)
{

    ADI_GPIO_TypeDef    *pPort;     /* pointer to port registers */

#ifdef ADI_DEBUG
    /* make sure we're initialized */
    if (adi_gpio_Device.pData->ReferenceCount == 0)
    {
        return (ADI_GPIO_NOT_INITIALIZED);
    }
    /* validate the port */
    if (!IsPortValid(Port))
    {
        return (ADI_GPIO_INVALID_PORT);
    }
    /* validate the pins */
    if (!ArePinsValid(Port, Pins))
    {
        return (ADI_GPIO_INVALID_PINS);
    }
#endif

    pPort = adi_gpio_Device.pReg[Port];

    ADI_ENTER_CRITICAL_REGION();
    if (bFlag)
    {
        pPort->PE |= Pins;
    } else
    {
        pPort->PE &= (uint16_t)(~Pins);
    }
    ADI_EXIT_CRITICAL_REGION();

    return (ADI_GPIO_SUCCESS);
}


/*!
    @brief      Gets the Output Driver Status for GPIO Pin(s)

    @details    Gets the output driver status for the GPIO pins on the given
                port.

    @param[in]  Port    Identifies the GPIO port to be sensed (ADI_GPIO_PORTx)
    @param[in]  Pins    Identifies the GPIO port pins.
    @param[out] pValue  The passed pValue parameter is written with a packed value containing
                        the status of all the requested GPIO pins on the given port.

                        To get the status of a single GPIO pin, return value can be direcly used.
                        For example to see if pin 4 on port 2 has output enabled, the following is used:
                            adi_gpio_GetOutputEnable(#ADI_GPIO_PORT2, #ADI_GPIO_PIN_4, &pValue)
                        pValue will contain the required information.

                        If information is required for multiple pins, following method is required:
                            adi_gpio_GetOutputEnable(#ADI_GPIO_PORT2, (#ADI_GPIO_PIN_3 | #ADI_GPIO_PIN_4 | #ADI_GPIO_PIN_7), &pValue)
                        To test if pin 4 on port 2 has output enabled, the following is used:
                            if  (pValue & ADU_GPIO_PIN_4) {
                                the output driver is enabled for pin 4 on port 2
                            } else {
                                the output driver is disabled for pin 4 on port 2
                            }

    @return     Status
                    - #ADI_GPIO_SUCCESS if successfully configured
                    - #ADI_GPIO_NOT_INITIALIZED [D] if not yet initialized
                    - #ADI_GPIO_INVALID_PORT [D] if the Port parameter is invalid

    @sa         adi_gpio_OutputEnable
*/
ADI_GPIO_RESULT adi_gpio_GetOutputEnable(const ADI_GPIO_PORT Port, const ADI_GPIO_DATA Pins, uint16_t* const pValue)
{

    ADI_GPIO_TypeDef    *pPort;     /* pointer to port registers */

#ifdef ADI_DEBUG
    /* make sure we're initialized */
    if (adi_gpio_Device.pData->ReferenceCount == 0)
    {
        return (ADI_GPIO_NOT_INITIALIZED);
    }
    /* validate the port */
    if (!IsPortValid(Port))
    {
        return (ADI_GPIO_INVALID_PORT);
    }
    /* validate the pins */
    if (!ArePinsValid(Port, Pins))
    {
        return (ADI_GPIO_INVALID_PINS);
    }
#endif

    pPort = adi_gpio_Device.pReg[Port];

    *pValue = (pPort->OEN) & Pins;

    return (ADI_GPIO_SUCCESS);
}


/*!

    @brief      Gets the Input Driver Status for GPIO Pin(s)

    @details    Gets the input driver status for the GPIO pins on the given
                port.

    @param[in]  Port    Identifies the GPIO port to be sensed (ADI_GPIO_PORTx)
    @param[in]  Pins    Identifies the GPIO port pins.
    @param[out] pValue  The passed pValue parameter is written with a packed value containing
                        the status of all the requested GPIO pins on the given port.

                        To get the status of a single GPIO pin, return value can be direcly used.
                        For example to see if pin 4 on port 2 has input enabled, the following is used:
                            adi_gpio_GetOutputEnable(#ADI_GPIO_PORT2, #ADI_GPIO_PIN_4, &pValue)
                        pValue will contain the required information.

                        If information is required for multiple pins, following method is required:
                            adi_gpio_GetOutputEnable(#ADI_GPIO_PORT2, (#ADI_GPIO_PIN_3 | #ADI_GPIO_PIN_4 | #ADI_GPIO_PIN_7), &pValue)
                        To test if pin 4 on port 2 has input enabled, the following is used:
                            if  (pValue & ADU_GPIO_PIN_4) {
                                the input driver is enabled for pin 4 on port 2
                            } else {
                                the input driver is disabled for pin 4 on port 2
                            }

    @return     Status
                    - #ADI_GPIO_SUCCESS if successfully configured
                    - #ADI_GPIO_NOT_INITIALIZED [D] if not yet initialized
                    - #ADI_GPIO_INVALID_PORT [D] if the Port parameter is invalid

    @sa         adi_gpio_InputEnable
*/
 ADI_GPIO_RESULT adi_gpio_GetInputEnable(const ADI_GPIO_PORT Port, const ADI_GPIO_DATA Pins, uint16_t* const pValue)
{

    ADI_GPIO_TypeDef    *pPort;     /* pointer to port registers */

#ifdef ADI_DEBUG
    /* make sure we're initialized */
    if (adi_gpio_Device.pData->ReferenceCount == 0)
    {
        return (ADI_GPIO_NOT_INITIALIZED);
    }
    /* validate the port */
    if (!IsPortValid(Port))
    {
        return (ADI_GPIO_INVALID_PORT);
    }
    /* validate the pins */
    if (!ArePinsValid(Port, Pins))
    {
        return (ADI_GPIO_INVALID_PINS);
    }
#endif

    pPort = adi_gpio_Device.pReg[Port];

    *pValue = (pPort->IEN) & Pins;

    return (ADI_GPIO_SUCCESS);
}


/*!

    @brief      Gets the Pull-Up Status for GPIO Pin(s)

    @details    Gets the pull-up status for the GPIO pins on the given port.
                Simply gets the hard-wired pull enable.

    @param[in]  Port    Identifies the GPIO port to be sensed (ADI_GPIO_PORTx)
    @param[in]  Pins    Identifies the GPIO port pins.
    @param[out] pValue  The passed pValue parameter is written with a packed value containing
                        the status of all the requested GPIO pins on the given port.

                        To get the status of a single GPIO pin, return value can be direcly used.
                        For example to see if pin 4 on port 2 has Pull-Up enabled, the following is used:
                            adi_gpio_GetOutputEnable(#ADI_GPIO_PORT2, #ADI_GPIO_PIN_4, &pValue)
                        pValue will contain the required information.

                        If information is required for multiple pins, following method is required:
                            adi_gpio_GetOutputEnable(#ADI_GPIO_PORT2, (#ADI_GPIO_PIN_3 | #ADI_GPIO_PIN_4 | #ADI_GPIO_PIN_7), &pValue)
                        To test if pin 4 on port 2 has Pull-Up enabled, the following is used:
                            if  (pValue & ADU_GPIO_PIN_4) {
                                the pull-up is enabled for pin 4 on port 2
                            } else {
                                the pull-up is enabled for pin 4 on port 2
                            }

    @return     Status
                    - #ADI_GPIO_SUCCESS if successfully configured
                    - #ADI_GPIO_NOT_INITIALIZED [D] if not yet initialized
                    - #ADI_GPIO_INVALID_PORT [D] if the Port parameter is invalid

    @sa         adi_gpio_PullUpEnable
*/
ADI_GPIO_RESULT adi_gpio_GetPullUpEnable(const ADI_GPIO_PORT Port, const ADI_GPIO_DATA Pins, uint16_t* const pValue)
{

    ADI_GPIO_TypeDef    *pPort;     /* pointer to port registers */

#ifdef ADI_DEBUG
    /* make sure we're initialized */
    if (adi_gpio_Device.pData->ReferenceCount == 0)
    {
        return (ADI_GPIO_NOT_INITIALIZED);
    }
    /* validate the port */
    if (!IsPortValid(Port))
    {
        return (ADI_GPIO_INVALID_PORT);
    }
    /* validate the pins */
    if (!ArePinsValid(Port, Pins))
    {
        return (ADI_GPIO_INVALID_PINS);
    }
#endif

    pPort = adi_gpio_Device.pReg[Port];

    *pValue = (pPort->PE) & Pins;

    return (ADI_GPIO_SUCCESS);
}


/*!

    @brief      Sets the Given GPIO pin(s) to a Logical High Level

    @details    Sets the given GPIO pin(s) on the given port to a logical high
                level.

    @param[in]  Port    Identifies the GPIO port to be configured (ADI_GPIO_PORTx)
    @param[in]  Pins    Identifies one or more GPIO pins to be configured. GPIO
                        pins can be passed one at a time or in combination. To
                        configure a single GPIO pin, a single GPIO value is
                        passed for this parameter. For example, #ADI_GPIO_PIN_4.
                        Alternatively, multiple GPIO pins can be configured
                        simultaneously by OR-ing together GPIO pin values and
                        passing the resulting value for this parameter. For
                        example, #ADI_GPIO_PIN_0 | #ADI_GPIO_PIN_5 | #ADI_GPIO_PIN_6.

    @return     Status
                    - #ADI_GPIO_SUCCESS if successfully configured
                    - #ADI_GPIO_NOT_INITIALIZED [D] if not yet initialized
                    - #ADI_GPIO_INVALID_PORT [D] if the Port parameter is invalid
                    - #ADI_GPIO_INVALID_PINS [D] if the Pins parameter is invalid

    @sa         adi_gpio_SetLow, adi_gpio_Toggle, adi_gpio_SetData, adi_gpio_GetData
*/
ADI_GPIO_RESULT adi_gpio_SetHigh(const ADI_GPIO_PORT Port, const ADI_GPIO_DATA Pins)
{

    ADI_GPIO_TypeDef    *pPort;     /* pointer to port registers */

#ifdef ADI_DEBUG
    /* make sure we're initialized */
    if (adi_gpio_Device.pData->ReferenceCount == 0)
    {
        return (ADI_GPIO_NOT_INITIALIZED);
    }
    /* validate the port */
    if (!IsPortValid(Port))
    {
        return (ADI_GPIO_INVALID_PORT);
    }
    /* validate the pins */
    if (!ArePinsValid(Port, Pins))
    {
        return (ADI_GPIO_INVALID_PINS);
    }
#endif

    pPort = adi_gpio_Device.pReg[Port];

    /* set the given GPIOs high */
    pPort->SET = Pins;

    return (ADI_GPIO_SUCCESS);
}


/*!

    @brief      Sets the Given GPIO pin(s) to a Logical Low Level

    @details    Sets the given GPIO pin(s) on the given port to a logical low
                level.

    @param[in]  Port    Identifies the GPIO port to be configured (ADI_GPIO_PORTx)
    @param[in]  Pins    Identifies one or more GPIO pins to be configured. GPIO
                        pins can be passed one at a time or in combination. To
                        configure a single GPIO pin, a single GPIO value is
                        passed for this parameter. For example, #ADI_GPIO_PIN_4.
                        Alternatively, multiple GPIO pins can be configured
                        simultaneously by OR-ing together GPIO pin values and
                        passing the resulting value for this parameter. For
                        example, #ADI_GPIO_PIN_0 | #ADI_GPIO_PIN_5 | #ADI_GPIO_PIN_6.

    @return     Status
                    - #ADI_GPIO_SUCCESS if successfully configured
                    - #ADI_GPIO_NOT_INITIALIZED [D] if not yet initialized
                    - #ADI_GPIO_INVALID_PORT [D] if the Port parameter is invalid
                    - #ADI_GPIO_INVALID_PINS [D] if the Pins parameter is invalid

    @sa         adi_gpio_SetHigh, adi_gpio_Toggle, adi_gpio_SetData, adi_gpio_GetData
*/
ADI_GPIO_RESULT adi_gpio_SetLow(const ADI_GPIO_PORT Port, const ADI_GPIO_DATA Pins)
{

    ADI_GPIO_TypeDef    *pPort;     /* pointer to port registers */

#ifdef ADI_DEBUG
    /* make sure we're initialized */
    if (adi_gpio_Device.pData->ReferenceCount == 0)
    {
        return (ADI_GPIO_NOT_INITIALIZED);
    }
    /* validate the port */
    if (!IsPortValid(Port))
    {
        return (ADI_GPIO_INVALID_PORT);
    }
    /* validate the pins */
    if (!ArePinsValid(Port, Pins))
    {
        return (ADI_GPIO_INVALID_PINS);
    }
#endif

    pPort = adi_gpio_Device.pReg[Port];

    /* set the given GPIOs low */
    pPort->CLR = Pins;

    return (ADI_GPIO_SUCCESS);
}


/*!

    @brief      Toggles the Logical Level of the Given GPIO pin(s)

    @details    Toggles the logical level of the given GPIO pin(s) on the given port.
                If a given GPIO pin is at a logical low level, this function will
                change the level to a logical high value. If a given GPIO pin is
                at a logical high level, this function will change the level to a
                logical low value.

    @param[in]  Port    Identifies the GPIO port to be configured (ADI_GPIO_PORTx)
    @param[in]  Pins    Identifies one or more GPIO pins to be configured. GPIO
                        pins can be passed one at a time or in combination. To
                        configure a single GPIO pin, a single GPIO value is
                        passed for this parameter. For example, #ADI_GPIO_PIN_4.
                        Alternatively, multiple GPIO pins can be configured
                        simultaneously by OR-ing together GPIO pin values and
                        passing the resulting value for this parameter. For
                        example, #ADI_GPIO_PIN_0 | #ADI_GPIO_PIN_5 | #ADI_GPIO_PIN_6.

    @return     Status
                    - #ADI_GPIO_SUCCESS if successfully configured
                    - #ADI_GPIO_NOT_INITIALIZED [D] if not yet initialized
                    - #ADI_GPIO_INVALID_PORT [D] if the Port parameter is invalid
                    - #ADI_GPIO_INVALID_PINS [D] if the Pins parameter is invalid

    @sa         adi_gpio_SetHigh, adi_gpio_SetLow, adi_gpio_SetData, adi_gpio_GetData
*/
ADI_GPIO_RESULT adi_gpio_Toggle(const ADI_GPIO_PORT Port, const ADI_GPIO_DATA Pins)
{

    ADI_GPIO_TypeDef    *pPort;     /* pointer to port registers */

#ifdef ADI_DEBUG
    /* make sure we're initialized */
    if (adi_gpio_Device.pData->ReferenceCount == 0)
    {
        return (ADI_GPIO_NOT_INITIALIZED);
    }
    /* validate the port */
    if (!IsPortValid(Port))
    {
        return (ADI_GPIO_INVALID_PORT);
    }
    /* validate the pins */
    if (!ArePinsValid(Port, Pins))
    {
        return (ADI_GPIO_INVALID_PINS);
    }
#endif

    pPort = adi_gpio_Device.pReg[Port];

    /* toggle the given GPIOs */
    pPort->TGL = Pins;

    return (ADI_GPIO_SUCCESS);
}


/*!

    @brief      Sets the Values of all GPIO Pins on the Given Port to
                a Specified Level

    @details    Sets the values of all GPIO pins on the given port to the
                specified value.

    @param[in]  Port    Identifies the GPIO port to be configured (ADI_GPIO_PORTx)
    @param[in]  Pins    Identifies which GPIO pins are to be set to a logical
                        high level. All other GPIO pins on the port will be set
                        to a logical low level. For example, to set pins 0 and
                        1 to a logical high level and all other pins to a logical
                        low level, this parameter should be passed as
                        #ADI_GPIO_PIN_0 | #ADI_GPIO_PIN_1.

    @return     Status
                    - #ADI_GPIO_SUCCESS if successfully configured
                    - #ADI_GPIO_NOT_INITIALIZED [D] if not yet initialized
                    - #ADI_GPIO_INVALID_PORT [D] if the Port parameter is invalid
                    - #ADI_GPIO_INVALID_PINS [D] if the Pins parameter is invalid

    @sa         adi_gpio_SetHigh, adi_gpio_SetLow, adi_gpio_Toggle, adi_gpio_GetData
*/
ADI_GPIO_RESULT adi_gpio_SetData(const ADI_GPIO_PORT Port, const ADI_GPIO_DATA Pins)
{

    ADI_GPIO_TypeDef    *pPort;     /* pointer to port registers */

#ifdef ADI_DEBUG
    /* make sure we're initialized */
    if (adi_gpio_Device.pData->ReferenceCount == 0)
    {
        return (ADI_GPIO_NOT_INITIALIZED);
    }
    /* validate the port */
    if (!IsPortValid(Port))
    {
        return (ADI_GPIO_INVALID_PORT);
    }
    /* validate the pins */
    if (!ArePinsValid(Port, Pins))
    {
        return (ADI_GPIO_INVALID_PINS);
    }
#endif

    pPort = adi_gpio_Device.pReg[Port];

    /* set the GPIOs as directed */
    pPort->OUT = Pins;

    return (ADI_GPIO_SUCCESS);
}


/*!
    @brief      Gets/Senses the Input Level of all GPIO Pins on the Given Port

    @details    Gets the level of all GPIO input pins on the given port

    @param[in]  Port    Identifies the GPIO port to be sensed (ADI_GPIO_PORTx)
    @param[in]  Pins    Identifies the GPIO port pins.
    @param[out] pValue  The passed pValue parameter is written with a packed value containing
                        the status of all the requested GPIO pins on the given port.

                        To get the status of a single GPIO pin, return value can be direcly used.
                        For example to see if pin 4 on port 2 is a logical high level, the following is used:
                            adi_gpio_GetData(#ADI_GPIO_PORT2, #ADI_GPIO_PIN_4, &pValue)
                        pValue will contain the required information.

                        If information is required for multiple pins, following method is required:
                            adi_gpio_GetData(#ADI_GPIO_PORT2, (#ADI_GPIO_PIN_3 | #ADI_GPIO_PIN_4 | #ADI_GPIO_PIN_7), &pValue)
                        To test if pin 4 on port 2 is a logical high level, the following is used:
                            if  (pValue & ADU_GPIO_PIN_4) {
                                pin 4 on port 2 is a logical high value
                            } else {
                                pin 4 on port 2 is a logical low value
                            }

    @return     Status
                    - #ADI_GPIO_SUCCESS if successfully configured
                    - #ADI_GPIO_NOT_INITIALIZED [D] if not yet initialized
                    - #ADI_GPIO_INVALID_PORT [D] if the Port parameter is invalid

    @sa         adi_gpio_SetHigh, adi_gpio_SetLow, adi_gpio_Toggle, adi_gpio_SetData
*/
ADI_GPIO_RESULT adi_gpio_GetData (const ADI_GPIO_PORT Port, const ADI_GPIO_DATA Pins, uint16_t* const pValue)
{

    ADI_GPIO_TypeDef    *pPort;     /* pointer to port registers */

#ifdef ADI_DEBUG
    /* make sure we're initialized */
    if (adi_gpio_Device.pData->ReferenceCount == 0)
    {
        return (ADI_GPIO_NOT_INITIALIZED);
    }
    /* validate the port */
    if (!IsPortValid(Port))
    {
        return (ADI_GPIO_INVALID_PORT);
    }
    /* validate the pins */
    if (!ArePinsValid(Port, Pins))
    {
        return (ADI_GPIO_INVALID_PINS);
    }
#endif

    pPort = adi_gpio_Device.pReg[Port];

    /* return the status of the GPIOs */
    *pValue = (pPort->IN) & Pins;

    return (ADI_GPIO_SUCCESS);
}


/*!
    @brief      Gets the Output Level of all GPIO Pins on the Given Port

    @details    Gets the level of all GPIO output pins on the given port

    @param[in]  Port    Identifies the GPIO port to be read (ADI_GPIO_PORTx)
    @param[in]  Pins    Identifies the GPIO port pins.
    @param[out] pValue  The passed pValue parameter is written with a packed value containing
                        the status of all the requested GPIO pins on the given port.

                        To get the status of a single GPIO pin, return value can be direcly used.
                        For example to see if pin 4 on port 2 is a logical high Output Level, the following is used:
                            adi_gpio_GetOutputData(#ADI_GPIO_PORT2, #ADI_GPIO_PIN_4, &pValue)
                        pValue will contain the required information.

                        If information is required for multiple pins, following method is required:
                            adi_gpio_GetOutputData(#ADI_GPIO_PORT2, (#ADI_GPIO_PIN_3 | #ADI_GPIO_PIN_4 | #ADI_GPIO_PIN_7), &pValue)
                        To test if pin 4 on port 2 is a logical high Output Level, the following is used:
                            if  (pValue & ADU_GPIO_PIN_4) {
                                pin 4 on port 2 is a logical high Output Level
                            } else {
                                pin 4 on port 2 is a logical high Output Level
                            }

    @return     Status
                    - #ADI_GPIO_SUCCESS if successfully configured
                    - #ADI_GPIO_NOT_INITIALIZED [D] if not yet initialized
                    - #ADI_GPIO_INVALID_PORT [D] if the Port parameter is invalid

    @sa         adi_gpio_SetHigh, adi_gpio_SetLow, adi_gpio_Toggle, adi_gpio_SetData
    @sa         adi_gpio_GetData
*/
ADI_GPIO_RESULT adi_gpio_GetOutputData (const ADI_GPIO_PORT Port, const ADI_GPIO_DATA Pins, uint16_t* const pValue)
{

    ADI_GPIO_TypeDef    *pPort;     /* pointer to port registers */

#ifdef ADI_DEBUG
    /* make sure we're initialized */
    if (adi_gpio_Device.pData->ReferenceCount == 0)
    {
        return (ADI_GPIO_NOT_INITIALIZED);
    }
    /* validate the port */
    if (!IsPortValid(Port))
    {
        return (ADI_GPIO_INVALID_PORT);
    }
    /* validate the pins */
    if (!ArePinsValid(Port, Pins))
    {
        return (ADI_GPIO_INVALID_PINS);
    }
#endif

    pPort = adi_gpio_Device.pReg[Port];

    /* return the status of the oputput register */
    *pValue = (pPort->OUT) & Pins;

    return (ADI_GPIO_SUCCESS);
}


/*!
    @brief      Register or unregister an application callback function for external pin interrupts.

    @details    Applications may register a callback function that will be called when an
                external interrupt occurs. In addition to registering the interrupt,
                the application should call the #adi_gpio_EnableExIRQ API to enable the
                external pin interrupt.

                The driver dispatches calls to registered callback functions when the
                properly configured pin(s) latches an external interrupt input on the GPIO
                pin(s).  The callback is dispatched with the following parameters, respectively:

                    - application-provided callback parameter (/a pCBParam),
                    - the interrupt ID (IRQn_Type) that initiated the interrupt,
                    - NULL.

    @note       The application callback function /b must /b clear /b pin /b interrupts before returning from the callback.

    @param[in]  eIrq            The interrupt for which the callback is being registered.
    @param[in]  pfCallback      Pointer to the callback function. This can be passed as NULL to
                                unregister the callback.
    @param[in]  pCBParam        Callback prameter which will be passed back to the application
                                when the callback is called..

    @return     Status
                    - #ADI_GPIO_SUCCESS if successfully registered the callback.
                    - #ADI_GPIO_NOT_INITIALIZED [D] if not yet initialized
                    - #ADI_GPIO_INVALID_INTERRUPT [D] if interrupt ID is invalid

    @sa         adi_gpio_EnableExIRQ
    @sa         adi_gpio_DisableExIRQ
    @sa         adi_gpio_SetGroupInterruptPolarity
    @sa         adi_gpio_GetGroupInterruptPolarity
    @sa         adi_gpio_GetGroupInterruptStatus
*/
ADI_GPIO_RESULT  adi_gpio_RegisterCallback (const IRQn_Type eIrq, ADI_CALLBACK const pfCallback, void *const pCBParam )
{
    uint16_t index = 0u;

#ifdef ADI_DEBUG
    /* make sure we're initialized */
    if (adi_gpio_Device.pData->ReferenceCount == 0)
    {
        return (ADI_GPIO_NOT_INITIALIZED);
    }
    /* validate the interrupt number */
    if (!IsInterruptValid(eIrq))
    {
        return (ADI_GPIO_INVALID_INTERRUPT);
    }
#endif

    ADI_ENTER_CRITICAL_REGION();

    switch (eIrq) {
        case XINT_EVT0_IRQn:
        case XINT_EVT1_IRQn:
        case XINT_EVT2_IRQn:
        case XINT_EVT3_IRQn:
            index = (uint16_t)eIrq - (uint16_t)XINT_EVT0_IRQn;
            break;

        case SYS_GPIO_INTA_IRQn:
        case SYS_GPIO_INTB_IRQn:
            index = (uint16_t)eIrq - (uint16_t)SYS_GPIO_INTA_IRQn + ADI_GPIO_IRQ_GROUPA_INDEX;
            break;

        default:
            break;  /* This shall not be reached */
    }

    adi_gpio_Device.pData->CallbackTable[index].pfCallback = pfCallback;
    adi_gpio_Device.pData->CallbackTable[index].pCBParam = pCBParam;

    ADI_EXIT_CRITICAL_REGION();

    /* return the status */
    return (ADI_GPIO_SUCCESS);
}

/*!
    @brief       To enable processor to wake up from hibernation when there is an activity on specified device.

    @param[in]  eDevice       Indicate the device for which interrupt need to be enabled.
                              Activity on the device will trigger the interrupt. 
    @param[in]  eCondition    Identifier of type ADI_GPIO_IRQ_TRIGGER_CONDITION that specifies the trigger condition for the wakeup.
    
    @return     Status
                    - #ADI_GPIO_SUCCESS if successfully successfully enabled the interrupt.
                    - #ADI_GPIO_NOT_INITIALIZED [D] if not yet initialized

*/



ADI_GPIO_RESULT  adi_gpio_EnableDeviceInterrupt (ADI_GPIO_INTERRUPT_SOURCE eDevice,ADI_GPIO_IRQ_TRIGGER_CONDITION eCondition)
{

    uint32_t volatile   *pIRQCFG= &pADI_XINT0->CFG0;;   /* pointer to the appropriate interrupt config register */
    uint32_t            CfgReg;     /* interrupt config register value */

#ifdef ADI_DEBUG
    /* make sure we're initialized */
    if (adi_gpio_Device.pData->ReferenceCount == 0)
    {
        return (ADI_GPIO_NOT_INITIALIZED);
    }
#endif

    ADI_ENTER_CRITICAL_REGION();
    /* read/modify/write the appropriate bits in the register */
    CfgReg = *pIRQCFG;
    CfgReg &= ~((BITM_XINT_CFG0_UART_RX_EN|BITM_XINT_CFG0_UART_RX_MDE));    
    CfgReg |= (BITM_XINT_CFG0_UART_RX_EN|(uint32_t)eCondition <<BITP_XINT_CFG0_UART_RX_MDE);
    *pIRQCFG = CfgReg;
    ADI_EXIT_CRITICAL_REGION();
    return (ADI_GPIO_SUCCESS);
}
/*!
    @brief       To disable the waking up  the processor from hibernation

    @param[in]  eDevice   Specifies the device.    
    @return     Status
                    - #ADI_GPIO_SUCCESS if successfully successfully disabled the interrupt..
                    - #ADI_GPIO_NOT_INITIALIZED [D] if not yet initialized
*/

ADI_GPIO_RESULT  adi_gpio_DisableDeviceInterrupt (ADI_GPIO_INTERRUPT_SOURCE eDevice)
{

    uint32_t volatile   *pIRQCFG= &pADI_XINT0->CFG0;   /* pointer to the appropriate interrupt config register */
    uint32_t            CfgReg;     /* interrupt config register value */

#ifdef ADI_DEBUG
    /* make sure we're initialized */
    if (adi_gpio_Device.pData->ReferenceCount == 0)
    {
        return (ADI_GPIO_NOT_INITIALIZED);
    }

#endif

    ADI_ENTER_CRITICAL_REGION();
    /* read/modify/write the appropriate bits in the register */
    CfgReg = *pIRQCFG;
    CfgReg &= ~(BITM_XINT_CFG0_UART_RX_EN);
    *pIRQCFG = CfgReg;
    ADI_EXIT_CRITICAL_REGION();
    return (ADI_GPIO_SUCCESS);
}

/*@}*/

/*! \cond PRIVATE */
/* All of the following is excluded from the doxygen output... */

/* Common external interrupt handler */
static void CommonInterruptHandler(const ADI_GPIO_IRQ_INDEX index, const IRQn_Type eIrq)
{
    ADI_GPIO_PORT Port;
    ADI_GPIO_TypeDef *pPort;
    ADI_GPIO_DATA Pins;
    ADI_GPIO_DATA nIntEnabledPins;

    ADI_GPIO_CALLBACK_INFO *pCallbackInfo = &adi_gpio_Device.pData->CallbackTable[index];

    /* clear the external interrupts */
    if((SYS_GPIO_INTA_IRQn != eIrq) && (SYS_GPIO_INTB_IRQn != eIrq))
    {
        pADI_XINT0->CLR = (uint32_t)1u << (eIrq - XINT_EVT0_IRQn);
         
        /* params list is: application-registered cbParam, interrupt ID, and NULL */
        if(pCallbackInfo->pfCallback)
        {
            /* TODO: Check if we need to pass interrupt status also here */
            pCallbackInfo->pfCallback (pCallbackInfo->pCBParam, (uint32_t) eIrq, NULL);
        }
    }
    else
    {
        /* Loop over all the ports. */
        for(Port=ADI_GPIO_PORT0; Port<ADI_GPIO_NUM_PORTS; Port++)
        {
            pPort = adi_gpio_Device.pReg[Port];

            /* Is the interrupt is for GROUP A */
            if(SYS_GPIO_INTA_IRQn == eIrq)
            {
                nIntEnabledPins =  pPort->IENA;
            }
            else  /* Is the interrupt is for GROUP B */
            {
                nIntEnabledPins =  pPort->IENB;
            }

            /* Clear only required interrupts */
            Pins = ((pPort->INT) & nIntEnabledPins);
            pPort->INT = Pins;

            /* params list is: application-registered cbParam, Port number, and interrupt status */
            if((pCallbackInfo->pfCallback != NULL) && (Pins != 0u))
            {
                pCallbackInfo->pfCallback (pCallbackInfo->pCBParam, (uint32_t)Port,&Pins);
            }
            else
            {
                /* Nothong to do */
            }
        }
    }
}


/* strongly-bound interrupt handlers to override the default weak bindings */
ADI_INT_HANDLER(Ext_Int0_Handler)
{
    ISR_PROLOG();
    CommonInterruptHandler(ADI_GPIO_IRQ_EXT0_INDEX, XINT_EVT0_IRQn);
    ISR_EPILOG();      
}

ADI_INT_HANDLER(Ext_Int1_Handler)
{
    ISR_PROLOG();
    CommonInterruptHandler(ADI_GPIO_IRQ_EXT1_INDEX, XINT_EVT1_IRQn);
    ISR_EPILOG();    
}

ADI_INT_HANDLER(Ext_Int2_Handler)
{
    ISR_PROLOG();
    CommonInterruptHandler(ADI_GPIO_IRQ_EXT2_INDEX, XINT_EVT2_IRQn);
    ISR_EPILOG();    
    
}

ADI_INT_HANDLER(Ext_Int3_Handler)
{
    ISR_PROLOG();


    ADI_GPIO_CALLBACK_INFO *pCallbackInfo = &adi_gpio_Device.pData->CallbackTable[ADI_GPIO_IRQ_EXT3_INDEX];    
    uint32_t nEvent=ADI_GPIO_EVENT_EXT_INT3,nEventFlag=0u;
    
     if((pADI_XINT0->EXT_STAT &BITM_XINT_EXT_STAT_STAT_UART_RXWKUP)==BITM_XINT_EXT_STAT_STAT_UART_RXWKUP)
     {
         nEvent |=ADI_GPIO_EVENT_UART_ACTIVITY;
         nEventFlag =BITM_XINT_CLR_UART_RX_CLR;
     }
     if(pCallbackInfo->pfCallback)
    {
        pCallbackInfo->pfCallback (pCallbackInfo->pCBParam, (uint32_t)nEvent, NULL);
    }
     pADI_XINT0->CLR |= (nEventFlag | BITM_XINT_CLR_IRQ3);
     
    ISR_EPILOG();    

}

ADI_INT_HANDLER(GPIO_A_Int_Handler)
{
    ISR_PROLOG();
    CommonInterruptHandler(ADI_GPIO_IRQ_GROUPA_INDEX,SYS_GPIO_INTA_IRQn);
    ISR_EPILOG();    
    
}

ADI_INT_HANDLER(GPIO_B_Int_Handler)
{
    ISR_PROLOG();
    CommonInterruptHandler(ADI_GPIO_IRQ_GROUPB_INDEX,SYS_GPIO_INTB_IRQn);
    ISR_EPILOG();    
    
}


#ifdef ADI_DEBUG

/*!
    @brief      Tests a Port Parameter for Validity

    @details    A debug function that checks a Port parameter for validity

    @param[in]  Port    Identifies a GPIO port (ADI_GPIO_PORTx)

    @return     Status
                    - true      the Port value contains valid data
                    - false     the Port value contains invalid data
*/
static bool_t IsPortValid(const ADI_GPIO_PORT Port)
{
    bool_t PortValid = false;

    /* make sure the port is valid */
    switch (Port)
    {
        case ADI_GPIO_PORT0:
        case ADI_GPIO_PORT1:
        case ADI_GPIO_PORT2:
            PortValid = true;
            break;

        default:
            break;
    }

    return PortValid;
}


/*!
    @brief      Tests a Pins Parameter for Validity

    @details    A debug function that checks a Pins parameter for validity

    @param[in]  Pins    Logical OR-ing of one or more ADI_GPIO_PIN_x values

    @return     Status
                    - true      the Pins value contains valid data
                    - false     the Pins value contains invalid data
*/
static bool_t ArePinsValid(const ADI_GPIO_PORT Port, const ADI_GPIO_DATA Pins)
{
    uint16_t PinValid = 0u;

    /* test for a valid pin */
    switch (Port)
    {
        case ADI_GPIO_PORT0:
            PinValid = ~ADI_GPIO_PORT0_PIN_AVL & Pins;
            break;

        case ADI_GPIO_PORT1:
            PinValid = ~ADI_GPIO_PORT1_PIN_AVL & Pins;
            break;

        case ADI_GPIO_PORT2:
            PinValid = ~ADI_GPIO_PORT2_PIN_AVL & Pins;
            break;

        default:
            break;
    }

    if (PinValid == 0u)
    {
        return true;
    }
    else
    {
        return false;
    }
}


/*!
    @brief      Tests an eIrq Parameter for Validity

    @details    A debug function that checks an eIrq parameter for validity

    @param[in]  eIrq    Identifier of type IRQn_Type

    @return     Status
                    - true      the eIrq value contains valid data
                    - false     the eIrq value contains invalid data
*/
static bool_t IsInterruptValid(const IRQn_Type eIrq)
{
    bool_t IntValid=false;

    /* make sure it's a legal GPIO interrupt */
    switch (eIrq)
    {
        case XINT_EVT0_IRQn:
        case XINT_EVT1_IRQn:
        case XINT_EVT2_IRQn:
        case XINT_EVT3_IRQn:
        case SYS_GPIO_INTA_IRQn:
        case SYS_GPIO_INTB_IRQn:
            IntValid = true;
            break;

        default:
            break;
    }

    return IntValid;
}


/*!
    @brief      Tests an External Interrupt eIrq Parameter for Validity

    @details    A debug function that checks an External Interrupt eIrq parameter for validity

    @param[in]  eIrq    Identifier of type IRQn_Type

    @return     Status
                    - true      eIrq value contains valid data
                    - false     eIrq value contains invalid data
*/
static bool_t IsExInterruptValid(const IRQn_Type eIrq)
{
    bool_t IntValid=false;

    /* make sure it's a legal GPIO interrupt */
    switch (eIrq)
    {
        case XINT_EVT0_IRQn:
        case XINT_EVT1_IRQn:
        case XINT_EVT2_IRQn:
        case XINT_EVT3_IRQn:
            IntValid = true;
            break;

        default:
            break;
    }

    return IntValid;
}


/*!

    @brief      Tests a Trigger Condition Parameter for Validity

    @details    A debug function that checks a trigger condition parameter for validity

    @param[in]  eCondition  Identifier of type ADI_GPIO_IRQ_TRIGGER_CONDITION

    @return     Status
                    - true      the eCondition value contains valid data
                    - false     the eCondition value contains invalid data
*/
static bool_t IsTriggerValid (const ADI_GPIO_IRQ_TRIGGER_CONDITION eCondition)
{
    bool_t TrigValid=false;

    /* make sure it's a valid trigger condition */
    switch (eCondition)
    {
        case ADI_GPIO_IRQ_RISING_EDGE:
        case ADI_GPIO_IRQ_FALLING_EDGE:
        case ADI_GPIO_IRQ_EITHER_EDGE:
        case ADI_GPIO_IRQ_HIGH_LEVEL:
        case ADI_GPIO_IRQ_LOW_LEVEL:
            TrigValid = true;
            break;

        default:
            break;
    }

    return TrigValid;
}
#endif  /* ADI_DEBUG */
/*! \endcond */

#endif /* _ADI_GPIO_V1_C_ */

/*
** EOF
*/
