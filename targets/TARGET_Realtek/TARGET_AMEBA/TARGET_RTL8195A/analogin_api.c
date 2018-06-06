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
#include "PinNames.h"
#include "hal_adc.h"
#include "analogin_api.h"

#ifdef CONFIG_MBED_ENABLED
#include "platform_stdlib.h"
#endif

#if CONFIG_ADC_EN
#include "pinmap.h"


extern u32 ConfigDebugErr;
extern u32 ConfigDebuginfo;


void analogin_init (analogin_t *obj, PinName pin)
{
    uint32_t adc_idx;
    PSAL_ADC_MNGT_ADPT      pSalADCMngtAdpt     = NULL;
    PSAL_ADC_USERCB_ADPT    pSalADCUserCBAdpt   = NULL;
    PSAL_ADC_HND            pSalADCHND          = NULL;

    HAL_ADC_INIT_DAT        HalADCInitDataTmp;
    PHAL_ADC_INIT_DAT       pHalADCInitDataTmp = &HalADCInitDataTmp;
    /* To backup user config first */
    
    _memset(&(obj->HalADCInitData), 0, sizeof(HAL_ADC_INIT_DAT));
    _memcpy(pHalADCInitDataTmp, &(obj->HalADCInitData), sizeof(HAL_ADC_INIT_DAT));
    _memset(obj, 0x00, sizeof(analogin_t));
    
    ConfigDebugErr &= (~(_DBG_ADC_|_DBG_GDMA_));
    ConfigDebugInfo&= (~(_DBG_ADC_|_DBG_GDMA_));

    adc_idx = pin & 0x0F;
    
    /* Get I2C device handler */
    pSalADCMngtAdpt     = &(obj->SalADCMngtAdpt);
    pSalADCUserCBAdpt   = (PSAL_ADC_USERCB_ADPT)&(obj->SalADCUserCBAdpt);   

    /*To assign the rest pointers*/
    pSalADCMngtAdpt->pSalHndPriv    = &(obj->SalADCHndPriv);
    pSalADCMngtAdpt->pSalHndPriv->ppSalADCHnd = (void**)&(pSalADCMngtAdpt->pSalHndPriv);

    /* To assign the default (ROM) HAL OP initialization function */
    pSalADCMngtAdpt->pHalOpInit         = &HalADCOpInit;

    /* To assign the default (ROM) HAL GDMA OP initialization function */
    pSalADCMngtAdpt->pHalGdmaOpInit     = &HalGdmaOpInit;

    /* To assign the default (ROM) SAL interrupt function */
    pSalADCMngtAdpt->pSalIrqFunc        = &ADCISRHandle;

    /* To assign the default (ROM) SAL DMA TX interrupt function */
    pSalADCMngtAdpt->pSalDMAIrqFunc     = &ADCGDMAISRHandle;

    /* To backup user config first */
    //_memcpy(pHalADCInitDataTmp, &(obj->HalADCInitData), sizeof(HAL_ADC_INIT_DAT));
    
    pSalADCMngtAdpt->pHalInitDat        = &(obj->HalADCInitData);
    pSalADCMngtAdpt->pHalOp             = &(obj->HalADCOp);
    pSalADCMngtAdpt->pIrqHnd            = &(obj->ADCIrqHandleDat);
    pSalADCMngtAdpt->pHalGdmaAdp        = &(obj->HalADCGdmaAdpt);
    pSalADCMngtAdpt->pHalGdmaOp         = &(obj->HalADCGdmaOp);
    pSalADCMngtAdpt->pIrqGdmaHnd        = &(obj->ADCGdmaIrqHandleDat);
    pSalADCMngtAdpt->pUserCB            = &(obj->SalADCUserCB);
    
    /* Assign the private SAL handle to public SAL handle */
    pSalADCHND      = &(pSalADCMngtAdpt->pSalHndPriv->SalADCHndPriv);

    /* Assign the internal HAL initial data pointer to the SAL handle */
    pSalADCHND->pInitDat    = pSalADCMngtAdpt->pHalInitDat;

    /* Assign the internal user callback pointer to the SAL handle */
    pSalADCHND->pUserCB     = pSalADCMngtAdpt->pUserCB;

    /*To assign user callback pointers*/
    pSalADCMngtAdpt->pUserCB->pTXCB     = pSalADCUserCBAdpt;
    pSalADCMngtAdpt->pUserCB->pTXCCB    = (pSalADCUserCBAdpt+1);
    pSalADCMngtAdpt->pUserCB->pRXCB     = (pSalADCUserCBAdpt+2);
    pSalADCMngtAdpt->pUserCB->pRXCCB    = (pSalADCUserCBAdpt+3);
    pSalADCMngtAdpt->pUserCB->pRDREQCB  = (pSalADCUserCBAdpt+4);
    pSalADCMngtAdpt->pUserCB->pERRCB    = (pSalADCUserCBAdpt+5);
    pSalADCMngtAdpt->pUserCB->pDMATXCB  = (pSalADCUserCBAdpt+6);
    pSalADCMngtAdpt->pUserCB->pDMATXCCB = (pSalADCUserCBAdpt+7);
    pSalADCMngtAdpt->pUserCB->pDMARXCB  = (pSalADCUserCBAdpt+8);
    pSalADCMngtAdpt->pUserCB->pDMARXCCB = (pSalADCUserCBAdpt+9);
        
    /* Set ADC Device Number */
    pSalADCHND->DevNum = adc_idx;

    /* Load ADC default value */
    RtkADCLoadDefault(pSalADCHND);

    /* Assign ADC Pin Mux */
    pSalADCHND->PinMux        = 0;
    pSalADCHND->OpType        = ADC_RDREG_TYPE;

    /* Load user setting */
    if ((pHalADCInitDataTmp->ADCEndian == ADC_DATA_ENDIAN_LITTLE) || (pHalADCInitDataTmp->ADCEndian == ADC_DATA_ENDIAN_BIG)) {
        pSalADCHND->pInitDat->ADCEndian = pHalADCInitDataTmp->ADCEndian;
    }
    
    if ((pHalADCInitDataTmp->ADCAudioEn != ADC_FEATURE_DISABLED) && (pHalADCInitDataTmp->ADCAudioEn < 2)) {
        pSalADCHND->pInitDat->ADCAudioEn = pHalADCInitDataTmp->ADCAudioEn;
    }
    
    /* Init ADC now */
    pSalADCHND->pInitDat->ADCBurstSz       =   8;
    pSalADCHND->pInitDat->ADCOneShotTD     =   8;
    RtkADCInit(pSalADCHND); 
}

