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
#include "platform/wait_api.h"

#if DEVICE_I2C

#include "cmsis.h"
#include "pinmap.h"
#include "PeripheralPins.h"
/*  F1 HAL not ready to move to I2C common code - this is ongoing */
#if !defined(__STM32F1xx_HAL_H)
#include "i2c_device.h" // family specific defines

#ifndef DEBUG_STDIO
#   define DEBUG_STDIO 0
#endif

#if DEBUG_STDIO
#   include <stdio.h>
#   define DEBUG_PRINTF(...) do { printf(__VA_ARGS__); } while(0)
#else
#   define DEBUG_PRINTF(...) {}
#endif

#if DEVICE_I2C_ASYNCH
    #define I2C_S(obj) (struct i2c_s *) (&((obj)->i2c))
#else
    #define I2C_S(obj) (struct i2c_s *) (obj)
#endif

/*  Family specific description for I2C */
#define I2C_NUM (5)
static I2C_HandleTypeDef* i2c_handles[I2C_NUM];

/* Timeout values are based on core clock and I2C clock.
   The BYTE_TIMEOUT is computed as twice the number of cycles it would
   take to send 10 bits over I2C. Most Flags should take less than that.
   This is for immediate FLAG or ACK check.
*/
#define BYTE_TIMEOUT ((SystemCoreClock / obj_s->hz) * 2 * 10)
/* Timeout values based on I2C clock.
   The BYTE_TIMEOUT_US is computed as 3x the time in us it would
   take to send 10 bits over I2C. Most Flags should take less than that.
   This is for complete transfers check.
*/
#define BYTE_TIMEOUT_US   ((SystemCoreClock / obj_s->hz) * 3 * 10)
/* Timeout values for flags and events waiting loops. These timeouts are
   not based on accurate values, they just guarantee that the application will
   not remain stuck if the I2C communication is corrupted. 
*/
#define FLAG_TIMEOUT ((int)0x1000)

/* GENERIC INIT and HELPERS FUNCTIONS */

#if defined(I2C1_BASE)
static void i2c1_irq(void)
{
    I2C_HandleTypeDef * handle = i2c_handles[0];
    HAL_I2C_EV_IRQHandler(handle);
    HAL_I2C_ER_IRQHandler(handle);
}
#endif
#if defined(I2C2_BASE)
static void i2c2_irq(void)
{
    I2C_HandleTypeDef * handle = i2c_handles[1];
    HAL_I2C_EV_IRQHandler(handle);
    HAL_I2C_ER_IRQHandler(handle);
}
#endif
#if defined(I2C3_BASE)
static void i2c3_irq(void)
{
    I2C_HandleTypeDef * handle = i2c_handles[2];
    HAL_I2C_EV_IRQHandler(handle);
    HAL_I2C_ER_IRQHandler(handle);
}
#endif
#if defined(I2C4_BASE)
static void i2c4_irq(void)
{
    I2C_HandleTypeDef * handle = i2c_handles[3];
    HAL_I2C_EV_IRQHandler(handle);
    HAL_I2C_ER_IRQHandler(handle);
}
#endif
#if defined(FMPI2C1_BASE)
static void i2c5_irq(void)
{
    I2C_HandleTypeDef * handle = i2c_handles[4];
    HAL_I2C_EV_IRQHandler(handle);
    HAL_I2C_ER_IRQHandler(handle);
}
#endif

void i2c_ev_err_enable(i2c_t *obj, uint32_t handler) {
    struct i2c_s *obj_s = I2C_S(obj);
    IRQn_Type irq_event_n = obj_s->event_i2cIRQ;
    IRQn_Type irq_error_n = obj_s->error_i2cIRQ;

    /* Set up event IT using IRQ and handler tables */
    NVIC_SetVector(irq_event_n, handler);
    HAL_NVIC_SetPriority(irq_event_n, 0, 0);
    HAL_NVIC_EnableIRQ(irq_event_n);
    /* Set up error IT using IRQ and handler tables */
    NVIC_SetVector(irq_error_n, handler);
    HAL_NVIC_SetPriority(irq_error_n, 0, 1);
    HAL_NVIC_EnableIRQ(irq_error_n);
}

void i2c_ev_err_disable(i2c_t *obj) {
    struct i2c_s *obj_s = I2C_S(obj);
    IRQn_Type irq_event_n = obj_s->event_i2cIRQ;
    IRQn_Type irq_error_n = obj_s->error_i2cIRQ;

    HAL_NVIC_DisableIRQ(irq_event_n);
    HAL_NVIC_DisableIRQ(irq_error_n);
}

