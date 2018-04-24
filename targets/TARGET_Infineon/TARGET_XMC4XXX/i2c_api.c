/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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

/***************************************************************** Includes **/
#include "pinmap.h"
#include "PeripheralPins.h"
#include "xmc4_helper.h"
#include "mbed_assert.h"
#include "i2c_api.h"
#include "xmc_gpio.h"
#include "xmc_i2c.h"

/****************************************************************** Defines **/
#define I2C_TIMEOUT 100000

#define I2C_START_SEND      0
#define I2C_STOP_SEND       1

/******************************************************************* Macros **/

/****************************************************************** Globals **/

/* i2c configuration */
XMC_I2C_CH_CONFIG_t i2c_cfg =
{
        .baudrate = 50000U, // Default to 100kHz
};

/* pin configuration for i2c sda */
XMC_GPIO_CONFIG_t i2c_sda_pin_cfg =
{
        .output_strength = XMC_GPIO_OUTPUT_STRENGTH_MEDIUM
};

/* pin configuration for i2c sdc */
XMC_GPIO_CONFIG_t i2c_scl_pin_cfg =
{
        .output_strength = XMC_GPIO_OUTPUT_STRENGTH_MEDIUM
};

/******************************************************* Internal Functions **/


/**************************************************************** Functions **/

/** Initialize the I2C peripheral. It sets the default parameters for I2C
 *  peripheral, and configures its specifieds pins.
 *
 *  @param obj  The I2C object
 *  @param sda  The sda pin
 *  @param scl  The scl pin
 */
void i2c_init(i2c_t *obj, PinName sda, PinName scl){

    /* Sanity check arguments */
    MBED_ASSERT(obj);

    /* Determine corresponding USIC module & channel */
    I2CName i2c_sda = (I2CName)pinmap_peripheral(sda, PinMap_I2C_SDA);
    I2CName i2c_sdc = (I2CName)pinmap_peripheral(scl, PinMap_I2C_SCL);
    I2CName i2c = (I2CName)pinmap_merge(i2c_sda, i2c_sdc);
    /* Check if both pins are a possible I2C pair */
    MBED_ASSERT((int)i2c != NC);

    /* Get SDA and SCL pin alternative functions */
    uint8_t sda_function = (uint8_t)pinmap_function(sda, PinMap_I2C_SDA);
    uint8_t scl_function = (uint8_t)pinmap_function(scl, PinMap_I2C_SCL);

    /* Get USIC BASE address */
    XMC_USIC_CH_t *channel = ((XMC_USIC_CH_t *)i2c);
    obj->channel = channel;
    obj->start_stop = I2C_STOP_SEND;

    /* Get RX pin function */
    pin_io_function_t *sda_cfg = get_pin_function(sda);
    uint8_t clk_source = 0;

    /* Set UART index */
    switch ((int)obj->channel) {
    case I2C_0:
        obj->dx0 = sda_cfg->usic0_ch0_dx0;
        clk_source = USIC0_C0_DX1_SCLKOUT;
        break;
    case I2C_1:
        obj->dx0 = sda_cfg->usic0_ch1_dx0;
        clk_source = USIC0_C1_DX1_SCLKOUT;
        break;
#ifdef USIC1

    case I2C_2:
        obj->dx0 = sda_cfg->usic1_ch0_dx0;
        clk_source = USIC1_C0_DX1_SCLKOUT;
        break;
    case I2C_3:
        obj->dx0 = sda_cfg->usic1_ch1_dx0;
        clk_source = USIC1_C1_DX1_SCLKOUT;
        break;
#endif
#ifdef USIC2

    case I2C_4:
        obj->dx0 = sda_cfg->usic2_ch0_dx0;
        clk_source = USIC2_C0_DX1_SCLKOUT;
        break;
    case I2C_5:
        obj->dx0 = sda_cfg->usic2_ch1_dx0;
        clk_source = USIC2_C1_DX1_SCLKOUT;
        break;
#endif

    }

    /* Get Port Base address */
    XMC_GPIO_PORT_t *port_sda = (XMC_GPIO_PORT_t *)XMC4_GET_PORT_BASE_FROM_NAME(sda);
    XMC_GPIO_PORT_t *port_scl = (XMC_GPIO_PORT_t *)XMC4_GET_PORT_BASE_FROM_NAME(scl);

    /* Initialize I2C channel */
    XMC_I2C_CH_Init(channel, &i2c_cfg);
    XMC_I2C_CH_SetInputSource(channel, XMC_I2C_CH_INPUT_SDA, obj->dx0);
    XMC_I2C_CH_SetInputSource(channel, XMC_I2C_CH_INPUT_SCL, clk_source);
    XMC_I2C_CH_Start(channel);

    /* Initialize SDA and SCL Pins */
    i2c_sda_pin_cfg.mode = XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN | (sda_function << PORT0_IOCR0_PC0_Pos);
    i2c_scl_pin_cfg.mode = XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN | (scl_function << PORT0_IOCR0_PC0_Pos);
    XMC_GPIO_Init(port_sda, XMC4_GET_PIN_NBR_FROM_NAME(sda), &i2c_sda_pin_cfg);
    XMC_GPIO_Init(port_scl, XMC4_GET_PIN_NBR_FROM_NAME(scl), &i2c_scl_pin_cfg);
}

/** Configure the I2C frequency
 *
 *  @param obj The I2C object
 *  @param hz  Frequency in Hz
 */
void i2c_frequency(i2c_t *obj, int hz){

    /* Sanity check arguments */
    MBED_ASSERT(obj);

    XMC_I2C_CH_SetBaudrate(obj->channel, (uint32_t)hz);
}

/** Send START command
 *
 *  @param obj The I2C object
 */
