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
#include <stdio.h>
#include "TestHarness.h"
#include "mbed.h"

#if !DEVICE_SPI || !DEVICE_SPI_ASYNCH
#error spi_master_asynch requires asynch SPI
#endif


#define SHORT_XFR 3
#define LONG_XFR 16
#define TEST_BYTE0 0x00
#define TEST_BYTE1 0x11
#define TEST_BYTE2 0xFF
#define TEST_BYTE3 0xAA
#define TEST_BYTE4 0x55
#define TEST_BYTE5 0x50

#define TEST_BYTE_RX TEST_BYTE3
#define TEST_BYTE_TX_BASE TEST_BYTE5

#if defined(TARGET_K64F) || defined(TARGET_K66F)
#define TEST_MOSI_PIN PTD2
#define TEST_MISO_PIN PTD3
#define TEST_SCLK_PIN PTD1
#define TEST_CS_PIN   PTD0

#elif defined(TARGET_EFM32LG_STK3600) || defined(TARGET_EFM32GG_STK3700) || defined(TARGET_EFM32WG_STK3800)
#define TEST_MOSI_PIN PD0
#define TEST_MISO_PIN PD1
#define TEST_SCLK_PIN PD2
#define TEST_CS_PIN   PD3

#elif defined(TARGET_EFM32ZG_STK3200)
#define TEST_MOSI_PIN PD7
#define TEST_MISO_PIN PD6
#define TEST_SCLK_PIN PC15
#define TEST_CS_PIN   PC14

#elif defined(TARGET_EFM32HG_STK3400)
#define TEST_MOSI_PIN PE10
#define TEST_MISO_PIN PE11
#define TEST_SCLK_PIN PE12
#define TEST_CS_PIN   PE13

#elif defined(TARGET_RZ_A1H)
#define TEST_MOSI_PIN P10_14
#define TEST_MISO_PIN P10_15
#define TEST_SCLK_PIN P10_12
#define TEST_CS_PIN   P10_13

#elif defined(TARGET_FF_ARDUINO)
#define TEST_MOSI_PIN D11
#define TEST_MISO_PIN D12
#define TEST_SCLK_PIN D13
#define TEST_CS_PIN   D10

#elif defined(TARGET_DISCO_F429ZI)
#define TEST_MOSI_PIN PC_12
#define TEST_MISO_PIN PC_11
#define TEST_SCLK_PIN PC_10
#define TEST_CS_PIN   PA_15

#else
#error Target not supported
#endif

volatile int why;
volatile bool complete;
void cbdone(int event) {
    complete = true;
    why = event;
}


TEST_GROUP(SPI_Master_Asynchronous)
{
    uint8_t tx_buf[LONG_XFR];
    uint8_t rx_buf[LONG_XFR];
    SPI *obj;
    DigitalOut *cs;
    event_callback_t callback;

    void setup() {
        obj = new SPI(TEST_MOSI_PIN, TEST_MISO_PIN, TEST_SCLK_PIN);
        cs = new DigitalOut(TEST_CS_PIN);
        complete = false;
        why = 0;
        callback.attach(cbdone);

        // Set the default value of tx_buf
        for (uint32_t i = 0; i < sizeof(tx_buf); i++) {
            tx_buf[i] = i + TEST_BYTE_TX_BASE;
        }
        memset(rx_buf,TEST_BYTE_RX,sizeof(rx_buf));
    }
    void teardown() {
        delete obj;
        obj = NULL;
        delete cs;
        cs = NULL;
    }
    uint32_t cmpnbuf(uint8_t *expect, uint8_t *actual, uint32_t offset, uint32_t end, const char *file, uint32_t line)
    {
        uint32_t i;
        for (i = offset; i < end; i++){
            if (expect[i] != actual[i]) {
                break;
            }
        }
        if (i < end) {
            CHECK_EQUAL_LOCATION((int)expect[i], (int)actual[i], file, line);
        }
        CHECK_EQUAL_LOCATION(end, i, file, line);
        return i;
    }
    uint32_t cmpnbufc(uint8_t expect, uint8_t *actual, uint32_t offset, uint32_t end, const char *file, uint32_t line)
    {
        uint32_t i;
        for (i = offset; i < end; i++){
            if (expect != actual[i]) {
                break;
            }
        }
        if (i < end) {
            CHECK_EQUAL_LOCATION((int)expect, (int)actual[i], file, line);
        }
        CHECK_EQUAL_LOCATION(end, i, file, line);
        return i;
    }
    void dumpRXbuf() {
        uint32_t i;
        printf("\r\n");
        printf("RX Buffer Contents: [");
        //flushf(stdout);
        for (i = 0; i < sizeof(rx_buf); i++){
            printf("%02x",rx_buf[i]);
            if (i+1 < sizeof(rx_buf)){
                printf(",");
            }
        }
        printf("]\r\n");
    }
};

