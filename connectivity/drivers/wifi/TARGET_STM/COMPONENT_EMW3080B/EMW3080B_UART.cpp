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
#include "EMW3080B_UART.h"

#define DEBUG_SILENT  0
#define DEBUG_WARNING 1
#define DEBUG_INFO    2
#define DEBUG_LOG     3
#define DEFAULT_DEBUG DEBUG_SILENT

//#define  DEBUG_UART_DATA  1

static  EMW3080B_UART  *emw3080b_uart_object;


EMW3080B_UART::EMW3080B_UART(bool    debug,
                             PinName tx,
                             PinName rx,
                             PinName reset
                            ): BufferedSerial(tx, rx, 230400), _resetpin(reset)
{
    emw3080b_uart_object = this;

    if (debug) {
        _debug_level = DEBUG_LOG;
    } else {
        _debug_level = DEFAULT_DEBUG;
    }
}

#if DEBUG_UART_DATA
void debug_print(char *prefix, uint8_t *data, uint16_t len);
void debug_print(char *prefix, uint8_t *data, uint16_t len)
{
    if (NULL != data) {
        printf("%s[%d]:\n", prefix, len);
        for (int32_t i = 0; i < len; i++) {
            printf(" %02x", data[i]);
        }
        printf("\n");
        for (int32_t i = 0; i < len; i++) {
            printf(" %c ", data[i]);
        }
        printf("\n");
    }
}
#else
#define debug_print(...)
#endif /* DEBUG_UART_DATA */


void EMW3080B_UART::process_txrx_poll(void)
{
    uint8_t  c;
    ssize_t  n;
    mx_buf_t *nbuf = NULL;

    n = read(&c, 1);
    if (n == 1) {
        nbuf = slip_input_byte(c);
        if (NULL != nbuf) {
            debug_print("URX", MX_NET_BUFFER_PAYLOAD(nbuf), MX_NET_BUFFER_GET_PAYLOAD_SIZE(nbuf));
            mx_wifi_hci_input(nbuf);
        }
    }
}


void HWInterfaceReceivedPollTask(void)
{
    while (true) {
        emw3080b_uart_object->process_txrx_poll();
    }
}



int8_t EMW3080B_UART::IO_Init(uint16_t mode)
{
    int8_t rc = 0;

    if (MX_WIFI_RESET == mode) {
        /* HW reset */
        _resetpin = 0;
        rtos::ThisThread::sleep_for(100ms);
        _resetpin = 1;
        rtos::ThisThread::sleep_for(1200ms);
    } else {

        if (THREAD_OK == THREAD_INIT(MX_WIFI_UARTRecvThreadId,
                                     HWInterfaceReceivedPollTask, NULL,
                                     MX_WIFI_UART_THREAD_STACK_SIZE, MX_WIFI_UART_THREAD_PRIORITY)) {
            rc = MX_WIFI_STATUS_OK;
        } else {
            rc = MX_WIFI_STATUS_ERROR;
        }
    }
    return rc;
}


int8_t EMW3080B_UART::IO_DeInit(void)
{
    int8_t rc = 0;
    THREAD_DEINIT(MX_WIFI_UARTRecvThreadId);
    return rc;
}

void  EMW3080B_UART::IO_Delay(uint32_t delayms)
{
    printf("%s %d\n", __FILE__, __LINE__);
    return;
}

uint16_t EMW3080B_UART::IO_Send(uint8_t *pdata, uint16_t len)
{
    uint16_t rc;
    debug_print("UTX", pdata, len);
    rc = write(pdata, len);
    return rc;
}

uint16_t EMW3080B_UART::IO_Receive(uint8_t *pdata, uint16_t request_len)
{
    return 0;
}
