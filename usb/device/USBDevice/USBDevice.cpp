/* mbed Microcontroller Library
 * Copyright (c) 2018-2018 ARM Limited
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

#include "stdint.h"

#include "USBDevice.h"
#include "USBDescriptor.h"
#include "usb_phy_api.h"

//#define DEBUG

/* Device status */
#define DEVICE_STATUS_SELF_POWERED  (1U<<0)
#define DEVICE_STATUS_REMOTE_WAKEUP (1U<<1)

/* Endpoint status */
#define ENDPOINT_STATUS_HALT        (1U<<0)

/* Standard feature selectors */
#define DEVICE_REMOTE_WAKEUP        (1)
#define ENDPOINT_HALT               (0)

/* Endpoint macros */
#define EP_TO_INDEX(endpoint)   ((((endpoint & 0xf) << 1) | (endpoint & 0x80 ? 1 : 0)) - 2)
#define EP_VALID(endpoint)      (((endpoint) & ~0x8F) == 0)
#define EP_CONTROL(endpoint)    (((endpoint) & 0xF) == 0)

/* Other defines */
#define ENDPOINT_ENABLED (1 << 0)
#define ENDPOINT_STALLED (1 << 1)

#if defined(MAX_PACKET_SIZE_EP0)
#undef MAX_PACKET_SIZE_EP0
#endif
#define MAX_PACKET_SIZE_EP0 64


bool USBDevice::request_get_descriptor(void)
{
    assert_locked();

    bool success = false;
#ifdef DEBUG
    printf("get descr: type: %d\r\n", DESCRIPTOR_TYPE(transfer.setup.wValue));
#endif
    switch (DESCRIPTOR_TYPE(transfer.setup.wValue))
    {
        case DEVICE_DESCRIPTOR:
            if (device_desc() != NULL)
            {
                if ((device_desc()[0] == DEVICE_DESCRIPTOR_LENGTH) \
                    && (device_desc()[1] == DEVICE_DESCRIPTOR))
                {
#ifdef DEBUG
                    printf("device descr\r\n");
#endif
                    transfer.remaining = DEVICE_DESCRIPTOR_LENGTH;
                    transfer.ptr = (uint8_t*)device_desc();
                    transfer.direction = SEND;
                    success = true;
                }
            }
            break;
        case CONFIGURATION_DESCRIPTOR:
            if (configuration_desc() != NULL)
            {
                if ((configuration_desc()[0] == CONFIGURATION_DESCRIPTOR_LENGTH) \
                    && (configuration_desc()[1] == CONFIGURATION_DESCRIPTOR))
                {
#ifdef DEBUG
                    printf("conf descr request\r\n");
#endif
                    /* Get wTotalLength */
                    transfer.remaining = configuration_desc()[2] \
                        | (configuration_desc()[3] << 8);

                    transfer.ptr = (uint8_t*)configuration_desc();
                    transfer.direction = SEND;
                    success = true;
                }
            }
            break;
        case STRING_DESCRIPTOR:
#ifdef DEBUG
            printf("str descriptor\r\n");
#endif
            switch (DESCRIPTOR_INDEX(transfer.setup.wValue))
            {
                            case STRING_OFFSET_LANGID:
#ifdef DEBUG
                                printf("1\r\n");
#endif
                                transfer.remaining = string_langid_desc()[0];
                                transfer.ptr = (uint8_t*)string_langid_desc();
                                transfer.direction = SEND;
                                success = true;
                                break;
                            case STRING_OFFSET_IMANUFACTURER:
#ifdef DEBUG
                                printf("2\r\n");
#endif
                                transfer.remaining =  string_imanufacturer_desc()[0];
                                transfer.ptr = (uint8_t*)string_imanufacturer_desc();
                                transfer.direction = SEND;
                                success = true;
                                break;
                            case STRING_OFFSET_IPRODUCT:
#ifdef DEBUG
                                printf("3\r\n");
#endif
                                transfer.remaining = string_iproduct_desc()[0];
                                transfer.ptr = (uint8_t*)string_iproduct_desc();
                                transfer.direction = SEND;
                                success = true;
                                break;
                            case STRING_OFFSET_ISERIAL:
#ifdef DEBUG
                                printf("4\r\n");
#endif
                                transfer.remaining = string_iserial_desc()[0];
                                transfer.ptr = (uint8_t*)string_iserial_desc();
                                transfer.direction = SEND;
                                success = true;
                                break;
                            case STRING_OFFSET_ICONFIGURATION:
#ifdef DEBUG
                                printf("5\r\n");
#endif
                                transfer.remaining = string_iconfiguration_desc()[0];
                                transfer.ptr = (uint8_t*)string_iconfiguration_desc();
                                transfer.direction = SEND;
                                success = true;
                                break;
                            case STRING_OFFSET_IINTERFACE:
#ifdef DEBUG
                                printf("6\r\n");
#endif
                                transfer.remaining = string_iinterface_desc()[0];
                                transfer.ptr = (uint8_t*)string_iinterface_desc();
                                transfer.direction = SEND;
                                success = true;
                                break;
            }
            break;
        case INTERFACE_DESCRIPTOR:
#ifdef DEBUG
            printf("interface descr\r\n");
#endif
        case ENDPOINT_DESCRIPTOR:
#ifdef DEBUG
            printf("endpoint descr\r\n");
#endif
            /* TODO: Support is optional, not implemented here */
            break;
        default:
#ifdef DEBUG
            printf("ERROR\r\n");
#endif
            break;
    }

    return success;
}

