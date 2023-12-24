/* mbed Microcontroller Library
 * Copyright (c) 2006-2023 ARM Limited
 * Copyright (c) 2023 Geehy Semiconductor Inc.
 *
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

#include "mbed_assert.h"
#include "i2c_api.h"

#if DEVICE_I2C

#include "cmsis.h"
#include "pinmap.h"
#include "PeripheralPins.h"

#if DEVICE_I2C_ASYNCH
#define I2C_S(obj) (struct i2c_s *) (&((obj)->i2c))
#else
#define I2C_S(obj) (struct i2c_s *) (obj)
#endif

typedef enum
{
    I2C_MODE_STANDBY            = 0x00U,                                         /*!< I2C standby        */
    I2C_MODE_MASTER             = 0x10U,                                         /*!< I2C in Master Mode */
    I2C_MODE_SLAVE              = 0x20U                                          /*!< I2C in Slave Mode  */
} i2c_mode_enum;

/* I2C state definitions */
#define I2C_STATE_MSK             ((uint32_t)((OP_STATE_BUSY_TX | OP_STATE_BUSY_RX) & (~(uint32_t)OP_STATE_BUSY)))
#define I2C_STATE_NONE            ((uint32_t)(I2C_MODE_STANDBY))
#define I2C_STATE_MASTER_BUSY_TX  ((uint32_t)((OP_STATE_BUSY_TX & I2C_STATE_MSK) | I2C_MODE_MASTER))
#define I2C_STATE_MASTER_BUSY_RX  ((uint32_t)((OP_STATE_BUSY_RX & I2C_STATE_MSK) | I2C_MODE_MASTER))
#define I2C_STATE_SLAVE_BUSY_TX   ((uint32_t)((OP_STATE_BUSY_TX & I2C_STATE_MSK) | I2C_MODE_SLAVE))
#define I2C_STATE_SLAVE_BUSY_RX   ((uint32_t)((OP_STATE_BUSY_RX & I2C_STATE_MSK) | I2C_MODE_SLAVE))

#define  I2C_FIRST_FRAME                0x00000001U
#define  I2C_NEXT_FRAME                 0x00000002U
#define  I2C_FIRST_AND_LAST_FRAME       0x00000004U
#define  I2C_LAST_FRAME                 0x00000008U
#define  I2C_NO_OPTION_FRAME            0xFFFF0000U

#define TIMEOUT_BUSY  ((SystemCoreClock / obj_s->freq) * 2 * 10)
#define TIMEOUT_FLAG  (0xF0000U)


 /*!
 * @brief     Reset I2C peripheral by hardware method. Most of the implementation enable RCU reset.
 *
 * @param     obj : The I2C object
 *
 * @retval    None
 */
static void i2c_hw_reset(i2c_t *obj)
{
    struct i2c_s *obj_s = I2C_S(obj);

    if (obj_s->i2c == I2C_1)
    {
        RCM_EnableAPB1PeriphReset(RCM_APB1_PERIPH_I2C1);
        RCM_DisableAPB1PeriphReset(RCM_APB1_PERIPH_I2C1);
    }
    else if (obj_s->i2c == I2C_2)
    {
        RCM_EnableAPB1PeriphReset(RCM_APB1_PERIPH_I2C2);
        RCM_DisableAPB1PeriphReset(RCM_APB1_PERIPH_I2C2);
    }
    else if (obj_s->i2c == I2C_3)
    {
        RCM_EnableAPB1PeriphReset(RCM_APB1_PERIPH_I2C3);
        RCM_DisableAPB1PeriphReset(RCM_APB1_PERIPH_I2C3);
    }
}

 /*!
 * @brief     Initialize the I2C peripheral. It sets the default parameters for I2C.
 *
 * @param     obj : The I2C object
 *
 * @param     sda : The sda pin
 *
 * @param     scl : The scl pin
 *
 * @retval    None
 */
