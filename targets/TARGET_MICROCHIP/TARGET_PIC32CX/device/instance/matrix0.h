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

#ifndef _PIC32CX_MATRIX0_INSTANCE_
#define _PIC32CX_MATRIX0_INSTANCE_

/* ========== Register definition for MATRIX0 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_MATRIX0_MCFG                    (0x46004000U) /**< \brief (MATRIX0) Master Configuration Register */
  #define REG_MATRIX0_SCFG                    (0x46004040U) /**< \brief (MATRIX0) Slave Configuration Register */
  #define REG_MATRIX0_PRAS0                   (0x46004080U) /**< \brief (MATRIX0) Priority Register A for Slave 0 */
  #define REG_MATRIX0_PRBS0                   (0x46004084U) /**< \brief (MATRIX0) Priority Register B for Slave 0 */
  #define REG_MATRIX0_PRAS1                   (0x46004088U) /**< \brief (MATRIX0) Priority Register A for Slave 1 */
  #define REG_MATRIX0_PRBS1                   (0x4600408CU) /**< \brief (MATRIX0) Priority Register B for Slave 1 */
  #define REG_MATRIX0_PRAS2                   (0x46004090U) /**< \brief (MATRIX0) Priority Register A for Slave 2 */
  #define REG_MATRIX0_PRBS2                   (0x46004094U) /**< \brief (MATRIX0) Priority Register B for Slave 2 */
  #define REG_MATRIX0_PRAS3                   (0x46004098U) /**< \brief (MATRIX0) Priority Register A for Slave 3 */
  #define REG_MATRIX0_PRBS3                   (0x4600409CU) /**< \brief (MATRIX0) Priority Register B for Slave 3 */
  #define REG_MATRIX0_PRAS4                   (0x460040A0U) /**< \brief (MATRIX0) Priority Register A for Slave 4 */
  #define REG_MATRIX0_PRBS4                   (0x460040A4U) /**< \brief (MATRIX0) Priority Register B for Slave 4 */
  #define REG_MATRIX0_PRAS5                   (0x460040A8U) /**< \brief (MATRIX0) Priority Register A for Slave 5 */
  #define REG_MATRIX0_PRBS5                   (0x460040ACU) /**< \brief (MATRIX0) Priority Register B for Slave 5 */
  #define REG_MATRIX0_PRAS6                   (0x460040B0U) /**< \brief (MATRIX0) Priority Register A for Slave 6 */
  #define REG_MATRIX0_PRBS6                   (0x460040B4U) /**< \brief (MATRIX0) Priority Register B for Slave 6 */
  #define REG_MATRIX0_PRAS7                   (0x460040B8U) /**< \brief (MATRIX0) Priority Register A for Slave 7 */
  #define REG_MATRIX0_PRBS7                   (0x460040BCU) /**< \brief (MATRIX0) Priority Register B for Slave 7 */
  #define REG_MATRIX0_PRAS8                   (0x460040C0U) /**< \brief (MATRIX0) Priority Register A for Slave 8 */
  #define REG_MATRIX0_PRBS8                   (0x460040C4U) /**< \brief (MATRIX0) Priority Register B for Slave 8 */
  #define REG_MATRIX0_PRAS9                   (0x460040C8U) /**< \brief (MATRIX0) Priority Register A for Slave 9 */
  #define REG_MATRIX0_PRBS9                   (0x460040CCU) /**< \brief (MATRIX0) Priority Register B for Slave 9 */
  #define REG_MATRIX0_PRAS10                  (0x460040D0U) /**< \brief (MATRIX0) Priority Register A for Slave 10 */
  #define REG_MATRIX0_PRBS10                  (0x460040D4U) /**< \brief (MATRIX0) Priority Register B for Slave 10 */
  #define REG_MATRIX0_PRAS11                  (0x460040D8U) /**< \brief (MATRIX0) Priority Register A for Slave 11 */
  #define REG_MATRIX0_PRBS11                  (0x460040DCU) /**< \brief (MATRIX0) Priority Register B for Slave 11 */
  #define REG_MATRIX0_PRAS12                  (0x460040E0U) /**< \brief (MATRIX0) Priority Register A for Slave 12 */
  #define REG_MATRIX0_PRBS12                  (0x460040E4U) /**< \brief (MATRIX0) Priority Register B for Slave 12 */
  #define REG_MATRIX0_PRAS13                  (0x460040E8U) /**< \brief (MATRIX0) Priority Register A for Slave 13 */
  #define REG_MATRIX0_PRBS13                  (0x460040ECU) /**< \brief (MATRIX0) Priority Register B for Slave 13 */
  #define REG_MATRIX0_PRAS14                  (0x460040F0U) /**< \brief (MATRIX0) Priority Register A for Slave 14 */
  #define REG_MATRIX0_PRBS14                  (0x460040F4U) /**< \brief (MATRIX0) Priority Register B for Slave 14 */
  #define REG_MATRIX0_PRAS15                  (0x460040F8U) /**< \brief (MATRIX0) Priority Register A for Slave 15 */
  #define REG_MATRIX0_PRBS15                  (0x460040FCU) /**< \brief (MATRIX0) Priority Register B for Slave 15 */
  #define REG_MATRIX0_MRCR                    (0x46004100U) /**< \brief (MATRIX0) Master Remap Control Register */
  #define REG_MATRIX0_MEIER                   (0x46004150U) /**< \brief (MATRIX0) Master Error Interrupt Enable Register */
  #define REG_MATRIX0_MEIDR                   (0x46004154U) /**< \brief (MATRIX0) Master Error Interrupt Disable Register */
  #define REG_MATRIX0_MEIMR                   (0x46004158U) /**< \brief (MATRIX0) Master Error Interrupt Mask Register */
  #define REG_MATRIX0_MESR                    (0x4600415CU) /**< \brief (MATRIX0) Master Error Status Register */
  #define REG_MATRIX0_MEAR                    (0x46004160U) /**< \brief (MATRIX0) Master 0 Error Address Register */
  #define REG_MATRIX0_WPMR                    (0x460041E4U) /**< \brief (MATRIX0) Write Protect Mode Register */
  #define REG_MATRIX0_WPSR                    (0x460041E8U) /**< \brief (MATRIX0) Write Protect Status Register */
  #define REG_MATRIX0_PSR                     (0x46004200U) /**< \brief (MATRIX0) Protection Slave 0 Register */
  #define REG_MATRIX0_PASSR                   (0x46004240U) /**< \brief (MATRIX0) Protected Areas Split Slave 0 Register */
  #define REG_MATRIX0_PRTSR                   (0x46004280U) /**< \brief (MATRIX0) Protected Region Top Slave 0 Register */
  #define REG_MATRIX0_PPSELR                  (0x460042C0U) /**< \brief (MATRIX0) Protected Peripheral Select 1 Register */
