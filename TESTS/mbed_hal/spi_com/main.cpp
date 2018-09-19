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
#if !DEVICE_SPI
    #error [NOT_SUPPORTED] SPI is not supported on this platform, add 'DEVICE_SPI' definition to your platform.
#endif

#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "spi_api.h"
#include "pinmap.h"

using namespace utest::v1;

/* 1. Overview
 *
 * This is the SPI communication test which verifies various SPI configurations.
 * This test transfers data between SPI master and SPI slave.
 * Two boards are required with SPI peripheral available and slave/master mode support.
 *
 * Master part of the test is a standard green-tea test. Which can be run using the
 * following command:
 *   mbed test -t <toolchain> -m <target> -n tests-mbed_hal-spi_com
 * BUILD_MASTER flag must be set to 1.
 *
 * Slave part of the test can be build using the following command:
 *   mbed test -t <toolchain> -m <target> -n tests-mbed_hal-spi_com --compile
 * BUILD_MASTER flag must be set to 0 and target must be flushed manually.
 *
 * 2. What is verified by the test?
 *    a) master/slave mode,
 *    b) synchronous/asynchronous modes,
 *    c) full duplex/half duplex modes,
 *    d) symbol sizes: [1, 32],
 *    e) clock polarity/phase,
 *    f) bit ordering during transmission,
 *    g) clock frequency,
 *    h) rx count == tx coun, trx count != tx count,
 *    i) undefined rx/tx buffer,
 *    j) internal/external ss handling by master.
 *
 * 4. Test scenario
 *    Test assumes that default configuration:
 *     - 8 bit symbol,
 *     - sync mode,
 *     - full duplex,
 *     - clock idle low,
 *     - sample on the first clock edge,
 *     - MSB transmitted first,
 *     - 200 KHz clock clock,
 *     - internal SS handling,
 *    is supported by all SPI devices.
 *
 *    Test steps:
 *    a) Configuration is validated against the capabilities on master side.
 *    b) If master can not handle this configuration - test case is skipped.
 *    c) Master sends the configuration to slave using default configuration.
 *    d) Configuration is validated against the capabilities on slave side.
 *    e) If slave can not handle this configuration it sends CONFIG_STATUS_NOT_SUPPORTED
 *       status to master and test case is skipped. Otherwise slave sends
 *       CONFIG_STATUS_OK to the master.
 *    f) When both sides can handle the specified configuration, then slave and master
 *       loads the configuration and perform communication test.
 *
 * 3. Steps to run the test
 *
 *    a) connect MASTER and SLAVE as follows:
 *       SPI_MASTER_MOSI  -------------------  SPI_SLAVE_MOSI
 *       SPI_MASTER_MISO  -------------------  SPI_SLAVE_MISO
 *       SPI_MASTER_SS    -------------------  SPI_SLAVE_SS
 *       SPI_MASTER_CLK   -------------------  SPI_SLAVE_CLK
 *    b) configure MASTER/SLAVE pins.
 *    c) set BUILD_MASTER flag to 0.
 *    d) build slave part of the test:
 *       mbed test -t <toolchain> -m <target> -n tests-mbed_hal-spi_comm --compile
 *    e) flush the target with the slave part binary.
 *    f) set BUILD_MASTER flag to 1.
 *    g) run the master part of the test:
 *       mbed test -t <toolchain> -m <target> -n tests-mbed_hal-spi_comm
 *    f) reset the slave target.
 *
 * */

/***** test configuration *****/
#define BUILD_MASTER 1
#define BUILD_SLAVE 2
#define BUILD_NONE 0

/* Select build type (BUILD_MASTER, BUILD_SLAVE, BUILD_NONE). */
#define BUILD_TYPE BUILD_MASTER

#if (BUILD_TYPE == BUILD_NONE)
    #error [NOT_SUPPORTED] This test can not be run on CI. It is designed for manual testing.
#endif

/* Only MASTER can initialise the transfer, so SLAVE needs to be ready
 * when MASTER starts clocking.
 * Specify the delay on master side before each transfer (100 ms default).
 * If this is not enough for your case please adapt the delay.
 * Alternatively you can specify the delay 0. In this case test will hang before
 * each transmission until the specified button is pressed.
 */
#define TRANSMISSION_DELAY_MS 100
#define TRANSMISSION_BUTTON SW3

/* Pins configuration. */
#if (BUILD_TYPE == BUILD_MASTER)
#define SPI_MASTER_MOSI      PTD2
#define SPI_MASTER_MISO      PTD3
#define SPI_MASTER_SS        PTD0
#define SPI_MASTER_CLK       PTD1
#elif (BUILD_TYPE == BUILD_SLAVE)
#define SPI_SLAVE_MOSI       PE_6
#define SPI_SLAVE_MISO       PE_5
#define SPI_SLAVE_SS         PE_4
#define SPI_SLAVE_CLK        PE_2
#endif

/* Set to enable debug prints.
 * Note: Use terminal to see slave logs.
 *       Debug may request to increase delays before transmission is triggered on master side. */
#define DEBUG 0
/******************************/

#define FREQ_200KHZ (200000)
#define FREQ_1MHZ   (1000000)
#define FREQ_2MHZ   (2000000)
#define FREQ_MIN    (0)
#define FREQ_MAX    (0xFFFFFFFF)