void USBDevice::decode_setup_packet(uint8_t *data, setup_packet_t *packet)
{
    // No lock needed - stateless function

    /* Fill in the elements of a setup_packet_t structure from raw data */
    packet->bmRequestType.dataTransferDirection = (data[0] & 0x80) >> 7;
    packet->bmRequestType.Type = (data[0] & 0x60) >> 5;
    packet->bmRequestType.Recipient = data[0] & 0x1f;
    packet->bRequest = data[1];
    packet->wValue = (data[2] | (uint16_t)data[3] << 8);
    packet->wIndex = (data[4] | (uint16_t)data[5] << 8);
    packet->wLength = (data[6] | (uint16_t)data[7] << 8);
}


bool USBDevice::control_out(void)
{
    assert_locked();

    /* Control transfer data OUT stage */
    uint32_t packetSize;

    /* Check we should be transferring data OUT */
    if (transfer.direction != RECEIVE)
    {
         /* for other platforms, count on the HAL to handle this case */
         return false;
    }

    /* Read from endpoint */
    packetSize = phy->ep0_read_result(transfer.ptr, transfer.remaining);

    /* Check if transfer size is valid */
    if (packetSize > transfer.remaining)
    {
        /* Too big */
        return false;
    }

    /* Update transfer */
    transfer.ptr += packetSize;
    transfer.remaining -= packetSize;

    /* Check if transfer has completed */
    if (transfer.remaining == 0)
    {
        /* Transfer completed */
        if (transfer.notify)
        {
            /* Notify class layer. */
            transfer.notify = false;
            callback_request_xfer_done(&transfer.setup);
        }
        else
        {
            complete_request_xfer_done(true);
        }
    }
    else
    {
        phy->ep0_read();
    }

    return true;
}

bool USBDevice::control_in(void)
{
    assert_locked();

    /* Control transfer data IN stage */
    uint32_t packetSize;


    /* Check we should be transferring data IN */
    if (transfer.direction != SEND)
    {
        return false;
    }

    if ((transfer.remaining == 0) && transfer.zlp) {
        /* ZLP will be sent below */
        transfer.zlp = false;
    }

    packetSize = transfer.remaining;

    if (packetSize > max_packet_size_ep0)
    {
        packetSize = max_packet_size_ep0;
    }

    /* Write to endpoint */
    phy->ep0_write(transfer.ptr, packetSize);

    /* Update transfer */
    transfer.ptr += packetSize;
    transfer.remaining -= packetSize;

    /* Send status if all the data has been sent
     * NOTE - Start the status stage immediately
     * after writing the last packet. Do not wait
     * for the next IN event, as this can be dropped
     * if the ACK by the host is corrupted.
     *
     * For more info on this see section
     * 8.5.3.2 of the USB2.0 specification.
     */
    if ((transfer.remaining == 0) && !transfer.zlp)
    {
        /* Transfer completed */
        if (transfer.notify)
        {
            /* Notify class layer. */
            transfer.notify = false;
            callback_request_xfer_done(&transfer.setup);
        }
        else
        {
            complete_request_xfer_done(true);
        }


        /* Completed */
        return true;
    }


    return true;
}

