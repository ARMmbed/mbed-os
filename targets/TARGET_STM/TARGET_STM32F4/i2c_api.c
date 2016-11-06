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

#if DEVICE_I2C

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

#if DEVICE_I2C_ASYNCH
    #define I2C_S(obj) (struct i2c_s *) (&((obj)->i2c))
#else
    #define I2C_S(obj) (struct i2c_s *) (obj)
#endif


void i2c_init(i2c_t *obj, PinName sda, PinName scl) {

    struct i2c_s *obj_s = I2C_S(obj);

    // Determine the I2C to use
    I2CName i2c_sda = (I2CName)pinmap_peripheral(sda, PinMap_I2C_SDA);
    I2CName i2c_scl = (I2CName)pinmap_peripheral(scl, PinMap_I2C_SCL);

    obj_s->i2c = (I2CName)pinmap_merge(i2c_sda, i2c_scl);
    MBED_ASSERT(obj_s->i2c != (I2CName)NC);

    // Enable I2C1 clock and pinout if not done
    if ((obj_s->i2c == I2C_1) && !i2c1_inited) {
        i2c1_inited = 1;
        // Configure I2C pins
        pinmap_pinout(sda, PinMap_I2C_SDA);
        pinmap_pinout(scl, PinMap_I2C_SCL);
        pin_mode(sda, PullUp);
        pin_mode(scl, PullUp);
#if DEVICE_I2C_ASYNCH
        obj_s->event_i2cIRQ = I2C1_EV_IRQn;
        obj_s->error_i2cIRQ = I2C1_ER_IRQn;
#endif
        __I2C1_CLK_ENABLE();
    }
    // Enable I2C2 clock and pinout if not done
    if ((obj_s->i2c == I2C_2) && !i2c2_inited) {
        i2c2_inited = 1;
        // Configure I2C pins
        pinmap_pinout(sda, PinMap_I2C_SDA);
        pinmap_pinout(scl, PinMap_I2C_SCL);
        pin_mode(sda, PullUp);
        pin_mode(scl, PullUp);
#if DEVICE_I2C_ASYNCH
        obj_s->event_i2cIRQ = I2C2_EV_IRQn;
        obj_s->error_i2cIRQ = I2C2_ER_IRQn;
#endif
        __I2C2_CLK_ENABLE();
    }
#if defined I2C3_BASE
    // Enable I2C3 clock and pinout if not done
    if ((obj_s->i2c == I2C_3) && !i2c3_inited) {
        i2c3_inited = 1;
        // Configure I2C pins
        pinmap_pinout(sda, PinMap_I2C_SDA);
        pinmap_pinout(scl, PinMap_I2C_SCL);
        pin_mode(sda, PullUp);
        pin_mode(scl, PullUp);
#if DEVICE_I2C_ASYNCH
        obj_s->event_i2cIRQ = I2C3_EV_IRQn;
        obj_s->error_i2cIRQ = I2C3_ER_IRQn;
#endif
        __I2C3_CLK_ENABLE();
    }
#endif

#if defined FMPI2C1_BASE
    // Enable I2C3 clock and pinout if not done
    if ((obj_s->i2c == FMPI2C_1) && !fmpi2c1_inited) {
        fmpi2c1_inited = 1;
        // Configure I2C pins
        pinmap_pinout(sda, PinMap_I2C_SDA);
        pinmap_pinout(scl, PinMap_I2C_SCL);
        pin_mode(sda, PullUp);
        pin_mode(scl, PullUp);
#if DEVICE_I2C_ASYNCH
        obj_s->event_i2cIRQ = FMPI2C1_EV_IRQn;
        obj_s->error_i2cIRQ = FMPI2C1_ER_IRQn;
#endif
        __HAL_RCC_FMPI2C1_CLK_ENABLE();
    }
#endif

    // Reset to clear pending flags if any
    i2c_reset(obj);

    // I2C configuration
    i2c_frequency(obj, 100000); // 100 kHz per default

#if DEVICE_I2CSLAVE
    // I2C master by default
    obj_s->slave = 0;
#endif

#if DEVICE_I2C_ASYNCH
    // I2C Xfer operation init
    obj_s->XferOperation = I2C_FIRST_AND_LAST_FRAME;
#endif
}

