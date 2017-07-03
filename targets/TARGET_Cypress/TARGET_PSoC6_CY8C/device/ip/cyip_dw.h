/***************************************************************************//**
* \file cyip_dw.h
*
* \brief
* DW IP definitions
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

#ifndef _CYIP_DW_H_
#define _CYIP_DW_H_

#include "cyip_headers.h"

/* =========================================================================================================================== */
/* ================                                            DW                                             ================ */
/* =========================================================================================================================== */

#define DW_CH_STRUCT_SECTION_SIZE   0x00000020UL
#define DW_DESCR_STRUCT_SECTION_SIZE 0x00000020UL
#define DW_SECTION_SIZE             0x00001000UL

/**
  * \brief DW channel structure (DW_CH_STRUCT)
  */
typedef struct {
  __IOM uint32_t CH_CTL;                        /*!< 0x00000000 Channel control                                                */
   __IM uint32_t CH_STATUS;                     /*!< 0x00000004 Channel status                                                 */
  __IOM uint32_t CH_IDX;                        /*!< 0x00000008 Channel current indices                                        */
  __IOM uint32_t CH_CURR_PTR;                   /*!< 0x0000000C Channel current descriptor pointer                             */
  __IOM uint32_t INTR;                          /*!< 0x00000010 Interrupt                                                      */
  __IOM uint32_t INTR_SET;                      /*!< 0x00000014 Interrupt set                                                  */
  __IOM uint32_t INTR_MASK;                     /*!< 0x00000018 Interrupt mask                                                 */
   __IM uint32_t INTR_MASKED;                   /*!< 0x0000001C Interrupt masked                                               */
} DW_CH_STRUCT_Type;                            /*!< Size = 32 (0x20)                                                          */

/**
  * \brief DW descriptor structure (DW_DESCR_STRUCT)
  */
typedef struct {
  __IOM uint32_t DESCR_CTL;                     /*!< 0x00000000 Descriptor control                                             */
  __IOM uint32_t DESCR_SRC;                     /*!< 0x00000004 Descriptor source                                              */
  __IOM uint32_t DESCR_DST;                     /*!< 0x00000008 Descriptor destination                                         */
  __IOM uint32_t DESCR_X_CTL;                   /*!< 0x0000000C Descriptor X loop control                                      */
  __IOM uint32_t DESCR_Y_CTL;                   /*!< 0x00000010 Descriptor Y loop control                                      */
  __IOM uint32_t DESCR_NEXT_PTR;                /*!< 0x00000014 Descriptor next pointer                                        */
   __IM uint32_t RESERVED[2];
} DW_DESCR_STRUCT_Type;                         /*!< Size = 24 (0x18)                                                          */

/**
  * \brief Datawire Controller (DW)
  */
typedef struct {                                /*!< DW Structure                                                              */
  __IOM uint32_t CTL;                           /*!< 0x00000000 Control                                                        */
   __IM uint32_t STATUS;                        /*!< 0x00000004 Status                                                         */
   __IM uint32_t PENDING;                       /*!< 0x00000008 Pending channels                                               */
   __IM uint32_t RESERVED;
   __IM uint32_t STATUS_INTR;                   /*!< 0x00000010 System interrupt control                                       */
   __IM uint32_t STATUS_INTR_MASKED;            /*!< 0x00000014 Status of interrupts masked                                    */
   __IM uint32_t RESERVED1[2];
   __IM uint32_t ACT_DESCR_CTL;                 /*!< 0x00000020 Active descriptor control                                      */
   __IM uint32_t ACT_DESCR_SRC;                 /*!< 0x00000024 Active descriptor source                                       */
   __IM uint32_t ACT_DESCR_DST;                 /*!< 0x00000028 Active descriptor destination                                  */
   __IM uint32_t RESERVED2;
   __IM uint32_t ACT_DESCR_X_CTL;               /*!< 0x00000030 Active descriptor X loop control                               */
   __IM uint32_t ACT_DESCR_Y_CTL;               /*!< 0x00000034 Active descriptor Y loop control                               */
   __IM uint32_t ACT_DESCR_NEXT_PTR;            /*!< 0x00000038 Active descriptor next pointer                                 */
   __IM uint32_t RESERVED3;
   __IM uint32_t ACT_SRC;                       /*!< 0x00000040 Active source                                                  */
   __IM uint32_t ACT_DST;                       /*!< 0x00000044 Active destination                                             */
   __IM uint32_t RESERVED4[494];
        DW_CH_STRUCT_Type CH_STRUCT[32];        /*!< 0x00000800 DW channel structure                                           */
        DW_DESCR_STRUCT_Type DESCR_STRUCT;      /*!< 0x00000C00 DW descriptor structure                                        */
} DW_Type;                                      /*!< Size = 3104 (0xC20)                                                       */


