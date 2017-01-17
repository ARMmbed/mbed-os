/*
 *****************************************************************************
   @file:    adi_gpio.c
   @brief:   GPIO device driver implementation.
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
#include <drivers/gpio/adi_gpio.h>
#include <rtos_map/adi_rtos_map.h>
#include "adi_gpio_def.h"

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
* Pm140 (rule 11.3): a cast should not be performed between a pointer type and an integral type
*   The rule makes an exception for memory-mapped register accesses.
*/
#pragma diag_suppress=Pm123,Pm073,Pm143,Pm140
#endif /* __ICCARM__ */

/* Debug function declarations */
#ifdef ADI_DEBUG
static bool ArePinsValid      (const ADI_GPIO_PORT Port, const ADI_GPIO_DATA Pins);   /*!< tests for pins validity */
#endif /* ADI_DEBUG */


static void CommonInterruptHandler  (const ADI_GPIO_IRQ_INDEX index, const IRQn_Type eIrq);
void GPIO_A_Int_Handler(void);
void GPIO_B_Int_Handler(void);

/*==========  D A T A  ==========*/
static ADI_GPIO_DRIVER adi_gpio_Device =
{
    {
       pADI_GPIO0,       /* port 0 base address */
       pADI_GPIO1,       /* port 1 base address */
       pADI_GPIO2,       /* port 2 base address */
	   pADI_GPIO3,		 /* port 3 base address */
    },
    
    NULL
};
/*! \endcond */

/*! \addtogroup GPIO_Driver GPIO Driver
 *  @{

  @brief    GPIO port and pin identifiers
  @note The application must include drivers/gpio/adi_gpio.h to use this driver
  @details  The documented macros can be passed to the following functions:
      - adi_gpio_OutputEnable()
      - adi_gpio_PullUpEnable()
      - adi_gpio_SetHigh()
      - adi_gpio_SetLow()
      - adi_gpio_Toggle()
      - adi_gpio_SetData()
      - adi_gpio_GetData()

      To control a single GPIO, these macros can be passed to the functions one
      at a time. For example, to set the GPIO on port 2, pin 4 to a logical high
      level, the following is used:

      <pre>
      adi_gpio_SetHigh(ADI_GPIO_PORT2, ADI_GPIO_PIN_4)
      </pre>

      Multiple GPIOs, so long as they reside on the same port, can be controlled
      simultaneously. These macros can be OR-ed together and passed to the
      functions. For example, to set the GPIOs on port 2, pins 3, 4 and 7 to
      a logical low level, the following is used:

      <pre>
      adi_gpio_SetLow(ADI_GPIO_PORT2, ADI_GPIO_PIN_3 | ADI_GPIO_PIN_4 | ADI_GPIO_PIN_7)
      </pre>

      For the sensing, or adi_gpio_Getxxx, functions, the passed pValue parameter is written with
      a packed value containing the status of the requested GPIO pins on the given port.

      If information is required for a single pin, return value can be directly used
      For example to see if pin 4 on port 2 has the pull up enabled, the following is used:
      adi_gpio_GetData(ADI_GPIO_PORT2, ADI_GPIO_PIN_4, &pValue)
      pValue will contain the required information.

      If information is required for multiple pins, following method is required:
      <pre>
        adi_gpio_GetData(ADI_GPIO_PORT2, (ADI_GPIO_PIN_3 | ADI_GPIO_PIN_4 | ADI_GPIO_PIN_7), &pValue)
      </pre>
      To test if pin 4 on port 2 has pull up enabled, the following is used:
      <pre>
        if   (pValue & ADI_GPIO_PIN_4) {
                    the pull up is enabled for pin 4 on port 2
        } else {
                    the pull up is disabled for pin 4 on port 2
        }
      </pre>

 */

