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
#include <string.h>

#if defined(MBED_CONF_NANOSTACK_CONFIGURATION) && DEVICE_SPI && DEVICE_INTERRUPTIN && DEVICE_I2C && defined(MBED_CONF_RTOS_PRESENT)

#include "platform/arm_hal_interrupt.h"
#include "nanostack/platform/arm_hal_phy.h"
#include "ns_types.h"
#include "NanostackRfPhyAtmel.h"
#include "randLIB.h"
#include "AT86RFReg.h"
#include "AT86RF215Reg.h"
#include "nanostack/platform/arm_hal_phy.h"
#include "mbed_trace.h"
#include "mbed_toolchain.h"
#include "DigitalIn.h"
#include "DigitalOut.h"
#include "InterruptIn.h"
#include "SPI.h"
#include "inttypes.h"
#include "Timeout.h"
#include "platform/mbed_error.h"

#define TRACE_GROUP "AtRF"

/*Worst case sensitivity*/
#define RF_DEFAULT_SENSITIVITY -88
/*Run calibration every 5 minutes*/
#define RF_CALIBRATION_INTERVAL 6000000
/*Wait ACK for 2.5ms*/
#define RF_ACK_WAIT_DEFAULT_TIMEOUT 50
/*Base CCA backoff (50us units) - substitutes for Inter-Frame Spacing*/
#define RF_CCA_BASE_BACKOFF 13 /* 650us */
/*CCA random backoff (50us units)*/
#define RF_CCA_RANDOM_BACKOFF 51 /* 2550us */

#define RF_MTU 127

#define RF_PHY_MODE OQPSK_SIN_250

/*Radio RX and TX state definitions*/
#define RFF_ON 0x01
#define RFF_RX 0x02
#define RFF_TX 0x04
#define RFF_CCA 0x08

namespace {

typedef enum {
    RF_MODE_NORMAL = 0,
    RF_MODE_SNIFFER = 1,
    RF_MODE_ED = 2
} rf_mode_t;

/*Atmel RF Part Type*/
typedef enum {
    ATMEL_UNKNOW_DEV = 0,
    ATMEL_AT86RF212,
    ATMEL_AT86RF231, // No longer supported (doesn't give ED+status on frame read)
    ATMEL_AT86RF233
} rf_trx_part_e;

/*Atmel RF states*/
typedef enum {
    NOP = 0x00,
    BUSY_RX = 0x01,
    BUSY_TX = 0x02,
    RF_TX_START = 0x02,
    FORCE_TRX_OFF = 0x03,
    FORCE_PLL_ON = 0x04,
    RX_ON = 0x06,
    TRX_OFF = 0x08,
    PLL_ON = 0x09,
    BUSY_RX_AACK = 0x11,
    SLEEP = 0x0F,
    RX_AACK_ON = 0x16,
    TX_ARET_ON = 0x19,
    STATE_TRANSITION_IN_PROGRESS = 0x1F
} rf_trx_states_t;

} // anonymous namespace

static const uint8_t *rf_tx_data; // Points to Nanostack's buffer
static uint8_t rf_tx_length;
/*ACK wait duration changes depending on data rate*/
static uint16_t rf_ack_wait_duration = RF_ACK_WAIT_DEFAULT_TIMEOUT;

static int8_t rf_sensitivity = RF_DEFAULT_SENSITIVITY;
static rf_mode_t rf_mode = RF_MODE_NORMAL;
static uint8_t radio_tx_power = 0x00;   // Default to +4dBm
static uint8_t rf_phy_channel = 12;
static uint8_t rf_tuned = 1;
static uint8_t rf_use_antenna_diversity = 0;
static int16_t expected_ack_sequence = -1;
static uint8_t rf_rx_mode = 0;
static uint8_t rf_flags = 0;
static int8_t rf_radio_driver_id = -1;
static phy_device_driver_s device_driver;
static uint8_t mac_tx_handle = 0;
static uint8_t xah_ctrl_1;

/* Channel configurations for 2.4 and sub-GHz */
static const phy_rf_channel_configuration_s phy_24ghz = {.channel_0_center_frequency = 2405000000U, .channel_spacing = 5000000U, .datarate = 250000U, .number_of_channels = 16U, .modulation = M_OQPSK};
static const phy_rf_channel_configuration_s phy_subghz = {.channel_0_center_frequency = 868300000U, .channel_spacing = 2000000U, .datarate = 250000U, .number_of_channels = 11U, .modulation = M_OQPSK};

static const phy_device_channel_page_s phy_channel_pages[] = {
    { CHANNEL_PAGE_0, &phy_24ghz},
    { CHANNEL_PAGE_2, &phy_subghz},
    { CHANNEL_PAGE_0, NULL}
};

/**
 *  RF output power write
 *
 * \brief TX power has to be set before network start.
 *
 * \param power
 *              AT86RF233
 *              0 = 4 dBm
 *              1 = 3.7 dBm
 *              2 = 3.4 dBm
 *              3 = 3 dBm
 *              4 = 2.5 dBm
 *              5 = 2 dBm
 *              6 = 1 dBm
 *              7 = 0 dBm
 *              8 = -1 dBm
 *              9 = -2 dBm
 *              10 = -3 dBm
 *              11 = -4 dBm
 *              12 = -6 dBm
 *              13 = -8 dBm
 *              14 = -12 dBm
 *              15 = -17 dBm
 *
 *              AT86RF212B
 *              See datasheet for TX power settings
 *
 * \return 0, Supported Value
 * \return -1, Not Supported Value
 */
static rf_trx_part_e rf_radio_type_read(void);
static void rf_ack_wait_timer_start(uint16_t slots);
static void rf_handle_cca_ed_done(uint8_t full_trx_status);
static void rf_handle_tx_end(rf_trx_states_t trx_status);
static void rf_handle_rx_end(rf_trx_states_t trx_status);
static void rf_on(void);
static void rf_give_up_on_ack(void);
static void rf_receive(rf_trx_states_t trx_status = STATE_TRANSITION_IN_PROGRESS);
static rf_trx_states_t rf_poll_trx_state_change(rf_trx_states_t trx_state);
static void rf_init(void);
static int8_t rf_device_register(const uint8_t *mac_addr);
static void rf_device_unregister(void);
static int8_t rf_start_cca(uint8_t *data_ptr, uint16_t data_length, uint8_t tx_handle, data_protocol_e data_protocol);
static void rf_cca_abort(void);
static void rf_calibration_cb(void);
static void rf_init_phy_mode(void);
static void rf_ack_wait_timer_interrupt(void);
static void rf_calibration_timer_interrupt(void);
static void rf_calibration_timer_start(uint32_t slots);
static void rf_cca_timer_interrupt(void);
static void rf_cca_timer_start(uint32_t slots);
static uint8_t rf_scale_lqi(int8_t rssi);

static int8_t rf_interface_state_control(phy_interface_state_e new_state, uint8_t rf_channel);
static int8_t rf_extension(phy_extension_type_e extension_type, uint8_t *data_ptr);
static int8_t rf_address_write(phy_address_type_e address_type, uint8_t *address_ptr);

