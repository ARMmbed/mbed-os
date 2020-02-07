/***************************************************************************//**
* \file cy_smif_memslot.c
* \version 1.50
*
* \brief
*  This file provides the source code for the memory-level APIs of the SMIF driver.
*
* Note:
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

#include "cy_smif_memslot.h"

#ifdef CY_IP_MXSMIF

#if defined(__cplusplus)
extern "C" {
#endif

/** \cond INTERNAL */
/***************************************
*     Internal Constants
***************************************/

#define READ_ENHANCED_MODE_DISABLED (0xFFU)
#define BITS_IN_BYTE                (8U)
#define BYTES_IN_DWORD              (4U)
#define FOUR_BYTE_ADDRESS           (4U)     /* 4 byte addressing mode */
#define BITS_IN_BYTE_ABOVE_4GB      (3U)     /* Density of memory above 4GBit stored as poser of 2 */
#define PARAM_HEADERS_NUM           (CY_SMIF_SFDP_BFPT_BYTE_06)
#define FIRST_HEADER_OFFSET         (0x08U)  /* The offset of the 1-st Parameter Header */
#define PARAM_ID_MSB_REL_OFFSET     (0x07U)  /* The relative offset of Parameter ID MSB 
                                             * in the SFDP Header table 
                                             */
#define PARAM_MINOR_REV_REL_OFFSET  (0x01U)  /* The relative offset of Parameter Minor Revision 
                                             * in the SFDP Header table 
                                             */
#define PARAM_MAJOR_REV_REL_OFFSET  (0x02U)  /* The relative offset of Parameter Major Revision
                                             * in the SFDP Header table
                                             */
#define PARAM_ID_MSB_OFFSET         (0x08U)  /* The offset of Parameter ID MSB */
#define PARAM_ID_LSB_MASK           (0xFFUL) /* The mask of Parameter ID LSB */
#define PARAM_TABLE_PRT_OFFSET      (0x04UL) /* The relative offset of Parameter Table Pointer Byte 1 */
#define PARAM_TABLE_LENGTH_OFFSET   (0X03U)  /* The offset of Parameter Table Length in the Header Table */
#define PARAM_HEADER_NUM            (6U)     /* The supported number of the parameter headers */
#define HEADER_LENGTH               (0x8U)   /* The length of the SFDP header */
#define HEADERS_LENGTH              (HEADER_LENGTH + \
                                                 (CY_SMIF_SFDP_PARAM_HEADER_LENGTH * PARAM_HEADER_NUM))
#define TYPE_STEP                   (2UL)    /* The Erase Type step in the Basic Flash Parameter Table */
#define INSTRUCTION_NOT_SUPPORTED   (0XFFU)  /* The code for the not supported instruction */
#define BASIC_SPI_ID_LSB            (0X00UL) /* The JEDEC SFDP Basic SPI Flash Parameter ID LSB */
#define BASIC_SPI_ID_MSB            (0XFFUL) /* The JEDEC SFDP Basic SPI Flash Parameter ID MSB */
#define SECTOR_MAP_ID_LSB            (0x81UL) /* The JEDEC SFDP Sector Map ID LSB */
#define SECTOR_MAP_ID_MSB            (0xFFUL) /* The JEDEC SFDP Sector Map ID MSB */
#define SECTOR_MAP_DESCRIPTOR_MASK   (0x2U)  /* The mask for the type bit of the Sector Map descriptor */
#define SECTOR_MAP_COMAND_DESCRIPTOR_TYPE (0U) /* Code for the command descriptor type */
#define SECTOR_MAP_REGION_SIZE_MULTIPLIER (256UL) /* The multiplier for region size units */
#define FOUR_BYTE_ADDR_ID_LSB       (0X84UL) /* The 4-byte Address Instruction Table is assigned the ID LSB of 84h */
#define FOUR_BYTE_ADDR_ID_MSB       (0XFFUL) /* The 4-byte Address Instruction Table is assigned the ID MSB of FFh */
#define FOUR_BYTE_ADDR_ERASE_TYPE_1 (0X4UL)  /* The Erase Type 1 offset in 4-byte Address Instruction Table */
#define FOUR_BYTE_ADDR_ERASE_TYPE_4 (0X7UL)  /* The Erase Type 4 offset in 4-byte Address Instruction Table */
#define ERASE_T_COUNT_Pos           (0UL)    /* Erase Type X Erase, Typical time: count (Bits 4:0) */
#define ERASE_T_COUNT_Msk           (0x1FUL) /* Erase Type X Erase, Typical time: count (Bitfield-Mask) */
#define ERASE_T_UNITS_Pos           (5UL)    /* Erase Type X Erase, Typical time: units (Bits 6:5) */
#define ERASE_T_UNITS_Msk           (0x60UL) /* Erase Type X Erase, Typical time: units (Bitfield-Mask) */
#define ERASE_T_COUNT_OFFSET        (0x04U)  /* The offset of the Erase count 10th DWORD */
#define ERASE_T_LENGTH              (0x07U)  /* The Erase Type Typical time length */
#define COMMAND_IS_NOT_FOUND        (0x0U) 
#define PARAMETER_IS_NOT_FOUND      (0x0U)
#define SMIF_MAX_RX_COUNT           (65536UL)
#define ONE_MILLI_IN_MICRO          (1000UL)
#define SMIF_TRANSFER_TIMEOUT       (1000UL) /* The timeout (microseconds) to use in polling of 
                                             * the transfer status of the SMIF block 
                                             */
#define TIMEOUT_SLICE_DIV           (4U)     /* The division factor to use for slicing the timeout
                                             * while polling the memory
                                             */
#define SUPPORT_ERASE_COMMAND_Pos   (1UL)    /* The position of the Support for Erase Commands 
                                             * in byte 1 of the JEDEC 4-byte Address Instruction Table 
                                             */ 
#define SUPPORT_ERASE_COMMAND_Msk   (0x0000001EUL) /* The mask of the Support for Erase Commands
                                                   * in the JEDEC 4-byte Address Instruction Table 
                                                   */                                                   
#define SUPPORT_FAST_READ_1S_1S_1S_CMD_Pos (1UL)          /* The position of the Support for Fast Read Command 1S-1S-1S 
                                                          * in the JEDEC 4-byte Address Instruction Table, DWORD 1 
                                                          */
#define SUPPORT_FAST_READ_1S_1S_1S_CMD_Msk (0x00000002UL) /* The mask of the Support for Fast Read Command 1S-1S-1S
                                                          * in the JEDEC 4-byte Address Instruction Table, DWORD 1
                                                          */
#define SUPPORT_FAST_READ_1S_1S_2S_CMD_Pos (2UL)          /* The position of the Support for Fast Read Command 1S-1S-2S 
                                                          * in the JEDEC 4-byte Address Instruction Table, DWORD 1 
                                                          */
#define SUPPORT_FAST_READ_1S_1S_2S_CMD_Msk (0x00000004UL) /* The mask of the Support for Fast Read Command 1S-1S-2S
                                                          * in the JEDEC 4-byte Address Instruction Table, DWORD 1
                                                          */
#define SUPPORT_FAST_READ_1S_2S_2S_CMD_Pos (3UL)          /* The position of the Support for Fast Read Command 1S-2S-2S 
                                                          * in the JEDEC 4-byte Address Instruction Table, DWORD 1 
                                                          */
#define SUPPORT_FAST_READ_1S_2S_2S_CMD_Msk (0x00000008UL) /* The mask of the Support for Fast Read Command 1S-2S-2S
                                                          * in the JEDEC 4-byte Address Instruction Table, DWORD 1
                                                          */                                                    
#define SUPPORT_FAST_READ_1S_1S_4S_CMD_Pos (4UL)          /* The position of the Support for Fast Read Command 1S-1S-4S 
                                                          * in the JEDEC 4-byte Address Instruction Table, DWORD 1 
                                                          */
#define SUPPORT_FAST_READ_1S_1S_4S_CMD_Msk (0x00000010UL) /* The mask of the Support for Fast Read Command 1S-1S-4S
                                                          * in the JEDEC 4-byte Address Instruction Table, DWORD 1
                                                          */
#define SUPPORT_FAST_READ_1S_4S_4S_CMD_Pos (5UL)          /* The position of the Support for Fast Read Command 1S-4S-4S 
                                                          * in the JEDEC 4-byte Address Instruction Table, DWORD 1 
                                                          */
#define SUPPORT_FAST_READ_1S_4S_4S_CMD_Msk (0x00000020UL) /* The mask of the Support for Fast Read Command 1S-4S-4S
                                                          * in the JEDEC 4-byte Address Instruction Table, DWORD 1
                                                          */                             
#define SUPPORT_PP_1S_1S_1S_CMD_Pos (6UL)              /* The position of the Support for Page Program Command 1S-1S-1S 
                                                        * in the JEDEC 4-byte Address Instruction Table, DWORD 1 
                                                        */                                                 
#define SUPPORT_PP_1S_1S_1S_CMD_Msk (0x00000040UL)     /* The mask of the Support for Page Program Command 1S-1S-1S
                                                        * in the JEDEC 4-byte Address Instruction Table, DWORD 1
                                                        */
#define SUPPORT_PP_1S_1S_4S_CMD_Pos (7UL)              /* The position of the Support for Page Program Command 1S-1S-4S 
                                                        * in the JEDEC 4-byte Address Instruction Table, DWORD 1 
                                                        */                                                        
#define SUPPORT_PP_1S_1S_4S_CMD_Msk (0x00000080UL)     /* The mask of the Support for Page Program Command 1S-1S-4S
                                                        * in the JEDEC 4-byte Address Instruction Table, DWORD 1
                                                        */
#define SUPPORT_PP_1S_4S_4S_CMD_Pos (8UL)              /* The position of the Support for Page Program Command 1S-4S-4S 
                                                        * in the JEDEC 4-byte Address Instruction Table, DWORD 1 
                                                        */                                                         
#define SUPPORT_PP_1S_4S_4S_CMD_Msk (0x00000100UL)     /* The mask of the Support for Page Program Command 1S-4S-4S
                                                        * in the JEDEC 4-byte Address Instruction Table, DWORD 1 
                                                        */                                               
#define FOUR_BYTE_ADDRESS_TABLE_BYTE_0 (0U)  /* Byte 0x00 of the JEDEC 4-byte Address Instruction Table */
#define FOUR_BYTE_ADDRESS_TABLE_BYTE_1 (1U)  /* Byte 0x01 of the JEDEC 4-byte Address Instruction Table */
#define ERASE_TYPE_COUNT               (4U)  /* The number of Erase Types  */

#define MEM_ADDR_VALID(addr, size) (0U == ((addr)%(size)))  /* This address must be a multiple of 
                                                             * the SMIF XIP memory size 
                                                             */
#define MEM_MAPPED_SIZE_VALID(size) (((size) >= 0x10000U) && (0U == ((size)&((size)-1U))) )
#define MEM_ADDR_SIZE_VALID(addrSize)  ((0U < (addrSize)) && ((addrSize) <= CY_SMIF_FOUR_BYTES_ADDR))


/***************************************
*     Internal enums
***************************************/
/** Specifies protocol mode. */
typedef enum
{
    PROTOCOL_MODE_1S_1S_1S        = 1U,     /**< One DQ signal used during command transfer,
                                             * address transfer, and data transfer. All phases are SDR.
                                             */
    PROTOCOL_MODE_1S_1S_2S        = 2U,     /**< One DQ signal used during command transfer, and address transfer,
                                             *  two DQ signals used during data transfer. All phases are SDR.
                                             */
    PROTOCOL_MODE_1S_2S_2S        = 3U,     /**< One DQ signal used during command transfer, two DQ signals used
                                             *  during address transfer, and data transfer. All phases are SDR.
                                             */
    PROTOCOL_MODE_1S_1S_4S        = 4U,     /**< One DQ signal used during command and address transfer, 
                                             * four DQ signals used during data transfer. All phases are SDR.
                                             */
    PROTOCOL_MODE_1S_4S_4S        = 5U,     /**< One DQ signal used during command transfer, four DQ signals used 
                                            * during address transfer, and data transfer. All phases are SDR.
                                            */
    PROTOCOL_MODE_WRONG           = 0xFFU   /**< Unknown or unsupported mode */   
                                      
} cy_en_smif_protocol_mode_t;
/** \endcond*/


/***************************************
*     Internal Structures
***************************************/

/**
* This internal structure is used to store data for erase types.
*/
typedef struct
{
    uint8_t eraseCmd;  /**< The instruction used for erase transaction*/
    uint32_t eraseSize;    /**< The number of bytes to be erased at one erase transaction*/
    uint32_t eraseTime;    /**< The maximum erase time for one erase transaction */
} cy_stc_smif_erase_type_t;


/***************************************
*     Internal Function Prototypes
***************************************/
static void XipRegInit(SMIF_DEVICE_Type volatile *dev,
                            cy_stc_smif_mem_config_t const * memCfg);
static cy_en_smif_status_t SfdpReadBuffer(SMIF_Type *base, 
                                         cy_stc_smif_mem_cmd_t const *cmdSfdp,
                                         uint8_t const sfdpAddress[],
                                         cy_en_smif_slave_select_t  slaveSelect,
                                         uint32_t size,
                                         uint8_t sfdpBuffer[],
                                         cy_stc_smif_context_t *context);
static uint32_t SfdpFindParameterHeader(uint32_t id, uint8_t const sfdpBuffer[]);
static cy_en_smif_status_t SfdpFindParameterTableAddress(uint32_t id,
                                                     uint8_t const sfdpBuffer[],
                                                     uint8_t address[],
                                                     uint32_t *tableLength);
static uint32_t SfdpGetNumOfAddrBytes(uint8_t const sfdpBuffer[]);
static uint32_t SfdpGetMemoryDensity(uint8_t const sfdpBuffer[]);
static void SfdpGetReadCmd_1_4_4(uint8_t const sfdpBuffer[], 
                             cy_stc_smif_mem_cmd_t* cmdRead);
static void SfdpGetReadCmd_1_1_4(uint8_t const sfdpBuffer[], 
                             cy_stc_smif_mem_cmd_t* cmdRead);
static void SfdpGetReadCmd_1_2_2(uint8_t const sfdpBuffer[], 
                             cy_stc_smif_mem_cmd_t* cmdRead);
static void SfdpGetReadCmd_1_1_2(uint8_t const sfdpBuffer[], 
                             cy_stc_smif_mem_cmd_t* cmdRead);
static void SfdpGetReadCmd_1_1_1(uint8_t const sfdpBuffer[], 
                             cy_stc_smif_mem_cmd_t* cmdRead);
static cy_en_smif_protocol_mode_t SfdpGetReadCmdParams(uint8_t const sfdpBuffer[], 
                                                       cy_en_smif_data_select_t dataSelect, 
                                                       cy_stc_smif_mem_cmd_t* cmdRead);
static void SfdpGetReadFourBytesCmd(uint8_t const sfdpBuffer[], 
                                    cy_en_smif_protocol_mode_t protocolMode, 
                                    cy_stc_smif_mem_cmd_t* cmdRead);                                
static uint32_t SfdpGetPageSize(uint8_t const sfdpBuffer[]);
static uint32_t SfdpGetEraseTime(uint32_t const eraseOffset, uint8_t const sfdpBuffer[], cy_stc_smif_erase_type_t eraseType[]);
static uint32_t SfdpGetChipEraseTime(uint8_t const sfdpBuffer[]);
static uint32_t SfdpGetPageProgramTime(uint8_t const sfdpBuffer[]);
static void SfdpSetWriteEnableCommand(cy_stc_smif_mem_cmd_t* cmdWriteEnable);
static void SfdpSetWriteDisableCommand(cy_stc_smif_mem_cmd_t* cmdWriteDisable);
static void SfdpSetProgramCommand_1_1_1(cy_stc_smif_mem_cmd_t* cmdProgram);
static void SfdpSetProgramCommandFourBytes_1_1_1(cy_stc_smif_mem_cmd_t* cmdProgram);
static void SfdpSetProgramCommandFourBytes_1_1_4(cy_stc_smif_mem_cmd_t* cmdProgram);
static void SfdpSetProgramCommandFourBytes_1_4_4(cy_stc_smif_mem_cmd_t* cmdProgram);
static void SfdpGetProgramFourBytesCmd(uint8_t const sfdpBuffer[], 
                                       cy_en_smif_protocol_mode_t protocolMode, 
                                       cy_stc_smif_mem_cmd_t* cmdProgram);
static void SfdpGetQuadEnableParameters(cy_stc_smif_mem_device_cfg_t *device, 
                                    uint8_t const sfdpBuffer[]);
static void SfdpSetChipEraseCommand(cy_stc_smif_mem_cmd_t* cmdChipErase);
static uint32_t SfdpGetSectorEraseCommand(cy_stc_smif_mem_device_cfg_t *device, 
                                          uint8_t const sfdpBuffer[],
                                          cy_stc_smif_erase_type_t eraseTypeStc[]);
static cy_en_smif_status_t ReadAnyReg(SMIF_Type *base, cy_en_smif_slave_select_t slaveSelect,
                                          uint8_t *value, uint8_t command, uint8_t const *address,
                                          uint32_t addressSize, cy_stc_smif_context_t const *context);
static cy_en_smif_status_t SfdpEnterFourByteAddressing(SMIF_Type *base, uint8_t entryMethodByte, 
                                                              cy_stc_smif_mem_device_cfg_t *device,
                                                              cy_en_smif_slave_select_t slaveSelect,
                                                              cy_stc_smif_context_t const *context);
static void SfdpGetEraseSizeAndCmd(uint8_t const sfdpBuffer[], cy_stc_smif_erase_type_t eraseType[]);
static cy_en_smif_status_t SfdpPopulateRegionInfo(SMIF_Type *base, uint8_t const sectorMapBuff[],
                                   uint32_t const buffLength, cy_stc_smif_mem_device_cfg_t *device,
                                    cy_en_smif_slave_select_t slaveSelect, const cy_stc_smif_context_t *context,
                                    cy_stc_smif_erase_type_t eraseType[]);
static void SfdpSetWipStatusRegisterCommand(cy_stc_smif_mem_cmd_t* readStsRegWipCmd);
static cy_en_smif_status_t PollTransferStatus(SMIF_Type const *base, cy_en_smif_txfr_status_t transferStatus,
                                              cy_stc_smif_context_t const *context);
static void ValueToByteArray(uint32_t value, uint8_t *byteArray, uint32_t startPos, uint32_t size);
static uint32_t ByteArrayToValue(uint8_t const *byteArray, uint32_t size);

