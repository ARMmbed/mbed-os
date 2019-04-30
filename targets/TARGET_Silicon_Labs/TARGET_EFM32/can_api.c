/* mbed Microcontroller Library
 * Copyright (c) 2019 ToolSense
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
#include "can_api.h"
#include "clocking.h"


#if DEVICE_CAN

#include "cmsis.h"
#include "pinmap.h"
#include "pinmap_function.h"
#include "PeripheralPins.h"
#include "mbed_assert.h"
#include "em_cmu.h"
#include "em_can.h"

static uint32_t can_irq_ids[CAN_COUNT] = {0};
static can_irq_handler irq_handler;

// CAN bus interfaces
#define CAN_TX_IF                 0
#define CAN_RX_IF                 1

void can_init(can_t *obj, PinName rd, PinName td)
{
    can_init_freq(obj, rd, td, 100000);
}

void can_init_freq(can_t *obj, PinName rd, PinName td, int hz)
{
    CANName can_rd = (CANName)pinmap_peripheral(rd, PinMap_CAN_RX);
    CANName can_td = (CANName)pinmap_peripheral(td, PinMap_CAN_TX);
    obj->instance = (CAN_TypeDef *)pinmap_merge(can_rd, can_td);

    CMU_Clock_TypeDef cmuClock_number;
    switch ((CANName)obj->instance) {
#ifdef CAN0
        case CAN_0:
        	cmuClock_number = cmuClock_CAN0;
            break;
#endif
#ifdef CAN1
        case CAN_1:
        	cmuClock_number = cmuClock_CAN1;
            break;
#endif
    }

    MBED_ASSERT((unsigned int)rd != NC);
    MBED_ASSERT((unsigned int)td != NC);


    // Configure CAN pins
    pinmap_pinout(rd, PinMap_CAN_RX);
    pinmap_pinout(td, PinMap_CAN_TX);


    CMU_ClockEnable(cmuClock_number, true);

	CAN_Init_TypeDef CanInit = CAN_INIT_DEFAULT;
	CanInit.bitrate=hz;
	CAN_Init(obj->instance, &CanInit);
	CAN_SetMode(obj->instance, canModeNormal);

    uint32_t loc_rd = pin_location(rd, PinMap_CAN_RX);
    uint32_t loc_td = pin_location(td, PinMap_CAN_TX);

    CAN_SetRoute(obj->instance, true, loc_rd, loc_td);


    // Add pull-ups
    if (rd != NC) {
        pin_mode(rd, gpioModeInput);
    }
    if (td != NC) {
        pin_mode(td, gpioModePushPull);
    }

    CAN_ConfigureMessageObject(obj->instance, CAN_TX_IF, 1, true, true, false, true, true);

    CAN_ConfigureMessageObject(obj->instance, CAN_RX_IF, 2, true, false, false, true, true);

	CAN_MessageObject_TypeDef receiver;

	receiver.msgNum = 2;
	receiver.id = 0;
	receiver.mask = 0;
	receiver.extended = false;
	receiver.extendedMask = false;

	CAN_SetIdAndFilter(obj->instance, CAN_RX_IF, true, &receiver, true);
}

void can_irq_init(can_t *obj, can_irq_handler handler, uint32_t id)
{
	int index = 0;

    switch ((CANName)obj->instance) {
#ifdef CAN0
        case CAN_0:
        	index = 0;
            break;
#endif
#ifdef CAN1
        case CAN_1:
        	index = 1;
            break;
#endif
    }

	irq_handler = handler;
    can_irq_ids[index] = id;
}

void can_irq_free(can_t *obj)
{
   	CAN_MessageIntDisable(obj->instance, _CAN_IF0IEN_MESSAGE_MASK);
	CAN_MessageIntClear(obj->instance, 0xFFFFFFFF);

    switch ((CANName)obj->instance) {
#ifdef CAN0
        case CAN_0:
        	NVIC_DisableIRQ(CAN0_IRQn);
            break;
#endif
#ifdef CAN1
        case CAN_1:
        	NVIC_DisableIRQ(CAN1_IRQn);
            break;
#endif
    }
}

void can_free(can_t *obj)
{
	CAN_Enable(obj->instance, false);
}

int can_frequency(can_t *obj, int f)
{
    CAN_Init_TypeDef CanInit = CAN_INIT_DEFAULT;
    CanInit.bitrate=f;

    CAN_SetBitTiming(obj->instance,
                   CanInit.bitrate,
                   CanInit.propagationTimeSegment,
                   CanInit.phaseBufferSegment1,
                   CanInit.phaseBufferSegment2,
                   CanInit.synchronisationJumpWidth);
    return 0;
}

int can_write(can_t *obj, CAN_Message msg, int cc)
{
    CAN_MessageObject_TypeDef message;

    message.id = msg.id;
    message.msgNum = 1;

    if (msg.format == 0) message.extended = false;
    else message.extended = true;

    message.dlc = msg.len;

    for (int i = 0; i < message.dlc; ++i) {
        message.data[i] = (uint32_t)msg.data[i];
    }

    CAN_SendMessage(obj->instance, CAN_TX_IF, &message, true);

    return 1;
}

int can_read(can_t *obj, CAN_Message *msg, int handle)
{
	CAN_MessageObject_TypeDef receiver;

	if (CAN_HasNewdata(obj->instance)) {

		receiver.msgNum = 2;
		receiver.extended = false;
		receiver.extendedMask = false;

		CAN_ReadMessage(obj->instance, CAN_RX_IF, &receiver);

		msg->id = receiver.id;
		msg->len = receiver.dlc;

		for (int i = 0; i < receiver.dlc; ++i) {
			msg->data[i] = (unsigned char)receiver.data[i];
		}

	    return 1;
	}

	return 0;
}

void can_reset(can_t *obj)
{
    CAN_Reset(obj->instance);
}

unsigned char can_rderror(can_t *obj)
{
	return ((obj->instance->ERRCNT>>_CAN_ERRCNT_REC_SHIFT)&0xFF);
}

unsigned char can_tderror(can_t *obj)
{
	return ((obj->instance->ERRCNT)&0xFF);
}

void can_monitor(can_t *obj, int silent)
{
	CanMode mode = MODE_NORMAL;

    if (silent) {
    	mode = MODE_SILENT;
    }

    CAN_SetMode(obj->instance, mode);
}

int can_mode(can_t *obj, CanMode mode)
{
	CAN_SetMode(obj->instance, mode);
	return 1;
}

int can_filter(can_t *obj, uint32_t id, uint32_t mask, CANFormat format, int32_t handle)
{
	CAN_MessageObject_TypeDef message;

	message.msgNum = 2;
	message.id = id;
	message.mask = mask;

	if (format == CANStandard) {
	  message.extended = false;
	  message.extendedMask = false;
	} else {
	  message.extended = true;
	  message.extendedMask = true;
	}

	CAN_SetIdAndFilter(obj->instance, CAN_RX_IF, true, &message, true);

	return handle;
}

void can_irq_set(can_t *obj, CanIrqType type, uint32_t enable)
{
	uint32_t ier = _CAN_IF0IEN_MESSAGE_MASK;

    if (enable) {
    	CAN_MessageIntEnable(obj->instance, ier);
    } else {
    	CAN_MessageIntDisable(obj->instance, ier);
    }

	switch ((CANName)obj->instance) {
#ifdef CAN0
        case CAN_0:
        	NVIC_SetVector(CAN0_IRQn, (uint32_t)CAN0_IRQHandler);
        	NVIC_EnableIRQ(CAN0_IRQn);
            break;
#endif
#ifdef CAN1
        case CAN_1:
        	NVIC_SetVector(CAN1_IRQn, (uint32_t)CAN1_IRQHandler);
        	NVIC_EnableIRQ(CAN1_IRQn);
            break;
#endif
    }
}

static void can_irq(CANName name, int id)
{
	CAN_TypeDef *can;
	can = (CAN_TypeDef *)name;

	if (can->STATUS & CAN_STATUS_RXOK) {
		irq_handler(can_irq_ids[id], IRQ_RX);
	    CAN_MessageIntClear(can, 0xFFFFFFFF);
	}

	if (can->STATUS & CAN_STATUS_TXOK) {
		irq_handler(can_irq_ids[id], IRQ_TX);
	    CAN_MessageIntClear(can, 0xFFFFFFFF);
	}
}

#ifdef CAN0
void CAN0_IRQHandler(void)
{
	can_irq(CAN_0, 0);
}
#endif

#ifdef CAN1
void CAN1_IRQHandler(void)
{
	can_irq(CAN_1, 1);
}
#endif

const PinMap *can_rd_pinmap()
{
    return PinMap_CAN_TX;
}

const PinMap *can_td_pinmap()
{
    return PinMap_CAN_RX;
}

#endif //DEVICE_CAN
