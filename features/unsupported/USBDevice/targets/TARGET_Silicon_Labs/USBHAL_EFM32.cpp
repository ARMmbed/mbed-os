/* Copyright 2015 Silicon Labs, http://www.silabs.com
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

#if defined TARGET_EFM32GG_STK3700 || \
    defined TARGET_EFM32LG_STK3600 || \
    defined TARGET_EFM32WG_STK3800 || \
    defined TARGET_EFM32HG_STK3400

#include "USBHAL.h"
#include "em_usb.h"
#include "em_usbtypes.h"
#include "em_usbhal.h"
#include "em_usbd.h"
#include "mbed_sleep.h"

enum USBISRCommand {
    CMD_HANDLED = 0,
    CMD_EP0SETUP,
    CMD_EP0IN,
    CMD_EP0OUT,
    CMD_EP_XFER_COMPLETED,
    CMD_SOF,
    CMD_BUSRESET,
    CMD_SUSPEND_STATE_CHANGED,
    CMD_ENUM_END_MARKER
};

enum IEPStatus {
    NOT_CONFIGURED = 0,
    IDLE = 1,
    READ_PENDING = 2,
    WRITE_PENDING = 3,
    READ_COMPLETE = 4,
    WRITE_COMPLETE = 5,
    FAILED_INVALID = 6,
    FAILED_STALLED = 7
};

typedef struct {
    IEPStatus status;
    uint32_t byte_count;
    uint32_t max_packet;
    USB_XferCompleteCb_TypeDef intern_cb;
    uint8_t *data_buf;
} ep_state_t;

USBHAL * USBHAL::instance;
static uint8_t ep0setupdata[8];
static ep_state_t ep_state[NUMBER_OF_ENDPOINTS];
#ifdef USB_USE_DYNAMIC_MEMORY
static uint8_t ep0in_data_buf[MAX_PACKET_SIZE_EP0] __attribute__ ((aligned (4)));
static uint8_t ep0out_data_buf[MAX_PACKET_SIZE_EP0]; // FIXME: does this need to be this big?
#else
static uint8_t ep_data_buf[NUMBER_OF_ENDPOINTS][64] __attribute__ ((aligned (4)));
#endif

static void run_cmd(USBISRCommand cmd, uint32_t param);
static void (*isrptr)() = NULL;
static USBISRCommand usb_isrcmd = CMD_HANDLED;
static uint32_t usb_isrcmd_param = 0;

extern "C" void usbhal_allow_em2(bool allow_em2);

#ifdef DEBUG_USB_API
#define TRACE(fmt,...)            printf("USB:   %s: " fmt "\n", __func__, __VA_ARGS__);
#define TRACE_FUNC_IN             printf("USB: > %s\n",__func__);
#define TRACE_FUNC_IN_P(fmt, ...) printf("USB: > %s: " fmt "\n", __func__, __VA_ARGS__);
#else
#define TRACE(fmt,...)
#define TRACE_FUNC_IN
#define TRACE_FUNC_IN_P(fmt, ...)
#endif

static EP_STATUS internEndpointRead(uint8_t ep, uint32_t maxSize);

static int usbhal_xfer_complete_cb(uint8_t epaddr, USB_Status_TypeDef status,
                                   uint32_t xferred, uint32_t remaining);
static void usbhal_free_buffers(void);

/* Internal EP transfer complete callbacks */
#define EPCB(n) static int usbhal_xfer_complete_cb_##n(USB_Status_TypeDef status,              \
                                                       uint32_t xferred, uint32_t remaining) { \
        return usbhal_xfer_complete_cb(n, status, xferred, remaining);                         \
}
/*   ------^   */
EPCB(EP0OUT)
EPCB(EP0IN)
EPCB(EP1OUT)
EPCB(EP1IN)
EPCB(EP2OUT)
EPCB(EP2IN)
EPCB(EP3OUT)
EPCB(EP3IN)
#ifndef TARGET_EFM32HG_STK3400
EPCB(EP4OUT)
EPCB(EP4IN)
EPCB(EP5OUT)
EPCB(EP5IN)
EPCB(EP6OUT)
EPCB(EP6IN)
#endif

static inline bool is_aligned(const void *pointer, size_t byte_count)
{
    return ((uintptr_t)pointer % byte_count == 0);
}