// SPI write tx length: FIFO-1, read length: 0
//   Checks: Null pointer exceptions, completion event
TEST(SPI_Master_Asynchronous, short_tx_0_rx)
{
    int rc;
    // Write a buffer of Short Transfer length.
    rc = obj->transfer( (const uint8_t *) tx_buf, SHORT_XFR,  (uint8_t *) NULL, 0, callback, SPI_EVENT_ALL);
    CHECK_EQUAL(0, rc);

    while (!complete);

    // Make sure that the callback fires.
    CHECK_EQUAL(why, SPI_EVENT_COMPLETE);

    // TODO: Check for a null pointer exception
}


//
// SPI write tx length: FIFO-1, read length: 0, non-null read pointer
//   Checks: Null pointer exceptions, completion event, canary values in read buffer
TEST(SPI_Master_Asynchronous, short_tx_0_rx_nn)
{
    int rc;
    // Write a buffer of Short Transfer length.
    rc = obj->transfer( (const uint8_t *)tx_buf,SHORT_XFR,(uint8_t *) rx_buf, 0,callback, SPI_EVENT_ALL);
    CHECK_EQUAL(0, rc);

    while (!complete);

    // Make sure that the callback fires.
    CHECK_EQUAL(SPI_EVENT_COMPLETE, why);

    // Check that the rx buffer is untouched
    cmpnbufc(TEST_BYTE_RX,rx_buf,0,sizeof(rx_buf),__FILE__,__LINE__);
}

// SPI write tx length: 0, read length: FIFO-1
//   Checks: Receive value==fill character, completion event
TEST(SPI_Master_Asynchronous, 0_tx_short_rx)
{
    int rc;
    // Read a buffer of Short Transfer length.
    rc = obj->transfer( (const uint8_t *)NULL,0,(uint8_t *) rx_buf,SHORT_XFR,callback, SPI_EVENT_ALL);
    CHECK_EQUAL(0, rc);

    while (!complete);

    // Make sure that the callback fires.
    CHECK_EQUAL(SPI_EVENT_COMPLETE, why);

    // TODO: Check for null pointer exception
    // Check that the receive buffer contains the fill byte.
    cmpnbufc(SPI_FILL_WORD,rx_buf,0,SHORT_XFR,__FILE__,__LINE__);
    // Check that remaining portion of the receive buffer contains the rx test byte
    cmpnbufc(TEST_BYTE_RX,rx_buf,SHORT_XFR,sizeof(rx_buf),__FILE__,__LINE__);
}

// SPI write tx length: 0, read length: FIFO-1
//   Checks: Receive value==fill character, completion event
TEST(SPI_Master_Asynchronous, 0_tx_nn_short_rx)
{
    int rc;
    // Read a buffer of Short Transfer length.
    rc = obj->transfer(tx_buf,0,rx_buf,SHORT_XFR,callback, SPI_EVENT_ALL);
    CHECK_EQUAL(0, rc);

    while (!complete);

    // Make sure that the callback fires.
    CHECK_EQUAL(SPI_EVENT_COMPLETE, why);

    // Check that the receive buffer contains the fill byte.
    cmpnbufc(SPI_FILL_WORD,rx_buf,0,SHORT_XFR,__FILE__,__LINE__);
    // Check that remaining portion of the receive buffer contains the rx test byte
    cmpnbufc(TEST_BYTE_RX,rx_buf,SHORT_XFR,sizeof(rx_buf),__FILE__,__LINE__);
}

