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
#include "mbed_assert.h"
#include "i2c_api.h"
#include "cmsis.h"
#include "pinmap.h"


#include "riic_iodefine.h"
#include "RZ_A1_Init.h"
#include "MBRZA1H.h"

volatile struct st_riic *RIIC[] = RIIC_ADDRESS_LIST;

#define REG(N) \
    RIIC[obj->i2c]->RIICn##N

#define NACKF (1 << 4)

static const PinMap PinMap_I2C_SDA[] = {
    {P1_1 , I2C_0, 1},
    {P1_3 , I2C_1, 1},
    {P1_7 , I2C_3, 1},
    {NC   , NC   , 0}
};

static const PinMap PinMap_I2C_SCL[] = {
    {P1_0 , I2C_0, 1},
    {P1_2 , I2C_1, 1},
    {P1_6 , I2C_3, 1},
    {NC   , NC,    0}
};

// Clear the Transmit data Empty TDRE
static inline int i2c_addressed(i2c_t *obj) {
    volatile int sar0 = (REG(SR1.UINT8[0])&1),
                 trs  = (REG(CR2.UINT8[0])&0x20) >> 5;
    return sar0 | (trs <<1);
}

static inline int i2c_status(i2c_t *obj) {
    return REG(SR2.UINT8[0]);
}

static inline void i2c_clear_TDRE(i2c_t *obj) {
    REG(SR2.UINT32) &= ~(1 << 7);
}

static inline void i2c_wait_RDRF(i2c_t *obj) {
    while (!(i2c_status(obj) & (1 << 5))) ;
}

static void i2c_reg_reset(i2c_t *obj) {
    // full reset
    REG(CR1.UINT8[0]) &= ~(1 << 7); // CR1.ICE off
    REG(CR1.UINT8[0]) |=  (1 << 6); // CR1.IICRST on
    REG(CR1.UINT8[0]) |=  (1 << 7); // CR1.ICE on

    REG(MR1.UINT8[0])  =  0x08;  // P_phi /8  9bit (including Ack)
    REG(SER.UINT8[0])  =  0x00;  // no slave addr enabled

    // set frequency
    REG(MR1.UINT8[0]) |=  obj->pclk_bit;
    REG(BRL.UINT32)    =  obj->width;
    REG(BRH.UINT32)    =  obj->width;

    REG(MR2.UINT8[0])  =  0x07;
    REG(MR3.UINT8[0])  =  0x00;

    REG(FER.UINT8[0])  =  0x72;  // SCLE, NFE enabled, TMOT
    REG(IER.UINT8[0])  =  0x00;  // no interrupt

    REG(CR1.UINT32) &= ~(1 << 6); // CR1.IICRST negate reset
}

// Wait until the Trans Data Empty (TDRE) is set
static int i2c_wait_TDRE(i2c_t *obj) {
    int timeout = 0;

    while (!(i2c_status(obj) & (1 << 7))) {
        timeout ++;
        if (timeout > 100000) return -1;
    }

    return 0;
}

static inline int i2c_wait_TEND(i2c_t *obj) {
    int timeout = 0;

    while (!(i2c_status(obj) & (1 << 6))) {
        timeout ++;
        if (timeout > 100000) return -1;
    }

    return 0;
}


static int i2c_wait_STOP(i2c_t *obj) {
    volatile uint32_t work_reg;

    /* wait SR2.STOP = 1 */
    work_reg = REG(SR2.UINT32);
    while ((work_reg & (1 << 3)) == 0) {
        work_reg = REG(SR2.UINT32);
    }
    /* SR2.NACKF = 0 */
    REG(SR2.UINT32) &= ~(1 << 4);
    /* SR2.STOP  = 0 */
    REG(SR2.UINT32) &= ~(1 << 3);
    
    return 0;
}


