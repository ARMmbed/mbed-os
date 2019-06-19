/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
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

#if defined(MBED_CONF_NANOSTACK_CONFIGURATION) && DEVICE_SPI && DEVICE_INTERRUPTIN && defined(MBED_CONF_RTOS_PRESENT)

#include "NanostackRfPhyMcr20a.h"
#include "ns_types.h"
#include "platform/arm_hal_interrupt.h"
#include "nanostack/platform/arm_hal_phy.h"
#include <string.h>
#include "rtos.h"
#include "mbed_interface.h"
#include "platform/mbed_error.h"

using namespace mbed;
using namespace rtos;

/* Freescale headers which are for C files */
extern "C" {
#include "MCR20Drv.h"
#include "MCR20Reg.h"
#include "MCR20Overwrites.h"
}


#define RF_BUFFER_SIZE 128

/*Radio RX and TX state definitions*/
#define RFF_ON 0x01
#define RFF_RX 0x02
#define RFF_TX 0x04
#define RFF_CCA 0x08

#define RF_MODE_NORMAL  0
#define RF_MODE_SNIFFER 1

#define RF_CCA_THRESHOLD 75 /* -75 dBm */

#define RF_TX_POWER_MAX 0

/* PHY constants in symbols */
#define gPhyWarmUpTime_c       9
#define gPhySHRDuration_c     10
#define gPhySymbolsPerOctet_c  2
#define gPhyAckWaitDuration_c 54

#define gCcaED_c               0
#define gCcaCCA_MODE1_c        1

#define gXcvrRunState_d       gXcvrPwrAutodoze_c
#if !defined(TARGET_KW24D)
#define gXcvrLowPowerState_d  gXcvrPwrHibernate_c
#else
#define gXcvrLowPowerState_d  gXcvrPwrAutodoze_c
#endif

namespace {

/* MCR20A XCVR states */
typedef enum xcvrState_tag {
    gIdle_c,
    gRX_c,
    gTX_c,
    gCCA_c,
    gTR_c,
    gCCCA_c,
} xcvrState_t;

/* MCR20A XCVR low power states */
typedef enum xcvrPwrMode_tag {
    gXcvrPwrIdle_c,
    gXcvrPwrAutodoze_c,
    gXcvrPwrDoze_c,
    gXcvrPwrHibernate_c
} xcvrPwrMode_t;


/*RF Part Type*/
typedef enum {
    FREESCALE_UNKNOW_DEV = 0,
    FREESCALE_MCR20A
} rf_trx_part_e;

/*Atmel RF states*/
typedef enum {
    NOP = 0x00,
    BUSY_RX = 0x01,
    RF_TX_START = 0x02,
    FORCE_TRX_OFF = 0x03,
    FORCE_PLL_ON = 0x04,
    RX_ON = 0x06,
    TRX_OFF = 0x08,
    PLL_ON = 0x09,
    BUSY_RX_AACK = 0x11,
    SLEEP = 0x0F,
    RX_AACK_ON = 0x16,
    TX_ARET_ON = 0x19
} rf_trx_states_t;

} // anonymous namespace

/*RF receive buffer*/
static uint8_t rf_buffer[RF_BUFFER_SIZE];

/* TX info */
static uint8_t  radio_tx_power = 0x17; /* 0 dBm */
static uint8_t  mac_tx_handle = 0;
static uint8_t  need_ack = 0;
static uint16_t tx_len = 0;

/* RF driver data */
static xcvrState_t mPhySeqState;
static xcvrPwrMode_t mPwrState;
static phy_device_driver_s device_driver;
static uint8_t mStatusAndControlRegs[8];
static uint8_t rf_rnd = 0;
static int8_t  rf_radio_driver_id = -1;
static uint8_t MAC_address[8];

/* Driver instance handle and hardware */
static NanostackRfPhyMcr20a *rf = NULL;
static SPI *spi = NULL;
static DigitalOut *cs = NULL;
static DigitalOut *rst = NULL;
static InterruptIn *irq = NULL;
static DigitalIn *irq_pin = NULL;
static Thread *irq_thread = NULL;

/* Channel info */                 /* 2405    2410    2415    2420    2425    2430    2435    2440    2445    2450    2455    2460    2465    2470    2475    2480 */
static const uint8_t  pll_int[16] =  {0x0B,   0x0B,   0x0B,   0x0B,   0x0B,   0x0B,   0x0C,   0x0C,   0x0C,   0x0C,   0x0C,   0x0C,   0x0D,   0x0D,   0x0D,   0x0D};
static const uint16_t pll_frac[16] = {0x2800, 0x5000, 0x7800, 0xA000, 0xC800, 0xF000, 0x1800, 0x4000, 0x6800, 0x9000, 0xB800, 0xE000, 0x0800, 0x3000, 0x5800, 0x8000};

/* Channel configurations for 2.4 */
static const phy_rf_channel_configuration_s phy_24ghz = {.channel_0_center_frequency = 2405000000U, .channel_spacing = 5000000U, .datarate = 250000U, .number_of_channels = 16U, .modulation = M_OQPSK};

static const phy_device_channel_page_s phy_channel_pages[] = {
    { CHANNEL_PAGE_0, &phy_24ghz},
    { CHANNEL_PAGE_0, NULL}
};


static rf_trx_part_e rf_radio_type_read(void);

MBED_UNUSED static void rf_ack_wait_timer_start(uint16_t slots);
MBED_UNUSED static void rf_ack_wait_timer_stop(void);
MBED_UNUSED static void rf_handle_cca_ed_done(void);
MBED_UNUSED static void rf_handle_tx_end(void);
MBED_UNUSED static void rf_handle_rx_end(void);
MBED_UNUSED static void rf_on(void);
MBED_UNUSED static void rf_receive(void);
MBED_UNUSED static void rf_poll_trx_state_change(rf_trx_states_t trx_state);
MBED_UNUSED static void rf_init(void);
MBED_UNUSED static void rf_set_mac_address(const uint8_t *ptr);
MBED_UNUSED static int8_t rf_device_register(void);
MBED_UNUSED static void rf_device_unregister(void);
MBED_UNUSED static int8_t rf_start_cca(uint8_t *data_ptr, uint16_t data_length, uint8_t tx_handle, data_protocol_e data_protocol);
MBED_UNUSED static void rf_cca_abort(void);
MBED_UNUSED static void rf_read_mac_address(uint8_t *ptr);
MBED_UNUSED static int8_t rf_read_random(void);
MBED_UNUSED static void rf_calibration_cb(void);
MBED_UNUSED static void rf_init_phy_mode(void);
MBED_UNUSED static void rf_ack_wait_timer_interrupt(void);
MBED_UNUSED static void rf_calibration_timer_interrupt(void);
MBED_UNUSED static void rf_calibration_timer_start(uint32_t slots);
MBED_UNUSED static void rf_cca_timer_interrupt(void);
MBED_UNUSED static void rf_cca_timer_start(uint32_t slots);
MBED_UNUSED static uint16_t rf_get_phy_mtu_size(void);
MBED_UNUSED static uint8_t rf_scale_lqi(int8_t rssi);

/**
 *  RF output power write
 *
 * \brief TX power has to be set before network start.
 *
 * \param power
 *              See datasheet for TX power settings
 *
 * \return 0, Supported Value
 * \return -1, Not Supported Value
 */
MBED_UNUSED static int8_t rf_tx_power_set(uint8_t power);
MBED_UNUSED static uint8_t rf_tx_power_get(void);
MBED_UNUSED static int8_t rf_enable_antenna_diversity(void);

