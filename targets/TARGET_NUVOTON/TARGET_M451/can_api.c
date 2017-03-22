/* mbed Microcontroller Library
 * Copyright (c) 2015-2016 Nuvoton
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
 #include "m451_gpio.h"
 #include "m451_can.h"
 
 #if DEVICE_CAN
 #include <string.h>
 #include "cmsis.h"
 #include "pinmap.h"
 #include "PeripheralPins.h"
 #include "nu_modutil.h"
 #include "nu_miscutil.h"
 #include "nu_bitutil.h"
 #include "mbed_critical.h"
 
 #define NU_CAN_DEBUG    0
 #define CAN_NUM         1
 
 static uint32_t can_irq_ids[CAN_NUM] = {0};
 static can_irq_handler can0_irq_handler;

 
 static const struct nu_modinit_s can_modinit_tab[] = {
    {CAN_0, CAN0_MODULE, 0, 0, CAN0_RST, CAN0_IRQn, NULL},
    
    {NC, 0, 0, 0, 0, (IRQn_Type) 0, NULL}
};

 
 void can_init(can_t *obj, PinName rd, PinName td)
 {
    uint32_t can_td = (CANName)pinmap_peripheral(td, PinMap_CAN_TD);
    uint32_t can_rd = (CANName)pinmap_peripheral(rd, PinMap_CAN_RD);
    obj->can = (CANName)pinmap_merge(can_td, can_rd);
    MBED_ASSERT((int)obj->can != NC);
    
    const struct nu_modinit_s *modinit = get_modinit(obj->can, can_modinit_tab);
    MBED_ASSERT(modinit != NULL);
    MBED_ASSERT(modinit->modname == obj->can);
    
    // Reset this module
    SYS_ResetModule(modinit->rsetidx);
    
    // Enable IP clock
    CLK_EnableModuleClock(modinit->clkidx);
     
    obj->index = 0;
    
    pinmap_pinout(td, PinMap_CAN_TD);
    pinmap_pinout(rd, PinMap_CAN_RD);
    
    /* For M453 mbed Board Transmitter Setting (RS Pin) */
    GPIO_SetMode(PA, BIT0| BIT1, GPIO_MODE_OUTPUT);    
    PA0 = 0x00;
    PA1 = 0x00;   

    CAN_Open((CAN_T *)NU_MODBASE(obj->can), 500000, CAN_NORMAL_MODE);
    
    can_filter(obj, 0, 0, CANStandard, 0);
 }
 
 
void can_free(can_t *obj)
{

    const struct nu_modinit_s *modinit = get_modinit(obj->can, can_modinit_tab);
    
    MBED_ASSERT(modinit != NULL);
    MBED_ASSERT(modinit->modname == obj->can);
    
    // Reset this module
    SYS_ResetModule(modinit->rsetidx);
    
    CLK_DisableModuleClock(modinit->clkidx);
}

int can_frequency(can_t *obj, int hz)
{
    CAN_SetBaudRate((CAN_T *)NU_MODBASE(obj->can), hz);
    
    return CAN_GetCANBitRate((CAN_T *)NU_MODBASE(obj->can));
}

static void can_irq(CANName name, int id) 
{
    
    CAN_T *can = (CAN_T *)NU_MODBASE(name);
    uint32_t u8IIDRstatus;

    u8IIDRstatus = can->IIDR;

    if(u8IIDRstatus == 0x00008000) {      /* Check Status Interrupt Flag (Error status Int and Status change Int) */
        /**************************/
        /* Status Change interrupt*/
        /**************************/
        if(can->STATUS & CAN_STATUS_RXOK_Msk) {
            can->STATUS &= ~CAN_STATUS_RXOK_Msk;   /* Clear Rx Ok status*/
            can0_irq_handler(can_irq_ids[id], IRQ_RX);
        }

        if(can->STATUS & CAN_STATUS_TXOK_Msk) {
            can->STATUS &= ~CAN_STATUS_TXOK_Msk;    /* Clear Tx Ok status*/
            can0_irq_handler(can_irq_ids[id], IRQ_TX);
        }

        /**************************/
        /* Error Status interrupt */
        /**************************/
        if(can->STATUS & CAN_STATUS_EWARN_Msk) {
            can0_irq_handler(can_irq_ids[id], IRQ_ERROR);
        }

        if(can->STATUS & CAN_STATUS_BOFF_Msk) {
            can0_irq_handler(can_irq_ids[id], IRQ_BUS);
        }
    } else if (u8IIDRstatus!=0) {

        can0_irq_handler(can_irq_ids[id], IRQ_OVERRUN);
        
        CAN_CLR_INT_PENDING_BIT(can, ((can->IIDR) -1));      /* Clear Interrupt Pending */

    } else if(can->WU_STATUS == 1) {

        can->WU_STATUS = 0;                       /* Write '0' to clear */
        can0_irq_handler(can_irq_ids[id], IRQ_WAKEUP);
    }
}

void CAN0_IRQHandler(void)
{
    can_irq(CAN_0, 0);
}

