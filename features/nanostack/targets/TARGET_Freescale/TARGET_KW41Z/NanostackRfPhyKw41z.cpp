/*
* Copyright (c) 2016-2018 ARM Limited. All rights reserved.
* SPDX-License-Identifier: Apache-2.0
* Licensed under the Apache License, Version 2.0 (the License); you may
* not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an AS IS BASIS, WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
#include "mbed_power_mgmt.h"
#include "common_functions.h"
#include "platform/arm_hal_interrupt.h"
#include "platform/arm_hal_phy.h"
#include "NanostackRfPhyKw41z.h"

#include "fsl_xcvr.h"

#define RF_THREAD_STACK_SIZE 1024

static void rf_thread_loop();
Thread rf_thread(osPriorityRealtime, RF_THREAD_STACK_SIZE);

#define PHY_MTU_SIZE     127
#define CRC_LENGTH 0
#define PHY_HEADER_LENGTH 0

#define BM_ZLL_IRQSTS_TMRxMSK (ZLL_IRQSTS_TMR1MSK_MASK | \
                               ZLL_IRQSTS_TMR2MSK_MASK | \
                               ZLL_IRQSTS_TMR3MSK_MASK | \
                               ZLL_IRQSTS_TMR4MSK_MASK)

#define RF_CCA_THRESHOLD 75 /* -75 dBm */

#define gPhyDefaultTxPowerLevel_d     (22)
#define gPhyMaxTxPowerLevel_d         (32)
#define gCcaED_c                      (0)
#define gCcaCCA_MODE1_c               (1)

#define gPhyTimeMask_c                (0x00FFFFFF)

#define KW41Z_SHR_PHY_TIME            12
#define KW41Z_PER_BYTE_TIME           2
#define KW41Z_ACK_WAIT_TIME           54

static int8_t rf_radio_driver_id = -1;
static uint8_t need_ack = 0;

/* PHY states */
typedef enum xcvrState_tag {
    gIdle_c,
    gRX_c,
    gTX_c,
    gCCA_c,
    gTR_c,
    gCCCA_c,
} xcvrState_t;

static int8_t rf_interface_state_control(phy_interface_state_e new_state, uint8_t rf_channel);
static int8_t rf_start_cca(uint8_t *data_ptr, uint16_t data_length, uint8_t tx_handle, data_protocol_e data_protocol);
static int8_t rf_address_write(phy_address_type_e address_type, uint8_t *address_ptr);
static int8_t rf_extension(phy_extension_type_e extension_type, uint8_t *data_ptr);
static void rf_mac_hw_init(void);
static void rf_mac_ed_state_enable(void);
static void rf_mac_set_pending(uint8_t status);
static void rf_mac_set_shortAddress(uint8_t* valueAddress);
static void rf_mac_set_panId(uint8_t* valueAddress);
static void rf_mac_set_mac64(const uint8_t* valueAddress);
static uint8_t rf_convert_energy_level(uint8_t energyLevel);
static void rf_abort(void);
static void rf_ack_wait_timer_start(uint16_t time);
static void rf_get_timestamp(uint32_t *pRetClk);
static uint32_t rf_get_timeout(void);
static void rf_set_timeout(uint32_t timeout);
static void rf_promiscuous(uint8_t state);
static void handle_IRQ_events(void);
static uint8_t PhyPlmeSetCurrentChannelRequest(uint8_t channel, uint8_t pan);
static void rf_receive(void);
static void rf_handle_rx_end(void);

static uint8_t MAC64_addr_default[8] = {1, 2, 3, 4, 5, 6, 7, 8};
static uint8_t MAC64_addr[8];

static xcvrState_t mPhySeqState;
static uint8_t rf_mac_handle;
static volatile uint8_t rf_ed_value = 0;
static volatile bool rf_ack_pending_state = false;
static volatile bool sleep_blocked = false;

static NanostackRfPhyKw41z *rf = NULL;

#define MAC_PACKET_SIZE 127 //MAX MAC payload is 127 bytes
static uint8_t PHYPAYLOAD[MAC_PACKET_SIZE];

const phy_rf_channel_configuration_s phy_2_4ghz = {2405000000U, 5000000U, 250000U, 16U, M_OQPSK};

const phy_device_channel_page_s phy_channel_pages[] = {
        {CHANNEL_PAGE_0, &phy_2_4ghz},
        {CHANNEL_PAGE_0, NULL}
};

static phy_device_driver_s device_driver = {
    PHY_LINK_15_4_2_4GHZ_TYPE,
    PHY_LAYER_PAYLOAD_DATA_FLOW,
    MAC64_addr,
    PHY_MTU_SIZE,
    (char*)"NXP kw41z",
    CRC_LENGTH,
    PHY_HEADER_LENGTH,
    &rf_interface_state_control,
    &rf_start_cca,
    &rf_address_write,
    &rf_extension,
    phy_channel_pages,
    NULL,
    NULL,
    NULL,
    NULL
};

static void rf_thread_loop()
{
    for (;;) {
        ThisThread::flags_wait_all(1);

        platform_enter_critical();

        handle_IRQ_events();

        platform_exit_critical();
        NVIC_ClearPendingIRQ(Radio_1_IRQn);
        NVIC_EnableIRQ(Radio_1_IRQn);
    }
}

