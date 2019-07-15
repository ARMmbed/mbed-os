/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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

#include "mbed_assert.h"
#include "dma_api.h"
#include "cmsis.h"
#include "PeripheralPins.h"
#include "r_typedefs.h"

#include "iodefine.h"
#include "RZ_A1_Init.h"

volatile struct st_riic *RIIC[] = RIIC_ADDRESS_LIST;

#define REG(N) \
    RIIC[obj->i2c.i2c]->RIICn##N

/* RIICnCR1 */
#define CR1_RST   (1 << 6)
#define CR1_ICE   (1 << 7)

/* RIICnCR2 */
#define CR2_ST    (1 << 1)
#define CR2_RS    (1 << 2)
#define CR2_SP    (1 << 3)
#define CR2_TRS   (1 << 5)
#define CR2_BBSY  (1 << 7)

/* RIICnMR3 */
#define MR3_ACKBT (1 << 3)
#define MR3_ACKWP (1 << 4)
#define MR3_WAIT  (1 << 6)

/* RIICnSER */
#define SER_SAR0E (1 << 0)

/* RIICnSR1 */
#define SR1_AAS0  (1 << 0)

/* RIICnSR2 */
#define SR2_START (1 << 2)
#define SR2_STOP  (1 << 3)
#define SR2_NACKF (1 << 4)
#define SR2_RDRF  (1 << 5)
#define SR2_TEND  (1 << 6)
#define SR2_TDRE  (1 << 7)

#define WAIT_TIMEOUT    (3600000)  /* Loop counter : Time-out is about 1s. By 3600000 loops, measured value is 969ms. */

static inline int i2c_status(i2c_t *obj) {
    return REG(SR2.UINT8[0]);
}

static void i2c_reg_reset(i2c_t *obj) {
    /* full reset */
    REG(CR1.UINT8[0]) &= ~CR1_ICE; // CR1.ICE off
    REG(CR1.UINT8[0]) |=  CR1_RST; // CR1.IICRST on
    REG(CR1.UINT8[0]) |=  CR1_ICE; // CR1.ICE on

    REG(MR1.UINT8[0])  =  0x08;    // P_phi /x  9bit (including Ack)
    REG(SER.UINT8[0])  =  0x00;    // no slave addr enabled

    /* set frequency */
    REG(MR1.UINT8[0]) |=  obj->i2c.pclk_bit;
    REG(BRL.UINT8[0])  =  obj->i2c.width_low;
    REG(BRH.UINT8[0])  =  obj->i2c.width_hi;

    REG(MR2.UINT8[0])  =  0x07;
    REG(MR3.UINT8[0])  =  0x00;

    REG(FER.UINT8[0])  =  0x72;    // SCLE, NFE enabled, TMOT
    REG(IER.UINT8[0])  =  0x00;    // no interrupt

    REG(CR1.UINT32) &= ~CR1_RST;   // CR1.IICRST negate reset
}

static inline int i2c_wait_RDRF(i2c_t *obj) {
    int timeout = 0;
    
    /* There is no timeout, but the upper limit value is set to avoid an infinite loop. */
    while ((i2c_status(obj) & SR2_RDRF) == 0) {
        timeout ++;
        if (timeout >= WAIT_TIMEOUT) {
            return -1;
        }
    }

    return 0;
}

static int i2c_wait_TDRE(i2c_t *obj) {
    int timeout = 0;

    /* There is no timeout, but the upper limit value is set to avoid an infinite loop. */
    while ((i2c_status(obj) & SR2_TDRE) == 0) {
        timeout ++;
        if (timeout >= WAIT_TIMEOUT) {
            return -1;
        }
    }

    return 0;
}

static int i2c_wait_TEND(i2c_t *obj) {
    int timeout = 0;
    
    /* There is no timeout, but the upper limit value is set to avoid an infinite loop. */
    while ((i2c_status(obj) & SR2_TEND) == 0) {
        timeout ++;
        if (timeout >= WAIT_TIMEOUT) {
            return -1;
        }
    }

    return 0;
}


static int i2c_wait_START(i2c_t *obj) {
    int timeout = 0;
    
    /* There is no timeout, but the upper limit value is set to avoid an infinite loop. */
    while ((i2c_status(obj) & SR2_START) == 0) {
        timeout ++;
        if (timeout >= WAIT_TIMEOUT) {
            return -1;
        }
    }

    return 0;
}

static int i2c_wait_STOP(i2c_t *obj) {
    int timeout = 0;
    
    /* There is no timeout, but the upper limit value is set to avoid an infinite loop. */
    while ((i2c_status(obj) & SR2_STOP) == 0) {
        timeout ++;
        if (timeout >= WAIT_TIMEOUT) {
            return -1;
        }
    }

    return 0;
}

