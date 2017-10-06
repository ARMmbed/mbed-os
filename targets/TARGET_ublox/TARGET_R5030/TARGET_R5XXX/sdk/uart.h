#ifndef INCLUDED_UART
#define INCLUDED_UART
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

/** uart HAL Spreadsheet version number */
#define UART_HAL_VERSION 5

/** Uart Module
*/
struct uart_s {
   /** UART Control register  Enable at address offset 0x000, read-write */
   uint32_t crs;
   /** UART Control register  Disable at address offset 0x004, read-write */
   uint32_t crc;
   /** UART Control register  at address offset 0x008, read-write */
   uint32_t cr;
   /** Reserved space */
   uint8_t fill0[4];
   /** interrupt enable register Enable at address offset 0x010, read-write */
   uint32_t imrs;
   /** interrupt enable register Disable at address offset 0x014, read-write */
   uint32_t imrc;
   /** interrupt enable register at address offset 0x018, read-write */
   uint32_t imr;
   /** Reserved space */
   uint8_t fill1[4];
   /** status register at address offset 0x020, read-write */
   uint32_t sr;
   /** Baudrate generator counter reload value UART baudrate = fPCLK / (16*(<BaudRate>+1)) at address offset 0x024, read-write */
   uint32_t br;
   /**  at address offset 0x028, read-write */
   uint32_t tor;
   /** TX FIFO flush register                                                                                                             TX FIFO Flush status ( readback ) at address offset 0x02C, read-write */
   uint32_t tffr;
   /** RX FIFO flush register.                                                                                                           RX FIFO Flush status ( readback ) at address offset 0x030, read-write */
   uint32_t rffr;
   /** Number of Data words currently in TX FIFO at address offset 0x034, read-only */
   uint32_t twcr;
   /** Number of Data words currently in RX FIFO at address offset 0x038, read-only */
   uint32_t rwcr;
   /**  at address offset 0x03C, read-write */
   uint32_t dst_ext;
   /**  at address offset 0x040, read-write */
   uint32_t autobaud;
   /**  at address offset 0x044, read-write */
   uint32_t nco;
   /** Reserved space */
   uint8_t fill2[56];
   /** TX data buffer register at address offset 0x080, read-write */
   uint32_t thr;
   /** Reserved space */
   uint8_t fill3[124];
   /** RX data buffer register at address offset 0x100, read-only */
   uint32_t rhr;
   /** Reserved space */
   uint8_t fill4[252];
   /** TX DMA: current pointer register. Aligned to tx dma mode ( 8/16/32 bit ). Upper  bits [31:28] correspond to "0010".  at address offset 0x200, read-write */
   uint32_t ctxptr;
   /** TX DMA: current byte count register at address offset 0x204, read-write */
   uint32_t ctxcnt;
   /** TX DMA: next pointer register. Aligned to tx dma mode ( 8/16/32 bit ). Upper  bits [31:28] correspond to "0010" at address offset 0x208, read-write */
   uint32_t ntxptr;
   /** TX DMA: next byte count register at address offset 0x20C, read-write */
   uint32_t ntxcnt;
   /** RX DMA: current pointer register. Aligned to tx dma mode ( 8/16/32 bit ). Upper  bits [31:28] correspond to "0010" at address offset 0x210, read-write */
   uint32_t crxptr;
   /** RX DMA: current free byte count register. Has to be a multiple of 2 or 4 in 16 or 32 bit modes respectively. at address offset 0x214, read-write */
   uint32_t crxcnt;
   /** RX DMA: next pointer register. Aligned to tx dma mode ( 8/16/32 bit ). Upper  bits [31:28] correspond to "0010" at address offset 0x218, read-write */
   uint32_t nrxptr;
   /** RX DMA: next free byte count register at address offset 0x21C, read-write */
   uint32_t nrxcnt;
   /** RX DMA: RX Data byte compare value register at address offset 0x220, read-write */
   uint32_t rxcmp;
};

