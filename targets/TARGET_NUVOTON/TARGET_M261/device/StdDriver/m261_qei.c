/**************************************************************************//**
 * @file     qei.c
 * @version  V3.00
 * $Revision: 2 $
 * $Date: 17/09/20 9:33a $
 * @brief    Quadrature Encoder Interface (QEI) driver source file
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

/** @addtogroup QEI_Driver QEI Driver
  @{
*/

/** @addtogroup QEI_EXPORTED_FUNCTIONS QEI Exported Functions
  @{
*/

/**
  * @brief      Close QEI function
  * @param[in]  qei         The pointer of the specified QEI module.
  * @return     None
  * @details    This function reset QEI configuration and stop QEI counting.
  */
void QEI_Close(QEI_T* qei)
{
    /* Reset QEI configuration */
    qei->CTL = 0UL;
}

/**
  * @brief      Disable QEI interrupt
  * @param[in]  qei         The pointer of the specified QEI module.
  * @param[in]  u32IntSel   Interrupt type selection.
  *                         - \ref QEI_CTL_DIRIEN_Msk   : Direction change interrupt
  *                         - \ref QEI_CTL_OVUNIEN_Msk  : Counter overflow or underflow interrupt
  *                         - \ref QEI_CTL_CMPIEN_Msk   : Compare-match interrupt
  *                         - \ref QEI_CTL_IDXIEN_Msk   : Index detected interrupt
  * @return     None
  * @details    This function disable QEI specified interrupt.
  */
void QEI_DisableInt(QEI_T* qei, uint32_t u32IntSel)
{
    /* Disable QEI specified interrupt */
    QEI_DISABLE_INT(qei, u32IntSel);

    /* Disable NVIC QEI IRQ */
    if(qei == QEI0)
    {
        NVIC_DisableIRQ(QEI0_IRQn);
    }
    else
    {
        NVIC_DisableIRQ(QEI1_IRQn);
    }
}

/**
  * @brief      Enable QEI interrupt
  * @param[in]  qei         The pointer of the specified QEI module.
  * @param[in]  u32IntSel   Interrupt type selection.
  *                         - \ref QEI_CTL_DIRIEN_Msk   : Direction change interrupt
  *                         - \ref QEI_CTL_OVUNIEN_Msk  : Counter overflow or underflow interrupt
  *                         - \ref QEI_CTL_CMPIEN_Msk   : Compare-match interrupt
  *                         - \ref QEI_CTL_IDXIEN_Msk   : Index detected interrupt
  * @return     None
  * @details    This function enable QEI specified interrupt.
  */
void QEI_EnableInt(QEI_T* qei, uint32_t u32IntSel)
{
    /* Enable QEI specified interrupt */
    QEI_ENABLE_INT(qei, u32IntSel);

    /* Enable NVIC QEI IRQ */
    if(qei == QEI0)
    {
        NVIC_EnableIRQ(QEI0_IRQn);
    }
    else
    {
        NVIC_EnableIRQ(QEI1_IRQn);
    }
}

/**
  * @brief      Open QEI in specified mode and enable input
  * @param[in]  qei         The pointer of the specified QEI module.
  * @param[in]  u32Mode     QEI counting mode.
  *                         - \ref QEI_CTL_X4_FREE_COUNTING_MODE
  *                         - \ref QEI_CTL_X2_FREE_COUNTING_MODE
  *                         - \ref QEI_CTL_X4_COMPARE_COUNTING_MODE
  *                         - \ref QEI_CTL_X2_COMPARE_COUNTING_MODE
  * @param[in]  u32Value    The counter maximum value in compare-counting mode.
  * @return     None
  * @details    This function set QEI in specified mode and enable input.
  */
void QEI_Open(QEI_T* qei, uint32_t u32Mode, uint32_t u32Value)
{
    /* Set QEI function configuration */
    /* Set QEI counting mode */
    /* Enable IDX, QEA and QEB input to QEI controller */
    qei->CTL = (qei->CTL & (~QEI_CTL_MODE_Msk)) | ((u32Mode) | QEI_CTL_CHAEN_Msk | QEI_CTL_CHBEN_Msk | QEI_CTL_IDXEN_Msk);

    /* Set QEI maximum count value in in compare-counting mode */
    qei->CNTMAX = u32Value;
}

/**
  * @brief      Start QEI function
  * @param[in]  qei     The pointer of the specified QEI module.
  * @return     None
  * @details    This function enable QEI function and start QEI counting.
  */
void QEI_Start(QEI_T* qei)
{
    /* Enable QEI controller function */
    qei->CTL |= QEI_CTL_QEIEN_Msk;
}

/**
  * @brief      Stop QEI function
  * @param[in]  qei     The pointer of the specified QEI module.
  * @return     None
  * @details    This function disable QEI function and stop QEI counting.
  */
void QEI_Stop(QEI_T* qei)
{
    /* Disable QEI controller function */
    qei->CTL &= (~QEI_CTL_QEIEN_Msk);
}


/*@}*/ /* end of group QEI_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group QEI_Driver */

/*@}*/ /* end of group Standard_Driver */

/*** (C) COPYRIGHT 2019 Nuvoton Technology Corp. ***/
