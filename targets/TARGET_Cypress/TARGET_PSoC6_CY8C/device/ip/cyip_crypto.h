/***************************************************************************//**
* \file cyip_crypto.h
*
* \brief
* CRYPTO IP definitions
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

#ifndef _CYIP_CRYPTO_H_
#define _CYIP_CRYPTO_H_

#include "cyip_headers.h"

/* =========================================================================================================================== */
/* ================                                          CRYPTO                                           ================ */
/* =========================================================================================================================== */

#define CRYPTO_SECTION_SIZE         0x00010000UL

/**
  * \brief Cryptography component (CRYPTO)
  */
typedef struct {                                /*!< CRYPTO Structure                                                          */
  __IOM uint32_t CTL;                           /*!< 0x00000000 Control                                                        */
   __IM uint32_t STATUS;                        /*!< 0x00000004 Status                                                         */
   __IM uint32_t RESERVED[6];
   __IM uint32_t ERROR_STATUS0;                 /*!< 0x00000020 Error status 0                                                 */
  __IOM uint32_t ERROR_STATUS1;                 /*!< 0x00000024 Error status 1                                                 */
   __IM uint32_t RESERVED1[6];
  __IOM uint32_t INSTR_FF_CTL;                  /*!< 0x00000040 Instruction FIFO control                                       */
   __IM uint32_t INSTR_FF_STATUS;               /*!< 0x00000044 Instruction FIFO status                                        */
   __OM uint32_t INSTR_FF_WR;                   /*!< 0x00000048 Instruction FIFO write                                         */
   __IM uint32_t RESERVED2[13];
   __IM uint32_t RF_DATA[16];                   /*!< 0x00000080 Register-file                                                  */
   __IM uint32_t RESERVED3[16];
  __IOM uint32_t AES_CTL;                       /*!< 0x00000100 AES control                                                    */
   __IM uint32_t RESERVED4[31];
   __IM uint32_t STR_RESULT;                    /*!< 0x00000180 String result                                                  */
   __IM uint32_t RESERVED5[31];
  __IOM uint32_t PR_LFSR_CTL0;                  /*!< 0x00000200 Pseudo random LFSR control 0                                   */
  __IOM uint32_t PR_LFSR_CTL1;                  /*!< 0x00000204 Pseudo random LFSR control 1                                   */
  __IOM uint32_t PR_LFSR_CTL2;                  /*!< 0x00000208 Pseudo random LFSR control 2                                   */
   __IM uint32_t RESERVED6;
  __IOM uint32_t PR_RESULT;                     /*!< 0x00000210 Pseudo random result                                           */
   __IM uint32_t RESERVED7[27];
  __IOM uint32_t TR_CTL0;                       /*!< 0x00000280 True random control 0                                          */
  __IOM uint32_t TR_CTL1;                       /*!< 0x00000284 True random control 1                                          */
  __IOM uint32_t TR_RESULT;                     /*!< 0x00000288 True random result                                             */
   __IM uint32_t RESERVED8[5];
  __IOM uint32_t TR_GARO_CTL;                   /*!< 0x000002A0 True random GARO control                                       */
  __IOM uint32_t TR_FIRO_CTL;                   /*!< 0x000002A4 True random FIRO control                                       */
   __IM uint32_t RESERVED9[6];
  __IOM uint32_t TR_MON_CTL;                    /*!< 0x000002C0 True random monitor control                                    */
   __IM uint32_t RESERVED10;
  __IOM uint32_t TR_MON_CMD;                    /*!< 0x000002C8 True random monitor command                                    */
   __IM uint32_t RESERVED11;
  __IOM uint32_t TR_MON_RC_CTL;                 /*!< 0x000002D0 True random monitor RC control                                 */
   __IM uint32_t RESERVED12;
   __IM uint32_t TR_MON_RC_STATUS0;             /*!< 0x000002D8 True random monitor RC status 0                                */
   __IM uint32_t TR_MON_RC_STATUS1;             /*!< 0x000002DC True random monitor RC status 1                                */
  __IOM uint32_t TR_MON_AP_CTL;                 /*!< 0x000002E0 True random monitor AP control                                 */
   __IM uint32_t RESERVED13;
   __IM uint32_t TR_MON_AP_STATUS0;             /*!< 0x000002E8 True random monitor AP status 0                                */
   __IM uint32_t TR_MON_AP_STATUS1;             /*!< 0x000002EC True random monitor AP status 1                                */
   __IM uint32_t RESERVED14[4];
  __IOM uint32_t SHA_CTL;                       /*!< 0x00000300 SHA control                                                    */
   __IM uint32_t RESERVED15[63];
  __IOM uint32_t CRC_CTL;                       /*!< 0x00000400 CRC control                                                    */
   __IM uint32_t RESERVED16[3];
  __IOM uint32_t CRC_DATA_CTL;                  /*!< 0x00000410 CRC data control                                               */
   __IM uint32_t RESERVED17[3];
  __IOM uint32_t CRC_POL_CTL;                   /*!< 0x00000420 CRC polynomial control                                         */
   __IM uint32_t RESERVED18[3];
  __IOM uint32_t CRC_LFSR_CTL;                  /*!< 0x00000430 CRC LFSR control                                               */
   __IM uint32_t RESERVED19[3];
  __IOM uint32_t CRC_REM_CTL;                   /*!< 0x00000440 CRC remainder control                                          */
   __IM uint32_t RESERVED20;
   __IM uint32_t CRC_REM_RESULT;                /*!< 0x00000448 CRC remainder result                                           */
   __IM uint32_t RESERVED21[13];
  __IOM uint32_t VU_CTL0;                       /*!< 0x00000480 Vector unit control 0                                          */
  __IOM uint32_t VU_CTL1;                       /*!< 0x00000484 Vector unit control 1                                          */
   __IM uint32_t RESERVED22[2];
   __IM uint32_t VU_STATUS;                     /*!< 0x00000490 Vector unit status                                             */
   __IM uint32_t RESERVED23[203];
  __IOM uint32_t INTR;                          /*!< 0x000007C0 Interrupt register                                             */
  __IOM uint32_t INTR_SET;                      /*!< 0x000007C4 Interrupt set register                                         */
  __IOM uint32_t INTR_MASK;                     /*!< 0x000007C8 Interrupt mask register                                        */
   __IM uint32_t INTR_MASKED;                   /*!< 0x000007CC Interrupt masked register                                      */
   __IM uint32_t RESERVED24[3596];
  __IOM uint32_t MEM_BUFF[4096];                /*!< 0x00004000 Memory buffer                                                  */
} CRYPTO_Type;                                  /*!< Size = 32768 (0x8000)                                                     */


