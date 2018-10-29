/***************************************************************************//**
* \file cy_tcpwm_quaddec.c
* \version 1.0.1
*
* \brief
*  The source file of the tcpwm driver.
*
********************************************************************************
* \copyright
* Copyright 2016-2018, Cypress Semiconductor Corporation.  All rights reserved.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/

#include "cy_tcpwm_quaddec.h"

#if defined(__cplusplus)
extern "C" {
#endif


/*******************************************************************************
* Function Name: Cy_TCPWM_QuadDec_Init
****************************************************************************//**
*
* Initializes the counter in the TCPWM block for the QuadDec operation.
*
* \param base
* The pointer to a TCPWM instance.
*
* \param cntNum
* The Counter instance number in the selected TCPWM.
*
* \param config
* The pointer to a configuration structure. See \ref cy_stc_tcpwm_quaddec_config_t.
*
* \return error / status code. See \ref cy_en_tcpwm_status_t.
*
* \funcusage
* \snippet tcpwm/tcpwm_v1_0_quaddec_sut_01.cydsn/main_cm4.c snippet_Cy_TCPWM_QuadDec_Config
* \snippet tcpwm/tcpwm_v1_0_quaddec_sut_01.cydsn/main_cm4.c snippet_Cy_TCPWM_QuadDec_Init
*
*******************************************************************************/
cy_en_tcpwm_status_t Cy_TCPWM_QuadDec_Init(TCPWM_Type *base, uint32_t cntNum,
                                           cy_stc_tcpwm_quaddec_config_t const *config)
{
    cy_en_tcpwm_status_t status = CY_TCPWM_BAD_PARAM;

    if ((NULL != base) && (NULL != config))
    {
        base->CNT[cntNum].CTRL = ( _VAL2FLD(TCPWM_CNT_CTRL_QUADRATURE_MODE, config->resolution) |
                            _VAL2FLD(TCPWM_CNT_CTRL_MODE, CY_TCPWM_QUADDEC_CTRL_QUADDEC_MODE));

        if (CY_TCPWM_INPUT_CREATOR != config->phiAInput)
        {
            base->CNT[cntNum].TR_CTRL0 = (_VAL2FLD(TCPWM_CNT_TR_CTRL0_COUNT_SEL, config->phiAInput) |
                                          _VAL2FLD(TCPWM_CNT_TR_CTRL0_START_SEL, config->phiBInput) |
                                          _VAL2FLD(TCPWM_CNT_TR_CTRL0_RELOAD_SEL, config->indexInput) |
                                          _VAL2FLD(TCPWM_CNT_TR_CTRL0_STOP_SEL, config->stopInput));
        }

        base->CNT[cntNum].TR_CTRL1 = (_VAL2FLD(TCPWM_CNT_TR_CTRL1_CAPTURE_EDGE, CY_TCPWM_INPUT_LEVEL) |
                                      _VAL2FLD(TCPWM_CNT_TR_CTRL1_COUNT_EDGE, CY_TCPWM_INPUT_LEVEL) |
                                      _VAL2FLD(TCPWM_CNT_TR_CTRL1_START_EDGE, CY_TCPWM_INPUT_LEVEL) |
                                      _VAL2FLD(TCPWM_CNT_TR_CTRL1_RELOAD_EDGE, config->indexInputMode) |
                                      _VAL2FLD(TCPWM_CNT_TR_CTRL1_STOP_EDGE, config->stopInputMode));

        base->CNT[cntNum].INTR_MASK = config->interruptSources;

        status = CY_TCPWM_SUCCESS;
    }

    return(status);
}


/*******************************************************************************
* Function Name: Cy_TCPWM_QuadDec_DeInit
****************************************************************************//**
*
* De-initializes the counter in the TCPWM block, returns register values to
* default.
*
* \param base
* The pointer to a TCPWM instance.
*
* \param cntNum
* The Counter instance number in the selected TCPWM.
*
* \param config
* The pointer to a configuration structure. See \ref cy_stc_tcpwm_quaddec_config_t.
*
* \funcusage
* \snippet tcpwm/tcpwm_v1_0_quaddec_sut_01.cydsn/main_cm4.c snippet_Cy_TCPWM_QuadDec_DeInit
*
*******************************************************************************/
void Cy_TCPWM_QuadDec_DeInit(TCPWM_Type *base, uint32_t cntNum, cy_stc_tcpwm_quaddec_config_t const *config)
{
    base->CNT[cntNum].CTRL = CY_TCPWM_CNT_CTRL_DEFAULT;
    base->CNT[cntNum].COUNTER = CY_TCPWM_CNT_COUNTER_DEFAULT;
    base->CNT[cntNum].CC = CY_TCPWM_CNT_CC_DEFAULT;
    base->CNT[cntNum].CC_BUFF = CY_TCPWM_CNT_CC_BUFF_DEFAULT;
    base->CNT[cntNum].PERIOD = CY_TCPWM_CNT_PERIOD_DEFAULT;
    base->CNT[cntNum].PERIOD_BUFF = CY_TCPWM_CNT_PERIOD_BUFF_DEFAULT;
    base->CNT[cntNum].TR_CTRL1 = CY_TCPWM_CNT_TR_CTRL1_DEFAULT;
    base->CNT[cntNum].TR_CTRL2 = CY_TCPWM_CNT_TR_CTRL2_DEFAULT;
    base->CNT[cntNum].INTR = CY_TCPWM_CNT_INTR_DEFAULT;
    base->CNT[cntNum].INTR_SET = CY_TCPWM_CNT_INTR_SET_DEFAULT;
    base->CNT[cntNum].INTR_MASK = CY_TCPWM_CNT_INTR_MASK_DEFAULT;

    if (CY_TCPWM_INPUT_CREATOR != config->phiAInput)
    {
        base->CNT[cntNum].TR_CTRL0 = CY_TCPWM_CNT_TR_CTRL0_DEFAULT;
    }
}

#if defined(__cplusplus)
}
#endif


/* [] END OF FILE */
