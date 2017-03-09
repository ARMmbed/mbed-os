/* mbed Microcontroller Library
 * Copyright (c) 2015 ARM Limited
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
}private_i2c_t;


/*
 * Retrieve the private data of the instance related to a given IP
 */
static private_i2c_t* get_i2c_private(i2c_t *obj) {
    static private_i2c_t data0, data1;
    /*
     * Select which instance to give using the base
     * address of registers
     */
    switch((intptr_t)obj->i2c) {
        case I2C0_BASE:
            return &data0;
        case I2C1_BASE:
            return &data1;
        default:
            error("i2c driver private data structure not found for this registers base address");
            return (void*)0;
    }
}

/*
 * Infer the current state of a possibly combined transfer
 * (repeated restart) from the current state and the "stop" parameter
 * of read and write functions
 * MUST be called ONCE AND ONLY ONCE at the beginning of i2c transfer
 * functions (read and write)
 */
static i2c_transfer_state_t update_transfer_state(i2c_t *obj, int stop) {
    private_i2c_t* private_data = get_i2c_private(obj);
    i2c_transfer_state_t *state = &private_data->transfer_state;

    /*
     * Choose the current and next state depending on the current state
     * This basically implements rising and falling edge detection on
     * "stop" variable
     */
    switch(*state) {
        /* This is the default state for non restarted repeat transfer */
        default:
        case I2C_TRANSFER_SINGLE: /* Not a combined transfer */
            if (stop) {
                *state = I2C_TRANSFER_SINGLE;
            } else {
                *state = I2C_TRANSFER_COMBINED_FIRST_MESSAGE;
            }
            break;

        /* First message of a combined transfer */
        case I2C_TRANSFER_COMBINED_FIRST_MESSAGE:
        /* Message in the middle of a combined transfer */
        case I2C_TRANSFER_COMBINED_INTERMEDIATE_MESSAGE:
            if (stop) {
                *state = I2C_TRANSFER_COMBINED_LAST_MESSAGE;
            } else {
                *state = I2C_TRANSFER_COMBINED_INTERMEDIATE_MESSAGE;
            }
            break;

        /* Last message of a combined transfer */
        case I2C_TRANSFER_COMBINED_LAST_MESSAGE:
            if (stop) {
                *state = I2C_TRANSFER_SINGLE;
            } else {
                *state = I2C_TRANSFER_COMBINED_FIRST_MESSAGE;
            }
            break;
    }

    return *state;
}


static const PinMap PinMap_I2C_SDA[] = {
    {SHIELD_SDA, I2C_0, 0},
    {SENSOR_SDA, I2C_1, 0},
    {NC, NC , 0}
};

static const PinMap PinMap_I2C_SCL[] = {
    {SHIELD_SCL, I2C_0, 0},
    {SENSOR_SCL, I2C_1, 0},
    {NC, NC, 0}
};

static void clear_isr(i2c_t *obj) {
    /*
     * Writing to the IRQ status register clears set bits. Therefore, to
     * clear indiscriminately, just read the register and write it back.
     */
    uint32_t reg = obj->i2c->IRQ_STATUS;
    obj->i2c->IRQ_STATUS = reg;
}

void i2c_init(i2c_t *obj, PinName sda, PinName scl) {
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

    get_i2c_private(obj)->transfer_state = I2C_TRANSFER_SINGLE;

    i2c_frequency(obj, 100000); /* Default to 100kHz SCL frequency */
}

int i2c_start(i2c_t *obj) {
    return 0;
}

