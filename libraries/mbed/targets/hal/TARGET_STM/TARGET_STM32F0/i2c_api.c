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

#ifndef USE_STM32F0XX_HAL_I2C__FIX
#error Please use stm32f0xx_hal_i2c__FIX.c
#endif

#include "cmsis.h"
#include "pinmap.h"
#include "PeripheralPins.h"
#include "stdlib.h"

#define I2C_TIMEOUT_TCR     ((uint32_t)25)     /* 25 ms */
#define I2C_TIMEOUT_STOPF   ((uint32_t)25)     /* 25 ms */
#define I2C_TIMEOUT_TXIS    ((uint32_t)25)     /* 25 ms */
#define I2C_TIMEOUT_RXNE    ((uint32_t)25)     /* 25 ms */
//#define I2C_TIMEOUT_FLAG    ((uint32_t)25)     /* 25 ms */

/* Timeout values for flags and events waiting loops. These timeouts are
   not based on accurate values, they just guarantee that the application will
   not remain stuck if the I2C communication is corrupted. */
#define FLAG_TIMEOUT ((int)0x1000)
#define LONG_TIMEOUT ((int)0x8000)

#if defined(I2C2_Base)
  #define MODULE_SIZE_I2C 2
  #if DEVICE_I2C_ASYNCH
static const IRQn_Type I2cEventIRQs[MODULE_SIZE_I2C] = {  
    I2C1_IRQn,  
    I2C2_IRQn,  
};  
static const IRQn_Type I2cErrorIRQs[MODULE_SIZE_I2C] = {  
    I2C1_IRQn,  
    I2C2_IRQn,  
};
  #endif
#else
  #define MODULE_SIZE_I2C 1
  #if DEVICE_I2C_ASYNCH
static const IRQn_Type I2cEventIRQs[MODULE_SIZE_I2C] = {  
    I2C1_IRQn,  
};  
static const IRQn_Type I2cErrorIRQs[MODULE_SIZE_I2C] = {  
    I2C1_IRQn,  
};
  #endif
#endif

I2C_HandleTypeDef t_I2cHandle[MODULE_SIZE_I2C];

int i2c1_inited = 0;
#if defined(I2C2_BASE)
int i2c2_inited = 0;
#endif

#if !DEVICE_I2C_ASYNCH
int i2c_module_lookup(i2c_t *obj)
{
	switch(obj->i2c)
	{
		case I2C_1: return 0;
#if defined(I2C2_Base)
		case I2C_2: return 1;
#endif		
		default: return 0;
	}
}
#else
int i2c_module_lookup(i2c_t *obj)
{
	switch(obj->i2c.i2c)
	{
		case I2C_1: return 0;
#if defined(I2C2_Base)
		case I2C_2: return 1;
#endif		
		default: return 0;
	}
}
#endif


void i2c_init(i2c_t *obj, PinName sda, PinName scl) {
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
        __HAL_RCC_I2C1_CONFIG(RCC_I2C1CLKSOURCE_SYSCLK);
        __I2C1_CLK_ENABLE();
        // Configure I2C pins
        pinmap_pinout(sda, PinMap_I2C_SDA);
        pinmap_pinout(scl, PinMap_I2C_SCL);
        pin_mode(sda, OpenDrain);
        pin_mode(scl, OpenDrain);
    }

#if defined(I2C2_BASE)
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
#endif

    // Reset to clear pending flags if any
    i2c_reset(obj);

    // I2C configuration
    i2c_frequency(obj, 100000); // 100 kHz per default
}

void i2c_frequency(i2c_t *obj, int hz) {
    MBED_ASSERT((hz == 100000) || (hz == 400000) || (hz == 1000000));
#if !DEVICE_I2C_ASYNCH	
    I2C_HandleTypeDef *handle = &t_I2cHandle[i2c_module_lookup(obj)];
    t_I2cHandle[i2c_module_lookup(obj)].Instance = (I2C_TypeDef *)(obj->i2c);
#else
    I2C_HandleTypeDef *handle = &t_I2cHandle[i2c_module_lookup(obj)];
    t_I2cHandle[i2c_module_lookup(obj)].Instance = (I2C_TypeDef *)(obj->i2c.i2c);
#endif	
    int timeout;

    // wait before init
    timeout = LONG_TIMEOUT;
    while ((__HAL_I2C_GET_FLAG(handle, I2C_FLAG_BUSY)) && (timeout-- != 0));

    // Common settings: I2C clock = 48 MHz, Analog filter = ON, Digital filter coefficient = 0
    switch (hz) {
        case 100000:
            handle->Init.Timing = 0x10805E89; // Standard mode with Rise Time = 400ns and Fall Time = 100ns
            break;
        case 400000:
            handle->Init.Timing = 0x00901850; // Fast mode with Rise Time = 250ns and Fall Time = 100ns
            break;
        case 1000000:
            handle->Init.Timing = 0x00700818; // Fast mode Plus with Rise Time = 60ns and Fall Time = 100ns
            break;
        default:
            break;
    }

    // I2C configuration
    handle->Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
    handle->Init.DualAddressMode  = I2C_DUALADDRESS_DISABLED;
    handle->Init.GeneralCallMode  = I2C_GENERALCALL_DISABLED;
    handle->Init.NoStretchMode    = I2C_NOSTRETCH_DISABLED;
    handle->Init.OwnAddress1      = 0;
    handle->Init.OwnAddress2      = 0;
    handle->Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    HAL_I2C_Init(handle);
		
#if DEVICE_I2C_ASYNCH_DMA
    static DMA_HandleTypeDef hdma_tx;
    static DMA_HandleTypeDef hdma_rx;
    handle->Instance = (I2C_TypeDef *)(obj->i2c.i2c);

    if(handle->Instance == I2C2) {
      hdma_tx.Instance                 = DMA1_Channel4;
      hdma_rx.Instance                 = DMA1_Channel5;
    }
    else {
      hdma_tx.Instance                 = DMA1_Channel2;
      hdma_rx.Instance                 = DMA1_Channel3;
    }

    /* Enable DMAx clock */

    __HAL_RCC_DMA1_CLK_ENABLE();   
    
    /* Configure the DMA handler for Transmission process */
    hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
    hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
    hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    hdma_tx.Init.Mode                = DMA_NORMAL;
    hdma_tx.Init.Priority            = DMA_PRIORITY_LOW;

    HAL_DMA_Init(&hdma_tx);   

    /* Associate the initialized DMA handle to the the I2C handle */
    __HAL_LINKDMA(handle, hdmatx, hdma_tx);
      
    /* Configure the DMA handler for Reception process */
    hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
    hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    hdma_rx.Init.Mode                = DMA_NORMAL;
    hdma_rx.Init.Priority            = DMA_PRIORITY_HIGH;

    HAL_DMA_Init(&hdma_rx);
      
    /* Associate the initialized DMA handle to the the I2C handle */
    __HAL_LINKDMA(handle, hdmarx, hdma_rx);
    
#endif
}

