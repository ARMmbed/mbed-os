/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include <string.h>
#include "mbed_assert.h"
#include "can_api.h"
#include "RZ_A1_Init.h"
#include "cmsis.h"
#include "pinmap.h"
#include "rscan0_iodefine.h"
#include "r_typedefs.h"
#include "MBRZA1H.h"

#define CAN_NUM         5
#define CAN_SND_RCV     2
#define IRQ_NUM         8

static void can_rec_irq(uint32_t ch);
static void can_trx_irq(uint32_t ch);
static void can_err_irq(uint32_t ch, CanIrqType type);
static void can0_rec_irq(void);
static void can1_rec_irq(void);
static void can2_rec_irq(void);
static void can3_rec_irq(void);
static void can4_rec_irq(void);
static void can0_trx_irq(void);
static void can1_trx_irq(void);
static void can2_trx_irq(void);
static void can3_trx_irq(void);
static void can4_trx_irq(void);
static void can0_err_warning_irq(void);
static void can1_err_warning_irq(void);
static void can2_err_warning_irq(void);
static void can3_err_warning_irq(void);
static void can4_err_warning_irq(void);
static void can0_overrun_irq(void);
static void can1_overrun_irq(void);
static void can2_overrun_irq(void);
static void can3_overrun_irq(void);
static void can4_overrun_irq(void);
static void can0_passive_irq(void);
static void can1_passive_irq(void);
static void can2_passive_irq(void);
static void can3_passive_irq(void);
static void can4_passive_irq(void);
static void can0_arb_lost_irq(void);
static void can1_arb_lost_irq(void);
static void can2_arb_lost_irq(void);
static void can3_arb_lost_irq(void);
static void can4_arb_lost_irq(void);
static void can0_bus_err_irq(void);
static void can1_bus_err_irq(void);
static void can2_bus_err_irq(void);
static void can3_bus_err_irq(void);
static void can4_bus_err_irq(void);
static void can_reset_reg(can_t *obj);
static void can_reset_recv_rule(can_t *obj);
static void can_reset_buffer(can_t *obj);
static void can_reconfigure_channel(void);
static void can_set_frequency(can_t *obj, int f);
static void can_set_global_mode(int mode);
static void can_set_channel_mode(uint32_t ch, int mode);

typedef enum {
    CAN_SEND = 0,
    CAN_RECV
} CANfunc;

typedef enum {
    GL_OPE = 0,
    GL_RESET,
    GL_TEST
} Globalmode;

typedef enum {
    CH_COMM = 0,
    CH_RESET,
    CH_HOLD
} Channelmode;

typedef struct {
    IRQn_Type   int_num;    /* Interrupt number */
    IRQHandler  handler;    /* Interrupt handler */
} can_info_int_t;

static can_irq_handler irq_handler;
static uint32_t can_irq_id[CAN_NUM];
static int can_initialized[CAN_NUM] = {0};

static const PinMap PinMap_CAN_RD[] = {
    {P7_8  , CAN_0, 4},
    {P9_1  , CAN_0, 3},
    {P1_4  , CAN_1, 3},
    {P5_9  , CAN_1, 5},
    {P7_11 , CAN_1, 4},
    {P11_12, CAN_1, 1},
    {P4_9  , CAN_2, 6},
    {P6_4  , CAN_2, 3},
    {P7_2  , CAN_2, 5},
    {P2_12 , CAN_3, 5},
    {P4_2  , CAN_3, 4},
    {P1_5  , CAN_4, 3},
    {P2_14 , CAN_4, 5},
    {NC    , NC   , 0}
};

static const PinMap PinMap_CAN_TD[] = {
    {P7_9  , CAN_0, 4},
    {P9_0  , CAN_0, 3},
    {P5_10 , CAN_1, 5},
    {P7_10 , CAN_1, 4},
    {P11_13, CAN_1, 1},
    {P4_8  , CAN_2, 6},
    {P6_5  , CAN_2, 3},
    {P7_3  , CAN_2, 5},
    {P2_13 , CAN_3, 5},
    {P4_3  , CAN_3, 4},
    {P4_11 , CAN_4, 6},
    {P8_10 , CAN_4, 5},
    {NC    , NC   , 0}
};

static __IO uint32_t *CTR_MATCH[] = {
    &RSCAN0C0CTR,
    &RSCAN0C1CTR,
    &RSCAN0C2CTR,
    &RSCAN0C3CTR,
    &RSCAN0C4CTR,
};

static __IO uint32_t *CFG_MATCH[] = {
    &RSCAN0C0CFG,
    &RSCAN0C1CFG,
    &RSCAN0C2CFG,
    &RSCAN0C3CFG,
    &RSCAN0C4CFG,
};

static __IO uint32_t *RFCC_MATCH[] = {
    &RSCAN0RFCC0,
    &RSCAN0RFCC1,
    &RSCAN0RFCC2,
    &RSCAN0RFCC3,
    &RSCAN0RFCC4,
    &RSCAN0RFCC5,
    &RSCAN0RFCC6,
    &RSCAN0RFCC7
};

static __IO uint32_t *TXQCC_MATCH[] = {
    &RSCAN0TXQCC0,
    &RSCAN0TXQCC1,
    &RSCAN0TXQCC2,
    &RSCAN0TXQCC3,
    &RSCAN0TXQCC4,
};

