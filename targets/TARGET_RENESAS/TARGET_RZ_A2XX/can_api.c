/* mbed Microcontroller Library
 * Copyright (c) 2006-2020 ARM Limited
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
#if DEVICE_CAN
#include "mbed_assert.h"
#include "can_api.h"
#include "RZ_A2_Init.h"
#include "cmsis.h"
#include "PeripheralPins.h"
#include "iodefine.h"
#include "r_typedefs.h"

#include "mbed_drv_cfg.h"
#include "rza_io_regrw.h"
#include "iobitmask.h"
#include "r_can_rz_config.h"
#include "r_can_rz_if.h"

/* RZA IO */
#define REG_32W                 RZA_IO_RegWrite_32
#define REG_NONSHIFT            IOREG_NONSHIFT_ACCESS
#define REG_NONMASK             IOREG_NONMASK_ACCESS
/* Channel */
#define CHANNEL_MAX             MAX_CHANNELS
#define CAN_TX_RX               2
#define CAN_IRQ_TYPE_MAX        8
/* RX rule */
#define RXRULE_J_MAX            16
#define RXRULE_CH0_MAX          CAN0_RX_RULE_NUM
#define RXRULE_CH1_MAX          CAN1_RX_RULE_NUM
/* TR_FIFO */
#define TX_TRFIFO_INDEX(ch)     (ch * TRFIFO_NUM_PER_CH)
#define RX_TRFIFO_INDEX(ch)     (ch * TRFIFO_NUM_PER_CH) + 1
/* Normal bit rate */
#define NSS_MAX                 1
#define NTSEG1_MAX              128
#define NTSEG2_MAX              32
#define N_TOTAL_TQMAX           (NSS_MAX + NTSEG1_MAX + NTSEG2_MAX)
#define N_TOTAL_TQMIN           8
#define N_SAMPLE_POINT          0.666666667
#define N_SJW                   3
#define N_BITRATE_DEFAULT       100000
#define N_BITRATE_MAX           1000000
/* Data bit rate */
#define DSS_MAX                 1
#define DTSEG1_MAX              16
#define DTSEG2_MAX              8
#define D_TOTAL_TQMAX           (DSS_MAX + DTSEG1_MAX + DTSEG2_MAX)
#define D_TOTAL_TQMIN           8
#define D_SAMPLE_POINT          0.666666667
#define D_SJW                   3
#define D_CH0_BITRATE           2000000
#define D_CH1_BITRATE           2000000

/*
 * RCANFD Register
 */
/* CmCTR */
#define CTR_BEIE                RCANFD_RSCFD0CFDC0CTR_BEIE
#define CTR_EWIE                RCANFD_RSCFD0CFDC0CTR_EWIE
#define CTR_EPIE                RCANFD_RSCFD0CFDC0CTR_EPIE
#define CTR_BOEIE               RCANFD_RSCFD0CFDC0CTR_BOEIE
#define CTR_BORIE               RCANFD_RSCFD0CFDC0CTR_BORIE
#define CTR_OLIE                RCANFD_RSCFD0CFDC0CTR_OLIE
#define CTR_BLIE                RCANFD_RSCFD0CFDC0CTR_BLIE
#define CTR_ALIE                RCANFD_RSCFD0CFDC0CTR_ALIE
#define CTR_TAIE                RCANFD_RSCFD0CFDC0CTR_TAIE
#define CTR_IEALL               (CTR_BEIE | CTR_EWIE | CTR_EPIE | CTR_BOEIE | CTR_BORIE | \
                                 CTR_OLIE | CTR_BLIE | CTR_ALIE | CTR_TAIE)
#define CTR_EOCOIE              RCANFD_RSCFD0CFDC0CTR_EOCOIE
#define CTR_SOCOIE              RCANFD_RSCFD0CFDC0CTR_SOCOIE
#define CTR_TDCVFIE             RCANFD_RSCFD0CFDC0CTR_TDCVFIE
#define CTR_BOM                 RCANFD_RSCFD0CFDC0CTR_BOM
#define CTR_ERRD                RCANFD_RSCFD0CFDC0CTR_ERRD
#define CTR_CTME                RCANFD_RSCFD0CFDC0CTR_CTME
#define CTR_CTMS                RCANFD_RSCFD0CFDC0CTR_CTMS
#define CTR_CRCT                RCANFD_RSCFD0CFDC0CTR_CRCT
#define CTR_ROM                 RCANFD_RSCFD0CFDC0CTR_ROM
#define CTR_CTME_SHIFT          RCANFD_RSCFD0CFDC0CTR_CTME_SHIFT
#define CTR_CTMS_SHIFT          RCANFD_RSCFD0CFDC0CTR_CTMS_SHIFT
#define MASK_CTR_INIT           (CTR_ERRD | CTR_BOM | CTR_TDCVFIE | CTR_SOCOIE | CTR_EOCOIE | CTR_IEALL)
/* CmERFL */
#define ERFL_BEF                RCANFD_RSCFD0CFDC0ERFL_BEF
#define ERFL_EWF                RCANFD_RSCFD0CFDC0ERFL_EWF
#define ERFL_EPF                RCANFD_RSCFD0CFDC0ERFL_EPF
#define ERFL_BOEF               RCANFD_RSCFD0CFDC0ERFL_BOEF
#define ERFL_BORF               RCANFD_RSCFD0CFDC0ERFL_BORF
#define ERFL_OVLF               RCANFD_RSCFD0CFDC0ERFL_OVLF
#define ERFL_BLF                RCANFD_RSCFD0CFDC0ERFL_BLF
#define ERFL_ALF                RCANFD_RSCFD0CFDC0ERFL_ALF
#define ERFL_SERR               RCANFD_RSCFD0CFDC0ERFL_SERR
#define ERFL_FERR               RCANFD_RSCFD0CFDC0ERFL_FERR
#define ERFL_AERR               RCANFD_RSCFD0CFDC0ERFL_AERR
#define ERFL_CERR               RCANFD_RSCFD0CFDC0ERFL_CERR
#define ERFL_B1ERR              RCANFD_RSCFD0CFDC0ERFL_B1ERR
#define ERFL_B0ERR              RCANFD_RSCFD0CFDC0ERFL_B0ERR
#define ERFL_ADERR              RCANFD_RSCFD0CFDC0ERFL_ADERR
#define ERFL_ALLERR             (ERFL_SERR | ERFL_FERR | ERFL_AERR | ERFL_CERR | ERFL_B1ERR | ERFL_B0ERR | ERFL_ADERR)
/* CmFDCFG */
#define FDCFG_EOCCFG            RCANFD_RSCFD0CFDC0FDCFG_EOCCFG
#define FDCFG_ESIC              RCANFD_RSCFD0CFDC0FDCFG_ESIC
#define FDCFG_GWEN              RCANFD_RSCFD0CFDC0FDCFG_GWEN
#define FDCFG_GWFDF             RCANFD_RSCFD0CFDC0FDCFG_GWFDF
#define FDCFG_GWBRS             RCANFD_RSCFD0CFDC0FDCFG_GWBRS
#define FDCFG_TMME              RCANFD_RSCFD0CFDC0FDCFG_TMME
#define FDCFG_FDOE              RCANFD_RSCFD0CFDC0FDCFG_FDOE
#define FDCFG_REFE              RCANFD_RSCFD0CFDC0FDCFG_REFE
#define MASK_FDCFG_INIT         (FDCFG_REFE | FDCFG_FDOE | FDCFG_TMME | FDCFG_GWBRS | FDCFG_GWFDF| \
                                 FDCFG_GWEN | FDCFG_ESIC | FDCFG_EOCCFG)
