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
#ifndef DEVICE_SPI
#error [NOT_SUPPORTED] SPI is not supported on this platform, add 'DEVICE_SPI' definition to your platform.
#endif

#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "spi_api.h"
#include "pinmap.h"

using namespace utest::v1;

#define CAPABILITY_WORD_LENGTH_8 (1<<7)

#define FREQ_200KHZ (200000)
#define FREQ_1MHZ   (1000000)
#define FREQ_2MHZ   (2000000)

#define SPI_MASTER_MOSI      SPI_TEST_MASTER_PIN(MOSI)
#define SPI_MASTER_MISO      SPI_TEST_MASTER_PIN(MISO)
#define SPI_MASTER_SCK       SPI_TEST_MASTER_PIN(SCK)
#define SPI_MASTER_CS        SPI_TEST_MASTER_PIN(CS)
#define SPI_SLAVE_MOSI       SPI_TEST_SLAVE_PIN(MOSI)
#define SPI_SLAVE_MISO       SPI_TEST_SLAVE_PIN(MISO)
#define SPI_SLAVE_SCK        SPI_TEST_SLAVE_PIN(SCK)
#define SPI_SLAVE_CS         SPI_TEST_SLAVE_PIN(CS)

#define SYM_CNT 100

static uint8_t tx_buff_uint8[SYM_CNT];
static uint8_t rx_buff_uint8[SYM_CNT];
static uint16_t tx_buff_uint16[SYM_CNT];
static uint16_t rx_buff_uint16[SYM_CNT];
static uint32_t tx_buff_uint32[SYM_CNT];
static uint32_t rx_buff_uint32[SYM_CNT];

static const uint8_t fill_symbol_8 = 0x55;
static const uint16_t fill_symbol_16 = 0x5555;
static const uint32_t fill_symbol_32 = 0x55555555;

/* There is no tolerance defined how much requested frequency can differ from the actual frequency.
 * We will use requested frequency as tolerance for now.
 */
#define FREQ_TOLERANCE(requested_freq) (requested_freq)

/* SPI format/frequency configuration. */
typedef struct {
    uint8_t symbol_size;
    spi_mode_t mode;
    spi_bit_ordering_t bit_ordering;
    uint32_t freq_hz;
} config_test_case_t;

/* Array witch test cases which represents different SPI configs for testing. */
static config_test_case_t test_cases[] = {
    /* default config */
    {8, SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ},
    /* symbol size testing */
    {0, SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ},
    {1, SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ},
    {7, SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ},
    {9, SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ},
    {15, SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ},
    {16, SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ},
    {17, SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ},
    {31, SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ},
    {32, SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ},
    /* mode testing */
    {8, SPI_MODE_IDLE_LOW_SAMPLE_SECOND_EDGE, SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ},
    {8, SPI_MODE_IDLE_HIGH_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ},
    {8, SPI_MODE_IDLE_HIGH_SAMPLE_SECOND_EDGE, SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ},
    /* bit ordering testing */
    {8, SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_LSB_FIRST, FREQ_200KHZ},
    /* freq testing */
    {8, SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_LSB_FIRST, FREQ_1MHZ},
    {8, SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_LSB_FIRST, FREQ_2MHZ},
};

#ifdef DEVICE_SPI_ASYNCH

static spi_t *expected_spi_obj;
static uint32_t some_ctx;
static spi_async_event_t expected_event;
static volatile bool handler_called;

static void async_handler(spi_t *obj, void *ctx, spi_async_event_t *event)
{
    TEST_ASSERT_EQUAL(expected_spi_obj, obj);
    TEST_ASSERT_EQUAL((void *)&some_ctx, ctx);
    TEST_ASSERT_EQUAL(expected_event.transfered, event->transfered);

    handler_called = true;
}

#endif

/* Test that spi_get_module() returns the SPIName - unique identifier to the peripheral associated
 * to this SPI channel. */
void test_get_module()
{
    SPIName spi_name;

    spi_name = spi_get_module(SPI_MASTER_MOSI, SPI_MASTER_MISO, SPI_MASTER_SCK);

    TEST_ASSERT_EQUAL(SPI_TEST_MASTER, spi_name);

#ifdef SPISLAVE
    spi_name = spi_get_module(SPI_SLAVE_MOSI, SPI_SLAVE_MISO, SPI_SLAVE_SCK);

    TEST_ASSERT_EQUAL(SPI_TEST_SLAVE, spi_name);
#endif
}

