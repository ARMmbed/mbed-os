/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "fsl_sdhc_hal.h"

/*FUNCTION****************************************************************
 *
 * Function Name: sdhc_hal_enable_intr_signal
 * Description: Enable specified interrupts
 *
 *END*********************************************************************/
void sdhc_hal_enable_intr_signal(uint8_t instance, bool isEnabled, uint32_t mask)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    if (isEnabled)
    {
        HW_SDHC_IRQSIGEN_SET(mask);
    }
    else
    {
        HW_SDHC_IRQSIGEN_CLR(mask);
    }
}

/*FUNCTION****************************************************************
 *
 * Function Name: sdhc_hal_enable_intr_state
 * Description: Enable specified interrupts' state
 *
 *END*********************************************************************/
void sdhc_hal_enable_intr_state(uint8_t instance, bool isEnabled, uint32_t mask)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    if (isEnabled)
    {
        HW_SDHC_IRQSTATEN_SET(mask);
    }
    else
    {
        HW_SDHC_IRQSTATEN_CLR(mask);
    }
}

/*FUNCTION****************************************************************
 *
 * Function Name: sdhc_hal_get_resp
 * Description: get command response
 *
 *END*********************************************************************/
void sdhc_hal_get_resp(uint8_t instance, uint32_t * resp)
{
    assert(instance < HW_SDHC_INSTANCE_COUNT);
    resp[0] = BR_SDHC_CMDRSP0_CMDRSP0;
    resp[1] = BR_SDHC_CMDRSP1_CMDRSP1;
    resp[2] = BR_SDHC_CMDRSP2_CMDRSP2;
    resp[3] = BR_SDHC_CMDRSP3_CMDRSP3;
}

/*************************************************************************************************
 * EOF
 ************************************************************************************************/
