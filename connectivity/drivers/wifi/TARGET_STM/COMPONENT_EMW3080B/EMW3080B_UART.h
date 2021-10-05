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
#include "mbed.h"
#include "mbed_debug.h"
#include "mbed_error.h"
#include "mx_wifi.h"
#include "core/mx_wifi_hci.h"
#include "core/mx_wifi_slip.h"



class EMW3080B_UART : public BufferedSerial {
public:
    EMW3080B_UART(bool    debug,
                  PinName tx,
                  PinName rx,
                  PinName reset
                 );

private:
    DigitalOut   _resetpin;
    THREAD_DECLARE(MX_WIFI_UARTRecvThreadId);


public:
    void process_txrx_poll(void);

    int8_t   IO_Init(uint16_t mode);
    int8_t   IO_DeInit(void);
    void     IO_Delay(uint32_t delayms);
    uint16_t IO_Send(uint8_t *data, uint16_t len);
    uint16_t IO_Receive(uint8_t *buffer, uint16_t buff_size);

    uint8_t  _debug_level;
};
