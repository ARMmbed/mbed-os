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

#ifndef _PIC32CX_PIO0A_INSTANCE_
#define _PIC32CX_PIO0A_INSTANCE_

/* ========== Register definition for PIO0A peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_PIO_MSKR0                      (0x40048000U) /**< \brief (PIOA) PIO Mask Register (io_group = 0) */
  #define REG_PIO_CFGR0                      (0x40048004U) /**< \brief (PIOA) PIO Configuration Register (io_group = 0) */
  #define REG_PIO_PDSR0                      (0x40048008U) /**< \brief (PIOA) PIO Pin Data Status Register (io_group = 0) */
  #define REG_PIO_LOCKSR0                    (0x4004800CU) /**< \brief (PIOA) PIO Lock Status Register (io_group = 0) */
  #define REG_PIO_SODR0                      (0x40048010U) /**< \brief (PIOA) PIO Set Output Data Register (io_group = 0) */
  #define REG_PIO_CODR0                      (0x40048014U) /**< \brief (PIOA) PIO Clear Output Data Register (io_group = 0) */
  #define REG_PIO_ODSR0                      (0x40048018U) /**< \brief (PIOA) PIO Output Data Status Register (io_group = 0) */
  #define REG_PIO_IER0                       (0x40048020U) /**< \brief (PIOA) PIO Interrupt Enable Register (io_group = 0) */
  #define REG_PIO_IDR0                       (0x40048024U) /**< \brief (PIOA) PIO Interrupt Disable Register (io_group = 0) */
  #define REG_PIO_IMR0                       (0x40048028U) /**< \brief (PIOA) PIO Interrupt Mask Register (io_group = 0) */
  #define REG_PIO_ISR0                       (0x4004802CU) /**< \brief (PIOA) PIO Interrupt Status Register (io_group = 0) */
  #define REG_PIO_IOFR0                      (0x4004803CU) /**< \brief (PIOA) PIO I/O Freeze Configuration Register (io_group = 0) */
  #define REG_PIO_MSKR1                      (0x40048040U) /**< \brief (PIOB) PIO Mask Register (io_group = 1) */
  #define REG_PIO_CFGR1                      (0x40048044U) /**< \brief (PIOB) PIO Configuration Register (io_group = 1) */
  #define REG_PIO_PDSR1                      (0x40048048U) /**< \brief (PIOB) PIO Pin Data Status Register (io_group = 1) */
  #define REG_PIO_LOCKSR1                    (0x4004804CU) /**< \brief (PIOB) PIO Lock Status Register (io_group = 1) */
  #define REG_PIO_SODR1                      (0x40048050U) /**< \brief (PIOB) PIO Set Output Data Register (io_group = 1) */
  #define REG_PIO_CODR1                      (0x40048054U) /**< \brief (PIOB) PIO Clear Output Data Register (io_group = 1) */
  #define REG_PIO_ODSR1                      (0x40048058U) /**< \brief (PIOB) PIO Output Data Status Register (io_group = 1) */
  #define REG_PIO_IER1                       (0x40048060U) /**< \brief (PIOB) PIO Interrupt Enable Register (io_group = 1) */
  #define REG_PIO_IDR1                       (0x40048064U) /**< \brief (PIOB) PIO Interrupt Disable Register (io_group = 1) */
  #define REG_PIO_IMR1                       (0x40048068U) /**< \brief (PIOB) PIO Interrupt Mask Register (io_group = 1) */
  #define REG_PIO_ISR1                       (0x4004806CU) /**< \brief (PIOB) PIO Interrupt Status Register (io_group = 1) */
  #define REG_PIO_IOFR1                      (0x4004807CU) /**< \brief (PIOB) PIO I/O Freeze Configuration Register (io_group = 1) */
  #define REG_PIO_MSKR2                      (0x40048080U) /**< \brief (PIOC) PIO Mask Register (io_group = 2) */
  #define REG_PIO_CFGR2                      (0x40048084U) /**< \brief (PIOC) PIO Configuration Register (io_group = 2) */
  #define REG_PIO_PDSR2                      (0x40048088U) /**< \brief (PIOC) PIO Pin Data Status Register (io_group = 2) */
  #define REG_PIO_LOCKSR2                    (0x4004808CU) /**< \brief (PIOC) PIO Lock Status Register (io_group = 2) */
  #define REG_PIO_SODR2                      (0x40048090U) /**< \brief (PIOC) PIO Set Output Data Register (io_group = 2) */
  #define REG_PIO_CODR2                      (0x40048094U) /**< \brief (PIOC) PIO Clear Output Data Register (io_group = 2) */
  #define REG_PIO_ODSR2                      (0x40048098U) /**< \brief (PIOC) PIO Output Data Status Register (io_group = 2) */
  #define REG_PIO_IER2                       (0x400480A0U) /**< \brief (PIOC) PIO Interrupt Enable Register (io_group = 2) */
  #define REG_PIO_IDR2                       (0x400480A4U) /**< \brief (PIOC) PIO Interrupt Disable Register (io_group = 2) */
  #define REG_PIO_IMR2                       (0x400480A8U) /**< \brief (PIOC) PIO Interrupt Mask Register (io_group = 2) */
  #define REG_PIO_ISR2                       (0x400480ACU) /**< \brief (PIOC) PIO Interrupt Status Register (io_group = 2) */
  #define REG_PIO_IOFR2                      (0x400480BCU) /**< \brief (PIOC) PIO I/O Freeze Configuration Register (io_group = 2) */
  #define REG_PIO_SCDR                       (0x40048500U) /**< \brief (PIOA/B/C) PIO Slow Clock Divider Debouncing Register */
  #define REG_PIO_WPMR                       (0x400485E0U) /**< \brief (PIOA/B/C) PIO Write Protection Mode Register */
  #define REG_PIO_WPSR                       (0x400485E4U) /**< \brief (PIOA/B/C) PIO Write Protection Status Register */
  #define REG_PIO_P_MSKR0                    (0x40049000U) /**< \brief (PIOA) Privilege PIO Mask Register (io_group = 0) */
  #define REG_PIO_P_CFGR0                    (0x40049004U) /**< \brief (PIOA) Privilege PIO Configuration Register (io_group = 0) */
  #define REG_PIO_P_PDSR0                    (0x40049008U) /**< \brief (PIOA) Privilege PIO Pin Data Status Register (io_group = 0) */
  #define REG_PIO_P_LOCKSR0                  (0x4004900CU) /**< \brief (PIOA) Privilege PIO Lock Status Register (io_group = 0) */
  #define REG_PIO_P_SODR0                    (0x40049010U) /**< \brief (PIOA) Privilege PIO Set Output Data Register (io_group = 0) */
  #define REG_PIO_P_CODR0                    (0x40049014U) /**< \brief (PIOA) Privilege PIO Clear Output Data Register (io_group = 0) */
  #define REG_PIO_P_ODSR0                    (0x40049018U) /**< \brief (PIOA) Privilege PIO Output Data Status Register (io_group = 0) */
  #define REG_PIO_P_IER0                     (0x40049020U) /**< \brief (PIOA) Privilege PIO Interrupt Enable Register (io_group = 0) */
  #define REG_PIO_P_IDR0                     (0x40049024U) /**< \brief (PIOA) Privilege PIO Interrupt Disable Register (io_group = 0) */
  #define REG_PIO_P_IMR0                     (0x40049028U) /**< \brief (PIOA) Privilege PIO Interrupt Mask Register (io_group = 0) */
  #define REG_PIO_P_ISR0                     (0x4004902CU) /**< \brief (PIOA) Privilege PIO Interrupt Status Register (io_group = 0) */
  #define REG_PIO_P_SIONR0                   (0x40049030U) /**< \brief (PIOA) Privilege PIO Set I/O Non-Secure Register (io_group = 0) */
  #define REG_PIO_P_SIOSR0                   (0x40049034U) /**< \brief (PIOA) Privilege PIO Set I/O Secure Register (io_group = 0) */
  #define REG_PIO_P_IOSSR0                   (0x40049038U) /**< \brief (PIOA) Privilege PIO I/O Security Status Register (io_group = 0) */
  #define REG_PIO_P_IOFR0                    (0x4004903CU) /**< \brief (PIOA) Privilege PIO I/O Freeze Configuration Register (io_group = 0) */
  #define REG_PIO_P_MSKR1                    (0x40049040U) /**< \brief (PIOB) Privilege PIO Mask Register (io_group = 1) */
  #define REG_PIO_P_CFGR1                    (0x40049044U) /**< \brief (PIOB) Privilege PIO Configuration Register (io_group = 1) */
  #define REG_PIO_P_PDSR1                    (0x40049048U) /**< \brief (PIOB) Privilege PIO Pin Data Status Register (io_group = 1) */
  #define REG_PIO_P_LOCKSR1                  (0x4004904CU) /**< \brief (PIOB) Privilege PIO Lock Status Register (io_group = 1) */
  #define REG_PIO_P_SODR1                    (0x40049050U) /**< \brief (PIOB) Privilege PIO Set Output Data Register (io_group = 1) */
  #define REG_PIO_P_CODR1                    (0x40049054U) /**< \brief (PIOB) Privilege PIO Clear Output Data Register (io_group = 1) */
  #define REG_PIO_P_ODSR1                    (0x40049058U) /**< \brief (PIOB) Privilege PIO Output Data Status Register (io_group = 1) */
  #define REG_PIO_P_IER1                     (0x40049060U) /**< \brief (PIOB) Privilege PIO Interrupt Enable Register (io_group = 1) */
  #define REG_PIO_P_IDR1                     (0x40049064U) /**< \brief (PIOB) Privilege PIO Interrupt Disable Register (io_group = 1) */
  #define REG_PIO_P_IMR1                     (0x40049068U) /**< \brief (PIOB) Privilege PIO Interrupt Mask Register (io_group = 1) */
  #define REG_PIO_P_ISR1                     (0x4004906CU) /**< \brief (PIOB) Privilege PIO Interrupt Status Register (io_group = 1) */
  #define REG_PIO_P_SIONR1                   (0x40049070U) /**< \brief (PIOB) Privilege PIO Set I/O Non-Secure Register (io_group = 1) */
  #define REG_PIO_P_SIOSR1                   (0x40049074U) /**< \brief (PIOB) Privilege PIO Set I/O Secure Register (io_group = 1) */
  #define REG_PIO_P_IOSSR1                   (0x40049078U) /**< \brief (PIOB) Privilege PIO I/O Security Status Register (io_group = 1) */
  #define REG_PIO_P_IOFR1                    (0x4004907CU) /**< \brief (PIOB) Privilege PIO I/O Freeze Configuration Register (io_group = 1) */
  #define REG_PIO_P_MSKR2                    (0x40049080U) /**< \brief (PIOC) Privilege PIO Mask Register (io_group = 2) */
  #define REG_PIO_P_CFGR2                    (0x40049084U) /**< \brief (PIOC) Privilege PIO Configuration Register (io_group = 2) */
  #define REG_PIO_P_PDSR2                    (0x40049088U) /**< \brief (PIOC) Privilege PIO Pin Data Status Register (io_group = 2) */
  #define REG_PIO_P_LOCKSR2                  (0x4004908CU) /**< \brief (PIOC) Privilege PIO Lock Status Register (io_group = 2) */
  #define REG_PIO_P_SODR2                    (0x40049090U) /**< \brief (PIOC) Privilege PIO Set Output Data Register (io_group = 2) */
  #define REG_PIO_P_CODR2                    (0x40049094U) /**< \brief (PIOC) Privilege PIO Clear Output Data Register (io_group = 2) */
  #define REG_PIO_P_ODSR2                    (0x40049098U) /**< \brief (PIOC) Privilege PIO Output Data Status Register (io_group = 2) */
  #define REG_PIO_P_IER2                     (0x400490A0U) /**< \brief (PIOC) Privilege PIO Interrupt Enable Register (io_group = 2) */
  #define REG_PIO_P_IDR2                     (0x400490A4U) /**< \brief (PIOC) Privilege PIO Interrupt Disable Register (io_group = 2) */
  #define REG_PIO_P_IMR2                     (0x400490A8U) /**< \brief (PIOC) Privilege PIO Interrupt Mask Register (io_group = 2) */
  #define REG_PIO_P_ISR2                     (0x400490ACU) /**< \brief (PIOC) Privilege PIO Interrupt Status Register (io_group = 2) */
  #define REG_PIO_P_SIONR2                   (0x400490B0U) /**< \brief (PIOC) Privilege PIO Set I/O Non-Secure Register (io_group = 2) */
  #define REG_PIO_P_SIOSR2                   (0x400490B4U) /**< \brief (PIOC) Privilege PIO Set I/O Secure Register (io_group = 2) */
  #define REG_PIO_P_IOSSR2                   (0x400490B8U) /**< \brief (PIOC) Privilege PIO I/O Security Status Register (io_group = 2) */
  #define REG_PIO_P_IOFR2                    (0x400490BCU) /**< \brief (PIOC) Privilege PIO I/O Freeze Configuration Register (io_group = 2) */
  #define REG_PIO_P_SCDR                     (0x40049500U) /**< \brief (PIOA/B/C) Privilege PIO Slow Clock Divider Debouncing Register */
  #define REG_PIO_P_WPMR                     (0x400495E0U) /**< \brief (PIOA/B/C) Privilege PIO Write Protection Mode Register */
  #define REG_PIO_P_WPSR                     (0x400495E4U) /**< \brief (PIOA/B/C) Privilege PIO Write Protection Status Register */
  #define REG_PIO_MSKR3                      (0x4800C000U) /**< \brief (PIOD) PIO Mask Register (io_group = 0) */
  #define REG_PIO_CFGR3                      (0x4800C004U) /**< \brief (PIOD) PIO Configuration Register (io_group = 0) */
  #define REG_PIO_PDSR3                      (0x4800C008U) /**< \brief (PIOD) PIO Pin Data Status Register (io_group = 0) */
  #define REG_PIO_LOCKSR3                    (0x4800C00CU) /**< \brief (PIOD) PIO Lock Status Register (io_group = 0) */
  #define REG_PIO_SODR3                      (0x4800C010U) /**< \brief (PIOD) PIO Set Output Data Register (io_group = 0) */
  #define REG_PIO_CODR3                      (0x4800C014U) /**< \brief (PIOD) PIO Clear Output Data Register (io_group = 0) */
  #define REG_PIO_ODSR3                      (0x4800C018U) /**< \brief (PIOD) PIO Output Data Status Register (io_group = 0) */
  #define REG_PIO_IER3                       (0x4800C020U) /**< \brief (PIOD) PIO Interrupt Enable Register (io_group = 0) */
  #define REG_PIO_IDR3                       (0x4800C024U) /**< \brief (PIOD) PIO Interrupt Disable Register (io_group = 0) */
  #define REG_PIO_IMR3                       (0x4800C028U) /**< \brief (PIOD) PIO Interrupt Mask Register (io_group = 0) */
  #define REG_PIO_ISR3                       (0x4800C02CU) /**< \brief (PIOD) PIO Interrupt Status Register (io_group = 0) */
  #define REG_PIO_IOFR3                      (0x4800C03CU) /**< \brief (PIOD) PIO I/O Freeze Configuration Register (io_group = 0) */
  #define REG_PIO_SCDR3                      (0x4800C500U) /**< \brief (PIOD) PIO Slow Clock Divider Debouncing Register */
  #define REG_PIO_WPMR3                      (0x4800C5E0U) /**< \brief (PIOD) PIO Write Protection Mode Register */
  #define REG_PIO_WPSR3                      (0x4800C5E4U) /**< \brief (PIOD) PIO Write Protection Status Register */
  #define REG_PIO_P_MSKR3                    (0x4800C000U) /**< \brief (PIOD) Privilege PIO Mask Register (io_group = 0) */
  #define REG_PIO_P_CFGR3                    (0x4800C004U) /**< \brief (PIOD) Privilege PIO Configuration Register (io_group = 0) */
  #define REG_PIO_P_PDSR3                    (0x4800C008U) /**< \brief (PIOD) Privilege PIO Pin Data Status Register (io_group = 0) */
  #define REG_PIO_P_LOCKSR3                  (0x4800C00CU) /**< \brief (PIOD) Privilege PIO Lock Status Register (io_group = 0) */
  #define REG_PIO_P_SODR3                    (0x4800C010U) /**< \brief (PIOD) Privilege PIO Set Output Data Register (io_group = 0) */
  #define REG_PIO_P_CODR3                    (0x4800C014U) /**< \brief (PIOD) Privilege PIO Clear Output Data Register (io_group = 0) */
  #define REG_PIO_P_ODSR3                    (0x4800C018U) /**< \brief (PIOD) Privilege PIO Output Data Status Register (io_group = 0) */
  #define REG_PIO_P_IER3                     (0x4800C020U) /**< \brief (PIOD) Privilege PIO Interrupt Enable Register (io_group = 0) */
  #define REG_PIO_P_IDR3                     (0x4800C024U) /**< \brief (PIOD) Privilege PIO Interrupt Disable Register (io_group = 0) */
  #define REG_PIO_P_IMR3                     (0x4800C028U) /**< \brief (PIOD) Privilege PIO Interrupt Mask Register (io_group = 0) */
  #define REG_PIO_P_ISR3                     (0x4800C02CU) /**< \brief (PIOD) Privilege PIO Interrupt Status Register (io_group = 0) */
  #define REG_PIO_P_SIONR3                   (0x4800C030U) /**< \brief (PIOD) Privilege PIO Set I/O Non-Secure Register (io_group = 0) */
  #define REG_PIO_P_SIOSR3                   (0x4800C034U) /**< \brief (PIOD) Privilege PIO Set I/O Secure Register (io_group = 0) */
  #define REG_PIO_P_IOSSR3                   (0x4800C038U) /**< \brief (PIOD) Privilege PIO I/O Security Status Register (io_group = 0) */
  #define REG_PIO_P_IOFR3                    (0x4800C03CU) /**< \brief (PIOD) Privilege PIO I/O Freeze Configuration Register (io_group = 0) */
  #define REG_PIO_P_SCDR3                    (0x4800C500U) /**< \brief (PIOD) Privilege PIO Slow Clock Divider Debouncing Register */
  #define REG_PIO_P_WPMR3                    (0x4800C5E0U) /**< \brief (PIOD) Privilege PIO Write Protection Mode Register */
  #define REG_PIO_P_WPSR3                    (0x4800C5E4U) /**< \brief (PIOD) Privilege PIO Write Protection Status Register */
