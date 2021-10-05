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
/* Private includes ----------------------------------------------------------*/
#include "EMW3080B_SPI.h"

#define DEBUG_SILENT  0
#define DEBUG_WARNING 1
#define DEBUG_INFO    2
#define DEBUG_LOG     3
#define DEFAULT_DEBUG DEBUG_SILENT

static  EMW3080B_SPI  *emw3080B_spi_object;


EMW3080B_SPI::EMW3080B_SPI(bool    debug,
                           PinName mosi,
                           PinName miso,
                           PinName sclk,
                           PinName nss,
                           PinName notify,
                           PinName flow,
                           PinName reset
                          ): SPI(mosi, miso, sclk), _resetpin(reset), _nsspin(nss), _notifypin(notify), _flowpin(flow)
{
    if (debug) {
        _debug_level = DEBUG_INFO; // too much real time impact with DEBUG_LOG
    } else {
        _debug_level = DEFAULT_DEBUG;
    }

    _notify_irq = new InterruptIn(notify);
    _flow_irq   = new InterruptIn(flow);

    _notify_irq->rise(NULL);
    _flow_irq->rise(NULL);

    emw3080B_spi_object = this;
    /* MXWIFI supports 30 Mhz SPI clock  but Mbed implementation is limitted to 5 Mhz */
    /* DMA implementation would be required to increase this supported frequency    */
    SPI::frequency(5000000);
    /* Phase 0 and One edge => mode 0 */
    SPI::format(8, 0);

    SPI::set_default_write_value(0);

    SEM_INIT(spi_transfer_done_sem, 1);
}

int  flow_rise_count   = 0;
int  notify_rise_count = 0;

void EMW3080B_SPI::flow_rise()
{
    flow_rise_count++;
    SEM_SIGNAL(spi_flow_rise_sem);
}

void EMW3080B_SPI::notify_rise()
{
    notify_rise_count++;
    if (SEM_SIGNAL(spi_txrx_sem) != SEM_OK) {
        error("failed to signal spi_txrx_sem\n");
    }
}


#pragma pack(1)
typedef struct _spi_header {
    uint8_t  type;
    uint16_t len;
    uint16_t lenx;
    uint8_t  dummy[3];
} spi_header_t;
#pragma pack()


/* Private define ------------------------------------------------------------*/
/* SPI protocol */
#define SPI_WRITE         (0x0A)
#define SPI_READ          (0x0B)
#define SPI_HEADER_SIZE   (5)
#define SPI_DATA_SIZE     (MX_WIFI_HCI_DATA_SIZE)

#define SPI_WAITING_FLOW_HIGH_TIMEOUT    (100) /* ms unit */
#define SPI_MAX_TRANSMIT_DURATION        (500) /* ms unit */


/* SPI CS */
#define MX_WIFI_SPI_CS_HIGH() _nsspin = 1
#define MX_WIFI_SPI_CS_LOW()  _nsspin = 0



/**
  * @brief  Initialize SPI
  * @param  None
  * @retval None
  */
int8_t EMW3080B_SPI::IO_Init(uint16_t mode)
{
    int8_t ret = 0;

    if (MX_WIFI_RESET == mode) {
        /* HW reset */
        MX_WIFI_SPI_CS_HIGH();

        _resetpin = 0;
        rtos::ThisThread::sleep_for(100ms);
        _resetpin = 1;
        rtos::ThisThread::sleep_for(1200ms);
    } else {
        ret = mx_wifi_spi_txrx_start();
    }
    return ret;
}


/**
  * @brief  DeInitialize SPI
  * @param  None
  * @retval None
  */
int8_t EMW3080B_SPI::IO_DeInit(void)
{
    mx_wifi_spi_txrx_stop();
    return 0;
}

void  EMW3080B_SPI::IO_Delay(uint32_t delayms)
{
    return;
}

uint16_t EMW3080B_SPI::IO_Send(uint8_t *data, uint16_t len)
{
    if ((NULL == data) || (0 == len) || (len > SPI_DATA_SIZE)) {
        return 0;
    }

    spi_tx_data = data;
    spi_tx_len  = len;

    if (SEM_SIGNAL(spi_txrx_sem) != SEM_OK) {
        /* Happen if received thread did not have a chance to run on time, need to increase priority */
        debug_if(_debug_level >= DEBUG_WARNING, "EMW3080B_SPI : spi semaphore has been already notified\n");
    }

    return len;
}


uint16_t  EMW3080B_SPI::IO_Receive(uint8_t *buffer, uint16_t buff_size)
{
    return 0U;
}


int8_t EMW3080B_SPI::wait_flow_high(uint32_t timeout)
{
    int8_t ret = 0;

    if (SEM_WAIT(spi_flow_rise_sem, timeout, NULL) != SEM_OK) {
        debug_if(_debug_level >= DEBUG_WARNING, "EMW3080B_SPI : wait_flow_high semaphore failed, timeout\n");
        ret = -1;
    }

    return ret;
}

