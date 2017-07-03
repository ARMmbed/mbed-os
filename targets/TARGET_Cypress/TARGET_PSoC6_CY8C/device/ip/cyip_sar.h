/***************************************************************************//**
* \file cyip_sar.h
*
* \brief
* SAR IP definitions
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

#ifndef _CYIP_SAR_H_
#define _CYIP_SAR_H_

#include "cyip_headers.h"

/* =========================================================================================================================== */
/* ================                                            SAR                                            ================ */
/* =========================================================================================================================== */

#define SAR_SECTION_SIZE            0x00010000UL

/**
  * \brief SAR ADC with Sequencer (SAR)
  */
typedef struct {                                /*!< SAR Structure                                                             */
  __IOM uint32_t CTRL;                          /*!< 0x00000000 Analog control register.                                       */
  __IOM uint32_t SAMPLE_CTRL;                   /*!< 0x00000004 Sample control register.                                       */
   __IM uint32_t RESERVED[2];
  __IOM uint32_t SAMPLE_TIME01;                 /*!< 0x00000010 Sample time specification ST0 and ST1                          */
  __IOM uint32_t SAMPLE_TIME23;                 /*!< 0x00000014 Sample time specification ST2 and ST3                          */
  __IOM uint32_t RANGE_THRES;                   /*!< 0x00000018 Global range detect threshold register.                        */
  __IOM uint32_t RANGE_COND;                    /*!< 0x0000001C Global range detect mode register.                             */
  __IOM uint32_t CHAN_EN;                       /*!< 0x00000020 Enable bits for the channels                                   */
  __IOM uint32_t START_CTRL;                    /*!< 0x00000024 Start control register (firmware trigger).                     */
   __IM uint32_t RESERVED1[22];
  __IOM uint32_t CHAN_CONFIG[16];               /*!< 0x00000080 Channel configuration register.                                */
   __IM uint32_t RESERVED2[16];
   __IM uint32_t CHAN_WORK[16];                 /*!< 0x00000100 Channel working data register                                  */
   __IM uint32_t RESERVED3[16];
   __IM uint32_t CHAN_RESULT[16];               /*!< 0x00000180 Channel result data register                                   */
   __IM uint32_t RESERVED4[16];
   __IM uint32_t CHAN_WORK_UPDATED;             /*!< 0x00000200 Channel working data register 'updated' bits                   */
   __IM uint32_t CHAN_RESULT_UPDATED;           /*!< 0x00000204 Channel result data register 'updated' bits                    */
   __IM uint32_t CHAN_WORK_NEWVALUE;            /*!< 0x00000208 Channel working data register 'new value' bits                 */
   __IM uint32_t CHAN_RESULT_NEWVALUE;          /*!< 0x0000020C Channel result data register 'new value' bits                  */
  __IOM uint32_t INTR;                          /*!< 0x00000210 Interrupt request register.                                    */
  __IOM uint32_t INTR_SET;                      /*!< 0x00000214 Interrupt set request register                                 */
  __IOM uint32_t INTR_MASK;                     /*!< 0x00000218 Interrupt mask register.                                       */
   __IM uint32_t INTR_MASKED;                   /*!< 0x0000021C Interrupt masked request register                              */
  __IOM uint32_t SATURATE_INTR;                 /*!< 0x00000220 Saturate interrupt request register.                           */
  __IOM uint32_t SATURATE_INTR_SET;             /*!< 0x00000224 Saturate interrupt set request register                        */
  __IOM uint32_t SATURATE_INTR_MASK;            /*!< 0x00000228 Saturate interrupt mask register.                              */
   __IM uint32_t SATURATE_INTR_MASKED;          /*!< 0x0000022C Saturate interrupt masked request register                     */
  __IOM uint32_t RANGE_INTR;                    /*!< 0x00000230 Range detect interrupt request register.                       */
  __IOM uint32_t RANGE_INTR_SET;                /*!< 0x00000234 Range detect interrupt set request register                    */
  __IOM uint32_t RANGE_INTR_MASK;               /*!< 0x00000238 Range detect interrupt mask register.                          */
   __IM uint32_t RANGE_INTR_MASKED;             /*!< 0x0000023C Range interrupt masked request register                        */
   __IM uint32_t INTR_CAUSE;                    /*!< 0x00000240 Interrupt cause register                                       */
   __IM uint32_t RESERVED5[15];
  __IOM uint32_t INJ_CHAN_CONFIG;               /*!< 0x00000280 Injection channel configuration register.                      */
   __IM uint32_t RESERVED6[3];
   __IM uint32_t INJ_RESULT;                    /*!< 0x00000290 Injection channel result register                              */
   __IM uint32_t RESERVED7[3];
   __IM uint32_t STATUS;                        /*!< 0x000002A0 Current status of internal SAR registers (mostly for debug)    */
   __IM uint32_t AVG_STAT;                      /*!< 0x000002A4 Current averaging status (for debug)                           */
   __IM uint32_t RESERVED8[22];
  __IOM uint32_t MUX_SWITCH0;                   /*!< 0x00000300 SARMUX Firmware switch controls                                */
  __IOM uint32_t MUX_SWITCH_CLEAR0;             /*!< 0x00000304 SARMUX Firmware switch control clear                           */
   __IM uint32_t RESERVED9[14];
  __IOM uint32_t MUX_SWITCH_DS_CTRL;            /*!< 0x00000340 SARMUX switch DSI control                                      */
  __IOM uint32_t MUX_SWITCH_SQ_CTRL;            /*!< 0x00000344 SARMUX switch Sar Sequencer control                            */
   __IM uint32_t MUX_SWITCH_STATUS;             /*!< 0x00000348 SARMUX switch status                                           */
   __IM uint32_t RESERVED10[749];
  __IOM uint32_t ANA_TRIM0;                     /*!< 0x00000F00 Analog trim register.                                          */
  __IOM uint32_t ANA_TRIM1;                     /*!< 0x00000F04 Analog trim register.                                          */
} SAR_Type;                                     /*!< Size = 3848 (0xF08)                                                       */


