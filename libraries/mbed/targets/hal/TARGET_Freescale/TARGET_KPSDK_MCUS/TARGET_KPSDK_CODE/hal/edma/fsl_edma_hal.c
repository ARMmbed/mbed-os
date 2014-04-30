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
#include "fsl_edma_hal.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_hal_init
 * Description   : Init edma module.
 *
 *END**************************************************************************/
void edma_hal_init(uint32_t instance, const edma_config_t *init)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(init);

    edma_hal_set_minor_loop_mapping(instance, init->isEnableMinorLoopping);
    edma_hal_set_continuous_mode(instance, init->isEnableContinuousMode);
    edma_hal_set_halt_on_error(instance, init->isHaltOnError);
    if (init->isEnableRoundrobinArbitration)
    {
        edma_hal_set_roundrobin_channel_arbitration(instance);
    }
    else
    {
        edma_hal_set_fixed_priority_channel_arbitration(instance);
    }
    edma_hal_set_debug_mode(instance, init->isEnableDebug);

#if (FSL_FEATURE_DMA_CHANNEL_GROUP_COUNT > 0x1U)
    edma_hal_set_group_priority(instance, init->groupPriority);
    if (init->isEnableGroupRoundrobinArbitration)
    {
        edma_hal_set_roundrobin_group_arbitration(instance);
    }
    else
    {
        edma_hal_set_fixed_priority_group_arbitration(instance);
    }
#endif
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_hal_stcd_push_to_htcd
 * Description   : Copy the configuration data from the software TCD to hardware TCD.
 *
 *END**************************************************************************/
void edma_hal_stcd_push_to_htcd(uint32_t instance, uint32_t channel, edma_software_tcd_t *stcd)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);
    assert(stcd);

    HW_DMA_TCDn_SADDR_WR(instance, channel, stcd->SADDR);
    HW_DMA_TCDn_SOFF_WR(instance, channel, stcd->SOFF);
    HW_DMA_TCDn_ATTR_WR(instance, channel, stcd->ATTR);
    HW_DMA_TCDn_NBYTES_MLNO_WR(instance, channel, stcd->NBYTES_MLNO);
    HW_DMA_TCDn_SLAST_WR(instance, channel, stcd->SLAST);
    HW_DMA_TCDn_DADDR_WR(instance, channel, stcd->DADDR);
    HW_DMA_TCDn_DOFF_WR(instance, channel, stcd->DOFF);
    HW_DMA_TCDn_CITER_ELINKYES_WR(instance, channel, stcd->CITER_ELINKYES);
    HW_DMA_TCDn_DLASTSGA_WR(instance, channel, stcd->DLAST_SGA);
    HW_DMA_TCDn_CSR_WR(instance, channel, stcd->CSR);
    HW_DMA_TCDn_BITER_ELINKYES_WR(instance, channel, stcd->BITER_ELINKYES);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_hal_htcd_get_nbytes_configuration.
 * Description   : Get nbytes configuration data.
 *
 *END**************************************************************************/
uint32_t edma_hal_htcd_get_nbytes_configuration(uint32_t instance, uint32_t channel)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);

    if (BR_DMA_CR_EMLM(instance))
    {
        if (BR_DMA_TCDn_NBYTES_MLOFFYES_SMLOE(instance, channel) ||
                BR_DMA_TCDn_NBYTES_MLOFFYES_SMLOE(instance, channel))
        {
            return BR_DMA_TCDn_NBYTES_MLOFFYES_NBYTES(instance, channel);
        }
        else
        {
            return BR_DMA_TCDn_NBYTES_MLOFFNO_NBYTES(instance, channel);
        }
    }
    else
    {
        return BR_DMA_TCDn_NBYTES_MLNO_NBYTES(instance, channel);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_hal_htcd_get_current_major_count
 * Description   : Get current major loop count.
 *
 *END**************************************************************************/
uint32_t edma_hal_htcd_get_current_major_count(uint32_t instance, uint32_t channel)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);

    if (BR_DMA_TCDn_CITER_ELINKYES_ELINK(instance, channel))
    {
        return BR_DMA_TCDn_CITER_ELINKYES_CITER(instance, channel);
    }
    else
    {
        return BR_DMA_TCDn_CITER_ELINKNO_CITER(instance, channel);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_hal_htcd_get_begin_major_count
 * Description   : Get begin major loop count.
 *
 *END**************************************************************************/
uint32_t edma_hal_htcd_get_begin_major_count(uint32_t instance, uint32_t channel)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);

    if (BR_DMA_TCDn_BITER_ELINKYES_ELINK(instance, channel))
    {
        return BR_DMA_TCDn_BITER_ELINKYES_BITER(instance, channel);
    }
    else
    {
        return BR_DMA_TCDn_BITER_ELINKNO_BITER(instance, channel);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_hal_htcd_get_unfinished_bytes
 * Description   : Get the bytes number not be transferred for this hardware TCD.
 *
 *END**************************************************************************/
uint32_t edma_hal_htcd_get_unfinished_bytes(uint32_t instance, uint32_t channel)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);

    return edma_hal_htcd_get_current_major_count(instance, channel) *
                edma_hal_htcd_get_nbytes_configuration(instance, channel);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : edma_hal_htcd_get_finished_bytes
 * Description   : Get the bytes number already be transferred for this hardware TCD.
 *
 *END**************************************************************************/
uint32_t edma_hal_htcd_get_finished_bytes(uint32_t instance, uint32_t channel)
{
    assert(instance < HW_DMA_INSTANCE_COUNT);
    assert(channel < FSL_FEATURE_DMA_MODULE_CHANNEL);

    return (edma_hal_htcd_get_begin_major_count(instance, channel) - 
                edma_hal_htcd_get_current_major_count(instance, channel)) *
                        edma_hal_htcd_get_nbytes_configuration(instance, channel);
}
/*******************************************************************************
 * EOF
 ******************************************************************************/

