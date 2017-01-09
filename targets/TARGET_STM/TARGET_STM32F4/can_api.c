/* mbed Microcontroller Library
 * Copyright (c) 2006-2016 ARM Limited
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
#include <math.h>
#include <string.h>

#define CAN_NUM    2
static CAN_HandleTypeDef CanHandle;
static uint32_t can_irq_ids[CAN_NUM] = {0};
static can_irq_handler irq_handler;

void can_init(can_t *obj, PinName rd, PinName td) 
{
    uint32_t filter_number;
    CANName can_rd = (CANName)pinmap_peripheral(rd, PinMap_CAN_RD);
    CANName can_td = (CANName)pinmap_peripheral(td, PinMap_CAN_TD);
    obj->can = (CANName)pinmap_merge(can_rd, can_td);
    MBED_ASSERT((int)obj->can != NC);    

    if(obj->can == CAN_1) {
        __HAL_RCC_CAN1_CLK_ENABLE();
        obj->index = 0;
    } else {
        __HAL_RCC_CAN2_CLK_ENABLE();
        obj->index = 1;
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

    CanHandle.Instance = (CAN_TypeDef *)(obj->can);

    CanHandle.Init.TTCM = DISABLE;
    CanHandle.Init.ABOM = DISABLE;
    CanHandle.Init.AWUM = DISABLE;
    CanHandle.Init.NART = DISABLE;
    CanHandle.Init.RFLM = DISABLE;
    CanHandle.Init.TXFP = DISABLE;
    CanHandle.Init.Mode = CAN_MODE_NORMAL;
    CanHandle.Init.SJW = CAN_SJW_1TQ;
    CanHandle.Init.BS1 = CAN_BS1_6TQ;
    CanHandle.Init.BS2 = CAN_BS2_8TQ;
    CanHandle.Init.Prescaler = 2;

    if (HAL_CAN_Init(&CanHandle) != HAL_OK) {
       error("Cannot initialize CAN");
    }

    filter_number = (obj->can == CAN_1) ? 0 : 14;

    // Set initial CAN frequency to 100kb/s
    can_frequency(obj, 100000);

    can_filter(obj, 0, 0, CANStandard, filter_number);
}

void can_irq_init(can_t *obj, can_irq_handler handler, uint32_t id) 
{
    irq_handler = handler;
    can_irq_ids[obj->index] = id;    
}

void can_irq_free(can_t *obj) 
{
    CAN_TypeDef *can = (CAN_TypeDef *)(obj->can);
  
    can->IER &= ~(CAN_IT_FMP0 | CAN_IT_FMP1 | CAN_IT_TME | \
                  CAN_IT_ERR | CAN_IT_EPV | CAN_IT_BOF);
    can_irq_ids[obj->can] = 0;
}

void can_free(can_t *obj) 
{
    // Reset CAN and disable clock
    if (obj->can == CAN_1) {
        __HAL_RCC_CAN1_FORCE_RESET();
        __HAL_RCC_CAN1_RELEASE_RESET();
        __HAL_RCC_CAN1_CLK_DISABLE();
    }

    if (obj->can == CAN_2) {
        __HAL_RCC_CAN2_FORCE_RESET();
        __HAL_RCC_CAN2_RELEASE_RESET();
        __HAL_RCC_CAN2_CLK_DISABLE();
    }
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
        btr = ((timing_pts[bits][1] << 20) & 0x00700000)
            | ((timing_pts[bits][0] << 16) & 0x000F0000)
            | ((psjw                << 24) & 0x0000C000)
            | ((brp                 <<  0) & 0x000003FF);
    } else {
        btr = 0xFFFFFFFF;
    }
    
    return btr;

}

int can_frequency(can_t *obj, int f) 
{
    int pclk = HAL_RCC_GetPCLK1Freq();
    int btr = can_speed(pclk, (unsigned int)f, 1);
    CAN_TypeDef *can = (CAN_TypeDef *)(obj->can);

    if (btr > 0) {
        can->MCR |= CAN_MCR_INRQ ;
        while((can->MSR & CAN_MSR_INAK) != CAN_MSR_INAK) {
        }
        can->BTR = btr;
        can->MCR &= ~(uint32_t)CAN_MCR_INRQ;
        while((can->MSR & CAN_MSR_INAK) == CAN_MSR_INAK) {
        }
        return 1;
    } else {
        return 0;
    }
}

int can_write(can_t *obj, CAN_Message msg, int cc) 
{
    uint32_t  transmitmailbox = 5;
    CAN_TypeDef *can = (CAN_TypeDef *)(obj->can);

    /* Select one empty transmit mailbox */
    if ((can->TSR&CAN_TSR_TME0) == CAN_TSR_TME0) {
        transmitmailbox = 0;
    } else if ((can->TSR&CAN_TSR_TME1) == CAN_TSR_TME1) {
        transmitmailbox = 1;
    } else if ((can->TSR&CAN_TSR_TME2) == CAN_TSR_TME2) {
        transmitmailbox = 2;
    } else {
        transmitmailbox = CAN_TXSTATUS_NOMAILBOX;
    }

    if (transmitmailbox != CAN_TXSTATUS_NOMAILBOX) {
    can->sTxMailBox[transmitmailbox].TIR &= CAN_TI0R_TXRQ;
    if (!(msg.format))
    {
      can->sTxMailBox[transmitmailbox].TIR |= ((msg.id << 21) | msg.type);
    }
    else
    {
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
  }

  return 1;
}

