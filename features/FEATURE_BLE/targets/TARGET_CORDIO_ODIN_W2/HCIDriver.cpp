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

#include "CordioBLE.h"
#include "CordioHCIDriver.h"
#include "hci_defs.h"
#include "hci_api.h"
#include "hci_cmd.h"
#include "hci_core.h"
#include "bstream.h"
#include "wsf_buf.h"
#include <stdbool.h>
#include "hci_mbed_os_adaptation.h"
#include "H4TransportDriver.h"
#include "OdinCordioInterface.h"

namespace ble {
namespace vendor {
namespace odin_w2 {

class HCIDriver : public cordio::CordioHCIDriver {

    public:
        HCIDriver(cordio::CordioHCITransportDriver& transport_driver, PinName shutdown_name, PinName hci_rts_name) :
            cordio::CordioHCIDriver(transport_driver),
            shutdown(shutdown_name, 0),
            hci_rts(hci_rts_name, 0),
            service_pack_index(0),
            service_pack_transfered(false) {
        };

        virtual void do_initialize();

        virtual void do_terminate();

        virtual void start_reset_sequence();

        virtual void handle_reset_sequence(uint8_t *pMsg);

    private:
        void start_service_pack_transfert(void) {
            service_pack_index = 0;
            service_pack_transfered = false;
            send_service_pack_command();
        }

        void send_service_pack_command(void) {
            uint16_t cmd_len = OdinServicePack[service_pack_index + HCI_CMD_HDR_LEN];
            cmd_opcode_ack_expected = (OdinServicePack[service_pack_index + 2] << 8) | OdinServicePack[service_pack_index + 1];
            uint8_t *pBuf = hciCmdAlloc(cmd_opcode_ack_expected, cmd_len);
            if (pBuf) {
                memcpy(pBuf, OdinServicePack + service_pack_index + 1, cmd_len + HCI_CMD_HDR_LEN);
                hciCmdSend(pBuf);
            }
            else {
                printf("Error cannot allocate memory for the buffer");
            }
        }