/** bit field defines for uart_s#crs */
#define UART_CRS_MODE_OFFSET 0
#define UART_CRS_MODE_SIZE 2
/** 7 bit no parity */
#define UART_CRS_MODE_7NP_VALUE 2
/** 7 bit with parity */
#define UART_CRS_MODE_7P_VALUE 3
/** 8 bit no parity */
#define UART_CRS_MODE_8NP_VALUE 0
/** 8 bit with parity */
#define UART_CRS_MODE_8P_VALUE 1
#define UART_CRS_RX_STOPBITS_OFFSET 2
#define UART_CRS_RX_STOPBITS_SIZE 2
/**  */
#define UART_CRS_RX_STOPBITS_10_VALUE 1
/**  */
#define UART_CRS_RX_STOPBITS_15_VALUE 2
/**  */
#define UART_CRS_RX_STOPBITS_20_VALUE 3
/**  */
#define UART_CRS_RX_STOPBITS_5_VALUE 0
#define UART_CRS_PARITYMODE_OFFSET 4
#define UART_CRS_PARITYMODE_SIZE 1
/**  */
#define UART_CRS_PARITYMODE_EVEN_VALUE 0
/**  */
#define UART_CRS_PARITYMODE_ODD_VALUE 1
#define UART_CRS_LOOPBACK_OFFSET 5
#define UART_CRS_LOOPBACK_SIZE 1
/**  */
#define UART_CRS_LOOPBACK_DISABLE_VALUE 0
/**  */
#define UART_CRS_LOOPBACK_ENABLE_VALUE 1
#define UART_CRS_BR_EN_OFFSET 6
#define UART_CRS_BR_EN_SIZE 1
/**  */
#define UART_CRS_BR_EN_DISABLE_VALUE 0
/**  */
#define UART_CRS_BR_EN_ENABLE_VALUE 1
#define UART_CRS_RX_EN_OFFSET 7
#define UART_CRS_RX_EN_SIZE 1
/**  */
#define UART_CRS_RX_EN_DISABLE_VALUE 0
/**  */
#define UART_CRS_RX_EN_ENABLE_VALUE 1
#define UART_CRS_REMOTELOOPBACK_OFFSET 8
#define UART_CRS_REMOTELOOPBACK_SIZE 1
/**  */
#define UART_CRS_REMOTELOOPBACK_DISABLE_VALUE 0
/**  */
#define UART_CRS_REMOTELOOPBACK_ENABLE_VALUE 1
#define UART_CRS_HW_FLOW_EN_OFFSET 9
#define UART_CRS_HW_FLOW_EN_SIZE 1
#define UART_CRS_TX_STOPBITS_OFFSET 10
#define UART_CRS_TX_STOPBITS_SIZE 2
#define UART_CRS_FILTER_EN_OFFSET 18
#define UART_CRS_FILTER_EN_SIZE 1
/**  */
#define UART_CRS_FILTER_EN_DISABLE_VALUE 0
/**  */
#define UART_CRS_FILTER_EN_ENABLE_VALUE 1
#define UART_CRS_TX_EN_OFFSET 19
#define UART_CRS_TX_EN_SIZE 1
/**  */
#define UART_CRS_TX_EN_DISABLE_VALUE 0
/**  */
#define UART_CRS_TX_EN_ENABLE_VALUE 1
#define UART_CRS_RTS_EN_FF_OFFSET 20
#define UART_CRS_RTS_EN_FF_SIZE 1
#define UART_CRS_RTS_EN_HF_OFFSET 21
#define UART_CRS_RTS_EN_HF_SIZE 1
#define UART_CRS_RTS_EN_DC_OFFSET 22
#define UART_CRS_RTS_EN_DC_SIZE 1
#define UART_CRS_RTS_EN_DCN_OFFSET 23
#define UART_CRS_RTS_EN_DCN_SIZE 1
#define UART_CRS_RTS_EN_TH_OFFSET 24
#define UART_CRS_RTS_EN_TH_SIZE 5
#define UART_CRS_AUTOBAUD_EN_OFFSET 29
#define UART_CRS_AUTOBAUD_EN_SIZE 1
#define UART_CRS_CR_RXDMA_EN_OFFSET 30
#define UART_CRS_CR_RXDMA_EN_SIZE 1
#define UART_CRS_CR_TXDMA_EN_OFFSET 31
#define UART_CRS_CR_TXDMA_EN_SIZE 1