static int i2c_set_STOP(i2c_t *obj) {
    /* SR2.STOP = 0 */
    REG(SR2.UINT32) &= ~SR2_STOP;
    /* Stop condition */
    REG(CR2.UINT32) |= CR2_SP;

    return 0;
}

static void i2c_set_SR2_NACKF_STOP(i2c_t *obj) {
    /* SR2.NACKF = 0 */
    REG(SR2.UINT32) &= ~SR2_NACKF;
    /* SR2.STOP = 0 */
    REG(SR2.UINT32) &= ~SR2_STOP;
}

static void i2c_set_MR3_NACK(i2c_t *obj) {
    /* send a NOT ACK */
    REG(MR3.UINT32) |=  MR3_ACKWP;
    REG(MR3.UINT32) |=  MR3_ACKBT;
    REG(MR3.UINT32) &= ~MR3_ACKWP;
}

static void i2c_set_MR3_ACK(i2c_t *obj) {
    /* send a ACK */
    REG(MR3.UINT32) |=  MR3_ACKWP;
    REG(MR3.UINT32) &= ~MR3_ACKBT;
    REG(MR3.UINT32) &= ~MR3_ACKWP;
}

static inline void i2c_power_enable(i2c_t *obj) {
    volatile uint8_t dummy;
    switch ((int)obj->i2c.i2c) {
        case I2C_0:
            CPGSTBCR9 &= ~(0x80);
            break;
        case I2C_1:
            CPGSTBCR9 &= ~(0x40);
            break;
        case I2C_2:
            CPGSTBCR9 &= ~(0x20);
            break;
        case I2C_3:
            CPGSTBCR9 &= ~(0x10);
            break;
    }
    dummy = CPGSTBCR9;
    (void)dummy;
}

void i2c_init(i2c_t *obj, PinName sda, PinName scl) {
    /* determine the I2C to use */
    I2CName i2c_sda = (I2CName)pinmap_peripheral(sda, PinMap_I2C_SDA);
    I2CName i2c_scl = (I2CName)pinmap_peripheral(scl, PinMap_I2C_SCL);
    obj->i2c.i2c = pinmap_merge(i2c_sda, i2c_scl);
    MBED_ASSERT((int)obj->i2c.i2c != NC);

    /* enable power */
    i2c_power_enable(obj);

    /* set default frequency at 100k */
    i2c_frequency(obj, 100000);

    pinmap_pinout(sda, PinMap_I2C_SDA);
    pinmap_pinout(scl, PinMap_I2C_SCL);
    
    obj->i2c.last_stop_flag = 1;
}

inline int i2c_start(i2c_t *obj) {
    int timeout = 0;

    while ((REG(CR2.UINT32) & CR2_BBSY) != 0) {
        timeout ++;
        if (timeout >= obj->i2c.bbsy_wait_cnt) {
            break;
        }
    }
    /* Start Condition */
    REG(CR2.UINT8[0]) |= CR2_ST;

    return 0;
}

static inline int i2c_restart(i2c_t *obj) {
    /* SR2.START = 0 */
    REG(SR2.UINT32) &= ~SR2_START;
    /* ReStart condition */
    REG(CR2.UINT32) |= CR2_RS;

    return 0;
}

inline int i2c_stop(i2c_t *obj) {
    (void)i2c_set_STOP(obj);
    (void)i2c_wait_STOP(obj);
    i2c_set_SR2_NACKF_STOP(obj);
    
    return 0;
}

static void i2c_set_err_noslave(i2c_t *obj) {
    (void)i2c_set_STOP(obj);
    (void)i2c_wait_STOP(obj);
    i2c_set_SR2_NACKF_STOP(obj);
    obj->i2c.last_stop_flag = 1;
}

static inline int i2c_do_write(i2c_t *obj, int value) {
    int timeout = 0;

    /* There is no timeout, but the upper limit value is set to avoid an infinite loop. */
    while ((i2c_status(obj) & SR2_TDRE) == 0) {
        timeout ++;
        if (timeout >= WAIT_TIMEOUT) {
            return -1;
        }
    }
    /* write the data */
    REG(DRT.UINT32) = value;

    return 0;
}

static inline int i2c_read_address_write(i2c_t *obj, int value) {
    int status;
    
    status = i2c_wait_TDRE(obj);
    if (status == 0) {
        /* write the data */
        REG(DRT.UINT32) = value;
    }
    
    return status;

}

static inline int i2c_do_read(i2c_t *obj, int last) {
    if (last == 2) {
        /* this time is befor last byte read */
        /* Set MR3 WAIT bit is 1 */;
        REG(MR3.UINT32) |= MR3_WAIT;
    } else if (last == 1) {
        i2c_set_MR3_NACK(obj);
    } else {
        i2c_set_MR3_ACK(obj);
    }

    /* return the data */
    return (REG(DRR.UINT32) & 0xFF);
}

