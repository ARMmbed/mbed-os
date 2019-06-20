/* mbed Microcontroller Library
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2018 All rights reserved
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
#include "pinmap.h"
#include "gpio_include.h"

#if DEVICE_I2C_ASYNCH
#define I2C_S(obj) (struct i2c_s *) (&((obj)->i2c))
#else
#define I2C_S(obj) (struct i2c_s *) (obj)
#endif

static const PinMap PinMap_I2C_SDA[] = {
    {PC1, I2C_0, PIN_DATA(1, 2)},
    {PA5, I2C_1, PIN_DATA(1, 2)},
    {PL1, I2C_2, PIN_DATA(3, 2)},
    {NC,  NC,    0}
};

static const PinMap PinMap_I2C_SCL[] = {
    {PC0, I2C_0, PIN_DATA(1, 2)},
    {PA4, I2C_1, PIN_DATA(1, 2)},
    {PL0, I2C_2, PIN_DATA(3, 2)},
    {NC,  NC,    0}
};

// Clock setting structure definition
typedef struct {
    uint32_t sck;
    uint32_t prsck;
} I2C_clock_setting_t;

// SCK Divider value table
static const uint32_t I2C_SCK_DIVIDER_TBL[8] = {
    20, 24, 32, 48, 80, 144, 272, 528
};

I2C_clock_setting_t clk;
static uint32_t start_flag = 0;

#if DEVICE_I2C_ASYNCH
enum {
    I2C_TRANSFER_STATE_IDLE = 0U,
    I2C_TRANSFER_STATE_START,
    I2C_TRANSFER_STATE_WRITE,
    I2C_TRANSFER_STATE_RESTART,
    I2C_TRANSFER_STATE_READ,
    I2C_TRANSFER_STATE_MAX
} TransferState;

typedef struct {
    IRQn_Type i2c;
} i2c_irq_t;

static const i2c_irq_t I2C_CH0_IRQN_TBL[1] = {
    {INTI2C0_IRQn}
};

static const i2c_irq_t I2C_CH1_IRQN_TBL[1] = {
    {INTI2C1_IRQn}
};

static const i2c_irq_t I2C_CH2_IRQN_TBL[1] = {
    {INTI2C2_IRQn}
};
#endif

static int32_t wait_status(i2c_t *p_obj);
static void i2c_start_bit(i2c_t *obj);

#if DEVICE_I2C_ASYNCH
static void disable_irq(uint32_t irqn);
static void clear_irq(uint32_t irqn);
static void i2c_irq_handler(i2c_t *obj);
#endif

// Initialize the I2C peripheral. It sets the default parameters for I2C
void i2c_init(i2c_t *obj, PinName sda, PinName scl)
{
    struct i2c_s *obj_s = I2C_S(obj);
    MBED_ASSERT(obj_s != NULL);
    I2CName i2c_sda  = (I2CName)pinmap_peripheral(sda, PinMap_I2C_SDA);
    I2CName i2c_scl  = (I2CName)pinmap_peripheral(scl, PinMap_I2C_SCL);
    I2CName i2c_name = (I2CName)pinmap_merge(i2c_sda, i2c_scl);
    MBED_ASSERT((int)i2c_name != NC);

    switch (i2c_name) {
        case I2C_0:
            TSB_CG_FSYSENA_IPENA20 = ENABLE;
            TSB_CG_FSYSENA_IPENA02 = ENABLE;
            obj_s->i2c = TSB_I2C0;
#if DEVICE_I2C_ASYNCH
            obj_s->irqn = (uint32_t)&I2C_CH0_IRQN_TBL;
#endif
            break;
        case I2C_1:
            TSB_CG_FSYSENA_IPENA21 = ENABLE;
            TSB_CG_FSYSENA_IPENA00 = ENABLE;
            obj_s->i2c = TSB_I2C1;
#if DEVICE_I2C_ASYNCH
            obj_s->irqn = (uint32_t)&I2C_CH1_IRQN_TBL;
#endif
            break;
        case I2C_2:
            TSB_CG_FSYSENA_IPENA22 = ENABLE;
            TSB_CG_FSYSENA_IPENA10 = ENABLE;
            obj_s->i2c = TSB_I2C2;
#if DEVICE_I2C_ASYNCH
            obj_s->irqn = (uint32_t)&I2C_CH2_IRQN_TBL;
#endif
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

    i2c_reset(obj);
    i2c_frequency(obj, 100000);
    obj_s->i2c->CR2 = (I2CxCR2_I2CM_ENABLE | I2CxCR2_TRX | I2CxCR2_PIN_CLEAR |
                       I2CxCR2_INIT);
    obj_s->i2c->OP  = I2CxOP_INIT;
    obj_s->i2c->IE  = I2CxIE_CLEAR;
}

// Configure the I2C frequency
void i2c_frequency(i2c_t *obj, int hz)
{
    struct i2c_s *obj_s = I2C_S(obj);
    uint64_t sck;
    uint64_t tmp_sck;
    uint64_t prsck;
    uint64_t tmp_prsck;
    uint64_t fscl;
    uint64_t tmp_fscl;
    uint64_t fx;

    SystemCoreClockUpdate();

    if (hz <= 1000000) {
        sck = tmp_sck = 0;
        prsck = tmp_prsck = 1;
        fscl = tmp_fscl = 0;
        for (prsck = 1; prsck <= 32; prsck++) {
            fx = ((uint64_t)SystemCoreClock / prsck);
            if ((fx < 20000000U) && (fx > 6666666U)) {
                for (sck = 0; sck <= 7; sck++) {
                    fscl = (fx / (uint64_t)I2C_SCK_DIVIDER_TBL[sck]);
                    if ((fscl <= (uint64_t)hz) && (fscl > tmp_fscl)) {
                        tmp_fscl = fscl;
                        tmp_sck = sck;
                        tmp_prsck = (prsck < 32) ? prsck : 0;
                    }
                }
            }
        }
        clk.sck = (uint32_t)tmp_sck;
        clk.prsck = (tmp_prsck < 32) ? (uint32_t)(tmp_prsck - 1) : 0;
    }

    obj_s->i2c->CR1 = (I2CxCR1_ACK | clk.sck);
    obj_s->i2c->PRS = (I2CxPRS_PRCK & clk.prsck);
}

int i2c_start(i2c_t *obj)
{
    start_flag = 1;  // Start Condition
    return 0;
}

int i2c_stop(i2c_t *obj)
{
    struct i2c_s *obj_s = I2C_S(obj);
    uint32_t timeout = I2C_TIMEOUT;

    obj_s->i2c->CR2 = I2CxCR2_STOP_CONDITION;

    while ((obj_s->i2c->SR & I2CxSR_BB) == I2CxSR_BB) {
        if (timeout == 0) {
            break;
        }
        timeout--;
    }

    return 0;
}

void i2c_reset(i2c_t *obj)
{
    struct i2c_s *obj_s = I2C_S(obj);
    obj_s->i2c->CR2 = I2CxCR2_SWRES_10;
    obj_s->i2c->CR2 = I2CxCR2_SWRES_01;
}

int i2c_read(i2c_t *obj, int address, char *data, int length, int stop)
{
    int32_t result = 0;
    int32_t count  = 0;
    int32_t pdata  = 0;

    if (length > 0) {
        start_flag = 1;  // Start Condition
        if (i2c_byte_write(obj, (int32_t)((uint32_t)address | 1U)) == I2C_ACK) {
            while (count < length) {
                pdata = i2c_byte_read(obj, ((count < (length - 1)) ? 0 : 1));
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

    return result;
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

    return result;
}

int i2c_byte_read(i2c_t *obj, int last)
{
    struct i2c_s *obj_s = I2C_S(obj);
    int32_t result = 0;

    obj_s->i2c->ST = I2CxST_CLEAR;

    if (last) {
        obj_s->i2c->OP |= I2CxOP_MFACK;
    } else {
        obj_s->i2c->OP &= ~I2CxOP_MFACK;
    }

    obj_s->i2c->DBR = (0 & I2CxDBR_DB_MASK);

    if (wait_status(obj) < 0) {
        result = -1;
    } else {
        result = (int32_t)(obj_s->i2c->DBR & I2CxDBR_DB_MASK);
    }

    return result;
}

int i2c_byte_write(i2c_t *obj, int data)
{
    struct i2c_s *obj_s = I2C_S(obj);
    int32_t result = 0;

    obj_s->i2c->ST = I2CxST_CLEAR;
    if (start_flag == 1) {
        obj_s->i2c->DBR = (data & I2CxDBR_DB_MASK);
        i2c_start_bit(obj);
        start_flag = 0;
    } else {
        obj_s->i2c->DBR = (data & I2CxDBR_DB_MASK);
    }

    if (wait_status(obj) < 0) {
        return -1;
    }

    if (!((obj_s->i2c->SR & I2CxSR_LRB) == I2CxSR_LRB)) {
        result = 1;
    } else {
        result = 0;
    }

    return result;
}

static void i2c_start_bit(i2c_t *obj)  // Send START command
{
    struct i2c_s *obj_s = I2C_S(obj);
    uint32_t opreg = 0;

    opreg = obj_s->i2c->OP;
    opreg &= ~(I2CxOP_RSTA | I2CxOP_SREN);

    if ((obj_s->i2c->SR & I2CxSR_BB)) {
        opreg |= I2CxOP_SREN;
    }

    obj_s->i2c->OP = opreg;
    obj_s->i2c->CR2 |= I2CxCR2_START_CONDITION;
}

static int32_t wait_status(i2c_t *p_obj)
{
    struct i2c_s *obj_s = I2C_S(p_obj);
    volatile int32_t timeout;
    timeout = I2C_TIMEOUT;

    while (!((obj_s->i2c->ST & I2CxST_I2C) == I2CxST_I2C)) {
        if ((timeout--) == 0) {
            return (-1);
        }
    }

    return 0;
}

void i2c_slave_mode(i2c_t *obj, int enable_slave)
{
    struct i2c_s *obj_s = I2C_S(obj);
    if (enable_slave) {
        i2c_reset(obj);
        obj_s->i2c->OP  = I2CxOP_SLAVE_INIT;
        obj_s->i2c->CR1 = (I2CxCR1_ACK | clk.sck);
        obj_s->i2c->CR2 = (I2CxCR2_INIT | I2CxCR2_PIN_CLEAR);
        obj_s->i2c->CR2 = I2CxCR2_INIT;
        obj_s->i2c->PRS = (I2CxPRS_PRCK & clk.prsck);
        obj_s->i2c->AR  = (obj_s->address & I2CAR_SA_MASK);
        obj_s->i2c->IE  = I2CxIE_INTI2C;
    } else {
        i2c_reset(obj);
        obj_s->i2c->CR2 = (I2CxCR2_I2CM_ENABLE | I2CxCR2_TRX | I2CxCR2_PIN_CLEAR |
                           I2CxCR2_INIT);
        obj_s->i2c->OP  = I2CxOP_INIT;
        obj_s->i2c->CR1 = (I2CxCR1_ACK | clk.sck);
        obj_s->i2c->PRS = (I2CxPRS_PRCK & clk.prsck);
        obj_s->i2c->ST = I2CxST_CLEAR;
    }
}

int i2c_slave_receive(i2c_t *obj)
{
    struct i2c_s *obj_s = I2C_S(obj);
    int32_t result = I2C_NO_DATA;

    if ((obj_s->i2c->ST & I2CxST_I2C) && (obj_s->i2c->OP & I2CxOP_SAST)) {
        // Detect and clear arbitration lost.
        if(!(obj_s->i2c->SR & 0x08)) {
            obj_s->i2c->DBR = 0x00;
        }
        if ((obj_s->i2c->SR & I2CxSR_TRX) == I2CxSR_TRX) {
            result = I2C_READ_ADDRESSED;
        } else {
            result = I2C_WRITE_ADDRESSED;
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
        if ((obj_s->i2c->SR & I2CxSR_TRX)) {
            return (count);
        } else {
            if (pdata < 0) {
                break;
            }
            data[count++] = (uint8_t)pdata;
        }
    }

    i2c_slave_mode(obj,1);

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

    i2c_slave_mode(obj,1);

    return (count);
}

void i2c_slave_address(i2c_t *obj, int idx, uint32_t address, uint32_t mask)
{
    struct i2c_s *obj_s = I2C_S(obj);
    obj_s->address = address & I2CAR_SA_MASK;
    i2c_slave_mode(obj,1);
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

#if DEVICE_I2C_ASYNCH

void i2c_transfer_asynch(i2c_t *obj, const void *tx, size_t tx_length, void *rx, size_t rx_length, uint32_t address,
                         uint32_t stop, uint32_t handler, uint32_t event, DMAUsage hint)
{
    struct i2c_s *obj_s = I2C_S(obj);
    i2c_irq_t *p_irqn = (i2c_irq_t *)obj_s->irqn;

    if(obj_s->state == I2C_TRANSFER_STATE_IDLE) {
        // Disable and clear interrupt flag.
        disable_irq(obj_s->irqn);
        obj_s->i2c->IE = I2CxIE_CLEAR;
        obj_s->i2c->ST = I2CxST_CLEAR;
        clear_irq(obj_s->irqn);

        // Store given buffer data and lenght into I2C object and set state as I2C_TRANSFER_STATE_START.
        obj_s->address = address;
        obj_s->event = 0;
        obj_s->stop  = stop;
        obj->tx_buff.buffer = (void *)tx;
        obj->tx_buff.length = tx_length;
        obj->tx_buff.pos    = 0;
        obj->rx_buff.buffer = rx;
        obj->rx_buff.length = rx_length;
        obj->rx_buff.pos    = 0;
        obj_s->state         = I2C_TRANSFER_STATE_START;

        // Enable I2C interrupt.
        obj_s->i2c->IE       = I2CxIE_INTI2C;

        if ((tx_length == 0) && (rx_length != 0)) {
            i2c_start_bit(obj);
            obj_s->i2c->DBR = ((address | 1U) & I2CxDBR_DB_MASK);
        } else {
            i2c_start_bit(obj);
            obj_s->i2c->DBR = (address & I2CxDBR_DB_MASK);
        }

        // Enable I2C interrupr in NVIC.
        NVIC_EnableIRQ(p_irqn->i2c);
        NVIC_SetVector(p_irqn->i2c, handler);
    }
}

uint32_t i2c_irq_handler_asynch(i2c_t *obj)
{
    struct i2c_s *obj_s = I2C_S(obj);
    i2c_irq_handler(obj);
    return (obj_s->event & I2C_EVENT_ALL);
}

uint8_t i2c_active(i2c_t *obj)
{
    struct i2c_s *obj_s = I2C_S(obj);
    uint8_t ret_val = 0;

    if ((obj_s->i2c->CR2 & 0x08)) {
        ret_val = 1;
    }
    return ret_val;
}

void i2c_abort_asynch(i2c_t *obj)
{
    struct i2c_s *obj_s = I2C_S(obj);

    // Generate Stop condition on I2C bus
    i2c_stop(obj);

    // Set state as idle and disable I2C interrupt.
    obj_s->state = I2C_TRANSFER_STATE_IDLE;
    disable_irq(obj_s->irqn);
    clear_irq(obj_s->irqn);
    obj_s->i2c->IE = I2CxIE_CLEAR;

    // Given I2C Software Reset
    i2c_reset(obj);

    // Re-Store the I2C configuration
    obj_s->i2c->CR2 = (I2CxCR2_I2CM_ENABLE | I2CxCR2_TRX | I2CxCR2_PIN_CLEAR | I2CxCR2_INIT);
    obj_s->i2c->OP  = I2CxOP_INIT;
    obj_s->i2c->CR1 = (I2CxCR1_ACK | clk.sck);
    obj_s->i2c->PRS = (I2CxPRS_PRCK & clk.prsck);
    obj_s->i2c->ST  = I2CxST_CLEAR;

}

static void disable_irq(uint32_t irqn)
{
    i2c_irq_t *p_irqn = (i2c_irq_t *)irqn;
    NVIC_DisableIRQ(p_irqn->i2c);
}

static void clear_irq(uint32_t irqn)
{
    i2c_irq_t *p_irqn = (i2c_irq_t *)irqn;
    NVIC_ClearPendingIRQ(p_irqn->i2c);
}

static void i2c_irq_handler(i2c_t *obj)
{
    struct i2c_s *obj_s = I2C_S(obj);
    obj_s->i2c->ST = I2CxST_CLEAR;

    switch(obj_s->state) {
        case I2C_TRANSFER_STATE_START:
            // Check ACK for sent slave address.
            if (!((obj_s->i2c->SR & I2CxSR_LRB) == I2CxSR_LRB)) {

                if(obj->tx_buff.length != (unsigned long)0) {    // Check Tx buff length.
                    obj_s->i2c->DBR = *((uint8_t *)obj->tx_buff.buffer)& I2CxDBR_DB_MASK;
                    obj->tx_buff.buffer = (uint8_t *)obj->tx_buff.buffer + sizeof(uint8_t);
                    obj->tx_buff.pos++;
                    obj_s->state = I2C_TRANSFER_STATE_WRITE;
                } else if(obj->rx_buff.length != 0) {           // Check Rx buff length.
                    if ((obj->rx_buff.pos < (obj->rx_buff.length - 1))) {
                        obj_s->i2c->OP &= ~I2CxOP_MFACK;
                    } else {
                        obj_s->i2c->OP |= I2CxOP_MFACK;
                    }
                    obj_s->i2c->DBR = 0x00;
                    obj_s->state = I2C_TRANSFER_STATE_READ;
                } else {    // Return transfer complete because of not given any Tx/Rx data.
                    obj_s->event = I2C_EVENT_TRANSFER_COMPLETE;
                    obj_s->state = I2C_TRANSFER_STATE_IDLE;
                }
            } else {    // Return "No Slave", Because of Did not get any ACK for sent slave address.
                obj_s->event = (I2C_EVENT_ERROR | I2C_EVENT_ERROR_NO_SLAVE);
                obj_s->state = I2C_TRANSFER_STATE_IDLE;
            }
            break;
        case I2C_TRANSFER_STATE_WRITE:
            if(obj->tx_buff.pos < obj->tx_buff.length) {
                if (!((obj_s->i2c->SR & I2CxSR_LRB) == I2CxSR_LRB)) {
                    obj_s->i2c->DBR = *((uint8_t *)obj->tx_buff.buffer)& I2CxDBR_DB_MASK;
                    obj->tx_buff.buffer = (uint8_t *)obj->tx_buff.buffer + sizeof(uint8_t);
                    obj->tx_buff.pos++;
                } else {
                    obj_s->event = (I2C_EVENT_ERROR | I2C_EVENT_TRANSFER_EARLY_NACK);
                    obj_s->state =  I2C_TRANSFER_STATE_IDLE;
                }
            } else if(obj->rx_buff.length != 0) {
                if (!((obj_s->i2c->SR & I2CxSR_LRB) == I2CxSR_LRB)) {
                    i2c_start_bit(obj);
                    obj_s->i2c->DBR = ((obj_s->address | 1U) & I2CxDBR_DB_MASK);
                    obj_s->state = I2C_TRANSFER_STATE_RESTART;
                } else {
                    obj_s->event = (I2C_EVENT_ERROR | I2C_EVENT_TRANSFER_EARLY_NACK);
                    obj_s->state =  I2C_TRANSFER_STATE_IDLE;
                }
            } else {
                if(obj_s->stop) {
                    obj_s->i2c->CR2 = I2CxCR2_STOP_CONDITION;
                }
                obj_s->event = I2C_EVENT_TRANSFER_COMPLETE;
                obj_s->state = I2C_TRANSFER_STATE_IDLE;
            }
            break;
        case I2C_TRANSFER_STATE_RESTART:
            if(!((obj_s->i2c->SR & I2CxSR_LRB) == I2CxSR_LRB)) {
                // Set ACK/NACK
                if ((obj->rx_buff.pos < (obj->rx_buff.length - 1))) {
                    obj_s->i2c->OP &= ~I2CxOP_MFACK;
                } else {
                    obj_s->i2c->OP |= I2CxOP_MFACK;
                }
                obj_s->i2c->DBR = 0x00;
                obj_s->state = I2C_TRANSFER_STATE_READ;
            } else {
                obj_s->event = (I2C_EVENT_ERROR | I2C_EVENT_TRANSFER_EARLY_NACK);
                obj_s->state = I2C_TRANSFER_STATE_IDLE;
            }
            break;
        case I2C_TRANSFER_STATE_READ:
            if(obj->rx_buff.pos < obj->rx_buff.length) {
                *((uint8_t *)obj->rx_buff.buffer) = (uint8_t)obj_s->i2c->DBR & I2CxDBR_DB_MASK;
                obj->rx_buff.buffer = (uint8_t *)obj->rx_buff.buffer + sizeof(uint8_t);
                obj->rx_buff.pos++;
            }
            if(obj->rx_buff.pos < obj->rx_buff.length) {
                // Set ACK/NACK
                if ((obj->rx_buff.pos < (obj->rx_buff.length - 1))) {
                    obj_s->i2c->OP &= ~I2CxOP_MFACK;
                } else {
                    obj_s->i2c->OP |= I2CxOP_MFACK;
                }
                obj_s->i2c->DBR = 0x00;
            } else {
                if(obj_s->stop) {
                    obj_s->i2c->CR2 = I2CxCR2_STOP_CONDITION;
                }
                obj_s->event = I2C_EVENT_TRANSFER_COMPLETE;
                obj_s->state = I2C_TRANSFER_STATE_IDLE;
            }
            break;
        default:
            break;
    }
    if(obj_s->state == I2C_TRANSFER_STATE_IDLE) {
        disable_irq(obj_s->irqn);
        obj_s->i2c->IE = I2CxIE_CLEAR;
    }
}

#endif  // #if DEVICE_I2C_ASYNCH

#endif  // #if DEVICE_I2C
