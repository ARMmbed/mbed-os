/***************************************************************************//**
* \file cy_smif_memslot.h
* \version 1.50
*
* \brief
*  This file provides the constants and parameter values for the memory-level
*  APIs of the SMIF driver.
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

#if !defined(CY_SMIF_MEMORYSLOT_H)
#define CY_SMIF_MEMORYSLOT_H

#include <stdint.h>
#include <stdbool.h>
#include "cy_syslib.h"
#include "cy_device_headers.h"
#include "cy_smif.h"

#ifdef CY_IP_MXSMIF

#if defined(__cplusplus)
extern "C" {
#endif


/**
* \addtogroup group_smif_macros_status
* \{
*/

/***************************************
*            Constants
****************************************/
#define CY_SMIF_DEVICE_BUSY        (1U)    /**< The external memory is busy */
#define CY_SMIF_DEVICE_READY       (0U)    /**< The external memory is ready */

/** \} group_smif_macros_status */

/**
* \addtogroup group_smif_macros_cmd
* \{
*/
#define CY_SMIF_CMD_WITHOUT_PARAM  (0U)    /**< No parameter */
#define CY_SMIF_TX_LAST_BYTE       (1U)    /**< The last byte in the command transmission
                                            * (SS is set high after the transmission)
                                            */
#define CY_SMIF_TX_NOT_LAST_BYTE   (0U)     /**< Not the last byte in the command transmission
                                            * (SS remains low after the transmission)
                                            */
#define CY_SMIF_READ_ONE_BYTE      (1U)     /**< Read 1 byte */
#define CY_SMIF_WRITE_ONE_BYTE     (1U)     /**< Write 1 byte */
#define CY_SMIF_WRITE_TWO_BYTES    (2U)     /**< Write 2 bytes */
#define CY_SMIF_ONE_WORD           (4U)     /**< 4 bytes */

#define CY_SMIF_DUAL_QUAD_DISABLED (0U)     /**< The dual quad transmission mode is disabled */
#define CY_SMIF_DUAL_QUAD_ENABLED  (1U)     /**< The dual quad transmission mode is enabled */


/** \} group_smif_macros_status */

/**
* \addtogroup group_smif_macros_flags
* \{
*/

#define CY_SMIF_FLAG_ALL_DISABLED       (0U) /**< All memory configuration flags are disabled */
/** Enables the write capability for the memory slave in the memory-mapped
 * mode. Valid when the memory-mapped mode is enabled */
#define CY_SMIF_FLAG_WRITE_ENABLE       (SMIF_DEVICE_CTL_WR_EN_Msk)
/** Determines if the device is memory-mapped. If enabled, this memory slot will
 * be initialized in System init */
#define CY_SMIF_FLAG_MEMORY_MAPPED      (2U)
#define CY_SMIF_FLAG_DETECT_SFDP        (4U) /**< Enables the Autodetect using the SFDP */
/** Enables the crypto support for this memory slave. All access to the
* memory device goes through the encryption/decryption
* Valid when the memory-mapped mode is enabled */
#define CY_SMIF_FLAG_CRYPTO_ENABLE      (SMIF_DEVICE_CTL_CRYPTO_EN_Msk)

/** \} group_smif_macros_flags */

/**
* \addtogroup group_smif_macros_sfdp
* \{
*/

/***************************************
*            SFDP constants
****************************************/
#define CY_SMIF_SFDP_ADDRESS_LENGTH                 (0x03U)                 /**< The length of the SFDP address */
#define CY_SMIF_SFDP_PARAM_HEADER_LENGTH            (0x8U)                  /**< The length of the Parameter header */
#define CY_SMIF_SFDP_PARAMETER_TABLE_LENGTH         (0x64U)                 /**< The length of the Parameter table */
#define CY_SMIF_SFDP_LENGTH                         (CY_SMIF_SFDP_PARAMETER_TABLE_LENGTH) /**< The length of the SFDP */
#define CY_SMIF_SFDP_SIGNATURE_BYTE_00              (0x00U)                 /**< The SFDP Signature byte 0x00. Should be "S" */
#define CY_SMIF_SFDP_SIGNATURE_BYTE_01              (0x01U)                 /**< The SFDP Signature byte 0x01. Should be "F" */
#define CY_SMIF_SFDP_SIGNATURE_BYTE_02              (0x02U)                 /**< The SFDP Signature byte 0x02. Should be "D" */
#define CY_SMIF_SFDP_SIGNATURE_BYTE_03              (0x03U)                 /**< The SFDP Signature byte 0x03. Should be "P" */
#define CY_SMIF_SFDP_MINOR_REV                      (0x04U)                 /**< The SFDP Header byte 0x04. Defines the JEDEC JESD216 Revision */
#define CY_SMIF_SFDP_MAJOR_REV                      (0x05U)                 /**< The SFDP Header byte 0x05. Defines the SFDP Major Revision */
#define CY_SMIF_SFDP_MAJOR_REV_1                    (0x01U)                 /**< The SFDP Major Revision is 1 */
#define CY_SMIF_SFDP_JEDEC_REV_B                    (0x06U)                 /**< The JEDEC JESD216 Revision is B */
#define CY_SMIF_SFDP_PARAM_TABLE_PTR                (0x0CU)                 /**< Specifies the start of the JEDEC Basic Flash
                                                                            * Parameter Table in the SFDP structure
                                                                            */
