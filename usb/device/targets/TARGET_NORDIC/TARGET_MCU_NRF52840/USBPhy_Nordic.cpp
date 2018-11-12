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

#include "USBPhyHw.h"

#define MAX_PACKET_SIZE_SETUP NRF_DRV_USBD_EPSIZE
#define MAX_PACKET_NON_ISO    NRF_DRV_USBD_EPSIZE
#define MAX_PACKET_ISO        NRF_DRV_USBD_ISOSIZE
#define ENDPOINT_NON_ISO      (USB_EP_ATTR_ALLOW_BULK | USB_EP_ATTR_ALLOW_INT)

#define IS_IN_EP(ep) 	(ep & 0x80)  // Checks if the given endpoint is an IN endpoint (MSB set)
#define IS_OUT_EP(ep) 	(ep & ~0x80) // Checks if the given endpoint is an OUT endpoint (MSB clear)

static USBPhyHw *instance;

static void usbd_event_handler(nrf_drv_usbd_evt_t const * const p_event);
static void power_usb_event_handler(nrf_drv_power_usb_evt_t event);

USBPhy *get_usb_phy() {
	static USBPhyHw usbphy;
	return &usbphy;
}

USBPhyHw::USBPhyHw() :
		events(NULL), sof_enabled(false),
		connect_enabled(false),
		usb_event_type(USB_HW_EVENT_NONE),
		usb_power_event(NRF_DRV_POWER_USB_EVT_REMOVED) {

}

USBPhyHw::~USBPhyHw() {

}

void USBPhyHw::init(USBPhyEvents *events) {
	// Disable IRQ
	//NVIC_DisableIRQ(USBD_IRQn);

	this->events = events;

	ret_code_t ret;

	// Initialize power module to track USB Power events
	ret = nrf_drv_power_init(NULL);
	APP_ERROR_CHECK(ret);

	// Register callback for USB Power events
	static const nrf_drv_power_usbevt_config_t config = { .handler =
			power_usb_event_handler };
	ret = nrf_drv_power_usbevt_init(&config);
	APP_ERROR_CHECK(ret);

	// Initialize USB Device driver
	ret = nrf_drv_usbd_init(usbd_event_handler);
	APP_ERROR_CHECK(ret);

	instance = this;

	// Enable IRQ
	//NVIC_SetVector(USBD_IRQn, (uint32_t)&_usbisr);
	//NVIC_EnableIRQ(USBD_IRQn);
}

void USBPhyHw::deinit() {
	// Disconnect and disable interrupt
	disconnect();
	// Disable the USB Device driver
	ret_code_t ret = nrf_drv_usbd_uninit();
	APP_ERROR_CHECK(ret);
	//NVIC_DisableIRQ(USBD_IRQn);
}

bool USBPhyHw::powered() {
	if (nrf_drv_power_usbstatus_get() == NRF_DRV_POWER_USB_STATE_CONNECTED
			|| nrf_drv_power_usbstatus_get() == NRF_DRV_POWER_USB_STATE_READY)
		return true;
	else
		return false;
}

void USBPhyHw::connect() {
	connect_enabled = true;
	//nrf_drv_usbd_start(true);
}

void USBPhyHw::disconnect() {
	connect_enabled = false;
	//nrf_drv_usbd_stop();
}

void USBPhyHw::configure() {
	// Not needed
}

void USBPhyHw::unconfigure() {
	// Not needed
}

void USBPhyHw::sof_enable() {
	// TODO - Enable SOF interrupt
	// Can this safely be done if
	// nrf_drv_usbd_start is called with SoF enabled?
	sof_enabled = true;
}

void USBPhyHw::sof_disable() {
	// TODO - Disable SOF interrupt
	// Can this safely be done if
	// nrf_drv_usbd_start is called with SoF enabled?
	sof_enabled = false;
}

void USBPhyHw::set_address(uint8_t address) {
	// nothing to do, handled by hardware; but don't STALL
}

void USBPhyHw::remote_wakeup() {
	// Not supported(?)
}

const usb_ep_table_t *USBPhyHw::endpoint_table() {

	static const usb_ep_table_t template_table =
	{
		1536, // 64 bytes per bulk/int endpoint pair (8), 1023 bytes for iso endpoint pair (1)
		{
			{ USB_EP_ATTR_ALLOW_CTRL	| USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0 },
			{ ENDPOINT_NON_ISO 			| USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0 },
			{ ENDPOINT_NON_ISO 			| USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0 },
			{ ENDPOINT_NON_ISO 			| USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0 },
			{ ENDPOINT_NON_ISO			| USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0 },
			{ ENDPOINT_NON_ISO 			| USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0 },
			{ ENDPOINT_NON_ISO 			| USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0 },
			{ ENDPOINT_NON_ISO 			| USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0 },
			{ USB_EP_ATTR_ALLOW_ISO 	| USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0 },
			{ 0 						| USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0 },
			{ 0							| USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0 },
			{ 0							| USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0 },
			{ 0 						| USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0 },
			{ 0							| USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0 },
			{ 0							| USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0 },
			{ 0							| USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0 },
		}
	};
	return &template_table;
}

