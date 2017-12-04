#if DEVICE_EMAC

#include <stdio.h>
#include "cb_main.h"
#include "cb_wlan.h"
#include "cb_wlan_types.h"
#include "cb_otp.h"
#include "cb_wlan_target_data.h"
#include "emac_api.h"
#include "mbed_assert.h"
#include "rtos.h"
#include "mbed_events.h"

/*===========================================================================
* DEFINES
*=========================================================================*/
#define WIFI_EMAC_API_MTU_SIZE  (1500U)

/*===========================================================================
* TYPES
*=========================================================================*/
typedef struct {
    emac_link_input_fn wifi_input_cb;
    emac_link_state_change_fn wifi_state_cb;
    void* link_input_user_data;
    void* link_state_user_data;
    bool linkStateRegistered;
} wifi_emac_api_s;

/*===========================================================================
* DECLARATIONS
*=========================================================================*/
static void statusIndication(void *dummy, cbWLAN_StatusIndicationInfo status, void *data);
static void packetIndication(void *dummy, cbWLAN_PacketIndicationInfo *packetInfo);
static cb_boolean handleWlanTargetCopyFromDataFrame(uint8_t* buffer, cbWLANTARGET_dataFrame* frame, uint32_t size, uint32_t offsetInFrame);
static cb_boolean handleWlanTargetCopyToDataFrame(cbWLANTARGET_dataFrame* frame, uint8_t* buffer, uint32_t size, uint32_t offsetInFrame);
static cbWLANTARGET_dataFrame* handleWlanTargetAllocDataFrame(uint32_t size);
static void handleWlanTargetFreeDataFrame(cbWLANTARGET_dataFrame* frame);
static cb_uint32 handleWlanTargetGetDataFrameSize(cbWLANTARGET_dataFrame* frame);
static cb_uint8 handleWlanTargetGetDataFrameTID(cbWLANTARGET_dataFrame* frame);

static uint32_t wifi_get_mtu_size(emac_interface_t *emac);
static void wifi_get_ifname(emac_interface_t *emac, char *name, uint8_t size);
static uint8_t wifi_get_hwaddr_size(emac_interface_t *emac);
static void wifi_get_hwaddr(emac_interface_t *emac, uint8_t *addr);
static void wifi_set_hwaddr(emac_interface_t *emac, uint8_t *addr);
static bool wifi_link_out(emac_interface_t *emac, emac_stack_mem_t *buf);
static bool wifi_power_up(emac_interface_t *emac);
static void wifi_power_down(emac_interface_t *emac);
static void wifi_set_link_input_cb(emac_interface_t *emac, emac_link_input_fn input_cb, void *data);
static void wifi_set_link_state_cb(emac_interface_t *emac, emac_link_state_change_fn state_cb, void *data);

/*===========================================================================
* DEFINITIONS
*=========================================================================*/
static wifi_emac_api_s _admin;
static const char _ifname[] = "WL0";

const emac_interface_ops_t wifi_emac_interface = {
    .get_mtu_size = wifi_get_mtu_size,
    .get_ifname = wifi_get_ifname,
    .get_hwaddr_size = wifi_get_hwaddr_size,
    .get_hwaddr = wifi_get_hwaddr,
    .set_hwaddr = wifi_set_hwaddr,
    .link_out = wifi_link_out,
    .power_up = wifi_power_up,
    .power_down = wifi_power_down,
    .set_link_input_cb = wifi_set_link_input_cb,
    .set_link_state_cb = wifi_set_link_state_cb
};

static emac_interface_t* _intf = NULL;

static const cbWLANTARGET_Callback _wlanTargetCallback =
{
    handleWlanTargetCopyFromDataFrame,
    handleWlanTargetCopyToDataFrame,
    handleWlanTargetAllocDataFrame,
    handleWlanTargetFreeDataFrame,
    handleWlanTargetGetDataFrameSize,
    handleWlanTargetGetDataFrameTID
};

/*===========================================================================
* FUNCTIONS
*=========================================================================*/
static void statusIndication(void *dummy, cbWLAN_StatusIndicationInfo status, void *data)
{
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
    if (sendCb) {
        _admin.wifi_state_cb(_admin.link_state_user_data, linkUp);
    }
}

