/***************************************************************************//**
* \file cy_mxotpc.c
* \version 1.0
*
* Provides an API implementation of the OTP driver.
*
********************************************************************************
* \copyright
* Copyright 2016-2019 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
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
*******************************************************************************/

#include "cy_device.h"

#if defined (CY_IP_MXS28OTPC)

#include "cy_mxotpc.h"
#include <stdlib.h>

/* Static Functions */
static cy_mxotpc_status_t Cy_MXOTPC_GetWriteProtectedData(uint32_t otpRowNum, uint32_t otpWriteData, uint32_t *otpWriteProtData);

/* Static Data types */
/** OTP Program Enable Sequence Data */
static uint8_t otpProgramEnable[] = { 0xf, 0x4, 0x8, 0xd};

/** \cond PARAM_CHECK_MACROS */
/** Parameter check macros */ 

#define CY_MXOTPC_IS_ROW_VALID(row)              (row < CY_MXOTPC_MAX_OTP_ROW)
#define CY_MXOTPC_IS_BITNUM_VALID(bit)           (bit < 32)
#define CY_MXOTPC_IS_DEST_ADDR_ALIGNED(addr)     (!((uint32_t)addr % 4u))
#define CY_MXOTPC_IS_ARRAY_SIZE_VALID(row, size) ((row+size) < CY_MXOTPC_MAX_OTP_ROW)
#define CY_MXOTPC_IS_SIZE_VALID(size)            (size != 0)
/** \endcond */

/*******************************************************************************
* Function Name: Cy_MXOTPC_Init
****************************************************************************//**
*
* Initializes the MXOTP Driver:
* - Enables the MXOTPC IP.
*
*  \param mxotpcConfig
*  This structure contains the data that needs to be filled in OTP_CTL register.
*
* Note - Allow NULL also, meaning OTP IP will go with default Configuration 
*
*  \return the MXOTPC API status \ref cy_mxotpc_status_t.
*
*******************************************************************************/
cy_mxotpc_status_t Cy_MXOTPC_Init(cy_mxotpc_ctl_config_t const *mxotpcConfig)
{
    Cy_MXOTPC_IpEnable();

    if(mxotpcConfig != NULL)
        Cy_MXOTPC_OtpCtlConfig(mxotpcConfig);

    return CY_MXOTPC_SUCCESS;
}

/*******************************************************************************
* Function Name: Cy_MXOTPC_DeInit
****************************************************************************//**
*
* DeInitializes the MXOTP Driver:
* - Disables the MXOTPC IP.
*
* Note - Allow NULL also, meaning OTP IP will go with default Configuration 
*
*  \return the MXOTPC API status \ref cy_mxotpc_status_t.
*
*******************************************************************************/
cy_mxotpc_status_t Cy_MXOTPC_DeInit(void)
{
    Cy_MXOTPC_IpDisable();

    return CY_MXOTPC_SUCCESS;
}


