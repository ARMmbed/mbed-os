/***************************************************************************//**
* \file cyip_i2s.h
*
* \brief
* I2S IP definitions
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

#ifndef _CYIP_I2S_H_
#define _CYIP_I2S_H_

#include "cyip_headers.h"

/* =========================================================================================================================== */
/* ================                                            I2S                                            ================ */
/* =========================================================================================================================== */

#define I2S_SECTION_SIZE            0x00001000UL

/**
  * \brief I2S registers (I2S)
  */
typedef struct {                                /*!< I2S Structure                                                             */
  __IOM uint32_t CTL;                           /*!< 0x00000000 Control                                                        */
   __IM uint32_t RESERVED[3];
  __IOM uint32_t CLOCK_CTL;                     /*!< 0x00000010 Clock control                                                  */
  __IOM uint32_t COMMON_CTL;                    /*!< 0x00000014 Common control                                                 */
   __IM uint32_t RESERVED1[2];
  __IOM uint32_t CMD;                           /*!< 0x00000020 Command                                                        */
   __IM uint32_t RESERVED2[7];
  __IOM uint32_t TR_CTL;                        /*!< 0x00000040 Trigger control                                                */
   __IM uint32_t RESERVED3[15];
  __IOM uint32_t TX_CTL;                        /*!< 0x00000080 Transmitter control                                            */
  __IOM uint32_t TX_WATCHDOG;                   /*!< 0x00000084 Transmitter watchdog                                           */
   __IM uint32_t RESERVED4[6];
  __IOM uint32_t RX_CTL;                        /*!< 0x000000A0 Receiver control                                               */
  __IOM uint32_t RX_WATCHDOG;                   /*!< 0x000000A4 Receiver watchdog                                              */
   __IM uint32_t RESERVED5[86];
  __IOM uint32_t TX_FIFO_CTL;                   /*!< 0x00000200 TX FIFO control                                                */
   __IM uint32_t TX_FIFO_STATUS;                /*!< 0x00000204 TX FIFO status                                                 */
   __OM uint32_t TX_FIFO_WR;                    /*!< 0x00000208 TX FIFO write                                                  */
   __IM uint32_t RESERVED6[61];
  __IOM uint32_t RX_FIFO_CTL;                   /*!< 0x00000300 RX FIFO control                                                */
   __IM uint32_t RX_FIFO_STATUS;                /*!< 0x00000304 RX FIFO status                                                 */
   __IM uint32_t RX_FIFO_RD;                    /*!< 0x00000308 RX FIFO read                                                   */
   __IM uint32_t RX_FIFO_RD_SILENT;             /*!< 0x0000030C RX FIFO silent read                                            */
   __IM uint32_t RESERVED7[764];
  __IOM uint32_t INTR;                          /*!< 0x00000F00 Interrupt register                                             */
  __IOM uint32_t INTR_SET;                      /*!< 0x00000F04 Interrupt set register                                         */
  __IOM uint32_t INTR_MASK;                     /*!< 0x00000F08 Interrupt mask register                                        */
   __IM uint32_t INTR_MASKED;                   /*!< 0x00000F0C Interrupt masked register                                      */
} I2S_Type;                                     /*!< Size = 3856 (0xF10)                                                       */