void i2c_frequency(i2c_t *obj, int hz) {
    float64_t pclk_val;
    float64_t wait_utime;
    volatile float64_t bps;
    volatile float64_t L_time;         /* H Width period */
    volatile float64_t H_time;         /* L Width period */
    uint32_t tmp_L_width;
    uint32_t tmp_H_width;
    uint32_t remainder;
    uint32_t wk_cks = 0;

    /* set PCLK */
    if (false == RZ_A1_IsClockMode0()) {
        pclk_val = (float64_t)CM1_RENESAS_RZ_A1_P0_CLK;
    } else {
        pclk_val = (float64_t)CM0_RENESAS_RZ_A1_P0_CLK;
    }

    /* Min 10kHz, Max 400kHz */
    if (hz < 10000) {
        bps = 10000;
    } else if (hz > 400000) {
        bps = 400000;
    } else {
        bps = (float64_t)hz;
    }

    /* Calculation L width time */
    L_time = (1 / (2 * bps));   /* Harf period of frequency */
    H_time = L_time;

    /* Check I2C mode of Speed */
    if (bps > 100000) {
        /* Fast-mode */
        L_time -= 102E-9;    /* Falling time of SCL clock. */
        H_time -= 138E-9;    /* Rising time of SCL clock. */
        /* Check L wideth */
        if (L_time < 1.3E-6) {
            /* Wnen L width less than 1.3us */
            /* Subtract Rise up and down time for SCL from H/L width */
            L_time = 1.3E-6;
            H_time = (1 / bps) - L_time - 138E-9 - 102E-9;
        }
    }

    tmp_L_width   = (uint32_t)(L_time * pclk_val * 10);
    tmp_L_width >>= 1;
    wk_cks++;
    while (tmp_L_width >= 341) {
        tmp_L_width >>= 1;
        wk_cks++;
    }
    remainder   = tmp_L_width % 10;
    tmp_L_width = ((tmp_L_width + 9) / 10) - 3;       /* carry */

    tmp_H_width   = (uint32_t)(H_time * pclk_val * 10);
    tmp_H_width >>= wk_cks;
    if (remainder == 0) {
        tmp_H_width   = ((tmp_H_width + 9) / 10) - 3; /* carry */
    } else {
        remainder    += tmp_H_width % 10;
        tmp_H_width   = (tmp_H_width / 10) - 3;
        if (remainder > 10) {
            tmp_H_width += 1;                         /* fine adjustment */
        }
    }
    /* timeout of BBSY bit is minimum low width by frequency */
    /* so timeout calculates "(low width) * 2" by frequency */
    wait_utime = (L_time * 2) * 1000000;
    /* 1 wait of BBSY bit is about 0.3us. if it's below 0.3us, wait count is set as 1. */
    if (wait_utime <= 0.3) {
        obj->i2c.bbsy_wait_cnt = 1;
    } else {
        obj->i2c.bbsy_wait_cnt = (int)(wait_utime / 0.3);
    }


    /* I2C Rate */
    obj->i2c.pclk_bit  = (uint8_t)(0x10 * wk_cks);        /* P_phi / xx */
    obj->i2c.width_low = (uint8_t)(tmp_L_width | 0x000000E0);
    obj->i2c.width_hi  = (uint8_t)(tmp_H_width | 0x000000E0);

    /* full reset */
    i2c_reg_reset(obj);
}

