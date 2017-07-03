/***************************************************************************//**
* \file cyip_efuse.h
*
* \brief
* EFUSE IP definitions
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

#ifndef _CYIP_EFUSE_H_
#define _CYIP_EFUSE_H_

#include "cyip_headers.h"

/* =========================================================================================================================== */
/* ================                                           EFUSE                                           ================ */
/* =========================================================================================================================== */

#define EFUSE_SECTION_SIZE          0x00000080UL

/**
  * \brief EFUSE MXS40 registers (EFUSE)
  */
typedef struct {                                /*!< EFUSE Structure                                                           */
  __IOM uint32_t CTL;                           /*!< 0x00000000 Control                                                        */
   __IM uint32_t RESERVED[3];
  __IOM uint32_t CMD;                           /*!< 0x00000010 Command                                                        */
   __IM uint32_t RESERVED1[3];
  __IOM uint32_t SEQ_DEFAULT;                   /*!< 0x00000020 Sequencer Default value                                        */
   __IM uint32_t RESERVED2[7];
  __IOM uint32_t SEQ_READ_CTL_0;                /*!< 0x00000040 Sequencer read control 0                                       */
  __IOM uint32_t SEQ_READ_CTL_1;                /*!< 0x00000044 Sequencer read control 1                                       */
  __IOM uint32_t SEQ_READ_CTL_2;                /*!< 0x00000048 Sequencer read control 2                                       */
  __IOM uint32_t SEQ_READ_CTL_3;                /*!< 0x0000004C Sequencer read control 3                                       */
  __IOM uint32_t SEQ_READ_CTL_4;                /*!< 0x00000050 Sequencer read control 4                                       */
  __IOM uint32_t SEQ_READ_CTL_5;                /*!< 0x00000054 Sequencer read control 5                                       */
   __IM uint32_t RESERVED3[2];
  __IOM uint32_t SEQ_PROGRAM_CTL_0;             /*!< 0x00000060 Sequencer program control 0                                    */
  __IOM uint32_t SEQ_PROGRAM_CTL_1;             /*!< 0x00000064 Sequencer program control 1                                    */
  __IOM uint32_t SEQ_PROGRAM_CTL_2;             /*!< 0x00000068 Sequencer program control 2                                    */
  __IOM uint32_t SEQ_PROGRAM_CTL_3;             /*!< 0x0000006C Sequencer program control 3                                    */
  __IOM uint32_t SEQ_PROGRAM_CTL_4;             /*!< 0x00000070 Sequencer program control 4                                    */
  __IOM uint32_t SEQ_PROGRAM_CTL_5;             /*!< 0x00000074 Sequencer program control 5                                    */
} EFUSE_Type;                                   /*!< Size = 120 (0x78)                                                         */


