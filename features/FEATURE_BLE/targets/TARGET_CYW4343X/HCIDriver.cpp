/*
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

#include <stdio.h>
#include "CordioBLE.h"
#include "CordioHCIDriver.h"
#include "hci_api.h"
#include "hci_cmd.h"
#include "hci_core.h"
#include "bstream.h"
#include <stdbool.h>
#include "hci_mbed_os_adaptation.h"
#include "H4TransportDriver.h"

extern const int brcm_patch_ram_length;
extern const uint8_t brcm_patchram_buf[];

static const uint8_t pre_brcm_patchram_buf[] = {
    // RESET followed by download mini driver cmd
    0x03, 0x0C, 0x00,
    0x2E, 0xFC, 0x00,
};
static const int pre_brcm_patch_ram_length = sizeof(pre_brcm_patchram_buf);

#define HCI_RESET_RAND_CNT        4
#define HCI_VS_CMD_SET_SLEEP_MODE 0xFC27


extern "C" uint32_t Set_GPIO_Clock(uint32_t port_idx);

// 0: push pull
// 1: open drain
static void output_mode(PinName pin, int mode)
{
    MBED_ASSERT(pin != (PinName)NC);
    uint32_t port_index = STM_PORT(pin);
    uint32_t pin_index  = STM_PIN(pin);

    // Enable GPIO clock
    uint32_t gpio_add = Set_GPIO_Clock(port_index);
    GPIO_TypeDef *gpio = (GPIO_TypeDef *)gpio_add;


    /* Output mode configuration*/
    gpio->OTYPER  &= ~((GPIO_OTYPER_OT_0) << ((uint16_t)pin_index)) ;
    gpio->OTYPER |= (uint16_t)(((uint16_t)mode) << ((uint16_t)pin_index));
}

namespace ble {
namespace vendor {
namespace wise1530 {

class HCIDriver : public cordio::CordioHCIDriver {
public:
    HCIDriver(
        cordio::CordioHCITransportDriver& transport_driver,
        PinName bt_host_wake_name,
        PinName bt_device_wake_name,
        PinName bt_power_name
    ) : cordio::CordioHCIDriver(transport_driver),
        bt_host_wake_name(bt_host_wake_name),
        bt_device_wake_name(bt_device_wake_name),
        bt_power_name(bt_power_name),
        bt_host_wake(bt_host_wake_name, PIN_INPUT, PullNone, 0),
        bt_device_wake(bt_device_wake_name, PIN_OUTPUT, PullDefault, 1),
        bt_power(bt_power_name, PIN_OUTPUT, PullUp, 0),
        service_pack_index(0),
        service_pack_ptr(0),
        service_pack_length(0),
        service_pack_next(),
        service_pack_transfered(false) {
    }

    virtual void do_initialize()
    {
        output_mode(bt_host_wake_name, 1);
        output_mode(bt_device_wake_name, 0);
        output_mode(bt_power_name, 1);

        wait_ms(500);

        bt_device_wake = 0;
        wait_ms(500);

        bt_power = 1;
        wait_ms(500);
    }

    virtual void do_terminate() { }

    virtual void start_reset_sequence()
    {
        prepare_service_pack_transfert();
    }

