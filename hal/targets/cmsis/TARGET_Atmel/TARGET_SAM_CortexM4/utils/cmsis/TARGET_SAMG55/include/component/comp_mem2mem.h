/**
 * \file
 *
 * Copyright (c) 2014-2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef _SAMG55_MEM2MEM_COMPONENT_
#define _SAMG55_MEM2MEM_COMPONENT_

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR Memory to Memory */
/* ============================================================================= */
/** \addtogroup SAMG55_MEM2MEM Memory to Memory */
/*@{*/

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
/** \brief Mem2mem hardware registers */
typedef struct {
    __IO uint32_t MEM2MEM_THR;   /**< \brief (Mem2mem Offset: 0x00) Memory to Memory Transfer Holding Register */
    __IO uint32_t MEM2MEM_MR;    /**< \brief (Mem2mem Offset: 0x04) Memory to Memory Mode Register */
    __O  uint32_t MEM2MEM_IER;   /**< \brief (Mem2mem Offset: 0x08) Memory to Memory Interrupt Enable Register */
    __O  uint32_t MEM2MEM_IDR;   /**< \brief (Mem2mem Offset: 0x0C) Memory to Memory Interrupt Disable Register */
    __I  uint32_t MEM2MEM_IMR;   /**< \brief (Mem2mem Offset: 0x10) Memory to Memory Interrupt Mask Register */
    __I  uint32_t MEM2MEM_ISR;   /**< \brief (Mem2mem Offset: 0x14) Memory to Memory Interrupt Status Register */
    __I  uint32_t Reserved1[58];
    __IO uint32_t MEM2MEM_RPR;   /**< \brief (Mem2mem Offset: 0x100) Receive Pointer Register */
    __IO uint32_t MEM2MEM_RCR;   /**< \brief (Mem2mem Offset: 0x104) Receive Counter Register */
    __IO uint32_t MEM2MEM_TPR;   /**< \brief (Mem2mem Offset: 0x108) Transmit Pointer Register */
    __IO uint32_t MEM2MEM_TCR;   /**< \brief (Mem2mem Offset: 0x10C) Transmit Counter Register */
    __IO uint32_t MEM2MEM_RNPR;  /**< \brief (Mem2mem Offset: 0x110) Receive Next Pointer Register */
    __IO uint32_t MEM2MEM_RNCR;  /**< \brief (Mem2mem Offset: 0x114) Receive Next Counter Register */
    __IO uint32_t MEM2MEM_TNPR;  /**< \brief (Mem2mem Offset: 0x118) Transmit Next Pointer Register */
    __IO uint32_t MEM2MEM_TNCR;  /**< \brief (Mem2mem Offset: 0x11C) Transmit Next Counter Register */
    __O  uint32_t MEM2MEM_PTCR;  /**< \brief (Mem2mem Offset: 0x120) Transfer Control Register */
    __I  uint32_t MEM2MEM_PTSR;  /**< \brief (Mem2mem Offset: 0x124) Transfer Status Register */
} Mem2mem;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/* -------- MEM2MEM_THR : (MEM2MEM Offset: 0x00) Memory to Memory Transfer Holding Register -------- */
#define MEM2MEM_THR_THDATA_Pos 0
#define MEM2MEM_THR_THDATA_Msk (0xffffffffu << MEM2MEM_THR_THDATA_Pos) /**< \brief (MEM2MEM_THR) Transfer Holding Data */
#define MEM2MEM_THR_THDATA(value) ((MEM2MEM_THR_THDATA_Msk & ((value) << MEM2MEM_THR_THDATA_Pos)))
/* -------- MEM2MEM_MR : (MEM2MEM Offset: 0x04) Memory to Memory Mode Register -------- */
#define MEM2MEM_MR_TSIZE_Pos 0
#define MEM2MEM_MR_TSIZE_Msk (0x3u << MEM2MEM_MR_TSIZE_Pos) /**< \brief (MEM2MEM_MR) Transfer Size */
#define   MEM2MEM_MR_TSIZE_T_8BIT (0x0u << 0) /**< \brief (MEM2MEM_MR) The buffer size is defined in byte. */
#define   MEM2MEM_MR_TSIZE_T_16BIT (0x1u << 0) /**< \brief (MEM2MEM_MR) The buffer size is defined in half-word (16-bit). */
#define   MEM2MEM_MR_TSIZE_T_32BIT (0x2u << 0) /**< \brief (MEM2MEM_MR) The buffer size is defined in word (32-bit). Default value. */
/* -------- MEM2MEM_IER : (MEM2MEM Offset: 0x08) Memory to Memory Interrupt Enable Register -------- */
#define MEM2MEM_IER_RXEND (0x1u << 0) /**< \brief (MEM2MEM_IER) End of Transfer Interrupt Enable */
#define MEM2MEM_IER_RXBUFF (0x1u << 1) /**< \brief (MEM2MEM_IER) Buffer Full Interrupt Enable */
/* -------- MEM2MEM_IDR : (MEM2MEM Offset: 0x0C) Memory to Memory Interrupt Disable Register -------- */
#define MEM2MEM_IDR_RXEND (0x1u << 0) /**< \brief (MEM2MEM_IDR) End of Transfer Interrupt Disable */
#define MEM2MEM_IDR_RXBUFF (0x1u << 1) /**< \brief (MEM2MEM_IDR) Buffer Full Interrupt Disable */
/* -------- MEM2MEM_IMR : (MEM2MEM Offset: 0x10) Memory to Memory Interrupt Mask Register -------- */
#define MEM2MEM_IMR_RXEND (0x1u << 0) /**< \brief (MEM2MEM_IMR) End of Transfer Interrupt Mask */
#define MEM2MEM_IMR_RXBUFF (0x1u << 1) /**< \brief (MEM2MEM_IMR) Buffer Full Interrupt Mask */
/* -------- MEM2MEM_ISR : (MEM2MEM Offset: 0x14) Memory to Memory Interrupt Status Register -------- */
#define MEM2MEM_ISR_RXEND (0x1u << 0) /**< \brief (MEM2MEM_ISR) End of Transfer */
#define MEM2MEM_ISR_RXBUFF (0x1u << 1) /**< \brief (MEM2MEM_ISR) Buffer Full */
/* -------- MEM2MEM_RPR : (MEM2MEM Offset: 0x100) Receive Pointer Register -------- */
#define MEM2MEM_RPR_RXPTR_Pos 0
#define MEM2MEM_RPR_RXPTR_Msk (0xffffffffu << MEM2MEM_RPR_RXPTR_Pos) /**< \brief (MEM2MEM_RPR) Receive Pointer Register */
#define MEM2MEM_RPR_RXPTR(value) ((MEM2MEM_RPR_RXPTR_Msk & ((value) << MEM2MEM_RPR_RXPTR_Pos)))
/* -------- MEM2MEM_RCR : (MEM2MEM Offset: 0x104) Receive Counter Register -------- */
#define MEM2MEM_RCR_RXCTR_Pos 0
#define MEM2MEM_RCR_RXCTR_Msk (0xffffu << MEM2MEM_RCR_RXCTR_Pos) /**< \brief (MEM2MEM_RCR) Receive Counter Register */
#define MEM2MEM_RCR_RXCTR(value) ((MEM2MEM_RCR_RXCTR_Msk & ((value) << MEM2MEM_RCR_RXCTR_Pos)))
/* -------- MEM2MEM_TPR : (MEM2MEM Offset: 0x108) Transmit Pointer Register -------- */
#define MEM2MEM_TPR_TXPTR_Pos 0
#define MEM2MEM_TPR_TXPTR_Msk (0xffffffffu << MEM2MEM_TPR_TXPTR_Pos) /**< \brief (MEM2MEM_TPR) Transmit Counter Register */
#define MEM2MEM_TPR_TXPTR(value) ((MEM2MEM_TPR_TXPTR_Msk & ((value) << MEM2MEM_TPR_TXPTR_Pos)))
/* -------- MEM2MEM_TCR : (MEM2MEM Offset: 0x10C) Transmit Counter Register -------- */
#define MEM2MEM_TCR_TXCTR_Pos 0
#define MEM2MEM_TCR_TXCTR_Msk (0xffffu << MEM2MEM_TCR_TXCTR_Pos) /**< \brief (MEM2MEM_TCR) Transmit Counter Register */
#define MEM2MEM_TCR_TXCTR(value) ((MEM2MEM_TCR_TXCTR_Msk & ((value) << MEM2MEM_TCR_TXCTR_Pos)))
/* -------- MEM2MEM_RNPR : (MEM2MEM Offset: 0x110) Receive Next Pointer Register -------- */
#define MEM2MEM_RNPR_RXNPTR_Pos 0
#define MEM2MEM_RNPR_RXNPTR_Msk (0xffffffffu << MEM2MEM_RNPR_RXNPTR_Pos) /**< \brief (MEM2MEM_RNPR) Receive Next Pointer */
#define MEM2MEM_RNPR_RXNPTR(value) ((MEM2MEM_RNPR_RXNPTR_Msk & ((value) << MEM2MEM_RNPR_RXNPTR_Pos)))
/* -------- MEM2MEM_RNCR : (MEM2MEM Offset: 0x114) Receive Next Counter Register -------- */
#define MEM2MEM_RNCR_RXNCTR_Pos 0
#define MEM2MEM_RNCR_RXNCTR_Msk (0xffffu << MEM2MEM_RNCR_RXNCTR_Pos) /**< \brief (MEM2MEM_RNCR) Receive Next Counter */
#define MEM2MEM_RNCR_RXNCTR(value) ((MEM2MEM_RNCR_RXNCTR_Msk & ((value) << MEM2MEM_RNCR_RXNCTR_Pos)))
/* -------- MEM2MEM_TNPR : (MEM2MEM Offset: 0x118) Transmit Next Pointer Register -------- */
#define MEM2MEM_TNPR_TXNPTR_Pos 0
#define MEM2MEM_TNPR_TXNPTR_Msk (0xffffffffu << MEM2MEM_TNPR_TXNPTR_Pos) /**< \brief (MEM2MEM_TNPR) Transmit Next Pointer */
#define MEM2MEM_TNPR_TXNPTR(value) ((MEM2MEM_TNPR_TXNPTR_Msk & ((value) << MEM2MEM_TNPR_TXNPTR_Pos)))
/* -------- MEM2MEM_TNCR : (MEM2MEM Offset: 0x11C) Transmit Next Counter Register -------- */
#define MEM2MEM_TNCR_TXNCTR_Pos 0
#define MEM2MEM_TNCR_TXNCTR_Msk (0xffffu << MEM2MEM_TNCR_TXNCTR_Pos) /**< \brief (MEM2MEM_TNCR) Transmit Counter Next */
#define MEM2MEM_TNCR_TXNCTR(value) ((MEM2MEM_TNCR_TXNCTR_Msk & ((value) << MEM2MEM_TNCR_TXNCTR_Pos)))
/* -------- MEM2MEM_PTCR : (MEM2MEM Offset: 0x120) Transfer Control Register -------- */
#define MEM2MEM_PTCR_RXTEN (0x1u << 0) /**< \brief (MEM2MEM_PTCR) Receiver Transfer Enable */
#define MEM2MEM_PTCR_RXTDIS (0x1u << 1) /**< \brief (MEM2MEM_PTCR) Receiver Transfer Disable */
#define MEM2MEM_PTCR_TXTEN (0x1u << 8) /**< \brief (MEM2MEM_PTCR) Transmitter Transfer Enable */
#define MEM2MEM_PTCR_TXTDIS (0x1u << 9) /**< \brief (MEM2MEM_PTCR) Transmitter Transfer Disable */
#define MEM2MEM_PTCR_RXCBEN (0x1u << 16) /**< \brief (MEM2MEM_PTCR) Receiver Circular Buffer Enable */
#define MEM2MEM_PTCR_RXCBDIS (0x1u << 17) /**< \brief (MEM2MEM_PTCR) Receiver Circular Buffer Disable */
#define MEM2MEM_PTCR_TXCBEN (0x1u << 18) /**< \brief (MEM2MEM_PTCR) Transmitter Circular Buffer Enable */
#define MEM2MEM_PTCR_TXCBDIS (0x1u << 19) /**< \brief (MEM2MEM_PTCR) Transmitter Circular Buffer Disable */
#define MEM2MEM_PTCR_ERRCLR (0x1u << 24) /**< \brief (MEM2MEM_PTCR) Transfer Bus Error Clear */
/* -------- MEM2MEM_PTSR : (MEM2MEM Offset: 0x124) Transfer Status Register -------- */
#define MEM2MEM_PTSR_RXTEN (0x1u << 0) /**< \brief (MEM2MEM_PTSR) Receiver Transfer Enable */
#define MEM2MEM_PTSR_TXTEN (0x1u << 8) /**< \brief (MEM2MEM_PTSR) Transmitter Transfer Enable */
#define MEM2MEM_PTSR_RXCBEN (0x1u << 16) /**< \brief (MEM2MEM_PTSR) Receiver Transfer Enable */
#define MEM2MEM_PTSR_TXCBEN (0x1u << 18) /**< \brief (MEM2MEM_PTSR) Transmitter Transfer Enable */
#define MEM2MEM_PTSR_ERR (0x1u << 24) /**< \brief (MEM2MEM_PTSR) Transfer Bus Error (clear on read) */

/*@}*/


#endif /* _SAMG55_MEM2MEM_COMPONENT_ */