int i2c_read(i2c_t *obj, int address, char *data, int length, int stop) {
    int count = 0;
    int status;
    int value;

    i2c_set_MR3_ACK(obj);
    /* There is a STOP condition for last processing */
    if (obj->i2c.last_stop_flag != 0) {
        status = i2c_start(obj);
        if (status != 0) {
            i2c_set_err_noslave(obj);
            return I2C_ERROR_BUS_BUSY;
        }
    }
    obj->i2c.last_stop_flag = stop;
    /*  Send Slave address */
    status = i2c_read_address_write(obj, (address | 0x01));
    if (status != 0) {
        i2c_set_err_noslave(obj);
        return I2C_ERROR_NO_SLAVE;
    }
    /* wait RDRF */
    status = i2c_wait_RDRF(obj);
    /* check ACK/NACK */
    if ((status != 0) || ((REG(SR2.UINT32) & SR2_NACKF) != 0)) {
        /* Slave sends NACK */
        (void)i2c_set_STOP(obj);
        /* dummy read */
        value = REG(DRR.UINT32);
        (void)i2c_wait_STOP(obj);
        i2c_set_SR2_NACKF_STOP(obj);
        obj->i2c.last_stop_flag = 1;
        return I2C_ERROR_NO_SLAVE;
    }
    if (length != 0) {
        /* Read in all except last byte */
        if (length > 2) {
            /* dummy read */
            value = REG(DRR.UINT32);
            for (count = 0; count < (length - 1); count++) {
                /* wait for it to arrive */
                status = i2c_wait_RDRF(obj);
                if (status != 0) {
                    i2c_set_err_noslave(obj);
                    return I2C_ERROR_NO_SLAVE;
                }
                /* Recieve the data */
                if (count == (length - 2)) {
                    value = i2c_do_read(obj, 1);
                } else if ((length >= 3) && (count == (length - 3))) {
                    value = i2c_do_read(obj, 2);
                } else {
                    value = i2c_do_read(obj, 0);
                }
                data[count] = (char)value;
            }
        } else if (length == 2) {
            /* Set MR3 WAIT bit is 1 */
            REG(MR3.UINT32) |= MR3_WAIT;
            /* dummy read */
            value = REG(DRR.UINT32);
            /* wait for it to arrive */
            status = i2c_wait_RDRF(obj);
            if (status != 0) {
                i2c_set_err_noslave(obj);
                return I2C_ERROR_NO_SLAVE;
            }
            i2c_set_MR3_NACK(obj);
            data[count] = (char)REG(DRR.UINT32);
            count++;
        } else {
            /* length == 1 */
            /* Set MR3 WAIT bit is 1 */;
            REG(MR3.UINT32) |=  MR3_WAIT;
            i2c_set_MR3_NACK(obj);
            /* dummy read */
            value = REG(DRR.UINT32);
        }
        /* wait for it to arrive */
        status = i2c_wait_RDRF(obj);
        if (status != 0) {
            i2c_set_err_noslave(obj);
            return I2C_ERROR_NO_SLAVE;
        }

        /* If not repeated start, send stop. */
        if (stop) {
            (void)i2c_set_STOP(obj);
            /* RIICnDRR read */
            value = (REG(DRR.UINT32) & 0xFF);
            data[count] = (char)value;
            /* RIICnMR3.WAIT = 0 */
            REG(MR3.UINT32) &= ~MR3_WAIT;
            (void)i2c_wait_STOP(obj);
            i2c_set_SR2_NACKF_STOP(obj);
        } else {
            (void)i2c_restart(obj);
            /* RIICnDRR read */
            value = (REG(DRR.UINT32) & 0xFF);
            data[count] = (char)value;
            /* RIICnMR3.WAIT = 0 */
            REG(MR3.UINT32) &= ~MR3_WAIT;
            (void)i2c_wait_START(obj);
            /* SR2.START = 0 */
            REG(SR2.UINT32) &= ~SR2_START;
        }
    } else {
        /* If not repeated start, send stop. */
        if (stop) {
            (void)i2c_set_STOP(obj);
            (void)i2c_wait_STOP(obj);
            i2c_set_SR2_NACKF_STOP(obj);
        } else {
            (void)i2c_restart(obj);
            (void)i2c_wait_START(obj);
            /* SR2.START = 0 */
            REG(SR2.UINT32) &= ~SR2_START;
        }
    }

    return length;
}

int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop) {
    int cnt;
    int status;

    /* There is a STOP condition for last processing */
    if (obj->i2c.last_stop_flag != 0) {
        status = i2c_start(obj);
        if (status != 0) {
            i2c_set_err_noslave(obj);
            return I2C_ERROR_BUS_BUSY;
        }
    }
    obj->i2c.last_stop_flag = stop;
    /*  Send Slave address */
    status = i2c_do_write(obj, address);
    if (status != 0) {
        i2c_set_err_noslave(obj);
        return I2C_ERROR_NO_SLAVE;
    }
    /* Wait send end */
    status = i2c_wait_TEND(obj);
    if ((status != 0) || ((REG(SR2.UINT32) & SR2_NACKF) != 0)) {
        /* Slave sends NACK */
        i2c_set_err_noslave(obj);
        return I2C_ERROR_NO_SLAVE;
    }
    /* Send Write data */
    for (cnt=0; cnt<length; cnt++) {
        status = i2c_do_write(obj, data[cnt]);
        if(status != 0) {
            i2c_set_err_noslave(obj);
            return cnt;
        } else {
            /* Wait send end */
            status = i2c_wait_TEND(obj);
            if ((status != 0) || ((REG(SR2.UINT32) & SR2_NACKF) != 0)) {
                /* Slave sends NACK */
                i2c_set_err_noslave(obj);
                return I2C_ERROR_NO_SLAVE;
            }
        }
    }
    /* If not repeated start, send stop. */
    if (stop) {
        (void)i2c_set_STOP(obj);
        (void)i2c_wait_STOP(obj);
        i2c_set_SR2_NACKF_STOP(obj);
    } else {
        (void)i2c_restart(obj);
        (void)i2c_wait_START(obj);
        /* SR2.START = 0 */
        REG(SR2.UINT32) &= ~SR2_START;

    }
    
    return length;
}

void i2c_reset(i2c_t *obj) {
    (void)i2c_set_STOP(obj);
    (void)i2c_wait_STOP(obj);
    i2c_set_SR2_NACKF_STOP(obj);
}

