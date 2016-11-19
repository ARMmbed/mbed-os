/*******************************************************************************
 * Copyright (C) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *******************************************************************************
 */

#include <string.h>
#include "mbed_assert.h"
#include "cmsis.h"
#include "spi_api.h"
#include "spi_multi_api.h"
#include "pinmap.h"
#include "ioman_regs.h"
#include "clkman_regs.h"
#include "PeripheralPins.h"

#define DEFAULT_CHAR    8
#define DEFAULT_MODE    0
#define DEFAULT_FREQ    1000000

// BYTE maximums for FIFO and page writes; FIFO depth spec'd as 16-bit words
#define SPI_MAX_BYTE_LEN     (MXC_CFG_SPI_FIFO_DEPTH * 2)
#define SPI_MAX_PAGE_LEN     (MXC_CFG_SPI_FIFO_DEPTH * 2)

#if DEVICE_SPI_ASYNCH
// Instance references for async transactions
static struct spi_s *state[MXC_CFG_SPI_INSTANCES] = {NULL};
#endif

//******************************************************************************
void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
    // Make sure pins are pointing to the same SPI instance
    SPIName spi_mosi = (SPIName)pinmap_peripheral(mosi, PinMap_SPI_MOSI);
    SPIName spi_miso = (SPIName)pinmap_peripheral(miso, PinMap_SPI_MISO);
    SPIName spi_sclk = (SPIName)pinmap_peripheral(sclk, PinMap_SPI_SCLK);
    SPIName spi_ssel = (SPIName)pinmap_peripheral(ssel, PinMap_SPI_SSEL);

    SPIName spi_data = (SPIName)pinmap_merge(spi_mosi, spi_miso);
    SPIName spi_cntl;

    // Give the application the option to manually control Slave Select
    if ((SPIName)spi_ssel != (SPIName)NC) {
        spi_cntl = (SPIName)pinmap_merge(spi_sclk, spi_ssel);
        // Slave select is currently limited to slave select zero. If others are
        // to be supported a function to map PinName to a value suitable for use
        // in mstr_cfg.slave_sel will be required.
        obj->spi.ssel = 0;
    } else {
        spi_cntl = spi_sclk;
        obj->spi.ssel = -1;
    }

    SPIName spi = (SPIName)pinmap_merge(spi_data, spi_cntl);

    MBED_ASSERT((SPIName)spi != (SPIName)NC);

    // Set the obj pointer to the proper SPI Instance
    obj->spi.spi = (mxc_spi_regs_t*)spi;

    // Set the SPI index and FIFOs
    obj->spi.index = MXC_SPI_GET_IDX(obj->spi.spi);
    obj->spi.fifo = MXC_SPI_GET_SPI_FIFO(obj->spi.index);

    // Configure the pins
    pinmap_pinout(mosi, PinMap_SPI_MOSI);
    pinmap_pinout(miso, PinMap_SPI_MISO);
    pinmap_pinout(sclk, PinMap_SPI_SCLK);
    pinmap_pinout(ssel, PinMap_SPI_SSEL);

#if DEVICE_SPI_ASYNCH
    // Configure default page size; size is known to async interface
    obj->spi.spi->mstr_cfg = (obj->spi.spi->mstr_cfg & ~MXC_F_SPI_MSTR_CFG_PAGE_SIZE) | MXC_S_SPI_MSTR_CFG_PAGE_32B;
#endif

    // Enable SPI and FIFOs
    obj->spi.spi->gen_ctrl = (MXC_F_SPI_GEN_CTRL_SPI_MSTR_EN |
                              MXC_F_SPI_GEN_CTRL_TX_FIFO_EN |
                              MXC_F_SPI_GEN_CTRL_RX_FIFO_EN );

    obj->spi.sclk = sclk;       // save the sclk PinName in the object as a key for Quad SPI pin mapping lookup
    spi_master_width(obj, 0);   // default this for Single SPI communications
}