void USBDevice::complete_request_xfer_done(bool success)
{
    lock();

    if (!success) {
        phy->ep0_stall();
        unlock();
        return;
    }

    /* Status stage */
    if (transfer.stage == CTRL_STAGE_DATA_OUT) {
        transfer.stage = CTRL_STAGE_STATUS;
        phy->ep0_write(NULL, 0);
    }  else if (transfer.stage == CTRL_STAGE_DATA_IN) {
        transfer.stage = CTRL_STAGE_STATUS;
        phy->ep0_read();
    }

    unlock();
}

bool USBDevice::request_set_address(void)
{
    assert_locked();

    /* Set the device address */
    phy->set_address(transfer.setup.wValue);

    if (transfer.setup.wValue == 0)
    {
        device.state = DEFAULT;
    }
    else
    {
        device.state = ADDRESS;
    }

    return true;
}

bool USBDevice::request_set_configuration(void)
{
    assert_locked();

    device.configuration = transfer.setup.wValue;
    /* Set the device configuration */
    if (device.configuration == 0)
    {
        /* Not configured */
        phy->unconfigure();
        device.state = ADDRESS;
    }
    else
    {
        transfer.user_callback = true;
        callback_set_configuration(device.configuration);
    }

    return true;
}

void USBDevice::complete_set_configuration(bool success)
{
    lock();

    if (success) {
        /* Valid configuration */
        phy->configure();
        device.state = CONFIGURED;
    }
    complete_request(success);

    unlock();
}

bool USBDevice::request_get_configuration(void)
{
    assert_locked();

    /* Send the device configuration */
    transfer.ptr = &device.configuration;
    transfer.remaining = sizeof(device.configuration);
    transfer.direction = SEND;
    return true;
}

bool USBDevice::request_get_interface(void)
{
    assert_locked();

    /* Return the selected alternate setting for an interface */

    if (device.state != CONFIGURED)
    {
        return false;
    }

    /* Send the alternate setting */
    transfer.setup.wIndex = current_interface;
    transfer.ptr = &current_alternate;
    transfer.remaining = sizeof(current_alternate);
    transfer.direction = SEND;
    return true;
}

bool USBDevice::request_set_interface(void)
{
    assert_locked();

    transfer.user_callback = true;
    callback_set_interface(transfer.setup.wIndex, transfer.setup.wValue);
    return true;
}

void USBDevice::complete_set_interface(bool success)
{
    lock();

    if (success) {
        current_interface = transfer.setup.wIndex;
        current_alternate = transfer.setup.wValue;
    }
    complete_request(success);

    unlock();
}

bool USBDevice::request_set_feature()
{
    assert_locked();
    bool success = false;

    if (device.state != CONFIGURED)
    {
        /* Endpoint or interface must be zero */
        if (transfer.setup.wIndex != 0)
        {
            return false;
        }
    }

    switch (transfer.setup.bmRequestType.Recipient)
    {
        case DEVICE_RECIPIENT:
            /* TODO: Remote wakeup feature not supported */
            break;
        case ENDPOINT_RECIPIENT:
            if (!EP_VALID(transfer.setup.wIndex)) {
                break;
            } else if (transfer.setup.wValue == ENDPOINT_HALT) {
                phy->endpoint_stall(transfer.setup.wIndex);
                success = true;
            }
            break;
        default:
            break;
    }

    return success;
}

bool USBDevice::request_clear_feature()
{
    assert_locked();

    bool success = false;

    if (device.state != CONFIGURED)
    {
        /* Endpoint or interface must be zero */
        if (transfer.setup.wIndex != 0)
        {
            return false;
        }
    }

    switch (transfer.setup.bmRequestType.Recipient)
    {
        case DEVICE_RECIPIENT:
            /* TODO: Remote wakeup feature not supported */
            break;
        case ENDPOINT_RECIPIENT:
            if (!EP_VALID(transfer.setup.wIndex)) {
                break;
            } else if (transfer.setup.wValue == ENDPOINT_HALT) {
                phy->endpoint_unstall(transfer.setup.wIndex);
                success = true;
            }
            break;
        default:
            break;
    }

    return success;
}