USBHAL::USBHAL(void)
{
    TRACE_FUNC_IN;

    isrptr = &USBHAL::_usbisr;

    if (instance) {
        TRACE("Assert self failed! instance=%p", instance);
        abort();
    }
    instance = this;

    // When USB is active, we can't go below EM1. This block may
    // be dynamically removed/reinstated to allow deeper sleep.
    usbhal_allow_em2(false);

    epCallback[EP0OUT] = NULL;
    epCallback[EP0IN ] = NULL;
    epCallback[EP1OUT] = &USBHAL::EP1_OUT_callback;
    epCallback[EP1IN ] = &USBHAL::EP1_IN_callback;
    epCallback[EP2OUT] = &USBHAL::EP2_OUT_callback;
    epCallback[EP2IN ] = &USBHAL::EP2_IN_callback;
    epCallback[EP3OUT] = &USBHAL::EP3_OUT_callback;
    epCallback[EP3IN ] = &USBHAL::EP3_IN_callback;
#ifndef TARGET_EFM32HG_STK3400
    epCallback[EP4OUT] = &USBHAL::EP4_OUT_callback;
    epCallback[EP4IN ] = &USBHAL::EP4_IN_callback;
    epCallback[EP5OUT] = &USBHAL::EP5_OUT_callback;
    epCallback[EP5IN ] = &USBHAL::EP5_IN_callback;
    epCallback[EP6OUT] = &USBHAL::EP6_OUT_callback;
    epCallback[EP6IN ] = &USBHAL::EP6_IN_callback;
#endif

    memset(ep_state, 0, sizeof(ep_state));

    ep_state[EP0OUT].intern_cb = usbhal_xfer_complete_cb_EP0OUT;
    ep_state[EP0IN ].intern_cb = usbhal_xfer_complete_cb_EP0IN;
    ep_state[EP1OUT].intern_cb = usbhal_xfer_complete_cb_EP1OUT;
    ep_state[EP1IN ].intern_cb = usbhal_xfer_complete_cb_EP1IN;
    ep_state[EP2OUT].intern_cb = usbhal_xfer_complete_cb_EP2OUT;
    ep_state[EP2IN ].intern_cb = usbhal_xfer_complete_cb_EP2IN;
    ep_state[EP3OUT].intern_cb = usbhal_xfer_complete_cb_EP3OUT;
    ep_state[EP3IN ].intern_cb = usbhal_xfer_complete_cb_EP3IN;
#ifndef TARGET_EFM32HG_STK3400
    ep_state[EP4OUT].intern_cb = usbhal_xfer_complete_cb_EP4OUT;
    ep_state[EP4IN ].intern_cb = usbhal_xfer_complete_cb_EP4IN;
    ep_state[EP5OUT].intern_cb = usbhal_xfer_complete_cb_EP5OUT;
    ep_state[EP5IN ].intern_cb = usbhal_xfer_complete_cb_EP5IN;
    ep_state[EP6OUT].intern_cb = usbhal_xfer_complete_cb_EP6OUT;
    ep_state[EP6IN ].intern_cb = usbhal_xfer_complete_cb_EP6IN;
#endif

#ifdef USB_USE_DYNAMIC_MEMORY
    ep_state[EP0OUT].data_buf = ep0out_data_buf;
    ep_state[EP0IN].data_buf = ep0in_data_buf;
#else
    for (int i=0 ; i<NUMBER_OF_ENDPOINTS ; i++) {
        ep_state[i].data_buf = ep_data_buf[i];
    }
#endif
}

USBHAL::~USBHAL(void)
{
    TRACE_FUNC_IN;
    USBD_AbortAllTransfers();
    USBD_Disconnect();
    usbhal_free_buffers();

    usbhal_allow_em2(true);
}

extern "C" void usbhal_allow_em2(bool allow_em2)
{
    static bool blocked = false;
    if (allow_em2 && blocked) {
        sleep_manager_unlock_deep_sleep();
        blocked = false;
    } else if (!blocked) {
        sleep_manager_lock_deep_sleep();
        blocked = true;
    }
}

static void usbhal_reset_cb(void)
{
    TRACE_FUNC_IN;
    run_cmd(CMD_BUSRESET, 0);
}

#ifdef DEBUG_USB_API
static const char *usbstate[] = { "NONE", "ATTACHED", "POWERED", "DEFAULT",
                                  "ADDRESSED", "CONFIGURED", "SUSPENDED", "???" };
#endif

