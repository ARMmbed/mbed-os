/*
 * Copyright (c) 2023, Nuvoton Technology Corporation
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

#if DEVICE_CAN

#include "cmsis.h"
#include "mbed_error.h"
#include "mbed_assert.h"
#include "mbed_toolchain.h"
#include "PeripheralPins.h"
#include "gpio_api.h"
#include "nu_modutil.h"
#include "nu_bitutil.h"
#include <string.h>
#include <stdbool.h>
#include <assert.h>

/* Notes of implementation
 *
 * 1. Each CANFD instance supports two IRQ lines. Use only line 0. Line 1 is not used.
 * 2. For Rx disabling multiple filter handles,
 *    (1) Map all filter handles to filter handle 0
 *    (2) Use Rx FIFO 0 for filter handle 0
 *    (3) Support mask
 * 3. For Rx enabling multiple filter handles,
 *    (1) Map filter handles to SID/XID filter elements directly: 0 to 0/0, 1 to 1/1, etc. 
 *    (2) Use Rx FIFO 0 for filter handle 0
 *    (3) Use Rx FIFO 1 for filter handle through first invoking can_filter()
 *    (4) Use dedicated Rx Buffer for other filter handles
 *        The front half for SID filter element and back half for XID filter element
 *    (5) Support mask only for filter handle 0 and the one through first invoking can_filter()
 *        H/W supports mask on Rx FIFO 0/1 but not on dedicated Rx Buffer.
 * 4. Continuing above, the thread below discusses on CAN filter more precisely:
 *    https://os.mbed.com/questions/85183/How-to-use-CAN-filter-function/
 *    It has some points to note:
 *    (1) No message will get accepted without filter configured.
 *    (2) Received message is compared following filter handle 0, 1, 2, 3.
 *        If no match, the message is discarded.
 *        On match, the message can be fetched through can_read() with the match filter.
 *    (3) It is required filter handle 0 be configured in the constructor and accept any message by default.
 *        If not reconfigured, no message will reach filter handle other than 0.
 * 5. For Tx, use only dedicated Tx Buffer. BSP CANFD driver doesn't support Tx FIFO/Queue.
 * 6. Support no CAN FD.
 * 7. CAN HAL doesn't define modes clearly. Following other chip porting, map them to H/W as below:
 *    MODE_NORMAL --> Normal operation
 *    MODE_SILENT --> Bus Monitor mode
 *    MODE_TEST_GLOBAL/LOCAL --> Test/External Loop Back mode
 *    MODE_TEST_SILENT --> Test/Internal Loop Back mode
 */

/* Enable or not multiple filter handles
 *
 * Reasons for disabling the feature:
 * (1) Per-handle implementation doesn't support mask on all filter handles.
 *     SID/XID filter elements directing to Rx FIFO 0/1 can support mask.
 *     SID/XID filter elements directing to dedicated Rx Buffer cannot support mask. 
 * (2) Mbed OS CAN HAL API allows ignoring 'handle' parameter.
 * (3) The default filter handle 0 will accept all messages,
 *     so by default, all messages cannot reach other user defined ones,
 *     unless filter handle 0 reconfigured.
 *     However, most samples are unaware of this.
 */
#define NU_CAN_EN_MULT_HNDL         0

/* Max number of message ID filter handle */
#define NU_CAN_MAXNUM_HNDL          8

/* Max number of Standard message ID filter elements configured */
#define NU_CAN_MAXNUM_SIDFLTR       NU_CAN_MAXNUM_HNDL

/* Max number of Extended message ID filter elements configured */
#define NU_CAN_MAXNUM_XIDFLTR       NU_CAN_MAXNUM_HNDL

/* Max number of dedicated Rx Buffer elements configured */
#define NU_CAN_MAXNUM_RXBUF         (NU_CAN_MAXNUM_SIDFLTR + NU_CAN_MAXNUM_XIDFLTR)

/* Configured number of SID filter elements must be less than H/W limit */
static_assert(NU_CAN_MAXNUM_SIDFLTR <= CANFD_MAX_11_BIT_FTR_ELEMS,
              "Configured number of SID filter elements must be less than H/W limit");

/* Configured number of SID filter elements must be equal to filter handles */
static_assert(NU_CAN_MAXNUM_SIDFLTR == NU_CAN_MAXNUM_HNDL,
              "Configured number of SID filter elements must be equal to filter handles");
              
/* Configured number of XID filter elements must be less than H/W limit */
static_assert(NU_CAN_MAXNUM_XIDFLTR <= CANFD_MAX_29_BIT_FTR_ELEMS,
              "Configured number of XID filter elements must be less than H/W limit");

/* Configured number of XID filter elements must be equal to filter handles */
static_assert(NU_CAN_MAXNUM_XIDFLTR == NU_CAN_MAXNUM_HNDL,
              "Configured number of XID filter elements must be equal to filter handles");

