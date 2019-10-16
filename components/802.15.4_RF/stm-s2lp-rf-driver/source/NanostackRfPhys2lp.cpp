/*
 * Copyright (c) 2018 ARM Limited. All rights reserved.
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
#include "platform/arm_hal_interrupt.h"
#include "nanostack/platform/arm_hal_phy.h"
#include "ns_types.h"
#include "NanostackRfPhys2lp.h"
#include "s2lpReg.h"
#include "rf_configuration.h"
#include "randLIB.h"
#include "mbed_trace.h"
#include "mbed_toolchain.h"
#include "common_functions.h"
#include <Timer.h>
#include "Timeout.h"
#include "Thread.h"
#include "mbed_wait_api.h"
#include "platform/mbed_error.h"

using namespace mbed;
using namespace rtos;

#define TRACE_GROUP "s2lp"

#define INTERRUPT_GPIO  S2LP_GPIO3

#if INTERRUPT_GPIO == S2LP_GPIO0
#define INT_IN_GPIO     rf->RF_S2LP_GPIO0
#elif INTERRUPT_GPIO == S2LP_GPIO1
#define INT_IN_GPIO     rf->RF_S2LP_GPIO1
#elif INTERRUPT_GPIO == S2LP_GPIO2
#define INT_IN_GPIO     rf->RF_S2LP_GPIO2
#else
#define INT_IN_GPIO     rf->RF_S2LP_GPIO3
#endif

#ifdef TEST_GPIOS_ENABLED
#define TEST_TX_STARTED     rf->TEST1 = 1;
#define TEST_TX_DONE        rf->TEST1 = 0;
#define TEST_RX_STARTED     rf->TEST2 = 1;
#define TEST_RX_DONE        rf->TEST2 = 0;
#define TEST_ACK_TX_STARTED rf->TEST3 = 1;
#define TEST_ACK_TX_DONE    rf->TEST3 = 0;
#define TEST1_ON            rf->TEST4 = 1;
#define TEST1_OFF           rf->TEST4 = 0;
#define TEST2_ON            rf->TEST5 = 1;
#define TEST2_OFF           rf->TEST5 = 0;
extern void (*fhss_uc_switch)(void);
extern void (*fhss_bc_switch)(void);
#else //TEST_GPIOS_ENABLED
#define TEST_TX_STARTED
#define TEST_TX_DONE
#define TEST_RX_STARTED
#define TEST_RX_DONE
#define TEST_ACK_TX_STARTED
#define TEST_ACK_TX_DONE
#define TEST1_ON
#define TEST1_OFF
#define TEST2_ON
#define TEST2_OFF
#endif //TEST_GPIOS_ENABLED

#define MAC_FRAME_TYPE_MASK     0x07
#define MAC_FRAME_BEACON        (0)
#define MAC_TYPE_DATA           (1)
#define MAC_TYPE_ACK            (2)
#define MAC_TYPE_COMMAND        (3)
#define MAC_DATA_PENDING        0x10
#define FC_DST_MODE             0x0C
#define FC_SRC_MODE             0xC0
#define FC_DST_ADDR_NONE        0x00
#define FC_DST_16_BITS          0x08
#define FC_DST_64_BITS          0x0C
#define FC_SRC_64_BITS          0xC0
#define FC_SEQUENCE_COMPRESSION 0x01
#define FC_AR                   0x20
#define FC_PAN_ID_COMPRESSION   0x40
#define VERSION_FIELD_MASK      0x30
#define SHIFT_SEQ_COMP_FIELD    (0)
#define SHIFT_VERSION_FIELD     (4)
#define SHIFT_PANID_COMP_FIELD  (6)
#define OFFSET_DST_PAN_ID       (3)
#define OFFSET_DST_ADDR         (5)

#define CS_SELECT()  {rf->CS = 0;}
#define CS_RELEASE() {rf->CS = 1;}

typedef enum {
    RF_MODE_NORMAL = 0,
    RF_MODE_SNIFFER = 1
} rf_mode_e;

class UnlockedSPI : public SPI {
public:
    UnlockedSPI(PinName sdi, PinName sdo, PinName sclk) :
        SPI(sdi, sdo, sclk) { }
    virtual void lock() { }
    virtual void unlock() { }
};

class RFPins {
public:
    RFPins(PinName spi_sdi, PinName spi_sdo,
           PinName spi_sclk, PinName spi_cs, PinName spi_sdn,
#ifdef TEST_GPIOS_ENABLED
           PinName spi_test1, PinName spi_test2, PinName spi_test3, PinName spi_test4, PinName spi_test5,
#endif //TEST_GPIOS_ENABLED
           PinName spi_gpio0, PinName spi_gpio1, PinName spi_gpio2,
           PinName spi_gpio3);
    UnlockedSPI spi;
    DigitalOut CS;
    DigitalOut SDN;
#ifdef TEST_GPIOS_ENABLED
    DigitalOut TEST1;
    DigitalOut TEST2;
    DigitalOut TEST3;
    DigitalOut TEST4;
    DigitalOut TEST5;
#endif //TEST_GPIOS_ENABLED
    InterruptIn RF_S2LP_GPIO0;
    InterruptIn RF_S2LP_GPIO1;
    InterruptIn RF_S2LP_GPIO2;
    InterruptIn RF_S2LP_GPIO3;
    Timeout cca_timer;
    Timeout backup_timer;
    Timer tx_timer;
    Thread irq_thread;
    Mutex mutex;
    void rf_irq_task();
};

RFPins::RFPins(PinName spi_sdi, PinName spi_sdo,
               PinName spi_sclk, PinName spi_cs, PinName spi_sdn,
#ifdef TEST_GPIOS_ENABLED
               PinName spi_test1, PinName spi_test2, PinName spi_test3, PinName spi_test4, PinName spi_test5,
#endif //TEST_GPIOS_ENABLED
               PinName spi_gpio0, PinName spi_gpio1, PinName spi_gpio2,
               PinName spi_gpio3)
    :   spi(spi_sdi, spi_sdo, spi_sclk),
        CS(spi_cs),
        SDN(spi_sdn),
#ifdef TEST_GPIOS_ENABLED
        TEST1(spi_test1),
        TEST2(spi_test2),
        TEST3(spi_test3),
        TEST4(spi_test4),
        TEST5(spi_test5),
#endif //TEST_GPIOS_ENABLED
        RF_S2LP_GPIO0(spi_gpio0),
        RF_S2LP_GPIO1(spi_gpio1),
        RF_S2LP_GPIO2(spi_gpio2),
        RF_S2LP_GPIO3(spi_gpio3),
        irq_thread(osPriorityRealtime, 1024)
{
    irq_thread.start(mbed::callback(this, &RFPins::rf_irq_task));
}

static uint8_t rf_read_register(uint8_t addr);
static s2lp_states_e rf_read_state(void);
static void rf_write_register(uint8_t addr, uint8_t data);
static void rf_interrupt_handler(void);
static void rf_receive(uint8_t rx_channel);
static void rf_cca_timer_stop(void);
static void rf_backup_timer_start(uint32_t slots);
static void rf_backup_timer_stop(void);
static void rf_rx_ready_handler(void);
static uint32_t read_irq_status(void);
static bool rf_rx_filter(uint8_t *mac_header, uint8_t *mac_64bit_addr, uint8_t *mac_16bit_addr, uint8_t *pan_id);
static void rf_cca_timer_start(uint32_t slots);

static RFPins *rf;
static phy_device_driver_s device_driver;
static int8_t rf_radio_driver_id = -1;
static uint8_t *tx_data_ptr;
static uint16_t tx_data_length;
static uint16_t rx_data_length;
static uint32_t enabled_interrupts;
static uint8_t mac_tx_handle;
static uint8_t rf_rx_channel;
static uint8_t rf_new_channel;
static uint8_t rx_buffer[RF_MTU];
static rf_states_e rf_state = RF_IDLE;
// This will be used when given channel spacing cannot be supported by transceiver
static uint8_t rf_channel_multiplier = 1;
static uint16_t tx_sequence = 0xffff;
static uint32_t tx_time = 0;
static uint32_t rx_time = 0;
static uint32_t tx_finnish_time = 0;
static uint32_t rf_symbol_rate;
static bool cca_enabled = true;
static uint8_t s2lp_PAN_ID[2] = {0xff, 0xff};
static uint8_t s2lp_short_address[2];
static uint8_t s2lp_MAC[8];
static rf_mode_e rf_mode = RF_MODE_NORMAL;
static bool rf_update_config = false;
static uint16_t cur_packet_len = 0xffff;
static uint32_t receiver_ready_timestamp;

static int16_t rssi_threshold = RSSI_THRESHOLD;

/* Channel configurations for sub-GHz */
static phy_rf_channel_configuration_s phy_subghz = {
    .channel_0_center_frequency = 868300000U,
    .channel_spacing = 500000U,
    .datarate = 250000U,
    .number_of_channels = 11U,
    .modulation = M_2FSK,
    .modulation_index = MODULATION_INDEX_UNDEFINED
};