static void usbhal_state_change_cb(USBD_State_TypeDef oldState,
                                   USBD_State_TypeDef newState)
{
    TRACE("state changed %s -> %s", usbstate[oldState], usbstate[newState]);

    if (oldState == USBD_STATE_SUSPENDED) {
        run_cmd(CMD_SUSPEND_STATE_CHANGED, 0);
    }

    if (newState == USBD_STATE_SUSPENDED) {
        run_cmd(CMD_SUSPEND_STATE_CHANGED, 1);
    }

    // Should call connectStateChanged from here as well but there is
    // no documentation on when to actually do so. (And the implementation
    // in USBDevice.cpp is a stub)

    // HACK! Since connectStateChanged is not used, indicate the loss
    // off connection by reporting a bus reset. This causes USBDevice
    // to realise that at least it's not in CONFIGURED anymore, and
    // stop trying to read/write in a busyloop.
    if (newState == USBD_STATE_NONE) {
        run_cmd(CMD_BUSRESET, 0);
    }
}

static int usbhal_setupcmd_cb(const USB_Setup_TypeDef *setup)
{
    TRACE_FUNC_IN;
    if (!setup) {
        EFM_ASSERT(false);
        return USB_STATUS_REQ_ERR;
    }

    memcpy(ep0setupdata, setup, 8);
    run_cmd(CMD_EP0SETUP, 0);

    return USB_STATUS_OK;
}

static void usbhal_sof_cb(uint16_t frameNum)
{
    run_cmd(CMD_SOF, frameNum);
}

static void usbhal_free_buffers(void)
{
#ifdef USB_USE_DYNAMIC_MEMORY
    TRACE_FUNC_IN;

    for (int i=EP1OUT ; i<NUMBER_OF_ENDPOINTS ; i++ ) {
        if (ep_state[i].data_buf) {
            free(ep_state[i].data_buf);
            ep_state[i].data_buf = NULL;
        }
    }
#endif
}

void USBHAL::connect(void)
{
    TRACE_FUNC_IN;

    // Init datastructures must be static - driver will use these even after the init function exits!

    static const uint8_t buffer_multiplier[] = { 1 }; // Mult 1 for control EP
    static const USBD_Callbacks_TypeDef usbd_callbacks = {
        .usbReset = usbhal_reset_cb,
        .usbStateChange = usbhal_state_change_cb,
        .setupCmd = usbhal_setupcmd_cb,
        .isSelfPowered = NULL,
        .sofInt = usbhal_sof_cb
    };

    USBD_Init_TypeDef initdata = {
        .deviceDescriptor = NULL,
        .configDescriptor = NULL,
        .stringDescriptors = NULL,
        .numberOfStrings = 0,
        .bufferingMultiplier = buffer_multiplier,
        .callbacks = &usbd_callbacks,
        .reserved = 0
    };

    int ret = USBD_Init(&initdata);

    TRACE("init = %d, devicedesc = %lx, configdesc = %lx", ret,
          (uint32_t) initdata.deviceDescriptor,
          (uint32_t) initdata.configDescriptor);

    EFM_ASSERT(ret == USB_STATUS_OK);
}

void USBHAL::disconnect(void)
{
    TRACE_FUNC_IN;
    USBD_Disconnect();
}

void USBHAL::configureDevice(void)
{
    TRACE_FUNC_IN;
    USBD_SetUsbState(USBD_STATE_CONFIGURED);
}

void USBHAL::unconfigureDevice(void)
{
    TRACE_FUNC_IN;
    USBD_SetUsbState(USBD_STATE_DEFAULT);
    usbhal_free_buffers();
}

void USBHAL::setAddress(uint8_t address)
{
    TRACE_FUNC_IN_P("addr 0x%x", (unsigned)address);
    USBD_SetAddress(address);
}

void USBHAL::remoteWakeup(void)
{
    TRACE_FUNC_IN;
    USBD_RemoteWakeup();
}

void USBHAL::EP0setup(uint8_t *buffer)
{
    TRACE_FUNC_IN;
    EFM_ASSERT(buffer);
    if (buffer) {
        memcpy(buffer, ep0setupdata, 8);
    }
}

void USBHAL::EP0read(void)
{
    TRACE_FUNC_IN;
    (void)internEndpointRead(0, MAX_PACKET_SIZE_EP0);
}

void USBHAL::EP0readStage(void)
{
    TRACE_FUNC_IN;
    // Not needed
}

