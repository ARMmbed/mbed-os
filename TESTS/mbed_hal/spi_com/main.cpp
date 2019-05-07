/*
 * Copyright (c) 2018 ARM Limited
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

#define MANUAL_TEST 0
#if (!MANUAL_TEST)
#error [NOT_SUPPORTED] Skip CI - designed for manual testing only.
#endif

#ifndef DEVICE_SPI
#error [NOT_SUPPORTED] SPI is not supported on this platform, add 'DEVICE_SPI' definition to your platform.
#endif

#include "mbed.h"
#include <ctype.h>

#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "spi_com_test.h"

#if DEVICE_SPI_SLAVE
#error [NOT_SUPPORTED] This test can only be run on device that support SPI Master AND Slave.
#endif
#if DEVICE_SPI_COUNT < 2
#error [NOT_SUPPORTED] This test can only be run on device with at least two SPI peripherals.
#endif

using namespace utest::v1;

/* Enable debug mode to dump rx/tx buffers.*/
#define DEBUG 0

/* This must be changed if SS active state is 1. */
#define SS_ACTIVE (0)
#define SS_INACTIVE (!SS_ACTIVE)

#define FREQ_200KHZ (200000)
#define FREQ_1MHZ   (1000000)
#define FREQ_2MHZ   (2000000)
#define FREQ_MIN    (0)
#define FREQ_MAX    (0xFFFFFFFF)

#define TEST_SYM_CNT 5

#ifndef MIN
#define MIN(a, b) (((a)>(b))?(b):(a))
#endif

#ifndef MAX
#define MAX(a, b) ((a>b)?(a):(b))
#endif

#define TEST_CAPABILITY_BIT(MASK, CAP) ((1 << CAP) & (MASK))

typedef enum {
    FULL_DUPLEX, HALF_DUPLEX
} duplex_t;

/* Test context structure. */
typedef struct {
    const void *tx;
    uint32_t tx_len;
    void *rx;
    uint32_t rx_len;
    void *fill_symbol;
    DigitalOut *ss;
    uint8_t bits;
    volatile uint32_t count; // test output
    volatile bool has_error;
} test_context_t;

/* SPI test configuration. */
typedef struct {
    uint8_t symbol_size;
    spi_mode_t mode;
    spi_bit_ordering_t bit_ordering;
    uint32_t freq_hz;
    uint32_t master_tx_cnt;
    uint32_t master_rx_cnt;
    uint32_t slave_tx_cnt;
    uint32_t slave_rx_cnt;
    bool master_tx_defined;
    bool master_rx_defined;
    bool slave_tx_defined;
    bool slave_rx_defined;
    bool auto_ss;
    duplex_t duplex;
    bool sync;
} config_test_case_t;

/* Semaphore used to control master/slave SPI communication */
Semaphore join(0, 2);
Timer t;

static spi_t slave = {0};
static spi_t master = {0};

static const uint8_t tx_master[] = "0123456789abcdefghijklmnopqrstuvwxyz=+/-*!?";
static const uint8_t tx_slave[]  = "zyxwvutsrqponmlkjihgfedcba9876543210=+/-*!?";

static uint8_t rx_slave[sizeof(tx_master) + 1] = {0};
static uint8_t rx_master[sizeof(tx_slave) + 1] = {0};
static uint8_t rx_pattern_slave[sizeof(tx_master) + 1] = {0};
static uint8_t rx_pattern_master[sizeof(tx_slave) + 1] = {0};