/* Configured number of dedicated Rx Buffer elements must be less than H/W limit */
static_assert(NU_CAN_MAXNUM_RXBUF <= CANFD_MAX_RX_BUF_ELEMS,
              "Configured number of dedicated Rx Buffer elements must be less than H/W limit");

/* Convert to string literal */
#define NU_STR_(X)  #X
#define NU_STR(X)   NU_STR_(X)

/* Rx buffer type for filter */
enum {
    NU_CAN_RXBUF_TYPE_NONE = 0,
    NU_CAN_RXBUF_TYPE_FIFO_0,
    NU_CAN_RXBUF_TYPE_FIFO_1,
    NU_CAN_RXBUF_TYPE_DEDICATED,
};

struct nu_can_filter {
    uint32_t    id;
    uint32_t    mask;
    CANFormat   format;
    int32_t     handle;
    uint32_t    rxbuf_type;
};

struct nu_can_var {
    CANFD_FD_T                      canfd_config;
    bool                            rx_fifo_0_used;
    bool                            rx_fifo_1_used;
    struct nu_can_filter            filters[NU_CAN_MAXNUM_HNDL];
    CANFD_FD_MSG_T                  msg_staging;
    can_irq_handler                 irq_handler;
    uintptr_t                       irq_context;

    /* Mark the following area is reserved for not being cleared */
    uint32_t                        reserved;

    /* Following fields are static-initialized */
    void                            (*vec)(void);
    IRQn_Type                       irq_line1;
};

/* IRQ handler for CAN IRQ line 0. CAN IRQ line 1 is not used. */
void CANFD00_IRQHandler(void);
void CANFD10_IRQHandler(void);
void CANFD20_IRQHandler(void);
void CANFD30_IRQHandler(void);

static void can_reconfig(can_t *obj, CANFD_FD_T *canfd_config);
static void can_irq(CANName can);
static void can_filters_reconfig(can_t *obj);
static int can_filter_bind_rxbuf_type(can_t *obj, struct nu_can_filter *filter);
static int can_filter_config(can_t *obj, const struct nu_can_filter *filter);

/* Get SID/XID filter element index by filter handle
 *
 * One filter handle maps to one SID filter and one XID filter.
 * These three are numbered the same.
 */
static inline uint32_t can_filter_sidfltr_index(const struct nu_can_filter *filter)
{
    return ((uint32_t) filter->handle);
}
static inline uint32_t can_filter_xidfltr_index(const struct nu_can_filter *filter)
{
    return ((uint32_t) filter->handle);
}

/* Get dedicated Rx Buffer element index for SID/XID filter element by filter handle
 *
 * Front half of dedicated Rx Buffer elements for SID filter, back half for XID filter.
 */
static inline uint32_t can_filter_sidfltr_rxbuf_index(const struct nu_can_filter *filter)
{
    return ((uint32_t) filter->handle);
}
static inline uint32_t can_filter_xidfltr_rxbuf_index(const struct nu_can_filter *filter)
{
    return (((uint32_t) filter->handle) + NU_CAN_MAXNUM_SIDFLTR);
}

/* Is message matched? */
static inline bool can_filter_message_matched(const struct nu_can_filter *filter, const CANFD_FD_MSG_T *msg_staging)
{
    if (filter->format == CANStandard && msg_staging->eIdType != eCANFD_SID) {
        return false;
    }

    if (filter->format == CANExtended && msg_staging->eIdType != eCANFD_XID) {
        return false;
    }

    if ((filter->id & filter->mask) != (msg_staging->u32Id & filter->mask)) {
        return false;
    }

    return true;
}

static struct nu_can_var can0_var = {
    .vec            = CANFD00_IRQHandler,
    .irq_line1      = CANFD01_IRQn,
};
static struct nu_can_var can1_var = {
    .vec            = CANFD10_IRQHandler,
    .irq_line1      = CANFD11_IRQn,
};
static struct nu_can_var can2_var = {
    .vec            = CANFD20_IRQHandler,
    .irq_line1      = CANFD21_IRQn,
};
static struct nu_can_var can3_var = {
    .vec            = CANFD30_IRQHandler,
    .irq_line1      = CANFD31_IRQn,
};

static const struct nu_modinit_s can_modinit_tab[] = {
    {CAN_0, CANFD0_MODULE, CLK_CLKSEL0_CANFD0SEL_HCLK, CLK_CLKDIV5_CANFD0(1), CANFD0_RST, CANFD00_IRQn, &can0_var},
    {CAN_1, CANFD1_MODULE, CLK_CLKSEL0_CANFD1SEL_HCLK, CLK_CLKDIV5_CANFD1(1), CANFD1_RST, CANFD10_IRQn, &can1_var},
    {CAN_2, CANFD2_MODULE, CLK_CLKSEL0_CANFD2SEL_HCLK, CLK_CLKDIV5_CANFD2(1), CANFD2_RST, CANFD20_IRQn, &can2_var},
    {CAN_3, CANFD3_MODULE, CLK_CLKSEL0_CANFD3SEL_HCLK, CLK_CLKDIV5_CANFD3(1), CANFD3_RST, CANFD30_IRQn, &can3_var},

    {NC, 0, 0, 0, 0, (IRQn_Type) 0, NULL}
};

