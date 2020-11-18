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

#ifndef _PIC32CX_MATRIX_COMPONENT_
#define _PIC32CX_MATRIX_COMPONENT_

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR AHB Bus Matrix */
/* ============================================================================= */
/** \addtogroup PIC32CX_MATRIX AHB Bus Matrix */
/*@{*/

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
/** \brief MatrixPr hardware registers */
typedef struct {
  __IO uint32_t MATRIX_PRAS; /**< \brief (MatrixPr Offset: 0x0) Priority Register A for Slave 0 */
  __IO uint32_t MATRIX_PRBS; /**< \brief (MatrixPr Offset: 0x4) Priority Register B for Slave 0 */
} MatrixPr;
/** \brief Matrix hardware registers */
#define MATRIXPR_NUMBER 16
typedef struct {
  __IO uint32_t MATRIX_MCFG[16];            /**< \brief (Matrix Offset: 0x0) Master Configuration Register */
  __IO uint32_t MATRIX_SCFG[16];            /**< \brief (Matrix Offset: 0x40) Slave Configuration Register */
       MatrixPr MATRIX_PR[MATRIXPR_NUMBER]; /**< \brief (Matrix Offset: 0x0080) 0 .. 15 */
  __IO uint32_t MATRIX_MRCR;                /**< \brief (Matrix Offset: 0x100) Master Remap Control Register */
  __I  uint32_t Reserved1[19];
  __O  uint32_t MATRIX_MEIER;               /**< \brief (Matrix Offset: 0x150) Master Error Interrupt Enable Register */
  __O  uint32_t MATRIX_MEIDR;               /**< \brief (Matrix Offset: 0x154) Master Error Interrupt Disable Register */
  __I  uint32_t MATRIX_MEIMR;               /**< \brief (Matrix Offset: 0x158) Master Error Interrupt Mask Register */
  __I  uint32_t MATRIX_MESR;                /**< \brief (Matrix Offset: 0x15C) Master Error Status Register */
  __I  uint32_t MATRIX_MEAR[16];            /**< \brief (Matrix Offset: 0x160) Master 0 Error Address Register */
  __I  uint32_t Reserved2[17];
  __IO uint32_t MATRIX_WPMR;                /**< \brief (Matrix Offset: 0x1E4) Write Protect Mode Register */
  __I  uint32_t MATRIX_WPSR;                /**< \brief (Matrix Offset: 0x1E8) Write Protect Status Register */
  __I  uint32_t Reserved3[5];
  __IO uint32_t MATRIX_PSR[16];             /**< \brief (Matrix Offset: 0x200) Protection Slave 0 Register */
  __IO uint32_t MATRIX_PASSR[16];           /**< \brief (Matrix Offset: 0x240) Protected Areas Split Slave 0 Register */
  __IO uint32_t MATRIX_PRTSR[16];           /**< \brief (Matrix Offset: 0x280) Protected Region Top Slave 0 Register */
  __IO uint32_t MATRIX_PPSELR[3];           /**< \brief (Matrix Offset: 0x2C0) Protected Peripheral Select 1 Register */
} Matrix;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/* -------- MATRIX_MCFG[16] : (MATRIX Offset: 0x0) Master Configuration Register -------- */
#define MATRIX_MCFG_ULBT_Pos 0
#define MATRIX_MCFG_ULBT_Msk (0x7u << MATRIX_MCFG_ULBT_Pos) /**< \brief (MATRIX_MCFG[16]) Undefined Length Burst Type */
#define MATRIX_MCFG_ULBT(value) ((MATRIX_MCFG_ULBT_Msk & ((value) << MATRIX_MCFG_ULBT_Pos)))
#define   MATRIX_MCFG_ULBT_UNLIMITED (0x0u << 0) /**< \brief (MATRIX_MCFG[16]) Unlimited Length Burst-No predicted end of burst is generated, therefore INCR bursts coming from this master can only be broken if the Slave Slot Cycle Limit is reached. If the Slot Cycle Limit is not reached, the burst is normally completed by the master, at the latest, on the next system bus 1 Kbyte address boundary, allowing up to 256-beat word bursts or 128-beat double-word bursts. This value should not be used in the very particular case of a master capable of performing back-to-back undefined length bursts on a single slave, since this could indefinitely freeze the slave arbitration and thus prevent another master from accessing this slave. */
#define   MATRIX_MCFG_ULBT_SINGLE (0x1u << 0) /**< \brief (MATRIX_MCFG[16]) Single Access-The undefined length burst is treated as a succession of single accesses, allowing re-arbitration at each beat of the INCR burst or bursts sequence. */
#define   MATRIX_MCFG_ULBT_4_BEAT (0x2u << 0) /**< \brief (MATRIX_MCFG[16]) 4-beat Burst-The undefined length burst or bursts sequence is split into 4-beat bursts or less, allowing re-arbitration every 4 beats. */
#define   MATRIX_MCFG_ULBT_8_BEAT (0x3u << 0) /**< \brief (MATRIX_MCFG[16]) 8-beat Burst-The undefined length burst or bursts sequence is split into 8-beat bursts or less, allowing re-arbitration every 8 beats. */
#define   MATRIX_MCFG_ULBT_16_BEAT (0x4u << 0) /**< \brief (MATRIX_MCFG[16]) 16-beat Burst-The undefined length burst or bursts sequence is split into 16-beat bursts or less, allowing re-arbitration every 16 beats. */
#define   MATRIX_MCFG_ULBT_32_BEAT (0x5u << 0) /**< \brief (MATRIX_MCFG[16]) 32-beat Burst-The undefined length burst or bursts sequence is split into 32-beat bursts or less, allowing re-arbitration every 32 beats. */
#define   MATRIX_MCFG_ULBT_64_BEAT (0x6u << 0) /**< \brief (MATRIX_MCFG[16]) 64-beat Burst-The undefined length burst or bursts sequence is split into 64-beat bursts or less, allowing re-arbitration every 64 beats. */
#define   MATRIX_MCFG_ULBT_128_BEAT (0x7u << 0) /**< \brief (MATRIX_MCFG[16]) 128-beat Burst-The undefined length burst or bursts sequence is split into 128-beat bursts or less, allowing re-arbitration every 128 beats. Unless duly needed, the ULBT should be left at its default 0 value for power saving. */
/* -------- MATRIX_SCFG[16] : (MATRIX Offset: 0x40) Slave Configuration Register -------- */
#define MATRIX_SCFG_SLOT_CYCLE_Pos 0
#define MATRIX_SCFG_SLOT_CYCLE_Msk (0x1ffu << MATRIX_SCFG_SLOT_CYCLE_Pos) /**< \brief (MATRIX_SCFG[16]) Maximum Bus Grant Duration for Masters */
#define MATRIX_SCFG_SLOT_CYCLE(value) ((MATRIX_SCFG_SLOT_CYCLE_Msk & ((value) << MATRIX_SCFG_SLOT_CYCLE_Pos)))
#define MATRIX_SCFG_DEFMSTR_TYPE_Pos 16
#define MATRIX_SCFG_DEFMSTR_TYPE_Msk (0x3u << MATRIX_SCFG_DEFMSTR_TYPE_Pos) /**< \brief (MATRIX_SCFG[16]) Default Master Type */
#define MATRIX_SCFG_DEFMSTR_TYPE(value) ((MATRIX_SCFG_DEFMSTR_TYPE_Msk & ((value) << MATRIX_SCFG_DEFMSTR_TYPE_Pos)))
#define   MATRIX_SCFG_DEFMSTR_TYPE_NONE (0x0u << 16) /**< \brief (MATRIX_SCFG[16]) No Default Master-At the end of the current slave access, if no other master request is pending, the slave is disconnected from all masters. This results in a one clock cycle latency for the first access of a burst transfer or for a single access. */
#define   MATRIX_SCFG_DEFMSTR_TYPE_LAST (0x1u << 16) /**< \brief (MATRIX_SCFG[16]) Last Default Master-At the end of the current slave access, if no other master request is pending, the slave stays connected to the last master having accessed it. This results in not having one clock cycle latency when the last master tries to access the slave again. */
#define   MATRIX_SCFG_DEFMSTR_TYPE_FIXED (0x2u << 16) /**< \brief (MATRIX_SCFG[16]) Fixed Default Master-At the end of the current slave access, if no other master request is pending, the slave connects to the fixed master the number that has been written in the FIXED_DEFMSTR field. This results in not having one clock cycle latency when the fixed master tries to access the slave again. */
#define MATRIX_SCFG_FIXED_DEFMSTR_Pos 18
#define MATRIX_SCFG_FIXED_DEFMSTR_Msk (0xfu << MATRIX_SCFG_FIXED_DEFMSTR_Pos) /**< \brief (MATRIX_SCFG[16]) Fixed Default Master */
#define MATRIX_SCFG_FIXED_DEFMSTR(value) ((MATRIX_SCFG_FIXED_DEFMSTR_Msk & ((value) << MATRIX_SCFG_FIXED_DEFMSTR_Pos)))
/* -------- MATRIX_PRAS : (MATRIX Offset: N/A) Priority Register A for Slave 0 -------- */
#define MATRIX_PRAS_M0PR_Pos 0
#define MATRIX_PRAS_M0PR_Msk (0x3u << MATRIX_PRAS_M0PR_Pos) /**< \brief (MATRIX_PRAS) Master 0 Priority */
#define MATRIX_PRAS_M0PR(value) ((MATRIX_PRAS_M0PR_Msk & ((value) << MATRIX_PRAS_M0PR_Pos)))
#define MATRIX_PRAS_LQOSEN0 (0x1u << 2) /**< \brief (MATRIX_PRAS) Latency Quality of Service Enable for Master 0 */
#define MATRIX_PRAS_M1PR_Pos 4
#define MATRIX_PRAS_M1PR_Msk (0x3u << MATRIX_PRAS_M1PR_Pos) /**< \brief (MATRIX_PRAS) Master 1 Priority */
#define MATRIX_PRAS_M1PR(value) ((MATRIX_PRAS_M1PR_Msk & ((value) << MATRIX_PRAS_M1PR_Pos)))
#define MATRIX_PRAS_LQOSEN1 (0x1u << 6) /**< \brief (MATRIX_PRAS) Latency Quality of Service Enable for Master 1 */
#define MATRIX_PRAS_M2PR_Pos 8
#define MATRIX_PRAS_M2PR_Msk (0x3u << MATRIX_PRAS_M2PR_Pos) /**< \brief (MATRIX_PRAS) Master 2 Priority */
#define MATRIX_PRAS_M2PR(value) ((MATRIX_PRAS_M2PR_Msk & ((value) << MATRIX_PRAS_M2PR_Pos)))
#define MATRIX_PRAS_LQOSEN2 (0x1u << 10) /**< \brief (MATRIX_PRAS) Latency Quality of Service Enable for Master 2 */
#define MATRIX_PRAS_M3PR_Pos 12
#define MATRIX_PRAS_M3PR_Msk (0x3u << MATRIX_PRAS_M3PR_Pos) /**< \brief (MATRIX_PRAS) Master 3 Priority */
#define MATRIX_PRAS_M3PR(value) ((MATRIX_PRAS_M3PR_Msk & ((value) << MATRIX_PRAS_M3PR_Pos)))
#define MATRIX_PRAS_LQOSEN3 (0x1u << 14) /**< \brief (MATRIX_PRAS) Latency Quality of Service Enable for Master 3 */
#define MATRIX_PRAS_M4PR_Pos 16
#define MATRIX_PRAS_M4PR_Msk (0x3u << MATRIX_PRAS_M4PR_Pos) /**< \brief (MATRIX_PRAS) Master 4 Priority */
#define MATRIX_PRAS_M4PR(value) ((MATRIX_PRAS_M4PR_Msk & ((value) << MATRIX_PRAS_M4PR_Pos)))
#define MATRIX_PRAS_LQOSEN4 (0x1u << 18) /**< \brief (MATRIX_PRAS) Latency Quality of Service Enable for Master 4 */
#define MATRIX_PRAS_M5PR_Pos 20
#define MATRIX_PRAS_M5PR_Msk (0x3u << MATRIX_PRAS_M5PR_Pos) /**< \brief (MATRIX_PRAS) Master 5 Priority */
#define MATRIX_PRAS_M5PR(value) ((MATRIX_PRAS_M5PR_Msk & ((value) << MATRIX_PRAS_M5PR_Pos)))
#define MATRIX_PRAS_LQOSEN5 (0x1u << 22) /**< \brief (MATRIX_PRAS) Latency Quality of Service Enable for Master 5 */
#define MATRIX_PRAS_M6PR_Pos 24
#define MATRIX_PRAS_M6PR_Msk (0x3u << MATRIX_PRAS_M6PR_Pos) /**< \brief (MATRIX_PRAS) Master 6 Priority */
#define MATRIX_PRAS_M6PR(value) ((MATRIX_PRAS_M6PR_Msk & ((value) << MATRIX_PRAS_M6PR_Pos)))
#define MATRIX_PRAS_LQOSEN6 (0x1u << 26) /**< \brief (MATRIX_PRAS) Latency Quality of Service Enable for Master 6 */
#define MATRIX_PRAS_M7PR_Pos 28
#define MATRIX_PRAS_M7PR_Msk (0x3u << MATRIX_PRAS_M7PR_Pos) /**< \brief (MATRIX_PRAS) Master 7 Priority */
#define MATRIX_PRAS_M7PR(value) ((MATRIX_PRAS_M7PR_Msk & ((value) << MATRIX_PRAS_M7PR_Pos)))
#define MATRIX_PRAS_LQOSEN7 (0x1u << 30) /**< \brief (MATRIX_PRAS) Latency Quality of Service Enable for Master 7 */
/* -------- MATRIX_PRBS : (MATRIX Offset: N/A) Priority Register B for Slave 0 -------- */
#define MATRIX_PRBS_M8PR_Pos 0
#define MATRIX_PRBS_M8PR_Msk (0x3u << MATRIX_PRBS_M8PR_Pos) /**< \brief (MATRIX_PRBS) Master 8 Priority */
#define MATRIX_PRBS_M8PR(value) ((MATRIX_PRBS_M8PR_Msk & ((value) << MATRIX_PRBS_M8PR_Pos)))
#define MATRIX_PRBS_LQOSEN8 (0x1u << 2) /**< \brief (MATRIX_PRBS) Latency Quality of Service Enable for Master 8 */
#define MATRIX_PRBS_M9PR_Pos 4
#define MATRIX_PRBS_M9PR_Msk (0x3u << MATRIX_PRBS_M9PR_Pos) /**< \brief (MATRIX_PRBS) Master 9 Priority */
#define MATRIX_PRBS_M9PR(value) ((MATRIX_PRBS_M9PR_Msk & ((value) << MATRIX_PRBS_M9PR_Pos)))
#define MATRIX_PRBS_LQOSEN9 (0x1u << 6) /**< \brief (MATRIX_PRBS) Latency Quality of Service Enable for Master 9 */
#define MATRIX_PRBS_M10PR_Pos 8
#define MATRIX_PRBS_M10PR_Msk (0x3u << MATRIX_PRBS_M10PR_Pos) /**< \brief (MATRIX_PRBS) Master 10 Priority */
#define MATRIX_PRBS_M10PR(value) ((MATRIX_PRBS_M10PR_Msk & ((value) << MATRIX_PRBS_M10PR_Pos)))
#define MATRIX_PRBS_LQOSEN10 (0x1u << 10) /**< \brief (MATRIX_PRBS) Latency Quality of Service Enable for Master 10 */
#define MATRIX_PRBS_M11PR_Pos 12
#define MATRIX_PRBS_M11PR_Msk (0x3u << MATRIX_PRBS_M11PR_Pos) /**< \brief (MATRIX_PRBS) Master 11 Priority */
#define MATRIX_PRBS_M11PR(value) ((MATRIX_PRBS_M11PR_Msk & ((value) << MATRIX_PRBS_M11PR_Pos)))
#define MATRIX_PRBS_LQOSEN11 (0x1u << 14) /**< \brief (MATRIX_PRBS) Latency Quality of Service Enable for Master 11 */
#define MATRIX_PRBS_M12PR_Pos 16
#define MATRIX_PRBS_M12PR_Msk (0x3u << MATRIX_PRBS_M12PR_Pos) /**< \brief (MATRIX_PRBS) Master 12 Priority */
#define MATRIX_PRBS_M12PR(value) ((MATRIX_PRBS_M12PR_Msk & ((value) << MATRIX_PRBS_M12PR_Pos)))
#define MATRIX_PRBS_LQOSEN12 (0x1u << 18) /**< \brief (MATRIX_PRBS) Latency Quality of Service Enable for Master 12 */
#define MATRIX_PRBS_M13PR_Pos 20
#define MATRIX_PRBS_M13PR_Msk (0x3u << MATRIX_PRBS_M13PR_Pos) /**< \brief (MATRIX_PRBS) Master 13 Priority */
#define MATRIX_PRBS_M13PR(value) ((MATRIX_PRBS_M13PR_Msk & ((value) << MATRIX_PRBS_M13PR_Pos)))
#define MATRIX_PRBS_LQOSEN13 (0x1u << 22) /**< \brief (MATRIX_PRBS) Latency Quality of Service Enable for Master 13 */
#define MATRIX_PRBS_M14PR_Pos 24
#define MATRIX_PRBS_M14PR_Msk (0x3u << MATRIX_PRBS_M14PR_Pos) /**< \brief (MATRIX_PRBS) Master 14 Priority */
#define MATRIX_PRBS_M14PR(value) ((MATRIX_PRBS_M14PR_Msk & ((value) << MATRIX_PRBS_M14PR_Pos)))
#define MATRIX_PRBS_LQOSEN14 (0x1u << 26) /**< \brief (MATRIX_PRBS) Latency Quality of Service Enable for Master 14 */
#define MATRIX_PRBS_M15PR_Pos 28
#define MATRIX_PRBS_M15PR_Msk (0x3u << MATRIX_PRBS_M15PR_Pos) /**< \brief (MATRIX_PRBS) Master 15 Priority */
#define MATRIX_PRBS_M15PR(value) ((MATRIX_PRBS_M15PR_Msk & ((value) << MATRIX_PRBS_M15PR_Pos)))
#define MATRIX_PRBS_LQOSEN15 (0x1u << 30) /**< \brief (MATRIX_PRBS) Latency Quality of Service Enable for Master 15 */
/* -------- MATRIX_MRCR : (MATRIX Offset: 0x100) Master Remap Control Register -------- */
#define MATRIX_MRCR_RCB0 (0x1u << 0) /**< \brief (MATRIX_MRCR) Remap Command Bit for Master 0 */
#define MATRIX_MRCR_RCB1 (0x1u << 1) /**< \brief (MATRIX_MRCR) Remap Command Bit for Master 1 */
#define MATRIX_MRCR_RCB2 (0x1u << 2) /**< \brief (MATRIX_MRCR) Remap Command Bit for Master 2 */
#define MATRIX_MRCR_RCB3 (0x1u << 3) /**< \brief (MATRIX_MRCR) Remap Command Bit for Master 3 */
#define MATRIX_MRCR_RCB4 (0x1u << 4) /**< \brief (MATRIX_MRCR) Remap Command Bit for Master 4 */
#define MATRIX_MRCR_RCB5 (0x1u << 5) /**< \brief (MATRIX_MRCR) Remap Command Bit for Master 5 */
#define MATRIX_MRCR_RCB6 (0x1u << 6) /**< \brief (MATRIX_MRCR) Remap Command Bit for Master 6 */
#define MATRIX_MRCR_RCB7 (0x1u << 7) /**< \brief (MATRIX_MRCR) Remap Command Bit for Master 7 */
#define MATRIX_MRCR_RCB8 (0x1u << 8) /**< \brief (MATRIX_MRCR) Remap Command Bit for Master 8 */
#define MATRIX_MRCR_RCB9 (0x1u << 9) /**< \brief (MATRIX_MRCR) Remap Command Bit for Master 9 */
#define MATRIX_MRCR_RCB10 (0x1u << 10) /**< \brief (MATRIX_MRCR) Remap Command Bit for Master 10 */
#define MATRIX_MRCR_RCB11 (0x1u << 11) /**< \brief (MATRIX_MRCR) Remap Command Bit for Master 11 */
#define MATRIX_MRCR_RCB12 (0x1u << 12) /**< \brief (MATRIX_MRCR) Remap Command Bit for Master 12 */
#define MATRIX_MRCR_RCB13 (0x1u << 13) /**< \brief (MATRIX_MRCR) Remap Command Bit for Master 13 */
#define MATRIX_MRCR_RCB14 (0x1u << 14) /**< \brief (MATRIX_MRCR) Remap Command Bit for Master 14 */
#define MATRIX_MRCR_RCB15 (0x1u << 15) /**< \brief (MATRIX_MRCR) Remap Command Bit for Master 15 */
/* -------- MATRIX_MEIER : (MATRIX Offset: 0x150) Master Error Interrupt Enable Register -------- */
#define MATRIX_MEIER_MERR0 (0x1u << 0) /**< \brief (MATRIX_MEIER) Master 0 Access Error */
#define MATRIX_MEIER_MERR1 (0x1u << 1) /**< \brief (MATRIX_MEIER) Master 1 Access Error */
#define MATRIX_MEIER_MERR2 (0x1u << 2) /**< \brief (MATRIX_MEIER) Master 2 Access Error */
#define MATRIX_MEIER_MERR3 (0x1u << 3) /**< \brief (MATRIX_MEIER) Master 3 Access Error */
#define MATRIX_MEIER_MERR4 (0x1u << 4) /**< \brief (MATRIX_MEIER) Master 4 Access Error */
#define MATRIX_MEIER_MERR5 (0x1u << 5) /**< \brief (MATRIX_MEIER) Master 5 Access Error */
#define MATRIX_MEIER_MERR6 (0x1u << 6) /**< \brief (MATRIX_MEIER) Master 6 Access Error */
#define MATRIX_MEIER_MERR7 (0x1u << 7) /**< \brief (MATRIX_MEIER) Master 7 Access Error */
#define MATRIX_MEIER_MERR8 (0x1u << 8) /**< \brief (MATRIX_MEIER) Master 8 Access Error */
#define MATRIX_MEIER_MERR9 (0x1u << 9) /**< \brief (MATRIX_MEIER) Master 9 Access Error */
#define MATRIX_MEIER_MERR10 (0x1u << 10) /**< \brief (MATRIX_MEIER) Master 10 Access Error */
#define MATRIX_MEIER_MERR11 (0x1u << 11) /**< \brief (MATRIX_MEIER) Master 11 Access Error */
#define MATRIX_MEIER_MERR12 (0x1u << 12) /**< \brief (MATRIX_MEIER) Master 12 Access Error */
#define MATRIX_MEIER_MERR13 (0x1u << 13) /**< \brief (MATRIX_MEIER) Master 13 Access Error */
#define MATRIX_MEIER_MERR14 (0x1u << 14) /**< \brief (MATRIX_MEIER) Master 14 Access Error */
#define MATRIX_MEIER_MERR15 (0x1u << 15) /**< \brief (MATRIX_MEIER) Master 15 Access Error */
/* -------- MATRIX_MEIDR : (MATRIX Offset: 0x154) Master Error Interrupt Disable Register -------- */
#define MATRIX_MEIDR_MERR0 (0x1u << 0) /**< \brief (MATRIX_MEIDR) Master 0 Access Error */
#define MATRIX_MEIDR_MERR1 (0x1u << 1) /**< \brief (MATRIX_MEIDR) Master 1 Access Error */
#define MATRIX_MEIDR_MERR2 (0x1u << 2) /**< \brief (MATRIX_MEIDR) Master 2 Access Error */
#define MATRIX_MEIDR_MERR3 (0x1u << 3) /**< \brief (MATRIX_MEIDR) Master 3 Access Error */
#define MATRIX_MEIDR_MERR4 (0x1u << 4) /**< \brief (MATRIX_MEIDR) Master 4 Access Error */
#define MATRIX_MEIDR_MERR5 (0x1u << 5) /**< \brief (MATRIX_MEIDR) Master 5 Access Error */
#define MATRIX_MEIDR_MERR6 (0x1u << 6) /**< \brief (MATRIX_MEIDR) Master 6 Access Error */
#define MATRIX_MEIDR_MERR7 (0x1u << 7) /**< \brief (MATRIX_MEIDR) Master 7 Access Error */
#define MATRIX_MEIDR_MERR8 (0x1u << 8) /**< \brief (MATRIX_MEIDR) Master 8 Access Error */
#define MATRIX_MEIDR_MERR9 (0x1u << 9) /**< \brief (MATRIX_MEIDR) Master 9 Access Error */
#define MATRIX_MEIDR_MERR10 (0x1u << 10) /**< \brief (MATRIX_MEIDR) Master 10 Access Error */
#define MATRIX_MEIDR_MERR11 (0x1u << 11) /**< \brief (MATRIX_MEIDR) Master 11 Access Error */
#define MATRIX_MEIDR_MERR12 (0x1u << 12) /**< \brief (MATRIX_MEIDR) Master 12 Access Error */
#define MATRIX_MEIDR_MERR13 (0x1u << 13) /**< \brief (MATRIX_MEIDR) Master 13 Access Error */
#define MATRIX_MEIDR_MERR14 (0x1u << 14) /**< \brief (MATRIX_MEIDR) Master 14 Access Error */
#define MATRIX_MEIDR_MERR15 (0x1u << 15) /**< \brief (MATRIX_MEIDR) Master 15 Access Error */
/* -------- MATRIX_MEIMR : (MATRIX Offset: 0x158) Master Error Interrupt Mask Register -------- */
#define MATRIX_MEIMR_MERR0 (0x1u << 0) /**< \brief (MATRIX_MEIMR) Master 0 Access Error */
#define MATRIX_MEIMR_MERR1 (0x1u << 1) /**< \brief (MATRIX_MEIMR) Master 1 Access Error */
#define MATRIX_MEIMR_MERR2 (0x1u << 2) /**< \brief (MATRIX_MEIMR) Master 2 Access Error */
#define MATRIX_MEIMR_MERR3 (0x1u << 3) /**< \brief (MATRIX_MEIMR) Master 3 Access Error */
#define MATRIX_MEIMR_MERR4 (0x1u << 4) /**< \brief (MATRIX_MEIMR) Master 4 Access Error */
#define MATRIX_MEIMR_MERR5 (0x1u << 5) /**< \brief (MATRIX_MEIMR) Master 5 Access Error */
#define MATRIX_MEIMR_MERR6 (0x1u << 6) /**< \brief (MATRIX_MEIMR) Master 6 Access Error */
#define MATRIX_MEIMR_MERR7 (0x1u << 7) /**< \brief (MATRIX_MEIMR) Master 7 Access Error */
#define MATRIX_MEIMR_MERR8 (0x1u << 8) /**< \brief (MATRIX_MEIMR) Master 8 Access Error */
#define MATRIX_MEIMR_MERR9 (0x1u << 9) /**< \brief (MATRIX_MEIMR) Master 9 Access Error */
#define MATRIX_MEIMR_MERR10 (0x1u << 10) /**< \brief (MATRIX_MEIMR) Master 10 Access Error */
#define MATRIX_MEIMR_MERR11 (0x1u << 11) /**< \brief (MATRIX_MEIMR) Master 11 Access Error */
#define MATRIX_MEIMR_MERR12 (0x1u << 12) /**< \brief (MATRIX_MEIMR) Master 12 Access Error */
#define MATRIX_MEIMR_MERR13 (0x1u << 13) /**< \brief (MATRIX_MEIMR) Master 13 Access Error */
#define MATRIX_MEIMR_MERR14 (0x1u << 14) /**< \brief (MATRIX_MEIMR) Master 14 Access Error */
#define MATRIX_MEIMR_MERR15 (0x1u << 15) /**< \brief (MATRIX_MEIMR) Master 15 Access Error */
/* -------- MATRIX_MESR : (MATRIX Offset: 0x15C) Master Error Status Register -------- */
#define MATRIX_MESR_MERR0 (0x1u << 0) /**< \brief (MATRIX_MESR) Master 0 Access Error */
#define MATRIX_MESR_MERR1 (0x1u << 1) /**< \brief (MATRIX_MESR) Master 1 Access Error */
#define MATRIX_MESR_MERR2 (0x1u << 2) /**< \brief (MATRIX_MESR) Master 2 Access Error */
#define MATRIX_MESR_MERR3 (0x1u << 3) /**< \brief (MATRIX_MESR) Master 3 Access Error */
#define MATRIX_MESR_MERR4 (0x1u << 4) /**< \brief (MATRIX_MESR) Master 4 Access Error */
#define MATRIX_MESR_MERR5 (0x1u << 5) /**< \brief (MATRIX_MESR) Master 5 Access Error */
#define MATRIX_MESR_MERR6 (0x1u << 6) /**< \brief (MATRIX_MESR) Master 6 Access Error */
#define MATRIX_MESR_MERR7 (0x1u << 7) /**< \brief (MATRIX_MESR) Master 7 Access Error */
#define MATRIX_MESR_MERR8 (0x1u << 8) /**< \brief (MATRIX_MESR) Master 8 Access Error */
#define MATRIX_MESR_MERR9 (0x1u << 9) /**< \brief (MATRIX_MESR) Master 9 Access Error */
#define MATRIX_MESR_MERR10 (0x1u << 10) /**< \brief (MATRIX_MESR) Master 10 Access Error */
#define MATRIX_MESR_MERR11 (0x1u << 11) /**< \brief (MATRIX_MESR) Master 11 Access Error */
#define MATRIX_MESR_MERR12 (0x1u << 12) /**< \brief (MATRIX_MESR) Master 12 Access Error */
#define MATRIX_MESR_MERR13 (0x1u << 13) /**< \brief (MATRIX_MESR) Master 13 Access Error */
#define MATRIX_MESR_MERR14 (0x1u << 14) /**< \brief (MATRIX_MESR) Master 14 Access Error */
#define MATRIX_MESR_MERR15 (0x1u << 15) /**< \brief (MATRIX_MESR) Master 15 Access Error */
/* -------- MATRIX_MEAR[16] : (MATRIX Offset: 0x160) Master 0 Error Address Register -------- */
#define MATRIX_MEAR_ERRADD_Pos 0
#define MATRIX_MEAR_ERRADD_Msk (0xffffffffu << MATRIX_MEAR_ERRADD_Pos) /**< \brief (MATRIX_MEAR[16]) Master Error Address */
/* -------- MATRIX_WPMR : (MATRIX Offset: 0x1E4) Write Protect Mode Register -------- */
#define MATRIX_WPMR_WPEN (0x1u << 0) /**< \brief (MATRIX_WPMR) Write Protection Enable */
#define MATRIX_WPMR_CFGFRZ (0x1u << 7) /**< \brief (MATRIX_WPMR) Configuration Freeze */
#define MATRIX_WPMR_WPKEY_Pos 8
#define MATRIX_WPMR_WPKEY_Msk (0xffffffu << MATRIX_WPMR_WPKEY_Pos) /**< \brief (MATRIX_WPMR) Write Protection Key */
#define MATRIX_WPMR_WPKEY(value) ((MATRIX_WPMR_WPKEY_Msk & ((value) << MATRIX_WPMR_WPKEY_Pos)))
#define   MATRIX_WPMR_WPKEY_PASSWD (0x4D4154u << 8) /**< \brief (MATRIX_WPMR) Writing any other value in this field aborts the write operation of the WPEN and CFGFRZ bits. Always reads as 0. */
/* -------- MATRIX_WPSR : (MATRIX Offset: 0x1E8) Write Protect Status Register -------- */
#define MATRIX_WPSR_WPVS (0x1u << 0) /**< \brief (MATRIX_WPSR) Write Protection Violation Status */
#define MATRIX_WPSR_WPVSRC_Pos 8
#define MATRIX_WPSR_WPVSRC_Msk (0xffffu << MATRIX_WPSR_WPVSRC_Pos) /**< \brief (MATRIX_WPSR) Write Protection Violation Source */
/* -------- MATRIX_PSR[16] : (MATRIX Offset: 0x200) Protection Slave 0 Register -------- */
#define MATRIX_PSR_LAUSERH0 (0x1u << 0) /**< \brief (MATRIX_PSR[16]) Low Area USER in HSELx Protected Region */
#define MATRIX_PSR_LAUSERH1 (0x1u << 1) /**< \brief (MATRIX_PSR[16]) Low Area USER in HSELx Protected Region */
#define MATRIX_PSR_LAUSERH2 (0x1u << 2) /**< \brief (MATRIX_PSR[16]) Low Area USER in HSELx Protected Region */
#define MATRIX_PSR_LAUSERH3 (0x1u << 3) /**< \brief (MATRIX_PSR[16]) Low Area USER in HSELx Protected Region */
#define MATRIX_PSR_LAUSERH4 (0x1u << 4) /**< \brief (MATRIX_PSR[16]) Low Area USER in HSELx Protected Region */
#define MATRIX_PSR_LAUSERH5 (0x1u << 5) /**< \brief (MATRIX_PSR[16]) Low Area USER in HSELx Protected Region */
#define MATRIX_PSR_LAUSERH6 (0x1u << 6) /**< \brief (MATRIX_PSR[16]) Low Area USER in HSELx Protected Region */
#define MATRIX_PSR_LAUSERH7 (0x1u << 7) /**< \brief (MATRIX_PSR[16]) Low Area USER in HSELx Protected Region */
#define MATRIX_PSR_RDUSERH0 (0x1u << 8) /**< \brief (MATRIX_PSR[16]) Read USER for HSELx Protected Region */
#define MATRIX_PSR_RDUSERH1 (0x1u << 9) /**< \brief (MATRIX_PSR[16]) Read USER for HSELx Protected Region */
#define MATRIX_PSR_RDUSERH2 (0x1u << 10) /**< \brief (MATRIX_PSR[16]) Read USER for HSELx Protected Region */
#define MATRIX_PSR_RDUSERH3 (0x1u << 11) /**< \brief (MATRIX_PSR[16]) Read USER for HSELx Protected Region */
#define MATRIX_PSR_RDUSERH4 (0x1u << 12) /**< \brief (MATRIX_PSR[16]) Read USER for HSELx Protected Region */
#define MATRIX_PSR_RDUSERH5 (0x1u << 13) /**< \brief (MATRIX_PSR[16]) Read USER for HSELx Protected Region */
#define MATRIX_PSR_RDUSERH6 (0x1u << 14) /**< \brief (MATRIX_PSR[16]) Read USER for HSELx Protected Region */
#define MATRIX_PSR_RDUSERH7 (0x1u << 15) /**< \brief (MATRIX_PSR[16]) Read USER for HSELx Protected Region */
#define MATRIX_PSR_WRUSERH0 (0x1u << 16) /**< \brief (MATRIX_PSR[16]) Write USER for HSELx Protected Region */
#define MATRIX_PSR_WRUSERH1 (0x1u << 17) /**< \brief (MATRIX_PSR[16]) Write USER for HSELx Protected Region */
#define MATRIX_PSR_WRUSERH2 (0x1u << 18) /**< \brief (MATRIX_PSR[16]) Write USER for HSELx Protected Region */
#define MATRIX_PSR_WRUSERH3 (0x1u << 19) /**< \brief (MATRIX_PSR[16]) Write USER for HSELx Protected Region */
#define MATRIX_PSR_WRUSERH4 (0x1u << 20) /**< \brief (MATRIX_PSR[16]) Write USER for HSELx Protected Region */
#define MATRIX_PSR_WRUSERH5 (0x1u << 21) /**< \brief (MATRIX_PSR[16]) Write USER for HSELx Protected Region */
#define MATRIX_PSR_WRUSERH6 (0x1u << 22) /**< \brief (MATRIX_PSR[16]) Write USER for HSELx Protected Region */
#define MATRIX_PSR_WRUSERH7 (0x1u << 23) /**< \brief (MATRIX_PSR[16]) Write USER for HSELx Protected Region */
#define MATRIX_PSR_DPSOA0 (0x1u << 24) /**< \brief (MATRIX_PSR[16]) Downward Protection Split Address for HSELx Protected Region */
#define MATRIX_PSR_DPSOA1 (0x1u << 25) /**< \brief (MATRIX_PSR[16]) Downward Protection Split Address for HSELx Protected Region */
#define MATRIX_PSR_DPSOA2 (0x1u << 26) /**< \brief (MATRIX_PSR[16]) Downward Protection Split Address for HSELx Protected Region */
#define MATRIX_PSR_DPSOA3 (0x1u << 27) /**< \brief (MATRIX_PSR[16]) Downward Protection Split Address for HSELx Protected Region */
#define MATRIX_PSR_DPSOA4 (0x1u << 28) /**< \brief (MATRIX_PSR[16]) Downward Protection Split Address for HSELx Protected Region */
#define MATRIX_PSR_DPSOA5 (0x1u << 29) /**< \brief (MATRIX_PSR[16]) Downward Protection Split Address for HSELx Protected Region */
#define MATRIX_PSR_DPSOA6 (0x1u << 30) /**< \brief (MATRIX_PSR[16]) Downward Protection Split Address for HSELx Protected Region */
#define MATRIX_PSR_DPSOA7 (0x1u << 31) /**< \brief (MATRIX_PSR[16]) Downward Protection Split Address for HSELx Protected Region */
/* -------- MATRIX_PASSR[16] : (MATRIX Offset: 0x240) Protected Areas Split Slave 0 Register -------- */
#define MATRIX_PASSR_PASPLIT0_Pos 0
#define MATRIX_PASSR_PASPLIT0_Msk (0xfu << MATRIX_PASSR_PASPLIT0_Pos) /**< \brief (MATRIX_PASSR[16]) Protected Areas Split for HSELx Protected Region */
#define MATRIX_PASSR_PASPLIT0(value) ((MATRIX_PASSR_PASPLIT0_Msk & ((value) << MATRIX_PASSR_PASPLIT0_Pos)))
#define MATRIX_PASSR_PASPLIT1_Pos 4
#define MATRIX_PASSR_PASPLIT1_Msk (0xfu << MATRIX_PASSR_PASPLIT1_Pos) /**< \brief (MATRIX_PASSR[16]) Protected Areas Split for HSELx Protected Region */
#define MATRIX_PASSR_PASPLIT1(value) ((MATRIX_PASSR_PASPLIT1_Msk & ((value) << MATRIX_PASSR_PASPLIT1_Pos)))
#define MATRIX_PASSR_PASPLIT2_Pos 8
#define MATRIX_PASSR_PASPLIT2_Msk (0xfu << MATRIX_PASSR_PASPLIT2_Pos) /**< \brief (MATRIX_PASSR[16]) Protected Areas Split for HSELx Protected Region */
#define MATRIX_PASSR_PASPLIT2(value) ((MATRIX_PASSR_PASPLIT2_Msk & ((value) << MATRIX_PASSR_PASPLIT2_Pos)))
#define MATRIX_PASSR_PASPLIT3_Pos 12
#define MATRIX_PASSR_PASPLIT3_Msk (0xfu << MATRIX_PASSR_PASPLIT3_Pos) /**< \brief (MATRIX_PASSR[16]) Protected Areas Split for HSELx Protected Region */
#define MATRIX_PASSR_PASPLIT3(value) ((MATRIX_PASSR_PASPLIT3_Msk & ((value) << MATRIX_PASSR_PASPLIT3_Pos)))
#define MATRIX_PASSR_PASPLIT4_Pos 16
#define MATRIX_PASSR_PASPLIT4_Msk (0xfu << MATRIX_PASSR_PASPLIT4_Pos) /**< \brief (MATRIX_PASSR[16]) Protected Areas Split for HSELx Protected Region */
#define MATRIX_PASSR_PASPLIT4(value) ((MATRIX_PASSR_PASPLIT4_Msk & ((value) << MATRIX_PASSR_PASPLIT4_Pos)))
#define MATRIX_PASSR_PASPLIT5_Pos 20
#define MATRIX_PASSR_PASPLIT5_Msk (0xfu << MATRIX_PASSR_PASPLIT5_Pos) /**< \brief (MATRIX_PASSR[16]) Protected Areas Split for HSELx Protected Region */
#define MATRIX_PASSR_PASPLIT5(value) ((MATRIX_PASSR_PASPLIT5_Msk & ((value) << MATRIX_PASSR_PASPLIT5_Pos)))
#define MATRIX_PASSR_PASPLIT6_Pos 24
#define MATRIX_PASSR_PASPLIT6_Msk (0xfu << MATRIX_PASSR_PASPLIT6_Pos) /**< \brief (MATRIX_PASSR[16]) Protected Areas Split for HSELx Protected Region */
#define MATRIX_PASSR_PASPLIT6(value) ((MATRIX_PASSR_PASPLIT6_Msk & ((value) << MATRIX_PASSR_PASPLIT6_Pos)))
#define MATRIX_PASSR_PASPLIT7_Pos 28
#define MATRIX_PASSR_PASPLIT7_Msk (0xfu << MATRIX_PASSR_PASPLIT7_Pos) /**< \brief (MATRIX_PASSR[16]) Protected Areas Split for HSELx Protected Region */
#define MATRIX_PASSR_PASPLIT7(value) ((MATRIX_PASSR_PASPLIT7_Msk & ((value) << MATRIX_PASSR_PASPLIT7_Pos)))
/* -------- MATRIX_PRTSR[16] : (MATRIX Offset: 0x280) Protected Region Top Slave 0 Register -------- */
#define MATRIX_PRTSR_PRTOP0_Pos 0
#define MATRIX_PRTSR_PRTOP0_Msk (0xfu << MATRIX_PRTSR_PRTOP0_Pos) /**< \brief (MATRIX_PRTSR[16]) HSELx Protected Region Top */
#define MATRIX_PRTSR_PRTOP0(value) ((MATRIX_PRTSR_PRTOP0_Msk & ((value) << MATRIX_PRTSR_PRTOP0_Pos)))
#define MATRIX_PRTSR_PRTOP1_Pos 4
#define MATRIX_PRTSR_PRTOP1_Msk (0xfu << MATRIX_PRTSR_PRTOP1_Pos) /**< \brief (MATRIX_PRTSR[16]) HSELx Protected Region Top */
#define MATRIX_PRTSR_PRTOP1(value) ((MATRIX_PRTSR_PRTOP1_Msk & ((value) << MATRIX_PRTSR_PRTOP1_Pos)))
#define MATRIX_PRTSR_PRTOP2_Pos 8
#define MATRIX_PRTSR_PRTOP2_Msk (0xfu << MATRIX_PRTSR_PRTOP2_Pos) /**< \brief (MATRIX_PRTSR[16]) HSELx Protected Region Top */
#define MATRIX_PRTSR_PRTOP2(value) ((MATRIX_PRTSR_PRTOP2_Msk & ((value) << MATRIX_PRTSR_PRTOP2_Pos)))
#define MATRIX_PRTSR_PRTOP3_Pos 12
#define MATRIX_PRTSR_PRTOP3_Msk (0xfu << MATRIX_PRTSR_PRTOP3_Pos) /**< \brief (MATRIX_PRTSR[16]) HSELx Protected Region Top */
#define MATRIX_PRTSR_PRTOP3(value) ((MATRIX_PRTSR_PRTOP3_Msk & ((value) << MATRIX_PRTSR_PRTOP3_Pos)))
#define MATRIX_PRTSR_PRTOP4_Pos 16
#define MATRIX_PRTSR_PRTOP4_Msk (0xfu << MATRIX_PRTSR_PRTOP4_Pos) /**< \brief (MATRIX_PRTSR[16]) HSELx Protected Region Top */
#define MATRIX_PRTSR_PRTOP4(value) ((MATRIX_PRTSR_PRTOP4_Msk & ((value) << MATRIX_PRTSR_PRTOP4_Pos)))
#define MATRIX_PRTSR_PRTOP5_Pos 20
#define MATRIX_PRTSR_PRTOP5_Msk (0xfu << MATRIX_PRTSR_PRTOP5_Pos) /**< \brief (MATRIX_PRTSR[16]) HSELx Protected Region Top */
#define MATRIX_PRTSR_PRTOP5(value) ((MATRIX_PRTSR_PRTOP5_Msk & ((value) << MATRIX_PRTSR_PRTOP5_Pos)))
#define MATRIX_PRTSR_PRTOP6_Pos 24
#define MATRIX_PRTSR_PRTOP6_Msk (0xfu << MATRIX_PRTSR_PRTOP6_Pos) /**< \brief (MATRIX_PRTSR[16]) HSELx Protected Region Top */
#define MATRIX_PRTSR_PRTOP6(value) ((MATRIX_PRTSR_PRTOP6_Msk & ((value) << MATRIX_PRTSR_PRTOP6_Pos)))
#define MATRIX_PRTSR_PRTOP7_Pos 28
#define MATRIX_PRTSR_PRTOP7_Msk (0xfu << MATRIX_PRTSR_PRTOP7_Pos) /**< \brief (MATRIX_PRTSR[16]) HSELx Protected Region Top */
#define MATRIX_PRTSR_PRTOP7(value) ((MATRIX_PRTSR_PRTOP7_Msk & ((value) << MATRIX_PRTSR_PRTOP7_Pos)))
/* -------- MATRIX_PPSELR[3] : (MATRIX Offset: 0x2C0) Protected Peripheral Select 1 Register -------- */
#define MATRIX_PPSELR_USERP0 (0x1u << 0) /**< \brief (MATRIX_PPSELR[3]) User PSELy Peripheral */
#define MATRIX_PPSELR_USERP1 (0x1u << 1) /**< \brief (MATRIX_PPSELR[3]) User PSELy Peripheral */
#define MATRIX_PPSELR_USERP2 (0x1u << 2) /**< \brief (MATRIX_PPSELR[3]) User PSELy Peripheral */
#define MATRIX_PPSELR_USERP3 (0x1u << 3) /**< \brief (MATRIX_PPSELR[3]) User PSELy Peripheral */
#define MATRIX_PPSELR_USERP4 (0x1u << 4) /**< \brief (MATRIX_PPSELR[3]) User PSELy Peripheral */
#define MATRIX_PPSELR_USERP5 (0x1u << 5) /**< \brief (MATRIX_PPSELR[3]) User PSELy Peripheral */
#define MATRIX_PPSELR_USERP6 (0x1u << 6) /**< \brief (MATRIX_PPSELR[3]) User PSELy Peripheral */
#define MATRIX_PPSELR_USERP7 (0x1u << 7) /**< \brief (MATRIX_PPSELR[3]) User PSELy Peripheral */
#define MATRIX_PPSELR_USERP8 (0x1u << 8) /**< \brief (MATRIX_PPSELR[3]) User PSELy Peripheral */
#define MATRIX_PPSELR_USERP9 (0x1u << 9) /**< \brief (MATRIX_PPSELR[3]) User PSELy Peripheral */
#define MATRIX_PPSELR_USERP10 (0x1u << 10) /**< \brief (MATRIX_PPSELR[3]) User PSELy Peripheral */
#define MATRIX_PPSELR_USERP11 (0x1u << 11) /**< \brief (MATRIX_PPSELR[3]) User PSELy Peripheral */
#define MATRIX_PPSELR_USERP12 (0x1u << 12) /**< \brief (MATRIX_PPSELR[3]) User PSELy Peripheral */
#define MATRIX_PPSELR_USERP13 (0x1u << 13) /**< \brief (MATRIX_PPSELR[3]) User PSELy Peripheral */
#define MATRIX_PPSELR_USERP14 (0x1u << 14) /**< \brief (MATRIX_PPSELR[3]) User PSELy Peripheral */
#define MATRIX_PPSELR_USERP15 (0x1u << 15) /**< \brief (MATRIX_PPSELR[3]) User PSELy Peripheral */
#define MATRIX_PPSELR_USERP16 (0x1u << 16) /**< \brief (MATRIX_PPSELR[3]) User PSELy Peripheral */
#define MATRIX_PPSELR_USERP17 (0x1u << 17) /**< \brief (MATRIX_PPSELR[3]) User PSELy Peripheral */
#define MATRIX_PPSELR_USERP18 (0x1u << 18) /**< \brief (MATRIX_PPSELR[3]) User PSELy Peripheral */
#define MATRIX_PPSELR_USERP19 (0x1u << 19) /**< \brief (MATRIX_PPSELR[3]) User PSELy Peripheral */
#define MATRIX_PPSELR_USERP20 (0x1u << 20) /**< \brief (MATRIX_PPSELR[3]) User PSELy Peripheral */
#define MATRIX_PPSELR_USERP21 (0x1u << 21) /**< \brief (MATRIX_PPSELR[3]) User PSELy Peripheral */
#define MATRIX_PPSELR_USERP22 (0x1u << 22) /**< \brief (MATRIX_PPSELR[3]) User PSELy Peripheral */
#define MATRIX_PPSELR_USERP23 (0x1u << 23) /**< \brief (MATRIX_PPSELR[3]) User PSELy Peripheral */
#define MATRIX_PPSELR_USERP24 (0x1u << 24) /**< \brief (MATRIX_PPSELR[3]) User PSELy Peripheral */
#define MATRIX_PPSELR_USERP25 (0x1u << 25) /**< \brief (MATRIX_PPSELR[3]) User PSELy Peripheral */
#define MATRIX_PPSELR_USERP26 (0x1u << 26) /**< \brief (MATRIX_PPSELR[3]) User PSELy Peripheral */
#define MATRIX_PPSELR_USERP27 (0x1u << 27) /**< \brief (MATRIX_PPSELR[3]) User PSELy Peripheral */
#define MATRIX_PPSELR_USERP28 (0x1u << 28) /**< \brief (MATRIX_PPSELR[3]) User PSELy Peripheral */
#define MATRIX_PPSELR_USERP29 (0x1u << 29) /**< \brief (MATRIX_PPSELR[3]) User PSELy Peripheral */
#define MATRIX_PPSELR_USERP30 (0x1u << 30) /**< \brief (MATRIX_PPSELR[3]) User PSELy Peripheral */
#define MATRIX_PPSELR_USERP31 (0x1u << 31) /**< \brief (MATRIX_PPSELR[3]) User PSELy Peripheral */

/*@}*/


#endif /* _PIC32CX_MATRIX_COMPONENT_ */