/* =======================================================  EFUSE.CTL  ======================================================= */
#define EFUSE_CTL_ENABLED_Pos             (31UL)                /*!< EFUSE CTL: ENABLED (Bit 31)                               */
#define EFUSE_CTL_ENABLED_Msk             (0x80000000UL)        /*!< EFUSE CTL: ENABLED (Bitfield-Mask: 0x01)                  */
/* =======================================================  EFUSE.CMD  ======================================================= */
#define EFUSE_CMD_BIT_DATA_Pos            (0UL)                 /*!< EFUSE CMD: BIT_DATA (Bit 0)                               */
#define EFUSE_CMD_BIT_DATA_Msk            (0x1UL)               /*!< EFUSE CMD: BIT_DATA (Bitfield-Mask: 0x01)                 */
#define EFUSE_CMD_BIT_ADDR_Pos            (4UL)                 /*!< EFUSE CMD: BIT_ADDR (Bit 4)                               */
#define EFUSE_CMD_BIT_ADDR_Msk            (0x70UL)              /*!< EFUSE CMD: BIT_ADDR (Bitfield-Mask: 0x07)                 */
#define EFUSE_CMD_BYTE_ADDR_Pos           (8UL)                 /*!< EFUSE CMD: BYTE_ADDR (Bit 8)                              */
#define EFUSE_CMD_BYTE_ADDR_Msk           (0x1f00UL)            /*!< EFUSE CMD: BYTE_ADDR (Bitfield-Mask: 0x1f)                */
#define EFUSE_CMD_MACRO_ADDR_Pos          (16UL)                /*!< EFUSE CMD: MACRO_ADDR (Bit 16)                            */
#define EFUSE_CMD_MACRO_ADDR_Msk          (0xf0000UL)           /*!< EFUSE CMD: MACRO_ADDR (Bitfield-Mask: 0x0f)               */
#define EFUSE_CMD_START_Pos               (31UL)                /*!< EFUSE CMD: START (Bit 31)                                 */
#define EFUSE_CMD_START_Msk               (0x80000000UL)        /*!< EFUSE CMD: START (Bitfield-Mask: 0x01)                    */
/* ===================================================  EFUSE.SEQ_DEFAULT  =================================================== */
#define EFUSE_SEQ_DEFAULT_STROBE_A_Pos    (16UL)                /*!< EFUSE SEQ_DEFAULT: STROBE_A (Bit 16)                      */
#define EFUSE_SEQ_DEFAULT_STROBE_A_Msk    (0x10000UL)           /*!< EFUSE SEQ_DEFAULT: STROBE_A (Bitfield-Mask: 0x01)         */
#define EFUSE_SEQ_DEFAULT_STROBE_B_Pos    (17UL)                /*!< EFUSE SEQ_DEFAULT: STROBE_B (Bit 17)                      */
#define EFUSE_SEQ_DEFAULT_STROBE_B_Msk    (0x20000UL)           /*!< EFUSE SEQ_DEFAULT: STROBE_B (Bitfield-Mask: 0x01)         */
#define EFUSE_SEQ_DEFAULT_STROBE_C_Pos    (18UL)                /*!< EFUSE SEQ_DEFAULT: STROBE_C (Bit 18)                      */
#define EFUSE_SEQ_DEFAULT_STROBE_C_Msk    (0x40000UL)           /*!< EFUSE SEQ_DEFAULT: STROBE_C (Bitfield-Mask: 0x01)         */
#define EFUSE_SEQ_DEFAULT_STROBE_D_Pos    (19UL)                /*!< EFUSE SEQ_DEFAULT: STROBE_D (Bit 19)                      */
#define EFUSE_SEQ_DEFAULT_STROBE_D_Msk    (0x80000UL)           /*!< EFUSE SEQ_DEFAULT: STROBE_D (Bitfield-Mask: 0x01)         */
#define EFUSE_SEQ_DEFAULT_STROBE_E_Pos    (20UL)                /*!< EFUSE SEQ_DEFAULT: STROBE_E (Bit 20)                      */
#define EFUSE_SEQ_DEFAULT_STROBE_E_Msk    (0x100000UL)          /*!< EFUSE SEQ_DEFAULT: STROBE_E (Bitfield-Mask: 0x01)         */
#define EFUSE_SEQ_DEFAULT_STROBE_F_Pos    (21UL)                /*!< EFUSE SEQ_DEFAULT: STROBE_F (Bit 21)                      */
#define EFUSE_SEQ_DEFAULT_STROBE_F_Msk    (0x200000UL)          /*!< EFUSE SEQ_DEFAULT: STROBE_F (Bitfield-Mask: 0x01)         */
#define EFUSE_SEQ_DEFAULT_STROBE_G_Pos    (22UL)                /*!< EFUSE SEQ_DEFAULT: STROBE_G (Bit 22)                      */
#define EFUSE_SEQ_DEFAULT_STROBE_G_Msk    (0x400000UL)          /*!< EFUSE SEQ_DEFAULT: STROBE_G (Bitfield-Mask: 0x01)         */
/* =================================================  EFUSE.SEQ_READ_CTL_0  ================================================== */
#define EFUSE_SEQ_READ_CTL_0_CYCLES_Pos   (0UL)                 /*!< EFUSE SEQ_READ_CTL_0: CYCLES (Bit 0)                      */
#define EFUSE_SEQ_READ_CTL_0_CYCLES_Msk   (0x3ffUL)             /*!< EFUSE SEQ_READ_CTL_0: CYCLES (Bitfield-Mask: 0x3ff)       */
#define EFUSE_SEQ_READ_CTL_0_STROBE_A_Pos (16UL)                /*!< EFUSE SEQ_READ_CTL_0: STROBE_A (Bit 16)                   */
#define EFUSE_SEQ_READ_CTL_0_STROBE_A_Msk (0x10000UL)           /*!< EFUSE SEQ_READ_CTL_0: STROBE_A (Bitfield-Mask: 0x01)      */
#define EFUSE_SEQ_READ_CTL_0_STROBE_B_Pos (17UL)                /*!< EFUSE SEQ_READ_CTL_0: STROBE_B (Bit 17)                   */
#define EFUSE_SEQ_READ_CTL_0_STROBE_B_Msk (0x20000UL)           /*!< EFUSE SEQ_READ_CTL_0: STROBE_B (Bitfield-Mask: 0x01)      */
#define EFUSE_SEQ_READ_CTL_0_STROBE_C_Pos (18UL)                /*!< EFUSE SEQ_READ_CTL_0: STROBE_C (Bit 18)                   */
#define EFUSE_SEQ_READ_CTL_0_STROBE_C_Msk (0x40000UL)           /*!< EFUSE SEQ_READ_CTL_0: STROBE_C (Bitfield-Mask: 0x01)      */
#define EFUSE_SEQ_READ_CTL_0_STROBE_D_Pos (19UL)                /*!< EFUSE SEQ_READ_CTL_0: STROBE_D (Bit 19)                   */
#define EFUSE_SEQ_READ_CTL_0_STROBE_D_Msk (0x80000UL)           /*!< EFUSE SEQ_READ_CTL_0: STROBE_D (Bitfield-Mask: 0x01)      */
#define EFUSE_SEQ_READ_CTL_0_STROBE_E_Pos (20UL)                /*!< EFUSE SEQ_READ_CTL_0: STROBE_E (Bit 20)                   */
#define EFUSE_SEQ_READ_CTL_0_STROBE_E_Msk (0x100000UL)          /*!< EFUSE SEQ_READ_CTL_0: STROBE_E (Bitfield-Mask: 0x01)      */
#define EFUSE_SEQ_READ_CTL_0_STROBE_F_Pos (21UL)                /*!< EFUSE SEQ_READ_CTL_0: STROBE_F (Bit 21)                   */
#define EFUSE_SEQ_READ_CTL_0_STROBE_F_Msk (0x200000UL)          /*!< EFUSE SEQ_READ_CTL_0: STROBE_F (Bitfield-Mask: 0x01)      */
#define EFUSE_SEQ_READ_CTL_0_STROBE_G_Pos (22UL)                /*!< EFUSE SEQ_READ_CTL_0: STROBE_G (Bit 22)                   */
#define EFUSE_SEQ_READ_CTL_0_STROBE_G_Msk (0x400000UL)          /*!< EFUSE SEQ_READ_CTL_0: STROBE_G (Bitfield-Mask: 0x01)      */
#define EFUSE_SEQ_READ_CTL_0_DONE_Pos     (31UL)                /*!< EFUSE SEQ_READ_CTL_0: DONE (Bit 31)                       */
#define EFUSE_SEQ_READ_CTL_0_DONE_Msk     (0x80000000UL)        /*!< EFUSE SEQ_READ_CTL_0: DONE (Bitfield-Mask: 0x01)          */
/* =================================================  EFUSE.SEQ_READ_CTL_1  ================================================== */
#define EFUSE_SEQ_READ_CTL_1_CYCLES_Pos   (0UL)                 /*!< EFUSE SEQ_READ_CTL_1: CYCLES (Bit 0)                      */
#define EFUSE_SEQ_READ_CTL_1_CYCLES_Msk   (0x3ffUL)             /*!< EFUSE SEQ_READ_CTL_1: CYCLES (Bitfield-Mask: 0x3ff)       */
#define EFUSE_SEQ_READ_CTL_1_STROBE_A_Pos (16UL)                /*!< EFUSE SEQ_READ_CTL_1: STROBE_A (Bit 16)                   */
#define EFUSE_SEQ_READ_CTL_1_STROBE_A_Msk (0x10000UL)           /*!< EFUSE SEQ_READ_CTL_1: STROBE_A (Bitfield-Mask: 0x01)      */
#define EFUSE_SEQ_READ_CTL_1_STROBE_B_Pos (17UL)                /*!< EFUSE SEQ_READ_CTL_1: STROBE_B (Bit 17)                   */
#define EFUSE_SEQ_READ_CTL_1_STROBE_B_Msk (0x20000UL)           /*!< EFUSE SEQ_READ_CTL_1: STROBE_B (Bitfield-Mask: 0x01)      */
#define EFUSE_SEQ_READ_CTL_1_STROBE_C_Pos (18UL)                /*!< EFUSE SEQ_READ_CTL_1: STROBE_C (Bit 18)                   */
#define EFUSE_SEQ_READ_CTL_1_STROBE_C_Msk (0x40000UL)           /*!< EFUSE SEQ_READ_CTL_1: STROBE_C (Bitfield-Mask: 0x01)      */
#define EFUSE_SEQ_READ_CTL_1_STROBE_D_Pos (19UL)                /*!< EFUSE SEQ_READ_CTL_1: STROBE_D (Bit 19)                   */
#define EFUSE_SEQ_READ_CTL_1_STROBE_D_Msk (0x80000UL)           /*!< EFUSE SEQ_READ_CTL_1: STROBE_D (Bitfield-Mask: 0x01)      */
#define EFUSE_SEQ_READ_CTL_1_STROBE_E_Pos (20UL)                /*!< EFUSE SEQ_READ_CTL_1: STROBE_E (Bit 20)                   */
#define EFUSE_SEQ_READ_CTL_1_STROBE_E_Msk (0x100000UL)          /*!< EFUSE SEQ_READ_CTL_1: STROBE_E (Bitfield-Mask: 0x01)      */
#define EFUSE_SEQ_READ_CTL_1_STROBE_F_Pos (21UL)                /*!< EFUSE SEQ_READ_CTL_1: STROBE_F (Bit 21)                   */
#define EFUSE_SEQ_READ_CTL_1_STROBE_F_Msk (0x200000UL)          /*!< EFUSE SEQ_READ_CTL_1: STROBE_F (Bitfield-Mask: 0x01)      */
#define EFUSE_SEQ_READ_CTL_1_STROBE_G_Pos (22UL)                /*!< EFUSE SEQ_READ_CTL_1: STROBE_G (Bit 22)                   */
#define EFUSE_SEQ_READ_CTL_1_STROBE_G_Msk (0x400000UL)          /*!< EFUSE SEQ_READ_CTL_1: STROBE_G (Bitfield-Mask: 0x01)      */
#define EFUSE_SEQ_READ_CTL_1_DONE_Pos     (31UL)                /*!< EFUSE SEQ_READ_CTL_1: DONE (Bit 31)                       */
#define EFUSE_SEQ_READ_CTL_1_DONE_Msk     (0x80000000UL)        /*!< EFUSE SEQ_READ_CTL_1: DONE (Bitfield-Mask: 0x01)          */
/* =================================================  EFUSE.SEQ_READ_CTL_2  ================================================== */
#define EFUSE_SEQ_READ_CTL_2_CYCLES_Pos   (0UL)                 /*!< EFUSE SEQ_READ_CTL_2: CYCLES (Bit 0)                      */
#define EFUSE_SEQ_READ_CTL_2_CYCLES_Msk   (0x3ffUL)             /*!< EFUSE SEQ_READ_CTL_2: CYCLES (Bitfield-Mask: 0x3ff)       */
#define EFUSE_SEQ_READ_CTL_2_STROBE_A_Pos (16UL)                /*!< EFUSE SEQ_READ_CTL_2: STROBE_A (Bit 16)                   */
#define EFUSE_SEQ_READ_CTL_2_STROBE_A_Msk (0x10000UL)           /*!< EFUSE SEQ_READ_CTL_2: STROBE_A (Bitfield-Mask: 0x01)      */
#define EFUSE_SEQ_READ_CTL_2_STROBE_B_Pos (17UL)                /*!< EFUSE SEQ_READ_CTL_2: STROBE_B (Bit 17)                   */
#define EFUSE_SEQ_READ_CTL_2_STROBE_B_Msk (0x20000UL)           /*!< EFUSE SEQ_READ_CTL_2: STROBE_B (Bitfield-Mask: 0x01)      */
#define EFUSE_SEQ_READ_CTL_2_STROBE_C_Pos (18UL)                /*!< EFUSE SEQ_READ_CTL_2: STROBE_C (Bit 18)                   */
#define EFUSE_SEQ_READ_CTL_2_STROBE_C_Msk (0x40000UL)           /*!< EFUSE SEQ_READ_CTL_2: STROBE_C (Bitfield-Mask: 0x01)      */
#define EFUSE_SEQ_READ_CTL_2_STROBE_D_Pos (19UL)                /*!< EFUSE SEQ_READ_CTL_2: STROBE_D (Bit 19)                   */
#define EFUSE_SEQ_READ_CTL_2_STROBE_D_Msk (0x80000UL)           /*!< EFUSE SEQ_READ_CTL_2: STROBE_D (Bitfield-Mask: 0x01)      */
#define EFUSE_SEQ_READ_CTL_2_STROBE_E_Pos (20UL)                /*!< EFUSE SEQ_READ_CTL_2: STROBE_E (Bit 20)                   */
#define EFUSE_SEQ_READ_CTL_2_STROBE_E_Msk (0x100000UL)          /*!< EFUSE SEQ_READ_CTL_2: STROBE_E (Bitfield-Mask: 0x01)      */
#define EFUSE_SEQ_READ_CTL_2_STROBE_F_Pos (21UL)                /*!< EFUSE SEQ_READ_CTL_2: STROBE_F (Bit 21)                   */
#define EFUSE_SEQ_READ_CTL_2_STROBE_F_Msk (0x200000UL)          /*!< EFUSE SEQ_READ_CTL_2: STROBE_F (Bitfield-Mask: 0x01)      */
#define EFUSE_SEQ_READ_CTL_2_STROBE_G_Pos (22UL)                /*!< EFUSE SEQ_READ_CTL_2: STROBE_G (Bit 22)                   */
#define EFUSE_SEQ_READ_CTL_2_STROBE_G_Msk (0x400000UL)          /*!< EFUSE SEQ_READ_CTL_2: STROBE_G (Bitfield-Mask: 0x01)      */
#define EFUSE_SEQ_READ_CTL_2_DONE_Pos     (31UL)                /*!< EFUSE SEQ_READ_CTL_2: DONE (Bit 31)                       */
#define EFUSE_SEQ_READ_CTL_2_DONE_Msk     (0x80000000UL)        /*!< EFUSE SEQ_READ_CTL_2: DONE (Bitfield-Mask: 0x01)          */
/* =================================================  EFUSE.SEQ_READ_CTL_3  ================================================== */
#define EFUSE_SEQ_READ_CTL_3_CYCLES_Pos   (0UL)                 /*!< EFUSE SEQ_READ_CTL_3: CYCLES (Bit 0)                      */
#define EFUSE_SEQ_READ_CTL_3_CYCLES_Msk   (0x3ffUL)             /*!< EFUSE SEQ_READ_CTL_3: CYCLES (Bitfield-Mask: 0x3ff)       */
#define EFUSE_SEQ_READ_CTL_3_STROBE_A_Pos (16UL)                /*!< EFUSE SEQ_READ_CTL_3: STROBE_A (Bit 16)                   */
#define EFUSE_SEQ_READ_CTL_3_STROBE_A_Msk (0x10000UL)           /*!< EFUSE SEQ_READ_CTL_3: STROBE_A (Bitfield-Mask: 0x01)      */
#define EFUSE_SEQ_READ_CTL_3_STROBE_B_Pos (17UL)                /*!< EFUSE SEQ_READ_CTL_3: STROBE_B (Bit 17)                   */
#define EFUSE_SEQ_READ_CTL_3_STROBE_B_Msk (0x20000UL)           /*!< EFUSE SEQ_READ_CTL_3: STROBE_B (Bitfield-Mask: 0x01)      */
#define EFUSE_SEQ_READ_CTL_3_STROBE_C_Pos (18UL)                /*!< EFUSE SEQ_READ_CTL_3: STROBE_C (Bit 18)                   */
#define EFUSE_SEQ_READ_CTL_3_STROBE_C_Msk (0x40000UL)           /*!< EFUSE SEQ_READ_CTL_3: STROBE_C (Bitfield-Mask: 0x01)      */
#define EFUSE_SEQ_READ_CTL_3_STROBE_D_Pos (19UL)                /*!< EFUSE SEQ_READ_CTL_3: STROBE_D (Bit 19)                   */
#define EFUSE_SEQ_READ_CTL_3_STROBE_D_Msk (0x80000UL)           /*!< EFUSE SEQ_READ_CTL_3: STROBE_D (Bitfield-Mask: 0x01)      */
#define EFUSE_SEQ_READ_CTL_3_STROBE_E_Pos (20UL)                /*!< EFUSE SEQ_READ_CTL_3: STROBE_E (Bit 20)                   */
#define EFUSE_SEQ_READ_CTL_3_STROBE_E_Msk (0x100000UL)          /*!< EFUSE SEQ_READ_CTL_3: STROBE_E (Bitfield-Mask: 0x01)      */
#define EFUSE_SEQ_READ_CTL_3_STROBE_F_Pos (21UL)                /*!< EFUSE SEQ_READ_CTL_3: STROBE_F (Bit 21)                   */
#define EFUSE_SEQ_READ_CTL_3_STROBE_F_Msk (0x200000UL)          /*!< EFUSE SEQ_READ_CTL_3: STROBE_F (Bitfield-Mask: 0x01)      */
#define EFUSE_SEQ_READ_CTL_3_STROBE_G_Pos (22UL)                /*!< EFUSE SEQ_READ_CTL_3: STROBE_G (Bit 22)                   */
#define EFUSE_SEQ_READ_CTL_3_STROBE_G_Msk (0x400000UL)          /*!< EFUSE SEQ_READ_CTL_3: STROBE_G (Bitfield-Mask: 0x01)      */
#define EFUSE_SEQ_READ_CTL_3_DONE_Pos     (31UL)                /*!< EFUSE SEQ_READ_CTL_3: DONE (Bit 31)                       */
#define EFUSE_SEQ_READ_CTL_3_DONE_Msk     (0x80000000UL)        /*!< EFUSE SEQ_READ_CTL_3: DONE (Bitfield-Mask: 0x01)          */
/* =================================================  EFUSE.SEQ_READ_CTL_4  ================================================== */
#define EFUSE_SEQ_READ_CTL_4_CYCLES_Pos   (0UL)                 /*!< EFUSE SEQ_READ_CTL_4: CYCLES (Bit 0)                      */
#define EFUSE_SEQ_READ_CTL_4_CYCLES_Msk   (0x3ffUL)             /*!< EFUSE SEQ_READ_CTL_4: CYCLES (Bitfield-Mask: 0x3ff)       */
#define EFUSE_SEQ_READ_CTL_4_STROBE_A_Pos (16UL)                /*!< EFUSE SEQ_READ_CTL_4: STROBE_A (Bit 16)                   */
#define EFUSE_SEQ_READ_CTL_4_STROBE_A_Msk (0x10000UL)           /*!< EFUSE SEQ_READ_CTL_4: STROBE_A (Bitfield-Mask: 0x01)      */
#define EFUSE_SEQ_READ_CTL_4_STROBE_B_Pos (17UL)                /*!< EFUSE SEQ_READ_CTL_4: STROBE_B (Bit 17)                   */
#define EFUSE_SEQ_READ_CTL_4_STROBE_B_Msk (0x20000UL)           /*!< EFUSE SEQ_READ_CTL_4: STROBE_B (Bitfield-Mask: 0x01)      */
#define EFUSE_SEQ_READ_CTL_4_STROBE_C_Pos (18UL)                /*!< EFUSE SEQ_READ_CTL_4: STROBE_C (Bit 18)                   */
#define EFUSE_SEQ_READ_CTL_4_STROBE_C_Msk (0x40000UL)           /*!< EFUSE SEQ_READ_CTL_4: STROBE_C (Bitfield-Mask: 0x01)      */
#define EFUSE_SEQ_READ_CTL_4_STROBE_D_Pos (19UL)                /*!< EFUSE SEQ_READ_CTL_4: STROBE_D (Bit 19)                   */
#define EFUSE_SEQ_READ_CTL_4_STROBE_D_Msk (0x80000UL)           /*!< EFUSE SEQ_READ_CTL_4: STROBE_D (Bitfield-Mask: 0x01)      */
#define EFUSE_SEQ_READ_CTL_4_STROBE_E_Pos (20UL)                /*!< EFUSE SEQ_READ_CTL_4: STROBE_E (Bit 20)                   */
#define EFUSE_SEQ_READ_CTL_4_STROBE_E_Msk (0x100000UL)          /*!< EFUSE SEQ_READ_CTL_4: STROBE_E (Bitfield-Mask: 0x01)      */
#define EFUSE_SEQ_READ_CTL_4_STROBE_F_Pos (21UL)                /*!< EFUSE SEQ_READ_CTL_4: STROBE_F (Bit 21)                   */
#define EFUSE_SEQ_READ_CTL_4_STROBE_F_Msk (0x200000UL)          /*!< EFUSE SEQ_READ_CTL_4: STROBE_F (Bitfield-Mask: 0x01)      */
#define EFUSE_SEQ_READ_CTL_4_STROBE_G_Pos (22UL)                /*!< EFUSE SEQ_READ_CTL_4: STROBE_G (Bit 22)                   */
#define EFUSE_SEQ_READ_CTL_4_STROBE_G_Msk (0x400000UL)          /*!< EFUSE SEQ_READ_CTL_4: STROBE_G (Bitfield-Mask: 0x01)      */
#define EFUSE_SEQ_READ_CTL_4_DONE_Pos     (31UL)                /*!< EFUSE SEQ_READ_CTL_4: DONE (Bit 31)                       */
#define EFUSE_SEQ_READ_CTL_4_DONE_Msk     (0x80000000UL)        /*!< EFUSE SEQ_READ_CTL_4: DONE (Bitfield-Mask: 0x01)          */
/* =================================================  EFUSE.SEQ_READ_CTL_5  ================================================== */
#define EFUSE_SEQ_READ_CTL_5_CYCLES_Pos   (0UL)                 /*!< EFUSE SEQ_READ_CTL_5: CYCLES (Bit 0)                      */
#define EFUSE_SEQ_READ_CTL_5_CYCLES_Msk   (0x3ffUL)             /*!< EFUSE SEQ_READ_CTL_5: CYCLES (Bitfield-Mask: 0x3ff)       */
#define EFUSE_SEQ_READ_CTL_5_STROBE_A_Pos (16UL)                /*!< EFUSE SEQ_READ_CTL_5: STROBE_A (Bit 16)                   */
#define EFUSE_SEQ_READ_CTL_5_STROBE_A_Msk (0x10000UL)           /*!< EFUSE SEQ_READ_CTL_5: STROBE_A (Bitfield-Mask: 0x01)      */
#define EFUSE_SEQ_READ_CTL_5_STROBE_B_Pos (17UL)                /*!< EFUSE SEQ_READ_CTL_5: STROBE_B (Bit 17)                   */
#define EFUSE_SEQ_READ_CTL_5_STROBE_B_Msk (0x20000UL)           /*!< EFUSE SEQ_READ_CTL_5: STROBE_B (Bitfield-Mask: 0x01)      */
#define EFUSE_SEQ_READ_CTL_5_STROBE_C_Pos (18UL)                /*!< EFUSE SEQ_READ_CTL_5: STROBE_C (Bit 18)                   */
#define EFUSE_SEQ_READ_CTL_5_STROBE_C_Msk (0x40000UL)           /*!< EFUSE SEQ_READ_CTL_5: STROBE_C (Bitfield-Mask: 0x01)      */
#define EFUSE_SEQ_READ_CTL_5_STROBE_D_Pos (19UL)                /*!< EFUSE SEQ_READ_CTL_5: STROBE_D (Bit 19)                   */
#define EFUSE_SEQ_READ_CTL_5_STROBE_D_Msk (0x80000UL)           /*!< EFUSE SEQ_READ_CTL_5: STROBE_D (Bitfield-Mask: 0x01)      */
#define EFUSE_SEQ_READ_CTL_5_STROBE_E_Pos (20UL)                /*!< EFUSE SEQ_READ_CTL_5: STROBE_E (Bit 20)                   */
#define EFUSE_SEQ_READ_CTL_5_STROBE_E_Msk (0x100000UL)          /*!< EFUSE SEQ_READ_CTL_5: STROBE_E (Bitfield-Mask: 0x01)      */
#define EFUSE_SEQ_READ_CTL_5_STROBE_F_Pos (21UL)                /*!< EFUSE SEQ_READ_CTL_5: STROBE_F (Bit 21)                   */
#define EFUSE_SEQ_READ_CTL_5_STROBE_F_Msk (0x200000UL)          /*!< EFUSE SEQ_READ_CTL_5: STROBE_F (Bitfield-Mask: 0x01)      */
#define EFUSE_SEQ_READ_CTL_5_STROBE_G_Pos (22UL)                /*!< EFUSE SEQ_READ_CTL_5: STROBE_G (Bit 22)                   */
#define EFUSE_SEQ_READ_CTL_5_STROBE_G_Msk (0x400000UL)          /*!< EFUSE SEQ_READ_CTL_5: STROBE_G (Bitfield-Mask: 0x01)      */
#define EFUSE_SEQ_READ_CTL_5_DONE_Pos     (31UL)                /*!< EFUSE SEQ_READ_CTL_5: DONE (Bit 31)                       */
#define EFUSE_SEQ_READ_CTL_5_DONE_Msk     (0x80000000UL)        /*!< EFUSE SEQ_READ_CTL_5: DONE (Bitfield-Mask: 0x01)          */
/* ================================================  EFUSE.SEQ_PROGRAM_CTL_0  ================================================ */
#define EFUSE_SEQ_PROGRAM_CTL_0_CYCLES_Pos (0UL)                /*!< EFUSE SEQ_PROGRAM_CTL_0: CYCLES (Bit 0)                   */
#define EFUSE_SEQ_PROGRAM_CTL_0_CYCLES_Msk (0x3ffUL)            /*!< EFUSE SEQ_PROGRAM_CTL_0: CYCLES (Bitfield-Mask: 0x3ff)    */
#define EFUSE_SEQ_PROGRAM_CTL_0_STROBE_A_Pos (16UL)             /*!< EFUSE SEQ_PROGRAM_CTL_0: STROBE_A (Bit 16)                */
#define EFUSE_SEQ_PROGRAM_CTL_0_STROBE_A_Msk (0x10000UL)        /*!< EFUSE SEQ_PROGRAM_CTL_0: STROBE_A (Bitfield-Mask: 0x01)   */
#define EFUSE_SEQ_PROGRAM_CTL_0_STROBE_B_Pos (17UL)             /*!< EFUSE SEQ_PROGRAM_CTL_0: STROBE_B (Bit 17)                */
#define EFUSE_SEQ_PROGRAM_CTL_0_STROBE_B_Msk (0x20000UL)        /*!< EFUSE SEQ_PROGRAM_CTL_0: STROBE_B (Bitfield-Mask: 0x01)   */
#define EFUSE_SEQ_PROGRAM_CTL_0_STROBE_C_Pos (18UL)             /*!< EFUSE SEQ_PROGRAM_CTL_0: STROBE_C (Bit 18)                */
#define EFUSE_SEQ_PROGRAM_CTL_0_STROBE_C_Msk (0x40000UL)        /*!< EFUSE SEQ_PROGRAM_CTL_0: STROBE_C (Bitfield-Mask: 0x01)   */
#define EFUSE_SEQ_PROGRAM_CTL_0_STROBE_D_Pos (19UL)             /*!< EFUSE SEQ_PROGRAM_CTL_0: STROBE_D (Bit 19)                */
#define EFUSE_SEQ_PROGRAM_CTL_0_STROBE_D_Msk (0x80000UL)        /*!< EFUSE SEQ_PROGRAM_CTL_0: STROBE_D (Bitfield-Mask: 0x01)   */
#define EFUSE_SEQ_PROGRAM_CTL_0_STROBE_E_Pos (20UL)             /*!< EFUSE SEQ_PROGRAM_CTL_0: STROBE_E (Bit 20)                */
#define EFUSE_SEQ_PROGRAM_CTL_0_STROBE_E_Msk (0x100000UL)       /*!< EFUSE SEQ_PROGRAM_CTL_0: STROBE_E (Bitfield-Mask: 0x01)   */
#define EFUSE_SEQ_PROGRAM_CTL_0_STROBE_F_Pos (21UL)             /*!< EFUSE SEQ_PROGRAM_CTL_0: STROBE_F (Bit 21)                */
#define EFUSE_SEQ_PROGRAM_CTL_0_STROBE_F_Msk (0x200000UL)       /*!< EFUSE SEQ_PROGRAM_CTL_0: STROBE_F (Bitfield-Mask: 0x01)   */
#define EFUSE_SEQ_PROGRAM_CTL_0_STROBE_G_Pos (22UL)             /*!< EFUSE SEQ_PROGRAM_CTL_0: STROBE_G (Bit 22)                */
#define EFUSE_SEQ_PROGRAM_CTL_0_STROBE_G_Msk (0x400000UL)       /*!< EFUSE SEQ_PROGRAM_CTL_0: STROBE_G (Bitfield-Mask: 0x01)   */
#define EFUSE_SEQ_PROGRAM_CTL_0_DONE_Pos  (31UL)                /*!< EFUSE SEQ_PROGRAM_CTL_0: DONE (Bit 31)                    */
#define EFUSE_SEQ_PROGRAM_CTL_0_DONE_Msk  (0x80000000UL)        /*!< EFUSE SEQ_PROGRAM_CTL_0: DONE (Bitfield-Mask: 0x01)       */
/* ================================================  EFUSE.SEQ_PROGRAM_CTL_1  ================================================ */
#define EFUSE_SEQ_PROGRAM_CTL_1_CYCLES_Pos (0UL)                /*!< EFUSE SEQ_PROGRAM_CTL_1: CYCLES (Bit 0)                   */
#define EFUSE_SEQ_PROGRAM_CTL_1_CYCLES_Msk (0x3ffUL)            /*!< EFUSE SEQ_PROGRAM_CTL_1: CYCLES (Bitfield-Mask: 0x3ff)    */
#define EFUSE_SEQ_PROGRAM_CTL_1_STROBE_A_Pos (16UL)             /*!< EFUSE SEQ_PROGRAM_CTL_1: STROBE_A (Bit 16)                */
#define EFUSE_SEQ_PROGRAM_CTL_1_STROBE_A_Msk (0x10000UL)        /*!< EFUSE SEQ_PROGRAM_CTL_1: STROBE_A (Bitfield-Mask: 0x01)   */
#define EFUSE_SEQ_PROGRAM_CTL_1_STROBE_B_Pos (17UL)             /*!< EFUSE SEQ_PROGRAM_CTL_1: STROBE_B (Bit 17)                */
#define EFUSE_SEQ_PROGRAM_CTL_1_STROBE_B_Msk (0x20000UL)        /*!< EFUSE SEQ_PROGRAM_CTL_1: STROBE_B (Bitfield-Mask: 0x01)   */
#define EFUSE_SEQ_PROGRAM_CTL_1_STROBE_C_Pos (18UL)             /*!< EFUSE SEQ_PROGRAM_CTL_1: STROBE_C (Bit 18)                */
#define EFUSE_SEQ_PROGRAM_CTL_1_STROBE_C_Msk (0x40000UL)        /*!< EFUSE SEQ_PROGRAM_CTL_1: STROBE_C (Bitfield-Mask: 0x01)   */
#define EFUSE_SEQ_PROGRAM_CTL_1_STROBE_D_Pos (19UL)             /*!< EFUSE SEQ_PROGRAM_CTL_1: STROBE_D (Bit 19)                */
#define EFUSE_SEQ_PROGRAM_CTL_1_STROBE_D_Msk (0x80000UL)        /*!< EFUSE SEQ_PROGRAM_CTL_1: STROBE_D (Bitfield-Mask: 0x01)   */
#define EFUSE_SEQ_PROGRAM_CTL_1_STROBE_E_Pos (20UL)             /*!< EFUSE SEQ_PROGRAM_CTL_1: STROBE_E (Bit 20)                */
#define EFUSE_SEQ_PROGRAM_CTL_1_STROBE_E_Msk (0x100000UL)       /*!< EFUSE SEQ_PROGRAM_CTL_1: STROBE_E (Bitfield-Mask: 0x01)   */
#define EFUSE_SEQ_PROGRAM_CTL_1_STROBE_F_Pos (21UL)             /*!< EFUSE SEQ_PROGRAM_CTL_1: STROBE_F (Bit 21)                */
#define EFUSE_SEQ_PROGRAM_CTL_1_STROBE_F_Msk (0x200000UL)       /*!< EFUSE SEQ_PROGRAM_CTL_1: STROBE_F (Bitfield-Mask: 0x01)   */
#define EFUSE_SEQ_PROGRAM_CTL_1_STROBE_G_Pos (22UL)             /*!< EFUSE SEQ_PROGRAM_CTL_1: STROBE_G (Bit 22)                */
#define EFUSE_SEQ_PROGRAM_CTL_1_STROBE_G_Msk (0x400000UL)       /*!< EFUSE SEQ_PROGRAM_CTL_1: STROBE_G (Bitfield-Mask: 0x01)   */
#define EFUSE_SEQ_PROGRAM_CTL_1_DONE_Pos  (31UL)                /*!< EFUSE SEQ_PROGRAM_CTL_1: DONE (Bit 31)                    */
#define EFUSE_SEQ_PROGRAM_CTL_1_DONE_Msk  (0x80000000UL)        /*!< EFUSE SEQ_PROGRAM_CTL_1: DONE (Bitfield-Mask: 0x01)       */
/* ================================================  EFUSE.SEQ_PROGRAM_CTL_2  ================================================ */
#define EFUSE_SEQ_PROGRAM_CTL_2_CYCLES_Pos (0UL)                /*!< EFUSE SEQ_PROGRAM_CTL_2: CYCLES (Bit 0)                   */
#define EFUSE_SEQ_PROGRAM_CTL_2_CYCLES_Msk (0x3ffUL)            /*!< EFUSE SEQ_PROGRAM_CTL_2: CYCLES (Bitfield-Mask: 0x3ff)    */
#define EFUSE_SEQ_PROGRAM_CTL_2_STROBE_A_Pos (16UL)             /*!< EFUSE SEQ_PROGRAM_CTL_2: STROBE_A (Bit 16)                */
#define EFUSE_SEQ_PROGRAM_CTL_2_STROBE_A_Msk (0x10000UL)        /*!< EFUSE SEQ_PROGRAM_CTL_2: STROBE_A (Bitfield-Mask: 0x01)   */
#define EFUSE_SEQ_PROGRAM_CTL_2_STROBE_B_Pos (17UL)             /*!< EFUSE SEQ_PROGRAM_CTL_2: STROBE_B (Bit 17)                */
#define EFUSE_SEQ_PROGRAM_CTL_2_STROBE_B_Msk (0x20000UL)        /*!< EFUSE SEQ_PROGRAM_CTL_2: STROBE_B (Bitfield-Mask: 0x01)   */
#define EFUSE_SEQ_PROGRAM_CTL_2_STROBE_C_Pos (18UL)             /*!< EFUSE SEQ_PROGRAM_CTL_2: STROBE_C (Bit 18)                */
#define EFUSE_SEQ_PROGRAM_CTL_2_STROBE_C_Msk (0x40000UL)        /*!< EFUSE SEQ_PROGRAM_CTL_2: STROBE_C (Bitfield-Mask: 0x01)   */
#define EFUSE_SEQ_PROGRAM_CTL_2_STROBE_D_Pos (19UL)             /*!< EFUSE SEQ_PROGRAM_CTL_2: STROBE_D (Bit 19)                */
#define EFUSE_SEQ_PROGRAM_CTL_2_STROBE_D_Msk (0x80000UL)        /*!< EFUSE SEQ_PROGRAM_CTL_2: STROBE_D (Bitfield-Mask: 0x01)   */
#define EFUSE_SEQ_PROGRAM_CTL_2_STROBE_E_Pos (20UL)             /*!< EFUSE SEQ_PROGRAM_CTL_2: STROBE_E (Bit 20)                */
#define EFUSE_SEQ_PROGRAM_CTL_2_STROBE_E_Msk (0x100000UL)       /*!< EFUSE SEQ_PROGRAM_CTL_2: STROBE_E (Bitfield-Mask: 0x01)   */
#define EFUSE_SEQ_PROGRAM_CTL_2_STROBE_F_Pos (21UL)             /*!< EFUSE SEQ_PROGRAM_CTL_2: STROBE_F (Bit 21)                */
#define EFUSE_SEQ_PROGRAM_CTL_2_STROBE_F_Msk (0x200000UL)       /*!< EFUSE SEQ_PROGRAM_CTL_2: STROBE_F (Bitfield-Mask: 0x01)   */
#define EFUSE_SEQ_PROGRAM_CTL_2_STROBE_G_Pos (22UL)             /*!< EFUSE SEQ_PROGRAM_CTL_2: STROBE_G (Bit 22)                */
#define EFUSE_SEQ_PROGRAM_CTL_2_STROBE_G_Msk (0x400000UL)       /*!< EFUSE SEQ_PROGRAM_CTL_2: STROBE_G (Bitfield-Mask: 0x01)   */
#define EFUSE_SEQ_PROGRAM_CTL_2_DONE_Pos  (31UL)                /*!< EFUSE SEQ_PROGRAM_CTL_2: DONE (Bit 31)                    */
#define EFUSE_SEQ_PROGRAM_CTL_2_DONE_Msk  (0x80000000UL)        /*!< EFUSE SEQ_PROGRAM_CTL_2: DONE (Bitfield-Mask: 0x01)       */
/* ================================================  EFUSE.SEQ_PROGRAM_CTL_3  ================================================ */
#define EFUSE_SEQ_PROGRAM_CTL_3_CYCLES_Pos (0UL)                /*!< EFUSE SEQ_PROGRAM_CTL_3: CYCLES (Bit 0)                   */
#define EFUSE_SEQ_PROGRAM_CTL_3_CYCLES_Msk (0x3ffUL)            /*!< EFUSE SEQ_PROGRAM_CTL_3: CYCLES (Bitfield-Mask: 0x3ff)    */
#define EFUSE_SEQ_PROGRAM_CTL_3_STROBE_A_Pos (16UL)             /*!< EFUSE SEQ_PROGRAM_CTL_3: STROBE_A (Bit 16)                */
#define EFUSE_SEQ_PROGRAM_CTL_3_STROBE_A_Msk (0x10000UL)        /*!< EFUSE SEQ_PROGRAM_CTL_3: STROBE_A (Bitfield-Mask: 0x01)   */
#define EFUSE_SEQ_PROGRAM_CTL_3_STROBE_B_Pos (17UL)             /*!< EFUSE SEQ_PROGRAM_CTL_3: STROBE_B (Bit 17)                */
#define EFUSE_SEQ_PROGRAM_CTL_3_STROBE_B_Msk (0x20000UL)        /*!< EFUSE SEQ_PROGRAM_CTL_3: STROBE_B (Bitfield-Mask: 0x01)   */
#define EFUSE_SEQ_PROGRAM_CTL_3_STROBE_C_Pos (18UL)             /*!< EFUSE SEQ_PROGRAM_CTL_3: STROBE_C (Bit 18)                */
#define EFUSE_SEQ_PROGRAM_CTL_3_STROBE_C_Msk (0x40000UL)        /*!< EFUSE SEQ_PROGRAM_CTL_3: STROBE_C (Bitfield-Mask: 0x01)   */
#define EFUSE_SEQ_PROGRAM_CTL_3_STROBE_D_Pos (19UL)             /*!< EFUSE SEQ_PROGRAM_CTL_3: STROBE_D (Bit 19)                */
#define EFUSE_SEQ_PROGRAM_CTL_3_STROBE_D_Msk (0x80000UL)        /*!< EFUSE SEQ_PROGRAM_CTL_3: STROBE_D (Bitfield-Mask: 0x01)   */
#define EFUSE_SEQ_PROGRAM_CTL_3_STROBE_E_Pos (20UL)             /*!< EFUSE SEQ_PROGRAM_CTL_3: STROBE_E (Bit 20)                */
#define EFUSE_SEQ_PROGRAM_CTL_3_STROBE_E_Msk (0x100000UL)       /*!< EFUSE SEQ_PROGRAM_CTL_3: STROBE_E (Bitfield-Mask: 0x01)   */
#define EFUSE_SEQ_PROGRAM_CTL_3_STROBE_F_Pos (21UL)             /*!< EFUSE SEQ_PROGRAM_CTL_3: STROBE_F (Bit 21)                */
#define EFUSE_SEQ_PROGRAM_CTL_3_STROBE_F_Msk (0x200000UL)       /*!< EFUSE SEQ_PROGRAM_CTL_3: STROBE_F (Bitfield-Mask: 0x01)   */
#define EFUSE_SEQ_PROGRAM_CTL_3_STROBE_G_Pos (22UL)             /*!< EFUSE SEQ_PROGRAM_CTL_3: STROBE_G (Bit 22)                */
#define EFUSE_SEQ_PROGRAM_CTL_3_STROBE_G_Msk (0x400000UL)       /*!< EFUSE SEQ_PROGRAM_CTL_3: STROBE_G (Bitfield-Mask: 0x01)   */
#define EFUSE_SEQ_PROGRAM_CTL_3_DONE_Pos  (31UL)                /*!< EFUSE SEQ_PROGRAM_CTL_3: DONE (Bit 31)                    */
#define EFUSE_SEQ_PROGRAM_CTL_3_DONE_Msk  (0x80000000UL)        /*!< EFUSE SEQ_PROGRAM_CTL_3: DONE (Bitfield-Mask: 0x01)       */
/* ================================================  EFUSE.SEQ_PROGRAM_CTL_4  ================================================ */
#define EFUSE_SEQ_PROGRAM_CTL_4_CYCLES_Pos (0UL)                /*!< EFUSE SEQ_PROGRAM_CTL_4: CYCLES (Bit 0)                   */
#define EFUSE_SEQ_PROGRAM_CTL_4_CYCLES_Msk (0x3ffUL)            /*!< EFUSE SEQ_PROGRAM_CTL_4: CYCLES (Bitfield-Mask: 0x3ff)    */
#define EFUSE_SEQ_PROGRAM_CTL_4_STROBE_A_Pos (16UL)             /*!< EFUSE SEQ_PROGRAM_CTL_4: STROBE_A (Bit 16)                */
#define EFUSE_SEQ_PROGRAM_CTL_4_STROBE_A_Msk (0x10000UL)        /*!< EFUSE SEQ_PROGRAM_CTL_4: STROBE_A (Bitfield-Mask: 0x01)   */
#define EFUSE_SEQ_PROGRAM_CTL_4_STROBE_B_Pos (17UL)             /*!< EFUSE SEQ_PROGRAM_CTL_4: STROBE_B (Bit 17)                */
#define EFUSE_SEQ_PROGRAM_CTL_4_STROBE_B_Msk (0x20000UL)        /*!< EFUSE SEQ_PROGRAM_CTL_4: STROBE_B (Bitfield-Mask: 0x01)   */
#define EFUSE_SEQ_PROGRAM_CTL_4_STROBE_C_Pos (18UL)             /*!< EFUSE SEQ_PROGRAM_CTL_4: STROBE_C (Bit 18)                */
#define EFUSE_SEQ_PROGRAM_CTL_4_STROBE_C_Msk (0x40000UL)        /*!< EFUSE SEQ_PROGRAM_CTL_4: STROBE_C (Bitfield-Mask: 0x01)   */
#define EFUSE_SEQ_PROGRAM_CTL_4_STROBE_D_Pos (19UL)             /*!< EFUSE SEQ_PROGRAM_CTL_4: STROBE_D (Bit 19)                */
#define EFUSE_SEQ_PROGRAM_CTL_4_STROBE_D_Msk (0x80000UL)        /*!< EFUSE SEQ_PROGRAM_CTL_4: STROBE_D (Bitfield-Mask: 0x01)   */
#define EFUSE_SEQ_PROGRAM_CTL_4_STROBE_E_Pos (20UL)             /*!< EFUSE SEQ_PROGRAM_CTL_4: STROBE_E (Bit 20)                */
#define EFUSE_SEQ_PROGRAM_CTL_4_STROBE_E_Msk (0x100000UL)       /*!< EFUSE SEQ_PROGRAM_CTL_4: STROBE_E (Bitfield-Mask: 0x01)   */
#define EFUSE_SEQ_PROGRAM_CTL_4_STROBE_F_Pos (21UL)             /*!< EFUSE SEQ_PROGRAM_CTL_4: STROBE_F (Bit 21)                */
#define EFUSE_SEQ_PROGRAM_CTL_4_STROBE_F_Msk (0x200000UL)       /*!< EFUSE SEQ_PROGRAM_CTL_4: STROBE_F (Bitfield-Mask: 0x01)   */
#define EFUSE_SEQ_PROGRAM_CTL_4_STROBE_G_Pos (22UL)             /*!< EFUSE SEQ_PROGRAM_CTL_4: STROBE_G (Bit 22)                */
#define EFUSE_SEQ_PROGRAM_CTL_4_STROBE_G_Msk (0x400000UL)       /*!< EFUSE SEQ_PROGRAM_CTL_4: STROBE_G (Bitfield-Mask: 0x01)   */
#define EFUSE_SEQ_PROGRAM_CTL_4_DONE_Pos  (31UL)                /*!< EFUSE SEQ_PROGRAM_CTL_4: DONE (Bit 31)                    */
#define EFUSE_SEQ_PROGRAM_CTL_4_DONE_Msk  (0x80000000UL)        /*!< EFUSE SEQ_PROGRAM_CTL_4: DONE (Bitfield-Mask: 0x01)       */
/* ================================================  EFUSE.SEQ_PROGRAM_CTL_5  ================================================ */
#define EFUSE_SEQ_PROGRAM_CTL_5_CYCLES_Pos (0UL)                /*!< EFUSE SEQ_PROGRAM_CTL_5: CYCLES (Bit 0)                   */
#define EFUSE_SEQ_PROGRAM_CTL_5_CYCLES_Msk (0x3ffUL)            /*!< EFUSE SEQ_PROGRAM_CTL_5: CYCLES (Bitfield-Mask: 0x3ff)    */
#define EFUSE_SEQ_PROGRAM_CTL_5_STROBE_A_Pos (16UL)             /*!< EFUSE SEQ_PROGRAM_CTL_5: STROBE_A (Bit 16)                */
#define EFUSE_SEQ_PROGRAM_CTL_5_STROBE_A_Msk (0x10000UL)        /*!< EFUSE SEQ_PROGRAM_CTL_5: STROBE_A (Bitfield-Mask: 0x01)   */
#define EFUSE_SEQ_PROGRAM_CTL_5_STROBE_B_Pos (17UL)             /*!< EFUSE SEQ_PROGRAM_CTL_5: STROBE_B (Bit 17)                */
#define EFUSE_SEQ_PROGRAM_CTL_5_STROBE_B_Msk (0x20000UL)        /*!< EFUSE SEQ_PROGRAM_CTL_5: STROBE_B (Bitfield-Mask: 0x01)   */
#define EFUSE_SEQ_PROGRAM_CTL_5_STROBE_C_Pos (18UL)             /*!< EFUSE SEQ_PROGRAM_CTL_5: STROBE_C (Bit 18)                */
#define EFUSE_SEQ_PROGRAM_CTL_5_STROBE_C_Msk (0x40000UL)        /*!< EFUSE SEQ_PROGRAM_CTL_5: STROBE_C (Bitfield-Mask: 0x01)   */
#define EFUSE_SEQ_PROGRAM_CTL_5_STROBE_D_Pos (19UL)             /*!< EFUSE SEQ_PROGRAM_CTL_5: STROBE_D (Bit 19)                */
#define EFUSE_SEQ_PROGRAM_CTL_5_STROBE_D_Msk (0x80000UL)        /*!< EFUSE SEQ_PROGRAM_CTL_5: STROBE_D (Bitfield-Mask: 0x01)   */
#define EFUSE_SEQ_PROGRAM_CTL_5_STROBE_E_Pos (20UL)             /*!< EFUSE SEQ_PROGRAM_CTL_5: STROBE_E (Bit 20)                */
#define EFUSE_SEQ_PROGRAM_CTL_5_STROBE_E_Msk (0x100000UL)       /*!< EFUSE SEQ_PROGRAM_CTL_5: STROBE_E (Bitfield-Mask: 0x01)   */
#define EFUSE_SEQ_PROGRAM_CTL_5_STROBE_F_Pos (21UL)             /*!< EFUSE SEQ_PROGRAM_CTL_5: STROBE_F (Bit 21)                */
#define EFUSE_SEQ_PROGRAM_CTL_5_STROBE_F_Msk (0x200000UL)       /*!< EFUSE SEQ_PROGRAM_CTL_5: STROBE_F (Bitfield-Mask: 0x01)   */
#define EFUSE_SEQ_PROGRAM_CTL_5_STROBE_G_Pos (22UL)             /*!< EFUSE SEQ_PROGRAM_CTL_5: STROBE_G (Bit 22)                */
#define EFUSE_SEQ_PROGRAM_CTL_5_STROBE_G_Msk (0x400000UL)       /*!< EFUSE SEQ_PROGRAM_CTL_5: STROBE_G (Bitfield-Mask: 0x01)   */
#define EFUSE_SEQ_PROGRAM_CTL_5_DONE_Pos  (31UL)                /*!< EFUSE SEQ_PROGRAM_CTL_5: DONE (Bit 31)                    */
#define EFUSE_SEQ_PROGRAM_CTL_5_DONE_Msk  (0x80000000UL)        /*!< EFUSE SEQ_PROGRAM_CTL_5: DONE (Bitfield-Mask: 0x01)       */


#endif /* _CYIP_EFUSE_H_ */


/* [] END OF FILE */