uint32_t i2c_get_irq_handler(i2c_t *obj)
{
    struct i2c_s *obj_s = I2C_S(obj);
    I2C_HandleTypeDef *handle = &(obj_s->handle);
    uint32_t handler = 0;

    switch (obj_s->index) {
#if defined(I2C1_BASE)
        case 0:
            handler = (uint32_t)&i2c1_irq;
            break;
#endif
#if defined(I2C2_BASE)
        case 1:
            handler = (uint32_t)&i2c2_irq;
            break;
#endif
#if defined(I2C3_BASE)
        case 2:
            handler = (uint32_t)&i2c3_irq;
            break;
#endif
#if defined(I2C4_BASE)
        case 3:
            handler = (uint32_t)&i2c4_irq;
            break;
#endif
#if defined(FMPI2C1_BASE)
        case 4:
            handler = (uint32_t)&i2c5_irq;
            break;
#endif
    }

    i2c_handles[obj_s->index] = handle;
    return handler;
}

void i2c_init(i2c_t *obj, PinName sda, PinName scl) {

    struct i2c_s *obj_s = I2C_S(obj);

    // Determine the I2C to use
    I2CName i2c_sda = (I2CName)pinmap_peripheral(sda, PinMap_I2C_SDA);
    I2CName i2c_scl = (I2CName)pinmap_peripheral(scl, PinMap_I2C_SCL);
    obj_s->sda = sda;
    obj_s->scl = scl;

    obj_s->i2c = (I2CName)pinmap_merge(i2c_sda, i2c_scl);
    MBED_ASSERT(obj_s->i2c != (I2CName)NC);

#if defined I2C1_BASE
    // Enable I2C1 clock and pinout if not done
    if (obj_s->i2c == I2C_1) {
        obj_s->index = 0;
        // Configure I2C pins
        pinmap_pinout(sda, PinMap_I2C_SDA);
        pinmap_pinout(scl, PinMap_I2C_SCL);
        pin_mode(sda, PullUp);
        pin_mode(scl, PullUp);
        obj_s->event_i2cIRQ = I2C1_EV_IRQn;
        obj_s->error_i2cIRQ = I2C1_ER_IRQn;
        __HAL_RCC_I2C1_CLK_ENABLE();
    }
#endif
#if defined I2C2_BASE
    // Enable I2C2 clock and pinout if not done
    if (obj_s->i2c == I2C_2) {
        obj_s->index = 1;
        // Configure I2C pins
        pinmap_pinout(sda, PinMap_I2C_SDA);
        pinmap_pinout(scl, PinMap_I2C_SCL);
        pin_mode(sda, PullUp);
        pin_mode(scl, PullUp);
        obj_s->event_i2cIRQ = I2C2_EV_IRQn;
        obj_s->error_i2cIRQ = I2C2_ER_IRQn;
        __HAL_RCC_I2C2_CLK_ENABLE();
    }
#endif
#if defined I2C3_BASE
    // Enable I2C3 clock and pinout if not done
    if (obj_s->i2c == I2C_3) {
        obj_s->index = 2;
        // Configure I2C pins
        pinmap_pinout(sda, PinMap_I2C_SDA);
        pinmap_pinout(scl, PinMap_I2C_SCL);
        pin_mode(sda, PullUp);
        pin_mode(scl, PullUp);
        obj_s->event_i2cIRQ = I2C3_EV_IRQn;
        obj_s->error_i2cIRQ = I2C3_ER_IRQn;
        __HAL_RCC_I2C3_CLK_ENABLE();
    }
#endif
#if defined I2C4_BASE
    // Enable I2C3 clock and pinout if not done
    if (obj_s->i2c == I2C_4) {
        obj_s->index = 3;
        // Configure I2C pins
        pinmap_pinout(sda, PinMap_I2C_SDA);
        pinmap_pinout(scl, PinMap_I2C_SCL);
        pin_mode(sda, PullUp);
        pin_mode(scl, PullUp);
        obj_s->event_i2cIRQ = I2C4_EV_IRQn;
        obj_s->error_i2cIRQ = I2C4_ER_IRQn;
        __HAL_RCC_I2C4_CLK_ENABLE();
    }
#endif
#if defined FMPI2C1_BASE
    // Enable I2C3 clock and pinout if not done
    if (obj_s->i2c == FMPI2C_1) {
        obj_s->index = 4;
        // Configure I2C pins
        pinmap_pinout(sda, PinMap_I2C_SDA);
        pinmap_pinout(scl, PinMap_I2C_SCL);
        pin_mode(sda, PullUp);
        pin_mode(scl, PullUp);
        obj_s->event_i2cIRQ = FMPI2C1_EV_IRQn;
        obj_s->error_i2cIRQ = FMPI2C1_ER_IRQn;
        __HAL_RCC_FMPI2C1_CLK_ENABLE();
    }
#endif

    // Reset to clear pending flags if any
    i2c_reset(obj);

    // I2C configuration
    if(!obj_s->hz)
        obj_s->hz = 100000; // 100 kHz per default
    i2c_frequency(obj, obj_s->hz );

#if DEVICE_I2CSLAVE
    // I2C master by default
    obj_s->slave = 0;
    obj_s->pending_slave_tx_master_rx = 0;
    obj_s->pending_slave_rx_maxter_tx = 0;
#endif

    // I2C Xfer operation init
    obj_s->event = 0;
    obj_s->XferOperation = I2C_FIRST_AND_LAST_FRAME;
}