void i2c_init(i2c_t *obj, PinName sda, PinName scl)
{
    struct i2c_s *obj_s = I2C_S(obj);

    /* find the I2C by pins */
    uint32_t i2c_sda = pinmap_peripheral(sda, PinMap_I2C_SDA);
    uint32_t i2c_scl = pinmap_peripheral(scl, PinMap_I2C_SCL);

    obj_s->sda = sda;
    obj_s->scl = scl;
    obj_s->i2c = (I2CName)pinmap_merge(i2c_sda, i2c_scl);
    MBED_ASSERT(obj_s->i2c != (I2CName)NC);
    I2C_T *i2c = (I2C_T *)obj_s->i2c;

    switch (obj_s->i2c)//obj->index is useless?,what's mean of obj->index?
    {
        case I2C_1:
            /* enable I2C1 clock and configure the pins of I2C1 */
            obj_s->index = 0;
            RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_I2C1);

            break;

        case I2C_2:
            /* enable I2C2 clock and configure the pins of I2C2 */
            obj_s->index = 1;
            RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_I2C2);

            break;

        case I2C_3:
            /* enable I2C3 clock and configure the pins of I2C3 */
            obj_s->index = 2;
            RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_I2C3);

            break;

        default:
            break;
    }
    /* configure the pins of I2C */
    pinmap_pinout(sda, PinMap_I2C_SDA);
    pinmap_pinout(scl, PinMap_I2C_SCL);

    pin_mode(sda, OpenDrainPullUp);
    pin_mode(scl, OpenDrainPullUp);

    I2C_Reset(i2c);

    /* 100 KHz as the default I2C frequence */
    i2c_frequency(obj, 100000);

    obj_s->state = (OP_STATE_T)I2C_MODE_STANDBY;               
    obj_s->previous_state_mode = I2C_MODE_STANDBY;
    obj_s->global_trans_option = I2C_FIRST_AND_LAST_FRAME;

#if DEVICE_I2CSLAVE
    /* I2C master by default */
    obj_s->slave = 0;
#endif
}

 /*!
 * @brief     Configure the I2C frequency
 *
 * @param     obj : The I2C object
 *
 * @param     hz  : Frequency in Hz
 *
 * @retval    None
 */
void i2c_frequency(i2c_t *obj, int hz)
{
    int timeout;
    struct i2c_s *obj_s = I2C_S(obj);
    I2C_T *i2c = (I2C_T *)obj_s->i2c;
    I2C_Config_T i2cConfigStruct;

    /* wait until I2C_FLAG_I2CBSY flag is reset */
    timeout = TIMEOUT_BUSY;
    while (I2C_ReadStatusFlag(i2c, I2C_FLAG_BUSBSY) && (--timeout != 0));

    i2c_hw_reset(obj);
    /* disable I2C peripheral */
    I2C_Disable(i2c);

    i2cConfigStruct.mode = I2C_MODE_I2C;
    i2cConfigStruct.dutyCycle = I2C_DUTYCYCLE_2;
    i2cConfigStruct.ackAddress = I2C_ACK_ADDRESS_7BIT;
    i2cConfigStruct.ownAddress1 = 0x00;
    i2cConfigStruct.ack = I2C_ACK_DISABLE;
    i2cConfigStruct.clockSpeed = hz;

    I2C_Config(i2c, &i2cConfigStruct);

    /* enable I2C peripheral */
    I2C_Enable(i2c);
}

 /*!
 * @brief     Reset I2C peripheral. TODO: The action here. Most of the implementation sends stop()
 *
 * @param     obj : The I2C object
 *
 * @retval    None
 */
void i2c_reset(i2c_t *obj)
{
    i2c_stop(obj);
}

 /*!
 * @brief     Send START command
 *
 * @param     obj : The I2C object
 *
 * @retval    None
 */
int i2c_start(i2c_t *obj)
{
    int timeout;
    struct i2c_s *obj_s = I2C_S(obj);
    I2C_T *i2c = (I2C_T *)obj_s->i2c;

    /* clear I2C_FLAG_AERR Flag */
    I2C_ClearStatusFlag(i2c, I2C_FLAG_AE);

    /* wait until I2C_FLAG_I2CBSY flag is reset */
    timeout = TIMEOUT_FLAG;
    while ((I2C_ReadStatusFlag(i2c, I2C_FLAG_BUSBSY)) == SET)
    {
        if ((timeout--) == 0)
        {
            return (int)APM_BUSY;
        }
    }
    /* wait until I2C_FLAG_I2CBSY flag is reset */
    timeout = TIMEOUT_FLAG;
    while (i2c->CTRL1_B.STOP == SET)
    {
        if ((timeout--) == 0)
        {
            return (int)APM_ERROR;
        }
    }
    
    /* generate a START condition */
    I2C_EnableGenerateStart(i2c);

    /* ensure the i2c has been started successfully */
    timeout = TIMEOUT_FLAG;
    while ((I2C_ReadStatusFlag(i2c, I2C_FLAG_START)) == RESET)
    {
        if ((timeout--) == 0)
        {
            return (int)APM_ERROR;
        }
    }

    return (int)APM_OK;
}

 /*!
 * @brief     Send STOP command
 *
 * @param     obj : The I2C object
 *
 * @retval    None
 */
