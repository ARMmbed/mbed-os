/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#if DEVICE_I2C

#include "i2c_api.h"
#include "i2c_def.h"
#include "cmsis.h"
#include "pinmap.h"
#include "mbed_error.h"
#include "mbed_wait_api.h"
/* States of a possibly combined I2C transfer */
typedef enum i2c_transfer_state_t {
    I2C_TRANSFER_SINGLE, /* Non combined transfer */
    I2C_TRANSFER_COMBINED_FIRST_MESSAGE, /*
                                          * First message of a
                                          * combined transfer
                                          */
    I2C_TRANSFER_COMBINED_INTERMEDIATE_MESSAGE, /*
                                                 * Message in the middle
                                                 * of a combined
                                                 * transfer
                                                 */
    I2C_TRANSFER_COMBINED_LAST_MESSAGE, /*
                                         * Last message of a combined
                                         * transfer
                                         */
} i2c_transfer_state_t;

/*
 * Driver private data structure that should not be shared by multiple
 * instances of the driver
 * (same driver for multiple instances of the IP)
 */
typedef struct private_i2c_t {
    /* State of a possibly combined ongoing i2c transfer */
    i2c_transfer_state_t transfer_state;
} private_i2c_t;

struct i2c_msg_s {
    uint16_t addr;              /**< Slave address */
    uint16_t flags;             /**< I2C flags; See I2C_M_* definitions */
#ifdef CONFIG_I2C_USERIO
    uint16_t length;            /**< The length of buffer */
    uint8_t *buffer;            /**< The Buffer for transferring message */
#else
    uint8_t *buffer;            /**< The Buffer for transferring message */
    int length;                 /**< The length of buffer */
#endif
};

#define S5JS100_DEFAULT_I2CXFER_CLOCK   (100 * 1000) // 100KHz
#define S5JS100_DEFAULT_I2C_TIMEOUT     10000
#define S5JS100_DEFAULT_I2CSLAVE_ADDR   0x69
#define HSI2C_INT_XFER_DONE (HSI2C_INT_XFER_DONE_NOACK_MANUAL | HSI2C_INT_XFER_DONE_MANUAL)

/*
 * Retrieve the private data of the instance related to a given IP
 */
#if 0
static private_i2c_t *get_i2c_private(i2c_t *obj)
{
    static private_i2c_t data0, data1;
    /*
     * Select which instance to give using the base
     * address of registers
     */
    switch ((intptr_t)obj->i2c) {
        case I2C0_BASE:
            return &data0;
        case I2C1_BASE:
            return &data1;
        default:
            error("i2c driver private data structure not found for this registers base address");
            return (void *)0;
    }
}
#endif

static const PinMap PinMap_I2C_SDA[] = {
    {I2C_SDA, I2C_0, 0},
    {NC, NC, 0}
};

static const PinMap PinMap_I2C_SCL[] = {
    {I2C_SCL, I2C_0, 0},
    {NC, NC, 0}
};
static void hsi2c_set_hs_timing(unsigned int base, unsigned int nClkDiv,
                                unsigned int tSTART_SU, unsigned int tSTART_HD,
                                unsigned int tSTOP_SU, unsigned int tSDA_SU,
                                unsigned int tDATA_SU, unsigned int tDATA_HD,
                                unsigned int tSCL_L, unsigned int tSCL_H,
                                unsigned int tSR_RELEASE)
{
    tSTART_SU &= 0xFF;
    tSTART_HD &= 0xFF;
    tSTOP_SU &= 0xFF;
    putreg32(((tSTART_SU << 24) | (tSTART_HD << 16) | (tSTOP_SU << 8)), base + I2C_TIMING_HS1);

    tDATA_SU &= 0xFF;
    tSCL_L &= 0xFF;
    tSCL_H &= 0xFF;
    putreg32(((tDATA_SU << 24) | (tSCL_L << 8) | (tSCL_H << 0)), base + I2C_TIMING_HS2);

    nClkDiv &= 0xFF;
    tSR_RELEASE &= 0xFF;
    putreg32(((nClkDiv << 16) | (tSR_RELEASE << 0)), base + I2C_TIMING_HS3);

    tDATA_HD &= 0xFFFF;
    putreg32(tDATA_HD, base + I2C_TIMING_SLA);

}