#define CY_SMIF_SFDP_THREE_BYTES_ADDR_CODE          (0x00U)                 /**< Code for the SFDP Address Bytes Number 3 */
#define CY_SMIF_SFDP_THREE_OR_FOUR_BYTES_ADDR_CODE  (0x01U)                 /**< Code for the SFDP Address Bytes Number 3 or 4 */
#define CY_SMIF_SFDP_FOUR_BYTES_ADDR_CODE           (0x02U)                 /**< Code for the SFDP Address Bytes Number 4 */
#define CY_SMIF_THREE_BYTES_ADDR                    (0x03U)                 /**< The address Bytes Number is 3 */
#define CY_SMIF_FOUR_BYTES_ADDR                     (0x04U)                 /**< The address Bytes Number is 4 */
#define CY_SMIF_READ_MODE_BYTE                      (0x5AU)                 /**< The mode byte for the SMIF read */
#define CY_SMIF_WRITE_STATUS_REG1_CMD               (0x01U)                 /**< The write status register 1 command */
#define CY_SMIF_SINGLE_PROGRAM_CMD                  (0x02U)                 /**< The command for a single SMIF program */
#define CY_SMIF_SINGLE_READ_CMD                     (0x03U)                 /**< The command for a single SMIF read */
#define CY_SMIF_WRITE_DISABLE_CMD                   (0x04U)                 /**< The Write Disable command */
#define CY_SMIF_READ_STATUS_REG1_CMD                (0x05U)                 /**< The read status register 1 command */
#define CY_SMIF_WRITE_ENABLE_CMD                    (0x06U)                 /**< The Write Enable command */
#define CY_SMIF_READ_STATUS_REG2_T1_CMD             (0x35U)                 /**< The read status register 2 type 1 command */
#define CY_SMIF_WRITE_STATUS_REG2_CMD               (0x3EU)                 /**< The write status register 2 command */
#define CY_SMIF_READ_STATUS_REG2_T2_CMD             (0x3FU)                 /**< The read status register 2 type 2 command */
#define CY_SMIF_CHIP_ERASE_CMD                      (0x60U)                 /**< The Chip Erase command */
#define CY_SMIF_QE_BIT_STATUS_REG2_T1               (0x02U)                 /**< The QE bit is in status register 2 type 1.
                                                                            * It should be written as the second byte.
                                                                            */
#define CY_SMIF_FAST_READ_4_BYTES_CMD_1S_1S_1S      (0x0CU)                 /**< The command for a 1S-1S-1S SMIF fast read with 4-byte addressing */ 
#define CY_SMIF_FAST_READ_4_BYTES_CMD_1S_1S_2S      (0x3CU)                 /**< The command for a 1S-1S-2S SMIF fast read with 4-byte addressing */
#define CY_SMIF_FAST_READ_4_BYTES_CMD_1S_2S_2S      (0xBCU)                 /**< The command for a 1S-2S-2S SMIF fast read with 4-byte addressing */
#define CY_SMIF_FAST_READ_4_BYTES_CMD_1S_1S_4S      (0x6CU)                 /**< The command for a 1S-1S-4S SMIF fast read with 4-byte addressing */
#define CY_SMIF_FAST_READ_4_BYTES_CMD_1S_4S_4S      (0xECU)                 /**< The command for a 1S-4S-4S SMIF fast read with 4-byte addressing */

#define CY_SMIF_PAGE_PROGRAM_4_BYTES_CMD_1S_1S_1S   (0x12U)                 /**< The command for a 1S-1S-1S SMIF page program with 4-byte addressing */
#define CY_SMIF_PAGE_PROGRAM_4_BYTES_CMD_1S_1S_4S   (0x34U)                 /**< The command for a 1S-1S-4S SMIF page program with 4-byte addressing */
#define CY_SMIF_PAGE_PROGRAM_4_BYTES_CMD_1S_4S_4S   (0x3EU)                 /**< The command for a 1S-4S-4S SMIF page program with 4-byte addressing */

                                                                        
#define CY_SMIF_BRWR_EXTADD_MASK                    (0x80U)                 /**< The Extended Address Enable (EXTADD) mask */                                               
                                                                            
#define CY_SMIF_SFDP_ERASE_TIME_1MS                 (1U)                    /**< Units of Erase Typical Time in ms */
#define CY_SMIF_SFDP_ERASE_TIME_16MS                (16U)                   /**< Units of Erase Typical Time in ms */
#define CY_SMIF_SFDP_ERASE_TIME_128MS               (128U)                  /**< Units of Erase Typical Time in ms */
#define CY_SMIF_SFDP_ERASE_TIME_1S                  (1000U)                 /**< Units of Erase Typical Time in ms */
            
#define CY_SMIF_SFDP_CHIP_ERASE_TIME_16MS           (16U)                   /**< Units of Chip Erase Typical Time in ms */
#define CY_SMIF_SFDP_CHIP_ERASE_TIME_256MS          (256U)                  /**< Units of Chip Erase Typical Time in ms */
#define CY_SMIF_SFDP_CHIP_ERASE_TIME_4S             (4000U)                 /**< Units of Chip Erase Typical Time in ms */
#define CY_SMIF_SFDP_CHIP_ERASE_TIME_64S            (64000U)                /**< Units of Chip Erase Typical Time in ms */
            
#define CY_SMIF_SFDP_PROG_TIME_8US                  (8U)                    /**< Units of Page Program Typical Time in us */
#define CY_SMIF_SFDP_PROG_TIME_64US                 (64U)                   /**< Units of Page Program Typical Time in us */
            
