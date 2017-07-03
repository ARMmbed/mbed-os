/***************************************************************************//**
* \file cyip_ctdac.h
*
* \brief
* CTDAC IP definitions
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

#ifndef _CYIP_CTDAC_H_
#define _CYIP_CTDAC_H_

#include "cyip_headers.h"

/* =========================================================================================================================== */
/* ================                                           CTDAC                                           ================ */
/* =========================================================================================================================== */

#define CTDAC_SECTION_SIZE          0x00010000UL

/**
  * \brief Continuous Time DAC (CTDAC)
  */
typedef struct {                                /*!< CTDAC Structure                                                           */
  __IOM uint32_t CTDAC_CTRL;                    /*!< 0x00000000 Global CTDAC control                                           */
   __IM uint32_t RESERVED[7];
  __IOM uint32_t INTR;                          /*!< 0x00000020 Interrupt request register                                     */
  __IOM uint32_t INTR_SET;                      /*!< 0x00000024 Interrupt request set register                                 */
  __IOM uint32_t INTR_MASK;                     /*!< 0x00000028 Interrupt request mask                                         */
   __IM uint32_t INTR_MASKED;                   /*!< 0x0000002C Interrupt request masked                                       */
   __IM uint32_t RESERVED1[32];
  __IOM uint32_t CTDAC_SW;                      /*!< 0x000000B0 CTDAC switch control                                           */
  __IOM uint32_t CTDAC_SW_CLEAR;                /*!< 0x000000B4 CTDAC switch control clear                                     */
   __IM uint32_t RESERVED2[18];
  __IOM uint32_t CTDAC_VAL;                     /*!< 0x00000100 DAC Value                                                      */
  __IOM uint32_t CTDAC_VAL_NXT;                 /*!< 0x00000104 Next DAC value (double buffering)                              */
   __IM uint32_t RESERVED3[894];
  __IOM uint32_t TRIM;                          /*!< 0x00000F00 Trim                                                           */
} CTDAC_Type;                                   /*!< Size = 3844 (0xF04)                                                       */