static int8_t rf_device_register(void)
{
    if (rf_radio_driver_id < 0) {
        rf_mac_hw_init();
        /**
         * Read factory stored Mac address to RAM
         */
        common_write_32_bit(ZLL->MACLONGADDRS0_MSB, MAC64_addr);
        common_write_32_bit(ZLL->MACLONGADDRS0_LSB, MAC64_addr + 4);

        rf_radio_driver_id = arm_net_phy_register(&device_driver);
    }

    return rf_radio_driver_id;
}

static void rf_device_unregister(void)
{
    arm_net_phy_unregister(rf_radio_driver_id);

    if (sleep_blocked) {
        sleep_manager_unlock_deep_sleep();
        sleep_blocked = false;
    }
}

/*
 * \brief Function enables/disables Rx promiscuous mode.
 *
 * \param state of XCVR promiscuous mode
 *
 * \return none
 */
static void rf_promiscuous(uint8_t state)
{
    if (state) {
        ZLL->PHY_CTRL |= ZLL_PHY_CTRL_PROMISCUOUS_MASK;
        /* FRM_VER[11:8] = b1111. Any FrameVersion accepted */
        ZLL->RX_FRAME_FILTER |= (ZLL_RX_FRAME_FILTER_FRM_VER_FILTER_MASK |
                                 ZLL_RX_FRAME_FILTER_ACK_FT_MASK |
                                 ZLL_RX_FRAME_FILTER_NS_FT_MASK);
    } else {
        ZLL->PHY_CTRL &= ~ZLL_PHY_CTRL_PROMISCUOUS_MASK;
        /* FRM_VER[11:8] = b0011. Accept FrameVersion 0 and 1 packets, reject all others */
        /* Beacon, Data and MAC command frame types accepted */
        ZLL->RX_FRAME_FILTER &= ~(ZLL_RX_FRAME_FILTER_FRM_VER_FILTER_MASK |
                                 ZLL_RX_FRAME_FILTER_ACK_FT_MASK  |
                                 ZLL_RX_FRAME_FILTER_NS_FT_MASK   |
                                 ZLL_RX_FRAME_FILTER_ACTIVE_PROMISCUOUS_MASK);
        ZLL->RX_FRAME_FILTER |= ZLL_RX_FRAME_FILTER_FRM_VER_FILTER(3);
    }
}

static void rf_mac_set_pending(uint8_t status)
{
    uint32_t reg = ZLL->SAM_TABLE;

    /* Disable the Source Address Matching feature and set FP manually */
    reg |= ZLL_SAM_TABLE_ACK_FRM_PND_CTRL_MASK;
    if (status) {
        reg |= ZLL_SAM_TABLE_ACK_FRM_PND_MASK;
        rf_ack_pending_state = true;
    } else {
        reg &= ~ZLL_SAM_TABLE_ACK_FRM_PND_MASK;
        rf_ack_pending_state = false;
    }
    ZLL->SAM_TABLE = reg;
}

static int8_t rf_interface_state_control(phy_interface_state_e new_state, uint8_t rf_channel)
{
    platform_enter_critical();

    switch (new_state)
    {
        /*Reset PHY driver and set to idle*/
        case PHY_INTERFACE_RESET:
            rf_abort();
            if (sleep_blocked) {
                sleep_manager_unlock_deep_sleep();
                sleep_blocked = false;
            }
            break;
        /*Disable PHY Interface driver*/
        case PHY_INTERFACE_DOWN:
            rf_abort();
            if (sleep_blocked) {
                sleep_manager_unlock_deep_sleep();
                sleep_blocked = false;
            }
            break;
        /*Enable PHY Interface driver*/
        case PHY_INTERFACE_UP:
            if (PhyPlmeSetCurrentChannelRequest(rf_channel, 0)) {
                return 1;
            }
            if (!sleep_blocked) {
                /* Disable enter to deep sleep when transfer active */
                sleep_manager_lock_deep_sleep();
                sleep_blocked = true;
            }
            rf_receive();
            break;
        /*Enable wireless interface ED scan mode*/
        case PHY_INTERFACE_RX_ENERGY_STATE:
            if (PhyPlmeSetCurrentChannelRequest(rf_channel, 0)) {
                return 1;
            }
            if (!sleep_blocked) {
                /* Disable enter to deep sleep when transfer active */
                sleep_manager_lock_deep_sleep();
                sleep_blocked = true;
            }
            rf_abort();
            rf_mac_ed_state_enable();
            break;
        case PHY_INTERFACE_SNIFFER_STATE:             /**< Enable Sniffer state */
            rf_promiscuous(1);
            if (PhyPlmeSetCurrentChannelRequest(rf_channel, 0)) {
                return 1;
            }
            if (!sleep_blocked) {
                /* Disable enter to deep sleep when transfer active */
                sleep_manager_lock_deep_sleep();
                sleep_blocked = true;
            }
            rf_receive();
            break;
    }

    platform_exit_critical();

    return 0;
}

/*
 * \brief Function forces the XCVR to Idle state.
 *
 * \param none
 *
 * \return none
 */
