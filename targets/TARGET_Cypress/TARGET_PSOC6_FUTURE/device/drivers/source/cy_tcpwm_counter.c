/***************************************************************************//**
* \file cy_tcpwm_counter.c
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

#include "cy_tcpwm_counter.h"

#if defined(__cplusplus)
extern "C" {
#endif


/*******************************************************************************
* Function Name: Cy_TCPWM_Counter_Init
****************************************************************************//**
*
* Initializes the counter in the TCPWM block for the Counter operation.
*
* \param base
* The pointer to a TCPWM instance.
*
* \param cntNum
* The Counter instance number in the selected TCPWM.
*
* \param config
* The pointer to configuration structure. See \ref cy_stc_tcpwm_counter_config_t.
*
* \return error / status code. See \ref cy_en_tcpwm_status_t.
*
* \funcusage
* \snippet tcpwm/tcpwm_v1_0_counter_sut_01.cydsn/main_cm4.c snippet_Cy_TCPWM_Counter_Config
* \snippet tcpwm/tcpwm_v1_0_counter_sut_01.cydsn/main_cm4.c snippet_Cy_TCPWM_Counter_Init
*
*******************************************************************************/
cy_en_tcpwm_status_t Cy_TCPWM_Counter_Init(TCPWM_Type *base, uint32_t cntNum,
                                           cy_stc_tcpwm_counter_config_t const *config)
{
    cy_en_tcpwm_status_t status = CY_TCPWM_BAD_PARAM;

    if ((NULL != base) && (NULL != config))
    {
        base->CNT[cntNum].CTRL = (_VAL2FLD(TCPWM_CNT_CTRL_GENERIC, config->clockPrescaler) |
                                  _VAL2FLD(TCPWM_CNT_CTRL_ONE_SHOT, config->runMode) |
                                  _VAL2FLD(TCPWM_CNT_CTRL_UP_DOWN_MODE, config->countDirection) |
                                  _VAL2FLD(TCPWM_CNT_CTRL_MODE, config->compareOrCapture) |
                                  (config->enableCompareSwap ? TCPWM_CNT_CTRL_AUTO_RELOAD_CC_Msk : 0UL));

        if (CY_TCPWM_COUNTER_COUNT_UP == config->countDirection)
        {
            base->CNT[cntNum].COUNTER = CY_TCPWM_CNT_UP_INIT_VAL;
        }
        else if (CY_TCPWM_COUNTER_COUNT_DOWN == config->countDirection)
        {
            base->CNT[cntNum].COUNTER = config->period;
        }
        else
        {
            base->CNT[cntNum].COUNTER = CY_TCPWM_CNT_UP_DOWN_INIT_VAL;
        }

        if (CY_TCPWM_COUNTER_MODE_COMPARE == config->compareOrCapture)
        {
            base->CNT[cntNum].CC = config->compare0;
            base->CNT[cntNum].CC_BUFF = config->compare1;
        }

        base->CNT[cntNum].PERIOD = config->period;

        if (CY_TCPWM_INPUT_CREATOR != config->countInput)
        {
            base->CNT[cntNum].TR_CTRL0 = (_VAL2FLD(TCPWM_CNT_TR_CTRL0_CAPTURE_SEL, config->captureInput) |
                                          _VAL2FLD(TCPWM_CNT_TR_CTRL0_RELOAD_SEL, config->reloadInput) |
                                          _VAL2FLD(TCPWM_CNT_TR_CTRL0_START_SEL, config->startInput) |
                                          _VAL2FLD(TCPWM_CNT_TR_CTRL0_STOP_SEL, config->stopInput) |
                                          _VAL2FLD(TCPWM_CNT_TR_CTRL0_COUNT_SEL, config->countInput));
        }

        base->CNT[cntNum].TR_CTRL1 = (_VAL2FLD(TCPWM_CNT_TR_CTRL1_CAPTURE_EDGE, config->captureInputMode) |
                                      _VAL2FLD(TCPWM_CNT_TR_CTRL1_RELOAD_EDGE, config->reloadInputMode) |
                                      _VAL2FLD(TCPWM_CNT_TR_CTRL1_START_EDGE, config->startInputMode) |
                                      _VAL2FLD(TCPWM_CNT_TR_CTRL1_STOP_EDGE, config->stopInputMode) |
                                      _VAL2FLD(TCPWM_CNT_TR_CTRL1_COUNT_EDGE, config->countInputMode));

        base->CNT[cntNum].INTR_MASK = config->interruptSources;

        status = CY_TCPWM_SUCCESS;
    }

    return(status);
}


/*******************************************************************************
* Function Name: Cy_TCPWM_Counter_DeInit
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
* The pointer to configuration structure. See \ref cy_stc_tcpwm_counter_config_t.
*
* \funcusage
* \snippet tcpwm/tcpwm_v1_0_counter_sut_01.cydsn/main_cm4.c snippet_Cy_TCPWM_Counter_DeInit
*
*******************************************************************************/
void Cy_TCPWM_Counter_DeInit(TCPWM_Type *base, uint32_t cntNum, cy_stc_tcpwm_counter_config_t const *config)
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

    if (CY_TCPWM_INPUT_CREATOR != config->countInput)
    {
        base->CNT[cntNum].TR_CTRL0 = CY_TCPWM_CNT_TR_CTRL0_DEFAULT;
    }
}

#if defined(__cplusplus)
}
#endif


/* [] END OF FILE */
