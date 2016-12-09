/* mbed Microcontroller Library
 * Copyright (c) 2006-2016 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *	 http://www.apache.org/licenses/LICENSE-2.0
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
#include "mbed_error.h"

#include <math.h>
#include <string.h>
#include "pinmap.h"



#define BFN_PREP(x, name)       ( ((x)<<name##_SHIFT) & name##_MASK )
#define BFN_GET(y, name)        ( ((y) & name##_MASK)>>name##_SHIFT )
#define BFN_SET(y, x, name)     (y = ((y)&~name##_MASK) | BFN_PREP(x,name) )

/* CANCNTL, address 0x4005 0000 */
#define CANCNTL_INIT                (1 << 0)		   // Initialization
#define CANCNTL_IE					(1 << 1)		   // Module interrupt enable
#define CANCNTL_SIE					(1 << 2)		   // Status change interrupt enable
#define CANCNTL_EIE					(1 << 3)		   // Error interrupt enable
#define CANCNTL_DAR					(1 << 5)		   // Disable automatic retransmission
#define CANCNTL_CCE					(1 << 6)		   // Configuration change enable
#define CANCNTL_TEST				(1 << 7)		   // Test mode enable

/* CANSTAT, address 0x4005 0004 */
#define CANSTAT_LEC_MASK			 0x0007			 // Last error code Type of the last error to occur on the CAN bus.The LEC field holds a code which indicates the type of the last error to occur on the CAN bus. This field will be cleared to `0' when a message has been transferred (reception or transmission) without error. The unused code `111' may be written by the CPU to check for updates.
#define CANSTAT_LEC_SHIFT			 0				 
#define CANSTAT_TXOK				 (1 << 3)		   // Transmitted a message successfully This bit must be reset by the CPU. It is never reset by the CAN controller.
#define CANSTAT_RXOK				 (1 << 4)		   // Received a message successfully This bit must be reset by the CPU. It is never reset by the CAN controller.
#define CANSTAT_EPASS				 (1 << 5)		   // Error passive
#define CANSTAT_EWARN				 (1 << 6)		   // Warning status
#define CANSTAT_BOFF				 (1 << 7)		   // Busoff status

/* CANEC, address 0x4005 0008 */
#define CANEC_TEC_MASK				 0x00FF			 // Transmit error counter Current value of the transmit error counter (maximum value 255)
#define CANEC_TEC_SHIFT				 0				 
#define CANEC_REC_MASK				 0x7F00			 // Receive error counter Current value of the receive error counter (maximum value 127).
#define CANEC_REC_SHIFT				 8				 
#define CANEC_RP					 (1 << 15)		  // Receive error passive

/* CANBT, address 0x4005 000C */
#define CANBT_BRP_MASK				 0x003F			 // Baud rate prescaler The value by which the oscillator frequency is divided for generating the bit time quanta. The bit time is built up from a multiple of this quanta. Valid values for the Baud Rate Prescaler are 0 to 63.[1]
#define CANBT_BRP_SHIFT				 0				 
#define CANBT_SJW_MASK				 0x00C0			 // (Re)synchronization jump width Valid programmed values are 0 to 3.[1]
#define CANBT_SJW_SHIFT				 6				 
#define CANBT_TSEG1_MASK			 0x0F00			 // Time segment before the sample point Valid values are 1 to				  15.[1]
#define CANBT_TSEG1_SHIFT			 8				 
#define CANBT_TSEG2_MASK			 0x7000			 // Time segment after the sample point Valid values are 0 to				  7.[1]
#define CANBT_TSEG2_SHIFT			 12				

/* CANINT, address 0x4005 0010 */
#define CANINT_INTID_MASK			 0xFFFF			 // 0x0000 = No interrupt is pending.				0 0x0001 - 0x0020 = Number of message object which caused the interrupt. 0x0021 - 0x7FFF = Unused 0x8000 = Status interrupt 0x8001 - 0xFFFF = Unused
#define CANINT_INTID_SHIFT			 0				 

/* CANTEST, address 0x4005 0014 */
#define CANTEST_BASIC				 (1 << 2)		   // Basic mode
#define CANTEST_SILENT				 (1 << 3)		   // Silent mode
#define CANTEST_LBACK				 (1 << 4)		   // Loop back mode
#define CANTEST_TX_MASK				 0x0060			 // Control of CAN_TXD pins
#define CANTEST_TX_SHIFT			 5				 
#define CANTEST_RX					 (1 << 7)		   // Monitors the actual value of the CAN_RXD pin.

/* CANBRPE, address 0x4005 0018 */
#define CANBRPE_BRPE_MASK			  0x000F			 // Baud rate prescaler extension By programming BRPE the Baud Rate Prescaler can be extended to values up to 1023. Hardware interprets the value as the value of BRPE (MSBs) and BRP (LSBs) plus one. Allowed values are 0 to 15.
#define CANBRPE_BRPE_SHIFT			  0				 

