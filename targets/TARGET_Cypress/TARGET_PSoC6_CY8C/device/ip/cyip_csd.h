/***************************************************************************//**
* \file cyip_csd.h
*
* \brief
* CSD IP definitions
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

#ifndef _CYIP_CSD_H_
#define _CYIP_CSD_H_

#include "cyip_headers.h"

/* =========================================================================================================================== */
/* ================                                            CSD                                            ================ */
/* =========================================================================================================================== */

#define CSD_SECTION_SIZE            0x00001000UL

/**
  * \brief Capsense Controller (CSD)
  */
typedef struct {                                /*!< CSD Structure                                                             */
  __IOM uint32_t CONFIG;                        /*!< 0x00000000 Configuration and Control                                      */
  __IOM uint32_t SPARE;                         /*!< 0x00000004 Spare MMIO                                                     */
   __IM uint32_t RESERVED[30];
   __IM uint32_t STATUS;                        /*!< 0x00000080 Status Register                                                */
   __IM uint32_t STAT_SEQ;                      /*!< 0x00000084 Current Sequencer status                                       */
   __IM uint32_t STAT_CNTS;                     /*!< 0x00000088 Current status counts                                          */
   __IM uint32_t RESERVED1[17];
   __IM uint32_t RESULT_VAL1;                   /*!< 0x000000D0 Result CSD/CSX accumulation counter value 1                    */
   __IM uint32_t RESULT_VAL2;                   /*!< 0x000000D4 Result CSX accumulation counter value 2                        */
   __IM uint32_t RESERVED2[2];
   __IM uint32_t ADC_RES;                       /*!< 0x000000E0 ADC measurement                                                */
   __IM uint32_t RESERVED3[3];
  __IOM uint32_t INTR;                          /*!< 0x000000F0 CSD Interrupt Request Register                                 */
  __IOM uint32_t INTR_SET;                      /*!< 0x000000F4 CSD Interrupt set register                                     */
  __IOM uint32_t INTR_MASK;                     /*!< 0x000000F8 CSD Interrupt mask register                                    */
   __IM uint32_t INTR_MASKED;                   /*!< 0x000000FC CSD Interrupt masked register                                  */
   __IM uint32_t RESERVED4[32];
  __IOM uint32_t HSCMP;                         /*!< 0x00000180 High Speed Comparator configuration                            */
  __IOM uint32_t AMBUF;                         /*!< 0x00000184 Reference Generator configuration                              */
  __IOM uint32_t REFGEN;                        /*!< 0x00000188 Reference Generator configuration                              */
  __IOM uint32_t CSDCMP;                        /*!< 0x0000018C CSD Comparator configuration                                   */
   __IM uint32_t RESERVED5[24];
  __IOM uint32_t SW_RES;                        /*!< 0x000001F0 Switch Resistance configuration                                */
   __IM uint32_t RESERVED6[3];
  __IOM uint32_t SENSE_PERIOD;                  /*!< 0x00000200 Sense clock period                                             */
  __IOM uint32_t SENSE_DUTY;                    /*!< 0x00000204 Sense clock duty cycle                                         */
   __IM uint32_t RESERVED7[30];
  __IOM uint32_t SW_HS_P_SEL;                   /*!< 0x00000280 HSCMP Pos input switch Waveform selection                      */
  __IOM uint32_t SW_HS_N_SEL;                   /*!< 0x00000284 HSCMP Neg input switch Waveform selection                      */
  __IOM uint32_t SW_SHIELD_SEL;                 /*!< 0x00000288 Shielding switches Waveform selection                          */
   __IM uint32_t RESERVED8;
  __IOM uint32_t SW_AMUXBUF_SEL;                /*!< 0x00000290 Amuxbuffer switches Waveform selection                         */
  __IOM uint32_t SW_BYP_SEL;                    /*!< 0x00000294 AMUXBUS bypass switches Waveform selection                     */
   __IM uint32_t RESERVED9[2];
  __IOM uint32_t SW_CMP_P_SEL;                  /*!< 0x000002A0 CSDCMP Pos Switch Waveform selection                           */
  __IOM uint32_t SW_CMP_N_SEL;                  /*!< 0x000002A4 CSDCMP Neg Switch Waveform selection                           */
  __IOM uint32_t SW_REFGEN_SEL;                 /*!< 0x000002A8 Reference Generator Switch Waveform selection                  */
   __IM uint32_t RESERVED10;
  __IOM uint32_t SW_FW_MOD_SEL;                 /*!< 0x000002B0 Full Wave Cmod Switch Waveform selection                       */
  __IOM uint32_t SW_FW_TANK_SEL;                /*!< 0x000002B4 Full Wave Csh_tank Switch Waveform selection                   */
   __IM uint32_t RESERVED11[2];
  __IOM uint32_t SW_DSI_SEL;                    /*!< 0x000002C0 DSI output switch control Waveform selection                   */
   __IM uint32_t RESERVED12[3];
  __IOM uint32_t IO_SEL;                        /*!< 0x000002D0 IO output control Waveform selection                           */
   __IM uint32_t RESERVED13[11];
  __IOM uint32_t SEQ_TIME;                      /*!< 0x00000300 Sequencer Timing                                               */
   __IM uint32_t RESERVED14[3];
  __IOM uint32_t SEQ_INIT_CNT;                  /*!< 0x00000310 Sequencer Initial conversion and sample counts                 */
  __IOM uint32_t SEQ_NORM_CNT;                  /*!< 0x00000314 Sequencer Normal conversion and sample counts                  */
   __IM uint32_t RESERVED15[2];
  __IOM uint32_t ADC_CTL;                       /*!< 0x00000320 ADC Control                                                    */
   __IM uint32_t RESERVED16[7];
  __IOM uint32_t SEQ_START;                     /*!< 0x00000340 Sequencer start                                                */
   __IM uint32_t RESERVED17[47];
  __IOM uint32_t IDACA;                         /*!< 0x00000400 IDACA Configuration                                            */
   __IM uint32_t RESERVED18[63];
  __IOM uint32_t IDACB;                         /*!< 0x00000500 IDACB Configuration                                            */
} CSD_Type;                                     /*!< Size = 1284 (0x504)                                                       */


