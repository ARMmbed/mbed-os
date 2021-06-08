/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

#if MBED_CONF_NSAPI_PRESENT

#include "QUECTEL_EC2X.h"
#include "ONBOARD_QUECTEL_EG25.h"
#include "gpio_api.h"
#include "platform/mbed_thread.h"
#include "PinNames.h"
#include "drivers/BufferedSerial.h"
#include "CellularLog.h"
#include "mbed_wait_api.h"

using namespace mbed;

ONBOARD_QUECTEL_EG25::ONBOARD_QUECTEL_EG25(FileHandle *fh, PinName pwr, bool active_high, PinName rst) : QUECTEL_EC2X(fh, pwr, active_high, rst)
{
    initialized = 0;
    ls = new DigitalOut(LS_LED);
    net_status = new InterruptIn(NET_STATUS);
}

nsapi_error_t ONBOARD_QUECTEL_EG25::hard_power_on()
{
    onboard_modem_init();
    return NSAPI_ERROR_OK;
}

nsapi_error_t ONBOARD_QUECTEL_EG25::hard_power_off()
{
    onboard_modem_deinit();
    return NSAPI_ERROR_OK;
}

nsapi_error_t ONBOARD_QUECTEL_EG25::soft_power_on()
{
    // See base function description. This function is for power on but reset too.
    onboard_modem_power_down();
    onboard_modem_power_up();
    return NSAPI_ERROR_OK;
}

nsapi_error_t ONBOARD_QUECTEL_EG25::soft_power_off()
{
    onboard_modem_power_down();
    return NSAPI_ERROR_OK;
}

CellularDevice *CellularDevice::get_target_default_instance()
{
    static BufferedSerial serial(MDMTXD, MDMRXD, 115200);
#if DEVICE_SERIAL_FC
    if (MDMRTS != NC && MDMCTS != NC) {
        tr_debug("Modem flow control: RTS %d CTS %d", MDMRTS, MDMCTS);
        serial.set_flow_control(SerialBase::RTSCTS, MDMRTS, MDMCTS);
    }
#endif
    static ONBOARD_QUECTEL_EG25 device(&serial, MDMPWRON, 0, MDMRST);
    return &device;
}

void ONBOARD_QUECTEL_EG25::press_power_button(int time_us)
{
    gpio_t gpio;
    gpio_init_inout(&gpio, MDMPWRON, PIN_OUTPUT, OpenDrainNoPull, 0);
    wait_us(time_us);
    gpio_init_inout(&gpio, MDMPWRON, PIN_INPUT, PullNone, 1);
}

void ONBOARD_QUECTEL_EG25::onboard_modem_init()
{
    // PWRKEY/RADIO_ONOFF = output open drain
    // RADIO_PWR = Enables the regulator. High = on. Has pulldown on board.
    // RADIO_RESET/MDMRST = Reset the radio. Open drain or float.
    // RADIO_STATUS/STATUS = input with pull up. Gets driven low when radio is on.

    gpio_t gpio;
    // Enable radio power regulator and buffer, configure RESET_N and PWR_ON.
    gpio_init_inout(&gpio, RADIO_PWR, PIN_OUTPUT, PushPullNoPull, 1);
    gpio_init_inout(&gpio, BUF_EN, PIN_OUTPUT, OpenDrainNoPull, 0);
    // MDMRST and MDMPWRON get driven low via open drain.
    gpio_init_inout(&gpio, MDMRST, PIN_INPUT, PullNone, 1);
    gpio_init_inout(&gpio, MDMPWRON, PIN_INPUT, PullNone, 1);
    gpio_init_in_ex(&gpio, RADIO_STATUS, PullUp);
    // Quectel hardware design guide recommends >= 30ms from powered until
    //  PWRKEY(MDMPWRON) is pulled low.
    wait_us(40000);
    initialized = 1;
}

void ONBOARD_QUECTEL_EG25::onboard_modem_deinit()
{
    // Make sure to power down before removing power!
    onboard_modem_power_down();
    gpio_t gpio;

    // Set all to inputs no pull. Let pull resistors do their thing. Allows
    // for lowest power draw. Disable radio power regulator and buffer.
    gpio_init_inout(&gpio, MDMRST, PIN_INPUT, PullNone, 1);
    gpio_init_inout(&gpio, MDMPWRON, PIN_INPUT, PullNone, 1);
    gpio_init_inout(&gpio, BUF_EN, PIN_INPUT, PullNone, 1);
    gpio_init_inout(&gpio, RADIO_PWR, PIN_INPUT, PullNone, 0);
    initialized = 0;
}

void ONBOARD_QUECTEL_EG25::link_status()
{
    ls->write(net_status->read());
}

void ONBOARD_QUECTEL_EG25::onboard_modem_power_up()
{
    // NET_STATUS = input. InterruptIn make LS LED follow.
    // LS_LED = output. Follow NET_STATUS.
    net_status->rise(callback(this,&ONBOARD_QUECTEL_EG25::link_status));
    net_status->fall(callback(this,&ONBOARD_QUECTEL_EG25::link_status));

    // Make sure the radio is initialized so it can be powered on.
    if (!initialized){
        onboard_modem_init();
    }

    gpio_t status;
    gpio_init_in_ex(&status, RADIO_STATUS, PullUp);

    int radio_off = gpio_read(&status);
    // If radio is on, do nothing.
    if (!radio_off) {
        return;
    }

    // Activate PWRKEY at least 500ms to turn on.
    press_power_button(550000);

    // Wait 10s for status to go low before retrying.
    uint8_t timeout = 10;
    do {
        thread_sleep_for(1000);
        radio_off = gpio_read(&status);
        if(!timeout--){
            press_power_button(550000);
            timeout = 10;
        }
    } while (radio_off);

    _at.lock();

    _at.set_at_timeout(15000);
    _at.resp_start();
    _at.set_stop_tag("RDY");
    bool rdy = _at.consume_to_stop_tag();
    _at.set_stop_tag(OK);

    _at.unlock();
    if (rdy) {
        tr_debug("Radio outputted RDY");
    } else {
        tr_debug("Radio did not output RDY within 15s.");
    }
}

void ONBOARD_QUECTEL_EG25::onboard_modem_power_down()
{
    gpio_t status;
    gpio_init_in_ex(&status, RADIO_STATUS, PullUp);
    // Need volatile so status check is not optimized out.
    volatile int radio_off = gpio_read(&status);
    // Do nothing if it's already off.
    if (radio_off) {
        return;
    }

    // Make sure the I/O are properly initialized.
    if (!initialized){
        onboard_modem_init();
    }

    do {
        // Activate PWRKEY for at least 650ms to turn off.
        press_power_button(680000);
        // Wait 40s for status to go high before resetting.
        uint8_t timeout = 40;
        do {
            thread_sleep_for(1000);
            radio_off = gpio_read(&status);
        } while (!radio_off && timeout);

        if (radio_off) {
            break;
        }
        else {
            onboard_modem_reset();
        }
    }
    while (!radio_off);
}

void ONBOARD_QUECTEL_EG25::onboard_modem_reset()
{
    gpio_t gpio;
    gpio_init_inout(&gpio, MDMRST, PIN_OUTPUT, OpenDrainNoPull, 0);
    thread_sleep_for(500);
    gpio_init_inout(&gpio, MDMRST, PIN_INPUT, PullNone, 1);
    thread_sleep_for(100);
}

#endif // MBED_CONF_NSAPI_PRESENT
