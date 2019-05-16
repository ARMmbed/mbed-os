/* mbed Microcontroller Library
 * Copyright (c) 2018 GigaDevice Semiconductor Inc.
 *
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

#include "can_api.h"
#include "pinmap.h"
#include "PeripheralPins.h"
#include "mbed_error.h"

#if DEVICE_CAN

/* BS1[3:0] + 1 + BS2[2:0] + 1 */
#define DEV_CAN_BT_SEG_MAX          24
#define DEV_CAN_BT_SEG_MIN          4

/* CAN related register mask */
#define DEV_CAN_BS1_MASK            0x000F0000
#define DEV_CAN_BS2_MASK            0x00700000
#define DEV_CAN_BAUDPSC_MASK        0x000003FF
#define DEV_CAN_SJW_MASK            0x03000000
/* CAN0 interrupt vector number */
#define CAN0_IRQ_BASE_NUM             19

/* CAN1 interrupt vector number */
#define CAN1_IRQ_BASE_NUM             63

static uint32_t can_irq_ids[2] = {0};
static can_irq_handler irq_callback;

/** CAN interrupt handle .
 *
 *  @param can_periph CAN0 or CAN1.
 *  @param id the CANx index .
 */
static void dev_can_irq_handle(uint32_t periph, int id)
{
    uint32_t flag0 = 0, flag1 = 0, flag2 = 0;

    flag0 = can_interrupt_flag_get(periph, CAN_INT_FLAG_MTF0);
    flag1 = can_interrupt_flag_get(periph, CAN_INT_FLAG_MTF1);
    flag2 = can_interrupt_flag_get(periph, CAN_INT_FLAG_MTF2);

    if (flag0) {
        can_flag_clear(periph, CAN_FLAG_MTF0);
    }
    if (flag1) {
        can_flag_clear(periph, CAN_FLAG_MTF1);
    }
    if (flag2) {
        can_flag_clear(periph, CAN_FLAG_MTF2);
    }

    /* CAN transmit complete interrupt handle */
    if (flag0 || flag1 || flag2) {
        irq_callback(can_irq_ids[id], IRQ_TX);
    }

    /* CAN receive complete interrupt handle */
    if (CAN_INTEN_RFNEIE0 == (CAN_INTEN(periph) & CAN_INTEN_RFNEIE0)) {
        if (0 != can_receive_message_length_get(periph, CAN_FIFO0)) {
            irq_callback(can_irq_ids[id], IRQ_RX);
        }
    }

    /* CAN error interrupt handle */
    if (SET == can_interrupt_flag_get(periph, CAN_INT_FLAG_ERRIF)) {
        /* passive error interrupt handle */
        if (CAN_INTEN_PERRIE == (CAN_INTEN(periph) & CAN_INTEN_PERRIE)) {
            if (SET == can_flag_get(periph, CAN_FLAG_PERR)) {
                irq_callback(can_irq_ids[id], IRQ_PASSIVE);
            }
        }

        /* bus-off interrupt handle */
        if (CAN_INTEN_BOIE == (CAN_INTEN(periph) & CAN_INTEN_BOIE)) {
            if (SET == can_flag_get(periph, CAN_FLAG_BOERR)) {
                irq_callback(can_irq_ids[id], IRQ_BUS);
            }
        }

        irq_callback(can_irq_ids[id], IRQ_ERROR);
    }
}

/** CAN1 Interrupt Request entry .
 *
 */
static void dev_can0_irq_entry(void)
{
    dev_can_irq_handle(CAN0, 0);
}

/** CAN1 Interrupt Request entry .
 *
 */
static void dev_can1_irq_entry(void)
{
    dev_can_irq_handle(CAN1, 1);
}

/** Config the CAN mode .
 *
 *  @param can_periph CAN0 or CAN1.
 *  @param mode the mode to be set.
 */
static void dev_can_mode_config(uint32_t can_periph, uint32_t mode)
{
    /* enter the initialization mode, only in initialization mode CAN register can be configured */
    can_working_mode_set(can_periph, CAN_MODE_INITIALIZE);

    CAN_BT(can_periph) &= ~BT_MODE(3);
    CAN_BT(can_periph) |= BT_MODE(mode);

    /* enter the normal mode */
    can_working_mode_set(can_periph, CAN_MODE_NORMAL);
}

