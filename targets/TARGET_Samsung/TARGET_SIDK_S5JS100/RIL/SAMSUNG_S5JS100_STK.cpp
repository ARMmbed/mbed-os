/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#include "mbed.h"
#include "SAMSUNG_S5JS100_Modem_Proxy.h"
#include "SAMSUNG_S5JS100_STK.h"

#include "mbed_trace.h"
#define TRACE_GROUP "STK"

#ifndef SAMSUNG_S5JS100_STK_DBG_ON
#define SAMSUNG_S5JS100_STK_DBG_ON        1
#endif
#define SAMSUNG_S5JS100_STK_DBG        if (SAMSUNG_S5JS100_STK_DBG_ON) tr_info


namespace mbed {
extern SAMSUNG_S5JS100_MODEM_PROXY *gModemProxy;

SAMSUNG_S5JS100_STK *SAMSUNG_S5JS100_STK::GetInstance()
{
    static SAMSUNG_S5JS100_STK *s5js100_stkInstance;

    if (s5js100_stkInstance == NULL) {
        s5js100_stkInstance = new SAMSUNG_S5JS100_STK();
    }
    return s5js100_stkInstance;
}

SAMSUNG_S5JS100_STK::SAMSUNG_S5JS100_STK()
{
    rsp_cb = NULL;
    setup_event_list = NULL;
    setup_event_list_size = 0;
    usat_thread = NULL;
    usat_thread_run = 0;
}

SAMSUNG_S5JS100_STK::~SAMSUNG_S5JS100_STK()
{
    if (gModemProxy != NULL)
        gModemProxy->registerSTK(NULL);
}

int SAMSUNG_S5JS100_STK::RegisterToModemProxy(void)
{
    if (gModemProxy == NULL)
        return -1;
    gModemProxy->registerSTK(this);
    return 0;
}

void SAMSUNG_S5JS100_STK::OnRequest(STK_msg request, void *data, unsigned int datalen)
{
    if (RegisterToModemProxy() < 0) {
        OnRequestComplete(request, STK_E_GENERIC_FAILURE, NULL, 0);
        return;
    }

    switch (request) {
    case SEND_STK_TERMINAL_RSP:
        gModemProxy->sendStkTerminalRsp(data, datalen);
        break;
    case SEND_STK_ENVELOPE_CMD:
        gModemProxy->sendStkEnvelopeCmd(data, datalen);
        break;
    default:
        SAMSUNG_S5JS100_STK_DBG("SAMSUNG_S5JS100_STK::OnRequest Invalid Request (%d)", request);
        break;
    }
}

void SAMSUNG_S5JS100_STK::OnRequestComplete(STK_msg request, STK_Errno e, void *response, unsigned int responselen)
{
    //return ParseMsg(request, e, response, responselen);
    return;
}

void SAMSUNG_S5JS100_STK::OnUnsolicitedResponse(STK_msg unsolResponse, void *data, unsigned int datalen)
{
    return ParseMsg(unsolResponse, STK_E_SUCCESS, data, datalen);
}

void SAMSUNG_S5JS100_STK::ParseMsg(int stkmsg, STK_Errno e, void *msg, unsigned int msglen)
{
    uicc_proactive_cmd_hrd *cmd_hdr;
    UICC_COMMAND_TYPE cmd_type;
    unsigned int i;
#if SAMSUNG_S5JS100_STK_DBG_ON
    char *tmsg;
    char mm[256] = "";
#endif

    if (e != STK_E_SUCCESS)
        return;

#if SAMSUNG_S5JS100_STK_DBG_ON
    SAMSUNG_S5JS100_STK_DBG("SAMSUNG_S5JS100_STK::ParseMsg msglen[%d]", msglen);
    tmsg = (char *)msg;
    SAMSUNG_S5JS100_STK_DBG("\n");
    for (i = 0; i < msglen; i++) {
        sprintf(mm + (i % 16) * 3, "%02X ", tmsg[i]);
        //MODEM_LINK_DEVICE_SHMEM_DBG_ON("%02X ", msg->data[i]);
        if (i % 16 == 15 && i != 0)
            SAMSUNG_S5JS100_STK_DBG("%s", mm);
    }
    if ((i - 1) % 16 != 15)
        SAMSUNG_S5JS100_STK_DBG("%s", mm);
#endif

    cmd_hdr = (uicc_proactive_cmd_hrd *)msg;
    if (cmd_hdr->command_tag != BER_TLV_PROACTIVE_COMMAND) {
        tr_error("[ParseMsg] Not BER_TLV_PROACTIVE_COMMAND");
        return;
    }

    /* Parse message */
    if (cmd_hdr->command_details.tag == BER_COMP_command_detail_tag) {
        cmd_type = (UICC_COMMAND_TYPE)(cmd_hdr->command_details.type);
        switch (cmd_type) {
        case UICC_COMMAND_REFRESH:
            proactive_refresh(msg, msglen);
            break;
        case UICC_COMMAND_SETUP_EVENT_LIST:
            proactive_setup_event_list(msg, msglen);
            break;
        case UICC_COMMAND_OPEN_CHANNEL:
            proactive_open_channel(msg, msglen);
            break;
        case UICC_COMMAND_CLOSE_CHANNEL:
            proactive_close_channel(msg, msglen);
            break;
        case UICC_COMMAND_RECEIVE_DATA:
            proactive_receive_data(msg, msglen);
            break;
        case UICC_COMMAND_SEND_DATA:
            proactive_send_data(msg, msglen);
            break;
        case UICC_COMMAND_GET_CHANNEL_STATUS:
            proactive_get_channel_status(msg, msglen);
            break;
        default:
            tr_error("[ParseMsg] Not supported command (0x%02X)", cmd_type);
            break;
        }
    }

    if (rsp_cb != NULL)
        return rsp_cb(msg, msglen);
}

static int check_device_identy(uicc_device_identities *identy)
{
    if (identy->tag != 0x82 || identy->length != 0x02 ||
        identy->source_device != 0x81 || identy->destination_device != 0x82) {
        return -1;
    }

    return 0;
}

int SAMSUNG_S5JS100_STK::proactive_refresh(void *msg, unsigned int msglen)
{
    SAMSUNG_S5JS100_STK_DBG("%s", __func__);

    return 0;
}

int SAMSUNG_S5JS100_STK::proactive_setup_event_list(void *msg, unsigned int msglen)
{
    uicc_setup_event_list m;
    unsigned char *b = (unsigned char *)msg;
    unsigned int offset = 0;
    int i;

    m.hrd = (uicc_proactive_cmd_hrd *)b;
    offset += sizeof(uicc_proactive_cmd_hrd);
    m.device_identy = (uicc_device_identities *)(b + offset);
    offset += m.device_identy->length + 2;
    m.event_list = (uicc_event_list *)(b + offset);

    if (check_device_identy(m.device_identy)) {
        tr_error("%s device_identy fail", __func__);
        return -1;
    }

    if (m.event_list->tag != 0x99) {
        tr_error("%s event_list tag fail", __func__);
        return -1;
    }

    if (setup_event_list != NULL) {
        free(setup_event_list);
        setup_event_list_size = 0;
    }

    setup_event_list = (uicc_event_list_event *)malloc(m.event_list->length);
    setup_event_list_size = m.event_list->length;
    for (i = 0; i < m.event_list->length; i++)
        setup_event_list[i] = (uicc_event_list_event)(m.event_list->events[i]);


    SAMSUNG_S5JS100_STK_DBG("%s", __func__);

    SAMSUNG_S5JS100_STK::GetInstance()->OnRequest(SEND_STK_TERMINAL_RSP, msg, msglen);

    return 0;
}

void SAMSUNG_S5JS100_STK::usat_task(void)
{
    nsapi_size_or_error_t retcode;
    SocketAddress sock_addr;
    int tcpudpmode = 0;        //tcp : 0, udp : 1
    SAMSUNG_S5JS100_STK *stk = GetInstance();

    switch (stk->connection.protocol_type) {
    case 0x01:
    case 0x03:
    case 0x05:
        tcpudpmode = 0;
        break;
    case 0x02:
    case 0x04:
        tcpudpmode = 1;
        break;
    default:
        tr_error("SAMSUNG_S5JS100_STK::usat_task Undefined protocol type (0x%X)\n", stk->connection.protocol_type);
        return;
    }

    stk->connection.NetInterface = NetworkInterface::get_default_instance();

    sock_addr.set_port(stk->connection.port);
    sock_addr.set_ip_address((char *)(stk->connection.dest_address));

    if (tcpudpmode) {
        retcode = stk->connection.udp.open(stk->connection.NetInterface);
        if (retcode != NSAPI_ERROR_OK) {
            tr_error("UDPSocket.open() fails, code: %d\n", retcode);
            return;
        }
        retcode = stk->connection.udp.connect(sock_addr);
        if (retcode < 0) {
            tr_error("UDPSocket.connect() fails, code: %d\n", retcode);
            return;
        }
        stk->connection.udp.set_timeout(15000);
    } else {
        retcode = stk->connection.tcp.open(stk->connection.NetInterface);
        if (retcode != NSAPI_ERROR_OK) {
            tr_error("TCPSocket.open() fails, code: %d\n", retcode);
            return;
        }
        retcode = stk->connection.tcp.connect(sock_addr);
        if (retcode < 0) {
            tr_error("TCPSocket.connect() fails, code: %d\n", retcode);
            return;
        }
        stk->connection.tcp.set_timeout(15000);
    }

    while (stk->usat_thread_run) {
        if (tcpudpmode)
            stk->connection.udp.recv(stk->connection.recv_buf, sizeof(stk->connection.recv_buf));
        else
            stk->connection.tcp.recv(stk->connection.recv_buf, sizeof(stk->connection.recv_buf));

        stk->envelope_data_available(0, 0, (unsigned char)(sizeof(stk->connection.recv_buf)));
    }

    stk->connection.tcp.close();
}

int SAMSUNG_S5JS100_STK::proactive_open_channel(void *msg, unsigned int msglen)
{
    uicc_open_channel_default m;
    unsigned char *b = (unsigned char *)msg;
    unsigned int offset = 0;

    m.hrd = (uicc_proactive_cmd_hrd *)b;
    offset += sizeof(uicc_proactive_cmd_hrd);
    m.device_identy = (uicc_device_identities *)(b + offset);
    offset += m.device_identy->length + 2;
    m.alpha_identifier = (uicc_alpha_identifier *)(b + offset);
    offset += m.alpha_identifier->length + 2;
    m.icon_identifier = (uicc_icon_identifier *)(b + offset);
    offset += m.icon_identifier->length + 2;
    m.bearer_description = (uicc_bearer_description *)(b + offset);
    offset += m.bearer_description->length + 2;
    m.buffer_size = (uicc_buffer_size *)(b + offset);
    offset += m.buffer_size->length + 2;
    m.local_address = (uicc_other_address *)(b + offset);
    offset += m.local_address->length + 2;
    m.user_login = (uicc_text_string *)(b + offset);
    offset += m.user_login->length + 2;
    m.user_password = (uicc_text_string *)(b + offset);
    offset += m.user_password->length + 2;
    m.uicc_terminal_interface_transport_level = (uicc_uicc_terminal_interface_transport_level *)(b + offset);
    offset += m.uicc_terminal_interface_transport_level->length + 2;
    m.data_destination_address = (uicc_other_address *)(b + offset);
    offset += m.data_destination_address->length + 2;
    m.text_attribute = (uicc_text_attribute *)(b + offset);
    offset += m.text_attribute->length + 2;
    m.frame_identifier = (uicc_frame_identifier *)(b + offset);

    if (check_device_identy(m.device_identy)) {
        tr_error("%s device_identy fail", __func__);
        return -1;
    }

    SAMSUNG_S5JS100_STK_DBG("[%s]", __func__);
    SAMSUNG_S5JS100_STK_DBG("Bearer type   : 0x%X", m.bearer_description->bearer_type);
    SAMSUNG_S5JS100_STK_DBG("Buffer size   : 0x%X", m.buffer_size->buffer_size);
    SAMSUNG_S5JS100_STK_DBG("Local address : %s", m.local_address->address);
    SAMSUNG_S5JS100_STK_DBG("User login    : %s", (m.user_login->length == 0) ? "(null)" : (char *)(m.user_login->text_string));
    SAMSUNG_S5JS100_STK_DBG("User password : %s", (m.user_password->length == 0) ? "(null)" : (char *)(m.user_password->text_string));
    SAMSUNG_S5JS100_STK_DBG("protocol type : 0x%X", m.uicc_terminal_interface_transport_level->transport_protocol_type);
    SAMSUNG_S5JS100_STK_DBG("port number   : %d", m.uicc_terminal_interface_transport_level->port_number);
    SAMSUNG_S5JS100_STK_DBG("Dest  address : %s", m.data_destination_address->address);

    connection.protocol_type = m.uicc_terminal_interface_transport_level->transport_protocol_type;
    connection.port = m.uicc_terminal_interface_transport_level->port_number;
    memcpy(connection.local_address, m.local_address->address, 24);
    memcpy(connection.dest_address, m.data_destination_address->address, 24);
    memcpy(connection.login, m.user_login, 24);
    memcpy(connection.password, m.user_password, 24);
    connection.received_size = 0;

    usat_thread_run = 1;
    usat_thread = new rtos::Thread(osPriorityNormal, 2048, NULL, "usat_task");
    usat_thread->start(this->usat_task);

    SAMSUNG_S5JS100_STK::GetInstance()->OnRequest(SEND_STK_TERMINAL_RSP, msg, msglen);

    return 0;
}

int SAMSUNG_S5JS100_STK::proactive_close_channel(void *msg, unsigned int msglen)
{
    uicc_close_channel m;
    unsigned char *b = (unsigned char *)msg;
    unsigned int offset = 0;

    m.hrd = (uicc_proactive_cmd_hrd *)b;
    offset += sizeof(uicc_proactive_cmd_hrd);
    m.device_identy = (uicc_device_identities *)(b + offset);
    offset += m.device_identy->length + 2;
    m.alpha_identifier = (uicc_alpha_identifier *)(b + offset);
    offset += m.alpha_identifier->length + 2;
    m.icon_identifier = (uicc_icon_identifier *)(b + offset);
    offset += m.icon_identifier->length + 2;
    m.text_attribute = (uicc_text_attribute *)(b + offset);
    offset += m.text_attribute->length + 2;
    m.frame_identifier = (uicc_frame_identifier *)(b + offset);

    if (check_device_identy(m.device_identy)) {
        tr_error("%s device_identy fail", __func__);
        return -1;
    }

    SAMSUNG_S5JS100_STK_DBG("[%s]", __func__);

    SAMSUNG_S5JS100_STK::GetInstance()->OnRequest(SEND_STK_TERMINAL_RSP, msg, msglen);

    return 0;
}

int SAMSUNG_S5JS100_STK::proactive_receive_data(void *msg, unsigned int msglen)
{
    uicc_receive_data m;
    unsigned char *b = (unsigned char *)msg;
    unsigned int offset = 0;

    m.hrd = (uicc_proactive_cmd_hrd *)b;
    offset += sizeof(uicc_proactive_cmd_hrd);
    m.device_identy = (uicc_device_identities *)(b + offset);
    offset += m.device_identy->length + 2;
    m.alpha_identifier = (uicc_alpha_identifier *)(b + offset);
    offset += m.alpha_identifier->length + 2;
    m.icon_identifier = (uicc_icon_identifier *)(b + offset);
    offset += m.icon_identifier->length + 2;
    m.channel_data_length = (uicc_channel_data_length *)(b + offset);
    offset += m.channel_data_length->length + 2;
    m.text_attribute = (uicc_text_attribute *)(b + offset);
    offset += m.text_attribute->length + 2;
    m.frame_identifier = (uicc_frame_identifier *)(b + offset);

    if (check_device_identy(m.device_identy)) {
        tr_error("%s device_identy fail", __func__);
        return -1;
    }

    SAMSUNG_S5JS100_STK_DBG("[%s]", __func__);

    SAMSUNG_S5JS100_STK::GetInstance()->OnRequest(SEND_STK_TERMINAL_RSP, msg, msglen);

    return 0;
}

int SAMSUNG_S5JS100_STK::proactive_send_data(void *msg, unsigned int msglen)
{
    uicc_send_data m;
    unsigned char *b = (unsigned char *)msg;
    unsigned int offset = 0;

    m.hrd = (uicc_proactive_cmd_hrd *)b;
    offset += sizeof(uicc_proactive_cmd_hrd);
    m.device_identy = (uicc_device_identities *)(b + offset);
    offset += m.device_identy->length + 2;
    m.alpha_identifier = (uicc_alpha_identifier *)(b + offset);
    offset += m.alpha_identifier->length + 2;
    m.icon_identifier = (uicc_icon_identifier *)(b + offset);
    offset += m.icon_identifier->length + 2;
    m.channel_data = (uicc_channel_data *)(b + offset);
    offset += m.channel_data->length + 2;
    m.text_attribute = (uicc_text_attribute *)(b + offset);
    offset += m.text_attribute->length + 2;
    m.frame_identifier = (uicc_frame_identifier *)(b + offset);

    if (check_device_identy(m.device_identy)) {
        tr_error("%s device_identy fail", __func__);
        return -1;
    }

    SAMSUNG_S5JS100_STK_DBG("[%s]", __func__);

    SAMSUNG_S5JS100_STK::GetInstance()->OnRequest(SEND_STK_TERMINAL_RSP, msg, msglen);

    return 0;
}

int SAMSUNG_S5JS100_STK::proactive_get_channel_status(void *msg, unsigned int msglen)
{
    uicc_get_channel_status m;
    unsigned char *b = (unsigned char *)msg;
    unsigned int offset = 0;

    m.hrd = (uicc_proactive_cmd_hrd *)b;
    offset += sizeof(uicc_proactive_cmd_hrd);
    m.device_identy = (uicc_device_identities *)(b + offset);

    if (check_device_identy(m.device_identy)) {
        tr_error("%s device_identy fail", __func__);
        return -1;
    }

    SAMSUNG_S5JS100_STK_DBG("[%s]", __func__);

    SAMSUNG_S5JS100_STK::GetInstance()->OnRequest(SEND_STK_TERMINAL_RSP, msg, msglen);

    return 0;
}

/* TS 102 223 V12 7.5.10.2 */
int SAMSUNG_S5JS100_STK::envelope_data_available(unsigned char channel_status0, unsigned char channel_status1, unsigned char receive_data_length)
{
    unsigned char total_length = 16;
    static unsigned char envelope_data_available_msg[16] = {
        BER_TLV_EVENT_DOWNLOAD, 14,
        BER_COMP_event_list_tag, 0x01, UICC_EVENT_Data_available,
        BER_COMP_device_identity_tag, 0x02, 0x82, 0x81,
        BER_COMP_channel_status_tag, 0x02, 0x00, 0x00,
        BER_COMP_channel_data_length_tag, 0x02, 0x01,
    };

    envelope_data_available_msg[15] = receive_data_length;

    SAMSUNG_S5JS100_STK::GetInstance()->OnRequest(SEND_STK_ENVELOPE_CMD, (void *)envelope_data_available_msg, total_length);

    return 0;
}

/* TS 102 223 V12 7.5.11.2 */
int SAMSUNG_S5JS100_STK::envelope_channel_status(unsigned char channel_status0, unsigned char channel_status1, unsigned char addr_type, unsigned char *ipaddr)
{
    unsigned char total_length = 35;
    static unsigned char envelope_channel_status_msg[48] = {
        BER_TLV_EVENT_DOWNLOAD, 33,
        BER_COMP_event_list_tag, 0x01, UICC_EVENT_Channel_status,
        BER_COMP_device_identity_tag, 0x02, 0x82, 0x81,
        BER_COMP_channel_status_tag, 0x02, 0x00, 0x00,
        BER_COMP_bearer_description_tag, 0x02, 0x03, 0x00,
        BER_COMP_other_address_tag, 0x10, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    envelope_channel_status_msg[1] = total_length - 2;

    envelope_channel_status_msg[11] = 0;    //channel status 0 (8.56)
    envelope_channel_status_msg[12] = 0;    //channel status 1 (8.56)

    envelope_channel_status_msg[19] = addr_type;    // type of address (0x21:IPV4, 0x57:IPV6)
    strncpy((char *)(envelope_channel_status_msg + 20), (char *)ipaddr, 15);

    SAMSUNG_S5JS100_STK::GetInstance()->OnRequest(SEND_STK_ENVELOPE_CMD, (void *)envelope_channel_status_msg, total_length);

    return 0;
}

}
