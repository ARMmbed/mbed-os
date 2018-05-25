#ifndef INCLUDED_I2C
#define INCLUDED_I2C
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>


/** Here comes the decription of this IP component
*/
struct i2c_ss {
   /** Control register (set bit) at address offset 0x000, read-write */
   uint32_t crset;
   /** Control register (clear bit) at address offset 0x004, read-write */
   uint32_t crclear;
   /** Control register at address offset 0x008, read-write */
   uint32_t cr;
   /** Reserved space */
   uint8_t fill0[4];
   /** Interrupt enable register (set bit) at address offset 0x010, read-write */
   uint32_t intenableset;
   /** Interrupt enable register (clear bit) at address offset 0x014, read-write */
   uint32_t intenableclear;
   /** Interrupt Enable Register at address offset 0x018, read-write */
   uint32_t intenable;
   /** Reserved space */
   uint8_t fill1[4];
   /** Status Register at address offset 0x020, read-write */
   uint32_t sr;
   /** Own Address Register at address offset 0x024, read-write */
   uint32_t oar;
   /** Clock Control Register at address offset 0x028, read-write */
   uint32_t ccr;
   /** Transmit FIFO flush at address offset 0x02C, read-write */
   uint32_t txflush;
   /** Receive FIFO flush at address offset 0x030, read-write */
   uint32_t rxflush;
   /** Transmit FIFO word count at address offset 0x034, read-only */
   uint32_t txwordcount;
   /** Receive FIFO word count at address offset 0x038, read-only */
   uint32_t rxwordcount;
   /** Number received bytes in MST mode at address offset 0x03C, read-write */
   uint32_t nrbr;
   /** Reserved space */
   uint8_t fill2[64];
   /** Transmit data buffer at address offset 0x080, write-only */
   uint32_t txbuffer;
   /** Reserved space */
   uint8_t fill3[124];
   /** Receive data buffer at address offset 0x100, read-only */
   uint32_t rxbuffer;
};

/** bit field defines for i2c_s#crset */
#define I2C_CRSET_ITE_OFFSET 0
#define I2C_CRSET_ITE_SIZE 1
#define I2C_CRSET_STOP_OFFSET 1
#define I2C_CRSET_STOP_SIZE 1
#define I2C_CRSET_ACK_OFFSET 2
#define I2C_CRSET_ACK_SIZE 1
#define I2C_CRSET_START_OFFSET 3
#define I2C_CRSET_START_SIZE 1
#define I2C_CRSET_GC_OFFSET 4
#define I2C_CRSET_GC_SIZE 1
#define I2C_CRSET_INSYN_OFFSET 5
#define I2C_CRSET_INSYN_SIZE 1

/** bit field defines for i2c_s#crclear */
#define I2C_CRCLEAR_ITE_OFFSET 0
#define I2C_CRCLEAR_ITE_SIZE 1
#define I2C_CRCLEAR_STOP_OFFSET 1
#define I2C_CRCLEAR_STOP_SIZE 1
#define I2C_CRCLEAR_ACK_OFFSET 2
#define I2C_CRCLEAR_ACK_SIZE 1
#define I2C_CRCLEAR_START_OFFSET 3
#define I2C_CRCLEAR_START_SIZE 1
#define I2C_CRCLEAR_GC_OFFSET 4
#define I2C_CRCLEAR_GC_SIZE 1
#define I2C_CRCLEAR_INSYN_OFFSET 5
#define I2C_CRCLEAR_INSYN_SIZE 1

/** bit field defines for i2c_s#cr */
#define I2C_CR_ITE_OFFSET 0
#define I2C_CR_ITE_SIZE 1
#define I2C_CR_STOP_OFFSET 1
#define I2C_CR_STOP_SIZE 1
#define I2C_CR_ACK_OFFSET 2
#define I2C_CR_ACK_SIZE 1
#define I2C_CR_START_OFFSET 3
#define I2C_CR_START_SIZE 1
#define I2C_CR_GC_OFFSET 4
#define I2C_CR_GC_SIZE 1
#define I2C_CR_INSYN_OFFSET 5
#define I2C_CR_INSYN_SIZE 1