/* =======================================================  SAR.CTRL  ======================================================== */
#define SAR_CTRL_PWR_CTRL_VREF_Pos        (0UL)                 /*!< SAR CTRL: PWR_CTRL_VREF (Bit 0)                           */
#define SAR_CTRL_PWR_CTRL_VREF_Msk        (0x7UL)               /*!< SAR CTRL: PWR_CTRL_VREF (Bitfield-Mask: 0x07)             */
#define SAR_CTRL_VREF_SEL_Pos             (4UL)                 /*!< SAR CTRL: VREF_SEL (Bit 4)                                */
#define SAR_CTRL_VREF_SEL_Msk             (0x70UL)              /*!< SAR CTRL: VREF_SEL (Bitfield-Mask: 0x07)                  */
#define SAR_CTRL_VREF_BYP_CAP_EN_Pos      (7UL)                 /*!< SAR CTRL: VREF_BYP_CAP_EN (Bit 7)                         */
#define SAR_CTRL_VREF_BYP_CAP_EN_Msk      (0x80UL)              /*!< SAR CTRL: VREF_BYP_CAP_EN (Bitfield-Mask: 0x01)           */
#define SAR_CTRL_NEG_SEL_Pos              (9UL)                 /*!< SAR CTRL: NEG_SEL (Bit 9)                                 */
#define SAR_CTRL_NEG_SEL_Msk              (0xe00UL)             /*!< SAR CTRL: NEG_SEL (Bitfield-Mask: 0x07)                   */
#define SAR_CTRL_SAR_HW_CTRL_NEGVREF_Pos  (13UL)                /*!< SAR CTRL: SAR_HW_CTRL_NEGVREF (Bit 13)                    */
#define SAR_CTRL_SAR_HW_CTRL_NEGVREF_Msk  (0x2000UL)            /*!< SAR CTRL: SAR_HW_CTRL_NEGVREF (Bitfield-Mask: 0x01)       */
#define SAR_CTRL_COMP_DLY_Pos             (14UL)                /*!< SAR CTRL: COMP_DLY (Bit 14)                               */
#define SAR_CTRL_COMP_DLY_Msk             (0xc000UL)            /*!< SAR CTRL: COMP_DLY (Bitfield-Mask: 0x03)                  */
#define SAR_CTRL_SPARE_Pos                (16UL)                /*!< SAR CTRL: SPARE (Bit 16)                                  */
#define SAR_CTRL_SPARE_Msk                (0xf0000UL)           /*!< SAR CTRL: SPARE (Bitfield-Mask: 0x0f)                     */
#define SAR_CTRL_BOOSTPUMP_EN_Pos         (20UL)                /*!< SAR CTRL: BOOSTPUMP_EN (Bit 20)                           */
#define SAR_CTRL_BOOSTPUMP_EN_Msk         (0x100000UL)          /*!< SAR CTRL: BOOSTPUMP_EN (Bitfield-Mask: 0x01)              */
#define SAR_CTRL_REFBUF_EN_Pos            (21UL)                /*!< SAR CTRL: REFBUF_EN (Bit 21)                              */
#define SAR_CTRL_REFBUF_EN_Msk            (0x200000UL)          /*!< SAR CTRL: REFBUF_EN (Bitfield-Mask: 0x01)                 */
#define SAR_CTRL_COMP_PWR_Pos             (24UL)                /*!< SAR CTRL: COMP_PWR (Bit 24)                               */
#define SAR_CTRL_COMP_PWR_Msk             (0x7000000UL)         /*!< SAR CTRL: COMP_PWR (Bitfield-Mask: 0x07)                  */
#define SAR_CTRL_DEEPSLEEP_ON_Pos         (27UL)                /*!< SAR CTRL: DEEPSLEEP_ON (Bit 27)                           */
#define SAR_CTRL_DEEPSLEEP_ON_Msk         (0x8000000UL)         /*!< SAR CTRL: DEEPSLEEP_ON (Bitfield-Mask: 0x01)              */
#define SAR_CTRL_DSI_SYNC_CONFIG_Pos      (28UL)                /*!< SAR CTRL: DSI_SYNC_CONFIG (Bit 28)                        */
#define SAR_CTRL_DSI_SYNC_CONFIG_Msk      (0x10000000UL)        /*!< SAR CTRL: DSI_SYNC_CONFIG (Bitfield-Mask: 0x01)           */
#define SAR_CTRL_DSI_MODE_Pos             (29UL)                /*!< SAR CTRL: DSI_MODE (Bit 29)                               */
#define SAR_CTRL_DSI_MODE_Msk             (0x20000000UL)        /*!< SAR CTRL: DSI_MODE (Bitfield-Mask: 0x01)                  */
#define SAR_CTRL_SWITCH_DISABLE_Pos       (30UL)                /*!< SAR CTRL: SWITCH_DISABLE (Bit 30)                         */
#define SAR_CTRL_SWITCH_DISABLE_Msk       (0x40000000UL)        /*!< SAR CTRL: SWITCH_DISABLE (Bitfield-Mask: 0x01)            */
#define SAR_CTRL_ENABLED_Pos              (31UL)                /*!< SAR CTRL: ENABLED (Bit 31)                                */
#define SAR_CTRL_ENABLED_Msk              (0x80000000UL)        /*!< SAR CTRL: ENABLED (Bitfield-Mask: 0x01)                   */
/* ====================================================  SAR.SAMPLE_CTRL  ==================================================== */
#define SAR_SAMPLE_CTRL_LEFT_ALIGN_Pos    (1UL)                 /*!< SAR SAMPLE_CTRL: LEFT_ALIGN (Bit 1)                       */
#define SAR_SAMPLE_CTRL_LEFT_ALIGN_Msk    (0x2UL)               /*!< SAR SAMPLE_CTRL: LEFT_ALIGN (Bitfield-Mask: 0x01)         */
#define SAR_SAMPLE_CTRL_SINGLE_ENDED_SIGNED_Pos (2UL)           /*!< SAR SAMPLE_CTRL: SINGLE_ENDED_SIGNED (Bit 2)              */
#define SAR_SAMPLE_CTRL_SINGLE_ENDED_SIGNED_Msk (0x4UL)         /*!< SAR SAMPLE_CTRL: SINGLE_ENDED_SIGNED (Bitfield-Mask: 0x01)*/
#define SAR_SAMPLE_CTRL_DIFFERENTIAL_SIGNED_Pos (3UL)           /*!< SAR SAMPLE_CTRL: DIFFERENTIAL_SIGNED (Bit 3)              */
#define SAR_SAMPLE_CTRL_DIFFERENTIAL_SIGNED_Msk (0x8UL)         /*!< SAR SAMPLE_CTRL: DIFFERENTIAL_SIGNED (Bitfield-Mask: 0x01)*/
#define SAR_SAMPLE_CTRL_AVG_CNT_Pos       (4UL)                 /*!< SAR SAMPLE_CTRL: AVG_CNT (Bit 4)                          */
#define SAR_SAMPLE_CTRL_AVG_CNT_Msk       (0x70UL)              /*!< SAR SAMPLE_CTRL: AVG_CNT (Bitfield-Mask: 0x07)            */
#define SAR_SAMPLE_CTRL_AVG_SHIFT_Pos     (7UL)                 /*!< SAR SAMPLE_CTRL: AVG_SHIFT (Bit 7)                        */
#define SAR_SAMPLE_CTRL_AVG_SHIFT_Msk     (0x80UL)              /*!< SAR SAMPLE_CTRL: AVG_SHIFT (Bitfield-Mask: 0x01)          */
#define SAR_SAMPLE_CTRL_AVG_MODE_Pos      (8UL)                 /*!< SAR SAMPLE_CTRL: AVG_MODE (Bit 8)                         */
#define SAR_SAMPLE_CTRL_AVG_MODE_Msk      (0x100UL)             /*!< SAR SAMPLE_CTRL: AVG_MODE (Bitfield-Mask: 0x01)           */
#define SAR_SAMPLE_CTRL_CONTINUOUS_Pos    (16UL)                /*!< SAR SAMPLE_CTRL: CONTINUOUS (Bit 16)                      */
#define SAR_SAMPLE_CTRL_CONTINUOUS_Msk    (0x10000UL)           /*!< SAR SAMPLE_CTRL: CONTINUOUS (Bitfield-Mask: 0x01)         */
#define SAR_SAMPLE_CTRL_DSI_TRIGGER_EN_Pos (17UL)               /*!< SAR SAMPLE_CTRL: DSI_TRIGGER_EN (Bit 17)                  */
#define SAR_SAMPLE_CTRL_DSI_TRIGGER_EN_Msk (0x20000UL)          /*!< SAR SAMPLE_CTRL: DSI_TRIGGER_EN (Bitfield-Mask: 0x01)     */
#define SAR_SAMPLE_CTRL_DSI_TRIGGER_LEVEL_Pos (18UL)            /*!< SAR SAMPLE_CTRL: DSI_TRIGGER_LEVEL (Bit 18)               */
#define SAR_SAMPLE_CTRL_DSI_TRIGGER_LEVEL_Msk (0x40000UL)       /*!< SAR SAMPLE_CTRL: DSI_TRIGGER_LEVEL (Bitfield-Mask: 0x01)  */
#define SAR_SAMPLE_CTRL_DSI_SYNC_TRIGGER_Pos (19UL)             /*!< SAR SAMPLE_CTRL: DSI_SYNC_TRIGGER (Bit 19)                */
#define SAR_SAMPLE_CTRL_DSI_SYNC_TRIGGER_Msk (0x80000UL)        /*!< SAR SAMPLE_CTRL: DSI_SYNC_TRIGGER (Bitfield-Mask: 0x01)   */
#define SAR_SAMPLE_CTRL_UAB_SCAN_MODE_Pos (22UL)                /*!< SAR SAMPLE_CTRL: UAB_SCAN_MODE (Bit 22)                   */
#define SAR_SAMPLE_CTRL_UAB_SCAN_MODE_Msk (0x400000UL)          /*!< SAR SAMPLE_CTRL: UAB_SCAN_MODE (Bitfield-Mask: 0x01)      */
#define SAR_SAMPLE_CTRL_REPEAT_INVALID_Pos (23UL)               /*!< SAR SAMPLE_CTRL: REPEAT_INVALID (Bit 23)                  */
#define SAR_SAMPLE_CTRL_REPEAT_INVALID_Msk (0x800000UL)         /*!< SAR SAMPLE_CTRL: REPEAT_INVALID (Bitfield-Mask: 0x01)     */
#define SAR_SAMPLE_CTRL_VALID_SEL_Pos     (24UL)                /*!< SAR SAMPLE_CTRL: VALID_SEL (Bit 24)                       */
#define SAR_SAMPLE_CTRL_VALID_SEL_Msk     (0x7000000UL)         /*!< SAR SAMPLE_CTRL: VALID_SEL (Bitfield-Mask: 0x07)          */
#define SAR_SAMPLE_CTRL_VALID_SEL_EN_Pos  (27UL)                /*!< SAR SAMPLE_CTRL: VALID_SEL_EN (Bit 27)                    */
#define SAR_SAMPLE_CTRL_VALID_SEL_EN_Msk  (0x8000000UL)         /*!< SAR SAMPLE_CTRL: VALID_SEL_EN (Bitfield-Mask: 0x01)       */
#define SAR_SAMPLE_CTRL_VALID_IGNORE_Pos  (28UL)                /*!< SAR SAMPLE_CTRL: VALID_IGNORE (Bit 28)                    */
#define SAR_SAMPLE_CTRL_VALID_IGNORE_Msk  (0x10000000UL)        /*!< SAR SAMPLE_CTRL: VALID_IGNORE (Bitfield-Mask: 0x01)       */
#define SAR_SAMPLE_CTRL_TRIGGER_OUT_EN_Pos (30UL)               /*!< SAR SAMPLE_CTRL: TRIGGER_OUT_EN (Bit 30)                  */
#define SAR_SAMPLE_CTRL_TRIGGER_OUT_EN_Msk (0x40000000UL)       /*!< SAR SAMPLE_CTRL: TRIGGER_OUT_EN (Bitfield-Mask: 0x01)     */
#define SAR_SAMPLE_CTRL_EOS_DSI_OUT_EN_Pos (31UL)               /*!< SAR SAMPLE_CTRL: EOS_DSI_OUT_EN (Bit 31)                  */
#define SAR_SAMPLE_CTRL_EOS_DSI_OUT_EN_Msk (0x80000000UL)       /*!< SAR SAMPLE_CTRL: EOS_DSI_OUT_EN (Bitfield-Mask: 0x01)     */
/* ===================================================  SAR.SAMPLE_TIME01  =================================================== */
#define SAR_SAMPLE_TIME01_SAMPLE_TIME0_Pos (0UL)                /*!< SAR SAMPLE_TIME01: SAMPLE_TIME0 (Bit 0)                   */
#define SAR_SAMPLE_TIME01_SAMPLE_TIME0_Msk (0x3ffUL)            /*!< SAR SAMPLE_TIME01: SAMPLE_TIME0 (Bitfield-Mask: 0x3ff)    */
#define SAR_SAMPLE_TIME01_SAMPLE_TIME1_Pos (16UL)               /*!< SAR SAMPLE_TIME01: SAMPLE_TIME1 (Bit 16)                  */
#define SAR_SAMPLE_TIME01_SAMPLE_TIME1_Msk (0x3ff0000UL)        /*!< SAR SAMPLE_TIME01: SAMPLE_TIME1 (Bitfield-Mask: 0x3ff)    */
/* ===================================================  SAR.SAMPLE_TIME23  =================================================== */
#define SAR_SAMPLE_TIME23_SAMPLE_TIME2_Pos (0UL)                /*!< SAR SAMPLE_TIME23: SAMPLE_TIME2 (Bit 0)                   */
#define SAR_SAMPLE_TIME23_SAMPLE_TIME2_Msk (0x3ffUL)            /*!< SAR SAMPLE_TIME23: SAMPLE_TIME2 (Bitfield-Mask: 0x3ff)    */
#define SAR_SAMPLE_TIME23_SAMPLE_TIME3_Pos (16UL)               /*!< SAR SAMPLE_TIME23: SAMPLE_TIME3 (Bit 16)                  */
#define SAR_SAMPLE_TIME23_SAMPLE_TIME3_Msk (0x3ff0000UL)        /*!< SAR SAMPLE_TIME23: SAMPLE_TIME3 (Bitfield-Mask: 0x3ff)    */
/* ====================================================  SAR.RANGE_THRES  ==================================================== */
#define SAR_RANGE_THRES_RANGE_LOW_Pos     (0UL)                 /*!< SAR RANGE_THRES: RANGE_LOW (Bit 0)                        */
#define SAR_RANGE_THRES_RANGE_LOW_Msk     (0xffffUL)            /*!< SAR RANGE_THRES: RANGE_LOW (Bitfield-Mask: 0xffff)        */
#define SAR_RANGE_THRES_RANGE_HIGH_Pos    (16UL)                /*!< SAR RANGE_THRES: RANGE_HIGH (Bit 16)                      */
#define SAR_RANGE_THRES_RANGE_HIGH_Msk    (0xffff0000UL)        /*!< SAR RANGE_THRES: RANGE_HIGH (Bitfield-Mask: 0xffff)       */
/* ====================================================  SAR.RANGE_COND  ===================================================== */
#define SAR_RANGE_COND_RANGE_COND_Pos     (30UL)                /*!< SAR RANGE_COND: RANGE_COND (Bit 30)                       */
#define SAR_RANGE_COND_RANGE_COND_Msk     (0xc0000000UL)        /*!< SAR RANGE_COND: RANGE_COND (Bitfield-Mask: 0x03)          */
/* ======================================================  SAR.CHAN_EN  ====================================================== */
#define SAR_CHAN_EN_CHAN_EN_Pos           (0UL)                 /*!< SAR CHAN_EN: CHAN_EN (Bit 0)                              */
#define SAR_CHAN_EN_CHAN_EN_Msk           (0xffffUL)            /*!< SAR CHAN_EN: CHAN_EN (Bitfield-Mask: 0xffff)              */
/* ====================================================  SAR.START_CTRL  ===================================================== */
#define SAR_START_CTRL_FW_TRIGGER_Pos     (0UL)                 /*!< SAR START_CTRL: FW_TRIGGER (Bit 0)                        */
#define SAR_START_CTRL_FW_TRIGGER_Msk     (0x1UL)               /*!< SAR START_CTRL: FW_TRIGGER (Bitfield-Mask: 0x01)          */
/* ====================================================  SAR.CHAN_CONFIG  ==================================================== */
#define SAR_CHAN_CONFIG_POS_PIN_ADDR_Pos  (0UL)                 /*!< SAR CHAN_CONFIG: POS_PIN_ADDR (Bit 0)                     */
#define SAR_CHAN_CONFIG_POS_PIN_ADDR_Msk  (0x7UL)               /*!< SAR CHAN_CONFIG: POS_PIN_ADDR (Bitfield-Mask: 0x07)       */
#define SAR_CHAN_CONFIG_POS_PORT_ADDR_Pos (4UL)                 /*!< SAR CHAN_CONFIG: POS_PORT_ADDR (Bit 4)                    */
#define SAR_CHAN_CONFIG_POS_PORT_ADDR_Msk (0x70UL)              /*!< SAR CHAN_CONFIG: POS_PORT_ADDR (Bitfield-Mask: 0x07)      */
#define SAR_CHAN_CONFIG_DIFFERENTIAL_EN_Pos (8UL)               /*!< SAR CHAN_CONFIG: DIFFERENTIAL_EN (Bit 8)                  */
#define SAR_CHAN_CONFIG_DIFFERENTIAL_EN_Msk (0x100UL)           /*!< SAR CHAN_CONFIG: DIFFERENTIAL_EN (Bitfield-Mask: 0x01)    */
#define SAR_CHAN_CONFIG_AVG_EN_Pos        (10UL)                /*!< SAR CHAN_CONFIG: AVG_EN (Bit 10)                          */
#define SAR_CHAN_CONFIG_AVG_EN_Msk        (0x400UL)             /*!< SAR CHAN_CONFIG: AVG_EN (Bitfield-Mask: 0x01)             */
#define SAR_CHAN_CONFIG_SAMPLE_TIME_SEL_Pos (12UL)              /*!< SAR CHAN_CONFIG: SAMPLE_TIME_SEL (Bit 12)                 */
#define SAR_CHAN_CONFIG_SAMPLE_TIME_SEL_Msk (0x3000UL)          /*!< SAR CHAN_CONFIG: SAMPLE_TIME_SEL (Bitfield-Mask: 0x03)    */
#define SAR_CHAN_CONFIG_NEG_PIN_ADDR_Pos  (16UL)                /*!< SAR CHAN_CONFIG: NEG_PIN_ADDR (Bit 16)                    */
#define SAR_CHAN_CONFIG_NEG_PIN_ADDR_Msk  (0x70000UL)           /*!< SAR CHAN_CONFIG: NEG_PIN_ADDR (Bitfield-Mask: 0x07)       */
#define SAR_CHAN_CONFIG_NEG_PORT_ADDR_Pos (20UL)                /*!< SAR CHAN_CONFIG: NEG_PORT_ADDR (Bit 20)                   */
#define SAR_CHAN_CONFIG_NEG_PORT_ADDR_Msk (0x700000UL)          /*!< SAR CHAN_CONFIG: NEG_PORT_ADDR (Bitfield-Mask: 0x07)      */
#define SAR_CHAN_CONFIG_NEG_ADDR_EN_Pos   (24UL)                /*!< SAR CHAN_CONFIG: NEG_ADDR_EN (Bit 24)                     */
#define SAR_CHAN_CONFIG_NEG_ADDR_EN_Msk   (0x1000000UL)         /*!< SAR CHAN_CONFIG: NEG_ADDR_EN (Bitfield-Mask: 0x01)        */
#define SAR_CHAN_CONFIG_DSI_OUT_EN_Pos    (31UL)                /*!< SAR CHAN_CONFIG: DSI_OUT_EN (Bit 31)                      */
#define SAR_CHAN_CONFIG_DSI_OUT_EN_Msk    (0x80000000UL)        /*!< SAR CHAN_CONFIG: DSI_OUT_EN (Bitfield-Mask: 0x01)         */
/* =====================================================  SAR.CHAN_WORK  ===================================================== */
#define SAR_CHAN_WORK_WORK_Pos            (0UL)                 /*!< SAR CHAN_WORK: WORK (Bit 0)                               */
#define SAR_CHAN_WORK_WORK_Msk            (0xffffUL)            /*!< SAR CHAN_WORK: WORK (Bitfield-Mask: 0xffff)               */
#define SAR_CHAN_WORK_CHAN_WORK_NEWVALUE_MIR_Pos (27UL)         /*!< SAR CHAN_WORK: CHAN_WORK_NEWVALUE_MIR (Bit 27)            */
#define SAR_CHAN_WORK_CHAN_WORK_NEWVALUE_MIR_Msk (0x8000000UL)  /*!< SAR CHAN_WORK: CHAN_WORK_NEWVALUE_MIR (Bitfield-Mask: 0x01)*/
#define SAR_CHAN_WORK_CHAN_WORK_UPDATED_MIR_Pos (31UL)          /*!< SAR CHAN_WORK: CHAN_WORK_UPDATED_MIR (Bit 31)             */
#define SAR_CHAN_WORK_CHAN_WORK_UPDATED_MIR_Msk (0x80000000UL)  /*!< SAR CHAN_WORK: CHAN_WORK_UPDATED_MIR (Bitfield-Mask: 0x01)*/
/* ====================================================  SAR.CHAN_RESULT  ==================================================== */
#define SAR_CHAN_RESULT_RESULT_Pos        (0UL)                 /*!< SAR CHAN_RESULT: RESULT (Bit 0)                           */
#define SAR_CHAN_RESULT_RESULT_Msk        (0xffffUL)            /*!< SAR CHAN_RESULT: RESULT (Bitfield-Mask: 0xffff)           */
#define SAR_CHAN_RESULT_CHAN_RESULT_NEWVALUE_MIR_Pos (27UL)     /*!< SAR CHAN_RESULT: CHAN_RESULT_NEWVALUE_MIR (Bit 27)        */
#define SAR_CHAN_RESULT_CHAN_RESULT_NEWVALUE_MIR_Msk (0x8000000UL) /*!< SAR CHAN_RESULT: CHAN_RESULT_NEWVALUE_MIR (Bitfield-Mask: 0x01)*/
#define SAR_CHAN_RESULT_SATURATE_INTR_MIR_Pos (29UL)            /*!< SAR CHAN_RESULT: SATURATE_INTR_MIR (Bit 29)               */
#define SAR_CHAN_RESULT_SATURATE_INTR_MIR_Msk (0x20000000UL)    /*!< SAR CHAN_RESULT: SATURATE_INTR_MIR (Bitfield-Mask: 0x01)  */
#define SAR_CHAN_RESULT_RANGE_INTR_MIR_Pos (30UL)               /*!< SAR CHAN_RESULT: RANGE_INTR_MIR (Bit 30)                  */
#define SAR_CHAN_RESULT_RANGE_INTR_MIR_Msk (0x40000000UL)       /*!< SAR CHAN_RESULT: RANGE_INTR_MIR (Bitfield-Mask: 0x01)     */
#define SAR_CHAN_RESULT_CHAN_RESULT_UPDATED_MIR_Pos (31UL)      /*!< SAR CHAN_RESULT: CHAN_RESULT_UPDATED_MIR (Bit 31)         */
#define SAR_CHAN_RESULT_CHAN_RESULT_UPDATED_MIR_Msk (0x80000000UL) /*!< SAR CHAN_RESULT: CHAN_RESULT_UPDATED_MIR (Bitfield-Mask: 0x01)*/
/* =================================================  SAR.CHAN_WORK_UPDATED  ================================================= */
#define SAR_CHAN_WORK_UPDATED_CHAN_WORK_UPDATED_Pos (0UL)       /*!< SAR CHAN_WORK_UPDATED: CHAN_WORK_UPDATED (Bit 0)          */
#define SAR_CHAN_WORK_UPDATED_CHAN_WORK_UPDATED_Msk (0xffffUL)  /*!< SAR CHAN_WORK_UPDATED: CHAN_WORK_UPDATED (Bitfield-Mask: 0xffff)*/
/* ================================================  SAR.CHAN_RESULT_UPDATED  ================================================ */
#define SAR_CHAN_RESULT_UPDATED_CHAN_RESULT_UPDATED_Pos (0UL)   /*!< SAR CHAN_RESULT_UPDATED: CHAN_RESULT_UPDATED (Bit 0)      */
#define SAR_CHAN_RESULT_UPDATED_CHAN_RESULT_UPDATED_Msk (0xffffUL) /*!< SAR CHAN_RESULT_UPDATED: CHAN_RESULT_UPDATED (Bitfield-Mask: 0xffff)*/
/* ================================================  SAR.CHAN_WORK_NEWVALUE  ================================================= */
#define SAR_CHAN_WORK_NEWVALUE_CHAN_WORK_NEWVALUE_Pos (0UL)     /*!< SAR CHAN_WORK_NEWVALUE: CHAN_WORK_NEWVALUE (Bit 0)        */
#define SAR_CHAN_WORK_NEWVALUE_CHAN_WORK_NEWVALUE_Msk (0xffffUL) /*!< SAR CHAN_WORK_NEWVALUE: CHAN_WORK_NEWVALUE (Bitfield-Mask: 0xffff)*/
/* ===============================================  SAR.CHAN_RESULT_NEWVALUE  ================================================ */
#define SAR_CHAN_RESULT_NEWVALUE_CHAN_RESULT_NEWVALUE_Pos (0UL) /*!< SAR CHAN_RESULT_NEWVALUE: CHAN_RESULT_NEWVALUE (Bit 0)    */
#define SAR_CHAN_RESULT_NEWVALUE_CHAN_RESULT_NEWVALUE_Msk (0xffffUL) /*!< SAR CHAN_RESULT_NEWVALUE: CHAN_RESULT_NEWVALUE (Bitfield-Mask: 0xffff)*/
/* =======================================================  SAR.INTR  ======================================================== */
#define SAR_INTR_EOS_INTR_Pos             (0UL)                 /*!< SAR INTR: EOS_INTR (Bit 0)                                */
#define SAR_INTR_EOS_INTR_Msk             (0x1UL)               /*!< SAR INTR: EOS_INTR (Bitfield-Mask: 0x01)                  */
#define SAR_INTR_OVERFLOW_INTR_Pos        (1UL)                 /*!< SAR INTR: OVERFLOW_INTR (Bit 1)                           */
#define SAR_INTR_OVERFLOW_INTR_Msk        (0x2UL)               /*!< SAR INTR: OVERFLOW_INTR (Bitfield-Mask: 0x01)             */
#define SAR_INTR_FW_COLLISION_INTR_Pos    (2UL)                 /*!< SAR INTR: FW_COLLISION_INTR (Bit 2)                       */
#define SAR_INTR_FW_COLLISION_INTR_Msk    (0x4UL)               /*!< SAR INTR: FW_COLLISION_INTR (Bitfield-Mask: 0x01)         */
#define SAR_INTR_DSI_COLLISION_INTR_Pos   (3UL)                 /*!< SAR INTR: DSI_COLLISION_INTR (Bit 3)                      */
#define SAR_INTR_DSI_COLLISION_INTR_Msk   (0x8UL)               /*!< SAR INTR: DSI_COLLISION_INTR (Bitfield-Mask: 0x01)        */
#define SAR_INTR_INJ_EOC_INTR_Pos         (4UL)                 /*!< SAR INTR: INJ_EOC_INTR (Bit 4)                            */
#define SAR_INTR_INJ_EOC_INTR_Msk         (0x10UL)              /*!< SAR INTR: INJ_EOC_INTR (Bitfield-Mask: 0x01)              */
#define SAR_INTR_INJ_SATURATE_INTR_Pos    (5UL)                 /*!< SAR INTR: INJ_SATURATE_INTR (Bit 5)                       */
#define SAR_INTR_INJ_SATURATE_INTR_Msk    (0x20UL)              /*!< SAR INTR: INJ_SATURATE_INTR (Bitfield-Mask: 0x01)         */
#define SAR_INTR_INJ_RANGE_INTR_Pos       (6UL)                 /*!< SAR INTR: INJ_RANGE_INTR (Bit 6)                          */
#define SAR_INTR_INJ_RANGE_INTR_Msk       (0x40UL)              /*!< SAR INTR: INJ_RANGE_INTR (Bitfield-Mask: 0x01)            */
#define SAR_INTR_INJ_COLLISION_INTR_Pos   (7UL)                 /*!< SAR INTR: INJ_COLLISION_INTR (Bit 7)                      */
#define SAR_INTR_INJ_COLLISION_INTR_Msk   (0x80UL)              /*!< SAR INTR: INJ_COLLISION_INTR (Bitfield-Mask: 0x01)        */
/* =====================================================  SAR.INTR_SET  ====================================================== */
#define SAR_INTR_SET_EOS_SET_Pos          (0UL)                 /*!< SAR INTR_SET: EOS_SET (Bit 0)                             */
#define SAR_INTR_SET_EOS_SET_Msk          (0x1UL)               /*!< SAR INTR_SET: EOS_SET (Bitfield-Mask: 0x01)               */
#define SAR_INTR_SET_OVERFLOW_SET_Pos     (1UL)                 /*!< SAR INTR_SET: OVERFLOW_SET (Bit 1)                        */
#define SAR_INTR_SET_OVERFLOW_SET_Msk     (0x2UL)               /*!< SAR INTR_SET: OVERFLOW_SET (Bitfield-Mask: 0x01)          */
#define SAR_INTR_SET_FW_COLLISION_SET_Pos (2UL)                 /*!< SAR INTR_SET: FW_COLLISION_SET (Bit 2)                    */
#define SAR_INTR_SET_FW_COLLISION_SET_Msk (0x4UL)               /*!< SAR INTR_SET: FW_COLLISION_SET (Bitfield-Mask: 0x01)      */
#define SAR_INTR_SET_DSI_COLLISION_SET_Pos (3UL)                /*!< SAR INTR_SET: DSI_COLLISION_SET (Bit 3)                   */
#define SAR_INTR_SET_DSI_COLLISION_SET_Msk (0x8UL)              /*!< SAR INTR_SET: DSI_COLLISION_SET (Bitfield-Mask: 0x01)     */
#define SAR_INTR_SET_INJ_EOC_SET_Pos      (4UL)                 /*!< SAR INTR_SET: INJ_EOC_SET (Bit 4)                         */
#define SAR_INTR_SET_INJ_EOC_SET_Msk      (0x10UL)              /*!< SAR INTR_SET: INJ_EOC_SET (Bitfield-Mask: 0x01)           */
#define SAR_INTR_SET_INJ_SATURATE_SET_Pos (5UL)                 /*!< SAR INTR_SET: INJ_SATURATE_SET (Bit 5)                    */
#define SAR_INTR_SET_INJ_SATURATE_SET_Msk (0x20UL)              /*!< SAR INTR_SET: INJ_SATURATE_SET (Bitfield-Mask: 0x01)      */
#define SAR_INTR_SET_INJ_RANGE_SET_Pos    (6UL)                 /*!< SAR INTR_SET: INJ_RANGE_SET (Bit 6)                       */
#define SAR_INTR_SET_INJ_RANGE_SET_Msk    (0x40UL)              /*!< SAR INTR_SET: INJ_RANGE_SET (Bitfield-Mask: 0x01)         */
#define SAR_INTR_SET_INJ_COLLISION_SET_Pos (7UL)                /*!< SAR INTR_SET: INJ_COLLISION_SET (Bit 7)                   */
#define SAR_INTR_SET_INJ_COLLISION_SET_Msk (0x80UL)             /*!< SAR INTR_SET: INJ_COLLISION_SET (Bitfield-Mask: 0x01)     */
/* =====================================================  SAR.INTR_MASK  ===================================================== */
#define SAR_INTR_MASK_EOS_MASK_Pos        (0UL)                 /*!< SAR INTR_MASK: EOS_MASK (Bit 0)                           */
#define SAR_INTR_MASK_EOS_MASK_Msk        (0x1UL)               /*!< SAR INTR_MASK: EOS_MASK (Bitfield-Mask: 0x01)             */
#define SAR_INTR_MASK_OVERFLOW_MASK_Pos   (1UL)                 /*!< SAR INTR_MASK: OVERFLOW_MASK (Bit 1)                      */
#define SAR_INTR_MASK_OVERFLOW_MASK_Msk   (0x2UL)               /*!< SAR INTR_MASK: OVERFLOW_MASK (Bitfield-Mask: 0x01)        */
#define SAR_INTR_MASK_FW_COLLISION_MASK_Pos (2UL)               /*!< SAR INTR_MASK: FW_COLLISION_MASK (Bit 2)                  */
#define SAR_INTR_MASK_FW_COLLISION_MASK_Msk (0x4UL)             /*!< SAR INTR_MASK: FW_COLLISION_MASK (Bitfield-Mask: 0x01)    */
#define SAR_INTR_MASK_DSI_COLLISION_MASK_Pos (3UL)              /*!< SAR INTR_MASK: DSI_COLLISION_MASK (Bit 3)                 */
#define SAR_INTR_MASK_DSI_COLLISION_MASK_Msk (0x8UL)            /*!< SAR INTR_MASK: DSI_COLLISION_MASK (Bitfield-Mask: 0x01)   */
#define SAR_INTR_MASK_INJ_EOC_MASK_Pos    (4UL)                 /*!< SAR INTR_MASK: INJ_EOC_MASK (Bit 4)                       */
#define SAR_INTR_MASK_INJ_EOC_MASK_Msk    (0x10UL)              /*!< SAR INTR_MASK: INJ_EOC_MASK (Bitfield-Mask: 0x01)         */
#define SAR_INTR_MASK_INJ_SATURATE_MASK_Pos (5UL)               /*!< SAR INTR_MASK: INJ_SATURATE_MASK (Bit 5)                  */
#define SAR_INTR_MASK_INJ_SATURATE_MASK_Msk (0x20UL)            /*!< SAR INTR_MASK: INJ_SATURATE_MASK (Bitfield-Mask: 0x01)    */
#define SAR_INTR_MASK_INJ_RANGE_MASK_Pos  (6UL)                 /*!< SAR INTR_MASK: INJ_RANGE_MASK (Bit 6)                     */
#define SAR_INTR_MASK_INJ_RANGE_MASK_Msk  (0x40UL)              /*!< SAR INTR_MASK: INJ_RANGE_MASK (Bitfield-Mask: 0x01)       */
#define SAR_INTR_MASK_INJ_COLLISION_MASK_Pos (7UL)              /*!< SAR INTR_MASK: INJ_COLLISION_MASK (Bit 7)                 */
#define SAR_INTR_MASK_INJ_COLLISION_MASK_Msk (0x80UL)           /*!< SAR INTR_MASK: INJ_COLLISION_MASK (Bitfield-Mask: 0x01)   */
/* ====================================================  SAR.INTR_MASKED  ==================================================== */
#define SAR_INTR_MASKED_EOS_MASKED_Pos    (0UL)                 /*!< SAR INTR_MASKED: EOS_MASKED (Bit 0)                       */
#define SAR_INTR_MASKED_EOS_MASKED_Msk    (0x1UL)               /*!< SAR INTR_MASKED: EOS_MASKED (Bitfield-Mask: 0x01)         */
#define SAR_INTR_MASKED_OVERFLOW_MASKED_Pos (1UL)               /*!< SAR INTR_MASKED: OVERFLOW_MASKED (Bit 1)                  */
#define SAR_INTR_MASKED_OVERFLOW_MASKED_Msk (0x2UL)             /*!< SAR INTR_MASKED: OVERFLOW_MASKED (Bitfield-Mask: 0x01)    */
#define SAR_INTR_MASKED_FW_COLLISION_MASKED_Pos (2UL)           /*!< SAR INTR_MASKED: FW_COLLISION_MASKED (Bit 2)              */
#define SAR_INTR_MASKED_FW_COLLISION_MASKED_Msk (0x4UL)         /*!< SAR INTR_MASKED: FW_COLLISION_MASKED (Bitfield-Mask: 0x01)*/
#define SAR_INTR_MASKED_DSI_COLLISION_MASKED_Pos (3UL)          /*!< SAR INTR_MASKED: DSI_COLLISION_MASKED (Bit 3)             */
#define SAR_INTR_MASKED_DSI_COLLISION_MASKED_Msk (0x8UL)        /*!< SAR INTR_MASKED: DSI_COLLISION_MASKED (Bitfield-Mask: 0x01)*/
#define SAR_INTR_MASKED_INJ_EOC_MASKED_Pos (4UL)                /*!< SAR INTR_MASKED: INJ_EOC_MASKED (Bit 4)                   */
#define SAR_INTR_MASKED_INJ_EOC_MASKED_Msk (0x10UL)             /*!< SAR INTR_MASKED: INJ_EOC_MASKED (Bitfield-Mask: 0x01)     */
#define SAR_INTR_MASKED_INJ_SATURATE_MASKED_Pos (5UL)           /*!< SAR INTR_MASKED: INJ_SATURATE_MASKED (Bit 5)              */
#define SAR_INTR_MASKED_INJ_SATURATE_MASKED_Msk (0x20UL)        /*!< SAR INTR_MASKED: INJ_SATURATE_MASKED (Bitfield-Mask: 0x01)*/
#define SAR_INTR_MASKED_INJ_RANGE_MASKED_Pos (6UL)              /*!< SAR INTR_MASKED: INJ_RANGE_MASKED (Bit 6)                 */
#define SAR_INTR_MASKED_INJ_RANGE_MASKED_Msk (0x40UL)           /*!< SAR INTR_MASKED: INJ_RANGE_MASKED (Bitfield-Mask: 0x01)   */
#define SAR_INTR_MASKED_INJ_COLLISION_MASKED_Pos (7UL)          /*!< SAR INTR_MASKED: INJ_COLLISION_MASKED (Bit 7)             */
#define SAR_INTR_MASKED_INJ_COLLISION_MASKED_Msk (0x80UL)       /*!< SAR INTR_MASKED: INJ_COLLISION_MASKED (Bitfield-Mask: 0x01)*/
/* ===================================================  SAR.SATURATE_INTR  =================================================== */
#define SAR_SATURATE_INTR_SATURATE_INTR_Pos (0UL)               /*!< SAR SATURATE_INTR: SATURATE_INTR (Bit 0)                  */
#define SAR_SATURATE_INTR_SATURATE_INTR_Msk (0xffffUL)          /*!< SAR SATURATE_INTR: SATURATE_INTR (Bitfield-Mask: 0xffff)  */
/* =================================================  SAR.SATURATE_INTR_SET  ================================================= */
#define SAR_SATURATE_INTR_SET_SATURATE_SET_Pos (0UL)            /*!< SAR SATURATE_INTR_SET: SATURATE_SET (Bit 0)               */
#define SAR_SATURATE_INTR_SET_SATURATE_SET_Msk (0xffffUL)       /*!< SAR SATURATE_INTR_SET: SATURATE_SET (Bitfield-Mask: 0xffff)*/
/* ================================================  SAR.SATURATE_INTR_MASK  ================================================= */
#define SAR_SATURATE_INTR_MASK_SATURATE_MASK_Pos (0UL)          /*!< SAR SATURATE_INTR_MASK: SATURATE_MASK (Bit 0)             */
#define SAR_SATURATE_INTR_MASK_SATURATE_MASK_Msk (0xffffUL)     /*!< SAR SATURATE_INTR_MASK: SATURATE_MASK (Bitfield-Mask: 0xffff)*/
/* ===============================================  SAR.SATURATE_INTR_MASKED  ================================================ */
#define SAR_SATURATE_INTR_MASKED_SATURATE_MASKED_Pos (0UL)      /*!< SAR SATURATE_INTR_MASKED: SATURATE_MASKED (Bit 0)         */
#define SAR_SATURATE_INTR_MASKED_SATURATE_MASKED_Msk (0xffffUL) /*!< SAR SATURATE_INTR_MASKED: SATURATE_MASKED (Bitfield-Mask: 0xffff)*/
/* ====================================================  SAR.RANGE_INTR  ===================================================== */
#define SAR_RANGE_INTR_RANGE_INTR_Pos     (0UL)                 /*!< SAR RANGE_INTR: RANGE_INTR (Bit 0)                        */
#define SAR_RANGE_INTR_RANGE_INTR_Msk     (0xffffUL)            /*!< SAR RANGE_INTR: RANGE_INTR (Bitfield-Mask: 0xffff)        */
/* ==================================================  SAR.RANGE_INTR_SET  =================================================== */
#define SAR_RANGE_INTR_SET_RANGE_SET_Pos  (0UL)                 /*!< SAR RANGE_INTR_SET: RANGE_SET (Bit 0)                     */
#define SAR_RANGE_INTR_SET_RANGE_SET_Msk  (0xffffUL)            /*!< SAR RANGE_INTR_SET: RANGE_SET (Bitfield-Mask: 0xffff)     */
/* ==================================================  SAR.RANGE_INTR_MASK  ================================================== */
#define SAR_RANGE_INTR_MASK_RANGE_MASK_Pos (0UL)                /*!< SAR RANGE_INTR_MASK: RANGE_MASK (Bit 0)                   */
#define SAR_RANGE_INTR_MASK_RANGE_MASK_Msk (0xffffUL)           /*!< SAR RANGE_INTR_MASK: RANGE_MASK (Bitfield-Mask: 0xffff)   */
/* =================================================  SAR.RANGE_INTR_MASKED  ================================================= */
#define SAR_RANGE_INTR_MASKED_RANGE_MASKED_Pos (0UL)            /*!< SAR RANGE_INTR_MASKED: RANGE_MASKED (Bit 0)               */
#define SAR_RANGE_INTR_MASKED_RANGE_MASKED_Msk (0xffffUL)       /*!< SAR RANGE_INTR_MASKED: RANGE_MASKED (Bitfield-Mask: 0xffff)*/
/* ====================================================  SAR.INTR_CAUSE  ===================================================== */
#define SAR_INTR_CAUSE_EOS_MASKED_MIR_Pos (0UL)                 /*!< SAR INTR_CAUSE: EOS_MASKED_MIR (Bit 0)                    */
#define SAR_INTR_CAUSE_EOS_MASKED_MIR_Msk (0x1UL)               /*!< SAR INTR_CAUSE: EOS_MASKED_MIR (Bitfield-Mask: 0x01)      */
#define SAR_INTR_CAUSE_OVERFLOW_MASKED_MIR_Pos (1UL)            /*!< SAR INTR_CAUSE: OVERFLOW_MASKED_MIR (Bit 1)               */
#define SAR_INTR_CAUSE_OVERFLOW_MASKED_MIR_Msk (0x2UL)          /*!< SAR INTR_CAUSE: OVERFLOW_MASKED_MIR (Bitfield-Mask: 0x01) */
#define SAR_INTR_CAUSE_FW_COLLISION_MASKED_MIR_Pos (2UL)        /*!< SAR INTR_CAUSE: FW_COLLISION_MASKED_MIR (Bit 2)           */
#define SAR_INTR_CAUSE_FW_COLLISION_MASKED_MIR_Msk (0x4UL)      /*!< SAR INTR_CAUSE: FW_COLLISION_MASKED_MIR (Bitfield-Mask: 0x01)*/
#define SAR_INTR_CAUSE_DSI_COLLISION_MASKED_MIR_Pos (3UL)       /*!< SAR INTR_CAUSE: DSI_COLLISION_MASKED_MIR (Bit 3)          */
#define SAR_INTR_CAUSE_DSI_COLLISION_MASKED_MIR_Msk (0x8UL)     /*!< SAR INTR_CAUSE: DSI_COLLISION_MASKED_MIR (Bitfield-Mask: 0x01)*/
#define SAR_INTR_CAUSE_INJ_EOC_MASKED_MIR_Pos (4UL)             /*!< SAR INTR_CAUSE: INJ_EOC_MASKED_MIR (Bit 4)                */
#define SAR_INTR_CAUSE_INJ_EOC_MASKED_MIR_Msk (0x10UL)          /*!< SAR INTR_CAUSE: INJ_EOC_MASKED_MIR (Bitfield-Mask: 0x01)  */
#define SAR_INTR_CAUSE_INJ_SATURATE_MASKED_MIR_Pos (5UL)        /*!< SAR INTR_CAUSE: INJ_SATURATE_MASKED_MIR (Bit 5)           */
#define SAR_INTR_CAUSE_INJ_SATURATE_MASKED_MIR_Msk (0x20UL)     /*!< SAR INTR_CAUSE: INJ_SATURATE_MASKED_MIR (Bitfield-Mask: 0x01)*/
#define SAR_INTR_CAUSE_INJ_RANGE_MASKED_MIR_Pos (6UL)           /*!< SAR INTR_CAUSE: INJ_RANGE_MASKED_MIR (Bit 6)              */
#define SAR_INTR_CAUSE_INJ_RANGE_MASKED_MIR_Msk (0x40UL)        /*!< SAR INTR_CAUSE: INJ_RANGE_MASKED_MIR (Bitfield-Mask: 0x01)*/
#define SAR_INTR_CAUSE_INJ_COLLISION_MASKED_MIR_Pos (7UL)       /*!< SAR INTR_CAUSE: INJ_COLLISION_MASKED_MIR (Bit 7)          */
#define SAR_INTR_CAUSE_INJ_COLLISION_MASKED_MIR_Msk (0x80UL)    /*!< SAR INTR_CAUSE: INJ_COLLISION_MASKED_MIR (Bitfield-Mask: 0x01)*/
#define SAR_INTR_CAUSE_SATURATE_MASKED_RED_Pos (30UL)           /*!< SAR INTR_CAUSE: SATURATE_MASKED_RED (Bit 30)              */
#define SAR_INTR_CAUSE_SATURATE_MASKED_RED_Msk (0x40000000UL)   /*!< SAR INTR_CAUSE: SATURATE_MASKED_RED (Bitfield-Mask: 0x01) */
#define SAR_INTR_CAUSE_RANGE_MASKED_RED_Pos (31UL)              /*!< SAR INTR_CAUSE: RANGE_MASKED_RED (Bit 31)                 */
#define SAR_INTR_CAUSE_RANGE_MASKED_RED_Msk (0x80000000UL)      /*!< SAR INTR_CAUSE: RANGE_MASKED_RED (Bitfield-Mask: 0x01)    */
/* ==================================================  SAR.INJ_CHAN_CONFIG  ================================================== */
#define SAR_INJ_CHAN_CONFIG_INJ_PIN_ADDR_Pos (0UL)              /*!< SAR INJ_CHAN_CONFIG: INJ_PIN_ADDR (Bit 0)                 */
#define SAR_INJ_CHAN_CONFIG_INJ_PIN_ADDR_Msk (0x7UL)            /*!< SAR INJ_CHAN_CONFIG: INJ_PIN_ADDR (Bitfield-Mask: 0x07)   */
#define SAR_INJ_CHAN_CONFIG_INJ_PORT_ADDR_Pos (4UL)             /*!< SAR INJ_CHAN_CONFIG: INJ_PORT_ADDR (Bit 4)                */
#define SAR_INJ_CHAN_CONFIG_INJ_PORT_ADDR_Msk (0x70UL)          /*!< SAR INJ_CHAN_CONFIG: INJ_PORT_ADDR (Bitfield-Mask: 0x07)  */
#define SAR_INJ_CHAN_CONFIG_INJ_DIFFERENTIAL_EN_Pos (8UL)       /*!< SAR INJ_CHAN_CONFIG: INJ_DIFFERENTIAL_EN (Bit 8)          */
#define SAR_INJ_CHAN_CONFIG_INJ_DIFFERENTIAL_EN_Msk (0x100UL)   /*!< SAR INJ_CHAN_CONFIG: INJ_DIFFERENTIAL_EN (Bitfield-Mask: 0x01)*/
#define SAR_INJ_CHAN_CONFIG_INJ_AVG_EN_Pos (10UL)               /*!< SAR INJ_CHAN_CONFIG: INJ_AVG_EN (Bit 10)                  */
#define SAR_INJ_CHAN_CONFIG_INJ_AVG_EN_Msk (0x400UL)            /*!< SAR INJ_CHAN_CONFIG: INJ_AVG_EN (Bitfield-Mask: 0x01)     */
#define SAR_INJ_CHAN_CONFIG_INJ_SAMPLE_TIME_SEL_Pos (12UL)      /*!< SAR INJ_CHAN_CONFIG: INJ_SAMPLE_TIME_SEL (Bit 12)         */
#define SAR_INJ_CHAN_CONFIG_INJ_SAMPLE_TIME_SEL_Msk (0x3000UL)  /*!< SAR INJ_CHAN_CONFIG: INJ_SAMPLE_TIME_SEL (Bitfield-Mask: 0x03)*/
#define SAR_INJ_CHAN_CONFIG_INJ_TAILGATING_Pos (30UL)           /*!< SAR INJ_CHAN_CONFIG: INJ_TAILGATING (Bit 30)              */
#define SAR_INJ_CHAN_CONFIG_INJ_TAILGATING_Msk (0x40000000UL)   /*!< SAR INJ_CHAN_CONFIG: INJ_TAILGATING (Bitfield-Mask: 0x01) */
#define SAR_INJ_CHAN_CONFIG_INJ_START_EN_Pos (31UL)             /*!< SAR INJ_CHAN_CONFIG: INJ_START_EN (Bit 31)                */
#define SAR_INJ_CHAN_CONFIG_INJ_START_EN_Msk (0x80000000UL)     /*!< SAR INJ_CHAN_CONFIG: INJ_START_EN (Bitfield-Mask: 0x01)   */
/* ====================================================  SAR.INJ_RESULT  ===================================================== */
#define SAR_INJ_RESULT_INJ_RESULT_Pos     (0UL)                 /*!< SAR INJ_RESULT: INJ_RESULT (Bit 0)                        */
#define SAR_INJ_RESULT_INJ_RESULT_Msk     (0xffffUL)            /*!< SAR INJ_RESULT: INJ_RESULT (Bitfield-Mask: 0xffff)        */
#define SAR_INJ_RESULT_INJ_NEWVALUE_Pos   (27UL)                /*!< SAR INJ_RESULT: INJ_NEWVALUE (Bit 27)                     */
#define SAR_INJ_RESULT_INJ_NEWVALUE_Msk   (0x8000000UL)         /*!< SAR INJ_RESULT: INJ_NEWVALUE (Bitfield-Mask: 0x01)        */
#define SAR_INJ_RESULT_INJ_COLLISION_INTR_MIR_Pos (28UL)        /*!< SAR INJ_RESULT: INJ_COLLISION_INTR_MIR (Bit 28)           */
#define SAR_INJ_RESULT_INJ_COLLISION_INTR_MIR_Msk (0x10000000UL) /*!< SAR INJ_RESULT: INJ_COLLISION_INTR_MIR (Bitfield-Mask: 0x01)*/
#define SAR_INJ_RESULT_INJ_SATURATE_INTR_MIR_Pos (29UL)         /*!< SAR INJ_RESULT: INJ_SATURATE_INTR_MIR (Bit 29)            */
#define SAR_INJ_RESULT_INJ_SATURATE_INTR_MIR_Msk (0x20000000UL) /*!< SAR INJ_RESULT: INJ_SATURATE_INTR_MIR (Bitfield-Mask: 0x01)*/
#define SAR_INJ_RESULT_INJ_RANGE_INTR_MIR_Pos (30UL)            /*!< SAR INJ_RESULT: INJ_RANGE_INTR_MIR (Bit 30)               */
#define SAR_INJ_RESULT_INJ_RANGE_INTR_MIR_Msk (0x40000000UL)    /*!< SAR INJ_RESULT: INJ_RANGE_INTR_MIR (Bitfield-Mask: 0x01)  */
#define SAR_INJ_RESULT_INJ_EOC_INTR_MIR_Pos (31UL)              /*!< SAR INJ_RESULT: INJ_EOC_INTR_MIR (Bit 31)                 */
#define SAR_INJ_RESULT_INJ_EOC_INTR_MIR_Msk (0x80000000UL)      /*!< SAR INJ_RESULT: INJ_EOC_INTR_MIR (Bitfield-Mask: 0x01)    */
/* ======================================================  SAR.STATUS  ======================================================= */
#define SAR_STATUS_CUR_CHAN_Pos           (0UL)                 /*!< SAR STATUS: CUR_CHAN (Bit 0)                              */
#define SAR_STATUS_CUR_CHAN_Msk           (0x1fUL)              /*!< SAR STATUS: CUR_CHAN (Bitfield-Mask: 0x1f)                */
#define SAR_STATUS_SW_VREF_NEG_Pos        (30UL)                /*!< SAR STATUS: SW_VREF_NEG (Bit 30)                          */
#define SAR_STATUS_SW_VREF_NEG_Msk        (0x40000000UL)        /*!< SAR STATUS: SW_VREF_NEG (Bitfield-Mask: 0x01)             */
#define SAR_STATUS_BUSY_Pos               (31UL)                /*!< SAR STATUS: BUSY (Bit 31)                                 */
#define SAR_STATUS_BUSY_Msk               (0x80000000UL)        /*!< SAR STATUS: BUSY (Bitfield-Mask: 0x01)                    */
/* =====================================================  SAR.AVG_STAT  ====================================================== */
#define SAR_AVG_STAT_CUR_AVG_ACCU_Pos     (0UL)                 /*!< SAR AVG_STAT: CUR_AVG_ACCU (Bit 0)                        */
#define SAR_AVG_STAT_CUR_AVG_ACCU_Msk     (0xfffffUL)           /*!< SAR AVG_STAT: CUR_AVG_ACCU (Bitfield-Mask: 0xfffff)       */
#define SAR_AVG_STAT_INTRLV_BUSY_Pos      (23UL)                /*!< SAR AVG_STAT: INTRLV_BUSY (Bit 23)                        */
#define SAR_AVG_STAT_INTRLV_BUSY_Msk      (0x800000UL)          /*!< SAR AVG_STAT: INTRLV_BUSY (Bitfield-Mask: 0x01)           */
#define SAR_AVG_STAT_CUR_AVG_CNT_Pos      (24UL)                /*!< SAR AVG_STAT: CUR_AVG_CNT (Bit 24)                        */
#define SAR_AVG_STAT_CUR_AVG_CNT_Msk      (0xff000000UL)        /*!< SAR AVG_STAT: CUR_AVG_CNT (Bitfield-Mask: 0xff)           */
/* ====================================================  SAR.MUX_SWITCH0  ==================================================== */
#define SAR_MUX_SWITCH0_MUX_FW_P0_VPLUS_Pos (0UL)               /*!< SAR MUX_SWITCH0: MUX_FW_P0_VPLUS (Bit 0)                  */
#define SAR_MUX_SWITCH0_MUX_FW_P0_VPLUS_Msk (0x1UL)             /*!< SAR MUX_SWITCH0: MUX_FW_P0_VPLUS (Bitfield-Mask: 0x01)    */
#define SAR_MUX_SWITCH0_MUX_FW_P1_VPLUS_Pos (1UL)               /*!< SAR MUX_SWITCH0: MUX_FW_P1_VPLUS (Bit 1)                  */
#define SAR_MUX_SWITCH0_MUX_FW_P1_VPLUS_Msk (0x2UL)             /*!< SAR MUX_SWITCH0: MUX_FW_P1_VPLUS (Bitfield-Mask: 0x01)    */
#define SAR_MUX_SWITCH0_MUX_FW_P2_VPLUS_Pos (2UL)               /*!< SAR MUX_SWITCH0: MUX_FW_P2_VPLUS (Bit 2)                  */
#define SAR_MUX_SWITCH0_MUX_FW_P2_VPLUS_Msk (0x4UL)             /*!< SAR MUX_SWITCH0: MUX_FW_P2_VPLUS (Bitfield-Mask: 0x01)    */
#define SAR_MUX_SWITCH0_MUX_FW_P3_VPLUS_Pos (3UL)               /*!< SAR MUX_SWITCH0: MUX_FW_P3_VPLUS (Bit 3)                  */
#define SAR_MUX_SWITCH0_MUX_FW_P3_VPLUS_Msk (0x8UL)             /*!< SAR MUX_SWITCH0: MUX_FW_P3_VPLUS (Bitfield-Mask: 0x01)    */
#define SAR_MUX_SWITCH0_MUX_FW_P4_VPLUS_Pos (4UL)               /*!< SAR MUX_SWITCH0: MUX_FW_P4_VPLUS (Bit 4)                  */
#define SAR_MUX_SWITCH0_MUX_FW_P4_VPLUS_Msk (0x10UL)            /*!< SAR MUX_SWITCH0: MUX_FW_P4_VPLUS (Bitfield-Mask: 0x01)    */
#define SAR_MUX_SWITCH0_MUX_FW_P5_VPLUS_Pos (5UL)               /*!< SAR MUX_SWITCH0: MUX_FW_P5_VPLUS (Bit 5)                  */
#define SAR_MUX_SWITCH0_MUX_FW_P5_VPLUS_Msk (0x20UL)            /*!< SAR MUX_SWITCH0: MUX_FW_P5_VPLUS (Bitfield-Mask: 0x01)    */
#define SAR_MUX_SWITCH0_MUX_FW_P6_VPLUS_Pos (6UL)               /*!< SAR MUX_SWITCH0: MUX_FW_P6_VPLUS (Bit 6)                  */
#define SAR_MUX_SWITCH0_MUX_FW_P6_VPLUS_Msk (0x40UL)            /*!< SAR MUX_SWITCH0: MUX_FW_P6_VPLUS (Bitfield-Mask: 0x01)    */
#define SAR_MUX_SWITCH0_MUX_FW_P7_VPLUS_Pos (7UL)               /*!< SAR MUX_SWITCH0: MUX_FW_P7_VPLUS (Bit 7)                  */
#define SAR_MUX_SWITCH0_MUX_FW_P7_VPLUS_Msk (0x80UL)            /*!< SAR MUX_SWITCH0: MUX_FW_P7_VPLUS (Bitfield-Mask: 0x01)    */
#define SAR_MUX_SWITCH0_MUX_FW_P0_VMINUS_Pos (8UL)              /*!< SAR MUX_SWITCH0: MUX_FW_P0_VMINUS (Bit 8)                 */
#define SAR_MUX_SWITCH0_MUX_FW_P0_VMINUS_Msk (0x100UL)          /*!< SAR MUX_SWITCH0: MUX_FW_P0_VMINUS (Bitfield-Mask: 0x01)   */
#define SAR_MUX_SWITCH0_MUX_FW_P1_VMINUS_Pos (9UL)              /*!< SAR MUX_SWITCH0: MUX_FW_P1_VMINUS (Bit 9)                 */
#define SAR_MUX_SWITCH0_MUX_FW_P1_VMINUS_Msk (0x200UL)          /*!< SAR MUX_SWITCH0: MUX_FW_P1_VMINUS (Bitfield-Mask: 0x01)   */
#define SAR_MUX_SWITCH0_MUX_FW_P2_VMINUS_Pos (10UL)             /*!< SAR MUX_SWITCH0: MUX_FW_P2_VMINUS (Bit 10)                */
#define SAR_MUX_SWITCH0_MUX_FW_P2_VMINUS_Msk (0x400UL)          /*!< SAR MUX_SWITCH0: MUX_FW_P2_VMINUS (Bitfield-Mask: 0x01)   */
#define SAR_MUX_SWITCH0_MUX_FW_P3_VMINUS_Pos (11UL)             /*!< SAR MUX_SWITCH0: MUX_FW_P3_VMINUS (Bit 11)                */
#define SAR_MUX_SWITCH0_MUX_FW_P3_VMINUS_Msk (0x800UL)          /*!< SAR MUX_SWITCH0: MUX_FW_P3_VMINUS (Bitfield-Mask: 0x01)   */
#define SAR_MUX_SWITCH0_MUX_FW_P4_VMINUS_Pos (12UL)             /*!< SAR MUX_SWITCH0: MUX_FW_P4_VMINUS (Bit 12)                */
#define SAR_MUX_SWITCH0_MUX_FW_P4_VMINUS_Msk (0x1000UL)         /*!< SAR MUX_SWITCH0: MUX_FW_P4_VMINUS (Bitfield-Mask: 0x01)   */
#define SAR_MUX_SWITCH0_MUX_FW_P5_VMINUS_Pos (13UL)             /*!< SAR MUX_SWITCH0: MUX_FW_P5_VMINUS (Bit 13)                */
#define SAR_MUX_SWITCH0_MUX_FW_P5_VMINUS_Msk (0x2000UL)         /*!< SAR MUX_SWITCH0: MUX_FW_P5_VMINUS (Bitfield-Mask: 0x01)   */
#define SAR_MUX_SWITCH0_MUX_FW_P6_VMINUS_Pos (14UL)             /*!< SAR MUX_SWITCH0: MUX_FW_P6_VMINUS (Bit 14)                */
#define SAR_MUX_SWITCH0_MUX_FW_P6_VMINUS_Msk (0x4000UL)         /*!< SAR MUX_SWITCH0: MUX_FW_P6_VMINUS (Bitfield-Mask: 0x01)   */
#define SAR_MUX_SWITCH0_MUX_FW_P7_VMINUS_Pos (15UL)             /*!< SAR MUX_SWITCH0: MUX_FW_P7_VMINUS (Bit 15)                */
#define SAR_MUX_SWITCH0_MUX_FW_P7_VMINUS_Msk (0x8000UL)         /*!< SAR MUX_SWITCH0: MUX_FW_P7_VMINUS (Bitfield-Mask: 0x01)   */
#define SAR_MUX_SWITCH0_MUX_FW_VSSA_VMINUS_Pos (16UL)           /*!< SAR MUX_SWITCH0: MUX_FW_VSSA_VMINUS (Bit 16)              */
#define SAR_MUX_SWITCH0_MUX_FW_VSSA_VMINUS_Msk (0x10000UL)      /*!< SAR MUX_SWITCH0: MUX_FW_VSSA_VMINUS (Bitfield-Mask: 0x01) */
#define SAR_MUX_SWITCH0_MUX_FW_TEMP_VPLUS_Pos (17UL)            /*!< SAR MUX_SWITCH0: MUX_FW_TEMP_VPLUS (Bit 17)               */
#define SAR_MUX_SWITCH0_MUX_FW_TEMP_VPLUS_Msk (0x20000UL)       /*!< SAR MUX_SWITCH0: MUX_FW_TEMP_VPLUS (Bitfield-Mask: 0x01)  */
#define SAR_MUX_SWITCH0_MUX_FW_AMUXBUSA_VPLUS_Pos (18UL)        /*!< SAR MUX_SWITCH0: MUX_FW_AMUXBUSA_VPLUS (Bit 18)           */
#define SAR_MUX_SWITCH0_MUX_FW_AMUXBUSA_VPLUS_Msk (0x40000UL)   /*!< SAR MUX_SWITCH0: MUX_FW_AMUXBUSA_VPLUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH0_MUX_FW_AMUXBUSB_VPLUS_Pos (19UL)        /*!< SAR MUX_SWITCH0: MUX_FW_AMUXBUSB_VPLUS (Bit 19)           */
#define SAR_MUX_SWITCH0_MUX_FW_AMUXBUSB_VPLUS_Msk (0x80000UL)   /*!< SAR MUX_SWITCH0: MUX_FW_AMUXBUSB_VPLUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH0_MUX_FW_AMUXBUSA_VMINUS_Pos (20UL)       /*!< SAR MUX_SWITCH0: MUX_FW_AMUXBUSA_VMINUS (Bit 20)          */
#define SAR_MUX_SWITCH0_MUX_FW_AMUXBUSA_VMINUS_Msk (0x100000UL) /*!< SAR MUX_SWITCH0: MUX_FW_AMUXBUSA_VMINUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH0_MUX_FW_AMUXBUSB_VMINUS_Pos (21UL)       /*!< SAR MUX_SWITCH0: MUX_FW_AMUXBUSB_VMINUS (Bit 21)          */
#define SAR_MUX_SWITCH0_MUX_FW_AMUXBUSB_VMINUS_Msk (0x200000UL) /*!< SAR MUX_SWITCH0: MUX_FW_AMUXBUSB_VMINUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH0_MUX_FW_SARBUS0_VPLUS_Pos (22UL)         /*!< SAR MUX_SWITCH0: MUX_FW_SARBUS0_VPLUS (Bit 22)            */
#define SAR_MUX_SWITCH0_MUX_FW_SARBUS0_VPLUS_Msk (0x400000UL)   /*!< SAR MUX_SWITCH0: MUX_FW_SARBUS0_VPLUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH0_MUX_FW_SARBUS1_VPLUS_Pos (23UL)         /*!< SAR MUX_SWITCH0: MUX_FW_SARBUS1_VPLUS (Bit 23)            */
#define SAR_MUX_SWITCH0_MUX_FW_SARBUS1_VPLUS_Msk (0x800000UL)   /*!< SAR MUX_SWITCH0: MUX_FW_SARBUS1_VPLUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH0_MUX_FW_SARBUS0_VMINUS_Pos (24UL)        /*!< SAR MUX_SWITCH0: MUX_FW_SARBUS0_VMINUS (Bit 24)           */
#define SAR_MUX_SWITCH0_MUX_FW_SARBUS0_VMINUS_Msk (0x1000000UL) /*!< SAR MUX_SWITCH0: MUX_FW_SARBUS0_VMINUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH0_MUX_FW_SARBUS1_VMINUS_Pos (25UL)        /*!< SAR MUX_SWITCH0: MUX_FW_SARBUS1_VMINUS (Bit 25)           */
#define SAR_MUX_SWITCH0_MUX_FW_SARBUS1_VMINUS_Msk (0x2000000UL) /*!< SAR MUX_SWITCH0: MUX_FW_SARBUS1_VMINUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH0_MUX_FW_P4_COREIO0_Pos (26UL)            /*!< SAR MUX_SWITCH0: MUX_FW_P4_COREIO0 (Bit 26)               */
#define SAR_MUX_SWITCH0_MUX_FW_P4_COREIO0_Msk (0x4000000UL)     /*!< SAR MUX_SWITCH0: MUX_FW_P4_COREIO0 (Bitfield-Mask: 0x01)  */
#define SAR_MUX_SWITCH0_MUX_FW_P5_COREIO1_Pos (27UL)            /*!< SAR MUX_SWITCH0: MUX_FW_P5_COREIO1 (Bit 27)               */
#define SAR_MUX_SWITCH0_MUX_FW_P5_COREIO1_Msk (0x8000000UL)     /*!< SAR MUX_SWITCH0: MUX_FW_P5_COREIO1 (Bitfield-Mask: 0x01)  */
#define SAR_MUX_SWITCH0_MUX_FW_P6_COREIO2_Pos (28UL)            /*!< SAR MUX_SWITCH0: MUX_FW_P6_COREIO2 (Bit 28)               */
#define SAR_MUX_SWITCH0_MUX_FW_P6_COREIO2_Msk (0x10000000UL)    /*!< SAR MUX_SWITCH0: MUX_FW_P6_COREIO2 (Bitfield-Mask: 0x01)  */
#define SAR_MUX_SWITCH0_MUX_FW_P7_COREIO3_Pos (29UL)            /*!< SAR MUX_SWITCH0: MUX_FW_P7_COREIO3 (Bit 29)               */
#define SAR_MUX_SWITCH0_MUX_FW_P7_COREIO3_Msk (0x20000000UL)    /*!< SAR MUX_SWITCH0: MUX_FW_P7_COREIO3 (Bitfield-Mask: 0x01)  */
/* =================================================  SAR.MUX_SWITCH_CLEAR0  ================================================= */
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_P0_VPLUS_Pos (0UL)         /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_P0_VPLUS (Bit 0)            */
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_P0_VPLUS_Msk (0x1UL)       /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_P0_VPLUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_P1_VPLUS_Pos (1UL)         /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_P1_VPLUS (Bit 1)            */
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_P1_VPLUS_Msk (0x2UL)       /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_P1_VPLUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_P2_VPLUS_Pos (2UL)         /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_P2_VPLUS (Bit 2)            */
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_P2_VPLUS_Msk (0x4UL)       /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_P2_VPLUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_P3_VPLUS_Pos (3UL)         /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_P3_VPLUS (Bit 3)            */
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_P3_VPLUS_Msk (0x8UL)       /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_P3_VPLUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_P4_VPLUS_Pos (4UL)         /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_P4_VPLUS (Bit 4)            */
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_P4_VPLUS_Msk (0x10UL)      /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_P4_VPLUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_P5_VPLUS_Pos (5UL)         /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_P5_VPLUS (Bit 5)            */
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_P5_VPLUS_Msk (0x20UL)      /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_P5_VPLUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_P6_VPLUS_Pos (6UL)         /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_P6_VPLUS (Bit 6)            */
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_P6_VPLUS_Msk (0x40UL)      /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_P6_VPLUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_P7_VPLUS_Pos (7UL)         /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_P7_VPLUS (Bit 7)            */
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_P7_VPLUS_Msk (0x80UL)      /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_P7_VPLUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_P0_VMINUS_Pos (8UL)        /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_P0_VMINUS (Bit 8)           */
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_P0_VMINUS_Msk (0x100UL)    /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_P0_VMINUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_P1_VMINUS_Pos (9UL)        /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_P1_VMINUS (Bit 9)           */
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_P1_VMINUS_Msk (0x200UL)    /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_P1_VMINUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_P2_VMINUS_Pos (10UL)       /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_P2_VMINUS (Bit 10)          */
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_P2_VMINUS_Msk (0x400UL)    /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_P2_VMINUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_P3_VMINUS_Pos (11UL)       /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_P3_VMINUS (Bit 11)          */
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_P3_VMINUS_Msk (0x800UL)    /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_P3_VMINUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_P4_VMINUS_Pos (12UL)       /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_P4_VMINUS (Bit 12)          */
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_P4_VMINUS_Msk (0x1000UL)   /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_P4_VMINUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_P5_VMINUS_Pos (13UL)       /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_P5_VMINUS (Bit 13)          */
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_P5_VMINUS_Msk (0x2000UL)   /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_P5_VMINUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_P6_VMINUS_Pos (14UL)       /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_P6_VMINUS (Bit 14)          */
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_P6_VMINUS_Msk (0x4000UL)   /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_P6_VMINUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_P7_VMINUS_Pos (15UL)       /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_P7_VMINUS (Bit 15)          */
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_P7_VMINUS_Msk (0x8000UL)   /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_P7_VMINUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_VSSA_VMINUS_Pos (16UL)     /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_VSSA_VMINUS (Bit 16)        */
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_VSSA_VMINUS_Msk (0x10000UL) /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_VSSA_VMINUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_TEMP_VPLUS_Pos (17UL)      /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_TEMP_VPLUS (Bit 17)         */
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_TEMP_VPLUS_Msk (0x20000UL) /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_TEMP_VPLUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_AMUXBUSA_VPLUS_Pos (18UL)  /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_AMUXBUSA_VPLUS (Bit 18)     */
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_AMUXBUSA_VPLUS_Msk (0x40000UL) /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_AMUXBUSA_VPLUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_AMUXBUSB_VPLUS_Pos (19UL)  /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_AMUXBUSB_VPLUS (Bit 19)     */
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_AMUXBUSB_VPLUS_Msk (0x80000UL) /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_AMUXBUSB_VPLUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_AMUXBUSA_VMINUS_Pos (20UL) /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_AMUXBUSA_VMINUS (Bit 20)    */
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_AMUXBUSA_VMINUS_Msk (0x100000UL) /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_AMUXBUSA_VMINUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_AMUXBUSB_VMINUS_Pos (21UL) /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_AMUXBUSB_VMINUS (Bit 21)    */
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_AMUXBUSB_VMINUS_Msk (0x200000UL) /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_AMUXBUSB_VMINUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_SARBUS0_VPLUS_Pos (22UL)   /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_SARBUS0_VPLUS (Bit 22)      */
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_SARBUS0_VPLUS_Msk (0x400000UL) /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_SARBUS0_VPLUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_SARBUS1_VPLUS_Pos (23UL)   /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_SARBUS1_VPLUS (Bit 23)      */
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_SARBUS1_VPLUS_Msk (0x800000UL) /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_SARBUS1_VPLUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_SARBUS0_VMINUS_Pos (24UL)  /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_SARBUS0_VMINUS (Bit 24)     */
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_SARBUS0_VMINUS_Msk (0x1000000UL) /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_SARBUS0_VMINUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_SARBUS1_VMINUS_Pos (25UL)  /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_SARBUS1_VMINUS (Bit 25)     */
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_SARBUS1_VMINUS_Msk (0x2000000UL) /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_SARBUS1_VMINUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_P4_COREIO0_Pos (26UL)      /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_P4_COREIO0 (Bit 26)         */
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_P4_COREIO0_Msk (0x4000000UL) /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_P4_COREIO0 (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_P5_COREIO1_Pos (27UL)      /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_P5_COREIO1 (Bit 27)         */
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_P5_COREIO1_Msk (0x8000000UL) /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_P5_COREIO1 (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_P6_COREIO2_Pos (28UL)      /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_P6_COREIO2 (Bit 28)         */
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_P6_COREIO2_Msk (0x10000000UL) /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_P6_COREIO2 (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_P7_COREIO3_Pos (29UL)      /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_P7_COREIO3 (Bit 29)         */
#define SAR_MUX_SWITCH_CLEAR0_MUX_FW_P7_COREIO3_Msk (0x20000000UL) /*!< SAR MUX_SWITCH_CLEAR0: MUX_FW_P7_COREIO3 (Bitfield-Mask: 0x01)*/
/* ================================================  SAR.MUX_SWITCH_DS_CTRL  ================================================= */
#define SAR_MUX_SWITCH_DS_CTRL_MUX_DS_CTRL_P0_Pos (0UL)         /*!< SAR MUX_SWITCH_DS_CTRL: MUX_DS_CTRL_P0 (Bit 0)            */
#define SAR_MUX_SWITCH_DS_CTRL_MUX_DS_CTRL_P0_Msk (0x1UL)       /*!< SAR MUX_SWITCH_DS_CTRL: MUX_DS_CTRL_P0 (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_DS_CTRL_MUX_DS_CTRL_P1_Pos (1UL)         /*!< SAR MUX_SWITCH_DS_CTRL: MUX_DS_CTRL_P1 (Bit 1)            */
#define SAR_MUX_SWITCH_DS_CTRL_MUX_DS_CTRL_P1_Msk (0x2UL)       /*!< SAR MUX_SWITCH_DS_CTRL: MUX_DS_CTRL_P1 (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_DS_CTRL_MUX_DS_CTRL_P2_Pos (2UL)         /*!< SAR MUX_SWITCH_DS_CTRL: MUX_DS_CTRL_P2 (Bit 2)            */
#define SAR_MUX_SWITCH_DS_CTRL_MUX_DS_CTRL_P2_Msk (0x4UL)       /*!< SAR MUX_SWITCH_DS_CTRL: MUX_DS_CTRL_P2 (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_DS_CTRL_MUX_DS_CTRL_P3_Pos (3UL)         /*!< SAR MUX_SWITCH_DS_CTRL: MUX_DS_CTRL_P3 (Bit 3)            */
#define SAR_MUX_SWITCH_DS_CTRL_MUX_DS_CTRL_P3_Msk (0x8UL)       /*!< SAR MUX_SWITCH_DS_CTRL: MUX_DS_CTRL_P3 (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_DS_CTRL_MUX_DS_CTRL_P4_Pos (4UL)         /*!< SAR MUX_SWITCH_DS_CTRL: MUX_DS_CTRL_P4 (Bit 4)            */
#define SAR_MUX_SWITCH_DS_CTRL_MUX_DS_CTRL_P4_Msk (0x10UL)      /*!< SAR MUX_SWITCH_DS_CTRL: MUX_DS_CTRL_P4 (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_DS_CTRL_MUX_DS_CTRL_P5_Pos (5UL)         /*!< SAR MUX_SWITCH_DS_CTRL: MUX_DS_CTRL_P5 (Bit 5)            */
#define SAR_MUX_SWITCH_DS_CTRL_MUX_DS_CTRL_P5_Msk (0x20UL)      /*!< SAR MUX_SWITCH_DS_CTRL: MUX_DS_CTRL_P5 (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_DS_CTRL_MUX_DS_CTRL_P6_Pos (6UL)         /*!< SAR MUX_SWITCH_DS_CTRL: MUX_DS_CTRL_P6 (Bit 6)            */
#define SAR_MUX_SWITCH_DS_CTRL_MUX_DS_CTRL_P6_Msk (0x40UL)      /*!< SAR MUX_SWITCH_DS_CTRL: MUX_DS_CTRL_P6 (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_DS_CTRL_MUX_DS_CTRL_P7_Pos (7UL)         /*!< SAR MUX_SWITCH_DS_CTRL: MUX_DS_CTRL_P7 (Bit 7)            */
#define SAR_MUX_SWITCH_DS_CTRL_MUX_DS_CTRL_P7_Msk (0x80UL)      /*!< SAR MUX_SWITCH_DS_CTRL: MUX_DS_CTRL_P7 (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_DS_CTRL_MUX_DS_CTRL_VSSA_Pos (16UL)      /*!< SAR MUX_SWITCH_DS_CTRL: MUX_DS_CTRL_VSSA (Bit 16)         */
#define SAR_MUX_SWITCH_DS_CTRL_MUX_DS_CTRL_VSSA_Msk (0x10000UL) /*!< SAR MUX_SWITCH_DS_CTRL: MUX_DS_CTRL_VSSA (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_DS_CTRL_MUX_DS_CTRL_TEMP_Pos (17UL)      /*!< SAR MUX_SWITCH_DS_CTRL: MUX_DS_CTRL_TEMP (Bit 17)         */
#define SAR_MUX_SWITCH_DS_CTRL_MUX_DS_CTRL_TEMP_Msk (0x20000UL) /*!< SAR MUX_SWITCH_DS_CTRL: MUX_DS_CTRL_TEMP (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_DS_CTRL_MUX_DS_CTRL_AMUXBUSA_Pos (18UL)  /*!< SAR MUX_SWITCH_DS_CTRL: MUX_DS_CTRL_AMUXBUSA (Bit 18)     */
#define SAR_MUX_SWITCH_DS_CTRL_MUX_DS_CTRL_AMUXBUSA_Msk (0x40000UL) /*!< SAR MUX_SWITCH_DS_CTRL: MUX_DS_CTRL_AMUXBUSA (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_DS_CTRL_MUX_DS_CTRL_AMUXBUSB_Pos (19UL)  /*!< SAR MUX_SWITCH_DS_CTRL: MUX_DS_CTRL_AMUXBUSB (Bit 19)     */
#define SAR_MUX_SWITCH_DS_CTRL_MUX_DS_CTRL_AMUXBUSB_Msk (0x80000UL) /*!< SAR MUX_SWITCH_DS_CTRL: MUX_DS_CTRL_AMUXBUSB (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_DS_CTRL_MUX_DS_CTRL_SARBUS0_Pos (22UL)   /*!< SAR MUX_SWITCH_DS_CTRL: MUX_DS_CTRL_SARBUS0 (Bit 22)      */
#define SAR_MUX_SWITCH_DS_CTRL_MUX_DS_CTRL_SARBUS0_Msk (0x400000UL) /*!< SAR MUX_SWITCH_DS_CTRL: MUX_DS_CTRL_SARBUS0 (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_DS_CTRL_MUX_DS_CTRL_SARBUS1_Pos (23UL)   /*!< SAR MUX_SWITCH_DS_CTRL: MUX_DS_CTRL_SARBUS1 (Bit 23)      */
#define SAR_MUX_SWITCH_DS_CTRL_MUX_DS_CTRL_SARBUS1_Msk (0x800000UL) /*!< SAR MUX_SWITCH_DS_CTRL: MUX_DS_CTRL_SARBUS1 (Bitfield-Mask: 0x01)*/
/* ================================================  SAR.MUX_SWITCH_SQ_CTRL  ================================================= */
#define SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_P0_Pos (0UL)         /*!< SAR MUX_SWITCH_SQ_CTRL: MUX_SQ_CTRL_P0 (Bit 0)            */
#define SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_P0_Msk (0x1UL)       /*!< SAR MUX_SWITCH_SQ_CTRL: MUX_SQ_CTRL_P0 (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_P1_Pos (1UL)         /*!< SAR MUX_SWITCH_SQ_CTRL: MUX_SQ_CTRL_P1 (Bit 1)            */
#define SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_P1_Msk (0x2UL)       /*!< SAR MUX_SWITCH_SQ_CTRL: MUX_SQ_CTRL_P1 (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_P2_Pos (2UL)         /*!< SAR MUX_SWITCH_SQ_CTRL: MUX_SQ_CTRL_P2 (Bit 2)            */
#define SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_P2_Msk (0x4UL)       /*!< SAR MUX_SWITCH_SQ_CTRL: MUX_SQ_CTRL_P2 (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_P3_Pos (3UL)         /*!< SAR MUX_SWITCH_SQ_CTRL: MUX_SQ_CTRL_P3 (Bit 3)            */
#define SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_P3_Msk (0x8UL)       /*!< SAR MUX_SWITCH_SQ_CTRL: MUX_SQ_CTRL_P3 (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_P4_Pos (4UL)         /*!< SAR MUX_SWITCH_SQ_CTRL: MUX_SQ_CTRL_P4 (Bit 4)            */
#define SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_P4_Msk (0x10UL)      /*!< SAR MUX_SWITCH_SQ_CTRL: MUX_SQ_CTRL_P4 (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_P5_Pos (5UL)         /*!< SAR MUX_SWITCH_SQ_CTRL: MUX_SQ_CTRL_P5 (Bit 5)            */
#define SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_P5_Msk (0x20UL)      /*!< SAR MUX_SWITCH_SQ_CTRL: MUX_SQ_CTRL_P5 (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_P6_Pos (6UL)         /*!< SAR MUX_SWITCH_SQ_CTRL: MUX_SQ_CTRL_P6 (Bit 6)            */
#define SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_P6_Msk (0x40UL)      /*!< SAR MUX_SWITCH_SQ_CTRL: MUX_SQ_CTRL_P6 (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_P7_Pos (7UL)         /*!< SAR MUX_SWITCH_SQ_CTRL: MUX_SQ_CTRL_P7 (Bit 7)            */
#define SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_P7_Msk (0x80UL)      /*!< SAR MUX_SWITCH_SQ_CTRL: MUX_SQ_CTRL_P7 (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_VSSA_Pos (16UL)      /*!< SAR MUX_SWITCH_SQ_CTRL: MUX_SQ_CTRL_VSSA (Bit 16)         */
#define SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_VSSA_Msk (0x10000UL) /*!< SAR MUX_SWITCH_SQ_CTRL: MUX_SQ_CTRL_VSSA (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_TEMP_Pos (17UL)      /*!< SAR MUX_SWITCH_SQ_CTRL: MUX_SQ_CTRL_TEMP (Bit 17)         */
#define SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_TEMP_Msk (0x20000UL) /*!< SAR MUX_SWITCH_SQ_CTRL: MUX_SQ_CTRL_TEMP (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_AMUXBUSA_Pos (18UL)  /*!< SAR MUX_SWITCH_SQ_CTRL: MUX_SQ_CTRL_AMUXBUSA (Bit 18)     */
#define SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_AMUXBUSA_Msk (0x40000UL) /*!< SAR MUX_SWITCH_SQ_CTRL: MUX_SQ_CTRL_AMUXBUSA (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_AMUXBUSB_Pos (19UL)  /*!< SAR MUX_SWITCH_SQ_CTRL: MUX_SQ_CTRL_AMUXBUSB (Bit 19)     */
#define SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_AMUXBUSB_Msk (0x80000UL) /*!< SAR MUX_SWITCH_SQ_CTRL: MUX_SQ_CTRL_AMUXBUSB (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_SARBUS0_Pos (22UL)   /*!< SAR MUX_SWITCH_SQ_CTRL: MUX_SQ_CTRL_SARBUS0 (Bit 22)      */
#define SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_SARBUS0_Msk (0x400000UL) /*!< SAR MUX_SWITCH_SQ_CTRL: MUX_SQ_CTRL_SARBUS0 (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_SARBUS1_Pos (23UL)   /*!< SAR MUX_SWITCH_SQ_CTRL: MUX_SQ_CTRL_SARBUS1 (Bit 23)      */
#define SAR_MUX_SWITCH_SQ_CTRL_MUX_SQ_CTRL_SARBUS1_Msk (0x800000UL) /*!< SAR MUX_SWITCH_SQ_CTRL: MUX_SQ_CTRL_SARBUS1 (Bitfield-Mask: 0x01)*/
/* =================================================  SAR.MUX_SWITCH_STATUS  ================================================= */
#define SAR_MUX_SWITCH_STATUS_MUX_FW_P0_VPLUS_Pos (0UL)         /*!< SAR MUX_SWITCH_STATUS: MUX_FW_P0_VPLUS (Bit 0)            */
#define SAR_MUX_SWITCH_STATUS_MUX_FW_P0_VPLUS_Msk (0x1UL)       /*!< SAR MUX_SWITCH_STATUS: MUX_FW_P0_VPLUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_STATUS_MUX_FW_P1_VPLUS_Pos (1UL)         /*!< SAR MUX_SWITCH_STATUS: MUX_FW_P1_VPLUS (Bit 1)            */
#define SAR_MUX_SWITCH_STATUS_MUX_FW_P1_VPLUS_Msk (0x2UL)       /*!< SAR MUX_SWITCH_STATUS: MUX_FW_P1_VPLUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_STATUS_MUX_FW_P2_VPLUS_Pos (2UL)         /*!< SAR MUX_SWITCH_STATUS: MUX_FW_P2_VPLUS (Bit 2)            */
#define SAR_MUX_SWITCH_STATUS_MUX_FW_P2_VPLUS_Msk (0x4UL)       /*!< SAR MUX_SWITCH_STATUS: MUX_FW_P2_VPLUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_STATUS_MUX_FW_P3_VPLUS_Pos (3UL)         /*!< SAR MUX_SWITCH_STATUS: MUX_FW_P3_VPLUS (Bit 3)            */
#define SAR_MUX_SWITCH_STATUS_MUX_FW_P3_VPLUS_Msk (0x8UL)       /*!< SAR MUX_SWITCH_STATUS: MUX_FW_P3_VPLUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_STATUS_MUX_FW_P4_VPLUS_Pos (4UL)         /*!< SAR MUX_SWITCH_STATUS: MUX_FW_P4_VPLUS (Bit 4)            */
#define SAR_MUX_SWITCH_STATUS_MUX_FW_P4_VPLUS_Msk (0x10UL)      /*!< SAR MUX_SWITCH_STATUS: MUX_FW_P4_VPLUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_STATUS_MUX_FW_P5_VPLUS_Pos (5UL)         /*!< SAR MUX_SWITCH_STATUS: MUX_FW_P5_VPLUS (Bit 5)            */
#define SAR_MUX_SWITCH_STATUS_MUX_FW_P5_VPLUS_Msk (0x20UL)      /*!< SAR MUX_SWITCH_STATUS: MUX_FW_P5_VPLUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_STATUS_MUX_FW_P6_VPLUS_Pos (6UL)         /*!< SAR MUX_SWITCH_STATUS: MUX_FW_P6_VPLUS (Bit 6)            */
#define SAR_MUX_SWITCH_STATUS_MUX_FW_P6_VPLUS_Msk (0x40UL)      /*!< SAR MUX_SWITCH_STATUS: MUX_FW_P6_VPLUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_STATUS_MUX_FW_P7_VPLUS_Pos (7UL)         /*!< SAR MUX_SWITCH_STATUS: MUX_FW_P7_VPLUS (Bit 7)            */
#define SAR_MUX_SWITCH_STATUS_MUX_FW_P7_VPLUS_Msk (0x80UL)      /*!< SAR MUX_SWITCH_STATUS: MUX_FW_P7_VPLUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_STATUS_MUX_FW_P0_VMINUS_Pos (8UL)        /*!< SAR MUX_SWITCH_STATUS: MUX_FW_P0_VMINUS (Bit 8)           */
#define SAR_MUX_SWITCH_STATUS_MUX_FW_P0_VMINUS_Msk (0x100UL)    /*!< SAR MUX_SWITCH_STATUS: MUX_FW_P0_VMINUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_STATUS_MUX_FW_P1_VMINUS_Pos (9UL)        /*!< SAR MUX_SWITCH_STATUS: MUX_FW_P1_VMINUS (Bit 9)           */
#define SAR_MUX_SWITCH_STATUS_MUX_FW_P1_VMINUS_Msk (0x200UL)    /*!< SAR MUX_SWITCH_STATUS: MUX_FW_P1_VMINUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_STATUS_MUX_FW_P2_VMINUS_Pos (10UL)       /*!< SAR MUX_SWITCH_STATUS: MUX_FW_P2_VMINUS (Bit 10)          */
#define SAR_MUX_SWITCH_STATUS_MUX_FW_P2_VMINUS_Msk (0x400UL)    /*!< SAR MUX_SWITCH_STATUS: MUX_FW_P2_VMINUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_STATUS_MUX_FW_P3_VMINUS_Pos (11UL)       /*!< SAR MUX_SWITCH_STATUS: MUX_FW_P3_VMINUS (Bit 11)          */
#define SAR_MUX_SWITCH_STATUS_MUX_FW_P3_VMINUS_Msk (0x800UL)    /*!< SAR MUX_SWITCH_STATUS: MUX_FW_P3_VMINUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_STATUS_MUX_FW_P4_VMINUS_Pos (12UL)       /*!< SAR MUX_SWITCH_STATUS: MUX_FW_P4_VMINUS (Bit 12)          */
#define SAR_MUX_SWITCH_STATUS_MUX_FW_P4_VMINUS_Msk (0x1000UL)   /*!< SAR MUX_SWITCH_STATUS: MUX_FW_P4_VMINUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_STATUS_MUX_FW_P5_VMINUS_Pos (13UL)       /*!< SAR MUX_SWITCH_STATUS: MUX_FW_P5_VMINUS (Bit 13)          */
#define SAR_MUX_SWITCH_STATUS_MUX_FW_P5_VMINUS_Msk (0x2000UL)   /*!< SAR MUX_SWITCH_STATUS: MUX_FW_P5_VMINUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_STATUS_MUX_FW_P6_VMINUS_Pos (14UL)       /*!< SAR MUX_SWITCH_STATUS: MUX_FW_P6_VMINUS (Bit 14)          */
#define SAR_MUX_SWITCH_STATUS_MUX_FW_P6_VMINUS_Msk (0x4000UL)   /*!< SAR MUX_SWITCH_STATUS: MUX_FW_P6_VMINUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_STATUS_MUX_FW_P7_VMINUS_Pos (15UL)       /*!< SAR MUX_SWITCH_STATUS: MUX_FW_P7_VMINUS (Bit 15)          */
#define SAR_MUX_SWITCH_STATUS_MUX_FW_P7_VMINUS_Msk (0x8000UL)   /*!< SAR MUX_SWITCH_STATUS: MUX_FW_P7_VMINUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_STATUS_MUX_FW_VSSA_VMINUS_Pos (16UL)     /*!< SAR MUX_SWITCH_STATUS: MUX_FW_VSSA_VMINUS (Bit 16)        */
#define SAR_MUX_SWITCH_STATUS_MUX_FW_VSSA_VMINUS_Msk (0x10000UL) /*!< SAR MUX_SWITCH_STATUS: MUX_FW_VSSA_VMINUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_STATUS_MUX_FW_TEMP_VPLUS_Pos (17UL)      /*!< SAR MUX_SWITCH_STATUS: MUX_FW_TEMP_VPLUS (Bit 17)         */
#define SAR_MUX_SWITCH_STATUS_MUX_FW_TEMP_VPLUS_Msk (0x20000UL) /*!< SAR MUX_SWITCH_STATUS: MUX_FW_TEMP_VPLUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_STATUS_MUX_FW_AMUXBUSA_VPLUS_Pos (18UL)  /*!< SAR MUX_SWITCH_STATUS: MUX_FW_AMUXBUSA_VPLUS (Bit 18)     */
#define SAR_MUX_SWITCH_STATUS_MUX_FW_AMUXBUSA_VPLUS_Msk (0x40000UL) /*!< SAR MUX_SWITCH_STATUS: MUX_FW_AMUXBUSA_VPLUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_STATUS_MUX_FW_AMUXBUSB_VPLUS_Pos (19UL)  /*!< SAR MUX_SWITCH_STATUS: MUX_FW_AMUXBUSB_VPLUS (Bit 19)     */
#define SAR_MUX_SWITCH_STATUS_MUX_FW_AMUXBUSB_VPLUS_Msk (0x80000UL) /*!< SAR MUX_SWITCH_STATUS: MUX_FW_AMUXBUSB_VPLUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_STATUS_MUX_FW_AMUXBUSA_VMINUS_Pos (20UL) /*!< SAR MUX_SWITCH_STATUS: MUX_FW_AMUXBUSA_VMINUS (Bit 20)    */
#define SAR_MUX_SWITCH_STATUS_MUX_FW_AMUXBUSA_VMINUS_Msk (0x100000UL) /*!< SAR MUX_SWITCH_STATUS: MUX_FW_AMUXBUSA_VMINUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_STATUS_MUX_FW_AMUXBUSB_VMINUS_Pos (21UL) /*!< SAR MUX_SWITCH_STATUS: MUX_FW_AMUXBUSB_VMINUS (Bit 21)    */
#define SAR_MUX_SWITCH_STATUS_MUX_FW_AMUXBUSB_VMINUS_Msk (0x200000UL) /*!< SAR MUX_SWITCH_STATUS: MUX_FW_AMUXBUSB_VMINUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_STATUS_MUX_FW_SARBUS0_VPLUS_Pos (22UL)   /*!< SAR MUX_SWITCH_STATUS: MUX_FW_SARBUS0_VPLUS (Bit 22)      */
#define SAR_MUX_SWITCH_STATUS_MUX_FW_SARBUS0_VPLUS_Msk (0x400000UL) /*!< SAR MUX_SWITCH_STATUS: MUX_FW_SARBUS0_VPLUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_STATUS_MUX_FW_SARBUS1_VPLUS_Pos (23UL)   /*!< SAR MUX_SWITCH_STATUS: MUX_FW_SARBUS1_VPLUS (Bit 23)      */
#define SAR_MUX_SWITCH_STATUS_MUX_FW_SARBUS1_VPLUS_Msk (0x800000UL) /*!< SAR MUX_SWITCH_STATUS: MUX_FW_SARBUS1_VPLUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_STATUS_MUX_FW_SARBUS0_VMINUS_Pos (24UL)  /*!< SAR MUX_SWITCH_STATUS: MUX_FW_SARBUS0_VMINUS (Bit 24)     */
#define SAR_MUX_SWITCH_STATUS_MUX_FW_SARBUS0_VMINUS_Msk (0x1000000UL) /*!< SAR MUX_SWITCH_STATUS: MUX_FW_SARBUS0_VMINUS (Bitfield-Mask: 0x01)*/
#define SAR_MUX_SWITCH_STATUS_MUX_FW_SARBUS1_VMINUS_Pos (25UL)  /*!< SAR MUX_SWITCH_STATUS: MUX_FW_SARBUS1_VMINUS (Bit 25)     */
#define SAR_MUX_SWITCH_STATUS_MUX_FW_SARBUS1_VMINUS_Msk (0x2000000UL) /*!< SAR MUX_SWITCH_STATUS: MUX_FW_SARBUS1_VMINUS (Bitfield-Mask: 0x01)*/
/* =====================================================  SAR.ANA_TRIM0  ===================================================== */
#define SAR_ANA_TRIM0_CAP_TRIM_Pos        (0UL)                 /*!< SAR ANA_TRIM0: CAP_TRIM (Bit 0)                           */
#define SAR_ANA_TRIM0_CAP_TRIM_Msk        (0x1fUL)              /*!< SAR ANA_TRIM0: CAP_TRIM (Bitfield-Mask: 0x1f)             */
#define SAR_ANA_TRIM0_TRIMUNIT_Pos        (5UL)                 /*!< SAR ANA_TRIM0: TRIMUNIT (Bit 5)                           */
#define SAR_ANA_TRIM0_TRIMUNIT_Msk        (0x20UL)              /*!< SAR ANA_TRIM0: TRIMUNIT (Bitfield-Mask: 0x01)             */
/* =====================================================  SAR.ANA_TRIM1  ===================================================== */
#define SAR_ANA_TRIM1_SAR_REF_BUF_TRIM_Pos (0UL)                /*!< SAR ANA_TRIM1: SAR_REF_BUF_TRIM (Bit 0)                   */
#define SAR_ANA_TRIM1_SAR_REF_BUF_TRIM_Msk (0x3fUL)             /*!< SAR ANA_TRIM1: SAR_REF_BUF_TRIM (Bitfield-Mask: 0x3f)     */


#endif /* _CYIP_SAR_H_ */


/* [] END OF FILE */