static void packetIndication(void *dummy, cbWLAN_PacketIndicationInfo *packetInfo)
{
    (void)dummy;
    _admin.wifi_input_cb(_admin.link_input_user_data, (void*)packetInfo->rxData);
}

static cb_boolean handleWlanTargetCopyFromDataFrame(uint8_t* buffer, cbWLANTARGET_dataFrame* frame, uint32_t size, uint32_t offsetInFrame)
{
    void* dummy = NULL;
    emac_stack_mem_t** phead = (emac_stack_mem_chain_t **)&frame;
    emac_stack_mem_t* pbuf;
    uint32_t copySize, bytesCopied = 0, pbufOffset = 0;

    MBED_ASSERT(frame != NULL);
    MBED_ASSERT(buffer != NULL);

    pbuf = emac_stack_mem_chain_dequeue(dummy, phead);
    while (pbuf != NULL) {
        if ((pbufOffset + emac_stack_mem_len(dummy, pbuf)) >= offsetInFrame) {
            copySize = cb_MIN(size, emac_stack_mem_len(dummy, pbuf) - (offsetInFrame - pbufOffset));
            memcpy(buffer, (int8_t *)emac_stack_mem_ptr(dummy, pbuf) + (offsetInFrame - pbufOffset), copySize);
            buffer += copySize;
            bytesCopied += copySize;
            pbuf = emac_stack_mem_chain_dequeue(dummy, phead);
            break;
        }
        pbufOffset += emac_stack_mem_len(dummy, pbuf);
        pbuf = emac_stack_mem_chain_dequeue(dummy, phead);
    }

    while (pbuf != NULL && bytesCopied < size) {
        copySize = cb_MIN(emac_stack_mem_len(dummy, pbuf), size - bytesCopied);
        memcpy(buffer, emac_stack_mem_ptr(dummy, pbuf), copySize);
        buffer += copySize;
        bytesCopied += copySize;
        pbuf = emac_stack_mem_chain_dequeue(dummy, phead);
    }

    MBED_ASSERT(bytesCopied <= size);

    return (bytesCopied == size);
}

static cb_boolean handleWlanTargetCopyToDataFrame(cbWLANTARGET_dataFrame* frame, uint8_t* buffer, uint32_t size, uint32_t offsetInFrame)
{
    void* dummy = NULL;
    emac_stack_mem_t** phead = (emac_stack_mem_chain_t **)&frame;
    emac_stack_mem_t* pbuf;
    uint32_t copySize, bytesCopied = 0, pbufOffset = 0;

    MBED_ASSERT(frame != NULL);
    MBED_ASSERT(buffer != NULL);

    pbuf = emac_stack_mem_chain_dequeue(dummy, phead);
    while (pbuf != NULL) {
        if ((pbufOffset + emac_stack_mem_len(dummy, pbuf)) >= offsetInFrame) {
            copySize = cb_MIN(size, emac_stack_mem_len(dummy, pbuf) - (offsetInFrame - pbufOffset));
            memcpy((uint8_t *)emac_stack_mem_ptr(dummy, pbuf) + (offsetInFrame - pbufOffset), buffer, copySize);
            buffer += copySize;
            bytesCopied += copySize;
            pbuf = emac_stack_mem_chain_dequeue(dummy, phead);
            break;
        }
        pbufOffset += emac_stack_mem_len(dummy, pbuf);
        pbuf = emac_stack_mem_chain_dequeue(dummy, phead);
    }

    while (pbuf != NULL && bytesCopied < size) {
        copySize = cb_MIN(emac_stack_mem_len(dummy, pbuf), size - bytesCopied);
        memcpy(emac_stack_mem_ptr(dummy, pbuf), buffer, copySize);
        buffer += copySize;
        bytesCopied += copySize;
        pbuf = emac_stack_mem_chain_dequeue(dummy, phead);
    }

    MBED_ASSERT(bytesCopied <= size);

    return (bytesCopied == size);
}

static cbWLANTARGET_dataFrame* handleWlanTargetAllocDataFrame(uint32_t size)
{
    void* dummy = NULL;

    return (cbWLANTARGET_dataFrame*)emac_stack_mem_alloc(dummy, size, 0);
}

static void handleWlanTargetFreeDataFrame(cbWLANTARGET_dataFrame* frame)
{
    void* dummy = NULL;

    emac_stack_mem_free(dummy, (emac_stack_mem_t*)frame);
}