void i2c_frequency(i2c_t *obj, int hz)
{
    int timeout;
    struct i2c_s *obj_s = I2C_S(obj);
    I2C_HandleTypeDef *handle = &(obj_s->handle);

    // wait before init
    timeout = BYTE_TIMEOUT;
    while ((__HAL_I2C_GET_FLAG(handle, I2C_FLAG_BUSY)) && (--timeout != 0));

#ifdef I2C_IP_VERSION_V1
    handle->Init.ClockSpeed      = hz;
    handle->Init.DutyCycle       = I2C_DUTYCYCLE_2;
#endif
#ifdef I2C_IP_VERSION_V2
    /*  Only predefined timing for below frequencies are supported */
    MBED_ASSERT((hz == 100000) || (hz == 400000) || (hz == 1000000));
    handle->Init.Timing = get_i2c_timing(hz);

    // Enable the Fast Mode Plus capability
    if (hz == 1000000) {
#if defined(I2C1_BASE) && defined(__HAL_SYSCFG_FASTMODEPLUS_ENABLE) && defined (I2C_FASTMODEPLUS_I2C1)
        if (obj_s->i2c == I2C_1) {
            __HAL_SYSCFG_FASTMODEPLUS_ENABLE(I2C_FASTMODEPLUS_I2C1);
        }
#endif
#if defined(I2C2_BASE) && defined(__HAL_SYSCFG_FASTMODEPLUS_ENABLE) && defined (I2C_FASTMODEPLUS_I2C2)
        if (obj_s->i2c == I2C_2) {
            __HAL_SYSCFG_FASTMODEPLUS_ENABLE(I2C_FASTMODEPLUS_I2C2);
        }
#endif
#if defined(I2C3_BASE) && defined(__HAL_SYSCFG_FASTMODEPLUS_ENABLE) && defined (I2C_FASTMODEPLUS_I2C3)
        if (obj_s->i2c == I2C_3) {
            __HAL_SYSCFG_FASTMODEPLUS_ENABLE(I2C_FASTMODEPLUS_I2C3);
        }
#endif
#if defined(I2C4_BASE) && defined(__HAL_SYSCFG_FASTMODEPLUS_ENABLE) && defined (I2C_FASTMODEPLUS_I2C4)
        if (obj_s->i2c == I2C_4) {
            __HAL_SYSCFG_FASTMODEPLUS_ENABLE(I2C_FASTMODEPLUS_I2C4);
        }
#endif
    }
#endif //I2C_IP_VERSION_V2

    /*##-1- Configure the I2C clock source. The clock is derived from the SYSCLK #*/
#if defined(I2C1_BASE) && defined (__HAL_RCC_I2C1_CONFIG)
    if (obj_s->i2c == I2C_1) {
        __HAL_RCC_I2C1_CONFIG(I2CAPI_I2C1_CLKSRC);
    }
#endif
#if defined(I2C2_BASE) && defined(__HAL_RCC_I2C2_CONFIG)
    if (obj_s->i2c == I2C_2) {
        __HAL_RCC_I2C2_CONFIG(I2CAPI_I2C2_CLKSRC);
    }
#endif
#if defined(I2C3_BASE) && defined(__HAL_RCC_I2C3_CONFIG)
    if (obj_s->i2c == I2C_3) {
        __HAL_RCC_I2C3_CONFIG(I2CAPI_I2C3_CLKSRC);
    }
#endif
#if defined(I2C4_BASE) && defined(__HAL_RCC_I2C4_CONFIG)
    if (obj_s->i2c == I2C_4) {
        __HAL_RCC_I2C4_CONFIG(I2CAPI_I2C4_CLKSRC);
    }
#endif

#ifdef I2C_ANALOGFILTER_ENABLE
    /* Enable the Analog I2C Filter */
    HAL_I2CEx_AnalogFilter_Config(handle,I2C_ANALOGFILTER_ENABLE);
#endif

    // I2C configuration
    handle->Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
    handle->Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
    handle->Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
    handle->Init.NoStretchMode   = I2C_NOSTRETCH_DISABLED;
    handle->Init.OwnAddress1     = 0;
    handle->Init.OwnAddress2     = 0;
    HAL_I2C_Init(handle);

    /*  store frequency for timeout computation */
    obj_s->hz = hz;
}

