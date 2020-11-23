/* ---------------------------------------------------------------------------- */
/*                Microchip Microcontroller Software Support                    */
/*                       SAM Software Package License                           */
/* ---------------------------------------------------------------------------- */
/* Copyright (c) %copyright_year%, Microchip Technology Inc.                    */
/*                                                                              */
/* All rights reserved.                                                         */
/*                                                                              */
/* Redistribution and use in source and binary forms, with or without           */
/* modification, are permitted provided that the following condition is met:    */
/*                                                                              */
/* - Redistributions of source code must retain the above copyright notice,     */
/* this list of conditions and the disclaimer below.                            */
/*                                                                              */
/* Microchip's name may not be used to endorse or promote products derived from */
/* this software without specific prior written permission.                     */
/*                                                                              */
/* DISCLAIMER:  THIS SOFTWARE IS PROVIDED BY MICROCHIP "AS IS" AND ANY EXPRESS  */
/* OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES */
/* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT    */
/* ARE DISCLAIMED. IN NO EVENT SHALL MICROCHIP BE LIABLE FOR ANY DIRECT,        */
/* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES           */
/* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; */
/* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND  */
/* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT   */
/* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF     */
/* THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.            */
/* ---------------------------------------------------------------------------- */

#ifndef _PIC32CX_MATRIX1_INSTANCE_
#define _PIC32CX_MATRIX1_INSTANCE_