/* CANIF1_CMDREQ, address 0x4005 0020 and CANIF2_CMDREQ, address 0x4005 0080 */
#define CANIFn_CMDREQ_MN_MASK		  0x003F			 // Message number 0x01 - 0x20 = Valid message numbers. The message object in the message RAM is selected for data transfer. 0x00 = Not a valid message number. This value is interpreted as 0x20.[1] 0x21 - 0x3F = Not a valid message number. This value is interpreted as 0x01 - 0x1F.[1]
#define CANIFn_CMDREQ_MN_SHIFT		  0				 
#define CANIFn_CMDREQ_BUSY			  (1 << 15)		  // BUSY flag

/* CANIF1_CMDMSK, address 0x4005 0024 and CANIF2_CMDMSK, address 0x4005 0084 */
#define CANIFn_CMDMSK_DATA_B		   (1 << 0)		   // Access data bytes 4-7
#define CANIFn_CMDMSK_DATA_A		   (1 << 1)		   // Access data bytes 0-3
#define CANIFn_CMDMSK_TXRQST		   (1 << 2)		   // Access transmission request bit (Write direction)
#define CANIFn_CMDMSK_NEWDAT		   (1 << 2)		   // Access new data bit (Read direction)
#define CANIFn_CMDMSK_CLRINTPND		   (1 << 3)		   // This bit is ignored in the write direction.
#define CANIFn_CMDMSK_CTRL			   (1 << 4)		   // Access control bits
#define CANIFn_CMDMSK_ARB			   (1 << 5)		   // Access arbitration bits
#define CANIFn_CMDMSK_MASK			   (1 << 6)		   // Access mask bits
#define CANIFn_CMDMSK_WR			   (1 << 7)		   // Write transfer Transfer data from the selected message buffer registers to the message object addressed by the command request register CANIFn_CMDREQ.
#define CANIFn_CMDMSK_RD			   (0 << 7)		   // Read transfer Read data from the selected message buffer registers to the message object addressed by the command request register CANIFn_CMDREQ.

/* CANIF1_MSK1, address 0x4005 0028 and CANIF2_MASK1, address 0x4005 0088 */
#define CANIFn_MSK1_MSK_MASK		   0xFFFF			 // Identifier mask
#define CANIFn_MSK1_MSK_SHIFT		   0				 

/* CANIF1_MSK2, address 0x4005 002C and CANIF2_MASK2, address 0x4005 008C */
#define CANIFn_MSK2_MSK_MASK		   0x1FFF			 // Identifier mask
#define CANIFn_MSK2_MSK_SHIFT		   0				 
#define CANIFn_MSK2_MDIR			   (1 << 14)		  // Mask message direction
#define CANIFn_MSK2_MXTD			   (1 << 15)		  // Mask extend identifier

/* CANIF1_ARB1, address 0x4005 0030 and CANIF2_ARB1, address 0x4005 0090 */
#define CANIFn_ARB1_ID_MASK			   0xFFFF			 // Message identifier 29-bit identifier (extended frame) 11-bit identifier (standard frame)
#define CANIFn_ARB1_ID_SHIFT		   0				 

/* CANIF1_ARB2, address 0x4005 0034 and CANIF2_ARB2, address 0x4005 0094 */
#define CANIFn_ARB2_ID_MASK			   0x1FFF			 // Message identifier 29-bit identifier (extended frame) 11-bit identifier (standard frame)
#define CANIFn_ARB2_ID_SHIFT		   0				 
#define CANIFn_ARB2_DIR				   (1 << 13)		  // Message direction
#define CANIFn_ARB2_XTD				   (1 << 14)		  // Extend identifier
#define CANIFn_ARB2_MSGVAL			   (1 << 15)		  // Message valid Remark: The CPU must reset the MSGVAL bit of all unused Messages Objects during the initialization before it resets bit INIT in the CAN Control Register. This bit must also be reset before the identifier ID28:0, the control bits XTD, DIR, or the Data Length Code DLC3:0 are modified, or if the Messages Object is no longer required.

/* CANIF1_MCTRL, address 0x4005 0038 and CANIF2_MCTRL, address 0x4005 0098 */
#define CANIFn_MCTRL_DLC_MASK		  0x000F			 // Data length code Remark: The Data Length Code of a Message Object must be defined the same as in all the corresponding objects with the same identifier at other nodes. When the Message Handler stores a data frame, it will write the DLC to the value given by the received message. 0000 - 1000 = Data frame has 0 - 8 data bytes. 1001 - 1111 = Data frame has 8 data bytes.
#define CANIFn_MCTRL_DLC_SHIFT		  0				 
#define CANIFn_MCTRL_EOB			 (1 << 7)		   // End of buffer
#define CANIFn_MCTRL_TXRQST			 (1 << 8)		   // Transmit request
#define CANIFn_MCTRL_RMTEN			 (1 << 9)		   // Remote enable
#define CANIFn_MCTRL_RXIE			 (1 << 10)		   // Receive interrupt enable
#define CANIFn_MCTRL_TXIE			 (1 << 11)		   // Transmit interrupt enable
#define CANIFn_MCTRL_UMASK			 (1 << 12)		   // Use acceptance mask Remark: If UMASK is set to 1, the message object's mask bits have to be programmed during initialization of the message object before MAGVAL is set to 1.
#define CANIFn_MCTRL_INTPND			 (1 << 13)		   // Interrupt pending
#define CANIFn_MCTRL_MSGLST			 (1 << 14)		   // Message lost (only valid for message objects in the direction receive).
#define CANIFn_MCTRL_NEWDAT			 (1 << 15)		   // New data

