/* mbed Microcontroller Library
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "objects.h"
#include "analogout_api.h"


#if CONFIG_DAC_EN

#include "cmsis.h"
#include "pinmap.h"
#include <string.h>

#define DAC_POSITIVE_FULL_SCALE  0x7E0
#define DAC_NEGATIVE_FULL_SCALE  0x820


/** \brief analogout_init:\n
 *          to initialize DAC
 *  
 *   This function is mainly to initialize a DAC channel.
 *   \para dac_t *: obj
 *   \para PinName: pin
 */
void analogout_init(dac_t *obj, PinName pin)
{
    uint32_t dac_idx;
    uint32_t DacTemp;
    PHAL_DAC_INIT_DAT   pHalDacInitData = (PHAL_DAC_INIT_DAT)&(obj->DACpara);
    dac_idx = pin & 0x0F;

    /* Assign dac index */
    pHalDacInitData->DACIdx = dac_idx;

    pHalDacInitData->DACEn         =   DAC_DISABLE;
    pHalDacInitData->DACDataRate   =   DAC_DATA_RATE_250K;
    pHalDacInitData->DACEndian     =   DAC_DATA_ENDIAN_LITTLE;
    pHalDacInitData->DACBurstSz    =   10;
    pHalDacInitData->DACDbgSel     =   DAC_DBG_SEL_DISABLE;
    pHalDacInitData->DACDscDbgSel  =   DAC_DSC_DBG_SEL_DISABLE;
    pHalDacInitData->DACBPDsc      =   DAC_BYPASS_DSC_SEL_DISABLE;
    pHalDacInitData->DACDeltaSig   =   0;
    pHalDacInitData->DACAnaCtrl0   =   0;
    pHalDacInitData->DACAnaCtrl1   =   0;
    pHalDacInitData->DACIntrMSK    =   DAC_FEATURE_DISABLED;
    
    /* DAC Function and Clock Enable*/
    HalDACPinMuxInit(pHalDacInitData);

    HalDACInit8195a(pHalDacInitData);
    
    HAL_DAC_WRITE32(pHalDacInitData->DACIdx, REG_DAC_INTR_CTRL, 
                                (BIT_DAC_FIFO_FULL_EN       |
                                 BIT_DAC_FIFO_OVERFLOW_EN   |
                                 BIT_DAC_FIFO_STOP_EN       |
                                 BIT_DAC__WRITE_ERROR_EN    |
                                 BIT_DAC_DSC_OVERFLOW0_EN   |
                                 BIT_DAC_DSC_OVERFLOW1_EN));
    DBG_DAC_INFO("INTR MSK:%x\n", HAL_DAC_READ32(pHalDacInitData->DACIdx,REG_DAC_INTR_CTRL));

    DacTemp = HAL_DAC_READ32(pHalDacInitData->DACIdx, REG_DAC_ANAPAR_DA1);
    DacTemp |= (BIT31);
    HAL_DAC_WRITE32(pHalDacInitData->DACIdx, REG_DAC_ANAPAR_DA1, DacTemp);
    DBG_DAC_INFO("REG_DAC_ANAPAR_DA1:%08x\n",HAL_DAC_READ32(pHalDacInitData->DACIdx, REG_DAC_ANAPAR_DA1));
    
    DacTemp = HAL_DAC_READ32(pHalDacInitData->DACIdx, REG_DAC_CTRL);
    DacTemp |= BIT3;
    HAL_DAC_WRITE32(pHalDacInitData->DACIdx, REG_DAC_CTRL, DacTemp);
    DBG_DAC_INFO("REG_DAC_CTRL:%08x\n",DacTemp);
    
    pHalDacInitData->DACEn  = DAC_ENABLE;
    HalDACEnableRtl8195a(pHalDacInitData);
    osDelay(6); //hardware needs some time to get ready
}

/** \brief analogout_free:\n
 *          to free DAC
 *  
 *   This function is mainly to free a DAC channel.
 *   \para dac_t *: obj
 */
