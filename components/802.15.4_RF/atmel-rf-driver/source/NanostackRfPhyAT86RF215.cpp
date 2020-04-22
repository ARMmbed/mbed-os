/*
 * Copyright (c) 2020 ARM Limited. All rights reserved.
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

#include <string.h>

#if defined(MBED_CONF_NANOSTACK_CONFIGURATION) && DEVICE_SPI && DEVICE_INTERRUPTIN && defined(MBED_CONF_RTOS_PRESENT)

#include "ns_types.h"
#include "platform/arm_hal_interrupt.h"
#include "platform/mbed_wait_api.h"
#include "nanostack/platform/arm_hal_phy.h"
#include "NanostackRfPhyAtmel.h"
#include "AT86RF215Reg.h"
#include "mbed_trace.h"
#include "common_functions.h"
#include <Timer.h>
#include "Timeout.h"
#include "SPI.h"

#define TRACE_GROUP "AtRF"

#define RF_MTU_15_4_2011    127
#define RF_MTU_15_4G_2012   2047

namespace {

typedef enum {
    RF_NOP = 0x00,
    RF_SLEEP = 0x01,
    RF_TRX_OFF = 0x02,
    RF_TXPREP = 0x03,
    RF_TX = 0x04,
    RF_RX = 0x05,
    RF_TRANSITION = 0x06,
    RF_RESET = 0x07
} rf_command_e;

typedef enum {
    COMMON = 0x00,
    RF_09 = 0x01,
    RF_24 = 0x02,
    BBC0 = 0x03,
    BBC1 = 0x04
} rf_modules_e;

typedef enum {
    RF_IDLE,
    RF_CSMA_STARTED,
    RF_CSMA_WHILE_RX,
    RF_TX_STARTED,
    RF_RX_STARTED
} rf_states_e;

} // anonymous namespace

static void rf_init(void);
static int8_t rf_address_write(phy_address_type_e address_type, uint8_t *address_ptr);
static int8_t rf_extension(phy_extension_type_e extension_type, uint8_t *data_ptr);
static int8_t rf_interface_state_control(phy_interface_state_e new_state, uint8_t rf_channel);
static int8_t rf_start_csma_ca(uint8_t *data_ptr, uint16_t data_length, uint8_t tx_handle, data_protocol_e data_protocol);
static void rf_init_registers(rf_modules_e module);
static void rf_spi_exchange(const void *tx, size_t tx_len, void *rx, size_t rx_len);
static uint8_t rf_read_rf_register(uint8_t addr, rf_modules_e module);
static void rf_write_rf_register(uint8_t addr, rf_modules_e module, uint8_t data);
static void rf_write_rf_register_field(uint8_t addr, rf_modules_e module, uint8_t field, uint8_t value);
static void rf_write_tx_packet_length(uint16_t packet_length, rf_modules_e module);
static uint16_t rf_read_rx_frame_length(rf_modules_e module);
static void rf_write_tx_buffer(uint8_t *data, uint16_t len, rf_modules_e module);
static int rf_read_rx_buffer(uint16_t length, rf_modules_e module);
static void rf_irq_rf_enable(uint8_t irq, rf_modules_e module);
static void rf_irq_rf_disable(uint8_t irq, rf_modules_e module);
static void rf_irq_bbc_enable(uint8_t irq, rf_modules_e module);
static void rf_irq_bbc_disable(uint8_t irq, rf_modules_e module);
static rf_command_e rf_read_state(rf_modules_e module);
static void rf_poll_state_change(rf_command_e state, rf_modules_e module);
static void rf_change_state(rf_command_e state, rf_modules_e module);
static void rf_receive(uint16_t rx_channel, rf_modules_e module);
static void rf_interrupt_handler(void);
static void rf_irq_task_process_irq(void);
static void rf_handle_cca_ed_done(void);
static void rf_start_tx(void);
static void rf_backup_timer_interrupt(void);
static void rf_backup_timer_stop(void);
static uint32_t  rf_backup_timer_start(uint16_t bytes, uint32_t time_us);
static int rf_set_channel(uint16_t channel, rf_modules_e module);
static int rf_set_ch0_frequency(uint32_t frequency, rf_modules_e module);
static int rf_set_channel_spacing(uint32_t channel_spacing, rf_modules_e module);
static int rf_set_fsk_symbol_rate_configuration(uint32_t symbol_rate, rf_modules_e module);
static void rf_calculate_symbol_rate(uint32_t baudrate, phy_modulation_e modulation);
static void rf_conf_set_cca_threshold(uint8_t percent);
// Defined register read/write functions
#define rf_read_bbc_register(x, y) rf_read_rf_register(x, (rf_modules_e)(y + 2))
#define rf_read_common_register(x) rf_read_rf_register(x, COMMON)
#define rf_write_bbc_register(x, y, z) rf_write_rf_register(x, (rf_modules_e)(y + 2), z)
#define rf_write_common_register(x, z) rf_write_rf_register(x, COMMON, z)
#define rf_write_bbc_register_field(v, x, y, z) rf_write_rf_register_field(v, (rf_modules_e)(x + 2), y, z)
#define rf_write_common_register_field(v, y, z) rf_write_rf_register_field(v, COMMON, y, z)

static int8_t rf_radio_driver_id = -1;
static phy_device_driver_s device_driver;
static uint8_t rf_version_num = 0;
static rf_modules_e rf_module = RF_24;
static phy_802_15_4_mode_t mac_mode = IEEE_802_15_4_2011;
static uint8_t mac_tx_handle = 0;
static rf_states_e rf_state = RF_IDLE;
static bool receiver_enabled = false;
static int8_t cca_prepare_status = PHY_TX_NOT_ALLOWED;
static uint8_t rx_buffer[RF_MTU_15_4G_2012];
static uint8_t rf_rx_channel;
static uint16_t tx_sequence = 0xffff;
static uint16_t cur_tx_packet_len = 0xffff;
static uint16_t cur_rx_packet_len = 0xffff;
static uint32_t cur_rx_stop_time = 0;
static uint32_t tx_time = 0;
static uint32_t rx_time = 0;
static uint8_t rf09_irq_mask = 0;
static uint8_t rf24_irq_mask = 0;
static uint8_t bbc0_irq_mask = 0;
static uint8_t bbc1_irq_mask = 0;

static bool rf_update_config = false;
static int8_t cca_threshold = -80;
static bool cca_enabled = true;
static uint32_t rf_symbol_rate;

/* Channel configurations for 2.4 and sub-GHz */
static const phy_rf_channel_configuration_s phy_24ghz = {.channel_0_center_frequency = 2350000000U,
                                                         .channel_spacing = 5000000U,
                                                         .datarate = 250000U,
                                                         .number_of_channels = 16U,
                                                         .modulation = M_OQPSK
                                                        };