#define CY_SMIF_SFDP_UNIT_0                         (0U)                    /**< Units of Basic Flash Parameter Table Time Parameters */
#define CY_SMIF_SFDP_UNIT_1                         (1U)                    /**< Units of Basic Flash Parameter Table Time Parameters */
#define CY_SMIF_SFDP_UNIT_2                         (2U)                    /**< Units of Basic Flash Parameter Table Time Parameters */
#define CY_SMIF_SFDP_UNIT_3                         (3U)                    /**< Units of Basic Flash Parameter Table Time Parameters */


#define CY_SMIF_STATUS_REG_BUSY_MASK                (0x01U)                 /**< The busy mask for the status registers */
#define CY_SMIF_NO_COMMAND_OR_MODE                  (0xFFFFFFFFUL)          /**< No command or mode present */
#define CY_SMIF_SFDP_QER_0                          (0x00UL)                /**< The quad Enable Requirements case 0 */
#define CY_SMIF_SFDP_QER_1                          (0x01UL)                /**< The quad Enable Requirements case 1 */
#define CY_SMIF_SFDP_QER_2                          (0x02UL)                /**< The quad Enable Requirements case 2 */
#define CY_SMIF_SFDP_QER_3                          (0x03UL)                /**< The quad Enable Requirements case 3 */
#define CY_SMIF_SFDP_QER_4                          (0x04UL)                /**< The quad Enable Requirements case 4 */
#define CY_SMIF_SFDP_QER_5                          (0x05UL)                /**< The quad Enable Requirements case 5 */
#define CY_SMIF_SFDP_QE_BIT_1_OF_SR_2               (0x02UL)                /**< The QE is bit 1 of the status register 2 */
#define CY_SMIF_SFDP_QE_BIT_6_OF_SR_1               (0x40UL)                /**< The QE is bit 6 of the status register 1 */
#define CY_SMIF_SFDP_QE_BIT_7_OF_SR_2               (0x80UL)                /**< The QE is bit 7 of the status register 2 */
#define CY_SMIF_SFDP_BFPT_BYTE_02                   (0x02U)                 /**< The byte 0x02 of the JEDEC Basic Flash Parameter Table */
#define CY_SMIF_SFDP_BFPT_BYTE_04                   (0x04U)                 /**< The byte 0x04 of the JEDEC Basic Flash Parameter Table */
#define CY_SMIF_SFDP_BFPT_BYTE_05                   (0x05U)                 /**< The byte 0x05 of the JEDEC Basic Flash Parameter Table */
#define CY_SMIF_SFDP_BFPT_BYTE_06                   (0x06U)                 /**< The byte 0x06 of the JEDEC Basic Flash Parameter Table:
                                                                             * number of Parameter Headers (zero based, 05h = 6 parameters)
                                                                             */
#define CY_SMIF_SFDP_BFPT_BYTE_08                   (0x08U)                 /**< The byte 0x08 of the JEDEC Basic Flash Parameter Table */
#define CY_SMIF_SFDP_BFPT_BYTE_09                   (0x09U)                 /**< The byte 0x09 of the JEDEC Basic Flash Parameter Table */
#define CY_SMIF_SFDP_BFPT_BYTE_0A                   (0x0AU)                 /**< The byte 0x0A of the JEDEC Basic Flash Parameter Table */
#define CY_SMIF_SFDP_BFPT_BYTE_0B                   (0x0BU)                 /**< The byte 0x0B of the JEDEC Basic Flash Parameter Table */
#define CY_SMIF_SFDP_BFPT_BYTE_0C                   (0x0CU)                 /**< The byte 0x0C of the JEDEC Basic Flash Parameter Table */
#define CY_SMIF_SFDP_BFPT_BYTE_0D                   (0x0DU)                 /**< The byte 0x0D of the JEDEC Basic Flash Parameter Table */
#define CY_SMIF_SFDP_BFPT_BYTE_0E                   (0x0EU)                 /**< The byte 0x0E of the JEDEC Basic Flash Parameter Table */
#define CY_SMIF_SFDP_BFPT_BYTE_0F                   (0x0FU)                 /**< The byte 0x0F of the JEDEC Basic Flash Parameter Table */
#define CY_SMIF_SFDP_BFPT_BYTE_1C                   (0x1CU)                 /**< The byte 0x1C of the JEDEC Basic Flash Parameter Table */
#define CY_SMIF_SFDP_BFPT_BYTE_1D                   (0x1DU)                 /**< The byte 0x1D of the JEDEC Basic Flash Parameter Table */
#define CY_SMIF_SFDP_BFPT_BYTE_23                   (0x23U)                 /**< The byte 0x23 of the JEDEC Basic Flash Parameter Table */
#define CY_SMIF_SFDP_BFPT_BYTE_28                   (0x28U)                 /**< The byte 0x28 of the JEDEC Basic Flash Parameter Table */
#define CY_SMIF_SFDP_BFPT_BYTE_3A                   (0x3AU)                 /**< The byte 0x3A of the JEDEC Basic Flash Parameter Table */
#define CY_SMIF_SFDP_BFPT_BYTE_3C                   (0x3CU)                 /**< The byte 0x3C of the JEDEC Basic Flash Parameter Table */
#define CY_SMIF_SFDP_BFPT_ERASE_BYTE                (36U)                   /**< The byte 36 of the JEDEC Basic Flash Parameter Table */

#define CY_SMIF_JEDEC_BFPT_10TH_DWORD               (9U)                    /**< Offset to JEDEC Basic Flash Parameter Table: 10th DWORD  */
#define CY_SMIF_JEDEC_BFPT_11TH_DWORD               (10U)                   /**< Offset to JEDEC Basic Flash Parameter Table: 11th DWORD  */


