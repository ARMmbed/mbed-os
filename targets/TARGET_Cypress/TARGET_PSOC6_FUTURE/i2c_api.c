/*
 * mbed Microcontroller Library
 * Copyright (c) 2017-2018 Future Electronics
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
#include "mbed_error.h"
#include "PeripheralPins.h"
#include "pinmap.h"
#include "psoc6_utils.h"

#include "cy_sysclk.h"
#include "cy_gpio.h"
#include "cy_scb_i2c.h"
#include "cy_sysint.h"

#define I2C_DEFAULT_SPEED               100000
#define NUM_I2C_PORTS                   8
#define I2C_DEFAULT_IRQ_PRIORITY        3
#define I2C_NUM_DIVIDERS                3
#define MIN_I2C_CLOCK_FREQUENCY         CY_SCB_I2C_SLAVE_STD_CLK_MIN
// Default timeout in milliseconds.
#define I2C_DEFAULT_TIMEOUT             1000

#define PENDING_NONE                    0
#define PENDING_RX                      1
#define PENDING_TX                      2
#define PENDING_TX_RX                   3

typedef enum {
    I2C_DIVIDER_LOW = 0,
    I2C_DIVIDER_MID,
    I2C_DIVIDER_HIGH,
    I2C_INVALID_DIVIDER = 0xff
} I2cDividerType;


typedef struct {
    uint32_t                div_num;
    cy_en_divider_types_t   div_type;
    uint32_t                clk_frequency;
} I2cDividerInfo;


static const cy_stc_scb_i2c_config_t default_i2c_config = {
    .i2cMode                = CY_SCB_I2C_MASTER,
    .useRxFifo              = true,
    .useTxFifo              = true,
    .slaveAddress           = 0,
    .slaveAddressMask       = 0,
    .acceptAddrInFifo       = false,
    .ackGeneralAddr         = false,
    .enableWakeFromSleep    = false
};


static I2cDividerInfo i2c_dividers[I2C_NUM_DIVIDERS] = {
    { I2C_INVALID_DIVIDER, 0, CY_SCB_I2C_SLAVE_STD_CLK_MIN }, // Low divider uses lowest possible frequency.
    { I2C_INVALID_DIVIDER, 0, CY_SCB_I2C_SLAVE_FST_CLK_MIN },
    { I2C_INVALID_DIVIDER, 0, CY_SCB_I2C_SLAVE_FSTP_CLK_MIN }
};

typedef struct i2c_s i2c_obj_t;

#if DEVICE_I2C_ASYNCH
#define OBJ_P(in)     (&(in->i2c))
#else
#define OBJ_P(in)     (in)
#endif


#if DEVICE_I2C_ASYNCH

static IRQn_Type i2c_irq_allocate_channel(i2c_obj_t *obj)
{
#if defined (TARGET_MCU_PSOC6_M0)
    obj->cm0p_irq_src = scb_0_interrupt_IRQn + obj->i2c_id;
    return cy_m0_nvic_allocate_channel(CY_SERIAL_IRQN_ID + obj->i2c_id);
#else
    return (IRQn_Type)(ioss_interrupts_gpio_0_IRQn + obj->i2c_id);
#endif // M0
}

static void i2c_irq_release_channel(IRQn_Type channel, uint32_t i2c_id)
{
#if defined (TARGET_MCU_PSOC6_M0)
    cy_m0_nvic_release_channel(channel, CY_SERIAL_IRQN_ID + i2c_id);
#endif //M0
}

static int i2c_irq_setup_channel(i2c_obj_t *obj)
{
    cy_stc_sysint_t irq_config;

    if (obj->irqn == unconnected_IRQn) {
        IRQn_Type irqn = i2c_irq_allocate_channel(obj);
        if (irqn < 0) {
            return (-1);
        }
        // Configure NVIC
        irq_config.intrPriority = I2C_DEFAULT_IRQ_PRIORITY;
        irq_config.intrSrc = irqn;
#if defined (TARGET_MCU_PSOC6_M0)
        irq_config.cm0pSrc = obj->cm0p_irq_src;
#endif
        if (Cy_SysInt_Init(&irq_config, (cy_israddress)(obj->handler)) != CY_SYSINT_SUCCESS) {
            return(-1);
        }

        obj->irqn = irqn;
        NVIC_EnableIRQ(irqn);
    }
    return 0;
}

#endif // DEVICE_I2C_ASYNCH

static int allocate_divider(I2cDividerType divider)
{
    I2cDividerInfo *p_div = &i2c_dividers[divider];

    if (p_div->div_num == CY_INVALID_DIVIDER) {
        p_div->div_num = cy_clk_allocate_divider(CY_SYSCLK_DIV_8_BIT);
        if (p_div->div_num != CY_INVALID_DIVIDER) {
            p_div->div_type = CY_SYSCLK_DIV_8_BIT;
        } else {
            p_div->div_num = cy_clk_allocate_divider(CY_SYSCLK_DIV_16_BIT);
            if (p_div->div_num != CY_INVALID_DIVIDER) {
                p_div->div_type = CY_SYSCLK_DIV_16_BIT;
            }
        }
    }

    if (p_div->div_num != CY_INVALID_DIVIDER) {
        // Set up proper frequency;
        uint32_t div_value = CY_CLK_PERICLK_FREQ_HZ / p_div->clk_frequency;
        p_div->clk_frequency = CY_CLK_PERICLK_FREQ_HZ / div_value;
        if (Cy_SysClk_PeriphSetDivider(p_div->div_type, p_div->div_num, div_value) == CY_SYSCLK_SUCCESS) {
            Cy_SysClk_PeriphEnableDivider(p_div->div_type, p_div->div_num);
        } else {
            p_div->div_num = CY_INVALID_DIVIDER;
        }
    }

    return (p_div->div_num == CY_INVALID_DIVIDER)? -1 : 0;
}

/*
 * Select one of the 3 dividers used depending on the required frequency.
 */