static const phy_rf_channel_configuration_s phy_subghz = {.channel_0_center_frequency = 868300000U,
                                                          .channel_spacing = 2000000U,
                                                          .datarate = 250000U,
                                                          .number_of_channels = 11U,
                                                          .modulation = M_OQPSK
                                                         };

static phy_rf_channel_configuration_s phy_current_config;

static const phy_device_channel_page_s phy_channel_pages[] = {
    { CHANNEL_PAGE_0, &phy_24ghz},
    { CHANNEL_PAGE_2, &phy_subghz},
    { CHANNEL_PAGE_0, NULL}
};

using namespace mbed;
using namespace rtos;

#include "rfbits.h"
static RFBits *rf;
static TestPins *test_pins;

#define MAC_FRAME_TYPE_MASK     0x07
#define MAC_TYPE_ACK            (2)
#define MAC_DATA_PENDING        0x10
#define FC_AR                   0x20
#define VERSION_FIELD_MASK      0x30
#define SHIFT_VERSION_FIELD     (4)

#define SIG_RADIO           1
#define SIG_TIMER_BACKUP    2
#define SIG_TIMER_CCA       4
#define SIG_TIMERS (SIG_TIMER_BACKUP|SIG_TIMER_CCA)
#define SIG_ALL (SIG_RADIO|SIG_TIMERS)

#define ACK_FRAME_LENGTH    3
#define PACKET_PROCESSING_TIME          5000
#define MAX_STATE_TRANSITION_TIME_US    1000
#define CCA_BACKUP_TIMEOUT              1000
#define MAX_TRANSMISSION_TIME           1000000

#define MIN_CCA_THRESHOLD  -117
#define MAX_CCA_THRESHOLD  -5

static uint32_t rf_get_timestamp(void)
{
    return (uint32_t)rf->tx_timer.read_us();
}

static void rf_lock(void)
{
    platform_enter_critical();
}

static void rf_unlock(void)
{
    platform_exit_critical();
}

static int8_t rf_device_register(const uint8_t *mac_addr)
{
    rf_init();
    device_driver.PHY_MAC = (uint8_t *)mac_addr;
    device_driver.driver_description = (char *)"ATMEL_MAC";
    device_driver.link_type = PHY_LINK_15_4_2_4GHZ_TYPE;
    device_driver.phy_channel_pages = phy_channel_pages;
    device_driver.phy_MTU = RF_MTU_15_4G_2012;
    device_driver.phy_header_length = 0;
    device_driver.phy_tail_length = 0;
    device_driver.address_write = &rf_address_write;
    device_driver.extension = &rf_extension;
    device_driver.state_control = &rf_interface_state_control;
    device_driver.tx = &rf_start_csma_ca;
    device_driver.phy_rx_cb = NULL;
    device_driver.phy_tx_done_cb = NULL;
    rf_radio_driver_id = arm_net_phy_register(&device_driver);
    rf_update_config = true;
    return rf_radio_driver_id;
}

static int8_t rf_address_write(phy_address_type_e address_type, uint8_t *address_ptr)
{
    uint8_t rf_address = 0, addr_size = 0;
    switch (address_type) {
        case PHY_MAC_48BIT:
            break;
        case PHY_MAC_64BIT:
            rf_address = BBC_MACEA0;
            addr_size = 8;
            break;
        case PHY_MAC_16BIT:
            rf_address = BBC_MACSHA0F0;
            addr_size = 2;
            break;
        case PHY_MAC_PANID:
            rf_address = BBC_MACPID0F0;
            addr_size = 2;
            break;
    }
    for (uint8_t i = 0; i < addr_size; i++) {
        rf_write_bbc_register(rf_address++, rf_module, address_ptr[(addr_size - 1) - i]);
    }
    return 0;
}

static int8_t rf_extension(phy_extension_type_e extension_type, uint8_t *data_ptr)
{
    phy_csma_params_t *csma_params;
    uint32_t *timer_value;
    switch (extension_type) {
        case PHY_EXTENSION_SET_CHANNEL:
            if (rf_state == RF_IDLE || (rf_state == RF_CSMA_STARTED && !(rf_read_rf_register(RF_EDC, rf_module) & RF_EDSINGLE))) {
                rf_receive(*data_ptr, rf_module);
            } else {
                return -1;
            }
            break;
        case PHY_EXTENSION_READ_RX_TIME:
            common_write_32_bit(rx_time, data_ptr);
            break;
        case PHY_EXTENSION_GET_TIMESTAMP:
            timer_value = (uint32_t *)data_ptr;
            *timer_value = rf_get_timestamp();
            break;
        case PHY_EXTENSION_SET_CSMA_PARAMETERS:
            csma_params = (phy_csma_params_t *)data_ptr;
            if (csma_params->backoff_time == 0) {
                TEST_CSMA_DONE
                rf->cca_timer.detach();
                if (rf_state == RF_TX_STARTED) {
                    rf_state = RF_IDLE;
                    rf_receive(rf_rx_channel, rf_module);
                }
                tx_time = 0;
            } else {
                tx_time = csma_params->backoff_time;
                cca_enabled = csma_params->cca_enabled;
            }
            break;
        case PHY_EXTENSION_GET_SYMBOLS_PER_SECOND:
            timer_value = (uint32_t *)data_ptr;
            *timer_value = rf_symbol_rate;
            break;
        case PHY_EXTENSION_DYNAMIC_RF_SUPPORTED:
            *data_ptr = true;
            break;
        case PHY_EXTENSION_SET_RF_CONFIGURATION:
            memcpy(&phy_current_config, data_ptr, sizeof(phy_rf_channel_configuration_s));
            rf_calculate_symbol_rate(phy_current_config.datarate, phy_current_config.modulation);
            rf_update_config = true;
            if (rf_state == RF_IDLE) {
                rf_receive(rf_rx_channel, rf_module);
            }
            break;
        case PHY_EXTENSION_SET_CCA_THRESHOLD:
            rf_conf_set_cca_threshold(*data_ptr);
            break;
        case PHY_EXTENSION_SET_802_15_4_MODE:
            mac_mode = (phy_802_15_4_mode_t) *data_ptr; // *NOPAD*
            if (mac_mode == IEEE_802_15_4_2011) {
                rf_module = RF_24;
            } else if (mac_mode == IEEE_802_15_4G_2012) {
                rf_module = RF_09;
            }
            break;
        default:
            break;
    }
    return 0;
}

