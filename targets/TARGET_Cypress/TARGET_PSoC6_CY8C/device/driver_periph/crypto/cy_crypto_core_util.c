/***************************************************************************//**
* \file cy_crypto_core_util.c
* \version 1.0
*
* \brief
*  This file provides the source code to the API for the utils
*  in the Crypto driver.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "crypto/cy_crypto_common.h"
#include "crypto/cy_crypto_core_util.h"
#include "crypto/cy_crypto_core_instructions.h"
#include "crypto/cy_crypto_core_vu_hw.h"
#include "syslib/cy_syslib.h"
#include <stdbool.h>


#if (CY_CPU_CORTEX_M0P)

#if (CPUSS_CRYPTO_PRESENT == 1)

/* The flag to state that an HW error occurred */
bool cy_crypto_IsHwErrorOccured = false;

void Cy_Crypto_Core_ErrorHandler(void);


/*******************************************************************************
* Function Name: Cy_Crypto_Core_Enable
****************************************************************************//**
*
* The function to enable the Crypto hardware.
*
* This function available for CM0+ core only.
*
* \return
* Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Core_Enable(void)
{
    /* Enable Crypto HW */
    CRYPTO->CTL = (uint32_t)(_VAL2FLD(CRYPTO_CTL_ISOLATE, 1uL) |
                             _VAL2FLD(CRYPTO_CTL_RETAIN,  0uL) |
                             _VAL2FLD(CRYPTO_CTL_POWER,   1uL) |
                             _VAL2FLD(CRYPTO_CTL_ENABLED, 1uL));

    CRYPTO->CTL = (uint32_t)(_VAL2FLD(CRYPTO_CTL_ISOLATE, 0uL) |
                             _VAL2FLD(CRYPTO_CTL_RETAIN,  0uL) |
                             _VAL2FLD(CRYPTO_CTL_POWER,   1uL) |
                             _VAL2FLD(CRYPTO_CTL_ENABLED, 1uL));

#if (CPUSS_CRYPTO_VU == 1)

    CRYPTO->VU_CTL1 = (uint32_t)CRYPTO->MEM_BUFF;

    /* Set the stack pointer to the Crypto buff size, in words */
    CY_CRYPTO_VU_SET_REG(15, CPUSS_CRYPTO_BUFF_SIZE, 1);

    /* Clear whole register file */
    CY_CRYPTO_VU_SET_REG(14, 0, 1);
    CY_CRYPTO_VU_SET_REG(13, 0, 1);
    CY_CRYPTO_VU_SET_REG(12, 0, 1);
    CY_CRYPTO_VU_SET_REG(11, 0, 1);
    CY_CRYPTO_VU_SET_REG(10, 0, 1);
    CY_CRYPTO_VU_SET_REG(9, 0, 1);
    CY_CRYPTO_VU_SET_REG(8, 0, 1);
    CY_CRYPTO_VU_SET_REG(7, 0, 1);
    CY_CRYPTO_VU_SET_REG(6, 0, 1);
    CY_CRYPTO_VU_SET_REG(5, 0, 1);
    CY_CRYPTO_VU_SET_REG(4, 0, 1);
    CY_CRYPTO_VU_SET_REG(3, 0, 1);
    CY_CRYPTO_VU_SET_REG(2, 0, 1);
    CY_CRYPTO_VU_SET_REG(1, 0, 1);
    CY_CRYPTO_VU_SET_REG(0, 0, 1);

#endif /* #if (CPUSS_CRYPTO_VU == 1) */

    return (CY_CRYPTO_SUCCESS);
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_Disable
****************************************************************************//**
*
* Disables the operation of the CRYPTO block.
*
* This function available for CM0+ core only.
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Core_Disable(void)
{
    CRYPTO->CTL = (uint32_t)(_VAL2FLD(CRYPTO_CTL_ISOLATE, 0uL) |
                             _VAL2FLD(CRYPTO_CTL_RETAIN,  0uL) |
                             _VAL2FLD(CRYPTO_CTL_POWER,   0uL) |
                             _VAL2FLD(CRYPTO_CTL_ENABLED, 0uL));

    return (CY_CRYPTO_SUCCESS);
}