/* GCTR */
#define GCTR_DEIE               RCANFD_RSCFD0CFDGCTR_DEIE
#define GCTR_MEIE               RCANFD_RSCFD0CFDGCTR_MEIE
#define GCTR_THLEIE             RCANFD_RSCFD0CFDGCTR_THLEIE
#define GCTR_CMPOFIE            RCANFD_RSCFD0CFDGCTR_CMPOFIE
#define MASK_GCTR_INIT          (GCTR_DEIE | GCTR_MEIE | GCTR_THLEIE | GCTR_CMPOFIE)
/* CFCCk */
#define CFCC_CFTXIE             RCANFD_RSCFD0CFDCFCC0_CFTXIE
#define CFCC_CFRXIE             RCANFD_RSCFD0CFDCFCC0_CFRXIE
#define CFCC_CFE                RCANFD_RSCFD0CFDCFCC0_CFE
#define MASK_CFCC_INIT          ~(CFCC_CFTXIE | CFCC_CFRXIE | CFCC_CFE)
/* CFSTSk */
#define CFSTS_CFRXIF            RCANFD_RSCFD0CFDCFSTS0_CFRXIF
#define CFSTS_CFTXIF            RCANFD_RSCFD0CFDCFSTS0_CFTXIF
/* RFCCx*/
#define RFCC_RFIE               RCANFD_RSCFD0CFDRFCC0_RFIE
#define RFCC_RFE                RCANFD_RSCFD0CFDRFCC0_RFE
#define MASK_RFCC_INIT          ~(RFCC_RFIE | RFCC_RFE)
/* TXQCCm */
#define TXQCC_TXQDC             RCANFD_RSCFD0CFDTXQCC0_TXQDC
#define TXQCC_TXQIE             RCANFD_RSCFD0CFDTXQCC0_TXQIE
#define TXQCC_TXQIM             RCANFD_RSCFD0CFDTXQCC0_TXQIM
/* THLCCm */
#define THLCC_THLIE             RCANFD_RSCFD0CFDTHLCC0_THLIE
#define THLCC_THLIM             RCANFD_RSCFD0CFDTHLCC0_THLIM
#define THLCC_THLDTE            RCANFD_RSCFD0CFDTHLCC0_THLDTE
/* GTSTCFG */
#define GTSTCFG_C0ICBCE         RCANFD_RSCFD0CFDGTSTCFG_C0ICBCE
#define GTSTCFG_C1ICBCE         RCANFD_RSCFD0CFDGTSTCFG_C1ICBCE
#define GTSTCFG_C0ICBCE_SHIFT   RCANFD_RSCFD0CFDGTSTCFG_C0ICBCE_SHIFT
#define GTSTCFG_C1ICBCE_SHIFT   RCANFD_RSCFD0CFDGTSTCFG_C1ICBCE_SHIFT
/* GTSTCTR */
#define GTSTCTR_ICBCTME         RCANFD_RSCFD0CFDGTSTCTR_ICBCTME
#define GTSTCTR_ICBCTME_SHIFT   RCANFD_RSCFD0CFDGTSTCTR_ICBCTME_SHIFT
/* RX buffer size */
#define RX_FIFO_MULTIPLE        8
#define RX_BUFFER_MAX           (4 * ((19 * RX_FIFO_MULTIPLE) + 1)) / 4

static void can_rec_irq(uint32_t ch);
static void can_trx_irq(uint32_t ch);
static void can_err_irq(uint32_t ch);
static void can0_rec_irq(void);
static void can0_trx_irq(void);
static void can0_err_irq(void);
static void can1_rec_irq(void);
static void can1_trx_irq(void);
static void can1_err_irq(void);
static void set_normal_bitrate(uint32_t ch, int f);
static void reset_register(uint32_t ch);
static void reset_buffer(uint32_t ch);
static void get_rxrule_index(uint32_t ch, int32_t handle, uint8_t *page, uint8_t *j);
static void reconfigure_channel(uint32_t ch);
static void set_test_mode(uint32_t ch, uint32_t mode, uint32_t enable);

typedef enum {
    CAN_TX = 0,
    CAN_RX
} can_operation;

typedef enum {
    TEST_DISABLE = 0,
    TEST_ENABLE
} ctr_ctme;