/*******************************************************************************
* Function Name: Cy_SMIF_MemInit
****************************************************************************//**
*
* This function initializes the slots of the memory device in the SMIF
* configuration.
* This function must be called when a memory device is required to be used
* in memory-mapped (XIP) mode. This function can also be called instead of
* calling \ref Cy_SMIF_MemSfdpDetect when SFDP auto-discovery is enabled.
* Note that this function performs SFDP on all the external memories whereas
* \ref Cy_SMIF_MemSfdpDetect peforms it only on one memory that is specified
* through the arguments. This function configures the SMIF device slot registers
* with the configuration from \ref cy_stc_smif_mem_config_t structure which is 
* a member of the \ref cy_stc_smif_block_config_t structure. If SFDP discovery
* is enabled in the configuration strucutre through autoDetectSfdp field,
* this function calls \ref Cy_SMIF_MemSfdpDetect function for each memory,
* fills the structures with the discovered parameters, and configures the
* SMIF device slot registers accordingly. \ref Cy_SMIF_Init must have been 
* called prior to calling this funciton.
* The \ref cy_stc_smif_context_t context structure returned from \ref Cy_SMIF_Init
* is passed as a parameter to this function.
*
* \note 4-byte addressing mode is set when the memory device supports 
*       3- or 4-byte addressing mode.
*
* \param base
* The address of the slave-slot device register to initialize.
*
* \param blockConfig
* The configuration structure array that configures the SMIF memory device to be
* mapped into the PSoC memory map. \ref cy_stc_smif_mem_config_t
*
* \param context
* This is the pointer to the context structure \ref cy_stc_smif_context_t
* allocated by the user. The structure is used during the SMIF
* operation for internal configuration and data retention. The user must not
* modify anything in this structure.
*
* \return The memory slot initialization status.
*       - \ref CY_SMIF_SUCCESS
*       - \ref CY_SMIF_BAD_PARAM
*       - \ref CY_SMIF_SFDP_SS0_FAILED
*       - \ref CY_SMIF_SFDP_SS1_FAILED
*       - \ref CY_SMIF_SFDP_SS2_FAILED
*       - \ref CY_SMIF_SFDP_SS3_FAILED
*
*******************************************************************************/
cy_en_smif_status_t Cy_SMIF_MemInit(SMIF_Type *base,
                            cy_stc_smif_block_config_t const * blockConfig,
                            cy_stc_smif_context_t *context)
{
    SMIF_DEVICE_Type volatile * device;
    cy_stc_smif_mem_config_t const * memCfg;
    uint32_t result = (uint32_t)CY_SMIF_BAD_PARAM;
    uint32_t sfdpRes =(uint32_t)CY_SMIF_SUCCESS;
    uint32_t idx;

    if ((NULL != base) && (NULL != blockConfig) && (NULL != blockConfig->memConfig) 
        && (NULL != context) && (0U != blockConfig->memCount))
    {
        uint32_t size = blockConfig->memCount;
        cy_stc_smif_mem_config_t** extMemCfg = blockConfig->memConfig;

        result = (uint32_t)CY_SMIF_SUCCESS;
        for(idx = 0UL; idx < size; idx++)
        {
            memCfg = extMemCfg[idx];
            if (NULL != memCfg)
            {
                /* Check smif memory slot configuration*/
                CY_ASSERT_L3(CY_SMIF_SLAVE_SEL_VALID(memCfg->slaveSelect));
                CY_ASSERT_L3(CY_SMIF_DATA_SEL_VALID(memCfg->dataSelect));
                CY_ASSERT_L1(NULL != memCfg->deviceCfg);
                
                device = Cy_SMIF_GetDeviceBySlot(base, memCfg->slaveSelect);
                if (NULL != device)
                {
                    /* The slave-slot initialization of the device control register.
                     * Cy_SMIF_MemSfdpDetect() must work */
                    SMIF_DEVICE_CTL(device)  = _CLR_SET_FLD32U(SMIF_DEVICE_CTL(device),
                                                               SMIF_DEVICE_CTL_DATA_SEL,
                                                              (uint32_t)memCfg->dataSelect);
                    uint32_t sfdpRet = (uint32_t)CY_SMIF_SUCCESS;
                    if (0U != (memCfg->flags & CY_SMIF_FLAG_DETECT_SFDP))
                    {
                        sfdpRet = (uint32_t)Cy_SMIF_MemSfdpDetect(base,
                                                memCfg->deviceCfg,
                                                memCfg->slaveSelect,
                                                memCfg->dataSelect,
                                                context);
                        if((uint32_t)CY_SMIF_SUCCESS != sfdpRet)
                        {
                            sfdpRes |=  ((uint32_t)CY_SMIF_SFDP_FAIL << idx);
                        }
                    }
                    /* Check the size of the smif memory slot address */
                    CY_ASSERT_L2(MEM_ADDR_SIZE_VALID(memCfg->deviceCfg->numOfAddrBytes));

                    if (((uint32_t)CY_SMIF_SUCCESS == sfdpRet) &&
                            (0U != (memCfg->flags & CY_SMIF_FLAG_MEMORY_MAPPED)))
                    {
                        /* Check valid parameters for XIP */
                        CY_ASSERT_L3(MEM_ADDR_VALID( memCfg->baseAddress, memCfg->memMappedSize));
                        CY_ASSERT_L3(MEM_MAPPED_SIZE_VALID( memCfg->memMappedSize));
                        
                        XipRegInit(device, memCfg);

                        /* The device control register initialization */
                        SMIF_DEVICE_CTL(device) = (memCfg->flags & CY_SMIF_FLAG_WRITE_ENABLE) |
                                      (memCfg->flags & CY_SMIF_FLAG_CRYPTO_ENABLE) |
                                      _VAL2FLD(SMIF_DEVICE_CTL_DATA_SEL,  (uint32_t)memCfg->dataSelect) |
                                      SMIF_DEVICE_CTL_ENABLED_Msk;
                    }
                }
                else
                {
                    result = (uint32_t)CY_SMIF_BAD_PARAM;
                    break;
                }
            }
        }
    }
    if((uint32_t)CY_SMIF_SUCCESS != sfdpRes)
    {
        result = CY_SMIF_ID | CY_PDL_STATUS_ERROR | sfdpRes;
    }
    return (cy_en_smif_status_t) result;
}


/*******************************************************************************
* Function Name: XipRegInit
****************************************************************************//**
*
* \internal
* This function initializes the memory device registers used for the XIP mode of
* the specified device.
*
* \param dev
* The SMIF memory device registers structure. \ref SMIF_DEVICE_Type
*
* \param memCfg
* The memory configuration structure that configures the SMIF memory device to
*  map into the PSoC memory map. \ref cy_stc_smif_mem_config_t
*
*******************************************************************************/
static void XipRegInit(SMIF_DEVICE_Type volatile *dev, cy_stc_smif_mem_config_t const * memCfg)
{
    cy_stc_smif_mem_device_cfg_t const * devCfg = memCfg->deviceCfg;
    cy_stc_smif_mem_cmd_t const * read = devCfg->readCmd;
    cy_stc_smif_mem_cmd_t const * prog = devCfg->programCmd;

    SMIF_DEVICE_ADDR(dev) = (SMIF_DEVICE_ADDR_ADDR_Msk & memCfg->baseAddress);

    /* Convert the size in the mask*/
    SMIF_DEVICE_MASK(dev)= (SMIF_DEVICE_MASK_MASK_Msk & (~(memCfg->memMappedSize) + 1UL));

    SMIF_DEVICE_ADDR_CTL(dev) = _VAL2FLD(SMIF_DEVICE_ADDR_CTL_SIZE2, (devCfg->numOfAddrBytes - 1UL)) |
                                ((0UL != memCfg->dualQuadSlots)? SMIF_DEVICE_ADDR_CTL_DIV2_Msk: 0UL);

    if(NULL != read)
    {
        SMIF_DEVICE_RD_CMD_CTL(dev) = (CY_SMIF_NO_COMMAND_OR_MODE != read->command) ?
                                    (_VAL2FLD(SMIF_DEVICE_RD_CMD_CTL_CODE,  (uint32_t)read->command)  |
                                    _VAL2FLD(SMIF_DEVICE_RD_CMD_CTL_WIDTH, (uint32_t)read->cmdWidth) |
                                    SMIF_DEVICE_RD_CMD_CTL_PRESENT_Msk)
                                    : 0U;

        SMIF_DEVICE_RD_ADDR_CTL(dev) = _VAL2FLD(SMIF_DEVICE_RD_ADDR_CTL_WIDTH, (uint32_t)read->addrWidth);

        SMIF_DEVICE_RD_MODE_CTL(dev) = (CY_SMIF_NO_COMMAND_OR_MODE != read->mode) ?
                                    (_VAL2FLD(SMIF_DEVICE_RD_CMD_CTL_CODE,  (uint32_t)read->mode)     |
                                        _VAL2FLD(SMIF_DEVICE_RD_CMD_CTL_WIDTH, (uint32_t)read->modeWidth)|
                                        SMIF_DEVICE_RD_CMD_CTL_PRESENT_Msk)
                                    : 0U;

        SMIF_DEVICE_RD_DUMMY_CTL(dev) = (0UL != read->dummyCycles)?
                                        (_VAL2FLD(SMIF_DEVICE_RD_DUMMY_CTL_SIZE5, (read->dummyCycles - 1UL)) |
                                        SMIF_DEVICE_RD_DUMMY_CTL_PRESENT_Msk)
                                        : 0U;

        SMIF_DEVICE_RD_DATA_CTL(dev) = _VAL2FLD(SMIF_DEVICE_RD_DATA_CTL_WIDTH, (uint32_t)read->dataWidth);
    }

    if(NULL != prog)
    {
        SMIF_DEVICE_WR_CMD_CTL(dev) = (CY_SMIF_NO_COMMAND_OR_MODE != prog->command) ?
                                    (_VAL2FLD(SMIF_DEVICE_WR_CMD_CTL_CODE,  (uint32_t)prog->command) |
                                    _VAL2FLD(SMIF_DEVICE_WR_CMD_CTL_WIDTH, (uint32_t)prog->cmdWidth)|
                                    SMIF_DEVICE_WR_CMD_CTL_PRESENT_Msk)
                                    : 0U;

        SMIF_DEVICE_WR_ADDR_CTL(dev) = _VAL2FLD(SMIF_DEVICE_WR_ADDR_CTL_WIDTH, (uint32_t)prog->addrWidth);

        SMIF_DEVICE_WR_MODE_CTL(dev) = (CY_SMIF_NO_COMMAND_OR_MODE != prog->mode) ?
                                        (_VAL2FLD(SMIF_DEVICE_WR_CMD_CTL_CODE,  (uint32_t)prog->mode)     |
                                        _VAL2FLD(SMIF_DEVICE_WR_CMD_CTL_WIDTH, (uint32_t)prog->modeWidth)|
                                        SMIF_DEVICE_WR_CMD_CTL_PRESENT_Msk)
                                        : 0UL;

        SMIF_DEVICE_WR_DUMMY_CTL(dev) = (0UL != prog->dummyCycles) ?
                                        (_VAL2FLD(SMIF_DEVICE_WR_DUMMY_CTL_SIZE5, (prog->dummyCycles - 1UL)) |
                                        SMIF_DEVICE_WR_DUMMY_CTL_PRESENT_Msk)
                                        : 0U;

        SMIF_DEVICE_WR_DATA_CTL(dev) = _VAL2FLD(SMIF_DEVICE_WR_DATA_CTL_WIDTH, (uint32_t)prog->dataWidth);
    }
}


/*******************************************************************************
* Function Name: Cy_SMIF_MemDeInit
****************************************************************************//**
*
* This function de-initializes all slave slots of the memory device to their default
* values.
*
* \param base
* Holds the base address of the SMIF block registers.
*
*******************************************************************************/
void Cy_SMIF_MemDeInit(SMIF_Type *base)
{
    /* Configure the SMIF device slots to the default values. The default value is 0 */
    uint32_t deviceIndex;

    for(deviceIndex = 0UL; deviceIndex < (uint32_t)SMIF_DEVICE_NR; deviceIndex++)
    {
        SMIF_DEVICE_IDX_CTL(base, deviceIndex)           = 0U;
        SMIF_DEVICE_IDX_ADDR(base, deviceIndex)          = 0U;
        SMIF_DEVICE_IDX_MASK(base, deviceIndex)          = 0U;
        SMIF_DEVICE_IDX_ADDR_CTL(base, deviceIndex)      = 0U;
        SMIF_DEVICE_IDX_RD_CMD_CTL(base, deviceIndex)    = 0U;
        SMIF_DEVICE_IDX_RD_ADDR_CTL(base, deviceIndex)   = 0U;
        SMIF_DEVICE_IDX_RD_MODE_CTL(base, deviceIndex)   = 0U;
        SMIF_DEVICE_IDX_RD_DUMMY_CTL(base, deviceIndex)  = 0U;
        SMIF_DEVICE_IDX_RD_DATA_CTL(base, deviceIndex)   = 0U;
        SMIF_DEVICE_IDX_WR_CMD_CTL(base, deviceIndex)    = 0U;
        SMIF_DEVICE_IDX_WR_ADDR_CTL(base, deviceIndex)   = 0U;
        SMIF_DEVICE_IDX_WR_MODE_CTL(base, deviceIndex)   = 0U;
        SMIF_DEVICE_IDX_WR_DUMMY_CTL(base, deviceIndex)  = 0U;
        SMIF_DEVICE_IDX_WR_DATA_CTL(base, deviceIndex)   = 0U;
    }
}


/*******************************************************************************
* Function Name: Cy_SMIF_MemCmdWriteEnable
****************************************************************************//**
*
* This function sends the Write Enable command to the memory device.
*
* \note This function uses the low-level Cy_SMIF_TransmitCommand() API.
* The Cy_SMIF_TransmitCommand() API works in a blocking mode. In the dual quad mode,
* this API is called for each memory.
*
* \param base
* Holds the base address of the SMIF block registers.
*
* \param memDevice
* The device to which the command is sent.
*
* \param context
* This is the pointer to the context structure \ref cy_stc_smif_context_t
* allocated by the user. The structure is used during the SMIF
* operation for internal configuration and data retention. The user must not
* modify anything in this structure.
*
* \return A status of the command transmission.
*       - \ref CY_SMIF_SUCCESS
*       - \ref CY_SMIF_EXCEED_TIMEOUT
*       - \ref CY_SMIF_CMD_NOT_FOUND
*
*******************************************************************************/
cy_en_smif_status_t Cy_SMIF_MemCmdWriteEnable(SMIF_Type *base,
                                        cy_stc_smif_mem_config_t const *memDevice,
                                        cy_stc_smif_context_t const *context)
{
    /* The memory Write Enable */
    cy_stc_smif_mem_cmd_t* writeEn = memDevice->deviceCfg->writeEnCmd;
    
    cy_en_smif_status_t result = CY_SMIF_CMD_NOT_FOUND;
    
    if(NULL != writeEn)
    {
        result = Cy_SMIF_TransmitCommand( base, (uint8_t) writeEn->command,
                                        writeEn->cmdWidth,
                                        CY_SMIF_CMD_WITHOUT_PARAM,
                                        CY_SMIF_CMD_WITHOUT_PARAM,
                                        CY_SMIF_WIDTH_NA,
                                        memDevice->slaveSelect,
                                        CY_SMIF_TX_LAST_BYTE,
                                        context);
    }

    return result;
}


/*******************************************************************************
* Function Name: Cy_SMIF_MemCmdWriteDisable
****************************************************************************//**
*
* This function sends a Write Disable command to the memory device.
*
* \note This function uses the low-level Cy_SMIF_TransmitCommand() API.
* Cy_SMIF_TransmitCommand() API works in a blocking mode. In the dual quad mode
* this API should be called for each memory.
*
* \param base
* Holds the base address of the SMIF block registers.
*
* \param memDevice
* The device to which the command is sent.
*
* \param context
* This is the pointer to the context structure \ref cy_stc_smif_context_t
* allocated by the user. The structure is used during the SMIF
* operation for internal configuration and data retention. The user must not
* modify anything in this structure.
*
* \return A status of the command transmission.
*       - \ref CY_SMIF_SUCCESS
*       - \ref CY_SMIF_EXCEED_TIMEOUT
*       - \ref CY_SMIF_CMD_NOT_FOUND
*
*******************************************************************************/
cy_en_smif_status_t Cy_SMIF_MemCmdWriteDisable(SMIF_Type *base,
                                    cy_stc_smif_mem_config_t const *memDevice,
                                    cy_stc_smif_context_t const *context)
{
    cy_stc_smif_mem_cmd_t* writeDis = memDevice->deviceCfg->writeDisCmd;

    cy_en_smif_status_t result = CY_SMIF_CMD_NOT_FOUND;
    
    if(NULL != writeDis)
    {
        /* The memory write disable */
        result = Cy_SMIF_TransmitCommand( base, (uint8_t)writeDis->command,
                                          writeDis->cmdWidth,
                                          CY_SMIF_CMD_WITHOUT_PARAM,
                                          CY_SMIF_CMD_WITHOUT_PARAM,
                                          CY_SMIF_WIDTH_NA,
                                          memDevice->slaveSelect,
                                          CY_SMIF_TX_LAST_BYTE,
                                          context);
    }
    
    return result;
}


/*******************************************************************************
* Function Name: Cy_SMIF_MemIsBusy
****************************************************************************//**
*
* This function checks if the status of the memory device is busy.
* This is done by reading the status register and the corresponding bit
* (stsRegBusyMask). This function is a blocking function until the status
* register from the memory is read.
*
* \note In the dual quad mode, this API is called for each memory.
*
* \param base
* Holds the base address of the SMIF block registers.
*
* \param memDevice
*  The device to which the command is sent.
*
* \param context
* This is the pointer to the context structure \ref cy_stc_smif_context_t
* allocated by the user. The structure is used during the SMIF
* operation for internal configuration and data retention. The user must not
* modify anything in this structure.
*
* \return A status of the memory device.
*       - True - The device is busy or a timeout occurs.
*       - False - The device is not busy.
*
* \note Check \ref group_smif_usage_rules for any usage restriction 
*
*******************************************************************************/
bool Cy_SMIF_MemIsBusy(SMIF_Type *base, cy_stc_smif_mem_config_t const *memDevice,
                            cy_stc_smif_context_t const *context)
{
    uint8_t  status = 1U;
    cy_en_smif_status_t readStsResult = CY_SMIF_CMD_NOT_FOUND;
    cy_stc_smif_mem_device_cfg_t* device =  memDevice->deviceCfg;

    if(NULL != device->readStsRegWipCmd)
    {
        readStsResult = Cy_SMIF_MemCmdReadStatus(base, memDevice, &status,
                            (uint8_t)device->readStsRegWipCmd->command,
                            context);
    }  

    if (CY_SMIF_SUCCESS == readStsResult)
    {
        /* Masked not busy bits in returned status */
        status &= (uint8_t)device->stsRegBusyMask;
    }

    return (0U != status);
}


/*******************************************************************************
* Function Name: Cy_SMIF_MemQuadEnable
****************************************************************************//**
*
* This function enables the memory device for the quad mode of operation.
* This command must be executed before sending quad SPI commands to the
* memory device.
*
* \note In the dual quad mode, this API is called for each memory.
*
* \param base
* Holds the base address of the SMIF block registers.
*
* \param memDevice
* The device to which the command is sent.
*
* \param context
* This is the pointer to the context structure \ref cy_stc_smif_context_t
* allocated by the user. The structure is used during the SMIF
* operation for internal configuration and data retention. The user must not
* modify anything in this structure.
*
* \return A status of the command.
*   - \ref CY_SMIF_SUCCESS
*   - \ref CY_SMIF_NO_QE_BIT
*   - \ref CY_SMIF_CMD_FIFO_FULL
*   - \ref CY_SMIF_BAD_PARAM
*   - \ref CY_SMIF_CMD_NOT_FOUND
*
* \note Check \ref group_smif_usage_rules for any usage restriction 
*
*******************************************************************************/
cy_en_smif_status_t Cy_SMIF_MemQuadEnable(SMIF_Type *base,
                                    cy_stc_smif_mem_config_t const *memDevice,
                                    cy_stc_smif_context_t const *context)
{
    cy_en_smif_status_t result= CY_SMIF_CMD_NOT_FOUND;
    uint8_t statusReg[CY_SMIF_QE_BIT_STATUS_REG2_T1] = {0U};  
    cy_stc_smif_mem_device_cfg_t* device =  memDevice->deviceCfg;

    /* Check that command exists */
    if((NULL != device->readStsRegQeCmd)  &&
       (NULL != device->writeStsRegQeCmd) &&
       (NULL != device->readStsRegWipCmd))
    {
        uint8_t readQeCmd  = (uint8_t)device->readStsRegQeCmd->command;
        uint8_t writeQeCmd = (uint8_t)device->writeStsRegQeCmd->command;
        uint8_t readWipCmd = (uint8_t)device->readStsRegWipCmd->command;

        result = Cy_SMIF_MemCmdReadStatus(base, memDevice, &statusReg[0U],
                    readQeCmd, context);
        if (CY_SMIF_SUCCESS == result)
        {
            uint32_t qeMask = device->stsRegQuadEnableMask;

            switch(qeMask)
            {
                case CY_SMIF_SFDP_QE_BIT_6_OF_SR_1:
                    statusReg[0U] |= (uint8_t)qeMask;
                    result = Cy_SMIF_MemCmdWriteStatus(base, memDevice,
                                &statusReg[0U], writeQeCmd, context);
                    break;
                case CY_SMIF_SFDP_QE_BIT_1_OF_SR_2:
                    /* Read status register 1 with the assumption that WIP is always in
                    * status register 1 */
                    result = Cy_SMIF_MemCmdReadStatus(base, memDevice,
                                &statusReg[0U], readWipCmd, context);
                    if (CY_SMIF_SUCCESS == result)
                    {
                        result = Cy_SMIF_MemCmdReadStatus(base, memDevice,
                                    &statusReg[1U], readQeCmd, context);
                        if (CY_SMIF_SUCCESS == result)
                        {
                            statusReg[1U] |= (uint8_t)qeMask;
                            result = Cy_SMIF_MemCmdWriteStatus(base, memDevice,
                                        statusReg, writeQeCmd, context);
                        }
                    }
                    break;
                case CY_SMIF_SFDP_QE_BIT_7_OF_SR_2:
                    result = Cy_SMIF_MemCmdReadStatus(base, memDevice,
                                &statusReg[1U], readQeCmd, context);
                    if (CY_SMIF_SUCCESS == result)
                    {
                        statusReg[1U] |= (uint8_t)qeMask;
                        result = Cy_SMIF_MemCmdWriteStatus(base, memDevice,
                                    &statusReg[1U], writeQeCmd, context);
                    }
                    break;
                default:
                    result = CY_SMIF_NO_QE_BIT;
                    break;
            }
        }
    }
    return(result);
}


