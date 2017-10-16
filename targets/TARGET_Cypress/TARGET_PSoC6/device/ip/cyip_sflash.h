/***************************************************************************//**
* \file cyip_sflash.h
*
* \brief
* SFLASH IP definitions
*
* \note
* Generated 9/21/2017 by CyDeviceHeaderGenerator v1.2.0.101
* from the register map configuration rev#1007711
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#ifndef _CYIP_SFLASH_H_
#define _CYIP_SFLASH_H_

#include "cyip_headers.h"

/*******************************************************************************
*                                    SFLASH
*******************************************************************************/

#define SFLASH_SECTION_SIZE                     0x00008000UL

/**
  * \brief FLASH Supervisory Region (SFLASH)
  */
typedef struct {
   __IM uint16_t RESERVED;
  __IOM uint16_t SILICON_ID;                    /*!< 0x00000002 Indicates siliconID of the device */
   __IM uint32_t RESERVED1[383];
  __IOM uint8_t  DIE_LOT[3];                    /*!< 0x00000600 Lot Number (3 bytes) */
  __IOM uint8_t  DIE_WAFER;                     /*!< 0x00000603 Wafer Number */
  __IOM uint8_t  DIE_X;                         /*!< 0x00000604 X Position on Wafer, CRI Pass/Fail Bin */
  __IOM uint8_t  DIE_Y;                         /*!< 0x00000605 Y Position on Wafer, CHI Pass/Fail Bin */
  __IOM uint8_t  DIE_SORT;                      /*!< 0x00000606 Sort1/2/3 Pass/Fail Bin */
  __IOM uint8_t  DIE_MINOR;                     /*!< 0x00000607 Minor Revision Number */
  __IOM uint8_t  DIE_DAY;                       /*!< 0x00000608 Day number */
  __IOM uint8_t  DIE_MONTH;                     /*!< 0x00000609 Month number */
  __IOM uint8_t  DIE_YEAR;                      /*!< 0x0000060A Year number */
   __IM uint8_t  RESERVED2[97];
  __IOM uint32_t CSP_PANEL_ID;                  /*!< 0x0000066C CSP Panel Id to record panel ID of CSP die */
   __IM uint32_t RESERVED3[100];
  __IOM uint32_t BLE_DEVICE_ADDRESS[128];       /*!< 0x00000800 BLE_DEVICE_ADDRESS */
  __IOM uint32_t USER_FREE_ROW1[128];           /*!< 0x00000A00 USER_FREE_ROW1 */
  __IOM uint32_t USER_FREE_ROW2[128];           /*!< 0x00000C00 USER_FREE_ROW2 */
  __IOM uint32_t USER_FREE_ROW3[128];           /*!< 0x00000E00 USER_FREE_ROW3 */
   __IM uint32_t RESERVED4[302];
  __IOM uint8_t  DEVICE_UID[16];                /*!< 0x000014B8 Unique Identifier Number for each device */
  __IOM uint8_t  MASTER_KEY[16];                /*!< 0x000014C8 Master key to change other keys */
  __IOM uint32_t STANDARD_SMPU_STRUCT_SLAVE_ADDR[16]; /*!< 0x000014D8 Standard SMPU STRUCT Slave Address value */
  __IOM uint32_t STANDARD_SMPU_STRUCT_SLAVE_ATTR[16]; /*!< 0x00001518 Standard SMPU STRUCT Slave Attribute value */
  __IOM uint32_t STANDARD_SMPU_STRUCT_MASTER_ATTR[16]; /*!< 0x00001558 Standard SMPU STRUCT Master Attribute value */
  __IOM uint32_t STANDARD_MPU_STRUCT[16];       /*!< 0x00001598 Standard MPU STRUCT */
  __IOM uint32_t STANDARD_PPU_STRUCT[16];       /*!< 0x000015D8 Standard PPU STRUCT */
   __IM uint32_t RESERVED5[122];
  __IOM uint16_t PILO_FREQ_STEP;                /*!< 0x00001800 Resolution step for PILO at class in BCD format */
   __IM uint16_t RESERVED6;
  __IOM uint32_t CSDV2_CSD0_ADC_VREF0;          /*!< 0x00001804 CSD 1p2 & 1p6 voltage levels for accuracy */
  __IOM uint32_t CSDV2_CSD0_ADC_VREF1;          /*!< 0x00001808 CSD 2p3 & 0p8 voltage levels for accuracy */
  __IOM uint32_t CSDV2_CSD0_ADC_VREF2;          /*!< 0x0000180C CSD calibration spare voltage level for accuracy */
   __IM uint32_t RESERVED7[508];
  __IOM uint32_t FLASH_BOOT_OBJECT_SIZE;        /*!< 0x00002000 Flash Boot - Object Size */
  __IOM uint32_t FLASH_BOOT_APP_ID;             /*!< 0x00002004 Flash Boot - Application ID/Version */
  __IOM uint32_t FLASH_BOOT_ATTRIBUTE;          /*!< 0x00002008 N/A */
  __IOM uint32_t FLASH_BOOT_N_CORES;            /*!< 0x0000200C Flash Boot - Number of Cores(N) */
  __IOM uint32_t FLASH_BOOT_VT_OFFSET;          /*!< 0x00002010 Flash Boot - Core Vector Table offset */
  __IOM uint32_t FLASH_BOOT_CORE_CPUID;         /*!< 0x00002014 Flash Boot - Core CPU ID/Core Index */
  __IOM uint32_t FLASH_BOOT_VECTOR_TABLE[48];   /*!< 0x00002018 Flash Boot - Core Vector Table */
  __IOM uint8_t  FLASH_BOOT_CODE[8192];         /*!< 0x000020D8 Flash Boot - Code and Data */
  __IOM uint8_t  FLASH_BOOT_CMAC[16];           /*!< 0x000040D8 Secure Hash/CMAC - Not applicable */
   __IM uint32_t RESERVED8[1606];
  __IOM uint8_t  PUBLIC_KEY[3072];              /*!< 0x00005A00 Public key for signature verification (max RSA key size 4096) */
   __IM uint32_t RESERVED9[1152];
  __IOM uint32_t TOC1_OBJECT_SIZE;              /*!< 0x00007800 Object size in bytes for CRC calculation starting from offset
                                                                0x00 */
  __IOM uint32_t TOC1_MAGIC_NUMBER;             /*!< 0x00007804 Magic number(0x01211219) */
  __IOM uint32_t TOC1_FCMAC_OBJECTS;            /*!< 0x00007808 Number of objects starting from offset 0xC to be verified for
                                                                FACTORY_CMAC */
  __IOM uint32_t TOC1_SFLASH_GENERAL_TRIM_ADDR; /*!< 0x0000780C Address of trims stored in SFLASH */
  __IOM uint32_t TOC1_UNIQUE_ID_ADDR;           /*!< 0x00007810 Address of Unique ID stored in SFLASH */
  __IOM uint32_t TOC1_FB_OBJECT_ADDR;           /*!< 0x00007814 Addresss of FLASH Boot(FB) object that include FLASH patch also */
  __IOM uint32_t TOC1_SYSCALL_TABLE_ADDR;       /*!< 0x00007818 Address of SYSCALL_TABLE entry in SFLASH */
   __IM uint32_t RESERVED10[120];
  __IOM uint32_t TOC1_CRC_ADDR;                 /*!< 0x000079FC Upper 2 bytes contain CRC16-CCITT and lower 2 bytes are 0 */
  __IOM uint32_t RTOC1_OBJECT_SIZE;             /*!< 0x00007A00 Redundant Object size in bytes for CRC calculation starting
                                                                from offset 0x00 */
  __IOM uint32_t RTOC1_MAGIC_NUMBER;            /*!< 0x00007A04 Redundant Magic number(0x01211219) */
  __IOM uint32_t RTOC1_FCMAC_OBJECTS;           /*!< 0x00007A08 Redundant Number of objects starting from offset 0xC to be
                                                                verified for FACTORY_CMAC */
  __IOM uint32_t RTOC1_SFLASH_GENERAL_TRIM_ADDR; /*!< 0x00007A0C Redundant Address of trims stored in SFLASH */
  __IOM uint32_t RTOC1_UNIQUE_ID_ADDR;          /*!< 0x00007A10 Redundant Address of Unique ID stored in SFLASH */
  __IOM uint32_t RTOC1_FB_OBJECT_ADDR;          /*!< 0x00007A14 Redundant Addresss of FLASH Boot(FB) object that include FLASH
                                                                patch also */
  __IOM uint32_t RTOC1_SYSCALL_TABLE_ADDR;      /*!< 0x00007A18 Redundant Address of SYSCALL_TABLE entry in SFLASH */
   __IM uint32_t RESERVED11[120];
  __IOM uint32_t RTOC1_CRC_ADDR;                /*!< 0x00007BFC Redundant CRC,Upper 2 bytes contain CRC16-CCITT and lower 2
                                                                bytes are 0 */
  __IOM uint32_t TOC2_OBJECT_SIZE;              /*!< 0x00007C00 Object size in bytes for CRC calculation starting from offset
                                                                0x00 */
  __IOM uint32_t TOC2_MAGIC_NUMBER;             /*!< 0x00007C04 Magic number(0x01211220) */
  __IOM uint32_t TOC2_KEY_BLOCK_ADDR;           /*!< 0x00007C08 Address of Key Storage FLASH blocks */
  __IOM uint32_t TOC2_SMIF_CFG_STRUCT_ADDR;     /*!< 0x00007C0C Null terminated table of pointers representing the SMIF
                                                                configuration structure */
  __IOM uint32_t TOC2_FIRST_USER_APP_ADDR;      /*!< 0x00007C10 Address of First User Application Object */
  __IOM uint32_t TOC2_FIRST_USER_APP_FORMAT;    /*!< 0x00007C14 Format of First User Application Object. 0 - Basic, 1 - Cypress
                                                                standard & 2 - Simplified */
  __IOM uint32_t TOC2_SECOND_USER_APP_ADDR;     /*!< 0x00007C18 Address of Second User Application Object */
  __IOM uint32_t TOC2_SECOND_USER_APP_FORMAT;   /*!< 0x00007C1C Format of Second User Application Object. 0 - Basic, 1 -
                                                                Cypress standard & 2 - Simplified */
  __IOM uint32_t TOC2_SCMAC_OBJECTS;            /*!< 0x00007C20 Number of additional objects(in addition to objects covered by
                                                                FACORY_CAMC) starting from offset 0x24 to be verified for
                                                                SECURE_CMAC(SCMAC) */
  __IOM uint32_t TOC2_SIGNATURE_VERIF_KEY;      /*!< 0x00007C24 Address of signature verification key (0 if none).The object is
                                                                signature specific key. It is the public key in case of RSA */
   __IM uint32_t RESERVED12[117];
  __IOM uint32_t TOC2_CRC_ADDR;                 /*!< 0x00007DFC CRC,Upper 2 bytes contain CRC16-CCITT and lower 2 bytes are 0 */
  __IOM uint32_t RTOC2_OBJECT_SIZE;             /*!< 0x00007E00 Redundant Object size in bytes for CRC calculation starting
                                                                from offset 0x00 */
  __IOM uint32_t RTOC2_MAGIC_NUMBER;            /*!< 0x00007E04 Redundant Magic number(0x01211220) */
  __IOM uint32_t RTOC2_KEY_BLOCK_ADDR;          /*!< 0x00007E08 Redundant Address of Key Storage FLASH blocks */
  __IOM uint32_t RTOC2_SMIF_CFG_STRUCT_ADDR;    /*!< 0x00007E0C Redundant Null terminated table of pointers representing the
                                                                SMIF configuration structure */
  __IOM uint32_t RTOC2_FIRST_USER_APP_ADDR;     /*!< 0x00007E10 Redundant Address of First User Application Object */
  __IOM uint32_t RTOC2_FIRST_USER_APP_FORMAT;   /*!< 0x00007E14 Redundant Format of First User Application Object. 0 - Basic, 1
                                                                - Cypress standard & 2 - Simplified */
  __IOM uint32_t RTOC2_SECOND_USER_APP_ADDR;    /*!< 0x00007E18 Redundant Address of Second User Application Object */
  __IOM uint32_t RTOC2_SECOND_USER_APP_FORMAT;  /*!< 0x00007E1C Redundant Format of Second User Application Object. 0 - Basic,
                                                                1 - Cypress standard & 2 - Simplified */
  __IOM uint32_t RTOC2_SCMAC_OBJECTS;           /*!< 0x00007E20 Redundant Number of additional objects(in addition to objects
                                                                covered by FACORY_CAMC) starting from offset 0x24 to be verified
                                                                for SECURE_CMAC(SCMAC) */
  __IOM uint32_t RTOC2_SIGNATURE_VERIF_KEY;     /*!< 0x00007E24 Redundant Address of signature verification key (0 if none).The
                                                                object is signature specific key. It is the public key in case
                                                                of RSA */
   __IM uint32_t RESERVED13[117];
  __IOM uint32_t RTOC2_CRC_ADDR;                /*!< 0x00007FFC Redundant CRC,Upper 2 bytes contain CRC16-CCITT and lower 2
                                                                bytes are 0 */
} SFLASH_Type;                                  /*!< Size = 32768 (0x8000) */