/* Private functions */
MBED_UNUSED static void    rf_abort(void);
MBED_UNUSED static void    rf_promiscuous(uint8_t mode);
MBED_UNUSED static void    rf_get_timestamp(uint32_t *pRetClk);
MBED_UNUSED static void    rf_set_timeout(uint32_t *pEndTime);
MBED_UNUSED static void    rf_set_power_state(xcvrPwrMode_t newState);
MBED_UNUSED static uint8_t rf_if_read_rnd(void);
MBED_UNUSED static uint8_t rf_convert_LQI(uint8_t hwLqi);
MBED_UNUSED static uint8_t rf_get_channel_energy(void);
MBED_UNUSED static uint8_t rf_convert_energy_level(uint8_t energyLevel);
MBED_UNUSED static int8_t  rf_convert_LQI_to_RSSI(uint8_t lqi);
MBED_UNUSED static int8_t  rf_interface_state_control(phy_interface_state_e new_state, uint8_t rf_channel);
MBED_UNUSED static int8_t  rf_extension(phy_extension_type_e extension_type, uint8_t *data_ptr);
MBED_UNUSED static int8_t  rf_address_write(phy_address_type_e address_type, uint8_t *address_ptr);
static void PHY_InterruptThread(void);
static void handle_interrupt(void);


/*
 * \brief Read connected radio part.
 *
 * This function only return valid information when rf_init() is called
 *
 * \return
 */
static rf_trx_part_e rf_radio_type_read(void)
{
    return FREESCALE_MCR20A;
}

/*
 * \brief Function initialises and registers the RF driver.
 *
 * \param none
 *
 * \return rf_radio_driver_id Driver ID given by NET library
 */
static int8_t rf_device_register(void)
{
    rf_trx_part_e radio_type;

    rf_init();



    radio_type = rf_radio_type_read();
    if (radio_type == FREESCALE_MCR20A) {
        /*Set pointer to MAC address*/
        device_driver.PHY_MAC = MAC_address;
        device_driver.driver_description = (char *)"FREESCALE_MAC";

        //Create setup Used Radio chips
        /*Type of RF PHY is SubGHz*/
        device_driver.link_type = PHY_LINK_15_4_2_4GHZ_TYPE;

        device_driver.phy_channel_pages = phy_channel_pages;
        /*Maximum size of payload is 127*/
        device_driver.phy_MTU = 127;
        /*No header in PHY*/
        device_driver.phy_header_length = 0;
        /*No tail in PHY*/
        device_driver.phy_tail_length = 0;
        /*Set address write function*/
        device_driver.address_write = &rf_address_write;
        /*Set RF extension function*/
        device_driver.extension = &rf_extension;
        /*Set RF state control function*/
        device_driver.state_control = &rf_interface_state_control;
        /*Set transmit function*/
        device_driver.tx = &rf_start_cca;
        /*Upper layer callbacks init to NULL*/
        device_driver.phy_rx_cb = NULL;
        device_driver.phy_tx_done_cb = NULL;
        /*Virtual upper data callback init to NULL*/
        device_driver.arm_net_virtual_rx_cb = NULL;
        device_driver.arm_net_virtual_tx_cb = NULL;

        /*Register device driver*/
        rf_radio_driver_id = arm_net_phy_register(&device_driver);
    }

    return rf_radio_driver_id;
}

/*
 * \brief Function unregisters the RF driver.
 *
 * \param none
 *
 * \return none
 */
static void rf_device_unregister(void)
{
    arm_net_phy_unregister(rf_radio_driver_id);
}

/*
 * \brief Function returns the generated 8-bit random value for seeding Pseudo-random generator.
 *
 * \param none
 *
 * \return random value
 */
static int8_t rf_read_random(void)
{
    return rf_rnd;
}

/*
 * \brief Function is a call back for ACK wait timeout.
 *
 * \param none
 *
 * \return none
 */
static void rf_ack_wait_timer_interrupt(void)
{
    /* The packet was transmitted successfully, but no ACK was received */
    if (device_driver.phy_tx_done_cb) {
        device_driver.phy_tx_done_cb(rf_radio_driver_id, mac_tx_handle, PHY_LINK_TX_SUCCESS, 1, 1);
    }
    rf_receive();
}

/*
 * \brief Function is a call back for calibration interval timer.
 *
 * \param none
 *
 * \return none
 */
static void rf_calibration_timer_interrupt(void)
{
}

/*
 * \brief Function is a call back for cca interval timer.
 *
 * \param none
 *
 * \return none
 */
static void rf_cca_timer_interrupt(void)
{
    /* CCA time-out handled by Hardware */
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
    uint32_t timeout;

    rf_get_timestamp(&timeout);
    timeout += time;
    rf_set_timeout(&timeout);
}

/*
 * \brief Function starts the calibration interval.
 *
 * \param slots Given slots, resolution 50us
 *
 * \return none
 */
static void rf_calibration_timer_start(uint32_t slots)
{
    (void)slots;
}

/*
 * \brief Function starts the CCA timout.
 *
 * \param slots Given slots, resolution 50us
 *
 * \return none
 */
static void rf_cca_timer_start(uint32_t slots)
{
    (void)slots;
}

/*
 * \brief Function stops the ACK wait timeout.
 *
 * \param none
 *
 * \return none
 */
static void rf_ack_wait_timer_stop(void)
{
}

/*
 * \brief Function reads the MAC address array.
 *
 * \param ptr Pointer to read array
 *
 * \return none
 */
static void rf_read_mac_address(uint8_t *ptr)
{
    memcpy(ptr, MAC_address, 8);
}

/*
 * \brief Function sets the MAC address array.
 *
 * \param ptr Pointer to given MAC address array
 *
 * \return none
 */
static void rf_set_mac_address(const uint8_t *ptr)
{
    memcpy(MAC_address, ptr, 8);
}

static uint16_t rf_get_phy_mtu_size(void)
{
    return device_driver.phy_MTU;
}

/*
 * \brief Function writes 16-bit address in RF address filter.
 *
 * \param short_address Given short address
 *
 * \return none
 */
static void rf_set_short_adr(uint8_t *short_address)
{
    /* Write one register at a time to be accessible from hibernate mode */
    MCR20Drv_IndirectAccessSPIWrite(MACSHORTADDRS0_MSB, short_address[0]);
    MCR20Drv_IndirectAccessSPIWrite(MACSHORTADDRS0_LSB, short_address[1]);
}

/*
 * \brief Function writes PAN Id in RF PAN Id filter.
 *
 * \param pan_id Given PAN Id
 *
 * \return none
 */
static void rf_set_pan_id(uint8_t *pan_id)
{
    /* Write one register at a time to be accessible from hibernate mode */
    MCR20Drv_IndirectAccessSPIWrite(MACPANID0_MSB, pan_id[0]);
    MCR20Drv_IndirectAccessSPIWrite(MACPANID0_LSB, pan_id[1]);
}

/*
 * \brief Function writes 64-bit address in RF address filter.
 *
 * \param address Given 64-bit address
 *
 * \return none
 */
static void rf_set_address(uint8_t *address)
{
    /* Write one register at a time to be accessible from hibernate mode */
    MCR20Drv_IndirectAccessSPIWrite(MACLONGADDRS0_0,  address[7]);
    MCR20Drv_IndirectAccessSPIWrite(MACLONGADDRS0_8,  address[6]);
    MCR20Drv_IndirectAccessSPIWrite(MACLONGADDRS0_16, address[5]);
    MCR20Drv_IndirectAccessSPIWrite(MACLONGADDRS0_24, address[4]);
    MCR20Drv_IndirectAccessSPIWrite(MACLONGADDRS0_32, address[3]);
    MCR20Drv_IndirectAccessSPIWrite(MACLONGADDRS0_40, address[2]);
    MCR20Drv_IndirectAccessSPIWrite(MACLONGADDRS0_48, address[1]);
    MCR20Drv_IndirectAccessSPIWrite(MACLONGADDRS0_56, address[0]);
}

/*
 * \brief Function sets the RF channel.
 *
 * \param ch New channel
 *
 * \return none
 */
static void rf_channel_set(uint8_t channel)
{
    MCR20Drv_DirectAccessSPIWrite(PLL_INT0, pll_int[channel - 11]);
    MCR20Drv_DirectAccessSPIMultiByteWrite(PLL_FRAC0_LSB, (uint8_t *) &pll_frac[channel - 11], 2);
}