void can_irq_init(can_t *obj, can_irq_handler handler, uint32_t id)
{
    can0_irq_handler = handler;
    can_irq_ids[obj->index] = id; 
}

void can_irq_free(can_t *obj)
{
    CAN_DisableInt((CAN_T *)NU_MODBASE(obj->can), (CAN_CON_IE_Msk|CAN_CON_SIE_Msk|CAN_CON_EIE_Msk));
    
    can_irq_ids[obj->index] = 0;
    
    NVIC_DisableIRQ(CAN0_IRQn);    
}

void can_irq_set(can_t *obj, CanIrqType irq, uint32_t enable)
{
    CAN_T *can_base = (CAN_T *) NU_MODBASE(obj->can);
    
    CAN_EnterInitMode(can_base, ((enable != 0 )? CAN_CON_IE_Msk :0) );
    
    
    switch (irq)
    {
        case IRQ_ERROR:
        case IRQ_BUS:
        case IRQ_PASSIVE:
            can_base->CON = can_base->CON |CAN_CON_EIE_Msk;
            can_base->CON = can_base->CON |CAN_CON_SIE_Msk;
            break;
        
        case IRQ_RX:
        case IRQ_TX:
        case IRQ_OVERRUN:
        case IRQ_WAKEUP:
            can_base->CON = can_base->CON |CAN_CON_SIE_Msk;
            break;
        
        default:
            break;
    
    }

    CAN_LeaveInitMode(can_base);
    
    NVIC_SetVector(CAN0_IRQn, (uint32_t)&CAN0_IRQHandler);
    NVIC_EnableIRQ(CAN0_IRQn);
    
}

int can_write(can_t *obj, CAN_Message msg, int cc)
{
    STR_CANMSG_T CMsg;
    
    CMsg.IdType = (uint32_t)msg.format;
    CMsg.FrameType = (uint32_t)!msg.type;
    CMsg.Id = msg.id;
    CMsg.DLC = msg.len;
    memcpy((void *)&CMsg.Data[0],(const void *)&msg.data[0], (unsigned int)8);

    return CAN_Transmit((CAN_T *)NU_MODBASE(obj->can), cc, &CMsg);
}

int can_read(can_t *obj, CAN_Message *msg, int handle)
{
    STR_CANMSG_T CMsg;

    if(!CAN_Receive((CAN_T *)NU_MODBASE(obj->can), handle, &CMsg))
    return 0;
        
    msg->format = (CANFormat)CMsg.IdType;
    msg->type = (CANType)!CMsg.FrameType;
    msg->id = CMsg.Id;
    msg->len = CMsg.DLC;
    memcpy(&msg->data[0], &CMsg.Data[0], 8);
    
    return 1;
}

int can_mode(can_t *obj, CanMode mode)
{
    CAN_T *can_base = (CAN_T *) NU_MODBASE(obj->can);
    
    int success = 0;
    switch (mode)
    {
        case MODE_RESET:
            CAN_LeaveTestMode(can_base);
            success = 1;
            break;
        
        case MODE_NORMAL:
            CAN_EnterTestMode(can_base, CAN_TEST_BASIC_Msk);
            success = 1;
            break;
        
        case MODE_SILENT:
            CAN_EnterTestMode(can_base, CAN_TEST_SILENT_Msk);
            success = 1;
            break;
        
        case MODE_TEST_LOCAL:
        case MODE_TEST_GLOBAL:
            CAN_EnterTestMode(can_base, CAN_TEST_LBACK_Msk);
            success = 1;
            break;
        
        case MODE_TEST_SILENT:
            CAN_EnterTestMode(can_base, CAN_TEST_SILENT_Msk | CAN_TEST_LBACK_Msk);
            success = 1;
            break;
        
        default:
            success = 0;
            break;
        
    }
    
    
    return success;
}

int can_filter(can_t *obj, uint32_t id, uint32_t mask, CANFormat format, int32_t handle)
{
    return CAN_SetRxMsg((CAN_T *)NU_MODBASE(obj->can), handle , (uint32_t)format, id);
}


void can_reset(can_t *obj)
{
    const struct nu_modinit_s *modinit = get_modinit(obj->can, can_modinit_tab);
    
    MBED_ASSERT(modinit != NULL);
    MBED_ASSERT(modinit->modname == obj->can);
    
    // Reset this module
    SYS_ResetModule(modinit->rsetidx);
    
}

unsigned char can_rderror(can_t *obj)
{
    CAN_T *can = (CAN_T *)NU_MODBASE(obj->can); 
    return ((can->ERR>>8)&0xFF);
}

unsigned char can_tderror(can_t *obj)
{
    CAN_T *can = (CAN_T *)NU_MODBASE(obj->can);
    return ((can->ERR)&0xFF);
}

void can_monitor(can_t *obj, int silent)
{
    CAN_EnterTestMode((CAN_T *)NU_MODBASE(obj->can), CAN_TEST_SILENT_Msk);
}
 
#endif // DEVICE_CAN
