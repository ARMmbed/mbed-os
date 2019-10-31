/* mbed Microcontroller Library
 * Copyright (c) 2015-2017 Nuvoton
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

#include "cmsis.h"
#include "pinmap.h"
#include "PeripheralPins.h"
#include "nu_modutil.h"
#include "nu_miscutil.h"
#include "nu_bitutil.h"
#include "mbed_critical.h"

#define NU_I2C_DEBUG    0

#if NU_I2C_DEBUG
struct i2c_s MY_I2C;
struct i2c_s MY_I2C_2;
char MY_I2C_STATUS[64];
int MY_I2C_STATUS_POS = 0;
uint32_t MY_I2C_TIMEOUT;
uint32_t MY_I2C_ELAPSED;
uint32_t MY_I2C_T1;
uint32_t MY_I2C_T2;
#endif

struct nu_i2c_var {
    i2c_t *     obj;
    void        (*vec)(void);
};

// NOTE: NANO130 doesn't support relocating vector table. ISR vector passed into NVIC_SetVector() can only be weak symbol defined in startup_Nano100Series.c.
void I2C0_IRQHandler(void);
void I2C1_IRQHandler(void);
static void i2c_irq(i2c_t *obj);
static void i2c_fsm_reset(i2c_t *obj, uint32_t i2c_ctl);
static void i2c_fsm_tranfini(i2c_t *obj, int lastdatanaked);

static struct nu_i2c_var i2c0_var = {
    .obj                =   NULL,
    .vec                =   I2C0_IRQHandler,
};
static struct nu_i2c_var i2c1_var = {
    .obj                =   NULL,
    .vec                =   I2C1_IRQHandler,
};

static uint32_t i2c_modinit_mask = 0;

static const struct nu_modinit_s i2c_modinit_tab[] = {
    {I2C_0, I2C0_MODULE, 0, 0, I2C0_RST, I2C0_IRQn, &i2c0_var},
    {I2C_1, I2C1_MODULE, 0, 0, I2C1_RST, I2C1_IRQn, &i2c1_var},
    
    {NC, 0, 0, 0, 0, (IRQn_Type) 0, NULL}
};

static int i2c_do_tran(i2c_t *obj, char *buf, int length, int read, int naklastdata);
static int i2c_do_trsn(i2c_t *obj, uint32_t i2c_ctl, int sync);
#define NU_I2C_TIMEOUT_STAT_INT     500000
#define NU_I2C_TIMEOUT_STOP         500000
static int i2c_poll_status_timeout(i2c_t *obj, int (*is_status)(i2c_t *obj), uint32_t timeout);
static int i2c_poll_tran_heatbeat_timeout(i2c_t *obj, uint32_t timeout);
static int i2c_is_trsn_done(i2c_t *obj);
static int i2c_is_tran_started(i2c_t *obj);
static int i2c_addr2data(int address, int read);

/* 
 * Wrapper for I2C_SET_CONTROL_REG. Clear new state IF before state change is triggered.
 *
 * NOTE:
 *  NUC472/M453/M487: I2C_T::CTL.SI for both I2C new state IF and trigger state change 
 *  NANO130: I2C_T::INTSTS.INTSTS for I2C new state IF and I2C_T::CON.I2C_STS for trigger state change
 */
__STATIC_INLINE void i2c_set_control_reg(I2C_T *i2c_base, uint8_t i2c_ctrl)
{
    if ((i2c_ctrl & I2C_CON_I2C_STS_Msk) && (i2c_base->INTSTS & I2C_INTSTS_INTSTS_Msk)) {
        i2c_base->INTSTS = I2C_INTSTS_INTSTS_Msk;
    }
    I2C_SET_CONTROL_REG(i2c_base, i2c_ctrl);
}

#if DEVICE_I2CSLAVE
// Convert mbed address to BSP address.
static int i2c_addr2bspaddr(int address);
#endif  // #if DEVICE_I2CSLAVE
static void i2c_enable_int(i2c_t *obj);
static void i2c_disable_int(i2c_t *obj);
static int i2c_set_int(i2c_t *obj, int inten);


#if DEVICE_I2C_ASYNCH
static void i2c_buffer_set(i2c_t *obj, const void *tx, size_t tx_length, void *rx, size_t rx_length);
static void i2c_enable_vector_interrupt(i2c_t *obj, uint32_t handler, int enable);
static void i2c_teardown_async(i2c_t *obj);
#endif

#define TRANCTRL_STARTED        (1)         // Guard I2C ISR from data transfer prematurely
#define TRANCTRL_NAKLASTDATA    (1 << 1)    // Request NACK on last data
#define TRANCTRL_LASTDATANAKED  (1 << 2)    // Last data NACKed
#define TRANCTRL_RECVDATA       (1 << 3)    // Receive data available

uint32_t us_ticker_read(void);