int i2c_stop(i2c_t *obj)
{
    struct i2c_s *obj_s = I2C_S(obj);
    I2C_T *i2c = (I2C_T *)obj_s->i2c;

    /* generate a STOP condition */
    I2C_EnableGenerateStop(i2c);

    /* wait for STOP bit reset */
    while (i2c->CTRL1_B.STOP);

    return 0;
}

 /*!
 * @brief     Read one byte
 *
 * @param     obj : The I2C object
 *
 * @param     last : Acknoledge
 *
 * @retval    The read byte
 */
int i2c_byte_read(i2c_t *obj, int last)
{   
    int timeout;
    struct i2c_s *obj_s = I2C_S(obj);
    I2C_T *i2c = (I2C_T *)obj_s->i2c;

    if (last)
    {
        /* disable acknowledge */
        I2C_DisableAcknowledge(i2c);//clear ACK
    }
    else
    {
        /* enable acknowledge */
        I2C_EnableAcknowledge(i2c);
    }

    /* wait until the byte is received */
    timeout = TIMEOUT_FLAG;
    while ((I2C_ReadStatusFlag(i2c, I2C_FLAG_RXBNE)) == RESET)
    {
        if ((timeout--) == 0)
        {
            return -1;
        }
    }

    return (int)(i2c)->DATA;
}

 /*!
 * @brief     Write one byte
 *
 * @param     obj : The I2C object
 *
 * @param     data : Byte to be written
 *
 * @retval    0 if NAK was received, 1 if ACK was received, 2 for timeout.
 */
int i2c_byte_write(i2c_t *obj, int data)
{
    int timeout;
    struct i2c_s *obj_s = I2C_S(obj);
    I2C_T *i2c = (I2C_T *)obj_s->i2c;

    ((I2C_T *)obj_s->i2c)->DATA = (uint8_t)data;

    /* wait until the byte is transmitted */
    timeout = TIMEOUT_FLAG;
    while (((I2C_ReadStatusFlag(i2c, I2C_FLAG_TXBE)) == RESET) && \
            ((I2C_ReadStatusFlag(i2c, I2C_FLAG_BTC)) == RESET))
    {
        if ((timeout--) == 0)
        {
            return 2;
        }
    }

    return 1;
}

 /*!
 * @brief     Blocking reading data
 *
 * @param     obj : The I2C object
 *
 * @param     address : 7-bit address (last bit is 1)
 *
 * @param     data : Byte to be written
 *
 * @param     length : Number of bytes to read
 *
 * @param     stop : Stop to be generated after the transfer is done
 *
 * @retval    Number of read bytes
 */
