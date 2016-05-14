/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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

#include "device.h"
#include "buffer.h"
#include "dma_api.h"
#include "i2c_api.h"
#include "PeripheralPins.h"
#include "twi.h"
#include "pdc.h"
#include "mbed_assert.h"
#include "ioport.h"

/**
 * \defgroup GeneralI2C I2C Configuration Functions
 * @{
 */

/** TWI Bus Clock 400kHz */
extern uint8_t g_sys_init;

#define TWI_CLK    (400000u)

#define ADDR_LENGTH  0

#define MAX_I2C		8

extern uint32_t twi_mk_addr(const uint8_t *addr, int len);

void pinmap_find_i2c_info(Twi *sercombase, i2c_t *obj)
{
    if(sercombase==TWI0) {
        obj->i2c.flexcom=FLEXCOM0;
        obj->i2c.module_number=0;
        obj->i2c.pdc =PDC_TWI0;
        obj->i2c.irq_type=FLEXCOM0_IRQn;
    } else if(sercombase==TWI1) {
        obj->i2c.flexcom=FLEXCOM1;
        obj->i2c.module_number=1;
        obj->i2c.pdc =PDC_TWI1;
        obj->i2c.irq_type=FLEXCOM1_IRQn;
    } else if(sercombase==TWI2) {
        obj->i2c.flexcom=FLEXCOM2;
        obj->i2c.module_number=2;
        obj->i2c.pdc =PDC_TWI2;
        obj->i2c.irq_type=FLEXCOM2_IRQn;
    } else if(sercombase==TWI3) {
        obj->i2c.flexcom=FLEXCOM3;
        obj->i2c.module_number=3;
        obj->i2c.pdc =PDC_TWI3;
        obj->i2c.irq_type=FLEXCOM3_IRQn;
    } else if(sercombase==TWI4) {
        obj->i2c.flexcom=FLEXCOM4;
        obj->i2c.module_number=4;
        obj->i2c.pdc =PDC_TWI4;
        obj->i2c.irq_type=FLEXCOM4_IRQn;
    } else if(sercombase==TWI5) {
        obj->i2c.flexcom=FLEXCOM5;
        obj->i2c.module_number=5;
        obj->i2c.pdc =PDC_TWI5;
        obj->i2c.irq_type=FLEXCOM5_IRQn;
    } else if(sercombase==TWI6) {
        obj->i2c.flexcom=FLEXCOM6;
        obj->i2c.module_number=6;
        obj->i2c.pdc =PDC_TWI6;
        obj->i2c.irq_type=FLEXCOM6_IRQn;
    } else if(sercombase==TWI7) {
        obj->i2c.flexcom=FLEXCOM7;
        obj->i2c.module_number=7;
        obj->i2c.pdc =PDC_TWI7;
        obj->i2c.irq_type=FLEXCOM7_IRQn;
    } else {
        obj->i2c.flexcom=(Flexcom *)NC;
        obj->i2c.module_number=0;
        obj->i2c.pdc =(Pdc *) NC;
    }
}


/** Initialize the I2C peripheral. It sets the default parameters for I2C
 *  peripheral, and configure its specifieds pins.
 *  @param obj  The i2c object
 *  @param sda  The sda pin
 *  @param scl  The scl pin
 */