i2c_t *get_i2c_obj(I2C_HandleTypeDef *hi2c){
    /* Aim of the function is to get i2c_s pointer using hi2c pointer */
    /* Highly inspired from magical linux kernel's "container_of" */
    /* (which was not directly used since not compatible with IAR toolchain) */
    struct i2c_s *obj_s;
    i2c_t *obj;

    obj_s = (struct i2c_s *)( (char *)hi2c - offsetof(struct i2c_s,handle));
    obj = (i2c_t *)( (char *)obj_s - offsetof(i2c_t,i2c));

    return (obj);
}

void i2c_reset(i2c_t *obj) {

    int timeout;
    struct i2c_s *obj_s = I2C_S(obj);
    I2C_HandleTypeDef *handle = &(obj_s->handle);

    handle->Instance = (I2C_TypeDef *)(obj_s->i2c);

    // wait before reset
    timeout = BYTE_TIMEOUT;
    while ((__HAL_I2C_GET_FLAG(handle, I2C_FLAG_BUSY)) && (--timeout != 0));
#if defined I2C1_BASE
    if (obj_s->i2c == I2C_1) {
        __HAL_RCC_I2C1_FORCE_RESET();
        __HAL_RCC_I2C1_RELEASE_RESET();
    }
#endif
#if defined I2C2_BASE
    if (obj_s->i2c == I2C_2) {
        __HAL_RCC_I2C2_FORCE_RESET();
        __HAL_RCC_I2C2_RELEASE_RESET();
    }
#endif
#if defined I2C3_BASE
    if (obj_s->i2c == I2C_3) {
        __HAL_RCC_I2C3_FORCE_RESET();
        __HAL_RCC_I2C3_RELEASE_RESET();
    }
#endif
#if defined I2C4_BASE
    if (obj_s->i2c == I2C_4) {
        __HAL_RCC_I2C4_FORCE_RESET();
        __HAL_RCC_I2C4_RELEASE_RESET();
    }
#endif
#if defined FMPI2C1_BASE
    if (obj_s->i2c == FMPI2C_1) {
        __HAL_RCC_FMPI2C1_FORCE_RESET();
        __HAL_RCC_FMPI2C1_RELEASE_RESET();
    }
#endif
}

/* SYNCHRONOUS API FUNCTIONS */

int i2c_read(i2c_t *obj, int address, char *data, int length, int stop) {
    struct i2c_s *obj_s = I2C_S(obj);
    I2C_HandleTypeDef *handle = &(obj_s->handle);
    int count = 0, ret = 0;
    uint32_t timeout = 0;

    if ((obj_s->XferOperation == I2C_FIRST_AND_LAST_FRAME) ||
        (obj_s->XferOperation == I2C_LAST_FRAME)) {
        if (stop)
            obj_s->XferOperation = I2C_FIRST_AND_LAST_FRAME;
        else
            obj_s->XferOperation = I2C_FIRST_FRAME;
    } else if ((obj_s->XferOperation == I2C_FIRST_FRAME) ||
        (obj_s->XferOperation == I2C_NEXT_FRAME)) {
        if (stop)
            obj_s->XferOperation = I2C_LAST_FRAME;
        else
            obj_s->XferOperation = I2C_NEXT_FRAME;
    }

    obj_s->event = 0;

     /* Activate default IRQ handlers for sync mode
     * which would be overwritten in async mode
     */
    i2c_ev_err_enable(obj, i2c_get_irq_handler(obj));

    ret = HAL_I2C_Master_Sequential_Receive_IT(handle, address, (uint8_t *) data, length, obj_s->XferOperation);

    if(ret == HAL_OK) {
        timeout = BYTE_TIMEOUT_US * length;
        /*  transfer started : wait completion or timeout */
        while(!(obj_s->event & I2C_EVENT_ALL) && (--timeout != 0)) {
            wait_us(1);
        }

        i2c_ev_err_disable(obj);

        if((timeout == 0) || (obj_s->event != I2C_EVENT_TRANSFER_COMPLETE)) {
            DEBUG_PRINTF(" TIMEOUT or error in i2c_read\r\n");
            /* re-init IP to try and get back in a working state */
            i2c_init(obj, obj_s->sda, obj_s->scl);
        } else {
            count = length;
        }
    } else {
        DEBUG_PRINTF("ERROR in i2c_read\r\n");
    }

    return count;
}