int i2c_read(i2c_t *obj, int address, char *data, int length, int stop)
{
    struct i2c_s *obj_s = I2C_S(obj);
    I2C_T *i2c = (I2C_T *)obj_s->i2c;
    uint32_t count = 0U;
    int timeout = 0;

    if (obj_s->global_trans_option == I2C_FIRST_AND_LAST_FRAME || \
        obj_s->global_trans_option == I2C_LAST_FRAME)
    {
        if (stop)
        {
            obj_s->global_trans_option = I2C_FIRST_AND_LAST_FRAME;
        }
        else
        {
            obj_s->global_trans_option = I2C_FIRST_FRAME;
        }
    } 
    else if (obj_s->global_trans_option == I2C_FIRST_FRAME || \
             obj_s->global_trans_option == I2C_NEXT_FRAME)
    {
        if (stop)
        {
            obj_s->global_trans_option = I2C_LAST_FRAME;
        } 
        else
        {
            obj_s->global_trans_option = I2C_NEXT_FRAME;
        }
    }

    if (obj_s->global_trans_option == I2C_FIRST_AND_LAST_FRAME || obj_s->global_trans_option == I2C_FIRST_FRAME)
    {
        /* wait until I2C_FLAG_I2CBSY flag is reset */
        timeout = TIMEOUT_FLAG;
        while ((I2C_ReadStatusFlag(i2c, I2C_FLAG_BUSBSY)) == SET)
        {
            if ((timeout--) == 0)
            {
                i2c_stop(obj);
                return I2C_ERROR_BUS_BUSY;
            }
        }
    }

    if (obj_s->global_trans_option == I2C_FIRST_AND_LAST_FRAME || obj_s->global_trans_option == I2C_FIRST_FRAME ||
            obj_s->previous_state_mode != I2C_STATE_MASTER_BUSY_RX)
    {
        /* generate a START condition */
        I2C_EnableGenerateStart(i2c);

        /* ensure the i2c has been started successfully */
        timeout = TIMEOUT_FLAG;
        while ((I2C_ReadStatusFlag(i2c, I2C_FLAG_START)) == RESET)
        {
            if ((timeout--) == 0)
            {
                i2c_stop(obj);
                return I2C_ERROR_BUS_BUSY;
            }
        }

        /* send slave address */
        I2C_Tx7BitAddress(i2c, address, I2C_DIRECTION_RX);

        /* wait until I2C_FLAG_ADDSEND flag is set */
        timeout = 0;
        while (!I2C_ReadStatusFlag(i2c, I2C_FLAG_ADDR)) 
        {
            timeout++;
            if (timeout > 100000)
            {
                i2c_stop(obj);
                return I2C_ERROR_NO_SLAVE;
            }
        }

        if (length == 1)
        {
            /* disable acknowledge */
            I2C_DisableAcknowledge(i2c);
            /* send a stop condition to I2C bus */
        }
        else if (length == 2)
        {
            /* send a NACK for the next data byte which will be received into the shift register */
            I2C_ConfigNACKPosition(i2c, I2C_NACK_POSITION_NEXT);
            /* disable acknowledge */
            I2C_DisableAcknowledge(i2c);
        }
        else
        {
            /* enable acknowledge */
            I2C_EnableAcknowledge(i2c);
        }

        timeout = 0;
        /* wait until I2C_FLAG_ADDSEND flag is set */
        while (!I2C_ReadStatusFlag(i2c, I2C_FLAG_ADDR)) {
            timeout++;
            if (timeout > 100000) {
                i2c_stop(obj);
                return I2C_ERROR_NO_SLAVE;
            }
        }

        I2C_ClearStatusFlag(i2c, I2C_FLAG_ADDR);
    }

    obj_s->state = (OP_STATE_T)I2C_STATE_MASTER_BUSY_RX;

    for (count = 0; count < length; count++)
    {
        if (length > 2 && count == length - 3)
        {
            while (I2C_ReadStatusFlag(i2c, I2C_FLAG_BTC) == RESET);
            I2C_DisableAcknowledge(i2c);
        }
        else if (length == 2 && count == 0)
        {
            while (I2C_ReadStatusFlag(i2c, I2C_FLAG_BTC) == RESET);
        }

        while (I2C_ReadStatusFlag(i2c, I2C_FLAG_RXBNE) == RESET);
        data[count] = I2C_RxData(i2c);
    }

    obj_s->previous_state_mode = obj_s->state;

    /* if not sequential read, then send stop */
    if (stop)
    {
        i2c_stop(obj);
    }

    return count;
}

 /*!
 * @brief     Blocking sending data
 *
 * @param     obj : The I2C object
 *
 * @param     address : 7-bit address (last bit is 1)
 *
 * @param     data : Byte to be written
 *
 * @param     length : Number of bytes to read
 *
 * @param     stop : Stop to be generated after the transfer is done
 *
 * @retval    zero or non-zero - Number of written bytes,negative - I2C_ERROR_XXX status
 */