/*******************************************************************************
* Function Name: Cy_MXOTPC_WriteProgramSequence
****************************************************************************//**
*
* Program Sequence for Writes
*
* 1.Write Following Bit
* OTP_CTL.OTPProg_En =1 // Enable OTP programming at top level control
* 2.Check whether OTP is busy or not
* OTP_CMD.START == 0 // OTP is not busy
* 3.Check whether programming is enabled or not (PROGRAM Enable Seq)
* OTP_STATUS.ProgOK == 1 // OTP programming enabled
* If this bit is not set, perform following steps, otherwise skip.
* a.OTP commands to enable OTP Programming 
* i.Write OTP_PROGDATA = 0xF
* ii.Write OTP_CMD reg (with CMD =0x02), VALUE:  0x9100_0000
* iii.Wait for OTP_CMD.START == 0
* iv.Repeat from point i to iii for OTP_PROGDATA 0x4, 0x8 and 0xd in sequence
* Check whether programming is enabled or not
* OTP_STATUS.ProgOK == 1 // OTP programming enabled
*
*  \return the MXOTPC API status \ref cy_mxotpc_status_t.
*
*******************************************************************************/
cy_mxotpc_status_t Cy_MXOTPC_WriteProgramSequence(void)
{
    uint32_t idx;
    cy_mxotpc_cmd_t otpCmd = {0, 0, 0};

    /* Check if programming is already enabled or not */
    if(!Cy_MXOTPC_OtpGetStatus(MXOTPC_OTP_STATUS_PROGOK_Msk))  
    {
        /* PROGRAM Sequence */
        /* 1.Write Following Bit
        * OTP_CTL.OTPProg_En =1 // Enable OTP programming at top level control */
        Cy_MXOTPC_OtpProgEnable(true);

        /* 2.Check whether OTP is busy or not
        * OTP_CMD.START == 0 // OTP is not busy */
        if(Cy_MXOTPC_OtpCheckStartStatus(CY_START_STATUS_CHECK_TIMEOUT) == CY_MXOTPC_TIMEOUT)
            return CY_MXOTPC_TIMEOUT;

        /* 3.Check whether programming is enabled or not (PROGRAM Enable Seq)
        * OTP_STATUS.ProgOK == 1 // OTP programming enabled
        * If this bit is not set, perform following steps, otherwise skip.
        * a.OTP commands to enable OTP Programming.
        * i.Write OTP_PROGDATA = 0xF
        * ii.Write OTP_CMD reg (with CMD =0x02), VALUE:    0x9100_0000
        * iii.Wait for OTP_CMD.START == 0
        * iv.Repeat from point i to iii for OTP_PROGDATA 0x4, 0x8 and 0xd in sequence
        * Check whether programming is enabled or not
        * OTP_STATUS.ProgOK == 1 // OTP programming enabled */
        for(idx = 0; idx < sizeof(otpProgramEnable); idx++)
        {
            Cy_MXOTPC_OtpSetProgData(otpProgramEnable[idx]);

            otpCmd.cmd = CY_MXOTPC_OTP_PROGENABLE_CMD;
            Cy_MXOTPC_OtpCmd(&otpCmd);

            if(Cy_MXOTPC_OtpCheckStartStatus(CY_START_STATUS_CHECK_TIMEOUT) == CY_MXOTPC_TIMEOUT)
               return CY_MXOTPC_TIMEOUT;
        }
        if(!Cy_MXOTPC_OtpGetStatus(MXOTPC_OTP_STATUS_PROGOK_Msk))
            return CY_MXOTPC_WRITE_ERROR;
    }

    return CY_MXOTPC_SUCCESS;
}

/*******************************************************************************
* Function Name: Cy_MXOTPC_GetWriteProtectedData
****************************************************************************//**
*
* Write Protection for already set OTP bits
*
*  \param otpRowNum
*  Contains the Row Number to be written
*  \param otpWriteData
*  Contains the Data to be written
*  \param otpFinalWriteData
*  Contains the corrected Data to be Written
*
*  \return the MXOTPC API status \ref cy_mxotpc_status_t.
*
*******************************************************************************/
static cy_mxotpc_status_t Cy_MXOTPC_GetWriteProtectedData(uint32_t otpRowNum, uint32_t otpWriteData, uint32_t *otpWriteProtData)
{
    uint32_t otpReadData;
    cy_mxotpc_status_t status;

    status = Cy_MXOTPC_ReadRow( otpRowNum, &otpReadData );
    if( !status )
    {
        *otpWriteProtData = (otpWriteData ^ otpReadData) & otpWriteData;
        return CY_MXOTPC_SUCCESS;
    }
    else
        return status;
}