void can_init_freq(can_t *obj, PinName rd, PinName td, int hz)
{
    uint32_t can_rd = (CANName)pinmap_peripheral(rd, PinMap_CAN_RD);
    uint32_t can_td = (CANName)pinmap_peripheral(td, PinMap_CAN_TD);
    obj->can = (CANName)pinmap_merge(can_rd, can_td);
    MBED_ASSERT((int)obj->can != NC);

    const struct nu_modinit_s *modinit = get_modinit(obj->can, can_modinit_tab);
    MBED_ASSERT(modinit != NULL);
    MBED_ASSERT(modinit->modname == (int) obj->can);

    struct nu_can_var *var = (struct nu_can_var *) modinit->var;

    /* Clear var to zero except reserved area */
    memset(var, 0x00, offsetof(struct nu_can_var, reserved));

    obj->pin_rd = rd;
    obj->pin_td = td;

    pinmap_pinout(rd, PinMap_CAN_RD);
    pinmap_pinout(td, PinMap_CAN_TD);

    // Enable IP clock
    CLK_EnableModuleClock(modinit->clkidx);

    // Reset IP
    SYS_ResetModule(modinit->rsetidx);

    CANFD_FD_T *canfd_config = &var->canfd_config;

    /* Based on BSP CAN driver default configuration, no CAN FD */
    CANFD_GetDefaultConfig(canfd_config, CANFD_OP_CAN_MODE);

    /* Change default configuration here */
    {
        /* Change normal bit rate to specified. CAN FD is not supported,
         * so data bit rate will be the same as above. */
        if (hz > 0) {
            canfd_config->sBtConfig.sNormBitRate.u32BitRate = hz;
        }

        /* Change max number of SID filter elements */
        canfd_config->sElemSize.u32SIDFC = NU_CAN_MAXNUM_SIDFLTR;

        /* Change max number of SID filter elements */
        canfd_config->sElemSize.u32XIDFC = NU_CAN_MAXNUM_XIDFLTR;

        /* Change max number of dedicated Rx Buffer elements */
        canfd_config->sElemSize.u32RxBuf = NU_CAN_MAXNUM_RXBUF;

        can_reconfig(obj, canfd_config);
    }

    /* As required, filter handle 0 defaults to accept-any */
    struct nu_can_filter *filter_0 = &var->filters[0];
    filter_0->id            = 0;
    filter_0->mask          = 0;
    filter_0->format        = CANAny;
    filter_0->handle        = 0;

    /* Bind filter handle 0 to Rx Buffer type */
    can_filter_bind_rxbuf_type(obj, filter_0);
    MBED_ASSERT(filter_0->rxbuf_type != NU_CAN_RXBUF_TYPE_NONE);

    /* Configure filter handle 0 */
    can_filter_config(obj, filter_0);
}

void can_init(can_t *obj, PinName rd, PinName td)
{
    can_init_freq(obj, rd, td, -1);
}

void can_free(can_t *obj)
{
    const struct nu_modinit_s *modinit = get_modinit(obj->can, can_modinit_tab);
    MBED_ASSERT(modinit != NULL);
    MBED_ASSERT(modinit->modname == (int) obj->can);

    CANFD_T *can_base = (CANFD_T *) NU_MODBASE(obj->can);

    CANFD_Close(can_base);

    // Reset this module
    SYS_ResetModule(modinit->rsetidx);

    // Disable interrupts
    CANFD_DisableInt(can_base,
                     0xFFFFFFFF,    // interrupt line 0
                     0,             // interrupt line 1 unused
                     0xFFFFFFFF,    // Tx Buffer Transmission 0-31 Interrupts
                     0xFFFFFFFF);   // Tx Buffer Cancellation Finished 0-31 Interrupts
    NVIC_DisableIRQ(modinit->irq_n);

    // Disable IP clock
    CLK_DisableModuleClock(modinit->clkidx);

    /* Free up pins */
    gpio_set(obj->pin_rd);
    gpio_set(obj->pin_td);
    obj->pin_rd = NC;
    obj->pin_td = NC;
}

int can_frequency(can_t *obj, int hz)
{
    const struct nu_modinit_s *modinit = get_modinit(obj->can, can_modinit_tab);
    MBED_ASSERT(modinit != NULL);
    MBED_ASSERT(modinit->modname == (int) obj->can);

    struct nu_can_var *var = (struct nu_can_var *) modinit->var;

    /* Based on previous configuration */
    CANFD_FD_T *canfd_config = &var->canfd_config;

    canfd_config->sBtConfig.sNormBitRate.u32BitRate = hz;

    can_reconfig(obj, canfd_config);

    /* With BSP CAN FD driver, all filters configured before will get cleared by above call.
     * Reconfigure them back. */
    can_filters_reconfig(obj);

    /* 1 on success, or 0 on failure */
    return 1;
}