//******************************************************************************
void spi_format(spi_t *obj, int bits, int mode, int slave)
{
    // Check the validity of the inputs
    MBED_ASSERT(((bits >= 1) && (bits <= 32)) && ((mode >= 0) && (mode <= 3)));

    // Only supports master mode
    MBED_ASSERT(!slave);

    // Save formatting data
    obj->spi.bits = bits;

    // Set the mode
    MXC_SET_FIELD(&obj->spi.spi->mstr_cfg, MXC_F_SPI_MSTR_CFG_SPI_MODE, mode << MXC_F_SPI_MSTR_CFG_SPI_MODE_POS);
}

//******************************************************************************
void spi_frequency(spi_t *obj, int hz)
{
    // Maximum frequency is half the system frequency
    MBED_ASSERT((unsigned int)hz <= (SystemCoreClock / 2));
    unsigned clocks = ((SystemCoreClock / 2) / hz);

    // Figure out the divider ratio
    int clk_div = 1;
    while (clk_div < 10) {
        if (clocks < 0x10) {
            break;
        }
        clk_div++;
        clocks = clocks >> 1;
    }

    // Turn on the SPI clock
    if (obj->spi.index == 0) {
        MXC_CLKMAN->sys_clk_ctrl_11_spi0 = clk_div;
    } else if (obj->spi.index == 1) {
        MXC_CLKMAN->sys_clk_ctrl_12_spi1 = clk_div;
    } else if (obj->spi.index == 2) {
        MXC_CLKMAN->sys_clk_ctrl_13_spi2 = clk_div;
    } else {
        MBED_ASSERT(0);
    }

    // Set the number of clocks to hold sclk high and low
    MXC_SET_FIELD(&obj->spi.spi->mstr_cfg, (MXC_F_SPI_MSTR_CFG_SCK_HI_CLK | MXC_F_SPI_MSTR_CFG_SCK_LO_CLK),
        ((clocks << MXC_F_SPI_MSTR_CFG_SCK_HI_CLK_POS) | (clocks << MXC_F_SPI_MSTR_CFG_SCK_LO_CLK_POS)));
}

//******************************************************************************
void spi_master_width(spi_t *obj, SpiWidth width)
{
    // Save the width to be used in the SPI header
    switch (width) {
        case WidthSingle:
            obj->spi.width = MXC_S_SPI_FIFO_WIDTH_SINGLE;
            break;
        case WidthDual:
            obj->spi.width = MXC_S_SPI_FIFO_WIDTH_DUAL;
            break;
        case WidthQuad:
            obj->spi.width = MXC_S_SPI_FIFO_WIDTH_QUAD;
            // do pin mapping for SDIO[2] and SDIO[3] if Quad SPI is selected
            pinmap_pinout(obj->spi.sclk, PinMap_SPI_QUAD);
            break;
        default:
            MBED_ASSERT(0);
    }
}

//******************************************************************************
/** Performs a master write or read transaction
 *
 * @param[in] obj           The SPI peripheral to use for sending
 * @param[in] value         The value to send
 * @param[in] direction     Direction of the transaction, TX, RX or both
 * @return Returns the value received during send
 */
static int spi_master_transaction(spi_t *obj, int value, uint32_t direction)
{
    int bits;

    // Create the header
    uint16_t header =  (direction |                 // direction based on SPI object
                        MXC_S_SPI_FIFO_UNIT_BITS |  // unit size
                        ((obj->spi.bits == 32) ? 0 : obj->spi.bits << MXC_F_SPI_FIFO_SIZE_POS) |    // Number of units
                        obj->spi.width |            // I/O width
                        ((obj->spi.ssel == -1) ? 0 : 1 << MXC_F_SPI_FIFO_DASS_POS));

    // Send the message header
    *obj->spi.fifo->trans_16 = header;

    // Send the data
    if (obj->spi.bits < 17) {
        *obj->spi.fifo->trans_16 = (uint16_t)value;
    } else {
        *obj->spi.fifo->trans_32 = (uint32_t)value;
    }

    // Get the data
    bits = obj->spi.bits;
    int result = 0;
    int i = 0;
    while (bits > 0) {
        // Wait for data
        while (((obj->spi.spi->fifo_ctrl & MXC_F_SPI_FIFO_CTRL_RX_FIFO_USED)
                >> MXC_F_SPI_FIFO_CTRL_RX_FIFO_USED_POS) < 1);

        result |= (*obj->spi.fifo->rslts_8 << (i++*8));
        bits-=8;
    }

    return result;
}