bool USBDevice::request_get_status(void)
{
    assert_locked();

    static uint16_t status;
    bool success = false;

    if (device.state != CONFIGURED)
    {
        /* Endpoint or interface must be zero */
        if (transfer.setup.wIndex != 0)
        {
            return false;
        }
    }

    switch (transfer.setup.bmRequestType.Recipient)
    {
        case DEVICE_RECIPIENT:
            /* TODO: Currently only supports self powered devices */
            status = DEVICE_STATUS_SELF_POWERED;
            success = true;
            break;
        case INTERFACE_RECIPIENT:
            status = 0;
            success = true;
            break;
        case ENDPOINT_RECIPIENT:
            if (!EP_VALID(transfer.setup.wIndex)) {
                break;
            } else if (EP_CONTROL(transfer.setup.wIndex)) {
                /* Control endpoint can't be halted */
                status = 0;
            } else if (endpoint_info[EP_TO_INDEX(transfer.setup.wIndex & 0xFF)].flags & ENDPOINT_STALLED)
            {
                status = ENDPOINT_STATUS_HALT;
            }
            else
            {
                status = 0;
            }
            success = true;
            break;
        default:
            break;
    }

    if (success)
    {
        /* Send the status */
        transfer.ptr = (uint8_t *)&status; /* Assumes little endian */
        transfer.remaining = sizeof(status);
        transfer.direction = SEND;
    }

    return success;
}

bool USBDevice::request_setup(void)
{
    assert_locked();

    bool success = false;

    /* Process standard requests */
    if ((transfer.setup.bmRequestType.Type == STANDARD_TYPE))
    {
        switch (transfer.setup.bRequest)
        {
             case GET_STATUS:
                 success = request_get_status();
                 break;
             case CLEAR_FEATURE:
                 success = request_clear_feature();
                 break;
             case SET_FEATURE:
                 success = request_set_feature();
                 break;
             case SET_ADDRESS:
                success = request_set_address();
                 break;
             case GET_DESCRIPTOR:
                 success = request_get_descriptor();
                 break;
             case SET_DESCRIPTOR:
                 /* TODO: Support is optional, not implemented here */
                 success = false;
                 break;
             case GET_CONFIGURATION:
                 success = request_get_configuration();
                 break;
             case SET_CONFIGURATION:
                 success = request_set_configuration();
                 break;
             case GET_INTERFACE:
                 success = request_get_interface();
                 break;
             case SET_INTERFACE:
                 success = request_set_interface();
                 break;
             default:
                 break;
        }
    }

    return success;
}

bool USBDevice::control_setup(void)
{
    assert_locked();

    /* Control transfer setup stage */
    uint8_t buffer[MAX_PACKET_SIZE_EP0];

    phy->ep0_setup_read_result(buffer, MAX_PACKET_SIZE_EP0);

    /* Initialise control transfer state */
    decode_setup_packet(buffer, &transfer.setup);
    transfer.ptr = NULL;
    transfer.remaining = 0;
    transfer.direction = 0;
    transfer.zlp = false;
    transfer.notify = false;
    transfer.stage = CTRL_STAGE_SETUP;

#ifdef DEBUG
    printf("dataTransferDirection: %d\r\nType: %d\r\nRecipient: %d\r\nbRequest: %d\r\nwValue: %d\r\nwIndex: %d\r\nwLength: %d\r\n",transfer.setup.bmRequestType.dataTransferDirection,
                                                                                                                                   transfer.setup.bmRequestType.Type,
                                                                                                                                   transfer.setup.bmRequestType.Recipient,
                                                                                                                                   transfer.setup.bRequest,
                                                                                                                                   transfer.setup.wValue,
                                                                                                                                   transfer.setup.wIndex,
                                                                                                                                   transfer.setup.wLength);
#endif

    /* Class / vendor specific */
    transfer.user_callback = callback_request(&transfer.setup);

    if (!transfer.user_callback)
    {
        /* Standard requests */
        if (!request_setup())
        {
#ifdef DEBUG
            printf("fail!!!!\r\n");
#endif
            return false;
        }
    }

    if (!transfer.user_callback) {
        control_setup_continue();
    }
    return true;
}
void USBDevice::complete_request(bool success)
{
    lock();

    if (success) {
        control_setup_continue();
    } else {
        phy->ep0_stall();
    }

    unlock();
}

