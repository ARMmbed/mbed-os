/* mbed Microcontroller Library
 * Copyright (c) 2006-2017 ARM Limited
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

#if DEVICE_CAN

#include "cmsis.h"
#include "pinmap.h"
#include "PeripheralPins.h"
#include "mbed_error.h"
#include "can_device.h" // Specific to STM32 serie
#include <math.h>
#include <string.h>

static uint32_t can_irq_ids[CAN_NUM] = {0};
static can_irq_handler irq_handler;

static void can_registers_init(can_t *obj)
{
    if (HAL_CAN_Init(&obj->CanHandle) != HAL_OK) {
        error("Cannot initialize CAN");
    }

    // Set initial CAN frequency to specified frequency
    if (can_frequency(obj, obj->hz) != 1) {
        error("Can frequency could not be set\n");
    }
}

void can_init(can_t *obj, PinName rd, PinName td)
{
    can_init_freq(obj, rd, td, 100000);
}

void can_init_freq (can_t *obj, PinName rd, PinName td, int hz)
{
    CANName can_rd = (CANName)pinmap_peripheral(rd, PinMap_CAN_RD);
    CANName can_td = (CANName)pinmap_peripheral(td, PinMap_CAN_TD);
    CANName can = (CANName)pinmap_merge(can_rd, can_td);

    MBED_ASSERT((int)can != NC);

    if (can == CAN_1) {
        __HAL_RCC_CAN1_CLK_ENABLE();
        obj->index = 0;
    }
#if defined(CAN2_BASE) && defined(CAN_2)
    else if (can == CAN_2) {
        __HAL_RCC_CAN1_CLK_ENABLE(); // needed to set filters
        __HAL_RCC_CAN2_CLK_ENABLE();
        obj->index = 1;
    }
#endif
#if defined(CAN3_BASE) && defined(CAN_3)
    else if (can == CAN_3) {
        __HAL_RCC_CAN3_CLK_ENABLE();
        obj->index = 2;
    }
#endif
    else {
        return;
    }

    // Configure the CAN pins
    pinmap_pinout(rd, PinMap_CAN_RD);
    pinmap_pinout(td, PinMap_CAN_TD);
    if (rd != NC) {
        pin_mode(rd, PullUp);
    }
    if (td != NC) {
        pin_mode(td, PullUp);
    }

    /*  Use default values for rist init */
    obj->CanHandle.Instance = (CAN_TypeDef *)can;
    obj->CanHandle.Init.TTCM = DISABLE;
    obj->CanHandle.Init.ABOM = DISABLE;
    obj->CanHandle.Init.AWUM = DISABLE;
    obj->CanHandle.Init.NART = DISABLE;
    obj->CanHandle.Init.RFLM = DISABLE;
    obj->CanHandle.Init.TXFP = DISABLE;
    obj->CanHandle.Init.Mode = CAN_MODE_NORMAL;
    obj->CanHandle.Init.SJW = CAN_SJW_1TQ;
    obj->CanHandle.Init.BS1 = CAN_BS1_6TQ;
    obj->CanHandle.Init.BS2 = CAN_BS2_8TQ;
    obj->CanHandle.Init.Prescaler = 2;

    /*  Store frequency to be restored in case of reset */
    obj->hz = hz;

    can_registers_init(obj);

    uint32_t filter_number = (can == CAN_1) ? 0 : 14;
    can_filter(obj, 0, 0, CANStandard, filter_number);
}


void can_irq_init(can_t *obj, can_irq_handler handler, uint32_t id)
{
    irq_handler = handler;
    can_irq_ids[obj->index] = id;
}

void can_irq_free(can_t *obj)
{
    CAN_TypeDef *can = obj->CanHandle.Instance;

    can->IER &= ~(CAN_IT_FMP0 | CAN_IT_FMP1 | CAN_IT_TME | \
                  CAN_IT_ERR | CAN_IT_EPV | CAN_IT_BOF);
    can_irq_ids[obj->index] = 0;
}