static I2cDividerType select_divider(uint32_t frequency)
{
    if (frequency <= (MIN_I2C_CLOCK_FREQUENCY / CY_SCB_I2C_DUTY_CYCLE_MAX)) {
        // Required speed lower than min supported.
        return I2C_INVALID_DIVIDER;
    } else if (frequency <= CY_SCB_I2C_STD_DATA_RATE) {
        return I2C_DIVIDER_LOW;
    } else if (frequency <= CY_SCB_I2C_FST_DATA_RATE) {
        return I2C_DIVIDER_MID;
    } else if (frequency <= CY_SCB_I2C_FSTP_DATA_RATE) {
        return I2C_DIVIDER_HIGH;
    } else {
        // Required speed too high;
        return I2C_INVALID_DIVIDER;
    }
}

/*
 * Initializes i2c clock for the required speed
 */
static cy_en_sysclk_status_t i2c_init_clock(i2c_obj_t *obj, uint32_t speed)
{
    I2cDividerInfo *p_div = NULL;
    cy_en_sysclk_status_t status = CY_SYSCLK_INVALID_STATE;
    I2cDividerType divider = select_divider(speed);

    if (divider == I2C_INVALID_DIVIDER) {
        error("i2c: required speed/frequency is out of valid range.");
        return CY_SYSCLK_BAD_PARAM;
    }

    if (allocate_divider(divider) < 0) {
        error("i2c: cannot allocate clock divider.");
        return CY_SYSCLK_INVALID_STATE;
    }

    obj->divider = divider;
    p_div = &i2c_dividers[divider];

    status = Cy_SysClk_PeriphAssignDivider(obj->clock, p_div->div_type, p_div->div_num);
    if (status != CY_SYSCLK_SUCCESS) {
        error("i2c: cannot assign clock divider.");
        return status;
    }

    /* Set desired speed/frequency */
    obj->actual_speed = Cy_SCB_I2C_SetDataRate(obj->base, speed, p_div->clk_frequency);
    return (obj->actual_speed != 0)? CY_SYSCLK_SUCCESS : CY_SYSCLK_BAD_PARAM;
}

/*
 * Initializes i/o pins for i2c sda/scl.
 */
static void i2c_init_pins(i2c_obj_t *obj)
{
    int sda_function = pinmap_function(obj->pin_sda, PinMap_I2C_SDA);
    int scl_function = pinmap_function(obj->pin_scl, PinMap_I2C_SCL);
    pin_function(obj->pin_sda, sda_function);
    pin_function(obj->pin_scl, scl_function);
}


/*
 * Initializes and enables I2C/SCB.
 */
