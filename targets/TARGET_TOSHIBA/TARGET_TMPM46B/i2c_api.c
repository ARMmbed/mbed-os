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
#include "tmpm46b_i2c.h"
#include <string.h>
#include <stdlib.h>

static const PinMap PinMap_I2C_SDA[] = {
    {PK2, I2C_0, PIN_DATA(3, 2)},
    {PF7, I2C_1, PIN_DATA(4, 2)},
    {PH0, I2C_2, PIN_DATA(4, 2)},
    {NC,  NC,    0}
};

static const PinMap PinMap_I2C_SCL[] = {
    {PK3, I2C_0, PIN_DATA(3, 2)},
    {PF6, I2C_1, PIN_DATA(4, 2)},
    {PH1, I2C_2, PIN_DATA(4, 2)},
    {NC,  NC,    0}
};

#define SBI_I2C_SEND              0x00
#define SBI_I2C_RECEIVE           0x01
#define MAX_NUM_I2C               3
#define DELAY_MS_MULTIPLIER       5500

struct i2c_xfer {
    int32_t count;
    int32_t len;
    void *done;
    char *buf;
};

// Clock setting structure definition
typedef struct {
    uint32_t sck;
    uint32_t prsck;
} I2C_clock_setting_t;

static void DelayMS(uint32_t delay)
{
    volatile uint32_t VarI;
    for (VarI = 0; VarI < delay * DELAY_MS_MULTIPLIER; VarI++);
}

static const uint32_t I2C_SCK_DIVIDER_TBL[8] = {
    20, 24, 32, 48, 80, 144, 272, 528
};  // SCK Divider value table

static I2C_clock_setting_t clk;
static I2C_InitTypeDef myi2c;
static int32_t start_flag = 1;
static struct i2c_xfer xfer[MAX_NUM_I2C];
static TSB_I2C_TypeDef *i2c_lut[MAX_NUM_I2C] = {TSB_I2C0, TSB_I2C1, TSB_I2C2};
static char *gI2C_TxData = NULL;
static char *gI2C_LTxData = NULL;
static uint8_t send_byte = 0;
static uint8_t byte_func = 0;