void CANFD00_IRQHandler(void)
{
    can_irq(CAN_0);
}

void CANFD10_IRQHandler(void)
{
    can_irq(CAN_1);
}

void CANFD20_IRQHandler(void)
{
    can_irq(CAN_2);
}

void CANFD30_IRQHandler(void)
{
    can_irq(CAN_3);
}

void can_irq_init(can_t *obj, can_irq_handler handler, uintptr_t context)
{
    const struct nu_modinit_s *modinit = get_modinit(obj->can, can_modinit_tab);
    MBED_ASSERT(modinit != NULL);
    MBED_ASSERT(modinit->modname == (int) obj->can);

    struct nu_can_var *var = (struct nu_can_var *) modinit->var;

    var->irq_handler = handler;
    var->irq_context = context;
}

void can_irq_free(can_t *obj)
{
    const struct nu_modinit_s *modinit = get_modinit(obj->can, can_modinit_tab);
    MBED_ASSERT(modinit != NULL);
    MBED_ASSERT(modinit->modname == (int) obj->can);

    struct nu_can_var *var = (struct nu_can_var *) modinit->var;

    var->irq_handler = NULL;
    var->irq_context = 0;
}

void can_irq_set(can_t *obj, CanIrqType irq, uint32_t enable)
{
    if (enable) {
        const struct nu_modinit_s *modinit = get_modinit(obj->can, can_modinit_tab);
        MBED_ASSERT(modinit != NULL);
        MBED_ASSERT(modinit->modname == (int) obj->can);

        struct nu_can_var *var = (struct nu_can_var *) modinit->var;

        NVIC_SetVector(modinit->irq_n, (uint32_t) var->vec);
        NVIC_EnableIRQ(modinit->irq_n);
    }

    /* We use only interrupt line 0. */
    uint32_t line0_interrupts = 0;
    uint32_t line1_interrupts = 0;

    /* Interrupt mapping from HAL CAN to MCU CANFD */
    switch (irq) {
        case IRQ_RX:
            line0_interrupts = CANFD_IE_RF0NE_Msk | CANFD_IE_RF1NE_Msk | CANFD_IE_DRXE_Msk;
            break;

        case IRQ_TX:
            line0_interrupts = CANFD_IE_TCE_Msk;
            break;

        case IRQ_ERROR:
            line0_interrupts = CANFD_IE_EWE_Msk;
            break;

        case IRQ_OVERRUN:
            break;

        case IRQ_WAKEUP:
            break;

        case IRQ_PASSIVE:
            line0_interrupts = CANFD_IE_EPE_Msk;
            break;

        case IRQ_ARB:
            break;

        case IRQ_BUS:
            line0_interrupts = CANFD_IE_BOE_Msk;
            break;

        case IRQ_READY:
            break;

        default:
            break;
    }

    CANFD_T *can_base = (CANFD_T *) NU_MODBASE(obj->can);

    uint32_t ie = CANFD_ReadReg(&can_base->IE);

    /* Tx Buffer Transmission/Cancellation Finished Interrupt Enable
     *
     * Each Tx Buffer has its own Transmission/Cancellation Finished Interrupt Enable bit.
     * Dependent on their overall switch IE.TCE/IE.TCFE, these bits are set altogether or not.
     */
    uint32_t txbtie = (CANFD_IE_TCE_Msk & (ie | line0_interrupts)) ? 0xFFFFFFFF : 0;
    uint32_t txbcie = (CANFD_IE_TCFE_Msk & (ie | line0_interrupts)) ? 0xFFFFFFFF : 0;

    if (enable) {
        CANFD_EnableInt(can_base, line0_interrupts, line1_interrupts, txbtie, txbcie);
    } else {
        CANFD_DisableInt(can_base, line0_interrupts, line1_interrupts, txbtie, txbcie);
    }
}

int can_write(can_t *obj, CAN_Message msg, int cc)
{
    /* Unused */
    (void) cc;

    const struct nu_modinit_s *modinit = get_modinit(obj->can, can_modinit_tab);
    MBED_ASSERT(modinit != NULL);
    MBED_ASSERT(modinit->modname == (int) obj->can);

    struct nu_can_var *var = (struct nu_can_var *) modinit->var;

    CANFD_T *can_base = (CANFD_T *) NU_MODBASE(obj->can);

    /* Populate the message */
    CANFD_FD_MSG_T *msg_staging = &var->msg_staging;
    memset(msg_staging, 0x00, sizeof(CANFD_FD_MSG_T));

    /* Message ID */
    msg_staging->u32Id = msg.id;
    /* CANFD_FD_MSG_T.u32DLC, not suitable naming, is data length in bytes, not data length code. */
    msg_staging->u32DLC = (msg.len <= CANFD_MAX_MESSAGE_BYTES) ? msg.len : CANFD_MAX_MESSAGE_BYTES;
    memcpy(msg_staging->au8Data, msg.data, msg_staging->u32DLC);
    /* Standard/extended message */
    msg_staging->eIdType = (msg.format == CANStandard) ? eCANFD_SID : eCANFD_XID;
    /* Data/remote message */
    msg_staging->eFrmType = (msg.type == CANData) ? eCANFD_DATA_FRM : eCANFD_REMOTE_FRM;
    /* No FD format */
    msg_staging->bFDFormat = 0;
    /* No bit rate switch */
    msg_staging->bBitRateSwitch = 0;

    int success;

    /* BSP CAN driver supports Tx Dedicated Buffer */
    uint32_t i;
    for (i = 0; i < var->canfd_config.sElemSize.u32TxBuf; i ++) {
        success = CANFD_TransmitTxMsg(can_base, i, msg_staging);
        if (success) {
            return 1;
        }
    }

    /* BSP CAN driver supports no Tx FIFO/Queue */

    return 0;
}