/* CANIF1_DA1, address 0x4005 003C and CANIF2_DA1, address 0x4005 009C */
#define CANIFn_DA1_DATA0_MASK		  0x00FF			 // Data byte 0
#define CANIFn_DA1_DATA0_SHIFT		  0				 
#define CANIFn_DA1_DATA1_MASK		  0xFF00			 // Data byte 1
#define CANIFn_DA1_DATA1_SHIFT		  8				 

/* CANIF1_DA2, address 0x4005 0040 and CANIF2_DA2, address 0x4005 00A0 */
#define CANIFn_DA2_DATA2_MASK		  0x00FF			 // Data byte 2
#define CANIFn_DA2_DATA2_SHIFT		  0				 
#define CANIFn_DA2_DATA3_MASK		  0xFF00			 // Data byte 3
#define CANIFn_DA2_DATA3_SHIFT		  8				 

/* CANIF1_DB1, address 0x4005 0044 and CANIF2_DB1, address 0x4005 00A4 */
#define CANIFn_DB1_DATA4_MASK		  0x00FF			 // Data byte 4
#define CANIFn_DB1_DATA4_SHIFT		  0				 
#define CANIFn_DB1_DATA5_MASK		  0xFF00			 // Data byte 5
#define CANIFn_DB1_DATA5_SHIFT		  8				 

/* CANIF1_DB2, address 0x4005 0048 and CANIF2_DB2, address 0x4005 00A8 */
#define CANIFn_DB2_DATA6_MASK		  0x00FF			 // Data byte 6
#define CANIFn_DB2_DATA6_SHIFT		  0				 
#define CANIFn_DB2_DATA7_MASK		  0xFF00			 // Data byte 7
#define CANIFn_DB2_DATA7_SHIFT		  8				 

/* CANTXREQ1, address 0x4005 0100 */
#define CANTXREQ1_TXRQST_MASK		  0xFFFF			 // Transmission request bit of message objects 16 to 1. 0 = This message object is not waiting for transmission. 1 = The transmission of this message object is requested and not yet done. Reserved
#define CANTXREQ1_TXRQST_SHIFT		  0				 

/* CANTXREQ2, address 0x4005 0104 */
#define CANTXREQ2_TXRQST_MASK		  0xFFFF			 // Transmission request bit of message objects 32 to 17. 0 = This message object is not waiting for transmission. 1 = The transmission of this message object is requested and not yet done. Reserved
#define CANTXREQ2_TXRQST_SHIFT		  0				 

/* CANND1, address 0x4005 0120 */
#define CANND1_NEWDAT_MASK			 0xFFFF			 // New data bits of message objects 16 to 1. 0 = No new data has been written into the data portion of this Message Object by the Message Handler since last time this flag was cleared by the CPU. 1 = The Message Handler or the CPU has written new data into the data portion of this Message Object.
#define CANND1_NEWDAT_SHIFT			 0				 

/* CANND2, address 0x4005 0124 */
#define CANND2_NEWDAT_MASK			 0xFFFF			 // New data bits of message objects 32 to 17. 0 = No new data has been written into the data portion of this Message Object by the Message Handler since last time this flag was cleared by the CPU. 1 = The Message Handler or the CPU has written new data into the data portion of this Message Object.
#define CANND2_NEWDAT_SHIFT			 0				 

/* CANIR1, address 0x4005 0140 */
#define CANIR1_INTPND_INTERRUPT_MASK   0xFFFF			 // pending bits of message objects 16 to 1. essage object is ignored by the message essage object is the source of an interrupt. Reserved
#define CANIR1_INTPND_INTERRUPT_SHIFT  0				 

/* CANIR2, addresses 0x4005 0144 */
#define CANIR2_INTPND_MASK			 0xFFFF			 // Interrupt pending bits of message objects 32 to 17. 0 = This message object is ignored by the message handler. 1 = This message object is the source of an interrupt. Reserved
#define CANIR2_INTPND_SHIFT			 0				 

/* CANMSGV1, addresses 0x4005 0160 */
#define CANMSGV1_MSGVAL_MASK		   0xFFFF			 // Message valid bits of message objects 16 to 1. 0 = This message object is ignored by the message handler. 1 = This message object is configured and should be considered by the message handler. Reserved
#define CANMSGV1_MSGVAL_SHIFT		   0				 

/* CANMSGV2, address 0x4005 0164 */
#define CANMSGV2_MSGVAL_MASK		   0xFFFF			 // Message valid bits of message objects 32 to 17. 0 = This message object is ignored by the message handler. 1 = This message object is configured and should be considered by the message handler. Reserved
#define CANMSGV2_MSGVAL_SHIFT		   0				 

/* CANCLKDIV, address 0x4005 0180 */
#define CANCLKDIV_CLKDIVVAL_MASK	   0x000F			 // Clock divider value. CAN_CLK = PCLK/(CLKDIVVAL +1) 0000: CAN_CLK = PCLK divided by 1. 0001: CAN_CLK = PCLK divided by 2. 0010: CAN_CLK = PCLK divided by 3 0011: CAN_CLK = PCLK divided by 4. ... 1111: CAN_CLK = PCLK divided by 16.
#define CANCLKDIV_CLKDIVVAL_SHIFT	   0	

