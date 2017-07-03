/***************************************************************************//**
* \file cyip_cpuss.h
*
* \brief
* CPUSS IP definitions
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

#ifndef _CYIP_CPUSS_H_
#define _CYIP_CPUSS_H_

#include "cyip_headers.h"

/* =========================================================================================================================== */
/* ================                                           CPUSS                                           ================ */
/* =========================================================================================================================== */

#define CPUSS_SECTION_SIZE          0x00010000UL

/**
  * \brief CPU subsystem (CPUSS) (CPUSS)
  */
typedef struct {                                /*!< CPUSS Structure                                                           */
  __IOM uint32_t CM0_CTL;                       /*!< 0x00000000 CM0+ control                                                   */
   __IM uint32_t RESERVED;
   __IM uint32_t CM0_STATUS;                    /*!< 0x00000008 CM0+ status                                                    */
   __IM uint32_t RESERVED1;
  __IOM uint32_t CM0_CLOCK_CTL;                 /*!< 0x00000010 CM0+ clock control                                             */
   __IM uint32_t RESERVED2[2];
  __IOM uint32_t CM0_NMI_CTL;                   /*!< 0x0000001C CM0+ NMI control                                               */
  __IOM uint32_t CM0_INT_CTL0;                  /*!< 0x00000020 CM0+ interrupt control 0                                       */
  __IOM uint32_t CM0_INT_CTL1;                  /*!< 0x00000024 CM0+ interrupt control 1                                       */
  __IOM uint32_t CM0_INT_CTL2;                  /*!< 0x00000028 CM0+ interrupt control 2                                       */
  __IOM uint32_t CM0_INT_CTL3;                  /*!< 0x0000002C CM0+ interrupt control 3                                       */
  __IOM uint32_t CM0_INT_CTL4;                  /*!< 0x00000030 CM0+ interrupt control 4                                       */
  __IOM uint32_t CM0_INT_CTL5;                  /*!< 0x00000034 CM0+ interrupt control 5                                       */
  __IOM uint32_t CM0_INT_CTL6;                  /*!< 0x00000038 CM0+ interrupt control 6                                       */
  __IOM uint32_t CM0_INT_CTL7;                  /*!< 0x0000003C CM0+ interrupt control 7                                       */
   __IM uint32_t RESERVED3[16];
  __IOM uint32_t CM4_PWR_CTL;                   /*!< 0x00000080 CM4 power control                                              */
   __IM uint32_t RESERVED4;
   __IM uint32_t CM4_STATUS;                    /*!< 0x00000088 CM4 status                                                     */
   __IM uint32_t RESERVED5;
  __IOM uint32_t CM4_CLOCK_CTL;                 /*!< 0x00000090 CM4 clock control                                              */
   __IM uint32_t RESERVED6[3];
  __IOM uint32_t CM4_NMI_CTL;                   /*!< 0x000000A0 CM4 NMI control                                                */
   __IM uint32_t RESERVED7[7];
  __IOM uint32_t UDB_PWR_CTL;                   /*!< 0x000000C0 UDB power control                                              */
   __IM uint32_t RESERVED8[15];
  __IOM uint32_t RAM0_CTL0;                     /*!< 0x00000100 RAM 0 control 0                                                */
   __IM uint32_t RESERVED9[15];
  __IOM uint32_t RAM0_PWR_MACRO_CTL[16];        /*!< 0x00000140 RAM 0 power control                                            */
  __IOM uint32_t RAM1_CTL0;                     /*!< 0x00000180 RAM 1 control 0                                                */
   __IM uint32_t RESERVED10[3];
  __IOM uint32_t RAM1_PWR_CTL;                  /*!< 0x00000190 RAM1 power control                                             */
   __IM uint32_t RESERVED11[3];
  __IOM uint32_t RAM2_CTL0;                     /*!< 0x000001A0 RAM 2 control 0                                                */
   __IM uint32_t RESERVED12[3];
  __IOM uint32_t RAM2_PWR_CTL;                  /*!< 0x000001B0 RAM2 power control                                             */
   __IM uint32_t RESERVED13[3];
  __IOM uint32_t ROM_CTL;                       /*!< 0x000001C0 ROM control                                                    */
   __IM uint32_t RESERVED14[7];
  __IOM uint32_t ETB_PWR_CTL;                   /*!< 0x000001E0 ETB power control                                              */
   __IM uint32_t RESERVED15[9];
   __IM uint32_t DP_STATUS;                     /*!< 0x00000208 Debug port status                                              */
   __IM uint32_t RESERVED16[5];
  __IOM uint32_t BUFF_CTL;                      /*!< 0x00000220 Buffer control                                                 */
   __IM uint32_t RESERVED17[7];
  __IOM uint32_t SYSTICK_CTL;                   /*!< 0x00000240 SysTick timer control                                          */
   __IM uint32_t RESERVED18[12];
   __IM uint32_t MBIST_STAT;                    /*!< 0x00000274 Memory BIST status                                             */
   __IM uint32_t RESERVED19[2];
  __IOM uint32_t WOUNDING;                      /*!< 0x00000280 Wounding                                                       */
   __IM uint32_t RESERVED20[3];
  __IOM uint32_t PROTECTION;                    /*!< 0x00000290 Protection status                                              */
   __IM uint32_t RESERVED21[3];
  __IOM uint32_t CM0_PC0_HANDLER;               /*!< 0x000002A0 CM0+ protection context 0 handler                              */
   __IM uint32_t RESERVED22[3];
  __IOM uint32_t CM0_VECTOR_TABLE_BASE;         /*!< 0x000002B0 CM0+ vector table base                                         */
   __IM uint32_t RESERVED23[3];
  __IOM uint32_t CM4_VECTOR_TABLE_BASE;         /*!< 0x000002C0 CM4 vector table base                                          */
   __IM uint32_t RESERVED24[79];
   __IM uint32_t IDENTITY;                      /*!< 0x00000400 Identity                                                       */
   __IM uint32_t RESERVED25[15103];
  __IOM uint32_t TRIM_ROM_CTL;                  /*!< 0x0000F000 ROM trim control                                               */
  __IOM uint32_t TRIM_RAM_CTL;                  /*!< 0x0000F004 RAM trim control                                               */
} CPUSS_Type;                                   /*!< Size = 61448 (0xF008)                                                     */