/** bit field defines for uart_s#crc */
#define UART_CRC_MODE_OFFSET 0
#define UART_CRC_MODE_SIZE 2
/** 7 bit no parity */
#define UART_CRC_MODE_7NP_VALUE 2
/** 7 bit with parity */
#define UART_CRC_MODE_7P_VALUE 3
/** 8 bit no parity */
#define UART_CRC_MODE_8NP_VALUE 0
/** 8 bit with parity */
#define UART_CRC_MODE_8P_VALUE 1
#define UART_CRC_RX_STOPBITS_OFFSET 2
#define UART_CRC_RX_STOPBITS_SIZE 2
/**  */
#define UART_CRC_RX_STOPBITS_10_VALUE 1
/**  */
#define UART_CRC_RX_STOPBITS_15_VALUE 2
/**  */
#define UART_CRC_RX_STOPBITS_20_VALUE 3
/**  */
#define UART_CRC_RX_STOPBITS_5_VALUE 0
#define UART_CRC_PARITYMODE_OFFSET 4
#define UART_CRC_PARITYMODE_SIZE 1
/**  */
#define UART_CRC_PARITYMODE_EVEN_VALUE 0
/**  */
#define UART_CRC_PARITYMODE_ODD_VALUE 1
#define UART_CRC_LOOPBACK_OFFSET 5
#define UART_CRC_LOOPBACK_SIZE 1
/**  */
#define UART_CRC_LOOPBACK_DISABLE_VALUE 0
/**  */
#define UART_CRC_LOOPBACK_ENABLE_VALUE 1
#define UART_CRC_BR_EN_OFFSET 6
#define UART_CRC_BR_EN_SIZE 1
/**  */
#define UART_CRC_BR_EN_DISABLE_VALUE 0
/**  */
#define UART_CRC_BR_EN_ENABLE_VALUE 1
#define UART_CRC_RX_EN_OFFSET 7
#define UART_CRC_RX_EN_SIZE 1
/**  */
#define UART_CRC_RX_EN_DISABLE_VALUE 0
/**  */
#define UART_CRC_RX_EN_ENABLE_VALUE 1
#define UART_CRC_REMOTELOOPBACK_OFFSET 8
#define UART_CRC_REMOTELOOPBACK_SIZE 1
/**  */
#define UART_CRC_REMOTELOOPBACK_DISABLE_VALUE 0
/**  */
#define UART_CRC_REMOTELOOPBACK_ENABLE_VALUE 1
#define UART_CRC_HW_FLOW_EN_OFFSET 9
#define UART_CRC_HW_FLOW_EN_SIZE 1
#define UART_CRC_TX_STOPBITS_OFFSET 10
#define UART_CRC_TX_STOPBITS_SIZE 2
#define UART_CRC_FILTER_EN_OFFSET 18
#define UART_CRC_FILTER_EN_SIZE 1
/**  */
#define UART_CRC_FILTER_EN_DISABLE_VALUE 0
/**  */
#define UART_CRC_FILTER_EN_ENABLE_VALUE 1
#define UART_CRC_TX_EN_OFFSET 19
#define UART_CRC_TX_EN_SIZE 1
/**  */
#define UART_CRC_TX_EN_DISABLE_VALUE 0
/**  */
#define UART_CRC_TX_EN_ENABLE_VALUE 1
#define UART_CRC_RTS_EN_FF_OFFSET 20
#define UART_CRC_RTS_EN_FF_SIZE 1
#define UART_CRC_RTS_EN_HF_OFFSET 21
#define UART_CRC_RTS_EN_HF_SIZE 1
#define UART_CRC_RTS_EN_DC_OFFSET 22
#define UART_CRC_RTS_EN_DC_SIZE 1
#define UART_CRC_RTS_EN_DCN_OFFSET 23
#define UART_CRC_RTS_EN_DCN_SIZE 1
#define UART_CRC_RTS_EN_TH_OFFSET 24
#define UART_CRC_RTS_EN_TH_SIZE 5
#define UART_CRC_AUTOBAUD_EN_OFFSET 29
#define UART_CRC_AUTOBAUD_EN_SIZE 1
#define UART_CRC_CR_RXDMA_EN_OFFSET 30
#define UART_CRC_CR_RXDMA_EN_SIZE 1
#define UART_CRC_CR_TXDMA_EN_OFFSET 31
#define UART_CRC_CR_TXDMA_EN_SIZE 1

