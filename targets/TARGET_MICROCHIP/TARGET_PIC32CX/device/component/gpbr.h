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

#ifndef _PIC32CX_GPBR_COMPONENT_
#define _PIC32CX_GPBR_COMPONENT_

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR General Purpose Backup Registers */
/* ============================================================================= */
/** \addtogroup PIC32CX_GPBR General Purpose Backup Registers */
/*@{*/

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
/** \brief Gpbr hardware registers */
typedef struct {
  __IO uint32_t GPBR_MR;      /**< \brief (Gpbr Offset: 0x0) GPBR Mode Register */
  __I  uint32_t Reserved1[1];
  __IO uint32_t SYS_GPBR[24]; /**< \brief (Gpbr Offset: 0x8) General Purpose Backup Register */
} Gpbr;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/* -------- GPBR_MR : (GPBR Offset: 0x0) GPBR Mode Register -------- */
#define GPBR_MR_GPBRWP0 (0x1u << 0) /**< \brief (GPBR_MR) GPBRx Write Protection */
#define GPBR_MR_GPBRWP1 (0x1u << 1) /**< \brief (GPBR_MR) GPBRx Write Protection */
#define GPBR_MR_GPBRWP2 (0x1u << 2) /**< \brief (GPBR_MR) GPBRx Write Protection */
#define GPBR_MR_GPBRWP3 (0x1u << 3) /**< \brief (GPBR_MR) GPBRx Write Protection */
#define GPBR_MR_GPBRWP4 (0x1u << 4) /**< \brief (GPBR_MR) GPBRx Write Protection */
#define GPBR_MR_GPBRWP5 (0x1u << 5) /**< \brief (GPBR_MR) GPBRx Write Protection */
#define GPBR_MR_GPBRWP6 (0x1u << 6) /**< \brief (GPBR_MR) GPBRx Write Protection */
#define GPBR_MR_GPBRWP7 (0x1u << 7) /**< \brief (GPBR_MR) GPBRx Write Protection */
#define GPBR_MR_GPBRWP8 (0x1u << 8) /**< \brief (GPBR_MR) GPBRx Write Protection */
#define GPBR_MR_GPBRWP9 (0x1u << 9) /**< \brief (GPBR_MR) GPBRx Write Protection */
#define GPBR_MR_GPBRWP10 (0x1u << 10) /**< \brief (GPBR_MR) GPBRx Write Protection */
#define GPBR_MR_GPBRWP11 (0x1u << 11) /**< \brief (GPBR_MR) GPBRx Write Protection */
#define GPBR_MR_GPBRWP12 (0x1u << 12) /**< \brief (GPBR_MR) GPBRx Write Protection */
#define GPBR_MR_GPBRWP13 (0x1u << 13) /**< \brief (GPBR_MR) GPBRx Write Protection */
#define GPBR_MR_GPBRWP14 (0x1u << 14) /**< \brief (GPBR_MR) GPBRx Write Protection */
#define GPBR_MR_GPBRWP15 (0x1u << 15) /**< \brief (GPBR_MR) GPBRx Write Protection */
#define GPBR_MR_GPBRRP0 (0x1u << 16) /**< \brief (GPBR_MR) GPBRx Read Protection */
#define GPBR_MR_GPBRRP1 (0x1u << 17) /**< \brief (GPBR_MR) GPBRx Read Protection */
#define GPBR_MR_GPBRRP2 (0x1u << 18) /**< \brief (GPBR_MR) GPBRx Read Protection */
#define GPBR_MR_GPBRRP3 (0x1u << 19) /**< \brief (GPBR_MR) GPBRx Read Protection */
#define GPBR_MR_GPBRRP4 (0x1u << 20) /**< \brief (GPBR_MR) GPBRx Read Protection */
#define GPBR_MR_GPBRRP5 (0x1u << 21) /**< \brief (GPBR_MR) GPBRx Read Protection */
#define GPBR_MR_GPBRRP6 (0x1u << 22) /**< \brief (GPBR_MR) GPBRx Read Protection */
#define GPBR_MR_GPBRRP7 (0x1u << 23) /**< \brief (GPBR_MR) GPBRx Read Protection */
#define GPBR_MR_GPBRRP8 (0x1u << 24) /**< \brief (GPBR_MR) GPBRx Read Protection */
#define GPBR_MR_GPBRRP9 (0x1u << 25) /**< \brief (GPBR_MR) GPBRx Read Protection */
#define GPBR_MR_GPBRRP10 (0x1u << 26) /**< \brief (GPBR_MR) GPBRx Read Protection */
#define GPBR_MR_GPBRRP11 (0x1u << 27) /**< \brief (GPBR_MR) GPBRx Read Protection */
#define GPBR_MR_GPBRRP12 (0x1u << 28) /**< \brief (GPBR_MR) GPBRx Read Protection */
#define GPBR_MR_GPBRRP13 (0x1u << 29) /**< \brief (GPBR_MR) GPBRx Read Protection */
#define GPBR_MR_GPBRRP14 (0x1u << 30) /**< \brief (GPBR_MR) GPBRx Read Protection */
#define GPBR_MR_GPBRRP15 (0x1u << 31) /**< \brief (GPBR_MR) GPBRx Read Protection */
/* -------- SYS_GPBR[24] : (GPBR Offset: 0x8) General Purpose Backup Register -------- */
#define SYS_GPBR_GPBR_VALUE_Pos 0
#define SYS_GPBR_GPBR_VALUE_Msk (0xffffffffu << SYS_GPBR_GPBR_VALUE_Pos) /**< \brief (SYS_GPBR[24]) Value of SYS_GPBRx */
#define SYS_GPBR_GPBR_VALUE(value) ((SYS_GPBR_GPBR_VALUE_Msk & ((value) << SYS_GPBR_GPBR_VALUE_Pos)))

/*@}*/


#endif /* _PIC32CX_GPBR_COMPONENT_ */
