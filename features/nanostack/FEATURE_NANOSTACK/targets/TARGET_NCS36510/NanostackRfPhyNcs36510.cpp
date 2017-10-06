/*
* Copyright (c) 2016-2016 ARM Limited. All rights reserved.
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
#include "mbed.h"
#include "ns_types.h"
#include <string.h>
#include "common_functions.h"
#include "randLIB.h"
#include "platform/arm_hal_interrupt.h"
#include "platform/arm_hal_phy.h"
#include "NanostackRfPhyNcs36510.h"

extern "C" {
#include "TARGET_NCS36510/memory_map.h"
#include "TARGET_NCS36510/clock.h"
#include "TARGET_NCS36510/ticker.h"
#include "TARGET_NCS36510/rfAna.h"
}

#define RF_THREAD_STACK_SIZE 1024

#define SIGNAL_COUNT_RADIO 1

static void rf_thread_loop();
Thread rf_thread(osPriorityRealtime, RF_THREAD_STACK_SIZE);

#define PHY_MTU_SIZE     127
#define CRC_LENGTH 0
#define PHY_HEADER_LENGTH 0

/**
 * MAC status code bit definition
 */
#define MAC_STATUS_SUCCESS			(0x0) /**< Success */
#define MAC_STATUS_TIMEOUT			(0x1) /**< Time out */
#define MAC_STATUS_BUSY				(0x2) /**< Channel Busy */
#define MAC_STATUS_CRCFAIL			(0x3) /**< CRC Failed */
#define MAC_STATUS_NOACK			(0x5) /**< No ACK */
#define MAC_STATUS_UNLOCK			(0x6) /**< PLL unlocked */
#define MAC_STATUS_BADSTART			(0x7) /**< Bad Start */
#define MAC_STATUS_RXACK_PENDING	(0x8) /**< ACK frame was received with the Pending bit set */
#define MAC_STATUS_TXACK_PENDING	(0x9) /**< ACK frame was transmitted with the Pending bit set */
#define MAC_STATUS_FAIL_FILTER		(0xA) /**< One or more frame filtering tests has failed */
#define MAC_STATUS_PANID_CONFLICT	(0xB) /**< A PANID conflict has been detected */
#define MAC_STATUS_NOTCOMPLETE		(0xF) /**< Not complete */

/**
 * MAC sequence modes
*/
#define MAC_SEQUENCE_NOP		(0x0) /**< NOP */
#define MAC_SEQUENCE_RX			(0x3) /**< RX */
#define MAC_SEQUENCE_TX			(0x4) /**< TX */
#define MAC_SEQUENCE_ED			(0x5) /**< ED */
#define MAC_SEQUENCE_CCA		(0x6) /**< CCA */

/**
 * MAC Interrupt enable / disable
*/
#define MAC_IRQ_NONE			(0x0) /**< No IRQ */
#define MAC_IRQ_COMPLETE		(0x1) /**< Event-complete IRQ */
#define MAC_IRQ_EVENT_STARTED	(0x2) /**< Event-started IRQ */
#define MAC_IRQ_DATA			(0x4) /**< Data-arrived IRQ */
#define MAC_IRQ_FRAME_STARTED	(0x8) /**< Frame-started IRQ */
#define MAC_IRQ_PACKET_FAIL 	(0x10) /**< Failed-packet IRQ */
#define MAC_IRQ_FRAME_MATCH	    (0x20) /**< Frame-match IRQ (indicating matching process is done) */
#define MAC_IRQ_ALL				(0x3F) /**< All IRQs */

#define MAC_RSSI_TO_ED 0
#define MAC_RSSI_TO_LQI 1

#define MAC_RF_TRX_OFF 0
#define MAC_RF_RX_ON 1
#define MAC_RF_TX_ON 2
#define MAC_RF_ED_SCAN 3

static int8_t rf_radio_driver_id = -1;

static int8_t rf_interface_state_control(phy_interface_state_e new_state, uint8_t rf_channel);
static int8_t rf_start_cca(uint8_t *data_ptr, uint16_t data_length, uint8_t tx_handle, data_protocol_e data_protocol);
static int8_t rf_address_write(phy_address_type_e address_type, uint8_t *address_ptr);
static int8_t rf_extension(phy_extension_type_e extension_type, uint8_t *data_ptr);