void USBDevice::complete_request(uint8_t *data, uint32_t size, Direction direction)
{
    lock();

    transfer.remaining = size;
    transfer.ptr = data;
    transfer.direction = direction;
    control_setup_continue();

    unlock();
}

void USBDevice::control_setup_continue()
{
    assert_locked();

    /* Check transfer size and direction */
    if (transfer.setup.wLength>0)
    {
        if (transfer.setup.bmRequestType.dataTransferDirection \
            == SEND)
        {
            /* IN data stage is required */
            if (transfer.direction != SEND)
            {
                phy->ep0_stall();
                return;
            }

            /* Transfer must be less than or equal to the size */
            /* requested by the host */
            if (transfer.remaining > transfer.setup.wLength)
            {
                transfer.remaining = transfer.setup.wLength;
            }
        }
        else
        {

            /* OUT data stage is required */
            if (transfer.direction != RECEIVE)
            {
                phy->ep0_stall();
                return;
            }

            /* Transfer must be equal to the size requested by the host */
            if (transfer.remaining != transfer.setup.wLength)
            {
                phy->ep0_stall();
                return;
            }
        }
    }
    else
    {
        /* No data stage; transfer size must be zero */
        if (transfer.remaining != 0)
        {
            phy->ep0_stall();
            return;
        }
    }

    /* Data or status stage if applicable */
    if (transfer.setup.wLength>0)
    {
        if (transfer.setup.bmRequestType.dataTransferDirection \
            == SEND)
        {
            /* Check if we'll need to send a zero length packet at */
            /* the end of this transfer */
            if (transfer.setup.wLength > transfer.remaining)
            {
                /* Device wishes to transfer less than host requested */
                if ((transfer.remaining % MAX_PACKET_SIZE_EP0) == 0)
                {
                    /* Transfer is a multiple of EP0 max packet size */
                    transfer.zlp = true;
                }
            }

            /* IN stage */
            transfer.stage = CTRL_STAGE_DATA_IN;
            control_in();
        }
        else
        {
            /* OUT stage */
            transfer.stage = CTRL_STAGE_DATA_OUT;
            phy->ep0_read();
        }
    }
    else
    {
        /* Status stage */
        transfer.stage = CTRL_STAGE_STATUS;
        phy->ep0_write(NULL, 0);
    }
}

void USBDevice::reset(void)
{
    lock();

    /* Call class / vendor specific busReset function */
    callback_reset();

    unlock();
}

void USBDevice::complete_reset()
{
    lock();

    memset(endpoint_info, 0, sizeof(endpoint_info));
    device.state = DEFAULT;
    device.configuration = 0;
    device.suspended = false;

    unlock();
}

void USBDevice::ep0_setup(void)
{
    assert_locked();

    /* Endpoint 0 setup event */
    if (!control_setup())
    {
        /* Protocol stall */
        phy->ep0_stall();
    }

    /* Return true if an OUT data stage is expected */
}

void USBDevice::ep0_out(void)
{
    assert_locked();

    if (transfer.stage == CTRL_STAGE_STATUS) {
        // No action needed on status stage
        return;
    }

    /* Endpoint 0 OUT data event */
    if (!control_out())
    {
        /* Protocol stall; this will stall both endpoints */
        phy->ep0_stall();
    }
}

void USBDevice::ep0_in(void)
{
    assert_locked();

#ifdef DEBUG
    printf("ep0_in\r\n");
#endif
    if (transfer.stage == CTRL_STAGE_STATUS) {
        // No action needed on status stage
        return;
    }

    /* Endpoint 0 IN data event */
    if (!control_in())
    {
        /* Protocol stall; this will stall both endpoints */
        phy->ep0_stall();
    }
}

