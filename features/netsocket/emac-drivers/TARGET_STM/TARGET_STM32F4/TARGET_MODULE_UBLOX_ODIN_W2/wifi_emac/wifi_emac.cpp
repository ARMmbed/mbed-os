/*---------------------------------------------------------------------------
 * Copyright (c) 2019, u-blox Malmö, All Rights Reserved
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
#if DEVICE_WIFI

#include "mbed_interface.h"
#include "mbed_assert.h"
#include "netsocket/nsapi_types.h"

#include "wifi_emac.h"

#include "cb_wlan_target_data.h"
#include "cb_wlan_types.h"
#include "cb_wlan.h"
#include "cb_otp.h"
#include "cb_main.h"

#define OdinWiFiEMAC_MTU_SIZE  (1500U)
static const char _ifname[] = "WL0";

cb_boolean handleWlanTargetCopyFromDataFrame(uint8_t* buffer, cbWLANTARGET_dataFrame* frame, uint32_t size, uint32_t offsetInFrame);
cb_boolean handleWlanTargetCopyToDataFrame(cbWLANTARGET_dataFrame* frame, uint8_t* buffer, uint32_t size, uint32_t offsetInFrame);
cbWLANTARGET_dataFrame* handleWlanTargetAllocDataFrame(uint32_t size);
void handleWlanTargetFreeDataFrame(cbWLANTARGET_dataFrame* frame);
cb_uint32 handleWlanTargetGetDataFrameSize(cbWLANTARGET_dataFrame* frame);
cb_uint8 handleWlanTargetGetDataFrameTID(cbWLANTARGET_dataFrame* frame);
void handleWlanStatusIndication(void *dummy, cbWLAN_StatusIndicationInfo status, void *data);
void handleWlanPacketIndication(void *dummy, cbWLAN_PacketIndicationInfo *packetInfo);
void send_wlan_packet(OdinWiFiEMAC *ptr, void *buf);

static const cbWLANTARGET_Callback _wlanTargetCallback =
{
    handleWlanTargetCopyFromDataFrame,
    handleWlanTargetCopyToDataFrame,
    handleWlanTargetAllocDataFrame,
    handleWlanTargetFreeDataFrame,
    handleWlanTargetGetDataFrameSize,
    handleWlanTargetGetDataFrameTID
};

void handleWlanStatusIndication(void *dummy, cbWLAN_StatusIndicationInfo status, void *data)
{
    OdinWiFiEMAC &instance = OdinWiFiEMAC::get_instance();
    bool linkUp = false;
    bool sendCb = true;
    (void)dummy;
    (void)data;

    switch (status) {
        case cbWLAN_STATUS_CONNECTED:
        case cbWLAN_STATUS_AP_UP:
            linkUp = true;
            break;
        case cbWLAN_STATUS_STOPPED:
        case cbWLAN_STATUS_ERROR:
        case cbWLAN_STATUS_DISCONNECTED:
        case cbWLAN_STATUS_CONNECTION_FAILURE:
        case cbWLAN_STATUS_AP_DOWN:
            break;
        case cbWLAN_STATUS_CONNECTING:
        default:
            sendCb = false;
            break;
    }
    if (sendCb && instance.emac_link_state_cb) {
        instance.emac_link_state_cb(linkUp);
    }
}

void handleWlanPacketIndication(void *dummy, cbWLAN_PacketIndicationInfo *packetInfo)
{
    OdinWiFiEMAC &instance = OdinWiFiEMAC::get_instance();
    (void)dummy;

    if (instance.emac_link_input_cb) {
        instance.emac_link_input_cb((void*)packetInfo->rxData);
    }
}

cb_boolean handleWlanTargetCopyFromDataFrame(uint8_t* buffer, cbWLANTARGET_dataFrame* frame, uint32_t size, uint32_t offsetInFrame)
{
    EMACMemoryManager *mem = OdinWiFiEMAC::get_instance().memory_manager;
    MBED_ASSERT(mem != NULL);

    emac_mem_buf_t* phead = static_cast<emac_mem_buf_t *>(frame);
    emac_mem_buf_t* pbuf;
    uint32_t copySize, bytesCopied = 0, pbufOffset = 0;

    MBED_ASSERT(frame != NULL);
    MBED_ASSERT(buffer != NULL);

    pbuf = phead;
    while (pbuf != NULL) {
        if ((pbufOffset + mem->get_len(pbuf)) >= offsetInFrame) {
            copySize = cb_MIN(size, mem->get_len(pbuf) - (offsetInFrame - pbufOffset));
            memcpy(buffer, (int8_t *)mem->get_ptr(pbuf) + (offsetInFrame - pbufOffset), copySize);
            buffer += copySize;
            bytesCopied += copySize;
            pbuf = mem->get_next(pbuf);
            break;
        }
        pbufOffset += mem->get_len(pbuf);
        pbuf = mem->get_next(pbuf);
    }

    while (pbuf != NULL && bytesCopied < size) {
        copySize = cb_MIN(mem->get_len(pbuf), size - bytesCopied);
        memcpy(buffer, mem->get_ptr(pbuf), copySize);
        buffer += copySize;
        bytesCopied += copySize;
        pbuf = mem->get_next(pbuf);
    }

    MBED_ASSERT(bytesCopied <= size);

    return (bytesCopied == size);
}

cb_boolean handleWlanTargetCopyToDataFrame(cbWLANTARGET_dataFrame* frame, uint8_t* buffer, uint32_t size, uint32_t offsetInFrame)
{
    EMACMemoryManager *mem = OdinWiFiEMAC::get_instance().memory_manager;
    MBED_ASSERT(mem != NULL);

    emac_mem_buf_t* phead = static_cast<emac_mem_buf_t *>(frame);
    emac_mem_buf_t* pbuf;
    uint32_t copySize, bytesCopied = 0, pbufOffset = 0;

    MBED_ASSERT(frame != NULL);
    MBED_ASSERT(buffer != NULL);

    pbuf = phead;
    while (pbuf != NULL) {
        if ((pbufOffset + mem->get_len(pbuf)) >= offsetInFrame) {
            copySize = cb_MIN(size, mem->get_len(pbuf) - (offsetInFrame - pbufOffset));
            memcpy((uint8_t *)mem->get_ptr(pbuf) + (offsetInFrame - pbufOffset), buffer, copySize);
            buffer += copySize;
            bytesCopied += copySize;
            pbuf = mem->get_next(pbuf);
            break;
        }
        pbufOffset += mem->get_len(pbuf);
        pbuf = mem->get_next(pbuf);
    }

    while (pbuf != NULL && bytesCopied < size) {
        copySize = cb_MIN(mem->get_len(pbuf), size - bytesCopied);
        memcpy(mem->get_ptr(pbuf), buffer, copySize);
        buffer += copySize;
        bytesCopied += copySize;
        pbuf = mem->get_next(pbuf);
    }

    MBED_ASSERT(bytesCopied <= size);

    return (bytesCopied == size);
}

cbWLANTARGET_dataFrame* handleWlanTargetAllocDataFrame(uint32_t size)
{
    EMACMemoryManager *mem = OdinWiFiEMAC::get_instance().memory_manager;
    MBED_ASSERT(mem != NULL);
    return (cbWLANTARGET_dataFrame*)mem->alloc_pool(size, 0);
}

void handleWlanTargetFreeDataFrame(cbWLANTARGET_dataFrame* frame)
{
    EMACMemoryManager *mem = OdinWiFiEMAC::get_instance().memory_manager;
    MBED_ASSERT(mem != NULL);
    mem->free(static_cast<emac_mem_buf_t *>(frame));
}

uint32_t handleWlanTargetGetDataFrameSize(cbWLANTARGET_dataFrame* frame)
{
    EMACMemoryManager *mem = OdinWiFiEMAC::get_instance().memory_manager;
    MBED_ASSERT(mem != NULL);
    return mem->get_total_len(static_cast<emac_mem_buf_t *>(frame));
}

uint8_t handleWlanTargetGetDataFrameTID(cbWLANTARGET_dataFrame* frame)
{
    (void)frame;
    return (uint8_t)cbWLAN_AC_BE;
}

OdinWiFiEMAC::OdinWiFiEMAC()
{
    emac_link_input_cb = NULL;
    emac_link_state_cb = NULL;
    cbWLANTARGET_registerCallbacks((cbWLANTARGET_Callback*)&_wlanTargetCallback);
}

cbWLAN_Handle OdinWiFiEMAC::get_wifi_emac_handle()
{
	return this->handle;
}

void OdinWiFiEMAC::set_wifi_emac_handle(cbWLAN_Handle _handle)
{
	this->handle = _handle;
}

void send_wlan_packet(OdinWiFiEMAC *ptr, void *buf)
{
	cbWLAN_sendPacket(ptr->handle, buf);
}

bool OdinWiFiEMAC::link_out(emac_mem_buf_t *buf)
{
    EMACMemoryManager *mem = OdinWiFiEMAC::get_instance().memory_manager;

    // Break call chain to avoid the driver affecting stack usage for the IP stack thread too much
    emac_mem_buf_t *new_buf = mem->alloc_pool(mem->get_total_len(buf), 0);
    if (new_buf != NULL) {
        mem->copy(new_buf, buf);
        int id = cbMAIN_getEventQueue()->call(send_wlan_packet, this, new_buf);
        if (id != 0) {
            cbMAIN_dispatchEventQueue();
        } else {
            mem->free(new_buf);
        }
    }
    mem->free(buf);
    return true;
}

bool OdinWiFiEMAC::power_up()
{
    /* Initialize the hardware */
    /* No-op at this stage */
    return true;
}