/* ======================================================  CRYPTO.CTL  ======================================================= */
#define CRYPTO_CTL_ISOLATE_Pos            (0UL)                 /*!< CRYPTO CTL: ISOLATE (Bit 0)                               */
#define CRYPTO_CTL_ISOLATE_Msk            (0x1UL)               /*!< CRYPTO CTL: ISOLATE (Bitfield-Mask: 0x01)                 */
#define CRYPTO_CTL_RETAIN_Pos             (1UL)                 /*!< CRYPTO CTL: RETAIN (Bit 1)                                */
#define CRYPTO_CTL_RETAIN_Msk             (0x2UL)               /*!< CRYPTO CTL: RETAIN (Bitfield-Mask: 0x01)                  */
#define CRYPTO_CTL_POWER_Pos              (2UL)                 /*!< CRYPTO CTL: POWER (Bit 2)                                 */
#define CRYPTO_CTL_POWER_Msk              (0x4UL)               /*!< CRYPTO CTL: POWER (Bitfield-Mask: 0x01)                   */
#define CRYPTO_CTL_ENABLED_Pos            (31UL)                /*!< CRYPTO CTL: ENABLED (Bit 31)                              */
#define CRYPTO_CTL_ENABLED_Msk            (0x80000000UL)        /*!< CRYPTO CTL: ENABLED (Bitfield-Mask: 0x01)                 */
/* =====================================================  CRYPTO.STATUS  ===================================================== */
#define CRYPTO_STATUS_AES_BUSY_Pos        (0UL)                 /*!< CRYPTO STATUS: AES_BUSY (Bit 0)                           */
#define CRYPTO_STATUS_AES_BUSY_Msk        (0x1UL)               /*!< CRYPTO STATUS: AES_BUSY (Bitfield-Mask: 0x01)             */
#define CRYPTO_STATUS_DES_BUSY_Pos        (1UL)                 /*!< CRYPTO STATUS: DES_BUSY (Bit 1)                           */
#define CRYPTO_STATUS_DES_BUSY_Msk        (0x2UL)               /*!< CRYPTO STATUS: DES_BUSY (Bitfield-Mask: 0x01)             */
#define CRYPTO_STATUS_SHA_BUSY_Pos        (2UL)                 /*!< CRYPTO STATUS: SHA_BUSY (Bit 2)                           */
#define CRYPTO_STATUS_SHA_BUSY_Msk        (0x4UL)               /*!< CRYPTO STATUS: SHA_BUSY (Bitfield-Mask: 0x01)             */
#define CRYPTO_STATUS_CRC_BUSY_Pos        (3UL)                 /*!< CRYPTO STATUS: CRC_BUSY (Bit 3)                           */
#define CRYPTO_STATUS_CRC_BUSY_Msk        (0x8UL)               /*!< CRYPTO STATUS: CRC_BUSY (Bitfield-Mask: 0x01)             */
#define CRYPTO_STATUS_STR_BUSY_Pos        (4UL)                 /*!< CRYPTO STATUS: STR_BUSY (Bit 4)                           */
#define CRYPTO_STATUS_STR_BUSY_Msk        (0x10UL)              /*!< CRYPTO STATUS: STR_BUSY (Bitfield-Mask: 0x01)             */
#define CRYPTO_STATUS_PR_BUSY_Pos         (5UL)                 /*!< CRYPTO STATUS: PR_BUSY (Bit 5)                            */
#define CRYPTO_STATUS_PR_BUSY_Msk         (0x20UL)              /*!< CRYPTO STATUS: PR_BUSY (Bitfield-Mask: 0x01)              */
#define CRYPTO_STATUS_TR_BUSY_Pos         (6UL)                 /*!< CRYPTO STATUS: TR_BUSY (Bit 6)                            */
#define CRYPTO_STATUS_TR_BUSY_Msk         (0x40UL)              /*!< CRYPTO STATUS: TR_BUSY (Bitfield-Mask: 0x01)              */
#define CRYPTO_STATUS_VU_BUSY_Pos         (7UL)                 /*!< CRYPTO STATUS: VU_BUSY (Bit 7)                            */
#define CRYPTO_STATUS_VU_BUSY_Msk         (0x80UL)              /*!< CRYPTO STATUS: VU_BUSY (Bitfield-Mask: 0x01)              */
#define CRYPTO_STATUS_CMD_FF_BUSY_Pos     (31UL)                /*!< CRYPTO STATUS: CMD_FF_BUSY (Bit 31)                       */
#define CRYPTO_STATUS_CMD_FF_BUSY_Msk     (0x80000000UL)        /*!< CRYPTO STATUS: CMD_FF_BUSY (Bitfield-Mask: 0x01)          */
/* =================================================  CRYPTO.ERROR_STATUS0  ================================================== */
#define CRYPTO_ERROR_STATUS0_DATA32_Pos   (0UL)                 /*!< CRYPTO ERROR_STATUS0: DATA32 (Bit 0)                      */
#define CRYPTO_ERROR_STATUS0_DATA32_Msk   (0xffffffffUL)        /*!< CRYPTO ERROR_STATUS0: DATA32 (Bitfield-Mask: 0xffffffff)  */
/* =================================================  CRYPTO.ERROR_STATUS1  ================================================== */
#define CRYPTO_ERROR_STATUS1_DATA23_Pos   (0UL)                 /*!< CRYPTO ERROR_STATUS1: DATA23 (Bit 0)                      */
#define CRYPTO_ERROR_STATUS1_DATA23_Msk   (0xffffffUL)          /*!< CRYPTO ERROR_STATUS1: DATA23 (Bitfield-Mask: 0xffffff)    */
#define CRYPTO_ERROR_STATUS1_IDX_Pos      (24UL)                /*!< CRYPTO ERROR_STATUS1: IDX (Bit 24)                        */
#define CRYPTO_ERROR_STATUS1_IDX_Msk      (0x7000000UL)         /*!< CRYPTO ERROR_STATUS1: IDX (Bitfield-Mask: 0x07)           */
#define CRYPTO_ERROR_STATUS1_VALID_Pos    (31UL)                /*!< CRYPTO ERROR_STATUS1: VALID (Bit 31)                      */
#define CRYPTO_ERROR_STATUS1_VALID_Msk    (0x80000000UL)        /*!< CRYPTO ERROR_STATUS1: VALID (Bitfield-Mask: 0x01)         */
/* ==================================================  CRYPTO.INSTR_FF_CTL  ================================================== */
#define CRYPTO_INSTR_FF_CTL_EVENT_LEVEL_Pos (0UL)               /*!< CRYPTO INSTR_FF_CTL: EVENT_LEVEL (Bit 0)                  */
#define CRYPTO_INSTR_FF_CTL_EVENT_LEVEL_Msk (0x7UL)             /*!< CRYPTO INSTR_FF_CTL: EVENT_LEVEL (Bitfield-Mask: 0x07)    */
#define CRYPTO_INSTR_FF_CTL_CLEAR_Pos     (16UL)                /*!< CRYPTO INSTR_FF_CTL: CLEAR (Bit 16)                       */
#define CRYPTO_INSTR_FF_CTL_CLEAR_Msk     (0x10000UL)           /*!< CRYPTO INSTR_FF_CTL: CLEAR (Bitfield-Mask: 0x01)          */
#define CRYPTO_INSTR_FF_CTL_BLOCK_Pos     (17UL)                /*!< CRYPTO INSTR_FF_CTL: BLOCK (Bit 17)                       */
#define CRYPTO_INSTR_FF_CTL_BLOCK_Msk     (0x20000UL)           /*!< CRYPTO INSTR_FF_CTL: BLOCK (Bitfield-Mask: 0x01)          */
/* ================================================  CRYPTO.INSTR_FF_STATUS  ================================================= */
#define CRYPTO_INSTR_FF_STATUS_USED_Pos   (0UL)                 /*!< CRYPTO INSTR_FF_STATUS: USED (Bit 0)                      */
#define CRYPTO_INSTR_FF_STATUS_USED_Msk   (0xfUL)               /*!< CRYPTO INSTR_FF_STATUS: USED (Bitfield-Mask: 0x0f)        */
#define CRYPTO_INSTR_FF_STATUS_EVENT_Pos  (16UL)                /*!< CRYPTO INSTR_FF_STATUS: EVENT (Bit 16)                    */
#define CRYPTO_INSTR_FF_STATUS_EVENT_Msk  (0x10000UL)           /*!< CRYPTO INSTR_FF_STATUS: EVENT (Bitfield-Mask: 0x01)       */
#define CRYPTO_INSTR_FF_STATUS_BUSY_Pos   (31UL)                /*!< CRYPTO INSTR_FF_STATUS: BUSY (Bit 31)                     */
#define CRYPTO_INSTR_FF_STATUS_BUSY_Msk   (0x80000000UL)        /*!< CRYPTO INSTR_FF_STATUS: BUSY (Bitfield-Mask: 0x01)        */
/* ==================================================  CRYPTO.INSTR_FF_WR  =================================================== */
#define CRYPTO_INSTR_FF_WR_DATA32_Pos     (0UL)                 /*!< CRYPTO INSTR_FF_WR: DATA32 (Bit 0)                        */
#define CRYPTO_INSTR_FF_WR_DATA32_Msk     (0xffffffffUL)        /*!< CRYPTO INSTR_FF_WR: DATA32 (Bitfield-Mask: 0xffffffff)    */
/* ====================================================  CRYPTO.RF_DATA  ===================================================== */
#define CRYPTO_RF_DATA_DATA32_Pos         (0UL)                 /*!< CRYPTO RF_DATA: DATA32 (Bit 0)                            */
#define CRYPTO_RF_DATA_DATA32_Msk         (0xffffffffUL)        /*!< CRYPTO RF_DATA: DATA32 (Bitfield-Mask: 0xffffffff)        */
/* ====================================================  CRYPTO.AES_CTL  ===================================================== */
#define CRYPTO_AES_CTL_KEY_SIZE_Pos       (0UL)                 /*!< CRYPTO AES_CTL: KEY_SIZE (Bit 0)                          */
#define CRYPTO_AES_CTL_KEY_SIZE_Msk       (0x3UL)               /*!< CRYPTO AES_CTL: KEY_SIZE (Bitfield-Mask: 0x03)            */
/* ===================================================  CRYPTO.STR_RESULT  =================================================== */
#define CRYPTO_STR_RESULT_MEMCMP_Pos      (0UL)                 /*!< CRYPTO STR_RESULT: MEMCMP (Bit 0)                         */
#define CRYPTO_STR_RESULT_MEMCMP_Msk      (0x1UL)               /*!< CRYPTO STR_RESULT: MEMCMP (Bitfield-Mask: 0x01)           */
/* ==================================================  CRYPTO.PR_LFSR_CTL0  ================================================== */
#define CRYPTO_PR_LFSR_CTL0_LFSR32_Pos    (0UL)                 /*!< CRYPTO PR_LFSR_CTL0: LFSR32 (Bit 0)                       */
#define CRYPTO_PR_LFSR_CTL0_LFSR32_Msk    (0xffffffffUL)        /*!< CRYPTO PR_LFSR_CTL0: LFSR32 (Bitfield-Mask: 0xffffffff)   */
/* ==================================================  CRYPTO.PR_LFSR_CTL1  ================================================== */
#define CRYPTO_PR_LFSR_CTL1_LFSR31_Pos    (0UL)                 /*!< CRYPTO PR_LFSR_CTL1: LFSR31 (Bit 0)                       */
#define CRYPTO_PR_LFSR_CTL1_LFSR31_Msk    (0x7fffffffUL)        /*!< CRYPTO PR_LFSR_CTL1: LFSR31 (Bitfield-Mask: 0x7fffffff)   */
/* ==================================================  CRYPTO.PR_LFSR_CTL2  ================================================== */
#define CRYPTO_PR_LFSR_CTL2_LFSR29_Pos    (0UL)                 /*!< CRYPTO PR_LFSR_CTL2: LFSR29 (Bit 0)                       */
#define CRYPTO_PR_LFSR_CTL2_LFSR29_Msk    (0x1fffffffUL)        /*!< CRYPTO PR_LFSR_CTL2: LFSR29 (Bitfield-Mask: 0x1fffffff)   */
/* ===================================================  CRYPTO.PR_RESULT  ==================================================== */
#define CRYPTO_PR_RESULT_DATA32_Pos       (0UL)                 /*!< CRYPTO PR_RESULT: DATA32 (Bit 0)                          */
#define CRYPTO_PR_RESULT_DATA32_Msk       (0xffffffffUL)        /*!< CRYPTO PR_RESULT: DATA32 (Bitfield-Mask: 0xffffffff)      */
/* ====================================================  CRYPTO.TR_CTL0  ===================================================== */
#define CRYPTO_TR_CTL0_SAMPLE_CLOCK_DIV_Pos (0UL)               /*!< CRYPTO TR_CTL0: SAMPLE_CLOCK_DIV (Bit 0)                  */
#define CRYPTO_TR_CTL0_SAMPLE_CLOCK_DIV_Msk (0xffUL)            /*!< CRYPTO TR_CTL0: SAMPLE_CLOCK_DIV (Bitfield-Mask: 0xff)    */
#define CRYPTO_TR_CTL0_RED_CLOCK_DIV_Pos  (8UL)                 /*!< CRYPTO TR_CTL0: RED_CLOCK_DIV (Bit 8)                     */
#define CRYPTO_TR_CTL0_RED_CLOCK_DIV_Msk  (0xff00UL)            /*!< CRYPTO TR_CTL0: RED_CLOCK_DIV (Bitfield-Mask: 0xff)       */
#define CRYPTO_TR_CTL0_INIT_DELAY_Pos     (16UL)                /*!< CRYPTO TR_CTL0: INIT_DELAY (Bit 16)                       */
#define CRYPTO_TR_CTL0_INIT_DELAY_Msk     (0xff0000UL)          /*!< CRYPTO TR_CTL0: INIT_DELAY (Bitfield-Mask: 0xff)          */
#define CRYPTO_TR_CTL0_VON_NEUMANN_CORR_Pos (24UL)              /*!< CRYPTO TR_CTL0: VON_NEUMANN_CORR (Bit 24)                 */
#define CRYPTO_TR_CTL0_VON_NEUMANN_CORR_Msk (0x1000000UL)       /*!< CRYPTO TR_CTL0: VON_NEUMANN_CORR (Bitfield-Mask: 0x01)    */
#define CRYPTO_TR_CTL0_STOP_ON_AP_DETECT_Pos (28UL)             /*!< CRYPTO TR_CTL0: STOP_ON_AP_DETECT (Bit 28)                */
#define CRYPTO_TR_CTL0_STOP_ON_AP_DETECT_Msk (0x10000000UL)     /*!< CRYPTO TR_CTL0: STOP_ON_AP_DETECT (Bitfield-Mask: 0x01)   */
#define CRYPTO_TR_CTL0_STOP_ON_RC_DETECT_Pos (29UL)             /*!< CRYPTO TR_CTL0: STOP_ON_RC_DETECT (Bit 29)                */
#define CRYPTO_TR_CTL0_STOP_ON_RC_DETECT_Msk (0x20000000UL)     /*!< CRYPTO TR_CTL0: STOP_ON_RC_DETECT (Bitfield-Mask: 0x01)   */
/* ====================================================  CRYPTO.TR_CTL1  ===================================================== */
#define CRYPTO_TR_CTL1_RO11_EN_Pos        (0UL)                 /*!< CRYPTO TR_CTL1: RO11_EN (Bit 0)                           */
#define CRYPTO_TR_CTL1_RO11_EN_Msk        (0x1UL)               /*!< CRYPTO TR_CTL1: RO11_EN (Bitfield-Mask: 0x01)             */
#define CRYPTO_TR_CTL1_RO15_EN_Pos        (1UL)                 /*!< CRYPTO TR_CTL1: RO15_EN (Bit 1)                           */
#define CRYPTO_TR_CTL1_RO15_EN_Msk        (0x2UL)               /*!< CRYPTO TR_CTL1: RO15_EN (Bitfield-Mask: 0x01)             */
#define CRYPTO_TR_CTL1_GARO15_EN_Pos      (2UL)                 /*!< CRYPTO TR_CTL1: GARO15_EN (Bit 2)                         */
#define CRYPTO_TR_CTL1_GARO15_EN_Msk      (0x4UL)               /*!< CRYPTO TR_CTL1: GARO15_EN (Bitfield-Mask: 0x01)           */
#define CRYPTO_TR_CTL1_GARO31_EN_Pos      (3UL)                 /*!< CRYPTO TR_CTL1: GARO31_EN (Bit 3)                         */
#define CRYPTO_TR_CTL1_GARO31_EN_Msk      (0x8UL)               /*!< CRYPTO TR_CTL1: GARO31_EN (Bitfield-Mask: 0x01)           */
#define CRYPTO_TR_CTL1_FIRO15_EN_Pos      (4UL)                 /*!< CRYPTO TR_CTL1: FIRO15_EN (Bit 4)                         */
#define CRYPTO_TR_CTL1_FIRO15_EN_Msk      (0x10UL)              /*!< CRYPTO TR_CTL1: FIRO15_EN (Bitfield-Mask: 0x01)           */
#define CRYPTO_TR_CTL1_FIRO31_EN_Pos      (5UL)                 /*!< CRYPTO TR_CTL1: FIRO31_EN (Bit 5)                         */
#define CRYPTO_TR_CTL1_FIRO31_EN_Msk      (0x20UL)              /*!< CRYPTO TR_CTL1: FIRO31_EN (Bitfield-Mask: 0x01)           */
/* ===================================================  CRYPTO.TR_RESULT  ==================================================== */
#define CRYPTO_TR_RESULT_DATA32_Pos       (0UL)                 /*!< CRYPTO TR_RESULT: DATA32 (Bit 0)                          */
#define CRYPTO_TR_RESULT_DATA32_Msk       (0xffffffffUL)        /*!< CRYPTO TR_RESULT: DATA32 (Bitfield-Mask: 0xffffffff)      */
/* ==================================================  CRYPTO.TR_GARO_CTL  =================================================== */
#define CRYPTO_TR_GARO_CTL_POLYNOMIAL31_Pos (0UL)               /*!< CRYPTO TR_GARO_CTL: POLYNOMIAL31 (Bit 0)                  */
#define CRYPTO_TR_GARO_CTL_POLYNOMIAL31_Msk (0x7fffffffUL)      /*!< CRYPTO TR_GARO_CTL: POLYNOMIAL31 (Bitfield-Mask: 0x7fffffff)*/
/* ==================================================  CRYPTO.TR_FIRO_CTL  =================================================== */
#define CRYPTO_TR_FIRO_CTL_POLYNOMIAL31_Pos (0UL)               /*!< CRYPTO TR_FIRO_CTL: POLYNOMIAL31 (Bit 0)                  */
#define CRYPTO_TR_FIRO_CTL_POLYNOMIAL31_Msk (0x7fffffffUL)      /*!< CRYPTO TR_FIRO_CTL: POLYNOMIAL31 (Bitfield-Mask: 0x7fffffff)*/
/* ===================================================  CRYPTO.TR_MON_CTL  =================================================== */
#define CRYPTO_TR_MON_CTL_BITSTREAM_SEL_Pos (0UL)               /*!< CRYPTO TR_MON_CTL: BITSTREAM_SEL (Bit 0)                  */
#define CRYPTO_TR_MON_CTL_BITSTREAM_SEL_Msk (0x3UL)             /*!< CRYPTO TR_MON_CTL: BITSTREAM_SEL (Bitfield-Mask: 0x03)    */
/* ===================================================  CRYPTO.TR_MON_CMD  =================================================== */
#define CRYPTO_TR_MON_CMD_START_AP_Pos    (0UL)                 /*!< CRYPTO TR_MON_CMD: START_AP (Bit 0)                       */
#define CRYPTO_TR_MON_CMD_START_AP_Msk    (0x1UL)               /*!< CRYPTO TR_MON_CMD: START_AP (Bitfield-Mask: 0x01)         */
#define CRYPTO_TR_MON_CMD_START_RC_Pos    (1UL)                 /*!< CRYPTO TR_MON_CMD: START_RC (Bit 1)                       */
#define CRYPTO_TR_MON_CMD_START_RC_Msk    (0x2UL)               /*!< CRYPTO TR_MON_CMD: START_RC (Bitfield-Mask: 0x01)         */
/* =================================================  CRYPTO.TR_MON_RC_CTL  ================================================== */
#define CRYPTO_TR_MON_RC_CTL_CUTOFF_COUNT8_Pos (0UL)            /*!< CRYPTO TR_MON_RC_CTL: CUTOFF_COUNT8 (Bit 0)               */
#define CRYPTO_TR_MON_RC_CTL_CUTOFF_COUNT8_Msk (0xffUL)         /*!< CRYPTO TR_MON_RC_CTL: CUTOFF_COUNT8 (Bitfield-Mask: 0xff) */
/* ===============================================  CRYPTO.TR_MON_RC_STATUS0  ================================================ */
#define CRYPTO_TR_MON_RC_STATUS0_BIT_Pos  (0UL)                 /*!< CRYPTO TR_MON_RC_STATUS0: BIT (Bit 0)                     */
#define CRYPTO_TR_MON_RC_STATUS0_BIT_Msk  (0x1UL)               /*!< CRYPTO TR_MON_RC_STATUS0: BIT (Bitfield-Mask: 0x01)       */
/* ===============================================  CRYPTO.TR_MON_RC_STATUS1  ================================================ */
#define CRYPTO_TR_MON_RC_STATUS1_REP_COUNT_Pos (0UL)            /*!< CRYPTO TR_MON_RC_STATUS1: REP_COUNT (Bit 0)               */
#define CRYPTO_TR_MON_RC_STATUS1_REP_COUNT_Msk (0xffUL)         /*!< CRYPTO TR_MON_RC_STATUS1: REP_COUNT (Bitfield-Mask: 0xff) */
/* =================================================  CRYPTO.TR_MON_AP_CTL  ================================================== */
#define CRYPTO_TR_MON_AP_CTL_CUTOFF_COUNT16_Pos (0UL)           /*!< CRYPTO TR_MON_AP_CTL: CUTOFF_COUNT16 (Bit 0)              */
#define CRYPTO_TR_MON_AP_CTL_CUTOFF_COUNT16_Msk (0xffffUL)      /*!< CRYPTO TR_MON_AP_CTL: CUTOFF_COUNT16 (Bitfield-Mask: 0xffff)*/
#define CRYPTO_TR_MON_AP_CTL_WINDOW_SIZE_Pos (16UL)             /*!< CRYPTO TR_MON_AP_CTL: WINDOW_SIZE (Bit 16)                */
#define CRYPTO_TR_MON_AP_CTL_WINDOW_SIZE_Msk (0xffff0000UL)     /*!< CRYPTO TR_MON_AP_CTL: WINDOW_SIZE (Bitfield-Mask: 0xffff) */
/* ===============================================  CRYPTO.TR_MON_AP_STATUS0  ================================================ */
#define CRYPTO_TR_MON_AP_STATUS0_BIT_Pos  (0UL)                 /*!< CRYPTO TR_MON_AP_STATUS0: BIT (Bit 0)                     */
#define CRYPTO_TR_MON_AP_STATUS0_BIT_Msk  (0x1UL)               /*!< CRYPTO TR_MON_AP_STATUS0: BIT (Bitfield-Mask: 0x01)       */
/* ===============================================  CRYPTO.TR_MON_AP_STATUS1  ================================================ */
#define CRYPTO_TR_MON_AP_STATUS1_OCC_COUNT_Pos (0UL)            /*!< CRYPTO TR_MON_AP_STATUS1: OCC_COUNT (Bit 0)               */
#define CRYPTO_TR_MON_AP_STATUS1_OCC_COUNT_Msk (0xffffUL)       /*!< CRYPTO TR_MON_AP_STATUS1: OCC_COUNT (Bitfield-Mask: 0xffff)*/
#define CRYPTO_TR_MON_AP_STATUS1_WINDOW_INDEX_Pos (16UL)        /*!< CRYPTO TR_MON_AP_STATUS1: WINDOW_INDEX (Bit 16)           */
#define CRYPTO_TR_MON_AP_STATUS1_WINDOW_INDEX_Msk (0xffff0000UL) /*!< CRYPTO TR_MON_AP_STATUS1: WINDOW_INDEX (Bitfield-Mask: 0xffff)*/
/* ====================================================  CRYPTO.SHA_CTL  ===================================================== */
#define CRYPTO_SHA_CTL_MODE_Pos           (0UL)                 /*!< CRYPTO SHA_CTL: MODE (Bit 0)                              */
#define CRYPTO_SHA_CTL_MODE_Msk           (0x7UL)               /*!< CRYPTO SHA_CTL: MODE (Bitfield-Mask: 0x07)                */
/* ====================================================  CRYPTO.CRC_CTL  ===================================================== */
#define CRYPTO_CRC_CTL_DATA_REVERSE_Pos   (0UL)                 /*!< CRYPTO CRC_CTL: DATA_REVERSE (Bit 0)                      */
#define CRYPTO_CRC_CTL_DATA_REVERSE_Msk   (0x1UL)               /*!< CRYPTO CRC_CTL: DATA_REVERSE (Bitfield-Mask: 0x01)        */
#define CRYPTO_CRC_CTL_REM_REVERSE_Pos    (8UL)                 /*!< CRYPTO CRC_CTL: REM_REVERSE (Bit 8)                       */
#define CRYPTO_CRC_CTL_REM_REVERSE_Msk    (0x100UL)             /*!< CRYPTO CRC_CTL: REM_REVERSE (Bitfield-Mask: 0x01)         */
/* ==================================================  CRYPTO.CRC_DATA_CTL  ================================================== */
#define CRYPTO_CRC_DATA_CTL_DATA_XOR_Pos  (0UL)                 /*!< CRYPTO CRC_DATA_CTL: DATA_XOR (Bit 0)                     */
#define CRYPTO_CRC_DATA_CTL_DATA_XOR_Msk  (0xffUL)              /*!< CRYPTO CRC_DATA_CTL: DATA_XOR (Bitfield-Mask: 0xff)       */
/* ==================================================  CRYPTO.CRC_POL_CTL  =================================================== */
#define CRYPTO_CRC_POL_CTL_POLYNOMIAL_Pos (0UL)                 /*!< CRYPTO CRC_POL_CTL: POLYNOMIAL (Bit 0)                    */
#define CRYPTO_CRC_POL_CTL_POLYNOMIAL_Msk (0xffffffffUL)        /*!< CRYPTO CRC_POL_CTL: POLYNOMIAL (Bitfield-Mask: 0xffffffff)*/
/* ==================================================  CRYPTO.CRC_LFSR_CTL  ================================================== */
#define CRYPTO_CRC_LFSR_CTL_LFSR32_Pos    (0UL)                 /*!< CRYPTO CRC_LFSR_CTL: LFSR32 (Bit 0)                       */
#define CRYPTO_CRC_LFSR_CTL_LFSR32_Msk    (0xffffffffUL)        /*!< CRYPTO CRC_LFSR_CTL: LFSR32 (Bitfield-Mask: 0xffffffff)   */
/* ==================================================  CRYPTO.CRC_REM_CTL  =================================================== */
#define CRYPTO_CRC_REM_CTL_REM_XOR_Pos    (0UL)                 /*!< CRYPTO CRC_REM_CTL: REM_XOR (Bit 0)                       */
#define CRYPTO_CRC_REM_CTL_REM_XOR_Msk    (0xffffffffUL)        /*!< CRYPTO CRC_REM_CTL: REM_XOR (Bitfield-Mask: 0xffffffff)   */
/* =================================================  CRYPTO.CRC_REM_RESULT  ================================================= */
#define CRYPTO_CRC_REM_RESULT_REM_Pos     (0UL)                 /*!< CRYPTO CRC_REM_RESULT: REM (Bit 0)                        */
#define CRYPTO_CRC_REM_RESULT_REM_Msk     (0xffffffffUL)        /*!< CRYPTO CRC_REM_RESULT: REM (Bitfield-Mask: 0xffffffff)    */
/* ====================================================  CRYPTO.VU_CTL0  ===================================================== */
#define CRYPTO_VU_CTL0_ALWAYS_EXECUTE_Pos (0UL)                 /*!< CRYPTO VU_CTL0: ALWAYS_EXECUTE (Bit 0)                    */
#define CRYPTO_VU_CTL0_ALWAYS_EXECUTE_Msk (0x1UL)               /*!< CRYPTO VU_CTL0: ALWAYS_EXECUTE (Bitfield-Mask: 0x01)      */
/* ====================================================  CRYPTO.VU_CTL1  ===================================================== */
#define CRYPTO_VU_CTL1_ADDR_Pos           (14UL)                /*!< CRYPTO VU_CTL1: ADDR (Bit 14)                             */
#define CRYPTO_VU_CTL1_ADDR_Msk           (0xffffc000UL)        /*!< CRYPTO VU_CTL1: ADDR (Bitfield-Mask: 0x3ffff)             */
/* ===================================================  CRYPTO.VU_STATUS  ==================================================== */
#define CRYPTO_VU_STATUS_CARRY_Pos        (0UL)                 /*!< CRYPTO VU_STATUS: CARRY (Bit 0)                           */
#define CRYPTO_VU_STATUS_CARRY_Msk        (0x1UL)               /*!< CRYPTO VU_STATUS: CARRY (Bitfield-Mask: 0x01)             */
#define CRYPTO_VU_STATUS_EVEN_Pos         (1UL)                 /*!< CRYPTO VU_STATUS: EVEN (Bit 1)                            */
#define CRYPTO_VU_STATUS_EVEN_Msk         (0x2UL)               /*!< CRYPTO VU_STATUS: EVEN (Bitfield-Mask: 0x01)              */
#define CRYPTO_VU_STATUS_ZERO_Pos         (2UL)                 /*!< CRYPTO VU_STATUS: ZERO (Bit 2)                            */
#define CRYPTO_VU_STATUS_ZERO_Msk         (0x4UL)               /*!< CRYPTO VU_STATUS: ZERO (Bitfield-Mask: 0x01)              */
#define CRYPTO_VU_STATUS_ONE_Pos          (3UL)                 /*!< CRYPTO VU_STATUS: ONE (Bit 3)                             */
#define CRYPTO_VU_STATUS_ONE_Msk          (0x8UL)               /*!< CRYPTO VU_STATUS: ONE (Bitfield-Mask: 0x01)               */
/* ======================================================  CRYPTO.INTR  ====================================================== */
#define CRYPTO_INTR_INSTR_FF_LEVEL_Pos    (0UL)                 /*!< CRYPTO INTR: INSTR_FF_LEVEL (Bit 0)                       */
#define CRYPTO_INTR_INSTR_FF_LEVEL_Msk    (0x1UL)               /*!< CRYPTO INTR: INSTR_FF_LEVEL (Bitfield-Mask: 0x01)         */
#define CRYPTO_INTR_INSTR_FF_OVERFLOW_Pos (1UL)                 /*!< CRYPTO INTR: INSTR_FF_OVERFLOW (Bit 1)                    */
#define CRYPTO_INTR_INSTR_FF_OVERFLOW_Msk (0x2UL)               /*!< CRYPTO INTR: INSTR_FF_OVERFLOW (Bitfield-Mask: 0x01)      */
#define CRYPTO_INTR_TR_INITIALIZED_Pos    (2UL)                 /*!< CRYPTO INTR: TR_INITIALIZED (Bit 2)                       */
#define CRYPTO_INTR_TR_INITIALIZED_Msk    (0x4UL)               /*!< CRYPTO INTR: TR_INITIALIZED (Bitfield-Mask: 0x01)         */
#define CRYPTO_INTR_TR_DATA_AVAILABLE_Pos (3UL)                 /*!< CRYPTO INTR: TR_DATA_AVAILABLE (Bit 3)                    */
#define CRYPTO_INTR_TR_DATA_AVAILABLE_Msk (0x8UL)               /*!< CRYPTO INTR: TR_DATA_AVAILABLE (Bitfield-Mask: 0x01)      */
#define CRYPTO_INTR_PR_DATA_AVAILABLE_Pos (4UL)                 /*!< CRYPTO INTR: PR_DATA_AVAILABLE (Bit 4)                    */
#define CRYPTO_INTR_PR_DATA_AVAILABLE_Msk (0x10UL)              /*!< CRYPTO INTR: PR_DATA_AVAILABLE (Bitfield-Mask: 0x01)      */
#define CRYPTO_INTR_INSTR_OPC_ERROR_Pos   (16UL)                /*!< CRYPTO INTR: INSTR_OPC_ERROR (Bit 16)                     */
#define CRYPTO_INTR_INSTR_OPC_ERROR_Msk   (0x10000UL)           /*!< CRYPTO INTR: INSTR_OPC_ERROR (Bitfield-Mask: 0x01)        */
#define CRYPTO_INTR_INSTR_CC_ERROR_Pos    (17UL)                /*!< CRYPTO INTR: INSTR_CC_ERROR (Bit 17)                      */
#define CRYPTO_INTR_INSTR_CC_ERROR_Msk    (0x20000UL)           /*!< CRYPTO INTR: INSTR_CC_ERROR (Bitfield-Mask: 0x01)         */
#define CRYPTO_INTR_BUS_ERROR_Pos         (18UL)                /*!< CRYPTO INTR: BUS_ERROR (Bit 18)                           */
#define CRYPTO_INTR_BUS_ERROR_Msk         (0x40000UL)           /*!< CRYPTO INTR: BUS_ERROR (Bitfield-Mask: 0x01)              */
#define CRYPTO_INTR_TR_AP_DETECT_ERROR_Pos (19UL)               /*!< CRYPTO INTR: TR_AP_DETECT_ERROR (Bit 19)                  */
#define CRYPTO_INTR_TR_AP_DETECT_ERROR_Msk (0x80000UL)          /*!< CRYPTO INTR: TR_AP_DETECT_ERROR (Bitfield-Mask: 0x01)     */
#define CRYPTO_INTR_TR_RC_DETECT_ERROR_Pos (20UL)               /*!< CRYPTO INTR: TR_RC_DETECT_ERROR (Bit 20)                  */
#define CRYPTO_INTR_TR_RC_DETECT_ERROR_Msk (0x100000UL)         /*!< CRYPTO INTR: TR_RC_DETECT_ERROR (Bitfield-Mask: 0x01)     */
/* ====================================================  CRYPTO.INTR_SET  ==================================================== */
#define CRYPTO_INTR_SET_INSTR_FF_LEVEL_Pos (0UL)                /*!< CRYPTO INTR_SET: INSTR_FF_LEVEL (Bit 0)                   */
#define CRYPTO_INTR_SET_INSTR_FF_LEVEL_Msk (0x1UL)              /*!< CRYPTO INTR_SET: INSTR_FF_LEVEL (Bitfield-Mask: 0x01)     */
#define CRYPTO_INTR_SET_INSTR_FF_OVERFLOW_Pos (1UL)             /*!< CRYPTO INTR_SET: INSTR_FF_OVERFLOW (Bit 1)                */
#define CRYPTO_INTR_SET_INSTR_FF_OVERFLOW_Msk (0x2UL)           /*!< CRYPTO INTR_SET: INSTR_FF_OVERFLOW (Bitfield-Mask: 0x01)  */
#define CRYPTO_INTR_SET_TR_INITIALIZED_Pos (2UL)                /*!< CRYPTO INTR_SET: TR_INITIALIZED (Bit 2)                   */
#define CRYPTO_INTR_SET_TR_INITIALIZED_Msk (0x4UL)              /*!< CRYPTO INTR_SET: TR_INITIALIZED (Bitfield-Mask: 0x01)     */
#define CRYPTO_INTR_SET_TR_DATA_AVAILABLE_Pos (3UL)             /*!< CRYPTO INTR_SET: TR_DATA_AVAILABLE (Bit 3)                */
#define CRYPTO_INTR_SET_TR_DATA_AVAILABLE_Msk (0x8UL)           /*!< CRYPTO INTR_SET: TR_DATA_AVAILABLE (Bitfield-Mask: 0x01)  */
#define CRYPTO_INTR_SET_PR_DATA_AVAILABLE_Pos (4UL)             /*!< CRYPTO INTR_SET: PR_DATA_AVAILABLE (Bit 4)                */
#define CRYPTO_INTR_SET_PR_DATA_AVAILABLE_Msk (0x10UL)          /*!< CRYPTO INTR_SET: PR_DATA_AVAILABLE (Bitfield-Mask: 0x01)  */
#define CRYPTO_INTR_SET_INSTR_OPC_ERROR_Pos (16UL)              /*!< CRYPTO INTR_SET: INSTR_OPC_ERROR (Bit 16)                 */
#define CRYPTO_INTR_SET_INSTR_OPC_ERROR_Msk (0x10000UL)         /*!< CRYPTO INTR_SET: INSTR_OPC_ERROR (Bitfield-Mask: 0x01)    */
#define CRYPTO_INTR_SET_INSTR_CC_ERROR_Pos (17UL)               /*!< CRYPTO INTR_SET: INSTR_CC_ERROR (Bit 17)                  */
#define CRYPTO_INTR_SET_INSTR_CC_ERROR_Msk (0x20000UL)          /*!< CRYPTO INTR_SET: INSTR_CC_ERROR (Bitfield-Mask: 0x01)     */
#define CRYPTO_INTR_SET_BUS_ERROR_Pos     (18UL)                /*!< CRYPTO INTR_SET: BUS_ERROR (Bit 18)                       */
#define CRYPTO_INTR_SET_BUS_ERROR_Msk     (0x40000UL)           /*!< CRYPTO INTR_SET: BUS_ERROR (Bitfield-Mask: 0x01)          */
#define CRYPTO_INTR_SET_TR_AP_DETECT_ERROR_Pos (19UL)           /*!< CRYPTO INTR_SET: TR_AP_DETECT_ERROR (Bit 19)              */
#define CRYPTO_INTR_SET_TR_AP_DETECT_ERROR_Msk (0x80000UL)      /*!< CRYPTO INTR_SET: TR_AP_DETECT_ERROR (Bitfield-Mask: 0x01) */
#define CRYPTO_INTR_SET_TR_RC_DETECT_ERROR_Pos (20UL)           /*!< CRYPTO INTR_SET: TR_RC_DETECT_ERROR (Bit 20)              */
#define CRYPTO_INTR_SET_TR_RC_DETECT_ERROR_Msk (0x100000UL)     /*!< CRYPTO INTR_SET: TR_RC_DETECT_ERROR (Bitfield-Mask: 0x01) */
/* ===================================================  CRYPTO.INTR_MASK  ==================================================== */
#define CRYPTO_INTR_MASK_INSTR_FF_LEVEL_Pos (0UL)               /*!< CRYPTO INTR_MASK: INSTR_FF_LEVEL (Bit 0)                  */
#define CRYPTO_INTR_MASK_INSTR_FF_LEVEL_Msk (0x1UL)             /*!< CRYPTO INTR_MASK: INSTR_FF_LEVEL (Bitfield-Mask: 0x01)    */
#define CRYPTO_INTR_MASK_INSTR_FF_OVERFLOW_Pos (1UL)            /*!< CRYPTO INTR_MASK: INSTR_FF_OVERFLOW (Bit 1)               */
#define CRYPTO_INTR_MASK_INSTR_FF_OVERFLOW_Msk (0x2UL)          /*!< CRYPTO INTR_MASK: INSTR_FF_OVERFLOW (Bitfield-Mask: 0x01) */
#define CRYPTO_INTR_MASK_TR_INITIALIZED_Pos (2UL)               /*!< CRYPTO INTR_MASK: TR_INITIALIZED (Bit 2)                  */
#define CRYPTO_INTR_MASK_TR_INITIALIZED_Msk (0x4UL)             /*!< CRYPTO INTR_MASK: TR_INITIALIZED (Bitfield-Mask: 0x01)    */
#define CRYPTO_INTR_MASK_TR_DATA_AVAILABLE_Pos (3UL)            /*!< CRYPTO INTR_MASK: TR_DATA_AVAILABLE (Bit 3)               */
#define CRYPTO_INTR_MASK_TR_DATA_AVAILABLE_Msk (0x8UL)          /*!< CRYPTO INTR_MASK: TR_DATA_AVAILABLE (Bitfield-Mask: 0x01) */
#define CRYPTO_INTR_MASK_PR_DATA_AVAILABLE_Pos (4UL)            /*!< CRYPTO INTR_MASK: PR_DATA_AVAILABLE (Bit 4)               */
#define CRYPTO_INTR_MASK_PR_DATA_AVAILABLE_Msk (0x10UL)         /*!< CRYPTO INTR_MASK: PR_DATA_AVAILABLE (Bitfield-Mask: 0x01) */
#define CRYPTO_INTR_MASK_INSTR_OPC_ERROR_Pos (16UL)             /*!< CRYPTO INTR_MASK: INSTR_OPC_ERROR (Bit 16)                */
#define CRYPTO_INTR_MASK_INSTR_OPC_ERROR_Msk (0x10000UL)        /*!< CRYPTO INTR_MASK: INSTR_OPC_ERROR (Bitfield-Mask: 0x01)   */
#define CRYPTO_INTR_MASK_INSTR_CC_ERROR_Pos (17UL)              /*!< CRYPTO INTR_MASK: INSTR_CC_ERROR (Bit 17)                 */
#define CRYPTO_INTR_MASK_INSTR_CC_ERROR_Msk (0x20000UL)         /*!< CRYPTO INTR_MASK: INSTR_CC_ERROR (Bitfield-Mask: 0x01)    */
#define CRYPTO_INTR_MASK_BUS_ERROR_Pos    (18UL)                /*!< CRYPTO INTR_MASK: BUS_ERROR (Bit 18)                      */
#define CRYPTO_INTR_MASK_BUS_ERROR_Msk    (0x40000UL)           /*!< CRYPTO INTR_MASK: BUS_ERROR (Bitfield-Mask: 0x01)         */
#define CRYPTO_INTR_MASK_TR_AP_DETECT_ERROR_Pos (19UL)          /*!< CRYPTO INTR_MASK: TR_AP_DETECT_ERROR (Bit 19)             */
#define CRYPTO_INTR_MASK_TR_AP_DETECT_ERROR_Msk (0x80000UL)     /*!< CRYPTO INTR_MASK: TR_AP_DETECT_ERROR (Bitfield-Mask: 0x01)*/
#define CRYPTO_INTR_MASK_TR_RC_DETECT_ERROR_Pos (20UL)          /*!< CRYPTO INTR_MASK: TR_RC_DETECT_ERROR (Bit 20)             */
#define CRYPTO_INTR_MASK_TR_RC_DETECT_ERROR_Msk (0x100000UL)    /*!< CRYPTO INTR_MASK: TR_RC_DETECT_ERROR (Bitfield-Mask: 0x01)*/
/* ==================================================  CRYPTO.INTR_MASKED  =================================================== */
#define CRYPTO_INTR_MASKED_INSTR_FF_LEVEL_Pos (0UL)             /*!< CRYPTO INTR_MASKED: INSTR_FF_LEVEL (Bit 0)                */
#define CRYPTO_INTR_MASKED_INSTR_FF_LEVEL_Msk (0x1UL)           /*!< CRYPTO INTR_MASKED: INSTR_FF_LEVEL (Bitfield-Mask: 0x01)  */
#define CRYPTO_INTR_MASKED_INSTR_FF_OVERFLOW_Pos (1UL)          /*!< CRYPTO INTR_MASKED: INSTR_FF_OVERFLOW (Bit 1)             */
#define CRYPTO_INTR_MASKED_INSTR_FF_OVERFLOW_Msk (0x2UL)        /*!< CRYPTO INTR_MASKED: INSTR_FF_OVERFLOW (Bitfield-Mask: 0x01)*/
#define CRYPTO_INTR_MASKED_TR_INITIALIZED_Pos (2UL)             /*!< CRYPTO INTR_MASKED: TR_INITIALIZED (Bit 2)                */
#define CRYPTO_INTR_MASKED_TR_INITIALIZED_Msk (0x4UL)           /*!< CRYPTO INTR_MASKED: TR_INITIALIZED (Bitfield-Mask: 0x01)  */
#define CRYPTO_INTR_MASKED_TR_DATA_AVAILABLE_Pos (3UL)          /*!< CRYPTO INTR_MASKED: TR_DATA_AVAILABLE (Bit 3)             */
#define CRYPTO_INTR_MASKED_TR_DATA_AVAILABLE_Msk (0x8UL)        /*!< CRYPTO INTR_MASKED: TR_DATA_AVAILABLE (Bitfield-Mask: 0x01)*/
#define CRYPTO_INTR_MASKED_PR_DATA_AVAILABLE_Pos (4UL)          /*!< CRYPTO INTR_MASKED: PR_DATA_AVAILABLE (Bit 4)             */
#define CRYPTO_INTR_MASKED_PR_DATA_AVAILABLE_Msk (0x10UL)       /*!< CRYPTO INTR_MASKED: PR_DATA_AVAILABLE (Bitfield-Mask: 0x01)*/
#define CRYPTO_INTR_MASKED_INSTR_OPC_ERROR_Pos (16UL)           /*!< CRYPTO INTR_MASKED: INSTR_OPC_ERROR (Bit 16)              */
#define CRYPTO_INTR_MASKED_INSTR_OPC_ERROR_Msk (0x10000UL)      /*!< CRYPTO INTR_MASKED: INSTR_OPC_ERROR (Bitfield-Mask: 0x01) */
#define CRYPTO_INTR_MASKED_INSTR_CC_ERROR_Pos (17UL)            /*!< CRYPTO INTR_MASKED: INSTR_CC_ERROR (Bit 17)               */
#define CRYPTO_INTR_MASKED_INSTR_CC_ERROR_Msk (0x20000UL)       /*!< CRYPTO INTR_MASKED: INSTR_CC_ERROR (Bitfield-Mask: 0x01)  */
#define CRYPTO_INTR_MASKED_BUS_ERROR_Pos  (18UL)                /*!< CRYPTO INTR_MASKED: BUS_ERROR (Bit 18)                    */
#define CRYPTO_INTR_MASKED_BUS_ERROR_Msk  (0x40000UL)           /*!< CRYPTO INTR_MASKED: BUS_ERROR (Bitfield-Mask: 0x01)       */
#define CRYPTO_INTR_MASKED_TR_AP_DETECT_ERROR_Pos (19UL)        /*!< CRYPTO INTR_MASKED: TR_AP_DETECT_ERROR (Bit 19)           */
#define CRYPTO_INTR_MASKED_TR_AP_DETECT_ERROR_Msk (0x80000UL)   /*!< CRYPTO INTR_MASKED: TR_AP_DETECT_ERROR (Bitfield-Mask: 0x01)*/
#define CRYPTO_INTR_MASKED_TR_RC_DETECT_ERROR_Pos (20UL)        /*!< CRYPTO INTR_MASKED: TR_RC_DETECT_ERROR (Bit 20)           */
#define CRYPTO_INTR_MASKED_TR_RC_DETECT_ERROR_Msk (0x100000UL)  /*!< CRYPTO INTR_MASKED: TR_RC_DETECT_ERROR (Bitfield-Mask: 0x01)*/
/* ====================================================  CRYPTO.MEM_BUFF  ==================================================== */
#define CRYPTO_MEM_BUFF_DATA32_Pos        (0UL)                 /*!< CRYPTO MEM_BUFF: DATA32 (Bit 0)                           */
#define CRYPTO_MEM_BUFF_DATA32_Msk        (0xffffffffUL)        /*!< CRYPTO MEM_BUFF: DATA32 (Bitfield-Mask: 0xffffffff)       */


#endif /* _CYIP_CRYPTO_H_ */


/* [] END OF FILE */