/** bit field defines for uart_s#cr */
#define UART_CR_MODE_OFFSET 0
#define UART_CR_MODE_SIZE 2
/** 7 bit no parity */
#define UART_CR_MODE_7NP_VALUE 2
/** 7 bit with parity */
#define UART_CR_MODE_7P_VALUE 3
/** 8 bit no parity */
#define UART_CR_MODE_8NP_VALUE 0
/** 8 bit with parity */
#define UART_CR_MODE_8P_VALUE 1
#define UART_CR_RX_STOPBITS_OFFSET 2
#define UART_CR_RX_STOPBITS_SIZE 2
/**  */
#define UART_CR_RX_STOPBITS_10_VALUE 1
/**  */
#define UART_CR_RX_STOPBITS_15_VALUE 2
/**  */
#define UART_CR_RX_STOPBITS_20_VALUE 3
/**  */
#define UART_CR_RX_STOPBITS_5_VALUE 0
#define UART_CR_PARITYMODE_OFFSET 4
#define UART_CR_PARITYMODE_SIZE 1
/**  */
#define UART_CR_PARITYMODE_EVEN_VALUE 0
/**  */
#define UART_CR_PARITYMODE_ODD_VALUE 1
#define UART_CR_LOOPBACK_OFFSET 5
#define UART_CR_LOOPBACK_SIZE 1
/**  */
#define UART_CR_LOOPBACK_DISABLE_VALUE 0
/**  */
#define UART_CR_LOOPBACK_ENABLE_VALUE 1
#define UART_CR_BR_EN_OFFSET 6
#define UART_CR_BR_EN_SIZE 1
/**  */
#define UART_CR_BR_EN_DISABLE_VALUE 0
/**  */
#define UART_CR_BR_EN_ENABLE_VALUE 1
#define UART_CR_RX_EN_OFFSET 7
#define UART_CR_RX_EN_SIZE 1
/**  */
#define UART_CR_RX_EN_DISABLE_VALUE 0
/**  */
#define UART_CR_RX_EN_ENABLE_VALUE 1
#define UART_CR_REMOTELOOPBACK_OFFSET 8
#define UART_CR_REMOTELOOPBACK_SIZE 1
/**  */
#define UART_CR_REMOTELOOPBACK_DISABLE_VALUE 0
/**  */
#define UART_CR_REMOTELOOPBACK_ENABLE_VALUE 1
#define UART_CR_HW_FLOW_EN_OFFSET 9
#define UART_CR_HW_FLOW_EN_SIZE 1
#define UART_CR_TX_STOPBITS_OFFSET 10
#define UART_CR_TX_STOPBITS_SIZE 2
#define UART_CR_FILTER_EN_OFFSET 18
#define UART_CR_FILTER_EN_SIZE 1
/**  */
#define UART_CR_FILTER_EN_DISABLE_VALUE 0
/**  */
#define UART_CR_FILTER_EN_ENABLE_VALUE 1
#define UART_CR_TX_EN_OFFSET 19
#define UART_CR_TX_EN_SIZE 1
/**  */
#define UART_CR_TX_EN_DISABLE_VALUE 0
/**  */
#define UART_CR_TX_EN_ENABLE_VALUE 1
#define UART_CR_RTS_EN_FF_OFFSET 20
#define UART_CR_RTS_EN_FF_SIZE 1
#define UART_CR_RTS_EN_HF_OFFSET 21
#define UART_CR_RTS_EN_HF_SIZE 1
#define UART_CR_RTS_EN_DC_OFFSET 22
#define UART_CR_RTS_EN_DC_SIZE 1
#define UART_CR_RTS_EN_DCN_OFFSET 23
#define UART_CR_RTS_EN_DCN_SIZE 1
#define UART_CR_RTS_EN_TH_OFFSET 24
#define UART_CR_RTS_EN_TH_SIZE 5
#define UART_CR_AUTOBAUD_EN_OFFSET 29
#define UART_CR_AUTOBAUD_EN_SIZE 1
#define UART_CR_CR_RXDMA_EN_OFFSET 30
#define UART_CR_CR_RXDMA_EN_SIZE 1
#define UART_CR_CR_TXDMA_EN_OFFSET 31
#define UART_CR_CR_TXDMA_EN_SIZE 1