/*
 *  UNITARY APIS.
 *  For very basic operations, direct registers access is needed
 *  There are 2 different IPs version that need to be supported
 */
#ifdef I2C_IP_VERSION_V1
int i2c_start(i2c_t *obj) {

    int timeout;
    struct i2c_s *obj_s = I2C_S(obj);
    I2C_HandleTypeDef *handle = &(obj_s->handle);

    // Clear Acknowledge failure flag
    __HAL_I2C_CLEAR_FLAG(handle, I2C_FLAG_AF);

    // Wait the STOP condition has been previously correctly sent
    // This timeout can be avoid in some specific cases by simply clearing the STOP bit
    timeout = FLAG_TIMEOUT;
    while ((handle->Instance->CR1 & I2C_CR1_STOP) == I2C_CR1_STOP) {
        if ((timeout--) == 0) {
            return 1;
        }
    }

    // Generate the START condition
    handle->Instance->CR1 |= I2C_CR1_START;

    // Wait the START condition has been correctly sent
    timeout = FLAG_TIMEOUT;
    while (__HAL_I2C_GET_FLAG(handle, I2C_FLAG_SB) == RESET) {
        if ((timeout--) == 0) {
            return 1;
        }
    }

    return 0;
}

int i2c_stop(i2c_t *obj) {
    struct i2c_s *obj_s = I2C_S(obj);
    I2C_TypeDef *i2c = (I2C_TypeDef *)obj_s->i2c;

    // Generate the STOP condition
    i2c->CR1 |= I2C_CR1_STOP;

    return 0;
}

int i2c_byte_read(i2c_t *obj, int last) {

    int timeout;
    struct i2c_s *obj_s = I2C_S(obj);
    I2C_HandleTypeDef *handle = &(obj_s->handle);

    if (last) {
        // Don't acknowledge the last byte
        handle->Instance->CR1 &= ~I2C_CR1_ACK;
    } else {
        // Acknowledge the byte
        handle->Instance->CR1 |= I2C_CR1_ACK;
    }

    // Wait until the byte is received
    timeout = FLAG_TIMEOUT;
    while (__HAL_I2C_GET_FLAG(handle, I2C_FLAG_RXNE) == RESET) {
        if ((timeout--) == 0) {
            return -1;
        }
    }

    return (int)handle->Instance->DR;
}