inline int i2c_start(i2c_t *obj) {
#if !DEVICE_I2C_ASYNCH	
    I2C_TypeDef *i2c = (I2C_TypeDef *)(obj->i2c);
    I2C_HandleTypeDef *handle = &t_I2cHandle[i2c_module_lookup(obj)];
    t_I2cHandle[i2c_module_lookup(obj)].Instance = (I2C_TypeDef *)(obj->i2c);
#else
    I2C_TypeDef *i2c = (I2C_TypeDef *)(obj->i2c.i2c);
    I2C_HandleTypeDef *handle = &t_I2cHandle[i2c_module_lookup(obj)];
    t_I2cHandle[i2c_module_lookup(obj)].Instance = (I2C_TypeDef *)(obj->i2c.i2c);  
#endif	
    int timeout;

    // Clear Acknowledge failure flag
    __HAL_I2C_CLEAR_FLAG(handle, I2C_FLAG_AF);

    // Wait the STOP condition has been previously correctly sent
    timeout = FLAG_TIMEOUT;
    while ((i2c->CR2 & I2C_CR2_STOP) == I2C_CR2_STOP){
        if ((timeout--) == 0) {
            return 1;
        }
    }

    // Generate the START condition
    i2c->CR2 |= I2C_CR2_START;

    // Wait the START condition has been correctly sent
    timeout = FLAG_TIMEOUT;
    while (__HAL_I2C_GET_FLAG(handle, I2C_FLAG_BUSY) == RESET) {
        if ((timeout--) == 0) {
            return 1;
        }
    }

    return 0;
}

inline int i2c_stop(i2c_t *obj) {
#if !DEVICE_I2C_ASYNCH
    I2C_TypeDef *i2c = (I2C_TypeDef *)(obj->i2c);
#else
    I2C_TypeDef *i2c = (I2C_TypeDef *)(obj->i2c.i2c);
#endif

    // Generate the STOP condition
    i2c->CR2 |= I2C_CR2_STOP;

    return 0;
}

int i2c_read(i2c_t *obj, int address, char *data, int length, int stop) {
#if !DEVICE_I2C_ASYNCH
    I2C_TypeDef *i2c = (I2C_TypeDef *)(obj->i2c);
    I2C_HandleTypeDef *handle = &t_I2cHandle[i2c_module_lookup(obj)];
    t_I2cHandle[i2c_module_lookup(obj)].Instance = (I2C_TypeDef *)(obj->i2c);
#else
    I2C_TypeDef *i2c = (I2C_TypeDef *)(obj->i2c.i2c);
    I2C_HandleTypeDef *handle = &t_I2cHandle[i2c_module_lookup(obj)];
    t_I2cHandle[i2c_module_lookup(obj)].Instance = (I2C_TypeDef *)(obj->i2c.i2c);
#endif	
    int timeout;
    int count;
    int value;

    // Update CR2 register
    i2c->CR2 = (i2c->CR2 & (uint32_t)~((uint32_t)(I2C_CR2_SADD | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_AUTOEND | I2C_CR2_RD_WRN | I2C_CR2_START | I2C_CR2_STOP)))
               | (uint32_t)(((uint32_t)address & I2C_CR2_SADD) | (((uint32_t)length << 16) & I2C_CR2_NBYTES) | (uint32_t)I2C_SOFTEND_MODE | (uint32_t)I2C_GENERATE_START_READ);

    // Read all bytes
    for (count = 0; count < length; count++) {
        value = i2c_byte_read(obj, 0);
        data[count] = (char)value;
    }

    // Wait transfer complete
    timeout = FLAG_TIMEOUT;
    while (__HAL_I2C_GET_FLAG(handle, I2C_FLAG_TC) == RESET) {
        timeout--;
        if (timeout == 0) {
            return -1;
        }
    }
    __HAL_I2C_CLEAR_FLAG(handle, I2C_FLAG_TC);

    // If not repeated start, send stop.
    if (stop) {
        i2c_stop(obj);
        // Wait until STOPF flag is set
        timeout = FLAG_TIMEOUT;
        while (__HAL_I2C_GET_FLAG(handle, I2C_FLAG_STOPF) == RESET) {
            timeout--;
            if (timeout == 0) {
                return -1;
            }
        }
        // Clear STOP Flag
        __HAL_I2C_CLEAR_FLAG(handle, I2C_FLAG_STOPF);
    }

    return length;
}

int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop) {
#if !DEVICE_I2C_ASYNCH
    I2C_TypeDef *i2c = (I2C_TypeDef *)(obj->i2c);
    I2C_HandleTypeDef *handle = &t_I2cHandle[i2c_module_lookup(obj)];
    t_I2cHandle[i2c_module_lookup(obj)].Instance = (I2C_TypeDef *)(obj->i2c);
#else
    I2C_TypeDef *i2c = (I2C_TypeDef *)(obj->i2c.i2c);
    I2C_HandleTypeDef *handle = &t_I2cHandle[i2c_module_lookup(obj)];
    t_I2cHandle[i2c_module_lookup(obj)].Instance = (I2C_TypeDef *)(obj->i2c.i2c);
#endif
    int timeout;
    int count;

    // Update CR2 register
    i2c->CR2 = (i2c->CR2 & (uint32_t)~((uint32_t)(I2C_CR2_SADD | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_AUTOEND | I2C_CR2_RD_WRN | I2C_CR2_START | I2C_CR2_STOP)))
               | (uint32_t)(((uint32_t)address & I2C_CR2_SADD) | (((uint32_t)length << 16) & I2C_CR2_NBYTES) | (uint32_t)I2C_SOFTEND_MODE | (uint32_t)I2C_GENERATE_START_WRITE);

    for (count = 0; count < length; count++) {
        i2c_byte_write(obj, data[count]);
    }

    // Wait transfer complete
    timeout = FLAG_TIMEOUT;
    while (__HAL_I2C_GET_FLAG(handle, I2C_FLAG_TC) == RESET) {
        timeout--;
        if (timeout == 0) {
            return -1;
        }
    }
    __HAL_I2C_CLEAR_FLAG(handle, I2C_FLAG_TC);

    // If not repeated start, send stop
    if (stop) {
        i2c_stop(obj);
        // Wait until STOPF flag is set
        timeout = FLAG_TIMEOUT;
        while (__HAL_I2C_GET_FLAG(handle, I2C_FLAG_STOPF) == RESET) {
            timeout--;
            if (timeout == 0) {
                return -1;
            }
        }
        // Clear STOP Flag
        __HAL_I2C_CLEAR_FLAG(handle, I2C_FLAG_STOPF);
    }

    return count;
}