/*
 * \brief Function initialises the radio driver and resets the radio.
 *
 * \param none
 *
 * \return none
 */
static void rf_init(void)
{
    uint32_t index;
    mPhySeqState = gIdle_c;
    mPwrState = gXcvrPwrIdle_c;
    /*Reset RF module*/
    MCR20Drv_RESET();
    /* Initialize the transceiver SPI driver */
    MCR20Drv_Init();
    /* Disable Tristate on MISO for SPI reads */
    MCR20Drv_IndirectAccessSPIWrite(MISC_PAD_CTRL, 0x02);
    /* Set XCVR clock output settings */
#if !defined(TARGET_KW24D)
    MCR20Drv_Set_CLK_OUT_Freq(gMCR20_ClkOutFreq_d);
#endif
    /* Set default XCVR power state */
    rf_set_power_state(gXcvrRunState_d);

    /* PHY_CTRL1 default HW settings  + AUTOACK enabled */
    mStatusAndControlRegs[PHY_CTRL1] = cPHY_CTRL1_AUTOACK;
    /* PHY_CTRL2 : mask all PP interrupts */
    mStatusAndControlRegs[PHY_CTRL2] = cPHY_CTRL2_CRC_MSK | \
                                       cPHY_CTRL2_PLL_UNLOCK_MSK | \
                                       /*cPHY_CTRL2_FILTERFAIL_MSK | */ \
                                       cPHY_CTRL2_RX_WMRK_MSK | \
                                       cPHY_CTRL2_CCAMSK | \
                                       cPHY_CTRL2_RXMSK | \
                                       cPHY_CTRL2_TXMSK | \
                                       cPHY_CTRL2_SEQMSK;
    /* PHY_CTRL3 : enable timer 3 and disable remaining interrupts */
    mStatusAndControlRegs[PHY_CTRL3] = cPHY_CTRL3_ASM_MSK    | \
                                       cPHY_CTRL3_PB_ERR_MSK | \
                                       cPHY_CTRL3_WAKE_MSK   | \
                                       cPHY_CTRL3_TMR3CMP_EN;
    /* PHY_CTRL4 unmask global TRX interrupts, enable 16 bit mode for TC2 - TC2 prime EN */
    mStatusAndControlRegs[PHY_CTRL4] = cPHY_CTRL4_TC2PRIME_EN | (gCcaCCA_MODE1_c << cPHY_CTRL4_CCATYPE_Shift_c);
    /* Clear all PP IRQ bits to avoid unexpected interrupts immediately after initialization */
    mStatusAndControlRegs[IRQSTS1] = cIRQSTS1_PLL_UNLOCK_IRQ | \
                                     cIRQSTS1_FILTERFAIL_IRQ | \
                                     cIRQSTS1_RXWTRMRKIRQ | \
                                     cIRQSTS1_CCAIRQ | \
                                     cIRQSTS1_RXIRQ | \
                                     cIRQSTS1_TXIRQ | \
                                     cIRQSTS1_SEQIRQ;

    mStatusAndControlRegs[IRQSTS2] = cIRQSTS2_ASM_IRQ | cIRQSTS2_PB_ERR_IRQ | cIRQSTS2_WAKE_IRQ;
    /* Mask and clear all TMR IRQs */
    mStatusAndControlRegs[IRQSTS3] = cIRQSTS3_TMR4MSK | cIRQSTS3_TMR3MSK | cIRQSTS3_TMR2MSK | cIRQSTS3_TMR1MSK | \
                                     cIRQSTS3_TMR4IRQ | cIRQSTS3_TMR3IRQ | cIRQSTS3_TMR2IRQ | cIRQSTS3_TMR1IRQ;
    /* Write settings to XCVR */
    MCR20Drv_DirectAccessSPIMultiByteWrite(PHY_CTRL1, &mStatusAndControlRegs[PHY_CTRL1], 5);
    /* Clear all interrupts */
    MCR20Drv_DirectAccessSPIMultiByteWrite(IRQSTS1, &mStatusAndControlRegs[IRQSTS1], 3);

    /*  RX_FRAME_FILTER. Accept FrameVersion 0 and 1 packets, reject all others */
    MCR20Drv_IndirectAccessSPIWrite(RX_FRAME_FILTER, (cRX_FRAME_FLT_FRM_VER | \
                                                      cRX_FRAME_FLT_BEACON_FT | \
                                                      cRX_FRAME_FLT_DATA_FT | \
                                                      cRX_FRAME_FLT_CMD_FT));
    /* Direct register overwrites */
    for (index = 0; index < sizeof(overwrites_direct) / sizeof(overwrites_t); index++) {
        MCR20Drv_DirectAccessSPIWrite(overwrites_direct[index].address, overwrites_direct[index].data);
    }
    /* Indirect register overwrites */
    for (index = 0; index < sizeof(overwrites_indirect) / sizeof(overwrites_t); index++) {
        MCR20Drv_IndirectAccessSPIWrite(overwrites_indirect[index].address, overwrites_indirect[index].data);
    }

    /* Set the CCA energy threshold value */
    MCR20Drv_IndirectAccessSPIWrite(CCA1_THRESH, RF_CCA_THRESHOLD);
    /* Set prescaller to obtain 1 symbol (16us) timebase */
    MCR20Drv_IndirectAccessSPIWrite(TMR_PRESCALE, 0x05);

    MCR20Drv_IRQ_Enable();

    /*Read random variable. This will be used when seeding pseudo-random generator*/
    rf_rnd = rf_if_read_rnd();
    /*Write initial eui64*/
    rf_set_address(MAC_address);
    /*set default channel to 11*/
    rf_channel_set(11);
    /*Start receiver*/
    rf_receive();
}

/**
 * \brief Function gets called when MAC is setting radio off.
 *
 * \param none
 *
 * \return none
 */
static void rf_off(void)
{
    /* Abort any ongoing sequences */
    rf_abort();
    /* Set XCVR in a low power state */
    rf_set_power_state(gXcvrLowPowerState_d);
}

/*
 * \brief Function polls the RF state until it has changed to desired state.
 *
 * \param trx_state RF state
 *
 * \return none
 */
static void rf_poll_trx_state_change(rf_trx_states_t trx_state)
{
    (void)trx_state;
}

/*
 * \brief Function starts the CCA process before starting data transmission and copies the data to RF TX FIFO.
 *
 * \param data_ptr Pointer to TX data
 * \param data_length Length of the TX data
 * \param tx_handle Handle to transmission
 * \return 0 Success
 * \return -1 Busy
 */