/** bit field defines for uart_s#imrs */
#define UART_IMRS_RXFIFO_NOTEMPTY_INT_EN_OFFSET 0
#define UART_IMRS_RXFIFO_NOTEMPTY_INT_EN_SIZE 1
#define UART_IMRS_TX_NOT_FULL_INT_EN_OFFSET 1
#define UART_IMRS_TX_NOT_FULL_INT_EN_SIZE 1
#define UART_IMRS_TXFIFO_HALFEMPTY_INT_EN_OFFSET 2
#define UART_IMRS_TXFIFO_HALFEMPTY_INT_EN_SIZE 1
#define UART_IMRS_ERROR_PARITY_INT_EN_OFFSET 3
#define UART_IMRS_ERROR_PARITY_INT_EN_SIZE 1
#define UART_IMRS_ERROR_FRAME_INT_EN_OFFSET 4
#define UART_IMRS_ERROR_FRAME_INT_EN_SIZE 1
#define UART_IMRS_ERROR_OVERRUN_INT_EN_OFFSET 5
#define UART_IMRS_ERROR_OVERRUN_INT_EN_SIZE 1
#define UART_IMRS_TIMEOUT_NOTEMPTY_INT_EN_OFFSET 6
#define UART_IMRS_TIMEOUT_NOTEMPTY_INT_EN_SIZE 1
#define UART_IMRS_RXFIFO_HALFFULL_INT_EN_OFFSET 7
#define UART_IMRS_RXFIFO_HALFFULL_INT_EN_SIZE 1
#define UART_IMRS_RXFIFO_FULL_INT_EN_OFFSET 8
#define UART_IMRS_RXFIFO_FULL_INT_EN_SIZE 1
#define UART_IMRS_TX_END_INT_EN_OFFSET 9
#define UART_IMRS_TX_END_INT_EN_SIZE 1
#define UART_IMRS_RESERVED_OFFSET 10
#define UART_IMRS_RESERVED_SIZE 1
#define UART_IMRS_RXNOTEMPTY_INT_EN_OFFSET 11
#define UART_IMRS_RXNOTEMPTY_INT_EN_SIZE 1
#define UART_IMRS_RTS_INT_EN_OFFSET 12
#define UART_IMRS_RTS_INT_EN_SIZE 1
#define UART_IMRS_AUTOBAUD_INT_EN_OFFSET 13
#define UART_IMRS_AUTOBAUD_INT_EN_SIZE 1
#define UART_IMRS_AUTOBAUD_IDLE_INT_EN_OFFSET 14
#define UART_IMRS_AUTOBAUD_IDLE_INT_EN_SIZE 1
#define UART_IMRS_RXMATCH_INT_EN_OFFSET 27
#define UART_IMRS_RXMATCH_INT_EN_SIZE 1
#define UART_IMRS_RXBF_INT_EN_OFFSET 28
#define UART_IMRS_RXBF_INT_EN_SIZE 1
#define UART_IMRS_RXNBF_INT_EN_OFFSET 29
#define UART_IMRS_RXNBF_INT_EN_SIZE 1
#define UART_IMRS_TXBE_INT_EN_OFFSET 30
#define UART_IMRS_TXBE_INT_EN_SIZE 1
#define UART_IMRS_TXNBE_INT_EN_OFFSET 31
#define UART_IMRS_TXNBE_INT_EN_SIZE 1

/** bit field defines for uart_s#imrc */
#define UART_IMRC_RXFIFO_NOTEMPTY_INT_EN_OFFSET 0
#define UART_IMRC_RXFIFO_NOTEMPTY_INT_EN_SIZE 1
#define UART_IMRC_TX_NOT_FULL_INT_EN_OFFSET 1
#define UART_IMRC_TX_NOT_FULL_INT_EN_SIZE 1
#define UART_IMRC_TXFIFO_HALFEMPTY_INT_EN_OFFSET 2
#define UART_IMRC_TXFIFO_HALFEMPTY_INT_EN_SIZE 1
#define UART_IMRC_ERROR_PARITY_INT_EN_OFFSET 3
#define UART_IMRC_ERROR_PARITY_INT_EN_SIZE 1
#define UART_IMRC_ERROR_FRAME_INT_EN_OFFSET 4
#define UART_IMRC_ERROR_FRAME_INT_EN_SIZE 1
#define UART_IMRC_ERROR_OVERRUN_INT_EN_OFFSET 5
#define UART_IMRC_ERROR_OVERRUN_INT_EN_SIZE 1
#define UART_IMRC_TIMEOUT_NOTEMPTY_INT_EN_OFFSET 6
#define UART_IMRC_TIMEOUT_NOTEMPTY_INT_EN_SIZE 1
#define UART_IMRC_RXFIFO_HALFFULL_INT_EN_OFFSET 7
#define UART_IMRC_RXFIFO_HALFFULL_INT_EN_SIZE 1
#define UART_IMRC_RXFIFO_FULL_INT_EN_OFFSET 8
#define UART_IMRC_RXFIFO_FULL_INT_EN_SIZE 1
#define UART_IMRC_TX_END_INT_EN_OFFSET 9
#define UART_IMRC_TX_END_INT_EN_SIZE 1
#define UART_IMRC_RESERVED_OFFSET 10
#define UART_IMRC_RESERVED_SIZE 1
#define UART_IMRC_RXNOTEMPTY_INT_EN_OFFSET 11
#define UART_IMRC_RXNOTEMPTY_INT_EN_SIZE 1
#define UART_IMRC_RTS_INT_EN_OFFSET 12
#define UART_IMRC_RTS_INT_EN_SIZE 1
#define UART_IMRC_AUTOBAUD_INT_EN_OFFSET 13
#define UART_IMRC_AUTOBAUD_INT_EN_SIZE 1
#define UART_IMRC_AUTOBAUD_IDLE_INT_EN_OFFSET 14
#define UART_IMRC_AUTOBAUD_IDLE_INT_EN_SIZE 1
#define UART_IMRC_RXMATCH_INT_EN_OFFSET 27
#define UART_IMRC_RXMATCH_INT_EN_SIZE 1
#define UART_IMRC_RXBF_INT_EN_OFFSET 28
#define UART_IMRC_RXBF_INT_EN_SIZE 1
#define UART_IMRC_RXNBF_INT_EN_OFFSET 29
#define UART_IMRC_RXNBF_INT_EN_SIZE 1
#define UART_IMRC_TXBE_INT_EN_OFFSET 30
#define UART_IMRC_TXBE_INT_EN_SIZE 1
#define UART_IMRC_TXNBE_INT_EN_OFFSET 31
#define UART_IMRC_TXNBE_INT_EN_SIZE 1