int  spi_handler_count = 0;
int  spi_handler_event_value = 0;

void EMW3080B_SPI::spi_handler(int event)
{
    spi_handler_count++;
    spi_handler_event_value = event;

    SEM_SIGNAL(spi_transfer_done_sem);
}


int32_t EMW3080B_SPI::TransmitReceive(uint8_t *txdata, uint8_t *rxdata, uint32_t datalen,
                                      uint32_t timeout)
{
    int32_t ret = 0;
    debug_if(_debug_level >= DEBUG_LOG, "EMW3080B_SPI : Spi Tx Rx %" PRIu32 "\n", datalen);
    SPI::transfer((const uint8_t *) txdata, (int) datalen, rxdata, (int) datalen, callback(this, &EMW3080B_SPI::spi_handler), SPI_EVENT_COMPLETE);
    if (SEM_WAIT(spi_transfer_done_sem, timeout, NULL) != SEM_OK) {
        debug_if(_debug_level >= DEBUG_WARNING, "EMW3080B_SPI : Timeout on TransmitReceive %d\n", spi_handler_count);
        ret = -1;
    }

    return ret;
}


int32_t EMW3080B_SPI::Transmit(uint8_t *txdata, uint32_t datalen, uint32_t timeout)
{
    int32_t ret = 0;
    debug_if(_debug_level >= DEBUG_LOG, "EMW3080B_SPI : Spi Tx %" PRIu32 "\n", datalen);
    SPI::transfer((const uint8_t *) txdata, (int) datalen, (uint8_t *)NULL, (int) datalen, callback(this, &EMW3080B_SPI::spi_handler), SPI_EVENT_COMPLETE);
    if (SEM_WAIT(spi_transfer_done_sem, timeout, NULL) != SEM_OK) {
        debug_if(_debug_level >= DEBUG_WARNING, "EMW3080B_SPI : Timeout on Transmit\n");
        ret = -1;
    }
    return ret;
}

int32_t EMW3080B_SPI::Receive(uint8_t *rxdata, uint32_t datalen, uint32_t timeout)
{
    int32_t ret = 0;
    debug_if(_debug_level >= DEBUG_LOG, "EMW3080B_SPI : Spi Rx %" PRIu32 "\n", datalen);
    SPI::transfer((const uint8_t *) NULL, (int) datalen, rxdata, (int) datalen, callback(this, &EMW3080B_SPI::spi_handler), SPI_EVENT_COMPLETE);
    if (SEM_WAIT(spi_transfer_done_sem, timeout, NULL) != SEM_OK) {
        debug_if(_debug_level >= DEBUG_WARNING, "EMW3080B_SPI : Timeout on Receive\n");
        ret = -1;
    }
    return ret;
}