/*******************************************************************************
* Function Name: Cy_MXOTPC_WriteRowReadBack
****************************************************************************//**
*
* Writes a particular row with 32-bit data with/without ECC and reads the row back.
*
* Indirect PROGRAM Sequence
* 1.Write Following Bit
* OTP_CTL.OTPProg_En =1 // Enable OTP programming at top level control
* 2.Check whether OTP is busy or not
* OTP_CMD.START == 0 // OTP is not busy
* 3.Check whether programming is enabled or not (PROGRAM Enable Seq)
* OTP_STATUS.ProgOK == 1 // OTP programming enabled
* If this bit is not set, perform following steps, otherwise skip.
* a.OTP commands to enable OTP Programming
* i.Write OTP_PROGDATA = 0xF
* ii.Write OTP_CMD reg (with CMD =0x02), VALUE:  0x9100_0000
* iii.Wait for OTP_CMD.START == 0
* iv.Repeat from point i to iii for OTP_PROGDATA 0x4, 0x8 and 0xd in sequence
* Check whether programming is enabled or not
* OTP_STATUS.ProgOK == 1 // OTP programming enabled
* 4.Write OTP_PROGDATA register
* Write 32-bit data value to be programmed inside OTP_PROGDATA reg
* 5.WRITE OTP_CMD Register
* Write following value in OTP_CMD reg.
* For OTP_CMD command (0x0A) at 511th row, you may use following value,
* 0x8501FF00
* 6.check completion of access
* Read 31st bit of OTP_CMD register to check completion of programming.
* OTP_CMD.START == 0 // access completed
* 7. Check programming status
* OTP_STATUS.PROGFAIL == 1 // Program failure
*
*  \param otpRowNum
*  Contains the Row Number to be written
*  \param otpWriteData
*  Contains the Data to be written
*  \param otpReadBackData
*  Contains the Read back Data of the Row Number passed.
*  \param enable_ecc
*  Contains boolean, which tells to enable ECC for that row or not.
*
*  \return the MXOTPC API status \ref cy_mxotpc_status_t.
*
*******************************************************************************/
cy_mxotpc_status_t Cy_MXOTPC_WriteRowReadBack(uint32_t otpRowNum, uint32_t otpWriteData, uint32_t *otpReadBackData, bool enable_ecc)
{
    cy_mxotpc_cmd_t otpCmd = {0, 0, 0};
    cy_mxotpc_status_t result = CY_MXOTPC_INVALID;
    uint32_t otpWriteProtData, otpReadData;

    CY_ASSERT_L2(CY_MXOTPC_IS_ROW_VALID(otpRowNum));

    /* Indirect PROGRAM Sequence */
    result = Cy_MXOTPC_WriteProgramSequence();
    if(result != CY_MXOTPC_SUCCESS)
        return result;

    /* 4.Write OTP_PROGDATA register
    * Write 32-bit data value to be programmed inside OTP_PROGDATA reg */
    /* Write Protection : OTP bit that is already set shouldnt be set again */
    result = Cy_MXOTPC_GetWriteProtectedData(otpRowNum, otpWriteData, &otpWriteProtData);
    if(result != CY_MXOTPC_SUCCESS)
        return result;

    /* otpWriteProtData = 0 means required OTP's set already, Nothing to Write */
    if(otpWriteProtData)
    {
        Cy_MXOTPC_OtpSetProgData(otpWriteProtData);

        /* 5.WRITE OTP_CMD Register
        * Write following value in OTP_CMD reg.
        * For OTP_CMD command (0x0A) at 511th row, you may use following value,
        * 0x8501FF00 */
        if(enable_ecc)
            otpCmd.cmd = CY_MXOTPC_PROG_ECC_CMD;
        else
            otpCmd.cmd = CY_MXOTPC_PROG_CMD;

        otpCmd.rowAddr = otpRowNum;
        Cy_MXOTPC_OtpCmd(&otpCmd);

        /* 6.check completion of access
        * Read 31st bit of OTP_CMD register to check completion of programming.
        * OTP_CMD.START == 0 // access completed */
        if(Cy_MXOTPC_OtpCheckStartStatus(CY_START_STATUS_CHECK_TIMEOUT) == CY_MXOTPC_TIMEOUT)
            return CY_MXOTPC_TIMEOUT;

        /* 7. Check programming status
        * OTP_STATUS.PROGFAIL == 1 // Program failure */
        if(Cy_MXOTPC_OtpGetStatus(MXOTPC_OTP_STATUS_PROGFAIL_Msk))  
            return CY_MXOTPC_WRITE_PROG_ERROR;

        /* Read the row back and verify if the data is written successfully */
        result = Cy_MXOTPC_ReadRow( otpRowNum, &otpReadData );
        if(result != CY_MXOTPC_SUCCESS)
            return result;
        
        if((otpReadData & otpWriteData) != otpWriteData)
            return CY_MXOTPC_WRITE_ERROR;
        
        /* Read back the Row Data and pass it back to caller for cross check */
        if( otpReadBackData )
            *otpReadBackData = otpReadData;
    }
    return CY_MXOTPC_SUCCESS;
}

/*******************************************************************************
* Function Name: Cy_MXOTPC_WriteRow
****************************************************************************//**
*
* Writes a particular row with 32-bit data with/without ECC.
*
*  \param otpRowNum
*  Contains the Row Number to be written
*  \param otpWriteData
*  Contains the Data to be written
*  \param enable_ecc
*  Contains boolean, which tells to enable ECC for that row or not.
*
*  \return the MXOTPC API status \ref cy_mxotpc_status_t.
*
*******************************************************************************/
cy_mxotpc_status_t Cy_MXOTPC_WriteRow(uint32_t otpRowNum, uint32_t otpWriteData, bool enable_ecc)
{
    CY_ASSERT_L2(CY_MXOTPC_IS_ROW_VALID(otpRowNum));

    return Cy_MXOTPC_WriteRowReadBack(otpRowNum, otpWriteData, NULL, enable_ecc);
}