int i2c_byte_write(i2c_t *obj, int data) {

    int timeout;
    struct i2c_s *obj_s = I2C_S(obj);
    I2C_HandleTypeDef *handle = &(obj_s->handle);

    handle->Instance->DR = (uint8_t)data;

    // Wait until the byte (might be the address) is transmitted
    timeout = FLAG_TIMEOUT;
    while ((__HAL_I2C_GET_FLAG(handle, I2C_FLAG_TXE) == RESET) &&
            (__HAL_I2C_GET_FLAG(handle, I2C_FLAG_BTF) == RESET) &&
             (__HAL_I2C_GET_FLAG(handle, I2C_FLAG_ADDR) == RESET)) {
        if ((timeout--) == 0) {
            return 0;
        }
    }

     if (__HAL_I2C_GET_FLAG(handle, I2C_FLAG_ADDR) != RESET)
     {
         __HAL_I2C_CLEAR_ADDRFLAG(handle);
     }

    return 1;
}
#endif //I2C_IP_VERSION_V1
#ifdef I2C_IP_VERSION_V2
int i2c_start(i2c_t *obj) {
    struct i2c_s *obj_s = I2C_S(obj);
    I2C_HandleTypeDef *handle = &(obj_s->handle);
    I2C_TypeDef *i2c = (I2C_TypeDef *)obj_s->i2c;
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

int i2c_stop(i2c_t *obj) {
    struct i2c_s *obj_s = I2C_S(obj);
    I2C_TypeDef *i2c = (I2C_TypeDef *)obj_s->i2c;

    // Generate the STOP condition
    i2c->CR2 |= I2C_CR2_STOP;

    return 0;
}

int i2c_byte_read(i2c_t *obj, int last) {
    struct i2c_s *obj_s = I2C_S(obj);
    I2C_TypeDef *i2c = (I2C_TypeDef *)obj_s->i2c;
    I2C_HandleTypeDef *handle = &(obj_s->handle);
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
    struct i2c_s *obj_s = I2C_S(obj);
    I2C_TypeDef *i2c = (I2C_TypeDef *)obj_s->i2c;
    I2C_HandleTypeDef *handle = &(obj_s->handle);
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
#endif //I2C_IP_VERSION_V2

/*
 *  SYNC APIS
 */
int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop) {
    struct i2c_s *obj_s = I2C_S(obj);
    I2C_HandleTypeDef *handle = &(obj_s->handle);
    int count = 0, ret = 0;
    uint32_t timeout = 0;

    if ((obj_s->XferOperation == I2C_FIRST_AND_LAST_FRAME) ||
        (obj_s->XferOperation == I2C_LAST_FRAME)) {
        if (stop)
            obj_s->XferOperation = I2C_FIRST_AND_LAST_FRAME;
        else
            obj_s->XferOperation = I2C_FIRST_FRAME;
    } else if ((obj_s->XferOperation == I2C_FIRST_FRAME) ||
        (obj_s->XferOperation == I2C_NEXT_FRAME)) {
        if (stop)
            obj_s->XferOperation = I2C_LAST_FRAME;
        else
            obj_s->XferOperation = I2C_NEXT_FRAME;
    }

    obj_s->event = 0;

    i2c_ev_err_enable(obj, i2c_get_irq_handler(obj));

    ret = HAL_I2C_Master_Sequential_Transmit_IT(handle, address, (uint8_t *) data, length, obj_s->XferOperation); 

    if(ret == HAL_OK) {
        timeout = BYTE_TIMEOUT_US * length;
        /*  transfer started : wait completion or timeout */
        while(!(obj_s->event & I2C_EVENT_ALL) && (--timeout != 0)) {
            wait_us(1);
        }

        i2c_ev_err_disable(obj);

        if((timeout == 0) || (obj_s->event != I2C_EVENT_TRANSFER_COMPLETE)) {
            DEBUG_PRINTF(" TIMEOUT or error in i2c_write\r\n");
            /* re-init IP to try and get back in a working state */
            i2c_init(obj, obj_s->sda, obj_s->scl);
         } else {
            count = length;
       }
    } else {
        DEBUG_PRINTF("ERROR in i2c_read\r\n");
    }

    return count;
}

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c){
    /* Get object ptr based on handler ptr */
    i2c_t *obj = get_i2c_obj(hi2c);
    struct i2c_s *obj_s = I2C_S(obj);

#if DEVICE_I2C_ASYNCH
    /* Handle potential Tx/Rx use case */
    if ((obj->tx_buff.length) && (obj->rx_buff.length)) {
        if (obj_s->stop) {
            obj_s->XferOperation = I2C_LAST_FRAME;
        } else {
            obj_s->XferOperation = I2C_NEXT_FRAME;
        }

        HAL_I2C_Master_Sequential_Receive_IT(hi2c, obj_s->address, (uint8_t*)obj->rx_buff.buffer , obj->rx_buff.length, obj_s->XferOperation); 
    }
    else
#endif
    {
        /* Set event flag */
        obj_s->event = I2C_EVENT_TRANSFER_COMPLETE;
    }
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c){
    /* Get object ptr based on handler ptr */
    i2c_t *obj = get_i2c_obj(hi2c);
    struct i2c_s *obj_s = I2C_S(obj);

    /* Set event flag */
    obj_s->event = I2C_EVENT_TRANSFER_COMPLETE;
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c){
    /* Get object ptr based on handler ptr */
    i2c_t *obj = get_i2c_obj(hi2c);
    struct i2c_s *obj_s = I2C_S(obj);

    DEBUG_PRINTF("HAL_I2C_ErrorCallback:%d, index=%d\r\n", (int) hi2c->ErrorCode, obj_s->index);

    /* re-init IP to try and get back in a working state */
    i2c_init(obj, obj_s->sda, obj_s->scl);

    /* Keep Set event flag */
    obj_s->event = I2C_EVENT_ERROR;
}

#if DEVICE_I2CSLAVE
/* SLAVE API FUNCTIONS */
void i2c_slave_address(i2c_t *obj, int idx, uint32_t address, uint32_t mask) {
    struct i2c_s *obj_s = I2C_S(obj);
    I2C_HandleTypeDef *handle = &(obj_s->handle);

    // I2C configuration
    handle->Init.OwnAddress1     = address;
    HAL_I2C_Init(handle);

    i2c_ev_err_enable(obj, i2c_get_irq_handler(obj));

    HAL_I2C_EnableListen_IT(handle);
}