/** bit field defines for uart_s#imr */
#define UART_IMR_RXFIFO_NOTEMPTY_INT_EN_OFFSET 0
#define UART_IMR_RXFIFO_NOTEMPTY_INT_EN_SIZE 1
#define UART_IMR_TX_NOT_FULL_INT_EN_OFFSET 1
#define UART_IMR_TX_NOT_FULL_INT_EN_SIZE 1
#define UART_IMR_TXFIFO_HALFEMPTY_INT_EN_OFFSET 2
#define UART_IMR_TXFIFO_HALFEMPTY_INT_EN_SIZE 1
#define UART_IMR_ERROR_PARITY_INT_EN_OFFSET 3
#define UART_IMR_ERROR_PARITY_INT_EN_SIZE 1
#define UART_IMR_ERROR_FRAME_INT_EN_OFFSET 4
#define UART_IMR_ERROR_FRAME_INT_EN_SIZE 1
#define UART_IMR_ERROR_OVERRUN_INT_EN_OFFSET 5
#define UART_IMR_ERROR_OVERRUN_INT_EN_SIZE 1
#define UART_IMR_TIMEOUT_NOTEMPTY_INT_EN_OFFSET 6
#define UART_IMR_TIMEOUT_NOTEMPTY_INT_EN_SIZE 1
#define UART_IMR_RXFIFO_HALFFULL_INT_EN_OFFSET 7
#define UART_IMR_RXFIFO_HALFFULL_INT_EN_SIZE 1
#define UART_IMR_RXFIFO_FULL_INT_EN_OFFSET 8
#define UART_IMR_RXFIFO_FULL_INT_EN_SIZE 1
#define UART_IMR_TX_END_INT_EN_OFFSET 9
#define UART_IMR_TX_END_INT_EN_SIZE 1
#define UART_IMR_RESERVED_OFFSET 10
#define UART_IMR_RESERVED_SIZE 1
#define UART_IMR_RXNOTEMPTY_INT_EN_OFFSET 11
#define UART_IMR_RXNOTEMPTY_INT_EN_SIZE 1
#define UART_IMR_RTS_INT_EN_OFFSET 12
#define UART_IMR_RTS_INT_EN_SIZE 1
#define UART_IMR_AUTOBAUD_INT_EN_OFFSET 13
#define UART_IMR_AUTOBAUD_INT_EN_SIZE 1
#define UART_IMR_AUTOBAUD_IDLE_INT_EN_OFFSET 14
#define UART_IMR_AUTOBAUD_IDLE_INT_EN_SIZE 1
#define UART_IMR_RXMATCH_INT_EN_OFFSET 27
#define UART_IMR_RXMATCH_INT_EN_SIZE 1
#define UART_IMR_RXBF_INT_EN_OFFSET 28
#define UART_IMR_RXBF_INT_EN_SIZE 1
#define UART_IMR_RXNBF_INT_EN_OFFSET 29
#define UART_IMR_RXNBF_INT_EN_SIZE 1
#define UART_IMR_TXBE_INT_EN_OFFSET 30
#define UART_IMR_TXBE_INT_EN_SIZE 1
#define UART_IMR_TXNBE_INT_EN_OFFSET 31
#define UART_IMR_TXNBE_INT_EN_SIZE 1