static void hsi2c_set_fs_timing(unsigned int base, unsigned int nClkDiv,
                                unsigned int tSTART_SU, unsigned int tSTART_HD,
                                unsigned int tSTOP_SU, unsigned int tDATA_SU,
                                unsigned int tDATA_HD, unsigned int tSCL_L,
                                unsigned int tSCL_H, unsigned int tSR_RELEASE)
{
    tSTART_SU &= 0xFF;
    tSTART_HD &= 0xFF;
    tSTOP_SU &= 0xFF;
    putreg32(((tSTART_SU << 24) | (tSTART_HD << 16) | (tSTOP_SU << 8)), base + I2C_TIMING_FS1);

    tDATA_SU &= 0xFF;
    tSCL_L &= 0xFF;
    tSCL_H &= 0xFF;
    putreg32(((tDATA_SU << 24) | (tSCL_L << 8) | (tSCL_H << 0)), base + I2C_TIMING_FS2);

    nClkDiv &= 0xFF;
    tSR_RELEASE &= 0xFF;
    putreg32(((nClkDiv << 16) | (tSR_RELEASE << 0)), base + I2C_TIMING_FS3);

    tDATA_HD &= 0xFFFF;
    putreg32(tDATA_HD, base + I2C_TIMING_SLA);
}

static void hsi2c_calculate_timing(unsigned int base, unsigned int nPclk,
                                   unsigned int nOpClk)
{
    unsigned int reg;
    unsigned int nClkDiv;
    uint32_t tFTL_CYCLE_SCL;
    int i;
    int uTemp0;
    int uTemp1;
    int uTemp2 = 0;

    reg = getreg32(base + I2C_CONF);
    reg &= ~(0x7 << 13);
    putreg32(reg, base + I2C_CONF);

    reg = getreg32(base + I2C_CONF);
    reg &= ~(0x7 << 16);
    putreg32(reg, base + I2C_CONF);

    tFTL_CYCLE_SCL = (getreg32(base + I2C_CONF) >> 16) & 0x7;

    uTemp0 = (float)(nPclk / nOpClk) - (tFTL_CYCLE_SCL + 3) * 2;

    for (i = 0; i < 256; i++) {
        uTemp1 = (uTemp0 + ((tFTL_CYCLE_SCL + 3) % (i + 1)) * 2) / (i + 1);
        if (uTemp1 < 512) {     /*  TSCL_L/H max is 512 / 2 */
            uTemp2 = uTemp1 - 2;
            break;
        }
    }

    unsigned int tSCL_H;
    nClkDiv = i;
    if (nOpClk > I2C_SPEED_400KHZ) {
        tSCL_H = ((uTemp2 + 10) / 3) - 5;
    } else {
        tSCL_H = uTemp2 / 2;
    }

    unsigned int tSCL_L = uTemp2 - tSCL_H;
    unsigned int tSTART_SU = tSCL_L;
    unsigned int tSTART_HD = tSCL_L;
    unsigned int tSTOP_SU = tSCL_L;
    unsigned int tSDA_SU = tSCL_L;
    unsigned int tDATA_SU = tSCL_L;
    unsigned int tDATA_HD = tSCL_L / 2;
    unsigned int tSR_RELEASE = uTemp2;

    if (nOpClk > I2C_SPEED_400KHZ) {
        /* 400Khz setting for Extended ID */
        hsi2c_set_fs_timing(base, 1, 38, 38, 38, 19, 19, 38, 38, 76);
        hsi2c_set_hs_timing(base, nClkDiv, tSTART_SU, tSTART_HD, tSTOP_SU,
                            tSDA_SU, tDATA_SU, tDATA_HD, tSCL_L, tSCL_H, tSR_RELEASE);
    } else {
        hsi2c_set_fs_timing(base, nClkDiv, tSTART_SU, tSTART_HD, tSTOP_SU,
                            tDATA_SU, tDATA_HD, tSCL_L, tSCL_H, tSR_RELEASE);
    }
}