/*******************************************************************************
* Function Name: Cy_Crypto_Core_CheckHwForErrors
****************************************************************************//**
*
* The function checks if a Crypto HW error occurred. If yes, copy the error
* information from the Crypto registers to the communication structure.
*
* This function available for CM0+ core only.
*
* \param cryptoContext
* The pointer to vcy_stc_crypto_context_t structure which stores
* the Crypto driver context.
*
* \return
* A Crypto status \ref cy_en_crypto_status_t.
*
*******************************************************************************/
cy_en_crypto_status_t Cy_Crypto_Core_CheckHwForErrors(cy_stc_crypto_context_t *cryptoContext)
{
    cy_en_crypto_status_t result = CY_CRYPTO_SUCCESS;

    if (cy_crypto_IsHwErrorOccured)
    {
        result = CY_CRYPTO_HW_ERROR;

    }

    cryptoContext->hwErrorStatus.errorStatus0 = _FLD2VAL(CRYPTO_ERROR_STATUS0_DATA32, CRYPTO->ERROR_STATUS0);

    cryptoContext->hwErrorStatus.errorStatus1 = (_FLD2VAL(CRYPTO_ERROR_STATUS1_DATA23, CRYPTO->ERROR_STATUS1) |
                                                 _FLD2VAL(CRYPTO_ERROR_STATUS1_IDX, CRYPTO->ERROR_STATUS1) |
                                                 _FLD2VAL(CRYPTO_ERROR_STATUS1_VALID, CRYPTO->ERROR_STATUS1));
    return (result);
}

/*******************************************************************************
* Function Name: Cy_Crypto_SetReg1Instr
****************************************************************************//**
*
* Writes one 32-Bit data word into Crypto FIFO.
*
* This function available for CM0+ core only.
*
* \param data0
* The address of data to be placed into Crypto FIFO
* on the address CRYPTO_REGFILE_R0.
*
*******************************************************************************/
void Cy_Crypto_SetReg1Instr(uint32_t data0)
{
    /* Check whether FIFO has enough space for 1 instruction */
    while(Cy_Crypto_Core_GetFIFOUsed() >= (CY_CRYPTO_FIFODEPTH - 1u))
    {
    }

    CRYPTO->INSTR_FF_WR = (uint32_t)(((uint32_t)CY_CRYPTO_SET_REG1_OPC << CY_CRYPTO_OPCODE_POS) |
                                      (uint32_t)CY_CRYPTO_REGFILE_R0);

    CRYPTO->INSTR_FF_WR = data0;
}

/*******************************************************************************
* Function Name: Cy_Crypto_SetReg2Instr
****************************************************************************//**
*
* Writes two 32-Bit data words into Crypto FIFO.
*
* This function available for CM0+ core only.
*
* \param data0
* The address of data to be placed into Crypto FIFO
* on the address CRYPTO_REGFILE_R0.
*
* \param data1
* The address of data to be placed into Crypto FIFO
* on the address CRYPTO_REGFILE_R1.
*
*******************************************************************************/
void Cy_Crypto_SetReg2Instr(uint32_t data0, uint32_t data1)
{
    /* Check whether FIFO has enough space for 2 instructions */
    while(Cy_Crypto_Core_GetFIFOUsed() >= (CY_CRYPTO_FIFODEPTH - 2u))
    {
    }

    CRYPTO->INSTR_FF_WR = (uint32_t)(((uint32_t)CY_CRYPTO_SET_REG2_OPC << CY_CRYPTO_OPCODE_POS) |
                                     ((uint32_t)CY_CRYPTO_REGFILE_R1 << CY_CRYPTO_RSRC4_SHIFT) |
                                      (uint32_t)CY_CRYPTO_REGFILE_R0);

    CRYPTO->INSTR_FF_WR = data0;
    CRYPTO->INSTR_FF_WR = data1;
}

/*******************************************************************************
* Function Name: Cy_Crypto_SetReg3Instr
****************************************************************************//**
*
* Writes three 32-Bit data words into Crypto FIFO.
*
* This function available for CM0+ core only.
*
* \param data0
* The address of data to be placed into Crypto FIFO
* on the address CRYPTO_REGFILE_R0.
*
* \param data1
* The address of data to be placed into Crypto FIFO
* on the address CRYPTO_REGFILE_R1.
*
* \param data2
* The address of data to be be placed into Crypto FIFO
* on the address CRYPTO_REGFILE_R2.
*
*******************************************************************************/
void Cy_Crypto_SetReg3Instr(uint32_t data0, uint32_t data1, uint32_t data2)
{
    /* Check whether FIFO has enough space for 3 instructions */
    while(Cy_Crypto_Core_GetFIFOUsed() >= (CY_CRYPTO_FIFODEPTH - 3u))
    {
    }

    CRYPTO->INSTR_FF_WR = (uint32_t)( ((uint32_t)CY_CRYPTO_SET_REG3_OPC << CY_CRYPTO_OPCODE_POS) |
                                      ((uint32_t)CY_CRYPTO_REGFILE_R2 << CY_CRYPTO_RSRC8_SHIFT) |
                                      ((uint32_t)CY_CRYPTO_REGFILE_R1 << CY_CRYPTO_RSRC4_SHIFT) |
                                       (uint32_t)CY_CRYPTO_REGFILE_R0 );

    CRYPTO->INSTR_FF_WR = data0;
    CRYPTO->INSTR_FF_WR = data1;
    CRYPTO->INSTR_FF_WR = data2;
}

