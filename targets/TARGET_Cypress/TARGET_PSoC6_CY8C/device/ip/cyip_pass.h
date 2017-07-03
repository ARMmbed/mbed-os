/***************************************************************************//**
* \file cyip_pass.h
*
* \brief
* PASS IP definitions
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

#ifndef _CYIP_PASS_H_
#define _CYIP_PASS_H_

#include "cyip_headers.h"

/* =========================================================================================================================== */
/* ================                                           PASS                                            ================ */
/* =========================================================================================================================== */

#define PASS_AREF_SECTION_SIZE      0x00000100UL
#define PASS_SECTION_SIZE           0x00010000UL

/**
  * \brief AREF configuration (PASS_AREF)
  */
typedef struct {
  __IOM uint32_t AREF_CTRL;                     /*!< 0x00000000 global AREF control                                            */
   __IM uint32_t RESERVED[63];
} PASS_AREF_Type;                               /*!< Size = 4 (0x4)                                                            */

/**
  * \brief PASS top-level MMIO (DSABv2, INTR) (PASS)
  */
typedef struct {                                /*!< PASS Structure                                                            */
   __IM uint32_t INTR_CAUSE;                    /*!< 0x00000000 Interrupt cause register                                       */
   __IM uint32_t RESERVED[895];
        PASS_AREF_Type AREF;                    /*!< 0x00000E00 AREF configuration                                             */
  __IOM uint32_t VREF_TRIM0;                    /*!< 0x00000F00 VREF Trim bits                                                 */
  __IOM uint32_t VREF_TRIM1;                    /*!< 0x00000F04 VREF Trim bits                                                 */
  __IOM uint32_t VREF_TRIM2;                    /*!< 0x00000F08 VREF Trim bits                                                 */
  __IOM uint32_t VREF_TRIM3;                    /*!< 0x00000F0C VREF Trim bits                                                 */
  __IOM uint32_t IZTAT_TRIM0;                   /*!< 0x00000F10 IZTAT Trim bits                                                */
  __IOM uint32_t IZTAT_TRIM1;                   /*!< 0x00000F14 IZTAT Trim bits                                                */
  __IOM uint32_t IPTAT_TRIM0;                   /*!< 0x00000F18 IPTAT Trim bits                                                */
  __IOM uint32_t ICTAT_TRIM0;                   /*!< 0x00000F1C ICTAT Trim bits                                                */
} PASS_Type;                                    /*!< Size = 3872 (0xF20)                                                       */


/* ==================================================  PASS_AREF.AREF_CTRL  ================================================== */
#define PASS_AREF_AREF_CTRL_AREF_MODE_Pos (0UL)                 /*!< PASS_AREF AREF_CTRL: AREF_MODE (Bit 0)                    */
#define PASS_AREF_AREF_CTRL_AREF_MODE_Msk (0x1UL)               /*!< PASS_AREF AREF_CTRL: AREF_MODE (Bitfield-Mask: 0x01)      */
#define PASS_AREF_AREF_CTRL_AREF_BIAS_SCALE_Pos (2UL)           /*!< PASS_AREF AREF_CTRL: AREF_BIAS_SCALE (Bit 2)              */
#define PASS_AREF_AREF_CTRL_AREF_BIAS_SCALE_Msk (0xcUL)         /*!< PASS_AREF AREF_CTRL: AREF_BIAS_SCALE (Bitfield-Mask: 0x03)*/
#define PASS_AREF_AREF_CTRL_AREF_RMB_Pos  (4UL)                 /*!< PASS_AREF AREF_CTRL: AREF_RMB (Bit 4)                     */
#define PASS_AREF_AREF_CTRL_AREF_RMB_Msk  (0x70UL)              /*!< PASS_AREF AREF_CTRL: AREF_RMB (Bitfield-Mask: 0x07)       */
#define PASS_AREF_AREF_CTRL_CTB_IPTAT_SCALE_Pos (7UL)           /*!< PASS_AREF AREF_CTRL: CTB_IPTAT_SCALE (Bit 7)              */
#define PASS_AREF_AREF_CTRL_CTB_IPTAT_SCALE_Msk (0x80UL)        /*!< PASS_AREF AREF_CTRL: CTB_IPTAT_SCALE (Bitfield-Mask: 0x01)*/
#define PASS_AREF_AREF_CTRL_CTB_IPTAT_REDIRECT_Pos (8UL)        /*!< PASS_AREF AREF_CTRL: CTB_IPTAT_REDIRECT (Bit 8)           */
#define PASS_AREF_AREF_CTRL_CTB_IPTAT_REDIRECT_Msk (0xff00UL)   /*!< PASS_AREF AREF_CTRL: CTB_IPTAT_REDIRECT (Bitfield-Mask: 0xff)*/
#define PASS_AREF_AREF_CTRL_IZTAT_SEL_Pos (16UL)                /*!< PASS_AREF AREF_CTRL: IZTAT_SEL (Bit 16)                   */
#define PASS_AREF_AREF_CTRL_IZTAT_SEL_Msk (0x10000UL)           /*!< PASS_AREF AREF_CTRL: IZTAT_SEL (Bitfield-Mask: 0x01)      */
#define PASS_AREF_AREF_CTRL_CLOCK_PUMP_PERI_SEL_Pos (19UL)      /*!< PASS_AREF AREF_CTRL: CLOCK_PUMP_PERI_SEL (Bit 19)         */
#define PASS_AREF_AREF_CTRL_CLOCK_PUMP_PERI_SEL_Msk (0x80000UL) /*!< PASS_AREF AREF_CTRL: CLOCK_PUMP_PERI_SEL (Bitfield-Mask: 0x01)*/
#define PASS_AREF_AREF_CTRL_VREF_SEL_Pos  (20UL)                /*!< PASS_AREF AREF_CTRL: VREF_SEL (Bit 20)                    */
#define PASS_AREF_AREF_CTRL_VREF_SEL_Msk  (0x300000UL)          /*!< PASS_AREF AREF_CTRL: VREF_SEL (Bitfield-Mask: 0x03)       */
#define PASS_AREF_AREF_CTRL_DEEPSLEEP_MODE_Pos (28UL)           /*!< PASS_AREF AREF_CTRL: DEEPSLEEP_MODE (Bit 28)              */
#define PASS_AREF_AREF_CTRL_DEEPSLEEP_MODE_Msk (0x30000000UL)   /*!< PASS_AREF AREF_CTRL: DEEPSLEEP_MODE (Bitfield-Mask: 0x03) */
#define PASS_AREF_AREF_CTRL_DEEPSLEEP_ON_Pos (30UL)             /*!< PASS_AREF AREF_CTRL: DEEPSLEEP_ON (Bit 30)                */
#define PASS_AREF_AREF_CTRL_DEEPSLEEP_ON_Msk (0x40000000UL)     /*!< PASS_AREF AREF_CTRL: DEEPSLEEP_ON (Bitfield-Mask: 0x01)   */
#define PASS_AREF_AREF_CTRL_ENABLED_Pos   (31UL)                /*!< PASS_AREF AREF_CTRL: ENABLED (Bit 31)                     */
#define PASS_AREF_AREF_CTRL_ENABLED_Msk   (0x80000000UL)        /*!< PASS_AREF AREF_CTRL: ENABLED (Bitfield-Mask: 0x01)        */