static const phy_device_channel_page_s phy_channel_pages[] = {
    { CHANNEL_PAGE_2, &phy_subghz},
    { CHANNEL_PAGE_0, NULL}
};


#include "rtos.h"

static void rf_irq_task_process_irq();

#define SIG_ALL             (SIG_RADIO|SIG_TIMER_CCA|SIG_TIMER_BACKUP)
#define SIG_RADIO           1
#define SIG_TIMER_CCA       2
#define SIG_TIMER_BACKUP    4


#define ACK_FRAME_LENGTH    3
// Give some additional time for processing, PHY headers, CRC etc.
#define PACKET_SENDING_EXTRA_TIME   5000
#define MAX_PACKET_SENDING_TIME (uint32_t)(8000000/phy_subghz.datarate)*FIFO_SIZE + PACKET_SENDING_EXTRA_TIME
#define ACK_SENDING_TIME (uint32_t)(8000000/phy_subghz.datarate)*ACK_FRAME_LENGTH + PACKET_SENDING_EXTRA_TIME

#ifdef TEST_GPIOS_ENABLED
void test1_toggle(void)
{
    if (rf->TEST4) {
        rf->TEST4 = 0;
    } else {
        rf->TEST4 = 1;
    }
}
void test2_toggle(void)
{
    if (rf->TEST5) {
        rf->TEST5 = 0;
    } else {
        rf->TEST5 = 1;
    }
}
#endif //TEST_GPIOS_ENABLED

static void rf_calculate_symbol_rate(uint32_t baudrate, phy_modulation_e modulation)
{
    (void) modulation;
    uint8_t bits_in_symbols = 1;
    rf_symbol_rate = baudrate / bits_in_symbols;
}

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

static void rf_cca_timer_signal(void)
{
    rf->irq_thread.flags_set(SIG_TIMER_CCA);
}

static void rf_backup_timer_signal(void)
{
    rf->irq_thread.flags_set(SIG_TIMER_BACKUP);
}

/*
 * \brief Function writes/read data in SPI interface
 */
static void rf_spi_exchange(const void *tx, size_t tx_len, void *rx, size_t rx_len)
{
    rf->spi.write(static_cast<const char *>(tx), tx_len, static_cast<char *>(rx), rx_len);
}

static uint8_t rf_read_register(uint8_t addr)
{
    const uint8_t tx[2] = {SPI_RD_REG, addr};
    uint8_t rx[3];
    rf_lock();
    CS_SELECT();
    rf_spi_exchange(tx, sizeof(tx), rx, sizeof(rx));
    CS_RELEASE();
    rf_unlock();
    return rx[2];
}

static void rf_write_register(uint8_t addr, uint8_t data)
{
    const uint8_t tx[3] = {SPI_WR_REG, addr, data};
    rf_lock();
    CS_SELECT();
    rf_spi_exchange(tx, sizeof(tx), NULL, 0);
    CS_RELEASE();
    rf_unlock();
}

static void rf_write_register_field(uint8_t addr, uint8_t field, uint8_t value)
{
    uint8_t reg_tmp = rf_read_register(addr);
    reg_tmp &= ~field;
    reg_tmp |= value;
    rf_write_register(addr, reg_tmp);
}

static s2lp_states_e rf_read_state(void)
{
    return (s2lp_states_e)(rf_read_register(MC_STATE0) >> 1);
}

static void rf_poll_state_change(s2lp_states_e state)
{
    uint16_t break_counter = 0;
    while (state != rf_read_state()) {
        if (break_counter++ == 0xffff) {
            tr_err("Failed to change state from %x to: %x", rf_read_state(), state);
            break;
        }
    }
}

static void rf_enable_gpio_signal(uint8_t gpio_pin, uint8_t interrupt_signal, uint8_t gpio_mode)
{
    rf_write_register(GPIO0_CONF + gpio_pin, (interrupt_signal << 3) | gpio_mode);
}

static void rf_enable_interrupt(uint8_t event)
{
    if (enabled_interrupts & (1 << event)) {
        return;
    }
    rf_write_register_field(IRQ_MASK0 - (event / 8), 1 << (event % 8), 1 << (event % 8));
    enabled_interrupts |= (1 << event);
}

static void rf_disable_interrupt(uint8_t event)
{
    if (!(enabled_interrupts & (1 << event))) {
        return;
    }
    rf_write_register_field(IRQ_MASK0 - (event / 8), 1 << (event % 8), 0 << (event % 8));
    enabled_interrupts &= ~(1 << event);
}

