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

#include "platform/mbed_critical.h"

#include "nrf_clock.h"

/*
 * TODO list for nRF52840 USBD driver
 *
 * 1.) Properly enable/disable start-of-frame interrupt.
 *
 * Description: Currently, start-of-frame interrupts are masked by a flag at this layer
 * but still cause the processor to be interrupted for no purpose.
 *
 * The Nordic driver requires you to call nrf_drv_start(bool)
 * with a boolean flag indicating whether it should enable start-of-frame
 * interrupts or not. From the datasheet it seems to be possible to
 * enable/disable SoF interrupts on the fly, but the fact that they
 * force you to make the SoF decision during "start" makes me suspicious
 * the underlying driver may manage/use the SoF flag in other ways.
 *
 * Next steps: Investigate how the SoF flag is used during "nrf_drv_start" and
 * determine if enabling/disabling this interrupt would cause internal problems
 * with the Nordic USBD driver
 *
 *
 */
#define MAX_PACKET_SIZE_SETUP NRF_DRV_USBD_EPSIZE
#define MAX_PACKET_NON_ISO    NRF_DRV_USBD_EPSIZE
#define MAX_PACKET_ISO        NRF_DRV_USBD_ISOSIZE
#define ENDPOINT_NON_ISO      (USB_EP_ATTR_ALLOW_BULK | USB_EP_ATTR_ALLOW_INT)

#define IS_IN_EP(ep) 	(ep & 0x80)  // Checks if the given endpoint is an IN endpoint (MSB set)
#define IS_OUT_EP(ep) 	(ep & ~0x80) // Checks if the given endpoint is an OUT endpoint (MSB clear)

// If this bit is set in setup.bmRequestType, the setup transfer
// is DEVICE->HOST (IN transfer)
// if it is clear, the transfer is HOST->DEVICE (OUT transfer)
#define SETUP_TRANSFER_DIR_MASK 0x80

// Debugging flag for tracking USB events
#define USBD_DEBUG 0

// Nordic USBD driver IRQ handler
extern "C" void USBD_IRQHandler(void);

static USBPhyHw *instance = 0;

static void usbd_event_handler(nrf_drv_usbd_evt_t const * const p_event);
static void power_usb_event_handler(nrf_drv_power_usb_evt_t event);

#if USBD_DEBUG

// Static array of saved events to track what happens
static nrf_drv_usbd_evt_t debug_events[32];
static uint8_t debug_evt_index = 0;

#endif

USBPhy *get_usb_phy() {
	static USBPhyHw usbphy;
	return &usbphy;
}

USBPhyHw::USBPhyHw() :
		events(NULL), sof_enabled(false), connect_enabled(false),
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

	/* Configure selected size of the packed on EP0 */
	nrf_drv_usbd_ep_max_packet_size_set(NRF_DRV_USBD_EPOUT0, MAX_PACKET_SIZE_SETUP);
	nrf_drv_usbd_ep_max_packet_size_set(NRF_DRV_USBD_EPIN0, MAX_PACKET_SIZE_SETUP);

	// Store a reference to this instance
	instance = this;

	/*
	 * TODO - Configure ISOIN endpoint to respond with ZLP when
	 * no data is ready to be sent
	 *
	 * This is a feature available in the Nordic SDK15.2
	 * For now we just configure the appropriate register on initialization
	 */
	NRF_USBD->ISOINCONFIG |= 0x01; // set RESPONSE to 1 (respond with ZLP)

	// Enable IRQ
	NVIC_SetVector(USBD_IRQn, (uint32_t)USBD_IRQHandler);
	//NVIC_SetPriority(USBD_IRQn, 7);
	//NVIC_EnableIRQ(USBD_IRQn); // This is handled by the Nordic driver
}

void USBPhyHw::deinit() {
	// Disconnect and disable interrupt
	disconnect();
	// Disable the USB Device driver
	ret_code_t ret = nrf_drv_usbd_uninit();
	APP_ERROR_CHECK(ret);
	//NVIC_DisableIRQ(USBD_IRQn); // This is handled by the Nordic driver

	// Disable the power peripheral driver
	nrf_drv_power_uninit();

	// Clear the instance pointer
	instance = 0;
}