int can_read(can_t *obj, CAN_Message *msg, int handle)
{
#if NU_CAN_EN_MULT_HNDL
    /* Check validity of filter handle */
    if (handle < 0 || handle >= NU_CAN_MAXNUM_HNDL) {
        error("Support max " NU_STR(NU_CAN_MAXNUM_HNDL) " CAN filters");
        return 0;
    }
#else
    /* Single filter handle 0 */
    int32_t handle_orig = handle;
    (void) handle_orig;
    handle = 0;
#endif

    const struct nu_modinit_s *modinit = get_modinit(obj->can, can_modinit_tab);
    MBED_ASSERT(modinit != NULL);
    MBED_ASSERT(modinit->modname == (int) obj->can);

    struct nu_can_var *var = (struct nu_can_var *) modinit->var;
    const struct nu_can_filter *filter = &var->filters[handle];
    CANFD_FD_MSG_T *msg_staging = &var->msg_staging;

    CANFD_T *can_base = (CANFD_T *) NU_MODBASE(obj->can);

    if (filter->rxbuf_type == NU_CAN_RXBUF_TYPE_FIFO_0 ||
        filter->rxbuf_type == NU_CAN_RXBUF_TYPE_FIFO_1) {
        uint32_t fifo_idx = (filter->rxbuf_type == NU_CAN_RXBUF_TYPE_FIFO_0) ? 0: 1;
        while (1) {
            if (!CANFD_ReadRxFifoMsg(can_base, fifo_idx, msg_staging)) {
                /* Rx FIFO 0/1 empty */
                goto message_no_accepted;
            }

            if (!can_filter_message_matched(filter, msg_staging)) {
                /* Not matched, go next one */
                continue;
            }

            /* Find one matched */
            goto message_matched;
        }
    } else {
        /* Get dedicated Rx Buffer element index for SID/XID filter element by filter handle */
        uint32_t sidfltr_rxdbf_idx = can_filter_sidfltr_rxbuf_index(filter);
        uint32_t xidfltr_rxdbf_idx = can_filter_xidfltr_rxbuf_index(filter);

        /* Receive from dedicated Rx Buffer for SID filter element */
        if (filter->format == CANStandard || filter->format == CANAny) {
            if (!CANFD_ReadRxBufMsg(can_base, sidfltr_rxdbf_idx, msg_staging)) {
                /* Dedicated Rx Buffer empty */
                goto check_xid_filter;
            }

            if (!can_filter_message_matched(filter, msg_staging)) {
                /* Not matched */
                goto check_xid_filter;
            }

            /* Find one matched */
            goto message_matched;
        }

check_xid_filter:

        /* Receive from dedicated Rx Buffer for XID filter element */
        if (filter->format == CANExtended || filter->format == CANAny) {
            if (!CANFD_ReadRxBufMsg(can_base, xidfltr_rxdbf_idx, msg_staging)) {
                /* Dedicated Rx Buffer empty */
                goto message_no_accepted;
            }

            if (!can_filter_message_matched(filter, msg_staging)) {
                /* Not matched */
                goto message_no_matched;
            }

            /* Find one matched */
            goto message_matched;
        }
    }

message_no_accepted:
message_no_matched:

    return 0;

message_matched:

    /* ID match, populate the message */
    memset(msg, 0x00, sizeof(CAN_Message));

    /* Message ID */
    msg->id = msg_staging->u32Id;
    /* CANFD_FD_MSG_T.u32DLC, not suitable naming, is data length in bytes, not data length code. */
    msg->len = (msg_staging->u32DLC <= sizeof(msg->data)) ? msg_staging->u32DLC : sizeof(msg->data);
    memcpy(&msg->data[0], &msg_staging->au8Data[0], msg->len);
    /* Standard/Extended message */
    msg->format = (msg_staging->eIdType == eCANFD_SID) ? CANStandard : CANExtended;
    /* Data/Remote message */
    msg->type = (msg_staging->eFrmType == eCANFD_DATA_FRM) ? CANData : CANRemote;

    return 1;
}