int i2c_byte_read(i2c_t *obj, int last) {
    int status;
    int data;

    data = i2c_do_read(obj, last);
    /* wait for it to arrive */
    status = i2c_wait_RDRF(obj);
    if (status != 0) {
        i2c_set_SR2_NACKF_STOP(obj);
        return I2C_ERROR_NO_SLAVE;
    }
    
    return data;
}

int i2c_byte_write(i2c_t *obj, int data) {
    int ack = 0;
    int status;
    int timeout = 0;
    
    status = i2c_do_write(obj, (data & 0xFF));
    if (status != 0) {
        i2c_set_SR2_NACKF_STOP(obj);
    } else {
        while (((i2c_status(obj) & SR2_RDRF) == 0) && ((i2c_status(obj) & SR2_TEND) == 0)) {
            timeout++;
            if (timeout >= WAIT_TIMEOUT) {
                return ack;
            }
        }
        /* check ACK/NACK */
        if ((REG(SR2.UINT32) & SR2_NACKF) != 0) {
            /* NACK */
            i2c_set_SR2_NACKF_STOP(obj);
        } else {
            ack = 1;
        }
    }

    return ack;
}

void i2c_slave_mode(i2c_t *obj, int enable_slave) {
    if (enable_slave != 0) {
        REG(SER.UINT32) |= SER_SAR0E;   // only slave addr 0 is enabled
    } else {
        REG(SER.UINT32) &= ~SER_SAR0E;  // no slave addr enabled
    }
}

int i2c_slave_receive(i2c_t *obj) {
    int status;
    int retval;

    status = (REG(SR1.UINT8[0]) & SR1_AAS0);
    status |= (REG(CR2.UINT8[0]) & CR2_TRS) >> 4;

    switch(status) {
        case 0x01:
            /* the master is writing to this slave */
            retval = 3;
            break;
        case 0x02:
            /* the master is writing to all slave  */
            retval = 2;
            break;
        case 0x03:
            /* the master has requested a read from this slave */
            retval = 1;
            break;
        default :
            /* no data */
            retval = 0;
            break;
    }

    return retval;
}

int i2c_slave_read(i2c_t *obj, char *data, int length) {
    int timeout = 0;
    int count;
    int break_flg = 0;

    if(length <= 0) {
        return 0;
    }
    for (count = 0; ((count < (length + 1)) && (break_flg == 0)); count++) {
        /* There is no timeout, but the upper limit value is set to avoid an infinite loop. */
        while (((i2c_status(obj) & SR2_STOP) != 0) || ((i2c_status(obj) & SR2_RDRF) == 0)) {
            if ((i2c_status(obj) & SR2_STOP) != 0) {
                break_flg = 1;
                break;
            }
            timeout ++;
            if (timeout >= WAIT_TIMEOUT) {
                return -1;
            }
        }
        if (break_flg == 0) {
            if (count == 0) {
                /* dummy read */
                (void)REG(DRR.UINT32);
            } else {
                data[count - 1] = (char)(REG(DRR.UINT32) & 0xFF);
            }
        }
    }
    if (break_flg == 0) {
        (void)i2c_wait_STOP(obj);
    } else {
        if ((i2c_status(obj) & SR2_RDRF) != 0) {
            if (count <= 1) {
                /* fail safe */
                /* dummy read */
                (void)REG(DRR.UINT32);
            } else {
                data[count - 2] = (char)(REG(DRR.UINT32) & 0xFF);
            }
        }
    }
    /* SR2.STOP = 0 */
    REG(SR2.UINT32) &= ~SR2_STOP;

    return (count - 1);
}

int i2c_slave_write(i2c_t *obj, const char *data, int length) {
    int count = 0;
    int status = 0;

    if(length <= 0) {
        return 0;
    }

    while ((count < length) && (status == 0)) {
        status = i2c_do_write(obj, data[count]);
        if(status == 0) {
            /* Wait send end */
            status = i2c_wait_TEND(obj);
            if ((status != 0) || ((count < (length - 1)) && ((REG(SR2.UINT32) & SR2_NACKF) != 0))) {
                /* NACK */
                break;
            }
        }
        count++;
    }
    /* dummy read */
    (void)REG(DRR.UINT32);
    (void)i2c_wait_STOP(obj);
    i2c_set_SR2_NACKF_STOP(obj);

    return count;
}

