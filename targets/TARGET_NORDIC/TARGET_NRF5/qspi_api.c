/*
 * Copyright (c) 2017 Nordic Semiconductor ASA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice, this list
 *      of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form, except as embedded into a Nordic Semiconductor ASA
 *      integrated circuit in a product or a software update for such product, must reproduce
 *      the above copyright notice, this list of conditions and the following disclaimer in
 *      the documentation and/or other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of its contributors may be
 *      used to endorse or promote products derived from this software without specific prior
 *      written permission.
 *
 *   4. This software, with or without modification, must only be used with a
 *      Nordic Semiconductor ASA integrated circuit.
 *
 *   5. Any software provided in binary or object form under this license must not be reverse
 *      engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "qspi_api.h"
#include "nrf_drv_qspi.h"
#include "nrf_drv_common.h"

#if DEVICE_QSPI

void QSPI_IRQHandler(void);

#define HZ_TO_CONFIG(hz) ((32000000/(hz))-1)

nrf_drv_qspi_config_t config;

static void qspi_irq_handler(nrf_drv_qspi_evt_t event, void * p_context)
{
    qspi_t *obj = (qspi_t *) p_context;
    obj->is_busy = 0;

#if DEVICE_QSPI_ASYNCH
    if (obj->handler)
    {
        void (*handler)(void) = (void (*)(void))obj->handler;
        obj->handler = 0;
        handler();
    }
#endif
}

int32_t qspi_init(qspi_t *obj,
                  int hz,
                  qspi_pins_t * pins,
                  int spi_mode)
{
    (void)(obj);
    if (hz > 32000000)
    {
        /* Frequency is too high. */
        return -1;
    }

    config.pins.sck_pin     = (uint32_t)pins->sclk;
    config.pins.csn_pin     = (uint32_t)pins->ssel;
    config.pins.io0_pin     = (uint32_t)pins->io0;
    config.pins.io1_pin     = (uint32_t)pins->io1;
    config.pins.io2_pin     = (uint32_t)pins->io2;
    config.pins.io3_pin     = (uint32_t)pins->io3;
    config.irq_priority     = SPI_DEFAULT_CONFIG_IRQ_PRIORITY;

    config.phy_if.sck_freq  = HZ_TO_CONFIG(hz),
    config.phy_if.sck_delay = 0x05,
    config.phy_if.dpmen     = false;
    config.phy_if.spi_mode  = spi_mode == 0 ? NRF_QSPI_MODE_0 : NRF_QSPI_MODE_1;
    obj->is_busy = 0;

#if DEVICE_QSPI_ASYNCH
    obj->handler = 0;
#endif

    NVIC_SetVector(QSPI_IRQn, (uint32_t) QSPI_IRQHandler);
    nrf_drv_common_irq_enable(QSPI_IRQn, config.irq_priority);

    return 0;

}

int32_t qspi_prepare(qspi_t *obj,
                     qspi_addrmode_t  addrmode,
                     qspi_readmode_t  readmode,
                     qspi_writemode_t writemode)
{
    switch (readmode) {
        case QSPI_READMODE_1_1_2:
            config.prot_if.readoc   = NRF_QSPI_READOC_READ2O;
            break;
        case QSPI_READMODE_1_1_4:
            config.prot_if.readoc   = NRF_QSPI_READOC_READ4O;
            break;
        case QSPI_READMODE_1_2_2:
            config.prot_if.readoc   = NRF_QSPI_READOC_READ2IO;
            break;
        case QSPI_READMODE_1_4_4:
            config.prot_if.readoc   = NRF_QSPI_READOC_READ4IO;
            break;
        case QSPI_READMODE_1_1_1:
        default:
            config.prot_if.readoc   = NRF_QSPI_READOC_FASTREAD;
    }

    switch (writemode) {
        case QSPI_WRITEMODE_1_1_2:
            config.prot_if.writeoc   = QSPI_IFCONFIG0_WRITEOC_PP2O;
            break;
        case QSPI_WRITEMODE_1_1_4:
            config.prot_if.writeoc   = QSPI_IFCONFIG0_WRITEOC_PP4O;
            break;
        case QSPI_WRITEMODE_1_4_4:
            config.prot_if.writeoc   = QSPI_IFCONFIG0_WRITEOC_PP4IO;
            break;
        case QSPI_WRITEMODE_1_1_1:
        default:
            config.prot_if.writeoc  = NRF_QSPI_WRITEOC_PP;
    }

    config.prot_if.addrmode = addrmode == QSPI_ADDRMODE_24BIT ?
                                  NRF_QSPI_ADDRMODE_24BIT :
                                  NRF_QSPI_ADDRMODE_32BIT;
    return 0;
}

int32_t qspi_start(qspi_t *obj)
{
    nrf_drv_qspi_init(&config, qspi_irq_handler, (void *) obj);
    return 0;
}

int32_t qspi_free(qspi_t *obj)
{
    (void)(obj);
    return 0;
}

/* Static internal functions */

static int32_t _qspi_erase_block(qspi_t *obj, uint32_t address)
{
    if (obj->is_busy == 1)
    {
        return -1;
    }

    obj->is_busy = 1;
    if (nrf_drv_qspi_erase(NRF_QSPI_ERASE_LEN_4KB, address) != NRF_SUCCESS)
    {
        obj->is_busy = 0;
        return -1;
    }

    return 0;
}

