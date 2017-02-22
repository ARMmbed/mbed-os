/**
 ******************************************************************************
 * @file    i2c.h
 * @brief    (API) Public header of i2c driver
 * @internal
 * @author   ON Semiconductor
 * $Rev:  $
 * $Date: 2016-04-20 $
 ******************************************************************************
 * Copyright 2016 Semiconductor Components Industries LLC (d/b/a “ON Semiconductor”).
 * All rights reserved.  This software and/or documentation is licensed by ON Semiconductor
 * under limited terms and conditions.  The terms and conditions pertaining to the software
 * and/or documentation are available at http://www.onsemi.com/site/pdf/ONSEMI_T&C.pdf
 * (“ON Semiconductor Standard Terms and Conditions of Sale, Section 8 Software”) and
 * if applicable the software license agreement.  Do not use this software and/or
 * documentation unless you have carefully read and you agree to the limited terms and
 * conditions.  By using this software and/or documentation, you agree to the limited
 * terms and conditions.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ON SEMICONDUCTOR SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
 * INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 * @endinternal
 *
 * @ingroup i2c
 */

#include "mbed_assert.h"
#include "i2c_api.h"
#include "clock.h"
#include "i2c_ipc7208_map.h"
#include "memory_map.h"
#include "PeripheralPins.h"

#ifndef I2C_H_
#define    I2C_H_

/* Miscellaneous I/O and control operations codes */
#define I2C_IPC7208_IOCTL_NOT_ACK        0x03
#define I2C_IPC7208_IOCTL_NULL_CMD       0x04
#define I2C_IPC7208_IOCTL_ACK            0x05

/* Definitions for the clock speed. */
#define I2C_SPEED_100K_AT_8MHZ     (uint8_t)0x12
#define I2C_SPEED_100K_AT_16MHZ    (uint8_t)0x26
#define I2C_SPEED_400K_AT_8MHZ     (uint8_t)0x03
#define I2C_SPEED_400K_AT_16MHZ    (uint8_t)0x08


/* I2C commands */
#define I2C_CMD_NULL          0x00
#define I2C_CMD_WDAT0         0x10
#define I2C_CMD_WDAT1         0x11
#define I2C_CMD_WDAT8         0x12
#define I2C_CMD_RDAT8         0x13
#define I2C_CMD_STOP          0x14
#define I2C_CMD_START         0x15
#define I2C_CMD_VRFY_ACK      0x16
#define I2C_CMD_VRFY_VACK     0x17

/* Status register bits */
#define I2C_STATUS_CMD_FIFO_MPTY_BIT     0x01
#define I2C_STATUS_RD_DATA_RDY_BIT       0x02
#define I2C_STATUS_BUS_ERR_BIT           0x04
#define I2C_STATUS_RD_DATA_UFL_BIT       0x08
#define I2C_STATUS_CMD_FIFO_OFL_BIT      0x10
#define I2C_STATUS_CMD_FIFO_FULL_BIT     0x20

/* I2C return status */
#define I2C_STATUS_INVALID            0xFF
#define I2C_STATUS_SUCCESS            0x00
#define I2C_STATUS_FAIL               0x01
#define I2C_STATUS_BUS_ERROR          0x02
#define I2C_STATUS_RD_DATA_UFL        0x03
#define I2C_STATUS_CMD_FIFO_OFL       0x04
#define I2C_STATUS_INTERRUPT_ERROR    0x05
#define I2C_STATUS_CMD_FIFO_EMPTY     0x06

/* I2C clock divider position */
#define I2C_CLOCKDIVEDER_VAL_MASK       0x1F
#define I2C_APB_CLK_DIVIDER_VAL_MASK    0x1FE0

/* Error check */
#define I2C_UFL_CHECK         (obj->membase->STATUS.WORD & 0x80)
#define I2C_FIFO_FULL         (obj->membase->STATUS.WORD & 0x20)
#define FIFO_OFL_CHECK        (obj->membase->STATUS.WORD & 0x10)
#define I2C_BUS_ERR_CHECK     (obj->membase->STATUS.WORD & 0x04)
#define RD_DATA_READY         (obj->membase->STATUS.WORD & 0x02)
#define I2C_FIFO_EMPTY        (obj->membase->STATUS.WORD & 0x01)

#define I2C_API_STATUS_SUCCESS    0
#define PAD_REG_ADRS_BYTE_SIZE    4

#define SEND_COMMAND(cmd)  while(!I2C_FIFO_EMPTY); wait_us(1); obj->membase->CMD_REG = cmd;

/** Init I2C device.
 * @details
 * Sets the necessary registers. The baud rate is set default to 100K
 *
 * @param obj A I2C device instance.
 * @param sda GPIO number for SDA line
 * @param scl GPIO number for SCL line
 * @return None
 */
extern void fI2cInit(i2c_t *obj,PinName sda,PinName scl);

/** Set baud rate or frequency
 * @details
 * Sets user baudrate
 *
 * @param obj A I2C device instance.
 * @param hz User desired baud rate/frequency
 * @return None
 */
extern void fI2cFrequency(i2c_t *obj, uint32_t hz);

/** Sends start bit
 * @details
 * Sends start bit on i2c pins
 *
 * @param obj A I2C device instance.
 * @return status
 */
extern int32_t fI2cStart(i2c_t *obj);

/** Sends stop bit
 * @details
 * Sends stop bit on i2c pins
 *
 * @param obj A I2C device instance.
 * @return status
 */
extern int32_t fI2cStop(i2c_t *obj);

/** Reads data from a I2C device in blocking fashion.
 * @details
 * The data is read from the receive queue into the buffer. The receive queue is
 * filled by the interrupt handler.  If not enough data is available,
 *
 * @param d The device to read from.
 * @param buf The buffer to read into (only the contents of the buffer may be modified, not the buffer itself).
 * @param len The maximum number of bytes to read, typically the buffer length.
 * @return On Success: The actual number of bytes read. On Failure: Failure code.
 */
extern int32_t fI2cReadB(i2c_t *d, char *buf, int len);

/** Write data to an I2C device.
 * @details
 * The commands(I2C instructions) and data arrive at the I2C Engine via the Command FIFO.
 * The command to write the data & data to be written is sent to command FIFO by writing it into command register.
 *
 * @param d The device to write to.
 * @param buf The buffer to write from (the contents of the buffer may not be modified).
 * @param len The number of bytes to write.
 * @return On success: The actual number of bytes written. On Failure: Failure code
 */
extern int32_t fI2cWriteB(i2c_t *d, const char *buf, int len);

#endif /* I2C_H_ */