static void i2c_init_peripheral(i2c_obj_t *obj)
{
    cy_stc_scb_i2c_config_t i2c_config = default_i2c_config;
    I2cDividerInfo *p_div = &i2c_dividers[obj->divider];

    Cy_SCB_I2C_Init(obj->base, &i2c_config, &obj->context);
    Cy_SCB_I2C_SetDataRate(obj->base,obj->actual_speed, p_div->clk_frequency);
    Cy_SCB_I2C_Enable(obj->base);
}

/*
 * Coverts PDL status into Mbed status.
 */
static int i2c_convert_status(cy_en_scb_i2c_status_t status)
{
    switch (status) {
        case CY_SCB_I2C_MASTER_NOT_READY:
        case CY_SCB_I2C_MASTER_MANUAL_ARB_LOST:
        case CY_SCB_I2C_MASTER_MANUAL_BUS_ERR:
        case CY_SCB_I2C_MASTER_MANUAL_ABORT_START:
            return I2C_ERROR_BUS_BUSY;

        case CY_SCB_I2C_MASTER_MANUAL_TIMEOUT:
        case CY_SCB_I2C_MASTER_MANUAL_ADDR_NAK:
        case CY_SCB_I2C_MASTER_MANUAL_NAK:
            return I2C_ERROR_NO_SLAVE;

        case CY_SCB_I2C_SUCCESS:
        case CY_SCB_I2C_BAD_PARAM:
        default:
            return 0;
    }
}

/*
 * Callback function to handle into and out of deep sleep state transitions.
 */
#if DEVICE_SLEEP && DEVICE_LPTICKER
static cy_en_syspm_status_t i2c_pm_callback(cy_stc_syspm_callback_params_t *callback_params)
{
    cy_stc_syspm_callback_params_t params = *callback_params;
    i2c_obj_t *obj = (i2c_obj_t *)params.context;
    params.context = &obj->context;

    return Cy_SCB_I2C_DeepSleepCallback(&params);
}
#endif // DEVICE_SLEEP && DEVICE_LPTICKER


void i2c_init(i2c_t *obj_in, PinName sda, PinName scl)
{
    i2c_obj_t *obj = OBJ_P(obj_in);
    uint32_t i2c = pinmap_peripheral(sda, PinMap_I2C_SDA);
    i2c = pinmap_merge(i2c, pinmap_peripheral(scl, PinMap_I2C_SCL));
    if (i2c != (uint32_t)NC) {
        if (cy_reserve_io_pin(sda) || cy_reserve_io_pin(scl)) {
            error("I2C pin reservation conflict.");
        }
        obj->base = (CySCB_Type*)i2c;
        obj->i2c_id = ((I2CName)i2c - I2C_0) / (I2C_1 - I2C_0);
        obj->pin_sda = sda;
        obj->pin_scl = scl;
        obj->clock = CY_PIN_CLOCK(pinmap_function(scl, PinMap_I2C_SCL));
        obj->divider = I2C_INVALID_DIVIDER;
        obj->mode = CY_SCB_I2C_MASTER;
        obj->timeout = I2C_DEFAULT_TIMEOUT;
#if DEVICE_I2C_ASYNCH
        obj->pending = PENDING_NONE;
        obj->events = 0;
#endif // DEVICE_I2C_ASYNCH
        i2c_init_clock(obj, I2C_DEFAULT_SPEED);
        i2c_init_pins(obj);
        i2c_init_peripheral(obj);
#if DEVICE_SLEEP && DEVICE_LPTICKER
        obj->pm_callback_handler.callback = i2c_pm_callback;
        obj->pm_callback_handler.type = CY_SYSPM_DEEPSLEEP;
        obj->pm_callback_handler.skipMode = 0;
        obj->pm_callback_handler.callbackParams = &obj->pm_callback_params;
        obj->pm_callback_params.base = obj->base;
        obj->pm_callback_params.context = obj;
        if (!Cy_SysPm_RegisterCallback(&obj->pm_callback_handler)) {
            error("PM callback registration failed!");
        }
#endif // DEVICE_SLEEP && DEVICE_LPTICKER
    } else {
        error("I2C pinout mismatch. Requested pins Rx and Tx can't be used for the same I2C communication.");
    }
}