/* =====================================================  CPUSS.CM0_CTL  ===================================================== */
#define CPUSS_CM0_CTL_SLV_STALL_Pos       (28UL)                /*!< CPUSS CM0_CTL: SLV_STALL (Bit 28)                         */
#define CPUSS_CM0_CTL_SLV_STALL_Msk       (0x10000000UL)        /*!< CPUSS CM0_CTL: SLV_STALL (Bitfield-Mask: 0x01)            */
#define CPUSS_CM0_CTL_ENABLED_Pos         (31UL)                /*!< CPUSS CM0_CTL: ENABLED (Bit 31)                           */
#define CPUSS_CM0_CTL_ENABLED_Msk         (0x80000000UL)        /*!< CPUSS CM0_CTL: ENABLED (Bitfield-Mask: 0x01)              */
/* ===================================================  CPUSS.CM0_STATUS  ==================================================== */
#define CPUSS_CM0_STATUS_SLEEPING_Pos     (0UL)                 /*!< CPUSS CM0_STATUS: SLEEPING (Bit 0)                        */
#define CPUSS_CM0_STATUS_SLEEPING_Msk     (0x1UL)               /*!< CPUSS CM0_STATUS: SLEEPING (Bitfield-Mask: 0x01)          */
#define CPUSS_CM0_STATUS_SLEEPDEEP_Pos    (1UL)                 /*!< CPUSS CM0_STATUS: SLEEPDEEP (Bit 1)                       */
#define CPUSS_CM0_STATUS_SLEEPDEEP_Msk    (0x2UL)               /*!< CPUSS CM0_STATUS: SLEEPDEEP (Bitfield-Mask: 0x01)         */
/* ==================================================  CPUSS.CM0_CLOCK_CTL  ================================================== */
#define CPUSS_CM0_CLOCK_CTL_SLOW_INT_DIV_Pos (8UL)              /*!< CPUSS CM0_CLOCK_CTL: SLOW_INT_DIV (Bit 8)                 */
#define CPUSS_CM0_CLOCK_CTL_SLOW_INT_DIV_Msk (0xff00UL)         /*!< CPUSS CM0_CLOCK_CTL: SLOW_INT_DIV (Bitfield-Mask: 0xff)   */
#define CPUSS_CM0_CLOCK_CTL_PERI_INT_DIV_Pos (24UL)             /*!< CPUSS CM0_CLOCK_CTL: PERI_INT_DIV (Bit 24)                */
#define CPUSS_CM0_CLOCK_CTL_PERI_INT_DIV_Msk (0xff000000UL)     /*!< CPUSS CM0_CLOCK_CTL: PERI_INT_DIV (Bitfield-Mask: 0xff)   */
/* ===================================================  CPUSS.CM0_NMI_CTL  =================================================== */
#define CPUSS_CM0_NMI_CTL_MUX0_SEL_Pos    (0UL)                 /*!< CPUSS CM0_NMI_CTL: MUX0_SEL (Bit 0)                       */
#define CPUSS_CM0_NMI_CTL_MUX0_SEL_Msk    (0xffUL)              /*!< CPUSS CM0_NMI_CTL: MUX0_SEL (Bitfield-Mask: 0xff)         */
/* ==================================================  CPUSS.CM0_INT_CTL0  =================================================== */
#define CPUSS_CM0_INT_CTL0_MUX0_SEL_Pos   (0UL)                 /*!< CPUSS CM0_INT_CTL0: MUX0_SEL (Bit 0)                      */
#define CPUSS_CM0_INT_CTL0_MUX0_SEL_Msk   (0xffUL)              /*!< CPUSS CM0_INT_CTL0: MUX0_SEL (Bitfield-Mask: 0xff)        */
#define CPUSS_CM0_INT_CTL0_MUX1_SEL_Pos   (8UL)                 /*!< CPUSS CM0_INT_CTL0: MUX1_SEL (Bit 8)                      */
#define CPUSS_CM0_INT_CTL0_MUX1_SEL_Msk   (0xff00UL)            /*!< CPUSS CM0_INT_CTL0: MUX1_SEL (Bitfield-Mask: 0xff)        */
#define CPUSS_CM0_INT_CTL0_MUX2_SEL_Pos   (16UL)                /*!< CPUSS CM0_INT_CTL0: MUX2_SEL (Bit 16)                     */
#define CPUSS_CM0_INT_CTL0_MUX2_SEL_Msk   (0xff0000UL)          /*!< CPUSS CM0_INT_CTL0: MUX2_SEL (Bitfield-Mask: 0xff)        */
#define CPUSS_CM0_INT_CTL0_MUX3_SEL_Pos   (24UL)                /*!< CPUSS CM0_INT_CTL0: MUX3_SEL (Bit 24)                     */
#define CPUSS_CM0_INT_CTL0_MUX3_SEL_Msk   (0xff000000UL)        /*!< CPUSS CM0_INT_CTL0: MUX3_SEL (Bitfield-Mask: 0xff)        */
/* ==================================================  CPUSS.CM0_INT_CTL1  =================================================== */
#define CPUSS_CM0_INT_CTL1_MUX0_SEL_Pos   (0UL)                 /*!< CPUSS CM0_INT_CTL1: MUX0_SEL (Bit 0)                      */
#define CPUSS_CM0_INT_CTL1_MUX0_SEL_Msk   (0xffUL)              /*!< CPUSS CM0_INT_CTL1: MUX0_SEL (Bitfield-Mask: 0xff)        */
#define CPUSS_CM0_INT_CTL1_MUX1_SEL_Pos   (8UL)                 /*!< CPUSS CM0_INT_CTL1: MUX1_SEL (Bit 8)                      */
#define CPUSS_CM0_INT_CTL1_MUX1_SEL_Msk   (0xff00UL)            /*!< CPUSS CM0_INT_CTL1: MUX1_SEL (Bitfield-Mask: 0xff)        */
#define CPUSS_CM0_INT_CTL1_MUX2_SEL_Pos   (16UL)                /*!< CPUSS CM0_INT_CTL1: MUX2_SEL (Bit 16)                     */
#define CPUSS_CM0_INT_CTL1_MUX2_SEL_Msk   (0xff0000UL)          /*!< CPUSS CM0_INT_CTL1: MUX2_SEL (Bitfield-Mask: 0xff)        */
#define CPUSS_CM0_INT_CTL1_MUX3_SEL_Pos   (24UL)                /*!< CPUSS CM0_INT_CTL1: MUX3_SEL (Bit 24)                     */
#define CPUSS_CM0_INT_CTL1_MUX3_SEL_Msk   (0xff000000UL)        /*!< CPUSS CM0_INT_CTL1: MUX3_SEL (Bitfield-Mask: 0xff)        */
/* ==================================================  CPUSS.CM0_INT_CTL2  =================================================== */
#define CPUSS_CM0_INT_CTL2_MUX0_SEL_Pos   (0UL)                 /*!< CPUSS CM0_INT_CTL2: MUX0_SEL (Bit 0)                      */
#define CPUSS_CM0_INT_CTL2_MUX0_SEL_Msk   (0xffUL)              /*!< CPUSS CM0_INT_CTL2: MUX0_SEL (Bitfield-Mask: 0xff)        */
#define CPUSS_CM0_INT_CTL2_MUX1_SEL_Pos   (8UL)                 /*!< CPUSS CM0_INT_CTL2: MUX1_SEL (Bit 8)                      */
#define CPUSS_CM0_INT_CTL2_MUX1_SEL_Msk   (0xff00UL)            /*!< CPUSS CM0_INT_CTL2: MUX1_SEL (Bitfield-Mask: 0xff)        */
#define CPUSS_CM0_INT_CTL2_MUX2_SEL_Pos   (16UL)                /*!< CPUSS CM0_INT_CTL2: MUX2_SEL (Bit 16)                     */
#define CPUSS_CM0_INT_CTL2_MUX2_SEL_Msk   (0xff0000UL)          /*!< CPUSS CM0_INT_CTL2: MUX2_SEL (Bitfield-Mask: 0xff)        */
#define CPUSS_CM0_INT_CTL2_MUX3_SEL_Pos   (24UL)                /*!< CPUSS CM0_INT_CTL2: MUX3_SEL (Bit 24)                     */
#define CPUSS_CM0_INT_CTL2_MUX3_SEL_Msk   (0xff000000UL)        /*!< CPUSS CM0_INT_CTL2: MUX3_SEL (Bitfield-Mask: 0xff)        */
/* ==================================================  CPUSS.CM0_INT_CTL3  =================================================== */
#define CPUSS_CM0_INT_CTL3_MUX0_SEL_Pos   (0UL)                 /*!< CPUSS CM0_INT_CTL3: MUX0_SEL (Bit 0)                      */
#define CPUSS_CM0_INT_CTL3_MUX0_SEL_Msk   (0xffUL)              /*!< CPUSS CM0_INT_CTL3: MUX0_SEL (Bitfield-Mask: 0xff)        */
#define CPUSS_CM0_INT_CTL3_MUX1_SEL_Pos   (8UL)                 /*!< CPUSS CM0_INT_CTL3: MUX1_SEL (Bit 8)                      */
#define CPUSS_CM0_INT_CTL3_MUX1_SEL_Msk   (0xff00UL)            /*!< CPUSS CM0_INT_CTL3: MUX1_SEL (Bitfield-Mask: 0xff)        */
#define CPUSS_CM0_INT_CTL3_MUX2_SEL_Pos   (16UL)                /*!< CPUSS CM0_INT_CTL3: MUX2_SEL (Bit 16)                     */
#define CPUSS_CM0_INT_CTL3_MUX2_SEL_Msk   (0xff0000UL)          /*!< CPUSS CM0_INT_CTL3: MUX2_SEL (Bitfield-Mask: 0xff)        */
#define CPUSS_CM0_INT_CTL3_MUX3_SEL_Pos   (24UL)                /*!< CPUSS CM0_INT_CTL3: MUX3_SEL (Bit 24)                     */
#define CPUSS_CM0_INT_CTL3_MUX3_SEL_Msk   (0xff000000UL)        /*!< CPUSS CM0_INT_CTL3: MUX3_SEL (Bitfield-Mask: 0xff)        */
/* ==================================================  CPUSS.CM0_INT_CTL4  =================================================== */
#define CPUSS_CM0_INT_CTL4_MUX0_SEL_Pos   (0UL)                 /*!< CPUSS CM0_INT_CTL4: MUX0_SEL (Bit 0)                      */
#define CPUSS_CM0_INT_CTL4_MUX0_SEL_Msk   (0xffUL)              /*!< CPUSS CM0_INT_CTL4: MUX0_SEL (Bitfield-Mask: 0xff)        */
#define CPUSS_CM0_INT_CTL4_MUX1_SEL_Pos   (8UL)                 /*!< CPUSS CM0_INT_CTL4: MUX1_SEL (Bit 8)                      */
#define CPUSS_CM0_INT_CTL4_MUX1_SEL_Msk   (0xff00UL)            /*!< CPUSS CM0_INT_CTL4: MUX1_SEL (Bitfield-Mask: 0xff)        */
#define CPUSS_CM0_INT_CTL4_MUX2_SEL_Pos   (16UL)                /*!< CPUSS CM0_INT_CTL4: MUX2_SEL (Bit 16)                     */
#define CPUSS_CM0_INT_CTL4_MUX2_SEL_Msk   (0xff0000UL)          /*!< CPUSS CM0_INT_CTL4: MUX2_SEL (Bitfield-Mask: 0xff)        */
#define CPUSS_CM0_INT_CTL4_MUX3_SEL_Pos   (24UL)                /*!< CPUSS CM0_INT_CTL4: MUX3_SEL (Bit 24)                     */
#define CPUSS_CM0_INT_CTL4_MUX3_SEL_Msk   (0xff000000UL)        /*!< CPUSS CM0_INT_CTL4: MUX3_SEL (Bitfield-Mask: 0xff)        */
/* ==================================================  CPUSS.CM0_INT_CTL5  =================================================== */
#define CPUSS_CM0_INT_CTL5_MUX0_SEL_Pos   (0UL)                 /*!< CPUSS CM0_INT_CTL5: MUX0_SEL (Bit 0)                      */
#define CPUSS_CM0_INT_CTL5_MUX0_SEL_Msk   (0xffUL)              /*!< CPUSS CM0_INT_CTL5: MUX0_SEL (Bitfield-Mask: 0xff)        */
#define CPUSS_CM0_INT_CTL5_MUX1_SEL_Pos   (8UL)                 /*!< CPUSS CM0_INT_CTL5: MUX1_SEL (Bit 8)                      */
#define CPUSS_CM0_INT_CTL5_MUX1_SEL_Msk   (0xff00UL)            /*!< CPUSS CM0_INT_CTL5: MUX1_SEL (Bitfield-Mask: 0xff)        */
#define CPUSS_CM0_INT_CTL5_MUX2_SEL_Pos   (16UL)                /*!< CPUSS CM0_INT_CTL5: MUX2_SEL (Bit 16)                     */
#define CPUSS_CM0_INT_CTL5_MUX2_SEL_Msk   (0xff0000UL)          /*!< CPUSS CM0_INT_CTL5: MUX2_SEL (Bitfield-Mask: 0xff)        */
#define CPUSS_CM0_INT_CTL5_MUX3_SEL_Pos   (24UL)                /*!< CPUSS CM0_INT_CTL5: MUX3_SEL (Bit 24)                     */
#define CPUSS_CM0_INT_CTL5_MUX3_SEL_Msk   (0xff000000UL)        /*!< CPUSS CM0_INT_CTL5: MUX3_SEL (Bitfield-Mask: 0xff)        */
/* ==================================================  CPUSS.CM0_INT_CTL6  =================================================== */
#define CPUSS_CM0_INT_CTL6_MUX0_SEL_Pos   (0UL)                 /*!< CPUSS CM0_INT_CTL6: MUX0_SEL (Bit 0)                      */
#define CPUSS_CM0_INT_CTL6_MUX0_SEL_Msk   (0xffUL)              /*!< CPUSS CM0_INT_CTL6: MUX0_SEL (Bitfield-Mask: 0xff)        */
#define CPUSS_CM0_INT_CTL6_MUX1_SEL_Pos   (8UL)                 /*!< CPUSS CM0_INT_CTL6: MUX1_SEL (Bit 8)                      */
#define CPUSS_CM0_INT_CTL6_MUX1_SEL_Msk   (0xff00UL)            /*!< CPUSS CM0_INT_CTL6: MUX1_SEL (Bitfield-Mask: 0xff)        */
#define CPUSS_CM0_INT_CTL6_MUX2_SEL_Pos   (16UL)                /*!< CPUSS CM0_INT_CTL6: MUX2_SEL (Bit 16)                     */
#define CPUSS_CM0_INT_CTL6_MUX2_SEL_Msk   (0xff0000UL)          /*!< CPUSS CM0_INT_CTL6: MUX2_SEL (Bitfield-Mask: 0xff)        */
#define CPUSS_CM0_INT_CTL6_MUX3_SEL_Pos   (24UL)                /*!< CPUSS CM0_INT_CTL6: MUX3_SEL (Bit 24)                     */
#define CPUSS_CM0_INT_CTL6_MUX3_SEL_Msk   (0xff000000UL)        /*!< CPUSS CM0_INT_CTL6: MUX3_SEL (Bitfield-Mask: 0xff)        */
/* ==================================================  CPUSS.CM0_INT_CTL7  =================================================== */
#define CPUSS_CM0_INT_CTL7_MUX0_SEL_Pos   (0UL)                 /*!< CPUSS CM0_INT_CTL7: MUX0_SEL (Bit 0)                      */
#define CPUSS_CM0_INT_CTL7_MUX0_SEL_Msk   (0xffUL)              /*!< CPUSS CM0_INT_CTL7: MUX0_SEL (Bitfield-Mask: 0xff)        */
#define CPUSS_CM0_INT_CTL7_MUX1_SEL_Pos   (8UL)                 /*!< CPUSS CM0_INT_CTL7: MUX1_SEL (Bit 8)                      */
#define CPUSS_CM0_INT_CTL7_MUX1_SEL_Msk   (0xff00UL)            /*!< CPUSS CM0_INT_CTL7: MUX1_SEL (Bitfield-Mask: 0xff)        */
#define CPUSS_CM0_INT_CTL7_MUX2_SEL_Pos   (16UL)                /*!< CPUSS CM0_INT_CTL7: MUX2_SEL (Bit 16)                     */
#define CPUSS_CM0_INT_CTL7_MUX2_SEL_Msk   (0xff0000UL)          /*!< CPUSS CM0_INT_CTL7: MUX2_SEL (Bitfield-Mask: 0xff)        */
#define CPUSS_CM0_INT_CTL7_MUX3_SEL_Pos   (24UL)                /*!< CPUSS CM0_INT_CTL7: MUX3_SEL (Bit 24)                     */
#define CPUSS_CM0_INT_CTL7_MUX3_SEL_Msk   (0xff000000UL)        /*!< CPUSS CM0_INT_CTL7: MUX3_SEL (Bitfield-Mask: 0xff)        */
/* ===================================================  CPUSS.CM4_PWR_CTL  =================================================== */
#define CPUSS_CM4_PWR_CTL_ISOLATE_Pos     (0UL)                 /*!< CPUSS CM4_PWR_CTL: ISOLATE (Bit 0)                        */
#define CPUSS_CM4_PWR_CTL_ISOLATE_Msk     (0x1UL)               /*!< CPUSS CM4_PWR_CTL: ISOLATE (Bitfield-Mask: 0x01)          */
#define CPUSS_CM4_PWR_CTL_RETAIN_Pos      (1UL)                 /*!< CPUSS CM4_PWR_CTL: RETAIN (Bit 1)                         */
#define CPUSS_CM4_PWR_CTL_RETAIN_Msk      (0x2UL)               /*!< CPUSS CM4_PWR_CTL: RETAIN (Bitfield-Mask: 0x01)           */
#define CPUSS_CM4_PWR_CTL_POWER_Pos       (2UL)                 /*!< CPUSS CM4_PWR_CTL: POWER (Bit 2)                          */
#define CPUSS_CM4_PWR_CTL_POWER_Msk       (0x4UL)               /*!< CPUSS CM4_PWR_CTL: POWER (Bitfield-Mask: 0x01)            */
#define CPUSS_CM4_PWR_CTL_RESET_Pos       (3UL)                 /*!< CPUSS CM4_PWR_CTL: RESET (Bit 3)                          */
#define CPUSS_CM4_PWR_CTL_RESET_Msk       (0x8UL)               /*!< CPUSS CM4_PWR_CTL: RESET (Bitfield-Mask: 0x01)            */
#define CPUSS_CM4_PWR_CTL_CLOCK_Pos       (4UL)                 /*!< CPUSS CM4_PWR_CTL: CLOCK (Bit 4)                          */
#define CPUSS_CM4_PWR_CTL_CLOCK_Msk       (0x10UL)              /*!< CPUSS CM4_PWR_CTL: CLOCK (Bitfield-Mask: 0x01)            */
/* ===================================================  CPUSS.CM4_STATUS  ==================================================== */
#define CPUSS_CM4_STATUS_SLEEPING_Pos     (0UL)                 /*!< CPUSS CM4_STATUS: SLEEPING (Bit 0)                        */
#define CPUSS_CM4_STATUS_SLEEPING_Msk     (0x1UL)               /*!< CPUSS CM4_STATUS: SLEEPING (Bitfield-Mask: 0x01)          */
#define CPUSS_CM4_STATUS_SLEEPDEEP_Pos    (1UL)                 /*!< CPUSS CM4_STATUS: SLEEPDEEP (Bit 1)                       */
#define CPUSS_CM4_STATUS_SLEEPDEEP_Msk    (0x2UL)               /*!< CPUSS CM4_STATUS: SLEEPDEEP (Bitfield-Mask: 0x01)         */
/* ==================================================  CPUSS.CM4_CLOCK_CTL  ================================================== */
#define CPUSS_CM4_CLOCK_CTL_FAST_INT_DIV_Pos (8UL)              /*!< CPUSS CM4_CLOCK_CTL: FAST_INT_DIV (Bit 8)                 */
#define CPUSS_CM4_CLOCK_CTL_FAST_INT_DIV_Msk (0xff00UL)         /*!< CPUSS CM4_CLOCK_CTL: FAST_INT_DIV (Bitfield-Mask: 0xff)   */
/* ===================================================  CPUSS.CM4_NMI_CTL  =================================================== */
#define CPUSS_CM4_NMI_CTL_MUX0_SEL_Pos    (0UL)                 /*!< CPUSS CM4_NMI_CTL: MUX0_SEL (Bit 0)                       */
#define CPUSS_CM4_NMI_CTL_MUX0_SEL_Msk    (0xffUL)              /*!< CPUSS CM4_NMI_CTL: MUX0_SEL (Bitfield-Mask: 0xff)         */
/* ===================================================  CPUSS.UDB_PWR_CTL  =================================================== */
#define CPUSS_UDB_PWR_CTL_ISOLATE_Pos     (0UL)                 /*!< CPUSS UDB_PWR_CTL: ISOLATE (Bit 0)                        */
#define CPUSS_UDB_PWR_CTL_ISOLATE_Msk     (0x1UL)               /*!< CPUSS UDB_PWR_CTL: ISOLATE (Bitfield-Mask: 0x01)          */
#define CPUSS_UDB_PWR_CTL_RETAIN_Pos      (1UL)                 /*!< CPUSS UDB_PWR_CTL: RETAIN (Bit 1)                         */
#define CPUSS_UDB_PWR_CTL_RETAIN_Msk      (0x2UL)               /*!< CPUSS UDB_PWR_CTL: RETAIN (Bitfield-Mask: 0x01)           */
#define CPUSS_UDB_PWR_CTL_POWER_Pos       (2UL)                 /*!< CPUSS UDB_PWR_CTL: POWER (Bit 2)                          */
#define CPUSS_UDB_PWR_CTL_POWER_Msk       (0x4UL)               /*!< CPUSS UDB_PWR_CTL: POWER (Bitfield-Mask: 0x01)            */
#define CPUSS_UDB_PWR_CTL_RESET_Pos       (3UL)                 /*!< CPUSS UDB_PWR_CTL: RESET (Bit 3)                          */
#define CPUSS_UDB_PWR_CTL_RESET_Msk       (0x8UL)               /*!< CPUSS UDB_PWR_CTL: RESET (Bitfield-Mask: 0x01)            */
#define CPUSS_UDB_PWR_CTL_CLOCK_Pos       (4UL)                 /*!< CPUSS UDB_PWR_CTL: CLOCK (Bit 4)                          */
#define CPUSS_UDB_PWR_CTL_CLOCK_Msk       (0x10UL)              /*!< CPUSS UDB_PWR_CTL: CLOCK (Bitfield-Mask: 0x01)            */
/* ====================================================  CPUSS.RAM0_CTL0  ==================================================== */
#define CPUSS_RAM0_CTL0_SLOW_WS_Pos       (0UL)                 /*!< CPUSS RAM0_CTL0: SLOW_WS (Bit 0)                          */
#define CPUSS_RAM0_CTL0_SLOW_WS_Msk       (0x3UL)               /*!< CPUSS RAM0_CTL0: SLOW_WS (Bitfield-Mask: 0x03)            */
#define CPUSS_RAM0_CTL0_FAST_WS_Pos       (8UL)                 /*!< CPUSS RAM0_CTL0: FAST_WS (Bit 8)                          */
#define CPUSS_RAM0_CTL0_FAST_WS_Msk       (0x300UL)             /*!< CPUSS RAM0_CTL0: FAST_WS (Bitfield-Mask: 0x03)            */
/* ===============================================  CPUSS.RAM0_PWR_MACRO_CTL  ================================================ */
#define CPUSS_RAM0_PWR_MACRO_CTL_ISOLATE_Pos (0UL)              /*!< CPUSS RAM0_PWR_MACRO_CTL: ISOLATE (Bit 0)                 */
#define CPUSS_RAM0_PWR_MACRO_CTL_ISOLATE_Msk (0x1UL)            /*!< CPUSS RAM0_PWR_MACRO_CTL: ISOLATE (Bitfield-Mask: 0x01)   */
#define CPUSS_RAM0_PWR_MACRO_CTL_RETAIN_Pos (1UL)               /*!< CPUSS RAM0_PWR_MACRO_CTL: RETAIN (Bit 1)                  */
#define CPUSS_RAM0_PWR_MACRO_CTL_RETAIN_Msk (0x2UL)             /*!< CPUSS RAM0_PWR_MACRO_CTL: RETAIN (Bitfield-Mask: 0x01)    */
#define CPUSS_RAM0_PWR_MACRO_CTL_POWER_Pos (2UL)                /*!< CPUSS RAM0_PWR_MACRO_CTL: POWER (Bit 2)                   */
#define CPUSS_RAM0_PWR_MACRO_CTL_POWER_Msk (0x4UL)              /*!< CPUSS RAM0_PWR_MACRO_CTL: POWER (Bitfield-Mask: 0x01)     */
/* ====================================================  CPUSS.RAM1_CTL0  ==================================================== */
#define CPUSS_RAM1_CTL0_SLOW_WS_Pos       (0UL)                 /*!< CPUSS RAM1_CTL0: SLOW_WS (Bit 0)                          */
#define CPUSS_RAM1_CTL0_SLOW_WS_Msk       (0x3UL)               /*!< CPUSS RAM1_CTL0: SLOW_WS (Bitfield-Mask: 0x03)            */
#define CPUSS_RAM1_CTL0_FAST_WS_Pos       (8UL)                 /*!< CPUSS RAM1_CTL0: FAST_WS (Bit 8)                          */
#define CPUSS_RAM1_CTL0_FAST_WS_Msk       (0x300UL)             /*!< CPUSS RAM1_CTL0: FAST_WS (Bitfield-Mask: 0x03)            */
/* ==================================================  CPUSS.RAM1_PWR_CTL  =================================================== */
#define CPUSS_RAM1_PWR_CTL_ISOLATE_Pos    (0UL)                 /*!< CPUSS RAM1_PWR_CTL: ISOLATE (Bit 0)                       */
#define CPUSS_RAM1_PWR_CTL_ISOLATE_Msk    (0x1UL)               /*!< CPUSS RAM1_PWR_CTL: ISOLATE (Bitfield-Mask: 0x01)         */
#define CPUSS_RAM1_PWR_CTL_RETAIN_Pos     (1UL)                 /*!< CPUSS RAM1_PWR_CTL: RETAIN (Bit 1)                        */
#define CPUSS_RAM1_PWR_CTL_RETAIN_Msk     (0x2UL)               /*!< CPUSS RAM1_PWR_CTL: RETAIN (Bitfield-Mask: 0x01)          */
#define CPUSS_RAM1_PWR_CTL_POWER_Pos      (2UL)                 /*!< CPUSS RAM1_PWR_CTL: POWER (Bit 2)                         */
#define CPUSS_RAM1_PWR_CTL_POWER_Msk      (0x4UL)               /*!< CPUSS RAM1_PWR_CTL: POWER (Bitfield-Mask: 0x01)           */
/* ====================================================  CPUSS.RAM2_CTL0  ==================================================== */
#define CPUSS_RAM2_CTL0_SLOW_WS_Pos       (0UL)                 /*!< CPUSS RAM2_CTL0: SLOW_WS (Bit 0)                          */
#define CPUSS_RAM2_CTL0_SLOW_WS_Msk       (0x3UL)               /*!< CPUSS RAM2_CTL0: SLOW_WS (Bitfield-Mask: 0x03)            */
#define CPUSS_RAM2_CTL0_FAST_WS_Pos       (8UL)                 /*!< CPUSS RAM2_CTL0: FAST_WS (Bit 8)                          */
#define CPUSS_RAM2_CTL0_FAST_WS_Msk       (0x300UL)             /*!< CPUSS RAM2_CTL0: FAST_WS (Bitfield-Mask: 0x03)            */
/* ==================================================  CPUSS.RAM2_PWR_CTL  =================================================== */
#define CPUSS_RAM2_PWR_CTL_ISOLATE_Pos    (0UL)                 /*!< CPUSS RAM2_PWR_CTL: ISOLATE (Bit 0)                       */
#define CPUSS_RAM2_PWR_CTL_ISOLATE_Msk    (0x1UL)               /*!< CPUSS RAM2_PWR_CTL: ISOLATE (Bitfield-Mask: 0x01)         */
#define CPUSS_RAM2_PWR_CTL_RETAIN_Pos     (1UL)                 /*!< CPUSS RAM2_PWR_CTL: RETAIN (Bit 1)                        */
#define CPUSS_RAM2_PWR_CTL_RETAIN_Msk     (0x2UL)               /*!< CPUSS RAM2_PWR_CTL: RETAIN (Bitfield-Mask: 0x01)          */
#define CPUSS_RAM2_PWR_CTL_POWER_Pos      (2UL)                 /*!< CPUSS RAM2_PWR_CTL: POWER (Bit 2)                         */
#define CPUSS_RAM2_PWR_CTL_POWER_Msk      (0x4UL)               /*!< CPUSS RAM2_PWR_CTL: POWER (Bitfield-Mask: 0x01)           */
/* =====================================================  CPUSS.ROM_CTL  ===================================================== */
#define CPUSS_ROM_CTL_SLOW_WS_Pos         (0UL)                 /*!< CPUSS ROM_CTL: SLOW_WS (Bit 0)                            */
#define CPUSS_ROM_CTL_SLOW_WS_Msk         (0x3UL)               /*!< CPUSS ROM_CTL: SLOW_WS (Bitfield-Mask: 0x03)              */
#define CPUSS_ROM_CTL_FAST_WS_Pos         (8UL)                 /*!< CPUSS ROM_CTL: FAST_WS (Bit 8)                            */
#define CPUSS_ROM_CTL_FAST_WS_Msk         (0x300UL)             /*!< CPUSS ROM_CTL: FAST_WS (Bitfield-Mask: 0x03)              */
/* ===================================================  CPUSS.ETB_PWR_CTL  =================================================== */
#define CPUSS_ETB_PWR_CTL_ISOLATE_Pos     (0UL)                 /*!< CPUSS ETB_PWR_CTL: ISOLATE (Bit 0)                        */
#define CPUSS_ETB_PWR_CTL_ISOLATE_Msk     (0x1UL)               /*!< CPUSS ETB_PWR_CTL: ISOLATE (Bitfield-Mask: 0x01)          */
#define CPUSS_ETB_PWR_CTL_RETAIN_Pos      (1UL)                 /*!< CPUSS ETB_PWR_CTL: RETAIN (Bit 1)                         */
#define CPUSS_ETB_PWR_CTL_RETAIN_Msk      (0x2UL)               /*!< CPUSS ETB_PWR_CTL: RETAIN (Bitfield-Mask: 0x01)           */
#define CPUSS_ETB_PWR_CTL_POWER_Pos       (2UL)                 /*!< CPUSS ETB_PWR_CTL: POWER (Bit 2)                          */
#define CPUSS_ETB_PWR_CTL_POWER_Msk       (0x4UL)               /*!< CPUSS ETB_PWR_CTL: POWER (Bitfield-Mask: 0x01)            */
/* ====================================================  CPUSS.DP_STATUS  ==================================================== */
#define CPUSS_DP_STATUS_SWJ_CONNECTED_Pos (0UL)                 /*!< CPUSS DP_STATUS: SWJ_CONNECTED (Bit 0)                    */
#define CPUSS_DP_STATUS_SWJ_CONNECTED_Msk (0x1UL)               /*!< CPUSS DP_STATUS: SWJ_CONNECTED (Bitfield-Mask: 0x01)      */
#define CPUSS_DP_STATUS_SWJ_DEBUG_EN_Pos  (1UL)                 /*!< CPUSS DP_STATUS: SWJ_DEBUG_EN (Bit 1)                     */
#define CPUSS_DP_STATUS_SWJ_DEBUG_EN_Msk  (0x2UL)               /*!< CPUSS DP_STATUS: SWJ_DEBUG_EN (Bitfield-Mask: 0x01)       */
#define CPUSS_DP_STATUS_SWJ_JTAG_SEL_Pos  (2UL)                 /*!< CPUSS DP_STATUS: SWJ_JTAG_SEL (Bit 2)                     */
#define CPUSS_DP_STATUS_SWJ_JTAG_SEL_Msk  (0x4UL)               /*!< CPUSS DP_STATUS: SWJ_JTAG_SEL (Bitfield-Mask: 0x01)       */
/* ====================================================  CPUSS.BUFF_CTL  ===================================================== */
#define CPUSS_BUFF_CTL_WRITE_BUFF_Pos     (0UL)                 /*!< CPUSS BUFF_CTL: WRITE_BUFF (Bit 0)                        */
#define CPUSS_BUFF_CTL_WRITE_BUFF_Msk     (0x1UL)               /*!< CPUSS BUFF_CTL: WRITE_BUFF (Bitfield-Mask: 0x01)          */
/* ===================================================  CPUSS.SYSTICK_CTL  =================================================== */
#define CPUSS_SYSTICK_CTL_TENMS_Pos       (0UL)                 /*!< CPUSS SYSTICK_CTL: TENMS (Bit 0)                          */
#define CPUSS_SYSTICK_CTL_TENMS_Msk       (0xffffffUL)          /*!< CPUSS SYSTICK_CTL: TENMS (Bitfield-Mask: 0xffffff)        */
#define CPUSS_SYSTICK_CTL_CLOCK_SOURCE_Pos (24UL)               /*!< CPUSS SYSTICK_CTL: CLOCK_SOURCE (Bit 24)                  */
#define CPUSS_SYSTICK_CTL_CLOCK_SOURCE_Msk (0x3000000UL)        /*!< CPUSS SYSTICK_CTL: CLOCK_SOURCE (Bitfield-Mask: 0x03)     */
#define CPUSS_SYSTICK_CTL_SKEW_Pos        (30UL)                /*!< CPUSS SYSTICK_CTL: SKEW (Bit 30)                          */
#define CPUSS_SYSTICK_CTL_SKEW_Msk        (0x40000000UL)        /*!< CPUSS SYSTICK_CTL: SKEW (Bitfield-Mask: 0x01)             */
#define CPUSS_SYSTICK_CTL_NOREF_Pos       (31UL)                /*!< CPUSS SYSTICK_CTL: NOREF (Bit 31)                         */
#define CPUSS_SYSTICK_CTL_NOREF_Msk       (0x80000000UL)        /*!< CPUSS SYSTICK_CTL: NOREF (Bitfield-Mask: 0x01)            */
/* ===================================================  CPUSS.MBIST_STAT  ==================================================== */
#define CPUSS_MBIST_STAT_SFP_READY_Pos    (0UL)                 /*!< CPUSS MBIST_STAT: SFP_READY (Bit 0)                       */
#define CPUSS_MBIST_STAT_SFP_READY_Msk    (0x1UL)               /*!< CPUSS MBIST_STAT: SFP_READY (Bitfield-Mask: 0x01)         */
#define CPUSS_MBIST_STAT_SFP_FAIL_Pos     (1UL)                 /*!< CPUSS MBIST_STAT: SFP_FAIL (Bit 1)                        */
#define CPUSS_MBIST_STAT_SFP_FAIL_Msk     (0x2UL)               /*!< CPUSS MBIST_STAT: SFP_FAIL (Bitfield-Mask: 0x01)          */
/* ====================================================  CPUSS.WOUNDING  ===================================================== */
#define CPUSS_WOUNDING_RAM0_WOUND_Pos     (0UL)                 /*!< CPUSS WOUNDING: RAM0_WOUND (Bit 0)                        */
#define CPUSS_WOUNDING_RAM0_WOUND_Msk     (0x7UL)               /*!< CPUSS WOUNDING: RAM0_WOUND (Bitfield-Mask: 0x07)          */
#define CPUSS_WOUNDING_RAM1_WOUND_Pos     (4UL)                 /*!< CPUSS WOUNDING: RAM1_WOUND (Bit 4)                        */
#define CPUSS_WOUNDING_RAM1_WOUND_Msk     (0x70UL)              /*!< CPUSS WOUNDING: RAM1_WOUND (Bitfield-Mask: 0x07)          */
#define CPUSS_WOUNDING_RAM2_WOUND_Pos     (8UL)                 /*!< CPUSS WOUNDING: RAM2_WOUND (Bit 8)                        */
#define CPUSS_WOUNDING_RAM2_WOUND_Msk     (0x700UL)             /*!< CPUSS WOUNDING: RAM2_WOUND (Bitfield-Mask: 0x07)          */
#define CPUSS_WOUNDING_FLASH_WOUND_Pos    (12UL)                /*!< CPUSS WOUNDING: FLASH_WOUND (Bit 12)                      */
#define CPUSS_WOUNDING_FLASH_WOUND_Msk    (0x7000UL)            /*!< CPUSS WOUNDING: FLASH_WOUND (Bitfield-Mask: 0x07)         */
/* ===================================================  CPUSS.PROTECTION  ==================================================== */
#define CPUSS_PROTECTION_STATE_Pos        (0UL)                 /*!< CPUSS PROTECTION: STATE (Bit 0)                           */
#define CPUSS_PROTECTION_STATE_Msk        (0x7UL)               /*!< CPUSS PROTECTION: STATE (Bitfield-Mask: 0x07)             */
/* =================================================  CPUSS.CM0_PC0_HANDLER  ================================================= */
#define CPUSS_CM0_PC0_HANDLER_ADDR_Pos    (0UL)                 /*!< CPUSS CM0_PC0_HANDLER: ADDR (Bit 0)                       */
#define CPUSS_CM0_PC0_HANDLER_ADDR_Msk    (0xffffffffUL)        /*!< CPUSS CM0_PC0_HANDLER: ADDR (Bitfield-Mask: 0xffffffff)   */
/* ==============================================  CPUSS.CM0_VECTOR_TABLE_BASE  ============================================== */
#define CPUSS_CM0_VECTOR_TABLE_BASE_ADDR24_Pos (8UL)            /*!< CPUSS CM0_VECTOR_TABLE_BASE: ADDR24 (Bit 8)               */
#define CPUSS_CM0_VECTOR_TABLE_BASE_ADDR24_Msk (0xffffff00UL)   /*!< CPUSS CM0_VECTOR_TABLE_BASE: ADDR24 (Bitfield-Mask: 0xffffff)*/
/* ==============================================  CPUSS.CM4_VECTOR_TABLE_BASE  ============================================== */
#define CPUSS_CM4_VECTOR_TABLE_BASE_ADDR22_Pos (10UL)           /*!< CPUSS CM4_VECTOR_TABLE_BASE: ADDR22 (Bit 10)              */
#define CPUSS_CM4_VECTOR_TABLE_BASE_ADDR22_Msk (0xfffffc00UL)   /*!< CPUSS CM4_VECTOR_TABLE_BASE: ADDR22 (Bitfield-Mask: 0x3fffff)*/
/* ====================================================  CPUSS.IDENTITY  ===================================================== */
#define CPUSS_IDENTITY_P_Pos              (0UL)                 /*!< CPUSS IDENTITY: P (Bit 0)                                 */
#define CPUSS_IDENTITY_P_Msk              (0x1UL)               /*!< CPUSS IDENTITY: P (Bitfield-Mask: 0x01)                   */
#define CPUSS_IDENTITY_NS_Pos             (1UL)                 /*!< CPUSS IDENTITY: NS (Bit 1)                                */
#define CPUSS_IDENTITY_NS_Msk             (0x2UL)               /*!< CPUSS IDENTITY: NS (Bitfield-Mask: 0x01)                  */
#define CPUSS_IDENTITY_PC_Pos             (4UL)                 /*!< CPUSS IDENTITY: PC (Bit 4)                                */
#define CPUSS_IDENTITY_PC_Msk             (0xf0UL)              /*!< CPUSS IDENTITY: PC (Bitfield-Mask: 0x0f)                  */
#define CPUSS_IDENTITY_MS_Pos             (8UL)                 /*!< CPUSS IDENTITY: MS (Bit 8)                                */
#define CPUSS_IDENTITY_MS_Msk             (0xf00UL)             /*!< CPUSS IDENTITY: MS (Bitfield-Mask: 0x0f)                  */
/* ==================================================  CPUSS.TRIM_ROM_CTL  =================================================== */
#define CPUSS_TRIM_ROM_CTL_RM_Pos         (0UL)                 /*!< CPUSS TRIM_ROM_CTL: RM (Bit 0)                            */
#define CPUSS_TRIM_ROM_CTL_RM_Msk         (0xfUL)               /*!< CPUSS TRIM_ROM_CTL: RM (Bitfield-Mask: 0x0f)              */
#define CPUSS_TRIM_ROM_CTL_RME_Pos        (4UL)                 /*!< CPUSS TRIM_ROM_CTL: RME (Bit 4)                           */
#define CPUSS_TRIM_ROM_CTL_RME_Msk        (0x10UL)              /*!< CPUSS TRIM_ROM_CTL: RME (Bitfield-Mask: 0x01)             */
/* ==================================================  CPUSS.TRIM_RAM_CTL  =================================================== */
#define CPUSS_TRIM_RAM_CTL_RM_Pos         (0UL)                 /*!< CPUSS TRIM_RAM_CTL: RM (Bit 0)                            */
#define CPUSS_TRIM_RAM_CTL_RM_Msk         (0xfUL)               /*!< CPUSS TRIM_RAM_CTL: RM (Bitfield-Mask: 0x0f)              */
#define CPUSS_TRIM_RAM_CTL_RME_Pos        (4UL)                 /*!< CPUSS TRIM_RAM_CTL: RME (Bit 4)                           */
#define CPUSS_TRIM_RAM_CTL_RME_Msk        (0x10UL)              /*!< CPUSS TRIM_RAM_CTL: RME (Bitfield-Mask: 0x01)             */
#define CPUSS_TRIM_RAM_CTL_WPULSE_Pos     (5UL)                 /*!< CPUSS TRIM_RAM_CTL: WPULSE (Bit 5)                        */
#define CPUSS_TRIM_RAM_CTL_WPULSE_Msk     (0xe0UL)              /*!< CPUSS TRIM_RAM_CTL: WPULSE (Bitfield-Mask: 0x07)          */
#define CPUSS_TRIM_RAM_CTL_RA_Pos         (8UL)                 /*!< CPUSS TRIM_RAM_CTL: RA (Bit 8)                            */
#define CPUSS_TRIM_RAM_CTL_RA_Msk         (0x300UL)             /*!< CPUSS TRIM_RAM_CTL: RA (Bitfield-Mask: 0x03)              */
#define CPUSS_TRIM_RAM_CTL_WA_Pos         (12UL)                /*!< CPUSS TRIM_RAM_CTL: WA (Bit 12)                           */
#define CPUSS_TRIM_RAM_CTL_WA_Msk         (0x7000UL)            /*!< CPUSS TRIM_RAM_CTL: WA (Bitfield-Mask: 0x07)              */


#endif /* _CYIP_CPUSS_H_ */


/* [] END OF FILE */