static void rf_disable_all_interrupts(void)
{
    if ((uint8_t)enabled_interrupts & 0xff) {
        rf_write_register(IRQ_MASK0, 0);
    }
    if ((uint8_t)(enabled_interrupts >> 8) & 0xff) {
        rf_write_register(IRQ_MASK1, 0);
    }
    if ((uint8_t)(enabled_interrupts >> 16) & 0xff) {
        rf_write_register(IRQ_MASK2, 0);
    }
    if ((uint8_t)(enabled_interrupts >> 24) & 0xff) {
        rf_write_register(IRQ_MASK3, 0);
    }
    enabled_interrupts = 0;
    read_irq_status();
}

static void rf_enable_gpio_interrupt(void)
{
    rf_enable_gpio_signal(INTERRUPT_GPIO, NIRQ, DIG_OUT_HIGH);
    INT_IN_GPIO.mode(PullUp);
    INT_IN_GPIO.fall(&rf_interrupt_handler);
    INT_IN_GPIO.enable_irq();
}

static void rf_send_command(s2lp_commands_e command)
{
    const uint8_t tx[2] = {SPI_CMD, command};
    rf_lock();
    CS_SELECT();
    rf_spi_exchange(tx, sizeof(tx), NULL, 0);
    CS_RELEASE();
    rf_unlock();
}

static void rf_state_change(s2lp_states_e state, bool lock_mode_tx)
{
    s2lp_commands_e command;

    if (S2LP_STATE_READY == state) {
        s2lp_states_e cur_state = rf_read_state();
        if (S2LP_STATE_TX == cur_state || S2LP_STATE_RX == cur_state) {
            command = S2LP_CMD_SABORT;
        } else {
            command = S2LP_CMD_READY;
        }
    } else if (S2LP_STATE_LOCK == state && lock_mode_tx) {
        command = S2LP_CMD_LOCKTX;
    } else if (S2LP_STATE_LOCK == state && !lock_mode_tx) {
        command = S2LP_CMD_LOCKRX;
    } else if (S2LP_STATE_RX == state) {
        command = S2LP_CMD_RX;
    } else if (S2LP_STATE_TX == state) {
        command = S2LP_CMD_TX;
    } else {
        tr_err("Invalid state %x", state);
        return;
    }
    rf_send_command(command);
    rf_poll_state_change(state);
}

static uint8_t rf_write_tx_fifo(uint8_t *ptr, uint16_t length)
{
    uint8_t free_bytes_in_fifo = FIFO_SIZE - rf_read_register(TX_FIFO_STATUS);
    const uint8_t spi_header[SPI_HEADER_LENGTH] = {SPI_WR_REG, TX_FIFO};
    uint8_t written_length = length;
    if (length > free_bytes_in_fifo) {
        written_length = free_bytes_in_fifo;
    }
    CS_SELECT();
    rf_spi_exchange(spi_header, SPI_HEADER_LENGTH, NULL, 0);
    rf_spi_exchange(ptr, written_length, NULL, 0);
    CS_RELEASE();
    return written_length;
}

static int rf_read_rx_fifo(uint16_t rx_index, uint16_t length)
{
    if ((rx_index + length) > RF_MTU) {
        return -1;
    }
    uint8_t *ptr = &rx_buffer[rx_index];
    const uint8_t spi_header[SPI_HEADER_LENGTH] = {SPI_RD_REG, RX_FIFO};
    CS_SELECT();
    rf_spi_exchange(spi_header, SPI_HEADER_LENGTH, NULL, 0);
    rf_spi_exchange(NULL, 0, ptr, length);
    CS_RELEASE();
    return length;
}

static void rf_flush_tx_fifo(void)
{
    if (rf_read_register(TX_FIFO_STATUS)) {
        rf_send_command(S2LP_CMD_FLUSHTXFIFO);
    }
}

static void rf_flush_rx_fifo(void)
{
    if (rf_read_register(RX_FIFO_STATUS)) {
        rf_send_command(S2LP_CMD_FLUSHRXFIFO);
    }
}

static void rf_write_packet_length(uint16_t packet_length)
{
    if ((uint8_t)(cur_packet_len >> 8) != (packet_length / 256)) {
        rf_write_register(PCKTLEN1, packet_length / 256);
    }
    if ((uint8_t)cur_packet_len != (packet_length % 256)) {
        rf_write_register(PCKTLEN0, packet_length % 256);
    }
    cur_packet_len = packet_length;
}

static uint32_t read_irq_status(void)
{
    const uint8_t tx[2] = {SPI_RD_REG, IRQ_STATUS3};
    uint8_t rx[6];
    CS_SELECT();
    rf_spi_exchange(tx, sizeof(tx), rx, sizeof(rx));
    CS_RELEASE();
    return (((uint32_t)rx[2] << 24) | ((uint32_t)rx[3] << 16) | ((uint32_t)rx[4] << 8) | rx[5]);
}

static void rf_set_channel_configuration_registers(void)
{
    // Set RSSI threshold
    uint8_t rssi_th;
    rf_conf_calculate_rssi_threshold_registers(rssi_threshold, &rssi_th);
    rf_write_register(RSSI_TH, rssi_th);
    // Set deviation
    uint32_t deviation = rf_conf_calculate_deviation(phy_subghz.modulation_index, phy_subghz.datarate);
    if (!deviation) {
        deviation = DEFAULT_DEVIATION;
    }
    uint8_t fdev_m, fdev_e;
    rf_conf_calculate_deviation_registers(deviation, &fdev_m, &fdev_e);
    rf_write_register(MOD0, fdev_m);
    rf_write_register_field(MOD1, FDEV_E_FIELD, fdev_e);

    // Set datarate
    uint16_t datarate_m;
    uint8_t datarate_e;
    rf_conf_calculate_datarate_registers(phy_subghz.datarate, &datarate_m, &datarate_e);
    rf_write_register_field(MOD2, DATARATE_E_FIELD, datarate_e);
    rf_write_register(MOD3, (uint8_t)datarate_m);
    rf_write_register(MOD4, datarate_m >> 8);
    // Set RX filter bandwidth, using channel spacing as RX filter bandwidth
    uint8_t chflt_m, chflt_e;
    rf_conf_calculate_rx_filter_bandwidth_registers(phy_subghz.channel_spacing, &chflt_m, &chflt_e);
    rf_write_register_field(CHFLT, CHFLT_M_FIELD, chflt_m << 4);
    rf_write_register_field(CHFLT, CHFLT_E_FIELD, chflt_e);
    // Set base frequency (Channel 0 center frequency)
    uint8_t synt3, synt2, synt1, synt0;
    rf_conf_calculate_base_frequency_registers(phy_subghz.channel_0_center_frequency, &synt3, &synt2, &synt1, &synt0);
    rf_write_register_field(SYNT3, SYNT_FIELD, synt3);
    rf_write_register(SYNT2, synt2);
    rf_write_register(SYNT1, synt1);
    rf_write_register(SYNT0, synt0);
    // Set channel spacing
    uint8_t ch_space;
    uint8_t ch_space_divider = 1;
    while (rf_conf_calculate_channel_spacing_registers(phy_subghz.channel_spacing / ch_space_divider, &ch_space)) {
        ch_space_divider++;
        rf_channel_multiplier++;
    }
    rf_write_register(CHSPACE, ch_space);
}

