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

#if DEVICE_I2C

#include "mbed_error.h"
#include "PeripheralNames.h"
#include "pinmap.h"
#include "tmpm46b_i2c.h"
#include "mbed_wait_api.h"
#include <string.h>
#include <stdlib.h>

#define SBI_I2C_SEND                                0x00
#define SBI_I2C_RECEIVE                             0x01
#define I2C_TRANSFER_STATE_IDLE                     (0x0U)
#define I2C_TRANSFER_STATE_BUSY                     (0x1U)
#define I2C_ACK                                     (1)
#define I2C_NO_DATA                                 (0)
#define I2C_READ_ADDRESSED                          (1)
#define I2C_WRITE_ADDRESSED                         (3)
#define I2C_TIMEOUT                                 (100000)
#define I2CCR2_REPEATED_START_CONDITION             ((uint32_t)0x00000018)

#if DEVICE_I2C_ASYNCH
#define I2C_S(obj) (struct i2c_s *) (&(obj->i2c))
#else
#define I2C_S(obj) (struct i2c_s *) (obj)
#endif

static inline void repeated_start(struct i2c_s *obj_s);
static int32_t wait_status(i2c_t *obj);
static void I2C_Start_Condition(struct i2c_s *p_obj, uint32_t data);

#if DEVICE_I2C_ASYNCH
static inline void state_idle(struct i2c_s *obj_s);
#endif

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

// Clock setting structure definition
typedef struct {
    uint32_t sck;
    uint32_t prsck;
} I2C_clock_setting_t;

static const uint32_t I2C_SCK_DIVIDER_TBL[8] = {
    20, 24, 32, 48, 80, 144, 272, 528
};  // SCK Divider value table

static int32_t start_flag = 0;
static I2C_State status;
static I2C_clock_setting_t clk;
static I2C_InitTypeDef myi2c;

// Initialize the I2C peripheral. It sets the default parameters for I2C
void i2c_init(i2c_t *obj, PinName sda, PinName scl)
{
    struct i2c_s *obj_s = I2C_S(obj);
    MBED_ASSERT(obj != NULL);
    I2CName i2c_sda = (I2CName)pinmap_peripheral(sda, PinMap_I2C_SDA);
    I2CName i2c_scl = (I2CName)pinmap_peripheral(scl, PinMap_I2C_SCL);
    I2CName i2c_name = (I2CName)pinmap_merge(i2c_sda, i2c_scl);
    MBED_ASSERT((int)i2c_name != NC);

    switch (i2c_name) {
        case I2C_0:
            CG_SetFcPeriphB(CG_FC_PERIPH_I2C0, ENABLE);
            CG_SetFcPeriphA(CG_FC_PERIPH_PORTK, ENABLE);
            obj_s->i2c = TSB_I2C0;
            obj_s->index = 0;
            obj_s->IRQn = INTI2C0_IRQn;
            break;
        case I2C_1:
            CG_SetFcPeriphB(CG_FC_PERIPH_I2C1, ENABLE);
            CG_SetFcPeriphA(CG_FC_PERIPH_PORTF, ENABLE);
            obj_s->i2c = TSB_I2C1;
            obj_s->index = 1;
            obj_s->IRQn = INTI2C1_IRQn;
            break;
        case I2C_2:
            CG_SetFcPeriphB(CG_FC_PERIPH_I2C2, ENABLE);
            CG_SetFcPeriphA(CG_FC_PERIPH_PORTH, ENABLE);
            obj_s->i2c = TSB_I2C2;
            obj_s->index = 2;
            obj_s->IRQn = INTI2C2_IRQn;
            break;
        default:
            error("I2C is not available");
            break;
    }

#if DEVICE_I2C_ASYNCH
    obj_s->state = I2C_TRANSFER_STATE_IDLE;
#endif
    pinmap_pinout(sda, PinMap_I2C_SDA);
    pin_mode(sda, OpenDrain);
    pin_mode(sda, PullUp);

    pinmap_pinout(scl, PinMap_I2C_SCL);
    pin_mode(scl, OpenDrain);
    pin_mode(scl, PullUp);

    NVIC_DisableIRQ(obj_s->IRQn);
    i2c_reset(obj);
    i2c_frequency(obj, 100000);
}