/*******************************************************************************
* Function Name: Cy_MXOTPC_WriteBitReadBack
****************************************************************************//**
*
* Writes a particular row with 32-bit data without ECC and Read the Row back
*
* Indirect PROGRAM Sequence
* 1.Write Following Bit
* OTP_CTL.OTPProg_En =1 // Enable OTP programming at top level control
* 2.Check whether OTP is busy or not
* OTP_CMD.START == 0 // OTP is not busy
* 3.Check whether programming is enabled or not (PROGRAM Enable Seq)
* OTP_STATUS.ProgOK == 1 // OTP programming enabled
* If this bit is not set, perform following steps, otherwise skip.
* a.OTP commands to enable OTP Programming 
* i.Write OTP_PROGDATA = 0xF
* ii.Write OTP_CMD reg (with CMD =0x02), VALUE:  0x9100_0000
* iii.Wait for OTP_CMD.START == 0
* iv.Repeat from point i to iii for OTP_PROGDATA 0x4, 0x8 and 0xd in sequence
* Check whether programming is enabled or not
* OTP_STATUS.ProgOK == 1 // OTP programming enabled
* 4.Write OTP_PROGDATA register after reading back and masking the
* required bit position, Write 32-bit data value to be programmed
* inside OTP_PROGDATA reg.
* 5.WRITE OTP_CMD Register
* Write following value in OTP_CMD reg.
* For OTP_CMD command (0x0A) at 511th row, you may use following value,
* 0x8501FF00
* 6.check completion of access
* Read 31st bit of OTP_CMD register to check completion of programming.
* OTP_CMD.START == 0 // access completed
* 7. Check programming status
* OTP_STATUS.PROGFAIL == 1 // Program failure
*
*  \param otpRowNum
*  Contains the Row Number to be written
*  \param bitNum
*  Contains the bit position to be written
*  \param otpReadBackData
*  Contains the Read back Data of the Row Number passed.
*
*  \return the MXOTPC API status \ref cy_mxotpc_status_t.
*
*******************************************************************************/
cy_mxotpc_status_t Cy_MXOTPC_WriteBitReadBack(uint32_t otpRowNum, uint32_t bitNum, uint32_t *otpReadBackData)
{
    cy_mxotpc_cmd_t otpCmd = {0, 0, 0};
    cy_mxotpc_status_t result = CY_MXOTPC_INVALID;
    uint32_t otpWriteProtData, otpReadData;

    CY_ASSERT_L2(CY_MXOTPC_IS_ROW_VALID(otpRowNum));
    CY_ASSERT_L2(CY_MXOTPC_IS_BITNUM_VALID(bitNum));

    /* Indirect PROGRAM Sequence */
    result = Cy_MXOTPC_WriteProgramSequence();
    if(result != CY_MXOTPC_SUCCESS)
        return result;

    /* 4.Write OTP_PROGDATA register after reading back and masking the
    * required bit position, Write 32-bit data value to be programmed
    * inside OTP_PROGDATA reg */
    /* Write Protection : OTP bit that is already set shouldnt be set again */
    result = Cy_MXOTPC_GetWriteProtectedData(otpRowNum, (1 << bitNum), &otpWriteProtData);
    if(result != CY_MXOTPC_SUCCESS)
        return result;

    /* otpWriteProtData = 0 means required OTP's set already, Nothing to Write */
    if(otpWriteProtData)
    {
        Cy_MXOTPC_OtpSetProgData(otpWriteProtData);

        /* 5.WRITE OTP_CMD Register
        * Write following value in OTP_CMD reg.
        * For OTP_CMD command (0x0A) at 511th row, you may use following value,
        * 0x8501FF00 */
        otpCmd.cmd = CY_MXOTPC_PROG_CMD;
        otpCmd.rowAddr = otpRowNum;
        Cy_MXOTPC_OtpCmd(&otpCmd);

        /* 6.check completion of access
        * Read 31st bit of OTP_CMD register to check completion of programming.
        * OTP_CMD.START == 0 // access completed */
        if(Cy_MXOTPC_OtpCheckStartStatus(CY_START_STATUS_CHECK_TIMEOUT) == CY_MXOTPC_TIMEOUT)
            return CY_MXOTPC_TIMEOUT;

        /* 7. Check programming status
        * OTP_STATUS.PROGFAIL == 1 // Program failure */
        if(Cy_MXOTPC_OtpGetStatus(MXOTPC_OTP_STATUS_PROGFAIL_Msk))  
            return CY_MXOTPC_WRITE_ERROR;

    }
    
    /* Read the row back and verify if the data is written successfully */
    result = Cy_MXOTPC_ReadRow( otpRowNum, &otpReadData );
    if(result != CY_MXOTPC_SUCCESS)
        return result;

    if(!(otpReadData & (1 << bitNum)))
        return CY_MXOTPC_WRITE_ERROR;

    /* Read back the Row Data and pass it back to caller for cross check */
    if(otpReadBackData)
        *otpReadBackData = otpReadData;
    return CY_MXOTPC_SUCCESS;
}