static void rf_init_registers(void)
{
    rf_write_register_field(PCKTCTRL3, PCKT_FORMAT_FIELD, PCKT_FORMAT_802_15_4);
    rf_write_register_field(MOD2, MOD_TYPE_FIELD, MOD_2FSK);
    rf_write_register(PCKT_FLT_OPTIONS, 0);
    rf_write_register_field(PCKTCTRL1, PCKT_CRCMODE_FIELD, PCKT_CRCMODE_0x04C11DB7);
    rf_write_register_field(PCKTCTRL1, PCKT_TXSOURCE_FIELD, PCKT_TXSOURCE_NORMAL);
    rf_write_register_field(PCKTCTRL1, PCKT_WHITENING_FIELD, PCKT_WHITENING_ENABLED);
    rf_write_register_field(PCKTCTRL2, PCKT_FIXVARLEN_FIELD, PCKT_VARIABLE_LEN);
    rf_write_register_field(PCKTCTRL2, PCKT_FCS_TYPE_FIELD, PCKT_FCS_TYPE_4_OCTET);
    rf_write_register_field(PCKTCTRL3, PCKT_RXMODE_FIELD, PCKT_RXMODE_NORMAL);
    rf_write_register_field(PCKTCTRL3, PCKT_BYTE_SWAP_FIELD, PCKT_BYTE_SWAP_LSB);
    rf_write_register(PCKTCTRL5, PCKT_PREAMBLE_LEN);
    rf_write_register_field(PCKTCTRL6, PCKT_SYNCLEN_FIELD, PCKT_SYNCLEN);
    rf_write_register_field(QI, PQI_TH_FIELD, PQI_TH);
    rf_write_register_field(QI, SQI_EN_FIELD, SQI_EN);
    rf_write_register(SYNC0, SFD0);
    rf_write_register(SYNC1, SFD1);
    rf_set_channel_configuration_registers();
}

static int8_t rf_address_write(phy_address_type_e address_type, uint8_t *address_ptr)
{
    int8_t ret_val = 0;
    switch (address_type) {
        /*Set 48-bit address*/
        case PHY_MAC_48BIT:
            break;
        /*Set 64-bit address*/
        case PHY_MAC_64BIT:
            memcpy(s2lp_MAC, address_ptr, 8);
            break;
        /*Set 16-bit address*/
        case PHY_MAC_16BIT:
            memcpy(s2lp_short_address, address_ptr, 2);
            break;
        /*Set PAN Id*/
        case PHY_MAC_PANID:
            memcpy(s2lp_PAN_ID, address_ptr, 2);
            break;
    }
    return ret_val;
}

static int8_t rf_extension(phy_extension_type_e extension_type, uint8_t *data_ptr)
{
    int8_t retval = 0;
    phy_csma_params_t *csma_params;
    phy_rf_channel_configuration_s *channel_params;
    uint32_t *timer_value;
    switch (extension_type) {
        case PHY_EXTENSION_SET_CHANNEL:
            if (rf_state == RF_IDLE || rf_state == RF_CSMA_STARTED) {
                rf_receive(*data_ptr);
            } else {
                // Store the new channel if couldn't change it yet.
                rf_new_channel = *data_ptr;
                retval = -1;
            }
            break;
        case PHY_EXTENSION_CTRL_PENDING_BIT:
            break;
        /*Return frame pending status*/
        case PHY_EXTENSION_READ_LAST_ACK_PENDING_STATUS:
            break;
        case PHY_EXTENSION_ACCEPT_ANY_BEACON:
            break;
        case PHY_EXTENSION_SET_TX_TIME:
            tx_time = common_read_32_bit(data_ptr);
            break;
        case PHY_EXTENSION_READ_RX_TIME:
            common_write_32_bit(rx_time, data_ptr);
            break;
        case PHY_EXTENSION_DYNAMIC_RF_SUPPORTED:
            *data_ptr = true;
            break;
        case PHY_EXTENSION_GET_TIMESTAMP:
            timer_value = (uint32_t *)data_ptr;
            *timer_value = rf_get_timestamp();
            break;
        case PHY_EXTENSION_SET_CSMA_PARAMETERS:
            csma_params = (phy_csma_params_t *)data_ptr;
            if (csma_params->backoff_time == 0) {
                rf_cca_timer_stop();
                if (rf_read_register(TX_FIFO_STATUS)) {
                    rf_send_command(S2LP_CMD_SABORT);
                    rf_poll_state_change(S2LP_STATE_READY);
                    rf_send_command(S2LP_CMD_FLUSHTXFIFO);
                    rf_poll_state_change(S2LP_STATE_READY);
                }
                if (rf_state == RF_TX_STARTED) {
                    rf_state = RF_IDLE;
                    rf_receive(rf_rx_channel);
                }
                tx_time = 0;
            } else {
                tx_time = csma_params->backoff_time;
                cca_enabled = csma_params->cca_enabled;
            }
            break;
        case PHY_EXTENSION_READ_TX_FINNISH_TIME:
            timer_value = (uint32_t *)data_ptr;
            *timer_value = tx_finnish_time;
            break;

        case PHY_EXTENSION_GET_SYMBOLS_PER_SECOND:
            timer_value = (uint32_t *)data_ptr;
            *timer_value = rf_symbol_rate;
            break;
        case PHY_EXTENSION_SET_RF_CONFIGURATION:
            channel_params = (phy_rf_channel_configuration_s *)data_ptr;
            phy_subghz.datarate = channel_params->datarate;
            phy_subghz.channel_spacing = channel_params->channel_spacing;
            phy_subghz.channel_0_center_frequency = channel_params->channel_0_center_frequency;
            phy_subghz.number_of_channels = channel_params->number_of_channels;
            phy_subghz.modulation = channel_params->modulation;
            phy_subghz.modulation_index = channel_params->modulation_index;
            rf_calculate_symbol_rate(phy_subghz.datarate, phy_subghz.modulation);
            rf_update_config = true;
            if (rf_state == RF_IDLE) {
                rf_receive(rf_rx_channel);
            }
            break;
        case PHY_EXTENSION_SET_TX_POWER:
            // TODO: Set TX output power
            break;
        case PHY_EXTENSION_SET_CCA_THRESHOLD:
            rssi_threshold = rf_conf_cca_threshold_percent_to_rssi(*data_ptr);
            rf_update_config = true;
            if (rf_state == RF_IDLE) {
                rf_receive(rf_rx_channel);
            }
            break;
        default:
            break;
    }

    return retval;
}