int can_mode(can_t *obj, CanMode mode)
{
    bool monitor_enabled;
    bool test_enabled;
    bool loopback_enabled;

    switch (mode) {
        case MODE_RESET:
            MBED_FALLTHROUGH;
        case MODE_NORMAL:
            monitor_enabled = false;
            test_enabled = false;
            loopback_enabled = false;
            break;

        case MODE_SILENT:
            monitor_enabled = true;
            test_enabled = false;
            loopback_enabled = false;
            break;

        case MODE_TEST_GLOBAL:
            MBED_FALLTHROUGH;
        case MODE_TEST_LOCAL:
            monitor_enabled = false;
            test_enabled = true;
            loopback_enabled = true;
            break;

        case MODE_TEST_SILENT:
            monitor_enabled = true;
            test_enabled = true;
            loopback_enabled = true;
            break;

        default:
            /* 1 if success, or 0 if failure */
            return 0;
    }

    CANFD_T *can_base = (CANFD_T *) NU_MODBASE(obj->can);
    
    /* Enter INIT mode for configuration */
    CANFD_RunToNormal(can_base, false);

    /* Enable write-protect configuration change */
    can_base->CCCR = CANFD_ReadReg(&can_base->CCCR) | CANFD_CCCR_CCE_Msk;

    /* Enable monitor or not */
    if (monitor_enabled) {
        can_base->CCCR = CANFD_ReadReg(&can_base->CCCR) | CANFD_CCCR_MON_Msk;
    } else {
        can_base->CCCR = CANFD_ReadReg(&can_base->CCCR) & ~CANFD_CCCR_MON_Msk;
    }

    /* Enable Test mode or not */
    if (test_enabled) {
        can_base->CCCR = CANFD_ReadReg(&can_base->CCCR) | CANFD_CCCR_TEST_Msk;
    } else {
        can_base->CCCR = CANFD_ReadReg(&can_base->CCCR) & ~CANFD_CCCR_TEST_Msk;
    }

    /* Enable loopback or not */
    if (loopback_enabled) {
        can_base->TEST = CANFD_ReadReg(&can_base->TEST) | CANFD_TEST_LBCK_Msk;
    } else {
        can_base->TEST = CANFD_ReadReg(&can_base->TEST) & ~CANFD_TEST_LBCK_Msk;
    }

    /* Leave INIT mode for normal operation */
    CANFD_RunToNormal(can_base, true);

    /* 1 if success, or 0 if failure */
    return 1;
}

int can_filter(can_t *obj, uint32_t id, uint32_t mask, CANFormat format, int32_t handle)
{
#if NU_CAN_EN_MULT_HNDL
    /* Check validity of filter handle */
    if (handle < 0 || handle >= NU_CAN_MAXNUM_HNDL) {
        error("Support max " NU_STR(NU_CAN_MAXNUM_HNDL) " CAN filters");
        return 0;
    }
#else
    /* Single filter handle 0 */
    int32_t handle_orig = handle;
    (void) handle_orig;
    handle = 0;
#endif

    const struct nu_modinit_s *modinit = get_modinit(obj->can, can_modinit_tab);
    MBED_ASSERT(modinit != NULL);
    MBED_ASSERT(modinit->modname == (int) obj->can);

    struct nu_can_var *var = (struct nu_can_var *) modinit->var;
    struct nu_can_filter *filter = &var->filters[handle];

    /* Keep user-defined filter configuration */
    filter->id = id;
    filter->mask = mask;
    filter->format = format;
    filter->handle = handle;

    /* Bind filter to Rx Buffer type */
    can_filter_bind_rxbuf_type(obj, filter);
    MBED_ASSERT(filter->rxbuf_type != NU_CAN_RXBUF_TYPE_NONE);

    if (can_filter_config(obj, filter)) {
#if NU_CAN_EN_MULT_HNDL
        return handle;
#else
        /* NOTE: 0 is ambiguous, error or filter handle 0. */
        return handle_orig;
#endif
    } else {
        return 0;
    }
}

void can_reset(can_t *obj)
{
    can_mode(obj, MODE_RESET);
}

unsigned char can_rderror(can_t *obj)
{
    CANFD_T *can_base = (CANFD_T *) NU_MODBASE(obj->can);

    uint32_t ecr = CANFD_ReadReg(&can_base->ECR);
    return (uint8_t) ((ecr >> CANFD_ECR_REC_Pos) & CANFD_ECR_REC_Msk);
}

unsigned char can_tderror(can_t *obj)
{
    CANFD_T *can_base = (CANFD_T *) NU_MODBASE(obj->can);

    uint32_t ecr = CANFD_ReadReg(&can_base->ECR);
    return (uint8_t) ((ecr >> CANFD_ECR_TEC_Pos) & CANFD_ECR_TEC_Msk);
}

void can_monitor(can_t *obj, int silent)
{
    CANFD_T *can_base = (CANFD_T *) NU_MODBASE(obj->can);

    bool test_enabled = CANFD_ReadReg(&can_base->CCCR) & CANFD_CCCR_TEST_Msk;

    CanMode mode;

    if (silent) {
        if (test_enabled) {
            mode = MODE_TEST_SILENT;
        } else {
            mode = MODE_SILENT;
        }
    } else {
        if (test_enabled) {
            mode = MODE_TEST_GLOBAL;
        } else {
            mode = MODE_NORMAL;
        }
    }

    can_mode(obj, mode);
}