static __IO uint32_t *THLCC_MATCH[] = {
    &RSCAN0THLCC0,
    &RSCAN0THLCC1,
    &RSCAN0THLCC2,
    &RSCAN0THLCC3,
    &RSCAN0THLCC4,
};

static __IO uint32_t *STS_MATCH[] = {
    &RSCAN0C0STS,
    &RSCAN0C1STS,
    &RSCAN0C2STS,
    &RSCAN0C3STS,
    &RSCAN0C4STS,
};

static __IO uint32_t *ERFL_MATCH[] = {
    &RSCAN0C0ERFL,
    &RSCAN0C1ERFL,
    &RSCAN0C2ERFL,
    &RSCAN0C3ERFL,
    &RSCAN0C4ERFL,
};

static __IO uint32_t *CFCC_TBL[CAN_NUM][CAN_SND_RCV] = {
    { &RSCAN0CFCC0 , &RSCAN0CFCC1  },
    { &RSCAN0CFCC3 , &RSCAN0CFCC4  },
    { &RSCAN0CFCC6 , &RSCAN0CFCC7  },
    { &RSCAN0CFCC9 , &RSCAN0CFCC10 },
    { &RSCAN0CFCC12, &RSCAN0CFCC13 }
};

static __IO uint32_t *CFSTS_TBL[CAN_NUM][CAN_SND_RCV] = {
    { &RSCAN0CFSTS0 , &RSCAN0CFSTS1  },
    { &RSCAN0CFSTS3 , &RSCAN0CFSTS4  },
    { &RSCAN0CFSTS6 , &RSCAN0CFSTS7  },
    { &RSCAN0CFSTS9 , &RSCAN0CFSTS10 },
    { &RSCAN0CFSTS12, &RSCAN0CFSTS13 }
};

static __IO uint32_t *CFPCTR_TBL[CAN_NUM][CAN_SND_RCV] = {
    { &RSCAN0CFPCTR0 , &RSCAN0CFPCTR1  },
    { &RSCAN0CFPCTR3 , &RSCAN0CFPCTR4  },
    { &RSCAN0CFPCTR6 , &RSCAN0CFPCTR7  },
    { &RSCAN0CFPCTR9 , &RSCAN0CFPCTR10 },
    { &RSCAN0CFPCTR12, &RSCAN0CFPCTR13 }
};

static __IO uint32_t *CFID_TBL[CAN_NUM][CAN_SND_RCV] = {
    { &RSCAN0CFID0 , &RSCAN0CFID1  },
    { &RSCAN0CFID3 , &RSCAN0CFID4  },
    { &RSCAN0CFID6 , &RSCAN0CFID7  },
    { &RSCAN0CFID9 , &RSCAN0CFID10 },
    { &RSCAN0CFID12, &RSCAN0CFID13 }
};

static __IO uint32_t *CFPTR_TBL[CAN_NUM][CAN_SND_RCV] = {
    { &RSCAN0CFPTR0 , &RSCAN0CFPTR1  },
    { &RSCAN0CFPTR3 , &RSCAN0CFPTR4  },
    { &RSCAN0CFPTR6 , &RSCAN0CFPTR7  },
    { &RSCAN0CFPTR9 , &RSCAN0CFPTR10 },
    { &RSCAN0CFPTR12, &RSCAN0CFPTR13 }
};

static __IO uint32_t *CFDF0_TBL[CAN_NUM][CAN_SND_RCV] = {
    { &RSCAN0CFDF00 , &RSCAN0CFDF01  },
    { &RSCAN0CFDF03 , &RSCAN0CFDF04  },
    { &RSCAN0CFDF06 , &RSCAN0CFDF07  },
    { &RSCAN0CFDF09 , &RSCAN0CFDF010 },
    { &RSCAN0CFDF012, &RSCAN0CFDF013 }
};

static __IO uint32_t *CFDF1_TBL[CAN_NUM][CAN_SND_RCV] = {
    { &RSCAN0CFDF10 , &RSCAN0CFDF11  },
    { &RSCAN0CFDF13 , &RSCAN0CFDF14  },
    { &RSCAN0CFDF16 , &RSCAN0CFDF17  },
    { &RSCAN0CFDF19 , &RSCAN0CFDF110 },
    { &RSCAN0CFDF112, &RSCAN0CFDF113 }
};