/* ====================================================  PASS.INTR_CAUSE  ==================================================== */
#define PASS_INTR_CAUSE_CTB0_INT_Pos      (0UL)                 /*!< PASS INTR_CAUSE: CTB0_INT (Bit 0)                         */
#define PASS_INTR_CAUSE_CTB0_INT_Msk      (0x1UL)               /*!< PASS INTR_CAUSE: CTB0_INT (Bitfield-Mask: 0x01)           */
#define PASS_INTR_CAUSE_CTB1_INT_Pos      (1UL)                 /*!< PASS INTR_CAUSE: CTB1_INT (Bit 1)                         */
#define PASS_INTR_CAUSE_CTB1_INT_Msk      (0x2UL)               /*!< PASS INTR_CAUSE: CTB1_INT (Bitfield-Mask: 0x01)           */
#define PASS_INTR_CAUSE_CTB2_INT_Pos      (2UL)                 /*!< PASS INTR_CAUSE: CTB2_INT (Bit 2)                         */
#define PASS_INTR_CAUSE_CTB2_INT_Msk      (0x4UL)               /*!< PASS INTR_CAUSE: CTB2_INT (Bitfield-Mask: 0x01)           */
#define PASS_INTR_CAUSE_CTB3_INT_Pos      (3UL)                 /*!< PASS INTR_CAUSE: CTB3_INT (Bit 3)                         */
#define PASS_INTR_CAUSE_CTB3_INT_Msk      (0x8UL)               /*!< PASS INTR_CAUSE: CTB3_INT (Bitfield-Mask: 0x01)           */
#define PASS_INTR_CAUSE_CTDAC0_INT_Pos    (4UL)                 /*!< PASS INTR_CAUSE: CTDAC0_INT (Bit 4)                       */
#define PASS_INTR_CAUSE_CTDAC0_INT_Msk    (0x10UL)              /*!< PASS INTR_CAUSE: CTDAC0_INT (Bitfield-Mask: 0x01)         */
#define PASS_INTR_CAUSE_CTDAC1_INT_Pos    (5UL)                 /*!< PASS INTR_CAUSE: CTDAC1_INT (Bit 5)                       */
#define PASS_INTR_CAUSE_CTDAC1_INT_Msk    (0x20UL)              /*!< PASS INTR_CAUSE: CTDAC1_INT (Bitfield-Mask: 0x01)         */
#define PASS_INTR_CAUSE_CTDAC2_INT_Pos    (6UL)                 /*!< PASS INTR_CAUSE: CTDAC2_INT (Bit 6)                       */
#define PASS_INTR_CAUSE_CTDAC2_INT_Msk    (0x40UL)              /*!< PASS INTR_CAUSE: CTDAC2_INT (Bitfield-Mask: 0x01)         */
#define PASS_INTR_CAUSE_CTDAC3_INT_Pos    (7UL)                 /*!< PASS INTR_CAUSE: CTDAC3_INT (Bit 7)                       */
#define PASS_INTR_CAUSE_CTDAC3_INT_Msk    (0x80UL)              /*!< PASS INTR_CAUSE: CTDAC3_INT (Bitfield-Mask: 0x01)         */
/* ====================================================  PASS.VREF_TRIM0  ==================================================== */
#define PASS_VREF_TRIM0_VREF_ABS_TRIM_Pos (0UL)                 /*!< PASS VREF_TRIM0: VREF_ABS_TRIM (Bit 0)                    */
#define PASS_VREF_TRIM0_VREF_ABS_TRIM_Msk (0xffUL)              /*!< PASS VREF_TRIM0: VREF_ABS_TRIM (Bitfield-Mask: 0xff)      */
/* ====================================================  PASS.VREF_TRIM1  ==================================================== */
#define PASS_VREF_TRIM1_VREF_TEMPCO_TRIM_Pos (0UL)              /*!< PASS VREF_TRIM1: VREF_TEMPCO_TRIM (Bit 0)                 */
#define PASS_VREF_TRIM1_VREF_TEMPCO_TRIM_Msk (0xffUL)           /*!< PASS VREF_TRIM1: VREF_TEMPCO_TRIM (Bitfield-Mask: 0xff)   */
/* ====================================================  PASS.VREF_TRIM2  ==================================================== */
#define PASS_VREF_TRIM2_VREF_CURV_TRIM_Pos (0UL)                /*!< PASS VREF_TRIM2: VREF_CURV_TRIM (Bit 0)                   */
#define PASS_VREF_TRIM2_VREF_CURV_TRIM_Msk (0xffUL)             /*!< PASS VREF_TRIM2: VREF_CURV_TRIM (Bitfield-Mask: 0xff)     */
/* ====================================================  PASS.VREF_TRIM3  ==================================================== */
#define PASS_VREF_TRIM3_VREF_ATTEN_TRIM_Pos (0UL)               /*!< PASS VREF_TRIM3: VREF_ATTEN_TRIM (Bit 0)                  */
#define PASS_VREF_TRIM3_VREF_ATTEN_TRIM_Msk (0xfUL)             /*!< PASS VREF_TRIM3: VREF_ATTEN_TRIM (Bitfield-Mask: 0x0f)    */
/* ===================================================  PASS.IZTAT_TRIM0  ==================================================== */
#define PASS_IZTAT_TRIM0_IZTAT_ABS_TRIM_Pos (0UL)               /*!< PASS IZTAT_TRIM0: IZTAT_ABS_TRIM (Bit 0)                  */
#define PASS_IZTAT_TRIM0_IZTAT_ABS_TRIM_Msk (0xffUL)            /*!< PASS IZTAT_TRIM0: IZTAT_ABS_TRIM (Bitfield-Mask: 0xff)    */
/* ===================================================  PASS.IZTAT_TRIM1  ==================================================== */
#define PASS_IZTAT_TRIM1_IZTAT_TC_TRIM_Pos (0UL)                /*!< PASS IZTAT_TRIM1: IZTAT_TC_TRIM (Bit 0)                   */
#define PASS_IZTAT_TRIM1_IZTAT_TC_TRIM_Msk (0xffUL)             /*!< PASS IZTAT_TRIM1: IZTAT_TC_TRIM (Bitfield-Mask: 0xff)     */
/* ===================================================  PASS.IPTAT_TRIM0  ==================================================== */
#define PASS_IPTAT_TRIM0_IPTAT_CORE_TRIM_Pos (0UL)              /*!< PASS IPTAT_TRIM0: IPTAT_CORE_TRIM (Bit 0)                 */
#define PASS_IPTAT_TRIM0_IPTAT_CORE_TRIM_Msk (0xfUL)            /*!< PASS IPTAT_TRIM0: IPTAT_CORE_TRIM (Bitfield-Mask: 0x0f)   */
#define PASS_IPTAT_TRIM0_IPTAT_CTBM_TRIM_Pos (4UL)              /*!< PASS IPTAT_TRIM0: IPTAT_CTBM_TRIM (Bit 4)                 */
#define PASS_IPTAT_TRIM0_IPTAT_CTBM_TRIM_Msk (0xf0UL)           /*!< PASS IPTAT_TRIM0: IPTAT_CTBM_TRIM (Bitfield-Mask: 0x0f)   */
/* ===================================================  PASS.ICTAT_TRIM0  ==================================================== */
#define PASS_ICTAT_TRIM0_ICTAT_TRIM_Pos   (0UL)                 /*!< PASS ICTAT_TRIM0: ICTAT_TRIM (Bit 0)                      */
#define PASS_ICTAT_TRIM0_ICTAT_TRIM_Msk   (0xfUL)               /*!< PASS ICTAT_TRIM0: ICTAT_TRIM (Bitfield-Mask: 0x0f)        */


#endif /* _CYIP_PASS_H_ */


/* [] END OF FILE */