void i2c_slave_mode(i2c_t *obj, int enable_slave) {

    struct i2c_s *obj_s = I2C_S(obj);
    I2C_HandleTypeDef *handle = &(obj_s->handle);

    if (enable_slave) {
        obj_s->slave = 1;
        HAL_I2C_EnableListen_IT(handle);
    } else {
        obj_s->slave = 0;
        HAL_I2C_DisableListen_IT(handle);
    }
}

// See I2CSlave.h
#define NoData         0 // the slave has not been addressed
#define ReadAddressed  1 // the master has requested a read from this slave (slave = transmitter)
#define WriteGeneral   2 // the master is writing to all slave
#define WriteAddressed 3 // the master is writing to this slave (slave = receiver)


void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode) {
    /* Get object ptr based on handler ptr */
    i2c_t *obj = get_i2c_obj(hi2c);
    struct i2c_s *obj_s = I2C_S(obj);

    /*  Transfer direction in HAL is from Master point of view */
    if(TransferDirection == I2C_DIRECTION_RECEIVE) {
        obj_s->pending_slave_tx_master_rx = 1;
    }

    if(TransferDirection == I2C_DIRECTION_TRANSMIT) {
        obj_s->pending_slave_rx_maxter_tx = 1;
    }
}

void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *I2cHandle){
    /* Get object ptr based on handler ptr */
    i2c_t *obj = get_i2c_obj(I2cHandle);
    struct i2c_s *obj_s = I2C_S(obj);
    obj_s->pending_slave_tx_master_rx = 0;
}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *I2cHandle){
    /* Get object ptr based on handler ptr */
    i2c_t *obj = get_i2c_obj(I2cHandle);
    struct i2c_s *obj_s = I2C_S(obj);
    obj_s->pending_slave_rx_maxter_tx = 0;
}

void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c)
{
    /* restart listening for master requests */
    HAL_I2C_EnableListen_IT(hi2c);
}

int i2c_slave_receive(i2c_t *obj) {

    struct i2c_s *obj_s = I2C_S(obj);
    int retValue = NoData;

     if(obj_s->pending_slave_rx_maxter_tx) {
         retValue = WriteAddressed;
     }

     if(obj_s->pending_slave_tx_master_rx) {
            retValue = ReadAddressed;
     }

    return (retValue);
}

int i2c_slave_read(i2c_t *obj, char *data, int length) {
    struct i2c_s *obj_s = I2C_S(obj);
    I2C_HandleTypeDef *handle = &(obj_s->handle);
    int count = 0;
    int ret = 0;
    uint32_t timeout = 0;

    /*  Always use I2C_NEXT_FRAME as slave will just adapt to master requests */
    ret = HAL_I2C_Slave_Sequential_Receive_IT(handle, (uint8_t *) data, length, I2C_NEXT_FRAME);

    if(ret == HAL_OK) {
        timeout = BYTE_TIMEOUT_US * length;
        while(obj_s->pending_slave_rx_maxter_tx && (--timeout != 0)) {
            wait_us(1);
        }

         if(timeout != 0) {
             count = length;
         } else {
             DEBUG_PRINTF("TIMEOUT or error in i2c_slave_read\r\n");
         }
    }
    return count;
}

int i2c_slave_write(i2c_t *obj, const char *data, int length) {
    struct i2c_s *obj_s = I2C_S(obj);
    I2C_HandleTypeDef *handle = &(obj_s->handle);
    int count = 0;
    int ret = 0;
    uint32_t timeout = 0;

    /*  Always use I2C_NEXT_FRAME as slave will just adapt to master requests */
    ret = HAL_I2C_Slave_Sequential_Transmit_IT(handle, (uint8_t *) data, length, I2C_NEXT_FRAME);

    if(ret == HAL_OK) {
        timeout = BYTE_TIMEOUT_US * length;
        while(obj_s->pending_slave_tx_master_rx && (--timeout != 0)) {
            wait_us(1);
        }

         if(timeout != 0) {
             count = length;
         } else {
             DEBUG_PRINTF("TIMEOUT or error in i2c_slave_write\r\n");
         }
    }

    return count;
}
#endif // DEVICE_I2CSLAVE