static void rf_abort(void)
{
    /* Mask XCVR irq */
    NVIC_DisableIRQ(Radio_1_IRQn);

    mPhySeqState = gIdle_c;

    /* Mask SEQ interrupt */
    ZLL->PHY_CTRL |= ZLL_PHY_CTRL_SEQMSK_MASK;

    /* Disable timer trigger (for scheduled XCVSEQ) */
    if (ZLL->PHY_CTRL & ZLL_PHY_CTRL_TMRTRIGEN_MASK) {
        ZLL->PHY_CTRL &= ~ZLL_PHY_CTRL_TMRTRIGEN_MASK;
        /* give the FSM enough time to start if it was triggered */
        while( (XCVR_MISC->XCVR_CTRL & XCVR_CTRL_XCVR_STATUS_TSM_COUNT_MASK) == 0) {}
    }

    /* If XCVR is not idle, abort current SEQ */
    if (ZLL->PHY_CTRL & ZLL_PHY_CTRL_XCVSEQ_MASK) {
        /* Abort current SEQ */
        ZLL->PHY_CTRL &= ~ZLL_PHY_CTRL_XCVSEQ_MASK;

        /* Wait for Sequence Idle (if not already) */
        while (ZLL->SEQ_STATE & ZLL_SEQ_STATE_SEQ_STATE_MASK) {}
    }

    /* Stop timers */
    ZLL->PHY_CTRL &= ~(ZLL_PHY_CTRL_TMR1CMP_EN_MASK |
                       ZLL_PHY_CTRL_TMR2CMP_EN_MASK |
                       ZLL_PHY_CTRL_TMR3CMP_EN_MASK |
                       ZLL_PHY_CTRL_TC3TMOUT_MASK);

    /* clear all IRQ bits to avoid unexpected interrupts */
    ZLL->IRQSTS = ZLL->IRQSTS;

    /* Unmask XCVR irq */
    NVIC_EnableIRQ(Radio_1_IRQn);
}

static int8_t rf_start_cca(uint8_t *data_ptr, uint16_t data_length, uint8_t tx_handle, data_protocol_e data_protocol)
{
    uint32_t reg, tx_len;
    uint32_t irqSts;
    volatile uint8_t *pPB;
    uint8_t i;
    uint32_t tx_warmup_time;

    platform_enter_critical();

    if (mPhySeqState == gRX_c) {
        rf_abort();
    }

    /* Check if transmitter is busy*/
    if (mPhySeqState != gIdle_c) {
        platform_exit_critical();
        /*Return busy*/
        return -1;
    }

    /*Store TX handle*/
    rf_mac_handle = tx_handle;

    /* Check if transmitted data needs to be acked */
    need_ack = (*data_ptr & 0x20) == 0x20;

    /* Load data into Packet Buffer */
    pPB = (uint8_t*)ZLL->PKT_BUFFER_TX;

    tx_len = data_length + 2;
    *pPB++ = tx_len; /* including 2 bytes of FCS */

    for (i = 0; i < data_length; i++) {
        *pPB++ = *data_ptr++;
    }

    reg = ZLL->PHY_CTRL;

    /* Perform CCA before TX */
    reg |= ZLL_PHY_CTRL_CCABFRTX_MASK;

    /* Set CCA mode 1 */
    reg &= ~(ZLL_PHY_CTRL_CCATYPE_MASK);
    reg |= ZLL_PHY_CTRL_CCATYPE(gCcaCCA_MODE1_c);
    ZLL->PHY_CTRL = reg;

    /* Perform TxRxAck sequence if required by phyTxMode */
    if (need_ack) {
        ZLL->PHY_CTRL |= ZLL_PHY_CTRL_RXACKRQD_MASK;
        mPhySeqState = gTR_c;
    } else {
        ZLL->PHY_CTRL &= ~ZLL_PHY_CTRL_RXACKRQD_MASK;
        mPhySeqState = gTX_c;
    }

    /* Ensure that no spurious interrupts are raised */
    irqSts = ZLL->IRQSTS;
    irqSts &= ~(ZLL_IRQSTS_TMR1IRQ_MASK | ZLL_IRQSTS_TMR4IRQ_MASK);
    irqSts |= ZLL_IRQSTS_TMR3MSK_MASK;
    ZLL->IRQSTS = irqSts;

    tx_warmup_time = (XCVR_TSM->END_OF_SEQ & XCVR_TSM_END_OF_SEQ_END_OF_TX_WU_MASK) >>
                      XCVR_TSM_END_OF_SEQ_END_OF_TX_WU_SHIFT;

    /* Compute warmup times (scaled to 16us) */
    if (tx_warmup_time & 0x0F) {
        tx_warmup_time = 1 + (tx_warmup_time >> 4);
    } else {
        tx_warmup_time = tx_warmup_time >> 4;
    }

    if (need_ack) {
        rf_ack_wait_timer_start(tx_warmup_time + KW41Z_SHR_PHY_TIME +
                                tx_len * KW41Z_PER_BYTE_TIME  + 10 + KW41Z_ACK_WAIT_TIME);
    }

    /* Unmask SEQ interrupt */
    ZLL->PHY_CTRL &= ~(ZLL_PHY_CTRL_SEQMSK_MASK);

    /* Start the TX / TRX */
    reg = ZLL->PHY_CTRL;
    reg &= ~(ZLL_PHY_CTRL_XCVSEQ_MASK);
    reg |= mPhySeqState;
    ZLL->PHY_CTRL = reg;

    platform_exit_critical();

    /*Return success*/
    return 0;
}