void i2c_init(i2c_t *obj, PinName sda, PinName scl)
{
    uint32_t i2c_sda = pinmap_peripheral(sda, PinMap_I2C_SDA);
    uint32_t i2c_scl = pinmap_peripheral(scl, PinMap_I2C_SCL);
    obj->i2c.i2c = (I2CName) pinmap_merge(i2c_sda, i2c_scl);
    MBED_ASSERT((int)obj->i2c.i2c != NC);
    
    const struct nu_modinit_s *modinit = get_modinit(obj->i2c.i2c, i2c_modinit_tab);
    MBED_ASSERT(modinit != NULL);
    MBED_ASSERT((I2CName) modinit->modname == obj->i2c.i2c);

    obj->i2c.pin_sda = sda;
    obj->i2c.pin_scl = scl;

    pinmap_pinout(sda, PinMap_I2C_SDA);
    pinmap_pinout(scl, PinMap_I2C_SCL);

    // Enable IP clock
    CLK_EnableModuleClock(modinit->clkidx);

    // Reset this module
    SYS_ResetModule(modinit->rsetidx);

#if DEVICE_I2C_ASYNCH
    obj->i2c.dma_usage = DMA_USAGE_NEVER;
    obj->i2c.event = 0;
    obj->i2c.stop = 0;
    obj->i2c.address = 0;
#endif

    // NOTE: Setting I2C bus clock to 100 KHz is required. See I2C::I2C in common/I2C.cpp.
    I2C_Open((I2C_T *) NU_MODBASE(obj->i2c.i2c), 100000);
    // NOTE: INTEN bit and FSM control bits (STA, STO, SI, AA) are packed in one register CTL0. We cannot control interrupt through 
    //       INTEN bit without impacting FSM control bits. Use NVIC_EnableIRQ/NVIC_DisableIRQ instead for interrupt control.
    I2C_T *i2c_base = (I2C_T *) NU_MODBASE(obj->i2c.i2c);
    i2c_base->CON |= (I2C_CON_INTEN_Msk | I2C_CON_IPEN_Msk);

    // Enable sync-mode vector interrupt.
    struct nu_i2c_var *var = (struct nu_i2c_var *) modinit->var;
    var->obj = obj;
    obj->i2c.tran_ctrl = 0;
    obj->i2c.stop = 0;
    obj->i2c.hdlr_async = 0;
    i2c_enable_vector_interrupt(obj, (uint32_t) var->vec, 1);
    
    // Mark this module to be inited.
    int i = modinit - i2c_modinit_tab;
    i2c_modinit_mask |= 1 << i;
}

void i2c_free(i2c_t *obj)
{
    const struct nu_modinit_s *modinit = get_modinit(obj->i2c.i2c, i2c_modinit_tab);
    MBED_ASSERT(modinit != NULL);
    MBED_ASSERT(modinit->modname == (int) obj->i2c.i2c);

    /* Disable I2C interrupt */
    NVIC_DisableIRQ(modinit->irq_n);

    I2C_T *i2c_base = (I2C_T *) NU_MODBASE(obj->i2c.i2c);

    /* Disable I2C module */
    I2C_Close(i2c_base);

    /* Disable IP clock */
    CLK_DisableModuleClock(modinit->clkidx);

    // Mark this module to be deinited.
    int i = modinit - i2c_modinit_tab;
    i2c_modinit_mask &= ~(1 << i);

    /* Free up pins */
    gpio_set(obj->i2c.pin_sda);
    gpio_set(obj->i2c.pin_scl);
    obj->i2c.pin_sda = NC;
    obj->i2c.pin_scl = NC;
}

int i2c_start(i2c_t *obj)
{
    return i2c_do_trsn(obj, I2C_CON_START_Msk | I2C_CON_I2C_STS_Msk, 1);
}

int i2c_stop(i2c_t *obj)
{
    return i2c_do_trsn(obj, I2C_CON_STOP_Msk | I2C_CON_I2C_STS_Msk, 1);
}

void i2c_frequency(i2c_t *obj, int hz)
{
    I2C_SetBusClockFreq((I2C_T *) NU_MODBASE(obj->i2c.i2c), hz);
}

int i2c_read(i2c_t *obj, int address, char *data, int length, int stop)
{
    if (i2c_start(obj)) {
        i2c_stop(obj);
        return I2C_ERROR_BUS_BUSY;
    }

    if (i2c_byte_write(obj, i2c_addr2data(address, 1)) != 1) {
        i2c_stop(obj);
        return I2C_ERROR_NO_SLAVE;
    }

    // Read in bytes
    length = i2c_do_tran(obj, data, length, 1, 1);

    // If not repeated start, send stop.
    if (stop) {
        i2c_stop(obj);
    }

    return length;
}

int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop)
{
    if (i2c_start(obj)) {
        i2c_stop(obj);
        return I2C_ERROR_BUS_BUSY;
    }

    if (i2c_byte_write(obj, i2c_addr2data(address, 0)) != 1) {
        i2c_stop(obj);
        return I2C_ERROR_NO_SLAVE;
    }

    // Write out bytes
    length = i2c_do_tran(obj, (char *) data, length, 0, 1);

    if (stop) {
        i2c_stop(obj);
    }

    return length;
}

void i2c_reset(i2c_t *obj)
{
    i2c_stop(obj);
}

