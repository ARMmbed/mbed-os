/* mbed Microcontroller Library
 * Copyright (c) 2015 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/*
 *                 SSP interface Support
 *                 =====================
 */

#ifndef MBED_SPI_DEF_H
#define MBED_SPI_DEF_H

#include <stdint.h>                      /* standard types definitions                      */

#define Module_ID 0x00090108

typedef struct beetle_spi
{
    __IO uint32_t CONFIG;            /* 0x00 RW Configuration Register */
    __I  uint32_t IRQ_STATUS;        /* 0x04 RO Interrupt Status Register*/
    __O  uint32_t IRQ_ENABLE;        /* 0x08 WO Interrupt Enable Register*/
    __O  uint32_t IRQ_DISABLE;       /* 0x0C WO Interrupt Disable Register */
    __I  uint32_t IRQ_MASK;          /* 0x10 RO Interrupt Mask Register */
    __IO uint32_t SPI_ENABLE;        /* 0x14 RW SPI Enable Register */
    __IO uint32_t DELAY;             /* 0x18 RW Delay Register */
    __O  uint32_t TX_DATA;           /* 0x1C WO Transmit Data Register */
    __I  uint32_t RX_DATA;           /* 0x20 RO Receive Data Register */
    __IO uint32_t SLAVE_IDLE_COUNT;  /* 0x24 RW Slave Idle Count Register */
    __IO uint32_t TX_THRESHOLD;      /* 0x28 RW TX Threshold Register */
    __IO uint32_t RX_THRESHOLD;      /* 0x2C RW RX Threshold Register */
         uint32_t reserved[208];
    __I  uint32_t MID;               /* 0xFC RO Module ID Register */
}SPI_TypeDef;


#define SPI0_BASE   (0x4000C000ul)       /* Shield Header SPI Base Address */
#define SPI1_BASE   (0x4000D000ul)       /* ADC SPI Base Address */

#define SHIELD_SPI        ((SPI_TypeDef      *) SPI0_BASE )
#define ADC_SPI           ((SPI_TypeDef      *) SPI1_BASE )

/* Configuration Register Bit Masks */
#define CONFIG_MSEL     0x00001     // Bit [00] MSEL Mode Select
#define CONFIG_CPOL     0x00002     // Bit [01] CPOL External Clock Edge
#define CONFIG_CPHA     0x00004     // Bit [02] CPHA Clock Phase
#define CONFIG_MBRD     0x00038     // Bits [05:03] MBRD Master Baud Rate Divisor (2 to 256)
#define CONFIG_MBRD_0   0x00008
#define CONFIG_MBRD_1   0x00010
#define CONFIG_MBRD_2   0x00020
#define CONFIG_MBRD_SHIFT 3
#define CONFIG_TWS      0x000C0     // Bits [07:06] TWS Transfer Word Size
#define CONFIG_TWS_0    0x00000
#define CONFIG_TWS_1    0x00040
#define CONFIG_MRCS     0x00100     // Bit [08] MRCS Reference Clock Select
#define CONFIG_PSD      0x00200     // Bit [09] PSD Peripheral Select Decode
#define CONFIG_PCSL     0x03C00     // Bits [13:10] PCSL Peripheral Chip Select Lines (master mode only)
#define CONFIG_MCSE     0x04000     // Bit [14] MCSE Manual Chip Select Enable
#define CONFIG_MSE      0x08000     // Bit [15] MSE Manual Start Enable
#define CONFIG_MSC      0x10000     // Bit [16] MSC Manual Start Command
#define CONFIG_MFGE     0x20000     // Bit [17] MFGE Mode Fail Generation Enable
#define CONFIG_SPSE     0x40000     // Bit [18] SPSE Sample Point Shift Enable

/* Interrupt Status Register Bit Masks */
#define IRQ_STATUS_ROF  0x01        // Bit [00] ROF RX FIFO Overflow
#define IRQ_STATUS_MF   0x02        // Bit [01] MF Mode Fail
#define IRQ_STATUS_TNF  0x04        // Bit [02] TNF TX FIFO Not Full (current FIFO status)
#define IRQ_STATUS_TF   0x08        // Bit [03] TF TX FIFO Full (current FIFO status)
#define IRQ_STATUS_RNE  0x10        // Bit [04] RNE RX FIFO Not Empty (current FIFO status)
#define IRQ_STATUS_RF   0x20        // Bit [05] RF RX FIFO Full (current FIFO status)
#define IRQ_STATUS_TUF  0x40        // Bit [06] TUF TX FIFO Underflow