static int8_t rf_address_write(phy_address_type_e address_type, uint8_t *address_ptr)
{
    int8_t ret_val = 0;

    platform_enter_critical();

    switch (address_type) {
    case PHY_MAC_64BIT:
        rf_mac_set_mac64(address_ptr);
        break;
    /*Set 16-bit address*/
    case PHY_MAC_16BIT:
        rf_mac_set_shortAddress(address_ptr);
        break;
    /*Set PAN Id*/
    case PHY_MAC_PANID:
        rf_mac_set_panId(address_ptr);
        break;
    default:
        ret_val = -1;
    }

    platform_exit_critical();

    return ret_val;
}

static int8_t rf_extension(phy_extension_type_e extension_type, uint8_t *data_ptr)
{
    int ret_value = 0;

    platform_enter_critical();

    switch (extension_type) {
        case PHY_EXTENSION_CTRL_PENDING_BIT: /**< Control MAC pending bit for indirect data. */
            rf_mac_set_pending(*data_ptr);
            break;
        /* Return frame Auto Ack frame pending status */
        case PHY_EXTENSION_READ_LAST_ACK_PENDING_STATUS: {
            *data_ptr = rf_ack_pending_state;
            break;
        }
        case PHY_EXTENSION_SET_CHANNEL:  /**< Net library channel set. */
            break;
        case PHY_EXTENSION_READ_CHANNEL_ENERGY: /**< RF interface ED scan energy read. */
            *data_ptr = rf_ed_value;
            break;
        case PHY_EXTENSION_READ_LINK_STATUS: /**< Net library could read link status. */
        case PHY_EXTENSION_CONVERT_SIGNAL_INFO: /**< Convert signal info. */
        default:
            ret_value = -1;
    }

    platform_exit_critical();

    return ret_value;
}

/*
 * \brief Function converts the energy level from dBm to a 0-255 value.
 *
 * \param energyLevel in dBm
 *
 * \return energy level (0-255)
 */
static uint8_t rf_convert_energy_level(uint8_t energyLevel)
{
    int32_t temp = (int8_t)energyLevel;

    if (temp <= -82) {
        temp = 0x00;
    } else if (temp >= -3) {
        temp = 0xFF;
    } else {
        /* Convert energy level from dbm into a 0x00-0xFF value */
        temp = (255 * temp + 20910) / 79;
    }

    return (uint8_t)temp;
}

/**
 *  SET MAC 16 address to Register
 */
static void rf_mac_set_shortAddress(uint8_t* valueAddress) {
    ZLL->MACSHORTADDRS0 &= ~ZLL_MACSHORTADDRS0_MACSHORTADDRS0_MASK;
    ZLL->MACSHORTADDRS0 |= ZLL_MACSHORTADDRS0_MACSHORTADDRS0(common_read_16_bit(valueAddress));
}

/**
 *  SET PAN-ID to Register
 */
static void rf_mac_set_panId(uint8_t* valueAddress) {
    ZLL->MACSHORTADDRS0 &= ~ZLL_MACSHORTADDRS0_MACPANID0_MASK;
    ZLL->MACSHORTADDRS0 |= ZLL_MACSHORTADDRS0_MACPANID0(common_read_16_bit(valueAddress));
}

/**
 *  SET MAC64 address to register
 */
static void rf_mac_set_mac64(const uint8_t* valueAddress) {
    ZLL->MACLONGADDRS0_MSB = common_read_32_bit(valueAddress);
    valueAddress += 4;
    ZLL->MACLONGADDRS0_LSB = common_read_32_bit(valueAddress);
}

static void PhyPlmeSetPwrLevelRequest(uint8_t pwrStep)
{
    /* Do not exceed the Tx power limit for the current channel */
    if (pwrStep > gPhyMaxTxPowerLevel_d) {
        pwrStep = gPhyMaxTxPowerLevel_d;
    }

    if (pwrStep > 2) {
        pwrStep = (pwrStep << 1) - 2;
    }

    ZLL->PA_PWR = pwrStep;
}

static uint8_t PhyPlmeGetPwrLevelRequest(void)
{
    uint8_t pwrStep = (uint8_t)ZLL->PA_PWR;

    if (pwrStep > 2) {
        pwrStep = (pwrStep + 2) >> 1;
    }

    return pwrStep;
}

static uint8_t PhyPlmeSetCurrentChannelRequest(uint8_t channel, uint8_t pan)
{
    if((channel < 11) || (channel > 26)) {
        return 1;
    }

    if (!pan) {
        ZLL->CHANNEL_NUM0 = channel;
    } else {
        ZLL->CHANNEL_NUM1 = channel;
    }

    /* Make sure the current Tx power doesn't exceed the Tx power limit for the new channel */
    if (PhyPlmeGetPwrLevelRequest() > gPhyMaxTxPowerLevel_d) {
        PhyPlmeSetPwrLevelRequest(gPhyMaxTxPowerLevel_d);
    }

    return 0;
}

/*
 * Function is a RF interrupt vector.
 */
static void PHY_InterruptHandler(void)
{
    /* Disable and clear transceiver(IRQ_B) interrupt */
    NVIC_DisableIRQ(Radio_1_IRQn);
    rf_thread.flags_set(1);
}

