/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2019-2020 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/* Copyright (c) 2019-2020 Renesas Electronics Corporation.
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
/***********************************************************************************************************************
* File Name    : r_can_rz_if.h
* Description  : The RZ CAN API.
************************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 20.09.2019 1.00 First Release
***********************************************************************************************************************/
#ifndef CAN_INTERFACE_HEADER_FILE
#define CAN_INTERFACE_HEADER_FILE

/***********************************************************************************************************************
Includes   <System Includes>, "Project Includes"
***********************************************************************************************************************/
#include <stdint.h>

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define NOT_USED_PRV_(p)        ((void)(p))

#define BIT_SET      (1)
#define BIT_CLEAR    (0)

#define ENABLE       (0)
#define DISABLE      (1)

#define BIT00_MASK       (0x00000001)
#define BIT01_MASK       (0x00000002)
#define BIT02_MASK       (0x00000004)
#define BIT03_MASK       (0x00000008)
#define BIT04_MASK       (0x00000010)
#define BIT05_MASK       (0x00000020)
#define BIT06_MASK       (0x00000040)
#define BIT07_MASK       (0x00000080)
#define BIT08_MASK       (0x00000100)
#define BIT09_MASK       (0x00000200)
#define BIT10_MASK       (0x00000400)
#define BIT11_MASK       (0x00000800)
#define BIT12_MASK       (0x00001000)
#define BIT13_MASK       (0x00002000)
#define BIT14_MASK       (0x00004000)
#define BIT15_MASK       (0x00008000)
#define BIT16_MASK       (0x00010000)
#define BIT17_MASK       (0x00020000)
#define BIT18_MASK       (0x00040000)
#define BIT19_MASK       (0x00080000)
#define BIT20_MASK       (0x00100000)
#define BIT21_MASK       (0x00200000)
#define BIT22_MASK       (0x00400000)
#define BIT23_MASK       (0x00800000)
#define BIT24_MASK       (0x01000000)
#define BIT25_MASK       (0x02000000)
#define BIT26_MASK       (0x04000000)
#define BIT27_MASK       (0x08000000)
#define BIT28_MASK       (0x10000000)
#define BIT29_MASK       (0x20000000)
#define BIT30_MASK       (0x40000000)
#define BIT31_MASK       (0x80000000)

#define PROGRESS_OR_NOREQUEST    (0)
#define ABORT_COMPLETE           (1)
#define ONE_SHOT_TRANSMIT        (0x05)

#define CAN_FIFO_PTR_INC    (0xFF)
#define CAN_QUEUE_PTR_INC   (0xFF)

#define RX_HEAD_SIZE        (4 * 3)  /* ID_REG, PTR_REG, FDSTS_REG (4byte * 3register) */
#define RX_DATA_MAX_SIZE    (64)
#define RX_MSG_MAX_SIZE     (RX_HEAD_SIZE+RX_DATA_MAX_SIZE)

/*** Write data for register ************************/
#define THLSTS_THLELT_CLR          (0x00001F0B)
#define THLSTS_THLIF_CLR           (0x00001F07)
#define CFSTS_CFMLT_CLR            (0x0000FF1B)
#define CFSTS_CFRXIF_CLR           (0x0000FF17)
#define CFSTS_CFTXIF_CLR           (0x0000FF0F)
#define TXQSTS_TXQIF_CLR           (0x00000003)
#define RFSTS_RFMLT_CLR            (0x0000FF0B)
#define RFSTS_RFIF_CLR             (0x0000FF07)
#define GERFL_CMPOF_DEF_CLR        (0x00000006)
#define ERFL_ALF2BEF_CLR           (0x7FFF7F00)
#define FDSTS_SOCO2TDCVF_CLR       (0xFFFF007F)
#define TMSTS_TMTRF_CLR            (0x19)

/*** Mask bit for register W/R **********************/
/* used in R_CAN_Create func */
#define FDCFG_MASK_FOR_CREATE      (0x3F000407)
#define CFCC_MASK_FOR_CREATE       (0xFFFFFFFE)
#define RFCC_MASK_FOR_CREATE       (0xFFFFFFFE)
#define GCTR_MASK_FOR_CREATE       (0x00000F00)
#define CTR_MASK01_FOR_CREATE      (0x00EFFF00)
#define CTR_MASK02_FOR_CREATE      (0x80000000)
#define TXQCC_MASK_FOR_CREATE      (0x00003F00)
#define THLCC_MASK_FOR_CREATE      (0x00000700)

/* used in R_CAN_SetBitrate func */
#define CFDCFG_MASK_FOR_BITRATE    (0x00EF0300)