#if DEVICE_I2C_ASYNCH
/* ASYNCH MASTER API FUNCTIONS */
void HAL_I2C_AbortCpltCallback(I2C_HandleTypeDef *hi2c){
    /* Get object ptr based on handler ptr */
    i2c_t *obj = get_i2c_obj(hi2c);
    struct i2c_s *obj_s = I2C_S(obj);
    I2C_HandleTypeDef *handle = &(obj_s->handle);

    /* Disable IT. Not always done before calling macro */
    __HAL_I2C_DISABLE_IT(handle, I2C_IT_ALL);
    i2c_ev_err_disable(obj);

    /* Set event flag */
    obj_s->event = I2C_EVENT_ERROR;
}

void i2c_transfer_asynch(i2c_t *obj, const void *tx, size_t tx_length, void *rx, size_t rx_length, uint32_t address, uint32_t stop, uint32_t handler, uint32_t event, DMAUsage hint) {

    // TODO: DMA usage is currently ignored by this way
    (void) hint;

    struct i2c_s *obj_s = I2C_S(obj);
    I2C_HandleTypeDef *handle = &(obj_s->handle);

    /* Update object */
    obj->tx_buff.buffer = (void *)tx;
    obj->tx_buff.length = tx_length;
    obj->tx_buff.pos = 0;
    obj->tx_buff.width = 8;

    obj->rx_buff.buffer = (void *)rx;
    obj->rx_buff.length = rx_length;
    obj->rx_buff.pos = SIZE_MAX;
    obj->rx_buff.width = 8;

    obj_s->available_events = event;
    obj_s->event = 0;
    obj_s->address = address;
    obj_s->stop = stop;

    i2c_ev_err_enable(obj, handler);

    /* Set operation step depending if stop sending required or not */
    if ((tx_length && !rx_length) || (!tx_length && rx_length)) {
        if ((obj_s->XferOperation == I2C_FIRST_AND_LAST_FRAME) ||
            (obj_s->XferOperation == I2C_LAST_FRAME)) {
            if (stop)
                obj_s->XferOperation = I2C_FIRST_AND_LAST_FRAME;
            else
                obj_s->XferOperation = I2C_FIRST_FRAME;
        } else if ((obj_s->XferOperation == I2C_FIRST_FRAME) ||
            (obj_s->XferOperation == I2C_NEXT_FRAME)) {
            if (stop)
                obj_s->XferOperation = I2C_LAST_FRAME;
            else
                obj_s->XferOperation = I2C_NEXT_FRAME;
        }

        if (tx_length > 0) {
            HAL_I2C_Master_Sequential_Transmit_IT(handle, address, (uint8_t*)tx, tx_length, obj_s->XferOperation);
        }
        if (rx_length > 0) {
            HAL_I2C_Master_Sequential_Receive_IT(handle, address, (uint8_t*)rx, rx_length, obj_s->XferOperation);
        }
    }
    else if (tx_length && rx_length) {
        /* Two steps operation, don't modify XferOperation, keep it for next step */
        if ((obj_s->XferOperation == I2C_FIRST_AND_LAST_FRAME) ||
            (obj_s->XferOperation == I2C_LAST_FRAME)) {
                HAL_I2C_Master_Sequential_Transmit_IT(handle, address, (uint8_t*)tx, tx_length, I2C_FIRST_FRAME);
        } else if ((obj_s->XferOperation == I2C_FIRST_FRAME) ||
            (obj_s->XferOperation == I2C_NEXT_FRAME)) {
                HAL_I2C_Master_Sequential_Transmit_IT(handle, address, (uint8_t*)tx, tx_length, I2C_NEXT_FRAME);
        }
    }
}


uint32_t i2c_irq_handler_asynch(i2c_t *obj) {

    struct i2c_s *obj_s = I2C_S(obj);
    I2C_HandleTypeDef *handle = &(obj_s->handle);

    HAL_I2C_EV_IRQHandler(handle);
    HAL_I2C_ER_IRQHandler(handle);

     /*  Return I2C event status */
    return (obj_s->event & obj_s->available_events);
}

uint8_t i2c_active(i2c_t *obj) {

    struct i2c_s *obj_s = I2C_S(obj);
    I2C_HandleTypeDef *handle = &(obj_s->handle);

    if (handle->State == HAL_I2C_STATE_READY) {
        return 0;
    }
    else {
        return 1;
    }
}

void i2c_abort_asynch(i2c_t *obj) {

    struct i2c_s *obj_s = I2C_S(obj);
    I2C_HandleTypeDef *handle = &(obj_s->handle);

    /* Abort HAL requires DevAddress, but is not used. Use Dummy */
    uint16_t Dummy_DevAddress = 0x00;

    HAL_I2C_Master_Abort_IT(handle, Dummy_DevAddress);
}

#endif // DEVICE_I2C_ASYNCH

#endif // STM32F1

#endif // DEVICE_I2C
