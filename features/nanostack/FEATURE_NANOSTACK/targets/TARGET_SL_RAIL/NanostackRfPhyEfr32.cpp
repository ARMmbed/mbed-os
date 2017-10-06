/*
 * Copyright (c) 2016 Silicon Laboratories, Inc. http://www.silabs.com
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
#include "NanostackRfPhyEfr32.h"
#include "ns_types.h"
#include "platform/arm_hal_interrupt.h"
#include "nanostack/platform/arm_hal_phy.h"
#include "mbed_toolchain.h"
#include <string.h>

#include "mbed-trace/mbed_trace.h"
#define  TRACE_GROUP  "SLRF"

/* Silicon Labs headers */
extern "C" {
    #include "rail/rail.h"
    #include "rail/pa.h"
    #include "rail/pti.h"
    #include "rail/ieee802154/rail_ieee802154.h"
    #include "buffer-pool-memory-manager/buffer_pool_allocator.h"
}

/* RF driver data */
static phy_device_driver_s device_driver;
static int8_t  rf_radio_driver_id = -1;
static uint8_t MAC_address[8];
static uint16_t PAN_address;
static uint16_t short_address;

/* Driver instance handle */
static NanostackRfPhyEfr32 *rf = NULL;

/* Channel configurations */
static const phy_rf_channel_configuration_s phy_24ghz = {2405000000U, 5000000U, 250000U, 16U, M_OQPSK};
static const phy_rf_channel_configuration_s phy_subghz = {868300000U, 2000000U, 250000U, 11U, M_OQPSK};

static const phy_device_channel_page_s phy_channel_pages[] = {
        { CHANNEL_PAGE_0, &phy_24ghz},
        { CHANNEL_PAGE_2, &phy_subghz},
        { CHANNEL_PAGE_0, NULL}
};

/* Driver structures */
typedef enum {
    RADIO_UNINIT,
    RADIO_INITING,
    RADIO_IDLE,
    RADIO_TX,
    RADIO_RX,
    RADIO_CALIBRATION
} siliconlabs_modem_state_t;

static const RAIL_CsmaConfig_t csma_config = RAIL_CSMA_CONFIG_802_15_4_2003_2p4_GHz_OQPSK_CSMA;

#if defined(TARGET_EFR32MG1)
#include "ieee802154_subg_efr32xg1_configurator_out.h"
#include "ieee802154_efr32xg1_configurator_out.h"
#elif defined(TARGET_EFR32MG12)
#include "ieee802154_efr32xg12_configurator_out.h"
#else
#error "Not a valid target."
#endif

static const RAIL_ChannelConfigEntry_t entry[] = {
  {0U,  0U,  600000U,  868300000U},
  {1U,  10U, 2000000U, 906000000U},
  {11U, 26U, 5000000U, 2405000000U}
};

#if MBED_CONF_SL_RAIL_BAND == 868
#ifndef MBED_CONF_SL_RAIL_HAS_SUBGIG
#error "Sub-Gigahertz band is not supported on this target."
#endif
static const RAIL_ChannelConfig_t channels = {
  (RAIL_ChannelConfigEntry_t *) &entry[0],
  1
};
#elif MBED_CONF_SL_RAIL_BAND == 915
#ifndef MBED_CONF_SL_RAIL_HAS_SUBGIG
#error "Sub-Gigahertz band is not supported on this target."
#endif
static const RAIL_ChannelConfig_t channels = {
  (RAIL_ChannelConfigEntry_t *) &entry[1],
  1
};
#elif MBED_CONF_SL_RAIL_BAND == 2400
#ifndef MBED_CONF_SL_RAIL_HAS_2P4
#error "2.4GHz band is not supported on this target."
#endif
static const RAIL_ChannelConfig_t channels = {
  (RAIL_ChannelConfigEntry_t *) &entry[2],
  1
};
#else
#error "sl-rail.band is not correctly defined"
#endif

static const RAIL_IEEE802154_Config_t config = { false, false,
                                    RAIL_IEEE802154_ACCEPT_STANDARD_FRAMES,
                                    RAIL_RF_STATE_RX, 100, 192, 894, NULL };

static const RAIL_Init_t railInitParams = { 140, 38400000, RAIL_CAL_ALL_PENDING };