uint32_t USBHAL::EP0getReadResult(uint8_t *buffer)
{
    TRACE_FUNC_IN;
    EFM_ASSERT(buffer);

    uint32_t read = 0;
    endpointReadResult(0, buffer, &read);
    return read;
}

static int usbhal_xfer_complete_cb(uint8_t ep, USB_Status_TypeDef status,
                                   uint32_t xferred, uint32_t remaining)
{
    TRACE_FUNC_IN_P("ep 0x%x, status %u, xferred %lu, rem %lu",
                    ep, status, xferred, remaining);

    if (ep >= NUMBER_OF_ENDPOINTS) {
        EFM_ASSERT(false);
        return USB_STATUS_REQ_ERR;
    }

    switch (ep) {
        case EP0OUT:
            if (ep_state[EP0OUT].status == READ_PENDING) {
                ep_state[EP0OUT].status = READ_COMPLETE;
                ep_state[EP0OUT].byte_count = xferred;
                // drop zlp
                if (xferred == 0) {
                    break;
                }
            }
            run_cmd(CMD_EP0OUT, 0);
            break;

        case EP0IN:
            run_cmd(CMD_EP0IN, 0);
            break;

        default:
            bool write = ep & 1;

            if (status == USB_STATUS_OK) {
                if (!write && ep_state[ep].status == READ_PENDING) {
                    ep_state[ep].status = READ_COMPLETE;
                    ep_state[ep].byte_count = xferred;
                } else if (write && ep_state[ep].status == WRITE_PENDING) {
                    ep_state[ep].status = WRITE_COMPLETE;
                } else {
                    ep_state[ep].status = FAILED_INVALID;
                }
            } else {
                ep_state[ep].status = FAILED_INVALID;
            }

            if (ep_state[ep].status != FAILED_INVALID) {
                run_cmd(CMD_EP_XFER_COMPLETED, ep);
            }
            break;
    }

    return USB_STATUS_OK;
}

void USBHAL::EP0write(uint8_t *buffer, uint32_t size)
{
    //TRACE_FUNC_IN_P("buffer %lx, size %lu", (uint32_t) buffer, size);

    int ret;
    USB_XferCompleteCb_TypeDef cb = ep_state[EP0IN].intern_cb;

    EFM_ASSERT((buffer != NULL) || (size == 0));
    EFM_ASSERT(size <= MAX_PACKET_SIZE_EP0);

    if (!buffer || size == 0) {
        // No callback after writing EP0 ZLP
        cb = NULL;
    }

    if (buffer && !is_aligned(buffer,4)) {
        // Copy unaligned data to write-buffer before USBD_Write
        memcpy(ep_state[EP0IN].data_buf, buffer, size);
        ret = USBD_Write(0, ep_state[EP0IN].data_buf, size, cb);
    } else {
        ret = USBD_Write(0, buffer, size, cb);
    }

    if (ret != USB_STATUS_OK) {
        TRACE("FAILED - ret %d", ret);
    }
}

void USBHAL::EP0stall(void)
{
    TRACE_FUNC_IN;
    USBD_StallEp0();
}

static EP_STATUS internEndpointRead(uint8_t ep, uint32_t maxSize)
{
    //TRACE_FUNC_IN_P("endpoint 0x%x, size %ld, cb %d", (unsigned)ep, maxSize, useCallback);

    if (ep >= NUMBER_OF_ENDPOINTS) {
        EFM_ASSERT(false);
        return EP_INVALID;
    }

    ep_state[ep].status = READ_PENDING;

    int ret = USBD_Read(USB_EP_TO_ADDR(ep), ep_state[ep].data_buf, maxSize,
                        ep_state[ep].intern_cb);

    if (ret == USB_STATUS_OK) {
        return EP_PENDING;
    } else {
        TRACE("FAILED - ret %d", ret);

        if (ret == USB_STATUS_EP_STALLED) {
            return EP_STALLED;
        } else {
            return EP_INVALID;
        }
    }
}

EP_STATUS USBHAL::endpointRead(uint8_t endpoint, uint32_t maximumSize)
{
    return internEndpointRead(endpoint, maximumSize);
}