const PinMap *can_rd_pinmap()
{
    return PinMap_CAN_TD;
}

const PinMap *can_td_pinmap()
{
    return PinMap_CAN_RD;
}

void can_reconfig(can_t *obj, CANFD_FD_T *canfd_config)
{
    const struct nu_modinit_s *modinit = get_modinit(obj->can, can_modinit_tab);
    MBED_ASSERT(modinit != NULL);
    MBED_ASSERT(modinit->modname == (int) obj->can);

    struct nu_can_var *var = (struct nu_can_var *) modinit->var;

    CANFD_T *can_base = (CANFD_T *) NU_MODBASE(obj->can);

    /* Enter INIT mode for configuration */
    CANFD_RunToNormal(can_base, false);

    /* Enable write-protect configuration change */
    can_base->CCCR = CANFD_ReadReg(&can_base->CCCR) | CANFD_CCCR_CCE_Msk;

    /* Keep IRQ enabled or not */
    bool irq_enabled = NVIC_GetEnableIRQ(modinit->irq_n);

    CANFD_Open(can_base, canfd_config);

    /* Cover side effect of CANFD_Open() */
    if (!irq_enabled) {
        NVIC_DisableIRQ(modinit->irq_n);    // Disable IRQ line 0
        NVIC_DisableIRQ(var->irq_line1);    // Disable IRQ line 1
    }

    /* Leave INIT mode for normal operation */
    CANFD_RunToNormal(can_base, true);
}

/**
 * \brief Reconfigure filters configured before
 */
static void can_filters_reconfig(can_t *obj)
{
    const struct nu_modinit_s *modinit = get_modinit(obj->can, can_modinit_tab);
    MBED_ASSERT(modinit != NULL);
    MBED_ASSERT(modinit->modname == (int) obj->can);

    struct nu_can_var *var = (struct nu_can_var *) modinit->var;

    struct nu_can_filter *filter = var->filters;
    struct nu_can_filter *filter_end = var->filters + sizeof(var->filters) / sizeof(var->filters[0]);

    for (; filter != filter_end; filter ++) {
        /* A filter not binding to Rx Buffer type is not configured before. */
        if (filter->rxbuf_type == NU_CAN_RXBUF_TYPE_NONE) {
            continue;
        }

        can_filter_config(obj, filter);
    }
}

/**
 * \brief BInd filter to Rx Buffer type
 *
 * \return  0 if failure, or 1 if success
 */
static int can_filter_bind_rxbuf_type(can_t *obj, struct nu_can_filter *filter)
{
    const struct nu_modinit_s *modinit = get_modinit(obj->can, can_modinit_tab);
    MBED_ASSERT(modinit != NULL);
    MBED_ASSERT(modinit->modname == (int) obj->can);

    struct nu_can_var *var = (struct nu_can_var *) modinit->var;

    /* Bind to Rx FIFO 0/1 first if free, or dedicated Rx Buffer */
    if (filter->rxbuf_type == NU_CAN_RXBUF_TYPE_NONE) {
        if (!var->rx_fifo_0_used) {
            var->rx_fifo_0_used = true;
            filter->rxbuf_type = NU_CAN_RXBUF_TYPE_FIFO_0;
        } else if (!var->rx_fifo_1_used) {
            var->rx_fifo_1_used = true;
            filter->rxbuf_type = NU_CAN_RXBUF_TYPE_FIFO_1;
        } else {
            /* H/W doesn't support mask on directing to dedicated Rx Buffer. */
            if (filter->mask != 0xFFFFFFFF) {
                error("CAN HAL supports mask only on first two configured filters (including handle 0). Try disabling mask by setting mask to 0xFFFFFFFF.");
                return 0;
            }

            filter->rxbuf_type = NU_CAN_RXBUF_TYPE_DEDICATED;
        }
    }

    return 1;
}

/**
 * \brief Set up message ID filter
 *
 * \return  0 if failure, or 1 if success
 */