void i2c_init(i2c_t *obj, PinName sda, PinName scl)
{
    MBED_ASSERT(obj);
    MBED_ASSERT(sda !=NC && scl!=NC );

    if (g_sys_init == 0) {
        sysclk_init();
        board_init();
        g_sys_init = 1;
    }


    Twi* sda_base = (Twi*)pinmap_peripheral(sda, PinMap_I2C_SDA);
    Twi* scl_base = (Twi*)pinmap_peripheral(scl, PinMap_I2C_SCL);
    Twi* I2cBase = (Twi*)pinmap_merge((uint32_t)sda_base, (uint32_t)scl_base);

    MBED_ASSERT(I2cBase !=NC );

    obj->i2c.i2c_base=I2cBase;
    pinmap_find_i2c_info(I2cBase,obj);

    /* Configure I2C pins */
    pin_function(sda, pinmap_find_function(sda, PinMap_I2C_SDA));
    ioport_disable_pin(sda);

    pin_function(scl, pinmap_find_function(scl, PinMap_I2C_SCL));
    ioport_disable_pin(scl);

#if (SAMG55)
    /* Enable the peripheral and set TWI mode. */
    MBED_ASSERT((int)obj->i2c.flexcom!=NC);
    flexcom_enable(obj->i2c.flexcom);
    flexcom_set_opmode(obj->i2c.flexcom, FLEXCOM_TWI);
#else
    /* Enable the peripheral clock for TWI */
    pmc_enable_periph_clk(obj->i2c.i2c_base);
#endif

    twi_options_t twi_options;
    twi_options.master_clk=sysclk_get_cpu_hz();
    twi_options.speed=TWI_CLK;
    twi_options.smbus = 0;

    twi_master_init(obj->i2c.i2c_base,&twi_options);

    obj->i2c.is_slave=false;
    obj->i2c.speed=TWI_CLK;
    obj->i2c.master_clk=twi_options.master_clk;
}

/** Configure the I2C frequency.
 *  @param obj The i2c object
 *  @param hz  Frequency in Hz
 */
void i2c_frequency(i2c_t *obj, int hz)
{
    MBED_ASSERT(obj);
    if(obj->i2c.is_slave)
        twi_disable_slave_mode(obj->i2c.i2c_base);
    else
        twi_disable_master_mode(obj->i2c.i2c_base);

    twi_set_speed(obj->i2c.i2c_base,hz,obj->i2c.master_clk);

    if(obj->i2c.is_slave)
        twi_enable_slave_mode(obj->i2c.i2c_base);
    else
        twi_enable_master_mode(obj->i2c.i2c_base);
}

/** Send START command.
 *  @param obj The i2c object
 */
int  i2c_start(i2c_t *obj)
{
    MBED_ASSERT(obj);
    obj->i2c.i2c_base->TWI_CR = TWI_CR_START;
    return 0;
}

/** Send STOP command.
 *  @param obj The i2c object
 */
int  i2c_stop(i2c_t *obj)
{
    MBED_ASSERT(obj);
    obj->i2c.i2c_base->TWI_CR = TWI_CR_STOP;
    return 0;
}


uint32_t twi_master_read_no_stop(Twi *p_twi, twi_packet_t *p_packet, uint8_t stopena)
{
    uint32_t status;
    uint32_t cnt = p_packet->length;
    uint8_t *buffer = p_packet->buffer;
    uint8_t stop_sent = 0;
    uint32_t timeout = TWI_TIMEOUT;;

    /* Check argument */
    if (cnt == 0) {
        return TWI_INVALID_ARGUMENT;
    }

    /* Set read mode, slave address and 3 internal address byte lengths */
    p_twi->TWI_MMR = 0;
    p_twi->TWI_MMR = TWI_MMR_MREAD | TWI_MMR_DADR(p_packet->chip) |
                     ((p_packet->addr_length << TWI_MMR_IADRSZ_Pos) &
                      TWI_MMR_IADRSZ_Msk);

    /* Send a START condition */
    if ((cnt == 1) && (stopena == 1)) {
        p_twi->TWI_CR = TWI_CR_START | TWI_CR_STOP;
        stop_sent = 1;
    } else {
        p_twi->TWI_CR = TWI_CR_START;
        stop_sent = 0;
    }

    while (cnt > 0) {
        status = p_twi->TWI_SR;
        if (status & TWI_SR_NACK) {
            return TWI_RECEIVE_NACK;
        }

        if (!timeout--) {
            return TWI_ERROR_TIMEOUT;
        }

        /* Last byte ? */
        if ((cnt == 1)  && (!stop_sent) && (stopena == 1)) {
            p_twi->TWI_CR = TWI_CR_STOP;
            stop_sent = 1;
        }

        if (!(status & TWI_SR_RXRDY)) {
            continue;
        }
        *buffer++ = p_twi->TWI_RHR;

        cnt--;
        timeout = TWI_TIMEOUT;
    }
    if(stopena) {
        while (!(p_twi->TWI_SR & TWI_SR_TXCOMP)) {
        }
    }

    p_twi->TWI_SR;

    return TWI_SUCCESS;

}





