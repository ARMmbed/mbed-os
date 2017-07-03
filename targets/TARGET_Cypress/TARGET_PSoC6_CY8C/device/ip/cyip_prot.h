/***************************************************************************//**
* \file cyip_prot.h
*
* \brief
* PROT IP definitions
*
* \note
* Generated 2/9/2017 by CyDeviceHeaderGenerator v1.1.0.47
* from the register map configuration rev#961378
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#ifndef _CYIP_PROT_H_
#define _CYIP_PROT_H_

#include "cyip_headers.h"

/* =========================================================================================================================== */
/* ================                                           PROT                                            ================ */
/* =========================================================================================================================== */

#define PROT_SMPU_SMPU_STRUCT_SECTION_SIZE 0x00000040UL
#define PROT_SMPU_SECTION_SIZE      0x00004000UL
#define PROT_MPU_MPU_STRUCT_SECTION_SIZE 0x00000020UL
#define PROT_MPU_SECTION_SIZE       0x00000400UL
#define PROT_SECTION_SIZE           0x00010000UL

/**
  * \brief SMPU structure (PROT_SMPU_SMPU_STRUCT)
  */
typedef struct {
  __IOM uint32_t ADDR0;                         /*!< 0x00000000 SMPU region address 0 (slave structure)                        */
  __IOM uint32_t ATT0;                          /*!< 0x00000004 SMPU region attributes 0 (slave structure)                     */
   __IM uint32_t RESERVED[6];
   __IM uint32_t ADDR1;                         /*!< 0x00000020 SMPU region address 1 (master structure)                       */
  __IOM uint32_t ATT1;                          /*!< 0x00000024 SMPU region attributes 1 (master structure)                    */
   __IM uint32_t RESERVED1[6];
} PROT_SMPU_SMPU_STRUCT_Type;                   /*!< Size = 40 (0x28)                                                          */

/**
  * \brief SMPU (PROT_SMPU)
  */
typedef struct {
  __IOM uint32_t MS0_CTL;                       /*!< 0x00000000 Master 0 protection context control                            */
  __IOM uint32_t MS1_CTL;                       /*!< 0x00000004 Master 1 protection context control                            */
  __IOM uint32_t MS2_CTL;                       /*!< 0x00000008 Master 2 protection context control                            */
  __IOM uint32_t MS3_CTL;                       /*!< 0x0000000C Master 3 protection context control                            */
  __IOM uint32_t MS4_CTL;                       /*!< 0x00000010 Master 4 protection context control                            */
  __IOM uint32_t MS5_CTL;                       /*!< 0x00000014 Master 5 protection context control                            */
  __IOM uint32_t MS6_CTL;                       /*!< 0x00000018 Master 6 protection context control                            */
  __IOM uint32_t MS7_CTL;                       /*!< 0x0000001C Master 7 protection context control                            */
  __IOM uint32_t MS8_CTL;                       /*!< 0x00000020 Master 8 protection context control                            */
  __IOM uint32_t MS9_CTL;                       /*!< 0x00000024 Master 9 protection context control                            */
  __IOM uint32_t MS10_CTL;                      /*!< 0x00000028 Master 10 protection context control                           */
  __IOM uint32_t MS11_CTL;                      /*!< 0x0000002C Master 11 protection context control                           */
  __IOM uint32_t MS12_CTL;                      /*!< 0x00000030 Master 12 protection context control                           */
  __IOM uint32_t MS13_CTL;                      /*!< 0x00000034 Master 13 protection context control                           */
  __IOM uint32_t MS14_CTL;                      /*!< 0x00000038 Master 14 protection context control                           */
  __IOM uint32_t MS15_CTL;                      /*!< 0x0000003C Master 15 protection context control                           */
   __IM uint32_t RESERVED[2032];
        PROT_SMPU_SMPU_STRUCT_Type SMPU_STRUCT[32]; /*!< 0x00002000 SMPU structure                                             */
   __IM uint32_t RESERVED1[1536];
} PROT_SMPU_Type;                               /*!< Size = 10240 (0x2800)                                                     */

/**
  * \brief MPU structure (PROT_MPU_MPU_STRUCT)
  */
typedef struct {
  __IOM uint32_t ADDR;                          /*!< 0x00000000 MPU region address                                             */
  __IOM uint32_t ATT;                           /*!< 0x00000004 MPU region attrributes                                         */
   __IM uint32_t RESERVED[6];
} PROT_MPU_MPU_STRUCT_Type;                     /*!< Size = 8 (0x8)                                                            */

/**
  * \brief MPU (PROT_MPU)
  */
typedef struct {
  __IOM uint32_t MS_CTL;                        /*!< 0x00000000 Master control                                                 */
   __IM uint32_t RESERVED[127];
        PROT_MPU_MPU_STRUCT_Type MPU_STRUCT[16]; /*!< 0x00000200 MPU structure                                                 */
} PROT_MPU_Type;                                /*!< Size = 1024 (0x400)                                                       */

/**
  * \brief Protection (PROT)
  */
typedef struct {                                /*!< PROT Structure                                                            */
        PROT_SMPU_Type SMPU;                    /*!< 0x00000000 SMPU                                                           */
        PROT_MPU_Type CYMPU[16];                /*!< 0x00004000 MPU                                                            */
} PROT_Type;                                    /*!< Size = 32768 (0x8000)                                                     */