/** Config the interrupt .
 *
 *  @param can_periph CAN0 or CAN1.
 *  @param interrupt The interrupt type.
 *  @param enable enable or disable.
 */
static void dev_can_interrupt_config(uint32_t can_periph, uint32_t interrupt, uint32_t enable)
{
    if (enable) {
        can_interrupt_enable(can_periph, interrupt);
    } else {
        can_interrupt_disable(can_periph, interrupt);
    }
}

/* This table can be used to calculate bit time
The first value is bit segment 1(BS1[3:0]), the second is bit segment 2(BS2[2:0]) */
static const int sampling_points[23][2] = {
    {0x0, 0x0},      /* 2, 50% */
    {0x1, 0x0},      /* 3, 67% */
    {0x2, 0x0},      /* 4, 75% */
    {0x3, 0x0},      /* 5, 80% */
    {0x3, 0x1},      /* 6, 67% */
    {0x4, 0x1},      /* 7, 71% */
    {0x5, 0x1},      /* 8, 75% */
    {0x6, 0x1},      /* 9, 78% */
    {0x6, 0x2},      /* 10, 70% */
    {0x7, 0x2},      /* 11, 73% */
    {0x8, 0x2},      /* 12, 75% */
    {0x9, 0x2},      /* 13, 77% */
    {0x9, 0x3},      /* 14, 71% */
    {0xA, 0x3},      /* 15, 73% */
    {0xB, 0x3},      /* 16, 75% */
    {0xC, 0x3},      /* 17, 76% */
    {0xD, 0x3},      /* 18, 78% */
    {0xD, 0x4},      /* 19, 74% */
    {0xE, 0x4},      /* 20, 75% */
    {0xF, 0x4},      /* 21, 76% */
    {0xF, 0x5},      /* 22, 73% */
    {0xF, 0x6},      /* 23, 70% */
    {0xF, 0x7},      /* 24, 67% */
};

/** Set the baudrate.
 *
 *  @param freq The frequency value to be set.
 *
 *  @returns
 *     CAN_BT register value
 */
static unsigned int dev_can_baudrate_set(int freq)
{
    uint32_t reval;
    uint16_t baud_psc;
    uint16_t baud_psc_max;
    uint32_t temp;
    uint32_t bt_reg_config;
    uint8_t flag;
    int bits;

    flag = 0;

    /* computes the value that the CAN_BT register needs to be configured */
    /* (BAUDPSC[9:0] + 1) * ((BS1[3:0] + 1) + (BS2[2:0] + 1) + SJW(always 1)) */
    bt_reg_config = (rcu_clock_freq_get(CK_APB1) / freq);
    /* BAUDPSC[9:0] minimum value */
    baud_psc = bt_reg_config / DEV_CAN_BT_SEG_MAX;
    /* BAUDPSC[9:0] maximum value */
    baud_psc_max = bt_reg_config / DEV_CAN_BT_SEG_MIN;

    while ((!flag) && (baud_psc < baud_psc_max)) {
        baud_psc++;
        for (bits = 22; bits > 0; bits--) {
            temp = (bits + 3) * (baud_psc + 1);
            if (temp == bt_reg_config) {
                flag = 1;
                break;
            }
        }
    }

    if (flag) {
        reval = ((sampling_points[bits][1] << 20) & DEV_CAN_BS2_MASK)
                | ((sampling_points[bits][0] << 16) & DEV_CAN_BS1_MASK)
                | ((1                   << 24) & DEV_CAN_SJW_MASK)
                | ((baud_psc            <<  0) & DEV_CAN_BAUDPSC_MASK);
    } else {
        /* CAN_BT register reset value */
        reval = 0x01230000;
    }

    return reval;
}

/** init the CAN.
 *
 */
void can_init(can_t *obj, PinName rd, PinName td)
{
    can_init_freq(obj, rd, td, 500000);
}