#define DEFAULT_CFG 0
#define CONFIG_STATUS_OK 0x00
#define CONFIG_STATUS_NOT_SUPPORTED 0x01

#define TEST_SYM_CNT 5

typedef enum
{
    FULL_DUPLEX, HALF_DUPLEX_MOSI, HALF_DUPLEX_MISO
} duplex_t;

/* SPI test configuration. */
typedef struct
{
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

#define CONFIG_LEN (sizeof(config_test_case_t))
#define CONFIG_STATUS_LEN 1

/* Function inits specified buffer with given pattern. */
static void set_buffer(void * addr, uint32_t size, char val)
{
    if (addr == NULL) {
        return;
    }

    char *p_char = (char*) addr;

    for (uint32_t i = 0; i < size; i++) {
        p_char[i] = val;
    }
}

/* Function returns true if configuration is consistent with the capabilities of
 * the SPI peripheral, false otherwise. */
static bool check_capabilities(spi_capabilities_t *p_cabs, uint32_t symbol_size, bool slave, bool half_duplex)
{
    if (!(p_cabs->word_length & (1 << (symbol_size - 1))) ||
        (slave && !p_cabs->support_slave_mode) ||
        (half_duplex && !p_cabs->half_duplex)) {
        return false;
    }

    return true;
}

#ifdef DEVICE_SPI_ASYNCH
/* Callback function for SPI async transfers. */
static volatile uint32_t context;
static spi_t *expected_obj;
void spi_async_callback(spi_t *obj, void *ctx, spi_async_event_t *event) {
    TEST_ASSERT_EQUAL(expected_obj, obj);
    *((uint32_t*)ctx) = event->transfered;
}
#endif

/* Function used to perform transmission using sync or async modes. */
static uint32_t sync_async_transfer(spi_t *obj, const void *tx, uint32_t tx_len, void *rx, uint32_t rx_len, const void *fill_symbol, bool sync_mode)
{
    uint32_t count = 0;

    if (sync_mode) {
        count = spi_transfer(obj, tx, tx_len, rx, rx_len, fill_symbol);
    }
#ifdef DEVICE_SPI_ASYNCH
    else {
        context = 0;
        expected_obj = obj;
        bool ret = spi_transfer_async(obj, tx, tx_len, rx, rx_len, fill_symbol, spi_async_callback, &context);
        TEST_ASSERT_EQUAL(true, ret);
        /* Wait here for the end of transmission. Callback will set context to the number of
         * transfered symbols. */
        while(!context);

        count = context;
    }
#endif

    return count;
}

#if (BUILD_TYPE == BUILD_MASTER)
/* Array witch test cases which represents different SPI configurations for testing. */
static config_test_case_t test_cases[] = {
        /* default config: 8 bit symbol\sync mode\full duplex\clock idle low\sample on the first clock edge\MSB first\100 KHz clock\automatic SS handling */
/* 00 */{8, SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE    , SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ  , TEST_SYM_CNT   , TEST_SYM_CNT  , TEST_SYM_CNT  , TEST_SYM_CNT  , true , true , true , true , true , FULL_DUPLEX  },
        /* symbol size testing */
/* 01 */{1  , SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE  , SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ  , TEST_SYM_CNT   , TEST_SYM_CNT  , TEST_SYM_CNT  , TEST_SYM_CNT  , true , true , true , true , true , FULL_DUPLEX  },
/* 02 */{7  , SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE  , SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ  , TEST_SYM_CNT   , TEST_SYM_CNT  , TEST_SYM_CNT  , TEST_SYM_CNT  , true , true , true , true , true , FULL_DUPLEX  },
/* 03 */{9  , SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE  , SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ  , TEST_SYM_CNT   , TEST_SYM_CNT  , TEST_SYM_CNT  , TEST_SYM_CNT  , true , true , true , true , true , FULL_DUPLEX  },
/* 04 */{15 , SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE  , SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ  , TEST_SYM_CNT   , TEST_SYM_CNT  , TEST_SYM_CNT  , TEST_SYM_CNT  , true , true , true , true , true , FULL_DUPLEX  },
/* 05 */{16 , SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE  , SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ  , TEST_SYM_CNT   , TEST_SYM_CNT  , TEST_SYM_CNT  , TEST_SYM_CNT  , true , true , true , true , true , FULL_DUPLEX  },
/* 06 */{17 , SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE  , SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ  , TEST_SYM_CNT   , TEST_SYM_CNT  , TEST_SYM_CNT  , TEST_SYM_CNT  , true , true , true , true , true , FULL_DUPLEX  },
/* 07 */{31 , SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE  , SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ  , TEST_SYM_CNT   , TEST_SYM_CNT  , TEST_SYM_CNT  , TEST_SYM_CNT  , true , true , true , true , true , FULL_DUPLEX  },
/* 08 */{32 , SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE  , SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ  , TEST_SYM_CNT   , TEST_SYM_CNT  , TEST_SYM_CNT  , TEST_SYM_CNT  , true , true , true , true , true , FULL_DUPLEX  },
        /* mode testing */
/* 09 */{8  , SPI_MODE_IDLE_LOW_SAMPLE_SECOND_EDGE , SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ  , TEST_SYM_CNT   , TEST_SYM_CNT  , TEST_SYM_CNT  , TEST_SYM_CNT  , true , true , true , true , true , FULL_DUPLEX  },
/* 10 */{8  , SPI_MODE_IDLE_HIGH_SAMPLE_FIRST_EDGE , SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ  , TEST_SYM_CNT   , TEST_SYM_CNT  , TEST_SYM_CNT  , TEST_SYM_CNT  , true , true , true , true , true , FULL_DUPLEX  },
/* 11 */{8  , SPI_MODE_IDLE_HIGH_SAMPLE_SECOND_EDGE, SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ  , TEST_SYM_CNT   , TEST_SYM_CNT  , TEST_SYM_CNT  , TEST_SYM_CNT  , true , true , true , true , true , FULL_DUPLEX  },
        /* bit ordering testing */
/* 12 */{8  , SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE  , SPI_BIT_ORDERING_LSB_FIRST, FREQ_200KHZ  , TEST_SYM_CNT   , TEST_SYM_CNT  , TEST_SYM_CNT  , TEST_SYM_CNT  , true , true , true , true , true , FULL_DUPLEX  },
        /* freq testing */
/* 13 */{8  , SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE  , SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ, TEST_SYM_CNT   , TEST_SYM_CNT  , TEST_SYM_CNT  , TEST_SYM_CNT  , true , true , true , true , true , FULL_DUPLEX  },
/* 14 */{8  , SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE  , SPI_BIT_ORDERING_MSB_FIRST, FREQ_2MHZ  , TEST_SYM_CNT   , TEST_SYM_CNT  , TEST_SYM_CNT  , TEST_SYM_CNT  , true , true , true , true , true , FULL_DUPLEX  },
/* 15 */{8  , SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE  , SPI_BIT_ORDERING_MSB_FIRST, FREQ_MIN   , TEST_SYM_CNT   , TEST_SYM_CNT  , TEST_SYM_CNT  , TEST_SYM_CNT  , true , true , true , true , true , FULL_DUPLEX  },
/* 16 */{8  , SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE  , SPI_BIT_ORDERING_MSB_FIRST, FREQ_MAX   , TEST_SYM_CNT   , TEST_SYM_CNT  , TEST_SYM_CNT  , TEST_SYM_CNT  , true , true , true , true , true , FULL_DUPLEX  },
        /* master: TX > RX */
/* 17 */{8  , SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE  , SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ  , TEST_SYM_CNT   , TEST_SYM_CNT-2, TEST_SYM_CNT  , TEST_SYM_CNT  , true , true , true , true , true , FULL_DUPLEX  },
        /* master: TX < RX */
/* 18 */{8  , SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE  , SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ  , TEST_SYM_CNT-2 , TEST_SYM_CNT  , TEST_SYM_CNT  , TEST_SYM_CNT  , true , true , true , true , true , FULL_DUPLEX  },
        /* slave: TX > RX */
/* 19 */{8  , SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE  , SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ  , TEST_SYM_CNT   , TEST_SYM_CNT  , TEST_SYM_CNT  , TEST_SYM_CNT-2, true , true , true , true , true , FULL_DUPLEX  },
        /* slave: TX < RX */
/* 20 */{8  , SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE  , SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ  , TEST_SYM_CNT   , TEST_SYM_CNT  , TEST_SYM_CNT-2, TEST_SYM_CNT  , true , true , true , true , true , FULL_DUPLEX  },
        /* master tx buffer undefined */
/* 21 */{8  , SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE  , SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ  , TEST_SYM_CNT   , TEST_SYM_CNT  , TEST_SYM_CNT  , TEST_SYM_CNT  , false, true , true , true , true , FULL_DUPLEX  },
        /* master rx buffer undefined */
/* 22 */{8  , SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE  , SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ  , TEST_SYM_CNT   , TEST_SYM_CNT  , TEST_SYM_CNT  , TEST_SYM_CNT  , true , false, true , true , true , FULL_DUPLEX  },
        /* slave tx buffer undefined */
/* 23 */{8  , SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE  , SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ  , TEST_SYM_CNT   , TEST_SYM_CNT  , TEST_SYM_CNT  , TEST_SYM_CNT  , true , true , false, true , true , FULL_DUPLEX  },
        /* slave rx buffer undefined */
/* 24 */{8  , SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE  , SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ  , TEST_SYM_CNT   , TEST_SYM_CNT  , TEST_SYM_CNT  , TEST_SYM_CNT  , true , true , true , false, true , FULL_DUPLEX  },
        /* manual ss hadling by master */
/* 25 */{8  , SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE  , SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ  , TEST_SYM_CNT   , TEST_SYM_CNT  , TEST_SYM_CNT  , TEST_SYM_CNT  , true , true , true , true , false, FULL_DUPLEX  },
        /* half duplex mode  */
/* 26 */{8  , SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE  , SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ  , TEST_SYM_CNT   , TEST_SYM_CNT  , TEST_SYM_CNT  , TEST_SYM_CNT  , true , true , true , true , true , HALF_DUPLEX_MISO },
/* 27 */{8  , SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE  , SPI_BIT_ORDERING_MSB_FIRST, FREQ_200KHZ  , TEST_SYM_CNT   , TEST_SYM_CNT  , TEST_SYM_CNT  , TEST_SYM_CNT  , true , true , true , true , true , HALF_DUPLEX_MOSI },
};

/* Function waits before the transmission is triggered on the master side. */
static void wait_before_transmission()
{
    if (TRANSMISSION_DELAY_MS) {
        wait_ms(TRANSMISSION_DELAY_MS);
    } else {
        DigitalIn button(TRANSMISSION_BUTTON);

        while (button.read() == 1);
        while (button.read() == 0);
    }
}

/* Function compares given buffers and returns true when equal, false otherwise.
 * In case when buffer is undefined (NULL) function returns true. */
static bool check_buffers(void *p_pattern, void *p_buffer, uint32_t size)
{
    const char * p_byte_pattern = (const char*) p_pattern;
    const char * p_byte_buffer = (const char*) p_buffer;

    if (p_buffer == NULL || p_pattern == NULL) {
        return true;
    }

    while (size) {
        if (*p_byte_pattern != *p_byte_buffer) {
            return false;
        }
        p_byte_pattern++;
        p_byte_buffer++;
        size--;
    }

    return true;
}

/* Function initialises RX, TX buffers before transmission. */
template<typename T>
static void init_transmission_buffers(uint32_t tc_id, T *p_tx_pattern, T *p_rx1_pattern, T *p_rx2_pattern, T *p_tx_buff, T *p_rx_buff, T *p_fill_symbol)
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
    if (test_cases[tc_id].master_tx_cnt > test_cases[tc_id].master_rx_cnt) {
        set_buffer(&p_rx1_pattern[3], sizeof(T), 0x00);
        set_buffer(&p_rx1_pattern[4], sizeof(T), 0x00);
        set_buffer(&p_rx2_pattern[3], sizeof(T), 0x00);
        set_buffer(&p_rx2_pattern[4], sizeof(T), 0x00);
    }
    /* Exception: master TX < master RX . */
    if (test_cases[tc_id].master_tx_cnt < test_cases[tc_id].master_rx_cnt) {
        set_buffer(&p_rx2_pattern[3], sizeof(T), 0xFF);
        set_buffer(&p_rx2_pattern[4], sizeof(T), 0xFF);
    }

    /* Exception: slave TX > slave RX . */
    if (test_cases[tc_id].slave_tx_cnt > test_cases[tc_id].slave_rx_cnt) {
        set_buffer(&p_rx2_pattern[3], sizeof(T), 0x00);
        set_buffer(&p_rx2_pattern[4], sizeof(T), 0x00);
    }
    /* Exception: slave TX < slave RX . */
    if (test_cases[tc_id].slave_tx_cnt < test_cases[tc_id].slave_rx_cnt) {
        set_buffer(&p_rx1_pattern[3], sizeof(T), 0xFF);
        set_buffer(&p_rx1_pattern[4], sizeof(T), 0xFF);
        set_buffer(&p_rx2_pattern[3], sizeof(T), 0xFF);
        set_buffer(&p_rx2_pattern[4], sizeof(T), 0xFF);
    }

    /* Exception: master TX buffer undefined . */
    if (!test_cases[tc_id].master_tx_defined) {
        set_buffer(&p_rx2_pattern[0], sizeof(T), 0xFF);
        set_buffer(&p_rx2_pattern[1], sizeof(T), 0xFF);
        set_buffer(&p_rx2_pattern[2], sizeof(T), 0xFF);
        set_buffer(&p_rx2_pattern[3], sizeof(T), 0xFF);
        set_buffer(&p_rx2_pattern[4], sizeof(T), 0xFF);
    }

    /* Exception: master RX buffer undefined . */
    if (!test_cases[tc_id].master_rx_defined) {
        set_buffer(&p_rx1_pattern[0], sizeof(T), 0xAA);
        set_buffer(&p_rx1_pattern[1], sizeof(T), 0xBB);
        set_buffer(&p_rx1_pattern[2], sizeof(T), 0xCC);
        set_buffer(&p_rx1_pattern[3], sizeof(T), 0xDD);
        set_buffer(&p_rx1_pattern[4], sizeof(T), 0xEE);
    }

    /* Exception: slave TX buffer undefined . */
    if (!test_cases[tc_id].slave_tx_defined) {
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
    if (!test_cases[tc_id].slave_rx_defined) {
        set_buffer(&p_rx2_pattern[0], sizeof(T), 0x11);
        set_buffer(&p_rx2_pattern[1], sizeof(T), 0x22);
        set_buffer(&p_rx2_pattern[2], sizeof(T), 0x33);
        set_buffer(&p_rx2_pattern[3], sizeof(T), 0x44);
        set_buffer(&p_rx2_pattern[4], sizeof(T), 0x55);
    }

    /* Handle symbol size. */
    T sym_mask = ((1 << test_cases[tc_id].symbol_size) - 1);

    for (uint32_t i = 0; i < TEST_SYM_CNT; i++) {
        p_tx_pattern[i] = (p_tx_pattern[i] & sym_mask);
        p_rx1_pattern[i] = (p_rx1_pattern[i] & sym_mask);
        p_rx2_pattern[i] = (p_rx2_pattern[i] & sym_mask);
    }

    memcpy(p_tx_buff, p_tx_pattern, sizeof(T) * TEST_SYM_CNT);
    set_buffer(p_rx_buff, sizeof(T) * TEST_SYM_CNT, 0x00);
}

/* Function handles <ss> line if <ss> is specified.
 * When manual <ss> handling is selected <ss> is defined.
 */
static void handle_ss(DigitalOut * ss, bool select)
{
    if (ss) {
        if (select) {
            *ss = 0;
        } else {
            *ss = 1;
        }
    }
}

template<typename T>
static void dump_buffers(T *tx_pattern, T *rx1_pattern, T *rx2_pattern, T *tx_buff, T *rx_buff)
{
#if DEBUG
    printf("Dump buffers: \n");
    printf("tx_pattern : 0x%X 0x%X 0x%X 0x%X 0x%X \n", tx_pattern[0], tx_pattern[1], tx_pattern[2], tx_pattern[3], tx_pattern[4]);
    printf("rx1_pattern: 0x%X 0x%X 0x%X 0x%X 0x%X \n", rx1_pattern[0], rx1_pattern[1], rx1_pattern[2], rx1_pattern[3], rx1_pattern[4]);
    printf("rx2_pattern: 0x%X 0x%X 0x%X 0x%X 0x%X \n", rx2_pattern[0], rx2_pattern[1], rx2_pattern[2], rx2_pattern[3], rx2_pattern[4]);
    printf("tx_buff    : 0x%X 0x%X 0x%X 0x%X 0x%X \n", tx_buff[0], tx_buff[1], tx_buff[2], tx_buff[3], tx_buff[4]);
    printf("rx_buff    : 0x%X 0x%X 0x%X 0x%X 0x%X \n", rx_buff[0], rx_buff[1], rx_buff[2], rx_buff[3], rx_buff[4]);
#endif
}

/* Function which perform transfer using specified config on the master side. */
template<typename T, const uint32_t tc_id, bool sync>
void test_transfer_master()
{
    spi_t spi_master = { 0 };
    spi_capabilities_t capabilities = { 0 };
    uint32_t count;
    int8_t status;
    DigitalOut *ss_pin = NULL;
    PinName ss = SPI_MASTER_SS;
    PinName miso = SPI_MASTER_MISO;
    PinName mosi = SPI_MASTER_MOSI;
    uint32_t clocked_symbols_1 = (test_cases[tc_id].slave_tx_cnt + test_cases[tc_id].slave_rx_cnt);
    uint32_t clocked_symbols_2 = (TEST_SYM_CNT + TEST_SYM_CNT);

    T tx_pattern[TEST_SYM_CNT];
    T rx1_pattern[TEST_SYM_CNT];
    T rx2_pattern[TEST_SYM_CNT];
    T tx_buff[TEST_SYM_CNT];
    T rx_buff[TEST_SYM_CNT];
    T fill_symbol;

    void *p_tx_buff = tx_buff;
    void *p_rx_buff = rx_buff;

    if (!test_cases[tc_id].master_tx_defined) {
        p_tx_buff = NULL;
    }

    if (!test_cases[tc_id].master_rx_defined) {
        p_rx_buff = NULL;
    }

    spi_get_capabilities(spi_get_module(SPI_MASTER_MOSI,
    SPI_MASTER_MISO,
    SPI_MASTER_CLK), NC, &capabilities);

    /* Adapt Full Duplex/Half Duplex settings. */
    switch (test_cases[tc_id].duplex)
    {
        case HALF_DUPLEX_MOSI:
            miso = NC;
            break;

        case HALF_DUPLEX_MISO:
            mosi = NC;
            break;

        default:
            clocked_symbols_1 = TEST_SYM_CNT;
            clocked_symbols_2 = TEST_SYM_CNT;
            break;
    }

    /* Adapt sync/async mode. */
    test_cases[tc_id].sync = sync;

    /* Adapt manual/auto SS handling by master. */
    if (!test_cases[tc_id].auto_ss) {
        ss_pin = new DigitalOut(SPI_MASTER_SS);
        ss = NC;
        *ss_pin = 1;
    }

    spi_init(&spi_master, false, mosi, miso, SPI_MASTER_CLK, ss);

    /* Continue if master can handle the config, otherwise skip it. */
    if (check_capabilities(&capabilities, test_cases[tc_id].symbol_size, false, test_cases[tc_id].duplex)) {

        /* Adapt min/max frequency for testing based of capabilities. */
        switch (test_cases[tc_id].freq_hz)
        {
            case FREQ_MIN:
                test_cases[tc_id].freq_hz = capabilities.minimum_frequency;
                break;

            case FREQ_MAX:
                test_cases[tc_id].freq_hz = capabilities.maximum_frequency;
                break;

            default:

                break;
        }

        /* Use default format to transfer test configuration to slave. Default format
         * should be handled by all SPI devices. */
        spi_format(&spi_master, test_cases[DEFAULT_CFG].symbol_size, test_cases[DEFAULT_CFG].mode, test_cases[DEFAULT_CFG].bit_ordering);

        spi_frequency(&spi_master, test_cases[DEFAULT_CFG].freq_hz);

        /* Send config to slave. Slave will return status indicating if given config can be
         * handled by slave.
         */
        wait_before_transmission();
        handle_ss(ss_pin, true);
        count = spi_transfer(&spi_master, &test_cases[tc_id], CONFIG_LEN, NULL, 0, (void*) &fill_symbol);
        handle_ss(ss_pin, false);

        wait_before_transmission();
        handle_ss(ss_pin, true);
        count = spi_transfer(&spi_master, NULL, 0, &status, CONFIG_STATUS_LEN, (void*) &fill_symbol);
        handle_ss(ss_pin, false);

        /* Continue if slave can handle the config, otherwise skip it. */
        if (status == CONFIG_STATUS_OK) {
            spi_format(&spi_master, test_cases[tc_id].symbol_size, test_cases[tc_id].mode, test_cases[tc_id].bit_ordering);

            spi_frequency(&spi_master, test_cases[tc_id].freq_hz);

            init_transmission_buffers<T>(tc_id, &tx_pattern[0], &rx1_pattern[0], &rx2_pattern[0], &tx_buff[0], &rx_buff[0], &fill_symbol);

            dump_buffers<T>(tx_pattern, rx1_pattern, rx2_pattern, tx_buff, rx_buff);

            wait_before_transmission();
            handle_ss(ss_pin, true);
            count = sync_async_transfer(&spi_master, p_tx_buff, test_cases[tc_id].master_tx_cnt, p_rx_buff, test_cases[tc_id].master_rx_cnt, (void*) &fill_symbol, sync);
            handle_ss(ss_pin, false);

            dump_buffers<T>(tx_pattern, rx1_pattern, rx2_pattern, tx_buff, rx_buff);

            TEST_ASSERT_EQUAL(true, check_buffers(rx1_pattern, p_rx_buff, sizeof(T) * TEST_SYM_CNT));
            TEST_ASSERT_EQUAL(true, check_buffers(tx_pattern, p_tx_buff, sizeof(T) * TEST_SYM_CNT));
            TEST_ASSERT_EQUAL(clocked_symbols_1, count);

            /* Init TX buffer with data received from slave. */

            if (p_tx_buff && p_rx_buff) {
                memcpy(p_tx_buff, p_rx_buff, sizeof(tx_buff));
                memcpy(tx_pattern, p_rx_buff, sizeof(tx_buff));
            }

            set_buffer(rx_buff, sizeof(rx_buff), 0x00);

            dump_buffers<T>(tx_pattern, rx1_pattern, rx2_pattern, tx_buff, rx_buff);

            wait_before_transmission();
            handle_ss(ss_pin, true);
            count = sync_async_transfer(&spi_master, tx_buff, TEST_SYM_CNT, rx_buff, TEST_SYM_CNT, (void*) &fill_symbol, sync);
            handle_ss(ss_pin, false);

            dump_buffers<T>(tx_pattern, rx1_pattern, rx2_pattern, tx_buff, rx_buff);

            TEST_ASSERT_EQUAL(true, check_buffers(rx2_pattern, p_rx_buff, sizeof(T) * TEST_SYM_CNT));
            TEST_ASSERT_EQUAL(true, check_buffers(tx_pattern, p_tx_buff, sizeof(T) * TEST_SYM_CNT));
            TEST_ASSERT_EQUAL(clocked_symbols_2, count);

        } else if (status == CONFIG_STATUS_NOT_SUPPORTED) {
            TEST_SKIP_MESSAGE("Config not supported by slave. Skipping. \n");
        } else {
            TEST_ASSERT_TRUE_MESSAGE(false, "Invalid configuration status. Communication error!");
        }
    } else {
        TEST_SKIP_MESSAGE("Config not supported by master. Skipping. \n");
    }

    spi_free(&spi_master);
}

utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(40, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("SPI master-slave sync com - default config", test_transfer_master<uint8_t, 0, true>),
    Case("SPI master-slave sync com - symbol size: 1", test_transfer_master<uint8_t, 1, true>),
    Case("SPI master-slave sync com - symbol size: 7", test_transfer_master<uint8_t, 2, true>),
    Case("SPI master-slave sync com - symbol size: 9", test_transfer_master<uint16_t, 3, true>),
    Case("SPI master-slave sync com - symbol size: 15", test_transfer_master<uint16_t, 4, true>),
    Case("SPI master-slave sync com - symbol size: 16", test_transfer_master<uint16_t, 5, true>),
    Case("SPI master-slave sync com - symbol size: 17", test_transfer_master<uint32_t, 6, true>),
    Case("SPI master-slave sync com - symbol size: 31", test_transfer_master<uint32_t, 7, true>),
    Case("SPI master-slave sync com - symbol size: 32", test_transfer_master<uint32_t, 8, true>),
    Case("SPI master-slave sync com - mode: idle low, sample second edge", test_transfer_master<uint8_t, 9, true>),
    Case("SPI master-slave sync com - mode: idle high, sample first edge", test_transfer_master<uint8_t, 10, true>),
    Case("SPI master-slave sync com - mode: idle high, sample second edge", test_transfer_master<uint8_t, 11, true>),
    Case("SPI master-slave sync com - bit ordering: LSB first", test_transfer_master<uint8_t, 12, true>),
    Case("SPI master-slave sync com - freq testing: 200 KHz", test_transfer_master<uint8_t, 13, true>),
    Case("SPI master-slave sync com - freq testing: 2 MHz", test_transfer_master<uint8_t, 14, true>),
    Case("SPI master-slave sync com - freq testing: min defined", test_transfer_master<uint8_t, 15, true>),
    Case("SPI master-slave sync com - freq testing: max defined", test_transfer_master<uint8_t, 16, true>),
    Case("SPI master-slave sync com - master: TX > RX", test_transfer_master<uint8_t, 17, true>),
    Case("SPI master-slave sync com - master: TX < RX", test_transfer_master<uint8_t, 18, true>),
    Case("SPI master-slave sync com - slave: TX > RX", test_transfer_master<uint8_t, 19, true>),
    Case("SPI master-slave sync com - slave: TX < RX", test_transfer_master<uint8_t, 20, true>),
    Case("SPI master-slave sync com - master: TX undefined", test_transfer_master<uint8_t, 21, true>),
    Case("SPI master-slave sync com - master: RX undefined", test_transfer_master<uint8_t, 22, true>),
    Case("SPI master-slave sync com - slave: TX undefined", test_transfer_master<uint8_t, 23, true>),
    Case("SPI master-slave sync com - slave: RX undefined", test_transfer_master<uint8_t, 24, true>),
    Case("SPI master-slave sync com - master: manual ss", test_transfer_master<uint8_t, 25, true>),
    Case("SPI master-slave sync com - half duplex (MOSI)", test_transfer_master<uint8_t, 26, true>),
    Case("SPI master-slave sync com - half duplex (MISO)", test_transfer_master<uint8_t, 27, true>),
#ifdef DEVICE_SPI_ASYNCH
    Case("SPI master-slave async com - default config", test_transfer_master<uint8_t, 0, false>),
    Case("SPI master-slave async com - symbol size: 1", test_transfer_master<uint8_t, 1, false>),
    Case("SPI master-slave async com - symbol size: 7", test_transfer_master<uint8_t, 2, false>),
    Case("SPI master-slave async com - symbol size: 9", test_transfer_master<uint16_t, 3, false>),
    Case("SPI master-slave async com - symbol size: 15", test_transfer_master<uint16_t, 4, false>),
    Case("SPI master-slave async com - symbol size: 16", test_transfer_master<uint16_t, 5, false>),
    Case("SPI master-slave async com - symbol size: 17", test_transfer_master<uint32_t, 6, false>),
    Case("SPI master-slave async com - symbol size: 31", test_transfer_master<uint32_t, 7, false>),
    Case("SPI master-slave async com - symbol size: 32", test_transfer_master<uint32_t, 8, false>),
    Case("SPI master-slave async com - mode: idle low, sample second edge", test_transfer_master<uint8_t, 9, false>),
    Case("SPI master-slave async com - mode: idle high, sample first edge", test_transfer_master<uint8_t, 10, false>),
    Case("SPI master-slave async com - mode: idle high, sample second edge", test_transfer_master<uint8_t, 11, false>),
    Case("SPI master-slave async com - bit ordering: LSB first", test_transfer_master<uint8_t, 12, false>),
    Case("SPI master-slave async com - freq testing: 200 KHz", test_transfer_master<uint8_t, 13, false>),
    Case("SPI master-slave async com - freq testing: 2 MHz", test_transfer_master<uint8_t, 14, false>),
    Case("SPI master-slave async com - freq testing: min defined", test_transfer_master<uint8_t, 15, false>),
    Case("SPI master-slave async com - freq testing: max defined", test_transfer_master<uint8_t, 16, false>),
    Case("SPI master-slave async com - master: TX > RX", test_transfer_master<uint8_t, 17, false>),
    Case("SPI master-slave async com - master: TX < RX", test_transfer_master<uint8_t, 18, false>),
    Case("SPI master-slave async com - slave: TX > RX", test_transfer_master<uint8_t, 19, false>),
    Case("SPI master-slave async com - slave: TX < RX", test_transfer_master<uint8_t, 20, false>),
    Case("SPI master-slave async com - master: TX undefined", test_transfer_master<uint8_t, 21, false>),
    Case("SPI master-slave async com - master: RX undefined", test_transfer_master<uint8_t, 22, false>),
    Case("SPI master-slave async com - slave: TX undefined", test_transfer_master<uint8_t, 23, false>),
    Case("SPI master-slave async com - slave: RX undefined", test_transfer_master<uint8_t, 24, false>),
    Case("SPI master-slave async com - master: manual ss", test_transfer_master<uint8_t, 25, false>),
    Case("SPI master-slave async com - half duplex (MOSI)", test_transfer_master<uint8_t, 26, false>),
    Case("SPI master-slave async com - half duplex (MISO)", test_transfer_master<uint8_t, 27, false>),
#endif
};

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}
#else /* BUILD_SLAVE */
/* Function which perform transfer using specified config on the slave side. */
template<typename T>
void slave_transfer(spi_t *obj, config_test_case_t *config)
{
    uint32_t count;
    T tx_buff[TEST_SYM_CNT];
    T rx_buff[TEST_SYM_CNT];
    T fill_symbol;
    uint32_t clocked_symbols_1 = TEST_SYM_CNT;
    uint32_t clocked_symbols_2 = TEST_SYM_CNT;

    if (config->duplex != FULL_DUPLEX) {
        clocked_symbols_1 = (config->slave_tx_cnt + config->slave_rx_cnt);
        clocked_symbols_2 = (TEST_SYM_CNT + TEST_SYM_CNT);
    }

    set_buffer(&tx_buff[0], sizeof(T), 0x11);
    set_buffer(&tx_buff[1], sizeof(T), 0x22);
    set_buffer(&tx_buff[2], sizeof(T), 0x33);
    set_buffer(&tx_buff[3], sizeof(T), 0x44);
    set_buffer(&tx_buff[4], sizeof(T), 0x55);

    set_buffer(rx_buff, sizeof(rx_buff), 0x00);

    set_buffer(&fill_symbol, sizeof(fill_symbol), 0xFF);

    void *p_tx_buff = tx_buff;
    void *p_rx_buff = rx_buff;

    if (!config->slave_tx_defined) {
        p_tx_buff = NULL;
    }

    if (!config->slave_rx_defined) {
        p_rx_buff = NULL;
    }

    count = sync_async_transfer(obj, p_tx_buff, config->slave_tx_cnt, p_rx_buff, config->slave_rx_cnt, &fill_symbol, config->sync);

    TEST_ASSERT_EQUAL_MESSAGE(clocked_symbols_1, count, "Transmission 1: Invalid number of clocked symbols.\r\n");

    /* Send data received from master in the previous transmission. */
    if (p_tx_buff && p_rx_buff) {
        memcpy(p_tx_buff, p_rx_buff, sizeof(tx_buff));
    }

    set_buffer(rx_buff, sizeof(rx_buff), 0x00);

    count = sync_async_transfer(obj, p_tx_buff, TEST_SYM_CNT, p_rx_buff, TEST_SYM_CNT, &fill_symbol, config->sync);

    TEST_ASSERT_EQUAL_MESSAGE(clocked_symbols_2, count, "Transmission 2: Invalid number of clocked symbols.\r\n");
}