//******************************************************************************
int spi_master_write(spi_t *obj, int value)
{
    // set the fifo direction for full duplex, TX and RX simultaneously
    return spi_master_transaction(obj, value, MXC_S_SPI_FIFO_DIR_BOTH);
}

//******************************************************************************
int spi_master_read(spi_t *obj)
{
    return spi_master_transaction(obj, 0xFF, MXC_S_SPI_FIFO_DIR_RX);
}

//******************************************************************************
// spi_busy() is part of the synchronous API, it is not used by the asynchronous API.
int spi_busy(spi_t *obj)
{
    return !(obj->spi.spi->intfl & MXC_F_SPI_INTFL_TX_READY);
}

#if DEVICE_SPI_ASYNCH
//******************************************************************************
static uint32_t spi_master_read_rxfifo(mxc_spi_regs_t *spim, mxc_spi_fifo_regs_t *fifo, uint8_t *data, uint32_t len)
{
    uint32_t num = 0;
    uint32_t avail = ((spim->fifo_ctrl & MXC_F_SPI_FIFO_CTRL_RX_FIFO_USED) >> MXC_F_SPI_FIFO_CTRL_RX_FIFO_USED_POS);

    // Get data from the RXFIFO
    while (avail && (len - num)) {
        // Save data from the RXFIFO
        if ((avail >= 4) && ((len - num) >= 4)) {
            uint32_t temp = *fifo->rslts_32;
            data[num++] = temp;
            data[num++] = temp >> 8;
            data[num++] = temp >> 16;
            data[num++] = temp >> 24;
            avail -= 4;
        } else if ((avail >= 2) && ((len - num) >= 2)) {
            uint16_t temp = *fifo->rslts_16;
            data[num++] = temp;
            data[num++] = temp >> 8;
            avail -= 2;
        } else {
            data[num++] = *fifo->rslts_8;
            avail--;
        }

        // Check to see if there is more data in the FIFO
        if (avail == 0) {
            avail = ((spim->fifo_ctrl & MXC_F_SPI_FIFO_CTRL_RX_FIFO_USED) >> MXC_F_SPI_FIFO_CTRL_RX_FIFO_USED_POS);
        }
    }

    return num;
}