float analogin_read(analogin_t *obj)
{
    float value;
    uint32_t AnaloginTmp[2]      = {0,0};
    uint32_t AnaloginDatMsk      = 0xFFFF;
    uint8_t  AnaloginIdx         = 0;
    uint32_t AnalogDat           = 0;

    //no auto-calibration implemented yet, uses hard coded calibrate
    uint32_t Offset        = 0x2980;
    uint32_t AnalogDatFull = 0xAA00;

    PSAL_ADC_MNGT_ADPT      pSalADCMngtAdpt     = NULL;
    PSAL_ADC_HND            pSalADCHND          = NULL;
    
    pSalADCMngtAdpt         = &(obj->SalADCMngtAdpt);
    pSalADCHND              = &(pSalADCMngtAdpt->pSalHndPriv->SalADCHndPriv);
    AnaloginIdx             = pSalADCHND->DevNum;
    RtkADCReceiveBuf(pSalADCHND,&AnaloginTmp[0]);
    
    AnaloginDatMsk = (u32)(AnaloginDatMsk<<((u32)(16*(AnaloginIdx&0x01))));
    AnalogDat = AnaloginTmp[(AnaloginIdx/2)];
    AnalogDat = (AnalogDat & AnaloginDatMsk);    
    AnalogDat = (AnalogDat>>((u32)(16*(AnaloginIdx&0x01))));
    AnalogDat -= Offset;

    value = (float)(AnalogDat) / (float)(AnalogDatFull);
    return (float)value;
}

uint16_t analogin_read_u16(analogin_t *obj)
{
    uint32_t AnaloginTmp[2]      = {0,0};
    uint32_t AnaloginDatMsk      = 0xFFFF;
    uint8_t  AnaloginIdx         = 0;
    uint32_t AnalogDat           = 0;
    
    PSAL_ADC_MNGT_ADPT      pSalADCMngtAdpt     = NULL;
    PSAL_ADC_HND            pSalADCHND          = NULL;

    pSalADCMngtAdpt         = &(obj->SalADCMngtAdpt);
    pSalADCHND              = &(pSalADCMngtAdpt->pSalHndPriv->SalADCHndPriv);
    AnaloginIdx             = pSalADCHND->DevNum;
    RtkADCRxManualRotate(pSalADCHND,&AnaloginTmp[0]);
    
    AnaloginDatMsk = (u32)(AnaloginDatMsk<<((u32)(16*(AnaloginIdx&0x01))));
    AnalogDat = AnaloginTmp[(AnaloginIdx/2)];
    AnalogDat = (AnalogDat & AnaloginDatMsk);    
    AnalogDat = (AnalogDat>>((u32)(16*(AnaloginIdx&0x01))));

    return (uint16_t)AnalogDat;
}


void  analogin_deinit(analogin_t *obj)
{
    PSAL_ADC_MNGT_ADPT      pSalADCMngtAdpt     = NULL;
    PSAL_ADC_HND            pSalADCHND          = NULL;
    
    pSalADCMngtAdpt         = &(obj->SalADCMngtAdpt);
    pSalADCHND              = &(pSalADCMngtAdpt->pSalHndPriv->SalADCHndPriv);

    /* To deinit analogin */
    RtkADCDeInit(pSalADCHND);    
}
#endif
