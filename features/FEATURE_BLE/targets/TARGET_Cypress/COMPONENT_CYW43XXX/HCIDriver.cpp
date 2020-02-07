/*
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

#include <stdio.h>
#include "CordioBLE.h"
#include "CordioHCIDriver.h"
#include "hci_api.h"
#include "hci_cmd.h"
#include "hci_core.h"
#include "bstream.h"
#include <stdbool.h>
#include "hci_mbed_os_adaptation.h"
#include "CyH4TransportDriver.h"

extern const int brcm_patch_ram_length;
extern const uint8_t brcm_patchram_buf[];

#ifndef BT_UART_NO_3M_SUPPORT
static const uint8_t pre_brcm_patchram_buf[] = {
    // RESET followed by update uart baudrate
    0x03, 0x0C, 0x00,
        0x18, 0xFC, 0x06, 0x00, 0x00, 0xC0, 0xC6, 0x2D, 0x00,   //update uart baudrate 3 mbp
};
#else /* BT_UART_NO_3M_SUPPORT */
static const uint8_t pre_brcm_patchram_buf[] = {
    // RESET cmd
    0x03, 0x0C, 0x00,
};
#endif /* BT_UART_NO_3M_SUPPORT */

static const uint8_t pre_brcm_patchram_buf2[] = {
        //download mini driver
    0x2E, 0xFC, 0x00,
};

static const uint8_t post_brcm_patchram_buf[] = {
    // RESET cmd
    0x03, 0x0C, 0x00,
};

static const int pre_brcm_patch_ram_length = sizeof(pre_brcm_patchram_buf);
static const int pre_brcm_patch_ram_length2 = sizeof(pre_brcm_patchram_buf2);
static const int post_brcm_patch_ram_length = sizeof(post_brcm_patchram_buf);

#define HCI_RESET_RAND_CNT        4
#define HCI_VS_CMD_UPDATE_UART_BAUD_RATE 0xFC18
#define HCI_VS_CMD_SET_SLEEP_MODE 0xFC27


extern "C" uint32_t Set_GPIO_Clock(uint32_t port_idx);

namespace ble {
namespace vendor {
namespace cypress {

class HCIDriver : public cordio::CordioHCIDriver {
public:
    HCIDriver(
        ble::vendor::cypress_ble::CyH4TransportDriver& transport_driver,
        PinName bt_power_name,
	bool ps_enabled,
	uint8_t host_wake_irq,
	uint8_t dev_wake_irq
    ) : cordio::CordioHCIDriver(transport_driver),
        bt_power_name(bt_power_name),
        bt_power(bt_power_name, PIN_OUTPUT, PullUp, 0),
	is_powersave_enabled(ps_enabled),
	host_wake_irq(host_wake_irq),
	dev_wake_irq(dev_wake_irq),
        service_pack_index(0),
        service_pack_ptr(0),
        service_pack_length(0),
        service_pack_next(),
        service_pack_transfered(false),
        cy_transport_driver(transport_driver) {
    }

    virtual cordio::buf_pool_desc_t get_buffer_pool_description()
    {
        // Use default buffer pool
        return cordio::CordioHCIDriver::get_default_buffer_pool_description();
    }

