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
#include "i2c_api.h"

#if DEVICE_I2C

#include "cmsis.h"
#include "pinmap.h"
#include "error.h"
#include "stm32f4xx_hal.h"

/* Timeout values for flags and events waiting loops. These timeouts are
   not based on accurate values, they just guarantee that the application will 
   not remain stuck if the I2C communication is corrupted. */   
#define FLAG_TIMEOUT ((int)0x1000)
#define LONG_TIMEOUT ((int)0x8000)

static const PinMap PinMap_I2C_SDA[] = {
    {PB_11, I2C_2, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C2)},
    {PF_0 , I2C_2, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C2)},
    {PH_5 , I2C_2, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C2)},
    {PB_7,  I2C_1, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C1)},
    {PB_9,  I2C_1, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C1)},
    {PC_9,  I2C_3, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C3)},
    {PH_8,  I2C_3, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C3)},
    {NC,    NC,    0}
};

static const PinMap PinMap_I2C_SCL[] = {
    {PA_8,  I2C_3, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C3)},
    {PB_6,  I2C_1, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C1)},
    {PB_8,  I2C_1, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C1)},
    {PB_10, I2C_2, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C2)},
    {PF_1 , I2C_2, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C2)},
    {PH_4 , I2C_2, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C2)},
    {NC,    NC,    0}
};

I2C_HandleTypeDef I2cHandle;

void i2c_init(i2c_t *obj, PinName sda, PinName scl) {  
    // Determine the I2C to use
    I2CName i2c_sda = (I2CName)pinmap_peripheral(sda, PinMap_I2C_SDA);
    I2CName i2c_scl = (I2CName)pinmap_peripheral(scl, PinMap_I2C_SCL);

    obj->i2c = (I2CName)pinmap_merge(i2c_sda, i2c_scl);
    
    if (obj->i2c == (I2CName)NC) {
        error("I2C error: pinout mapping failed.");
    }

    // Enable I2C clock
    if (obj->i2c == I2C_1) {    
        __I2C1_CLK_ENABLE();
    }
    if (obj->i2c == I2C_2) {
        __I2C2_CLK_ENABLE();
    }
    if (obj->i2c == I2C_3) {
        __I2C3_CLK_ENABLE();
    }

    // Configure I2C pins
    pinmap_pinout(sda, PinMap_I2C_SDA);
    pinmap_pinout(scl, PinMap_I2C_SCL);
    pin_mode(sda, OpenDrain);
    pin_mode(scl, OpenDrain);
    
    // Reset to clear pending flags if any
    i2c_reset(obj);
    
    // I2C configuration
    i2c_frequency(obj, 100000); // 100 kHz per default    
}

void i2c_frequency(i2c_t *obj, int hz) {
    I2cHandle.Instance = (I2C_TypeDef *)(obj->i2c);
  
    if ((hz != 0) && (hz <= 400000)) {
        // I2C configuration      
        I2cHandle.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
        I2cHandle.Init.ClockSpeed      = hz;
        I2cHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
        I2cHandle.Init.DutyCycle       = I2C_DUTYCYCLE_2;
        I2cHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
        I2cHandle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLED;
        I2cHandle.Init.OwnAddress1     = 0;
        I2cHandle.Init.OwnAddress2     = 0;
        HAL_I2C_Init(&I2cHandle);    
    }
    else {
        error("I2C error: frequency setting failed (max 400kHz).");
    }
}

inline int i2c_start(i2c_t *obj) {
    I2C_TypeDef *i2c = (I2C_TypeDef *)(obj->i2c);
    int timeout;
    
    I2cHandle.Instance = (I2C_TypeDef *)(obj->i2c);
  
    // Clear Acknowledge failure flag
    __HAL_I2C_CLEAR_FLAG(&I2cHandle, I2C_FLAG_AF);
  
    // Generate the START condition
    i2c->CR1 |= I2C_CR1_START;
  
    // Wait the START condition has been correctly sent
    timeout = FLAG_TIMEOUT;
    while (__HAL_I2C_GET_FLAG(&I2cHandle, I2C_FLAG_SB) == RESET) {
      if ((timeout--) == 0) {
          return 1;
      }
    }
    
    return 0;
}

