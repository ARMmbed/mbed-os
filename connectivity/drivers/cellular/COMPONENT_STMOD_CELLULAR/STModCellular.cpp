/*
 * Copyright (c) 2018, Arm Limited and affiliates.
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

#include "STModCellular.h"
#include "rtos/ThisThread.h"
#include "mbed_trace.h"

#define TRACE_GROUP "STMD"

using namespace mbed;

STModCellular::STModCellular(FileHandle *fh) : STMOD_CELLULAR_MODEM(fh),
#if defined(TARGET_DISCO_L562QE)
    STMOD_SEL_12(PF_11),
    STMOD_SEL_34(PF_12),
#endif
    m_powerkey(MBED_CONF_STMOD_CELLULAR_POWER),
    m_reset(MBED_CONF_STMOD_CELLULAR_RESET),
    m_simsel0(MBED_CONF_STMOD_CELLULAR_SIMSEL0),
    m_simsel1(MBED_CONF_STMOD_CELLULAR_SIMSEL1),
    m_mdmdtr(MBED_CONF_STMOD_CELLULAR_MDMDTR),
    m_sim_reset(MBED_CONF_STMOD_CELLULAR_SIM_RESET),
    m_sim_clk(MBED_CONF_STMOD_CELLULAR_SIM_CLK),
    m_sim_data(MBED_CONF_STMOD_CELLULAR_SIM_DATA)
{
    tr_info("STModCellular creation");

#if defined(TARGET_DISCO_L562QE)
    /* See PinNames.h file, STMOD+ pins are configurable */
    STMOD_SEL_12 = 1;
    STMOD_SEL_34 = 1;
    tr_info("STMOD+ UART pins re-configuration");
#endif

    // start with modem disabled
    m_powerkey.write(0);
    m_reset.write(1);
    rtos::ThisThread::sleep_for(200);
    m_reset.write(0);
    rtos::ThisThread::sleep_for(150);

    rtos::ThisThread::sleep_for(50);
    m_simsel0.write(MBED_CONF_STMOD_CELLULAR_SIM_SELECTION & 0x01);
    m_simsel1.write(MBED_CONF_STMOD_CELLULAR_SIM_SELECTION & 0x02);
    rtos::ThisThread::sleep_for(50);
}

STModCellular::~STModCellular()
{
}

nsapi_error_t STModCellular::soft_power_on()
{
    tr_debug("STMOD cellular modem power ON");

#if (MBED_CONF_STMOD_CELLULAR_TYPE == STMOD_UG96)
    tr_info("Booting UG96");
    m_reset.write(1);
    rtos::ThisThread::sleep_for(200);
    m_reset.write(0);
    rtos::ThisThread::sleep_for(150);
    m_powerkey.write(1);
    rtos::ThisThread::sleep_for(150);
    m_powerkey.write(0);
    /* Because modem status is not available on STMOD+ connector,
     * let's wait for Modem complete boot */
    rtos::ThisThread::sleep_for(2300);
#elif (MBED_CONF_STMOD_CELLULAR_TYPE == STMOD_BG96)
    tr_info("Booting BG96");
    m_powerkey.write(1);
    m_reset.write(1);
    rtos::ThisThread::sleep_for(150);
    m_powerkey.write(0);
    m_reset.write(0);
    rtos::ThisThread::sleep_for(100);
    m_powerkey.write(1);
    rtos::ThisThread::sleep_for(200);
    m_powerkey.write(0);
    rtos::ThisThread::sleep_for(5000);
#endif

    nsapi_error_t err = STMOD_CELLULAR_MODEM::soft_power_on();
    if (err != 0) {
        return err;
    }

    // wait for RDY
    _at.lock();
    _at.set_at_timeout(5000);
    _at.set_stop_tag("RDY");
    bool rdy = _at.consume_to_stop_tag();
    (void)rdy;

    /*  Modem may send more bytes are RDY flag */
    _at.flush();

    /* Turn OFF ECHO before anything else */
    _at.set_stop_tag(mbed::OK);
    _at.cmd_start("ATE0");
    _at.cmd_stop();
    _at.consume_to_stop_tag();

    _at.restore_at_timeout();
    _at.unlock();

    tr_info("Modem %sready to receive AT commands", rdy ? "" : "NOT ");

    if ((MBED_CONF_STMOD_CELLULAR_CTS != NC) && (MBED_CONF_STMOD_CELLULAR_RTS != NC)) {
        tr_info("Enable flow control");

        pin_mode(MBED_CONF_STMOD_CELLULAR_CTS, PullDown);

        _at.lock();
        // enable CTS/RTS flowcontrol
        _at.set_stop_tag(mbed::OK);
        _at.set_at_timeout(400);
        _at.cmd_start("AT+IFC=");
        _at.write_int(2);
        _at.write_int(2);
        _at.cmd_stop_read_resp();
        err = _at.get_last_error();
        _at.restore_at_timeout();
        _at.unlock();

        if (err == NSAPI_ERROR_OK) {
            tr_debug("Flow control turned ON");
        } else {
            tr_error("Failed to enable hw flow control");
        }
    }

    rtos::ThisThread::sleep_for(500);

#if MBED_CONF_CELLULAR_DEBUG_AT
    _at.lock();
    /*  Verify Flow Control settings */
    _at.cmd_start("AT+IFC?");
    _at.cmd_stop_read_resp();
    _at.unlock();
#endif // MBED_CONF_CELLULAR_DEBUG_AT

    return err;
}

nsapi_error_t STModCellular::soft_power_off()
{
    _at.cmd_start("AT+QPOWD");
    _at.cmd_stop();
    rtos::ThisThread::sleep_for(1000);
    // should wait for POWERED DOWN with a time out up to 65 second according to the manual.
    // we cannot afford such a long wait though.
    return STMOD_CELLULAR_MODEM::soft_power_off();
}

#if MBED_CONF_STMOD_CELLULAR_PROVIDE_DEFAULT
#include "drivers/BufferedSerial.h"
CellularDevice *CellularDevice::get_default_instance()
{
    tr_debug("STMOD_CELLULAR default instance");

    static BufferedSerial serial(MBED_CONF_STMOD_CELLULAR_TX, MBED_CONF_STMOD_CELLULAR_RX, MBED_CONF_STMOD_CELLULAR_BAUDRATE);
    if ((MBED_CONF_STMOD_CELLULAR_CTS != NC) && (MBED_CONF_STMOD_CELLULAR_RTS != NC)) {
        serial.set_flow_control(SerialBase::RTSCTS, MBED_CONF_STMOD_CELLULAR_RTS, MBED_CONF_STMOD_CELLULAR_CTS);
    }
    static STModCellular device(&serial);
    return &device;
}
#endif