/*******************************************************************************
* Function Name: Cy_SMIF_MemCmdReadStatus
****************************************************************************//**
*
* This function reads the status register. This function is a blocking function,
* it will block the execution flow until the status register is read.
*
* \note This function uses the low-level Cy_SMIF_TransmitCommand() API.
* the Cy_SMIF_TransmitCommand() API works in a blocking mode. In the dual quad mode,
* this API is called for each memory.
*
* \param base
* Holds the base address of the SMIF block registers.
*
* \param memDevice
* The device to which the command is sent.
*
* \param status
* The status register value returned by the external memory.
*
* \param command
* The command required to read the status/configuration register.
*
* \param context
* This is the pointer to the context structure \ref cy_stc_smif_context_t
* allocated by the user. The structure is used during the SMIF
* operation for internal configuration and data retention. The user must not
* modify anything in this structure.
*
* \return A status of the command reception.
*       - \ref CY_SMIF_SUCCESS
*       - \ref CY_SMIF_CMD_FIFO_FULL
*       - \ref CY_SMIF_EXCEED_TIMEOUT
*       - \ref CY_SMIF_CMD_NOT_FOUND
*
* \note Check \ref group_smif_usage_rules for any usage restriction 
*
*******************************************************************************/
cy_en_smif_status_t Cy_SMIF_MemCmdReadStatus(SMIF_Type *base,
                                    cy_stc_smif_mem_config_t const *memDevice,
                                    uint8_t *status,
                                    uint8_t command,
                                    cy_stc_smif_context_t const *context)
{
    cy_en_smif_status_t result = CY_SMIF_CMD_NOT_FOUND;

    /* Read the memory status register */
    result = Cy_SMIF_TransmitCommand( base, command, CY_SMIF_WIDTH_SINGLE,
                CY_SMIF_CMD_WITHOUT_PARAM, CY_SMIF_CMD_WITHOUT_PARAM,
                CY_SMIF_WIDTH_NA, memDevice->slaveSelect, 
                CY_SMIF_TX_NOT_LAST_BYTE, context);

    if (CY_SMIF_SUCCESS == result)
    {
        result = Cy_SMIF_ReceiveDataBlocking( base, status,
                    CY_SMIF_READ_ONE_BYTE, CY_SMIF_WIDTH_SINGLE, context);
    }

    return(result);
}


/*******************************************************************************
* Function Name: Cy_SMIF_MemCmdWriteStatus
****************************************************************************//**
*
* This function writes the status register. This is a blocking function, it will
* block the execution flow until the command transmission is completed.
*
* \note This function uses the low-level Cy_SMIF_TransmitCommand() API.
* The Cy_SMIF_TransmitCommand() API works in a blocking mode. In the dual quad mode,
* this API is called for each memory.
*
* \param base
* Holds the base address of the SMIF block registers.
*
* \param memDevice
* The device to which the command is sent.
*
* \param status
* The status to write into the status register.
*
* \param command
* The command to write into the status/configuration register.
*
* \param context
* This is the pointer to the context structure \ref cy_stc_smif_context_t
* allocated by the user. The structure is used during the SMIF
* operation for internal configuration and data retention. The user must not
* modify anything in this structure.
*
* \return A status of the command transmission.
*       - \ref CY_SMIF_SUCCESS
*       - \ref CY_SMIF_EXCEED_TIMEOUT
*       - \ref CY_SMIF_CMD_NOT_FOUND
*
*******************************************************************************/
cy_en_smif_status_t Cy_SMIF_MemCmdWriteStatus(SMIF_Type *base,
                                    cy_stc_smif_mem_config_t const *memDevice,
                                    void const *status,
                                    uint8_t command,
                                    cy_stc_smif_context_t const *context)
{
    cy_en_smif_status_t result;

    /* The Write Enable */
    result =  Cy_SMIF_MemCmdWriteEnable(base, memDevice, context);

    /* The Write Status */
    if (CY_SMIF_SUCCESS == result)
    {
        uint32_t size;
        uint32_t qeMask = memDevice->deviceCfg->stsRegQuadEnableMask;

        size = ( CY_SMIF_SFDP_QE_BIT_1_OF_SR_2 == qeMask)? CY_SMIF_WRITE_TWO_BYTES:
                                                        CY_SMIF_WRITE_ONE_BYTE;
        result = Cy_SMIF_TransmitCommand( base, command, CY_SMIF_WIDTH_SINGLE,
                    (uint8_t const *)status, size, CY_SMIF_WIDTH_SINGLE,
                    memDevice->slaveSelect, CY_SMIF_TX_LAST_BYTE, context);
    }

    return(result);
}


/*******************************************************************************
* Function Name: Cy_SMIF_MemCmdChipErase
****************************************************************************//**
*
* This function performs a chip erase of the external memory. The Write Enable
* command is called before this API.
*
* \note This function uses the low-level Cy_SMIF_TransmitCommand() API.
* Cy_SMIF_TransmitCommand() API works in a blocking mode. In the dual quad mode,
* this API is called for each memory.
*
* \param base
* Holds the base address of the SMIF block registers.
*
* \param memDevice
* The device to which the command is sent
*
* \param context
* This is the pointer to the context structure \ref cy_stc_smif_context_t
* allocated by the user. The structure is used during the SMIF
* operation for internal configuration and data retention. The user must not
* modify anything in this structure.
*
* \return A status of the command transmission.
*       - \ref CY_SMIF_SUCCESS
*       - \ref CY_SMIF_EXCEED_TIMEOUT
*       - \ref CY_SMIF_CMD_NOT_FOUND
*
*******************************************************************************/
cy_en_smif_status_t Cy_SMIF_MemCmdChipErase(SMIF_Type *base,
                cy_stc_smif_mem_config_t const *memDevice,
                cy_stc_smif_context_t const *context)
{
    cy_en_smif_status_t result= CY_SMIF_CMD_NOT_FOUND;

    cy_stc_smif_mem_cmd_t *cmdErase = memDevice->deviceCfg->chipEraseCmd;
    if(NULL != cmdErase)
    {
        result = Cy_SMIF_TransmitCommand( base, (uint8_t)cmdErase->command,
                cmdErase->cmdWidth, CY_SMIF_CMD_WITHOUT_PARAM,
                CY_SMIF_CMD_WITHOUT_PARAM, CY_SMIF_WIDTH_NA,
                memDevice->slaveSelect, CY_SMIF_TX_LAST_BYTE, context);
    }

    return(result);
}


/*******************************************************************************
* Function Name: Cy_SMIF_MemCmdSectorErase
****************************************************************************//**
*
* This function performs a block Erase of the external memory. The Write Enable
* command is called before this API.
*
* \note This function uses the low-level Cy_SMIF_TransmitCommand() API.
* The Cy_SMIF_TransmitCommand() API works in a blocking mode. In the dual quad mode,
* this API is called for each memory.
*
* \param base
* Holds the base address of the SMIF block registers.
*
* \param memDevice
* The device to which the command is sent.
*
* \param sectorAddr
* The sector address to erase.
*
* \param context
* This is the pointer to the context structure \ref cy_stc_smif_context_t
* allocated by the user. The structure is used during the SMIF
* operation for internal configuration and data retention. The user must not
* modify anything in this structure.
*
* \return A status of the command transmission.
*       - \ref CY_SMIF_SUCCESS
*       - \ref CY_SMIF_BAD_PARAM
*       - \ref CY_SMIF_EXCEED_TIMEOUT
*       - \ref CY_SMIF_CMD_NOT_FOUND
*
*******************************************************************************/
cy_en_smif_status_t Cy_SMIF_MemCmdSectorErase(SMIF_Type *base,
                                        cy_stc_smif_mem_config_t const *memDevice,
                                        uint8_t const *sectorAddr,
                                        cy_stc_smif_context_t const *context)
{
    cy_en_smif_status_t result = CY_SMIF_BAD_PARAM;

    CY_ASSERT_L1(NULL != memDevice);

    if (NULL != sectorAddr)
    {
        cy_stc_smif_mem_device_cfg_t *device = memDevice->deviceCfg;
        cy_stc_smif_mem_cmd_t *cmdErase = device->eraseCmd;
        cy_stc_smif_hybrid_region_info_t* hybrInfo = NULL;

        result = Cy_SMIF_MemLocateHybridRegion(memDevice, &hybrInfo,
                ByteArrayToValue(sectorAddr, device->numOfAddrBytes));

        if ((NULL != cmdErase) && (CY_SMIF_WIDTH_NA != cmdErase->cmdWidth) && (result != CY_SMIF_BAD_PARAM))
        {
            uint8_t eraseCommand = (uint8_t)((result == CY_SMIF_SUCCESS) ? (hybrInfo->eraseCmd) : (cmdErase->command));
            result = Cy_SMIF_TransmitCommand( base, eraseCommand,
                    cmdErase->cmdWidth, sectorAddr, device->numOfAddrBytes,
                    cmdErase->cmdWidth, memDevice->slaveSelect,
                    CY_SMIF_TX_LAST_BYTE, context);
        }
        else
        {
            result = CY_SMIF_CMD_NOT_FOUND;
        }
    }
    return(result);
}


/*******************************************************************************
* Function Name: Cy_SMIF_MemCmdProgram
****************************************************************************//**
*
* This function performs the Program operation. 
*
* \note This function uses the  Cy_SMIF_TransmitCommand() API.
* The Cy_SMIF_TransmitCommand() API works in the blocking mode. In the dual quad mode,
* this API works with both types of memory simultaneously.
*
* \param base
* Holds the base address of the SMIF block registers.
*
* \param memDevice
* The device to which the command is sent.
*
* \param addr
* The address to program.
*
* \param writeBuff
* The pointer to the data to program. If this pointer is a NULL, then the
* function does not enable the interrupt. This use case is  typically used when
* the FIFO is handled outside the interrupt and is managed in either a 
* polling-based code or a DMA. The user would handle the FIFO management 
* in a DMA or a polling-based code. 
* If the user provides a NULL pointer in this function and does not handle 
* the FIFO transaction, this could either stall or timeout the operation 
* \ref Cy_SMIF_TransmitData().
*
* \param size
* The size of data to program. The user must ensure that the data size
* does not exceed the page size.
*
* \param cmdCompleteCb
* The callback function to call after the transfer completion. NULL interpreted
* as no callback.
*
* \param context
* This is the pointer to the context structure \ref cy_stc_smif_context_t
* allocated by the user. The structure is used during the SMIF
* operation for internal configuration and data retention. The user must not
* modify anything in this structure.
*
* \return A status of a transmission.
*       - \ref CY_SMIF_SUCCESS
*       - \ref CY_SMIF_CMD_FIFO_FULL
*       - \ref CY_SMIF_BAD_PARAM
*       - \ref CY_SMIF_EXCEED_TIMEOUT
*       - \ref CY_SMIF_CMD_NOT_FOUND
*
*******************************************************************************/
cy_en_smif_status_t Cy_SMIF_MemCmdProgram(SMIF_Type *base,
                                    cy_stc_smif_mem_config_t const *memDevice,
                                    uint8_t const *addr,
                                    uint8_t const *writeBuff,
                                    uint32_t size,
                                    cy_smif_event_cb_t cmdCompleteCb,
                                    cy_stc_smif_context_t *context)
{
    cy_en_smif_status_t result = CY_SMIF_SUCCESS;
    cy_en_smif_slave_select_t slaveSelected;

    cy_stc_smif_mem_device_cfg_t *device = memDevice->deviceCfg;
    cy_stc_smif_mem_cmd_t *cmdProg = device->programCmd;
    
    if(NULL == cmdProg)
    {
        result = CY_SMIF_CMD_NOT_FOUND;
    }
    else if ((NULL == addr) || (size > device->programSize))
    {
        result = CY_SMIF_BAD_PARAM;
    }
    else
    {
        slaveSelected = (0U == memDevice->dualQuadSlots)?  memDevice->slaveSelect :
                                                        (cy_en_smif_slave_select_t)memDevice->dualQuadSlots;
                                                      
        /* The page program command */
        result = Cy_SMIF_TransmitCommand( base, (uint8_t)cmdProg->command,
                cmdProg->cmdWidth, addr, device->numOfAddrBytes,
                cmdProg->addrWidth, slaveSelected, CY_SMIF_TX_NOT_LAST_BYTE,
                context);

        if((CY_SMIF_SUCCESS == result) && (CY_SMIF_NO_COMMAND_OR_MODE != cmdProg->mode))
        {
            result = Cy_SMIF_TransmitCommand(base, (uint8_t)cmdProg->mode,
                        cmdProg->modeWidth, CY_SMIF_CMD_WITHOUT_PARAM,
                        CY_SMIF_CMD_WITHOUT_PARAM, CY_SMIF_WIDTH_NA,
                        (cy_en_smif_slave_select_t)slaveSelected,
                        CY_SMIF_TX_NOT_LAST_BYTE, context);
        }

        if((CY_SMIF_SUCCESS == result) && (cmdProg->dummyCycles > 0U))
        {
            result = Cy_SMIF_SendDummyCycles(base, cmdProg->dummyCycles);
        }

        if(CY_SMIF_SUCCESS == result)
        {
            result = Cy_SMIF_TransmitData( base, writeBuff, size,
                    cmdProg->dataWidth, cmdCompleteCb, context);
        }
    }

    return(result);
}


/*******************************************************************************
* Function Name: Cy_SMIF_MemCmdRead
****************************************************************************//**
*
* This function performs the Read operation.
*
* \note This function uses the Cy_SMIF_TransmitCommand() API.
* The Cy_SMIF_TransmitCommand() API works in the blocking mode. In the dual quad mode,
* this API works with both types of memory simultaneously.
*
* \param base
* Holds the base address of the SMIF block registers.
*
* \param memDevice
* The device to which the command is sent.
*
* \param addr
* The address to read.
*
* \param readBuff
* The pointer to the variable where the read data is stored. If this pointer is 
* a NULL, then the function does not enable the interrupt. This use case is 
* typically used when the FIFO is handled outside the interrupt and is managed
* in either a  polling-based code or a DMA. The user would handle the FIFO
* management in a DMA or a polling-based code. 
* If the user provides a NULL pointer in this function and does not handle 
* the FIFO transaction, this could either stall or timeout the operation 
* \ref Cy_SMIF_TransmitData().
*
* \param size
* The size of data to read.
*
* \param cmdCompleteCb
* The callback function to call after the transfer completion. NULL interpreted
* as no callback.
*
* \param context
* This is the pointer to the context structure \ref cy_stc_smif_context_t
* allocated by the user. The structure is used during the SMIF
* operation for internal configuration and data retention. The user must not
* modify anything in this structure.
*
* \return A status of the transmission.
*       - \ref CY_SMIF_SUCCESS
*       - \ref CY_SMIF_CMD_FIFO_FULL
*       - \ref CY_SMIF_BAD_PARAM
*       - \ref CY_SMIF_EXCEED_TIMEOUT
*       - \ref CY_SMIF_CMD_NOT_FOUND
*
* \note Check \ref group_smif_usage_rules for any usage restriction 
*
*******************************************************************************/
cy_en_smif_status_t Cy_SMIF_MemCmdRead(SMIF_Type *base,
                                cy_stc_smif_mem_config_t const *memDevice,
                                uint8_t const *addr,
                                uint8_t* readBuff,
                                uint32_t size,
                                cy_smif_event_cb_t cmdCompleteCb,
                                cy_stc_smif_context_t *context)
{
    cy_en_smif_status_t result = CY_SMIF_BAD_PARAM;
    cy_en_smif_slave_select_t slaveSelected;
    cy_stc_smif_mem_device_cfg_t *device = memDevice->deviceCfg;
    cy_stc_smif_mem_cmd_t *cmdRead = device->readCmd;

    if(NULL == cmdRead)
    {
        result = CY_SMIF_CMD_NOT_FOUND;
    }
    else if(NULL == addr)
    {
        result = CY_SMIF_BAD_PARAM;
    }
    else
    {
        slaveSelected = (0U == memDevice->dualQuadSlots)?  memDevice->slaveSelect :
                               (cy_en_smif_slave_select_t)memDevice->dualQuadSlots;
                               
        result = Cy_SMIF_TransmitCommand( base, (uint8_t)cmdRead->command,
                    cmdRead->cmdWidth, addr, device->numOfAddrBytes,
                    cmdRead->addrWidth, slaveSelected, CY_SMIF_TX_NOT_LAST_BYTE,
                    context);

        if((CY_SMIF_SUCCESS == result) && (CY_SMIF_NO_COMMAND_OR_MODE != cmdRead->mode))
        {
            result = Cy_SMIF_TransmitCommand(base, (uint8_t)cmdRead->mode,
                        cmdRead->modeWidth, CY_SMIF_CMD_WITHOUT_PARAM,
                        CY_SMIF_CMD_WITHOUT_PARAM, CY_SMIF_WIDTH_NA,
                        (cy_en_smif_slave_select_t)slaveSelected,
                        CY_SMIF_TX_NOT_LAST_BYTE, context);
        }

        if((CY_SMIF_SUCCESS == result) && (0U < cmdRead->dummyCycles))
        {
            result = Cy_SMIF_SendDummyCycles(base, cmdRead->dummyCycles);
        }

        if(CY_SMIF_SUCCESS == result)
        {
            result = Cy_SMIF_ReceiveData(base, readBuff, size,
                        cmdRead->dataWidth, cmdCompleteCb, context);
        }
    }

    return(result);
}


/*******************************************************************************
* Function Name: Cy_SMIF_MemLocateHybridRegion
****************************************************************************//**
*
* This function locates the region structure by the address which belongs to it.
*
* \note This function is valid for the memories with hybrid sectors.
*
* \param memDevice
* The memory device configuration.
*
* \param regionInfo
* Places a hybrid region configuration structure that contains the region 
* specific parameters. See \ref cy_stc_smif_hybrid_region_info_t for
* reference.
*
* \param address
* The address for which a region is searched.
*
* \return A status of the region location.
*       - \ref CY_SMIF_SUCCESS
*       - \ref CY_SMIF_NOT_HYBRID_MEM
*       - \ref CY_SMIF_BAD_PARAM
*
* \funcusage 
* \snippet smif/snippet/main.c snippet_Cy_SMIF_MemLocateHybridRegion
*
*******************************************************************************/
cy_en_smif_status_t Cy_SMIF_MemLocateHybridRegion(cy_stc_smif_mem_config_t const *memDevice,
                                               cy_stc_smif_hybrid_region_info_t** regionInfo,
                                               uint32_t address)
{
    cy_en_smif_status_t result = CY_SMIF_BAD_PARAM;
    cy_stc_smif_hybrid_region_info_t* currInfo = NULL;
    CY_ASSERT_L1(NULL != memDevice);
    cy_stc_smif_mem_device_cfg_t *device = memDevice->deviceCfg;

    /* Check if the address exceeds the memory size */
    if(address <= device->memSize)
    {
        result = CY_SMIF_NOT_HYBRID_MEM;
        /* Check if the memory is hybrid */
        if(NULL != device->hybridRegionInfo)
        {
            uint32_t idx;
            uint32_t regionStartAddr;
            uint32_t regionEndAddr;
            for(idx = 0UL; idx < device->hybridRegionCount; idx++)
            {
                currInfo = device->hybridRegionInfo[idx];
                regionStartAddr = currInfo->regionAddress;
                regionEndAddr = regionStartAddr + (currInfo->sectorsCount * currInfo->eraseSize);
                if ((address >= regionStartAddr) && (address < regionEndAddr))
                {
                    *regionInfo = currInfo;
                    result = CY_SMIF_SUCCESS;
                    break;
                }
            }
        }
    }

    return result;
}