int i2c_byte_read(i2c_t *obj, int last) {
#if !DEVICE_I2C_ASYNCH
    I2C_TypeDef *i2c = (I2C_TypeDef *)(obj->i2c);
    I2C_HandleTypeDef *handle = &t_I2cHandle[i2c_module_lookup(obj)];
    t_I2cHandle[i2c_module_lookup(obj)].Instance = (I2C_TypeDef *)(obj->i2c);
#else
    I2C_TypeDef *i2c = (I2C_TypeDef *)(obj->i2c.i2c);
    I2C_HandleTypeDef *handle = &t_I2cHandle[i2c_module_lookup(obj)];
    t_I2cHandle[i2c_module_lookup(obj)].Instance = (I2C_TypeDef *)(obj->i2c.i2c);
#endif
    int timeout;

    // Wait until the byte is received
    timeout = FLAG_TIMEOUT;
    while (__HAL_I2C_GET_FLAG(handle, I2C_FLAG_RXNE) == RESET) {
        if ((timeout--) == 0) {
            return -1;
        }
    }

    return (int)i2c->RXDR;
}

int i2c_byte_write(i2c_t *obj, int data) {
#if !DEVICE_I2C_ASYNCH
    I2C_TypeDef *i2c = (I2C_TypeDef *)(obj->i2c);
    I2C_HandleTypeDef *handle = &t_I2cHandle[i2c_module_lookup(obj)];
    t_I2cHandle[i2c_module_lookup(obj)].Instance = (I2C_TypeDef *)(obj->i2c);
#else
    I2C_TypeDef *i2c = (I2C_TypeDef *)(obj->i2c.i2c);
    I2C_HandleTypeDef *handle = &t_I2cHandle[i2c_module_lookup(obj)];
    t_I2cHandle[i2c_module_lookup(obj)].Instance = (I2C_TypeDef *)(obj->i2c.i2c);
#endif
    int timeout;

    // Wait until the previous byte is transmitted
    timeout = FLAG_TIMEOUT;
    while (__HAL_I2C_GET_FLAG(handle, I2C_FLAG_TXIS) == RESET) {
        if ((timeout--) == 0) {
            return 0;
        }
    }

    i2c->TXDR = (uint8_t)data;

    return 1;
}