static void rf_if_cca_timer_start(uint32_t slots);
static void rf_if_enable_promiscuous_mode(void);
static void rf_if_lock(void);
static void rf_if_unlock(void);
static uint8_t rf_if_read_rnd(void);
static void rf_if_calibration_timer_start(uint32_t slots);
static void rf_if_interrupt_handler(void);
static void rf_if_ack_wait_timer_start(uint16_t slots);
static void rf_if_ack_wait_timer_stop(void);
static void rf_if_ack_pending_ctrl(uint8_t state);
static void rf_if_calibration(void);
static uint8_t rf_if_read_register(uint8_t addr);
static void rf_if_set_bit(uint8_t addr, uint8_t bit, uint8_t bit_mask);
static void rf_if_clear_bit(uint8_t addr, uint8_t bit);
static void rf_if_write_register(uint8_t addr, uint8_t data);
static void rf_if_reset_radio(void);
static void rf_if_enable_ant_div(void);
static void rf_if_disable_ant_div(void);
static void rf_if_enable_slptr(void);
static void rf_if_disable_slptr(void);
static void rf_if_write_antenna_diversity_settings(void);
static void rf_if_write_set_tx_power_register(uint8_t value);
static void rf_if_write_rf_settings(void);
static rf_trx_states_t rf_if_read_trx_state(void);
static uint16_t rf_if_read_packet(uint8_t data[RF_MTU], uint8_t *lqi_out, uint8_t *ed_out, bool *crc_good);
static void rf_if_write_short_addr_registers(uint8_t *short_address);
static uint8_t rf_if_last_acked_pending(void);
static void rf_if_write_pan_id_registers(uint8_t *pan_id);
static void rf_if_write_ieee_addr_registers(uint8_t *address);
static void rf_if_write_frame_buffer(const uint8_t *ptr, uint8_t length);
static rf_trx_states_t rf_if_change_trx_state(rf_trx_states_t trx_state);
static void rf_if_start_cca_process(void);
static int8_t rf_if_scale_rssi(uint8_t ed_level);
static void rf_if_set_channel_register(uint8_t channel);
static void rf_if_enable_promiscuous_mode(void);
static void rf_if_disable_promiscuous_mode(void);
static uint8_t rf_if_read_part_num(void);
static void rf_if_enable_irq(void);
static void rf_if_disable_irq(void);
static void rf_if_spi_exchange_n(const void *tx, size_t tx_len, void *rx, size_t rx_len);

static inline rf_trx_states_t rf_if_trx_status_from_full(uint8_t full_trx_status)
{
    return (rf_trx_states_t)(full_trx_status & 0x1F);
}

#ifdef MBED_CONF_RTOS_PRESENT

#include "rtos.h"

using namespace mbed;
using namespace rtos;

static void rf_if_irq_task_process_irq();

#define SIG_RADIO       1
#define SIG_TIMER_ACK   2
#define SIG_TIMER_CAL   4
#define SIG_TIMER_CCA   8

#define SIG_TIMERS (SIG_TIMER_ACK|SIG_TIMER_CAL|SIG_TIMER_CCA)
#define SIG_ALL (SIG_RADIO|SIG_TIMERS)
#endif

// HW pins to RF chip
#include "rfbits.h"


RFBits::RFBits(PinName spi_mosi, PinName spi_miso,
               PinName spi_sclk, PinName spi_cs,
               PinName spi_rst, PinName spi_slp, PinName spi_irq)
    :   spi(spi_mosi, spi_miso, spi_sclk),
        CS(spi_cs),
        RST(spi_rst),
        SLP_TR(spi_slp),
        IRQ(spi_irq)
#ifdef MBED_CONF_RTOS_PRESENT
    , irq_thread(osPriorityRealtime, MBED_CONF_ATMEL_RF_IRQ_THREAD_STACK_SIZE, NULL, "atmel_irq_thread"),
        irq_thread_215(osPriorityRealtime, MBED_CONF_ATMEL_RF_IRQ_THREAD_STACK_SIZE, NULL, "atmel_215_irq_thread")
#endif
{
#ifdef MBED_CONF_RTOS_PRESENT
    irq_thread.start(mbed::callback(this, &RFBits::rf_if_irq_task));
#endif
}

TestPins::TestPins(PinName test_pin_1, PinName test_pin_2, PinName test_pin_3, PinName test_pin_4, PinName test_pin_5)
    :   TEST1(test_pin_1),
        TEST2(test_pin_2),
        TEST3(test_pin_3),
        TEST4(test_pin_4),
        TEST5(test_pin_5)
{
}

static RFBits *rf;
static TestPins *test_pins;
static uint8_t rf_part_num = 0;
/*TODO: RSSI Base value setting*/
static int8_t rf_rssi_base_val = -91;

static void rf_if_lock(void)
{
    platform_enter_critical();
}

static void rf_if_unlock(void)
{
    platform_exit_critical();
}

#ifdef MBED_CONF_RTOS_PRESENT
static void rf_if_cca_timer_signal(void)
{
    rf->irq_thread.flags_set(SIG_TIMER_CCA);
}

static void rf_if_cal_timer_signal(void)
{
    rf->irq_thread.flags_set(SIG_TIMER_CAL);
}

static void rf_if_ack_timer_signal(void)
{
    rf->irq_thread.flags_set(SIG_TIMER_ACK);
}
#endif

// t1 = 180ns, SEL falling edge to MISO active [SPI setup assumed slow enough to not need manual delay]
#define CS_SELECT()  {rf->CS = 0; /* delay_ns(180); */}
// t9 = 250ns, last clock to SEL rising edge, t8 = 250ns, SPI idle time between consecutive access
#define CS_RELEASE() {wait_ns(250); rf->CS = 1; wait_ns(250);}

/*
 * \brief Read connected radio part.
 *
 * This function only return valid information when rf_init() is called
 *
 * \return
 */
static rf_trx_part_e rf_radio_type_read(void)
{
    rf_trx_part_e ret_val = ATMEL_UNKNOW_DEV;

    switch (rf_part_num) {
        case PART_AT86RF212:
            ret_val = ATMEL_AT86RF212;
            break;
        case PART_AT86RF233:
            ret_val = ATMEL_AT86RF233;
            break;
        default:
            break;
    }

    return ret_val;
}


/*
 * \brief Function starts the ACK wait timeout.
 *
 * \param slots Given slots, resolution 50us
 *
 * \return none
 */
static void rf_if_ack_wait_timer_start(uint16_t slots)
{
#ifdef MBED_CONF_RTOS_PRESENT
    rf->ack_timer.attach_us(rf_if_ack_timer_signal, slots * 50);
#else
    rf->ack_timer.attach_us(rf_ack_wait_timer_interrupt, slots * 50);
#endif
}

/*
 * \brief Function starts the calibration interval.
 *
 * \param slots Given slots, resolution 50us
 *
 * \return none
 */
static void rf_if_calibration_timer_start(uint32_t slots)
{
#ifdef MBED_CONF_RTOS_PRESENT
    rf->cal_timer.attach_us(rf_if_cal_timer_signal, slots * 50);
#else
    rf->cal_timer.attach_us(rf_calibration_timer_interrupt, slots * 50);
#endif
}

/*
 * \brief Function starts the CCA interval.
 *
 * \param slots Given slots, resolution 50us
 *
 * \return none
 */
static void rf_if_cca_timer_start(uint32_t slots)
{
#ifdef MBED_CONF_RTOS_PRESENT
    rf->cca_timer.attach_us(rf_if_cca_timer_signal, slots * 50);
#else
    rf->cca_timer.attach_us(rf_cca_timer_interrupt, slots * 50);
#endif
}

/*
 * \brief Function stops the CCA interval.
 *
 * \return none
 */
static void rf_if_cca_timer_stop(void)
{
    rf->cca_timer.detach();
}

/*
 * \brief Function stops the ACK wait timeout.
 *
 * \param none
 *
 * \return none
 */
static void rf_if_ack_wait_timer_stop(void)
{
    rf->ack_timer.detach();
}

/*
 * \brief Function sets bit(s) in given RF register.
 *
 * \param addr Address of the register to set
 * \param bit Bit(s) to set
 * \param bit_mask Masks the field inside the register
 *
 * \return none
 */
static void rf_if_set_bit(uint8_t addr, uint8_t bit, uint8_t bit_mask)
{
    uint8_t reg = rf_if_read_register(addr);
    reg &= ~bit_mask;
    reg |= bit;
    rf_if_write_register(addr, reg);
}

/*
 * \brief Function clears bit(s) in given RF register.
 *
 * \param addr Address of the register to clear
 * \param bit Bit(s) to clear
 *
 * \return none
 */
static void rf_if_clear_bit(uint8_t addr, uint8_t bit)
{
    rf_if_set_bit(addr, 0, bit);
}

/*
 * \brief Function writes register in RF.
 *
 * \param addr Address on the RF
 * \param data Written data
 *
 * \return none
 */
static void rf_if_write_register(uint8_t addr, uint8_t data)
{
    const uint8_t tx[2] = { static_cast<uint8_t>(0xC0 | addr), data };
    uint8_t rx[2];
    CS_SELECT();
    rf_if_spi_exchange_n(tx, 2, rx, 2);
    CS_RELEASE();
}