        void ack_service_pack_command(uint16_t opcode, uint8_t* msg) {
            /* check if response opcode is same as expected command opcode */
            MBED_ASSERT (cmd_opcode_ack_expected == opcode); 

            // update service pack index
            service_pack_index += (1 + HCI_CMD_HDR_LEN + OdinServicePack[service_pack_index + HCI_CMD_HDR_LEN]);

            if (service_pack_index < service_pack_size) {
                send_service_pack_command();
            }
            else if (opcode == HCID_VS_WRITE_BD_ADDR) {
               /* send an HCI Reset command to start the sequence */
                HciResetCmd();
                service_pack_transfered = true;
            }
            else {
                /* send BT device hardware address write command */
                send_hci_vs_cmd(HCID_VS_WRITE_BD_ADDR);
                cmd_opcode_ack_expected = HCID_VS_WRITE_BD_ADDR;
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

        DigitalOut      shutdown;                       // power/shutdown pin for bt device
        DigitalOut      hci_rts;                        // request to sent pin
        size_t          service_pack_index;             // Index of command to be recently sent over hci
        bool            service_pack_transfered;        // Flag to notify if service pack is completely transferred or not
        uint16_t        cmd_opcode_ack_expected;        // Command against which acknowledgment is expected
        uint32_t        service_pack_size;              // size of service pack
        char*           OdinServicePack ;
        vs_cmd_send_t   send_hci_vs_cmd ;

};

} // namespace odin_w2
} // namespace vendor
} // namespace ble

void ble::vendor::odin_w2::HCIDriver::do_initialize()
{
    cordio_callback_s    callback;

    hci_rts = 1;            // Flow Control is OFF

    shutdown = 0;           // BT Power is OFF
    wait_ms(20);
    shutdown = 1;           // BT Power is ON
    wait_ms(500);

    hci_rts = 0;            // Flow Control is ON

    /* ODIN ble driver initialization function  */
    cbCordio_Btinit(&callback);

    OdinServicePack = callback.Service_pack;
    send_hci_vs_cmd = callback.vs_command_callback;
    service_pack_size = callback.service_pack_size;
}

void ble::vendor::odin_w2::HCIDriver::do_terminate()
{
    // TODO: ASRI
}

void ble::vendor::odin_w2::HCIDriver::start_reset_sequence()
{
    /* Update baudrate of BT to speed up setup time */
    send_hci_vs_cmd(HCID_VS_UPDATE_UART_BAUD_RATE);

}

void ble::vendor::odin_w2::HCIDriver::handle_reset_sequence(uint8_t *pMsg)
{
    uint16_t       opcode;
    static uint8_t randCnt;

    /* if event is a command complete event */
    if (*pMsg == HCI_CMD_CMPL_EVT)
    {
        /* parse parameters */
        pMsg += HCI_EVT_HDR_LEN;
        pMsg++;                   /* skip num packets */
        BSTREAM_TO_UINT16(opcode, pMsg);
        pMsg++;                   /* skip status */

        if (opcode == HCID_VS_UPDATE_UART_BAUD_RATE)
        {
            update_uart_baud_rate();
            start_service_pack_transfert();
            return;
        }

        if (service_pack_transfered == false) {
            ack_service_pack_command(opcode, pMsg);
            return;
        }

        /* decode opcode */
        switch (opcode) {

            case HCI_OPCODE_RESET:
                /* Send (fast and slow) clock configuration command */
                send_hci_vs_cmd(HCID_VS_FAST_CLOCK_CONFIG_BTIP);
                break;

            case HCID_VS_FAST_CLOCK_CONFIG_BTIP:
                /* Send deep-sleep behavior control command (setting retransmission, inactivity and rts pulse width for Bt) */
                send_hci_vs_cmd(HCID_VS_HCILL_PARS_CFG);
                break;

            case HCID_VS_HCILL_PARS_CFG:
                /* Send sleep mode configuration command */
                send_hci_vs_cmd(HCID_VS_SLEEP_PROTOCOLS_CFG);
                break;

            case HCID_VS_SLEEP_PROTOCOLS_CFG:
                /* initialize rand command count */
                randCnt = 0;

                /* send next command in sequence */
                HciSetEventMaskCmd((uint8_t *)hciEventMask);
                break;

            case HCI_OPCODE_SET_EVENT_MASK:
                /* send next command in sequence */
                HciLeSetEventMaskCmd((uint8_t *)hciLeEventMask);
                break;

            case HCI_OPCODE_LE_SET_EVENT_MASK:
                /* send next command in sequence */
                HciSetEventMaskPage2Cmd((uint8_t *)hciEventMaskPage2);
                break;

            case HCI_OPCODE_SET_EVENT_MASK_PAGE2:
                /* send next command in sequence */
                HciReadBdAddrCmd();
                break;

            case HCI_OPCODE_READ_BD_ADDR:
                /* parse and store event parameters */
                BdaCpy(hciCoreCb.bdAddr, pMsg);

                /* send next command in sequence */
                HciLeReadBufSizeCmd();
                break;

            case HCI_OPCODE_LE_READ_BUF_SIZE:
                /* parse and store event parameters */
                BSTREAM_TO_UINT16(hciCoreCb.bufSize, pMsg);
                BSTREAM_TO_UINT8(hciCoreCb.numBufs, pMsg);

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

            case HCI_OPCODE_LE_READ_MAX_DATA_LEN:
            {
                uint16_t maxTxOctets;
                uint16_t maxTxTime;

                BSTREAM_TO_UINT16(maxTxOctets, pMsg);
                BSTREAM_TO_UINT16(maxTxTime, pMsg);

               /* use Controller's maximum supported payload octets and packet duration times
                * for transmission as Host's suggested values for maximum transmission number
                * of payload octets and maximum packet transmission time for new connections.
                */
                HciLeWriteDefDataLen(maxTxOctets, maxTxTime);
            }
            break;

            case HCI_OPCODE_LE_WRITE_DEF_DATA_LEN:
                if (hciCoreCb.extResetSeq)
                {
                    /* send first extended command */
                    (*hciCoreCb.extResetSeq)(pMsg, opcode);
                }
                else
                {
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
                if (hciCoreCb.extResetSeq)
                {
                    /* send next extended command in sequence */
                    (*hciCoreCb.extResetSeq)(pMsg, opcode);
                }
                break;

            case HCI_OPCODE_LE_RAND:
                /* check if need to send second rand command */
                if (randCnt < (HCI_RESET_RAND_CNT - 1))
                {
                    randCnt++;
                    HciLeRandCmd();
                }
                else
                {
                    signal_reset_sequence_done();
                }
                break;

            default:
                break;
        }
    }
}

ble::vendor::cordio::CordioHCIDriver& ble_cordio_get_hci_driver() {
    static ble::vendor::cordio::H4TransportDriver transport_driver (/* cbCFG_PIO_PIN_BT_HCI_TX */ PG_14, 
                                                                    /* cbCFG_PIO_PIN_BT_HCI_RX */ PC_7, 
                                                                    /* cbCFG_PIO_PIN_BT_HCI_CTS */ PG_15, 
                                                                    /* cbCFG_PIO_PIN_BT_HCI_RTS */ PG_12, 
                                                                    115200);
    static ble::vendor::odin_w2::HCIDriver hci_driver ( transport_driver, 
                                                        /* cbCFG_PIO_PIN_BT_ENABLE  */ PG_7, 
                                                        /* cbCFG_PIO_PIN_BT_HCI_RTS */ PG_12);

    return hci_driver;
}
