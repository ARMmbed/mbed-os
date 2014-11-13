/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2014, STMicroelectronics
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 */
#include "mbed_assert.h"
#include "i2c_api.h"

#if DEVICE_I2C

#include "cmsis.h"
#include "pinmap.h"

/* Timeout values for flags and events waiting loops. These timeouts are
   not based on accurate values, they just guarantee that the application will
   not remain stuck if the I2C communication is corrupted. */
#define FLAG_TIMEOUT ((int)0x1000)
#define LONG_TIMEOUT ((int)0x8000)

static const PinMap PinMap_I2C_SDA[] = {
    {PB_7,  I2C_1, STM_PIN_DATA(GPIO_Mode_AF_OD, 0)},
    {PB_9,  I2C_1, STM_PIN_DATA(GPIO_Mode_AF_OD, 2)}, // GPIO_Remap_I2C1
    {PB_11, I2C_2, STM_PIN_DATA(GPIO_Mode_AF_OD, 0)},
    {NC,    NC,    0}
};

static const PinMap PinMap_I2C_SCL[] = {
    {PB_6,  I2C_1, STM_PIN_DATA(GPIO_Mode_AF_OD, 0)},
    {PB_8,  I2C_1, STM_PIN_DATA(GPIO_Mode_AF_OD, 2)}, // GPIO_Remap_I2C1
    {PB_10, I2C_2, STM_PIN_DATA(GPIO_Mode_AF_OD, 0)},
    {NC,    NC,    0}
};

int i2c1_inited = 0;
int i2c2_inited = 0;

void i2c_init(i2c_t *obj, PinName sda, PinName scl) {
    // Determine the I2C to use
    I2CName i2c_sda = (I2CName)pinmap_peripheral(sda, PinMap_I2C_SDA);
    I2CName i2c_scl = (I2CName)pinmap_peripheral(scl, PinMap_I2C_SCL);

    obj->i2c = (I2CName)pinmap_merge(i2c_sda, i2c_scl);
    MBED_ASSERT(obj->i2c != (I2CName)NC);

    // Enable I2C clock and configure I2C pins if not done before
    if ((obj->i2c == I2C_1)&& !i2c1_inited) {
        i2c1_inited = 1;
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
        // Configure I2C pins
        pinmap_pinout(scl, PinMap_I2C_SCL);
        pin_mode(scl, OpenDrain);
        pinmap_pinout(sda, PinMap_I2C_SDA);
        pin_mode(sda, OpenDrain);
    }
    if ((obj->i2c == I2C_2)&& !i2c2_inited) {
        i2c2_inited = 1;
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
        // Configure I2C pins
        pinmap_pinout(scl, PinMap_I2C_SCL);
        pin_mode(scl, OpenDrain);
        pinmap_pinout(sda, PinMap_I2C_SDA);
        pin_mode(sda, OpenDrain);
    }

    // Reset to clear pending flags if any
    i2c_reset(obj);

    // I2C configuration
    i2c_frequency(obj, 100000); // 100 kHz per default
}

void i2c_frequency(i2c_t *obj, int hz) {
    int timeout;

    I2C_TypeDef    *i2c = (I2C_TypeDef *)(obj->i2c);
    I2C_InitTypeDef I2C_InitStructure;

    if ((hz != 0) && (hz <= 400000)) {
        // wait before init
        timeout = LONG_TIMEOUT;
        while ((I2C_GetFlagStatus(i2c, I2C_FLAG_BUSY)) && (timeout-- != 0)) {
        }

        I2C_DeInit(i2c);

        // I2C configuration
        I2C_InitStructure.I2C_Mode                = I2C_Mode_I2C;
        I2C_InitStructure.I2C_DutyCycle           = I2C_DutyCycle_2;
        I2C_InitStructure.I2C_OwnAddress1         = 0;
        I2C_InitStructure.I2C_Ack                 = I2C_Ack_Enable;
        I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
        I2C_InitStructure.I2C_ClockSpeed          = hz;
        I2C_Init(i2c, &I2C_InitStructure);

        I2C_Cmd(i2c, ENABLE);
    }
}