/** init the CAN frequency.
 *
 *  @param rd receive pin.
 *  @param td transmit pin.
 *  @param hz The bus frequency in hertz.
 */
void can_init_freq(can_t *obj, PinName rd, PinName td, int hz)
{
    CANName can_rd = (CANName)pinmap_peripheral(rd, PinMap_CAN_RD);
    CANName can_td = (CANName)pinmap_peripheral(td, PinMap_CAN_TD);
    obj->can = (CANName)pinmap_merge(can_rd, can_td);

    MBED_ASSERT((int)obj->can != NC);

    if (obj->can == CAN_0) {
        rcu_periph_clock_enable(RCU_CAN0);
        can_deinit(obj->can);
        obj->index = 0;
    } else if (obj->can == CAN_1) {
        rcu_periph_clock_enable(RCU_CAN0);
        rcu_periph_clock_enable(RCU_CAN1);
        can_deinit(obj->can);
        obj->index = 1;
    } else {
        return;
    }

    /* Configure the CAN pins */
    pinmap_pinout(rd, PinMap_CAN_RD);
    pinmap_pinout(td, PinMap_CAN_TD);
    if (rd != NC) {
        pin_mode(rd, PullUp);
    }
    if (td != NC) {
        pin_mode(td, PullUp);
    }

    dev_can_mode_config(obj->can, CAN_NORMAL_MODE);

    can_frequency(obj, hz);

    if (obj->can == CAN_0) {
        can_filter(obj, 0, 0, CANStandard, 0);
    } else {
        can_filter(obj, 0, 0, CANStandard, 14);
    }
}

/** disable CAN.
 *
 */
void can_free(can_t *obj)
{
    can_deinit(obj->can);

    if (obj->can == CAN0) {
        rcu_periph_clock_disable(RCU_CAN0);
    }

    if (obj->can == CAN1) {
        rcu_periph_clock_disable(RCU_CAN1);
    }
}

/** Set the frequency of the CAN interface.
 *
 *  @param hz The bus frequency in hertz.
 *
 *  @returns
 *    1 if successful,
 *    0 otherwise
 */
int can_frequency(can_t *obj, int hz)
{
    int reval = 0;

    /* The maximum baud rate support to 1M */
    if (hz <= 1000000) {
        if (SUCCESS == can_working_mode_set(obj->can, CAN_MODE_INITIALIZE)) {
            CAN_BT(obj->can) = dev_can_baudrate_set(hz);
        } else {
            error("the configuration of can frequency is out of range \r\n");
        }

        if (SUCCESS == can_working_mode_set(obj->can, CAN_MODE_NORMAL)) {
            reval = 1;
        } else {
            reval = 0;
        }
    }

    return reval;
}

/** init the interrupt.
 *
 *  @param handler the interrupt callback.
 *  @param id the CANx index.
 */
void can_irq_init(can_t *obj, can_irq_handler handler, uint32_t id)
{
    irq_callback = handler;
    can_irq_ids[obj->index] = id;
}

/** disable the interrupt.
 *
 */
void can_irq_free(can_t *obj)
{
    if (CAN0 == obj->can) {
        can_interrupt_disable(obj->can, CAN_INTEN_TMEIE | CAN_INTEN_RFNEIE0 | CAN_INTEN_RFNEIE1 | \
                              CAN_INTEN_PERRIE | CAN_INTEN_BOIE | CAN_INTEN_ERRIE);
    }

    if (CAN1 == obj->can) {
        can_interrupt_disable(obj->can, CAN_INTEN_TMEIE | CAN_INTEN_RFNEIE0 | CAN_INTEN_RFNEIE1 | \
                              CAN_INTEN_PERRIE | CAN_INTEN_BOIE | CAN_INTEN_ERRIE);
    }

    can_irq_ids[obj->index] = 0;
}

/** Set the interrupt handle.
 *
 *  @param type The interrupt type.
 *  @param enable enable or disable.
 */