/* Debug function to print received configuration details. */
void dump_config(config_test_case_t *config)
{
#if DEBUG
    Timer tim;
    tim.reset();
    tim.start();
    printf("TEST CASE CONFIGURATION\r\n");
    printf("symbol_size: %lu\r\n", (uint32_t) config->symbol_size);
    printf("spi_mode: %lu\r\n", (uint32_t) config->mode);
    printf("bit_ordering: %lu\r\n", (uint32_t) config->bit_ordering);
    printf("freq: %lu\r\n", (uint32_t) config->freq_hz);
    printf("master tx cnt: %lu\r\n", (uint32_t) config->master_tx_cnt);
    printf("master rx cnt: %lu\r\n", (uint32_t) config->master_rx_cnt);
    printf("slave tx cnt: %lu\r\n", (uint32_t) config->slave_tx_cnt);
    printf("slave rx cnt: %lu\r\n", (uint32_t) config->slave_rx_cnt);
    printf("master tx defined: %lu\r\n", (uint32_t) config->master_tx_defined);
    printf("master rx defined: %lu\r\n", (uint32_t) config->master_rx_defined);
    printf("slave tx defined: %lu\r\n", (uint32_t) config->slave_tx_defined);
    printf("slave rx defined: %lu\r\n", (uint32_t) config->slave_rx_defined);
    printf("auto ss: %lu\r\n", (uint32_t) config->slave_rx_defined);
    printf("full duplex: %lu\r\n", (uint32_t) config->duplex);
    printf("log time: %lu [us]\r\n", (uint32_t) tim.read_us());
    printf("---\r\n");
    tim.stop();
#endif
}