#else
  #define REG_MATRIX0_MCFG   (*(__IO uint32_t*)0x46004000U) /**< \brief (MATRIX0) Master Configuration Register */
  #define REG_MATRIX0_SCFG   (*(__IO uint32_t*)0x46004040U) /**< \brief (MATRIX0) Slave Configuration Register */
  #define REG_MATRIX0_PRAS0  (*(__IO uint32_t*)0x46004080U) /**< \brief (MATRIX0) Priority Register A for Slave 0 */
  #define REG_MATRIX0_PRBS0  (*(__IO uint32_t*)0x46004084U) /**< \brief (MATRIX0) Priority Register B for Slave 0 */
  #define REG_MATRIX0_PRAS1  (*(__IO uint32_t*)0x46004088U) /**< \brief (MATRIX0) Priority Register A for Slave 1 */
  #define REG_MATRIX0_PRBS1  (*(__IO uint32_t*)0x4600408CU) /**< \brief (MATRIX0) Priority Register B for Slave 1 */
  #define REG_MATRIX0_PRAS2  (*(__IO uint32_t*)0x46004090U) /**< \brief (MATRIX0) Priority Register A for Slave 2 */
  #define REG_MATRIX0_PRBS2  (*(__IO uint32_t*)0x46004094U) /**< \brief (MATRIX0) Priority Register B for Slave 2 */
  #define REG_MATRIX0_PRAS3  (*(__IO uint32_t*)0x46004098U) /**< \brief (MATRIX0) Priority Register A for Slave 3 */
  #define REG_MATRIX0_PRBS3  (*(__IO uint32_t*)0x4600409CU) /**< \brief (MATRIX0) Priority Register B for Slave 3 */
  #define REG_MATRIX0_PRAS4  (*(__IO uint32_t*)0x460040A0U) /**< \brief (MATRIX0) Priority Register A for Slave 4 */
  #define REG_MATRIX0_PRBS4  (*(__IO uint32_t*)0x460040A4U) /**< \brief (MATRIX0) Priority Register B for Slave 4 */
  #define REG_MATRIX0_PRAS5  (*(__IO uint32_t*)0x460040A8U) /**< \brief (MATRIX0) Priority Register A for Slave 5 */
  #define REG_MATRIX0_PRBS5  (*(__IO uint32_t*)0x460040ACU) /**< \brief (MATRIX0) Priority Register B for Slave 5 */
  #define REG_MATRIX0_PRAS6  (*(__IO uint32_t*)0x460040B0U) /**< \brief (MATRIX0) Priority Register A for Slave 6 */
  #define REG_MATRIX0_PRBS6  (*(__IO uint32_t*)0x460040B4U) /**< \brief (MATRIX0) Priority Register B for Slave 6 */
  #define REG_MATRIX0_PRAS7  (*(__IO uint32_t*)0x460040B8U) /**< \brief (MATRIX0) Priority Register A for Slave 7 */
  #define REG_MATRIX0_PRBS7  (*(__IO uint32_t*)0x460040BCU) /**< \brief (MATRIX0) Priority Register B for Slave 7 */
  #define REG_MATRIX0_PRAS8  (*(__IO uint32_t*)0x460040C0U) /**< \brief (MATRIX0) Priority Register A for Slave 8 */
  #define REG_MATRIX0_PRBS8  (*(__IO uint32_t*)0x460040C4U) /**< \brief (MATRIX0) Priority Register B for Slave 8 */
  #define REG_MATRIX0_PRAS9  (*(__IO uint32_t*)0x460040C8U) /**< \brief (MATRIX0) Priority Register A for Slave 9 */
  #define REG_MATRIX0_PRBS9  (*(__IO uint32_t*)0x460040CCU) /**< \brief (MATRIX0) Priority Register B for Slave 9 */
  #define REG_MATRIX0_PRAS10 (*(__IO uint32_t*)0x460040D0U) /**< \brief (MATRIX0) Priority Register A for Slave 10 */
  #define REG_MATRIX0_PRBS10 (*(__IO uint32_t*)0x460040D4U) /**< \brief (MATRIX0) Priority Register B for Slave 10 */
  #define REG_MATRIX0_PRAS11 (*(__IO uint32_t*)0x460040D8U) /**< \brief (MATRIX0) Priority Register A for Slave 11 */
  #define REG_MATRIX0_PRBS11 (*(__IO uint32_t*)0x460040DCU) /**< \brief (MATRIX0) Priority Register B for Slave 11 */
  #define REG_MATRIX0_PRAS12 (*(__IO uint32_t*)0x460040E0U) /**< \brief (MATRIX0) Priority Register A for Slave 12 */
  #define REG_MATRIX0_PRBS12 (*(__IO uint32_t*)0x460040E4U) /**< \brief (MATRIX0) Priority Register B for Slave 12 */
  #define REG_MATRIX0_PRAS13 (*(__IO uint32_t*)0x460040E8U) /**< \brief (MATRIX0) Priority Register A for Slave 13 */
  #define REG_MATRIX0_PRBS13 (*(__IO uint32_t*)0x460040ECU) /**< \brief (MATRIX0) Priority Register B for Slave 13 */
  #define REG_MATRIX0_PRAS14 (*(__IO uint32_t*)0x460040F0U) /**< \brief (MATRIX0) Priority Register A for Slave 14 */
  #define REG_MATRIX0_PRBS14 (*(__IO uint32_t*)0x460040F4U) /**< \brief (MATRIX0) Priority Register B for Slave 14 */
  #define REG_MATRIX0_PRAS15 (*(__IO uint32_t*)0x460040F8U) /**< \brief (MATRIX0) Priority Register A for Slave 15 */
  #define REG_MATRIX0_PRBS15 (*(__IO uint32_t*)0x460040FCU) /**< \brief (MATRIX0) Priority Register B for Slave 15 */
  #define REG_MATRIX0_MRCR   (*(__IO uint32_t*)0x46004100U) /**< \brief (MATRIX0) Master Remap Control Register */
  #define REG_MATRIX0_MEIER  (*(__O  uint32_t*)0x46004150U) /**< \brief (MATRIX0) Master Error Interrupt Enable Register */
  #define REG_MATRIX0_MEIDR  (*(__O  uint32_t*)0x46004154U) /**< \brief (MATRIX0) Master Error Interrupt Disable Register */
  #define REG_MATRIX0_MEIMR  (*(__I  uint32_t*)0x46004158U) /**< \brief (MATRIX0) Master Error Interrupt Mask Register */
  #define REG_MATRIX0_MESR   (*(__I  uint32_t*)0x4600415CU) /**< \brief (MATRIX0) Master Error Status Register */
  #define REG_MATRIX0_MEAR   (*(__I  uint32_t*)0x46004160U) /**< \brief (MATRIX0) Master 0 Error Address Register */
  #define REG_MATRIX0_WPMR   (*(__IO uint32_t*)0x460041E4U) /**< \brief (MATRIX0) Write Protect Mode Register */
  #define REG_MATRIX0_WPSR   (*(__I  uint32_t*)0x460041E8U) /**< \brief (MATRIX0) Write Protect Status Register */
  #define REG_MATRIX0_PSR    (*(__IO uint32_t*)0x46004200U) /**< \brief (MATRIX0) Protection Slave 0 Register */
  #define REG_MATRIX0_PASSR  (*(__IO uint32_t*)0x46004240U) /**< \brief (MATRIX0) Protected Areas Split Slave 0 Register */
  #define REG_MATRIX0_PRTSR  (*(__IO uint32_t*)0x46004280U) /**< \brief (MATRIX0) Protected Region Top Slave 0 Register */
  #define REG_MATRIX0_PPSELR (*(__IO uint32_t*)0x460042C0U) /**< \brief (MATRIX0) Protected Peripheral Select 1 Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_MATRIX0_INSTANCE_ */