/* ========================================================  I2S.CTL  ======================================================== */
#define I2S_CTL_TX_ENABLED_Pos            (30UL)                /*!< I2S CTL: TX_ENABLED (Bit 30)                              */
#define I2S_CTL_TX_ENABLED_Msk            (0x40000000UL)        /*!< I2S CTL: TX_ENABLED (Bitfield-Mask: 0x01)                 */
#define I2S_CTL_RX_ENABLED_Pos            (31UL)                /*!< I2S CTL: RX_ENABLED (Bit 31)                              */
#define I2S_CTL_RX_ENABLED_Msk            (0x80000000UL)        /*!< I2S CTL: RX_ENABLED (Bitfield-Mask: 0x01)                 */
/* =====================================================  I2S.CLOCK_CTL  ===================================================== */
#define I2S_CLOCK_CTL_CLOCK_DIV_Pos       (0UL)                 /*!< I2S CLOCK_CTL: CLOCK_DIV (Bit 0)                          */
#define I2S_CLOCK_CTL_CLOCK_DIV_Msk       (0x3fUL)              /*!< I2S CLOCK_CTL: CLOCK_DIV (Bitfield-Mask: 0x3f)            */
#define I2S_CLOCK_CTL_CLOCK_SEL_Pos       (8UL)                 /*!< I2S CLOCK_CTL: CLOCK_SEL (Bit 8)                          */
#define I2S_CLOCK_CTL_CLOCK_SEL_Msk       (0x100UL)             /*!< I2S CLOCK_CTL: CLOCK_SEL (Bitfield-Mask: 0x01)            */
/* ====================================================  I2S.COMMON_CTL  ===================================================== */
#define I2S_COMMON_CTL_LOOPBACK_Pos       (0UL)                 /*!< I2S COMMON_CTL: LOOPBACK (Bit 0)                          */
#define I2S_COMMON_CTL_LOOPBACK_Msk       (0x1UL)               /*!< I2S COMMON_CTL: LOOPBACK (Bitfield-Mask: 0x01)            */
/* ========================================================  I2S.CMD  ======================================================== */
#define I2S_CMD_TX_START_Pos              (0UL)                 /*!< I2S CMD: TX_START (Bit 0)                                 */
#define I2S_CMD_TX_START_Msk              (0x1UL)               /*!< I2S CMD: TX_START (Bitfield-Mask: 0x01)                   */
#define I2S_CMD_TX_PAUSE_Pos              (8UL)                 /*!< I2S CMD: TX_PAUSE (Bit 8)                                 */
#define I2S_CMD_TX_PAUSE_Msk              (0x100UL)             /*!< I2S CMD: TX_PAUSE (Bitfield-Mask: 0x01)                   */
#define I2S_CMD_RX_START_Pos              (16UL)                /*!< I2S CMD: RX_START (Bit 16)                                */
#define I2S_CMD_RX_START_Msk              (0x10000UL)           /*!< I2S CMD: RX_START (Bitfield-Mask: 0x01)                   */
/* ======================================================  I2S.TR_CTL  ======================================================= */
#define I2S_TR_CTL_TX_REQ_EN_Pos          (0UL)                 /*!< I2S TR_CTL: TX_REQ_EN (Bit 0)                             */
#define I2S_TR_CTL_TX_REQ_EN_Msk          (0x1UL)               /*!< I2S TR_CTL: TX_REQ_EN (Bitfield-Mask: 0x01)               */
#define I2S_TR_CTL_RX_REQ_EN_Pos          (16UL)                /*!< I2S TR_CTL: RX_REQ_EN (Bit 16)                            */
#define I2S_TR_CTL_RX_REQ_EN_Msk          (0x10000UL)           /*!< I2S TR_CTL: RX_REQ_EN (Bitfield-Mask: 0x01)               */
/* ======================================================  I2S.TX_CTL  ======================================================= */
#define I2S_TX_CTL_M_CLOCK_DIV_Pos        (0UL)                 /*!< I2S TX_CTL: M_CLOCK_DIV (Bit 0)                           */
#define I2S_TX_CTL_M_CLOCK_DIV_Msk        (0x7UL)               /*!< I2S TX_CTL: M_CLOCK_DIV (Bitfield-Mask: 0x07)             */
#define I2S_TX_CTL_B_CLOCK_INV_Pos        (3UL)                 /*!< I2S TX_CTL: B_CLOCK_INV (Bit 3)                           */
#define I2S_TX_CTL_B_CLOCK_INV_Msk        (0x8UL)               /*!< I2S TX_CTL: B_CLOCK_INV (Bitfield-Mask: 0x01)             */
#define I2S_TX_CTL_CH_NR_Pos              (4UL)                 /*!< I2S TX_CTL: CH_NR (Bit 4)                                 */
#define I2S_TX_CTL_CH_NR_Msk              (0x70UL)              /*!< I2S TX_CTL: CH_NR (Bitfield-Mask: 0x07)                   */
#define I2S_TX_CTL_MS_Pos                 (7UL)                 /*!< I2S TX_CTL: MS (Bit 7)                                    */
#define I2S_TX_CTL_MS_Msk                 (0x80UL)              /*!< I2S TX_CTL: MS (Bitfield-Mask: 0x01)                      */
#define I2S_TX_CTL_I2S_MODE_Pos           (8UL)                 /*!< I2S TX_CTL: I2S_MODE (Bit 8)                              */
#define I2S_TX_CTL_I2S_MODE_Msk           (0x300UL)             /*!< I2S TX_CTL: I2S_MODE (Bitfield-Mask: 0x03)                */
#define I2S_TX_CTL_WS_PULSE_Pos           (10UL)                /*!< I2S TX_CTL: WS_PULSE (Bit 10)                             */
#define I2S_TX_CTL_WS_PULSE_Msk           (0x400UL)             /*!< I2S TX_CTL: WS_PULSE (Bitfield-Mask: 0x01)                */
#define I2S_TX_CTL_OVHDATA_Pos            (12UL)                /*!< I2S TX_CTL: OVHDATA (Bit 12)                              */
#define I2S_TX_CTL_OVHDATA_Msk            (0x1000UL)            /*!< I2S TX_CTL: OVHDATA (Bitfield-Mask: 0x01)                 */
#define I2S_TX_CTL_WD_EN_Pos              (13UL)                /*!< I2S TX_CTL: WD_EN (Bit 13)                                */
#define I2S_TX_CTL_WD_EN_Msk              (0x2000UL)            /*!< I2S TX_CTL: WD_EN (Bitfield-Mask: 0x01)                   */
#define I2S_TX_CTL_CH_LEN_Pos             (16UL)                /*!< I2S TX_CTL: CH_LEN (Bit 16)                               */
#define I2S_TX_CTL_CH_LEN_Msk             (0x70000UL)           /*!< I2S TX_CTL: CH_LEN (Bitfield-Mask: 0x07)                  */
#define I2S_TX_CTL_WORD_LEN_Pos           (20UL)                /*!< I2S TX_CTL: WORD_LEN (Bit 20)                             */
#define I2S_TX_CTL_WORD_LEN_Msk           (0x700000UL)          /*!< I2S TX_CTL: WORD_LEN (Bitfield-Mask: 0x07)                */
/* ====================================================  I2S.TX_WATCHDOG  ==================================================== */
#define I2S_TX_WATCHDOG_WD_COUNTER_Pos    (0UL)                 /*!< I2S TX_WATCHDOG: WD_COUNTER (Bit 0)                       */
#define I2S_TX_WATCHDOG_WD_COUNTER_Msk    (0xffffffffUL)        /*!< I2S TX_WATCHDOG: WD_COUNTER (Bitfield-Mask: 0xffffffff)   */
/* ======================================================  I2S.RX_CTL  ======================================================= */
#define I2S_RX_CTL_M_CLOCK_DIV_Pos        (0UL)                 /*!< I2S RX_CTL: M_CLOCK_DIV (Bit 0)                           */
#define I2S_RX_CTL_M_CLOCK_DIV_Msk        (0x7UL)               /*!< I2S RX_CTL: M_CLOCK_DIV (Bitfield-Mask: 0x07)             */
#define I2S_RX_CTL_B_CLOCK_INV_Pos        (3UL)                 /*!< I2S RX_CTL: B_CLOCK_INV (Bit 3)                           */
#define I2S_RX_CTL_B_CLOCK_INV_Msk        (0x8UL)               /*!< I2S RX_CTL: B_CLOCK_INV (Bitfield-Mask: 0x01)             */
#define I2S_RX_CTL_CH_NR_Pos              (4UL)                 /*!< I2S RX_CTL: CH_NR (Bit 4)                                 */
#define I2S_RX_CTL_CH_NR_Msk              (0x70UL)              /*!< I2S RX_CTL: CH_NR (Bitfield-Mask: 0x07)                   */
#define I2S_RX_CTL_MS_Pos                 (7UL)                 /*!< I2S RX_CTL: MS (Bit 7)                                    */
#define I2S_RX_CTL_MS_Msk                 (0x80UL)              /*!< I2S RX_CTL: MS (Bitfield-Mask: 0x01)                      */
#define I2S_RX_CTL_I2S_MODE_Pos           (8UL)                 /*!< I2S RX_CTL: I2S_MODE (Bit 8)                              */
#define I2S_RX_CTL_I2S_MODE_Msk           (0x300UL)             /*!< I2S RX_CTL: I2S_MODE (Bitfield-Mask: 0x03)                */
#define I2S_RX_CTL_WS_PULSE_Pos           (10UL)                /*!< I2S RX_CTL: WS_PULSE (Bit 10)                             */
#define I2S_RX_CTL_WS_PULSE_Msk           (0x400UL)             /*!< I2S RX_CTL: WS_PULSE (Bitfield-Mask: 0x01)                */
#define I2S_RX_CTL_WD_EN_Pos              (13UL)                /*!< I2S RX_CTL: WD_EN (Bit 13)                                */
#define I2S_RX_CTL_WD_EN_Msk              (0x2000UL)            /*!< I2S RX_CTL: WD_EN (Bitfield-Mask: 0x01)                   */
#define I2S_RX_CTL_CH_LEN_Pos             (16UL)                /*!< I2S RX_CTL: CH_LEN (Bit 16)                               */
#define I2S_RX_CTL_CH_LEN_Msk             (0x70000UL)           /*!< I2S RX_CTL: CH_LEN (Bitfield-Mask: 0x07)                  */
#define I2S_RX_CTL_WORD_LEN_Pos           (20UL)                /*!< I2S RX_CTL: WORD_LEN (Bit 20)                             */
#define I2S_RX_CTL_WORD_LEN_Msk           (0x700000UL)          /*!< I2S RX_CTL: WORD_LEN (Bitfield-Mask: 0x07)                */
#define I2S_RX_CTL_BIT_EXTENSION_Pos      (23UL)                /*!< I2S RX_CTL: BIT_EXTENSION (Bit 23)                        */
#define I2S_RX_CTL_BIT_EXTENSION_Msk      (0x800000UL)          /*!< I2S RX_CTL: BIT_EXTENSION (Bitfield-Mask: 0x01)           */
/* ====================================================  I2S.RX_WATCHDOG  ==================================================== */
#define I2S_RX_WATCHDOG_WD_COUNTER_Pos    (0UL)                 /*!< I2S RX_WATCHDOG: WD_COUNTER (Bit 0)                       */
#define I2S_RX_WATCHDOG_WD_COUNTER_Msk    (0xffffffffUL)        /*!< I2S RX_WATCHDOG: WD_COUNTER (Bitfield-Mask: 0xffffffff)   */
/* ====================================================  I2S.TX_FIFO_CTL  ==================================================== */
#define I2S_TX_FIFO_CTL_TRIGGER_LEVEL_Pos (0UL)                 /*!< I2S TX_FIFO_CTL: TRIGGER_LEVEL (Bit 0)                    */
#define I2S_TX_FIFO_CTL_TRIGGER_LEVEL_Msk (0xffUL)              /*!< I2S TX_FIFO_CTL: TRIGGER_LEVEL (Bitfield-Mask: 0xff)      */
#define I2S_TX_FIFO_CTL_CLEAR_Pos         (16UL)                /*!< I2S TX_FIFO_CTL: CLEAR (Bit 16)                           */
#define I2S_TX_FIFO_CTL_CLEAR_Msk         (0x10000UL)           /*!< I2S TX_FIFO_CTL: CLEAR (Bitfield-Mask: 0x01)              */
#define I2S_TX_FIFO_CTL_FREEZE_Pos        (17UL)                /*!< I2S TX_FIFO_CTL: FREEZE (Bit 17)                          */
#define I2S_TX_FIFO_CTL_FREEZE_Msk        (0x20000UL)           /*!< I2S TX_FIFO_CTL: FREEZE (Bitfield-Mask: 0x01)             */
/* ==================================================  I2S.TX_FIFO_STATUS  =================================================== */
#define I2S_TX_FIFO_STATUS_USED_Pos       (0UL)                 /*!< I2S TX_FIFO_STATUS: USED (Bit 0)                          */
#define I2S_TX_FIFO_STATUS_USED_Msk       (0x1ffUL)             /*!< I2S TX_FIFO_STATUS: USED (Bitfield-Mask: 0x1ff)           */
#define I2S_TX_FIFO_STATUS_RD_PTR_Pos     (16UL)                /*!< I2S TX_FIFO_STATUS: RD_PTR (Bit 16)                       */
#define I2S_TX_FIFO_STATUS_RD_PTR_Msk     (0xff0000UL)          /*!< I2S TX_FIFO_STATUS: RD_PTR (Bitfield-Mask: 0xff)          */
#define I2S_TX_FIFO_STATUS_WR_PTR_Pos     (24UL)                /*!< I2S TX_FIFO_STATUS: WR_PTR (Bit 24)                       */
#define I2S_TX_FIFO_STATUS_WR_PTR_Msk     (0xff000000UL)        /*!< I2S TX_FIFO_STATUS: WR_PTR (Bitfield-Mask: 0xff)          */
/* ====================================================  I2S.TX_FIFO_WR  ===================================================== */
#define I2S_TX_FIFO_WR_DATA_Pos           (0UL)                 /*!< I2S TX_FIFO_WR: DATA (Bit 0)                              */
#define I2S_TX_FIFO_WR_DATA_Msk           (0xffffffffUL)        /*!< I2S TX_FIFO_WR: DATA (Bitfield-Mask: 0xffffffff)          */
/* ====================================================  I2S.RX_FIFO_CTL  ==================================================== */
#define I2S_RX_FIFO_CTL_TRIGGER_LEVEL_Pos (0UL)                 /*!< I2S RX_FIFO_CTL: TRIGGER_LEVEL (Bit 0)                    */
#define I2S_RX_FIFO_CTL_TRIGGER_LEVEL_Msk (0xffUL)              /*!< I2S RX_FIFO_CTL: TRIGGER_LEVEL (Bitfield-Mask: 0xff)      */
#define I2S_RX_FIFO_CTL_CLEAR_Pos         (16UL)                /*!< I2S RX_FIFO_CTL: CLEAR (Bit 16)                           */
#define I2S_RX_FIFO_CTL_CLEAR_Msk         (0x10000UL)           /*!< I2S RX_FIFO_CTL: CLEAR (Bitfield-Mask: 0x01)              */
#define I2S_RX_FIFO_CTL_FREEZE_Pos        (17UL)                /*!< I2S RX_FIFO_CTL: FREEZE (Bit 17)                          */
#define I2S_RX_FIFO_CTL_FREEZE_Msk        (0x20000UL)           /*!< I2S RX_FIFO_CTL: FREEZE (Bitfield-Mask: 0x01)             */
/* ==================================================  I2S.RX_FIFO_STATUS  =================================================== */
#define I2S_RX_FIFO_STATUS_USED_Pos       (0UL)                 /*!< I2S RX_FIFO_STATUS: USED (Bit 0)                          */
#define I2S_RX_FIFO_STATUS_USED_Msk       (0x1ffUL)             /*!< I2S RX_FIFO_STATUS: USED (Bitfield-Mask: 0x1ff)           */
#define I2S_RX_FIFO_STATUS_RD_PTR_Pos     (16UL)                /*!< I2S RX_FIFO_STATUS: RD_PTR (Bit 16)                       */
#define I2S_RX_FIFO_STATUS_RD_PTR_Msk     (0xff0000UL)          /*!< I2S RX_FIFO_STATUS: RD_PTR (Bitfield-Mask: 0xff)          */
#define I2S_RX_FIFO_STATUS_WR_PTR_Pos     (24UL)                /*!< I2S RX_FIFO_STATUS: WR_PTR (Bit 24)                       */
#define I2S_RX_FIFO_STATUS_WR_PTR_Msk     (0xff000000UL)        /*!< I2S RX_FIFO_STATUS: WR_PTR (Bitfield-Mask: 0xff)          */
/* ====================================================  I2S.RX_FIFO_RD  ===================================================== */
#define I2S_RX_FIFO_RD_DATA_Pos           (0UL)                 /*!< I2S RX_FIFO_RD: DATA (Bit 0)                              */
#define I2S_RX_FIFO_RD_DATA_Msk           (0xffffffffUL)        /*!< I2S RX_FIFO_RD: DATA (Bitfield-Mask: 0xffffffff)          */
/* =================================================  I2S.RX_FIFO_RD_SILENT  ================================================= */
#define I2S_RX_FIFO_RD_SILENT_DATA_Pos    (0UL)                 /*!< I2S RX_FIFO_RD_SILENT: DATA (Bit 0)                       */
#define I2S_RX_FIFO_RD_SILENT_DATA_Msk    (0xffffffffUL)        /*!< I2S RX_FIFO_RD_SILENT: DATA (Bitfield-Mask: 0xffffffff)   */
/* =======================================================  I2S.INTR  ======================================================== */
#define I2S_INTR_TX_TRIGGER_Pos           (0UL)                 /*!< I2S INTR: TX_TRIGGER (Bit 0)                              */
#define I2S_INTR_TX_TRIGGER_Msk           (0x1UL)               /*!< I2S INTR: TX_TRIGGER (Bitfield-Mask: 0x01)                */
#define I2S_INTR_TX_NOT_FULL_Pos          (1UL)                 /*!< I2S INTR: TX_NOT_FULL (Bit 1)                             */
#define I2S_INTR_TX_NOT_FULL_Msk          (0x2UL)               /*!< I2S INTR: TX_NOT_FULL (Bitfield-Mask: 0x01)               */
#define I2S_INTR_TX_EMPTY_Pos             (4UL)                 /*!< I2S INTR: TX_EMPTY (Bit 4)                                */
#define I2S_INTR_TX_EMPTY_Msk             (0x10UL)              /*!< I2S INTR: TX_EMPTY (Bitfield-Mask: 0x01)                  */
#define I2S_INTR_TX_OVERFLOW_Pos          (5UL)                 /*!< I2S INTR: TX_OVERFLOW (Bit 5)                             */
#define I2S_INTR_TX_OVERFLOW_Msk          (0x20UL)              /*!< I2S INTR: TX_OVERFLOW (Bitfield-Mask: 0x01)               */
#define I2S_INTR_TX_UNDERFLOW_Pos         (6UL)                 /*!< I2S INTR: TX_UNDERFLOW (Bit 6)                            */
#define I2S_INTR_TX_UNDERFLOW_Msk         (0x40UL)              /*!< I2S INTR: TX_UNDERFLOW (Bitfield-Mask: 0x01)              */
#define I2S_INTR_TX_WD_Pos                (8UL)                 /*!< I2S INTR: TX_WD (Bit 8)                                   */
#define I2S_INTR_TX_WD_Msk                (0x100UL)             /*!< I2S INTR: TX_WD (Bitfield-Mask: 0x01)                     */
#define I2S_INTR_RX_TRIGGER_Pos           (16UL)                /*!< I2S INTR: RX_TRIGGER (Bit 16)                             */
#define I2S_INTR_RX_TRIGGER_Msk           (0x10000UL)           /*!< I2S INTR: RX_TRIGGER (Bitfield-Mask: 0x01)                */
#define I2S_INTR_RX_NOT_EMPTY_Pos         (18UL)                /*!< I2S INTR: RX_NOT_EMPTY (Bit 18)                           */
#define I2S_INTR_RX_NOT_EMPTY_Msk         (0x40000UL)           /*!< I2S INTR: RX_NOT_EMPTY (Bitfield-Mask: 0x01)              */
#define I2S_INTR_RX_FULL_Pos              (19UL)                /*!< I2S INTR: RX_FULL (Bit 19)                                */
#define I2S_INTR_RX_FULL_Msk              (0x80000UL)           /*!< I2S INTR: RX_FULL (Bitfield-Mask: 0x01)                   */
#define I2S_INTR_RX_OVERFLOW_Pos          (21UL)                /*!< I2S INTR: RX_OVERFLOW (Bit 21)                            */
#define I2S_INTR_RX_OVERFLOW_Msk          (0x200000UL)          /*!< I2S INTR: RX_OVERFLOW (Bitfield-Mask: 0x01)               */
#define I2S_INTR_RX_UNDERFLOW_Pos         (22UL)                /*!< I2S INTR: RX_UNDERFLOW (Bit 22)                           */
#define I2S_INTR_RX_UNDERFLOW_Msk         (0x400000UL)          /*!< I2S INTR: RX_UNDERFLOW (Bitfield-Mask: 0x01)              */
#define I2S_INTR_RX_WD_Pos                (24UL)                /*!< I2S INTR: RX_WD (Bit 24)                                  */
#define I2S_INTR_RX_WD_Msk                (0x1000000UL)         /*!< I2S INTR: RX_WD (Bitfield-Mask: 0x01)                     */
/* =====================================================  I2S.INTR_SET  ====================================================== */
#define I2S_INTR_SET_TX_TRIGGER_Pos       (0UL)                 /*!< I2S INTR_SET: TX_TRIGGER (Bit 0)                          */
#define I2S_INTR_SET_TX_TRIGGER_Msk       (0x1UL)               /*!< I2S INTR_SET: TX_TRIGGER (Bitfield-Mask: 0x01)            */
#define I2S_INTR_SET_TX_NOT_FULL_Pos      (1UL)                 /*!< I2S INTR_SET: TX_NOT_FULL (Bit 1)                         */
#define I2S_INTR_SET_TX_NOT_FULL_Msk      (0x2UL)               /*!< I2S INTR_SET: TX_NOT_FULL (Bitfield-Mask: 0x01)           */
#define I2S_INTR_SET_TX_EMPTY_Pos         (4UL)                 /*!< I2S INTR_SET: TX_EMPTY (Bit 4)                            */
#define I2S_INTR_SET_TX_EMPTY_Msk         (0x10UL)              /*!< I2S INTR_SET: TX_EMPTY (Bitfield-Mask: 0x01)              */
#define I2S_INTR_SET_TX_OVERFLOW_Pos      (5UL)                 /*!< I2S INTR_SET: TX_OVERFLOW (Bit 5)                         */
#define I2S_INTR_SET_TX_OVERFLOW_Msk      (0x20UL)              /*!< I2S INTR_SET: TX_OVERFLOW (Bitfield-Mask: 0x01)           */
#define I2S_INTR_SET_TX_UNDERFLOW_Pos     (6UL)                 /*!< I2S INTR_SET: TX_UNDERFLOW (Bit 6)                        */
#define I2S_INTR_SET_TX_UNDERFLOW_Msk     (0x40UL)              /*!< I2S INTR_SET: TX_UNDERFLOW (Bitfield-Mask: 0x01)          */
#define I2S_INTR_SET_TX_WD_Pos            (8UL)                 /*!< I2S INTR_SET: TX_WD (Bit 8)                               */
#define I2S_INTR_SET_TX_WD_Msk            (0x100UL)             /*!< I2S INTR_SET: TX_WD (Bitfield-Mask: 0x01)                 */
#define I2S_INTR_SET_RX_TRIGGER_Pos       (16UL)                /*!< I2S INTR_SET: RX_TRIGGER (Bit 16)                         */
#define I2S_INTR_SET_RX_TRIGGER_Msk       (0x10000UL)           /*!< I2S INTR_SET: RX_TRIGGER (Bitfield-Mask: 0x01)            */
#define I2S_INTR_SET_RX_NOT_EMPTY_Pos     (18UL)                /*!< I2S INTR_SET: RX_NOT_EMPTY (Bit 18)                       */
#define I2S_INTR_SET_RX_NOT_EMPTY_Msk     (0x40000UL)           /*!< I2S INTR_SET: RX_NOT_EMPTY (Bitfield-Mask: 0x01)          */
#define I2S_INTR_SET_RX_FULL_Pos          (19UL)                /*!< I2S INTR_SET: RX_FULL (Bit 19)                            */
#define I2S_INTR_SET_RX_FULL_Msk          (0x80000UL)           /*!< I2S INTR_SET: RX_FULL (Bitfield-Mask: 0x01)               */
#define I2S_INTR_SET_RX_OVERFLOW_Pos      (21UL)                /*!< I2S INTR_SET: RX_OVERFLOW (Bit 21)                        */
#define I2S_INTR_SET_RX_OVERFLOW_Msk      (0x200000UL)          /*!< I2S INTR_SET: RX_OVERFLOW (Bitfield-Mask: 0x01)           */
#define I2S_INTR_SET_RX_UNDERFLOW_Pos     (22UL)                /*!< I2S INTR_SET: RX_UNDERFLOW (Bit 22)                       */
#define I2S_INTR_SET_RX_UNDERFLOW_Msk     (0x400000UL)          /*!< I2S INTR_SET: RX_UNDERFLOW (Bitfield-Mask: 0x01)          */
#define I2S_INTR_SET_RX_WD_Pos            (24UL)                /*!< I2S INTR_SET: RX_WD (Bit 24)                              */
#define I2S_INTR_SET_RX_WD_Msk            (0x1000000UL)         /*!< I2S INTR_SET: RX_WD (Bitfield-Mask: 0x01)                 */
/* =====================================================  I2S.INTR_MASK  ===================================================== */
#define I2S_INTR_MASK_TX_TRIGGER_Pos      (0UL)                 /*!< I2S INTR_MASK: TX_TRIGGER (Bit 0)                         */
#define I2S_INTR_MASK_TX_TRIGGER_Msk      (0x1UL)               /*!< I2S INTR_MASK: TX_TRIGGER (Bitfield-Mask: 0x01)           */
#define I2S_INTR_MASK_TX_NOT_FULL_Pos     (1UL)                 /*!< I2S INTR_MASK: TX_NOT_FULL (Bit 1)                        */
#define I2S_INTR_MASK_TX_NOT_FULL_Msk     (0x2UL)               /*!< I2S INTR_MASK: TX_NOT_FULL (Bitfield-Mask: 0x01)          */
#define I2S_INTR_MASK_TX_EMPTY_Pos        (4UL)                 /*!< I2S INTR_MASK: TX_EMPTY (Bit 4)                           */
#define I2S_INTR_MASK_TX_EMPTY_Msk        (0x10UL)              /*!< I2S INTR_MASK: TX_EMPTY (Bitfield-Mask: 0x01)             */
#define I2S_INTR_MASK_TX_OVERFLOW_Pos     (5UL)                 /*!< I2S INTR_MASK: TX_OVERFLOW (Bit 5)                        */
#define I2S_INTR_MASK_TX_OVERFLOW_Msk     (0x20UL)              /*!< I2S INTR_MASK: TX_OVERFLOW (Bitfield-Mask: 0x01)          */
#define I2S_INTR_MASK_TX_UNDERFLOW_Pos    (6UL)                 /*!< I2S INTR_MASK: TX_UNDERFLOW (Bit 6)                       */
#define I2S_INTR_MASK_TX_UNDERFLOW_Msk    (0x40UL)              /*!< I2S INTR_MASK: TX_UNDERFLOW (Bitfield-Mask: 0x01)         */
#define I2S_INTR_MASK_TX_WD_Pos           (8UL)                 /*!< I2S INTR_MASK: TX_WD (Bit 8)                              */
#define I2S_INTR_MASK_TX_WD_Msk           (0x100UL)             /*!< I2S INTR_MASK: TX_WD (Bitfield-Mask: 0x01)                */
#define I2S_INTR_MASK_RX_TRIGGER_Pos      (16UL)                /*!< I2S INTR_MASK: RX_TRIGGER (Bit 16)                        */
#define I2S_INTR_MASK_RX_TRIGGER_Msk      (0x10000UL)           /*!< I2S INTR_MASK: RX_TRIGGER (Bitfield-Mask: 0x01)           */
#define I2S_INTR_MASK_RX_NOT_EMPTY_Pos    (18UL)                /*!< I2S INTR_MASK: RX_NOT_EMPTY (Bit 18)                      */
#define I2S_INTR_MASK_RX_NOT_EMPTY_Msk    (0x40000UL)           /*!< I2S INTR_MASK: RX_NOT_EMPTY (Bitfield-Mask: 0x01)         */
#define I2S_INTR_MASK_RX_FULL_Pos         (19UL)                /*!< I2S INTR_MASK: RX_FULL (Bit 19)                           */
#define I2S_INTR_MASK_RX_FULL_Msk         (0x80000UL)           /*!< I2S INTR_MASK: RX_FULL (Bitfield-Mask: 0x01)              */
#define I2S_INTR_MASK_RX_OVERFLOW_Pos     (21UL)                /*!< I2S INTR_MASK: RX_OVERFLOW (Bit 21)                       */
#define I2S_INTR_MASK_RX_OVERFLOW_Msk     (0x200000UL)          /*!< I2S INTR_MASK: RX_OVERFLOW (Bitfield-Mask: 0x01)          */
#define I2S_INTR_MASK_RX_UNDERFLOW_Pos    (22UL)                /*!< I2S INTR_MASK: RX_UNDERFLOW (Bit 22)                      */
#define I2S_INTR_MASK_RX_UNDERFLOW_Msk    (0x400000UL)          /*!< I2S INTR_MASK: RX_UNDERFLOW (Bitfield-Mask: 0x01)         */
#define I2S_INTR_MASK_RX_WD_Pos           (24UL)                /*!< I2S INTR_MASK: RX_WD (Bit 24)                             */
#define I2S_INTR_MASK_RX_WD_Msk           (0x1000000UL)         /*!< I2S INTR_MASK: RX_WD (Bitfield-Mask: 0x01)                */
/* ====================================================  I2S.INTR_MASKED  ==================================================== */
#define I2S_INTR_MASKED_TX_TRIGGER_Pos    (0UL)                 /*!< I2S INTR_MASKED: TX_TRIGGER (Bit 0)                       */
#define I2S_INTR_MASKED_TX_TRIGGER_Msk    (0x1UL)               /*!< I2S INTR_MASKED: TX_TRIGGER (Bitfield-Mask: 0x01)         */
#define I2S_INTR_MASKED_TX_NOT_FULL_Pos   (1UL)                 /*!< I2S INTR_MASKED: TX_NOT_FULL (Bit 1)                      */
#define I2S_INTR_MASKED_TX_NOT_FULL_Msk   (0x2UL)               /*!< I2S INTR_MASKED: TX_NOT_FULL (Bitfield-Mask: 0x01)        */
#define I2S_INTR_MASKED_TX_EMPTY_Pos      (4UL)                 /*!< I2S INTR_MASKED: TX_EMPTY (Bit 4)                         */
#define I2S_INTR_MASKED_TX_EMPTY_Msk      (0x10UL)              /*!< I2S INTR_MASKED: TX_EMPTY (Bitfield-Mask: 0x01)           */
#define I2S_INTR_MASKED_TX_OVERFLOW_Pos   (5UL)                 /*!< I2S INTR_MASKED: TX_OVERFLOW (Bit 5)                      */
#define I2S_INTR_MASKED_TX_OVERFLOW_Msk   (0x20UL)              /*!< I2S INTR_MASKED: TX_OVERFLOW (Bitfield-Mask: 0x01)        */
#define I2S_INTR_MASKED_TX_UNDERFLOW_Pos  (6UL)                 /*!< I2S INTR_MASKED: TX_UNDERFLOW (Bit 6)                     */
#define I2S_INTR_MASKED_TX_UNDERFLOW_Msk  (0x40UL)              /*!< I2S INTR_MASKED: TX_UNDERFLOW (Bitfield-Mask: 0x01)       */
#define I2S_INTR_MASKED_TX_WD_Pos         (8UL)                 /*!< I2S INTR_MASKED: TX_WD (Bit 8)                            */
#define I2S_INTR_MASKED_TX_WD_Msk         (0x100UL)             /*!< I2S INTR_MASKED: TX_WD (Bitfield-Mask: 0x01)              */
#define I2S_INTR_MASKED_RX_TRIGGER_Pos    (16UL)                /*!< I2S INTR_MASKED: RX_TRIGGER (Bit 16)                      */
#define I2S_INTR_MASKED_RX_TRIGGER_Msk    (0x10000UL)           /*!< I2S INTR_MASKED: RX_TRIGGER (Bitfield-Mask: 0x01)         */
#define I2S_INTR_MASKED_RX_NOT_EMPTY_Pos  (18UL)                /*!< I2S INTR_MASKED: RX_NOT_EMPTY (Bit 18)                    */
#define I2S_INTR_MASKED_RX_NOT_EMPTY_Msk  (0x40000UL)           /*!< I2S INTR_MASKED: RX_NOT_EMPTY (Bitfield-Mask: 0x01)       */
#define I2S_INTR_MASKED_RX_FULL_Pos       (19UL)                /*!< I2S INTR_MASKED: RX_FULL (Bit 19)                         */
#define I2S_INTR_MASKED_RX_FULL_Msk       (0x80000UL)           /*!< I2S INTR_MASKED: RX_FULL (Bitfield-Mask: 0x01)            */
#define I2S_INTR_MASKED_RX_OVERFLOW_Pos   (21UL)                /*!< I2S INTR_MASKED: RX_OVERFLOW (Bit 21)                     */
#define I2S_INTR_MASKED_RX_OVERFLOW_Msk   (0x200000UL)          /*!< I2S INTR_MASKED: RX_OVERFLOW (Bitfield-Mask: 0x01)        */
#define I2S_INTR_MASKED_RX_UNDERFLOW_Pos  (22UL)                /*!< I2S INTR_MASKED: RX_UNDERFLOW (Bit 22)                    */
#define I2S_INTR_MASKED_RX_UNDERFLOW_Msk  (0x400000UL)          /*!< I2S INTR_MASKED: RX_UNDERFLOW (Bitfield-Mask: 0x01)       */
#define I2S_INTR_MASKED_RX_WD_Pos         (24UL)                /*!< I2S INTR_MASKED: RX_WD (Bit 24)                           */
#define I2S_INTR_MASKED_RX_WD_Msk         (0x1000000UL)         /*!< I2S INTR_MASKED: RX_WD (Bitfield-Mask: 0x01)              */


#endif /* _CYIP_I2S_H_ */


/* [] END OF FILE */
