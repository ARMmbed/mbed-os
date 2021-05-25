/***************************************************************************//**
* \file cy_mxotpc.h
* \version 1.0
*
* Provides the API declarations of the MXOTPC driver.
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

/**
* \addtogroup group_mxotpc
* \{
* MXOTPC is the OTP Controller which helps in read/write of OTP memory.
*
* The functions and other declarations used in this driver are in cy_mxotpc.h. 
* You can include cy_pdl.h (ModusToolbox only) to get access to all functions 
* and declarations in the PDL. 
* 
* You can use this driver to read/write OTP memory locations.
*
* \section group_mxotpc_more_information More Information
*
* For more information on the MXOTPC , refer to 
* the technical reference manual (TRM).
*
* \section group_mxotpc_MISRA MISRA-C Compliance
* The mxotpc driver does not have any specific deviations.
*
* \section group_mxotpc_changelog Changelog
* <table class="doxtable">
*   <tr><th>Version</th><th>Changes</th><th>Reason for Change</th></tr>
*   <tr>
*     <td>1.0</td>
*     <td>Initial version</td>
*     <td></td>
*   </tr>
* </table>
*
* \defgroup group_mxotpc_macros Macros
* \defgroup group_mxotpc_functions Functions
* \defgroup group_mxotpc_data_structures Data Structures
* \defgroup group_mxotpc_enums Enumerated Types
*/

#ifndef CY_MXOTPC_H
#define CY_MXOTPC_H

#include "cy_device.h"

#if defined (CY_IP_MXS28OTPC)

#include <stdint.h>
#include <stdbool.h>
#include "cy_syslib.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
* \addtogroup group_mxotpc_macros
* \{
*/
/** The MXOTPC driver identifier */
#define CY_MXOTPC_ID                                       CY_PDL_DRV_ID(0x36U)
/** CY_MXOTPC_MAX_OTP_ROW */
#define CY_MXOTPC_MAX_OTP_ROW                              512
/** CY_START_STATUS_CHECK_TIMEOUT */
#define CY_START_STATUS_CHECK_TIMEOUT                      1000

/** \} group_mxotpc_macros */


/** \cond PARAM_CHECK_MACROS */
/** Parameter check macros */ 

#define CY_MXOTPC_IS_DIRECT_CMD_VALID(cmd)     ((CY_MXOTPC_READ_CMD == (cmd)) || \
                                               (CY_MXOTPC_PROG_CMD == (cmd)) || \
                                               (CY_MXOTPC_PROG_ECC_CMD == (cmd)))
/** \endcond */


/**
* \addtogroup group_mxotpc_data_structures
* \{
*/
/** The OTP Control configuration structure. */
typedef struct
{
    bool fuseDisable;        /**< Latching of memory repair bits is disabled and forced to 0. */
    bool eccDblErrClear;     /**< When this field is set, ECC_DBL_ERR field of reg OTP_ECC_STATUS is cleared. */
    bool forceOtpClkEn;      /**< Force enable the OTP Clocks. */
    bool fuseReload;         /**< 1 - memory repair bits reload every time mxotpc is reset. 0- Memory repair bits reload only on deep sleep reset. */
    bool disCpuAccess;       /**< disables any command execution through CPU interface. */
    bool forcePowerDis;      /**< disable OTP power and force it into reset. */
} cy_mxotpc_ctl_config_t;

/** The OTP Command structure. */
typedef struct
{
    uint8_t  colAddr;                   /**< Column address of the OTP bit to be read only. Not considered for programming.*/
    uint16_t rowAddr;                   /**< Row address of OTP bit to be accessed.*/
    uint8_t  cmd;                       /**< OTP Controller command to access OTP.  */
} cy_mxotpc_cmd_t;


/** \} group_mxotpc_data_structures */

/*******************************************************************************
*       Enumerated Types
*******************************************************************************/

/**
* \addtogroup group_mxotpc_enums
* \{
*/