/* ==================================================  DW_CH_STRUCT.CH_CTL  ================================================== */
#define DW_CH_STRUCT_CH_CTL_P_Pos         (0UL)                 /*!< DW_CH_STRUCT CH_CTL: P (Bit 0)                            */
#define DW_CH_STRUCT_CH_CTL_P_Msk         (0x1UL)               /*!< DW_CH_STRUCT CH_CTL: P (Bitfield-Mask: 0x01)              */
#define DW_CH_STRUCT_CH_CTL_NS_Pos        (1UL)                 /*!< DW_CH_STRUCT CH_CTL: NS (Bit 1)                           */
#define DW_CH_STRUCT_CH_CTL_NS_Msk        (0x2UL)               /*!< DW_CH_STRUCT CH_CTL: NS (Bitfield-Mask: 0x01)             */
#define DW_CH_STRUCT_CH_CTL_B_Pos         (2UL)                 /*!< DW_CH_STRUCT CH_CTL: B (Bit 2)                            */
#define DW_CH_STRUCT_CH_CTL_B_Msk         (0x4UL)               /*!< DW_CH_STRUCT CH_CTL: B (Bitfield-Mask: 0x01)              */
#define DW_CH_STRUCT_CH_CTL_PC_Pos        (4UL)                 /*!< DW_CH_STRUCT CH_CTL: PC (Bit 4)                           */
#define DW_CH_STRUCT_CH_CTL_PC_Msk        (0xf0UL)              /*!< DW_CH_STRUCT CH_CTL: PC (Bitfield-Mask: 0x0f)             */
#define DW_CH_STRUCT_CH_CTL_PRIO_Pos      (16UL)                /*!< DW_CH_STRUCT CH_CTL: PRIO (Bit 16)                        */
#define DW_CH_STRUCT_CH_CTL_PRIO_Msk      (0x30000UL)           /*!< DW_CH_STRUCT CH_CTL: PRIO (Bitfield-Mask: 0x03)           */
#define DW_CH_STRUCT_CH_CTL_PREEMPTABLE_Pos (18UL)              /*!< DW_CH_STRUCT CH_CTL: PREEMPTABLE (Bit 18)                 */
#define DW_CH_STRUCT_CH_CTL_PREEMPTABLE_Msk (0x40000UL)         /*!< DW_CH_STRUCT CH_CTL: PREEMPTABLE (Bitfield-Mask: 0x01)    */
#define DW_CH_STRUCT_CH_CTL_ENABLED_Pos   (31UL)                /*!< DW_CH_STRUCT CH_CTL: ENABLED (Bit 31)                     */
#define DW_CH_STRUCT_CH_CTL_ENABLED_Msk   (0x80000000UL)        /*!< DW_CH_STRUCT CH_CTL: ENABLED (Bitfield-Mask: 0x01)        */
/* ================================================  DW_CH_STRUCT.CH_STATUS  ================================================= */
#define DW_CH_STRUCT_CH_STATUS_INTR_CAUSE_Pos (0UL)             /*!< DW_CH_STRUCT CH_STATUS: INTR_CAUSE (Bit 0)                */
#define DW_CH_STRUCT_CH_STATUS_INTR_CAUSE_Msk (0xfUL)           /*!< DW_CH_STRUCT CH_STATUS: INTR_CAUSE (Bitfield-Mask: 0x0f)  */
/* ==================================================  DW_CH_STRUCT.CH_IDX  ================================================== */
#define DW_CH_STRUCT_CH_IDX_X_IDX_Pos     (0UL)                 /*!< DW_CH_STRUCT CH_IDX: X_IDX (Bit 0)                        */
#define DW_CH_STRUCT_CH_IDX_X_IDX_Msk     (0xffUL)              /*!< DW_CH_STRUCT CH_IDX: X_IDX (Bitfield-Mask: 0xff)          */
#define DW_CH_STRUCT_CH_IDX_Y_IDX_Pos     (8UL)                 /*!< DW_CH_STRUCT CH_IDX: Y_IDX (Bit 8)                        */
#define DW_CH_STRUCT_CH_IDX_Y_IDX_Msk     (0xff00UL)            /*!< DW_CH_STRUCT CH_IDX: Y_IDX (Bitfield-Mask: 0xff)          */
/* ===============================================  DW_CH_STRUCT.CH_CURR_PTR  ================================================ */
#define DW_CH_STRUCT_CH_CURR_PTR_ADDR_Pos (2UL)                 /*!< DW_CH_STRUCT CH_CURR_PTR: ADDR (Bit 2)                    */
#define DW_CH_STRUCT_CH_CURR_PTR_ADDR_Msk (0xfffffffcUL)        /*!< DW_CH_STRUCT CH_CURR_PTR: ADDR (Bitfield-Mask: 0x3fffffff)*/
/* ===================================================  DW_CH_STRUCT.INTR  =================================================== */
#define DW_CH_STRUCT_INTR_CH_Pos          (0UL)                 /*!< DW_CH_STRUCT INTR: CH (Bit 0)                             */
#define DW_CH_STRUCT_INTR_CH_Msk          (0x1UL)               /*!< DW_CH_STRUCT INTR: CH (Bitfield-Mask: 0x01)               */
/* =================================================  DW_CH_STRUCT.INTR_SET  ================================================= */
#define DW_CH_STRUCT_INTR_SET_CH_Pos      (0UL)                 /*!< DW_CH_STRUCT INTR_SET: CH (Bit 0)                         */
#define DW_CH_STRUCT_INTR_SET_CH_Msk      (0x1UL)               /*!< DW_CH_STRUCT INTR_SET: CH (Bitfield-Mask: 0x01)           */
/* ================================================  DW_CH_STRUCT.INTR_MASK  ================================================= */
#define DW_CH_STRUCT_INTR_MASK_CH_Pos     (0UL)                 /*!< DW_CH_STRUCT INTR_MASK: CH (Bit 0)                        */
#define DW_CH_STRUCT_INTR_MASK_CH_Msk     (0x1UL)               /*!< DW_CH_STRUCT INTR_MASK: CH (Bitfield-Mask: 0x01)          */
/* ===============================================  DW_CH_STRUCT.INTR_MASKED  ================================================ */
#define DW_CH_STRUCT_INTR_MASKED_CH_Pos   (0UL)                 /*!< DW_CH_STRUCT INTR_MASKED: CH (Bit 0)                      */
#define DW_CH_STRUCT_INTR_MASKED_CH_Msk   (0x1UL)               /*!< DW_CH_STRUCT INTR_MASKED: CH (Bitfield-Mask: 0x01)        */


