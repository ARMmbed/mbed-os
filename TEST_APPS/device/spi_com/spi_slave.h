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

/* Function initialises RX, TX buffers before transmission. */
template<typename T>
void init_transmission_buffers_slave(config_test_case_t *config, T *p_tx_pattern, T *p_rx1_pattern, T *p_rx2_pattern,
                                     T *p_tx_buff, T *p_rx_buff, T *p_fill_symbol)
{
    uint8_t rx_pattern[] = { 0xAA, 0xBB, 0xCC, 0xDD, 0xEE };
    uint8_t tx_pattern[] = { 0x11, 0x22, 0x33, 0x44, 0x55 };

    /* Default patterns for TX/RX buffers. */
    init_buffer_with_pattern(p_tx_pattern, sym_count, sizeof(T), tx_pattern, sizeof(tx_pattern));
    init_buffer_with_pattern(p_rx1_pattern, sym_count, sizeof(T), rx_pattern, sizeof(rx_pattern));
    init_buffer_with_pattern(p_rx2_pattern, sym_count, sizeof(T), tx_pattern, sizeof(tx_pattern));

    set_buffer(p_fill_symbol, sizeof(T), 0xF5);

    /* Exception: master TX > master RX . */
    if (config->master_tx_cnt > config->master_rx_cnt) {
        set_buffer(&p_rx2_pattern[3], sizeof(T), 0x00);
        set_buffer(&p_rx2_pattern[4], sizeof(T), 0x00);
    }
    /* Exception: master TX < master RX . */
    if (config->master_tx_cnt < config->master_rx_cnt) {
        set_buffer(&p_rx1_pattern[3], sizeof(T), 0xF5);
        set_buffer(&p_rx1_pattern[4], sizeof(T), 0xF5);
    }

    /* Exception: slave TX > slave RX . */
    if (config->slave_tx_cnt > config->slave_rx_cnt) {
        set_buffer(&p_rx1_pattern[3], sizeof(T), 0x00);
        set_buffer(&p_rx1_pattern[4], sizeof(T), 0x00);
    }
    /* Exception: slave TX < slave RX . */
    if (config->slave_tx_cnt < config->slave_rx_cnt) {
        set_buffer(&p_rx2_pattern[3], sizeof(T), 0xF5);
        set_buffer(&p_rx2_pattern[4], sizeof(T), 0xF5);
    }

    /* Exception: master TX buffer undefined . */
    if (!config->master_tx_defined) {
        set_buffer(&p_rx1_pattern[0], sizeof(T), 0xF5);
        set_buffer(&p_rx1_pattern[1], sizeof(T), 0xF5);
        set_buffer(&p_rx1_pattern[2], sizeof(T), 0xF5);
        set_buffer(&p_rx1_pattern[3], sizeof(T), 0xF5);
        set_buffer(&p_rx1_pattern[4], sizeof(T), 0xF5);

        set_buffer(&p_rx2_pattern[0], sizeof(T), 0xF5);
        set_buffer(&p_rx2_pattern[1], sizeof(T), 0xF5);
        set_buffer(&p_rx2_pattern[2], sizeof(T), 0xF5);
        set_buffer(&p_rx2_pattern[3], sizeof(T), 0xF5);
        set_buffer(&p_rx2_pattern[4], sizeof(T), 0xF5);
    }

    /* Exception: master RX buffer undefined . */
    if (!config->master_rx_defined) {
        set_buffer(&p_rx2_pattern[0], sizeof(T), 0xAA);
        set_buffer(&p_rx2_pattern[1], sizeof(T), 0xBB);
        set_buffer(&p_rx2_pattern[2], sizeof(T), 0xCC);
        set_buffer(&p_rx2_pattern[3], sizeof(T), 0xDD);
        set_buffer(&p_rx2_pattern[4], sizeof(T), 0xEE);
    }

    /* Exception: slave TX buffer undefined . */
    if (!config->slave_tx_defined) {
        set_buffer(&p_rx2_pattern[0], sizeof(T), 0xF5);
        set_buffer(&p_rx2_pattern[1], sizeof(T), 0xF5);
        set_buffer(&p_rx2_pattern[2], sizeof(T), 0xF5);
        set_buffer(&p_rx2_pattern[3], sizeof(T), 0xF5);
        set_buffer(&p_rx2_pattern[4], sizeof(T), 0xF5);
    }

    /* Exception: slave RX buffer undefined . */
    if (!config->slave_rx_defined) {
        set_buffer(&p_rx1_pattern[0], sizeof(T), 0xAA);
        set_buffer(&p_rx1_pattern[1], sizeof(T), 0xBB);
        set_buffer(&p_rx1_pattern[2], sizeof(T), 0xCC);
        set_buffer(&p_rx1_pattern[3], sizeof(T), 0xDD);
        set_buffer(&p_rx1_pattern[4], sizeof(T), 0xEE);
    }

    /* Handle symbol size. */
    T sym_mask = ((1 << config->symbol_size) - 1);

    for (uint32_t i = 0; i < sym_count; i++) {
        p_tx_pattern[i] = (p_tx_pattern[i] & sym_mask);
        p_rx1_pattern[i] = (p_rx1_pattern[i] & sym_mask);
        p_rx2_pattern[i] = (p_rx2_pattern[i] & sym_mask);
    }

    memcpy(p_tx_buff, p_tx_pattern, sizeof(T) * sym_count);
    set_buffer(p_rx_buff, sizeof(T) * sym_count, 0x00);
}