/*
 * \brief Function reads RF register, and also outputs PHY_STATUS
 *
 * \param addr Address on the RF
 * \param[out] status_out Pointer to store PHY_STATUS
 *
 * \return Read register data
 */
static uint8_t rf_if_read_register_with_status(uint8_t addr, uint8_t *status_out)
{
    const uint8_t tx[1] = { static_cast<uint8_t>(0x80 | addr) };
    uint8_t rx[2];
    CS_SELECT();
    rf_if_spi_exchange_n(tx, 1, rx, 2);
    CS_RELEASE();
    if (status_out) {
        *status_out = rx[0];
    }
    return rx[1];
}

/*
 * \brief Function reads RF register.
 *
 * \param addr Address on the RF
 *
 * \return Read register data
 */
static uint8_t rf_if_read_register(uint8_t addr)
{
    return rf_if_read_register_with_status(addr, NULL);
}

/*
 * \brief Function resets the RF.
 *
 * \param none
 *
 * \return none
 */
static void rf_if_reset_radio(void)
{
#if MBED_CONF_ATMEL_RF_USE_SPI_SPACING_API
    rf->spi.frequency(MBED_CONF_ATMEL_RF_FULL_SPI_SPEED);
    int spacing = rf->spi.write_spacing(MBED_CONF_ATMEL_RF_FULL_SPI_SPEED_BYTE_SPACING);
    if (spacing < MBED_CONF_ATMEL_RF_FULL_SPI_SPEED_BYTE_SPACING) {
        rf->spi.frequency(MBED_CONF_ATMEL_RF_LOW_SPI_SPEED);
        rf->spi.write_spacing(0);
    }
#elif MBED_CONF_ATMEL_RF_ASSUME_SPACED_SPI
    rf->spi.frequency(MBED_CONF_ATMEL_RF_FULL_SPI_SPEED);
#else
    rf->spi.frequency(MBED_CONF_ATMEL_RF_LOW_SPI_SPEED);
#endif
    rf->IRQ.rise(nullptr);
    rf->RST = 1;
    ThisThread::sleep_for(2);
    rf->RST = 0;
    ThisThread::sleep_for(10);
    CS_RELEASE();
    rf->SLP_TR = 0;
    ThisThread::sleep_for(10);
    rf->RST = 1;
    ThisThread::sleep_for(10);

    rf->IRQ.rise(&rf_if_interrupt_handler);
}

/*
 * \brief Function enables the promiscuous mode.
 *
 * \param none
 *
 * \return none
 */
static void rf_if_enable_promiscuous_mode(void)
{
    if (!(xah_ctrl_1 & AACK_PROM_MODE)) {
        /*Set AACK_PROM_MODE to enable the promiscuous mode*/
        rf_if_write_register(XAH_CTRL_1, xah_ctrl_1 |= AACK_PROM_MODE);
    }
}

/*
 * \brief Function disable the promiscuous mode.
 *
 * \param none
 *
 * \return none
 */
static void rf_if_disable_promiscuous_mode(void)
{
    if (xah_ctrl_1 & AACK_PROM_MODE) {
        /*Clear AACK_PROM_MODE to disable the promiscuous mode*/
        rf_if_write_register(XAH_CTRL_1, xah_ctrl_1 &= ~AACK_PROM_MODE);
    }
}

/*
 * \brief Function enables the Antenna diversity usage.
 *
 * \param none
 *
 * \return none
 */
static void rf_if_enable_ant_div(void)
{
    /*Set ANT_EXT_SW_EN to enable controlling of antenna diversity*/
    rf_if_set_bit(ANT_DIV, ANT_EXT_SW_EN, ANT_EXT_SW_EN);
}

/*
 * \brief Function disables the Antenna diversity usage.
 *
 * \param none
 *
 * \return none
 */
static void rf_if_disable_ant_div(void)
{
    rf_if_clear_bit(ANT_DIV, ANT_EXT_SW_EN);
}

/*
 * \brief Function sets the SLP TR pin.
 *
 * \param none
 *
 * \return none
 */
static void rf_if_enable_slptr(void)
{
    rf->SLP_TR = 1;
}

/*
 * \brief Function clears the SLP TR pin.
 *
 * \param none
 *
 * \return none
 */
static void rf_if_disable_slptr(void)
{
    rf->SLP_TR = 0;
}

/*
 * \brief Function writes the antenna diversity settings.
 *
 * \param none
 *
 * \return none
 */
static void rf_if_write_antenna_diversity_settings(void)
{
    /*Recommended setting of PDT_THRES is 3 when antenna diversity is used*/
    rf_if_set_bit(RX_CTRL, 0x03, 0x0f);
    rf_if_write_register(ANT_DIV, ANT_DIV_EN | ANT_EXT_SW_EN | ANT_CTRL_DEFAULT);
}

/*
 * \brief Function writes the TX output power register.
 *
 * \param value Given register value
 *
 * \return none
 */
static void rf_if_write_set_tx_power_register(uint8_t value)
{
    rf_if_write_register(PHY_TX_PWR, value);
}

/*
 * \brief Function returns the RF part number.
 *
 * \param none
 *
 * \return part number
 */
static uint8_t rf_if_read_part_num(void)
{
    // Part number is already set
    if (rf_part_num) {
        return rf_part_num;
    }
    return rf_if_read_register(PART_NUM);
}

/*
 * \brief Function writes the RF settings and initialises SPI interface.
 *
 * \param none
 *
 * \return none
 */
static void rf_if_write_rf_settings(void)
{
    rf_part_num = rf_if_read_part_num();

    rf_if_write_register(XAH_CTRL_0, 0);

    /* Auto CRC on, IRQ status shows unmasked only, TRX_STATUS output on all accesses */
    rf_if_write_register(TRX_CTRL_1, TX_AUTO_CRC_ON | SPI_CMD_MODE_TRX_STATUS);

    rf_if_write_register(IRQ_MASK, CCA_ED_DONE | TRX_END | TRX_UR);
#ifdef TEST_GPIOS_ENABLED
    rf_if_set_bit(IRQ_MASK, RX_START, RX_START);
#endif
    xah_ctrl_1 = rf_if_read_register(XAH_CTRL_1);

    /*Read transceiver PART_NUM*/
    rf_part_num = rf_if_read_register(PART_NUM);

    /*Sub-GHz RF settings*/
    if (rf_part_num == PART_AT86RF212) {
        /*GC_TX_OFFS mode-dependent setting - OQPSK*/
        rf_if_write_register(RF_CTRL_0, 0x32);

        if (rf_if_read_register(VERSION_NUM) == VERSION_AT86RF212B) {
            /*TX Output Power setting - 0 dBm North American Band*/
            rf_if_write_register(PHY_TX_PWR, 0x03);
        } else {
            /*TX Output Power setting - 0 dBm North American Band*/
            rf_if_write_register(PHY_TX_PWR, 0x24);
        }

        /*PHY Mode: IEEE 802.15.4-2006/2011 - OQPSK-SIN-250*/
        rf_if_write_register(TRX_CTRL_2, RX_SAFE_MODE | RF_PHY_MODE);
        /*Based on receiver Characteristics. See AT86RF212B Datasheet where RSSI BASE VALUE in range -97 - -100 dBm*/
        rf_rssi_base_val = -98;
    }
    /*2.4GHz RF settings*/
    else {
#if 0
        /* Disable power saving functions for now - can only impact reliability,
         * and don't have any users demanding it. */
        /*Set RPC register*/
        rf_if_write_register(TRX_RPC, RX_RPC_CTRL | RX_RPC_EN | PLL_RPC_EN | XAH_TX_RPC_EN | IPAN_RPC_EN | TRX_RPC_RSVD_1);
#endif
        /*PHY Mode: IEEE 802.15.4 - Data Rate 250 kb/s*/
        rf_if_write_register(TRX_CTRL_2, RX_SAFE_MODE);
        rf_rssi_base_val = -91;
    }
}

/*
 * \brief Function returns the RF state
 *
 * \param none
 *
 * \return RF state
 */
static rf_trx_states_t rf_if_read_trx_state(void)
{
    return rf_if_trx_status_from_full(rf_if_read_register(TRX_STATUS));
}

