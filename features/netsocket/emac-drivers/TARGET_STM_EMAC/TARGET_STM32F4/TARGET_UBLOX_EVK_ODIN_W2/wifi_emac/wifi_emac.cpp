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

#define WIFI_EMAC_MTU_SIZE  (1500U)
static const char _ifname[] = "WL0";

cb_boolean handleWlanTargetCopyFromDataFrame(uint8_t* buffer, cbWLANTARGET_dataFrame* frame, uint32_t size, uint32_t offsetInFrame);
cb_boolean handleWlanTargetCopyToDataFrame(cbWLANTARGET_dataFrame* frame, uint8_t* buffer, uint32_t size, uint32_t offsetInFrame);
cbWLANTARGET_dataFrame* handleWlanTargetAllocDataFrame(uint32_t size);
void handleWlanTargetFreeDataFrame(cbWLANTARGET_dataFrame* frame);
cb_uint32 handleWlanTargetGetDataFrameSize(cbWLANTARGET_dataFrame* frame);
cb_uint8 handleWlanTargetGetDataFrameTID(cbWLANTARGET_dataFrame* frame);
void handleWlanStatusIndication(void *dummy, cbWLAN_StatusIndicationInfo status, void *data);
void handleWlanPacketIndication(void *dummy, cbWLAN_PacketIndicationInfo *packetInfo);
void send_wlan_packet(void *buf);

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
    WIFI_EMAC &instance = WIFI_EMAC::get_instance();
    bool linkUp = false;
    bool sendCb = true;
    (void)dummy;
    (void)data;

    switch (status) {
        case cbWLAN_STATUS_CONNECTED:
        case cbWLAN_STATUS_AP_STA_ADDED:
            linkUp = true;
            break;
        case cbWLAN_STATUS_STOPPED:
        case cbWLAN_STATUS_ERROR:
        case cbWLAN_STATUS_DISCONNECTED:
        case cbWLAN_STATUS_CONNECTION_FAILURE:
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
    WIFI_EMAC &instance = WIFI_EMAC::get_instance();
    (void)dummy;

    if (instance.emac_link_input_cb) {
        instance.emac_link_input_cb((void*)packetInfo->rxData);
    }
}

cb_boolean handleWlanTargetCopyFromDataFrame(uint8_t* buffer, cbWLANTARGET_dataFrame* frame, uint32_t size, uint32_t offsetInFrame)
{
    EMACMemoryManager *mem = WIFI_EMAC::get_instance().memory_manager;
    MBED_ASSERT(mem != NULL);

    //emac_mem_buf_t* phead = (emac_mem_buf_t *)frame;
    emac_mem_buf_t* pbuf = (emac_mem_buf_t *)frame;
    uint32_t copySize, bytesCopied = 0, pbufOffset = 0;

    MBED_ASSERT(frame != NULL);
    MBED_ASSERT(buffer != NULL);

    //pbuf = mem->get_next(phead);
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
    EMACMemoryManager *mem = WIFI_EMAC::get_instance().memory_manager;
    MBED_ASSERT(mem != NULL);

    //emac_mem_buf_t* phead = (emac_mem_buf_t *)frame;
    emac_mem_buf_t* pbuf = (emac_mem_buf_t *)frame;
    uint32_t copySize, bytesCopied = 0, pbufOffset = 0;

    MBED_ASSERT(frame != NULL);
    MBED_ASSERT(buffer != NULL);

    //pbuf = mem->get_next(phead);
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
    EMACMemoryManager *mem = WIFI_EMAC::get_instance().memory_manager;
    MBED_ASSERT(mem != NULL);
    return (cbWLANTARGET_dataFrame*)mem->alloc_pool(size,0);
}

void handleWlanTargetFreeDataFrame(cbWLANTARGET_dataFrame* frame)
{
    EMACMemoryManager *mem = WIFI_EMAC::get_instance().memory_manager;
    MBED_ASSERT(mem != NULL);
    mem->free((emac_mem_buf_t*)frame);
}

