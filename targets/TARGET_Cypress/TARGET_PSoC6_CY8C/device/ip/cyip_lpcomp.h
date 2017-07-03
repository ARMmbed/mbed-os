/***************************************************************************//**
* \file cyip_lpcomp.h
*
* \brief
* LPCOMP IP definitions
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

#ifndef _CYIP_LPCOMP_H_
#define _CYIP_LPCOMP_H_

#include "cyip_headers.h"

/* =========================================================================================================================== */
/* ================                                          LPCOMP                                           ================ */
/* =========================================================================================================================== */

#define LPCOMP_SECTION_SIZE         0x00010000UL

/**
  * \brief Low Power Comparators (LPCOMP)
  */
typedef struct {                                /*!< LPCOMP Structure                                                          */
  __IOM uint32_t CONFIG;                        /*!< 0x00000000 LPCOMP Configuration Register                                  */
   __IM uint32_t STATUS;                        /*!< 0x00000004 LPCOMP Status Register                                         */
   __IM uint32_t RESERVED[2];
  __IOM uint32_t INTR;                          /*!< 0x00000010 LPCOMP Interrupt request register                              */
  __IOM uint32_t INTR_SET;                      /*!< 0x00000014 LPCOMP Interrupt set register                                  */
  __IOM uint32_t INTR_MASK;                     /*!< 0x00000018 LPCOMP Interrupt request mask                                  */
   __IM uint32_t INTR_MASKED;                   /*!< 0x0000001C LPCOMP Interrupt request masked                                */
   __IM uint32_t RESERVED1[8];
  __IOM uint32_t CMP0_CTRL;                     /*!< 0x00000040 Comparator 0 control Register                                  */
   __IM uint32_t RESERVED2[3];
  __IOM uint32_t CMP0_SW;                       /*!< 0x00000050 Comparator 0 switch control                                    */
  __IOM uint32_t CMP0_SW_CLEAR;                 /*!< 0x00000054 Comparator 0 switch control clear                              */
   __IM uint32_t RESERVED3[10];
  __IOM uint32_t CMP1_CTRL;                     /*!< 0x00000080 Comparator 1 control Register                                  */
   __IM uint32_t RESERVED4[3];
  __IOM uint32_t CMP1_SW;                       /*!< 0x00000090 Comparator 1 switch control                                    */
  __IOM uint32_t CMP1_SW_CLEAR;                 /*!< 0x00000094 Comparator 1 switch control clear                              */
} LPCOMP_Type;                                  /*!< Size = 152 (0x98)                                                         */