int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop)
{
    struct i2c_s *obj_s = I2C_S(obj);
    APM_STATUS_T status = APM_OK;
    I2C_T *i2c = (I2C_T *)obj_s->i2c;
    uint32_t count = 0;
    int timeout = 0;

    if (obj_s->global_trans_option == I2C_FIRST_AND_LAST_FRAME || obj_s->global_trans_option == I2C_LAST_FRAME)
    {
        if (stop)
        {
            obj_s->global_trans_option = I2C_FIRST_AND_LAST_FRAME;
        }
        else
        {
            obj_s->global_trans_option = I2C_FIRST_FRAME;
        }
    }
    else if (obj_s->global_trans_option == I2C_FIRST_FRAME || obj_s->global_trans_option == I2C_NEXT_FRAME)
    {
        if (stop)
        {
            obj_s->global_trans_option = I2C_LAST_FRAME;
        }
        else
        {
            obj_s->global_trans_option = I2C_NEXT_FRAME;
        }
    }
    
    if (obj_s->global_trans_option == I2C_FIRST_AND_LAST_FRAME || obj_s->global_trans_option == I2C_FIRST_FRAME)
    {
        /* wait until I2C_FLAG_I2CBSY flag is reset */
        timeout = TIMEOUT_FLAG;
        while ((I2C_ReadStatusFlag(i2c, I2C_FLAG_BUSBSY)) == SET)
        {
            if ((timeout--) == 0)
            {
                i2c_stop(obj);
                return I2C_ERROR_BUS_BUSY;
            }
        }
    }
    
    if (obj_s->global_trans_option == I2C_FIRST_AND_LAST_FRAME || obj_s->global_trans_option == I2C_FIRST_FRAME || \
            obj_s->previous_state_mode != I2C_STATE_MASTER_BUSY_TX)
    {
        /* generate a START condition */
        I2C_EnableGenerateStart(i2c);

        /* ensure the i2c has been started successfully */
        timeout = TIMEOUT_FLAG;
        while ((I2C_ReadStatusFlag(i2c, I2C_FLAG_START)) == RESET)
        {
            if ((timeout--) == 0) {
                i2c_stop(obj);
                return I2C_ERROR_BUS_BUSY;
            }
        }

        /* send slave address */
        I2C_Tx7BitAddress(i2c, address, I2C_DIRECTION_TX);

        timeout = 0;
        /* wait until I2C_FLAG_ADDSEND flag is set */
        while (!I2C_ReadStatusFlag(i2c, I2C_FLAG_ADDR))
        {
            timeout++;
            if (timeout > 100000)
            {
                i2c_stop(obj);
                return I2C_ERROR_NO_SLAVE;
            }
        }

        /* clear ADDSEND */
        I2C_ClearStatusFlag(i2c, I2C_FLAG_ADDR);
    }

    obj_s->state = (OP_STATE_T)I2C_STATE_MASTER_BUSY_TX;

    for (count = 0; count < length; count++)
    {
        status = (APM_STATUS_T)i2c_byte_write(obj, data[count]);
        if (status != 1)
        {
            i2c_stop(obj);
            return count;
        }
    }

    obj_s->previous_state_mode = obj_s->state;

    /* if not sequential write, then send stop */
    if (stop)
    {
        i2c_stop(obj);
    }

    return count;
}

const PinMap *i2c_master_sda_pinmap()
{
    return PinMap_I2C_SDA;
}

const PinMap *i2c_master_scl_pinmap()
{
    return PinMap_I2C_SCL;
}

const PinMap *i2c_slave_sda_pinmap()
{
    return PinMap_I2C_SDA;
}

const PinMap *i2c_slave_scl_pinmap()
{
    return PinMap_I2C_SCL;
}

#if DEVICE_I2CSLAVE

 /*!
 * @brief     Configure I2C address
 *
 * @param     obj : The I2C object
 *
 * @param     idx : Currently not used
 *
 * @param     address : The address to be set
 *
 * @param     mask : Currently not used
 *
 * @retval    None
 */
void i2c_slave_address(i2c_t *obj, int idx, uint32_t address, uint32_t mask)
{
    struct i2c_s *obj_s = I2C_S(obj);
    I2C_T *i2c = (I2C_T *)obj_s->i2c;
    I2C_Config_T i2cConfigStruct;

    /* disable I2C peripheral */
    I2C_Disable(i2c);

    i2cConfigStruct.mode = I2C_MODE_I2C;
    i2cConfigStruct.dutyCycle = I2C_DUTYCYCLE_2;
    i2cConfigStruct.ackAddress = I2C_ACK_ADDRESS_7BIT;
    i2cConfigStruct.ownAddress1 = address;
    i2cConfigStruct.ack = I2C_ACK_ENABLE;
    i2cConfigStruct.clockSpeed = 100000;

    I2C_Config(I2C1, &i2cConfigStruct);

    /* enable I2C0 */
    I2C_Enable(i2c);
}

 /*!
 * @brief     Configure I2C as slave or master.
 *
 * @param     obj : The I2C object
 *
 * @param     enable_slave : Enable i2c hardware so you can receive events with ::i2c_slave_receive
 *
 * @retval    non-zero if a value is available
 */
void i2c_slave_mode(i2c_t *obj, int enable_slave)
{
    struct i2c_s *obj_s = I2C_S(obj);

    if (enable_slave)
    {
        obj_s->slave = 1;
    } 
    else
    {
        obj_s->slave = 0;
    }
}

 /*!
 * @brief     Check to see if the I2C slave has been addressed.
 *
 * @param     obj : The I2C object
 *
 * @retval    The status 1 - read addresses, 2 - write to all slaves,
 *            3 write addressed, 0 - the slave has not been addressed
 */