void i2c_reset(i2c_t *obj) {
    int timeout;
#if !DEVICE_I2C_ASYNCH
    I2C_HandleTypeDef *handle = &t_I2cHandle[i2c_module_lookup(obj)];
    t_I2cHandle[i2c_module_lookup(obj)].Instance = (I2C_TypeDef *)(obj->i2c);
#else
    I2C_HandleTypeDef *handle = &t_I2cHandle[i2c_module_lookup(obj)];
    t_I2cHandle[i2c_module_lookup(obj)].Instance = (I2C_TypeDef *)(obj->i2c.i2c);
#endif

    // Wait before reset
    timeout = LONG_TIMEOUT;
    while ((__HAL_I2C_GET_FLAG(handle, I2C_FLAG_BUSY)) && (timeout-- != 0));

#if !DEVICE_I2C_ASYNCH
	  if (obj->i2c == I2C_1) {
#else			
	  if (obj->i2c.i2c == I2C_1) {
#endif	
        __I2C1_FORCE_RESET();
        __I2C1_RELEASE_RESET();
    }
#if defined(I2C2_BASE)
#if !DEVICE_I2C_ASYNCH
    if (obj->i2c == I2C_2) {
#else			
    if (obj->i2c.i2c == I2C_2) {
#endif	
        __I2C2_FORCE_RESET();
        __I2C2_RELEASE_RESET();
    }
#endif
}

#if DEVICE_I2CSLAVE

void i2c_slave_address(i2c_t *obj, int idx, uint32_t address, uint32_t mask) {
#if !DEVICE_I2C_ASYNCH
    I2C_TypeDef *i2c = (I2C_TypeDef *)(obj->i2c);
#else
    I2C_TypeDef *i2c = (I2C_TypeDef *)(obj->i2c.i2c);
#endif
    uint16_t tmpreg = 0;

    // disable
    i2c->OAR1 &= (uint32_t)(~I2C_OAR1_OA1EN);
    // Get the old register value
    tmpreg = i2c->OAR1;
    // Reset address bits
    tmpreg &= 0xFC00;
    // Set new address
    tmpreg |= (uint16_t)((uint16_t)address & (uint16_t)0x00FE); // 7-bits
    // Store the new register value
    i2c->OAR1 = tmpreg;
    // enable
    i2c->OAR1 |= I2C_OAR1_OA1EN;
}

void i2c_slave_mode(i2c_t *obj, int enable_slave) {
#if !DEVICE_I2C_ASYNCH
    I2C_TypeDef *i2c = (I2C_TypeDef *)(obj->i2c);
#else
    I2C_TypeDef *i2c = (I2C_TypeDef *)(obj->i2c.i2c);
#endif
    uint16_t tmpreg;

    // Get the old register value
    tmpreg = i2c->OAR1;

    // Enable / disable slave
    if (enable_slave == 1) {
        tmpreg |= I2C_OAR1_OA1EN;
    } else {
        tmpreg &= (uint32_t)(~I2C_OAR1_OA1EN);
    }

    // Set new mode
    i2c->OAR1 = tmpreg;
}

// See I2CSlave.h
#define NoData         0 // the slave has not been addressed
#define ReadAddressed  1 // the master has requested a read from this slave (slave = transmitter)
#define WriteGeneral   2 // the master is writing to all slave
#define WriteAddressed 3 // the master is writing to this slave (slave = receiver)

int i2c_slave_receive(i2c_t *obj) {
#if !DEVICE_I2C_ASYNCH
    I2C_HandleTypeDef *handle = &t_I2cHandle[i2c_module_lookup(obj)];
    t_I2cHandle[i2c_module_lookup(obj)].Instance = (I2C_TypeDef *)(obj->i2c);
#else
    I2C_HandleTypeDef *handle = &t_I2cHandle[i2c_module_lookup(obj)];
    t_I2cHandle[i2c_module_lookup(obj)].Instance = (I2C_TypeDef *)(obj->i2c.i2c);
#endif
    int retValue = NoData;

    if (__HAL_I2C_GET_FLAG(handle, I2C_FLAG_BUSY) == 1) {
        if (__HAL_I2C_GET_FLAG(handle, I2C_FLAG_ADDR) == 1) {
            if (__HAL_I2C_GET_FLAG(handle, I2C_FLAG_DIR) == 1)
                retValue = ReadAddressed;
            else
                retValue = WriteAddressed;

            __HAL_I2C_CLEAR_FLAG(handle, I2C_FLAG_ADDR);
        }
    }

    return (retValue);
}

int i2c_slave_read(i2c_t *obj, char *data, int length) {
    char size = 0;

    while (size < length) data[size++] = (char)i2c_byte_read(obj, 0);

    return size;
}

int i2c_slave_write(i2c_t *obj, const char *data, int length) {
    char size = 0;

    do {
        i2c_byte_write(obj, data[size]);
        size++;
    } while (size < length);

    return size;
}


#endif // DEVICE_I2CSLAVE


#if DEVICE_I2C_ASYNCH

uint32_t g_event[MODULE_SIZE_I2C];
uint32_t g_address[MODULE_SIZE_I2C];
uint32_t g_stop[MODULE_SIZE_I2C];
uint32_t g_stop_previous[MODULE_SIZE_I2C];

#if !DEVICE_I2C_ASYNCH_DMA

void i2c_transfer_asynch(i2c_t *obj, const void *tx, size_t tx_length, void *rx, size_t rx_length, uint32_t address, uint32_t stop, uint32_t handler, uint32_t event, DMAUsage hint)
{
    size_t i = 0;

    // TODO: DMA usage is currently ignored  
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
  
    g_event[i2c_module_lookup(obj)] = event;  
    g_address[i2c_module_lookup(obj)] = address;
    g_stop_previous[i2c_module_lookup(obj)] = g_stop[i2c_module_lookup(obj)];
    g_stop[i2c_module_lookup(obj)] = stop; 
 
    // register the same thunking handler for both event and error interrupt!  
    IRQn_Type event_irq_n = I2cEventIRQs[i2c_module_lookup(obj)];  
    IRQn_Type error_irq_n = I2cErrorIRQs[i2c_module_lookup(obj)]; 
    NVIC_SetVector(event_irq_n, handler);
    NVIC_SetVector(error_irq_n, handler);
    NVIC_EnableIRQ(event_irq_n);
    NVIC_EnableIRQ(error_irq_n);		
  
    I2C_HandleTypeDef *handle = &t_I2cHandle[i2c_module_lookup(obj)];
    t_I2cHandle[i2c_module_lookup(obj)].Instance = (I2C_TypeDef *)(obj->i2c.i2c);
		
    if (use_tx && use_rx) {  
        // these methods are BLOCKING for creating the start condition and writing the first bytes!  
        if (tx_length == 1) {  
            HAL_I2C_Mem_Read_IT(handle, address, ((uint8_t*)tx)[0], I2C_MEMADD_SIZE_8BIT, rx, rx_length, stop, g_stop_previous[i2c_module_lookup(obj)]);  
        } else if (tx_length == 2) {  
            uint16_t memoryAddress = (((uint8_t*)tx)[0] << 8) | ((uint8_t*)tx)[1];  
            HAL_I2C_Mem_Read_IT(handle, address, memoryAddress, I2C_MEMADD_SIZE_16BIT, rx, rx_length, stop, g_stop_previous[i2c_module_lookup(obj)]);  
        }  
    // these methods are BLOCKING for creating the start condition!  
    } else if (use_tx) {  
        HAL_I2C_Master_Transmit_IT(handle, address, (uint8_t*)tx, tx_length, stop, g_stop_previous[i2c_module_lookup(obj)]);  
    } else if (use_rx) {  
        HAL_I2C_Master_Receive_IT(handle, address, (uint8_t*)rx, rx_length, stop, g_stop_previous[i2c_module_lookup(obj)]);  
    } else {  
        HAL_I2C_Master_Check_IT(handle, address, stop, g_stop_previous[i2c_module_lookup(obj)]);  
    }  
}  
  
uint32_t i2c_irq_handler_asynch(i2c_t *obj)  
{  
    I2C_HandleTypeDef *handle = &t_I2cHandle[i2c_module_lookup(obj)];
    t_I2cHandle[i2c_module_lookup(obj)].Instance = (I2C_TypeDef *)(obj->i2c.i2c);
  
    /* So we map the event and error interrupt on the same thunking handle.  
     * In order to distinguish between both, we need to check the flags in  
     * the status register.  
     *  
     * The Event interrupt is generated when:  
     *  – SB = 1 (Master)  
     *  – ADDR = 1 (Master/Slave)  
     *  – ADD10 = 1 (Master)  
     *  – STOPF = 1 (Slave)  
     *  – BTF = 1 with no TxE or RxNE event  
     *  – TxE event to 1 if ITBUFEN = 1  
     *  – RxNE event to 1if ITBUFEN = 1  
     *  
     * The Error interrupt is generated when:  
     *  – BERR=1        Misplaced Start or Stop condition  
     *  – ARLO=1        Arbitration lost (only in multi-master mode)  
     *  – AF=1          Acknowledge Failure  
     *  – OVR=1         Overrun/Underrun  
     *  – PECERR = 1    PEC Error in reception (not applicable)  
     *  – TIMEOUT = 1   SCL remained LOW for 25 ms (Timeout) (only in SMBus mode)  
     *  – SMBALERT = 1  SMBus alert (not applicable, we don't use SMBus)  
     *  
     * All these flags are in the Status Register 1, and all event flags are  
     * in the lower 8 bits, while all error flags are in the upper 8 bits.  
     */  
    int status = handle->Instance->ISR;  
    int event = 0;  
    HAL_I2C_StateTypeDef previousState = HAL_I2C_GetState(handle);  
    HAL_I2C_StateTypeDef currentState;  
  
    // check error flags first, since they take presedence over events!  
    if (status & 0x7f00) {  
        // update buffer positions  
        size_t position = handle->XferSize - handle->XferCount;  
        if (previousState & 0x20) {  
            obj->rx_buff.pos = position;  
        } else {  
            obj->tx_buff.pos = position;  
        }  
  
        HAL_I2C_ER_IRQHandler(handle);  
        currentState = HAL_I2C_GetState(handle);  
  
        event = I2C_EVENT_ERROR;  
        if (HAL_I2C_GetError(handle) & HAL_I2C_ERROR_AF) {  
            event |= I2C_EVENT_TRANSFER_EARLY_NACK;  
        }  
  
        return (event & g_event[i2c_module_lookup(obj)]);  
    }  
  
    // check event flags next  
    if (status & 0x00ff) {  
			// busy w/ no stop
				if ((status == 0x8081) || (status == 0x8041)) { 

				 /* Disable ERR, TC, STOP, NACK, TXI interrupt */
					__HAL_I2C_DISABLE_IT(handle,I2C_IT_ERRI | I2C_IT_TCI| I2C_IT_STOPI| I2C_IT_NACKI | I2C_IT_TXI );

					/* Clear Configuration Register 2 */
					I2C_RESET_CR2(handle);

					handle->State = HAL_I2C_STATE_READY;
				
					event = I2C_EVENT_TRANSFER_COMPLETE;
				}
				else{
					HAL_I2C_EV_IRQHandler(handle, g_stop[i2c_module_lookup(obj)]);  
					currentState = HAL_I2C_GetState(handle);  
				
					if (currentState == HAL_I2C_STATE_READY) {  
							// update buffer positions  
							size_t position = handle->XferSize - handle->XferCount;  
							if (previousState & 0x20) {  
									obj->rx_buff.pos = position;  
							} else {  
									obj->tx_buff.pos = position;  
							}  
							event = I2C_EVENT_TRANSFER_COMPLETE;
							
							if(__HAL_I2C_GET_FLAG(handle,I2C_FLAG_AF)) 
							{
								event |= I2C_EVENT_TRANSFER_EARLY_NACK;
							}
					}  
			}
    }  
    return (event & g_event[i2c_module_lookup(obj)]);
}  
  
uint8_t i2c_active(i2c_t *obj)  
{  
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
    
void i2c_abort_asynch(i2c_t *obj)  
{  
    i2c_reset(obj);  
}

#if DEVICE_I2CSLAVE

#endif //DEVICE_I2CSLAVE

#else //DEVICE_I2C_ASYNCH_DMA

#if defined(I2C2_Base)
static const IRQn_Type I2C_DMATx_IRQs[MODULE_SIZE_I2C] = {  
    DMA1_Ch2_3_DMA2_Ch1_2_IRQn,  
    DMA1_Ch4_7_DMA2_Ch3_5_IRQn,  
};
static const IRQn_Type I2C_DMARx_IRQs[MODULE_SIZE_I2C] = {  
    DMA1_Ch2_3_DMA2_Ch1_2_IRQn,  
    DMA1_Ch4_7_DMA2_Ch3_5_IRQn,  
};
#else
static const IRQn_Type I2C_DMATx_IRQs[MODULE_SIZE_I2C] = {  
    DMA1_Ch2_3_DMA2_Ch1_2_IRQn,  
};
static const IRQn_Type I2C_DMARx_IRQs[MODULE_SIZE_I2C] = {  
    DMA1_Ch2_3_DMA2_Ch1_2_IRQn,  
};
#endif

int dma_module_lookup(DMA_HandleTypeDef *hdma)
{
	switch((uint32_t)&(hdma->Instance))
	{
    case DMA1_Channel2_BASE:
		case DMA1_Channel3_BASE: return 0;
#if defined(I2C2_Base)
    case DMA1_Channel4_BASE:
		case DMA1_Channel5_BASE: return 1;
#endif		
		default: return 0;
	}
}

static HAL_StatusTypeDef i2c_WaitOnFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Flag, FlagStatus Status, uint32_t Timeout)  
{  
  uint32_t tickstart = HAL_GetTick();

  /* Wait until flag is set */
  if(Status == RESET)
  {    
    while(__HAL_I2C_GET_FLAG(hi2c, Flag) == RESET)
    {
      /* Check for the Timeout */
      if(Timeout != HAL_MAX_DELAY)
      {
        if((Timeout == 0) || ((HAL_GetTick() - tickstart) > Timeout))
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
        if((Timeout == 0) || ((HAL_GetTick() - tickstart) > Timeout))
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

static HAL_StatusTypeDef i2c_IsAcknowledgeFailed(I2C_HandleTypeDef *hi2c, uint32_t Timeout)
{
  uint32_t tickstart = 0x00;
  tickstart = HAL_GetTick();

  if(__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_AF) == SET)
  {
    /* Generate stop if necessary only in case of I2C peripheral in MASTER mode */
    if((hi2c->State == HAL_I2C_STATE_MASTER_BUSY_TX) || (hi2c->State == HAL_I2C_STATE_MEM_BUSY_TX)
       || (hi2c->State == HAL_I2C_STATE_MEM_BUSY_RX))
    {
      /* No need to generate the STOP condition if AUTOEND mode is enabled */
      /* Generate the STOP condition only in case of SOFTEND mode is enabled */
      if((hi2c->Instance->CR2 & I2C_AUTOEND_MODE) != I2C_AUTOEND_MODE)
      {
        /* Generate Stop */
        hi2c->Instance->CR2 |= I2C_CR2_STOP;
      }
    }
		
    /* Wait until STOP Flag is reset */
    /* AutoEnd should be initiate after AF */
    while(__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_STOPF) == RESET)
    {
      /* Check for the Timeout */
      if(Timeout != HAL_MAX_DELAY)
      {
        if((Timeout == 0) || ((HAL_GetTick() - tickstart) > Timeout))
        {
          hi2c->State= HAL_I2C_STATE_READY;
          /* Process Unlocked */
          __HAL_UNLOCK(hi2c);
          return HAL_TIMEOUT;
        }
      }
    }

    /* Clear NACKF Flag */
    __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_AF);

    /* Clear STOP Flag */
    __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_STOPF);

    /* Clear Configuration Register 2 */
    I2C_RESET_CR2(hi2c);

    hi2c->ErrorCode = HAL_I2C_ERROR_AF;
    hi2c->State= HAL_I2C_STATE_READY;

    /* Process Unlocked */
    __HAL_UNLOCK(hi2c);

    return HAL_ERROR;
  }
  return HAL_OK;      
}

static HAL_StatusTypeDef i2c_WaitOnSTOPFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Timeout)
{  
  uint32_t tickstart = 0x00;
  tickstart = HAL_GetTick();
  
  while(__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_STOPF) == RESET)
  {
    /* Check if a NACK is detected */
    if(i2c_IsAcknowledgeFailed(hi2c, Timeout) != HAL_OK)
    {
      return HAL_ERROR;
    }

    /* Check for the Timeout */
    if((Timeout == 0) || ((HAL_GetTick() - tickstart) > Timeout))
    {
      hi2c->ErrorCode |= HAL_I2C_ERROR_TIMEOUT;
      hi2c->State= HAL_I2C_STATE_READY;

      /* Process Unlocked */
      __HAL_UNLOCK(hi2c);

      return HAL_TIMEOUT;
    }
  }
  return HAL_OK;
}