static void hsi2c_conf(unsigned int base, unsigned int nOpClk)
{
    unsigned int val;

    val = getreg32(base + I2C_CONF);
    val &= ~(3 << 30);
    if (nOpClk > I2C_SPEED_400KHZ) {
        val |= (1 << 29);
    } else {
        val &= ~(1 << 29);
    }
    putreg32(val, base + I2C_CONF);
}

static void hsi2c_enable_int(unsigned int base, unsigned int bit)
{
    unsigned int val;
    val = getreg32(base + INT_EN);
    val |= bit;
    putreg32(val, base + INT_EN);
}

static void hsi2c_disable_int(unsigned int base, unsigned int bit)
{
    unsigned int val;

    val = getreg32(base + INT_EN);
    val &= ~bit;
    putreg32(val, base + INT_EN);
}

static int hsi2c_manual_fast_init(i2c_t *obj)
{
    unsigned int val;
    unsigned int base = obj->base;

    hsi2c_conf(base, obj->xfer_speed);
    hsi2c_calculate_timing(base, obj->clock, obj->xfer_speed);
    hsi2c_enable_int(base, HSI2C_INT_XFER_DONE_MANUAL | HSI2C_INT_XFER_DONE_NOACK_MANUAL);

    obj->initialized = 1;

    val = getreg32(base + I2C_CONF);
    val &= ~((1 << 31) | (1 << 29));
    putreg32(val, base + I2C_CONF);

    hsi2c_enable_int(base, HSI2C_INT_XFER_DONE);

    return 0;
}


static void hsi2c_set_slave_addr(unsigned int base, unsigned short addr,
                                 unsigned int is_master)
{
    unsigned int val;

    addr &= 0x3FF;

    val = getreg32(base + I2C_ADDR);

    if (is_master == 0) {
        val &= ~0x3ff;
        val |= addr;
    } else {
        val &= ~(0x3FF << 10);
        val |= (addr << 10);
    }
    putreg32(val, base + I2C_ADDR);
}

static int hsi2c_wait_xfer_done(i2c_t *obj)
{
    int val;
    int timeout = obj->timeout;
    unsigned int base = obj->base;

    while (timeout-- > 0) {
        val = getreg32(base + INT_STAT) & HSI2C_INT_XFER_DONE_MANUAL;
        if (val) {
            putreg32(val, base + INT_STAT);
            return (val == HSI2C_INT_XFER_DONE_MANUAL);
        }
    }

    return -1;
}

static void hsi2c_start(i2c_t *obj)
{
    putreg32(0x88, I2C0_BASE + CTL);

    putreg32(I2C_START, obj->base + I2C_MANUAL_CMD);

    hsi2c_wait_xfer_done(obj);
}

static void hsi2c_stop(i2c_t *obj)
{
    putreg32(I2C_STOP, obj->base + I2C_MANUAL_CMD);

    hsi2c_wait_xfer_done(obj);
}

static void hsi2c_repstart(i2c_t *obj)
{
    putreg32(I2C_RESTART, obj->base + I2C_MANUAL_CMD);
}

static int hsi2c_outb(i2c_t *obj, unsigned char data)
{
    unsigned int val;
    int ret;

    val = ((unsigned int)data) << 24 | I2C_SEND_DATA;
    putreg32(val, obj->base + I2C_MANUAL_CMD);

    ret = hsi2c_wait_xfer_done(obj);

    return ret;
}

static int hsi2c_inb(i2c_t *obj, unsigned char is_ack)
{
    unsigned int val = I2C_READ_DATA;
    unsigned char data;
    int ret;
    unsigned int base = obj->base;

    /* Looks awkward, but if I2C_RX_ACK is set, ACK is NOT generated */
    if (!is_ack) {
        val |= I2C_RX_ACK;
    }
    putreg32(val, base + I2C_MANUAL_CMD);

    ret = hsi2c_wait_xfer_done(obj);
    if (ret < 0) {
        return ret; /* timeout */
    }

    data = (getreg32(base + I2C_MANUAL_CMD) >> 16) & 0xff;

    return data;
}