typedef struct {
    IRQn_Type   id;         /* Interrupt identifier */
    IRQHandler  handler;    /* Interrupt handler    */
} can_irq_int_t;

static const can_irq_int_t irq_tbl[CHANNEL_MAX][CAN_IRQ_TYPE_MAX] = {
    {   /* ch0 */
        { CFRXI0_IRQn, can0_rec_irq }, /* RxIrq : IRQ_RX       */
        { CTXI0_IRQn, can0_trx_irq },  /* TxIrq : IRQ_TX       */
        { CERI0_IRQn, can0_err_irq },  /* EwIrq : IRQ_ERROR    */
        { CERI0_IRQn, can0_err_irq },  /* DoIrq : IRQ_OVERRUN  */
        { CERI0_IRQn, can0_err_irq },  /* WuIrq : IRQ_WAKEUP   */
        { CERI0_IRQn, can0_err_irq },  /* EpIrq : IRQ_PASSIVE  */
        { CERI0_IRQn, can0_err_irq },  /* AlIrq : IRQ_ARB      */
        { CERI0_IRQn, can0_err_irq }   /* BeIrq : IRQ_BUS      */
    },
    {   /* ch1 */
        { CFRXI1_IRQn, can1_rec_irq }, /* RxIrq : IRQ_RX       */
        { CTXI1_IRQn, can1_trx_irq },  /* TxIrq : IRQ_TX       */
        { CERI1_IRQn, can1_err_irq },  /* EwIrq : IRQ_ERROR    */
        { CERI1_IRQn, can1_err_irq },  /* DoIrq : IRQ_OVERRUN  */
        { CERI1_IRQn, can1_err_irq },  /* WuIrq : IRQ_WAKEUP   */
        { CERI1_IRQn, can1_err_irq },  /* EpIrq : IRQ_PASSIVE  */
        { CERI1_IRQn, can1_err_irq },  /* EpIrq : IRQ_PASSIVE  */
        { CERI1_IRQn, can1_err_irq }   /* BeIrq : IRQ_BUS      */
    },
};

static const uint32_t irq_enable_flag_tbl[CAN_IRQ_TYPE_MAX] = {
    CFCC_CFRXIE,      /* RxIrq : IRQ_RX       */
    CFCC_CFTXIE,      /* TxIrq : IRQ_TX       */
    CTR_EWIE,         /* EwIrq : IRQ_ERROR    */
    CTR_OLIE,         /* DoIrq : IRQ_OVERRUN  */
    0,                /* WuIrq : IRQ_WAKEUP   */
    CTR_EPIE,         /* EpIrq : IRQ_PASSIVE  */
    CTR_ALIE,         /* AlIrq : IRQ_ARB      */
    CTR_BEIE,         /* BeIrq : IRQ_BUS      */
};

/* Channel bit rate configuration */
static __IO uint32_t *g_regtbl_ncfg[CHANNEL_MAX] = {
    &RCANFD.RSCFD0CFDC0NCFG.LONG,
    &RCANFD.RSCFD0CFDC1NCFG.LONG
};

/* Channel data bit rate configuration */
static __IO uint32_t *g_regtbl_dcfg[CHANNEL_MAX] = {
    &RCANFD.RSCFD0CFDC0DCFG.LONG,
    &RCANFD.RSCFD0CFDC1DCFG.LONG
};

/* Channel CANFD configuration */
static __IO uint32_t *g_regtbl_fdcfg[CHANNEL_MAX] = {
    &RCANFD.RSCFD0CFDC0FDCFG.LONG,
    &RCANFD.RSCFD0CFDC1FDCFG.LONG
};
static uint32_t g_fdcfg_init[CHANNEL_MAX] = {
    CAN_C0FDCFG,
    CAN_C1FDCFG
};

/* Channel control */
static __IO uint32_t *g_regtbl_ctr[CHANNEL_MAX] = {
    &RCANFD.RSCFD0CFDC0CTR.LONG,
    &RCANFD.RSCFD0CFDC1CTR.LONG
};
static uint32_t g_ctr_init[CHANNEL_MAX] = {
    CAN_C0CTR,
    CAN_C1CTR
};

/* Channel status */
static __IO uint32_t *g_regtbl_sts[CHANNEL_MAX] = {
    &RCANFD.RSCFD0CFDC0STS.LONG,
    &RCANFD.RSCFD0CFDC1STS.LONG
};

/* Channel error flag */
static __IO uint32_t *g_regtbl_erfl[CHANNEL_MAX] = {
    &RCANFD.RSCFD0CFDC0ERFL.LONG,
    &RCANFD.RSCFD0CFDC1ERFL.LONG
};

/* TRFIFO Configuration */
static __IO uint32_t *g_regtbl_cfcc[CHANNEL_MAX][CAN_TX_RX] = {
    { &RCANFD.RSCFD0CFDCFCC0.LONG, &RCANFD.RSCFD0CFDCFCC1.LONG },   // CAN0(TX TR_FIFO0, RX TR_FIFO1)
    { &RCANFD.RSCFD0CFDCFCC3.LONG, &RCANFD.RSCFD0CFDCFCC4.LONG }    // CAN1(TX TR_FIFO3, RX TR_FIFO4)
};
static uint32_t g_cfcc_init[CHANNEL_MAX][CAN_TX_RX] = {
    { CAN_CFCC0, CAN_CFCC1 },                                       // CAN0(TX TR_FIFO0, RX TR_FIFO1)
    { CAN_CFCC3, CAN_CFCC4 }                                        // CAN1(TX TR_FIFO3, RX TR_FIFO4)
};

/* TRFIFO Status */
static __IO uint32_t *g_regtbl_cfsts[CHANNEL_MAX][CAN_TX_RX] = {
    { &RCANFD.RSCFD0CFDCFSTS0.LONG, &RCANFD.RSCFD0CFDCFSTS1.LONG }, // CAN0(TX TR_FIFO0, RX TR_FIFO1)
    { &RCANFD.RSCFD0CFDCFSTS3.LONG, &RCANFD.RSCFD0CFDCFSTS4.LONG }  // CAN1(TX TR_FIFO3, RX TR_FIFO4)
};