EP_STATUS USBHAL::endpointReadResult(uint8_t endpoint, uint8_t *data, uint32_t *bytesRead)
{
    TRACE_FUNC_IN;

    if (endpoint >= NUMBER_OF_ENDPOINTS) {
        EFM_ASSERT(false);
        return EP_INVALID;
    }

    EFM_ASSERT(data);
    EFM_ASSERT(bytesRead);
    if (!data || !bytesRead) {
        return EP_INVALID;
    }

    switch (ep_state[endpoint].status) {
        case READ_PENDING:
            return EP_PENDING;

        case READ_COMPLETE:
            memcpy(data, ep_state[endpoint].data_buf, ep_state[endpoint].byte_count);
            *bytesRead = ep_state[endpoint].byte_count;
            ep_state[endpoint].status = IDLE;
            return EP_COMPLETED;

        case FAILED_STALLED:
            ep_state[endpoint].status = IDLE;
            return EP_STALLED;

        default:
            ep_state[endpoint].status = IDLE;
            return EP_INVALID;
    }
}

EP_STATUS USBHAL::endpointWrite(uint8_t endpoint, uint8_t *data, uint32_t size)
{
    TRACE_FUNC_IN_P("endpoint 0x%x, data 0x%lx, size %lu", (unsigned )endpoint, (uint32_t)data, size);

    EFM_ASSERT(endpoint < NUMBER_OF_ENDPOINTS);
    EFM_ASSERT(endpoint > EP0IN);
    EFM_ASSERT(size <= ep_state[endpoint].max_packet);
    EFM_ASSERT(data);

    uint8_t ep = USB_EP_TO_INDEX(endpoint);

    if (endpoint >= NUMBER_OF_ENDPOINTS || endpoint <= EP0IN) {
        return EP_INVALID;
    }

    if (size > ep_state[endpoint].max_packet) {
        return EP_INVALID;
    }

    if (!data) {
        return EP_INVALID;
    }

    memcpy(ep_state[ep].data_buf, data, size);

    ep_state[ep].status = WRITE_PENDING;
    int ret = USBD_Write(USB_EP_TO_ADDR(endpoint), ep_state[ep].data_buf, size, ep_state[ep].intern_cb);

    if (ret == USB_STATUS_EP_STALLED) {
        ep_state[ep].status = IDLE;
        return EP_STALLED;
    } else if (ret != USB_STATUS_OK) {
        ep_state[ep].status = IDLE;
        return EP_INVALID;
    }

    return EP_PENDING;
}

EP_STATUS USBHAL::endpointWriteResult(uint8_t endpoint)
{
    if (endpoint >= NUMBER_OF_ENDPOINTS) {
        EFM_ASSERT(false);
        return EP_INVALID;
    }

    switch (ep_state[endpoint].status) {
        case WRITE_PENDING:
            return EP_PENDING;

        case WRITE_COMPLETE:
            ep_state[endpoint].status = IDLE;
            return EP_COMPLETED;

        case FAILED_STALLED:
            ep_state[endpoint].status = IDLE;
            return EP_STALLED;

        default:
            ep_state[endpoint].status = IDLE;
            return EP_INVALID;
    }
}

void USBHAL::stallEndpoint(uint8_t endpoint)
{
    TRACE_FUNC_IN;

    EFM_ASSERT(endpoint < NUMBER_OF_ENDPOINTS);
    EFM_ASSERT((endpoint != EP0OUT) && (endpoint != EP0IN));

    USBD_StallEp(USB_EP_TO_ADDR(endpoint));
}

void USBHAL::unstallEndpoint(uint8_t endpoint)
{
    TRACE_FUNC_IN;

    EFM_ASSERT(endpoint < NUMBER_OF_ENDPOINTS);
    EFM_ASSERT((endpoint != EP0OUT) && (endpoint != EP0IN));

    USBD_UnStallEp(USB_EP_TO_ADDR(endpoint));
}