static const can_info_int_t can_int_info[CAN_NUM][IRQ_NUM] = 
{
    {   /* ch0 */
        { INTRCAN0REC_IRQn, can0_rec_irq         }, /* RxIrq */
        { INTRCAN0TRX_IRQn, can0_trx_irq         }, /* TxIrq */
        { INTRCAN0ERR_IRQn, can0_err_warning_irq }, /* EwIrq */
        { INTRCAN0ERR_IRQn, can0_overrun_irq     }, /* DoIrq */
        { INTRCAN0ERR_IRQn, NULL                 }, /* WuIrq(not supported) */
        { INTRCAN0ERR_IRQn, can0_passive_irq     }, /* EpIrq */
        { INTRCAN0ERR_IRQn, can0_arb_lost_irq    }, /* AlIrq */
        { INTRCAN0ERR_IRQn, can0_bus_err_irq     }  /* BeIrq */
    },
    {   /* ch1 */
        { INTRCAN1REC_IRQn, can1_rec_irq         }, /* RxIrq */
        { INTRCAN1TRX_IRQn, can1_trx_irq         }, /* TxIrq */
        { INTRCAN1ERR_IRQn, can1_err_warning_irq }, /* EwIrq */
        { INTRCAN1ERR_IRQn, can1_overrun_irq     }, /* DoIrq */
        { INTRCAN1ERR_IRQn, NULL                 }, /* WuIrq(not supported) */
        { INTRCAN1ERR_IRQn, can1_passive_irq     }, /* EpIrq */
        { INTRCAN1ERR_IRQn, can1_arb_lost_irq    }, /* AlIrq */
        { INTRCAN1ERR_IRQn, can1_bus_err_irq     }  /* BeIrq */
    },
    {   /* ch2 */
        { INTRCAN2REC_IRQn, can2_rec_irq         }, /* RxIrq */
        { INTRCAN2TRX_IRQn, can2_trx_irq         }, /* TxIrq */
        { INTRCAN2ERR_IRQn, can2_err_warning_irq }, /* EwIrq */
        { INTRCAN2ERR_IRQn, can2_overrun_irq     }, /* DoIrq */
        { INTRCAN2ERR_IRQn, NULL                 }, /* WuIrq(not supported) */
        { INTRCAN2ERR_IRQn, can2_passive_irq     }, /* EpIrq */
        { INTRCAN2ERR_IRQn, can2_arb_lost_irq    }, /* AlIrq */
        { INTRCAN2ERR_IRQn, can2_bus_err_irq     }  /* BeIrq */
    },
    {   /* ch3 */
        { INTRCAN3REC_IRQn, can3_rec_irq         }, /* RxIrq */
        { INTRCAN3TRX_IRQn, can3_trx_irq         }, /* TxIrq */
        { INTRCAN3ERR_IRQn, can3_err_warning_irq }, /* EwIrq */
        { INTRCAN3ERR_IRQn, can3_overrun_irq     }, /* DoIrq */
        { INTRCAN3ERR_IRQn, NULL                 }, /* WuIrq(not supported) */
        { INTRCAN3ERR_IRQn, can3_passive_irq     }, /* EpIrq */
        { INTRCAN3ERR_IRQn, can3_arb_lost_irq    }, /* AlIrq */
        { INTRCAN3ERR_IRQn, can3_bus_err_irq     }  /* BeIrq */
    },
    {   /* ch4 */
        { INTRCAN4REC_IRQn, can4_rec_irq         }, /* RxIrq */
        { INTRCAN4TRX_IRQn, can4_trx_irq         }, /* TxIrq */
        { INTRCAN4ERR_IRQn, can4_err_warning_irq }, /* EwIrq */
        { INTRCAN4ERR_IRQn, can4_overrun_irq     }, /* DoIrq */
        { INTRCAN4ERR_IRQn, NULL                 }, /* WuIrq(not supported) */
        { INTRCAN4ERR_IRQn, can4_passive_irq     }, /* EpIrq */
        { INTRCAN4ERR_IRQn, can4_arb_lost_irq    }, /* AlIrq */
        { INTRCAN4ERR_IRQn, can4_bus_err_irq     }  /* BeIrq */
    }
};

static __IO uint32_t *dmy_gaflid = &RSCAN0GAFLID0;
static __IO uint32_t *dmy_gaflm  = &RSCAN0GAFLM0;
static __IO uint32_t *dmy_gaflp0 = &RSCAN0GAFLP00;
static __IO uint32_t *dmy_gaflp1 = &RSCAN0GAFLP10;

void can_irq_init(can_t *obj, can_irq_handler handler, uint32_t id) {
    irq_handler = handler;
    can_irq_id[obj->ch] = id;
}

void can_irq_free(can_t *obj) {
    can_irq_id[obj->ch] = 0;
}

void can_irq_set(can_t *obj, CanIrqType type, uint32_t enable) {
    __IO uint32_t *dmy_ctr;

    /* Wake-up Irq is not supported */
    if (type != IRQ_WAKEUP) {
        if (enable) {
            dmy_ctr = CTR_MATCH[obj->ch];
            if (type == IRQ_ERROR) {
                /* EWIE interrupts is enable */
                *dmy_ctr |= 0x00000200;
            } else if (type == IRQ_OVERRUN) {
                /* OLIE interrupts is enable */
                *dmy_ctr |= 0x00002000;
            } else if (type == IRQ_PASSIVE) {
                /* EPIE interrupts is enable */
                *dmy_ctr |= 0x00000400;
            } else if (type == IRQ_ARB) {
                /* ALIE interrupts is enable */
                *dmy_ctr |= 0x00008000;
            } else if (type == IRQ_BUS) {
                /* BEIE interrupts is enable */
                *dmy_ctr |= 0x00000100;
            }
            InterruptHandlerRegister(can_int_info[obj->ch][type].int_num, can_int_info[obj->ch][type].handler);
            GIC_SetPriority(can_int_info[obj->ch][type].int_num, 5);
            GIC_EnableIRQ(can_int_info[obj->ch][type].int_num);
        } else {
            GIC_DisableIRQ(can_int_info[obj->ch][type].int_num);
        }
    }
}

static void can_rec_irq(uint32_t ch) {
    __IO uint32_t *dmy_cfsts;

    dmy_cfsts = CFSTS_TBL[ch][CAN_RECV];
    *dmy_cfsts &= 0xFFFFFFF7;           // Clear CFRXIF

    irq_handler(can_irq_id[ch], IRQ_RX);
}