void can_irq_set(can_t *obj, CanIrqType type, uint32_t enable)
{
    uint32_t irq_num;
    uint32_t vector = 0;

    if (obj->can == CAN_0) {
        vector = (uint32_t)dev_can0_irq_entry;
        irq_num = CAN0_IRQ_BASE_NUM;
    }

    else if (obj->can == CAN_1) {
        vector = (uint32_t)dev_can1_irq_entry;
        irq_num = CAN1_IRQ_BASE_NUM;
    }

    switch (type) {
        case IRQ_RX:
            dev_can_interrupt_config(obj->can, CAN_INT_RFNE0, enable);
            break;
        case IRQ_TX:
            dev_can_interrupt_config(obj->can, CAN_INT_TME, enable);
            irq_num += 1;
            break;
        case IRQ_ERROR:
            dev_can_interrupt_config(obj->can, CAN_INT_ERR | CAN_INT_ERRN, enable);
            irq_num += 3;
            break;
        case IRQ_PASSIVE:
            dev_can_interrupt_config(obj->can, CAN_INT_ERR | CAN_INT_PERR, enable);
            irq_num += 3;
            break;
        case IRQ_BUS:
            dev_can_interrupt_config(obj->can, CAN_INT_ERR | CAN_INT_BO, enable);
            irq_num += 3;
            break;
        default:
            return;
    }

    NVIC_SetVector((IRQn_Type)irq_num, vector);
    NVIC_EnableIRQ((IRQn_Type)irq_num);
}

/** Write a CANMessage to the bus.
 *
 *  @param msg The CANMessage to write.
 *
 *  @returns
 *    0 if write failed,
 *    1 if write was successful
 */
int can_write(can_t *obj, CAN_Message msg, int cc)
{
    can_trasnmit_message_struct transmit_message;
    uint32_t i;

    can_struct_para_init(CAN_TX_MESSAGE_STRUCT, &transmit_message);
    /* configure frame type: data or remote */
    if (CANData == msg.type) {
        transmit_message.tx_ft = CAN_FT_DATA;
    } else if (CANRemote == msg.type) {
        transmit_message.tx_ft = CAN_FT_REMOTE;
    } else {
        error("frame type of transmit message is invalid \r\n");
    }

    /* configure frame format: standard or extended */
    if (CANStandard == msg.format) {
        transmit_message.tx_ff = CAN_FF_STANDARD;
        transmit_message.tx_sfid = msg.id;
    } else if (CANExtended == msg.format) {
        transmit_message.tx_ff = CAN_FF_EXTENDED;
        transmit_message.tx_efid = msg.id;
    } else {
        error("frame format of transmit message is invalid \r\n");
    }

    transmit_message.tx_dlen = msg.len;

    for (i = 0; i < msg.len; i++) {
        transmit_message.tx_data[i] = msg.data[i];
    }

    can_message_transmit(obj->can, &transmit_message);

    return 1;
}

/** Read a CANMessage from the bus.
 *
 *  @param msg A CANMessage to read to.
 *  @param handle message filter handle (0 for any message).
 *
 *  @returns
 *    0 if no message arrived,
 *    1 if message arrived
 */
int can_read(can_t *obj, CAN_Message *msg, int handle)
{
    uint8_t i;
    uint8_t fifo_number;

    fifo_number = (uint8_t)handle;
    can_receive_message_struct receive_message;

    /* if the frame is not received, retrun 0 */
    if (0 == can_receive_message_length_get(obj->can, CAN_FIFO0)) {
        return 0;
    }

    can_message_receive(obj->can, fifo_number, &receive_message);

    if (receive_message.rx_ff == CAN_RFIFOMI_FF) {
        msg->format = CANExtended;
    } else {
        msg->format = CANStandard;
    }

    if (0 == msg->format) {
        msg->id = (uint32_t)0x000007FF & (receive_message.rx_sfid);
    } else {
        msg->id = (uint32_t)0x1FFFFFFF & (receive_message.rx_efid);
    }

    if (receive_message.rx_ft == CAN_RFIFOMI_FT) {
        msg->type = CANRemote;
    } else {
        msg->type = CANData;
    }

    msg->len = (uint8_t)receive_message.rx_dlen;

    for (i = 0; i < msg->len; i++) {
        msg->data[i] = (uint8_t)receive_message.rx_data[i];
    }
    /* If the frame is received successfully, retrun 1 */
    return 1;
}