/* ========== Register definition for MATRIX1 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_MATRIX1_MCFG                    (0x40044000U) /**< \brief (MATRIX1) Master Configuration Register */
  #define REG_MATRIX1_SCFG                    (0x40044040U) /**< \brief (MATRIX1) Slave Configuration Register */
  #define REG_MATRIX1_PRAS0                   (0x40044080U) /**< \brief (MATRIX1) Priority Register A for Slave 0 */
  #define REG_MATRIX1_PRBS0                   (0x40044084U) /**< \brief (MATRIX1) Priority Register B for Slave 0 */
  #define REG_MATRIX1_PRAS1                   (0x40044088U) /**< \brief (MATRIX1) Priority Register A for Slave 1 */
  #define REG_MATRIX1_PRBS1                   (0x4004408CU) /**< \brief (MATRIX1) Priority Register B for Slave 1 */
  #define REG_MATRIX1_PRAS2                   (0x40044090U) /**< \brief (MATRIX1) Priority Register A for Slave 2 */
  #define REG_MATRIX1_PRBS2                   (0x40044094U) /**< \brief (MATRIX1) Priority Register B for Slave 2 */
  #define REG_MATRIX1_PRAS3                   (0x40044098U) /**< \brief (MATRIX1) Priority Register A for Slave 3 */
  #define REG_MATRIX1_PRBS3                   (0x4004409CU) /**< \brief (MATRIX1) Priority Register B for Slave 3 */
  #define REG_MATRIX1_PRAS4                   (0x400440A0U) /**< \brief (MATRIX1) Priority Register A for Slave 4 */
  #define REG_MATRIX1_PRBS4                   (0x400440A4U) /**< \brief (MATRIX1) Priority Register B for Slave 4 */
  #define REG_MATRIX1_PRAS5                   (0x400440A8U) /**< \brief (MATRIX1) Priority Register A for Slave 5 */
  #define REG_MATRIX1_PRBS5                   (0x400440ACU) /**< \brief (MATRIX1) Priority Register B for Slave 5 */
  #define REG_MATRIX1_PRAS6                   (0x400440B0U) /**< \brief (MATRIX1) Priority Register A for Slave 6 */
  #define REG_MATRIX1_PRBS6                   (0x400440B4U) /**< \brief (MATRIX1) Priority Register B for Slave 6 */
  #define REG_MATRIX1_PRAS7                   (0x400440B8U) /**< \brief (MATRIX1) Priority Register A for Slave 7 */
  #define REG_MATRIX1_PRBS7                   (0x400440BCU) /**< \brief (MATRIX1) Priority Register B for Slave 7 */
  #define REG_MATRIX1_PRAS8                   (0x400440C0U) /**< \brief (MATRIX1) Priority Register A for Slave 8 */
  #define REG_MATRIX1_PRBS8                   (0x400440C4U) /**< \brief (MATRIX1) Priority Register B for Slave 8 */
  #define REG_MATRIX1_PRAS9                   (0x400440C8U) /**< \brief (MATRIX1) Priority Register A for Slave 9 */
  #define REG_MATRIX1_PRBS9                   (0x400440CCU) /**< \brief (MATRIX1) Priority Register B for Slave 9 */
  #define REG_MATRIX1_PRAS10                  (0x400440D0U) /**< \brief (MATRIX1) Priority Register A for Slave 10 */
  #define REG_MATRIX1_PRBS10                  (0x400440D4U) /**< \brief (MATRIX1) Priority Register B for Slave 10 */
  #define REG_MATRIX1_PRAS11                  (0x400440D8U) /**< \brief (MATRIX1) Priority Register A for Slave 11 */
  #define REG_MATRIX1_PRBS11                  (0x400440DCU) /**< \brief (MATRIX1) Priority Register B for Slave 11 */
  #define REG_MATRIX1_PRAS12                  (0x400440E0U) /**< \brief (MATRIX1) Priority Register A for Slave 12 */
  #define REG_MATRIX1_PRBS12                  (0x400440E4U) /**< \brief (MATRIX1) Priority Register B for Slave 12 */
  #define REG_MATRIX1_PRAS13                  (0x400440E8U) /**< \brief (MATRIX1) Priority Register A for Slave 13 */
  #define REG_MATRIX1_PRBS13                  (0x400440ECU) /**< \brief (MATRIX1) Priority Register B for Slave 13 */
  #define REG_MATRIX1_PRAS14                  (0x400440F0U) /**< \brief (MATRIX1) Priority Register A for Slave 14 */
  #define REG_MATRIX1_PRBS14                  (0x400440F4U) /**< \brief (MATRIX1) Priority Register B for Slave 14 */
  #define REG_MATRIX1_PRAS15                  (0x400440F8U) /**< \brief (MATRIX1) Priority Register A for Slave 15 */
  #define REG_MATRIX1_PRBS15                  (0x400440FCU) /**< \brief (MATRIX1) Priority Register B for Slave 15 */
  #define REG_MATRIX1_MRCR                    (0x40044100U) /**< \brief (MATRIX1) Master Remap Control Register */
  #define REG_MATRIX1_MEIER                   (0x40044150U) /**< \brief (MATRIX1) Master Error Interrupt Enable Register */
  #define REG_MATRIX1_MEIDR                   (0x40044154U) /**< \brief (MATRIX1) Master Error Interrupt Disable Register */
  #define REG_MATRIX1_MEIMR                   (0x40044158U) /**< \brief (MATRIX1) Master Error Interrupt Mask Register */
  #define REG_MATRIX1_MESR                    (0x4004415CU) /**< \brief (MATRIX1) Master Error Status Register */
  #define REG_MATRIX1_MEAR                    (0x40044160U) /**< \brief (MATRIX1) Master 0 Error Address Register */
  #define REG_MATRIX1_WPMR                    (0x400441E4U) /**< \brief (MATRIX1) Write Protect Mode Register */
  #define REG_MATRIX1_WPSR                    (0x400441E8U) /**< \brief (MATRIX1) Write Protect Status Register */
  #define REG_MATRIX1_PSR                     (0x40044200U) /**< \brief (MATRIX1) Protection Slave 0 Register */
  #define REG_MATRIX1_PASSR                   (0x40044240U) /**< \brief (MATRIX1) Protected Areas Split Slave 0 Register */
  #define REG_MATRIX1_PRTSR                   (0x40044280U) /**< \brief (MATRIX1) Protected Region Top Slave 0 Register */
  #define REG_MATRIX1_PPSELR                  (0x400442C0U) /**< \brief (MATRIX1) Protected Peripheral Select 1 Register */