/* Test mode */
#define STD_TEST            (0)
#define TEST_LISTEN_ONLY    (1)
#define EXT_LOOPBACK        (2)
#define INT_LOOPBACK        (3)

#define VAL_CLEAR           (0)
#define CAN_PORT            (0x04)    /* CAN PORT Function */

#define RAM_TEST_PROTECT_RELEASE_CODE_1 (0x00007575)
#define RAM_TEST_PROTECT_RELEASE_CODE_2 (0x00008A8A)
#define RAM_INIT_DATA                   (0x00000000)


/*** CAN Hard resources *****************************/
#define MAX_CHANNELS    (2)    /* Number of mounted channels  */
#define CH_0            (0)    /* CAN CH0    */
#define CH_1            (1)    /* CAN CH1    */
#define UNUSED          (0)

#define TRFIFO_NUM_PER_CH    (3)   /* Number of Transmit/Receive FIFO Buffers per channel */
#define TRFIFO_MAX           (MAX_CHANNELS * TRFIFO_NUM_PER_CH)

#define RXFIFO_MAX           (8)

#define TXBUF_PER_CH         (16)   /* Number of Transmit Buffers per channel */
#define TXBUF_MAX            (MAX_CHANNELS * TXBUF_PER_CH)
#define CH_0_TXBUF_TOP       (0)
#define CH_0_TXBUF_END       (15)
#define CH_1_TXBUF_TOP       (16)
#define CH_1_TXBUF_END       (31)

#define RXBUF_MAX            (32)


/*** CAN mode setting values ******************************/
/* RSCFDnCFDGCTR: Global Control Register */
#define CFDGCTR_GMDC_TEST     (0x00000002)    /* bit1-0=2: Global test mode      */
#define CFDGCTR_GMDC_RESET    (0x00000001)    /* bit1-0=1: Global reset mode     */
#define CFDGCTR_GMDC_OP       (0x00000000)    /* bit1-0=0: Global operating mode */

/* RSCFDnCFDCmSTS：Channel m Status Register */
#define CFDCmCTR_CSLPSTS    (0x00000004)    /* bit2=1  : Channel stop mode          */
#define CFDCmCTR_CHLTSTS    (0x00000002)    /* bit1-0=2: Channel halt mode          */
#define CFDCmCTR_CHRST      (0x00000001)    /* bit1-0=1: Channel reset mode         */
#define CFDCmCTR_CHOP       (0x00000000)    /* bit1-0=0: Channel communication mode */

/*** CAN API arguments values *****************************/
#define GLOBAL_STOP         (0)
#define GLOBAL_RESET        (1)
#define GLOBAL_OPERATION    (2)
#define GLOBAL_TEST         (3)
#define CHANNEL_STOP        (4)
#define CHANNEL_RESET       (5)
#define CHANNEL_HALT        (6)
#define CHANNEL_COM         (7)

/*** CAN API ACTION TYPES ***/
#define ENABLE_COMMON                   (0)
#define DISABLE_COMMON                  (1)
#define ENABLE_CHANNEL                  (2)
#define DISABLE_CHANNEL                 (3)
#define CANPORT_TEST_LISTEN_ONLY        (4)
#define CANPORT_TEST_0_EXT_LOOPBACK     (5)
#define CANPORT_TEST_1_INT_LOOPBACK     (6)
#define CANPORT_RETURN_TO_NORMAL        (7)


/* Tx mode */
#define TX_TRFIFO    (0)
#define TX_TXBUF     (1)
#define TX_QUEUE     (2)
/* Rx mode */
#define RX_TRFIFO    (0)
#define RX_RXFIFO    (1)
#define RX_RXBUF     (2)

/*** CAN API return values *****************************/
#define R_CAN_OK                      (0)
#define R_CAN_NOT_OK                  (1)
#define R_CAN_ERR                     (2)
#define R_CAN_BAD_CH_NR               (3)
#define R_CAN_SW_BAD_IDX              (4)
#define R_CAN_BAD_ACTION_TYPE         (5)
#define R_CAN_SW_RAM_ERR              (6)
#define R_CAN_MODULE_STOP_ERR         (7)
#define R_CAN_BAD_MODE_TYPE           (8)
#define R_CAN_BAD_ONESHOT_TYPE        (9)
#define R_CAN_BUF_EMPTY               (10)
#define R_CAN_BUF_OVERFLOW            (11)
#define R_CAN_MSGLOST                 (12)
#define R_CAN_REG_ISR_ERR             (13)
#define R_CAN_NO_SENTDATA             (14)
#define R_CAN_GLB_STOP_ERR            (15)
#define R_CAN_GLB_RST_ERR             (16)
#define R_CAN_GLB_TEST_ERR            (17)
#define R_CAN_GLB_OP_ERR              (18)
#define R_CAN_CH_STOP_ERR             (19)
#define R_CAN_CH_RST_ERR              (20)
#define R_CAN_CH_HALT_ERR             (21)
#define R_CAN_CH_COM_ERR              (22)
#define R_CAN_STATUS_ERROR_ACTIVE     (23)
#define R_CAN_STATUS_ERROR_PASSIVE    (24)
#define R_CAN_STATUS_BUSOFF           (25)
#define R_CAN_CRC_ERR                 (26)
#define R_CAN_ICC_TMO                 (27)