uint32_t USBPhyHw::ep0_set_max_packet(uint32_t max_packet) {
	if (max_packet > MAX_PACKET_SIZE_SETUP)
		max_packet = MAX_PACKET_SIZE_SETUP;

	nrf_drv_usbd_ep_max_packet_size_set(NRF_DRV_USBD_EPOUT0, max_packet);
	nrf_drv_usbd_ep_max_packet_size_set(NRF_DRV_USBD_EPIN0, max_packet);
	return max_packet;
}

// read setup packet
void USBPhyHw::ep0_setup_read_result(uint8_t *buffer, uint32_t size) {
	if (size > sizeof(this->setup_buf)) {
		size = sizeof(this->setup_buf);
	}
	memcpy(buffer, &this->setup_buf, size);
}

void USBPhyHw::ep0_read(uint8_t *data, uint32_t size) {
	nrf_drv_usbd_transfer_t* transfer = get_transfer_buffer((usb_ep_t)(NRF_DRV_USBD_EPOUT0));
	memset(transfer, 0, sizeof(nrf_drv_usbd_transfer_t));
	transfer->p_data.rx = data;
	transfer->size = size;
	nrf_drv_usbd_ep_transfer(NRF_DRV_USBD_EPOUT0, transfer);
}

uint32_t USBPhyHw::ep0_read_result() {
	return nrf_drv_usbd_epout_size_get(NRF_DRV_USBD_EPOUT0);
}

void USBPhyHw::ep0_write(uint8_t *buffer, uint32_t size) {
	nrf_drv_usbd_transfer_t* transfer = get_transfer_buffer((usb_ep_t)(NRF_DRV_USBD_EPIN0));
	memset(transfer, 0, sizeof(nrf_drv_usbd_transfer_t));
	transfer->p_data.tx = buffer;
	transfer->size = size;
	nrf_drv_usbd_ep_transfer(NRF_DRV_USBD_EPIN0, transfer);
}

void USBPhyHw::ep0_stall() {
	// Note: This stall must be automatically cleared by the next setup packet
	// TODO: Check if this is actually happening
	nrf_drv_usbd_setup_stall();
}

bool USBPhyHw::endpoint_add(usb_ep_t endpoint, uint32_t max_packet, usb_ep_type_t type) {
	nrf_drv_usbd_ep_t nrf_ep = get_nordic_endpoint(endpoint);
	nrf_drv_usbd_ep_enable(nrf_ep);
	nrf_drv_usbd_ep_max_packet_size_set(nrf_ep, max_packet);
	return nrf_drv_usbd_ep_enable_check(nrf_ep);
}

void USBPhyHw::endpoint_remove(usb_ep_t endpoint) {
	nrf_drv_usbd_ep_t nrf_ep = get_nordic_endpoint(endpoint);
	nrf_drv_usbd_ep_disable(nrf_ep);
}

void USBPhyHw::endpoint_stall(usb_ep_t endpoint) {
	nrf_drv_usbd_ep_stall(get_nordic_endpoint(endpoint));
}

void USBPhyHw::endpoint_unstall(usb_ep_t endpoint) {
	nrf_drv_usbd_ep_stall_clear(get_nordic_endpoint(endpoint));
}

bool USBPhyHw::endpoint_read(usb_ep_t endpoint, uint8_t *data, uint32_t size) {
	nrf_drv_usbd_transfer_t* transfer = get_transfer_buffer(endpoint);
	memset(transfer, 0, sizeof(nrf_drv_usbd_transfer_t));
	transfer->p_data.rx = data;
	transfer->size = size;
	ret_code_t ret = nrf_drv_usbd_ep_transfer(get_nordic_endpoint(endpoint), transfer);
	return (ret == NRF_SUCCESS);
}

uint32_t USBPhyHw::endpoint_read_result(usb_ep_t endpoint) {
	return nrf_drv_usbd_epout_size_get(get_nordic_endpoint(endpoint));
}

bool USBPhyHw::endpoint_write(usb_ep_t endpoint, uint8_t *data, uint32_t size) {
	nrf_drv_usbd_transfer_t* transfer = get_transfer_buffer(endpoint);
	memset(transfer, 0, sizeof(nrf_drv_usbd_transfer_t));
	transfer->p_data.tx = data;
	transfer->size = size;
	ret_code_t ret = nrf_drv_usbd_ep_transfer(get_nordic_endpoint(endpoint), transfer);
	return (ret == NRF_SUCCESS);
}