// Configure the I2C frequency
void i2c_frequency(i2c_t *obj, int hz)
{
    struct i2c_s *obj_s = I2C_S(obj);
    uint32_t sck        = 0;
    uint32_t tmp_sck    = 0;
    uint32_t prsck      = 1;
    uint32_t tmp_prsck  = 1;
    uint32_t fscl       = 0;
    uint32_t tmp_fscl   = 0;
    uint64_t fx         = 0;

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

    I2C_SWReset(obj_s->i2c);
    I2C_Init(obj_s->i2c, &myi2c);
    NVIC_DisableIRQ(obj_s->IRQn);
}

int i2c_start(i2c_t *obj)
{
    start_flag = 1;
    return 0;
}

int i2c_stop(i2c_t *obj)
{
    struct i2c_s *obj_s = I2C_S(obj);
    I2C_GenerateStop(obj_s->i2c);
    return 0;
}

void i2c_reset(i2c_t *obj)
{
    struct i2c_s *obj_s = I2C_S(obj);
    I2C_SWReset(obj_s->i2c);
}

int i2c_read(i2c_t *obj, int address, char *data, int length, int stop)
{
    int32_t result = 0;
    int32_t count  = 0;

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

int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop)
{
    int32_t result = 0;
    int32_t count  = 0;

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
    struct i2c_s *obj_s = I2C_S(obj);

    int32_t result;

    I2C_ClearINTOutput(obj_s->i2c);

    if (last) {
        I2C_SetACK(obj_s->i2c, DISABLE);
    } else {
        I2C_SetACK(obj_s->i2c, ENABLE);
    }
    I2C_SetSendData(obj_s->i2c, 0x00);

    if (wait_status(obj) < 0) {
        result = -1;
    } else {
        result = (int32_t)I2C_GetReceiveData(obj_s->i2c);
    }
    return (result);
}

int i2c_byte_write(i2c_t *obj, int data)
{
    struct i2c_s *obj_s = I2C_S(obj);

    int32_t result;

    I2C_ClearINTOutput(obj_s->i2c);

    if (start_flag == 1) {
        I2C_Start_Condition(obj_s, (uint32_t)data);
        start_flag = 0;
    } else {
        I2C_SetSendData(obj_s->i2c, (uint32_t)data);
    }

    if (wait_status(obj) < 0) {
        return (-1);
    }

    status = I2C_GetState(obj_s->i2c);
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
    struct i2c_s *obj_s = I2C_S(obj);

    obj_s->myi2c.I2CDataLen = I2C_DATA_LEN_8;
    obj_s->myi2c.I2CACKState = ENABLE;
    obj_s->myi2c.I2CClkDiv = clk.sck;
    obj_s->myi2c.PrescalerClkDiv = clk.prsck;

    if (enable_slave) {
        obj_s->myi2c.I2CSelfAddr = obj_s->address;
        I2C_SetINTReq(obj_s->i2c, ENABLE);
    } else {
        obj_s->myi2c.I2CSelfAddr = 0xE0;
        NVIC_DisableIRQ(obj_s->IRQn);
        I2C_ClearINTOutput(obj_s->i2c);
    }
    I2C_Init(obj_s->i2c, &obj_s->myi2c);
}