#define CY_SMIF_SFDP_SECTOR_MAP_CMD_OFFSET          (1UL)    /**< The offset for the detection command instruction in the Sector Map command descriptor */
#define CY_SMIF_SFDP_SECTOR_MAP_ADDR_CODE_OFFSET    (2UL)    /**< The offset for the detection command address length in the Sector Map command descriptor */
#define CY_SMIF_SFDP_SECTOR_MAP_REG_MSK_OFFSET      (3UL)    /**< The offset for the read data mask in the Sector Map command descriptor */
#define CY_SMIF_SFDP_SECTOR_MAP_REG_ADDR_OFFSET     (4UL)    /**< The offset for the detection command address in the Sector Map command descriptor */
#define CY_SMIF_SFDP_SECTOR_MAP_REGION_COUNT_OFFSET (2UL)    /**< The offset for the regions count in the Sector Map descriptor */
#define CY_SMIF_SFDP_SECTOR_MAP_CONFIG_ID_OFFSET    (2UL)    /**< The offset for the configuration ID in the Sector Map descriptor */
#define CY_SMIF_SFDP_SECTOR_MAP_SUPPORTED_ET_MASK   (0xFU)   /**< The mask for the supported erase type code in the Sector Map descriptor */
#define CY_SMIF_SFDP_SECTOR_MAP_ADDR_BYTES_Msk      (0xC0UL) /**< The mask for the configuration detection command address bytes in the Sector Map descriptor */
#define CY_SMIF_SFDP_SECTOR_MAP_ADDR_BYTES_Pos      (6UL)    /**< The position of the configuration detection command address bytes in the Sector Map descriptor */


/* ----------------------------  1st DWORD  ---------------------------- */
#define CY_SMIF_SFDP_FAST_READ_1_1_4_Pos            (6UL)                   /**< The SFDP 1-1-4 fast read support (Bit 6)               */
#define CY_SMIF_SFDP_FAST_READ_1_1_4_Msk            (0x40UL)                /**< The SFDP 1-1-4 fast read support (Bitfield-Mask: 0x01) */
#define CY_SMIF_SFDP_FAST_READ_1_4_4_Pos            (5UL)                   /**< The SFDP 1-4-4 fast read support (Bit 5)               */
#define CY_SMIF_SFDP_FAST_READ_1_4_4_Msk            (0x20UL)                /**< The SFDP 1-4-4 fast read support (Bitfield-Mask: 0x01) */
#define CY_SMIF_SFDP_FAST_READ_1_2_2_Pos            (4UL)                   /**< The SFDP 1-2-2 fast read support (Bit 4)               */
#define CY_SMIF_SFDP_FAST_READ_1_2_2_Msk            (0x10UL)                /**< The SFDP 1-2-2 fast read support (Bitfield-Mask: 0x01) */
#define CY_SMIF_SFDP_ADDRESS_BYTES_Pos              (1UL)                   /**< The SFDP number of address bytes (Bit 1)               */
#define CY_SMIF_SFDP_ADDRESS_BYTES_Msk              (0x06UL)                /**< The SFDP number of address bytes (Bitfield-Mask: 0x03) */
#define CY_SMIF_SFDP_FAST_READ_1_1_2_Pos            (0UL)                   /**< The SFDP 1-1-2 fast read support (Bit 0)               */
#define CY_SMIF_SFDP_FAST_READ_1_1_2_Msk            (0x01UL)                /**< The SFDP 1-1-2 fast read support (Bitfield-Mask: 0x01) */

/* ----------------------------  2nd DWORD  ---------------------------- */
#define CY_SMIF_SFDP_SIZE_ABOVE_4GB_Msk             (0x80000000UL)          /**< Flash memory density bit define if it >= 4 Gbit  or <= 2Gbit*/

/* ----------------------------  3rd DWORD  ---------------------------- */
#define CY_SMIF_SFDP_1_4_4_DUMMY_CYCLES_Pos         (0UL)                   /**< The SFDP number of 1-4-4 fast read dummy cycles (Bit 0)               */
#define CY_SMIF_SFDP_1_4_4_DUMMY_CYCLES_Msk         (0x1FUL)                /**< The SFDP number of 1-4-4 fast read dummy cycles (Bitfield-Mask: 0x1F) */
#define CY_SMIF_SFDP_1_4_4_MODE_CYCLES_Pos          (5UL)                   /**< The SFDP number of 1-4-4 fast read mode cycles (Bit 5)                */
#define CY_SMIF_SFDP_1_4_4_MODE_CYCLES_Msk          (0xE0UL)                /**< The SFDP number of 1-4-4 fast read mode cycles (Bitfield-Mask: 0x07)  */
#define CY_SMIF_SFDP_1_1_4_DUMMY_CYCLES_Pos         (0UL)                   /**< The SFDP number of 1-1-4 fast read dummy cycles (Bit 0)               */
#define CY_SMIF_SFDP_1_1_4_DUMMY_CYCLES_Msk         (0x1FUL)                /**< The SFDP number of 1-1-4 fast read dummy cycles (Bitfield-Mask: 0x1F) */
#define CY_SMIF_SFDP_1_1_4_MODE_CYCLES_Pos          (5UL)                   /**< The SFDP number of 1-1-4 fast read mode cycles (Bit 5)                */
#define CY_SMIF_SFDP_1_1_4_MODE_CYCLES_Msk          (0xE0UL)                /**< The SFDP number of 1-1-4 fast read mode cycles (Bitfield-Mask: 0x07)  */