//******************************************************************************
static uint32_t spi_master_transfer_handler(spi_t *obj)
{
    uint8_t read;
    uint8_t write;
    uint16_t header;
    uint32_t pages;
    uint32_t bytes;
    uint32_t inten;
    unsigned remain;
    unsigned bytes_read;
    unsigned head_rem_temp;
    unsigned avail;
    struct spi_s *req = &obj->spi;
    mxc_spi_regs_t *spim = obj->spi.spi;
    mxc_spi_fifo_regs_t *fifo = obj->spi.fifo;

    inten = 0;

    // Figure out if we're reading
    read = (req->rx_data != NULL) ? 1 : 0;

    // Figure out if we're writing
    write = (req->tx_data != NULL) ? 1 : 0;

    // Read byte from the FIFO if we are reading
    if (read) {

        // Read all of the data in the RXFIFO, or until we don't need anymore
        bytes_read = spi_master_read_rxfifo(spim, fifo, &req->rx_data[req->read_num], (req->len - req->read_num));

        req->read_num += bytes_read;

        // Adjust head_rem if we are only reading
        if (!write && (req->head_rem > 0)) {
            req->head_rem -= bytes_read;
        }

        // Figure out how many bytes we have left to read
        if (req->head_rem > 0) {
            remain = req->head_rem;
        } else {
            remain = req->len - req->read_num;
        }

        if (remain) {

            // Set the RX interrupts
            if (remain > MXC_CFG_SPI_FIFO_DEPTH) {
                spim->fifo_ctrl = ((spim->fifo_ctrl & ~MXC_F_SPI_FIFO_CTRL_RX_FIFO_AF_LVL) |
                                   ((MXC_CFG_SPI_FIFO_DEPTH - 2) << MXC_F_SPI_FIFO_CTRL_RX_FIFO_AF_LVL_POS));
            } else {
                spim->fifo_ctrl = ((spim->fifo_ctrl & ~MXC_F_SPI_FIFO_CTRL_RX_FIFO_AF_LVL) |
                                   ((remain - 1) << MXC_F_SPI_FIFO_CTRL_RX_FIFO_AF_LVL_POS));
            }

            inten |= MXC_F_SPI_INTEN_RX_FIFO_AF;
        }
    }

    // Figure out how many bytes we have left to send headers for
    if (write) {
        remain = req->len - req->write_num;
    } else {
        remain = req->len - req->read_num;
    }

    // See if we need to send a new header
    if ((req->head_rem <= 0) && remain) {

        // Set the transaction configuration in the header
        header = ((write | (read << 1)) << MXC_F_SPI_FIFO_DIR_POS) | (req->width << MXC_F_SPI_FIFO_WIDTH_POS);

        if (remain >= SPI_MAX_BYTE_LEN) {

            // Send a 32 byte header
            if (remain == SPI_MAX_BYTE_LEN) {

                header |= (MXC_S_SPI_FIFO_UNIT_BYTES | MXC_F_SPI_FIFO_DASS);

                // Save the number of bytes we need to write to the FIFO
                bytes = SPI_MAX_BYTE_LEN;

            } else {
                // Send in increments of 32 byte pages
                header |= MXC_S_SPI_FIFO_UNIT_PAGES;
                pages = remain / SPI_MAX_PAGE_LEN;

                if (pages >= 32) {
                    // 0 maps to 32 in the header
                    bytes = 32 * SPI_MAX_PAGE_LEN;
                } else {
                    header |= (pages << MXC_F_SPI_FIFO_SIZE_POS);
                    bytes = pages * SPI_MAX_PAGE_LEN;
                }

                // Check if this is the last header we will send
                if ((remain - bytes) == 0) {
                    header |= MXC_F_SPI_FIFO_DASS;
                }
            }

            fifo->trans_16[0] = header;

            // Save the number of bytes we need to write to the FIFO
            req->head_rem = bytes;

        } else {
            // Send final header with the number of bytes remaining and de-assert the SS at the end of the transaction
            header |= (MXC_S_SPI_FIFO_UNIT_BYTES | (remain << MXC_F_SPI_FIFO_SIZE_POS) | MXC_F_SPI_FIFO_DASS);
            fifo->trans_16[0] = header;
            req->head_rem = remain;
        }
    }

    // Put data into the FIFO if we are writing
    remain = req->len - req->write_num;
    head_rem_temp = req->head_rem;
    if (write && head_rem_temp) {

        // Fill the FIFO
        avail = (MXC_CFG_SPI_FIFO_DEPTH - ((spim->fifo_ctrl & MXC_F_SPI_FIFO_CTRL_TX_FIFO_USED) >> MXC_F_SPI_FIFO_CTRL_TX_FIFO_USED_POS));

        // Use memcpy for everything except the last byte in odd length transactions
        while ((avail >= 2) && (head_rem_temp >= 2)) {

            unsigned length;
            if (head_rem_temp < avail) {
                length = head_rem_temp;
            } else {
                length = avail;
            }

            // Only memcpy even numbers
            length = ((length / 2) * 2);

            memcpy((void*)fifo->trans_32, &(req->tx_data[req->write_num]), length);

            head_rem_temp -= length;
            req->write_num += length;

            avail = (MXC_CFG_SPI_FIFO_DEPTH - ((spim->fifo_ctrl & MXC_F_SPI_FIFO_CTRL_TX_FIFO_USED) >> MXC_F_SPI_FIFO_CTRL_TX_FIFO_USED_POS));
        }

        // Copy the last byte and pad with 0xF0 to not get confused as header
        if ((avail >= 1) && (head_rem_temp == 1)) {

            // Write the last byte
            fifo->trans_16[0] = (0xF000 | req->tx_data[req->write_num]);

            avail -= 1;
            req->write_num += 1;
            head_rem_temp -= 1;
        }

        req->head_rem = head_rem_temp;
        remain = req->len - req->write_num;

        // Set the TX interrupts
        if (remain) {

            // Set the TX FIFO almost empty interrupt if we have to refill
            spim->fifo_ctrl = ((spim->fifo_ctrl & ~MXC_F_SPI_FIFO_CTRL_TX_FIFO_AE_LVL) |
                               ((MXC_CFG_SPI_FIFO_DEPTH - 2) << MXC_F_SPI_FIFO_CTRL_TX_FIFO_AE_LVL_POS));

            inten |= MXC_F_SPI_INTEN_TX_FIFO_AE;
        }
    }

    // Check to see if we've finished reading and writing
    if (((read && (req->read_num == req->len)) || !read) &&
            ((req->write_num == req->len) || !write)) {

        // Disable interrupts
        spim->inten = 0;
    }

    // Enable the SPIM interrupts
    return inten;
}