static void i2c_TransferConfig(I2C_HandleTypeDef *hi2c,  uint16_t DevAddress, uint8_t Size, uint32_t Mode, uint32_t Request)
{
  uint32_t tmpreg = 0;
  
  /* Check the parameters */
  assert_param(IS_I2C_ALL_INSTANCE(hi2c->Instance));
  assert_param(IS_TRANSFER_MODE(Mode));
  assert_param(IS_TRANSFER_REQUEST(Request));
    
  /* Get the CR2 register value */
  tmpreg = hi2c->Instance->CR2;
  
  /* clear tmpreg specific bits */
  tmpreg &= (uint32_t)~((uint32_t)(I2C_CR2_SADD | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_AUTOEND | I2C_CR2_RD_WRN | I2C_CR2_START | I2C_CR2_STOP));
  
  /* update tmpreg */
  tmpreg |= (uint32_t)(((uint32_t)DevAddress & I2C_CR2_SADD) | (((uint32_t)Size << 16 ) & I2C_CR2_NBYTES) | \
            (uint32_t)Mode | (uint32_t)Request);
  
  /* update CR2 register */
  hi2c->Instance->CR2 = tmpreg;  
}  

static HAL_StatusTypeDef i2c_WaitOnTXISFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Timeout)  
{  
  uint32_t tickstart = HAL_GetTick();
  
  while(__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_TXIS) == RESET)
  {
    /* Check if a NACK is detected */
    if(i2c_IsAcknowledgeFailed(hi2c, Timeout) != HAL_OK)
    {
      return HAL_ERROR;
    }

    /* Check for the Timeout */
    if(Timeout != HAL_MAX_DELAY)
    {
      if((Timeout == 0) || ((HAL_GetTick() - tickstart) > Timeout))
      {
        hi2c->ErrorCode |= HAL_I2C_ERROR_TIMEOUT;
        hi2c->State= HAL_I2C_STATE_READY;

        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);

        return HAL_TIMEOUT;
      }
    }
  }
  return HAL_OK;      
}