/* Array witch test cases which represents different SPI configurations for testing. */
static config_test_case_t test_cases[] = {
    /* default config: 8 bit symbol\sync mode\full duplex\clock idle low\sample on the first clock edge\MSB first\200 KHz clock\manual SS handling */
    /* 00 */{8, SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ, TEST_SYM_CNT, TEST_SYM_CNT, TEST_SYM_CNT, TEST_SYM_CNT, true, true, true, true, false, FULL_DUPLEX  },
    /* symbol size testing */
    /* 01 */{1, SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ, TEST_SYM_CNT, TEST_SYM_CNT, TEST_SYM_CNT, TEST_SYM_CNT, true, true, true, true, false, FULL_DUPLEX  },
    /* 02 */{7, SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ, TEST_SYM_CNT, TEST_SYM_CNT, TEST_SYM_CNT, TEST_SYM_CNT, true, true, true, true, false, FULL_DUPLEX  },
    /* 03 */{9, SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ, TEST_SYM_CNT, TEST_SYM_CNT, TEST_SYM_CNT, TEST_SYM_CNT, true, true, true, true, false, FULL_DUPLEX  },
    /* 04 */{15, SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ, TEST_SYM_CNT, TEST_SYM_CNT, TEST_SYM_CNT, TEST_SYM_CNT, true, true, true, true, false, FULL_DUPLEX  },
    /* 05 */{16, SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ, TEST_SYM_CNT, TEST_SYM_CNT, TEST_SYM_CNT, TEST_SYM_CNT, true, true, true, true, false, FULL_DUPLEX  },
    /* 06 */{17, SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ, TEST_SYM_CNT, TEST_SYM_CNT, TEST_SYM_CNT, TEST_SYM_CNT, true, true, true, true, false, FULL_DUPLEX  },
    /* 07 */{31, SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ, TEST_SYM_CNT, TEST_SYM_CNT, TEST_SYM_CNT, TEST_SYM_CNT, true, true, true, true, false, FULL_DUPLEX  },
    /* 08 */{32, SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ, TEST_SYM_CNT, TEST_SYM_CNT, TEST_SYM_CNT, TEST_SYM_CNT, true, true, true, true, false, FULL_DUPLEX  },
    /* mode testing */
    /* 09 */{8, SPI_MODE_IDLE_LOW_SAMPLE_SECOND_EDGE, SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ, TEST_SYM_CNT, TEST_SYM_CNT, TEST_SYM_CNT, TEST_SYM_CNT, true, true, true, true, false, FULL_DUPLEX  },
    /* 10 */{8, SPI_MODE_IDLE_HIGH_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ, TEST_SYM_CNT, TEST_SYM_CNT, TEST_SYM_CNT, TEST_SYM_CNT, true, true, true, true, false, FULL_DUPLEX  },
    /* 11 */{8, SPI_MODE_IDLE_HIGH_SAMPLE_SECOND_EDGE, SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ, TEST_SYM_CNT, TEST_SYM_CNT, TEST_SYM_CNT, TEST_SYM_CNT, true, true, true, true, false, FULL_DUPLEX  },
    /* bit ordering testing */
    /* 12 */{8, SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_LSB_FIRST, FREQ_200KHZ, TEST_SYM_CNT, TEST_SYM_CNT, TEST_SYM_CNT, TEST_SYM_CNT, true, true, true, true, false, FULL_DUPLEX  },
    /* freq testing */
    /* 13 */{8, SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ, TEST_SYM_CNT, TEST_SYM_CNT, TEST_SYM_CNT, TEST_SYM_CNT, true, true, true, true, false, FULL_DUPLEX  },
    /* 14 */{8, SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_MSB_FIRST, FREQ_2MHZ, TEST_SYM_CNT, TEST_SYM_CNT, TEST_SYM_CNT, TEST_SYM_CNT, true, true, true, true, false, FULL_DUPLEX  },
    /* 15 */{8, SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_MSB_FIRST, FREQ_MIN, TEST_SYM_CNT, TEST_SYM_CNT, TEST_SYM_CNT, TEST_SYM_CNT, true, true, true, true, false, FULL_DUPLEX  },
    /* 16 */{8, SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_MSB_FIRST, FREQ_MAX, TEST_SYM_CNT, TEST_SYM_CNT, TEST_SYM_CNT, TEST_SYM_CNT, true, true, true, true, false, FULL_DUPLEX  },
    /* master: TX > RX */
    /* 17 */{8, SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ, TEST_SYM_CNT, TEST_SYM_CNT - 2, TEST_SYM_CNT, TEST_SYM_CNT, true, true, true, true, false, FULL_DUPLEX  },
    /* master: TX < RX */
    /* 18 */{8, SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ, TEST_SYM_CNT - 2, TEST_SYM_CNT, TEST_SYM_CNT, TEST_SYM_CNT, true, true, true, true, false, FULL_DUPLEX  },
    /* slave: TX > RX */
    /* 19 */{8, SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ, TEST_SYM_CNT, TEST_SYM_CNT, TEST_SYM_CNT, TEST_SYM_CNT - 2, true, true, true, true, false, FULL_DUPLEX  },
    /* slave: TX < RX */
    /* 20 */{8, SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ, TEST_SYM_CNT, TEST_SYM_CNT, TEST_SYM_CNT - 2, TEST_SYM_CNT, true, true, true, true, false, FULL_DUPLEX  },
    /* master tx buffer undefined */
    /* 21 */{8, SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ, TEST_SYM_CNT, TEST_SYM_CNT, TEST_SYM_CNT, TEST_SYM_CNT, false, true, true, true, false, FULL_DUPLEX  },
    /* master rx buffer undefined */
    /* 22 */{8, SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ, TEST_SYM_CNT, TEST_SYM_CNT, TEST_SYM_CNT, TEST_SYM_CNT, true, false, true, true, false, FULL_DUPLEX  },
    /* slave tx buffer undefined */
    /* 23 */{8, SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ, TEST_SYM_CNT, TEST_SYM_CNT, TEST_SYM_CNT, TEST_SYM_CNT, true, true, false, true, false, FULL_DUPLEX  },
    /* slave rx buffer undefined */
    /* 24 */{8, SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ, TEST_SYM_CNT, TEST_SYM_CNT, TEST_SYM_CNT, TEST_SYM_CNT, true, true, true, false, false, FULL_DUPLEX  },
    /* auto ss hadling by master */
    /* 25 */{8, SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ, TEST_SYM_CNT, TEST_SYM_CNT, TEST_SYM_CNT, TEST_SYM_CNT, true, true, true, true, false, FULL_DUPLEX  },
    /* one symbol */
    /* 26 */{8, SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ, 1, 1, 1, 1, true, true, true, true, false, FULL_DUPLEX  },
    /* long buffer */
    /* 27 */{8, SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ, 32, 32, 32, 32, true, true, true, true, false, FULL_DUPLEX  },
};

