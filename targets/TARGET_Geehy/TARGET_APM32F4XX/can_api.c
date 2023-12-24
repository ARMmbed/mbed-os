/* mbed Microcontroller Library
 * Copyright (c) 2006-2023 ARM Limited
 * Copyright (c) 2023 Geehy Semiconductor Inc.
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
#include "apm32f4xx_can.h"

#if DEVICE_CAN

/* BS1[3:0] + 1 + BS2[2:0] + 1 */
#define CAN_BIT_SEG_MAX          24
#define CAN_BIT_SEG_MIN          4

/* CAN related register mask */
#define CAN_TIM_SEG1_MASK        0x000F0000
#define CAN_TIM_SEG2_MASK        0x00700000
#define CAN_BAUD_PSC_MASK        0x000003FF
#define CAN_SJW_MASK             0x03000000

static uintptr_t irq_contexts[2] = {0};
static can_irq_handler irq_callback;

 /*!
 * @brief     Initialize the analogin peripheral
 *
 * @param     can_periph : CAN0 or CAN1.
 *
 * @param     id : The CANx index .
 *
 * @retval    None
 */
static void can_irq_handle(CAN_T* can, int id)
{
    uint32_t flag1 = 0, flag2 = 0, flag3 = 0;

    flag1 = CAN_ReadStatusFlag(can, CAN_FLAG_REQC0);
    flag2 = CAN_ReadStatusFlag(can, CAN_FLAG_REQC1);
    flag3 = CAN_ReadStatusFlag(can, CAN_FLAG_REQC2);

    if (flag1)
    {
        CAN_ClearStatusFlag(can, CAN_FLAG_REQC0);
    }
    if (flag2)
    {
        CAN_ClearStatusFlag(can, CAN_FLAG_REQC1);
    }
    if (flag3)
    {
        CAN_ClearStatusFlag(can, CAN_FLAG_REQC2);
    }

    /* CAN transmit complete interrupt handle */
    if (flag1 || flag2 || flag3)
    {
        irq_callback(irq_contexts[id], IRQ_TX);
    }

    /* CAN receive complete interrupt handle */
    if (CAN_ReadIntFlag(can,CAN_INT_F0MP))
    {
        irq_callback(irq_contexts[id], IRQ_RX);
    }

    /* CAN error interrupt handle */
    if (CAN_ReadIntFlag(can, CAN_INT_ERR) != RESET)
    {
        /* passive error interrupt handle */
        if (CAN_ReadIntFlag(can, CAN_INT_ERRP) != RESET)
        {
            irq_callback(irq_contexts[id], IRQ_PASSIVE);
        }

        /* bus-off interrupt handle */
        if (CAN_ReadIntFlag(can, CAN_INT_BOF) != RESET)
        {
            irq_callback(irq_contexts[id], IRQ_BUS);
        }

        irq_callback(irq_contexts[id], IRQ_ERROR);
    }
}

/*!
 * @brief     CAN1 Interrupt Request entry
 *
 * @param     None
 *
 * @retval    None
 */
static void can1_irq_entry(void)
{
    can_irq_handle(CAN1,0);
}

/*!
 * @brief     CAN1 Interrupt Request entry
 *
 * @param     None
 *
 * @retval    None
 */
static void can2_irq_entry(void)
{
    can_irq_handle(CAN2,1);
}

/*!
 * @brief     Config the CAN mode
 *
 * @param     can_periph : CAN0 or CAN1
 *
 * @param     mode : The mode to be set
 *
 * @retval    None
 */
static void can_mode_config(CAN_T* can, CAN_MODE_T mode)
{
    /* enter the initialization mode, only in initialization mode CAN register can be configured */
    CAN_OperatingMode(can,CAN_OPERATING_MODE_INIT);

    can->BITTIM &= (uint32_t)0x3fffffff;
    can->BITTIM |= (uint32_t)mode << 30;

    CAN_OperatingMode(can,CAN_OPERATING_MODE_NORMAL);
}

/*!
 * @brief     Config the CAN mode
 *
 * @param     can_periph : CAN0 or CAN1
 *
 * @param     interrupt : The interrupt type
 *
 * @param     enable : enable or disable
 *
 * @retval    None
 */