static int32_t _qspi_erase_chip(qspi_t *obj)
{
    if (obj->is_busy == 1)
    {
        return -1;
    }

    obj->is_busy = 1;
    if (nrf_drv_qspi_erase(QSPI_ERASE_LEN_LEN_All, 0) != NRF_SUCCESS)
    {
        obj->is_busy = 0;
        return -1;
    }

    return 0;
}

static int32_t _qspi_program_page(qspi_t *obj, uint32_t address, const uint8_t *data, uint32_t size)
{
    if (obj->is_busy == 1)
    {
        return -1;
    }

    obj->is_busy = 1;
    if (nrf_drv_qspi_write(data, size, address) != NRF_SUCCESS)
    {
        obj->is_busy = 0;
        return -1;
    }

    return 0;
}

static int32_t _qspi_page_read(qspi_t *obj, uint32_t address, uint8_t *data, uint32_t size)
{
    if (obj->is_busy == 1)
    {
        return -1;
    }

    obj->is_busy = 1;
    if (nrf_drv_qspi_read(data, size, address) != NRF_SUCCESS)
    {
        obj->is_busy = 0;
        return -1;
    }

    return 0;
}

int32_t qspi_erase_block(qspi_t *obj, uint32_t address)
{
    int32_t ret = _qspi_erase_block(obj, address);
    if (ret != 0)
    {
        return ret;
    }

    while (obj->is_busy == 1);
    return 0;
}

int32_t qspi_erase_chip(qspi_t *obj)
{
    int32_t ret = _qspi_erase_chip(obj);
    if (ret != 0)
    {
        return ret;
    }

    while (obj->is_busy == 1);
    return 0;
}

int32_t qspi_program_page(qspi_t *obj, uint32_t address, const uint8_t *data, uint32_t size)
{
    int32_t ret = _qspi_program_page(obj, address, data, size);
    if (ret != 0)
    {
        return ret;
    }

    while (obj->is_busy == 1);
    return 0;
}

int32_t qspi_page_read(qspi_t *obj, uint32_t address, uint8_t *data, uint32_t size)
{
    int32_t ret = _qspi_page_read(obj, address, data, size);
    if (ret != 0)
    {
        return ret;
    }

    while (obj->is_busy == 1);
    return 0;
}

int32_t qspi_send_instruction(qspi_t *obj,
                              uint8_t cmd,
                              uint32_t tx_size,
                              const uint8_t *tx_data,
                              uint32_t rx_size,
                              uint8_t *rx_data)
{
    ret_code_t ret_code;
    uint32_t i;
    uint8_t data[8];
    uint32_t data_size = tx_size + rx_size;

    // this function is asynchronous so we can store data in function body
    if (obj->is_busy == 1)
    {
        return -1;
    }

    obj->is_busy = 1;

    nrf_qspi_cinstr_conf_t config;
    config.opcode    = cmd;
    config.io2_level = false;
    config.io3_level = false;
    config.wipwait   = false;
    config.wren      = false;

    if (data_size < 9)
    {
        config.length =NRF_QSPI_CINSTR_LEN_1B + data_size;
    }
    else
    {
        obj->is_busy = 0;
        return -1;
    }

    // preparing data to send
    for (i = 0; i < tx_size; ++i)
    {
        data[i] = tx_data[i];
    }

    ret_code = nrf_drv_qspi_cinstr_xfer(&config, data, data);

    if (ret_code != NRF_SUCCESS)
    {
        obj->is_busy = 0;
        return -1;
    }

    // preparing received data
    for (i = 0; i < rx_size; ++i)
    {
        // Data is sending as a normal SPI transmission so there is one buffer to send and receive data.
        rx_data[i] = data[i + tx_size];
    }

    obj->is_busy = 0;
    return 0;

}

int32_t qspi_is_busy(qspi_t *obj)
{
    return obj->is_busy;
}

#if DEVICE_QSPI_ASYNCH

uint32_t qspi_irq_handler_asynch(qspi_t *obj)
{
    MBED_ASSERT(obj);
    return obj->event & QSPI_EVENT_COMPLETE;
}

int32_t qspi_start_erase_block(qspi_t *obj, uint32_t address,
                               uint32_t handler, uint32_t event, DMAUsage hint)
{
    (void)hint;
    obj->handler = handler;
    obj->event   = event;
    return _qspi_erase_block(obj, address);
}

int32_t qspi_start_erase_chip(qspi_t *obj,
                              uint32_t handler, uint32_t event, DMAUsage hint)
{
    (void)hint;
    obj->handler = handler;
    obj->event   = event;
    return _qspi_erase_chip(obj);
}

int32_t qspi_start_program_page(qspi_t *obj, uint32_t address, const uint8_t *data, uint32_t size,
                                uint32_t handler, uint32_t event, DMAUsage hint)
{
    (void)hint;
    obj->handler = handler;
    obj->event   = event;
    return _qspi_program_page(obj, address, data, size);
}

int32_t qspi_start_page_read(qspi_t *obj, uint32_t address, uint8_t *data, uint32_t size,
                             uint32_t handler, uint32_t event, DMAUsage hint)
{
    (void)hint;
    obj->handler = handler;
    obj->event   = event;
    return _qspi_page_read(obj, address, data, size);
}

#endif

#endif

/** @}*/