static void PhyIsrSeqCleanup(void)
{
    uint32_t irqStatus;

    /* Set the PHY sequencer back to IDLE */
    ZLL->PHY_CTRL &= ~ZLL_PHY_CTRL_XCVSEQ_MASK;
    /* Mask SEQ, RX, TX and CCA interrupts */
    ZLL->PHY_CTRL |= ZLL_PHY_CTRL_CCAMSK_MASK |
                     ZLL_PHY_CTRL_RXMSK_MASK  |
                     ZLL_PHY_CTRL_TXMSK_MASK  |
                     ZLL_PHY_CTRL_SEQMSK_MASK;

    while (ZLL->SEQ_STATE & ZLL_SEQ_STATE_SEQ_STATE_MASK) {}

    irqStatus = ZLL->IRQSTS;
    /* Mask TMR3 interrupt */
    irqStatus |= ZLL_IRQSTS_TMR3MSK_MASK;
    /* Clear transceiver interrupts except TMRxIRQ */
    irqStatus &= ~(ZLL_IRQSTS_TMR1IRQ_MASK |
                    ZLL_IRQSTS_TMR2IRQ_MASK |
                    ZLL_IRQSTS_TMR3IRQ_MASK |
                    ZLL_IRQSTS_TMR4IRQ_MASK);
    ZLL->IRQSTS = irqStatus;
}

static void PhyIsrTimeoutCleanup(void)
{
    uint32_t irqStatus;

    /* Set the PHY sequencer back to IDLE and disable TMR3 comparator and timeout */
    ZLL->PHY_CTRL &= ~(ZLL_PHY_CTRL_TMR3CMP_EN_MASK |
                       ZLL_PHY_CTRL_TC3TMOUT_MASK   |
                       ZLL_PHY_CTRL_XCVSEQ_MASK);
    /* Mask SEQ, RX, TX and CCA interrupts */
    ZLL->PHY_CTRL |= ZLL_PHY_CTRL_CCAMSK_MASK |
                     ZLL_PHY_CTRL_RXMSK_MASK  |
                     ZLL_PHY_CTRL_TXMSK_MASK  |
                     ZLL_PHY_CTRL_SEQMSK_MASK;

    while (ZLL->SEQ_STATE & ZLL_SEQ_STATE_SEQ_STATE_MASK) {}

    irqStatus = ZLL->IRQSTS;
    /* Mask TMR3 interrupt */
    irqStatus |= ZLL_IRQSTS_TMR3MSK_MASK;
    /* Clear transceiver interrupts except TMR1IRQ and TMR4IRQ. */
    irqStatus &= ~( ZLL_IRQSTS_TMR1IRQ_MASK |
                    ZLL_IRQSTS_TMR4IRQ_MASK );
    ZLL->IRQSTS = irqStatus;

    /* The packet was transmitted successfully, but no ACK was received */
    if (device_driver.phy_tx_done_cb) {
        device_driver.phy_tx_done_cb(rf_radio_driver_id, rf_mac_handle, PHY_LINK_TX_SUCCESS, 1, 1);
    }
}

/*
 * \brief Function get the time-out for the current sequence.
 *
 * \return sequence time-out value
 */
static uint32_t rf_get_timeout(void)
{
    return ZLL->T3CMP;
}

/*
 * \brief Function set a time-out to an XCVR sequence.
 *
 * \param pEndTime sequence time-out value [symbols]
 *
 * \return none
 */
static void rf_set_timeout(uint32_t pEndTime)
{
    uint32_t irqsts;

    platform_enter_critical();

    /* disable TMR3 compare */
    ZLL->PHY_CTRL &= ~ZLL_PHY_CTRL_TMR3CMP_EN_MASK;
    ZLL->T3CMP = pEndTime & ZLL_T3CMP_T3CMP_MASK;

    /* acknowledge TMR3 IRQ */
    irqsts  = ZLL->IRQSTS & BM_ZLL_IRQSTS_TMRxMSK;
    irqsts |= ZLL_IRQSTS_TMR3IRQ_MASK;
    ZLL->IRQSTS = irqsts;
    /* enable TMR3 compare */
    ZLL->PHY_CTRL |= ZLL_PHY_CTRL_TMR3CMP_EN_MASK;
    /* enable autosequence stop by TC3 match */
    ZLL->PHY_CTRL |= ZLL_PHY_CTRL_TC3TMOUT_MASK;

    platform_exit_critical();

}

/**
 * Call this only One time
 */