//******************************************************************************
void spi_master_transfer(spi_t *obj, const void *tx, size_t tx_length, void *rx, size_t rx_length, uint8_t bit_width, uint32_t handler, uint32_t event, DMAUsage hint)
{
    MBED_ASSERT(tx_length == rx_length);
    MBED_ASSERT(bit_width == obj->spi.bits);

    // Save object reference for callback
    state[obj->spi.index] = &obj->spi;

    // Initialize request info
    obj->spi.tx_data = tx;
    obj->spi.rx_data = rx;
    obj->spi.len = tx_length;
    obj->spi.callback = (void(*)())handler;
    obj->spi.event = event;
    // Clear transfer state
    obj->spi.read_num = 0;
    obj->spi.write_num = 0;
    obj->spi.head_rem = 0;

    NVIC_EnableIRQ(MXC_SPI_GET_IRQ(obj->spi.index));

    obj->spi.spi->inten = spi_master_transfer_handler(obj);
}

//******************************************************************************
uint32_t spi_irq_handler_asynch(spi_t *obj)
{
    mxc_spi_regs_t *spim = obj->spi.spi;
    uint32_t flags;

    // Clear the interrupt flags
    spim->inten = 0;
    flags = spim->intfl;
    spim->intfl = flags;

    // Figure out if this SPIM has an active request
    if (flags) {
        if ((spim->inten = spi_master_transfer_handler(obj)) != 0) {
            return 0;
        }
    }

    state[obj->spi.index] = NULL;

    return SPI_EVENT_COMPLETE;
}

//******************************************************************************
uint8_t spi_active(spi_t *obj)
{
    mxc_spi_regs_t *spim = obj->spi.spi;

    // Check to see if there are any ongoing transactions
    if ((state[obj->spi.index] == NULL) &&
            !(spim->fifo_ctrl & MXC_F_SPI_FIFO_CTRL_TX_FIFO_USED)) {
        return 0;
    }

    return 1;
}

//******************************************************************************
void spi_abort_asynch(spi_t *obj)
{
    mxc_spi_regs_t *spim = obj->spi.spi;

    // Disable interrupts, clear the flags
    spim->inten = 0;
    spim->intfl = spim->intfl;

    // Reset the SPIM to cancel the on ongoing transaction
    spim->gen_ctrl &= ~(MXC_F_SPI_GEN_CTRL_SPI_MSTR_EN);
    spim->gen_ctrl |= (MXC_F_SPI_GEN_CTRL_SPI_MSTR_EN);

    state[obj->spi.index] = NULL;
}

//******************************************************************************
static void SPI_IRQHandler(int spim_num)
{
    if (state[spim_num] != NULL) {
        if (state[spim_num]->callback != NULL) {
            state[spim_num]->callback();
            return;
        }
    }
    mxc_spi_regs_t *spim = MXC_SPI_GET_SPI(spim_num);
    spim->inten = 0;
}

//******************************************************************************
void SPI0_IRQHandler(void) { SPI_IRQHandler(0); }
void SPI1_IRQHandler(void) { SPI_IRQHandler(1); }
void SPI2_IRQHandler(void) { SPI_IRQHandler(2); }

#endif