static int8_t rf_start_cca(uint8_t *data_ptr, uint16_t data_length, uint8_t tx_handle, data_protocol_e data_protocol)
{
    uint8_t ccaMode;

    /* Parameter validation */
    if (!data_ptr || (data_length > 125) || (PHY_LAYER_PAYLOAD != data_protocol)) {
        return -1;
    }

    if (mPhySeqState == gRX_c) {
        uint8_t phyReg = MCR20Drv_DirectAccessSPIRead(SEQ_STATE) & 0x1F;
        /* Check for an Rx in progress. */
        if ((phyReg <= 0x06) || (phyReg == 0x15) || (phyReg == 0x16)) {
            if (device_driver.phy_tx_done_cb) {
                device_driver.phy_tx_done_cb(rf_radio_driver_id, mac_tx_handle, PHY_LINK_CCA_FAIL, 1, 1);
            }
            return -1;
        }
        rf_abort();
    }

    /*Check if transmitter is busy*/
    if (mPhySeqState != gIdle_c) {
        /*Return busy*/
        return -1;
    }

    /*Store TX handle*/
    mac_tx_handle = tx_handle;
    /*Check if transmitted data needs to be acked*/
    need_ack = (*data_ptr & 0x20) == 0x20;

    /* Set XCVR power state in run mode */
    rf_set_power_state(gXcvrRunState_d);
    /* Load data into XCVR */
    tx_len = data_length + 2;
    MCR20Drv_PB_SPIBurstWrite(data_ptr - 1, data_length + 1);
    MCR20Drv_PB_SPIByteWrite(0, tx_len);

    /* Set CCA mode 1 */
    ccaMode = (mStatusAndControlRegs[PHY_CTRL4] >> cPHY_CTRL4_CCATYPE_Shift_c) & cPHY_CTRL4_CCATYPE;
    if (ccaMode != gCcaCCA_MODE1_c) {
        mStatusAndControlRegs[PHY_CTRL4] &= ~(cPHY_CTRL4_CCATYPE << cPHY_CTRL4_CCATYPE_Shift_c);
        mStatusAndControlRegs[PHY_CTRL4] |= gCcaCCA_MODE1_c << cPHY_CTRL4_CCATYPE_Shift_c;
        MCR20Drv_DirectAccessSPIWrite(PHY_CTRL4, mStatusAndControlRegs[PHY_CTRL4]);
    }

    /* Read XCVR registers */
    mStatusAndControlRegs[0] = MCR20Drv_DirectAccessSPIMultiByteRead(IRQSTS2, &mStatusAndControlRegs[1], 4);
    mStatusAndControlRegs[PHY_CTRL1] &= ~(cPHY_CTRL1_XCVSEQ);
    mStatusAndControlRegs[PHY_CTRL1] |= gCCA_c;
    mPhySeqState = gCCA_c;

    /* Ensure that no spurious interrupts are raised */
    mStatusAndControlRegs[IRQSTS3] &= 0xF0; /* do not change other IRQ status */
    mStatusAndControlRegs[IRQSTS3] |= (cIRQSTS3_TMR3MSK | cIRQSTS3_TMR3IRQ);
    MCR20Drv_DirectAccessSPIMultiByteWrite(IRQSTS1, mStatusAndControlRegs, 3);

    /* Write XCVR settings */
    MCR20Drv_DirectAccessSPIWrite(PHY_CTRL1, mStatusAndControlRegs[PHY_CTRL1]);

    /* Unmask SEQ interrupt */
    mStatusAndControlRegs[PHY_CTRL2] &= ~(cPHY_CTRL2_SEQMSK);
    MCR20Drv_DirectAccessSPIWrite(PHY_CTRL2, mStatusAndControlRegs[PHY_CTRL2]);

    /*Return success*/
    return 0;
}

/*
 * \brief Function aborts CCA process.
 *
 * \param none
 *
 * \return none
 */
static void rf_cca_abort(void)
{
    rf_abort();
}

/*
 * \brief Function starts the transmission of the frame. Called from ISR context!
 *
 * \param none
 *
 * \return none
 */
static void rf_start_tx(void)
{
    /* Perform TxRxAck sequence if required by phyTxMode */
    if (need_ack) {
        mStatusAndControlRegs[PHY_CTRL1] |= cPHY_CTRL1_RXACKRQD;
        mPhySeqState = gTR_c;
    } else {
        mStatusAndControlRegs[PHY_CTRL1] &= ~(cPHY_CTRL1_RXACKRQD);
        mPhySeqState = gTX_c;
    }

    mStatusAndControlRegs[PHY_CTRL1] &= ~(cPHY_CTRL1_XCVSEQ);
    mStatusAndControlRegs[PHY_CTRL1] |= mPhySeqState;

    /* Unmask SEQ interrupt */
    mStatusAndControlRegs[PHY_CTRL2] &= ~(cPHY_CTRL2_SEQMSK);

    /* Start the sequence immediately */
    MCR20Drv_DirectAccessSPIMultiByteWrite(PHY_CTRL1, &mStatusAndControlRegs[PHY_CTRL1], 2);

    if (need_ack) {
        rf_ack_wait_timer_start(gPhyWarmUpTime_c + gPhySHRDuration_c + tx_len * gPhySymbolsPerOctet_c + gPhyAckWaitDuration_c);
    }
}

/*
 * \brief Function sets the RF in RX state. Called from ISR context!
 *
 * \param none
 *
 * \return none
 */
static void rf_receive(void)
{
    uint8_t phyRegs[5];

    /* RX can start only from Idle state */
    if (mPhySeqState != gIdle_c) {
        return;
    }

    /* Set XCVR power state in run mode */
    rf_set_power_state(gXcvrRunState_d);
    /* read XVCR settings */
    phyRegs[IRQSTS1] = MCR20Drv_DirectAccessSPIMultiByteRead(IRQSTS2, &phyRegs[IRQSTS2], 4);
    /* unmask SEQ interrupt */
    phyRegs[PHY_CTRL2] &= ~(cPHY_CTRL2_SEQMSK);
    /* set XcvrSeq to RX */
    phyRegs[PHY_CTRL1] &= ~(cPHY_CTRL1_XCVSEQ);
    phyRegs[PHY_CTRL1] |=  gRX_c;
    mPhySeqState = gRX_c;
    /* Ensure that no spurious interrupts are raised */
    phyRegs[IRQSTS3] &= 0xF0; /* do not change other IRQ status */
    phyRegs[IRQSTS3] |= cIRQSTS3_TMR3MSK | cIRQSTS3_TMR3IRQ;
    /* sync settings with XCVR */
    MCR20Drv_DirectAccessSPIMultiByteWrite(IRQSTS1, phyRegs, 5);
}

/*
 * \brief Function calibrates the radio.
 *
 * \param none
 *
 * \return none
 */
static void rf_calibration_cb(void)
{
}

/*
 * \brief Function sets RF_ON flag when radio is powered.
 *
 * \param none
 *
 * \return none
 */
static void rf_on(void)
{
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
    uint8_t rf_lqi = MCR20Drv_DirectAccessSPIRead(LQI_VALUE);
    int8_t rf_rssi = 0;
    uint8_t len = mStatusAndControlRegs[RX_FRM_LEN] - 2;


    /*Start receiver*/
    rf_receive();

    /*Check the length is valid*/
    if (len > 1 && len < RF_BUFFER_SIZE) {
        rf_lqi  = rf_convert_LQI(rf_lqi);
        rf_rssi = rf_convert_LQI_to_RSSI(rf_lqi);
        /*gcararu: Scale LQI using received RSSI, to match the LQI reported by the ATMEL radio */
        rf_lqi  = rf_scale_lqi(rf_rssi);

        /*Read received packet*/
        MCR20Drv_PB_SPIBurstRead(rf_buffer, len);
        if (device_driver.phy_rx_cb) {
            device_driver.phy_rx_cb(rf_buffer, len, rf_lqi, rf_rssi, rf_radio_driver_id);
        }
    }
}

/*
 * \brief Function is called when MAC is shutting down the radio.
 *
 * \param none
 *
 * \return none
 */
static void rf_shutdown(void)
{
    /*Call RF OFF*/
    rf_off();
}

/*
 * \brief Function is a call back for TX end interrupt.
 *
 * \param none
 *
 * \return none
 */
static void rf_handle_tx_end(void)
{
    uint8_t rx_frame_pending = mStatusAndControlRegs[IRQSTS1] & cIRQSTS1_RX_FRM_PEND;

    /*Start receiver*/
    rf_receive();

    if (!device_driver.phy_tx_done_cb) {
        return;
    }

    /*Call PHY TX Done API*/
    if (need_ack) {
        if (rx_frame_pending) {
            device_driver.phy_tx_done_cb(rf_radio_driver_id, mac_tx_handle, PHY_LINK_TX_DONE_PENDING, 1, 1);
        } else {
            // arm_net_phy_tx_done(rf_radio_driver_id, mac_tx_handle, PHY_LINK_TX_SUCCESS, 1, 1);
            device_driver.phy_tx_done_cb(rf_radio_driver_id, mac_tx_handle, PHY_LINK_TX_DONE, 1, 1);
        }
    } else {
        device_driver.phy_tx_done_cb(rf_radio_driver_id, mac_tx_handle, PHY_LINK_TX_SUCCESS, 1, 1);
    }
}