int i2c_byte_read(i2c_t *obj, int last)
{
    char data = 0;
    i2c_do_tran(obj, &data, 1, 1, last);
    return data;
}

int i2c_byte_write(i2c_t *obj, int data)
{
    char data_[1];
    data_[0] = data & 0xFF;
    
    if (i2c_do_tran(obj, data_, 1, 0, 0) == 1 &&
        ! (obj->i2c.tran_ctrl & TRANCTRL_LASTDATANAKED)) {
        return 1;
    }
    else {
        return 0;
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

#if DEVICE_I2CSLAVE

// See I2CSlave.h
#define NoData         0    // the slave has not been addressed
#define ReadAddressed  1    // the master has requested a read from this slave (slave = transmitter)
#define WriteGeneral   2    // the master is writing to all slave
#define WriteAddressed 3    // the master is writing to this slave (slave = receiver)

void i2c_slave_mode(i2c_t *obj, int enable_slave)
{
    I2C_T *i2c_base = (I2C_T *) NU_MODBASE(obj->i2c.i2c);
    
    i2c_disable_int(obj);

    obj->i2c.slaveaddr_state = NoData;
    obj->i2c.hdlr_async = 0;
    
    // Switch to not addressed mode
    i2c_set_control_reg(i2c_base, I2C_CON_I2C_STS_Msk | I2C_CON_ACK_Msk);
    
    i2c_enable_int(obj);
}

int i2c_slave_receive(i2c_t *obj)
{
    int slaveaddr_state;
    
    i2c_disable_int(obj);
    slaveaddr_state = obj->i2c.slaveaddr_state;
    i2c_enable_int(obj);
    
    return slaveaddr_state;
}

int i2c_slave_read(i2c_t *obj, char *data, int length)
{
    return i2c_do_tran(obj, data, length, 1, 1);
}

int i2c_slave_write(i2c_t *obj, const char *data, int length)
{
    return i2c_do_tran(obj, (char *) data, length, 0, 1);
}

void i2c_slave_address(i2c_t *obj, int idx, uint32_t address, uint32_t mask)
{
    I2C_T *i2c_base = (I2C_T *) NU_MODBASE(obj->i2c.i2c);
    
    i2c_disable_int(obj);
    
    // NOTE: On NUC472/M451, non-zero slave address can still work as GC mode is enabled.
    //       On M480, non-zero slave address won't work as GC mode is enabled.
    I2C_SetSlaveAddr(i2c_base, 0, i2c_addr2bspaddr(address), I2C_GCMODE_DISABLE);
    
    i2c_enable_int(obj);
}

static int i2c_addr2bspaddr(int address)
{
    return (address >> 1);
}

#endif // #if DEVICE_I2CSLAVE

static void i2c_enable_int(i2c_t *obj)
{
    const struct nu_modinit_s *modinit = get_modinit(obj->i2c.i2c, i2c_modinit_tab);
    
    core_util_critical_section_enter();
    
    // Enable I2C interrupt
    NVIC_EnableIRQ(modinit->irq_n);
    obj->i2c.inten = 1;
    
    core_util_critical_section_exit();
}

static void i2c_disable_int(i2c_t *obj)
{
    const struct nu_modinit_s *modinit = get_modinit(obj->i2c.i2c, i2c_modinit_tab);
    
    core_util_critical_section_enter();
    
    // Disable I2C interrupt
    NVIC_DisableIRQ(modinit->irq_n);
    obj->i2c.inten = 0;
    
    core_util_critical_section_exit();
}

static int i2c_set_int(i2c_t *obj, int inten)
{
    int inten_back;
    
    core_util_critical_section_enter();
    
    inten_back = obj->i2c.inten;
    
    core_util_critical_section_exit();
    
    if (inten) {
        i2c_enable_int(obj);
    }
    else {
        i2c_disable_int(obj);
    }
    
    return inten_back;
}

static int i2c_do_tran(i2c_t *obj, char *buf, int length, int read, int naklastdata)
{
    if (! buf || ! length) {
        return 0;
    }
    
    int tran_len = 0;
    
    i2c_disable_int(obj);
    obj->i2c.tran_ctrl = naklastdata ? (TRANCTRL_STARTED | TRANCTRL_NAKLASTDATA) : TRANCTRL_STARTED;
    obj->i2c.tran_beg = buf;
    obj->i2c.tran_pos = buf;
    obj->i2c.tran_end = buf + length;
    obj->i2c.hdlr_async = 0;
    i2c_enable_int(obj);
    
    if (i2c_poll_tran_heatbeat_timeout(obj, NU_I2C_TIMEOUT_STAT_INT)) {
#if NU_I2C_DEBUG
        MY_I2C_2 = obj->i2c;
        while (1);
#endif
    }
    else {
        i2c_disable_int(obj);
        tran_len = obj->i2c.tran_pos - obj->i2c.tran_beg;
        obj->i2c.tran_beg = NULL;
        obj->i2c.tran_pos = NULL;
        obj->i2c.tran_end = NULL;
        i2c_enable_int(obj);
    }
    
    return tran_len;
}

static int i2c_do_trsn(i2c_t *obj, uint32_t i2c_ctl, int sync)
{
    I2C_T *i2c_base = (I2C_T *) NU_MODBASE(obj->i2c.i2c);
    int err = 0;
    
    i2c_disable_int(obj);

    if (i2c_poll_status_timeout(obj, i2c_is_trsn_done, NU_I2C_TIMEOUT_STAT_INT)) {
        err = I2C_ERROR_BUS_BUSY;
#if NU_I2C_DEBUG
        MY_I2C_2 = obj->i2c;
        while (1);
#endif
    }
    else {
#if 1
        // NOTE: Avoid duplicate Start/Stop. Otherwise, we may meet strange error.
        uint32_t status = I2C_GET_STATUS(i2c_base);
        
        switch (status) {
        case 0x08:  // Start
        case 0x10:  // Master Repeat Start
            if (i2c_ctl & I2C_CON_START_Msk) {
                return 0;
            }
            else {
                break;
            }
        case 0xF8:  // Bus Released
            if ((i2c_ctl & (I2C_CON_START_Msk | I2C_CON_STOP_Msk)) == I2C_CON_STOP_Msk) {
                return 0;
            }
            else {
                break;
            }
        }
#endif        
        i2c_set_control_reg(i2c_base, i2c_ctl);
        if (sync && i2c_poll_status_timeout(obj, i2c_is_trsn_done, NU_I2C_TIMEOUT_STAT_INT)) {
            err = I2C_ERROR_BUS_BUSY;
#if NU_I2C_DEBUG
            MY_I2C_2 = obj->i2c;
            while (1);
#endif
        }
    }

    i2c_enable_int(obj);
    
    return err;
}

static int i2c_poll_status_timeout(i2c_t *obj, int (*is_status)(i2c_t *obj), uint32_t timeout)
{
    uint32_t t1, t2, elapsed = 0;
    int status_assert = 0;
    
    t1 = us_ticker_read();
    while (1) {
        status_assert = is_status(obj);
        if (status_assert) {
            break;
        }
        
        t2 = us_ticker_read();
        elapsed = (t2 > t1) ? (t2 - t1) : ((uint64_t) t2 + 0xFFFFFFFF - t1 + 1);
        if (elapsed >= timeout) {
#if NU_I2C_DEBUG
            MY_I2C_T1 = t1;
            MY_I2C_T2 = t2;
            MY_I2C_ELAPSED = elapsed;
            MY_I2C_TIMEOUT = timeout;
            MY_I2C_2 = obj->i2c;
            while (1);
#endif
            break;
        }
    }
    
    return (elapsed >= timeout);
}

static int i2c_poll_tran_heatbeat_timeout(i2c_t *obj, uint32_t timeout)
{
    uint32_t t1, t2, elapsed = 0;
    int tran_started;
    char *tran_pos = NULL;
    char *tran_pos2 = NULL;
    
    i2c_disable_int(obj);
    tran_pos = obj->i2c.tran_pos;
    i2c_enable_int(obj);
    t1 = us_ticker_read();
    while (1) {
        i2c_disable_int(obj);
        tran_started = i2c_is_tran_started(obj);
        i2c_enable_int(obj);
        if (! tran_started) {    // Transfer completed or stopped
            break;
        }
        
        i2c_disable_int(obj);
        tran_pos2 = obj->i2c.tran_pos;
        i2c_enable_int(obj);
        t2 = us_ticker_read();
        if (tran_pos2 != tran_pos) {    // Transfer on-going
            t1 = t2;
            tran_pos = tran_pos2;
            continue;
        }
        
        elapsed = (t2 > t1) ? (t2 - t1) : ((uint64_t) t2 + 0xFFFFFFFF - t1 + 1);
        if (elapsed >= timeout) {   // Transfer idle
#if NU_I2C_DEBUG
            MY_I2C = obj->i2c;
            MY_I2C_T1 = t1;
            MY_I2C_T2 = t2;
            MY_I2C_ELAPSED = elapsed;
            MY_I2C_TIMEOUT = timeout;
            MY_I2C_2 = obj->i2c;
            while (1);
#endif
            break;
        }
    }
    
    return (elapsed >= timeout);
}


static int i2c_is_trsn_done(i2c_t *obj)
{
    I2C_T *i2c_base = (I2C_T *) NU_MODBASE(obj->i2c.i2c);
    int i2c_int;
    uint32_t status;
    int inten_back;
    
    inten_back = i2c_set_int(obj, 0);
    // NANO130
    i2c_int = !! (i2c_base->INTSTS & I2C_INTSTS_INTSTS_Msk);
    status = I2C_GET_STATUS(i2c_base);
    i2c_set_int(obj, inten_back);
    
    return (i2c_int || status == 0xF8);
}

static int i2c_is_tran_started(i2c_t *obj)
{
    int started;
    int inten_back;
    
    inten_back = i2c_set_int(obj, 0);
    started = !! (obj->i2c.tran_ctrl & TRANCTRL_STARTED);
    i2c_set_int(obj, inten_back);
    
    return started;
}

static int i2c_addr2data(int address, int read)
{
    return read ? (address | 1) : (address & 0xFE);
}

void I2C0_IRQHandler(void)
{
    i2c_irq(i2c0_var.obj);
}
void I2C1_IRQHandler(void)
{
    i2c_irq(i2c1_var.obj);
}

static void i2c_irq(i2c_t *obj)
{
    if (obj->i2c.hdlr_async) {
        void (*hdlr_async)(void) = (void(*)(void))(obj->i2c.hdlr_async);
        hdlr_async();
        return;
    }
    
    I2C_T *i2c_base = (I2C_T *) NU_MODBASE(obj->i2c.i2c);
    uint32_t status;
    
    if (I2C_GET_TIMEOUT_FLAG(i2c_base)) {
        I2C_ClearTimeoutFlag(i2c_base);
        return;
    }
    
    status = I2C_GET_STATUS(i2c_base);
#if NU_I2C_DEBUG
    if (MY_I2C_STATUS_POS < (sizeof (MY_I2C_STATUS) / sizeof (MY_I2C_STATUS[0]))) {
        MY_I2C_STATUS[MY_I2C_STATUS_POS ++] = status;
    }
    else {
        memset(MY_I2C_STATUS, 0x00, sizeof (MY_I2C_STATUS));
        MY_I2C_STATUS_POS = 0;
    }
#endif
    
    switch (status) {
        // Master Transmit
        case 0x28:  // Master Transmit Data ACK
        case 0x18:  // Master Transmit Address ACK
        case 0x08:  // Start
        case 0x10:  // Master Repeat Start
            if ((obj->i2c.tran_ctrl & TRANCTRL_STARTED) && obj->i2c.tran_pos) {
                if (obj->i2c.tran_pos < obj->i2c.tran_end) {
                    I2C_SET_DATA(i2c_base, *obj->i2c.tran_pos ++);
                    i2c_set_control_reg(i2c_base, I2C_CON_I2C_STS_Msk | I2C_CON_ACK_Msk);
                }
                else {
                    i2c_fsm_tranfini(obj, 0);
                }
            }
            else {
                i2c_disable_int(obj);
            }
            break;
            
        case 0x30:  // Master Transmit Data NACK
            i2c_fsm_tranfini(obj, 1);
            break;
            
        case 0x20:  // Master Transmit Address NACK
            i2c_fsm_tranfini(obj, 1);
            break;
            
        case 0x38:  // Master Arbitration Lost
            i2c_fsm_reset(obj, I2C_CON_I2C_STS_Msk | I2C_CON_ACK_Msk);
            break;
        
        case 0x48:  // Master Receive Address NACK
            i2c_fsm_tranfini(obj, 1);
            break;
            
        case 0x40:  // Master Receive Address ACK
        case 0x50:  // Master Receive Data ACK
        case 0x58:  // Master Receive Data NACK
            if ((obj->i2c.tran_ctrl & TRANCTRL_STARTED) && obj->i2c.tran_pos) {
                if (obj->i2c.tran_pos < obj->i2c.tran_end) {
                    if (status == 0x50 || status == 0x58) {
                        if (obj->i2c.tran_ctrl & TRANCTRL_RECVDATA) {
                            *obj->i2c.tran_pos ++ = I2C_GET_DATA(i2c_base);
                            obj->i2c.tran_ctrl &= ~TRANCTRL_RECVDATA;
                        }
                    }
                    
                    if (status == 0x58) {
#if NU_I2C_DEBUG
                        if (obj->i2c.tran_pos != obj->i2c.tran_end) {
                            MY_I2C = obj->i2c;
                            while (1);
                        }
#endif
                        i2c_fsm_tranfini(obj, 1);
                    }
                    else if (obj->i2c.tran_pos == obj->i2c.tran_end) {
                        obj->i2c.tran_ctrl &= ~TRANCTRL_STARTED;
                        i2c_disable_int(obj);
                    }
                    else {
                        uint32_t i2c_ctl = I2C_CON_I2C_STS_Msk | I2C_CON_ACK_Msk;
                        if ((obj->i2c.tran_end - obj->i2c.tran_pos) == 1 &&
                            obj->i2c.tran_ctrl & TRANCTRL_NAKLASTDATA) {
                            // Last data
                            i2c_ctl &= ~I2C_CON_ACK_Msk;
                        }
                        i2c_set_control_reg(i2c_base, i2c_ctl);
                        obj->i2c.tran_ctrl |= TRANCTRL_RECVDATA;
                    }
                }
                else {
                    obj->i2c.tran_ctrl &= ~TRANCTRL_STARTED;
                    i2c_disable_int(obj);
                    break;
                }
            }
            else {
                i2c_disable_int(obj);
            }
            break;
            
        //case 0x00:  // Bus error
        
        // Slave Transmit
        case 0xB8:  // Slave Transmit Data ACK
        case 0xA8:  // Slave Transmit Address ACK
        case 0xB0:  // Slave Transmit Arbitration Lost
            if ((obj->i2c.tran_ctrl & TRANCTRL_STARTED) && obj->i2c.tran_pos) {
                if (obj->i2c.tran_pos < obj->i2c.tran_end) {
                    uint32_t i2c_ctl = I2C_CON_I2C_STS_Msk | I2C_CON_ACK_Msk;
                    
                    I2C_SET_DATA(i2c_base, *obj->i2c.tran_pos ++);
                    if (obj->i2c.tran_pos == obj->i2c.tran_end &&
                        obj->i2c.tran_ctrl & TRANCTRL_NAKLASTDATA) {
                        // Last data
                        i2c_ctl &= ~I2C_CON_ACK_Msk;
                    }
                    i2c_set_control_reg(i2c_base, i2c_ctl);
                }
                else {
                    obj->i2c.tran_ctrl &= ~TRANCTRL_STARTED;
                    i2c_disable_int(obj);
                    break;
                }
            }
            else {
                i2c_disable_int(obj);
            }
            obj->i2c.slaveaddr_state = ReadAddressed;
            break;
        //case 0xA0:  // Slave Transmit Repeat Start or Stop
        case 0xC0:  // Slave Transmit Data NACK
        case 0xC8:  // Slave Transmit Last Data ACK
            obj->i2c.slaveaddr_state = NoData;
            i2c_fsm_reset(obj, I2C_CON_I2C_STS_Msk | I2C_CON_ACK_Msk);
            break;
        
        // Slave Receive
        case 0x80:  // Slave Receive Data ACK
        case 0x88:  // Slave Receive Data NACK
        case 0x60:  // Slave Receive Address ACK    
        case 0x68:  // Slave Receive Arbitration Lost
            obj->i2c.slaveaddr_state = WriteAddressed;
            if ((obj->i2c.tran_ctrl & TRANCTRL_STARTED) && obj->i2c.tran_pos) {
                if (obj->i2c.tran_pos < obj->i2c.tran_end) {
                    if (status == 0x80 || status == 0x88) {
                        if (obj->i2c.tran_ctrl & TRANCTRL_RECVDATA) {
                            *obj->i2c.tran_pos ++ = I2C_GET_DATA(i2c_base);
                            obj->i2c.tran_ctrl &= ~TRANCTRL_RECVDATA;
                        }
                    }
                    
                    if (status == 0x88) {
#if NU_I2C_DEBUG
                        if (obj->i2c.tran_pos != obj->i2c.tran_end) {
                            MY_I2C = obj->i2c;
                            while (1);
                        }
#endif
                        obj->i2c.slaveaddr_state = NoData;
                        i2c_fsm_reset(obj, I2C_CON_I2C_STS_Msk | I2C_CON_ACK_Msk);
                    }
                    else if (obj->i2c.tran_pos == obj->i2c.tran_end) {
                        obj->i2c.tran_ctrl &= ~TRANCTRL_STARTED;
                        i2c_disable_int(obj);
                    }
                    else {
                        uint32_t i2c_ctl = I2C_CON_I2C_STS_Msk | I2C_CON_ACK_Msk;
                        if ((obj->i2c.tran_end - obj->i2c.tran_pos) == 1 &&
                            obj->i2c.tran_ctrl & TRANCTRL_NAKLASTDATA) {
                            // Last data
                            i2c_ctl &= ~I2C_CON_ACK_Msk;
                        }
                        i2c_set_control_reg(i2c_base, i2c_ctl);
                        obj->i2c.tran_ctrl |= TRANCTRL_RECVDATA;
                    }
                }
                else {
                    obj->i2c.tran_ctrl &= ~TRANCTRL_STARTED;
                    i2c_disable_int(obj);
                    break;
                }
            }
            else {
                i2c_disable_int(obj);
            }
            break;
        //case 0xA0:  // Slave Receive Repeat Start or Stop
        
        // GC mode
        //case 0xA0:  // GC mode Repeat Start or Stop
        case 0x90:  // GC mode Data ACK
        case 0x98:  // GC mode Data NACK
        case 0x70:  // GC mode Address ACK
        case 0x78:  // GC mode Arbitration Lost
            obj->i2c.slaveaddr_state = WriteAddressed;
            if ((obj->i2c.tran_ctrl & TRANCTRL_STARTED) && obj->i2c.tran_pos) {
                if (obj->i2c.tran_pos < obj->i2c.tran_end) {
                    if (status == 0x90 || status == 0x98) {
                        if (obj->i2c.tran_ctrl & TRANCTRL_RECVDATA) {
                            *obj->i2c.tran_pos ++ = I2C_GET_DATA(i2c_base);
                            obj->i2c.tran_ctrl &= ~TRANCTRL_RECVDATA;
                        }
                    }
                    
                    if (status == 0x98) {
#if NU_I2C_DEBUG
                        if (obj->i2c.tran_pos != obj->i2c.tran_end) {
                            MY_I2C = obj->i2c;
                            while (1);
                        }
#endif
                        obj->i2c.slaveaddr_state = NoData;
                        i2c_fsm_reset(obj, I2C_CON_I2C_STS_Msk | I2C_CON_ACK_Msk);
                    }
                    else if (obj->i2c.tran_pos == obj->i2c.tran_end) {
                        obj->i2c.tran_ctrl &= ~TRANCTRL_STARTED;
                        i2c_disable_int(obj);
                    }
                    else {
                        uint32_t i2c_ctl = I2C_CON_I2C_STS_Msk | I2C_CON_ACK_Msk;
                        if ((obj->i2c.tran_end - obj->i2c.tran_pos) == 1 &&
                            obj->i2c.tran_ctrl & TRANCTRL_NAKLASTDATA) {
                            // Last data
                            i2c_ctl &= ~I2C_CON_ACK_Msk;
                        }
                        i2c_set_control_reg(i2c_base, i2c_ctl);
                        obj->i2c.tran_ctrl |= TRANCTRL_RECVDATA;
                    }
                }
                else {
                    obj->i2c.tran_ctrl &= ~TRANCTRL_STARTED;
                    i2c_disable_int(obj);
                    break;
                }
            }
            else {
                i2c_disable_int(obj);
            }
            break;
        
        case 0xF8:  // Bus Released
            break;
            
        default:
            i2c_fsm_reset(obj, I2C_CON_I2C_STS_Msk | I2C_CON_ACK_Msk);
    }
}

static void i2c_fsm_reset(i2c_t *obj, uint32_t i2c_ctl)
{
    I2C_T *i2c_base = (I2C_T *) NU_MODBASE(obj->i2c.i2c);

    obj->i2c.stop = 0;
    
    obj->i2c.tran_ctrl = 0;
            
    i2c_set_control_reg(i2c_base, i2c_ctl);
    obj->i2c.slaveaddr_state = NoData;
}

static void i2c_fsm_tranfini(i2c_t *obj, int lastdatanaked)
{
    if (lastdatanaked) {
        obj->i2c.tran_ctrl |= TRANCTRL_LASTDATANAKED;
    }
            
    obj->i2c.tran_ctrl &= ~TRANCTRL_STARTED;
    i2c_disable_int(obj);
}

#if DEVICE_I2C_ASYNCH

void i2c_transfer_asynch(i2c_t *obj, const void *tx, size_t tx_length, void *rx, size_t rx_length, uint32_t address, uint32_t stop, uint32_t handler, uint32_t event, DMAUsage hint)
{
    // NOTE: M451 I2C only supports 7-bit slave address. The mbed I2C address passed in is shifted left by 1 bit (7-bit addr << 1).
    MBED_ASSERT((address & 0xFFFFFF00) == 0);
    
    // NOTE: First transmit and then receive.
    
    (void) hint;
    obj->i2c.dma_usage = DMA_USAGE_NEVER;
    obj->i2c.stop = stop;
    obj->i2c.address = address;
    obj->i2c.event = event;
    i2c_buffer_set(obj, tx, tx_length, rx, rx_length);

    obj->i2c.hdlr_async = handler;
    i2c_start(obj);
}

uint32_t i2c_irq_handler_asynch(i2c_t *obj)
{
    int event = 0;

    I2C_T *i2c_base = (I2C_T *) NU_MODBASE(obj->i2c.i2c);
    uint32_t status = I2C_GET_STATUS(i2c_base);
    switch (status) {
        case 0x08:  // Start
        case 0x10: {// Master Repeat Start
            if (obj->tx_buff.buffer && obj->tx_buff.pos < obj->tx_buff.length) {
                I2C_SET_DATA(i2c_base, (i2c_addr2data(obj->i2c.address, 0)));
                i2c_set_control_reg(i2c_base, I2C_CON_I2C_STS_Msk);
            }
            else if (obj->rx_buff.buffer && obj->rx_buff.pos < obj->rx_buff.length) {
                I2C_SET_DATA(i2c_base, (i2c_addr2data(obj->i2c.address, 1)));
                i2c_set_control_reg(i2c_base, I2C_CON_I2C_STS_Msk);
            }
            else {
                event = I2C_EVENT_TRANSFER_COMPLETE;
                if (obj->i2c.stop) {
                    i2c_set_control_reg(i2c_base, I2C_CON_STOP_Msk | I2C_CON_I2C_STS_Msk);
                }
            }
            break;
        }
        
        case 0x18:  // Master Transmit Address ACK
        case 0x28:  // Master Transmit Data ACK
            if (obj->tx_buff.buffer && obj->tx_buff.pos < obj->tx_buff.length) {
                uint8_t *tx = (uint8_t *)obj->tx_buff.buffer;
                I2C_SET_DATA(i2c_base, tx[obj->tx_buff.pos ++]);
                i2c_set_control_reg(i2c_base, I2C_CON_I2C_STS_Msk);
            }
            else if (obj->rx_buff.buffer && obj->rx_buff.pos < obj->rx_buff.length) {
                i2c_set_control_reg(i2c_base, I2C_CON_START_Msk | I2C_CON_I2C_STS_Msk);
            }
            else {
                event = I2C_EVENT_TRANSFER_COMPLETE;
                if (obj->i2c.stop) {
                    i2c_set_control_reg(i2c_base, I2C_CON_STOP_Msk | I2C_CON_I2C_STS_Msk);
                }
            }
            break;
            
        case 0x20:  // Master Transmit Address NACK
            event = I2C_EVENT_ERROR_NO_SLAVE;
            if (obj->i2c.stop) {
                i2c_set_control_reg(i2c_base, I2C_CON_STOP_Msk | I2C_CON_I2C_STS_Msk);
            }
            break;
            
        case 0x30:  // Master Transmit Data NACK
            if (obj->tx_buff.buffer && obj->tx_buff.pos < obj->tx_buff.length) {
                event = I2C_EVENT_TRANSFER_EARLY_NACK;
                i2c_set_control_reg(i2c_base, I2C_CON_STOP_Msk | I2C_CON_I2C_STS_Msk);
            }
            else if (obj->rx_buff.buffer && obj->rx_buff.pos < obj->rx_buff.length) {
                i2c_set_control_reg(i2c_base, I2C_CON_START_Msk | I2C_CON_I2C_STS_Msk);
            }
            else {
                event = I2C_EVENT_TRANSFER_COMPLETE;
                if (obj->i2c.stop) {
                    i2c_set_control_reg(i2c_base, I2C_CON_STOP_Msk | I2C_CON_I2C_STS_Msk);
                }
            }
            break;
                
        case 0x38:  // Master Arbitration Lost
            i2c_set_control_reg(i2c_base, I2C_CON_I2C_STS_Msk);  // Enter not addressed SLV mode
            event = I2C_EVENT_ERROR;
            break;
            
        case 0x50:  // Master Receive Data ACK
            if (obj->rx_buff.buffer && obj->rx_buff.pos < obj->rx_buff.length) {
                uint8_t *rx = (uint8_t *) obj->rx_buff.buffer;
                rx[obj->rx_buff.pos ++] = I2C_GET_DATA(((I2C_T *) NU_MODBASE(obj->i2c.i2c)));
            }
        case 0x40:  // Master Receive Address ACK
            i2c_set_control_reg(i2c_base, I2C_CON_I2C_STS_Msk | ((obj->rx_buff.pos != obj->rx_buff.length - 1) ? I2C_CON_ACK_Msk : 0));
            break;
            
        case 0x48:  // Master Receive Address NACK    
            event = I2C_EVENT_ERROR_NO_SLAVE;
            if (obj->i2c.stop) {
                i2c_set_control_reg(i2c_base, I2C_CON_STOP_Msk | I2C_CON_I2C_STS_Msk);
            }
            break;
            
        case 0x58:  // Master Receive Data NACK
            if (obj->rx_buff.buffer && obj->rx_buff.pos < obj->rx_buff.length) {
                uint8_t *rx = (uint8_t *) obj->rx_buff.buffer;
                rx[obj->rx_buff.pos ++] = I2C_GET_DATA(((I2C_T *) NU_MODBASE(obj->i2c.i2c)));
            }
            i2c_set_control_reg(i2c_base, I2C_CON_START_Msk | I2C_CON_I2C_STS_Msk);
            break;
            
        case 0x00:  // Bus error
            event = I2C_EVENT_ERROR;
            i2c_reset(obj);
            break;
            
        default:
            event = I2C_EVENT_ERROR;
            if (obj->i2c.stop) {
                i2c_set_control_reg(i2c_base, I2C_CON_STOP_Msk | I2C_CON_I2C_STS_Msk);
            }
    }
    
    if (event) {
        i2c_teardown_async(obj);
    }

    return (event & obj->i2c.event);
}

uint8_t i2c_active(i2c_t *obj)
{   
    // hdlr_async will get non-NULL for async transfer. Use it to judge if async transfer is on-going.
    return !! obj->i2c.hdlr_async;
}

void i2c_abort_asynch(i2c_t *obj)
{
    i2c_teardown_async(obj);
    i2c_stop(obj);
}

static void i2c_buffer_set(i2c_t *obj, const void *tx, size_t tx_length, void *rx, size_t rx_length)
{
    obj->tx_buff.buffer = (void *) tx;
    obj->tx_buff.length = tx_length;
    obj->tx_buff.pos = 0;
    obj->rx_buff.buffer = rx;
    obj->rx_buff.length = rx_length;
    obj->rx_buff.pos = 0;
}

static void i2c_enable_vector_interrupt(i2c_t *obj, uint32_t handler, int enable)
{
    const struct nu_modinit_s *modinit = get_modinit(obj->i2c.i2c, i2c_modinit_tab);
    MBED_ASSERT(modinit != NULL);
    MBED_ASSERT((I2CName) modinit->modname == obj->i2c.i2c);
    
    if (enable) {
        NVIC_SetVector(modinit->irq_n, handler);
        i2c_enable_int(obj);
    }
    else {
        i2c_disable_int(obj);
    }

}

static void i2c_teardown_async(i2c_t *obj)
{
    obj->i2c.hdlr_async = 0;
}

#endif

#endif