/*
 * \brief Function reads packet buffer.
 *
 * \param data_out Output buffer
 * \param lqi_out LQI output
 * \param ed_out ED output
 * \param crc_good CRC good indication
 *
 * \return PSDU length [0..RF_MTU]
 */
static uint16_t rf_if_read_packet(uint8_t data_out[RF_MTU], uint8_t *lqi_out, uint8_t *ed_out, bool *crc_good)
{
    CS_SELECT();
    const uint8_t tx[1] = { 0x20 };
    uint8_t rx[3];
    rf_if_spi_exchange_n(tx, 1, rx, 2);
    uint8_t len = rx[1] & 0x7F;
    rf_if_spi_exchange_n(NULL, 0, data_out, len);
    rf_if_spi_exchange_n(NULL, 0, rx, 3);
    *lqi_out = rx[0];
    *ed_out = rx[1];
    *crc_good = rx[2] & 0x80;
    CS_RELEASE();

    return len;
}

/*
 * \brief Function writes RF short address registers
 *
 * \param short_address Given short address
 *
 * \return none
 */
static void rf_if_write_short_addr_registers(uint8_t *short_address)
{
    rf_if_write_register(SHORT_ADDR_1, *short_address++);
    rf_if_write_register(SHORT_ADDR_0, *short_address);
}

/*
 * \brief Function sets the frame pending in ACK message
 *
 * \param state Given frame pending state
 *
 * \return none
 */
static void rf_if_ack_pending_ctrl(uint8_t state)
{
    rf_if_lock();
    if (state) {
        rf_if_set_bit(CSMA_SEED_1, (1 << AACK_SET_PD), (1 << AACK_SET_PD));
    } else {
        rf_if_clear_bit(CSMA_SEED_1, (1 << AACK_SET_PD));
    }
    rf_if_unlock();
}

/*
 * \brief Function returns the state of frame pending control
 *
 * \param none
 *
 * \return Frame pending state
 */
static uint8_t rf_if_last_acked_pending(void)
{
    uint8_t last_acked_data_pending;

    rf_if_lock();
    if (rf_if_read_register(CSMA_SEED_1) & (1 << AACK_SET_PD)) {
        last_acked_data_pending = 1;
    } else {
        last_acked_data_pending = 0;
    }
    rf_if_unlock();

    return last_acked_data_pending;
}

/*
 * \brief Function calibrates the RF part.
 *
 * \param none
 *
 * \return none
 */
static void rf_if_calibration(void)
{
    rf_if_set_bit(FTN_CTRL, FTN_START, FTN_START);
    /*Wait while calibration is running*/
    while (rf_if_read_register(FTN_CTRL) & FTN_START);
}

/*
 * \brief Function writes RF PAN Id registers
 *
 * \param pan_id Given PAN Id
 *
 * \return none
 */
static void rf_if_write_pan_id_registers(uint8_t *pan_id)
{
    rf_if_write_register(PAN_ID_1, *pan_id++);
    rf_if_write_register(PAN_ID_0, *pan_id);
}

/*
 * \brief Function writes RF IEEE Address registers
 *
 * \param address Given IEEE Address
 *
 * \return none
 */
static void rf_if_write_ieee_addr_registers(uint8_t *address)
{
    uint8_t i;
    uint8_t temp = IEEE_ADDR_0;

    for (i = 0; i < 8; i++) {
        rf_if_write_register(temp++, address[7 - i]);
    }
}

/*
 * \brief Function writes data in RF frame buffer.
 *
 * \param ptr Pointer to data (PSDU, except FCS)
 * \param length Pointer to length (PSDU length, minus 2 for FCS)
 *
 * \return none
 */
static void rf_if_write_frame_buffer(const uint8_t *ptr, uint8_t length)
{
    const uint8_t cmd[2] = { 0x60, static_cast<uint8_t>(length + 2) };

    CS_SELECT();
    rf_if_spi_exchange_n(cmd, 2, NULL, 0);
    rf_if_spi_exchange_n(ptr, length, NULL, 0);
    CS_RELEASE();
}

/*
 * \brief Function returns 8-bit random value.
 *
 * \param none
 *
 * \return random value
 */
static uint8_t rf_if_read_rnd(void)
{
    uint8_t temp;
    uint8_t tmp_rpc_val = 0;
    /*RPC must be disabled while reading the random number*/
    if (rf_part_num == PART_AT86RF233) {
        tmp_rpc_val = rf_if_read_register(TRX_RPC);
        rf_if_write_register(TRX_RPC, RX_RPC_CTRL | TRX_RPC_RSVD_1);
    }


    wait_ns(1000);
    temp = ((rf_if_read_register(PHY_RSSI) >> 5) << 6);
    wait_ns(1000);
    temp |= ((rf_if_read_register(PHY_RSSI) >> 5) << 4);
    wait_ns(1000);
    temp |= ((rf_if_read_register(PHY_RSSI) >> 5) << 2);
    wait_ns(1000);
    temp |= ((rf_if_read_register(PHY_RSSI) >> 5));
    wait_ns(1000);
    if (rf_part_num == PART_AT86RF233) {
        rf_if_write_register(TRX_RPC, tmp_rpc_val);
    }
    return temp;
}

/*
 * \brief Function changes the state of the RF.
 *
 * \param trx_state Given RF state
 *
 * \return none
 */
static rf_trx_states_t rf_if_change_trx_state(rf_trx_states_t trx_state)
{
    rf_if_write_register(TRX_STATE, trx_state);
    /*Wait while not in desired state*/
    return rf_poll_trx_state_change(trx_state);
}

/*
 * \brief Function starts the CCA process
 *
 * \param none
 *
 * \return none
 */
static void rf_if_start_cca_process(void)
{
    rf_if_write_register(PHY_CC_CCA, CCA_REQUEST | CCA_MODE_3A | rf_phy_channel);
}

/*
 * \brief Function scales RSSI
 *
 * \param ed_level ED level read from chip
 *
 * \return appropriately scaled RSSI dBm
 */
static int8_t rf_if_scale_rssi(uint8_t ed_level)
{
    if (rf_part_num == PART_AT86RF212) {
        /* Data sheet says to multiply by 1.03 - this is 1.03125, rounding down */
        ed_level += ed_level >> 5;
    }
    return rf_rssi_base_val + ed_level;
}

/*
 * \brief Function sets the RF channel field
 *
 * \param Given channel
 *
 * \return none
 */
static void rf_if_set_channel_register(uint8_t channel)
{
    rf_if_set_bit(PHY_CC_CCA, channel, CCA_CHANNEL_MASK);
}

/*
 * \brief Function enables RF irq pin interrupts in RF interface.
 *
 * \param none
 *
 * \return none
 */
static void rf_if_enable_irq(void)
{
    rf->IRQ.enable_irq();
}

/*
 * \brief Function disables RF irq pin interrupts in RF interface.
 *
 * \param none
 *
 * \return none
 */
static void rf_if_disable_irq(void)
{
    rf->IRQ.disable_irq();
}

#ifdef MBED_CONF_RTOS_PRESENT
static void rf_if_interrupt_handler(void)
{
    rf->irq_thread.flags_set(SIG_RADIO);
}

// Started during construction of rf, so variable
// rf isn't set at the start. Uses 'this' instead.
void RFBits::rf_if_irq_task(void)
{
    for (;;) {
        uint32_t flags = ThisThread::flags_wait_any(SIG_ALL);
        rf_if_lock();
        if (flags & SIG_RADIO) {
            rf_if_irq_task_process_irq();
        }
        if (flags & SIG_TIMER_ACK) {
            rf_ack_wait_timer_interrupt();
        }
        if (flags & SIG_TIMER_CCA) {
            rf_cca_timer_interrupt();
        }
        if (flags & SIG_TIMER_CAL) {
            rf_calibration_timer_interrupt();
        }
        rf_if_unlock();
    }
}

static void rf_if_irq_task_process_irq(void)
#else
/*
 * \brief Function is a RF interrupt vector. End of frame in RX and TX are handled here as well as CCA process interrupt.
 *
 * \param none
 *
 * \return none
 */