static int8_t rf_interface_state_control(phy_interface_state_e new_state, uint8_t rf_channel)
{
    int8_t ret_val = 0;
    switch (new_state) {
        case PHY_INTERFACE_RESET:
            break;
        case PHY_INTERFACE_DOWN:
            break;
        case PHY_INTERFACE_UP:
            rf_receive(rf_channel, rf_module);
            break;
        case PHY_INTERFACE_RX_ENERGY_STATE:
            break;
        case PHY_INTERFACE_SNIFFER_STATE:
            break;
    }
    return ret_val;
}

#ifdef TEST_GPIOS_ENABLED
static void test1_toggle(void)
{
    if (test_pins->TEST4) {
        test_pins->TEST4 = 0;
    } else {
        test_pins->TEST4 = 1;
    }
}
static void test2_toggle(void)
{
    if (test_pins->TEST5) {
        test_pins->TEST5 = 0;
    } else {
        test_pins->TEST5 = 1;
    }
}
#endif //TEST_GPIOS_ENABLED

static void rf_init(void)
{
#ifdef TEST_GPIOS_ENABLED
    fhss_bc_switch = test1_toggle;
    fhss_uc_switch = test2_toggle;
#endif //TEST_GPIOS_ENABLED
    rf_lock();
    // Disable interrupts
    rf_write_rf_register(RF_IRQM, RF_09, 0);
    rf_write_rf_register(RF_IRQM, RF_24, 0);
    // Ensure basebands enabled, I/Q IF's disabled
    rf_write_rf_register_field(RF_IQIFC1, COMMON, CHPM, RF_MODE_BBRF);
    rf_change_state(RF_TRX_OFF, RF_09);
    rf_change_state(RF_TRX_OFF, RF_24);
    memcpy(&phy_current_config, &phy_24ghz, sizeof(phy_rf_channel_configuration_s));
    rf_calculate_symbol_rate(phy_current_config.datarate, phy_current_config.modulation);
    rf_init_registers(RF_24);
    rf->IRQ.rise(&rf_interrupt_handler);
    rf->IRQ.enable_irq();
    rf->tx_timer.start();
    rf_unlock();
}

static void rf_init_registers(rf_modules_e module)
{
    // O-QPSK configuration using IEEE Std 802.15.4-2011
    // FSK configuration using IEEE Std 802.15.4g-2012
    if (mac_mode == IEEE_802_15_4_2011) {
        device_driver.link_type = PHY_LINK_15_4_2_4GHZ_TYPE;
        // 16-bit FCS
        rf_write_bbc_register_field(BBC_PC, module, FCST, FCST);
        // Enable O-QPSK
        rf_write_bbc_register_field(BBC_PC, module, PT, BB_MROQPSK);
        // Chip frequency 2000kchip/s
        rf_write_bbc_register_field(BBC_OQPSKC0, module, FCHIP, BB_FCHIP2000);
        // FCS type legacy O-QPSK is 16-bit
        rf_write_bbc_register_field(BBC_OQPSKC2, module, FCSTLEG, FCS_16);
        // Listen for both MR-O-QPSK and legacy O-QPSK PHY
        rf_write_bbc_register_field(BBC_OQPSKC2, module, RXM, RXM_2);
        // PHY type Legacy O-QPSK
        rf_write_bbc_register_field(BBC_OQPSKPHRTX, module, LEG, LEG);
        // Low pass filter cut-off frequency to 1000 kHz
        rf_write_rf_register_field(RF_TXCUTC, module, LPFCUT, RF_FLC1000KHZ);
        // Set TX filter to sample frequency / 2
        rf_write_rf_register_field(RF_TXDFE, module, RCUT, RCUT_4);
        // Enable auto ack
        rf_write_bbc_register_field(BBC_AMCS, module, AACK, AACK);
        // Enable address filter unit 0
        rf_write_bbc_register_field(BBC_AFC0, module, AFEN0, AFEN0);
        // Allow Ack frame type with address filter
        rf_write_bbc_register_field(BBC_AFFTM, module, TYPE_2, TYPE_2);
    } else if (mac_mode == IEEE_802_15_4G_2012) {
        device_driver.link_type = PHY_LINK_15_4_SUBGHZ_TYPE;
        // Enable FSK
        rf_write_bbc_register_field(BBC_PC, module, PT, BB_MRFSK);
        // Disable auto ack
        rf_write_bbc_register_field(BBC_AMCS, module, AACK, 0);
        // Disable address filter unit 0
        rf_write_bbc_register_field(BBC_AFC0, module, AFEN0, 0);
        // Set bandwidth time product
        rf_write_bbc_register_field(BBC_FSKC0, module, BT, BT_20);
        // Disable interleaving
        rf_write_bbc_register_field(BBC_FSKC2, module, FECIE, 0);
        // Disable receiver override
        rf_write_bbc_register_field(BBC_FSKC2, module, RXO, RXO_DIS);
        // Set modulation index
        if (phy_current_config.modulation_index == MODULATION_INDEX_0_5) {
            rf_write_bbc_register_field(BBC_FSKC0, module, MIDX, MIDX_05);
            rf_write_rf_register_field(RF_TXDFE, module, RCUT, RCUT_0);
        } else {
            rf_write_bbc_register_field(BBC_FSKC0, module, MIDX, MIDX_10);
            rf_write_rf_register_field(RF_TXDFE, module, RCUT, RCUT_4);
        }
        // Set Gain control settings
        rf_write_rf_register_field(RF_AGCC, module, AVGS, AVGS_8_SAMPLES);
        rf_write_rf_register_field(RF_AGCS, module, TGT, TGT_1);
        // Set symbol rate and related configurations
        rf_set_fsk_symbol_rate_configuration(phy_current_config.datarate, module);
        // Set preamble length
        uint8_t preamble_len = 24;
        if (phy_current_config.datarate < 150000) {
            preamble_len = 8;
        } else if (phy_current_config.datarate < 300000) {
            preamble_len = 12;
        }
        rf_write_bbc_register(BBC_FSKPLL, module, preamble_len);
        rf_write_bbc_register_field(BBC_FSKC3, module, PDT, PDT_6);
    }
    // Disable filtering FCS
    rf_write_bbc_register_field(BBC_PC, module, FCSFE, 0);
    // Set channel spacing
    rf_set_channel_spacing(phy_current_config.channel_spacing, module);
    // Set channel 0 center frequency
    rf_set_ch0_frequency(phy_current_config.channel_0_center_frequency, module);
    // Set channel (must be called after frequency change)
    rf_set_channel(rf_rx_channel, module);
}

