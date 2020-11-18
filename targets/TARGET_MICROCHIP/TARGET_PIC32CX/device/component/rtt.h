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

#ifndef _PIC32CX_RTT_COMPONENT_
#define _PIC32CX_RTT_COMPONENT_

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR Real-time Timer */
/* ============================================================================= */
/** \addtogroup PIC32CX_RTT Real-time Timer */
/*@{*/

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
/** \brief Rtt hardware registers */
typedef struct {
  __IO uint32_t RTT_MR;   /**< \brief (Rtt Offset: 0x0) Mode Register */
  __IO uint32_t RTT_AR;   /**< \brief (Rtt Offset: 0x4) Alarm Register */
  __I  uint32_t RTT_VR;   /**< \brief (Rtt Offset: 0x8) Value Register */
  __I  uint32_t RTT_SR;   /**< \brief (Rtt Offset: 0xC) Status Register */
  __IO uint32_t RTT_MODR; /**< \brief (Rtt Offset: 0x10) Modulo Selection Register */
} Rtt;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/* -------- RTT_MR : (RTT Offset: 0x0) Mode Register -------- */
#define RTT_MR_RTPRES_Pos 0
#define RTT_MR_RTPRES_Msk (0xffffu << RTT_MR_RTPRES_Pos) /**< \brief (RTT_MR) Real-time Timer Prescaler Value */
#define RTT_MR_RTPRES(value) ((RTT_MR_RTPRES_Msk & ((value) << RTT_MR_RTPRES_Pos)))
#define RTT_MR_ALMIEN (0x1u << 16) /**< \brief (RTT_MR) Alarm Interrupt Enable */
#define RTT_MR_RTTINCIEN (0x1u << 17) /**< \brief (RTT_MR) Real-time Timer Increment Interrupt Enable */
#define RTT_MR_RTTRST (0x1u << 18) /**< \brief (RTT_MR) Real-time Timer Restart */
#define RTT_MR_RTTDIS (0x1u << 20) /**< \brief (RTT_MR) Real-time Timer Disable */
#define RTT_MR_INC2AEN (0x1u << 21) /**< \brief (RTT_MR) RTTINC2 Alarm and Interrupt Enable */
#define RTT_MR_RTC1HZ (0x1u << 24) /**< \brief (RTT_MR) Real-Time Clock 1Hz Clock Selection */
/* -------- RTT_AR : (RTT Offset: 0x4) Alarm Register -------- */
#define RTT_AR_ALMV_Pos 0
#define RTT_AR_ALMV_Msk (0xffffffffu << RTT_AR_ALMV_Pos) /**< \brief (RTT_AR) Alarm Value */
#define RTT_AR_ALMV(value) ((RTT_AR_ALMV_Msk & ((value) << RTT_AR_ALMV_Pos)))
/* -------- RTT_VR : (RTT Offset: 0x8) Value Register -------- */
#define RTT_VR_CRTV_Pos 0
#define RTT_VR_CRTV_Msk (0xffffffffu << RTT_VR_CRTV_Pos) /**< \brief (RTT_VR) Current Real-time Value */
/* -------- RTT_SR : (RTT Offset: 0xC) Status Register -------- */
#define RTT_SR_ALMS (0x1u << 0) /**< \brief (RTT_SR) Real-time Alarm Status (cleared on read) */
#define RTT_SR_RTTINC (0x1u << 1) /**< \brief (RTT_SR) Prescaler Roll-over Status (cleared on read) */
#define RTT_SR_RTTINC2 (0x1u << 2) /**< \brief (RTT_SR) Predefined Number of Prescaler Roll-overs Status (cleared on read) */
/* -------- RTT_MODR : (RTT Offset: 0x10) Modulo Selection Register -------- */
#define RTT_MODR_SELINC2_Pos 0
#define RTT_MODR_SELINC2_Msk (0x7u << RTT_MODR_SELINC2_Pos) /**< \brief (RTT_MODR) Selection of the 32-bit Counter Modulo to generate RTTINC2 Flag */
#define RTT_MODR_SELINC2(value) ((RTT_MODR_SELINC2_Msk & ((value) << RTT_MODR_SELINC2_Pos)))
#define   RTT_MODR_SELINC2_NO_RTTINC2 (0x0u << 0) /**< \brief (RTT_MODR) The RTTINC2 flag never rises */
#define   RTT_MODR_SELINC2_MOD64 (0x1u << 0) /**< \brief (RTT_MODR) The RTTINC2 flag is set when CRTV modulo 64 equals 0 */
#define   RTT_MODR_SELINC2_MOD128 (0x2u << 0) /**< \brief (RTT_MODR) The RTTINC2 flag is set when CRTV modulo 128 equals 0 */
#define   RTT_MODR_SELINC2_MOD256 (0x3u << 0) /**< \brief (RTT_MODR) The RTTINC2 flag is set when CRTV modulo 256 equals 0 */
#define   RTT_MODR_SELINC2_MOD512 (0x4u << 0) /**< \brief (RTT_MODR) The RTTINC2 flag is set when CRTV modulo 512 equals 0 */
#define   RTT_MODR_SELINC2_MOD1024 (0x5u << 0) /**< \brief (RTT_MODR) The RTTINC2 flag is set when CRTV modulo 1024 equals 0. Example: If RTPRES=32 then RTTINC2 flag rises once per second if the slow clock is 32.768 kHz. */
#define   RTT_MODR_SELINC2_MOD2048 (0x6u << 0) /**< \brief (RTT_MODR) The RTTINC2 flag is set when CRTV modulo 2048 equals 0 */
#define   RTT_MODR_SELINC2_MOD4096 (0x7u << 0) /**< \brief (RTT_MODR) The RTTINC2 flag is set when CRTV modulo 4096 equals 0 */

/*@}*/


#endif /* _PIC32CX_RTT_COMPONENT_ */
