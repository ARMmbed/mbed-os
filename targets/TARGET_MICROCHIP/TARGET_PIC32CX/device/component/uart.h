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

#ifndef _PIC32CX_UART_COMPONENT_
#define _PIC32CX_UART_COMPONENT_

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR Universal Asynchronous Receiver Transmitter */
/* ============================================================================= */
/** \addtogroup PIC32CX_UART Universal Asynchronous Receiver Transmitter */
/*@{*/

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
/** \brief Uart hardware registers */
typedef struct {
  __O  uint32_t UART_CR;       /**< \brief (Uart Offset: 0x0) Control Register */
  __IO uint32_t UART_MR;       /**< \brief (Uart Offset: 0x4) Mode Register */
  __O  uint32_t UART_IER;      /**< \brief (Uart Offset: 0x8) Interrupt Enable Register */
  __O  uint32_t UART_IDR;      /**< \brief (Uart Offset: 0xC) Interrupt Disable Register */
  __I  uint32_t UART_IMR;      /**< \brief (Uart Offset: 0x10) Interrupt Mask Register */
  __I  uint32_t UART_SR;       /**< \brief (Uart Offset: 0x14) Interrupt Status Register */
  __I  uint32_t UART_RHR;      /**< \brief (Uart Offset: 0x18) Receive Holding Register */
  __O  uint32_t UART_THR;      /**< \brief (Uart Offset: 0x1C) Transmit Holding Register */
  __IO uint32_t UART_BRGR;     /**< \brief (Uart Offset: 0x20) Baud Rate Generator Register */
  __IO uint32_t UART_CMPR;     /**< \brief (Uart Offset: 0x24) Comparison Register */
  __IO uint32_t UART_RTOR;     /**< \brief (Uart Offset: 0x28) Receiver Time-out Register */
  __I  uint32_t UART_CSR;      /**< \brief (Uart Offset: 0x2C) Current Status Register */
  __I  uint32_t Reserved1[45];
  __IO uint32_t UART_WPMR;     /**< \brief (Uart Offset: 0xE4) Write Protection Mode Register */
  __I  uint32_t Reserved2[6];
  __IO uint32_t UART_RPR;      /**< \brief (Uart Offset: 0x100) Receive Pointer Register */
  __IO uint32_t UART_RCR;      /**< \brief (Uart Offset: 0x104) Receive Counter Register */
  __IO uint32_t UART_TPR;      /**< \brief (Uart Offset: 0x108) Transmit Pointer Register */
  __IO uint32_t UART_TCR;      /**< \brief (Uart Offset: 0x10C) Transmit Counter Register */
  __IO uint32_t UART_RNPR;     /**< \brief (Uart Offset: 0x110) Receive Next Pointer Register */
  __IO uint32_t UART_RNCR;     /**< \brief (Uart Offset: 0x114) Receive Next Counter Register */
  __IO uint32_t UART_TNPR;     /**< \brief (Uart Offset: 0x118) Transmit Next Pointer Register */
  __IO uint32_t UART_TNCR;     /**< \brief (Uart Offset: 0x11C) Transmit Next Counter Register */
  __O  uint32_t UART_PTCR;     /**< \brief (Uart Offset: 0x120) Transfer Control Register */
  __I  uint32_t UART_PTSR;     /**< \brief (Uart Offset: 0x124) Transfer Status Register */
  __IO uint32_t UART_PWPMR;    /**< \brief (Uart Offset: 0x128) Write Protection Mode Register */
} Uart;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */
/* -------- UART_CR : (UART Offset: 0x0) Control Register -------- */
#define UART_CR_RSTRX (0x1u << 2) /**< \brief (UART_CR) Reset Receiver */
#define UART_CR_RSTTX (0x1u << 3) /**< \brief (UART_CR) Reset Transmitter */
#define UART_CR_RXEN (0x1u << 4) /**< \brief (UART_CR) Receiver Enable */
#define UART_CR_RXDIS (0x1u << 5) /**< \brief (UART_CR) Receiver Disable */
#define UART_CR_TXEN (0x1u << 6) /**< \brief (UART_CR) Transmitter Enable */
#define UART_CR_TXDIS (0x1u << 7) /**< \brief (UART_CR) Transmitter Disable */
#define UART_CR_RSTSTA (0x1u << 8) /**< \brief (UART_CR) Reset Status */
#define UART_CR_RETTO (0x1u << 10) /**< \brief (UART_CR) Rearm Time-out */
#define UART_CR_STTTO (0x1u << 11) /**< \brief (UART_CR) Start Time-out */
#define UART_CR_REQCLR (0x1u << 12) /**< \brief (UART_CR) Request Clear */
#define UART_CR_OPT_SLEEP (0x1u << 14) /**< \brief (UART_CR) Optical Logic Sleep Mode Command */
/* -------- UART_MR : (UART Offset: 0x4) Mode Register -------- */
#define UART_MR_OPT_EN (0x1u << 0) /**< \brief (UART_MR) UART Optical Interface Enable */
#define   UART_MR_OPT_EN_DISABLED (0x0u << 0) /**< \brief (UART_MR) The UART transmitter data is not inverted before modulation. */
#define   UART_MR_OPT_EN_ENABLED (0x1u << 0) /**< \brief (UART_MR) The UART transmitter data is inverted before modulation. */
#define UART_MR_OPT_RXINV (0x1u << 1) /**< \brief (UART_MR) UART Receive Data Inverted */
#define   UART_MR_OPT_RXINV_DISABLED (0x0u << 1) /**< \brief (UART_MR) The comparator data output is not inverted before entering UART. */
#define   UART_MR_OPT_RXINV_ENABLED (0x1u << 1) /**< \brief (UART_MR) The comparator data output is inverted before entering UART. */
#define UART_MR_OPT_MDINV (0x1u << 2) /**< \brief (UART_MR) UART Modulated Data Inverted */
#define   UART_MR_OPT_MDINV_DISABLED (0x0u << 2) /**< \brief (UART_MR) The output of the modulator is not inverted. */
#define   UART_MR_OPT_MDINV_ENABLED (0x1u << 2) /**< \brief (UART_MR) The output of the modulator is inverted. */
#define UART_MR_OPT_DMOD (0x1u << 3) /**< \brief (UART_MR) Optical Demodulation Enable */
#define   UART_MR_OPT_DMOD_DISABLED (0x0u << 3) /**< \brief (UART_MR) The optical demodulator is disabled. External demodulation must be enabled. */
#define   UART_MR_OPT_DMOD_ENABLED (0x1u << 3) /**< \brief (UART_MR) The optical demodulator is enabled. */
#define UART_MR_FILTER (0x1u << 4) /**< \brief (UART_MR) Receiver Digital Filter */
#define   UART_MR_FILTER_DISABLED (0x0u << 4) /**< \brief (UART_MR) UART does not filter the receive line. */
#define   UART_MR_FILTER_ENABLED (0x1u << 4) /**< \brief (UART_MR) UART filters the receive line using a three-sample filter (16x-bit clock) (2 over 3 majority). */
#define UART_MR_ACON (0x1u << 5) /**< \brief (UART_MR) Analog Comparator Enable */
#define   UART_MR_ACON_DISABLED (0x0u << 5) /**< \brief (UART_MR) The analog comparator is disabled. If OPT_EN=1, the analog comparator is enabled. */
#define   UART_MR_ACON_ENABLED (0x1u << 5) /**< \brief (UART_MR) The analog comparator is enabled. */
#define UART_MR_EDGESEL_Pos 6
#define UART_MR_EDGESEL_Msk (0x3u << UART_MR_EDGESEL_Pos) /**< \brief (UART_MR) Analog Comparator Output Edge Selection */
#define UART_MR_EDGESEL(value) ((UART_MR_EDGESEL_Msk & ((value) << UART_MR_EDGESEL_Pos)))
#define   UART_MR_EDGESEL_RISING (0x0u << 6) /**< \brief (UART_MR) The UART_SR.ACE is set if a rising edge is detected on analog comparator output. */
#define   UART_MR_EDGESEL_FALLING (0x1u << 6) /**< \brief (UART_MR) The UART_SR.ACE is set if a falling edge is detected on analog comparator output. */
#define   UART_MR_EDGESEL_ANY_EDGE (0x2u << 6) /**< \brief (UART_MR) The UART_SR.ACE is set as soon as an edge is detected on analog comparator output. */
#define UART_MR_OPT_WKUP (0x1u << 8) /**< \brief (UART_MR) Optical Link Activity Wake-up Enable */
#define   UART_MR_OPT_WKUP_DISABLED (0x0u << 8) /**< \brief (UART_MR) To detect any activity on the output of analog comparator, the clock is always active and OPT_EN must be written to 1.. */
#define   UART_MR_OPT_WKUP_ENABLED (0x1u << 8) /**< \brief (UART_MR) If OPT_EN=0 and a logical 1 is detected after inversion (if RXINV=1) on analog comparator output, the clock if automatically enabled for all UART sub-modules. After a period of inactivity on URXD (time-out) the software can instruct the UART to disabled the clock of all sub-modules to reduce power consumption by applying the UART_CR.OPT_SLEEP command. */
#define UART_MR_PAR_Pos 9
#define UART_MR_PAR_Msk (0x7u << UART_MR_PAR_Pos) /**< \brief (UART_MR) Parity Type */
#define UART_MR_PAR(value) ((UART_MR_PAR_Msk & ((value) << UART_MR_PAR_Pos)))
#define   UART_MR_PAR_EVEN (0x0u << 9) /**< \brief (UART_MR) Even Parity */
#define   UART_MR_PAR_ODD (0x1u << 9) /**< \brief (UART_MR) Odd Parity */
#define   UART_MR_PAR_SPACE (0x2u << 9) /**< \brief (UART_MR) Space: parity forced to 0 */
#define   UART_MR_PAR_MARK (0x3u << 9) /**< \brief (UART_MR) Mark: parity forced to 1 */
#define   UART_MR_PAR_NO (0x4u << 9) /**< \brief (UART_MR) No parity */
#define UART_MR_BRSRCCK (0x1u << 12) /**< \brief (UART_MR) Baud Rate Source Clock */
#define   UART_MR_BRSRCCK_PERIPH_CLK (0x0u << 12) /**< \brief (UART_MR) The baud rate is driven by the peripheral clock */
#define   UART_MR_BRSRCCK_GCLK (0x1u << 12) /**< \brief (UART_MR) The baud rate is driven by a PMC-programmable clock GCLK (see the section "Power Management Controller (PMC)"). */
#define UART_MR_CHMODE_Pos 14
#define UART_MR_CHMODE_Msk (0x3u << UART_MR_CHMODE_Pos) /**< \brief (UART_MR) Channel Mode */
#define UART_MR_CHMODE(value) ((UART_MR_CHMODE_Msk & ((value) << UART_MR_CHMODE_Pos)))
#define   UART_MR_CHMODE_NORMAL (0x0u << 14) /**< \brief (UART_MR) Normal mode */
#define   UART_MR_CHMODE_AUTOMATIC (0x1u << 14) /**< \brief (UART_MR) Automatic echo */
#define   UART_MR_CHMODE_LOCAL_LOOPBACK (0x2u << 14) /**< \brief (UART_MR) Local loopback */
#define   UART_MR_CHMODE_REMOTE_LOOPBACK (0x3u << 14) /**< \brief (UART_MR) Remote loopback */
#define UART_MR_OPT_CLKDIV_Pos 16
#define UART_MR_OPT_CLKDIV_Msk (0x1fu << UART_MR_OPT_CLKDIV_Pos) /**< \brief (UART_MR) Optical Link Clock Divider */
#define UART_MR_OPT_CLKDIV(value) ((UART_MR_OPT_CLKDIV_Msk & ((value) << UART_MR_OPT_CLKDIV_Pos)))
#define UART_MR_OPT_DUTY_Pos 24
#define UART_MR_OPT_DUTY_Msk (0x7u << UART_MR_OPT_DUTY_Pos) /**< \brief (UART_MR) Optical Link Modulation Clock Duty Cycle */
#define UART_MR_OPT_DUTY(value) ((UART_MR_OPT_DUTY_Msk & ((value) << UART_MR_OPT_DUTY_Pos)))
#define   UART_MR_OPT_DUTY_DUTY_50 (0x0u << 24) /**< \brief (UART_MR) Modulation clock duty cycle Is 50%. */
#define   UART_MR_OPT_DUTY_DUTY_43P75 (0x1u << 24) /**< \brief (UART_MR) Modulation clock duty cycle Is 43.75%. */
#define   UART_MR_OPT_DUTY_DUTY_37P5 (0x2u << 24) /**< \brief (UART_MR) Modulation clock duty cycle Is 37.5%. */
#define   UART_MR_OPT_DUTY_DUTY_31P25 (0x3u << 24) /**< \brief (UART_MR) Modulation clock duty cycle Is 31.75%. */
#define   UART_MR_OPT_DUTY_DUTY_25 (0x4u << 24) /**< \brief (UART_MR) Modulation clock duty cycle Is 25%. */
#define   UART_MR_OPT_DUTY_DUTY_18P75 (0x5u << 24) /**< \brief (UART_MR) Modulation clock duty cycle Is 18.75%. */
#define   UART_MR_OPT_DUTY_DUTY_12P5 (0x6u << 24) /**< \brief (UART_MR) Modulation clock duty cycle Is 12.5%. */
#define   UART_MR_OPT_DUTY_DUTY_6P25 (0x7u << 24) /**< \brief (UART_MR) Modulation clock duty cycle Is 6.25%. */
#define UART_MR_OPT_CMPTH_Pos 28
#define UART_MR_OPT_CMPTH_Msk (0x7u << UART_MR_OPT_CMPTH_Pos) /**< \brief (UART_MR) Receive Path Comparator Threshold */
#define UART_MR_OPT_CMPTH(value) ((UART_MR_OPT_CMPTH_Msk & ((value) << UART_MR_OPT_CMPTH_Pos)))
#define   UART_MR_OPT_CMPTH_VDDIO_DIV2 (0x0u << 28) /**< \brief (UART_MR) Comparator threshold is VDDIO/2 volts. */
#define   UART_MR_OPT_CMPTH_VDDIO_DIV2P5 (0x1u << 28) /**< \brief (UART_MR) Comparator threshold is VDDIO/2.5 volts. */
#define   UART_MR_OPT_CMPTH_VDDIO_DIV3P3 (0x2u << 28) /**< \brief (UART_MR) Comparator threshold is VDDIO/3.3 volts. */
#define   UART_MR_OPT_CMPTH_VDDIO_DIV5 (0x3u << 28) /**< \brief (UART_MR) Comparator threshold is VDDIO/5 volts. */
#define   UART_MR_OPT_CMPTH_VDDIO_DIV10 (0x4u << 28) /**< \brief (UART_MR) Comparator threshold is VDDIO/10 volts. */
/* -------- UART_IER : (UART Offset: 0x8) Interrupt Enable Register -------- */
#define UART_IER_RXRDY (0x1u << 0) /**< \brief (UART_IER) Enable RXRDY Interrupt */
#define UART_IER_TXRDY (0x1u << 1) /**< \brief (UART_IER) Enable TXRDY Interrupt */
#define UART_IER_ENDRX (0x1u << 3) /**< \brief (UART_IER) Enable End of Receive Transfer Interrupt */
#define UART_IER_ENDTX (0x1u << 4) /**< \brief (UART_IER) Enable End of Transmit Interrupt */
#define UART_IER_OVRE (0x1u << 5) /**< \brief (UART_IER) Enable Overrun Error Interrupt */
#define UART_IER_FRAME (0x1u << 6) /**< \brief (UART_IER) Enable Framing Error Interrupt */
#define UART_IER_PARE (0x1u << 7) /**< \brief (UART_IER) Enable Parity Error Interrupt */
#define UART_IER_TIMEOUT (0x1u << 8) /**< \brief (UART_IER) Enable Time-out Interrupt */
#define UART_IER_TXEMPTY (0x1u << 9) /**< \brief (UART_IER) Enable TXEMPTY Interrupt */
#define UART_IER_TXBUFE (0x1u << 11) /**< \brief (UART_IER) Enable Buffer Empty Interrupt */
#define UART_IER_RXBUFF (0x1u << 12) /**< \brief (UART_IER) Enable Buffer Full Interrupt */
#define UART_IER_CMP (0x1u << 15) /**< \brief (UART_IER) Enable Comparison Interrupt */
#define UART_IER_ACE (0x1u << 16) /**< \brief (UART_IER) Enable Analog Comparator Event Interrupt */
/* -------- UART_IDR : (UART Offset: 0xC) Interrupt Disable Register -------- */
#define UART_IDR_RXRDY (0x1u << 0) /**< \brief (UART_IDR) Disable RXRDY Interrupt */
#define UART_IDR_TXRDY (0x1u << 1) /**< \brief (UART_IDR) Disable TXRDY Interrupt */
#define UART_IDR_ENDRX (0x1u << 3) /**< \brief (UART_IDR) Disable End of Receive Transfer Interrupt */
#define UART_IDR_ENDTX (0x1u << 4) /**< \brief (UART_IDR) Disable End of Transmit Interrupt */
#define UART_IDR_OVRE (0x1u << 5) /**< \brief (UART_IDR) Disable Overrun Error Interrupt */
#define UART_IDR_FRAME (0x1u << 6) /**< \brief (UART_IDR) Disable Framing Error Interrupt */
#define UART_IDR_PARE (0x1u << 7) /**< \brief (UART_IDR) Disable Parity Error Interrupt */
#define UART_IDR_TIMEOUT (0x1u << 8) /**< \brief (UART_IDR) Disable Time-out Interrupt */
#define UART_IDR_TXEMPTY (0x1u << 9) /**< \brief (UART_IDR) Disable TXEMPTY Interrupt */
#define UART_IDR_TXBUFE (0x1u << 11) /**< \brief (UART_IDR) Disable Buffer Empty Interrupt */
#define UART_IDR_RXBUFF (0x1u << 12) /**< \brief (UART_IDR) Disable Buffer Full Interrupt */
#define UART_IDR_CMP (0x1u << 15) /**< \brief (UART_IDR) Disable Comparison Interrupt */
#define UART_IDR_ACE (0x1u << 16) /**< \brief (UART_IDR) Disable Analog Comparator Event Interrupt */
/* -------- UART_IMR : (UART Offset: 0x10) Interrupt Mask Register -------- */
#define UART_IMR_RXRDY (0x1u << 0) /**< \brief (UART_IMR) Mask RXRDY Interrupt */
#define UART_IMR_TXRDY (0x1u << 1) /**< \brief (UART_IMR) Mask TXRDY Interrupt */
#define UART_IMR_ENDRX (0x1u << 3) /**< \brief (UART_IMR) Mask End of Receive Transfer Interrupt */
#define UART_IMR_ENDTX (0x1u << 4) /**< \brief (UART_IMR) Mask End of Transmit Interrupt */
#define UART_IMR_OVRE (0x1u << 5) /**< \brief (UART_IMR) Mask Overrun Error Interrupt */
#define UART_IMR_FRAME (0x1u << 6) /**< \brief (UART_IMR) Mask Framing Error Interrupt */
#define UART_IMR_PARE (0x1u << 7) /**< \brief (UART_IMR) Mask Parity Error Interrupt */
#define UART_IMR_TIMEOUT (0x1u << 8) /**< \brief (UART_IMR) Mask Time-out Interrupt */
#define UART_IMR_TXEMPTY (0x1u << 9) /**< \brief (UART_IMR) Mask TXEMPTY Interrupt */
#define UART_IMR_TXBUFE (0x1u << 11) /**< \brief (UART_IMR) Mask TXBUFE Interrupt */
#define UART_IMR_RXBUFF (0x1u << 12) /**< \brief (UART_IMR) Mask RXBUFF Interrupt */
#define UART_IMR_CMP (0x1u << 15) /**< \brief (UART_IMR) Mask Comparison Interrupt */
#define UART_IMR_ACE (0x1u << 16) /**< \brief (UART_IMR) Mask Analog Comparator Event Interrupt */
/* -------- UART_SR : (UART Offset: 0x14) Interrupt Status Register -------- */
#define UART_SR_RXRDY (0x1u << 0) /**< \brief (UART_SR) Receiver Ready (Cleared by reading the UART_RHR) */
#define UART_SR_TXRDY (0x1u << 1) /**< \brief (UART_SR) Transmitter Ready (Cleared by writing the UART_THR) */
#define UART_SR_ENDRX (0x1u << 3) /**< \brief (UART_SR) End of Receiver Transfer */
#define UART_SR_ENDTX (0x1u << 4) /**< \brief (UART_SR) End of Transmitter Transfer */
#define UART_SR_OVRE (0x1u << 5) /**< \brief (UART_SR) Overrun Error (Cleared by writing UART_CR.RSTSTA) */
#define UART_SR_FRAME (0x1u << 6) /**< \brief (UART_SR) Framing Error (Cleared by writing UART_CR.RSTSTA) */
#define UART_SR_PARE (0x1u << 7) /**< \brief (UART_SR) Parity Error (Cleared by writing UART_CR.RSTSTA) */
#define UART_SR_TIMEOUT (0x1u << 8) /**< \brief (UART_SR) Receiver Time-out (Cleared by writing the UART_CR.STTTO) */
#define UART_SR_TXEMPTY (0x1u << 9) /**< \brief (UART_SR) Transmitter Empty (Cleared by writing the UART_THR) */
#define UART_SR_TXBUFE (0x1u << 11) /**< \brief (UART_SR) Transmission Buffer Empty */
#define UART_SR_RXBUFF (0x1u << 12) /**< \brief (UART_SR) Receive Buffer Full */
#define UART_SR_CMP (0x1u << 15) /**< \brief (UART_SR) Comparison Match (Cleared by writing the UART_CR.RSTSTA) */
#define UART_SR_ACE (0x1u << 16) /**< \brief (UART_SR) Analog Comparator Event Interrupt (Cleared by writing UART_CR.RSTSTA) */
/* -------- UART_RHR : (UART Offset: 0x18) Receive Holding Register -------- */
#define UART_RHR_RXCHR_Pos 0
#define UART_RHR_RXCHR_Msk (0xffu << UART_RHR_RXCHR_Pos) /**< \brief (UART_RHR) Received Character */
/* -------- UART_THR : (UART Offset: 0x1C) Transmit Holding Register -------- */
#define UART_THR_TXCHR_Pos 0
#define UART_THR_TXCHR_Msk (0xffu << UART_THR_TXCHR_Pos) /**< \brief (UART_THR) Character to be Transmitted */
#define UART_THR_TXCHR(value) ((UART_THR_TXCHR_Msk & ((value) << UART_THR_TXCHR_Pos)))
/* -------- UART_BRGR : (UART Offset: 0x20) Baud Rate Generator Register -------- */
#define UART_BRGR_CD_Pos 0
#define UART_BRGR_CD_Msk (0xffffu << UART_BRGR_CD_Pos) /**< \brief (UART_BRGR) Clock Divisor */
#define UART_BRGR_CD(value) ((UART_BRGR_CD_Msk & ((value) << UART_BRGR_CD_Pos)))
/* -------- UART_CMPR : (UART Offset: 0x24) Comparison Register -------- */
#define UART_CMPR_VAL1_Pos 0
#define UART_CMPR_VAL1_Msk (0xffu << UART_CMPR_VAL1_Pos) /**< \brief (UART_CMPR) First Comparison Value for Received Character */
#define UART_CMPR_VAL1(value) ((UART_CMPR_VAL1_Msk & ((value) << UART_CMPR_VAL1_Pos)))
#define UART_CMPR_CMPMODE (0x1u << 12) /**< \brief (UART_CMPR) Comparison Mode */
#define   UART_CMPR_CMPMODE_FLAG_ONLY (0x0u << 12) /**< \brief (UART_CMPR) Any character is received and comparison function drives CMP flag. */
#define   UART_CMPR_CMPMODE_START_CONDITION (0x1u << 12) /**< \brief (UART_CMPR) Comparison condition must be met to start reception. */
#define UART_CMPR_CMPPAR (0x1u << 14) /**< \brief (UART_CMPR) Compare Parity */
#define UART_CMPR_VAL2_Pos 16
#define UART_CMPR_VAL2_Msk (0xffu << UART_CMPR_VAL2_Pos) /**< \brief (UART_CMPR) Second Comparison Value for Received Character */
#define UART_CMPR_VAL2(value) ((UART_CMPR_VAL2_Msk & ((value) << UART_CMPR_VAL2_Pos)))
/* -------- UART_RTOR : (UART Offset: 0x28) Receiver Time-out Register -------- */
#define UART_RTOR_TO_Pos 0
#define UART_RTOR_TO_Msk (0xffu << UART_RTOR_TO_Pos) /**< \brief (UART_RTOR) Time-out Value */
#define UART_RTOR_TO(value) ((UART_RTOR_TO_Msk & ((value) << UART_RTOR_TO_Pos)))
/* -------- UART_CSR : (UART Offset: 0x2C) Current Status Register -------- */
#define UART_CSR_ACO (0x1u << 0) /**< \brief (UART_CSR) Analog Comparator Output */
/* -------- UART_WPMR : (UART Offset: 0xE4) Write Protection Mode Register -------- */
#define UART_WPMR_WPEN (0x1u << 0) /**< \brief (UART_WPMR) Write Protection Enable */
#define UART_WPMR_WPKEY_Pos 8
#define UART_WPMR_WPKEY_Msk (0xffffffu << UART_WPMR_WPKEY_Pos) /**< \brief (UART_WPMR) Write Protection Key */
#define UART_WPMR_WPKEY(value) ((UART_WPMR_WPKEY_Msk & ((value) << UART_WPMR_WPKEY_Pos)))
#define   UART_WPMR_WPKEY_PASSWD (0x554152u << 8) /**< \brief (UART_WPMR) Writing any other value in this field aborts the write operation. Always reads as 0. */
/* -------- UART_RPR : (UART Offset: 0x100) Receive Pointer Register -------- */
#define UART_RPR_RXPTR_Pos 0
#define UART_RPR_RXPTR_Msk (0xffffffffu << UART_RPR_RXPTR_Pos) /**< \brief (UART_RPR) Receive Pointer Register */
#define UART_RPR_RXPTR(value) ((UART_RPR_RXPTR_Msk & ((value) << UART_RPR_RXPTR_Pos)))
/* -------- UART_RCR : (UART Offset: 0x104) Receive Counter Register -------- */
#define UART_RCR_RXCTR_Pos 0
#define UART_RCR_RXCTR_Msk (0xffffu << UART_RCR_RXCTR_Pos) /**< \brief (UART_RCR) Receive Counter Register */
#define UART_RCR_RXCTR(value) ((UART_RCR_RXCTR_Msk & ((value) << UART_RCR_RXCTR_Pos)))
/* -------- UART_TPR : (UART Offset: 0x108) Transmit Pointer Register -------- */
#define UART_TPR_TXPTR_Pos 0
#define UART_TPR_TXPTR_Msk (0xffffffffu << UART_TPR_TXPTR_Pos) /**< \brief (UART_TPR) Transmit Counter Register */
#define UART_TPR_TXPTR(value) ((UART_TPR_TXPTR_Msk & ((value) << UART_TPR_TXPTR_Pos)))
/* -------- UART_TCR : (UART Offset: 0x10C) Transmit Counter Register -------- */
#define UART_TCR_TXCTR_Pos 0
#define UART_TCR_TXCTR_Msk (0xffffu << UART_TCR_TXCTR_Pos) /**< \brief (UART_TCR) Transmit Counter Register */
#define UART_TCR_TXCTR(value) ((UART_TCR_TXCTR_Msk & ((value) << UART_TCR_TXCTR_Pos)))
/* -------- UART_RNPR : (UART Offset: 0x110) Receive Next Pointer Register -------- */
#define UART_RNPR_RXNPTR_Pos 0
#define UART_RNPR_RXNPTR_Msk (0xffffffffu << UART_RNPR_RXNPTR_Pos) /**< \brief (UART_RNPR) Receive Next Pointer */
#define UART_RNPR_RXNPTR(value) ((UART_RNPR_RXNPTR_Msk & ((value) << UART_RNPR_RXNPTR_Pos)))
/* -------- UART_RNCR : (UART Offset: 0x114) Receive Next Counter Register -------- */
#define UART_RNCR_RXNCTR_Pos 0
#define UART_RNCR_RXNCTR_Msk (0xffffu << UART_RNCR_RXNCTR_Pos) /**< \brief (UART_RNCR) Receive Next Counter */
#define UART_RNCR_RXNCTR(value) ((UART_RNCR_RXNCTR_Msk & ((value) << UART_RNCR_RXNCTR_Pos)))
/* -------- UART_TNPR : (UART Offset: 0x118) Transmit Next Pointer Register -------- */
#define UART_TNPR_TXNPTR_Pos 0
#define UART_TNPR_TXNPTR_Msk (0xffffffffu << UART_TNPR_TXNPTR_Pos) /**< \brief (UART_TNPR) Transmit Next Pointer */
#define UART_TNPR_TXNPTR(value) ((UART_TNPR_TXNPTR_Msk & ((value) << UART_TNPR_TXNPTR_Pos)))
/* -------- UART_TNCR : (UART Offset: 0x11C) Transmit Next Counter Register -------- */
#define UART_TNCR_TXNCTR_Pos 0
#define UART_TNCR_TXNCTR_Msk (0xffffu << UART_TNCR_TXNCTR_Pos) /**< \brief (UART_TNCR) Transmit Counter Next */
#define UART_TNCR_TXNCTR(value) ((UART_TNCR_TXNCTR_Msk & ((value) << UART_TNCR_TXNCTR_Pos)))
/* -------- UART_PTCR : (UART Offset: 0x120) Transfer Control Register -------- */
#define UART_PTCR_RXTEN (0x1u << 0) /**< \brief (UART_PTCR) Receiver Transfer Enable */
#define UART_PTCR_RXTDIS (0x1u << 1) /**< \brief (UART_PTCR) Receiver Transfer Disable */
#define UART_PTCR_TXTEN (0x1u << 8) /**< \brief (UART_PTCR) Transmitter Transfer Enable */
#define UART_PTCR_TXTDIS (0x1u << 9) /**< \brief (UART_PTCR) Transmitter Transfer Disable */
#define UART_PTCR_RXCBEN (0x1u << 16) /**< \brief (UART_PTCR) Receiver Circular Buffer Enable */
#define UART_PTCR_RXCBDIS (0x1u << 17) /**< \brief (UART_PTCR) Receiver Circular Buffer Disable */
#define UART_PTCR_TXCBEN (0x1u << 18) /**< \brief (UART_PTCR) Transmitter Circular Buffer Enable */
#define UART_PTCR_TXCBDIS (0x1u << 19) /**< \brief (UART_PTCR) Transmitter Circular Buffer Disable */
#define UART_PTCR_ERRCLR (0x1u << 24) /**< \brief (UART_PTCR) Transfer Bus Error Clear */
/* -------- UART_PTSR : (UART Offset: 0x124) Transfer Status Register -------- */
#define UART_PTSR_RXTEN (0x1u << 0) /**< \brief (UART_PTSR) Receiver Transfer Enable */
#define UART_PTSR_TXTEN (0x1u << 8) /**< \brief (UART_PTSR) Transmitter Transfer Enable */
#define UART_PTSR_RXCBEN (0x1u << 16) /**< \brief (UART_PTSR) Receiver Circular Buffer Enable */
#define UART_PTSR_TXCBEN (0x1u << 18) /**< \brief (UART_PTSR) Transmitter Circular Buffer Enable */
#define UART_PTSR_ERR (0x1u << 24) /**< \brief (UART_PTSR) Transfer Bus Error */
/* -------- UART_PWPMR : (UART Offset: 0x128) Write Protection Mode Register -------- */
#define UART_PWPMR_WPPTREN (0x1u << 0) /**< \brief (UART_PWPMR) Write Protection Pointer Registers Enable */
#define UART_PWPMR_WPCTREN (0x1u << 1) /**< \brief (UART_PWPMR) Write Protection Counter Registers Enable */
#define UART_PWPMR_WPCREN (0x1u << 2) /**< \brief (UART_PWPMR) Write Protection Control Register Enable */
#define UART_PWPMR_WPKEY_Pos 8
#define UART_PWPMR_WPKEY_Msk (0xffffffu << UART_PWPMR_WPKEY_Pos) /**< \brief (UART_PWPMR) Write Protection Key */
#define UART_PWPMR_WPKEY(value) ((UART_PWPMR_WPKEY_Msk & ((value) << UART_PWPMR_WPKEY_Pos)))
#define   UART_PWPMR_WPKEY_PASSWD (0x504443u << 8) /**< \brief (UART_PWPMR) Writing any other value in this field aborts the write operation. Always reads as 0. */

/*@}*/


#endif /* _PIC32CX_UART_COMPONENT_ */