inline int i2c_start(i2c_t *obj) {
    I2C_TypeDef *i2c = (I2C_TypeDef *)(obj->i2c);
    int          timeout;

    I2C_ClearFlag(i2c, I2C_FLAG_AF); // Clear Acknowledge failure flag

    // Generate the START condition
    I2C_GenerateSTART(i2c, ENABLE);

    // Wait the START condition has been correctly sent
    timeout = FLAG_TIMEOUT;
    while (I2C_GetFlagStatus(i2c, I2C_FLAG_SB) == RESET) {
        timeout--;
        if (timeout == 0) {
            return 1;
        }
    }

    return 0;
}

inline int i2c_stop(i2c_t *obj) {
    I2C_TypeDef *i2c = (I2C_TypeDef *)(obj->i2c);

    I2C_GenerateSTOP(i2c, ENABLE);

    return 0;
}

int i2c_read(i2c_t *obj, int address, char *data, int length, int stop) {
    I2C_TypeDef *i2c = (I2C_TypeDef *)(obj->i2c);
    int          timeout;
    int          count;
    int          value;

    i2c_start(obj);

    // Send slave address for read
    I2C_Send7bitAddress(i2c, address, I2C_Direction_Receiver);

    // Wait address is acknowledged
    timeout = FLAG_TIMEOUT;
    while (I2C_CheckEvent(i2c, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) == ERROR) {
        timeout--;
        if (timeout == 0) {
            return -1;
        }
    }

    // Read all bytes except last one
    for (count = 0; count < (length - 1); count++) {
        value       = i2c_byte_read(obj, 0);
        data[count] = (char)value;
    }

    // If not repeated start, send stop.
    // Warning: must be done BEFORE the data is read.
    if (stop) {
        i2c_stop(obj);
    }

    // Read the last byte
    value       = i2c_byte_read(obj, 1);
    data[count] = (char)value;

    return length;
}

int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop) {
    I2C_TypeDef *i2c = (I2C_TypeDef *)(obj->i2c);
    int          timeout;
    int          count;

    i2c_start(obj);

    // Send slave address for write
    I2C_Send7bitAddress(i2c, address, I2C_Direction_Transmitter);

    // Wait address is acknowledged
    timeout = FLAG_TIMEOUT;
    while (I2C_CheckEvent(i2c, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == ERROR) {
        timeout--;
        if (timeout == 0) {
            return -1;
        }
    }

    for (count = 0; count < length; count++) {
        if (i2c_byte_write(obj, data[count]) != 1) {
            i2c_stop(obj);
            return 0;
        }
    }

    // If not repeated start, send stop.
    if (stop) {
        i2c_stop(obj);
    }

    return count;
}

int i2c_byte_read(i2c_t *obj, int last) {
    I2C_TypeDef *i2c = (I2C_TypeDef *)(obj->i2c);
    uint8_t      data;
    int          timeout;

    if (last) {
        // Don't acknowledge the last byte
        I2C_AcknowledgeConfig(i2c, DISABLE);
    } else {
        // Acknowledge the byte
        I2C_AcknowledgeConfig(i2c, ENABLE);
    }

    // Wait until the byte is received
    timeout = FLAG_TIMEOUT;
    while (I2C_GetFlagStatus(i2c, I2C_FLAG_RXNE) == RESET) {
        timeout--;
        if (timeout == 0) {
            return -1;
        }
    }

    data = I2C_ReceiveData(i2c);

    return (int)data;
}

int i2c_byte_write(i2c_t *obj, int data) {
    I2C_TypeDef *i2c = (I2C_TypeDef *)(obj->i2c);
    int          timeout;

    I2C_SendData(i2c, (uint8_t)data);

    // Wait until the byte is transmitted
    timeout = FLAG_TIMEOUT;
    while ((I2C_GetFlagStatus(i2c, I2C_FLAG_TXE) == RESET) &&
           (I2C_GetFlagStatus(i2c, I2C_FLAG_BTF) == RESET)) {
        timeout--;
        if (timeout == 0) {
            return 0;
        }
    }

    return 1;
}

void i2c_reset(i2c_t *obj) {
    I2C_TypeDef *i2c = (I2C_TypeDef *)(obj->i2c);
    int          timeout;

    // wait before reset
    timeout = LONG_TIMEOUT;
    while ((I2C_GetFlagStatus(i2c, I2C_FLAG_BUSY)) && (timeout-- != 0)) {
    }

    if (obj->i2c == I2C_1) {
        RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);
        RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);
    }
    if (obj->i2c == I2C_2) {
        RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2, ENABLE);
        RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2, DISABLE);
    }
}