/** bit field defines for i2c_s#intenableset */
#define I2C_INTENABLESET_SB_OFFSET 0
#define I2C_INTENABLESET_SB_SIZE 1
#define I2C_INTENABLESET_MSL_OFFSET 1
#define I2C_INTENABLESET_MSL_SIZE 1
#define I2C_INTENABLESET_ADSL_OFFSET 2
#define I2C_INTENABLESET_ADSL_SIZE 1
#define I2C_INTENABLESET_BTF_OFFSET 3
#define I2C_INTENABLESET_BTF_SIZE 1
#define I2C_INTENABLESET_BUSY_OFFSET 4
#define I2C_INTENABLESET_BUSY_SIZE 1
#define I2C_INTENABLESET_TRA_OFFSET 5
#define I2C_INTENABLESET_TRA_SIZE 1
#define I2C_INTENABLESET_ADD10_OFFSET 6
#define I2C_INTENABLESET_ADD10_SIZE 1
#define I2C_INTENABLESET_GC_OFFSET 7
#define I2C_INTENABLESET_GC_SIZE 1
#define I2C_INTENABLESET_BERR_OFFSET 8
#define I2C_INTENABLESET_BERR_SIZE 1
#define I2C_INTENABLESET_ARLO_OFFSET 9
#define I2C_INTENABLESET_ARLO_SIZE 1
#define I2C_INTENABLESET_STOPF_OFFSET 10
#define I2C_INTENABLESET_STOPF_SIZE 1
#define I2C_INTENABLESET_AF_OFFSET 11
#define I2C_INTENABLESET_AF_SIZE 1
#define I2C_INTENABLESET_ENDAD_OFFSET 12
#define I2C_INTENABLESET_ENDAD_SIZE 1
#define I2C_INTENABLESET_RXNOTEMPTY_OFFSET 13
#define I2C_INTENABLESET_RXNOTEMPTY_SIZE 1
#define I2C_INTENABLESET_RXHALFFULL_OFFSET 14
#define I2C_INTENABLESET_RXHALFFULL_SIZE 1
#define I2C_INTENABLESET_RXFULL_OFFSET 15
#define I2C_INTENABLESET_RXFULL_SIZE 1
#define I2C_INTENABLESET_TXEMPTY_OFFSET 16
#define I2C_INTENABLESET_TXEMPTY_SIZE 1
#define I2C_INTENABLESET_TXHALFFULL_OFFSET 17
#define I2C_INTENABLESET_TXHALFFULL_SIZE 1
#define I2C_INTENABLESET_TXFULL_OFFSET 18
#define I2C_INTENABLESET_TXFULL_SIZE 1
#define I2C_INTENABLESET_NRB_OFFSET 19
#define I2C_INTENABLESET_NRB_SIZE 1

/** bit field defines for i2c_s#intenableclear */
#define I2C_INTENABLECLEAR_SB_OFFSET 0
#define I2C_INTENABLECLEAR_SB_SIZE 1
#define I2C_INTENABLECLEAR_MSL_OFFSET 1
#define I2C_INTENABLECLEAR_MSL_SIZE 1
#define I2C_INTENABLECLEAR_ADSL_OFFSET 2
#define I2C_INTENABLECLEAR_ADSL_SIZE 1
#define I2C_INTENABLECLEAR_BTF_OFFSET 3
#define I2C_INTENABLECLEAR_BTF_SIZE 1
#define I2C_INTENABLECLEAR_BUSY_OFFSET 4
#define I2C_INTENABLECLEAR_BUSY_SIZE 1
#define I2C_INTENABLECLEAR_TRA_OFFSET 5
#define I2C_INTENABLECLEAR_TRA_SIZE 1
#define I2C_INTENABLECLEAR_ADD10_OFFSET 6
#define I2C_INTENABLECLEAR_ADD10_SIZE 1
#define I2C_INTENABLECLEAR_GC_OFFSET 7
#define I2C_INTENABLECLEAR_GC_SIZE 1
#define I2C_INTENABLECLEAR_BERR_OFFSET 8
#define I2C_INTENABLECLEAR_BERR_SIZE 1
#define I2C_INTENABLECLEAR_ARLO_OFFSET 9
#define I2C_INTENABLECLEAR_ARLO_SIZE 1
#define I2C_INTENABLECLEAR_STOPF_OFFSET 10
#define I2C_INTENABLECLEAR_STOPF_SIZE 1
#define I2C_INTENABLECLEAR_AF_OFFSET 11
#define I2C_INTENABLECLEAR_AF_SIZE 1
#define I2C_INTENABLECLEAR_ENDAD_OFFSET 12
#define I2C_INTENABLECLEAR_ENDAD_SIZE 1
#define I2C_INTENABLECLEAR_RXNOTEMPTY_OFFSET 13
#define I2C_INTENABLECLEAR_RXNOTEMPTY_SIZE 1
#define I2C_INTENABLECLEAR_RXHALFFULL_OFFSET 14
#define I2C_INTENABLECLEAR_RXHALFFULL_SIZE 1
#define I2C_INTENABLECLEAR_RXFULL_OFFSET 15
#define I2C_INTENABLECLEAR_RXFULL_SIZE 1
#define I2C_INTENABLECLEAR_TXEMPTY_OFFSET 16
#define I2C_INTENABLECLEAR_TXEMPTY_SIZE 1
#define I2C_INTENABLECLEAR_TXHALFFULL_OFFSET 17
#define I2C_INTENABLECLEAR_TXHALFFULL_SIZE 1
#define I2C_INTENABLECLEAR_TXFULL_OFFSET 18
#define I2C_INTENABLECLEAR_TXFULL_SIZE 1
#define I2C_INTENABLECLEAR_NRB_OFFSET 19
#define I2C_INTENABLECLEAR_NRB_SIZE 1

