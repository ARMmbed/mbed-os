/*
 * Copyright (c) 2018 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#define IS_MASTER 1
#include "mbed_config.h"
#include "../spi_config/spi_test_common.h"

/* Function initialises RX, TX buffers before transmission. */
template<typename T>
void init_transmission_buffers(config_test_case_t *config, T *p_tx_pattern, T *p_rx1_pattern, T *p_rx2_pattern, T *p_tx_buff, T *p_rx_buff, T *p_fill_symbol)
{
    /* Default patterns for TX/RX buffers. */
    set_buffer(&p_tx_pattern[0], sizeof(T), 0xAA);
    set_buffer(&p_tx_pattern[1], sizeof(T), 0xBB);
    set_buffer(&p_tx_pattern[2], sizeof(T), 0xCC);
    set_buffer(&p_tx_pattern[3], sizeof(T), 0xDD);
    set_buffer(&p_tx_pattern[4], sizeof(T), 0xEE);

    set_buffer(&p_rx1_pattern[0], sizeof(T), 0x11);
    set_buffer(&p_rx1_pattern[1], sizeof(T), 0x22);
    set_buffer(&p_rx1_pattern[2], sizeof(T), 0x33);
    set_buffer(&p_rx1_pattern[3], sizeof(T), 0x44);
    set_buffer(&p_rx1_pattern[4], sizeof(T), 0x55);

    set_buffer(&p_rx2_pattern[0], sizeof(T), 0xAA);
    set_buffer(&p_rx2_pattern[1], sizeof(T), 0xBB);
    set_buffer(&p_rx2_pattern[2], sizeof(T), 0xCC);
    set_buffer(&p_rx2_pattern[3], sizeof(T), 0xDD);
    set_buffer(&p_rx2_pattern[4], sizeof(T), 0xEE);

    set_buffer(p_fill_symbol, sizeof(T), 0xFF);

    /* Exception: master TX > master RX . */
    if (config->master_tx_cnt > config->master_rx_cnt) {
        set_buffer(&p_rx1_pattern[3], sizeof(T), 0x00);
        set_buffer(&p_rx1_pattern[4], sizeof(T), 0x00);
        set_buffer(&p_rx2_pattern[3], sizeof(T), 0x00);
        set_buffer(&p_rx2_pattern[4], sizeof(T), 0x00);
    }
    /* Exception: master TX < master RX . */
    if (config->master_tx_cnt < config->master_rx_cnt) {
        set_buffer(&p_rx2_pattern[3], sizeof(T), 0xFF);
        set_buffer(&p_rx2_pattern[4], sizeof(T), 0xFF);
    }

    /* Exception: slave TX > slave RX . */
    if (config->slave_tx_cnt > config->slave_rx_cnt) {
        set_buffer(&p_rx2_pattern[3], sizeof(T), 0x00);
        set_buffer(&p_rx2_pattern[4], sizeof(T), 0x00);
    }
    /* Exception: slave TX < slave RX . */
    if (config->slave_tx_cnt < config->slave_rx_cnt) {
        set_buffer(&p_rx1_pattern[3], sizeof(T), 0xFF);
        set_buffer(&p_rx1_pattern[4], sizeof(T), 0xFF);
        set_buffer(&p_rx2_pattern[3], sizeof(T), 0xFF);
        set_buffer(&p_rx2_pattern[4], sizeof(T), 0xFF);
    }

    /* Exception: master TX buffer undefined . */
    if (!config->master_tx_defined) {
        set_buffer(&p_rx2_pattern[0], sizeof(T), 0xFF);
        set_buffer(&p_rx2_pattern[1], sizeof(T), 0xFF);
        set_buffer(&p_rx2_pattern[2], sizeof(T), 0xFF);
        set_buffer(&p_rx2_pattern[3], sizeof(T), 0xFF);
        set_buffer(&p_rx2_pattern[4], sizeof(T), 0xFF);
    }

    /* Exception: master RX buffer undefined . */
    if (!config->master_rx_defined) {
        set_buffer(&p_rx1_pattern[0], sizeof(T), 0xAA);
        set_buffer(&p_rx1_pattern[1], sizeof(T), 0xBB);
        set_buffer(&p_rx1_pattern[2], sizeof(T), 0xCC);
        set_buffer(&p_rx1_pattern[3], sizeof(T), 0xDD);
        set_buffer(&p_rx1_pattern[4], sizeof(T), 0xEE);
    }

    /* Exception: slave TX buffer undefined . */
    if (!config->slave_tx_defined) {
        set_buffer(&p_rx1_pattern[0], sizeof(T), 0xFF);
        set_buffer(&p_rx1_pattern[1], sizeof(T), 0xFF);
        set_buffer(&p_rx1_pattern[2], sizeof(T), 0xFF);
        set_buffer(&p_rx1_pattern[3], sizeof(T), 0xFF);
        set_buffer(&p_rx1_pattern[4], sizeof(T), 0xFF);

        set_buffer(&p_rx2_pattern[0], sizeof(T), 0xFF);
        set_buffer(&p_rx2_pattern[1], sizeof(T), 0xFF);
        set_buffer(&p_rx2_pattern[2], sizeof(T), 0xFF);
        set_buffer(&p_rx2_pattern[3], sizeof(T), 0xFF);
        set_buffer(&p_rx2_pattern[4], sizeof(T), 0xFF);
    }

    /* Exception: slave RX buffer undefined . */
    if (!config->slave_rx_defined) {
        set_buffer(&p_rx2_pattern[0], sizeof(T), 0x11);
        set_buffer(&p_rx2_pattern[1], sizeof(T), 0x22);
        set_buffer(&p_rx2_pattern[2], sizeof(T), 0x33);
        set_buffer(&p_rx2_pattern[3], sizeof(T), 0x44);
        set_buffer(&p_rx2_pattern[4], sizeof(T), 0x55);
    }

    /* Handle symbol size. */
    T sym_mask = ((1 << config->symbol_size) - 1);

    for (uint32_t i = 0; i < TEST_SYM_CNT; i++) {
        p_tx_pattern[i] = (p_tx_pattern[i] & sym_mask);
        p_rx1_pattern[i] = (p_rx1_pattern[i] & sym_mask);
        p_rx2_pattern[i] = (p_rx2_pattern[i] & sym_mask);
    }

    memcpy(p_tx_buff, p_tx_pattern, sizeof(T) * TEST_SYM_CNT);
    set_buffer(p_rx_buff, sizeof(T) * TEST_SYM_CNT, 0x00);
}