static void rf_mac_hw_init(void)
{
    xcvrStatus_t xcvrStatus;
    uint32_t phyReg;

    /* The data rate argument only matters when GFSK/MSK protocol is selected */
    xcvrStatus = XCVR_Init(ZIGBEE_MODE, DR_500KBPS);
    if (xcvrStatus != gXcvrSuccess_c) {
        return;
    }

    mPhySeqState = gIdle_c;

    /* Enable 16 bit mode for TC2 - TC2 prime EN, disable all timers,
       enable AUTOACK, mask all interrupts */
    ZLL->PHY_CTRL = (gCcaCCA_MODE1_c << ZLL_PHY_CTRL_CCATYPE_SHIFT) |
                     ZLL_PHY_CTRL_TC2PRIME_EN_MASK    |
                     ZLL_PHY_CTRL_TSM_MSK_MASK        |
                     ZLL_PHY_CTRL_WAKE_MSK_MASK       |
                     ZLL_PHY_CTRL_CRC_MSK_MASK        |
                     ZLL_PHY_CTRL_PLL_UNLOCK_MSK_MASK |
                     ZLL_PHY_CTRL_FILTERFAIL_MSK_MASK |
                     ZLL_PHY_CTRL_RX_WMRK_MSK_MASK    |
                     ZLL_PHY_CTRL_CCAMSK_MASK         |
                     ZLL_PHY_CTRL_RXMSK_MASK          |
                     ZLL_PHY_CTRL_TXMSK_MASK          |
                     ZLL_PHY_CTRL_SEQMSK_MASK         |
                     ZLL_PHY_CTRL_AUTOACK_MASK        |
                     ZLL_PHY_CTRL_TRCV_MSK_MASK;

    /* Clear all PP IRQ bits to avoid unexpected interrupts immediately after init
       disable all timer interrupts */
    ZLL->IRQSTS = ZLL->IRQSTS;

    /* Clear HW indirect queue */
    ZLL->SAM_TABLE |= ZLL_SAM_TABLE_INVALIDATE_ALL_MASK;

    /*  Frame Filtering
        FRM_VER[7:6] = b11. Accept FrameVersion 0 and 1 packets, reject all others */
    ZLL->RX_FRAME_FILTER &= ~ZLL_RX_FRAME_FILTER_FRM_VER_FILTER_MASK;
    ZLL->RX_FRAME_FILTER = ZLL_RX_FRAME_FILTER_FRM_VER_FILTER(3) |
                           ZLL_RX_FRAME_FILTER_CMD_FT_MASK  |
                           ZLL_RX_FRAME_FILTER_DATA_FT_MASK |
                           ZLL_RX_FRAME_FILTER_BEACON_FT_MASK;

    /* Set prescaller to obtain 1 symbol (16us) timebase */
    ZLL->TMR_PRESCALE = 0x05;

    /* Set CCA threshold to -75 dBm */
    ZLL->CCA_LQI_CTRL &= ~ZLL_CCA_LQI_CTRL_CCA1_THRESH_MASK;
    ZLL->CCA_LQI_CTRL |= ZLL_CCA_LQI_CTRL_CCA1_THRESH(RF_CCA_THRESHOLD);

    /* Set the default power level */
    PhyPlmeSetPwrLevelRequest(gPhyDefaultTxPowerLevel_d);

    /* Adjust ACK delay to fulfill the 802.15.4 turnaround requirements */
    ZLL->ACKDELAY &= ~ZLL_ACKDELAY_ACKDELAY_MASK;
    ZLL->ACKDELAY |= ZLL_ACKDELAY_ACKDELAY(-8);

    /* Adjust LQI compensation */
    ZLL->CCA_LQI_CTRL &= ~ZLL_CCA_LQI_CTRL_LQI_OFFSET_COMP_MASK;
    ZLL->CCA_LQI_CTRL |= ZLL_CCA_LQI_CTRL_LQI_OFFSET_COMP(96);

    /* Enable the RxWatermark IRQ */
    ZLL->PHY_CTRL &= ~ZLL_PHY_CTRL_RX_WMRK_MSK_MASK;
    /* Set Rx watermark level */
    ZLL->RX_WTR_MARK = 0;

    /* Set default channels */
    PhyPlmeSetCurrentChannelRequest(0x0B, 0); /* 2405 MHz */
    PhyPlmeSetCurrentChannelRequest(0x0B, 1); /* 2405 MHz */

    /* DSM settings */
    phyReg = (RSIM->RF_OSC_CTRL & RSIM_RF_OSC_CTRL_BB_XTAL_READY_COUNT_SEL_MASK) >>
             RSIM_RF_OSC_CTRL_BB_XTAL_READY_COUNT_SEL_SHIFT;
    phyReg = (1024U << phyReg) / (SystemCoreClock / 32768) + 1;
    RSIM->DSM_OSC_OFFSET = phyReg;

    osStatus status = rf_thread.start(mbed::callback(rf_thread_loop));
    MBED_ASSERT(status == osOK);

    /** Clear and enable MAC IRQ at task level, when scheduler is on. */
    InstallIRQHandler((IRQn_Type)Radio_1_IRQn, (uint32_t)PHY_InterruptHandler);

    /* Unmask Transceiver Global Interrupts */
    ZLL->PHY_CTRL &= ~ZLL_PHY_CTRL_TRCV_MSK_MASK;

    NVIC_ClearPendingIRQ(Radio_1_IRQn);
    NVIC_EnableIRQ(Radio_1_IRQn);
}

/*
 * \brief Function reads a time-stamp value from XCVR [symbols]
 *
 * \param pEndTime pointer to location where time-stamp will be stored
 *
 * \return none
 */
static void rf_get_timestamp(uint32_t *pRetClk)
{
    if (NULL == pRetClk) {
        return;
    }

    platform_enter_critical();

    *pRetClk = 0;
    *pRetClk = ZLL->EVENT_TMR >> ZLL_EVENT_TMR_EVENT_TMR_SHIFT;

    platform_exit_critical();
}

/*
 * \brief Function starts the ACK wait time-out.
 *
 * \param slots The ACK wait time-out in [symbols]
 *
 * \return none
 */
static void rf_ack_wait_timer_start(uint16_t time)
{
    uint32_t timestamp, t;

    rf_get_timestamp(&timestamp);

    t = (rf_get_timeout() - timestamp) & gPhyTimeMask_c;

    if (t > 1) {
        timestamp += time;
        rf_set_timeout(timestamp);
    }
}

/*
 * \brief Function sets the RF in RX state.
 *
 * \param none
 *
 * \return none
 */