/** Blocking reading data.
 *  @param obj     The i2c object
 *  @param address 7-bit address (last bit is 1)
 *  @param data    The buffer for receiving
 *  @param length  Number of bytes to read
 *  @param stop    Stop to be generated after the transfer is done
 *  @return Number of read bytes
 */
int  i2c_read(i2c_t *obj, int address, char *data, int length, int stop)
{
    MBED_ASSERT(obj);

    twi_packet_t packet;
    packet.chip= (address>>1) & 0x7F;
    packet.addr_length=ADDR_LENGTH;
    packet.buffer=data;
    packet.length=length;

    uint8_t status;
    status= twi_master_read_no_stop(obj->i2c.i2c_base, &packet, stop);

    if(TWI_SUCCESS==status)
        return length;

    return 0;
}


uint32_t twi_master_write_no_stop(Twi *p_twi, twi_packet_t *p_packet, uint8_t stopena)
{
    uint32_t status;
    uint32_t cnt = p_packet->length;
    uint8_t *buffer = p_packet->buffer;

    /* Check argument */
    if (cnt == 0) {
        return TWI_INVALID_ARGUMENT;
    }

    /* Set write mode, slave address and 3 internal address byte lengths */
    p_twi->TWI_MMR = 0;
    p_twi->TWI_MMR = TWI_MMR_DADR(p_packet->chip) |
                     ((p_packet->addr_length << TWI_MMR_IADRSZ_Pos) &
                      TWI_MMR_IADRSZ_Msk);

    /* Send a START condition */
    if ((cnt == 1) && (stopena == 1)) {
        p_twi->TWI_CR = TWI_CR_START | TWI_CR_STOP;
    } else {
        p_twi->TWI_CR = TWI_CR_START;
    }

    /* Send all bytes */
    while (cnt > 0) {
        status = p_twi->TWI_SR;
        if (status & TWI_SR_NACK) {
            return TWI_RECEIVE_NACK;
        }

        if (!(status & TWI_SR_TXRDY)) {
            continue;
        }
        p_twi->TWI_THR = *buffer++;

        cnt--;
    }

    while (1) {
        status = p_twi->TWI_SR;
        if (status & TWI_SR_NACK) {
            return TWI_RECEIVE_NACK;
        }

        if (status & TWI_SR_TXRDY) {
            break;
        }
    }

    if (stopena) {
        p_twi->TWI_CR = TWI_CR_STOP;
        while (!(p_twi->TWI_SR & TWI_SR_TXCOMP));
    }

    return TWI_SUCCESS;
}



/** Blocking sending data.
 *  @param obj     The i2c object
 *  @param address 7-bit address (last bit is 0)
 *  @param data    The buffer for sending
 *  @param length  Number of bytes to wrte
 *  @param stop    Stop to be generated after the transfer is done
 *  @return Number of written bytes
 */
int  i2c_write(i2c_t *obj, int address, const char *data, int length, int stop)
{
    MBED_ASSERT(obj);

    twi_packet_t packet;
    packet.chip= (address>>1) & 0x7F;
    packet.addr_length=ADDR_LENGTH;
    packet.buffer= (void *)data;
    packet.length=length;

    uint8_t status;
    status= twi_master_write_no_stop(obj->i2c.i2c_base,&packet, stop);

    if(TWI_SUCCESS==status)
        return length;

    return 0;
}

/** Reset I2C peripheral. TODO: The action here. Most of the implementation sends stop().
 *  @param obj The i2c object
 */
void i2c_reset(i2c_t *obj)
{
    MBED_ASSERT(obj);
    twi_reset(obj->i2c.i2c_base);
}

/** Read one byte.
 *  @param obj The i2c object
 *  @param last Acknoledge
 *  @return The read byte
 */
