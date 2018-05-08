/* mbed Microcontroller Library
 * Copyright (c) 2016 u-blox
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

//#if DEVICE_I2C

/* reference frequency in Hz*/
#define REF_FREQ   (26000000UL)

/** Macro generating the mask for a bitfield of \p n bits */
#define DRIVER_BIT_SET(n)                      (1u<<(n))

#define DRIVER_BITS_CLR(data, mask)            ((data) & ~(mask))
#define DRIVER_BITS_SET(data, bits)            ((data) |  (bits))

/** Macro generating the mask for a bitfield of \p n bits */
#define DRIVER_BIT_MASK(n)                      (DRIVER_BIT_SET(n) - 1)

/** Macro generating the mask for a bitfield defined as `name_OFFSET` and `name_SIZE` */
#define DRIVER_BITFIELD_MASK_(name)             (DRIVER_BIT_MASK(name##_SIZE) << (name##_OFFSET))
#define DRIVER_BITFIELD_MASK(name)              DRIVER_BITFIELD_MASK_(name)

/** Extract bitfield defined as `name_OFFSET` and `name_SIZE` from \p data */
#define DRIVER_BITFIELD_GET_(data, name)        (((data) >> name##_OFFSET) & DRIVER_BIT_MASK(name##_SIZE))
#define DRIVER_BITFIELD_GET(data, name)         DRIVER_BITFIELD_GET_(data, name)

/** Return \p data with bitfield defined as `name_OFFSET` and `name_SIZE` cleared */
#define DRIVER_BITFIELD_CLR(data, name)         ((data) & ~DRIVER_BITFIELD_MASK(name))

/** Return \p bitfield defined as `name_OFFSET` and `name_SIZE` set to \p value */
#define DRIVER_BITFIELD_VAL_(name, value)       (((value) & DRIVER_BIT_MASK(name##_SIZE)) << name##_OFFSET)
#define DRIVER_BITFIELD_VAL(name, value)        DRIVER_BITFIELD_VAL_(name, value)

/** Return \p data with bitfield defined as `name_OFFSET` and `name_SIZE` set to \p value */
#define DRIVER_BITFIELD_SET(data, name, value)  (DRIVER_BITFIELD_CLR(data, name) | DRIVER_BITFIELD_VAL(name, value))

/** Return \p bitfield defined as `name_OFFSET` and `name_SIZE` set to \p name_ENUM_value */
#define DRIVER_BITFIELD_ENUM_(name, enumValue)      DRIVER_BITFIELD_VAL(name, name##_ENUM_##enumValue)
#define DRIVER_BITFIELD_ENUM(name, enumValue)       DRIVER_BITFIELD_ENUM_(name, enumValue)

/** Return \p data with bitfield defined as `name_OFFSET` and `name_SIZE` set to \p name_ENUM_value */
#define DRIVER_BITFIELD_SET_ENUM(data, name, enumValue)  DRIVER_BITFIELD_SET(data, name,  DRIVER_BITFIELD_ENUM(name, enumValue))

#define FIFO_SIZE   (16)

#define MAX_TIMEOUT (0xFFFF) //TODO: this large value is for testing only


//PIO channels used by I2C0 bus
#define I2C1SclxCIO_CHANNEL    (50)
#define I2C1SdaxSIO_CHANNEL    (51)
#define I2C2SclxCIO_CHANNEL    (52)
#define I2C2SdaxSIO_CHANNEL    (53)

#define ERROR_BITS  (DRIVER_BITFIELD_MASK(I2C_SR_BERR) | DRIVER_BITFIELD_MASK(I2C_SR_ARLO) | DRIVER_BITFIELD_MASK(I2C_SR_AF))

int is_bus_busy(i2c_t *obj)
{
    uint32_t timeout = MAX_TIMEOUT;

    while((1 == DRIVER_BITFIELD_GET(obj->reg_base->sr, I2C_SR_BUSY)) && (timeout > 0))
    {
        timeout--;
    }

    if(1 == DRIVER_BITFIELD_GET(obj->reg_base->sr, I2C_SR_BUSY) )
    {
        return true;
    }
    else
    {
        return false;
    }
}

