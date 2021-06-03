/*
 * Copyright 2021, Cypress Semiconductor Corporation (an Infineon company)
 * SPDX-License-Identifier: Apache-2.0
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

#ifndef INCLUDED_WHD_SPI_H_
#define INCLUDED_WHD_SPI_H_

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************
*             SPI Constants
******************************************************/
/* GSPI v1 */
#define SPI_FRAME_CONTROL              ( (uint32_t)0x1000D )

/* Register addresses */
#define SPI_BUS_CONTROL                ( (uint32_t)0x0000 )
#define SPI_RESPONSE_DELAY             ( (uint32_t)0x0001 )
#define SPI_STATUS_ENABLE              ( (uint32_t)0x0002 )
#define SPI_RESET_BP                   ( (uint32_t)0x0003 )    /* (corerev >= 1) */
#define SPI_INTERRUPT_REGISTER         ( (uint32_t)0x0004 )    /* 16 bits - Interrupt status */
#define SPI_INTERRUPT_ENABLE_REGISTER  ( (uint32_t)0x0006 )    /* 16 bits - Interrupt mask */
#define SPI_STATUS_REGISTER            ( (uint32_t)0x0008 )    /* 32 bits */
#define SPI_FUNCTION1_INFO             ( (uint32_t)0x000C )    /* 16 bits */
#define SPI_FUNCTION2_INFO             ( (uint32_t)0x000E )    /* 16 bits */
#define SPI_FUNCTION3_INFO             ( (uint32_t)0x0010 )    /* 16 bits */
#define SPI_READ_TEST_REGISTER         ( (uint32_t)0x0014 )    /* 32 bits */
#define SPI_RESP_DELAY_F0              ( (uint32_t)0x001c )    /* 8 bits (corerev >= 3) */
#define SPI_RESP_DELAY_F1              ( (uint32_t)0x001d )    /* 8 bits (corerev >= 3) */
#define SPI_RESP_DELAY_F2              ( (uint32_t)0x001e )    /* 8 bits (corerev >= 3) */
#define SPI_RESP_DELAY_F3              ( (uint32_t)0x001f )    /* 8 bits (corerev >= 3) */

/******************************************************
*             Bit Masks
******************************************************/

/* GSPI */
#define SPI_READ_TEST_REGISTER_VALUE   ( (uint32_t)0xFEEDBEAD )
#define SPI_READ_TEST_REG_LSB          ( ( (SPI_READ_TEST_REGISTER_VALUE) ) & 0xff )
#define SPI_READ_TEST_REG_LSB_SFT1     ( ( (SPI_READ_TEST_REGISTER_VALUE << 1) ) & 0xff )
#define SPI_READ_TEST_REG_LSB_SFT2     ( ( (SPI_READ_TEST_REGISTER_VALUE << 1) + 1 ) & 0xff )
#define SPI_READ_TEST_REG_LSB_SFT3     ( ( (SPI_READ_TEST_REGISTER_VALUE + 1) << 1 ) & 0xff )

/* SPI_BUS_CONTROL Bits */
#define WORD_LENGTH_32             ( (uint32_t)0x01 )     /* 0/1 16/32 bit word length */
#define ENDIAN_BIG                 ( (uint32_t)0x02 )     /* 0/1 Little/Big Endian */
#define CLOCK_PHASE                ( (uint32_t)0x04 )     /* 0/1 clock phase delay */
#define CLOCK_POLARITY             ( (uint32_t)0x08 )     /* 0/1 Idle state clock polarity is low/high */
#define HIGH_SPEED_MODE            ( (uint32_t)0x10 )     /* 1/0 High Speed mode / Normal mode */
#define INTERRUPT_POLARITY_HIGH    ( (uint32_t)0x20 )     /* 1/0 Interrupt active polarity is high/low */
#define WAKE_UP                    ( (uint32_t)0x80 )     /* 0/1 Wake-up command from Host to WLAN */

/* SPI_RESPONSE_DELAY Bit mask */
#define RESPONSE_DELAY_MASK 0xFF   /* Configurable rd response delay in multiples of 8 bits */