/** bit field defines for i2c_s#intenable */
#define I2C_INTENABLE_SB_OFFSET 0
#define I2C_INTENABLE_SB_SIZE 1
#define I2C_INTENABLE_MSL_OFFSET 1
#define I2C_INTENABLE_MSL_SIZE 1
#define I2C_INTENABLE_ADSL_OFFSET 2
#define I2C_INTENABLE_ADSL_SIZE 1
#define I2C_INTENABLE_BTF_OFFSET 3
#define I2C_INTENABLE_BTF_SIZE 1
#define I2C_INTENABLE_BUSY_OFFSET 4
#define I2C_INTENABLE_BUSY_SIZE 1
#define I2C_INTENABLE_TRA_OFFSET 5
#define I2C_INTENABLE_TRA_SIZE 1
#define I2C_INTENABLE_ADD10_OFFSET 6
#define I2C_INTENABLE_ADD10_SIZE 1
#define I2C_INTENABLE_GC_OFFSET 7
#define I2C_INTENABLE_GC_SIZE 1
#define I2C_INTENABLE_BERR_OFFSET 8
#define I2C_INTENABLE_BERR_SIZE 1
#define I2C_INTENABLE_ARLO_OFFSET 9
#define I2C_INTENABLE_ARLO_SIZE 1
#define I2C_INTENABLE_STOPF_OFFSET 10
#define I2C_INTENABLE_STOPF_SIZE 1
#define I2C_INTENABLE_AF_OFFSET 11
#define I2C_INTENABLE_AF_SIZE 1
#define I2C_INTENABLE_ENDAD_OFFSET 12
#define I2C_INTENABLE_ENDAD_SIZE 1
#define I2C_INTENABLE_RXNOTEMPTY_OFFSET 13
#define I2C_INTENABLE_RXNOTEMPTY_SIZE 1
#define I2C_INTENABLE_RXHALFFULL_OFFSET 14
#define I2C_INTENABLE_RXHALFFULL_SIZE 1
#define I2C_INTENABLE_RXFULL_OFFSET 15
#define I2C_INTENABLE_RXFULL_SIZE 1
#define I2C_INTENABLE_TXEMPTY_OFFSET 16
#define I2C_INTENABLE_TXEMPTY_SIZE 1
#define I2C_INTENABLE_TXHALFFULL_OFFSET 17
#define I2C_INTENABLE_TXHALFFULL_SIZE 1
#define I2C_INTENABLE_TXFULL_OFFSET 18
#define I2C_INTENABLE_TXFULL_SIZE 1
#define I2C_INTENABLE_NRB_OFFSET 19
#define I2C_INTENABLE_NRB_SIZE 1

