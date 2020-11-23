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

#ifndef _PIC32CX_MATRIX3_INSTANCE_
#define _PIC32CX_MATRIX3_INSTANCE_

/* ========== Register definition for MATRIX3 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_MATRIX3_MCFG                    (0x48020000U) /**< \brief (MATRIX3) Master Configuration Register */
  #define REG_MATRIX3_SCFG                    (0x48020040U) /**< \brief (MATRIX3) Slave Configuration Register */
  #define REG_MATRIX3_PRAS0                   (0x48020080U) /**< \brief (MATRIX3) Priority Register A for Slave 0 */
  #define REG_MATRIX3_PRBS0                   (0x48020084U) /**< \brief (MATRIX3) Priority Register B for Slave 0 */
  #define REG_MATRIX3_PRAS1                   (0x48020088U) /**< \brief (MATRIX3) Priority Register A for Slave 1 */
  #define REG_MATRIX3_PRBS1                   (0x4802008CU) /**< \brief (MATRIX3) Priority Register B for Slave 1 */
  #define REG_MATRIX3_PRAS2                   (0x48020090U) /**< \brief (MATRIX3) Priority Register A for Slave 2 */
  #define REG_MATRIX3_PRBS2                   (0x48020094U) /**< \brief (MATRIX3) Priority Register B for Slave 2 */
  #define REG_MATRIX3_PRAS3                   (0x48020098U) /**< \brief (MATRIX3) Priority Register A for Slave 3 */
  #define REG_MATRIX3_PRBS3                   (0x4802009CU) /**< \brief (MATRIX3) Priority Register B for Slave 3 */
  #define REG_MATRIX3_PRAS4                   (0x480200A0U) /**< \brief (MATRIX3) Priority Register A for Slave 4 */
  #define REG_MATRIX3_PRBS4                   (0x480200A4U) /**< \brief (MATRIX3) Priority Register B for Slave 4 */
  #define REG_MATRIX3_PRAS5                   (0x480200A8U) /**< \brief (MATRIX3) Priority Register A for Slave 5 */
  #define REG_MATRIX3_PRBS5                   (0x480200ACU) /**< \brief (MATRIX3) Priority Register B for Slave 5 */
  #define REG_MATRIX3_PRAS6                   (0x480200B0U) /**< \brief (MATRIX3) Priority Register A for Slave 6 */
  #define REG_MATRIX3_PRBS6                   (0x480200B4U) /**< \brief (MATRIX3) Priority Register B for Slave 6 */
  #define REG_MATRIX3_PRAS7                   (0x480200B8U) /**< \brief (MATRIX3) Priority Register A for Slave 7 */
  #define REG_MATRIX3_PRBS7                   (0x480200BCU) /**< \brief (MATRIX3) Priority Register B for Slave 7 */
  #define REG_MATRIX3_PRAS8                   (0x480200C0U) /**< \brief (MATRIX3) Priority Register A for Slave 8 */
  #define REG_MATRIX3_PRBS8                   (0x480200C4U) /**< \brief (MATRIX3) Priority Register B for Slave 8 */
  #define REG_MATRIX3_PRAS9                   (0x480200C8U) /**< \brief (MATRIX3) Priority Register A for Slave 9 */
  #define REG_MATRIX3_PRBS9                   (0x480200CCU) /**< \brief (MATRIX3) Priority Register B for Slave 9 */
  #define REG_MATRIX3_PRAS10                  (0x480200D0U) /**< \brief (MATRIX3) Priority Register A for Slave 10 */
  #define REG_MATRIX3_PRBS10                  (0x480200D4U) /**< \brief (MATRIX3) Priority Register B for Slave 10 */
  #define REG_MATRIX3_PRAS11                  (0x480200D8U) /**< \brief (MATRIX3) Priority Register A for Slave 11 */
  #define REG_MATRIX3_PRBS11                  (0x480200DCU) /**< \brief (MATRIX3) Priority Register B for Slave 11 */
  #define REG_MATRIX3_PRAS12                  (0x480200E0U) /**< \brief (MATRIX3) Priority Register A for Slave 12 */
  #define REG_MATRIX3_PRBS12                  (0x480200E4U) /**< \brief (MATRIX3) Priority Register B for Slave 12 */
  #define REG_MATRIX3_PRAS13                  (0x480200E8U) /**< \brief (MATRIX3) Priority Register A for Slave 13 */
  #define REG_MATRIX3_PRBS13                  (0x480200ECU) /**< \brief (MATRIX3) Priority Register B for Slave 13 */
  #define REG_MATRIX3_PRAS14                  (0x480200F0U) /**< \brief (MATRIX3) Priority Register A for Slave 14 */
  #define REG_MATRIX3_PRBS14                  (0x480200F4U) /**< \brief (MATRIX3) Priority Register B for Slave 14 */
  #define REG_MATRIX3_PRAS15                  (0x480200F8U) /**< \brief (MATRIX3) Priority Register A for Slave 15 */
  #define REG_MATRIX3_PRBS15                  (0x480200FCU) /**< \brief (MATRIX3) Priority Register B for Slave 15 */
  #define REG_MATRIX3_MRCR                    (0x48020100U) /**< \brief (MATRIX3) Master Remap Control Register */
  #define REG_MATRIX3_MEIER                   (0x48020150U) /**< \brief (MATRIX3) Master Error Interrupt Enable Register */
  #define REG_MATRIX3_MEIDR                   (0x48020154U) /**< \brief (MATRIX3) Master Error Interrupt Disable Register */
  #define REG_MATRIX3_MEIMR                   (0x48020158U) /**< \brief (MATRIX3) Master Error Interrupt Mask Register */
  #define REG_MATRIX3_MESR                    (0x4802015CU) /**< \brief (MATRIX3) Master Error Status Register */
  #define REG_MATRIX3_MEAR                    (0x48020160U) /**< \brief (MATRIX3) Master 0 Error Address Register */
  #define REG_MATRIX3_WPMR                    (0x480201E4U) /**< \brief (MATRIX3) Write Protect Mode Register */
  #define REG_MATRIX3_WPSR                    (0x480201E8U) /**< \brief (MATRIX3) Write Protect Status Register */
  #define REG_MATRIX3_PSR                     (0x48020200U) /**< \brief (MATRIX3) Protection Slave 0 Register */
  #define REG_MATRIX3_PASSR                   (0x48020240U) /**< \brief (MATRIX3) Protected Areas Split Slave 0 Register */
  #define REG_MATRIX3_PRTSR                   (0x48020280U) /**< \brief (MATRIX3) Protected Region Top Slave 0 Register */
  #define REG_MATRIX3_PPSELR                  (0x480202C0U) /**< \brief (MATRIX3) Protected Peripheral Select 1 Register */
