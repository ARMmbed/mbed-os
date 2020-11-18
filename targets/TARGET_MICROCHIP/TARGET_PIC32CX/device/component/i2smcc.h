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

#ifndef _PIC32CX_I2SMCC_COMPONENT_
#define _PIC32CX_I2SMCC_COMPONENT_

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR Inter-IC Sound Multi Channel Controller */
/* ============================================================================= */
/** \addtogroup PIC32CX_I2SMCC Inter-IC Sound Multi Channel Controller */
/*@{*/

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
/** \brief I2smcc hardware registers */
typedef struct {
  __O  uint32_t I2SMCC_CR;     /**< \brief (I2smcc Offset: 0x0) Control Register */
  __IO uint32_t I2SMCC_MRA;    /**< \brief (I2smcc Offset: 0x4) Mode Register A */
  __IO uint32_t I2SMCC_MRB;    /**< \brief (I2smcc Offset: 0x8) Mode Register B */
  __I  uint32_t I2SMCC_SR;     /**< \brief (I2smcc Offset: 0xC) Status Register */
  __O  uint32_t I2SMCC_IERA;   /**< \brief (I2smcc Offset: 0x10) Interrupt Enable Register A */
  __O  uint32_t I2SMCC_IDRA;   /**< \brief (I2smcc Offset: 0x14) Interrupt Disable Register A */
  __I  uint32_t I2SMCC_IMRA;   /**< \brief (I2smcc Offset: 0x18) Interrupt Mask Register A */
  __I  uint32_t I2SMCC_ISRA;   /**< \brief (I2smcc Offset: 0x1C) Interrupt Status Register A */
  __O  uint32_t I2SMCC_IERB;   /**< \brief (I2smcc Offset: 0x20) Interrupt Enable Register B */
  __O  uint32_t I2SMCC_IDRB;   /**< \brief (I2smcc Offset: 0x24) Interrupt Disable Register B */
  __I  uint32_t I2SMCC_IMRB;   /**< \brief (I2smcc Offset: 0x28) Interrupt Mask Register B */
  __I  uint32_t I2SMCC_ISRB;   /**< \brief (I2smcc Offset: 0x2C) Interrupt Status Register B */
  __I  uint32_t I2SMCC_RHR;    /**< \brief (I2smcc Offset: 0x30) Receiver Holding Register */
  __O  uint32_t I2SMCC_THR;    /**< \brief (I2smcc Offset: 0x34) Transmitter Holding Register */
  __I  uint32_t Reserved1[2];
  __I  uint32_t I2SMCC_RHL0R;  /**< \brief (I2smcc Offset: 0x40) Receiver Holding Left 0 Register */
  __I  uint32_t I2SMCC_RHR0R;  /**< \brief (I2smcc Offset: 0x44) Receiver Holding Right 0 Register */
  __I  uint32_t I2SMCC_RHL1R;  /**< \brief (I2smcc Offset: 0x48) Receiver Holding Left 1 Register */
  __I  uint32_t I2SMCC_RHR1R;  /**< \brief (I2smcc Offset: 0x4C) Receiver Holding Right 1 Register */
  __I  uint32_t I2SMCC_RHL2R;  /**< \brief (I2smcc Offset: 0x50) Receiver Holding Left 2 Register */
  __I  uint32_t I2SMCC_RHR2R;  /**< \brief (I2smcc Offset: 0x54) Receiver Holding Right 2 Register */
  __I  uint32_t I2SMCC_RHL3R;  /**< \brief (I2smcc Offset: 0x58) Receiver Holding Left 3 Register */
  __I  uint32_t I2SMCC_RHR3R;  /**< \brief (I2smcc Offset: 0x5C) Receiver Holding Right 3 Register */
  __O  uint32_t I2SMCC_THL0R;  /**< \brief (I2smcc Offset: 0x60) Transmitter Holding Left 0 Register */
  __O  uint32_t I2SMCC_THR0R;  /**< \brief (I2smcc Offset: 0x64) Transmitter Holding Right 0 Register */
  __O  uint32_t I2SMCC_THL1R;  /**< \brief (I2smcc Offset: 0x68) Transmitter Holding Left 1 Register */
  __O  uint32_t I2SMCC_THR1R;  /**< \brief (I2smcc Offset: 0x6C) Transmitter Holding Right 1 Register */
  __O  uint32_t I2SMCC_THL2R;  /**< \brief (I2smcc Offset: 0x70) Transmitter Holding Left 2 Register */
  __O  uint32_t I2SMCC_THR2R;  /**< \brief (I2smcc Offset: 0x74) Transmitter Holding Right 2 Register */
  __O  uint32_t I2SMCC_THL3R;  /**< \brief (I2smcc Offset: 0x78) Transmitter Holding Left 3 Register */
  __O  uint32_t I2SMCC_THR3R;  /**< \brief (I2smcc Offset: 0x7C) Transmitter Holding Right 3 Register */
  __I  uint32_t Reserved2[25];
  __IO uint32_t I2SMCC_WPMR;   /**< \brief (I2smcc Offset: 0xE4) Write Protection Mode Register */
  __I  uint32_t I2SMCC_WPSR;   /**< \brief (I2smcc Offset: 0xE8) Write Protection Status Register */
  __I  uint32_t Reserved3[5];
  __IO uint32_t I2SMCC_RPR;    /**< \brief (I2smcc Offset: 0x100) Receive Pointer Register */
  __IO uint32_t I2SMCC_RCR;    /**< \brief (I2smcc Offset: 0x104) Receive Counter Register */
  __IO uint32_t I2SMCC_TPR;    /**< \brief (I2smcc Offset: 0x108) Transmit Pointer Register */
  __IO uint32_t I2SMCC_TCR;    /**< \brief (I2smcc Offset: 0x10C) Transmit Counter Register */
  __IO uint32_t I2SMCC_RNPR;   /**< \brief (I2smcc Offset: 0x110) Receive Next Pointer Register */
  __IO uint32_t I2SMCC_RNCR;   /**< \brief (I2smcc Offset: 0x114) Receive Next Counter Register */
  __IO uint32_t I2SMCC_TNPR;   /**< \brief (I2smcc Offset: 0x118) Transmit Next Pointer Register */
  __IO uint32_t I2SMCC_TNCR;   /**< \brief (I2smcc Offset: 0x11C) Transmit Next Counter Register */
  __O  uint32_t I2SMCC_PTCR;   /**< \brief (I2smcc Offset: 0x120) Transfer Control Register */
  __I  uint32_t I2SMCC_PTSR;   /**< \brief (I2smcc Offset: 0x124) Transfer Status Register */
  __IO uint32_t I2SMCC_PWPMR;  /**< \brief (I2smcc Offset: 0x128) Write Protection Mode Register */
} I2smcc;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/* -------- I2SMCC_CR : (I2SMCC Offset: 0x0) Control Register -------- */
#define I2SMCC_CR_RXEN (0x1u << 0) /**< \brief (I2SMCC_CR) Receiver Enable */
#define I2SMCC_CR_RXDIS (0x1u << 1) /**< \brief (I2SMCC_CR) Receiver Disable */
#define I2SMCC_CR_CKEN (0x1u << 2) /**< \brief (I2SMCC_CR) Clocks Enable */
#define I2SMCC_CR_CKDIS (0x1u << 3) /**< \brief (I2SMCC_CR) Clocks Disable */
#define I2SMCC_CR_TXEN (0x1u << 4) /**< \brief (I2SMCC_CR) Transmitter Enable */
#define I2SMCC_CR_TXDIS (0x1u << 5) /**< \brief (I2SMCC_CR) Transmitter Disable */
#define I2SMCC_CR_SWRST (0x1u << 7) /**< \brief (I2SMCC_CR) Software Reset */
/* -------- I2SMCC_MRA : (I2SMCC Offset: 0x4) Mode Register A -------- */
#define I2SMCC_MRA_MODE (0x1u << 0) /**< \brief (I2SMCC_MRA) Inter-IC Sound Multi Channel Controller Mode */
#define   I2SMCC_MRA_MODE_SLAVE (0x0u << 0) /**< \brief (I2SMCC_MRA) I2SMCC_CK and I2SMCC_WS pin inputs used as bit clock and word select/frame synchronization. */
#define   I2SMCC_MRA_MODE_MASTER (0x1u << 0) /**< \brief (I2SMCC_MRA) Bit clock and word select/frame synchronization generated by I2SMCC from MCK and output to I2SMCC_CK and I2SMCC_WS pins. MCK is output as master clock on I2SMCC_MCK if I2SMCC_MRA.IMCKMODE is set. */
#define I2SMCC_MRA_DATALENGTH_Pos 1
#define I2SMCC_MRA_DATALENGTH_Msk (0x7u << I2SMCC_MRA_DATALENGTH_Pos) /**< \brief (I2SMCC_MRA) Data Word Length */
#define I2SMCC_MRA_DATALENGTH(value) ((I2SMCC_MRA_DATALENGTH_Msk & ((value) << I2SMCC_MRA_DATALENGTH_Pos)))
#define   I2SMCC_MRA_DATALENGTH_32_BITS (0x0u << 1) /**< \brief (I2SMCC_MRA) Data length is set to 32 bits. */
#define   I2SMCC_MRA_DATALENGTH_24_BITS (0x1u << 1) /**< \brief (I2SMCC_MRA) Data length is set to 24 bits. */
#define   I2SMCC_MRA_DATALENGTH_20_BITS (0x2u << 1) /**< \brief (I2SMCC_MRA) Data length is set to 20 bits. */
#define   I2SMCC_MRA_DATALENGTH_18_BITS (0x3u << 1) /**< \brief (I2SMCC_MRA) Data length is set to 18 bits. */
#define   I2SMCC_MRA_DATALENGTH_16_BITS (0x4u << 1) /**< \brief (I2SMCC_MRA) Data length is set to 16 bits. */
#define   I2SMCC_MRA_DATALENGTH_16_BITS_COMPACT (0x5u << 1) /**< \brief (I2SMCC_MRA) Data length is set to 16-bit compact stereo. Left sample in bits [15:0] and right sample in bits [31:16] of same word. */
#define   I2SMCC_MRA_DATALENGTH_8_BITS (0x6u << 1) /**< \brief (I2SMCC_MRA) Data length is set to 8 bits. */
#define   I2SMCC_MRA_DATALENGTH_8_BITS_COMPACT (0x7u << 1) /**< \brief (I2SMCC_MRA) Data length is set to 8-bit compact stereo. Left sample in bits [7:0] and right sample in bits [15:8] of the same word. */
#define I2SMCC_MRA_WIRECFG_Pos 4
#define I2SMCC_MRA_WIRECFG_Msk (0x3u << I2SMCC_MRA_WIRECFG_Pos) /**< \brief (I2SMCC_MRA) Wire Configuration */
#define I2SMCC_MRA_WIRECFG(value) ((I2SMCC_MRA_WIRECFG_Msk & ((value) << I2SMCC_MRA_WIRECFG_Pos)))
#define   I2SMCC_MRA_WIRECFG_I2S_1_TDM_0 (0x0u << 4) /**< \brief (I2SMCC_MRA) In I2S and LJ formats, I2SMCC_DIN0 and I2SMCC_DOUT0 are used to transmit and receive I2S frames. In TDM and TDMLJ formats, I2SMCC_DIN0 and I2SMCC_DOUT0 are used to transmit and receive TDM frames. */
#define   I2SMCC_MRA_WIRECFG_I2S_2_TDM_1 (0x1u << 4) /**< \brief (I2SMCC_MRA) In I2S and LJ formats, I2SMCC_DIN1..0 and I2SMCC_DOUT1..0 are used to transmit and receive 2 I2S frames. In TDM and TDMLJ formats, I2SMCC_DIN1 and I2SMCC_DOUT1 are used to transmit and receive TDM frames. */
#define   I2SMCC_MRA_WIRECFG_I2S_4_TDM_2 (0x2u << 4) /**< \brief (I2SMCC_MRA) In I2S and LJ formats, I2SMCC_DIN3..0 and I2SMCC_DOUT3..0 are used to transmit and receive 4 I2S frames. In TDM and TDMLJ formats, I2SMCC_DIN2 and I2SMCC_DOUT2 are used to transmit and receive TDM frames. */
#define   I2SMCC_MRA_WIRECFG_TDM_3 (0x3u << 4) /**< \brief (I2SMCC_MRA) In I2S and LJ formats, reserved for future use, do not use. In TDM and TDMLJ formats, I2SMCC_DIN3 and I2SMCC_DOUT3 are used to transmit and receive TDM frames. */
#define I2SMCC_MRA_FORMAT_Pos 6
#define I2SMCC_MRA_FORMAT_Msk (0x3u << I2SMCC_MRA_FORMAT_Pos) /**< \brief (I2SMCC_MRA) Data Format */
#define I2SMCC_MRA_FORMAT(value) ((I2SMCC_MRA_FORMAT_Msk & ((value) << I2SMCC_MRA_FORMAT_Pos)))
#define   I2SMCC_MRA_FORMAT_I2S (0x0u << 6) /**< \brief (I2SMCC_MRA) I2S format, stereo with I2SMCC_WS low for left channel, and MSB of sample starting one I2SMCC_CK period after I2SMCC_WS edge. */
#define   I2SMCC_MRA_FORMAT_LJ (0x1u << 6) /**< \brief (I2SMCC_MRA) Left-justified format, stereo with I2SMCC_WS high for left channel, and MSB of sample starting on I2SMCC_WS edge. */
#define   I2SMCC_MRA_FORMAT_TDM (0x2u << 6) /**< \brief (I2SMCC_MRA) TDM format, with (NBCHAN + 1) channels, I2SMCC_WS high at beginning of first channel, and MSB of sample starting one I2SMCC_CK period after I2SMCC_WS edge. */
#define   I2SMCC_MRA_FORMAT_TDMLJ (0x3u << 6) /**< \brief (I2SMCC_MRA) TDM format, left-justified, with (NBCHAN + 1) channels, I2SMCC_WS high at beginning of first channel, and MSB of sample starting on I2SMCC_WS edge. */
#define I2SMCC_MRA_RXMONO (0x1u << 8) /**< \brief (I2SMCC_MRA) Receive Mono */
#define I2SMCC_MRA_RXLOOP (0x1u << 9) /**< \brief (I2SMCC_MRA) Loop-back Test Mode */
#define I2SMCC_MRA_TXMONO (0x1u << 10) /**< \brief (I2SMCC_MRA) Transmit Mono */
#define I2SMCC_MRA_TXSAME (0x1u << 11) /**< \brief (I2SMCC_MRA) Transmit Data when Underrun */
#define I2SMCC_MRA_SRCCLK (0x1u << 12) /**< \brief (I2SMCC_MRA) Source Clock Selection */
#define I2SMCC_MRA_NBCHAN_Pos 13
#define I2SMCC_MRA_NBCHAN_Msk (0x7u << I2SMCC_MRA_NBCHAN_Pos) /**< \brief (I2SMCC_MRA) Number of TDM Channels-1 */
#define I2SMCC_MRA_NBCHAN(value) ((I2SMCC_MRA_NBCHAN_Msk & ((value) << I2SMCC_MRA_NBCHAN_Pos)))
#define I2SMCC_MRA_IMCKDIV_Pos 16
#define I2SMCC_MRA_IMCKDIV_Msk (0x3fu << I2SMCC_MRA_IMCKDIV_Pos) /**< \brief (I2SMCC_MRA) Selected Clock to I2SMCC Master Clock Ratio */
#define I2SMCC_MRA_IMCKDIV(value) ((I2SMCC_MRA_IMCKDIV_Msk & ((value) << I2SMCC_MRA_IMCKDIV_Pos)))
#define I2SMCC_MRA_TDMFS_Pos 22
#define I2SMCC_MRA_TDMFS_Msk (0x3u << I2SMCC_MRA_TDMFS_Pos) /**< \brief (I2SMCC_MRA) TDM Frame Synchronization */
#define I2SMCC_MRA_TDMFS(value) ((I2SMCC_MRA_TDMFS_Msk & ((value) << I2SMCC_MRA_TDMFS_Pos)))
#define   I2SMCC_MRA_TDMFS_SLOT (0x0u << 22) /**< \brief (I2SMCC_MRA) I2SMCC_WS pulse is high for one time slot at beginning of frame. */
#define   I2SMCC_MRA_TDMFS_HALF (0x1u << 22) /**< \brief (I2SMCC_MRA) I2SMCC_WS pulse is high for half the time slots at beginning of frame. */
#define   I2SMCC_MRA_TDMFS_BIT (0x2u << 22) /**< \brief (I2SMCC_MRA) I2SMCC_WS pulse is high for one bit period at beginning of frame, i.e., one ISCK period. */
#define I2SMCC_MRA_ISCKDIV_Pos 24
#define I2SMCC_MRA_ISCKDIV_Msk (0x3fu << I2SMCC_MRA_ISCKDIV_Pos) /**< \brief (I2SMCC_MRA) Selected Clock to I2SMCC Serial Clock Ratio */
#define I2SMCC_MRA_ISCKDIV(value) ((I2SMCC_MRA_ISCKDIV_Msk & ((value) << I2SMCC_MRA_ISCKDIV_Pos)))
#define I2SMCC_MRA_IMCKMODE (0x1u << 30) /**< \brief (I2SMCC_MRA) Master Clock Mode */
#define I2SMCC_MRA_IWS (0x1u << 31) /**< \brief (I2SMCC_MRA) I2SMCC_WS Slot Length */
/* -------- I2SMCC_MRB : (I2SMCC Offset: 0x8) Mode Register B -------- */
#define I2SMCC_MRB_CRAMODE (0x1u << 0) /**< \brief (I2SMCC_MRB) Common Register Access Mode */
#define   I2SMCC_MRB_CRAMODE_LEFT_FIRST (0x0u << 0) /**< \brief (I2SMCC_MRB) All enabled I2S left channels are filled first, then I2S right channels. */
#define   I2SMCC_MRB_CRAMODE_REGULAR (0x1u << 0) /**< \brief (I2SMCC_MRB) An enabled I2S left channel is filled, then the corresponding right channel, until all channels are filled. */
#define I2SMCC_MRB_CLKSEL (0x1u << 16) /**< \brief (I2SMCC_MRB) Serial Clock Selection */
/* -------- I2SMCC_SR : (I2SMCC Offset: 0xC) Status Register -------- */
#define I2SMCC_SR_RXEN (0x1u << 0) /**< \brief (I2SMCC_SR) Receiver Enabled */
#define I2SMCC_SR_TXEN (0x1u << 4) /**< \brief (I2SMCC_SR) Transmitter Enabled */
/* -------- I2SMCC_IERA : (I2SMCC Offset: 0x10) Interrupt Enable Register A -------- */
#define I2SMCC_IERA_TXLRDY0 (0x1u << 0) /**< \brief (I2SMCC_IERA) I2S Transmit Left 0 or TDM Channel 2x ReadyInterrupt Enable */
#define I2SMCC_IERA_TXRRDY0 (0x1u << 1) /**< \brief (I2SMCC_IERA) I2S Transmit Right 0 or TDM Channel [2x]+1 Ready Interrupt Enable */
#define I2SMCC_IERA_TXLRDY1 (0x1u << 2) /**< \brief (I2SMCC_IERA) I2S Transmit Left 1 or TDM Channel 2x ReadyInterrupt Enable */
#define I2SMCC_IERA_TXRRDY1 (0x1u << 3) /**< \brief (I2SMCC_IERA) I2S Transmit Right 1 or TDM Channel [2x]+1 Ready Interrupt Enable */
#define I2SMCC_IERA_TXLRDY2 (0x1u << 4) /**< \brief (I2SMCC_IERA) I2S Transmit Left 2 or TDM Channel 2x ReadyInterrupt Enable */
#define I2SMCC_IERA_TXRRDY2 (0x1u << 5) /**< \brief (I2SMCC_IERA) I2S Transmit Right 2 or TDM Channel [2x]+1 Ready Interrupt Enable */
#define I2SMCC_IERA_TXLRDY3 (0x1u << 6) /**< \brief (I2SMCC_IERA) I2S Transmit Left 3 or TDM Channel 2x ReadyInterrupt Enable */
#define I2SMCC_IERA_TXRRDY3 (0x1u << 7) /**< \brief (I2SMCC_IERA) I2S Transmit Right 3 or TDM Channel [2x]+1 Ready Interrupt Enable */
#define I2SMCC_IERA_TXLUNF0 (0x1u << 8) /**< \brief (I2SMCC_IERA) I2S Transmit Left 0 or TDM Channel 2x Underrun Interrupt Enable */
#define I2SMCC_IERA_TXRUNF0 (0x1u << 9) /**< \brief (I2SMCC_IERA) I2S Transmit Right 0 or TDM Channel [2x]+1 Underrun Interrupt Enable */
#define I2SMCC_IERA_TXLUNF1 (0x1u << 10) /**< \brief (I2SMCC_IERA) I2S Transmit Left 1 or TDM Channel 2x Underrun Interrupt Enable */
#define I2SMCC_IERA_TXRUNF1 (0x1u << 11) /**< \brief (I2SMCC_IERA) I2S Transmit Right 1 or TDM Channel [2x]+1 Underrun Interrupt Enable */
#define I2SMCC_IERA_TXLUNF2 (0x1u << 12) /**< \brief (I2SMCC_IERA) I2S Transmit Left 2 or TDM Channel 2x Underrun Interrupt Enable */
#define I2SMCC_IERA_TXRUNF2 (0x1u << 13) /**< \brief (I2SMCC_IERA) I2S Transmit Right 2 or TDM Channel [2x]+1 Underrun Interrupt Enable */
#define I2SMCC_IERA_TXLUNF3 (0x1u << 14) /**< \brief (I2SMCC_IERA) I2S Transmit Left 3 or TDM Channel 2x Underrun Interrupt Enable */
#define I2SMCC_IERA_TXRUNF3 (0x1u << 15) /**< \brief (I2SMCC_IERA) I2S Transmit Right 3 or TDM Channel [2x]+1 Underrun Interrupt Enable */
#define I2SMCC_IERA_RXLRDY0 (0x1u << 16) /**< \brief (I2SMCC_IERA) I2S Receive Left 0 or TDM Channel 2x Ready Interrupt Enable */
#define I2SMCC_IERA_RXRRDY0 (0x1u << 17) /**< \brief (I2SMCC_IERA) I2S Receive Right 0 or TDM Channel [2x]+1 Ready Interrupt Enable */
#define I2SMCC_IERA_RXLRDY1 (0x1u << 18) /**< \brief (I2SMCC_IERA) I2S Receive Left 1 or TDM Channel 2x Ready Interrupt Enable */
#define I2SMCC_IERA_RXRRDY1 (0x1u << 19) /**< \brief (I2SMCC_IERA) I2S Receive Right 1 or TDM Channel [2x]+1 Ready Interrupt Enable */
#define I2SMCC_IERA_RXLRDY2 (0x1u << 20) /**< \brief (I2SMCC_IERA) I2S Receive Left 2 or TDM Channel 2x Ready Interrupt Enable */
#define I2SMCC_IERA_RXRRDY2 (0x1u << 21) /**< \brief (I2SMCC_IERA) I2S Receive Right 2 or TDM Channel [2x]+1 Ready Interrupt Enable */
#define I2SMCC_IERA_RXLRDY3 (0x1u << 22) /**< \brief (I2SMCC_IERA) I2S Receive Left 3 or TDM Channel 2x Ready Interrupt Enable */
#define I2SMCC_IERA_RXRRDY3 (0x1u << 23) /**< \brief (I2SMCC_IERA) I2S Receive Right 3 or TDM Channel [2x]+1 Ready Interrupt Enable */
#define I2SMCC_IERA_RXLOVF0 (0x1u << 24) /**< \brief (I2SMCC_IERA) I2S Receive Left 0 or TDM Channel 2x Overrun Interrupt Enable */
#define I2SMCC_IERA_RXROVF0 (0x1u << 25) /**< \brief (I2SMCC_IERA) I2S Receive Right 0 or TDM Channel [2x]+1 Overrun Interrupt Enable */
#define I2SMCC_IERA_RXLOVF1 (0x1u << 26) /**< \brief (I2SMCC_IERA) I2S Receive Left 1 or TDM Channel 2x Overrun Interrupt Enable */
#define I2SMCC_IERA_RXROVF1 (0x1u << 27) /**< \brief (I2SMCC_IERA) I2S Receive Right 1 or TDM Channel [2x]+1 Overrun Interrupt Enable */
#define I2SMCC_IERA_RXLOVF2 (0x1u << 28) /**< \brief (I2SMCC_IERA) I2S Receive Left 2 or TDM Channel 2x Overrun Interrupt Enable */
#define I2SMCC_IERA_RXROVF2 (0x1u << 29) /**< \brief (I2SMCC_IERA) I2S Receive Right 2 or TDM Channel [2x]+1 Overrun Interrupt Enable */
#define I2SMCC_IERA_RXLOVF3 (0x1u << 30) /**< \brief (I2SMCC_IERA) I2S Receive Left 3 or TDM Channel 2x Overrun Interrupt Enable */
#define I2SMCC_IERA_RXROVF3 (0x1u << 31) /**< \brief (I2SMCC_IERA) I2S Receive Right 3 or TDM Channel [2x]+1 Overrun Interrupt Enable */
/* -------- I2SMCC_IDRA : (I2SMCC Offset: 0x14) Interrupt Disable Register A -------- */
#define I2SMCC_IDRA_TXLRDY0 (0x1u << 0) /**< \brief (I2SMCC_IDRA) I2S Transmit Left 0 or TDM Channel 2x ReadyInterrupt Disable */
#define I2SMCC_IDRA_TXRRDY0 (0x1u << 1) /**< \brief (I2SMCC_IDRA) I2S Transmit Right 0 or TDM Channel [2x]+1 Ready Interrupt Disable */
#define I2SMCC_IDRA_TXLRDY1 (0x1u << 2) /**< \brief (I2SMCC_IDRA) I2S Transmit Left 1 or TDM Channel 2x ReadyInterrupt Disable */
#define I2SMCC_IDRA_TXRRDY1 (0x1u << 3) /**< \brief (I2SMCC_IDRA) I2S Transmit Right 1 or TDM Channel [2x]+1 Ready Interrupt Disable */
#define I2SMCC_IDRA_TXLRDY2 (0x1u << 4) /**< \brief (I2SMCC_IDRA) I2S Transmit Left 2 or TDM Channel 2x ReadyInterrupt Disable */
#define I2SMCC_IDRA_TXRRDY2 (0x1u << 5) /**< \brief (I2SMCC_IDRA) I2S Transmit Right 2 or TDM Channel [2x]+1 Ready Interrupt Disable */
#define I2SMCC_IDRA_TXLRDY3 (0x1u << 6) /**< \brief (I2SMCC_IDRA) I2S Transmit Left 3 or TDM Channel 2x ReadyInterrupt Disable */
#define I2SMCC_IDRA_TXRRDY3 (0x1u << 7) /**< \brief (I2SMCC_IDRA) I2S Transmit Right 3 or TDM Channel [2x]+1 Ready Interrupt Disable */
#define I2SMCC_IDRA_TXLUNF0 (0x1u << 8) /**< \brief (I2SMCC_IDRA) I2S Transmit Left 0 or TDM Channel 2x Underrun Interrupt Disable */
#define I2SMCC_IDRA_TXRUNF0 (0x1u << 9) /**< \brief (I2SMCC_IDRA) I2S Transmit Right 0 or TDM Channel [2x]+1 Underrun Interrupt Disable */
#define I2SMCC_IDRA_TXLUNF1 (0x1u << 10) /**< \brief (I2SMCC_IDRA) I2S Transmit Left 1 or TDM Channel 2x Underrun Interrupt Disable */
#define I2SMCC_IDRA_TXRUNF1 (0x1u << 11) /**< \brief (I2SMCC_IDRA) I2S Transmit Right 1 or TDM Channel [2x]+1 Underrun Interrupt Disable */
#define I2SMCC_IDRA_TXLUNF2 (0x1u << 12) /**< \brief (I2SMCC_IDRA) I2S Transmit Left 2 or TDM Channel 2x Underrun Interrupt Disable */
#define I2SMCC_IDRA_TXRUNF2 (0x1u << 13) /**< \brief (I2SMCC_IDRA) I2S Transmit Right 2 or TDM Channel [2x]+1 Underrun Interrupt Disable */
#define I2SMCC_IDRA_TXLUNF3 (0x1u << 14) /**< \brief (I2SMCC_IDRA) I2S Transmit Left 3 or TDM Channel 2x Underrun Interrupt Disable */
#define I2SMCC_IDRA_TXRUNF3 (0x1u << 15) /**< \brief (I2SMCC_IDRA) I2S Transmit Right 3 or TDM Channel [2x]+1 Underrun Interrupt Disable */
#define I2SMCC_IDRA_RXLRDY0 (0x1u << 16) /**< \brief (I2SMCC_IDRA) I2S Receive Left 0 or TDM Channel 2x Ready Interrupt Disable */
#define I2SMCC_IDRA_RXRRDY0 (0x1u << 17) /**< \brief (I2SMCC_IDRA) I2S Receive Right 0 or TDM Channel [2x]+1 Ready Interrupt Disable */
#define I2SMCC_IDRA_RXLRDY1 (0x1u << 18) /**< \brief (I2SMCC_IDRA) I2S Receive Left 1 or TDM Channel 2x Ready Interrupt Disable */
#define I2SMCC_IDRA_RXRRDY1 (0x1u << 19) /**< \brief (I2SMCC_IDRA) I2S Receive Right 1 or TDM Channel [2x]+1 Ready Interrupt Disable */
#define I2SMCC_IDRA_RXLRDY2 (0x1u << 20) /**< \brief (I2SMCC_IDRA) I2S Receive Left 2 or TDM Channel 2x Ready Interrupt Disable */
#define I2SMCC_IDRA_RXRRDY2 (0x1u << 21) /**< \brief (I2SMCC_IDRA) I2S Receive Right 2 or TDM Channel [2x]+1 Ready Interrupt Disable */
#define I2SMCC_IDRA_RXLRDY3 (0x1u << 22) /**< \brief (I2SMCC_IDRA) I2S Receive Left 3 or TDM Channel 2x Ready Interrupt Disable */
#define I2SMCC_IDRA_RXRRDY3 (0x1u << 23) /**< \brief (I2SMCC_IDRA) I2S Receive Right 3 or TDM Channel [2x]+1 Ready Interrupt Disable */
#define I2SMCC_IDRA_RXLOVF0 (0x1u << 24) /**< \brief (I2SMCC_IDRA) I2S Receive Left 0 or TDM Channel 2x Overrun Interrupt Disable */
#define I2SMCC_IDRA_RXROVF0 (0x1u << 25) /**< \brief (I2SMCC_IDRA) I2S Receive Right 0 or TDM Channel [2x]+1 Overrun Interrupt Disable */
#define I2SMCC_IDRA_RXLOVF1 (0x1u << 26) /**< \brief (I2SMCC_IDRA) I2S Receive Left 1 or TDM Channel 2x Overrun Interrupt Disable */
#define I2SMCC_IDRA_RXROVF1 (0x1u << 27) /**< \brief (I2SMCC_IDRA) I2S Receive Right 1 or TDM Channel [2x]+1 Overrun Interrupt Disable */
#define I2SMCC_IDRA_RXLOVF2 (0x1u << 28) /**< \brief (I2SMCC_IDRA) I2S Receive Left 2 or TDM Channel 2x Overrun Interrupt Disable */
#define I2SMCC_IDRA_RXROVF2 (0x1u << 29) /**< \brief (I2SMCC_IDRA) I2S Receive Right 2 or TDM Channel [2x]+1 Overrun Interrupt Disable */
#define I2SMCC_IDRA_RXLOVF3 (0x1u << 30) /**< \brief (I2SMCC_IDRA) I2S Receive Left 3 or TDM Channel 2x Overrun Interrupt Disable */
#define I2SMCC_IDRA_RXROVF3 (0x1u << 31) /**< \brief (I2SMCC_IDRA) I2S Receive Right 3 or TDM Channel [2x]+1 Overrun Interrupt Disable */
/* -------- I2SMCC_IMRA : (I2SMCC Offset: 0x18) Interrupt Mask Register A -------- */
#define I2SMCC_IMRA_TXLRDY0 (0x1u << 0) /**< \brief (I2SMCC_IMRA) I2S Transmit Left 0 or TDM Channel 2x ReadyInterrupt Mask */
#define I2SMCC_IMRA_TXRRDY0 (0x1u << 1) /**< \brief (I2SMCC_IMRA) I2S Transmit Right 0 or TDM Channel [2x]+1 Ready Interrupt Mask */
#define I2SMCC_IMRA_TXLRDY1 (0x1u << 2) /**< \brief (I2SMCC_IMRA) I2S Transmit Left 1 or TDM Channel 2x ReadyInterrupt Mask */
#define I2SMCC_IMRA_TXRRDY1 (0x1u << 3) /**< \brief (I2SMCC_IMRA) I2S Transmit Right 1 or TDM Channel [2x]+1 Ready Interrupt Mask */
#define I2SMCC_IMRA_TXLRDY2 (0x1u << 4) /**< \brief (I2SMCC_IMRA) I2S Transmit Left 2 or TDM Channel 2x ReadyInterrupt Mask */
#define I2SMCC_IMRA_TXRRDY2 (0x1u << 5) /**< \brief (I2SMCC_IMRA) I2S Transmit Right 2 or TDM Channel [2x]+1 Ready Interrupt Mask */
#define I2SMCC_IMRA_TXLRDY3 (0x1u << 6) /**< \brief (I2SMCC_IMRA) I2S Transmit Left 3 or TDM Channel 2x ReadyInterrupt Mask */
#define I2SMCC_IMRA_TXRRDY3 (0x1u << 7) /**< \brief (I2SMCC_IMRA) I2S Transmit Right 3 or TDM Channel [2x]+1 Ready Interrupt Mask */
#define I2SMCC_IMRA_TXLUNF0 (0x1u << 8) /**< \brief (I2SMCC_IMRA) I2S Transmit Left 0 or TDM Channel 2x Underrun Interrupt Mask */
#define I2SMCC_IMRA_TXRUNF0 (0x1u << 9) /**< \brief (I2SMCC_IMRA) I2S Transmit Right 0 or TDM Channel [2x]+1 Underrun Interrupt Mask */
#define I2SMCC_IMRA_TXLUNF1 (0x1u << 10) /**< \brief (I2SMCC_IMRA) I2S Transmit Left 1 or TDM Channel 2x Underrun Interrupt Mask */
#define I2SMCC_IMRA_TXRUNF1 (0x1u << 11) /**< \brief (I2SMCC_IMRA) I2S Transmit Right 1 or TDM Channel [2x]+1 Underrun Interrupt Mask */
#define I2SMCC_IMRA_TXLUNF2 (0x1u << 12) /**< \brief (I2SMCC_IMRA) I2S Transmit Left 2 or TDM Channel 2x Underrun Interrupt Mask */
#define I2SMCC_IMRA_TXRUNF2 (0x1u << 13) /**< \brief (I2SMCC_IMRA) I2S Transmit Right 2 or TDM Channel [2x]+1 Underrun Interrupt Mask */
#define I2SMCC_IMRA_TXLUNF3 (0x1u << 14) /**< \brief (I2SMCC_IMRA) I2S Transmit Left 3 or TDM Channel 2x Underrun Interrupt Mask */
#define I2SMCC_IMRA_TXRUNF3 (0x1u << 15) /**< \brief (I2SMCC_IMRA) I2S Transmit Right 3 or TDM Channel [2x]+1 Underrun Interrupt Mask */
#define I2SMCC_IMRA_RXLRDY0 (0x1u << 16) /**< \brief (I2SMCC_IMRA) I2S Receive Left 0 or TDM Channel 2x Ready Interrupt Mask */
#define I2SMCC_IMRA_RXRRDY0 (0x1u << 17) /**< \brief (I2SMCC_IMRA) I2S Receive Right 0 or TDM Channel [2x]+1 Ready Interrupt Mask */
#define I2SMCC_IMRA_RXLRDY1 (0x1u << 18) /**< \brief (I2SMCC_IMRA) I2S Receive Left 1 or TDM Channel 2x Ready Interrupt Mask */
#define I2SMCC_IMRA_RXRRDY1 (0x1u << 19) /**< \brief (I2SMCC_IMRA) I2S Receive Right 1 or TDM Channel [2x]+1 Ready Interrupt Mask */
#define I2SMCC_IMRA_RXLRDY2 (0x1u << 20) /**< \brief (I2SMCC_IMRA) I2S Receive Left 2 or TDM Channel 2x Ready Interrupt Mask */
#define I2SMCC_IMRA_RXRRDY2 (0x1u << 21) /**< \brief (I2SMCC_IMRA) I2S Receive Right 2 or TDM Channel [2x]+1 Ready Interrupt Mask */
#define I2SMCC_IMRA_RXLRDY3 (0x1u << 22) /**< \brief (I2SMCC_IMRA) I2S Receive Left 3 or TDM Channel 2x Ready Interrupt Mask */
#define I2SMCC_IMRA_RXRRDY3 (0x1u << 23) /**< \brief (I2SMCC_IMRA) I2S Receive Right 3 or TDM Channel [2x]+1 Ready Interrupt Mask */
#define I2SMCC_IMRA_RXLOVF0 (0x1u << 24) /**< \brief (I2SMCC_IMRA) I2S Receive Left 0 or TDM Channel 2x Overrun Interrupt Mask */
#define I2SMCC_IMRA_RXROVF0 (0x1u << 25) /**< \brief (I2SMCC_IMRA) I2S Receive Right 0 or TDM Channel [2x]+1 Overrun Interrupt Mask */
#define I2SMCC_IMRA_RXLOVF1 (0x1u << 26) /**< \brief (I2SMCC_IMRA) I2S Receive Left 1 or TDM Channel 2x Overrun Interrupt Mask */
#define I2SMCC_IMRA_RXROVF1 (0x1u << 27) /**< \brief (I2SMCC_IMRA) I2S Receive Right 1 or TDM Channel [2x]+1 Overrun Interrupt Mask */
#define I2SMCC_IMRA_RXLOVF2 (0x1u << 28) /**< \brief (I2SMCC_IMRA) I2S Receive Left 2 or TDM Channel 2x Overrun Interrupt Mask */
#define I2SMCC_IMRA_RXROVF2 (0x1u << 29) /**< \brief (I2SMCC_IMRA) I2S Receive Right 2 or TDM Channel [2x]+1 Overrun Interrupt Mask */
#define I2SMCC_IMRA_RXLOVF3 (0x1u << 30) /**< \brief (I2SMCC_IMRA) I2S Receive Left 3 or TDM Channel 2x Overrun Interrupt Mask */
#define I2SMCC_IMRA_RXROVF3 (0x1u << 31) /**< \brief (I2SMCC_IMRA) I2S Receive Right 3 or TDM Channel [2x]+1 Overrun Interrupt Mask */
/* -------- I2SMCC_ISRA : (I2SMCC Offset: 0x1C) Interrupt Status Register A -------- */
#define I2SMCC_ISRA_TXLRDY0 (0x1u << 0) /**< \brief (I2SMCC_ISRA) I2S Transmit Left 0 or TDM Channel 2x Ready Flag (Cleared by writing I2SMCC_THR/THLxR) */
#define I2SMCC_ISRA_TXRRDY0 (0x1u << 1) /**< \brief (I2SMCC_ISRA) I2S Transmit Right 0 or TDM Channel [2x]+1 Ready Flag (Cleared by writing I2SMCC_THR/THRxR) */
#define I2SMCC_ISRA_TXLRDY1 (0x1u << 2) /**< \brief (I2SMCC_ISRA) I2S Transmit Left 1 or TDM Channel 2x Ready Flag (Cleared by writing I2SMCC_THR/THLxR) */
#define I2SMCC_ISRA_TXRRDY1 (0x1u << 3) /**< \brief (I2SMCC_ISRA) I2S Transmit Right 1 or TDM Channel [2x]+1 Ready Flag (Cleared by writing I2SMCC_THR/THRxR) */
#define I2SMCC_ISRA_TXLRDY2 (0x1u << 4) /**< \brief (I2SMCC_ISRA) I2S Transmit Left 2 or TDM Channel 2x Ready Flag (Cleared by writing I2SMCC_THR/THLxR) */
#define I2SMCC_ISRA_TXRRDY2 (0x1u << 5) /**< \brief (I2SMCC_ISRA) I2S Transmit Right 2 or TDM Channel [2x]+1 Ready Flag (Cleared by writing I2SMCC_THR/THRxR) */
#define I2SMCC_ISRA_TXLRDY3 (0x1u << 6) /**< \brief (I2SMCC_ISRA) I2S Transmit Left 3 or TDM Channel 2x Ready Flag (Cleared by writing I2SMCC_THR/THLxR) */
#define I2SMCC_ISRA_TXRRDY3 (0x1u << 7) /**< \brief (I2SMCC_ISRA) I2S Transmit Right 3 or TDM Channel [2x]+1 Ready Flag (Cleared by writing I2SMCC_THR/THRxR) */
#define I2SMCC_ISRA_TXLUNF0 (0x1u << 8) /**< \brief (I2SMCC_ISRA) I2S Transmit Left 0 or TDM Channel 2x Underrun (Cleared on read) */
#define I2SMCC_ISRA_TXRUNF0 (0x1u << 9) /**< \brief (I2SMCC_ISRA) I2S Transmit Right 0 or TDM Channel [2x]+1 Underrun Flag (Cleared on read) */
#define I2SMCC_ISRA_TXLUNF1 (0x1u << 10) /**< \brief (I2SMCC_ISRA) I2S Transmit Left 1 or TDM Channel 2x Underrun (Cleared on read) */
#define I2SMCC_ISRA_TXRUNF1 (0x1u << 11) /**< \brief (I2SMCC_ISRA) I2S Transmit Right 1 or TDM Channel [2x]+1 Underrun Flag (Cleared on read) */
#define I2SMCC_ISRA_TXLUNF2 (0x1u << 12) /**< \brief (I2SMCC_ISRA) I2S Transmit Left 2 or TDM Channel 2x Underrun (Cleared on read) */
#define I2SMCC_ISRA_TXRUNF2 (0x1u << 13) /**< \brief (I2SMCC_ISRA) I2S Transmit Right 2 or TDM Channel [2x]+1 Underrun Flag (Cleared on read) */
#define I2SMCC_ISRA_TXLUNF3 (0x1u << 14) /**< \brief (I2SMCC_ISRA) I2S Transmit Left 3 or TDM Channel 2x Underrun (Cleared on read) */
#define I2SMCC_ISRA_TXRUNF3 (0x1u << 15) /**< \brief (I2SMCC_ISRA) I2S Transmit Right 3 or TDM Channel [2x]+1 Underrun Flag (Cleared on read) */
#define I2SMCC_ISRA_RXLRDY0 (0x1u << 16) /**< \brief (I2SMCC_ISRA) I2S Receive Left 0 or TDM Channel 2x Ready Flag (Cleared by reading I2SMCC_RHR/RHLxR) */
#define I2SMCC_ISRA_RXRRDY0 (0x1u << 17) /**< \brief (I2SMCC_ISRA) I2S Receive Right 0 or TDM Channel [2x]+1 Ready Flag (Cleared by reading I2SMCC_RHR/RHLxR) */
#define I2SMCC_ISRA_RXLRDY1 (0x1u << 18) /**< \brief (I2SMCC_ISRA) I2S Receive Left 1 or TDM Channel 2x Ready Flag (Cleared by reading I2SMCC_RHR/RHLxR) */
#define I2SMCC_ISRA_RXRRDY1 (0x1u << 19) /**< \brief (I2SMCC_ISRA) I2S Receive Right 1 or TDM Channel [2x]+1 Ready Flag (Cleared by reading I2SMCC_RHR/RHLxR) */
#define I2SMCC_ISRA_RXLRDY2 (0x1u << 20) /**< \brief (I2SMCC_ISRA) I2S Receive Left 2 or TDM Channel 2x Ready Flag (Cleared by reading I2SMCC_RHR/RHLxR) */
#define I2SMCC_ISRA_RXRRDY2 (0x1u << 21) /**< \brief (I2SMCC_ISRA) I2S Receive Right 2 or TDM Channel [2x]+1 Ready Flag (Cleared by reading I2SMCC_RHR/RHLxR) */
#define I2SMCC_ISRA_RXLRDY3 (0x1u << 22) /**< \brief (I2SMCC_ISRA) I2S Receive Left 3 or TDM Channel 2x Ready Flag (Cleared by reading I2SMCC_RHR/RHLxR) */
#define I2SMCC_ISRA_RXRRDY3 (0x1u << 23) /**< \brief (I2SMCC_ISRA) I2S Receive Right 3 or TDM Channel [2x]+1 Ready Flag (Cleared by reading I2SMCC_RHR/RHLxR) */
#define I2SMCC_ISRA_RXLOVF0 (0x1u << 24) /**< \brief (I2SMCC_ISRA) I2S Receive Left 0 or TDM Channel 2x Overrun Flag (Cleared on read) */
#define I2SMCC_ISRA_RXROVF0 (0x1u << 25) /**< \brief (I2SMCC_ISRA) I2S Receive Right 0 or TDM Channel [2x]+1 Overrun Flag (Cleared on read) */
#define I2SMCC_ISRA_RXLOVF1 (0x1u << 26) /**< \brief (I2SMCC_ISRA) I2S Receive Left 1 or TDM Channel 2x Overrun Flag (Cleared on read) */
#define I2SMCC_ISRA_RXROVF1 (0x1u << 27) /**< \brief (I2SMCC_ISRA) I2S Receive Right 1 or TDM Channel [2x]+1 Overrun Flag (Cleared on read) */
#define I2SMCC_ISRA_RXLOVF2 (0x1u << 28) /**< \brief (I2SMCC_ISRA) I2S Receive Left 2 or TDM Channel 2x Overrun Flag (Cleared on read) */
#define I2SMCC_ISRA_RXROVF2 (0x1u << 29) /**< \brief (I2SMCC_ISRA) I2S Receive Right 2 or TDM Channel [2x]+1 Overrun Flag (Cleared on read) */
#define I2SMCC_ISRA_RXLOVF3 (0x1u << 30) /**< \brief (I2SMCC_ISRA) I2S Receive Left 3 or TDM Channel 2x Overrun Flag (Cleared on read) */
#define I2SMCC_ISRA_RXROVF3 (0x1u << 31) /**< \brief (I2SMCC_ISRA) I2S Receive Right 3 or TDM Channel [2x]+1 Overrun Flag (Cleared on read) */
/* -------- I2SMCC_IERB : (I2SMCC Offset: 0x20) Interrupt Enable Register B -------- */
#define I2SMCC_IERB_WERR (0x1u << 0) /**< \brief (I2SMCC_IERB) Write Error Interrupt Enable */
#define I2SMCC_IERB_ENDTX (0x1u << 4) /**< \brief (I2SMCC_IERB) End of Transmit Buffer Interrupt Enable */
#define I2SMCC_IERB_ENDRX (0x1u << 5) /**< \brief (I2SMCC_IERB) End of Receive Buffer Interrupt Enable */
#define I2SMCC_IERB_TXBUFE (0x1u << 6) /**< \brief (I2SMCC_IERB) Transmit Buffer Empty Interrupt Enable */
#define I2SMCC_IERB_RXBUFF (0x1u << 7) /**< \brief (I2SMCC_IERB) Receive Buffer Full Interrupt Enable */
/* -------- I2SMCC_IDRB : (I2SMCC Offset: 0x24) Interrupt Disable Register B -------- */
#define I2SMCC_IDRB_WERR (0x1u << 0) /**< \brief (I2SMCC_IDRB) Write Error Interrupt Disable */
#define I2SMCC_IDRB_ENDTX (0x1u << 4) /**< \brief (I2SMCC_IDRB) End of Transmit Buffer Interrupt Disable */
#define I2SMCC_IDRB_ENDRX (0x1u << 5) /**< \brief (I2SMCC_IDRB) End of Receive Buffer Interrupt Disable */
#define I2SMCC_IDRB_TXBUFE (0x1u << 6) /**< \brief (I2SMCC_IDRB) Transmit Buffer Empty Interrupt Disable */
#define I2SMCC_IDRB_RXBUFF (0x1u << 7) /**< \brief (I2SMCC_IDRB) Receive Buffer Full Interrupt Disable */
/* -------- I2SMCC_IMRB : (I2SMCC Offset: 0x28) Interrupt Mask Register B -------- */
#define I2SMCC_IMRB_WERR (0x1u << 0) /**< \brief (I2SMCC_IMRB) Write Error Interrupt Enable */
#define I2SMCC_IMRB_ENDTX (0x1u << 4) /**< \brief (I2SMCC_IMRB) End of Transmit Buffer Interrupt Mask */
#define I2SMCC_IMRB_ENDRX (0x1u << 5) /**< \brief (I2SMCC_IMRB) End of Receive Buffer Interrupt Mask */
#define I2SMCC_IMRB_TXBUFE (0x1u << 6) /**< \brief (I2SMCC_IMRB) Transmit Buffer Empty Interrupt Mask */
#define I2SMCC_IMRB_RXBUFF (0x1u << 7) /**< \brief (I2SMCC_IMRB) Receive Buffer Full Interrupt Mask */
/* -------- I2SMCC_ISRB : (I2SMCC Offset: 0x2C) Interrupt Status Register B -------- */
#define I2SMCC_ISRB_WERR (0x1u << 0) /**< \brief (I2SMCC_ISRB) Write Error Flag (cleared on read) */
#define I2SMCC_ISRB_ENDTX (0x1u << 4) /**< \brief (I2SMCC_ISRB) End of TX Buffer (cleared by writing I2SMCC_TCR or I2SMCC_TNCR) */
#define I2SMCC_ISRB_ENDRX (0x1u << 5) /**< \brief (I2SMCC_ISRB) End of RX Buffer (cleared by writing I2SMCC_RCR or I2SMCC_RNCR) */
#define I2SMCC_ISRB_TXBUFE (0x1u << 6) /**< \brief (I2SMCC_ISRB) Transmit Buffer Empty (cleared by writing I2SMCC_TCR or I2SMCC_TNCR) */
#define I2SMCC_ISRB_RXBUFF (0x1u << 7) /**< \brief (I2SMCC_ISRB) Receive Buffer Full (cleared by writing I2SMCC_RCR or I2SMCC_RNCR) */
/* -------- I2SMCC_RHR : (I2SMCC Offset: 0x30) Receiver Holding Register -------- */
#define I2SMCC_RHR_RHR_Pos 0
#define I2SMCC_RHR_RHR_Msk (0xffffffffu << I2SMCC_RHR_RHR_Pos) /**< \brief (I2SMCC_RHR) Receiver Holding Register */
/* -------- I2SMCC_THR : (I2SMCC Offset: 0x34) Transmitter Holding Register -------- */
#define I2SMCC_THR_THR_Pos 0
#define I2SMCC_THR_THR_Msk (0xffffffffu << I2SMCC_THR_THR_Pos) /**< \brief (I2SMCC_THR) Transmitter Holding Register */
#define I2SMCC_THR_THR(value) ((I2SMCC_THR_THR_Msk & ((value) << I2SMCC_THR_THR_Pos)))
/* -------- I2SMCC_RHL0R : (I2SMCC Offset: 0x40) Receiver Holding Left 0 Register -------- */
#define I2SMCC_RHL0R_RHL_Pos 0
#define I2SMCC_RHL0R_RHL_Msk (0xffffffffu << I2SMCC_RHL0R_RHL_Pos) /**< \brief (I2SMCC_RHL0R) Receiver Holding Left */
/* -------- I2SMCC_RHR0R : (I2SMCC Offset: 0x44) Receiver Holding Right 0 Register -------- */
#define I2SMCC_RHR0R_RHR_Pos 0
#define I2SMCC_RHR0R_RHR_Msk (0xffffffffu << I2SMCC_RHR0R_RHR_Pos) /**< \brief (I2SMCC_RHR0R) Receiver Holding Right */
/* -------- I2SMCC_RHL1R : (I2SMCC Offset: 0x48) Receiver Holding Left 1 Register -------- */
#define I2SMCC_RHL1R_RHL_Pos 0
#define I2SMCC_RHL1R_RHL_Msk (0xffffffffu << I2SMCC_RHL1R_RHL_Pos) /**< \brief (I2SMCC_RHL1R) Receiver Holding Left */
/* -------- I2SMCC_RHR1R : (I2SMCC Offset: 0x4C) Receiver Holding Right 1 Register -------- */
#define I2SMCC_RHR1R_RHR_Pos 0
#define I2SMCC_RHR1R_RHR_Msk (0xffffffffu << I2SMCC_RHR1R_RHR_Pos) /**< \brief (I2SMCC_RHR1R) Receiver Holding Right */
/* -------- I2SMCC_RHL2R : (I2SMCC Offset: 0x50) Receiver Holding Left 2 Register -------- */
#define I2SMCC_RHL2R_RHL_Pos 0
#define I2SMCC_RHL2R_RHL_Msk (0xffffffffu << I2SMCC_RHL2R_RHL_Pos) /**< \brief (I2SMCC_RHL2R) Receiver Holding Left */
/* -------- I2SMCC_RHR2R : (I2SMCC Offset: 0x54) Receiver Holding Right 2 Register -------- */
#define I2SMCC_RHR2R_RHR_Pos 0
#define I2SMCC_RHR2R_RHR_Msk (0xffffffffu << I2SMCC_RHR2R_RHR_Pos) /**< \brief (I2SMCC_RHR2R) Receiver Holding Right */
/* -------- I2SMCC_RHL3R : (I2SMCC Offset: 0x58) Receiver Holding Left 3 Register -------- */
#define I2SMCC_RHL3R_RHL_Pos 0
#define I2SMCC_RHL3R_RHL_Msk (0xffffffffu << I2SMCC_RHL3R_RHL_Pos) /**< \brief (I2SMCC_RHL3R) Receiver Holding Left */
/* -------- I2SMCC_RHR3R : (I2SMCC Offset: 0x5C) Receiver Holding Right 3 Register -------- */
#define I2SMCC_RHR3R_RHR_Pos 0
#define I2SMCC_RHR3R_RHR_Msk (0xffffffffu << I2SMCC_RHR3R_RHR_Pos) /**< \brief (I2SMCC_RHR3R) Receiver Holding Right */
/* -------- I2SMCC_THL0R : (I2SMCC Offset: 0x60) Transmitter Holding Left 0 Register -------- */
#define I2SMCC_THL0R_THL_Pos 0
#define I2SMCC_THL0R_THL_Msk (0xffffffffu << I2SMCC_THL0R_THL_Pos) /**< \brief (I2SMCC_THL0R) Transmitter Holding Left */
#define I2SMCC_THL0R_THL(value) ((I2SMCC_THL0R_THL_Msk & ((value) << I2SMCC_THL0R_THL_Pos)))
/* -------- I2SMCC_THR0R : (I2SMCC Offset: 0x64) Transmitter Holding Right 0 Register -------- */
#define I2SMCC_THR0R_THR_Pos 0
#define I2SMCC_THR0R_THR_Msk (0xffffffffu << I2SMCC_THR0R_THR_Pos) /**< \brief (I2SMCC_THR0R) Transmitter Holding Right */
#define I2SMCC_THR0R_THR(value) ((I2SMCC_THR0R_THR_Msk & ((value) << I2SMCC_THR0R_THR_Pos)))
/* -------- I2SMCC_THL1R : (I2SMCC Offset: 0x68) Transmitter Holding Left 1 Register -------- */
#define I2SMCC_THL1R_THL_Pos 0
#define I2SMCC_THL1R_THL_Msk (0xffffffffu << I2SMCC_THL1R_THL_Pos) /**< \brief (I2SMCC_THL1R) Transmitter Holding Left */
#define I2SMCC_THL1R_THL(value) ((I2SMCC_THL1R_THL_Msk & ((value) << I2SMCC_THL1R_THL_Pos)))
/* -------- I2SMCC_THR1R : (I2SMCC Offset: 0x6C) Transmitter Holding Right 1 Register -------- */
#define I2SMCC_THR1R_THR_Pos 0
#define I2SMCC_THR1R_THR_Msk (0xffffffffu << I2SMCC_THR1R_THR_Pos) /**< \brief (I2SMCC_THR1R) Transmitter Holding Right */
#define I2SMCC_THR1R_THR(value) ((I2SMCC_THR1R_THR_Msk & ((value) << I2SMCC_THR1R_THR_Pos)))
/* -------- I2SMCC_THL2R : (I2SMCC Offset: 0x70) Transmitter Holding Left 2 Register -------- */
#define I2SMCC_THL2R_THL_Pos 0
#define I2SMCC_THL2R_THL_Msk (0xffffffffu << I2SMCC_THL2R_THL_Pos) /**< \brief (I2SMCC_THL2R) Transmitter Holding Left */
#define I2SMCC_THL2R_THL(value) ((I2SMCC_THL2R_THL_Msk & ((value) << I2SMCC_THL2R_THL_Pos)))
/* -------- I2SMCC_THR2R : (I2SMCC Offset: 0x74) Transmitter Holding Right 2 Register -------- */
#define I2SMCC_THR2R_THR_Pos 0
#define I2SMCC_THR2R_THR_Msk (0xffffffffu << I2SMCC_THR2R_THR_Pos) /**< \brief (I2SMCC_THR2R) Transmitter Holding Right */
#define I2SMCC_THR2R_THR(value) ((I2SMCC_THR2R_THR_Msk & ((value) << I2SMCC_THR2R_THR_Pos)))
/* -------- I2SMCC_THL3R : (I2SMCC Offset: 0x78) Transmitter Holding Left 3 Register -------- */
#define I2SMCC_THL3R_THL_Pos 0
#define I2SMCC_THL3R_THL_Msk (0xffffffffu << I2SMCC_THL3R_THL_Pos) /**< \brief (I2SMCC_THL3R) Transmitter Holding Left */
#define I2SMCC_THL3R_THL(value) ((I2SMCC_THL3R_THL_Msk & ((value) << I2SMCC_THL3R_THL_Pos)))
/* -------- I2SMCC_THR3R : (I2SMCC Offset: 0x7C) Transmitter Holding Right 3 Register -------- */
#define I2SMCC_THR3R_THR_Pos 0
#define I2SMCC_THR3R_THR_Msk (0xffffffffu << I2SMCC_THR3R_THR_Pos) /**< \brief (I2SMCC_THR3R) Transmitter Holding Right */
#define I2SMCC_THR3R_THR(value) ((I2SMCC_THR3R_THR_Msk & ((value) << I2SMCC_THR3R_THR_Pos)))
/* -------- I2SMCC_WPMR : (I2SMCC Offset: 0xE4) Write Protection Mode Register -------- */
#define I2SMCC_WPMR_WPCFEN (0x1u << 0) /**< \brief (I2SMCC_WPMR) Write Protection Configuration Enable */
#define I2SMCC_WPMR_WPITEN (0x1u << 1) /**< \brief (I2SMCC_WPMR) Write Protection Interrupt Enable */
#define I2SMCC_WPMR_WPCTEN (0x1u << 2) /**< \brief (I2SMCC_WPMR) Write Protection Control Enable */
#define I2SMCC_WPMR_WPKEY_Pos 8
#define I2SMCC_WPMR_WPKEY_Msk (0xffffffu << I2SMCC_WPMR_WPKEY_Pos) /**< \brief (I2SMCC_WPMR) Write Protection Key */
#define I2SMCC_WPMR_WPKEY(value) ((I2SMCC_WPMR_WPKEY_Msk & ((value) << I2SMCC_WPMR_WPKEY_Pos)))
#define   I2SMCC_WPMR_WPKEY_PASSWD (0x493253u << 8) /**< \brief (I2SMCC_WPMR) Writing any other value in this field aborts the write operation. Always reads as 0. */
/* -------- I2SMCC_WPSR : (I2SMCC Offset: 0xE8) Write Protection Status Register -------- */
#define I2SMCC_WPSR_WPVS (0x1u << 0) /**< \brief (I2SMCC_WPSR) Write Protection Violation Status */
#define I2SMCC_WPSR_WPVSRC_Pos 8
#define I2SMCC_WPSR_WPVSRC_Msk (0xffffffu << I2SMCC_WPSR_WPVSRC_Pos) /**< \brief (I2SMCC_WPSR) Write Protection Violation Source */
/* -------- I2SMCC_RPR : (I2SMCC Offset: 0x100) Receive Pointer Register -------- */
#define I2SMCC_RPR_RXPTR_Pos 0
#define I2SMCC_RPR_RXPTR_Msk (0xffffffffu << I2SMCC_RPR_RXPTR_Pos) /**< \brief (I2SMCC_RPR) Receive Pointer Register */
#define I2SMCC_RPR_RXPTR(value) ((I2SMCC_RPR_RXPTR_Msk & ((value) << I2SMCC_RPR_RXPTR_Pos)))
/* -------- I2SMCC_RCR : (I2SMCC Offset: 0x104) Receive Counter Register -------- */
#define I2SMCC_RCR_RXCTR_Pos 0
#define I2SMCC_RCR_RXCTR_Msk (0xffffu << I2SMCC_RCR_RXCTR_Pos) /**< \brief (I2SMCC_RCR) Receive Counter Register */
#define I2SMCC_RCR_RXCTR(value) ((I2SMCC_RCR_RXCTR_Msk & ((value) << I2SMCC_RCR_RXCTR_Pos)))
/* -------- I2SMCC_TPR : (I2SMCC Offset: 0x108) Transmit Pointer Register -------- */
#define I2SMCC_TPR_TXPTR_Pos 0
#define I2SMCC_TPR_TXPTR_Msk (0xffffffffu << I2SMCC_TPR_TXPTR_Pos) /**< \brief (I2SMCC_TPR) Transmit Counter Register */
#define I2SMCC_TPR_TXPTR(value) ((I2SMCC_TPR_TXPTR_Msk & ((value) << I2SMCC_TPR_TXPTR_Pos)))
/* -------- I2SMCC_TCR : (I2SMCC Offset: 0x10C) Transmit Counter Register -------- */
#define I2SMCC_TCR_TXCTR_Pos 0
#define I2SMCC_TCR_TXCTR_Msk (0xffffu << I2SMCC_TCR_TXCTR_Pos) /**< \brief (I2SMCC_TCR) Transmit Counter Register */
#define I2SMCC_TCR_TXCTR(value) ((I2SMCC_TCR_TXCTR_Msk & ((value) << I2SMCC_TCR_TXCTR_Pos)))
/* -------- I2SMCC_RNPR : (I2SMCC Offset: 0x110) Receive Next Pointer Register -------- */
#define I2SMCC_RNPR_RXNPTR_Pos 0
#define I2SMCC_RNPR_RXNPTR_Msk (0xffffffffu << I2SMCC_RNPR_RXNPTR_Pos) /**< \brief (I2SMCC_RNPR) Receive Next Pointer */
#define I2SMCC_RNPR_RXNPTR(value) ((I2SMCC_RNPR_RXNPTR_Msk & ((value) << I2SMCC_RNPR_RXNPTR_Pos)))
/* -------- I2SMCC_RNCR : (I2SMCC Offset: 0x114) Receive Next Counter Register -------- */
#define I2SMCC_RNCR_RXNCTR_Pos 0
#define I2SMCC_RNCR_RXNCTR_Msk (0xffffu << I2SMCC_RNCR_RXNCTR_Pos) /**< \brief (I2SMCC_RNCR) Receive Next Counter */
#define I2SMCC_RNCR_RXNCTR(value) ((I2SMCC_RNCR_RXNCTR_Msk & ((value) << I2SMCC_RNCR_RXNCTR_Pos)))
/* -------- I2SMCC_TNPR : (I2SMCC Offset: 0x118) Transmit Next Pointer Register -------- */
#define I2SMCC_TNPR_TXNPTR_Pos 0
#define I2SMCC_TNPR_TXNPTR_Msk (0xffffffffu << I2SMCC_TNPR_TXNPTR_Pos) /**< \brief (I2SMCC_TNPR) Transmit Next Pointer */
#define I2SMCC_TNPR_TXNPTR(value) ((I2SMCC_TNPR_TXNPTR_Msk & ((value) << I2SMCC_TNPR_TXNPTR_Pos)))
/* -------- I2SMCC_TNCR : (I2SMCC Offset: 0x11C) Transmit Next Counter Register -------- */
#define I2SMCC_TNCR_TXNCTR_Pos 0
#define I2SMCC_TNCR_TXNCTR_Msk (0xffffu << I2SMCC_TNCR_TXNCTR_Pos) /**< \brief (I2SMCC_TNCR) Transmit Counter Next */
#define I2SMCC_TNCR_TXNCTR(value) ((I2SMCC_TNCR_TXNCTR_Msk & ((value) << I2SMCC_TNCR_TXNCTR_Pos)))
/* -------- I2SMCC_PTCR : (I2SMCC Offset: 0x120) Transfer Control Register -------- */
#define I2SMCC_PTCR_RXTEN (0x1u << 0) /**< \brief (I2SMCC_PTCR) Receiver Transfer Enable */
#define I2SMCC_PTCR_RXTDIS (0x1u << 1) /**< \brief (I2SMCC_PTCR) Receiver Transfer Disable */
#define I2SMCC_PTCR_TXTEN (0x1u << 8) /**< \brief (I2SMCC_PTCR) Transmitter Transfer Enable */
#define I2SMCC_PTCR_TXTDIS (0x1u << 9) /**< \brief (I2SMCC_PTCR) Transmitter Transfer Disable */
#define I2SMCC_PTCR_RXCBEN (0x1u << 16) /**< \brief (I2SMCC_PTCR) Receiver Circular Buffer Enable */
#define I2SMCC_PTCR_RXCBDIS (0x1u << 17) /**< \brief (I2SMCC_PTCR) Receiver Circular Buffer Disable */
#define I2SMCC_PTCR_TXCBEN (0x1u << 18) /**< \brief (I2SMCC_PTCR) Transmitter Circular Buffer Enable */
#define I2SMCC_PTCR_TXCBDIS (0x1u << 19) /**< \brief (I2SMCC_PTCR) Transmitter Circular Buffer Disable */
#define I2SMCC_PTCR_ERRCLR (0x1u << 24) /**< \brief (I2SMCC_PTCR) Transfer Bus Error Clear */
/* -------- I2SMCC_PTSR : (I2SMCC Offset: 0x124) Transfer Status Register -------- */
#define I2SMCC_PTSR_RXTEN (0x1u << 0) /**< \brief (I2SMCC_PTSR) Receiver Transfer Enable */
#define I2SMCC_PTSR_TXTEN (0x1u << 8) /**< \brief (I2SMCC_PTSR) Transmitter Transfer Enable */
#define I2SMCC_PTSR_RXCBEN (0x1u << 16) /**< \brief (I2SMCC_PTSR) Receiver Circular Buffer Enable */
#define I2SMCC_PTSR_TXCBEN (0x1u << 18) /**< \brief (I2SMCC_PTSR) Transmitter Circular Buffer Enable */
#define I2SMCC_PTSR_ERR (0x1u << 24) /**< \brief (I2SMCC_PTSR) Transfer Bus Error */
/* -------- I2SMCC_PWPMR : (I2SMCC Offset: 0x128) Write Protection Mode Register -------- */
#define I2SMCC_PWPMR_WPPTREN (0x1u << 0) /**< \brief (I2SMCC_PWPMR) Write Protection Pointer Registers Enable */
#define I2SMCC_PWPMR_WPCTREN (0x1u << 1) /**< \brief (I2SMCC_PWPMR) Write Protection Counter Registers Enable */
#define I2SMCC_PWPMR_WPCREN (0x1u << 2) /**< \brief (I2SMCC_PWPMR) Write Protection Control Register Enable */
#define I2SMCC_PWPMR_WPKEY_Pos 8
#define I2SMCC_PWPMR_WPKEY_Msk (0xffffffu << I2SMCC_PWPMR_WPKEY_Pos) /**< \brief (I2SMCC_PWPMR) Write Protection Key */
#define I2SMCC_PWPMR_WPKEY(value) ((I2SMCC_PWPMR_WPKEY_Msk & ((value) << I2SMCC_PWPMR_WPKEY_Pos)))
#define   I2SMCC_PWPMR_WPKEY_PASSWD (0x504443u << 8) /**< \brief (I2SMCC_PWPMR) Writing any other value in this field aborts the write operation. Always reads as 0. */

/*@}*/


#endif /* _PIC32CX_I2SMCC_COMPONENT_ */