void can_free(can_t *obj)
{
    CANName can = (CANName) obj->CanHandle.Instance;
    // Reset CAN and disable clock
    if (can == CAN_1) {
        __HAL_RCC_CAN1_FORCE_RESET();
        __HAL_RCC_CAN1_RELEASE_RESET();
        __HAL_RCC_CAN1_CLK_DISABLE();
    }
#if defined(CAN2_BASE) && defined(CAN_2)
    if (can == CAN_2) {
        __HAL_RCC_CAN2_FORCE_RESET();
        __HAL_RCC_CAN2_RELEASE_RESET();
        __HAL_RCC_CAN2_CLK_DISABLE();
    }
#endif
#if defined(CAN3_BASE) && defined(CAN_3)
    if (can == CAN_3) {
        __HAL_RCC_CAN3_FORCE_RESET();
        __HAL_RCC_CAN3_RELEASE_RESET();
        __HAL_RCC_CAN3_CLK_DISABLE();
    }
#endif
}

// The following table is used to program bit_timing. It is an adjustment of the sample
// point by synchronizing on the start-bit edge and resynchronizing on the following edges.
// This table has the sampling points as close to 75% as possible (most commonly used).
// The first value is TSEG1, the second TSEG2.
static const int timing_pts[23][2] = {
    {0x0, 0x0},      // 2,  50%
    {0x1, 0x0},      // 3,  67%
    {0x2, 0x0},      // 4,  75%
    {0x3, 0x0},      // 5,  80%
    {0x3, 0x1},      // 6,  67%
    {0x4, 0x1},      // 7,  71%
    {0x5, 0x1},      // 8,  75%
    {0x6, 0x1},      // 9,  78%
    {0x6, 0x2},      // 10, 70%
    {0x7, 0x2},      // 11, 73%
    {0x8, 0x2},      // 12, 75%
    {0x9, 0x2},      // 13, 77%
    {0x9, 0x3},      // 14, 71%
    {0xA, 0x3},      // 15, 73%
    {0xB, 0x3},      // 16, 75%
    {0xC, 0x3},      // 17, 76%
    {0xD, 0x3},      // 18, 78%
    {0xD, 0x4},      // 19, 74%
    {0xE, 0x4},      // 20, 75%
    {0xF, 0x4},      // 21, 76%
    {0xF, 0x5},      // 22, 73%
    {0xF, 0x6},      // 23, 70%
    {0xF, 0x7},      // 24, 67%
};

static unsigned int can_speed(unsigned int pclk, unsigned int cclk, unsigned char psjw)
{
    uint32_t    btr;
    uint16_t    brp = 0;
    uint32_t    calcbit;
    uint32_t    bitwidth;
    int         hit = 0;
    int         bits;

    bitwidth = (pclk / cclk);

    brp = bitwidth / 0x18;
    while ((!hit) && (brp < bitwidth / 4)) {
        brp++;
        for (bits = 22; bits > 0; bits--) {
            calcbit = (bits + 3) * (brp + 1);
            if (calcbit == bitwidth) {
                hit = 1;
                break;
            }
        }
    }

    if (hit) {
        btr = ((timing_pts[bits][1] << CAN_BTR_TS2_Pos) & CAN_BTR_TS2) |
              ((timing_pts[bits][0] << CAN_BTR_TS1_Pos) & CAN_BTR_TS1) |
              ((psjw                << CAN_BTR_SJW_Pos) & CAN_BTR_SJW) |
              ((brp                 << CAN_BTR_BRP_Pos) & CAN_BTR_BRP);
    } else {
        btr = 0xFFFFFFFF;
    }

    return btr;

}