    virtual void handle_reset_sequence(uint8_t *pMsg)
    {
        uint16_t       opcode;
        static uint8_t randCnt;

        /* if event is a command complete event */
        if (*pMsg == HCI_CMD_CMPL_EVT) {
            /* parse parameters */
            pMsg += HCI_EVT_HDR_LEN;
            pMsg++;                   /* skip num packets */
            BSTREAM_TO_UINT16(opcode, pMsg);
            pMsg++;                   /* skip status */

            if (service_pack_transfered == false) {
                randCnt = 0;
                ack_service_pack_command(opcode, pMsg);
                return;
            }

            /* decode opcode */
            switch (opcode) {
                // Note: Reset is handled by ack_service_pack.
                case HCI_VS_CMD_SET_SLEEP_MODE:
                    HciWriteLeHostSupport();
                    break;

                case HCI_OPCODE_WRITE_LE_HOST_SUPPORT:
                    randCnt = 0;
                    /* send next command in sequence */
                    HciSetEventMaskCmd((uint8_t *) hciEventMask);
                    break;

                case HCI_OPCODE_SET_EVENT_MASK:
                    randCnt = 0;
                    /* send next command in sequence */
                    HciLeSetEventMaskCmd((uint8_t *) hciLeEventMask);
                    break;

                case HCI_OPCODE_LE_SET_EVENT_MASK:
                    /* send next command in sequence */
                    HciSetEventMaskPage2Cmd((uint8_t *) hciEventMaskPage2);
                    break;

                case HCI_OPCODE_SET_EVENT_MASK_PAGE2:
                    /* send next command in sequence */
                    HciReadBdAddrCmd();
                    break;

                case HCI_OPCODE_READ_BD_ADDR:
                    /* parse and store event parameters */
                    BdaCpy(hciCoreCb.bdAddr, pMsg);
                    HciLeReadBufSizeCmd();
                    break;

                case HCI_OPCODE_LE_READ_BUF_SIZE:
                    /* parse and store event parameters */
                    BSTREAM_TO_UINT16(hciCoreCb.bufSize, pMsg);
                    BSTREAM_TO_UINT8(hciCoreCb.numBufs, pMsg);

                    // FixMe: The number of ACL buffer returned by the chip is
                    // incorrect. If more than two ACL packets are present in
                    // the controller, it may block the controller.
                    // Important: The ACL overflow event is **not** reported
                    // by the controller.
                    hciCoreCb.numBufs = 2;

                    /* initialize ACL buffer accounting */
                    hciCoreCb.availBufs = hciCoreCb.numBufs;

                    /* send next command in sequence */
                    HciLeReadSupStatesCmd();
                    break;

                case HCI_OPCODE_LE_READ_SUP_STATES:
                    /* parse and store event parameters */
                    memcpy(hciCoreCb.leStates, pMsg, HCI_LE_STATES_LEN);

                    /* send next command in sequence */
                    HciLeReadWhiteListSizeCmd();
                    break;

                case HCI_OPCODE_LE_READ_WHITE_LIST_SIZE:
                    /* parse and store event parameters */
                    BSTREAM_TO_UINT8(hciCoreCb.whiteListSize, pMsg);

                    /* send next command in sequence */
                    HciLeReadLocalSupFeatCmd();
                    break;

                case HCI_OPCODE_LE_READ_LOCAL_SUP_FEAT:
                    /* parse and store event parameters */
                    BSTREAM_TO_UINT16(hciCoreCb.leSupFeat, pMsg);

                    /* send next command in sequence */
                    hciCoreReadResolvingListSize();
                    break;

                case HCI_OPCODE_LE_READ_RES_LIST_SIZE:
                    /* parse and store event parameters */
                    BSTREAM_TO_UINT8(hciCoreCb.resListSize, pMsg);

                    /* send next command in sequence */
                    hciCoreReadMaxDataLen();
                    break;

                case HCI_OPCODE_LE_READ_MAX_DATA_LEN: {
                    uint16_t maxTxOctets;
                    uint16_t maxTxTime;

                    BSTREAM_TO_UINT16(maxTxOctets, pMsg);
                    BSTREAM_TO_UINT16(maxTxTime, pMsg);

                    /* use Controller's maximum supported payload octets and packet duration times
                    * for transmission as Host's suggested values for maximum transmission number
                    * of payload octets and maximum packet transmission time for new connections.
                    */
                    HciLeWriteDefDataLen(maxTxOctets, maxTxTime);
                }   break;

                case HCI_OPCODE_LE_WRITE_DEF_DATA_LEN:
                    if (hciCoreCb.extResetSeq) {
                        /* send first extended command */
                        (*hciCoreCb.extResetSeq)(pMsg, opcode);
                    } else {
                        /* initialize extended parameters */
                        hciCoreCb.maxAdvDataLen = 0;
                        hciCoreCb.numSupAdvSets = 0;
                        hciCoreCb.perAdvListSize = 0;

                        /* send next command in sequence */
                        HciLeRandCmd();
                    }
                    break;

                case HCI_OPCODE_LE_READ_MAX_ADV_DATA_LEN:
                case HCI_OPCODE_LE_READ_NUM_SUP_ADV_SETS:
                case HCI_OPCODE_LE_READ_PER_ADV_LIST_SIZE:
                    if (hciCoreCb.extResetSeq) {
                        /* send next extended command in sequence */
                        (*hciCoreCb.extResetSeq)(pMsg, opcode);
                    }
                    break;

                case HCI_OPCODE_LE_RAND:
                    /* check if need to send second rand command */
                    if (randCnt < (HCI_RESET_RAND_CNT-1)) {
                        randCnt++;
                        HciLeRandCmd();
                    } else {
                        uint8_t addr[6] = { 0 };
                        memcpy(addr, pMsg, sizeof(addr));
                        DM_RAND_ADDR_SET(addr, DM_RAND_ADDR_STATIC);
                        // note: will invoke set rand address
                        cordio::BLE::deviceInstance().getGap().setAddress(
                            BLEProtocol::AddressType::RANDOM_STATIC,
                            addr
                        );
                    }
                    break;

                case HCI_OPCODE_LE_SET_RAND_ADDR:
                    /* send next command in sequence */
                    signal_reset_sequence_done();
                    break;

                default:
                    break;
            }
        }
    }

private:

