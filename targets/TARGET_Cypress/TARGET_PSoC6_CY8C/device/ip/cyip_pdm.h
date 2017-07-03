/***************************************************************************//**
* \file cyip_pdm.h
*
* \brief
* PDM IP definitions
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

#ifndef _CYIP_PDM_H_
#define _CYIP_PDM_H_

#include "cyip_headers.h"

/* =========================================================================================================================== */
/* ================                                            PDM                                            ================ */
/* =========================================================================================================================== */

#define PDM_SECTION_SIZE            0x00001000UL

/**
  * \brief PDM registers (PDM)
  */
typedef struct {                                /*!< PDM Structure                                                             */
  __IOM uint32_t CTL;                           /*!< 0x00000000 Control                                                        */
   __IM uint32_t RESERVED[3];
  __IOM uint32_t CLOCK_CTL;                     /*!< 0x00000010 Clock control                                                  */
  __IOM uint32_t MODE_CTL;                      /*!< 0x00000014 Mode control                                                   */
  __IOM uint32_t DATA_CTL;                      /*!< 0x00000018 Data control                                                   */
   __IM uint32_t RESERVED1;
  __IOM uint32_t CMD;                           /*!< 0x00000020 Command                                                        */
   __IM uint32_t RESERVED2[7];
  __IOM uint32_t TR_CTL;                        /*!< 0x00000040 Trigger control                                                */
   __IM uint32_t RESERVED3[175];
  __IOM uint32_t RX_FIFO_CTL;                   /*!< 0x00000300 RX FIFO control                                                */
   __IM uint32_t RX_FIFO_STATUS;                /*!< 0x00000304 RX FIFO status                                                 */
   __IM uint32_t RX_FIFO_RD;                    /*!< 0x00000308 RX FIFO read                                                   */
   __IM uint32_t RX_FIFO_RD_SILENT;             /*!< 0x0000030C RX FIFO silent read                                            */
   __IM uint32_t RESERVED4[764];
  __IOM uint32_t INTR;                          /*!< 0x00000F00 Interrupt register                                             */
  __IOM uint32_t INTR_SET;                      /*!< 0x00000F04 Interrupt set register                                         */
  __IOM uint32_t INTR_MASK;                     /*!< 0x00000F08 Interrupt mask register                                        */
   __IM uint32_t INTR_MASKED;                   /*!< 0x00000F0C Interrupt masked register                                      */
} PDM_Type;                                     /*!< Size = 3856 (0xF10)                                                       */


