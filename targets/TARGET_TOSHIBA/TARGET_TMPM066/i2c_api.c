/* mbed Microcontroller Library
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
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
#include "i2c_api.h"
#include "mbed_error.h"
#include "PeripheralNames.h"
#include "pinmap.h"

#define I2C_NACK                (0)
#define I2C_ACK                 (1)
#define I2C_NO_DATA             (0)
#define I2C_READ_ADDRESSED      (1)
#define I2C_WRITE_GENERAL       (2)
#define I2C_WRITE_ADDRESSED     (3)
#define SELF_ADDR               (0xE0)
#define I2C_TIMEOUT             (100000)

static const PinMap PinMap_I2C_SDA[] = {
    {PC1, I2C_0, PIN_DATA(1, 2)},
    {PG1, I2C_1, PIN_DATA(1, 2)},
    {NC,  NC,    0}
};

static const PinMap PinMap_I2C_SCL[] = {
    {PC0, I2C_0, PIN_DATA(1, 2)},
    {PG0, I2C_1, PIN_DATA(1, 2)},
    {NC,  NC,    0}
};

void I2C_ClearINTOutput(TSB_I2C_TypeDef * I2Cx);
// Clock setting structure definition
typedef struct {
    uint32_t sck;
    uint32_t prsck;
} I2C_clock_setting_t;

static const uint32_t I2C_SCK_DIVIDER_TBL[8] = {20, 24, 32, 48, 80, 144, 272, 528};  // SCK Divider value table
static uint32_t start_flag = 0;
I2C_clock_setting_t clk;
I2C_State status;

static int32_t wait_status(i2c_t *obj)
{
    volatile int32_t timeout = I2C_TIMEOUT;

    while (I2C_GetINTI2CStatus(obj->i2c) == DISABLE) {
        if ((timeout--) == 0) {
            return (-1);
        }
    }
    return (0);
}

// Initialize the I2C peripheral. It sets the default parameters for I2C
void i2c_init(i2c_t *obj, PinName sda, PinName scl)
{
    MBED_ASSERT(obj != NULL);
    I2CName i2c_sda = (I2CName)pinmap_peripheral(sda, PinMap_I2C_SDA);
    I2CName i2c_scl = (I2CName)pinmap_peripheral(scl, PinMap_I2C_SCL);
    I2CName i2c_name = (I2CName)pinmap_merge(i2c_sda, i2c_scl);
    MBED_ASSERT((int)i2c_name != NC);

    switch (i2c_name) {
        case I2C_0:
            CG_SetFcPeriphA(CG_FC_PERIPH_I2C0, ENABLE);
            obj->i2c = TSB_I2C0;
            obj->IRQn = INTI2C0_IRQn;
            break;
        case I2C_1:
            CG_SetFcPeriphB(CG_FC_PERIPH_I2C1, ENABLE);
            obj->i2c = TSB_I2C1;
            obj->IRQn = INTI2C1_IRQn;
            break;
        default:
            error("I2C is not available");
            break;
    }

    pinmap_pinout(sda, PinMap_I2C_SDA);
    pin_mode(sda, OpenDrain);
    pin_mode(sda, PullUp);

    pinmap_pinout(scl, PinMap_I2C_SCL);
    pin_mode(scl, OpenDrain);
    pin_mode(scl, PullUp);

    NVIC_DisableIRQ(obj->IRQn);

    i2c_reset(obj);
    i2c_frequency(obj, 100000);
}

// Configure the I2C frequency
void i2c_frequency(i2c_t *obj, int hz)
{
    uint64_t sck, tmp_sck;
    uint64_t prsck, tmp_prsck;
    uint64_t fscl, tmp_fscl;
    uint64_t fx;

    SystemCoreClockUpdate();

    if (hz <= 1000000) {
        sck   = tmp_sck   = 0;
        prsck = tmp_prsck = 1;
        fscl  = tmp_fscl  = 0;
        for (prsck = 1; prsck <= 32; prsck++) {
            fx = ((uint64_t)SystemCoreClock / prsck);
            if ((fx < 40000000U) && (fx > 6666666U)) {
                for (sck = 0; sck <= 7; sck++) {
                    fscl = (fx / (uint64_t)I2C_SCK_DIVIDER_TBL[sck]);
                    if ((fscl <= (uint64_t)hz) && (fscl > tmp_fscl)) {
                        tmp_fscl = fscl;
                        tmp_sck = sck;
                        tmp_prsck = (prsck < 32)? prsck: 0;
                    }
                }
            }
        }
        clk.sck = (uint32_t)tmp_sck;
        clk.prsck = (tmp_prsck < 32) ? (uint32_t)(tmp_prsck) : 0;
    }
    obj->myi2c.I2CSelfAddr = SELF_ADDR;
    obj->myi2c.I2CDataLen = I2C_DATA_LEN_8;
    obj->myi2c.I2CACKState = ENABLE;
    obj->myi2c.I2CClkDiv = clk.sck;
    obj->myi2c.PrescalerClkDiv = clk.prsck;

    I2C_Init(obj->i2c, &obj->myi2c);
    NVIC_DisableIRQ(obj->IRQn);
}

int i2c_start(i2c_t *obj)
{
    start_flag = 1;
    return 0;
}

int i2c_stop(i2c_t *obj)
{
    I2C_GenerateStop(obj->i2c);
    return 0;
}

void i2c_reset(i2c_t *obj)
{
    I2C_SWReset(obj->i2c);
}

int i2c_read(i2c_t *obj, int address, char *data, int length, int stop)
{
    int32_t result = 0;
    int32_t count = 0;

    if (length > 0) {
        start_flag = 1;  // Start Condition
        if (i2c_byte_write(obj, (int32_t)((uint32_t)address | 1U)) == I2C_ACK) {
            while (count < length) {
                int32_t pdata = i2c_byte_read(obj, ((count < (length - 1)) ? 0 : 1));
                if (pdata < 0) {
                    break;
                }
                data[count++] = (uint8_t)pdata;
            }
            result = count;
        } else {
            stop = 1;
            result = I2C_ERROR_NO_SLAVE;
        }

        if (stop) {  // Stop Condition
            i2c_stop(obj);
        }
    }
    return (result);
}

int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop)  // Blocking sending data
{
    int32_t result = 0;
    int32_t count = 0;

    start_flag = 1; // Start Condition
    if (i2c_byte_write(obj, address) == I2C_ACK) {
        while (count < length) {
            if (i2c_byte_write(obj, (int32_t)data[count++]) < I2C_ACK) {
                break;
            }
        }
        result = count;
    } else {
        stop = 1;
        result = I2C_ERROR_NO_SLAVE;
    }

    if (stop) {  // Stop Condition
        i2c_stop(obj);
    }
    return (result);
}

int i2c_byte_read(i2c_t *obj, int last)
{
    int32_t result;

    I2C_ClearINTOutput(obj->i2c);

    if (last) {
        I2C_SelectACKoutput(obj->i2c, ENABLE);
    } else {
        I2C_SelectACKoutput(obj->i2c, DISABLE);
    }
    I2C_SetSendData(obj->i2c, 0x00);

    if (wait_status(obj) < 0) {
        result = -1;
    } else {
        result = (int32_t)I2C_GetReceiveData(obj->i2c);
    }
    return (result);
}

void I2C_Start_Condition(i2c_t *p_obj, uint32_t data)
{
    status = I2C_GetState(p_obj->i2c);
    if (status.Bit.BusState) {
        I2C_SetRepeatStart(p_obj->i2c, ENABLE);
    }
    I2C_SetSendData(p_obj->i2c, (uint32_t)data);
    I2C_GenerateStart(p_obj->i2c);
}

int i2c_byte_write(i2c_t *obj, int data)
{
    int32_t result;

    I2C_ClearINTOutput(obj->i2c);

    if (start_flag == 1) {
        I2C_Start_Condition(obj, (uint32_t)data);
        start_flag = 0;
    } else {
        I2C_SetSendData(obj->i2c, (uint32_t)data);
    }

    if (wait_status(obj) < 0) {
        return (-1);
    }

    status = I2C_GetState(obj->i2c);
    if (!status.Bit.LastRxBit) {
        result = 1;
    } else {
        result = 0;
    }
    return (result);
}

void i2c_slave_mode(i2c_t *obj, int enable_slave)
{
    i2c_reset(obj);

    obj->myi2c.I2CDataLen = I2C_DATA_LEN_8;
    obj->myi2c.I2CACKState = ENABLE;
    obj->myi2c.I2CClkDiv = clk.sck;
    obj->myi2c.PrescalerClkDiv = clk.prsck;

    if (enable_slave) {
        obj->myi2c.I2CSelfAddr = obj->address;
        I2C_SetINTI2CReq(obj->i2c, ENABLE);
    } else {
        obj->myi2c.I2CSelfAddr = SELF_ADDR;
        NVIC_DisableIRQ(obj->IRQn);
        I2C_ClearINTOutput(obj->i2c);
    }
    I2C_Init(obj->i2c, &obj->myi2c);
}

int i2c_slave_receive(i2c_t *obj)
{
    int32_t result = I2C_NO_DATA;

    if  ((I2C_GetINTI2CStatus(obj->i2c)) && (I2C_GetSlaveAddrMatchState(obj->i2c))) {
        status = I2C_GetState(obj->i2c);
        if (!status.Bit.TRx) {
            result = I2C_WRITE_ADDRESSED;
        } else {
            result = I2C_READ_ADDRESSED;
        }
    }
    return (result);
}

int i2c_slave_read(i2c_t *obj, char *data, int length)
{
    int32_t count = 0;

    while (count < length) {
        int32_t pdata = i2c_byte_read(obj, ((count < (length - 1))? 0: 1));
        status = I2C_GetState(obj->i2c);
        if (status.Bit.TRx) {
            return (count);
        } else {
            if (pdata < 0) {
                break;
            }
            data[count++] = (uint8_t)pdata;
        }
    }
    i2c_slave_mode(obj, 1);
    return (count);
}

int i2c_slave_write(i2c_t *obj, const char *data, int length)
{
    int32_t count = 0;

    while (count < length) {
        if (i2c_byte_write(obj, (int32_t)data[count++]) < I2C_ACK) {
            break;
        }
    }
    i2c_slave_mode(obj, 1);
    return (count);
}

void i2c_slave_address(i2c_t *obj, int idx, uint32_t address, uint32_t mask)
{
    obj->address = address & 0xFE;
    i2c_slave_mode(obj, 1);
}