static void can_trx_irq(uint32_t ch) {
    __IO uint32_t *dmy_cfsts;

    dmy_cfsts = CFSTS_TBL[ch][CAN_SEND];
    *dmy_cfsts &= 0xFFFFFFEF;           // Clear CFTXIF

    irq_handler(can_irq_id[ch], IRQ_TX);
}

static void can_err_irq(uint32_t ch, CanIrqType type) {
    __IO uint32_t *dmy_erfl;
    int val = 1;
    
    dmy_erfl = ERFL_MATCH[ch];
    switch (type) {
        case IRQ_ERROR:
            *dmy_erfl &= 0xFFFFFFFD;    // Clear EWF
            break;
        case IRQ_OVERRUN:
            *dmy_erfl &= 0xFFFFFFDF;    // Clear OVLF
            break;
        case IRQ_PASSIVE:
            *dmy_erfl &= 0xFFFFFFFB;    // Clear EPF
            break;
        case IRQ_ARB:
            *dmy_erfl &= 0xFFFFFF7F;    // Clear ALF
            break;
        case IRQ_BUS:
            *dmy_erfl &= 0xFFFF00FF;    // Clear ADERR、B0ERR、B1ERR、CERR、AERR、FERR、SERR
            *dmy_erfl &= 0xFFFFFFFE;    // Clear BEF
            break;
        case IRQ_WAKEUP:
            /* not supported */
            /* fall through */
        default:
            val = 0;
            break;
    }
    if (val == 1) {
        irq_handler(can_irq_id[ch], type);
    }
}

static void can0_rec_irq(void) {
    can_rec_irq(CAN_0);
}

static void can1_rec_irq(void) {
    can_rec_irq(CAN_1);
}

static void can2_rec_irq(void) {
    can_rec_irq(CAN_2);
}

static void can3_rec_irq(void) {
    can_rec_irq(CAN_3);
}

static void can4_rec_irq(void) {
    can_rec_irq(CAN_4);
}

static void can0_trx_irq(void) {
    can_trx_irq(CAN_0);
}

static void can1_trx_irq(void) {
    can_trx_irq(CAN_1);
}

static void can2_trx_irq(void) {
    can_trx_irq(CAN_2);
}

static void can3_trx_irq(void) {
    can_trx_irq(CAN_3);
}

static void can4_trx_irq(void) {
    can_trx_irq(CAN_4);
}

static void can0_err_warning_irq(void) {
    can_err_irq(CAN_0, IRQ_ERROR);
}

static void can1_err_warning_irq(void) {
    can_err_irq(CAN_1, IRQ_ERROR);
}

static void can2_err_warning_irq(void) {
    can_err_irq(CAN_2, IRQ_ERROR);
}

static void can3_err_warning_irq(void) {
    can_err_irq(CAN_3, IRQ_ERROR);
}

static void can4_err_warning_irq(void) {
    can_err_irq(CAN_4, IRQ_ERROR);
}

static void can0_overrun_irq(void) {
    can_err_irq(CAN_0, IRQ_OVERRUN);
}

static void can1_overrun_irq(void) {
    can_err_irq(CAN_1, IRQ_OVERRUN);
}

static void can2_overrun_irq(void) {
    can_err_irq(CAN_2, IRQ_OVERRUN);
}

static void can3_overrun_irq(void) {
    can_err_irq(CAN_3, IRQ_OVERRUN);
}

static void can4_overrun_irq(void) {
    can_err_irq(CAN_4, IRQ_OVERRUN);
}

static void can0_passive_irq(void) {
    can_err_irq(CAN_0, IRQ_PASSIVE);
}

static void can1_passive_irq(void) {
    can_err_irq(CAN_1, IRQ_PASSIVE);
}

static void can2_passive_irq(void) {
    can_err_irq(CAN_2, IRQ_PASSIVE);
}

static void can3_passive_irq(void) {
    can_err_irq(CAN_3, IRQ_PASSIVE);
}

static void can4_passive_irq(void) {
    can_err_irq(CAN_4, IRQ_PASSIVE);
}

static void can0_arb_lost_irq(void) {
    can_err_irq(CAN_0, IRQ_ARB);
}

static void can1_arb_lost_irq(void) {
    can_err_irq(CAN_1, IRQ_ARB);
}

static void can2_arb_lost_irq(void) {
    can_err_irq(CAN_2, IRQ_ARB);
}

static void can3_arb_lost_irq(void) {
    can_err_irq(CAN_3, IRQ_ARB);
}

static void can4_arb_lost_irq(void) {
    can_err_irq(CAN_4, IRQ_ARB);
}

static void can0_bus_err_irq(void) {
    can_err_irq(CAN_0, IRQ_BUS);
}

static void can1_bus_err_irq(void) {
    can_err_irq(CAN_1, IRQ_BUS);
}

static void can2_bus_err_irq(void) {
    can_err_irq(CAN_2, IRQ_BUS);
}

static void can3_bus_err_irq(void) {
    can_err_irq(CAN_3, IRQ_BUS);
}

static void can4_bus_err_irq(void) {
    can_err_irq(CAN_4, IRQ_BUS);
}