/* RXFIFO Configuration */
static __IO uint32_t *g_regtbl_rfcc[CHANNEL_MAX] = {
    &RCANFD.RSCFD0CFDRFCC0.LONG,                                    // CAN0(RX_FIFO0)
    &RCANFD.RSCFD0CFDRFCC4.LONG                                     // CAN1(RX_FIFO4)
};
static uint32_t g_rfcc_init[CHANNEL_MAX] = {
    CAN_RFCC0,                                                      // CAN0(RX_FIFO0)
    CAN_RFCC4                                                       // CAN1(RX_FIFO4)
};

/* TXQCC */
static __IO uint32_t *g_regtbl_txqcc[CHANNEL_MAX] = {
    &RCANFD.RSCFD0CFDTXQCC0.LONG,
    &RCANFD.RSCFD0CFDTXQCC1.LONG
};
static uint32_t g_init_txqcc[CHANNEL_MAX] = {
    CAN_TXQCC0,
    CAN_TXQCC1
};

/* THLCC */
static __IO uint32_t *g_regtbl_thlcc[CHANNEL_MAX] = {
    &RCANFD.RSCFD0CFDTHLCC0.LONG,
    &RCANFD.RSCFD0CFDTHLCC1.LONG
};
static uint32_t g_init_thlcc[CHANNEL_MAX] = {
    CAN_THLCC0,
    CAN_THLCC1
};

/* RX rule register : ID */
static __IO uint32_t *g_regtbl_gaflid[RXRULE_J_MAX] = {
    &RCANFD.RSCFD0CFDGAFLID0.LONG,
    &RCANFD.RSCFD0CFDGAFLID1.LONG,
    &RCANFD.RSCFD0CFDGAFLID2.LONG,
    &RCANFD.RSCFD0CFDGAFLID3.LONG,
    &RCANFD.RSCFD0CFDGAFLID4.LONG,
    &RCANFD.RSCFD0CFDGAFLID5.LONG,
    &RCANFD.RSCFD0CFDGAFLID6.LONG,
    &RCANFD.RSCFD0CFDGAFLID7.LONG,
    &RCANFD.RSCFD0CFDGAFLID8.LONG,
    &RCANFD.RSCFD0CFDGAFLID9.LONG,
    &RCANFD.RSCFD0CFDGAFLID10.LONG,
    &RCANFD.RSCFD0CFDGAFLID11.LONG,
    &RCANFD.RSCFD0CFDGAFLID12.LONG,
    &RCANFD.RSCFD0CFDGAFLID13.LONG,
    &RCANFD.RSCFD0CFDGAFLID14.LONG,
    &RCANFD.RSCFD0CFDGAFLID15.LONG
};

/* RX rule register : Mask */
static __IO uint32_t *g_regtbl_gaflm[RXRULE_J_MAX] = {
    &RCANFD.RSCFD0CFDGAFLM0.LONG,
    &RCANFD.RSCFD0CFDGAFLM1.LONG,
    &RCANFD.RSCFD0CFDGAFLM2.LONG,
    &RCANFD.RSCFD0CFDGAFLM3.LONG,
    &RCANFD.RSCFD0CFDGAFLM4.LONG,
    &RCANFD.RSCFD0CFDGAFLM5.LONG,
    &RCANFD.RSCFD0CFDGAFLM6.LONG,
    &RCANFD.RSCFD0CFDGAFLM7.LONG,
    &RCANFD.RSCFD0CFDGAFLM8.LONG,
    &RCANFD.RSCFD0CFDGAFLM9.LONG,
    &RCANFD.RSCFD0CFDGAFLM10.LONG,
    &RCANFD.RSCFD0CFDGAFLM11.LONG,
    &RCANFD.RSCFD0CFDGAFLM12.LONG,
    &RCANFD.RSCFD0CFDGAFLM13.LONG,
    &RCANFD.RSCFD0CFDGAFLM14.LONG,
    &RCANFD.RSCFD0CFDGAFLM15.LONG
};

/* RX rule register : pointer1 */
static __IO uint32_t *g_regtbl_gaflp1[RXRULE_J_MAX] = {
    &RCANFD.RSCFD0CFDGAFLP1_0.LONG,
    &RCANFD.RSCFD0CFDGAFLP1_1.LONG,
    &RCANFD.RSCFD0CFDGAFLP1_2.LONG,
    &RCANFD.RSCFD0CFDGAFLP1_3.LONG,
    &RCANFD.RSCFD0CFDGAFLP1_4.LONG,
    &RCANFD.RSCFD0CFDGAFLP1_5.LONG,
    &RCANFD.RSCFD0CFDGAFLP1_6.LONG,
    &RCANFD.RSCFD0CFDGAFLP1_7.LONG,
    &RCANFD.RSCFD0CFDGAFLP1_8.LONG,
    &RCANFD.RSCFD0CFDGAFLP1_9.LONG,
    &RCANFD.RSCFD0CFDGAFLP1_10.LONG,
    &RCANFD.RSCFD0CFDGAFLP1_11.LONG,
    &RCANFD.RSCFD0CFDGAFLP1_12.LONG,
    &RCANFD.RSCFD0CFDGAFLP1_13.LONG,
    &RCANFD.RSCFD0CFDGAFLP1_14.LONG,
    &RCANFD.RSCFD0CFDGAFLP1_15.LONG
};

/* DLC to data length */
static const uint8_t dlc_to_length[16] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64
};

volatile int g_normal_bitrate[CHANNEL_MAX];
volatile int g_data_bitrate[CHANNEL_MAX] = {D_CH0_BITRATE, D_CH1_BITRATE};
uint32_t rx_buf[RX_BUFFER_MAX];
static can_irq_handler irq_handler[CHANNEL_MAX] = {NULL};
static uint32_t can_irq_id[CHANNEL_MAX] = {0};