    // send pre_brcm_patchram_buf
    void prepare_service_pack_transfert(void)
    {
        service_pack_ptr = pre_brcm_patchram_buf;
        service_pack_length = pre_brcm_patch_ram_length;
        service_pack_next = &HCIDriver::start_service_pack_transfert;
        service_pack_index = 0;
        service_pack_transfered = false;
        send_service_pack_command();
    }

    // Called once pre_brcm_patchram_buf has been transferred; send brcm_patchram_buf
    void start_service_pack_transfert(void)
    {
        service_pack_ptr = brcm_patchram_buf;
        service_pack_length = brcm_patch_ram_length;
        service_pack_next = &HCIDriver::terminate_service_pack_transfert;
        service_pack_index = 0;
        service_pack_transfered = false;
        send_service_pack_command();
    }

    // Called once post_brcm_patchram_buf has been transferred; start regular initialization.
    void terminate_service_pack_transfert(void)
    {
        service_pack_ptr = NULL;
        service_pack_length = 0;
        service_pack_next = NULL;
        service_pack_index = 0;
        service_pack_transfered = true;
        wait_ms(1000);
        set_sleep_mode();
    }

    void send_service_pack_command(void)
    {
        uint16_t cmd_len = service_pack_ptr[service_pack_index + 2];
        uint16_t cmd_opcode = (service_pack_ptr[service_pack_index + 1] << 8) | service_pack_ptr[service_pack_index + 0];
        uint8_t *pBuf = hciCmdAlloc(cmd_opcode, cmd_len);
        if (pBuf) {
            memcpy(pBuf + HCI_CMD_HDR_LEN, service_pack_ptr + service_pack_index + HCI_CMD_HDR_LEN, cmd_len);
            hciCmdSend(pBuf);
        } else {
        }
    }

    void ack_service_pack_command(uint16_t opcode, uint8_t* msg)
    {
        uint16_t cmd_opcode = (service_pack_ptr[service_pack_index + 1] << 8) | service_pack_ptr[service_pack_index + 0];

        if (cmd_opcode != opcode)  {
            // DO something in case of error

            while (true);

        }

        // update service pack index
        service_pack_index += (HCI_CMD_HDR_LEN + service_pack_ptr[service_pack_index + 2]);

        if (service_pack_index < service_pack_length) {
            send_service_pack_command();
        } else {
            (this->*service_pack_next)();
        }
    }