static inline void i2c_power_enable(i2c_t *obj) {
    volatile uint8_t dummy;
    switch ((int)obj->i2c) {
        case I2C_0: CPGSTBCR9 &= ~(0x80); break;
        case I2C_1: CPGSTBCR9 &= ~(0x40); break;
        case I2C_2: CPGSTBCR9 &= ~(0x20); break;
        case I2C_3: CPGSTBCR9 &= ~(0x10); break;
    }
    dummy = CPGSTBCR9;
}

void i2c_init(i2c_t *obj, PinName sda, PinName scl) {
    // determine the SPI to use
    I2CName i2c_sda = (I2CName)pinmap_peripheral(sda, PinMap_I2C_SDA);
    I2CName i2c_scl = (I2CName)pinmap_peripheral(scl, PinMap_I2C_SCL);
    obj->i2c = pinmap_merge(i2c_sda, i2c_scl);
    MBED_ASSERT((int)obj->i2c != NC);

    // enable power
    i2c_power_enable(obj);

    // set default frequency at 100k
    i2c_frequency(obj, 100000);

    // full reset
    i2c_reg_reset(obj);

    pinmap_pinout(sda, PinMap_I2C_SDA);
    pinmap_pinout(scl, PinMap_I2C_SCL);
}

inline int i2c_start(i2c_t *obj) {
    if (REG(CR2.UINT32) & (1 << 7)) { // BBSY check
        return 0xff;
    }
    REG(CR2.UINT8[0]) |= 0x02; // start

    return 0x10;
}

inline int i2c_stop(i2c_t *obj) {
    /* SR2.STOP  = 0 */
    REG(SR2.UINT32) &= ~(1 << 3);
    // write the stop bit
    REG(CR2.UINT32) |= (1 << 3);

    return 0;
}

static inline int i2c_do_write(i2c_t *obj, int value) {
    // write the data
    if (!(i2c_status(obj) & NACKF)) { // NACF=0
        i2c_wait_TDRE(obj);
        REG(DRT.UINT32) = value;
    }  else {
        return 0xff;
    }
    return i2c_status(obj);
}

static inline int i2c_do_read(i2c_t *obj, int last) {
    if (obj->dummy) {
        volatile int dummy = REG(DRR.UINT32);
        obj->dummy = 0;
    }

    // wait for it to arrive
    i2c_wait_RDRF(obj);

    if (last == 2) {
        /* this time is befor last byte read */
        /* Set MR3 WATI bit is 1 */;
        REG(MR3.UINT32) |= (1 << 6);
    } else if (last == 1) {
        // send a NOT ACK
        REG(MR3.UINT32) |= (1 <<4);
        REG(MR3.UINT32) |=  (1 <<3);
        REG(MR3.UINT32) &= ~(1 <<4);
    } else {
        // send a ACK
        REG(MR3.UINT32) |= (1 <<4);
        REG(MR3.UINT32) &= ~(1 <<3);
        REG(MR3.UINT32) &= ~(1 <<4);
    }

    // return the data
    return (REG(DRR.UINT32) & 0xFF);
}

void i2c_frequency(i2c_t *obj, int hz) {
    int freq;
    int oldfreq = 0;
    int newfreq = 0;
    uint32_t pclk;
    uint32_t pclk_base;
    uint32_t tmp_width;
    uint32_t width = 0;
    uint8_t count;
    uint8_t pclk_bit = 0;

    /* set PCLK */
    if (false == RZ_A1_IsClockMode0()) {
        pclk_base = (uint32_t)CM1_RENESAS_RZ_A1_P0_CLK;
    } else {
        pclk_base = (uint32_t)CM0_RENESAS_RZ_A1_P0_CLK;
    }

    /* Min 10kHz, Max 400kHz */
    if (hz < 10000) {
        freq = 10000;
    } else if (hz > 400000) {
        freq = 400000;
    } else {
        freq = hz;
    }

    for (count = 0; count < 7; count++) {
        // IIC phi = P0 phi / rate
        pclk = pclk_base / (2 << count);
        // In case of "CLE = 1, NFE = 1, CKS != 000( IIC phi < P0 phi ), nf = 1"
        // freq = 1 / {[( BRH + 2 + 1 ) + ( BRL + 2 + 1 )] / pclk }
        // BRH is regarded as same value with BRL
        // 2( BRH + 3 ) / pclk  = 1 / freq
        tmp_width = ((pclk / freq) / 2) - 3;
        // Carry in a decimal point
        tmp_width += 1;
        if ((tmp_width >= 0x00000001) && (tmp_width <= 0x0000001F)) {
            // Calculate theoretical value, and Choose max value of them
            newfreq = pclk / (tmp_width + 3) / 2;
            if (newfreq >= oldfreq) {
                oldfreq  = newfreq;
                width    = tmp_width;
                pclk_bit = (uint8_t)(0x10 * (count + 1));
            }
        }
    }

    if (width != 0) {
        // I2C Rate
        obj->pclk_bit = pclk_bit;  // P_phi / xx
        obj->width    = (width | 0x000000E0);
    } else {
        // Default 
        obj->pclk_bit = 0x00;      // P_phi / 1
        obj->width    = 0x000000FF;
    }
}