/*!
    @brief      Initializes the GPIO functions.

    @details    This function initializes the GPIO driver. This function should be called before calling any of the GPIO
                driver APIs.

    @param[in] pMemory      Pointer to the memory required for the driver to operate.
                            The size of the memory should be at least #ADI_GPIO_MEMORY_SIZE bytes.
                            
    @param[in] MemorySize   Size of the memory (in bytes) passed in pMemory parameter.

    @return     Status
                    - ADI_GPIO_SUCCESS                  If successfully initialized the GPIO driver.
                    - ADI_GPIO_NULL_PARAMETER       [D] If the given pointer to the driver memory is pointing to NULL.
                    - ADI_GPIO_INVALID_MEMORY_SIZE  [D] If the given memory size is not sufficient to operate the driver. 

    @note       This function clears memory reserved for managing the callback function when it is called 
                for the first time. It is expected from user to call "adi_gpio_UnInit" function when the GPIO service is no longer required.

    @sa         adi_gpio_UnInit
*/
ADI_GPIO_RESULT adi_gpio_Init(
    void*                const pMemory,
    uint32_t             const MemorySize
)
{

#ifdef ADI_DEBUG
        /* Verify the given memory pointer */
        if(NULL == pMemory)
        {
            return ADI_GPIO_NULL_PARAMETER;
        }
        /* Check if the memory size is sufficient to operate the driver */ 
        if(MemorySize < ADI_GPIO_MEMORY_SIZE)
        {
            return ADI_GPIO_INVALID_MEMORY_SIZE;
        }
        assert(ADI_GPIO_MEMORY_SIZE == sizeof(ADI_GPIO_DEV_DATA));
#endif

    /* Only initialize on 1st init call, i.e., preserve callbacks on multiple inits */
    if (NULL == adi_gpio_Device.pData)
    {
        uint32_t i;

        adi_gpio_Device.pData = (ADI_GPIO_DEV_DATA*)pMemory;

        /* Initialize the callback table */
        for (i = 0u; i < ADI_GPIO_NUM_INTERRUPTS; i++)
        {
            adi_gpio_Device.pData->CallbackTable[i].pfCallback = NULL;
            adi_gpio_Device.pData->CallbackTable[i].pCBParam   = NULL;
        }
        
        /* Enable the group interrupts */
        NVIC_EnableIRQ(SYS_GPIO_INTA_IRQn);
        NVIC_EnableIRQ(SYS_GPIO_INTB_IRQn);
    }

    return (ADI_GPIO_SUCCESS);
}


/*!
    @brief      Un-initialize the GPIO driver.

    @details    Terminates the GPIO functions, leaving everything unchanged.

    @return     Status
                    - #ADI_GPIO_SUCCESS if successfully uninitialized
                    - #ADI_GPIO_NOT_INITIALIZED [D] if not yet initialized

    @sa         adi_gpio_Init
*/
ADI_GPIO_RESULT adi_gpio_UnInit(void)
{

#ifdef ADI_DEBUG
    /* IF (not initialized) */
    if (NULL == adi_gpio_Device.pData)
    {
        /* return error if not initialized */
        return (ADI_GPIO_NOT_INITIALIZED);
    }
#endif
    
    /* Clear the data pointer */
    adi_gpio_Device.pData = NULL;

    return (ADI_GPIO_SUCCESS);
}