/** bit field defines for uart_s#sr */
#define UART_SR_RXFIFO_NOTEMPTY_OFFSET 0
#define UART_SR_RXFIFO_NOTEMPTY_SIZE 1
#define UART_SR_TX_EMPTY_OFFSET 1
#define UART_SR_TX_EMPTY_SIZE 1
#define UART_SR_TXFIFO_HALFEMPTY_OFFSET 2
#define UART_SR_TXFIFO_HALFEMPTY_SIZE 1
#define UART_SR_ERROR_PARITY_OFFSET 3
#define UART_SR_ERROR_PARITY_SIZE 1
#define UART_SR_ERROR_FRAME_OFFSET 4
#define UART_SR_ERROR_FRAME_SIZE 1
#define UART_SR_ERROR_OVERRUN_OFFSET 5
#define UART_SR_ERROR_OVERRUN_SIZE 1
#define UART_SR_TIMEOUT_NOTEMPTY_OFFSET 6
#define UART_SR_TIMEOUT_NOTEMPTY_SIZE 1
#define UART_SR_RXFIFO_HALFFULL_OFFSET 7
#define UART_SR_RXFIFO_HALFFULL_SIZE 1
#define UART_SR_RXFIFO_FULL_OFFSET 8
#define UART_SR_RXFIFO_FULL_SIZE 1
#define UART_SR_TX_END_OFFSET 9
#define UART_SR_TX_END_SIZE 1
#define UART_SR_TX_FIFO_FULL_OFFSET 10
#define UART_SR_TX_FIFO_FULL_SIZE 1
#define UART_SR_STICKY_RXNOTEMPTY_OFFSET 11
#define UART_SR_STICKY_RXNOTEMPTY_SIZE 1
#define UART_SR_RTS_STATUS_OFFSET 12
#define UART_SR_RTS_STATUS_SIZE 1
#define UART_SR_AUTOBAUD_COMPLETE_OFFSET 13
#define UART_SR_AUTOBAUD_COMPLETE_SIZE 1
#define UART_SR_AUTOBAUD_IDLE_OFFSET 14
#define UART_SR_AUTOBAUD_IDLE_SIZE 1
#define UART_SR_RXDMAACTIVE_OFFSET 25
#define UART_SR_RXDMAACTIVE_SIZE 1
#define UART_SR_TXDMAACTIVE_OFFSET 26
#define UART_SR_TXDMAACTIVE_SIZE 1
#define UART_SR_ISR_RXMATCH_OFFSET 27
#define UART_SR_ISR_RXMATCH_SIZE 1
#define UART_SR_ISR_RXBF_OFFSET 28
#define UART_SR_ISR_RXBF_SIZE 1
#define UART_SR_ISR_RXNBF_OFFSET 29
#define UART_SR_ISR_RXNBF_SIZE 1
#define UART_SR_ISR_TXBE_OFFSET 30
#define UART_SR_ISR_TXBE_SIZE 1
#define UART_SR_ISR_TXNBE_OFFSET 31
#define UART_SR_ISR_TXNBE_SIZE 1

/** bit field defines for uart_s#br */
#define UART_BR_BAUDRATE_OFFSET 0
#define UART_BR_BAUDRATE_SIZE 16
#define UART_BR_SAMPLING_MODE_OFFSET 24
#define UART_BR_SAMPLING_MODE_SIZE 2
/**  */
#define UART_BR_SAMPLING_MODE_16_VALUE 0
/**  */
#define UART_BR_SAMPLING_MODE_4_VALUE 2
/**  */
#define UART_BR_SAMPLING_MODE_8_VALUE 1
#define UART_BR_NCO_BYPASS_OFFSET 26
#define UART_BR_NCO_BYPASS_SIZE 1

/** bit field defines for uart_s#tor */
#define UART_TOR_TIMEOUT_OFFSET 0
#define UART_TOR_TIMEOUT_SIZE 8
#define UART_TOR_TIMEOUT_RX_DMA_OFFSET 16
#define UART_TOR_TIMEOUT_RX_DMA_SIZE 8

/** bit field defines for uart_s#tffr */
#define UART_TFFR_NONE_OFFSET 0
#define UART_TFFR_NONE_SIZE 1

/** bit field defines for uart_s#rffr */
#define UART_RFFR_NONE_OFFSET 0
#define UART_RFFR_NONE_SIZE 1