/* ========================================================  PDM.CTL  ======================================================== */
#define PDM_CTL_PGA_R_Pos                 (0UL)                 /*!< PDM CTL: PGA_R (Bit 0)                                    */
#define PDM_CTL_PGA_R_Msk                 (0xfUL)               /*!< PDM CTL: PGA_R (Bitfield-Mask: 0x0f)                      */
#define PDM_CTL_PGA_L_Pos                 (8UL)                 /*!< PDM CTL: PGA_L (Bit 8)                                    */
#define PDM_CTL_PGA_L_Msk                 (0xf00UL)             /*!< PDM CTL: PGA_L (Bitfield-Mask: 0x0f)                      */
#define PDM_CTL_SOFT_MUTE_Pos             (16UL)                /*!< PDM CTL: SOFT_MUTE (Bit 16)                               */
#define PDM_CTL_SOFT_MUTE_Msk             (0x10000UL)           /*!< PDM CTL: SOFT_MUTE (Bitfield-Mask: 0x01)                  */
#define PDM_CTL_STEP_SEL_Pos              (17UL)                /*!< PDM CTL: STEP_SEL (Bit 17)                                */
#define PDM_CTL_STEP_SEL_Msk              (0x20000UL)           /*!< PDM CTL: STEP_SEL (Bitfield-Mask: 0x01)                   */
#define PDM_CTL_ENABLED_Pos               (31UL)                /*!< PDM CTL: ENABLED (Bit 31)                                 */
#define PDM_CTL_ENABLED_Msk               (0x80000000UL)        /*!< PDM CTL: ENABLED (Bitfield-Mask: 0x01)                    */
/* =====================================================  PDM.CLOCK_CTL  ===================================================== */
#define PDM_CLOCK_CTL_CLK_CLOCK_DIV_Pos   (0UL)                 /*!< PDM CLOCK_CTL: CLK_CLOCK_DIV (Bit 0)                      */
#define PDM_CLOCK_CTL_CLK_CLOCK_DIV_Msk   (0x3UL)               /*!< PDM CLOCK_CTL: CLK_CLOCK_DIV (Bitfield-Mask: 0x03)        */
#define PDM_CLOCK_CTL_MCLKQ_CLOCK_DIV_Pos (4UL)                 /*!< PDM CLOCK_CTL: MCLKQ_CLOCK_DIV (Bit 4)                    */
#define PDM_CLOCK_CTL_MCLKQ_CLOCK_DIV_Msk (0x30UL)              /*!< PDM CLOCK_CTL: MCLKQ_CLOCK_DIV (Bitfield-Mask: 0x03)      */
#define PDM_CLOCK_CTL_CKO_CLOCK_DIV_Pos   (8UL)                 /*!< PDM CLOCK_CTL: CKO_CLOCK_DIV (Bit 8)                      */
#define PDM_CLOCK_CTL_CKO_CLOCK_DIV_Msk   (0xf00UL)             /*!< PDM CLOCK_CTL: CKO_CLOCK_DIV (Bitfield-Mask: 0x0f)        */
#define PDM_CLOCK_CTL_SINC_RATE_Pos       (16UL)                /*!< PDM CLOCK_CTL: SINC_RATE (Bit 16)                         */
#define PDM_CLOCK_CTL_SINC_RATE_Msk       (0x7f0000UL)          /*!< PDM CLOCK_CTL: SINC_RATE (Bitfield-Mask: 0x7f)            */
/* =====================================================  PDM.MODE_CTL  ====================================================== */
#define PDM_MODE_CTL_PCM_CH_SET_Pos       (0UL)                 /*!< PDM MODE_CTL: PCM_CH_SET (Bit 0)                          */
#define PDM_MODE_CTL_PCM_CH_SET_Msk       (0x3UL)               /*!< PDM MODE_CTL: PCM_CH_SET (Bitfield-Mask: 0x03)            */
#define PDM_MODE_CTL_SWAP_LR_Pos          (2UL)                 /*!< PDM MODE_CTL: SWAP_LR (Bit 2)                             */
#define PDM_MODE_CTL_SWAP_LR_Msk          (0x4UL)               /*!< PDM MODE_CTL: SWAP_LR (Bitfield-Mask: 0x01)               */
#define PDM_MODE_CTL_S_CYCLES_Pos         (8UL)                 /*!< PDM MODE_CTL: S_CYCLES (Bit 8)                            */
#define PDM_MODE_CTL_S_CYCLES_Msk         (0x700UL)             /*!< PDM MODE_CTL: S_CYCLES (Bitfield-Mask: 0x07)              */
#define PDM_MODE_CTL_CKO_DELAY_Pos        (16UL)                /*!< PDM MODE_CTL: CKO_DELAY (Bit 16)                          */
#define PDM_MODE_CTL_CKO_DELAY_Msk        (0x70000UL)           /*!< PDM MODE_CTL: CKO_DELAY (Bitfield-Mask: 0x07)             */
#define PDM_MODE_CTL_HPF_GAIN_Pos         (24UL)                /*!< PDM MODE_CTL: HPF_GAIN (Bit 24)                           */
#define PDM_MODE_CTL_HPF_GAIN_Msk         (0xf000000UL)         /*!< PDM MODE_CTL: HPF_GAIN (Bitfield-Mask: 0x0f)              */
#define PDM_MODE_CTL_HPF_EN_N_Pos         (28UL)                /*!< PDM MODE_CTL: HPF_EN_N (Bit 28)                           */
#define PDM_MODE_CTL_HPF_EN_N_Msk         (0x10000000UL)        /*!< PDM MODE_CTL: HPF_EN_N (Bitfield-Mask: 0x01)              */
/* =====================================================  PDM.DATA_CTL  ====================================================== */
#define PDM_DATA_CTL_WORD_LEN_Pos         (0UL)                 /*!< PDM DATA_CTL: WORD_LEN (Bit 0)                            */
#define PDM_DATA_CTL_WORD_LEN_Msk         (0x3UL)               /*!< PDM DATA_CTL: WORD_LEN (Bitfield-Mask: 0x03)              */
#define PDM_DATA_CTL_BIT_EXTENSION_Pos    (8UL)                 /*!< PDM DATA_CTL: BIT_EXTENSION (Bit 8)                       */
#define PDM_DATA_CTL_BIT_EXTENSION_Msk    (0x100UL)             /*!< PDM DATA_CTL: BIT_EXTENSION (Bitfield-Mask: 0x01)         */
/* ========================================================  PDM.CMD  ======================================================== */
#define PDM_CMD_STREAM_EN_Pos             (0UL)                 /*!< PDM CMD: STREAM_EN (Bit 0)                                */
#define PDM_CMD_STREAM_EN_Msk             (0x1UL)               /*!< PDM CMD: STREAM_EN (Bitfield-Mask: 0x01)                  */
/* ======================================================  PDM.TR_CTL  ======================================================= */
#define PDM_TR_CTL_RX_REQ_EN_Pos          (16UL)                /*!< PDM TR_CTL: RX_REQ_EN (Bit 16)                            */
#define PDM_TR_CTL_RX_REQ_EN_Msk          (0x10000UL)           /*!< PDM TR_CTL: RX_REQ_EN (Bitfield-Mask: 0x01)               */
/* ====================================================  PDM.RX_FIFO_CTL  ==================================================== */
#define PDM_RX_FIFO_CTL_TRIGGER_LEVEL_Pos (0UL)                 /*!< PDM RX_FIFO_CTL: TRIGGER_LEVEL (Bit 0)                    */
#define PDM_RX_FIFO_CTL_TRIGGER_LEVEL_Msk (0xffUL)              /*!< PDM RX_FIFO_CTL: TRIGGER_LEVEL (Bitfield-Mask: 0xff)      */
#define PDM_RX_FIFO_CTL_CLEAR_Pos         (16UL)                /*!< PDM RX_FIFO_CTL: CLEAR (Bit 16)                           */
#define PDM_RX_FIFO_CTL_CLEAR_Msk         (0x10000UL)           /*!< PDM RX_FIFO_CTL: CLEAR (Bitfield-Mask: 0x01)              */
#define PDM_RX_FIFO_CTL_FREEZE_Pos        (17UL)                /*!< PDM RX_FIFO_CTL: FREEZE (Bit 17)                          */
#define PDM_RX_FIFO_CTL_FREEZE_Msk        (0x20000UL)           /*!< PDM RX_FIFO_CTL: FREEZE (Bitfield-Mask: 0x01)             */
/* ==================================================  PDM.RX_FIFO_STATUS  =================================================== */
#define PDM_RX_FIFO_STATUS_USED_Pos       (0UL)                 /*!< PDM RX_FIFO_STATUS: USED (Bit 0)                          */
#define PDM_RX_FIFO_STATUS_USED_Msk       (0x1ffUL)             /*!< PDM RX_FIFO_STATUS: USED (Bitfield-Mask: 0x1ff)           */
#define PDM_RX_FIFO_STATUS_RD_PTR_Pos     (16UL)                /*!< PDM RX_FIFO_STATUS: RD_PTR (Bit 16)                       */
#define PDM_RX_FIFO_STATUS_RD_PTR_Msk     (0xff0000UL)          /*!< PDM RX_FIFO_STATUS: RD_PTR (Bitfield-Mask: 0xff)          */
#define PDM_RX_FIFO_STATUS_WR_PTR_Pos     (24UL)                /*!< PDM RX_FIFO_STATUS: WR_PTR (Bit 24)                       */
#define PDM_RX_FIFO_STATUS_WR_PTR_Msk     (0xff000000UL)        /*!< PDM RX_FIFO_STATUS: WR_PTR (Bitfield-Mask: 0xff)          */
/* ====================================================  PDM.RX_FIFO_RD  ===================================================== */
#define PDM_RX_FIFO_RD_DATA_Pos           (0UL)                 /*!< PDM RX_FIFO_RD: DATA (Bit 0)                              */
#define PDM_RX_FIFO_RD_DATA_Msk           (0xffffffffUL)        /*!< PDM RX_FIFO_RD: DATA (Bitfield-Mask: 0xffffffff)          */
/* =================================================  PDM.RX_FIFO_RD_SILENT  ================================================= */
#define PDM_RX_FIFO_RD_SILENT_DATA_Pos    (0UL)                 /*!< PDM RX_FIFO_RD_SILENT: DATA (Bit 0)                       */
#define PDM_RX_FIFO_RD_SILENT_DATA_Msk    (0xffffffffUL)        /*!< PDM RX_FIFO_RD_SILENT: DATA (Bitfield-Mask: 0xffffffff)   */
/* =======================================================  PDM.INTR  ======================================================== */
#define PDM_INTR_RX_TRIGGER_Pos           (16UL)                /*!< PDM INTR: RX_TRIGGER (Bit 16)                             */
#define PDM_INTR_RX_TRIGGER_Msk           (0x10000UL)           /*!< PDM INTR: RX_TRIGGER (Bitfield-Mask: 0x01)                */
#define PDM_INTR_RX_NOT_EMPTY_Pos         (18UL)                /*!< PDM INTR: RX_NOT_EMPTY (Bit 18)                           */
#define PDM_INTR_RX_NOT_EMPTY_Msk         (0x40000UL)           /*!< PDM INTR: RX_NOT_EMPTY (Bitfield-Mask: 0x01)              */
#define PDM_INTR_RX_FULL_Pos              (19UL)                /*!< PDM INTR: RX_FULL (Bit 19)                                */
#define PDM_INTR_RX_FULL_Msk              (0x80000UL)           /*!< PDM INTR: RX_FULL (Bitfield-Mask: 0x01)                   */
#define PDM_INTR_RX_OVERFLOW_Pos          (21UL)                /*!< PDM INTR: RX_OVERFLOW (Bit 21)                            */
#define PDM_INTR_RX_OVERFLOW_Msk          (0x200000UL)          /*!< PDM INTR: RX_OVERFLOW (Bitfield-Mask: 0x01)               */
#define PDM_INTR_RX_UNDERFLOW_Pos         (22UL)                /*!< PDM INTR: RX_UNDERFLOW (Bit 22)                           */
#define PDM_INTR_RX_UNDERFLOW_Msk         (0x400000UL)          /*!< PDM INTR: RX_UNDERFLOW (Bitfield-Mask: 0x01)              */
/* =====================================================  PDM.INTR_SET  ====================================================== */
#define PDM_INTR_SET_RX_TRIGGER_Pos       (16UL)                /*!< PDM INTR_SET: RX_TRIGGER (Bit 16)                         */
#define PDM_INTR_SET_RX_TRIGGER_Msk       (0x10000UL)           /*!< PDM INTR_SET: RX_TRIGGER (Bitfield-Mask: 0x01)            */
#define PDM_INTR_SET_RX_NOT_EMPTY_Pos     (18UL)                /*!< PDM INTR_SET: RX_NOT_EMPTY (Bit 18)                       */
#define PDM_INTR_SET_RX_NOT_EMPTY_Msk     (0x40000UL)           /*!< PDM INTR_SET: RX_NOT_EMPTY (Bitfield-Mask: 0x01)          */
#define PDM_INTR_SET_RX_FULL_Pos          (19UL)                /*!< PDM INTR_SET: RX_FULL (Bit 19)                            */
#define PDM_INTR_SET_RX_FULL_Msk          (0x80000UL)           /*!< PDM INTR_SET: RX_FULL (Bitfield-Mask: 0x01)               */
#define PDM_INTR_SET_RX_OVERFLOW_Pos      (21UL)                /*!< PDM INTR_SET: RX_OVERFLOW (Bit 21)                        */
#define PDM_INTR_SET_RX_OVERFLOW_Msk      (0x200000UL)          /*!< PDM INTR_SET: RX_OVERFLOW (Bitfield-Mask: 0x01)           */
#define PDM_INTR_SET_RX_UNDERFLOW_Pos     (22UL)                /*!< PDM INTR_SET: RX_UNDERFLOW (Bit 22)                       */
#define PDM_INTR_SET_RX_UNDERFLOW_Msk     (0x400000UL)          /*!< PDM INTR_SET: RX_UNDERFLOW (Bitfield-Mask: 0x01)          */
/* =====================================================  PDM.INTR_MASK  ===================================================== */
#define PDM_INTR_MASK_RX_TRIGGER_Pos      (16UL)                /*!< PDM INTR_MASK: RX_TRIGGER (Bit 16)                        */
#define PDM_INTR_MASK_RX_TRIGGER_Msk      (0x10000UL)           /*!< PDM INTR_MASK: RX_TRIGGER (Bitfield-Mask: 0x01)           */
#define PDM_INTR_MASK_RX_NOT_EMPTY_Pos    (18UL)                /*!< PDM INTR_MASK: RX_NOT_EMPTY (Bit 18)                      */
#define PDM_INTR_MASK_RX_NOT_EMPTY_Msk    (0x40000UL)           /*!< PDM INTR_MASK: RX_NOT_EMPTY (Bitfield-Mask: 0x01)         */
#define PDM_INTR_MASK_RX_FULL_Pos         (19UL)                /*!< PDM INTR_MASK: RX_FULL (Bit 19)                           */
#define PDM_INTR_MASK_RX_FULL_Msk         (0x80000UL)           /*!< PDM INTR_MASK: RX_FULL (Bitfield-Mask: 0x01)              */
#define PDM_INTR_MASK_RX_OVERFLOW_Pos     (21UL)                /*!< PDM INTR_MASK: RX_OVERFLOW (Bit 21)                       */
#define PDM_INTR_MASK_RX_OVERFLOW_Msk     (0x200000UL)          /*!< PDM INTR_MASK: RX_OVERFLOW (Bitfield-Mask: 0x01)          */
#define PDM_INTR_MASK_RX_UNDERFLOW_Pos    (22UL)                /*!< PDM INTR_MASK: RX_UNDERFLOW (Bit 22)                      */
#define PDM_INTR_MASK_RX_UNDERFLOW_Msk    (0x400000UL)          /*!< PDM INTR_MASK: RX_UNDERFLOW (Bitfield-Mask: 0x01)         */
/* ====================================================  PDM.INTR_MASKED  ==================================================== */
#define PDM_INTR_MASKED_RX_TRIGGER_Pos    (16UL)                /*!< PDM INTR_MASKED: RX_TRIGGER (Bit 16)                      */
#define PDM_INTR_MASKED_RX_TRIGGER_Msk    (0x10000UL)           /*!< PDM INTR_MASKED: RX_TRIGGER (Bitfield-Mask: 0x01)         */
#define PDM_INTR_MASKED_RX_NOT_EMPTY_Pos  (18UL)                /*!< PDM INTR_MASKED: RX_NOT_EMPTY (Bit 18)                    */
#define PDM_INTR_MASKED_RX_NOT_EMPTY_Msk  (0x40000UL)           /*!< PDM INTR_MASKED: RX_NOT_EMPTY (Bitfield-Mask: 0x01)       */
#define PDM_INTR_MASKED_RX_FULL_Pos       (19UL)                /*!< PDM INTR_MASKED: RX_FULL (Bit 19)                         */
#define PDM_INTR_MASKED_RX_FULL_Msk       (0x80000UL)           /*!< PDM INTR_MASKED: RX_FULL (Bitfield-Mask: 0x01)            */
#define PDM_INTR_MASKED_RX_OVERFLOW_Pos   (21UL)                /*!< PDM INTR_MASKED: RX_OVERFLOW (Bit 21)                     */
#define PDM_INTR_MASKED_RX_OVERFLOW_Msk   (0x200000UL)          /*!< PDM INTR_MASKED: RX_OVERFLOW (Bitfield-Mask: 0x01)        */
#define PDM_INTR_MASKED_RX_UNDERFLOW_Pos  (22UL)                /*!< PDM INTR_MASKED: RX_UNDERFLOW (Bit 22)                    */
#define PDM_INTR_MASKED_RX_UNDERFLOW_Msk  (0x400000UL)          /*!< PDM INTR_MASKED: RX_UNDERFLOW (Bitfield-Mask: 0x01)       */


#endif /* _CYIP_PDM_H_ */


/* [] END OF FILE */