void i2c_frequency(i2c_t *obj_in, int hz)
{
    i2c_obj_t *obj = OBJ_P(obj_in);
    Cy_SCB_I2C_Disable(obj->base, &obj->context);
    i2c_init_clock(obj, hz);
    Cy_SCB_I2C_Enable(obj->base);
}

int  i2c_start(i2c_t *obj_in)
{
    // Unsupported, start condition is sent automatically.
    return 0;
}

int  i2c_stop(i2c_t *obj_in)
{
    // Unsupported, stop condition is sent automatically.
    return 0;
}

int i2c_read(i2c_t *obj_in, int address, char *data, int length, int stop)
{
    cy_en_scb_i2c_status_t status = CY_SCB_I2C_SUCCESS;
    i2c_obj_t *obj = OBJ_P(obj_in);
    cy_en_scb_i2c_command_t ack = CY_SCB_I2C_ACK;
    int byte_count = 0;
    address >>= 1;

    // Start transaction, send address.
    if (obj->context.state == CY_SCB_I2C_IDLE) {
        status = Cy_SCB_I2C_MasterSendStart(obj->base, address, CY_SCB_I2C_READ_XFER, obj->timeout, &obj->context);
    }
    if (status == CY_SCB_I2C_SUCCESS) {
        while (length > 0) {
            if (length == 1) {
                ack = CY_SCB_I2C_NAK;
            }
            status = Cy_SCB_I2C_MasterReadByte(obj->base, ack, (uint8_t *)data, obj->timeout, &obj->context);
            if (status != CY_SCB_I2C_SUCCESS) {
                break;
            }
            ++byte_count;
            --length;
            ++data;
        }
        // SCB in I2C mode is very time sensitive. In practice we have to request STOP after
        // each block, otherwise it may break the transmission.
        Cy_SCB_I2C_MasterSendStop(obj->base, obj->timeout, &obj->context);
    }

    if (status != CY_SCB_I2C_SUCCESS) {
        Cy_SCB_I2C_MasterSendStop(obj->base, obj->timeout, &obj->context);
        byte_count = i2c_convert_status(status);
    }

    return byte_count;
}

int i2c_write(i2c_t *obj_in, int address, const char *data, int length, int stop)
{
    cy_en_scb_i2c_status_t status = CY_SCB_I2C_SUCCESS;
    i2c_obj_t *obj = OBJ_P(obj_in);
    int byte_count = 0;
    address >>= 1;

    // Start transaction, send address.
    if (obj->context.state == CY_SCB_I2C_IDLE) {
        status = Cy_SCB_I2C_MasterSendStart(obj->base, address, CY_SCB_I2C_WRITE_XFER, obj->timeout, &obj->context);
    }
    if (status == CY_SCB_I2C_SUCCESS) {
        while (length > 0) {
            status = Cy_SCB_I2C_MasterWriteByte(obj->base, *data, obj->timeout, &obj->context);
            if (status != CY_SCB_I2C_SUCCESS) {
                break;;
            }
            ++byte_count;
            --length;
            ++data;
        }
        // SCB in I2C mode is very time sensitive. In practice we have to request STOP after
        // each block, otherwise it may break the transmission.
        Cy_SCB_I2C_MasterSendStop(obj->base, obj->timeout, &obj->context);
    }

    if (status != CY_SCB_I2C_SUCCESS) {
        Cy_SCB_I2C_MasterSendStop(obj->base, obj->timeout, &obj->context);
        byte_count = i2c_convert_status(status);
    }

    return byte_count;
}

void i2c_reset(i2c_t *obj_in)
{
    i2c_stop(obj_in);
}

int i2c_byte_read(i2c_t *obj_in, int last)
{
    i2c_obj_t *obj = OBJ_P(obj_in);
    uint8_t tmp_byte = 0;
    cy_en_scb_i2c_command_t ack = last? CY_SCB_I2C_NAK : CY_SCB_I2C_ACK;
    cy_en_scb_i2c_status_t status = Cy_SCB_I2C_MasterReadByte(obj->base, ack, &tmp_byte, obj->timeout, &obj->context);

    if (status == CY_SCB_I2C_SUCCESS) {
        return tmp_byte;
    } else {
        return 0;
    }
}