static void i2c_DMAMasterReceiveCplt(DMA_HandleTypeDef *hdma)   
{  
  I2C_HandleTypeDef* hi2c = ( I2C_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;  
  uint16_t DevAddress;
  
  /* Check if last DMA request was done with RELOAD */
  /* Set NBYTES to write and reload if size > 255 */
  if( (hi2c->XferSize == 255) && (hi2c->XferSize < hi2c->XferCount) )
  {
    /* Wait until TCR flag is set */
    if(i2c_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_TCR, RESET, I2C_TIMEOUT_TCR) != HAL_OK)      
    {
      hi2c->ErrorCode |= HAL_I2C_ERROR_TIMEOUT;
    }

    /* Disable DMA Request */
    hi2c->Instance->CR1 &= ~I2C_CR1_RXDMAEN; 

    /* Check if Errors has been detected during transfer */
    if(hi2c->ErrorCode != HAL_I2C_ERROR_NONE)
    {
      if(g_stop[dma_module_lookup(hdma)]) { 
        /* No need to Check TC flag, with AUTOEND mode the stop is automatically generated */
        /* Wait until STOPF flag is reset */ 
        if(i2c_WaitOnSTOPFlagUntilTimeout(hi2c, I2C_TIMEOUT_STOPF) != HAL_OK)
        {
          if(hi2c->ErrorCode == HAL_I2C_ERROR_AF)
          {
            hi2c->ErrorCode |= HAL_I2C_ERROR_AF;
          }
          else
          {
            hi2c->ErrorCode |= HAL_I2C_ERROR_TIMEOUT;
          }
        }
      
        /* Clear STOP Flag */
        __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_STOPF);
      }
      
      /* Clear Configuration Register 2 */
      I2C_RESET_CR2(hi2c);
    
      hi2c->XferCount = 0;
    
      hi2c->State = HAL_I2C_STATE_READY;
      HAL_I2C_ErrorCallback(hi2c);
    }
    else
    {
      hi2c->pBuffPtr += hi2c->XferSize;
      hi2c->XferCount -= hi2c->XferSize;
      if(hi2c->XferCount > 255)
      {
        hi2c->XferSize = 255;
      }
      else
      {
        hi2c->XferSize = hi2c->XferCount;
      }

      DevAddress = (hi2c->Instance->CR2 & I2C_CR2_SADD);

      /* Enable the DMA channel */
      HAL_DMA_Start_IT(hi2c->hdmarx, (uint32_t)&hi2c->Instance->RXDR, (uint32_t)hi2c->pBuffPtr, hi2c->XferSize);

      /* Send Slave Address */
      /* Set NBYTES to write and reload if size > 255 */
      if( (hi2c->XferSize == 255) && (hi2c->XferSize < hi2c->XferCount) )
      {
        i2c_TransferConfig(hi2c,DevAddress,hi2c->XferSize, I2C_RELOAD_MODE, I2C_NO_STARTSTOP);
      }
      else
      {
        if(g_stop[dma_module_lookup(hdma)]) i2c_TransferConfig(hi2c,DevAddress,hi2c->XferSize, I2C_AUTOEND_MODE, I2C_NO_STARTSTOP);
        else i2c_TransferConfig(hi2c,DevAddress,hi2c->XferSize, I2C_RELOAD_MODE, I2C_NO_STARTSTOP);
      }  

      /* Wait until RXNE flag is set */
      if(i2c_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_RXNE, RESET, I2C_TIMEOUT_RXNE) != HAL_OK)      
      {
        hi2c->ErrorCode |= HAL_I2C_ERROR_TIMEOUT;
      }

      /* Check if Errors has been detected during transfer */
      if(hi2c->ErrorCode != HAL_I2C_ERROR_NONE)
      {
        if(g_stop[dma_module_lookup(hdma)]) {
          /* No need to Check TC flag, with AUTOEND mode the stop is automatically generated */
          /* Wait until STOPF flag is reset */ 
          if(i2c_WaitOnSTOPFlagUntilTimeout(hi2c, I2C_TIMEOUT_STOPF) != HAL_OK)
          {
            if(hi2c->ErrorCode == HAL_I2C_ERROR_AF)
            {
              hi2c->ErrorCode |= HAL_I2C_ERROR_AF;
            }
            else
            {
              hi2c->ErrorCode |= HAL_I2C_ERROR_TIMEOUT;
            }
          }
        
          /* Clear STOP Flag */
          __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_STOPF);
        }
        
        /* Clear Configuration Register 2 */
        I2C_RESET_CR2(hi2c);
      
        hi2c->XferCount = 0;

        hi2c->State = HAL_I2C_STATE_READY;
        HAL_I2C_ErrorCallback(hi2c);
      }
      else
      {
        /* Enable DMA Request */
        hi2c->Instance->CR1 |= I2C_CR1_RXDMAEN;
      }
    }
  }
  else
  {
    if(g_stop[dma_module_lookup(hdma)]) {
      /* No need to Check TC flag, with AUTOEND mode the stop is automatically generated */
      /* Wait until STOPF flag is reset */ 
      if(i2c_WaitOnSTOPFlagUntilTimeout(hi2c, I2C_TIMEOUT_STOPF) != HAL_OK)
      {
        if(hi2c->ErrorCode == HAL_I2C_ERROR_AF)
        {
          hi2c->ErrorCode |= HAL_I2C_ERROR_AF;
        }
        else
        {
          hi2c->ErrorCode |= HAL_I2C_ERROR_TIMEOUT;
        }
      }
    
      /* Clear STOP Flag */
      __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_STOPF);
  	}
    
    /* Clear Configuration Register 2 */
    I2C_RESET_CR2(hi2c);
  
    /* Disable DMA Request */
    hi2c->Instance->CR1 &= ~I2C_CR1_RXDMAEN; 
  
    hi2c->XferCount = 0;
  
    hi2c->State = HAL_I2C_STATE_READY;

    /* Check if Errors has been detected during transfer */
    if(hi2c->ErrorCode != HAL_I2C_ERROR_NONE)
    {
      HAL_I2C_ErrorCallback(hi2c);
    }
    else
    {
      HAL_I2C_MemRxCpltCallback(hi2c);
    }
  }
}