/* ----------------------------  4th DWORD  ---------------------------- */
#define CY_SMIF_SFDP_1_1_2_DUMMY_CYCLES_Pos         (0UL)                   /**< The SFDP number of 1_1_2 fast read dummy cycles (Bit 0)               */
#define CY_SMIF_SFDP_1_1_2_DUMMY_CYCLES_Msk         (0x1FUL)                /**< The SFDP number of 1_1_2 fast read dummy cycles (Bitfield-Mask: 0x1F) */
#define CY_SMIF_SFDP_1_1_2_MODE_CYCLES_Pos          (5UL)                   /**< The SFDP number of 1_1_2 fast read mode cycles (Bit 5)                */
#define CY_SMIF_SFDP_1_1_2_MODE_CYCLES_Msk          (0xE0UL)                /**< The SFDP number of 1_1_2 fast read mode cycles (Bitfield-Mask: 0x07)  */
#define CY_SMIF_SFDP_1_2_2_DUMMY_CYCLES_Pos         (0UL)                   /**< The SFDP number of 1_2_2 fast read dummy cycles (Bit 0)               */
#define CY_SMIF_SFDP_1_2_2_DUMMY_CYCLES_Msk         (0x1FUL)                /**< The SFDP number of 1_2_2 fast read dummy cycles (Bitfield-Mask: 0x1F) */
#define CY_SMIF_SFDP_1_2_2_MODE_CYCLES_Pos          (5UL)                   /**< The SFDP number of 1_2_2 fast read mode cycles (Bit 5)                */
#define CY_SMIF_SFDP_1_2_2_MODE_CYCLES_Msk          (0xE0UL)                /**< The SFDP number of 1_2_2 fast read mode cycles (Bitfield-Mask: 0x07)  */

/* ----------------------------  10th DWORD  --------------------------- */
#define CY_SMIF_SFDP_ERASE_T1_COUNT_Pos             (4UL)                   /**< Erase Type 1 Erase, Typical time: count (Bits 8:4) */
#define CY_SMIF_SFDP_ERASE_T1_COUNT_Msk             (0x1F0UL)               /**< Erase Type 1 Erase, Typical time: count (Bitfield-Mask ) */
#define CY_SMIF_SFDP_ERASE_T1_UNITS_Pos             (9UL)                   /**< Erase Type 1 Erase, Typical time: units (Bits 10:9) */
#define CY_SMIF_SFDP_ERASE_T1_UNITS_Msk             (0x600UL)               /**< Erase Type 1 Erase, Typical time: units (Bitfield-Mask ) */
#define CY_SMIF_SFDP_ERASE_MUL_COUNT_Pos            (0UL)                   /**< Multiplier from typical erase time to maximum erase time (Bits 3:0) */
#define CY_SMIF_SFDP_ERASE_MUL_COUNT_Msk            (0x0FUL)                /**< Multiplier from typical erase time to maximum erase time (Bitfield-Mask ) */


/* ----------------------------  11th DWORD  --------------------------- */
#define CY_SMIF_SFDP_PAGE_SIZE_Pos                  (4UL)                   /**< The SFDP page size (Bit 4)                                    */
#define CY_SMIF_SFDP_PAGE_SIZE_Msk                  (0xF0UL)                /**< The SFDP page size (Bitfield-Mask: 0x0F)                      */
#define CY_SMIF_SFDP_PAGE_PROG_COUNT_Pos            (8UL)                   /**< The SFDP Chip Page Program Typical time: count (Bits 12:8)    */
#define CY_SMIF_SFDP_PAGE_PROG_COUNT_Msk            (0x1F00UL)              /**< The SFDP Chip Page Program Typical time: count (Bitfield-Mask)*/
#define CY_SMIF_SFDP_PAGE_PROG_UNITS_Pos            (13UL)                  /**< The SFDP Chip Page Program Typical time: units (Bit 13)       */
#define CY_SMIF_SFDP_PAGE_PROG_UNITS_Msk            (0x2000UL)              /**< The SFDP Chip Page Program Typical time: units (Bitfield-Mask)*/
#define CY_SMIF_SFDP_CHIP_ERASE_COUNT_Pos           (24UL)                  /**< The SFDP Chip Erase Typical time: count (Bits 28:24)          */
#define CY_SMIF_SFDP_CHIP_ERASE_COUNT_Msk           (0x1F000000UL)          /**< The SFDP Chip Erase Typical time: count (Bitfield-Mask)       */
#define CY_SMIF_SFDP_CHIP_ERASE_UNITS_Pos           (29UL)                  /**< The SFDP Chip Erase Typical time: units (Bits 29:30)          */
#define CY_SMIF_SFDP_CHIP_ERASE_UNITS_Msk           (0x60000000UL)          /**< The SFDP Chip Erase Typical time: units (Bitfield-Mask)       */
#define CY_SMIF_SFDP_PROG_MUL_COUNT_Pos             (0UL)                   /**< Multiplier from typical time to max time for Page or byte program (Bits 3:0)          */
#define CY_SMIF_SFDP_PROG_MUL_COUNT_Msk             (0x0FUL)                /**< Multiplier from typical time to max time for Page or byte program (Bitfield-Mask)       */

/* ----------------------------  15th DWORD  --------------------------- */
#define CY_SMIF_SFDP_QE_REQUIREMENTS_Pos            (4UL)                   /**< The SFDP quad enable requirements field (Bit 4)               */
#define CY_SMIF_SFDP_QE_REQUIREMENTS_Msk            (0x70UL)                /**< The SFDP quad enable requirements field (Bitfield-Mask: 0x07) */