static int can_filter_config(can_t *obj, const struct nu_can_filter *filter)
{
    CANFD_T *can_base = (CANFD_T *) NU_MODBASE(obj->can);

    /* Enter INIT mode for configuration */
    CANFD_RunToNormal(can_base, false);

    /* Enable write-protect configuration change */
    can_base->CCCR = CANFD_ReadReg(&can_base->CCCR) | CANFD_CCCR_CCE_Msk;

    /* Global filter configuration
     *
     * 1. Reject unmatched standard/extended message ID
     * 2. Accept remote message
     */
    CANFD_SetGFC(CANFD0, eCANFD_REJ_NON_MATCH_FRM, eCANFD_REJ_NON_MATCH_FRM, 0, 0);

    /* Get SID/XID filter element index by filter handle */
    uint32_t sidfltr_idx = can_filter_sidfltr_index(filter);
    uint32_t xidfltr_idx = can_filter_xidfltr_index(filter);

    /* Get dedicated Rx Buffer element index for SID/XID filter element by filter handle */
    uint32_t sidfltr_rxdbf_idx = can_filter_sidfltr_rxbuf_index(filter);
    uint32_t xidfltr_rxdbf_idx = can_filter_xidfltr_rxbuf_index(filter);

    /* Configure filter for Standard message ID
     *
     * Direct accepted message to Rx FIFO 0, Rx FIFO 1, or dedicated Rx Buffer
     *
     * NOTE: H/W doesn't support mask on directing to dedicated Rx Buffer.
     */
    if (filter->format == CANStandard || filter->format == CANAny) {
        switch (filter->rxbuf_type) {
        case NU_CAN_RXBUF_TYPE_FIFO_0:
            CANFD_SetSIDFltr(can_base, sidfltr_idx, CANFD_RX_FIFO0_STD_MASK(filter->id, filter->mask));
            break;

        case NU_CAN_RXBUF_TYPE_FIFO_1:
            CANFD_SetSIDFltr(can_base, sidfltr_idx, CANFD_RX_FIFO1_STD_MASK(filter->id, filter->mask));
            break;

        default:
            CANFD_SetSIDFltr(can_base, sidfltr_idx, CANFD_RX_BUFFER_STD(filter->id, sidfltr_rxdbf_idx));
        }
    }

    /* Configure filter for Extended message ID
     *
     * Direct accepted message to Rx FIFO 0, Rx FIFO 1, or dedicated Rx Buffer
     *
     * NOTE: H/W doesn't support mask on directing to dedicated Rx Buffer.
     * NOTE: CANFD.XIDAM applies to all XID filters and is unsuitable for the
     *       per-XID filter requirement.
     */
    if (filter->format == CANExtended || filter->format == CANAny) {
        switch (filter->rxbuf_type) {
        case NU_CAN_RXBUF_TYPE_FIFO_0:
            CANFD_SetXIDFltr(can_base, xidfltr_idx, CANFD_RX_FIFO0_EXT_MASK_LOW(filter->id), CANFD_RX_FIFO0_EXT_MASK_HIGH(filter->mask));
            break;

        case NU_CAN_RXBUF_TYPE_FIFO_1:
            CANFD_SetXIDFltr(can_base, xidfltr_idx, CANFD_RX_FIFO1_EXT_MASK_LOW(filter->id), CANFD_RX_FIFO1_EXT_MASK_HIGH(filter->mask));
            break;

        default:
            CANFD_SetXIDFltr(can_base, xidfltr_idx, CANFD_RX_BUFFER_EXT_LOW(filter->id, xidfltr_rxdbf_idx), CANFD_RX_BUFFER_EXT_HIGH(filter->id, xidfltr_rxdbf_idx));
        }
    }

    /* Leave INIT mode for normal operation */
    CANFD_RunToNormal(can_base, true);

    return 1;
}

static void can_irq(CANName can)
{
    const struct nu_modinit_s *modinit = get_modinit(can, can_modinit_tab);
    MBED_ASSERT(modinit != NULL);
    MBED_ASSERT(modinit->modname == (int) can);

    struct nu_can_var *var = (struct nu_can_var *) modinit->var;

    CANFD_T *can_base = (CANFD_T *) NU_MODBASE(can);

    uint32_t ir = CANFD_ReadReg(&can_base->IR);
    uint32_t ie = CANFD_ReadReg(&can_base->IE);

    /* Clear all interrupt status flags */
    CANFD_ClearStatusFlag(can_base, ir);

    if (ir & CANFD_IR_TC_Msk) {
        if (var->irq_handler && (ie & CANFD_IE_TCE_Msk)) {
            var->irq_handler(var->irq_context, IRQ_TX);
        }
    }

    if (ir & (CANFD_IR_RF0N_Msk | CANFD_IR_RF1N_Msk | CANFD_IR_DRX_Msk)) {
        if (var->irq_handler && (ie & (CANFD_IE_RF0NE_Msk | CANFD_IE_RF1NE_Msk | CANFD_IE_DRXE_Msk))) {
            var->irq_handler(var->irq_context, IRQ_RX);
        }
    }

    if (ir & CANFD_IR_EW_Msk) {
        if (var->irq_handler && (ie & CANFD_IE_EWE_Msk)) {
            var->irq_handler(var->irq_context, IRQ_ERROR);
        }
    }

    if (ir & CANFD_IR_EP_Msk) {
        if (var->irq_handler && (ie & CANFD_IE_EPE_Msk)) {
            var->irq_handler(var->irq_context, IRQ_PASSIVE);
        }
    }

    if (ir & CANFD_IR_BO_Msk) {
        if (var->irq_handler && (ie & CANFD_IE_BOE_Msk)) {
            var->irq_handler(var->irq_context, IRQ_BUS);
        }
    }
}

#endif // DEVICE_CAN
