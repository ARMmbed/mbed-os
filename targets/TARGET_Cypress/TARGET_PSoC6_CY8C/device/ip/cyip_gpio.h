/***************************************************************************//**
* \file cyip_gpio.h
*
* \brief
* GPIO IP definitions
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

#ifndef _CYIP_GPIO_H_
#define _CYIP_GPIO_H_

#include "cyip_headers.h"

/* =========================================================================================================================== */
/* ================                                           GPIO                                            ================ */
/* =========================================================================================================================== */

#define GPIO_PRT_SECTION_SIZE       0x00000080UL
#define GPIO_SECTION_SIZE           0x00010000UL

/**
  * \brief GPIO port registers (GPIO_PRT)
  */
typedef struct {
  __IOM uint32_t OUT;                           /*!< 0x00000000 Port output data register                                      */
  __IOM uint32_t OUT_CLR;                       /*!< 0x00000004 Port output data set register                                  */
  __IOM uint32_t OUT_SET;                       /*!< 0x00000008 Port output data clear register                                */
  __IOM uint32_t OUT_INV;                       /*!< 0x0000000C Port output data invert register                               */
   __IM uint32_t IN;                            /*!< 0x00000010 Port input state register                                      */
  __IOM uint32_t INTR;                          /*!< 0x00000014 Port interrupt status register                                 */
  __IOM uint32_t INTR_MASK;                     /*!< 0x00000018 Port interrupt mask register                                   */
   __IM uint32_t INTR_MASKED;                   /*!< 0x0000001C Port interrupt masked status register                          */
  __IOM uint32_t INTR_SET;                      /*!< 0x00000020 Port interrupt set register                                    */
  __IOM uint32_t INTR_CFG;                      /*!< 0x00000024 Port interrupt configuration register                          */
  __IOM uint32_t CFG;                           /*!< 0x00000028 Port configuration register                                    */
  __IOM uint32_t CFG_IN;                        /*!< 0x0000002C Port input buffer configuration register                       */
  __IOM uint32_t CFG_OUT;                       /*!< 0x00000030 Port output buffer configuration register                      */
  __IOM uint32_t CFG_SIO;                       /*!< 0x00000034 Port SIO configuration register                                */
   __IM uint32_t RESERVED;
  __IOM uint32_t CFG_IN_GPIO5V;                 /*!< 0x0000003C Port GPIO5V input buffer configuration register                */
   __IM uint32_t RESERVED1[16];
} GPIO_PRT_Type;                                /*!< Size = 64 (0x40)                                                          */

/**
  * \brief GPIO port control/configuration (GPIO)
  */
typedef struct {                                /*!< GPIO Structure                                                            */
        GPIO_PRT_Type PRT[128];                 /*!< 0x00000000 GPIO port registers                                            */
   __IM uint32_t INTR_CAUSE0;                   /*!< 0x00004000 Interrupt port cause register 0                                */
   __IM uint32_t INTR_CAUSE1;                   /*!< 0x00004004 Interrupt port cause register 1                                */
   __IM uint32_t INTR_CAUSE2;                   /*!< 0x00004008 Interrupt port cause register 2                                */
   __IM uint32_t INTR_CAUSE3;                   /*!< 0x0000400C Interrupt port cause register 3                                */
   __IM uint32_t VDD_ACTIVE;                    /*!< 0x00004010 Extern power supply detection register                         */
  __IOM uint32_t VDD_INTR;                      /*!< 0x00004014 Supply detection interrupt register                            */
  __IOM uint32_t VDD_INTR_MASK;                 /*!< 0x00004018 Supply detection interrupt mask register                       */
   __IM uint32_t VDD_INTR_MASKED;               /*!< 0x0000401C Supply detection interrupt masked register                     */
  __IOM uint32_t VDD_INTR_SET;                  /*!< 0x00004020 Supply detection interrupt set register                        */
} GPIO_Type;                                    /*!< Size = 16420 (0x4024)                                                     */