#else
  #define REG_MATRIX3_MCFG   (*(__IO uint32_t*)0x48020000U) /**< \brief (MATRIX3) Master Configuration Register */
  #define REG_MATRIX3_SCFG   (*(__IO uint32_t*)0x48020040U) /**< \brief (MATRIX3) Slave Configuration Register */
  #define REG_MATRIX3_PRAS0  (*(__IO uint32_t*)0x48020080U) /**< \brief (MATRIX3) Priority Register A for Slave 0 */
  #define REG_MATRIX3_PRBS0  (*(__IO uint32_t*)0x48020084U) /**< \brief (MATRIX3) Priority Register B for Slave 0 */
  #define REG_MATRIX3_PRAS1  (*(__IO uint32_t*)0x48020088U) /**< \brief (MATRIX3) Priority Register A for Slave 1 */
  #define REG_MATRIX3_PRBS1  (*(__IO uint32_t*)0x4802008CU) /**< \brief (MATRIX3) Priority Register B for Slave 1 */
  #define REG_MATRIX3_PRAS2  (*(__IO uint32_t*)0x48020090U) /**< \brief (MATRIX3) Priority Register A for Slave 2 */
  #define REG_MATRIX3_PRBS2  (*(__IO uint32_t*)0x48020094U) /**< \brief (MATRIX3) Priority Register B for Slave 2 */
  #define REG_MATRIX3_PRAS3  (*(__IO uint32_t*)0x48020098U) /**< \brief (MATRIX3) Priority Register A for Slave 3 */
  #define REG_MATRIX3_PRBS3  (*(__IO uint32_t*)0x4802009CU) /**< \brief (MATRIX3) Priority Register B for Slave 3 */
  #define REG_MATRIX3_PRAS4  (*(__IO uint32_t*)0x480200A0U) /**< \brief (MATRIX3) Priority Register A for Slave 4 */
  #define REG_MATRIX3_PRBS4  (*(__IO uint32_t*)0x480200A4U) /**< \brief (MATRIX3) Priority Register B for Slave 4 */
  #define REG_MATRIX3_PRAS5  (*(__IO uint32_t*)0x480200A8U) /**< \brief (MATRIX3) Priority Register A for Slave 5 */
  #define REG_MATRIX3_PRBS5  (*(__IO uint32_t*)0x480200ACU) /**< \brief (MATRIX3) Priority Register B for Slave 5 */
  #define REG_MATRIX3_PRAS6  (*(__IO uint32_t*)0x480200B0U) /**< \brief (MATRIX3) Priority Register A for Slave 6 */
  #define REG_MATRIX3_PRBS6  (*(__IO uint32_t*)0x480200B4U) /**< \brief (MATRIX3) Priority Register B for Slave 6 */
  #define REG_MATRIX3_PRAS7  (*(__IO uint32_t*)0x480200B8U) /**< \brief (MATRIX3) Priority Register A for Slave 7 */
  #define REG_MATRIX3_PRBS7  (*(__IO uint32_t*)0x480200BCU) /**< \brief (MATRIX3) Priority Register B for Slave 7 */
  #define REG_MATRIX3_PRAS8  (*(__IO uint32_t*)0x480200C0U) /**< \brief (MATRIX3) Priority Register A for Slave 8 */
  #define REG_MATRIX3_PRBS8  (*(__IO uint32_t*)0x480200C4U) /**< \brief (MATRIX3) Priority Register B for Slave 8 */
  #define REG_MATRIX3_PRAS9  (*(__IO uint32_t*)0x480200C8U) /**< \brief (MATRIX3) Priority Register A for Slave 9 */
  #define REG_MATRIX3_PRBS9  (*(__IO uint32_t*)0x480200CCU) /**< \brief (MATRIX3) Priority Register B for Slave 9 */
  #define REG_MATRIX3_PRAS10 (*(__IO uint32_t*)0x480200D0U) /**< \brief (MATRIX3) Priority Register A for Slave 10 */
  #define REG_MATRIX3_PRBS10 (*(__IO uint32_t*)0x480200D4U) /**< \brief (MATRIX3) Priority Register B for Slave 10 */
  #define REG_MATRIX3_PRAS11 (*(__IO uint32_t*)0x480200D8U) /**< \brief (MATRIX3) Priority Register A for Slave 11 */
  #define REG_MATRIX3_PRBS11 (*(__IO uint32_t*)0x480200DCU) /**< \brief (MATRIX3) Priority Register B for Slave 11 */
  #define REG_MATRIX3_PRAS12 (*(__IO uint32_t*)0x480200E0U) /**< \brief (MATRIX3) Priority Register A for Slave 12 */
  #define REG_MATRIX3_PRBS12 (*(__IO uint32_t*)0x480200E4U) /**< \brief (MATRIX3) Priority Register B for Slave 12 */
  #define REG_MATRIX3_PRAS13 (*(__IO uint32_t*)0x480200E8U) /**< \brief (MATRIX3) Priority Register A for Slave 13 */
  #define REG_MATRIX3_PRBS13 (*(__IO uint32_t*)0x480200ECU) /**< \brief (MATRIX3) Priority Register B for Slave 13 */
  #define REG_MATRIX3_PRAS14 (*(__IO uint32_t*)0x480200F0U) /**< \brief (MATRIX3) Priority Register A for Slave 14 */
  #define REG_MATRIX3_PRBS14 (*(__IO uint32_t*)0x480200F4U) /**< \brief (MATRIX3) Priority Register B for Slave 14 */
  #define REG_MATRIX3_PRAS15 (*(__IO uint32_t*)0x480200F8U) /**< \brief (MATRIX3) Priority Register A for Slave 15 */
  #define REG_MATRIX3_PRBS15 (*(__IO uint32_t*)0x480200FCU) /**< \brief (MATRIX3) Priority Register B for Slave 15 */
  #define REG_MATRIX3_MRCR   (*(__IO uint32_t*)0x48020100U) /**< \brief (MATRIX3) Master Remap Control Register */
  #define REG_MATRIX3_MEIER  (*(__O  uint32_t*)0x48020150U) /**< \brief (MATRIX3) Master Error Interrupt Enable Register */
  #define REG_MATRIX3_MEIDR  (*(__O  uint32_t*)0x48020154U) /**< \brief (MATRIX3) Master Error Interrupt Disable Register */
  #define REG_MATRIX3_MEIMR  (*(__I  uint32_t*)0x48020158U) /**< \brief (MATRIX3) Master Error Interrupt Mask Register */
  #define REG_MATRIX3_MESR   (*(__I  uint32_t*)0x4802015CU) /**< \brief (MATRIX3) Master Error Status Register */
  #define REG_MATRIX3_MEAR   (*(__I  uint32_t*)0x48020160U) /**< \brief (MATRIX3) Master 0 Error Address Register */
  #define REG_MATRIX3_WPMR   (*(__IO uint32_t*)0x480201E4U) /**< \brief (MATRIX3) Write Protect Mode Register */
  #define REG_MATRIX3_WPSR   (*(__I  uint32_t*)0x480201E8U) /**< \brief (MATRIX3) Write Protect Status Register */
  #define REG_MATRIX3_PSR    (*(__IO uint32_t*)0x48020200U) /**< \brief (MATRIX3) Protection Slave 0 Register */
  #define REG_MATRIX3_PASSR  (*(__IO uint32_t*)0x48020240U) /**< \brief (MATRIX3) Protected Areas Split Slave 0 Register */
  #define REG_MATRIX3_PRTSR  (*(__IO uint32_t*)0x48020280U) /**< \brief (MATRIX3) Protected Region Top Slave 0 Register */
  #define REG_MATRIX3_PPSELR (*(__IO uint32_t*)0x480202C0U) /**< \brief (MATRIX3) Protected Peripheral Select 1 Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_MATRIX3_INSTANCE_ */