void USBDevice::out(usb_ep_t endpoint)
{
    assert_locked();
    if (endpoint_info[EP_TO_INDEX(endpoint)].callback) {
        (this->*(endpoint_info[EP_TO_INDEX(endpoint)].callback))(endpoint);
    }
}

void USBDevice::in(usb_ep_t endpoint)
{
    assert_locked();
    if (endpoint_info[EP_TO_INDEX(endpoint)].callback) {
        (this->*(endpoint_info[EP_TO_INDEX(endpoint)].callback))(endpoint);
    }
}

bool USBDevice::configured(void)
{
    lock();

    /* Returns true if device is in the CONFIGURED state */
    bool ret =  (device.state == CONFIGURED);

    unlock();
    return ret;
}

void USBDevice::connect(bool blocking)
{
    /* Connect device */
    lock();
    phy->connect();
    unlock();

    if (blocking) {
        /* Block if not configured */
        while (!configured());
    }
}

void USBDevice::disconnect(void)
{
    lock();

    /* Disconnect device */
    phy->disconnect();
    
    /* Set initial device state */
    device.state = POWERED;
    device.configuration = 0;
    device.suspended = false;

    unlock();
}

bool USBDevice::endpoint_add(usb_ep_t endpoint, uint32_t max_packet_size, usb_ep_type_t type, ep_cb_t callback)
{
    lock();

    endpoint_info_t *info = &endpoint_info[EP_TO_INDEX(endpoint)];
    MBED_ASSERT(!(info->flags & ENDPOINT_ENABLED));
    MBED_ASSERT(max_packet_size <= 1024);

    bool ret = false;
    if (phy->endpoint_add(endpoint, max_packet_size, type)) {
        info->callback = callback;
        info->flags |= ENDPOINT_ENABLED;
        info->pending = 0;
        info->max_packet_size = max_packet_size;
        ret = true;
    }

    unlock();
    return ret;
}

void USBDevice::endpoint_remove(usb_ep_t endpoint)
{
    lock();

    endpoint_info_t *info = &endpoint_info[EP_TO_INDEX(endpoint)];
    MBED_ASSERT(info->flags & ENDPOINT_ENABLED);

    info->callback = NULL;
    info->flags = 0;
    info->pending = 0;
    info->max_packet_size = 0;

    phy->endpoint_remove(endpoint);

    unlock();
}

void USBDevice::endpoint_stall(usb_ep_t endpoint)
{
    lock();

    endpoint_info_t *info = &endpoint_info[EP_TO_INDEX(endpoint)];
    MBED_ASSERT(info->flags & ENDPOINT_ENABLED);

    info->flags |= ENDPOINT_STALLED;
    phy->endpoint_stall(endpoint);

    unlock();
}

void USBDevice::endpoint_unstall(usb_ep_t endpoint)
{
    lock();

    endpoint_info_t *info = &endpoint_info[EP_TO_INDEX(endpoint)];
    MBED_ASSERT(info->flags & ENDPOINT_ENABLED);

    info->flags &= ~ENDPOINT_STALLED;
    phy->endpoint_unstall(endpoint);

    unlock();
}

uint8_t * USBDevice::find_descriptor(uint8_t descriptorType)
{
    /* Find a descriptor within the list of descriptors */
    /* following a configuration descriptor. */
    uint16_t wTotalLength;
    uint8_t *ptr;

    if (configuration_desc() == NULL)
    {
        return NULL;
    }

    /* Check this is a configuration descriptor */
    if ((configuration_desc()[0] != CONFIGURATION_DESCRIPTOR_LENGTH) \
            || (configuration_desc()[1] != CONFIGURATION_DESCRIPTOR))
    {
        return NULL;
    }

    wTotalLength = configuration_desc()[2] | (configuration_desc()[3] << 8);

    /* Check there are some more descriptors to follow */
    if (wTotalLength <= (CONFIGURATION_DESCRIPTOR_LENGTH+2))
    /* +2 is for bLength and bDescriptorType of next descriptor */
    {
        return NULL;
    }

    /* Start at first descriptor after the configuration descriptor */
    ptr = &(((uint8_t*)configuration_desc())[CONFIGURATION_DESCRIPTOR_LENGTH]);

    do {
        if (ptr[1] /* bDescriptorType */ == descriptorType)
        {
            /* Found */
            return ptr;
        }

        /* Skip to next descriptor */
        ptr += ptr[0]; /* bLength */
    } while (ptr < (configuration_desc() + wTotalLength));

    /* Reached end of the descriptors - not found */
    return NULL;
}