/*!
    @brief      Group the pins for the given group interrupt.

    @details    Group the given pins for the Group A/B interrupt.
                Applications can register/unregister a callback using the #adi_gpio_RegisterCallback API
                to get a notification when the group interrupt occurs.

    @param[in]  Port   GPIO port number to be operated on.
    @param[in]  eIrq   Interrupt (Group A/B) to which the pin(s) are to be grouped.
    @param[in]  Pins   The GPIO pins which needs to be grouped.
                           Pin bits that are set enable the interrupt for the group A/B.
                           Pin bits that are clear disable the interrupt for the group A/B.
    @return     Status
                    - #ADI_GPIO_SUCCESS                 If successfully grouped the given pins.
                    - #ADI_GPIO_NOT_INITIALIZED     [D] If GPIO driver is not yet initialized.
                    - #ADI_GPIO_INVALID_PINS        [D] The given pins are invalid.

    @sa         adi_gpio_RegisterCallback
    @sa         adi_gpio_SetGroupInterruptPolarity
*/
ADI_GPIO_RESULT adi_gpio_SetGroupInterruptPins(const ADI_GPIO_PORT Port, const ADI_GPIO_IRQ eIrq, const ADI_GPIO_DATA Pins)
{
    ADI_GPIO_TypeDef    *pPort;     /* pointer to port registers */
    ADI_INT_STATUS_ALLOC();
#ifdef ADI_DEBUG
    /* make sure we're initialized */
    if (NULL == adi_gpio_Device.pData)
    {
        return (ADI_GPIO_NOT_INITIALIZED);
    }

    /* validate the pins */
    if (!ArePinsValid(Port, Pins))
    {
        return (ADI_GPIO_INVALID_PINS);
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
    @brief      Set the interrupt polarity for the given pins.

    @details    Sets the interrupt polarity for the given pins for the given port.
                When the corresponding bit is set an interrupt is generated when the pin transitions from low-to-high. When the corresponding bit is cleared an interrupt is generated when the pin transitions from high-to-low.

    @param[in]  Port   GPIO port number to be operated on.
    @param[in]  Pins   Pins whose polarity to be set.

    @return     Status
                    - #ADI_GPIO_SUCCESS             If successfully set the polarity.
                    - #ADI_GPIO_NOT_INITIALIZED [D] If not yet initialized.
                    - #ADI_GPIO_INVALID_PINS    [D] If the given pins are invalid.

    @sa         adi_gpio_RegisterCallback
    @sa         adi_gpio_SetGroupInterruptPins
*/
ADI_GPIO_RESULT adi_gpio_SetGroupInterruptPolarity(const ADI_GPIO_PORT Port, const ADI_GPIO_DATA Pins)
{
    ADI_GPIO_TypeDef    *pPort;     /* pointer to port registers */

#ifdef ADI_DEBUG
    /* make sure we're initialized */
    if (NULL == adi_gpio_Device.pData)
    {
        return (ADI_GPIO_NOT_INITIALIZED);
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
    @brief      Enables/Disables the Output Drivers for GPIO Pin(s)

    @details    Enables/disables the output drivers for the given GPIO pin(s) on
                the given port.

    @param[in]  Port    The GPIO port to be configured.
    @param[in]  Pins    One or more GPIO pins to be configured. GPIO
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
                    - #ADI_GPIO_SUCCESS             If successfully configured
                    - #ADI_GPIO_NOT_INITIALIZED [D] If GPIO driver not yet initialized.
                    - #ADI_GPIO_INVALID_PINS    [D] If the given pins are invalid.
*/
ADI_GPIO_RESULT adi_gpio_OutputEnable(const ADI_GPIO_PORT Port, const ADI_GPIO_DATA Pins, const bool bFlag)
{

    ADI_GPIO_TypeDef    *pPort;     /* pointer to port registers */
    ADI_INT_STATUS_ALLOC();
    
#ifdef ADI_DEBUG
    /* make sure we're initialized */
    if (NULL == adi_gpio_Device.pData)
    {
        return (ADI_GPIO_NOT_INITIALIZED);
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

    @param[in]  Port    The GPIO port to be configured.
    @param[in]  Pins    One or more GPIO pins to be configured. GPIO
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
                    - #ADI_GPIO_SUCCESS             If successfully configured.
                    - #ADI_GPIO_NOT_INITIALIZED [D] If GPIO driver not yet initialized.
                    - #ADI_GPIO_INVALID_PINS    [D] If the given pins are invalid.
*/
ADI_GPIO_RESULT adi_gpio_InputEnable(const ADI_GPIO_PORT Port, const ADI_GPIO_DATA Pins, const bool bFlag)
{

    ADI_GPIO_TypeDef    *pPort;     /* pointer to port registers */
    ADI_INT_STATUS_ALLOC();
    
#ifdef ADI_DEBUG
    /* make sure we're initialized */
    if (NULL == adi_gpio_Device.pData) {
        return (ADI_GPIO_NOT_INITIALIZED);
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

    @param[in]  Port    The GPIO port to be configured.
    @param[in]  Pins    One or more GPIO pins to be configured. GPIO
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
                    - #ADI_GPIO_SUCCESS             If successfully configured.
                    - #ADI_GPIO_NOT_INITIALIZED [D] If GPIO driver not yet initialized.
                    - #ADI_GPIO_INVALID_PINS    [D] If the given pins are invalid.
*/
ADI_GPIO_RESULT adi_gpio_PullUpEnable(const ADI_GPIO_PORT Port, const ADI_GPIO_DATA Pins, const bool bFlag)
{

    ADI_GPIO_TypeDef    *pPort;     /* pointer to port registers */
    ADI_INT_STATUS_ALLOC();
    
#ifdef ADI_DEBUG
    /* make sure we're initialized */
    if (NULL == adi_gpio_Device.pData)
    {
        return (ADI_GPIO_NOT_INITIALIZED);
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

    @brief      Sets the Given GPIO pin(s) to a Logical High Level

    @details    Sets the given GPIO pin(s) on the given port to a logical high
                level.

    @param[in]  Port    GPIO port whose pins need to be set to logical high level.
    @param[in]  Pins    One or more GPIO pins to be set to logical high. GPIO
                        pins can be passed one at a time or in combination. To
                        configure a single GPIO pin, a single GPIO value is
                        passed for this parameter. For example, #ADI_GPIO_PIN_4.
                        Alternatively, multiple GPIO pins can be configured
                        simultaneously by OR-ing together GPIO pin values and
                        passing the resulting value for this parameter. For
                        example, #ADI_GPIO_PIN_0 | #ADI_GPIO_PIN_5 | #ADI_GPIO_PIN_6.

    @return     Status
                    - #ADI_GPIO_SUCCESS             If successfully configured.
                    - #ADI_GPIO_NOT_INITIALIZED [D] If GPIO driver not yet initialized.
                    - #ADI_GPIO_INVALID_PINS    [D] If the given pins are invalid.

    @sa         adi_gpio_SetLow, adi_gpio_Toggle, adi_gpio_SetData, adi_gpio_GetData
*/
ADI_GPIO_RESULT adi_gpio_SetHigh(const ADI_GPIO_PORT Port, const ADI_GPIO_DATA Pins)
{

    ADI_GPIO_TypeDef    *pPort;     /* pointer to port registers */

#ifdef ADI_DEBUG
    /* make sure we're initialized */
    if (NULL == adi_gpio_Device.pData)
    {
        return (ADI_GPIO_NOT_INITIALIZED);
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

    @param[in]  Port    The GPIO port whose pins need to be set to logical low level.
    @param[in]  Pins    One or more GPIO pins to be whose logic level to be set. GPIO
                        pins can be passed one at a time or in combination. To
                        configure a single GPIO pin, a single GPIO value is
                        passed for this parameter. For example, #ADI_GPIO_PIN_4.
                        Alternatively, multiple GPIO pins can be configured
                        simultaneously by OR-ing together GPIO pin values and
                        passing the resulting value for this parameter. For
                        example, #ADI_GPIO_PIN_0 | #ADI_GPIO_PIN_5 | #ADI_GPIO_PIN_6.

    @return     Status
                    - #ADI_GPIO_SUCCESS             If successfully configured.
                    - #ADI_GPIO_NOT_INITIALIZED [D] If GPIO driver not yet initialized.
                    - #ADI_GPIO_INVALID_PINS    [D] If the given pins are invalid.

    @sa         adi_gpio_SetHigh, adi_gpio_Toggle, adi_gpio_SetData, adi_gpio_GetData
*/
ADI_GPIO_RESULT adi_gpio_SetLow(const ADI_GPIO_PORT Port, const ADI_GPIO_DATA Pins)
{

    ADI_GPIO_TypeDef    *pPort;     /* pointer to port registers */

#ifdef ADI_DEBUG
    /* make sure we're initialized */
    if (NULL == adi_gpio_Device.pData)
    {
        return (ADI_GPIO_NOT_INITIALIZED);
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

    @param[in]  Port    The GPIO port whose pins to be toggled.
    @param[in]  Pins    The GPIO pins whose logic level to be toggled. GPIO
                        pins can be passed one at a time or in combination. To
                        configure a single GPIO pin, a single GPIO value is
                        passed for this parameter. For example, #ADI_GPIO_PIN_4.
                        Alternatively, multiple GPIO pins can be configured
                        simultaneously by OR-ing together GPIO pin values and
                        passing the resulting value for this parameter. For
                        example, #ADI_GPIO_PIN_0 | #ADI_GPIO_PIN_5 | #ADI_GPIO_PIN_6.

    @return     Status
                    - #ADI_GPIO_SUCCESS             If successfully configured.
                    - #ADI_GPIO_NOT_INITIALIZED [D] If GPIO driver not yet initialized.
                    - #ADI_GPIO_INVALID_PINS    [D] If the given pins are invalid.

    @sa         adi_gpio_SetHigh, adi_gpio_SetLow, adi_gpio_SetData, adi_gpio_GetData
*/
ADI_GPIO_RESULT adi_gpio_Toggle(const ADI_GPIO_PORT Port, const ADI_GPIO_DATA Pins)
{

    ADI_GPIO_TypeDef    *pPort;     /* pointer to port registers */

#ifdef ADI_DEBUG
    /* make sure we're initialized */
    if (NULL == adi_gpio_Device.pData)
    {
        return (ADI_GPIO_NOT_INITIALIZED);
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

    @brief      Sets the logic level of all GPIO pins on the given port to
                a given logic level.

    @details    Sets the logic level of all the GPIO pins on the given port to the
                given value.

    @param[in]  Port    The GPIO port whose pins logic level to be set.
    @param[in]  Pins    The GPIO pins whose logic level to be set high. All other 
                        GPIO pins on the port will be set to a logical low level. 
                        For example, to set pins 0 and 1 to a logical high level and 
                        all other pins to a logical low level, this parameter should 
                        be passed as #ADI_GPIO_PIN_0 | #ADI_GPIO_PIN_1.

    @return     Status
                    - #ADI_GPIO_SUCCESS             If successfully set the given data.
                    - #ADI_GPIO_NOT_INITIALIZED [D] If GPIO driver not yet initialized.
                    - #ADI_GPIO_INVALID_PINS    [D] If the given pins are invalid.

    @sa         adi_gpio_SetHigh, adi_gpio_SetLow, adi_gpio_Toggle, adi_gpio_GetData
*/
ADI_GPIO_RESULT adi_gpio_SetData(const ADI_GPIO_PORT Port, const ADI_GPIO_DATA Pins)
{

    ADI_GPIO_TypeDef    *pPort;     /* pointer to port registers */

#ifdef ADI_DEBUG
    /* make sure we're initialized */
    if (NULL == adi_gpio_Device.pData)
    {
        return (ADI_GPIO_NOT_INITIALIZED);
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
    @brief      Gets/Senses the input level of all GPIO Pins on the given port.

    @details    Gets the level of all GPIO input pins on the given port.

    @param[in]  Port    The GPIO port whose input level to be sensed.
    @param[in]  Pins    The GPIO pins to be sensed. To sense a single GPIO pin, a single 
                        GPIO value is passed for this parameter. For example, #ADI_GPIO_PIN_4.
                        Alternatively, multiple GPIO pins can be configured
                        simultaneously by OR-ing together GPIO pin values and
                        passing the resulting value for this parameter. For
                        example, #ADI_GPIO_PIN_0 | #ADI_GPIO_PIN_5 | #ADI_GPIO_PIN_6.
    @param[out] pValue  The passed pValue parameter is written with a packed value containing
                        the status of all the requested GPIO pins on the given port.

    To get the status of a single GPIO pin, return value can be directly used.
    For example to see if pin 4 on port 2 is a logical high level, the following is used:
    <pre>
        adi_gpio_GetData(#ADI_GPIO_PORT2, #ADI_GPIO_PIN_4, &pValue)
    </pre>
    pValue will contain the required information.

    If information is required for multiple pins, following method is required:
    <pre>
        adi_gpio_GetData(#ADI_GPIO_PORT2, (#ADI_GPIO_PIN_3 | #ADI_GPIO_PIN_4 | #ADI_GPIO_PIN_7), &pValue)
    </pre>

    To test if pin 4 on port 2 is a logical high level, the following is used:
    <pre>
        if  (pValue & ADI_GPIO_PIN_4) {
            pin 4 on port 2 is a logical high value
        } else {
            pin 4 on port 2 is a logical low value
        }
    </pre>

    @return     Status
                    - #ADI_GPIO_SUCCESS             If successfully sensed the input pins.
                    - #ADI_GPIO_NOT_INITIALIZED [D] If GPIO driver not yet initialized.
                    - #ADI_GPIO_INVALID_PINS    [D] If the given pins are invalid.

    @sa         adi_gpio_SetHigh, adi_gpio_SetLow, adi_gpio_Toggle, adi_gpio_SetData
*/
ADI_GPIO_RESULT adi_gpio_GetData (const ADI_GPIO_PORT Port, const ADI_GPIO_DATA Pins, uint16_t* const pValue)
{

    ADI_GPIO_TypeDef    *pPort;     /* pointer to port registers */

#ifdef ADI_DEBUG
    /* make sure we're initialized */
    if (NULL == adi_gpio_Device.pData)
    {
        return (ADI_GPIO_NOT_INITIALIZED);
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
    @brief      Register or unregister an application callback function for group (A/B) interrupts.

    @details    Applications may register a callback function that will be called when a
                GPIO group (A/B) interrupt occurs. 
                
                The driver dispatches calls to registered callback functions when the
                properly configured pin(s) latches an external interrupt input on the GPIO
                pin(s).  The callback is dispatched with the following parameters, respectively:
                    - application-provided callback parameter (\a pCBParam),
                    - The GPIO Port,
                    - The GPIO Pins.

    @param[in]  eIrq            The interrupt for which the callback is being registered.
    @param[in]  pfCallback      Pointer to the callback function. This can be passed as NULL to
                                unregister the callback.
    @param[in]  pCBParam        Callback parameter which will be passed back to the application
                                when the callback is called..

    @return     Status
                    - #ADI_GPIO_SUCCESS if successfully registered the callback.
                    - #ADI_GPIO_NOT_INITIALIZED [D] if not yet initialized
                    - #ADI_GPIO_INVALID_INTERRUPT [D] if interrupt ID is invalid

    @sa         adi_gpio_SetGroupInterruptPolarity
*/
ADI_GPIO_RESULT  adi_gpio_RegisterCallback (const ADI_GPIO_IRQ eIrq, ADI_CALLBACK const pfCallback, void *const pCBParam )
{
    uint16_t index = 0u;
    ADI_INT_STATUS_ALLOC();
    
#ifdef ADI_DEBUG
    /* make sure we're initialized */
    if (NULL == adi_gpio_Device.pData)
    {
        return (ADI_GPIO_NOT_INITIALIZED);
    }
#endif

    index = (uint16_t)eIrq - (uint16_t)SYS_GPIO_INTA_IRQn + ADI_GPIO_IRQ_GROUPA_INDEX;

    ADI_ENTER_CRITICAL_REGION();
    
    adi_gpio_Device.pData->CallbackTable[index].pfCallback = pfCallback;
    adi_gpio_Device.pData->CallbackTable[index].pCBParam = pCBParam;

    ADI_EXIT_CRITICAL_REGION();

    /* return the status */
    return (ADI_GPIO_SUCCESS);
}



/*@}*/

/*! \cond PRIVATE */
/* All of the following is excluded from the doxygen output... */

/* Common group (A/B) interrupt handler */
static void CommonInterruptHandler(const ADI_GPIO_IRQ_INDEX index, const IRQn_Type eIrq)
{
    ADI_GPIO_PORT Port;
    ADI_GPIO_TypeDef *pPort;
    ADI_GPIO_DATA Pins;
    ADI_GPIO_DATA nIntEnabledPins;

    ADI_GPIO_CALLBACK_INFO *pCallbackInfo = &adi_gpio_Device.pData->CallbackTable[index];

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
            pCallbackInfo->pfCallback (pCallbackInfo->pCBParam, (uint32_t)Port, &Pins);
        }
    }
}

/* Interrupt A handler */
void GPIO_A_Int_Handler(void)
{
    ISR_PROLOG()
    CommonInterruptHandler(ADI_GPIO_IRQ_GROUPA_INDEX, SYS_GPIO_INTA_IRQn);
    ISR_EPILOG()
}

/* Interrupt B handler */
void GPIO_B_Int_Handler (void)
{
    ISR_PROLOG()
    CommonInterruptHandler(ADI_GPIO_IRQ_GROUPB_INDEX, SYS_GPIO_INTB_IRQn);
    ISR_EPILOG()      
}

#ifdef ADI_DEBUG


/*!
    @brief      Tests a Pins Parameter for Validity

    @details    A debug function that checks a Pins parameter for validity

    @param[in]  Pins    Logical OR-ing of one or more ADI_GPIO_PIN_x values

    @return     Status
                    - true      the Pins value contains valid data
                    - false     the Pins value contains invalid data
*/
static bool ArePinsValid(const ADI_GPIO_PORT Port, const ADI_GPIO_DATA Pins)
{
    uint32_t PinValid = 0u;

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
            
        case ADI_GPIO_PORT3:
            PinValid = ~ADI_GPIO_PORT3_PIN_AVL & Pins;
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
#endif /* ADI_DEBUG */

/*! \endcond  */

/*
** EOF
*/