/*
 * \brief Function is a call back for CCA ED done interrupt.
 *
 * \param none
 *
 * \return none
 */
static void rf_handle_cca_ed_done(void)
{
    /*Check the result of CCA process*/
    if (!(mStatusAndControlRegs[IRQSTS2] & cIRQSTS2_CCA)) {
        rf_start_tx();
    } else if (device_driver.phy_tx_done_cb) {
        /*Send CCA fail notification*/
        device_driver.phy_tx_done_cb(rf_radio_driver_id, mac_tx_handle, PHY_LINK_CCA_FAIL, 1, 1);
    }
}

/*
 * \brief Function sets the TX power variable.
 *
 * \param power TX power setting
 *
 * \return 0 Success
 * \return -1 Fail
 */
static int8_t rf_tx_power_set(uint8_t power)
{
    /* gcapraru: Map MCR20A Tx power levels over ATMEL values */
    static uint8_t pwrLevelMapping[16] = {25, 25, 25, 24, 24, 24, 23, 23, 22, 22, 21, 20, 19, 18, 17, 14};

    if (power > 15) {
        return -1;
    }

    radio_tx_power = power;
    MCR20Drv_DirectAccessSPIWrite(PA_PWR, pwrLevelMapping[power]);
    return 0;
}

/*
 * \brief Function returns the TX power variable.
 *
 * \param none
 *
 * \return radio_tx_power TX power variable
 */
static uint8_t rf_tx_power_get(void)
{
    return radio_tx_power;
}

/*
 * \brief Function enables the usage of Antenna diversity.
 *
 * \param none
 *
 * \return 0 Success
 */
static int8_t rf_enable_antenna_diversity(void)
{
    uint8_t phyReg;

    phyReg = MCR20Drv_IndirectAccessSPIRead(ANT_AGC_CTRL);
    phyReg |= cANT_AGC_CTRL_FAD_EN_Mask_c;
    MCR20Drv_IndirectAccessSPIWrite(ANT_AGC_CTRL, phyReg);

    phyReg = MCR20Drv_IndirectAccessSPIRead(ANT_PAD_CTRL);
    phyReg |= 0x02;
    MCR20Drv_IndirectAccessSPIWrite(ANT_PAD_CTRL, phyReg);

    return 0;
}

/*
 * \brief Function gives the control of RF states to MAC.
 *
 * \param new_state RF state
 * \param rf_channel RF channel
 *
 * \return 0 Success
 */
static int8_t rf_interface_state_control(phy_interface_state_e new_state, uint8_t rf_channel)
{
    int8_t ret_val = 0;
    switch (new_state) {
        /*Reset PHY driver and set to idle*/
        case PHY_INTERFACE_RESET:
            break;
        /*Disable PHY Interface driver*/
        case PHY_INTERFACE_DOWN:
            rf_shutdown();
            break;
        /*Enable PHY Interface driver*/
        case PHY_INTERFACE_UP:
            rf_channel_set(rf_channel);
            rf_receive();
            break;
        /*Enable wireless interface ED scan mode*/
        case PHY_INTERFACE_RX_ENERGY_STATE:
            rf_abort();
            rf_channel_set(rf_channel);
            break;
        case PHY_INTERFACE_SNIFFER_STATE:             /**< Enable Sniffer state */
            rf_promiscuous(1);
            rf_channel_set(rf_channel);
            rf_receive();
            break;
    }
    return ret_val;
}

/*
 * \brief Function controls the ACK pending, channel setting and energy detection.
 *
 * \param extension_type Type of control
 * \param data_ptr Data from NET library
 *
 * \return 0 Success
 */
static int8_t rf_extension(phy_extension_type_e extension_type, uint8_t *data_ptr)
{
    switch (extension_type) {
        /*Control MAC pending bit for Indirect data transmission*/
        case PHY_EXTENSION_CTRL_PENDING_BIT: {
            uint8_t reg = MCR20Drv_DirectAccessSPIRead(SRC_CTRL);

            if (*data_ptr) {
                reg |= cSRC_CTRL_ACK_FRM_PND;
            } else {
                reg &= ~cSRC_CTRL_ACK_FRM_PND;
            }

            MCR20Drv_DirectAccessSPIWrite(SRC_CTRL, reg);
            break;

        }
        /*Return frame Auto Ack frame pending status*/
        case PHY_EXTENSION_READ_LAST_ACK_PENDING_STATUS: {
            uint8_t reg = MCR20Drv_DirectAccessSPIRead(SRC_CTRL);
            if (reg & cSRC_CTRL_ACK_FRM_PND) {
                *data_ptr = 1;
            } else {
                *data_ptr  = 0;
            }
            break;
        }
        /*Read energy on the channel*/
        case PHY_EXTENSION_READ_CHANNEL_ENERGY:
            *data_ptr = rf_get_channel_energy();
            break;
        default:
            break;
    }
    return 0;
}

/*
 * \brief Function sets the addresses to RF address filters.
 *
 * \param address_type Type of address
 * \param address_ptr Pointer to given address
 *
 * \return 0 Success
 */
static int8_t rf_address_write(phy_address_type_e address_type, uint8_t *address_ptr)
{
    int8_t ret_val = 0;
    switch (address_type) {
        /*Set 48-bit address*/
        case PHY_MAC_48BIT:
            break;
        /*Set 64-bit address*/
        case PHY_MAC_64BIT:
            rf_set_address(address_ptr);
            break;
        /*Set 16-bit address*/
        case PHY_MAC_16BIT:
            rf_set_short_adr(address_ptr);
            break;
        /*Set PAN Id*/
        case PHY_MAC_PANID:
            rf_set_pan_id(address_ptr);
            break;
    }
    return ret_val;
}

/*
 * \brief Function initialises the ACK wait time and returns the used PHY mode.
 *
 * \param none
 *
 * \return tmp Used PHY mode
 */
static void rf_init_phy_mode(void)
{
}

/*
 * \brief Function is a RF interrupt vector. End of frame in RX and TX are handled here as well as CCA process interrupt.
 *
 * \param none
 *
 * \return none
 */
static void PHY_InterruptHandler(void)
{
    MCR20Drv_IRQ_Disable();
    irq_thread->flags_set(1);
}

static void PHY_InterruptThread(void)
{
    for (;;) {
        ThisThread::flags_wait_all(1);
        handle_interrupt();
        MCR20Drv_IRQ_Enable();
    }
}