void can_init_freq(can_t *obj, PinName rd, PinName td, int hz) {
    __IO uint32_t *dmy_ctr;

    /* determine the CAN to use */
    uint32_t can_rx = pinmap_peripheral(rd, PinMap_CAN_RD);
    uint32_t can_tx = pinmap_peripheral(td, PinMap_CAN_TD);
    obj->ch = pinmap_merge(can_tx, can_rx);
    MBED_ASSERT((int)obj->ch != NC);

    /* enable CAN clock */
    CPGSTBCR3 &= ~(CPG_STBCR3_BIT_MSTP32);
    /* Has CAN RAM initialisation completed ? */
    while ((RSCAN0GSTS & 0x08) == 0x08) {
        __NOP();
    }
    /* clear Global Stop mode bit */
    RSCAN0GCTR &= 0xFFFFFFFB;
    /* clear Channel Stop mode bit */
    dmy_ctr = CTR_MATCH[obj->ch];
    *dmy_ctr &= 0xFFFFFFFB;
    /* Enter global reset mode */
    can_set_global_mode(GL_RESET);
    /* Enter channel reset mode */
    can_set_channel_mode(obj->ch, CH_RESET);
    /* reset register */
    can_reset_reg(obj);

    can_initialized[obj->ch] = 1;
    /* reconfigure channel which is already initialized */
    can_reconfigure_channel();
    
    /* pin out the can pins */
    pinmap_pinout(rd, PinMap_CAN_RD);
    pinmap_pinout(td, PinMap_CAN_TD);

    /* set can frequency */
    can_frequency(obj, hz);
}

void can_init(can_t *obj, PinName rd, PinName td) {
    can_init_freq(obj, rd, td, 100000);
}

void can_free(can_t *obj) {
    /* disable CAN clock */
    CPGSTBCR3 |= CPG_STBCR3_BIT_MSTP32;
}

int can_frequency(can_t *obj, int f) {
    __IO uint32_t *dmy_cfcc;
    int retval = 0;
    
    if (f <= 1000000) {
        /* less than 1Mhz */
        /* set Channel Reset mode */
        can_set_channel_mode(obj->ch, CH_RESET);
        can_set_frequency(obj, f);
        /* set Channel Communication mode */
        can_set_channel_mode(obj->ch, CH_COMM);
        /* restore  CFE bit since it is cleared */
        /* Use send/receive FIFO buffer */
        dmy_cfcc = CFCC_TBL[obj->ch][CAN_SEND];
        *dmy_cfcc |= 0x01;
        dmy_cfcc = CFCC_TBL[obj->ch][CAN_RECV];
        *dmy_cfcc |= 0x01;
        retval = 1;
    }

    return retval;
}

void can_reset(can_t *obj) {
    /* Enter global reset mode */
    can_set_global_mode(GL_RESET);
    /* Enter channel reset mode */
    can_set_channel_mode(obj->ch, CH_RESET);
    /* reset register */
    can_reset_reg(obj);
    /* reconfigure channel which is already initialized */
    can_reconfigure_channel();
}

int can_write(can_t *obj, CAN_Message msg, int cc) {
    __IO uint32_t *dmy_sts;
    __IO uint32_t *dmy_cfsts;
    __IO uint32_t *dmy_cfid;
    __IO uint32_t *dmy_cfptr;
    __IO uint32_t *dmy_cfdf0;
    __IO uint32_t *dmy_cfdf1;
    __IO uint32_t *dmy_cfpctr;
    int retval = 0;

    /* Wait to become channel communication mode */
    dmy_sts = STS_MATCH[obj->ch];
    while ((*dmy_sts & 0x07) != 0) {
        __NOP();
    }
    
    if (((msg.format == CANStandard) && (msg.id <= 0x07FF)) || ((msg.format == CANExtended) && (msg.id <= 0x1FFFFFFF))) {
        /* send/receive FIFO buffer isn't full */
        dmy_cfsts = CFSTS_TBL[obj->ch][CAN_SEND];
        if ((*dmy_cfsts & 0x02) != 0x02) {
            /* set format, frame type and send/receive FIFO buffer ID(b10-0 or b28-0) */
            dmy_cfid = CFID_TBL[obj->ch][CAN_SEND];
            *dmy_cfid = ((msg.format << 31) | (msg.type << 30));
            if (msg.format == CANStandard) {
                *dmy_cfid |= (msg.id & 0x07FF);
            } else {
                *dmy_cfid |= (msg.id & 0x1FFFFFFF);
            }
            /* set length */
            dmy_cfptr = CFPTR_TBL[obj->ch][CAN_SEND];
            *dmy_cfptr = msg.len << 28;
            /* set data */
            dmy_cfdf0 = CFDF0_TBL[obj->ch][CAN_SEND];
            memcpy((void *)dmy_cfdf0, &msg.data[0], 4);
            dmy_cfdf1 = CFDF1_TBL[obj->ch][CAN_SEND];
            memcpy((void *)dmy_cfdf1, &msg.data[4], 4);
            /* send request */
            dmy_cfpctr = CFPCTR_TBL[obj->ch][CAN_SEND];
            *dmy_cfpctr = 0xFF;
            retval = 1;
        }
    }
    
    return retval;
}