void i2c_frequency(i2c_t *obj, int hz)
{

    int timeout;
    struct i2c_s *obj_s = I2C_S(obj);
    I2C_HandleTypeDef *handle = &(obj_s->handle);

    MBED_ASSERT((hz > 0) && (hz <= 400000));

    // wait before init
    timeout = LONG_TIMEOUT;
    while ((__HAL_I2C_GET_FLAG(handle, I2C_FLAG_BUSY)) && (timeout-- != 0));

    // I2C configuration
    handle->Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
    handle->Init.ClockSpeed      = hz;
    handle->Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
    handle->Init.DutyCycle       = I2C_DUTYCYCLE_2;
    handle->Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
    handle->Init.NoStretchMode   = I2C_NOSTRETCH_DISABLED;
    handle->Init.OwnAddress1     = 0;
    handle->Init.OwnAddress2     = 0;
    HAL_I2C_Init(handle);

#if DEVICE_I2CSLAVE
    if (obj_s->slave) {
        /* Enable Address Acknowledge */
        handle->Instance->CR1 |= I2C_CR1_ACK;
    }
#endif

}

inline int i2c_start(i2c_t *obj) {

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

inline int i2c_stop(i2c_t *obj) {
    struct i2c_s *obj_s = I2C_S(obj);
    I2C_TypeDef *i2c = (I2C_TypeDef *)obj_s->i2c;

    // Generate the STOP condition
    i2c->CR1 |= I2C_CR1_STOP;

    return 0;
}

int i2c_read(i2c_t *obj, int address, char *data, int length, int stop) {

    int timeout;
    int count;
    int value;
    struct i2c_s *obj_s = I2C_S(obj);
    I2C_HandleTypeDef *handle = &(obj_s->handle);

    i2c_start(obj);

    // Wait until SB flag is set
    timeout = FLAG_TIMEOUT;
    while (__HAL_I2C_GET_FLAG(handle, I2C_FLAG_SB) == RESET) {
        timeout--;
        if (timeout == 0) {
            return -1;
        }
    }

    handle->Instance->DR = __HAL_I2C_7BIT_ADD_READ(address);

    // Wait address is acknowledged
    timeout = FLAG_TIMEOUT;
    while (__HAL_I2C_GET_FLAG(handle, I2C_FLAG_ADDR) == RESET) {
        timeout--;
        if (timeout == 0) {
            return -1;
        }
    }
    __HAL_I2C_CLEAR_ADDRFLAG(handle);

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

int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop) {

    int timeout;
    int count;
    struct i2c_s *obj_s = I2C_S(obj);
    I2C_HandleTypeDef *handle = &(obj_s->handle);

    i2c_start(obj);

    // Wait until SB flag is set
    timeout = FLAG_TIMEOUT;
    while (__HAL_I2C_GET_FLAG(handle, I2C_FLAG_SB) == RESET) {
        timeout--;
        if (timeout == 0) {
            return -1;
        }
    }

    handle->Instance->DR = __HAL_I2C_7BIT_ADD_WRITE(address);

    // Wait address is acknowledged
    timeout = FLAG_TIMEOUT;
    while (__HAL_I2C_GET_FLAG(handle, I2C_FLAG_ADDR) == RESET) {
        timeout--;
        if (timeout == 0) {
            return -1;
        }
    }
    __HAL_I2C_CLEAR_ADDRFLAG(handle);

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

void i2c_reset(i2c_t *obj) {

    int timeout;
    struct i2c_s *obj_s = I2C_S(obj);
    I2C_HandleTypeDef *handle = &(obj_s->handle);

    handle->Instance = (I2C_TypeDef *)(obj_s->i2c);

    // wait before reset
    timeout = LONG_TIMEOUT;
    while ((__HAL_I2C_GET_FLAG(handle, I2C_FLAG_BUSY)) && (timeout-- != 0));

    if (obj_s->i2c == I2C_1) {
        __I2C1_FORCE_RESET();
        __I2C1_RELEASE_RESET();
    }

    if (obj_s->i2c == I2C_2) {
        __I2C2_FORCE_RESET();
        __I2C2_RELEASE_RESET();
    }
#if defined I2C3_BASE
    if (obj_s->i2c == I2C_3) {
        __I2C3_FORCE_RESET();
        __I2C3_RELEASE_RESET();
    }
#endif

#if defined FMPI2C1_BASE
    if (obj_s->i2c == FMPI2C_1) {
        __HAL_RCC_FMPI2C1_FORCE_RESET();
        __HAL_RCC_FMPI2C1_RELEASE_RESET();
    }
#endif
}

#if DEVICE_I2CSLAVE

void i2c_slave_address(i2c_t *obj, int idx, uint32_t address, uint32_t mask) {

    uint16_t tmpreg = 0;
    struct i2c_s *obj_s = I2C_S(obj);
    I2C_TypeDef *i2c = (I2C_TypeDef *)obj_s->i2c;

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

    struct i2c_s *obj_s = I2C_S(obj);
    I2C_TypeDef *i2c = (I2C_TypeDef *)obj_s->i2c;

    if (enable_slave) {
        obj_s->slave = 1;

        /* Enable Address Acknowledge */
        i2c->CR1 |= I2C_CR1_ACK;
    }
}

// See I2CSlave.h
#define NoData         0 // the slave has not been addressed
#define ReadAddressed  1 // the master has requested a read from this slave (slave = transmitter)
#define WriteGeneral   2 // the master is writing to all slave
#define WriteAddressed 3 // the master is writing to this slave (slave = receiver)

int i2c_slave_receive(i2c_t *obj) {

    struct i2c_s *obj_s = I2C_S(obj);
    I2C_HandleTypeDef *handle = &(obj_s->handle);

    int retValue = NoData;

    /* Reading BUSY flag before ADDR flag could clear ADDR */
    int addr = __HAL_I2C_GET_FLAG(handle, I2C_FLAG_ADDR);

    if (__HAL_I2C_GET_FLAG(handle, I2C_FLAG_BUSY) == 1) {
        if (addr == 1) {
            if (__HAL_I2C_GET_FLAG(handle, I2C_FLAG_TRA) == 1) {
                retValue = ReadAddressed;
            } else {
                retValue = WriteAddressed;
            }
            __HAL_I2C_CLEAR_ADDRFLAG(handle);
        }
    }

    return (retValue);
}

int i2c_slave_read(i2c_t *obj, char *data, int length) {

    struct i2c_s *obj_s = I2C_S(obj);
    I2C_HandleTypeDef *handle = &(obj_s->handle);

    uint32_t Timeout;
    int size = 0;

    while (length > 0) {

        /* Wait until RXNE flag is set */
        // Wait until the byte is received
        Timeout = FLAG_TIMEOUT;
        while (__HAL_I2C_GET_FLAG(handle, I2C_FLAG_RXNE) == RESET) {
            Timeout--;
            if (Timeout == 0) {
                return -1;
            }
        }

        /* Read data from DR */
        (*data++) = handle->Instance->DR;
        length--;
        size++;

        if ((__HAL_I2C_GET_FLAG(handle, I2C_FLAG_BTF) == SET) && (length != 0)) {
            /* Read data from DR */
            (*data++) = handle->Instance->DR;
            length--;
            size++;
        }
    }

    /* Wait until STOP flag is set */
    Timeout = FLAG_TIMEOUT;
    while (__HAL_I2C_GET_FLAG(handle, I2C_FLAG_STOPF) == RESET) {
        Timeout--;
        if (Timeout == 0) {
            return -1;
        }
    }

    /* Clear STOP flag */
    __HAL_I2C_CLEAR_STOPFLAG(handle);

    /* Wait until BUSY flag is reset */
    Timeout = FLAG_TIMEOUT;
    while (__HAL_I2C_GET_FLAG(handle, I2C_FLAG_BUSY) == SET) {
        Timeout--;
        if (Timeout == 0) {
            return -1;
        }
    }

    return size;
}

int i2c_slave_write(i2c_t *obj, const char *data, int length) {

    uint32_t Timeout;
    int size = 0;
    struct i2c_s *obj_s = I2C_S(obj);
    I2C_HandleTypeDef *handle = &(obj_s->handle);

    while (length > 0) {
        /* Wait until TXE flag is set */
        Timeout = FLAG_TIMEOUT;
        while (__HAL_I2C_GET_FLAG(handle, I2C_FLAG_TXE) == RESET) {
            Timeout--;
            if (Timeout == 0) {
                return -1;
            }
        }

        /* Write data to DR */
        handle->Instance->DR = (*data++);
        length--;
        size++;

        if ((__HAL_I2C_GET_FLAG(handle, I2C_FLAG_BTF) == SET) && (length != 0)) {
            /* Write data to DR */
            handle->Instance->DR = (*data++);
            length--;
            size++;
        }
    }

    /* Wait until AF flag is set */
    Timeout = FLAG_TIMEOUT;
    while (__HAL_I2C_GET_FLAG(handle, I2C_FLAG_AF) == RESET) {
        Timeout--;
        if (Timeout == 0) {
            return -1;
        }
    }

    /* Clear AF flag */
    __HAL_I2C_CLEAR_FLAG(handle, I2C_FLAG_AF);


    /* Wait until BUSY flag is reset */
    Timeout = FLAG_TIMEOUT;
    while (__HAL_I2C_GET_FLAG(handle, I2C_FLAG_BUSY) == SET) {
        Timeout--;
        if (Timeout == 0) {
            return -1;
        }
    }

    handle->State = HAL_I2C_STATE_READY;

    /* Process Unlocked */
    __HAL_UNLOCK(handle);

    return size;
}

#endif // DEVICE_I2CSLAVE

#if DEVICE_I2C_ASYNCH


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

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c){
    /* Get object ptr based on handler ptr */
    i2c_t *obj = get_i2c_obj(hi2c);
    struct i2c_s *obj_s = I2C_S(obj);

    /* Handle potential Tx/Rx use case */
    if ((obj->tx_buff.length) && (obj->rx_buff.length)) {

    if (obj_s->stop) {
            obj_s->XferOperation = I2C_LAST_FRAME;
        }
        else {
            obj_s->XferOperation = I2C_NEXT_FRAME;
        }

        HAL_I2C_Master_Sequential_Receive_IT(hi2c, obj_s->address, (uint8_t*)obj->rx_buff.buffer , obj->rx_buff.length, obj_s->XferOperation); 
    }
    else {
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
    I2C_HandleTypeDef *handle = &(obj_s->handle);

    /* Disable IT. Not always done before calling macro */
    __HAL_I2C_DISABLE_IT(handle, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);

    /* Set event flag */
    obj_s->event = I2C_EVENT_ERROR;
}

void HAL_I2C_AbortCpltCallback(I2C_HandleTypeDef *hi2c){
    /* Get object ptr based on handler ptr */
    i2c_t *obj = get_i2c_obj(hi2c);
    struct i2c_s *obj_s = I2C_S(obj);
    I2C_HandleTypeDef *handle = &(obj_s->handle);

    /* Disable IT. Not always done before calling macro */
    __HAL_I2C_DISABLE_IT(handle, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);

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

    IRQn_Type irq_event_n = obj_s->event_i2cIRQ;
    IRQn_Type irq_error_n = obj_s->error_i2cIRQ;

    /* Set up event IT using IRQ and handler tables */
    NVIC_SetVector(irq_event_n, handler);
    HAL_NVIC_SetPriority(irq_event_n, 0, 1);
    HAL_NVIC_EnableIRQ(irq_event_n);

    /* Set up error IT using IRQ and handler tables */
    NVIC_SetVector(irq_error_n, handler);
    HAL_NVIC_SetPriority(irq_error_n, 0, 0);
    HAL_NVIC_EnableIRQ(irq_error_n);

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

#endif // DEVICE_I2C