int  i2c_byte_read(i2c_t *obj, int last)
{
    MBED_ASSERT(obj);
    if(!last)
        twi_enable_slave_nack(obj->i2c.i2c_base);

    return twi_read_byte(obj->i2c.i2c_base);
}

/** Write one byte.
 *  @param obj The i2c object
 *  @param data Byte to be written
 *  @return 1 if NAK was received, 0 if ACK was received, 2 for timeout.
 */
#define ACK 0
#define NAK	1
#define TIMEOUT	2

int  i2c_byte_write(i2c_t *obj, int data)
{
    MBED_ASSERT(obj);
    twi_write_byte(obj->i2c.i2c_base,data);

    uint32_t timeout = TWI_TIMEOUT;
    while (timeout--) {
        uint32_t status = obj->i2c.i2c_base->TWI_SR;
        if (status & TWI_SR_NACK) {
            return NAK;
        }

        if (status & TWI_SR_TXRDY) {
            return ACK;
        }

        if (timeout<1) {
            return TIMEOUT;
        }
    }

    return ACK;
}

/**@}*/

#if DEVICE_I2CSLAVE

/**
 * \defgroup SynchI2C Synchronous I2C Hardware Abstraction Layer for slave
 * @{
 */

/** Configure I2C as slave or master.
 *  @param obj The I2C object
 *  @return non-zero if a value is available
 */
void i2c_slave_mode(i2c_t *obj, int enable_slave)
{
    MBED_ASSERT(obj);
    /* Disable TWI interrupts */
    obj->i2c.i2c_base->TWI_IDR = ~0UL;
    obj->i2c.i2c_base->TWI_SR;

    /* Reset TWI */
    twi_reset(obj->i2c.i2c_base);

    MBED_ASSERT(obj);
    if(enable_slave)
        twi_enable_slave_mode(obj->i2c.i2c_base);
    else
        twi_enable_master_mode(obj->i2c.i2c_base);
}

/** Check to see if the I2C slave has been addressed.
 *  @param obj The I2C object
 *  @return The status - 1 - read addresses, 2 - write to all slaves,
 *         3 write addressed, 0 - the slave has not been addressed
 */
int  i2c_slave_receive(i2c_t *obj)
{
    uint32_t status = obj->i2c.i2c_base->TWI_SR;
    if((status & TWI_SR_SVACC)) {
        if(status & TWI_SR_SVREAD)
            return 1;
        else
            return 3;
    }
    return 0;
}


uint32_t twi_slave_read_n(Twi *p_twi, uint8_t *p_data, int length)
{
    uint32_t status, cnt = 0;

    do {
        status = p_twi->TWI_SR;
        if (status & TWI_SR_SVACC) {
            if (!(status & (TWI_SR_GACC| TWI_SR_SVREAD )) &&
                    (status & TWI_SR_RXRDY)
               ) {
                *p_data++ = (uint8_t) p_twi->TWI_RHR;
                cnt++;
                if(cnt>=length) break;
            }
        } else if ((status & (TWI_SR_EOSACC | TWI_SR_TXCOMP))
                   == (TWI_SR_EOSACC | TWI_SR_TXCOMP)) {
            break;
        }
    } while (1);

    return cnt;
}

/** Read I2C slave.
 *  @param obj The I2C object
 *  @return non-zero if a value is available
 */
int  i2c_slave_read(i2c_t *obj, char *data, int length)
{
    MBED_ASSERT(obj);
    int read= twi_slave_read_n(obj->i2c.i2c_base,(uint8_t *)  data,length);
    return read;
}


uint32_t twi_slave_write_n(Twi *p_twi, uint8_t *p_data, int length)
{
    uint32_t status, cnt = 0;

    do {
        status = p_twi->TWI_SR;
        if (status & TWI_SR_SVACC) {
            if ((status & TWI_SR_SVREAD) && !(status & TWI_SR_GACC) &&
                    (status & TWI_SR_TXRDY)) {
                p_twi->TWI_THR = *p_data++;
                cnt++;
                if(cnt>=length) break;
            }
        } else if ((status & (TWI_SR_EOSACC | TWI_SR_TXCOMP))
                   == (TWI_SR_EOSACC | TWI_SR_TXCOMP)) {
            break;
        }
    } while (1);

    return cnt;
}