static void rf_csma_ca_timer_interrupt(void)
{
    cca_prepare_status = device_driver.phy_tx_done_cb(rf_radio_driver_id, mac_tx_handle, PHY_LINK_CCA_PREPARE, 0, 0);
    if (cca_prepare_status == PHY_TX_NOT_ALLOWED) {
        if (rf_state == RF_CSMA_STARTED) {
            rf_state = RF_IDLE;
        } else if (rf_state == RF_CSMA_WHILE_RX) {
            rf_state = RF_RX_STARTED;
        }
        TEST_CSMA_DONE
        return;
    }
    rf_irq_rf_enable(EDC, RF_09);
    rf_irq_rf_enable(EDC, rf_module);
    rf_write_rf_register_field(RF_EDC, rf_module, EDM, RF_EDSINGLE);
    rf_backup_timer_start(0, CCA_BACKUP_TIMEOUT);
}

static void rf_csma_ca_timer_signal(void)
{
    rf->irq_thread_215.flags_set(SIG_TIMER_CCA);
}

static int8_t rf_start_csma_ca(uint8_t *data_ptr, uint16_t data_length, uint8_t tx_handle, data_protocol_e data_protocol)
{
    rf_lock();
    if (rf_state != RF_IDLE) {
        rf_unlock();
        return -1;
    }
    rf_state = RF_CSMA_STARTED;

    // If Ack is requested, store the MAC sequence. This will be compared with received Ack.
    uint8_t version = ((*(data_ptr + 1) & VERSION_FIELD_MASK) >> SHIFT_VERSION_FIELD);
    if ((version != MAC_FRAME_VERSION_2) && (*data_ptr & FC_AR)) {
        tx_sequence = *(data_ptr + 2);
    }
    rf_write_tx_buffer(data_ptr, data_length, rf_module);
    if (phy_current_config.modulation == M_OQPSK) {
        data_length += 2;
    } else if (phy_current_config.modulation == M_2FSK) {
        data_length += 4;
    }
    rf_write_tx_packet_length(data_length, rf_module);
    mac_tx_handle = tx_handle;

    if (tx_time) {
        uint32_t backoff_time = tx_time - rf_get_timestamp();
        // Max. time to TX can be 65ms, otherwise time has passed already -> send immediately
        if (backoff_time <= 65000) {
            rf->cca_timer.attach_us(rf_csma_ca_timer_signal, backoff_time);
            TEST_CSMA_STARTED
            rf_unlock();
            return 0;
        }
    }
    // Short timeout to start CCA immediately.
    rf->cca_timer.attach_us(rf_csma_ca_timer_signal, 1);
    TEST_CSMA_STARTED
    rf_unlock();
    return 0;
}

static void rf_handle_cca_ed_done(void)
{
    TEST_CSMA_DONE
    rf_backup_timer_stop();
    rf_irq_rf_disable(EDC, RF_09);
    rf_irq_rf_disable(EDC, rf_module);
    if (rf_state == RF_CSMA_WHILE_RX) {
        rf_state = RF_RX_STARTED;
    }

    if ((cca_enabled == true) && (rf_state == RF_RX_STARTED)) {
        uint32_t backup_time = cur_rx_stop_time - rf_get_timestamp();
        if (backup_time > MAX_TRANSMISSION_TIME) {
            backup_time = 0;
        }
        rf_backup_timer_start(0, backup_time + PACKET_PROCESSING_TIME);
        device_driver.phy_tx_done_cb(rf_radio_driver_id, mac_tx_handle, PHY_LINK_CCA_FAIL, 0, 0);
        return;
    }
    if ((cca_enabled == true) && (((int8_t) rf_read_rf_register(RF_EDV, rf_module) > cca_threshold))) {
        rf_state = RF_IDLE;
        device_driver.phy_tx_done_cb(rf_radio_driver_id, mac_tx_handle, PHY_LINK_CCA_FAIL, 0, 0);
        return;
    }
    if (cca_prepare_status == PHY_RESTART_CSMA) {
        device_driver.phy_tx_done_cb(rf_radio_driver_id, mac_tx_handle, PHY_LINK_CCA_OK, 0, 0);
        if (tx_time) {
            uint32_t backoff_time = tx_time - rf_get_timestamp();
            // Max. time to TX can be 65ms, otherwise time has passed already -> send immediately
            if (backoff_time > 65000) {
                backoff_time = 1;
            }
            rf->cca_timer.attach_us(rf_csma_ca_timer_signal, backoff_time);
            TEST_CSMA_STARTED
        }
        return;
    }
    rf_irq_bbc_disable(RXFE, rf_module);
    rf_start_tx();
}

static void rf_handle_tx_done(void)
{
    rf_backup_timer_stop();
    TEST_TX_DONE
    rf_irq_bbc_disable(TXFE, rf_module);
    rf_state = RF_IDLE;
    rf_receive(rf_rx_channel, rf_module);
    device_driver.phy_tx_done_cb(rf_radio_driver_id, mac_tx_handle, PHY_LINK_TX_SUCCESS, 0, 0);
}

static void rf_start_tx(void)
{
    receiver_enabled = false;
    rf_change_state(RF_TXPREP, rf_module);
    rf_irq_bbc_enable(TXFE, rf_module);
    rf_change_state(RF_TX, rf_module);
    rf_state = RF_TX_STARTED;
    TEST_TX_STARTED
    rf_backup_timer_start(cur_tx_packet_len, 0);
}