static void config_pio_channel(uint8_t channel)
{
    struct pio_s *pio_channel_regbase;
    uint8_t channel_offset_in_reg;

    channel_offset_in_reg = channel & PIO_CHANNEL_SUB_32_MASK;

    if( (channel >> PIO_CHANNEL_OVER_32_SHIFT) == 0) {
        pio_channel_regbase = (struct pio_s *)PIO_CONTROL_BASE;
    } else {
        pio_channel_regbase = (struct pio_s *)(PIO_CONTROL_BASE + 0x220);
    }

    //TODO: PIO configuration is not yet confirmed as I2C on FPGA is directly routed to pin so no PIO involved
    //pio_channel_regbase->pio_pdr_0 = (1 << channel_offset_in_reg);     //pio disable
    //pio_channel_regbase->pio_odr_0  = (1 << channel_offset_in_reg);    //output driver disable
    //pio_channel_regbase->pio_pldr_0 = (1 << channel_offset_in_reg);    //pad config: pulldown disabled
    //pio_channel_regbase->pio_phdr_0 = (1 << channel_offset_in_reg);    //pad config: pullup disabled
    //pio_channel_regbase->pio_percper_0 = (1 << channel_offset_in_reg); //periph pullup/down disable
    //pio_channel_regbase->pio_iner_0 = (1 << channel_offset_in_reg);    //input enable register
    //pio_channel_regbase->pio_ster_0 = (1 << channel_offset_in_reg);    //schmitt trigger disable
    //pio_channel_regbase->pio_mdsdr_0 = (1 << channel_offset_in_reg);   //msb drive strength disable
    //pio_channel_regbase->pio_ldsdr_0 = (1 << channel_offset_in_reg);   //lsb drive strength disable
    //pio_channel_regbase->pio_ifer_0 = (1 << channel_offset_in_reg);    //input filter disable
    //pio_channel_regbase->pio_mddr_0 = (1 << channel_offset_in_reg);    //multidriver disable
    pio_channel_regbase->pio_asr_0 |= (1 << channel_offset_in_reg);    //mux0 enable

}

static void disable_pio_channel(uint8_t channel)
{
    struct pio_s *pio_channel_regbase;
    uint8_t channel_offset_in_reg;

    channel_offset_in_reg = channel & PIO_CHANNEL_SUB_32_MASK;

    if( (channel >> PIO_CHANNEL_OVER_32_SHIFT) == 0) {
        pio_channel_regbase = (struct pio_s *)PIO_CONTROL_BASE;
    } else {
        pio_channel_regbase = (struct pio_s *)(PIO_CONTROL_BASE + 0x220);
    }
    pio_channel_regbase->pio_pdr_0 = (1 << channel_offset_in_reg);     //pio disable
}

void i2c_init(i2c_t *obj, PinName sda, PinName scl)
{
    obj->i2c_sda_pin = sda;
    obj->i2c_scl_pin = scl;

    if (obj->i2c_sda_pin == I2C1_SDA)
    {
        config_pio_channel(I2C1SdaxSIO_CHANNEL);
        config_pio_channel(I2C1SclxCIO_CHANNEL);
        obj->reg_base = app_ss_i2c1;
    }
    else if (obj->i2c_sda_pin == I2C2_SDA)
    {
        config_pio_channel(I2C2SdaxSIO_CHANNEL);
        config_pio_channel(I2C2SclxCIO_CHANNEL);
        obj->reg_base = app_ss_i2c2;
    }
    /*else if (obj->i2c_sda_pin == I2C3_SDA) //TODO: I2C3 is connected to ACC, have to remove all I2C3 related code in final version
    {
        //config_pio_channel(I2C3SdaxSIO_CHANNEL);
        //config_pio_channel(I2C3SclxCIO_CHANNEL);
        obj->reg_base = app_ss_i2c3;
    }*/ else {
        MBED_ASSERT(false);
    }

    obj->reg_base->cr = 0x0;
    obj->reg_base->sr = 0x0;
    obj->reg_base->crclear = DRIVER_BITFIELD_MASK(I2C_CR_START);
    obj->reg_base->crset = DRIVER_BITFIELD_MASK(I2C_CR_STOP);
    obj->reg_base->crclear = DRIVER_BITFIELD_MASK(I2C_CR_STOP);

    /* use 7bit addressing mode */
    obj->reg_base->oar = DRIVER_BITFIELD_CLR(obj->reg_base->oar, I2C_OAR_OAM);

    /* ACK enabled */
    obj->reg_base->crset = DRIVER_BITFIELD_MASK(I2C_CR_ACK);
    obj->reg_base->crset = DRIVER_BITFIELD_MASK(I2C_CR_INSYN);
    i2c_frequency(obj, 50000); //TODO: change it to default 100KHz in final version

    /* fifo flush */
    obj->reg_base->txflush = 0x0;
    obj->reg_base->rxflush = 0x0;
}