int can_read(can_t *obj, CAN_Message *msg, int handle) {
    __IO uint32_t *dmy_sts;
    __IO uint32_t *dmy_cfsts;
    __IO uint32_t *dmy_cfid;
    __IO uint32_t *dmy_cfptr;
    __IO uint32_t *dmy_cfdf0;
    __IO uint32_t *dmy_cfdf1;
    __IO uint32_t *dmy_cfpctr;
    int retval = 0;

    /* Wait to become channel communication mode */
    dmy_sts = STS_MATCH[obj->ch];
    while ((*dmy_sts & 0x07) != 0) {
        __NOP();
    }
    
    /* send/receive FIFO buffer isn't empty */
    dmy_cfsts = CFSTS_TBL[obj->ch][CAN_RECV];
    while ((*dmy_cfsts & 0x01) != 0x01) {
        /* get format, frame type and send/receive FIFO buffer ID(b10-0 or b28-0) */
        dmy_cfid = CFID_TBL[obj->ch][CAN_RECV];
        msg->format = (CANFormat)(*dmy_cfid >> 31);
        msg->type = (CANType)((*dmy_cfid >> 30) & 0x1);
        if (msg->format == CANStandard) {
            msg->id = (*dmy_cfid & 0x07FF);
        } else {
            msg->id = (*dmy_cfid & 0x1FFFFFFF);
        }
        /* get length */
        dmy_cfptr = CFPTR_TBL[obj->ch][CAN_RECV];
        msg->len = (unsigned char)(*dmy_cfptr >> 28);
        /* get data */
        dmy_cfdf0 = CFDF0_TBL[obj->ch][CAN_RECV];
        memcpy(&msg->data[0], (void *)dmy_cfdf0, 4);
        dmy_cfdf1 = CFDF1_TBL[obj->ch][CAN_RECV];
        memcpy(&msg->data[4], (void *)dmy_cfdf1, 4);
        /* receive(next data) request */
        dmy_cfpctr = CFPCTR_TBL[obj->ch][CAN_RECV];
        *dmy_cfpctr = 0xFF;
        retval = 1;
    }
    
    return retval;
}

unsigned char can_rderror(can_t *obj) {
    __IO uint32_t *dmy_sts;
    
    dmy_sts = STS_MATCH[obj->ch];
    return (unsigned char)((*dmy_sts >> 16) & 0xFF);
}

unsigned char can_tderror(can_t *obj) {
    __IO uint32_t *dmy_sts;
    
    dmy_sts = STS_MATCH[obj->ch];
    return (unsigned char)((*dmy_sts >> 24) & 0xFF);
}

int can_mode(can_t *obj, CanMode mode) {
    __IO uint32_t *dmy_ctr;
    __IO uint32_t *dmy_sts;
    __IO uint32_t *dmy_cfcc;
    int ch_cnt;
    can_t *tmp_obj;
    tmp_obj = obj;
    int retval = 1;
    
    switch (mode) {
        case MODE_RESET:
            can_set_global_mode(GL_RESET);
            can_set_channel_mode(obj->ch, CH_RESET);
            for (ch_cnt = 0; ch_cnt < CAN_NUM; ch_cnt++) {
                can_initialized[ch_cnt] = 0;
            }
            break;
        case MODE_NORMAL:
            can_set_global_mode(GL_OPE);
            can_set_channel_mode(obj->ch, CH_COMM);
            break;
        case MODE_SILENT:
            can_set_channel_mode(obj->ch, CH_HOLD);
            /* set listen only mode, enable communication test mode */
            dmy_ctr = CTR_MATCH[obj->ch];
            *dmy_ctr = ((*dmy_ctr & 0x00FFFFFF) | 0x03000000);
            can_set_channel_mode(obj->ch, CH_COMM);
            break;
        case MODE_TEST_LOCAL:
            can_set_channel_mode(obj->ch, CH_HOLD);
            /* set self test mode 0, enable communication test mode */
            dmy_ctr = CTR_MATCH[obj->ch];
            *dmy_ctr = ((*dmy_ctr & 0x00FFFFFF) | 0x05000000);
            can_set_channel_mode(obj->ch, CH_COMM);
            break;
        case MODE_TEST_GLOBAL:
            /* set the channel between the communication test on channel 1 and channel 2 */
            /* set Channel Hold mode */
            for (tmp_obj->ch = CAN_1; tmp_obj->ch <= CAN_2; tmp_obj->ch++) {
                dmy_sts = STS_MATCH[tmp_obj->ch];
                if ((*dmy_sts & 0x04) == 0x04) {
                    /* Channel Stop mode */
                    /* clear Channel Stop mode bit */
                    dmy_ctr = CTR_MATCH[tmp_obj->ch];
                    *dmy_ctr &= 0xFFFFFFFB;
                    can_set_channel_mode(tmp_obj->ch, CH_RESET);
                }
                can_set_channel_mode(tmp_obj->ch, CH_HOLD);
            }
            can_set_global_mode(GL_TEST);
            /* enable communication test between channel1 and channel2 */
            RSCAN0GTSTCFG = 0x06;
            RSCAN0GTSTCTR = 0x01;
            /* send and receive setting of channel1 and channel2 */
            for (tmp_obj->ch = CAN_1; tmp_obj->ch <= CAN_2; tmp_obj->ch++) {
                can_reset_buffer(tmp_obj);
                /* set global interrrupt */
                /* THLEIE, MEIE and DEIE interrupts are disable */
                RSCAN0GCTR &= 0xFFFFF8FF;
                /* BLIE, OLIE, BORIE and BOEIE interrupts are disable */
                /* TAIE, ALIE, EPIE, EWIE and BEIE interrupts are enable */
                dmy_ctr = CTR_MATCH[tmp_obj->ch];
                *dmy_ctr &= 0x00018700;
                can_set_global_mode(GL_OPE);
                can_set_channel_mode(tmp_obj->ch, CH_COMM);
                /* Use send/receive FIFO buffer */
                dmy_cfcc = CFCC_TBL[tmp_obj->ch][CAN_SEND];
                *dmy_cfcc |= 0x01;
                dmy_cfcc = CFCC_TBL[tmp_obj->ch][CAN_RECV];
                *dmy_cfcc |= 0x01;
            }
            break;
        case MODE_TEST_SILENT:
            /* not supported */
            /* fall through */
        default:
            retval = 0;
            break;
    }
    
    return retval;
}

