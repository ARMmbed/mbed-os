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

#ifndef _PIC32CX_IPC_COMPONENT_
#define _PIC32CX_IPC_COMPONENT_

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR Inter-processor Communication */
/* ============================================================================= */
/** \addtogroup PIC32CX_IPC Inter-processor Communication */
/*@{*/

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
/** \brief Ipc hardware registers */
typedef struct {
  __O  uint32_t IPC_ISCR; /**< \brief (Ipc Offset: 0x0) Interrupt Set Command Register */
  __O  uint32_t IPC_ICCR; /**< \brief (Ipc Offset: 0x4) Interrupt Clear Command Register */
  __I  uint32_t IPC_IPR;  /**< \brief (Ipc Offset: 0x8) Interrupt Pending Register */
  __O  uint32_t IPC_IECR; /**< \brief (Ipc Offset: 0xC) Interrupt Enable Command Register */
  __O  uint32_t IPC_IDCR; /**< \brief (Ipc Offset: 0x10) Interrupt Disable Command Register */
  __I  uint32_t IPC_IMR;  /**< \brief (Ipc Offset: 0x14) Interrupt Mask Register */
  __I  uint32_t IPC_ISR;  /**< \brief (Ipc Offset: 0x18) Interrupt Status Register */
  __IO uint32_t IPC_WPMR; /**< \brief (Ipc Offset: 0x1C) Write Protection Mode Register */
} Ipc;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/* -------- IPC_ISCR : (IPC Offset: 0x0) Interrupt Set Command Register -------- */
#define IPC_ISCR_IRQ0 (0x1u << 0) /**< \brief (IPC_ISCR) Interrupt Set */
#define IPC_ISCR_IRQ1 (0x1u << 1) /**< \brief (IPC_ISCR) Interrupt Set */
#define IPC_ISCR_IRQ2 (0x1u << 2) /**< \brief (IPC_ISCR) Interrupt Set */
#define IPC_ISCR_IRQ3 (0x1u << 3) /**< \brief (IPC_ISCR) Interrupt Set */
#define IPC_ISCR_IRQ4 (0x1u << 4) /**< \brief (IPC_ISCR) Interrupt Set */
#define IPC_ISCR_IRQ5 (0x1u << 5) /**< \brief (IPC_ISCR) Interrupt Set */
#define IPC_ISCR_IRQ6 (0x1u << 6) /**< \brief (IPC_ISCR) Interrupt Set */
#define IPC_ISCR_IRQ7 (0x1u << 7) /**< \brief (IPC_ISCR) Interrupt Set */
#define IPC_ISCR_IRQ8 (0x1u << 8) /**< \brief (IPC_ISCR) Interrupt Set */
#define IPC_ISCR_IRQ9 (0x1u << 9) /**< \brief (IPC_ISCR) Interrupt Set */
#define IPC_ISCR_IRQ10 (0x1u << 10) /**< \brief (IPC_ISCR) Interrupt Set */
#define IPC_ISCR_IRQ11 (0x1u << 11) /**< \brief (IPC_ISCR) Interrupt Set */
#define IPC_ISCR_IRQ12 (0x1u << 12) /**< \brief (IPC_ISCR) Interrupt Set */
#define IPC_ISCR_IRQ13 (0x1u << 13) /**< \brief (IPC_ISCR) Interrupt Set */
#define IPC_ISCR_IRQ14 (0x1u << 14) /**< \brief (IPC_ISCR) Interrupt Set */
#define IPC_ISCR_IRQ15 (0x1u << 15) /**< \brief (IPC_ISCR) Interrupt Set */
#define IPC_ISCR_IRQ16 (0x1u << 16) /**< \brief (IPC_ISCR) Interrupt Set */
#define IPC_ISCR_IRQ17 (0x1u << 17) /**< \brief (IPC_ISCR) Interrupt Set */
#define IPC_ISCR_IRQ18 (0x1u << 18) /**< \brief (IPC_ISCR) Interrupt Set */
#define IPC_ISCR_IRQ19 (0x1u << 19) /**< \brief (IPC_ISCR) Interrupt Set */
#define IPC_ISCR_IRQ20 (0x1u << 20) /**< \brief (IPC_ISCR) Interrupt Set */
#define IPC_ISCR_IRQ21 (0x1u << 21) /**< \brief (IPC_ISCR) Interrupt Set */
#define IPC_ISCR_IRQ22 (0x1u << 22) /**< \brief (IPC_ISCR) Interrupt Set */
#define IPC_ISCR_IRQ23 (0x1u << 23) /**< \brief (IPC_ISCR) Interrupt Set */
#define IPC_ISCR_IRQ24 (0x1u << 24) /**< \brief (IPC_ISCR) Interrupt Set */
#define IPC_ISCR_IRQ25 (0x1u << 25) /**< \brief (IPC_ISCR) Interrupt Set */
#define IPC_ISCR_IRQ26 (0x1u << 26) /**< \brief (IPC_ISCR) Interrupt Set */
#define IPC_ISCR_IRQ27 (0x1u << 27) /**< \brief (IPC_ISCR) Interrupt Set */
#define IPC_ISCR_IRQ28 (0x1u << 28) /**< \brief (IPC_ISCR) Interrupt Set */
#define IPC_ISCR_IRQ29 (0x1u << 29) /**< \brief (IPC_ISCR) Interrupt Set */
#define IPC_ISCR_IRQ30 (0x1u << 30) /**< \brief (IPC_ISCR) Interrupt Set */
#define IPC_ISCR_IRQ31 (0x1u << 31) /**< \brief (IPC_ISCR) Interrupt Set */
/* -------- IPC_ICCR : (IPC Offset: 0x4) Interrupt Clear Command Register -------- */
#define IPC_ICCR_IRQ0 (0x1u << 0) /**< \brief (IPC_ICCR) Interrupt Clear */
#define IPC_ICCR_IRQ1 (0x1u << 1) /**< \brief (IPC_ICCR) Interrupt Clear */
#define IPC_ICCR_IRQ2 (0x1u << 2) /**< \brief (IPC_ICCR) Interrupt Clear */
#define IPC_ICCR_IRQ3 (0x1u << 3) /**< \brief (IPC_ICCR) Interrupt Clear */
#define IPC_ICCR_IRQ4 (0x1u << 4) /**< \brief (IPC_ICCR) Interrupt Clear */
#define IPC_ICCR_IRQ5 (0x1u << 5) /**< \brief (IPC_ICCR) Interrupt Clear */
#define IPC_ICCR_IRQ6 (0x1u << 6) /**< \brief (IPC_ICCR) Interrupt Clear */
#define IPC_ICCR_IRQ7 (0x1u << 7) /**< \brief (IPC_ICCR) Interrupt Clear */
#define IPC_ICCR_IRQ8 (0x1u << 8) /**< \brief (IPC_ICCR) Interrupt Clear */
#define IPC_ICCR_IRQ9 (0x1u << 9) /**< \brief (IPC_ICCR) Interrupt Clear */
#define IPC_ICCR_IRQ10 (0x1u << 10) /**< \brief (IPC_ICCR) Interrupt Clear */
#define IPC_ICCR_IRQ11 (0x1u << 11) /**< \brief (IPC_ICCR) Interrupt Clear */
#define IPC_ICCR_IRQ12 (0x1u << 12) /**< \brief (IPC_ICCR) Interrupt Clear */
#define IPC_ICCR_IRQ13 (0x1u << 13) /**< \brief (IPC_ICCR) Interrupt Clear */
#define IPC_ICCR_IRQ14 (0x1u << 14) /**< \brief (IPC_ICCR) Interrupt Clear */
#define IPC_ICCR_IRQ15 (0x1u << 15) /**< \brief (IPC_ICCR) Interrupt Clear */
#define IPC_ICCR_IRQ16 (0x1u << 16) /**< \brief (IPC_ICCR) Interrupt Clear */
#define IPC_ICCR_IRQ17 (0x1u << 17) /**< \brief (IPC_ICCR) Interrupt Clear */
#define IPC_ICCR_IRQ18 (0x1u << 18) /**< \brief (IPC_ICCR) Interrupt Clear */
#define IPC_ICCR_IRQ19 (0x1u << 19) /**< \brief (IPC_ICCR) Interrupt Clear */
#define IPC_ICCR_IRQ20 (0x1u << 20) /**< \brief (IPC_ICCR) Interrupt Clear */
#define IPC_ICCR_IRQ21 (0x1u << 21) /**< \brief (IPC_ICCR) Interrupt Clear */
#define IPC_ICCR_IRQ22 (0x1u << 22) /**< \brief (IPC_ICCR) Interrupt Clear */
#define IPC_ICCR_IRQ23 (0x1u << 23) /**< \brief (IPC_ICCR) Interrupt Clear */
#define IPC_ICCR_IRQ24 (0x1u << 24) /**< \brief (IPC_ICCR) Interrupt Clear */
#define IPC_ICCR_IRQ25 (0x1u << 25) /**< \brief (IPC_ICCR) Interrupt Clear */
#define IPC_ICCR_IRQ26 (0x1u << 26) /**< \brief (IPC_ICCR) Interrupt Clear */
#define IPC_ICCR_IRQ27 (0x1u << 27) /**< \brief (IPC_ICCR) Interrupt Clear */
#define IPC_ICCR_IRQ28 (0x1u << 28) /**< \brief (IPC_ICCR) Interrupt Clear */
#define IPC_ICCR_IRQ29 (0x1u << 29) /**< \brief (IPC_ICCR) Interrupt Clear */
#define IPC_ICCR_IRQ30 (0x1u << 30) /**< \brief (IPC_ICCR) Interrupt Clear */
#define IPC_ICCR_IRQ31 (0x1u << 31) /**< \brief (IPC_ICCR) Interrupt Clear */
/* -------- IPC_IPR : (IPC Offset: 0x8) Interrupt Pending Register -------- */
#define IPC_IPR_IRQ0 (0x1u << 0) /**< \brief (IPC_IPR) Interrupt Pending */
#define IPC_IPR_IRQ1 (0x1u << 1) /**< \brief (IPC_IPR) Interrupt Pending */
#define IPC_IPR_IRQ2 (0x1u << 2) /**< \brief (IPC_IPR) Interrupt Pending */
#define IPC_IPR_IRQ3 (0x1u << 3) /**< \brief (IPC_IPR) Interrupt Pending */
#define IPC_IPR_IRQ4 (0x1u << 4) /**< \brief (IPC_IPR) Interrupt Pending */
#define IPC_IPR_IRQ5 (0x1u << 5) /**< \brief (IPC_IPR) Interrupt Pending */
#define IPC_IPR_IRQ6 (0x1u << 6) /**< \brief (IPC_IPR) Interrupt Pending */
#define IPC_IPR_IRQ7 (0x1u << 7) /**< \brief (IPC_IPR) Interrupt Pending */
#define IPC_IPR_IRQ8 (0x1u << 8) /**< \brief (IPC_IPR) Interrupt Pending */
#define IPC_IPR_IRQ9 (0x1u << 9) /**< \brief (IPC_IPR) Interrupt Pending */
#define IPC_IPR_IRQ10 (0x1u << 10) /**< \brief (IPC_IPR) Interrupt Pending */
#define IPC_IPR_IRQ11 (0x1u << 11) /**< \brief (IPC_IPR) Interrupt Pending */
#define IPC_IPR_IRQ12 (0x1u << 12) /**< \brief (IPC_IPR) Interrupt Pending */
#define IPC_IPR_IRQ13 (0x1u << 13) /**< \brief (IPC_IPR) Interrupt Pending */
#define IPC_IPR_IRQ14 (0x1u << 14) /**< \brief (IPC_IPR) Interrupt Pending */
#define IPC_IPR_IRQ15 (0x1u << 15) /**< \brief (IPC_IPR) Interrupt Pending */
#define IPC_IPR_IRQ16 (0x1u << 16) /**< \brief (IPC_IPR) Interrupt Pending */
#define IPC_IPR_IRQ17 (0x1u << 17) /**< \brief (IPC_IPR) Interrupt Pending */
#define IPC_IPR_IRQ18 (0x1u << 18) /**< \brief (IPC_IPR) Interrupt Pending */
#define IPC_IPR_IRQ19 (0x1u << 19) /**< \brief (IPC_IPR) Interrupt Pending */
#define IPC_IPR_IRQ20 (0x1u << 20) /**< \brief (IPC_IPR) Interrupt Pending */
#define IPC_IPR_IRQ21 (0x1u << 21) /**< \brief (IPC_IPR) Interrupt Pending */
#define IPC_IPR_IRQ22 (0x1u << 22) /**< \brief (IPC_IPR) Interrupt Pending */
#define IPC_IPR_IRQ23 (0x1u << 23) /**< \brief (IPC_IPR) Interrupt Pending */
#define IPC_IPR_IRQ24 (0x1u << 24) /**< \brief (IPC_IPR) Interrupt Pending */
#define IPC_IPR_IRQ25 (0x1u << 25) /**< \brief (IPC_IPR) Interrupt Pending */
#define IPC_IPR_IRQ26 (0x1u << 26) /**< \brief (IPC_IPR) Interrupt Pending */
#define IPC_IPR_IRQ27 (0x1u << 27) /**< \brief (IPC_IPR) Interrupt Pending */
#define IPC_IPR_IRQ28 (0x1u << 28) /**< \brief (IPC_IPR) Interrupt Pending */
#define IPC_IPR_IRQ29 (0x1u << 29) /**< \brief (IPC_IPR) Interrupt Pending */
#define IPC_IPR_IRQ30 (0x1u << 30) /**< \brief (IPC_IPR) Interrupt Pending */
#define IPC_IPR_IRQ31 (0x1u << 31) /**< \brief (IPC_IPR) Interrupt Pending */
/* -------- IPC_IECR : (IPC Offset: 0xC) Interrupt Enable Command Register -------- */
#define IPC_IECR_IRQ0 (0x1u << 0) /**< \brief (IPC_IECR) Interrupt Enable */
#define IPC_IECR_IRQ1 (0x1u << 1) /**< \brief (IPC_IECR) Interrupt Enable */
#define IPC_IECR_IRQ2 (0x1u << 2) /**< \brief (IPC_IECR) Interrupt Enable */
#define IPC_IECR_IRQ3 (0x1u << 3) /**< \brief (IPC_IECR) Interrupt Enable */
#define IPC_IECR_IRQ4 (0x1u << 4) /**< \brief (IPC_IECR) Interrupt Enable */
#define IPC_IECR_IRQ5 (0x1u << 5) /**< \brief (IPC_IECR) Interrupt Enable */
#define IPC_IECR_IRQ6 (0x1u << 6) /**< \brief (IPC_IECR) Interrupt Enable */
#define IPC_IECR_IRQ7 (0x1u << 7) /**< \brief (IPC_IECR) Interrupt Enable */
#define IPC_IECR_IRQ8 (0x1u << 8) /**< \brief (IPC_IECR) Interrupt Enable */
#define IPC_IECR_IRQ9 (0x1u << 9) /**< \brief (IPC_IECR) Interrupt Enable */
#define IPC_IECR_IRQ10 (0x1u << 10) /**< \brief (IPC_IECR) Interrupt Enable */
#define IPC_IECR_IRQ11 (0x1u << 11) /**< \brief (IPC_IECR) Interrupt Enable */
#define IPC_IECR_IRQ12 (0x1u << 12) /**< \brief (IPC_IECR) Interrupt Enable */
#define IPC_IECR_IRQ13 (0x1u << 13) /**< \brief (IPC_IECR) Interrupt Enable */
#define IPC_IECR_IRQ14 (0x1u << 14) /**< \brief (IPC_IECR) Interrupt Enable */
#define IPC_IECR_IRQ15 (0x1u << 15) /**< \brief (IPC_IECR) Interrupt Enable */
#define IPC_IECR_IRQ16 (0x1u << 16) /**< \brief (IPC_IECR) Interrupt Enable */
#define IPC_IECR_IRQ17 (0x1u << 17) /**< \brief (IPC_IECR) Interrupt Enable */
#define IPC_IECR_IRQ18 (0x1u << 18) /**< \brief (IPC_IECR) Interrupt Enable */
#define IPC_IECR_IRQ19 (0x1u << 19) /**< \brief (IPC_IECR) Interrupt Enable */
#define IPC_IECR_IRQ20 (0x1u << 20) /**< \brief (IPC_IECR) Interrupt Enable */
#define IPC_IECR_IRQ21 (0x1u << 21) /**< \brief (IPC_IECR) Interrupt Enable */
#define IPC_IECR_IRQ22 (0x1u << 22) /**< \brief (IPC_IECR) Interrupt Enable */
#define IPC_IECR_IRQ23 (0x1u << 23) /**< \brief (IPC_IECR) Interrupt Enable */
#define IPC_IECR_IRQ24 (0x1u << 24) /**< \brief (IPC_IECR) Interrupt Enable */
#define IPC_IECR_IRQ25 (0x1u << 25) /**< \brief (IPC_IECR) Interrupt Enable */
#define IPC_IECR_IRQ26 (0x1u << 26) /**< \brief (IPC_IECR) Interrupt Enable */
#define IPC_IECR_IRQ27 (0x1u << 27) /**< \brief (IPC_IECR) Interrupt Enable */
#define IPC_IECR_IRQ28 (0x1u << 28) /**< \brief (IPC_IECR) Interrupt Enable */
#define IPC_IECR_IRQ29 (0x1u << 29) /**< \brief (IPC_IECR) Interrupt Enable */
#define IPC_IECR_IRQ30 (0x1u << 30) /**< \brief (IPC_IECR) Interrupt Enable */
#define IPC_IECR_IRQ31 (0x1u << 31) /**< \brief (IPC_IECR) Interrupt Enable */
/* -------- IPC_IDCR : (IPC Offset: 0x10) Interrupt Disable Command Register -------- */
#define IPC_IDCR_IRQ0 (0x1u << 0) /**< \brief (IPC_IDCR) Interrupt Disable */
#define IPC_IDCR_IRQ1 (0x1u << 1) /**< \brief (IPC_IDCR) Interrupt Disable */
#define IPC_IDCR_IRQ2 (0x1u << 2) /**< \brief (IPC_IDCR) Interrupt Disable */
#define IPC_IDCR_IRQ3 (0x1u << 3) /**< \brief (IPC_IDCR) Interrupt Disable */
#define IPC_IDCR_IRQ4 (0x1u << 4) /**< \brief (IPC_IDCR) Interrupt Disable */
#define IPC_IDCR_IRQ5 (0x1u << 5) /**< \brief (IPC_IDCR) Interrupt Disable */
#define IPC_IDCR_IRQ6 (0x1u << 6) /**< \brief (IPC_IDCR) Interrupt Disable */
#define IPC_IDCR_IRQ7 (0x1u << 7) /**< \brief (IPC_IDCR) Interrupt Disable */
#define IPC_IDCR_IRQ8 (0x1u << 8) /**< \brief (IPC_IDCR) Interrupt Disable */
#define IPC_IDCR_IRQ9 (0x1u << 9) /**< \brief (IPC_IDCR) Interrupt Disable */
#define IPC_IDCR_IRQ10 (0x1u << 10) /**< \brief (IPC_IDCR) Interrupt Disable */
#define IPC_IDCR_IRQ11 (0x1u << 11) /**< \brief (IPC_IDCR) Interrupt Disable */
#define IPC_IDCR_IRQ12 (0x1u << 12) /**< \brief (IPC_IDCR) Interrupt Disable */
#define IPC_IDCR_IRQ13 (0x1u << 13) /**< \brief (IPC_IDCR) Interrupt Disable */
#define IPC_IDCR_IRQ14 (0x1u << 14) /**< \brief (IPC_IDCR) Interrupt Disable */
#define IPC_IDCR_IRQ15 (0x1u << 15) /**< \brief (IPC_IDCR) Interrupt Disable */
#define IPC_IDCR_IRQ16 (0x1u << 16) /**< \brief (IPC_IDCR) Interrupt Disable */
#define IPC_IDCR_IRQ17 (0x1u << 17) /**< \brief (IPC_IDCR) Interrupt Disable */
#define IPC_IDCR_IRQ18 (0x1u << 18) /**< \brief (IPC_IDCR) Interrupt Disable */
#define IPC_IDCR_IRQ19 (0x1u << 19) /**< \brief (IPC_IDCR) Interrupt Disable */
#define IPC_IDCR_IRQ20 (0x1u << 20) /**< \brief (IPC_IDCR) Interrupt Disable */
#define IPC_IDCR_IRQ21 (0x1u << 21) /**< \brief (IPC_IDCR) Interrupt Disable */
#define IPC_IDCR_IRQ22 (0x1u << 22) /**< \brief (IPC_IDCR) Interrupt Disable */
#define IPC_IDCR_IRQ23 (0x1u << 23) /**< \brief (IPC_IDCR) Interrupt Disable */
#define IPC_IDCR_IRQ24 (0x1u << 24) /**< \brief (IPC_IDCR) Interrupt Disable */
#define IPC_IDCR_IRQ25 (0x1u << 25) /**< \brief (IPC_IDCR) Interrupt Disable */
#define IPC_IDCR_IRQ26 (0x1u << 26) /**< \brief (IPC_IDCR) Interrupt Disable */
#define IPC_IDCR_IRQ27 (0x1u << 27) /**< \brief (IPC_IDCR) Interrupt Disable */
#define IPC_IDCR_IRQ28 (0x1u << 28) /**< \brief (IPC_IDCR) Interrupt Disable */
#define IPC_IDCR_IRQ29 (0x1u << 29) /**< \brief (IPC_IDCR) Interrupt Disable */
#define IPC_IDCR_IRQ30 (0x1u << 30) /**< \brief (IPC_IDCR) Interrupt Disable */
#define IPC_IDCR_IRQ31 (0x1u << 31) /**< \brief (IPC_IDCR) Interrupt Disable */
/* -------- IPC_IMR : (IPC Offset: 0x14) Interrupt Mask Register -------- */
#define IPC_IMR_IRQ0 (0x1u << 0) /**< \brief (IPC_IMR) Interrupt Mask */
#define IPC_IMR_IRQ1 (0x1u << 1) /**< \brief (IPC_IMR) Interrupt Mask */
#define IPC_IMR_IRQ2 (0x1u << 2) /**< \brief (IPC_IMR) Interrupt Mask */
#define IPC_IMR_IRQ3 (0x1u << 3) /**< \brief (IPC_IMR) Interrupt Mask */
#define IPC_IMR_IRQ4 (0x1u << 4) /**< \brief (IPC_IMR) Interrupt Mask */
#define IPC_IMR_IRQ5 (0x1u << 5) /**< \brief (IPC_IMR) Interrupt Mask */
#define IPC_IMR_IRQ6 (0x1u << 6) /**< \brief (IPC_IMR) Interrupt Mask */
#define IPC_IMR_IRQ7 (0x1u << 7) /**< \brief (IPC_IMR) Interrupt Mask */
#define IPC_IMR_IRQ8 (0x1u << 8) /**< \brief (IPC_IMR) Interrupt Mask */
#define IPC_IMR_IRQ9 (0x1u << 9) /**< \brief (IPC_IMR) Interrupt Mask */
#define IPC_IMR_IRQ10 (0x1u << 10) /**< \brief (IPC_IMR) Interrupt Mask */
#define IPC_IMR_IRQ11 (0x1u << 11) /**< \brief (IPC_IMR) Interrupt Mask */
#define IPC_IMR_IRQ12 (0x1u << 12) /**< \brief (IPC_IMR) Interrupt Mask */
#define IPC_IMR_IRQ13 (0x1u << 13) /**< \brief (IPC_IMR) Interrupt Mask */
#define IPC_IMR_IRQ14 (0x1u << 14) /**< \brief (IPC_IMR) Interrupt Mask */
#define IPC_IMR_IRQ15 (0x1u << 15) /**< \brief (IPC_IMR) Interrupt Mask */
#define IPC_IMR_IRQ16 (0x1u << 16) /**< \brief (IPC_IMR) Interrupt Mask */
#define IPC_IMR_IRQ17 (0x1u << 17) /**< \brief (IPC_IMR) Interrupt Mask */
#define IPC_IMR_IRQ18 (0x1u << 18) /**< \brief (IPC_IMR) Interrupt Mask */
#define IPC_IMR_IRQ19 (0x1u << 19) /**< \brief (IPC_IMR) Interrupt Mask */
#define IPC_IMR_IRQ20 (0x1u << 20) /**< \brief (IPC_IMR) Interrupt Mask */
#define IPC_IMR_IRQ21 (0x1u << 21) /**< \brief (IPC_IMR) Interrupt Mask */
#define IPC_IMR_IRQ22 (0x1u << 22) /**< \brief (IPC_IMR) Interrupt Mask */
#define IPC_IMR_IRQ23 (0x1u << 23) /**< \brief (IPC_IMR) Interrupt Mask */
#define IPC_IMR_IRQ24 (0x1u << 24) /**< \brief (IPC_IMR) Interrupt Mask */
#define IPC_IMR_IRQ25 (0x1u << 25) /**< \brief (IPC_IMR) Interrupt Mask */
#define IPC_IMR_IRQ26 (0x1u << 26) /**< \brief (IPC_IMR) Interrupt Mask */
#define IPC_IMR_IRQ27 (0x1u << 27) /**< \brief (IPC_IMR) Interrupt Mask */
#define IPC_IMR_IRQ28 (0x1u << 28) /**< \brief (IPC_IMR) Interrupt Mask */
#define IPC_IMR_IRQ29 (0x1u << 29) /**< \brief (IPC_IMR) Interrupt Mask */
#define IPC_IMR_IRQ30 (0x1u << 30) /**< \brief (IPC_IMR) Interrupt Mask */
#define IPC_IMR_IRQ31 (0x1u << 31) /**< \brief (IPC_IMR) Interrupt Mask */
/* -------- IPC_ISR : (IPC Offset: 0x18) Interrupt Status Register -------- */
#define IPC_ISR_IRQ0 (0x1u << 0) /**< \brief (IPC_ISR) Current Interrupt Identifier */
#define IPC_ISR_IRQ1 (0x1u << 1) /**< \brief (IPC_ISR) Current Interrupt Identifier */
#define IPC_ISR_IRQ2 (0x1u << 2) /**< \brief (IPC_ISR) Current Interrupt Identifier */
#define IPC_ISR_IRQ3 (0x1u << 3) /**< \brief (IPC_ISR) Current Interrupt Identifier */
#define IPC_ISR_IRQ4 (0x1u << 4) /**< \brief (IPC_ISR) Current Interrupt Identifier */
#define IPC_ISR_IRQ5 (0x1u << 5) /**< \brief (IPC_ISR) Current Interrupt Identifier */
#define IPC_ISR_IRQ6 (0x1u << 6) /**< \brief (IPC_ISR) Current Interrupt Identifier */
#define IPC_ISR_IRQ7 (0x1u << 7) /**< \brief (IPC_ISR) Current Interrupt Identifier */
#define IPC_ISR_IRQ8 (0x1u << 8) /**< \brief (IPC_ISR) Current Interrupt Identifier */
#define IPC_ISR_IRQ9 (0x1u << 9) /**< \brief (IPC_ISR) Current Interrupt Identifier */
#define IPC_ISR_IRQ10 (0x1u << 10) /**< \brief (IPC_ISR) Current Interrupt Identifier */
#define IPC_ISR_IRQ11 (0x1u << 11) /**< \brief (IPC_ISR) Current Interrupt Identifier */
#define IPC_ISR_IRQ12 (0x1u << 12) /**< \brief (IPC_ISR) Current Interrupt Identifier */
#define IPC_ISR_IRQ13 (0x1u << 13) /**< \brief (IPC_ISR) Current Interrupt Identifier */
#define IPC_ISR_IRQ14 (0x1u << 14) /**< \brief (IPC_ISR) Current Interrupt Identifier */
#define IPC_ISR_IRQ15 (0x1u << 15) /**< \brief (IPC_ISR) Current Interrupt Identifier */
#define IPC_ISR_IRQ16 (0x1u << 16) /**< \brief (IPC_ISR) Current Interrupt Identifier */
#define IPC_ISR_IRQ17 (0x1u << 17) /**< \brief (IPC_ISR) Current Interrupt Identifier */
#define IPC_ISR_IRQ18 (0x1u << 18) /**< \brief (IPC_ISR) Current Interrupt Identifier */
#define IPC_ISR_IRQ19 (0x1u << 19) /**< \brief (IPC_ISR) Current Interrupt Identifier */
#define IPC_ISR_IRQ20 (0x1u << 20) /**< \brief (IPC_ISR) Current Interrupt Identifier */
#define IPC_ISR_IRQ21 (0x1u << 21) /**< \brief (IPC_ISR) Current Interrupt Identifier */
#define IPC_ISR_IRQ22 (0x1u << 22) /**< \brief (IPC_ISR) Current Interrupt Identifier */
#define IPC_ISR_IRQ23 (0x1u << 23) /**< \brief (IPC_ISR) Current Interrupt Identifier */
#define IPC_ISR_IRQ24 (0x1u << 24) /**< \brief (IPC_ISR) Current Interrupt Identifier */
#define IPC_ISR_IRQ25 (0x1u << 25) /**< \brief (IPC_ISR) Current Interrupt Identifier */
#define IPC_ISR_IRQ26 (0x1u << 26) /**< \brief (IPC_ISR) Current Interrupt Identifier */
#define IPC_ISR_IRQ27 (0x1u << 27) /**< \brief (IPC_ISR) Current Interrupt Identifier */
#define IPC_ISR_IRQ28 (0x1u << 28) /**< \brief (IPC_ISR) Current Interrupt Identifier */
#define IPC_ISR_IRQ29 (0x1u << 29) /**< \brief (IPC_ISR) Current Interrupt Identifier */
#define IPC_ISR_IRQ30 (0x1u << 30) /**< \brief (IPC_ISR) Current Interrupt Identifier */
#define IPC_ISR_IRQ31 (0x1u << 31) /**< \brief (IPC_ISR) Current Interrupt Identifier */
/* -------- IPC_WPMR : (IPC Offset: 0x1C) Write Protection Mode Register -------- */
#define IPC_WPMR_WPITEN (0x1u << 1) /**< \brief (IPC_WPMR) Write Protection Interrupt Enable */
#define IPC_WPMR_WPKEY_Pos 8
#define IPC_WPMR_WPKEY_Msk (0xffffffu << IPC_WPMR_WPKEY_Pos) /**< \brief (IPC_WPMR) Write Protection Key */
#define IPC_WPMR_WPKEY(value) ((IPC_WPMR_WPKEY_Msk & ((value) << IPC_WPMR_WPKEY_Pos)))
#define   IPC_WPMR_WPKEY_PASSWD (0x495043u << 8) /**< \brief (IPC_WPMR) Writing any other value in this field aborts the write operation of the WPEN bit. Always reads as 0. */

/*@}*/


#endif /* _PIC32CX_IPC_COMPONENT_ */