/* =====================================================  LPCOMP.CONFIG  ===================================================== */
#define LPCOMP_CONFIG_LPREF_EN_Pos        (30UL)                /*!< LPCOMP CONFIG: LPREF_EN (Bit 30)                          */
#define LPCOMP_CONFIG_LPREF_EN_Msk        (0x40000000UL)        /*!< LPCOMP CONFIG: LPREF_EN (Bitfield-Mask: 0x01)             */
#define LPCOMP_CONFIG_ENABLED_Pos         (31UL)                /*!< LPCOMP CONFIG: ENABLED (Bit 31)                           */
#define LPCOMP_CONFIG_ENABLED_Msk         (0x80000000UL)        /*!< LPCOMP CONFIG: ENABLED (Bitfield-Mask: 0x01)              */
/* =====================================================  LPCOMP.STATUS  ===================================================== */
#define LPCOMP_STATUS_OUT0_Pos            (0UL)                 /*!< LPCOMP STATUS: OUT0 (Bit 0)                               */
#define LPCOMP_STATUS_OUT0_Msk            (0x1UL)               /*!< LPCOMP STATUS: OUT0 (Bitfield-Mask: 0x01)                 */
#define LPCOMP_STATUS_OUT1_Pos            (16UL)                /*!< LPCOMP STATUS: OUT1 (Bit 16)                              */
#define LPCOMP_STATUS_OUT1_Msk            (0x10000UL)           /*!< LPCOMP STATUS: OUT1 (Bitfield-Mask: 0x01)                 */
/* ======================================================  LPCOMP.INTR  ====================================================== */
#define LPCOMP_INTR_COMP0_Pos             (0UL)                 /*!< LPCOMP INTR: COMP0 (Bit 0)                                */
#define LPCOMP_INTR_COMP0_Msk             (0x1UL)               /*!< LPCOMP INTR: COMP0 (Bitfield-Mask: 0x01)                  */
#define LPCOMP_INTR_COMP1_Pos             (1UL)                 /*!< LPCOMP INTR: COMP1 (Bit 1)                                */
#define LPCOMP_INTR_COMP1_Msk             (0x2UL)               /*!< LPCOMP INTR: COMP1 (Bitfield-Mask: 0x01)                  */
/* ====================================================  LPCOMP.INTR_SET  ==================================================== */
#define LPCOMP_INTR_SET_COMP0_Pos         (0UL)                 /*!< LPCOMP INTR_SET: COMP0 (Bit 0)                            */
#define LPCOMP_INTR_SET_COMP0_Msk         (0x1UL)               /*!< LPCOMP INTR_SET: COMP0 (Bitfield-Mask: 0x01)              */
#define LPCOMP_INTR_SET_COMP1_Pos         (1UL)                 /*!< LPCOMP INTR_SET: COMP1 (Bit 1)                            */
#define LPCOMP_INTR_SET_COMP1_Msk         (0x2UL)               /*!< LPCOMP INTR_SET: COMP1 (Bitfield-Mask: 0x01)              */
/* ===================================================  LPCOMP.INTR_MASK  ==================================================== */
#define LPCOMP_INTR_MASK_COMP0_MASK_Pos   (0UL)                 /*!< LPCOMP INTR_MASK: COMP0_MASK (Bit 0)                      */
#define LPCOMP_INTR_MASK_COMP0_MASK_Msk   (0x1UL)               /*!< LPCOMP INTR_MASK: COMP0_MASK (Bitfield-Mask: 0x01)        */
#define LPCOMP_INTR_MASK_COMP1_MASK_Pos   (1UL)                 /*!< LPCOMP INTR_MASK: COMP1_MASK (Bit 1)                      */
#define LPCOMP_INTR_MASK_COMP1_MASK_Msk   (0x2UL)               /*!< LPCOMP INTR_MASK: COMP1_MASK (Bitfield-Mask: 0x01)        */
/* ==================================================  LPCOMP.INTR_MASKED  =================================================== */
#define LPCOMP_INTR_MASKED_COMP0_MASKED_Pos (0UL)               /*!< LPCOMP INTR_MASKED: COMP0_MASKED (Bit 0)                  */
#define LPCOMP_INTR_MASKED_COMP0_MASKED_Msk (0x1UL)             /*!< LPCOMP INTR_MASKED: COMP0_MASKED (Bitfield-Mask: 0x01)    */
#define LPCOMP_INTR_MASKED_COMP1_MASKED_Pos (1UL)               /*!< LPCOMP INTR_MASKED: COMP1_MASKED (Bit 1)                  */
#define LPCOMP_INTR_MASKED_COMP1_MASKED_Msk (0x2UL)             /*!< LPCOMP INTR_MASKED: COMP1_MASKED (Bitfield-Mask: 0x01)    */
/* ===================================================  LPCOMP.CMP0_CTRL  ==================================================== */
#define LPCOMP_CMP0_CTRL_MODE0_Pos        (0UL)                 /*!< LPCOMP CMP0_CTRL: MODE0 (Bit 0)                           */
#define LPCOMP_CMP0_CTRL_MODE0_Msk        (0x3UL)               /*!< LPCOMP CMP0_CTRL: MODE0 (Bitfield-Mask: 0x03)             */
#define LPCOMP_CMP0_CTRL_HYST0_Pos        (5UL)                 /*!< LPCOMP CMP0_CTRL: HYST0 (Bit 5)                           */
#define LPCOMP_CMP0_CTRL_HYST0_Msk        (0x20UL)              /*!< LPCOMP CMP0_CTRL: HYST0 (Bitfield-Mask: 0x01)             */
#define LPCOMP_CMP0_CTRL_INTTYPE0_Pos     (6UL)                 /*!< LPCOMP CMP0_CTRL: INTTYPE0 (Bit 6)                        */
#define LPCOMP_CMP0_CTRL_INTTYPE0_Msk     (0xc0UL)              /*!< LPCOMP CMP0_CTRL: INTTYPE0 (Bitfield-Mask: 0x03)          */
#define LPCOMP_CMP0_CTRL_DSI_BYPASS0_Pos  (10UL)                /*!< LPCOMP CMP0_CTRL: DSI_BYPASS0 (Bit 10)                    */
#define LPCOMP_CMP0_CTRL_DSI_BYPASS0_Msk  (0x400UL)             /*!< LPCOMP CMP0_CTRL: DSI_BYPASS0 (Bitfield-Mask: 0x01)       */
#define LPCOMP_CMP0_CTRL_DSI_LEVEL0_Pos   (11UL)                /*!< LPCOMP CMP0_CTRL: DSI_LEVEL0 (Bit 11)                     */
#define LPCOMP_CMP0_CTRL_DSI_LEVEL0_Msk   (0x800UL)             /*!< LPCOMP CMP0_CTRL: DSI_LEVEL0 (Bitfield-Mask: 0x01)        */
/* ====================================================  LPCOMP.CMP0_SW  ===================================================== */
#define LPCOMP_CMP0_SW_CMP0_IP0_Pos       (0UL)                 /*!< LPCOMP CMP0_SW: CMP0_IP0 (Bit 0)                          */
#define LPCOMP_CMP0_SW_CMP0_IP0_Msk       (0x1UL)               /*!< LPCOMP CMP0_SW: CMP0_IP0 (Bitfield-Mask: 0x01)            */
#define LPCOMP_CMP0_SW_CMP0_AP0_Pos       (1UL)                 /*!< LPCOMP CMP0_SW: CMP0_AP0 (Bit 1)                          */
#define LPCOMP_CMP0_SW_CMP0_AP0_Msk       (0x2UL)               /*!< LPCOMP CMP0_SW: CMP0_AP0 (Bitfield-Mask: 0x01)            */
#define LPCOMP_CMP0_SW_CMP0_BP0_Pos       (2UL)                 /*!< LPCOMP CMP0_SW: CMP0_BP0 (Bit 2)                          */
#define LPCOMP_CMP0_SW_CMP0_BP0_Msk       (0x4UL)               /*!< LPCOMP CMP0_SW: CMP0_BP0 (Bitfield-Mask: 0x01)            */
#define LPCOMP_CMP0_SW_CMP0_IN0_Pos       (4UL)                 /*!< LPCOMP CMP0_SW: CMP0_IN0 (Bit 4)                          */
#define LPCOMP_CMP0_SW_CMP0_IN0_Msk       (0x10UL)              /*!< LPCOMP CMP0_SW: CMP0_IN0 (Bitfield-Mask: 0x01)            */
#define LPCOMP_CMP0_SW_CMP0_AN0_Pos       (5UL)                 /*!< LPCOMP CMP0_SW: CMP0_AN0 (Bit 5)                          */
#define LPCOMP_CMP0_SW_CMP0_AN0_Msk       (0x20UL)              /*!< LPCOMP CMP0_SW: CMP0_AN0 (Bitfield-Mask: 0x01)            */
#define LPCOMP_CMP0_SW_CMP0_BN0_Pos       (6UL)                 /*!< LPCOMP CMP0_SW: CMP0_BN0 (Bit 6)                          */
#define LPCOMP_CMP0_SW_CMP0_BN0_Msk       (0x40UL)              /*!< LPCOMP CMP0_SW: CMP0_BN0 (Bitfield-Mask: 0x01)            */
#define LPCOMP_CMP0_SW_CMP0_VN0_Pos       (7UL)                 /*!< LPCOMP CMP0_SW: CMP0_VN0 (Bit 7)                          */
#define LPCOMP_CMP0_SW_CMP0_VN0_Msk       (0x80UL)              /*!< LPCOMP CMP0_SW: CMP0_VN0 (Bitfield-Mask: 0x01)            */
/* =================================================  LPCOMP.CMP0_SW_CLEAR  ================================================== */
#define LPCOMP_CMP0_SW_CLEAR_CMP0_IP0_Pos (0UL)                 /*!< LPCOMP CMP0_SW_CLEAR: CMP0_IP0 (Bit 0)                    */
#define LPCOMP_CMP0_SW_CLEAR_CMP0_IP0_Msk (0x1UL)               /*!< LPCOMP CMP0_SW_CLEAR: CMP0_IP0 (Bitfield-Mask: 0x01)      */
#define LPCOMP_CMP0_SW_CLEAR_CMP0_AP0_Pos (1UL)                 /*!< LPCOMP CMP0_SW_CLEAR: CMP0_AP0 (Bit 1)                    */
#define LPCOMP_CMP0_SW_CLEAR_CMP0_AP0_Msk (0x2UL)               /*!< LPCOMP CMP0_SW_CLEAR: CMP0_AP0 (Bitfield-Mask: 0x01)      */
#define LPCOMP_CMP0_SW_CLEAR_CMP0_BP0_Pos (2UL)                 /*!< LPCOMP CMP0_SW_CLEAR: CMP0_BP0 (Bit 2)                    */
#define LPCOMP_CMP0_SW_CLEAR_CMP0_BP0_Msk (0x4UL)               /*!< LPCOMP CMP0_SW_CLEAR: CMP0_BP0 (Bitfield-Mask: 0x01)      */
#define LPCOMP_CMP0_SW_CLEAR_CMP0_IN0_Pos (4UL)                 /*!< LPCOMP CMP0_SW_CLEAR: CMP0_IN0 (Bit 4)                    */
#define LPCOMP_CMP0_SW_CLEAR_CMP0_IN0_Msk (0x10UL)              /*!< LPCOMP CMP0_SW_CLEAR: CMP0_IN0 (Bitfield-Mask: 0x01)      */
#define LPCOMP_CMP0_SW_CLEAR_CMP0_AN0_Pos (5UL)                 /*!< LPCOMP CMP0_SW_CLEAR: CMP0_AN0 (Bit 5)                    */
#define LPCOMP_CMP0_SW_CLEAR_CMP0_AN0_Msk (0x20UL)              /*!< LPCOMP CMP0_SW_CLEAR: CMP0_AN0 (Bitfield-Mask: 0x01)      */
#define LPCOMP_CMP0_SW_CLEAR_CMP0_BN0_Pos (6UL)                 /*!< LPCOMP CMP0_SW_CLEAR: CMP0_BN0 (Bit 6)                    */
#define LPCOMP_CMP0_SW_CLEAR_CMP0_BN0_Msk (0x40UL)              /*!< LPCOMP CMP0_SW_CLEAR: CMP0_BN0 (Bitfield-Mask: 0x01)      */
#define LPCOMP_CMP0_SW_CLEAR_CMP0_VN0_Pos (7UL)                 /*!< LPCOMP CMP0_SW_CLEAR: CMP0_VN0 (Bit 7)                    */
#define LPCOMP_CMP0_SW_CLEAR_CMP0_VN0_Msk (0x80UL)              /*!< LPCOMP CMP0_SW_CLEAR: CMP0_VN0 (Bitfield-Mask: 0x01)      */
/* ===================================================  LPCOMP.CMP1_CTRL  ==================================================== */
#define LPCOMP_CMP1_CTRL_MODE1_Pos        (0UL)                 /*!< LPCOMP CMP1_CTRL: MODE1 (Bit 0)                           */
#define LPCOMP_CMP1_CTRL_MODE1_Msk        (0x3UL)               /*!< LPCOMP CMP1_CTRL: MODE1 (Bitfield-Mask: 0x03)             */
#define LPCOMP_CMP1_CTRL_HYST1_Pos        (5UL)                 /*!< LPCOMP CMP1_CTRL: HYST1 (Bit 5)                           */
#define LPCOMP_CMP1_CTRL_HYST1_Msk        (0x20UL)              /*!< LPCOMP CMP1_CTRL: HYST1 (Bitfield-Mask: 0x01)             */
#define LPCOMP_CMP1_CTRL_INTTYPE1_Pos     (6UL)                 /*!< LPCOMP CMP1_CTRL: INTTYPE1 (Bit 6)                        */
#define LPCOMP_CMP1_CTRL_INTTYPE1_Msk     (0xc0UL)              /*!< LPCOMP CMP1_CTRL: INTTYPE1 (Bitfield-Mask: 0x03)          */
#define LPCOMP_CMP1_CTRL_DSI_BYPASS1_Pos  (10UL)                /*!< LPCOMP CMP1_CTRL: DSI_BYPASS1 (Bit 10)                    */
#define LPCOMP_CMP1_CTRL_DSI_BYPASS1_Msk  (0x400UL)             /*!< LPCOMP CMP1_CTRL: DSI_BYPASS1 (Bitfield-Mask: 0x01)       */
#define LPCOMP_CMP1_CTRL_DSI_LEVEL1_Pos   (11UL)                /*!< LPCOMP CMP1_CTRL: DSI_LEVEL1 (Bit 11)                     */
#define LPCOMP_CMP1_CTRL_DSI_LEVEL1_Msk   (0x800UL)             /*!< LPCOMP CMP1_CTRL: DSI_LEVEL1 (Bitfield-Mask: 0x01)        */
/* ====================================================  LPCOMP.CMP1_SW  ===================================================== */
#define LPCOMP_CMP1_SW_CMP1_IP1_Pos       (0UL)                 /*!< LPCOMP CMP1_SW: CMP1_IP1 (Bit 0)                          */
#define LPCOMP_CMP1_SW_CMP1_IP1_Msk       (0x1UL)               /*!< LPCOMP CMP1_SW: CMP1_IP1 (Bitfield-Mask: 0x01)            */
#define LPCOMP_CMP1_SW_CMP1_AP1_Pos       (1UL)                 /*!< LPCOMP CMP1_SW: CMP1_AP1 (Bit 1)                          */
#define LPCOMP_CMP1_SW_CMP1_AP1_Msk       (0x2UL)               /*!< LPCOMP CMP1_SW: CMP1_AP1 (Bitfield-Mask: 0x01)            */
#define LPCOMP_CMP1_SW_CMP1_BP1_Pos       (2UL)                 /*!< LPCOMP CMP1_SW: CMP1_BP1 (Bit 2)                          */
#define LPCOMP_CMP1_SW_CMP1_BP1_Msk       (0x4UL)               /*!< LPCOMP CMP1_SW: CMP1_BP1 (Bitfield-Mask: 0x01)            */
#define LPCOMP_CMP1_SW_CMP1_IN1_Pos       (4UL)                 /*!< LPCOMP CMP1_SW: CMP1_IN1 (Bit 4)                          */
#define LPCOMP_CMP1_SW_CMP1_IN1_Msk       (0x10UL)              /*!< LPCOMP CMP1_SW: CMP1_IN1 (Bitfield-Mask: 0x01)            */
#define LPCOMP_CMP1_SW_CMP1_AN1_Pos       (5UL)                 /*!< LPCOMP CMP1_SW: CMP1_AN1 (Bit 5)                          */
#define LPCOMP_CMP1_SW_CMP1_AN1_Msk       (0x20UL)              /*!< LPCOMP CMP1_SW: CMP1_AN1 (Bitfield-Mask: 0x01)            */
#define LPCOMP_CMP1_SW_CMP1_BN1_Pos       (6UL)                 /*!< LPCOMP CMP1_SW: CMP1_BN1 (Bit 6)                          */
#define LPCOMP_CMP1_SW_CMP1_BN1_Msk       (0x40UL)              /*!< LPCOMP CMP1_SW: CMP1_BN1 (Bitfield-Mask: 0x01)            */
#define LPCOMP_CMP1_SW_CMP1_VN1_Pos       (7UL)                 /*!< LPCOMP CMP1_SW: CMP1_VN1 (Bit 7)                          */
#define LPCOMP_CMP1_SW_CMP1_VN1_Msk       (0x80UL)              /*!< LPCOMP CMP1_SW: CMP1_VN1 (Bitfield-Mask: 0x01)            */
/* =================================================  LPCOMP.CMP1_SW_CLEAR  ================================================== */
#define LPCOMP_CMP1_SW_CLEAR_CMP1_IP1_Pos (0UL)                 /*!< LPCOMP CMP1_SW_CLEAR: CMP1_IP1 (Bit 0)                    */
#define LPCOMP_CMP1_SW_CLEAR_CMP1_IP1_Msk (0x1UL)               /*!< LPCOMP CMP1_SW_CLEAR: CMP1_IP1 (Bitfield-Mask: 0x01)      */
#define LPCOMP_CMP1_SW_CLEAR_CMP1_AP1_Pos (1UL)                 /*!< LPCOMP CMP1_SW_CLEAR: CMP1_AP1 (Bit 1)                    */
#define LPCOMP_CMP1_SW_CLEAR_CMP1_AP1_Msk (0x2UL)               /*!< LPCOMP CMP1_SW_CLEAR: CMP1_AP1 (Bitfield-Mask: 0x01)      */
#define LPCOMP_CMP1_SW_CLEAR_CMP1_BP1_Pos (2UL)                 /*!< LPCOMP CMP1_SW_CLEAR: CMP1_BP1 (Bit 2)                    */
#define LPCOMP_CMP1_SW_CLEAR_CMP1_BP1_Msk (0x4UL)               /*!< LPCOMP CMP1_SW_CLEAR: CMP1_BP1 (Bitfield-Mask: 0x01)      */
#define LPCOMP_CMP1_SW_CLEAR_CMP1_IN1_Pos (4UL)                 /*!< LPCOMP CMP1_SW_CLEAR: CMP1_IN1 (Bit 4)                    */
#define LPCOMP_CMP1_SW_CLEAR_CMP1_IN1_Msk (0x10UL)              /*!< LPCOMP CMP1_SW_CLEAR: CMP1_IN1 (Bitfield-Mask: 0x01)      */
#define LPCOMP_CMP1_SW_CLEAR_CMP1_AN1_Pos (5UL)                 /*!< LPCOMP CMP1_SW_CLEAR: CMP1_AN1 (Bit 5)                    */
#define LPCOMP_CMP1_SW_CLEAR_CMP1_AN1_Msk (0x20UL)              /*!< LPCOMP CMP1_SW_CLEAR: CMP1_AN1 (Bitfield-Mask: 0x01)      */
#define LPCOMP_CMP1_SW_CLEAR_CMP1_BN1_Pos (6UL)                 /*!< LPCOMP CMP1_SW_CLEAR: CMP1_BN1 (Bit 6)                    */
#define LPCOMP_CMP1_SW_CLEAR_CMP1_BN1_Msk (0x40UL)              /*!< LPCOMP CMP1_SW_CLEAR: CMP1_BN1 (Bitfield-Mask: 0x01)      */
#define LPCOMP_CMP1_SW_CLEAR_CMP1_VN1_Pos (7UL)                 /*!< LPCOMP CMP1_SW_CLEAR: CMP1_VN1 (Bit 7)                    */
#define LPCOMP_CMP1_SW_CLEAR_CMP1_VN1_Msk (0x80UL)              /*!< LPCOMP CMP1_SW_CLEAR: CMP1_VN1 (Bitfield-Mask: 0x01)      */


#endif /* _CYIP_LPCOMP_H_ */


/* [] END OF FILE */
