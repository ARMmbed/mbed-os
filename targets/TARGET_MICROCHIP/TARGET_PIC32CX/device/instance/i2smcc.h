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

#ifndef _PIC32CX_I2SMCC_INSTANCE_
#define _PIC32CX_I2SMCC_INSTANCE_

/* ========== Register definition for I2SMCC peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
  #define REG_I2SMCC_CR                     (0x48024000U) /**< \brief (I2SMCC) Control Register */
  #define REG_I2SMCC_MRA                    (0x48024004U) /**< \brief (I2SMCC) Mode Register A */
  #define REG_I2SMCC_MRB                    (0x48024008U) /**< \brief (I2SMCC) Mode Register B */
  #define REG_I2SMCC_SR                     (0x4802400CU) /**< \brief (I2SMCC) Status Register */
  #define REG_I2SMCC_IERA                   (0x48024010U) /**< \brief (I2SMCC) Interrupt Enable Register A */
  #define REG_I2SMCC_IDRA                   (0x48024014U) /**< \brief (I2SMCC) Interrupt Disable Register A */
  #define REG_I2SMCC_IMRA                   (0x48024018U) /**< \brief (I2SMCC) Interrupt Mask Register A */
  #define REG_I2SMCC_ISRA                   (0x4802401CU) /**< \brief (I2SMCC) Interrupt Status Register A */
  #define REG_I2SMCC_IERB                   (0x48024020U) /**< \brief (I2SMCC) Interrupt Enable Register B */
  #define REG_I2SMCC_IDRB                   (0x48024024U) /**< \brief (I2SMCC) Interrupt Disable Register B */
  #define REG_I2SMCC_IMRB                   (0x48024028U) /**< \brief (I2SMCC) Interrupt Mask Register B */
  #define REG_I2SMCC_ISRB                   (0x4802402CU) /**< \brief (I2SMCC) Interrupt Status Register B */
  #define REG_I2SMCC_RHR                    (0x48024030U) /**< \brief (I2SMCC) Receiver Holding Register */
  #define REG_I2SMCC_THR                    (0x48024034U) /**< \brief (I2SMCC) Transmitter Holding Register */
  #define REG_I2SMCC_RHL0R                  (0x48024040U) /**< \brief (I2SMCC) Receiver Holding Left 0 Register */
  #define REG_I2SMCC_RHR0R                  (0x48024044U) /**< \brief (I2SMCC) Receiver Holding Right 0 Register */
  #define REG_I2SMCC_RHL1R                  (0x48024048U) /**< \brief (I2SMCC) Receiver Holding Left 1 Register */
  #define REG_I2SMCC_RHR1R                  (0x4802404CU) /**< \brief (I2SMCC) Receiver Holding Right 1 Register */
  #define REG_I2SMCC_RHL2R                  (0x48024050U) /**< \brief (I2SMCC) Receiver Holding Left 2 Register */
  #define REG_I2SMCC_RHR2R                  (0x48024054U) /**< \brief (I2SMCC) Receiver Holding Right 2 Register */
  #define REG_I2SMCC_RHL3R                  (0x48024058U) /**< \brief (I2SMCC) Receiver Holding Left 3 Register */
  #define REG_I2SMCC_RHR3R                  (0x4802405CU) /**< \brief (I2SMCC) Receiver Holding Right 3 Register */
  #define REG_I2SMCC_THL0R                  (0x48024060U) /**< \brief (I2SMCC) Transmitter Holding Left 0 Register */
  #define REG_I2SMCC_THR0R                  (0x48024064U) /**< \brief (I2SMCC) Transmitter Holding Right 0 Register */
  #define REG_I2SMCC_THL1R                  (0x48024068U) /**< \brief (I2SMCC) Transmitter Holding Left 1 Register */
  #define REG_I2SMCC_THR1R                  (0x4802406CU) /**< \brief (I2SMCC) Transmitter Holding Right 1 Register */
  #define REG_I2SMCC_THL2R                  (0x48024070U) /**< \brief (I2SMCC) Transmitter Holding Left 2 Register */
  #define REG_I2SMCC_THR2R                  (0x48024074U) /**< \brief (I2SMCC) Transmitter Holding Right 2 Register */
  #define REG_I2SMCC_THL3R                  (0x48024078U) /**< \brief (I2SMCC) Transmitter Holding Left 3 Register */
  #define REG_I2SMCC_THR3R                  (0x4802407CU) /**< \brief (I2SMCC) Transmitter Holding Right 3 Register */
  #define REG_I2SMCC_WPMR                   (0x480240E4U) /**< \brief (I2SMCC) Write Protection Mode Register */
  #define REG_I2SMCC_WPSR                   (0x480240E8U) /**< \brief (I2SMCC) Write Protection Status Register */
  #define REG_I2SMCC_RPR                    (0x48024100U) /**< \brief (I2SMCC) Receive Pointer Register */
  #define REG_I2SMCC_RCR                    (0x48024104U) /**< \brief (I2SMCC) Receive Counter Register */
  #define REG_I2SMCC_TPR                    (0x48024108U) /**< \brief (I2SMCC) Transmit Pointer Register */
  #define REG_I2SMCC_TCR                    (0x4802410CU) /**< \brief (I2SMCC) Transmit Counter Register */
  #define REG_I2SMCC_RNPR                   (0x48024110U) /**< \brief (I2SMCC) Receive Next Pointer Register */
  #define REG_I2SMCC_RNCR                   (0x48024114U) /**< \brief (I2SMCC) Receive Next Counter Register */
  #define REG_I2SMCC_TNPR                   (0x48024118U) /**< \brief (I2SMCC) Transmit Next Pointer Register */
  #define REG_I2SMCC_TNCR                   (0x4802411CU) /**< \brief (I2SMCC) Transmit Next Counter Register */
  #define REG_I2SMCC_PTCR                   (0x48024120U) /**< \brief (I2SMCC) Transfer Control Register */
  #define REG_I2SMCC_PTSR                   (0x48024124U) /**< \brief (I2SMCC) Transfer Status Register */
  #define REG_I2SMCC_PWPMR                  (0x48024128U) /**< \brief (I2SMCC) Write Protection Mode Register */
