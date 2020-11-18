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

#ifndef _PIC32CX_CMCC_COMPONENT_
#define _PIC32CX_CMCC_COMPONENT_

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR Cortex-M Cache Controller */
/* ============================================================================= */
/** \addtogroup PIC32CX_CMCC Cortex-M Cache Controller */
/*@{*/

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
/** \brief Cmcc hardware registers */
typedef struct {
  __I  uint32_t CMCC_TYPE;     /**< \brief (Cmcc Offset: 0x0) Cache Controller Type Register */
  __IO uint32_t CMCC_CFG;      /**< \brief (Cmcc Offset: 0x4) Configuration Register */
  __O  uint32_t CMCC_CTRL;     /**< \brief (Cmcc Offset: 0x8) Control Register */
  __I  uint32_t CMCC_SR;       /**< \brief (Cmcc Offset: 0xC) Status Register */
  __I  uint32_t Reserved1[4];
  __O  uint32_t CMCC_MAINT0;   /**< \brief (Cmcc Offset: 0x20) Maintenance Register 0 */
  __O  uint32_t CMCC_MAINT1;   /**< \brief (Cmcc Offset: 0x24) Maintenance Register 1 */
  __IO uint32_t CMCC_MCFG;     /**< \brief (Cmcc Offset: 0x28) Monitor Configuration Register */
  __IO uint32_t CMCC_MEN;      /**< \brief (Cmcc Offset: 0x2C) Monitor Enable Register */
  __O  uint32_t CMCC_MCTRL;    /**< \brief (Cmcc Offset: 0x30) Monitor Control Register */
  __I  uint32_t CMCC_MSR;      /**< \brief (Cmcc Offset: 0x34) Monitor Status Register */
  __I  uint32_t Reserved2[43];
  __IO uint32_t CMCC_WPMR;     /**< \brief (Cmcc Offset: 0xE4) Write Protection Mode Register */
} Cmcc;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/* -------- CMCC_TYPE : (CMCC Offset: 0x0) Cache Controller Type Register -------- */
#define CMCC_TYPE_AP (0x1u << 0) /**< \brief (CMCC_TYPE) Access Port Access Allowed */
#define CMCC_TYPE_GCLK (0x1u << 1) /**< \brief (CMCC_TYPE) Dynamic Clock Gating Supported */
#define CMCC_TYPE_RANDP (0x1u << 2) /**< \brief (CMCC_TYPE) Random Selection Policy Supported */
#define CMCC_TYPE_LRUP (0x1u << 3) /**< \brief (CMCC_TYPE) Least Recently Used Policy Supported */
#define CMCC_TYPE_RRP (0x1u << 4) /**< \brief (CMCC_TYPE) Random Selection Policy Supported */
#define CMCC_TYPE_WAYNUM_Pos 5
#define CMCC_TYPE_WAYNUM_Msk (0x3u << CMCC_TYPE_WAYNUM_Pos) /**< \brief (CMCC_TYPE) Number of Ways */
#define   CMCC_TYPE_WAYNUM_DMAPPED (0x0u << 5) /**< \brief (CMCC_TYPE) Direct Mapped Cache */
#define   CMCC_TYPE_WAYNUM_ARCH2WAY (0x1u << 5) /**< \brief (CMCC_TYPE) 2-way set associative */
#define   CMCC_TYPE_WAYNUM_ARCH4WAY (0x2u << 5) /**< \brief (CMCC_TYPE) 4-way set associative */
#define   CMCC_TYPE_WAYNUM_ARCH8WAY (0x3u << 5) /**< \brief (CMCC_TYPE) 8-way set associative */
#define CMCC_TYPE_LCKDOWN (0x1u << 7) /**< \brief (CMCC_TYPE) Lockdown Supported */
#define CMCC_TYPE_CSIZE_Pos 8
#define CMCC_TYPE_CSIZE_Msk (0x7u << CMCC_TYPE_CSIZE_Pos) /**< \brief (CMCC_TYPE) Data Cache Size */
#define   CMCC_TYPE_CSIZE_CSIZE_1KB (0x0u << 8) /**< \brief (CMCC_TYPE) Cache size is 1 Kbyte */
#define   CMCC_TYPE_CSIZE_CSIZE_2KB (0x1u << 8) /**< \brief (CMCC_TYPE) Cache size is 2 Kbytes */
#define   CMCC_TYPE_CSIZE_CSIZE_4KB (0x2u << 8) /**< \brief (CMCC_TYPE) Cache size is 4 Kbytes */
#define   CMCC_TYPE_CSIZE_CSIZE_8KB (0x3u << 8) /**< \brief (CMCC_TYPE) Cache size is 8 Kbytes */
#define   CMCC_TYPE_CSIZE_CSIZE_16KB (0x4u << 8) /**< \brief (CMCC_TYPE) Cache size is 16 Kbytes */
#define   CMCC_TYPE_CSIZE_CSIZE_32KB (0x5u << 8) /**< \brief (CMCC_TYPE) Cache size is 32 Kbytes */
#define   CMCC_TYPE_CSIZE_CSIZE_64KB (0x6u << 8) /**< \brief (CMCC_TYPE) Cache size is 64 Kbytes */
#define CMCC_TYPE_CLSIZE_Pos 11
#define CMCC_TYPE_CLSIZE_Msk (0x7u << CMCC_TYPE_CLSIZE_Pos) /**< \brief (CMCC_TYPE) Cache LIne Size */
#define   CMCC_TYPE_CLSIZE_CLSIZE_1KB (0x0u << 11) /**< \brief (CMCC_TYPE) Cache line size is 4 bytes */
#define   CMCC_TYPE_CLSIZE_CLSIZE_2KB (0x1u << 11) /**< \brief (CMCC_TYPE) Cache line size is 8 bytes */
#define   CMCC_TYPE_CLSIZE_CLSIZE_4KB (0x2u << 11) /**< \brief (CMCC_TYPE) Cache line size is 16 bytes */
#define   CMCC_TYPE_CLSIZE_CLSIZE_8KB (0x3u << 11) /**< \brief (CMCC_TYPE) Cache line size is 32 bytes */
/* -------- CMCC_CFG : (CMCC Offset: 0x4) Configuration Register -------- */
#define CMCC_CFG_GCLKDIS (0x1u << 0) /**< \brief (CMCC_CFG) Disable Clock Gating */
#define CMCC_CFG_ICDIS (0x1u << 1) /**< \brief (CMCC_CFG) Instruction Caching Disable */
#define CMCC_CFG_DCDIS (0x1u << 2) /**< \brief (CMCC_CFG) Data Caching Disable */
#define CMCC_CFG_PRGCSIZE_Pos 4
#define CMCC_CFG_PRGCSIZE_Msk (0x7u << CMCC_CFG_PRGCSIZE_Pos) /**< \brief (CMCC_CFG) Programmable Cache Size */
#define CMCC_CFG_PRGCSIZE(value) ((CMCC_CFG_PRGCSIZE_Msk & ((value) << CMCC_CFG_PRGCSIZE_Pos)))
#define CMCC_CFG_LOCK (0x1u << 8) /**< \brief (CMCC_CFG) Configuration Lock Until Next System Reset (Write-once) */
/* -------- CMCC_CTRL : (CMCC Offset: 0x8) Control Register -------- */
#define CMCC_CTRL_CEN (0x1u << 0) /**< \brief (CMCC_CTRL) Cache Controller Enable */
#define CMCC_CTRL_LOCK (0x1u << 8) /**< \brief (CMCC_CTRL) Control Lock Until Next System Reset (Write-once) */
/* -------- CMCC_SR : (CMCC Offset: 0xC) Status Register -------- */
#define CMCC_SR_CSTS (0x1u << 0) /**< \brief (CMCC_SR) Cache Controller Status */
/* -------- CMCC_MAINT0 : (CMCC Offset: 0x20) Maintenance Register 0 -------- */
#define CMCC_MAINT0_INVALL (0x1u << 0) /**< \brief (CMCC_MAINT0) Cache Controller Invalidate All */
/* -------- CMCC_MAINT1 : (CMCC Offset: 0x24) Maintenance Register 1 -------- */
#define CMCC_MAINT1_INDEX_Pos 4
#define CMCC_MAINT1_INDEX_Msk (0x7fu << CMCC_MAINT1_INDEX_Pos) /**< \brief (CMCC_MAINT1) Invalidate Index */
#define CMCC_MAINT1_INDEX(value) ((CMCC_MAINT1_INDEX_Msk & ((value) << CMCC_MAINT1_INDEX_Pos)))
#define CMCC_MAINT1_WAY_Pos 30
#define CMCC_MAINT1_WAY_Msk (0x3u << CMCC_MAINT1_WAY_Pos) /**< \brief (CMCC_MAINT1) Invalidate Way */
#define CMCC_MAINT1_WAY(value) ((CMCC_MAINT1_WAY_Msk & ((value) << CMCC_MAINT1_WAY_Pos)))
#define   CMCC_MAINT1_WAY_WAY0 (0x0u << 30) /**< \brief (CMCC_MAINT1) Way 0 is selection for index invalidation. */
#define   CMCC_MAINT1_WAY_WAY1 (0x1u << 30) /**< \brief (CMCC_MAINT1) Way 1 is selection for index invalidation. */
#define   CMCC_MAINT1_WAY_WAY2 (0x2u << 30) /**< \brief (CMCC_MAINT1) Way 2 is selection for index invalidation. */
#define   CMCC_MAINT1_WAY_WAY3 (0x3u << 30) /**< \brief (CMCC_MAINT1) Way 3 is selection for index invalidation. */
/* -------- CMCC_MCFG : (CMCC Offset: 0x28) Monitor Configuration Register -------- */
#define CMCC_MCFG_MODE_Pos 0
#define CMCC_MCFG_MODE_Msk (0x3u << CMCC_MCFG_MODE_Pos) /**< \brief (CMCC_MCFG) Cache Controller Monitor Counter Mode */
#define CMCC_MCFG_MODE(value) ((CMCC_MCFG_MODE_Msk & ((value) << CMCC_MCFG_MODE_Pos)))
#define   CMCC_MCFG_MODE_CYCLE_COUNT (0x0u << 0) /**< \brief (CMCC_MCFG) Cycle counter */
#define   CMCC_MCFG_MODE_IHIT_COUNT (0x1u << 0) /**< \brief (CMCC_MCFG) Instruction hit counter, only relevant for unified cache and instruction cache architecture */
#define   CMCC_MCFG_MODE_DHIT_COUNT (0x2u << 0) /**< \brief (CMCC_MCFG) Data hit counter, only relevant for unified cache and data cache architecture */
/* -------- CMCC_MEN : (CMCC Offset: 0x2C) Monitor Enable Register -------- */
#define CMCC_MEN_MENABLE (0x1u << 0) /**< \brief (CMCC_MEN) Cache Controller Monitor Enable */
/* -------- CMCC_MCTRL : (CMCC Offset: 0x30) Monitor Control Register -------- */
#define CMCC_MCTRL_SWRST (0x1u << 0) /**< \brief (CMCC_MCTRL) Monitor */
/* -------- CMCC_MSR : (CMCC Offset: 0x34) Monitor Status Register -------- */
#define CMCC_MSR_EVENT_CNT_Pos 0
#define CMCC_MSR_EVENT_CNT_Msk (0xffffffffu << CMCC_MSR_EVENT_CNT_Pos) /**< \brief (CMCC_MSR) Monitor Event Counter */
/* -------- CMCC_WPMR : (CMCC Offset: 0xE4) Write Protection Mode Register -------- */
#define CMCC_WPMR_WPCFG (0x1u << 0) /**< \brief (CMCC_WPMR) Write Protection Configuration Enable */
#define CMCC_WPMR_WPCR (0x1u << 2) /**< \brief (CMCC_WPMR) Write Protection Control Enable */
#define CMCC_WPMR_WPKEY_Pos 8
#define CMCC_WPMR_WPKEY_Msk (0xffffffu << CMCC_WPMR_WPKEY_Pos) /**< \brief (CMCC_WPMR) Write Protection Key */
#define CMCC_WPMR_WPKEY(value) ((CMCC_WPMR_WPKEY_Msk & ((value) << CMCC_WPMR_WPKEY_Pos)))
#define   CMCC_WPMR_WPKEY_PASSWD (0x434D43u << 8) /**< \brief (CMCC_WPMR) Writing any other value in this field aborts the write operation of the WPCFG and WPCR bits. Always reads as 0. */

/*@}*/


#endif /* _PIC32CX_CMCC_COMPONENT_ */