/* Test that spi_get_capabilities() fills the given spi_capabilities_t instance with capabilities
 * of the specified SPI peripheral. */
void test_get_capabilities()
{
    spi_capabilities_t capabilities;

    /* Slave capabielities. */
    spi_get_capabilities(spi_get_module(SPI_MASTER_MOSI, SPI_MASTER_MISO, SPI_MASTER_SCK), NC, &capabilities);

    /* The supported frequency range must include the range [0.2..2] MHz according to defined behaviour.
     * At least a symbol width of 8 bit must be supported.
     */
    TEST_ASSERT(capabilities.minimum_frequency <= FREQ_200KHZ);
    TEST_ASSERT(capabilities.maximum_frequency >= FREQ_2MHZ);
    TEST_ASSERT_TRUE(capabilities.word_length & CAPABILITY_WORD_LENGTH_8);

    /* SS pin not passed to spi_get_capabilities. */
    spi_get_capabilities(spi_get_module(SPI_MASTER_MOSI, SPI_MASTER_MISO, SPI_MASTER_SCK), SPI_MASTER_SCK, &capabilities);

    TEST_ASSERT(capabilities.minimum_frequency <= FREQ_200KHZ);
    TEST_ASSERT(capabilities.maximum_frequency >= FREQ_2MHZ);
    TEST_ASSERT_TRUE(capabilities.word_length & CAPABILITY_WORD_LENGTH_8);

}

/* Test that spi_init() successfully initializes the pins and spi_free() can successfully
 * reset the pins to their default state. */
void test_init_free()
{
    spi_t spi_obj = { 0 };
    spi_capabilities_t capabilities = { 0 };

#ifdef SPISLAVE
    spi_get_capabilities(spi_get_module(SPI_SLAVE_MOSI, SPI_SLAVE_MISO, SPI_SLAVE_SCK), SPI_SLAVE_CS, &capabilities);
#endif

    /* SPI master - CS pin ignored. */
    /* Full duplex. */
    spi_init(&spi_obj, false, SPI_MASTER_MOSI, SPI_MASTER_MISO, SPI_MASTER_SCK, NC);

    spi_free(&spi_obj);

    /* Half duplex: MASTER <--- SLAVE. */
    spi_init(&spi_obj, false, NC, SPI_MASTER_MISO, SPI_MASTER_SCK, NC);

    spi_free(&spi_obj);

    /* Half duplex: MASTER ---> SLAVE. */
    spi_init(&spi_obj, false, SPI_MASTER_MOSI, NC, SPI_MASTER_SCK, NC);

    spi_free(&spi_obj);

    /* SPI master - SS pin defined. */
    /* Full duplex. */
    spi_init(&spi_obj, false, SPI_MASTER_MOSI, SPI_MASTER_MISO, SPI_MASTER_SCK, SPI_MASTER_CS);

    spi_free(&spi_obj);

    /* Half duplex: MASTER <--- SLAVE. */
    spi_init(&spi_obj, false, NC, SPI_MASTER_MISO, SPI_MASTER_SCK, SPI_MASTER_CS);

    spi_free(&spi_obj);

    /* Half duplex: MASTER ---> SLAVE. */
    spi_init(&spi_obj, false, SPI_MASTER_MOSI, NC, SPI_MASTER_SCK, SPI_MASTER_CS);

    spi_free(&spi_obj);

#ifdef SPISLAVE
    if (capabilities.support_slave_mode) {
        /* SPI slave - CS pin must be defined. */
        /* Full duplex. */
        spi_init(&spi_obj, true, SPI_SLAVE_MOSI, SPI_SLAVE_MISO, SPI_SLAVE_SCK, SPI_SLAVE_CS);

        spi_free(&spi_obj);

        /* Half duplex: MASTER <--- SLAVE. */
        spi_init(&spi_obj, true, NC, SPI_SLAVE_MISO, SPI_SLAVE_SCK, SPI_SLAVE_CS);

        spi_free(&spi_obj);

        /* Half duplex: MASTER ---> SLAVE. */
        spi_init(&spi_obj, true, SPI_SLAVE_MOSI, NC, SPI_SLAVE_SCK, SPI_SLAVE_CS);

        spi_free(&spi_obj);
    }
#endif

    /* Indicate that test case has been successfully executed. */
    TEST_ASSERT_TRUE(true);
}