/** MXOTPC API return status */
typedef enum
{
    CY_MXOTPC_SUCCESS           =  0x00UL,                                    /**Success  */
    CY_MXOTPC_BAD_PARAM         =  CY_MXOTPC_ID | CY_PDL_STATUS_ERROR | 0x01UL,  /** Invalid function input parameter */
    CY_MXOTPC_TIMEOUT           =  CY_MXOTPC_ID | CY_PDL_STATUS_ERROR | 0x02UL,  /** Timeout occurred */
    CY_MXOTPC_INVALID           =  CY_MXOTPC_ID | CY_PDL_STATUS_ERROR | 0x03UL,  /** invalid state */
    CY_MXOTPC_READ_ERROR        =  CY_MXOTPC_ID | CY_PDL_STATUS_ERROR | 0x04UL,  /** Read Error  */
    CY_MXOTPC_WRITE_ERROR       =  CY_MXOTPC_ID | CY_PDL_STATUS_ERROR | 0x05UL,  /** Write Error  */
    CY_MXOTPC_WRITE_PROG_ERROR  =  CY_MXOTPC_ID | CY_PDL_STATUS_ERROR | 0x06UL,  /** OTP Programming Error  */
    CY_MXOTPC_WRITE_ECC_ERROR   =  CY_MXOTPC_ID | CY_PDL_STATUS_ERROR | 0x07UL,  /** OTP ECC Programming Error  */
    CY_MXOTPC_READ_ECC_ERROR    =  CY_MXOTPC_ID | CY_PDL_STATUS_ERROR | 0x08UL,  /** Read ECC Error  */
} cy_mxotpc_status_t;

/** MXOTPC Commands */
typedef enum
{
  CY_MXOTPC_READ_CMD,                       /** 0x00, READ */
  CY_MXOTPC_READBURST_CMD,                  /** 0x01, READBURST */
  CY_MXOTPC_OTP_PROGENABLE_CMD,             /** 0x02, OTP_PROGENABLE */
  CY_MXOTPC_OTP_PROGDISABLE_CMD,            /** 0x03, OTP_PROGDISABLE */
  CY_MXOTPC_PRESCREEN_CMD,                  /** 0x04, PRESCREEN */
  CY_MXOTPC_PRESCREEN_RP_CMD,               /** 0x05, PRESCREEN_RP */
  CY_MXOTPC_FLUSH_CMD,                      /** 0x06, FLUSH */
  CY_MXOTPC_NOP_CMD,                        /** 0x07, NOP */
  CY_MXOTPC_PROG_ECC_CMD,                   /** 0x08, PROG_ECC */
  CY_MXOTPC_PROG_ECC_WREAD_CMD,             /** 0x09, PROG_ECC_WREAD */
  CY_MXOTPC_PROG_CMD,                       /** 0x0a, PROG */
  CY_MXOTPC_PROGRAM_RP_CMD,                 /** 0x0b, PROGRAM_RP */
  CY_MXOTPC_PROGRAM_OVST_CMD,               /** 0x0c, PROGRAM_OVST */
  CY_MXOTPC_RELOAD_CMD,                     /** 0x0d, RELOAD */
  CY_MXOTPC_ERASE_CMD,                      /** 0x0e, ERASE */
  CY_MXOTPC_LOAD_RF_CMD,                    /** 0x0f, LOAD_RF */
  CY_MXOTPC_CTRL_WR_CMD,                    /** 0x10, CTRL_WR */
  CY_MXOTPC_CTRL_RD_CMD,                    /** 0x11, CTRL_RD */
  CY_MXOTPC_READ_HP_CMD,                    /** 0x12, READ_HP */
  CY_MXOTPC_READ_OVST_CMD,                  /** 0x13, READ_OVST */
  CY_MXOTPC_READ_VERIFY_Y0_CMD,             /** 0x14, READ_VERIFY_Y0 */
  CY_MXOTPC_READ_VERIFY_Y1_CMD,             /** 0x15, READ_VERIFY_Y1 */
  CY_MXOTPC_READ_FORCE0_CMD,                /** 0x16, READ_FORCE0 */
  CY_MXOTPC_READ_FORCE1_CMD,                /** 0x17, READ_FORCE1 */
  CY_MXOTPC_BURNIN_CMD,                     /** 0x18, BURNIN */
  CY_MXOTPC_PROGRAM_LOCK_CMD,               /** 0x19, PROGRAM_LOCK */
  CY_MXOTPC_PROGRAM_TESTCOL_CMD,            /** 0x1a, PROGRAM_TESTCOL */
  CY_MXOTPC_READ_TESTCOL_CMD,               /** 0x1b, READ_TESTCOL */
  CY_MXOTPC_READ_FOUT_CMD = 0x1e,           /** 0x1e, READ_FOUT */
  CY_MXOTPC_SET_RESET_CMD = 0x1f,           /** 0x1f, SET_RESET */
} cy_mxotpc_cmd_types_t;