int i2c_start(i2c_t *obj)
{
    uint32_t timeout = MAX_TIMEOUT;

    obj->reg_base->crset = DRIVER_BITFIELD_MASK(I2C_CR_START);
    while(timeout--)
    {
        if (DRIVER_BITFIELD_GET(obj->reg_base->sr, I2C_SR_SB))
        {
            return true;
        }
    }
    return false;
}

int i2c_stop(i2c_t *obj)
{
    obj->reg_base->crset = DRIVER_BITFIELD_MASK(I2C_CR_STOP);
    return 0;
}

void i2c_frequency(i2c_t *obj, int hz)
{
    /* hz should not be zero */
    MBED_ASSERT(hz);

    hz=hz/1000; /* convert to KHz */
    bool fast_mode;
    unsigned char fr_value;
    uint16_t cc_value;
    uint32_t engine_frequency_in_khz;

    /* TODO: Should the engine frequency be read at run time? */
    engine_frequency_in_khz = (REF_FREQ/1000);
    MBED_ASSERT(engine_frequency_in_khz <= 100000); /* FR value for frequency greater than 100MHz is not specified */

    /* evaluate FR bits */
    if(engine_frequency_in_khz <= 10000)
    {
        fr_value = 0;
    }
    else if(engine_frequency_in_khz <= 16670)
    {
        fr_value = 1;
    }
    else if(engine_frequency_in_khz <= 26670)
    {
        fr_value = 2;
    }
    else if(engine_frequency_in_khz <= 40000)
    {
        fr_value = 3;
    }
    else if(engine_frequency_in_khz <= 53330)
    {
        fr_value = 4;
    }
    else if(engine_frequency_in_khz <= 66000)
    {
        fr_value = 5;
    }
    else if(engine_frequency_in_khz <= 80000)
    {
        fr_value = 6;
    }
    else /* 80-100 MHz */
    {
        fr_value = 7;
    }

    /* evaluate if fast mode or standard mode */
    if(hz <= 100) /* standard mode */
    {
        fast_mode = false;
        /* Set Clock Divider */
        cc_value = ((engine_frequency_in_khz / hz) - 7)/2;
    }
    else /* fast mode */
    {
        fast_mode = true;
        /* Set Clock Divider */
        cc_value = ((engine_frequency_in_khz / hz) - 9)/3;
    }
    cc_value = cc_value & 0xFFF; /*CC field is 12 bits */
    if(cc_value < 2)
    {
        cc_value = 2; /* the divider must be equal or greater than 2 */
    }

    /* set FR bits according to FENG */
    obj->reg_base->oar = DRIVER_BITFIELD_SET(obj->reg_base->oar, I2C_OAR_FR, fr_value);

    /* set FM_SM bit according to required I2C clock frequency, set CC value as calculated above */
    obj->reg_base->ccr = DRIVER_BITFIELD_SET(obj->reg_base->ccr, I2C_CCR_FM_SM, fast_mode);
    obj->reg_base->ccr |= DRIVER_BITFIELD_SET(obj->reg_base->ccr, I2C_CCR_CC, cc_value);
}