/*******************************************************************************
* Function Name: Cy_MXOTPC_WriteBit
****************************************************************************//**
*
* Writes a particular row with 32-bit data without ECC
*
*  \param otpRowNum
*  Contains the Row Number to be written
*  \param bitNum
*  Contains the bit position to be written
*
*  \return the MXOTPC API status \ref cy_mxotpc_status_t.
*
*******************************************************************************/
cy_mxotpc_status_t Cy_MXOTPC_WriteBit(uint32_t otpRowNum, uint32_t bitNum)
{
    CY_ASSERT_L2(CY_MXOTPC_IS_ROW_VALID(otpRowNum));
    CY_ASSERT_L2(CY_MXOTPC_IS_BITNUM_VALID(bitNum));

    return Cy_MXOTPC_WriteBitReadBack(otpRowNum, bitNum, NULL);
}

/*******************************************************************************
* Function Name: Cy_MXOTPC_WriteEcc
****************************************************************************//**
*
* Writes a particular row with ECC.
*
* Indirect PROGRAM Sequence
* 1.Write Following Bit
* OTP_CTL.OTPProg_En =1 // Enable OTP programming at top level control
* 2.Check whether OTP is busy or not
* OTP_CMD.START == 0 // OTP is not busy
* 3.Check whether programming is enabled or not (PROGRAM Enable Seq)
* OTP_STATUS.ProgOK == 1 // OTP programming enabled
* If this bit is not set, perform following steps, otherwise skip.
* a.OTP commands to enable OTP Programming 
* i.Write OTP_PROGDATA = 0xF
* ii.Write OTP_CMD reg (with CMD =0x02), VALUE:  0x9100_0000
* iii.Wait for OTP_CMD.START == 0
* iv.Repeat from point i to iii for OTP_PROGDATA 0x4, 0x8 and 0xd in sequence
* Check whether programming is enabled or not
* OTP_STATUS.ProgOK == 1 // OTP programming enabled
* 4.WRITE OTP_CMD Register
* Write following value in OTP_CMD reg.
* For OTP_CMD command (0x0A) at 511th row, you may use following value,
* 0x8401FF00
* 5.check completion of access
* Read 31st bit of OTP_CMD register to check completion of programming.
* OTP_CMD.START == 0 // access completed
* 6. Check programming status
* OTP_STATUS.PROGFAIL == 1 // Program failure
* 7. Read back the Row and check the ECC_STATUS register
* Check ECC_STATUS.ECC_ENABLE bits , if set then ECC_STATUS.ECC_DBL_ERR shouldnt
* be set.
*  \param otpRowNum
*  Contains the Row Number to be written with ECC
*
*  \return the MXOTPC API status \ref cy_mxotpc_status_t.
*
*******************************************************************************/
cy_mxotpc_status_t Cy_MXOTPC_WriteEcc(uint32_t otpRowNum)
{
    cy_mxotpc_cmd_t otpCmd = {0, 0, 0};
    uint32_t otpTempData;
    cy_mxotpc_status_t result = CY_MXOTPC_INVALID;

    CY_ASSERT_L2(CY_MXOTPC_IS_ROW_VALID(otpRowNum));

    /* Indirect ECC PROGRAM Sequence */
    result = Cy_MXOTPC_WriteProgramSequence();
    if(result != CY_MXOTPC_SUCCESS)
        return result;

    /* 4.WRITE OTP_CMD Register
    * Write following value in OTP_CMD reg.
    * For OTP_CMD command (0x0A) at 511th row, you may use following value,
    * 0x8481FF00 */
    otpCmd.cmd = CY_MXOTPC_PROG_ECC_WREAD_CMD;
    otpCmd.rowAddr = otpRowNum;
    Cy_MXOTPC_OtpCmd(&otpCmd);

    /* 5.check completion of access
    * Read 31st bit of OTP_CMD register to check completion of programming.
    * OTP_CMD.START == 0 // access completed */
    if(Cy_MXOTPC_OtpCheckStartStatus(CY_START_STATUS_CHECK_TIMEOUT) == CY_MXOTPC_TIMEOUT)
        return CY_MXOTPC_TIMEOUT;

    /* 6. Check programming status
    * OTP_STATUS.PROGFAIL == 1 // Program failure */
    if(Cy_MXOTPC_OtpGetStatus(MXOTPC_OTP_STATUS_PROGFAIL_Msk))  
        return CY_MXOTPC_WRITE_PROG_ERROR;

    /* Read the Row to check the ECC_STATUS */
    Cy_MXOTPC_ReadRow(otpRowNum, &otpTempData);

    if(!Cy_MXOTPC_OtpGetEccStatus(MXOTPC_ECC_STATUS_ECC_ENABLE_Msk))
        return CY_MXOTPC_WRITE_ECC_ERROR; 

    if(Cy_MXOTPC_OtpGetEccStatus(MXOTPC_ECC_STATUS_ECC_DBL_ERR_Msk))
        return CY_MXOTPC_WRITE_ECC_ERROR; 

    return CY_MXOTPC_SUCCESS;
}