// SPI write tx length: FIFO-1 ascending values, read length: FIFO-1
//   Checks: Receive buffer == tx buffer, completion event
TEST(SPI_Master_Asynchronous, short_tx_short_rx)
{
    int rc;
    // Write/Read a buffer of Long Transfer length.
    rc = obj->transfer( tx_buf,SHORT_XFR,rx_buf,SHORT_XFR,callback, SPI_EVENT_ALL);
    CHECK_EQUAL(0, rc);

    while (!complete);

    // Make sure that the callback fires.
    CHECK_EQUAL(SPI_EVENT_COMPLETE, why);

    // Check that the rx buffer contains the tx bytes
    cmpnbuf(tx_buf,rx_buf,0,SHORT_XFR,__FILE__,__LINE__);
    // Check that remaining portion of the receive buffer contains the rx test byte
    cmpnbufc(TEST_BYTE_RX,rx_buf,SHORT_XFR,sizeof(rx_buf),__FILE__,__LINE__);
}
// SPI write tx length: 2xFIFO ascending values, read length: 2xFIFO
//   Checks: Receive buffer == tx buffer, completion event
TEST(SPI_Master_Asynchronous, long_tx_long_rx)
{
    int rc;
    // Write/Read a buffer of Long Transfer length.
    rc = obj->transfer(tx_buf,LONG_XFR,rx_buf,LONG_XFR,callback, SPI_EVENT_ALL);
    CHECK_EQUAL(0, rc);

    while (!complete);

    // Make sure that the callback fires.
    CHECK_EQUAL(SPI_EVENT_COMPLETE, why);

    //dumpRXbuf();
    // Check that the rx buffer contains the tx bytes
    cmpnbuf(tx_buf,rx_buf,0,LONG_XFR,__FILE__,__LINE__);
    // Check that remaining portion of the receive buffer contains the rx test byte
    cmpnbufc(TEST_BYTE_RX,rx_buf,LONG_XFR,sizeof(rx_buf),__FILE__,__LINE__);
}

// SPI write tx length: 2xFIFO, ascending, read length: FIFO-1
//   Checks: Receive buffer == tx buffer, completion event, read buffer overflow
TEST(SPI_Master_Asynchronous, long_tx_short_rx)
{
    int rc;
    // Write a buffer of Short Transfer length.
    rc = obj->transfer(tx_buf,LONG_XFR,rx_buf,SHORT_XFR,callback, SPI_EVENT_ALL);
    CHECK_EQUAL(0, rc);

    while (!complete);

    // Make sure that the callback fires.
    CHECK_EQUAL(SPI_EVENT_COMPLETE, why);

    // Check that the rx buffer contains the tx bytes
    cmpnbuf(tx_buf,rx_buf,0,SHORT_XFR,__FILE__,__LINE__);
    // Check that remaining portion of the receive buffer contains the rx test byte
    cmpnbufc(TEST_BYTE_RX,rx_buf,SHORT_XFR,sizeof(rx_buf),__FILE__,__LINE__);
}

// SPI write tx length: FIFO-1, ascending, read length: 2xFIFO
//   Checks: Receive buffer == tx buffer, then fill, completion event
TEST(SPI_Master_Asynchronous, short_tx_long_rx)
{
    int rc;
    // Write a buffer of Short Transfer length.
    rc = obj->transfer(tx_buf,SHORT_XFR,rx_buf,LONG_XFR,callback, SPI_EVENT_ALL);
    CHECK_EQUAL(0, rc);

    while (!complete);

    // Make sure that the callback fires.
    CHECK_EQUAL(SPI_EVENT_COMPLETE, why);

    //dumpRXbuf();
    // Check that the rx buffer contains the tx bytes
    cmpnbuf(tx_buf,rx_buf,0,SHORT_XFR,__FILE__,__LINE__);
    // Check that the rx buffer contains the tx fill bytes
    cmpnbufc(SPI_FILL_WORD,rx_buf,SHORT_XFR,LONG_XFR,__FILE__,__LINE__);
    // Check that remaining portion of the receive buffer contains the rx test byte
    cmpnbufc(TEST_BYTE_RX,rx_buf,LONG_XFR,sizeof(rx_buf),__FILE__,__LINE__);
}

TEST(SPI_Master_Asynchronous, queue_test)
{
    int rc;
    // Write/Read a buffer of Long Transfer length.
    rc = obj->transfer( tx_buf,4,rx_buf,4,callback, 0);
    CHECK_EQUAL(0, rc);
    rc = obj->transfer( &tx_buf[4],4, &rx_buf[4],4,callback, 0);
    CHECK_EQUAL(0, rc);
    rc = obj->transfer( &tx_buf[8],4, &rx_buf[8],4,callback, SPI_EVENT_ALL);
    CHECK_EQUAL(0, rc);

    while (!complete);

    // Make sure that the callback fires.
    CHECK_EQUAL(SPI_EVENT_COMPLETE, why);

    // Check that the rx buffer contains the tx bytes
    cmpnbuf(tx_buf,rx_buf,0,12,__FILE__,__LINE__);
    // Check that remaining portion of the receive buffer contains the rx test byte
    cmpnbufc(TEST_BYTE_RX,rx_buf,12,sizeof(rx_buf),__FILE__,__LINE__);
}