/* =====================================================  GPIO_PRT.OUT  ====================================================== */
#define GPIO_PRT_OUT_OUT0_Pos             (0UL)                 /*!< GPIO_PRT OUT: OUT0 (Bit 0)                                */
#define GPIO_PRT_OUT_OUT0_Msk             (0x1UL)               /*!< GPIO_PRT OUT: OUT0 (Bitfield-Mask: 0x01)                  */
#define GPIO_PRT_OUT_OUT1_Pos             (1UL)                 /*!< GPIO_PRT OUT: OUT1 (Bit 1)                                */
#define GPIO_PRT_OUT_OUT1_Msk             (0x2UL)               /*!< GPIO_PRT OUT: OUT1 (Bitfield-Mask: 0x01)                  */
#define GPIO_PRT_OUT_OUT2_Pos             (2UL)                 /*!< GPIO_PRT OUT: OUT2 (Bit 2)                                */
#define GPIO_PRT_OUT_OUT2_Msk             (0x4UL)               /*!< GPIO_PRT OUT: OUT2 (Bitfield-Mask: 0x01)                  */
#define GPIO_PRT_OUT_OUT3_Pos             (3UL)                 /*!< GPIO_PRT OUT: OUT3 (Bit 3)                                */
#define GPIO_PRT_OUT_OUT3_Msk             (0x8UL)               /*!< GPIO_PRT OUT: OUT3 (Bitfield-Mask: 0x01)                  */
#define GPIO_PRT_OUT_OUT4_Pos             (4UL)                 /*!< GPIO_PRT OUT: OUT4 (Bit 4)                                */
#define GPIO_PRT_OUT_OUT4_Msk             (0x10UL)              /*!< GPIO_PRT OUT: OUT4 (Bitfield-Mask: 0x01)                  */
#define GPIO_PRT_OUT_OUT5_Pos             (5UL)                 /*!< GPIO_PRT OUT: OUT5 (Bit 5)                                */
#define GPIO_PRT_OUT_OUT5_Msk             (0x20UL)              /*!< GPIO_PRT OUT: OUT5 (Bitfield-Mask: 0x01)                  */
#define GPIO_PRT_OUT_OUT6_Pos             (6UL)                 /*!< GPIO_PRT OUT: OUT6 (Bit 6)                                */
#define GPIO_PRT_OUT_OUT6_Msk             (0x40UL)              /*!< GPIO_PRT OUT: OUT6 (Bitfield-Mask: 0x01)                  */
#define GPIO_PRT_OUT_OUT7_Pos             (7UL)                 /*!< GPIO_PRT OUT: OUT7 (Bit 7)                                */
#define GPIO_PRT_OUT_OUT7_Msk             (0x80UL)              /*!< GPIO_PRT OUT: OUT7 (Bitfield-Mask: 0x01)                  */
/* ===================================================  GPIO_PRT.OUT_CLR  ==================================================== */
#define GPIO_PRT_OUT_CLR_OUT0_Pos         (0UL)                 /*!< GPIO_PRT OUT_CLR: OUT0 (Bit 0)                            */
#define GPIO_PRT_OUT_CLR_OUT0_Msk         (0x1UL)               /*!< GPIO_PRT OUT_CLR: OUT0 (Bitfield-Mask: 0x01)              */
#define GPIO_PRT_OUT_CLR_OUT1_Pos         (1UL)                 /*!< GPIO_PRT OUT_CLR: OUT1 (Bit 1)                            */
#define GPIO_PRT_OUT_CLR_OUT1_Msk         (0x2UL)               /*!< GPIO_PRT OUT_CLR: OUT1 (Bitfield-Mask: 0x01)              */
#define GPIO_PRT_OUT_CLR_OUT2_Pos         (2UL)                 /*!< GPIO_PRT OUT_CLR: OUT2 (Bit 2)                            */
#define GPIO_PRT_OUT_CLR_OUT2_Msk         (0x4UL)               /*!< GPIO_PRT OUT_CLR: OUT2 (Bitfield-Mask: 0x01)              */
#define GPIO_PRT_OUT_CLR_OUT3_Pos         (3UL)                 /*!< GPIO_PRT OUT_CLR: OUT3 (Bit 3)                            */
#define GPIO_PRT_OUT_CLR_OUT3_Msk         (0x8UL)               /*!< GPIO_PRT OUT_CLR: OUT3 (Bitfield-Mask: 0x01)              */
#define GPIO_PRT_OUT_CLR_OUT4_Pos         (4UL)                 /*!< GPIO_PRT OUT_CLR: OUT4 (Bit 4)                            */
#define GPIO_PRT_OUT_CLR_OUT4_Msk         (0x10UL)              /*!< GPIO_PRT OUT_CLR: OUT4 (Bitfield-Mask: 0x01)              */
#define GPIO_PRT_OUT_CLR_OUT5_Pos         (5UL)                 /*!< GPIO_PRT OUT_CLR: OUT5 (Bit 5)                            */
#define GPIO_PRT_OUT_CLR_OUT5_Msk         (0x20UL)              /*!< GPIO_PRT OUT_CLR: OUT5 (Bitfield-Mask: 0x01)              */
#define GPIO_PRT_OUT_CLR_OUT6_Pos         (6UL)                 /*!< GPIO_PRT OUT_CLR: OUT6 (Bit 6)                            */
#define GPIO_PRT_OUT_CLR_OUT6_Msk         (0x40UL)              /*!< GPIO_PRT OUT_CLR: OUT6 (Bitfield-Mask: 0x01)              */
#define GPIO_PRT_OUT_CLR_OUT7_Pos         (7UL)                 /*!< GPIO_PRT OUT_CLR: OUT7 (Bit 7)                            */
#define GPIO_PRT_OUT_CLR_OUT7_Msk         (0x80UL)              /*!< GPIO_PRT OUT_CLR: OUT7 (Bitfield-Mask: 0x01)              */
/* ===================================================  GPIO_PRT.OUT_SET  ==================================================== */
#define GPIO_PRT_OUT_SET_OUT0_Pos         (0UL)                 /*!< GPIO_PRT OUT_SET: OUT0 (Bit 0)                            */
#define GPIO_PRT_OUT_SET_OUT0_Msk         (0x1UL)               /*!< GPIO_PRT OUT_SET: OUT0 (Bitfield-Mask: 0x01)              */
#define GPIO_PRT_OUT_SET_OUT1_Pos         (1UL)                 /*!< GPIO_PRT OUT_SET: OUT1 (Bit 1)                            */
#define GPIO_PRT_OUT_SET_OUT1_Msk         (0x2UL)               /*!< GPIO_PRT OUT_SET: OUT1 (Bitfield-Mask: 0x01)              */
#define GPIO_PRT_OUT_SET_OUT2_Pos         (2UL)                 /*!< GPIO_PRT OUT_SET: OUT2 (Bit 2)                            */
#define GPIO_PRT_OUT_SET_OUT2_Msk         (0x4UL)               /*!< GPIO_PRT OUT_SET: OUT2 (Bitfield-Mask: 0x01)              */
#define GPIO_PRT_OUT_SET_OUT3_Pos         (3UL)                 /*!< GPIO_PRT OUT_SET: OUT3 (Bit 3)                            */
#define GPIO_PRT_OUT_SET_OUT3_Msk         (0x8UL)               /*!< GPIO_PRT OUT_SET: OUT3 (Bitfield-Mask: 0x01)              */
#define GPIO_PRT_OUT_SET_OUT4_Pos         (4UL)                 /*!< GPIO_PRT OUT_SET: OUT4 (Bit 4)                            */
#define GPIO_PRT_OUT_SET_OUT4_Msk         (0x10UL)              /*!< GPIO_PRT OUT_SET: OUT4 (Bitfield-Mask: 0x01)              */
#define GPIO_PRT_OUT_SET_OUT5_Pos         (5UL)                 /*!< GPIO_PRT OUT_SET: OUT5 (Bit 5)                            */
#define GPIO_PRT_OUT_SET_OUT5_Msk         (0x20UL)              /*!< GPIO_PRT OUT_SET: OUT5 (Bitfield-Mask: 0x01)              */
#define GPIO_PRT_OUT_SET_OUT6_Pos         (6UL)                 /*!< GPIO_PRT OUT_SET: OUT6 (Bit 6)                            */
#define GPIO_PRT_OUT_SET_OUT6_Msk         (0x40UL)              /*!< GPIO_PRT OUT_SET: OUT6 (Bitfield-Mask: 0x01)              */
#define GPIO_PRT_OUT_SET_OUT7_Pos         (7UL)                 /*!< GPIO_PRT OUT_SET: OUT7 (Bit 7)                            */
#define GPIO_PRT_OUT_SET_OUT7_Msk         (0x80UL)              /*!< GPIO_PRT OUT_SET: OUT7 (Bitfield-Mask: 0x01)              */
/* ===================================================  GPIO_PRT.OUT_INV  ==================================================== */
#define GPIO_PRT_OUT_INV_OUT0_Pos         (0UL)                 /*!< GPIO_PRT OUT_INV: OUT0 (Bit 0)                            */
#define GPIO_PRT_OUT_INV_OUT0_Msk         (0x1UL)               /*!< GPIO_PRT OUT_INV: OUT0 (Bitfield-Mask: 0x01)              */
#define GPIO_PRT_OUT_INV_OUT1_Pos         (1UL)                 /*!< GPIO_PRT OUT_INV: OUT1 (Bit 1)                            */
#define GPIO_PRT_OUT_INV_OUT1_Msk         (0x2UL)               /*!< GPIO_PRT OUT_INV: OUT1 (Bitfield-Mask: 0x01)              */
#define GPIO_PRT_OUT_INV_OUT2_Pos         (2UL)                 /*!< GPIO_PRT OUT_INV: OUT2 (Bit 2)                            */
#define GPIO_PRT_OUT_INV_OUT2_Msk         (0x4UL)               /*!< GPIO_PRT OUT_INV: OUT2 (Bitfield-Mask: 0x01)              */
#define GPIO_PRT_OUT_INV_OUT3_Pos         (3UL)                 /*!< GPIO_PRT OUT_INV: OUT3 (Bit 3)                            */
#define GPIO_PRT_OUT_INV_OUT3_Msk         (0x8UL)               /*!< GPIO_PRT OUT_INV: OUT3 (Bitfield-Mask: 0x01)              */
#define GPIO_PRT_OUT_INV_OUT4_Pos         (4UL)                 /*!< GPIO_PRT OUT_INV: OUT4 (Bit 4)                            */
#define GPIO_PRT_OUT_INV_OUT4_Msk         (0x10UL)              /*!< GPIO_PRT OUT_INV: OUT4 (Bitfield-Mask: 0x01)              */
#define GPIO_PRT_OUT_INV_OUT5_Pos         (5UL)                 /*!< GPIO_PRT OUT_INV: OUT5 (Bit 5)                            */
#define GPIO_PRT_OUT_INV_OUT5_Msk         (0x20UL)              /*!< GPIO_PRT OUT_INV: OUT5 (Bitfield-Mask: 0x01)              */
#define GPIO_PRT_OUT_INV_OUT6_Pos         (6UL)                 /*!< GPIO_PRT OUT_INV: OUT6 (Bit 6)                            */
#define GPIO_PRT_OUT_INV_OUT6_Msk         (0x40UL)              /*!< GPIO_PRT OUT_INV: OUT6 (Bitfield-Mask: 0x01)              */
#define GPIO_PRT_OUT_INV_OUT7_Pos         (7UL)                 /*!< GPIO_PRT OUT_INV: OUT7 (Bit 7)                            */
#define GPIO_PRT_OUT_INV_OUT7_Msk         (0x80UL)              /*!< GPIO_PRT OUT_INV: OUT7 (Bitfield-Mask: 0x01)              */
/* ======================================================  GPIO_PRT.IN  ====================================================== */
#define GPIO_PRT_IN_IN0_Pos               (0UL)                 /*!< GPIO_PRT IN: IN0 (Bit 0)                                  */
#define GPIO_PRT_IN_IN0_Msk               (0x1UL)               /*!< GPIO_PRT IN: IN0 (Bitfield-Mask: 0x01)                    */
#define GPIO_PRT_IN_IN1_Pos               (1UL)                 /*!< GPIO_PRT IN: IN1 (Bit 1)                                  */
#define GPIO_PRT_IN_IN1_Msk               (0x2UL)               /*!< GPIO_PRT IN: IN1 (Bitfield-Mask: 0x01)                    */
#define GPIO_PRT_IN_IN2_Pos               (2UL)                 /*!< GPIO_PRT IN: IN2 (Bit 2)                                  */
#define GPIO_PRT_IN_IN2_Msk               (0x4UL)               /*!< GPIO_PRT IN: IN2 (Bitfield-Mask: 0x01)                    */
#define GPIO_PRT_IN_IN3_Pos               (3UL)                 /*!< GPIO_PRT IN: IN3 (Bit 3)                                  */
#define GPIO_PRT_IN_IN3_Msk               (0x8UL)               /*!< GPIO_PRT IN: IN3 (Bitfield-Mask: 0x01)                    */
#define GPIO_PRT_IN_IN4_Pos               (4UL)                 /*!< GPIO_PRT IN: IN4 (Bit 4)                                  */
#define GPIO_PRT_IN_IN4_Msk               (0x10UL)              /*!< GPIO_PRT IN: IN4 (Bitfield-Mask: 0x01)                    */
#define GPIO_PRT_IN_IN5_Pos               (5UL)                 /*!< GPIO_PRT IN: IN5 (Bit 5)                                  */
#define GPIO_PRT_IN_IN5_Msk               (0x20UL)              /*!< GPIO_PRT IN: IN5 (Bitfield-Mask: 0x01)                    */
#define GPIO_PRT_IN_IN6_Pos               (6UL)                 /*!< GPIO_PRT IN: IN6 (Bit 6)                                  */
#define GPIO_PRT_IN_IN6_Msk               (0x40UL)              /*!< GPIO_PRT IN: IN6 (Bitfield-Mask: 0x01)                    */
#define GPIO_PRT_IN_IN7_Pos               (7UL)                 /*!< GPIO_PRT IN: IN7 (Bit 7)                                  */
#define GPIO_PRT_IN_IN7_Msk               (0x80UL)              /*!< GPIO_PRT IN: IN7 (Bitfield-Mask: 0x01)                    */
#define GPIO_PRT_IN_FLT_IN_Pos            (8UL)                 /*!< GPIO_PRT IN: FLT_IN (Bit 8)                               */
#define GPIO_PRT_IN_FLT_IN_Msk            (0x100UL)             /*!< GPIO_PRT IN: FLT_IN (Bitfield-Mask: 0x01)                 */
/* =====================================================  GPIO_PRT.INTR  ===================================================== */
#define GPIO_PRT_INTR_EDGE0_Pos           (0UL)                 /*!< GPIO_PRT INTR: EDGE0 (Bit 0)                              */
#define GPIO_PRT_INTR_EDGE0_Msk           (0x1UL)               /*!< GPIO_PRT INTR: EDGE0 (Bitfield-Mask: 0x01)                */
#define GPIO_PRT_INTR_EDGE1_Pos           (1UL)                 /*!< GPIO_PRT INTR: EDGE1 (Bit 1)                              */
#define GPIO_PRT_INTR_EDGE1_Msk           (0x2UL)               /*!< GPIO_PRT INTR: EDGE1 (Bitfield-Mask: 0x01)                */
#define GPIO_PRT_INTR_EDGE2_Pos           (2UL)                 /*!< GPIO_PRT INTR: EDGE2 (Bit 2)                              */
#define GPIO_PRT_INTR_EDGE2_Msk           (0x4UL)               /*!< GPIO_PRT INTR: EDGE2 (Bitfield-Mask: 0x01)                */
#define GPIO_PRT_INTR_EDGE3_Pos           (3UL)                 /*!< GPIO_PRT INTR: EDGE3 (Bit 3)                              */
#define GPIO_PRT_INTR_EDGE3_Msk           (0x8UL)               /*!< GPIO_PRT INTR: EDGE3 (Bitfield-Mask: 0x01)                */
#define GPIO_PRT_INTR_EDGE4_Pos           (4UL)                 /*!< GPIO_PRT INTR: EDGE4 (Bit 4)                              */
#define GPIO_PRT_INTR_EDGE4_Msk           (0x10UL)              /*!< GPIO_PRT INTR: EDGE4 (Bitfield-Mask: 0x01)                */
#define GPIO_PRT_INTR_EDGE5_Pos           (5UL)                 /*!< GPIO_PRT INTR: EDGE5 (Bit 5)                              */
#define GPIO_PRT_INTR_EDGE5_Msk           (0x20UL)              /*!< GPIO_PRT INTR: EDGE5 (Bitfield-Mask: 0x01)                */
#define GPIO_PRT_INTR_EDGE6_Pos           (6UL)                 /*!< GPIO_PRT INTR: EDGE6 (Bit 6)                              */
#define GPIO_PRT_INTR_EDGE6_Msk           (0x40UL)              /*!< GPIO_PRT INTR: EDGE6 (Bitfield-Mask: 0x01)                */
#define GPIO_PRT_INTR_EDGE7_Pos           (7UL)                 /*!< GPIO_PRT INTR: EDGE7 (Bit 7)                              */
#define GPIO_PRT_INTR_EDGE7_Msk           (0x80UL)              /*!< GPIO_PRT INTR: EDGE7 (Bitfield-Mask: 0x01)                */
#define GPIO_PRT_INTR_FLT_EDGE_Pos        (8UL)                 /*!< GPIO_PRT INTR: FLT_EDGE (Bit 8)                           */
#define GPIO_PRT_INTR_FLT_EDGE_Msk        (0x100UL)             /*!< GPIO_PRT INTR: FLT_EDGE (Bitfield-Mask: 0x01)             */
#define GPIO_PRT_INTR_IN_IN0_Pos          (16UL)                /*!< GPIO_PRT INTR: IN_IN0 (Bit 16)                            */
#define GPIO_PRT_INTR_IN_IN0_Msk          (0x10000UL)           /*!< GPIO_PRT INTR: IN_IN0 (Bitfield-Mask: 0x01)               */
#define GPIO_PRT_INTR_IN_IN1_Pos          (17UL)                /*!< GPIO_PRT INTR: IN_IN1 (Bit 17)                            */
#define GPIO_PRT_INTR_IN_IN1_Msk          (0x20000UL)           /*!< GPIO_PRT INTR: IN_IN1 (Bitfield-Mask: 0x01)               */
#define GPIO_PRT_INTR_IN_IN2_Pos          (18UL)                /*!< GPIO_PRT INTR: IN_IN2 (Bit 18)                            */
#define GPIO_PRT_INTR_IN_IN2_Msk          (0x40000UL)           /*!< GPIO_PRT INTR: IN_IN2 (Bitfield-Mask: 0x01)               */
#define GPIO_PRT_INTR_IN_IN3_Pos          (19UL)                /*!< GPIO_PRT INTR: IN_IN3 (Bit 19)                            */
#define GPIO_PRT_INTR_IN_IN3_Msk          (0x80000UL)           /*!< GPIO_PRT INTR: IN_IN3 (Bitfield-Mask: 0x01)               */
#define GPIO_PRT_INTR_IN_IN4_Pos          (20UL)                /*!< GPIO_PRT INTR: IN_IN4 (Bit 20)                            */
#define GPIO_PRT_INTR_IN_IN4_Msk          (0x100000UL)          /*!< GPIO_PRT INTR: IN_IN4 (Bitfield-Mask: 0x01)               */
#define GPIO_PRT_INTR_IN_IN5_Pos          (21UL)                /*!< GPIO_PRT INTR: IN_IN5 (Bit 21)                            */
#define GPIO_PRT_INTR_IN_IN5_Msk          (0x200000UL)          /*!< GPIO_PRT INTR: IN_IN5 (Bitfield-Mask: 0x01)               */
#define GPIO_PRT_INTR_IN_IN6_Pos          (22UL)                /*!< GPIO_PRT INTR: IN_IN6 (Bit 22)                            */
#define GPIO_PRT_INTR_IN_IN6_Msk          (0x400000UL)          /*!< GPIO_PRT INTR: IN_IN6 (Bitfield-Mask: 0x01)               */
#define GPIO_PRT_INTR_IN_IN7_Pos          (23UL)                /*!< GPIO_PRT INTR: IN_IN7 (Bit 23)                            */
#define GPIO_PRT_INTR_IN_IN7_Msk          (0x800000UL)          /*!< GPIO_PRT INTR: IN_IN7 (Bitfield-Mask: 0x01)               */
#define GPIO_PRT_INTR_FLT_IN_IN_Pos       (24UL)                /*!< GPIO_PRT INTR: FLT_IN_IN (Bit 24)                         */
#define GPIO_PRT_INTR_FLT_IN_IN_Msk       (0x1000000UL)         /*!< GPIO_PRT INTR: FLT_IN_IN (Bitfield-Mask: 0x01)            */
/* ==================================================  GPIO_PRT.INTR_MASK  =================================================== */
#define GPIO_PRT_INTR_MASK_EDGE0_Pos      (0UL)                 /*!< GPIO_PRT INTR_MASK: EDGE0 (Bit 0)                         */
#define GPIO_PRT_INTR_MASK_EDGE0_Msk      (0x1UL)               /*!< GPIO_PRT INTR_MASK: EDGE0 (Bitfield-Mask: 0x01)           */
#define GPIO_PRT_INTR_MASK_EDGE1_Pos      (1UL)                 /*!< GPIO_PRT INTR_MASK: EDGE1 (Bit 1)                         */
#define GPIO_PRT_INTR_MASK_EDGE1_Msk      (0x2UL)               /*!< GPIO_PRT INTR_MASK: EDGE1 (Bitfield-Mask: 0x01)           */
#define GPIO_PRT_INTR_MASK_EDGE2_Pos      (2UL)                 /*!< GPIO_PRT INTR_MASK: EDGE2 (Bit 2)                         */
#define GPIO_PRT_INTR_MASK_EDGE2_Msk      (0x4UL)               /*!< GPIO_PRT INTR_MASK: EDGE2 (Bitfield-Mask: 0x01)           */
#define GPIO_PRT_INTR_MASK_EDGE3_Pos      (3UL)                 /*!< GPIO_PRT INTR_MASK: EDGE3 (Bit 3)                         */
#define GPIO_PRT_INTR_MASK_EDGE3_Msk      (0x8UL)               /*!< GPIO_PRT INTR_MASK: EDGE3 (Bitfield-Mask: 0x01)           */
#define GPIO_PRT_INTR_MASK_EDGE4_Pos      (4UL)                 /*!< GPIO_PRT INTR_MASK: EDGE4 (Bit 4)                         */
#define GPIO_PRT_INTR_MASK_EDGE4_Msk      (0x10UL)              /*!< GPIO_PRT INTR_MASK: EDGE4 (Bitfield-Mask: 0x01)           */
#define GPIO_PRT_INTR_MASK_EDGE5_Pos      (5UL)                 /*!< GPIO_PRT INTR_MASK: EDGE5 (Bit 5)                         */
#define GPIO_PRT_INTR_MASK_EDGE5_Msk      (0x20UL)              /*!< GPIO_PRT INTR_MASK: EDGE5 (Bitfield-Mask: 0x01)           */
#define GPIO_PRT_INTR_MASK_EDGE6_Pos      (6UL)                 /*!< GPIO_PRT INTR_MASK: EDGE6 (Bit 6)                         */
#define GPIO_PRT_INTR_MASK_EDGE6_Msk      (0x40UL)              /*!< GPIO_PRT INTR_MASK: EDGE6 (Bitfield-Mask: 0x01)           */
#define GPIO_PRT_INTR_MASK_EDGE7_Pos      (7UL)                 /*!< GPIO_PRT INTR_MASK: EDGE7 (Bit 7)                         */
#define GPIO_PRT_INTR_MASK_EDGE7_Msk      (0x80UL)              /*!< GPIO_PRT INTR_MASK: EDGE7 (Bitfield-Mask: 0x01)           */
#define GPIO_PRT_INTR_MASK_FLT_EDGE_Pos   (8UL)                 /*!< GPIO_PRT INTR_MASK: FLT_EDGE (Bit 8)                      */
#define GPIO_PRT_INTR_MASK_FLT_EDGE_Msk   (0x100UL)             /*!< GPIO_PRT INTR_MASK: FLT_EDGE (Bitfield-Mask: 0x01)        */
/* =================================================  GPIO_PRT.INTR_MASKED  ================================================== */
#define GPIO_PRT_INTR_MASKED_EDGE0_Pos    (0UL)                 /*!< GPIO_PRT INTR_MASKED: EDGE0 (Bit 0)                       */
#define GPIO_PRT_INTR_MASKED_EDGE0_Msk    (0x1UL)               /*!< GPIO_PRT INTR_MASKED: EDGE0 (Bitfield-Mask: 0x01)         */
#define GPIO_PRT_INTR_MASKED_EDGE1_Pos    (1UL)                 /*!< GPIO_PRT INTR_MASKED: EDGE1 (Bit 1)                       */
#define GPIO_PRT_INTR_MASKED_EDGE1_Msk    (0x2UL)               /*!< GPIO_PRT INTR_MASKED: EDGE1 (Bitfield-Mask: 0x01)         */
#define GPIO_PRT_INTR_MASKED_EDGE2_Pos    (2UL)                 /*!< GPIO_PRT INTR_MASKED: EDGE2 (Bit 2)                       */
#define GPIO_PRT_INTR_MASKED_EDGE2_Msk    (0x4UL)               /*!< GPIO_PRT INTR_MASKED: EDGE2 (Bitfield-Mask: 0x01)         */
#define GPIO_PRT_INTR_MASKED_EDGE3_Pos    (3UL)                 /*!< GPIO_PRT INTR_MASKED: EDGE3 (Bit 3)                       */
#define GPIO_PRT_INTR_MASKED_EDGE3_Msk    (0x8UL)               /*!< GPIO_PRT INTR_MASKED: EDGE3 (Bitfield-Mask: 0x01)         */
#define GPIO_PRT_INTR_MASKED_EDGE4_Pos    (4UL)                 /*!< GPIO_PRT INTR_MASKED: EDGE4 (Bit 4)                       */
#define GPIO_PRT_INTR_MASKED_EDGE4_Msk    (0x10UL)              /*!< GPIO_PRT INTR_MASKED: EDGE4 (Bitfield-Mask: 0x01)         */
#define GPIO_PRT_INTR_MASKED_EDGE5_Pos    (5UL)                 /*!< GPIO_PRT INTR_MASKED: EDGE5 (Bit 5)                       */
#define GPIO_PRT_INTR_MASKED_EDGE5_Msk    (0x20UL)              /*!< GPIO_PRT INTR_MASKED: EDGE5 (Bitfield-Mask: 0x01)         */
#define GPIO_PRT_INTR_MASKED_EDGE6_Pos    (6UL)                 /*!< GPIO_PRT INTR_MASKED: EDGE6 (Bit 6)                       */
#define GPIO_PRT_INTR_MASKED_EDGE6_Msk    (0x40UL)              /*!< GPIO_PRT INTR_MASKED: EDGE6 (Bitfield-Mask: 0x01)         */
#define GPIO_PRT_INTR_MASKED_EDGE7_Pos    (7UL)                 /*!< GPIO_PRT INTR_MASKED: EDGE7 (Bit 7)                       */
#define GPIO_PRT_INTR_MASKED_EDGE7_Msk    (0x80UL)              /*!< GPIO_PRT INTR_MASKED: EDGE7 (Bitfield-Mask: 0x01)         */
#define GPIO_PRT_INTR_MASKED_FLT_EDGE_Pos (8UL)                 /*!< GPIO_PRT INTR_MASKED: FLT_EDGE (Bit 8)                    */
#define GPIO_PRT_INTR_MASKED_FLT_EDGE_Msk (0x100UL)             /*!< GPIO_PRT INTR_MASKED: FLT_EDGE (Bitfield-Mask: 0x01)      */
/* ===================================================  GPIO_PRT.INTR_SET  =================================================== */
#define GPIO_PRT_INTR_SET_EDGE0_Pos       (0UL)                 /*!< GPIO_PRT INTR_SET: EDGE0 (Bit 0)                          */
#define GPIO_PRT_INTR_SET_EDGE0_Msk       (0x1UL)               /*!< GPIO_PRT INTR_SET: EDGE0 (Bitfield-Mask: 0x01)            */
#define GPIO_PRT_INTR_SET_EDGE1_Pos       (1UL)                 /*!< GPIO_PRT INTR_SET: EDGE1 (Bit 1)                          */
#define GPIO_PRT_INTR_SET_EDGE1_Msk       (0x2UL)               /*!< GPIO_PRT INTR_SET: EDGE1 (Bitfield-Mask: 0x01)            */
#define GPIO_PRT_INTR_SET_EDGE2_Pos       (2UL)                 /*!< GPIO_PRT INTR_SET: EDGE2 (Bit 2)                          */
#define GPIO_PRT_INTR_SET_EDGE2_Msk       (0x4UL)               /*!< GPIO_PRT INTR_SET: EDGE2 (Bitfield-Mask: 0x01)            */
#define GPIO_PRT_INTR_SET_EDGE3_Pos       (3UL)                 /*!< GPIO_PRT INTR_SET: EDGE3 (Bit 3)                          */
#define GPIO_PRT_INTR_SET_EDGE3_Msk       (0x8UL)               /*!< GPIO_PRT INTR_SET: EDGE3 (Bitfield-Mask: 0x01)            */
#define GPIO_PRT_INTR_SET_EDGE4_Pos       (4UL)                 /*!< GPIO_PRT INTR_SET: EDGE4 (Bit 4)                          */
#define GPIO_PRT_INTR_SET_EDGE4_Msk       (0x10UL)              /*!< GPIO_PRT INTR_SET: EDGE4 (Bitfield-Mask: 0x01)            */
#define GPIO_PRT_INTR_SET_EDGE5_Pos       (5UL)                 /*!< GPIO_PRT INTR_SET: EDGE5 (Bit 5)                          */
#define GPIO_PRT_INTR_SET_EDGE5_Msk       (0x20UL)              /*!< GPIO_PRT INTR_SET: EDGE5 (Bitfield-Mask: 0x01)            */
#define GPIO_PRT_INTR_SET_EDGE6_Pos       (6UL)                 /*!< GPIO_PRT INTR_SET: EDGE6 (Bit 6)                          */
#define GPIO_PRT_INTR_SET_EDGE6_Msk       (0x40UL)              /*!< GPIO_PRT INTR_SET: EDGE6 (Bitfield-Mask: 0x01)            */
#define GPIO_PRT_INTR_SET_EDGE7_Pos       (7UL)                 /*!< GPIO_PRT INTR_SET: EDGE7 (Bit 7)                          */
#define GPIO_PRT_INTR_SET_EDGE7_Msk       (0x80UL)              /*!< GPIO_PRT INTR_SET: EDGE7 (Bitfield-Mask: 0x01)            */
#define GPIO_PRT_INTR_SET_FLT_EDGE_Pos    (8UL)                 /*!< GPIO_PRT INTR_SET: FLT_EDGE (Bit 8)                       */
#define GPIO_PRT_INTR_SET_FLT_EDGE_Msk    (0x100UL)             /*!< GPIO_PRT INTR_SET: FLT_EDGE (Bitfield-Mask: 0x01)         */
/* ===================================================  GPIO_PRT.INTR_CFG  =================================================== */
#define GPIO_PRT_INTR_CFG_EDGE0_SEL_Pos   (0UL)                 /*!< GPIO_PRT INTR_CFG: EDGE0_SEL (Bit 0)                      */
#define GPIO_PRT_INTR_CFG_EDGE0_SEL_Msk   (0x3UL)               /*!< GPIO_PRT INTR_CFG: EDGE0_SEL (Bitfield-Mask: 0x03)        */
#define GPIO_PRT_INTR_CFG_EDGE1_SEL_Pos   (2UL)                 /*!< GPIO_PRT INTR_CFG: EDGE1_SEL (Bit 2)                      */
#define GPIO_PRT_INTR_CFG_EDGE1_SEL_Msk   (0xcUL)               /*!< GPIO_PRT INTR_CFG: EDGE1_SEL (Bitfield-Mask: 0x03)        */
#define GPIO_PRT_INTR_CFG_EDGE2_SEL_Pos   (4UL)                 /*!< GPIO_PRT INTR_CFG: EDGE2_SEL (Bit 4)                      */
#define GPIO_PRT_INTR_CFG_EDGE2_SEL_Msk   (0x30UL)              /*!< GPIO_PRT INTR_CFG: EDGE2_SEL (Bitfield-Mask: 0x03)        */
#define GPIO_PRT_INTR_CFG_EDGE3_SEL_Pos   (6UL)                 /*!< GPIO_PRT INTR_CFG: EDGE3_SEL (Bit 6)                      */
#define GPIO_PRT_INTR_CFG_EDGE3_SEL_Msk   (0xc0UL)              /*!< GPIO_PRT INTR_CFG: EDGE3_SEL (Bitfield-Mask: 0x03)        */
#define GPIO_PRT_INTR_CFG_EDGE4_SEL_Pos   (8UL)                 /*!< GPIO_PRT INTR_CFG: EDGE4_SEL (Bit 8)                      */
#define GPIO_PRT_INTR_CFG_EDGE4_SEL_Msk   (0x300UL)             /*!< GPIO_PRT INTR_CFG: EDGE4_SEL (Bitfield-Mask: 0x03)        */
#define GPIO_PRT_INTR_CFG_EDGE5_SEL_Pos   (10UL)                /*!< GPIO_PRT INTR_CFG: EDGE5_SEL (Bit 10)                     */
#define GPIO_PRT_INTR_CFG_EDGE5_SEL_Msk   (0xc00UL)             /*!< GPIO_PRT INTR_CFG: EDGE5_SEL (Bitfield-Mask: 0x03)        */
#define GPIO_PRT_INTR_CFG_EDGE6_SEL_Pos   (12UL)                /*!< GPIO_PRT INTR_CFG: EDGE6_SEL (Bit 12)                     */
#define GPIO_PRT_INTR_CFG_EDGE6_SEL_Msk   (0x3000UL)            /*!< GPIO_PRT INTR_CFG: EDGE6_SEL (Bitfield-Mask: 0x03)        */
#define GPIO_PRT_INTR_CFG_EDGE7_SEL_Pos   (14UL)                /*!< GPIO_PRT INTR_CFG: EDGE7_SEL (Bit 14)                     */
#define GPIO_PRT_INTR_CFG_EDGE7_SEL_Msk   (0xc000UL)            /*!< GPIO_PRT INTR_CFG: EDGE7_SEL (Bitfield-Mask: 0x03)        */
#define GPIO_PRT_INTR_CFG_FLT_EDGE_SEL_Pos (16UL)               /*!< GPIO_PRT INTR_CFG: FLT_EDGE_SEL (Bit 16)                  */
#define GPIO_PRT_INTR_CFG_FLT_EDGE_SEL_Msk (0x30000UL)          /*!< GPIO_PRT INTR_CFG: FLT_EDGE_SEL (Bitfield-Mask: 0x03)     */
#define GPIO_PRT_INTR_CFG_FLT_SEL_Pos     (18UL)                /*!< GPIO_PRT INTR_CFG: FLT_SEL (Bit 18)                       */
#define GPIO_PRT_INTR_CFG_FLT_SEL_Msk     (0x1c0000UL)          /*!< GPIO_PRT INTR_CFG: FLT_SEL (Bitfield-Mask: 0x07)          */
/* =====================================================  GPIO_PRT.CFG  ====================================================== */
#define GPIO_PRT_CFG_DRIVE_MODE0_Pos      (0UL)                 /*!< GPIO_PRT CFG: DRIVE_MODE0 (Bit 0)                         */
#define GPIO_PRT_CFG_DRIVE_MODE0_Msk      (0x7UL)               /*!< GPIO_PRT CFG: DRIVE_MODE0 (Bitfield-Mask: 0x07)           */
#define GPIO_PRT_CFG_IN_EN0_Pos           (3UL)                 /*!< GPIO_PRT CFG: IN_EN0 (Bit 3)                              */
#define GPIO_PRT_CFG_IN_EN0_Msk           (0x8UL)               /*!< GPIO_PRT CFG: IN_EN0 (Bitfield-Mask: 0x01)                */
#define GPIO_PRT_CFG_DRIVE_MODE1_Pos      (4UL)                 /*!< GPIO_PRT CFG: DRIVE_MODE1 (Bit 4)                         */
#define GPIO_PRT_CFG_DRIVE_MODE1_Msk      (0x70UL)              /*!< GPIO_PRT CFG: DRIVE_MODE1 (Bitfield-Mask: 0x07)           */
#define GPIO_PRT_CFG_IN_EN1_Pos           (7UL)                 /*!< GPIO_PRT CFG: IN_EN1 (Bit 7)                              */
#define GPIO_PRT_CFG_IN_EN1_Msk           (0x80UL)              /*!< GPIO_PRT CFG: IN_EN1 (Bitfield-Mask: 0x01)                */
#define GPIO_PRT_CFG_DRIVE_MODE2_Pos      (8UL)                 /*!< GPIO_PRT CFG: DRIVE_MODE2 (Bit 8)                         */
#define GPIO_PRT_CFG_DRIVE_MODE2_Msk      (0x700UL)             /*!< GPIO_PRT CFG: DRIVE_MODE2 (Bitfield-Mask: 0x07)           */
#define GPIO_PRT_CFG_IN_EN2_Pos           (11UL)                /*!< GPIO_PRT CFG: IN_EN2 (Bit 11)                             */
#define GPIO_PRT_CFG_IN_EN2_Msk           (0x800UL)             /*!< GPIO_PRT CFG: IN_EN2 (Bitfield-Mask: 0x01)                */
#define GPIO_PRT_CFG_DRIVE_MODE3_Pos      (12UL)                /*!< GPIO_PRT CFG: DRIVE_MODE3 (Bit 12)                        */
#define GPIO_PRT_CFG_DRIVE_MODE3_Msk      (0x7000UL)            /*!< GPIO_PRT CFG: DRIVE_MODE3 (Bitfield-Mask: 0x07)           */
#define GPIO_PRT_CFG_IN_EN3_Pos           (15UL)                /*!< GPIO_PRT CFG: IN_EN3 (Bit 15)                             */
#define GPIO_PRT_CFG_IN_EN3_Msk           (0x8000UL)            /*!< GPIO_PRT CFG: IN_EN3 (Bitfield-Mask: 0x01)                */
#define GPIO_PRT_CFG_DRIVE_MODE4_Pos      (16UL)                /*!< GPIO_PRT CFG: DRIVE_MODE4 (Bit 16)                        */
#define GPIO_PRT_CFG_DRIVE_MODE4_Msk      (0x70000UL)           /*!< GPIO_PRT CFG: DRIVE_MODE4 (Bitfield-Mask: 0x07)           */
#define GPIO_PRT_CFG_IN_EN4_Pos           (19UL)                /*!< GPIO_PRT CFG: IN_EN4 (Bit 19)                             */
#define GPIO_PRT_CFG_IN_EN4_Msk           (0x80000UL)           /*!< GPIO_PRT CFG: IN_EN4 (Bitfield-Mask: 0x01)                */
#define GPIO_PRT_CFG_DRIVE_MODE5_Pos      (20UL)                /*!< GPIO_PRT CFG: DRIVE_MODE5 (Bit 20)                        */
#define GPIO_PRT_CFG_DRIVE_MODE5_Msk      (0x700000UL)          /*!< GPIO_PRT CFG: DRIVE_MODE5 (Bitfield-Mask: 0x07)           */
#define GPIO_PRT_CFG_IN_EN5_Pos           (23UL)                /*!< GPIO_PRT CFG: IN_EN5 (Bit 23)                             */
#define GPIO_PRT_CFG_IN_EN5_Msk           (0x800000UL)          /*!< GPIO_PRT CFG: IN_EN5 (Bitfield-Mask: 0x01)                */
#define GPIO_PRT_CFG_DRIVE_MODE6_Pos      (24UL)                /*!< GPIO_PRT CFG: DRIVE_MODE6 (Bit 24)                        */
#define GPIO_PRT_CFG_DRIVE_MODE6_Msk      (0x7000000UL)         /*!< GPIO_PRT CFG: DRIVE_MODE6 (Bitfield-Mask: 0x07)           */
#define GPIO_PRT_CFG_IN_EN6_Pos           (27UL)                /*!< GPIO_PRT CFG: IN_EN6 (Bit 27)                             */
#define GPIO_PRT_CFG_IN_EN6_Msk           (0x8000000UL)         /*!< GPIO_PRT CFG: IN_EN6 (Bitfield-Mask: 0x01)                */
#define GPIO_PRT_CFG_DRIVE_MODE7_Pos      (28UL)                /*!< GPIO_PRT CFG: DRIVE_MODE7 (Bit 28)                        */
#define GPIO_PRT_CFG_DRIVE_MODE7_Msk      (0x70000000UL)        /*!< GPIO_PRT CFG: DRIVE_MODE7 (Bitfield-Mask: 0x07)           */
#define GPIO_PRT_CFG_IN_EN7_Pos           (31UL)                /*!< GPIO_PRT CFG: IN_EN7 (Bit 31)                             */
#define GPIO_PRT_CFG_IN_EN7_Msk           (0x80000000UL)        /*!< GPIO_PRT CFG: IN_EN7 (Bitfield-Mask: 0x01)                */
/* ====================================================  GPIO_PRT.CFG_IN  ==================================================== */
#define GPIO_PRT_CFG_IN_VTRIP_SEL0_0_Pos  (0UL)                 /*!< GPIO_PRT CFG_IN: VTRIP_SEL0_0 (Bit 0)                     */
#define GPIO_PRT_CFG_IN_VTRIP_SEL0_0_Msk  (0x1UL)               /*!< GPIO_PRT CFG_IN: VTRIP_SEL0_0 (Bitfield-Mask: 0x01)       */
#define GPIO_PRT_CFG_IN_VTRIP_SEL1_0_Pos  (1UL)                 /*!< GPIO_PRT CFG_IN: VTRIP_SEL1_0 (Bit 1)                     */
#define GPIO_PRT_CFG_IN_VTRIP_SEL1_0_Msk  (0x2UL)               /*!< GPIO_PRT CFG_IN: VTRIP_SEL1_0 (Bitfield-Mask: 0x01)       */
#define GPIO_PRT_CFG_IN_VTRIP_SEL2_0_Pos  (2UL)                 /*!< GPIO_PRT CFG_IN: VTRIP_SEL2_0 (Bit 2)                     */
#define GPIO_PRT_CFG_IN_VTRIP_SEL2_0_Msk  (0x4UL)               /*!< GPIO_PRT CFG_IN: VTRIP_SEL2_0 (Bitfield-Mask: 0x01)       */
#define GPIO_PRT_CFG_IN_VTRIP_SEL3_0_Pos  (3UL)                 /*!< GPIO_PRT CFG_IN: VTRIP_SEL3_0 (Bit 3)                     */
#define GPIO_PRT_CFG_IN_VTRIP_SEL3_0_Msk  (0x8UL)               /*!< GPIO_PRT CFG_IN: VTRIP_SEL3_0 (Bitfield-Mask: 0x01)       */
#define GPIO_PRT_CFG_IN_VTRIP_SEL4_0_Pos  (4UL)                 /*!< GPIO_PRT CFG_IN: VTRIP_SEL4_0 (Bit 4)                     */
#define GPIO_PRT_CFG_IN_VTRIP_SEL4_0_Msk  (0x10UL)              /*!< GPIO_PRT CFG_IN: VTRIP_SEL4_0 (Bitfield-Mask: 0x01)       */
#define GPIO_PRT_CFG_IN_VTRIP_SEL5_0_Pos  (5UL)                 /*!< GPIO_PRT CFG_IN: VTRIP_SEL5_0 (Bit 5)                     */
#define GPIO_PRT_CFG_IN_VTRIP_SEL5_0_Msk  (0x20UL)              /*!< GPIO_PRT CFG_IN: VTRIP_SEL5_0 (Bitfield-Mask: 0x01)       */
#define GPIO_PRT_CFG_IN_VTRIP_SEL6_0_Pos  (6UL)                 /*!< GPIO_PRT CFG_IN: VTRIP_SEL6_0 (Bit 6)                     */
#define GPIO_PRT_CFG_IN_VTRIP_SEL6_0_Msk  (0x40UL)              /*!< GPIO_PRT CFG_IN: VTRIP_SEL6_0 (Bitfield-Mask: 0x01)       */
#define GPIO_PRT_CFG_IN_VTRIP_SEL7_0_Pos  (7UL)                 /*!< GPIO_PRT CFG_IN: VTRIP_SEL7_0 (Bit 7)                     */
#define GPIO_PRT_CFG_IN_VTRIP_SEL7_0_Msk  (0x80UL)              /*!< GPIO_PRT CFG_IN: VTRIP_SEL7_0 (Bitfield-Mask: 0x01)       */
/* ===================================================  GPIO_PRT.CFG_OUT  ==================================================== */
#define GPIO_PRT_CFG_OUT_SLOW0_Pos        (0UL)                 /*!< GPIO_PRT CFG_OUT: SLOW0 (Bit 0)                           */
#define GPIO_PRT_CFG_OUT_SLOW0_Msk        (0x1UL)               /*!< GPIO_PRT CFG_OUT: SLOW0 (Bitfield-Mask: 0x01)             */
#define GPIO_PRT_CFG_OUT_SLOW1_Pos        (1UL)                 /*!< GPIO_PRT CFG_OUT: SLOW1 (Bit 1)                           */
#define GPIO_PRT_CFG_OUT_SLOW1_Msk        (0x2UL)               /*!< GPIO_PRT CFG_OUT: SLOW1 (Bitfield-Mask: 0x01)             */
#define GPIO_PRT_CFG_OUT_SLOW2_Pos        (2UL)                 /*!< GPIO_PRT CFG_OUT: SLOW2 (Bit 2)                           */
#define GPIO_PRT_CFG_OUT_SLOW2_Msk        (0x4UL)               /*!< GPIO_PRT CFG_OUT: SLOW2 (Bitfield-Mask: 0x01)             */
#define GPIO_PRT_CFG_OUT_SLOW3_Pos        (3UL)                 /*!< GPIO_PRT CFG_OUT: SLOW3 (Bit 3)                           */
#define GPIO_PRT_CFG_OUT_SLOW3_Msk        (0x8UL)               /*!< GPIO_PRT CFG_OUT: SLOW3 (Bitfield-Mask: 0x01)             */
#define GPIO_PRT_CFG_OUT_SLOW4_Pos        (4UL)                 /*!< GPIO_PRT CFG_OUT: SLOW4 (Bit 4)                           */
#define GPIO_PRT_CFG_OUT_SLOW4_Msk        (0x10UL)              /*!< GPIO_PRT CFG_OUT: SLOW4 (Bitfield-Mask: 0x01)             */
#define GPIO_PRT_CFG_OUT_SLOW5_Pos        (5UL)                 /*!< GPIO_PRT CFG_OUT: SLOW5 (Bit 5)                           */
#define GPIO_PRT_CFG_OUT_SLOW5_Msk        (0x20UL)              /*!< GPIO_PRT CFG_OUT: SLOW5 (Bitfield-Mask: 0x01)             */
#define GPIO_PRT_CFG_OUT_SLOW6_Pos        (6UL)                 /*!< GPIO_PRT CFG_OUT: SLOW6 (Bit 6)                           */
#define GPIO_PRT_CFG_OUT_SLOW6_Msk        (0x40UL)              /*!< GPIO_PRT CFG_OUT: SLOW6 (Bitfield-Mask: 0x01)             */
#define GPIO_PRT_CFG_OUT_SLOW7_Pos        (7UL)                 /*!< GPIO_PRT CFG_OUT: SLOW7 (Bit 7)                           */
#define GPIO_PRT_CFG_OUT_SLOW7_Msk        (0x80UL)              /*!< GPIO_PRT CFG_OUT: SLOW7 (Bitfield-Mask: 0x01)             */
#define GPIO_PRT_CFG_OUT_DRIVE_SEL0_Pos   (16UL)                /*!< GPIO_PRT CFG_OUT: DRIVE_SEL0 (Bit 16)                     */
#define GPIO_PRT_CFG_OUT_DRIVE_SEL0_Msk   (0x30000UL)           /*!< GPIO_PRT CFG_OUT: DRIVE_SEL0 (Bitfield-Mask: 0x03)        */
#define GPIO_PRT_CFG_OUT_DRIVE_SEL1_Pos   (18UL)                /*!< GPIO_PRT CFG_OUT: DRIVE_SEL1 (Bit 18)                     */
#define GPIO_PRT_CFG_OUT_DRIVE_SEL1_Msk   (0xc0000UL)           /*!< GPIO_PRT CFG_OUT: DRIVE_SEL1 (Bitfield-Mask: 0x03)        */
#define GPIO_PRT_CFG_OUT_DRIVE_SEL2_Pos   (20UL)                /*!< GPIO_PRT CFG_OUT: DRIVE_SEL2 (Bit 20)                     */
#define GPIO_PRT_CFG_OUT_DRIVE_SEL2_Msk   (0x300000UL)          /*!< GPIO_PRT CFG_OUT: DRIVE_SEL2 (Bitfield-Mask: 0x03)        */
#define GPIO_PRT_CFG_OUT_DRIVE_SEL3_Pos   (22UL)                /*!< GPIO_PRT CFG_OUT: DRIVE_SEL3 (Bit 22)                     */
#define GPIO_PRT_CFG_OUT_DRIVE_SEL3_Msk   (0xc00000UL)          /*!< GPIO_PRT CFG_OUT: DRIVE_SEL3 (Bitfield-Mask: 0x03)        */
#define GPIO_PRT_CFG_OUT_DRIVE_SEL4_Pos   (24UL)                /*!< GPIO_PRT CFG_OUT: DRIVE_SEL4 (Bit 24)                     */
#define GPIO_PRT_CFG_OUT_DRIVE_SEL4_Msk   (0x3000000UL)         /*!< GPIO_PRT CFG_OUT: DRIVE_SEL4 (Bitfield-Mask: 0x03)        */
#define GPIO_PRT_CFG_OUT_DRIVE_SEL5_Pos   (26UL)                /*!< GPIO_PRT CFG_OUT: DRIVE_SEL5 (Bit 26)                     */
#define GPIO_PRT_CFG_OUT_DRIVE_SEL5_Msk   (0xc000000UL)         /*!< GPIO_PRT CFG_OUT: DRIVE_SEL5 (Bitfield-Mask: 0x03)        */
#define GPIO_PRT_CFG_OUT_DRIVE_SEL6_Pos   (28UL)                /*!< GPIO_PRT CFG_OUT: DRIVE_SEL6 (Bit 28)                     */
#define GPIO_PRT_CFG_OUT_DRIVE_SEL6_Msk   (0x30000000UL)        /*!< GPIO_PRT CFG_OUT: DRIVE_SEL6 (Bitfield-Mask: 0x03)        */
#define GPIO_PRT_CFG_OUT_DRIVE_SEL7_Pos   (30UL)                /*!< GPIO_PRT CFG_OUT: DRIVE_SEL7 (Bit 30)                     */
#define GPIO_PRT_CFG_OUT_DRIVE_SEL7_Msk   (0xc0000000UL)        /*!< GPIO_PRT CFG_OUT: DRIVE_SEL7 (Bitfield-Mask: 0x03)        */
/* ===================================================  GPIO_PRT.CFG_SIO  ==================================================== */
#define GPIO_PRT_CFG_SIO_VREG_EN01_Pos    (0UL)                 /*!< GPIO_PRT CFG_SIO: VREG_EN01 (Bit 0)                       */
#define GPIO_PRT_CFG_SIO_VREG_EN01_Msk    (0x1UL)               /*!< GPIO_PRT CFG_SIO: VREG_EN01 (Bitfield-Mask: 0x01)         */
#define GPIO_PRT_CFG_SIO_IBUF_SEL01_Pos   (1UL)                 /*!< GPIO_PRT CFG_SIO: IBUF_SEL01 (Bit 1)                      */
#define GPIO_PRT_CFG_SIO_IBUF_SEL01_Msk   (0x2UL)               /*!< GPIO_PRT CFG_SIO: IBUF_SEL01 (Bitfield-Mask: 0x01)        */
#define GPIO_PRT_CFG_SIO_VTRIP_SEL01_Pos  (2UL)                 /*!< GPIO_PRT CFG_SIO: VTRIP_SEL01 (Bit 2)                     */
#define GPIO_PRT_CFG_SIO_VTRIP_SEL01_Msk  (0x4UL)               /*!< GPIO_PRT CFG_SIO: VTRIP_SEL01 (Bitfield-Mask: 0x01)       */
#define GPIO_PRT_CFG_SIO_VREF_SEL01_Pos   (3UL)                 /*!< GPIO_PRT CFG_SIO: VREF_SEL01 (Bit 3)                      */
#define GPIO_PRT_CFG_SIO_VREF_SEL01_Msk   (0x18UL)              /*!< GPIO_PRT CFG_SIO: VREF_SEL01 (Bitfield-Mask: 0x03)        */
#define GPIO_PRT_CFG_SIO_VOH_SEL01_Pos    (5UL)                 /*!< GPIO_PRT CFG_SIO: VOH_SEL01 (Bit 5)                       */
#define GPIO_PRT_CFG_SIO_VOH_SEL01_Msk    (0xe0UL)              /*!< GPIO_PRT CFG_SIO: VOH_SEL01 (Bitfield-Mask: 0x07)         */
#define GPIO_PRT_CFG_SIO_VREG_EN23_Pos    (8UL)                 /*!< GPIO_PRT CFG_SIO: VREG_EN23 (Bit 8)                       */
#define GPIO_PRT_CFG_SIO_VREG_EN23_Msk    (0x100UL)             /*!< GPIO_PRT CFG_SIO: VREG_EN23 (Bitfield-Mask: 0x01)         */
#define GPIO_PRT_CFG_SIO_IBUF_SEL23_Pos   (9UL)                 /*!< GPIO_PRT CFG_SIO: IBUF_SEL23 (Bit 9)                      */
#define GPIO_PRT_CFG_SIO_IBUF_SEL23_Msk   (0x200UL)             /*!< GPIO_PRT CFG_SIO: IBUF_SEL23 (Bitfield-Mask: 0x01)        */
#define GPIO_PRT_CFG_SIO_VTRIP_SEL23_Pos  (10UL)                /*!< GPIO_PRT CFG_SIO: VTRIP_SEL23 (Bit 10)                    */
#define GPIO_PRT_CFG_SIO_VTRIP_SEL23_Msk  (0x400UL)             /*!< GPIO_PRT CFG_SIO: VTRIP_SEL23 (Bitfield-Mask: 0x01)       */
#define GPIO_PRT_CFG_SIO_VREF_SEL23_Pos   (11UL)                /*!< GPIO_PRT CFG_SIO: VREF_SEL23 (Bit 11)                     */
#define GPIO_PRT_CFG_SIO_VREF_SEL23_Msk   (0x1800UL)            /*!< GPIO_PRT CFG_SIO: VREF_SEL23 (Bitfield-Mask: 0x03)        */
#define GPIO_PRT_CFG_SIO_VOH_SEL23_Pos    (13UL)                /*!< GPIO_PRT CFG_SIO: VOH_SEL23 (Bit 13)                      */
#define GPIO_PRT_CFG_SIO_VOH_SEL23_Msk    (0xe000UL)            /*!< GPIO_PRT CFG_SIO: VOH_SEL23 (Bitfield-Mask: 0x07)         */
#define GPIO_PRT_CFG_SIO_VREG_EN45_Pos    (16UL)                /*!< GPIO_PRT CFG_SIO: VREG_EN45 (Bit 16)                      */
#define GPIO_PRT_CFG_SIO_VREG_EN45_Msk    (0x10000UL)           /*!< GPIO_PRT CFG_SIO: VREG_EN45 (Bitfield-Mask: 0x01)         */
#define GPIO_PRT_CFG_SIO_IBUF_SEL45_Pos   (17UL)                /*!< GPIO_PRT CFG_SIO: IBUF_SEL45 (Bit 17)                     */
#define GPIO_PRT_CFG_SIO_IBUF_SEL45_Msk   (0x20000UL)           /*!< GPIO_PRT CFG_SIO: IBUF_SEL45 (Bitfield-Mask: 0x01)        */
#define GPIO_PRT_CFG_SIO_VTRIP_SEL45_Pos  (18UL)                /*!< GPIO_PRT CFG_SIO: VTRIP_SEL45 (Bit 18)                    */
#define GPIO_PRT_CFG_SIO_VTRIP_SEL45_Msk  (0x40000UL)           /*!< GPIO_PRT CFG_SIO: VTRIP_SEL45 (Bitfield-Mask: 0x01)       */
#define GPIO_PRT_CFG_SIO_VREF_SEL45_Pos   (19UL)                /*!< GPIO_PRT CFG_SIO: VREF_SEL45 (Bit 19)                     */
#define GPIO_PRT_CFG_SIO_VREF_SEL45_Msk   (0x180000UL)          /*!< GPIO_PRT CFG_SIO: VREF_SEL45 (Bitfield-Mask: 0x03)        */
#define GPIO_PRT_CFG_SIO_VOH_SEL45_Pos    (21UL)                /*!< GPIO_PRT CFG_SIO: VOH_SEL45 (Bit 21)                      */
#define GPIO_PRT_CFG_SIO_VOH_SEL45_Msk    (0xe00000UL)          /*!< GPIO_PRT CFG_SIO: VOH_SEL45 (Bitfield-Mask: 0x07)         */
#define GPIO_PRT_CFG_SIO_VREG_EN67_Pos    (24UL)                /*!< GPIO_PRT CFG_SIO: VREG_EN67 (Bit 24)                      */
#define GPIO_PRT_CFG_SIO_VREG_EN67_Msk    (0x1000000UL)         /*!< GPIO_PRT CFG_SIO: VREG_EN67 (Bitfield-Mask: 0x01)         */
#define GPIO_PRT_CFG_SIO_IBUF_SEL67_Pos   (25UL)                /*!< GPIO_PRT CFG_SIO: IBUF_SEL67 (Bit 25)                     */
#define GPIO_PRT_CFG_SIO_IBUF_SEL67_Msk   (0x2000000UL)         /*!< GPIO_PRT CFG_SIO: IBUF_SEL67 (Bitfield-Mask: 0x01)        */
#define GPIO_PRT_CFG_SIO_VTRIP_SEL67_Pos  (26UL)                /*!< GPIO_PRT CFG_SIO: VTRIP_SEL67 (Bit 26)                    */
#define GPIO_PRT_CFG_SIO_VTRIP_SEL67_Msk  (0x4000000UL)         /*!< GPIO_PRT CFG_SIO: VTRIP_SEL67 (Bitfield-Mask: 0x01)       */
#define GPIO_PRT_CFG_SIO_VREF_SEL67_Pos   (27UL)                /*!< GPIO_PRT CFG_SIO: VREF_SEL67 (Bit 27)                     */
#define GPIO_PRT_CFG_SIO_VREF_SEL67_Msk   (0x18000000UL)        /*!< GPIO_PRT CFG_SIO: VREF_SEL67 (Bitfield-Mask: 0x03)        */
#define GPIO_PRT_CFG_SIO_VOH_SEL67_Pos    (29UL)                /*!< GPIO_PRT CFG_SIO: VOH_SEL67 (Bit 29)                      */
#define GPIO_PRT_CFG_SIO_VOH_SEL67_Msk    (0xe0000000UL)        /*!< GPIO_PRT CFG_SIO: VOH_SEL67 (Bitfield-Mask: 0x07)         */
/* ================================================  GPIO_PRT.CFG_IN_GPIO5V  ================================================= */
#define GPIO_PRT_CFG_IN_GPIO5V_VTRIP_SEL0_1_Pos (0UL)           /*!< GPIO_PRT CFG_IN_GPIO5V: VTRIP_SEL0_1 (Bit 0)              */
#define GPIO_PRT_CFG_IN_GPIO5V_VTRIP_SEL0_1_Msk (0x1UL)         /*!< GPIO_PRT CFG_IN_GPIO5V: VTRIP_SEL0_1 (Bitfield-Mask: 0x01)*/
#define GPIO_PRT_CFG_IN_GPIO5V_VTRIP_SEL1_1_Pos (1UL)           /*!< GPIO_PRT CFG_IN_GPIO5V: VTRIP_SEL1_1 (Bit 1)              */
#define GPIO_PRT_CFG_IN_GPIO5V_VTRIP_SEL1_1_Msk (0x2UL)         /*!< GPIO_PRT CFG_IN_GPIO5V: VTRIP_SEL1_1 (Bitfield-Mask: 0x01)*/
#define GPIO_PRT_CFG_IN_GPIO5V_VTRIP_SEL2_1_Pos (2UL)           /*!< GPIO_PRT CFG_IN_GPIO5V: VTRIP_SEL2_1 (Bit 2)              */
#define GPIO_PRT_CFG_IN_GPIO5V_VTRIP_SEL2_1_Msk (0x4UL)         /*!< GPIO_PRT CFG_IN_GPIO5V: VTRIP_SEL2_1 (Bitfield-Mask: 0x01)*/
#define GPIO_PRT_CFG_IN_GPIO5V_VTRIP_SEL3_1_Pos (3UL)           /*!< GPIO_PRT CFG_IN_GPIO5V: VTRIP_SEL3_1 (Bit 3)              */
#define GPIO_PRT_CFG_IN_GPIO5V_VTRIP_SEL3_1_Msk (0x8UL)         /*!< GPIO_PRT CFG_IN_GPIO5V: VTRIP_SEL3_1 (Bitfield-Mask: 0x01)*/
#define GPIO_PRT_CFG_IN_GPIO5V_VTRIP_SEL4_1_Pos (4UL)           /*!< GPIO_PRT CFG_IN_GPIO5V: VTRIP_SEL4_1 (Bit 4)              */
#define GPIO_PRT_CFG_IN_GPIO5V_VTRIP_SEL4_1_Msk (0x10UL)        /*!< GPIO_PRT CFG_IN_GPIO5V: VTRIP_SEL4_1 (Bitfield-Mask: 0x01)*/
#define GPIO_PRT_CFG_IN_GPIO5V_VTRIP_SEL5_1_Pos (5UL)           /*!< GPIO_PRT CFG_IN_GPIO5V: VTRIP_SEL5_1 (Bit 5)              */
#define GPIO_PRT_CFG_IN_GPIO5V_VTRIP_SEL5_1_Msk (0x20UL)        /*!< GPIO_PRT CFG_IN_GPIO5V: VTRIP_SEL5_1 (Bitfield-Mask: 0x01)*/
#define GPIO_PRT_CFG_IN_GPIO5V_VTRIP_SEL6_1_Pos (6UL)           /*!< GPIO_PRT CFG_IN_GPIO5V: VTRIP_SEL6_1 (Bit 6)              */
#define GPIO_PRT_CFG_IN_GPIO5V_VTRIP_SEL6_1_Msk (0x40UL)        /*!< GPIO_PRT CFG_IN_GPIO5V: VTRIP_SEL6_1 (Bitfield-Mask: 0x01)*/
#define GPIO_PRT_CFG_IN_GPIO5V_VTRIP_SEL7_1_Pos (7UL)           /*!< GPIO_PRT CFG_IN_GPIO5V: VTRIP_SEL7_1 (Bit 7)              */
#define GPIO_PRT_CFG_IN_GPIO5V_VTRIP_SEL7_1_Msk (0x80UL)        /*!< GPIO_PRT CFG_IN_GPIO5V: VTRIP_SEL7_1 (Bitfield-Mask: 0x01)*/