int main()
{
    spi_t spi_slave = { 0 };
    int8_t status;
    uint8_t fill_symbol = 0xFF;
    spi_capabilities_t capabilities = { 0 };

    printf(" ------- Welcome -------- \r\n");

    spi_get_capabilities(spi_get_module(SPI_SLAVE_MOSI,
                                        SPI_SLAVE_MISO,
                                        SPI_SLAVE_CLK),
                         SPI_SLAVE_SS,
                         &capabilities);

    while (true) {
        spi_init(&spi_slave, true, SPI_SLAVE_MOSI, SPI_SLAVE_MISO, SPI_SLAVE_CLK, SPI_SLAVE_SS);

        spi_format(&spi_slave, 8, SPI_MODE_IDLE_LOW_SAMPLE_FIRST_EDGE, SPI_BIT_ORDERING_MSB_FIRST);

        config_test_case_t config = { 0 };

        spi_transfer(&spi_slave, NULL, 0, &config, CONFIG_LEN, &fill_symbol);

        dump_config(&config);

        if (check_capabilities(&capabilities, config.symbol_size, true, config.duplex)
#ifndef DEVICE_SPI_ASYNCH
        && config.sync
#endif
        ) {
            status = CONFIG_STATUS_OK;
            PinName miso = SPI_SLAVE_MISO;
            PinName mosi = SPI_SLAVE_MOSI;

            spi_transfer(&spi_slave, &status, CONFIG_STATUS_LEN, NULL, 0, (void*) &fill_symbol);

            spi_free(&spi_slave);

            /* Adapt Full duplex/Half duplex settings. */
            switch (config.duplex)
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

            spi_init(&spi_slave, true, mosi, miso, SPI_SLAVE_CLK, SPI_SLAVE_SS);

            spi_format(&spi_slave, config.symbol_size, (spi_mode_t) config.mode, (spi_bit_ordering_t) config.bit_ordering);

            if (config.symbol_size <= 8) {
                slave_transfer<uint8_t>(&spi_slave, &config);
            } else if (config.symbol_size <= 16) {
                slave_transfer<uint16_t>(&spi_slave, &config);
            } else {
                slave_transfer<uint16_t>(&spi_slave, &config);
            }
        } else {
            status = CONFIG_STATUS_NOT_SUPPORTED;
            spi_transfer(&spi_slave, &status, CONFIG_STATUS_LEN, NULL, 0, &fill_symbol);
        }

        spi_free(&spi_slave);
    }
}
#endif