static void can_interrupt_config(CAN_T* can, CAN_INT_T interrupt, uint32_t enable)
{
    if (enable)
    {
        CAN_EnableInterrupt(can,interrupt);
    }
    else
    {
        CAN_DisableInterrupt(can,interrupt);
    }
}

/* This table can be used to calculate bit time
The first value is bit segment 1(BS1[3:0]), the second is bit segment 2(BS2[2:0]) */
static const int sampling_points[23][2] =
{
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

/*!
 * @brief     Set the baudrate
 *
 * @param     can_periph : CAN0 or CAN1
 *
 * @param     freq : The frequency value to be set
 *
 * @retval    CAN_BT register value
 */
static unsigned int can_baudrate_set(CAN_T* can,int frequency)
{
    uint32_t ret;
    uint16_t baud_psc;
    uint16_t baud_psc_max;
    uint32_t temp;
    uint32_t bt_reg_config;
    uint8_t flag;
    int bits;
    uint32_t PCLK1;

    flag = 0;

    RCM_ReadPCLKFreq(&PCLK1,0);

    /* computes the value that the CAN_BT register needs to be configured */
    /* (BAUDPSC[9:0] + 1) * ((BS1[3:0] + 1) + (BS2[2:0] + 1) + SJW(always 1)) */
    bt_reg_config = PCLK1 / frequency;
    /* BAUDPSC[9:0] minimum value */
    baud_psc = bt_reg_config / CAN_BIT_SEG_MAX;
    /* BAUDPSC[9:0] maximum value */
    baud_psc_max = bt_reg_config / CAN_BIT_SEG_MIN;

    while ((!flag) && (baud_psc < baud_psc_max))
    {
        baud_psc++;
        for (bits = 22; bits > 0; bits--)
        {
            temp = (bits + 3) * (baud_psc + 1);
            if (temp == bt_reg_config)
            {
                flag = 1;
                break;
            }
        }
    }

    if (flag)
    {
        can->BITTIM_B.RSYNJW  = 1;
        can->BITTIM_B.TIMSEG1 = sampling_points[bits][0];
        can->BITTIM_B.TIMSEG2 = sampling_points[bits][1];
        can->BITTIM_B.BRPSC   = baud_psc;
    }
    else
    {
        /* CAN_BITTIM register reset value */
        can->BITTIM = 0x01230000;
    }

    return ret;
}

/*!
 * @brief     Set the baudrate
 *
 * @param     rd : receive pin
 *
 * @param     td : transmit pin
 *
 * @param     hz : The bus frequency in hertz
 *
 * @retval    None
 */
void can_init_freq(can_t *obj, PinName rd, PinName td, int hz)
{
    CAN_Config_T canConfig;
    CAN_ConfigStructInit(&canConfig);

    CANName can_rx = (CANName)pinmap_peripheral(rd, PinMap_CAN_RD);
    CANName can_tx = (CANName)pinmap_peripheral(td, PinMap_CAN_TD);
    obj->can = (CANName)pinmap_merge(can_rx, can_tx);

    CAN_T *can = (CAN_T *)(obj->can);

    MBED_ASSERT((int)obj->can != NC);

    if (obj->can == CAN_1)
    {
        RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_CAN1);
        CAN_Reset(can);
        obj->index = 0;
    }
    else if (obj->can == CAN_2)
    {
        RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_CAN1);
        RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_CAN2);
        CAN_Reset(can);
        obj->index = 1;
    }
    else
    {
        return;
    }

    /* Configure the CAN pins */
    pinmap_pinout(rd, PinMap_CAN_RD);
    pinmap_pinout(td, PinMap_CAN_TD);

    if (rd != NC)
    {
        pin_mode(rd, PullUp);
    }
    if (td != NC)
    {
        pin_mode(td, PullUp);
    }

    CAN_Config(can,&canConfig);
    
    can_mode_config(can, CAN_MODE_NORMAL);

    can_frequency(obj, hz);

    if (obj->can == CAN_1)
    {
        can_filter(obj, 0, 0, CANStandard, 0);
    }
    else
    {
        can_filter(obj, 0, 0, CANStandard, 14);
    }
}

/*!
 * @brief     init the CAN
 *
 * @param     rd : receive pin
 *
 * @param     td : transmit pin
 *
 * @retval    None
 */