    virtual void do_initialize()
    {
        //Prevent PSoC6 to enter deep-sleep till BT initialization is complete
        sleep_manager_lock_deep_sleep();
        rtos::ThisThread::sleep_for(500);
        bt_power = 1;
        rtos::ThisThread::sleep_for(500);
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
                case HCI_VS_CMD_UPDATE_UART_BAUD_RATE:
#ifndef BT_UART_NO_3M_SUPPORT
                    cy_transport_driver.update_uart_baud_rate(DEF_BT_3M_BAUD_RATE);
#endif /* BT_UART_NO_3M_SUPPORT */
#ifdef CY_DEBUG
                    HciReadLocalVerInfoCmd();
#else
                    set_sleep_mode();
#endif
                    break;

#ifdef CY_DEBUG
                case HCI_OPCODE_READ_LOCAL_VER_INFO:
                    uint8_t hci_version;
                    uint8_t hci_revision;
                    uint8_t lmp_revision;
                    uint16_t manufacturer_name;

                    BSTREAM_TO_UINT8(hci_version, pMsg);
                    BSTREAM_TO_UINT8(hci_revision, pMsg);
                    BSTREAM_TO_UINT8(lmp_revision, pMsg);
                    BSTREAM_TO_UINT16(manufacturer_name, pMsg);

                    if(hci_revision == 0 || manufacturer_name == 0xF)
                    {
                        printf("bt firmware download failed, rom code is being used\n");
                    }
                    else
                    {
                        printf("bt firmware download success\n");
                    }

                    set_sleep_mode();
                    break;
#endif

                // Note: Reset is handled by ack_service_pack.
                case HCI_VS_CMD_SET_SLEEP_MODE:
                    HciWriteLeHostSupport();
                    sleep_manager_unlock_deep_sleep();
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
                        set_random_static_address(addr);
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

    // send pre_brcm_patchram_buf issue hci reset and update baud rate on 43012
    void prepare_service_pack_transfert(void)
    {
        service_pack_ptr = pre_brcm_patchram_buf;
        service_pack_length = pre_brcm_patch_ram_length;
        service_pack_next = &HCIDriver::prepare_service_pack_transfert2;
        service_pack_index = 0;
        service_pack_transfered = false;
        send_service_pack_command();
    }

    // Called one pre_brcm_patchram_buf has been transferred; send pre_brcm_patchram_buf2 update uart baudrate
    // on PSoC6 to send hci download minidriver
    void prepare_service_pack_transfert2(void)
    {
#ifndef BT_UART_NO_3M_SUPPORT
        cy_transport_driver.update_uart_baud_rate(DEF_BT_3M_BAUD_RATE);
#endif /* BT_UART_NO_3M_SUPPORT */
        service_pack_ptr = pre_brcm_patchram_buf2;
        service_pack_length = pre_brcm_patch_ram_length2;
        service_pack_next = &HCIDriver::start_service_pack_transfert;
        service_pack_index = 0;
        service_pack_transfered = false;
        send_service_pack_command();
    }

    // Called once pre_brcm_patchram_buf2 has been transferred; send brcm_patchram_buf
    void start_service_pack_transfert(void)
    {
        service_pack_ptr = brcm_patchram_buf;
        service_pack_length = brcm_patch_ram_length;
        service_pack_next = &HCIDriver::post_service_pack_transfert;
        service_pack_index = 0;
        service_pack_transfered = false;
        send_service_pack_command();
    }

    // Called once brcm_patchram_buf has been transferred; send post_brcm_patchram_buf
    void post_service_pack_transfert(void)
    {
        cy_transport_driver.update_uart_baud_rate(DEF_BT_BAUD_RATE);
        service_pack_ptr = post_brcm_patchram_buf;
        service_pack_length = post_brcm_patch_ram_length;
        service_pack_next = &HCIDriver::terminate_service_pack_transfert;;
        service_pack_index = 0;
        service_pack_transfered = false;
        rtos::ThisThread::sleep_for(1000);
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
#ifndef BT_UART_NO_3M_SUPPORT
        HciUpdateUartBaudRate();
#else /* BT_UART_NO_3M_SUPPORT */
        set_sleep_mode();
#endif /* BT_UART_NO_3M_SUPPORT */
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

        if (service_pack_index < (size_t)service_pack_length) {
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
                  if (is_powersave_on()) {
                     pBuf[HCI_CMD_HDR_LEN] = 0x01; // sleep
		  } else {
                     pBuf[HCI_CMD_HDR_LEN] = 0x00; // no sleep
		  }
                  pBuf[HCI_CMD_HDR_LEN + 1] = 0x00; // no idle threshold host (N/A)
                  if (is_powersave_on()) {
                     pBuf[HCI_CMD_HDR_LEN + 2] = 0x05; // no idle threshold HC (N/A)
		  } else {
                     pBuf[HCI_CMD_HDR_LEN + 2] = 0x00; // no idle threshold HC (N/A)
                  }
                  if (is_powersave_on()) {
                     pBuf[HCI_CMD_HDR_LEN + 3] = dev_wake_irq; // BT WAKE
		  } else {
                     pBuf[HCI_CMD_HDR_LEN + 3] = 0x00; // BT WAKE
		  }
                  if (is_powersave_on()) {
                      pBuf[HCI_CMD_HDR_LEN + 4] = host_wake_irq; // HOST WAKE
		  } else {
                     pBuf[HCI_CMD_HDR_LEN + 4] = 0x00; // HOST WAKE
		  }
                  pBuf[HCI_CMD_HDR_LEN + 5] = 0x00; // Sleep during SCO
                  pBuf[HCI_CMD_HDR_LEN + 6] = 0x00; // Combining sleep mode and SCM
                  pBuf[HCI_CMD_HDR_LEN + 7] = 0x00; // Tristate TX
                  pBuf[HCI_CMD_HDR_LEN + 8] = 0x00; // Active connection handling on suspend
                  pBuf[HCI_CMD_HDR_LEN + 9] = 0x00; // resume timeout
                  pBuf[HCI_CMD_HDR_LEN + 10] = 0x00; // break to host
                  pBuf[HCI_CMD_HDR_LEN + 11] = 0x00; // Pulsed host wake
                  hciCmdSend(pBuf);
            }
    }

    // 0x18, 0xFC, 0x06, 0x00, 0x00, 0xC0, 0xC6, 0x2D, 0x00,   //update uart baudrate 3 mbp
    void HciUpdateUartBaudRate()
    {
            uint8_t *pBuf;
            if ((pBuf = hciCmdAlloc(HCI_VS_CMD_UPDATE_UART_BAUD_RATE, 6)) != NULL)
            {
                  pBuf[HCI_CMD_HDR_LEN] = 0x00; // encoded_baud_rate
                  pBuf[HCI_CMD_HDR_LEN + 1] = 0x00; // use_encoded_form
                  pBuf[HCI_CMD_HDR_LEN + 2] = 0xC0; // explicit baud rate bit 0-7
                  pBuf[HCI_CMD_HDR_LEN + 3] = 0xC6; // explicit baud rate bit 8-15
                  pBuf[HCI_CMD_HDR_LEN + 4] = 0x2D; // explicit baud rate bit 16-23
                  pBuf[HCI_CMD_HDR_LEN + 5] = 0x00; // explicit baud rate bit 24-31
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

    bool is_powersave_on(void)
    {
       return (is_powersave_enabled);
    }

    PinName bt_power_name;
    DigitalInOut bt_power;

    bool is_powersave_enabled;
    uint8_t host_wake_irq;
    uint8_t dev_wake_irq;

    size_t service_pack_index;
    const uint8_t* service_pack_ptr;
    int service_pack_length;
    void (HCIDriver::*service_pack_next)();
    bool service_pack_transfered;
    ble::vendor::cypress_ble::CyH4TransportDriver& cy_transport_driver;

};

} // namespace cypress
} // namespace vendor
} // namespace ble

ble::vendor::cordio::CordioHCIDriver& ble_cordio_get_hci_driver()
{
    static ble::vendor::cypress_ble::CyH4TransportDriver& transport_driver =
          ble_cordio_get_h4_transport_driver();
    static ble::vendor::cypress::HCIDriver hci_driver(
        transport_driver,
        /* bt_power */ CYBSP_BT_POWER,
	transport_driver.get_enabled_powersave(),
	transport_driver.get_host_wake_irq_event(),
	transport_driver.get_dev_wake_irq_event()
    );
    return hci_driver;
}