/* Function handles test_init command. */
int test_init_master(spi_t * obj, config_test_case_t *config, DigitalOut ** ss)
{
    spi_capabilities_t capabilities = { 0 };
    PinName ss_pin = SPI_SS;
    PinName miso = SPI_MISO;
    PinName mosi = SPI_MOSI;

    spi_get_capabilities(spi_get_module(SPI_MOSI, SPI_MISO, SPI_CLK), NC, &capabilities);

    /* Adapt Full Duplex/Half Duplex settings. */
    switch (config->duplex)
    {
        case HALF_DUPLEX_MOSI:
            miso = NC;
            break;

        case HALF_DUPLEX_MISO:
            mosi = NC;
            break;

        default:
            break;
    }

    /* Adapt min/max frequency for testing based of capabilities. */
    switch (config->freq_hz)
    {
        case FREQ_MIN:
            config->freq_hz = capabilities.minimum_frequency;
            break;

        case FREQ_MAX:
            config->freq_hz = capabilities.maximum_frequency;
            break;

        default:
            break;
    }

    /* Adapt manual/auto SS handling by master. */
    if (!config->auto_ss) {
        ss_pin = NC;
        *ss = new DigitalOut(SPI_SS);
        **ss = 1;
    }

    spi_init(obj, false, mosi, miso, SPI_CLK, ss_pin);

    spi_format(obj, config->symbol_size, config->mode, config->bit_ordering);

    spi_frequency(obj, config->freq_hz);

    return CMDLINE_RETCODE_SUCCESS;
}

/* Function handles test_transfer command. */
int test_transfer_master(spi_t * obj, config_test_case_t *config, DigitalOut * ss)
{
    int status = CMDLINE_RETCODE_SUCCESS;

    if (config->symbol_size <= 8) {
        status = perform_transfer<uint8_t>(obj, config, ss);
    } else if (config->symbol_size <= 16) {
        status = perform_transfer<uint16_t>(obj,config, ss);
    } else {
        status = perform_transfer<uint32_t>(obj,config, ss);
    }

    return status;
}

/* Function handles test_finish command. */
int test_finish_master(spi_t * obj, config_test_case_t *config)
{
    spi_free(obj);

    return CMDLINE_RETCODE_SUCCESS;
}