void can_init(can_t *obj, PinName rd, PinName td)
{  
    can_init_freq(obj, rd, td, 500000);
}

/*!
 * @brief     disable CAN
 *
 * @param     rd : receive pin
 *
 * @param     td : transmit pin
 *
 * @retval    None
 */
void can_free(can_t *obj)
{
    CAN_T *can = (CAN_T *)(obj->can);
    CAN_Reset(can);

    if(obj->can == CAN_1)
    {
        RCM_DisableAPB1PeriphClock(RCM_APB1_PERIPH_CAN1);
    }
    if(obj->can == CAN_2)
    {
        RCM_DisableAPB1PeriphClock(RCM_APB1_PERIPH_CAN2);
    }
}

/*!
 * @brief     Set the frequency of the CAN interface.
 *
 * @param     hz : The bus frequency in hertz
 *
 * @retval    1 if successful,0 otherwise
 */
int can_frequency(can_t *obj, int hz)
{
    int ret = 0;
    CAN_T *can = (CAN_T *)(obj->can);
    /* The maximum baud rate support to 1M */
    if (hz <= 1000000)
    {

        if (CAN_OperatingMode(can, CAN_OPERATING_MODE_INIT) == SUCCESS)
        {
            can_baudrate_set(can,hz);
        }
        else
        {
            error("the configuration of can frequency is out of range \r\n");
        }

        if (CAN_OperatingMode(can, CAN_OPERATING_MODE_NORMAL) == SUCCESS)
        {
            ret = 1;
        }
        else
        {
            ret = 0;
        }
    }

    return ret;
}

/*!
 * @brief     Set the frequency of the CAN interface.
 *
 * @param     handler : The interrupt callback.
 *
 * @param     id : The CANx index
 *
 * @retval    None
 */
void can_irq_init(can_t *obj, can_irq_handler handler, uintptr_t context)
{
    irq_callback = handler;
    irq_contexts[obj->index] = context;
}

/*!
 * @brief     disable the interrupt
 *
 * @param     None
 *
 * @retval    None
 */
void can_irq_free(can_t *obj)
{
    CAN_T *can = (CAN_T *)(obj->can);
    if(obj->can == CAN_1)
    {
        CAN_DisableInterrupt(can,CAN_INT_TXME | CAN_INT_F0MP | CAN_INT_F1MP | \
                             CAN_INT_ERRP | CAN_INT_BOF | CAN_INT_ERR);
    }

    if(obj->can == CAN_2)
    {
        CAN_DisableInterrupt(can,CAN_INT_TXME | CAN_INT_F0MP | CAN_INT_F1MP | \
                             CAN_INT_ERRP | CAN_INT_BOF | CAN_INT_ERR);
    }

    irq_contexts[obj->index] = 0;
}

/*!
 * @brief     Set the interrupt handle.
 *
 * @param     irq : The interrupt irq.
 *
 * @param     enable : enable or disable.
 *
 * @retval    None
 */
void can_irq_set(can_t *obj, CanIrqType irq, uint32_t enable)
{
    uint32_t vector = 0;
    IRQn_Type irq_num = 0;
    CAN_T *can = (CAN_T *)(obj->can);

    if (obj->can == CAN_1) 
    {
        vector = (uint32_t)can1_irq_entry;
    }
    else if (obj->can == CAN_2)
    {
        vector = (uint32_t)can2_irq_entry;
    }

    switch (irq)
    {
        case IRQ_RX:
            can_interrupt_config(can, CAN_INT_F0MP, enable);
            if(obj->can == CAN_1)
            {
                irq_num = CAN1_RX0_IRQn;
            }
            else
            {
                irq_num = CAN2_RX0_IRQn;
            }
            break;
        case IRQ_TX:
            can_interrupt_config(can, CAN_INT_TXME, enable);
            if(obj->can == CAN_1)
            {
                irq_num = CAN1_TX_IRQn;
            }
            else
            {
                irq_num = CAN2_TX_IRQn;
            }
            break;
        case IRQ_ERROR:
            can_interrupt_config(can, CAN_INT_ERR, enable);
            if(obj->can == CAN_1)
            {
                irq_num = CAN1_SCE_IRQn;
            }
            else
            {
                irq_num = CAN2_SCE_IRQn;
            }
            break;
        case IRQ_PASSIVE:
            can_interrupt_config(can, CAN_INT_ERRP, enable);
            if(obj->can == CAN_1)
            {
                irq_num = CAN1_SCE_IRQn;
            }
            else
            {
                irq_num = CAN2_SCE_IRQn;
            }
            break;
        case IRQ_BUS:
            can_interrupt_config(can, CAN_INT_BOF, enable);
            if(obj->can == CAN_1)
            {
                irq_num = CAN1_SCE_IRQn;
            }
            else
            {
                irq_num = CAN2_SCE_IRQn;
            }
            break;
        default:
            return;
    }

    NVIC_SetVector(irq_num, vector);
    NVIC_EnableIRQ(irq_num);
}