static int sendbytes(i2c_t *obj, struct i2c_msg_s *msg)
{
    uint8_t *p = msg->buffer;
    int count = msg->length;
    int nak_ok = msg->flags & I2C_M_IGNORE_NAK;
    int wrcount = 0, ret;

    while (count > 0) {
        ret = hsi2c_outb(obj, *p);
        if ((ret == 1) || ((ret == 0) && nak_ok)) {
            count--;
            p++;
            wrcount++;
        } else if (ret == 0) {
            /* NAK from the slave */
            return -EIO;
        } else {
            /* Timeout */
            return ret;
        }
    }

    return wrcount;
}

static int readbytes(i2c_t *obj, struct i2c_msg_s *msg)
{
    int val;
    int rdcount = 0;
    unsigned char *p = msg->buffer;
    int count = msg->length;

    while (count > 0) {
        val = hsi2c_inb(obj, (count > 1));
        if (val < 0) {
            break;
        }

        *p++ = val;
        rdcount++;
        count--;
    }

    return rdcount;
}

static int try_address(i2c_t *obj, unsigned char addr, int retries)
{
    int i, ret = 0;

    for (i = 0; i <= retries; i++) {
        ret = hsi2c_outb(obj, addr);
        if (ret == 1 || i == retries) {
            break;
        }
        hsi2c_stop(obj);
        wait_us(obj->timeout / 2);
        hsi2c_start(obj);
    }

    return ret;
}

static int do_address(i2c_t *obj, struct i2c_msg_s *msg)
{
    unsigned short flags = msg->flags;
    unsigned short nak_ok = msg->flags & I2C_M_IGNORE_NAK;
    unsigned char addr;
    int ret;
    int retries;

    retries = nak_ok ? 0 : obj->retries;

    if (flags & I2C_M_TEN) {
        /* a 10-bit address in manual mode */
        addr = 0xf0 | ((msg->addr >> 7) & 0x06);

        ret = try_address(obj, addr, retries);
        if ((ret != 1) && !nak_ok) {
            return -ENXIO;
        }

        /* the remaining 8 bit address */
        ret = hsi2c_outb(obj, msg->addr & 0xff);
        if ((ret != 1) && !nak_ok) {
            return -ENXIO;
        }

        if (flags & I2C_M_READ) {
            hsi2c_repstart(obj);
            hsi2c_wait_xfer_done(obj);
            addr |= 0x1;
            ret = try_address(obj, addr, retries);
            if ((ret != 1) && !nak_ok) {
                return -EIO;
            }
        }
    } else {
        /* 7-bit address */
        addr = msg->addr << 1;
        if (flags & I2C_M_READ) {
            addr |= 0x1;
        }

        ret = try_address(obj, addr, retries);
        if ((ret != 1) && !nak_ok) {
            return -ENXIO;
        }
    }

    return 0;
}
static void hsi2c_set_auto_config(unsigned int base, unsigned int stop,
                                  unsigned int tx, unsigned int len)
{
    unsigned int val = getreg32(base + I2C_AUTO_CONF);

    if (stop) {
        val |= (1 << 17);
    } else {
        val &= ~(1 << 17);
    }
    if (tx) {
        val &= ~(1 << 16);
    } else {
        val |= (1 << 16);
    }

    val &= ~0xFFFF;
    val |= len;
    putreg32(val, base + I2C_AUTO_CONF);
}

static void hsi2c_set_trans_mode(unsigned int base, unsigned int master,
                                 unsigned int tx)
{
    unsigned int val = getreg32(base + CTL);

    val |= (1 << 0); /* ctrl 0 bit write 1 */

    if (master) {
        val |= (1 << 3);
    } else {
        val &= ~(1 << 3);
    }

    val &= ~(3 << 6);
    if (tx) {
        val |= (1 << 7);
    } else {
        val |= (1 << 6);
    }

    putreg32(val, base + CTL);
}