/* Test that spi_format() sets/updates the transmission format of the SPI peripheral. */
void test_set_format()
{
    spi_t spi_obj = { 0 };
    spi_capabilities_t capabilities = { 0 };

#ifdef SPISLAVE
    spi_get_capabilities(spi_get_module(SPI_SLAVE_MOSI, SPI_SLAVE_MISO, SPI_SLAVE_SCK), SPI_SLAVE_CS, &capabilities);
#endif

    /* SPI master. */
    spi_init(&spi_obj, false, SPI_MASTER_MOSI, SPI_MASTER_MISO, SPI_MASTER_SCK, NC);

    /* 8 bit word length is obligate, all possible combinations of SPI mode and bit ordering are obligate. */
    spi_format(&spi_obj, 8, SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_MSB_FIRST);
    spi_format(&spi_obj, 8, SPI_MODE_IDLE_LOW_SAMPLE_SECOND_EDGE, SPI_BIT_ORDERING_MSB_FIRST);
    spi_format(&spi_obj, 8, SPI_MODE_IDLE_HIGH_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_MSB_FIRST);
    spi_format(&spi_obj, 8, SPI_MODE_IDLE_HIGH_SAMPLE_SECOND_EDGE, SPI_BIT_ORDERING_MSB_FIRST);
    spi_format(&spi_obj, 8, SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_LSB_FIRST);
    spi_format(&spi_obj, 8, SPI_MODE_IDLE_LOW_SAMPLE_SECOND_EDGE, SPI_BIT_ORDERING_LSB_FIRST);
    spi_format(&spi_obj, 8, SPI_MODE_IDLE_HIGH_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_LSB_FIRST);
    spi_format(&spi_obj, 8, SPI_MODE_IDLE_HIGH_SAMPLE_SECOND_EDGE, SPI_BIT_ORDERING_LSB_FIRST);

    spi_free(&spi_obj);

#ifdef SPISLAVE
    if (capabilities.support_slave_mode) {

        /* SPI slave. */
        spi_init(&spi_obj, true, SPI_SLAVE_MOSI, SPI_SLAVE_MISO, SPI_SLAVE_SCK, SPI_SLAVE_CS);

        /* 8 bit word length is obligate, all possible combinations of SPI mode and bit ordering are obligate. */
        spi_format(&spi_obj, 8, SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_MSB_FIRST);
        spi_format(&spi_obj, 8, SPI_MODE_IDLE_LOW_SAMPLE_SECOND_EDGE, SPI_BIT_ORDERING_MSB_FIRST);
        spi_format(&spi_obj, 8, SPI_MODE_IDLE_HIGH_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_MSB_FIRST);
        spi_format(&spi_obj, 8, SPI_MODE_IDLE_HIGH_SAMPLE_SECOND_EDGE, SPI_BIT_ORDERING_MSB_FIRST);
        spi_format(&spi_obj, 8, SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_LSB_FIRST);
        spi_format(&spi_obj, 8, SPI_MODE_IDLE_LOW_SAMPLE_SECOND_EDGE, SPI_BIT_ORDERING_LSB_FIRST);
        spi_format(&spi_obj, 8, SPI_MODE_IDLE_HIGH_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_LSB_FIRST);
        spi_format(&spi_obj, 8, SPI_MODE_IDLE_HIGH_SAMPLE_SECOND_EDGE, SPI_BIT_ORDERING_LSB_FIRST);

        spi_free(&spi_obj);
    }
#endif

    /* Indicate that test case has been successfully executed. */
    TEST_ASSERT_TRUE(true);
}