static int8_t rf_interface_state_control(phy_interface_state_e new_state, uint8_t rf_channel)
{
    int8_t ret_val = 0;
    switch (new_state) {
        /*Reset PHY driver and set to idle*/
        case PHY_INTERFACE_RESET:
            break;
        /*Disable PHY Interface driver*/
        case PHY_INTERFACE_DOWN:
            rf_lock();
            rf_send_command(S2LP_CMD_SABORT);
            rf_disable_all_interrupts();
            rf_poll_state_change(S2LP_STATE_READY);
            rf_flush_rx_fifo();
            rf_flush_tx_fifo();
            rf_state = RF_IDLE;
            rf_unlock();
            break;
        /*Enable PHY Interface driver*/
        case PHY_INTERFACE_UP:
            rf_mode = RF_MODE_NORMAL;
            rf_receive(rf_channel);
            break;
        /*Enable wireless interface ED scan mode*/
        case PHY_INTERFACE_RX_ENERGY_STATE:
            break;
        /*Enable Sniffer state*/
        case PHY_INTERFACE_SNIFFER_STATE:
            rf_mode = RF_MODE_SNIFFER;
            rf_receive(rf_channel);
            break;
    }
    return ret_val;
}

static void rf_tx_sent_handler(void)
{
    rf_backup_timer_stop();
    rf_disable_interrupt(TX_DATA_SENT);
    if (rf_state != RF_TX_ACK) {
        tx_finnish_time = rf_get_timestamp();
        TEST_TX_DONE
        rf_state = RF_IDLE;
        rf_receive(rf_rx_channel);
        if (device_driver.phy_tx_done_cb) {
            device_driver.phy_tx_done_cb(rf_radio_driver_id, mac_tx_handle, PHY_LINK_TX_SUCCESS, 0, 0);
        }
    } else {
        TEST_ACK_TX_DONE
        rf_receive(rf_rx_channel);
    }
}

static void rf_tx_threshold_handler(void)
{
    rf_backup_timer_stop();
    rf_disable_interrupt(TX_FIFO_ALMOST_EMPTY);
    // TODO check the FIFO threshold. By default, threshold is half of the FIFO size
    uint8_t written_length = rf_write_tx_fifo(tx_data_ptr, tx_data_length);
    if (written_length < tx_data_length) {
        tx_data_ptr += written_length;
        tx_data_length -= written_length;
        rf_enable_interrupt(TX_FIFO_ALMOST_EMPTY);
    }
    rf_backup_timer_start(MAX_PACKET_SENDING_TIME);
}

static void rf_start_tx(void)
{
    rf_send_command(S2LP_CMD_SABORT);
    rf_disable_all_interrupts();
    rf_poll_state_change(S2LP_STATE_READY);
    rf_state_change(S2LP_STATE_TX, false);
    // More TX data to be written in FIFO when TX threshold interrupt occurs
    if (tx_data_ptr) {
        rf_enable_interrupt(TX_FIFO_ALMOST_EMPTY);
    }
    rf_enable_interrupt(TX_DATA_SENT);
    rf_enable_interrupt(TX_FIFO_UNF_OVF);
    rf_backup_timer_start(MAX_PACKET_SENDING_TIME);
}

static int rf_cca_check(void)
{
    uint32_t time_from_receiver_ready = rf_get_timestamp() - receiver_ready_timestamp;
    if (time_from_receiver_ready < RSSI_SETTLING_TIME) {
        wait_us(RSSI_SETTLING_TIME - time_from_receiver_ready);
    }
    return (rf_read_register(LINK_QUALIF1) & CARRIER_SENSE);
}

static void rf_cca_timer_interrupt(void)
{
    int8_t status = device_driver.phy_tx_done_cb(rf_radio_driver_id, mac_tx_handle, PHY_LINK_CCA_PREPARE, 0, 0);
    if (status == PHY_TX_NOT_ALLOWED) {
        rf_flush_tx_fifo();
        if (rf_state == RF_CSMA_STARTED) {
            rf_state = RF_IDLE;
        }
        return;
    }
    if ((cca_enabled == true) && ((rf_state != RF_CSMA_STARTED && rf_state != RF_IDLE) || (read_irq_status() & (1 << SYNC_WORD)) || rf_cca_check())) {
        if (rf_state == RF_CSMA_STARTED) {
            rf_state = RF_IDLE;
        }
        rf_flush_tx_fifo();
        tx_finnish_time = rf_get_timestamp();
        if (device_driver.phy_tx_done_cb) {
            device_driver.phy_tx_done_cb(rf_radio_driver_id, mac_tx_handle, PHY_LINK_CCA_FAIL, 0, 0);
        }
    } else {
        if (status == PHY_RESTART_CSMA) {
            if (device_driver.phy_tx_done_cb) {
                device_driver.phy_tx_done_cb(rf_radio_driver_id, mac_tx_handle, PHY_LINK_CCA_OK, 0, 0);
            }
            if (tx_time) {
                uint32_t backoff_time = tx_time - rf_get_timestamp();
                // Max. time to TX can be 65ms, otherwise time has passed already -> send immediately
                if (backoff_time > 65000) {
                    backoff_time = 1;
                }
                rf_cca_timer_start(backoff_time);
            }
        } else {
            rf_start_tx();
            rf_state = RF_TX_STARTED;
            TEST_TX_STARTED
        }
    }
}

static void rf_cca_timer_stop(void)
{
    rf->cca_timer.detach();
}

static void rf_cca_timer_start(uint32_t slots)
{
    rf->cca_timer.attach_us(rf_cca_timer_signal, slots);
}

static void rf_backup_timer_interrupt(void)
{
    tx_finnish_time = rf_get_timestamp();
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
        if (device_driver.phy_tx_done_cb) {
            device_driver.phy_tx_done_cb(rf_radio_driver_id, mac_tx_handle, PHY_LINK_TX_SUCCESS, 0, 0);
        }
    }
    rf_flush_tx_fifo();
    TEST_TX_DONE
    TEST_RX_DONE
    rf_state = RF_IDLE;
    rf_receive(rf_rx_channel);
}

static void rf_backup_timer_stop(void)
{
    rf->backup_timer.detach();
}

static void rf_backup_timer_start(uint32_t slots)
{
    rf->backup_timer.attach_us(rf_backup_timer_signal, slots);
}