/** \} group_mxotpc_enums */


/**
* \addtogroup group_mxotpc_functions
* \{
*/

cy_mxotpc_status_t Cy_MXOTPC_Init(cy_mxotpc_ctl_config_t const *mxotpcConfig);
cy_mxotpc_status_t Cy_MXOTPC_DeInit(void);
cy_mxotpc_status_t Cy_MXOTPC_WriteRowReadBack(uint32_t otpRowNum, uint32_t otpWriteData, uint32_t *otpReadBackData, bool enable_ecc);
cy_mxotpc_status_t Cy_MXOTPC_WriteRow(uint32_t otpRowNum, uint32_t otpWriteData, bool enable_ecc);
cy_mxotpc_status_t Cy_MXOTPC_WriteBitReadBack(uint32_t otpRowNum, uint32_t bitNum, uint32_t *otpReadBackData);
cy_mxotpc_status_t Cy_MXOTPC_WriteBit(uint32_t otpRowNum, uint32_t bitNum);
cy_mxotpc_status_t Cy_MXOTPC_WriteEcc(uint32_t otpRowNum);
cy_mxotpc_status_t Cy_MXOTPC_ReadRow(uint32_t otpRowNum, uint32_t *otpReadData);
cy_mxotpc_status_t Cy_MXOTPC_ReadBootRow( uint32_t *otpReadData);
cy_mxotpc_status_t Cy_MXOTPC_ReadBit(uint32_t otpRowNum, uint32_t bitNum, uint32_t *otpReadBit);
cy_mxotpc_status_t Cy_MXOTPC_DirectWriteRow(uint32_t otpRowNum, uint32_t otpWriteData, uint32_t *otpReadBackData, bool otpProgEccEnable);
cy_mxotpc_status_t Cy_MXOTPC_IndirectReadBit(uint32_t otpRowNum, uint32_t bitNum, uint32_t *otpReadBit);
cy_mxotpc_status_t Cy_MXOTPC_WriteProgramSequence(void);
cy_mxotpc_status_t Cy_MXOTPC_WriteRowArray(uint32_t otpRowNum, uint32_t *otpSrcPointer, uint32_t size, bool enable_ecc);
cy_mxotpc_status_t Cy_MXOTPC_ReadRowArray(uint32_t otpRowNum, uint32_t *otpDstPointer, uint32_t size);
__STATIC_INLINE void Cy_MXOTPC_IpEnable(void);
__STATIC_INLINE void Cy_MXOTPC_IpDisable(void);
__STATIC_INLINE cy_mxotpc_status_t Cy_MXOTPC_OtpCtlConfig(cy_mxotpc_ctl_config_t const *otpConfig);
__STATIC_INLINE cy_mxotpc_status_t Cy_MXOTPC_OtpCmd(cy_mxotpc_cmd_t *otpCmd);
__STATIC_INLINE void Cy_MXOTPC_OtpStartCmd(void);
__STATIC_INLINE void Cy_MXOTPC_OtpCpuProgCmd(cy_mxotpc_cmd_types_t otpCmd);
__STATIC_INLINE uint32_t Cy_MXOTPC_OtpGetStatus(uint32_t mask);
__STATIC_INLINE uint32_t Cy_MXOTPC_OtpGetStartStatus(void);
__STATIC_INLINE uint32_t Cy_MXOTPC_OtpGetReadBitStatus(void);
__STATIC_INLINE uint32_t Cy_MXOTPC_OtpGetEccStatus(uint32_t mask);
__STATIC_INLINE uint32_t Cy_MXOTPC_OtpGetFoutEccStatus(uint32_t mask);
__STATIC_INLINE void Cy_MXOTPC_OtpSetProgData(uint32_t otpData);
__STATIC_INLINE uint32_t Cy_MXOTPC_OtpIndirectReadBit(void);
__STATIC_INLINE cy_mxotpc_status_t Cy_MXOTPC_OtpCheckStartStatus(uint32_t timeOut);
__STATIC_INLINE void Cy_MXOTPC_OtpProgEnable(bool enable);