/* ----------------------------  16th DWORD  --------------------------- */
#define CY_SMIF_SFDP_ENTER_4_BYTE_METHOD_B7 (1U)                            /**< Issue 0xB7 instruction */
#define CY_SMIF_SFDP_ENTER_4_BYTE_METHOD_WR_EN_B7 (2U)                      /**< Issue write enable instruction followed with 0xB7 */
#define CY_SMIF_SFDP_ENTER_4_BYTE_METHOD_ALWAYS_4_BYTE (0x40U)              /**< Memory always operates in 4-byte mode */
#define CY_SMIF_SFDP_ENTER_4_BYTE_METHOD_B7_CMD (0xB7U)                     /**< The instruction required to enter 4-byte addressing mode */


/** \cond INTERNAL */
/*******************************************************************************
* These are legacy constants and API. They are left here just 
* for backward compatibility.
* Do not use them in new designs.
*******************************************************************************/

#define CY_SMIF_FLAG_WR_EN                          CY_SMIF_FLAG_WRITE_ENABLE
#define CY_SMIF_FLAG_CRYPTO_EN                      CY_SMIF_FLAG_CRYPTO_ENABLE
#define CY_SMIF_SFDP_SING_BYTE_00                   CY_SMIF_SFDP_SIGNATURE_BYTE_00
#define CY_SMIF_SFDP_SING_BYTE_01                   CY_SMIF_SFDP_SIGNATURE_BYTE_01
#define CY_SMIF_SFDP_SING_BYTE_02                   CY_SMIF_SFDP_SIGNATURE_BYTE_02
#define CY_SMIF_SFDP_SING_BYTE_03                   CY_SMIF_SFDP_SIGNATURE_BYTE_03
#define CY_SMIF_WR_STS_REG1_CMD                     CY_SMIF_WRITE_STATUS_REG1_CMD
#define CY_SMIF_WR_DISABLE_CMD                      CY_SMIF_WRITE_DISABLE_CMD
#define CY_SMIF_RD_STS_REG1_CMD                     CY_SMIF_READ_STATUS_REG1_CMD
#define CY_SMIF_WR_ENABLE_CMD                       CY_SMIF_WRITE_ENABLE_CMD
#define CY_SMIF_RD_STS_REG2_T1_CMD                  CY_SMIF_READ_STATUS_REG2_T1_CMD          
#define CY_SMIF_WR_STS_REG2_CMD                     CY_SMIF_WRITE_STATUS_REG2_CMD
#define CY_SMIF_RD_STS_REG2_T2_CMD                  CY_SMIF_READ_STATUS_REG2_T2_CMD
#define CY_SMIF_QE_BIT_STS_REG2_T1                  CY_SMIF_QE_BIT_STATUS_REG2_T1
#define CY_SMIF_STS_REG_BUSY_MASK                   CY_SMIF_STATUS_REG_BUSY_MASK

#define Cy_SMIF_Memslot_Init                        Cy_SMIF_MemInit
#define Cy_SMIF_Memslot_DeInit                      Cy_SMIF_MemDeInit
#define Cy_SMIF_Memslot_CmdWriteEnable              Cy_SMIF_MemCmdWriteEnable
#define Cy_SMIF_Memslot_CmdWriteDisable             Cy_SMIF_MemCmdWriteDisable        
#define Cy_SMIF_Memslot_IsBusy                      Cy_SMIF_MemIsBusy  
#define Cy_SMIF_Memslot_QuadEnable                  Cy_SMIF_MemQuadEnable 
#define Cy_SMIF_Memslot_CmdReadSts                  Cy_SMIF_MemCmdReadStatus 
#define Cy_SMIF_Memslot_CmdWriteSts                 Cy_SMIF_MemCmdWriteStatus
#define Cy_SMIF_Memslot_CmdChipErase                Cy_SMIF_MemCmdChipErase
#define Cy_SMIF_Memslot_CmdSectorErase              Cy_SMIF_MemCmdSectorErase
#define Cy_SMIF_Memslot_SfdpDetect                  Cy_SMIF_MemSfdpDetect
#define Cy_SMIF_Memslot_CmdProgram                  Cy_SMIF_MemCmdProgram
#define Cy_SMIF_Memslot_CmdRead                     Cy_SMIF_MemCmdRead 

/** \endcond*/
/** \} group_smif_macros_sfdp */


/**
* \addtogroup group_smif_data_structures_memslot
* \{
*/

/** This command structure is used to store the Read/Write command
 * configuration. */
typedef struct
{
    uint32_t command;                       /**< The 8-bit command. This value is 0xFFFFFFFF when there is no command present */
    cy_en_smif_txfr_width_t cmdWidth;       /**< The width of the command transfer */
    cy_en_smif_txfr_width_t addrWidth;      /**< The width of the address transfer */
    uint32_t mode;                          /**< The 8-bit mode byte. This value is 0xFFFFFFFF when there is no mode present */
    cy_en_smif_txfr_width_t modeWidth;      /**< The width of the mode transfer */
    uint32_t dummyCycles;                   /**< The number of the dummy cycles. A zero value suggests no dummy cycles */
    cy_en_smif_txfr_width_t dataWidth;      /**< The width of the data transfer */
} cy_stc_smif_mem_cmd_t;