int can_read(can_t *obj, CAN_Message *msg, int handle) 
{
    //handle is the FIFO number

    CAN_TypeDef *can = (CAN_TypeDef *)(obj->can);    
    
    // check FPM0 which holds the pending message count in FIFO 0
    // if no message is pending, return 0
    if ((can->RF0R & CAN_RF0R_FMP0) == 0) {
        return 0;
    }

    /* Get the Id */
    msg->format = (CANFormat)((uint8_t)0x04 & can->sFIFOMailBox[handle].RIR);
    if (!msg->format) {
        msg->id = (uint32_t)0x000007FF & (can->sFIFOMailBox[handle].RIR >> 21);
    } else {
        msg->id = (uint32_t)0x1FFFFFFF & (can->sFIFOMailBox[handle].RIR >> 3);
    }
      
    msg->type = (CANType)((uint8_t)0x02 & can->sFIFOMailBox[handle].RIR);
    /* Get the DLC */
    msg->len = (uint8_t)0x0F & can->sFIFOMailBox[handle].RDTR;
//  /* Get the FMI */
//  msg->FMI = (uint8_t)0xFF & (can->sFIFOMailBox[handle].RDTR >> 8);
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
    if(handle == CAN_FIFO0) {
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
    CAN_TypeDef *can = (CAN_TypeDef *)(obj->can);

    can->MCR |= CAN_MCR_RESET;  
    can->ESR = 0x0;
}

unsigned char can_rderror(can_t *obj) 
{
    CAN_TypeDef *can = (CAN_TypeDef *)(obj->can);    
    return (can->ESR >> 24) & 0xFF;
}

unsigned char can_tderror(can_t *obj) 
{
    CAN_TypeDef *can = (CAN_TypeDef *)(obj->can);    
    return (can->ESR >> 16) & 0xFF;
}

void can_monitor(can_t *obj, int silent) 
{
    CAN_TypeDef *can = (CAN_TypeDef *)(obj->can);        
    
    can->MCR |= CAN_MCR_INRQ ;
    while((can->MSR & CAN_MSR_INAK) != CAN_MSR_INAK) {
    }
    if (silent) {
        can->BTR |= ((uint32_t)1 << 31);
    } else {
        can->BTR &= ~((uint32_t)1 << 31);
    }
    can->MCR &= ~(uint32_t)CAN_MCR_INRQ;
    while((can->MSR & CAN_MSR_INAK) == CAN_MSR_INAK) {
    }
}

int can_mode(can_t *obj, CanMode mode) 
{
    int success = 0;
    CAN_TypeDef *can = (CAN_TypeDef *)(obj->can);
    can->MCR |= CAN_MCR_INRQ ;
    while((can->MSR & CAN_MSR_INAK) != CAN_MSR_INAK) {
    }
    switch (mode) {
        case MODE_NORMAL:
            can->BTR &= ~(CAN_BTR_SILM | CAN_BTR_LBKM);
            success = 1;
            break;
        case MODE_SILENT:
            can->BTR |= CAN_BTR_SILM;
            can->BTR &= ~CAN_BTR_LBKM;
            success = 1;
            break;
        case MODE_TEST_GLOBAL:
        case MODE_TEST_LOCAL:
            can->BTR |= CAN_BTR_LBKM;
            can->BTR &= ~CAN_BTR_SILM;
            success = 1;
            break;
        case MODE_TEST_SILENT:
            can->BTR |= (CAN_BTR_SILM | CAN_BTR_LBKM);
            success = 1;
            break;
        default:
            success = 0;
            break;
    }
    can->MCR &= ~(uint32_t)CAN_MCR_INRQ;
    while((can->MSR & CAN_MSR_INAK) == CAN_MSR_INAK) {
    }
    return success;
}

int can_filter(can_t *obj, uint32_t id, uint32_t mask, CANFormat format, int32_t handle) 
{
    CanHandle.Instance = (CAN_TypeDef *)(obj->can);
    CAN_FilterConfTypeDef  sFilterConfig;
    
    sFilterConfig.FilterNumber = handle;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh = (uint8_t) (id >> 8);
    sFilterConfig.FilterIdLow = (uint8_t) id;
    sFilterConfig.FilterMaskIdHigh = (uint8_t) (mask >> 8);
    sFilterConfig.FilterMaskIdLow = (uint8_t) mask;
    sFilterConfig.FilterFIFOAssignment = 0;
    sFilterConfig.FilterActivation = ENABLE;
    sFilterConfig.BankNumber = 14 + handle;
  
    HAL_CAN_ConfigFilter(&CanHandle, &sFilterConfig);
      
    return 0;
}

static void can_irq(CANName name, int id) 
{
    uint32_t tmp1 = 0, tmp2 = 0, tmp3 = 0;    
    CanHandle.Instance = (CAN_TypeDef *)name;
    
    if(__HAL_CAN_GET_IT_SOURCE(&CanHandle, CAN_IT_TME)) {
        tmp1 = __HAL_CAN_TRANSMIT_STATUS(&CanHandle, CAN_TXMAILBOX_0);
        tmp2 = __HAL_CAN_TRANSMIT_STATUS(&CanHandle, CAN_TXMAILBOX_1);
        tmp3 = __HAL_CAN_TRANSMIT_STATUS(&CanHandle, CAN_TXMAILBOX_2);
        if(tmp1 || tmp2 || tmp3)  
        {
            irq_handler(can_irq_ids[id], IRQ_TX);
        }
    }
  
    tmp1 = __HAL_CAN_MSG_PENDING(&CanHandle, CAN_FIFO0);
    tmp2 = __HAL_CAN_GET_IT_SOURCE(&CanHandle, CAN_IT_FMP0);
  
    if((tmp1 != 0) && tmp2) {
         irq_handler(can_irq_ids[id], IRQ_RX);
    }
  
    tmp1 = __HAL_CAN_GET_FLAG(&CanHandle, CAN_FLAG_EPV);
    tmp2 = __HAL_CAN_GET_IT_SOURCE(&CanHandle, CAN_IT_EPV);
    tmp3 = __HAL_CAN_GET_IT_SOURCE(&CanHandle, CAN_IT_ERR); 
  
    if(tmp1 && tmp2 && tmp3) {
         irq_handler(can_irq_ids[id], IRQ_PASSIVE);
    }
  
    tmp1 = __HAL_CAN_GET_FLAG(&CanHandle, CAN_FLAG_BOF);
    tmp2 = __HAL_CAN_GET_IT_SOURCE(&CanHandle, CAN_IT_BOF);
    tmp3 = __HAL_CAN_GET_IT_SOURCE(&CanHandle, CAN_IT_ERR);  
    if(tmp1 && tmp2 && tmp3) {
        irq_handler(can_irq_ids[id], IRQ_BUS);
    }
  
    tmp3 = __HAL_CAN_GET_IT_SOURCE(&CanHandle, CAN_IT_ERR);  
    if(tmp1 && tmp2 && tmp3) {
        irq_handler(can_irq_ids[id], IRQ_ERROR);
    }  
}

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

void can_irq_set(can_t *obj, CanIrqType type, uint32_t enable)
{

    CAN_TypeDef *can = (CAN_TypeDef *)(obj->can);
    IRQn_Type irq_n = (IRQn_Type)0;
    uint32_t vector = 0;    
    uint32_t ier;

    if(obj->can == CAN_1) {
        switch (type) {
            case IRQ_RX:
                ier = CAN_IT_FMP0;
                irq_n = CAN1_RX0_IRQn;
                vector = (uint32_t)&CAN1_RX0_IRQHandler;
                break;
            case IRQ_TX:      
                ier = CAN_IT_TME;
                irq_n = CAN1_TX_IRQn;
                vector = (uint32_t)&CAN1_TX_IRQHandler;
                break;
            case IRQ_ERROR:   
                ier = CAN_IT_ERR;
                irq_n = CAN1_SCE_IRQn;
                vector = (uint32_t)&CAN1_SCE_IRQHandler;
                break;
            case IRQ_PASSIVE:
                ier = CAN_IT_EPV;
                irq_n = CAN1_SCE_IRQn;
                vector = (uint32_t)&CAN1_SCE_IRQHandler;
                break;
            case IRQ_BUS:
                ier = CAN_IT_BOF;
                irq_n = CAN1_SCE_IRQn;
                vector = (uint32_t)&CAN1_SCE_IRQHandler;
                break;
            default: return;
        }
    } else {
        switch (type) {
              case IRQ_RX:
                  ier = CAN_IT_FMP0;
                  irq_n = CAN2_RX0_IRQn;
                  vector = (uint32_t)&CAN2_RX0_IRQHandler;
                  break;
              case IRQ_TX:      
                  ier = CAN_IT_TME;
                  irq_n = CAN2_TX_IRQn;
                  vector = (uint32_t)&CAN2_TX_IRQHandler;
                  break;
              case IRQ_ERROR:   
                  ier = CAN_IT_ERR;
                  irq_n = CAN2_SCE_IRQn;
                  vector = (uint32_t)&CAN2_SCE_IRQHandler;
                  break;
              case IRQ_PASSIVE:
                  ier = CAN_IT_EPV;
                  irq_n = CAN2_SCE_IRQn;
                  vector = (uint32_t)&CAN2_SCE_IRQHandler;
                  break;
              case IRQ_BUS:
                  ier = CAN_IT_BOF;
                  irq_n = CAN2_SCE_IRQn;
                  vector = (uint32_t)&CAN2_SCE_IRQHandler;
                  break;
              default: return;
        }
    }

    if(enable) {
        can->IER |= ier;
    } else {
        can->IER &= ~ier;
    }

    NVIC_SetVector(irq_n, vector);
    NVIC_EnableIRQ(irq_n);
}

#endif // DEVICE_CAN

