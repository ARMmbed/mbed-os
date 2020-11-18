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

#ifndef _PIC32CX_AES_COMPONENT_
#define _PIC32CX_AES_COMPONENT_

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR Advanced Encryption Standard */
/* ============================================================================= */
/** \addtogroup PIC32CX_AES Advanced Encryption Standard */
/*@{*/

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
/** \brief Aes hardware registers */
typedef struct {
  __O  uint32_t AES_CR;        /**< \brief (Aes Offset: 0x0) Control Register */
  __IO uint32_t AES_MR;        /**< \brief (Aes Offset: 0x4) Mode Register */
  __I  uint32_t Reserved1[2];
  __O  uint32_t AES_IER;       /**< \brief (Aes Offset: 0x10) Interrupt Enable Register */
  __O  uint32_t AES_IDR;       /**< \brief (Aes Offset: 0x14) Interrupt Disable Register */
  __I  uint32_t AES_IMR;       /**< \brief (Aes Offset: 0x18) Interrupt Mask Register */
  __I  uint32_t AES_ISR;       /**< \brief (Aes Offset: 0x1C) Interrupt Status Register */
  __O  uint32_t AES_KEYWR[8];  /**< \brief (Aes Offset: 0x20) Key Word Register */
  __O  uint32_t AES_IDATAR[4]; /**< \brief (Aes Offset: 0x40) Input Data Register */
  __I  uint32_t AES_ODATAR[4]; /**< \brief (Aes Offset: 0x50) Output Data Register */
  __O  uint32_t AES_IVR[4];    /**< \brief (Aes Offset: 0x60) Initialization Vector Register */
  __IO uint32_t AES_AADLENR;   /**< \brief (Aes Offset: 0x70) Additional Authenticated Data Length Register */
  __IO uint32_t AES_CLENR;     /**< \brief (Aes Offset: 0x74) Plaintext/Ciphertext Length Register */
  __IO uint32_t AES_GHASHR[4]; /**< \brief (Aes Offset: 0x78) GCM Intermediate Hash Word Register */
  __I  uint32_t AES_TAGR[4];   /**< \brief (Aes Offset: 0x88) GCM Authentication Tag Word Register */
  __I  uint32_t AES_CTRR;      /**< \brief (Aes Offset: 0x98) GCM Encryption Counter Value Register */
  __IO uint32_t AES_GCMHR[4];  /**< \brief (Aes Offset: 0x9C) GCM H World Register */
  __I  uint32_t Reserved2[16];
  __IO uint32_t AES_EMR;       /**< \brief (Aes Offset: 0xB0) Extended Mode Register */
  __IO uint32_t AES_BCNT;      /**< \brief (Aes Offset: 0xB4) Byte Counter Register */
  __I  uint32_t Reserved3[12];
  __IO uint32_t AES_WPMR;      /**< \brief (Aes Offset: 0xE4) Write Protection Mode Register */
  __I  uint32_t AES_WPSR;      /**< \brief (Aes Offset: 0xE8) Write Protection Status Register */
  __I  uint32_t Reserved4[5];
  __IO uint32_t AES_RPR;       /**< \brief (Aes Offset: 0x100) Receive Pointer Register */
  __IO uint32_t AES_RCR;       /**< \brief (Aes Offset: 0x104) Receive Counter Register */
  __IO uint32_t AES_TPR;       /**< \brief (Aes Offset: 0x108) Transmit Pointer Register */
  __IO uint32_t AES_TCR;       /**< \brief (Aes Offset: 0x10C) Transmit Counter Register */
  __IO uint32_t AES_RNPR;      /**< \brief (Aes Offset: 0x110) Receive Next Pointer Register */
  __IO uint32_t AES_RNCR;      /**< \brief (Aes Offset: 0x114) Receive Next Counter Register */
  __IO uint32_t AES_TNPR;      /**< \brief (Aes Offset: 0x118) Transmit Next Pointer Register */
  __IO uint32_t AES_TNCR;      /**< \brief (Aes Offset: 0x11C) Transmit Next Counter Register */
  __O  uint32_t AES_PTCR;      /**< \brief (Aes Offset: 0x120) Transfer Control Register */
  __I  uint32_t AES_PTSR;      /**< \brief (Aes Offset: 0x124) Transfer Status Register */
  __IO uint32_t AES_PWPMR;     /**< \brief (Aes Offset: 0x128) Write Protection Mode Register */
} Aes;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/* -------- AES_CR : (AES Offset: 0x0) Control Register -------- */
#define AES_CR_START (0x1u << 0) /**< \brief (AES_CR) Start Processing */
#define AES_CR_KSWP (0x1u << 1) /**< \brief (AES_CR) Key Swap */
#define AES_CR_SWRST (0x1u << 8) /**< \brief (AES_CR) Software Reset */
#define AES_CR_UNLOCK (0x1u << 24) /**< \brief (AES_CR) Unlock Processing */
/* -------- AES_MR : (AES Offset: 0x4) Mode Register -------- */
#define AES_MR_CIPHER (0x1u << 0) /**< \brief (AES_MR) Processing Mode */
#define AES_MR_GTAGEN (0x1u << 1) /**< \brief (AES_MR) GCM Automatic Tag Generation Enable */
#define AES_MR_DUALBUFF (0x1u << 3) /**< \brief (AES_MR) Dual Input Buffer */
#define   AES_MR_DUALBUFF_INACTIVE (0x0u << 3) /**< \brief (AES_MR) AES_IDATARx cannot be written during processing of previous block. */
#define   AES_MR_DUALBUFF_ACTIVE (0x1u << 3) /**< \brief (AES_MR) AES_IDATARx can be written during processing of previous block when SMOD = 2. It speeds up the overall runtime of large files. */
#define AES_MR_PROCDLY_Pos 4
#define AES_MR_PROCDLY_Msk (0xfu << AES_MR_PROCDLY_Pos) /**< \brief (AES_MR) Processing Delay */
#define AES_MR_PROCDLY(value) ((AES_MR_PROCDLY_Msk & ((value) << AES_MR_PROCDLY_Pos)))
#define AES_MR_SMOD_Pos 8
#define AES_MR_SMOD_Msk (0x3u << AES_MR_SMOD_Pos) /**< \brief (AES_MR) Start Mode */
#define AES_MR_SMOD(value) ((AES_MR_SMOD_Msk & ((value) << AES_MR_SMOD_Pos)))
#define   AES_MR_SMOD_MANUAL_START (0x0u << 8) /**< \brief (AES_MR) Manual Mode */
#define   AES_MR_SMOD_AUTO_START (0x1u << 8) /**< \brief (AES_MR) Auto Mode */
#define   AES_MR_SMOD_IDATAR0_START (0x2u << 8) /**< \brief (AES_MR) AES_IDATAR0 access only Auto Mode (PDC) */
#define AES_MR_KEYSIZE_Pos 10
#define AES_MR_KEYSIZE_Msk (0x3u << AES_MR_KEYSIZE_Pos) /**< \brief (AES_MR) Key Size */
#define AES_MR_KEYSIZE(value) ((AES_MR_KEYSIZE_Msk & ((value) << AES_MR_KEYSIZE_Pos)))
#define   AES_MR_KEYSIZE_AES128 (0x0u << 10) /**< \brief (AES_MR) AES Key Size is 128 bits */
#define   AES_MR_KEYSIZE_AES192 (0x1u << 10) /**< \brief (AES_MR) AES Key Size is 192 bits */
#define   AES_MR_KEYSIZE_AES256 (0x2u << 10) /**< \brief (AES_MR) AES Key Size is 256 bits */
#define AES_MR_OPMOD_Pos 12
#define AES_MR_OPMOD_Msk (0x7u << AES_MR_OPMOD_Pos) /**< \brief (AES_MR) Operating Mode */
#define AES_MR_OPMOD(value) ((AES_MR_OPMOD_Msk & ((value) << AES_MR_OPMOD_Pos)))
#define   AES_MR_OPMOD_ECB (0x0u << 12) /**< \brief (AES_MR) ECB: Electronic Codebook mode */
#define   AES_MR_OPMOD_CBC (0x1u << 12) /**< \brief (AES_MR) CBC: Cipher Block Chaining mode */
#define   AES_MR_OPMOD_OFB (0x2u << 12) /**< \brief (AES_MR) OFB: Output Feedback mode */
#define   AES_MR_OPMOD_CFB (0x3u << 12) /**< \brief (AES_MR) CFB: Cipher Feedback mode */
#define   AES_MR_OPMOD_CTR (0x4u << 12) /**< \brief (AES_MR) CTR: Counter mode (16-bit internal counter) */
#define AES_MR_LOD (0x1u << 15) /**< \brief (AES_MR) Last Output Data Mode */
#define AES_MR_CFBS_Pos 16
#define AES_MR_CFBS_Msk (0x7u << AES_MR_CFBS_Pos) /**< \brief (AES_MR) Cipher Feedback Data Size */
#define AES_MR_CFBS(value) ((AES_MR_CFBS_Msk & ((value) << AES_MR_CFBS_Pos)))
#define   AES_MR_CFBS_SIZE_128BIT (0x0u << 16) /**< \brief (AES_MR) 128-bit */
#define   AES_MR_CFBS_SIZE_64BIT (0x1u << 16) /**< \brief (AES_MR) 64-bit */
#define   AES_MR_CFBS_SIZE_32BIT (0x2u << 16) /**< \brief (AES_MR) 32-bit */
#define   AES_MR_CFBS_SIZE_16BIT (0x3u << 16) /**< \brief (AES_MR) 16-bit */
#define   AES_MR_CFBS_SIZE_8BIT (0x4u << 16) /**< \brief (AES_MR) 8-bit */
#define AES_MR_CKEY_Pos 20
#define AES_MR_CKEY_Msk (0xfu << AES_MR_CKEY_Pos) /**< \brief (AES_MR) Key */
#define AES_MR_CKEY(value) ((AES_MR_CKEY_Msk & ((value) << AES_MR_CKEY_Pos)))
#define   AES_MR_CKEY_PASSWD (0xEu << 20) /**< \brief (AES_MR) This field must be written with 0xE the first time AES_MR is programmed. For subsequent programming of AES_MR, any value can be written, including that of 0xE. Always reads as 0. */
#define AES_MR_TAMPCLR (0x1u << 31) /**< \brief (AES_MR) Tamper Clear Enable */
/* -------- AES_IER : (AES Offset: 0x10) Interrupt Enable Register -------- */
#define AES_IER_DATRDY (0x1u << 0) /**< \brief (AES_IER) Data Ready Interrupt Enable */
#define AES_IER_ENDRX (0x1u << 1) /**< \brief (AES_IER) End of Receive Buffer Interrupt Enable */
#define AES_IER_ENDTX (0x1u << 2) /**< \brief (AES_IER) End of Transmit Buffer Interrupt Enable */
#define AES_IER_RXBUFF (0x1u << 3) /**< \brief (AES_IER) Receive Buffer Full Interrupt Enable */
#define AES_IER_TXBUFE (0x1u << 4) /**< \brief (AES_IER) Transmit Buffer Empty Interrupt Enable */
#define AES_IER_URAD (0x1u << 8) /**< \brief (AES_IER) Unspecified Register Access Detection Interrupt Enable */
#define AES_IER_TAGRDY (0x1u << 16) /**< \brief (AES_IER) GCM Tag Ready Interrupt Enable */
#define AES_IER_EOPAD (0x1u << 17) /**< \brief (AES_IER) End of Padding Interrupt Enable */
#define AES_IER_PLENERR (0x1u << 18) /**< \brief (AES_IER) Padding Length Error Interrupt Enable */
#define AES_IER_SECE (0x1u << 19) /**< \brief (AES_IER) Security and/or Safety Event Interrupt Enable */
/* -------- AES_IDR : (AES Offset: 0x14) Interrupt Disable Register -------- */
#define AES_IDR_DATRDY (0x1u << 0) /**< \brief (AES_IDR) Data Ready Interrupt Disable */
#define AES_IDR_ENDRX (0x1u << 1) /**< \brief (AES_IDR) End of Receive Buffer Interrupt Disable */
#define AES_IDR_ENDTX (0x1u << 2) /**< \brief (AES_IDR) End of Transmit Buffer Interrupt Disable */
#define AES_IDR_RXBUFF (0x1u << 3) /**< \brief (AES_IDR) Receive Buffer Full Interrupt Disable */
#define AES_IDR_TXBUFE (0x1u << 4) /**< \brief (AES_IDR) Transmit Buffer Empty Interrupt Disable */
#define AES_IDR_URAD (0x1u << 8) /**< \brief (AES_IDR) Unspecified Register Access Detection Interrupt Disable */
#define AES_IDR_TAGRDY (0x1u << 16) /**< \brief (AES_IDR) GCM Tag Ready Interrupt Enable */
#define AES_IDR_EOPAD (0x1u << 17) /**< \brief (AES_IDR) End of Padding Interrupt Enable */
#define AES_IDR_PLENERR (0x1u << 18) /**< \brief (AES_IDR) Padding Length Error Interrupt Enable */
#define AES_IDR_SECE (0x1u << 19) /**< \brief (AES_IDR) Security and/or Safety Event Interrupt Disable */
/* -------- AES_IMR : (AES Offset: 0x18) Interrupt Mask Register -------- */
#define AES_IMR_DATRDY (0x1u << 0) /**< \brief (AES_IMR) Data Ready Interrupt Mask */
#define AES_IMR_ENDRX (0x1u << 1) /**< \brief (AES_IMR) End of Receive Buffer Interrupt Mask */
#define AES_IMR_ENDTX (0x1u << 2) /**< \brief (AES_IMR) End of Transmit Buffer Interrupt Mask */
#define AES_IMR_RXBUFF (0x1u << 3) /**< \brief (AES_IMR) Receive Buffer Full Interrupt Mask */
#define AES_IMR_TXBUFE (0x1u << 4) /**< \brief (AES_IMR) Transmit Buffer Empty Interrupt Mask */
#define AES_IMR_URAD (0x1u << 8) /**< \brief (AES_IMR) Unspecified Register Access Detection Interrupt Mask */
#define AES_IMR_TAGRDY (0x1u << 16) /**< \brief (AES_IMR) GCM Tag Ready Interrupt Enable */
#define AES_IMR_EOPAD (0x1u << 17) /**< \brief (AES_IMR) End of Padding Interrupt Enable */
#define AES_IMR_PLENERR (0x1u << 18) /**< \brief (AES_IMR) Padding Length Error Interrupt Enable */
#define AES_IMR_SECE (0x1u << 19) /**< \brief (AES_IMR) Security and/or Safety Event Interrupt Mask */
/* -------- AES_ISR : (AES Offset: 0x1C) Interrupt Status Register -------- */
#define AES_ISR_DATRDY (0x1u << 0) /**< \brief (AES_ISR) Data Ready (cleared by setting bit START or bit SWRST in AES_CR or by reading AES_ODATARx) */
#define AES_ISR_ENDRX (0x1u << 1) /**< \brief (AES_ISR) End of RX Buffer (cleared by writing AES_RCR or AES_RNCR) */
#define AES_ISR_ENDTX (0x1u << 2) /**< \brief (AES_ISR) End of TX Buffer (cleared by writing AES_TCR or AES_TNCR) */
#define AES_ISR_RXBUFF (0x1u << 3) /**< \brief (AES_ISR) RX Buffer Full (cleared by writing AES_RCR or AES_RNCR) */
#define AES_ISR_TXBUFE (0x1u << 4) /**< \brief (AES_ISR) TX Buffer Empty (cleared by writing AES_TCR or AES_TNCR) */
#define AES_ISR_URAD (0x1u << 8) /**< \brief (AES_ISR) Unspecified Register Access Detection Status (cleared by writing SWRST in AES_CR) */
#define AES_ISR_URAT_Pos 12
#define AES_ISR_URAT_Msk (0xfu << AES_ISR_URAT_Pos) /**< \brief (AES_ISR) Unspecified Register Access (cleared by writing SWRST in AES_CR) */
#define   AES_ISR_URAT_IDR_WR_PROCESSING (0x0u << 12) /**< \brief (AES_ISR) Input Data register written during the data processing when SMOD = 2 mode. */
#define   AES_ISR_URAT_ODR_RD_PROCESSING (0x1u << 12) /**< \brief (AES_ISR) Output Data register read during the data processing. */
#define   AES_ISR_URAT_MR_WR_PROCESSING (0x2u << 12) /**< \brief (AES_ISR) Mode register written during the data processing. */
#define   AES_ISR_URAT_ODR_RD_SUBKGEN (0x3u << 12) /**< \brief (AES_ISR) Output Data register read during the sub-keys generation. */
#define   AES_ISR_URAT_MR_WR_SUBKGEN (0x4u << 12) /**< \brief (AES_ISR) Mode register written during the sub-keys generation. */
#define   AES_ISR_URAT_WOR_RD_ACCESS (0x5u << 12) /**< \brief (AES_ISR) Write-only register read access. */
#define AES_ISR_TAGRDY (0x1u << 16) /**< \brief (AES_ISR) GCM Tag Ready Interrupt Enable */
#define AES_ISR_EOPAD (0x1u << 17) /**< \brief (AES_ISR) End of Padding Interrupt Enable */
#define AES_ISR_PLENERR (0x1u << 18) /**< \brief (AES_ISR) Padding Length Error Interrupt Enable */
#define AES_ISR_SECE (0x1u << 19) /**< \brief (AES_ISR) Security and/or Safety Event (cleared on read) */
/* -------- AES_KEYWR[8] : (AES Offset: 0x20) Key Word Register -------- */
#define AES_KEYWR_KEYW_Pos 0
#define AES_KEYWR_KEYW_Msk (0xffffffffu << AES_KEYWR_KEYW_Pos) /**< \brief (AES_KEYWR[8]) Key Word */
#define AES_KEYWR_KEYW(value) ((AES_KEYWR_KEYW_Msk & ((value) << AES_KEYWR_KEYW_Pos)))
/* -------- AES_IDATAR[4] : (AES Offset: 0x40) Input Data Register -------- */
#define AES_IDATAR_IDATA_Pos 0
#define AES_IDATAR_IDATA_Msk (0xffffffffu << AES_IDATAR_IDATA_Pos) /**< \brief (AES_IDATAR[4]) Input Data Word */
#define AES_IDATAR_IDATA(value) ((AES_IDATAR_IDATA_Msk & ((value) << AES_IDATAR_IDATA_Pos)))
/* -------- AES_ODATAR[4] : (AES Offset: 0x50) Output Data Register -------- */
#define AES_ODATAR_ODATA_Pos 0
#define AES_ODATAR_ODATA_Msk (0xffffffffu << AES_ODATAR_ODATA_Pos) /**< \brief (AES_ODATAR[4]) Output Data */
/* -------- AES_IVR[4] : (AES Offset: 0x60) Initialization Vector Register -------- */
#define AES_IVR_IV_Pos 0
#define AES_IVR_IV_Msk (0xffffffffu << AES_IVR_IV_Pos) /**< \brief (AES_IVR[4]) Initialization Vector */
#define AES_IVR_IV(value) ((AES_IVR_IV_Msk & ((value) << AES_IVR_IV_Pos)))
/* -------- AES_EMR : (AES Offset: 0xB0) Extended Mode Register -------- */
#define AES_EMR_APEN (0x1u << 0) /**< \brief (AES_EMR) Auto padding enable */
#define AES_EMR_APM (0x1u << 1) /**< \brief (AES_EMR) Auto padding mode */
#define   AES_EMR_APM_IPSEC (0x0u << 1) /**< \brief (AES_EMR) IPSEC standard is used for auto padding. */
#define   AES_EMR_APM_SSL (0x1u << 1) /**< \brief (AES_EMR) SSL standard is used for auto padding. */
#define AES_EMR_KSEL (0x1u << 2) /**< \brief (AES_EMR) Key Selection */
#define AES_EMR_PLIPEN (0x1u << 4) /**< \brief (AES_EMR) Protocol Layer Improved Performance Enable */
#define AES_EMR_PLIPD (0x1u << 5) /**< \brief (AES_EMR) Protocol Layer Improved Performance Decipher */
#define AES_EMR_PKWL (0x1u << 6) /**< \brief (AES_EMR) Private Key Write Lock */
#define AES_EMR_PKRS (0x1u << 7) /**< \brief (AES_EMR) Private Key Internal Register Select */
#define AES_EMR_PADLEN_Pos 8
#define AES_EMR_PADLEN_Msk (0xfu << AES_EMR_PADLEN_Pos) /**< \brief (AES_EMR) Auto Padding Length */
#define AES_EMR_PADLEN(value) ((AES_EMR_PADLEN_Msk & ((value) << AES_EMR_PADLEN_Pos)))
#define AES_EMR_NHEAD_Pos 16
#define AES_EMR_NHEAD_Msk (0xfu << AES_EMR_NHEAD_Pos) /**< \brief (AES_EMR) IPSEC Next Header */
#define AES_EMR_NHEAD(value) ((AES_EMR_NHEAD_Msk & ((value) << AES_EMR_NHEAD_Pos)))
#define AES_EMR_ALGO (0x1u << 24) /**< \brief (AES_EMR) Encryption Algorithm */
#define   AES_EMR_ALGO_AES (0x0u << 24) /**< \brief (AES_EMR) The AES algorithm is used for encryption. */
#define   AES_EMR_ALGO_ARIA (0x1u << 24) /**< \brief (AES_EMR) The ARIA algorithm is used for encryption. */
#define AES_EMR_BPE (0x1u << 31) /**< \brief (AES_EMR) Block Processing End */
/* -------- AES_BCNT : (AES Offset: 0xB4) Byte Counter Register -------- */
#define SHA_BCNT_BCNT_Pos 0
#define SHA_BCNT_BCNT_Msk (0xffffffffu << AES_BCNT_BCNT_Pos) /**< \brief (AES_BCNT) Byte counter */
#define SHA_BCNT_BCNT(value) ((AES_BCNT_BCNT_Msk & ((value) << AES_BCNT_BCNT_Pos)))
/* -------- AES_WPMR : (AES Offset: 0xE4) Write Protection Mode Register -------- */
#define AES_WPMR_WPEN (0x1u << 0) /**< \brief (AES_WPMR) Write Protection Configuration Enable */
#define AES_WPMR_WPITEN (0x1u << 1) /**< \brief (AES_WPMR) Write Protection Interruption Enable */
#define AES_WPMR_WPCREN (0x1u << 2) /**< \brief (AES_WPMR) Write Protection Control Enable */
#define AES_WPMR_FIRSTE (0x1u << 4) /**< \brief (AES_WPMR) First Error Report Enable */
#define AES_WPMR_ACTION_Pos 5
#define AES_WPMR_ACTION_Msk (0x7u << AES_WPMR_ACTION_Pos) /**< \brief (AES_WPMR) Action on Abnormal Event Detection */
#define AES_WPMR_ACTION(value) ((AES_WPMR_ACTION_Msk & ((value) << AES_WPMR_ACTION_Pos)))
#define   AES_WPMR_ACTION_REPORT_ONLY (0x0u << 5) /**< \brief (AES_WPMR) No action (stop or clear key) is performed when one of PKRPVS, WPVS, CGD, SEQE, or SWE flags is set. */
#define   AES_WPMR_ACTION_LOCK_PKRPVS_WPVS_SWE (0x1u << 5) /**< \brief (AES_WPMR) If a processing is in progress when the AES_WPSR.PKRPVS/WPVS/SWE event detection occurs, the current processing is ended normally but no other processing is started while a AES_CR.UNLOCK command is issued. */
#define   AES_WPMR_ACTION_LOCK_CGD_SEQE (0x2u << 5) /**< \brief (AES_WPMR) If a processing is in progress when the AES_WPSR.CGD/SEQE event detection occurs, the current processing is ended normally but no other processing is started while a AES_CR.UNLOCK command is issued. */
#define   AES_WPMR_ACTION_LOCK_ANY_EV (0x3u << 5) /**< \brief (AES_WPMR) If a processing is in progress when the AES_WPSR.PKRPVS/WPVS/CGD/SEQE/SWE events detection occurs, the current processing is ended normally but no other processing is started while a AES_CR.UNLOCK command is issued. */
#define   AES_WPMR_ACTION_CLEAR_PKRPVS_WPVS_SWE (0x4u << 5) /**< \brief (AES_WPMR) If a processing is in progress when the AES_WPSR.PKRPVS/WPVS/SWE events detection occurs, the current processing is ended normally but no other processing is started while a AES_CR.UNLOCK command is issued. Moreover, the AES_KEYWRx key is immediately cleared. */
#define   AES_WPMR_ACTION_CLEAR_CGD_SEQE (0x5u << 5) /**< \brief (AES_WPMR) If a processing is in progress when the AES_WPSR.CGD/SEQE events detection occurs, the current processing is ended normally but no other processing is started while a AES_CR.UNLOCK command is issued. Moreover, the AES_KEYWRx key is immediately cleared. */
#define   AES_WPMR_ACTION_CLEAR_ANY_EV (0x6u << 5) /**< \brief (AES_WPMR) If a processing is in progress when the AES_WPSR.PKRPVS/WPVS/CGD/SEQE/SWE events detection occurs, the current processing is ended normally but no other processing is started while a AES_CR.UNLOCK command is issued. Moreover, the AES_KEYWRx key is immediately cleared. */
#define AES_WPMR_WPKEY_Pos 8
#define AES_WPMR_WPKEY_Msk (0xffffffu << AES_WPMR_WPKEY_Pos) /**< \brief (AES_WPMR) Write Protection Key */
#define AES_WPMR_WPKEY(value) ((AES_WPMR_WPKEY_Msk & ((value) << AES_WPMR_WPKEY_Pos)))
#define   AES_WPMR_WPKEY_PASSWD (0x414553u << 8) /**< \brief (AES_WPMR) Writing any other value in this field aborts the write operation of the WPEN,WPITEN,WPCREN bits. Always reads as 0. */
/* -------- AES_WPSR : (AES Offset: 0xE8) Write Protection Status Register -------- */
#define AES_WPSR_WPVS (0x1u << 0) /**< \brief (AES_WPSR) Write Protection Violation Status (cleared on read) */
#define AES_WPSR_CGD (0x1u << 1) /**< \brief (AES_WPSR) Clock Glitch Detected (cleared on read) */
#define AES_WPSR_SEQE (0x1u << 2) /**< \brief (AES_WPSR) Internal Sequencer Error (cleared on read) */
#define AES_WPSR_SWE (0x1u << 3) /**< \brief (AES_WPSR) Software Control Error (cleared on read) */
#define AES_WPSR_PKRPVS (0x1u << 4) /**< \brief (AES_WPSR) Private Key Internal Register Protection Violation Status (cleared on read) */
#define AES_WPSR_WPVSRC_Pos 8
#define AES_WPSR_WPVSRC_Msk (0xffu << AES_WPSR_WPVSRC_Pos) /**< \brief (AES_WPSR) Write Protection Violation Source */
#define AES_WPSR_SWETYP_Pos 24
#define AES_WPSR_SWETYP_Msk (0xfu << AES_WPSR_SWETYP_Pos) /**< \brief (AES_WPSR) Software Error Type (cleared on read) */
#define   AES_WPSR_SWETYP_READ_WO (0x0u << 24) /**< \brief (AES_WPSR) A write-only register has been read (Warning). */
#define   AES_WPSR_SWETYP_WRITE_RO (0x1u << 24) /**< \brief (AES_WPSR) AES is enabled and a write access has been performed on a read-only register (Warning). */
#define   AES_WPSR_SWETYP_UNDEF_RW (0x2u << 24) /**< \brief (AES_WPSR) Access to an undefined address (Warning). */
#define   AES_WPSR_SWETYP_CTRL_START (0x3u << 24) /**< \brief (AES_WPSR) Abnormal use of AES_CR.START command when DMA access is configured. */
#define   AES_WPSR_SWETYP_WEIRD_ACTION (0x4u << 24) /**< \brief (AES_WPSR) A key write, init value write, output data read, AES_MR and AES_EMR write, private key bus access has been performed while a current processing is in progress (abnormal). */
#define   AES_WPSR_SWETYP_INCOMPLETE_KEY (0x5u << 24) /**< \brief (AES_WPSR) A tentative of start is required while the key is not fully loaded into the AES_KEYWRx registers. */
#define AES_WPSR_ECLASS (0x1u << 31) /**< \brief (AES_WPSR) Software Error Class (cleared on read) */
#define   AES_WPSR_ECLASS_WARNING (0x0u << 31) /**< \brief (AES_WPSR) An abnormal access that does not affect system functionality */
#define   AES_WPSR_ECLASS_ERROR (0x1u << 31) /**< \brief (AES_WPSR) An access is performed into key, input data, control registers while the AES is performing an encryption/decryption or a start is request by software or DMA while the key is not fully configured. */
/* -------- AES_RPR : (AES Offset: 0x100) Receive Pointer Register -------- */
#define AES_RPR_RXPTR_Pos 0
#define AES_RPR_RXPTR_Msk (0xffffffffu << AES_RPR_RXPTR_Pos) /**< \brief (AES_RPR) Receive Pointer Register */
#define AES_RPR_RXPTR(value) ((AES_RPR_RXPTR_Msk & ((value) << AES_RPR_RXPTR_Pos)))
/* -------- AES_RCR : (AES Offset: 0x104) Receive Counter Register -------- */
#define AES_RCR_RXCTR_Pos 0
#define AES_RCR_RXCTR_Msk (0xffffu << AES_RCR_RXCTR_Pos) /**< \brief (AES_RCR) Receive Counter Register */
#define AES_RCR_RXCTR(value) ((AES_RCR_RXCTR_Msk & ((value) << AES_RCR_RXCTR_Pos)))
/* -------- AES_TPR : (AES Offset: 0x108) Transmit Pointer Register -------- */
#define AES_TPR_TXPTR_Pos 0
#define AES_TPR_TXPTR_Msk (0xffffffffu << AES_TPR_TXPTR_Pos) /**< \brief (AES_TPR) Transmit Counter Register */
#define AES_TPR_TXPTR(value) ((AES_TPR_TXPTR_Msk & ((value) << AES_TPR_TXPTR_Pos)))
/* -------- AES_TCR : (AES Offset: 0x10C) Transmit Counter Register -------- */
#define AES_TCR_TXCTR_Pos 0
#define AES_TCR_TXCTR_Msk (0xffffu << AES_TCR_TXCTR_Pos) /**< \brief (AES_TCR) Transmit Counter Register */
#define AES_TCR_TXCTR(value) ((AES_TCR_TXCTR_Msk & ((value) << AES_TCR_TXCTR_Pos)))
/* -------- AES_RNPR : (AES Offset: 0x110) Receive Next Pointer Register -------- */
#define AES_RNPR_RXNPTR_Pos 0
#define AES_RNPR_RXNPTR_Msk (0xffffffffu << AES_RNPR_RXNPTR_Pos) /**< \brief (AES_RNPR) Receive Next Pointer */
#define AES_RNPR_RXNPTR(value) ((AES_RNPR_RXNPTR_Msk & ((value) << AES_RNPR_RXNPTR_Pos)))
/* -------- AES_RNCR : (AES Offset: 0x114) Receive Next Counter Register -------- */
#define AES_RNCR_RXNCTR_Pos 0
#define AES_RNCR_RXNCTR_Msk (0xffffu << AES_RNCR_RXNCTR_Pos) /**< \brief (AES_RNCR) Receive Next Counter */
#define AES_RNCR_RXNCTR(value) ((AES_RNCR_RXNCTR_Msk & ((value) << AES_RNCR_RXNCTR_Pos)))
/* -------- AES_TNPR : (AES Offset: 0x118) Transmit Next Pointer Register -------- */
#define AES_TNPR_TXNPTR_Pos 0
#define AES_TNPR_TXNPTR_Msk (0xffffffffu << AES_TNPR_TXNPTR_Pos) /**< \brief (AES_TNPR) Transmit Next Pointer */
#define AES_TNPR_TXNPTR(value) ((AES_TNPR_TXNPTR_Msk & ((value) << AES_TNPR_TXNPTR_Pos)))
/* -------- AES_TNCR : (AES Offset: 0x11C) Transmit Next Counter Register -------- */
#define AES_TNCR_TXNCTR_Pos 0
#define AES_TNCR_TXNCTR_Msk (0xffffu << AES_TNCR_TXNCTR_Pos) /**< \brief (AES_TNCR) Transmit Counter Next */
#define AES_TNCR_TXNCTR(value) ((AES_TNCR_TXNCTR_Msk & ((value) << AES_TNCR_TXNCTR_Pos)))
/* -------- AES_PTCR : (AES Offset: 0x120) Transfer Control Register -------- */
#define AES_PTCR_RXTEN (0x1u << 0) /**< \brief (AES_PTCR) Receiver Transfer Enable */
#define AES_PTCR_RXTDIS (0x1u << 1) /**< \brief (AES_PTCR) Receiver Transfer Disable */
#define AES_PTCR_TXTEN (0x1u << 8) /**< \brief (AES_PTCR) Transmitter Transfer Enable */
#define AES_PTCR_TXTDIS (0x1u << 9) /**< \brief (AES_PTCR) Transmitter Transfer Disable */
#define AES_PTCR_RXCBEN (0x1u << 16) /**< \brief (AES_PTCR) Receiver Circular Buffer Enable */
#define AES_PTCR_RXCBDIS (0x1u << 17) /**< \brief (AES_PTCR) Receiver Circular Buffer Disable */
#define AES_PTCR_TXCBEN (0x1u << 18) /**< \brief (AES_PTCR) Transmitter Circular Buffer Enable */
#define AES_PTCR_TXCBDIS (0x1u << 19) /**< \brief (AES_PTCR) Transmitter Circular Buffer Disable */
#define AES_PTCR_ERRCLR (0x1u << 24) /**< \brief (AES_PTCR) Transfer Bus Error Clear */
/* -------- AES_PTSR : (AES Offset: 0x124) Transfer Status Register -------- */
#define AES_PTSR_RXTEN (0x1u << 0) /**< \brief (AES_PTSR) Receiver Transfer Enable */
#define AES_PTSR_TXTEN (0x1u << 8) /**< \brief (AES_PTSR) Transmitter Transfer Enable */
#define AES_PTSR_RXCBEN (0x1u << 16) /**< \brief (AES_PTSR) Receiver Circular Buffer Enable */
#define AES_PTSR_TXCBEN (0x1u << 18) /**< \brief (AES_PTSR) Transmitter Circular Buffer Enable */
#define AES_PTSR_ERR (0x1u << 24) /**< \brief (AES_PTSR) Transfer Bus Error */
/* -------- AES_PWPMR : (AES Offset: 0x128) Write Protection Mode Register -------- */
#define AES_PWPMR_WPPTREN (0x1u << 0) /**< \brief (AES_PWPMR) Write Protection Pointer Registers Enable */
#define AES_PWPMR_WPCTREN (0x1u << 1) /**< \brief (AES_PWPMR) Write Protection Counter Registers Enable */
#define AES_PWPMR_WPCREN (0x1u << 2) /**< \brief (AES_PWPMR) Write Protection Control Register Enable */
#define AES_PWPMR_WPKEY_Pos 8
#define AES_PWPMR_WPKEY_Msk (0xffffffu << AES_PWPMR_WPKEY_Pos) /**< \brief (AES_PWPMR) Write Protection Key */
#define AES_PWPMR_WPKEY(value) ((AES_PWPMR_WPKEY_Msk & ((value) << AES_PWPMR_WPKEY_Pos)))
#define   AES_PWPMR_WPKEY_PASSWD (0x504443u << 8) /**< \brief (AES_PWPMR) Writing any other value in this field aborts the write operation. Always reads as 0. */

/*@}*/


#endif /* _PIC32CX_AES_COMPONENT_ */