int i2c_read(i2c_t *obj, int address, char *data, int length, int stop) {
    int count = 0;
    int status;
    int value;
    volatile uint32_t work_reg = 0;

    // full reset
    i2c_reg_reset(obj);
    obj->dummy = 1;
    
    status = i2c_start(obj);

    if (status == 0xff) {
        i2c_stop(obj);
        i2c_wait_STOP(obj);
        return I2C_ERROR_BUS_BUSY;
    }

    status = i2c_do_write(obj, (address | 0x01));
    if (status & 0x01) {
        i2c_stop(obj);
        i2c_wait_STOP(obj);
        return I2C_ERROR_NO_SLAVE;
    }
    
    /* wati RDRF */
    i2c_wait_RDRF(obj);
    /* check ACK/NACK */
    if ((REG(SR2.UINT32) & (1 << 4) == 1)) {
        /* Slave sends NACK */
        i2c_stop(obj);
        // dummy read
        value = REG(DRR.UINT32);
        i2c_wait_STOP(obj);
        return I2C_ERROR_NO_SLAVE;
    }
    
    // Read in all except last byte
    if (length > 2) {
        for (count = 0; count < (length - 1); count++) {
            if (count == (length - 2)) {
                value = i2c_do_read(obj, 1);
            } else if ((length >= 3) && (count == (length - 3))) {
                value = i2c_do_read(obj, 2);
            } else {
                value = i2c_do_read(obj, 0);
            }
            status = i2c_status(obj);
            if (status & 0x10) {
                i2c_stop(obj);
                i2c_wait_STOP(obj);
                return count;
            }
            data[count] = (char) value;
        }
    } else if (length == 2) {
        /* Set MR3 WATI bit is 1 */;
        REG(MR3.UINT32) |= (1 << 6);
        // dummy read
        value = REG(DRR.UINT32);
        // wait for it to arrive
        i2c_wait_RDRF(obj);
        // send a NOT ACK
        REG(MR3.UINT32) |= (1 <<4);
        REG(MR3.UINT32) |=  (1 <<3);
        REG(MR3.UINT32) &= ~(1 <<4);
        data[count] = (char)REG(DRR.UINT32);
        count++;
    } else if (length == 1) {
        /* Set MR3 WATI bit is 1 */;
        REG(MR3.UINT32) |= (1 << 6);
        // send a NOT ACK
        REG(MR3.UINT32) |= (1 <<4);
        REG(MR3.UINT32) |=  (1 <<3);
        REG(MR3.UINT32) &= ~(1 <<4);
        // dummy read
        value = REG(DRR.UINT32);
    } else {
        // Do Nothing
    }

    // read in last byte
    i2c_wait_RDRF(obj);
    // If not repeated start, send stop.
    if (stop) {
        /* RIICnSR2.STOP = 0 */
        REG(SR2.UINT32) &= ~(1 << 3);
        /* RIICnCR2.SP   = 1 */
        REG(CR2.UINT32) |= (1 << 3);
        /* RIICnDRR read */
        value = REG(DRR.UINT32) & 0xFF;
        data[count] = (char) value;
        /* RIICnMR3.WAIT = 0 */
        REG(MR3.UINT32) &= ~(1 << 6);
        i2c_wait_STOP(obj);
    } else {
        /* RIICnDRR read */
        value = REG(DRR.UINT32) & 0xFF;
        data[count] = (char) value;
    }

    return length;
}