static void rf_receive(void)
{
    uint32_t irqSts;

    /* RX can start only from Idle state */
    if (mPhySeqState != gIdle_c) {
        rf_abort();
    }

    mPhySeqState = gRX_c;

    /* Ensure that no spurious interrupts are raised, but do not change TMR1 and TMR4 IRQ status */
    irqSts = ZLL->IRQSTS;
    irqSts &= ~(ZLL_IRQSTS_TMR1IRQ_MASK | ZLL_IRQSTS_TMR4IRQ_MASK);
    irqSts |= ZLL_IRQSTS_TMR3MSK_MASK;
    ZLL->IRQSTS = irqSts;

    /* unmask SEQ interrupt */
    ZLL->PHY_CTRL &= ~ZLL_PHY_CTRL_SEQMSK_MASK;

    /* Start the RX sequence */
    ZLL->PHY_CTRL |= gRX_c;
}

static void rf_mac_ed_state_enable(void)
{
    uint32_t ccaMode, irqSts;

    mPhySeqState = gCCA_c;

    /* Switch to ED mode */
    ccaMode = (ZLL->PHY_CTRL  & ZLL_PHY_CTRL_CCATYPE_MASK) >> ZLL_PHY_CTRL_CCATYPE_SHIFT;
    if (ccaMode != gCcaED_c) {
        ZLL->PHY_CTRL &= ~(ZLL_PHY_CTRL_CCATYPE_MASK);
    }

    /* Ensure that no spurious interrupts are raised(do not change TMR1 and TMR4 IRQ status) */
    irqSts = ZLL->IRQSTS;
    irqSts &= ~(ZLL_IRQSTS_TMR1IRQ_MASK | ZLL_IRQSTS_TMR4IRQ_MASK);
    irqSts |= ZLL_IRQSTS_TMR3MSK_MASK;
    ZLL->IRQSTS = irqSts;

    /* Unmask SEQ interrupt */
    ZLL->PHY_CTRL &= ~ZLL_PHY_CTRL_SEQMSK_MASK;

    /* start ED sequence */
    ZLL->PHY_CTRL |= gCCA_c;
}

/*
 * \brief Function is a call back for TX end interrupt.
 *
 * \param none
 *
 * \return none
 */
static void rf_handle_tx_end(bool framePending)
{
    /*Start receiver*/
    rf_receive();

    if (!device_driver.phy_tx_done_cb) {
        return;
    }

    /*Call PHY TX Done API*/
    if (need_ack) {
        if (framePending) {
            device_driver.phy_tx_done_cb(rf_radio_driver_id, rf_mac_handle, PHY_LINK_TX_DONE_PENDING, 1, 1);
        } else {
            device_driver.phy_tx_done_cb(rf_radio_driver_id, rf_mac_handle, PHY_LINK_TX_DONE, 1, 1);
        }
    } else {
        device_driver.phy_tx_done_cb(rf_radio_driver_id, rf_mac_handle, PHY_LINK_TX_SUCCESS, 1, 1);
    }
}

/*
 * \brief Function converts LQI into RSSI.
 *
 * \param LQI
 *
 * \return RSSI
 */
static int8_t rf_convert_LQI_to_RSSI(uint8_t lqi)
{
    int32_t rssi = (36 * lqi - 9836) / 109;

    return (int8_t)rssi;
}

/*
 * \brief Function scale the LQI value reported by RF into a 0-255 value.
 *
 * \param hwLqi - the LQI value reported by RF
 *
 * \return scaled LQI
 */
static uint8_t rf_convert_LQI(uint8_t hwLqi)
{
    if (hwLqi >= 220) {
        return 255;
    } else {
        return (51 * hwLqi) / 44;
    }
}

/*
 * \brief Function is a call back for RX end interrupt.
 *
 * \param none
 *
 * \return none
 */
static void rf_handle_rx_end(void)
{
    uint8_t rf_lqi = (ZLL->LQI_AND_RSSI & ZLL_LQI_AND_RSSI_LQI_VALUE_MASK) >>
                                          ZLL_LQI_AND_RSSI_LQI_VALUE_SHIFT;
    int8_t rf_rssi = 0;
    uint8_t len;
    uint8_t i;
    volatile uint8_t *pPB;

    len = (ZLL->IRQSTS & ZLL_IRQSTS_RX_FRAME_LENGTH_MASK) >> ZLL_IRQSTS_RX_FRAME_LENGTH_SHIFT; /* Including FCS (2 bytes) */

    /* Excluding FCS (2 bytes) */
    len -= 2;

    /*Check the length is valid*/
    if (len > 1 && len < MAC_PACKET_SIZE) {
        rf_lqi  = rf_convert_LQI(rf_lqi);
        rf_rssi = rf_convert_LQI_to_RSSI(rf_lqi);

        /* Load data from Packet Buffer */
        pPB = (uint8_t*)ZLL->PKT_BUFFER_RX;

        for (i = 0; i < len; i++) {
            PHYPAYLOAD[i] = *pPB++;
        }

        /* Start receiver */
        rf_receive();

        if (device_driver.phy_rx_cb) {
            device_driver.phy_rx_cb(PHYPAYLOAD, len, rf_lqi, rf_rssi, rf_radio_driver_id);
        }
    } else {
        /* Start receiver */
        rf_receive();
    }
}