//void i2c_slave_mode(i2c_t *obj, int enable_slave)
//{
//  if (enable_slave)
//  {
//      obj->reg_base->intenableset = DRIVER_BITFIELD_MASK(I2C_INTENABLESET_ADSL);
//  }
//  else
//  {
//      obj->reg_base->intenableclear = DRIVER_BITFIELD_MASK(I2C_INTENABLESET_ADSL);
//  }
//  //i2c_stop(obj);
//}
//
//int i2c_slave_receive(i2c_t *obj)
//{
//    if (DRIVER_BITFIELD_GET(obj->reg_base->sr, I2C_SR_ADSL) != 0) { //address matched
//
//      obj->reg_base->sr = DRIVER_BITFIELD_MASK(I2C_SR_ADSL);
//      if ( ((uint8_t)obj->reg_base->rxbuffer) & 0x01 ) { //check if R/W bit is set or clear
//
//          return 1;
//      }
//      else {
//          return 3;
//      }
//    } else if (DRIVER_BITFIELD_GET(obj->reg_base->sr, I2C_SR_GC) != 0)  {
//        return 2;
//    } else {
//        return 0;
//    }
//}
//
//void i2c_slave_address(i2c_t *obj, int idx, uint32_t address, uint32_t mask)
//{
//
//  obj->reg_base->oar |= (unsigned char) (address & 0xFE);
//}
//
//int  i2c_slave_read(i2c_t *obj, char *data, int length)
//{
//  uint32_t rxFifoBytes=0;
//  int bytes_read=0;
//
//  if(!DRIVER_BITFIELD_GET(obj->reg_base->sr, I2C_SR_RXNOTEMPTY)) //0= RX buffer empty, 1= At least one data word is in RX
//    {
//       return 0;
//    }
//    /* receive bytes from fifo */
//    while (length) /* loop until all bytes are received */
//    {
//        rxFifoBytes = obj->reg_base->rxwordcount; /* get fifo count */
//        if (rxFifoBytes)
//        {
//          do
//          {
//              *data++ = (uint8_t) obj->reg_base->rxbuffer;
//              length--;
//              rxFifoBytes--;
//              bytes_read++;
//          } while (length && rxFifoBytes); /* receive from fifo as long as there are bytes available */
//        }
//    }
//  return bytes_read;
//}
//
//int i2c_slave_write(i2c_t *obj, const char *data, int length)
//{
//    uint32_t timeout = MAX_TIMEOUT;
//    uint32_t status;
//    int bytes_written=0;
//    uint32_t txFifoBytes=0;
//
//    /* transfer bytes keeping in check fifo count and by reading fifo empty flag */
//    while (length) /* loop until all bytes are transferred */
//    {
//        txFifoBytes = obj->reg_base->txwordcount; /* get available space in fifo */
//        do
//        {
//            obj->reg_base->txbuffer = *data++;
//            length--;
//            txFifoBytes--;
//            bytes_written++;
//        } while (length && txFifoBytes); /* write to fifo as long as there is space in fifo */
//
//        /* wait until transfer is complete or an error is generated */
//        timeout=MAX_TIMEOUT;
//        while (timeout--)
//        {
//            status=obj->reg_base->sr;
//            if(DRIVER_BITFIELD_GET(status, I2C_SR_TXEMPTY) ) /* fifo empty i.e transfer complete, put in more data */
//            {
//              obj->reg_base->sr=0;
//              obj->reg_base->crset = DRIVER_BITFIELD_MASK(I2C_CR_STOP);
//              obj->reg_base->crclear = DRIVER_BITFIELD_MASK(I2C_CR_STOP);
//                break;
//            }
////            else if ( (status &  ERROR_BITS) || timeout <= 0)
////            {
////                obj->reg_base->crset = DRIVER_BITFIELD_MASK(I2C_CR_STOP);
////                return 0; /* FIXME: return error or number of bytes transferred? */
////            }
//            timeout--;
//        }
//    }
//
//    return bytes_written;
//}