/* SFLASH.SILICON_ID */
#define SFLASH_SILICON_ID_ID_Pos                0UL
#define SFLASH_SILICON_ID_ID_Msk                0xFFFFUL
/* SFLASH.DIE_LOT */
#define SFLASH_DIE_LOT_LOT_Pos                  0UL
#define SFLASH_DIE_LOT_LOT_Msk                  0xFFUL
/* SFLASH.DIE_WAFER */
#define SFLASH_DIE_WAFER_WAFER_Pos              0UL
#define SFLASH_DIE_WAFER_WAFER_Msk              0xFFUL
/* SFLASH.DIE_X */
#define SFLASH_DIE_X_X_Pos                      0UL
#define SFLASH_DIE_X_X_Msk                      0xFFUL
/* SFLASH.DIE_Y */
#define SFLASH_DIE_Y_Y_Pos                      0UL
#define SFLASH_DIE_Y_Y_Msk                      0xFFUL
/* SFLASH.DIE_SORT */
#define SFLASH_DIE_SORT_S1_PASS_Pos             0UL
#define SFLASH_DIE_SORT_S1_PASS_Msk             0x1UL
#define SFLASH_DIE_SORT_S2_PASS_Pos             1UL
#define SFLASH_DIE_SORT_S2_PASS_Msk             0x2UL
#define SFLASH_DIE_SORT_S3_PASS_Pos             2UL
#define SFLASH_DIE_SORT_S3_PASS_Msk             0x4UL
#define SFLASH_DIE_SORT_CRI_PASS_Pos            3UL
#define SFLASH_DIE_SORT_CRI_PASS_Msk            0x8UL
#define SFLASH_DIE_SORT_CHI_PASS_Pos            4UL
#define SFLASH_DIE_SORT_CHI_PASS_Msk            0x10UL
#define SFLASH_DIE_SORT_ENG_PASS_Pos            5UL
#define SFLASH_DIE_SORT_ENG_PASS_Msk            0x20UL
/* SFLASH.DIE_MINOR */
#define SFLASH_DIE_MINOR_MINOR_Pos              0UL
#define SFLASH_DIE_MINOR_MINOR_Msk              0xFFUL
/* SFLASH.DIE_DAY */
#define SFLASH_DIE_DAY_MINOR_Pos                0UL
#define SFLASH_DIE_DAY_MINOR_Msk                0xFFUL
/* SFLASH.DIE_MONTH */
#define SFLASH_DIE_MONTH_MINOR_Pos              0UL
#define SFLASH_DIE_MONTH_MINOR_Msk              0xFFUL
/* SFLASH.DIE_YEAR */
#define SFLASH_DIE_YEAR_MINOR_Pos               0UL
#define SFLASH_DIE_YEAR_MINOR_Msk               0xFFUL
/* SFLASH.CSP_PANEL_ID */
#define SFLASH_CSP_PANEL_ID_DATA32_Pos          0UL
#define SFLASH_CSP_PANEL_ID_DATA32_Msk          0xFFFFFFFFUL
/* SFLASH.BLE_DEVICE_ADDRESS */
#define SFLASH_BLE_DEVICE_ADDRESS_ADDR_Pos      0UL
#define SFLASH_BLE_DEVICE_ADDRESS_ADDR_Msk      0xFFFFFFFFUL
/* SFLASH.USER_FREE_ROW1 */
#define SFLASH_USER_FREE_ROW1_DATA32_Pos        0UL
#define SFLASH_USER_FREE_ROW1_DATA32_Msk        0xFFFFFFFFUL
/* SFLASH.USER_FREE_ROW2 */
#define SFLASH_USER_FREE_ROW2_DATA32_Pos        0UL
#define SFLASH_USER_FREE_ROW2_DATA32_Msk        0xFFFFFFFFUL
/* SFLASH.USER_FREE_ROW3 */
#define SFLASH_USER_FREE_ROW3_DATA32_Pos        0UL
#define SFLASH_USER_FREE_ROW3_DATA32_Msk        0xFFFFFFFFUL
/* SFLASH.DEVICE_UID */
#define SFLASH_DEVICE_UID_DATA_Pos              0UL
#define SFLASH_DEVICE_UID_DATA_Msk              0xFFUL
/* SFLASH.MASTER_KEY */
#define SFLASH_MASTER_KEY_DATA_Pos              0UL
#define SFLASH_MASTER_KEY_DATA_Msk              0xFFUL
/* SFLASH.STANDARD_SMPU_STRUCT_SLAVE_ADDR */
#define SFLASH_STANDARD_SMPU_STRUCT_SLAVE_ADDR_DATA32_Pos 0UL
#define SFLASH_STANDARD_SMPU_STRUCT_SLAVE_ADDR_DATA32_Msk 0xFFFFFFFFUL
/* SFLASH.STANDARD_SMPU_STRUCT_SLAVE_ATTR */
#define SFLASH_STANDARD_SMPU_STRUCT_SLAVE_ATTR_DATA32_Pos 0UL
#define SFLASH_STANDARD_SMPU_STRUCT_SLAVE_ATTR_DATA32_Msk 0xFFFFFFFFUL
/* SFLASH.STANDARD_SMPU_STRUCT_MASTER_ATTR */
#define SFLASH_STANDARD_SMPU_STRUCT_MASTER_ATTR_DATA32_Pos 0UL
#define SFLASH_STANDARD_SMPU_STRUCT_MASTER_ATTR_DATA32_Msk 0xFFFFFFFFUL
/* SFLASH.STANDARD_MPU_STRUCT */
#define SFLASH_STANDARD_MPU_STRUCT_DATA32_Pos   0UL
#define SFLASH_STANDARD_MPU_STRUCT_DATA32_Msk   0xFFFFFFFFUL
/* SFLASH.STANDARD_PPU_STRUCT */
#define SFLASH_STANDARD_PPU_STRUCT_DATA32_Pos   0UL
#define SFLASH_STANDARD_PPU_STRUCT_DATA32_Msk   0xFFFFFFFFUL
/* SFLASH.PILO_FREQ_STEP */
#define SFLASH_PILO_FREQ_STEP_STEP_Pos          0UL
#define SFLASH_PILO_FREQ_STEP_STEP_Msk          0xFFFFUL
/* SFLASH.CSDV2_CSD0_ADC_VREF0 */
#define SFLASH_CSDV2_CSD0_ADC_VREF0_VREF_HI_LEVELS_1P2_Pos 0UL
#define SFLASH_CSDV2_CSD0_ADC_VREF0_VREF_HI_LEVELS_1P2_Msk 0xFFFFUL
#define SFLASH_CSDV2_CSD0_ADC_VREF0_VREF_HI_LEVELS_1P6_Pos 16UL
#define SFLASH_CSDV2_CSD0_ADC_VREF0_VREF_HI_LEVELS_1P6_Msk 0xFFFF0000UL
/* SFLASH.CSDV2_CSD0_ADC_VREF1 */
#define SFLASH_CSDV2_CSD0_ADC_VREF1_VREF_HI_LEVELS_2P1_Pos 0UL
#define SFLASH_CSDV2_CSD0_ADC_VREF1_VREF_HI_LEVELS_2P1_Msk 0xFFFFUL
#define SFLASH_CSDV2_CSD0_ADC_VREF1_VREF_HI_LEVELS_0P8_Pos 16UL
#define SFLASH_CSDV2_CSD0_ADC_VREF1_VREF_HI_LEVELS_0P8_Msk 0xFFFF0000UL
/* SFLASH.CSDV2_CSD0_ADC_VREF2 */
#define SFLASH_CSDV2_CSD0_ADC_VREF2_VREF_HI_LEVELS_2P6_Pos 0UL
#define SFLASH_CSDV2_CSD0_ADC_VREF2_VREF_HI_LEVELS_2P6_Msk 0xFFFFUL
#define SFLASH_CSDV2_CSD0_ADC_VREF2_UNUSED_Pos  16UL
#define SFLASH_CSDV2_CSD0_ADC_VREF2_UNUSED_Msk  0xFFFF0000UL
/* SFLASH.FLASH_BOOT_OBJECT_SIZE */
#define SFLASH_FLASH_BOOT_OBJECT_SIZE_DATA32_Pos 0UL
#define SFLASH_FLASH_BOOT_OBJECT_SIZE_DATA32_Msk 0xFFFFFFFFUL
/* SFLASH.FLASH_BOOT_APP_ID */
#define SFLASH_FLASH_BOOT_APP_ID_APP_ID_Pos     0UL
#define SFLASH_FLASH_BOOT_APP_ID_APP_ID_Msk     0xFFFFUL
#define SFLASH_FLASH_BOOT_APP_ID_MINOR_VERSION_Pos 16UL
#define SFLASH_FLASH_BOOT_APP_ID_MINOR_VERSION_Msk 0xFF0000UL
#define SFLASH_FLASH_BOOT_APP_ID_MAJOR_VERSION_Pos 24UL
#define SFLASH_FLASH_BOOT_APP_ID_MAJOR_VERSION_Msk 0xF000000UL
/* SFLASH.FLASH_BOOT_ATTRIBUTE */
#define SFLASH_FLASH_BOOT_ATTRIBUTE_DATA32_Pos  0UL
#define SFLASH_FLASH_BOOT_ATTRIBUTE_DATA32_Msk  0xFFFFFFFFUL
/* SFLASH.FLASH_BOOT_N_CORES */
#define SFLASH_FLASH_BOOT_N_CORES_DATA32_Pos    0UL
#define SFLASH_FLASH_BOOT_N_CORES_DATA32_Msk    0xFFFFFFFFUL
/* SFLASH.FLASH_BOOT_VT_OFFSET */
#define SFLASH_FLASH_BOOT_VT_OFFSET_DATA32_Pos  0UL
#define SFLASH_FLASH_BOOT_VT_OFFSET_DATA32_Msk  0xFFFFFFFFUL
/* SFLASH.FLASH_BOOT_CORE_CPUID */
#define SFLASH_FLASH_BOOT_CORE_CPUID_DATA32_Pos 0UL
#define SFLASH_FLASH_BOOT_CORE_CPUID_DATA32_Msk 0xFFFFFFFFUL
/* SFLASH.FLASH_BOOT_VECTOR_TABLE */
#define SFLASH_FLASH_BOOT_VECTOR_TABLE_DATA32_Pos 0UL
#define SFLASH_FLASH_BOOT_VECTOR_TABLE_DATA32_Msk 0xFFFFFFFFUL
/* SFLASH.FLASH_BOOT_CODE */
#define SFLASH_FLASH_BOOT_CODE_DATA_Pos         0UL
#define SFLASH_FLASH_BOOT_CODE_DATA_Msk         0xFFUL
/* SFLASH.FLASH_BOOT_CMAC */
#define SFLASH_FLASH_BOOT_CMAC_DATA_Pos         0UL
#define SFLASH_FLASH_BOOT_CMAC_DATA_Msk         0xFFUL
/* SFLASH.PUBLIC_KEY */
#define SFLASH_PUBLIC_KEY_DATA_Pos              0UL
#define SFLASH_PUBLIC_KEY_DATA_Msk              0xFFUL
/* SFLASH.TOC1_OBJECT_SIZE */
#define SFLASH_TOC1_OBJECT_SIZE_DATA32_Pos      0UL
#define SFLASH_TOC1_OBJECT_SIZE_DATA32_Msk      0xFFFFFFFFUL
/* SFLASH.TOC1_MAGIC_NUMBER */
#define SFLASH_TOC1_MAGIC_NUMBER_DATA32_Pos     0UL
#define SFLASH_TOC1_MAGIC_NUMBER_DATA32_Msk     0xFFFFFFFFUL
/* SFLASH.TOC1_FCMAC_OBJECTS */
#define SFLASH_TOC1_FCMAC_OBJECTS_DATA32_Pos    0UL
#define SFLASH_TOC1_FCMAC_OBJECTS_DATA32_Msk    0xFFFFFFFFUL
/* SFLASH.TOC1_SFLASH_GENERAL_TRIM_ADDR */
#define SFLASH_TOC1_SFLASH_GENERAL_TRIM_ADDR_DATA32_Pos 0UL
#define SFLASH_TOC1_SFLASH_GENERAL_TRIM_ADDR_DATA32_Msk 0xFFFFFFFFUL
/* SFLASH.TOC1_UNIQUE_ID_ADDR */
#define SFLASH_TOC1_UNIQUE_ID_ADDR_DATA32_Pos   0UL
#define SFLASH_TOC1_UNIQUE_ID_ADDR_DATA32_Msk   0xFFFFFFFFUL
/* SFLASH.TOC1_FB_OBJECT_ADDR */
#define SFLASH_TOC1_FB_OBJECT_ADDR_DATA32_Pos   0UL
#define SFLASH_TOC1_FB_OBJECT_ADDR_DATA32_Msk   0xFFFFFFFFUL
/* SFLASH.TOC1_SYSCALL_TABLE_ADDR */
#define SFLASH_TOC1_SYSCALL_TABLE_ADDR_DATA32_Pos 0UL
#define SFLASH_TOC1_SYSCALL_TABLE_ADDR_DATA32_Msk 0xFFFFFFFFUL
/* SFLASH.TOC1_CRC_ADDR */
#define SFLASH_TOC1_CRC_ADDR_DATA32_Pos         0UL
#define SFLASH_TOC1_CRC_ADDR_DATA32_Msk         0xFFFFFFFFUL
/* SFLASH.RTOC1_OBJECT_SIZE */
#define SFLASH_RTOC1_OBJECT_SIZE_DATA32_Pos     0UL
#define SFLASH_RTOC1_OBJECT_SIZE_DATA32_Msk     0xFFFFFFFFUL
/* SFLASH.RTOC1_MAGIC_NUMBER */
#define SFLASH_RTOC1_MAGIC_NUMBER_DATA32_Pos    0UL
#define SFLASH_RTOC1_MAGIC_NUMBER_DATA32_Msk    0xFFFFFFFFUL
/* SFLASH.RTOC1_FCMAC_OBJECTS */
#define SFLASH_RTOC1_FCMAC_OBJECTS_DATA32_Pos   0UL
#define SFLASH_RTOC1_FCMAC_OBJECTS_DATA32_Msk   0xFFFFFFFFUL
/* SFLASH.RTOC1_SFLASH_GENERAL_TRIM_ADDR */
#define SFLASH_RTOC1_SFLASH_GENERAL_TRIM_ADDR_DATA32_Pos 0UL
#define SFLASH_RTOC1_SFLASH_GENERAL_TRIM_ADDR_DATA32_Msk 0xFFFFFFFFUL
/* SFLASH.RTOC1_UNIQUE_ID_ADDR */
#define SFLASH_RTOC1_UNIQUE_ID_ADDR_DATA32_Pos  0UL
#define SFLASH_RTOC1_UNIQUE_ID_ADDR_DATA32_Msk  0xFFFFFFFFUL
/* SFLASH.RTOC1_FB_OBJECT_ADDR */
#define SFLASH_RTOC1_FB_OBJECT_ADDR_DATA32_Pos  0UL
#define SFLASH_RTOC1_FB_OBJECT_ADDR_DATA32_Msk  0xFFFFFFFFUL
/* SFLASH.RTOC1_SYSCALL_TABLE_ADDR */
#define SFLASH_RTOC1_SYSCALL_TABLE_ADDR_DATA32_Pos 0UL
#define SFLASH_RTOC1_SYSCALL_TABLE_ADDR_DATA32_Msk 0xFFFFFFFFUL
/* SFLASH.RTOC1_CRC_ADDR */
#define SFLASH_RTOC1_CRC_ADDR_DATA32_Pos        0UL
#define SFLASH_RTOC1_CRC_ADDR_DATA32_Msk        0xFFFFFFFFUL
/* SFLASH.TOC2_OBJECT_SIZE */
#define SFLASH_TOC2_OBJECT_SIZE_DATA32_Pos      0UL
#define SFLASH_TOC2_OBJECT_SIZE_DATA32_Msk      0xFFFFFFFFUL
/* SFLASH.TOC2_MAGIC_NUMBER */
#define SFLASH_TOC2_MAGIC_NUMBER_DATA32_Pos     0UL
#define SFLASH_TOC2_MAGIC_NUMBER_DATA32_Msk     0xFFFFFFFFUL
/* SFLASH.TOC2_KEY_BLOCK_ADDR */
#define SFLASH_TOC2_KEY_BLOCK_ADDR_DATA32_Pos   0UL
#define SFLASH_TOC2_KEY_BLOCK_ADDR_DATA32_Msk   0xFFFFFFFFUL
/* SFLASH.TOC2_SMIF_CFG_STRUCT_ADDR */
#define SFLASH_TOC2_SMIF_CFG_STRUCT_ADDR_DATA32_Pos 0UL
#define SFLASH_TOC2_SMIF_CFG_STRUCT_ADDR_DATA32_Msk 0xFFFFFFFFUL
/* SFLASH.TOC2_FIRST_USER_APP_ADDR */
#define SFLASH_TOC2_FIRST_USER_APP_ADDR_DATA32_Pos 0UL
#define SFLASH_TOC2_FIRST_USER_APP_ADDR_DATA32_Msk 0xFFFFFFFFUL
/* SFLASH.TOC2_FIRST_USER_APP_FORMAT */
#define SFLASH_TOC2_FIRST_USER_APP_FORMAT_DATA32_Pos 0UL
#define SFLASH_TOC2_FIRST_USER_APP_FORMAT_DATA32_Msk 0xFFFFFFFFUL
/* SFLASH.TOC2_SECOND_USER_APP_ADDR */
#define SFLASH_TOC2_SECOND_USER_APP_ADDR_DATA32_Pos 0UL
#define SFLASH_TOC2_SECOND_USER_APP_ADDR_DATA32_Msk 0xFFFFFFFFUL
/* SFLASH.TOC2_SECOND_USER_APP_FORMAT */
#define SFLASH_TOC2_SECOND_USER_APP_FORMAT_DATA32_Pos 0UL
#define SFLASH_TOC2_SECOND_USER_APP_FORMAT_DATA32_Msk 0xFFFFFFFFUL
/* SFLASH.TOC2_SCMAC_OBJECTS */
#define SFLASH_TOC2_SCMAC_OBJECTS_DATA32_Pos    0UL
#define SFLASH_TOC2_SCMAC_OBJECTS_DATA32_Msk    0xFFFFFFFFUL
/* SFLASH.TOC2_SIGNATURE_VERIF_KEY */
#define SFLASH_TOC2_SIGNATURE_VERIF_KEY_DATA32_Pos 0UL
#define SFLASH_TOC2_SIGNATURE_VERIF_KEY_DATA32_Msk 0xFFFFFFFFUL
/* SFLASH.TOC2_CRC_ADDR */
#define SFLASH_TOC2_CRC_ADDR_DATA32_Pos         0UL
#define SFLASH_TOC2_CRC_ADDR_DATA32_Msk         0xFFFFFFFFUL
/* SFLASH.RTOC2_OBJECT_SIZE */
#define SFLASH_RTOC2_OBJECT_SIZE_DATA32_Pos     0UL
#define SFLASH_RTOC2_OBJECT_SIZE_DATA32_Msk     0xFFFFFFFFUL
/* SFLASH.RTOC2_MAGIC_NUMBER */
#define SFLASH_RTOC2_MAGIC_NUMBER_DATA32_Pos    0UL
#define SFLASH_RTOC2_MAGIC_NUMBER_DATA32_Msk    0xFFFFFFFFUL
/* SFLASH.RTOC2_KEY_BLOCK_ADDR */
#define SFLASH_RTOC2_KEY_BLOCK_ADDR_DATA32_Pos  0UL
#define SFLASH_RTOC2_KEY_BLOCK_ADDR_DATA32_Msk  0xFFFFFFFFUL
/* SFLASH.RTOC2_SMIF_CFG_STRUCT_ADDR */
#define SFLASH_RTOC2_SMIF_CFG_STRUCT_ADDR_DATA32_Pos 0UL
#define SFLASH_RTOC2_SMIF_CFG_STRUCT_ADDR_DATA32_Msk 0xFFFFFFFFUL
/* SFLASH.RTOC2_FIRST_USER_APP_ADDR */
#define SFLASH_RTOC2_FIRST_USER_APP_ADDR_DATA32_Pos 0UL
#define SFLASH_RTOC2_FIRST_USER_APP_ADDR_DATA32_Msk 0xFFFFFFFFUL
/* SFLASH.RTOC2_FIRST_USER_APP_FORMAT */
#define SFLASH_RTOC2_FIRST_USER_APP_FORMAT_DATA32_Pos 0UL
#define SFLASH_RTOC2_FIRST_USER_APP_FORMAT_DATA32_Msk 0xFFFFFFFFUL
/* SFLASH.RTOC2_SECOND_USER_APP_ADDR */
#define SFLASH_RTOC2_SECOND_USER_APP_ADDR_DATA32_Pos 0UL
#define SFLASH_RTOC2_SECOND_USER_APP_ADDR_DATA32_Msk 0xFFFFFFFFUL
/* SFLASH.RTOC2_SECOND_USER_APP_FORMAT */
#define SFLASH_RTOC2_SECOND_USER_APP_FORMAT_DATA32_Pos 0UL
#define SFLASH_RTOC2_SECOND_USER_APP_FORMAT_DATA32_Msk 0xFFFFFFFFUL
/* SFLASH.RTOC2_SCMAC_OBJECTS */
#define SFLASH_RTOC2_SCMAC_OBJECTS_DATA32_Pos   0UL
#define SFLASH_RTOC2_SCMAC_OBJECTS_DATA32_Msk   0xFFFFFFFFUL
/* SFLASH.RTOC2_SIGNATURE_VERIF_KEY */
#define SFLASH_RTOC2_SIGNATURE_VERIF_KEY_DATA32_Pos 0UL
#define SFLASH_RTOC2_SIGNATURE_VERIF_KEY_DATA32_Msk 0xFFFFFFFFUL
/* SFLASH.RTOC2_CRC_ADDR */
#define SFLASH_RTOC2_CRC_ADDR_DATA32_Pos        0UL
#define SFLASH_RTOC2_CRC_ADDR_DATA32_Msk        0xFFFFFFFFUL


#endif /* _CYIP_SFLASH_H_ */


/* [] END OF FILE */