static void rf_handle_ack(uint8_t seq_number, uint8_t pending)
{
    phy_link_tx_status_e phy_status;
    if (tx_sequence == (uint16_t)seq_number) {
        if (pending) {
            phy_status = PHY_LINK_TX_DONE_PENDING;
        } else {
            phy_status = PHY_LINK_TX_DONE;
        }
        // No CCA attempts done, just waited Ack
        device_driver.phy_tx_done_cb(rf_radio_driver_id, mac_tx_handle, phy_status, 0, 0);
        // Clear TX sequence when Ack is received to avoid duplicate Acks
        tx_sequence = 0xffff;
    }
}

static void rf_handle_rx_done(void)
{
    receiver_enabled = false;
    TEST_RX_DONE
    rf_backup_timer_stop();
    if (rf_state == RF_CSMA_WHILE_RX) {
        rf_state = RF_CSMA_STARTED;
        uint32_t backup_time = tx_time - rf_get_timestamp();
        // Next TX start event must occur in less than 65ms
        if (backup_time > 65000) {
            backup_time = 0;
        }
        rf_backup_timer_start(0, backup_time + CCA_BACKUP_TIMEOUT);
    } else if (rf_state == RF_RX_STARTED) {
        rf_state = RF_IDLE;
    }
    if (rf_read_bbc_register(BBC_PC, rf_module) & FCSOK) {
        if (!rf_read_rx_buffer(cur_rx_packet_len, rf_module)) {
            uint8_t version = ((rx_buffer[1] & VERSION_FIELD_MASK) >> SHIFT_VERSION_FIELD);
            if (((rx_buffer[0] & MAC_FRAME_TYPE_MASK) == MAC_TYPE_ACK) && (version < MAC_FRAME_VERSION_2)) {
                rf_handle_ack(rx_buffer[2], rx_buffer[0] & MAC_DATA_PENDING);
            } else {
                int8_t rssi = (int8_t) rf_read_rf_register(RF_EDV, rf_module);
                if (phy_current_config.modulation == M_OQPSK) {
                    cur_rx_packet_len -= 2;
                } else if (phy_current_config.modulation == M_2FSK) {
                    cur_rx_packet_len -= 4;
                }
                device_driver.phy_rx_cb(rx_buffer, cur_rx_packet_len, 0xf0, rssi, rf_radio_driver_id);
                // If auto ack used, must wait until RF returns to RF_TXPREP state
                if ((version != MAC_FRAME_VERSION_2) && (rx_buffer[0] & FC_AR)) {
                    wait_us(100);
                    rf_poll_state_change(RF_TXPREP, rf_module);
                }
            }
        }
    } else {
        if (device_driver.phy_rf_statistics) {
            device_driver.phy_rf_statistics->crc_fails++;
        }
        device_driver.phy_rx_cb(NULL, 0, 0, 0, rf_radio_driver_id);
    }
    rf_receive(rf_rx_channel, rf_module);
}

static void rf_handle_rx_start(void)
{
    rx_time = rf_get_timestamp();
    cur_rx_packet_len = rf_read_rx_frame_length(rf_module);
    if (!cur_rx_packet_len || cur_rx_packet_len > device_driver.phy_MTU) {
        return;
    }
    if (rf_state == RF_CSMA_STARTED) {
        rf_backup_timer_stop();
        rf_state = RF_CSMA_WHILE_RX;
    } else {
        rf_state = RF_RX_STARTED;
    }
    TEST_RX_STARTED
    cur_rx_stop_time = rf_backup_timer_start(cur_rx_packet_len, 0);
}

static void rf_receive(uint16_t rx_channel, rf_modules_e module)
{
    if ((receiver_enabled == true) && (rf_update_config == false) && (rx_channel == rf_rx_channel)) {
        return;
    }
    TEST_RX_DONE
    rf_lock();
    if (rf_update_config == true) {
        rf_update_config = false;
        rf_change_state(RF_TRX_OFF, module);
        rf_init_registers(module);
        rf_change_state(RF_TXPREP, module);
    }
    if (rx_channel != rf_rx_channel) {
        rf_change_state(RF_TXPREP, module);
        rf_set_channel(rx_channel, module);
        rf_rx_channel = rx_channel;
    }
    rf_change_state(RF_RX, module);
    rf_irq_bbc_enable(RXFS, module);
    rf_irq_bbc_enable(RXFE, module);
    receiver_enabled = true;
    rf_unlock();

}

static void rf_interrupt_handler(void)
{
    rf->irq_thread_215.flags_set(SIG_RADIO);
}

static void rf_irq_task_process_irq(void)
{
    uint8_t irq_rf09_status = 0, irq_bbc0_status = 0, irq_rf24_status = 0, irq_bbc1_status = 0;
    if (rf09_irq_mask) {
        irq_rf09_status = rf_read_common_register(RF09_IRQS);
        irq_rf09_status &= rf09_irq_mask;
    }
    if (bbc0_irq_mask) {
        irq_bbc0_status = rf_read_common_register(BBC0_IRQS);
        irq_bbc0_status &= bbc0_irq_mask;
    }
    if (rf24_irq_mask) {
        irq_rf24_status = rf_read_common_register(RF24_IRQS);
        irq_rf24_status &= rf24_irq_mask;
    }
    if (bbc1_irq_mask) {
        irq_bbc1_status = rf_read_common_register(BBC1_IRQS);
        irq_bbc1_status &= bbc1_irq_mask;
    }
    if ((rf_state == RF_CSMA_STARTED) || (rf_state == RF_CSMA_WHILE_RX) || (rf_state == RF_RX_STARTED)) {
        if ((irq_rf09_status & EDC) || (irq_rf24_status & EDC)) {
            rf_handle_cca_ed_done();
        }
    }
    if (rf_state == RF_TX_STARTED) {
        if ((irq_bbc0_status & TXFE) || (irq_bbc1_status & TXFE)) {
            rf_handle_tx_done();
        }
    }
    if ((rf_state == RF_IDLE) || (rf_state == RF_CSMA_STARTED)) {
        if ((irq_bbc0_status & RXFS) || (irq_bbc1_status & RXFS)) {
            rf_handle_rx_start();
        }
    }
    if ((rf_state == RF_RX_STARTED) || (rf_state == RF_CSMA_WHILE_RX)) {
        if ((irq_bbc0_status & RXFE) || (irq_bbc1_status & RXFE)) {
            rf_handle_rx_done();
        }
    }
}