/* Handy defines */
#define MSG_OBJ_MAX	  32
#define DLC_MAX		  8

#define ID_STD_MASK     0x07FF
#define ID_EXT_MASK     0x1FFFFFFF
#define DLC_MASK        0x0F


#define SCU_PINIO_CAN_TD   SCU_MODE_INACT
#define SCU_PINIO_CAN_RD   (SCU_MODE_INACT|SCU_MODE_INBUFF_EN)

static const PinMap PinMap_CAN_RD[] = {
    {P3_1,  CAN_0, (SCU_PINIO_CAN_RD | 2)},
    {PE_2 , CAN_0, (SCU_PINIO_CAN_RD | 1)},
    {P4_9,  CAN_1, (SCU_PINIO_CAN_RD | 6)},
    {P1_18, CAN_1, (SCU_PINIO_CAN_RD | 5)},
    {PE_1 , CAN_1, (SCU_PINIO_CAN_RD | 5)},
    {NC,	NC,	 0}
};

static const PinMap PinMap_CAN_TD[] = {
    {P3_2,  CAN_0, (SCU_PINIO_CAN_TD | 2)},
    {PE_3,  CAN_0, (SCU_PINIO_CAN_TD | 1)},
    {P4_8,  CAN_1, (SCU_PINIO_CAN_TD | 6)},
    {P1_17, CAN_1, (SCU_PINIO_CAN_TD | 5)},
    {PE_0,  CAN_1, (SCU_PINIO_CAN_TD | 5)},
    {NC,	NC,	 0}
};

#define CAN_NUM	2

static uint32_t can_irq_ids[CAN_NUM] = { 0 };
static uint32_t can_irq_flags[CAN_NUM] = { 0 };
static can_irq_handler irq_handler;


static uint32_t can_disable(can_t *obj){
    uint32_t sm = obj->dev->CNTL;
    obj->dev->CNTL |= CANCNTL_INIT;
    return sm;
}

static inline void can_enable(can_t *obj){
    if (obj->dev->CNTL & CANCNTL_INIT){
        obj->dev->CNTL &= ~CANCNTL_INIT;
    }
}

static inline void can_ClsTxIntStat(LPC_CCAN_T *can, int msgnum){
    can->IF[0].IF_CMDMSK_W = CANIFn_CMDMSK_RD | CANIFn_CMDMSK_CLRINTPND | CANIFn_CMDMSK_NEWDAT;
    can->IF[0].IF_CMDREQ = BFN_PREP(msgnum, CANIFn_CMDREQ_MN);
    while (can->IF[0].IF_CMDREQ & CANIFn_CMDREQ_BUSY);
}
static inline void can_irq(CANName name, int id){

    volatile int stat;
    int canint;

    LPC_CCAN_T *can = (LPC_CCAN_T *)(name);

    stat = can->STAT;

    if (can->IR2 & (1 << 13)){
        can_ClsTxIntStat(can, 30);
        irq_handler(can_irq_ids[id], IRQ_TX);
    }
    if (can->IR2 & (1 << 14)){
        can_ClsTxIntStat(can, 31);
        irq_handler(can_irq_ids[id], IRQ_TX);
    }
    if (can->IR2 & (1 << 15)){
        can_ClsTxIntStat(can, 32);
        irq_handler(can_irq_ids[id], IRQ_TX);
    }

    if (stat & 0x80){
        can->CNTL &= ~(1);
    }
    canint = can->INT;
    if (((can->ND2 << 16) + can->ND1) & (1 << (canint - 1))){
        irq_handler(can_irq_ids[id], IRQ_RX);
    }
}
static inline void can1_irq(){
    can_irq(CAN_0, 0);
}
static inline void can2_irq(){
    can_irq(CAN_1, 1);
}

void can_irq_init(can_t *obj, can_irq_handler handler, uint32_t id){

    irq_handler = handler;
    switch ((int)obj->dev){
    case CAN_0:
        can_irq_ids[0] = id;
        break;
    default:
        can_irq_ids[1] = id;
        break;
    }
}

void can_irq_free(can_t *obj){
    obj->dev->CNTL &= ~CANCNTL_IE;
    switch ((int)obj->dev){
    case CAN_0:
        NVIC_DisableIRQ(C_CAN0_IRQn);
        can_irq_ids[0] = 0;
        can_irq_flags[0] = 0;
        break;
    default:
        NVIC_DisableIRQ(C_CAN1_IRQn);
        can_irq_ids[1] = 0;
        can_irq_flags[1] = 0;
        break;
    }
}