/*!
 * @brief     Write a CANMessage to the bus.
 *
 * @param     message : The CANMessage to write.
 *
 * @retval    0 if write failed,1 if write was successful
 */
int can_write(can_t *obj, CAN_Message message, int cc)
{
    CAN_TxMessage_T TxMessage;
    uint32_t i;
    CAN_T *can = (CAN_T *)(obj->can);

    /* configure frame type: data or remote */
    if (message.type == CANData) 
    {
        TxMessage.remoteTxReq = CAN_RTXR_DATA;
    } 
    else if (message.type == CANRemote)
    {
        TxMessage.remoteTxReq = CAN_RTXR_REMOTE;
    } 
    else 
    {
        error("frame type of transmit message is invalid \r\n");
    }

    /* configure frame format: standard or extended */
    if (message.format == CANStandard)
    {
        TxMessage.typeID = CAN_TYPEID_STD;
        TxMessage.stdID = message.id;
    }
    else if (message.format == CANExtended)
    {
        TxMessage.typeID = CAN_TYPEID_EXT;
        TxMessage.extID = message.id;
    }
    else
    {
        error("frame format of transmit message is invalid \r\n");
    }

    TxMessage.dataLengthCode = message.len;

    for (i = 0; i < message.len; i++)
    {
        TxMessage.data[i] = message.data[i];
    }

    CAN_TxMessage(can,&TxMessage);

    return 1;
}

/*!
 * @brief     Read a CANMessage from the bus.
 *
 * @param     msg : A CANMessage to read to
 *
 * @param     handle : message filter handle (0 for any message)
 *
 * @retval    0 if no message arrived,1 if message arrived
 */
int can_read(can_t *obj, CAN_Message *msg, int handle)
{
    uint8_t i;
    uint8_t fifo_number;

    fifo_number = (uint8_t)handle;
    CAN_RxMessage_T RxMessage;
    CAN_T *can = (CAN_T *)(obj->can);

    /* if the frame is not received, retrun 0 */
    if (can->RXF0_B.FMNUM0 == 0)
    {
        return 0;
    }

    CAN_RxMessage(can, fifo_number, &RxMessage);

    if (RxMessage.typeID == CAN_TYPEID_STD)
    {
        msg->format = CANStandard;
    }
    else
    {
        msg->format = CANExtended;
    }

    if (msg->format == CANStandard)
    {
        msg->id = (uint32_t)0x000007FF & (RxMessage.stdID);
    } 
    else
    {
        msg->id = (uint32_t)0x1FFFFFFF & (RxMessage.extID);
    }

    if (RxMessage.remoteTxReq == CAN_RTXR_REMOTE)
    {
        msg->type = CANRemote;
    }
    else
    {
        msg->type = CANData;
    }

    msg->len = (uint8_t)RxMessage.dataLengthCode;

    for (i = 0; i < msg->len; i++)
    {
        msg->data[i] = (uint8_t)RxMessage.data[i];
    }
    /* If the frame is received successfully, retrun 1 */
    return 1;
}

/*!
 * @brief     Reset CAN interface
 *
 * @param     obj : can_periph
 *
 * @retval    None
 */
void can_reset(can_t *obj)
{
    CAN_T *can = (CAN_T *)(obj->can);

    CAN_Reset(can);
}

/*!
 * @brief     Detects read errors - Used to detect read overflow errors
 *
 * @param     obj : can_periph
 *
 * @retval    number of read errors
 */