int can_filter(can_t *obj, uint32_t id, uint32_t mask, CANFormat format, int32_t handle) {
    int retval = 0;
    
    if ((format == CANStandard) || (format == CANExtended)) {
        if (((format == CANStandard) && (id <= 0x07FF)) || ((format == CANExtended) && (id <= 0x1FFFFFFF))) {
            /* set Global Reset mode and Channel Reset mode */
            can_set_global_mode(GL_RESET);
            can_set_channel_mode(obj->ch, CH_RESET);
            /* enable receive rule table writing */
            RSCAN0GAFLECTR = 0x00000100;
            /* set the page number of receive rule table(page number = 0) */
            RSCAN0GAFLECTR |= (obj->ch * 4);
            /* set IDE format */
            *dmy_gaflid = (format << 31);
            if (format == CANExtended) {
                /* set receive rule ID for bit28-0 */
                *dmy_gaflid |= (id & 0x1FFFFFFF);
            } else {
                /* set receive rule ID for bit10-0 */
                *dmy_gaflid |= (id & 0x07FF);
            }
            /* set ID mask bit */
            *dmy_gaflm = (0xC0000000 | mask);
            /* disable receive rule table writing */
            RSCAN0GAFLECTR &= 0xFFFFFEFF;
            /* reconfigure channel which is already initialized */
            can_reconfigure_channel();
            retval = 1;
        }
    }
    
    return retval;
}

void can_monitor(can_t *obj, int silent) {
    __IO uint32_t *dmy_ctr;

    /* set Channel Hold mode */
    can_set_channel_mode(obj->ch, CH_HOLD);
    if (silent) {
        /* set listen only mode, enable communication test mode */
        dmy_ctr = CTR_MATCH[obj->ch];
        *dmy_ctr = ((*dmy_ctr & 0x00FFFFFF) | 0x03000000);
        can_set_channel_mode(obj->ch, CH_COMM);
    } else {
        /* set normal test mode, disable communication test mode */
        dmy_ctr = CTR_MATCH[obj->ch];
        *dmy_ctr &= 0x00FFFFFF;
        /* reset register */
        can_reset_reg(obj);
        /* reconfigure channel which is already initialized */
        can_reconfigure_channel();
    }
}

static void can_reset_reg(can_t *obj) {
    __IO uint32_t *dmy_ctr;

    /* time stamp source uses peripheral clock (pclk(P1_phi)/2), CAN clock uses clkc(P1_phi/2),           */
    /* mirror off, DLC not transfer, DLC check permit, transmit buffer priority, clock source not divided */
    RSCAN0GCFG = 0x00000003;
    /* set default frequency at 100k */
    can_set_frequency(obj, 100000);
    /* set receive rule */
    can_reset_recv_rule(obj);
    /* set buffer */
    can_reset_buffer(obj);
    /* set global interrrupt */
    /* THLEIE, MEIE and DEIE interrupts are disable */
    RSCAN0GCTR &= 0xFFFFF8FF;
    /* ALIE, BLIE, OLIE, BORIE, BOEIE, EPIE, EWIE and BEIE interrupts are disable */
    dmy_ctr = CTR_MATCH[obj->ch];
    *dmy_ctr &= 0xFFFF00FF;
}

static void can_reset_recv_rule(can_t *obj) {
    /* number of receive rules of each chanel = 64 */
    RSCAN0GAFLCFG0 = 0x40404040;
    RSCAN0GAFLCFG1 = 0x40000000;
    /* enable receive rule table writing */
    RSCAN0GAFLECTR = 0x00000100;
    /* set the page number of receive rule table(ex: id ch = 1, page number = 4) */
    RSCAN0GAFLECTR |= (obj->ch * 4);
    /* set standard ID, data frame and receive rule ID */
    *dmy_gaflid = 0x07FF;
    /* IDE bit, RTR bit and ID bit(28-0) are not compared */
    *dmy_gaflm = 0;
    /* DLC check is 1 bytes, not use a receive buffer */
    *dmy_gaflp0 = 0x10000000;
    /* use a send/receive FIFO buffer(ex: if ch = 1, FIFO buffer number = 4 and bit = 12) */
    *dmy_gaflp1 = (1 << ((obj->ch + 3) * 3));
    /* disable receive rule table writing */
    RSCAN0GAFLECTR &= 0xFFFFFEFF;
}