/* Function handles test_init command. */
int test_init_slave(spi_t *obj, config_test_case_t *config)
{
    spi_capabilities_t capabilities = { 0 };

    spi_get_capabilities(spi_get_module(SLAVE_SPI_MOSI,
                                        SLAVE_SPI_MISO,
                                        SLAVE_SPI_CLK),
                         SLAVE_SPI_SS, &capabilities);

    PinName miso = SLAVE_SPI_MISO;
    PinName mosi = SLAVE_SPI_MOSI;

    /* Adapt Half Duplex settings. */
    if (config->duplex == HALF_DUPLEX) {
        if (SLAVE_SPI_HF_DATA == SLAVE_SPI_MOSI) {
            miso = NC;
        } else if (SLAVE_SPI_HF_DATA == SLAVE_SPI_MISO) {
            mosi = NC;
        } else {
            printf("ERROR: Slave init in half duplex mode. \r\n ");
        }
    }

    spi_init(obj, true, mosi, miso, SLAVE_SPI_CLK, SLAVE_SPI_SS);

    spi_format(obj, config->symbol_size, config->mode, config->bit_ordering);

    return CMDLINE_RETCODE_SUCCESS;
}

/* Function handles test_transfer command. */
int test_transfer_slave(spi_t *obj, config_test_case_t *config)
{
    Timeout transm_thread_timeout;
    Thread transm_thread(osPriorityNormal);

    int status = CMDLINE_RETCODE_SUCCESS;
    trans_thread_params_t trans_thread_params = { obj, SLAVE, config, NULL, CMDLINE_RETCODE_SUCCESS };

    /* Set slave transmission test timeout to 5 sec + master delays */
    transm_thread_timeout.attach_us(transm_timeout_handler,
                                    5 * US_PER_S + 2 * MASTER_TRANSMISSION_DELAY_MS * US_PER_MS);

    transfer_finished = false;
    transm_thread.set_priority(osPriorityNormal);

    transm_thread.start(callback(transfer_thread, (void *) &trans_thread_params));

    transm_sem.wait();

    if (!transfer_finished) {
        transm_thread.terminate();
        printf("ERROR: Slave transmission timeout. \r\n ");
    }

    return status;
}

/* Function handles test_finish command. */
int test_finish_slave(spi_t *obj, config_test_case_t *config)
{
    spi_free(obj);

    return CMDLINE_RETCODE_SUCCESS;
}