static uint32_t handleWlanTargetGetDataFrameSize(cbWLANTARGET_dataFrame* frame)
{
    void* dummy = NULL;
    return emac_stack_mem_chain_len(dummy, (emac_stack_mem_t*)frame);
}

static uint8_t handleWlanTargetGetDataFrameTID(cbWLANTARGET_dataFrame* frame)
{
    (void)frame;
    return (uint8_t)cbWLAN_AC_BE;
}

/*===========================================================================
* API FUNCTIONS
*=========================================================================*/
static uint32_t wifi_get_mtu_size(emac_interface_t *emac)
{
    (void)emac;

    return WIFI_EMAC_API_MTU_SIZE;
}

static void wifi_get_ifname(emac_interface_t *emac, char *name, uint8_t size)
{
    (void)emac;
    MBED_ASSERT(name != NULL);
    memcpy((void*)name, (void*)&_ifname, cb_MIN(size, sizeof(_ifname)));
}

static uint8_t wifi_get_hwaddr_size(emac_interface_t *emac)
{
    (void)emac;

    return sizeof(cbWLAN_MACAddress);
}

static void wifi_get_hwaddr(emac_interface_t *emac, uint8_t *addr)
{
    (void)emac;

    cbOTP_read(cbOTP_MAC_WLAN, sizeof(cbWLAN_MACAddress), addr);
}

static void wifi_set_hwaddr(emac_interface_t *emac, uint8_t *addr)
{
    (void)emac;
    (void)addr;
    
    // Do nothing, not possible to change the address
}

static void send_packet(emac_interface_t *emac, void *buf)
{
    cbWLAN_sendPacket(buf);
    emac_stack_mem_free(emac,buf);
}

static bool wifi_link_out(emac_interface_t *emac, emac_stack_mem_t *buf)
{
    (void)emac;
    // Break call chain to avoid the driver affecting stack usage for the IP stack thread too much
    emac_stack_mem_t *new_buf = emac_stack_mem_alloc(emac, emac_stack_mem_chain_len(emac,buf),0);
    if (new_buf != NULL) {
        emac_stack_mem_copy(emac, new_buf, buf);
        int id = cbMAIN_getEventQueue()->call(send_packet, emac, new_buf);
        if (id != 0) {
            cbMAIN_dispatchEventQueue();        
        }
        else {
            emac_stack_mem_free(emac, new_buf);
        }
    }
    return true;
}


static bool wifi_power_up(emac_interface_t *emac)
{
    (void)emac;

    return true;
}

static void wifi_power_down(emac_interface_t *emac)
{
    (void)emac;
}

static void wifi_set_link_input_cb(emac_interface_t *emac, emac_link_input_fn input_cb, void *data)
{
    void *dummy = NULL;
    (void)emac;

    _admin.wifi_input_cb = input_cb;
    _admin.link_input_user_data = data;

    cbMAIN_driverLock();
    cbWLAN_registerPacketIndicationCallback(packetIndication, dummy);
    cbMAIN_driverUnlock();
}

static void wifi_set_link_state_cb(emac_interface_t *emac, emac_link_state_change_fn state_cb, void *data)
{
    cbRTSL_Status result;
    void *dummy = NULL;
    (void)emac;
    
    _admin.wifi_state_cb = state_cb;
    _admin.link_state_user_data = data;

    if (!_admin.linkStateRegistered) {
        cbMAIN_driverLock();
        result = cbWLAN_registerStatusCallback(statusIndication, dummy);
        cbMAIN_driverUnlock();
        if (result == cbSTATUS_OK) {
            _admin.linkStateRegistered = true;
        }
    }
}

emac_interface_t* wifi_emac_get_interface()
{
    if (_intf == NULL) {
        _intf = (emac_interface_t*)malloc(sizeof(emac_interface_t));
        if (_intf) {
            _intf->hw = NULL;
            memcpy((void*)&_intf->ops, &wifi_emac_interface, sizeof(wifi_emac_interface));
        }
    }
   return _intf;
}

void wifi_emac_init_mem(void)
{
    cbWLANTARGET_registerCallbacks((cbWLANTARGET_Callback*)&_wlanTargetCallback);
}

#endif