/* ==============================================  PROT_SMPU_SMPU_STRUCT.ADDR0  ============================================== */
#define PROT_SMPU_SMPU_STRUCT_ADDR0_SUBREGION_DISABLE_Pos (0UL) /*!< PROT_SMPU_SMPU_STRUCT ADDR0: SUBREGION_DISABLE (Bit 0)    */
#define PROT_SMPU_SMPU_STRUCT_ADDR0_SUBREGION_DISABLE_Msk (0xffUL) /*!< PROT_SMPU_SMPU_STRUCT ADDR0: SUBREGION_DISABLE (Bitfield-Mask: 0xff)*/
#define PROT_SMPU_SMPU_STRUCT_ADDR0_ADDR24_Pos (8UL)            /*!< PROT_SMPU_SMPU_STRUCT ADDR0: ADDR24 (Bit 8)               */
#define PROT_SMPU_SMPU_STRUCT_ADDR0_ADDR24_Msk (0xffffff00UL)   /*!< PROT_SMPU_SMPU_STRUCT ADDR0: ADDR24 (Bitfield-Mask: 0xffffff)*/
/* ==============================================  PROT_SMPU_SMPU_STRUCT.ATT0  =============================================== */
#define PROT_SMPU_SMPU_STRUCT_ATT0_UR_Pos (0UL)                 /*!< PROT_SMPU_SMPU_STRUCT ATT0: UR (Bit 0)                    */
#define PROT_SMPU_SMPU_STRUCT_ATT0_UR_Msk (0x1UL)               /*!< PROT_SMPU_SMPU_STRUCT ATT0: UR (Bitfield-Mask: 0x01)      */
#define PROT_SMPU_SMPU_STRUCT_ATT0_UW_Pos (1UL)                 /*!< PROT_SMPU_SMPU_STRUCT ATT0: UW (Bit 1)                    */
#define PROT_SMPU_SMPU_STRUCT_ATT0_UW_Msk (0x2UL)               /*!< PROT_SMPU_SMPU_STRUCT ATT0: UW (Bitfield-Mask: 0x01)      */
#define PROT_SMPU_SMPU_STRUCT_ATT0_UX_Pos (2UL)                 /*!< PROT_SMPU_SMPU_STRUCT ATT0: UX (Bit 2)                    */
#define PROT_SMPU_SMPU_STRUCT_ATT0_UX_Msk (0x4UL)               /*!< PROT_SMPU_SMPU_STRUCT ATT0: UX (Bitfield-Mask: 0x01)      */
#define PROT_SMPU_SMPU_STRUCT_ATT0_PR_Pos (3UL)                 /*!< PROT_SMPU_SMPU_STRUCT ATT0: PR (Bit 3)                    */
#define PROT_SMPU_SMPU_STRUCT_ATT0_PR_Msk (0x8UL)               /*!< PROT_SMPU_SMPU_STRUCT ATT0: PR (Bitfield-Mask: 0x01)      */
#define PROT_SMPU_SMPU_STRUCT_ATT0_PW_Pos (4UL)                 /*!< PROT_SMPU_SMPU_STRUCT ATT0: PW (Bit 4)                    */
#define PROT_SMPU_SMPU_STRUCT_ATT0_PW_Msk (0x10UL)              /*!< PROT_SMPU_SMPU_STRUCT ATT0: PW (Bitfield-Mask: 0x01)      */
#define PROT_SMPU_SMPU_STRUCT_ATT0_PX_Pos (5UL)                 /*!< PROT_SMPU_SMPU_STRUCT ATT0: PX (Bit 5)                    */
#define PROT_SMPU_SMPU_STRUCT_ATT0_PX_Msk (0x20UL)              /*!< PROT_SMPU_SMPU_STRUCT ATT0: PX (Bitfield-Mask: 0x01)      */
#define PROT_SMPU_SMPU_STRUCT_ATT0_NS_Pos (6UL)                 /*!< PROT_SMPU_SMPU_STRUCT ATT0: NS (Bit 6)                    */
#define PROT_SMPU_SMPU_STRUCT_ATT0_NS_Msk (0x40UL)              /*!< PROT_SMPU_SMPU_STRUCT ATT0: NS (Bitfield-Mask: 0x01)      */
#define PROT_SMPU_SMPU_STRUCT_ATT0_PC_MASK_0_Pos (8UL)          /*!< PROT_SMPU_SMPU_STRUCT ATT0: PC_MASK_0 (Bit 8)             */
#define PROT_SMPU_SMPU_STRUCT_ATT0_PC_MASK_0_Msk (0x100UL)      /*!< PROT_SMPU_SMPU_STRUCT ATT0: PC_MASK_0 (Bitfield-Mask: 0x01)*/
#define PROT_SMPU_SMPU_STRUCT_ATT0_PC_MASK_15_TO_1_Pos (9UL)    /*!< PROT_SMPU_SMPU_STRUCT ATT0: PC_MASK_15_TO_1 (Bit 9)       */
#define PROT_SMPU_SMPU_STRUCT_ATT0_PC_MASK_15_TO_1_Msk (0xfffe00UL) /*!< PROT_SMPU_SMPU_STRUCT ATT0: PC_MASK_15_TO_1 (Bitfield-Mask: 0x7fff)*/
#define PROT_SMPU_SMPU_STRUCT_ATT0_REGION_SIZE_Pos (24UL)       /*!< PROT_SMPU_SMPU_STRUCT ATT0: REGION_SIZE (Bit 24)          */
#define PROT_SMPU_SMPU_STRUCT_ATT0_REGION_SIZE_Msk (0x1f000000UL) /*!< PROT_SMPU_SMPU_STRUCT ATT0: REGION_SIZE (Bitfield-Mask: 0x1f)*/
#define PROT_SMPU_SMPU_STRUCT_ATT0_PC_MATCH_Pos (30UL)          /*!< PROT_SMPU_SMPU_STRUCT ATT0: PC_MATCH (Bit 30)             */
#define PROT_SMPU_SMPU_STRUCT_ATT0_PC_MATCH_Msk (0x40000000UL)  /*!< PROT_SMPU_SMPU_STRUCT ATT0: PC_MATCH (Bitfield-Mask: 0x01)*/
#define PROT_SMPU_SMPU_STRUCT_ATT0_ENABLED_Pos (31UL)           /*!< PROT_SMPU_SMPU_STRUCT ATT0: ENABLED (Bit 31)              */
#define PROT_SMPU_SMPU_STRUCT_ATT0_ENABLED_Msk (0x80000000UL)   /*!< PROT_SMPU_SMPU_STRUCT ATT0: ENABLED (Bitfield-Mask: 0x01) */
/* ==============================================  PROT_SMPU_SMPU_STRUCT.ADDR1  ============================================== */
#define PROT_SMPU_SMPU_STRUCT_ADDR1_SUBREGION_DISABLE_Pos (0UL) /*!< PROT_SMPU_SMPU_STRUCT ADDR1: SUBREGION_DISABLE (Bit 0)    */
#define PROT_SMPU_SMPU_STRUCT_ADDR1_SUBREGION_DISABLE_Msk (0xffUL) /*!< PROT_SMPU_SMPU_STRUCT ADDR1: SUBREGION_DISABLE (Bitfield-Mask: 0xff)*/
#define PROT_SMPU_SMPU_STRUCT_ADDR1_ADDR24_Pos (8UL)            /*!< PROT_SMPU_SMPU_STRUCT ADDR1: ADDR24 (Bit 8)               */
#define PROT_SMPU_SMPU_STRUCT_ADDR1_ADDR24_Msk (0xffffff00UL)   /*!< PROT_SMPU_SMPU_STRUCT ADDR1: ADDR24 (Bitfield-Mask: 0xffffff)*/
/* ==============================================  PROT_SMPU_SMPU_STRUCT.ATT1  =============================================== */
#define PROT_SMPU_SMPU_STRUCT_ATT1_UR_Pos (0UL)                 /*!< PROT_SMPU_SMPU_STRUCT ATT1: UR (Bit 0)                    */
#define PROT_SMPU_SMPU_STRUCT_ATT1_UR_Msk (0x1UL)               /*!< PROT_SMPU_SMPU_STRUCT ATT1: UR (Bitfield-Mask: 0x01)      */
#define PROT_SMPU_SMPU_STRUCT_ATT1_UW_Pos (1UL)                 /*!< PROT_SMPU_SMPU_STRUCT ATT1: UW (Bit 1)                    */
#define PROT_SMPU_SMPU_STRUCT_ATT1_UW_Msk (0x2UL)               /*!< PROT_SMPU_SMPU_STRUCT ATT1: UW (Bitfield-Mask: 0x01)      */
#define PROT_SMPU_SMPU_STRUCT_ATT1_UX_Pos (2UL)                 /*!< PROT_SMPU_SMPU_STRUCT ATT1: UX (Bit 2)                    */
#define PROT_SMPU_SMPU_STRUCT_ATT1_UX_Msk (0x4UL)               /*!< PROT_SMPU_SMPU_STRUCT ATT1: UX (Bitfield-Mask: 0x01)      */
#define PROT_SMPU_SMPU_STRUCT_ATT1_PR_Pos (3UL)                 /*!< PROT_SMPU_SMPU_STRUCT ATT1: PR (Bit 3)                    */
#define PROT_SMPU_SMPU_STRUCT_ATT1_PR_Msk (0x8UL)               /*!< PROT_SMPU_SMPU_STRUCT ATT1: PR (Bitfield-Mask: 0x01)      */
#define PROT_SMPU_SMPU_STRUCT_ATT1_PW_Pos (4UL)                 /*!< PROT_SMPU_SMPU_STRUCT ATT1: PW (Bit 4)                    */
#define PROT_SMPU_SMPU_STRUCT_ATT1_PW_Msk (0x10UL)              /*!< PROT_SMPU_SMPU_STRUCT ATT1: PW (Bitfield-Mask: 0x01)      */
#define PROT_SMPU_SMPU_STRUCT_ATT1_PX_Pos (5UL)                 /*!< PROT_SMPU_SMPU_STRUCT ATT1: PX (Bit 5)                    */
#define PROT_SMPU_SMPU_STRUCT_ATT1_PX_Msk (0x20UL)              /*!< PROT_SMPU_SMPU_STRUCT ATT1: PX (Bitfield-Mask: 0x01)      */
#define PROT_SMPU_SMPU_STRUCT_ATT1_NS_Pos (6UL)                 /*!< PROT_SMPU_SMPU_STRUCT ATT1: NS (Bit 6)                    */
#define PROT_SMPU_SMPU_STRUCT_ATT1_NS_Msk (0x40UL)              /*!< PROT_SMPU_SMPU_STRUCT ATT1: NS (Bitfield-Mask: 0x01)      */
#define PROT_SMPU_SMPU_STRUCT_ATT1_PC_MASK_0_Pos (8UL)          /*!< PROT_SMPU_SMPU_STRUCT ATT1: PC_MASK_0 (Bit 8)             */
#define PROT_SMPU_SMPU_STRUCT_ATT1_PC_MASK_0_Msk (0x100UL)      /*!< PROT_SMPU_SMPU_STRUCT ATT1: PC_MASK_0 (Bitfield-Mask: 0x01)*/
#define PROT_SMPU_SMPU_STRUCT_ATT1_PC_MASK_15_TO_1_Pos (9UL)    /*!< PROT_SMPU_SMPU_STRUCT ATT1: PC_MASK_15_TO_1 (Bit 9)       */
#define PROT_SMPU_SMPU_STRUCT_ATT1_PC_MASK_15_TO_1_Msk (0xfffe00UL) /*!< PROT_SMPU_SMPU_STRUCT ATT1: PC_MASK_15_TO_1 (Bitfield-Mask: 0x7fff)*/
#define PROT_SMPU_SMPU_STRUCT_ATT1_REGION_SIZE_Pos (24UL)       /*!< PROT_SMPU_SMPU_STRUCT ATT1: REGION_SIZE (Bit 24)          */
#define PROT_SMPU_SMPU_STRUCT_ATT1_REGION_SIZE_Msk (0x1f000000UL) /*!< PROT_SMPU_SMPU_STRUCT ATT1: REGION_SIZE (Bitfield-Mask: 0x1f)*/
#define PROT_SMPU_SMPU_STRUCT_ATT1_PC_MATCH_Pos (30UL)          /*!< PROT_SMPU_SMPU_STRUCT ATT1: PC_MATCH (Bit 30)             */
#define PROT_SMPU_SMPU_STRUCT_ATT1_PC_MATCH_Msk (0x40000000UL)  /*!< PROT_SMPU_SMPU_STRUCT ATT1: PC_MATCH (Bitfield-Mask: 0x01)*/
#define PROT_SMPU_SMPU_STRUCT_ATT1_ENABLED_Pos (31UL)           /*!< PROT_SMPU_SMPU_STRUCT ATT1: ENABLED (Bit 31)              */
#define PROT_SMPU_SMPU_STRUCT_ATT1_ENABLED_Msk (0x80000000UL)   /*!< PROT_SMPU_SMPU_STRUCT ATT1: ENABLED (Bitfield-Mask: 0x01) */


