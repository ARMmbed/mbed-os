/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2015, STMicroelectronics
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
#include "stdlib.h"

#if DEVICE_I2C

#ifndef USE_STM32F4XX_HAL_I2C__FIX
#error Please use stm32f4xx_hal_i2c__FIX.c
#endif

#include "cmsis.h"
#include "pinmap.h"
#include "PeripheralPins.h"

/* Timeout values for flags and events waiting loops. These timeouts are
   not based on accurate values, they just guarantee that the application will
   not remain stuck if the I2C communication is corrupted. */
#define FLAG_TIMEOUT ((int)0x1000)
#define LONG_TIMEOUT ((int)0x8000)

int i2c1_inited = 0;
int i2c2_inited = 0;
int i2c3_inited = 0;
int fmpi2c1_inited = 0;

#if defined(I2C3_BASE)
  #define MODULE_SIZE_I2C 3
  #if DEVICE_I2C_ASYNCH
static const IRQn_Type I2cEventIRQs[MODULE_SIZE_I2C] = {  
    I2C1_EV_IRQn,  
    I2C2_EV_IRQn, 
    I2C3_EV_IRQn,  
};  
static const IRQn_Type I2cErrorIRQs[MODULE_SIZE_I2C] = {  
    I2C1_ER_IRQn,  
    I2C2_ER_IRQn,  
    I2C3_ER_IRQn,
};
  #endif
#elif defined(I2C2_BASE)
  #define MODULE_SIZE_I2C 2
  #if DEVICE_I2C_ASYNCH
static const IRQn_Type I2cEventIRQs[MODULE_SIZE_I2C] = {  
    I2C1_EV_IRQn,  
    I2C2_EV_IRQn,  
};  
static const IRQn_Type I2cErrorIRQs[MODULE_SIZE_I2C] = {  
    I2C1_ER_IRQn,  
    I2C2_ER_IRQn,  
};
  #endif
#else
  #define MODULE_SIZE_I2C 1
  #if DEVICE_I2C_ASYNCH
static const IRQn_Type I2cEventIRQs[MODULE_SIZE_I2C] = {  
    I2C1_EV_IRQn,  
};  
static const IRQn_Type I2cErrorIRQs[MODULE_SIZE_I2C] = {  
    I2C1_ER_IRQn,  
};
  #endif
#endif

I2C_HandleTypeDef t_I2cHandle[MODULE_SIZE_I2C];

#if !DEVICE_I2C_ASYNCH
int i2c_module_lookup(i2c_t *obj)
{
	switch(obj->i2c)
	{
		case I2C_1: return 0;
#if defined(I2C2_Base)
		case I2C_2: return 1;
#endif
#if defined(I2C3_Base)
		case I2C_3: return 2;
#endif	    
		default: return 0;
	}
}
#else //DEVICE_I2C_ASYNCH
int i2c_module_lookup(i2c_t *obj)
{
	switch(obj->i2c.i2c)
	{
		case I2C_1: return 0;
#if defined(I2C2_Base)
		case I2C_2: return 1;
#endif
#if defined(I2C3_Base)
		case I2C_3: return 2;
#endif	    
		default: return 0;
	}
}

int i2c_module_lookup_handler(I2C_HandleTypeDef* hi2c)
{
  uint32_t I2C_address = (uint32_t)&(hi2c->Instance);
  
	switch(I2C_address)
	{
		case I2C_1: return 0;
#if defined(I2C2_BASE)
		case I2C_2: return 1;
#endif
#if defined(I2C3_BASE)
		case I2C_3: return 2;
#endif		
		default: return 0;
	}
}
#endif