static void i2c_DMAMasterTransmitCplt(DMA_HandleTypeDef *hdma) 
{
  uint16_t DevAddress;
  I2C_HandleTypeDef* hi2c = (I2C_HandleTypeDef*)((DMA_HandleTypeDef*)hdma)->Parent;
  
  /* Check if last DMA request was done with RELOAD */
  /* Set NBYTES to write and reload if size > 255 */
  if( (hi2c->XferSize == 255) && (hi2c->XferSize < hi2c->XferCount) )
  {
    /* Wait until TCR flag is set */
    if(i2c_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_TCR, RESET, I2C_TIMEOUT_TCR) != HAL_OK)      
    {
      hi2c->ErrorCode |= HAL_I2C_ERROR_TIMEOUT;
    }

    /* Disable DMA Request */
    hi2c->Instance->CR1 &= ~I2C_CR1_TXDMAEN; 

    /* Check if Errors has been detected during transfer */
    if(hi2c->ErrorCode != HAL_I2C_ERROR_NONE)
    {
      /* No need to Check TC flag, with AUTOEND mode the stop is automatically generated */
      /* Wait until STOPF flag is reset */ 
      if(i2c_WaitOnSTOPFlagUntilTimeout(hi2c, I2C_TIMEOUT_STOPF) != HAL_OK)
      {
        if(hi2c->ErrorCode == HAL_I2C_ERROR_AF)
        {
          hi2c->ErrorCode |= HAL_I2C_ERROR_AF;
        }
        else
        {
          hi2c->ErrorCode |= HAL_I2C_ERROR_TIMEOUT;
        }
      }
    
      /* Clear STOP Flag */
      __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_STOPF);
          
      /* Clear Configuration Register 2 */
      I2C_RESET_CR2(hi2c);

      hi2c->XferCount = 0;
    
      hi2c->State = HAL_I2C_STATE_READY;
      HAL_I2C_ErrorCallback(hi2c);
    }
    else
    {
      hi2c->pBuffPtr += hi2c->XferSize;
      hi2c->XferCount -= hi2c->XferSize;
      if(hi2c->XferCount > 255)
      {
        hi2c->XferSize = 255;
      }
      else
      {
        hi2c->XferSize = hi2c->XferCount;
      }

      DevAddress = (hi2c->Instance->CR2 & I2C_CR2_SADD);

      /* Enable the DMA channel */
      HAL_DMA_Start_IT(hi2c->hdmatx, (uint32_t)hi2c->pBuffPtr, (uint32_t)&hi2c->Instance->TXDR, hi2c->XferSize);

      /* Send Slave Address */
      /* Set NBYTES to write and reload if size > 255 */
      if( (hi2c->XferSize == 255) && (hi2c->XferSize < hi2c->XferCount) )
      {
        i2c_TransferConfig(hi2c,DevAddress,hi2c->XferSize, I2C_RELOAD_MODE, I2C_NO_STARTSTOP);
      }
      else
      {
        if(g_stop[dma_module_lookup(hdma)]) i2c_TransferConfig(hi2c,DevAddress,hi2c->XferSize, I2C_AUTOEND_MODE, I2C_NO_STARTSTOP);
        else i2c_TransferConfig(hi2c,DevAddress,hi2c->XferSize, I2C_RELOAD_MODE, I2C_NO_STARTSTOP);
      }  

      /* Wait until TXIS flag is set */
      if(i2c_WaitOnTXISFlagUntilTimeout(hi2c, I2C_TIMEOUT_TXIS) != HAL_OK)
      {
        if(g_stop[dma_module_lookup(hdma)]) {
          /* No need to Check TC flag, with AUTOEND mode the stop is automatically generated */
          /* Wait until STOPF flag is reset */ 
          if(i2c_WaitOnSTOPFlagUntilTimeout(hi2c, I2C_TIMEOUT_STOPF) != HAL_OK)
          {
            if(hi2c->ErrorCode == HAL_I2C_ERROR_AF)
            {
              hi2c->ErrorCode |= HAL_I2C_ERROR_AF;
            }
            else
            {
              hi2c->ErrorCode |= HAL_I2C_ERROR_TIMEOUT;
            }
          }

          /* Clear STOP Flag */
          __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_STOPF);
        }
        
        /* Clear Configuration Register 2 */
        I2C_RESET_CR2(hi2c);

        hi2c->XferCount = 0;

        hi2c->State = HAL_I2C_STATE_READY;
        HAL_I2C_ErrorCallback(hi2c);
      }
      else
      {
        /* Enable DMA Request */
        hi2c->Instance->CR1 |= I2C_CR1_TXDMAEN;
      }
    }
  }
  else
  {
    if(g_stop[dma_module_lookup(hdma)]) {
      /* No need to Check TC flag, with AUTOEND mode the stop is automatically generated */
      /* Wait until STOPF flag is reset */ 
      if(i2c_WaitOnSTOPFlagUntilTimeout(hi2c, I2C_TIMEOUT_STOPF) != HAL_OK)
      {
        if(hi2c->ErrorCode == HAL_I2C_ERROR_AF)
        {
          hi2c->ErrorCode |= HAL_I2C_ERROR_AF;
        }
        else
        {
          hi2c->ErrorCode |= HAL_I2C_ERROR_TIMEOUT;
        }
      }
    
      /* Clear STOP Flag */
      __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_STOPF);
  	}
    
    /* Clear Configuration Register 2 */
    I2C_RESET_CR2(hi2c);

    /* Disable DMA Request */
    hi2c->Instance->CR1 &= ~I2C_CR1_TXDMAEN; 
  
    hi2c->XferCount = 0;
  
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
}

void i2c_transfer_asynch(i2c_t *obj, const void *tx, size_t tx_length, void *rx, size_t rx_length, uint32_t address, uint32_t stop, uint32_t handler, uint32_t event, DMAUsage hint)
{
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
  
	  g_event[i2c_module_lookup(obj)] = event;  
    g_address[i2c_module_lookup(obj)] = address;
    g_stop_previous[i2c_module_lookup(obj)] = g_stop[i2c_module_lookup(obj)];
    g_stop[i2c_module_lookup(obj)] = stop;  
 
    // register the same thunking handler for both event and error interrupt!  
    IRQn_Type event_irq_n = I2cEventIRQs[i2c_module_lookup(obj)];  
    IRQn_Type error_irq_n = I2cErrorIRQs[i2c_module_lookup(obj)]; 
    NVIC_SetVector(error_irq_n, handler);
    NVIC_EnableIRQ(error_irq_n);

    /* NVIC configuration for DMA transfer complete interrupt (I2C_TX) */
    NVIC_SetVector(I2C_DMATx_IRQs[i2c_module_lookup(obj)], handler);
    HAL_NVIC_SetPriority(I2C_DMATx_IRQs[i2c_module_lookup(obj)], 0, 1);
    HAL_NVIC_EnableIRQ(I2C_DMATx_IRQs[i2c_module_lookup(obj)]);
      
    /* NVIC configuration for DMA transfer complete interrupt (I2C_RX) */
    NVIC_SetVector(I2C_DMARx_IRQs[i2c_module_lookup(obj)], handler);
    HAL_NVIC_SetPriority(I2C_DMARx_IRQs[i2c_module_lookup(obj)], 0, 0);   
    HAL_NVIC_EnableIRQ(I2C_DMARx_IRQs[i2c_module_lookup(obj)]);		
  
    I2C_HandleTypeDef *handle = &t_I2cHandle[i2c_module_lookup(obj)];
    t_I2cHandle[i2c_module_lookup(obj)].Instance = (I2C_TypeDef *)(obj->i2c.i2c);
		
    if (use_tx && use_rx) {  
        if (tx_length == 1) {  
            HAL_I2C_Mem_Read_DMA(handle, address, ((uint8_t*)tx)[0], I2C_MEMADD_SIZE_8BIT, rx, rx_length, stop, g_stop_previous[i2c_module_lookup(obj)]);  
        } else if (tx_length == 2) {  
            uint16_t memoryAddress = (((uint8_t*)tx)[0] << 8) | ((uint8_t*)tx)[1];  
            HAL_I2C_Mem_Read_DMA(handle, address, memoryAddress, I2C_MEMADD_SIZE_16BIT, rx, rx_length, stop, g_stop_previous[i2c_module_lookup(obj)]);  
        }  
    } else if (use_tx) {
        HAL_I2C_Master_Transmit_DMA(handle, address, (uint8_t*)tx, tx_length, stop, g_stop_previous[i2c_module_lookup(obj)]);  
    } else if (use_rx) {  
        HAL_I2C_Master_Receive_DMA(handle, address, (uint8_t*)rx, rx_length, stop, g_stop_previous[i2c_module_lookup(obj)]);  
    } else {
        NVIC_SetVector(event_irq_n, handler);      
        NVIC_EnableIRQ(event_irq_n);
        HAL_I2C_Master_Check_IT(handle, address, stop, g_stop_previous[i2c_module_lookup(obj)]);  
    }  
}  

