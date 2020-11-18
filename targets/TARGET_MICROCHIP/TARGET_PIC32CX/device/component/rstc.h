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

#ifndef _PIC32CX_RSTC_COMPONENT_
#define _PIC32CX_RSTC_COMPONENT_

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR Reset Controller */
/* ============================================================================= */
/** \addtogroup PIC32CX_RSTC Reset Controller */
/*@{*/

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
/** \brief Rstc hardware registers */
typedef struct {
  __O  uint32_t RSTC_CR; /**< \brief (Rstc Offset: 0x0) Control Register */
  __I  uint32_t RSTC_SR; /**< \brief (Rstc Offset: 0x4) Status Register */
  __IO uint32_t RSTC_MR; /**< \brief (Rstc Offset: 0x8) Mode Register */
} Rstc;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/* -------- RSTC_CR : (RSTC Offset: 0x0) Control Register -------- */
#define RSTC_CR_PROCRST (0x1u << 0) /**< \brief (RSTC_CR) Processor Reset */
#define RSTC_CR_PERRST (0x1u << 2) /**< \brief (RSTC_CR) Peripheral Reset */
#define RSTC_CR_EXTRST (0x1u << 3) /**< \brief (RSTC_CR) External Reset */
#define RSTC_CR_KEY_Pos 24
#define RSTC_CR_KEY_Msk (0xffu << RSTC_CR_KEY_Pos) /**< \brief (RSTC_CR) System Reset Key */
#define RSTC_CR_KEY(value) ((RSTC_CR_KEY_Msk & ((value) << RSTC_CR_KEY_Pos)))
#define   RSTC_CR_KEY_PASSWD (0xA5u << 24) /**< \brief (RSTC_CR) Writing any other value in this field aborts the write operation. */
/* -------- RSTC_SR : (RSTC Offset: 0x4) Status Register -------- */
#define RSTC_SR_URSTS (0x1u << 0) /**< \brief (RSTC_SR) User Reset Status (cleared on read) */
#define RSTC_SR_CORESMS (0x1u << 1) /**< \brief (RSTC_SR) VDDCORE Supply Monitor Reset Flag Status (cleared on read) */
#define RSTC_SR_RSTTYP_Pos 8
#define RSTC_SR_RSTTYP_Msk (0xfu << RSTC_SR_RSTTYP_Pos) /**< \brief (RSTC_SR) Reset Type */
#define   RSTC_SR_RSTTYP_GENERAL_RST (0x0u << 8) /**< \brief (RSTC_SR) First power-up reset */
#define   RSTC_SR_RSTTYP_BACKUP_RST (0x1u << 8) /**< \brief (RSTC_SR) VDDCORE reset. Wake-up from Backup mode */
#define   RSTC_SR_RSTTYP_WDT0_RST (0x2u << 8) /**< \brief (RSTC_SR) Watchdog 0 fault occurred */
#define   RSTC_SR_RSTTYP_SOFT_RST (0x3u << 8) /**< \brief (RSTC_SR) Processor reset required by the software */
#define   RSTC_SR_RSTTYP_USER_RST (0x4u << 8) /**< \brief (RSTC_SR) NRST pin detected low */
#define   RSTC_SR_RSTTYP_CORE_SM_RST (0x5u << 8) /**< \brief (RSTC_SR) Core Supply Monitor Reset */
#define   RSTC_SR_RSTTYP_CPU_FAIL_RST (0x6u << 8) /**< \brief (RSTC_SR) CPU clock failure detection occurred */
#define   RSTC_SR_RSTTYP_SLCK_XTAL_RST (0x7u << 8) /**< \brief (RSTC_SR) 32.768 kHz crystal failure detection fault occurred */
#define   RSTC_SR_RSTTYP_WDT1_RST (0x9u << 8) /**< \brief (RSTC_SR) Watchdog 1 fault occurred */
#define   RSTC_SR_RSTTYP_PORIO_RST (0xAu << 8) /**< \brief (RSTC_SR) VDDIO Supply Monitor (PORIO) Reset occurred */
#define RSTC_SR_NRSTL (0x1u << 16) /**< \brief (RSTC_SR) NRST Pin Level */
#define RSTC_SR_SRCMP (0x1u << 17) /**< \brief (RSTC_SR) Software Reset Command in Progress */
/* -------- RSTC_MR : (RSTC Offset: 0x8) Mode Register -------- */
#define RSTC_MR_URSTEN (0x1u << 0) /**< \brief (RSTC_MR) User Reset Enable */
#define RSTC_MR_SCKSW (0x1u << 1) /**< \brief (RSTC_MR) Slow Clock Switching */
#define RSTC_MR_URSTASYNC (0x1u << 2) /**< \brief (RSTC_MR) User Reset Asynchronous Control */
#define RSTC_MR_CPUFEN (0x1u << 3) /**< \brief (RSTC_MR) CPU Fail Enable */
#define RSTC_MR_URSTIEN (0x1u << 4) /**< \brief (RSTC_MR) User Reset Interrupt Enable */
#define RSTC_MR_SFTPMCRS (0x1u << 5) /**< \brief (RSTC_MR) Software PMC Reset */
#define RSTC_MR_WDTPMC0 (0x1u << 6) /**< \brief (RSTC_MR) WDT0 PMC Reset */
#define RSTC_MR_WDTPMC1 (0x1u << 7) /**< \brief (RSTC_MR) WDT1 PMC Reset */
#define RSTC_MR_ERSTL_Pos 8
#define RSTC_MR_ERSTL_Msk (0xfu << RSTC_MR_ERSTL_Pos) /**< \brief (RSTC_MR) External Reset Length */
#define RSTC_MR_ERSTL(value) ((RSTC_MR_ERSTL_Msk & ((value) << RSTC_MR_ERSTL_Pos)))
#define RSTC_MR_CORSMIEN (0x1u << 16) /**< \brief (RSTC_MR) VDDCORE Supply Monitor Interrupt Enable */
#define RSTC_MR_CPEREN (0x1u << 17) /**< \brief (RSTC_MR) Coprocessor Peripheral Enable */
#define RSTC_MR_CPROCEN (0x1u << 18) /**< \brief (RSTC_MR) Coprocessor (Second Processor) Enable */
#define RSTC_MR_PWRSW (0x1u << 21) /**< \brief (RSTC_MR) Backup Area Power Switch Control */
#define RSTC_MR_BADXTRST (0x1u << 22) /**< \brief (RSTC_MR) Bad XTAL Fail Reset */
#define RSTC_MR_KEY_Pos 24
#define RSTC_MR_KEY_Msk (0xffu << RSTC_MR_KEY_Pos) /**< \brief (RSTC_MR) Write Access Password */
#define RSTC_MR_KEY(value) ((RSTC_MR_KEY_Msk & ((value) << RSTC_MR_KEY_Pos)))
#define   RSTC_MR_KEY_PASSWD (0xA5u << 24) /**< \brief (RSTC_MR) Writing any other value in this field aborts the write operation. Always reads as 0. */

/*@}*/


#endif /* _PIC32CX_RSTC_COMPONENT_ */