/*******************************************************************************
* Function Name: SfdpReadBuffer
****************************************************************************//**
*
* This function reads the tables in the SDFP database into the buffer.
*
* \note This function is a blocking function and blocks until the structure data
* is read and returned. This function uses \ref Cy_SMIF_TransmitCommand()
*
* \param *base
* Holds the base address of the SMIF block registers.
*
* \param *cmdSfdp
*  The command structure to store the Read/Write command
*  configuration.
*
* \param sfdpAddress
* The pointer to an array with the address bytes
* associated with the memory command.
*
* \param slaveSelect
* Denotes the number of the slave device to which the transfer is made.
* (0, 1, 2 or 4 - the bit defines which slave to enable). The two-bit enable
* is possible only for the double quad SPI mode.
*
* \param size
* The size of data to be received. Must be > 0 and not greater than 65536.
*
* \param sfdpBuffer
* The pointer to an array with the SDFP buffer.
*
* \param context
* This is the pointer to the context structure \ref cy_stc_smif_context_t
* allocated by the user. The structure is used during the SMIF
* operation for internal configuration and data retention. The user must not
* modify anything in this structure.
*
* \return A status of the transmission.
*       - \ref CY_SMIF_SUCCESS
*       - \ref CY_SMIF_CMD_FIFO_FULL
*       - \ref CY_SMIF_NO_SFDP_SUPPORT
*       - \ref CY_SMIF_EXCEED_TIMEOUT
*
*******************************************************************************/
static cy_en_smif_status_t SfdpReadBuffer(SMIF_Type *base, 
                                         cy_stc_smif_mem_cmd_t const *cmdSfdp,
                                         uint8_t const sfdpAddress[],
                                         cy_en_smif_slave_select_t  slaveSelect,
                                         uint32_t size,
                                         uint8_t sfdpBuffer[],
                                         cy_stc_smif_context_t *context)
{
    cy_en_smif_status_t result = CY_SMIF_NO_SFDP_SUPPORT;

    result = Cy_SMIF_TransmitCommand( base, (uint8_t)cmdSfdp->command,
                cmdSfdp->cmdWidth, sfdpAddress, CY_SMIF_SFDP_ADDRESS_LENGTH,
                cmdSfdp->addrWidth, slaveSelect, CY_SMIF_TX_NOT_LAST_BYTE,
                context);

    if(CY_SMIF_SUCCESS == result)
    {
        result = Cy_SMIF_SendDummyCycles(base, cmdSfdp->dummyCycles);
        
        /* Get data from SFDP and 1st Basic Flash Parameter Headers only */
        if(CY_SMIF_SUCCESS == result)
        {
            result = Cy_SMIF_ReceiveData( base, sfdpBuffer, size,
                                         cmdSfdp->dataWidth, NULL, context);
                                         
            if (CY_SMIF_SUCCESS == result)
            {
                uint32_t cmdTimeout = context->timeout;
                while (((uint32_t) CY_SMIF_RX_COMPLETE != context->transferStatus) &&
                        (CY_SMIF_EXCEED_TIMEOUT != result))
                {
                    /* Wait until the Read of the SFDP operation is completed */
                    result = Cy_SMIF_TimeoutRun(&cmdTimeout);
                }
            }
        }   
    }

    return(result);
}


/*******************************************************************************
* Function Name: SfdpFindParameterHeader
****************************************************************************//**
*
* Finds the Parameter Header offset from the JEDEC basic flash parameter table. 
*
* \param id
* The parameter ID.
*
* \param sfdpBuffer
* The pointer to an array with the SDFP buffer.
*
* \return The relative parameter header offset in bytes.
*  Returns 0 when the parameter header is not found.
*
*******************************************************************************/
static uint32_t SfdpFindParameterHeader(uint32_t id, uint8_t const sfdpBuffer[])
{
    uint32_t headerOffset = PARAMETER_IS_NOT_FOUND;
    uint32_t maxMinorRevison = 0UL;
    uint32_t sfdpAddress = FIRST_HEADER_OFFSET; /* Begin from 1st Parameter Header */
    
    while (sfdpAddress <= (((uint32_t)sfdpBuffer[PARAM_HEADERS_NUM] * 
                                        HEADER_LENGTH) +
                                        FIRST_HEADER_OFFSET))
    {
        /* Check parameter ID */
        if (((id & PARAM_ID_LSB_MASK) == sfdpBuffer[sfdpAddress]) &&  /* Parameter ID LSB */ 
            (((id >> PARAM_ID_MSB_OFFSET) & PARAM_ID_LSB_MASK) == 
                    sfdpBuffer[sfdpAddress +  /* Parameter ID MSB */
                    PARAM_ID_MSB_REL_OFFSET]))
        {       
            /* Check parameter major and minor revisions */        
            if ((sfdpBuffer[sfdpAddress + PARAM_MINOR_REV_REL_OFFSET] >= maxMinorRevison) &&
                (sfdpBuffer[sfdpAddress + PARAM_MAJOR_REV_REL_OFFSET] == CY_SMIF_SFDP_MAJOR_REV_1))
            {
                /* Get the maximum minor revision */   
                maxMinorRevison = sfdpBuffer[sfdpAddress + PARAM_MINOR_REV_REL_OFFSET];
                
                /* Save the the Parameter Header offset with the maximum minor revision */
                headerOffset = sfdpAddress;
            } 
        }
    
        sfdpAddress += HEADER_LENGTH;       
    }

    return(headerOffset);
}


/*******************************************************************************
* Function Name: SfdpFindParameterTableAddress
****************************************************************************//**
*
* Reads the address and length of the Parameter Table from 
* the JEDEC basic flash parameter table. 
*
* \param id
* The parameter ID.
*
* \param sfdpBuffer
* The pointer to an array with the SDFP buffer.
*
* \param address
* The Parameter Table address.
*
* \param *tableLength
* The Parameter Table length.
*
* \return The command reception status.
*       - \ref CY_SMIF_SUCCESS
*       - \ref CY_SMIF_CMD_NOT_FOUND
*
*******************************************************************************/
static cy_en_smif_status_t SfdpFindParameterTableAddress(uint32_t id,
                                                    uint8_t const sfdpBuffer[],
                                                    uint8_t address[],
                                                    uint32_t *tableLength)
{
    cy_en_smif_status_t result = CY_SMIF_CMD_NOT_FOUND;
    uint32_t headerOffset;
    
    headerOffset = SfdpFindParameterHeader(id, sfdpBuffer);

    if (PARAMETER_IS_NOT_FOUND != headerOffset)
    {
        /* The Parameter Table address */
        address[2] = sfdpBuffer[headerOffset +
                                PARAM_TABLE_PRT_OFFSET];
        address[1] = sfdpBuffer[headerOffset +
                                PARAM_TABLE_PRT_OFFSET + 1UL];
        address[0] = sfdpBuffer[headerOffset +
                                PARAM_TABLE_PRT_OFFSET + 2UL];

        /* The Parameter Table length */
        *tableLength = (uint32_t)sfdpBuffer[headerOffset + PARAM_TABLE_LENGTH_OFFSET] * 
                       BYTES_IN_DWORD;
                       
        result = CY_SMIF_SUCCESS;              
    }
 
    return(result);
}


/*******************************************************************************
* Function Name: SfdpGetNumOfAddrBytes
****************************************************************************//**
*
* Reads the number of address bytes from the JEDEC basic flash parameter table.
*
* \note 4-byte addressing mode is set when the memory device supports 
*       3- or 4-byte addressing mode.
*
* \param sfdpBuffer
* The pointer to an array with the SDFP buffer.
*
* \return The number of address bytes used by the memory slave device.
*
*******************************************************************************/
static uint32_t SfdpGetNumOfAddrBytes(uint8_t const sfdpBuffer[])
{
    uint32_t addrBytesNum = 0UL;
    uint32_t sfdpAddrCode = _FLD2VAL(CY_SMIF_SFDP_ADDRESS_BYTES,
                                        (uint32_t)sfdpBuffer
                                        [CY_SMIF_SFDP_BFPT_BYTE_02]);
    switch(sfdpAddrCode)
    {
        case CY_SMIF_SFDP_THREE_BYTES_ADDR_CODE:
            addrBytesNum = CY_SMIF_THREE_BYTES_ADDR;
            break;
        case CY_SMIF_SFDP_THREE_OR_FOUR_BYTES_ADDR_CODE:
            addrBytesNum = CY_SMIF_FOUR_BYTES_ADDR;
            break;
        case CY_SMIF_SFDP_FOUR_BYTES_ADDR_CODE:
            addrBytesNum = CY_SMIF_FOUR_BYTES_ADDR;
            break;
        default:
            break;
    }
            
    return(addrBytesNum);
}


/*******************************************************************************
* Function Name: SfdpGetMemoryDensity
****************************************************************************//**
*
* Reads the Memory Density from the JEDEC basic flash parameter table. 
*
* \param sfdpBuffer
* The pointer to an array with the SDFP buffer.
*
* \return The external memory size:
* For densities of 2 gigabits or less - the size in bytes;
* For densities 4 gigabits and above - bit-31 is set to 1b to define that
* this memory is 4 gigabits and above; and other 30:0 bits define N where 
* the density is computed as 2^N bytes. 
* For example, 0x80000021 corresponds to 2^30 = 1 gigabyte. 
*
*******************************************************************************/
static uint32_t SfdpGetMemoryDensity(uint8_t const sfdpBuffer[])
{
    uint32_t memorySize;
    uint32_t locSize = Cy_SMIF_PackBytesArray(&sfdpBuffer[CY_SMIF_SFDP_BFPT_BYTE_04], true);

    if (0UL == (locSize & CY_SMIF_SFDP_SIZE_ABOVE_4GB_Msk))
    {
        memorySize = (locSize + 1UL)/BITS_IN_BYTE;
    }
    else
    {
        memorySize = (locSize - BITS_IN_BYTE_ABOVE_4GB) |
                CY_SMIF_SFDP_SIZE_ABOVE_4GB_Msk;
    }
                
    return(memorySize);
}


/*******************************************************************************
* Function Name: SfdpGetReadCmd_1_4_4
****************************************************************************//**
*
* Reads the FAST_READ_1_4_4 read command parameters from the JEDEC basic flash 
* parameter table. 
*
* \param sfdpBuffer
* The pointer to an array with the SDFP buffer.
*
* \param cmdRead
* The pointer to the read command parameters structure.
*
*******************************************************************************/
static void SfdpGetReadCmd_1_4_4(uint8_t const sfdpBuffer[], 
                             cy_stc_smif_mem_cmd_t* cmdRead)
{
    /* 8-bit command. 4 x I/O Read command */
    cmdRead->command = sfdpBuffer[CY_SMIF_SFDP_BFPT_BYTE_09];

    /* The command transfer width */
    cmdRead->cmdWidth = CY_SMIF_WIDTH_SINGLE;

    /* The address transfer width */
    cmdRead->addrWidth = CY_SMIF_WIDTH_QUAD;

    /* The 8-bit mode byte. This value is 0xFFFFFFFF when there is no mode present */
    if (0U == (_FLD2VAL(CY_SMIF_SFDP_1_4_4_MODE_CYCLES,
               (uint32_t) sfdpBuffer[CY_SMIF_SFDP_BFPT_BYTE_08])))
    {
        cmdRead->mode = CY_SMIF_NO_COMMAND_OR_MODE;
    }
    else
    {
        cmdRead->mode = READ_ENHANCED_MODE_DISABLED;
        cmdRead->modeWidth = CY_SMIF_WIDTH_QUAD;
    }

    /* The dummy cycles number. A zero value suggests no dummy cycles */
    cmdRead->dummyCycles = _FLD2VAL(CY_SMIF_SFDP_1_4_4_DUMMY_CYCLES, 
                           (uint32_t) sfdpBuffer[CY_SMIF_SFDP_BFPT_BYTE_08]);

    /* The data transfer width */
    cmdRead->dataWidth = CY_SMIF_WIDTH_QUAD;    
}


/*******************************************************************************
* Function Name: SfdpGetReadCmd_1_1_4
****************************************************************************//**
*
* Reads the FAST_READ_1_1_4 read command parameters from the JEDEC basic flash 
* parameter table. 
*
* \param sfdpBuffer
* The pointer to an array with the SDFP buffer.
*
* \param cmdRead
* The pointer to the read command parameters structure.
*
*******************************************************************************/
static void SfdpGetReadCmd_1_1_4(uint8_t const sfdpBuffer[], 
                             cy_stc_smif_mem_cmd_t* cmdRead)
{
    /* 8-bit command. 4 x I/O Read command */
    cmdRead->command = sfdpBuffer[CY_SMIF_SFDP_BFPT_BYTE_0B];

    /* The command transfer width */
    cmdRead->cmdWidth = CY_SMIF_WIDTH_SINGLE;

    /* The address transfer width */
    cmdRead->addrWidth = CY_SMIF_WIDTH_SINGLE;

    /* The 8-bit mode byte. This value is 0xFFFFFFFF when there is no mode present */
    if ((0U == _FLD2VAL(CY_SMIF_SFDP_1_1_4_MODE_CYCLES, (uint32_t) sfdpBuffer
                    [CY_SMIF_SFDP_BFPT_BYTE_0A])))
    {
        cmdRead->mode = CY_SMIF_NO_COMMAND_OR_MODE;
    }
    else
    {
        cmdRead->mode = READ_ENHANCED_MODE_DISABLED;
        cmdRead->modeWidth = CY_SMIF_WIDTH_SINGLE;
    }

    /* The dummy cycles number. A zero value suggests no dummy cycles */
    cmdRead->dummyCycles = _FLD2VAL(CY_SMIF_SFDP_1_1_4_DUMMY_CYCLES,
                          (uint32_t)sfdpBuffer[CY_SMIF_SFDP_BFPT_BYTE_0A]);

    /* The data transfer width */
    cmdRead->dataWidth = CY_SMIF_WIDTH_QUAD;  
}


/*******************************************************************************
* Function Name: SfdpGetReadCmd_1_2_2
****************************************************************************//**
*
* Reads the FAST_READ_1_2_2 read command parameters from the JEDEC basic flash 
* parameter table. 
*
* \param sfdpBuffer
* The pointer to an array with the SDFP buffer.
*
* \param cmdRead
* The pointer to the read command parameters structure.
*
*******************************************************************************/
static void SfdpGetReadCmd_1_2_2(uint8_t const sfdpBuffer[], 
                             cy_stc_smif_mem_cmd_t* cmdRead)
{
    /* 8-bit command. 2 x I/O Read command */
    cmdRead->command  = sfdpBuffer[CY_SMIF_SFDP_BFPT_BYTE_0F];

    /* The command transfer width */
    cmdRead->cmdWidth = CY_SMIF_WIDTH_SINGLE;

    /* The address transfer width */
    cmdRead->addrWidth = CY_SMIF_WIDTH_DUAL;

    /* The 8-bit mode byte. This value is 0xFFFFFFFF when there is no mode present */
    if (0U == _FLD2VAL(CY_SMIF_SFDP_1_2_2_MODE_CYCLES, (uint32_t)
              sfdpBuffer[CY_SMIF_SFDP_BFPT_BYTE_0E]))
    {
        cmdRead->mode = CY_SMIF_NO_COMMAND_OR_MODE;
    }
    else
    {
        cmdRead->mode = READ_ENHANCED_MODE_DISABLED;
        cmdRead->modeWidth = CY_SMIF_WIDTH_DUAL;
    }

    /* The dummy cycles number. A zero value suggests no dummy cycles. */
    cmdRead->dummyCycles = _FLD2VAL(CY_SMIF_SFDP_1_2_2_DUMMY_CYCLES,
                            (uint32_t) sfdpBuffer [CY_SMIF_SFDP_BFPT_BYTE_0E]);

    /* The data transfer width */
    cmdRead->dataWidth = CY_SMIF_WIDTH_DUAL;
}


/*******************************************************************************
* Function Name: SfdpGetReadCmd_1_1_2
****************************************************************************//**
*
* Reads the FAST_READ_1_1_2 read command parameters from the JEDEC basic flash 
* parameter table. 
*
* \param sfdpBuffer
* The pointer to an array with the SDFP buffer.
*
* \param cmdRead
* The pointer to the read command parameters structure.
*
*******************************************************************************/
static void SfdpGetReadCmd_1_1_2(uint8_t const sfdpBuffer[], 
                             cy_stc_smif_mem_cmd_t* cmdRead)
{
    /* 8-bit command. 2 x I/O Read command */
    cmdRead->command  = sfdpBuffer[CY_SMIF_SFDP_BFPT_BYTE_0D];

    /* The command transfer width */
    cmdRead->cmdWidth = CY_SMIF_WIDTH_SINGLE;

    /* The address transfer width */
    cmdRead->addrWidth = CY_SMIF_WIDTH_SINGLE;

    /* The 8-bit mode byte. This value is 0xFFFFFFFF when there is no mode present */
    if (0U == (_FLD2VAL(CY_SMIF_SFDP_1_1_2_MODE_CYCLES, (uint32_t)
        sfdpBuffer[CY_SMIF_SFDP_BFPT_BYTE_0C])))
    {
        cmdRead->mode = CY_SMIF_NO_COMMAND_OR_MODE;
    }
    else
    {
        cmdRead->mode = READ_ENHANCED_MODE_DISABLED;
        cmdRead->modeWidth = CY_SMIF_WIDTH_SINGLE;
    }

    /* The dummy cycles number. A zero value suggests no dummy cycles. */
    cmdRead->dummyCycles = _FLD2VAL(CY_SMIF_SFDP_1_1_2_DUMMY_CYCLES,
            (uint32_t)sfdpBuffer[CY_SMIF_SFDP_BFPT_BYTE_0C]);

    /* The data transfer width */
    cmdRead->dataWidth = CY_SMIF_WIDTH_DUAL;
}


/*******************************************************************************
* Function Name: SfdpGetReadCmd_1_1_1
****************************************************************************//**
*
* Reads the FAST_READ_1_1_1 read command parameters from the JEDEC basic flash 
* parameter table. 
*
* \param sfdpBuffer
* The pointer to an array with the SDFP buffer.
*
* \param cmdRead
* The pointer to the read command parameters structure.
*
*******************************************************************************/
static void SfdpGetReadCmd_1_1_1(uint8_t const sfdpBuffer[], 
                             cy_stc_smif_mem_cmd_t* cmdRead)
{
    (void)sfdpBuffer; /* Suppress warning */
    
    /* 8-bit command. 1 x I/O Read command */
    cmdRead->command  = CY_SMIF_SINGLE_READ_CMD;

    /* The command transfer width */
    cmdRead->cmdWidth = CY_SMIF_WIDTH_SINGLE;

    /* The address transfer width */
    cmdRead->addrWidth = CY_SMIF_WIDTH_SINGLE;

    /* The 8 bit-mode byte. This value is 0xFFFFFFFF when there is no mode present */
    cmdRead->mode = CY_SMIF_NO_COMMAND_OR_MODE;

    /* The dummy cycles number. A zero value suggests no dummy cycles. */
    cmdRead->dummyCycles = 0UL;

    /* The data transfer width */
    cmdRead->dataWidth = CY_SMIF_WIDTH_SINGLE;
}