void can_irq_set(can_t *obj, CanIrqType type, uint32_t enable){
    int id = 0;
    if ((int)obj->dev == CAN_0){
        id = 0;
    } else{
        id = 1;
    }

    // Put CAN in Reset Mode and enable interrupt
    can_disable(obj);

    if (enable == 0){
        can_irq_flags[id] &= ~(1 << type);
        if (can_irq_flags[id] == 0){
            obj->dev->CNTL &= ~(CANCNTL_IE | CANCNTL_SIE);
        }
    } else{
        can_irq_flags[id] |= (1 << type);
        obj->dev->CNTL |= CANCNTL_IE | CANCNTL_SIE;
    }
    // Take it out of reset...
    can_enable(obj);

    switch ((int)obj->dev){
    case CAN_0:
        if (enable){
            NVIC_SetVector(C_CAN0_IRQn, (uint32_t)&can1_irq);
            NVIC_EnableIRQ(C_CAN0_IRQn);
        } else{
            if (can_irq_flags[id] == 0){
                NVIC_DisableIRQ(C_CAN0_IRQn);
            }
        }
        break;
    default:
        if (enable){
            NVIC_SetVector(C_CAN1_IRQn, (uint32_t)&can2_irq);
            NVIC_EnableIRQ(C_CAN1_IRQn);
        } else{
            if (can_irq_flags[id] == 0){
                NVIC_DisableIRQ(C_CAN1_IRQn);
            }
        }
        break;
    }
}

static const int timing_pts[23][2] = {
    {0x0, 0x0},	  // 2,  50%
    {0x1, 0x0},	  // 3,  67%
    {0x2, 0x0},	  // 4,  75%
    {0x3, 0x0},	  // 5,  80%
    {0x3, 0x1},	  // 6,  67%
    {0x4, 0x1},	  // 7,  71%
    {0x5, 0x1},	  // 8,  75%
    {0x6, 0x1},	  // 9,  78%
    {0x6, 0x2},	  // 10, 70%
    {0x7, 0x2},	  // 11, 73%
    {0x8, 0x2},	  // 12, 75%
    {0x9, 0x2},	  // 13, 77%
    {0x9, 0x3},	  // 14, 71%
    {0xA, 0x3},	  // 15, 73%
    {0xB, 0x3},	  // 16, 75%
    {0xC, 0x3},	  // 17, 76%
    {0xD, 0x3},	  // 18, 78%
    {0xD, 0x4},	  // 19, 74%
    {0xE, 0x4},	  // 20, 75%
    {0xF, 0x4},	  // 21, 76%
    {0xF, 0x5},	  // 22, 73%
    {0xF, 0x6},	  // 23, 70%
    {0xF, 0x7},	  // 24, 67%
};

static unsigned int can_speed(unsigned int sclk, unsigned int cclk, unsigned char psjw){
    uint32_t	btr;
    uint32_t	clkdiv = 2;
    uint16_t	brp = 0;
    uint32_t	calcbit;
    uint32_t	bitwidth;
    int		 hit = 0;
    int		 bits = 0;

    sclk /= clkdiv;

    bitwidth = sclk / cclk;

    brp = bitwidth / 0x18;
    while ((!hit) && (brp < bitwidth / 4)){
        brp++;
        for (bits = 22; bits > 0; bits--){
            calcbit = (bits + 3) * (brp + 1);
            if (calcbit == bitwidth){
                hit = 1;
                break;
            }
        }
    }
    clkdiv = clkdiv - 1;

    if (hit){
        btr = BFN_PREP(timing_pts[bits][1], CANBT_TSEG2)
            | BFN_PREP(timing_pts[bits][0], CANBT_TSEG1)
            | BFN_PREP(psjw, CANBT_SJW)
            | BFN_PREP(brp, CANBT_BRP);
        btr = btr | (clkdiv << 16);

    } else{
        btr = 0;
    }

    return btr;
}

int can_frequency(can_t *obj, int f){
    int btr = can_speed(SystemCoreClock, (unsigned int)f, 1);
    int clkdiv = (btr >> 16) & 0x0F;
    btr = btr & 0xFFFF;

    if (btr > 0){
        uint32_t cntl_init = obj->dev->CNTL | CANCNTL_INIT;
        // Set the bit clock
        obj->dev->CNTL |= CANCNTL_CCE | CANCNTL_INIT;
        obj->dev->CLKDIV = clkdiv;
        obj->dev->BT = btr;
        obj->dev->BRPE = 0x0000;
        obj->dev->CNTL &= ~(CANCNTL_CCE | CANCNTL_INIT);
        obj->dev->CNTL |= cntl_init;
        return 1;
    }
    return 0;
}

int can_config_rxmsgobj(can_t *obj){
    uint16_t i = 0;

    // Make sure the interface is available
    while (obj->dev->IF[0].IF_CMDREQ & CANIFn_CMDREQ_BUSY);

    // Mark message valid, Direction = RX, Don't care about anything else
    obj->dev->IF[0].IF_ARB1 = 0;
    obj->dev->IF[0].IF_ARB2 = 0;
    obj->dev->IF[0].IF_MCTRL = 0;

    for (i = 1; i <= MSG_OBJ_MAX; i++){
        // Transfer arb and control fields to message object
        obj->dev->IF[0].IF_CMDMSK_W = CANIFn_CMDMSK_WR | CANIFn_CMDMSK_ARB | CANIFn_CMDMSK_CTRL;// | CANIFn_CMDMSK_TXRQST;

        // Start Transfer to given message number
        obj->dev->IF[0].IF_CMDREQ = BFN_PREP(i, CANIFn_CMDREQ_MN);

        // Wait until transfer to message ram complete - TODO: maybe not block??
        while (obj->dev->IF[0].IF_CMDREQ & CANIFn_CMDREQ_BUSY);
    }

    return 1;
}