int can_frequency(can_t *obj, int f)
{
    int pclk = HAL_RCC_GetPCLK1Freq();
    int btr = can_speed(pclk, (unsigned int)f, 1);
    CAN_TypeDef *can = obj->CanHandle.Instance;
    uint32_t tickstart = 0;
    int status = 1;

    if (btr > 0) {
        can->MCR |= CAN_MCR_INRQ ;
        /* Get tick */
        tickstart = HAL_GetTick();
        while ((can->MSR & CAN_MSR_INAK) != CAN_MSR_INAK) {
            if ((HAL_GetTick() - tickstart) > 2) {
                status = 0;
                break;
            }
        }
        if (status != 0) {
            /*  Do not erase all BTR registers (e.g. silent mode), only the
             *  ones calculated in can_speed */
            can->BTR &= ~(CAN_BTR_TS2 | CAN_BTR_TS1 | CAN_BTR_SJW | CAN_BTR_BRP);
            can->BTR |= btr;

            can->MCR &= ~(uint32_t)CAN_MCR_INRQ;
            /* Get tick */
            tickstart = HAL_GetTick();
            while ((can->MSR & CAN_MSR_INAK) == CAN_MSR_INAK) {
                if ((HAL_GetTick() - tickstart) > 2) {
                    status = 0;
                    break;
                }
            }
            if (status == 0) {
                error("can ESR  0x%04x.%04x + timeout status %d", (can->ESR & 0xFFFF0000) >> 16, (can->ESR & 0xFFFF), status);
            }
        } else {
            error("can init request timeout\n");
        }
    } else {
        status = 0;
    }
    return status;
}

int can_write(can_t *obj, CAN_Message msg, int cc)
{
    uint32_t  transmitmailbox = CAN_TXSTATUS_NOMAILBOX;
    CAN_TypeDef *can = obj->CanHandle.Instance;

    /* Select one empty transmit mailbox */
    if ((can->TSR & CAN_TSR_TME0) == CAN_TSR_TME0) {
        transmitmailbox = 0;
    } else if ((can->TSR & CAN_TSR_TME1) == CAN_TSR_TME1) {
        transmitmailbox = 1;
    } else if ((can->TSR & CAN_TSR_TME2) == CAN_TSR_TME2) {
        transmitmailbox = 2;
    } else {
      return 0;
    }

    can->sTxMailBox[transmitmailbox].TIR &= CAN_TI0R_TXRQ;
    if (!(msg.format)) {
      can->sTxMailBox[transmitmailbox].TIR |= ((msg.id << 21) | msg.type);
    } else {
      can->sTxMailBox[transmitmailbox].TIR |= ((msg.id << 3) | CAN_ID_EXT | msg.type);
    }

    /* Set up the DLC */
    can->sTxMailBox[transmitmailbox].TDTR &= (uint32_t)0xFFFFFFF0;
    can->sTxMailBox[transmitmailbox].TDTR |= (msg.len & (uint8_t)0x0000000F);

    /* Set up the data field */
    can->sTxMailBox[transmitmailbox].TDLR = (((uint32_t)msg.data[3] << 24) |
                                            ((uint32_t)msg.data[2] << 16) |
                                            ((uint32_t)msg.data[1] << 8) |
                                            ((uint32_t)msg.data[0]));
    can->sTxMailBox[transmitmailbox].TDHR = (((uint32_t)msg.data[7] << 24) |
                                            ((uint32_t)msg.data[6] << 16) |
                                            ((uint32_t)msg.data[5] << 8) |
                                                ((uint32_t)msg.data[4]));
    /* Request transmission */
    can->sTxMailBox[transmitmailbox].TIR |= CAN_TI0R_TXRQ;

    return 1;
}

