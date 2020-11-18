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

#ifndef _PIC32CX_SHA_COMPONENT_
#define _PIC32CX_SHA_COMPONENT_

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR Secure Hash Algorithm */
/* ============================================================================= */
/** \addtogroup PIC32CX_SHA Secure Hash Algorithm */
/*@{*/

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
/** \brief Sha hardware registers */
typedef struct {
  __O  uint32_t SHA_CR;          /**< \brief (Sha Offset: 0x0) Control Register */
  __IO uint32_t SHA_MR;          /**< \brief (Sha Offset: 0x4) Mode Register */
  __I  uint32_t Reserved1[2];
  __O  uint32_t SHA_IER;         /**< \brief (Sha Offset: 0x10) Interrupt Enable Register */
  __O  uint32_t SHA_IDR;         /**< \brief (Sha Offset: 0x14) Interrupt Disable Register */
  __I  uint32_t SHA_IMR;         /**< \brief (Sha Offset: 0x18) Interrupt Mask Register */
  __I  uint32_t SHA_ISR;         /**< \brief (Sha Offset: 0x1C) Interrupt Status Register */
  __IO uint32_t SHA_MSR;         /**< \brief (Sha Offset: 0x20) Message Size Register */
  __I  uint32_t Reserved2[3];
  __IO uint32_t SHA_BCR;         /**< \brief (Sha Offset: 0x30) Bytes Count Register */
  __I  uint32_t Reserved3[3];
  __O  uint32_t SHA_IDATAR[16];  /**< \brief (Sha Offset: 0x40) Input Data 0 Register */
  __IO uint32_t SHA_IODATAR[16]; /**< \brief (Sha Offset: 0x80) Input/Output Data 0 Register */
  __I  uint32_t Reserved4[9];
  __IO uint32_t SHA_WPMR;        /**< \brief (Sha Offset: 0xE4) Write Protection Mode Register */
  __I  uint32_t SHA_WPSR;        /**< \brief (Sha Offset: 0xE8) Write Protection Status Register */
  __I  uint32_t Reserved5[7];
  __IO uint32_t SHA_TPR;         /**< \brief (Sha Offset: 0x108) Transmit Pointer Register */
  __IO uint32_t SHA_TCR;         /**< \brief (Sha Offset: 0x10C) Transmit Counter Register */
  __I  uint32_t Reserved6[2];
  __IO uint32_t SHA_TNPR;        /**< \brief (Sha Offset: 0x118) Transmit Next Pointer Register */
  __IO uint32_t SHA_TNCR;        /**< \brief (Sha Offset: 0x11C) Transmit Next Counter Register */
  __O  uint32_t SHA_PTCR;        /**< \brief (Sha Offset: 0x120) Transfer Control Register */
  __I  uint32_t SHA_PTSR;        /**< \brief (Sha Offset: 0x124) Transfer Status Register */
  __IO uint32_t SHA_PWPMR;       /**< \brief (Sha Offset: 0x128) Write Protection Mode Register */
} Sha;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/* -------- SHA_CR : (SHA Offset: 0x0) Control Register -------- */
#define SHA_CR_START (0x1u << 0) /**< \brief (SHA_CR) Start Processing */
#define SHA_CR_FIRST (0x1u << 4) /**< \brief (SHA_CR) First Block of a Message */
#define SHA_CR_SWRST (0x1u << 8) /**< \brief (SHA_CR) Software Reset */
#define SHA_CR_WUIHV (0x1u << 12) /**< \brief (SHA_CR) Write User Initial Hash Values */
#define SHA_CR_WUIEHV (0x1u << 13) /**< \brief (SHA_CR) Write User Initial or Expected Hash Values */
#define SHA_CR_UNLOCK (0x1u << 24) /**< \brief (SHA_CR) Unlock Processing */
/* -------- SHA_MR : (SHA Offset: 0x4) Mode Register -------- */
#define SHA_MR_SMOD_Pos 0
#define SHA_MR_SMOD_Msk (0x3u << SHA_MR_SMOD_Pos) /**< \brief (SHA_MR) Start Mode */
#define SHA_MR_SMOD(value) ((SHA_MR_SMOD_Msk & ((value) << SHA_MR_SMOD_Pos)))
#define   SHA_MR_SMOD_MANUAL_START (0x0u << 0) /**< \brief (SHA_MR) Manual mode */
#define   SHA_MR_SMOD_AUTO_START (0x1u << 0) /**< \brief (SHA_MR) Auto mode */
#define   SHA_MR_SMOD_IDATAR0_START (0x2u << 0) /**< \brief (SHA_MR) SHA_IDATAR0 access only mode (mandatory when DMA is used) */
#define SHA_MR_AOE (0x1u << 3) /**< \brief (SHA_MR) Always ON Enable */
#define SHA_MR_PROCDLY (0x1u << 4) /**< \brief (SHA_MR) Processing Delay */
#define   SHA_MR_PROCDLY_SHORTEST (0x0u << 4) /**< \brief (SHA_MR) SHA processing runtime is the shortest one */
#define   SHA_MR_PROCDLY_LONGEST (0x1u << 4) /**< \brief (SHA_MR) SHA processing runtime is the longest one (reduces the SHA bandwidth requirement, reduces the system bus overload) */
#define SHA_MR_UIHV (0x1u << 5) /**< \brief (SHA_MR) User Initial Hash Value Registers */
#define SHA_MR_UIEHV (0x1u << 6) /**< \brief (SHA_MR) User Initial or Expected Hash Value Registers */
#define SHA_MR_BPE (0x1u << 7) /**< \brief (SHA_MR) Block Processing End */
#define SHA_MR_ALGO_Pos 8
#define SHA_MR_ALGO_Msk (0xfu << SHA_MR_ALGO_Pos) /**< \brief (SHA_MR) SHA Algorithm */
#define SHA_MR_ALGO(value) ((SHA_MR_ALGO_Msk & ((value) << SHA_MR_ALGO_Pos)))
#define   SHA_MR_ALGO_SHA1 (0x0u << 8) /**< \brief (SHA_MR) SHA1 algorithm processed */
#define   SHA_MR_ALGO_SHA256 (0x1u << 8) /**< \brief (SHA_MR) SHA256 algorithm processed */
#define   SHA_MR_ALGO_SHA384 (0x2u << 8) /**< \brief (SHA_MR) SHA384 algorithm processed */
#define   SHA_MR_ALGO_SHA512 (0x3u << 8) /**< \brief (SHA_MR) SHA512 algorithm processed */
#define   SHA_MR_ALGO_SHA224 (0x4u << 8) /**< \brief (SHA_MR) SHA224 algorithm processed */
#define   SHA_MR_ALGO_SHA512_224 (0x5u << 8) /**< \brief (SHA_MR) SHA512/224 algorithm processed */
#define   SHA_MR_ALGO_SHA512_256 (0x6u << 8) /**< \brief (SHA_MR) SHA512/256 algorithm processed */
#define   SHA_MR_ALGO_HMAC_SHA1 (0x8u << 8) /**< \brief (SHA_MR) HMAC algorithm with SHA1 Hash processed */
#define   SHA_MR_ALGO_HMAC_SHA256 (0x9u << 8) /**< \brief (SHA_MR) HMAC algorithm with SHA256 Hash processed */
#define   SHA_MR_ALGO_HMAC_SHA384 (0xAu << 8) /**< \brief (SHA_MR) HMAC algorithm with SHA384 Hash processed */
#define   SHA_MR_ALGO_HMAC_SHA512 (0xBu << 8) /**< \brief (SHA_MR) HMAC algorithm with SHA512 Hash processed */
#define   SHA_MR_ALGO_HMAC_SHA224 (0xCu << 8) /**< \brief (SHA_MR) HMAC algorithm with SHA224 Hash processed */
#define   SHA_MR_ALGO_HMAC_SHA512_224 (0xDu << 8) /**< \brief (SHA_MR) HMAC algorithm with SHA512/224 Hash processed */
#define   SHA_MR_ALGO_HMAC_SHA512_256 (0xEu << 8) /**< \brief (SHA_MR) HMAC algorithm with SHA512/256 Hash processed */
#define SHA_MR_TMPLCK (0x1u << 15) /**< \brief (SHA_MR) Tamper Lock Enable */
#define SHA_MR_DUALBUFF (0x1u << 16) /**< \brief (SHA_MR) Dual Input Buffer */
#define   SHA_MR_DUALBUFF_INACTIVE (0x0u << 16) /**< \brief (SHA_MR) SHA_IDATARx and SHA_IODATARx cannot be written during processing of previous block. */
#define   SHA_MR_DUALBUFF_ACTIVE (0x1u << 16) /**< \brief (SHA_MR) SHA_IDATARx and SHA_IODATARx can be written during processing of previous block when SMOD value = 2. It speeds up the overall runtime of large files. */
#define SHA_MR_CHECK_Pos 24
#define SHA_MR_CHECK_Msk (0x3u << SHA_MR_CHECK_Pos) /**< \brief (SHA_MR) Hash Check */
#define SHA_MR_CHECK(value) ((SHA_MR_CHECK_Msk & ((value) << SHA_MR_CHECK_Pos)))
#define   SHA_MR_CHECK_NO_CHECK (0x0u << 24) /**< \brief (SHA_MR) No check is performed */
#define   SHA_MR_CHECK_CHECK_EHV (0x1u << 24) /**< \brief (SHA_MR) Check is performed with expected hash stored in internal expected hash value registers. */
#define   SHA_MR_CHECK_CHECK_MESSAGE (0x2u << 24) /**< \brief (SHA_MR) Check is performed with expected hash provided after the message. */
#define SHA_MR_CHKCNT_Pos 28
#define SHA_MR_CHKCNT_Msk (0xfu << SHA_MR_CHKCNT_Pos) /**< \brief (SHA_MR) Check Counter */
#define SHA_MR_CHKCNT(value) ((SHA_MR_CHKCNT_Msk & ((value) << SHA_MR_CHKCNT_Pos)))
/* -------- SHA_IER : (SHA Offset: 0x10) Interrupt Enable Register -------- */
#define SHA_IER_DATRDY (0x1u << 0) /**< \brief (SHA_IER) Data Ready Interrupt Enable */
#define SHA_IER_ENDTX (0x1u << 1) /**< \brief (SHA_IER) End of Transmit Buffer Interrupt Enable */
#define SHA_IER_TXBUFE (0x1u << 2) /**< \brief (SHA_IER) Transmit Buffer Empty Interrupt Enable */
#define SHA_IER_URAD (0x1u << 8) /**< \brief (SHA_IER) Unspecified Register Access Detection Interrupt Enable */
#define SHA_IER_CHECKF (0x1u << 16) /**< \brief (SHA_IER) Check Done Interrupt Enable */
#define SHA_IER_SECE (0x1u << 24) /**< \brief (SHA_IER) Security and/or Safety Event Interrupt Enable */
/* -------- SHA_IDR : (SHA Offset: 0x14) Interrupt Disable Register -------- */
#define SHA_IDR_DATRDY (0x1u << 0) /**< \brief (SHA_IDR) Data Ready Interrupt Disable */
#define SHA_IDR_ENDTX (0x1u << 1) /**< \brief (SHA_IDR) End of Transmit Buffer Interrupt Disable */
#define SHA_IDR_TXBUFE (0x1u << 2) /**< \brief (SHA_IDR) Transmit Buffer Empty Interrupt Disable */
#define SHA_IDR_URAD (0x1u << 8) /**< \brief (SHA_IDR) Unspecified Register Access Detection Interrupt Disable */
#define SHA_IDR_CHECKF (0x1u << 16) /**< \brief (SHA_IDR) Check Done Interrupt Disable */
#define SHA_IDR_SECE (0x1u << 24) /**< \brief (SHA_IDR) Security and/or Safety Event Interrupt Disable */
/* -------- SHA_IMR : (SHA Offset: 0x18) Interrupt Mask Register -------- */
#define SHA_IMR_DATRDY (0x1u << 0) /**< \brief (SHA_IMR) Data Ready Interrupt Mask */
#define SHA_IMR_ENDTX (0x1u << 1) /**< \brief (SHA_IMR) End of Transmit Buffer Interrupt Mask */
#define SHA_IMR_TXBUFE (0x1u << 2) /**< \brief (SHA_IMR) Transmit Buffer Empty Interrupt Mask */
#define SHA_IMR_URAD (0x1u << 8) /**< \brief (SHA_IMR) Unspecified Register Access Detection Interrupt Mask */
#define SHA_IMR_CHECKF (0x1u << 16) /**< \brief (SHA_IMR) Check Done Interrupt Mask */
#define SHA_IMR_SECE (0x1u << 24) /**< \brief (SHA_IMR) Security and/or Safety Event Interrupt Mask */
/* -------- SHA_ISR : (SHA Offset: 0x1C) Interrupt Status Register -------- */
#define SHA_ISR_DATRDY (0x1u << 0) /**< \brief (SHA_ISR) Data Ready (cleared by writing a 1 to bit SWRST or START in SHA_CR, or by reading SHA_IODATARx) */
#define SHA_ISR_ENDTX (0x1u << 1) /**< \brief (SHA_ISR) End of TX Buffer (cleared by writing SHA_TCR or SHA_TNCR) */
#define SHA_ISR_TXBUFE (0x1u << 2) /**< \brief (SHA_ISR) TX Buffer Empty (cleared by writing SHA_TCR or SHA_TNCR) */
#define SHA_ISR_WRDY (0x1u << 4) /**< \brief (SHA_ISR) Input Data Register Write Ready */
#define SHA_ISR_URAD (0x1u << 8) /**< \brief (SHA_ISR) Unspecified Register Access Detection Status (cleared by writing a 1 to SWRST bit in SHA_CR) */
#define SHA_ISR_URAT_Pos 12
#define SHA_ISR_URAT_Msk (0x7u << SHA_ISR_URAT_Pos) /**< \brief (SHA_ISR) Unspecified Register Access Type (cleared by writing a 1 to SWRST bit in SHA_CR) */
#define SHA_ISR_CHECKF (0x1u << 16) /**< \brief (SHA_ISR) Check Done Status (cleared by writing START or SWRST bits in SHA_CR or by reading SHA_IODATARx) */
#define SHA_ISR_CHKST_Pos 20
#define SHA_ISR_CHKST_Msk (0xfu << SHA_ISR_CHKST_Pos) /**< \brief (SHA_ISR) Check Status (cleared by writing START or SWRST bits in SHA_CR or by reading SHA_IODATARx) */
#define SHA_ISR_SECE (0x1u << 24) /**< \brief (SHA_ISR) Security and/or Safety Event */
/* -------- SHA_MSR : (SHA Offset: 0x20) Message Size Register -------- */
#define SHA_MSR_MSGSIZE_Pos 0
#define SHA_MSR_MSGSIZE_Msk (0xffffffffu << SHA_MSR_MSGSIZE_Pos) /**< \brief (SHA_MSR) Message Size */
#define SHA_MSR_MSGSIZE(value) ((SHA_MSR_MSGSIZE_Msk & ((value) << SHA_MSR_MSGSIZE_Pos)))
/* -------- SHA_BCR : (SHA Offset: 0x30) Bytes Count Register -------- */
#define SHA_BCR_BYTCNT_Pos 0
#define SHA_BCR_BYTCNT_Msk (0xffffffffu << SHA_BCR_BYTCNT_Pos) /**< \brief (SHA_BCR) Remaining Byte Count Before Auto Padding */
#define SHA_BCR_BYTCNT(value) ((SHA_BCR_BYTCNT_Msk & ((value) << SHA_BCR_BYTCNT_Pos)))
/* -------- SHA_IDATAR[16] : (SHA Offset: 0x40) Input Data 0 Register -------- */
#define SHA_IDATAR_IDATA_Pos 0
#define SHA_IDATAR_IDATA_Msk (0xffffffffu << SHA_IDATAR_IDATA_Pos) /**< \brief (SHA_IDATAR[16]) Input Data */
#define SHA_IDATAR_IDATA(value) ((SHA_IDATAR_IDATA_Msk & ((value) << SHA_IDATAR_IDATA_Pos)))
/* -------- SHA_IODATAR[16] : (SHA Offset: 0x80) Input/Output Data 0 Register -------- */
#define SHA_IODATAR_IODATA_Pos 0
#define SHA_IODATAR_IODATA_Msk (0xffffffffu << SHA_IODATAR_IODATA_Pos) /**< \brief (SHA_IODATAR[16]) Input/Output Data */
#define SHA_IODATAR_IODATA(value) ((SHA_IODATAR_IODATA_Msk & ((value) << SHA_IODATAR_IODATA_Pos)))
/* -------- SHA_WPMR : (SHA Offset: 0xE4) Write Protection Mode Register -------- */
#define SHA_WPMR_WPEN (0x1u << 0) /**< \brief (SHA_WPMR) Write Protection Configuration Enable */
#define SHA_WPMR_WPITEN (0x1u << 1) /**< \brief (SHA_WPMR) Write Protection Interruption Enable */
#define SHA_WPMR_WPCREN (0x1u << 2) /**< \brief (SHA_WPMR) Write Protection Control Enable */
#define SHA_WPMR_FIRSTE (0x1u << 4) /**< \brief (SHA_WPMR) First Error Report Enable */
#define SHA_WPMR_ACTION_Pos 5
#define SHA_WPMR_ACTION_Msk (0x3u << SHA_WPMR_ACTION_Pos) /**< \brief (SHA_WPMR) Action on Abnormal Event Detection */
#define SHA_WPMR_ACTION(value) ((SHA_WPMR_ACTION_Msk & ((value) << SHA_WPMR_ACTION_Pos)))
#define   SHA_WPMR_ACTION_REPORT_ONLY (0x0u << 5) /**< \brief (SHA_WPMR) No action (stop or clear key) is performed when one of WPVS,CGD,SEQE, or SWE flag is set. */
#define   SHA_WPMR_ACTION_LOCK_WPVS_SWE (0x1u << 5) /**< \brief (SHA_WPMR) If a processing is in progress when the SHA_WPSR.WPVS/SWE event detection occurs, the current processing is ended normally but no other processing is started while a SHA_CR.UNLOCK command is issued. */
#define   SHA_WPMR_ACTION_LOCK_CGD_SEQE (0x2u << 5) /**< \brief (SHA_WPMR) If a processing is in progress when the SHA_WPSR.CGD/SEQE event detection occurs, the current processing is ended normally but no other processing is started while a SHA_CR.UNLOCK command is issued. */
#define   SHA_WPMR_ACTION_LOCK_ANY_EV (0x3u << 5) /**< \brief (SHA_WPMR) If a processing is in progress when the SHA_WPSR.WPVS/CGD/SEQE/SWE events detection occurs, the current processing is ended normally but no other processing is started while a SHA_CR.UNLOCK command is issued. */
#define SHA_WPMR_WPKEY_Pos 8
#define SHA_WPMR_WPKEY_Msk (0xffffffu << SHA_WPMR_WPKEY_Pos) /**< \brief (SHA_WPMR) Write Protection Key */
#define SHA_WPMR_WPKEY(value) ((SHA_WPMR_WPKEY_Msk & ((value) << SHA_WPMR_WPKEY_Pos)))
#define   SHA_WPMR_WPKEY_PASSWD (0x534841u << 8) /**< \brief (SHA_WPMR) Writing any other value in this field aborts the write operation of the WPEN,WPITEN,WPCREN bits. Always reads as 0. */
/* -------- SHA_WPSR : (SHA Offset: 0xE8) Write Protection Status Register -------- */
#define SHA_WPSR_WPVS (0x1u << 0) /**< \brief (SHA_WPSR) Write Protection Violation Status (cleared on read) */
#define SHA_WPSR_CGD (0x1u << 1) /**< \brief (SHA_WPSR) Clock Glitch Detected (cleared on read) */
#define SHA_WPSR_SEQE (0x1u << 2) /**< \brief (SHA_WPSR) Internal Sequencer Error (cleared on read) */
#define SHA_WPSR_SWE (0x1u << 3) /**< \brief (SHA_WPSR) Software Control Error (cleared on read) */
#define SHA_WPSR_WPVSRC_Pos 8
#define SHA_WPSR_WPVSRC_Msk (0xffu << SHA_WPSR_WPVSRC_Pos) /**< \brief (SHA_WPSR) Write Protection Violation Source */
#define SHA_WPSR_SWETYP_Pos 24
#define SHA_WPSR_SWETYP_Msk (0xfu << SHA_WPSR_SWETYP_Pos) /**< \brief (SHA_WPSR) Software Error Type (cleared on read) */
#define   SHA_WPSR_SWETYP_READ_WO (0x0u << 24) /**< \brief (SHA_WPSR) A write-only register has been read (Warning). */
#define   SHA_WPSR_SWETYP_WRITE_RO (0x1u << 24) /**< \brief (SHA_WPSR) SHA is enabled and a write access has been performed on a read-only register (Warning). */
#define   SHA_WPSR_SWETYP_UNDEF_RW (0x2u << 24) /**< \brief (SHA_WPSR) Access to an undefined address (Warning). */
#define   SHA_WPSR_SWETYP_CTRL_START (0x3u << 24) /**< \brief (SHA_WPSR) SHA is locked and a start command with SHA_CR.START has been performed. */
#define   SHA_WPSR_SWETYP_AUTO_START (0x4u << 24) /**< \brief (SHA_WPSR) SHA is locked and a tentative automatic start has been performed by writing input data registers (SHA_MR.SMOD>0). */
#define   SHA_WPSR_SWETYP_BAD_START (0x5u << 24) /**< \brief (SHA_WPSR) SHA is not locked and a start command with SHA_CR.START has been performed whereas Start mode is automatic (SHA_MR.SMOD>0) */
#define SHA_WPSR_ECLASS (0x1u << 31) /**< \brief (SHA_WPSR) Software Error Class (cleared on read) */
#define   SHA_WPSR_ECLASS_WARNING (0x0u << 31) /**< \brief (SHA_WPSR) An abnormal access that does not affect system functionality */
#define   SHA_WPSR_ECLASS_ERROR (0x1u << 31) /**< \brief (SHA_WPSR) An access is performed into key, input data, control registers while the SHA is performing an encryption/decryption or a start is request by software or DMA while the key is not fully configured. */
/* -------- SHA_TPR : (SHA Offset: 0x108) Transmit Pointer Register -------- */
#define SHA_TPR_TXPTR_Pos 0
#define SHA_TPR_TXPTR_Msk (0xffffffffu << SHA_TPR_TXPTR_Pos) /**< \brief (SHA_TPR) Transmit Counter Register */
#define SHA_TPR_TXPTR(value) ((SHA_TPR_TXPTR_Msk & ((value) << SHA_TPR_TXPTR_Pos)))
/* -------- SHA_TCR : (SHA Offset: 0x10C) Transmit Counter Register -------- */
#define SHA_TCR_TXCTR_Pos 0
#define SHA_TCR_TXCTR_Msk (0xffffu << SHA_TCR_TXCTR_Pos) /**< \brief (SHA_TCR) Transmit Counter Register */
#define SHA_TCR_TXCTR(value) ((SHA_TCR_TXCTR_Msk & ((value) << SHA_TCR_TXCTR_Pos)))
/* -------- SHA_TNPR : (SHA Offset: 0x118) Transmit Next Pointer Register -------- */
#define SHA_TNPR_TXNPTR_Pos 0
#define SHA_TNPR_TXNPTR_Msk (0xffffffffu << SHA_TNPR_TXNPTR_Pos) /**< \brief (SHA_TNPR) Transmit Next Pointer */
#define SHA_TNPR_TXNPTR(value) ((SHA_TNPR_TXNPTR_Msk & ((value) << SHA_TNPR_TXNPTR_Pos)))
/* -------- SHA_TNCR : (SHA Offset: 0x11C) Transmit Next Counter Register -------- */
#define SHA_TNCR_TXNCTR_Pos 0
#define SHA_TNCR_TXNCTR_Msk (0xffffu << SHA_TNCR_TXNCTR_Pos) /**< \brief (SHA_TNCR) Transmit Counter Next */
#define SHA_TNCR_TXNCTR(value) ((SHA_TNCR_TXNCTR_Msk & ((value) << SHA_TNCR_TXNCTR_Pos)))
/* -------- SHA_PTCR : (SHA Offset: 0x120) Transfer Control Register -------- */
#define SHA_PTCR_RXTEN (0x1u << 0) /**< \brief (SHA_PTCR) Receiver Transfer Enable */
#define SHA_PTCR_RXTDIS (0x1u << 1) /**< \brief (SHA_PTCR) Receiver Transfer Disable */
#define SHA_PTCR_TXTEN (0x1u << 8) /**< \brief (SHA_PTCR) Transmitter Transfer Enable */
#define SHA_PTCR_TXTDIS (0x1u << 9) /**< \brief (SHA_PTCR) Transmitter Transfer Disable */
#define SHA_PTCR_RXCBEN (0x1u << 16) /**< \brief (SHA_PTCR) Receiver Circular Buffer Enable */
#define SHA_PTCR_RXCBDIS (0x1u << 17) /**< \brief (SHA_PTCR) Receiver Circular Buffer Disable */
#define SHA_PTCR_TXCBEN (0x1u << 18) /**< \brief (SHA_PTCR) Transmitter Circular Buffer Enable */
#define SHA_PTCR_TXCBDIS (0x1u << 19) /**< \brief (SHA_PTCR) Transmitter Circular Buffer Disable */
#define SHA_PTCR_ERRCLR (0x1u << 24) /**< \brief (SHA_PTCR) Transfer Bus Error Clear */
/* -------- SHA_PTSR : (SHA Offset: 0x124) Transfer Status Register -------- */
#define SHA_PTSR_RXTEN (0x1u << 0) /**< \brief (SHA_PTSR) Receiver Transfer Enable */
#define SHA_PTSR_TXTEN (0x1u << 8) /**< \brief (SHA_PTSR) Transmitter Transfer Enable */
#define SHA_PTSR_RXCBEN (0x1u << 16) /**< \brief (SHA_PTSR) Receiver Circular Buffer Enable */
#define SHA_PTSR_TXCBEN (0x1u << 18) /**< \brief (SHA_PTSR) Transmitter Circular Buffer Enable */
#define SHA_PTSR_ERR (0x1u << 24) /**< \brief (SHA_PTSR) Transfer Bus Error */
/* -------- SHA_PWPMR : (SHA Offset: 0x128) Write Protection Mode Register -------- */
#define SHA_PWPMR_WPPTREN (0x1u << 0) /**< \brief (SHA_PWPMR) Write Protection Pointer Registers Enable */
#define SHA_PWPMR_WPCTREN (0x1u << 1) /**< \brief (SHA_PWPMR) Write Protection Counter Registers Enable */
#define SHA_PWPMR_WPCREN (0x1u << 2) /**< \brief (SHA_PWPMR) Write Protection Control Register Enable */
#define SHA_PWPMR_WPKEY_Pos 8
#define SHA_PWPMR_WPKEY_Msk (0xffffffu << SHA_PWPMR_WPKEY_Pos) /**< \brief (SHA_PWPMR) Write Protection Key */
#define SHA_PWPMR_WPKEY(value) ((SHA_PWPMR_WPKEY_Msk & ((value) << SHA_PWPMR_WPKEY_Pos)))
#define   SHA_PWPMR_WPKEY_PASSWD (0x504443u << 8) /**< \brief (SHA_PWPMR) Writing any other value in this field aborts the write operation. Always reads as 0. */

/*@}*/


#endif /* _PIC32CX_SHA_COMPONENT_ */