unsigned char can_rderror(can_t *obj)
{
    CAN_T *can = (CAN_T *)(obj->can);

    return (can->ERRSTS >> 24) & 0xFF;
}

/*!
 * @brief     Detects write errors - Used to detect write overflow errors
 *
 * @param     obj : can_periph
 *
 * @retval    number of write errors
 */
unsigned char can_tderror(can_t *obj)
{
    CAN_T *can = (CAN_T *)(obj->can);

    return (can->ERRSTS >> 16) & 0xFF;
}

/*!
 * @brief     Puts or removes the CAN interface into silent monitoring mode
 *
 * @param     obj : can_periph
 *
 * @param     silent : boolean indicating whether to go into silent mode or not
 *
 * @retval    None
 */
void can_monitor(can_t *obj, int silent)
{
    CAN_T *can = (CAN_T *)(obj->can);
    if (silent)
    {
        can_mode_config(can, CAN_MODE_SILENT);
    }
    else
    {
        can_mode_config(can, CAN_MODE_NORMAL);
    }
}

/*!
 * @brief     Change CAN operation to the specified mode
 *
 * @param     obj : can_periph
 *
 * @param     mode : The new operation mode (CAN::Normal, CAN::Silent, CAN::LocalTest, CAN::GlobalTest, CAN::SilentTest)
 *
 * @retval    0 if mode change failed or unsupported,1 if mode change was successful
 */
int can_mode(can_t *obj, CanMode mode)
{
    CAN_T *can = (CAN_T *)(obj->can);
    switch (mode)
    {
        case MODE_NORMAL:
            can_mode_config(can, CAN_MODE_NORMAL);
            break;
        case MODE_SILENT:
            can_mode_config(can, CAN_MODE_SILENT);
            break;
        case MODE_TEST_GLOBAL:
        case MODE_TEST_LOCAL:
            can_mode_config(can, CAN_MODE_LOOPBACK);
            break;
        case MODE_TEST_SILENT:
            can_mode_config(can, CAN_MODE_SILENT_LOOPBACK);
            break;
        default:
            return 0; 
    }
    return 0;
}

/*!
 * @brief     Filter out incomming messages
 *
 * @param     obj : can_periph
 *
 * @param     id : The id to filter on
 *
 * @param     mask : The mask applied to the id
 *
 * @param     format : format to filter on (Default CANAny)
  *
 * @param     handle : message filter handle (Optional)
 *
 * @retval    0 if filter change failed or unsupported,new filter handle if successful
 */
int can_filter(can_t *obj, uint32_t id, uint32_t mask, CANFormat format, int32_t handle)
{
    CAN_FilterConfig_T filterConfig;
    CAN_T *can = (CAN_T *)(obj->can);

    filterConfig.filterNumber = handle;
    filterConfig.filterMode = CAN_FILTER_MODE_IDMASK;
    filterConfig.filterScale = CAN_FILTER_SCALE_32BIT;
    filterConfig.filterFIFO = CAN_FILTER_FIFO_0;
    filterConfig.filterActivation = ENABLE;

    switch (format)
    {
        case CANStandard:
            /* configure SFID[10:0] */
            filterConfig.filterIdHigh = id << 5;
            filterConfig.filterIdLow =  0x0;
            /* configure SFID[10:0] mask */
            filterConfig.filterMaskIdHigh = mask << 5;
            /* both data and remote frames can be received */
            filterConfig.filterMaskIdLow = 0x0;
            break;

        case CANExtended:
            /* configure EFID[28:13] */
            filterConfig.filterIdHigh = id >> 13;
            /* configure EFID[12:0] and frame format bit set */
            filterConfig.filterIdLow = (id << 3) | (1 << 2);
            /* configure EFID[28:13] mask */
            filterConfig.filterMaskIdHigh = mask >> 13;
            /* configure EFID[12:0] and frame format bit mask */
            /* both data and remote frames can be received */
            filterConfig.filterMaskIdLow = (mask << 3) | (1 << 2);
            break;

        case CANAny:
            error("CANAny mode is not supported \r\n");

        default:
            error("parameter is not supported \r\n");
    }

    CAN_ConfigFilter(&filterConfig);
    CAN_SlaveStartBank(can,handle);

    return handle;
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