inline int i2c_stop(i2c_t *obj) {
    I2C_TypeDef *i2c = (I2C_TypeDef *)(obj->i2c);
  
    // Generate the STOP condition
    i2c->CR1 |= I2C_CR1_STOP;

    return 0;
}

int i2c_read(i2c_t *obj, int address, char *data, int length, int stop) {  
    if (length == 0) return 0;
  
    I2cHandle.Instance = (I2C_TypeDef *)(obj->i2c);

    // Reception process with 5 seconds timeout
    if (HAL_I2C_Master_Receive(&I2cHandle, (uint16_t)address, (uint8_t *)data, length, 5000) != HAL_OK) {
        return 0; // Error
    }

    return length;
}

int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop) {
    if (length == 0) return 0;
  
    I2cHandle.Instance = (I2C_TypeDef *)(obj->i2c);

    // Transmission process with 5 seconds timeout
    if (HAL_I2C_Master_Transmit(&I2cHandle, (uint16_t)address, (uint8_t *)data, length, 5000) != HAL_OK) {
        return 0; // Error
    }
    
    return length;
}

int i2c_byte_read(i2c_t *obj, int last) {
    I2C_TypeDef *i2c = (I2C_TypeDef *)(obj->i2c);
    int timeout;
  
    if (last) {
        // Don't acknowledge the last byte
        i2c->CR1 &= ~I2C_CR1_ACK;
    } else {
        // Acknowledge the byte
        i2c->CR1 |= I2C_CR1_ACK;
    }

    // Wait until the byte is received
    timeout = FLAG_TIMEOUT;
    while (__HAL_I2C_GET_FLAG(&I2cHandle, I2C_FLAG_RXNE) == RESET) {
      if ((timeout--) == 0) {
          return 0;
      }
    }
    
    return (int)i2c->DR;
}

int i2c_byte_write(i2c_t *obj, int data) {
    I2C_TypeDef *i2c = (I2C_TypeDef *)(obj->i2c);
    int timeout;

    i2c->DR = (uint8_t)data;

    // Wait until the byte is transmitted
    timeout = FLAG_TIMEOUT;  
    while ((__HAL_I2C_GET_FLAG(&I2cHandle, I2C_FLAG_TXE) == RESET) &&
           (__HAL_I2C_GET_FLAG(&I2cHandle, I2C_FLAG_BTF) == RESET)) {
        if ((timeout--) == 0) {
            return 0;
        }
    }
    
    return 1;
}

void i2c_reset(i2c_t *obj) {
    if (obj->i2c == I2C_1) {    
        __I2C1_FORCE_RESET();
        __I2C1_RELEASE_RESET();
    }
    if (obj->i2c == I2C_2) {
        __I2C2_FORCE_RESET();
        __I2C2_RELEASE_RESET();
    }
    if (obj->i2c == I2C_3) {
        __I2C3_FORCE_RESET();
        __I2C3_RELEASE_RESET();
    }
}

#if DEVICE_I2CSLAVE

void i2c_slave_address(i2c_t *obj, int idx, uint32_t address, uint32_t mask) {
    I2C_TypeDef *i2c = (I2C_TypeDef *)(obj->i2c);
    uint16_t tmpreg;

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
    // TO BE DONE
    return(0);
}

int i2c_slave_read(i2c_t *obj, char *data, int length) {
    if (length == 0) return 0;
  
    I2cHandle.Instance = (I2C_TypeDef *)(obj->i2c);

    // Reception process with 5 seconds timeout
    if (HAL_I2C_Slave_Receive(&I2cHandle, (uint8_t *)data, length, 5000) != HAL_OK) {
        return 0; // Error
    }

    return length;
}

int i2c_slave_write(i2c_t *obj, const char *data, int length) {
    if (length == 0) return 0;
  
    I2cHandle.Instance = (I2C_TypeDef *)(obj->i2c);

    // Transmission process with 5 seconds timeout
    if (HAL_I2C_Slave_Transmit(&I2cHandle, (uint8_t *)data, length, 5000) != HAL_OK) {
        return 0; // Error
    }

    return length;
}


#endif // DEVICE_I2CSLAVE

#endif // DEVICE_I2C