int i2c_read(i2c_t *obj, int address, char *data, int length, int stop)
{
    uint32_t timeout = MAX_TIMEOUT;
    uint32_t rxFifoBytes=0;
    uint32_t status;
    int bytes_read=0;

    /* send start */
    if (!i2c_start(obj)) /* start condition failure. Clear start and send stop ? */
    {
        printf("Error: start failure\r\n");
        /* clear start and release lines */
        if(DRIVER_BITFIELD_GET(obj->reg_base->cr, I2C_CR_START))
        {
            obj->reg_base->crclear = DRIVER_BITFIELD_MASK(I2C_CR_START);
        }
        obj->reg_base->crset = DRIVER_BITFIELD_MASK(I2C_CR_STOP);
        obj->reg_base->crclear = DRIVER_BITFIELD_MASK(I2C_CR_STOP);
        return -1;
    }

    /* Clear BTF flag and set number of bytes to receive */
    obj->reg_base->sr = DRIVER_BITFIELD_CLR(obj->reg_base->sr, I2C_SR_BTF);
    obj->reg_base->nrbr = (length > FIFO_SIZE) ? FIFO_SIZE : length;

    /* send slave address and check endad bit */
    obj->reg_base->txbuffer = (uint8_t)(address|1); //r/w bit set to high for read

    /* wait for successful address transfer */
    timeout=MAX_TIMEOUT;
    while (true)
    {
        status=obj->reg_base->sr;
        if( DRIVER_BITFIELD_GET(status, I2C_SR_ENDAD) )
        {
            obj->reg_base->sr = DRIVER_BITFIELD_CLR(obj->reg_base->sr, I2C_SR_ENDAD); // clear the bit
            obj->reg_base->cr = obj->reg_base->cr;
            break;
        }
        else if ( (status &  ERROR_BITS) || timeout <= 0)
        {
            printf("Error: address transfer error\r\n");
            obj->reg_base->crset = DRIVER_BITFIELD_MASK(I2C_CR_STOP);
            obj->reg_base->crclear = DRIVER_BITFIELD_MASK(I2C_CR_STOP);
            return -1;
        }
        timeout--;
    }

    while (length) /* loop until all bytes are received */ //FIXME: Recovery mechanism in case slave is not responding
    {
        /* get fifo count */
        rxFifoBytes = obj->reg_base->rxwordcount;
        if (rxFifoBytes)
        {
            obj->reg_base->sr = DRIVER_BITFIELD_CLR(obj->reg_base->sr, I2C_SR_BTF);
            do
            {
                /* if the next byte is going to be the last, set stop bit before reading last byte so that master sends NACK after last byte */
                if (length == 1 && stop == true)
                {
                    obj->reg_base->crclear = DRIVER_BITFIELD_MASK(I2C_CR_ACK);
                    obj->reg_base->crset = DRIVER_BITFIELD_MASK(I2C_CR_STOP);
                    obj->reg_base->sr = DRIVER_BITFIELD_CLR(obj->reg_base->sr, I2C_SR_NRB);
                }
                *data++ = (uint8_t)obj->reg_base->rxbuffer;
                length--;
                rxFifoBytes--;
                bytes_read++;
            } while (length && rxFifoBytes); /* receive from fifo as long as there are bytes available */
        }
        /* check if required number of bytes are available in fifo */
        if (DRIVER_BITFIELD_GET(obj->reg_base->sr, I2C_SR_NRB))
        {
            if (length)
            {
                obj->reg_base->nrbr = (length > FIFO_SIZE) ? FIFO_SIZE : length;
            }
            else
            {
                obj->reg_base->sr=0;
            }
        }
    }

    obj->reg_base->crclear = DRIVER_BITFIELD_MASK(I2C_CR_ACK);
    return bytes_read;
}

