/**************************************************************************//**
 * @file     ecap.c
 * @version  V3.00

 * @brief    Enhanced Input Capture Timer (ECAP) driver source file
 *
 * @note
 * @copyright (C) 2019 Nuvoton Technology Corp. All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of Nuvoton Technology Corp. nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/
#include "M261.h"



/** @addtogroup Standard_Driver Standard Driver
  @{
*/

/** @addtogroup ECAP_Driver ECAP Driver
  @{
*/

/** @addtogroup ECAP_EXPORTED_FUNCTIONS ECAP Exported Functions
  @{
*/

/**
  * @brief      Enable ECAP function
  * @param[in]  ecap        The pointer of the specified ECAP module.
  * @param[in]  u32FuncMask Input capture function select
  *                         - \ref ECAP_DISABLE_COMPARE
  *                         - \ref ECAP_COMPARE_FUNCTION
  * @return     None
  * @details    This macro enable input capture function and select compare and reload function.
  */
void ECAP_Open(ECAP_T* ecap, uint32_t u32FuncMask)
{
    /* Clear Input capture mode*/
    ecap->CTL0 = ecap->CTL0 & ~(ECAP_CTL0_CMPEN_Msk);

    /* Enable Input Capture and set mode */
    ecap->CTL0 |= ECAP_CTL0_CAPEN_Msk | (u32FuncMask);
}



/**
  * @brief      Disable ECAP function
  * @param[in]  ecap        The pointer of the specified ECAP module.
  * @return     None
  * @details    This macro disable input capture function.
  */
void ECAP_Close(ECAP_T* ecap)
{
    /* Disable Input Capture*/
    ecap->CTL0 &= ~ECAP_CTL0_CAPEN_Msk;
}

/**
  * @brief This macro is used to enable input channel interrupt
  * @param[in] ecap      Specify ECAP port
  * @param[in] u32Mask  The input channel Mask
  *                  - \ref ECAP_CTL0_CAPIEN0_Msk
  *                  - \ref ECAP_CTL0_CAPIEN1_Msk
  *                  - \ref ECAP_CTL0_CAPIEN2_Msk
  *                  - \ref ECAP_CTL0_OVIEN_Msk
  *                  - \ref ECAP_CTL0_CMPIEN_Msk
  * @return None
  * @details This macro will enable the input channel_n interrupt.
  */
void ECAP_EnableINT(ECAP_T* ecap, uint32_t u32Mask)
{
    /* Enable input channel interrupt */
    ecap->CTL0 |= (u32Mask);

    /* Enable NVIC ECAP IRQ */
    if(ecap == ECAP0)
    {
        NVIC_EnableIRQ(ECAP0_IRQn);
    }
    else
    {
        NVIC_EnableIRQ(ECAP1_IRQn);
    }
}

/**
  * @brief This macro is used to disable input channel interrupt
  * @param[in] ecap      Specify ECAP port
  * @param[in] u32Mask  The input channel number
  *                  - \ref ECAP_CTL0_CAPIEN0_Msk
  *                  - \ref ECAP_CTL0_CAPIEN1_Msk
  *                  - \ref ECAP_CTL0_CAPIEN2_Msk
  *                  - \ref ECAP_CTL0_OVIEN_Msk
  *                  - \ref ECAP_CTL0_CMPIEN_Msk
  * @return None
  * @details This macro will disable the input channel_n interrupt.
  */
void ECAP_DisableINT(ECAP_T* ecap, uint32_t u32Mask)
{
    /* Disable input channel interrupt */
    (ecap->CTL0) &= ~(u32Mask);

    /* Disable NVIC ECAP IRQ */
    if(ecap == ECAP0)
    {
        NVIC_DisableIRQ(ECAP0_IRQn);
    }
    else
    {
        NVIC_DisableIRQ(ECAP1_IRQn);
    }
}

/*@}*/ /* end of group ECAP_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group ECAP_Driver */

/*@}*/ /* end of group Standard_Driver */


/*** (C) COPYRIGHT 2019 Nuvoton Technology Corp. ***/