/** This structure specifies data used for memory with hybrid sectors */
typedef struct
{
    uint32_t regionAddress;                 /**< This specifies the address where a region starts */
    uint32_t sectorsCount;                  /**< This specifies the number of sectors in the region */
    uint32_t eraseCmd;                      /**< This specifies the region specific erase instruction*/
    uint32_t eraseSize;                     /**< This specifies the size of one sector */
    uint32_t eraseTime;                     /**< Max time for sector erase type 1 cycle time in ms*/
} cy_stc_smif_hybrid_region_info_t;


/**
*
* This configuration structure of the SMIF memory device is used to store
* device-specific parameters.
* These parameters are used to set up the memory mode initialization and the
* memory API.
*/
typedef struct
{
    uint32_t numOfAddrBytes;                              /**< This specifies the number of address bytes used by the 
                                                          * memory slave device, valid values 1-4 */
    uint32_t memSize;                                     /**< The memory size: For densities of 2 gigabits or less - the size in bytes;
                                                          * For densities 4 gigabits and above - bit-31 is set to 1b to define that
                                                          * this memory is 4 gigabits and above; and other 30:0 bits define N where 
                                                          * the density is computed as 2^N bytes. 
                                                          * For example, 0x80000021 corresponds to 2^30 = 1 gigabyte.
                                                          */
    cy_stc_smif_mem_cmd_t* readCmd;                       /**< This specifies the Read command */
    cy_stc_smif_mem_cmd_t* writeEnCmd;                    /**< This specifies the Write Enable command */
    cy_stc_smif_mem_cmd_t* writeDisCmd;                   /**< This specifies the Write Disable command */
    cy_stc_smif_mem_cmd_t* eraseCmd;                      /**< This specifies the Erase command */
    uint32_t eraseSize;                                   /**< This specifies the sector size of each Erase */
    cy_stc_smif_mem_cmd_t* chipEraseCmd;                  /**< This specifies the Chip Erase command */
    cy_stc_smif_mem_cmd_t* programCmd;                    /**< This specifies the Program command */
    uint32_t programSize;                                 /**< This specifies the page size for programming */
    cy_stc_smif_mem_cmd_t* readStsRegWipCmd;              /**< This specifies the command to read the WIP-containing status register  */
    cy_stc_smif_mem_cmd_t* readStsRegQeCmd;               /**< This specifies the command to read the QE-containing status register */
    cy_stc_smif_mem_cmd_t* writeStsRegQeCmd;              /**< This specifies the command to write into the QE-containing status register */
    cy_stc_smif_mem_cmd_t* readSfdpCmd;                   /**< This specifies the read SFDP command */
    uint32_t stsRegBusyMask;                              /**< The Busy mask for the status registers */
    uint32_t stsRegQuadEnableMask;                        /**< The QE mask for the status registers */
    uint32_t eraseTime;                                   /**< Max time for erase type 1 cycle time in ms */
    uint32_t chipEraseTime;                               /**< Max time for chip erase cycle time in ms */
    uint32_t programTime;                                 /**< Max time for page program cycle time in us */
    uint32_t hybridRegionCount;                           /**< This specifies the number of regions for memory with hybrid sectors */
    cy_stc_smif_hybrid_region_info_t** hybridRegionInfo;  /**< This specifies data for memory with hybrid sectors */
} cy_stc_smif_mem_device_cfg_t;

 
/**
*
* This SMIF memory configuration structure is used to store the memory configuration for the memory mode of operation.
* This data structure is stored in a fixed location in the flash. The data structure is required
* for the initialization of the SMIF in the SystemInit.
*/
typedef struct
{
    /** Determines the slave select where the memory device is placed */
    cy_en_smif_slave_select_t slaveSelect;
    /** Determines if the device is memory-mapped, enables the Autodetect
     * using the SFDP, enables the write capability, or enables the crypto
     * support for this memory slave */
    uint32_t flags;
    /** The data-line selection options for a slave device */
    cy_en_smif_data_select_t dataSelect;
    /** The base address the memory slave is mapped to in the PSoC memory map.
     * This address must be a multiple of the SMIF XIP memory size/capacity. The
     * SMIF XIP memory region should NOT overlap with other memory regions
     * (with exception to dual quad mode). Valid when the memory-mapped mode is
     * enabled.
     */
    uint32_t baseAddress;
    /** The size/capacity allocated in the PSoC memory map for the memory slave
     * device. The capacity is allocated from the base address. The capacity
     * must be a power of 2 and greater or equal than 64 KB. Valid when
     * memory-mapped mode is enabled
     */
    uint32_t memMappedSize;
    /** Defines if this memory device is one of the devices in the dual quad SPI
     * configuration. Equals the sum of the slave-slot numbers.  */
    uint32_t dualQuadSlots;
    cy_stc_smif_mem_device_cfg_t* deviceCfg;   /**< The configuration of the device */
} cy_stc_smif_mem_config_t;


/**
*
* This SMIF memory configuration structure is used to store the memory configuration for the memory mode of operation.
* This data structure is stored in a fixed location in the flash. The data structure is required
* for the initialization of the SMIF in the SystemInit.
*/
typedef struct
{
    uint32_t memCount;                         /**< The number of SMIF memory defined  */
    cy_stc_smif_mem_config_t** memConfig;      /**< The pointer to the array of the memory configuration structures of size Memory_count */
    uint32_t majorVersion;                     /**< The version of the SMIF driver */
    uint32_t minorVersion;                     /**< The version of the SMIF Driver */
} cy_stc_smif_block_config_t;


/** \} group_smif_data_structures_memslot */