bool USBPhyHw::powered() {
	if (nrf_drv_power_usbstatus_get() == NRF_DRV_POWER_USB_STATE_CONNECTED
			|| nrf_drv_power_usbstatus_get() == NRF_DRV_POWER_USB_STATE_READY)
		return true;
	else
		return false;
}

void USBPhyHw::connect() {

	// To save power, we only enable the USBD peripheral
	// when there's actually VBUS detected

	// So flag that the USB stack is ready to connect
	this->connect_enabled = true;

	// If VBUS is already available, enable immediately
	if(nrf_drv_power_usbstatus_get() == NRF_DRV_POWER_USB_STATE_CONNECTED)
	{
		// Enabling USB will cause NRF_DRV_POWER_USB_EVT_READY
		// to occur, which will start the USBD peripheral
		// when the internal regulator has settled
		if(!nrf_drv_usbd_is_enabled())
			nrf_drv_usbd_enable();

		if(nrf_drv_power_usbstatus_get() == NRF_DRV_POWER_USB_STATE_READY
		   && !nrf_drv_usbd_is_started())
				nrf_drv_usbd_start(true);
	}
}

void USBPhyHw::disconnect() {

	this->connect_enabled = false;

	if(nrf_drv_usbd_is_started())
		nrf_drv_usbd_stop();
	if(nrf_drv_usbd_is_enabled())
		nrf_drv_usbd_disable();
}

void USBPhyHw::configure() {
	// Not needed
}

void USBPhyHw::unconfigure() {
	// Remove all endpoints (except control, obviously)
	nrf_drv_usbd_ep_default_config();
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
	disable_usb_interrupts();

	if (max_packet > MAX_PACKET_SIZE_SETUP)
		max_packet = MAX_PACKET_SIZE_SETUP;

	nrf_drv_usbd_ep_max_packet_size_set(NRF_DRV_USBD_EPOUT0, max_packet);
	nrf_drv_usbd_ep_max_packet_size_set(NRF_DRV_USBD_EPIN0, max_packet);

	enable_usb_interrupts();

	return max_packet;
}

// read setup packet
void USBPhyHw::ep0_setup_read_result(uint8_t *buffer, uint32_t size) {

	disable_usb_interrupts();

	if (size > sizeof(this->setup_buf)) {
		size = sizeof(this->setup_buf);
	}
	memcpy(buffer, &this->setup_buf, size);

	enable_usb_interrupts();
}

void USBPhyHw::ep0_read(uint8_t *data, uint32_t size) {
	nrf_drv_usbd_transfer_t* transfer = get_transfer_buffer((usb_ep_t)(NRF_DRV_USBD_EPOUT0));
	memset(transfer, 0, sizeof(nrf_drv_usbd_transfer_t));
	transfer->p_data.rx = data;
	transfer->size = size;

//	if((setup_total == 1) && ((setup_buf.bmRequestType & SETUP_TRANSFER_DIR_MASK) == 0))
//	{
//		setup_remaining -= size;
//	}
//	else
//	{
//	// Update the number of bytes remaining in the setup data stage
//	setup_remaining -= size;
//	}

	setup_remaining -= size;

	nrf_drv_usbd_setup_data_clear(); // tell the hardware to receive another OUT packet

	// Check if this is the last chunk
	if(setup_remaining == 0)
			nrf_usbd_shorts_enable(NRF_USBD_SHORT_EP0DATADONE_EP0STATUS_MASK); // if it is, go to status stage next

	nrf_drv_usbd_ep_transfer(NRF_DRV_USBD_EPOUT0, transfer);
}

uint32_t USBPhyHw::ep0_read_result() {
	return nrf_drv_usbd_epout_size_get(NRF_DRV_USBD_EPOUT0);
}