void can_irq_init(can_t *obj, can_irq_handler handler, uint32_t id)
{
    irq_handler[obj->ch] = handler;
    can_irq_id[obj->ch] = id;
}

void can_irq_free(can_t *obj)
{
    can_irq_id[obj->ch] = 0;
    *g_regtbl_cfcc[obj->ch][CAN_RX] &= ~CFCC_CFRXIE;
    *g_regtbl_cfcc[obj->ch][CAN_TX] &= ~CFCC_CFTXIE;
    *g_regtbl_ctr[obj->ch] &= ~(CTR_EWIE | CTR_OLIE | CTR_EPIE | CTR_ALIE | CTR_BEIE);
    if (CH_0 == obj->ch) {
        GIC_DisableIRQ(CFRXI0_IRQn);
        GIC_DisableIRQ(CTXI0_IRQn);
        GIC_DisableIRQ(CERI0_IRQn);
    } else {
        GIC_DisableIRQ(CFRXI1_IRQn);
        GIC_DisableIRQ(CTXI1_IRQn);
        GIC_DisableIRQ(CERI1_IRQn);
    }
}

void can_irq_set(can_t *obj, CanIrqType type, uint32_t enable)
{
    switch (type) {
        case IRQ_RX:
            if (enable) {
                *g_regtbl_cfcc[obj->ch][CAN_RX] |= CFCC_CFRXIE;
            } else {
                *g_regtbl_cfcc[obj->ch][CAN_RX] &= ~CFCC_CFRXIE;
            }
            break;
        case IRQ_TX:
            if (enable) {
                *g_regtbl_cfcc[obj->ch][CAN_TX] |= CFCC_CFTXIE;
            } else {
                *g_regtbl_cfcc[obj->ch][CAN_TX] &= ~CFCC_CFTXIE;
            }
            break;
        case IRQ_ERROR:
        case IRQ_OVERRUN:
        case IRQ_PASSIVE:
        case IRQ_ARB:
        case IRQ_BUS:
            if (enable) {
                *g_regtbl_ctr[obj->ch] |= irq_enable_flag_tbl[type];
            } else {
                *g_regtbl_ctr[obj->ch] &= ~irq_enable_flag_tbl[type];
            }
            break;
        case IRQ_WAKEUP:
        /* not supported */
        /* fall through */
        default:
            return;
    }
    if (enable) {
        InterruptHandlerRegister(irq_tbl[obj->ch][type].id, irq_tbl[obj->ch][type].handler);
        GIC_SetPriority(irq_tbl[obj->ch][type].id, 20);
        GIC_EnableIRQ(irq_tbl[obj->ch][type].id);
        GIC_SetConfiguration(irq_tbl[obj->ch][type].id, 1);
    } else {
        GIC_DisableIRQ(irq_tbl[obj->ch][type].id);
    }
}

static void can_rec_irq(uint32_t ch)
{
    *g_regtbl_cfsts[ch][CAN_RX] &= ~CFSTS_CFRXIF;
    irq_handler[ch](can_irq_id[ch], IRQ_RX);
}

static void can_trx_irq(uint32_t ch)
{
    *g_regtbl_cfsts[ch][CAN_TX] &= ~CFSTS_CFTXIF;
    irq_handler[ch](can_irq_id[ch], IRQ_TX);
}

static void can_err_irq(uint32_t ch)
{
    /* error warning */
    if (*g_regtbl_erfl[ch] & ERFL_EWF) {
        *g_regtbl_erfl[ch] &= ~ERFL_EWF;
        irq_handler[ch](can_irq_id[ch], IRQ_ERROR);
    }

    /* over load */
    if (*g_regtbl_erfl[ch] & ERFL_OVLF) {
        *g_regtbl_erfl[ch] &= ~ERFL_OVLF;
        irq_handler[ch](can_irq_id[ch], IRQ_OVERRUN);
    }

    /* error passive */
    if (*g_regtbl_erfl[ch] & ERFL_EPF) {
        *g_regtbl_erfl[ch] &= ~ERFL_EPF;
        irq_handler[ch](can_irq_id[ch], IRQ_PASSIVE);
    }

    /* arbitration lost */
    if (*g_regtbl_erfl[ch] & ERFL_ALF) {
        *g_regtbl_erfl[ch] &= ~ERFL_ALF;
        irq_handler[ch](can_irq_id[ch], IRQ_ARB);
    }

    /* bus error */
    if (*g_regtbl_erfl[ch] & ERFL_ALLERR) {
        *g_regtbl_erfl[ch] &= ~ERFL_ALLERR;
        *g_regtbl_erfl[ch] &= ~ERFL_BEF;
        irq_handler[ch](can_irq_id[ch], IRQ_BUS);
    }
}

static void can0_rec_irq(void)
{
    can_rec_irq(CAN_0);
}

static void can0_trx_irq(void)
{
    can_trx_irq(CAN_0);
}

static void can0_err_irq(void)
{
    can_err_irq(CAN_0);
}

static void can1_rec_irq(void)
{
    can_rec_irq(CAN_1);
}

static void can1_trx_irq(void)
{
    can_trx_irq(CAN_1);
}

static void can1_err_irq(void)
{
    can_err_irq(CAN_1);
}

static void set_normal_bitrate(uint32_t ch, int f)
{
    uint32_t clkc;
    uint16_t tq_total;
    uint16_t brp;
    uint8_t  tseg1 = 0;
    uint8_t  tseg2 = 0;
    uint8_t  sjw = 0;

    /* clkc */
    if (false == RZ_A2_IsClockMode0()) {
        clkc = CM1_RENESAS_RZ_A2_P1_CLK / 2;
    } else {
        clkc = CM0_RENESAS_RZ_A2_P1_CLK / 2;
    }
    /* calculate NTSEG1 bit and NTSEG2 bit */
    for (brp = 1; brp < 1024; brp++) {
        tq_total = clkc / (f * brp);
        if ((N_TOTAL_TQMIN <= tq_total) && (tq_total <= N_TOTAL_TQMAX)) {
            tseg1 = tq_total * N_SAMPLE_POINT - NSS_MAX;
            tseg2 = tq_total - (NSS_MAX + tseg1);
            if ((NTSEG1_MAX >= tseg1) && (NTSEG2_MAX >= tseg2)) {
                break;
            }
        }
    }
    /* calculate NSJW */
    sjw = (tseg2 > N_SJW) ? N_SJW : tseg2;
    /* set RSCAN0CmCFG register */
    *g_regtbl_ncfg[ch] = ((tseg2 - 1) << 24) | ((tseg1 - 1) << 16) | ((sjw - 1) << 11) | (brp - 1);
}