/* SPI_STATUS_ENABLE Bits */
#define STATUS_ENABLE              ( (uint32_t)0x01 )     /* 1/0 Status sent/not sent to host after read/write */
#define INTR_WITH_STATUS           ( (uint32_t)0x02 )     /* 0/1 Do-not / do-interrupt if status is sent */
#define RESP_DELAY_ALL             ( (uint32_t)0x04 )     /* Applicability of resp delay to F1 or all func's read */
#define DWORD_PKT_LEN_EN           ( (uint32_t)0x08 )     /* Packet len denoted in dwords instead of bytes */
#define CMD_ERR_CHK_EN             ( (uint32_t)0x20 )     /* Command error check enable */
#define DATA_ERR_CHK_EN            ( (uint32_t)0x40 )     /* Data error check enable */

/* SPI_RESET_BP Bits*/
#define RESET_ON_WLAN_BP_RESET     ( (uint32_t)0x04 )     /* enable reset for WLAN backplane */
#define RESET_SPI                  ( (uint32_t)0x80 )     /* reset the above enabled logic */

/* SPI_INTERRUPT_REGISTER and SPI_INTERRUPT_ENABLE_REGISTER Bits */
#define DATA_UNAVAILABLE           ( (uint32_t)0x0001 )   /* Requested data not available; Clear by writing a "1" */
#define F2_F3_FIFO_RD_UNDERFLOW    ( (uint32_t)0x0002 )
#define F2_F3_FIFO_WR_OVERFLOW     ( (uint32_t)0x0004 )
#define COMMAND_ERROR              ( (uint32_t)0x0008 )   /* Cleared by writing 1 */
#define DATA_ERROR                 ( (uint32_t)0x0010 )   /* Cleared by writing 1 */
#define F2_PACKET_AVAILABLE        ( (uint32_t)0x0020 )
#define F3_PACKET_AVAILABLE        ( (uint32_t)0x0040 )
#define F1_OVERFLOW                ( (uint32_t)0x0080 )   /* Due to last write. Bkplane has pending write requests */
#define MISC_INTR0                 ( (uint32_t)0x0100 )
#define MISC_INTR1                 ( (uint32_t)0x0200 )
#define MISC_INTR2                 ( (uint32_t)0x0400 )
#define MISC_INTR3                 ( (uint32_t)0x0800 )
#define MISC_INTR4                 ( (uint32_t)0x1000 )
#define F1_INTR                    ( (uint32_t)0x2000 )
#define F2_INTR                    ( (uint32_t)0x4000 )
#define F3_INTR                    ( (uint32_t)0x8000 )

/* SPI_STATUS_REGISTER Bits */
#define STATUS_DATA_NOT_AVAILABLE  ( (uint32_t)0x00000001 )
#define STATUS_UNDERFLOW           ( (uint32_t)0x00000002 )
#define STATUS_OVERFLOW            ( (uint32_t)0x00000004 )
#define STATUS_F2_INTR             ( (uint32_t)0x00000008 )
#define STATUS_F3_INTR             ( (uint32_t)0x00000010 )
#define STATUS_F2_RX_READY         ( (uint32_t)0x00000020 )
#define STATUS_F3_RX_READY         ( (uint32_t)0x00000040 )
#define STATUS_HOST_CMD_DATA_ERR   ( (uint32_t)0x00000080 )
#define STATUS_F2_PKT_AVAILABLE    ( (uint32_t)0x00000100 )
#define STATUS_F2_PKT_LEN_MASK     ( (uint32_t)0x000FFE00 )
#define STATUS_F2_PKT_LEN_SHIFT    ( (uint32_t)9 )
#define STATUS_F3_PKT_AVAILABLE    ( (uint32_t)0x00100000 )
#define STATUS_F3_PKT_LEN_MASK     ( (uint32_t)0xFFE00000 )
#define STATUS_F3_PKT_LEN_SHIFT    ( (uint32_t)21 )

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ifndef INCLUDED_WHD_SPI_H_ */