void USBPhyHw::ep0_write(uint8_t *buffer, uint32_t size) {

	// If the transfer size is 0 and
	// the setup packet request size was 0,
	// enter the status stage immediately
	if(size == 0 && setup_buf.wLength == 0)
	{
		nrf_drv_usbd_setup_clear();
	}

	// TODO - this function should check what stage the control transfer
	// is in before enabling shorts or triggering events.
	// During the status stage ep0_write is called with size=0
	// ZLP arguments. See note below:

	nrf_drv_usbd_transfer_t* transfer = get_transfer_buffer(NRF_DRV_USBD_EPIN0);
	memset(transfer, 0, sizeof(nrf_drv_usbd_transfer_t));
	transfer->p_data.tx = buffer;
	transfer->size = size;

	// If this is a zero-length-packet (ZLP)
	// Set the ZLP flag
	if(size == 0)
		transfer->flags |= NRF_DRV_USBD_TRANSFER_ZLP_FLAG;

	// Update the number of bytes remaining in the setup data stage
	setup_remaining -= size;

	// Check if this is the last chunk, conditions:
	// 1: the remaining bytes will be 0
	// OR 2: the transfer size is < ep max size (including 0, short packet)
	size_t ep_size = nrf_drv_usbd_ep_max_packet_size_get(NRF_DRV_USBD_EPIN0);
	if((setup_remaining == 0) || (size < ep_size))
	{
		// Enter status stage after next DMA transfer completes
		nrf_usbd_shorts_enable(NRF_USBD_SHORT_EP0DATADONE_EP0STATUS_MASK);
	}

	nrf_drv_usbd_ep_transfer(NRF_DRV_USBD_EPIN0, transfer);
}

void USBPhyHw::ep0_stall() {
	// Note: This stall must be automatically cleared by the next setup packet
	// Hardware appears to take care of this
	// See nRF52840 product specification section 6.35.8

	// Update: Above assumption seems incorrect
	// Added in EP0 stall clears	 in the setup packet event handler
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
	// Reset data toggle for bulk/interrupt endpoints
	if(nrf_ep != NRF_DRV_USBD_EPOUT8 && nrf_ep != NRF_DRV_USBD_EPIN8)
		nrf_drv_usbd_ep_dtoggle_clear(nrf_ep);

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

	//if(endpoint == 8)
	//	return true;

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

	// If this is a zero-length-packet (ZLP)
	// Set the ZLP flag
	if(size == 0)
		transfer->flags |= NRF_DRV_USBD_TRANSFER_ZLP_FLAG;

	ret_code_t ret = nrf_drv_usbd_ep_transfer(get_nordic_endpoint(endpoint), transfer);
	return (ret == NRF_SUCCESS);
}

void USBPhyHw::endpoint_abort(usb_ep_t endpoint) {
	nrf_drv_usbd_ep_t nrf_ep = get_nordic_endpoint(endpoint);
	// Don't call abort on ISO endpoints -- this will cause an ASSERT in the Nordic driver
	//if(nrf_ep != NRF_DRV_USBD_EPOUT8 && nrf_ep != NRF_DRV_USBD_EPIN8)
	nrf_drv_usbd_ep_abort(nrf_ep);
}