int i2c_slave_receive(i2c_t *obj)
{
    struct i2c_s *obj_s = I2C_S(obj);
    I2C_T *i2c = (I2C_T *)obj_s->i2c;
    int ret = 0;

    I2C_EnableAcknowledge(i2c);

    if (I2C_ReadStatusFlag(i2c, I2C_FLAG_ADDR))
    {
        I2C_EnableAcknowledge(i2c);

        if (I2C_ReadStatusFlag(i2c, I2C_FLAG_GENCALL))
        {
            ret = 2;
        }
        if (I2C_ReadStatusFlag(i2c, I2C_FLAG_TR))
        {
            ret = 1;
        }
        else
        {
            ret = 3;
        }

    }

    I2C_EnableAcknowledge(i2c);

    return (ret);
}

 /*!
 * @brief     Configure I2C as slave or master.
 *
 * @param     obj : The I2C object
 *
 * @param     data : The buffer for receiving
 *
 * @param     length : Number of bytes to read
 *
 * @retval    non-zero if a value is available
 */
int i2c_slave_read(i2c_t *obj, char *data, int length)
{
    struct i2c_s *obj_s = I2C_S(obj);
    I2C_T *i2c = (I2C_T *)obj_s->i2c;
    int count = 0;
    int timeout = 0;

    I2C_EnableAcknowledge(i2c);

    /* wait until ADDSEND bit is set */
    while (!I2C_ReadStatusFlag(i2c, I2C_FLAG_ADDR))
    {
        timeout++;
        if (timeout > 100000)
        {
            return -1;
        }
    }
    /* clear ADDSEND bit */

    I2C_ClearStatusFlag(i2c, I2C_FLAG_ADDR);

    while (length > 0)
    {
        /* wait until the RBNE bit is set */
        timeout = 0;
        while (!I2C_ReadStatusFlag(i2c, I2C_FLAG_RXBNE))
        {
            timeout++;
            if (timeout > 100000)
            {
                return -1;
            }
        }
        *data = I2C_RxData(i2c);
        data++;
        length--;
        count++;
    }
    /* wait until the STPDET bit is set */
    timeout = 0;
    while (!I2C_ReadStatusFlag(i2c, I2C_FLAG_STOP))
    {
        timeout++;
        if (timeout > 100)
        {
            return count;
        }
    }
    /* clear the STPDET bit */
    I2C_ClearStatusFlag(i2c, I2C_FLAG_STOP);
    I2C_Enable(i2c);

    I2C_DisableAcknowledge(i2c);

    return count;
}

 /*!
 * @brief     Configure I2C as slave or master.
 *
 * @param     obj : The I2C object
 *
 * @param     data : The buffer for receiving
 *
 * @param     length : Number of bytes to read
 *
 * @retval    non-zero if a value is available
 */
int i2c_slave_write(i2c_t *obj, const char *data, int length)
{
    struct i2c_s *obj_s = I2C_S(obj);
    I2C_T *i2c = (I2C_T *)obj_s->i2c;
    int count = 0;
    int timeout = 0;

    I2C_EnableAcknowledge(i2c);
    /* wait until ADDSEND bit is set */
    while (!I2C_ReadStatusFlag(i2c, I2C_FLAG_ADDR))
    {
        timeout++;
        if (timeout > 100000)
        {
            return -1;
        }
    }
    /* clear ADDSEND bit */
    I2C_ClearStatusFlag(i2c, I2C_FLAG_ADDR);
    while (length > 0)
    {
        /* wait until the TBE bit is set */
        timeout = 0;
        while (!I2C_ReadStatusFlag(i2c, I2C_FLAG_TXBE))
        {
            timeout++;
            if (timeout > 100000)
            {
                return -1;
            }
        }
        I2C_TxData(i2c, *data);
        data++;
        length--;
        count++;
    }
    /* the master doesn't acknowledge for the last byte */
    timeout = 0;
    while (!I2C_ReadStatusFlag(i2c, I2C_FLAG_AE))
    {
        timeout++;
        if (timeout > 100000)
        {
            return -1;
        }
    }
    /* clear the bit of AERR */
    I2C_ClearStatusFlag(i2c, I2C_FLAG_AE);
    /* disable acknowledge */
    I2C_DisableAcknowledge(i2c);

    return count;
}
#endif /* DEVICE_I2CSLAVE */

#endif /* DEVICE_I2C */