int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop) {
    int i, status;

    // full reset
    i2c_reg_reset(obj);

    status = i2c_start(obj);

    if ((status == 0xff)) {
        i2c_stop(obj);
        i2c_wait_STOP(obj);
        return I2C_ERROR_BUS_BUSY;
    }
    
    /**/
    status = REG(CR2.UINT32);
    status = REG(SR2.UINT32);
    /**/

    status = i2c_do_write(obj, address);
    if (status & 0x10) {
        i2c_stop(obj);
        i2c_wait_STOP(obj);
        return I2C_ERROR_NO_SLAVE;
    }

    /**/
    status = REG(CR2.UINT32);
    status = REG(SR2.UINT32);
    /**/
    for (i=0; i<length; i++) {
    /**/
    status = REG(CR2.UINT32);
    status = REG(SR2.UINT32);
    /**/
        status = i2c_do_write(obj, data[i]);
        if(status & 0x10) {
            i2c_stop(obj);
            i2c_wait_STOP(obj);
            return i;
        }
    }

    i2c_wait_TEND(obj);

    // If not repeated start, send stop.
    if (stop) {
        i2c_stop(obj);
        i2c_wait_STOP(obj);
    }

    return length;
}

void i2c_reset(i2c_t *obj) {
    i2c_stop(obj);
    i2c_wait_STOP(obj);
}

int i2c_byte_read(i2c_t *obj, int last) {
    obj->dummy = 1;
    
    return (i2c_do_read(obj, last) & 0xFF);
}

int i2c_byte_write(i2c_t *obj, int data) {
    int ack;
    int status = i2c_do_write(obj, (data & 0xFF));
    if (status & NACKF) {
        ack = 0;
    } else {
        ack = 1;
    }

    return ack;
}

void i2c_slave_mode(i2c_t *obj, int enable_slave) {
    if (enable_slave != 0) {
        REG(SER.UINT32)  =  0x01;  // only slave addr 1 is enabled
    } else {
        REG(SER.UINT32)  =  0x00;  // no slave addr enabled
    }
}

int i2c_slave_receive(i2c_t *obj) {
    int status;
    int retval;

    status = i2c_addressed(obj);
    switch(status) {
        case 0x3: retval = 1; break;
        case 0x2: retval = 2; break;
        case 0x1: retval = 3; break;
        default : retval = 1; break;
    }

    return(retval);
}

int i2c_slave_read(i2c_t *obj, char *data, int length) {
    int count = 0;
    int status;

    volatile int dummy = REG(DRR.UINT32) ;

    do {
        i2c_wait_RDRF(obj);
        status = i2c_status(obj);
        if(!(status & 0x10)) {
            data[count] = REG(DRR.UINT32) & 0xFF;
        }
        count++;
    } while ( !(status & 0x10)  && (count < length) );

    if(status & 0x10) {
        i2c_stop(obj);
        i2c_wait_STOP(obj);
    }

    //i2c_clear_TDRE(obj);

    return count;
}

int i2c_slave_write(i2c_t *obj, const char *data, int length) {
    int count = 0;
    int status;

    if(length <= 0) {
        return(0);
    }

    do {
        status = i2c_do_write(obj, data[count]);
        count++;
    } while ((count < length) && !(status & 0x10));

    if (!(status & 0x10)) {
        i2c_stop(obj);
        i2c_wait_STOP(obj);
    }

    i2c_clear_TDRE(obj);

    return(count);
}

void i2c_slave_address(i2c_t *obj, int idx, uint32_t address, uint32_t mask) {
    REG(SAR0.UINT32) = address & 0xfe;
}