static void handle_interrupt(void)
{
    uint8_t xcvseqCopy;

    /* Read transceiver interrupt status and control registers */
    mStatusAndControlRegs[IRQSTS1] =
        MCR20Drv_DirectAccessSPIMultiByteRead(IRQSTS2, &mStatusAndControlRegs[IRQSTS2], 7);

    xcvseqCopy = mStatusAndControlRegs[PHY_CTRL1] & cPHY_CTRL1_XCVSEQ;

    /* Flter Fail IRQ */
    if ((mStatusAndControlRegs[IRQSTS1] & cIRQSTS1_FILTERFAIL_IRQ) &&
            !(mStatusAndControlRegs[PHY_CTRL2] & cPHY_CTRL2_FILTERFAIL_MSK)) {
        if (xcvseqCopy == gRX_c) {
            /* Abort current SEQ */
            mStatusAndControlRegs[PHY_CTRL1] &= ~(cPHY_CTRL1_XCVSEQ);
            MCR20Drv_DirectAccessSPIWrite(PHY_CTRL1, mStatusAndControlRegs[PHY_CTRL1]);
            /* Wait for Sequence Idle */
            while ((MCR20Drv_DirectAccessSPIRead(SEQ_STATE) & 0x1F) != 0);
            /* Clear IRQ flags: */
            MCR20Drv_DirectAccessSPIWrite(IRQSTS1, cIRQSTS1_SEQIRQ);
            /* Restart Rx asap */
            mStatusAndControlRegs[PHY_CTRL1] |= gRX_c;
            MCR20Drv_DirectAccessSPIWrite(PHY_CTRL1, mStatusAndControlRegs[PHY_CTRL1]);
        }
    }

    /* TMR3 IRQ: ACK wait time-out */
    if ((mStatusAndControlRegs[IRQSTS3] & cIRQSTS3_TMR3IRQ) &&
            !(mStatusAndControlRegs[IRQSTS3] & cIRQSTS3_TMR3MSK)) {
        /* Disable TMR3 IRQ */
        mStatusAndControlRegs[IRQSTS3] |= cIRQSTS3_TMR3MSK;

        if (xcvseqCopy == gTR_c) {
            /* Set XCVR to Idle */
            mPhySeqState = gIdle_c;
            mStatusAndControlRegs[PHY_CTRL1] &=  ~(cPHY_CTRL1_XCVSEQ);
            /* Mask interrupts */
            mStatusAndControlRegs[PHY_CTRL2] |= cPHY_CTRL2_CCAMSK | cPHY_CTRL2_RXMSK | cPHY_CTRL2_TXMSK | cPHY_CTRL2_SEQMSK;
            /* Sync settings with XCVR */
            MCR20Drv_DirectAccessSPIMultiByteWrite(IRQSTS1, mStatusAndControlRegs, 5);

            rf_ack_wait_timer_interrupt();
            return;
        }
    }

    /* Sequencer interrupt, the autosequence has completed */
    if ((mStatusAndControlRegs[IRQSTS1] & cIRQSTS1_SEQIRQ) &&
            !(mStatusAndControlRegs[PHY_CTRL2] & cPHY_CTRL2_SEQMSK)) {
        /* Set XCVR to Idle */
        mPhySeqState = gIdle_c;
        mStatusAndControlRegs[PHY_CTRL1] &=  ~(cPHY_CTRL1_XCVSEQ);
        /* Mask interrupts */
        mStatusAndControlRegs[PHY_CTRL2] |= cPHY_CTRL2_CCAMSK | cPHY_CTRL2_RXMSK | cPHY_CTRL2_TXMSK | cPHY_CTRL2_SEQMSK;
        /* Sync settings with XCVR */
        MCR20Drv_DirectAccessSPIMultiByteWrite(IRQSTS1, mStatusAndControlRegs, 5);

        /* PLL unlock, the autosequence has been aborted due to PLL unlock */
        if (mStatusAndControlRegs[IRQSTS1] & cIRQSTS1_PLL_UNLOCK_IRQ) {
            if (xcvseqCopy == gRX_c) {
                rf_receive();
            }
            return;
        }

        switch (xcvseqCopy) {
            case gTX_c:
            case gTR_c:
                rf_handle_tx_end();
                break;

            case gRX_c:
                rf_handle_rx_end();
                break;

            case gCCA_c:
                rf_handle_cca_ed_done();
                break;

            default:
                break;
        }

        return;
    }
    /* Other IRQ. Clear XCVR interrupt flags */
    MCR20Drv_DirectAccessSPIMultiByteWrite(IRQSTS1, mStatusAndControlRegs, 3);
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
    MCR20Drv_IRQ_Disable();

    mPhySeqState = gIdle_c;

    mStatusAndControlRegs[IRQSTS1] = MCR20Drv_DirectAccessSPIMultiByteRead(IRQSTS2, &mStatusAndControlRegs[IRQSTS2], 5);

    /* Mask SEQ interrupt */
    mStatusAndControlRegs[PHY_CTRL2] |= cPHY_CTRL2_SEQMSK;
    MCR20Drv_DirectAccessSPIWrite(PHY_CTRL2, mStatusAndControlRegs[PHY_CTRL2]);

    if ((mStatusAndControlRegs[PHY_CTRL1] & cPHY_CTRL1_XCVSEQ) != gIdle_c) {
        /* Abort current SEQ */
        mStatusAndControlRegs[PHY_CTRL1] &= ~(cPHY_CTRL1_XCVSEQ);
        MCR20Drv_DirectAccessSPIWrite(PHY_CTRL1, mStatusAndControlRegs[PHY_CTRL1]);

        /* Wait for Sequence Idle (if not already) */
        while ((MCR20Drv_DirectAccessSPIRead(SEQ_STATE) & 0x1F) != 0);
        //while ( !(MCR20Drv_DirectAccessSPIRead(IRQSTS1) & cIRQSTS1_SEQIRQ));
        mStatusAndControlRegs[IRQSTS1] |= cIRQSTS1_SEQIRQ;
    }

    /* Clear all PP IRQ bits to avoid unexpected interrupts and mask TMR3 interrupt.
       Do not change TMR IRQ status. */
    mStatusAndControlRegs[IRQSTS3] &= 0xF0;
    mStatusAndControlRegs[IRQSTS3] |= (cIRQSTS3_TMR3MSK | cIRQSTS3_TMR3IRQ);
    MCR20Drv_DirectAccessSPIMultiByteWrite(IRQSTS1, mStatusAndControlRegs, 3);

    /* Unmask XCVR irq */
    MCR20Drv_IRQ_Enable();
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
    MCR20Drv_DirectAccessSPIMultiByteRead(EVENT_TMR_LSB, (uint8_t *) pRetClk, 3);

    platform_exit_critical();
}

/*
 * \brief Function set a time-out to an XCVR sequence.
 *
 * \param pEndTime pointer to the sequence time-out value [symbols]
 *
 * \return none
 */
static void rf_set_timeout(uint32_t *pEndTime)
{
    uint8_t phyReg;

    if (NULL == pEndTime) {
        return;
    }

    platform_enter_critical();

    phyReg = MCR20Drv_DirectAccessSPIRead(IRQSTS3);
    phyReg &= 0xF0;                    /* do not change IRQ status */
    phyReg |= (cIRQSTS3_TMR3MSK);      /* mask TMR3 interrupt */
    MCR20Drv_DirectAccessSPIWrite(IRQSTS3, phyReg);

    MCR20Drv_DirectAccessSPIMultiByteWrite(T3CMP_LSB, (uint8_t *) pEndTime, 3);

    phyReg &= ~(cIRQSTS3_TMR3MSK);      /* unmask TMR3 interrupt */
    phyReg |= (cIRQSTS3_TMR3IRQ);       /* aknowledge TMR3 IRQ */
    MCR20Drv_DirectAccessSPIWrite(IRQSTS3, phyReg);

    platform_exit_critical();
}

/*
 * \brief Function reads a random number from RF.
 *
 * \param none
 *
 * \return 8-bit random number
 */
