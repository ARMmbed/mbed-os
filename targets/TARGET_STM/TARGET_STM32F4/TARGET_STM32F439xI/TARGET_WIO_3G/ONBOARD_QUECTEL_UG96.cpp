/* mbed Microcontroller Library
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

#if MBED_CONF_NSAPI_PRESENT

#include "ONBOARD_QUECTEL_UG96.h"

#include "gpio_api.h"
#include "platform/mbed_thread.h"
#include "PinNames.h"
#include "drivers/BufferedSerial.h"

#define WAIT_AFTER_POWR_CHANGED (1000)  // [msec.]

using namespace mbed;

ONBOARD_QUECTEL_UG96::ONBOARD_QUECTEL_UG96(FileHandle *fh) : QUECTEL_UG96(fh)
{
    onboard_modem_init();
}

nsapi_error_t ONBOARD_QUECTEL_UG96::hard_power_on()
{
    onboard_modem_init();
    return NSAPI_ERROR_OK;
}

nsapi_error_t ONBOARD_QUECTEL_UG96::hard_power_off()
{
    onboard_modem_deinit();
    return NSAPI_ERROR_OK;
}

nsapi_error_t ONBOARD_QUECTEL_UG96::soft_power_on()
{
    onboard_modem_power_up();
    return NSAPI_ERROR_OK;
}

nsapi_error_t ONBOARD_QUECTEL_UG96::soft_power_off()
{
    onboard_modem_power_down();
    return NSAPI_ERROR_OK;
}

void ONBOARD_QUECTEL_UG96::press_power_button(int time_ms)
{
    gpio_t gpio;

    gpio_init_out_ex(&gpio, PWRKEY, 1);
    thread_sleep_for(time_ms);
    gpio_write(&gpio, 0);
}

void ONBOARD_QUECTEL_UG96::onboard_modem_init()
{
    gpio_t gpio;

    // Power Supply
    gpio_init_out_ex(&gpio, M_POWR, 0);
    // Turn On/Off
    gpio_init_out_ex(&gpio, PWRKEY, 0);
    gpio_init_out_ex(&gpio, RESET_MODULE, 0);
    // Status Indication
    gpio_init_in_ex(&gpio, MDMSTAT, PullUp);
    // Main UART Interface
    gpio_init_out_ex(&gpio, MDMDTR, 0);

    thread_sleep_for(WAIT_AFTER_POWR_CHANGED);
}

void ONBOARD_QUECTEL_UG96::onboard_modem_deinit()
{
    gpio_t gpio;

    // Power supply OFF
    gpio_init_out_ex(&gpio, M_POWR, 0);
    thread_sleep_for(WAIT_AFTER_POWR_CHANGED);
}

void ONBOARD_QUECTEL_UG96::onboard_modem_power_up()
{
    gpio_t gpio;

    // Power supply ON
    gpio_init_out_ex(&gpio, M_POWR, 1);
    thread_sleep_for(WAIT_AFTER_POWR_CHANGED);

    // Turn on
    thread_sleep_for(100);
    press_power_button(200);
}

void ONBOARD_QUECTEL_UG96::onboard_modem_power_down()
{
    gpio_t gpio;

    // Power supply OFF
    gpio_init_out_ex(&gpio, M_POWR, 0);
    thread_sleep_for(WAIT_AFTER_POWR_CHANGED);
}

CellularDevice *CellularDevice::get_target_default_instance()
{
    static BufferedSerial serial(MDMTXD, MDMRXD, 115200);
    static ONBOARD_QUECTEL_UG96 device(&serial);
    return &device;
}

#endif // MBED_CONF_NSAPI_PRESENT