void can_init(can_t *obj, PinName rd, PinName td){

    CANName can_rd = (CANName)pinmap_peripheral(rd, PinMap_CAN_RD);
    CANName can_td = (CANName)pinmap_peripheral(td, PinMap_CAN_TD);
    obj->dev = (LPC_CCAN_T *)pinmap_merge(can_rd, can_td);
    MBED_ASSERT((int)obj->dev != NC);

    pinmap_pinout(rd, PinMap_CAN_RD);
    pinmap_pinout(td, PinMap_CAN_TD);

    switch ((int)obj->dev){
    case CAN_0:
        LPC_RGU->RESET_CTRL1 = 1 << (RGU_CAN0_RST - 32);
        LPC_RGU->RESET_CTRL1 = 0;
        LPC_CCU1->CLKCCU[CLK_APB3_CAN0].CFG |= 1;
        break;
    default:
        LPC_RGU->RESET_CTRL1 = 1 << (RGU_CAN1_RST - 32);
        LPC_RGU->RESET_CTRL1 = 0;
        LPC_CCU1->CLKCCU[CLK_APB1_CAN1].CFG |= 1;
        break;
    }

    can_frequency(obj, 250000);

    obj->dev->CNTL &= ~CANCNTL_INIT;
    while (obj->dev->CNTL & CANCNTL_INIT);

    // Initialize RX message object
    can_config_rxmsgobj(obj);
}

void can_reset(can_t *obj){

    switch ((int)obj->dev){
    case CAN_0:
        LPC_RGU->RESET_CTRL1 = 1 << (RGU_CAN0_RST - 32);
        LPC_RGU->RESET_CTRL1 = 0;
        break;
    default:
        LPC_RGU->RESET_CTRL1 = 1 << (RGU_CAN1_RST - 32);
        LPC_RGU->RESET_CTRL1 = 0;
        break;
    }

    obj->dev->STAT = 0;

    can_config_rxmsgobj(obj);
}


void can_free(can_t *obj){

    can_disable(obj);

    switch ((int)obj->dev){
    case CAN_0:
        LPC_RGU->RESET_CTRL1 = 1 << (RGU_CAN0_RST - 32);
        LPC_RGU->RESET_CTRL1 = 0;
        LPC_CCU1->CLKCCU[CLK_APB3_CAN0].CFG &= ~1;
        NVIC_DisableIRQ(C_CAN0_IRQn);
        break;
    default:
        LPC_RGU->RESET_CTRL1 = 1 << (RGU_CAN1_RST - 32);
        LPC_RGU->RESET_CTRL1 = 0;
        LPC_CCU1->CLKCCU[CLK_APB1_CAN1].CFG &= ~1;
        NVIC_DisableIRQ(C_CAN1_IRQn);
        break;
    }

}

int can_mode(can_t *obj, CanMode mode){
    int success = 0;
    switch (mode){
    case MODE_RESET:
        obj->dev->CNTL &= ~CANCNTL_TEST;
        can_disable(obj);
        success = 1;
        break;
    case MODE_NORMAL:
        obj->dev->CNTL &= ~CANCNTL_TEST;
        can_enable(obj);
        success = 1;
        break;
    case MODE_SILENT:
        obj->dev->CNTL |= CANCNTL_TEST;
        obj->dev->TEST |= CANTEST_SILENT;
        obj->dev->TEST &= ~CANTEST_LBACK;
        success = 1;
        break;
    case MODE_TEST_LOCAL:
    case MODE_TEST_GLOBAL:
        obj->dev->CNTL |= CANCNTL_TEST;
        obj->dev->TEST &= ~CANTEST_SILENT;
        obj->dev->TEST |= CANTEST_LBACK;
        success = 1;
        break;
    case MODE_TEST_SILENT:
        obj->dev->CNTL |= CANCNTL_TEST;
        obj->dev->TEST |= (CANTEST_LBACK | CANTEST_SILENT);
        success = 1;
        break;
    default:
        success = 0;
        break;
    }

    return success;
}

