/* mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
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

#if MBED_CONF_CELLULAR_PRESENT

#include "gpio_api.h"
#include "platform/mbed_thread.h"
#include "PinNames.h"
#include "BufferedSerial.h"
#include "ONBOARD_TELIT_ME310.h"
#include "ThisThread.h"
#include "CellularLog.h"


using namespace mbed;

ONBOARD_TELIT_ME310::ONBOARD_TELIT_ME310(FileHandle *fh) : TELIT_ME310(fh, P0_31, true)
{
}

nsapi_error_t ONBOARD_TELIT_ME310::hard_power_on()
{
    onboard_modem_init();
    return NSAPI_ERROR_OK;
}

nsapi_error_t ONBOARD_TELIT_ME310::hard_power_off()
{
    onboard_modem_deinit();
    return NSAPI_ERROR_OK;
}

nsapi_error_t ONBOARD_TELIT_ME310::soft_power_on()
{
    onboard_modem_power_up();
    // From Telit_xE310 Global form factor App note: It is mandatory to avoid sending data to the serial ports during the first 200ms of the module start-up.
    rtos::ThisThread::sleep_for(200);
    return NSAPI_ERROR_OK;
}

nsapi_error_t ONBOARD_TELIT_ME310::soft_power_off()
{
    onboard_modem_power_down();
    return NSAPI_ERROR_OK;
}

nsapi_error_t ONBOARD_TELIT_ME310::init()
{
    nsapi_error_t err = AT_CellularDevice::init();
    if (err != NSAPI_ERROR_OK) {
        return err;
    }
    _at.lock();
#if DEVICE_SERIAL_FC
    _at.at_cmd_discard("&K3;&C1;&D0", "");
#else
    _at.at_cmd_discard("&K0;&C1;&D0", "");
#endif

    // AT#QSS=1
    // Enable the Query SIM Status unsolicited indication in the ME. The format of the
    // unsolicited indication is the following:
    // #QSS: <status>
    // The ME informs at
    // every SIM status change through the basic unsolicited indication where <status> range is 0...1
    // <status> values:
    // - 0: SIM not inserted
    // - 1: SIM inserted
    _at.at_cmd_discard("#QSS", "=1");

    // AT#PSNT=1
    // Set command enables unsolicited result code for packet service network type (PSNT)
    // having the following format:
    // #PSNT:<nt>
    // <nt> values:
    // - 0: GPRS network
    // - 4: LTE network
    // - 5: unknown or not registered
    _at.at_cmd_discard("#PSNT", "=1");

    // AT+CMER=2
    // Set command enables sending of unsolicited result codes from TA to TE in the case of
    // indicator state changes.
    // Current setting: buffer +CIEV Unsolicited Result Codes in the TA when TA-TE link is
    // reserved (e.g. on-line data mode) and flush them to the TE after
    // reservation; otherwise forward them directly to the TE
    _at.at_cmd_discard("+CMER", "=2");

    // AT+CMEE=2
    // Set command disables the use of result code +CME ERROR: <err> as an indication of an
    // error relating to the +Cxxx command issued. When enabled, device related errors cause the +CME
    // ERROR: <err> final result code instead of the default ERROR final result code. ERROR is returned
    // normally when the error message is related to syntax, invalid parameters or DTE functionality.
    // Current setting: enable and use verbose <err> values
    _at.at_cmd_discard("+CMEE", "=2");

    // AT#PORTCFG=0
    // Set command allows to connect Service Access Points to the external physical ports giving a great
    // flexibility. Examples of Service Access Points: AT Parser Instance #1, #2, #3, etc..
    _at.at_cmd_discard("#PORTCFG", "=", "%d", EP_ATLAS_PORT_CONFIGURATION_VARIANT);

    // AT&W&P
    // - AT&W: Execution command stores on profile <n> the complete configuration of the device. If
    //         parameter is omitted, the command has the same behavior of AT&W0.
    // - AT&P: Execution command defines which full profile will be loaded at startup. If parameter
    //         is omitted, the command has the same behavior as AT&P0
    _at.at_cmd_discard("&W&P", "");

    return _at.unlock_return_error();
}

void ONBOARD_TELIT_ME310::press_power_button(int time_ms)
{
    gpio_t gpio_CELL_ON_OFF;
    gpio_init_out_ex(&gpio_CELL_ON_OFF, P0_31, 1);
    gpio_write(&gpio_CELL_ON_OFF, 1);
}

void ONBOARD_TELIT_ME310::onboard_modem_init()
{
}

void ONBOARD_TELIT_ME310::onboard_modem_deinit()
{
}

void ONBOARD_TELIT_ME310::onboard_modem_power_up()
{
    /* keep the power line low for 5 seconds */
    press_power_button(6000);
    /* give modem a little time to respond */
}

void ONBOARD_TELIT_ME310::onboard_modem_power_down()
{
    gpio_t gpio;

    gpio_init_out_ex(&gpio, P0_31, 0);
    /* keep the power line low for more than 3 seconds.
     * If 3G_ON_OFF pin is kept low for more than a second, a controlled disconnect and shutdown takes
     * place, Due to the network disconnect, shut-off can take up to 30 seconds. However, we wait for 10
     * seconds only   */
    thread_sleep_for(10 * 1000);
}

CellularDevice *CellularDevice::get_target_default_instance()
{
    static BufferedSerial serial(P1_2, P1_1, 115200);
#if DEVICE_SERIAL_FC
    if (P0_11 != NC && P1_8 != NC) {
        tr_debug("Modem flow control: RTS %d CTS %d", P0_11, P1_8);
        serial.set_flow_control(SerialBase::RTSCTS, P0_11, P1_8);
    }
#endif
    static ONBOARD_TELIT_ME310 device(&serial);
    return &device;
}

#endif // MBED_CONF_CELLULAR_PRESENT