int i2c_slave_receive(i2c_t *obj)
{
    struct i2c_s *obj_s = I2C_S(obj);
    int32_t result = I2C_NO_DATA;

    if  ((I2C_GetINTStatus(obj_s->i2c)) && (I2C_GetSlaveAddrMatchState(obj_s->i2c))) {
        status = I2C_GetState(obj_s->i2c);
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
    struct i2c_s *obj_s = I2C_S(obj);
    int32_t count = 0;

    while (count < length) {
        int32_t pdata = i2c_byte_read(obj, 0);
        status = I2C_GetState(obj_s->i2c);
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
    struct i2c_s *obj_s = I2C_S(obj);
    obj_s->address = address & 0xFE;
    i2c_slave_mode(obj, 1);
}


#if DEVICE_I2C_ASYNCH
void i2c_transfer_asynch(i2c_t *obj, const void *tx, size_t tx_length, void *rx, size_t rx_length, uint32_t address,
                         uint32_t stop, uint32_t handler, uint32_t event, DMAUsage hint)
{
    struct i2c_s *obj_s = I2C_S(obj);
    obj_s->event_mask = event;
    obj_s->stop = stop;
    obj_s->address = address;

    // copy the buffers to the I2C object
    obj->tx_buff.buffer = (void *) tx;
    obj->tx_buff.length = tx ? tx_length : 0;
    obj->tx_buff.pos = 0;

    obj->rx_buff.buffer = rx;
    obj->rx_buff.length = rx ? rx_length : 0;
    obj->rx_buff.pos = 0;

    obj_s->state = I2C_TRANSFER_STATE_BUSY;
    I2C_SetINTReq(obj_s->i2c, ENABLE);
    NVIC_ClearPendingIRQ(obj_s->IRQn);
    NVIC_SetVector(obj_s->IRQn, (uint32_t)handler);
    NVIC_EnableIRQ(obj_s->IRQn);

    if ((tx_length == 0) && (rx_length != 0)) {
        I2C_SetSendData(obj_s->i2c, address | SBI_I2C_RECEIVE);
    } else {
        I2C_SetSendData(obj_s->i2c, address | SBI_I2C_SEND);
    }
    I2C_GenerateStart(obj_s->i2c);
}

uint32_t i2c_irq_handler_asynch(i2c_t *obj)
{
    struct i2c_s *obj_s = I2C_S(obj);
    uint32_t tmp_read = 0U;
    uint32_t event = 0;
    I2C_State flag_state;

    flag_state = I2C_GetState(obj_s->i2c);

    if ((!flag_state.Bit.MasterSlave) || (obj_s->state != I2C_TRANSFER_STATE_BUSY)) {
        I2C_GenerateStop(obj_s->i2c);
        event = I2C_EVENT_ERROR;
        state_idle(obj_s);
        return (event & obj_s->event_mask);
    }

    if (flag_state.Bit.TRx) { // Transmit

        if (flag_state.Bit.LastRxBit) { // NACK Recieved
            I2C_GenerateStop(obj_s->i2c);

            if (obj->tx_buff.pos == 0) {
                I2C_GenerateStop(obj_s->i2c);
                event = (I2C_EVENT_ERROR | I2C_EVENT_ERROR_NO_SLAVE);
                state_idle(obj_s);

            } else {
                I2C_GenerateStop(obj_s->i2c);
                event = (I2C_EVENT_ERROR | I2C_EVENT_TRANSFER_EARLY_NACK);
                state_idle(obj_s);
            }

        } else { // ACK Received

            if (obj->tx_buff.pos < obj->tx_buff.length) {
                I2C_SetSendData(obj_s->i2c, (*((uint8_t *)obj->tx_buff.buffer + obj->tx_buff.pos) & 0xFF)); // Send next data
                obj->tx_buff.pos++;

            } else if (obj->rx_buff.length != 0) { // Transmit complete Receive Pending
                repeated_start(obj_s);
                I2C_SetSendData(obj_s->i2c, obj_s->address | SBI_I2C_RECEIVE);

            } else { // Transmit complete and NO data to Receive
                I2C_GenerateStop(obj_s->i2c);
                event = I2C_EVENT_TRANSFER_COMPLETE;
                state_idle(obj_s);
            }
        }
    } else { // Receive

        if (obj->rx_buff.pos > obj->rx_buff.length) {
            I2C_GenerateStop(obj_s->i2c);
            event = I2C_EVENT_TRANSFER_COMPLETE;
            state_idle(obj_s);
            I2C_SetACK(obj_s->i2c, ENABLE);

        } else {

            if (obj->rx_buff.pos == obj->rx_buff.length) {
                I2C_SetBitNum(obj_s->i2c, I2C_DATA_LEN_1);

            } else if (obj->rx_buff.pos == (obj->rx_buff.length - 1)) {
                I2C_SetACK(obj_s->i2c, DISABLE);

            } else {
                // Do nothing
            }

            tmp_read = I2C_GetReceiveData(obj_s->i2c);

            if (obj->rx_buff.pos > 0) {
                *((uint8_t *)obj->rx_buff.buffer + (obj->rx_buff.pos - 1)) = tmp_read;
            } else {
                // first read is dummy read
            }
            obj->rx_buff.pos++;
        }
    }

    return (event & obj_s->event_mask);
}

uint8_t i2c_active(i2c_t *obj)
{
    struct i2c_s *obj_s = I2C_S(obj);

    return (obj_s->state != I2C_TRANSFER_STATE_IDLE);
}

void i2c_abort_asynch(i2c_t *obj)
{
    struct i2c_s *obj_s = I2C_S(obj);

    I2C_ClearINTReq(obj_s->i2c);
    NVIC_ClearPendingIRQ(obj_s->IRQn);
    I2C_GenerateStop(obj_s->i2c);
    state_idle(obj_s);
    I2C_SWReset(obj_s->i2c);
    I2C_Init(obj_s->i2c, &obj_s->myi2c);
}

static inline void state_idle(struct i2c_s *obj_s)
{
    I2C_State flag_state;

    obj_s->state = I2C_TRANSFER_STATE_IDLE;
    NVIC_DisableIRQ(obj_s->IRQn);
    I2C_SetINTReq(obj_s->i2c, DISABLE);
    // wait until bus state releases after stop condition
    do {
        flag_state = I2C_GetState(obj_s->i2c);
    } while (flag_state.Bit.BusState);
    // To satisfy the setup time of restart, at least 4.7µs wait must be created by software (Ref. TRM pg. 561)
    wait_us(5);
}

#endif //DEVICE_I2C_ASYNCH

static int32_t wait_status(i2c_t *obj)
{
    struct i2c_s *obj_s = I2C_S(obj);
    volatile int32_t timeout = I2C_TIMEOUT;

    while (I2C_GetINTStatus(obj_s->i2c) == DISABLE) {
        if ((timeout--) == 0) {
            return (-1);
        }
    }
    return (0);
}

static inline void repeated_start(struct i2c_s *obj_s)
{
    I2C_State flag_state;

    obj_s->i2c->CR2 = I2CCR2_REPEATED_START_CONDITION;
    // wait until bus state releases
    do {
        flag_state = I2C_GetState(obj_s->i2c);
    } while (flag_state.Bit.BusState);
    // Checks that no other device is pulling the SCL pin to "Low".
    do {
        flag_state = I2C_GetState(obj_s->i2c);
    } while (!flag_state.Bit.LastRxBit);

    I2C_GenerateStart(obj_s->i2c);
}

static void I2C_Start_Condition(struct i2c_s *p_obj, uint32_t data)
{
    status = I2C_GetState(p_obj->i2c);
    if (status.Bit.BusState) {
        repeated_start(p_obj);
        I2C_SetSendData(p_obj->i2c, (uint32_t)data);
    } else {
        I2C_SetSendData(p_obj->i2c, (uint32_t)data);
        I2C_GenerateStart(p_obj->i2c);
    }
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

#endif  // #if DEVICE_I2C