/** bit field defines for uart_s#twcr */
#define UART_TWCR_NONE_OFFSET 0
#define UART_TWCR_NONE_SIZE 2

/** bit field defines for uart_s#rwcr */
#define UART_RWCR_NONE_OFFSET 0
#define UART_RWCR_NONE_SIZE 2

/** bit field defines for uart_s#dst_ext */
#define UART_DST_EXT_DATAEXT_OFFSET 0
#define UART_DST_EXT_DATAEXT_SIZE 3
#define UART_DST_EXT_STEXT_OFFSET 3
#define UART_DST_EXT_STEXT_SIZE 3

/** bit field defines for uart_s#autobaud */
#define UART_AUTOBAUD_AUTOBAUD_BIT_CNT_OFFSET 0
#define UART_AUTOBAUD_AUTOBAUD_BIT_CNT_SIZE 5
#define UART_AUTOBAUD_AUTOBAUD_COMPLETE_OFFSET 5
#define UART_AUTOBAUD_AUTOBAUD_COMPLETE_SIZE 1
#define UART_AUTOBAUD_AUTOBAUD_STATUS_OFFSET 6
#define UART_AUTOBAUD_AUTOBAUD_STATUS_SIZE 1
#define UART_AUTOBAUD_AUTOBAUD_MBC_REACHED_OFFSET 7
#define UART_AUTOBAUD_AUTOBAUD_MBC_REACHED_SIZE 1
#define UART_AUTOBAUD_AUTOBAUD_MAX_BIT_CNT_OFFSET 8
#define UART_AUTOBAUD_AUTOBAUD_MAX_BIT_CNT_SIZE 10

/** bit field defines for uart_s#nco */
#define UART_NCO_NCO_PHASE_OFFSET 0
#define UART_NCO_NCO_PHASE_SIZE 8

/** bit field defines for uart_s#thr */
#define UART_THR_NONE_OFFSET 0
#define UART_THR_NONE_SIZE 8

/** bit field defines for uart_s#rhr */
#define UART_RHR_RHR_DATA_OFFSET 0
#define UART_RHR_RHR_DATA_SIZE 8
#define UART_RHR_PARITYERROR_OFFSET 8
#define UART_RHR_PARITYERROR_SIZE 1
#define UART_RHR_FRAMEERROR_OFFSET 9
#define UART_RHR_FRAMEERROR_SIZE 1

/** bit field defines for uart_s#ctxptr */
#define UART_CTXPTR_ADDR_OFFSET 0
#define UART_CTXPTR_ADDR_SIZE 15
#define UART_CTXPTR_ADDR_OFFSET_OFFSET 28
#define UART_CTXPTR_ADDR_OFFSET_SIZE 4

/** bit field defines for uart_s#ctxcnt */
#define UART_CTXCNT_NONE_OFFSET 0
#define UART_CTXCNT_NONE_SIZE 12

/** bit field defines for uart_s#ntxptr */
#define UART_NTXPTR_ADDR_OFFSET 0
#define UART_NTXPTR_ADDR_SIZE 15
#define UART_NTXPTR_ADDR_OFFSET_OFFSET 28
#define UART_NTXPTR_ADDR_OFFSET_SIZE 4

/** bit field defines for uart_s#ntxcnt */
#define UART_NTXCNT_NONE_OFFSET 0
#define UART_NTXCNT_NONE_SIZE 12

/** bit field defines for uart_s#crxptr */
#define UART_CRXPTR_ADDR_OFFSET 0
#define UART_CRXPTR_ADDR_SIZE 15
#define UART_CRXPTR_ADDR_OFFSET_OFFSET 28
#define UART_CRXPTR_ADDR_OFFSET_SIZE 4

/** bit field defines for uart_s#crxcnt */
#define UART_CRXCNT_NONE_OFFSET 0
#define UART_CRXCNT_NONE_SIZE 12

/** bit field defines for uart_s#nrxptr */
#define UART_NRXPTR_ADDR_OFFSET 0
#define UART_NRXPTR_ADDR_SIZE 15
#define UART_NRXPTR_ADDR_OFFSET_OFFSET 28
#define UART_NRXPTR_ADDR_OFFSET_SIZE 4

/** bit field defines for uart_s#nrxcnt */
#define UART_NRXCNT_NONE_OFFSET 0
#define UART_NRXCNT_NONE_SIZE 12

/** bit field defines for uart_s#rxcmp */
#define UART_RXCMP_NONE_OFFSET 0
#define UART_RXCMP_NONE_SIZE 8

/* EOF uart.h */
#endif