int i2c_byte_write(i2c_t *obj_in, int data)
{
    i2c_obj_t *obj = OBJ_P(obj_in);
    cy_en_scb_i2c_status_t status = Cy_SCB_I2C_MasterWriteByte(obj->base, (uint8_t)data, obj->timeout, &obj->context);
    switch (status) {
        case CY_SCB_I2C_MASTER_MANUAL_TIMEOUT:
            return 2;
        case CY_SCB_I2C_MASTER_MANUAL_ADDR_NAK:
        case CY_SCB_I2C_MASTER_MANUAL_NAK:
            return 0;
        case CY_SCB_I2C_SUCCESS:
            return 1;
        default:
            // Error has occurred.
            return (-1);
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

#if DEVICE_I2C_ASYNCH

void i2c_transfer_asynch(i2c_t *obj_in,
                         const void *tx,
                         size_t tx_length,
                         void *rx, size_t rx_length,
                         uint32_t address,
                         uint32_t stop,
                         uint32_t handler,
                         uint32_t event,
                         DMAUsage hint)
{
    i2c_obj_t *obj = OBJ_P(obj_in);

    (void)hint; // At the moment we do not support DMA transfers, so this parameter gets ignored.

    if (obj->pending != PENDING_NONE) {
        return;
    }

    obj->rx_config.slaveAddress = address >> 1;
    obj->tx_config.slaveAddress = address >> 1;
    obj->events = event;
    obj->handler = handler;
    if (i2c_irq_setup_channel(obj) < 0) {
        return;
    }

    obj->rx_config.buffer = rx;
    obj->rx_config.bufferSize = rx_length;
    obj->rx_config.xferPending = !stop;

    obj->tx_config.buffer = (void*)tx;
    obj->tx_config.bufferSize = tx_length;
    obj->tx_config.xferPending = rx_length || !stop;

    if (tx_length) {
        // Write first, then read, or write only.
        if (rx_length > 0) {
            obj->pending = PENDING_TX_RX;
        } else {
            obj->pending = PENDING_TX;
        }
        Cy_SCB_I2C_MasterWrite(obj->base, &obj->tx_config, &obj->context);
    } else if (rx_length) {
        // Read transaction;
        obj->pending = PENDING_RX;
        Cy_SCB_I2C_MasterRead(obj->base, &obj->rx_config, &obj->context);
    }
}

uint32_t i2c_irq_handler_asynch(i2c_t *obj_in)
{
    i2c_obj_t *obj = OBJ_P(obj_in);
    uint32_t event = 0;
    // Process actual interrupt.
    Cy_SCB_I2C_Interrupt(obj->base, &obj->context);
    if (obj->context.state == CY_SCB_I2C_MASTER_CMPLT) {
        if (obj->context.masterStatus & CY_SCB_I2C_MASTER_ERR) {
            if (obj->context.masterStatus & CY_SCB_I2C_MASTER_ADDR_NAK) {
                event = I2C_EVENT_ERROR_NO_SLAVE;
            } else if (obj->context.masterStatus & CY_SCB_I2C_MASTER_DATA_NAK) {
                event = I2C_EVENT_TRANSFER_EARLY_NACK;
            } else {
                event = I2C_EVENT_ERROR;
            }
        } else {
            // Check if a read phase is pending after write.
            if (obj->pending == PENDING_TX_RX) {
                obj->pending = PENDING_RX;
                Cy_SCB_I2C_MasterRead(obj->base, &obj->rx_config, &obj->context);
            } else {
                event = I2C_EVENT_TRANSFER_COMPLETE;
            }
        }
    }
    if (event) {
        obj->pending = PENDING_NONE;
    }
    return event & obj->events;
}

uint8_t i2c_active(i2c_t *obj_in)
{
    i2c_obj_t *obj = OBJ_P(obj_in);
    return (obj->pending != PENDING_NONE);
}

void i2c_abort_asynch(i2c_t *obj_in)
{
    i2c_obj_t *obj = OBJ_P(obj_in);
    if (obj->pending != PENDING_NONE) {
        if (obj->pending == PENDING_RX) {
            Cy_SCB_I2C_MasterAbortRead(obj->base, &obj->context);
        } else {
            Cy_SCB_I2C_MasterAbortWrite(obj->base, &obj->context);
        }
    }
}

#endif // DEVICE_ASYNCH

#endif  // #if DEVICE_I2C