/* Function returns true if configuration is consistent with the capabilities of
 * the SPI peripheral, false otherwise. */
static bool check_capabilities(spi_capabilities_t *p_cabs, config_test_case_t *tc_config, bool slave)
{
    printf("word_len: 0x%X, symbol_size: %d \r\n", p_cabs->word_length, tc_config->symbol_size);

    if ((!TEST_CAPABILITY_BIT(p_cabs->word_length, (tc_config->symbol_size - 1))) ||
            (!TEST_CAPABILITY_BIT(p_cabs->clk_modes, tc_config->mode)) ||
            (!TEST_CAPABILITY_BIT(p_cabs->bit_order, tc_config->bit_ordering)) ||
            (!p_cabs->support_slave_mode) ||
            (tc_config->freq_hz != FREQ_MAX && tc_config->freq_hz != FREQ_MIN && tc_config->freq_hz < p_cabs->minimum_frequency && tc_config->freq_hz > p_cabs->maximum_frequency) ||
            (!tc_config->sync && !p_cabs->async_mode) ||
            (tc_config->duplex == HALF_DUPLEX && !p_cabs->half_duplex) ||
            (tc_config->auto_ss && !p_cabs->hw_cs_handle)
       ) {
        return false;
    }

    return true;
}

/* Function sets Slave Select signal. */
static void toggle_ss(DigitalOut *pin, bool value)
{
    if (pin != NULL) {
        *pin = value;
    }
}

/* Slave thread used for communication test. */
static void fn_slave(void *vctx)
{
    test_context_t *ctx = (test_context_t *)vctx;
    ctx->count = 0;
    ctx->count = spi_transfer(&slave, ctx->tx, ctx->tx_len, ctx->rx, ctx->rx_len, ctx->fill_symbol);
    join.release();
}