static void hsi2c_set_hwacg_mode(unsigned int base, unsigned int slave)
{
    unsigned int val = getreg32(base + CTL);

    val &= ~(0x3 << 24);

    if (slave) {
        val |= (0x1 << 24);
    } else {
        val &= ~(0x1 << 24);
    }

    putreg32(val, base + CTL);
}

static void hsi2c_set_fifo_level(unsigned int base)
{
    putreg32(0x10013, base + FIFO_CTL);
}


static void hsi2c_master_setup(i2c_t *obj, unsigned int mode,
                               unsigned int speed, unsigned int slave_addr)
{
    if (obj->mode == I2C_POLLING) {
        obj->xfer_speed = speed;
        hsi2c_calculate_timing(obj->base, obj->clock, speed);
    }
#ifdef CONFIG_S5JS100_I2C_INTERRUPT_MODE
    else if (priv->mode == I2C_INTERRUPT) {
        priv->master_test_data = (struct master_data *)malloc(sizeof(struct master_data));
        /* complete_init(&priv->master_test_data->done); */
        hsi2c_set_trans_mode(obj->base, 1, 1);  /* set master mode */
        hsi2c_conf(obj->base, speed);
        hsi2c_calculate_timing(obj->base, obj->clock, speed);
        hsi2c_set_slave_addr(obj->base, slave_addr, 1);
        hsi2c_set_auto_mode(obj->base);
        hsi2c_set_fifo_level(obj->base);
    }
#endif
}

static void hsi2c_slave_setup(i2c_t *obj, unsigned int mode,
                              unsigned int speed, unsigned int slave_addr)
{
//  obj->i2c->slave_test_data = (struct slave_data *)malloc(sizeof(struct slave_data));

    /* slave mode is only support slave mode */
    hsi2c_set_trans_mode(obj->base, 0, 0);  /* set slave mode */

    /*set hwacg for slave mode */
    hsi2c_set_hwacg_mode(obj->base, 1);

    hsi2c_conf(obj->base, speed);
    hsi2c_calculate_timing(obj->base, obj->clock, speed);
    hsi2c_set_slave_addr(obj->base, slave_addr, 0);

    hsi2c_disable_int(obj->base, HSI2C_INT_ALL);
    hsi2c_enable_int(obj->base, HSI2C_INT_SLAVE_ADDR_MATCH | HSI2C_INT_RX_ALMOST_FULL);

    hsi2c_set_fifo_level(obj->base);
    hsi2c_set_auto_config(obj->base, 0, 0, 0);

    hsi2c_set_trans_mode(obj->base, 0, 0);

#ifdef CONFIG_S5JS100_I2C_INTERRUPT_MODE
    if ((priv->master == I2C_SLAVE_MODE) || (priv->mode == I2C_INTERRUPT)) {
        irq_attach(priv->config->irq, priv->config->isr, NULL);
    }
#endif
}

static int hsi2c_setup(i2c_t *obj, unsigned int master,
                       unsigned int mode, unsigned int speed, unsigned int slave_addr)
{
    obj->master = master;
    obj->mode = mode;
    obj->xfer_speed = speed;
    obj->slave_addr = slave_addr;

    hsi2c_manual_fast_init(obj);

    if (master == I2C_MASTER) {
        hsi2c_master_setup(obj, mode, speed, slave_addr);
    } else if (master == I2C_SLAVE_MODE) {
        hsi2c_slave_setup(obj, mode, speed, slave_addr);
    }

    return 0;
}

unsigned int i2c_setclock(i2c_t *obj, unsigned int frequency)
{
    /* Has the I2C bus frequency changed? */
    if (frequency != obj->xfer_speed) {
        /*
         * Calculate the clock divider that results in the highest frequency
         * that is than or equal to the desired speed.
         */
        if (obj->mode != I2C_POLLING) {
            hsi2c_conf(obj->base, frequency);
        }

        hsi2c_calculate_timing(obj->base, obj->clock, frequency);
    }

    /* Save the new I2C frequency */
    obj->xfer_speed = frequency;
    return 0;
}