static void rf_write_tx_packet_length(uint16_t packet_length, rf_modules_e module)
{
    if (packet_length > device_driver.phy_MTU) {
        return;
    }
    if ((uint8_t)(cur_tx_packet_len >> 8) != (packet_length / 256)) {
        rf_write_bbc_register(BBC_TXFLH, module, packet_length / 256);
    }
    if ((uint8_t)cur_tx_packet_len != (packet_length % 256)) {
        rf_write_bbc_register(BBC_TXFLL, module, packet_length % 256);
    }
    cur_tx_packet_len = packet_length;
}

static uint16_t rf_read_rx_frame_length(rf_modules_e module)
{
    const uint8_t tx[2] = { static_cast<uint8_t>(module + 2), static_cast<uint8_t>(BBC_RXFLL) };
    uint8_t rx[2];
    rf->CS = 0;
    rf_spi_exchange(tx, 2, NULL, 0);
    rf_spi_exchange(NULL, 0, rx, 2);
    rf->CS = 1;
    return (uint16_t)((rx[1] << 8) | rx[0]);
}

static void rf_write_tx_buffer(uint8_t *data, uint16_t len, rf_modules_e module)
{
    uint16_t buffer_addr = BBC0_FBTXS + (0x1000 * (module - 1));
    const uint8_t tx[2] = { static_cast<uint8_t>(0x80 | (buffer_addr >> 8)), static_cast<uint8_t>(buffer_addr) };
    rf->CS = 0;
    rf_spi_exchange(tx, 2, NULL, 0);
    rf_spi_exchange(data, len, NULL, 0);
    rf->CS = 1;
}

static int rf_read_rx_buffer(uint16_t length, rf_modules_e module)
{
    if (length > device_driver.phy_MTU) {
        return -1;
    }
    uint8_t *ptr = rx_buffer;
    uint16_t buffer_addr = BBC0_FBRXS + (0x1000 * (module - 1));
    const uint8_t tx[2] = { static_cast<uint8_t>(buffer_addr >> 8), static_cast<uint8_t>(buffer_addr) };
    rf->CS = 0;
    rf_spi_exchange(tx, 2, NULL, 0);
    rf_spi_exchange(NULL, 0, ptr, length);
    rf->CS = 1;
    return 0;
}

static void rf_irq_rf_enable(uint8_t irq, rf_modules_e module)
{
    if ((module == RF_09) && !(rf09_irq_mask & irq)) {
        rf_write_rf_register_field(RF_IRQM, module, irq, irq);
        rf09_irq_mask |= irq;
    } else if ((module == RF_24) && !(rf24_irq_mask & irq)) {
        rf_write_rf_register_field(RF_IRQM, module, irq, irq);
        rf24_irq_mask |= irq;
    }
}

static void rf_irq_rf_disable(uint8_t irq, rf_modules_e module)
{
    if ((module == RF_09) && (rf09_irq_mask & irq)) {
        rf_write_rf_register_field(RF_IRQM, module, irq, 0);
        rf09_irq_mask &= ~irq;
    } else if ((module == RF_24) && (rf24_irq_mask & irq)) {
        rf_write_rf_register_field(RF_IRQM, module, irq, 0);
        rf24_irq_mask &= ~irq;
    }
}

static void rf_irq_bbc_enable(uint8_t irq, rf_modules_e module)
{
    if ((module == RF_09) && !(bbc0_irq_mask & irq)) {
        rf_write_bbc_register_field(BBC_IRQM, module, irq, irq);
        bbc0_irq_mask |= irq;
    } else if ((module == RF_24) && !(bbc1_irq_mask & irq)) {
        rf_write_bbc_register_field(BBC_IRQM, module, irq, irq);
        bbc1_irq_mask |= irq;
    }
}

static void rf_irq_bbc_disable(uint8_t irq, rf_modules_e module)
{
    if ((module == RF_09) && (bbc0_irq_mask & irq)) {
        rf_write_bbc_register_field(BBC_IRQM, module, irq, 0);
        bbc0_irq_mask &= ~irq;
    } else if ((module == RF_24) && (bbc1_irq_mask & irq)) {
        rf_write_bbc_register_field(BBC_IRQM, module, irq, 0);
        bbc1_irq_mask &= ~irq;
    }
}

static rf_command_e rf_read_state(rf_modules_e module)
{
    return (rf_command_e) rf_read_rf_register(RF_STATE, module);
}

static void rf_poll_state_change(rf_command_e state, rf_modules_e module)
{
    uint32_t transition_start_time = rf_get_timestamp();
    while (rf_read_state(module) != state) {
        if (rf_get_timestamp() > (transition_start_time + MAX_STATE_TRANSITION_TIME_US)) {
            tr_err("Failed to change module %u state from %x to: %x", module, rf_read_state(module), state);
            break;
        }
    }
}

static void rf_change_state(rf_command_e state, rf_modules_e module)
{
    rf_write_rf_register(RF_CMD, module, state);
    return rf_poll_state_change(state, module);
}

static void rf_spi_exchange(const void *tx, size_t tx_len, void *rx, size_t rx_len)
{
    rf->spi.write(static_cast<const char *>(tx), tx_len, static_cast<char *>(rx), rx_len);
}

static uint8_t rf_read_rf_register(uint8_t addr, rf_modules_e module)
{
    const uint8_t tx[2] = { static_cast<uint8_t>(module), static_cast<uint8_t>(addr) };
    uint8_t rx[3];
    rf->CS = 0;
    rf_spi_exchange(tx, 2, rx, 3);
    rf->CS = 1;
    return rx[2];
}

static void rf_write_rf_register(uint8_t addr, rf_modules_e module, uint8_t data)
{
    const uint8_t tx[3] = { static_cast<uint8_t>(0x80 | module), static_cast<uint8_t>(addr), static_cast<uint8_t>(data) };
    uint8_t rx[2];
    rf->CS = 0;
    rf_spi_exchange(tx, 3, rx, 2);
    rf->CS = 1;
}

static void rf_write_rf_register_field(uint8_t addr, rf_modules_e module, uint8_t field, uint8_t value)
{
    uint8_t reg_tmp = rf_read_rf_register(addr, module);
    reg_tmp &= ~field;
    reg_tmp |= value;
    rf_write_rf_register(addr, module, reg_tmp);
}