static void set_data_bitrate(uint32_t ch, int f)
{
    uint32_t clkc;
    uint16_t tq_total;
    uint16_t dbrp;
    uint8_t  dtseg1 = 0;
    uint8_t  dtseg2 = 0;
    uint8_t  dsjw = 0;

    /* clkc */
    if (false == RZ_A2_IsClockMode0()) {
        clkc = CM1_RENESAS_RZ_A2_P1_CLK / 2;
    } else {
        clkc = CM0_RENESAS_RZ_A2_P1_CLK / 2;
    }
    /* calculate DTSEG1 bit and DTSEG2 bit */
    for (dbrp = 1; dbrp < 256; dbrp++) {
        tq_total = clkc / (f * dbrp);
        if ((D_TOTAL_TQMIN <= tq_total) && (tq_total <= D_TOTAL_TQMAX)) {
            dtseg1 = tq_total * D_SAMPLE_POINT - DSS_MAX;
            dtseg2 = tq_total - (DSS_MAX + dtseg1);
            if ((DTSEG1_MAX >= dtseg1) && (DTSEG2_MAX >= dtseg2)) {
                break;
            }
        }
    }
    /* calculate DSJW */
    dsjw = (dtseg2 >= D_SJW) ? D_SJW : dtseg2;
    /* set RSCAN0CmDCFG register */
    *g_regtbl_dcfg[ch] = ((dsjw - 1) << 24) | ((dtseg2 - 1) << 20) | ((dtseg1 - 1) << 16) | (dbrp - 1);
}

static void reset_register(uint32_t ch)
{
    /* Global function setting */
    REG_32W(&RCANFD.RSCFD0CFDGCFG.LONG, CAN_GCFG, REG_NONSHIFT, REG_NONMASK);
    REG_32W(&RCANFD.RSCFD0CFDGFDCFG.LONG, CAN_GFDCFG, REG_NONSHIFT, REG_NONMASK);

    /* Communication speed setting */
    set_normal_bitrate(ch, g_normal_bitrate[ch]);
    set_data_bitrate(ch, g_data_bitrate[ch]);

    /* Rx rule setting */
    Wrap_Can_SetRxRule();

    /* Buffer setting */
    reset_buffer(ch);

    /* Global Error Interrupt setting */
    REG_32W(&RCANFD.RSCFD0CFDGCTR.LONG, CAN_GCTR, REG_NONSHIFT, MASK_GCTR_INIT);

    /* CANFD setting */
    REG_32W(g_regtbl_ctr[ch], g_ctr_init[ch], REG_NONSHIFT, MASK_CTR_INIT);
    REG_32W(g_regtbl_fdcfg[ch], g_fdcfg_init[ch], REG_NONSHIFT, MASK_FDCFG_INIT);

    /* Global Test mode switch (reset->test) */
    R_CAN_Control(UNUSED, GLOBAL_TEST);

    /* Channel halt mode switch (reset->halt) */
    R_CAN_Control(ch, CHANNEL_HALT);

    /* Channel Restricted Operation Mode setting */
    REG_32W(g_regtbl_ctr[ch], g_ctr_init[ch], REG_NONSHIFT, CTR_ROM);
}

static void reset_buffer(uint32_t ch)
{
    /* TR FIFO */
    REG_32W(g_regtbl_cfcc[ch][CAN_TX], g_cfcc_init[ch][CAN_TX], REG_NONSHIFT, MASK_CFCC_INIT);
    REG_32W(g_regtbl_cfcc[ch][CAN_RX], g_cfcc_init[ch][CAN_RX], REG_NONSHIFT, MASK_CFCC_INIT);

    /* RX FIFO */
    REG_32W(g_regtbl_rfcc[ch], g_rfcc_init[ch], REG_NONSHIFT, MASK_RFCC_INIT);

    /* RX buffer */
    REG_32W(&RCANFD.RSCFD0CFDRMNB.LONG, CAN_RMNB, REG_NONSHIFT, REG_NONMASK);

    /* TX buffer */
    REG_32W(&RCANFD.RSCFD0CFDTMIEC0.LONG, CAN_TMIEC0, REG_NONSHIFT, REG_NONMASK);

    /* TX queue */
    REG_32W(g_regtbl_txqcc[ch], g_init_txqcc[ch], REG_NONSHIFT, (TXQCC_TXQDC | TXQCC_TXQIE | TXQCC_TXQIM));

    /* TX history */
    REG_32W(g_regtbl_thlcc[ch], g_init_thlcc[ch], REG_NONSHIFT, (THLCC_THLIE | THLCC_THLIM | THLCC_THLDTE));
}

static void get_rxrule_index(uint32_t ch, int32_t handle, uint8_t *page, uint8_t *j)
{
    *page = (handle + (ch * CAN0_RX_RULE_NUM)) / RXRULE_J_MAX;
    *j    = (handle + (ch * CAN0_RX_RULE_NUM)) % RXRULE_J_MAX;
}

static void reconfigure_channel(uint32_t ch)
{
    R_CAN_Control(UNUSED, GLOBAL_OPERATION);
    R_CAN_Control(ch, CHANNEL_COM);
    REG_32W(g_regtbl_cfcc[ch][CAN_TX], CFCC_CFE, REG_NONSHIFT, CFCC_CFE);
    REG_32W(g_regtbl_cfcc[ch][CAN_RX], CFCC_CFE, REG_NONSHIFT, CFCC_CFE);
}