/* ===================================================  CTDAC.CTDAC_CTRL  ==================================================== */
#define CTDAC_CTDAC_CTRL_DEGLITCH_CNT_Pos (0UL)                 /*!< CTDAC CTDAC_CTRL: DEGLITCH_CNT (Bit 0)                    */
#define CTDAC_CTDAC_CTRL_DEGLITCH_CNT_Msk (0x3fUL)              /*!< CTDAC CTDAC_CTRL: DEGLITCH_CNT (Bitfield-Mask: 0x3f)      */
#define CTDAC_CTDAC_CTRL_DEGLITCH_CO6_Pos (8UL)                 /*!< CTDAC CTDAC_CTRL: DEGLITCH_CO6 (Bit 8)                    */
#define CTDAC_CTDAC_CTRL_DEGLITCH_CO6_Msk (0x100UL)             /*!< CTDAC CTDAC_CTRL: DEGLITCH_CO6 (Bitfield-Mask: 0x01)      */
#define CTDAC_CTDAC_CTRL_DEGLITCH_COS_Pos (9UL)                 /*!< CTDAC CTDAC_CTRL: DEGLITCH_COS (Bit 9)                    */
#define CTDAC_CTDAC_CTRL_DEGLITCH_COS_Msk (0x200UL)             /*!< CTDAC CTDAC_CTRL: DEGLITCH_COS (Bitfield-Mask: 0x01)      */
#define CTDAC_CTDAC_CTRL_OUT_EN_Pos       (22UL)                /*!< CTDAC CTDAC_CTRL: OUT_EN (Bit 22)                         */
#define CTDAC_CTDAC_CTRL_OUT_EN_Msk       (0x400000UL)          /*!< CTDAC CTDAC_CTRL: OUT_EN (Bitfield-Mask: 0x01)            */
#define CTDAC_CTDAC_CTRL_CTDAC_RANGE_Pos  (23UL)                /*!< CTDAC CTDAC_CTRL: CTDAC_RANGE (Bit 23)                    */
#define CTDAC_CTDAC_CTRL_CTDAC_RANGE_Msk  (0x800000UL)          /*!< CTDAC CTDAC_CTRL: CTDAC_RANGE (Bitfield-Mask: 0x01)       */
#define CTDAC_CTDAC_CTRL_CTDAC_MODE_Pos   (24UL)                /*!< CTDAC CTDAC_CTRL: CTDAC_MODE (Bit 24)                     */
#define CTDAC_CTDAC_CTRL_CTDAC_MODE_Msk   (0x3000000UL)         /*!< CTDAC CTDAC_CTRL: CTDAC_MODE (Bitfield-Mask: 0x03)        */
#define CTDAC_CTDAC_CTRL_DISABLED_MODE_Pos (27UL)               /*!< CTDAC CTDAC_CTRL: DISABLED_MODE (Bit 27)                  */
#define CTDAC_CTDAC_CTRL_DISABLED_MODE_Msk (0x8000000UL)        /*!< CTDAC CTDAC_CTRL: DISABLED_MODE (Bitfield-Mask: 0x01)     */
#define CTDAC_CTDAC_CTRL_DSI_STROBE_EN_Pos (28UL)               /*!< CTDAC CTDAC_CTRL: DSI_STROBE_EN (Bit 28)                  */
#define CTDAC_CTDAC_CTRL_DSI_STROBE_EN_Msk (0x10000000UL)       /*!< CTDAC CTDAC_CTRL: DSI_STROBE_EN (Bitfield-Mask: 0x01)     */
#define CTDAC_CTDAC_CTRL_DSI_STROBE_LEVEL_Pos (29UL)            /*!< CTDAC CTDAC_CTRL: DSI_STROBE_LEVEL (Bit 29)               */
#define CTDAC_CTDAC_CTRL_DSI_STROBE_LEVEL_Msk (0x20000000UL)    /*!< CTDAC CTDAC_CTRL: DSI_STROBE_LEVEL (Bitfield-Mask: 0x01)  */
#define CTDAC_CTDAC_CTRL_DEEPSLEEP_ON_Pos (30UL)                /*!< CTDAC CTDAC_CTRL: DEEPSLEEP_ON (Bit 30)                   */
#define CTDAC_CTDAC_CTRL_DEEPSLEEP_ON_Msk (0x40000000UL)        /*!< CTDAC CTDAC_CTRL: DEEPSLEEP_ON (Bitfield-Mask: 0x01)      */
#define CTDAC_CTDAC_CTRL_ENABLED_Pos      (31UL)                /*!< CTDAC CTDAC_CTRL: ENABLED (Bit 31)                        */
#define CTDAC_CTDAC_CTRL_ENABLED_Msk      (0x80000000UL)        /*!< CTDAC CTDAC_CTRL: ENABLED (Bitfield-Mask: 0x01)           */
/* ======================================================  CTDAC.INTR  ======================================================= */
#define CTDAC_INTR_VDAC_EMPTY_Pos         (0UL)                 /*!< CTDAC INTR: VDAC_EMPTY (Bit 0)                            */
#define CTDAC_INTR_VDAC_EMPTY_Msk         (0x1UL)               /*!< CTDAC INTR: VDAC_EMPTY (Bitfield-Mask: 0x01)              */
/* ====================================================  CTDAC.INTR_SET  ===================================================== */
#define CTDAC_INTR_SET_VDAC_EMPTY_SET_Pos (0UL)                 /*!< CTDAC INTR_SET: VDAC_EMPTY_SET (Bit 0)                    */
#define CTDAC_INTR_SET_VDAC_EMPTY_SET_Msk (0x1UL)               /*!< CTDAC INTR_SET: VDAC_EMPTY_SET (Bitfield-Mask: 0x01)      */
/* ====================================================  CTDAC.INTR_MASK  ==================================================== */
#define CTDAC_INTR_MASK_VDAC_EMPTY_MASK_Pos (0UL)               /*!< CTDAC INTR_MASK: VDAC_EMPTY_MASK (Bit 0)                  */
#define CTDAC_INTR_MASK_VDAC_EMPTY_MASK_Msk (0x1UL)             /*!< CTDAC INTR_MASK: VDAC_EMPTY_MASK (Bitfield-Mask: 0x01)    */
/* ===================================================  CTDAC.INTR_MASKED  =================================================== */
#define CTDAC_INTR_MASKED_VDAC_EMPTY_MASKED_Pos (0UL)           /*!< CTDAC INTR_MASKED: VDAC_EMPTY_MASKED (Bit 0)              */
#define CTDAC_INTR_MASKED_VDAC_EMPTY_MASKED_Msk (0x1UL)         /*!< CTDAC INTR_MASKED: VDAC_EMPTY_MASKED (Bitfield-Mask: 0x01)*/
/* ====================================================  CTDAC.CTDAC_SW  ===================================================== */
#define CTDAC_CTDAC_SW_CTDD_CVD_Pos       (0UL)                 /*!< CTDAC CTDAC_SW: CTDD_CVD (Bit 0)                          */
#define CTDAC_CTDAC_SW_CTDD_CVD_Msk       (0x1UL)               /*!< CTDAC CTDAC_SW: CTDD_CVD (Bitfield-Mask: 0x01)            */
#define CTDAC_CTDAC_SW_CTDO_CO6_Pos       (8UL)                 /*!< CTDAC CTDAC_SW: CTDO_CO6 (Bit 8)                          */
#define CTDAC_CTDAC_SW_CTDO_CO6_Msk       (0x100UL)             /*!< CTDAC CTDAC_SW: CTDO_CO6 (Bitfield-Mask: 0x01)            */
/* =================================================  CTDAC.CTDAC_SW_CLEAR  ================================================== */
#define CTDAC_CTDAC_SW_CLEAR_CTDD_CVD_Pos (0UL)                 /*!< CTDAC CTDAC_SW_CLEAR: CTDD_CVD (Bit 0)                    */
#define CTDAC_CTDAC_SW_CLEAR_CTDD_CVD_Msk (0x1UL)               /*!< CTDAC CTDAC_SW_CLEAR: CTDD_CVD (Bitfield-Mask: 0x01)      */
#define CTDAC_CTDAC_SW_CLEAR_CTDO_CO6_Pos (8UL)                 /*!< CTDAC CTDAC_SW_CLEAR: CTDO_CO6 (Bit 8)                    */
#define CTDAC_CTDAC_SW_CLEAR_CTDO_CO6_Msk (0x100UL)             /*!< CTDAC CTDAC_SW_CLEAR: CTDO_CO6 (Bitfield-Mask: 0x01)      */
/* ====================================================  CTDAC.CTDAC_VAL  ==================================================== */
#define CTDAC_CTDAC_VAL_VALUE_Pos         (0UL)                 /*!< CTDAC CTDAC_VAL: VALUE (Bit 0)                            */
#define CTDAC_CTDAC_VAL_VALUE_Msk         (0xfffUL)             /*!< CTDAC CTDAC_VAL: VALUE (Bitfield-Mask: 0xfff)             */
/* ==================================================  CTDAC.CTDAC_VAL_NXT  ================================================== */
#define CTDAC_CTDAC_VAL_NXT_VALUE_Pos     (0UL)                 /*!< CTDAC CTDAC_VAL_NXT: VALUE (Bit 0)                        */
#define CTDAC_CTDAC_VAL_NXT_VALUE_Msk     (0xfffUL)             /*!< CTDAC CTDAC_VAL_NXT: VALUE (Bitfield-Mask: 0xfff)         */
/* ======================================================  CTDAC.TRIM  ======================================================= */
#define CTDAC_TRIM_TRIM_VAL_Pos           (0UL)                 /*!< CTDAC TRIM: TRIM_VAL (Bit 0)                              */
#define CTDAC_TRIM_TRIM_VAL_Msk           (0xfUL)               /*!< CTDAC TRIM: TRIM_VAL (Bitfield-Mask: 0x0f)                */


#endif /* _CYIP_CTDAC_H_ */


/* [] END OF FILE */