static void rf_if_interrupt_handler(void)
#endif
{
    uint8_t irq_status, full_trx_status;

    /*Read and clear interrupt flag, and pick up trx_status*/
    irq_status = rf_if_read_register_with_status(IRQ_STATUS, &full_trx_status);

#ifdef TEST_GPIOS_ENABLED
    if (irq_status & RX_START) {
        TEST_RX_STARTED
    }
#endif
    /*Frame end interrupt (RX and TX)*/
    if (irq_status & TRX_END) {
        rf_trx_states_t trx_status = rf_if_trx_status_from_full(full_trx_status);
        if (rf_flags & RFF_TX) {
            rf_handle_tx_end(trx_status);
        } else if (rf_flags & RFF_RX) {
            rf_handle_rx_end(trx_status);
        } else {
            //something went really wrong
        }
    }
    if (irq_status & CCA_ED_DONE) {
        rf_handle_cca_ed_done(full_trx_status);
    }
    if (irq_status & TRX_UR) {
        // Here some counter could be used to monitor the under run occurrence count.
        // Do not print anything here!
    }
}

/*
 * \brief Function writes/read data in SPI interface
 */
static void rf_if_spi_exchange_n(const void *tx, size_t tx_len, void *rx, size_t rx_len)
{
#if 1
    rf->spi.write(static_cast<const char *>(tx), tx_len,
                  static_cast<char *>(rx), rx_len);
#else
    const uint8_t *txb = static_cast<const uint8_t *>(tx);
    uint8_t *rxb = static_cast<uint8_t *>(rx);
    while (tx_len > 0 || rx_len > 0) {
        uint8_t b;
        if (tx_len) {
            tx_len--;
            b = *txb++;
        } else {
            b = 0xFF;
        }
        b = rf->spi.write(b);
        if (rx_len) {
            rx_len--;
            *rxb++ = b;
        }
    }
#endif
}

/*
 * \brief Function sets given RF flag on.
 *
 * \param x Given RF flag
 *
 * \return none
 */
static void rf_flags_set(uint8_t x)
{
    rf_flags |= x;
}

/*
 * \brief Function clears given RF flag on.
 *
 * \param x Given RF flag
 *
 * \return none
 */
static void rf_flags_clear(uint8_t x)
{
    rf_flags &= ~x;
}

/*
 * \brief Function checks if given RF flag is on.
 *
 * \param x Given RF flag
 *
 * \return states of the given flags
 */
static uint8_t rf_flags_check(uint8_t x)
{
    return (rf_flags & x);
}

/*
 * \brief Function clears all RF flags.
 *
 * \param none
 *
 * \return none
 */
static void rf_flags_reset(void)
{
    rf_flags = 0;
}

/*
 * \brief Function initialises and registers the RF driver.
 *
 * \param none
 *
 * \return rf_radio_driver_id Driver ID given by NET library
 */