/* ===================================================  PROT_SMPU.MS0_CTL  =================================================== */
#define PROT_SMPU_MS0_CTL_P_Pos           (0UL)                 /*!< PROT_SMPU MS0_CTL: P (Bit 0)                              */
#define PROT_SMPU_MS0_CTL_P_Msk           (0x1UL)               /*!< PROT_SMPU MS0_CTL: P (Bitfield-Mask: 0x01)                */
#define PROT_SMPU_MS0_CTL_NS_Pos          (1UL)                 /*!< PROT_SMPU MS0_CTL: NS (Bit 1)                             */
#define PROT_SMPU_MS0_CTL_NS_Msk          (0x2UL)               /*!< PROT_SMPU MS0_CTL: NS (Bitfield-Mask: 0x01)               */
#define PROT_SMPU_MS0_CTL_PRIO_Pos        (8UL)                 /*!< PROT_SMPU MS0_CTL: PRIO (Bit 8)                           */
#define PROT_SMPU_MS0_CTL_PRIO_Msk        (0x300UL)             /*!< PROT_SMPU MS0_CTL: PRIO (Bitfield-Mask: 0x03)             */
#define PROT_SMPU_MS0_CTL_PC_MASK_0_Pos   (16UL)                /*!< PROT_SMPU MS0_CTL: PC_MASK_0 (Bit 16)                     */
#define PROT_SMPU_MS0_CTL_PC_MASK_0_Msk   (0x10000UL)           /*!< PROT_SMPU MS0_CTL: PC_MASK_0 (Bitfield-Mask: 0x01)        */
#define PROT_SMPU_MS0_CTL_PC_MASK_15_TO_1_Pos (17UL)            /*!< PROT_SMPU MS0_CTL: PC_MASK_15_TO_1 (Bit 17)               */
#define PROT_SMPU_MS0_CTL_PC_MASK_15_TO_1_Msk (0xfffe0000UL)    /*!< PROT_SMPU MS0_CTL: PC_MASK_15_TO_1 (Bitfield-Mask: 0x7fff)*/
/* ===================================================  PROT_SMPU.MS1_CTL  =================================================== */
#define PROT_SMPU_MS1_CTL_P_Pos           (0UL)                 /*!< PROT_SMPU MS1_CTL: P (Bit 0)                              */
#define PROT_SMPU_MS1_CTL_P_Msk           (0x1UL)               /*!< PROT_SMPU MS1_CTL: P (Bitfield-Mask: 0x01)                */
#define PROT_SMPU_MS1_CTL_NS_Pos          (1UL)                 /*!< PROT_SMPU MS1_CTL: NS (Bit 1)                             */
#define PROT_SMPU_MS1_CTL_NS_Msk          (0x2UL)               /*!< PROT_SMPU MS1_CTL: NS (Bitfield-Mask: 0x01)               */
#define PROT_SMPU_MS1_CTL_PRIO_Pos        (8UL)                 /*!< PROT_SMPU MS1_CTL: PRIO (Bit 8)                           */
#define PROT_SMPU_MS1_CTL_PRIO_Msk        (0x300UL)             /*!< PROT_SMPU MS1_CTL: PRIO (Bitfield-Mask: 0x03)             */
#define PROT_SMPU_MS1_CTL_PC_MASK_0_Pos   (16UL)                /*!< PROT_SMPU MS1_CTL: PC_MASK_0 (Bit 16)                     */
#define PROT_SMPU_MS1_CTL_PC_MASK_0_Msk   (0x10000UL)           /*!< PROT_SMPU MS1_CTL: PC_MASK_0 (Bitfield-Mask: 0x01)        */
#define PROT_SMPU_MS1_CTL_PC_MASK_15_TO_1_Pos (17UL)            /*!< PROT_SMPU MS1_CTL: PC_MASK_15_TO_1 (Bit 17)               */
#define PROT_SMPU_MS1_CTL_PC_MASK_15_TO_1_Msk (0xfffe0000UL)    /*!< PROT_SMPU MS1_CTL: PC_MASK_15_TO_1 (Bitfield-Mask: 0x7fff)*/
/* ===================================================  PROT_SMPU.MS2_CTL  =================================================== */
#define PROT_SMPU_MS2_CTL_P_Pos           (0UL)                 /*!< PROT_SMPU MS2_CTL: P (Bit 0)                              */
#define PROT_SMPU_MS2_CTL_P_Msk           (0x1UL)               /*!< PROT_SMPU MS2_CTL: P (Bitfield-Mask: 0x01)                */
#define PROT_SMPU_MS2_CTL_NS_Pos          (1UL)                 /*!< PROT_SMPU MS2_CTL: NS (Bit 1)                             */
#define PROT_SMPU_MS2_CTL_NS_Msk          (0x2UL)               /*!< PROT_SMPU MS2_CTL: NS (Bitfield-Mask: 0x01)               */
#define PROT_SMPU_MS2_CTL_PRIO_Pos        (8UL)                 /*!< PROT_SMPU MS2_CTL: PRIO (Bit 8)                           */
#define PROT_SMPU_MS2_CTL_PRIO_Msk        (0x300UL)             /*!< PROT_SMPU MS2_CTL: PRIO (Bitfield-Mask: 0x03)             */
#define PROT_SMPU_MS2_CTL_PC_MASK_0_Pos   (16UL)                /*!< PROT_SMPU MS2_CTL: PC_MASK_0 (Bit 16)                     */
#define PROT_SMPU_MS2_CTL_PC_MASK_0_Msk   (0x10000UL)           /*!< PROT_SMPU MS2_CTL: PC_MASK_0 (Bitfield-Mask: 0x01)        */
#define PROT_SMPU_MS2_CTL_PC_MASK_15_TO_1_Pos (17UL)            /*!< PROT_SMPU MS2_CTL: PC_MASK_15_TO_1 (Bit 17)               */
#define PROT_SMPU_MS2_CTL_PC_MASK_15_TO_1_Msk (0xfffe0000UL)    /*!< PROT_SMPU MS2_CTL: PC_MASK_15_TO_1 (Bitfield-Mask: 0x7fff)*/
/* ===================================================  PROT_SMPU.MS3_CTL  =================================================== */
#define PROT_SMPU_MS3_CTL_P_Pos           (0UL)                 /*!< PROT_SMPU MS3_CTL: P (Bit 0)                              */
#define PROT_SMPU_MS3_CTL_P_Msk           (0x1UL)               /*!< PROT_SMPU MS3_CTL: P (Bitfield-Mask: 0x01)                */
#define PROT_SMPU_MS3_CTL_NS_Pos          (1UL)                 /*!< PROT_SMPU MS3_CTL: NS (Bit 1)                             */
#define PROT_SMPU_MS3_CTL_NS_Msk          (0x2UL)               /*!< PROT_SMPU MS3_CTL: NS (Bitfield-Mask: 0x01)               */
#define PROT_SMPU_MS3_CTL_PRIO_Pos        (8UL)                 /*!< PROT_SMPU MS3_CTL: PRIO (Bit 8)                           */
#define PROT_SMPU_MS3_CTL_PRIO_Msk        (0x300UL)             /*!< PROT_SMPU MS3_CTL: PRIO (Bitfield-Mask: 0x03)             */
#define PROT_SMPU_MS3_CTL_PC_MASK_0_Pos   (16UL)                /*!< PROT_SMPU MS3_CTL: PC_MASK_0 (Bit 16)                     */
#define PROT_SMPU_MS3_CTL_PC_MASK_0_Msk   (0x10000UL)           /*!< PROT_SMPU MS3_CTL: PC_MASK_0 (Bitfield-Mask: 0x01)        */
#define PROT_SMPU_MS3_CTL_PC_MASK_15_TO_1_Pos (17UL)            /*!< PROT_SMPU MS3_CTL: PC_MASK_15_TO_1 (Bit 17)               */
#define PROT_SMPU_MS3_CTL_PC_MASK_15_TO_1_Msk (0xfffe0000UL)    /*!< PROT_SMPU MS3_CTL: PC_MASK_15_TO_1 (Bitfield-Mask: 0x7fff)*/
/* ===================================================  PROT_SMPU.MS4_CTL  =================================================== */
#define PROT_SMPU_MS4_CTL_P_Pos           (0UL)                 /*!< PROT_SMPU MS4_CTL: P (Bit 0)                              */
#define PROT_SMPU_MS4_CTL_P_Msk           (0x1UL)               /*!< PROT_SMPU MS4_CTL: P (Bitfield-Mask: 0x01)                */
#define PROT_SMPU_MS4_CTL_NS_Pos          (1UL)                 /*!< PROT_SMPU MS4_CTL: NS (Bit 1)                             */
#define PROT_SMPU_MS4_CTL_NS_Msk          (0x2UL)               /*!< PROT_SMPU MS4_CTL: NS (Bitfield-Mask: 0x01)               */
#define PROT_SMPU_MS4_CTL_PRIO_Pos        (8UL)                 /*!< PROT_SMPU MS4_CTL: PRIO (Bit 8)                           */
#define PROT_SMPU_MS4_CTL_PRIO_Msk        (0x300UL)             /*!< PROT_SMPU MS4_CTL: PRIO (Bitfield-Mask: 0x03)             */
#define PROT_SMPU_MS4_CTL_PC_MASK_0_Pos   (16UL)                /*!< PROT_SMPU MS4_CTL: PC_MASK_0 (Bit 16)                     */
#define PROT_SMPU_MS4_CTL_PC_MASK_0_Msk   (0x10000UL)           /*!< PROT_SMPU MS4_CTL: PC_MASK_0 (Bitfield-Mask: 0x01)        */
#define PROT_SMPU_MS4_CTL_PC_MASK_15_TO_1_Pos (17UL)            /*!< PROT_SMPU MS4_CTL: PC_MASK_15_TO_1 (Bit 17)               */
#define PROT_SMPU_MS4_CTL_PC_MASK_15_TO_1_Msk (0xfffe0000UL)    /*!< PROT_SMPU MS4_CTL: PC_MASK_15_TO_1 (Bitfield-Mask: 0x7fff)*/
/* ===================================================  PROT_SMPU.MS5_CTL  =================================================== */
#define PROT_SMPU_MS5_CTL_P_Pos           (0UL)                 /*!< PROT_SMPU MS5_CTL: P (Bit 0)                              */
#define PROT_SMPU_MS5_CTL_P_Msk           (0x1UL)               /*!< PROT_SMPU MS5_CTL: P (Bitfield-Mask: 0x01)                */
#define PROT_SMPU_MS5_CTL_NS_Pos          (1UL)                 /*!< PROT_SMPU MS5_CTL: NS (Bit 1)                             */
#define PROT_SMPU_MS5_CTL_NS_Msk          (0x2UL)               /*!< PROT_SMPU MS5_CTL: NS (Bitfield-Mask: 0x01)               */
#define PROT_SMPU_MS5_CTL_PRIO_Pos        (8UL)                 /*!< PROT_SMPU MS5_CTL: PRIO (Bit 8)                           */
#define PROT_SMPU_MS5_CTL_PRIO_Msk        (0x300UL)             /*!< PROT_SMPU MS5_CTL: PRIO (Bitfield-Mask: 0x03)             */
#define PROT_SMPU_MS5_CTL_PC_MASK_0_Pos   (16UL)                /*!< PROT_SMPU MS5_CTL: PC_MASK_0 (Bit 16)                     */
#define PROT_SMPU_MS5_CTL_PC_MASK_0_Msk   (0x10000UL)           /*!< PROT_SMPU MS5_CTL: PC_MASK_0 (Bitfield-Mask: 0x01)        */
#define PROT_SMPU_MS5_CTL_PC_MASK_15_TO_1_Pos (17UL)            /*!< PROT_SMPU MS5_CTL: PC_MASK_15_TO_1 (Bit 17)               */
#define PROT_SMPU_MS5_CTL_PC_MASK_15_TO_1_Msk (0xfffe0000UL)    /*!< PROT_SMPU MS5_CTL: PC_MASK_15_TO_1 (Bitfield-Mask: 0x7fff)*/
/* ===================================================  PROT_SMPU.MS6_CTL  =================================================== */
#define PROT_SMPU_MS6_CTL_P_Pos           (0UL)                 /*!< PROT_SMPU MS6_CTL: P (Bit 0)                              */
#define PROT_SMPU_MS6_CTL_P_Msk           (0x1UL)               /*!< PROT_SMPU MS6_CTL: P (Bitfield-Mask: 0x01)                */
#define PROT_SMPU_MS6_CTL_NS_Pos          (1UL)                 /*!< PROT_SMPU MS6_CTL: NS (Bit 1)                             */
#define PROT_SMPU_MS6_CTL_NS_Msk          (0x2UL)               /*!< PROT_SMPU MS6_CTL: NS (Bitfield-Mask: 0x01)               */
#define PROT_SMPU_MS6_CTL_PRIO_Pos        (8UL)                 /*!< PROT_SMPU MS6_CTL: PRIO (Bit 8)                           */
#define PROT_SMPU_MS6_CTL_PRIO_Msk        (0x300UL)             /*!< PROT_SMPU MS6_CTL: PRIO (Bitfield-Mask: 0x03)             */
#define PROT_SMPU_MS6_CTL_PC_MASK_0_Pos   (16UL)                /*!< PROT_SMPU MS6_CTL: PC_MASK_0 (Bit 16)                     */
#define PROT_SMPU_MS6_CTL_PC_MASK_0_Msk   (0x10000UL)           /*!< PROT_SMPU MS6_CTL: PC_MASK_0 (Bitfield-Mask: 0x01)        */
#define PROT_SMPU_MS6_CTL_PC_MASK_15_TO_1_Pos (17UL)            /*!< PROT_SMPU MS6_CTL: PC_MASK_15_TO_1 (Bit 17)               */
#define PROT_SMPU_MS6_CTL_PC_MASK_15_TO_1_Msk (0xfffe0000UL)    /*!< PROT_SMPU MS6_CTL: PC_MASK_15_TO_1 (Bitfield-Mask: 0x7fff)*/
/* ===================================================  PROT_SMPU.MS7_CTL  =================================================== */
#define PROT_SMPU_MS7_CTL_P_Pos           (0UL)                 /*!< PROT_SMPU MS7_CTL: P (Bit 0)                              */
#define PROT_SMPU_MS7_CTL_P_Msk           (0x1UL)               /*!< PROT_SMPU MS7_CTL: P (Bitfield-Mask: 0x01)                */
#define PROT_SMPU_MS7_CTL_NS_Pos          (1UL)                 /*!< PROT_SMPU MS7_CTL: NS (Bit 1)                             */
#define PROT_SMPU_MS7_CTL_NS_Msk          (0x2UL)               /*!< PROT_SMPU MS7_CTL: NS (Bitfield-Mask: 0x01)               */
#define PROT_SMPU_MS7_CTL_PRIO_Pos        (8UL)                 /*!< PROT_SMPU MS7_CTL: PRIO (Bit 8)                           */
#define PROT_SMPU_MS7_CTL_PRIO_Msk        (0x300UL)             /*!< PROT_SMPU MS7_CTL: PRIO (Bitfield-Mask: 0x03)             */
#define PROT_SMPU_MS7_CTL_PC_MASK_0_Pos   (16UL)                /*!< PROT_SMPU MS7_CTL: PC_MASK_0 (Bit 16)                     */
#define PROT_SMPU_MS7_CTL_PC_MASK_0_Msk   (0x10000UL)           /*!< PROT_SMPU MS7_CTL: PC_MASK_0 (Bitfield-Mask: 0x01)        */
#define PROT_SMPU_MS7_CTL_PC_MASK_15_TO_1_Pos (17UL)            /*!< PROT_SMPU MS7_CTL: PC_MASK_15_TO_1 (Bit 17)               */
#define PROT_SMPU_MS7_CTL_PC_MASK_15_TO_1_Msk (0xfffe0000UL)    /*!< PROT_SMPU MS7_CTL: PC_MASK_15_TO_1 (Bitfield-Mask: 0x7fff)*/
/* ===================================================  PROT_SMPU.MS8_CTL  =================================================== */
#define PROT_SMPU_MS8_CTL_P_Pos           (0UL)                 /*!< PROT_SMPU MS8_CTL: P (Bit 0)                              */
#define PROT_SMPU_MS8_CTL_P_Msk           (0x1UL)               /*!< PROT_SMPU MS8_CTL: P (Bitfield-Mask: 0x01)                */
#define PROT_SMPU_MS8_CTL_NS_Pos          (1UL)                 /*!< PROT_SMPU MS8_CTL: NS (Bit 1)                             */
#define PROT_SMPU_MS8_CTL_NS_Msk          (0x2UL)               /*!< PROT_SMPU MS8_CTL: NS (Bitfield-Mask: 0x01)               */
#define PROT_SMPU_MS8_CTL_PRIO_Pos        (8UL)                 /*!< PROT_SMPU MS8_CTL: PRIO (Bit 8)                           */
#define PROT_SMPU_MS8_CTL_PRIO_Msk        (0x300UL)             /*!< PROT_SMPU MS8_CTL: PRIO (Bitfield-Mask: 0x03)             */
#define PROT_SMPU_MS8_CTL_PC_MASK_0_Pos   (16UL)                /*!< PROT_SMPU MS8_CTL: PC_MASK_0 (Bit 16)                     */
#define PROT_SMPU_MS8_CTL_PC_MASK_0_Msk   (0x10000UL)           /*!< PROT_SMPU MS8_CTL: PC_MASK_0 (Bitfield-Mask: 0x01)        */
#define PROT_SMPU_MS8_CTL_PC_MASK_15_TO_1_Pos (17UL)            /*!< PROT_SMPU MS8_CTL: PC_MASK_15_TO_1 (Bit 17)               */
#define PROT_SMPU_MS8_CTL_PC_MASK_15_TO_1_Msk (0xfffe0000UL)    /*!< PROT_SMPU MS8_CTL: PC_MASK_15_TO_1 (Bitfield-Mask: 0x7fff)*/
/* ===================================================  PROT_SMPU.MS9_CTL  =================================================== */
#define PROT_SMPU_MS9_CTL_P_Pos           (0UL)                 /*!< PROT_SMPU MS9_CTL: P (Bit 0)                              */
#define PROT_SMPU_MS9_CTL_P_Msk           (0x1UL)               /*!< PROT_SMPU MS9_CTL: P (Bitfield-Mask: 0x01)                */
#define PROT_SMPU_MS9_CTL_NS_Pos          (1UL)                 /*!< PROT_SMPU MS9_CTL: NS (Bit 1)                             */
#define PROT_SMPU_MS9_CTL_NS_Msk          (0x2UL)               /*!< PROT_SMPU MS9_CTL: NS (Bitfield-Mask: 0x01)               */
#define PROT_SMPU_MS9_CTL_PRIO_Pos        (8UL)                 /*!< PROT_SMPU MS9_CTL: PRIO (Bit 8)                           */
#define PROT_SMPU_MS9_CTL_PRIO_Msk        (0x300UL)             /*!< PROT_SMPU MS9_CTL: PRIO (Bitfield-Mask: 0x03)             */
#define PROT_SMPU_MS9_CTL_PC_MASK_0_Pos   (16UL)                /*!< PROT_SMPU MS9_CTL: PC_MASK_0 (Bit 16)                     */
#define PROT_SMPU_MS9_CTL_PC_MASK_0_Msk   (0x10000UL)           /*!< PROT_SMPU MS9_CTL: PC_MASK_0 (Bitfield-Mask: 0x01)        */
#define PROT_SMPU_MS9_CTL_PC_MASK_15_TO_1_Pos (17UL)            /*!< PROT_SMPU MS9_CTL: PC_MASK_15_TO_1 (Bit 17)               */
#define PROT_SMPU_MS9_CTL_PC_MASK_15_TO_1_Msk (0xfffe0000UL)    /*!< PROT_SMPU MS9_CTL: PC_MASK_15_TO_1 (Bitfield-Mask: 0x7fff)*/
/* ==================================================  PROT_SMPU.MS10_CTL  =================================================== */
#define PROT_SMPU_MS10_CTL_P_Pos          (0UL)                 /*!< PROT_SMPU MS10_CTL: P (Bit 0)                             */
#define PROT_SMPU_MS10_CTL_P_Msk          (0x1UL)               /*!< PROT_SMPU MS10_CTL: P (Bitfield-Mask: 0x01)               */
#define PROT_SMPU_MS10_CTL_NS_Pos         (1UL)                 /*!< PROT_SMPU MS10_CTL: NS (Bit 1)                            */
#define PROT_SMPU_MS10_CTL_NS_Msk         (0x2UL)               /*!< PROT_SMPU MS10_CTL: NS (Bitfield-Mask: 0x01)              */
#define PROT_SMPU_MS10_CTL_PRIO_Pos       (8UL)                 /*!< PROT_SMPU MS10_CTL: PRIO (Bit 8)                          */
#define PROT_SMPU_MS10_CTL_PRIO_Msk       (0x300UL)             /*!< PROT_SMPU MS10_CTL: PRIO (Bitfield-Mask: 0x03)            */
#define PROT_SMPU_MS10_CTL_PC_MASK_0_Pos  (16UL)                /*!< PROT_SMPU MS10_CTL: PC_MASK_0 (Bit 16)                    */
#define PROT_SMPU_MS10_CTL_PC_MASK_0_Msk  (0x10000UL)           /*!< PROT_SMPU MS10_CTL: PC_MASK_0 (Bitfield-Mask: 0x01)       */
#define PROT_SMPU_MS10_CTL_PC_MASK_15_TO_1_Pos (17UL)           /*!< PROT_SMPU MS10_CTL: PC_MASK_15_TO_1 (Bit 17)              */
#define PROT_SMPU_MS10_CTL_PC_MASK_15_TO_1_Msk (0xfffe0000UL)   /*!< PROT_SMPU MS10_CTL: PC_MASK_15_TO_1 (Bitfield-Mask: 0x7fff)*/
/* ==================================================  PROT_SMPU.MS11_CTL  =================================================== */
#define PROT_SMPU_MS11_CTL_P_Pos          (0UL)                 /*!< PROT_SMPU MS11_CTL: P (Bit 0)                             */
#define PROT_SMPU_MS11_CTL_P_Msk          (0x1UL)               /*!< PROT_SMPU MS11_CTL: P (Bitfield-Mask: 0x01)               */
#define PROT_SMPU_MS11_CTL_NS_Pos         (1UL)                 /*!< PROT_SMPU MS11_CTL: NS (Bit 1)                            */
#define PROT_SMPU_MS11_CTL_NS_Msk         (0x2UL)               /*!< PROT_SMPU MS11_CTL: NS (Bitfield-Mask: 0x01)              */
#define PROT_SMPU_MS11_CTL_PRIO_Pos       (8UL)                 /*!< PROT_SMPU MS11_CTL: PRIO (Bit 8)                          */
#define PROT_SMPU_MS11_CTL_PRIO_Msk       (0x300UL)             /*!< PROT_SMPU MS11_CTL: PRIO (Bitfield-Mask: 0x03)            */
#define PROT_SMPU_MS11_CTL_PC_MASK_0_Pos  (16UL)                /*!< PROT_SMPU MS11_CTL: PC_MASK_0 (Bit 16)                    */
#define PROT_SMPU_MS11_CTL_PC_MASK_0_Msk  (0x10000UL)           /*!< PROT_SMPU MS11_CTL: PC_MASK_0 (Bitfield-Mask: 0x01)       */
#define PROT_SMPU_MS11_CTL_PC_MASK_15_TO_1_Pos (17UL)           /*!< PROT_SMPU MS11_CTL: PC_MASK_15_TO_1 (Bit 17)              */
#define PROT_SMPU_MS11_CTL_PC_MASK_15_TO_1_Msk (0xfffe0000UL)   /*!< PROT_SMPU MS11_CTL: PC_MASK_15_TO_1 (Bitfield-Mask: 0x7fff)*/
/* ==================================================  PROT_SMPU.MS12_CTL  =================================================== */
#define PROT_SMPU_MS12_CTL_P_Pos          (0UL)                 /*!< PROT_SMPU MS12_CTL: P (Bit 0)                             */
#define PROT_SMPU_MS12_CTL_P_Msk          (0x1UL)               /*!< PROT_SMPU MS12_CTL: P (Bitfield-Mask: 0x01)               */
#define PROT_SMPU_MS12_CTL_NS_Pos         (1UL)                 /*!< PROT_SMPU MS12_CTL: NS (Bit 1)                            */
#define PROT_SMPU_MS12_CTL_NS_Msk         (0x2UL)               /*!< PROT_SMPU MS12_CTL: NS (Bitfield-Mask: 0x01)              */
#define PROT_SMPU_MS12_CTL_PRIO_Pos       (8UL)                 /*!< PROT_SMPU MS12_CTL: PRIO (Bit 8)                          */
#define PROT_SMPU_MS12_CTL_PRIO_Msk       (0x300UL)             /*!< PROT_SMPU MS12_CTL: PRIO (Bitfield-Mask: 0x03)            */
#define PROT_SMPU_MS12_CTL_PC_MASK_0_Pos  (16UL)                /*!< PROT_SMPU MS12_CTL: PC_MASK_0 (Bit 16)                    */
#define PROT_SMPU_MS12_CTL_PC_MASK_0_Msk  (0x10000UL)           /*!< PROT_SMPU MS12_CTL: PC_MASK_0 (Bitfield-Mask: 0x01)       */
#define PROT_SMPU_MS12_CTL_PC_MASK_15_TO_1_Pos (17UL)           /*!< PROT_SMPU MS12_CTL: PC_MASK_15_TO_1 (Bit 17)              */
#define PROT_SMPU_MS12_CTL_PC_MASK_15_TO_1_Msk (0xfffe0000UL)   /*!< PROT_SMPU MS12_CTL: PC_MASK_15_TO_1 (Bitfield-Mask: 0x7fff)*/
/* ==================================================  PROT_SMPU.MS13_CTL  =================================================== */
#define PROT_SMPU_MS13_CTL_P_Pos          (0UL)                 /*!< PROT_SMPU MS13_CTL: P (Bit 0)                             */
#define PROT_SMPU_MS13_CTL_P_Msk          (0x1UL)               /*!< PROT_SMPU MS13_CTL: P (Bitfield-Mask: 0x01)               */
#define PROT_SMPU_MS13_CTL_NS_Pos         (1UL)                 /*!< PROT_SMPU MS13_CTL: NS (Bit 1)                            */
#define PROT_SMPU_MS13_CTL_NS_Msk         (0x2UL)               /*!< PROT_SMPU MS13_CTL: NS (Bitfield-Mask: 0x01)              */
#define PROT_SMPU_MS13_CTL_PRIO_Pos       (8UL)                 /*!< PROT_SMPU MS13_CTL: PRIO (Bit 8)                          */
#define PROT_SMPU_MS13_CTL_PRIO_Msk       (0x300UL)             /*!< PROT_SMPU MS13_CTL: PRIO (Bitfield-Mask: 0x03)            */
#define PROT_SMPU_MS13_CTL_PC_MASK_0_Pos  (16UL)                /*!< PROT_SMPU MS13_CTL: PC_MASK_0 (Bit 16)                    */
#define PROT_SMPU_MS13_CTL_PC_MASK_0_Msk  (0x10000UL)           /*!< PROT_SMPU MS13_CTL: PC_MASK_0 (Bitfield-Mask: 0x01)       */
#define PROT_SMPU_MS13_CTL_PC_MASK_15_TO_1_Pos (17UL)           /*!< PROT_SMPU MS13_CTL: PC_MASK_15_TO_1 (Bit 17)              */
#define PROT_SMPU_MS13_CTL_PC_MASK_15_TO_1_Msk (0xfffe0000UL)   /*!< PROT_SMPU MS13_CTL: PC_MASK_15_TO_1 (Bitfield-Mask: 0x7fff)*/
/* ==================================================  PROT_SMPU.MS14_CTL  =================================================== */
#define PROT_SMPU_MS14_CTL_P_Pos          (0UL)                 /*!< PROT_SMPU MS14_CTL: P (Bit 0)                             */
#define PROT_SMPU_MS14_CTL_P_Msk          (0x1UL)               /*!< PROT_SMPU MS14_CTL: P (Bitfield-Mask: 0x01)               */
#define PROT_SMPU_MS14_CTL_NS_Pos         (1UL)                 /*!< PROT_SMPU MS14_CTL: NS (Bit 1)                            */
#define PROT_SMPU_MS14_CTL_NS_Msk         (0x2UL)               /*!< PROT_SMPU MS14_CTL: NS (Bitfield-Mask: 0x01)              */
#define PROT_SMPU_MS14_CTL_PRIO_Pos       (8UL)                 /*!< PROT_SMPU MS14_CTL: PRIO (Bit 8)                          */
#define PROT_SMPU_MS14_CTL_PRIO_Msk       (0x300UL)             /*!< PROT_SMPU MS14_CTL: PRIO (Bitfield-Mask: 0x03)            */
#define PROT_SMPU_MS14_CTL_PC_MASK_0_Pos  (16UL)                /*!< PROT_SMPU MS14_CTL: PC_MASK_0 (Bit 16)                    */
#define PROT_SMPU_MS14_CTL_PC_MASK_0_Msk  (0x10000UL)           /*!< PROT_SMPU MS14_CTL: PC_MASK_0 (Bitfield-Mask: 0x01)       */
#define PROT_SMPU_MS14_CTL_PC_MASK_15_TO_1_Pos (17UL)           /*!< PROT_SMPU MS14_CTL: PC_MASK_15_TO_1 (Bit 17)              */
#define PROT_SMPU_MS14_CTL_PC_MASK_15_TO_1_Msk (0xfffe0000UL)   /*!< PROT_SMPU MS14_CTL: PC_MASK_15_TO_1 (Bitfield-Mask: 0x7fff)*/
/* ==================================================  PROT_SMPU.MS15_CTL  =================================================== */
#define PROT_SMPU_MS15_CTL_P_Pos          (0UL)                 /*!< PROT_SMPU MS15_CTL: P (Bit 0)                             */
#define PROT_SMPU_MS15_CTL_P_Msk          (0x1UL)               /*!< PROT_SMPU MS15_CTL: P (Bitfield-Mask: 0x01)               */
#define PROT_SMPU_MS15_CTL_NS_Pos         (1UL)                 /*!< PROT_SMPU MS15_CTL: NS (Bit 1)                            */
#define PROT_SMPU_MS15_CTL_NS_Msk         (0x2UL)               /*!< PROT_SMPU MS15_CTL: NS (Bitfield-Mask: 0x01)              */
#define PROT_SMPU_MS15_CTL_PRIO_Pos       (8UL)                 /*!< PROT_SMPU MS15_CTL: PRIO (Bit 8)                          */
#define PROT_SMPU_MS15_CTL_PRIO_Msk       (0x300UL)             /*!< PROT_SMPU MS15_CTL: PRIO (Bitfield-Mask: 0x03)            */
#define PROT_SMPU_MS15_CTL_PC_MASK_0_Pos  (16UL)                /*!< PROT_SMPU MS15_CTL: PC_MASK_0 (Bit 16)                    */
#define PROT_SMPU_MS15_CTL_PC_MASK_0_Msk  (0x10000UL)           /*!< PROT_SMPU MS15_CTL: PC_MASK_0 (Bitfield-Mask: 0x01)       */
#define PROT_SMPU_MS15_CTL_PC_MASK_15_TO_1_Pos (17UL)           /*!< PROT_SMPU MS15_CTL: PC_MASK_15_TO_1 (Bit 17)              */
#define PROT_SMPU_MS15_CTL_PC_MASK_15_TO_1_Msk (0xfffe0000UL)   /*!< PROT_SMPU MS15_CTL: PC_MASK_15_TO_1 (Bitfield-Mask: 0x7fff)*/