/** Change CAN operation to the specified mode.
 *
 *  @param mode The new operation mode (CAN::Normal, CAN::Silent, CAN::LocalTest, CAN::GlobalTest, CAN::SilentTest).
 *
 *  @returns
 *    0 if mode change failed or unsupported,
 *    1 if mode change was successful
 */
int can_mode(can_t *obj, CanMode mode)
{
    switch (mode) {
        case MODE_NORMAL:
            dev_can_mode_config(obj->can, CAN_NORMAL_MODE);
            break;
        case MODE_SILENT:
            dev_can_mode_config(obj->can, CAN_SILENT_MODE);
            break;
        case MODE_TEST_GLOBAL:
        case MODE_TEST_LOCAL:
            dev_can_mode_config(obj->can, CAN_LOOPBACK_MODE);
            break;
        case MODE_TEST_SILENT:
            dev_can_mode_config(obj->can, CAN_SILENT_LOOPBACK_MODE);
            break;
        default:
            return 0;
    }

    return 1;
}

/** Filter out incomming messages.
 *
 *  @param id the id to filter on.
 *  @param mask the mask applied to the id.
 *  @param format format to filter on (Default CANAny).
 *  @param handle message filter handle (Optional).
 *
 *  @returns
 *    0 if filter change failed or unsupported,
 *    new filter handle if successful
 */
int can_filter(can_t *obj, uint32_t id, uint32_t mask, CANFormat format, int32_t handle)
{
    can_filter_parameter_struct can_filter;

    can_filter.filter_number = handle;
    can_filter.filter_mode = CAN_FILTERMODE_MASK;
    can_filter.filter_bits = CAN_FILTERBITS_32BIT;
    can_filter.filter_fifo_number = CAN_FIFO0;
    can_filter.filter_enable = ENABLE;

    switch (format) {
        case CANStandard:
            /* configure SFID[10:0] */
            can_filter.filter_list_high = id << 5;
            can_filter.filter_list_low =  0x0;
            /* configure SFID[10:0] mask */
            can_filter.filter_mask_high = mask << 5;
            /* both data and remote frames can be received */
            can_filter.filter_mask_low = 0x0;

            break;

        case CANExtended:
            /* configure EFID[28:13] */
            can_filter.filter_list_high = id >> 13;
            /* configure EFID[12:0] and frame format bit set */
            can_filter.filter_list_low = (id << 3) | (1 << 2);
            /* configure EFID[28:13] mask */
            can_filter.filter_mask_high = mask >> 13;
            /* configure EFID[12:0] and frame format bit mask */
            /* both data and remote frames can be received */
            can_filter.filter_mask_low = (mask << 3) | (1 << 2);

            break;

        case CANAny:
            error("CANAny mode is not supported \r\n");
            return 0;

        default:
            error("parameter is not supported \r\n");
            return 0;
    }

    can_filter_init(&can_filter);
    can1_filter_start_bank(handle);

    return handle;
}

/** Reset CAN interface.
 *
 * To use after error overflow.
 */
void can_reset(can_t *obj)
{
    can_deinit(obj->can);
}

/**  Detects read errors - Used to detect read overflow errors.
 *
 *  @returns number of read errors
 */
unsigned char can_rderror(can_t *obj)
{
    return can_receive_error_number_get(obj->can);
}

/** Detects write errors - Used to detect write overflow errors.
 *
 *  @returns number of write errors
 */
unsigned char can_tderror(can_t *obj)
{
    return can_transmit_error_number_get(obj->can);
}

/** Puts or removes the CAN interface into silent monitoring mode.
 *
 *  @param silent boolean indicating whether to go into silent mode or not.
 */
void can_monitor(can_t *obj, int silent)
{
    if (silent) {
        dev_can_mode_config(obj->can, CAN_SILENT_MODE);
    } else {
        dev_can_mode_config(obj->can, CAN_NORMAL_MODE);
    }
}

const PinMap *can_rd_pinmap()
{
    return PinMap_CAN_TD;
}

const PinMap *can_td_pinmap()
{
    return PinMap_CAN_RD;
}

#endif