/*******************************************************************************
* Function Name: SfdpGetReadCmdParams
****************************************************************************//**
*
* Reads the read command parameters from the JEDEC basic flash parameter table. 
*
* \param sfdpBuffer
* The pointer to an array with the SDFP buffer.
*
* \param dataSelect
* The data line selection options for a slave device.
*
* \param cmdRead
* The pointer to the read command parameters structure.
*
* \return Protocol Mode.
*
*******************************************************************************/
static cy_en_smif_protocol_mode_t SfdpGetReadCmdParams(uint8_t const sfdpBuffer[], 
                                     cy_en_smif_data_select_t dataSelect, 
                                      cy_stc_smif_mem_cmd_t* cmdRead)
{
    cy_en_smif_protocol_mode_t protocolMode = PROTOCOL_MODE_WRONG;
    uint32_t sfdpDataIndex = CY_SMIF_SFDP_BFPT_BYTE_02;
    bool quadEnabled = ((CY_SMIF_DATA_SEL1 != dataSelect) &&
                        (CY_SMIF_DATA_SEL3 != dataSelect));

    if (quadEnabled)
    {
        if (_FLD2BOOL(CY_SMIF_SFDP_FAST_READ_1_4_4,
                      ((uint32_t) sfdpBuffer[sfdpDataIndex])))
        {
            SfdpGetReadCmd_1_4_4(sfdpBuffer, cmdRead);
            protocolMode = PROTOCOL_MODE_1S_4S_4S;
        }
        else if (_FLD2BOOL(CY_SMIF_SFDP_FAST_READ_1_1_4,
                            ((uint32_t)sfdpBuffer[sfdpDataIndex])))
        {
            SfdpGetReadCmd_1_1_4(sfdpBuffer, cmdRead);
            protocolMode = PROTOCOL_MODE_1S_1S_4S;
        }
        else
        {
            /* Wrong mode */
            CY_ASSERT_L2(true);
        }
    }
    else
    {
        if ((_FLD2BOOL(CY_SMIF_SFDP_FAST_READ_1_2_2,
                       (uint32_t)sfdpBuffer[sfdpDataIndex])))
        {
            SfdpGetReadCmd_1_2_2(sfdpBuffer, cmdRead);
            protocolMode = PROTOCOL_MODE_1S_2S_2S;
        }
        else
        {
            if (_FLD2BOOL(CY_SMIF_SFDP_FAST_READ_1_1_2,
                          (uint32_t)sfdpBuffer[sfdpDataIndex]))
            {
                SfdpGetReadCmd_1_1_2(sfdpBuffer, cmdRead);
                protocolMode = PROTOCOL_MODE_1S_1S_2S;
            }
            else
            {
                SfdpGetReadCmd_1_1_1(sfdpBuffer, cmdRead);
                protocolMode = PROTOCOL_MODE_1S_1S_1S;
            }
        }
    }
    
    return protocolMode;
}


/*******************************************************************************
* Function Name: SfdpGetReadFourBytesCmd
****************************************************************************//**
*
* Reads the read command instruction from the 4-byte Address Instruction Table.  
*
* \param sfdpBuffer
* The pointer to an array with the SDFP buffer with the 4-byte Address
* Instruction Table.
*
* \param protocolMode
* Protocol Mode.
*
* \param cmdRead
* The pointer to the read command parameters structure.
*
*******************************************************************************/
static void SfdpGetReadFourBytesCmd(uint8_t const sfdpBuffer[], 
                                      cy_en_smif_protocol_mode_t protocolMode, 
                                      cy_stc_smif_mem_cmd_t* cmdRead)
{
    /* Get the mask which contains the Support for Fast Read Commands
     * from 4-byte Address Instruction Table, DWORD 1 
     */
    uint32_t sfdpForBytesTableDword1 = ((uint32_t*)sfdpBuffer)[FOUR_BYTE_ADDRESS_TABLE_BYTE_0];
 
    switch (protocolMode)
    {
        case PROTOCOL_MODE_1S_4S_4S:
            if (_FLD2BOOL(SUPPORT_FAST_READ_1S_4S_4S_CMD, sfdpForBytesTableDword1))
            {
                cmdRead->command = CY_SMIF_FAST_READ_4_BYTES_CMD_1S_4S_4S;
            }
            break;
        case PROTOCOL_MODE_1S_1S_4S:
            if (_FLD2BOOL(SUPPORT_FAST_READ_1S_1S_4S_CMD, sfdpForBytesTableDword1))
            {
                cmdRead->command = CY_SMIF_FAST_READ_4_BYTES_CMD_1S_1S_4S;
            }
            break;
        case PROTOCOL_MODE_1S_2S_2S:
            if (_FLD2BOOL(SUPPORT_FAST_READ_1S_2S_2S_CMD, sfdpForBytesTableDword1))
            {
                cmdRead->command = CY_SMIF_FAST_READ_4_BYTES_CMD_1S_2S_2S;
            }
            break;
        case PROTOCOL_MODE_1S_1S_2S:
            if (_FLD2BOOL(SUPPORT_FAST_READ_1S_1S_2S_CMD, sfdpForBytesTableDword1))
            {
                cmdRead->command = CY_SMIF_FAST_READ_4_BYTES_CMD_1S_1S_2S;
            }
            break;
        case PROTOCOL_MODE_1S_1S_1S:
            if (_FLD2BOOL(SUPPORT_FAST_READ_1S_1S_1S_CMD, sfdpForBytesTableDword1))
            {
                cmdRead->command = CY_SMIF_FAST_READ_4_BYTES_CMD_1S_1S_1S;
            }
            break;
        default:
            /* There are no instructions for 4-byte mode. Use instruction for 3-byte mode */
            break;
    }  
}


/*******************************************************************************
* Function Name: SfdpGetPageSize
****************************************************************************//**
*
* Reads the page size from the JEDEC basic flash parameter table. 
*
* \param sfdpBuffer
* The pointer to an array with the SDFP buffer.
*
* \return The page size in bytes.
*
*******************************************************************************/
static uint32_t SfdpGetPageSize(uint8_t const sfdpBuffer[])
{
    uint32_t size;

    /* The page size */
    size = 0x01UL << _FLD2VAL(CY_SMIF_SFDP_PAGE_SIZE,
        (uint32_t) sfdpBuffer[CY_SMIF_SFDP_BFPT_BYTE_28]);

    return(size);
}


/*******************************************************************************
* Function Name: SfdpGetEraseTime
****************************************************************************//**
*
* Calculates erase time. 
*
* \param eraseOffset
* The offset of the Sector Erase command in the SFDP buffer.
*
* \param sfdpBuffer
* The pointer to an array with the SDFP buffer.
*
* \param eraseTypeTime
* The pointer to an array with the erase time in us for different erase types.
*
* \return Default erase time in us.
*
*******************************************************************************/
static uint32_t SfdpGetEraseTime(uint32_t const eraseOffset, uint8_t const sfdpBuffer[], cy_stc_smif_erase_type_t eraseType[])
{
    /* Get the value of 10th DWORD from the JEDEC basic flash parameter table */
    uint32_t readEraseTime = ((uint32_t*)sfdpBuffer)[CY_SMIF_JEDEC_BFPT_10TH_DWORD];

    uint32_t eraseTimeDefaultIndex = (((eraseOffset - CY_SMIF_SFDP_BFPT_BYTE_1D) + TYPE_STEP) / TYPE_STEP);
    uint32_t eraseMul = _FLD2VAL(CY_SMIF_SFDP_ERASE_MUL_COUNT, readEraseTime);
    uint32_t eraseUnits = 0UL;
    uint32_t eraseCount = 0UL;
    uint32_t eraseMs = 0UL;
    uint32_t eraseTypeTypicalTime;

    for (uint32_t idx = 0UL; idx < ERASE_TYPE_COUNT; idx++){
        eraseTypeTypicalTime = (readEraseTime >> (idx * ERASE_T_LENGTH))>> ERASE_T_COUNT_OFFSET;
        eraseUnits = _FLD2VAL(ERASE_T_UNITS, eraseTypeTypicalTime);
        eraseCount = _FLD2VAL(ERASE_T_COUNT, eraseTypeTypicalTime);

        switch (eraseUnits)
        {
            case CY_SMIF_SFDP_UNIT_0:
                eraseMs = CY_SMIF_SFDP_ERASE_TIME_1MS;
                break;
            case CY_SMIF_SFDP_UNIT_1:
                eraseMs = CY_SMIF_SFDP_ERASE_TIME_16MS;
                break;
            case CY_SMIF_SFDP_UNIT_2:
                eraseMs = CY_SMIF_SFDP_ERASE_TIME_128MS;
                break;
            case CY_SMIF_SFDP_UNIT_3:
                eraseMs = CY_SMIF_SFDP_ERASE_TIME_1S;
                break;
            default:
                /* An unsupported SFDP value */
                break;
        }

        /* Convert typical time to max time */
        eraseType[idx].eraseTime = ((eraseCount + 1UL) * eraseMs) * (2UL * (eraseMul + 1UL));
    }

    return(eraseType[eraseTimeDefaultIndex - 1UL].eraseTime);
}


/*******************************************************************************
* Function Name: SfdpGetChipEraseTime
****************************************************************************//**
*
* Calculates chip erase time. 
*
* \param sfdpBuffer
* The pointer to an array with the SDFP buffer.
*
* \return Chip erase time in us.
*
*******************************************************************************/
static uint32_t SfdpGetChipEraseTime(uint8_t const sfdpBuffer[])
{
    /* Get the value of 10th DWORD from the JEDEC basic flash parameter table */
    uint32_t readEraseTime = ((uint32_t*)sfdpBuffer)[CY_SMIF_JEDEC_BFPT_10TH_DWORD];

    /* Get the value of 11th DWORD from the JEDEC basic flash parameter table */
    uint32_t chipEraseProgTime = ((uint32_t*)sfdpBuffer)[CY_SMIF_JEDEC_BFPT_11TH_DWORD];

    uint32_t chipEraseTimeMax;
    uint32_t chipEraseUnits = _FLD2VAL(CY_SMIF_SFDP_CHIP_ERASE_UNITS, chipEraseProgTime);
    uint32_t chipEraseCount = _FLD2VAL(CY_SMIF_SFDP_CHIP_ERASE_COUNT, chipEraseProgTime);
    uint32_t chipEraseMs = 0UL;
    uint32_t eraseMul = _FLD2VAL(CY_SMIF_SFDP_ERASE_MUL_COUNT, readEraseTime);

    switch (chipEraseUnits)
    {
        case CY_SMIF_SFDP_UNIT_0:
            chipEraseMs = CY_SMIF_SFDP_CHIP_ERASE_TIME_16MS;
            break;
        case CY_SMIF_SFDP_UNIT_1:
            chipEraseMs = CY_SMIF_SFDP_CHIP_ERASE_TIME_256MS;
            break;
        case CY_SMIF_SFDP_UNIT_2:
            chipEraseMs = CY_SMIF_SFDP_CHIP_ERASE_TIME_4S;
            break;
        case CY_SMIF_SFDP_UNIT_3:
            chipEraseMs = CY_SMIF_SFDP_CHIP_ERASE_TIME_64S;
            break;
        default:
            /* An unsupported SFDP value*/
            break;
    }

    /* Convert typical time to max time */
    chipEraseTimeMax = ((chipEraseCount + 1UL)*chipEraseMs) * (2UL *(eraseMul + 1UL));

    return(chipEraseTimeMax);
}


/*******************************************************************************
* Function Name: SfdpGetPageProgramTime
****************************************************************************//**
*
* Calculates page program time. 
*
* \param sfdpBuffer
* The pointer to an array with the SDFP buffer.
*
* \return Page program time in us.
*
*******************************************************************************/
static uint32_t SfdpGetPageProgramTime(uint8_t const sfdpBuffer[])
{
    /* Get the value of 11th DWORD from the JEDEC basic flash parameter table */
    uint32_t chipEraseProgTime = ((uint32_t*)sfdpBuffer)[CY_SMIF_JEDEC_BFPT_11TH_DWORD];
    uint32_t programTimeMax;
    uint32_t programTimeUnits = _FLD2VAL(CY_SMIF_SFDP_PAGE_PROG_UNITS, chipEraseProgTime);
    uint32_t programTimeCount  = _FLD2VAL(CY_SMIF_SFDP_PAGE_PROG_COUNT, chipEraseProgTime);
    uint32_t progMul = _FLD2VAL(CY_SMIF_SFDP_PROG_MUL_COUNT, chipEraseProgTime);
    uint32_t progUs;
    
    if (CY_SMIF_SFDP_UNIT_0 == programTimeUnits)
    {
        progUs = CY_SMIF_SFDP_PROG_TIME_8US;
    }
    else
    {
        progUs = CY_SMIF_SFDP_PROG_TIME_64US;
    }

    /* Convert typical time to max time */
    programTimeMax = ((programTimeCount + 1UL) * progUs) * (2UL * (progMul + 1UL));

    return(programTimeMax);
}


/*******************************************************************************
* Function Name: SfdpSetWriteEnableCommand
****************************************************************************//**
*
* Sets the Write Enable command and the width of the command transfer.
*
* \param cmdWriteEnable
* The pointer to the Write Enable command parameters structure.
*
*******************************************************************************/
static void SfdpSetWriteEnableCommand(cy_stc_smif_mem_cmd_t* cmdWriteEnable)
{
    /* 8-bit command. Write Enable */
    cmdWriteEnable->command = CY_SMIF_WRITE_ENABLE_CMD;

    /* The width of the command transfer */
    cmdWriteEnable->cmdWidth = CY_SMIF_WIDTH_SINGLE;
}


/*******************************************************************************
* Function Name: SfdpSetWriteDisableCommand
****************************************************************************//**
*
* Sets the Write Disable command and the width of the command transfer.
*
* \param cmdWriteDisable
* The pointer to the Write Disable command parameters structure.
*
*******************************************************************************/
static void SfdpSetWriteDisableCommand(cy_stc_smif_mem_cmd_t* cmdWriteDisable)
{
    /* The 8-bit command. Write Disable */
    cmdWriteDisable->command = CY_SMIF_WRITE_DISABLE_CMD;

    /* The width of the command transfer */
    cmdWriteDisable->cmdWidth = CY_SMIF_WIDTH_SINGLE;
}


/*******************************************************************************
* Function Name: SfdpSetProgramCommand_1_1_1 
****************************************************************************//**
*
* Sets the Program command parameters for 1S-1S-1S Protocol mode and 
* 3-byte addressing mode.
*
* \param cmdProgram
* The pointer to the Program command parameters structure.
*
*******************************************************************************/
static void SfdpSetProgramCommand_1_1_1(cy_stc_smif_mem_cmd_t* cmdProgram)
{
    /* 8-bit command. 1 x I/O Program command */
    cmdProgram->command = CY_SMIF_SINGLE_PROGRAM_CMD;
    /* The command transfer width */
    cmdProgram->cmdWidth = CY_SMIF_WIDTH_SINGLE;
    /* The address transfer width */
    cmdProgram->addrWidth = CY_SMIF_WIDTH_SINGLE;
    /* 8-bit mode byte. This value is 0xFFFFFFFF when there is no mode present */
    cmdProgram->mode = CY_SMIF_NO_COMMAND_OR_MODE;
    /* The width of the mode command transfer */
    cmdProgram->modeWidth = CY_SMIF_WIDTH_SINGLE;
    /* The dummy cycles number. A zero value suggests no dummy cycles */
    cmdProgram->dummyCycles = 0UL;
    /* The data transfer width */
    cmdProgram->dataWidth = CY_SMIF_WIDTH_SINGLE;
}


/*******************************************************************************
* Function Name: SfdpSetProgramCommandFourBytes_1_1_1
****************************************************************************//**
*
* Sets the Program command parameters for 1S-1S-1S Protocol mode and 
* 4-byte addressing mode.
*
* \param cmdProgram
* The pointer to the Program command parameters structure.
*
*******************************************************************************/
static void SfdpSetProgramCommandFourBytes_1_1_1(cy_stc_smif_mem_cmd_t* cmdProgram)
{
    /* 8-bit command. 1 x I/O Program command */
    cmdProgram->command = CY_SMIF_PAGE_PROGRAM_4_BYTES_CMD_1S_1S_1S;
    /* The command transfer width */
    cmdProgram->cmdWidth = CY_SMIF_WIDTH_SINGLE;
    /* The address transfer width */
    cmdProgram->addrWidth = CY_SMIF_WIDTH_SINGLE;
    /* 8-bit mode byte. This value is 0xFFFFFFFF when there is no mode present */
    cmdProgram->mode = CY_SMIF_NO_COMMAND_OR_MODE;
    /* The width of the mode command transfer */
    cmdProgram->modeWidth = CY_SMIF_WIDTH_SINGLE;
    /* The dummy cycles number. A zero value suggests no dummy cycles */
    cmdProgram->dummyCycles = 0UL;
    /* The data transfer width */
    cmdProgram->dataWidth = CY_SMIF_WIDTH_SINGLE;
}


/*******************************************************************************
* Function Name: SfdpSetProgramCommandFourBytes_1_1_4
****************************************************************************//**
*
* Sets the Program command parameters for 1S-1S-4S Protocol mode and 
* 4-byte addressing mode.
*
* \param cmdProgram
* The pointer to the Program command parameters structure.
*
*******************************************************************************/
static void SfdpSetProgramCommandFourBytes_1_1_4(cy_stc_smif_mem_cmd_t* cmdProgram)
{
    /* 8-bit command. 1 x I/O Program command */
    cmdProgram->command = CY_SMIF_PAGE_PROGRAM_4_BYTES_CMD_1S_1S_4S;
    /* The command transfer width */
    cmdProgram->cmdWidth = CY_SMIF_WIDTH_SINGLE;
    /* The address transfer width */
    cmdProgram->addrWidth = CY_SMIF_WIDTH_SINGLE;
    /* 8-bit mode byte. This value is 0xFFFFFFFF when there is no mode present */
    cmdProgram->mode = CY_SMIF_NO_COMMAND_OR_MODE;
    /* The width of the mode command transfer */
    cmdProgram->modeWidth = CY_SMIF_WIDTH_SINGLE;
    /* The dummy cycles number. A zero value suggests no dummy cycles */
    cmdProgram->dummyCycles = 0UL;
    /* The data transfer width */
    cmdProgram->dataWidth = CY_SMIF_WIDTH_QUAD;
}


/*******************************************************************************
* Function Name: SfdpSetProgramCommandFourBytes_1_4_4
****************************************************************************//**
*
* Sets the Program command parameters for 1S-4S-4S Protocol mode and 
* 4-byte addressing mode.
*
* \param cmdProgram
* The pointer to the Program command parameters structure.
*
*******************************************************************************/
static void SfdpSetProgramCommandFourBytes_1_4_4(cy_stc_smif_mem_cmd_t* cmdProgram)
{
    /* 8-bit command. 1 x I/O Program command */
    cmdProgram->command = CY_SMIF_PAGE_PROGRAM_4_BYTES_CMD_1S_4S_4S;
    /* The command transfer width */
    cmdProgram->cmdWidth = CY_SMIF_WIDTH_SINGLE;
    /* The address transfer width */
    cmdProgram->addrWidth = CY_SMIF_WIDTH_QUAD;
    /* 8-bit mode byte. This value is 0xFFFFFFFF when there is no mode present */
    cmdProgram->mode = CY_SMIF_NO_COMMAND_OR_MODE;
    /* The width of the mode command transfer */
    cmdProgram->modeWidth = CY_SMIF_WIDTH_QUAD;
    /* The dummy cycles number. A zero value suggests no dummy cycles */
    cmdProgram->dummyCycles = 0UL;
    /* The data transfer width */
    cmdProgram->dataWidth = CY_SMIF_WIDTH_QUAD;
}

