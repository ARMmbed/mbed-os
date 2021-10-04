/*
 * Copyright (c) STMicroelectronics 2021
 * SPDX-License-Identifier: Apache-2.0
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


/* Includes ------------------------------------------------------------------*/
#include <inttypes.h>

/* Private includes ----------------------------------------------------------*/
#include "mbed.h"
#include "mbed_debug.h"
#include "mbed_error.h"
#include "mx_wifi.h"
#include "core/mx_wifi_hci.h"



class EMW3080B_SPI : public SPI {
public:
    EMW3080B_SPI(bool debug     = MBED_CONF_EMW3080B_WIFI_DEBUG,
                 PinName mosi   = MBED_CONF_EMW3080B_WIFI_MOSI,
                 PinName miso   = MBED_CONF_EMW3080B_WIFI_MISO,
                 PinName sclk   = MBED_CONF_EMW3080B_WIFI_SCLK,
                 PinName nss    = MBED_CONF_EMW3080B_WIFI_NSS,
                 PinName notify = MBED_CONF_EMW3080B_WIFI_NOTIFY,
                 PinName flow   = MBED_CONF_EMW3080B_WIFI_FLOW,
                 PinName reset  = MBED_CONF_EMW3080B_WIFI_RESET
                );

private:
    DigitalOut   _resetpin;
    DigitalOut   _nsspin;

    DigitalIn   _notifypin;
    DigitalIn   _flowpin;

    InterruptIn *_notify_irq;
    InterruptIn *_flow_irq;

    void    flow_rise(void);
    void    notify_rise(void);
    int8_t  wait_flow_high(uint32_t timeout);

    SEM_DECLARE(spi_txrx_sem);
    SEM_DECLARE(spi_flow_rise_sem);
    SEM_DECLARE(spi_transfer_done_sem);

    uint8_t *spi_tx_data = NULL;
    uint16_t spi_tx_len  = 0;

    int32_t TransmitReceive(uint8_t *txdata, uint8_t *rxdata, uint32_t datalen, uint32_t timeout);
    int32_t Transmit(uint8_t *txdata, uint32_t datalen, uint32_t timeout);
    int32_t Receive(uint8_t *rxdata, uint32_t datalen, uint32_t timeout);
    void    spi_handler(int event);

    int8_t mx_wifi_spi_txrx_start(void);
    int8_t mx_wifi_spi_txrx_stop(void);

    THREAD_DECLARE(MX_WIFI_TxRxThreadId);


public:

    int8_t   IO_Init(uint16_t mode);
    int8_t    IO_DeInit(void);
    void     IO_Delay(uint32_t delayms);
    uint16_t IO_Send(uint8_t *data, uint16_t len);
    uint16_t IO_Receive(uint8_t *buffer, uint16_t buff_size);
    void    process_txrx_poll(uint32_t timeout);

    uint8_t _debug_level;
};