static void rf_mac_hw_init(void);
static void rf_mac_timers_disable_trig_event(void);
static void rf_mac_reset(void);
static void rf_mac_rx_enable(void);
static void rf_mac_ed_state_enable(void);
static uint8_t rf_mac_convert_rssi(uint8_t scale);
static int8_t rf_mac_get_rssi(void);
static void rf_mac_set_rx_on_state(bool enable);
static void rf_mac_write(uint8_t *data_ptr, uint8_t length);

static void rf_mac_set_pending(uint8_t status);

static void rf_mac_set_shortAddress(uint8_t* valueAddress);
static void rf_mac_set_panId(uint8_t* valueAddress);
static void rf_mac_set_mac64(const uint8_t* valueAddress);

static void rf_mac_get_mac64(uint8_t* valueAddress);

static int8_t set_channel(uint8_t channel);
static void handle_IRQ_events(void);

static uint8_t MAC64_addr_default[8] = {1, 2, 3, 4, 5, 6, 7, 8};
static uint8_t MAC64_addr[8];

static uint8_t rf_mac_state = MAC_RF_TRX_OFF;
static bool rf_ack_pending_state = false;
static bool rf_mac_ack_requsted = false;
static uint8_t rf_mac_handle;
volatile uint8_t rf_ed_value = 0;

static NanostackRfPhyNcs36510 *rf = NULL;

#define MAC_PACKET_SIZE 127 //MAX MAC payload is 127 bytes
static uint8_t PHYPAYLOAD[MAC_PACKET_SIZE];

//TODO: verify these values
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
    (char*)"ON Semi ncs36510",
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
        osEvent event = rf_thread.signal_wait(0);
        if (event.status != osEventSignal) {
            continue;
        }

        platform_enter_critical();
        if (event.value.signals & SIGNAL_COUNT_RADIO) {
            handle_IRQ_events();
        }
        platform_exit_critical();
        NVIC_ClearPendingIRQ(MacHw_IRQn);
        NVIC_EnableIRQ(MacHw_IRQn);
    }
}

static int8_t rf_device_register(void)
{
    if( rf_radio_driver_id < 0 ) {

        rf_mac_hw_init();
        /**
         * Read factory stored Mac address to RAM
         */
        common_write_32_bit(MACHWREG->LONG_ADDRESS_HIGH, MAC64_addr);
        common_write_32_bit(MACHWREG->LONG_ADDRESS_LOW, MAC64_addr + 4);
        rf_radio_driver_id = arm_net_phy_register(&device_driver);
    }

    return rf_radio_driver_id;
}

static void rf_device_unregister(void)
{
    arm_net_phy_unregister(rf_radio_driver_id);
}

void rf_read_mac_address(uint8_t *address_ptr)
{
    platform_enter_critical();
    rf_mac_get_mac64(address_ptr);
    platform_exit_critical();
}

int8_t rf_read_random(void)
{
    //TODO: Read random from randomizer
    return 1;
}

void rf_set_mac_address(const uint8_t *ptr)
{
    platform_enter_critical();
    rf_mac_set_mac64(ptr);
    platform_exit_critical();
}

static void rf_mac_set_pending(uint8_t status) {

    if (status) {
        MACHWREG->OPTIONS.BITS.TFPO = 0;
        MACHWREG->OPTIONS.BITS.TFP = 1;
        rf_ack_pending_state = true;
    } else {
        rf_ack_pending_state = false;
        MACHWREG->OPTIONS.BITS.TFPO = 0;
        MACHWREG->OPTIONS.BITS.TFP = 0;
    }
}

static int8_t rf_interface_state_control(phy_interface_state_e new_state, uint8_t rf_channel)
{
    platform_enter_critical();
    switch(new_state){
        case PHY_INTERFACE_RESET: {           /**< Reset PHY driver and set to idle. */
            rf_mac_set_rx_on_state(false);
            break;
            }
        case PHY_INTERFACE_DOWN: {             /**< Disable PHY interface driver (RF radio disable). */
            rf_mac_set_rx_on_state(false);
            break;
            }
        case PHY_INTERFACE_UP: {              /**< Enable PHY interface driver (RF radio receiver ON). */
            set_channel(rf_channel);
            rf_mac_set_rx_on_state(true);

            break;
            }
        case PHY_INTERFACE_RX_ENERGY_STATE: { /**< Enable wirless interface ED scan mode. */
            rf_ed_value = 0;
            set_channel(rf_channel);
            rf_mac_set_rx_on_state(false);
            rf_mac_ed_state_enable();
            break;
            }
        case PHY_INTERFACE_SNIFFER_STATE: {
            set_channel(rf_channel);
            rf_mac_set_rx_on_state(true);
            break;
            }
    }
    platform_exit_critical();
    return 0;
}