static int8_t rf_start_cca(uint8_t *data_ptr, uint16_t data_length, uint8_t tx_handle, data_protocol_e data_protocol)
{
    rf_lock();
    if (rf_state != RF_IDLE) {
        rf_unlock();
        return -1;
    }
    rf_state = RF_CSMA_STARTED;
    uint8_t written_length = rf_write_tx_fifo(data_ptr, data_length);
    if (written_length < data_length) {
        tx_data_ptr = data_ptr + written_length;
        tx_data_length = data_length - written_length;
    } else {
        tx_data_ptr = NULL;
    }
    // If Ack is requested, store the MAC sequence. This will be compared with received Ack.
    uint8_t version = ((*(data_ptr + 1) & VERSION_FIELD_MASK) >> SHIFT_VERSION_FIELD);
    if ((version != MAC_FRAME_VERSION_2) && (*data_ptr & FC_AR)) {
        tx_sequence = *(data_ptr + 2);
    }
    // TODO: Define this better
    rf_write_packet_length(data_length + 4);
    mac_tx_handle = tx_handle;
    if (tx_time) {
        uint32_t backoff_time = tx_time - rf_get_timestamp();
        // Max. time to TX can be 65ms, otherwise time has passed already -> send immediately
        if (backoff_time <= 65000) {
            rf_cca_timer_start(backoff_time);
            rf_unlock();
            return 0;
        }
    }
    // Short timeout to start CCA immediately.
    rf_cca_timer_start(1);
    rf_unlock();
    return 0;
}

static void rf_send_ack(uint8_t seq)
{
    rf_state = RF_TX_ACK;
    uint8_t ack_frame[3] = {MAC_TYPE_ACK, 0, seq};
    rf_write_tx_fifo(ack_frame, sizeof(ack_frame));
    rf_write_packet_length(sizeof(ack_frame) + 4);
    tx_data_ptr = NULL;
    rf_start_tx();
    TEST_ACK_TX_STARTED
    rf_backup_timer_start(ACK_SENDING_TIME);
}