/* ===============================================  DW_DESCR_STRUCT.DESCR_CTL  =============================================== */
#define DW_DESCR_STRUCT_DESCR_CTL_WAIT_FOR_DEACT_Pos (0UL)      /*!< DW_DESCR_STRUCT DESCR_CTL: WAIT_FOR_DEACT (Bit 0)         */
#define DW_DESCR_STRUCT_DESCR_CTL_WAIT_FOR_DEACT_Msk (0x3UL)    /*!< DW_DESCR_STRUCT DESCR_CTL: WAIT_FOR_DEACT (Bitfield-Mask: 0x03)*/
#define DW_DESCR_STRUCT_DESCR_CTL_INTR_TYPE_Pos (2UL)           /*!< DW_DESCR_STRUCT DESCR_CTL: INTR_TYPE (Bit 2)              */
#define DW_DESCR_STRUCT_DESCR_CTL_INTR_TYPE_Msk (0xcUL)         /*!< DW_DESCR_STRUCT DESCR_CTL: INTR_TYPE (Bitfield-Mask: 0x03)*/
#define DW_DESCR_STRUCT_DESCR_CTL_TR_OUT_TYPE_Pos (4UL)         /*!< DW_DESCR_STRUCT DESCR_CTL: TR_OUT_TYPE (Bit 4)            */
#define DW_DESCR_STRUCT_DESCR_CTL_TR_OUT_TYPE_Msk (0x30UL)      /*!< DW_DESCR_STRUCT DESCR_CTL: TR_OUT_TYPE (Bitfield-Mask: 0x03)*/
#define DW_DESCR_STRUCT_DESCR_CTL_TR_IN_TYPE_Pos (6UL)          /*!< DW_DESCR_STRUCT DESCR_CTL: TR_IN_TYPE (Bit 6)             */
#define DW_DESCR_STRUCT_DESCR_CTL_TR_IN_TYPE_Msk (0xc0UL)       /*!< DW_DESCR_STRUCT DESCR_CTL: TR_IN_TYPE (Bitfield-Mask: 0x03)*/
#define DW_DESCR_STRUCT_DESCR_CTL_CH_DISABLE_Pos (24UL)         /*!< DW_DESCR_STRUCT DESCR_CTL: CH_DISABLE (Bit 24)            */
#define DW_DESCR_STRUCT_DESCR_CTL_CH_DISABLE_Msk (0x1000000UL)  /*!< DW_DESCR_STRUCT DESCR_CTL: CH_DISABLE (Bitfield-Mask: 0x01)*/
#define DW_DESCR_STRUCT_DESCR_CTL_SRC_TRANSFER_SIZE_Pos (26UL)  /*!< DW_DESCR_STRUCT DESCR_CTL: SRC_TRANSFER_SIZE (Bit 26)     */
#define DW_DESCR_STRUCT_DESCR_CTL_SRC_TRANSFER_SIZE_Msk (0x4000000UL) /*!< DW_DESCR_STRUCT DESCR_CTL: SRC_TRANSFER_SIZE (Bitfield-Mask: 0x01)*/
#define DW_DESCR_STRUCT_DESCR_CTL_DST_TRANSFER_SIZE_Pos (27UL)  /*!< DW_DESCR_STRUCT DESCR_CTL: DST_TRANSFER_SIZE (Bit 27)     */
#define DW_DESCR_STRUCT_DESCR_CTL_DST_TRANSFER_SIZE_Msk (0x8000000UL) /*!< DW_DESCR_STRUCT DESCR_CTL: DST_TRANSFER_SIZE (Bitfield-Mask: 0x01)*/
#define DW_DESCR_STRUCT_DESCR_CTL_DATA_SIZE_Pos (28UL)          /*!< DW_DESCR_STRUCT DESCR_CTL: DATA_SIZE (Bit 28)             */
#define DW_DESCR_STRUCT_DESCR_CTL_DATA_SIZE_Msk (0x30000000UL)  /*!< DW_DESCR_STRUCT DESCR_CTL: DATA_SIZE (Bitfield-Mask: 0x03)*/
#define DW_DESCR_STRUCT_DESCR_CTL_DESCR_TYPE_Pos (30UL)         /*!< DW_DESCR_STRUCT DESCR_CTL: DESCR_TYPE (Bit 30)            */
#define DW_DESCR_STRUCT_DESCR_CTL_DESCR_TYPE_Msk (0xc0000000UL) /*!< DW_DESCR_STRUCT DESCR_CTL: DESCR_TYPE (Bitfield-Mask: 0x03)*/
/* ===============================================  DW_DESCR_STRUCT.DESCR_SRC  =============================================== */
#define DW_DESCR_STRUCT_DESCR_SRC_SRC_ADDR_Pos (0UL)            /*!< DW_DESCR_STRUCT DESCR_SRC: SRC_ADDR (Bit 0)               */
#define DW_DESCR_STRUCT_DESCR_SRC_SRC_ADDR_Msk (0xffffffffUL)   /*!< DW_DESCR_STRUCT DESCR_SRC: SRC_ADDR (Bitfield-Mask: 0xffffffff)*/
/* ===============================================  DW_DESCR_STRUCT.DESCR_DST  =============================================== */
#define DW_DESCR_STRUCT_DESCR_DST_DST_ADDR_Pos (0UL)            /*!< DW_DESCR_STRUCT DESCR_DST: DST_ADDR (Bit 0)               */
#define DW_DESCR_STRUCT_DESCR_DST_DST_ADDR_Msk (0xffffffffUL)   /*!< DW_DESCR_STRUCT DESCR_DST: DST_ADDR (Bitfield-Mask: 0xffffffff)*/
/* ==============================================  DW_DESCR_STRUCT.DESCR_X_CTL  ============================================== */
#define DW_DESCR_STRUCT_DESCR_X_CTL_SRC_X_INCR_Pos (0UL)        /*!< DW_DESCR_STRUCT DESCR_X_CTL: SRC_X_INCR (Bit 0)           */
#define DW_DESCR_STRUCT_DESCR_X_CTL_SRC_X_INCR_Msk (0xfffUL)    /*!< DW_DESCR_STRUCT DESCR_X_CTL: SRC_X_INCR (Bitfield-Mask: 0xfff)*/
#define DW_DESCR_STRUCT_DESCR_X_CTL_DST_X_INCR_Pos (12UL)       /*!< DW_DESCR_STRUCT DESCR_X_CTL: DST_X_INCR (Bit 12)          */
#define DW_DESCR_STRUCT_DESCR_X_CTL_DST_X_INCR_Msk (0xfff000UL) /*!< DW_DESCR_STRUCT DESCR_X_CTL: DST_X_INCR (Bitfield-Mask: 0xfff)*/
#define DW_DESCR_STRUCT_DESCR_X_CTL_X_COUNT_Pos (24UL)          /*!< DW_DESCR_STRUCT DESCR_X_CTL: X_COUNT (Bit 24)             */
#define DW_DESCR_STRUCT_DESCR_X_CTL_X_COUNT_Msk (0xff000000UL)  /*!< DW_DESCR_STRUCT DESCR_X_CTL: X_COUNT (Bitfield-Mask: 0xff)*/
/* ==============================================  DW_DESCR_STRUCT.DESCR_Y_CTL  ============================================== */
#define DW_DESCR_STRUCT_DESCR_Y_CTL_SRC_Y_INCR_Pos (0UL)        /*!< DW_DESCR_STRUCT DESCR_Y_CTL: SRC_Y_INCR (Bit 0)           */
#define DW_DESCR_STRUCT_DESCR_Y_CTL_SRC_Y_INCR_Msk (0xfffUL)    /*!< DW_DESCR_STRUCT DESCR_Y_CTL: SRC_Y_INCR (Bitfield-Mask: 0xfff)*/
#define DW_DESCR_STRUCT_DESCR_Y_CTL_DST_Y_INCR_Pos (12UL)       /*!< DW_DESCR_STRUCT DESCR_Y_CTL: DST_Y_INCR (Bit 12)          */
#define DW_DESCR_STRUCT_DESCR_Y_CTL_DST_Y_INCR_Msk (0xfff000UL) /*!< DW_DESCR_STRUCT DESCR_Y_CTL: DST_Y_INCR (Bitfield-Mask: 0xfff)*/
#define DW_DESCR_STRUCT_DESCR_Y_CTL_Y_COUNT_Pos (24UL)          /*!< DW_DESCR_STRUCT DESCR_Y_CTL: Y_COUNT (Bit 24)             */
#define DW_DESCR_STRUCT_DESCR_Y_CTL_Y_COUNT_Msk (0xff000000UL)  /*!< DW_DESCR_STRUCT DESCR_Y_CTL: Y_COUNT (Bitfield-Mask: 0xff)*/
/* ============================================  DW_DESCR_STRUCT.DESCR_NEXT_PTR  ============================================= */
#define DW_DESCR_STRUCT_DESCR_NEXT_PTR_ADDR_Pos (2UL)           /*!< DW_DESCR_STRUCT DESCR_NEXT_PTR: ADDR (Bit 2)              */
#define DW_DESCR_STRUCT_DESCR_NEXT_PTR_ADDR_Msk (0xfffffffcUL)  /*!< DW_DESCR_STRUCT DESCR_NEXT_PTR: ADDR (Bitfield-Mask: 0x3fffffff)*/