int  i2c_start(i2c_t *obj){

    /* Not supported */
    return 0;
}

/** Send STOP command
 *
 *  @param obj The I2C object
 */
int  i2c_stop(i2c_t *obj){

    XMC_I2C_CH_MasterStop(obj->channel);
    obj->start_stop = I2C_STOP_SEND;

    return 0;
}

/** Read one byte
 *
 *  @param obj The I2C object
 *  @param last Acknoledge
 *  @return The read byte
 */
int i2c_byte_read(i2c_t *obj, int last){

    int timeout = I2C_TIMEOUT;

    /* Master ACK or NACK */
    if(last){
        XMC_I2C_CH_MasterReceiveNack(obj->channel);
    }else{
        XMC_I2C_CH_MasterReceiveAck(obj->channel);
    }

    while(!(XMC_I2C_CH_GetStatusFlag(obj->channel) & (XMC_I2C_CH_STATUS_FLAG_RECEIVE_INDICATION |
            XMC_I2C_CH_STATUS_FLAG_ALTERNATIVE_RECEIVE_INDICATION)) && (timeout--))
    {
        /* wait for ACK */
    }
    XMC_I2C_CH_ClearStatusFlag(obj->channel, XMC_I2C_CH_STATUS_FLAG_RECEIVE_INDICATION |
            XMC_I2C_CH_STATUS_FLAG_ALTERNATIVE_RECEIVE_INDICATION);

    if(timeout <= 0){
        return 0;
    }

    return (int)XMC_I2C_CH_GetReceivedData(obj->channel);
}

/** Write one byte
 *
 *  @param obj The I2C object
 *  @param data Byte to be written
 *  @return 0 if NAK was received, 1 if ACK was received, 2 for timeout.
 */
int i2c_byte_write(i2c_t *obj, int data){

    int timeout = I2C_TIMEOUT;

    /* Set register to write */
    XMC_I2C_CH_MasterTransmit(obj->channel, (uint8_t)data);
    while(!(XMC_I2C_CH_GetStatusFlag(obj->channel) & (XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED | XMC_I2C_CH_STATUS_FLAG_NACK_RECEIVED)) && (timeout--))
    {
        /* wait for ACK */
    }
    /* Check if timeout */
    if(timeout <= 0){
        return 2;
    }
    /* Check if NACK */
    if(XMC_I2C_CH_GetStatusFlag(obj->channel) & XMC_I2C_CH_STATUS_FLAG_NACK_RECEIVED){
        return 0;
    }
    XMC_I2C_CH_ClearStatusFlag(obj->channel, (XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED | XMC_I2C_CH_STATUS_FLAG_NACK_RECEIVED));

    /* ACK received */
    return 1;
}

/** Blocking reading data
 *
 *  @param obj     The I2C object
 *  @param address 7-bit address (last bit is 1)
 *  @param data    The buffer for receiving
 *  @param length  Number of bytes to read
 *  @param stop    Stop to be generated after the transfer is done
 *  @return Number of read bytes
 */
int i2c_read(i2c_t *obj, int address, char *data, int length, int stop){

    int i = 0;
    int timeout = I2C_TIMEOUT;
    int rec_data;

    /* check if start or restart condition must be send */
    if(obj->start_stop == I2C_STOP_SEND){
        XMC_I2C_CH_MasterStart(obj->channel, address, XMC_I2C_CH_CMD_READ);
        obj->start_stop = I2C_START_SEND;
    }else{
        XMC_I2C_CH_MasterRepeatedStart(obj->channel, address, XMC_I2C_CH_CMD_READ);
    }

    while(!(XMC_I2C_CH_GetStatusFlag(obj->channel) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) && (timeout--))
    {
        /* wait for ACK */
    }
    XMC_I2C_CH_ClearStatusFlag(obj->channel, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

    while (i < length) {
        /* Master ACK or NACK */
        if(i == (length - 1)){
            rec_data = i2c_byte_read(obj, 1);
        }else{
            rec_data = i2c_byte_read(obj, 0);
        }

        if(data == 0){
            return i;
        }

        data[i++] = rec_data;
    }

    if (stop) {
        i2c_stop(obj);
        obj->start_stop = I2C_STOP_SEND;
    }

    return length;
}

/** Blocking sending data
 *
 *  @param obj     The I2C object
 *  @param address 7-bit address (last bit is 0)
 *  @param data    The buffer for sending
 *  @param length  Number of bytes to write
 *  @param stop    Stop to be generated after the transfer is done
 *  @return
 *      zero or non-zero - Number of written bytes
 *      negative - I2C_ERROR_XXX status
 */
int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop){

    int timeout = I2C_TIMEOUT;

    /* Generate start condition and send slave address with R/W bit */
    XMC_I2C_CH_MasterStart(obj->channel, address, XMC_I2C_CH_CMD_WRITE);
    while(!(XMC_I2C_CH_GetStatusFlag(obj->channel) & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) && (timeout--))
    {
        /* wait for ACK */
    }
    XMC_I2C_CH_ClearStatusFlag(obj->channel, XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED);

    if(timeout <= 0){
        return I2C_ERROR_NO_SLAVE;
    }
    obj->start_stop = I2C_START_SEND;

    /* Write number of bytes */
    for (int i = 0; i < length; i++) {
        if (!i2c_byte_write(obj, data[i])) {
            i2c_stop(obj);
            return i;
        }
    }

    if (stop) {
        i2c_stop(obj);
        obj->start_stop = I2C_STOP_SEND;
    }

    return length;
}

/** Reset I2C peripheral.
 *
 *  @param obj The I2C object
 */
void i2c_reset(i2c_t *obj){

    /* Apply stop condition */
    i2c_stop(obj);
    obj->start_stop = I2C_STOP_SEND;
}

/*EOF*/