/** \} group_mxotpc_functions */

/**
* \addtogroup group_mxotpc_functions
* \{
*/

/*******************************************************************************
* Function Name: Cy_MXOTPC_IpEnable
****************************************************************************//**
*
* Enables the MXOTPC IP.
*
*
*******************************************************************************/
__STATIC_INLINE void Cy_MXOTPC_IpEnable(void)
{
    MXOTPC_CTL |= MXOTPC_CTL_ENABLED_Msk;
}

/*******************************************************************************
* Function Name: Cy_MXOTPC_IpDisable
****************************************************************************//**
*
* Disables the MXOTPC IP.
*
*
*******************************************************************************/
__STATIC_INLINE void Cy_MXOTPC_IpDisable(void)
{
    MXOTPC_CTL &= ~MXOTPC_CTL_ENABLED_Msk;
}

/*******************************************************************************
* Function Name: Cy_MxOTPC_OtpCtlConfig
****************************************************************************//**
*
* Configures MXOTPC through OTP_CTL register.
*
*  \param otpConfig
*  This structure contains the data that needs to be filled in OTP_CTL register.
*
*  \return cy_mxotpc_status_t
*
*******************************************************************************/
__STATIC_INLINE cy_mxotpc_status_t Cy_MXOTPC_OtpCtlConfig(cy_mxotpc_ctl_config_t const *otpConfig)
{
    cy_mxotpc_status_t ret = CY_MXOTPC_BAD_PARAM;
    if(otpConfig != NULL)
    {
       
        MXOTPC_OTP_CTL = _VAL2FLD(MXOTPC_OTP_CTL_FUSE_DISABLE, otpConfig->fuseDisable)              |
                         _VAL2FLD(MXOTPC_OTP_CTL_ECC_DBL_ERR_CLR, otpConfig->eccDblErrClear)        |
                         _VAL2FLD(MXOTPC_OTP_CTL_FORCE_OTP_CLK_EN, otpConfig->forceOtpClkEn)        |
                         _VAL2FLD(MXOTPC_OTP_CTL_FUSE_RELOAD_PER_PU, otpConfig->fuseReload)         |
                         _VAL2FLD(MXOTPC_OTP_CTL_DISCPUACCESS, otpConfig->disCpuAccess)             |
                         _VAL2FLD(MXOTPC_OTP_CTL_FORCE_OTP_PWR_DIS, otpConfig->forcePowerDis);

        ret = CY_MXOTPC_SUCCESS;
    }
    return ret;
}

/*******************************************************************************
* Function Name: Cy_MXOTPC_OtpProgEnable
****************************************************************************//**
*
* Enable/Disable the OTP CPU Program control
*
*  \param enable
*  Contians value to enable or disable the OTPPROG_EN bit of OTP_CTL
*
*******************************************************************************/
__STATIC_INLINE void Cy_MXOTPC_OtpProgEnable(bool enable)
{
    if(enable)
        MXOTPC_OTP_CTL |= MXOTPC_OTP_CTL_OTPPROG_EN_Msk;
    else
        MXOTPC_OTP_CTL &= ~MXOTPC_OTP_CTL_OTPPROG_EN_Msk;
}