int can_read(can_t *obj, CAN_Message *msg, int handle)
{
    //handle is the FIFO number

    CAN_TypeDef *can = obj->CanHandle.Instance;

    // check FPM0 which holds the pending message count in FIFO 0
    // if no message is pending, return 0
    if ((can->RF0R & CAN_RF0R_FMP0) == 0) {
        return 0;
    }

    /* Get the Id */
    msg->format = (CANFormat)(((uint8_t)0x04 & can->sFIFOMailBox[handle].RIR) >> 2);
    if (!msg->format) {
        msg->id = (uint32_t)0x000007FF & (can->sFIFOMailBox[handle].RIR >> 21);
    } else {
        msg->id = (uint32_t)0x1FFFFFFF & (can->sFIFOMailBox[handle].RIR >> 3);
    }

    msg->type = (CANType)(((uint8_t)0x02 & can->sFIFOMailBox[handle].RIR) >> 1);
    /* Get the DLC */
    msg->len = (uint8_t)0x0F & can->sFIFOMailBox[handle].RDTR;
    /* Get the FMI */
    // msg->FMI = (uint8_t)0xFF & (can->sFIFOMailBox[handle].RDTR >> 8);
    /* Get the data field */
    msg->data[0] = (uint8_t)0xFF & can->sFIFOMailBox[handle].RDLR;
    msg->data[1] = (uint8_t)0xFF & (can->sFIFOMailBox[handle].RDLR >> 8);
    msg->data[2] = (uint8_t)0xFF & (can->sFIFOMailBox[handle].RDLR >> 16);
    msg->data[3] = (uint8_t)0xFF & (can->sFIFOMailBox[handle].RDLR >> 24);
    msg->data[4] = (uint8_t)0xFF & can->sFIFOMailBox[handle].RDHR;
    msg->data[5] = (uint8_t)0xFF & (can->sFIFOMailBox[handle].RDHR >> 8);
    msg->data[6] = (uint8_t)0xFF & (can->sFIFOMailBox[handle].RDHR >> 16);
    msg->data[7] = (uint8_t)0xFF & (can->sFIFOMailBox[handle].RDHR >> 24);

    /* Release the FIFO */
    if (handle == CAN_FIFO0) {
        /* Release FIFO0 */
        can->RF0R |= CAN_RF0R_RFOM0;
    } else { /* FIFONumber == CAN_FIFO1 */
        /* Release FIFO1 */
        can->RF1R |= CAN_RF1R_RFOM1;
    }

    return 1;
}

void can_reset(can_t *obj)
{
    CAN_TypeDef *can = obj->CanHandle.Instance;

    /* Reset IP and delete errors */
    can->MCR |= CAN_MCR_RESET;
    can->ESR = 0x0;

    /* restore registers state as saved in obj context */
    can_registers_init(obj);
}

unsigned char can_rderror(can_t *obj)
{
    CAN_TypeDef *can = obj->CanHandle.Instance;
    return (can->ESR >> 24) & 0xFF;
}

unsigned char can_tderror(can_t *obj)
{
    CAN_TypeDef *can = obj->CanHandle.Instance;
    return (can->ESR >> 16) & 0xFF;
}

void can_monitor(can_t *obj, int silent)
{
    CanMode mode = MODE_NORMAL;
    /*  Update current state w/ or w/o silent */
    if(silent) {
        switch (obj->CanHandle.Init.Mode) {
            case CAN_MODE_LOOPBACK:
            case CAN_MODE_SILENT_LOOPBACK:
                mode = MODE_TEST_SILENT;
                break;
            default:
                mode = MODE_SILENT;
                break;
        }
    } else {
        switch (obj->CanHandle.Init.Mode) {
            case CAN_MODE_LOOPBACK:
            case CAN_MODE_SILENT_LOOPBACK:
                mode = MODE_TEST_LOCAL;
                break;
            default:
                mode = MODE_NORMAL;
                break;
        }
    }

    can_mode(obj, mode);
}