static void rf_backup_timer_interrupt(void)
{
    receiver_enabled = false;
    rf_read_common_register(RF09_IRQS);
    rf_read_common_register(RF24_IRQS);
    rf_read_common_register(BBC0_IRQS);
    rf_read_common_register(BBC1_IRQS);
    rf_irq_rf_disable(EDC, RF_09);
    rf_irq_rf_disable(EDC, RF_24);
    if (rf_state == RF_RX_STARTED) {
        if (device_driver.phy_rf_statistics) {
            device_driver.phy_rf_statistics->rx_timeouts++;
        }
    } else {
        if (device_driver.phy_rf_statistics) {
            device_driver.phy_rf_statistics->tx_timeouts++;
        }
    }
    if (rf_state == RF_TX_STARTED) {
        device_driver.phy_tx_done_cb(rf_radio_driver_id, mac_tx_handle, PHY_LINK_TX_SUCCESS, 0, 0);
    }
    if ((rf_state == RF_CSMA_STARTED) || (rf_state == RF_CSMA_WHILE_RX)) {
        TEST_CSMA_DONE
        device_driver.phy_tx_done_cb(rf_radio_driver_id, mac_tx_handle, PHY_LINK_CCA_FAIL, 0, 0);
    }
    TEST_TX_DONE
    TEST_RX_DONE
    rf_state = RF_IDLE;
    rf_receive(rf_rx_channel, rf_module);
}

static void rf_backup_timer_signal(void)
{
    rf->irq_thread_215.flags_set(SIG_TIMER_BACKUP);
}

static void rf_backup_timer_stop(void)
{
    rf->cal_timer.detach();
}

static uint32_t rf_backup_timer_start(uint16_t bytes, uint32_t time_us)
{
    if (!time_us) {
        time_us = (uint32_t)(8000000 / phy_current_config.datarate) * bytes + PACKET_PROCESSING_TIME;
    }
    // Using cal_timer as backup timer
    rf->cal_timer.attach_us(rf_backup_timer_signal, time_us);

    return (rf_get_timestamp() + time_us);
}

static int rf_set_channel(uint16_t channel, rf_modules_e module)
{
    rf_write_rf_register(RF_CNL, module, (uint8_t) channel);
    rf_write_rf_register_field(RF_CNM, module, CNH, (uint8_t)(channel >> 8));
    return 0;
}

static int rf_set_ch0_frequency(uint32_t frequency, rf_modules_e module)
{
    if (module == RF_24) {
        frequency -= 1500000000;
    }
    frequency /= 25000;
    rf_write_rf_register(RF_CCF0L, module, (uint8_t)frequency);
    rf_write_rf_register(RF_CCF0H, module, (uint8_t)(frequency >> 8));
    return 0;
}

static int rf_set_channel_spacing(uint32_t channel_spacing, rf_modules_e module)
{
    channel_spacing /= 25000;
    rf_write_rf_register(RF_CS, module, channel_spacing);
    return 0;
}