#if defined (MBED_CONF_SL_RAIL_HAS_2P4)
    // Set up the PA for 2.4 GHz operation
static const RADIO_PAInit_t paInit2p4 = {
        PA_SEL_2P4_HP,    /* Power Amplifier mode */
        PA_VOLTMODE_DCDC, /* Power Amplifier vPA Voltage mode */
        100,              /* Desired output power in dBm * 10 */
        0,                /* Output power offset in dBm * 10 */
        10                /* Desired ramp time in us */
    };
#endif

#if defined (MBED_CONF_SL_RAIL_HAS_SUBGIG)
    // Set up the PA for sub-GHz operation
static const RADIO_PAInit_t paInitSubGhz = {
        PA_SEL_SUBGIG,    /* Power Amplifier mode */
        PA_VOLTMODE_DCDC, /* Power Amplifier vPA Voltage mode */
        100,              /* Desired output power in dBm * 10 */
        0,                /* Output power offset in dBm * 10 */
        10                /* Desired ramp time in us */
    };
#endif

static volatile siliconlabs_modem_state_t radio_state = RADIO_UNINIT;
static volatile int8_t channel = -1;
static volatile uint8_t current_tx_handle = 0;
static volatile uint8_t current_tx_sequence = 0;
static volatile bool waiting_for_ack = false;
static volatile bool data_pending = false, last_ack_pending_bit = false;
static volatile uint32_t last_tx = 0;

/* ARM_NWK_HAL prototypes */
static int8_t rf_extension(phy_extension_type_e extension_type, uint8_t *data_ptr);
static int8_t rf_interface_state_control(phy_interface_state_e new_state, uint8_t rf_channel);
static int8_t rf_address_write(phy_address_type_e address_type, uint8_t *address_ptr);
static int8_t rf_start_cca(uint8_t *data_ptr, uint16_t data_length, uint8_t tx_handle, data_protocol_e data_protocol );

/* Local function prototypes */
static bool rail_checkAndSwitchChannel(uint8_t channel);

/*============ CODE =========*/

/*
 * \brief Function initialises and registers the RF driver.
 *
 * \param none
 *
 * \return rf_radio_driver_id Driver ID given by NET library
 */