/*******************************************************************************
* Function Name: SfdpGetProgramFourBytesCmd
****************************************************************************//**
*
* Sets the Page Program command instruction for 4-byte addressing mode.
*
* \note When the Program command is not found for 4 byte addressing mode 
* the Program command instruction is set for 1S-1S-1S Protocol mode and 
* 3-byte addressing mode.
*
* \param sfdpBuffer
* The pointer to an array with the SDFP buffer with the 4-byte Address
* Instruction table.
*
* \param protocolMode
* Protocol Mode.
*
* \param cmdProgram
* The pointer to the Program command parameters structure.
*
*******************************************************************************/
static void SfdpGetProgramFourBytesCmd(uint8_t const sfdpBuffer[], 
                                       cy_en_smif_protocol_mode_t protocolMode, 
                                       cy_stc_smif_mem_cmd_t* cmdProgram)  
{
    /* Get the mask which contains the Support for Page Program Commands
     * from 4-byte Address Instruction Table, DWORD 1 
     */
    uint32_t sfdpForBytesTableDword1 = ((uint32_t*)sfdpBuffer)[FOUR_BYTE_ADDRESS_TABLE_BYTE_0];
    
    switch (protocolMode)
    {
        case PROTOCOL_MODE_1S_4S_4S:
            if (_FLD2BOOL(SUPPORT_PP_1S_4S_4S_CMD, sfdpForBytesTableDword1))
            {
                SfdpSetProgramCommandFourBytes_1_4_4(cmdProgram);
            }
            else if (_FLD2BOOL(SUPPORT_PP_1S_1S_4S_CMD, sfdpForBytesTableDword1))
            {
                SfdpSetProgramCommandFourBytes_1_1_4(cmdProgram);
            }
            else if (_FLD2BOOL(SUPPORT_PP_1S_1S_1S_CMD, sfdpForBytesTableDword1))
            {
                SfdpSetProgramCommandFourBytes_1_1_1(cmdProgram);
            }
            else
            {
                /* There are no instructions for 4-byte mode. Use the instruction for 3-byte mode */
                SfdpSetProgramCommand_1_1_1(cmdProgram);
            }
            break;
        case PROTOCOL_MODE_1S_1S_4S:
            if (_FLD2BOOL(SUPPORT_PP_1S_1S_4S_CMD, sfdpForBytesTableDword1))
            {
                SfdpSetProgramCommandFourBytes_1_1_4(cmdProgram); 
            }
            else if (_FLD2BOOL(SUPPORT_PP_1S_1S_1S_CMD, sfdpForBytesTableDword1))
            {
                SfdpSetProgramCommandFourBytes_1_1_1(cmdProgram);
            }
            else
            {
                /* There are no instructions for 4-byte mode. Use the instruction for 3-byte mode */
                SfdpSetProgramCommand_1_1_1(cmdProgram);
            }
            break;
        case PROTOCOL_MODE_1S_2S_2S:
        case PROTOCOL_MODE_1S_1S_2S:
        case PROTOCOL_MODE_1S_1S_1S:
            if (_FLD2BOOL(SUPPORT_PP_1S_1S_1S_CMD, sfdpForBytesTableDword1))
            {
                SfdpSetProgramCommandFourBytes_1_1_1(cmdProgram);
            }
            else
            {
                /* There are no instructions for 4-byte mode. Use the instruction for 3-byte mode */
                SfdpSetProgramCommand_1_1_1(cmdProgram);
            }
            break;
        default:
            /* Wrong mode */
            CY_ASSERT_L2(true);  
            break;
    }
}


/*******************************************************************************
* Function Name: SfdpSetWipStatusRegisterCommand
****************************************************************************//**
*
* Sets the WIP-containing status register command and 
* the width of the command transfer.
*
* \param readStsRegWipCmd
* The pointer to the WIP-containing status register command parameters structure.
*
*******************************************************************************/
static void SfdpSetWipStatusRegisterCommand(cy_stc_smif_mem_cmd_t* readStsRegWipCmd)
{
    /* 8-bit command. WIP RDSR */
    readStsRegWipCmd->command  = CY_SMIF_READ_STATUS_REG1_CMD;
    /* The command transfer width */
    readStsRegWipCmd->cmdWidth = CY_SMIF_WIDTH_SINGLE;
}


/*******************************************************************************
* Function Name: SfdpGetQuadEnableParameters
****************************************************************************//**
*
* Gets the Quad Enable parameters.
*
* \param device
* The device structure instance declared by the user. This is where the detected
* parameters are stored and returned.
*
* \param sfdpBuffer
* The pointer to an array with the SDFP buffer.
*
*******************************************************************************/
static void SfdpGetQuadEnableParameters(cy_stc_smif_mem_device_cfg_t *device, 
                                    uint8_t const sfdpBuffer[])
{
    CY_ASSERT_L1(NULL != device->readStsRegQeCmd);
    CY_ASSERT_L1(NULL != device->writeStsRegQeCmd);

    /* The command transfer width */
    device->writeStsRegQeCmd->cmdWidth = CY_SMIF_WIDTH_SINGLE;

    /* The QE mask for the status registers */
    switch (_FLD2VAL(CY_SMIF_SFDP_QE_REQUIREMENTS, (uint32_t)sfdpBuffer
        [CY_SMIF_SFDP_BFPT_BYTE_3A]))
    {
        case CY_SMIF_SFDP_QER_0:
            device->stsRegQuadEnableMask = CY_SMIF_NO_COMMAND_OR_MODE;
            device->writeStsRegQeCmd->command  = CY_SMIF_NO_COMMAND_OR_MODE;
            device->readStsRegQeCmd->command  = CY_SMIF_NO_COMMAND_OR_MODE;
            break;
        case CY_SMIF_SFDP_QER_1:
        case CY_SMIF_SFDP_QER_4:
        case CY_SMIF_SFDP_QER_5:
            device->stsRegQuadEnableMask = CY_SMIF_SFDP_QE_BIT_1_OF_SR_2;

            /* The command to write into the QE-containing status register */
            /* The 8-bit command. QE WRSR */
            device->writeStsRegQeCmd->command  = CY_SMIF_WRITE_STATUS_REG1_CMD;
            device->readStsRegQeCmd->command  = CY_SMIF_READ_STATUS_REG2_T1_CMD;
            break;
        case CY_SMIF_SFDP_QER_2:
            device->stsRegQuadEnableMask = CY_SMIF_SFDP_QE_BIT_6_OF_SR_1;

            /* The command to write into the QE-containing status register */
            /* The 8-bit command. QE WRSR */
            device->writeStsRegQeCmd->command  = CY_SMIF_WRITE_STATUS_REG1_CMD;
            device->readStsRegQeCmd->command  = CY_SMIF_READ_STATUS_REG1_CMD;
            break;
        case CY_SMIF_SFDP_QER_3:
            device->stsRegQuadEnableMask = CY_SMIF_SFDP_QE_BIT_7_OF_SR_2;

            /* The command to write into the QE-containing status register */
            /* The 8-bit command. QE WRSR */
            device->writeStsRegQeCmd->command  = CY_SMIF_WRITE_STATUS_REG2_CMD;
            device->readStsRegQeCmd->command  = CY_SMIF_READ_STATUS_REG2_T2_CMD;
            break;
        default:
            break;
    }
}


/*******************************************************************************
* Function Name: SfdpSetChipEraseCommand
****************************************************************************//**
*
* Sets the Chip Erase command and the width of the command transfer.
*
* \param cmdChipErase
* The pointer to the Chip Erase command parameters structure.
*
*******************************************************************************/
static void SfdpSetChipEraseCommand(cy_stc_smif_mem_cmd_t* cmdChipErase)
{
    /* 8-bit command. Chip Erase */
    cmdChipErase->command  = CY_SMIF_CHIP_ERASE_CMD;
    /* The width of the command transfer */
    cmdChipErase->cmdWidth = CY_SMIF_WIDTH_SINGLE;
}


/*******************************************************************************
* Function Name: SfdpGetSectorEraseCommand
****************************************************************************//**
*
* Sets the Sector Erase command and the width of the command transfer.
*
* \note When the Erase command is not found the width of the command 
*  transfer (cmdWidth) is set to CY_SMIF_WIDTH_NA.
*
* \param device
* The device structure instance declared by the user. This is where the detected
* parameters are stored and returned.
*
* \param sfdpBuffer
* The pointer to an array with the SDFP buffer.
*
* \param eraseTypeCmd
* The pointer to an array with the erase commands for different erase types.
*
* \return The offset of the Sector Erase command in the SFDP buffer. 
*  Returns 0 when the Sector Erase command is not found.
*
*******************************************************************************/
static uint32_t SfdpGetSectorEraseCommand(cy_stc_smif_mem_device_cfg_t *device, 
                                          uint8_t const sfdpBuffer[],
                                          cy_stc_smif_erase_type_t eraseTypeStc[])
{
    uint32_t eraseOffset;
    if (FOUR_BYTE_ADDRESS == device->numOfAddrBytes)
    {
        uint32_t eraseType;     /* Erase Type decreased to 1 */
        uint32_t eraseTypeMask;

        eraseOffset = COMMAND_IS_NOT_FOUND;
        device->eraseCmd->command = CY_SMIF_NO_COMMAND_OR_MODE;

        /* Get the mask which contains the Support for Erase Commands (Types 1-4) 
         * from 4-byte Address Instruction Table, DWORD 1 
         */
        eraseTypeMask = _FLD2VAL(SUPPORT_ERASE_COMMAND, (uint32_t)sfdpBuffer[FOUR_BYTE_ADDRESS_TABLE_BYTE_1]);

        /* Find Erase Type (decreased to 1) */ 
        for (eraseType = 0UL; eraseType <= ERASE_TYPE_COUNT; eraseType++)
        {
            if ((1UL << eraseType) == (eraseTypeMask & (1UL << eraseType)))
            {
                /* Erase Type found */
                break;
            }
        }
        
        if (eraseType < ERASE_TYPE_COUNT)
        {
            /* Calculate the offset for the sector Erase command in the 4-byte Address Instruction Table, DWORD 2 */
            eraseOffset = FOUR_BYTE_ADDR_ERASE_TYPE_1 + eraseType;  

            /* Update all erase commands for 4-bytes*/
            for(uint32_t i = 0UL; i< ERASE_TYPE_COUNT; i++)
            {
                eraseTypeStc[i].eraseCmd = sfdpBuffer[FOUR_BYTE_ADDR_ERASE_TYPE_1 + i];
            }
            /* Get the sector Erase command
             * from the 4-byte Address Instruction Table, DWORD 2 
             */
            device->eraseCmd->command = sfdpBuffer[eraseOffset];
            
            /* Recalculate eraseOffset for the 3-byte Address Instruction Table 
             * to find the device->eraseSize and device->eraseTime parameters based on Erase Type.
             */
            eraseOffset = CY_SMIF_SFDP_BFPT_BYTE_1D + (eraseType * TYPE_STEP);
        }       
    }
    else
    {
        eraseOffset = CY_SMIF_SFDP_BFPT_BYTE_1D;
        while (INSTRUCTION_NOT_SUPPORTED == sfdpBuffer[eraseOffset])
        {
            if (eraseOffset >= CY_SMIF_SFDP_BFPT_BYTE_23)
            {
                /* The Sector Erase command is not found */
                eraseOffset = COMMAND_IS_NOT_FOUND;
                break;
            }
            eraseOffset += TYPE_STEP; /* Check the next Erase Type */
        }

        if (COMMAND_IS_NOT_FOUND != eraseOffset)
        {
            /* Get the sector Erase command from the JEDEC basic flash parameter table */
            device->eraseCmd->command = sfdpBuffer[eraseOffset];
        }  
    }

    if (COMMAND_IS_NOT_FOUND != eraseOffset)
    {
        /* The command transfer width */
        device->eraseCmd->cmdWidth = CY_SMIF_WIDTH_SINGLE;

        /* The address transfer width */
        device->eraseCmd->addrWidth = CY_SMIF_WIDTH_SINGLE;
    }
    else
    {
        device->eraseCmd->cmdWidth = CY_SMIF_WIDTH_NA;
    }

    return(eraseOffset);
}


/*******************************************************************************
* Function Name: ReadAnyReg
****************************************************************************//**
*
* This function reads any registers by address. This function is a blocking 
* function, it will block the execution flow until the status register is read.
*
* \param base
* Holds the base address of the SMIF block registers.
*
* \param slaveSelect
* The slave select line for the device.
*
* \param value
* The value of the register.
*
* \param command
* The command required to read the status/configuration register.
*
* \param address
* The register address array.
*
* \param addressSize
* The size of the address array.
*
* \param context
* This is the pointer to the context structure \ref cy_stc_smif_context_t
* allocated by the user. The structure is used during the SMIF
* operation for internal configuration and data retention. The user must not
* modify anything in this structure.
*
* \return A status of the command reception.
*       - \ref CY_SMIF_SUCCESS
*       - \ref CY_SMIF_CMD_FIFO_FULL
*       - \ref CY_SMIF_EXCEED_TIMEOUT
*       - \ref CY_SMIF_CMD_NOT_FOUND
*
*******************************************************************************/
static cy_en_smif_status_t ReadAnyReg(SMIF_Type *base,
                                    cy_en_smif_slave_select_t  slaveSelect,
                                    uint8_t *value,
                                    uint8_t command,
                                    uint8_t const *address,
                                    uint32_t addressSize,
                                    cy_stc_smif_context_t const *context)
{
    cy_en_smif_status_t result = CY_SMIF_CMD_NOT_FOUND;

    /* Read the memory register */
    result = Cy_SMIF_TransmitCommand(base, command, CY_SMIF_WIDTH_SINGLE,
                address, addressSize,
                CY_SMIF_WIDTH_SINGLE, slaveSelect,
                CY_SMIF_TX_NOT_LAST_BYTE, context);

    if (CY_SMIF_SUCCESS == result)
    {
        result = Cy_SMIF_ReceiveDataBlocking( base, value,
                    CY_SMIF_READ_ONE_BYTE, CY_SMIF_WIDTH_SINGLE, context);
    }

    return(result);
}


/*******************************************************************************
* Function Name: SfdpEnterFourByteAddressing
****************************************************************************//**
*
* This function sets 4-byte address mode for a memory device as defined in 
* 16th DWORD of JEDEC Basic Flash Parameter Table. 
*
* \note The entry methods which do not support the required
* operation of writing into the register.
*
* \param base
* Holds the base address of the SMIF block registers.
*
* \param entryMethodByte
* The byte which defines the supported method to enter 4-byte addressing mode.
*
* \param device
* The device structure instance declared by the user. This is where the detected
* parameters are stored and returned.
*
* \param slaveSelect
* The slave select line for the device.
*
* \param context
* This is the pointer to the context structure \ref cy_stc_smif_context_t
* allocated by the user. The structure is used during the SMIF
* operation for internal configuration and data retention. The user must not
* modify anything in this structure.
*
* \return A status of 4-byte addressing mode command transmit.
*       - \ref CY_SMIF_SUCCESS
*       - \ref CY_SMIF_EXCEED_TIMEOUT
*       - \ref CY_SMIF_CMD_NOT_FOUND
*******************************************************************************/
static cy_en_smif_status_t SfdpEnterFourByteAddressing(SMIF_Type *base, uint8_t entryMethodByte, 
                                                       cy_stc_smif_mem_device_cfg_t *device,
                                                       cy_en_smif_slave_select_t slaveSelect,
                                                       cy_stc_smif_context_t const *context)
{
    cy_en_smif_status_t result = CY_SMIF_CMD_NOT_FOUND;
    if ((entryMethodByte & CY_SMIF_SFDP_ENTER_4_BYTE_METHOD_ALWAYS_4_BYTE) != 0U)
    {
        /* Memory always operates in 4-byte mode */
        result = CY_SMIF_SUCCESS;
    }
    if ((entryMethodByte & CY_SMIF_SFDP_ENTER_4_BYTE_METHOD_B7) != 0U)
    {
        if ((entryMethodByte & CY_SMIF_SFDP_ENTER_4_BYTE_METHOD_WR_EN_B7) != 0U)
        {
            /* To enter a 4-byte addressing write enable is required */
            cy_stc_smif_mem_cmd_t* writeEn = device->writeEnCmd;
            if(NULL != writeEn)
            {
                result = Cy_SMIF_TransmitCommand(base,
                                                (uint8_t) writeEn->command,
                                                writeEn->cmdWidth,
                                                CY_SMIF_CMD_WITHOUT_PARAM,
                                                CY_SMIF_CMD_WITHOUT_PARAM,
                                                CY_SMIF_WIDTH_NA,
                                                slaveSelect,
                                                CY_SMIF_TX_LAST_BYTE,
                                                context);
            }
        }
        if ((CY_SMIF_CMD_NOT_FOUND == result) || (CY_SMIF_SUCCESS == result))
        {
            /* To enter a 4-byte addressing B7 instruction is required*/
            result = Cy_SMIF_TransmitCommand(base,
                                             CY_SMIF_SFDP_ENTER_4_BYTE_METHOD_B7_CMD,
                                             CY_SMIF_WIDTH_SINGLE,
                                             CY_SMIF_CMD_WITHOUT_PARAM,
                                             CY_SMIF_CMD_WITHOUT_PARAM,
                                             CY_SMIF_WIDTH_NA,
                                             slaveSelect,
                                             CY_SMIF_TX_LAST_BYTE,
                                             context);
        }
    }

    return result;
}


/*******************************************************************************
* Function Name: SfdpGetEraseSizeAndCmd
****************************************************************************//**
*
* Fills arrays with an erase size and cmd for all erase types.
*
* \param sfdpBuffer
* The pointer to an array with the Basic Flash Parameter table buffer.
*
* \param eraseTypeCmd
* The pointer to an array with the erase commands for all erase types.
*
* \param eraseTypeSize
* The pointer to an array with the erase size for all erase types.
*
*******************************************************************************/
static void SfdpGetEraseSizeAndCmd(uint8_t const sfdpBuffer[],
                                   cy_stc_smif_erase_type_t eraseType[])
{
    uint32_t idx = 0UL;
    for (uint32_t currET = 0UL; currET < ERASE_TYPE_COUNT; currET++)
    {
        /* The erase size in the SFDP buffer defined as power of two */
        eraseType[currET].eraseSize = 1UL << sfdpBuffer[CY_SMIF_SFDP_BFPT_BYTE_1C + idx];
        eraseType[currET].eraseCmd = sfdpBuffer[CY_SMIF_SFDP_BFPT_BYTE_1D + idx];
        idx += TYPE_STEP;
    }
}