static void can_reset_buffer(can_t *obj) {
    __IO uint32_t *dmy_rfcc;
    __IO uint32_t *dmy_cfcc;
    __IO uint32_t *dmy_txqcc;
    __IO uint32_t *dmy_thlcc;
    int cnt;
    
    /* set linked send buffer number(ex: if ch = 1 and mode = send, buffer number = 16), interval timer is pclk/2 */
    /* number of rows of send/receive FIFO buffer = 4 */
    dmy_cfcc = CFCC_TBL[obj->ch][CAN_SEND];
    *dmy_cfcc = 0x00011100;                 /* send/receive FIFO mode is send */
    dmy_cfcc = CFCC_TBL[obj->ch][CAN_RECV];
    *dmy_cfcc = 0x00001100;                 /* send/receive FIFO mode is receive */
    /* receive buffer is not used */
    RSCAN0RMNB = 0;
    /* receive FIFO buffer is not used */
    for (cnt = 0; cnt < 8; cnt++) {
        dmy_rfcc = RFCC_MATCH[cnt];
        *dmy_rfcc = 0;
    }
    /* send queue is not used */
    dmy_txqcc = TXQCC_MATCH[obj->ch];
    *dmy_txqcc = 0;
    /* send history is not used */
    dmy_thlcc = THLCC_MATCH[obj->ch];
    *dmy_thlcc = 0;

    /* CFTXIE and CFRXIE interrupts are enable */
    dmy_cfcc = CFCC_TBL[obj->ch][CAN_SEND];
    *dmy_cfcc |= 0x04;
    dmy_cfcc = CFCC_TBL[obj->ch][CAN_RECV];
    *dmy_cfcc |= 0x02;
    /* TMIEp interrupt is disable */
    RSCAN0TMIEC0 = 0x00000000;
    RSCAN0TMIEC1 = 0x00000000;
    RSCAN0TMIEC2 = 0x00000000;
}

static void can_reconfigure_channel(void) {
    __IO uint32_t *dmy_cfcc;
    int ch_cnt;

    for (ch_cnt = 0; ch_cnt < CAN_NUM; ch_cnt++) {
        if (can_initialized[ch_cnt] == 1) {
            /* set Global Operation mode and Channel Communication mode */
            can_set_global_mode(GL_OPE);
            can_set_channel_mode(ch_cnt, CH_COMM);
            /* Use send/receive FIFO buffer */
            dmy_cfcc = CFCC_TBL[ch_cnt][CAN_SEND];
            *dmy_cfcc |= 0x01;
            dmy_cfcc = CFCC_TBL[ch_cnt][CAN_RECV];
            *dmy_cfcc |= 0x01;
        }
    }
}

static void can_set_frequency(can_t *obj, int f) {
    __IO uint32_t *dmy_cfg;
    int oldfreq = 0;
    int newfreq = 0;
    uint32_t  clkc_val;
    uint8_t tmp_tq;
    uint8_t tq = 0;
    uint8_t tmp_brp;
    uint8_t brp = 0;
    uint8_t tseg1 = 0;
    uint8_t tseg2 = 0;
    uint8_t sjw = 0;
    
    /* set clkc */
    if (RZ_A1_IsClockMode0() == false) {
        clkc_val = CM1_RENESAS_RZ_A1_P1_CLK / 2;
    } else {
        clkc_val = CM0_RENESAS_RZ_A1_P1_CLK / 2;
    }
    /* calculate BRP bit and Choose max value of calculated frequency */
    for (tmp_tq = 8; tmp_tq <= 25; tmp_tq++) {
        /* f = fCAN / ((BRP+1) * Tq) */
        /* BRP = (fCAN / (f * Tq)) - 1 */
        tmp_brp = ((clkc_val / (f * tmp_tq)) - 1) + 1;   // carry(decimal point is carry)
        newfreq = clkc_val / ((tmp_brp + 1) * tmp_tq);
        if (newfreq >= oldfreq) {
            oldfreq  = newfreq;
            tq       = tmp_tq;
            brp      = tmp_brp;
        }
    }
    /* calculate TSEG1 bit and TSEG2 bit */
    tseg1 = (tq - 1) * 0.666666667;
    tseg2 = (tq - 1) - tseg1;
    sjw = (tseg2 > 4)? 4 : tseg2;
    /* set RSCAN0CmCFG register */
    dmy_cfg = CFG_MATCH[obj->ch];
    *dmy_cfg = ((sjw - 1) << 24) | ((tseg2 - 1) << 20) | ((tseg1 - 1) << 16) | brp;
}

static void can_set_global_mode(int mode) {
    /* set Global mode */
    RSCAN0GCTR = ((RSCAN0GCTR & 0xFFFFFFFC) | mode);
    /* Wait to cahnge into Global XXXX mode */
    while ((RSCAN0GSTS & 0x07) != mode) {
        __NOP();
    }
}

static void can_set_channel_mode(uint32_t ch, int mode) {
    __IO uint32_t *dmy_ctr;
    __IO uint32_t *dmy_sts;

    /* set Channel mode */
    dmy_ctr = CTR_MATCH[ch];
    *dmy_ctr = ((*dmy_ctr & 0xFFFFFFFC) | mode);
    /* Wait to cahnge into Channel XXXX mode */
    dmy_sts = STS_MATCH[ch];
    while ((*dmy_sts & 0x07) != mode) {
        __NOP();
    }
}

