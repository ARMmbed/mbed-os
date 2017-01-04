/*!
 *****************************************************************************
 * @file:    adi_nvic.c
 * @brief:   NVIC controller API implementation file.
 * @version: $Revision: 33290 $
 * @date:    $Date: 2016-01-19 14:11:48 +0000 (Tue, 19 Jan 2016) $
 *-----------------------------------------------------------------------------
 *
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

#include <adi_processor.h>
#include <services/int/adi_nvic.h>

/** @addtogroup  RTOS_INTERRUPTS RTOS-specific Interrupt APIs
 *  @{
 */

#ifdef __ICCARM__
/*
* IAR MISRA C 2004 error suppressions.
*
* Pm152 (rule 17.4): array indexing shall only be applied to objects defined as an array type
*   The two halves of the descriptor array are accessed through pointers.
*
* Pm140 (rule 11.3): a cast should not be performed between a pointer type and an integral type
*   Required for interrupt vector table accesses.
*/
#pragma diag_suppress=Pm152,Pm140
#endif /* __ICCARM__ */

/* Default NVIC interrupt handler */
static void adi_nvic_DefaultHandler(
    void
);

/*!
  @brief        Registers interrupt handler with NVIC.
  @details      The given interrupt handler is written into the NVIC vector table.

  @param[in]    iid             Interrupt ID for which the handler to be registered.
  @param[in]    pfHandler       Pointer to the interrupt handler function.

  @return       Status
                - Index - Upon success returns the index to the interrupt vector table.
                - (-1)  - Upon failure.

  @sa           adi_nvic_UnRegisterHandler()
 */
int32_t adi_nvic_RegisterHandler (
        uint32_t                        iid,
        ADI_NVIC_HANDLER                pfHandler
)
{
    /* get the IVT pointer from the NVIC offset register in case its been relocated */
    ADI_NVIC_HANDLER *pIVT = (ADI_NVIC_HANDLER *)SCB->VTOR;

    uint32_t index = ADI_NVIC_SID_IRQ(iid);
#ifdef ADI_DEBUG
#if defined (ADI_INT_NUM_INT)
    /* this macro is not currently defined but it should be in a future release */
    if(iid >= ADI_INT_NUM_INT)
    {
        return -1;
    }
#endif
#endif /* ADI_DEBUG */

    __disable_irq();

    pIVT[index] = pfHandler;

    __enable_irq();

    return (int32_t)index;
}


/*!
  @brief        UnRegisters interrupt handler with NVIC.
  @details      Writes back default interrupt handler into the NVIC vector table.

  @param[in]    iid             Interrupt ID for which the handler to be unregistered.

  @return       Status
                - Index - Upon success returns the index to the interrupt vector table.
                - (-1)  - Upon failure.

  @sa           adi_nvic_RegisterHandler()
 */
int32_t adi_nvic_UnRegisterHandler (
        uint32_t                        iid
)
{
	/* get the IVT from the NVIC offset register in case its been relocated */
	ADI_NVIC_HANDLER *pIVT = (ADI_NVIC_HANDLER *)SCB->VTOR;

    uint32_t index = ADI_NVIC_SID_IRQ(iid);
#ifdef ADI_DEBUG
#if defined (ADI_INT_NUM_INT)
    /* this macro is not currently defined but it should be in a future release */
    if(iid >= ADI_INT_NUM_INT)
    {
        return -1;
    }
#endif
#endif /* ADI_DEBUG */

    __disable_irq();

    pIVT[index] = adi_nvic_DefaultHandler;

    __enable_irq();

    return (int32_t)index;
}



/*!
  @brief        Enables/Disables interrupt.

  @param[in]    iid             Interrupt ID of the interrupt to be enabled/disabled.
  @param[in]    bEnable         Flag which indicates whether to enable/disable.
                                'true'  - to Enable
                                'false' - to Disable

  @return       Status
                ADI_NVIC_SUCCESS        Successfully enabled/disabled the interrupt.
                ADI_NVIC_INVALID_INT[D] If the given interrupt ID is invalid or
                                        the given interrupt ID is a System Exception.
                                        System exception cannot be disabled.

  @sa           adi_nvic_register_handler()
 */
ADI_NVIC_RESULT adi_nvic_EnableInt(
    uint32_t    iid,
    bool_t      bEnable
)
{
#ifdef ADI_DEBUG
#if defined (ADI_INT_NUM_INT)
    /* this macro is not currently defined but it should be in a future release */
  if((iid >= ADI_INT_NUM_INT) || (iid <= 0))
  {
    return ADI_NVIC_INVALID_INT;
  }
#endif
#endif /* ADI_DEBUG */

  if(bEnable == true)
  {
    NVIC_EnableIRQ((IRQn_Type)iid);
  }
  else
  {
    NVIC_DisableIRQ((IRQn_Type)iid);
  }

  return ADI_NVIC_SUCCESS;
}

/* Default interrupt handler */
static void adi_nvic_DefaultHandler(
    void
)
{
    /* Un handled interrupt trap here */
    while(1){}
}

/*@}*/
