/*
 * mbed Microcontroller Library
 * Copyright (c) 2017-2017 ARM Limited
 * Copyright (c) 2017-2018 Future Electronics
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

#include "hci_api.h"
#include "bstream.h"
#include "driver/CordioHCIDriver.h"
#include "drivers/IPCPipeTransportDriver.h"
#include "psoc6_utils.h"

using namespace ble::vendor::cordio;
using namespace ble::vendor::cypress;

const uint16_t HCI_VEND_SET_BD_ADDR = 0xfda0;
const uint8_t  HCI_VEND_SET_BD_ADDR_LEN = 7; /* MAC address + address type */

class Psoc6HCIDriver : public CordioHCIDriver
{
public:
    Psoc6HCIDriver(IPCPipeTransportDriver& transport_driver) :
        CordioHCIDriver(transport_driver)
    {
    }


private:

    struct BdAddress {
        uint8_t mac_address[6];
        uint8_t addr_type;

        BdAddress() : addr_type(0) {}
    };

    virtual cordio::buf_pool_desc_t get_buffer_pool_description();

    /**
     * Initialize the chip.
     * The transport is up at that time.
     */
    virtual void do_initialize();

    /**
     * Terminate the driver
     */
    virtual void do_terminate() {}

    virtual void handle_reset_sequence(uint8_t *pMsg);

private:
    BdAddress   bd_address;
};

cordio::buf_pool_desc_t Psoc6HCIDriver::get_buffer_pool_description()
{
    // Use default buffer pool
    return ble::vendor::cordio::CordioHCIDriver::get_default_buffer_pool_description();
}


void Psoc6HCIDriver::do_initialize()
{
    cy_get_bd_mac_address(bd_address.mac_address);
}


void Psoc6HCIDriver::handle_reset_sequence(uint8_t *pMsg) {

    uint16_t       opcode;

    /* if event is a command complete event */
    if (*pMsg == HCI_CMD_CMPL_EVT) {
        /* parse parameters */
        uint8_t *pMsg2 = pMsg + HCI_EVT_HDR_LEN;
        pMsg2++;                  /* skip num packets */
        BSTREAM_TO_UINT16(opcode, pMsg2);
        pMsg2 -= 2;
        /* decode opcode */
        switch (opcode) {
            case HCI_OPCODE_RESET:
                /* send next command in sequence */
                HciVendorSpecificCmd(HCI_VEND_SET_BD_ADDR,
                                     HCI_VEND_SET_BD_ADDR_LEN,
                                     reinterpret_cast<uint8_t*>(&bd_address));
                break;

            case HCI_VEND_SET_BD_ADDR:
                /* pretend we have just completed reset */
                UINT16_TO_BSTREAM(pMsg2, HCI_OPCODE_RESET);
                CordioHCIDriver::handle_reset_sequence(pMsg);
                break;

            default:
                /* pass to parent */
                CordioHCIDriver::handle_reset_sequence(pMsg);
        }
    }
}


CordioHCIDriver& ble_cordio_get_hci_driver() {
    static IPCPipeTransportDriver transport_driver;

    static Psoc6HCIDriver hci_driver(
        transport_driver /* other hci driver parameters */
    );

    return hci_driver;
}