uint32_t OdinWiFiEMAC::get_mtu_size() const
{
    return OdinWiFiEMAC_MTU_SIZE;
}

void OdinWiFiEMAC::get_ifname(char *name, uint8_t size) const
{
    memcpy(name, _ifname, (size < sizeof(_ifname)) ? size : sizeof(_ifname));
}

uint8_t OdinWiFiEMAC::get_hwaddr_size() const
{
    return sizeof(cbWLAN_MACAddress);
}

bool OdinWiFiEMAC::get_hwaddr(uint8_t *addr) const
{
    cbOTP_read(cbOTP_MAC_WLAN, sizeof(cbWLAN_MACAddress), addr);
    return true;
}

void OdinWiFiEMAC::set_hwaddr(const uint8_t *addr)
{
  /* No-op at this stage */
}

void OdinWiFiEMAC::set_link_input_cb(emac_link_input_cb_t input_cb)
{
    emac_link_input_cb = input_cb;
    cbMAIN_driverLock();
    cbWLAN_registerPacketIndicationCallback(get_wifi_emac_handle(), handleWlanPacketIndication, NULL);
    cbMAIN_driverUnlock();
}

void OdinWiFiEMAC::set_link_state_cb(emac_link_state_change_cb_t state_cb)
{
    emac_link_state_cb = state_cb;

    cbMAIN_driverLock();
    cbWLAN_registerStatusCallback(handleWlanStatusIndication, NULL);
    cbMAIN_driverUnlock();
}

void OdinWiFiEMAC::power_down()
{
  /* No-op at this stage */
}

void OdinWiFiEMAC::set_memory_manager(EMACMemoryManager &mem_mngr)
{
    memory_manager = &mem_mngr;
}

uint32_t OdinWiFiEMAC::get_align_preference() const
{
    return 1;
}

void OdinWiFiEMAC::add_multicast_group(const uint8_t *address)
{

}

void OdinWiFiEMAC::remove_multicast_group(const uint8_t *address)
{

}

void OdinWiFiEMAC::set_all_multicast(bool all)
{

}

OdinWiFiEMAC &OdinWiFiEMAC::get_instance() {
    static OdinWiFiEMAC emac;
    return emac;
}

#endif // DEVICE_WIFI