/*******************************************************************************
* Function Name: Cy_MXOTPC_ReadRow
****************************************************************************//**
*
*  Reads the 32-bit value of a particular Row
*
*  \param otpRowNum
*  Contains the Row Number to be read
*  \param otpReadData
*  Contains the Read Data of the Row Number passed.
*
*  \return the MXOTPC API status \ref cy_mxotpc_status_t.
*
*******************************************************************************/
cy_mxotpc_status_t Cy_MXOTPC_ReadRow(uint32_t otpRowNum, uint32_t *otpReadData)
{
    CY_ASSERT_L2(CY_MXOTPC_IS_ROW_VALID(otpRowNum));

    *otpReadData = CY_GET_REG32((CY_MXOTPC_BASE + MXOTPC_SECTION_SIZE) + (otpRowNum * 4));

    if(Cy_MXOTPC_OtpGetEccStatus(MXOTPC_ECC_STATUS_ECC_DBL_ERR_Msk))
        return CY_MXOTPC_READ_ECC_ERROR;

    return CY_MXOTPC_SUCCESS;
}

/*******************************************************************************
* Function Name: Cy_MXOTPC_ReadBootRow
****************************************************************************//**
*
*  Reads the 32-bit value of Boot Row
*
*  \param otpReadData
*  Contains the Read Data of the Boot Row.
*
*  \return the MXOTPC API status \ref cy_mxotpc_status_t.
*
*******************************************************************************/
cy_mxotpc_status_t Cy_MXOTPC_ReadBootRow( uint32_t *otpReadData)
{
    *otpReadData = MXOTPC_BOOTROW;

    if(Cy_MXOTPC_OtpGetFoutEccStatus(MXOTPC_BOOT_ROW_FOUT_ECC_DED_STATUS_Msk))
        return CY_MXOTPC_READ_ECC_ERROR;

    return CY_MXOTPC_SUCCESS;
}

/*******************************************************************************
* Function Name: Cy_MXOTPC_ReadBit
****************************************************************************//**
*
*  Reads a bit status of a particular OTP Row
*
*  \param otpRowNum
*  Contains the Row Number, whose bit status should be Read
*  \param bitNum
*  Contains the bit position to be Read
*  \param otpReadBit
*  Contains the Read bit of bitNum position from the Row Number passed.
*
*  \return the MXOTPC API status \ref cy_mxotpc_status_t.
*
*******************************************************************************/
cy_mxotpc_status_t Cy_MXOTPC_ReadBit(uint32_t otpRowNum, uint32_t bitNum, uint32_t *otpReadBit)
{

    CY_ASSERT_L2(CY_MXOTPC_IS_ROW_VALID(otpRowNum));
    CY_ASSERT_L2(CY_MXOTPC_IS_BITNUM_VALID(bitNum));

    *otpReadBit = CY_GET_REG32((CY_MXOTPC_BASE + MXOTPC_SECTION_SIZE) + (otpRowNum * 4));
    /* Get the value of position:bitNum */
    *otpReadBit = (*otpReadBit & (1 << bitNum)? 1 : 0) ;

    if(Cy_MXOTPC_OtpGetEccStatus(MXOTPC_ECC_STATUS_ECC_DBL_ERR_Msk))
        return CY_MXOTPC_READ_ECC_ERROR;

    return CY_MXOTPC_SUCCESS;
}