int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop)
{
    uint32_t timeout = MAX_TIMEOUT;
    uint32_t status;
    uint32_t txFifoBytes=0;
    int bytes_written=0;

    /* check if bus is busy */
    if (is_bus_busy(obj))
    {
        printf("Error: bus busy\r\n");
        return -1;
    }

    if (!i2c_start(obj)) /* start condition failure. Clear start and send stop ? */
    {
        printf("Error: start failure\r\n");
        /* clear start and release lines */
        if(DRIVER_BITFIELD_GET(obj->reg_base->cr, I2C_CR_START))
        {
            obj->reg_base->crclear = DRIVER_BITFIELD_MASK(I2C_CR_START);
        }
        obj->reg_base->crset = DRIVER_BITFIELD_MASK(I2C_CR_STOP);
        obj->reg_base->crclear = DRIVER_BITFIELD_MASK(I2C_CR_STOP);
        return -1;
    }

    /* send slave address and check endad bit */
    obj->reg_base->txbuffer = address;

    /* wait for successful address transfer */
    timeout=MAX_TIMEOUT;
    while (true)
    {
        status=obj->reg_base->sr;
        if( DRIVER_BITFIELD_GET(status, I2C_SR_ENDAD) )
        {
            obj->reg_base->sr = DRIVER_BITFIELD_CLR(obj->reg_base->sr, I2C_SR_ENDAD); // clear the bit
            //TODO: Then the master waits for a write to the SR register followed by a write in the CR register, holding the SCL line low
            //obj->reg_base->sr=0;
            break;
        }
        else if ( (status &  ERROR_BITS) || timeout <= 0)
        {
            printf("Error: address transfer error\r\n");
            obj->reg_base->crset = DRIVER_BITFIELD_MASK(I2C_CR_STOP);
            obj->reg_base->crclear = DRIVER_BITFIELD_MASK(I2C_CR_STOP);
            return -1;
        }
        timeout--;
    }

    /* transfer bytes keeping in check fifo count and by reading fifo empty flag */
    while (length)
    {
        /* get available space in fifo */
        txFifoBytes = obj->reg_base->txwordcount;
        if (txFifoBytes)
        {
            do
            {
                obj->reg_base->txbuffer = *data++;
                length--;
                txFifoBytes--;
                bytes_written++;
            } while (length && txFifoBytes); /* write to fifo as long as there is space in fifo */
        }

        /* wait until transfer is complete or an error is generated */
        timeout=MAX_TIMEOUT;
        while (true)
        {
            status=obj->reg_base->sr;
            if( DRIVER_BITFIELD_GET(status, I2C_SR_TXEMPTY) ) /* fifo empty i.e transfer complete, put in more data */
            {
                obj->reg_base->sr = DRIVER_BITFIELD_CLR(obj->reg_base->sr, I2C_SR_ENDAD);
                //printf("tx\r\n");
                break;
            }
            else if ( (status &  ERROR_BITS) || timeout <= 0)
            {
                printf("Error: tx fifo error\r\n");
                obj->reg_base->crset = DRIVER_BITFIELD_MASK(I2C_CR_STOP);
                obj->reg_base->crclear = DRIVER_BITFIELD_MASK(I2C_CR_STOP);
                return -1; /* FIXME: return error or number of bytes transferred? */
            }
            timeout--;
        }
    }

    /* send stop if stop == true */
    if (stop)
    {
        obj->reg_base->crset = DRIVER_BITFIELD_MASK(I2C_CR_STOP);
    }

    return bytes_written;
}

void i2c_reset(i2c_t *obj)
{
    i2c_stop(obj);
}

int i2c_byte_read(i2c_t *obj, int last)
{
    uint8_t data;
    uint32_t timeout=MAX_TIMEOUT;

    /* Send ACK (last = 0) or not (last = 1) */
    if (last)
    {
        obj->reg_base->crclear = DRIVER_BITFIELD_MASK(I2C_CR_ACK);
    }
    else
    {
        obj->reg_base->crset = DRIVER_BITFIELD_MASK(I2C_CR_ACK);
    }

    /* wait until a byte is ready to be read from fifo or a timeout has occurred */
    while( !DRIVER_BITFIELD_GET(obj->reg_base->sr, I2C_SR_RXNOTEMPTY) && timeout--);

    if (timeout <= 0) {
        return -1;
    }

    data = (uint8_t) obj->reg_base->rxbuffer;
    return data;
}

int i2c_byte_write(i2c_t *obj, int data)
{
    int ret_val;
    uint32_t status;
    uint32_t timeout=MAX_TIMEOUT;

    /* wait until there is space in fifo */
    while (!DRIVER_BITFIELD_GET(obj->reg_base->sr, I2C_SR_TXHALFFULL)); /* 0 = TX buffer contains more than 8 words, 1 = TX buffer contains less than or exact 8 words */
    {
        timeout--;
        if (timeout == 0)
        {
            return 2;
        }
    }

    timeout=MAX_TIMEOUT;
    obj->reg_base->txbuffer = (uint8_t) data;
    /* wait until transfer is complete or an error has occured */
    while (timeout)
    {
        status = obj->reg_base->sr;
        if( DRIVER_BITFIELD_GET(status, I2C_SR_TXEMPTY) ) /* fifo empty i.e transfer complete */
        {
            ret_val=1;
            break;
        }
        else if (status &  ERROR_BITS)
        {
            ret_val=0;
            break;
        }
        else if (timeout <= 0)
        {
            ret_val=2;
            break;
        }
        timeout--;
    }
    /* stop to be generated? */
    /* obj->reg_base->crset = DRIVER_BITFIELD_MASK(I2C_CR_STOP); */
    return ret_val;
}

//#endif // DEVICE_I2C