int can_mode(can_t *obj, CanMode mode)
{
    int success = 0;
    CAN_TypeDef *can = obj->CanHandle.Instance;

    can->MCR |= CAN_MCR_INRQ ;
    while ((can->MSR & CAN_MSR_INAK) != CAN_MSR_INAK) {
    }

    switch (mode) {
        case MODE_NORMAL:
            obj->CanHandle.Init.Mode = CAN_MODE_NORMAL;
            can->BTR &= ~(CAN_BTR_SILM | CAN_BTR_LBKM);
            success = 1;
            break;
        case MODE_SILENT:
            obj->CanHandle.Init.Mode = CAN_MODE_SILENT;
            can->BTR |= CAN_BTR_SILM;
            can->BTR &= ~CAN_BTR_LBKM;
            success = 1;
            break;
        case MODE_TEST_GLOBAL:
        case MODE_TEST_LOCAL:
            obj->CanHandle.Init.Mode = CAN_MODE_LOOPBACK;
            can->BTR |= CAN_BTR_LBKM;
            can->BTR &= ~CAN_BTR_SILM;
            success = 1;
            break;
        case MODE_TEST_SILENT:
            obj->CanHandle.Init.Mode = CAN_MODE_SILENT_LOOPBACK;
            can->BTR |= (CAN_BTR_SILM | CAN_BTR_LBKM);
            success = 1;
            break;
        default:
            success = 0;
            break;
    }

    can->MCR &= ~(uint32_t)CAN_MCR_INRQ;
    while ((can->MSR & CAN_MSR_INAK) == CAN_MSR_INAK) {
    }

    return success;
}

int can_filter(can_t *obj, uint32_t id, uint32_t mask, CANFormat format, int32_t handle)
{
    int retval = 0;

    // filter for CANAny format cannot be configured for STM32
    if ((format == CANStandard) || (format == CANExtended)) {
        CAN_FilterConfTypeDef  sFilterConfig;
        sFilterConfig.FilterNumber = handle;
        sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
        sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;

        if (format == CANStandard) {
            sFilterConfig.FilterIdHigh = id << 5;
            sFilterConfig.FilterIdLow =  0x0;
            sFilterConfig.FilterMaskIdHigh = mask << 5;
            sFilterConfig.FilterMaskIdLow = 0x0; // allows both remote and data frames
        } else if (format == CANExtended) {
            sFilterConfig.FilterIdHigh = id >> 13; // EXTID[28:13]
            sFilterConfig.FilterIdLow = (0x00FF & (id << 3)) | (1 << 2);  // EXTID[12:0]
            sFilterConfig.FilterMaskIdHigh = mask >> 13;
            sFilterConfig.FilterMaskIdLow = (0x00FF & (mask << 3)) | (1 << 2);
        }

        sFilterConfig.FilterFIFOAssignment = 0;
        sFilterConfig.FilterActivation = ENABLE;
        sFilterConfig.BankNumber = 14 + handle;

        HAL_CAN_ConfigFilter(&obj->CanHandle, &sFilterConfig);
        retval = handle;
    }
    return retval;
}