static void set_test_mode(uint32_t ch, uint32_t mode, uint32_t enable)
{
    R_CAN_Control(ch, CHANNEL_HALT);
    REG_32W(g_regtbl_ctr[ch], mode, CTR_CTMS_SHIFT, CTR_CTMS);
    REG_32W(g_regtbl_ctr[ch], enable, CTR_CTME_SHIFT, CTR_CTME);
}

void can_init(can_t *obj, PinName rd, PinName td)
{
    /* set default frequency */
    can_init_freq(obj, rd, td, N_BITRATE_DEFAULT);
}

void can_init_freq(can_t *obj, PinName rd, PinName td, int hz)
{
    /* determine the CAN to use */
    uint32_t can_rx = pinmap_peripheral(rd, PinMap_CAN_RD);
    uint32_t can_tx = pinmap_peripheral(td, PinMap_CAN_TD);
    obj->ch = pinmap_merge(can_tx, can_rx);
    MBED_ASSERT((int)obj->ch != NC);

    /* initialize */
    CPG.STBCR3.BYTE &= ~CPG_STBCR3_MSTP32;
    R_CAN_PortSet(obj->ch, ENABLE_COMMON);
    R_CAN_PortSet(obj->ch, ENABLE_CHANNEL);
    Wrap_Can_WaitRamInitOver();
    R_CAN_Control(UNUSED, GLOBAL_RESET);
    RCANFD.RSCFD0CFDGRMCFG.LONG |= RCANFD_RSCFD0CFDGRMCFG_RCMC;
    R_CAN_Control(obj->ch, CHANNEL_RESET);
    g_normal_bitrate[obj->ch] = hz;
    reset_register(obj->ch);
    reconfigure_channel(obj->ch);

    /* pin out the can pins */
    pinmap_pinout(rd, PinMap_CAN_RD);
    pinmap_pinout(td, PinMap_CAN_TD);
}

void can_free(can_t *obj)
{
    /* disable CANFD clock */
    CPG.STBCR3.BYTE |= CPG_STBCR3_MSTP32;
}

int can_frequency(can_t *obj, int f)
{
    int retval = 0;

    /* less than normal bitrate */
    if (f <= N_BITRATE_MAX) {
        /* set normal bit rate */
        R_CAN_Control(obj->ch, CHANNEL_RESET);
        g_normal_bitrate[obj->ch] = f;
        set_normal_bitrate(obj->ch, g_normal_bitrate[obj->ch]);
        reconfigure_channel(obj->ch);
        retval = 1;
    }
    return retval;
}

int can_write(can_t *obj, CAN_Message msg, int cc)
{
    can_frame_t frame;

    /* CAN frame */
    frame.ID    = msg.id;       // Message ID
    frame.THDSE = 0;
    frame.RTR   = msg.type;     // 0:CANData, 1:CANRemote
    frame.IDE   = msg.format;   // 0:Standard ID, 1:Extend ID
    frame.LBL   = 0;
    frame.DLC   = msg.len;      // Data length
    frame.FDSTS = 1;
    frame.BRS   = 1;
    frame.ESI   = 0;

    if (R_CAN_OK != R_CAN_TxSet(TX_TRFIFO, TX_TRFIFO_INDEX(obj->ch), &frame, (uint32_t *)&msg.data[0])) {
        return 0;
    }
    if (R_CAN_OK != R_CAN_Tx(TX_TRFIFO, TX_TRFIFO_INDEX(obj->ch), ENABLE)) {
        return 0;
    }
    return 1;
}

int can_read(can_t *obj, CAN_Message *msg, int handle)
{
    uint32_t  page_index = 0;
    uint8_t   received_msg_num = 0;
    uint8_t   msg_cnt;

    if (R_CAN_OK != R_CAN_RxPoll(RX_TRFIFO, RX_TRFIFO_INDEX(obj->ch))) {
        return 0;
    }
    if (R_CAN_OK != R_CAN_RxRead(RX_TRFIFO, RX_TRFIFO_INDEX(obj->ch), (uint32_t *)&rx_buf[0])) {
        return 0;
    }

    /* number of received message */
    received_msg_num = rx_buf[0];

    for (msg_cnt = 0; msg_cnt < received_msg_num; msg_cnt++) {
        page_index = msg_cnt * 19;
        /* IDE 0:Standard ID, 1:Extend ID */
        msg->format = (CANFormat)(rx_buf[page_index + 1] >> 31);
        /* RTR/RRS : Data or Remote */
        msg->type = (CANType)((rx_buf[page_index + 1] >> 30) & 0x1);
        /* ID : Standard ID(b10-b0), Extend ID(b28-b0) */
        msg->id = (rx_buf[page_index + 1] & 0x1FFFFFFF);
        /* data Length */
        msg->len = dlc_to_length[(uint8_t)(rx_buf[page_index + 2] >> 28)];
        /* received data */
        msg->data[0] = (uint8_t)(rx_buf[page_index + 4] & 0x000000FF);
        msg->data[1] = (uint8_t)(rx_buf[page_index + 4] >> 8)  & 0x000000FF;
        msg->data[2] = (uint8_t)(rx_buf[page_index + 4] >> 16) & 0x000000FF;
        msg->data[3] = (uint8_t)(rx_buf[page_index + 4] >> 24) & 0x000000FF;
        msg->data[4] = (uint8_t)(rx_buf[page_index + 5] & 0x000000FF);
        msg->data[5] = (uint8_t)(rx_buf[page_index + 5] >> 8)  & 0x000000FF;
        msg->data[6] = (uint8_t)(rx_buf[page_index + 5] >> 16) & 0x000000FF;
        msg->data[7] = (uint8_t)(rx_buf[page_index + 5] >> 24) & 0x000000FF;
    }

    return 1; /* message arrived */
}