// Initialize the I2C peripheral. It sets the default parameters for I2C
void i2c_init(i2c_t *obj, PinName sda, PinName scl)
{
    MBED_ASSERT(obj != NULL);
    I2CName i2c_sda = (I2CName)pinmap_peripheral(sda, PinMap_I2C_SDA);
    I2CName i2c_scl = (I2CName)pinmap_peripheral(scl, PinMap_I2C_SCL);
    I2CName i2c_name = (I2CName)pinmap_merge(i2c_sda, i2c_scl);
    MBED_ASSERT((int)i2c_name != NC);

    switch(i2c_name) {
        case I2C_0:
            CG_SetFcPeriphB(CG_FC_PERIPH_I2C0, ENABLE);
            CG_SetFcPeriphA(CG_FC_PERIPH_PORTK, ENABLE);
            obj->i2c = TSB_I2C0;
            obj->index = 0;
            obj->IRQn = INTI2C0_IRQn;
            break;
        case I2C_1:
            CG_SetFcPeriphB(CG_FC_PERIPH_I2C1, ENABLE);
            CG_SetFcPeriphA(CG_FC_PERIPH_PORTF, ENABLE);
            obj->i2c = TSB_I2C1;
            obj->index = 1;
            obj->IRQn = INTI2C1_IRQn;
            break;
        case I2C_2:
            CG_SetFcPeriphB(CG_FC_PERIPH_I2C2, ENABLE);
            CG_SetFcPeriphA(CG_FC_PERIPH_PORTH, ENABLE);
            obj->i2c = TSB_I2C2;
            obj->index = 2;
            obj->IRQn = INTI2C2_IRQn;
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

    i2c_reset(obj);
    i2c_frequency(obj, 100000);
}

// Configure the I2C frequency
void i2c_frequency(i2c_t *obj, int hz)
{
    uint32_t sck = 0;
    uint32_t tmp_sck = 0;
    uint32_t prsck = 1;
    uint32_t tmp_prsck = 1;
    uint32_t fscl = 0;
    uint32_t tmp_fscl = 0;
    uint64_t fx;

    if (hz <= 400000) { // Maximum 400khz clock frequency supported by  M46B
        for (prsck = 1; prsck <= 32; prsck++) {
            fx = ((uint64_t)SystemCoreClock / prsck);
            if ((fx < 20000000U) && (fx > 6666666U)) {
                for (sck = 0; sck <= 7; sck++) {
                    fscl = (fx / (uint64_t)I2C_SCK_DIVIDER_TBL[sck]);
                    if ((fscl <= (uint64_t)hz) && (fscl > tmp_fscl)) {
                        tmp_fscl = fscl;
                        tmp_sck = sck;
                        tmp_prsck = (prsck < 32)? prsck: 1;
                    }
                }
            }
        }
        clk.sck = (uint32_t)tmp_sck;
        clk.prsck = (tmp_prsck < 32)? (uint32_t)tmp_prsck - 1 : 1;
    } else {
        clk.sck = I2C_SCK_CLK_DIV_24;
        clk.prsck = I2C_PRESCALER_DIV_4;
    }
    myi2c.I2CSelfAddr = 0xE0; // Self Address
    myi2c.I2CDataLen = I2C_DATA_LEN_8;
    myi2c.I2CACKState = ENABLE;
    myi2c.I2CClkDiv = clk.sck;
    myi2c.PrescalerClkDiv = clk.prsck;

    I2C_SWReset(obj->i2c);
    I2C_Init(obj->i2c, &myi2c);
    NVIC_EnableIRQ(obj->IRQn);
    I2C_SetINTReq(obj->i2c, ENABLE);
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

static void wait_i2c_bus_free(i2c_t *obj)
{
    I2C_State status;

    do {
        status = I2C_GetState(obj->i2c);
    } while (status.Bit.BusState);
}

int i2c_read(i2c_t *obj, int address, char *data, int length, int stop)
{
    TSB_I2C_TypeDef *sbi = obj->i2c;
    uint32_t i2c_num = 0;
    obj->address = address;

    i2c_num = obj->index;

    // receive data
    xfer[i2c_num].count = 0;
    xfer[i2c_num].len = length;
    xfer[i2c_num].buf = data;

    I2C_SetSendData(sbi, address | SBI_I2C_RECEIVE);
    I2C_GenerateStart(sbi);

    wait_i2c_bus_free(obj);
    return (xfer[i2c_num].count - 1);
}

int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop)
{
    int8_t i = 0;
    TSB_I2C_TypeDef *sbi = obj->i2c;
    uint32_t i2c_num = 0;
    obj->address = address;

    i2c_num = obj->index;
    gI2C_TxData = (char *)calloc(length, sizeof(int8_t));

    for (i = 0; i < length; i++) {
        gI2C_TxData[i] = data[i];
    }
    // receive data
    xfer[i2c_num].count = 0;
    xfer[i2c_num].len = length;
    xfer[i2c_num].buf = gI2C_TxData;

    I2C_SetSendData(sbi, address | SBI_I2C_SEND);
    I2C_GenerateStart(sbi);  // Start condition

    wait_i2c_bus_free(obj);
    free(gI2C_TxData);
    DelayMS(8);
    if (((xfer[i2c_num].count - 1) == 0) && (byte_func == 1)) {
        send_byte = 1;
        i2c_byte_write(obj, 0x00);
        xfer[i2c_num].count = 1;
        byte_func = 0;
    }
    return (xfer[i2c_num].count - 1);
}

int i2c_byte_read(i2c_t *obj, int last)
{
    char i2c_ret = 0;
    i2c_read(obj, obj->address, &i2c_ret, 1, last);
    return i2c_ret;
}

int i2c_byte_write(i2c_t *obj, int data)
{
    uint32_t wb = 1;
    static size_t counter = 1;

    byte_func = 1;
    if (start_flag == 0 && send_byte == 0) {
        gI2C_LTxData = (char *)realloc(gI2C_LTxData, counter++);
        gI2C_LTxData[counter - 2] = data;
    }

    if (send_byte == 1) {
        wb = i2c_write(obj, obj->address, gI2C_LTxData, (counter - 1), 0);
        start_flag = 1;
        send_byte = 0;
        byte_func = 0;
        counter = 1;
        return wb;
    } else {
        if (start_flag == 1) {
            obj->address = data;
            start_flag = 0;
        } else {
            // Store the number of written bytes
            wb = i2c_write(obj, obj->address, (char*)&data, 1, 0);
        }
        if (wb == 1)
            return 1;
        else
            return 0;
    }
}

static void i2c_irq_handler(int i2c_num)
{
    uint32_t tmp = 0U;
    TSB_I2C_TypeDef *sbi = i2c_lut[i2c_num];
    I2C_State sbi_sr;

    sbi_sr = I2C_GetState(sbi);

    // we don't support slave mode
    if (!sbi_sr.Bit.MasterSlave)
        return;

    if (sbi_sr.Bit.TRx) { // Tx mode
        if (sbi_sr.Bit.LastRxBit) { // LRB=1: the receiver requires no further data.
            I2C_GenerateStop(sbi);
        } else { // LRB=0: the receiver requires further data.
            if (xfer[i2c_num].count < xfer[i2c_num].len) {
                I2C_SetSendData(sbi, xfer[i2c_num].buf[xfer[i2c_num].count]); // Send next data
            } else if (xfer[i2c_num].count == xfer[i2c_num].len) { // I2C data send finished.
                I2C_GenerateStop(sbi);
            } else {
                // Do nothing
            }
            xfer[i2c_num].count++;
        }
    } else { // Rx Mode
        if (xfer[i2c_num].count > xfer[i2c_num].len) {
            I2C_GenerateStop(sbi);
            I2C_SetACK(sbi, ENABLE);
        } else {
            if (xfer[i2c_num].count == xfer[i2c_num].len) { // Rx last data
                I2C_SetBitNum(sbi, I2C_DATA_LEN_1);
            } else if (xfer[i2c_num].count == (xfer[i2c_num].len - 1)) { // Rx the data second to last
                // Not generate ACK for next data Rx end.
                I2C_SetACK(sbi, DISABLE);
            } else {
                // Do nothing
            }
            tmp = I2C_GetReceiveData(sbi);
            if (xfer[i2c_num].count > 0) {
                xfer[i2c_num].buf[xfer[i2c_num].count - 1U] = tmp;
            } else {
                // first read is dummy read
            }
            xfer[i2c_num].count++;
        }
    }
}

void INTI2C0_IRQHandler(void)
{
    i2c_irq_handler(0);
}

void INTI2C1_IRQHandler(void)
{
    i2c_irq_handler(1);
}

void INTI2C2_IRQHandler(void)
{
    i2c_irq_handler(2);
}
