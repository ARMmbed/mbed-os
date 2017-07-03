/***************************************************************************//**
* \file cy_crypto_core_util.h
* \version 1.0
*
* \brief
*  This file provides the headers to the API for the utils
*  in the Crypto driver.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#if !defined(CY_CRYPTO_CORE_UTIL_H)
#define CY_CRYPTO_CORE_UTIL_H

#include "cy_crypto_common.h"
#include "syslib/cy_syslib.h"
#include <stdbool.h>


#if (CY_CPU_CORTEX_M0P)

#if (CPUSS_CRYPTO_PRESENT == 1)


extern bool cy_crypto_IsHwErrorOccured;

typedef enum {
    CY_CRYPTO_CTL_ENABLED_DISABLED  = 0u,
    CY_CRYPTO_CTL_ENABLED_ENABLED   = 1u,
} cy_en_crypto_hw_enable_t;


cy_en_crypto_status_t Cy_Crypto_Core_Enable(void);

cy_en_crypto_status_t Cy_Crypto_Core_Disable(void);

cy_en_crypto_status_t Cy_Crypto_Core_CheckHwForErrors(cy_stc_crypto_context_t *cryptoContext);

void Cy_Crypto_SetReg1Instr(uint32_t data0);

void Cy_Crypto_SetReg2Instr(uint32_t data0, uint32_t data1);

void Cy_Crypto_SetReg3Instr(uint32_t data0, uint32_t data1, uint32_t data2);

void Cy_Crypto_SetReg4Instr(uint32_t data0, uint32_t data1, uint32_t data2, uint32_t data3);

void Cy_Crypto_Run0ParamInstr(uint8_t instr);

void Cy_Crypto_Run1ParamInstr(uint8_t instr,
                           uint32_t rdst0Shift);

void Cy_Crypto_Run2ParamInstr(uint8_t instr,
                           uint32_t rdst0Shift,
                           uint32_t rdst1Shift);

void Cy_Crypto_Run3ParamInstr(uint8_t instr,
                           uint8_t rdst0Shift,
                           uint8_t rdst1Shift,
                           uint8_t rdst2Shift);

void Cy_Crypto_Run4ParamInstr(uint8_t instr,
                           uint32_t rdst0Shift,
                           uint32_t rdst1Shift,
                           uint32_t rdst2Shift,
                           uint32_t rdst3Shift);

uint64_t Cy_Crypto_InvertEndian8(uint64_t srcVal);

/*******************************************************************************
* Function Name: Crypto_Server_GetFIFOUsed
****************************************************************************//**
*
* Returns the number of instructions in the instruction FIFO.
* The value of this field ranges from 0 to 8
*
*******************************************************************************/
__STATIC_INLINE uint8_t Cy_Crypto_Core_GetFIFOUsed(void)
{
    return((uint8_t)_FLD2VAL(CRYPTO_INSTR_FF_STATUS_USED, CRYPTO->INSTR_FF_STATUS));
}

/*******************************************************************************
* Function Name: Crypto_Server_WaitForFifoAvailable
*****************************************************************************//**
*
* Waits until number of entries in the instruction FIFO is less than
* specified in EVENT_LEVEL field in FF_CTL register, an event is generated:
* "event" = INSTR_FF_STATUS.USED < EVENT_LEVEL.
* By default EVENT_LEVEL = 0;
*
* This function available for CM0+ core only.
*
*******************************************************************************/
__STATIC_INLINE void Cy_Crypto_Core_WaitForFifoAvailable(void)
{
    while((_FLD2VAL(CRYPTO_INSTR_FF_STATUS_EVENT, CRYPTO->INSTR_FF_STATUS)) == 0)
    {
    }
}

/*******************************************************************************
* Function Name: Crypto_Server_WaitForFifoAvailable ???
*****************************************************************************//**
*
* Waits until all instruction in FIFO will be completed
*
* This function available for CM0+ core only.
*
*******************************************************************************/
__STATIC_INLINE void Cy_Crypto_Core_WaitForReady(void)
{
    while((_FLD2VAL(CRYPTO_INSTR_FF_STATUS_BUSY, CRYPTO->INSTR_FF_STATUS)) == 1)
    {
    }
}


#endif /* #if (CPUSS_CRYPTO_PRESENT == 1) */

#endif /* #if (CY_CPU_CORTEX_M0P) */

#endif /* #if !defined(CY_CRYPTO_CORE_UTIL_H) */


/* [] END OF FILE */