static int rf_set_fsk_symbol_rate_configuration(uint32_t symbol_rate, rf_modules_e module)
{
    if (symbol_rate == 50000) {
        rf_write_bbc_register_field(BBC_FSKC1, module, SRATE, SRATE_50KHZ);
        if (rf_version_num == 1) {
            rf_write_rf_register_field(RF_TXDFE, module, SR, SR_10);
        } else {
            rf_write_rf_register_field(RF_TXDFE, module, SR, SR_8);
        }
        rf_write_rf_register_field(RF_RXDFE, module, SR, SR_10);
        if (phy_current_config.modulation_index == MODULATION_INDEX_0_5) {
            rf_write_rf_register_field(RF_RXDFE, module, RCUT, RCUT_0);
        } else {
            rf_write_rf_register_field(RF_RXDFE, module, RCUT, RCUT_1);
        }
        rf_write_rf_register_field(RF_TXCUTC, module, PARAMP, RF_PARAMP32U);
        rf_write_rf_register_field(RF_TXCUTC, module, LPFCUT, RF_FLC80KHZ);
        rf_write_rf_register_field(RF_RXBWC, module, BW, RF_BW160KHZ_IF250KHZ);
        rf_write_rf_register_field(RF_RXBWC, module, IFS, 0);
    } else if (symbol_rate == 100000) {
        rf_write_bbc_register_field(BBC_FSKC1, module, SRATE, SRATE_100KHZ);
        if (rf_version_num == 1) {
            rf_write_rf_register_field(RF_TXDFE, module, SR, SR_5);
        } else {
            rf_write_rf_register_field(RF_TXDFE, module, SR, SR_4);
        }
        rf_write_rf_register_field(RF_RXDFE, module, SR, SR_5);
        rf_write_rf_register_field(RF_TXCUTC, module, PARAMP, RF_PARAMP16U);
        if (phy_current_config.modulation_index == MODULATION_INDEX_0_5) {
            rf_write_rf_register_field(RF_RXDFE, module, RCUT, RCUT_0);
            rf_write_rf_register_field(RF_TXCUTC, module, LPFCUT, RF_FLC100KHZ);
            rf_write_rf_register_field(RF_RXBWC, module, BW, RF_BW200KHZ_IF250KHZ);
        } else {
            rf_write_rf_register_field(RF_RXDFE, module, RCUT, RCUT_1);
            rf_write_rf_register_field(RF_TXCUTC, module, LPFCUT, RF_FLC160KHZ);
            rf_write_rf_register_field(RF_RXBWC, module, BW, RF_BW320KHZ_IF500KHZ);
        }
        rf_write_rf_register_field(RF_RXBWC, module, IFS, 0);
    } else if (symbol_rate == 150000) {
        rf_write_bbc_register_field(BBC_FSKC1, module, SRATE, SRATE_150KHZ);
        rf_write_rf_register_field(RF_TXDFE, module, SR, SR_2);
        rf_write_rf_register_field(RF_RXDFE, module, SR, SR_4);
        rf_write_rf_register_field(RF_TXCUTC, module, PARAMP, RF_PARAMP16U);
        if (phy_current_config.modulation_index == MODULATION_INDEX_0_5) {
            rf_write_rf_register_field(RF_RXDFE, module, RCUT, RCUT_0);
            rf_write_rf_register_field(RF_TXCUTC, module, LPFCUT, RF_FLC160KHZ);
            rf_write_rf_register_field(RF_RXBWC, module, BW, RF_BW320KHZ_IF500KHZ);
        } else {
            rf_write_rf_register_field(RF_RXDFE, module, RCUT, RCUT_1);
            rf_write_rf_register_field(RF_TXCUTC, module, LPFCUT, RF_FLC250KHZ);
            rf_write_rf_register_field(RF_RXBWC, module, BW, RF_BW400KHZ_IF500KHZ);
        }
        rf_write_rf_register_field(RF_RXBWC, module, IFS, 0);
    } else if (symbol_rate == 200000) {
        rf_write_bbc_register_field(BBC_FSKC1, module, SRATE, SRATE_200KHZ);
        rf_write_rf_register_field(RF_TXDFE, module, SR, SR_2);
        rf_write_rf_register_field(RF_RXDFE, module, SR, SR_4);
        rf_write_rf_register_field(RF_TXCUTC, module, PARAMP, RF_PARAMP16U);
        if (phy_current_config.modulation_index == MODULATION_INDEX_0_5) {
            rf_write_rf_register_field(RF_RXDFE, module, RCUT, RCUT_1);
            rf_write_rf_register_field(RF_TXCUTC, module, LPFCUT, RF_FLC200KHZ);
            rf_write_rf_register_field(RF_RXBWC, module, BW, RF_BW320KHZ_IF500KHZ);
            rf_write_rf_register_field(RF_RXBWC, module, IFS, 0);
        } else {
            rf_write_rf_register_field(RF_RXDFE, module, RCUT, RCUT_2);
            rf_write_rf_register_field(RF_TXCUTC, module, LPFCUT, RF_FLC315KHZ);
            rf_write_rf_register_field(RF_RXBWC, module, BW, RF_BW500KHZ_IF500KHZ);
            rf_write_rf_register_field(RF_RXBWC, module, IFS, IFS);
        }
    } else if (symbol_rate == 300000) {
        rf_write_bbc_register_field(BBC_FSKC1, module, SRATE, SRATE_300KHZ);
        rf_write_rf_register_field(RF_TXDFE, module, SR, SR_1);
        rf_write_rf_register_field(RF_RXDFE, module, SR, SR_2);
        rf_write_rf_register_field(RF_TXCUTC, module, PARAMP, RF_PARAMP8U);
        if (phy_current_config.modulation_index == MODULATION_INDEX_0_5) {
            rf_write_rf_register_field(RF_RXDFE, module, RCUT, RCUT_0);
            rf_write_rf_register_field(RF_TXCUTC, module, LPFCUT, RF_FLC315KHZ);
            rf_write_rf_register_field(RF_RXBWC, module, BW, RF_BW500KHZ_IF500KHZ);
            rf_write_rf_register_field(RF_RXBWC, module, IFS, IFS);
        } else {
            rf_write_rf_register_field(RF_RXDFE, module, RCUT, RCUT_1);
            rf_write_rf_register_field(RF_TXCUTC, module, LPFCUT, RF_FLC500KHZ);
            rf_write_rf_register_field(RF_RXBWC, module, BW, RF_BW630KHZ_IF1000KHZ);
            rf_write_rf_register_field(RF_RXBWC, module, IFS, 0);
        }
    } else if (symbol_rate == 400000) {
        rf_write_bbc_register_field(BBC_FSKC1, module, SRATE, SRATE_400KHZ);
        rf_write_rf_register_field(RF_TXDFE, module, SR, SR_1);
        rf_write_rf_register_field(RF_RXDFE, module, SR, SR_2);
        rf_write_rf_register_field(RF_TXCUTC, module, PARAMP, RF_PARAMP8U);
        if (phy_current_config.modulation_index == MODULATION_INDEX_0_5) {
            rf_write_rf_register_field(RF_RXDFE, module, RCUT, RCUT_0);
            rf_write_rf_register_field(RF_TXCUTC, module, LPFCUT, RF_FLC400KHZ);
            rf_write_rf_register_field(RF_RXBWC, module, BW, RF_BW630KHZ_IF1000KHZ);
            rf_write_rf_register_field(RF_RXBWC, module, IFS, 0);
        } else {
            rf_write_rf_register_field(RF_RXDFE, module, RCUT, RCUT_1);
            rf_write_rf_register_field(RF_TXCUTC, module, LPFCUT, RF_FLC625KHZ);
            rf_write_rf_register_field(RF_RXBWC, module, BW, RF_BW1000KHZ_IF1000KHZ);
            rf_write_rf_register_field(RF_RXBWC, module, IFS, IFS);
        }
    }
    return 0;
}

static void rf_conf_set_cca_threshold(uint8_t percent)
{
    uint8_t step = (MAX_CCA_THRESHOLD - MIN_CCA_THRESHOLD);
    cca_threshold = MIN_CCA_THRESHOLD + (step * percent) / 100;
}

static void rf_calculate_symbol_rate(uint32_t baudrate, phy_modulation_e modulation)
{
    uint8_t bits_in_symbols = 4;
    if (modulation == M_2FSK) {
        bits_in_symbols = 1;
    }
    rf_symbol_rate = baudrate / bits_in_symbols;
}

void RFBits::rf_irq_task(void)
{
    for (;;) {
        uint32_t flags = ThisThread::flags_wait_any(SIG_ALL);
        rf_lock();
        if (flags & SIG_RADIO) {
            rf_irq_task_process_irq();
        }
        if (flags & SIG_TIMER_CCA) {
            rf_csma_ca_timer_interrupt();
        }
        if (flags & SIG_TIMER_BACKUP) {
            rf_backup_timer_interrupt();
        }
        rf_unlock();
    }
}

int RFBits::init_215_driver(RFBits *_rf, TestPins *_test_pins, const uint8_t mac[8], uint8_t *rf_part_num)
{
    rf = _rf;
    test_pins = _test_pins;
    irq_thread_215.start(mbed::callback(this, &RFBits::rf_irq_task));
    rf->spi.frequency(25000000);
    *rf_part_num = rf_read_common_register(RF_PN);
    rf_version_num = rf_read_common_register(RF_VN);
    tr_info("RF version number: %x", rf_version_num);
    return rf_device_register(mac);
}

#endif // MBED_CONF_NANOSTACK_CONFIGURATION && DEVICE_SPI && DEVICE_INTERRUPTIN && defined(MBED_CONF_RTOS_PRESENT)