/*******************************************************************************
* Function Name: Cy_Crypto_SetReg4Instr
****************************************************************************//**
*
* Writes four 32-Bit data words into Crypto FIFO.
*
* This function available for CM0+ core only.
*
* \param data0
* The address of data to be placed into Crypto FIFO
* on the address CRYPTO_REGFILE_R0.
*
* \param data1
* The address of data to be placed into Crypto FIFO
* on the address CRYPTO_REGFILE_R1.
*
* \param data2
* The address of data to be placed into Crypto FIFO
* on the address CRYPTO_REGFILE_R2.
*
* \param data3
* The address of data to be placed into Crypto FIFO
* on the address CRYPTO_REGFILE_R3.
*
*******************************************************************************/
void Cy_Crypto_SetReg4Instr(uint32_t data0, uint32_t data1, uint32_t data2, uint32_t data3)
{
    /* Check whether FIFO has enough space for 4 instructions */
    while(Cy_Crypto_Core_GetFIFOUsed() >= (CY_CRYPTO_FIFODEPTH - 4u))
    {
    }

    CRYPTO->INSTR_FF_WR = (uint32_t)(((uint32_t)CY_CRYPTO_SET_REG4_OPC << CY_CRYPTO_OPCODE_POS) |
                                     ((uint32_t)CY_CRYPTO_REGFILE_R3 << CY_CRYPTO_RSRC12_SHIFT) |
                                     ((uint32_t)CY_CRYPTO_REGFILE_R2 << CY_CRYPTO_RSRC8_SHIFT) |
                                     ((uint32_t)CY_CRYPTO_REGFILE_R1 << CY_CRYPTO_RSRC4_SHIFT) |
                                      (uint32_t)CY_CRYPTO_REGFILE_R0);

    CRYPTO->INSTR_FF_WR = data0;
    CRYPTO->INSTR_FF_WR = data1;
    CRYPTO->INSTR_FF_WR = data2;
    CRYPTO->INSTR_FF_WR = data3;
}

/*******************************************************************************
* Function Name: Cy_Crypto_Run0ParamInstr
*****************************************************************************//**
*
* Run the Crypto instruction without parameters.
*
* This function available for CM0+ core only.
*
* \param instr
* The Opcode of the called instruction.
*
*******************************************************************************/
void Cy_Crypto_Run0ParamInstr(uint8_t instr)
{
    /* Check whether FIFO has enough space for 1 instruction */
    while(Cy_Crypto_Core_GetFIFOUsed() >= CY_CRYPTO_FIFODEPTH)
    {
    }

    CRYPTO->INSTR_FF_WR = (uint32_t)((uint32_t)instr << CY_CRYPTO_OPCODE_POS);
}

/*******************************************************************************
* Function Name: Cy_Crypto_Run1ParamInstr
*****************************************************************************//**
*
* Run the Crypto instruction with one parameter.
* The parameter must be placed into register 0
*
* This function available for CM0+ core only.
*
* \param instr
* The Opcode of the called instruction.
*
* \param rdst0Shift
* The shift for the instruction operand.
*
*******************************************************************************/
void Cy_Crypto_Run1ParamInstr(uint8_t instr,
                           uint32_t rdst0Shift)
{
    /* Check whether FIFO has enough space for 1 instruction */
    while(Cy_Crypto_Core_GetFIFOUsed() >= CY_CRYPTO_FIFODEPTH)
    {
    }

    CRYPTO->INSTR_FF_WR = (uint32_t)(((uint32_t)instr << CY_CRYPTO_OPCODE_POS) |
                                     ((uint32_t)CY_CRYPTO_REGFILE_R0 << rdst0Shift));
}

/*******************************************************************************
* Function Name: Cy_Crypto_Run2ParamInstr
*****************************************************************************//**
*
* Run the Crypto instruction with two parameters.
* The zero parameter must be placed into register 0,
* the first parameter must be placed into register 1.
*
* This function available for CM0+ core only.
*
* \param instr
* The Opcode of the called instruction.
*
* \param rdst0Shift
* The shift for the zero instruction operand.
*
* \param rdst1Shift
* The shift for the second instruction operand.
*
*******************************************************************************/
void Cy_Crypto_Run2ParamInstr(uint8_t instr,
                           uint32_t rdst0Shift,
                           uint32_t rdst1Shift)
{
    /* Check whether FIFO has enough space for 1 instruction */
    while(Cy_Crypto_Core_GetFIFOUsed() >= CY_CRYPTO_FIFODEPTH)
    {
    }

    CRYPTO->INSTR_FF_WR = (uint32_t)(((uint32_t)instr << CY_CRYPTO_OPCODE_POS) |
                                     ((uint32_t)CY_CRYPTO_REGFILE_R1 << rdst1Shift) |
                                     ((uint32_t)CY_CRYPTO_REGFILE_R0 << rdst0Shift));
}