uint32_t handleWlanTargetGetDataFrameSize(cbWLANTARGET_dataFrame* frame)
{
    EMACMemoryManager *mem = WIFI_EMAC::get_instance().memory_manager;
    MBED_ASSERT(mem != NULL);
    return mem->get_total_len((emac_mem_buf_t*)frame);
}

uint8_t handleWlanTargetGetDataFrameTID(cbWLANTARGET_dataFrame* frame)
{
    (void)frame;
    return (uint8_t)cbWLAN_AC_BE;
}

WIFI_EMAC::WIFI_EMAC()
{
    emac_link_input_cb = NULL;
    emac_link_state_cb = NULL;
    cbWLANTARGET_registerCallbacks((cbWLANTARGET_Callback*)&_wlanTargetCallback);
}

void send_wlan_packet(void *buf)
{
    cbWLAN_sendPacket(buf);
}

bool WIFI_EMAC::link_out(emac_mem_buf_t *buf)
{
    EMACMemoryManager *mem = WIFI_EMAC::get_instance().memory_manager;

    // Break call chain to avoid the driver affecting stack usage for the IP stack thread too much
    emac_mem_buf_t *new_buf = mem->alloc_pool(mem->get_total_len(buf), 0);
    if (new_buf != NULL) {
        mem->copy(new_buf, buf);
        int id = cbMAIN_getEventQueue()->call(send_wlan_packet, new_buf);
        if (id != 0) {
            cbMAIN_dispatchEventQueue();
        } else {
            mem->free(new_buf);
        }
    }
    mem->free(buf);
    return true;
}

bool WIFI_EMAC::power_up()
{
    /* Initialize the hardware */
    /* No-op at this stage */
    return true;
}

uint32_t WIFI_EMAC::get_mtu_size() const
{
    return WIFI_EMAC_MTU_SIZE;
}

void WIFI_EMAC::get_ifname(char *name, uint8_t size) const
{
    memcpy(name, _ifname, (size < sizeof(_ifname)) ? size : sizeof(_ifname));
}

uint8_t WIFI_EMAC::get_hwaddr_size() const
{
    return sizeof(cbWLAN_MACAddress);
}

bool WIFI_EMAC::get_hwaddr(uint8_t *addr) const
{
    cbOTP_read(cbOTP_MAC_WLAN, sizeof(cbWLAN_MACAddress), addr);
    return true;
}

void WIFI_EMAC::set_hwaddr(const uint8_t *addr)
{
  /* No-op at this stage */
}

void WIFI_EMAC::set_link_input_cb(emac_link_input_cb_t input_cb)
{
    emac_link_input_cb = input_cb;

    cbMAIN_driverLock();
    cbWLAN_registerPacketIndicationCallback(handleWlanPacketIndication, NULL);
    cbMAIN_driverUnlock();
}

void WIFI_EMAC::set_link_state_cb(emac_link_state_change_cb_t state_cb)
{
    emac_link_state_cb = state_cb;

    cbMAIN_driverLock();
    cbWLAN_registerStatusCallback(handleWlanStatusIndication, NULL);
    cbMAIN_driverUnlock();
}

void WIFI_EMAC::power_down()
{
  /* No-op at this stage */
}

void WIFI_EMAC::set_memory_manager(EMACMemoryManager &mem_mngr)
{
    memory_manager = &mem_mngr;
}

uint32_t WIFI_EMAC::get_align_preference() const
{
    return 1; // TODO not sure if there is a requirement but don't think so
}

void WIFI_EMAC::add_multicast_group(const uint8_t *address)
{
    // TODO anything to do here for WiFi?
}

void WIFI_EMAC::remove_multicast_group(const uint8_t *address)
{
    // TODO anything to do here for WiFi?
}

void WIFI_EMAC::set_all_multicast(bool all)
{
    // TODO anything to do here for WiFi?
}

WIFI_EMAC &WIFI_EMAC::get_instance() {
    static WIFI_EMAC emac;
    return emac;
}

// Weak so a module can override
MBED_WEAK EMAC &EMAC::get_default_instance()
{
    return WIFI_EMAC::get_instance();
}

#endif // DEVICE_WIFI