static void handle_IRQ_events(void)
{
    uint8_t xcvseqCopy;
    uint32_t irqStatus;

    /* Read current XCVRSEQ and interrupt status */
    xcvseqCopy = ZLL->PHY_CTRL & ZLL_PHY_CTRL_XCVSEQ_MASK;
    irqStatus     = ZLL->IRQSTS;
    /* Clear all xcvr interrupts */
    ZLL->IRQSTS = irqStatus;

    /* Filter Fail IRQ */
    if (irqStatus & ZLL_IRQSTS_FILTERFAIL_IRQ_MASK) {

    } else {
        /* Rx Watermark IRQ */
        if((!(ZLL->PHY_CTRL & ZLL_PHY_CTRL_RX_WMRK_MSK_MASK)) &&
           (irqStatus & ZLL_IRQSTS_RXWTRMRKIRQ_MASK)) {
            uint32_t rx_len = (irqStatus & ZLL_IRQSTS_RX_FRAME_LENGTH_MASK) >> ZLL_IRQSTS_RX_FRAME_LENGTH_SHIFT;

            /* Convert to symbols and add IFS and ACK duration */
            rx_len = rx_len * 2 + 12 + 22 + 2;
            rf_ack_wait_timer_start(rx_len);
        }
    }

    /* Sequencer interrupt, the autosequence has completed */
    if (irqStatus & ZLL_IRQSTS_SEQIRQ_MASK) {

        /* XCVR will be set to Idle */
        mPhySeqState = gIdle_c;

        /* PLL unlock, the autosequence has been aborted due to PLL unlock */
        if (irqStatus & ZLL_IRQSTS_PLL_UNLOCK_IRQ_MASK) {
            PhyIsrSeqCleanup();
            /* Start receiver */
            rf_receive();
        }
        /* TMR3 timeout, the autosequence has been aborted due to TMR3 timeout */
        else if ((irqStatus & ZLL_IRQSTS_TMR3IRQ_MASK) &&
                  (!(irqStatus & ZLL_IRQSTS_RXIRQ_MASK)) &&
                  (xcvseqCopy != gTX_c)) {
            PhyIsrTimeoutCleanup();
            /* Start receiver */
            rf_receive();
        } else {
            PhyIsrSeqCleanup();
            switch(xcvseqCopy)
            {
            case gTX_c:
                if ((ZLL->PHY_CTRL & ZLL_PHY_CTRL_CCABFRTX_MASK) &&
                    (irqStatus & ZLL_IRQSTS_CCA_MASK)) {
                    device_driver.phy_tx_done_cb(rf_radio_driver_id, rf_mac_handle,
                                                 PHY_LINK_CCA_FAIL, 1, 1);
                } else {
                    rf_handle_tx_end(false);
                }
                break;

            case gTR_c:
                if ((ZLL->PHY_CTRL & ZLL_PHY_CTRL_CCABFRTX_MASK) &&
                    (irqStatus & ZLL_IRQSTS_CCA_MASK)) {
                    device_driver.phy_tx_done_cb(rf_radio_driver_id, rf_mac_handle,
                                                 PHY_LINK_CCA_FAIL, 1, 1);
                } else {
                    rf_handle_tx_end((irqStatus & ZLL_IRQSTS_RX_FRM_PEND_MASK) > 0);
                }
                break;

            case gRX_c:
                rf_handle_rx_end();
                break;

            case gCCA_c:
                rf_ed_value = rf_convert_energy_level((ZLL->LQI_AND_RSSI &
                                                       ZLL_LQI_AND_RSSI_CCA1_ED_FNL_MASK) >>
                                                       ZLL_LQI_AND_RSSI_CCA1_ED_FNL_SHIFT);
                break;

            default:
                break;
            }
        }
    }
}

NanostackRfPhyKw41z::NanostackRfPhyKw41z()
{
    memcpy(MAC64_addr, MAC64_addr_default, sizeof(MAC64_addr));
}

NanostackRfPhyKw41z::~NanostackRfPhyKw41z()
{
    // Do nothing
}

int8_t NanostackRfPhyKw41z::rf_register()
{
    platform_enter_critical();

    if (rf != NULL) {
        platform_exit_critical();
        error("Multiple registrations of NanostackRfPhyKw41z not supported");
        return -1;
    }

    rf = this;
    int8_t radio_id = rf_device_register();
    if (radio_id < 0) {
        rf = NULL;
    }

    platform_exit_critical();
    return radio_id;
}

void NanostackRfPhyKw41z::rf_unregister()
{
    platform_enter_critical();

    if (rf != this) {
        platform_exit_critical();
        return;
    }

    rf_device_unregister();
    rf = NULL;

    platform_exit_critical();
}

void NanostackRfPhyKw41z::get_mac_address(uint8_t *mac)
{
    platform_enter_critical();

    memcpy((void*)mac, (void*)MAC64_addr, sizeof(MAC64_addr));

    platform_exit_critical();
}

void NanostackRfPhyKw41z::set_mac_address(uint8_t *mac)
{
    platform_enter_critical();

    if (NULL != rf) {
        error("NanostackRfPhyKw41z cannot change mac address when running");
        platform_exit_critical();
        return;
    }
    memcpy((void*)MAC64_addr, (void*)mac, sizeof(MAC64_addr));

    platform_exit_critical();
}

NanostackRfPhy &NanostackRfPhy::get_default_instance()
{
    static NanostackRfPhyKw41z rf_phy;
    return rf_phy;
}
