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

#ifndef _PIC32CX_CPKCC_COMPONENT_
#define _PIC32CX_CPKCC_COMPONENT_

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR Classic Public Key Cryptography Controller */
/* ============================================================================= */
/** \addtogroup PIC32CX_CPKCC Classic Public Key Cryptography Controller */
/*@{*/

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
/** \brief Cpkcc hardware registers */
typedef struct {
  __IO uint32_t CPKCC_R;         /**< \brief (Cpkcc Offset: 0x0) R Parameter Register */
  __IO uint32_t CPKCC_X;         /**< \brief (Cpkcc Offset: 0x4) X Parameter Register */
  __IO uint32_t CPKCC_Y;         /**< \brief (Cpkcc Offset: 0x8) Y Parameter Register */
  __IO uint32_t CPKCC_Z;         /**< \brief (Cpkcc Offset: 0xC) Z Parameter Register */
  __IO uint32_t CPKCC_J;         /**< \brief (Cpkcc Offset: 0x10) J Parameter Register */
  __IO uint32_t CPKCC_K;         /**< \brief (Cpkcc Offset: 0x14) K Parameter Register */
  __IO uint32_t CPKCC_N;         /**< \brief (Cpkcc Offset: 0x18) N Parameter Register */
  __IO uint32_t CPKCC_SMULA;     /**< \brief (Cpkcc Offset: 0x1C) SMULA Register */
  __IO uint32_t CPKCC_SMULB;     /**< \brief (Cpkcc Offset: 0x20) SMULB Register */
  __IO uint32_t CPKCC_SMULRL;    /**< \brief (Cpkcc Offset: 0x24) SMULRL Register */
  __IO uint32_t CPKCC_SMULRH;    /**< \brief (Cpkcc Offset: 0x28) SMULRH Register */
  __IO uint8_t  CPKCC_IDLE;      /**< \brief (Cpkcc Offset: 0x2C) IDLE Register */
  __IO uint8_t  CPKCC_IDLECACHE; /**< \brief (Cpkcc Offset: 0x2D) IDLECACHE Register */
  __I  uint8_t  Reserved1[2];
  __IO uint32_t CPKCC_CR_C;      /**< \brief (Cpkcc Offset: 0x30) CR_C Register */
  __IO uint32_t CPKCC_CR_S;      /**< \brief (Cpkcc Offset: 0x34) CR_S Register */
  __IO uint32_t CPKCC_CR;        /**< \brief (Cpkcc Offset: 0x38) CR Register */
  __IO uint32_t CPKCC_OR;        /**< \brief (Cpkcc Offset: 0x3C) Operation Register */
  __IO uint32_t CPKCC_SR;        /**< \brief (Cpkcc Offset: 0x40) SR Register */
  __I  uint32_t Reserved2[42];
  __IO uint32_t CPKCC_ADDRSIZE;  /**< \brief (Cpkcc Offset: 0xEC) ADDRSIZE Register */
  __IO uint32_t CPKCC_IPNAME[2]; /**< \brief (Cpkcc Offset: 0xF0) IPNAME1 Register */
  __IO uint32_t CPKCC_FEATURES;  /**< \brief (Cpkcc Offset: 0xF8) FEATURES Register */
} Cpkcc;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/* -------- CPKCC_R : (CPKCC Offset: 0x0) R Parameter Register -------- */
#define CPKCC_R_VALUE_Pos 0
#define CPKCC_R_VALUE_Msk (0xffffffffu << CPKCC_R_VALUE_Pos) /**< \brief (CPKCC_R) CPKCC_R value */
#define CPKCC_R_VALUE(value) ((CPKCC_R_VALUE_Msk & ((value) << CPKCC_R_VALUE_Pos)))
/* -------- CPKCC_X : (CPKCC Offset: 0x4) X Parameter Register -------- */
#define CPKCC_X_VALUE_Pos 0
#define CPKCC_X_VALUE_Msk (0xffffffffu << CPKCC_X_VALUE_Pos) /**< \brief (CPKCC_X) CPKCC_X value */
#define CPKCC_X_VALUE(value) ((CPKCC_X_VALUE_Msk & ((value) << CPKCC_X_VALUE_Pos)))
/* -------- CPKCC_Y : (CPKCC Offset: 0x8) Y Parameter Register -------- */
#define CPKCC_Y_VALUE_Pos 0
#define CPKCC_Y_VALUE_Msk (0xffffffffu << CPKCC_Y_VALUE_Pos) /**< \brief (CPKCC_Y) CPKCC_Y value */
#define CPKCC_Y_VALUE(value) ((CPKCC_Y_VALUE_Msk & ((value) << CPKCC_Y_VALUE_Pos)))
/* -------- CPKCC_Z : (CPKCC Offset: 0xC) Z Parameter Register -------- */
#define CPKCC_Z_VALUE_Pos 0
#define CPKCC_Z_VALUE_Msk (0xffffffffu << CPKCC_Z_VALUE_Pos) /**< \brief (CPKCC_Z) CPKCC_Z value */
#define CPKCC_Z_VALUE(value) ((CPKCC_Z_VALUE_Msk & ((value) << CPKCC_Z_VALUE_Pos)))
/* -------- CPKCC_J : (CPKCC Offset: 0x10) J Parameter Register -------- */
#define CPKCC_J_VALUE_Pos 0
#define CPKCC_J_VALUE_Msk (0xffffffffu << CPKCC_J_VALUE_Pos) /**< \brief (CPKCC_J) CPKCC_J value */
#define CPKCC_J_VALUE(value) ((CPKCC_J_VALUE_Msk & ((value) << CPKCC_J_VALUE_Pos)))
/* -------- CPKCC_K : (CPKCC Offset: 0x14) K Parameter Register -------- */
#define CPKCC_K_VALUE_Pos 0
#define CPKCC_K_VALUE_Msk (0xffffffffu << CPKCC_K_VALUE_Pos) /**< \brief (CPKCC_K) CPKCC_K value */
#define CPKCC_K_VALUE(value) ((CPKCC_K_VALUE_Msk & ((value) << CPKCC_K_VALUE_Pos)))
/* -------- CPKCC_N : (CPKCC Offset: 0x18) N Parameter Register -------- */
#define CPKCC_N_VALUE_Pos 0
#define CPKCC_N_VALUE_Msk (0xffffffffu << CPKCC_N_VALUE_Pos) /**< \brief (CPKCC_N) CPKCC_N value */
#define CPKCC_N_VALUE(value) ((CPKCC_N_VALUE_Msk & ((value) << CPKCC_N_VALUE_Pos)))
/* -------- CPKCC_SMULA : (CPKCC Offset: 0x1C) SMULA Register -------- */
#define CPKCC_SMULA_VALUE_Pos 0
#define CPKCC_SMULA_VALUE_Msk (0xffffffffu << CPKCC_SMULA_VALUE_Pos) /**< \brief (CPKCC_SMULA) CPKCC_SMULA value */
#define CPKCC_SMULA_VALUE(value) ((CPKCC_SMULA_VALUE_Msk & ((value) << CPKCC_SMULA_VALUE_Pos)))
/* -------- CPKCC_SMULB : (CPKCC Offset: 0x20) SMULB Register -------- */
#define CPKCC_SMULB_VALUE_Pos 0
#define CPKCC_SMULB_VALUE_Msk (0xffffffffu << CPKCC_SMULB_VALUE_Pos) /**< \brief (CPKCC_SMULB) CPKCC_SMULB value */
#define CPKCC_SMULB_VALUE(value) ((CPKCC_SMULB_VALUE_Msk & ((value) << CPKCC_SMULB_VALUE_Pos)))
/* -------- CPKCC_SMULRL : (CPKCC Offset: 0x24) SMULRL Register -------- */
#define CPKCC_SMULRL_VALUE_Pos 0
#define CPKCC_SMULRL_VALUE_Msk (0xffffffffu << CPKCC_SMULRL_VALUE_Pos) /**< \brief (CPKCC_SMULRL) CPKCC_SMULRL value */
#define CPKCC_SMULRL_VALUE(value) ((CPKCC_SMULRL_VALUE_Msk & ((value) << CPKCC_SMULRL_VALUE_Pos)))
/* -------- CPKCC_SMULRH : (CPKCC Offset: 0x28) SMULRH Register -------- */
#define CPKCC_SMULRH_VALUE_Pos 0
#define CPKCC_SMULRH_VALUE_Msk (0xffffffffu << CPKCC_SMULRH_VALUE_Pos) /**< \brief (CPKCC_SMULRH) CPKCC_SMULRH value */
#define CPKCC_SMULRH_VALUE(value) ((CPKCC_SMULRH_VALUE_Msk & ((value) << CPKCC_SMULRH_VALUE_Pos)))
/* -------- CPKCC_IDLE : (CPKCC Offset: 0x2C) IDLE Register -------- */
#define CPKCC_IDLE_VALUE_Pos 0
#define CPKCC_IDLE_VALUE_Msk (0xffu << CPKCC_IDLE_VALUE_Pos) /**< \brief (CPKCC_IDLE) CPKCC_IDLE value */
#define CPKCC_IDLE_VALUE(value) ((CPKCC_IDLE_VALUE_Msk & ((value) << CPKCC_IDLE_VALUE_Pos)))
/* -------- CPKCC_IDLECACHE : (CPKCC Offset: 0x2D) IDLECACHE Register -------- */
#define CPKCC_IDLECACHE_VALUE_Pos 0
#define CPKCC_IDLECACHE_VALUE_Msk (0xffu << CPKCC_IDLECACHE_VALUE_Pos) /**< \brief (CPKCC_IDLECACHE) CPKCC_IDLECACHE value */
#define CPKCC_IDLECACHE_VALUE(value) ((CPKCC_IDLECACHE_VALUE_Msk & ((value) << CPKCC_IDLECACHE_VALUE_Pos)))
/* -------- CPKCC_CR_C : (CPKCC Offset: 0x30) CR_C Register -------- */
#define CPKCC_CR_C_VALUE_Pos 0
#define CPKCC_CR_C_VALUE_Msk (0xffffffffu << CPKCC_CR_C_VALUE_Pos) /**< \brief (CPKCC_CR_C) CPKCC_CR_C value */
#define CPKCC_CR_C_VALUE(value) ((CPKCC_CR_C_VALUE_Msk & ((value) << CPKCC_CR_C_VALUE_Pos)))
/* -------- CPKCC_CR_S : (CPKCC Offset: 0x34) CR_S Register -------- */
#define CPKCC_CR_S_VALUE_Pos 0
#define CPKCC_CR_S_VALUE_Msk (0xffffffffu << CPKCC_CR_S_VALUE_Pos) /**< \brief (CPKCC_CR_S) CPKCC_CR_S value */
#define CPKCC_CR_S_VALUE(value) ((CPKCC_CR_S_VALUE_Msk & ((value) << CPKCC_CR_S_VALUE_Pos)))
/* -------- CPKCC_CR : (CPKCC Offset: 0x38) CR Register -------- */
#define CPKCC_CR_GF2N (0x1u << 0) /**< \brief (CPKCC_CR) GF(2n) mode */
#define CPKCC_CR_CARRYIN (0x1u << 1) /**< \brief (CPKCC_CR) CARRYIN value */
#define CPKCC_CR_ONEMUL (0x1u << 2) /**< \brief (CPKCC_CR) ONEMUL value */
#define CPKCC_CR_ABORT (0x1u << 3) /**< \brief (CPKCC_CR) ABORT value */
#define CPKCC_CR_SPILLW (0x1u << 4) /**< \brief (CPKCC_CR) Spill Word register bit */
#define CPKCC_CR_AWAKE (0x1u << 5) /**< \brief (CPKCC_CR) AWAKE value */
#define CPKCC_CR_PKCCIRQ (0x1u << 6) /**< \brief (CPKCC_CR) CPKCC_IRQ value */
#define CPKCC_CR_ITEN (0x1u << 7) /**< \brief (CPKCC_CR) ITEN value */
#define CPKCC_CR_CLRRAM (0x1u << 8) /**< \brief (CPKCC_CR) CLRRAM value */
#define CPKCC_CR_VALUE_Pos 9
#define CPKCC_CR_VALUE_Msk (0x7fffffu << CPKCC_CR_VALUE_Pos) /**< \brief (CPKCC_CR) CPKCC_CR value */
#define CPKCC_CR_VALUE(value) ((CPKCC_CR_VALUE_Msk & ((value) << CPKCC_CR_VALUE_Pos)))
/* -------- CPKCC_OR : (CPKCC Offset: 0x3C) Operation Register -------- */
#define CPKCC_OR_CMD0 (0x1u << 0) /**< \brief (CPKCC_OR) CMD0 value */
#define CPKCC_OR_CMD1 (0x1u << 1) /**< \brief (CPKCC_OR) CMD1 value */
#define CPKCC_OR_CMD2 (0x1u << 2) /**< \brief (CPKCC_OR) CMD2 value */
#define CPKCC_OR_CMD3 (0x1u << 3) /**< \brief (CPKCC_OR) CMD3 value */
#define CPKCC_OR_OPTM0 (0x1u << 4) /**< \brief (CPKCC_OR) OPTM0 value */
#define CPKCC_OR_OPTM1 (0x1u << 5) /**< \brief (CPKCC_OR) OPTM1 value */
#define CPKCC_OR_OPTC0 (0x1u << 6) /**< \brief (CPKCC_OR) OPTC0 value */
#define CPKCC_OR_OPTC1 (0x1u << 7) /**< \brief (CPKCC_OR) OPTC1 value */
#define CPKCC_OR_CARRYMUL (0x1u << 10) /**< \brief (CPKCC_OR) CARRYMUL value */
#define CPKCC_OR_VALUE_Pos 11
#define CPKCC_OR_VALUE_Msk (0x1fffffu << CPKCC_OR_VALUE_Pos) /**< \brief (CPKCC_OR) CPKCC_OR value */
#define CPKCC_OR_VALUE(value) ((CPKCC_OR_VALUE_Msk & ((value) << CPKCC_OR_VALUE_Pos)))
/* -------- CPKCC_SR : (CPKCC Offset: 0x40) SR Register -------- */
#define CPKCC_SR_BUSY (0x1u << 0) /**< \brief (CPKCC_SR) BUSY value */
#define CPKCC_SR_CACHE (0x1u << 1) /**< \brief (CPKCC_SR) CACHE value */
#define CPKCC_SR_CARRY (0x1u << 2) /**< \brief (CPKCC_SR) CARRY value */
#define CPKCC_SR_ZERO (0x1u << 3) /**< \brief (CPKCC_SR) ZERO value */
#define CPKCC_SR_RAMV (0x1u << 4) /**< \brief (CPKCC_SR) RAM violation */
#define CPKCC_SR_SHAREV (0x1u << 5) /**< \brief (CPKCC_SR) SHAREV value */
#define CPKCC_SR_CLRRAM_BUSY (0x1u << 6) /**< \brief (CPKCC_SR) CLRRAM_BUSY value */
#define CPKCC_SR_VALUE_Pos 7
#define CPKCC_SR_VALUE_Msk (0x1ffffffu << CPKCC_SR_VALUE_Pos) /**< \brief (CPKCC_SR) CPKCC_SR value */
#define CPKCC_SR_VALUE(value) ((CPKCC_SR_VALUE_Msk & ((value) << CPKCC_SR_VALUE_Pos)))
/* -------- CPKCC_ADDRSIZE : (CPKCC Offset: 0xEC) ADDRSIZE Register -------- */
#define CPKCC_ADDRSIZE_VALUE_Pos 0
#define CPKCC_ADDRSIZE_VALUE_Msk (0xffffffffu << CPKCC_ADDRSIZE_VALUE_Pos) /**< \brief (CPKCC_ADDRSIZE) CPKCC_ADDRSIZE value */
#define CPKCC_ADDRSIZE_VALUE(value) ((CPKCC_ADDRSIZE_VALUE_Msk & ((value) << CPKCC_ADDRSIZE_VALUE_Pos)))
/* -------- CPKCC_IPNAME[2] : (CPKCC Offset: 0xF0) IPNAME1 Register -------- */
#define CPKCC_IPNAME_VALUE_Pos 0
#define CPKCC_IPNAME_VALUE_Msk (0xffffffffu << CPKCC_IPNAME_VALUE_Pos) /**< \brief (CPKCC_IPNAME[2]) CPKCC_IPNAME1 value */
#define CPKCC_IPNAME_VALUE(value) ((CPKCC_IPNAME_VALUE_Msk & ((value) << CPKCC_IPNAME_VALUE_Pos)))
/* -------- CPKCC_FEATURES : (CPKCC Offset: 0xF8) FEATURES Register -------- */
#define CPKCC_FEATURES_VALUE_Pos 0
#define CPKCC_FEATURES_VALUE_Msk (0xffffffffu << CPKCC_FEATURES_VALUE_Pos) /**< \brief (CPKCC_FEATURES) CPKCC_FEATURES value */
#define CPKCC_FEATURES_VALUE(value) ((CPKCC_FEATURES_VALUE_Msk & ((value) << CPKCC_FEATURES_VALUE_Pos)))

/*@}*/


#endif /* _PIC32CX_CPKCC_COMPONENT_ */
