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

#ifndef _PIC32CX_AESB_COMPONENT_
#define _PIC32CX_AESB_COMPONENT_

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR Advanced Encryption Standard Bridge */
/* ============================================================================= */
/** \addtogroup PIC32CX_AESB Advanced Encryption Standard Bridge */
/*@{*/

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
/** \brief Aesb hardware registers */
typedef struct {
  __O  uint32_t AESB_CR;        /**< \brief (Aesb Offset: 0x0) Control Register */
  __IO uint32_t AESB_MR;        /**< \brief (Aesb Offset: 0x4) Mode Register */
  __I  uint32_t Reserved1[2];
  __O  uint32_t AESB_IER;       /**< \brief (Aesb Offset: 0x10) Interrupt Enable Register */
  __O  uint32_t AESB_IDR;       /**< \brief (Aesb Offset: 0x14) Interrupt Disable Register */
  __I  uint32_t AESB_IMR;       /**< \brief (Aesb Offset: 0x18) Interrupt Mask Register */
  __I  uint32_t AESB_ISR;       /**< \brief (Aesb Offset: 0x1C) Interrupt Status Register */
  __O  uint32_t AESB_KEYWR[4];  /**< \brief (Aesb Offset: 0x20) Key Word Register */
  __I  uint32_t Reserved2[4];
  __O  uint32_t AESB_IDATAR[4]; /**< \brief (Aesb Offset: 0x40) Input Data Register */
  __I  uint32_t AESB_ODATAR[4]; /**< \brief (Aesb Offset: 0x50) Output Data Register */
  __O  uint32_t AESB_IVR[4];    /**< \brief (Aesb Offset: 0x60) Initialization Vector Register */
  __I  uint32_t Reserved3[16];
  __IO uint32_t AESB_EMR;       /**< \brief (Aesb Offset: 0xB0) Extended Mode Register */
  __I  uint32_t Reserved4[12];
  __IO uint32_t AESB_WPMR;      /**< \brief (Aesb Offset: 0xE4) Write Protection Mode Register */
  __I  uint32_t AESB_WPSR;      /**< \brief (Aesb Offset: 0xE8) Write Protection Status Register */
} Aesb;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/* -------- AESB_CR : (AESB Offset: 0x0) Control Register -------- */
#define AESB_CR_START (0x1u << 0) /**< \brief (AESB_CR) Start Processing */
#define AESB_CR_SWRST (0x1u << 8) /**< \brief (AESB_CR) Software Reset */
#define AESB_CR_UNLOCK (0x1u << 24) /**< \brief (AESB_CR) Unlock Processing */
/* -------- AESB_MR : (AESB Offset: 0x4) Mode Register -------- */
#define AESB_MR_CIPHER (0x1u << 0) /**< \brief (AESB_MR) Processing Mode */
#define AESB_MR_AAHB (0x1u << 2) /**< \brief (AESB_MR) Automatic Bridge Mode */
#define AESB_MR_DUALBUFF (0x1u << 3) /**< \brief (AESB_MR) Dual Input Buffer */
#define   AESB_MR_DUALBUFF_INACTIVE (0x0u << 3) /**< \brief (AESB_MR) AESB_IDATARx cannot be written during processing of previous block. */
#define   AESB_MR_DUALBUFF_ACTIVE (0x1u << 3) /**< \brief (AESB_MR) AESB_IDATARx can be written during processing of previous block when SMOD = 0x2. It speeds up the overall runtime of large files. */
#define AESB_MR_PROCDLY_Pos 4
#define AESB_MR_PROCDLY_Msk (0xfu << AESB_MR_PROCDLY_Pos) /**< \brief (AESB_MR) Processing Delay */
#define AESB_MR_PROCDLY(value) ((AESB_MR_PROCDLY_Msk & ((value) << AESB_MR_PROCDLY_Pos)))
#define AESB_MR_SMOD_Pos 8
#define AESB_MR_SMOD_Msk (0x3u << AESB_MR_SMOD_Pos) /**< \brief (AESB_MR) Start Mode */
#define AESB_MR_SMOD(value) ((AESB_MR_SMOD_Msk & ((value) << AESB_MR_SMOD_Pos)))
#define   AESB_MR_SMOD_MANUAL_START (0x0u << 8) /**< \brief (AESB_MR) Manual mode */
#define   AESB_MR_SMOD_AUTO_START (0x1u << 8) /**< \brief (AESB_MR) Auto mode */
#define   AESB_MR_SMOD_IDATAR0_START (0x2u << 8) /**< \brief (AESB_MR) AESB_IDATAR0 access only Auto mode */
#define AESB_MR_OPMOD_Pos 12
#define AESB_MR_OPMOD_Msk (0x7u << AESB_MR_OPMOD_Pos) /**< \brief (AESB_MR) Operating Mode */
#define AESB_MR_OPMOD(value) ((AESB_MR_OPMOD_Msk & ((value) << AESB_MR_OPMOD_Pos)))
#define   AESB_MR_OPMOD_ECB (0x0u << 12) /**< \brief (AESB_MR) Electronic Code Book mode */
#define   AESB_MR_OPMOD_CBC (0x1u << 12) /**< \brief (AESB_MR) Cipher Block Chaining mode */
#define   AESB_MR_OPMOD_CTR (0x4u << 12) /**< \brief (AESB_MR) Counter mode (16-bit internal counter) */
#define AESB_MR_LOD (0x1u << 15) /**< \brief (AESB_MR) Last Output Data Mode */
#define AESB_MR_CKEY_Pos 20
#define AESB_MR_CKEY_Msk (0xfu << AESB_MR_CKEY_Pos) /**< \brief (AESB_MR) Key */
#define AESB_MR_CKEY(value) ((AESB_MR_CKEY_Msk & ((value) << AESB_MR_CKEY_Pos)))
#define   AESB_MR_CKEY_PASSWD (0xEu << 20) /**< \brief (AESB_MR) Must be written with 0xE the first time that AESB_MR is programmed. For subsequent programming of AESB_MR, any value can be written, including that of 0xE. Always reads as 0. */
#define AESB_MR_TAMPCLR (0x1u << 31) /**< \brief (AESB_MR) Tamper Clear Enable */
/* -------- AESB_IER : (AESB Offset: 0x10) Interrupt Enable Register -------- */
#define AESB_IER_DATRDY (0x1u << 0) /**< \brief (AESB_IER) Data Ready Interrupt Enable */
#define AESB_IER_URAD (0x1u << 8) /**< \brief (AESB_IER) Unspecified Register Access Detection Interrupt Enable */
#define AESB_IER_SECE (0x1u << 19) /**< \brief (AESB_IER) Security and/or Safety Event */
/* -------- AESB_IDR : (AESB Offset: 0x14) Interrupt Disable Register -------- */
#define AESB_IDR_DATRDY (0x1u << 0) /**< \brief (AESB_IDR) Data Ready Interrupt Disable */
#define AESB_IDR_URAD (0x1u << 8) /**< \brief (AESB_IDR) Unspecified Register Access Detection Interrupt Disable */
#define AESB_IDR_SECE (0x1u << 19) /**< \brief (AESB_IDR) Security and/or Safety Event */
/* -------- AESB_IMR : (AESB Offset: 0x18) Interrupt Mask Register -------- */
#define AESB_IMR_DATRDY (0x1u << 0) /**< \brief (AESB_IMR) Data Ready Interrupt Mask */
#define AESB_IMR_URAD (0x1u << 8) /**< \brief (AESB_IMR) Unspecified Register Access Detection Interrupt Mask */
#define AESB_IMR_SECE (0x1u << 19) /**< \brief (AESB_IMR) Security and/or Safety Event */
/* -------- AESB_ISR : (AESB Offset: 0x1C) Interrupt Status Register -------- */
#define AESB_ISR_DATRDY (0x1u << 0) /**< \brief (AESB_ISR) Data Ready */
#define AESB_ISR_URAD (0x1u << 8) /**< \brief (AESB_ISR) Unspecified Register Access Detection Status */
#define AESB_ISR_URAT_Pos 12
#define AESB_ISR_URAT_Msk (0xfu << AESB_ISR_URAT_Pos) /**< \brief (AESB_ISR) Unspecified Register Access */
#define   AESB_ISR_URAT_IDR_WR_PROCESSING (0x0u << 12) /**< \brief (AESB_ISR) Input Data register written during the data processing when SMOD = 0x2 mode */
#define   AESB_ISR_URAT_ODR_RD_PROCESSING (0x1u << 12) /**< \brief (AESB_ISR) Output Data register read during the data processing */
#define   AESB_ISR_URAT_MR_WR_PROCESSING (0x2u << 12) /**< \brief (AESB_ISR) Mode register written during the data processing */
#define   AESB_ISR_URAT_ODR_RD_SUBKGEN (0x3u << 12) /**< \brief (AESB_ISR) Output Data register read during the sub-keys generation */
#define   AESB_ISR_URAT_MR_WR_SUBKGEN (0x4u << 12) /**< \brief (AESB_ISR) Mode register written during the sub-keys generation */
#define   AESB_ISR_URAT_WOR_RD_ACCESS (0x5u << 12) /**< \brief (AESB_ISR) Write-only register read access */
#define AESB_ISR_SECE (0x1u << 19) /**< \brief (AESB_ISR) Security and/or Safety Event */
/* -------- AESB_KEYWR[4] : (AESB Offset: 0x20) Key Word Register -------- */
#define AESB_KEYWR_KEYW_Pos 0
#define AESB_KEYWR_KEYW_Msk (0xffffffffu << AESB_KEYWR_KEYW_Pos) /**< \brief (AESB_KEYWR[4]) Key Word */
#define AESB_KEYWR_KEYW(value) ((AESB_KEYWR_KEYW_Msk & ((value) << AESB_KEYWR_KEYW_Pos)))
/* -------- AESB_IDATAR[4] : (AESB Offset: 0x40) Input Data Register -------- */
#define AESB_IDATAR_IDATA_Pos 0
#define AESB_IDATAR_IDATA_Msk (0xffffffffu << AESB_IDATAR_IDATA_Pos) /**< \brief (AESB_IDATAR[4]) Input Data Word */
#define AESB_IDATAR_IDATA(value) ((AESB_IDATAR_IDATA_Msk & ((value) << AESB_IDATAR_IDATA_Pos)))
/* -------- AESB_ODATAR[4] : (AESB Offset: 0x50) Output Data Register -------- */
#define AESB_ODATAR_ODATA_Pos 0
#define AESB_ODATAR_ODATA_Msk (0xffffffffu << AESB_ODATAR_ODATA_Pos) /**< \brief (AESB_ODATAR[4]) Output Data */
/* -------- AESB_IVR[4] : (AESB Offset: 0x60) Initialization Vector Register -------- */
#define AESB_IVR_IV_Pos 0
#define AESB_IVR_IV_Msk (0xffffffffu << AESB_IVR_IV_Pos) /**< \brief (AESB_IVR[4]) Initialization Vector */
#define AESB_IVR_IV(value) ((AESB_IVR_IV_Msk & ((value) << AESB_IVR_IV_Pos)))
/* -------- AESB_EMR : (AESB Offset: 0xB0) Extended Mode Register -------- */
#define AESB_EMR_PKWO (0x1u << 6) /**< \brief (AESB_EMR) Private Key Write Once */
#define AESB_EMR_PKRS (0x1u << 7) /**< \brief (AESB_EMR) Private Key Internal Register Select */
/* -------- AESB_WPMR : (AESB Offset: 0xE4) Write Protection Mode Register -------- */
#define AESB_WPMR_WPEN (0x1u << 0) /**< \brief (AESB_WPMR) Write Protection Configuration Enable */
#define AESB_WPMR_WPITEN (0x1u << 1) /**< \brief (AESB_WPMR) Write Protection Interruption Enable */
#define AESB_WPMR_WPCREN (0x1u << 2) /**< \brief (AESB_WPMR) Write Protection Control Enable */
#define AESB_WPMR_FIRSTE (0x1u << 4) /**< \brief (AESB_WPMR) First Error Report Enable */
#define AESB_WPMR_ACTION_Pos 5
#define AESB_WPMR_ACTION_Msk (0x7u << AESB_WPMR_ACTION_Pos) /**< \brief (AESB_WPMR) Action on Abnormal Event Detection */
#define AESB_WPMR_ACTION(value) ((AESB_WPMR_ACTION_Msk & ((value) << AESB_WPMR_ACTION_Pos)))
#define   AESB_WPMR_ACTION_REPORT_ONLY (0x0u << 5) /**< \brief (AESB_WPMR) No action (stop or clear key) is performed when one of PKRPVS, WPVS, CGD, SEQE, or SWE flags is set. */
#define   AESB_WPMR_ACTION_LOCK_PKRPVS_WPVS_SWE (0x1u << 5) /**< \brief (AESB_WPMR) If a processing is in progress when the AESB_WPSR.PKRPVS/WPVS/SWE event detection occurs, the current processing is ended normally but no other processing is started while a AESB_CR.UNLOCK command is issued. */
#define   AESB_WPMR_ACTION_LOCK_CGD_SEQE (0x2u << 5) /**< \brief (AESB_WPMR) If a processing is in progress when the AESB_WPSR.CGD/SEQE event detection occurs, the current processing is ended normally but no other processing is started while a AESB_CR.UNLOCK command is issued. */
#define   AESB_WPMR_ACTION_LOCK_ANY_EV (0x3u << 5) /**< \brief (AESB_WPMR) If a processing is in progress when the AESB_WPSR.PKRPVS/WPVS/CGD/SEQE/SWE events detection occurs, the current processing is ended normally but no other processing is started while a AESB_CR.UNLOCK command is issued. */
#define   AESB_WPMR_ACTION_CLEAR_PKRPVS_WPVS_SWE (0x4u << 5) /**< \brief (AESB_WPMR) If a processing is in progress when the AESB_WPSR.PKRPVS/WPVS/SWE events detection occurs, the current processing is ended normally but no other processing is started while a AESB_CR.UNLOCK command is issued. Moreover, the AESB_KEYWRx key is immediately cleared. */
#define   AESB_WPMR_ACTION_CLEAR_CGD_SEQE (0x5u << 5) /**< \brief (AESB_WPMR) If a processing is in progress when the AESB_WPSR.CGD/SEQE events detection occurs, the current processing is ended normally but no other processing is started while a AESB_CR.UNLOCK command is issued. Moreover, the AESB_KEYWRx key is immediately cleared. */
#define   AESB_WPMR_ACTION_CLEAR_ANY_EV (0x6u << 5) /**< \brief (AESB_WPMR) If a processing is in progress when the AESB_WPSR.PKRPVS/WPVS/CGD/SEQE/SWE events detection occurs, the current processing is ended normally but no other processing is started while a AESB_CR.UNLOCK command is issued. Moreover, the AESB_KEYWRx key is immediately cleared. */
#define AESB_WPMR_WPKEY_Pos 8
#define AESB_WPMR_WPKEY_Msk (0xffffffu << AESB_WPMR_WPKEY_Pos) /**< \brief (AESB_WPMR) Write Protection Key */
#define AESB_WPMR_WPKEY(value) ((AESB_WPMR_WPKEY_Msk & ((value) << AESB_WPMR_WPKEY_Pos)))
#define   AESB_WPMR_WPKEY_PASSWD (0x414553u << 8) /**< \brief (AESB_WPMR) Writing any other value in this field aborts the write operation of the WPEN,WPITEN,WPCREN bits. Always reads as 0. */
/* -------- AESB_WPSR : (AESB Offset: 0xE8) Write Protection Status Register -------- */
#define AESB_WPSR_WPVS (0x1u << 0) /**< \brief (AESB_WPSR) Write Protection Violation Status (cleared on read) */
#define AESB_WPSR_CGD (0x1u << 1) /**< \brief (AESB_WPSR) Clock Glitch Detected (cleared on read) */
#define AESB_WPSR_SEQE (0x1u << 2) /**< \brief (AESB_WPSR) Internal Sequencer Error (cleared on read) */
#define AESB_WPSR_SWE (0x1u << 3) /**< \brief (AESB_WPSR) Software Control Error (cleared on read) */
#define AESB_WPSR_PKRPVS (0x1u << 4) /**< \brief (AESB_WPSR) Private Key Internal Register Protection Violation Status (cleared on read) */
#define AESB_WPSR_WPVSRC_Pos 8
#define AESB_WPSR_WPVSRC_Msk (0xffu << AESB_WPSR_WPVSRC_Pos) /**< \brief (AESB_WPSR) Write Protection Violation Source */
#define AESB_WPSR_SWETYP_Pos 24
#define AESB_WPSR_SWETYP_Msk (0xfu << AESB_WPSR_SWETYP_Pos) /**< \brief (AESB_WPSR) Software Error Type (cleared on read) */
#define   AESB_WPSR_SWETYP_READ_WO (0x0u << 24) /**< \brief (AESB_WPSR) A write-only register has been read (Warning). */
#define   AESB_WPSR_SWETYP_WRITE_RO (0x1u << 24) /**< \brief (AESB_WPSR) AESB is enabled and a write access has been performed on a read-only register (Warning). */
#define   AESB_WPSR_SWETYP_UNDEF_RW (0x2u << 24) /**< \brief (AESB_WPSR) Access to an undefined address (Warning). */
#define   AESB_WPSR_SWETYP_CTRL_START (0x3u << 24) /**< \brief (AESB_WPSR) Abnormal use of AESB_CR.START command when DMA access is configured. */
#define   AESB_WPSR_SWETYP_WEIRD_ACTION (0x4u << 24) /**< \brief (AESB_WPSR) A Private Key Bus access, key write, init value write, output data read or AESB_MR and AESB_EMR write has been performed while a process is in progress (abnormal). */
#define   AESB_WPSR_SWETYP_INCOMPLETE_KEY (0x5u << 24) /**< \brief (AESB_WPSR) A tentative of start is required while the key is not fully loaded into the AESB_KEYWRx registers. */
#define AESB_WPSR_ECLASS (0x1u << 31) /**< \brief (AESB_WPSR) Software Error Class (cleared on read) */
#define   AESB_WPSR_ECLASS_WARNING (0x0u << 31) /**< \brief (AESB_WPSR) An abnormal access that does not affect system functionality */
#define   AESB_WPSR_ECLASS_ERROR (0x1u << 31) /**< \brief (AESB_WPSR) An access is performed into key, input data, control registers while the AESB is performing an encryption/decryption or a start is request by software or DMA while the key is not fully configured. */

/*@}*/


#endif /* _PIC32CX_AESB_COMPONENT_ */
