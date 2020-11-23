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

#ifndef _PIC32CX_MATRIX2_INSTANCE_
#define _PIC32CX_MATRIX2_INSTANCE_

/* ========== Register definition for MATRIX2 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_MATRIX2_MCFG                    (0x4A000000U) /**< \brief (MATRIX2) Master Configuration Register */
  #define REG_MATRIX2_SCFG                    (0x4A000040U) /**< \brief (MATRIX2) Slave Configuration Register */
  #define REG_MATRIX2_PRAS0                   (0x4A000080U) /**< \brief (MATRIX2) Priority Register A for Slave 0 */
  #define REG_MATRIX2_PRBS0                   (0x4A000084U) /**< \brief (MATRIX2) Priority Register B for Slave 0 */
  #define REG_MATRIX2_PRAS1                   (0x4A000088U) /**< \brief (MATRIX2) Priority Register A for Slave 1 */
  #define REG_MATRIX2_PRBS1                   (0x4A00008CU) /**< \brief (MATRIX2) Priority Register B for Slave 1 */
  #define REG_MATRIX2_PRAS2                   (0x4A000090U) /**< \brief (MATRIX2) Priority Register A for Slave 2 */
  #define REG_MATRIX2_PRBS2                   (0x4A000094U) /**< \brief (MATRIX2) Priority Register B for Slave 2 */
  #define REG_MATRIX2_PRAS3                   (0x4A000098U) /**< \brief (MATRIX2) Priority Register A for Slave 3 */
  #define REG_MATRIX2_PRBS3                   (0x4A00009CU) /**< \brief (MATRIX2) Priority Register B for Slave 3 */
  #define REG_MATRIX2_PRAS4                   (0x4A0000A0U) /**< \brief (MATRIX2) Priority Register A for Slave 4 */
  #define REG_MATRIX2_PRBS4                   (0x4A0000A4U) /**< \brief (MATRIX2) Priority Register B for Slave 4 */
  #define REG_MATRIX2_PRAS5                   (0x4A0000A8U) /**< \brief (MATRIX2) Priority Register A for Slave 5 */
  #define REG_MATRIX2_PRBS5                   (0x4A0000ACU) /**< \brief (MATRIX2) Priority Register B for Slave 5 */
  #define REG_MATRIX2_PRAS6                   (0x4A0000B0U) /**< \brief (MATRIX2) Priority Register A for Slave 6 */
  #define REG_MATRIX2_PRBS6                   (0x4A0000B4U) /**< \brief (MATRIX2) Priority Register B for Slave 6 */
  #define REG_MATRIX2_PRAS7                   (0x4A0000B8U) /**< \brief (MATRIX2) Priority Register A for Slave 7 */
  #define REG_MATRIX2_PRBS7                   (0x4A0000BCU) /**< \brief (MATRIX2) Priority Register B for Slave 7 */
  #define REG_MATRIX2_PRAS8                   (0x4A0000C0U) /**< \brief (MATRIX2) Priority Register A for Slave 8 */
  #define REG_MATRIX2_PRBS8                   (0x4A0000C4U) /**< \brief (MATRIX2) Priority Register B for Slave 8 */
  #define REG_MATRIX2_PRAS9                   (0x4A0000C8U) /**< \brief (MATRIX2) Priority Register A for Slave 9 */
  #define REG_MATRIX2_PRBS9                   (0x4A0000CCU) /**< \brief (MATRIX2) Priority Register B for Slave 9 */
  #define REG_MATRIX2_PRAS10                  (0x4A0000D0U) /**< \brief (MATRIX2) Priority Register A for Slave 10 */
  #define REG_MATRIX2_PRBS10                  (0x4A0000D4U) /**< \brief (MATRIX2) Priority Register B for Slave 10 */
  #define REG_MATRIX2_PRAS11                  (0x4A0000D8U) /**< \brief (MATRIX2) Priority Register A for Slave 11 */
  #define REG_MATRIX2_PRBS11                  (0x4A0000DCU) /**< \brief (MATRIX2) Priority Register B for Slave 11 */
  #define REG_MATRIX2_PRAS12                  (0x4A0000E0U) /**< \brief (MATRIX2) Priority Register A for Slave 12 */
  #define REG_MATRIX2_PRBS12                  (0x4A0000E4U) /**< \brief (MATRIX2) Priority Register B for Slave 12 */
  #define REG_MATRIX2_PRAS13                  (0x4A0000E8U) /**< \brief (MATRIX2) Priority Register A for Slave 13 */
  #define REG_MATRIX2_PRBS13                  (0x4A0000ECU) /**< \brief (MATRIX2) Priority Register B for Slave 13 */
  #define REG_MATRIX2_PRAS14                  (0x4A0000F0U) /**< \brief (MATRIX2) Priority Register A for Slave 14 */
  #define REG_MATRIX2_PRBS14                  (0x4A0000F4U) /**< \brief (MATRIX2) Priority Register B for Slave 14 */
  #define REG_MATRIX2_PRAS15                  (0x4A0000F8U) /**< \brief (MATRIX2) Priority Register A for Slave 15 */
  #define REG_MATRIX2_PRBS15                  (0x4A0000FCU) /**< \brief (MATRIX2) Priority Register B for Slave 15 */
  #define REG_MATRIX2_MRCR                    (0x4A000100U) /**< \brief (MATRIX2) Master Remap Control Register */
  #define REG_MATRIX2_MEIER                   (0x4A000150U) /**< \brief (MATRIX2) Master Error Interrupt Enable Register */
  #define REG_MATRIX2_MEIDR                   (0x4A000154U) /**< \brief (MATRIX2) Master Error Interrupt Disable Register */
  #define REG_MATRIX2_MEIMR                   (0x4A000158U) /**< \brief (MATRIX2) Master Error Interrupt Mask Register */
  #define REG_MATRIX2_MESR                    (0x4A00015CU) /**< \brief (MATRIX2) Master Error Status Register */
  #define REG_MATRIX2_MEAR                    (0x4A000160U) /**< \brief (MATRIX2) Master 0 Error Address Register */
  #define REG_MATRIX2_WPMR                    (0x4A0001E4U) /**< \brief (MATRIX2) Write Protect Mode Register */
  #define REG_MATRIX2_WPSR                    (0x4A0001E8U) /**< \brief (MATRIX2) Write Protect Status Register */
  #define REG_MATRIX2_PSR                     (0x4A000200U) /**< \brief (MATRIX2) Protection Slave 0 Register */
  #define REG_MATRIX2_PASSR                   (0x4A000240U) /**< \brief (MATRIX2) Protected Areas Split Slave 0 Register */
  #define REG_MATRIX2_PRTSR                   (0x4A000280U) /**< \brief (MATRIX2) Protected Region Top Slave 0 Register */
  #define REG_MATRIX2_PPSELR                  (0x4A0002C0U) /**< \brief (MATRIX2) Protected Peripheral Select 1 Register */