static int8_t rf_device_register(const uint8_t *mac_addr)
{
    rf_trx_part_e radio_type;

    rf_init();

    radio_type = rf_radio_type_read();
    if (radio_type != ATMEL_UNKNOW_DEV) {
        /*Set pointer to MAC address*/
        device_driver.PHY_MAC = (uint8_t *)mac_addr;
        device_driver.driver_description = (char *)"ATMEL_MAC";
        //Create setup Used Radio chips
        if (radio_type == ATMEL_AT86RF212) {
            device_driver.link_type = PHY_LINK_15_4_SUBGHZ_TYPE;
        } else {
            device_driver.link_type = PHY_LINK_15_4_2_4GHZ_TYPE;
        }
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
        /*NULLIFY rx and tx_done callbacks*/
        device_driver.phy_rx_cb = NULL;
        device_driver.phy_tx_done_cb = NULL;
        /*Register device driver*/
        rf_radio_driver_id = arm_net_phy_register(&device_driver);
    } else {
        rf_if_disable_irq();
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
static void rf_device_unregister()
{
    if (rf_radio_driver_id >= 0) {
        arm_net_phy_unregister(rf_radio_driver_id);
        rf_radio_driver_id = -1;
    }
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
    rf_if_lock();
    rf_give_up_on_ack();
    rf_if_unlock();
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
    /*Calibrate RF*/
    rf_calibration_cb();
    /*Start new calibration timeout*/
    rf_calibration_timer_start(RF_CALIBRATION_INTERVAL);
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
    rf_flags_set(RFF_CCA);
    /*Start CCA process*/
    rf_if_start_cca_process();
}

/*
 * \brief Function starts the ACK wait timeout.
 *
 * \param slots Given slots, resolution 50us
 *
 * \return none
 */
static void rf_ack_wait_timer_start(uint16_t slots)
{
    rf_if_ack_wait_timer_start(slots);
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
    rf_if_calibration_timer_start(slots);
}

/*
 * \brief Function starts the CCA backoff.
 *
 * \param slots Given slots, resolution 50us
 *
 * \return none
 */
static void rf_cca_timer_start(uint32_t slots)
{
    rf_if_cca_timer_start(slots);
}

/*
 * \brief Function stops the CCA backoff.
 *
 * \return none
 */
static void rf_cca_timer_stop(void)
{
    rf_if_cca_timer_stop();
}

/*
 * \brief Function writes various RF settings in startup.
 *
 * \param none
 *
 * \return none
 */
static void rf_write_settings(void)
{
    rf_if_lock();
    rf_if_write_rf_settings();
    /*Set output power*/
    rf_if_write_set_tx_power_register(radio_tx_power);
    /*Initialise Antenna Diversity*/
    if (rf_use_antenna_diversity) {
        rf_if_write_antenna_diversity_settings();
    }
    rf_if_unlock();
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
    rf_if_lock();
    /*Wake up RF if sleeping*/
    if (rf_flags_check(RFF_ON) == 0) {
        rf_if_disable_slptr();
        rf_poll_trx_state_change(TRX_OFF);
    }
    /*Write address filter registers*/
    rf_if_write_short_addr_registers(short_address);
    /*RF back to sleep*/
    if (rf_flags_check(RFF_ON) == 0) {
        rf_if_enable_slptr();
    }
    rf_if_unlock();
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
    rf_if_lock();
    /*Wake up RF if sleeping*/
    if (rf_flags_check(RFF_ON) == 0) {
        rf_if_disable_slptr();
        rf_poll_trx_state_change(TRX_OFF);
    }
    /*Write address filter registers*/
    rf_if_write_pan_id_registers(pan_id);
    /*RF back to sleep*/
    if (rf_flags_check(RFF_ON) == 0) {
        rf_if_enable_slptr();
    }
    rf_if_unlock();
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
    rf_if_lock();
    /*Wake up RF if sleeping*/
    if (rf_flags_check(RFF_ON) == 0) {
        rf_if_disable_slptr();
        rf_poll_trx_state_change(TRX_OFF);
    }
    /*Write address filter registers*/
    rf_if_write_ieee_addr_registers(address);
    /*RF back to sleep*/
    if (rf_flags_check(RFF_ON) == 0) {
        rf_if_enable_slptr();
    }
    rf_if_unlock();
}

/*
 * \brief Function sets the RF channel.
 *
 * \param ch New channel
 *
 * \return none
 */
static void rf_channel_set(uint8_t ch)
{
    rf_if_lock();
    rf_phy_channel = ch;
    if (ch < 0x1f) {
        rf_if_set_channel_register(ch);
    }
    rf_if_unlock();
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
    rf_if_lock();

    /*Write RF settings*/
    rf_write_settings();
    /*Initialise PHY mode*/
    rf_init_phy_mode();
    /*Clear RF flags*/
    rf_flags_reset();
    /*Set RF in TRX OFF state*/
    rf_if_change_trx_state(TRX_OFF);
    /*Set RF in PLL_ON state*/
    rf_trx_states_t trx_status = rf_if_change_trx_state(PLL_ON);
    /*Start receiver*/
    rf_receive(trx_status);
    /*Read randomness, and add to seed*/
    randLIB_add_seed(rf_if_read_rnd());
    /*Start RF calibration timer*/
    rf_calibration_timer_start(RF_CALIBRATION_INTERVAL);

    rf_if_unlock();
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
    if (rf_flags_check(RFF_ON)) {
        rf_if_lock();
        rf_cca_abort();
        uint16_t while_counter = 0;
        /*Wait while receiving*/
        while (rf_if_read_trx_state() == BUSY_RX_AACK) {
            while_counter++;
            if (while_counter == 0xffff) {
                break;
            }
        }
        /*RF state change: RX_AACK_ON->PLL_ON->TRX_OFF->SLEEP*/
        if (rf_if_read_trx_state() == RX_AACK_ON) {
            rf_if_change_trx_state(PLL_ON);
        }
        rf_if_change_trx_state(TRX_OFF);
        rf_if_enable_slptr();

        /*Disable Antenna Diversity*/
        if (rf_use_antenna_diversity) {
            rf_if_disable_ant_div();
        }
        rf_if_unlock();
    }

    /*Clears all flags*/
    rf_flags_reset();
}

/*
 * \brief Function polls the RF state until it has changed to desired state.
 *
 * \param trx_state RF state
 *
 * \return none
 */
static rf_trx_states_t rf_poll_trx_state_change(rf_trx_states_t trx_state)
{
    uint16_t while_counter = 0;

    if (trx_state == FORCE_PLL_ON) {
        trx_state = PLL_ON;
    } else if (trx_state == FORCE_TRX_OFF) {
        trx_state = TRX_OFF;
    }

    rf_trx_states_t state_out;
    while ((state_out = rf_if_read_trx_state()) != trx_state) {
        while_counter++;
        if (while_counter == 0x1ff) {
            break;
        }
    }

    return state_out;
}

/*
 * \brief Function polls the RF state until it is no longer transitioning.
 *
 * \param trx_state RF state
 *
 * \return none
 */
static rf_trx_states_t rf_poll_for_state(void)
{
    rf_trx_states_t state_out;
    while ((state_out = rf_if_read_trx_state()) == STATE_TRANSITION_IN_PROGRESS) {
    }

    return state_out;
}

/*
 * \brief Function starts the CCA process before starting data transmission and copies the data to RF TX FIFO.
 *
 * \param data_ptr Pointer to TX data (excluding FCS)
 * \param data_length Length of the TX data (excluding FCS)
 * \param tx_handle Handle to transmission
 * \return 0 Success
 * \return -1 Busy
 */
static int8_t rf_start_cca(uint8_t *data_ptr, uint16_t data_length, uint8_t tx_handle, data_protocol_e data_protocol)
{
    (void)data_protocol;
    rf_if_lock();
    /*Check if transmitter is busy*/
    rf_trx_states_t trx_state = rf_if_read_trx_state();
    if (trx_state == BUSY_RX || trx_state == BUSY_RX_AACK || data_length > RF_MTU - 2) {
        rf_if_unlock();
        /*Return busy*/
        return -1;
    } else {
        rf_give_up_on_ack();

        /*Nanostack has a static TX buffer, which will remain valid until we*/
        /*generate a callback, so we just note the pointer for reading later.*/
        rf_tx_data = data_ptr;
        rf_tx_length = data_length;
        /*Start CCA timeout*/
        rf_cca_timer_start(RF_CCA_BASE_BACKOFF + randLIB_get_random_in_range(0, RF_CCA_RANDOM_BACKOFF));
        TEST_CSMA_STARTED
        /*Store TX handle*/
        mac_tx_handle = tx_handle;
        rf_if_unlock();
    }

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
    TEST_CSMA_DONE
    rf_cca_timer_stop();
    rf_flags_clear(RFF_CCA);
}

/*
 * \brief Function starts the transmission of the frame.
 *
 * \param none
 *
 * \return none
 */
static bool rf_start_tx()
{
    /* Attempt change to PLL_ON */
    rf_if_write_register(TRX_STATE, PLL_ON);

    // It appears that if radio is busy, rather than ignoring the state change,
    // the state change happens when it stops being busy - eg
    // after address match fail or finishing reception. If this happens, we do
    // not want to transmit - our channel clear check is stale (either someone is
    // still transmitting, or it's a long time since we checked). So wait for the
    // PLL_ON change and then go to receive mode without trying to transmit.
    rf_trx_states_t state = rf_poll_for_state();
    int poll_count = 0;
    while (state != PLL_ON) {
        /* Change didn't work (yet) - must be busy - assume it will eventually change */
        state = rf_poll_for_state();
        poll_count++;
    }

    rf_flags_clear(RFF_RX);
    // Check whether we saw any delay in the PLL_ON transition.
    if (poll_count > 0) {
        // let's get back to the receiving state.
        rf_receive(state);
        return false;
    }

    rf_flags_set(RFF_TX);
    /*RF state change: SLP_TR pulse triggers PLL_ON->BUSY_TX*/
    rf_if_enable_slptr();
    TEST_TX_STARTED
    /*Chip permits us to write frame buffer while it is transmitting*/
    /*As long as first byte of data is in within 176us of TX start, we're good */
    rf_if_write_frame_buffer(rf_tx_data, rf_tx_length);
    rf_if_disable_slptr();
    return true;
}

/*
 * \brief Function sets the RF in RX state.
 *
 * \param none
 *
 * \return none
 */
static void rf_receive(rf_trx_states_t trx_status)
{
    TEST_RX_DONE
    uint16_t while_counter = 0;
    if (rf_flags_check(RFF_ON) == 0) {
        rf_on();
        rf_channel_set(rf_phy_channel);
        trx_status = TRX_OFF;
    }
    /*If not yet in RX state set it*/
    if (rf_flags_check(RFF_RX) == 0) {
        /*Wait while receiving data. Just making sure, usually this shouldn't happen. */
        while (trx_status == BUSY_RX || trx_status == BUSY_RX_AACK || trx_status == STATE_TRANSITION_IN_PROGRESS) {
            while_counter++;
            if (while_counter == 0xffff) {
                break;
            }
            trx_status = rf_if_read_trx_state();
        }

        if ((rf_mode == RF_MODE_SNIFFER) || (rf_mode == RF_MODE_ED)) {
            if (trx_status != RX_ON) {
                trx_status = rf_if_change_trx_state(RX_ON);
            }
        } else {
            /*ACK is always received in promiscuous mode to bypass address filters*/
            if (rf_rx_mode) {
                rf_rx_mode = 0;
                rf_if_enable_promiscuous_mode();
            } else {
                rf_if_disable_promiscuous_mode();
            }
            if (trx_status != RX_AACK_ON) {
                trx_status = rf_if_change_trx_state(RX_AACK_ON);
            }
        }
        /*If calibration timer was unable to calibrate the RF, run calibration now*/
        if (!rf_tuned) {
            /*Start calibration. This can be done in states TRX_OFF, PLL_ON or in any receive state*/
            rf_if_calibration();
            /*RF is tuned now*/
            rf_tuned = 1;
        }

        rf_flags_set(RFF_RX);
    }
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
    /*clear tuned flag to start tuning in rf_receive*/
    rf_tuned = 0;
    /*If RF is in default receive state, start calibration*/
    if (rf_if_read_trx_state() == RX_AACK_ON) {
        rf_if_lock();
        /*Set RF in PLL_ON state*/
        rf_if_change_trx_state(PLL_ON);
        /*Set RF in TRX_OFF state to start PLL tuning*/
        rf_if_change_trx_state(TRX_OFF);
        /*Set RF in RX_ON state to calibrate*/
        rf_trx_states_t trx_status = rf_if_change_trx_state(RX_ON);
        /*Calibrate FTN*/
        rf_if_calibration();
        /*RF is tuned now*/
        rf_tuned = 1;
        /*Back to default receive state*/
        rf_flags_clear(RFF_RX);
        rf_receive(trx_status);
        rf_if_unlock();
    }
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
    /*Set RFF_ON flag*/
    if (rf_flags_check(RFF_ON) == 0) {
        rf_if_lock();
        rf_flags_set(RFF_ON);
        /*Enable Antenna diversity*/
        if (rf_use_antenna_diversity)
            /*Set ANT_EXT_SW_EN to enable controlling of antenna diversity*/
        {
            rf_if_enable_ant_div();
        }

        /*Wake up from sleep state*/
        rf_if_disable_slptr();
        rf_poll_trx_state_change(TRX_OFF);
        rf_if_unlock();
    }
}

/*
 * \brief Abandon waiting for an ack frame

 * \return none
 */
static void rf_give_up_on_ack(void)
{
    if (expected_ack_sequence == -1) {
        return;
    }

    rf_if_disable_promiscuous_mode();
    rf_if_ack_wait_timer_stop();
    expected_ack_sequence = -1;

    if (device_driver.phy_tx_done_cb) {
        device_driver.phy_tx_done_cb(rf_radio_driver_id, mac_tx_handle, PHY_LINK_TX_FAIL, 0, 0);
    }
}

/*
 * \brief Function handles the received ACK frame.
 *
 * \param seq_number Sequence number of received ACK
 * \param data_pending Pending bit state in received ACK
 *
 * \return none
 */
static void rf_handle_ack(uint8_t seq_number, uint8_t data_pending)
{
    phy_link_tx_status_e phy_status;
    /*Received ACK sequence must be equal with transmitted packet sequence*/
    if (expected_ack_sequence == seq_number) {
        rf_if_disable_promiscuous_mode();
        rf_if_ack_wait_timer_stop();
        expected_ack_sequence = -1;

        /*When data pending bit in ACK frame is set, inform NET library*/
        if (data_pending) {
            phy_status = PHY_LINK_TX_DONE_PENDING;
        } else {
            phy_status = PHY_LINK_TX_DONE;
        }
        /*Call PHY TX Done API*/
        if (device_driver.phy_tx_done_cb) {
            device_driver.phy_tx_done_cb(rf_radio_driver_id, mac_tx_handle, phy_status, 0, 0);
        }
    } else {
        rf_give_up_on_ack();
    }
}

/*
 * \brief Function is a call back for RX end interrupt.
 *
 * \param none
 *
 * \return none
 */
static void rf_handle_rx_end(rf_trx_states_t trx_status)
{
    TEST_RX_DONE
    /*Frame received interrupt*/
    if (!rf_flags_check(RFF_RX)) {
        return;
    }

    static uint8_t rf_buffer[RF_MTU];
    uint8_t rf_lqi, rf_ed;
    int8_t rf_rssi;
    bool crc_good;

    /*Read received packet*/
    uint8_t len = rf_if_read_packet(rf_buffer, &rf_lqi, &rf_ed, &crc_good);

    if (len < 5 || !crc_good) {
        rf_give_up_on_ack();
        return;
    }

    /* Convert raw ED to dBm value (chip-dependent) */
    rf_rssi = rf_if_scale_rssi(rf_ed);

    /* Create a virtual LQI using received RSSI, forgetting actual HW LQI */
    /* (should be done through PHY_EXTENSION_CONVERT_SIGNAL_INFO) */
    rf_lqi = rf_scale_lqi(rf_rssi);

    /*Handle received ACK*/
    if ((rf_buffer[0] & 0x07) == 0x02 && rf_mode != RF_MODE_SNIFFER) {
        /*Check if data is pending*/
        bool pending = (rf_buffer[0] & 0x10);

        /*Send sequence number in ACK handler*/
        rf_handle_ack(rf_buffer[2], pending);
    } else {
        rf_give_up_on_ack();
        if (device_driver.phy_rx_cb) {
            device_driver.phy_rx_cb(rf_buffer, len - 2, rf_lqi, rf_rssi, rf_radio_driver_id);
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
static void rf_handle_tx_end(rf_trx_states_t trx_status)
{
    TEST_TX_DONE
    rf_rx_mode = 0;
    /*If ACK is needed for this transmission*/
    if ((rf_tx_data[0] & 0x20) && rf_flags_check(RFF_TX)) {
        expected_ack_sequence = rf_tx_data[2];
        rf_ack_wait_timer_start(rf_ack_wait_duration);
        rf_rx_mode = 1;
    }
    rf_flags_clear(RFF_TX);
    /*Start receiver*/
    rf_receive(trx_status);

    /*Call PHY TX Done API*/
    if (device_driver.phy_tx_done_cb) {
        device_driver.phy_tx_done_cb(rf_radio_driver_id, mac_tx_handle, PHY_LINK_TX_SUCCESS, 0, 0);
    }
}

/*
 * \brief Function is a call back for CCA ED done interrupt.
 *
 * \param none
 *
 * \return none
 */
static void rf_handle_cca_ed_done(uint8_t full_trx_status)
{
    TEST_CSMA_DONE
    if (!rf_flags_check(RFF_CCA)) {
        return;
    }
    rf_flags_clear(RFF_CCA);

    bool success = false;

    /*Check the result of CCA process*/
    if ((full_trx_status & CCA_STATUS) && rf_if_trx_status_from_full(full_trx_status) == RX_AACK_ON) {
        success = rf_start_tx();
    }

    if (!success) {
        /*Send CCA fail notification*/
        if (device_driver.phy_tx_done_cb) {
            device_driver.phy_tx_done_cb(rf_radio_driver_id, mac_tx_handle, PHY_LINK_CCA_FAIL, 0, 0);
        }
    }
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
            rf_if_lock();
            rf_mode = RF_MODE_NORMAL;
            rf_channel_set(rf_channel);
            rf_receive();
            rf_if_enable_irq();
            rf_if_unlock();
            break;
        /*Enable wireless interface ED scan mode*/
        case PHY_INTERFACE_RX_ENERGY_STATE:
            rf_mode = RF_MODE_ED;
            rf_channel_set(rf_channel);
            rf_receive();
            rf_if_disable_irq();
            // Read status to clear pending flags.
            rf_if_read_register(IRQ_STATUS);
            // ED can be initiated by writing arbitrary value to PHY_ED_LEVEL
            rf_if_write_register(PHY_ED_LEVEL, 0xff);
            break;
        case PHY_INTERFACE_SNIFFER_STATE:             /**< Enable Sniffer state */
            rf_mode = RF_MODE_SNIFFER;
            rf_channel_set(rf_channel);
            rf_flags_clear(RFF_RX);
            rf_receive();
            rf_if_enable_irq();
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
        case PHY_EXTENSION_CTRL_PENDING_BIT:
            if (*data_ptr) {
                rf_if_ack_pending_ctrl(1);
            } else {
                rf_if_ack_pending_ctrl(0);
            }
            break;
        /*Return frame pending status*/
        case PHY_EXTENSION_READ_LAST_ACK_PENDING_STATUS:
            *data_ptr = rf_if_last_acked_pending();
            break;
        /*Set channel*/
        case PHY_EXTENSION_SET_CHANNEL:
            break;
        /*Read energy on the channel*/
        case PHY_EXTENSION_READ_CHANNEL_ENERGY:
            // End of the ED measurement is indicated by CCA_ED_DONE
            while (!(rf_if_read_register(IRQ_STATUS) & CCA_ED_DONE));
            // RF input power: RSSI base level + 1[db] * PHY_ED_LEVEL
            *data_ptr = rf_sensitivity + rf_if_read_register(PHY_ED_LEVEL);
            // Read status to clear pending flags.
            rf_if_read_register(IRQ_STATUS);
            // Next ED measurement is started, next PHY_EXTENSION_READ_CHANNEL_ENERGY call will return the result.
            rf_if_write_register(PHY_ED_LEVEL, 0xff);
            break;
        /*Read status of the link*/
        case PHY_EXTENSION_READ_LINK_STATUS:
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
    uint8_t tmp = 0;
    uint8_t part = rf_if_read_part_num();
    /*Read used PHY Mode*/
    tmp = rf_if_read_register(TRX_CTRL_2);
    /*Set ACK wait time for used data rate*/
    if (part == PART_AT86RF212) {
        if ((tmp & 0x1f) == 0x00) {
            rf_sensitivity = -110;
            rf_ack_wait_duration = 938;
            tmp = BPSK_20;
        } else if ((tmp & 0x1f) == 0x04) {
            rf_sensitivity = -108;
            rf_ack_wait_duration = 469;
            tmp = BPSK_40;
        } else if ((tmp & 0x1f) == 0x14) {
            rf_sensitivity = -108;
            rf_ack_wait_duration = 469;
            tmp = BPSK_40_ALT;
        } else if ((tmp & 0x1f) == 0x08) {
            rf_sensitivity = -101;
            rf_ack_wait_duration = 50;
            tmp = OQPSK_SIN_RC_100;
        } else if ((tmp & 0x1f) == 0x09) {
            rf_sensitivity = -99;
            rf_ack_wait_duration = 30;
            tmp = OQPSK_SIN_RC_200;
        } else if ((tmp & 0x1f) == 0x18) {
            rf_sensitivity = -102;
            rf_ack_wait_duration = 50;
            tmp = OQPSK_RC_100;
        } else if ((tmp & 0x1f) == 0x19) {
            rf_sensitivity = -100;
            rf_ack_wait_duration = 30;
            tmp = OQPSK_RC_200;
        } else if ((tmp & 0x1f) == 0x0c) {
            rf_sensitivity = -100;
            rf_ack_wait_duration = 20;
            tmp = OQPSK_SIN_250;
        } else if ((tmp & 0x1f) == 0x0d) {
            rf_sensitivity = -98;
            rf_ack_wait_duration = 25;
            tmp = OQPSK_SIN_500;
        } else if ((tmp & 0x1f) == 0x0f) {
            rf_sensitivity = -98;
            rf_ack_wait_duration = 25;
            tmp = OQPSK_SIN_500_ALT;
        } else if ((tmp & 0x1f) == 0x1c) {
            rf_sensitivity = -101;
            rf_ack_wait_duration = 20;
            tmp = OQPSK_RC_250;
        } else if ((tmp & 0x1f) == 0x1d) {
            rf_sensitivity = -99;
            rf_ack_wait_duration = 25;
            tmp = OQPSK_RC_500;
        } else if ((tmp & 0x1f) == 0x1f) {
            rf_sensitivity = -99;
            rf_ack_wait_duration = 25;
            tmp = OQPSK_RC_500_ALT;
        } else if ((tmp & 0x3f) == 0x2A) {
            rf_sensitivity = -91;
            rf_ack_wait_duration = 25;
            tmp = OQPSK_SIN_RC_400_SCR_ON;
        } else if ((tmp & 0x3f) == 0x0A) {
            rf_sensitivity = -91;
            rf_ack_wait_duration = 25;
            tmp = OQPSK_SIN_RC_400_SCR_OFF;
        } else if ((tmp & 0x3f) == 0x3A) {
            rf_sensitivity = -97;
            rf_ack_wait_duration = 25;
            tmp = OQPSK_RC_400_SCR_ON;
        } else if ((tmp & 0x3f) == 0x1A) {
            rf_sensitivity = -97;
            rf_ack_wait_duration = 25;
            tmp = OQPSK_RC_400_SCR_OFF;
        } else if ((tmp & 0x3f) == 0x2E) {
            rf_sensitivity = -93;
            rf_ack_wait_duration = 13;
            tmp = OQPSK_SIN_1000_SCR_ON;
        } else if ((tmp & 0x3f) == 0x0E) {
            rf_sensitivity = -93;
            rf_ack_wait_duration = 13;
            tmp = OQPSK_SIN_1000_SCR_OFF;
        } else if ((tmp & 0x3f) == 0x3E) {
            rf_sensitivity = -95;
            rf_ack_wait_duration = 13;
            tmp = OQPSK_RC_1000_SCR_ON;
        } else if ((tmp & 0x3f) == 0x1E) {
            rf_sensitivity = -95;
            rf_ack_wait_duration = 13;
            tmp = OQPSK_RC_1000_SCR_OFF;
        }
    } else {
        rf_sensitivity = -101;
        rf_ack_wait_duration = 20;
    }
    /*Board design might reduces the sensitivity*/
    //rf_sensitivity += RF_SENSITIVITY_CALIBRATION;
}


static uint8_t rf_scale_lqi(int8_t rssi)
{
    uint8_t scaled_lqi;

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

NanostackRfPhyAtmel::NanostackRfPhyAtmel(PinName spi_mosi, PinName spi_miso,
                                         PinName spi_sclk, PinName spi_cs,  PinName spi_rst, PinName spi_slp, PinName spi_irq,
                                         PinName i2c_sda, PinName i2c_scl)
    : _mac(i2c_sda, i2c_scl), _mac_addr(), _rf(NULL), _test_pins(NULL), _mac_set(false),
      _spi_mosi(spi_mosi), _spi_miso(spi_miso), _spi_sclk(spi_sclk),
      _spi_cs(spi_cs), _spi_rst(spi_rst), _spi_slp(spi_slp), _spi_irq(spi_irq)
{
    _rf = new RFBits(_spi_mosi, _spi_miso, _spi_sclk, _spi_cs, _spi_rst, _spi_slp, _spi_irq);
#ifdef TEST_GPIOS_ENABLED
    _test_pins = new TestPins(TEST_PIN_TX, TEST_PIN_RX, TEST_PIN_CSMA, TEST_PIN_SPARE_1, TEST_PIN_SPARE_2);
#endif
}

NanostackRfPhyAtmel::~NanostackRfPhyAtmel()
{
    delete _rf;
}

int8_t NanostackRfPhyAtmel::rf_register()
{
    if (NULL == _rf) {
        return -1;
    }

    rf_if_lock();

    if (rf != NULL) {
        rf_if_unlock();
        error("Multiple registrations of NanostackRfPhyAtmel not supported");
        return -1;
    }

    // Read the mac address if it hasn't been set by a user
    rf = _rf;
    test_pins = _test_pins;
    if (!_mac_set) {
        int ret = _mac.read_eui64((void *)_mac_addr);
        if (ret < 0) {
            rf = NULL;
            rf_if_unlock();
            return -1;
        }
    }
    /*Reset RF module*/
    rf_if_reset_radio();
    rf_part_num = rf_if_read_part_num();
    int8_t radio_id = -1;
    if (rf_part_num != PART_AT86RF231 && rf_part_num != PART_AT86RF233 && rf_part_num != PART_AT86RF212) {
        // Register RF type 215. Jumps to AT86RF215 driver.
        radio_id = rf->init_215_driver(_rf, _test_pins, _mac_addr, &rf_part_num);
    } else {
        // Register other RF types.
        radio_id = rf_device_register(_mac_addr);
    }
    tr_info("RF part number: %x", rf_part_num);
    if (radio_id < 0) {
        tr_err("RF registration failed");
        rf = NULL;
    }

    rf_if_unlock();
    return radio_id;
}

void NanostackRfPhyAtmel::rf_unregister()
{
    rf_if_lock();

    if (NULL == rf) {
        rf_if_unlock();
        return;
    }

    rf_device_unregister();
    rf = NULL;

    rf_if_unlock();
}

void NanostackRfPhyAtmel::get_mac_address(uint8_t *mac)
{
    rf_if_lock();

    if (NULL == rf) {
        error("NanostackRfPhyAtmel Must be registered to read mac address");
        rf_if_unlock();
        return;
    }
    memcpy((void *)mac, (void *)_mac_addr, sizeof(_mac_addr));

    rf_if_unlock();
}

void NanostackRfPhyAtmel::set_mac_address(uint8_t *mac)
{
    rf_if_lock();

    if (NULL != rf) {
        error("NanostackRfPhyAtmel cannot change mac address when running");
        rf_if_unlock();
        return;
    }
    memcpy((void *)_mac_addr, (void *)mac, sizeof(_mac_addr));
    _mac_set = true;

    rf_if_unlock();
}

#if MBED_CONF_ATMEL_RF_PROVIDE_DEFAULT

NanostackRfPhy &NanostackRfPhy::get_default_instance()
{
    static NanostackRfPhyAtmel rf_phy(ATMEL_SPI_MOSI, ATMEL_SPI_MISO, ATMEL_SPI_SCLK, ATMEL_SPI_CS,
                                      ATMEL_SPI_RST, ATMEL_SPI_SLP, ATMEL_SPI_IRQ, ATMEL_I2C_SDA, ATMEL_I2C_SCL);
    return rf_phy;
}

#endif // MBED_CONF_ATMEL_RF_PROVIDE_DEFAULT

#endif // MBED_CONF_NANOSTACK_CONFIGURATION