/** bit field defines for i2c_s#sr */
#define I2C_SR_SB_OFFSET 0
#define I2C_SR_SB_SIZE 1
#define I2C_SR_MSL_OFFSET 1
#define I2C_SR_MSL_SIZE 1
#define I2C_SR_ADSL_OFFSET 2
#define I2C_SR_ADSL_SIZE 1
#define I2C_SR_BTF_OFFSET 3
#define I2C_SR_BTF_SIZE 1
#define I2C_SR_BUSY_OFFSET 4
#define I2C_SR_BUSY_SIZE 1
#define I2C_SR_TRA_OFFSET 5
#define I2C_SR_TRA_SIZE 1
#define I2C_SR_ADD10_OFFSET 6
#define I2C_SR_ADD10_SIZE 1
#define I2C_SR_GC_OFFSET 7
#define I2C_SR_GC_SIZE 1
#define I2C_SR_BERR_OFFSET 8
#define I2C_SR_BERR_SIZE 1
#define I2C_SR_ARLO_OFFSET 9
#define I2C_SR_ARLO_SIZE 1
#define I2C_SR_STOPF_OFFSET 10
#define I2C_SR_STOPF_SIZE 1
#define I2C_SR_AF_OFFSET 11
#define I2C_SR_AF_SIZE 1
#define I2C_SR_ENDAD_OFFSET 12
#define I2C_SR_ENDAD_SIZE 1
#define I2C_SR_RXNOTEMPTY_OFFSET 13
#define I2C_SR_RXNOTEMPTY_SIZE 1
#define I2C_SR_RXHALFFULL_OFFSET 14
#define I2C_SR_RXHALFFULL_SIZE 1
#define I2C_SR_RXFULL_OFFSET 15
#define I2C_SR_RXFULL_SIZE 1
#define I2C_SR_TXEMPTY_OFFSET 16
#define I2C_SR_TXEMPTY_SIZE 1
#define I2C_SR_TXHALFFULL_OFFSET 17
#define I2C_SR_TXHALFFULL_SIZE 1
#define I2C_SR_TXFULL_OFFSET 18
#define I2C_SR_TXFULL_SIZE 1
#define I2C_SR_NRB_OFFSET 19
#define I2C_SR_NRB_SIZE 1

/** bit field defines for i2c_s#oar */
#define I2C_OAR_ADD_OFFSET 0
#define I2C_OAR_ADD_SIZE 10
#define I2C_OAR_OAM_OFFSET 10
#define I2C_OAR_OAM_SIZE 1
#define I2C_OAR_FR_OFFSET 11
#define I2C_OAR_FR_SIZE 3

/** bit field defines for i2c_s#ccr */
#define I2C_CCR_CC_OFFSET 0
#define I2C_CCR_CC_SIZE 12
#define I2C_CCR_FM_SM_OFFSET 12
#define I2C_CCR_FM_SM_SIZE 1

/** bit field defines for i2c_s#txflush */
#define I2C_TXFLUSH_TFFR_OFFSET 0
#define I2C_TXFLUSH_TFFR_SIZE 1

/** bit field defines for i2c_s#rxflush */
#define I2C_RXFLUSH_RFFR_OFFSET 0
#define I2C_RXFLUSH_RFFR_SIZE 1

/** bit field defines for i2c_s#txwordcount */
#define I2C_TXWORDCOUNT_TXWORDCOUNT_OFFSET 0
#define I2C_TXWORDCOUNT_TXWORDCOUNT_SIZE 5

/** bit field defines for i2c_s#rxwordcount */
#define I2C_RXWORDCOUNT_RXWORDCOUNT_OFFSET 0
#define I2C_RXWORDCOUNT_RXWORDCOUNT_SIZE 5

/** bit field defines for i2c_s#nrbr */
#define I2C_NRBR_NRB_OFFSET 0
#define I2C_NRBR_NRB_SIZE 8

/** bit field defines for i2c_s#txbuffer */
#define I2C_TXBUFFER_TXDATA_OFFSET 0
#define I2C_TXBUFFER_TXDATA_SIZE 8

/** bit field defines for i2c_s#rxbuffer */
#define I2C_RXBUFFER_RXDATA_OFFSET 0
#define I2C_RXBUFFER_RXDATA_SIZE 8

/* EOF i2c.h */
#endif