int can_filter(can_t *obj, uint32_t id, uint32_t mask, CANFormat format, int32_t handle){
    uint16_t i;
    int rxie = 0;

    if ((int)obj->dev == CAN_0){
        i = 0;
    } else{
        i = 1;
    }
    if (can_irq_flags[i] & (1 << IRQ_RX) != 0){
        rxie = CANIFn_MCTRL_RXIE;
    }
    // Find first free message object
    if (handle == 0){
        uint32_t msgval = obj->dev->MSGV1 | (obj->dev->MSGV2 << 16);
        // Find first free messagebox
        for (i = 0; i < 29; i++){
            if ((msgval & (1 << i)) == 0){
                handle = i + 1;
                break;
            }
        }
    }

    if (handle > 0 && handle < 30){
        if (format >= CANExtended || id & (~0x7ff) || mask & (~0x7ff)){
            // Mark message valid, Direction = TX, Extended Frame, Set Identifier and mask everything
            obj->dev->IF[0].IF_ARB1 = BFN_PREP(id, CANIFn_ARB1_ID);
            obj->dev->IF[0].IF_ARB2 = CANIFn_ARB2_MSGVAL | CANIFn_ARB2_XTD | BFN_PREP(id >> 16, CANIFn_ARB2_ID);
            obj->dev->IF[0].IF_MSK1 = BFN_PREP(mask, CANIFn_MSK1_MSK);
            obj->dev->IF[0].IF_MSK2 = BFN_PREP(mask >> 16, CANIFn_MSK2_MSK);
            if (format == CANExtended){
                obj->dev->IF[0].IF_MSK2 |= CANIFn_MSK2_MXTD;
            }
        } else{
            // Mark message valid, Direction = TX, Set Identifier and mask everything
            obj->dev->IF[0].IF_ARB2 = CANIFn_ARB2_MSGVAL | BFN_PREP(id << 2, CANIFn_ARB2_ID);
            obj->dev->IF[0].IF_MSK2 = /* CANIFn_MSK2_MDIR | */ BFN_PREP(mask << 2, CANIFn_MSK2_MSK);
        }

        // Use mask, single message object and set DLC
        obj->dev->IF[0].IF_MCTRL = CANIFn_MCTRL_UMASK | CANIFn_MCTRL_EOB | rxie | BFN_PREP(DLC_MAX, CANIFn_MCTRL_DLC);

        // Transfer all fields to message object
        obj->dev->IF[0].IF_CMDMSK_W = CANIFn_CMDMSK_WR | CANIFn_CMDMSK_MASK | CANIFn_CMDMSK_ARB | CANIFn_CMDMSK_CTRL;

        // Start Transfer to given message number
        obj->dev->IF[0].IF_CMDREQ = BFN_PREP(handle, CANIFn_CMDREQ_MN);

        // Wait until transfer to message ram complete - TODO: maybe not block??
        while (obj->dev->IF[0].IF_CMDREQ & CANIFn_CMDREQ_BUSY);
    }

    return handle;
}

int can_write(can_t *obj, CAN_Message msg, int cc){
    uint16_t msgnum = 30;
    int txie = 0;
    int i = 0;

    if ((int)obj->dev == CAN_0){
        i = 0;
    } else{
        i = 1;
    }
    if (can_irq_flags[i] & (1 << IRQ_RX) != 0){
        txie = CANIFn_MCTRL_TXIE;
    }

    if ((obj->dev->TXREQ2 & (1 << 13)) == 0){
        msgnum = 30;
    } else if ((obj->dev->TXREQ2 & (1 << 14)) == 0){
        msgnum = 31;
    } else if ((obj->dev->TXREQ2 & (1 << 15)) == 0){
        msgnum = 32;
    } else{
        return 0;
    }

    // Make sure controller is enabled
    can_enable(obj);

    // Make sure the interface is available
    while (obj->dev->IF[0].IF_CMDREQ & CANIFn_CMDREQ_BUSY);

    // Set the direction bit based on the message type
    uint32_t direction = 0;
    if (msg.type == CANData){
        direction = CANIFn_ARB2_DIR;
    }

    //if(msg.format == CANExtended)	{
    if (msg.id & (~0x7FF)){
        // Mark message valid, Extended Frame, Set Identifier and mask everything
        obj->dev->IF[0].IF_ARB1 = BFN_PREP(msg.id, CANIFn_ARB1_ID);
        obj->dev->IF[0].IF_ARB2 = CANIFn_ARB2_MSGVAL | CANIFn_ARB2_XTD | direction | BFN_PREP(msg.id >> 16, CANIFn_ARB2_ID);
        obj->dev->IF[0].IF_MSK1 = BFN_PREP(ID_EXT_MASK, CANIFn_MSK1_MSK);
        obj->dev->IF[0].IF_MSK2 = CANIFn_MSK2_MXTD | CANIFn_MSK2_MDIR | BFN_PREP(ID_EXT_MASK >> 16, CANIFn_MSK2_MSK);
    } else{
        // Mark message valid, Set Identifier and mask everything
        obj->dev->IF[0].IF_ARB2 = CANIFn_ARB2_MSGVAL | direction | BFN_PREP(msg.id << 2, CANIFn_ARB2_ID);
        obj->dev->IF[0].IF_MSK2 = CANIFn_MSK2_MDIR | BFN_PREP(ID_STD_MASK << 2, CANIFn_MSK2_MSK);
    }

    // Use mask, request transmission, single message object and set DLC
    obj->dev->IF[0].IF_MCTRL = CANIFn_MCTRL_UMASK | txie | CANIFn_MCTRL_TXRQST | CANIFn_MCTRL_EOB | BFN_PREP(msg.len, CANIFn_MCTRL_DLC);

    obj->dev->IF[0].IF_DA1 = BFN_PREP(msg.data[1], CANIFn_DA1_DATA1) | BFN_PREP(msg.data[0], CANIFn_DA1_DATA0);
    obj->dev->IF[0].IF_DA2 = BFN_PREP(msg.data[3], CANIFn_DA2_DATA3) | BFN_PREP(msg.data[2], CANIFn_DA2_DATA2);
    obj->dev->IF[0].IF_DB1 = BFN_PREP(msg.data[5], CANIFn_DB1_DATA5) | BFN_PREP(msg.data[4], CANIFn_DB1_DATA4);
    obj->dev->IF[0].IF_DB2 = BFN_PREP(msg.data[7], CANIFn_DB2_DATA7) | BFN_PREP(msg.data[6], CANIFn_DB2_DATA6);

    // Transfer all fields to message object
    obj->dev->IF[0].IF_CMDMSK_W = CANIFn_CMDMSK_WR | CANIFn_CMDMSK_MASK | CANIFn_CMDMSK_ARB | CANIFn_CMDMSK_CTRL | CANIFn_CMDMSK_TXRQST | CANIFn_CMDMSK_DATA_A | CANIFn_CMDMSK_DATA_B;

    // Start Transfer to given message number
    obj->dev->IF[0].IF_CMDREQ = BFN_PREP(msgnum, CANIFn_CMDREQ_MN);

    // Wait until transfer to message ram complete - TODO: maybe not block??
    while (obj->dev->IF[0].IF_CMDREQ & CANIFn_CMDREQ_BUSY);

    // Wait until TXOK is set, then clear it - TODO: maybe not block
    //while( !(obj->dev->STAT & CANSTAT_TXOK) );

    obj->dev->STAT &= ~(CANSTAT_TXOK);

    return 1;
}