#else
  #define REG_MATRIX2_MCFG   (*(__IO uint32_t*)0x4A000000U) /**< \brief (MATRIX2) Master Configuration Register */
  #define REG_MATRIX2_SCFG   (*(__IO uint32_t*)0x4A000040U) /**< \brief (MATRIX2) Slave Configuration Register */
  #define REG_MATRIX2_PRAS0  (*(__IO uint32_t*)0x4A000080U) /**< \brief (MATRIX2) Priority Register A for Slave 0 */
  #define REG_MATRIX2_PRBS0  (*(__IO uint32_t*)0x4A000084U) /**< \brief (MATRIX2) Priority Register B for Slave 0 */
  #define REG_MATRIX2_PRAS1  (*(__IO uint32_t*)0x4A000088U) /**< \brief (MATRIX2) Priority Register A for Slave 1 */
  #define REG_MATRIX2_PRBS1  (*(__IO uint32_t*)0x4A00008CU) /**< \brief (MATRIX2) Priority Register B for Slave 1 */
  #define REG_MATRIX2_PRAS2  (*(__IO uint32_t*)0x4A000090U) /**< \brief (MATRIX2) Priority Register A for Slave 2 */
  #define REG_MATRIX2_PRBS2  (*(__IO uint32_t*)0x4A000094U) /**< \brief (MATRIX2) Priority Register B for Slave 2 */
  #define REG_MATRIX2_PRAS3  (*(__IO uint32_t*)0x4A000098U) /**< \brief (MATRIX2) Priority Register A for Slave 3 */
  #define REG_MATRIX2_PRBS3  (*(__IO uint32_t*)0x4A00009CU) /**< \brief (MATRIX2) Priority Register B for Slave 3 */
  #define REG_MATRIX2_PRAS4  (*(__IO uint32_t*)0x4A0000A0U) /**< \brief (MATRIX2) Priority Register A for Slave 4 */
  #define REG_MATRIX2_PRBS4  (*(__IO uint32_t*)0x4A0000A4U) /**< \brief (MATRIX2) Priority Register B for Slave 4 */
  #define REG_MATRIX2_PRAS5  (*(__IO uint32_t*)0x4A0000A8U) /**< \brief (MATRIX2) Priority Register A for Slave 5 */
  #define REG_MATRIX2_PRBS5  (*(__IO uint32_t*)0x4A0000ACU) /**< \brief (MATRIX2) Priority Register B for Slave 5 */
  #define REG_MATRIX2_PRAS6  (*(__IO uint32_t*)0x4A0000B0U) /**< \brief (MATRIX2) Priority Register A for Slave 6 */
  #define REG_MATRIX2_PRBS6  (*(__IO uint32_t*)0x4A0000B4U) /**< \brief (MATRIX2) Priority Register B for Slave 6 */
  #define REG_MATRIX2_PRAS7  (*(__IO uint32_t*)0x4A0000B8U) /**< \brief (MATRIX2) Priority Register A for Slave 7 */
  #define REG_MATRIX2_PRBS7  (*(__IO uint32_t*)0x4A0000BCU) /**< \brief (MATRIX2) Priority Register B for Slave 7 */
  #define REG_MATRIX2_PRAS8  (*(__IO uint32_t*)0x4A0000C0U) /**< \brief (MATRIX2) Priority Register A for Slave 8 */
  #define REG_MATRIX2_PRBS8  (*(__IO uint32_t*)0x4A0000C4U) /**< \brief (MATRIX2) Priority Register B for Slave 8 */
  #define REG_MATRIX2_PRAS9  (*(__IO uint32_t*)0x4A0000C8U) /**< \brief (MATRIX2) Priority Register A for Slave 9 */
  #define REG_MATRIX2_PRBS9  (*(__IO uint32_t*)0x4A0000CCU) /**< \brief (MATRIX2) Priority Register B for Slave 9 */
  #define REG_MATRIX2_PRAS10 (*(__IO uint32_t*)0x4A0000D0U) /**< \brief (MATRIX2) Priority Register A for Slave 10 */
  #define REG_MATRIX2_PRBS10 (*(__IO uint32_t*)0x4A0000D4U) /**< \brief (MATRIX2) Priority Register B for Slave 10 */
  #define REG_MATRIX2_PRAS11 (*(__IO uint32_t*)0x4A0000D8U) /**< \brief (MATRIX2) Priority Register A for Slave 11 */
  #define REG_MATRIX2_PRBS11 (*(__IO uint32_t*)0x4A0000DCU) /**< \brief (MATRIX2) Priority Register B for Slave 11 */
  #define REG_MATRIX2_PRAS12 (*(__IO uint32_t*)0x4A0000E0U) /**< \brief (MATRIX2) Priority Register A for Slave 12 */
  #define REG_MATRIX2_PRBS12 (*(__IO uint32_t*)0x4A0000E4U) /**< \brief (MATRIX2) Priority Register B for Slave 12 */
  #define REG_MATRIX2_PRAS13 (*(__IO uint32_t*)0x4A0000E8U) /**< \brief (MATRIX2) Priority Register A for Slave 13 */
  #define REG_MATRIX2_PRBS13 (*(__IO uint32_t*)0x4A0000ECU) /**< \brief (MATRIX2) Priority Register B for Slave 13 */
  #define REG_MATRIX2_PRAS14 (*(__IO uint32_t*)0x4A0000F0U) /**< \brief (MATRIX2) Priority Register A for Slave 14 */
  #define REG_MATRIX2_PRBS14 (*(__IO uint32_t*)0x4A0000F4U) /**< \brief (MATRIX2) Priority Register B for Slave 14 */
  #define REG_MATRIX2_PRAS15 (*(__IO uint32_t*)0x4A0000F8U) /**< \brief (MATRIX2) Priority Register A for Slave 15 */
  #define REG_MATRIX2_PRBS15 (*(__IO uint32_t*)0x4A0000FCU) /**< \brief (MATRIX2) Priority Register B for Slave 15 */
  #define REG_MATRIX2_MRCR   (*(__IO uint32_t*)0x4A000100U) /**< \brief (MATRIX2) Master Remap Control Register */
  #define REG_MATRIX2_MEIER  (*(__O  uint32_t*)0x4A000150U) /**< \brief (MATRIX2) Master Error Interrupt Enable Register */
  #define REG_MATRIX2_MEIDR  (*(__O  uint32_t*)0x4A000154U) /**< \brief (MATRIX2) Master Error Interrupt Disable Register */
  #define REG_MATRIX2_MEIMR  (*(__I  uint32_t*)0x4A000158U) /**< \brief (MATRIX2) Master Error Interrupt Mask Register */
  #define REG_MATRIX2_MESR   (*(__I  uint32_t*)0x4A00015CU) /**< \brief (MATRIX2) Master Error Status Register */
  #define REG_MATRIX2_MEAR   (*(__I  uint32_t*)0x4A000160U) /**< \brief (MATRIX2) Master 0 Error Address Register */
  #define REG_MATRIX2_WPMR   (*(__IO uint32_t*)0x4A0001E4U) /**< \brief (MATRIX2) Write Protect Mode Register */
  #define REG_MATRIX2_WPSR   (*(__I  uint32_t*)0x4A0001E8U) /**< \brief (MATRIX2) Write Protect Status Register */
  #define REG_MATRIX2_PSR    (*(__IO uint32_t*)0x4A000200U) /**< \brief (MATRIX2) Protection Slave 0 Register */
  #define REG_MATRIX2_PASSR  (*(__IO uint32_t*)0x4A000240U) /**< \brief (MATRIX2) Protected Areas Split Slave 0 Register */
  #define REG_MATRIX2_PRTSR  (*(__IO uint32_t*)0x4A000280U) /**< \brief (MATRIX2) Protected Region Top Slave 0 Register */
  #define REG_MATRIX2_PPSELR (*(__IO uint32_t*)0x4A0002C0U) /**< \brief (MATRIX2) Protected Peripheral Select 1 Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_MATRIX2_INSTANCE_ */