#else
  #define REG_I2SMCC_CR    (*(__O  uint32_t*)0x48024000U) /**< \brief (I2SMCC) Control Register */
  #define REG_I2SMCC_MRA   (*(__IO uint32_t*)0x48024004U) /**< \brief (I2SMCC) Mode Register A */
  #define REG_I2SMCC_MRB   (*(__IO uint32_t*)0x48024008U) /**< \brief (I2SMCC) Mode Register B */
  #define REG_I2SMCC_SR    (*(__I  uint32_t*)0x4802400CU) /**< \brief (I2SMCC) Status Register */
  #define REG_I2SMCC_IERA  (*(__O  uint32_t*)0x48024010U) /**< \brief (I2SMCC) Interrupt Enable Register A */
  #define REG_I2SMCC_IDRA  (*(__O  uint32_t*)0x48024014U) /**< \brief (I2SMCC) Interrupt Disable Register A */
  #define REG_I2SMCC_IMRA  (*(__I  uint32_t*)0x48024018U) /**< \brief (I2SMCC) Interrupt Mask Register A */
  #define REG_I2SMCC_ISRA  (*(__I  uint32_t*)0x4802401CU) /**< \brief (I2SMCC) Interrupt Status Register A */
  #define REG_I2SMCC_IERB  (*(__O  uint32_t*)0x48024020U) /**< \brief (I2SMCC) Interrupt Enable Register B */
  #define REG_I2SMCC_IDRB  (*(__O  uint32_t*)0x48024024U) /**< \brief (I2SMCC) Interrupt Disable Register B */
  #define REG_I2SMCC_IMRB  (*(__I  uint32_t*)0x48024028U) /**< \brief (I2SMCC) Interrupt Mask Register B */
  #define REG_I2SMCC_ISRB  (*(__I  uint32_t*)0x4802402CU) /**< \brief (I2SMCC) Interrupt Status Register B */
  #define REG_I2SMCC_RHR   (*(__I  uint32_t*)0x48024030U) /**< \brief (I2SMCC) Receiver Holding Register */
  #define REG_I2SMCC_THR   (*(__O  uint32_t*)0x48024034U) /**< \brief (I2SMCC) Transmitter Holding Register */
  #define REG_I2SMCC_RHL0R (*(__I  uint32_t*)0x48024040U) /**< \brief (I2SMCC) Receiver Holding Left 0 Register */
  #define REG_I2SMCC_RHR0R (*(__I  uint32_t*)0x48024044U) /**< \brief (I2SMCC) Receiver Holding Right 0 Register */
  #define REG_I2SMCC_RHL1R (*(__I  uint32_t*)0x48024048U) /**< \brief (I2SMCC) Receiver Holding Left 1 Register */
  #define REG_I2SMCC_RHR1R (*(__I  uint32_t*)0x4802404CU) /**< \brief (I2SMCC) Receiver Holding Right 1 Register */
  #define REG_I2SMCC_RHL2R (*(__I  uint32_t*)0x48024050U) /**< \brief (I2SMCC) Receiver Holding Left 2 Register */
  #define REG_I2SMCC_RHR2R (*(__I  uint32_t*)0x48024054U) /**< \brief (I2SMCC) Receiver Holding Right 2 Register */
  #define REG_I2SMCC_RHL3R (*(__I  uint32_t*)0x48024058U) /**< \brief (I2SMCC) Receiver Holding Left 3 Register */
  #define REG_I2SMCC_RHR3R (*(__I  uint32_t*)0x4802405CU) /**< \brief (I2SMCC) Receiver Holding Right 3 Register */
  #define REG_I2SMCC_THL0R (*(__O  uint32_t*)0x48024060U) /**< \brief (I2SMCC) Transmitter Holding Left 0 Register */
  #define REG_I2SMCC_THR0R (*(__O  uint32_t*)0x48024064U) /**< \brief (I2SMCC) Transmitter Holding Right 0 Register */
  #define REG_I2SMCC_THL1R (*(__O  uint32_t*)0x48024068U) /**< \brief (I2SMCC) Transmitter Holding Left 1 Register */
  #define REG_I2SMCC_THR1R (*(__O  uint32_t*)0x4802406CU) /**< \brief (I2SMCC) Transmitter Holding Right 1 Register */
  #define REG_I2SMCC_THL2R (*(__O  uint32_t*)0x48024070U) /**< \brief (I2SMCC) Transmitter Holding Left 2 Register */
  #define REG_I2SMCC_THR2R (*(__O  uint32_t*)0x48024074U) /**< \brief (I2SMCC) Transmitter Holding Right 2 Register */
  #define REG_I2SMCC_THL3R (*(__O  uint32_t*)0x48024078U) /**< \brief (I2SMCC) Transmitter Holding Left 3 Register */
  #define REG_I2SMCC_THR3R (*(__O  uint32_t*)0x4802407CU) /**< \brief (I2SMCC) Transmitter Holding Right 3 Register */
  #define REG_I2SMCC_WPMR  (*(__IO uint32_t*)0x480240E4U) /**< \brief (I2SMCC) Write Protection Mode Register */
  #define REG_I2SMCC_WPSR  (*(__I  uint32_t*)0x480240E8U) /**< \brief (I2SMCC) Write Protection Status Register */
  #define REG_I2SMCC_RPR   (*(__IO uint32_t*)0x48024100U) /**< \brief (I2SMCC) Receive Pointer Register */
  #define REG_I2SMCC_RCR   (*(__IO uint32_t*)0x48024104U) /**< \brief (I2SMCC) Receive Counter Register */
  #define REG_I2SMCC_TPR   (*(__IO uint32_t*)0x48024108U) /**< \brief (I2SMCC) Transmit Pointer Register */
  #define REG_I2SMCC_TCR   (*(__IO uint32_t*)0x4802410CU) /**< \brief (I2SMCC) Transmit Counter Register */
  #define REG_I2SMCC_RNPR  (*(__IO uint32_t*)0x48024110U) /**< \brief (I2SMCC) Receive Next Pointer Register */
  #define REG_I2SMCC_RNCR  (*(__IO uint32_t*)0x48024114U) /**< \brief (I2SMCC) Receive Next Counter Register */
  #define REG_I2SMCC_TNPR  (*(__IO uint32_t*)0x48024118U) /**< \brief (I2SMCC) Transmit Next Pointer Register */
  #define REG_I2SMCC_TNCR  (*(__IO uint32_t*)0x4802411CU) /**< \brief (I2SMCC) Transmit Next Counter Register */
  #define REG_I2SMCC_PTCR  (*(__O  uint32_t*)0x48024120U) /**< \brief (I2SMCC) Transfer Control Register */
  #define REG_I2SMCC_PTSR  (*(__I  uint32_t*)0x48024124U) /**< \brief (I2SMCC) Transfer Status Register */
  #define REG_I2SMCC_PWPMR (*(__IO uint32_t*)0x48024128U) /**< \brief (I2SMCC) Write Protection Mode Register */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#endif /* _PIC32CX_I2SMCC_INSTANCE_ */