static uint8_t rf_if_read_rnd(void)
{
    uint8_t phyReg;

    MCR20Drv_IRQ_Disable();
    /* Check if XCVR is idle */
    phyReg = MCR20Drv_DirectAccessSPIRead(PHY_CTRL1);

    if ((phyReg & cPHY_CTRL1_XCVSEQ) == gIdle_c) {
        /* Program a new sequence */
        MCR20Drv_DirectAccessSPIWrite(PHY_CTRL1, phyReg | gCCA_c);
        /* Wait for sequence to finish */
        while (!(MCR20Drv_DirectAccessSPIRead(IRQSTS1) & cIRQSTS1_SEQIRQ));
        /* Clear interrupt flag */
        MCR20Drv_DirectAccessSPIWrite(IRQSTS1, cIRQSTS1_SEQIRQ);
    }

    MCR20Drv_IRQ_Enable();

    return MCR20Drv_IndirectAccessSPIRead(_RNG);
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
    int32_t rssi = (50 * lqi - 16820) / 163;
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
    uint32_t tmpLQI;

    /* LQI Saturation Level */
    if (hwLqi >= 230) {
        return 0xFF;
    } else if (hwLqi <= 9) {
        return 0;
    } else {
        /* Rescale the LQI values from min to saturation to the 0x00 - 0xFF range */
        /* The LQI value mst be multiplied by ~1.1087 */
        /* tmpLQI =  hwLqi * 7123 ~= hwLqi * 65536 * 0.1087 = hwLqi * 2^16 * 0.1087*/
        tmpLQI = ((uint32_t)hwLqi * (uint32_t)7123);
        /* tmpLQI =  (tmpLQI / 2^16) + hwLqi */
        tmpLQI = (uint32_t)(tmpLQI >> 16) + (uint32_t)hwLqi;

        return (uint8_t)tmpLQI;
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
    uint8_t rxFrameFltReg, phyCtrl4Reg;

    rxFrameFltReg = MCR20Drv_IndirectAccessSPIRead(RX_FRAME_FILTER);
    phyCtrl4Reg = MCR20Drv_DirectAccessSPIRead(PHY_CTRL4);

    if (state) {
        /* FRM_VER[1:0] = b00. 00: Any FrameVersion accepted (0,1,2 & 3) */
        /* All frame types accepted*/
        phyCtrl4Reg   |= cPHY_CTRL4_PROMISCUOUS;
        rxFrameFltReg &= ~(cRX_FRAME_FLT_FRM_VER);
        rxFrameFltReg |= (cRX_FRAME_FLT_ACK_FT | cRX_FRAME_FLT_NS_FT);
    } else {
        phyCtrl4Reg   &= ~cPHY_CTRL4_PROMISCUOUS;
        /* FRM_VER[1:0] = b11. Accept FrameVersion 0 and 1 packets, reject all others */
        /* Beacon, Data and MAC command frame types accepted */
        rxFrameFltReg &= ~(cRX_FRAME_FLT_FRM_VER);
        rxFrameFltReg |= (0x03 << cRX_FRAME_FLT_FRM_VER_Shift_c);
        rxFrameFltReg &= ~(cRX_FRAME_FLT_ACK_FT | cRX_FRAME_FLT_NS_FT);
    }

    MCR20Drv_IndirectAccessSPIWrite(RX_FRAME_FILTER, rxFrameFltReg);
    MCR20Drv_DirectAccessSPIWrite(PHY_CTRL4, phyCtrl4Reg);
}

/*
 * \brief Function used to switch XCVR power state.
 *
 * \param state The XCVR power mode
 *
 * \return none
 */
static void rf_set_power_state(xcvrPwrMode_t newState)
{
    uint8_t pwrMode;
    uint8_t xtalState;

    if (mPwrState == newState) {
        return;
    }

    /* Read power settings from RF */
    pwrMode = MCR20Drv_DirectAccessSPIRead(PWR_MODES);
    xtalState = pwrMode & cPWR_MODES_XTALEN;

    switch (newState) {
        case gXcvrPwrIdle_c:
            pwrMode &= ~(cPWR_MODES_AUTODOZE);
            pwrMode |= (cPWR_MODES_XTALEN | cPWR_MODES_PMC_MODE);
            break;
        case gXcvrPwrAutodoze_c:
            pwrMode |= (cPWR_MODES_XTALEN | cPWR_MODES_AUTODOZE | cPWR_MODES_PMC_MODE);
            break;
        case gXcvrPwrDoze_c:
            pwrMode &= ~(cPWR_MODES_AUTODOZE | cPWR_MODES_PMC_MODE);
            pwrMode |= cPWR_MODES_XTALEN;
            break;
        case gXcvrPwrHibernate_c:
            pwrMode &= ~(cPWR_MODES_XTALEN | cPWR_MODES_AUTODOZE | cPWR_MODES_PMC_MODE);
            break;
        default:
            return;
    }

    mPwrState = newState;
    MCR20Drv_DirectAccessSPIWrite(PWR_MODES, pwrMode);

    if (!xtalState && (pwrMode & cPWR_MODES_XTALEN)) {
        /* wait for crystal oscillator to complet its warmup */
        while ((MCR20Drv_DirectAccessSPIRead(PWR_MODES) & cPWR_MODES_XTAL_READY) != cPWR_MODES_XTAL_READY);
        /* wait for radio wakeup from hibernate interrupt */
        while ((MCR20Drv_DirectAccessSPIRead(IRQSTS2) & (cIRQSTS2_WAKE_IRQ | cIRQSTS2_TMRSTATUS)) != (cIRQSTS2_WAKE_IRQ | cIRQSTS2_TMRSTATUS));

        MCR20Drv_DirectAccessSPIWrite(IRQSTS2, cIRQSTS2_WAKE_IRQ);
    }
}

/*
 * \brief Function reads the energy level on the preselected channel.
 *
 * \return energy level
 */
static uint8_t rf_get_channel_energy(void)
{
    uint8_t ccaMode;

    MCR20Drv_IRQ_Disable();
    /* RX can start only from Idle state */
    if (mPhySeqState != gIdle_c) {
        MCR20Drv_IRQ_Enable();
        return 0;
    }

    /* Set XCVR power state in run mode */
    rf_set_power_state(gXcvrRunState_d);

    /* Switch to ED mode */
    ccaMode = (mStatusAndControlRegs[PHY_CTRL4] >> cPHY_CTRL4_CCATYPE_Shift_c) & cPHY_CTRL4_CCATYPE;
    if (ccaMode != gCcaED_c) {
        mStatusAndControlRegs[PHY_CTRL4] &= ~(cPHY_CTRL4_CCATYPE << cPHY_CTRL4_CCATYPE_Shift_c);
        mStatusAndControlRegs[PHY_CTRL4] |= gCcaED_c << cPHY_CTRL4_CCATYPE_Shift_c;
        MCR20Drv_DirectAccessSPIWrite(PHY_CTRL4, mStatusAndControlRegs[PHY_CTRL4]);
    }

    /* Start ED sequence */
    mStatusAndControlRegs[PHY_CTRL1] |= gCCA_c;
    MCR20Drv_DirectAccessSPIWrite(IRQSTS1, cIRQSTS1_CCAIRQ | cIRQSTS1_SEQIRQ);
    MCR20Drv_DirectAccessSPIWrite(PHY_CTRL1, mStatusAndControlRegs[PHY_CTRL1]);
    /* Wait for sequence to finish */
    while (!(MCR20Drv_DirectAccessSPIRead(IRQSTS1) & cIRQSTS1_SEQIRQ));
    /* Set XCVR to Idle */
    mStatusAndControlRegs[PHY_CTRL1] &= ~(cPHY_CTRL1_XCVSEQ);
    MCR20Drv_DirectAccessSPIWrite(PHY_CTRL1, mStatusAndControlRegs[PHY_CTRL1]);
    MCR20Drv_DirectAccessSPIWrite(IRQSTS1, cIRQSTS1_CCAIRQ | cIRQSTS1_SEQIRQ);

    MCR20Drv_IRQ_Enable();

    return rf_convert_energy_level(MCR20Drv_DirectAccessSPIRead(CCA1_ED_FNL));
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
    if (energyLevel >= 90) {
        /* ED value is below minimum. Return 0x00. */
        energyLevel = 0x00;
    } else if (energyLevel <= 26) {
        /* ED value is above maximum. Return 0xFF. */
        energyLevel = 0xFF;
    } else {
        /* Energy level (-90 dBm to -26 dBm ) --> varies form 0 to 64 */
        energyLevel = (90 - energyLevel);
        /* Rescale the energy level values to the 0x00-0xff range (0 to 64 translates in 0 to 255) */
        /* energyLevel * 3.9844 ~= 4 */
        /* Multiply with 4=2^2 by shifting left.
        The multiplication will not overflow beacause energyLevel has values between 0 and 63 */
        energyLevel <<= 2;
    }

    return energyLevel;
}

static uint8_t rf_scale_lqi(int8_t rssi)
{
    uint8_t scaled_lqi;
    /*Worst case sensitivity*/
    const int8_t rf_sensitivity = -98;

    /*rssi < RF sensitivity*/
    if (rssi < rf_sensitivity) {
        scaled_lqi = 0;
    }
    /*-91 dBm < rssi < -81 dBm (AT86RF233 XPro)*/
    /*-90 dBm < rssi < -80 dBm (AT86RF212B XPro)*/
    else if (rssi < (rf_sensitivity + 10)) {
        scaled_lqi = 31;
    }
    /*-81 dBm < rssi < -71 dBm (AT86RF233 XPro)*/
    /*-80 dBm < rssi < -70 dBm (AT86RF212B XPro)*/
    else if (rssi < (rf_sensitivity + 20)) {
        scaled_lqi = 207;
    }
    /*-71 dBm < rssi < -61 dBm (AT86RF233 XPro)*/
    /*-70 dBm < rssi < -60 dBm (AT86RF212B XPro)*/
    else if (rssi < (rf_sensitivity + 30)) {
        scaled_lqi = 255;
    }
    /*-61 dBm < rssi < -51 dBm (AT86RF233 XPro)*/
    /*-60 dBm < rssi < -50 dBm (AT86RF212B XPro)*/
    else if (rssi < (rf_sensitivity + 40)) {
        scaled_lqi = 255;
    }
    /*-51 dBm < rssi < -41 dBm (AT86RF233 XPro)*/
    /*-50 dBm < rssi < -40 dBm (AT86RF212B XPro)*/
    else if (rssi < (rf_sensitivity + 50)) {
        scaled_lqi = 255;
    }
    /*-41 dBm < rssi < -31 dBm (AT86RF233 XPro)*/
    /*-40 dBm < rssi < -30 dBm (AT86RF212B XPro)*/
    else if (rssi < (rf_sensitivity + 60)) {
        scaled_lqi = 255;
    }
    /*-31 dBm < rssi < -21 dBm (AT86RF233 XPro)*/
    /*-30 dBm < rssi < -20 dBm (AT86RF212B XPro)*/
    else if (rssi < (rf_sensitivity + 70)) {
        scaled_lqi = 255;
    }
    /*rssi > RF saturation*/
    else if (rssi > (rf_sensitivity + 80)) {
        scaled_lqi = 111;
    }
    /*-21 dBm < rssi < -11 dBm (AT86RF233 XPro)*/
    /*-20 dBm < rssi < -10 dBm (AT86RF212B XPro)*/
    else {
        scaled_lqi = 255;
    }

    return scaled_lqi;
}


/*****************************************************************************/
/*              Layer porting to the Freescale driver                        */
/*****************************************************************************/
extern "C" void xcvr_spi_init(uint32_t instance)
{
    (void)instance;
}

extern "C" void RF_IRQ_Init(void)
{
    MBED_ASSERT(irq != NULL);
    irq->mode(PullUp);
    irq->fall(&PHY_InterruptHandler);
}

extern "C" void RF_IRQ_Enable(void)
{
    MBED_ASSERT(irq != NULL);
    irq->enable_irq();
}

extern "C" void RF_IRQ_Disable(void)
{
    MBED_ASSERT(irq != NULL);
    irq->disable_irq();
}

extern "C" uint8_t RF_isIRQ_Pending(void)
{
    MBED_ASSERT(rf != NULL);
    return !irq_pin->read();
}

extern "C" void RF_RST_Set(int state)
{
    MBED_ASSERT(rst != NULL);
    *rst = state;
}

extern "C" void gXcvrAssertCS_d(void)
{
    MBED_ASSERT(cs != NULL);
    *cs = 0;
}

extern "C" void gXcvrDeassertCS_d(void)
{
    MBED_ASSERT(cs != NULL);
    *cs = 1;
}

extern "C" void xcvr_spi_configure_speed(uint32_t instance, uint32_t freq)
{
    MBED_ASSERT(spi != NULL);
    (void)instance;
    spi->frequency(freq);
}

extern "C" void xcvr_spi_transfer(uint32_t instance,
                                  uint8_t *sendBuffer,
                                  uint8_t *receiveBuffer,
                                  size_t transferByteCount)
{
    MBED_ASSERT(spi != NULL);
    (void)instance;
    volatile uint8_t dummy;

    if (!transferByteCount) {
        return;
    }

    if (!sendBuffer && !receiveBuffer) {
        return;
    }

    while (transferByteCount--) {
        if (sendBuffer) {
            dummy = *sendBuffer;
            sendBuffer++;
        } else {
            dummy = 0xFF;
        }

        dummy = spi->write(dummy);

        if (receiveBuffer) {
            *receiveBuffer = dummy;
            receiveBuffer++;
        }
    }
}

/*****************************************************************************/
/*****************************************************************************/

static void rf_if_lock(void)
{
    platform_enter_critical();
}

static void rf_if_unlock(void)
{
    platform_exit_critical();
}

NanostackRfPhyMcr20a::NanostackRfPhyMcr20a(PinName spi_mosi, PinName spi_miso,
                                           PinName spi_sclk, PinName spi_cs,  PinName spi_rst, PinName spi_irq)
    : _spi(spi_mosi, spi_miso, spi_sclk), _rf_cs(spi_cs), _rf_rst(spi_rst, 1),
      _rf_irq(spi_irq), _rf_irq_pin(spi_irq),
      _irq_thread(osPriorityRealtime, 1024)
{
    char mac48[6];
    mbed_mac_address(mac48);

    MAC_address[0] = mac48[0];
    MAC_address[1] = mac48[1];
    MAC_address[2] = mac48[2];
    MAC_address[3] = 0xFF;
    MAC_address[4] = 0xFF;
    MAC_address[5] = mac48[3];
    MAC_address[6] = mac48[4];
    MAC_address[7] = mac48[5];
}

NanostackRfPhyMcr20a::~NanostackRfPhyMcr20a()
{
    // Do nothing
}

int8_t NanostackRfPhyMcr20a::rf_register()
{

    rf_if_lock();

    if (rf != NULL) {
        rf_if_unlock();
        error("Multiple registrations of NanostackRfPhyMcr20a not supported");
        return -1;
    }

    _irq_thread.start(mbed::callback(PHY_InterruptThread));

    _pins_set();
    int8_t radio_id = rf_device_register();
    if (radio_id < 0) {
        _pins_clear();
        rf = NULL;
    }

    rf_if_unlock();
    return radio_id;
}

void NanostackRfPhyMcr20a::rf_unregister()
{
    rf_if_lock();

    if (rf != this) {
        rf_if_unlock();
        return;
    }

    rf_device_unregister();
    rf = NULL;
    _pins_clear();

    rf_if_unlock();
}

void NanostackRfPhyMcr20a::get_mac_address(uint8_t *mac)
{
    rf_if_lock();

    memcpy((void *)mac, (void *)MAC_address, sizeof(MAC_address));

    rf_if_unlock();
}

void NanostackRfPhyMcr20a::set_mac_address(uint8_t *mac)
{
    rf_if_lock();

    if (NULL != rf) {
        error("NanostackRfPhyAtmel cannot change mac address when running");
        rf_if_unlock();
        return;
    }
    memcpy((void *)MAC_address, (void *)mac, sizeof(MAC_address));

    rf_if_unlock();
}

void NanostackRfPhyMcr20a::_pins_set()
{
    spi = &_spi;
    cs = &_rf_cs;
    rst = &_rf_rst;
    irq = &_rf_irq;
    irq_pin = &_rf_irq_pin;
    irq_thread = &_irq_thread;
}

void NanostackRfPhyMcr20a::_pins_clear()
{
    spi = NULL;
    cs = NULL;
    rst = NULL;
    irq = NULL;
    irq_pin = NULL;
    irq_thread = NULL;
}

#if MBED_CONF_MCR20A_PROVIDE_DEFAULT || TARGET_KW24D

NanostackRfPhy &NanostackRfPhy::get_default_instance()
{
    static NanostackRfPhyMcr20a rf_phy(MCR20A_SPI_MOSI, MCR20A_SPI_MISO, MCR20A_SPI_SCLK, MCR20A_SPI_CS, MCR20A_SPI_RST, MCR20A_SPI_IRQ);
    return rf_phy;
}

#endif // MBED_CONF_MCR20A_PROVIDE_DEFAULT

#endif // MBED_CONF_NANOSTACK_CONFIGURATION