/* ===============================================  PROT_MPU_MPU_STRUCT.ADDR  ================================================ */
#define PROT_MPU_MPU_STRUCT_ADDR_SUBREGION_DISABLE_Pos (0UL)    /*!< PROT_MPU_MPU_STRUCT ADDR: SUBREGION_DISABLE (Bit 0)       */
#define PROT_MPU_MPU_STRUCT_ADDR_SUBREGION_DISABLE_Msk (0xffUL) /*!< PROT_MPU_MPU_STRUCT ADDR: SUBREGION_DISABLE (Bitfield-Mask: 0xff)*/
#define PROT_MPU_MPU_STRUCT_ADDR_ADDR24_Pos (8UL)               /*!< PROT_MPU_MPU_STRUCT ADDR: ADDR24 (Bit 8)                  */
#define PROT_MPU_MPU_STRUCT_ADDR_ADDR24_Msk (0xffffff00UL)      /*!< PROT_MPU_MPU_STRUCT ADDR: ADDR24 (Bitfield-Mask: 0xffffff)*/
/* ================================================  PROT_MPU_MPU_STRUCT.ATT  ================================================ */
#define PROT_MPU_MPU_STRUCT_ATT_UR_Pos    (0UL)                 /*!< PROT_MPU_MPU_STRUCT ATT: UR (Bit 0)                       */
#define PROT_MPU_MPU_STRUCT_ATT_UR_Msk    (0x1UL)               /*!< PROT_MPU_MPU_STRUCT ATT: UR (Bitfield-Mask: 0x01)         */
#define PROT_MPU_MPU_STRUCT_ATT_UW_Pos    (1UL)                 /*!< PROT_MPU_MPU_STRUCT ATT: UW (Bit 1)                       */
#define PROT_MPU_MPU_STRUCT_ATT_UW_Msk    (0x2UL)               /*!< PROT_MPU_MPU_STRUCT ATT: UW (Bitfield-Mask: 0x01)         */
#define PROT_MPU_MPU_STRUCT_ATT_UX_Pos    (2UL)                 /*!< PROT_MPU_MPU_STRUCT ATT: UX (Bit 2)                       */
#define PROT_MPU_MPU_STRUCT_ATT_UX_Msk    (0x4UL)               /*!< PROT_MPU_MPU_STRUCT ATT: UX (Bitfield-Mask: 0x01)         */
#define PROT_MPU_MPU_STRUCT_ATT_PR_Pos    (3UL)                 /*!< PROT_MPU_MPU_STRUCT ATT: PR (Bit 3)                       */
#define PROT_MPU_MPU_STRUCT_ATT_PR_Msk    (0x8UL)               /*!< PROT_MPU_MPU_STRUCT ATT: PR (Bitfield-Mask: 0x01)         */
#define PROT_MPU_MPU_STRUCT_ATT_PW_Pos    (4UL)                 /*!< PROT_MPU_MPU_STRUCT ATT: PW (Bit 4)                       */
#define PROT_MPU_MPU_STRUCT_ATT_PW_Msk    (0x10UL)              /*!< PROT_MPU_MPU_STRUCT ATT: PW (Bitfield-Mask: 0x01)         */
#define PROT_MPU_MPU_STRUCT_ATT_PX_Pos    (5UL)                 /*!< PROT_MPU_MPU_STRUCT ATT: PX (Bit 5)                       */
#define PROT_MPU_MPU_STRUCT_ATT_PX_Msk    (0x20UL)              /*!< PROT_MPU_MPU_STRUCT ATT: PX (Bitfield-Mask: 0x01)         */
#define PROT_MPU_MPU_STRUCT_ATT_NS_Pos    (6UL)                 /*!< PROT_MPU_MPU_STRUCT ATT: NS (Bit 6)                       */
#define PROT_MPU_MPU_STRUCT_ATT_NS_Msk    (0x40UL)              /*!< PROT_MPU_MPU_STRUCT ATT: NS (Bitfield-Mask: 0x01)         */
#define PROT_MPU_MPU_STRUCT_ATT_REGION_SIZE_Pos (24UL)          /*!< PROT_MPU_MPU_STRUCT ATT: REGION_SIZE (Bit 24)             */
#define PROT_MPU_MPU_STRUCT_ATT_REGION_SIZE_Msk (0x1f000000UL)  /*!< PROT_MPU_MPU_STRUCT ATT: REGION_SIZE (Bitfield-Mask: 0x1f)*/
#define PROT_MPU_MPU_STRUCT_ATT_ENABLED_Pos (31UL)              /*!< PROT_MPU_MPU_STRUCT ATT: ENABLED (Bit 31)                 */
#define PROT_MPU_MPU_STRUCT_ATT_ENABLED_Msk (0x80000000UL)      /*!< PROT_MPU_MPU_STRUCT ATT: ENABLED (Bitfield-Mask: 0x01)    */


/* ====================================================  PROT_MPU.MS_CTL  ==================================================== */
#define PROT_MPU_MS_CTL_PC_Pos            (0UL)                 /*!< PROT_MPU MS_CTL: PC (Bit 0)                               */
#define PROT_MPU_MS_CTL_PC_Msk            (0xfUL)               /*!< PROT_MPU MS_CTL: PC (Bitfield-Mask: 0x0f)                 */
#define PROT_MPU_MS_CTL_PC_SAVED_Pos      (16UL)                /*!< PROT_MPU MS_CTL: PC_SAVED (Bit 16)                        */
#define PROT_MPU_MS_CTL_PC_SAVED_Msk      (0xf0000UL)           /*!< PROT_MPU MS_CTL: PC_SAVED (Bitfield-Mask: 0x0f)           */


#endif /* _CYIP_PROT_H_ */


/* [] END OF FILE */