/* ========================================================  DW.CTL  ========================================================= */
#define DW_CTL_ENABLED_Pos                (31UL)                /*!< DW CTL: ENABLED (Bit 31)                                  */
#define DW_CTL_ENABLED_Msk                (0x80000000UL)        /*!< DW CTL: ENABLED (Bitfield-Mask: 0x01)                     */
/* =======================================================  DW.STATUS  ======================================================= */
#define DW_STATUS_P_Pos                   (0UL)                 /*!< DW STATUS: P (Bit 0)                                      */
#define DW_STATUS_P_Msk                   (0x1UL)               /*!< DW STATUS: P (Bitfield-Mask: 0x01)                        */
#define DW_STATUS_NS_Pos                  (1UL)                 /*!< DW STATUS: NS (Bit 1)                                     */
#define DW_STATUS_NS_Msk                  (0x2UL)               /*!< DW STATUS: NS (Bitfield-Mask: 0x01)                       */
#define DW_STATUS_B_Pos                   (2UL)                 /*!< DW STATUS: B (Bit 2)                                      */
#define DW_STATUS_B_Msk                   (0x4UL)               /*!< DW STATUS: B (Bitfield-Mask: 0x01)                        */
#define DW_STATUS_PC_Pos                  (4UL)                 /*!< DW STATUS: PC (Bit 4)                                     */
#define DW_STATUS_PC_Msk                  (0xf0UL)              /*!< DW STATUS: PC (Bitfield-Mask: 0x0f)                       */
#define DW_STATUS_CH_IDX_Pos              (8UL)                 /*!< DW STATUS: CH_IDX (Bit 8)                                 */
#define DW_STATUS_CH_IDX_Msk              (0x1f00UL)            /*!< DW STATUS: CH_IDX (Bitfield-Mask: 0x1f)                   */
#define DW_STATUS_PRIO_Pos                (16UL)                /*!< DW STATUS: PRIO (Bit 16)                                  */
#define DW_STATUS_PRIO_Msk                (0x30000UL)           /*!< DW STATUS: PRIO (Bitfield-Mask: 0x03)                     */
#define DW_STATUS_PREEMPTABLE_Pos         (18UL)                /*!< DW STATUS: PREEMPTABLE (Bit 18)                           */
#define DW_STATUS_PREEMPTABLE_Msk         (0x40000UL)           /*!< DW STATUS: PREEMPTABLE (Bitfield-Mask: 0x01)              */
#define DW_STATUS_STATE_Pos               (20UL)                /*!< DW STATUS: STATE (Bit 20)                                 */
#define DW_STATUS_STATE_Msk               (0x700000UL)          /*!< DW STATUS: STATE (Bitfield-Mask: 0x07)                    */
#define DW_STATUS_ACTIVE_Pos              (31UL)                /*!< DW STATUS: ACTIVE (Bit 31)                                */
#define DW_STATUS_ACTIVE_Msk              (0x80000000UL)        /*!< DW STATUS: ACTIVE (Bitfield-Mask: 0x01)                   */
/* ======================================================  DW.PENDING  ======================================================= */
#define DW_PENDING_CH_PENDING_Pos         (0UL)                 /*!< DW PENDING: CH_PENDING (Bit 0)                            */
#define DW_PENDING_CH_PENDING_Msk         (0xffffffffUL)        /*!< DW PENDING: CH_PENDING (Bitfield-Mask: 0xffffffff)        */
/* ====================================================  DW.STATUS_INTR  ===================================================== */
#define DW_STATUS_INTR_CH_Pos             (0UL)                 /*!< DW STATUS_INTR: CH (Bit 0)                                */
#define DW_STATUS_INTR_CH_Msk             (0xffffffffUL)        /*!< DW STATUS_INTR: CH (Bitfield-Mask: 0xffffffff)            */
/* =================================================  DW.STATUS_INTR_MASKED  ================================================= */
#define DW_STATUS_INTR_MASKED_CH_Pos      (0UL)                 /*!< DW STATUS_INTR_MASKED: CH (Bit 0)                         */
#define DW_STATUS_INTR_MASKED_CH_Msk      (0xffffffffUL)        /*!< DW STATUS_INTR_MASKED: CH (Bitfield-Mask: 0xffffffff)     */
/* ===================================================  DW.ACT_DESCR_CTL  ==================================================== */
#define DW_ACT_DESCR_CTL_DATA_Pos         (0UL)                 /*!< DW ACT_DESCR_CTL: DATA (Bit 0)                            */
#define DW_ACT_DESCR_CTL_DATA_Msk         (0xffffffffUL)        /*!< DW ACT_DESCR_CTL: DATA (Bitfield-Mask: 0xffffffff)        */
/* ===================================================  DW.ACT_DESCR_SRC  ==================================================== */
#define DW_ACT_DESCR_SRC_DATA_Pos         (0UL)                 /*!< DW ACT_DESCR_SRC: DATA (Bit 0)                            */
#define DW_ACT_DESCR_SRC_DATA_Msk         (0xffffffffUL)        /*!< DW ACT_DESCR_SRC: DATA (Bitfield-Mask: 0xffffffff)        */
/* ===================================================  DW.ACT_DESCR_DST  ==================================================== */
#define DW_ACT_DESCR_DST_DATA_Pos         (0UL)                 /*!< DW ACT_DESCR_DST: DATA (Bit 0)                            */
#define DW_ACT_DESCR_DST_DATA_Msk         (0xffffffffUL)        /*!< DW ACT_DESCR_DST: DATA (Bitfield-Mask: 0xffffffff)        */
/* ==================================================  DW.ACT_DESCR_X_CTL  =================================================== */
#define DW_ACT_DESCR_X_CTL_DATA_Pos       (0UL)                 /*!< DW ACT_DESCR_X_CTL: DATA (Bit 0)                          */
#define DW_ACT_DESCR_X_CTL_DATA_Msk       (0xffffffffUL)        /*!< DW ACT_DESCR_X_CTL: DATA (Bitfield-Mask: 0xffffffff)      */
/* ==================================================  DW.ACT_DESCR_Y_CTL  =================================================== */
#define DW_ACT_DESCR_Y_CTL_DATA_Pos       (0UL)                 /*!< DW ACT_DESCR_Y_CTL: DATA (Bit 0)                          */
#define DW_ACT_DESCR_Y_CTL_DATA_Msk       (0xffffffffUL)        /*!< DW ACT_DESCR_Y_CTL: DATA (Bitfield-Mask: 0xffffffff)      */
/* =================================================  DW.ACT_DESCR_NEXT_PTR  ================================================= */
#define DW_ACT_DESCR_NEXT_PTR_ADDR_Pos    (2UL)                 /*!< DW ACT_DESCR_NEXT_PTR: ADDR (Bit 2)                       */
#define DW_ACT_DESCR_NEXT_PTR_ADDR_Msk    (0xfffffffcUL)        /*!< DW ACT_DESCR_NEXT_PTR: ADDR (Bitfield-Mask: 0x3fffffff)   */
/* ======================================================  DW.ACT_SRC  ======================================================= */
#define DW_ACT_SRC_SRC_ADDR_Pos           (0UL)                 /*!< DW ACT_SRC: SRC_ADDR (Bit 0)                              */
#define DW_ACT_SRC_SRC_ADDR_Msk           (0xffffffffUL)        /*!< DW ACT_SRC: SRC_ADDR (Bitfield-Mask: 0xffffffff)          */
/* ======================================================  DW.ACT_DST  ======================================================= */
#define DW_ACT_DST_DST_ADDR_Pos           (0UL)                 /*!< DW ACT_DST: DST_ADDR (Bit 0)                              */
#define DW_ACT_DST_DST_ADDR_Msk           (0xffffffffUL)        /*!< DW ACT_DST: DST_ADDR (Bitfield-Mask: 0xffffffff)          */


#endif /* _CYIP_DW_H_ */


/* [] END OF FILE */