int can_read(can_t *obj, CAN_Message *msg, int handle){
    //uint16_t i;

    LPC_CCAN_T *can = (LPC_CCAN_T *)(obj->dev);

    if (can->CNTL & CANCNTL_INIT){
        can->CNTL &= ~CANCNTL_INIT;
    }
    // Find first message object with new data
    if (handle == 0){
        handle = can->INT;
        /*uint32_t newdata = obj->dev->ND1 | (obj->dev->ND2 << 16);
        // Find first free messagebox
        for(i = 0; i < 32; i++) {
            if(newdata & (1 << i)) {
                handle = i+1;
                break;
            }
        }*/
    }
    if (handle > 0 && handle < 32){
        // Wait until message interface is free
        while (can->IF[1].IF_CMDREQ & CANIFn_CMDREQ_BUSY);

        // Transfer all fields to message object
        can->IF[1].IF_CMDMSK_W = CANIFn_CMDMSK_RD | CANIFn_CMDMSK_MASK | CANIFn_CMDMSK_ARB | CANIFn_CMDMSK_CTRL | CANIFn_CMDMSK_CLRINTPND | CANIFn_CMDMSK_TXRQST | CANIFn_CMDMSK_DATA_A | CANIFn_CMDMSK_DATA_B;

        // Start Transfer from given message number
        can->IF[1].IF_CMDREQ = BFN_PREP(handle, CANIFn_CMDREQ_MN);

        // Wait until transfer to message ram complete
        while (can->IF[1].IF_CMDREQ & CANIFn_CMDREQ_BUSY);

        if (can->IF[1].IF_ARB2 & CANIFn_ARB2_XTD){
            msg->format = CANExtended;
            msg->id = (can->IF[1].IF_ARB1 & CANIFn_ARB1_ID_MASK);
            msg->id |= (can->IF[1].IF_ARB2 & CANIFn_ARB2_ID_MASK) << 16;
        } else{
            msg->format = CANStandard;
            msg->id = (can->IF[1].IF_ARB2 & CANIFn_ARB2_ID_MASK) >> 2;
        }

        if (can->IF[1].IF_ARB2 & CANIFn_ARB2_DIR){
            msg->type = CANRemote;
        } else{
            msg->type = CANData;
        }

        msg->len = BFN_GET(can->IF[1].IF_MCTRL, CANIFn_MCTRL_DLC); // TODO: If > 8, len = 8
        msg->data[0] = BFN_GET(can->IF[1].IF_DA1, CANIFn_DA1_DATA0);
        msg->data[1] = BFN_GET(can->IF[1].IF_DA1, CANIFn_DA1_DATA1);
        msg->data[2] = BFN_GET(can->IF[1].IF_DA2, CANIFn_DA2_DATA2);
        msg->data[3] = BFN_GET(can->IF[1].IF_DA2, CANIFn_DA2_DATA3);
        msg->data[4] = BFN_GET(can->IF[1].IF_DB1, CANIFn_DB1_DATA4);
        msg->data[5] = BFN_GET(can->IF[1].IF_DB1, CANIFn_DB1_DATA5);
        msg->data[6] = BFN_GET(can->IF[1].IF_DB2, CANIFn_DB2_DATA6);
        msg->data[7] = BFN_GET(can->IF[1].IF_DB2, CANIFn_DB2_DATA7);

        can->STAT &= ~(CANSTAT_RXOK);
        return 1;
    }
    return 0;
}

unsigned char can_rderror(can_t *obj){
    return BFN_GET(obj->dev->EC, CANEC_REC);
}

unsigned char can_tderror(can_t *obj){
    return BFN_GET(obj->dev->EC, CANEC_TEC);
}

void can_monitor(can_t *obj, int silent){
    if (silent){
        obj->dev->CNTL |= CANCNTL_TEST;
        obj->dev->TEST |= CANTEST_SILENT;
    } else{
        obj->dev->CNTL &= ~(CANCNTL_TEST);
        obj->dev->TEST &= ~CANTEST_SILENT;
    }

    if (!(obj->dev->CNTL & CANCNTL_INIT)){
        obj->dev->CNTL |= CANCNTL_INIT;
    }
}

#endif





