static void rf_handle_ack(uint8_t seq_number, uint8_t pending)
{
    phy_link_tx_status_e phy_status;
    if (tx_sequence == (uint16_t)seq_number) {
        tx_finnish_time = rf_get_timestamp();
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

static void rf_rx_ready_handler(void)
{
    rf_backup_timer_stop();
    TEST_RX_DONE
    rf_flush_tx_fifo();
    int rx_read_length = rf_read_rx_fifo(rx_data_length, rf_read_register(RX_FIFO_STATUS));
    if (rx_read_length < 0) {
        rf_receive(rf_rx_channel);
        return;
    }
    rx_data_length += rx_read_length;
    if (rf_mode != RF_MODE_SNIFFER) {
        rf_state = RF_IDLE;
        uint8_t version = ((rx_buffer[1] & VERSION_FIELD_MASK) >> SHIFT_VERSION_FIELD);
        if (((rx_buffer[0] & MAC_FRAME_TYPE_MASK) == MAC_TYPE_ACK) && (version < MAC_FRAME_VERSION_2)) {
            rf_handle_ack(rx_buffer[2], rx_buffer[0] & MAC_DATA_PENDING);
        } else if (rf_rx_filter(rx_buffer, s2lp_MAC, s2lp_short_address, s2lp_PAN_ID)) {
            int8_t rssi = (rf_read_register(RSSI_LEVEL) - RSSI_OFFSET);
            if (device_driver.phy_rx_cb) {
                device_driver.phy_rx_cb(rx_buffer, rx_data_length, 0xf0, rssi, rf_radio_driver_id);
            }
            // Check Ack request
            if ((version != MAC_FRAME_VERSION_2) && (rx_buffer[0] & FC_AR)) {
                rf_send_ack(rx_buffer[2]);
            }
        }
    } else {
        rf_state = RF_IDLE;
        int8_t rssi = (rf_read_register(RSSI_LEVEL) - RSSI_OFFSET);
        if (device_driver.phy_rx_cb) {
            device_driver.phy_rx_cb(rx_buffer, rx_data_length, 0xf0, rssi, rf_radio_driver_id);
        }
    }
    if ((rf_state != RF_TX_ACK) && (rf_state != RF_CSMA_STARTED)) {
        rf_receive(rf_rx_channel);
    }
}

static void rf_rx_threshold_handler(void)
{
    rf_backup_timer_stop();
    int rx_read_length = rf_read_rx_fifo(rx_data_length, rf_read_register(RX_FIFO_STATUS));
    if (rx_read_length < 0) {
        rf_receive(rf_rx_channel);
        return;
    }
    rx_data_length += rx_read_length;
    rf_backup_timer_start(MAX_PACKET_SENDING_TIME);
}

static void rf_sync_detected_handler(void)
{
    rx_time = rf_get_timestamp();
    rf_state = RF_RX_STARTED;
    TEST_RX_STARTED
    rf_disable_interrupt(SYNC_WORD);
    rf_enable_interrupt(RX_FIFO_ALMOST_FULL);
    rf_enable_interrupt(RX_DATA_READY);
    rf_backup_timer_start(MAX_PACKET_SENDING_TIME);
}

static void rf_receive(uint8_t rx_channel)
{
    if (rf_state == RF_TX_STARTED) {
        return;
    }
    rf_lock();
    rf_send_command(S2LP_CMD_SABORT);
    rf_disable_all_interrupts();
    rf_poll_state_change(S2LP_STATE_READY);
    rf_flush_rx_fifo();
    if (rf_update_config == true) {
        rf_channel_multiplier = 1;
        rf_update_config = false;
        rf_set_channel_configuration_registers();
    }
    if (rx_channel != rf_rx_channel) {
        rf_write_register(CHNUM, rx_channel * rf_channel_multiplier);
        rf_rx_channel = rf_new_channel = rx_channel;
    }
    rf_send_command(S2LP_CMD_RX);
    rf_enable_interrupt(SYNC_WORD);
    rf_enable_interrupt(RX_FIFO_UNF_OVF);
    rx_data_length = 0;
    if (rf_state != RF_CSMA_STARTED) {
        rf_state = RF_IDLE;
    }
    rf_poll_state_change(S2LP_STATE_RX);
    receiver_ready_timestamp = rf_get_timestamp();
    rf_unlock();
}

static void rf_interrupt_handler(void)
{
    rf->irq_thread.flags_set(SIG_RADIO);
}

void RFPins::rf_irq_task(void)
{
    for (;;) {
        uint32_t flags = ThisThread::flags_wait_any(SIG_ALL);
        rf_lock();
        if (flags & SIG_RADIO) {
            rf_irq_task_process_irq();
        }
        if (flags & SIG_TIMER_CCA) {
            rf_cca_timer_interrupt();
        }
        if (flags & SIG_TIMER_BACKUP) {
            rf_backup_timer_interrupt();
        }
        rf_unlock();
    }
}

static void rf_irq_task_process_irq(void)
{
    rf_lock();
    uint32_t irq_status = read_irq_status();
    if (rf_state == RF_TX_STARTED || rf_state == RF_TX_ACK) {
        if ((irq_status & (1 << TX_DATA_SENT)) && (enabled_interrupts & (1 << TX_DATA_SENT))) {
            rf_tx_sent_handler();
        }
    }
    if (rf_state == RF_TX_STARTED) {
        if ((irq_status & (1 << TX_FIFO_ALMOST_EMPTY)) && (enabled_interrupts & (1 << TX_FIFO_ALMOST_EMPTY))) {
            rf_tx_threshold_handler();
        }
    }
    if ((irq_status & (1 << TX_FIFO_UNF_OVF)) && (enabled_interrupts & (1 << TX_FIFO_UNF_OVF))) {
        rf_backup_timer_stop();
        tx_finnish_time = rf_get_timestamp();
        TEST_TX_DONE
        device_driver.phy_tx_done_cb(rf_radio_driver_id, mac_tx_handle, PHY_LINK_CCA_FAIL, 1, 0);
        rf_send_command(S2LP_CMD_SABORT);
        rf_poll_state_change(S2LP_STATE_READY);
        rf_send_command(S2LP_CMD_FLUSHTXFIFO);
        rf_poll_state_change(S2LP_STATE_READY);
        rf_state = RF_IDLE;
        rf_receive(rf_rx_channel);
    }
    if (rf_state == RF_IDLE || rf_state == RF_CSMA_STARTED || rf_state == RF_TX_STARTED) {
        if ((irq_status & (1 << SYNC_WORD)) && (enabled_interrupts & (1 << SYNC_WORD))) {
            rf_sync_detected_handler();
        }
    } else if (rf_state == RF_RX_STARTED) {
        if ((irq_status & (1 << RX_DATA_READY)) && (enabled_interrupts & (1 << RX_DATA_READY))) {
            if (!(irq_status & (1 << CRC_ERROR))) {
                rf_rx_ready_handler();
            } else {
                TEST_RX_DONE
                rf_backup_timer_stop();
                rf_flush_tx_fifo();
                rf_state = RF_IDLE;
                // In case the channel change was called during reception, driver is responsible to change the channel if CRC failed.
                rf_receive(rf_new_channel);
                if (device_driver.phy_rf_statistics) {
                    device_driver.phy_rf_statistics->crc_fails++;
                }
            }
        }
        if ((irq_status & (1 << RX_FIFO_ALMOST_FULL)) && (enabled_interrupts & (1 << RX_FIFO_ALMOST_FULL))) {
            rf_rx_threshold_handler();
        }
        if ((irq_status & (1 << RX_DATA_DISCARDED)) && (enabled_interrupts & (1 << RX_DATA_DISCARDED))) {

        }
        if ((irq_status & (1 << CRC_ERROR)) && (enabled_interrupts & (1 << CRC_ERROR))) {

        }
    }
    if ((irq_status & (1 << RX_FIFO_UNF_OVF)) && (enabled_interrupts & (1 << RX_FIFO_UNF_OVF))) {
        TEST_RX_DONE
        rf_backup_timer_stop();
        rf_send_command(S2LP_CMD_SABORT);
        rf_poll_state_change(S2LP_STATE_READY);
        rf_send_command(S2LP_CMD_FLUSHRXFIFO);
        rf_poll_state_change(S2LP_STATE_READY);
        rf_flush_tx_fifo();
        rf_state = RF_IDLE;
        rf_receive(rf_rx_channel);
    }
    rf_unlock();
}

static void rf_reset(void)
{
    // Shutdown
    rf->SDN = 1;
    ThisThread::sleep_for(10);
    // Wake up
    rf->SDN = 0;
    ThisThread::sleep_for(10);
}

static void rf_init(void)
{
#ifdef TEST_GPIOS_ENABLED
    fhss_bc_switch = test1_toggle;
    fhss_uc_switch = test2_toggle;
#endif //TEST_GPIOS_ENABLED
    rf_reset();
    rf->spi.frequency(10000000);
    CS_RELEASE();
    rf_init_registers();
    rf_enable_gpio_interrupt();
    rf_calculate_symbol_rate(phy_subghz.datarate, phy_subghz.modulation);
    rf->tx_timer.start();
}

static int8_t rf_device_register(const uint8_t *mac_addr)
{
    rf_init();
    /*Set pointer to MAC address*/
    device_driver.PHY_MAC = (uint8_t *)mac_addr;
    device_driver.driver_description = (char *)"S2LP_MAC";
    device_driver.link_type = PHY_LINK_15_4_SUBGHZ_TYPE;
    device_driver.phy_channel_pages = phy_channel_pages;
    device_driver.phy_MTU = RF_MTU;
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
    return rf_radio_driver_id;
}

static void rf_device_unregister()
{
    if (rf_radio_driver_id >= 0) {
        arm_net_phy_unregister(rf_radio_driver_id);
        rf_radio_driver_id = -1;
    }
}

void NanostackRfPhys2lp::get_mac_address(uint8_t *mac)
{
    rf_lock();

    if (NULL == rf) {
        error("NanostackRfPhys2lp Must be registered to read mac address");
        rf_unlock();
        return;
    }
    memcpy((void *)mac, (void *)_mac_addr, sizeof(_mac_addr));

    rf_unlock();
}

void NanostackRfPhys2lp::set_mac_address(uint8_t *mac)
{
    rf_lock();

    if (NULL != rf) {
        error("NanostackRfPhys2lp cannot change mac address when running");
        rf_unlock();
        return;
    }
    memcpy((void *)_mac_addr, (void *)mac, sizeof(_mac_addr));
    _mac_set = true;

    rf_unlock();
}

int8_t NanostackRfPhys2lp::rf_register()
{
    if (NULL == _rf) {
        return -1;
    }
    rf_lock();
    if (rf != NULL) {
        rf_unlock();
        error("Multiple registrations of NanostackRfPhyAtmel not supported");
        return -1;
    }

    if (!_mac_set) {
#ifdef AT24MAC
        int ret = _mac.read_eui64((void *)s2lp_MAC);
        if (ret < 0) {
            rf = NULL;
            rf_unlock();
            return -1;
        }
#else
        randLIB_seed_random();
        randLIB_get_n_bytes_random(s2lp_MAC, 8);
        s2lp_MAC[0] |= 2; //Set Local Bit
        s2lp_MAC[0] &= ~1; //Clear multicast bit
#endif
        set_mac_address(s2lp_MAC);
    }

    rf = _rf;

    int8_t radio_id = rf_device_register(_mac_addr);
    if (radio_id < 0) {
        rf = NULL;
    }
    rf_unlock();
    return radio_id;
}

void NanostackRfPhys2lp::rf_unregister()
{
    rf_lock();
    if (NULL == rf) {
        rf_unlock();
        return;
    }
    rf_device_unregister();
    rf = NULL;
    rf_unlock();
}

NanostackRfPhys2lp::NanostackRfPhys2lp(PinName spi_sdi, PinName spi_sdo, PinName spi_sclk, PinName spi_cs, PinName spi_sdn
#ifdef TEST_GPIOS_ENABLED
                                       ,PinName spi_test1, PinName spi_test2, PinName spi_test3, PinName spi_test4, PinName spi_test5
#endif //TEST_GPIOS_ENABLED
                                       ,PinName spi_gpio0, PinName spi_gpio1, PinName spi_gpio2, PinName spi_gpio3
#ifdef AT24MAC
                                       ,PinName i2c_sda, PinName i2c_scl
#endif //AT24MAC
                                       )
    :
#ifdef AT24MAC
                    _mac(i2c_sda, i2c_scl),
#endif //AT24MAC
                    _mac_addr(), _rf(NULL), _mac_set(false),
      _spi_sdi(spi_sdi), _spi_sdo(spi_sdo), _spi_sclk(spi_sclk), _spi_cs(spi_cs), _spi_sdn(spi_sdn),
#ifdef TEST_GPIOS_ENABLED
      _spi_test1(spi_test1), _spi_test2(spi_test2), _spi_test3(spi_test3), _spi_test4(spi_test4), _spi_test5(spi_test5),
#endif //TEST_GPIOS_ENABLED
      _spi_gpio0(spi_gpio0), _spi_gpio1(spi_gpio1), _spi_gpio2(spi_gpio2), _spi_gpio3(spi_gpio3)
{
    _rf = new RFPins(_spi_sdi, _spi_sdo, _spi_sclk, _spi_cs, _spi_sdn,
#ifdef TEST_GPIOS_ENABLED
                     _spi_test1, _spi_test2, _spi_test3, _spi_test4, _spi_test5,
#endif //TEST_GPIOS_ENABLED
                     _spi_gpio0, _spi_gpio1, _spi_gpio2, _spi_gpio3);
}

NanostackRfPhys2lp::~NanostackRfPhys2lp()
{
    delete _rf;
}

static bool rf_panid_filter_common(uint8_t *panid_start, uint8_t *pan_id, uint8_t frame_type)
{
    // PHY driver shouldn't drop received Beacon frames as they might be used by load balancing
    if (frame_type == MAC_FRAME_BEACON) {
        return true;
    }
    bool retval = true;
    uint8_t cmp_table[2] = {0xff, 0xff};
    if (!(pan_id[0] == 0xff && pan_id[1] == 0xff)) {
        if (memcmp((uint8_t *)panid_start, (uint8_t *) cmp_table, 2)) {
            retval = false;
        }
        if (!retval) {
            for (uint8_t i = 0; i < 2; i++) {
                cmp_table[1 - i] = panid_start[i];
            }
            if (!memcmp(pan_id, cmp_table, 2)) {
                retval = true;
            }
        }
    }
    return retval;
}

static bool rf_panid_v1_v0_filter(uint8_t *ptr, uint8_t *pan_id, uint8_t frame_type)
{
    return rf_panid_filter_common(ptr, pan_id, frame_type);
}

static bool rf_addr_filter_common(uint8_t *ptr, uint8_t addr_mode, uint8_t *mac_64bit_addr, uint8_t *mac_16bit_addr)
{
    uint8_t cmp_table[8] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    bool retval = true;
    switch (addr_mode) {
        case FC_DST_16_BITS:
            if (memcmp((uint8_t *)ptr, (uint8_t *) cmp_table, 2)) {
                retval = false;
            }
            if (!retval) {
                for (uint8_t i = 0; i < 2; i++) {
                    cmp_table[1 - i] = ptr[i];
                }

                if (!memcmp((uint8_t *)mac_16bit_addr, (uint8_t *) cmp_table, 2)) {
                    retval = true;
                }
            }
            break;
        case FC_DST_64_BITS:
            if (memcmp((uint8_t *)ptr, (uint8_t *) cmp_table, 8)) {
                retval = false;
            }
            if (!retval) {
                for (uint8_t i = 0; i < 8; i++) {
                    cmp_table[7 - i] = ptr[i];
                }

                if (!memcmp((uint8_t *)mac_64bit_addr, (uint8_t *) cmp_table, 8)) {
                    retval = true;
                }
            }
            break;
        case FC_DST_ADDR_NONE:
            retval = true;
            break;
        default:
            retval = false;
            break;
    }
    return retval;
}

static bool rf_addr_v1_v0_filter(uint8_t *ptr, uint8_t *mac_64bit_addr, uint8_t *mac_16bit_addr, uint8_t dst_mode)
{
    return rf_addr_filter_common(ptr, dst_mode, mac_64bit_addr, mac_16bit_addr);
}

static bool rf_rx_filter(uint8_t *mac_header, uint8_t *mac_64bit_addr, uint8_t *mac_16bit_addr, uint8_t *pan_id)
{
    uint8_t dst_mode = (mac_header[1] & FC_DST_MODE);
    uint8_t frame_type = mac_header[0] & MAC_FRAME_TYPE_MASK;
    uint8_t version = ((mac_header[1] & VERSION_FIELD_MASK) >> SHIFT_VERSION_FIELD);
    if (version != MAC_FRAME_VERSION_2) {
        if (!rf_panid_v1_v0_filter(mac_header + OFFSET_DST_PAN_ID, pan_id, frame_type)) {
            return false;
        }
        if (!rf_addr_v1_v0_filter(mac_header + OFFSET_DST_ADDR, mac_64bit_addr, mac_16bit_addr, dst_mode)) {
            return false;
        }
    }
    return true;
}

#if MBED_CONF_S2LP_PROVIDE_DEFAULT
NanostackRfPhy &NanostackRfPhy::get_default_instance()
{
    static NanostackRfPhys2lp rf_phy(S2LP_SPI_SDI, S2LP_SPI_SDO, S2LP_SPI_SCLK, S2LP_SPI_CS, S2LP_SPI_SDN
#ifdef TEST_GPIOS_ENABLED
                                     ,S2LP_SPI_TEST1, S2LP_SPI_TEST2, S2LP_SPI_TEST3, S2LP_SPI_TEST4, S2LP_SPI_TEST5
#endif //TEST_GPIOS_ENABLED
                                     ,S2LP_SPI_GPIO0, S2LP_SPI_GPIO1, S2LP_SPI_GPIO2, S2LP_SPI_GPIO3
#ifdef AT24MAC
                                     ,S2LP_I2C_SDA, S2LP_I2C_SCL
#endif //AT24MAC
                                     );
    return rf_phy;
}
#endif // MBED_CONF_S2LP_PROVIDE_DEFAULT
#endif // MBED_CONF_NANOSTACK_CONFIGURATION && DEVICE_SPI