static void can_irq(CANName name, int id)
{
    uint32_t tmp1 = 0, tmp2 = 0, tmp3 = 0;
    CAN_HandleTypeDef CanHandle;
    CanHandle.Instance = (CAN_TypeDef *)name;

    if (__HAL_CAN_GET_IT_SOURCE(&CanHandle, CAN_IT_TME)) {
        tmp1 = __HAL_CAN_TRANSMIT_STATUS(&CanHandle, CAN_TXMAILBOX_0);
        tmp2 = __HAL_CAN_TRANSMIT_STATUS(&CanHandle, CAN_TXMAILBOX_1);
        tmp3 = __HAL_CAN_TRANSMIT_STATUS(&CanHandle, CAN_TXMAILBOX_2);
        if (tmp1) {
            __HAL_CAN_CLEAR_FLAG(&CanHandle, CAN_FLAG_RQCP0);
        }
        if (tmp2) {
            __HAL_CAN_CLEAR_FLAG(&CanHandle, CAN_FLAG_RQCP1);
        }
        if (tmp3) {
            __HAL_CAN_CLEAR_FLAG(&CanHandle, CAN_FLAG_RQCP2);
        }
        if (tmp1 || tmp2 || tmp3) {
            irq_handler(can_irq_ids[id], IRQ_TX);
        }
    }

    tmp1 = __HAL_CAN_MSG_PENDING(&CanHandle, CAN_FIFO0);
    tmp2 = __HAL_CAN_GET_IT_SOURCE(&CanHandle, CAN_IT_FMP0);

    if ((tmp1 != 0) && tmp2) {
        irq_handler(can_irq_ids[id], IRQ_RX);
    }

    tmp1 = __HAL_CAN_GET_FLAG(&CanHandle, CAN_FLAG_EPV);
    tmp2 = __HAL_CAN_GET_IT_SOURCE(&CanHandle, CAN_IT_EPV);
    tmp3 = __HAL_CAN_GET_IT_SOURCE(&CanHandle, CAN_IT_ERR);

    if (tmp1 && tmp2 && tmp3) {
        irq_handler(can_irq_ids[id], IRQ_PASSIVE);
    }

    tmp1 = __HAL_CAN_GET_FLAG(&CanHandle, CAN_FLAG_BOF);
    tmp2 = __HAL_CAN_GET_IT_SOURCE(&CanHandle, CAN_IT_BOF);
    tmp3 = __HAL_CAN_GET_IT_SOURCE(&CanHandle, CAN_IT_ERR);
    if (tmp1 && tmp2 && tmp3) {
        irq_handler(can_irq_ids[id], IRQ_BUS);
    }

    tmp3 = __HAL_CAN_GET_IT_SOURCE(&CanHandle, CAN_IT_ERR);
    if (tmp1 && tmp2 && tmp3) {
        irq_handler(can_irq_ids[id], IRQ_ERROR);
    }
}

#if defined(TARGET_STM32F0)
void CAN_IRQHandler(void)
{
    can_irq(CAN_1, 0);
}
#elif defined(TARGET_STM32F3)
void CAN_RX0_IRQHandler(void)
{
    can_irq(CAN_1, 0);
}
void CAN_TX_IRQHandler(void)
{
    can_irq(CAN_1, 0);
}
void CAN_SCE_IRQHandler(void)
{
    can_irq(CAN_1, 0);
}
#else
void CAN1_RX0_IRQHandler(void)
{
    can_irq(CAN_1, 0);
}
void CAN1_TX_IRQHandler(void)
{
    can_irq(CAN_1, 0);
}
void CAN1_SCE_IRQHandler(void)
{
    can_irq(CAN_1, 0);
}
#if defined(CAN2_BASE) && defined(CAN_2)
void CAN2_RX0_IRQHandler(void)
{
    can_irq(CAN_2, 1);
}
void CAN2_TX_IRQHandler(void)
{
    can_irq(CAN_2, 1);
}
void CAN2_SCE_IRQHandler(void)
{
    can_irq(CAN_2, 1);
}
#endif
#if defined(CAN3_BASE) && defined(CAN_3)
void CAN3_RX0_IRQHandler(void)
{
    can_irq(CAN_3, 1);
}
void CAN3_TX_IRQHandler(void)
{
    can_irq(CAN_3, 1);
}
void CAN3_SCE_IRQHandler(void)
{
    can_irq(CAN_3, 1);
}
#endif
#endif // else