#else
  #define REG_MATRIX1_MCFG   (*(__IO uint32_t*)0x40044000U) /**< \brief (MATRIX1) Master Configuration Register */
  #define REG_MATRIX1_SCFG   (*(__IO uint32_t*)0x40044040U) /**< \brief (MATRIX1) Slave Configuration Register */
  #define REG_MATRIX1_PRAS0  (*(__IO uint32_t*)0x40044080U) /**< \brief (MATRIX1) Priority Register A for Slave 0 */
  #define REG_MATRIX1_PRBS0  (*(__IO uint32_t*)0x40044084U) /**< \brief (MATRIX1) Priority Register B for Slave 0 */
  #define REG_MATRIX1_PRAS1  (*(__IO uint32_t*)0x40044088U) /**< \brief (MATRIX1) Priority Register A for Slave 1 */
  #define REG_MATRIX1_PRBS1  (*(__IO uint32_t*)0x4004408CU) /**< \brief (MATRIX1) Priority Register B for Slave 1 */
  #define REG_MATRIX1_PRAS2  (*(__IO uint32_t*)0x40044090U) /**< \brief (MATRIX1) Priority Register A for Slave 2 */
  #define REG_MATRIX1_PRBS2  (*(__IO uint32_t*)0x40044094U) /**< \brief (MATRIX1) Priority Register B for Slave 2 */
  #define REG_MATRIX1_PRAS3  (*(__IO uint32_t*)0x40044098U) /**< \brief (MATRIX1) Priority Register A for Slave 3 */
  #define REG_MATRIX1_PRBS3  (*(__IO uint32_t*)0x4004409CU) /**< \brief (MATRIX1) Priority Register B for Slave 3 */
  #define REG_MATRIX1_PRAS4  (*(__IO uint32_t*)0x400440A0U) /**< \brief (MATRIX1) Priority Register A for Slave 4 */
  #define REG_MATRIX1_PRBS4  (*(__IO uint32_t*)0x400440A4U) /**< \brief (MATRIX1) Priority Register B for Slave 4 */
  #define REG_MATRIX1_PRAS5  (*(__IO uint32_t*)0x400440A8U) /**< \brief (MATRIX1) Priority Register A for Slave 5 */
  #define REG_MATRIX1_PRBS5  (*(__IO uint32_t*)0x400440ACU) /**< \brief (MATRIX1) Priority Register B for Slave 5 */
  #define REG_MATRIX1_PRAS6  (*(__IO uint32_t*)0x400440B0U) /**< \brief (MATRIX1) Priority Register A for Slave 6 */
  #define REG_MATRIX1_PRBS6  (*(__IO uint32_t*)0x400440B4U) /**< \brief (MATRIX1) Priority Register B for Slave 6 */
  #define REG_MATRIX1_PRAS7  (*(__IO uint32_t*)0x400440B8U) /**< \brief (MATRIX1) Priority Register A for Slave 7 */
  #define REG_MATRIX1_PRBS7  (*(__IO uint32_t*)0x400440BCU) /**< \brief (MATRIX1) Priority Register B for Slave 7 */
  #define REG_MATRIX1_PRAS8  (*(__IO uint32_t*)0x400440C0U) /**< \brief (MATRIX1) Priority Register A for Slave 8 */
  #define REG_MATRIX1_PRBS8  (*(__IO uint32_t*)0x400440C4U) /**< \brief (MATRIX1) Priority Register B for Slave 8 */
  #define REG_MATRIX1_PRAS9  (*(__IO uint32_t*)0x400440C8U) /**< \brief (MATRIX1) Priority Register A for Slave 9 */
  #define REG_MATRIX1_PRBS9  (*(__IO uint32_t*)0x400440CCU) /**< \brief (MATRIX1) Priority Register B for Slave 9 */
  #define REG_MATRIX1_PRAS10 (*(__IO uint32_t*)0x400440D0U) /**< \brief (MATRIX1) Priority Register A for Slave 10 */
  #define REG_MATRIX1_PRBS10 (*(__IO uint32_t*)0x400440D4U) /**< \brief (MATRIX1) Priority Register B for Slave 10 */
  #define REG_MATRIX1_PRAS11 (*(__IO uint32_t*)0x400440D8U) /**< \brief (MATRIX1) Priority Register A for Slave 11 */
  #define REG_MATRIX1_PRBS11 (*(__IO uint32_t*)0x400440DCU) /**< \brief (MATRIX1) Priority Register B for Slave 11 */
  #define REG_MATRIX1_PRAS12 (*(__IO uint32_t*)0x400440E0U) /**< \brief (MATRIX1) Priority Register A for Slave 12 */
  #define REG_MATRIX1_PRBS12 (*(__IO uint32_t*)0x400440E4U) /**< \brief (MATRIX1) Priority Register B for Slave 12 */
  #define REG_MATRIX1_PRAS13 (*(__IO uint32_t*)0x400440E8U) /**< \brief (MATRIX1) Priority Register A for Slave 13 */
  #define REG_MATRIX1_PRBS13 (*(__IO uint32_t*)0x400440ECU) /**< \brief (MATRIX1) Priority Register B for Slave 13 */
  #define REG_MATRIX1_PRAS14 (*(__IO uint32_t*)0x400440F0U) /**< \brief (MATRIX1) Priority Register A for Slave 14 */
  #define REG_MATRIX1_PRBS14 (*(__IO uint32_t*)0x400440F4U) /**< \brief (MATRIX1) Priority Register B for Slave 14 */
  #define REG_MATRIX1_PRAS15 (*(__IO uint32_t*)0x400440F8U) /**< \brief (MATRIX1) Priority Register A for Slave 15 */
  #define REG_MATRIX1_PRBS15 (*(__IO uint32_t*)0x400440FCU) /**< \brief (MATRIX1) Priority Register B for Slave 15 */
  #define REG_MATRIX1_MRCR   (*(__IO uint32_t*)0x40044100U) /**< \brief (MATRIX1) Master Remap Control Register */
  #define REG_MATRIX1_MEIER  (*(__O  uint32_t*)0x40044150U) /**< \brief (MATRIX1) Master Error Interrupt Enable Register */
  #define REG_MATRIX1_MEIDR  (*(__O  uint32_t*)0x40044154U) /**< \brief (MATRIX1) Master Error Interrupt Disable Register */
  #define REG_MATRIX1_MEIMR  (*(__I  uint32_t*)0x40044158U) /**< \brief (MATRIX1) Master Error Interrupt Mask Register */
  #define REG_MATRIX1_MESR   (*(__I  uint32_t*)0x4004415CU) /**< \brief (MATRIX1) Master Error Status Register */
  #define REG_MATRIX1_MEAR   (*(__I  uint32_t*)0x40044160U) /**< \brief (MATRIX1) Master 0 Error Address Register */
  #define REG_MATRIX1_WPMR   (*(__IO uint32_t*)0x400441E4U) /**< \brief (MATRIX1) Write Protect Mode Register */
  #define REG_MATRIX1_WPSR   (*(__I  uint32_t*)0x400441E8U) /**< \brief (MATRIX1) Write Protect Status Register */
  #define REG_MATRIX1_PSR    (*(__IO uint32_t*)0x40044200U) /**< \brief (MATRIX1) Protection Slave 0 Register */
  #define REG_MATRIX1_PASSR  (*(__IO uint32_t*)0x40044240U) /**< \brief (MATRIX1) Protected Areas Split Slave 0 Register */
  #define REG_MATRIX1_PRTSR  (*(__IO uint32_t*)0x40044280U) /**< \brief (MATRIX1) Protected Region Top Slave 0 Register */
  #define REG_MATRIX1_PPSELR (*(__IO uint32_t*)0x400442C0U) /**< \brief (MATRIX1) Protected Peripheral Select 1 Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_MATRIX1_INSTANCE_ */