void i2c_init(i2c_t *obj, PinName sda, PinName scl)
{
    // Determine the I2C to use
    I2CName i2c_sda = (I2CName)pinmap_peripheral(sda, PinMap_I2C_SDA);
    I2CName i2c_scl = (I2CName)pinmap_peripheral(scl, PinMap_I2C_SCL);

#if !DEVICE_I2C_ASYNCH
    obj->i2c = (I2CName)pinmap_merge(i2c_sda, i2c_scl);
    MBED_ASSERT(obj->i2c != (I2CName)NC);
#else
    obj->i2c.i2c = (I2CName)pinmap_merge(i2c_sda, i2c_scl);
    MBED_ASSERT(obj->i2c.i2c != (I2CName)NC);
#endif

    // Enable I2C1 clock and pinout if not done
#if !DEVICE_I2C_ASYNCH	
    if ((obj->i2c == I2C_1) && !i2c1_inited) {
#else
    if ((obj->i2c.i2c == I2C_1) && !i2c1_inited) {
#endif
        i2c1_inited = 1;
        __I2C1_CLK_ENABLE();
        // Configure I2C pins
        pinmap_pinout(sda, PinMap_I2C_SDA);
        pinmap_pinout(scl, PinMap_I2C_SCL);
        pin_mode(sda, OpenDrain);
        pin_mode(scl, OpenDrain);
    }
    // Enable I2C2 clock and pinout if not done
#if !DEVICE_I2C_ASYNCH	
    if ((obj->i2c == I2C_2) && !i2c2_inited) {
#else
    if ((obj->i2c.i2c == I2C_2) && !i2c2_inited) {
#endif
        i2c2_inited = 1;
        __I2C2_CLK_ENABLE();
        // Configure I2C pins
        pinmap_pinout(sda, PinMap_I2C_SDA);
        pinmap_pinout(scl, PinMap_I2C_SCL);
        pin_mode(sda, OpenDrain);
        pin_mode(scl, OpenDrain);
    }
#if defined I2C3_BASE
    // Enable I2C3 clock and pinout if not done
#if !DEVICE_I2C_ASYNCH	
    if ((obj->i2c == I2C_3) && !i2c3_inited) {
#else
    if ((obj->i2c.i2c == I2C_3) && !i2c3_inited) {
#endif
        i2c3_inited = 1;
        __I2C3_CLK_ENABLE();
        // Configure I2C pins
        pinmap_pinout(sda, PinMap_I2C_SDA);
        pinmap_pinout(scl, PinMap_I2C_SCL);
        pin_mode(sda, OpenDrain);
        pin_mode(scl, OpenDrain);
    }
#endif

#if defined FMPI2C1_BASE
    // Enable I2C3 clock and pinout if not done
    if ((obj->i2c == FMPI2C_1) && !fmpi2c1_inited) {
        fmpi2c1_inited = 1;
        __HAL_RCC_FMPI2C1_CLK_ENABLE();
        // Configure I2C pins
        pinmap_pinout(sda, PinMap_I2C_SDA);
        pinmap_pinout(scl, PinMap_I2C_SCL);
        pin_mode(sda, OpenDrain);
        pin_mode(scl, OpenDrain);
    }
#endif

    // Reset to clear pending flags if any
    i2c_reset(obj);

    // I2C configuration
    i2c_frequency(obj, 100000); // 100 kHz per default

    // I2C master by default
#if !DEVICE_I2C_ASYNCH    
    obj->slave = 0;
#else
    obj->i2c.slave = 0;    
#endif 
}

void i2c_frequency(i2c_t *obj, int hz)
{
    MBED_ASSERT((hz > 0) && (hz <= 400000));
#if !DEVICE_I2C_ASYNCH
    t_I2cHandle[i2c_module_lookup(obj)].Instance = (I2C_TypeDef *)(obj->i2c);
#else
    t_I2cHandle[i2c_module_lookup(obj)].Instance = (I2C_TypeDef *)(obj->i2c.i2c);
#endif
    int timeout;

    // wait before init
    timeout = LONG_TIMEOUT;
    while ((__HAL_I2C_GET_FLAG(&t_I2cHandle[i2c_module_lookup(obj)], I2C_FLAG_BUSY)) && (timeout-- != 0));

    // I2C configuration
    t_I2cHandle[i2c_module_lookup(obj)].Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
    t_I2cHandle[i2c_module_lookup(obj)].Init.ClockSpeed      = hz;
    t_I2cHandle[i2c_module_lookup(obj)].Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
    t_I2cHandle[i2c_module_lookup(obj)].Init.DutyCycle       = I2C_DUTYCYCLE_2;
    t_I2cHandle[i2c_module_lookup(obj)].Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
    t_I2cHandle[i2c_module_lookup(obj)].Init.NoStretchMode   = I2C_NOSTRETCH_DISABLED;
    t_I2cHandle[i2c_module_lookup(obj)].Init.OwnAddress1     = 0;
    t_I2cHandle[i2c_module_lookup(obj)].Init.OwnAddress2     = 0;
    HAL_I2C_Init(&t_I2cHandle[i2c_module_lookup(obj)]);
#if !DEVICE_I2C_ASYNCH    
    if (obj->slave) {
#else      
    if (obj->i2c.slave) {
#endif 
        /* Enable Address Acknowledge */
        t_I2cHandle[i2c_module_lookup(obj)].Instance->CR1 |= I2C_CR1_ACK;
    }
    
#if DEVICE_I2C_ASYNCH_DMA
  static DMA_HandleTypeDef hdma_tx;
  static DMA_HandleTypeDef hdma_rx;

  if(t_I2cHandle[i2c_module_lookup(obj)].Instance == I2C2) {
    hdma_tx.Instance                 = DMA1_Stream7;
    hdma_tx.Init.Channel             = DMA_CHANNEL_1;
    hdma_rx.Instance                 = DMA1_Stream2;
    hdma_rx.Init.Channel             = DMA_CHANNEL_1;
  }
  else if(t_I2cHandle[i2c_module_lookup(obj)].Instance == I2C3) {
    hdma_tx.Instance                 = DMA1_Stream4;
    hdma_tx.Init.Channel             = DMA_CHANNEL_1;
    hdma_rx.Instance                 = DMA1_Stream1;
    hdma_rx.Init.Channel             = DMA_CHANNEL_1;
  }
  else {
    hdma_tx.Instance                 = DMA1_Stream7;
    hdma_tx.Init.Channel             = DMA_CHANNEL_1;
    hdma_rx.Instance                 = DMA1_Stream0;
    hdma_rx.Init.Channel             = DMA_CHANNEL_1;
  }
  
  /* Enable DMA1 clock */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* Configure the DMA handler for Transmission process */
  hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
  hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
  hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
  hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
  hdma_tx.Init.Mode                = DMA_NORMAL;
  hdma_tx.Init.Priority            = DMA_PRIORITY_LOW;
  hdma_tx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;         
  hdma_tx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
  hdma_tx.Init.MemBurst            = DMA_MBURST_INC4;
  hdma_tx.Init.PeriphBurst         = DMA_PBURST_INC4;
  
  HAL_DMA_Init(&hdma_tx);   
  
  /* Associate the initialized DMA handle to the the I2C handle */
  __HAL_LINKDMA(&t_I2cHandle[i2c_module_lookup(obj)], hdmatx, hdma_tx);
    
  /* Configure the DMA handler for Transmission process */
  hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
  hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
  hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
  hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
  hdma_rx.Init.Mode                = DMA_NORMAL;
  hdma_rx.Init.Priority            = DMA_PRIORITY_HIGH;
  hdma_rx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;         
  hdma_rx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
  hdma_rx.Init.MemBurst            = DMA_MBURST_INC4;
  hdma_rx.Init.PeriphBurst         = DMA_PBURST_INC4; 

  HAL_DMA_Init(&hdma_rx);
    
  /* Associate the initialized DMA handle to the the I2C handle */
  __HAL_LINKDMA(&t_I2cHandle[i2c_module_lookup(obj)], hdmarx, hdma_rx);
#endif

}

inline int i2c_start(i2c_t *obj)
{
#if !DEVICE_I2C_ASYNCH
    I2C_TypeDef *i2c = (I2C_TypeDef *)(obj->i2c);
    t_I2cHandle[i2c_module_lookup(obj)].Instance = (I2C_TypeDef *)(obj->i2c);
#else
    I2C_TypeDef *i2c = (I2C_TypeDef *)(obj->i2c.i2c);
    t_I2cHandle[i2c_module_lookup(obj)].Instance = (I2C_TypeDef *)(obj->i2c.i2c);
#endif
    int timeout;

    // Clear Acknowledge failure flag
    __HAL_I2C_CLEAR_FLAG(&t_I2cHandle[i2c_module_lookup(obj)], I2C_FLAG_AF);

    // Wait the STOP condition has been previously correctly sent
	// This timeout can be avoid in some specific cases by simply clearing the STOP bit
    timeout = FLAG_TIMEOUT;
    while ((i2c->CR1 & I2C_CR1_STOP) == I2C_CR1_STOP) {
        if ((timeout--) == 0) {
            return 1;
        }
    }

    // Generate the START condition
    i2c->CR1 |= I2C_CR1_START;

    // Wait the START condition has been correctly sent
    timeout = FLAG_TIMEOUT;
    while (__HAL_I2C_GET_FLAG(&t_I2cHandle[i2c_module_lookup(obj)], I2C_FLAG_SB) == RESET) {
        if ((timeout--) == 0) {
            return 1;
        }
    }

    return 0;
}

inline int i2c_stop(i2c_t *obj)
{
#if !DEVICE_I2C_ASYNCH
    I2C_TypeDef *i2c = (I2C_TypeDef *)(obj->i2c);
#else
    I2C_TypeDef *i2c = (I2C_TypeDef *)(obj->i2c.i2c);
#endif 

    // Generate the STOP condition
    i2c->CR1 |= I2C_CR1_STOP;

    return 0;
}

int i2c_read(i2c_t *obj, int address, char *data, int length, int stop)
{
#if !DEVICE_I2C_ASYNCH
    I2C_TypeDef *i2c = (I2C_TypeDef *)(obj->i2c);
    t_I2cHandle[i2c_module_lookup(obj)].Instance = (I2C_TypeDef *)(obj->i2c);
#else
    I2C_TypeDef *i2c = (I2C_TypeDef *)(obj->i2c.i2c);
    t_I2cHandle[i2c_module_lookup(obj)].Instance = (I2C_TypeDef *)(obj->i2c.i2c);
#endif
    int timeout;
    int count;
    int value;

    i2c_start(obj);

    // Wait until SB flag is set
    timeout = FLAG_TIMEOUT;
    while (__HAL_I2C_GET_FLAG(&t_I2cHandle[i2c_module_lookup(obj)], I2C_FLAG_SB) == RESET) {
        timeout--;
        if (timeout == 0) {
            return -1;
        }
    }

    i2c->DR = __HAL_I2C_7BIT_ADD_READ(address);


    // Wait address is acknowledged
    timeout = FLAG_TIMEOUT;
    while (__HAL_I2C_GET_FLAG(&t_I2cHandle[i2c_module_lookup(obj)], I2C_FLAG_ADDR) == RESET) {
        timeout--;
        if (timeout == 0) {
            return -1;
        }
    }
    __HAL_I2C_CLEAR_ADDRFLAG(&t_I2cHandle[i2c_module_lookup(obj)]);

    // Read all bytes except last one
    for (count = 0; count < (length - 1); count++) {
        value = i2c_byte_read(obj, 0);
        data[count] = (char)value;
    }

    // If not repeated start, send stop.
    // Warning: must be done BEFORE the data is read.
    if (stop) {
        i2c_stop(obj);
    }

    // Read the last byte
    value = i2c_byte_read(obj, 1);
    data[count] = (char)value;

    return length;
}

int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop)
{
#if !DEVICE_I2C_ASYNCH
    I2C_TypeDef *i2c = (I2C_TypeDef *)(obj->i2c);
    t_I2cHandle[i2c_module_lookup(obj)].Instance = (I2C_TypeDef *)(obj->i2c);
#else
    I2C_TypeDef *i2c = (I2C_TypeDef *)(obj->i2c.i2c);
    t_I2cHandle[i2c_module_lookup(obj)].Instance = (I2C_TypeDef *)(obj->i2c.i2c);
#endif
    int timeout;
    int count;

    i2c_start(obj);

    // Wait until SB flag is set
    timeout = FLAG_TIMEOUT;
    while (__HAL_I2C_GET_FLAG(&t_I2cHandle[i2c_module_lookup(obj)], I2C_FLAG_SB) == RESET) {
        timeout--;
        if (timeout == 0) {
            return -1;
        }
    }

    i2c->DR = __HAL_I2C_7BIT_ADD_WRITE(address);


    // Wait address is acknowledged
    timeout = FLAG_TIMEOUT;
    while (__HAL_I2C_GET_FLAG(&t_I2cHandle[i2c_module_lookup(obj)], I2C_FLAG_ADDR) == RESET) {
        timeout--;
        if (timeout == 0) {
            return -1;
        }
    }
    __HAL_I2C_CLEAR_ADDRFLAG(&t_I2cHandle[i2c_module_lookup(obj)]);

    for (count = 0; count < length; count++) {
        if (i2c_byte_write(obj, data[count]) != 1) {
            i2c_stop(obj);
            return -1;
        }
    }

    // If not repeated start, send stop.
    if (stop) {
        i2c_stop(obj);
    }

    return count;
}

int i2c_byte_read(i2c_t *obj, int last)
{
#if !DEVICE_I2C_ASYNCH
    I2C_TypeDef *i2c = (I2C_TypeDef *)(obj->i2c);
    t_I2cHandle[i2c_module_lookup(obj)].Instance = (I2C_TypeDef *)(obj->i2c);
#else
    I2C_TypeDef *i2c = (I2C_TypeDef *)(obj->i2c.i2c);
    t_I2cHandle[i2c_module_lookup(obj)].Instance = (I2C_TypeDef *)(obj->i2c.i2c);
#endif
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
    while (__HAL_I2C_GET_FLAG(&t_I2cHandle[i2c_module_lookup(obj)], I2C_FLAG_RXNE) == RESET) {
        if ((timeout--) == 0) {
            return -1;
        }
    }

    return (int)i2c->DR;
}

int i2c_byte_write(i2c_t *obj, int data)
{
#if !DEVICE_I2C_ASYNCH
    I2C_TypeDef *i2c = (I2C_TypeDef *)(obj->i2c);
    t_I2cHandle[i2c_module_lookup(obj)].Instance = (I2C_TypeDef *)(obj->i2c);
#else
    I2C_TypeDef *i2c = (I2C_TypeDef *)(obj->i2c.i2c);
    t_I2cHandle[i2c_module_lookup(obj)].Instance = (I2C_TypeDef *)(obj->i2c.i2c);
#endif
    int timeout;

    i2c->DR = (uint8_t)data;

    // Wait until the byte is transmitted
    timeout = FLAG_TIMEOUT;
    while ((__HAL_I2C_GET_FLAG(&t_I2cHandle[i2c_module_lookup(obj)], I2C_FLAG_TXE) == RESET) &&
            (__HAL_I2C_GET_FLAG(&t_I2cHandle[i2c_module_lookup(obj)], I2C_FLAG_BTF) == RESET)) {
        if ((timeout--) == 0) {
            return 0;
        }
    }

    return 1;
}

void i2c_reset(i2c_t *obj)
{
    int timeout;

    // wait before reset
    timeout = LONG_TIMEOUT;
    while ((__HAL_I2C_GET_FLAG(&t_I2cHandle[i2c_module_lookup(obj)], I2C_FLAG_BUSY)) && (timeout-- != 0));

 #if !DEVICE_I2C_ASYNCH
    if (obj->i2c == I2C_1) {
#else
    if (obj->i2c.i2c == I2C_1) {
#endif 
        __I2C1_FORCE_RESET();
        __I2C1_RELEASE_RESET();
    }
#if !DEVICE_I2C_ASYNCH
    if (obj->i2c == I2C_2) {
#else
    if (obj->i2c.i2c == I2C_2) {
#endif
        __I2C2_FORCE_RESET();
        __I2C2_RELEASE_RESET();
    }
#if defined I2C3_BASE
#if !DEVICE_I2C_ASYNCH
    if (obj->i2c == I2C_3) {
#else
    if (obj->i2c.i2c == I2C_3) {
#endif
        __I2C3_FORCE_RESET();
        __I2C3_RELEASE_RESET();
    }
#endif

#if defined FMPI2C1_BASE
    if (obj->i2c == FMPI2C_1) {
        __HAL_RCC_FMPI2C1_FORCE_RESET();
        __HAL_RCC_FMPI2C1_RELEASE_RESET();
    }
#endif
}

#if DEVICE_I2CSLAVE

void i2c_slave_address(i2c_t *obj, int idx, uint32_t address, uint32_t mask)
{
#if !DEVICE_I2C_ASYNCH
    I2C_TypeDef *i2c = (I2C_TypeDef *)(obj->i2c);
#else
    I2C_TypeDef *i2c = (I2C_TypeDef *)(obj->i2c.i2c);
#endif
    uint16_t tmpreg = 0;

    // Get the old register value
    tmpreg = i2c->OAR1;
    // Reset address bits
    tmpreg &= 0xFC00;
    // Set new address
    tmpreg |= (uint16_t)((uint16_t)address & (uint16_t)0x00FE); // 7-bits
    // Store the new register value
    i2c->OAR1 = tmpreg;
}

void i2c_slave_mode(i2c_t *obj, int enable_slave)
{
    if (enable_slave) {
#if !DEVICE_I2C_ASYNCH			
        obj->slave = 1;
#else
			  obj->i2c.slave = 1;
#endif
        /* Enable Address Acknowledge */
        t_I2cHandle[i2c_module_lookup(obj)].Instance->CR1 |= I2C_CR1_ACK;
    }
}

// See I2CSlave.h
#define NoData         0 // the slave has not been addressed
#define ReadAddressed  1 // the master has requested a read from this slave (slave = transmitter)
#define WriteGeneral   2 // the master is writing to all slave
#define WriteAddressed 3 // the master is writing to this slave (slave = receiver)

int i2c_slave_receive(i2c_t *obj)
{
    I2C_HandleTypeDef I2cHandle = t_I2cHandle[i2c_module_lookup(obj)];
    int retValue = NoData;

    if (__HAL_I2C_GET_FLAG(&I2cHandle, I2C_FLAG_BUSY) == 1) {
        if (__HAL_I2C_GET_FLAG(&I2cHandle, I2C_FLAG_ADDR) == 1) {
            if (__HAL_I2C_GET_FLAG(&I2cHandle, I2C_FLAG_TRA) == 1)
                retValue = ReadAddressed;
            else
                retValue = WriteAddressed;

            __HAL_I2C_CLEAR_FLAG(&I2cHandle, I2C_FLAG_ADDR);
        }
    }

    return (retValue);
}

int i2c_slave_read(i2c_t *obj, char *data, int length)
{
    I2C_HandleTypeDef I2cHandle = t_I2cHandle[i2c_module_lookup(obj)];
    uint32_t Timeout;
    int size = 0;

    while (length > 0) {
        /* Wait until RXNE flag is set */
        // Wait until the byte is received
        Timeout = FLAG_TIMEOUT;
        while (__HAL_I2C_GET_FLAG(&I2cHandle, I2C_FLAG_RXNE) == RESET) {
            Timeout--;
            if (Timeout == 0) {
                return -1;
            }
        }

        /* Read data from DR */
        (*data++) = I2cHandle.Instance->DR;
        length--;
        size++;

        if ((__HAL_I2C_GET_FLAG(&I2cHandle, I2C_FLAG_BTF) == SET) && (length != 0)) {
            /* Read data from DR */
            (*data++) = I2cHandle.Instance->DR;
            length--;
            size++;
        }
    }

    /* Wait until STOP flag is set */
    Timeout = FLAG_TIMEOUT;
    while (__HAL_I2C_GET_FLAG(&I2cHandle, I2C_FLAG_STOPF) == RESET) {
        Timeout--;
        if (Timeout == 0) {
            return -1;
        }
    }

    /* Clear STOP flag */
    __HAL_I2C_CLEAR_STOPFLAG(&I2cHandle);

    /* Wait until BUSY flag is reset */
    Timeout = FLAG_TIMEOUT;
    while (__HAL_I2C_GET_FLAG(&I2cHandle, I2C_FLAG_BUSY) == SET) {
        Timeout--;
        if (Timeout == 0) {
            return -1;
        }
    }

    return size;
}

int i2c_slave_write(i2c_t *obj, const char *data, int length)
{
    I2C_HandleTypeDef I2cHandle = t_I2cHandle[i2c_module_lookup(obj)];
    uint32_t Timeout;
    int size = 0;

    while (length > 0) {
        /* Wait until TXE flag is set */
        Timeout = FLAG_TIMEOUT;
        while (__HAL_I2C_GET_FLAG(&I2cHandle, I2C_FLAG_TXE) == RESET) {
            Timeout--;
            if (Timeout == 0) {
                return -1;
            }
        }


        /* Write data to DR */
        I2cHandle.Instance->DR = (*data++);
        length--;
        size++;

        if ((__HAL_I2C_GET_FLAG(&I2cHandle, I2C_FLAG_BTF) == SET) && (length != 0)) {
            /* Write data to DR */
            I2cHandle.Instance->DR = (*data++);
            length--;
            size++;
        }
    }

    /* Wait until AF flag is set */
    Timeout = FLAG_TIMEOUT;
    while (__HAL_I2C_GET_FLAG(&I2cHandle, I2C_FLAG_AF) == RESET) {
        Timeout--;
        if (Timeout == 0) {
            return -1;
        }
    }


    /* Clear AF flag */
    __HAL_I2C_CLEAR_FLAG(&I2cHandle, I2C_FLAG_AF);


    /* Wait until BUSY flag is reset */
    Timeout = FLAG_TIMEOUT;
    while (__HAL_I2C_GET_FLAG(&I2cHandle, I2C_FLAG_BUSY) == SET) {
        Timeout--;
        if (Timeout == 0) {
            return -1;
        }
    }

    I2cHandle.State = HAL_I2C_STATE_READY;

    /* Process Unlocked */
    __HAL_UNLOCK(&I2cHandle);

    return size;
}

#endif // DEVICE_I2CSLAVE

#if DEVICE_I2C_ASYNCH
#if DEVICE_I2C_ASYNCH_DMA

#define I2C_TIMEOUT_FLAG          ((uint32_t)35)     /* 35 ms */

uint32_t g_event[MODULE_SIZE_I2C];
uint32_t g_stop[MODULE_SIZE_I2C];
uint32_t g_stop_previous[MODULE_SIZE_I2C];
uint32_t g_transmitIT[MODULE_SIZE_I2C];
uint32_t g_receiveDMA[MODULE_SIZE_I2C];
uint32_t g_transmitDMA[MODULE_SIZE_I2C];

#if defined(I2C3_BASE)
static const IRQn_Type I2C_DMATx_IRQs[MODULE_SIZE_I2C] = {  
    DMA1_Stream7_IRQn,  
    DMA1_Stream7_IRQn, 
    DMA1_Stream4_IRQn,  
};
static const IRQn_Type I2C_DMARx_IRQs[MODULE_SIZE_I2C] = {  
    DMA1_Stream0_IRQn,  
    DMA1_Stream2_IRQn,
    DMA1_Stream1_IRQn,  
};
#elif defined(I2C2_BASE)
static const IRQn_Type I2C_DMATx_IRQs[MODULE_SIZE_I2C] = {  
    DMA1_Stream7_IRQn,  
    DMA1_Stream7_IRQn,  
};
static const IRQn_Type I2C_DMARx_IRQs[MODULE_SIZE_I2C] = {  
    DMA1_Stream0_IRQn,  
    DMA1_Stream2_IRQn,  
};
#else
static const IRQn_Type I2C_DMATx_IRQs[MODULE_SIZE_I2C] = {  
    DMA1_Stream7_IRQn,  
};
static const IRQn_Type I2C_DMARx_IRQs[MODULE_SIZE_I2C] = {  
    DMA1_Stream0_IRQn,  
};
#endif

static HAL_StatusTypeDef i2c_WaitOnFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Flag, FlagStatus Status, uint32_t Timeout)
{
  uint32_t tickstart = 0;

  /* Get tick */
  tickstart = HAL_GetTick();

  /* Wait until flag is set */
  if(Status == RESET)
  {
    while(__HAL_I2C_GET_FLAG(hi2c, Flag) == RESET)
    {
      /* Check for the Timeout */
      if(Timeout != HAL_MAX_DELAY)
      {
        if((Timeout == 0)||((HAL_GetTick() - tickstart ) > Timeout))
        {
          hi2c->State= HAL_I2C_STATE_READY;

          /* Process Unlocked */
          __HAL_UNLOCK(hi2c);

          return HAL_TIMEOUT;
        }
      }
    }
  }
  else
  {
    while(__HAL_I2C_GET_FLAG(hi2c, Flag) != RESET)
    {
      /* Check for the Timeout */
      if(Timeout != HAL_MAX_DELAY)
      {
        if((Timeout == 0)||((HAL_GetTick() - tickstart ) > Timeout))
        {
          hi2c->State= HAL_I2C_STATE_READY;

          /* Process Unlocked */
          __HAL_UNLOCK(hi2c);

          return HAL_TIMEOUT;
        }
      }
    }
  }
  return HAL_OK;
}

static void i2c_DMAMasterReceiveCplt(DMA_HandleTypeDef *hdma)
{
  I2C_HandleTypeDef* hi2c = (I2C_HandleTypeDef*)((DMA_HandleTypeDef*)hdma)->Parent;

  /* Generate Stop */
  if(g_stop[i2c_module_lookup_handler(hi2c)]) hi2c->Instance->CR1 |= I2C_CR1_STOP;

  /* Disable Last DMA */
  hi2c->Instance->CR2 &= ~I2C_CR2_LAST;

  /* Disable Acknowledge */
  hi2c->Instance->CR1 &= ~I2C_CR1_ACK;

  /* Disable DMA Request */
  hi2c->Instance->CR2 &= ~I2C_CR2_DMAEN;

  hi2c->XferCount = 0;

  /* Wait until BUSY flag is reset */
  if(g_stop[i2c_module_lookup_handler(hi2c)]) {
    if(i2c_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_BUSY, SET, I2C_TIMEOUT_FLAG) != HAL_OK)
    {
      hi2c->ErrorCode |= HAL_I2C_ERROR_TIMEOUT;
    }
  }

  hi2c->State = HAL_I2C_STATE_READY;

  /* Check if Errors has been detected during transfer */
  if(hi2c->ErrorCode != HAL_I2C_ERROR_NONE)
  {
    HAL_I2C_ErrorCallback(hi2c);
  }
  else
  {
    HAL_I2C_MasterRxCpltCallback(hi2c);
  }
}

static void i2c_DMAMasterTransmitCplt(DMA_HandleTypeDef *hdma)
{
  I2C_HandleTypeDef* hi2c = (I2C_HandleTypeDef*)((DMA_HandleTypeDef*)hdma)->Parent;

  /* Wait until BTF flag is reset */
  if(i2c_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_BTF, RESET, I2C_TIMEOUT_FLAG) != HAL_OK)
  {
    hi2c->ErrorCode |= HAL_I2C_ERROR_TIMEOUT;
  }

  /* Generate Stop */
  if(g_stop[i2c_module_lookup_handler(hi2c)]) hi2c->Instance->CR1 |= I2C_CR1_STOP;

  /* Disable DMA Request */
  hi2c->Instance->CR2 &= ~I2C_CR2_DMAEN;

  hi2c->XferCount = 0;

  /* Wait until BUSY flag is reset */
  if(g_stop[i2c_module_lookup_handler(hi2c)]) {
    if(i2c_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_BUSY, SET, I2C_TIMEOUT_FLAG) != HAL_OK)
    {
      hi2c->ErrorCode |= HAL_I2C_ERROR_TIMEOUT;
    }
  }

  hi2c->State = HAL_I2C_STATE_READY;

  /* Check if Errors has been detected during transfer */
  if(hi2c->ErrorCode != HAL_I2C_ERROR_NONE)
  {
    HAL_I2C_ErrorCallback(hi2c);
  }
  else
  {
    HAL_I2C_MasterTxCpltCallback(hi2c);
  }
}

void i2c_transfer_asynch(i2c_t *obj, const void *tx, size_t tx_length, void *rx, size_t rx_length, uint32_t address, uint32_t stop, uint32_t handler, uint32_t event, DMAUsage hint) {

    // TODO: DMA usage is currently ignored by this way
    (void) hint;  
     
    // check which use-case we have  
    int use_tx = (tx != NULL && tx_length > 0);  
    int use_rx = (rx != NULL && rx_length > 0); 
  
  	obj->tx_buff.buffer = tx;
    obj->tx_buff.length = tx_length;  
    obj->tx_buff.pos = 0;  
    obj->tx_buff.width = 8;  
  
    obj->rx_buff.buffer = rx;  
    obj->rx_buff.length = rx_length;  
    obj->rx_buff.pos = 0;  
    obj->rx_buff.width = 8;  
  
    g_event[i2c_module_lookup(obj)] = event; 
 
    g_stop_previous[i2c_module_lookup(obj)] = g_stop[i2c_module_lookup(obj)];
    g_stop[i2c_module_lookup(obj)] = stop; 
    
    // register the same thunking handler for both event and error interrupt!  
    IRQn_Type event_irq_n = I2cEventIRQs[i2c_module_lookup(obj)];  
    IRQn_Type error_irq_n = I2cErrorIRQs[i2c_module_lookup(obj)]; 
    NVIC_SetVector(event_irq_n, handler); 
    HAL_NVIC_SetPriority(event_irq_n, 0, 1);      
    HAL_NVIC_EnableIRQ(event_irq_n);
    NVIC_SetVector(error_irq_n, handler); 
    HAL_NVIC_SetPriority(error_irq_n, 0, 0);      
    HAL_NVIC_EnableIRQ(error_irq_n);
    
    /* NVIC configuration for DMA transfer complete interrupt (I2C_TX) */
    NVIC_SetVector(I2C_DMATx_IRQs[i2c_module_lookup(obj)], handler);
    HAL_NVIC_SetPriority(I2C_DMATx_IRQs[i2c_module_lookup(obj)], 0, 3);
    HAL_NVIC_EnableIRQ(I2C_DMATx_IRQs[i2c_module_lookup(obj)]);
      
    /* NVIC configuration for DMA transfer complete interrupt (I2C_RX) */
    NVIC_SetVector(I2C_DMARx_IRQs[i2c_module_lookup(obj)], handler);
    HAL_NVIC_SetPriority(I2C_DMARx_IRQs[i2c_module_lookup(obj)], 0, 2);   
    HAL_NVIC_EnableIRQ(I2C_DMARx_IRQs[i2c_module_lookup(obj)]);		
  
    I2C_HandleTypeDef *handle = &t_I2cHandle[i2c_module_lookup(obj)];
    t_I2cHandle[i2c_module_lookup(obj)].Instance = (I2C_TypeDef *)(obj->i2c.i2c);

    if (use_tx && use_rx) {
        g_receiveDMA[i2c_module_lookup(obj)]=1;     
        if (tx_length == 1) {  
            HAL_I2C_Mem_Read_DMA(handle, address, ((uint8_t*)tx)[0], I2C_MEMADD_SIZE_8BIT, rx, rx_length, g_stop_previous[i2c_module_lookup(obj)]);  
        } else if (tx_length == 2) {  
            uint16_t memoryAddress = (((uint8_t*)tx)[0] << 8) | ((uint8_t*)tx)[1];  
            HAL_I2C_Mem_Read_DMA(handle, address, memoryAddress, I2C_MEMADD_SIZE_16BIT, rx, rx_length, g_stop_previous[i2c_module_lookup(obj)]);  
        }  
    } else if (use_tx) {
        g_transmitDMA[i2c_module_lookup(obj)]=1;
        HAL_I2C_Master_Transmit_DMA(handle, address, (uint8_t*)tx, tx_length, g_stop_previous[i2c_module_lookup(obj)]);
    } else if (use_rx) {  
        g_receiveDMA[i2c_module_lookup(obj)]=1;
        HAL_I2C_Master_Receive_DMA(handle, address, (uint8_t*)rx, rx_length, g_stop_previous[i2c_module_lookup(obj)]);  
    } else {
        g_transmitIT[i2c_module_lookup(obj)]=1;
        HAL_I2C_Master_Check_IT(handle, address, g_stop_previous[i2c_module_lookup(obj)]);  
    }  

}

uint32_t i2c_irq_handler_asynch(i2c_t *obj){
  int event = 0;
  
  if(g_receiveDMA[i2c_module_lookup(obj)]) {
    g_receiveDMA[i2c_module_lookup(obj)]=0;
		t_I2cHandle[i2c_module_lookup(obj)].hdmarx->XferCpltCallback = i2c_DMAMasterReceiveCplt;
    HAL_DMA_IRQHandler(t_I2cHandle[i2c_module_lookup(obj)].hdmarx);
  }
  if(g_transmitDMA[i2c_module_lookup(obj)]) {
    g_transmitDMA[i2c_module_lookup(obj)]=0;
    t_I2cHandle[i2c_module_lookup(obj)].hdmatx->XferCpltCallback = i2c_DMAMasterTransmitCplt;
    HAL_DMA_IRQHandler(t_I2cHandle[i2c_module_lookup(obj)].hdmatx);
  }
  
  if(g_transmitIT[i2c_module_lookup(obj)]) {
    g_transmitIT[i2c_module_lookup(obj)]=0;
    __HAL_I2C_DISABLE_IT(&t_I2cHandle[i2c_module_lookup(obj)], I2C_IT_EVT);
    __HAL_I2C_DISABLE_IT(&t_I2cHandle[i2c_module_lookup(obj)], I2C_IT_ERR);
    
    if(__HAL_I2C_GET_FLAG(&t_I2cHandle[i2c_module_lookup(obj)], I2C_FLAG_AF)) {
      __HAL_I2C_CLEAR_FLAG(&t_I2cHandle[i2c_module_lookup(obj)], I2C_FLAG_AF);
      event |= I2C_EVENT_TRANSFER_EARLY_NACK;
    }
    else {
      __HAL_I2C_CLEAR_ADDRFLAG(&t_I2cHandle[i2c_module_lookup(obj)]);
      HAL_I2C_EV_IRQHandler(&t_I2cHandle[i2c_module_lookup(obj)], g_stop[i2c_module_lookup(obj)]);
    }
    // If not repeated start, send stop.
    if (g_stop[i2c_module_lookup(obj)]) {
        i2c_stop(obj);
    }
    __HAL_UNLOCK(&t_I2cHandle[i2c_module_lookup(obj)]);
    t_I2cHandle[i2c_module_lookup(obj)].State=HAL_I2C_STATE_READY;
  }
  
  event |= I2C_EVENT_TRANSFER_COMPLETE;
  
  return (event & g_event[i2c_module_lookup(obj)]);
}

uint8_t i2c_active(i2c_t *obj){
    I2C_HandleTypeDef *handle = &t_I2cHandle[i2c_module_lookup(obj)];  
    HAL_I2C_StateTypeDef state = HAL_I2C_GetState(handle);  
  
    switch(state) {  
        case HAL_I2C_STATE_RESET:  
        case HAL_I2C_STATE_READY:  
        case HAL_I2C_STATE_ERROR:  
            return 0;  
        default:  
            return 1;  
     }  
}

void i2c_abort_asynch(i2c_t *obj){
    i2c_reset(obj);  
}

#if DEVICE_I2CSLAVE

#endif //DEVICE_I2CSLAVE 

#else // !DEVICE_I2C_ASYNCH_DMA

uint32_t g_handler[MODULE_SIZE_I2C];
uint32_t g_event[MODULE_SIZE_I2C];
uint32_t g_stop[MODULE_SIZE_I2C];
uint32_t g_stop_previous[MODULE_SIZE_I2C];
uint32_t g_transmitIT[MODULE_SIZE_I2C];
uint32_t g_receiveIT[MODULE_SIZE_I2C];
uint32_t g_errorIT[MODULE_SIZE_I2C];

#if defined(I2C1_BASE)
void i2c1_irq_handler(void)
{
	I2C_HandleTypeDef *handle = &t_I2cHandle[0];
	HAL_I2C_EV_IRQHandler(handle, g_stop[0]);
	if(g_transmitIT[0]) {
   	if(handle->XferCount > 0) {
			NVIC_SetVector(I2cEventIRQs[0], (uint32_t)i2c1_irq_handler);
		} else {
			NVIC_SetVector(I2cEventIRQs[0], g_handler[0]); 
		}
	} else {
    if(handle->XferCount > 2) {
			NVIC_SetVector(I2cEventIRQs[0], (uint32_t)i2c1_irq_handler);
		} else {
			NVIC_SetVector(I2cEventIRQs[0], g_handler[0]); 
		}
	}
}
#endif
#if defined(I2C2_BASE)
void i2c2_irq_handler(void)
{
	I2C_HandleTypeDef *handle = &t_I2cHandle[1];
	HAL_I2C_EV_IRQHandler(handle, g_stop[1]);
	if(g_transmitIT[1]) {
		if(handle->XferCount > 0) {
			NVIC_SetVector(I2cEventIRQs[1], (uint32_t)i2c2_irq_handler);
		} else {
			NVIC_SetVector(I2cEventIRQs[1], g_handler[1]); 
		}
	} else {
		if(handle->XferCount > 0) {
			NVIC_SetVector(I2cEventIRQs[1], (uint32_t)i2c2_irq_handler);
		} else {
			NVIC_SetVector(I2cEventIRQs[1], g_handler[1]); 
		}
	}
}
#endif
#if defined(I2C3_BASE)
void i2c3_irq_handler(void)
{
	I2C_HandleTypeDef *handle = &t_I2cHandle[2];
	HAL_I2C_EV_IRQHandler(handle, g_stop[2]);
	if(g_transmitIT[2]) {
		if(handle->XferCount > 1) {
			NVIC_SetVector(I2cEventIRQs[2], (uint32_t)i2c3_irq_handler);
		} else {
			NVIC_SetVector(I2cEventIRQs[2], g_handler[2]); 
		}
	} else {
		if(handle->XferCount > 2) {
			NVIC_SetVector(I2cEventIRQs[2], (uint32_t)i2c3_irq_handler);
		} else {
			NVIC_SetVector(I2cEventIRQs[2], g_handler[2]); 
		}
	}
}
#endif

#if defined(I2C1_BASE) && !defined(I2C2_BASE) && !defined(I2C3_BASE)
const uint32_t t_i2c_irq_handler[]={(uint32_t)i2c1_irq_handler};
#elif defined(I2C1_BASE) && defined(I2C2_BASE) && !defined(I2C3_BASE)
const uint32_t t_i2c_irq_handler[]={(uint32_t)i2c1_irq_handler, (uint32_t)i2c2_irq_handler};
#elif defined(I2C1_BASE) && defined(I2C2_BASE) && defined(I2C3_BASE)
const uint32_t t_i2c_irq_handler[]={(uint32_t)i2c1_irq_handler, (uint32_t)i2c2_irq_handler, (uint32_t)i2c3_irq_handler};
#endif

void i2c_transfer_asynch(i2c_t *obj, const void *tx, size_t tx_length, void *rx, size_t rx_length, uint32_t address, uint32_t stop, uint32_t handler, uint32_t event, DMAUsage hint) {
    size_t i = 0;
    // TODO: DMA usage is currently ignored by this way
    (void) hint;  
     
    // check which use-case we have  
    int use_tx = (tx != NULL && tx_length > 0);  
    int use_rx = (rx != NULL && rx_length > 0); 
  
	  obj->tx_buff.buffer = (uint8_t *)malloc(8*tx_length);
    for(i=0;i<tx_length;i++) {
      ((uint8_t *)obj->tx_buff.buffer)[i] = ((uint8_t *)tx)[i];
    }
    obj->tx_buff.length = tx_length;

    obj->tx_buff.pos = 0;  
    obj->tx_buff.width = 8;  
  
    obj->rx_buff.buffer = rx;  
    obj->rx_buff.length = rx_length;  
    obj->rx_buff.pos = 0;  
    obj->rx_buff.width = 8;  

    g_handler[i2c_module_lookup(obj)] = handler;  
    g_event[i2c_module_lookup(obj)] = event; 
    g_stop_previous[i2c_module_lookup(obj)] = g_stop[i2c_module_lookup(obj)];
    g_stop[i2c_module_lookup(obj)] = stop; 
  
    // register the same thunking handler for both event and error interrupt!  
    IRQn_Type event_irq_n = I2cEventIRQs[i2c_module_lookup(obj)];  
    IRQn_Type error_irq_n = I2cErrorIRQs[i2c_module_lookup(obj)]; 
    if(((tx_length > 1)&&(rx_length == 0)) || ((rx_length > 1)&&(tx_length == 0)) || ((rx_length > 1)&&(tx_length > 1))){
			NVIC_SetVector(event_irq_n, (uint32_t)i2c1_irq_handler); 
		} else {
			NVIC_SetVector(event_irq_n, handler); 
		}
		
    HAL_NVIC_SetPriority(event_irq_n, 0, 1);      
    HAL_NVIC_EnableIRQ(event_irq_n);
    NVIC_SetVector(error_irq_n, handler); 
    HAL_NVIC_SetPriority(error_irq_n, 0, 0);      
    HAL_NVIC_EnableIRQ(error_irq_n);
    
    I2C_HandleTypeDef *handle = &t_I2cHandle[i2c_module_lookup(obj)];
    t_I2cHandle[i2c_module_lookup(obj)].Instance = (I2C_TypeDef *)(obj->i2c.i2c);

    if (use_tx && use_rx) {
        g_receiveIT[i2c_module_lookup(obj)]=1;     
        if (tx_length == 1) {  
            HAL_I2C_Mem_Read_IT(handle, address, ((uint8_t*)tx)[0], I2C_MEMADD_SIZE_8BIT, rx, rx_length, g_stop_previous[i2c_module_lookup(obj)]);  
        } else if (tx_length == 2) {  
            uint16_t memoryAddress = (((uint8_t*)tx)[0] << 8) | ((uint8_t*)tx)[1];  
            HAL_I2C_Mem_Read_IT(handle, address, memoryAddress, I2C_MEMADD_SIZE_16BIT, rx, rx_length, g_stop_previous[i2c_module_lookup(obj)]);  
        }  
    } else if (use_tx) {
        g_transmitIT[i2c_module_lookup(obj)]=1;
        HAL_I2C_Master_Transmit_IT(handle, address, (uint8_t*)tx, tx_length, g_stop_previous[i2c_module_lookup(obj)]);
    } else if (use_rx) {  
        g_receiveIT[i2c_module_lookup(obj)]=1;
        HAL_I2C_Master_Receive_IT(handle, address, (uint8_t*)rx, rx_length, g_stop_previous[i2c_module_lookup(obj)]);  
    } else {
        g_errorIT[i2c_module_lookup(obj)]=1;
        HAL_I2C_Master_Check_IT(handle, address, g_stop_previous[i2c_module_lookup(obj)]);  
    }
    
    free(obj->tx_buff.buffer);
}


uint32_t i2c_irq_handler_asynch(i2c_t *obj){
  int event = 0;
  I2C_HandleTypeDef *handle = &t_I2cHandle[i2c_module_lookup(obj)];

  // If not repeated start, send stop.
  // Patch to be sure the stop will be generated
  if (g_stop[i2c_module_lookup(obj)]) {
    if((handle->Instance->SR2 & 0x03) == 0x03) {
      i2c_stop(obj);
    }
  }
	
  if(g_receiveIT[i2c_module_lookup(obj)]) {
    g_receiveIT[i2c_module_lookup(obj)]=0;
    HAL_I2C_EV_IRQHandler(handle, g_stop[i2c_module_lookup(obj)]);
  }
  if(g_transmitIT[i2c_module_lookup(obj)]) {
    g_transmitIT[i2c_module_lookup(obj)]=0;
    HAL_I2C_EV_IRQHandler(handle, g_stop[i2c_module_lookup(obj)]);
  }
  
  if(g_errorIT[i2c_module_lookup(obj)]) {
    g_errorIT[i2c_module_lookup(obj)]=0;
    __HAL_I2C_DISABLE_IT(&t_I2cHandle[i2c_module_lookup(obj)], I2C_IT_EVT);
    __HAL_I2C_DISABLE_IT(&t_I2cHandle[i2c_module_lookup(obj)], I2C_IT_ERR);
    
    if(__HAL_I2C_GET_FLAG(&t_I2cHandle[i2c_module_lookup(obj)], I2C_FLAG_AF)) {
      __HAL_I2C_CLEAR_FLAG(&t_I2cHandle[i2c_module_lookup(obj)], I2C_FLAG_AF);
      event |= I2C_EVENT_TRANSFER_EARLY_NACK;
    }
    else {
      __HAL_I2C_CLEAR_ADDRFLAG(&t_I2cHandle[i2c_module_lookup(obj)]);
      HAL_I2C_EV_IRQHandler(&t_I2cHandle[i2c_module_lookup(obj)], g_stop[i2c_module_lookup(obj)]);
    }
    __HAL_UNLOCK(&t_I2cHandle[i2c_module_lookup(obj)]);
  }

	__HAL_I2C_DISABLE_IT(&t_I2cHandle[i2c_module_lookup(obj)], I2C_IT_EVT|I2C_IT_ERR|I2C_IT_BUF);
  t_I2cHandle[i2c_module_lookup(obj)].State=HAL_I2C_STATE_READY;	
  event |= I2C_EVENT_TRANSFER_COMPLETE;
  
  return (event & g_event[i2c_module_lookup(obj)]);
}

uint8_t i2c_active(i2c_t *obj){
    I2C_HandleTypeDef *handle = &t_I2cHandle[i2c_module_lookup(obj)];  
    HAL_I2C_StateTypeDef state = HAL_I2C_GetState(handle);  
  
    switch(state) {  
        case HAL_I2C_STATE_RESET:  
        case HAL_I2C_STATE_READY:  
        case HAL_I2C_STATE_ERROR:  
            return 0;  
        default:  
            return 1;  
     }  
}

void i2c_abort_asynch(i2c_t *obj){
    i2c_reset(obj);  
}

#if DEVICE_I2CSLAVE

#endif //DEVICE_I2CSLAVE
#endif // DEVICE_I2C_ASYNCH_DMA
#endif // DEVICE_I2C_ASYNCH
#endif // DEVICE_I2C