static void clear_isr(i2c_t *obj)
{
    /*
     * Writing to the IRQ status register clears set bits. Therefore, to
     * clear indiscriminately, just read the register and write it back.
     */
    uint32_t reg = obj->i2c->IRQ_STATUS;
    obj->i2c->IRQ_STATUS = reg;
}

void i2c_init(i2c_t *obj, PinName sda, PinName scl)
{
    /* Determine the I2C to use */
    I2CName i2c_sda = (I2CName)pinmap_peripheral(sda, PinMap_I2C_SDA);
    I2CName i2c_scl = (I2CName)pinmap_peripheral(scl, PinMap_I2C_SCL);
    obj->i2c = (I2C_TypeDef *)pinmap_merge(i2c_sda, i2c_scl);

    if ((int)obj->i2c == NC) {
        error("I2C pin mapping failed");
    }

    pinmap_pinout(sda, PinMap_I2C_SDA);
    pinmap_pinout(scl, PinMap_I2C_SCL);

    /*
     * Default configuration:
     * - MS    : Master mode
     * - NEA   : Normal (7-bit) addressing
     * - ACKEN : Send ACKs when reading from slave
     * - CLR_FIFO : Not a configuration bit => clears the FIFO
     */
    uint32_t reg = I2C_CTRL_MS | \
                   I2C_CTRL_NEA | \
                   I2C_CTRL_ACKEN | \
                   I2C_CTRL_CLR_FIFO;

    obj->i2c->CONTROL = reg;
    obj->base = I2C0_BASE;
    obj->retries = 0;
    obj->timeout = S5JS100_DEFAULT_I2C_TIMEOUT;
    obj->master = I2C_MASTER;
    obj->initialized = 0;
    obj->clock = cal_clk_getrate(d1_acpu_l3);
    obj->slave_addr = S5JS100_DEFAULT_I2CSLAVE_ADDR;
    obj->mode = I2C_POLLING;
    obj->addrlen = 7;

    hsi2c_setup(obj, obj->master, obj->mode, obj->xfer_speed, obj->slave_addr);

    i2c_setclock(obj, S5JS100_DEFAULT_I2CXFER_CLOCK);
    //get_i2c_private(obj)->transfer_state = I2C_TRANSFER_SINGLE;

    //i2c_frequency(obj, 100000); /* Default to 100kHz SCL frequency */
}

int i2c_start(i2c_t *obj)
{
    return 0;
}

int i2c_stop(i2c_t *obj)
{
    /* Clear the hardware FIFO */
    obj->i2c->CONTROL |= I2C_CTRL_CLR_FIFO;
    /* Clear the HOLD bit used for performing combined transfers */
    obj->i2c->CONTROL &= ~I2C_CTRL_HOLD;
    /* Reset the transfer size (read and write) */
    obj->i2c->TRANSFER_SIZE = 0;
    /* Clear interrupts */
    clear_isr(obj);
    return 0;
}

void i2c_frequency(i2c_t *obj, int hz)
{
    /*
     * Divider is split in two halfs : A and B
     * A is 2 bits wide and B is 6 bits wide
     * The Fscl frequency (SCL clock) is calculated with the following
     * equation:
     * Fscl=SystemCoreClock/(22*(A+1)*(B+1))
     * Here, we only calculate the B divisor which already enables a
     * wide enough range of values
     */
    unsigned int frequency = hz;

    if (frequency != obj->xfer_speed) {
        if (obj->mode != I2C_POLLING) {
            hsi2c_conf(obj->base, frequency);
        }
        hsi2c_calculate_timing(obj->base, obj->clock, frequency);
    }
}