/******************************************************************************
Typedef definitions
******************************************************************************/
typedef struct {
    uint32_t    ID : 29;     /* ID Data                            */
    uint32_t    THDSE : 1;   /* Transmit History Data Store Enable */
    uint32_t    RTR : 1;     /* RTR  0:Data 1:Remote               */
    uint32_t    IDE : 1;     /* IDE  0:Standard 1:Extend           */
    uint32_t    LBL : 12;    /* Label Data                         */
    uint32_t    DLC : 4;     /* DLC Data                           */
    uint32_t    FDSTS: 1;    /* FD Status 0:CLASSICAL_CAN, 1:CANFD */
    uint32_t    BRS: 1;      /* BRS bit                            */
    uint32_t    ESI: 1;      /* ESI bit                            */
} can_frame_t;

typedef struct {
    void (*p_Addr)(void);                        /* Interrupt callback function pointer */
    uint8_t                  prior;              /* Interrupt priority                  */
} isr_attr_t;

typedef struct {
    isr_attr_t rxfifo;                     /* Receive FIFO interrupt                                 */
    isr_attr_t glb_err;                    /* Global error interrupt                                 */
    isr_attr_t tx[MAX_CHANNELS];           /* CANm transmit/receive FIFO transmit complete interrupt */
    isr_attr_t trfifo_rx[MAX_CHANNELS];    /* CANm transmit/receive FIFO receive complete interrupt  */
    isr_attr_t ch_err[MAX_CHANNELS];       /* CANm error interrupt                                   */

} can_isr_t;

/**************************************************************************************************
Exported global variables
***************************************************************************************************/
/**************************************************************************************************
Exported global functions (to be accessed by other files)
***************************************************************************************************/
/*****************************************************************
                R Z   C A N   A P I
******************************************************************/
/* INITIALIZATION */
uint8_t R_CAN_Create(const can_isr_t *cb_func);
uint8_t R_CAN_PortSet(const uint8_t  ch_nr, const uint8_t  action_type);
uint8_t R_CAN_Control(const uint8_t  ch_nr, const uint8_t  action_type);
void    R_CAN_SetBitrate(const uint8_t  ch_nr);
uint8_t R_CAN_Gateway(const uint8_t idx_nr);

/* TRANSMIT */
uint8_t R_CAN_TxSet(const uint8_t tx_mode, const uint8_t idx_nr, const can_frame_t *p_frame, const uint32_t *p_buf);
uint8_t R_CAN_Tx(const uint8_t tx_mode, const uint8_t idx_nr, const uint8_t one_shot_en);
uint8_t R_CAN_TxCheck(const uint8_t tx_mode, const uint8_t idx_nr);
uint8_t R_CAN_TxStopMsg(const uint8_t tx_mode, const uint8_t idx_nr);
uint8_t R_CAN_TxHis(const uint8_t ch_nr, uint32_t *p_buf);

/* RECEIVE */
uint8_t R_CAN_RxSet(const uint8_t rx_mode, const uint8_t idx_nr);
uint8_t R_CAN_RxPoll(const uint8_t rx_mode, const uint8_t idx_nr);
uint8_t R_CAN_RxRead(const uint8_t rx_mode, const uint8_t idx_nr, uint32_t *p_buf);

/* ERRORS */
uint8_t R_CAN_CheckErr(const uint8_t ch_nr);

/* DIAGNOSIS   */
uint8_t R_CAN_RamTest(const uint32_t wr_dt);
uint8_t R_CAN_CrcTest(const uint8_t tx_mode, const uint8_t tx_idx_nr, const can_frame_t *p_frame, const uint32_t *p_tx_buf,
                      const uint8_t rx_mode, const uint8_t rx_idx_nr, uint32_t *p_rx_buf);
/* API WRAPPER */
void Wrap_Can_WaitRamInitOver(void);
void Wrap_Can_SetRxRule(void);


#endif    /* CAN_INTERFACE_HEADER_FILE */