/* ======================================================  CSD.CONFIG  ======================================================= */
#define CSD_CONFIG_IREF_SEL_Pos           (0UL)                 /*!< CSD CONFIG: IREF_SEL (Bit 0)                              */
#define CSD_CONFIG_IREF_SEL_Msk           (0x1UL)               /*!< CSD CONFIG: IREF_SEL (Bitfield-Mask: 0x01)                */
#define CSD_CONFIG_FILTER_DELAY_Pos       (4UL)                 /*!< CSD CONFIG: FILTER_DELAY (Bit 4)                          */
#define CSD_CONFIG_FILTER_DELAY_Msk       (0x1f0UL)             /*!< CSD CONFIG: FILTER_DELAY (Bitfield-Mask: 0x1f)            */
#define CSD_CONFIG_SHIELD_DELAY_Pos       (10UL)                /*!< CSD CONFIG: SHIELD_DELAY (Bit 10)                         */
#define CSD_CONFIG_SHIELD_DELAY_Msk       (0xc00UL)             /*!< CSD CONFIG: SHIELD_DELAY (Bitfield-Mask: 0x03)            */
#define CSD_CONFIG_SENSE_EN_Pos           (12UL)                /*!< CSD CONFIG: SENSE_EN (Bit 12)                             */
#define CSD_CONFIG_SENSE_EN_Msk           (0x1000UL)            /*!< CSD CONFIG: SENSE_EN (Bitfield-Mask: 0x01)                */
#define CSD_CONFIG_FULL_WAVE_Pos          (17UL)                /*!< CSD CONFIG: FULL_WAVE (Bit 17)                            */
#define CSD_CONFIG_FULL_WAVE_Msk          (0x20000UL)           /*!< CSD CONFIG: FULL_WAVE (Bitfield-Mask: 0x01)               */
#define CSD_CONFIG_MUTUAL_CAP_Pos         (18UL)                /*!< CSD CONFIG: MUTUAL_CAP (Bit 18)                           */
#define CSD_CONFIG_MUTUAL_CAP_Msk         (0x40000UL)           /*!< CSD CONFIG: MUTUAL_CAP (Bitfield-Mask: 0x01)              */
#define CSD_CONFIG_CSX_DUAL_CNT_Pos       (19UL)                /*!< CSD CONFIG: CSX_DUAL_CNT (Bit 19)                         */
#define CSD_CONFIG_CSX_DUAL_CNT_Msk       (0x80000UL)           /*!< CSD CONFIG: CSX_DUAL_CNT (Bitfield-Mask: 0x01)            */
#define CSD_CONFIG_DSI_COUNT_SEL_Pos      (24UL)                /*!< CSD CONFIG: DSI_COUNT_SEL (Bit 24)                        */
#define CSD_CONFIG_DSI_COUNT_SEL_Msk      (0x1000000UL)         /*!< CSD CONFIG: DSI_COUNT_SEL (Bitfield-Mask: 0x01)           */
#define CSD_CONFIG_DSI_SAMPLE_EN_Pos      (25UL)                /*!< CSD CONFIG: DSI_SAMPLE_EN (Bit 25)                        */
#define CSD_CONFIG_DSI_SAMPLE_EN_Msk      (0x2000000UL)         /*!< CSD CONFIG: DSI_SAMPLE_EN (Bitfield-Mask: 0x01)           */
#define CSD_CONFIG_SAMPLE_SYNC_Pos        (26UL)                /*!< CSD CONFIG: SAMPLE_SYNC (Bit 26)                          */
#define CSD_CONFIG_SAMPLE_SYNC_Msk        (0x4000000UL)         /*!< CSD CONFIG: SAMPLE_SYNC (Bitfield-Mask: 0x01)             */
#define CSD_CONFIG_DSI_SENSE_EN_Pos       (27UL)                /*!< CSD CONFIG: DSI_SENSE_EN (Bit 27)                         */
#define CSD_CONFIG_DSI_SENSE_EN_Msk       (0x8000000UL)         /*!< CSD CONFIG: DSI_SENSE_EN (Bitfield-Mask: 0x01)            */
#define CSD_CONFIG_LP_MODE_Pos            (30UL)                /*!< CSD CONFIG: LP_MODE (Bit 30)                              */
#define CSD_CONFIG_LP_MODE_Msk            (0x40000000UL)        /*!< CSD CONFIG: LP_MODE (Bitfield-Mask: 0x01)                 */
#define CSD_CONFIG_ENABLE_Pos             (31UL)                /*!< CSD CONFIG: ENABLE (Bit 31)                               */
#define CSD_CONFIG_ENABLE_Msk             (0x80000000UL)        /*!< CSD CONFIG: ENABLE (Bitfield-Mask: 0x01)                  */
/* =======================================================  CSD.SPARE  ======================================================= */
#define CSD_SPARE_SPARE_Pos               (0UL)                 /*!< CSD SPARE: SPARE (Bit 0)                                  */
#define CSD_SPARE_SPARE_Msk               (0xfUL)               /*!< CSD SPARE: SPARE (Bitfield-Mask: 0x0f)                    */
/* ======================================================  CSD.STATUS  ======================================================= */
#define CSD_STATUS_CSD_SENSE_Pos          (1UL)                 /*!< CSD STATUS: CSD_SENSE (Bit 1)                             */
#define CSD_STATUS_CSD_SENSE_Msk          (0x2UL)               /*!< CSD STATUS: CSD_SENSE (Bitfield-Mask: 0x01)               */
#define CSD_STATUS_HSCMP_OUT_Pos          (2UL)                 /*!< CSD STATUS: HSCMP_OUT (Bit 2)                             */
#define CSD_STATUS_HSCMP_OUT_Msk          (0x4UL)               /*!< CSD STATUS: HSCMP_OUT (Bitfield-Mask: 0x01)               */
#define CSD_STATUS_CSDCMP_OUT_Pos         (3UL)                 /*!< CSD STATUS: CSDCMP_OUT (Bit 3)                            */
#define CSD_STATUS_CSDCMP_OUT_Msk         (0x8UL)               /*!< CSD STATUS: CSDCMP_OUT (Bitfield-Mask: 0x01)              */
/* =====================================================  CSD.STAT_SEQ  ====================================================== */
#define CSD_STAT_SEQ_SEQ_STATE_Pos        (0UL)                 /*!< CSD STAT_SEQ: SEQ_STATE (Bit 0)                           */
#define CSD_STAT_SEQ_SEQ_STATE_Msk        (0x7UL)               /*!< CSD STAT_SEQ: SEQ_STATE (Bitfield-Mask: 0x07)             */
#define CSD_STAT_SEQ_ADC_STATE_Pos        (16UL)                /*!< CSD STAT_SEQ: ADC_STATE (Bit 16)                          */
#define CSD_STAT_SEQ_ADC_STATE_Msk        (0x70000UL)           /*!< CSD STAT_SEQ: ADC_STATE (Bitfield-Mask: 0x07)             */
/* =====================================================  CSD.STAT_CNTS  ===================================================== */
#define CSD_STAT_CNTS_NUM_CONV_Pos        (0UL)                 /*!< CSD STAT_CNTS: NUM_CONV (Bit 0)                           */
#define CSD_STAT_CNTS_NUM_CONV_Msk        (0xffffUL)            /*!< CSD STAT_CNTS: NUM_CONV (Bitfield-Mask: 0xffff)           */
/* ====================================================  CSD.RESULT_VAL1  ==================================================== */
#define CSD_RESULT_VAL1_VALUE_Pos         (0UL)                 /*!< CSD RESULT_VAL1: VALUE (Bit 0)                            */
#define CSD_RESULT_VAL1_VALUE_Msk         (0xffffUL)            /*!< CSD RESULT_VAL1: VALUE (Bitfield-Mask: 0xffff)            */
#define CSD_RESULT_VAL1_BAD_CONVS_Pos     (16UL)                /*!< CSD RESULT_VAL1: BAD_CONVS (Bit 16)                       */
#define CSD_RESULT_VAL1_BAD_CONVS_Msk     (0xff0000UL)          /*!< CSD RESULT_VAL1: BAD_CONVS (Bitfield-Mask: 0xff)          */
/* ====================================================  CSD.RESULT_VAL2  ==================================================== */
#define CSD_RESULT_VAL2_VALUE_Pos         (0UL)                 /*!< CSD RESULT_VAL2: VALUE (Bit 0)                            */
#define CSD_RESULT_VAL2_VALUE_Msk         (0xffffUL)            /*!< CSD RESULT_VAL2: VALUE (Bitfield-Mask: 0xffff)            */
/* ======================================================  CSD.ADC_RES  ====================================================== */
#define CSD_ADC_RES_VIN_CNT_Pos           (0UL)                 /*!< CSD ADC_RES: VIN_CNT (Bit 0)                              */
#define CSD_ADC_RES_VIN_CNT_Msk           (0xffffUL)            /*!< CSD ADC_RES: VIN_CNT (Bitfield-Mask: 0xffff)              */
#define CSD_ADC_RES_HSCMP_POL_Pos         (16UL)                /*!< CSD ADC_RES: HSCMP_POL (Bit 16)                           */
#define CSD_ADC_RES_HSCMP_POL_Msk         (0x10000UL)           /*!< CSD ADC_RES: HSCMP_POL (Bitfield-Mask: 0x01)              */
#define CSD_ADC_RES_ADC_OVERFLOW_Pos      (30UL)                /*!< CSD ADC_RES: ADC_OVERFLOW (Bit 30)                        */
#define CSD_ADC_RES_ADC_OVERFLOW_Msk      (0x40000000UL)        /*!< CSD ADC_RES: ADC_OVERFLOW (Bitfield-Mask: 0x01)           */
#define CSD_ADC_RES_ADC_ABORT_Pos         (31UL)                /*!< CSD ADC_RES: ADC_ABORT (Bit 31)                           */
#define CSD_ADC_RES_ADC_ABORT_Msk         (0x80000000UL)        /*!< CSD ADC_RES: ADC_ABORT (Bitfield-Mask: 0x01)              */
/* =======================================================  CSD.INTR  ======================================================== */
#define CSD_INTR_SAMPLE_Pos               (1UL)                 /*!< CSD INTR: SAMPLE (Bit 1)                                  */
#define CSD_INTR_SAMPLE_Msk               (0x2UL)               /*!< CSD INTR: SAMPLE (Bitfield-Mask: 0x01)                    */
#define CSD_INTR_INIT_Pos                 (2UL)                 /*!< CSD INTR: INIT (Bit 2)                                    */
#define CSD_INTR_INIT_Msk                 (0x4UL)               /*!< CSD INTR: INIT (Bitfield-Mask: 0x01)                      */
#define CSD_INTR_ADC_RES_Pos              (8UL)                 /*!< CSD INTR: ADC_RES (Bit 8)                                 */
#define CSD_INTR_ADC_RES_Msk              (0x100UL)             /*!< CSD INTR: ADC_RES (Bitfield-Mask: 0x01)                   */
/* =====================================================  CSD.INTR_SET  ====================================================== */
#define CSD_INTR_SET_SAMPLE_Pos           (1UL)                 /*!< CSD INTR_SET: SAMPLE (Bit 1)                              */
#define CSD_INTR_SET_SAMPLE_Msk           (0x2UL)               /*!< CSD INTR_SET: SAMPLE (Bitfield-Mask: 0x01)                */
#define CSD_INTR_SET_INIT_Pos             (2UL)                 /*!< CSD INTR_SET: INIT (Bit 2)                                */
#define CSD_INTR_SET_INIT_Msk             (0x4UL)               /*!< CSD INTR_SET: INIT (Bitfield-Mask: 0x01)                  */
#define CSD_INTR_SET_ADC_RES_Pos          (8UL)                 /*!< CSD INTR_SET: ADC_RES (Bit 8)                             */
#define CSD_INTR_SET_ADC_RES_Msk          (0x100UL)             /*!< CSD INTR_SET: ADC_RES (Bitfield-Mask: 0x01)               */
/* =====================================================  CSD.INTR_MASK  ===================================================== */
#define CSD_INTR_MASK_SAMPLE_Pos          (1UL)                 /*!< CSD INTR_MASK: SAMPLE (Bit 1)                             */
#define CSD_INTR_MASK_SAMPLE_Msk          (0x2UL)               /*!< CSD INTR_MASK: SAMPLE (Bitfield-Mask: 0x01)               */
#define CSD_INTR_MASK_INIT_Pos            (2UL)                 /*!< CSD INTR_MASK: INIT (Bit 2)                               */
#define CSD_INTR_MASK_INIT_Msk            (0x4UL)               /*!< CSD INTR_MASK: INIT (Bitfield-Mask: 0x01)                 */
#define CSD_INTR_MASK_ADC_RES_Pos         (8UL)                 /*!< CSD INTR_MASK: ADC_RES (Bit 8)                            */
#define CSD_INTR_MASK_ADC_RES_Msk         (0x100UL)             /*!< CSD INTR_MASK: ADC_RES (Bitfield-Mask: 0x01)              */
/* ====================================================  CSD.INTR_MASKED  ==================================================== */
#define CSD_INTR_MASKED_SAMPLE_Pos        (1UL)                 /*!< CSD INTR_MASKED: SAMPLE (Bit 1)                           */
#define CSD_INTR_MASKED_SAMPLE_Msk        (0x2UL)               /*!< CSD INTR_MASKED: SAMPLE (Bitfield-Mask: 0x01)             */
#define CSD_INTR_MASKED_INIT_Pos          (2UL)                 /*!< CSD INTR_MASKED: INIT (Bit 2)                             */
#define CSD_INTR_MASKED_INIT_Msk          (0x4UL)               /*!< CSD INTR_MASKED: INIT (Bitfield-Mask: 0x01)               */
#define CSD_INTR_MASKED_ADC_RES_Pos       (8UL)                 /*!< CSD INTR_MASKED: ADC_RES (Bit 8)                          */
#define CSD_INTR_MASKED_ADC_RES_Msk       (0x100UL)             /*!< CSD INTR_MASKED: ADC_RES (Bitfield-Mask: 0x01)            */
/* =======================================================  CSD.HSCMP  ======================================================= */
#define CSD_HSCMP_HSCMP_EN_Pos            (0UL)                 /*!< CSD HSCMP: HSCMP_EN (Bit 0)                               */
#define CSD_HSCMP_HSCMP_EN_Msk            (0x1UL)               /*!< CSD HSCMP: HSCMP_EN (Bitfield-Mask: 0x01)                 */
#define CSD_HSCMP_HSCMP_INVERT_Pos        (4UL)                 /*!< CSD HSCMP: HSCMP_INVERT (Bit 4)                           */
#define CSD_HSCMP_HSCMP_INVERT_Msk        (0x10UL)              /*!< CSD HSCMP: HSCMP_INVERT (Bitfield-Mask: 0x01)             */
#define CSD_HSCMP_AZ_EN_Pos               (31UL)                /*!< CSD HSCMP: AZ_EN (Bit 31)                                 */
#define CSD_HSCMP_AZ_EN_Msk               (0x80000000UL)        /*!< CSD HSCMP: AZ_EN (Bitfield-Mask: 0x01)                    */
/* =======================================================  CSD.AMBUF  ======================================================= */
#define CSD_AMBUF_PWR_MODE_Pos            (0UL)                 /*!< CSD AMBUF: PWR_MODE (Bit 0)                               */
#define CSD_AMBUF_PWR_MODE_Msk            (0x3UL)               /*!< CSD AMBUF: PWR_MODE (Bitfield-Mask: 0x03)                 */
/* ======================================================  CSD.REFGEN  ======================================================= */
#define CSD_REFGEN_REFGEN_EN_Pos          (0UL)                 /*!< CSD REFGEN: REFGEN_EN (Bit 0)                             */
#define CSD_REFGEN_REFGEN_EN_Msk          (0x1UL)               /*!< CSD REFGEN: REFGEN_EN (Bitfield-Mask: 0x01)               */
#define CSD_REFGEN_BYPASS_Pos             (4UL)                 /*!< CSD REFGEN: BYPASS (Bit 4)                                */
#define CSD_REFGEN_BYPASS_Msk             (0x10UL)              /*!< CSD REFGEN: BYPASS (Bitfield-Mask: 0x01)                  */
#define CSD_REFGEN_VDDA_EN_Pos            (5UL)                 /*!< CSD REFGEN: VDDA_EN (Bit 5)                               */
#define CSD_REFGEN_VDDA_EN_Msk            (0x20UL)              /*!< CSD REFGEN: VDDA_EN (Bitfield-Mask: 0x01)                 */
#define CSD_REFGEN_RES_EN_Pos             (6UL)                 /*!< CSD REFGEN: RES_EN (Bit 6)                                */
#define CSD_REFGEN_RES_EN_Msk             (0x40UL)              /*!< CSD REFGEN: RES_EN (Bitfield-Mask: 0x01)                  */
#define CSD_REFGEN_GAIN_Pos               (8UL)                 /*!< CSD REFGEN: GAIN (Bit 8)                                  */
#define CSD_REFGEN_GAIN_Msk               (0x1f00UL)            /*!< CSD REFGEN: GAIN (Bitfield-Mask: 0x1f)                    */
#define CSD_REFGEN_VREFLO_SEL_Pos         (16UL)                /*!< CSD REFGEN: VREFLO_SEL (Bit 16)                           */
#define CSD_REFGEN_VREFLO_SEL_Msk         (0x1f0000UL)          /*!< CSD REFGEN: VREFLO_SEL (Bitfield-Mask: 0x1f)              */
#define CSD_REFGEN_VREFLO_INT_Pos         (23UL)                /*!< CSD REFGEN: VREFLO_INT (Bit 23)                           */
#define CSD_REFGEN_VREFLO_INT_Msk         (0x800000UL)          /*!< CSD REFGEN: VREFLO_INT (Bitfield-Mask: 0x01)              */
/* ======================================================  CSD.CSDCMP  ======================================================= */
#define CSD_CSDCMP_CSDCMP_EN_Pos          (0UL)                 /*!< CSD CSDCMP: CSDCMP_EN (Bit 0)                             */
#define CSD_CSDCMP_CSDCMP_EN_Msk          (0x1UL)               /*!< CSD CSDCMP: CSDCMP_EN (Bitfield-Mask: 0x01)               */
#define CSD_CSDCMP_POLARITY_SEL_Pos       (4UL)                 /*!< CSD CSDCMP: POLARITY_SEL (Bit 4)                          */
#define CSD_CSDCMP_POLARITY_SEL_Msk       (0x30UL)              /*!< CSD CSDCMP: POLARITY_SEL (Bitfield-Mask: 0x03)            */
#define CSD_CSDCMP_CMP_PHASE_Pos          (8UL)                 /*!< CSD CSDCMP: CMP_PHASE (Bit 8)                             */
#define CSD_CSDCMP_CMP_PHASE_Msk          (0x300UL)             /*!< CSD CSDCMP: CMP_PHASE (Bitfield-Mask: 0x03)               */
#define CSD_CSDCMP_CMP_MODE_Pos           (28UL)                /*!< CSD CSDCMP: CMP_MODE (Bit 28)                             */
#define CSD_CSDCMP_CMP_MODE_Msk           (0x10000000UL)        /*!< CSD CSDCMP: CMP_MODE (Bitfield-Mask: 0x01)                */
#define CSD_CSDCMP_FEEDBACK_MODE_Pos      (29UL)                /*!< CSD CSDCMP: FEEDBACK_MODE (Bit 29)                        */
#define CSD_CSDCMP_FEEDBACK_MODE_Msk      (0x20000000UL)        /*!< CSD CSDCMP: FEEDBACK_MODE (Bitfield-Mask: 0x01)           */
#define CSD_CSDCMP_AZ_EN_Pos              (31UL)                /*!< CSD CSDCMP: AZ_EN (Bit 31)                                */
#define CSD_CSDCMP_AZ_EN_Msk              (0x80000000UL)        /*!< CSD CSDCMP: AZ_EN (Bitfield-Mask: 0x01)                   */
/* ======================================================  CSD.SW_RES  ======================================================= */
#define CSD_SW_RES_RES_HCAV_Pos           (0UL)                 /*!< CSD SW_RES: RES_HCAV (Bit 0)                              */
#define CSD_SW_RES_RES_HCAV_Msk           (0x3UL)               /*!< CSD SW_RES: RES_HCAV (Bitfield-Mask: 0x03)                */
#define CSD_SW_RES_RES_HCAG_Pos           (2UL)                 /*!< CSD SW_RES: RES_HCAG (Bit 2)                              */
#define CSD_SW_RES_RES_HCAG_Msk           (0xcUL)               /*!< CSD SW_RES: RES_HCAG (Bitfield-Mask: 0x03)                */
#define CSD_SW_RES_RES_HCBV_Pos           (4UL)                 /*!< CSD SW_RES: RES_HCBV (Bit 4)                              */
#define CSD_SW_RES_RES_HCBV_Msk           (0x30UL)              /*!< CSD SW_RES: RES_HCBV (Bitfield-Mask: 0x03)                */
#define CSD_SW_RES_RES_HCBG_Pos           (6UL)                 /*!< CSD SW_RES: RES_HCBG (Bit 6)                              */
#define CSD_SW_RES_RES_HCBG_Msk           (0xc0UL)              /*!< CSD SW_RES: RES_HCBG (Bitfield-Mask: 0x03)                */
#define CSD_SW_RES_RES_F1PM_Pos           (16UL)                /*!< CSD SW_RES: RES_F1PM (Bit 16)                             */
#define CSD_SW_RES_RES_F1PM_Msk           (0x30000UL)           /*!< CSD SW_RES: RES_F1PM (Bitfield-Mask: 0x03)                */
#define CSD_SW_RES_RES_F2PT_Pos           (18UL)                /*!< CSD SW_RES: RES_F2PT (Bit 18)                             */
#define CSD_SW_RES_RES_F2PT_Msk           (0xc0000UL)           /*!< CSD SW_RES: RES_F2PT (Bitfield-Mask: 0x03)                */
/* ===================================================  CSD.SENSE_PERIOD  ==================================================== */
#define CSD_SENSE_PERIOD_SENSE_DIV_Pos    (0UL)                 /*!< CSD SENSE_PERIOD: SENSE_DIV (Bit 0)                       */
#define CSD_SENSE_PERIOD_SENSE_DIV_Msk    (0xfffUL)             /*!< CSD SENSE_PERIOD: SENSE_DIV (Bitfield-Mask: 0xfff)        */
#define CSD_SENSE_PERIOD_LFSR_SIZE_Pos    (16UL)                /*!< CSD SENSE_PERIOD: LFSR_SIZE (Bit 16)                      */
#define CSD_SENSE_PERIOD_LFSR_SIZE_Msk    (0x70000UL)           /*!< CSD SENSE_PERIOD: LFSR_SIZE (Bitfield-Mask: 0x07)         */
#define CSD_SENSE_PERIOD_LFSR_SCALE_Pos   (20UL)                /*!< CSD SENSE_PERIOD: LFSR_SCALE (Bit 20)                     */
#define CSD_SENSE_PERIOD_LFSR_SCALE_Msk   (0xf00000UL)          /*!< CSD SENSE_PERIOD: LFSR_SCALE (Bitfield-Mask: 0x0f)        */
#define CSD_SENSE_PERIOD_LFSR_CLEAR_Pos   (24UL)                /*!< CSD SENSE_PERIOD: LFSR_CLEAR (Bit 24)                     */
#define CSD_SENSE_PERIOD_LFSR_CLEAR_Msk   (0x1000000UL)         /*!< CSD SENSE_PERIOD: LFSR_CLEAR (Bitfield-Mask: 0x01)        */
#define CSD_SENSE_PERIOD_SEL_LFSR_MSB_Pos (25UL)                /*!< CSD SENSE_PERIOD: SEL_LFSR_MSB (Bit 25)                   */
#define CSD_SENSE_PERIOD_SEL_LFSR_MSB_Msk (0x2000000UL)         /*!< CSD SENSE_PERIOD: SEL_LFSR_MSB (Bitfield-Mask: 0x01)      */
#define CSD_SENSE_PERIOD_LFSR_BITS_Pos    (26UL)                /*!< CSD SENSE_PERIOD: LFSR_BITS (Bit 26)                      */
#define CSD_SENSE_PERIOD_LFSR_BITS_Msk    (0xc000000UL)         /*!< CSD SENSE_PERIOD: LFSR_BITS (Bitfield-Mask: 0x03)         */
/* ====================================================  CSD.SENSE_DUTY  ===================================================== */
#define CSD_SENSE_DUTY_SENSE_WIDTH_Pos    (0UL)                 /*!< CSD SENSE_DUTY: SENSE_WIDTH (Bit 0)                       */
#define CSD_SENSE_DUTY_SENSE_WIDTH_Msk    (0xfffUL)             /*!< CSD SENSE_DUTY: SENSE_WIDTH (Bitfield-Mask: 0xfff)        */
#define CSD_SENSE_DUTY_SENSE_POL_Pos      (16UL)                /*!< CSD SENSE_DUTY: SENSE_POL (Bit 16)                        */
#define CSD_SENSE_DUTY_SENSE_POL_Msk      (0x10000UL)           /*!< CSD SENSE_DUTY: SENSE_POL (Bitfield-Mask: 0x01)           */
#define CSD_SENSE_DUTY_OVERLAP_PHI1_Pos   (18UL)                /*!< CSD SENSE_DUTY: OVERLAP_PHI1 (Bit 18)                     */
#define CSD_SENSE_DUTY_OVERLAP_PHI1_Msk   (0x40000UL)           /*!< CSD SENSE_DUTY: OVERLAP_PHI1 (Bitfield-Mask: 0x01)        */
#define CSD_SENSE_DUTY_OVERLAP_PHI2_Pos   (19UL)                /*!< CSD SENSE_DUTY: OVERLAP_PHI2 (Bit 19)                     */
#define CSD_SENSE_DUTY_OVERLAP_PHI2_Msk   (0x80000UL)           /*!< CSD SENSE_DUTY: OVERLAP_PHI2 (Bitfield-Mask: 0x01)        */
/* ====================================================  CSD.SW_HS_P_SEL  ==================================================== */
#define CSD_SW_HS_P_SEL_SW_HMPM_Pos       (0UL)                 /*!< CSD SW_HS_P_SEL: SW_HMPM (Bit 0)                          */
#define CSD_SW_HS_P_SEL_SW_HMPM_Msk       (0x1UL)               /*!< CSD SW_HS_P_SEL: SW_HMPM (Bitfield-Mask: 0x01)            */
#define CSD_SW_HS_P_SEL_SW_HMPT_Pos       (4UL)                 /*!< CSD SW_HS_P_SEL: SW_HMPT (Bit 4)                          */
#define CSD_SW_HS_P_SEL_SW_HMPT_Msk       (0x10UL)              /*!< CSD SW_HS_P_SEL: SW_HMPT (Bitfield-Mask: 0x01)            */
#define CSD_SW_HS_P_SEL_SW_HMPS_Pos       (8UL)                 /*!< CSD SW_HS_P_SEL: SW_HMPS (Bit 8)                          */
#define CSD_SW_HS_P_SEL_SW_HMPS_Msk       (0x100UL)             /*!< CSD SW_HS_P_SEL: SW_HMPS (Bitfield-Mask: 0x01)            */
#define CSD_SW_HS_P_SEL_SW_HMMA_Pos       (12UL)                /*!< CSD SW_HS_P_SEL: SW_HMMA (Bit 12)                         */
#define CSD_SW_HS_P_SEL_SW_HMMA_Msk       (0x1000UL)            /*!< CSD SW_HS_P_SEL: SW_HMMA (Bitfield-Mask: 0x01)            */
#define CSD_SW_HS_P_SEL_SW_HMMB_Pos       (16UL)                /*!< CSD SW_HS_P_SEL: SW_HMMB (Bit 16)                         */
#define CSD_SW_HS_P_SEL_SW_HMMB_Msk       (0x10000UL)           /*!< CSD SW_HS_P_SEL: SW_HMMB (Bitfield-Mask: 0x01)            */
#define CSD_SW_HS_P_SEL_SW_HMCA_Pos       (20UL)                /*!< CSD SW_HS_P_SEL: SW_HMCA (Bit 20)                         */
#define CSD_SW_HS_P_SEL_SW_HMCA_Msk       (0x100000UL)          /*!< CSD SW_HS_P_SEL: SW_HMCA (Bitfield-Mask: 0x01)            */
#define CSD_SW_HS_P_SEL_SW_HMCB_Pos       (24UL)                /*!< CSD SW_HS_P_SEL: SW_HMCB (Bit 24)                         */
#define CSD_SW_HS_P_SEL_SW_HMCB_Msk       (0x1000000UL)         /*!< CSD SW_HS_P_SEL: SW_HMCB (Bitfield-Mask: 0x01)            */
#define CSD_SW_HS_P_SEL_SW_HMRH_Pos       (28UL)                /*!< CSD SW_HS_P_SEL: SW_HMRH (Bit 28)                         */
#define CSD_SW_HS_P_SEL_SW_HMRH_Msk       (0x10000000UL)        /*!< CSD SW_HS_P_SEL: SW_HMRH (Bitfield-Mask: 0x01)            */
/* ====================================================  CSD.SW_HS_N_SEL  ==================================================== */
#define CSD_SW_HS_N_SEL_SW_HCCC_Pos       (16UL)                /*!< CSD SW_HS_N_SEL: SW_HCCC (Bit 16)                         */
#define CSD_SW_HS_N_SEL_SW_HCCC_Msk       (0x10000UL)           /*!< CSD SW_HS_N_SEL: SW_HCCC (Bitfield-Mask: 0x01)            */
#define CSD_SW_HS_N_SEL_SW_HCCD_Pos       (20UL)                /*!< CSD SW_HS_N_SEL: SW_HCCD (Bit 20)                         */
#define CSD_SW_HS_N_SEL_SW_HCCD_Msk       (0x100000UL)          /*!< CSD SW_HS_N_SEL: SW_HCCD (Bitfield-Mask: 0x01)            */
#define CSD_SW_HS_N_SEL_SW_HCRH_Pos       (24UL)                /*!< CSD SW_HS_N_SEL: SW_HCRH (Bit 24)                         */
#define CSD_SW_HS_N_SEL_SW_HCRH_Msk       (0x7000000UL)         /*!< CSD SW_HS_N_SEL: SW_HCRH (Bitfield-Mask: 0x07)            */
#define CSD_SW_HS_N_SEL_SW_HCRL_Pos       (28UL)                /*!< CSD SW_HS_N_SEL: SW_HCRL (Bit 28)                         */
#define CSD_SW_HS_N_SEL_SW_HCRL_Msk       (0x70000000UL)        /*!< CSD SW_HS_N_SEL: SW_HCRL (Bitfield-Mask: 0x07)            */
/* ===================================================  CSD.SW_SHIELD_SEL  =================================================== */
#define CSD_SW_SHIELD_SEL_SW_HCAV_Pos     (0UL)                 /*!< CSD SW_SHIELD_SEL: SW_HCAV (Bit 0)                        */
#define CSD_SW_SHIELD_SEL_SW_HCAV_Msk     (0x7UL)               /*!< CSD SW_SHIELD_SEL: SW_HCAV (Bitfield-Mask: 0x07)          */
#define CSD_SW_SHIELD_SEL_SW_HCAG_Pos     (4UL)                 /*!< CSD SW_SHIELD_SEL: SW_HCAG (Bit 4)                        */
#define CSD_SW_SHIELD_SEL_SW_HCAG_Msk     (0x70UL)              /*!< CSD SW_SHIELD_SEL: SW_HCAG (Bitfield-Mask: 0x07)          */
#define CSD_SW_SHIELD_SEL_SW_HCBV_Pos     (8UL)                 /*!< CSD SW_SHIELD_SEL: SW_HCBV (Bit 8)                        */
#define CSD_SW_SHIELD_SEL_SW_HCBV_Msk     (0x700UL)             /*!< CSD SW_SHIELD_SEL: SW_HCBV (Bitfield-Mask: 0x07)          */
#define CSD_SW_SHIELD_SEL_SW_HCBG_Pos     (12UL)                /*!< CSD SW_SHIELD_SEL: SW_HCBG (Bit 12)                       */
#define CSD_SW_SHIELD_SEL_SW_HCBG_Msk     (0x7000UL)            /*!< CSD SW_SHIELD_SEL: SW_HCBG (Bitfield-Mask: 0x07)          */
#define CSD_SW_SHIELD_SEL_SW_HCCV_Pos     (16UL)                /*!< CSD SW_SHIELD_SEL: SW_HCCV (Bit 16)                       */
#define CSD_SW_SHIELD_SEL_SW_HCCV_Msk     (0x10000UL)           /*!< CSD SW_SHIELD_SEL: SW_HCCV (Bitfield-Mask: 0x01)          */
#define CSD_SW_SHIELD_SEL_SW_HCCG_Pos     (20UL)                /*!< CSD SW_SHIELD_SEL: SW_HCCG (Bit 20)                       */
#define CSD_SW_SHIELD_SEL_SW_HCCG_Msk     (0x100000UL)          /*!< CSD SW_SHIELD_SEL: SW_HCCG (Bitfield-Mask: 0x01)          */
/* ==================================================  CSD.SW_AMUXBUF_SEL  =================================================== */
#define CSD_SW_AMUXBUF_SEL_SW_IRBY_Pos    (4UL)                 /*!< CSD SW_AMUXBUF_SEL: SW_IRBY (Bit 4)                       */
#define CSD_SW_AMUXBUF_SEL_SW_IRBY_Msk    (0x10UL)              /*!< CSD SW_AMUXBUF_SEL: SW_IRBY (Bitfield-Mask: 0x01)         */
#define CSD_SW_AMUXBUF_SEL_SW_IRLB_Pos    (8UL)                 /*!< CSD SW_AMUXBUF_SEL: SW_IRLB (Bit 8)                       */
#define CSD_SW_AMUXBUF_SEL_SW_IRLB_Msk    (0x100UL)             /*!< CSD SW_AMUXBUF_SEL: SW_IRLB (Bitfield-Mask: 0x01)         */
#define CSD_SW_AMUXBUF_SEL_SW_ICA_Pos     (12UL)                /*!< CSD SW_AMUXBUF_SEL: SW_ICA (Bit 12)                       */
#define CSD_SW_AMUXBUF_SEL_SW_ICA_Msk     (0x1000UL)            /*!< CSD SW_AMUXBUF_SEL: SW_ICA (Bitfield-Mask: 0x01)          */
#define CSD_SW_AMUXBUF_SEL_SW_ICB_Pos     (16UL)                /*!< CSD SW_AMUXBUF_SEL: SW_ICB (Bit 16)                       */
#define CSD_SW_AMUXBUF_SEL_SW_ICB_Msk     (0x70000UL)           /*!< CSD SW_AMUXBUF_SEL: SW_ICB (Bitfield-Mask: 0x07)          */
#define CSD_SW_AMUXBUF_SEL_SW_IRLI_Pos    (20UL)                /*!< CSD SW_AMUXBUF_SEL: SW_IRLI (Bit 20)                      */
#define CSD_SW_AMUXBUF_SEL_SW_IRLI_Msk    (0x100000UL)          /*!< CSD SW_AMUXBUF_SEL: SW_IRLI (Bitfield-Mask: 0x01)         */
#define CSD_SW_AMUXBUF_SEL_SW_IRH_Pos     (24UL)                /*!< CSD SW_AMUXBUF_SEL: SW_IRH (Bit 24)                       */
#define CSD_SW_AMUXBUF_SEL_SW_IRH_Msk     (0x1000000UL)         /*!< CSD SW_AMUXBUF_SEL: SW_IRH (Bitfield-Mask: 0x01)          */
#define CSD_SW_AMUXBUF_SEL_SW_IRL_Pos     (28UL)                /*!< CSD SW_AMUXBUF_SEL: SW_IRL (Bit 28)                       */
#define CSD_SW_AMUXBUF_SEL_SW_IRL_Msk     (0x10000000UL)        /*!< CSD SW_AMUXBUF_SEL: SW_IRL (Bitfield-Mask: 0x01)          */
/* ====================================================  CSD.SW_BYP_SEL  ===================================================== */
#define CSD_SW_BYP_SEL_SW_BYA_Pos         (12UL)                /*!< CSD SW_BYP_SEL: SW_BYA (Bit 12)                           */
#define CSD_SW_BYP_SEL_SW_BYA_Msk         (0x1000UL)            /*!< CSD SW_BYP_SEL: SW_BYA (Bitfield-Mask: 0x01)              */
#define CSD_SW_BYP_SEL_SW_BYB_Pos         (16UL)                /*!< CSD SW_BYP_SEL: SW_BYB (Bit 16)                           */
#define CSD_SW_BYP_SEL_SW_BYB_Msk         (0x10000UL)           /*!< CSD SW_BYP_SEL: SW_BYB (Bitfield-Mask: 0x01)              */
#define CSD_SW_BYP_SEL_SW_CBCC_Pos        (20UL)                /*!< CSD SW_BYP_SEL: SW_CBCC (Bit 20)                          */
#define CSD_SW_BYP_SEL_SW_CBCC_Msk        (0x100000UL)          /*!< CSD SW_BYP_SEL: SW_CBCC (Bitfield-Mask: 0x01)             */
/* ===================================================  CSD.SW_CMP_P_SEL  ==================================================== */
#define CSD_SW_CMP_P_SEL_SW_SFPM_Pos      (0UL)                 /*!< CSD SW_CMP_P_SEL: SW_SFPM (Bit 0)                         */
#define CSD_SW_CMP_P_SEL_SW_SFPM_Msk      (0x7UL)               /*!< CSD SW_CMP_P_SEL: SW_SFPM (Bitfield-Mask: 0x07)           */
#define CSD_SW_CMP_P_SEL_SW_SFPT_Pos      (4UL)                 /*!< CSD SW_CMP_P_SEL: SW_SFPT (Bit 4)                         */
#define CSD_SW_CMP_P_SEL_SW_SFPT_Msk      (0x70UL)              /*!< CSD SW_CMP_P_SEL: SW_SFPT (Bitfield-Mask: 0x07)           */
#define CSD_SW_CMP_P_SEL_SW_SFPS_Pos      (8UL)                 /*!< CSD SW_CMP_P_SEL: SW_SFPS (Bit 8)                         */
#define CSD_SW_CMP_P_SEL_SW_SFPS_Msk      (0x700UL)             /*!< CSD SW_CMP_P_SEL: SW_SFPS (Bitfield-Mask: 0x07)           */
#define CSD_SW_CMP_P_SEL_SW_SFMA_Pos      (12UL)                /*!< CSD SW_CMP_P_SEL: SW_SFMA (Bit 12)                        */
#define CSD_SW_CMP_P_SEL_SW_SFMA_Msk      (0x1000UL)            /*!< CSD SW_CMP_P_SEL: SW_SFMA (Bitfield-Mask: 0x01)           */
#define CSD_SW_CMP_P_SEL_SW_SFMB_Pos      (16UL)                /*!< CSD SW_CMP_P_SEL: SW_SFMB (Bit 16)                        */
#define CSD_SW_CMP_P_SEL_SW_SFMB_Msk      (0x10000UL)           /*!< CSD SW_CMP_P_SEL: SW_SFMB (Bitfield-Mask: 0x01)           */
#define CSD_SW_CMP_P_SEL_SW_SFCA_Pos      (20UL)                /*!< CSD SW_CMP_P_SEL: SW_SFCA (Bit 20)                        */
#define CSD_SW_CMP_P_SEL_SW_SFCA_Msk      (0x100000UL)          /*!< CSD SW_CMP_P_SEL: SW_SFCA (Bitfield-Mask: 0x01)           */
#define CSD_SW_CMP_P_SEL_SW_SFCB_Pos      (24UL)                /*!< CSD SW_CMP_P_SEL: SW_SFCB (Bit 24)                        */
#define CSD_SW_CMP_P_SEL_SW_SFCB_Msk      (0x1000000UL)         /*!< CSD SW_CMP_P_SEL: SW_SFCB (Bitfield-Mask: 0x01)           */
/* ===================================================  CSD.SW_CMP_N_SEL  ==================================================== */
#define CSD_SW_CMP_N_SEL_SW_SCRH_Pos      (24UL)                /*!< CSD SW_CMP_N_SEL: SW_SCRH (Bit 24)                        */
#define CSD_SW_CMP_N_SEL_SW_SCRH_Msk      (0x7000000UL)         /*!< CSD SW_CMP_N_SEL: SW_SCRH (Bitfield-Mask: 0x07)           */
#define CSD_SW_CMP_N_SEL_SW_SCRL_Pos      (28UL)                /*!< CSD SW_CMP_N_SEL: SW_SCRL (Bit 28)                        */
#define CSD_SW_CMP_N_SEL_SW_SCRL_Msk      (0x70000000UL)        /*!< CSD SW_CMP_N_SEL: SW_SCRL (Bitfield-Mask: 0x07)           */
/* ===================================================  CSD.SW_REFGEN_SEL  =================================================== */
#define CSD_SW_REFGEN_SEL_SW_IAIB_Pos     (0UL)                 /*!< CSD SW_REFGEN_SEL: SW_IAIB (Bit 0)                        */
#define CSD_SW_REFGEN_SEL_SW_IAIB_Msk     (0x1UL)               /*!< CSD SW_REFGEN_SEL: SW_IAIB (Bitfield-Mask: 0x01)          */
#define CSD_SW_REFGEN_SEL_SW_IBCB_Pos     (4UL)                 /*!< CSD SW_REFGEN_SEL: SW_IBCB (Bit 4)                        */
#define CSD_SW_REFGEN_SEL_SW_IBCB_Msk     (0x10UL)              /*!< CSD SW_REFGEN_SEL: SW_IBCB (Bitfield-Mask: 0x01)          */
#define CSD_SW_REFGEN_SEL_SW_SGMB_Pos     (16UL)                /*!< CSD SW_REFGEN_SEL: SW_SGMB (Bit 16)                       */
#define CSD_SW_REFGEN_SEL_SW_SGMB_Msk     (0x10000UL)           /*!< CSD SW_REFGEN_SEL: SW_SGMB (Bitfield-Mask: 0x01)          */
#define CSD_SW_REFGEN_SEL_SW_SGRP_Pos     (20UL)                /*!< CSD SW_REFGEN_SEL: SW_SGRP (Bit 20)                       */
#define CSD_SW_REFGEN_SEL_SW_SGRP_Msk     (0x100000UL)          /*!< CSD SW_REFGEN_SEL: SW_SGRP (Bitfield-Mask: 0x01)          */
#define CSD_SW_REFGEN_SEL_SW_SGRE_Pos     (24UL)                /*!< CSD SW_REFGEN_SEL: SW_SGRE (Bit 24)                       */
#define CSD_SW_REFGEN_SEL_SW_SGRE_Msk     (0x1000000UL)         /*!< CSD SW_REFGEN_SEL: SW_SGRE (Bitfield-Mask: 0x01)          */
#define CSD_SW_REFGEN_SEL_SW_SGR_Pos      (28UL)                /*!< CSD SW_REFGEN_SEL: SW_SGR (Bit 28)                        */
#define CSD_SW_REFGEN_SEL_SW_SGR_Msk      (0x10000000UL)        /*!< CSD SW_REFGEN_SEL: SW_SGR (Bitfield-Mask: 0x01)           */
/* ===================================================  CSD.SW_FW_MOD_SEL  =================================================== */
#define CSD_SW_FW_MOD_SEL_SW_F1PM_Pos     (0UL)                 /*!< CSD SW_FW_MOD_SEL: SW_F1PM (Bit 0)                        */
#define CSD_SW_FW_MOD_SEL_SW_F1PM_Msk     (0x1UL)               /*!< CSD SW_FW_MOD_SEL: SW_F1PM (Bitfield-Mask: 0x01)          */
#define CSD_SW_FW_MOD_SEL_SW_F1MA_Pos     (8UL)                 /*!< CSD SW_FW_MOD_SEL: SW_F1MA (Bit 8)                        */
#define CSD_SW_FW_MOD_SEL_SW_F1MA_Msk     (0x700UL)             /*!< CSD SW_FW_MOD_SEL: SW_F1MA (Bitfield-Mask: 0x07)          */
#define CSD_SW_FW_MOD_SEL_SW_F1CA_Pos     (16UL)                /*!< CSD SW_FW_MOD_SEL: SW_F1CA (Bit 16)                       */
#define CSD_SW_FW_MOD_SEL_SW_F1CA_Msk     (0x70000UL)           /*!< CSD SW_FW_MOD_SEL: SW_F1CA (Bitfield-Mask: 0x07)          */
#define CSD_SW_FW_MOD_SEL_SW_C1CC_Pos     (20UL)                /*!< CSD SW_FW_MOD_SEL: SW_C1CC (Bit 20)                       */
#define CSD_SW_FW_MOD_SEL_SW_C1CC_Msk     (0x100000UL)          /*!< CSD SW_FW_MOD_SEL: SW_C1CC (Bitfield-Mask: 0x01)          */
#define CSD_SW_FW_MOD_SEL_SW_C1CD_Pos     (24UL)                /*!< CSD SW_FW_MOD_SEL: SW_C1CD (Bit 24)                       */
#define CSD_SW_FW_MOD_SEL_SW_C1CD_Msk     (0x1000000UL)         /*!< CSD SW_FW_MOD_SEL: SW_C1CD (Bitfield-Mask: 0x01)          */
#define CSD_SW_FW_MOD_SEL_SW_C1F1_Pos     (28UL)                /*!< CSD SW_FW_MOD_SEL: SW_C1F1 (Bit 28)                       */
#define CSD_SW_FW_MOD_SEL_SW_C1F1_Msk     (0x10000000UL)        /*!< CSD SW_FW_MOD_SEL: SW_C1F1 (Bitfield-Mask: 0x01)          */
/* ==================================================  CSD.SW_FW_TANK_SEL  =================================================== */
#define CSD_SW_FW_TANK_SEL_SW_F2PT_Pos    (4UL)                 /*!< CSD SW_FW_TANK_SEL: SW_F2PT (Bit 4)                       */
#define CSD_SW_FW_TANK_SEL_SW_F2PT_Msk    (0x10UL)              /*!< CSD SW_FW_TANK_SEL: SW_F2PT (Bitfield-Mask: 0x01)         */
#define CSD_SW_FW_TANK_SEL_SW_F2MA_Pos    (8UL)                 /*!< CSD SW_FW_TANK_SEL: SW_F2MA (Bit 8)                       */
#define CSD_SW_FW_TANK_SEL_SW_F2MA_Msk    (0x700UL)             /*!< CSD SW_FW_TANK_SEL: SW_F2MA (Bitfield-Mask: 0x07)         */
#define CSD_SW_FW_TANK_SEL_SW_F2CA_Pos    (12UL)                /*!< CSD SW_FW_TANK_SEL: SW_F2CA (Bit 12)                      */
#define CSD_SW_FW_TANK_SEL_SW_F2CA_Msk    (0x7000UL)            /*!< CSD SW_FW_TANK_SEL: SW_F2CA (Bitfield-Mask: 0x07)         */
#define CSD_SW_FW_TANK_SEL_SW_F2CB_Pos    (16UL)                /*!< CSD SW_FW_TANK_SEL: SW_F2CB (Bit 16)                      */
#define CSD_SW_FW_TANK_SEL_SW_F2CB_Msk    (0x70000UL)           /*!< CSD SW_FW_TANK_SEL: SW_F2CB (Bitfield-Mask: 0x07)         */
#define CSD_SW_FW_TANK_SEL_SW_C2CC_Pos    (20UL)                /*!< CSD SW_FW_TANK_SEL: SW_C2CC (Bit 20)                      */
#define CSD_SW_FW_TANK_SEL_SW_C2CC_Msk    (0x100000UL)          /*!< CSD SW_FW_TANK_SEL: SW_C2CC (Bitfield-Mask: 0x01)         */
#define CSD_SW_FW_TANK_SEL_SW_C2CD_Pos    (24UL)                /*!< CSD SW_FW_TANK_SEL: SW_C2CD (Bit 24)                      */
#define CSD_SW_FW_TANK_SEL_SW_C2CD_Msk    (0x1000000UL)         /*!< CSD SW_FW_TANK_SEL: SW_C2CD (Bitfield-Mask: 0x01)         */
#define CSD_SW_FW_TANK_SEL_SW_C2F2_Pos    (28UL)                /*!< CSD SW_FW_TANK_SEL: SW_C2F2 (Bit 28)                      */
#define CSD_SW_FW_TANK_SEL_SW_C2F2_Msk    (0x10000000UL)        /*!< CSD SW_FW_TANK_SEL: SW_C2F2 (Bitfield-Mask: 0x01)         */
/* ====================================================  CSD.SW_DSI_SEL  ===================================================== */
#define CSD_SW_DSI_SEL_DSI_CSH_TANK_Pos   (0UL)                 /*!< CSD SW_DSI_SEL: DSI_CSH_TANK (Bit 0)                      */
#define CSD_SW_DSI_SEL_DSI_CSH_TANK_Msk   (0xfUL)               /*!< CSD SW_DSI_SEL: DSI_CSH_TANK (Bitfield-Mask: 0x0f)        */
#define CSD_SW_DSI_SEL_DSI_CMOD_Pos       (4UL)                 /*!< CSD SW_DSI_SEL: DSI_CMOD (Bit 4)                          */
#define CSD_SW_DSI_SEL_DSI_CMOD_Msk       (0xf0UL)              /*!< CSD SW_DSI_SEL: DSI_CMOD (Bitfield-Mask: 0x0f)            */
/* ======================================================  CSD.IO_SEL  ======================================================= */
#define CSD_IO_SEL_CSD_TX_OUT_Pos         (0UL)                 /*!< CSD IO_SEL: CSD_TX_OUT (Bit 0)                            */
#define CSD_IO_SEL_CSD_TX_OUT_Msk         (0xfUL)               /*!< CSD IO_SEL: CSD_TX_OUT (Bitfield-Mask: 0x0f)              */
#define CSD_IO_SEL_CSD_TX_OUT_EN_Pos      (4UL)                 /*!< CSD IO_SEL: CSD_TX_OUT_EN (Bit 4)                         */
#define CSD_IO_SEL_CSD_TX_OUT_EN_Msk      (0xf0UL)              /*!< CSD IO_SEL: CSD_TX_OUT_EN (Bitfield-Mask: 0x0f)           */
#define CSD_IO_SEL_CSD_TX_AMUXB_EN_Pos    (12UL)                /*!< CSD IO_SEL: CSD_TX_AMUXB_EN (Bit 12)                      */
#define CSD_IO_SEL_CSD_TX_AMUXB_EN_Msk    (0xf000UL)            /*!< CSD IO_SEL: CSD_TX_AMUXB_EN (Bitfield-Mask: 0x0f)         */
#define CSD_IO_SEL_CSD_TX_N_OUT_Pos       (16UL)                /*!< CSD IO_SEL: CSD_TX_N_OUT (Bit 16)                         */
#define CSD_IO_SEL_CSD_TX_N_OUT_Msk       (0xf0000UL)           /*!< CSD IO_SEL: CSD_TX_N_OUT (Bitfield-Mask: 0x0f)            */
#define CSD_IO_SEL_CSD_TX_N_OUT_EN_Pos    (20UL)                /*!< CSD IO_SEL: CSD_TX_N_OUT_EN (Bit 20)                      */
#define CSD_IO_SEL_CSD_TX_N_OUT_EN_Msk    (0xf00000UL)          /*!< CSD IO_SEL: CSD_TX_N_OUT_EN (Bitfield-Mask: 0x0f)         */
#define CSD_IO_SEL_CSD_TX_N_AMUXA_EN_Pos  (24UL)                /*!< CSD IO_SEL: CSD_TX_N_AMUXA_EN (Bit 24)                    */
#define CSD_IO_SEL_CSD_TX_N_AMUXA_EN_Msk  (0xf000000UL)         /*!< CSD IO_SEL: CSD_TX_N_AMUXA_EN (Bitfield-Mask: 0x0f)       */
/* =====================================================  CSD.SEQ_TIME  ====================================================== */
#define CSD_SEQ_TIME_AZ_TIME_Pos          (0UL)                 /*!< CSD SEQ_TIME: AZ_TIME (Bit 0)                             */
#define CSD_SEQ_TIME_AZ_TIME_Msk          (0xffUL)              /*!< CSD SEQ_TIME: AZ_TIME (Bitfield-Mask: 0xff)               */
/* ===================================================  CSD.SEQ_INIT_CNT  ==================================================== */
#define CSD_SEQ_INIT_CNT_CONV_CNT_Pos     (0UL)                 /*!< CSD SEQ_INIT_CNT: CONV_CNT (Bit 0)                        */
#define CSD_SEQ_INIT_CNT_CONV_CNT_Msk     (0xffffUL)            /*!< CSD SEQ_INIT_CNT: CONV_CNT (Bitfield-Mask: 0xffff)        */
/* ===================================================  CSD.SEQ_NORM_CNT  ==================================================== */
#define CSD_SEQ_NORM_CNT_CONV_CNT_Pos     (0UL)                 /*!< CSD SEQ_NORM_CNT: CONV_CNT (Bit 0)                        */
#define CSD_SEQ_NORM_CNT_CONV_CNT_Msk     (0xffffUL)            /*!< CSD SEQ_NORM_CNT: CONV_CNT (Bitfield-Mask: 0xffff)        */
/* ======================================================  CSD.ADC_CTL  ====================================================== */
#define CSD_ADC_CTL_ADC_TIME_Pos          (0UL)                 /*!< CSD ADC_CTL: ADC_TIME (Bit 0)                             */
#define CSD_ADC_CTL_ADC_TIME_Msk          (0xffUL)              /*!< CSD ADC_CTL: ADC_TIME (Bitfield-Mask: 0xff)               */
#define CSD_ADC_CTL_ADC_MODE_Pos          (16UL)                /*!< CSD ADC_CTL: ADC_MODE (Bit 16)                            */
#define CSD_ADC_CTL_ADC_MODE_Msk          (0x30000UL)           /*!< CSD ADC_CTL: ADC_MODE (Bitfield-Mask: 0x03)               */
/* =====================================================  CSD.SEQ_START  ===================================================== */
#define CSD_SEQ_START_START_Pos           (0UL)                 /*!< CSD SEQ_START: START (Bit 0)                              */
#define CSD_SEQ_START_START_Msk           (0x1UL)               /*!< CSD SEQ_START: START (Bitfield-Mask: 0x01)                */
#define CSD_SEQ_START_SEQ_MODE_Pos        (1UL)                 /*!< CSD SEQ_START: SEQ_MODE (Bit 1)                           */
#define CSD_SEQ_START_SEQ_MODE_Msk        (0x2UL)               /*!< CSD SEQ_START: SEQ_MODE (Bitfield-Mask: 0x01)             */
#define CSD_SEQ_START_ABORT_Pos           (3UL)                 /*!< CSD SEQ_START: ABORT (Bit 3)                              */
#define CSD_SEQ_START_ABORT_Msk           (0x8UL)               /*!< CSD SEQ_START: ABORT (Bitfield-Mask: 0x01)                */
#define CSD_SEQ_START_DSI_START_EN_Pos    (4UL)                 /*!< CSD SEQ_START: DSI_START_EN (Bit 4)                       */
#define CSD_SEQ_START_DSI_START_EN_Msk    (0x10UL)              /*!< CSD SEQ_START: DSI_START_EN (Bitfield-Mask: 0x01)         */
#define CSD_SEQ_START_AZ0_SKIP_Pos        (8UL)                 /*!< CSD SEQ_START: AZ0_SKIP (Bit 8)                           */
#define CSD_SEQ_START_AZ0_SKIP_Msk        (0x100UL)             /*!< CSD SEQ_START: AZ0_SKIP (Bitfield-Mask: 0x01)             */
#define CSD_SEQ_START_AZ1_SKIP_Pos        (9UL)                 /*!< CSD SEQ_START: AZ1_SKIP (Bit 9)                           */
#define CSD_SEQ_START_AZ1_SKIP_Msk        (0x200UL)             /*!< CSD SEQ_START: AZ1_SKIP (Bitfield-Mask: 0x01)             */
/* =======================================================  CSD.IDACA  ======================================================= */
#define CSD_IDACA_VAL_Pos                 (0UL)                 /*!< CSD IDACA: VAL (Bit 0)                                    */
#define CSD_IDACA_VAL_Msk                 (0x7fUL)              /*!< CSD IDACA: VAL (Bitfield-Mask: 0x7f)                      */
#define CSD_IDACA_POL_DYN_Pos             (7UL)                 /*!< CSD IDACA: POL_DYN (Bit 7)                                */
#define CSD_IDACA_POL_DYN_Msk             (0x80UL)              /*!< CSD IDACA: POL_DYN (Bitfield-Mask: 0x01)                  */
#define CSD_IDACA_POLARITY_Pos            (8UL)                 /*!< CSD IDACA: POLARITY (Bit 8)                               */
#define CSD_IDACA_POLARITY_Msk            (0x300UL)             /*!< CSD IDACA: POLARITY (Bitfield-Mask: 0x03)                 */
#define CSD_IDACA_BAL_MODE_Pos            (10UL)                /*!< CSD IDACA: BAL_MODE (Bit 10)                              */
#define CSD_IDACA_BAL_MODE_Msk            (0xc00UL)             /*!< CSD IDACA: BAL_MODE (Bitfield-Mask: 0x03)                 */
#define CSD_IDACA_LEG1_MODE_Pos           (16UL)                /*!< CSD IDACA: LEG1_MODE (Bit 16)                             */
#define CSD_IDACA_LEG1_MODE_Msk           (0x30000UL)           /*!< CSD IDACA: LEG1_MODE (Bitfield-Mask: 0x03)                */
#define CSD_IDACA_LEG2_MODE_Pos           (18UL)                /*!< CSD IDACA: LEG2_MODE (Bit 18)                             */
#define CSD_IDACA_LEG2_MODE_Msk           (0xc0000UL)           /*!< CSD IDACA: LEG2_MODE (Bitfield-Mask: 0x03)                */
#define CSD_IDACA_DSI_CTRL_EN_Pos         (21UL)                /*!< CSD IDACA: DSI_CTRL_EN (Bit 21)                           */
#define CSD_IDACA_DSI_CTRL_EN_Msk         (0x200000UL)          /*!< CSD IDACA: DSI_CTRL_EN (Bitfield-Mask: 0x01)              */
#define CSD_IDACA_RANGE_Pos               (22UL)                /*!< CSD IDACA: RANGE (Bit 22)                                 */
#define CSD_IDACA_RANGE_Msk               (0xc00000UL)          /*!< CSD IDACA: RANGE (Bitfield-Mask: 0x03)                    */
#define CSD_IDACA_LEG1_EN_Pos             (24UL)                /*!< CSD IDACA: LEG1_EN (Bit 24)                               */
#define CSD_IDACA_LEG1_EN_Msk             (0x1000000UL)         /*!< CSD IDACA: LEG1_EN (Bitfield-Mask: 0x01)                  */
#define CSD_IDACA_LEG2_EN_Pos             (25UL)                /*!< CSD IDACA: LEG2_EN (Bit 25)                               */
#define CSD_IDACA_LEG2_EN_Msk             (0x2000000UL)         /*!< CSD IDACA: LEG2_EN (Bitfield-Mask: 0x01)                  */
/* =======================================================  CSD.IDACB  ======================================================= */
#define CSD_IDACB_VAL_Pos                 (0UL)                 /*!< CSD IDACB: VAL (Bit 0)                                    */
#define CSD_IDACB_VAL_Msk                 (0x7fUL)              /*!< CSD IDACB: VAL (Bitfield-Mask: 0x7f)                      */
#define CSD_IDACB_POL_DYN_Pos             (7UL)                 /*!< CSD IDACB: POL_DYN (Bit 7)                                */
#define CSD_IDACB_POL_DYN_Msk             (0x80UL)              /*!< CSD IDACB: POL_DYN (Bitfield-Mask: 0x01)                  */
#define CSD_IDACB_POLARITY_Pos            (8UL)                 /*!< CSD IDACB: POLARITY (Bit 8)                               */
#define CSD_IDACB_POLARITY_Msk            (0x300UL)             /*!< CSD IDACB: POLARITY (Bitfield-Mask: 0x03)                 */
#define CSD_IDACB_BAL_MODE_Pos            (10UL)                /*!< CSD IDACB: BAL_MODE (Bit 10)                              */
#define CSD_IDACB_BAL_MODE_Msk            (0xc00UL)             /*!< CSD IDACB: BAL_MODE (Bitfield-Mask: 0x03)                 */
#define CSD_IDACB_LEG1_MODE_Pos           (16UL)                /*!< CSD IDACB: LEG1_MODE (Bit 16)                             */
#define CSD_IDACB_LEG1_MODE_Msk           (0x30000UL)           /*!< CSD IDACB: LEG1_MODE (Bitfield-Mask: 0x03)                */
#define CSD_IDACB_LEG2_MODE_Pos           (18UL)                /*!< CSD IDACB: LEG2_MODE (Bit 18)                             */
#define CSD_IDACB_LEG2_MODE_Msk           (0xc0000UL)           /*!< CSD IDACB: LEG2_MODE (Bitfield-Mask: 0x03)                */
#define CSD_IDACB_DSI_CTRL_EN_Pos         (21UL)                /*!< CSD IDACB: DSI_CTRL_EN (Bit 21)                           */
#define CSD_IDACB_DSI_CTRL_EN_Msk         (0x200000UL)          /*!< CSD IDACB: DSI_CTRL_EN (Bitfield-Mask: 0x01)              */
#define CSD_IDACB_RANGE_Pos               (22UL)                /*!< CSD IDACB: RANGE (Bit 22)                                 */
#define CSD_IDACB_RANGE_Msk               (0xc00000UL)          /*!< CSD IDACB: RANGE (Bitfield-Mask: 0x03)                    */
#define CSD_IDACB_LEG1_EN_Pos             (24UL)                /*!< CSD IDACB: LEG1_EN (Bit 24)                               */
#define CSD_IDACB_LEG1_EN_Msk             (0x1000000UL)         /*!< CSD IDACB: LEG1_EN (Bitfield-Mask: 0x01)                  */
#define CSD_IDACB_LEG2_EN_Pos             (25UL)                /*!< CSD IDACB: LEG2_EN (Bit 25)                               */
#define CSD_IDACB_LEG2_EN_Msk             (0x2000000UL)         /*!< CSD IDACB: LEG2_EN (Bitfield-Mask: 0x01)                  */
#define CSD_IDACB_LEG3_EN_Pos             (26UL)                /*!< CSD IDACB: LEG3_EN (Bit 26)                               */
#define CSD_IDACB_LEG3_EN_Msk             (0x4000000UL)         /*!< CSD IDACB: LEG3_EN (Bitfield-Mask: 0x01)                  */


#endif /* _CYIP_CSD_H_ */


/* [] END OF FILE */