/*******************************************************************************
* Function Name: Cy_MXOTPC_OtpCmd
****************************************************************************//**
*
* Writed the OTP_CMD register with row/column addresses and command type along
* with the START bit set.
*
*  \param otpCmd
*  Contains command which consists of Column/Row address and CMD to be sent to 
*  OTP Controller.
*
*  \return cy_mxotpc_status_t
*   
*******************************************************************************/
__STATIC_INLINE cy_mxotpc_status_t Cy_MXOTPC_OtpCmd(cy_mxotpc_cmd_t *otpCmd)
{
    cy_mxotpc_status_t ret = CY_MXOTPC_BAD_PARAM;
    
    if(otpCmd != NULL)
    {
       
        MXOTPC_OTP_CMD = _VAL2FLD(MXOTPC_OTP_CMD_COLADDR, otpCmd->colAddr)   |
                         _VAL2FLD(MXOTPC_OTP_CMD_ROWADDR, otpCmd->rowAddr)   |
                         _VAL2FLD(MXOTPC_OTP_CMD_CMD, otpCmd->cmd)           |
                         MXOTPC_OTP_CMD_RD_ERR_Msk                           |
                         MXOTPC_OTP_CMD_START_Msk;

        ret = CY_MXOTPC_SUCCESS;
    }
    return ret;
}

/*******************************************************************************
* Function Name: Cy_MXOTPC_OtpStartCmd
****************************************************************************//**
*
* Starts programming the command , which was filled by Cy_MXOTPC_OtpCmd
* Note: Call Cy_MXOTPC_OtpCmd, before calling this function.
*
*******************************************************************************/
__STATIC_INLINE void Cy_MXOTPC_OtpStartCmd(void)
{
    MXOTPC_OTP_CMD |= MXOTPC_OTP_CMD_START_Msk;
}

/*******************************************************************************
* Function Name: Cy_MXOTPC_OtpCpuProgCmd
****************************************************************************//**
*
* Used with Direct access , Programs the CPU_PROG_CMD register with the command.
*
*  \param otpCmd
*  Contains command which consists of Column/Row address and CMD to be sent to 
*  OTP Controller
*
*******************************************************************************/
__STATIC_INLINE void Cy_MXOTPC_OtpCpuProgCmd(cy_mxotpc_cmd_types_t otpCmd)
{
    CY_ASSERT_L2(CY_MXOTPC_IS_DIRECT_CMD_VALID((cy_mxotpc_cmd_types_t)otpCmd));
    
    MXOTPC_CPU_PROG_CMD = _VAL2FLD(MXOTPC_CPU_PROG_CMD_PROG_CMD, otpCmd);
}

