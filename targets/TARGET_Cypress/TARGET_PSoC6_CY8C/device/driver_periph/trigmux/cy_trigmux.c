/***************************************************************************//**
* \file
* \version 1.0
*
* \brief Trigger mux APIs.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/
#include "cy_trigmux.h"


/*******************************************************************************
* Function Name: Cy_TrigMux_Connect
****************************************************************************//**
*
* This function connects an input trigger source and output trigger.
*
* \param inTrig
* An input selection for the trigger mux.
* - Bits 11:8 represent the trigger group selection.
* - Bits 6:0 select the input trigger number in the trigger group.
*
* \param outTrig
* The output of the trigger mux. This refers to the consumer of the trigger mux.
* - Bits 11:8 represent the trigger group selection.
* - Bits 6:0 select the output trigger number in the trigger group.
*
* \param invert
* - CY_TR_MUX_TR_INV_ENABLE: The output trigger is inverted.
* - CY_MUX_TR_INV_DISABLE: The output trigger is not inverted.
*
* \param trigType
* - TRIGGER_TYPE_EDGE: The trigger is synchronized to the consumer blocks clock
*   and a two-cycle pulse is generated on this clock.
* - TRIGGER_TYPE_LEVEL: The trigger is a simple level output.
*
* \return
* A status
* - 0: Successful connection made.
* - 1: An invalid input selection corresponding to the output.
*      Generally when the trigger groups do not match.
*
*******************************************************************************/
cy_en_trigmux_status_t Cy_TrigMux_Connect(uint32_t inTrig, uint32_t outTrig, uint32_t invert, en_trig_type_t trigType)
{
    volatile uint32_t* trOutCtlAddr;
    cy_en_trigmux_status_t retVal = CY_TRIGMUX_BAD_PARAM;

    if ((inTrig & CY_TR_GROUP_MASK) == (outTrig & CY_TR_GROUP_MASK))
    {
        trOutCtlAddr = &(PERI->TR_GR[(outTrig & CY_TR_GROUP_MASK) >> CY_TR_GROUP_SHIFT].TR_OUT_CTL[outTrig & CY_TR_MASK]);

        *trOutCtlAddr = _VAL2FLD(PERI_TR_GR_TR_OUT_CTL_TR_SEL, inTrig) |
        _VAL2FLD(PERI_TR_GR_TR_OUT_CTL_TR_INV, invert) |
        _VAL2FLD(PERI_TR_GR_TR_OUT_CTL_TR_EDGE, trigType);

        retVal = CY_TRIGMUX_SUCCESS;
    }

    return retVal;
}


/*******************************************************************************
* Function Name: Cy_TrigMux_SwTrigger
****************************************************************************//**
*
* This function generates a software trigger on an input trigger line.
* All output triggers connected to this input trigger will be triggered.
* The function also verifies that there is no activated trigger before
* generating another activation.
*
* \param trigLine
* The input of the trigger mux.
* - Bit 12 represents if the signal is an input/output.
* - Bits 11:8 represent the trigger group selection.
* - Bits 6:0 select the output trigger number in the trigger group.
*
* \param cycles
* The number of cycles during which the trigger remains activated.
* - The valid range: 1-254 cycles.
*
* \return
* A status
* - 0: If there was not an already activated trigger.
* - 1: If there was an already activated trigger.
*
*******************************************************************************/
cy_en_trigmux_status_t Cy_TrigMux_SwTrigger(uint32_t trigLine, uint32_t cycles)
{
    cy_en_trigmux_status_t retVal = CY_TRIGMUX_INVALID_STATE;

    if (PERI_TR_CMD_ACTIVATE_Msk != (PERI->TR_CMD & PERI_TR_CMD_ACTIVATE_Msk))
    {
        PERI->TR_CMD = _VAL2FLD(PERI_TR_CMD_TR_SEL, (trigLine & CY_TR_MASK)) |
            _VAL2FLD(PERI_TR_CMD_GROUP_SEL, ((trigLine & CY_TR_GROUP_MASK) >> CY_TR_GROUP_SHIFT)) |
            _VAL2FLD(PERI_TR_CMD_COUNT, cycles) |
            _VAL2FLD(PERI_TR_CMD_OUT_SEL, (trigLine & CY_TR_OUT_CTL_MASK) >> CY_TR_OUT_CTL_SHIFT) |
            _VAL2FLD(PERI_TR_CMD_ACTIVATE, CY_TR_ACTIVATE_ENABLE);

        retVal = CY_TRIGMUX_SUCCESS;
    }

    return retVal;
}

/* [] END OF FILE */