/* Master thread used for communication test. */
static void fn_master(void *vctx)
{
    test_context_t *ctx = (test_context_t *)vctx;
    uint32_t factor = 1;
    if (ctx->bits > 16) {
        factor = 4;
    } else if (ctx->bits > 8) {
        factor = 2;
    }

    const uint8_t *tx = (const uint8_t *)ctx->tx;
    uint8_t *rx = (uint8_t *)ctx->rx;
    uint8_t *rx_ptr = NULL;
    uint32_t rx_len = 0;
    ctx->count = 0;

    toggle_ss(ctx->ss, SS_ACTIVE);
    for (uint32_t i = 0; i < ctx->tx_len; i++) {
        if (i < ctx->rx_len && rx) {
            rx_ptr = &rx[i * factor];
            rx_len = 1;
        } else {
            rx_ptr = NULL;
            rx_len = 0;
        }
        ctx->count += spi_transfer(&master, &tx[i * factor], 1, rx_ptr, rx_len, ctx->fill_symbol);
        wait_ms(3);
    }
    if (ctx->tx_len < ctx->rx_len) {
        for (uint32_t i = ctx->tx_len; i < ctx->rx_len; i++) {
            ctx->count += spi_transfer(&master, NULL, 0, &rx[i * factor], 1, ctx->fill_symbol);
            wait_ms(3);
        }
    }
    toggle_ss(ctx->ss, SS_INACTIVE);

    join.release();
}

/* Slave interrupt handler for async transfer. */
static void slave_handler(spi_t *obj, void *vctx, spi_async_event_t *event)
{
    test_context_t *ctx = (test_context_t *)vctx;
    ctx->count = event->transfered;
    ctx->has_error = event->error;

    if (ctx->has_error) {
        printf("Slave error: %d \n", event->error);
        printf("Slave transfered: %lu \n", event->transfered);
    }

    join.release();
}

/* Master interrupt handler for async transfer. */
static void master_handler(spi_t *obj, void *vctx, spi_async_event_t *event)
{
    test_context_t *ctx = (test_context_t *)vctx;
    ctx->count = event->transfered;
    ctx->has_error = event->error;

    if (ctx->has_error) {
        printf("Master error: %d \n", event->error);
        printf("Master transfered: %lu \n", event->transfered);
    }

    join.release();
}

/* Function compares transmission buffers and returns true on success, false otherwise. */
template<typename T> bool check_buffer(const T *a, const T *b, T fill, uint32_t a_len, uint32_t b_len)
{
    static bool res = memcmp(a, b, a_len * sizeof(T)) == 0;
    for (uint32_t i = a_len; (i < b_len) && res; i++) {
        res &= b[i] == fill;
    }
    return res;
}