void EMW3080B_SPI::process_txrx_poll(uint32_t timeout)
{

    spi_header_t mheader, sheader;
    int32_t  ret;
    uint8_t *txdata;
    uint8_t *p = NULL;
    uint16_t datalen;
    static mx_buf_t *netb = NULL;
    bool first_miss = true;

    MX_WIFI_SPI_CS_HIGH();

    while (netb == NULL) {
        netb = MX_NET_BUFFER_ALLOC(MX_WIFI_BUFFER_SIZE);
        if (netb == NULL) {
            rtos::ThisThread::sleep_for(1ms);
            if (true == first_miss) {
                first_miss = false;
                debug_if(_debug_level >= DEBUG_WARNING, "EMW3080B_SPI : Running Out of buffer for RX\n");
            }
        }
    }

    /* waiting for data to be sent or to be received */
    if (SEM_WAIT(spi_txrx_sem, timeout, NULL) == SEM_OK) {
        if (spi_tx_data == NULL) {
            if (_notifypin == 0) {
                /* tx data null means no data to send , _flowpin low means no data to received */
                error("EMW3080B_SPI : Nothing to process but wake UP!!!\n");
            }
            txdata = NULL;
            mheader.len = 0;
        } else {
            mheader.len = spi_tx_len;
            txdata = spi_tx_data;
        }

        mheader.type = SPI_WRITE;
        mheader.lenx = ~mheader.len;
        mheader.dummy[0] = 0;
        mheader.dummy[1] = 0;
        mheader.dummy[2] = 0;

        MX_WIFI_SPI_CS_LOW();

        /* wait EMW to be ready */
        if (wait_flow_high(SPI_WAITING_FLOW_HIGH_TIMEOUT) != 0) {
            MX_WIFI_SPI_CS_HIGH();
            debug_if(_debug_level >= DEBUG_WARNING, "EMW3080B_SPI : wait flow high timeout, notify_rise_count %d  flow_rise_count %d  flow is %d\n", notify_rise_count, flow_rise_count, (int)_flowpin);
        }

        /* transmit only header part */
        sheader.type = 0;
        sheader.len = 0;

        if (TransmitReceive((uint8_t *)&mheader, (uint8_t *)&sheader, sizeof(mheader), SPI_MAX_TRANSMIT_DURATION)) {
            MX_WIFI_SPI_CS_HIGH();
            debug_if(_debug_level >= DEBUG_WARNING, "EMW3080B_SPI : Send mheader error\r\n");
        }
        if (sheader.type != SPI_READ) {
            MX_WIFI_SPI_CS_HIGH();
            debug_if(_debug_level >= DEBUG_WARNING, "EMW3080B_SPI : Invalid SPI type %02x\r\n", sheader.type);
        }
        if ((sheader.len ^ sheader.lenx) != 0xFFFF) {
            MX_WIFI_SPI_CS_HIGH();
            debug_if(_debug_level >= DEBUG_WARNING, "EMW3080B_SPI : Invalid len %04x-%04x\r\n", sheader.len, sheader.lenx);
        }

        /* send or received header must be not null */
        if ((sheader.len == 0) && (mheader.len == 0)) {
            MX_WIFI_SPI_CS_HIGH();
            return;
        }

        if ((sheader.len > SPI_DATA_SIZE) || (mheader.len > SPI_DATA_SIZE)) {
            MX_WIFI_SPI_CS_HIGH();
            debug_if(_debug_level >= DEBUG_WARNING, "EMW3080B_SPI : SPI length invalid: %d-%d\r\n", sheader.len, mheader.len);
        }

        /* keep max length */
        if (mheader.len > sheader.len) {
            datalen = mheader.len;
        } else {
            datalen = sheader.len;
        }

        /* allocate a buffer */
        if (sheader.len > 0) {
            /* get payload */
            p = MX_NET_BUFFER_PAYLOAD(netb);
        } else {
            p = NULL;
        }

        /* flow must be high */

        if (wait_flow_high(SPI_WAITING_FLOW_HIGH_TIMEOUT) != 0) {
            MX_WIFI_SPI_CS_HIGH();
            debug_if(_debug_level >= DEBUG_WARNING, "EMW3080B_SPI : timeout waiting for flow high after transfer !\r\n");
        }

        /* transmit and received */
        if (NULL != txdata) {
            spi_tx_data = NULL;
            spi_tx_len = 0;
            if (NULL != p) {
                ret = TransmitReceive(txdata, p, datalen, SPI_MAX_TRANSMIT_DURATION);
            } else {
                ret = Transmit(txdata, datalen, SPI_MAX_TRANSMIT_DURATION);
            }
        } else {
            ret = Receive(p, datalen, SPI_MAX_TRANSMIT_DURATION);
        }

        if (ret) {
            MX_WIFI_SPI_CS_HIGH();
            debug_if(_debug_level >= DEBUG_WARNING, "EMW3080B_SPI : Transmit/Receive data timeout\r\n");
        }

        /* resize the input buffer and sent it back to processing thread */
        if (sheader.len > 0) {
            MX_NET_BUFFER_SET_PAYLOAD_SIZE(netb, sheader.len);
            mx_wifi_hci_input(netb);
            netb = NULL;

        }
        MX_WIFI_SPI_CS_HIGH();

    }
}

void mx_wifi_spi_txrx_task(void)
{
    while (1) {
        emw3080B_spi_object->process_txrx_poll(WAIT_FOREVER);
    }
}

int8_t EMW3080B_SPI::mx_wifi_spi_txrx_start(void)
{
    int8_t ret = 0;

    SEM_INIT(spi_txrx_sem, 2);
    SEM_INIT(spi_flow_rise_sem, 1);
    SEM_INIT(spi_transfer_done_sem, 1);
    _notify_irq->rise(callback(this, &EMW3080B_SPI::notify_rise));
    _flow_irq->rise(callback(this, &EMW3080B_SPI::flow_rise));

    if (THREAD_OK == THREAD_INIT(MX_WIFI_TxRxThreadId,
                                 mx_wifi_spi_txrx_task, NULL,
                                 MX_WIFI_SPI_THREAD_STACK_SIZE,
                                 MX_WIFI_SPI_THREAD_PRIORITY)) {
        ret = 0;
    } else {
        ret = -1;
    }

    return ret;
}

int8_t EMW3080B_SPI::mx_wifi_spi_txrx_stop(void)
{

    THREAD_DEINIT(MX_WIFI_TxRxThreadId);
    _notify_irq->rise(NULL);
    _flow_irq->rise(NULL);
    SEM_DEINIT(spi_txrx_sem);
    SEM_DEINIT(spi_flow_rise_sem);
    SEM_DEINIT(spi_transfer_done_sem);
    return 0;
}
