/***************************************************************************//**
* \file cyip_lcd.h
*
* \brief
* LCD IP definitions
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

#ifndef _CYIP_LCD_H_
#define _CYIP_LCD_H_

#include "cyip_headers.h"

/* =========================================================================================================================== */
/* ================                                            LCD                                            ================ */
/* =========================================================================================================================== */

#define LCD_SECTION_SIZE            0x00010000UL

/**
  * \brief LCD Controller Block (LCD)
  */
typedef struct {                                /*!< LCD Structure                                                             */
   __IM uint32_t ID;                            /*!< 0x00000000 ID & Revision                                                  */
  __IOM uint32_t DIVIDER;                       /*!< 0x00000004 LCD Divider Register                                           */
  __IOM uint32_t CONTROL;                       /*!< 0x00000008 LCD Configuration Register                                     */
   __IM uint32_t RESERVED[61];
  __IOM uint32_t DATA0[8];                      /*!< 0x00000100 LCD Pin Data Registers                                         */
   __IM uint32_t RESERVED1[56];
  __IOM uint32_t DATA1[8];                      /*!< 0x00000200 LCD Pin Data Registers                                         */
   __IM uint32_t RESERVED2[56];
  __IOM uint32_t DATA2[8];                      /*!< 0x00000300 LCD Pin Data Registers                                         */
   __IM uint32_t RESERVED3[56];
  __IOM uint32_t DATA3[8];                      /*!< 0x00000400 LCD Pin Data Registers                                         */
} LCD_Type;                                     /*!< Size = 1056 (0x420)                                                       */


/* ========================================================  LCD.ID  ========================================================= */
#define LCD_ID_ID_Pos                     (0UL)                 /*!< LCD ID: ID (Bit 0)                                        */
#define LCD_ID_ID_Msk                     (0xffffUL)            /*!< LCD ID: ID (Bitfield-Mask: 0xffff)                        */
#define LCD_ID_REVISION_Pos               (16UL)                /*!< LCD ID: REVISION (Bit 16)                                 */
#define LCD_ID_REVISION_Msk               (0xffff0000UL)        /*!< LCD ID: REVISION (Bitfield-Mask: 0xffff)                  */
/* ======================================================  LCD.DIVIDER  ====================================================== */
#define LCD_DIVIDER_SUBFR_DIV_Pos         (0UL)                 /*!< LCD DIVIDER: SUBFR_DIV (Bit 0)                            */
#define LCD_DIVIDER_SUBFR_DIV_Msk         (0xffffUL)            /*!< LCD DIVIDER: SUBFR_DIV (Bitfield-Mask: 0xffff)            */
#define LCD_DIVIDER_DEAD_DIV_Pos          (16UL)                /*!< LCD DIVIDER: DEAD_DIV (Bit 16)                            */
#define LCD_DIVIDER_DEAD_DIV_Msk          (0xffff0000UL)        /*!< LCD DIVIDER: DEAD_DIV (Bitfield-Mask: 0xffff)             */
/* ======================================================  LCD.CONTROL  ====================================================== */
#define LCD_CONTROL_LS_EN_Pos             (0UL)                 /*!< LCD CONTROL: LS_EN (Bit 0)                                */
#define LCD_CONTROL_LS_EN_Msk             (0x1UL)               /*!< LCD CONTROL: LS_EN (Bitfield-Mask: 0x01)                  */
#define LCD_CONTROL_HS_EN_Pos             (1UL)                 /*!< LCD CONTROL: HS_EN (Bit 1)                                */
#define LCD_CONTROL_HS_EN_Msk             (0x2UL)               /*!< LCD CONTROL: HS_EN (Bitfield-Mask: 0x01)                  */
#define LCD_CONTROL_LCD_MODE_Pos          (2UL)                 /*!< LCD CONTROL: LCD_MODE (Bit 2)                             */
#define LCD_CONTROL_LCD_MODE_Msk          (0x4UL)               /*!< LCD CONTROL: LCD_MODE (Bitfield-Mask: 0x01)               */
#define LCD_CONTROL_TYPE_Pos              (3UL)                 /*!< LCD CONTROL: TYPE (Bit 3)                                 */
#define LCD_CONTROL_TYPE_Msk              (0x8UL)               /*!< LCD CONTROL: TYPE (Bitfield-Mask: 0x01)                   */
#define LCD_CONTROL_OP_MODE_Pos           (4UL)                 /*!< LCD CONTROL: OP_MODE (Bit 4)                              */
#define LCD_CONTROL_OP_MODE_Msk           (0x10UL)              /*!< LCD CONTROL: OP_MODE (Bitfield-Mask: 0x01)                */
#define LCD_CONTROL_BIAS_Pos              (5UL)                 /*!< LCD CONTROL: BIAS (Bit 5)                                 */
#define LCD_CONTROL_BIAS_Msk              (0x60UL)              /*!< LCD CONTROL: BIAS (Bitfield-Mask: 0x03)                   */
#define LCD_CONTROL_COM_NUM_Pos           (8UL)                 /*!< LCD CONTROL: COM_NUM (Bit 8)                              */
#define LCD_CONTROL_COM_NUM_Msk           (0xf00UL)             /*!< LCD CONTROL: COM_NUM (Bitfield-Mask: 0x0f)                */
#define LCD_CONTROL_LS_EN_STAT_Pos        (31UL)                /*!< LCD CONTROL: LS_EN_STAT (Bit 31)                          */
#define LCD_CONTROL_LS_EN_STAT_Msk        (0x80000000UL)        /*!< LCD CONTROL: LS_EN_STAT (Bitfield-Mask: 0x01)             */
/* =======================================================  LCD.DATA0  ======================================================= */
#define LCD_DATA0_DATA_Pos                (0UL)                 /*!< LCD DATA0: DATA (Bit 0)                                   */
#define LCD_DATA0_DATA_Msk                (0xffffffffUL)        /*!< LCD DATA0: DATA (Bitfield-Mask: 0xffffffff)               */
/* =======================================================  LCD.DATA1  ======================================================= */
#define LCD_DATA1_DATA_Pos                (0UL)                 /*!< LCD DATA1: DATA (Bit 0)                                   */
#define LCD_DATA1_DATA_Msk                (0xffffffffUL)        /*!< LCD DATA1: DATA (Bitfield-Mask: 0xffffffff)               */
/* =======================================================  LCD.DATA2  ======================================================= */
#define LCD_DATA2_DATA_Pos                (0UL)                 /*!< LCD DATA2: DATA (Bit 0)                                   */
#define LCD_DATA2_DATA_Msk                (0xffffffffUL)        /*!< LCD DATA2: DATA (Bitfield-Mask: 0xffffffff)               */
/* =======================================================  LCD.DATA3  ======================================================= */
#define LCD_DATA3_DATA_Pos                (0UL)                 /*!< LCD DATA3: DATA (Bit 0)                                   */
#define LCD_DATA3_DATA_Msk                (0xffffffffUL)        /*!< LCD DATA3: DATA (Bitfield-Mask: 0xffffffff)               */


#endif /* _CYIP_LCD_H_ */


/* [] END OF FILE */