/* Function which perform SPI transfer test using specified config. */
template<typename T, const uint32_t tc_id, bool sync>
void test_spi_transfer()
{
    DigitalOut *mcs_pin = NULL;
    PinName mcs = SPI_TEST_MASTER_CS;
    PinName mmclk = SPI_TEST_MASTER_SCK;
    PinName mmiso = SPI_TEST_MASTER_MISO;
    PinName mmosi = SPI_TEST_MASTER_MOSI;
    PinName scs = SPI_TEST_SLAVE_CS;
    PinName smclk = SPI_TEST_SLAVE_SCK;
    PinName smiso = SPI_TEST_SLAVE_MISO;
    PinName smosi = SPI_TEST_SLAVE_MOSI;
    uint32_t freq_hz = test_cases[tc_id].freq_hz;
    T symbol_mask = (T)((1 << test_cases[tc_id].symbol_size) - 1);

    uint32_t master_symbols_clocked = MAX(test_cases[tc_id].master_tx_cnt, test_cases[tc_id].master_rx_cnt);
    uint32_t slave_symbols_clocked = MAX(test_cases[tc_id].slave_tx_cnt, test_cases[tc_id].slave_rx_cnt);

    spi_capabilities_t master_capab = { 0 };
    spi_capabilities_t slave_capab = { 0 };

    Thread *tmaster;
    Thread *tslave;
    test_context_t master_ctx = { 0 };
    test_context_t slave_ctx = { 0 };

    T *p_tx_master = (T *)tx_master;
    T *p_rx_master = (T *)rx_master;
    T *p_tx_slave = (T *)tx_slave;
    T *p_rx_slave = (T *)rx_slave;
    T *p_rx_pattern_slave = (T *)rx_pattern_slave;
    T *p_rx_pattern_master = (T *)rx_pattern_master;
    T master_fill = (T)0x58585858; // X
    T slave_fill = (T)0x59595959;  // Y

    memset(rx_slave, 0, sizeof(rx_slave));
    memset(rx_master, 0, sizeof(rx_master));

    memcpy(rx_pattern_slave, tx_master, sizeof(tx_master));
    memcpy(rx_pattern_master, tx_slave, sizeof(tx_slave));

    for (uint32_t i = 0; i < test_cases[tc_id].slave_rx_cnt; i++) {
        p_rx_pattern_slave[i] &= symbol_mask;
    }

    for (uint32_t i = 0; i < test_cases[tc_id].master_rx_cnt; i++) {
        p_rx_pattern_master[i] &= symbol_mask;
    }

    if (!test_cases[tc_id].master_tx_defined) {
        p_tx_master = NULL;
    }
    if (!test_cases[tc_id].master_rx_defined) {
        p_rx_master = NULL;
    }

    spi_get_capabilities(
        spi_get_module(mmosi, mmiso, mmclk),
        scs,
        false,
        &master_capab
    );
    spi_get_capabilities(
        spi_get_module(smosi, smiso, smclk),
        scs,
        true,
        &slave_capab
    );

    if (!check_capabilities(&master_capab, &test_cases[tc_id], false) ||
            !check_capabilities(&slave_capab, &test_cases[tc_id], true)) {
        TEST_SKIP_MESSAGE("Configuration not supported. Skipping. \n");
    }

    /* Adapt min/max frequency for testing based of capabilities. */
    switch (freq_hz) {
        case FREQ_MIN:
            freq_hz = ((master_capab.minimum_frequency > slave_capab.minimum_frequency) ? master_capab.minimum_frequency : slave_capab.minimum_frequency);
            break;
        case FREQ_MAX:
            freq_hz = ((master_capab.maximum_frequency < slave_capab.maximum_frequency) ? master_capab.maximum_frequency : slave_capab.maximum_frequency);
            break;
        default:
            break;
    }

    /* Adapt manual/auto SS handling by master. */
    if (!test_cases[tc_id].auto_ss) {
        mcs_pin = new DigitalOut(mcs);
        mcs = NC;
        *mcs_pin = SS_INACTIVE;
    }

    spi_init(&slave, true, smosi, smiso, smclk, scs);
    spi_init(&master, false, mmosi, mmiso, mmclk, mcs);

    spi_format(&slave, test_cases[tc_id].symbol_size, test_cases[tc_id].mode, test_cases[tc_id].bit_ordering);
    spi_format(&master, test_cases[tc_id].symbol_size, test_cases[tc_id].mode, test_cases[tc_id].bit_ordering);

    spi_frequency(&master, freq_hz);

    t.reset();
    t.start();

    slave_ctx.tx = p_tx_slave;
    slave_ctx.tx_len = test_cases[tc_id].slave_tx_cnt;
    slave_ctx.rx = p_rx_slave;
    slave_ctx.rx_len = test_cases[tc_id].slave_rx_cnt;
    slave_ctx.fill_symbol = &slave_fill;
    slave_ctx.ss = NULL;
    slave_ctx.bits = test_cases[tc_id].symbol_size;

    master_ctx.tx = p_tx_master;
    master_ctx.tx_len = test_cases[tc_id].master_tx_cnt;
    master_ctx.rx = p_rx_master;
    master_ctx.rx_len = test_cases[tc_id].master_rx_cnt;
    master_ctx.fill_symbol = &master_fill;
    master_ctx.ss = mcs_pin;
    master_ctx.bits = test_cases[tc_id].symbol_size;

    /* In sync mode use two threads to handle SPI transmission. */
    if (sync) {
        tmaster = new Thread();
        MBED_ASSERT(tmaster != NULL);

        tslave = new Thread();
        MBED_ASSERT(tslave != NULL);

        tslave->start(callback(fn_slave, &slave_ctx));
        tmaster->start(callback(fn_master, &master_ctx));
    } else {
        spi_transfer_async(&slave,
                           p_tx_slave, slave_ctx.tx_len,
                           p_rx_slave, slave_ctx.rx_len,
                           &slave_fill, slave_handler, &slave_ctx, DMA_USAGE_OPPORTUNISTIC);

        toggle_ss(mcs_pin, SS_ACTIVE);
        spi_transfer_async(&master,
                           p_tx_master, master_ctx.tx_len,
                           p_rx_master, master_ctx.rx_len,
                           &master_fill, master_handler, &master_ctx, DMA_USAGE_OPPORTUNISTIC);
    }
    uint32_t i = 0;
    uint64_t cnt = 0;

    /* Try to perform test within 2 sec timeout. */
    while ((i < 2) && (t.read_ms() < 2000)) {
        if (join.wait(0) != 0) {
            i += 1;
        }
        cnt += 1;
    }

    if (sync) {
        tmaster->terminate();
        delete tmaster;
        tslave->terminate();
        delete tslave;
    }

    t.stop();
    toggle_ss(mcs_pin, SS_INACTIVE);
    spi_free(&master);
    spi_free(&slave);

#if DEBUG
    printf("Test context:\n");
    printf("Master: tx_sym_cnt: %lu tx:%.*s rx_sym_cnt: %lu\n", master_ctx.tx_len, (int)master_ctx.tx_len * sizeof(T), (char *)master_ctx.tx, master_ctx.rx_len);
    printf("Slave:  tx_sym_cnt: %lu tx:%.*s rx_sym_cnt: %lu\n", slave_ctx.tx_len, (int)slave_ctx.tx_len * sizeof(T), (char *)slave_ctx.tx, slave_ctx.rx_len);
    printf("Ran in %d ms (cnt = %llu) : %f\n", t.read_ms(), cnt, (float)cnt / (float)t.read_ms());
    printf("Slave  sent  : %.*s\n", (int)slave_ctx.tx_len * sizeof(T), (char *)tx_slave);
    printf("Master recved: %.*s\n", (int)master_ctx.rx_len * sizeof(T), (char *)rx_master);
    printf("Master sent  : %.*s\n", (int)master_ctx.tx_len * sizeof(T), (char *)tx_master);
    printf("Slave  recved: %.*s\n", (int)slave_ctx.rx_len * sizeof(T), (char *)rx_slave);
#endif

    TEST_ASSERT(!master_ctx.has_error);
    TEST_ASSERT(!slave_ctx.has_error);
    TEST_ASSERT_EQUAL(master_symbols_clocked, master_ctx.count);
    TEST_ASSERT_EQUAL(slave_symbols_clocked, slave_ctx.count);

    if ((p_tx_slave != NULL) && (p_rx_master != NULL)) {
        uint32_t n = MIN(slave_ctx.tx_len, master_ctx.rx_len);
        TEST_ASSERT_EQUAL(1, check_buffer((T *)rx_pattern_master, p_rx_master, slave_fill, n, master_ctx.rx_len));
    }
    if ((p_tx_master != NULL) && (p_rx_slave != NULL)) {
        uint32_t n = MIN(slave_ctx.rx_len, master_ctx.tx_len);
        TEST_ASSERT_EQUAL(1, check_buffer((T *)rx_pattern_slave, p_rx_slave, master_fill, n, slave_ctx.rx_len));
    }
    if (mcs_pin != NULL) {
        delete mcs_pin;
    }
}

utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(40, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("SPI master-slave sync com - default config", test_spi_transfer<uint8_t, 0, true>),
    Case("SPI master-slave sync com - symbol size: 1", test_spi_transfer<uint8_t, 1, true>),
    Case("SPI master-slave sync com - symbol size: 7", test_spi_transfer<uint8_t, 2, true>),
    Case("SPI master-slave sync com - symbol size: 9", test_spi_transfer<uint16_t, 3, true>),
    Case("SPI master-slave sync com - symbol size: 15", test_spi_transfer<uint16_t, 4, true>),
    Case("SPI master-slave sync com - symbol size: 16", test_spi_transfer<uint16_t, 5, true>),
    Case("SPI master-slave sync com - symbol size: 17", test_spi_transfer<uint32_t, 6, true>),
    Case("SPI master-slave sync com - symbol size: 31", test_spi_transfer<uint32_t, 7, true>),
    Case("SPI master-slave sync com - symbol size: 32", test_spi_transfer<uint32_t, 8, true>),
    Case("SPI master-slave sync com - mode: idle low, sample second edge", test_spi_transfer<uint8_t, 9, true>),
    Case("SPI master-slave sync com - mode: idle high, sample first edge", test_spi_transfer<uint8_t, 10, true>),
    Case("SPI master-slave sync com - mode: idle high, sample second edge", test_spi_transfer<uint8_t, 11, true>),
    Case("SPI master-slave sync com - bit ordering: LSB first", test_spi_transfer<uint8_t, 12, true>),
    Case("SPI master-slave sync com - freq testing: 200 KHz", test_spi_transfer<uint8_t, 13, true>),
    Case("SPI master-slave sync com - freq testing: 2 MHz", test_spi_transfer<uint8_t, 14, true>),
    Case("SPI master-slave sync com - freq testing: min defined", test_spi_transfer<uint8_t, 15, true>),
    Case("SPI master-slave sync com - freq testing: max defined", test_spi_transfer<uint8_t, 16, true>),
    Case("SPI master-slave sync com - master: TX > RX", test_spi_transfer<uint8_t, 17, true>),
    Case("SPI master-slave sync com - master: TX < RX", test_spi_transfer<uint8_t, 18, true>),
    Case("SPI master-slave sync com - slave: TX > RX", test_spi_transfer<uint8_t, 19, true>),
    Case("SPI master-slave sync com - slave: TX < RX", test_spi_transfer<uint8_t, 20, true>),
    Case("SPI master-slave sync com - master: TX undefined", test_spi_transfer<uint8_t, 21, true>),
    Case("SPI master-slave sync com - master: RX undefined", test_spi_transfer<uint8_t, 22, true>),
    Case("SPI master-slave sync com - slave: TX undefined", test_spi_transfer<uint8_t, 23, true>),
    Case("SPI master-slave sync com - slave: RX undefined", test_spi_transfer<uint8_t, 24, true>),
    Case("SPI master-slave sync com - master: manual ss", test_spi_transfer<uint8_t, 25, true>),
    Case("SPI master-slave sync com - one symbol", test_spi_transfer<uint8_t, 26, true>),
    Case("SPI master-slave sync com - long buffer", test_spi_transfer<uint8_t, 27, true>),
#ifdef DEVICE_SPI_ASYNCH
    Case("SPI master-slave async com - default config", test_spi_transfer<uint8_t, 0, false>),
    Case("SPI master-slave async com - symbol size: 1", test_spi_transfer<uint8_t, 1, false>),
    Case("SPI master-slave async com - symbol size: 7", test_spi_transfer<uint8_t, 2, false>),
    Case("SPI master-slave async com - symbol size: 9", test_spi_transfer<uint16_t, 3, false>),
    Case("SPI master-slave async com - symbol size: 15", test_spi_transfer<uint16_t, 4, false>),
    Case("SPI master-slave async com - symbol size: 16", test_spi_transfer<uint16_t, 5, false>),
    Case("SPI master-slave async com - symbol size: 17", test_spi_transfer<uint32_t, 6, false>),
    Case("SPI master-slave async com - symbol size: 31", test_spi_transfer<uint32_t, 7, false>),
    Case("SPI master-slave async com - symbol size: 32", test_spi_transfer<uint32_t, 8, false>),
    Case("SPI master-slave async com - mode: idle low, sample second edge", test_spi_transfer<uint8_t, 9, false>),
    Case("SPI master-slave async com - mode: idle high, sample first edge", test_spi_transfer<uint8_t, 10, false>),
    Case("SPI master-slave async com - mode: idle high, sample second edge", test_spi_transfer<uint8_t, 11, false>),
    Case("SPI master-slave async com - bit ordering: LSB first", test_spi_transfer<uint8_t, 12, false>),
    Case("SPI master-slave async com - freq testing: 200 KHz", test_spi_transfer<uint8_t, 13, false>),
    Case("SPI master-slave async com - freq testing: 2 MHz", test_spi_transfer<uint8_t, 14, false>),
    Case("SPI master-slave async com - master: manual ss", test_spi_transfer<uint8_t, 25, false>),
    Case("SPI master-slave async com - one symbol", test_spi_transfer<uint8_t, 26, false>),
    Case("SPI master-slave async com - long buffer", test_spi_transfer<uint8_t, 27, false>),
#endif
};

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}