bool USBHAL::realiseEndpoint(uint8_t endpoint, uint32_t maxPacket, uint32_t options)
{
    TRACE_FUNC_IN_P("endpoint %d, packetsize %ld, options 0x%lx", endpoint,
                    maxPacket, options);

    int mult = 1; // RX/TX buffer size multiplier
    int type = USB_EPTYPE_INTR;

    if (endpoint >= NUMBER_OF_ENDPOINTS) {
        EFM_ASSERT(false);
        return false;
    }

    if (endpoint == EP0IN || endpoint == EP0OUT) {
        EFM_ASSERT(false);
        return false;
    }

    ep_state[endpoint].max_packet = 0;

    if (endpoint == EPISO_OUT || endpoint ==  EPISO_IN) {
        if (maxPacket > MAX_PACKET_SIZE_EPISO) {
            EFM_ASSERT(false);
            return false;
        }
    } else if ((maxPacket > MAX_PACKET_SIZE_EPBULK) || (maxPacket > MAX_PACKET_SIZE_EPINT)) {
        EFM_ASSERT(false);
        return false;
    }

    // USBDevice performs a read right after creating the endpoints,
    // before calling configureDevice. The read will fail since
    // at that point the device state is still ADDRESSED. Workaround
    // is to force configured state here.
    //
    // This relies on USBDevice to not call realiseEndpoint unless
    // it is transitioning to the CONFIGURED state.
    USBD_SetUsbState(USBD_STATE_CONFIGURED);

    // Why doesn't this function have a type param? This is silly...
    switch (endpoint) {
        case EPBULK_OUT:
        case EPBULK_IN:
            type = USB_EPTYPE_BULK;
            mult = 2;
            break;
        case EPINT_OUT:
        case EPINT_IN:
            type = USB_EPTYPE_INTR;
            mult = 1;
            break;
        case EPISO_OUT:
        case EPISO_IN:
            type = USB_EPTYPE_ISOC;
            mult = 2; // ?
            break;
    }

    // Some options force the endpoint to a specific type
    if( options & ISOCHRONOUS ) {
        type = USB_EPTYPE_ISOC;
        mult = 2; // ?
    } else if ( options & RATE_FEEDBACK_MODE ) {
        // No support for whatever rate feedback is, but for interrupt only
        type = USB_EPTYPE_INTR;
        mult = 1;
    }

#ifdef USB_USE_DYNAMIC_MEMORY
    if (ep_state[endpoint].data_buf) {
        free(ep_state[endpoint].data_buf);
    }

    ep_state[endpoint].data_buf = (uint8_t *)malloc(maxPacket);

    if (!ep_state[endpoint].data_buf) {
        EFM_ASSERT(false);
        return false;
    }
#endif

    int ret = USBD_AddEndpoint(USB_EP_TO_ADDR(endpoint), type, maxPacket, mult);

    if (ret == USB_STATUS_OK) {
        ep_state[endpoint].status = IDLE;
        ep_state[endpoint].max_packet = maxPacket;
        return true;
    } else {
        return false;
    }
}

bool USBHAL::getEndpointStallState(unsigned char endpoint)
{
    TRACE_FUNC_IN;
    if (endpoint >= NUMBER_OF_ENDPOINTS) {
        EFM_ASSERT(false);
        return false;
    }
    return USBD_EpIsStalled(USB_EP_TO_ADDR(endpoint));
}

static void run_cmd(USBISRCommand cmd, uint32_t param)
{
    if (usb_isrcmd != CMD_HANDLED || cmd >= CMD_ENUM_END_MARKER) {
        EFM_ASSERT(false);
        abort();
    }

    usb_isrcmd = cmd;
    usb_isrcmd_param = param;
    isrptr();
}

void USBHAL::_usbisr(void)
{
    EFM_ASSERT(instance);
    instance->usbisr();
}

void USBHAL::usbisr(void)
{
    //TRACE_FUNC_IN;

    // This "ISR" is used just to route callbacks from SiL USB driver
    // callback context (which can not call protected/private USBHAL
    // methods), to the actual USBHAL.

    EFM_ASSERT(usb_isrcmd != CMD_HANDLED);
    switch (usb_isrcmd) {
        case CMD_EP0SETUP:
            this->EP0setupCallback();
            break;
        case CMD_EP0IN:
            this->EP0in();
            break;
        case CMD_EP0OUT:
            this->EP0out();
            break;
        case CMD_BUSRESET:
            this->busReset();
            break;
        case CMD_EP_XFER_COMPLETED:
            if (epCallback[usb_isrcmd_param] && instance) {
                (instance->*(epCallback[usb_isrcmd_param]))();
            }
            break;
        case CMD_SOF:
            this->SOF(usb_isrcmd_param);
            break;
        case CMD_SUSPEND_STATE_CHANGED:
            this->suspendStateChanged(usb_isrcmd_param);
            break;
        default:
            EFM_ASSERT(false);
            break;
    }
    usb_isrcmd = CMD_HANDLED;
}
#endif

// End of file