static int8_t rf_start_cca(uint8_t *data_ptr, uint16_t data_length, uint8_t tx_handle, data_protocol_e data_protocol)
{
    platform_enter_critical();
    (void)data_protocol;
    rf_mac_handle = tx_handle;
    rf_mac_write(data_ptr, data_length);
    platform_exit_critical();
    return 0;
}

static int8_t rf_address_write(phy_address_type_e address_type, uint8_t *address_ptr)
{
    int ret_value = 0;
    platform_enter_critical();
    switch (address_type) {
    case PHY_MAC_64BIT: /**< RF/PLC link layer address. */
        rf_mac_set_mac64(address_ptr);
        break;
    case PHY_MAC_16BIT: /**< RF interface short address. */
        rf_mac_set_shortAddress(address_ptr);
        break;
    case PHY_MAC_PANID: /**< RF interface 16-Bit PAN-ID. */
        rf_mac_set_panId(address_ptr);
        break;
    default:
        ret_value = -1;
    }
    platform_exit_critical();

    return ret_value;
}

static int8_t rf_extension(phy_extension_type_e extension_type, uint8_t *data_ptr)
{
    int ret_value = 0;
    platform_enter_critical();
    switch (extension_type) {
        case PHY_EXTENSION_CTRL_PENDING_BIT: /**< Control MAC pending bit for indirect data. */
            rf_mac_set_pending(*data_ptr);
            break;
        case PHY_EXTENSION_READ_LAST_ACK_PENDING_STATUS: /**< Read status if the last ACK is still pending. */
            *data_ptr = rf_ack_pending_state;
            break;
        case PHY_EXTENSION_SET_CHANNEL:  /**< Net library channel set. */
            return set_channel(*data_ptr);

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

static int8_t set_channel(uint8_t channel)
{
    if( channel > 10 && channel < 27 ){
        fRfAnaIoctl(SET_RF_CHANNEL, &channel);
        return 0;
    }
    return -1;
}


/**
 *  SET MAC 16 address to Register
 */
static void rf_mac_set_shortAddress(uint8_t* valueAddress) {
    MACHWREG->SHORT_ADDRESS = common_read_16_bit(valueAddress);
}

/**
 *  SET PAN-ID to Register
 */
static void rf_mac_set_panId(uint8_t* valueAddress) {
    MACHWREG->PANID = common_read_16_bit(valueAddress);
}

/**
 *  SET MAC64 address to register
 */
static void rf_mac_set_mac64(const uint8_t* valueAddress) {
    MACHWREG->LONG_ADDRESS_HIGH = common_read_32_bit(valueAddress);
    valueAddress += 4;
    MACHWREG->LONG_ADDRESS_LOW = common_read_32_bit(valueAddress);
}

static void rf_mac_get_mac64(uint8_t* valueAddress) {
    valueAddress = common_write_32_bit(MACHWREG->LONG_ADDRESS_HIGH, valueAddress);
    common_write_32_bit(MACHWREG->LONG_ADDRESS_LOW, valueAddress);
}

static void rf_mac_timers_disable_trig_event(void) {
    MACHWREG->TIMER_DISABLE.BITS.START = true;
    MACHWREG->TIMER_DISABLE.BITS.STOP = true;
    MACHWREG->SEQ_OPTIONS.BITS.NOW = true;
}

/**
 * Call this only One time
 */
static void rf_mac_hw_init(void) {
    uint32_t periphClockfrequency;
    uint8_t lutIndex;
    volatile uint8_t *pMatchReg = MACMATCHREG;

    /** Initialize rf peripheral */
    fRfAnaInit();

    /** Enable mac clock */
    CLOCK_ENABLE(CLOCK_MACHW);

    /** Disable and clear IRQs */
    MACHWREG->MASK_IRQ.WORD = MAC_IRQ_NONE;
    MACHWREG->CLEAR_IRQ.WORD = MAC_IRQ_ALL;
    NVIC_ClearPendingIRQ(MacHw_IRQn);

    /** Set sequence options */
    MACHWREG->SEQ_OPTIONS.BITS.MODE = 0x1;
    MACHWREG->SEQ_OPTIONS.BITS.NOACK = false;
    MACHWREG->SEQ_OPTIONS.BITS.NOW = true;
    MACHWREG->SEQ_OPTIONS.BITS.PRM = false;

    MACHWREG->SEQ_OPTIONS.BITS.ACK_ENABLE = false;
    MACHWREG->SEQ_OPTIONS.BITS.RES_ENABLE = false;

    /** Set clocks */
    periphClockfrequency = fClockGetPeriphClockfrequency();
    MACHWREG->DIVIDER.BITS.BIT_CLOCK_DIVIDER = (periphClockfrequency / 250000) - 1;
    MACHWREG->DIVIDER.BITS.SYSTEM_CLOCK_DIVIDER = (periphClockfrequency / 1000000) - 1;
    MACHWREG->DIVIDER.BITS.CHIP_CLOCK_DIVIDER = (periphClockfrequency / 2000000) - 1;

    /** Set miscellaneous */

    /** This value should be tuned tuned to hit tx sw ack window (192us-204us) */
    MACHWREG->RX_TX_WARMPUPS.BITS.TRANSMIT_WARMPUP = 0x16;

    /** This value is selected to allocate 1 bit margin between tr sw ack and tx sw ack */
    MACHWREG->RX_TX_WARMPUPS.BITS.RECEIVE_WARMPUP = 0x15;

    MACHWREG->TXCCA = 0x30;
    MACHWREG->CCA.BITS.CCA_LENGTH = 0x43;
    MACHWREG->CCA.BITS.CCA_DELAY = 0x26;
    MACHWREG->TX_ACK_DELAY = 0x21;
    MACHWREG->ACK_STOP.BITS.RXACK_END = 0xA8;
    MACHWREG->SLOT_OFFSET.WORD = 0x00070007;
    MACHWREG->TX_LENGTH.BITS.TX_PRE_CHIPS = 0x6;
    MACHWREG->TX_FLUSH = 0x00000008; /** Transmit flush duration (8 x 4us = 32 us) */

    /** Set AGC */
    MACHWREG->AGC_CONTROL.WORD = 0x00000007; // AGC enabled / AGC freeze enabled / Preamble detection mode

    /** It is unclear from design specification if a 16MHz is mandatory for AGC operations or only to build
    * settle and measurements delays */
    if (periphClockfrequency == CPU_CLOCK_ROOT_HZ) {
        /** AGC time unit = T(PCLK) x 2 = 16MHz period */
        MACHWREG->AGC_SETTINGS.BITS.DIVIDER = 1;
        /** settle delay = (value + 1) * AGC time unit = 500ns targeted */
        MACHWREG->AGC_SETTINGS.BITS.SETTLE_DELAY = 7;
        /** measurement delay = (value + 1) * AGC time unit = 1500ns targeted */
        MACHWREG->AGC_SETTINGS.BITS.MEASURE_DELAY = 0x17;
    } else {
        /** AGC time unit is T(PCLK) */
        MACHWREG->AGC_SETTINGS.BITS.DIVIDER = 0;
        /** settle delay = (value + 1) * AGC time unit = 500ns targeted */
        MACHWREG->AGC_SETTINGS.BITS.SETTLE_DELAY = (16 / CPU_CLOCK_DIV) - 1;
        /** measurement delay = (value + 1) * AGC time unit = 1500ns targeted */
        MACHWREG->AGC_SETTINGS.BITS.MEASURE_DELAY = (48 / CPU_CLOCK_DIV) - 1;
    }

    /** AGC high threshold: 3dB below the clipping level */
    MACHWREG->AGC_SETTINGS.BITS.HIGH_THRESHOLD = 1;
    /** AGC low threshold: 9dB below the high threshold */
    MACHWREG->AGC_SETTINGS.BITS.LOW_THRESHOLD = 0;

    /** Set Demodulator */
    DMDREG->DMD_CONTROL0.WORD = 0x7FFF0004;
    MACHWREG->SHORT_ADDRESS = 0x0000ffff;
    MACHWREG->PANID = 0x0000ffff;

    /** Reset macHw peripheral */
    rf_mac_reset();

    /** Initialise LUT RAM */
    for (lutIndex=0;lutIndex<96;lutIndex++) {
      *(pMatchReg + lutIndex) = 0xFF;
    }
    osStatus status = rf_thread.start(mbed::callback(rf_thread_loop));
    MBED_ASSERT(status == osOK);

    /** Clear and enable MAC IRQ at task level, when scheduler is on. */
    NVIC_ClearPendingIRQ(MacHw_IRQn);
    NVIC_EnableIRQ(MacHw_IRQn);
}

static void rf_mac_set_rx_on_state(bool enable) {
    /** Abort ongoing sequence */
    rf_mac_reset();
    /** Start rx if requested */
    if (enable) {
        /** Set requested filtering */
        MACHWREG->SEQ_OPTIONS.BITS.BEA_ENABLE = true;
        MACHWREG->SEQ_OPTIONS.BITS.DATA_ENABLE = true;
        MACHWREG->SEQ_OPTIONS.BITS.CMD_ENABLE = true;
        /** Start receiver */
        rf_mac_rx_enable();
    }
}

static void rf_mac_write(uint8_t *data_ptr, uint8_t length) {
    uint8_t i;
    volatile uint8_t *txRam = MACTXREG;

    /* This is not make sense but... */
    rf_mac_reset();

    /* Set tx state */
    rf_mac_state = MAC_RF_TX_ON;

    if (*data_ptr & 0x20) {
        MACHWREG->SEQ_OPTIONS.BITS.ACK_ENABLE = true;
    } else {
        MACHWREG->SEQ_OPTIONS.BITS.ACK_ENABLE = false;
    }
    rf_mac_ack_requsted = MACHWREG->SEQ_OPTIONS.BITS.ACK_ENABLE;

    /* Set data length */
    MACHWREG->TX_LENGTH.BITS.TXLENGTH = length + 2;

    *txRam++ = *data_ptr++;
    *txRam++ = *data_ptr++;
    *txRam++ = *data_ptr;
    //RR: Retransmission for Data request should have same DSN
    MACHWREG->TX_SEQ_NUMBER = *data_ptr++;
    for (i = 3; i < length; i++) {
        *txRam++ = *data_ptr++;
    }

    MACHWREG->SEQ_OPTIONS.BITS.PRM = 0;

    MACHWREG->SEQ_OPTIONS.BITS.NOACK = false;

    rf_mac_state = MAC_RF_TX_ON;

    /* Start CCA immediately */
    rf_mac_timers_disable_trig_event();

    while (MACHWREG->TIMER != 0x0) MACHWREG->TIMER = 0x0; // HW ISSUE: field is not set immediately

    /* Enable tx irq, reset protocol timer and start tx sequence */
    MACHWREG->MASK_IRQ.WORD = MAC_IRQ_COMPLETE;
    MACHWREG->SEQUENCER = MAC_SEQUENCE_TX;

}


static void rf_mac_ed_state_enable(void) {
    rf_mac_state = MAC_RF_ED_SCAN;
    /** Enable Energy scan state and event complete interrupt */
    MACHWREG->CLEAR_IRQ.WORD = MAC_IRQ_COMPLETE;
    MACHWREG->MASK_IRQ.WORD = MAC_IRQ_COMPLETE;
    MACHWREG->SEQUENCER = MAC_SEQUENCE_ED;
}


static void rf_mac_rx_enable(void) {
    rf_mac_state = MAC_RF_RX_ON;

    /** Enable rx irqs, reset protocol timer and start rx sequence */
    MACHWREG->MASK_IRQ.WORD = MAC_IRQ_COMPLETE | MAC_IRQ_FRAME_MATCH | MAC_IRQ_DATA;
    while (MACHWREG->TIMER != 0x0) MACHWREG->TIMER = 0x0; // HW ISSUE: field is not set immediately
    MACHWREG->SEQUENCER = MAC_SEQUENCE_RX;
    return;
}


static void rf_mac_reset(void) {
    uint32_t macHwDivider;

    /** Recommended abort sequence (with synchronous reset) */

    /** 1. Set clock divider to minimum (for single clock response) */
    macHwDivider = MACHWREG->DIVIDER.WORD;
    /** (to cope with protocol timer and ed REVB silicon issues it is required
     * to set protocol timer to 1 and not to 0 as suggested in macHw specification) */
    /* PK !!!MAC_REVD RevB -> RevD change list Item 25: protocol timer */
    MACHWREG->DIVIDER.WORD = 1;

    /** 2. Disable interrupts */
    MACHWREG->MASK_IRQ.WORD = MAC_IRQ_NONE;

    /** 3. Clear interrupts */
    MACHWREG->CLEAR_IRQ.WORD = MAC_IRQ_ALL;

    NVIC_ClearPendingIRQ(MacHw_IRQn);

    /** 4. Clear previous sequence type (write no-op sequence) */
    MACHWREG->SEQUENCER = MAC_SEQUENCE_NOP;

    /** 5. Move all MAC state machines to idle state (on, with synchronous reset) */
    MACHWREG->CONTROL.WORD = 0x00000003;

    /** 6. Release reset */
    MACHWREG->CONTROL.WORD = 0x00000002;

    /** 7. Disable start, stop timers */
    rf_mac_timers_disable_trig_event();

    /** 8. Return clock dividers to original value */
    MACHWREG->DIVIDER.WORD = macHwDivider;

    MACHWREG->SEQ_OPTIONS.BITS.BEA_ENABLE = False;
    MACHWREG->SEQ_OPTIONS.BITS.DATA_ENABLE = False;
    MACHWREG->SEQ_OPTIONS.BITS.CMD_ENABLE = False;

    /** Set MAC_HW state */
    rf_mac_state = MAC_RF_TRX_OFF;
}

static uint8_t rf_mac_convert_rssi(uint8_t scale) {
    /* RSSI Value: The value is captured at the end of packet reception or at the end of ED/CCA
     * measurements and is interpreted in dBm as follows:
     * 1xxxxxxx:  not used
     * 01111111:   0 dBm (or above)
     * 01111110:  -1 dBm
     * 01111101:  -2 dBm
     * -
     * 00000010:  -125 dBm
     * 00000001:  -126 dBm
     * 00000000:  -127 dBm (or below)
     */

    /* check rssi is well in spec range */
    //ASSERT ((DMDREG->DMD_STATUS.BITS.RSSI_VALUE & 0x80) != 0x80);
    if (DMDREG->DMD_STATUS.BITS.RSSI_VALUE & 0x80) {
        return 0;
    }

    /* convert rssi in sign char: translate 01111111 into 0 and following alike, make negative */
    signed char rssi_value = -1 * (DMDREG->DMD_STATUS.BITS.RSSI_VALUE ^ 0x7F);

    if (scale == MAC_RSSI_TO_ED ) {
        /**
         * For ED (IEEE 6.9.7) "The ED result shall be reported to the MLME as an 8 bit integer
         * ranging from 0x00 to 0xff. The minimum ED value (zero) shall indicate received power less than 10 dB
         * above the specified receiver sensitivity (see 6.5.3.3 and 6.6.3.4), and the range of received power spanned by
         *  the ED values shall be at least 40 dB. Within this range, the mapping from the received power in decibels to
         * ED value shall be linear with an accuracy of � 6 dB."
         * (-85dBm receiver sensitivity will be targeted => zero ED value is associated to -75dBm)
         * (span will have 51dBm range from 0x0=-75dBm to 0xff=-24dBm)
         */

        /* Clip maximal and minimal rssi value reported by ED */
        if (rssi_value < -75) rssi_value = -75;
        if (rssi_value > -24) rssi_value = -24;

        /* scale the span -75dBm --> -24dBm to 0x00 --> 0xFF
         * Attention:  This scaling implies that granularity of the result is changing from 1dBm per unit to 1/5 dBm per unit
         *   0xFF:              -24 dBm (or above)
         *   0xFE - 0xFB:       (impossible code)
         *   0xFA:              -25 dBm
         *   0xF9 - 0xF6:       (impossible code)
         *   0xF5:              -26 dBm
         *   ...
         *   0x05:              -74 dBm
         *   0x01 - 0x04:       (impossible code)
         *   0x00:              -75 dBm (or below)
         */
        return (rssi_value + 75) * 5;
    } else {

        /**
         * For LQI: (IEEE 6.9.7) "The LQI measurement shall be performed for each received packet, and the result shall be reported to the
         * MAC sublayer using PD-DATA.indication (see 6.2.1.3) as an integer ranging from 0x00 to 0xff. The
         * minimum and maximum LQI values (0x00 and 0xff) should be associated with the lowest and highest
         * quality compliant signals detectable by the receiver, and LQI values in between should be uniformly
         * distributed between these two limits. At least eight unique values of LQI shall be used."
         * (-85dBm sensitivity will be targeted => zero LQI value will be associated to -85dBm)
         * (span will have 64dBm range from 0x0=-85dBm to 0xff=-21dBm)
         */

        /* Clip maximal and minimal rssi value reported by LQI */
        if (rssi_value < -85) rssi_value = -85;
        if (rssi_value > -21) rssi_value = -21;

        /* scale the span -85dBm --> -21,25dBm to 0x00 --> 0xFF
         * Attention:  This scaling implies that granularity of the result is changing from 1dBm per unit to 1/4 dBm per unit
         *   0xFF: -21,25 dBm (or above)
         *   0xFE: (impossible code)
         *   0xFD: (impossible code)
         *   0xFC: -22 dBm
         *   0xFB: (impossible code)
         *   0xFA: (impossible code)
         *   0xF9: (impossible code)
         *   0xF8: -23 dBm
         *   ...
         *   0x05: (impossible code)
         *   0x04: - 84dBm
         *   0x03: (impossible code)
         *   0x02: (impossible code)
         *   0x01: (impossible code)
         *   0x00: - 85dBm
         */
        if (rssi_value == -21)
            return ((rssi_value + 85) * 4) - 1;
        else
            return (rssi_value + 85) * 4;
    }
}

static int8_t rf_mac_get_rssi(void) {
    int8_t rssi_value = -1 * (DMDREG->DMD_STATUS.BITS.RSSI_VALUE ^ 0x7F);
    return rssi_value;
}

static void rf_rx_ed_scan_interrupt() {
    MACHWREG->CLEAR_IRQ.WORD = MAC_IRQ_COMPLETE;
    if (MACHWREG->STATUS.BITS.CODE == MAC_STATUS_SUCCESS) {
        uint8_t ed = rf_mac_convert_rssi(MAC_RSSI_TO_ED);
        if (ed) {
            if (ed > rf_ed_value) {
                rf_ed_value = ed;
            }
        }
    }

    MACHWREG->MASK_IRQ.WORD = MAC_IRQ_COMPLETE;
    MACHWREG->SEQUENCER = MAC_SEQUENCE_ED;
}

static void rf_rx_interrupt() {
    // Frame match is used for association and data frames
    uint8_t seqSts = MACHWREG->STATUS.BITS.CODE;

    if (MACHWREG->IRQ_STATUS.BITS.FM) {
        if (!rf_ack_pending_state) {
            MACHWREG->OPTIONS.BITS.TFP = 0;
            MACHWREG->OPTIONS.BITS.TFPO = 1;
        } else {
            MACHWREG->OPTIONS.BITS.TFP = 1;
            MACHWREG->OPTIONS.BITS.TFPO = 1;
        }

        MACHWREG->CLEAR_IRQ.WORD = MAC_IRQ_FRAME_MATCH;
        return;
    }

    /** RR: Process the event complete IRQ */
    if (MACHWREG->IRQ_STATUS.BITS.EC || MACHWREG->IRQ_STATUS.BITS.DATA) {
        /** Clear the event */
        if (MACHWREG->IRQ_STATUS.BITS.EC) {
            MACHWREG->CLEAR_IRQ.WORD = MAC_IRQ_COMPLETE;
        }

        if (MACHWREG->IRQ_STATUS.BITS.DATA) {
            MACHWREG->CLEAR_IRQ.WORD = MAC_IRQ_DATA;
        }

        /** Build frame (containing received frame or timeout) */

        volatile uint8_t *rxRam = MACRXREG;
        uint8_t length;
        int8_t rssi;
        uint8_t lqi;

        /** Return directly in case of timeout */
        if (seqSts == MAC_STATUS_TIMEOUT) {
            /* Initialize frame status */
            return;
        }

        length = *rxRam++;
        if (length < 5){
            rf_mac_rx_enable();
            return;
        }
        length -= 2; //Cut CRC OUT

        /* Initialize frame status */
        for (uint8_t i=0; i < length; i++) {
            PHYPAYLOAD[i] = *rxRam++;
        }

        lqi = rf_mac_convert_rssi(MAC_RSSI_TO_LQI);
        rssi = rf_mac_get_rssi();
        rf_mac_rx_enable();
        //Call ARM API
        if( device_driver.phy_rx_cb ){
            device_driver.phy_rx_cb(PHYPAYLOAD, length, lqi, rssi, rf_radio_driver_id);
        }
    }
}

static void rf_mac_tx_interrupt(void)
{
    phy_link_tx_status_e status;
    /** Clear the event complete IRQ */
    MACHWREG->CLEAR_IRQ.WORD = MAC_IRQ_COMPLETE;
    /* This IRQ means that Data Request is complete; check the status */
    uint8_t sharedSeqSts = MACHWREG->STATUS.BITS.CODE;

    rf_mac_set_rx_on_state(true);

    switch (sharedSeqSts) {
    case MAC_STATUS_SUCCESS: /* Positive */
        //SET Success
        if (rf_mac_ack_requsted) {
            status = PHY_LINK_TX_DONE;
        } else {
            status = PHY_LINK_TX_SUCCESS;
        }
        break;

    case MAC_STATUS_RXACK_PENDING:  /* Positive for broadcast */
        status = PHY_LINK_TX_DONE_PENDING;
        break;


    case MAC_STATUS_BUSY:
        status = PHY_LINK_CCA_FAIL;
        break;

    default:
        status = PHY_LINK_TX_FAIL;
        break;
    }
    rf_mac_ack_requsted = false;
     //Call RX TX complete
    if( device_driver.phy_tx_done_cb ) {
        device_driver.phy_tx_done_cb(rf_radio_driver_id, rf_mac_handle, status, 1, 1);
    }
}

/**
* RF MAC Interrupt handler
*/
extern "C" void fIrqMacHwHandler(void)
{
    NVIC_DisableIRQ(MacHw_IRQn);
    rf_thread.signal_set(SIGNAL_COUNT_RADIO);
}

static void handle_IRQ_events(void)
{
    /** Set MAC timers to initial state */
    MACHWREG->TIMER_ENABLE.BITS.START = false;
    MACHWREG->TIMER_ENABLE.BITS.STOP = false;
    MACHWREG->TIMER_DISABLE.BITS.START = false;
    MACHWREG->TIMER_DISABLE.BITS.STOP = false;

    /** Disarm start/stop timers, disable and clear irq (event_complete) */
    rf_mac_timers_disable_trig_event();
    /** REVD changes to sequence tracking register. Sequence register can be used instead of rf_mac_state */

    if (rf_mac_state == MAC_RF_RX_ON) {
        rf_rx_interrupt();
    } else if(rf_mac_state == MAC_RF_TX_ON) {
        rf_mac_tx_interrupt();
    } else if (rf_mac_state == MAC_RF_ED_SCAN){
        rf_rx_ed_scan_interrupt();
    } else {
        /** Clear the event complete IRQ */
        MACHWREG->CLEAR_IRQ.WORD = MAC_IRQ_COMPLETE;
        uint8_t sharedSeqSts = MACHWREG->STATUS.BITS.CODE;
    }
}

NanostackRfPhyNcs36510::NanostackRfPhyNcs36510()
{
    memcpy(MAC64_addr, MAC64_addr_default, sizeof(MAC64_addr));
}

NanostackRfPhyNcs36510::~NanostackRfPhyNcs36510()
{
    // Do nothing
}

int8_t NanostackRfPhyNcs36510::rf_register()
{
    platform_enter_critical();

    if (rf != NULL) {
        platform_exit_critical();
        error("Multiple registrations of NanostackRfPhyNcs36510 not supported");
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

void NanostackRfPhyNcs36510::rf_unregister()
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

void NanostackRfPhyNcs36510::get_mac_address(uint8_t *mac)
{
    platform_enter_critical();

    memcpy((void*)mac, (void*)MAC64_addr, sizeof(MAC64_addr));

    platform_exit_critical();
}

void NanostackRfPhyNcs36510::set_mac_address(uint8_t *mac)
{
    platform_enter_critical();

    if (NULL != rf) {
        error("NanostackRfPhyNcs36510 cannot change mac address when running");
        platform_exit_critical();
        return;
    }
    memcpy((void*)MAC64_addr, (void*)mac, sizeof(MAC64_addr));

    platform_exit_critical();
}