/* Test that test_set_frequency() sets the frequency used during the SPI transfer. */
void test_set_frequency()
{
    spi_t spi_obj = { 0 };
    uint32_t real_freq;

    /* SPI master. */
    spi_init(&spi_obj, false, SPI_MASTER_MOSI, SPI_MASTER_MISO, SPI_MASTER_SCK, NC);

    spi_format(&spi_obj, 8, SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_MSB_FIRST);

    real_freq = spi_frequency(&spi_obj, FREQ_200KHZ);

    TEST_ASSERT_UINT32_WITHIN(FREQ_TOLERANCE(FREQ_200KHZ), FREQ_200KHZ, real_freq);

    real_freq = spi_frequency(&spi_obj, FREQ_1MHZ);

    TEST_ASSERT_UINT32_WITHIN(FREQ_TOLERANCE(FREQ_1MHZ), FREQ_1MHZ, real_freq);

    real_freq = spi_frequency(&spi_obj, FREQ_2MHZ);

    TEST_ASSERT_UINT32_WITHIN(FREQ_TOLERANCE(FREQ_2MHZ), FREQ_2MHZ, real_freq);

    spi_free(&spi_obj);
}

/* Test that spi_transfer() can successfully perform transfer in master mode
 *  (TX/RX buffers are defined and have the same sizes) and returns the number of
 *  symbols clocked on the bus during this transfer. */
void test_transfer_master()
{
    spi_t spi_obj = { 0 };
    spi_capabilities_t capabilities = { 0 };

    void *p_tx_buf;
    void *p_rx_buf;
    void *p_fill_sym;

    spi_get_capabilities(spi_get_module(SPI_MASTER_MOSI, SPI_MASTER_MISO, SPI_MASTER_SCK), NC, &capabilities);

    /* SPI master. */
    spi_init(&spi_obj, false, SPI_MASTER_MOSI, SPI_MASTER_MISO, SPI_MASTER_SCK, NC);

    for (uint32_t tc_id = 0; tc_id < (sizeof(test_cases) / sizeof(config_test_case_t)); tc_id++) {

        if (capabilities.word_length & (1 << (test_cases[tc_id].symbol_size - 1))) {

            spi_format(&spi_obj, test_cases[tc_id].symbol_size, test_cases[tc_id].mode,
                       test_cases[tc_id].bit_ordering);
            spi_frequency(&spi_obj, test_cases[tc_id].freq_hz);

            if (test_cases[tc_id].symbol_size < 8) {
                p_tx_buf = tx_buff_uint8;
                p_rx_buf = rx_buff_uint8;
                p_fill_sym = (void *) &fill_symbol_8;
            } else if (test_cases[tc_id].symbol_size < 16) {
                p_tx_buf = tx_buff_uint16;
                p_rx_buf = rx_buff_uint16;
                p_fill_sym = (void *) &fill_symbol_16;
            } else {
                p_tx_buf = tx_buff_uint32;
                p_rx_buf = rx_buff_uint32;
                p_fill_sym = (void *) &fill_symbol_32;
            }

            const uint32_t sym_cnt = spi_transfer(&spi_obj, p_tx_buf, SYM_CNT, p_rx_buf, SYM_CNT, p_fill_sym);

            TEST_ASSERT_EQUAL(SYM_CNT, sym_cnt);
        }
    }

    spi_free(&spi_obj);
}

/* Test that spi_transfer() can successfully perform transfer in master mode
 * (TX/RX buffers are undefined or have different sizes) and returns the number of
 * symbols clocked on the bus during this transfer. */