/*******************************************************************************
* Function Name: Cy_MXOTPC_DirectWriteRow
****************************************************************************//**
*
* Writes a particular row with 32-bit data without ECC.
*
* Direct PROGRAM Sequence
* 1.Write Following Bit
* OTP_CTL.OTPProg_En =1 // Enable OTP programming at top level control
* 2.Check whether OTP is busy or not
* OTP_CMD.START == 0 // OTP is not busy
* 3.Check whether programming is enabled or not (PROGRAM Enable Seq)
* OTP_STATUS.ProgOK == 1 // OTP programming enabled
* If this bit is not set, perform following steps, otherwise skip.
* a.OTP commands to enable OTP Programming 
* i.Write OTP_PROGDATA = 0xF
* ii.Write OTP_CMD reg (with CMD =0x02), VALUE:  0x9100_0000
* iii.Wait for OTP_CMD.START == 0
* iv.Repeat from point i to iii for OTP_PROGDATA 0x4, 0x8 and 0xd in sequence
* Check whether programming is enabled or not
* OTP_STATUS.ProgOK == 1 // OTP programming enabled
* 4.Write CPU_PROG_CMD register
* Write command (0x0A) for Direct write
* Write command (0x08) for Direct write with ECC
* 5.Directly write to the memory mapped OTP address
*
*  \param otpRowNum
*  Contains the Row Number to be written
*  \param otpWriteData
*  Contains the Data to be written
*  \param otpReadBackData
*  Contains the Read back Data of the Row Number passed.
*  \param otpProgEccEnable
*  Contains the data whether we have to enable ECC or not.
*
*  \return the MXOTPC API status \ref cy_mxotpc_status_t.
*
*******************************************************************************/
cy_mxotpc_status_t Cy_MXOTPC_DirectWriteRow(uint32_t otpRowNum, uint32_t otpWriteData, uint32_t *otpReadBackData, bool otpProgEccEnable)
{
    cy_mxotpc_status_t result = CY_MXOTPC_INVALID;
    uint32_t otpWriteProtData;

    CY_ASSERT_L2(CY_MXOTPC_IS_ROW_VALID(otpRowNum));

    /* Indirect PROGRAM Sequence */
    result = Cy_MXOTPC_WriteProgramSequence();
    if(result != CY_MXOTPC_SUCCESS)
        return result;

    /* 4.Write CPU_PROG_CMD register
    * Write command (0x0A) for Direct write */
    if(otpProgEccEnable)
        Cy_MXOTPC_OtpCpuProgCmd(CY_MXOTPC_PROG_ECC_CMD);
    else
        Cy_MXOTPC_OtpCpuProgCmd(CY_MXOTPC_PROG_CMD);

    /* 5.Directly write to the memory mapped OTP address */
    /* Write Protection : OTP bit that is already set shouldnt be set again */
    result = Cy_MXOTPC_GetWriteProtectedData(otpRowNum, otpWriteData, &otpWriteProtData);
    if( result != CY_MXOTPC_SUCCESS )
        return result;

    /* otpWriteProtData = 0 means required OTP's set already, Nothing to Write */
    if(otpWriteProtData)
    {
        CY_SET_REG32((CY_MXOTPC_BASE + MXOTPC_SECTION_SIZE) + (otpRowNum * 4), otpWriteProtData);
    }

    /* Read back the Row Data and pass it back to caller for cross check */
    if(otpReadBackData)
        return Cy_MXOTPC_ReadRow(otpRowNum, otpReadBackData);

    return CY_MXOTPC_SUCCESS;
}

/*******************************************************************************
* Function Name: Cy_MXOTPC_IndirectReadBit
****************************************************************************//**
*
*  Reads a bit status of a particular OTP Row using Indirect access
*
* Indirect READ Sequence
* 1.    Check whether OTP is busy or not
* OTP_CMD.START == 0 // OTP is not busy
* 2.    WRITE OTP_CMD Register
* Write following value in OTP_CMD reg.
* For READ command (0x00) at 15th row and 3rd bit, and clearing previous read error if exist, you may use following value,
* 0X9000_0F03
* 3.    check completion of access
* Read 31st bit of OTP_CMD register to check completion of programming.
* OTP_CMD.START == 0 // access completed
* 4.    Check Read status & Read bit
* OTP_CMD.ReadVal // Read value from specified bit.
* OTP_CMD.RD_ERR == 1 // Error in bit reading
* Read ECC_STATUS register for ECC status of the row. More details on ECC refer to register description.
*
*  \param otpRowNum
*  Contains the Row Number, whose bit status should be Read
*  \param bitNum
*  Contains the bit position to be Read
*  \param otpReadBit
*  Contains the Read bit of bitNum position from the Row Number passed.
*
*  \return the MXOTPC API status \ref cy_mxotpc_status_t.
*
*******************************************************************************/
cy_mxotpc_status_t Cy_MXOTPC_IndirectReadBit(uint32_t otpRowNum, uint32_t bitNum, uint32_t *otpReadBit)
{
    cy_mxotpc_cmd_t otpCmd = {0, 0, 0};

    CY_ASSERT_L2(CY_MXOTPC_IS_ROW_VALID(otpRowNum));
    CY_ASSERT_L2(CY_MXOTPC_IS_BITNUM_VALID(bitNum));

    /* Indirect READ Sequence */
    /* 1.    Check whether OTP is busy or not
    * OTP_CMD.START == 0 // OTP is not busy */
    if(Cy_MXOTPC_OtpCheckStartStatus(CY_START_STATUS_CHECK_TIMEOUT) == CY_MXOTPC_TIMEOUT)
        return CY_MXOTPC_READ_ERROR;

    /* 2.    WRITE OTP_CMD Register
    * Write following value in OTP_CMD reg.
    * For READ command (0x00) at 15th row and 3rd bit, and clearing previous read error if exist, you may use following value,
    * 0X9000_0F03 */
    otpCmd.cmd = CY_MXOTPC_READ_CMD;
    otpCmd.rowAddr = otpRowNum;
    otpCmd.colAddr = bitNum;
    Cy_MXOTPC_OtpCmd(&otpCmd);

    /* 3.    check completion of access
    * Read 31st bit of OTP_CMD register to check completion of programming.
    * OTP_CMD.START == 0 // access completed */
    if(Cy_MXOTPC_OtpCheckStartStatus(CY_START_STATUS_CHECK_TIMEOUT) == CY_MXOTPC_TIMEOUT)
        return CY_MXOTPC_READ_ERROR;

    /* 4.    Check Read status & Read bit
    * OTP_CMD.ReadVal // Read value from specified bit.
    * OTP_CMD.RD_ERR == 1 // Error in bit reading
    * Read ECC_STATUS register for ECC status of the row. More details on ECC refer to register description.*/
    if(Cy_MXOTPC_OtpGetReadBitStatus())
        return CY_MXOTPC_READ_ERROR;

    *otpReadBit = Cy_MXOTPC_OtpIndirectReadBit();

    if(Cy_MXOTPC_OtpGetEccStatus(MXOTPC_ECC_STATUS_ECC_DBL_ERR_Msk))
        return CY_MXOTPC_READ_ECC_ERROR;

    return CY_MXOTPC_SUCCESS;
}