/** Write I2C as slave.
 *  @param obj The I2C object
 *  @return non-zero if a value is available
 */
int  i2c_slave_write(i2c_t *obj, const char *data, int length)
{
    MBED_ASSERT(obj);
    int write= twi_slave_write_n(obj->i2c.i2c_base, (uint8_t *) data,length);
    return write;
}

/** Configure I2C address.
 *  @param obj     The I2C object
 *  @param idx     Currently not used
 *  @param address The address to be set
 *  @param mask    Currently not used
 */
void i2c_slave_address(i2c_t *obj, int idx/*not used*/, uint32_t address, uint32_t mask)
{
    MBED_ASSERT(obj);
    twi_set_slave_addr(obj->i2c.i2c_base, (address>>1));
}

#endif

/**@}*/

#if DEVICE_I2C_ASYNCH

/**
 * \defgroup AsynchI2C Asynchronous I2C Hardware Abstraction Layer
 * @{
 */

/** Start i2c asynchronous transfer.
 *  @param obj       The I2C object
 *  @param tx        The buffer to send
 *  @param tx_length The number of words to transmit
 *  @param rx        The buffer to receive
 *  @param rx_length The number of words to receive
 *  @param address   The address to be set - 7bit or 9 bit
 *  @param stop      If true, stop will be generated after the transfer is done
 *  @param handler   The I2C IRQ handler to be set
 *  @param hint      DMA hint usage
 */
#warning "Only DMA async supported by I2C master transfer"

void i2c_transfer_asynch(i2c_t *obj, const void *tx, size_t tx_length, void *rx, size_t rx_length, uint32_t address, uint32_t stop, uint32_t handler, uint32_t event, DMAUsage hint /*Not Used*/)
{
    uint32_t pdcenable=0;

    if(address) {
        twi_packet_t pdc_packet;
        pdc_packet.chip=(address>>1) & 0x7F;
        pdc_packet.addr_length=ADDR_LENGTH;

        /* Set write mode, slave address and 3 internal address byte lengths */
        obj->i2c.i2c_base->TWI_MMR = 0;
        obj->i2c.i2c_base->TWI_MMR = TWI_MMR_DADR(pdc_packet.chip) |
                                     ((pdc_packet.addr_length << TWI_MMR_IADRSZ_Pos) &
                                      TWI_MMR_IADRSZ_Msk);
    }

    if(tx) {
        pdc_packet_t pdc_packet_tx;
        pdc_packet_tx.ul_addr=(uint32_t)tx;
        pdc_packet_tx.ul_size=tx_length;

        pdcenable|=PERIPH_PTCR_TXTEN;
        /* Configure PDC for data send */
        pdc_tx_init(obj->i2c.pdc, &pdc_packet_tx, NULL);
    }

    if(rx) {
        obj->i2c.i2c_base->TWI_MMR |= TWI_MMR_MREAD;
        pdc_rx_clear_cnt(obj->i2c.pdc);
        pdc_packet_t pdc_packet_rx;
        pdc_packet_rx.ul_addr=(uint32_t)rx;
        pdc_packet_rx.ul_size=rx_length;
        pdcenable|=PERIPH_PTCR_RXTEN;

        /* Configure PDC for data receive */
        pdc_rx_init(obj->i2c.pdc, &pdc_packet_rx, NULL);
    }

    obj->i2c.dma_usage=hint;
    obj->i2c.event=event;
    obj->i2c.stop=stop;
    obj->i2c.address=address;

    NVIC_ClearPendingIRQ(obj->i2c.irq_type);
    NVIC_DisableIRQ(obj->i2c.irq_type);
    NVIC_SetVector(obj->i2c.irq_type,handler);
    NVIC_EnableIRQ(obj->i2c.irq_type);

    /* Enable TWI IRQ */
    twi_enable_interrupt(obj->i2c.i2c_base, TWI_IER_RXBUFF| TWI_IER_TXBUFE | TWI_IER_UNRE | TWI_IER_OVRE | TWI_IER_PECERR);

    /* Enable PDC transfers */
    pdc_enable_transfer(obj->i2c.pdc, pdcenable );

}

