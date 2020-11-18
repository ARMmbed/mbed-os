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

#ifndef _PIC32CX_SEFC_COMPONENT_
#define _PIC32CX_SEFC_COMPONENT_

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR Secure Embedded Flash Controller */
/* ============================================================================= */
/** \addtogroup PIC32CX_SEFC Secure Embedded Flash Controller */
/*@{*/

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
/** \brief Sefc hardware registers */
typedef struct {
  __IO uint32_t EEFC_FMR;      /**< \brief (Sefc Offset: 0x0) SEFC Flash Mode Register */
  __O  uint32_t EEFC_FCR;      /**< \brief (Sefc Offset: 0x4) SEFC Flash Command Register */
  __I  uint32_t EEFC_FSR;      /**< \brief (Sefc Offset: 0x8) SEFC Flash Status Register */
  __I  uint32_t EEFC_FRR;      /**< \brief (Sefc Offset: 0xC) SEFC Flash Result Register */
  __IO uint32_t EEFC_USR;      /**< \brief (Sefc Offset: 0x10) SEFC User Signature Rights Register */
  __IO uint32_t EEFC_KBLR;     /**< \brief (Sefc Offset: 0x14) SEFC Key Bus Lock Register */
  __I  uint32_t Reserved1[51];
  __IO uint32_t EEFC_WPMR;     /**< \brief (Sefc Offset: 0xE4) Write Protection Mode Register */
} Sefc;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/* -------- EEFC_FMR : (SEFC Offset: 0x0) SEFC Flash Mode Register -------- */
#define EEFC_FMR_FRDY (0x1u << 0) /**< \brief (EEFC_FMR) Flash Ready Interrupt Enable */
#define EEFC_FMR_FWS_Pos 8
#define EEFC_FMR_FWS_Msk (0xfu << EEFC_FMR_FWS_Pos) /**< \brief (EEFC_FMR) Flash Wait State */
#define EEFC_FMR_FWS(value) ((EEFC_FMR_FWS_Msk & ((value) << EEFC_FMR_FWS_Pos)))
#define EEFC_FMR_SCOD (0x1u << 16) /**< \brief (EEFC_FMR) Sequential Code Optimization Disable */
#define EEFC_FMR_CLOE (0x1u << 26) /**< \brief (EEFC_FMR) Code Loop Optimization Enable */
/* -------- EEFC_FCR : (SEFC Offset: 0x4) SEFC Flash Command Register -------- */
#define EEFC_FCR_FCMD_Pos 0
#define EEFC_FCR_FCMD_Msk (0xffu << EEFC_FCR_FCMD_Pos) /**< \brief (EEFC_FCR) Flash Command */
#define EEFC_FCR_FCMD(value) ((EEFC_FCR_FCMD_Msk & ((value) << EEFC_FCR_FCMD_Pos)))
#define   EEFC_FCR_FCMD_GETD (0x0u << 0) /**< \brief (EEFC_FCR) Get Flash descriptor */
#define   EEFC_FCR_FCMD_WP (0x1u << 0) /**< \brief (EEFC_FCR) Write page */
#define   EEFC_FCR_FCMD_WPL (0x2u << 0) /**< \brief (EEFC_FCR) Write page and lock */
#define   EEFC_FCR_FCMD_EA (0x5u << 0) /**< \brief (EEFC_FCR) Erase all */
#define   EEFC_FCR_FCMD_EPA (0x7u << 0) /**< \brief (EEFC_FCR) Erase pages */
#define   EEFC_FCR_FCMD_SLB (0x8u << 0) /**< \brief (EEFC_FCR) Set lock bit */
#define   EEFC_FCR_FCMD_CLB (0x9u << 0) /**< \brief (EEFC_FCR) Clear lock bit */
#define   EEFC_FCR_FCMD_GLB (0xAu << 0) /**< \brief (EEFC_FCR) Get lock bit */
#define   EEFC_FCR_FCMD_SGPB (0xBu << 0) /**< \brief (EEFC_FCR) Set GPNVM bit */
#define   EEFC_FCR_FCMD_CGPB (0xCu << 0) /**< \brief (EEFC_FCR) Clear GPNVM bit */
#define   EEFC_FCR_FCMD_GGPB (0xDu << 0) /**< \brief (EEFC_FCR) Get GPNVM bit */
#define   EEFC_FCR_FCMD_STUI (0xEu << 0) /**< \brief (EEFC_FCR) Start read unique identifier */
#define   EEFC_FCR_FCMD_SPUI (0xFu << 0) /**< \brief (EEFC_FCR) Stop read unique identifier */
#define   EEFC_FCR_FCMD_GCALB (0x10u << 0) /**< \brief (EEFC_FCR) Get CALIB bit */
#define   EEFC_FCR_FCMD_ES (0x11u << 0) /**< \brief (EEFC_FCR) Erase sector */
#define   EEFC_FCR_FCMD_WUS (0x12u << 0) /**< \brief (EEFC_FCR) Write user signature */
#define   EEFC_FCR_FCMD_EUS (0x13u << 0) /**< \brief (EEFC_FCR) Erase user signature */
#define   EEFC_FCR_FCMD_STUS (0x14u << 0) /**< \brief (EEFC_FCR) Start read user signature */
#define   EEFC_FCR_FCMD_SPUS (0x15u << 0) /**< \brief (EEFC_FCR) Stop read user signature */
#define   EEFC_FCR_FCMD_SUSP (0x17u << 0) /**< \brief (EEFC_FCR) Suspend */
#define   EEFC_FCR_FCMD_RES (0x18u << 0) /**< \brief (EEFC_FCR) Resume */
#define   EEFC_FCR_FCMD_SCK (0x19u << 0) /**< \brief (EEFC_FCR) Send cryptographic key */
#define EEFC_FCR_FARG_Pos 8
#define EEFC_FCR_FARG_Msk (0xffffu << EEFC_FCR_FARG_Pos) /**< \brief (EEFC_FCR) Flash Command Argument */
#define EEFC_FCR_FARG(value) ((EEFC_FCR_FARG_Msk & ((value) << EEFC_FCR_FARG_Pos)))
#define EEFC_FCR_FKEY_Pos 24
#define EEFC_FCR_FKEY_Msk (0xffu << EEFC_FCR_FKEY_Pos) /**< \brief (EEFC_FCR) Flash Writing Protection Key */
#define EEFC_FCR_FKEY(value) ((EEFC_FCR_FKEY_Msk & ((value) << EEFC_FCR_FKEY_Pos)))
#define   EEFC_FCR_FKEY_PASSWD (0x5Au << 24) /**< \brief (EEFC_FCR) The 0x5A value enables the command defined by the bits in the register. If the field is written with a different value, the write is not performed and no action is started. */
/* -------- EEFC_FSR : (SEFC Offset: 0x8) SEFC Flash Status Register -------- */
#define EEFC_FSR_FRDY (0x1u << 0) /**< \brief (EEFC_FSR) Flash Ready Status (cleared when Flash is busy) */
#define EEFC_FSR_FCMDE (0x1u << 1) /**< \brief (EEFC_FSR) Flash Command Error Status (cleared on read) */
#define EEFC_FSR_FLOCKE (0x1u << 2) /**< \brief (EEFC_FSR) Flash Lock Error Status (cleared on read or by writing EEFC_FCR) */
#define EEFC_FSR_FLERR (0x1u << 3) /**< \brief (EEFC_FSR) Flash Error Status (cleared when a programming operation starts) */
#define EEFC_FSR_WPERR (0x1u << 4) /**< \brief (EEFC_FSR) Write Protection Error Status (cleared on read) */
#define EEFC_FSR_FLSUSP (0x1u << 5) /**< \brief (EEFC_FSR) Flash Suspended Status (cleared when resuming the programming operation) */
#define EEFC_FSR_SECCELSBD (0x1u << 16) /**< \brief (EEFC_FSR) Single ECC Error on LSB Part of the Memory Flash Data Bus (cleared on read) */
#define EEFC_FSR_MECCELSBD (0x1u << 17) /**< \brief (EEFC_FSR) Multiple ECC Error on LSB Part of the Memory Flash Data Bus (cleared on read) */
#define EEFC_FSR_SECCEMSBD (0x1u << 18) /**< \brief (EEFC_FSR) Single ECC Error on MSB Part of the Memory Flash Data Bus (cleared on read) */
#define EEFC_FSR_MECCEMSBD (0x1u << 19) /**< \brief (EEFC_FSR) Multiple ECC Error on MSB Part of the Memory Flash Data Bus (cleared on read) */
#define EEFC_FSR_SECCELSBL (0x1u << 20) /**< \brief (EEFC_FSR) Single ECC Error on LSB Part of the Memory Lock Bits */
#define EEFC_FSR_MECCELSBL (0x1u << 21) /**< \brief (EEFC_FSR) Multiple ECC Error on LSB Part of the Memory Lock Bits */
#define EEFC_FSR_SECCEMSBL (0x1u << 22) /**< \brief (EEFC_FSR) Single ECC Error on MSB Part of the Memory Lock Bits */
#define EEFC_FSR_MECCEMSBL (0x1u << 23) /**< \brief (EEFC_FSR) Multiple ECC Error on MSB Part of the Memory Lock Bits */
/* -------- EEFC_FRR : (SEFC Offset: 0xC) SEFC Flash Result Register -------- */
#define EEFC_FRR_FVALUE_Pos 0
#define EEFC_FRR_FVALUE_Msk (0xffffffffu << EEFC_FRR_FVALUE_Pos) /**< \brief (EEFC_FRR) Flash Result Value */
/* -------- EEFC_USR : (SEFC Offset: 0x10) SEFC User Signature Rights Register -------- */
#define EEFC_USR_RDENUSB0 (0x1u << 0) /**< \brief (EEFC_USR) Read Enable for User Signature Block 0 */
#define EEFC_USR_RDENUSB1 (0x1u << 1) /**< \brief (EEFC_USR) Read Enable for User Signature Block 1 */
#define EEFC_USR_RDENUSB2 (0x1u << 2) /**< \brief (EEFC_USR) Read Enable for User Signature Block 2 */
#define EEFC_USR_RDENUSB3 (0x1u << 3) /**< \brief (EEFC_USR) Read Enable for User Signature Block 3 */
#define EEFC_USR_RDENUSB4 (0x1u << 4) /**< \brief (EEFC_USR) Read Enable for User Signature Block 4 */
#define EEFC_USR_RDENUSB5 (0x1u << 5) /**< \brief (EEFC_USR) Read Enable for User Signature Block 5 */
#define EEFC_USR_RDENUSB6 (0x1u << 6) /**< \brief (EEFC_USR) Read Enable for User Signature Block 6 */
#define EEFC_USR_RDENUSB7 (0x1u << 7) /**< \brief (EEFC_USR) Read Enable for User Signature Block 7 */
#define EEFC_USR_WRENUSB0 (0x1u << 8) /**< \brief (EEFC_USR) Write Enable for User Signature Block 0 */
#define EEFC_USR_WRENUSB1 (0x1u << 9) /**< \brief (EEFC_USR) Write Enable for User Signature Block 1 */
#define EEFC_USR_WRENUSB2 (0x1u << 10) /**< \brief (EEFC_USR) Write Enable for User Signature Block 2 */
#define EEFC_USR_WRENUSB3 (0x1u << 11) /**< \brief (EEFC_USR) Write Enable for User Signature Block 3 */
#define EEFC_USR_WRENUSB4 (0x1u << 12) /**< \brief (EEFC_USR) Write Enable for User Signature Block 4 */
#define EEFC_USR_WRENUSB5 (0x1u << 13) /**< \brief (EEFC_USR) Write Enable for User Signature Block 5 */
#define EEFC_USR_WRENUSB6 (0x1u << 14) /**< \brief (EEFC_USR) Write Enable for User Signature Block 6 */
#define EEFC_USR_WRENUSB7 (0x1u << 15) /**< \brief (EEFC_USR) Write Enable for User Signature Block 7 */
#define EEFC_USR_PRIVUSB0 (0x1u << 16) /**< \brief (EEFC_USR) Privileged Access for User Signature Block 0 */
#define EEFC_USR_PRIVUSB1 (0x1u << 17) /**< \brief (EEFC_USR) Privileged Access for User Signature Block 1 */
#define EEFC_USR_PRIVUSB2 (0x1u << 18) /**< \brief (EEFC_USR) Privileged Access for User Signature Block 2 */
#define EEFC_USR_PRIVUSB3 (0x1u << 19) /**< \brief (EEFC_USR) Privileged Access for User Signature Block 3 */
#define EEFC_USR_PRIVUSB4 (0x1u << 20) /**< \brief (EEFC_USR) Privileged Access for User Signature Block 4 */
#define EEFC_USR_PRIVUSB5 (0x1u << 21) /**< \brief (EEFC_USR) Privileged Access for User Signature Block 5 */
#define EEFC_USR_PRIVUSB6 (0x1u << 22) /**< \brief (EEFC_USR) Privileged Access for User Signature Block 6 */
#define EEFC_USR_PRIVUSB7 (0x1u << 23) /**< \brief (EEFC_USR) Privileged Access for User Signature Block 7 */
#define EEFC_USR_LOCKUSRB0 (0x1u << 24) /**< \brief (EEFC_USR) Lock User Signature Rights for User Signature Block 0 */
#define EEFC_USR_LOCKUSRB1 (0x1u << 25) /**< \brief (EEFC_USR) Lock User Signature Rights for User Signature Block 1 */
#define EEFC_USR_LOCKUSRB2 (0x1u << 26) /**< \brief (EEFC_USR) Lock User Signature Rights for User Signature Block 2 */
#define EEFC_USR_LOCKUSRB3 (0x1u << 27) /**< \brief (EEFC_USR) Lock User Signature Rights for User Signature Block 3 */
#define EEFC_USR_LOCKUSRB4 (0x1u << 28) /**< \brief (EEFC_USR) Lock User Signature Rights for User Signature Block 4 */
#define EEFC_USR_LOCKUSRB5 (0x1u << 29) /**< \brief (EEFC_USR) Lock User Signature Rights for User Signature Block 5 */
#define EEFC_USR_LOCKUSRB6 (0x1u << 30) /**< \brief (EEFC_USR) Lock User Signature Rights for User Signature Block 6 */
#define EEFC_USR_LOCKUSRB7 (0x1u << 31) /**< \brief (EEFC_USR) Lock User Signature Rights for User Signature Block 7 */
/* -------- EEFC_KBLR : (SEFC Offset: 0x14) SEFC Key Bus Lock Register -------- */
#define EEFC_KBLR_KBTLUSB0 (0x1u << 0) /**< \brief (EEFC_KBLR) Key Bus Transfer Lock from User Signature Block 0 */
#define EEFC_KBLR_KBTLUSB1 (0x1u << 1) /**< \brief (EEFC_KBLR) Key Bus Transfer Lock from User Signature Block 1 */
#define EEFC_KBLR_KBTLUSB2 (0x1u << 2) /**< \brief (EEFC_KBLR) Key Bus Transfer Lock from User Signature Block 2 */
#define EEFC_KBLR_KBTLUSB3 (0x1u << 3) /**< \brief (EEFC_KBLR) Key Bus Transfer Lock from User Signature Block 3 */
#define EEFC_KBLR_KBTLUSB4 (0x1u << 4) /**< \brief (EEFC_KBLR) Key Bus Transfer Lock from User Signature Block 4 */
#define EEFC_KBLR_KBTLUSB5 (0x1u << 5) /**< \brief (EEFC_KBLR) Key Bus Transfer Lock from User Signature Block 5 */
#define EEFC_KBLR_KBTLUSB6 (0x1u << 6) /**< \brief (EEFC_KBLR) Key Bus Transfer Lock from User Signature Block 6 */
#define EEFC_KBLR_KBTLUSB7 (0x1u << 7) /**< \brief (EEFC_KBLR) Key Bus Transfer Lock from User Signature Block 7 */
/* -------- EEFC_WPMR : (SEFC Offset: 0xE4) Write Protection Mode Register -------- */
#define EEFC_WPMR_WPEN (0x1u << 0) /**< \brief (EEFC_WPMR) Write Protection Enable */
#define EEFC_WPMR_GPNVMWP (0x1u << 1) /**< \brief (EEFC_WPMR) GPNVM Bit Write Protection */
#define EEFC_WPMR_LOCKWP (0x1u << 2) /**< \brief (EEFC_WPMR) Lock Bit Write Protection */
#define EEFC_WPMR_ERASEWP (0x1u << 3) /**< \brief (EEFC_WPMR) Erase and Write Protection */
#define EEFC_WPMR_USRWP (0x1u << 4) /**< \brief (EEFC_WPMR) User Signature Write Protection */
#define EEFC_WPMR_ERASEWL (0x1u << 7) /**< \brief (EEFC_WPMR) Erase and Write Lock */
#define EEFC_WPMR_WPKEY_Pos 8
#define EEFC_WPMR_WPKEY_Msk (0xffffffu << EEFC_WPMR_WPKEY_Pos) /**< \brief (EEFC_WPMR) Write Protection Key */
#define EEFC_WPMR_WPKEY(value) ((EEFC_WPMR_WPKEY_Msk & ((value) << EEFC_WPMR_WPKEY_Pos)))
#define   EEFC_WPMR_WPKEY_PASSWD (0x454643u << 8) /**< \brief (EEFC_WPMR) Writing any other value in this field aborts the write operation. Always reads as 0. */

/*@}*/


#endif /* _PIC32CX_SEFC_COMPONENT_ */