int i2c_stop(i2c_t *obj) {
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

void i2c_frequency(i2c_t *obj, int hz) {
    /*
     * Divider is split in two halfs : A and B
     * A is 2 bits wide and B is 6 bits wide
     * The Fscl frequency (SCL clock) is calculated with the following
     * equation:
     * Fscl=SystemCoreClock/(22*(A+1)*(B+1))
     * Here, we only calculate the B divisor which already enables a
     * wide enough range of values
     */
    uint32_t divisor_a = 0; /* Could be changed if a wider range of hz
                               is needed */
    uint32_t divisor_b = (SystemCoreClock / (22.0 * hz)) - 1;

    /* Clamp the divisors to their maximal value */
    divisor_a = divisor_a > I2C_CTRL_DIVISOR_A_BIT_MASK ?
                I2C_CTRL_DIVISOR_A_BIT_MASK : divisor_a;
    divisor_b = divisor_b > I2C_CTRL_DIVISOR_B_BIT_MASK ?
                I2C_CTRL_DIVISOR_B_BIT_MASK : divisor_b;

    uint8_t divisor_combinded = (divisor_a & I2C_CTRL_DIVISOR_A_BIT_MASK)
                              | (divisor_b & I2C_CTRL_DIVISOR_B_BIT_MASK);

    obj->i2c->CONTROL = (obj->i2c->CONTROL & ~I2C_CTRL_DIVISORS)
        | (divisor_combinded << I2C_CTRL_DIVISOR_OFFSET);
}

int i2c_read(i2c_t *obj, int address, char *data, int length, int stop) {
    int bytes_read = 0;
    int length_backup = length;
    char *data_backup = data;
    obj->last_xfer_address = address;
    i2c_transfer_state_t transfer_state = update_transfer_state(obj, stop);

    /* Try to write until it finally succeed or times out */
    int main_timeout = 10;
    int retry = 0;
    do {
        main_timeout--;

        retry = 0;
        bytes_read = 0;
        length = length_backup;
        data = data_backup;

        uint32_t reg = obj->i2c->CONTROL & 0xff7f;
        reg |= I2C_CTRL_RW | \
               I2C_CTRL_CLR_FIFO;
        /*
         * Only touch the HOLD bit at the beginning of
         * (possibly combined) transactions
         */
        if(transfer_state == I2C_TRANSFER_COMBINED_FIRST_MESSAGE
            || transfer_state == I2C_TRANSFER_SINGLE) {

            reg |= I2C_CTRL_HOLD;
        }
        obj->i2c->CONTROL = reg;

        /* Set the expected number of bytes to be received */
        if (length > I2C_TRANSFER_SIZE) {
            error("I2C transfer size too big for the FIFO");
        }
        obj->i2c->TRANSFER_SIZE = length & I2C_TRANSFER_SIZE;

        clear_isr(obj);

        /*
         * Start the transaction by writing address.
         * Discard the lower bit as it is automatically set
         * by the controller based on I2C_CTRL_RW bit in CONTROL
         * register
         */
        obj->i2c->ADDRESS = (address & 0xFF) >> 1;

        if(transfer_state == I2C_TRANSFER_COMBINED_LAST_MESSAGE
           || transfer_state == I2C_TRANSFER_SINGLE) {

            /* Clear the hold bit before reading the DATA register */
            obj->i2c->CONTROL &= ~I2C_CTRL_HOLD;
        }

        /* Wait for completion of the address transfer */
        int completion_timeout = 1000;
        while (completion_timeout) {
            completion_timeout--;

            uint32_t irq_status = obj->i2c->IRQ_STATUS;
            if (irq_status & I2C_IRQ_NACK
                || irq_status & I2C_IRQ_ARB_LOST) {

                retry = 1;
                break;
            }

            if(irq_status & I2C_IRQ_COMP) {
                break;
            }
        }

        /* If retry, jump to the beginning and try again */
        if (retry || !completion_timeout) {
            retry = 1;
            continue;
        }

        clear_isr(obj);

        /* Read the data from the DATA register */
        completion_timeout = 1000;
        while (length && completion_timeout) {
            completion_timeout--;

            uint32_t irq_status = obj->i2c->IRQ_STATUS;
            uint32_t status = obj->i2c->STATUS;

            if(irq_status & I2C_IRQ_NACK ||
               irq_status & I2C_IRQ_ARB_LOST) {

                retry = 1;
                break;
            }

            /*
             * Just wait for RXDV because COMP is only risen at the end
             * of the transfer
             */
            if (status & I2C_STATUS_RXDV) {
                *data++ = obj->i2c->DATA & 0xFF;
                length--;
                bytes_read++;
            }

            if (irq_status & I2C_IRQ_RX_UNF) {
                error("Reading more bytes than the I2C transfer size");
                retry = 1;
                break;
            }
        }

        /* If retry, jump to the beginning and try again */
        if (retry || !completion_timeout) {
            retry = 1;
            continue;
        }
    } while(retry && main_timeout);

    if (!main_timeout) {
        bytes_read = 0;
        data = data_backup;
    }

    obj->i2c->CONTROL |= I2C_CTRL_CLR_FIFO;
    clear_isr(obj);
    return bytes_read;
}

int i2c_write(i2c_t *obj, int address, const char *data, int length,
              int stop) {

    int bytes_written = 0;
    int length_backup = length;
    const char *data_backup = data;
    obj->last_xfer_address = address;
    i2c_transfer_state_t transfer_state = update_transfer_state(obj, stop);

    /* Try to write until it finally succeed or times out */
    int main_timeout = 10;
    int retry = 0;
    do {
        main_timeout--;

        retry = 0;
        bytes_written = 0;
        length = length_backup;
        data = data_backup;

        /* Read the defined bits in the control register */
        uint32_t reg = obj->i2c->CONTROL & 0xff7f;
        reg |= I2C_CTRL_CLR_FIFO;
        reg &= ~I2C_CTRL_RW;

        /*
         * Only touch the HOLD bit at the beginning of
         * (possibly combined) transactions
         */
        if(transfer_state == I2C_TRANSFER_COMBINED_FIRST_MESSAGE
           || transfer_state == I2C_TRANSFER_SINGLE) {

            reg |= I2C_CTRL_HOLD;
        }
        obj->i2c->CONTROL = reg;

        clear_isr(obj);

        /* Set the expected number of bytes to be transmitted */
        if (length > I2C_TRANSFER_SIZE) {
            error("I2C transfer size too big for the FIFO");
        }

        /* Set the expected number of bytes to be transmitted */
        obj->i2c->TRANSFER_SIZE = length & I2C_TRANSFER_SIZE;

        /*
         * Write the address, triggering the start of the transfer
         * Discard the lower bit as it is automatically set
         * by the controller based on I2C_CTRL_RW bit in CONTROL
         * register
         */
        obj->i2c->ADDRESS = (address & 0xFF) >> 1;

        /* Send the data bytes */
        int write_timeout = 1000 + length;
        while (length && write_timeout) {
            write_timeout--;
            uint32_t irq_status = obj->i2c->IRQ_STATUS;
            /* If overflow, undo last step */
            if (irq_status & I2C_IRQ_TX_OVF) {
                *data--;
                length++;
                bytes_written--;
                /* Clear the bit by writing 1 to it */
                obj->i2c->IRQ_STATUS |= I2C_IRQ_TX_OVF;
            }

            if (irq_status & I2C_IRQ_NACK
                || irq_status & I2C_IRQ_ARB_LOST) {

                retry = 1;
                break;
            }

            obj->i2c->DATA = *data++;
            length--;
            bytes_written++;
        }

        /* If retry, jump to the beginning and try again */
        if (retry || !write_timeout) {
            retry = 1;
            continue;
        }

        if(transfer_state == I2C_TRANSFER_COMBINED_LAST_MESSAGE
           || transfer_state == I2C_TRANSFER_SINGLE) {
            /*
             * Clear the hold bit to signify the end
             * of the write sequence
             */
            obj->i2c->CONTROL &= ~I2C_CTRL_HOLD;
        }


        /* Wait for transfer completion */
        int completion_timeout = 1000;
        while (completion_timeout) {
            completion_timeout--;

            uint32_t irq_status = obj->i2c->IRQ_STATUS;
            if(irq_status & I2C_IRQ_NACK
               || irq_status & I2C_IRQ_ARB_LOST) {
                retry = 1;
                break;
            }
            if(irq_status & I2C_IRQ_COMP) {
                break;
            }
        }

        /* If retry, jump to the beginning and try again */
        if (retry || !completion_timeout) {
            continue;
        }

        obj->i2c->CONTROL |= I2C_CTRL_CLR_FIFO;
        clear_isr(obj);
    } while(retry && main_timeout);

    return bytes_written;
}

void i2c_reset(i2c_t *obj) {
    i2c_stop(obj);
}

int i2c_byte_read(i2c_t *obj, int last) {
    char i2c_ret = 0;
    i2c_read(obj, obj->last_xfer_address, &i2c_ret, 1, last);
    return i2c_ret;
}

int i2c_byte_write(i2c_t *obj, int data) {
    /* Store the number of written bytes */
    uint32_t wb = i2c_write(obj, obj->last_xfer_address, (char*)&data, 1, 0);
    if (wb == 1)
        return 1;
    else
        return 0;
}

void i2c_slave_mode(i2c_t *obj, int enable_slave) {
}

int i2c_slave_receive(i2c_t *obj) {
    return 0;
}

int i2c_slave_read(i2c_t *obj, char *data, int length) {
    return 0;
}

int i2c_slave_write(i2c_t *obj, const char *data, int length) {
    return 0;
}

void i2c_slave_address(i2c_t *obj, int idx, uint32_t address, uint32_t mask) {
}
