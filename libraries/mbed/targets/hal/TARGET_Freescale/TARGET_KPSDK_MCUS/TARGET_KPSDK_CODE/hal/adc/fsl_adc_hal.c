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

#include "fsl_adc_hal.h"
#include "fsl_device_registers.h"
#include <assert.h>

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : adc_hal_start_calibration 
 * Description   : This function is to clear the calibrate flag bit and then set
 * the enable bit to start the calibration.
 * 
 *END**************************************************************************/
adc_status_t adc_hal_start_calibration(uint32_t instance)
{
    assert(instance < HW_ADC_INSTANCE_COUNT);
    
    /* Execute the calibration */
    HW_ADC_SC3_SET(instance, BM_ADC_SC3_CALF); /* Clear the calibration's flag */
    BW_ADC_SC3_CAL(instance, 1U);  /* Enable the calibration */
    while (!adc_hal_is_conversion_completed(instance, 0U)) 
    {} /* Wait conversion is competed */
    if (adc_hal_is_calibration_fail(instance))
    {
        return kStatus_ADC_Failed; /* Check for calibration fail error and return */
    }
    return kStatus_ADC_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : adc_hal_get_calibration_PG 
 * Description   : This function is to get the CLP0 - CLP4 and CLPS and 
 * accumulate them, and then return the value that can be set to PG directly. 
 * 
 *END**************************************************************************/
uint32_t adc_hal_get_calibration_PG(uint32_t instance)
{
    assert(instance < HW_ADC_INSTANCE_COUNT);
    uint32_t cal_var;
    
    /* Calculate plus-side calibration */
    cal_var = 0U;
    cal_var += BR_ADC_CLP0_CLP0(instance);
    cal_var += BR_ADC_CLP1_CLP1(instance);
    cal_var += BR_ADC_CLP2_CLP2(instance);
    cal_var += BR_ADC_CLP3_CLP3(instance);
    cal_var += BR_ADC_CLP4_CLP4(instance);
    cal_var += BR_ADC_CLPS_CLPS(instance);
    cal_var = 0x8000U | (cal_var>>1U);
    
    return cal_var;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : adc_hal_get_calibration_MG 
 * Description   : This function is to get the CLM0 - CLM4 and CLMS and 
 * accumulate them, and then return the value that can be set to MG directly. 
 * 
 *END**************************************************************************/
uint32_t adc_hal_get_calibration_MG(uint32_t instance)
{
    assert(instance < HW_ADC_INSTANCE_COUNT);
    uint32_t cal_var;
    
    /* Calculate minus-side calibration */
    cal_var = 0U;
    cal_var += BR_ADC_CLM0_CLM0(instance);
    cal_var += BR_ADC_CLM1_CLM1(instance);
    cal_var += BR_ADC_CLM2_CLM2(instance);
    cal_var += BR_ADC_CLM3_CLM3(instance);
    cal_var += BR_ADC_CLM4_CLM4(instance);
    cal_var += BR_ADC_CLMS_CLMS(instance);
    cal_var = 0x8000U | (cal_var>>1U);
    
    return cal_var;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
