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

#ifndef _PIC32CX_SYSCWP_COMPONENT_
#define _PIC32CX_SYSCWP_COMPONENT_

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR External Interrupt Controller */
/* ============================================================================= */
/** \addtogroup PIC32CX_SYSCWP External Interrupt Controller */
/*@{*/

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
/** \brief Syscwp hardware registers */
typedef struct {
  __IO uint32_t SYSC_WPMR; /**< \brief (Syscwp Offset: 0x0) Write Protection Mode Register */
  __I  uint32_t SYSC_WPSR; /**< \brief (Syscwp Offset: 0x4) Write Protection Status Register */
} Syscwp;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/* -------- SYSC_WPMR : (SYSCWP Offset: 0x0) Write Protection Mode Register -------- */
#define SYSC_WPMR_WPEN (0x1u << 0) /**< \brief (SYSC_WPMR) Write Protection Enable */
#define SYSC_WPMR_WPITEN (0x1u << 1) /**< \brief (SYSC_WPMR) Write Protection Interrupt Enable */
#define SYSC_WPMR_WPKEY_Pos 8
#define SYSC_WPMR_WPKEY_Msk (0xffffffu << SYSC_WPMR_WPKEY_Pos) /**< \brief (SYSC_WPMR) Write Protection Key */
#define SYSC_WPMR_WPKEY(value) ((SYSC_WPMR_WPKEY_Msk & ((value) << SYSC_WPMR_WPKEY_Pos)))
#define   SYSC_WPMR_WPKEY_PASSWD (0x535943u << 8) /**< \brief (SYSC_WPMR) Writing any other value in this field aborts the write operation of the WPEN and WPITEN bits. Always reads as 0. */
/* -------- SYSC_WPSR : (SYSCWP Offset: 0x4) Write Protection Status Register -------- */
#define SYSC_WPSR_WPVS (0x1u << 0) /**< \brief (SYSC_WPSR) Write Protection Register Violation Status */
#define SYSC_WPSR_WVSRC_Pos 8
#define SYSC_WPSR_WVSRC_Msk (0xffu << SYSC_WPSR_WVSRC_Pos) /**< \brief (SYSC_WPSR) Write Violation Source */

/*@}*/


#endif /* _PIC32CX_SYSCWP_COMPONENT_ */