/*******************************************************************************
* Function Name: Cy_Crypto_Run3ParamInstr
*****************************************************************************//**
*
* Run the Crypto instruction with three parameters.
* The zero parameter must be placed into register 0,
* the first parameter must be placed into register 1,
* the second parameter must be placed into register 2.
*
* This function available for CM0+ core only.
*
* \param instr
* The Opcode of the called instruction.
*
* \param rdst0Shift
* The shift for the zero instruction operand.
*
* \param rdst1Shift
* The shift for the second instruction operand.
*
** \param rdst2Shift
* The shift for the second instruction operand.
*
*******************************************************************************/
void Cy_Crypto_Run3ParamInstr(uint8_t instr,
                           uint8_t rdst0Shift,
                           uint8_t rdst1Shift,
                           uint8_t rdst2Shift)
{
    /* Check whether FIFO has enough space for 1 instruction */
    while(Cy_Crypto_Core_GetFIFOUsed() >= CY_CRYPTO_FIFODEPTH)
    {
    }

    CRYPTO->INSTR_FF_WR = (uint32_t)(((uint32_t)instr << CY_CRYPTO_OPCODE_POS) |
                                     ((uint32_t)CY_CRYPTO_REGFILE_R2 << rdst2Shift) |
                                     ((uint32_t)CY_CRYPTO_REGFILE_R1 << rdst1Shift) |
                                     ((uint32_t)CY_CRYPTO_REGFILE_R0 << rdst0Shift));
}

/*******************************************************************************
* Function Name: Cy_Crypto_Run4ParamInstr
*****************************************************************************//**
*
* Run the Crypto instruction with four parameters.
* The zero parameter must be placed into register 0,
* the first parameter must be placed into register 1,
* the second parameter must be placed into register 2,
* the third parameter must be placed into register 3.
*
* This function available for CM0+ core only.
*
* \param instr
* The Opcode of the called instruction.
*
* \param rdst0Shift
* The shift for the zero instruction operand.
*
* \param rdst1Shift
* The shift for the first instruction operand.
*
* \param rdst2Shift
* The shift for the second instruction operand.
*
* \param rdst3Shift
* The shift for the third instruction operand.
*
*******************************************************************************/
void Cy_Crypto_Run4ParamInstr(uint8_t instr,
                           uint32_t rdst0Shift,
                           uint32_t rdst1Shift,
                           uint32_t rdst2Shift,
                           uint32_t rdst3Shift)
{
    /* Check whether FIFO has enough space for 1 instruction */
    while(Cy_Crypto_Core_GetFIFOUsed() >= CY_CRYPTO_FIFODEPTH)
    {
    }

    CRYPTO->INSTR_FF_WR = (uint32_t)(((uint32_t)instr << CY_CRYPTO_OPCODE_POS) |
                                     ((uint32_t)CY_CRYPTO_REGFILE_R3 << rdst3Shift) |
                                     ((uint32_t)CY_CRYPTO_REGFILE_R2 << rdst2Shift) |
                                     ((uint32_t)CY_CRYPTO_REGFILE_R1 << rdst1Shift) |
                                     ((uint32_t)CY_CRYPTO_REGFILE_R0 << rdst0Shift));
}

/*******************************************************************************
* Function Name: Cy_Crypto_InvertEndian8
*****************************************************************************//**
*
* Inverts endianness for eight bytes.
*
* This function available for CM0+ core only.
*
* \param srcVal
* The input data whose endianness will be inverted.
*
* \return
* An input value with inverted endianness.
*
*******************************************************************************/
uint64_t Cy_Crypto_InvertEndian8(uint64_t srcVal)
{
    uint64_t tmpVal;

    tmpVal = ((srcVal << 8u) & 0xFF00FF00FF00FF00ULL) | ((srcVal >> 8u) & 0x00FF00FF00FF00FFULL);
    tmpVal = ((tmpVal << 16u) & 0xFFFF0000FFFF0000ULL) | ((tmpVal >> 16u) & 0x0000FFFF0000FFFFULL);

    return ((tmpVal << 32u) | (tmpVal >> 32u));
}

#endif /* #if (CPUSS_CRYPTO_PRESENT == 1) */
#endif /* #if (CY_CPU_CORTEX_M0P) */

/* [] END OF FILE */