/*******************************************************************************
* Function Name: SfdpPopulateRegionInfo
****************************************************************************//**
*
* Reads the current configuration for regions and populates regionInfo
* structures.
*
* \param base
* Holds the base address of the SMIF block registers.
*
* \param sectorMapBuff
* The pointer to an array with the Sector Map Parameter Table buffer.
*
* \param device
* The device structure instance declared by the user. This is where the detected
* parameters are stored and returned.
*
* \param slaveSelect
* The slave select line for the device.
*
* \param context
* This is the pointer to the context structure \ref cy_stc_smif_context_t
* allocated by the user. The structure is used during the SMIF
* operation for internal configuration and data retention. The user must not
* modify anything in this structure.
*
* \param eraseTypeSize
* The pointer to an array with the erase size for all erase types.
*
* \param eraseTypeCmd
* The pointer to an array with the erase commands for all erase types.
*
* \param eraseTypeTime
*  The pointer to an array with the erase time for all erase types.
*
* \return A status of the Sector Map Parameter Table parsing.
*       - \ref CY_SMIF_SUCCESS
*       - \ref CY_SMIF_SFDP_CORRUPTED_TABLE
*       - \ref CY_SMIF_NOT_HYBRID_MEM
*
*******************************************************************************/
static cy_en_smif_status_t SfdpPopulateRegionInfo(SMIF_Type *base,
                                   uint8_t const sectorMapBuff[],
                                   uint32_t const buffLength,
                                   cy_stc_smif_mem_device_cfg_t *device,
                                   cy_en_smif_slave_select_t slaveSelect,
                                   const cy_stc_smif_context_t *context,
                                   cy_stc_smif_erase_type_t eraseType[])
{
    uint8_t currCmd;
    uint8_t regMask;
    uint8_t regValue;
    uint8_t currRegisterAddr[ERASE_TYPE_COUNT] = {0U};
    uint8_t regionInfoIdx = 0U;
    uint32_t currTableIdx = 0UL;
    uint32_t addrBytesNum = 0UL;
    uint32_t addrCode = 0UL;
    cy_en_smif_status_t result = CY_SMIF_NOT_HYBRID_MEM;

    /* Loop across all command descriptors to find current configuration */
    while(SECTOR_MAP_COMAND_DESCRIPTOR_TYPE == (sectorMapBuff[currTableIdx] & SECTOR_MAP_DESCRIPTOR_MASK))
    {
        currCmd = sectorMapBuff[currTableIdx + CY_SMIF_SFDP_SECTOR_MAP_CMD_OFFSET];
        regMask = sectorMapBuff[currTableIdx + CY_SMIF_SFDP_SECTOR_MAP_REG_MSK_OFFSET];
        regValue = 0U;
        
        /* Get the address length for configuration detection */
        addrCode = _FLD2VAL(CY_SMIF_SFDP_SECTOR_MAP_ADDR_BYTES, sectorMapBuff[currTableIdx + CY_SMIF_SFDP_SECTOR_MAP_ADDR_CODE_OFFSET]);
        switch(addrCode) 
        {
            case CY_SMIF_SFDP_THREE_BYTES_ADDR_CODE:
                /* No address cycle */
                addrBytesNum = 0UL;
                break;
            case CY_SMIF_SFDP_THREE_OR_FOUR_BYTES_ADDR_CODE:
                addrBytesNum = CY_SMIF_THREE_BYTES_ADDR;
                break;
            case CY_SMIF_SFDP_FOUR_BYTES_ADDR_CODE:
                addrBytesNum = CY_SMIF_FOUR_BYTES_ADDR;
                break;
            default:
                /* Use the current settings */
                addrBytesNum = device->numOfAddrBytes; 
                break;
        }

        /* Get the control register address */
        for(uint32_t i = 0UL; i < addrBytesNum; i++)
        {
            /* Offset for control register in SFDP has little-endian byte order, need to swap it */
            currRegisterAddr[i] = sectorMapBuff[(currTableIdx + CY_SMIF_SFDP_SECTOR_MAP_REG_ADDR_OFFSET + addrBytesNum) - i - 1UL];
        }

        /* Read the value of the register for the current configuration detection*/
        result = ReadAnyReg(base, slaveSelect, &regValue, currCmd, &currRegisterAddr[0], addrBytesNum, context);

        if (CY_SMIF_SUCCESS == result)
        {
            /* Set the bit of the region idx to 1 if the config matches */
            regionInfoIdx = ((uint8_t)(regionInfoIdx << 1U)) | (((regValue & regMask) == 0U)?(0U):(1U));
        }

        currTableIdx += HEADER_LENGTH;
        if (currTableIdx > buffLength)
        {
            result = CY_SMIF_SFDP_CORRUPTED_TABLE;
            break;
        }
    }

    if (CY_SMIF_SUCCESS == result)
    {
        /* Find the matching configuration map descriptor */
        while(regionInfoIdx != sectorMapBuff[currTableIdx + 1UL])
        {
            /* Increment the table index to the next map */
            currTableIdx += (sectorMapBuff[currTableIdx + CY_SMIF_SFDP_SECTOR_MAP_CONFIG_ID_OFFSET] + 2UL) * BYTES_IN_DWORD;
            if (currTableIdx > buffLength)
            {
                result = CY_SMIF_SFDP_CORRUPTED_TABLE;
                break;
            }
        }
    }

    if (CY_SMIF_SUCCESS == result)
    {
        /* Populate region data from the sector map */
        uint8_t numOfRegions = sectorMapBuff[currTableIdx + CY_SMIF_SFDP_SECTOR_MAP_REGION_COUNT_OFFSET] + 1U;
        device->hybridRegionCount = (uint32_t) numOfRegions;

        if(numOfRegions <= 1U)
        {
            result = CY_SMIF_NOT_HYBRID_MEM;
        }
        else
        {
            uint8_t eraseTypeCode;
            uint32_t currRegionAddr = 0UL;
            uint32_t regionSize = 0UL;
            uint8_t supportedEraseType;
            uint8_t eraseTypeMask;
            cy_stc_smif_hybrid_region_info_t *currRegionPtr;
            for(uint8_t currRegion = 0U; currRegion< numOfRegions; currRegion++)
            {
                currRegionAddr = currRegionAddr + regionSize;
                currTableIdx += BYTES_IN_DWORD;

                supportedEraseType = 0U;
                eraseTypeMask = 1U;
                eraseTypeCode = sectorMapBuff[currTableIdx] & CY_SMIF_SFDP_SECTOR_MAP_SUPPORTED_ET_MASK;
                while(0U == (eraseTypeCode & eraseTypeMask))
                {
                    /* Erase type number defined as a bit position */
                    eraseTypeMask = eraseTypeMask << 1;
                    supportedEraseType++;
                    if(supportedEraseType > ERASE_TYPE_COUNT)
                    {
                        result = CY_SMIF_SFDP_CORRUPTED_TABLE;
                        break;
                    }
                }

                /* The region size as a zero-based count of 256 byte units */
                regionSize = ((*( (uint32_t*) &sectorMapBuff[currTableIdx]) >> BITS_IN_BYTE) + 1UL) * SECTOR_MAP_REGION_SIZE_MULTIPLIER;
                currRegionPtr = device->hybridRegionInfo[currRegion];

                currRegionPtr->regionAddress = currRegionAddr;
                currRegionPtr->eraseCmd = (uint32_t)eraseType[supportedEraseType].eraseCmd;
                currRegionPtr->eraseTime = eraseType[supportedEraseType].eraseTime;
                if(regionSize < eraseType[supportedEraseType].eraseSize)
                {
                    /* One region with a single sector */
                    currRegionPtr->eraseSize = regionSize;
                    currRegionPtr->sectorsCount = 1UL;
                }
                else
                {
                    currRegionPtr->eraseSize = eraseType[supportedEraseType].eraseSize;
                    currRegionPtr->sectorsCount = regionSize / eraseType[supportedEraseType].eraseSize;
                }
            }
        }
    }
    return result;
}


/*******************************************************************************
* Function Name: Cy_SMIF_MemSfdpDetect
****************************************************************************//**
*
* This function detects the device signature for SFDP devices.
* Refer to the SFDP spec (JESD216B) for details.
* The function asks the device using an SPI and then populates the relevant
* parameters for \ref cy_stc_smif_mem_device_cfg_t.
*
* \note This function is a blocking function and blocks until the structure data
* is read and returned. This function uses \ref Cy_SMIF_TransmitCommand()
* If there is no support for SFDP in the memory device, the API returns an
* error condition. The function requires:
*   - SMIF initialized and enabled to work in the normal mode;
*   - readSfdpCmd field of \ref cy_stc_smif_mem_device_cfg_t is enabled.
*
* \note The SFDP detect takes into account the types of the SPI supported by the
* memory device and also the dataSelect option selected to choose which SPI mode
* (SPI, DSPI, QSPI) to load into the structures. The algorithm prefers
* QSPI>DSPI>SPI, provided there is support for it in the memory device and the
* dataSelect selected by the user.
*
* \note 4-byte addressing mode is set when the memory device supports 
*       3- or 4-byte addressing mode.
*
* \note When the Erase command is not found the width of the command 
*  transfer (cmdWidth) is set to CY_SMIF_WIDTH_NA. When the Program command 
*  is not found for 4 byte addressing mode the Program command instruction 
*  is set for 1S-1S-1S Protocol mode and 3-byte addressing mode.
*
* \param base
* Holds the base address of the SMIF block registers.
*
* \param device
* The device structure instance declared by the user. This is where the detected
* parameters are stored and returned.
*
* \param slaveSelect
* The slave select line for the device.
*
* \param dataSelect
* The data line selection options for a slave device.
*
* \param context
* This is the pointer to the context structure \ref cy_stc_smif_context_t
* allocated by the user. The structure is used during the SMIF
* operation for internal configuration and data retention. The user must not
* modify anything in this structure.
*
* \return A status of the transmission.
*       - \ref CY_SMIF_SUCCESS
*       - \ref CY_SMIF_CMD_FIFO_FULL
*       - \ref CY_SMIF_NO_SFDP_SUPPORT
*       - \ref CY_SMIF_EXCEED_TIMEOUT
*
*******************************************************************************/
cy_en_smif_status_t Cy_SMIF_MemSfdpDetect(SMIF_Type *base,
                                    cy_stc_smif_mem_device_cfg_t *device,
                                    cy_en_smif_slave_select_t slaveSelect,
                                    cy_en_smif_data_select_t dataSelect,
                                    cy_stc_smif_context_t *context)
{
    /* Check the input parameters */
    CY_ASSERT_L1(NULL != device);
    
    uint8_t sfdpBuffer[CY_SMIF_SFDP_LENGTH];
    uint8_t sfdpAddress[CY_SMIF_SFDP_ADDRESS_LENGTH] = {0x00U, 0x00U, 0x00U};
    uint8_t addr4ByteAddress[CY_SMIF_SFDP_ADDRESS_LENGTH] = {0x00U, 0x00U, 0x00U};
    uint8_t sectorMapAddr[CY_SMIF_SFDP_ADDRESS_LENGTH] = {0x00U, 0x00U, 0x00U};
    cy_en_smif_status_t result = CY_SMIF_NO_SFDP_SUPPORT;
    cy_stc_smif_mem_cmd_t *cmdSfdp = device->readSfdpCmd;
    cy_stc_smif_erase_type_t eraseType[ERASE_TYPE_COUNT];

    /* Initialize the SFDP buffer */
    for (uint32_t i = 0U; i < CY_SMIF_SFDP_LENGTH; i++)
    {
        sfdpBuffer[i] = 0U;
    }
    
    /* Slave slot initialization */
    Cy_SMIF_SetDataSelect(base, slaveSelect, dataSelect);

    if (NULL != cmdSfdp)
    {
        /* Get the SDFP header and all parameter headers content into sfdpBuffer[] */
        result = SfdpReadBuffer(base, 
                               cmdSfdp,
                               sfdpAddress,
                               slaveSelect,
                               HEADERS_LENGTH,
                               sfdpBuffer,
                               context);
    }
    
    /* Check if we support all parameter headers */
    if ((CY_SMIF_SUCCESS == result) && 
       (sfdpBuffer[PARAM_HEADERS_NUM] > PARAM_HEADER_NUM))
    {
        result = CY_SMIF_NO_SFDP_SUPPORT;
    }

    if (CY_SMIF_SUCCESS == result)
    {
        if((sfdpBuffer[CY_SMIF_SFDP_SIGNATURE_BYTE_00] == (uint8_t)'S') &&
         (sfdpBuffer[CY_SMIF_SFDP_SIGNATURE_BYTE_01] == (uint8_t)'F') &&
         (sfdpBuffer[CY_SMIF_SFDP_SIGNATURE_BYTE_02] == (uint8_t)'D') &&
         (sfdpBuffer[CY_SMIF_SFDP_SIGNATURE_BYTE_03] == (uint8_t)'P') &&
         (sfdpBuffer[CY_SMIF_SFDP_MINOR_REV] >= CY_SMIF_SFDP_JEDEC_REV_B) &&
         (sfdpBuffer[CY_SMIF_SFDP_MAJOR_REV] == CY_SMIF_SFDP_MAJOR_REV_1))
        {
            /* Find the 4-byte Address Instruction Parameter Header */
            uint32_t id = (FOUR_BYTE_ADDR_ID_MSB << BITS_IN_BYTE) | FOUR_BYTE_ADDR_ID_LSB;
            uint32_t addr4ByteTableLength = 0UL;
            result = SfdpFindParameterTableAddress(id, sfdpBuffer, addr4ByteAddress, &addr4ByteTableLength);

            /* Find the Sector Map Parameter Header */
            id = (SECTOR_MAP_ID_MSB << BITS_IN_BYTE) | SECTOR_MAP_ID_LSB;
            uint32_t sectorMapTableLength = 0UL;
            result = SfdpFindParameterTableAddress(id, sfdpBuffer, sectorMapAddr, &sectorMapTableLength);
            if (CY_SMIF_CMD_NOT_FOUND == result)
            {
                device->hybridRegionCount = 0UL;
                device->hybridRegionInfo = NULL;
            }

            /* Find the JEDEC SFDP Basic SPI Flash Parameter Header */
            id = (BASIC_SPI_ID_MSB << BITS_IN_BYTE) | BASIC_SPI_ID_LSB;
            uint32_t basicSpiTableLength = 0UL;
            result = SfdpFindParameterTableAddress(id, sfdpBuffer, sfdpAddress, &basicSpiTableLength);

            if (CY_SMIF_SUCCESS == result)
            { 
                CY_ASSERT_L1(NULL != device->readCmd);
                CY_ASSERT_L1(NULL != device->writeEnCmd);
                CY_ASSERT_L1(NULL != device->writeDisCmd);
                CY_ASSERT_L1(NULL != device->eraseCmd);
                CY_ASSERT_L1(NULL != device->chipEraseCmd);
                CY_ASSERT_L1(NULL != device->programCmd);
                CY_ASSERT_L1(NULL != device->readStsRegWipCmd);
        
                /* Get the JEDEC basic flash parameter table content into sfdpBuffer[] */
                result = SfdpReadBuffer(base,
                                       cmdSfdp,
                                       sfdpAddress,
                                       slaveSelect,
                                       basicSpiTableLength,
                                       sfdpBuffer,
                                       context);

                /* The erase size and erase time for all 4 erase types */
                SfdpGetEraseSizeAndCmd(sfdpBuffer, eraseType);

                /* The number of address bytes used by the memory slave device */
                device->numOfAddrBytes = SfdpGetNumOfAddrBytes(sfdpBuffer);

                /* The external memory size */
                device->memSize = SfdpGetMemoryDensity(sfdpBuffer);

                /* The page size */
                device->programSize = SfdpGetPageSize(sfdpBuffer);

                /* The Write Enable command */
                SfdpSetWriteEnableCommand(device->writeEnCmd);

                /* The Write Disable command */
                SfdpSetWriteDisableCommand(device->writeDisCmd); 

                /* The busy mask for the status registers */
                device->stsRegBusyMask = CY_SMIF_STATUS_REG_BUSY_MASK;

                /* The command to read the WIP-containing status register */
                SfdpSetWipStatusRegisterCommand(device->readStsRegWipCmd);

                /* The command to write into the QE-containing status register */
                SfdpGetQuadEnableParameters(device, sfdpBuffer);

                /* Chip Erase command */
                SfdpSetChipEraseCommand(device->chipEraseCmd);

                /* Chip Erase Time */
                device->chipEraseTime = SfdpGetChipEraseTime(sfdpBuffer);

                /* Page Program Time */
                device->programTime = SfdpGetPageProgramTime(sfdpBuffer);

                /* The Read command for 3-byte addressing. The preference order quad > dual > single SPI */
                cy_stc_smif_mem_cmd_t *cmdRead = device->readCmd;
                cy_en_smif_protocol_mode_t pMode = SfdpGetReadCmdParams(sfdpBuffer, dataSelect, cmdRead);
                
                /* Read, Erase, and Program commands */
                uint32_t eraseTypeOffset = 1UL;
                if (FOUR_BYTE_ADDRESS == device->numOfAddrBytes)
                {
                    /* Enter 4-byte addressing mode */
                    result = SfdpEnterFourByteAddressing(base, sfdpBuffer[CY_SMIF_SFDP_BFPT_BYTE_3C], device, slaveSelect, context);
                    uint8_t fourByteAddressBuffer[CY_SMIF_SFDP_LENGTH];
                    if (CY_SMIF_SUCCESS == result)
                    {
                        /* Get the JEDEC 4-byte Address Instruction Table content into sfdpBuffer[] */
                        result = SfdpReadBuffer(base, cmdSfdp, addr4ByteAddress, slaveSelect, 
                                                addr4ByteTableLength, fourByteAddressBuffer, context);
                    }
                    if (CY_SMIF_SUCCESS == result)
                    {
                        /* Rewrite the Read command instruction for 4-byte addressing mode */
                        SfdpGetReadFourBytesCmd(fourByteAddressBuffer, pMode, cmdRead);
                      
                        /* Get the Program command instruction for 4-byte addressing mode */
                        SfdpGetProgramFourBytesCmd(fourByteAddressBuffer, pMode, device->programCmd);
                        
                        /* Find the sector Erase command type with 4-byte addressing */
                        eraseTypeOffset = SfdpGetSectorEraseCommand(device, fourByteAddressBuffer, eraseType);
                    }
                }
                else
                {
                    /* The program command for 3-byte addressing mode */
                    SfdpSetProgramCommand_1_1_1(device->programCmd);

                    /* Find the sector Erase command type with 3-byte addressing */
                    eraseTypeOffset = SfdpGetSectorEraseCommand(device, sfdpBuffer, eraseType);
                }
                
                if (COMMAND_IS_NOT_FOUND != eraseTypeOffset)
                {
                    /* The Erase sector size (from the JEDEC basic flash parameter table) */
                    device->eraseSize = 0x01UL << sfdpBuffer[eraseTypeOffset - 1UL];

                    /* Erase Time Type (from the JEDEC basic flash parameter table) */
                    device->eraseTime = SfdpGetEraseTime(eraseTypeOffset, sfdpBuffer, eraseType);
                }

                if (NULL != device->hybridRegionInfo)
                {
                    /* Get the Sector Map Parameter Table into sfdpBuffer[] */
                    result = SfdpReadBuffer(base, cmdSfdp, sectorMapAddr, slaveSelect,
                                            sectorMapTableLength, sfdpBuffer, context);
                    if (CY_SMIF_SUCCESS == result)
                    {
                        result = SfdpPopulateRegionInfo(base, sfdpBuffer, sectorMapTableLength, device, slaveSelect, context, eraseType);
                        if(result == CY_SMIF_NOT_HYBRID_MEM)
                        {
                            device->hybridRegionCount = 0UL;
                            device->hybridRegionInfo = NULL;
                            result = CY_SMIF_SUCCESS;
                        }
                    }
                }
            }
        }
        else
        {
            result = CY_SMIF_NO_SFDP_SUPPORT;
        }
    }

    return(result);
}


/*******************************************************************************
* Function Name: Cy_SMIF_MemIsReady
****************************************************************************//**
*
* Polls the memory device to check whether it is ready to accept new commands or
* not until either it is ready or the retries have exceeded the limit. 
* This is a blocking function, it will block the execution flow until
* the command transmission is completed.
*
* \param base
* Holds the base address of the SMIF block registers.
*
* \param memConfig
* The memory device configuration.
*
* \param timeoutUs
*  The timeout value in microseconds to apply while polling the memory. 
*
* \param context
* This is the pointer to the context structure \ref cy_stc_smif_context_t
* allocated by the user. The structure is used during the SMIF
* operation for internal configuration and data retention. The user must not
* modify anything in this structure.
*
* \return The status of the operation.
* \ref CY_SMIF_SUCCESS        - Memory is ready to accept new commands.
* \ref CY_SMIF_EXCEED_TIMEOUT - Memory is busy.
*
* \funcusage 
* \snippet smif/snippet/main.c snippet_Cy_SMIF_MemIsReady
*
*******************************************************************************/
cy_en_smif_status_t Cy_SMIF_MemIsReady(SMIF_Type *base, cy_stc_smif_mem_config_t const *memConfig, 
                                       uint32_t timeoutUs, cy_stc_smif_context_t const *context)
{    
    uint32_t timeoutChunk;
    uint32_t timeoutSlice = 0UL;
    uint32_t microChunk; 
    bool isBusy;
    
    /* Calculate the slice of time to split the timeoutUs delay into TIMEOUT_SLICE_DIV times */
    timeoutSlice = timeoutUs / TIMEOUT_SLICE_DIV; 
    
    if(timeoutSlice == 0UL) 
    {
        timeoutSlice = 1UL; 
    }
    
    do
    {
        timeoutChunk = (timeoutUs > timeoutSlice) ? timeoutSlice : timeoutUs;
        timeoutUs -= timeoutChunk;
        
        do
        {
            /* Check for UINT16 overflow */
            microChunk = (timeoutChunk > (uint32_t)UINT16_MAX) ? (uint32_t)UINT16_MAX : timeoutChunk; 
            timeoutChunk -= microChunk; 
            Cy_SysLib_DelayUs((uint16_t)microChunk);
        } while(timeoutChunk > 0UL);
        
        isBusy = Cy_SMIF_Memslot_IsBusy(base, (cy_stc_smif_mem_config_t* )memConfig, context);
    } while(isBusy && (timeoutUs > 0UL));

    return (isBusy ? CY_SMIF_EXCEED_TIMEOUT : CY_SMIF_SUCCESS);
}