#else
  #define REG_PIO_MSKR0     (*(__IO uint32_t*)0x40048000U) /**< \brief (PIOA) PIO Mask Register (io_group = 0) */
  #define REG_PIO_CFGR0     (*(__IO uint32_t*)0x40048004U) /**< \brief (PIOA) PIO Configuration Register (io_group = 0) */
  #define REG_PIO_PDSR0     (*(__I  uint32_t*)0x40048008U) /**< \brief (PIOA) PIO Pin Data Status Register (io_group = 0) */
  #define REG_PIO_LOCKSR0   (*(__I  uint32_t*)0x4004800CU) /**< \brief (PIOA) PIO Lock Status Register (io_group = 0) */
  #define REG_PIO_SODR0     (*(__O  uint32_t*)0x40048010U) /**< \brief (PIOA) PIO Set Output Data Register (io_group = 0) */
  #define REG_PIO_CODR0     (*(__O  uint32_t*)0x40048014U) /**< \brief (PIOA) PIO Clear Output Data Register (io_group = 0) */
  #define REG_PIO_ODSR0     (*(__IO uint32_t*)0x40048018U) /**< \brief (PIOA) PIO Output Data Status Register (io_group = 0) */
  #define REG_PIO_IER0      (*(__O  uint32_t*)0x40048020U) /**< \brief (PIOA) PIO Interrupt Enable Register (io_group = 0) */
  #define REG_PIO_IDR0      (*(__O  uint32_t*)0x40048024U) /**< \brief (PIOA) PIO Interrupt Disable Register (io_group = 0) */
  #define REG_PIO_IMR0      (*(__I  uint32_t*)0x40048028U) /**< \brief (PIOA) PIO Interrupt Mask Register (io_group = 0) */
  #define REG_PIO_ISR0      (*(__I  uint32_t*)0x4004802CU) /**< \brief (PIOA) PIO Interrupt Status Register (io_group = 0) */
  #define REG_PIO_IOFR0     (*(__O  uint32_t*)0x4004803CU) /**< \brief (PIOA) PIO I/O Freeze Configuration Register (io_group = 0) */
  #define REG_PIO_MSKR1     (*(__IO uint32_t*)0x40048040U) /**< \brief (PIOB) PIO Mask Register (io_group = 1) */
  #define REG_PIO_CFGR1     (*(__IO uint32_t*)0x40048044U) /**< \brief (PIOB) PIO Configuration Register (io_group = 1) */
  #define REG_PIO_PDSR1     (*(__I  uint32_t*)0x40048048U) /**< \brief (PIOB) PIO Pin Data Status Register (io_group = 1) */
  #define REG_PIO_LOCKSR1   (*(__I  uint32_t*)0x4004804CU) /**< \brief (PIOB) PIO Lock Status Register (io_group = 1) */
  #define REG_PIO_SODR1     (*(__O  uint32_t*)0x40048050U) /**< \brief (PIOB) PIO Set Output Data Register (io_group = 1) */
  #define REG_PIO_CODR1     (*(__O  uint32_t*)0x40048054U) /**< \brief (PIOB) PIO Clear Output Data Register (io_group = 1) */
  #define REG_PIO_ODSR1     (*(__IO uint32_t*)0x40048058U) /**< \brief (PIOB) PIO Output Data Status Register (io_group = 1) */
  #define REG_PIO_IER1      (*(__O  uint32_t*)0x40048060U) /**< \brief (PIOB) PIO Interrupt Enable Register (io_group = 1) */
  #define REG_PIO_IDR1      (*(__O  uint32_t*)0x40048064U) /**< \brief (PIOB) PIO Interrupt Disable Register (io_group = 1) */
  #define REG_PIO_IMR1      (*(__I  uint32_t*)0x40048068U) /**< \brief (PIOB) PIO Interrupt Mask Register (io_group = 1) */
  #define REG_PIO_ISR1      (*(__I  uint32_t*)0x4004806CU) /**< \brief (PIOB) PIO Interrupt Status Register (io_group = 1) */
  #define REG_PIO_IOFR1     (*(__O  uint32_t*)0x4004807CU) /**< \brief (PIOB) PIO I/O Freeze Configuration Register (io_group = 1) */
  #define REG_PIO_MSKR2     (*(__IO uint32_t*)0x40048080U) /**< \brief (PIOC) PIO Mask Register (io_group = 2) */
  #define REG_PIO_CFGR2     (*(__IO uint32_t*)0x40048084U) /**< \brief (PIOC) PIO Configuration Register (io_group = 2) */
  #define REG_PIO_PDSR2     (*(__I  uint32_t*)0x40048088U) /**< \brief (PIOC) PIO Pin Data Status Register (io_group = 2) */
  #define REG_PIO_LOCKSR2   (*(__I  uint32_t*)0x4004808CU) /**< \brief (PIOC) PIO Lock Status Register (io_group = 2) */
  #define REG_PIO_SODR2     (*(__O  uint32_t*)0x40048090U) /**< \brief (PIOC) PIO Set Output Data Register (io_group = 2) */
  #define REG_PIO_CODR2     (*(__O  uint32_t*)0x40048094U) /**< \brief (PIOC) PIO Clear Output Data Register (io_group = 2) */
  #define REG_PIO_ODSR2     (*(__IO uint32_t*)0x40048098U) /**< \brief (PIOC) PIO Output Data Status Register (io_group = 2) */
  #define REG_PIO_IER2      (*(__O  uint32_t*)0x400480A0U) /**< \brief (PIOC) PIO Interrupt Enable Register (io_group = 2) */
  #define REG_PIO_IDR2      (*(__O  uint32_t*)0x400480A4U) /**< \brief (PIOC) PIO Interrupt Disable Register (io_group = 2) */
  #define REG_PIO_IMR2      (*(__I  uint32_t*)0x400480A8U) /**< \brief (PIOC) PIO Interrupt Mask Register (io_group = 2) */
  #define REG_PIO_ISR2      (*(__I  uint32_t*)0x400480ACU) /**< \brief (PIOC) PIO Interrupt Status Register (io_group = 2) */
  #define REG_PIO_IOFR2     (*(__O  uint32_t*)0x400480BCU) /**< \brief (PIOC) PIO I/O Freeze Configuration Register (io_group = 2) */
  #define REG_PIO_SCDR      (*(__IO uint32_t*)0x40048500U) /**< \brief (PIOA/B/C) PIO Slow Clock Divider Debouncing Register */
  #define REG_PIO_WPMR      (*(__IO uint32_t*)0x400485E0U) /**< \brief (PIOA/B/C) PIO Write Protection Mode Register */
  #define REG_PIO_WPSR      (*(__I  uint32_t*)0x400485E4U) /**< \brief (PIOA/B/C) PIO Write Protection Status Register */
  #define REG_PIO_P_MSKR0   (*(__IO uint32_t*)0x40049000U) /**< \brief (PIOA) Privilege PIO Mask Register (io_group = 0) */
  #define REG_PIO_P_CFGR0   (*(__IO uint32_t*)0x40049004U) /**< \brief (PIOA) Privilege PIO Configuration Register (io_group = 0) */
  #define REG_PIO_P_PDSR0   (*(__I  uint32_t*)0x40049008U) /**< \brief (PIOA) Privilege PIO Pin Data Status Register (io_group = 0) */
  #define REG_PIO_P_LOCKSR0 (*(__I  uint32_t*)0x4004900CU) /**< \brief (PIOA) Privilege PIO Lock Status Register (io_group = 0) */
  #define REG_PIO_P_SODR0   (*(__O  uint32_t*)0x40049010U) /**< \brief (PIOA) Privilege PIO Set Output Data Register (io_group = 0) */
  #define REG_PIO_P_CODR0   (*(__O  uint32_t*)0x40049014U) /**< \brief (PIOA) Privilege PIO Clear Output Data Register (io_group = 0) */
  #define REG_PIO_P_ODSR0   (*(__IO uint32_t*)0x40049018U) /**< \brief (PIOA) Privilege PIO Output Data Status Register (io_group = 0) */
  #define REG_PIO_P_IER0    (*(__O  uint32_t*)0x40049020U) /**< \brief (PIOA) Privilege PIO Interrupt Enable Register (io_group = 0) */
  #define REG_PIO_P_IDR0    (*(__O  uint32_t*)0x40049024U) /**< \brief (PIOA) Privilege PIO Interrupt Disable Register (io_group = 0) */
  #define REG_PIO_P_IMR0    (*(__I  uint32_t*)0x40049028U) /**< \brief (PIOA) Privilege PIO Interrupt Mask Register (io_group = 0) */
  #define REG_PIO_P_ISR0    (*(__I  uint32_t*)0x4004902CU) /**< \brief (PIOA) Privilege PIO Interrupt Status Register (io_group = 0) */
  #define REG_PIO_P_SIONR0  (*(__O  uint32_t*)0x40049030U) /**< \brief (PIOA) Privilege PIO Set I/O Non-Secure Register (io_group = 0) */
  #define REG_PIO_P_SIOSR0  (*(__O  uint32_t*)0x40049034U) /**< \brief (PIOA) Privilege PIO Set I/O Secure Register (io_group = 0) */
  #define REG_PIO_P_IOSSR0  (*(__I  uint32_t*)0x40049038U) /**< \brief (PIOA) Privilege PIO I/O Security Status Register (io_group = 0) */
  #define REG_PIO_P_IOFR0   (*(__O  uint32_t*)0x4004903CU) /**< \brief (PIOA) Privilege PIO I/O Freeze Configuration Register (io_group = 0) */
  #define REG_PIO_P_MSKR1   (*(__IO uint32_t*)0x40049040U) /**< \brief (PIOB) Privilege PIO Mask Register (io_group = 1) */
  #define REG_PIO_P_CFGR1   (*(__IO uint32_t*)0x40049044U) /**< \brief (PIOB) Privilege PIO Configuration Register (io_group = 1) */
  #define REG_PIO_P_PDSR1   (*(__I  uint32_t*)0x40049048U) /**< \brief (PIOB) Privilege PIO Pin Data Status Register (io_group = 1) */
  #define REG_PIO_P_LOCKSR1 (*(__I  uint32_t*)0x4004904CU) /**< \brief (PIOB) Privilege PIO Lock Status Register (io_group = 1) */
  #define REG_PIO_P_SODR1   (*(__O  uint32_t*)0x40049050U) /**< \brief (PIOB) Privilege PIO Set Output Data Register (io_group = 1) */
  #define REG_PIO_P_CODR1   (*(__O  uint32_t*)0x40049054U) /**< \brief (PIOB) Privilege PIO Clear Output Data Register (io_group = 1) */
  #define REG_PIO_P_ODSR1   (*(__IO uint32_t*)0x40049058U) /**< \brief (PIOB) Privilege PIO Output Data Status Register (io_group = 1) */
  #define REG_PIO_P_IER1    (*(__O  uint32_t*)0x40049060U) /**< \brief (PIOB) Privilege PIO Interrupt Enable Register (io_group = 1) */
  #define REG_PIO_P_IDR1    (*(__O  uint32_t*)0x40049064U) /**< \brief (PIOB) Privilege PIO Interrupt Disable Register (io_group = 1) */
  #define REG_PIO_P_IMR1    (*(__I  uint32_t*)0x40049068U) /**< \brief (PIOB) Privilege PIO Interrupt Mask Register (io_group = 1) */
  #define REG_PIO_P_ISR1    (*(__I  uint32_t*)0x4004906CU) /**< \brief (PIOB) Privilege PIO Interrupt Status Register (io_group = 1) */
  #define REG_PIO_P_SIONR1  (*(__O  uint32_t*)0x40049070U) /**< \brief (PIOB) Privilege PIO Set I/O Non-Secure Register (io_group = 1) */
  #define REG_PIO_P_SIOSR1  (*(__O  uint32_t*)0x40049074U) /**< \brief (PIOB) Privilege PIO Set I/O Secure Register (io_group = 1) */
  #define REG_PIO_P_IOSSR1  (*(__I  uint32_t*)0x40049078U) /**< \brief (PIOB) Privilege PIO I/O Security Status Register (io_group = 1) */
  #define REG_PIO_P_IOFR1   (*(__O  uint32_t*)0x4004907CU) /**< \brief (PIOB) Privilege PIO I/O Freeze Configuration Register (io_group = 1) */
  #define REG_PIO_P_MSKR2   (*(__IO uint32_t*)0x40049080U) /**< \brief (PIOC) Privilege PIO Mask Register (io_group = 2) */
  #define REG_PIO_P_CFGR2   (*(__IO uint32_t*)0x40049084U) /**< \brief (PIOC) Privilege PIO Configuration Register (io_group = 2) */
  #define REG_PIO_P_PDSR2   (*(__I  uint32_t*)0x40049088U) /**< \brief (PIOC) Privilege PIO Pin Data Status Register (io_group = 2) */
  #define REG_PIO_P_LOCKSR2 (*(__I  uint32_t*)0x4004908CU) /**< \brief (PIOC) Privilege PIO Lock Status Register (io_group = 2) */
  #define REG_PIO_P_SODR2   (*(__O  uint32_t*)0x40049090U) /**< \brief (PIOC) Privilege PIO Set Output Data Register (io_group = 2) */
  #define REG_PIO_P_CODR2   (*(__O  uint32_t*)0x40049094U) /**< \brief (PIOC) Privilege PIO Clear Output Data Register (io_group = 2) */
  #define REG_PIO_P_ODSR2   (*(__IO uint32_t*)0x40049098U) /**< \brief (PIOC) Privilege PIO Output Data Status Register (io_group = 2) */
  #define REG_PIO_P_IER2    (*(__O  uint32_t*)0x400490A0U) /**< \brief (PIOC) Privilege PIO Interrupt Enable Register (io_group = 2) */
  #define REG_PIO_P_IDR2    (*(__O  uint32_t*)0x400490A4U) /**< \brief (PIOC) Privilege PIO Interrupt Disable Register (io_group = 2) */
  #define REG_PIO_P_IMR2    (*(__I  uint32_t*)0x400490A8U) /**< \brief (PIOC) Privilege PIO Interrupt Mask Register (io_group = 2) */
  #define REG_PIO_P_ISR2    (*(__I  uint32_t*)0x400490ACU) /**< \brief (PIOC) Privilege PIO Interrupt Status Register (io_group = 2) */
  #define REG_PIO_P_SIONR2  (*(__O  uint32_t*)0x400490B0U) /**< \brief (PIOC) Privilege PIO Set I/O Non-Secure Register (io_group = 2) */
  #define REG_PIO_P_SIOSR2  (*(__O  uint32_t*)0x400490B4U) /**< \brief (PIOC) Privilege PIO Set I/O Secure Register (io_group = 2) */
  #define REG_PIO_P_IOSSR2  (*(__I  uint32_t*)0x400490B8U) /**< \brief (PIOC) Privilege PIO I/O Security Status Register (io_group = 2) */
  #define REG_PIO_P_IOFR2   (*(__O  uint32_t*)0x400490BCU) /**< \brief (PIOC) Privilege PIO I/O Freeze Configuration Register (io_group = 2) */
  #define REG_PIO_P_SCDR    (*(__IO uint32_t*)0x40049500U) /**< \brief (PIOA/B/C) Privilege PIO Slow Clock Divider Debouncing Register */
  #define REG_PIO_P_WPMR    (*(__IO uint32_t*)0x400495E0U) /**< \brief (PIOA/B/C) Privilege PIO Write Protection Mode Register */
  #define REG_PIO_P_WPSR    (*(__I  uint32_t*)0x400495E4U) /**< \brief (PIOA/B/C) Privilege PIO Write Protection Status Register */
  #define REG_PIO_MSKR3     (*(__IO uint32_t*)0x4800C000U) /**< \brief (PIOD) PIO Mask Register (io_group = 0) */
  #define REG_PIO_CFGR3     (*(__IO uint32_t*)0x4800C004U) /**< \brief (PIOD) PIO Configuration Register (io_group = 0) */
  #define REG_PIO_PDSR3     (*(__I  uint32_t*)0x4800C008U) /**< \brief (PIOD) PIO Pin Data Status Register (io_group = 0) */
  #define REG_PIO_LOCKSR3   (*(__I  uint32_t*)0x4800C00CU) /**< \brief (PIOD) PIO Lock Status Register (io_group = 0) */
  #define REG_PIO_SODR3     (*(__O  uint32_t*)0x4800C010U) /**< \brief (PIOD) PIO Set Output Data Register (io_group = 0) */
  #define REG_PIO_CODR3     (*(__O  uint32_t*)0x4800C014U) /**< \brief (PIOD) PIO Clear Output Data Register (io_group = 0) */
  #define REG_PIO_ODSR3     (*(__IO uint32_t*)0x4800C018U) /**< \brief (PIOD) PIO Output Data Status Register (io_group = 0) */
  #define REG_PIO_IER3      (*(__O  uint32_t*)0x4800C020U) /**< \brief (PIOD) PIO Interrupt Enable Register (io_group = 0) */
  #define REG_PIO_IDR3      (*(__O  uint32_t*)0x4800C024U) /**< \brief (PIOD) PIO Interrupt Disable Register (io_group = 0) */
  #define REG_PIO_IMR3      (*(__I  uint32_t*)0x4800C028U) /**< \brief (PIOD) PIO Interrupt Mask Register (io_group = 0) */
  #define REG_PIO_ISR3      (*(__I  uint32_t*)0x4800C02CU) /**< \brief (PIOD) PIO Interrupt Status Register (io_group = 0) */
  #define REG_PIO_IOFR3     (*(__O  uint32_t*)0x4800C03CU) /**< \brief (PIOD) PIO I/O Freeze Configuration Register (io_group = 0) */
  #define REG_PIO_SCDR3     (*(__IO uint32_t*)0x4800C500U) /**< \brief (PIOD) PIO Slow Clock Divider Debouncing Register */
  #define REG_PIO_WPMR3     (*(__IO uint32_t*)0x4800C5E0U) /**< \brief (PIOD) PIO Write Protection Mode Register */
  #define REG_PIO_WPSR3     (*(__I  uint32_t*)0x4800C5E4U) /**< \brief (PIOD) PIO Write Protection Status Register */
  #define REG_PIO_P_MSKR3   (*(__IO uint32_t*)0x4800C000U) /**< \brief (PIOD) Privilege PIO Mask Register (io_group = 0) */
  #define REG_PIO_P_CFGR3   (*(__IO uint32_t*)0x4800C004U) /**< \brief (PIOD) Privilege PIO Configuration Register (io_group = 0) */
  #define REG_PIO_P_PDSR3   (*(__I  uint32_t*)0x4800C008U) /**< \brief (PIOD) Privilege PIO Pin Data Status Register (io_group = 0) */
  #define REG_PIO_P_LOCKSR3 (*(__I  uint32_t*)0x4800C00CU) /**< \brief (PIOD) Privilege PIO Lock Status Register (io_group = 0) */
  #define REG_PIO_P_SODR3   (*(__O  uint32_t*)0x4800C010U) /**< \brief (PIOD) Privilege PIO Set Output Data Register (io_group = 0) */
  #define REG_PIO_P_CODR3   (*(__O  uint32_t*)0x4800C014U) /**< \brief (PIOD) Privilege PIO Clear Output Data Register (io_group = 0) */
  #define REG_PIO_P_ODSR3   (*(__IO uint32_t*)0x4800C018U) /**< \brief (PIOD) Privilege PIO Output Data Status Register (io_group = 0) */
  #define REG_PIO_P_IER3    (*(__O  uint32_t*)0x4800C020U) /**< \brief (PIOD) Privilege PIO Interrupt Enable Register (io_group = 0) */
  #define REG_PIO_P_IDR3    (*(__O  uint32_t*)0x4800C024U) /**< \brief (PIOD) Privilege PIO Interrupt Disable Register (io_group = 0) */
  #define REG_PIO_P_IMR3    (*(__I  uint32_t*)0x4800C028U) /**< \brief (PIOD) Privilege PIO Interrupt Mask Register (io_group = 0) */
  #define REG_PIO_P_ISR3    (*(__I  uint32_t*)0x4800C02CU) /**< \brief (PIOD) Privilege PIO Interrupt Status Register (io_group = 0) */
  #define REG_PIO_P_SIONR3  (*(__O  uint32_t*)0x4800C030U) /**< \brief (PIOD) Privilege PIO Set I/O Non-Secure Register (io_group = 0) */
  #define REG_PIO_P_SIOSR3  (*(__O  uint32_t*)0x4800C034U) /**< \brief (PIOD) Privilege PIO Set I/O Secure Register (io_group = 0) */
  #define REG_PIO_P_IOSSR3  (*(__I  uint32_t*)0x4800C038U) /**< \brief (PIOD) Privilege PIO I/O Security Status Register (io_group = 0) */
  #define REG_PIO_P_IOFR3   (*(__O  uint32_t*)0x4800C03CU) /**< \brief (PIOD) Privilege PIO I/O Freeze Configuration Register (io_group = 0) */
  #define REG_PIO_P_SCDR3   (*(__IO uint32_t*)0x4800C500U) /**< \brief (PIOD) Privilege PIO Slow Clock Divider Debouncing Register */
  #define REG_PIO_P_WPMR3   (*(__IO uint32_t*)0x4800C5E0U) /**< \brief (PIOD) Privilege PIO Write Protection Mode Register */
  #define REG_PIO_P_WPSR3   (*(__I  uint32_t*)0x4800C5E4U) /**< \brief (PIOD) Privilege PIO Write Protection Status Register */

#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_PIO0A_INSTANCE_ */
