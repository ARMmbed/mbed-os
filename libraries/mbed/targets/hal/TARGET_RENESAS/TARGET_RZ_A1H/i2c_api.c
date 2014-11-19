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

    // set default frequency at 100k
    i2c_frequency(obj, 100000);

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
    obj->dummy = 1;
    MBED_ASSERT((int)obj->i2c != NC);

    // enable power
    i2c_power_enable(obj);

    // full reset
    i2c_reg_reset(obj);

    pinmap_pinout(sda, PinMap_I2C_SDA);
    pinmap_pinout(scl, PinMap_I2C_SCL);
}

inline int i2c_start(i2c_t *obj) {
    if (REG(CR2.UINT32) & (1 << 7)) { // BBSY check
        return 0xff;
    }
    REG(CR2.UINT8[0]) |= 0x62; // start

    return 0x10;
}

inline int i2c_stop(i2c_t *obj) {
    volatile int timeout = 0;

    // write the stop bit
    REG(CR2.UINT32) |= (1 << 3);

    // wait for SP bit to reset
    while(REG(CR2.UINT32) & (1 << 3)) {
        timeout ++;
        if (timeout > 100000) return 1;
    }

    obj->dummy = 1;
    REG(CR2.UINT32) &= ~ (1 << 3);
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
        REG(MR3.UINT32) |=  (1 <<3);
    } else {
        // send a ACK
        REG(MR3.UINT32) &= ~(1 <<3);
    }

    // return the data
    return (REG(DRR.UINT32) & 0xFF);
}

void i2c_frequency(i2c_t *obj, int hz) {
    uint32_t PCLK = 6666666;

    uint32_t pulse = PCLK / (hz * 2);

    // I2C Rate
    REG(BRL.UINT32) = pulse;
    REG(BRH.UINT32) = pulse;
}

int i2c_read(i2c_t *obj, int address, char *data, int length, int stop) {
    int count = 0;
    int status;
    int value;
    volatile uint32_t work_reg = 0;


    // full reset
    i2c_reg_reset(obj);

    status = i2c_start(obj);

    if (status == 0xff) {
        i2c_stop(obj);
        return I2C_ERROR_BUS_BUSY;
    }

    status = i2c_do_write(obj, (address | 0x01));
    if (status & 0x01) {
        i2c_stop(obj);
        return I2C_ERROR_NO_SLAVE;
    }
    
    /* wati RDRF */
    i2c_wait_RDRF(obj);
    /* check ACK/NACK */
    if ((REG(SR2.UINT32) & (1 << 4) == 1)) {
        /* Slave sends NACK */
        i2c_stop(obj);
        return I2C_ERROR_NO_SLAVE;
    }
    
    // Read in all except last byte
    if (length > 1) {
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
                return count;
            }
            data[count] = (char) value;
        }
    }

    // read in last byte
    i2c_wait_RDRF(obj);
    /* RIICnSR2.STOP = 0 */
    REG(SR2.UINT32) &= ~(1 << 3);
    /* RIICnCR2.SP   = 1 */
    REG(CR2.UINT32) |= (1 << 3);
    /* RIICnDRR read */
    value = REG(DRR.UINT32) & 0xFF;
    /* RIICnMR3.WAIT = 0 */
    REG(MR3.UINT32) &= ~(1 << 6);
    /* wait SR2.STOP = 1 */
    while ((work_reg & (1 << 3)) == (1 << 3)) {
        work_reg = REG(SR2.UINT32);
    }
    /* SR2.NACKF = 0 */
    REG(SR2.UINT32) &= ~(1 << 4);
    /* SR2.STOP  = 0 */
    REG(SR2.UINT32) &= ~(1 << 3);
    status = i2c_status(obj);
    if (status & 0x10) {
        i2c_stop(obj);
        return length - 1;
    }

    data[count] = (char) value;

    // If not repeated start, send stop.
    if (stop) {
        i2c_stop(obj);
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
        return I2C_ERROR_BUS_BUSY;
    }

    status = i2c_do_write(obj, address);
    if (status & 0x10) {
        i2c_stop(obj);
        return I2C_ERROR_NO_SLAVE;
    }

    for (i=0; i<length; i++) {
        status = i2c_do_write(obj, data[i]);
        if(status & 0x10) {
            i2c_stop(obj);
            return i;
        }
    }

    i2c_wait_TEND(obj);

    // If not repeated start, send stop.
    if (stop) {
        i2c_stop(obj);
    }

    return length;
}

void i2c_reset(i2c_t *obj) {
    i2c_stop(obj);
}

int i2c_byte_read(i2c_t *obj, int last) {
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

    if (obj->dummy) {
        volatile int dummy = REG(DRR.UINT32) ;
        obj->dummy = 0;
    }

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
    }

    i2c_clear_TDRE(obj);

    return(count);
}

void i2c_slave_address(i2c_t *obj, int idx, uint32_t address, uint32_t mask) {
    REG(SAR0.UINT32) = address & 0xfe;
}