/* ===================================================  GPIO.INTR_CAUSE0  ==================================================== */
#define GPIO_INTR_CAUSE0_PORT_INT_Pos     (0UL)                 /*!< GPIO INTR_CAUSE0: PORT_INT (Bit 0)                        */
#define GPIO_INTR_CAUSE0_PORT_INT_Msk     (0xffffffffUL)        /*!< GPIO INTR_CAUSE0: PORT_INT (Bitfield-Mask: 0xffffffff)    */
/* ===================================================  GPIO.INTR_CAUSE1  ==================================================== */
#define GPIO_INTR_CAUSE1_PORT_INT_Pos     (0UL)                 /*!< GPIO INTR_CAUSE1: PORT_INT (Bit 0)                        */
#define GPIO_INTR_CAUSE1_PORT_INT_Msk     (0xffffffffUL)        /*!< GPIO INTR_CAUSE1: PORT_INT (Bitfield-Mask: 0xffffffff)    */
/* ===================================================  GPIO.INTR_CAUSE2  ==================================================== */
#define GPIO_INTR_CAUSE2_PORT_INT_Pos     (0UL)                 /*!< GPIO INTR_CAUSE2: PORT_INT (Bit 0)                        */
#define GPIO_INTR_CAUSE2_PORT_INT_Msk     (0xffffffffUL)        /*!< GPIO INTR_CAUSE2: PORT_INT (Bitfield-Mask: 0xffffffff)    */
/* ===================================================  GPIO.INTR_CAUSE3  ==================================================== */
#define GPIO_INTR_CAUSE3_PORT_INT_Pos     (0UL)                 /*!< GPIO INTR_CAUSE3: PORT_INT (Bit 0)                        */
#define GPIO_INTR_CAUSE3_PORT_INT_Msk     (0xffffffffUL)        /*!< GPIO INTR_CAUSE3: PORT_INT (Bitfield-Mask: 0xffffffff)    */
/* ====================================================  GPIO.VDD_ACTIVE  ==================================================== */
#define GPIO_VDD_ACTIVE_VDDIO_ACTIVE_Pos  (0UL)                 /*!< GPIO VDD_ACTIVE: VDDIO_ACTIVE (Bit 0)                     */
#define GPIO_VDD_ACTIVE_VDDIO_ACTIVE_Msk  (0xffffUL)            /*!< GPIO VDD_ACTIVE: VDDIO_ACTIVE (Bitfield-Mask: 0xffff)     */
#define GPIO_VDD_ACTIVE_VDDA_ACTIVE_Pos   (30UL)                /*!< GPIO VDD_ACTIVE: VDDA_ACTIVE (Bit 30)                     */
#define GPIO_VDD_ACTIVE_VDDA_ACTIVE_Msk   (0x40000000UL)        /*!< GPIO VDD_ACTIVE: VDDA_ACTIVE (Bitfield-Mask: 0x01)        */
#define GPIO_VDD_ACTIVE_VDDD_ACTIVE_Pos   (31UL)                /*!< GPIO VDD_ACTIVE: VDDD_ACTIVE (Bit 31)                     */
#define GPIO_VDD_ACTIVE_VDDD_ACTIVE_Msk   (0x80000000UL)        /*!< GPIO VDD_ACTIVE: VDDD_ACTIVE (Bitfield-Mask: 0x01)        */
/* =====================================================  GPIO.VDD_INTR  ===================================================== */
#define GPIO_VDD_INTR_VDDIO_ACTIVE_Pos    (0UL)                 /*!< GPIO VDD_INTR: VDDIO_ACTIVE (Bit 0)                       */
#define GPIO_VDD_INTR_VDDIO_ACTIVE_Msk    (0xffffUL)            /*!< GPIO VDD_INTR: VDDIO_ACTIVE (Bitfield-Mask: 0xffff)       */
#define GPIO_VDD_INTR_VDDA_ACTIVE_Pos     (30UL)                /*!< GPIO VDD_INTR: VDDA_ACTIVE (Bit 30)                       */
#define GPIO_VDD_INTR_VDDA_ACTIVE_Msk     (0x40000000UL)        /*!< GPIO VDD_INTR: VDDA_ACTIVE (Bitfield-Mask: 0x01)          */
#define GPIO_VDD_INTR_VDDD_ACTIVE_Pos     (31UL)                /*!< GPIO VDD_INTR: VDDD_ACTIVE (Bit 31)                       */
#define GPIO_VDD_INTR_VDDD_ACTIVE_Msk     (0x80000000UL)        /*!< GPIO VDD_INTR: VDDD_ACTIVE (Bitfield-Mask: 0x01)          */
/* ==================================================  GPIO.VDD_INTR_MASK  =================================================== */
#define GPIO_VDD_INTR_MASK_VDDIO_ACTIVE_Pos (0UL)               /*!< GPIO VDD_INTR_MASK: VDDIO_ACTIVE (Bit 0)                  */
#define GPIO_VDD_INTR_MASK_VDDIO_ACTIVE_Msk (0xffffUL)          /*!< GPIO VDD_INTR_MASK: VDDIO_ACTIVE (Bitfield-Mask: 0xffff)  */
#define GPIO_VDD_INTR_MASK_VDDA_ACTIVE_Pos (30UL)               /*!< GPIO VDD_INTR_MASK: VDDA_ACTIVE (Bit 30)                  */
#define GPIO_VDD_INTR_MASK_VDDA_ACTIVE_Msk (0x40000000UL)       /*!< GPIO VDD_INTR_MASK: VDDA_ACTIVE (Bitfield-Mask: 0x01)     */
#define GPIO_VDD_INTR_MASK_VDDD_ACTIVE_Pos (31UL)               /*!< GPIO VDD_INTR_MASK: VDDD_ACTIVE (Bit 31)                  */
#define GPIO_VDD_INTR_MASK_VDDD_ACTIVE_Msk (0x80000000UL)       /*!< GPIO VDD_INTR_MASK: VDDD_ACTIVE (Bitfield-Mask: 0x01)     */
/* =================================================  GPIO.VDD_INTR_MASKED  ================================================== */
#define GPIO_VDD_INTR_MASKED_VDDIO_ACTIVE_Pos (0UL)             /*!< GPIO VDD_INTR_MASKED: VDDIO_ACTIVE (Bit 0)                */
#define GPIO_VDD_INTR_MASKED_VDDIO_ACTIVE_Msk (0xffffUL)        /*!< GPIO VDD_INTR_MASKED: VDDIO_ACTIVE (Bitfield-Mask: 0xffff)*/
#define GPIO_VDD_INTR_MASKED_VDDA_ACTIVE_Pos (30UL)             /*!< GPIO VDD_INTR_MASKED: VDDA_ACTIVE (Bit 30)                */
#define GPIO_VDD_INTR_MASKED_VDDA_ACTIVE_Msk (0x40000000UL)     /*!< GPIO VDD_INTR_MASKED: VDDA_ACTIVE (Bitfield-Mask: 0x01)   */
#define GPIO_VDD_INTR_MASKED_VDDD_ACTIVE_Pos (31UL)             /*!< GPIO VDD_INTR_MASKED: VDDD_ACTIVE (Bit 31)                */
#define GPIO_VDD_INTR_MASKED_VDDD_ACTIVE_Msk (0x80000000UL)     /*!< GPIO VDD_INTR_MASKED: VDDD_ACTIVE (Bitfield-Mask: 0x01)   */
/* ===================================================  GPIO.VDD_INTR_SET  =================================================== */
#define GPIO_VDD_INTR_SET_VDDIO_ACTIVE_Pos (0UL)                /*!< GPIO VDD_INTR_SET: VDDIO_ACTIVE (Bit 0)                   */
#define GPIO_VDD_INTR_SET_VDDIO_ACTIVE_Msk (0xffffUL)           /*!< GPIO VDD_INTR_SET: VDDIO_ACTIVE (Bitfield-Mask: 0xffff)   */
#define GPIO_VDD_INTR_SET_VDDA_ACTIVE_Pos (30UL)                /*!< GPIO VDD_INTR_SET: VDDA_ACTIVE (Bit 30)                   */
#define GPIO_VDD_INTR_SET_VDDA_ACTIVE_Msk (0x40000000UL)        /*!< GPIO VDD_INTR_SET: VDDA_ACTIVE (Bitfield-Mask: 0x01)      */
#define GPIO_VDD_INTR_SET_VDDD_ACTIVE_Pos (31UL)                /*!< GPIO VDD_INTR_SET: VDDD_ACTIVE (Bit 31)                   */
#define GPIO_VDD_INTR_SET_VDDD_ACTIVE_Msk (0x80000000UL)        /*!< GPIO VDD_INTR_SET: VDDD_ACTIVE (Bitfield-Mask: 0x01)      */


#endif /* _CYIP_GPIO_H_ */


/* [] END OF FILE */