void test_transfer_master_fill_sym()
{
    spi_t spi_obj = { 0 };
    spi_capabilities_t capabilities = { 0 };
    uint32_t sym_cnt;

    uint8_t tx_buf[SYM_CNT] = {0};
    uint8_t rx_buf[SYM_CNT] = {0};
    uint8_t fill_sym = 0;

    spi_get_capabilities(spi_get_module(SPI_MASTER_MOSI, SPI_MASTER_MISO, SPI_MASTER_SCK), NC, &capabilities);

    /* SPI master. */
    spi_init(&spi_obj, false, SPI_MASTER_MOSI, SPI_MASTER_MISO, SPI_MASTER_SCK, NC);

    spi_format(&spi_obj, 8, SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_MSB_FIRST);
    spi_frequency(&spi_obj, FREQ_1MHZ);

    sym_cnt = spi_transfer(&spi_obj, tx_buf, SYM_CNT / 2, rx_buf, SYM_CNT, &fill_sym);
    TEST_ASSERT_EQUAL(SYM_CNT, sym_cnt);

    sym_cnt = spi_transfer(&spi_obj, tx_buf, SYM_CNT, rx_buf, SYM_CNT / 2, &fill_sym);
    TEST_ASSERT_EQUAL(SYM_CNT, sym_cnt);

    sym_cnt = spi_transfer(&spi_obj, NULL, SYM_CNT, rx_buf, SYM_CNT, &fill_sym);
    TEST_ASSERT_EQUAL(SYM_CNT, sym_cnt);

    sym_cnt = spi_transfer(&spi_obj, tx_buf, SYM_CNT, NULL, SYM_CNT, &fill_sym);
    TEST_ASSERT_EQUAL(SYM_CNT, sym_cnt);

    spi_free(&spi_obj);

}

#ifdef DEVICE_SPI_ASYNCH
/* Test that spi_transfer_async() can successfully perform asynchronous transfer
 * in master mode. */
void test_transfer_master_async()
{
    spi_t spi_obj = {0};
    spi_capabilities_t capabilities = {0};

    const DMAUsage dma_modes[] = {
        DMA_USAGE_NEVER,
        DMA_USAGE_OPPORTUNISTIC,
        DMA_USAGE_ALWAYS,
        DMA_USAGE_TEMPORARY_ALLOCATED,
        DMA_USAGE_ALLOCATED
    };

    void *p_tx_buf;
    void *p_rx_buf;
    void *p_fill_sym;

    spi_get_capabilities(spi_get_module(SPI_MASTER_MOSI, SPI_MASTER_MISO, SPI_MASTER_SCK), NC, &capabilities);

    /* SPI master. */
    spi_init(&spi_obj, false, SPI_MASTER_MOSI, SPI_MASTER_MISO, SPI_MASTER_SCK, NC);

    for (uint32_t tc_id = 0; tc_id < (sizeof(test_cases) / sizeof(config_test_case_t)); tc_id++) {

        if (capabilities.word_length & (1 << (test_cases[tc_id].symbol_size - 1))) {

            spi_format(&spi_obj, test_cases[tc_id].symbol_size, test_cases[tc_id].mode, test_cases[tc_id].bit_ordering);
            spi_frequency(&spi_obj, test_cases[tc_id].freq_hz);

            if (test_cases[tc_id].symbol_size < 8) {
                p_tx_buf = tx_buff_uint8;
                p_rx_buf = rx_buff_uint8;
                p_fill_sym = (void *)&fill_symbol_8;
            } else if (test_cases[tc_id].symbol_size < 16) {
                p_tx_buf = tx_buff_uint16;
                p_rx_buf = rx_buff_uint16;
                p_fill_sym = (void *)&fill_symbol_16;
            } else {
                p_tx_buf = tx_buff_uint32;
                p_rx_buf = rx_buff_uint32;
                p_fill_sym = (void *)&fill_symbol_32;
            }

            for (uint32_t mode_id = 0; mode_id < (sizeof(dma_modes) / sizeof(DMAUsage)); mode_id++) {
                expected_spi_obj = &spi_obj;
                handler_called = false;
                expected_event.transfered = SYM_CNT;
                const bool async_status = spi_transfer_async(&spi_obj, p_tx_buf, SYM_CNT, p_rx_buf, SYM_CNT, p_fill_sym, async_handler, (void *)&some_ctx, dma_modes[mode_id]);

                TEST_ASSERT_EQUAL(true, async_status);

                while (!handler_called) {
                    /* Wait until the end of the spi transmission. */
                }

                TEST_ASSERT_EQUAL(true, handler_called);
            }
        }

    }
    spi_free(&spi_obj);
}