static int8_t rf_device_register(void)
{
    // If we already exist, bail.
    if(radio_state != RADIO_UNINIT) {
        return -1;
    }

#if MBED_CONF_SL_RAIL_BAND == 2400
    RADIO_PA_Init((RADIO_PAInit_t*)&paInit2p4);
#elif (MBED_CONF_SL_RAIL_BAND == 915) || (MBED_CONF_SL_RAIL_BAND == 868)
    RADIO_PA_Init((RADIO_PAInit_t*)&paInitSubGhz);
#endif

    // Set up PTI since it makes life so much easier
#if defined(MBED_CONF_SL_RAIL_PTI) && (MBED_CONF_SL_RAIL_PTI == 1)
    RADIO_PTIInit_t ptiInit = {
        MBED_CONF_SL_RAIL_PTI_MODE,
        MBED_CONF_SL_RAIL_PTI_BAUDRATE,
        MBED_CONF_SL_RAIL_PTI_DOUT_LOCATION,
        MBED_CONF_SL_RAIL_PTI_DOUT_PORT,
        MBED_CONF_SL_RAIL_PTI_DOUT_PIN,
        MBED_CONF_SL_RAIL_PTI_DCLK_LOCATION,
        MBED_CONF_SL_RAIL_PTI_DCLK_PORT,
        MBED_CONF_SL_RAIL_PTI_DCLK_PIN,
        MBED_CONF_SL_RAIL_PTI_DFRAME_LOCATION,
        MBED_CONF_SL_RAIL_PTI_DFRAME_PORT,
        MBED_CONF_SL_RAIL_PTI_DFRAME_PIN
    };

    RADIO_PTI_Init(&ptiInit);
#endif

    // Set up RAIL
    RAIL_RfInit(&railInitParams);
    RAIL_ChannelConfig(&channels);
#if (MBED_CONF_SL_RAIL_BAND == 2400)
    RAIL_RadioConfig((void*) ieee802154_config_base);
    channel = 11;
#elif (MBED_CONF_SL_RAIL_BAND == 915)
    RAIL_RadioConfig((void*) ieee802154_config_915);
    channel = 1;
#elif (MBED_CONF_SL_RAIL_BAND == 868)
    RAIL_RadioConfig((void*) ieee802154_config_863);
    channel = 0;
#endif
    RAIL_IEEE802154_Init((RAIL_IEEE802154_Config_t*)&config);

    /* Get real MAC address */
    /* MAC is stored MSB first */
    memcpy(MAC_address, (const void*)&DEVINFO->UNIQUEH, 4);
    memcpy(&MAC_address[4], (const void*)&DEVINFO->UNIQUEL, 4);

    /*Set pointer to MAC address*/
    device_driver.PHY_MAC = MAC_address;
    device_driver.driver_description = (char*)"EFR32_154";

    /*Type of RF PHY*/
#if MBED_CONF_SL_RAIL_BAND == 2400
    device_driver.link_type = PHY_LINK_15_4_2_4GHZ_TYPE;
#elif (MBED_CONF_SL_RAIL_BAND == 915) || (MBED_CONF_SL_RAIL_BAND == 868)
    device_driver.link_type = PHY_LINK_15_4_SUBGHZ_TYPE;
#endif

    device_driver.phy_channel_pages = phy_channel_pages;
    /*Maximum size of payload is 127*/
    device_driver.phy_MTU = 127;
    /*1 byte header in PHY layer (length)*/
    device_driver.phy_header_length = 1;
    /*No tail in PHY layer*/
    device_driver.phy_tail_length = 0;
    /*Set address write function*/
    device_driver.address_write = &rf_address_write;
    /*Set RF extension function*/
    device_driver.extension = &rf_extension;
    /*Set RF state control function*/
    device_driver.state_control = &rf_interface_state_control;
    /*Set transmit function*/
    device_driver.tx = &rf_start_cca;
    /*Upper layer callbacks init to NULL, get populated by arm_net_phy_register*/
    device_driver.phy_rx_cb = NULL;
    device_driver.phy_tx_done_cb = NULL;
    /*Virtual upper data callback init to NULL*/
    device_driver.arm_net_virtual_rx_cb = NULL;
    device_driver.arm_net_virtual_tx_cb = NULL;

    /*Register device driver*/
    rf_radio_driver_id = arm_net_phy_register(&device_driver);

    // If the radio hasn't called the ready callback by now, place it in the initing state
    if(radio_state == RADIO_UNINIT) {
        radio_state = RADIO_INITING;
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
 * \brief Function starts the CCA process before starting data transmission and copies the data to RF TX FIFO.
 *
 * \param data_ptr Pointer to TX data
 * \param data_length Length of the TX data
 * \param tx_handle Handle to transmission
 * \return 0 Success
 * \return -1 Busy
 */
static int8_t rf_start_cca(uint8_t *data_ptr, uint16_t data_length, uint8_t tx_handle, data_protocol_e data_protocol )
{

    RAIL_TxData_t txData = {
        data_ptr,
        data_length + 3
    };

    switch(radio_state) {
    case RADIO_UNINIT:
        tr_debug("Radio uninit\n");
        return -1;
    case RADIO_INITING:
        tr_debug("Radio initing\n");
        return -1;
    case RADIO_CALIBRATION:
        tr_debug("Radio calibrating\n");
        return -1;
    case RADIO_TX:
        tr_debug("Radio in TX mode\n");
        return -1;
    case RADIO_IDLE:
    case RADIO_RX:
        // If we're still waiting for an ACK, don't mess up the internal state
        if(waiting_for_ack || RAIL_RfStateGet() == RAIL_RF_STATE_TX) {
            if((RAIL_GetTime() - last_tx) < 30000) {
                tr_debug("Still waiting on previous ACK\n");
                return -1;
            } else {
                tr_debug("TXerr\n");
            }
        }

        data_ptr[0] = data_length + 2;
        RAIL_RfIdleExt(RAIL_IDLE_ABORT , true);
        RAIL_TxDataLoad(&txData);
        radio_state = RADIO_TX;

        RAIL_TxOptions_t txOpt;
        //Check to see whether we'll be waiting for an ACK
        if(data_ptr[1] & (1 << 5)) {
            txOpt.waitForAck = true;
            waiting_for_ack = true;
        } else {
            txOpt.waitForAck = false;
        }

        //tr_debug("Called TX, len %d, chan %d, ack %d\n", data_length, channel, waiting_for_ack ? 1 : 0);

        if(RAIL_TxStartWithOptions(channel, &txOpt, &RAIL_CcaCsma, (RAIL_CsmaConfig_t*) &csma_config) == 0) {
          //Save packet number and sequence
          current_tx_handle = tx_handle;
          current_tx_sequence = data_ptr[3];
          return 0;
        } else {
          RAIL_RfIdle();
          RAIL_RxStart(channel);
          radio_state = RADIO_RX;
          return -1;
        }
    }
    //Should never get here...
    return -1;
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
    switch (new_state)
    {
        /* Reset PHY driver and set to idle */
        case PHY_INTERFACE_RESET:
            RAIL_RfIdle();
            radio_state = RADIO_IDLE;
            break;
        /* Disable PHY Interface driver */
        case PHY_INTERFACE_DOWN:
            RAIL_RfIdle();
            radio_state = RADIO_IDLE;
            break;
        /* Enable RX */
        case PHY_INTERFACE_UP:
            if(rail_checkAndSwitchChannel(rf_channel)) {
                RAIL_IEEE802154_SetPromiscuousMode(false);
                RAIL_RxStart(channel);
                radio_state = RADIO_RX;
            } else {
                ret_val = -1;
            }
            break;
        /* Enable wireless interface ED scan mode */
        case PHY_INTERFACE_RX_ENERGY_STATE:
            tr_debug("Energy det req\n");
            // TODO: implement energy detection
            break;
        /* Enable RX in promiscuous mode (aka no address filtering) */
        case PHY_INTERFACE_SNIFFER_STATE:
            if(rail_checkAndSwitchChannel(rf_channel)) {
                RAIL_IEEE802154_SetPromiscuousMode(true);
                RAIL_RxStart(channel);
                radio_state = RADIO_RX;
            } else {
                ret_val = -1;
            }
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
    switch (extension_type)
    {
        /* Control MAC pending bit for Indirect data transmission */
        case PHY_EXTENSION_CTRL_PENDING_BIT:
            if(*data_ptr) {
                data_pending = true;
            } else {
                data_pending = false;
            }
            break;
        /* Return frame pending bit from last received ACK */
        case PHY_EXTENSION_READ_LAST_ACK_PENDING_STATUS:
            if(last_ack_pending_bit) {
                *data_ptr = 0xFF;
            } else {
                *data_ptr = 0;
            }
            break;
        /* Set channel */
        case PHY_EXTENSION_SET_CHANNEL:
            channel = *data_ptr;
            break;
        /* Read energy on the channel */
        case PHY_EXTENSION_READ_CHANNEL_ENERGY:
            // TODO: implement energy detection
            *data_ptr = 0;
            break;
        /* Read status of the link */
        case PHY_EXTENSION_READ_LINK_STATUS:
            // TODO: return accurate value here
            tr_debug("Trying to read link status\n");
            break;
        /* Convert between LQI and RSSI */
        case PHY_EXTENSION_CONVERT_SIGNAL_INFO:
            // TODO: return accurate value here
            tr_debug("Trying to read signal info\n");
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
    switch (address_type)
    {
        /*Set 48-bit address*/
        case PHY_MAC_48BIT:
            // 15.4 does not support 48-bit addressing
            ret_val = -1;
            break;
        /*Set 64-bit MAC address*/
        case PHY_MAC_64BIT:
            /* Store MAC in MSB order */
            memcpy(MAC_address, address_ptr, 8);
            tr_debug("MACw ");
            for(unsigned int i = 0; i < sizeof(MAC_address); i ++) {
                tr_debug("%02x:", MAC_address[i]);
            }
            tr_debug("\n");
            /* Pass MAC to the RF driver in LSB order */
            uint8_t MAC_reversed[8];
            for(unsigned int i = 0; i < sizeof(MAC_address); i ++) {
                MAC_reversed[i] = MAC_address[sizeof(MAC_address) - 1 - i];
            }
            RAIL_IEEE802154_SetLongAddress(MAC_reversed);
            break;
        /*Set 16-bit address*/
        case PHY_MAC_16BIT:
            short_address = address_ptr[0] << 8 | address_ptr[1];
            tr_debug("Filter EUI16 %04x\n", short_address);
            RAIL_IEEE802154_SetShortAddress(short_address);
            break;
        /*Set PAN Id*/
        case PHY_MAC_PANID:
            PAN_address = address_ptr[0] << 8 | address_ptr[1];
            tr_debug("Filter PAN %04x\n", PAN_address);
            RAIL_IEEE802154_SetPanId(PAN_address);
            break;
    }
    return ret_val;
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

NanostackRfPhyEfr32::NanostackRfPhyEfr32() : NanostackRfPhy()
{
    // Do nothing
}

NanostackRfPhyEfr32::~NanostackRfPhyEfr32()
{
    rf_unregister();
}

int8_t NanostackRfPhyEfr32::rf_register()
{

    rf_if_lock();

    if (rf != NULL) {
        rf_if_unlock();
        error("Multiple registrations of NanostackRfPhyEfr32 not supported");
        return -1;
    }

    int8_t radio_id = rf_device_register();
    if (radio_id < 0) {
        rf = NULL;
    } else {
        rf = this;
    }

    rf_if_unlock();
    return radio_id;
}

void NanostackRfPhyEfr32::rf_unregister()
{
    rf_if_lock();

    if (rf != this) {
        rf_if_unlock();
        return;
    }

    rf_device_unregister();
    rf = NULL;

    rf_if_unlock();
}

void NanostackRfPhyEfr32::get_mac_address(uint8_t *mac)
{
    rf_if_lock();

    memcpy(mac, MAC_address, sizeof(MAC_address));

    rf_if_unlock();
}

void NanostackRfPhyEfr32::set_mac_address(uint8_t *mac)
{
    rf_if_lock();

    if (NULL != rf) {
        error("NanostackRfPhyEfr32 cannot change mac address when running");
        rf_if_unlock();
        return;
    }
    
    memcpy(MAC_address, mac, sizeof(MAC_address));

    rf_if_unlock();
}

uint32_t NanostackRfPhyEfr32::get_driver_version()
{
    RAIL_Version_t railversion;
    RAIL_VersionGet(&railversion, true);

    return (railversion.major << 24) |
           (railversion.minor << 16) |
           (railversion.rev   << 8)  |
           (railversion.build);
}


//====================== RAIL-defined callbacks =========================
/**
 * Callback that lets the app know when the radio has finished init
 * and is ready.
 */
void RAILCb_RfReady(void) {
    radio_state = RADIO_IDLE;
}

/**
 * Interrupt level callback
 * Allows the user finer granularity in tx radio events.
 *
 * Radio Statuses:
 * RAIL_TX_CONFIG_BUFFER_UNDERFLOW
 * RAIL_TX_CONFIG_CHANNEL_BUSY
 *
 * @param[in] status A bit field that defines what event caused the callback
 */
void RAILCb_TxRadioStatus(uint8_t status) {
   if(device_driver.phy_tx_done_cb != NULL) {
    if(status == RAIL_TX_CONFIG_BUFFER_UNDERFLOW ||
       status == RAIL_TX_CONFIG_CHANNEL_BUSY ||
       status == RAIL_TX_CONFIG_TX_ABORTED ||
       status == RAIL_TX_CONFIG_TX_BLOCKED) {
        waiting_for_ack = false;
        device_driver.phy_tx_done_cb( rf_radio_driver_id,
                                      current_tx_handle,
                                      PHY_LINK_CCA_FAIL,
                                      8,
                                      1);
    } else {
        tr_debug("Packet TX error %d\n", status);
    }
  }
  radio_state = RADIO_RX;
}

/**
 * Called whenever an enabled radio status event occurs
 *
 * Triggers:
 *  RAIL_RX_CONFIG_PREAMBLE_DETECT
 *  RAIL_RX_CONFIG_SYNC1_DETECT
 *  RAIL_RX_CONFIG_SYNC2_DETECT
 *  RAIL_RX_CONFIG_INVALID_CRC
 *  RAIL_RX_CONFIG_BUFFER_OVERFLOW
 *  RAIL_RX_CONFIG_ADDRESS_FILTERED
 *
 * @param[in] status The event that triggered this callback
 */
void RAILCb_RxRadioStatus(uint8_t status) {
    switch(status) {
        case RAIL_RX_CONFIG_ADDRESS_FILTERED:
            break;
        default:
            tr_debug("RXE %d\n", status);
            break;
    }
}

/**
 * Callback that notifies the application that a calibration is needed.
 *
 * This callback function is called whenever the RAIL library detects that a
 * calibration is needed. It is up to the application to determine a valid
 * window to call RAIL_CalStart().
 *
 */
void RAILCb_CalNeeded(void) {
  // TODO: Implement on-the-fly recalibration
  tr_debug("!!!! Calling for calibration\n");
}

/**
 * Interrupt level callback to signify when the radio changes state.
 *
 * @param[in] state Current state of the radio, as defined by EFR32 data sheet
 */
void RAILCb_RadioStateChanged(uint8_t state) {
    return;
}

/**
 * This function is called when the RAIL timer expires
 *
 * You must implement a stub for this in your RAIL application even if you
 * don't use the timer.
 */
void RAILCb_TimerExpired(void) {
}

/**
 * Interrupt level callback to signify when the packet was sent
 * @param txPacketInfo Information about the packet that was transmitted.
 * @note that this structure is only valid during the timeframe of the
 * callback.
 */
void RAILCb_TxPacketSent(RAIL_TxPacketInfo_t *txPacketInfo) {
  if(device_driver.phy_tx_done_cb != NULL) {
    device_driver.phy_tx_done_cb( rf_radio_driver_id,
                                  current_tx_handle,
                                  // Normally we'd switch on ACK requested here, but Nanostack does that for us.
                                  PHY_LINK_TX_SUCCESS,
                                  // Succeeded, so how many times we tried is really not relevant.
                                  1,
                                  1);
  }
  last_tx = RAIL_GetTime();
  radio_state = RADIO_RX;
}

/**
 * Receive packet callback.
 *
 * @param[in] rxPacketHandle Contains a handle that points to the memory that
 *   the packet was stored in. This handle will be the same as something
 *   returned by the RAILCb_AllocateMemory() API. To convert this into a receive
 *   packet info struct use the *** function.
 *
 * This function is called whenever a packet is received and returns to you the
 * memory handle for where this received packet and its appended information was
 * stored. After this callback is done we will release the memory handle so you
 * must somehow increment a reference count or copy the data out within this
 * function.
 */
void RAILCb_RxPacketReceived(void *rxPacketHandle) {
    RAIL_RxPacketInfo_t* rxPacketInfo = (RAIL_RxPacketInfo_t*) memoryPtrFromHandle(rxPacketHandle);
    if(rxPacketInfo->appendedInfo.crcStatus) {
        /* If this is an ACK, deal with it */
        if( rxPacketInfo->dataLength == 4                         && 
            rxPacketInfo->dataPtr[3] == (current_tx_sequence)     &&
            waiting_for_ack) {
            /* Tell the radio to not ACK an ACK */
            RAIL_AutoAckCancelAck();
            waiting_for_ack = false;
            /* Save the pending bit */
            last_ack_pending_bit = (rxPacketInfo->dataPtr[1] & (1 << 4)) != 0;
            /* Tell the stack we got an ACK */
            //tr_debug("rACK\n");
            device_driver.phy_tx_done_cb( rf_radio_driver_id,
                                          current_tx_handle,
                                          last_ack_pending_bit ? PHY_LINK_TX_DONE_PENDING : PHY_LINK_TX_DONE,
                                          1,
                                          1);
        } else {
            /* Figure out whether we want to not ACK this packet */

            /*
            * dataPtr[0] = length
            * dataLength = length w/o length byte
            * dataptr[1:2] = 0x61C9 -> 0b01100001 0b1100 1001 (version 1, dest 3, src 2, ACKreq, type = 1)
            *   [1] => b[0:2] frame type, b[3] = security enabled, b[4] = frame pending, b[5] = ACKreq, b[6] = intrapan
            *   [2] => b[2:3] destmode, b[4:5] version, b[6:7] srcmode
            */
            if( (rxPacketInfo->dataPtr[1] & (1 << 5)) == 0 ) {
                /* Cancel the ACK if the sender did not request one */
                RAIL_AutoAckCancelAck();
            }

            //tr_debug("rPKT %d\n", rxPacketInfo->dataLength);
            /* Feed the received packet into the stack */
            device_driver.phy_rx_cb(rxPacketInfo->dataPtr + 1, 
                                    rxPacketInfo->dataLength - 1, 
                                    //TODO: take a new RAIL release that exposes LQI, or have LQI as function of RSSI
                                    255, 
                                    rxPacketInfo->appendedInfo.rssiLatch, 
                                    rf_radio_driver_id);
        }
    }
}


/**
 * Callback for when a Data Request is being received
 *
 * @param address The source address of the data request command
 *
 * This function is called when the command byte of an incoming frame is for a
 * data request, which requests an ACK. This callback will be called before the
 * packet is fully received, to allow the node to have more time to decide
 * whether to set frame pending in the outgoing ACK.
 */
void RAILCb_IEEE802154_DataRequestCommand(RAIL_IEEE802154_Address_t *address) {
    if(data_pending) {
        RAIL_IEEE802154_SetFramePending();
    }
}

/**
 * Callback that notifies the application when searching for an ACK has timed
 * out.
 *
 * @return void
 *
 * This callback function is called whenever the timeout for searching for an
 * ack is exceeded.
 */
void RAILCb_RxAckTimeout(void) {
    if(waiting_for_ack) {
        tr_debug("nACK\n");
        waiting_for_ack = false;
        device_driver.phy_tx_done_cb( rf_radio_driver_id,
                                      current_tx_handle,
                                      PHY_LINK_TX_FAIL,
                                      1,
                                      1);
    }
}

/**
 * Function to check the requested channel against the current channel,
 * and change the radio configuration if necessary.
 *
 * @param channel The new channel number requested
 * @return bool True if able to switch to the requested channel
 *
 */
static bool rail_checkAndSwitchChannel(uint8_t newChannel) {
    if(channel == newChannel) {
        return true;
    }

    if(newChannel > 0 && newChannel < 11) {
        if(MBED_CONF_SL_RAIL_BAND == 915) {
            channel = newChannel;
            return true;
        } else {
            return false;
        }
    } else if(newChannel >= 11 && newChannel <= 26) {
        if(MBED_CONF_SL_RAIL_BAND == 2400) {
            channel = newChannel;
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

/**
 * Callback that fires when the receive fifo exceeds the configured threshold
 * value
 *
 * @param[in] bytesAvailable Number of bytes available in the receive fifo at
 * the time of the callback dispatch
 *
 * @return void
 * @warning You must implement a stub for this in your RAIL application.
 *
 * Callback that fires when the receive fifo exceeds the configured threshold
 * value.  Provides the number of bytes available in the receive fifo at the
 * time of the callback dispatch.
 */
void RAILCb_RxFifoAlmostFull(uint16_t bytesAvailable) {
    tr_debug("RX near full (%d)\n", bytesAvailable);
}

/**
 * Callback that fires when the transmit fifo falls under the configured
 * threshold value
 *
 * @param[in] spaceAvailable Number of bytes open in the transmit fifo at the
 * time of the callback dispatch
 *
 * @return void
 * @warning You must implement a stub for this in your RAIL application.
 *
 * Callback that fires when the transmit fifo falls under the configured
 * threshold value. It only fires if a rising edge occurs across this
 * threshold. This callback will not fire on initailization nor after resetting
 * the transmit fifo with RAIL_ResetFifo().
 *
 * Provides the number of bytes open in the transmit fifo at the time of the
 * callback dispatch.
 */
void RAILCb_TxFifoAlmostEmpty(uint16_t spaceAvailable) {
    tr_debug("TX near empty (%d)\n", spaceAvailable);
}

/**
 * Callback for when AGC averaged RSSI is done
 *
 * @param avgRssi Contains the the RSSI in quarter dBm (dbm*4) on success and
 * returns \ref RAIL_RSSI_INVALID if there was a problem computing the result.
 *
 * Called in response to RAIL_StartAverageRSSI() to indicate that the hardware
 * has completed averaging. If you would like you can instead use the
 * RAIL_AverageRSSIReady() to wait for completion and RAIL_GetAverageRSSI() to
 * get the result.
 */
void RAILCb_RssiAverageDone(int16_t avgRssi) {
    tr_debug("RSSI done (%d)\n", avgRssi);
}