void can_irq_set(can_t *obj, CanIrqType type, uint32_t enable)
{
    CAN_TypeDef *can = obj->CanHandle.Instance;
    IRQn_Type irq_n = (IRQn_Type)0;
    uint32_t vector = 0;
    uint32_t ier;

    if ((CANName) can == CAN_1) {
        switch (type) {
            case IRQ_RX:
                ier = CAN_IT_FMP0;
                irq_n = CAN1_IRQ_RX_IRQN;
                vector = (uint32_t)&CAN1_IRQ_RX_VECT;
                break;
            case IRQ_TX:
                ier = CAN_IT_TME;
                irq_n = CAN1_IRQ_TX_IRQN;
                vector = (uint32_t)&CAN1_IRQ_TX_VECT;
                break;
            case IRQ_ERROR:
                ier = CAN_IT_ERR;
                irq_n = CAN1_IRQ_ERROR_IRQN;
                vector = (uint32_t)&CAN1_IRQ_ERROR_VECT;
                break;
            case IRQ_PASSIVE:
                ier = CAN_IT_EPV;
                irq_n = CAN1_IRQ_PASSIVE_IRQN;
                vector = (uint32_t)&CAN1_IRQ_PASSIVE_VECT;
                break;
            case IRQ_BUS:
                ier = CAN_IT_BOF;
                irq_n = CAN1_IRQ_BUS_IRQN;
                vector = (uint32_t)&CAN1_IRQ_BUS_VECT;
                break;
            default:
                return;
        }
    }
#if defined(CAN2_BASE) && defined(CAN_2)
    else if ((CANName) can == CAN_2) {
        switch (type) {
            case IRQ_RX:
                ier = CAN_IT_FMP0;
                irq_n = CAN2_IRQ_RX_IRQN;
                vector = (uint32_t)&CAN2_IRQ_RX_VECT;
                break;
            case IRQ_TX:
                ier = CAN_IT_TME;
                irq_n = CAN2_IRQ_TX_IRQN;
                vector = (uint32_t)&CAN2_IRQ_TX_VECT;
                break;
            case IRQ_ERROR:
                ier = CAN_IT_ERR;
                irq_n = CAN2_IRQ_ERROR_IRQN;
                vector = (uint32_t)&CAN2_IRQ_ERROR_VECT;
                break;
            case IRQ_PASSIVE:
                ier = CAN_IT_EPV;
                irq_n = CAN2_IRQ_PASSIVE_IRQN;
                vector = (uint32_t)&CAN2_IRQ_PASSIVE_VECT;
                break;
            case IRQ_BUS:
                ier = CAN_IT_BOF;
                irq_n = CAN2_IRQ_BUS_IRQN;
                vector = (uint32_t)&CAN2_IRQ_BUS_VECT;
                break;
            default:
                return;
        }
    }
#endif
#if defined(CAN3_BASE) && defined(CAN_3)
    else if ((CANName) can == CAN_3) {
        switch (type) {
            case IRQ_RX:
                ier = CAN_IT_FMP0;
                irq_n = CAN3_IRQ_RX_IRQN;
                vector = (uint32_t)&CAN3_IRQ_RX_VECT;
                break;
            case IRQ_TX:
                ier = CAN_IT_TME;
                irq_n = CAN3_IRQ_TX_IRQN;
                vector = (uint32_t)&CAN3_IRQ_TX_VECT;
                break;
            case IRQ_ERROR:
                ier = CAN_IT_ERR;
                irq_n = CAN3_IRQ_ERROR_IRQN;
                vector = (uint32_t)&CAN3_IRQ_ERROR_VECT;
                break;
            case IRQ_PASSIVE:
                ier = CAN_IT_EPV;
                irq_n = CAN3_IRQ_PASSIVE_IRQN;
                vector = (uint32_t)&CAN3_IRQ_PASSIVE_VECT;
                break;
            case IRQ_BUS:
                ier = CAN_IT_BOF;
                irq_n = CAN3_IRQ_BUS_IRQN;
                vector = (uint32_t)&CAN3_IRQ_BUS_VECT;
                break;
            default:
                return;
        }
    }
#endif
    else {
        return;
    }

    if (enable) {
        can->IER |= ier;
    } else {
        can->IER &= ~ier;
    }

    NVIC_SetVector(irq_n, vector);
    NVIC_EnableIRQ(irq_n);
}

#endif // DEVICE_CAN