int i2c_transfer(i2c_t *obj, struct i2c_msg_s *msgv, int msgc)
{
    struct i2c_msg_s *pmsg;
    int ret = 0;
    int i;
    int nak_ok;
    int start = 1;
    int stop = 1;
    //unsigned int base = obj->base;

    /* Ensure that address or flags don't change meanwhile */
    //s5js100_i2c_sem_wait(priv);

    /* TODO: initialization */
    if (start) {
        hsi2c_start(obj);
    }

    /*
    if (obj->xfer_speed > I2C_SPEED_400KHZ) {
        hsi2c_conf(base, I2C_SPEED_400KHZ);
        putreg32((0xF << 24 | I2C_SEND_DATA), base + I2C_MANUAL_CMD);
        hsi2c_wait_xfer_noack(priv);

        hsi2c_conf(base, priv->xfer_speed);
        hsi2c_repstart(priv);
        hsi2c_wait_xfer_noack(priv);
    }
    */

    for (i = 0; i < msgc; i++) {
        pmsg = &msgv[i];
        nak_ok = pmsg->flags & I2C_M_IGNORE_NAK;
        if (!(pmsg->flags & I2C_M_NOSTART)) {
            if ((i > 0) || (start == 0)) {
                hsi2c_repstart(obj);
                hsi2c_wait_xfer_done(obj);
            }
            ret = do_address(obj, pmsg);
            if ((ret != 0) && !nak_ok) {
                goto fail;
            }
        }
        if (pmsg->flags & I2C_M_READ) {
            /* read bytes into buffer */
            ret = readbytes(obj, pmsg);
            if (ret < pmsg->length) {
                if (ret >= 0) {
                    return -EIO;
                }
                goto fail;
            }
        } else {
            /* write bytes from buffer */
            ret = sendbytes(obj, pmsg);
            if (ret < pmsg->length) {
                if (ret >= 0) {
                    ret = -EIO;
                }
                goto fail;
            }
        }
    }

fail:
    if (stop) {
        hsi2c_stop(obj);
    }

    /* Ensure that address or flags don't change meanwhile */
    //s5js100_i2c_sem_post(priv);

    return ret;
}
int i2c_read(i2c_t *obj, int address, char *data, int length, int stop)
{

    struct i2c_msg_s xfer[2];

    xfer[0].addr = (unsigned int)address;
    xfer[0].flags = 0;
    xfer[0].buffer = (unsigned char *)&data[0];
    xfer[0].length = 1;

    xfer[1].addr = (unsigned int)address;
    xfer[1].flags = I2C_M_READ;
    xfer[1].buffer = (unsigned char *)&data[1];
    xfer[1].length = length;
    wait_us(100);

    return i2c_transfer(obj, xfer, 2);
}

int i2c_write(i2c_t *obj, int address, const char *data, int length,
              int stop)
{

    struct i2c_msg_s msg;

    msg.addr = address;
    msg.flags = (obj->addrlen == 10) ? I2C_M_TEN : 0;
    msg.buffer = (unsigned char *)data;
    msg.length = length;

    return i2c_transfer(obj, &msg, 1);
}

void i2c_reset(i2c_t *obj)
{
    i2c_stop(obj);
}

int i2c_byte_read(i2c_t *obj, int last)
{
    char i2c_ret = 0;
    i2c_read(obj, obj->last_xfer_address, &i2c_ret, 1, last);
    return i2c_ret;
}

int i2c_byte_write(i2c_t *obj, int data)
{
    /* Store the number of written bytes */
    uint32_t wb = i2c_write(obj, obj->last_xfer_address, (char *)&data, 1, 0);
    if (wb == 1) {
        return 1;
    } else {
        return 0;
    }
}

void i2c_slave_mode(i2c_t *obj, int enable_slave)
{
}

int i2c_slave_receive(i2c_t *obj)
{
    return 0;
}

int i2c_slave_read(i2c_t *obj, char *data, int length)
{
    return 0;
}

int i2c_slave_write(i2c_t *obj, const char *data, int length)
{
    return 0;
}

void i2c_slave_address(i2c_t *obj, int idx, uint32_t address, uint32_t mask)
{
}

#endif // DEVICE_I2C