void USBPhyHw::process() {
	// No valid event to process
	if (usb_event_type == USB_HW_EVENT_NONE)
		return;
	else if (usb_event_type == USB_HW_EVENT_USBD) {

#if USBD_DEBUG
		// Save this event to the static log
		memcpy(&debug_events[debug_evt_index++], &usb_event, sizeof(nrf_drv_usbd_evt_t));
		// Reset index if we overflow the buffer
		if(debug_evt_index >= 32)
			debug_evt_index = 0;
#endif

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
			this->_reset();
			events->reset();
			break;
		case NRF_DRV_USBD_EVT_SOF:
			if(sof_enabled)
				events->sof(usb_event.data.sof.framecnt);
			break;
		case NRF_DRV_USBD_EVT_EPTRANSFER:
			if(usb_event.data.eptransfer.status == NRF_USBD_EP_OK)// ||
			   //usb_event.data.eptransfer.status == NRF_USBD_EP_WAITING)
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
					{
						/* NOTE: Data values or size may be tested here to decide if clear or stall.
						 * If errata 154 is present the data transfer is acknowledged by the hardware. */
//						if (!nrf_drv_usbd_errata_154()) {
//							/* Transfer ok - allow status stage */
//							nrf_drv_usbd_setup_clear();
//						}

						events->ep0_out();
					}
					else
					{
						// TODO - this may impact valid ZLP transfers
						// on the ISOOUT endpoint
						// If ISOOUT endpoint transaction occurred
						// and the ZERO bit of SIZE.ISOOUT is set...
						// ignore it for now... possible hardware bug?
//						if((usb_event.data.eptransfer.ep == NRF_DRV_USBD_EPOUT8)
//						&& (NRF_USBD->SIZE.ISOOUT & (1 << 16)))
//							break;

						events->out((usb_ep_t) usb_event.data.eptransfer.ep);
					}
				}
			}
			break;
		case NRF_DRV_USBD_EVT_SETUP: {
			nrf_drv_usbd_ep_stall_clear(NRF_DRV_USBD_EPIN0);
			nrf_drv_usbd_ep_stall_clear(NRF_DRV_USBD_EPOUT0);

			// Copy the setup packet into the internal buffer
			nrf_drv_usbd_setup_get(&setup_buf);

			// Reset the remaining setup data length
			setup_remaining = setup_buf.wLength;

			// Skip data stage, go straight to status stage
//			if(setup_buf.wLength == 0) {
//				nrf_drv_usbd_setup_clear();
//			}
//			else if((setup_buf.bmRequestType & SETUP_TRANSFER_DIR_MASK) == 0) {
//				// HOST->DEVICE transfer, need to notify hardware of Data OUT stage
//				nrf_drv_usbd_setup_data_clear();
//			}

			// Notify the Mbed stack
			events->ep0_setup();
		}
			break;
		default:
			break;

		}
	}
	else if (usb_event_type == USB_HW_EVENT_POWER)
	{
		// Process USB power-related events
		switch (usb_power_event) {
		case NRF_DRV_POWER_USB_EVT_DETECTED:
			if(this->connect_enabled) {
				if(!nrf_drv_usbd_is_enabled())
					nrf_drv_usbd_enable();
				events->power(true);
			}
			break;
		case NRF_DRV_POWER_USB_EVT_REMOVED:
			events->power(false);
			break;
		case NRF_DRV_POWER_USB_EVT_READY:
			//if(this->connect_enabled) { // Not really necessary (only happens after enabled)
				if(!nrf_drv_usbd_is_started())
					nrf_drv_usbd_start(true);
			//}
			break;
		default:
			ASSERT(false);
		}
	}

	// Unflag the event type
	usb_event_type = USB_HW_EVENT_NONE;

	// Re-enable interrupt
	//NVIC_ClearPendingIRQ(USBD_IRQn);
	enable_usb_interrupts();
}

void USBPhyHw::_usb_event_handler(
		nrf_drv_usbd_evt_t const * const p_event) {
	disable_usb_interrupts();
	// Copy the event data into internal memory
	memcpy(&instance->usb_event, p_event, sizeof(instance->usb_event));
	// Tell the upper layers of the stack to process the event
	instance->usb_event_type = USB_HW_EVENT_USBD;
	instance->events->start_process();
}

void USBPhyHw::_usb_power_event_handler(nrf_drv_power_usb_evt_t event) {
	disable_usb_interrupts();
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

void USBPhyHw::_reset(void)
{
	// Disable all endpoints except for control endpoints
	nrf_drv_usbd_ep_default_config();

	nrf_drv_usbd_setup_clear();

	usb_event_type = USB_HW_EVENT_NONE;

	// TODO - Clear all endpoint interrupts?
}

void USBPhyHw::enable_usb_interrupts(void) {
	// Enable USB and USB-related power interrupts
	NVIC_EnableIRQ(USBD_IRQn);
	nrf_power_int_enable(NRF_POWER_INT_USBDETECTED_MASK |
						 NRF_POWER_INT_USBREMOVED_MASK |
						 NRF_POWER_INT_USBPWRRDY_MASK);
}

void USBPhyHw::disable_usb_interrupts(void) {
	// Disable USB and USB-related power interrupts
	NVIC_DisableIRQ(USBD_IRQn);
	nrf_power_int_disable(NRF_POWER_INT_USBDETECTED_MASK |
						 NRF_POWER_INT_USBREMOVED_MASK |
						 NRF_POWER_INT_USBPWRRDY_MASK);
}

static void power_usb_event_handler(nrf_drv_power_usb_evt_t event) {
	if(instance) {
		// Pass the event on to the USBPhyHW instance
		instance->_usb_power_event_handler(event);
	}
}

static void usbd_event_handler(nrf_drv_usbd_evt_t const * const p_event) {
	if(instance) {
		// Pass the event on to the USBPhyHW instance
		instance->_usb_event_handler(p_event);
	}
}