const usb_ep_table_t *USBDevice::endpoint_table()
{
    return phy->endpoint_table();
}

void USBDevice::suspend(bool suspended)
{
}


USBDevice::USBDevice(USBPhy *phy, uint16_t vendor_id, uint16_t product_id, uint16_t product_release) {
    this->vendor_id = vendor_id;
    this->product_id = product_id;
    this->product_release = product_release;

    memset(endpoint_info, 0, sizeof(endpoint_info));

    this->phy = phy;
    current_interface = 0;
    current_alternate = 0;
    locked = 0;

    /* Set initial device state */
    device.state = POWERED;
    device.configuration = 0;
    device.suspended = false;

    this->phy->init(this);
    max_packet_size_ep0 = this->phy->ep0_set_max_packet(MAX_PACKET_SIZE_EP0);
};

USBDevice::USBDevice(uint16_t vendor_id, uint16_t product_id, uint16_t product_release) {
    this->vendor_id = vendor_id;
    this->product_id = product_id;
    this->product_release = product_release;

    memset(endpoint_info, 0, sizeof(endpoint_info));

    this->phy = get_usb_phy();
    current_interface = 0;
    current_alternate = 0;
    locked = 0;

    /* Set initial device state */
    device.state = POWERED;
    device.configuration = 0;
    device.suspended = false;

    this->phy->init(this);
    max_packet_size_ep0 = this->phy->ep0_set_max_packet(MAX_PACKET_SIZE_EP0);
};

uint32_t USBDevice::endpoint_max_packet_size(usb_ep_t endpoint)
{
    lock();

    uint32_t size = 0;
    if (EP_CONTROL(endpoint)) {
        size = max_packet_size_ep0;
    } else {
        endpoint_info_t *info = &endpoint_info[EP_TO_INDEX(endpoint)];
        size = info->max_packet_size;
    }

    unlock();
    return size;
}

bool USBDevice::read_start(usb_ep_t endpoint)
{
    lock();

    endpoint_info_t *info = &endpoint_info[EP_TO_INDEX(endpoint)];
    MBED_ASSERT(info->flags & ENDPOINT_ENABLED);

    bool ret = phy->endpoint_read(endpoint, info->max_packet_size);
    if (ret) {
        info->pending += 1;
        ret = true;
    }

    unlock();
    return ret;
}

bool USBDevice::read_finish(usb_ep_t endpoint, uint8_t *buffer, uint32_t max_size, uint32_t *size)
{
    lock();

    endpoint_info_t *info = &endpoint_info[EP_TO_INDEX(endpoint)];
    MBED_ASSERT(info->flags & ENDPOINT_ENABLED);

    if (max_size < info->max_packet_size) {
        MBED_ASSERT(0);
        unlock();
        return false;
    }

    if(!configured()) {
        unlock();
        return false;
    }

    bool ret = phy->endpoint_read_result(endpoint, buffer, max_size, size);
    if (ret) {
        info->pending -= 1;
    }

    unlock();
    return ret;
}



bool USBDevice::write(usb_ep_t endpoint, uint8_t * buffer, uint32_t size)
{
    lock();

    endpoint_info_t *info = &endpoint_info[EP_TO_INDEX(endpoint)];
    MBED_ASSERT(info->flags & ENDPOINT_ENABLED);

    if (size > info->max_packet_size) {
        unlock();
        return false;
    }


    if(!configured()) {
        unlock();
        return false;
    }

    /* Send report */
    bool ret = phy->endpoint_write(endpoint, buffer, size);
    if (ret) {
        info->pending += 1;
    }

    unlock();
    return ret;
}