void i2c_slave_address(i2c_t *obj, int idx, uint32_t address, uint32_t mask) {
    REG(SAR0.UINT32) = (address & 0xfffffffe);
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

#define IRQ_NUM 4
#define IRQ_TX 0
#define IRQ_RX 1
#define IRQ_ERR1 2
#define IRQ_ERR2 3

static void i2c_irqs_set(i2c_t *obj, uint32_t enable);

static void i2c0_tx_irq(void);
static void i2c1_tx_irq(void);
static void i2c2_tx_irq(void);
static void i2c3_tx_irq(void);
static void i2c0_rx_irq(void);
static void i2c1_rx_irq(void);
static void i2c2_rx_irq(void);
static void i2c3_rx_irq(void);
static void i2c0_al_irq(void);
static void i2c1_al_irq(void);
static void i2c2_al_irq(void);
static void i2c3_al_irq(void);
static void i2c0_to_irq(void);
static void i2c1_to_irq(void);
static void i2c2_to_irq(void);
static void i2c3_to_irq(void);

static const IRQn_Type irq_set_tbl[RIIC_COUNT][IRQ_NUM] = {
    {INTIICTEI0_IRQn, INTIICRI0_IRQn, INTIICALI0_IRQn, INTIICTMOI0_IRQn},
    {INTIICTEI1_IRQn, INTIICRI1_IRQn, INTIICALI1_IRQn, INTIICTMOI1_IRQn},
    {INTIICTEI2_IRQn, INTIICRI2_IRQn, INTIICALI2_IRQn, INTIICTMOI2_IRQn},
    {INTIICTEI3_IRQn, INTIICRI3_IRQn, INTIICALI3_IRQn, INTIICTMOI3_IRQn},
};

static const IRQHandler hander_set_tbl[RIIC_COUNT][IRQ_NUM] = {
    {i2c0_tx_irq, i2c0_rx_irq, i2c0_al_irq, i2c0_to_irq},
    {i2c1_tx_irq, i2c1_rx_irq, i2c1_al_irq, i2c1_to_irq},
    {i2c2_tx_irq, i2c2_rx_irq, i2c2_al_irq, i2c2_to_irq},
    {i2c3_tx_irq, i2c3_rx_irq, i2c3_al_irq, i2c3_to_irq},
};

struct i2c_global_data_s {
    i2c_t *async_obj;
    uint32_t async_callback, event, shouldStop, address;
};

static struct i2c_global_data_s i2c_data[RIIC_COUNT];

static void i2c_transfer_finished(i2c_t *obj)
{
    i2c_irqs_set(obj, 0);
    uint32_t index = obj->i2c.i2c;
    i2c_data[index].event = I2C_EVENT_TRANSFER_COMPLETE;
    i2c_data[index].async_obj = NULL;
    ((void (*)())i2c_data[index].async_callback)();
}

static void i2c_tx_irq(IRQn_Type irq_num, uint32_t index)
{
    i2c_t *obj = i2c_data[index].async_obj;
    if ((REG(SR2.UINT32) & SR2_NACKF)) {
        /* Slave sends NACK */
        i2c_set_err_noslave(obj);
        i2c_data[index].event = I2C_EVENT_ERROR | I2C_EVENT_TRANSFER_EARLY_NACK;
        i2c_abort_asynch(obj);
        ((void (*)())i2c_data[index].async_callback)();
        return;
    }
    if (obj->tx_buff.pos == obj->tx_buff.length) {
        /* All datas have tranferred */
        
        /* Clear TEND */
        REG(SR2.UINT32) &= ~(SR2_TEND);
        
        /* If not repeated start, send stop. */
        if (i2c_data[index].shouldStop && obj->rx_buff.length == 0) {
            (void)i2c_set_STOP(obj);
            (void)i2c_wait_STOP(obj);
            i2c_set_SR2_NACKF_STOP(obj);
            i2c_transfer_finished(obj);
        } else {
            (void)i2c_restart(obj);
            (void)i2c_wait_START(obj);
            /* SR2.START = 0 */
            REG(SR2.UINT32) &= ~SR2_START;
            if (obj->rx_buff.length) {
                /* Ready to read */
                i2c_set_MR3_ACK(obj);
                
                /* Disable INTRIICTEI */
                REG(IER.UINT8[0]) &= ~(1 << 6);
                
                /*  Send Slave address */
                if (i2c_read_address_write(obj, (i2c_data[index].address | 0x01)) != 0) {
                    i2c_set_err_noslave(obj);
                    i2c_data[index].event = I2C_EVENT_ERROR | I2C_EVENT_ERROR_NO_SLAVE;
                    i2c_abort_asynch(obj);
                    ((void (*)())i2c_data[index].async_callback)();
                    return;
                }
            } else {
                i2c_transfer_finished(obj);
            }
        }
    } else {
        /* Send next 1 byte */
        if (i2c_do_write(obj, *(uint8_t *)obj->tx_buff.buffer) != 0) {
            i2c_set_err_noslave(obj);
            i2c_data[index].event = I2C_EVENT_ERROR | I2C_EVENT_ERROR_NO_SLAVE;
            i2c_abort_asynch(obj);
            ((void (*)())i2c_data[index].async_callback)();
            return;
        }
        obj->tx_buff.buffer = (uint8_t *)obj->tx_buff.buffer + 1;
        ++obj->tx_buff.pos;
    }
}

static void i2c_rx_irq(IRQn_Type irq_num, uint32_t index)
{
    i2c_t *obj = i2c_data[index].async_obj;
    if (obj->rx_buff.pos == SIZE_MAX) {
        if ((REG(SR2.UINT32) & SR2_NACKF) != 0) {
            /* Slave sends NACK */
            (void)i2c_set_STOP(obj);
            /* dummy read */
            if (REG(DRR.UINT32)) {}
            (void)i2c_wait_STOP(obj);
            i2c_set_SR2_NACKF_STOP(obj);
            obj->i2c.last_stop_flag = 1;
            
            i2c_data[index].event = I2C_EVENT_ERROR | I2C_EVENT_TRANSFER_EARLY_NACK;
            i2c_abort_asynch(obj);
            ((void (*)())i2c_data[index].async_callback)();
            return;
        }
        if (obj->rx_buff.length == 1) {
            /* length == 1 */
            /* Set MR3 WAIT bit is 1 */;
            REG(MR3.UINT32) |=  MR3_WAIT;
            i2c_set_MR3_NACK(obj);
        } else if (obj->rx_buff.length == 2) {
            /* Set MR3 WAIT bit is 1 */
            REG(MR3.UINT32) |= MR3_WAIT;
        }
        /* dummy read */
        if (REG(DRR.UINT32)) {}
        obj->rx_buff.pos = 0;
        return;
    }
    if ((REG(SR2.UINT32) & SR2_NACKF) != 0) {
        /* Slave sends NACK */
        i2c_set_err_noslave(obj);
        i2c_data[index].event = I2C_EVENT_ERROR | I2C_EVENT_TRANSFER_EARLY_NACK;
        i2c_abort_asynch(obj);
        ((void (*)())i2c_data[index].async_callback)();
        return;
    } else {
        switch (obj->rx_buff.length - obj->rx_buff.pos) {
            case 1:
                /* Finished */
                /* If not repeated start, send stop. */
                if (i2c_data[index].shouldStop) {
                    (void)i2c_set_STOP(obj);
                    /* RIICnDRR read */
                    *(uint8_t *)obj->rx_buff.buffer = REG(DRR.UINT32) & 0xFF;
                    /* RIICnMR3.WAIT = 0 */
                    REG(MR3.UINT32) &= ~MR3_WAIT;
                    (void)i2c_wait_STOP(obj);
                    i2c_set_SR2_NACKF_STOP(obj);
                } else {
                    (void)i2c_restart(obj);
                    /* RIICnDRR read */
                    *(uint8_t *)obj->rx_buff.buffer = REG(DRR.UINT32) & 0xFF;
                    /* RIICnMR3.WAIT = 0 */
                    REG(MR3.UINT32) &= ~MR3_WAIT;
                    (void)i2c_wait_START(obj);
                    /* SR2.START = 0 */
                    REG(SR2.UINT32) &= ~SR2_START;
                }
                
                i2c_transfer_finished(obj);
                return;
                
            case 2:
                i2c_set_MR3_NACK(obj);
                break;
                
            case 3:
                /* this time is befor last byte read */
                /* Set MR3 WAIT bit is 1 */
                REG(MR3.UINT32) |= MR3_WAIT;
                break;
                
            default:
                i2c_set_MR3_ACK(obj);
                break;
        }
        *(uint8_t *)obj->rx_buff.buffer = REG(DRR.UINT32) & 0xFF;
        obj->rx_buff.buffer = (uint8_t *)obj->rx_buff.buffer + 1;
        ++obj->rx_buff.pos;
    }
}

static void i2c_err_irq(IRQn_Type irq_num, uint32_t index)
{
    i2c_t *obj = i2c_data[index].async_obj;
    i2c_abort_asynch(obj);
    i2c_data[index].event = I2C_EVENT_ERROR;
    ((void (*)())i2c_data[index].async_callback)();
}

/* TX handler */
static void i2c0_tx_irq(void)
{
    i2c_tx_irq(INTIICTEI0_IRQn, 0);
}

static void i2c1_tx_irq(void)
{
    i2c_tx_irq(INTIICTEI1_IRQn, 1);
}

static void i2c2_tx_irq(void)
{
    i2c_tx_irq(INTIICTEI2_IRQn, 2);
}

static void i2c3_tx_irq(void)
{
    i2c_tx_irq(INTIICTEI3_IRQn, 3);
}

/* RX handler */
static void i2c0_rx_irq(void)
{
    i2c_rx_irq(INTIICRI0_IRQn, 0);
}

static void i2c1_rx_irq(void)
{
    i2c_rx_irq(INTIICRI1_IRQn, 1);
}

static void i2c2_rx_irq(void)
{
    i2c_rx_irq(INTIICRI2_IRQn, 2);
}

static void i2c3_rx_irq(void)
{
    i2c_rx_irq(INTIICRI3_IRQn, 3);
}

/* Arbitration Lost handler */
static void i2c0_al_irq(void)
{
    i2c_err_irq(INTIICALI0_IRQn, 0);
}

static void i2c1_al_irq(void)
{
    i2c_err_irq(INTIICALI1_IRQn, 1);
}

static void i2c2_al_irq(void)
{
    i2c_err_irq(INTIICALI2_IRQn, 2);
}

static void i2c3_al_irq(void)
{
    i2c_err_irq(INTIICALI3_IRQn, 3);
}

/* Timeout handler */
static void i2c0_to_irq(void)
{
    i2c_err_irq(INTIICTMOI0_IRQn, 0);
}

static void i2c1_to_irq(void)
{
    i2c_err_irq(INTIICTMOI1_IRQn, 1);
}

static void i2c2_to_irq(void)
{
    i2c_err_irq(INTIICTMOI2_IRQn, 2);
}

static void i2c3_to_irq(void)
{
    i2c_err_irq(INTIICTMOI3_IRQn, 3);
}

static void i2c_irqs_set(i2c_t *obj, uint32_t enable)
{
    int i;
    const IRQn_Type *irqTable = irq_set_tbl[obj->i2c.i2c];
    const IRQHandler *handlerTable = hander_set_tbl[obj->i2c.i2c];
    for (i = 0; i < IRQ_NUM; ++i) {
        if (enable) {
            InterruptHandlerRegister(irqTable[i], handlerTable[i]);
            GIC_SetPriority(irqTable[i], 5);
            if (i == 1) {
                GIC_SetConfiguration(irqTable[i], 3);
            } else {
                GIC_SetConfiguration(irqTable[i], 1);
            }
            GIC_EnableIRQ(irqTable[i]);
        } else {
            GIC_DisableIRQ(irqTable[i]);
        }
    }
    REG(IER.UINT8[0]) = enable ? 0x63 : 0x00;
}

/******************************************************************************
 * ASYNCHRONOUS HAL
 ******************************************************************************/

void i2c_transfer_asynch(i2c_t *obj, const void *tx, size_t tx_length, void *rx, size_t rx_length, uint32_t address, uint32_t stop, uint32_t handler, uint32_t event, DMAUsage hint)
{
    MBED_ASSERT(obj);
    MBED_ASSERT(tx ? tx_length : 1);
    MBED_ASSERT(rx ? rx_length : 1);
    MBED_ASSERT((REG(SER.UINT32) & SER_SAR0E) == 0); /* Slave mode */
    
    obj->tx_buff.buffer = (void *)tx;
    obj->tx_buff.length = tx_length;
    obj->tx_buff.pos = 0;
    obj->tx_buff.width = 8;
    obj->rx_buff.buffer = rx;
    obj->rx_buff.length = rx_length;
    obj->rx_buff.pos = SIZE_MAX;
    obj->rx_buff.width = 8;
    i2c_data[obj->i2c.i2c].async_obj = obj;
    i2c_data[obj->i2c.i2c].async_callback = handler;
    i2c_data[obj->i2c.i2c].event = 0;
    i2c_data[obj->i2c.i2c].shouldStop = stop;
    i2c_data[obj->i2c.i2c].address = address;
    i2c_irqs_set(obj, 1);
    
    /* There is a STOP condition for last processing */
    if (obj->i2c.last_stop_flag != 0) {
        if (i2c_start(obj) != 0) {
            i2c_set_err_noslave(obj);
            i2c_data[obj->i2c.i2c].event = I2C_EVENT_ERROR | I2C_EVENT_ERROR_NO_SLAVE;
            i2c_abort_asynch(obj);
            ((void (*)())handler)();
            return;
        }
    }
    obj->i2c.last_stop_flag = stop;
    
    if (rx_length && tx_length == 0) {
        /* Ready to read */
        i2c_set_MR3_ACK(obj);
        
        /* Disable INTRIICTEI */
        REG(IER.UINT8[0]) &= ~(1 << 6);
        
        address |= 0x01;
    }
    /* Send Slave address */
    if (i2c_do_write(obj, address) != 0) {
        i2c_set_err_noslave(obj);
        i2c_data[obj->i2c.i2c].event = I2C_EVENT_ERROR | I2C_EVENT_ERROR_NO_SLAVE;
        i2c_abort_asynch(obj);
        ((void (*)())handler)();
        return;
    }
}

uint32_t i2c_irq_handler_asynch(i2c_t *obj)
{
    return i2c_data[obj->i2c.i2c].event;
}

uint8_t i2c_active(i2c_t *obj)
{
    return i2c_data[obj->i2c.i2c].async_obj != NULL;
}

void i2c_abort_asynch(i2c_t *obj)
{
    i2c_data[obj->i2c.i2c].async_obj = NULL;
    i2c_irqs_set(obj, 0);
    i2c_reg_reset(obj);
}

#endif
#endif  // #if DEVICE_I2C