int can_mode(can_t *obj, CanMode mode)
{
    int retval = 1;
    switch (mode) {
        case MODE_RESET:
            /* set to reset mode */
            R_CAN_Control(UNUSED, GLOBAL_RESET);
            R_CAN_Control(obj->ch, CHANNEL_RESET);
            break;
        case MODE_NORMAL:
            /* disable test mode */
            set_test_mode(obj->ch, STD_TEST, TEST_DISABLE);
            reconfigure_channel(obj->ch);
            break;
        case MODE_SILENT:
            /* enable listen only mode */
            set_test_mode(obj->ch, TEST_LISTEN_ONLY, TEST_ENABLE);
            reconfigure_channel(obj->ch);
            break;
        case MODE_TEST_LOCAL:
            /* enable self test mode 0 */
            set_test_mode(obj->ch, EXT_LOOPBACK, TEST_ENABLE);
            reconfigure_channel(obj->ch);
            break;
        case MODE_TEST_GLOBAL:
            /* enable Inter-Channel Communication Test */
            R_CAN_Control(UNUSED, GLOBAL_TEST);
            REG_32W(&RCANFD.RSCFD0CFDGTSTCFG.LONG, TEST_ENABLE, GTSTCFG_C0ICBCE_SHIFT, GTSTCFG_C0ICBCE);
            REG_32W(&RCANFD.RSCFD0CFDGTSTCFG.LONG, TEST_ENABLE, GTSTCFG_C1ICBCE_SHIFT, GTSTCFG_C1ICBCE);
            REG_32W(&RCANFD.RSCFD0CFDGTSTCTR.LONG, TEST_ENABLE, GTSTCTR_ICBCTME_SHIFT, GTSTCTR_ICBCTME);
            /* enable standard test */
            set_test_mode(CH_0, STD_TEST, TEST_ENABLE);
            set_test_mode(CH_1, STD_TEST, TEST_ENABLE);
            reconfigure_channel(CH_0);
            reconfigure_channel(CH_1);
            break;
        case MODE_TEST_SILENT:
            /* enable self test mode 1 */
            set_test_mode(obj->ch, INT_LOOPBACK, TEST_ENABLE);
            reconfigure_channel(obj->ch);
            break;
        default:
            retval = 0;
            break;
    }
    return retval;
}

int can_filter(can_t *obj, uint32_t id, uint32_t mask, CANFormat format, int32_t handle)
{
    uint8_t page_index;
    uint8_t j_index;

    if ((CH_0 == obj->ch) && (handle >= CAN0_RX_RULE_NUM)) {
        return 0;
    }
    if ((CH_1 == obj->ch) && (handle >= CAN1_RX_RULE_NUM)) {
        return 0;
    }

    /* set to reset mode */
    R_CAN_Control(UNUSED, GLOBAL_RESET);
    R_CAN_Control(obj->ch, CHANNEL_RESET);

    /* write enable (RSCFD0CFDGAFLECTR.AFLDAE) */
    RCANFD.RSCFD0CFDGAFLECTR.LONG |= RCANFD_RSCFD0CFDGAFLECTR_AFLDAE;

    /* set page number */
    get_rxrule_index(obj->ch, handle, &page_index, &j_index);
    REG_32W(&RCANFD.RSCFD0CFDGAFLECTR.LONG, page_index, REG_NONSHIFT, 0x0000001F);

    /* set CAN format */
    REG_32W(g_regtbl_gaflid[j_index], format, 31, 0x80000000);
    if (CANExtended == format) {
        REG_32W(g_regtbl_gaflid[j_index], (id & 0x1FFFFFFF), REG_NONSHIFT, 0x1FFFFFFF);
    } else {
        REG_32W(g_regtbl_gaflid[j_index], (id & 0x000007FF), REG_NONSHIFT, 0x000007FF);
    }

    /* set masks(IDE, RTR, ID) */
    if (CANAny == format) {
        /* no masks */
        REG_32W(g_regtbl_gaflm[j_index], 0, REG_NONSHIFT, REG_NONMASK);
    } else {
        /* IDE and ID */
        REG_32W(g_regtbl_gaflm[j_index], (0x80000000 | mask), REG_NONSHIFT, REG_NONMASK);
    }
    /* set GAFLFDP1_j.GAFLFDP */
    if (CH_0 == obj->ch) {
        REG_32W(g_regtbl_gaflp1[j_index], 0x00000200, REG_NONSHIFT, REG_NONMASK); // CH0:use TRFIFO1
    } else {
        REG_32W(g_regtbl_gaflp1[j_index], 0x00001000, REG_NONSHIFT, REG_NONMASK); // CH1:use TRFIFO4
    }

    /* write disable (RSCFD0CFDGAFLECTR.AFLDAE) */
    RCANFD.RSCFD0CFDGAFLECTR.LONG &= ~RCANFD_RSCFD0CFDGAFLECTR_AFLDAE;

    reconfigure_channel(obj->ch);

    return 1;
}

void can_reset(can_t *obj)
{
    R_CAN_Control(UNUSED, GLOBAL_RESET);
    R_CAN_Control(obj->ch, CHANNEL_RESET);
    reset_register(obj->ch);
    reconfigure_channel(obj->ch);
}

unsigned char can_rderror(can_t *obj)
{
    return (unsigned char)(*g_regtbl_sts[obj->ch] >> RCANFD_RSCFD0CFDC0STS_REC_SHIFT);
}

unsigned char can_tderror(can_t *obj)
{
    return (unsigned char)(*g_regtbl_sts[obj->ch] >> RCANFD_RSCFD0CFDC0STS_TEC_SHIFT);
}

void can_monitor(can_t *obj, int silent)
{
    if (silent) {
        set_test_mode(obj->ch, TEST_LISTEN_ONLY, TEST_ENABLE);
    } else {
        set_test_mode(obj->ch, STD_TEST, TEST_DISABLE);
    }
    reconfigure_channel(obj->ch);
}

const PinMap *can_rd_pinmap()
{
    return PinMap_CAN_TD;
}

const PinMap *can_td_pinmap()
{
    return PinMap_CAN_RD;
}
#endif // DEVICE_CAN

