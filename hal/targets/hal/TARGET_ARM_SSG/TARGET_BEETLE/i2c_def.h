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
 *                 I2C interface Support
 *                 =====================
 */

#ifndef MBED_I2C_DEF_H
#define MBED_I2C_DEF_H

#include <stdint.h>                      /* standard types definitions                      */

typedef struct beetle_i2c
{
    __IO uint32_t CONTROL;         /* RW Control register */
    __I  uint32_t STATUS;          /* RO Status register */
    __IO uint32_t ADDRESS;         /* RW I2C address register */
    __IO uint32_t DATA;            /* RW I2C data register */
    __IO uint32_t IRQ_STATUS;      /* RO Interrupt status register ( read only but write to clear bits) */
    __IO uint32_t TRANSFER_SIZE;   /* RW Transfer size register */
    __IO uint32_t SLAVE_MONITOR;   /* RW Slave monitor pause register */
    __IO uint32_t TIMEOUT;         /* RW Time out register */
    __I  uint32_t IRQ_MASK;        /* RO Interrupt mask register */
    __O  uint32_t IRQ_ENABLE;      /* WO Interrupt enable register */
    __O  uint32_t IRQ_DISABLE;     /* WO Interrupt disable register */

}I2C_TypeDef;

#define I2C0_BASE   (0x40007000ul)       /* Shield Header I2C Base Address */
#define I2C1_BASE   (0x4000E000ul)       /* Onboard I2C Base Address */

#define SHIELD_I2C        ((I2C_TypeDef      *) I2C0_BASE )
#define BOARD_I2C         ((I2C_TypeDef      *) I2C1_BASE )

/* Control Register Masks */
#define I2C_CTRL_RW             0x0001 /* Transfer direction */
#define I2C_CTRL_MS             0x0002 /* Mode (master / slave) */
#define I2C_CTRL_NEA            0x0004 /* Addressing mode */
#define I2C_CTRL_ACKEN          0x0008 /* ACK enable */
#define I2C_CTRL_HOLD           0x0010 /* Clock hold enable */
#define I2C_SLVMON              0x0020 /* Slave monitor mode */
#define I2C_CTRL_CLR_FIFO       0x0040 /* Force clear of FIFO */
#define I2C_CTRL_DIVISOR_B      0x3F00 /* Stage B clock divider */
#define I2C_CTRL_DIVISOR_A      0xA000 /* Stage A clock divider */
#define I2C_CTRL_DIVISORS       0xFF00 /* Combined A and B fields */
#define I2C_CTRL_DIVISOR_OFFSET 8      /* Offset of the clock divisor in
                                        * the CONTROL register
                                        */
#define I2C_CTRL_DIVISOR_A_BIT_MASK 0x03
                                       /*
                                        * First part of the clock
                                        * divisor in the CONTROL register
                                        */
#define I2C_CTRL_DIVISOR_B_BIT_MASK 0x3F
                                       /*
                                        * Second part of the clock
                                        * divisor in the CONTROL register
                                        */

/* Status Register Masks */
#define I2C_STATUS_RXRW         0x0008 /* Mode of transmission from master */
#define I2C_STATUS_RXDV         0x0020 /* Valid data waiting to be read */
#define I2C_STATUS_TXDV         0x0040 /* Still a data byte to be sent */
#define I2C_STATUS_RXOVF        0x0080 /* Receiver overflow */
#define I2C_STATUS_BA           0x0100 /* Bus active */

/* Address Register Masks */
#define I2C_ADDRESS_7BIT        0x007F

/* Interrupt Status / Enable / Disable Register Masks */
#define I2C_IRQ_COMP            0x0001 /* Transfer complete */
#define I2C_IRQ_DATA            0x0002 /* More data */
#define I2C_IRQ_NACK            0x0004 /* Transfer not acknowledged */
#define I2C_IRQ_TO              0x0008 /* Transfer timed out */
#define I2C_IRQ_SLV_RDY         0x0010 /* Monitored slave ready */
#define I2C_IRQ_RX_OVF          0x0020 /* Receive overflow */
#define I2C_IRQ_TX_OVF          0x0040 /* Transmit overflow */
#define I2C_IRQ_RX_UNF          0x0080 /* Receive underflow */
#define I2C_IRQ_ARB_LOST        0x0200 /* Arbitration lost */

/* Transfer Size Register Masks */
#define I2C_TRANSFER_SIZE       0xFF

/* Error codes */
#define E_SUCCESS               0x0
#define E_INCOMPLETE_DATA       0x1

#endif