/*******************************************************************************
* Function Name: Cy_MXOTPC_OtpGetStatus
****************************************************************************//**
*
* Get the status of a parameter from OTP_STATUS Register.
*
*  \param mask
*  Contains the mask value of parameter whose status needs to be obtained
*
*  \return
*  Non-zero if status bit(s) are set,  Zero if status bit(s) are not set
*  Note: LOCK status is 8 bits, so return value can be used to figure out which 
*  lock bit is set.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_MXOTPC_OtpGetStatus(uint32_t mask)
{
    return (MXOTPC_OTP_STATUS & mask);
}

/*******************************************************************************
* Function Name: Cy_MXOTPC_OtpGetStatus
****************************************************************************//**
*
* Get the status of start command done by OTP_CTL.START bit.
*
*  \return
*  1 if START bit is set,  0 if START bit is not set
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_MXOTPC_OtpGetStartStatus(void)
{
    return ((MXOTPC_OTP_CMD & MXOTPC_OTP_CMD_START_Msk) ? 1 : 0);
}

/*******************************************************************************
* Function Name: Cy_MXOTPC_OtpGetReadBitStatus
****************************************************************************//**
*
* Get the read status from OTP_CMD.RD_ERR bit.
*
*  \return
*  1 if RD_ERR bit is set,  0 if RD_ERR bit is not set
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_MXOTPC_OtpGetReadBitStatus(void)
{
    return ((MXOTPC_OTP_CMD & MXOTPC_OTP_CMD_RD_ERR_Msk) ? 1 : 0);
}

/*******************************************************************************
* Function Name: Cy_MXOTPC_OtpGetEccStatus
****************************************************************************//**
*
* Get the read status from ECC_STATUS Register.
*
*  \param mask
*  Contains the mask value of parameter whose status needs to be obtained
*
*  \return
*  Non-zero if status bit(s) are set,  Zero if status bit(s) are not set
*  Note: LOCK status is 8 bits, so return value can be used to figure out which 
*  lock bit is set.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_MXOTPC_OtpGetEccStatus(uint32_t mask)
{
    return (MXOTPC_ECC_STATUS & mask);
}

/*******************************************************************************
* Function Name: Cy_MXOTPC_OtpGetFoutEccStatus
****************************************************************************//**
*
* Get the read status from FOUT_ECC_STATUS Register.
*
*  \param mask
*  Contains the mask value of parameter whose status needs to be obtained
*
*  \return
*  1 if status bit is set,  0 if status bit is not set
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_MXOTPC_OtpGetFoutEccStatus(uint32_t mask)
{
    return ((MXOTPC_FOUT_ECC_STATUS & mask) ? 1 : 0);
}

/*******************************************************************************
* Function Name: Cy_MXOTPC_OtpProgCtrl
****************************************************************************//**
*
* Writes in to OTP_PROGDATA Register.
*
*  \param otpData
*  Contians value to be written in to OTP_PROGDATA Register.
*
*******************************************************************************/
__STATIC_INLINE void Cy_MXOTPC_OtpSetProgData(uint32_t otpData)
{
    MXOTPC_OTP_PROGDATA = otpData;
}

/*******************************************************************************
* Function Name: Cy_MXOTPC_OtpIndirectReadBit
****************************************************************************//**
*
* Get the read bit from OTP_CMD.READVAL
*
* Note: This API  should be called after Cy_MXOTPC_OtpCmd & Cy_MXOTPC_OtpStartCmd.
*
*  \return
*  1 if status bit is set,  0 if status bit is not set
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_MXOTPC_OtpIndirectReadBit(void)
{
    return ((MXOTPC_OTP_CMD & MXOTPC_OTP_CMD_READVAL_Msk) ? 1 : 0);
}

/*******************************************************************************
* Function Name: Cy_MXOTPC_OtpCheckStartStatus
****************************************************************************//**
*
* Checks the status of OTP_CMD.START bit for timeOut milliseconds.
*
*  \return
*  CY_MXOTPC_SUCCESS if START bit is 0 within timeOut milliseconds
*  CY_MXOTPC_TIMEOUT if START bit is 1 within timeOut milliseconds
*
*******************************************************************************/
__STATIC_INLINE cy_mxotpc_status_t Cy_MXOTPC_OtpCheckStartStatus(uint32_t timeOut)
{
    while(timeOut) 
    {
        if(!Cy_MXOTPC_OtpGetStartStatus())
            return CY_MXOTPC_SUCCESS;
        Cy_SysLib_Delay(1);
        timeOut--;
    }

    return CY_MXOTPC_TIMEOUT;
}

/*******************************************************************************
* Function Name: Cy_MXOTPC_LockCCRegion
****************************************************************************//**
*
* Configure CC312_RGN_LOCK_CTL register
*
*  \param mask
*  Contians mask value to enable a particular lock region on CC312_RGN_LOCK_CTL register.
*
*******************************************************************************/
__STATIC_INLINE void Cy_MXOTPC_LockCCRegion(uint32_t mask)
{
    MXOTPC_CC312_RGN_LOCK_CTL |= mask;
}

/** \} group_mxotpc_functions */

#ifdef __cplusplus
}
#endif

#endif /* CY_IP_MXS28OTPC */

#endif /* CY_MXOTPC_H */

/** \} group_mxotpc */

/* [] END OF FILE */