/*******************************************************************************
* Function Name: Cy_MXOTPC_ReadRowArray
****************************************************************************//**
*
*  Reads Array of Rows
*
*  \param otpRowNum
*  Contains the starting Row Number to be read
*  \param otpDstPointer
*  Contains the Destination Address where the data read has to be written back.
*  \param size
*  Contains the number of rows to be read from otpRowNum.
*
*  \return the MXOTPC API status \ref cy_mxotpc_status_t.
*
*******************************************************************************/
cy_mxotpc_status_t Cy_MXOTPC_ReadRowArray(uint32_t otpRowNum, uint32_t *otpDstPointer, uint32_t size)
{
    cy_mxotpc_status_t status = CY_MXOTPC_READ_ERROR;
    CY_ASSERT_L2(CY_MXOTPC_IS_SIZE_VALID(size));
    CY_ASSERT_L2(CY_MXOTPC_IS_ROW_VALID(otpRowNum));
    CY_ASSERT_L2(CY_MXOTPC_IS_DEST_ADDR_ALIGNED(otpDstPointer));
    CY_ASSERT_L2(CY_MXOTPC_IS_ARRAY_SIZE_VALID(otpRowNum, size));

    while(size != 0)
    {
        status = Cy_MXOTPC_ReadRow( otpRowNum, otpDstPointer);
        if(status != CY_MXOTPC_SUCCESS)
            break;

        otpRowNum++;
        otpDstPointer++;
        size--;
    }

    return status;
}

/*******************************************************************************
* Function Name: Cy_MXOTPC_WriteRowArray
****************************************************************************//**
*
*  Writes Array of Rows
*
*  \param otpRowNum
*  Contains the starting Row Number to be written
*  \param otpSrcPointer
*  Contains the Destination Address where the data has to be read has to be written to rows.
*  \param size
*  Contains the number of rows to be written from otpRowNum.
*  \param enable_ecc
*  Contains boolean, which tells to enable ECC for that row or not.
*
*  \return the MXOTPC API status \ref cy_mxotpc_status_t.
*
*******************************************************************************/
cy_mxotpc_status_t Cy_MXOTPC_WriteRowArray(uint32_t otpRowNum, uint32_t *otpSrcPointer, uint32_t size, bool enable_ecc)
{
    cy_mxotpc_status_t status = CY_MXOTPC_WRITE_ERROR;
    CY_ASSERT_L2(CY_MXOTPC_IS_SIZE_VALID(size));
    CY_ASSERT_L2(CY_MXOTPC_IS_ROW_VALID(otpRowNum));
    CY_ASSERT_L2(CY_MXOTPC_IS_DEST_ADDR_ALIGNED(otpSrcPointer));
    CY_ASSERT_L2(CY_MXOTPC_IS_ARRAY_SIZE_VALID(otpRowNum, size));

    while(size != 0)
    {
        status = Cy_MXOTPC_WriteRow( otpRowNum, *otpSrcPointer, enable_ecc);
        if(status != CY_MXOTPC_SUCCESS)
            break;

        otpRowNum++;
        otpSrcPointer++;
        size--;
    }

    return status;
}

#endif /* CY_IP_MXS28OTPC */
/* [] END OF FILE */