/*******************************************************************************
* Function Name: Cy_SMIF_MemIsQuadEnabled
****************************************************************************//**
*
* Checks whether the QE (Quad Enable) bit is set or not in the configuration
* register of the memory.
* This is a blocking function, it will block the execution flow until
* the command transmission is completed.
*
* \param base
* Holds the base address of the SMIF block registers.
*
* \param memConfig
* The memory device configuration.
*
* \param isQuadEnabled
* This parameter is updated to indicate whether quad mode is enabled (true) or
* not (false). The value is valid only when the function returns
* CY_SMIF_SUCCESS.
*
* \param context
* This is the pointer to the context structure \ref cy_stc_smif_context_t
* allocated by the user. The structure is used during the SMIF
* operation for internal configuration and data retention. The user must not
* modify anything in this structure.
*
* \return The status of the operation. See \ref cy_en_smif_status_t.
*
* \funcusage 
* \snippet smif/snippet/main.c snippet_Cy_SMIF_MemIsQuadEnabled
*
*******************************************************************************/
cy_en_smif_status_t Cy_SMIF_MemIsQuadEnabled(SMIF_Type *base, cy_stc_smif_mem_config_t const *memConfig,
                                             bool *isQuadEnabled, cy_stc_smif_context_t const *context)
{
    cy_en_smif_status_t status;
    uint8_t readStatus = 0U;
    CY_ASSERT_L1(NULL !=  memConfig);
    uint32_t statusCmd = memConfig->deviceCfg->readStsRegQeCmd->command;
    uint8_t maskQE = (uint8_t) memConfig->deviceCfg->stsRegQuadEnableMask;

    status = Cy_SMIF_MemCmdReadStatus(base, memConfig, &readStatus, (uint8_t)statusCmd, context);

    *isQuadEnabled = false;
    if(CY_SMIF_SUCCESS == status)
    {
        /* Check whether quad mode is already enabled or not */
        *isQuadEnabled = (maskQE == (readStatus & maskQE));
    }

    return status;
}


/*******************************************************************************
* Function Name: Cy_SMIF_MemEnableQuadMode
****************************************************************************//**
*
* Sets the QE (QUAD Enable) bit in the external memory
* configuration register to enable quad SPI mode.
* This is a blocking function, it will block the execution flow until
* the command transmission is completed.
*
* \param base
* Holds the base address of the SMIF block registers.
*
* \param memConfig
* The memory device configuration.
*
* \param timeoutUs
*  The timeout value in microseconds to apply while polling the memory. 
*
* \param context
* This is the pointer to the context structure \ref cy_stc_smif_context_t
* allocated by the user. The structure is used during the SMIF
* operation for internal configuration and data retention. The user must not
* modify anything in this structure.
*
* \return The status of the operation. See \ref cy_en_smif_status_t.
*
* \funcusage 
* See \ref Cy_SMIF_MemIsQuadEnabled usage.
*
*******************************************************************************/
cy_en_smif_status_t Cy_SMIF_MemEnableQuadMode(SMIF_Type *base, cy_stc_smif_mem_config_t const *memConfig, 
                                              uint32_t timeoutUs, cy_stc_smif_context_t const *context)
{
    cy_en_smif_status_t status;

    CY_ASSERT_L1(NULL !=  memConfig);

    /* Send Write Enable to the external memory */
    status = Cy_SMIF_MemCmdWriteEnable(base, (cy_stc_smif_mem_config_t* )memConfig, context);

    if(CY_SMIF_SUCCESS == status)
    {
        status = Cy_SMIF_MemQuadEnable(base, (cy_stc_smif_mem_config_t* )memConfig, context);

        if(CY_SMIF_SUCCESS == status)
        {
            /* Poll the memory for the completion of the operation */
            status = Cy_SMIF_MemIsReady(base, memConfig, timeoutUs, context);
        }
    }

    return status;
}


/*******************************************************************************
* Function Name: PollTransferStatus
****************************************************************************//**
*
* Polls the SMIF block until the transfer status equals the expected value or
* timeout occurs.
* This is a blocking function, it will block the execution flow until
* the command transmission is completed.
*
* \param base
* Holds the base address of the SMIF block registers.
*
* \param transferStatus
* Transfer status value to be checked.
*
* \param context
* This is the pointer to the context structure \ref cy_stc_smif_context_t
* allocated by the user. The structure is used during the SMIF
* operation for internal configuration and data retention. The user must not
* modify anything in this structure.
*
* \return The status of the operation.
* \ref CY_SMIF_SUCCESS           - SMIF block has completed the transfer
* \ref CY_SMIF_EXCEED_TIMEOUT - Timeout occurred.
*
*******************************************************************************/
static cy_en_smif_status_t PollTransferStatus(SMIF_Type const *base, cy_en_smif_txfr_status_t transferStatus,
                                              cy_stc_smif_context_t const *context)
{
    cy_en_smif_status_t status = CY_SMIF_SUCCESS;
    uint32_t transferTimeout = SMIF_TRANSFER_TIMEOUT;

    while (((uint32_t)transferStatus != Cy_SMIF_GetTransferStatus(base, context)) && (transferTimeout > 0UL))
    {
        Cy_SysLib_DelayUs(CY_SMIF_WAIT_1_UNIT);
        transferTimeout--;
    }

    if((uint32_t)transferStatus != Cy_SMIF_GetTransferStatus(base, context))
    {
        status = CY_SMIF_EXCEED_TIMEOUT;
    }

    return status;
}


/*******************************************************************************
* Function Name: ValueToByteArray
****************************************************************************//**
*
* Unpacks 0-numBytes from a 4-byte value into the byte array byteArray.
*
* \param value
*  The 4-byte value to unpack.
*
* \param byteArray
* The byte array to fill.
*
* \param startPos
* The start position of the array to begin filling from.
*
* \param size
* The size of the array.
*
*
*******************************************************************************/
static void ValueToByteArray(uint32_t value, uint8_t *byteArray, uint32_t startPos, uint32_t size)
{
    do
    {
        size--;
        byteArray[size + startPos] = (uint8_t)(value & PARAM_ID_LSB_MASK);
        value >>= PARAM_ID_MSB_OFFSET; /* Shift to get the next byte */
    } while (size > 0U);
}


/*******************************************************************************
* Function Name: ByteArrayToValue
****************************************************************************//**
*
* Packs the byte array into a single value.
*
* \param byteArray
* The byte array to unpack.
*
* \param size
* The size of the array.
*
* \return
* The 4-byte value filled from the array.
*
*
*******************************************************************************/
static uint32_t ByteArrayToValue(uint8_t const *byteArray, uint32_t size)
{
    uint32_t value = 0UL;
    uint32_t idx = 0UL;
    for (idx = 0UL; idx < size; idx++)
    {
        value <<= 8;
        value |= ((uint32_t) byteArray[idx]);
    }
    return value;
}


/*******************************************************************************
* Function Name: Cy_SMIF_MemRead
****************************************************************************//**
*
* Reads data from the external memory and blocks until the read
* transfer is complete or a timeout occurs.
* This is a blocking function, it will block the execution flow until
* the command transmission is completed.
*
* \param base
* Holds the base address of the SMIF block registers.
*
* \param memConfig
* The memory device configuration.
*
* \param address
* The address to read data from.
*
* \param rxBuffer
* The buffer for storing the read data.
*
* \param length
* The size of data to read.
*
* \param context
* This is the pointer to the context structure \ref cy_stc_smif_context_t
* allocated by the user. The structure is used during the SMIF
* operation for internal configuration and data retention. The user must not
* modify anything in this structure.
*
* \return The status of the operation. See \ref cy_en_smif_status_t.
*
* \funcusage 
* \snippet smif/snippet/main.c snippet_Cy_SMIF_MemRead
*
*******************************************************************************/
cy_en_smif_status_t Cy_SMIF_MemRead(SMIF_Type *base, cy_stc_smif_mem_config_t const *memConfig, 
                                    uint32_t address, uint8_t rxBuffer[], 
                                    uint32_t length, cy_stc_smif_context_t *context)
{
    cy_en_smif_status_t status = CY_SMIF_BAD_PARAM;
    uint32_t chunk = 0UL;
    uint8_t addrArray[CY_SMIF_FOUR_BYTES_ADDR] = {0U};

    CY_ASSERT_L1(NULL != memConfig);
    CY_ASSERT_L1(NULL != rxBuffer);

    if((address + length) <= memConfig->deviceCfg->memSize)  /* Check if the address exceeds the memory size */
    {
        /* SMIF can read only up to 65536 bytes in one go. Split the larger read into multiple chunks */
        while (length > 0UL)
        {
            /* Get the number of bytes which can be read during one operation */
            chunk = (length > SMIF_MAX_RX_COUNT) ? (SMIF_MAX_RX_COUNT) : length;
            ValueToByteArray(address, &addrArray[0], 0UL,
                             memConfig->deviceCfg->numOfAddrBytes);
            
            /* Send the command to read data from the external memory to the rxBuffer array */            
            status = Cy_SMIF_MemCmdRead(base, memConfig, (const uint8_t *)addrArray, 
                                        (uint8_t *)rxBuffer, chunk, NULL, context);
            
            if(CY_SMIF_SUCCESS == status)
            {
                /* Wait until the SMIF block completes receiving data */
                status = PollTransferStatus(base, CY_SMIF_REC_CMPLT, context);
            }

            if(CY_SMIF_SUCCESS != status)
            {
                break;  
            }
   
            /* Recalculate the next rxBuffer offset */
            length -= chunk;
            address += chunk;
            rxBuffer = (uint8_t *)rxBuffer + chunk;
        }  
    }

    return status;
}


/*******************************************************************************
* Function Name: Cy_SMIF_MemWrite
****************************************************************************//**
*
* This function writes data to the external memory.
* This is a blocking function, it will block the execution flow until
* the command transmission is completed.
*
* \param base
* Holds the base address of the SMIF block registers.
*
* \param memConfig
* The memory device configuration.
*
* \param address
* The address to write data at.
*
* \param txBuffer
* The buffer holding the data to write in the external memory.
*
* \param length
* The size of data to write.
*
* \param context
* This is the pointer to the context structure \ref cy_stc_smif_context_t
* allocated by the user. The structure is used during the SMIF
* operation for internal configuration and data retention. The user must not
* modify anything in this structure.
*
* \return The status of the operation. See \ref cy_en_smif_status_t.
*
* \funcusage 
* \snippet smif/snippet/main.c snippet_Cy_SMIF_MemWrite
*
*******************************************************************************/
cy_en_smif_status_t Cy_SMIF_MemWrite(SMIF_Type *base, cy_stc_smif_mem_config_t const *memConfig, 
                                     uint32_t address, uint8_t const txBuffer[], 
                                     uint32_t length, cy_stc_smif_context_t *context)
{
    cy_en_smif_status_t status = CY_SMIF_BAD_PARAM;
    uint32_t offset = 0UL;
    uint32_t chunk = 0UL;
    uint32_t pageSize; 
    uint8_t addrArray[CY_SMIF_FOUR_BYTES_ADDR] = {0U};

    CY_ASSERT_L1(NULL != memConfig);
    CY_ASSERT_L1(NULL != txBuffer);
    
    pageSize = memConfig->deviceCfg->programSize; /* Get the page size */

    if((address + length) <= memConfig->deviceCfg->memSize)  /* Check if the address exceeds the memory size */
    {
        /* SMIF can read only up to 65536 bytes in one go. Split the larger read into multiple chunks */
        while (length > 0UL)
        {
            /* Get the number of bytes which can be written during one operation */
            offset = address % pageSize; 
            chunk = ((offset + length) < pageSize) ? length : (pageSize - offset);

            /* The Write Enable bit may be cleared by the memory after every successful
             * operation of write or erase operations. Therefore, must be set for
             * every loop. 
             */
            status = Cy_SMIF_MemCmdWriteEnable(base, memConfig, context);

            if(CY_SMIF_SUCCESS == status)
            {
                ValueToByteArray(address, &addrArray[0], 0UL,
                                 memConfig->deviceCfg->numOfAddrBytes);

                /* Send the command to write data from the txBuffer array to the external memory */
                status = Cy_SMIF_MemCmdProgram(base, memConfig, (const uint8_t *)addrArray, 
                                               (uint8_t *)txBuffer, chunk, NULL, context);

                if(CY_SMIF_SUCCESS == status)
                {
                    /* Wait until the SMIF block completes transmitting data */
                    status = PollTransferStatus(base, CY_SMIF_SEND_CMPLT, context);
                    
                    if(CY_SMIF_SUCCESS == status)
                    {
                        /* Check if the memory has completed the write operation. ProgramTime is in microseconds */
                        status = Cy_SMIF_MemIsReady(base, memConfig, memConfig->deviceCfg->programTime, context);
                    }
                }                        
            }
            
            if(CY_SMIF_SUCCESS != status)
            {
                break;  
            }

            /* Recalculate the next rxBuffer offset */
            txBuffer = (uint8_t *)txBuffer + chunk;
            address += chunk;
            length -= chunk;
        }
    }

    return status;
}


/*******************************************************************************
* Function Name: Cy_SMIF_MemEraseSector
****************************************************************************//**
*
* Erases a block/sector of the external memory.
* This is a blocking function, it will block the execution flow until
* the command transmission is completed.
*
* \param base
* Holds the base address of the SMIF block registers.
*
* \param memConfig
* The memory device configuration.
*
* \param address
* The address of the block to be erased.
*
* \param length
* The size of data to erase.
*
* \param context
* This is the pointer to the context structure \ref cy_stc_smif_context_t
* allocated by the user. The structure is used during the SMIF
* operation for internal configuration and data retention. The user must not
* modify anything in this structure.
*
* \return The status of the operation. See \ref cy_en_smif_status_t.
*
* \note The address should be aligned with the start address of the sector. \n
* The length should be equal to the sum of all erased sectors.
*
* \funcusage 
* \snippet smif/snippet/main.c snippet_Cy_SMIF_MemEraseSector
*
*******************************************************************************/
cy_en_smif_status_t Cy_SMIF_MemEraseSector(SMIF_Type *base, cy_stc_smif_mem_config_t const *memConfig,
                                           uint32_t address, uint32_t length,
                                           cy_stc_smif_context_t const *context)
{
    cy_en_smif_status_t status = CY_SMIF_BAD_PARAM;
    uint32_t endAddress = address + length;
    uint32_t eraseEnd = 0UL;
    uint32_t hybridRegionStart = 0UL;
    uint8_t addrArray[CY_SMIF_FOUR_BYTES_ADDR] = {0U};
    cy_stc_smif_hybrid_region_info_t* hybrInfo = NULL;
    CY_ASSERT_L1(NULL != memConfig);
    cy_stc_smif_mem_device_cfg_t *device = memConfig->deviceCfg;
    uint32_t eraseSectorSize = device->eraseSize;
    uint32_t maxEraseTime = device->eraseTime;

    /* In case of hybrid memory - update sector size and offset for first sector */
    status = Cy_SMIF_MemLocateHybridRegion(memConfig, &hybrInfo, address);
    if (CY_SMIF_SUCCESS == status)
    {
        hybridRegionStart = hybrInfo->regionAddress;
        eraseSectorSize = hybrInfo->eraseSize;
        eraseEnd = (hybrInfo->sectorsCount * eraseSectorSize) + hybridRegionStart;
    }

    /* Check if the end address not equal to start address */
    if(length == 0UL)
    {
        status = CY_SMIF_BAD_PARAM;
    }

    /* Check if the start address and the sector size are aligned */
    if((0UL == ((address - hybridRegionStart) % eraseSectorSize)) && (status != CY_SMIF_BAD_PARAM))
    {
        /* If the memory is hybrid and there is more than one region to
         * erase - update the sector size and offset for the last sector */
        if(endAddress < eraseEnd)
        {
            status = Cy_SMIF_MemLocateHybridRegion(memConfig, &hybrInfo, (endAddress - 1UL));
            if (CY_SMIF_SUCCESS == status)
            {
                hybridRegionStart = hybrInfo->regionAddress;
                eraseSectorSize = hybrInfo->eraseSize;
            }
        }

        /* Check if the end address and the sector size are aligned */
        if((0UL == ((endAddress - hybridRegionStart) % eraseSectorSize))  && (status != CY_SMIF_BAD_PARAM))
        {
            while(length > 0UL)
            {
                /* In case of hybrid memory - update erase size and time for current region */
                status = Cy_SMIF_MemLocateHybridRegion(memConfig, &hybrInfo, address);
                if (CY_SMIF_SUCCESS == status)
                {
                    maxEraseTime =  hybrInfo->eraseTime;
                    eraseSectorSize = hybrInfo->eraseSize;
                    hybridRegionStart = hybrInfo->regionAddress;
                    eraseEnd = (hybrInfo->sectorsCount * eraseSectorSize) + hybridRegionStart;
                    if(endAddress < eraseEnd)
                    {
                        eraseEnd = endAddress;
                    }
                }
                else
                {
                    eraseEnd = endAddress;
                }

                while (address < eraseEnd)
                {
                    /* The Write Enable bit may be cleared by the memory after every successful
                    * operation of write/erase operations. Therefore, it must be set for
                    * every loop.
                    */
                    status = Cy_SMIF_MemCmdWriteEnable(base, memConfig, context);
                    if(CY_SMIF_SUCCESS == status)
                    {
                        ValueToByteArray(address, &addrArray[0], 0UL, device->numOfAddrBytes);

                        /* Send the command to erase one sector */
                        status = Cy_SMIF_MemCmdSectorErase(base, (cy_stc_smif_mem_config_t* )memConfig,
                                                           (const uint8_t *)addrArray, context);
                        if(CY_SMIF_SUCCESS == status)
                        {
                            /* Wait until the erase operation is completed or a timeout occurs.
                             * Note: eraseTime is in milliseconds */
                            status = Cy_SMIF_MemIsReady(base, memConfig, (maxEraseTime * ONE_MILLI_IN_MICRO), context);

                            /* Recalculate the next sector address offset */
                            address += eraseSectorSize;
                            length -= eraseSectorSize;
                        }
                    }

                    if(CY_SMIF_SUCCESS != status)
                    {
                        break;
                    }
                }
            }
        }
    }

    return status;
}


/*******************************************************************************
* Function Name: Cy_SMIF_MemEraseChip
****************************************************************************//**
*
* Erases the entire chip of the external memory.
* This is a blocking function, it will block the execution flow until
* the command transmission is completed.
*
* \param base
* Holds the base address of the SMIF block registers.
*
* \param memConfig
* The memory device configuration.
*
* \param context
* This is the pointer to the context structure \ref cy_stc_smif_context_t
* allocated by the user. The structure is used during the SMIF
* operation for internal configuration and data retention. The user must not
* modify anything in this structure.
*
* \return The status of the operation. See \ref cy_en_smif_status_t.
*
* \funcusage 
* \snippet smif/snippet/main.c snippet_Cy_SMIF_MemEraseChip
*
*******************************************************************************/
cy_en_smif_status_t Cy_SMIF_MemEraseChip(SMIF_Type *base, cy_stc_smif_mem_config_t const *memConfig,
                                         cy_stc_smif_context_t const *context)
{
    cy_en_smif_status_t status;

    CY_ASSERT_L1(NULL != memConfig);

    /* The Write Enable bit may be cleared by the memory after every successful
    * operation of write/erase operations. Therefore, it must be set for
    * every loop 
    */
    status = Cy_SMIF_MemCmdWriteEnable(base, memConfig, context);

    if(CY_SMIF_SUCCESS == status)
    {
        /* Send the command to erase the entire chip */
        status = Cy_SMIF_MemCmdChipErase(base, memConfig, context);
        
        if(CY_SMIF_SUCCESS == status)
        {
            /* Wait until the erase operation is completed or a timeout occurs. chipEraseTime is in milliseconds */
            status = Cy_SMIF_MemIsReady(base, memConfig, 
                     (memConfig->deviceCfg->chipEraseTime * ONE_MILLI_IN_MICRO), context);   
        }
    }

    return status;
}

#if defined(__cplusplus)
}
#endif

#endif /* CY_IP_MXSMIF */

/* [] END OF FILE */
