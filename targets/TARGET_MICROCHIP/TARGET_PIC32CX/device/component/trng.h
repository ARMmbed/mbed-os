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

#ifndef _PIC32CX_TRNG_COMPONENT_
#define _PIC32CX_TRNG_COMPONENT_

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR True Random Number Generator */
/* ============================================================================= */
/** \addtogroup PIC32CX_TRNG True Random Number Generator */
/*@{*/

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
/** \brief Trng hardware registers */
typedef struct {
  __O  uint32_t TRNG_CR;       /**< \brief (Trng Offset: 0x0) Control Register */
  __IO uint32_t TRNG_MR;       /**< \brief (Trng Offset: 0x4) Mode Register */
  __O  uint32_t TRNG_PKBCR;    /**< \brief (Trng Offset: 0x8) Private Key Bus Control Register */
  __I  uint32_t Reserved1[1];
  __O  uint32_t TRNG_IER;      /**< \brief (Trng Offset: 0x10) Interrupt Enable Register */
  __O  uint32_t TRNG_IDR;      /**< \brief (Trng Offset: 0x14) Interrupt Disable Register */
  __I  uint32_t TRNG_IMR;      /**< \brief (Trng Offset: 0x18) Interrupt Mask Register */
  __I  uint32_t TRNG_ISR;      /**< \brief (Trng Offset: 0x1C) Interrupt Status Register */
  __I  uint32_t Reserved2[12];
  __I  uint32_t TRNG_ODATA;    /**< \brief (Trng Offset: 0x50) Output Data Register */
  __I  uint32_t Reserved3[36];
  __IO uint32_t TRNG_WPMR;     /**< \brief (Trng Offset: 0xE4) Write Protection Mode Register */
  __I  uint32_t TRNG_WPSR;     /**< \brief (Trng Offset: 0xE8) Write Protection Status Register */
} Trng;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/* -------- TRNG_CR : (TRNG Offset: 0x0) Control Register -------- */
#define TRNG_CR_ENABLE (0x1u << 0) /**< \brief (TRNG_CR) Enable TRNG to Provide Random Values */
#define TRNG_CR_WAKEY_Pos 8
#define TRNG_CR_WAKEY_Msk (0xffffffu << TRNG_CR_WAKEY_Pos) /**< \brief (TRNG_CR) Register Write Access Key */
#define TRNG_CR_WAKEY(value) ((TRNG_CR_WAKEY_Msk & ((value) << TRNG_CR_WAKEY_Pos)))
#define   TRNG_CR_WAKEY_PASSWD (0x524E47u << 8) /**< \brief (TRNG_CR) Writing any other value in this field aborts the write operation. */
/* -------- TRNG_MR : (TRNG Offset: 0x4) Mode Register -------- */
#define TRNG_MR_HALFR (0x1u << 0) /**< \brief (TRNG_MR) Half Rate Enable */
/* -------- TRNG_PKBCR : (TRNG Offset: 0x8) Private Key Bus Control Register -------- */
#define TRNG_PKBCR_KID (0x1u << 0) /**< \brief (TRNG_PKBCR) Key ID (Must be Always Written to 0) */
#define TRNG_PKBCR_KSLAVE_Pos 4
#define TRNG_PKBCR_KSLAVE_Msk (0x3u << TRNG_PKBCR_KSLAVE_Pos) /**< \brief (TRNG_PKBCR) Key Bus Slave */
#define TRNG_PKBCR_KSLAVE(value) ((TRNG_PKBCR_KSLAVE_Msk & ((value) << TRNG_PKBCR_KSLAVE_Pos)))
#define   TRNG_PKBCR_KSLAVE_AES_ID (0x0u << 4) /**< \brief (TRNG_PKBCR) AES */
#define   TRNG_PKBCR_KSLAVE_AESB_ID (0x1u << 4) /**< \brief (TRNG_PKBCR) AESB */
#define TRNG_PKBCR_KLENGTH_Pos 8
#define TRNG_PKBCR_KLENGTH_Msk (0xffu << TRNG_PKBCR_KLENGTH_Pos) /**< \brief (TRNG_PKBCR) Key Length */
#define TRNG_PKBCR_KLENGTH(value) ((TRNG_PKBCR_KLENGTH_Msk & ((value) << TRNG_PKBCR_KLENGTH_Pos)))
#define TRNG_PKBCR_WAKEY_Pos 16
#define TRNG_PKBCR_WAKEY_Msk (0xffffu << TRNG_PKBCR_WAKEY_Pos) /**< \brief (TRNG_PKBCR) Register Write Access Key */
#define TRNG_PKBCR_WAKEY(value) ((TRNG_PKBCR_WAKEY_Msk & ((value) << TRNG_PKBCR_WAKEY_Pos)))
#define   TRNG_PKBCR_WAKEY_PASSWD (0x524Bu << 16) /**< \brief (TRNG_PKBCR) Writing any other value in this field aborts the write operation. */
/* -------- TRNG_IER : (TRNG Offset: 0x10) Interrupt Enable Register -------- */
#define TRNG_IER_DATRDY (0x1u << 0) /**< \brief (TRNG_IER) Data Ready Interrupt Enable */
#define TRNG_IER_SECE (0x1u << 1) /**< \brief (TRNG_IER) Security and/or Safety Event Interrupt Enable */
#define TRNG_IER_EOTPKB (0x1u << 2) /**< \brief (TRNG_IER) End Of Transfer on Private Key Bus Interrupt Enable */
/* -------- TRNG_IDR : (TRNG Offset: 0x14) Interrupt Disable Register -------- */
#define TRNG_IDR_DATRDY (0x1u << 0) /**< \brief (TRNG_IDR) Data Ready Interrupt Disable */
#define TRNG_IDR_SECE (0x1u << 1) /**< \brief (TRNG_IDR) Security and/or Safety Event Interrupt Disable */
#define TRNG_IDR_EOTPKB (0x1u << 2) /**< \brief (TRNG_IDR) End Of Transfer on Private Key Bus Interrupt Disable */
/* -------- TRNG_IMR : (TRNG Offset: 0x18) Interrupt Mask Register -------- */
#define TRNG_IMR_DATRDY (0x1u << 0) /**< \brief (TRNG_IMR) Data Ready Interrupt Mask */
#define TRNG_IMR_SECE (0x1u << 1) /**< \brief (TRNG_IMR) Security and/or Safety Event Interrupt Mask */
#define TRNG_IMR_EOTPKB (0x1u << 2) /**< \brief (TRNG_IMR) End Of Transfer on Private Key Bus Interrupt Mask */
/* -------- TRNG_ISR : (TRNG Offset: 0x1C) Interrupt Status Register -------- */
#define TRNG_ISR_DATRDY (0x1u << 0) /**< \brief (TRNG_ISR) Data Ready (cleared on read) */
#define TRNG_ISR_SECE (0x1u << 1) /**< \brief (TRNG_ISR) Security and/or Safety Event (cleared on read) */
#define TRNG_ISR_EOTPKB (0x1u << 2) /**< \brief (TRNG_ISR) End Of Transfer on Private Key Bus (cleared on read) */
/* -------- TRNG_ODATA : (TRNG Offset: 0x50) Output Data Register -------- */
#define TRNG_ODATA_ODATA_Pos 0
#define TRNG_ODATA_ODATA_Msk (0xffffffffu << TRNG_ODATA_ODATA_Pos) /**< \brief (TRNG_ODATA) Output Data */
/* -------- TRNG_WPMR : (TRNG Offset: 0xE4) Write Protection Mode Register -------- */
#define TRNG_WPMR_WPEN (0x1u << 0) /**< \brief (TRNG_WPMR) Write Protection Enable */
#define TRNG_WPMR_WPITEN (0x1u << 1) /**< \brief (TRNG_WPMR) Write Protection Interrupt Enable */
#define TRNG_WPMR_WPCREN (0x1u << 2) /**< \brief (TRNG_WPMR) Write Protection Control Enable */
#define TRNG_WPMR_FIRSTE (0x1u << 4) /**< \brief (TRNG_WPMR) First Error Report Enable */
#define TRNG_WPMR_WPKEY_Pos 8
#define TRNG_WPMR_WPKEY_Msk (0xffffffu << TRNG_WPMR_WPKEY_Pos) /**< \brief (TRNG_WPMR) Write Protection Key */
#define TRNG_WPMR_WPKEY(value) ((TRNG_WPMR_WPKEY_Msk & ((value) << TRNG_WPMR_WPKEY_Pos)))
#define   TRNG_WPMR_WPKEY_PASSWD (0x524E47u << 8) /**< \brief (TRNG_WPMR) Writing any other value in this field aborts the write operation of bits WPEN, WPITEN and WPCREN. Always reads as 0. */
/* -------- TRNG_WPSR : (TRNG Offset: 0xE8) Write Protection Status Register -------- */
#define TRNG_WPSR_WPVS (0x1u << 0) /**< \brief (TRNG_WPSR) Write Protection Violation Status (cleared on read) */
#define TRNG_WPSR_CGD (0x1u << 1) /**< \brief (TRNG_WPSR) Clock Glitch Detected (cleared on read) */
#define TRNG_WPSR_SEQE (0x1u << 2) /**< \brief (TRNG_WPSR) Internal Sequencer Error (cleared on read) */
#define TRNG_WPSR_SWE (0x1u << 3) /**< \brief (TRNG_WPSR) Software Control Error (cleared on read) */
#define TRNG_WPSR_WPVSRC_Pos 8
#define TRNG_WPSR_WPVSRC_Msk (0xffffu << TRNG_WPSR_WPVSRC_Pos) /**< \brief (TRNG_WPSR) Write Protection Violation Source (cleared on read) */
#define TRNG_WPSR_SWETYP_Pos 24
#define TRNG_WPSR_SWETYP_Msk (0xfu << TRNG_WPSR_SWETYP_Pos) /**< \brief (TRNG_WPSR) Software Error Type (cleared on read) */
#define   TRNG_WPSR_SWETYP_READ_WO (0x0u << 24) /**< \brief (TRNG_WPSR) TRNG is enabled and a write-only register has been read (Warning). */
#define   TRNG_WPSR_SWETYP_WRITE_RO (0x1u << 24) /**< \brief (TRNG_WPSR) TRNG is enabled and a write access has been performed on a read-only register (Warning). */
#define   TRNG_WPSR_SWETYP_UNDEF_RW (0x2u << 24) /**< \brief (TRNG_WPSR) Access to an undefined address. */
#define   TRNG_WPSR_SWETYP_TRNG_DIS (0x3u << 24) /**< \brief (TRNG_WPSR) The TRNG_ODATA register was read when TRNG was disabled or TRNG used for private key bus transfer (Error). */
#define   TRNG_WPSR_SWETYP_PKB_BUSY (0x4u << 24) /**< \brief (TRNG_WPSR) A write access to TRNG_PKBCR has been attempted during a private key bus transfer (Error). */
#define TRNG_WPSR_ECLASS (0x1u << 31) /**< \brief (TRNG_WPSR) Software Error Class (cleared on read) */
#define   TRNG_WPSR_ECLASS_WARNING (0x0u << 31) /**< \brief (TRNG_WPSR) An abnormal access that does not affect system functionality. */
#define   TRNG_WPSR_ECLASS_ERROR (0x1u << 31) /**< \brief (TRNG_WPSR) Reading TRNG_ODATA when TRNG was disabled or TRNG used for private key bus transfer. TRNG does not provide a random value. Writing to TRNG_PKBCR while a private key bus transfer is ongoing does not launch a new private key bus transfer. */

/*@}*/


#endif /* _PIC32CX_TRNG_COMPONENT_ */