/* Test that spi_transfer_async_abort() can successfully abort an on-going async transfer. */
void test_transfer_master_async_abort()
{
    spi_t spi_obj = {0};
    spi_capabilities_t capabilities = {0};

    const DMAUsage dma_modes[] = {
        DMA_USAGE_NEVER,
        DMA_USAGE_OPPORTUNISTIC,
        DMA_USAGE_ALWAYS,
        DMA_USAGE_TEMPORARY_ALLOCATED,
        DMA_USAGE_ALLOCATED
    };

    void *p_tx_buf;
    void *p_rx_buf;
    void *p_fill_sym;

    spi_get_capabilities(spi_get_module(SPI_MASTER_MOSI, SPI_MASTER_MISO, SPI_MASTER_SCK), NC, &capabilities);

    /* SPI master. */
    spi_init(&spi_obj, false, SPI_MASTER_MOSI, SPI_MASTER_MISO, SPI_MASTER_SCK, NC);

    /* Skip last two configs (high frequency) since we may not be able to abort before the transmission ends. */
    for (uint32_t tc_id = 0; tc_id < ((sizeof(test_cases) / sizeof(config_test_case_t)) - 2); tc_id++) {

        if (capabilities.word_length & (1 << (test_cases[tc_id].symbol_size - 1))) {

            spi_format(&spi_obj, test_cases[tc_id].symbol_size, test_cases[tc_id].mode, test_cases[tc_id].bit_ordering);
            spi_frequency(&spi_obj, test_cases[tc_id].freq_hz);

            if (test_cases[tc_id].symbol_size < 8) {
                p_tx_buf = tx_buff_uint8;
                p_rx_buf = rx_buff_uint8;
                p_fill_sym = (void *)&fill_symbol_8;
            } else if (test_cases[tc_id].symbol_size < 16) {
                p_tx_buf = tx_buff_uint16;
                p_rx_buf = rx_buff_uint16;
                p_fill_sym = (void *)&fill_symbol_16;
            } else {
                p_tx_buf = tx_buff_uint32;
                p_rx_buf = rx_buff_uint32;
                p_fill_sym = (void *)&fill_symbol_32;
            }

            for (uint32_t mode_id = 0; mode_id < (sizeof(dma_modes) / sizeof(DMAUsage)); mode_id++) {
                expected_spi_obj = &spi_obj;
                expected_event.transfered = 0; // Abort will have to be indicated here
                handler_called = false;

                /* Ensure that transfer is aborted  is immediately after it is triggered. */
                core_util_critical_section_enter();

                const bool async_status = spi_transfer_async(&spi_obj, p_tx_buf, SYM_CNT, p_rx_buf, SYM_CNT, p_fill_sym, async_handler, (void *)&some_ctx, dma_modes[mode_id]);

                TEST_ASSERT_EQUAL(true, async_status);

                /* Transfer of 100 bytes @ 1 MHz should take about 100 us.
                 * This should be enough time to call abort during the transfer.
                 */
                spi_transfer_async_abort(&spi_obj);

                core_util_critical_section_exit();

                /* Wait a while before checking the flag which indicates if the async transfer has been completed. */
                wait_ms(2);

                /* Handler is not called in case of abort. */
                TEST_ASSERT_EQUAL(false, handler_called);
            }
        }
    }
    spi_free(&spi_obj);
}
#endif

utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(40, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason)
{
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_CONTINUE;
}

utest::v1::status_t greentea_failure_handler_abort(const Case *const source, const failure_t reason)
{
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_ABORT;
}

Case cases[] = {
    Case("SPI - get module name test", test_get_module, greentea_failure_handler),
    Case("SPI - get capabilities test", test_get_capabilities, greentea_failure_handler),
    Case("SPI - init, free test", test_init_free, greentea_failure_handler),
    Case("SPI - set format test", test_set_format, greentea_failure_handler),
    Case("SPI - set frequency test", test_set_frequency, greentea_failure_handler),
    Case("SPI - master transfer test", test_transfer_master, greentea_failure_handler),
    Case("SPI - fill symbol test", test_transfer_master_fill_sym, greentea_failure_handler),
#ifdef DEVICE_SPI_ASYNCH
    Case("SPI - master transfer async test", test_transfer_master_async, greentea_failure_handler),
    Case("SPI - master transfer async abort test", test_transfer_master_async_abort, greentea_failure_handler),
#endif
};

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}