    void set_sleep_mode()
    {
            uint8_t *pBuf;
            if ((pBuf = hciCmdAlloc(HCI_VS_CMD_SET_SLEEP_MODE, 12)) != NULL)
            {
                  pBuf[HCI_CMD_HDR_LEN] = 0x00; // no sleep moode
                  pBuf[HCI_CMD_HDR_LEN + 1] = 0x00; // no idle threshold host (N/A)
                  pBuf[HCI_CMD_HDR_LEN + 2] = 0x00; // no idle threshold HC (N/A)
                  pBuf[HCI_CMD_HDR_LEN + 3] = 0x00; // BT WAKE
                  pBuf[HCI_CMD_HDR_LEN + 4] = 0x00; // HOST WAKE
                  pBuf[HCI_CMD_HDR_LEN + 5] = 0x00; // Sleep during SCO
                  pBuf[HCI_CMD_HDR_LEN + 6] = 0x00; // Combining sleep mode and SCM
                  pBuf[HCI_CMD_HDR_LEN + 7] = 0x00; // Tristate TX
                  pBuf[HCI_CMD_HDR_LEN + 8] = 0x00; // Active connection handling on suspend
                  pBuf[HCI_CMD_HDR_LEN + 9] = 0x00; // resume timeout
                  pBuf[HCI_CMD_HDR_LEN + 10] = 0x00; // break to host
                  pBuf[HCI_CMD_HDR_LEN + 10] = 0x00; // Pulsed host wake
                  hciCmdSend(pBuf);
            }
    }

    static const uint16_t HCI_OPCODE_WRITE_LE_HOST_SUPPORT = 0x0C6D;

    void HciWriteLeHostSupport()
    {
      uint8_t *pBuf;
      if ((pBuf = hciCmdAlloc(HCI_OPCODE_WRITE_LE_HOST_SUPPORT, 2)) != NULL)
      {
        pBuf[HCI_CMD_HDR_LEN] = 0x01;
        pBuf[HCI_CMD_HDR_LEN + 1] = 0x00;
        hciCmdSend(pBuf);
      }
    }

    void hciCoreReadResolvingListSize(void)
    {
        /* if LL Privacy is supported by Controller and included */
        if ((hciCoreCb.leSupFeat & HCI_LE_SUP_FEAT_PRIVACY) &&
            (hciLeSupFeatCfg & HCI_LE_SUP_FEAT_PRIVACY))
        {
            /* send next command in sequence */
            HciLeReadResolvingListSize();
        }
        else
        {
            hciCoreCb.resListSize = 0;

            /* send next command in sequence */
            hciCoreReadMaxDataLen();
        }
    }

    void hciCoreReadMaxDataLen(void)
    {
    /* if LE Data Packet Length Extensions is supported by Controller and included */
        if ((hciCoreCb.leSupFeat & HCI_LE_SUP_FEAT_DATA_LEN_EXT) &&
            (hciLeSupFeatCfg & HCI_LE_SUP_FEAT_DATA_LEN_EXT))
        {
            /* send next command in sequence */
            HciLeReadMaxDataLen();
        }
        else
        {
            /* send next command in sequence */
            HciLeRandCmd();
        }
    }

    PinName bt_host_wake_name;
    PinName bt_device_wake_name;
    PinName bt_power_name;
    DigitalInOut bt_host_wake;
    DigitalInOut bt_device_wake;
    DigitalInOut bt_power;
    size_t service_pack_index;
    const uint8_t* service_pack_ptr;
    int service_pack_length;
    void (HCIDriver::*service_pack_next)();
    bool service_pack_transfered;

};

} // namespace wise1530
} // namespace vendor
} // namespace ble

ble::vendor::cordio::CordioHCIDriver& ble_cordio_get_hci_driver() {
    static ble::vendor::cordio::H4TransportDriver transport_driver(
        /* TX */ PA_2, /* RX */ PA_3, /* cts */ PA_0, /* rts */ PA_1, 115200
    );
    static ble::vendor::wise1530::HCIDriver hci_driver(
        transport_driver, /* host wake */ PC_0, /* device wake */ PB_8, /* bt_power */ PC_6
    );
    return hci_driver;
}