/* Interrupt Enable Register Bit Masks */
#define IRQ_ENABLE_ROFE  0x01       // Bit [00] ROFE RX FIFO Overflow Enable
#define IRQ_ENABLE_MFE   0x02       // Bit [01] MFE Mode Fail Enable
#define IRQ_ENABLE_TNFE  0x04       // Bit [02] TNFE TX FIFO Not Full Enable
#define IRQ_ENABLE_TFE   0x08       // Bit [03] TFE TX FIFO Full Enable
#define IRQ_ENABLE_RNEE  0x10       // Bit [04] RNEE RX FIFO Not Empty Enable
#define IRQ_ENABLE_RFE   0x20       // Bit [05] RFE RX FIFO Full Enable
#define IRQ_ENABLE_TUFE  0x40       // Bit [06] TUFE TX FIFO Underflow Enable

/* Interrupt Disable Register Bit Masks */
#define IRQ_DISABLE_ROFD  0x01      // Bit [00] ROFD RX FIFO Overflow Disable
#define IRQ_DISABLE_MFD   0x02      // Bit [01] MFD Mode Fail Disable
#define IRQ_DISABLE_TNFD  0x04      // Bit [02] TNFD TX FIFO Not Full Disable
#define IRQ_DISABLE_TFD   0x08      // Bit [03] TFD TX FIFO Full Disable
#define IRQ_DISABLE_RNED  0x10      // Bit [04] RNED RX FIFO Not Empty Disable
#define IRQ_DISABLE_RFD   0x20      // Bit [05] RFD RX FIFO Full Disable
#define IRQ_DISABLE_TUFD  0x40      // Bit [06] TUFD TX FIFO Underflow Disable

/* Interrupt Mask Register Bit Masks */
#define IRQ_MASK_ROFM  0x01         // Bit [00] ROFM RX FIFO Overflow Mask
#define IRQ_MASK_MFM   0x02         // Bit [01] MFM Mode Fail Mask
#define IRQ_MASK_TNFM  0x04         // Bit [02] TNFM TX FIFO Not Full Mask
#define IRQ_MASK_TFM   0x08         // Bit [03] TFM TX FIFO Full Mask
#define IRQ_MASK_RNEM  0x10         // Bit [04] RNEM RX FIFO Not Empty Mask
#define IRQ_MASK_RFM   0x20         // Bit [05] RFM RX FIFO Full Mask
#define IRQ_MASK_TUFM  0x40         // Bit [06] TUFM TX FIFO Underflow Mask

/* SPI Enable Register Bit Masks */
#define SPI_ENABLE_SPIE 0x01        // Bit [00] SPIE SPI Enable

/* Delay Register Bit Masks */
#define DELAY_D_INIT    0x000000FF  // Bits [07:00] D_INIT Delay Init
#define DELAY_D_AFTER   0x0000FF00  // Bits [15:08] D_AFTER Delay After
#define DELAY_D_BTWN    0x00FF0000  // Bits [23:16] D_BTWN Delay Between
#define DELAY_D_NSS     0xFF000000  // Bits [31:24] D_NSS Delay NSS

/* Transmit Data Register Bit Masks */
#define TX_DATA_TDATA   0xFF

/* Receive Data Register Bit Masks */
#define RX_DATA_RDATA   0xFF

/* Slave Idle Count Register Bit Masks */
#define SLAVE_IDLE_COUNT_SICNT 0xFF // Bits [07:00] SICNT Slave Idle Count

/* TX Threshold Register Bit Masks */
#define TX_THRESHOLD_TTRSH  0x07    // Bits [N:00] TTRSH TX Threshold

/* RX Threshold Register Bit Masks */
#define RX_THRESHOLD_RTRSH  0x07    // Bits [N:00] RTRSH RX Threshold

#endif
