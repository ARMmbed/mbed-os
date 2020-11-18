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

#ifndef _PIC32CX_SFR_COMPONENT_
#define _PIC32CX_SFR_COMPONENT_

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR Special Function Register */
/* ============================================================================= */
/** \addtogroup PIC32CX_SFR Special Function Register */
/*@{*/

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
/** \brief Sfr hardware registers */
typedef struct {
  __I  uint32_t Reserved1[4];
  __IO uint32_t SFR_SRAM0_SW_CFG;   /**< \brief (Sfr Offset: 0x10) SRAM0 Software Config Register */
  __I  uint32_t SFR_SRAM0_HW_CFG;   /**< \brief (Sfr Offset: 0x14) SRAM0 Hardware Status Register */
  __IO uint32_t SFR_SRAM0_CH[4];    /**< \brief (Sfr Offset: 0x18) SRAM0 Read Margin Register Channel */
  __IO uint32_t SFR_SRAM[2];        /**< \brief (Sfr Offset: 0x28) SRAM Configuration Register */
  __IO uint32_t SFR_CPKCC;          /**< \brief (Sfr Offset: 0x30) CPKCC Memory Configuration Register */
  __I  uint32_t Reserved2[3];
  __IO uint32_t SFR_HROMC;          /**< \brief (Sfr Offset: 0x40) HROMC Memory Configuration Register */
  __I  uint32_t Reserved3[3];
  __IO uint32_t SFR_HCACHEI_VALID;  /**< \brief (Sfr Offset: 0x50) HCACHEI Valid Memory Configuration Register */
  __IO uint32_t SFR_HCACHEI_DATA;   /**< \brief (Sfr Offset: 0x54) HCACHEI Data Memory Configuration Register */
  __IO uint32_t SFR_HCACHEI_TAG;    /**< \brief (Sfr Offset: 0x58) HCACHEI Tag Memory Configuration Register */
  __I  uint32_t Reserved4[1];
  __IO uint32_t SFR_HCACHED_VALID;  /**< \brief (Sfr Offset: 0x60) HCACHED Valid Memory Configuration Register */
  __IO uint32_t SFR_HCACHED_DATA;   /**< \brief (Sfr Offset: 0x64) HCACHED Data Memory Configuration Register */
  __IO uint32_t SFR_HCACHED_TAG;    /**< \brief (Sfr Offset: 0x68) HCACHEDTag Memory Configuration Register */
  __I  uint32_t Reserved5[1];
  __IO uint32_t SFR_FLASH;          /**< \brief (Sfr Offset: 0x70) Flash Memory Configuration Register */
  __I  uint32_t Reserved6[3];
  __IO uint32_t SFR_OPT_LINK;       /**< \brief (Sfr Offset: 0x80) Optical Link Register */
  __I  uint32_t Reserved7[3];
  __IO uint32_t SFR_JTAG;           /**< \brief (Sfr Offset: 0x90) JTAG Register */
  __I  uint32_t Reserved8[3];
  __IO uint32_t SFR_CORE_DEBUG_CFG; /**< \brief (Sfr Offset: 0xA0) Core Debug Configuration Register */
  __IO uint32_t SFR_EMAHB2AHB;      /**< \brief (Sfr Offset: 0xA4) AHB2AHB Configuration Register */
  __IO uint32_t SFR_SECURE;         /**< \brief (Sfr Offset: 0xA8) Secure Register */
  __I  uint32_t SFR_SECURE_BIT;     /**< \brief (Sfr Offset: 0xAC) Secure Bit Register */
  __IO uint32_t SFR_ERASE_FLASH;    /**< \brief (Sfr Offset: 0xB0) Erase Flash Register */
  __IO uint32_t SFR_PWM_DEBUG;      /**< \brief (Sfr Offset: 0xB4) PWM Debug Register */
  __I  uint32_t SFR_FFPI;           /**< \brief (Sfr Offset: 0xB8) FFPI Register */
  __IO uint32_t SFR_WAIT_MODE;      /**< \brief (Sfr Offset: 0xBC) Improved Wait Mode Register */
  __I  uint32_t SFR_ROM_CODE;       /**< \brief (Sfr Offset: 0xC0) ROM Code Register */
  __I  uint32_t Reserved9[8];
  __IO uint32_t SFR_WPMR;           /**< \brief (Sfr Offset: 0xE4) Write Protection Mode Register */
  __IO uint32_t SFR_WPSR;           /**< \brief (Sfr Offset: 0xE8) Write Protection Status Register */
  __I  uint32_t Reserved10[1];
  __IO uint32_t SFR_SPARE;          /**< \brief (Sfr Offset: 0xF0) Spare Register */
  __I  uint32_t Reserved11[2];
  __I  uint32_t SFR_VERSION;        /**< \brief (Sfr Offset: 0xFC) Version Register */
} Sfr;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/* -------- SFR_SRAM0_SW_CFG : (SFR Offset: 0x10) SRAM0 Software Config Register -------- */
#define SFR_SRAM0_SW_CFG_M128LS (0x1u << 0) /**< \brief (SFR_SRAM0_SW_CFG) 128 KB Light Sleep Mode */
#define SFR_SRAM0_SW_CFG_M128DS (0x1u << 1) /**< \brief (SFR_SRAM0_SW_CFG) 128 KB Deep Sleep Mode */
#define SFR_SRAM0_SW_CFG_M128SD (0x1u << 2) /**< \brief (SFR_SRAM0_SW_CFG) 128 KB Shutdown Mode */
#define SFR_SRAM0_SW_CFG_M256LS (0x1u << 4) /**< \brief (SFR_SRAM0_SW_CFG) 256 KB Light Sleep Mode */
#define SFR_SRAM0_SW_CFG_M256DS (0x1u << 5) /**< \brief (SFR_SRAM0_SW_CFG) 256 KB Deep Sleep Mode */
#define SFR_SRAM0_SW_CFG_M256SD (0x1u << 6) /**< \brief (SFR_SRAM0_SW_CFG) 256 KB Shutdown Mode */
#define SFR_SRAM0_SW_CFG_M384LS (0x1u << 8) /**< \brief (SFR_SRAM0_SW_CFG) 384 KB Light Sleep Mode */
#define SFR_SRAM0_SW_CFG_M384DS (0x1u << 9) /**< \brief (SFR_SRAM0_SW_CFG) 384 KB Deep Sleep Mode */
#define SFR_SRAM0_SW_CFG_M384SD (0x1u << 10) /**< \brief (SFR_SRAM0_SW_CFG) 384 KB Shutdown Mode */
#define SFR_SRAM0_SW_CFG_M512LS (0x1u << 12) /**< \brief (SFR_SRAM0_SW_CFG) 512 KB Light Sleep Mode */
#define SFR_SRAM0_SW_CFG_M512DS (0x1u << 13) /**< \brief (SFR_SRAM0_SW_CFG) 512 KB Deep Sleep Mode */
#define SFR_SRAM0_SW_CFG_M512SD (0x1u << 14) /**< \brief (SFR_SRAM0_SW_CFG) 512 KB Shutdown Mode */
#define SFR_SRAM0_SW_CFG_CLKG_DIS (0x1u << 16) /**< \brief (SFR_SRAM0_SW_CFG) Clock Gating Disable */
/* -------- SFR_SRAM0_HW_CFG : (SFR Offset: 0x14) SRAM0 Hardware Status Register -------- */
#define SFR_SRAM0_HW_CFG_M128LS (0x1u << 0) /**< \brief (SFR_SRAM0_HW_CFG) 128 KB Light Sleep Mode */
#define SFR_SRAM0_HW_CFG_M128DS (0x1u << 1) /**< \brief (SFR_SRAM0_HW_CFG) 128 KB Deep Sleep Mode */
#define SFR_SRAM0_HW_CFG_M128SD (0x1u << 2) /**< \brief (SFR_SRAM0_HW_CFG) 128 KB Shutdown Mode */
#define SFR_SRAM0_HW_CFG_M256LS (0x1u << 4) /**< \brief (SFR_SRAM0_HW_CFG) 256 KB Light Sleep Mode */
#define SFR_SRAM0_HW_CFG_M256DS (0x1u << 5) /**< \brief (SFR_SRAM0_HW_CFG) 256 KB Deep Sleep Mode */
#define SFR_SRAM0_HW_CFG_M256SD (0x1u << 6) /**< \brief (SFR_SRAM0_HW_CFG) 256 KB Shutdown Mode */
#define SFR_SRAM0_HW_CFG_M384LS (0x1u << 8) /**< \brief (SFR_SRAM0_HW_CFG) 384 KB Light Sleep Mode */
#define SFR_SRAM0_HW_CFG_M384DS (0x1u << 9) /**< \brief (SFR_SRAM0_HW_CFG) 384 KB Deep Sleep Mode */
#define SFR_SRAM0_HW_CFG_M384SD (0x1u << 10) /**< \brief (SFR_SRAM0_HW_CFG) 384 KB Shutdown Mode */
#define SFR_SRAM0_HW_CFG_M512LS (0x1u << 12) /**< \brief (SFR_SRAM0_HW_CFG) 512 KB Light Sleep Mode */
#define SFR_SRAM0_HW_CFG_M512DS (0x1u << 13) /**< \brief (SFR_SRAM0_HW_CFG) 512 KB Deep Sleep Mode */
#define SFR_SRAM0_HW_CFG_M512SD (0x1u << 14) /**< \brief (SFR_SRAM0_HW_CFG) 512 KB Shutdown Mode */
/* -------- SFR_SRAM0_CH[4] : (SFR Offset: 0x18) SRAM0 Read Margin Register Channel -------- */
#define SFR_SRAM0_CH_RM0_Pos 0
#define SFR_SRAM0_CH_RM0_Msk (0xfu << SFR_SRAM0_CH_RM0_Pos) /**< \brief (SFR_SRAM0_CH[4]) Read Margin */
#define SFR_SRAM0_CH_RM0(value) ((SFR_SRAM0_CH_RM0_Msk & ((value) << SFR_SRAM0_CH_RM0_Pos)))
#define SFR_SRAM0_CH_RME0 (0x1u << 4) /**< \brief (SFR_SRAM0_CH[4]) Read Margin Enable */
#define SFR_SRAM0_CH_RM1_Pos 8
#define SFR_SRAM0_CH_RM1_Msk (0xfu << SFR_SRAM0_CH_RM1_Pos) /**< \brief (SFR_SRAM0_CH[4]) Read Margin */
#define SFR_SRAM0_CH_RM1(value) ((SFR_SRAM0_CH_RM1_Msk & ((value) << SFR_SRAM0_CH_RM1_Pos)))
#define SFR_SRAM0_CH_RME1 (0x1u << 12) /**< \brief (SFR_SRAM0_CH[4]) Read Margin Enable */
#define SFR_SRAM0_CH_RM2_Pos 16
#define SFR_SRAM0_CH_RM2_Msk (0xfu << SFR_SRAM0_CH_RM2_Pos) /**< \brief (SFR_SRAM0_CH[4]) Read Margin */
#define SFR_SRAM0_CH_RM2(value) ((SFR_SRAM0_CH_RM2_Msk & ((value) << SFR_SRAM0_CH_RM2_Pos)))
#define SFR_SRAM0_CH_RME2 (0x1u << 20) /**< \brief (SFR_SRAM0_CH[4]) Read Margin Enable */
#define SFR_SRAM0_CH_RM3_Pos 24
#define SFR_SRAM0_CH_RM3_Msk (0xfu << SFR_SRAM0_CH_RM3_Pos) /**< \brief (SFR_SRAM0_CH[4]) Read Margin */
#define SFR_SRAM0_CH_RM3(value) ((SFR_SRAM0_CH_RM3_Msk & ((value) << SFR_SRAM0_CH_RM3_Pos)))
#define SFR_SRAM0_CH_RME3 (0x1u << 28) /**< \brief (SFR_SRAM0_CH[4]) Read Margin Enable */
/* -------- SFR_SRAM[2] : (SFR Offset: 0x28) SRAM Configuration Register -------- */
#define SFR_SRAM_RM0_Pos 0
#define SFR_SRAM_RM0_Msk (0xfu << SFR_SRAM_RM0_Pos) /**< \brief (SFR_SRAM[2]) Read Margin */
#define SFR_SRAM_RM0(value) ((SFR_SRAM_RM0_Msk & ((value) << SFR_SRAM_RM0_Pos)))
#define SFR_SRAM_RM1_Pos 4
#define SFR_SRAM_RM1_Msk (0xfu << SFR_SRAM_RM1_Pos) /**< \brief (SFR_SRAM[2]) Read Margin */
#define SFR_SRAM_RM1(value) ((SFR_SRAM_RM1_Msk & ((value) << SFR_SRAM_RM1_Pos)))
#define SFR_SRAM_RM2_Pos 8
#define SFR_SRAM_RM2_Msk (0xfu << SFR_SRAM_RM2_Pos) /**< \brief (SFR_SRAM[2]) Read Margin */
#define SFR_SRAM_RM2(value) ((SFR_SRAM_RM2_Msk & ((value) << SFR_SRAM_RM2_Pos)))
#define SFR_SRAM_RM3_Pos 12
#define SFR_SRAM_RM3_Msk (0xfu << SFR_SRAM_RM3_Pos) /**< \brief (SFR_SRAM[2]) Read Margin */
#define SFR_SRAM_RM3(value) ((SFR_SRAM_RM3_Msk & ((value) << SFR_SRAM_RM3_Pos)))
#define SFR_SRAM_RME0 (0x1u << 16) /**< \brief (SFR_SRAM[2]) Read Margin Enable */
#define SFR_SRAM_RME1 (0x1u << 17) /**< \brief (SFR_SRAM[2]) Read Margin Enable */
#define SFR_SRAM_RME2 (0x1u << 18) /**< \brief (SFR_SRAM[2]) Read Margin Enable */
#define SFR_SRAM_RME3 (0x1u << 19) /**< \brief (SFR_SRAM[2]) Read Margin Enable */
#define SFR_SRAM_LS (0x1u << 24) /**< \brief (SFR_SRAM[2]) Light Sleep Mode */
#define SFR_SRAM_DS (0x1u << 25) /**< \brief (SFR_SRAM[2]) Deep Sleep Mode */
#define SFR_SRAM_SD (0x1u << 26) /**< \brief (SFR_SRAM[2]) Shutdown Mode */
/* -------- SFR_CPKCC : (SFR Offset: 0x30) CPKCC Memory Configuration Register -------- */
#define SFR_CPKCC_ROM_RM_Pos 0
#define SFR_CPKCC_ROM_RM_Msk (0xfu << SFR_CPKCC_ROM_RM_Pos) /**< \brief (SFR_CPKCC) ROM Read Margin */
#define SFR_CPKCC_ROM_RM(value) ((SFR_CPKCC_ROM_RM_Msk & ((value) << SFR_CPKCC_ROM_RM_Pos)))
#define SFR_CPKCC_ROM_RME (0x1u << 4) /**< \brief (SFR_CPKCC) ROM Read Margin Enable */
#define SFR_CPKCC_ROM_LS (0x1u << 8) /**< \brief (SFR_CPKCC) ROM Light Sleep Mode */
#define SFR_CPKCC_ROM_SD (0x1u << 9) /**< \brief (SFR_CPKCC) Shutdown Mode */
#define SFR_CPKCC_RAM_RM_Pos 16
#define SFR_CPKCC_RAM_RM_Msk (0xfu << SFR_CPKCC_RAM_RM_Pos) /**< \brief (SFR_CPKCC) RAM Read Margin */
#define SFR_CPKCC_RAM_RM(value) ((SFR_CPKCC_RAM_RM_Msk & ((value) << SFR_CPKCC_RAM_RM_Pos)))
#define SFR_CPKCC_RAM_RME (0x1u << 20) /**< \brief (SFR_CPKCC) RAM Read Margin Enable */
#define SFR_CPKCC_RAM_LS (0x1u << 24) /**< \brief (SFR_CPKCC) RAM Light Sleep Mode */
#define SFR_CPKCC_RAM_DS (0x1u << 25) /**< \brief (SFR_CPKCC) RAM Deep Sleep Mode */
#define SFR_CPKCC_RAM_SD (0x1u << 26) /**< \brief (SFR_CPKCC) Shutdown Mode */
/* -------- SFR_HROMC : (SFR Offset: 0x40) HROMC Memory Configuration Register -------- */
#define SFR_HROMC_RM_Pos 0
#define SFR_HROMC_RM_Msk (0xfu << SFR_HROMC_RM_Pos) /**< \brief (SFR_HROMC) Read Margin */
#define SFR_HROMC_RM(value) ((SFR_HROMC_RM_Msk & ((value) << SFR_HROMC_RM_Pos)))
#define SFR_HROMC_RME (0x1u << 4) /**< \brief (SFR_HROMC) Read Margin Enable */
#define SFR_HROMC_LS (0x1u << 8) /**< \brief (SFR_HROMC) HROMC Light Sleep Mode */
#define SFR_HROMC_SD (0x1u << 9) /**< \brief (SFR_HROMC) Shutdown Mode */
/* -------- SFR_HCACHEI_VALID : (SFR Offset: 0x50) HCACHEI Valid Memory Configuration Register -------- */
#define SFR_HCACHEI_VALID_RM_Pos 0
#define SFR_HCACHEI_VALID_RM_Msk (0xfu << SFR_HCACHEI_VALID_RM_Pos) /**< \brief (SFR_HCACHEI_VALID) Read Margin */
#define SFR_HCACHEI_VALID_RM(value) ((SFR_HCACHEI_VALID_RM_Msk & ((value) << SFR_HCACHEI_VALID_RM_Pos)))
#define SFR_HCACHEI_VALID_RME (0x1u << 4) /**< \brief (SFR_HCACHEI_VALID) Read Margin Enable */
#define SFR_HCACHEI_VALID_LS (0x1u << 8) /**< \brief (SFR_HCACHEI_VALID) Light Sleep Mode */
#define SFR_HCACHEI_VALID_DS (0x1u << 9) /**< \brief (SFR_HCACHEI_VALID) Deep Sleep Mode */
#define SFR_HCACHEI_VALID_SD (0x1u << 10) /**< \brief (SFR_HCACHEI_VALID) Shutdown Mode */
/* -------- SFR_HCACHEI_DATA : (SFR Offset: 0x54) HCACHEI Data Memory Configuration Register -------- */
#define SFR_HCACHEI_DATA_RM_Pos 0
#define SFR_HCACHEI_DATA_RM_Msk (0xffffu << SFR_HCACHEI_DATA_RM_Pos) /**< \brief (SFR_HCACHEI_DATA) Read Margin */
#define SFR_HCACHEI_DATA_RM(value) ((SFR_HCACHEI_DATA_RM_Msk & ((value) << SFR_HCACHEI_DATA_RM_Pos)))
#define SFR_HCACHEI_DATA_RME_Pos 16
#define SFR_HCACHEI_DATA_RME_Msk (0xfu << SFR_HCACHEI_DATA_RME_Pos) /**< \brief (SFR_HCACHEI_DATA) Read Margin Enable */
#define SFR_HCACHEI_DATA_RME(value) ((SFR_HCACHEI_DATA_RME_Msk & ((value) << SFR_HCACHEI_DATA_RME_Pos)))
#define SFR_HCACHEI_DATA_LS_Pos 20
#define SFR_HCACHEI_DATA_LS_Msk (0xfu << SFR_HCACHEI_DATA_LS_Pos) /**< \brief (SFR_HCACHEI_DATA) Light Sleep Mode */
#define SFR_HCACHEI_DATA_LS(value) ((SFR_HCACHEI_DATA_LS_Msk & ((value) << SFR_HCACHEI_DATA_LS_Pos)))
#define SFR_HCACHEI_DATA_DS_Pos 24
#define SFR_HCACHEI_DATA_DS_Msk (0xfu << SFR_HCACHEI_DATA_DS_Pos) /**< \brief (SFR_HCACHEI_DATA) Deep Sleep Mode */
#define SFR_HCACHEI_DATA_DS(value) ((SFR_HCACHEI_DATA_DS_Msk & ((value) << SFR_HCACHEI_DATA_DS_Pos)))
#define SFR_HCACHEI_DATA_SD_Pos 28
#define SFR_HCACHEI_DATA_SD_Msk (0xfu << SFR_HCACHEI_DATA_SD_Pos) /**< \brief (SFR_HCACHEI_DATA) Shutdown Mode */
#define SFR_HCACHEI_DATA_SD(value) ((SFR_HCACHEI_DATA_SD_Msk & ((value) << SFR_HCACHEI_DATA_SD_Pos)))
/* -------- SFR_HCACHEI_TAG : (SFR Offset: 0x58) HCACHEI Tag Memory Configuration Register -------- */
#define SFR_HCACHEI_TAG_RM_Pos 0
#define SFR_HCACHEI_TAG_RM_Msk (0xffffu << SFR_HCACHEI_TAG_RM_Pos) /**< \brief (SFR_HCACHEI_TAG) Read Margin */
#define SFR_HCACHEI_TAG_RM(value) ((SFR_HCACHEI_TAG_RM_Msk & ((value) << SFR_HCACHEI_TAG_RM_Pos)))
#define SFR_HCACHEI_TAG_RME_Pos 16
#define SFR_HCACHEI_TAG_RME_Msk (0xfu << SFR_HCACHEI_TAG_RME_Pos) /**< \brief (SFR_HCACHEI_TAG) Read Margin Enable */
#define SFR_HCACHEI_TAG_RME(value) ((SFR_HCACHEI_TAG_RME_Msk & ((value) << SFR_HCACHEI_TAG_RME_Pos)))
#define SFR_HCACHEI_TAG_LS_Pos 20
#define SFR_HCACHEI_TAG_LS_Msk (0xfu << SFR_HCACHEI_TAG_LS_Pos) /**< \brief (SFR_HCACHEI_TAG) Light Sleep Mode */
#define SFR_HCACHEI_TAG_LS(value) ((SFR_HCACHEI_TAG_LS_Msk & ((value) << SFR_HCACHEI_TAG_LS_Pos)))
#define SFR_HCACHEI_TAG_DS_Pos 24
#define SFR_HCACHEI_TAG_DS_Msk (0xfu << SFR_HCACHEI_TAG_DS_Pos) /**< \brief (SFR_HCACHEI_TAG) Deep Sleep Mode */
#define SFR_HCACHEI_TAG_DS(value) ((SFR_HCACHEI_TAG_DS_Msk & ((value) << SFR_HCACHEI_TAG_DS_Pos)))
#define SFR_HCACHEI_TAG_SD_Pos 28
#define SFR_HCACHEI_TAG_SD_Msk (0xfu << SFR_HCACHEI_TAG_SD_Pos) /**< \brief (SFR_HCACHEI_TAG) Shutdown Mode */
#define SFR_HCACHEI_TAG_SD(value) ((SFR_HCACHEI_TAG_SD_Msk & ((value) << SFR_HCACHEI_TAG_SD_Pos)))
/* -------- SFR_HCACHED_VALID : (SFR Offset: 0x60) HCACHED Valid Memory Configuration Register -------- */
#define SFR_HCACHED_VALID_RM_Pos 0
#define SFR_HCACHED_VALID_RM_Msk (0xfu << SFR_HCACHED_VALID_RM_Pos) /**< \brief (SFR_HCACHED_VALID) Read Margin */
#define SFR_HCACHED_VALID_RM(value) ((SFR_HCACHED_VALID_RM_Msk & ((value) << SFR_HCACHED_VALID_RM_Pos)))
#define SFR_HCACHED_VALID_RME (0x1u << 4) /**< \brief (SFR_HCACHED_VALID) Read Margin Enable */
#define SFR_HCACHED_VALID_LS (0x1u << 8) /**< \brief (SFR_HCACHED_VALID) Light Sleep Mode */
#define SFR_HCACHED_VALID_DS (0x1u << 9) /**< \brief (SFR_HCACHED_VALID) Deep Sleep Mode */
#define SFR_HCACHED_VALID_SD (0x1u << 10) /**< \brief (SFR_HCACHED_VALID) Shutdown Mode */
/* -------- SFR_HCACHED_DATA : (SFR Offset: 0x64) HCACHED Data Memory Configuration Register -------- */
#define SFR_HCACHED_DATA_RM_Pos 0
#define SFR_HCACHED_DATA_RM_Msk (0xffffu << SFR_HCACHED_DATA_RM_Pos) /**< \brief (SFR_HCACHED_DATA) Read Margin */
#define SFR_HCACHED_DATA_RM(value) ((SFR_HCACHED_DATA_RM_Msk & ((value) << SFR_HCACHED_DATA_RM_Pos)))
#define SFR_HCACHED_DATA_RME_Pos 16
#define SFR_HCACHED_DATA_RME_Msk (0xfu << SFR_HCACHED_DATA_RME_Pos) /**< \brief (SFR_HCACHED_DATA) Read Margin Enable */
#define SFR_HCACHED_DATA_RME(value) ((SFR_HCACHED_DATA_RME_Msk & ((value) << SFR_HCACHED_DATA_RME_Pos)))
#define SFR_HCACHED_DATA_LS_Pos 20
#define SFR_HCACHED_DATA_LS_Msk (0xfu << SFR_HCACHED_DATA_LS_Pos) /**< \brief (SFR_HCACHED_DATA) Light Sleep Mode */
#define SFR_HCACHED_DATA_LS(value) ((SFR_HCACHED_DATA_LS_Msk & ((value) << SFR_HCACHED_DATA_LS_Pos)))
#define SFR_HCACHED_DATA_DS_Pos 24
#define SFR_HCACHED_DATA_DS_Msk (0xfu << SFR_HCACHED_DATA_DS_Pos) /**< \brief (SFR_HCACHED_DATA) Deep Sleep Mode */
#define SFR_HCACHED_DATA_DS(value) ((SFR_HCACHED_DATA_DS_Msk & ((value) << SFR_HCACHED_DATA_DS_Pos)))
#define SFR_HCACHED_DATA_SD_Pos 28
#define SFR_HCACHED_DATA_SD_Msk (0xfu << SFR_HCACHED_DATA_SD_Pos) /**< \brief (SFR_HCACHED_DATA) Shutdown Mode */
#define SFR_HCACHED_DATA_SD(value) ((SFR_HCACHED_DATA_SD_Msk & ((value) << SFR_HCACHED_DATA_SD_Pos)))
/* -------- SFR_HCACHED_TAG : (SFR Offset: 0x68) HCACHEDTag Memory Configuration Register -------- */
#define SFR_HCACHED_TAG_RM_Pos 0
#define SFR_HCACHED_TAG_RM_Msk (0xffffu << SFR_HCACHED_TAG_RM_Pos) /**< \brief (SFR_HCACHED_TAG) Read Margin */
#define SFR_HCACHED_TAG_RM(value) ((SFR_HCACHED_TAG_RM_Msk & ((value) << SFR_HCACHED_TAG_RM_Pos)))
#define SFR_HCACHED_TAG_RME_Pos 16
#define SFR_HCACHED_TAG_RME_Msk (0xfu << SFR_HCACHED_TAG_RME_Pos) /**< \brief (SFR_HCACHED_TAG) Read Margin Enable */
#define SFR_HCACHED_TAG_RME(value) ((SFR_HCACHED_TAG_RME_Msk & ((value) << SFR_HCACHED_TAG_RME_Pos)))
#define SFR_HCACHED_TAG_LS_Pos 20
#define SFR_HCACHED_TAG_LS_Msk (0xfu << SFR_HCACHED_TAG_LS_Pos) /**< \brief (SFR_HCACHED_TAG) Light Sleep Mode */
#define SFR_HCACHED_TAG_LS(value) ((SFR_HCACHED_TAG_LS_Msk & ((value) << SFR_HCACHED_TAG_LS_Pos)))
#define SFR_HCACHED_TAG_DS_Pos 24
#define SFR_HCACHED_TAG_DS_Msk (0xfu << SFR_HCACHED_TAG_DS_Pos) /**< \brief (SFR_HCACHED_TAG) Deep Sleep Mode */
#define SFR_HCACHED_TAG_DS(value) ((SFR_HCACHED_TAG_DS_Msk & ((value) << SFR_HCACHED_TAG_DS_Pos)))
#define SFR_HCACHED_TAG_SD_Pos 28
#define SFR_HCACHED_TAG_SD_Msk (0xfu << SFR_HCACHED_TAG_SD_Pos) /**< \brief (SFR_HCACHED_TAG) Shutdown Mode */
#define SFR_HCACHED_TAG_SD(value) ((SFR_HCACHED_TAG_SD_Msk & ((value) << SFR_HCACHED_TAG_SD_Pos)))
/* -------- SFR_FLASH : (SFR Offset: 0x70) Flash Memory Configuration Register -------- */
#define SFR_FLASH_PATCH_BYPASS (0x1u << 0) /**< \brief (SFR_FLASH)  */
/* -------- SFR_OPT_LINK : (SFR Offset: 0x80) Optical Link Register -------- */
#define SFR_OPT_LINK_CLK_SELECT (0x1u << 0) /**< \brief (SFR_OPT_LINK) Clock Selection */
/* -------- SFR_JTAG : (SFR Offset: 0x90) JTAG Register -------- */
#define SFR_JTAG_JTAG_LOCK (0x1u << 0) /**< \brief (SFR_JTAG) JTAG Lock */
/* -------- SFR_CORE_DEBUG_CFG : (SFR Offset: 0xA0) Core Debug Configuration Register -------- */
#define SFR_CORE_DEBUG_CFG_SWV (0x1u << 0) /**< \brief (SFR_CORE_DEBUG_CFG) SWV Selection */
#define SFR_CORE_DEBUG_CFG_XTRG1 (0x1u << 1) /**< \brief (SFR_CORE_DEBUG_CFG) From Core 1 to Core 0 Cross Triggering */
#define SFR_CORE_DEBUG_CFG_XTRG0 (0x1u << 2) /**< \brief (SFR_CORE_DEBUG_CFG) From Core 0 to Core 1 Cross Triggering */
/* -------- SFR_EMAHB2AHB : (SFR Offset: 0xA4) AHB2AHB Configuration Register -------- */
#define SFR_EMAHB2AHB_PFETCH8_0_1 (0x1u << 0) /**< \brief (SFR_EMAHB2AHB) AHB MASTER0_1 Converter Prefetch */
#define   SFR_EMAHB2AHB_PFETCH8_0_1_INCR4 (0x0u << 0) /**< \brief (SFR_EMAHB2AHB) INCR undefined burst converted to burst of 4 data. */
#define   SFR_EMAHB2AHB_PFETCH8_0_1_INCR8 (0x1u << 0) /**< \brief (SFR_EMAHB2AHB) INCR undefined burst converted to burst of 8 data. */
#define SFR_EMAHB2AHB_PFETCH8_1_0 (0x1u << 1) /**< \brief (SFR_EMAHB2AHB) AHB MASTER1_0 Converter Prefetch */
#define   SFR_EMAHB2AHB_PFETCH8_1_0_INCR4 (0x0u << 1) /**< \brief (SFR_EMAHB2AHB) INCR undefined burst converted to burst of 4 data. */
#define   SFR_EMAHB2AHB_PFETCH8_1_0_INCR8 (0x1u << 1) /**< \brief (SFR_EMAHB2AHB) INCR undefined burst converted to burst of 8 data. */
/* -------- SFR_SECURE : (SFR Offset: 0xA8) Secure Register -------- */
#define SFR_SECURE_ROM_ENA (0x1u << 0) /**< \brief (SFR_SECURE) ROM Access Enable */
/* -------- SFR_SECURE_BIT : (SFR Offset: 0xAC) Secure Bit Register -------- */
#define SFR_SECURE_BIT_MODE_DIS (0x1u << 0) /**< \brief (SFR_SECURE_BIT) Secure Mode */
/* -------- SFR_ERASE_FLASH : (SFR Offset: 0xB0) Erase Flash Register -------- */
#define SFR_ERASE_FLASH_MODE (0x1u << 0) /**< \brief (SFR_ERASE_FLASH) Erase Flash Content */
#define SFR_ERASE_FLASH_SRAM0 (0x1u << 1) /**< \brief (SFR_ERASE_FLASH) Erase SRAMx Content */
#define SFR_ERASE_FLASH_SRAM1 (0x1u << 2) /**< \brief (SFR_ERASE_FLASH) Erase SRAMx Content */
#define SFR_ERASE_FLASH_SRAM2 (0x1u << 3) /**< \brief (SFR_ERASE_FLASH) Erase SRAMx Content */
/* -------- SFR_PWM_DEBUG : (SFR Offset: 0xB4) PWM Debug Register -------- */
#define SFR_PWM_DEBUG_CORE0 (0x1u << 0) /**< \brief (SFR_PWM_DEBUG) Enable PWM Debug on Core 0 */
#define SFR_PWM_DEBUG_CORE1 (0x1u << 1) /**< \brief (SFR_PWM_DEBUG) Enable PWM Debug on Core 1 */
/* -------- SFR_FFPI : (SFR Offset: 0xB8) FFPI Register -------- */
#define SFR_FFPI_MODE (0x1u << 0) /**< \brief (SFR_FFPI) FFPI Status */
/* -------- SFR_WAIT_MODE : (SFR Offset: 0xBC) Improved Wait Mode Register -------- */
#define SFR_WAIT_MODE_STATUS (0x1u << 0) /**< \brief (SFR_WAIT_MODE) Improved Wait Mode Status */
/* -------- SFR_ROM_CODE : (SFR Offset: 0xC0) ROM Code Register -------- */
#define SFR_ROM_CODE_PLLA_STATUS_Pos 0
#define SFR_ROM_CODE_PLLA_STATUS_Msk (0x3u << SFR_ROM_CODE_PLLA_STATUS_Pos) /**< \brief (SFR_ROM_CODE) PLLA Status */
#define SFR_ROM_CODE_FORCE_BYPASS (0x1u << 2) /**< \brief (SFR_ROM_CODE)  */
#define SFR_ROM_CODE_FORCE_BYPASS_VALUE (0x1u << 3) /**< \brief (SFR_ROM_CODE)  */
/* -------- SFR_WPMR : (SFR Offset: 0xE4) Write Protection Mode Register -------- */
#define SFR_WPMR_WPEN (0x1u << 0) /**< \brief (SFR_WPMR) Write Protection Enable */
#define SFR_WPMR_WPKEY_Pos 8
#define SFR_WPMR_WPKEY_Msk (0xffffffu << SFR_WPMR_WPKEY_Pos) /**< \brief (SFR_WPMR) Write Protection Key */
#define SFR_WPMR_WPKEY(value) ((SFR_WPMR_WPKEY_Msk & ((value) << SFR_WPMR_WPKEY_Pos)))
#define   SFR_WPMR_WPKEY_PASSWD (0x534652u << 8) /**< \brief (SFR_WPMR) Writing any other value in this field aborts the write operation of the WPEN bit. Always reads as 0. */
/* -------- SFR_WPSR : (SFR Offset: 0xE8) Write Protection Status Register -------- */
#define SFR_WPSR_WPVS (0x1u << 0) /**< \brief (SFR_WPSR) Write Protection Violation Status */
#define SFR_WPSR_WPSRC_Pos 8
#define SFR_WPSR_WPSRC_Msk (0xffffu << SFR_WPSR_WPSRC_Pos) /**< \brief (SFR_WPSR) Write Protection Source */
#define SFR_WPSR_WPSRC(value) ((SFR_WPSR_WPSRC_Msk & ((value) << SFR_WPSR_WPSRC_Pos)))
/* -------- SFR_SPARE : (SFR Offset: 0xF0) Spare Register -------- */
#define SFR_SPARE_VALUE_Pos 0
#define SFR_SPARE_VALUE_Msk (0xffffffffu << SFR_SPARE_VALUE_Pos) /**< \brief (SFR_SPARE) Spare */
#define SFR_SPARE_VALUE(value) ((SFR_SPARE_VALUE_Msk & ((value) << SFR_SPARE_VALUE_Pos)))
/* -------- SFR_VERSION : (SFR Offset: 0xFC) Version Register -------- */
#define SFR_VERSION_VERSION_Pos 0
#define SFR_VERSION_VERSION_Msk (0xfffu << SFR_VERSION_VERSION_Pos) /**< \brief (SFR_VERSION) Version of the Hardware Module */
#define SFR_VERSION_MFN_Pos 16
#define SFR_VERSION_MFN_Msk (0x7u << SFR_VERSION_MFN_Pos) /**< \brief (SFR_VERSION) Metal Fix Number */

/*@}*/


#endif /* _PIC32CX_SFR_COMPONENT_ */