const uint8_t * USBDevice::device_desc() {
    uint8_t device_descriptor_temp[] = {
        DEVICE_DESCRIPTOR_LENGTH,       /* bLength */
        DEVICE_DESCRIPTOR,              /* bDescriptorType */
        LSB(USB_VERSION_2_0),           /* bcdUSB (LSB) */
        MSB(USB_VERSION_2_0),           /* bcdUSB (MSB) */
        0x00,                           /* bDeviceClass */
        0x00,                           /* bDeviceSubClass */
        0x00,                           /* bDeviceprotocol */
        (uint8_t)max_packet_size_ep0,   /* bMaxPacketSize0 */
        (uint8_t)(LSB(vendor_id)),                 /* idVendor (LSB) */
        (uint8_t)(MSB(vendor_id)),                 /* idVendor (MSB) */
        (uint8_t)(LSB(product_id)),                /* idProduct (LSB) */
        (uint8_t)(MSB(product_id)),                /* idProduct (MSB) */
        (uint8_t)(LSB(product_release)),           /* bcdDevice (LSB) */
        (uint8_t)(MSB(product_release)),           /* bcdDevice (MSB) */
        STRING_OFFSET_IMANUFACTURER,    /* iManufacturer */
        STRING_OFFSET_IPRODUCT,         /* iProduct */
        STRING_OFFSET_ISERIAL,          /* iSerialNumber */
        0x01                            /* bNumConfigurations */
    };
    MBED_ASSERT(sizeof(device_descriptor_temp) == sizeof(device_descriptor));
    memcpy(device_descriptor, device_descriptor_temp, sizeof(device_descriptor));
    return device_descriptor;
}

const uint8_t * USBDevice::string_langid_desc() {
    static const uint8_t string_langid_descriptor[] = {
        0x04,               /*bLength*/
        STRING_DESCRIPTOR,  /*bDescriptorType 0x03*/
        0x09,0x04,          /*bString Lang ID - 0x0409 - English*/
    };
    return string_langid_descriptor;
}

const uint8_t * USBDevice::string_imanufacturer_desc() {
    static const uint8_t string_imanufacturer_descriptor[] = {
        0x12,                                            /*bLength*/
        STRING_DESCRIPTOR,                               /*bDescriptorType 0x03*/
        'm',0,'b',0,'e',0,'d',0,'.',0,'o',0,'r',0,'g',0, /*bString iManufacturer - mbed.org*/
    };
    return string_imanufacturer_descriptor;
}

const uint8_t * USBDevice::string_iserial_desc() {
    static const uint8_t string_iserial_descriptor[] = {
        0x16,                                                           /*bLength*/
        STRING_DESCRIPTOR,                                              /*bDescriptorType 0x03*/
        '0',0,'1',0,'2',0,'3',0,'4',0,'5',0,'6',0,'7',0,'8',0,'9',0,    /*bString iSerial - 0123456789*/
    };
    return string_iserial_descriptor;
}

const uint8_t * USBDevice::string_iconfiguration_desc() {
    static const uint8_t string_iconfiguration_descriptor[] = {
        0x06,               /*bLength*/
        STRING_DESCRIPTOR,  /*bDescriptorType 0x03*/
        '0',0,'1',0,        /*bString iConfiguration - 01*/
    };
    return string_iconfiguration_descriptor;
}

const uint8_t * USBDevice::string_iinterface_desc() {
    static const uint8_t string_iinterface_descriptor[] = {
        0x08,               /*bLength*/
        STRING_DESCRIPTOR,  /*bDescriptorType 0x03*/
        'U',0,'S',0,'B',0,  /*bString iInterface - USB*/
    };
    return string_iinterface_descriptor;
}

const uint8_t * USBDevice::string_iproduct_desc() {
    static const uint8_t string_iproduct_descriptor[] = {
        0x16,                                                       /*bLength*/
        STRING_DESCRIPTOR,                                          /*bDescriptorType 0x03*/
        'U',0,'S',0,'B',0,' ',0,'D',0,'E',0,'V',0,'I',0,'C',0,'E',0 /*bString iProduct - USB DEVICE*/
    };
    return string_iproduct_descriptor;
}

void USBDevice::start_process() {
    lock();

    phy->process();

    unlock();
}