/**
* \addtogroup group_smif_mem_slot_functions
* \{
*/
cy_en_smif_status_t    Cy_SMIF_MemInit(SMIF_Type *base,
                                cy_stc_smif_block_config_t const * blockConfig,
                                cy_stc_smif_context_t *context);
void        Cy_SMIF_MemDeInit(SMIF_Type *base);
cy_en_smif_status_t    Cy_SMIF_MemCmdWriteEnable( SMIF_Type *base,
                                        cy_stc_smif_mem_config_t const *memDevice,
                                        cy_stc_smif_context_t const *context);
cy_en_smif_status_t    Cy_SMIF_MemCmdWriteDisable(SMIF_Type *base,
                                         cy_stc_smif_mem_config_t const *memDevice,
                                         cy_stc_smif_context_t const *context);
bool Cy_SMIF_MemIsBusy(SMIF_Type *base, cy_stc_smif_mem_config_t const *memDevice,
                                    cy_stc_smif_context_t const *context);
cy_en_smif_status_t    Cy_SMIF_MemQuadEnable(SMIF_Type *base,
                                        cy_stc_smif_mem_config_t const *memDevice,
                                        cy_stc_smif_context_t const *context);
cy_en_smif_status_t    Cy_SMIF_MemCmdReadStatus(SMIF_Type *base,
                                        cy_stc_smif_mem_config_t const *memDevice,
                                        uint8_t *status, uint8_t command,
                                        cy_stc_smif_context_t const *context);
cy_en_smif_status_t    Cy_SMIF_MemCmdWriteStatus(SMIF_Type *base,
                                        cy_stc_smif_mem_config_t const *memDevice,
                                        void const *status, uint8_t command,
                                        cy_stc_smif_context_t const *context);
cy_en_smif_status_t    Cy_SMIF_MemCmdChipErase(SMIF_Type *base,
                                        cy_stc_smif_mem_config_t const *memDevice,
                                        cy_stc_smif_context_t const *context);
cy_en_smif_status_t    Cy_SMIF_MemCmdSectorErase(SMIF_Type *base,
                                            cy_stc_smif_mem_config_t const *memDevice,
                                            uint8_t const *sectorAddr,
                                            cy_stc_smif_context_t const *context);
cy_en_smif_status_t    Cy_SMIF_MemCmdProgram(SMIF_Type *base,
                                    cy_stc_smif_mem_config_t const *memDevice,
                                    uint8_t const *addr,
                                    uint8_t const *writeBuff,
                                    uint32_t size,
                                    cy_smif_event_cb_t cmdCompleteCb,
                                    cy_stc_smif_context_t *context);
cy_en_smif_status_t    Cy_SMIF_MemCmdRead(SMIF_Type *base,
                                    cy_stc_smif_mem_config_t const *memDevice,
                                    uint8_t const *addr,
                                    uint8_t *readBuff,
                                    uint32_t size,
                                    cy_smif_event_cb_t cmdCompleteCb,
                                    cy_stc_smif_context_t *context);
cy_en_smif_status_t    Cy_SMIF_MemSfdpDetect(SMIF_Type *base,
                                    cy_stc_smif_mem_device_cfg_t *device,
                                    cy_en_smif_slave_select_t slaveSelect,
                                    cy_en_smif_data_select_t dataSelect,
                                    cy_stc_smif_context_t *context);
                                    
cy_en_smif_status_t Cy_SMIF_MemIsReady(SMIF_Type *base, cy_stc_smif_mem_config_t const *memConfig, 
                                       uint32_t timeoutUs, cy_stc_smif_context_t const *context); 
cy_en_smif_status_t Cy_SMIF_MemIsQuadEnabled(SMIF_Type *base, cy_stc_smif_mem_config_t const *memConfig, 
                                             bool *isQuadEnabled, cy_stc_smif_context_t const *context);
cy_en_smif_status_t Cy_SMIF_MemEnableQuadMode(SMIF_Type *base, cy_stc_smif_mem_config_t const *memConfig, 
                                              uint32_t timeoutUs, cy_stc_smif_context_t const *context);
cy_en_smif_status_t Cy_SMIF_MemRead(SMIF_Type *base, cy_stc_smif_mem_config_t const *memConfig, 
                                    uint32_t address, uint8_t rxBuffer[], 
                                    uint32_t length, cy_stc_smif_context_t *context);
cy_en_smif_status_t Cy_SMIF_MemWrite(SMIF_Type *base, cy_stc_smif_mem_config_t const *memConfig, 
                                     uint32_t address, uint8_t const txBuffer[], 
                                     uint32_t length, cy_stc_smif_context_t *context); 
cy_en_smif_status_t Cy_SMIF_MemEraseSector(SMIF_Type *base, cy_stc_smif_mem_config_t const *memConfig, 
                                           uint32_t address, uint32_t length, 
                                           cy_stc_smif_context_t const *context);
cy_en_smif_status_t Cy_SMIF_MemEraseChip(SMIF_Type *base, cy_stc_smif_mem_config_t const *memConfig,
                                         cy_stc_smif_context_t const *context);
cy_en_smif_status_t Cy_SMIF_MemLocateHybridRegion(cy_stc_smif_mem_config_t const *memDevice,
                                               cy_stc_smif_hybrid_region_info_t** regionInfo, uint32_t address);
/** \} group_smif_mem_slot_functions */


#if defined(__cplusplus)
}
#endif

#endif /* CY_IP_MXSMIF */

#endif /* (CY_SMIF_MEMORYSLOT_H) */


/* [] END OF FILE */
