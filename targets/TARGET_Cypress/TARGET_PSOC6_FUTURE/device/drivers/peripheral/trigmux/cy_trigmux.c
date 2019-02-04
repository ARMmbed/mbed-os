/***************************************************************************//**
* \file cy_trigmux.c
* \version 1.10
*
* \brief Trigger mux APIs.
*
********************************************************************************
* \copyright
* Copyright 2016-2018, Cypress Semiconductor Corporation.  All rights reserved.
* SPDX-License-Identifier: Apache-2.0
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
* - true: The output trigger is inverted.
* - false: The output trigger is not inverted.
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
cy_en_trigmux_status_t Cy_TrigMux_Connect(uint32_t inTrig, uint32_t outTrig, bool invert, en_trig_type_t trigType)
{
    volatile uint32_t* trOutCtlAddr;
    cy_en_trigmux_status_t retVal = CY_TRIGMUX_BAD_PARAM;

    CY_ASSERT_L3(CY_LPCOMP_IS_TRIGTYPE_VALID(trigType));

    /* inTrig and outTrig should be in the same group */
    if ((inTrig & CY_TR_GROUP_MASK) == (outTrig & CY_TR_GROUP_MASK))
    {
        trOutCtlAddr = &(PERI->TR_GR[(outTrig & CY_TR_GROUP_MASK) >> CY_TR_GROUP_SHIFT].TR_OUT_CTL[outTrig & CY_TR_MASK]);

        *trOutCtlAddr = _VAL2FLD(PERI_TR_GR_TR_OUT_CTL_TR_SEL, inTrig) |
        _BOOL2FLD(PERI_TR_GR_TR_OUT_CTL_TR_INV, invert) |
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
* - Bit 30 represents if the signal is an input/output. When this bit is set,
*   the trigger activation is for an output trigger from the trigger multiplexer.
*   When this bit is reset, the trigger activation is for an input trigger to
*   the trigger multiplexer.
* - Bits 11:8 represent the trigger group selection.
* - Bits 6:0 select the output trigger number in the trigger group.
*
* \param cycles
* The number of cycles during which the trigger remains activated.
* The valid range of cycles is 1-254.
* These two additional special values can be passed to this parameter:
* * CY_TRIGGER_INFINITE - The trigger will be active until the user
* clears it;
* * CY_TRIGGER_DEACTIVATE - The trigger will be deactivated forcibly.
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

    CY_ASSERT_L2(0U == (trigLine & (uint32_t)~CY_TR_PARAM_MASK));
    CY_ASSERT_L2(CY_TR_CYCLES_MAX >= cycles);


    if (CY_TRIGGER_DEACTIVATE != cycles)
    {
        if (PERI_TR_CMD_ACTIVATE_Msk != (PERI->TR_CMD & PERI_TR_CMD_ACTIVATE_Msk))
        {
            /* Activate the trigger if it is not in the active state. */
            PERI->TR_CMD = _VAL2FLD(PERI_TR_CMD_TR_SEL, (trigLine & CY_TR_MASK)) |
                _VAL2FLD(PERI_TR_CMD_GROUP_SEL, ((trigLine & CY_TR_GROUP_MASK) >> CY_TR_GROUP_SHIFT)) |
                _VAL2FLD(PERI_TR_CMD_COUNT, cycles) |
                _VAL2FLD(PERI_TR_CMD_OUT_SEL, (trigLine & CY_TR_OUT_CTL_MASK) >> CY_TR_OUT_CTL_SHIFT) |
                _VAL2FLD(PERI_TR_CMD_ACTIVATE, CY_TR_ACTIVATE_ENABLE);

            retVal = CY_TRIGMUX_SUCCESS;
        }
    }
    else
    {
        if (PERI_TR_CMD_ACTIVATE_Msk == (PERI->TR_CMD & PERI_TR_CMD_ACTIVATE_Msk))
        {
            /* Forcibly deactivate the trigger. */
            PERI->TR_CMD = _VAL2FLD(PERI_TR_CMD_TR_SEL, (trigLine & CY_TR_MASK)) |
                _VAL2FLD(PERI_TR_CMD_GROUP_SEL, ((trigLine & CY_TR_GROUP_MASK) >> CY_TR_GROUP_SHIFT)) |
                _VAL2FLD(PERI_TR_CMD_OUT_SEL, (trigLine & CY_TR_OUT_CTL_MASK) >> CY_TR_OUT_CTL_SHIFT);

            retVal = CY_TRIGMUX_SUCCESS;
        }
    }

    return retVal;
}

/* [] END OF FILE */