#if DEVICE_I2CSLAVE

void i2c_slave_address(i2c_t *obj, int idx, uint32_t address, uint32_t mask) {
    I2C_TypeDef *i2c = (I2C_TypeDef *)(obj->i2c);
    uint16_t     tmpreg;

    // Get the old register value
    tmpreg = i2c->OAR1;
    // Reset address bits
    tmpreg &= 0xFC00;
    // Set new address
    tmpreg |= (uint16_t)((uint16_t)address & (uint16_t)0x00FE); // 7-bits
    // Store the new register value
    i2c->OAR1 = tmpreg;
}

void i2c_slave_mode(i2c_t *obj, int enable_slave) {
    // Nothing to do
}

// See I2CSlave.h
#define NoData         0 // the slave has not been addressed
#define ReadAddressed  1 // the master has requested a read from this slave (slave = transmitter)
#define WriteGeneral   2 // the master is writing to all slave
#define WriteAddressed 3 // the master is writing to this slave (slave = receiver)

int i2c_slave_receive(i2c_t *obj) {
    int          retValue = NoData;
    uint32_t     event;
    I2C_TypeDef *i2c = (I2C_TypeDef *)(obj->i2c);

    event = I2C_GetLastEvent(i2c);
    if (event != 0) {
        switch (event) {
            case I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED:
                retValue = WriteAddressed;
                break;
            case I2C_EVENT_SLAVE_TRANSMITTER_ADDRESS_MATCHED:
                retValue = ReadAddressed;
                break;
            case I2C_EVENT_SLAVE_GENERALCALLADDRESS_MATCHED:
                retValue = WriteGeneral;
                break;
            default:
                retValue = NoData;
                break;
        }

        // clear ADDR
        if ((retValue == WriteAddressed) || (retValue == ReadAddressed)) {
            // read SR to clear ADDR flag
            i2c->SR1;
            i2c->SR2;
        }
        // clear stopf
        if (I2C_GetFlagStatus(i2c, I2C_FLAG_STOPF) == SET) {
            // read SR1 and write CR1 to clear STOP flag
            i2c->SR1;
            I2C_Cmd(i2c,  ENABLE);
        }
        // clear AF
        if (I2C_GetFlagStatus(i2c, I2C_FLAG_AF) == SET) {
            I2C_ClearFlag(i2c, I2C_FLAG_AF);
        }
    }
    return (retValue);
}

int i2c_slave_read(i2c_t *obj, char *data, int length) {
    int count = 0;

    // Read all bytes
    for (count = 0; count < length; count++) {
        data[count] = i2c_byte_read(obj, 0);
    }

    return count;
}

int i2c_slave_write(i2c_t *obj, const char *data, int length) {
    int count = 0;

    // Write all bytes
    for (count = 0; count < length; count++) {
        i2c_byte_write(obj, data[count]);
    }

    return count;
}

#endif // DEVICE_I2CSLAVE

#endif // DEVICE_I2C