/** The asynchronous IRQ handler
 *  @param obj The I2C object which holds the transfer information
 *  @return event flags if a transfer termination condition was met or 0 otherwise.
 */
uint32_t i2c_irq_handler_asynch(i2c_t *obj)
{
    uint32_t event=0;

    if(obj->i2c.stop) {
        i2c_stop(obj);
    }

    // Data transferred via DMA
    if((obj->i2c.i2c_base->TWI_SR & TWI_IER_TXBUFE)) {
	    twi_disable_interrupt(obj->i2c.i2c_base, TWI_IDR_TXBUFE | TWI_IDR_UNRE | TWI_IDR_OVRE | TWI_IDR_PECERR);		
        if(obj->i2c.event | I2C_EVENT_TRANSFER_COMPLETE)
            event |=I2C_EVENT_TRANSFER_COMPLETE;
    }
	
    if((obj->i2c.i2c_base->TWI_SR & TWI_IER_RXBUFF)) {
	    twi_disable_interrupt(obj->i2c.i2c_base, TWI_IDR_RXBUFF | TWI_IDR_UNRE | TWI_IDR_OVRE | TWI_IDR_PECERR);
	    if(obj->i2c.event | I2C_EVENT_TRANSFER_COMPLETE)
	    event |=I2C_EVENT_TRANSFER_COMPLETE;
    }
    
    if(obj->i2c.i2c_base->TWI_SR & TWI_IER_NACK) {
        if(obj->i2c.event | I2C_EVENT_TRANSFER_EARLY_NACK)
            event |=I2C_EVENT_TRANSFER_EARLY_NACK;
    }


    if((obj->i2c.i2c_base->TWI_SR & TWI_IER_UNRE) || (obj->i2c.i2c_base->TWI_SR & TWI_IER_OVRE) || (obj->i2c.i2c_base->TWI_SR & TWI_IER_PECERR) || (obj->i2c.i2c_base->TWI_SR & TWI_SR_TOUT) ) {
        if((obj->i2c.event | I2C_EVENT_ERROR))
            event |=I2C_EVENT_ERROR;

        if(obj->i2c.address) {
            uint8_t status= twi_probe(obj->i2c.i2c_base,obj->i2c.address);
            if((obj->i2c.event | I2C_EVENT_ERROR_NO_SLAVE) && (status!=TWI_SUCCESS) )
                event |=I2C_EVENT_ERROR_NO_SLAVE;
        }
    }

    return event;
}

/** Attempts to determine if I2C peripheral is already in use.
 *  @param obj The I2C object
 *  @return non-zero if the I2C module is active or zero if it is not
 */
uint8_t i2c_active(i2c_t *obj)
{

    if(obj->i2c.i2c_base->TWI_SR & TWI_SR_ENDTX && obj->i2c.i2c_base->TWI_SR & TWI_SR_ENDRX)
        return 0;

    return 1;

}

/** Abort ongoing asynchronous transaction.
 *  @param obj The I2C object
 */
void i2c_abort_asynch(i2c_t *obj)
{
    /* Disable PDC transfers */
    pdc_disable_transfer(obj->i2c.pdc, PERIPH_PTCR_RXTDIS | PERIPH_PTCR_TXTDIS);

    /* Clear PDC buffer receive counter */
    pdc_rx_clear_cnt(obj->i2c.pdc);

    /* Disable I2C IRQ */
    twi_disable_interrupt(obj->i2c.i2c_base, TWI_IDR_TXBUFE);
    twi_disable_interrupt(obj->i2c.i2c_base, TWI_IDR_RXBUFF);

    /* Disable I2C interrupt */
    NVIC_DisableIRQ(obj->i2c.irq_type);
}

#endif