void analogout_free(dac_t *obj)
{
    PHAL_DAC_INIT_DAT   pHalDacInitData = (PHAL_DAC_INIT_DAT)&(obj->DACpara);

    HalDACPinMuxDeInit(pHalDacInitData);

    pHalDacInitData->DACEn  = DAC_DISABLE;
    HalDACEnableRtl8195a(pHalDacInitData);
}

/** \brief analogout_write:\n
 *          to execute analogout_write
 *  
 *   This function is mainly to execute analog output and the value is a ratio.
 *   The upper/lower bound of DAC register input value is defined by 
 *   DAC_XXXXX_FULL_SCALE. The parameter "value" of this function should be 
 *   transfered to register value.
 *   
 *   \para dac_t *  : obj
 *   \para float      : value
 */
void analogout_write(dac_t *obj, float value)
{
    uint32_t    dactemp;
    uint16_t    dacnegtemp;
    PHAL_DAC_INIT_DAT   pHalDacInitData = (PHAL_DAC_INIT_DAT)&(obj->DACpara);
    
    if (value < 0.0f) {
        HAL_DAC_WRITE32(pHalDacInitData->DACIdx, REG_DAC0_FIFO_WR, 0x00000000);
    } else if (value > 1.0f) {
        dactemp = (DAC_POSITIVE_FULL_SCALE<<16) | DAC_POSITIVE_FULL_SCALE;
        HAL_DAC_WRITE32(pHalDacInitData->DACIdx, REG_DAC0_FIFO_WR, dactemp);
    } else {
        if (value >= 0.5) {
            dactemp     = (uint32_t)((((value-0.5)/0.5) * (2^12)) * DAC_POSITIVE_FULL_SCALE);
            dactemp     = dactemp / (2^12);
            dactemp     = (dactemp<<16) | dactemp;
            HAL_DAC_WRITE32(pHalDacInitData->DACIdx, REG_DAC0_FIFO_WR, dactemp);
        } else {
            dacnegtemp  = (DAC_NEGATIVE_FULL_SCALE & 0x7FF);
            dacnegtemp  = ((~dacnegtemp) + 1) & 0x7FF;
            dactemp     = (uint32_t)(((0.5-value)/0.5) * (2^12) * dacnegtemp);
            dactemp     = dactemp / (2^12);
            dactemp     = 0x1000 - dactemp; //change to 2's complement
            dactemp     = (dactemp<<16) | dactemp;
            HAL_DAC_WRITE32(pHalDacInitData->DACIdx, REG_DAC0_FIFO_WR, dactemp);
        }
    }
}

/** \brief analogout_write_u16:\n
 *          to execute analogout_write_u16
 *
 *   The register value of DAC input is a format of 2's complement.
 *   The most maximum value of positive value drives DAC to output a voltage about 3.3V.
 *   The most mimimum value of negative value drives DAC to output a voltage about 0.
 *   And the middle value of 0x000 will drive DAC to output a voltage of half of max voltage.
 *
 *   \para dac_t *  : obj
 *   \para float      : value
 */
void analogout_write_u16(dac_t *obj, uint16_t value)
{
    uint32_t    dactemp;
    PHAL_DAC_INIT_DAT   pHalDacInitData = (PHAL_DAC_INIT_DAT)&(obj->DACpara);
    
    /* To give a two point data */
    dactemp = (value << 16) | value;
    HAL_DAC_WRITE32(pHalDacInitData->DACIdx, REG_DAC0_FIFO_WR, dactemp);
}

/** \brief analogout_read_u16:\n
 *          to read back analog output value in float format
 *  
 *   This function is NOT available in rtl8195a hardware design.
 *   It always returns a fixed value of 0.0;
 *   \para dac_t *  : obj
 */
float analogout_read(dac_t *obj)
{
    return (float)0.0;
}

/** \brief analogout_read_u16:\n
 *          to read back analog output register value
 *  
 *   This function is NOT available in rtl8195a hardware design.
 *   It always returns a fixed value of 0xFFFF;
 *   \para dac_t *  : obj
 */
uint16_t analogout_read_u16(dac_t *obj)
{
    return (uint16_t)0xFFFF;
}

#endif