uint32_t i2c_irq_handler_asynch(i2c_t *obj)  
{  
    I2C_HandleTypeDef *handle = &t_I2cHandle[i2c_module_lookup(obj)];
    t_I2cHandle[i2c_module_lookup(obj)].Instance = (I2C_TypeDef *)(obj->i2c.i2c);  
  
    /* So we map the event and error interrupt on the same thunking handle.  
     * In order to distinguish between both, we need to check the flags in  
     * the status register.  
     *  
     * The Event interrupt is generated when:  
     *  – SB = 1 (Master)  
     *  – ADDR = 1 (Master/Slave)  
     *  – ADD10 = 1 (Master)  
     *  – STOPF = 1 (Slave)  
     *  – BTF = 1 with no TxE or RxNE event  
     *  – TxE event to 1 if ITBUFEN = 1  
     *  – RxNE event to 1if ITBUFEN = 1  
     *  
     * The Error interrupt is generated when:  
     *  – BERR=1        Misplaced Start or Stop condition  
     *  – ARLO=1        Arbitration lost (only in multi-master mode)  
     *  – AF=1          Acknowledge Failure  
     *  – OVR=1         Overrun/Underrun  
     *  – PECERR = 1    PEC Error in reception (not applicable)  
     *  – TIMEOUT = 1   SCL remained LOW for 25 ms (Timeout) (only in SMBus mode)  
     *  – SMBALERT = 1  SMBus alert (not applicable, we don't use SMBus)  
     *  
     * All these flags are in the Status Register 1, and all event flags are  
     * in the lower 8 bits, while all error flags are in the upper 8 bits.  
     */  
    int status = handle->Instance->ISR;  
    int event = 0;  
    HAL_I2C_StateTypeDef previousState = HAL_I2C_GetState(handle);  
    HAL_I2C_StateTypeDef currentState;  
  
    // check error flags first, since they take presedence over events!  
    if (status & 0x7f00) {  
        // update buffer positions  
        size_t position = handle->XferSize - handle->XferCount;  
        if (previousState & 0x20) {  
            obj->rx_buff.pos = position;  
        } else {  
            obj->tx_buff.pos = position;  
        }  
  
        HAL_I2C_ER_IRQHandler(handle);  
        currentState = HAL_I2C_GetState(handle);  
  
        event = I2C_EVENT_ERROR;  
        if (HAL_I2C_GetError(handle) & HAL_I2C_ERROR_AF) {  
            event |= I2C_EVENT_TRANSFER_EARLY_NACK;  
        }  
  
        return (event & g_event[i2c_module_lookup(obj)]);  
    }  
  
    // check event flags next  
    if (status & 0x00ff) {  
				if ((status == 0x8081) || (status == 0x8041)) { 
					
				  handle->hdmarx->XferCpltCallback = i2c_DMAMasterReceiveCplt;
          HAL_DMA_IRQHandler(handle->hdmarx); 

				  // Disable ERR, TC, STOP, NACK, TXI interrupt
					__HAL_I2C_DISABLE_IT(handle,I2C_IT_ERRI | I2C_IT_TCI| I2C_IT_STOPI| I2C_IT_NACKI | I2C_IT_TXI );

					// Clear Configuration Register 2
					I2C_RESET_CR2(handle);

					handle->State = HAL_I2C_STATE_READY;
				
					event = I2C_EVENT_TRANSFER_COMPLETE;
				}
				else{
					HAL_I2C_EV_IRQHandler(handle, g_stop[i2c_module_lookup(obj)]);
					handle->hdmarx->XferCpltCallback = i2c_DMAMasterReceiveCplt;
          HAL_DMA_IRQHandler(handle->hdmarx);
					currentState = HAL_I2C_GetState(handle);  
				
					if (currentState == HAL_I2C_STATE_READY) {  
							// update buffer positions  
							size_t position = handle->XferSize - handle->XferCount;  
							if (previousState & 0x20) {  
									obj->rx_buff.pos = position;  
							} else {  
									obj->tx_buff.pos = position;  
							}  
							event = I2C_EVENT_TRANSFER_COMPLETE;
							
							if(__HAL_I2C_GET_FLAG(handle,I2C_FLAG_AF)) 
							{
                __HAL_I2C_CLEAR_FLAG(handle,I2C_FLAG_AF);
								event |= I2C_EVENT_TRANSFER_EARLY_NACK;
							}
					}  
			}
    }
    else if (status == 0x8000)  {
			    handle->hdmatx->XferCpltCallback = i2c_DMAMasterTransmitCplt;
          HAL_DMA_IRQHandler(handle->hdmatx); 
					currentState = HAL_I2C_GetState(handle);  
				
					if (currentState == HAL_I2C_STATE_READY) {  
							// update buffer positions  
							size_t position = handle->XferSize - handle->XferCount;  
							if (previousState & 0x20) {  
									obj->rx_buff.pos = position;  
							} else {  
									obj->tx_buff.pos = position;  
							}  
							event = I2C_EVENT_TRANSFER_COMPLETE;
					}  
    }
    return (event & g_event[i2c_module_lookup(obj)]);
}  
  
uint8_t i2c_active(i2c_t *obj)  
{  
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
    
void i2c_abort_asynch(i2c_t *obj)  
{  
    i2c_reset(obj);  
}

#if DEVICE_I2CSLAVE

#endif // DEVICE_I2CSLAVE

#endif //!DEVICE_I2C_ASYNCH_DMA

#endif // DEVICE_I2C_ASYNCH


#endif // DEVICE_I2C