void USBPhyHw::endpoint_abort(usb_ep_t endpoint) {
	nrf_drv_usbd_ep_abort(get_nordic_endpoint(endpoint));
}

void USBPhyHw::process() {
	// No valid event to process
	if (usb_event_type == USB_HW_EVENT_NONE)
		return;
	else if (usb_event_type == USB_HW_EVENT_USBD) {
		// Process regular USBD events
		switch (usb_event.type) {
		case NRF_DRV_USBD_EVT_SUSPEND:
			events->suspend(true);
			break;
		case NRF_DRV_USBD_EVT_RESUME:
			events->suspend(false);
			break;
		case NRF_DRV_USBD_EVT_WUREQ:
			break;
		case NRF_DRV_USBD_EVT_RESET:
			events->reset();
			break;
		case NRF_DRV_USBD_EVT_SOF:
			if(sof_enabled)
				events->sof(nrf_usbd_framecntr_get());
			break;
		case NRF_DRV_USBD_EVT_EPTRANSFER:
			if(usb_event.data.eptransfer.status == NRF_USBD_EP_OK)
			{
				if(IS_IN_EP(usb_event.data.eptransfer.ep))
				{
					if((usb_event.data.eptransfer.ep & 0x7F) == 0)
						events->ep0_in();
					else
						events->in((usb_ep_t) usb_event.data.eptransfer.ep);
				}
				else
				{
					if((usb_event.data.eptransfer.ep & 0x7F) == 0)
						events->ep0_out();
					else
						events->out((usb_ep_t) usb_event.data.eptransfer.ep);
				}
			}
			break;
		case NRF_DRV_USBD_EVT_SETUP: {
			// Copy the setup packet into the internal buffer
			nrf_drv_usbd_setup_get(&setup_buf);
			events->ep0_setup();
		}
			break;
		default:
			break;

			// Re-enable interrupt
			//NVIC_ClearPendingIRQ(USB_IRQn);
			//NVIC_EnableIRQ(USB_IRQn);
		}
	}
	else if (usb_event_type == USB_HW_EVENT_POWER)
	{
		// Process USB power-related events
		switch (usb_power_event) {
		case NRF_DRV_POWER_USB_EVT_DETECTED:
			if(!nrf_drv_usbd_is_enabled())
				nrf_drv_usbd_enable();
			events->power(true);
			break;
		case NRF_DRV_POWER_USB_EVT_REMOVED:
			events->power(false);
			if(nrf_drv_usbd_is_started())
				nrf_drv_usbd_stop();
			if(nrf_drv_usbd_is_enabled())
				nrf_drv_usbd_disable();
			break;
		case NRF_DRV_POWER_USB_EVT_READY:
			if(!nrf_drv_usbd_is_started() && connect_enabled)
				nrf_drv_usbd_start(true);
			break;
		default:
			ASSERT(false)
			;
		}
	}

	// Unflag the event type
	usb_event_type = USB_HW_EVENT_NONE;

}

void USBPhyHw::_usb_event_handler(
		nrf_drv_usbd_evt_t const * const p_event) {
	// Copy the event data into internal memory
	memcpy(&instance->usb_event, p_event, sizeof(instance->usb_event));
	// Tell the upper layers of the stack to process the event
	instance->usb_event_type = USB_HW_EVENT_USBD;
	instance->events->start_process();
}

void USBPhyHw::_usb_power_event_handler(nrf_drv_power_usb_evt_t event) {
	// Copy the event data into internal memory
	instance->usb_power_event = event;
	// Tell the upper layers of the stack to process the event
	instance->usb_event_type = USB_HW_EVENT_POWER;
	instance->events->start_process();
}

nrf_drv_usbd_transfer_t* USBPhyHw::get_transfer_buffer(usb_ep_t endpoint) {
	// Index is base endpoint number * 2 (output), add 1 for input endpoints
	return &transfer_buf[(((endpoint & 0x7F) << 1) + ((endpoint & 0x80) >> 7))];
}

nrf_drv_usbd_ep_t USBPhyHw::get_nordic_endpoint(usb_ep_t endpoint) {
	// Clear the most-significant-bit (input endpoint flag)
	uint8_t endpoint_num = (endpoint & ~(0x80));
	// Make sure it's within the valid endpoint range
	ASSERT(((0 <= endpoint_num) && (endpoint_num <= 8)));
	return (nrf_drv_usbd_ep_t) endpoint;
}

static void power_usb_event_handler(nrf_drv_power_usb_evt_t event) {
	// Pass the event on to the USBPhyHW instance
	instance->_usb_power_event_handler(event);
}

static void usbd_event_handler(nrf_drv_usbd_evt_t const * const p_event) {
	// Pass the event on to the USBPhyHW instance
	instance->_usb_event_handler(p_event);
}
