/**
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    (C)2013 Semtech
 ___ _____ _   ___ _  _____ ___  ___  ___ ___
/ __|_   _/_\ / __| |/ / __/ _ \| _ \/ __| __|
\__ \ | |/ _ \ (__| ' <| _| (_) |   / (__| _|
|___/ |_/_/ \_\___|_|\_\_| \___/|_|_\\___|___|
embedded.connectivity.solutions===============

Description: LoRaWAN stack layer that controls both MAC and PHY underneath

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis ( Semtech ), Gregory Cristian ( Semtech ) and Daniel Jaeckle ( STACKFORCE )

Copyright (c) 2017, Arm Limited and affiliates.

SPDX-License-Identifier: BSD-3-Clause
*/

#include <stdio.h>
#include <math.h> //rint
#include <string.h>
#include "PinNames.h"
#include "Callback.h"
#include "mbed_wait_api.h"
#include "Timer.h"
#ifdef MBED_CONF_RTOS_PRESENT
#include "ThisThread.h"
using namespace rtos;
#endif
#include "SX1276_LoRaRadio.h"
#include "sx1276Regs-Fsk.h"
#include "sx1276Regs-LoRa.h"

/*!
 * Sync word for Private LoRa networks
 */
#define LORA_MAC_PRIVATE_SYNCWORD                   0x12

/*!
 * Sync word for Public LoRa networks
 */
#define LORA_MAC_PUBLIC_SYNCWORD                    0x34

/*!
 * SX1276 definitions
 */
#define XTAL_FREQ                                   32000000
#define FREQ_STEP                                   61.03515625

/*!
 * Constant values need to compute the RSSI value
 */
#define RSSI_OFFSET_LF                              -164.0
#define RSSI_OFFSET_HF                              -157.0
#define RF_MID_BAND_THRESH                          525000000


/*!
 * FSK bandwidth definition
 */
typedef struct
{
    uint32_t bandwidth;
    uint8_t  register_value;
} fsk_bw_t;

/*!
 * Radio registers definition
 */
typedef struct
{
    uint8_t     modem;
    uint8_t     addr;
    uint8_t     value;
} radio_registers_t;

#define RADIO_INIT_REGISTERS_VALUE                \
{                                                 \
    { MODEM_FSK , REG_LNA                , 0x23 },\
    { MODEM_FSK , REG_RXCONFIG           , 0x1E },\
    { MODEM_FSK , REG_RSSICONFIG         , 0xD2 },\
    { MODEM_FSK , REG_AFCFEI             , 0x01 },\
    { MODEM_FSK , REG_PREAMBLEDETECT     , 0xAA },\
    { MODEM_FSK , REG_OSC                , 0x07 },\
    { MODEM_FSK , REG_SYNCCONFIG         , 0x12 },\
    { MODEM_FSK , REG_SYNCVALUE1         , 0xC1 },\
    { MODEM_FSK , REG_SYNCVALUE2         , 0x94 },\
    { MODEM_FSK , REG_SYNCVALUE3         , 0xC1 },\
    { MODEM_FSK , REG_PACKETCONFIG1      , 0xD8 },\
    { MODEM_FSK , REG_FIFOTHRESH         , 0x8F },\
    { MODEM_FSK , REG_IMAGECAL           , 0x02 },\
    { MODEM_FSK , REG_DIOMAPPING1        , 0x00 },\
    { MODEM_FSK , REG_DIOMAPPING2        , 0x30 },\
    { MODEM_LORA, REG_LR_PAYLOADMAXLENGTH, 0x40 },\
}

static const fsk_bw_t fsk_bandwidths[] =
{
    { 2600  , 0x17 },
    { 3100  , 0x0F },
    { 3900  , 0x07 },
    { 5200  , 0x16 },
    { 6300  , 0x0E },
    { 7800  , 0x06 },
    { 10400 , 0x15 },
    { 12500 , 0x0D },
    { 15600 , 0x05 },
    { 20800 , 0x14 },
    { 25000 , 0x0C },
    { 31300 , 0x04 },
    { 41700 , 0x13 },
    { 50000 , 0x0B },
    { 62500 , 0x03 },
    { 83333 , 0x12 },
    { 100000, 0x0A },
    { 125000, 0x02 },
    { 166700, 0x11 },
    { 200000, 0x09 },
    { 250000, 0x01 },
    { 300000, 0x00 }, // Invalid bandwidth
};

/**
 * SPI read/write masks
 */
#define SPI_WRITE_CMD   0x80
#define SPI_READ_CMD    0x7F

/**
 * Signals
 */
#define SIG_DIO0    0x01
#define SIG_DIO1    0x02
#define SIG_DIO2    0x04
#define SIG_DIO3    0x08
#define SIG_DIO4    0x10
#define SIG_DIO5    0x20
#define SIG_TIMOUT  0x40

/**
 * Radio hardware registers initialization
 */
static const radio_registers_t radio_reg_init[] = RADIO_INIT_REGISTERS_VALUE;

enum RadioVariant {
    SX1276UNDEFINED = 0,
    SX1276MB1LAS,
    SX1276MB1MAS
};

#ifdef MBED_SX1276_LORA_RADIO_SPI_FREQUENCY
#define SPI_FREQUENCY    MBED_SX1276_LORA_RADIO_SPI_FREQUENCY
#else
#define SPI_FREQUENCY    8000000
#endif

using namespace mbed;
/**
 * Constructor
 */
SX1276_LoRaRadio::SX1276_LoRaRadio(PinName spi_mosi,
                                   PinName spi_miso,
                                   PinName spi_sclk,
                                   PinName nss,
                                   PinName reset,
                                   PinName dio0,
                                   PinName dio1,
                                   PinName dio2,
                                   PinName dio3,
                                   PinName dio4,
                                   PinName dio5,
                                   PinName rf_switch_ctl1,
                                   PinName rf_switch_ctl2,
                                   PinName txctl,
                                   PinName rxctl,
                                   PinName antswitch,
                                   PinName pwr_amp_ctl,
                                   PinName tcxo)
    :   _spi(spi_mosi, spi_miso, spi_sclk),
        _chip_select(nss, 1),
        _reset_ctl(reset),
        _dio0_ctl(dio0), _dio1_ctl(dio1), _dio2_ctl(dio2), _dio3_ctl(dio3), _dio4_ctl(dio4), _dio5_ctl(dio5),
        _rf_switch_ctl1(rf_switch_ctl1, 0), _rf_switch_ctl2(rf_switch_ctl2, 0),
        _txctl(txctl, 0), _rxctl(rxctl, 0),
        _ant_switch(antswitch, PIN_INPUT, PullUp, 0),
        _pwr_amp_ctl(pwr_amp_ctl),
        _tcxo(tcxo)

#ifdef MBED_CONF_RTOS_PRESENT
        , irq_thread(osPriorityRealtime, 1024, NULL, "LR-SX1276")
#endif
{
    _rf_ctrls.ant_switch = antswitch;
    _rf_ctrls.pwr_amp_ctl = pwr_amp_ctl;
    _rf_ctrls.rf_switch_ctl1 = rf_switch_ctl1;
    _rf_ctrls.rf_switch_ctl2 = rf_switch_ctl2;
    _rf_ctrls.rxctl = rxctl;
    _rf_ctrls.txctl = txctl;
    _rf_ctrls.tcxo = tcxo;

    _dio4_pin = dio4;
    _dio5_pin = dio5;

    _radio_events = NULL;

    if (tcxo != NC) {
        _tcxo = 1;
    }

#ifdef MBED_CONF_RTOS_PRESENT
    irq_thread.start(mbed::callback(this, &SX1276_LoRaRadio::rf_irq_task));
#endif
}

/**
 * Destructor
 */
SX1276_LoRaRadio::~SX1276_LoRaRadio()
{

}

/*****************************************************************************
 * Public APIs                                                               *
 ****************************************************************************/
/**
 * Acquire lock
 */
void SX1276_LoRaRadio::lock(void)
{
    mutex.lock();
}

/**
 * Release lock
 */
void SX1276_LoRaRadio::unlock(void)
{
    mutex.unlock();
}

/**
 * Initializes radio module
 */
void SX1276_LoRaRadio::init_radio(radio_events_t *events)
{
    _radio_events = events;

    // Reset the radio transceiver
    radio_reset();

    // Setup radio variant type
    set_sx1276_variant_type();

    // setup SPI frequency
    // default is 8MHz although, configurable through
    // SPI_FREQUENCY macro
    setup_spi();

    // Calibrate radio receiver chain
    rx_chain_calibration();

    // set radio mode to sleep
    set_operation_mode(RF_OPMODE_SLEEP);

    // Setup radio registers to defaults
    setup_registers();

    // set modem type - defaults to FSK here
    set_modem(MODEM_FSK);

    // set state to be idle
    _rf_settings.state = RF_IDLE;

    // Setup interrupts on DIO pins
    setup_interrupts();
}

/**
 * Can be used by application/stack or the driver itself
 */
void SX1276_LoRaRadio::radio_reset()
{
    _reset_ctl.output();
    _reset_ctl = 0;
    wait_ms(2);
    _reset_ctl.input();
    wait_ms(6);
}

/**
 * TODO: The purpose of this API is unclear.
 *       Need to start an internal discussion.
 */
bool SX1276_LoRaRadio::check_rf_frequency(uint32_t frequency)
{
    // Implement check. Currently all frequencies are supported ? What band ?
    return true;
}

/**
 * Returns current status of the radio state machine
 */
uint8_t SX1276_LoRaRadio::get_status(void)
{
    return _rf_settings.state;
}

/**
 * Sets up carrier frequency
 */
void SX1276_LoRaRadio::set_channel(uint32_t freq)
{
    _rf_settings.channel = freq;
    freq = (uint32_t) ((float) freq / (float) FREQ_STEP);
    write_to_register(REG_FRFMSB, (uint8_t) ((freq >> 16) & 0xFF));
    write_to_register(REG_FRFMID, (uint8_t) ((freq >> 8) & 0xFF));
    write_to_register(REG_FRFLSB, (uint8_t) (freq & 0xFF));
}

/**
 * Generates 32 bit random number based upon RSSI monitoring
 * Used for various calculation by the stack for example dev nonce
 *
 * When this API is used modem is set in LoRa mode and all interrupts are
 * masked. If the user had been using FSK mode, it should be noted that a
 * change of mode is required again because the registers have changed.
 * In addition to that RX and TX configuration APIs should be called again in
 * order to have correct desires setup.
 */
uint32_t SX1276_LoRaRadio::random( void )
{
    uint8_t i;
    uint32_t rnd = 0;

    /*
     * Radio setup for random number generation
     */
    set_modem( MODEM_LORA );

    // Disable LoRa modem interrupts
    write_to_register(REG_LR_IRQFLAGSMASK, RFLR_IRQFLAGS_RXTIMEOUT |
                  RFLR_IRQFLAGS_RXDONE |
                  RFLR_IRQFLAGS_PAYLOADCRCERROR |
                  RFLR_IRQFLAGS_VALIDHEADER |
                  RFLR_IRQFLAGS_TXDONE |
                  RFLR_IRQFLAGS_CADDONE |
                  RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL |
                  RFLR_IRQFLAGS_CADDETECTED );

    // Set radio in continuous reception
    set_operation_mode(RF_OPMODE_RECEIVER);

    for (i = 0; i < 32; i++) {
        wait_ms(1);
        // Unfiltered RSSI value reading. Only takes the LSB value
        rnd |= ((uint32_t) read_register( REG_LR_RSSIWIDEBAND) & 0x01) << i;
    }

    sleep();

    return rnd;
}

/**
 * Sets up receiver related configurations
 *
 * Must be called before setting the radio in rx mode
 */
void SX1276_LoRaRadio::set_rx_config(radio_modems_t modem, uint32_t bandwidth,
                                     uint32_t datarate, uint8_t coderate,
                                     uint32_t bandwidth_afc,
                                     uint16_t preamble_len,
                                     uint16_t symb_timeout, bool fix_len,
                                     uint8_t payload_len, bool crc_on,
                                     bool freq_hop_on, uint8_t hop_period,
                                     bool iq_inverted, bool rx_continuous)
{
    set_modem(modem);

    switch (modem) {
        case MODEM_FSK:
            _rf_settings.fsk.bandwidth = bandwidth;
            _rf_settings.fsk.datarate = datarate;
            _rf_settings.fsk.bandwidth_afc = bandwidth_afc;
            _rf_settings.fsk.fix_len = fix_len;
            _rf_settings.fsk.payload_len = payload_len;
            _rf_settings.fsk.crc_on = crc_on;
            _rf_settings.fsk.iq_inverted = iq_inverted;
            _rf_settings.fsk.rx_continuous = rx_continuous;
            _rf_settings.fsk.preamble_len = preamble_len;
            _rf_settings.fsk.rx_single_timeout = (symb_timeout + 1) / 2; // dividing by 2 as our detector size is 2 symbols (16 bytes)

            datarate = (uint16_t) ((float) XTAL_FREQ / (float) datarate);
            write_to_register(REG_BITRATEMSB, (uint8_t) (datarate >> 8));
            write_to_register(REG_BITRATELSB, (uint8_t) (datarate & 0xFF));

            write_to_register(REG_RXBW, get_fsk_bw_reg_val(bandwidth));
            write_to_register(REG_AFCBW, get_fsk_bw_reg_val(bandwidth_afc));

            write_to_register(REG_PREAMBLEMSB, (uint8_t) ((preamble_len >> 8) & 0xFF));
            write_to_register(REG_PREAMBLELSB, (uint8_t) (preamble_len & 0xFF));

            if (fix_len == 1) {
                write_to_register(REG_PAYLOADLENGTH, payload_len);
            } else {
                write_to_register(REG_PAYLOADLENGTH, 0xFF); // Set payload length to the maximum
            }

            write_to_register(
                    REG_PACKETCONFIG1,
                    (read_register(REG_PACKETCONFIG1)
                            & RF_PACKETCONFIG1_CRC_MASK
                            & RF_PACKETCONFIG1_PACKETFORMAT_MASK)
                            | ((fix_len == 1) ?
                                    RF_PACKETCONFIG1_PACKETFORMAT_FIXED :
                                    RF_PACKETCONFIG1_PACKETFORMAT_VARIABLE)
                            | (crc_on << 4));

            // TODO why packet mode 2 ?
            write_to_register(REG_PACKETCONFIG2, (read_register(REG_PACKETCONFIG2)
                                            | RF_PACKETCONFIG2_DATAMODE_PACKET));

            break;

        case MODEM_LORA:

            if (bandwidth > 2) {
                // Fatal error: When using LoRa modem only bandwidths 125, 250 and 500 kHz are supported
                while (1)
                    ;
                // TODO Return a proper error from here
            }

            // stupid hack. TODO think something better
            bandwidth+=7;

            _rf_settings.lora.bandwidth = bandwidth;
            _rf_settings.lora.datarate = datarate;
            _rf_settings.lora.coderate = coderate;
            _rf_settings.lora.preamble_len = preamble_len;
            _rf_settings.lora.fix_len = fix_len;
            _rf_settings.lora.payload_len = payload_len;
            _rf_settings.lora.crc_on = crc_on;
            _rf_settings.lora.freq_hop_on = freq_hop_on;
            _rf_settings.lora.hop_period = hop_period;
            _rf_settings.lora.iq_inverted = iq_inverted;
            _rf_settings.lora.rx_continuous = rx_continuous;

            if (datarate > 12) {
                datarate = 12;
            } else if (datarate < 6) {
                datarate = 6;
            }

            if (((bandwidth == 7) && ((datarate == 11) || (datarate == 12)))
                    || ((bandwidth == 8) && (datarate == 12))) {
                _rf_settings.lora.low_datarate_optimize = 0x01;
            } else {
                _rf_settings.lora.low_datarate_optimize = 0x00;
            }

            write_to_register(REG_LR_MODEMCONFIG1, (read_register( REG_LR_MODEMCONFIG1)
                            & RFLR_MODEMCONFIG1_BW_MASK
                            & RFLR_MODEMCONFIG1_CODINGRATE_MASK
                            & RFLR_MODEMCONFIG1_IMPLICITHEADER_MASK)
                            | (bandwidth << 4)
                            | (coderate << 1) | fix_len);

            write_to_register(REG_LR_MODEMCONFIG2, (read_register( REG_LR_MODEMCONFIG2)
                    & RFLR_MODEMCONFIG2_SF_MASK
                    & RFLR_MODEMCONFIG2_RXPAYLOADCRC_MASK
                    & RFLR_MODEMCONFIG2_SYMBTIMEOUTMSB_MASK)
                            | (datarate << 4)
                            | (crc_on << 2)
                            | ((symb_timeout >> 8)
                                    & ~RFLR_MODEMCONFIG2_SYMBTIMEOUTMSB_MASK));

            write_to_register(REG_LR_MODEMCONFIG3, (read_register( REG_LR_MODEMCONFIG3)
                    & RFLR_MODEMCONFIG3_LOWDATARATEOPTIMIZE_MASK)
                            | (_rf_settings.lora.low_datarate_optimize << 3));

            write_to_register(REG_LR_SYMBTIMEOUTLSB, (uint8_t) (symb_timeout & 0xFF));

            write_to_register(REG_LR_PREAMBLEMSB, (uint8_t) ((preamble_len >> 8) & 0xFF));
            write_to_register(REG_LR_PREAMBLELSB, (uint8_t) (preamble_len & 0xFF));

            if (fix_len == 1) {
                write_to_register(REG_LR_PAYLOADLENGTH, payload_len);
            }

            if (_rf_settings.lora.freq_hop_on == true) {
                write_to_register(REG_LR_PLLHOP, (read_register( REG_LR_PLLHOP)
                                & RFLR_PLLHOP_FASTHOP_MASK)
                                | RFLR_PLLHOP_FASTHOP_ON);
                write_to_register(REG_LR_HOPPERIOD,_rf_settings.lora.hop_period);
            }

            if ((bandwidth == 9) && (_rf_settings.channel > RF_MID_BAND_THRESH)) {
                // ERRATA 2.1 - Sensitivity Optimization with a 500 kHz Bandwidth
                write_to_register(REG_LR_TEST36, 0x02);
                write_to_register(REG_LR_TEST3A, 0x64);
            } else if (bandwidth == 9) {
                // ERRATA 2.1 - Sensitivity Optimization with a 500 kHz Bandwidth
                write_to_register(REG_LR_TEST36, 0x02);
                write_to_register(REG_LR_TEST3A, 0x7F);
            } else {
                // ERRATA 2.1 - Sensitivity Optimization with a 500 kHz Bandwidth
                write_to_register(REG_LR_TEST36, 0x03);
            }

            if (datarate == 6) {
                write_to_register(REG_LR_DETECTOPTIMIZE, (read_register(REG_LR_DETECTOPTIMIZE)
                                          & RFLR_DETECTIONOPTIMIZE_MASK)
                                          | RFLR_DETECTIONOPTIMIZE_SF6);
                write_to_register(REG_LR_DETECTIONTHRESHOLD, RFLR_DETECTIONTHRESH_SF6);
            } else {
                write_to_register(REG_LR_DETECTOPTIMIZE, (read_register(REG_LR_DETECTOPTIMIZE)
                                          & RFLR_DETECTIONOPTIMIZE_MASK)
                                          | RFLR_DETECTIONOPTIMIZE_SF7_TO_SF12);
                write_to_register(REG_LR_DETECTIONTHRESHOLD, RFLR_DETECTIONTHRESH_SF7_TO_SF12);
            }
            break;

        default:
            break;
    }
}

/**
 * Sets up transmitter related configuration
 *
 * Must be called before putting the radio module in Tx mode or trying
 * to send
 */
void SX1276_LoRaRadio::set_tx_config(radio_modems_t modem, int8_t power,
                                     uint32_t fdev, uint32_t bandwidth,
                                     uint32_t datarate, uint8_t coderate,
                                     uint16_t preamble_len, bool fix_len,
                                     bool crc_on, bool freq_hop_on,
                                     uint8_t hop_period, bool iq_inverted,
                                     uint32_t timeout)
{
    set_modem(modem);
    set_rf_tx_power(power);

    switch (modem) {
        case MODEM_FSK:
            _rf_settings.fsk.power = power;
            _rf_settings.fsk.f_dev = fdev;
            _rf_settings.fsk.bandwidth = bandwidth;
            _rf_settings.fsk.datarate = datarate;
            _rf_settings.fsk.preamble_len = preamble_len;
            _rf_settings.fsk.fix_len = fix_len;
            _rf_settings.fsk.crc_on = crc_on;
            _rf_settings.fsk.iq_inverted = iq_inverted;
            _rf_settings.fsk.tx_timeout = timeout;

            fdev = (uint16_t) ((float) fdev / (float) FREQ_STEP);
            write_to_register( REG_FDEVMSB, (uint8_t) (fdev >> 8));
            write_to_register( REG_FDEVLSB, (uint8_t) (fdev & 0xFF));

            datarate = (uint16_t) ((float) XTAL_FREQ / (float) datarate);
            write_to_register( REG_BITRATEMSB, (uint8_t) (datarate >> 8));
            write_to_register( REG_BITRATELSB, (uint8_t) (datarate & 0xFF));

            write_to_register( REG_PREAMBLEMSB, (preamble_len >> 8) & 0x00FF);
            write_to_register( REG_PREAMBLELSB, preamble_len & 0xFF);

            write_to_register(REG_PACKETCONFIG1,
                    (read_register( REG_PACKETCONFIG1) &
                    RF_PACKETCONFIG1_CRC_MASK &
                    RF_PACKETCONFIG1_PACKETFORMAT_MASK)
                            | ((fix_len == 1) ?
                                    RF_PACKETCONFIG1_PACKETFORMAT_FIXED :
                                    RF_PACKETCONFIG1_PACKETFORMAT_VARIABLE)
                            | (crc_on << 4));
            write_to_register(REG_PACKETCONFIG2,
                    (read_register( REG_PACKETCONFIG2)
                            | RF_PACKETCONFIG2_DATAMODE_PACKET));

            break;

        case MODEM_LORA:
            _rf_settings.lora.power = power;
            // Fatal error: When using LoRa modem only bandwidths 125, 250 and 500 kHz are supported
            MBED_ASSERT(bandwidth <= 2);
            bandwidth += 7;
            _rf_settings.lora.bandwidth = bandwidth;
            _rf_settings.lora.datarate = datarate;
            _rf_settings.lora.coderate = coderate;
            _rf_settings.lora.preamble_len = preamble_len;
            _rf_settings.lora.fix_len = fix_len;
            _rf_settings.lora.freq_hop_on = freq_hop_on;
            _rf_settings.lora.hop_period = hop_period;
            _rf_settings.lora.crc_on = crc_on;
            _rf_settings.lora.iq_inverted = iq_inverted;
            _rf_settings.lora.tx_timeout = timeout;

            if (datarate > 12) {
                datarate = 12;
            } else if (datarate < 6) {
                datarate = 6;
            }
            if (((bandwidth == 7) && ((datarate == 11) || (datarate == 12)))
                    || ((bandwidth == 8) && (datarate == 12))) {
                _rf_settings.lora.low_datarate_optimize = 0x01;
            } else {
                _rf_settings.lora.low_datarate_optimize = 0x00;
            }

            if (_rf_settings.lora.freq_hop_on == true) {
                write_to_register(REG_LR_PLLHOP, (read_register(REG_LR_PLLHOP)
                                & RFLR_PLLHOP_FASTHOP_MASK)
                                | RFLR_PLLHOP_FASTHOP_ON);
                write_to_register( REG_LR_HOPPERIOD, _rf_settings.lora.hop_period);
            }

            write_to_register(REG_LR_MODEMCONFIG1, (read_register( REG_LR_MODEMCONFIG1)
                    & RFLR_MODEMCONFIG1_BW_MASK
                    & RFLR_MODEMCONFIG1_CODINGRATE_MASK
                    & RFLR_MODEMCONFIG1_IMPLICITHEADER_MASK) | (bandwidth << 4)
                                                | (coderate << 1) | fix_len);

            write_to_register(REG_LR_MODEMCONFIG2, (read_register( REG_LR_MODEMCONFIG2)
                    & RFLR_MODEMCONFIG2_SF_MASK
                    & RFLR_MODEMCONFIG2_RXPAYLOADCRC_MASK)
                              | (datarate << 4)
                              | (crc_on << 2));

            write_to_register(REG_LR_MODEMCONFIG3, (read_register(REG_LR_MODEMCONFIG3)
                    & RFLR_MODEMCONFIG3_LOWDATARATEOPTIMIZE_MASK)
                              | (_rf_settings.lora.low_datarate_optimize << 3));

            write_to_register(REG_LR_PREAMBLEMSB, (preamble_len >> 8) & 0x00FF);
            write_to_register(REG_LR_PREAMBLELSB, preamble_len & 0xFF);

            if (datarate == 6) {
                write_to_register(REG_LR_DETECTOPTIMIZE, (read_register( REG_LR_DETECTOPTIMIZE)
                        & RFLR_DETECTIONOPTIMIZE_MASK) | RFLR_DETECTIONOPTIMIZE_SF6);
                write_to_register(REG_LR_DETECTIONTHRESHOLD, RFLR_DETECTIONTHRESH_SF6);
            } else {
                write_to_register(REG_LR_DETECTOPTIMIZE, (read_register( REG_LR_DETECTOPTIMIZE)
                        & RFLR_DETECTIONOPTIMIZE_MASK) | RFLR_DETECTIONOPTIMIZE_SF7_TO_SF12);
                write_to_register( REG_LR_DETECTIONTHRESHOLD, RFLR_DETECTIONTHRESH_SF7_TO_SF12);
            }

            break;
    }
}

/**
 * Calculates time on Air i.e., dwell time for a single packet
 *
 * Crucial for the stack in order to calculate dwell time so as to control
 * duty cycling.
 */
uint32_t SX1276_LoRaRadio::time_on_air(radio_modems_t modem, uint8_t pkt_len)
{
    uint32_t airTime = 0;

    switch (modem) {
        case MODEM_FSK:
            airTime =
                    rint((8 * (_rf_settings.fsk.preamble_len
                                    + ((read_register( REG_SYNCCONFIG)
                                            & ~RF_SYNCCONFIG_SYNCSIZE_MASK) + 1)
                                    + ((_rf_settings.fsk.fix_len == 0x01) ?
                                            0.0f : 1.0f)
                                    + (((read_register( REG_PACKETCONFIG1)
                                            & ~RF_PACKETCONFIG1_ADDRSFILTERING_MASK)
                                            != 0x00) ? 1.0f : 0) + pkt_len
                                    + ((_rf_settings.fsk.crc_on == 0x01) ?
                                            2.0 : 0))
                            / _rf_settings.fsk.datarate) * 1000);

            break;
        case MODEM_LORA:
            float bw = 0.0f;
            // REMARK: When using LoRa modem only bandwidths 125, 250 and 500 kHz are supported
            switch (_rf_settings.lora.bandwidth) {
                //case 0: // 7.8 kHz
                //    bw = 78e2;
                //    break;
                //case 1: // 10.4 kHz
                //    bw = 104e2;
                //    break;
                //case 2: // 15.6 kHz
                //    bw = 156e2;
                //    break;
                //case 3: // 20.8 kHz
                //    bw = 208e2;
                //    break;
                //case 4: // 31.2 kHz
                //    bw = 312e2;
                //    break;
                //case 5: // 41.4 kHz
                //    bw = 414e2;
                //    break;
                //case 6: // 62.5 kHz
                //    bw = 625e2;
                //    break;
                case 7: // 125 kHz
                    bw = 125000;
                    break;
                case 8: // 250 kHz
                    bw = 250000;
                    break;
                case 9: // 500 kHz
                    bw = 500000;
                    break;
            }

            // Symbol rate : time for one symbol (secs)
            float rs = bw / (1 << _rf_settings.lora.datarate);
            float ts = 1 / rs;
            // time of preamble
            float tPreamble = (_rf_settings.lora.preamble_len + 4.25f) * ts;
            // Symbol length of payload and time
            float tmp = ceil((8 * pkt_len - 4 * _rf_settings.lora.datarate + 28
                            + 16 * _rf_settings.lora.crc_on
                            - (_rf_settings.lora.fix_len ? 20 : 0))
                            / (float) (4
                                    * (_rf_settings.lora.datarate
                                            - ((_rf_settings.lora.low_datarate_optimize > 0)
                                                    ? 2 : 0))))
                            * (_rf_settings.lora.coderate + 4);
            float nPayload = 8 + ((tmp > 0) ? tmp : 0);
            float tPayload = nPayload * ts;
            // Time on air
            float tOnAir = tPreamble + tPayload;
            // return ms secs
            airTime = floor(tOnAir * 1000 + 0.999f);

            break;
    }

    return airTime;
}

/**
 * Prepares and sends the radio packet out in the air
 */
void SX1276_LoRaRadio::send(uint8_t *buffer, uint8_t size)
{
    uint32_t tx_timeout = 0;

    switch (_rf_settings.modem) {
        case MODEM_FSK:
            _rf_settings.fsk_packet_handler.nb_bytes = 0;
            _rf_settings.fsk_packet_handler.size = size;

            if (_rf_settings.fsk.fix_len == false) {
                write_fifo((uint8_t*) &size, 1);
            } else {
                write_to_register(REG_PAYLOADLENGTH, size);
            }

            if ((size > 0) && (size <= 64)) {
                _rf_settings.fsk_packet_handler.chunk_size = size;
            } else {
                memcpy(_data_buffer, buffer, size);
                _rf_settings.fsk_packet_handler.chunk_size = 32;
            }

            // Write payload buffer
            write_fifo(buffer, _rf_settings.fsk_packet_handler.chunk_size);
            _rf_settings.fsk_packet_handler.nb_bytes +=
                    _rf_settings.fsk_packet_handler.chunk_size;
            tx_timeout = _rf_settings.fsk.tx_timeout;

            break;

        case MODEM_LORA:
            if (_rf_settings.lora.iq_inverted == true) {
                write_to_register(REG_LR_INVERTIQ, ((read_register(REG_LR_INVERTIQ)
                                & RFLR_INVERTIQ_TX_MASK
                                & RFLR_INVERTIQ_RX_MASK)
                                    | RFLR_INVERTIQ_RX_OFF
                                    | RFLR_INVERTIQ_TX_ON));
                write_to_register( REG_LR_INVERTIQ2, RFLR_INVERTIQ2_ON);
            } else {
                write_to_register(REG_LR_INVERTIQ, ((read_register( REG_LR_INVERTIQ)
                                & RFLR_INVERTIQ_TX_MASK
                                & RFLR_INVERTIQ_RX_MASK)
                                    | RFLR_INVERTIQ_RX_OFF
                                    | RFLR_INVERTIQ_TX_OFF));
                write_to_register( REG_LR_INVERTIQ2, RFLR_INVERTIQ2_OFF);
            }

            _rf_settings.lora_packet_handler.size = size;

            // Initializes the payload size
            write_to_register(REG_LR_PAYLOADLENGTH, size);

            // Full buffer used for Tx
            write_to_register(REG_LR_FIFOTXBASEADDR, 0);
            write_to_register(REG_LR_FIFOADDRPTR, 0);

            // FIFO operations can not take place in Sleep mode
            if ((read_register( REG_OPMODE) & ~RF_OPMODE_MASK) == RF_OPMODE_SLEEP) {
                standby();
                wait_ms(1);
            }
            // write_to_register payload buffer
            write_fifo(buffer, size);
            tx_timeout = _rf_settings.lora.tx_timeout;

            break;
    }

    transmit(tx_timeout);
}

/**
 * sets the radio module to sleep
 */

void SX1276_LoRaRadio::sleep()
{
    // stop timers
    tx_timeout_timer.detach();

    // put module in sleep mode
    set_operation_mode(RF_OPMODE_SLEEP);
}

/**
 * Put radio in Standby mode
 */
void SX1276_LoRaRadio::standby( void )
{
    tx_timeout_timer.detach();

    set_operation_mode(RF_OPMODE_STANDBY);
    _rf_settings.state = RF_IDLE;
}

/**
 * Sets the radio module in receive mode
 *
 * A DIO4 interrupt let's the state machine know that a preamble is detected
 * and finally a DIO0 interrupt let's the state machine know that a packet is
 * ready to be read from the FIFO
 */
void SX1276_LoRaRadio::receive(void)
{
    switch (_rf_settings.modem) {
        case MODEM_FSK:

            // DIO0=PayloadReady/PacketSent
            // DIO1=FifoLevel
            // DIO2=RxTimeout
            // DIO3=FifoEmpty?
            // DIO4=PreambleDetect
            // DIO5=ModeReady?
            write_to_register(REG_DIOMAPPING1, (read_register( REG_DIOMAPPING1)
                    & RF_DIOMAPPING1_DIO0_MASK
                    & RF_DIOMAPPING1_DIO1_MASK
                    & RF_DIOMAPPING1_DIO2_MASK)
                              | RF_DIOMAPPING1_DIO0_00
                              | RF_DIOMAPPING1_DIO1_00
                              | RF_DIOMAPPING1_DIO2_10);

            write_to_register(REG_DIOMAPPING2, (read_register( REG_DIOMAPPING2)
                    & RF_DIOMAPPING2_DIO4_MASK
                    & RF_DIOMAPPING2_MAP_MASK)
                              | RF_DIOMAPPING2_DIO4_11
                              | RF_DIOMAPPING2_MAP_PREAMBLEDETECT);

            _rf_settings.fsk_packet_handler.fifo_thresh =
                    read_register(REG_FIFOTHRESH) & 0x3F;

            write_to_register(REG_RXCONFIG, RF_RXCONFIG_AFCAUTO_ON
                              | RF_RXCONFIG_AGCAUTO_ON
                              | RF_RXCONFIG_RXTRIGER_PREAMBLEDETECT);

            if (!_rf_settings.fsk.rx_continuous) {
                // the value for rx timeout in symbols cannot be more than 255
                // as the preamble length is fixed. We assert here for quick
                // diagnostics
                MBED_ASSERT(_rf_settings.fsk.rx_single_timeout <= 255);
                write_to_register(REG_RXTIMEOUT2, _rf_settings.fsk.rx_single_timeout);
                write_to_register(REG_RXTIMEOUT3, 0x00);
                write_to_register(REG_RXTIMEOUT1, 0x00);
            }

            _rf_settings.fsk_packet_handler.preamble_detected = 0;
            _rf_settings.fsk_packet_handler.sync_word_detected = 0;
            _rf_settings.fsk_packet_handler.nb_bytes = 0;
            _rf_settings.fsk_packet_handler.size = 0;

            break;

        case MODEM_LORA:

            if (_rf_settings.lora.iq_inverted == true) {
                write_to_register(REG_LR_INVERTIQ, ((read_register( REG_LR_INVERTIQ)
                                & RFLR_INVERTIQ_TX_MASK & RFLR_INVERTIQ_RX_MASK)
                                | RFLR_INVERTIQ_RX_ON | RFLR_INVERTIQ_TX_OFF));
                write_to_register( REG_LR_INVERTIQ2, RFLR_INVERTIQ2_ON);
            } else {
                write_to_register(REG_LR_INVERTIQ, ((read_register( REG_LR_INVERTIQ)
                                & RFLR_INVERTIQ_TX_MASK & RFLR_INVERTIQ_RX_MASK)
                                | RFLR_INVERTIQ_RX_OFF | RFLR_INVERTIQ_TX_OFF));
                write_to_register( REG_LR_INVERTIQ2, RFLR_INVERTIQ2_OFF);
            }

            // ERRATA 2.3 - Receiver Spurious Reception of a LoRa Signal
            if (_rf_settings.lora.bandwidth < 9) {
                write_to_register(REG_LR_DETECTOPTIMIZE,
                                  read_register(REG_LR_DETECTOPTIMIZE) & 0x7F);
                write_to_register(REG_LR_TEST30, 0x00);
                switch (_rf_settings.lora.bandwidth) {
                    case 0: // 7.8 kHz
                        write_to_register( REG_LR_TEST2F, 0x48);
                        set_channel(_rf_settings.channel + 7.81e3);
                        break;
                    case 1: // 10.4 kHz
                        write_to_register( REG_LR_TEST2F, 0x44);
                        set_channel(_rf_settings.channel + 10.42e3);
                        break;
                    case 2: // 15.6 kHz
                        write_to_register( REG_LR_TEST2F, 0x44);
                        set_channel(_rf_settings.channel + 15.62e3);
                        break;
                    case 3: // 20.8 kHz
                        write_to_register( REG_LR_TEST2F, 0x44);
                        set_channel(_rf_settings.channel + 20.83e3);
                        break;
                    case 4: // 31.2 kHz
                        write_to_register( REG_LR_TEST2F, 0x44);
                        set_channel(_rf_settings.channel + 31.25e3);
                        break;
                    case 5: // 41.4 kHz
                        write_to_register( REG_LR_TEST2F, 0x44);
                        set_channel(_rf_settings.channel + 41.67e3);
                        break;
                    case 6: // 62.5 kHz
                        write_to_register( REG_LR_TEST2F, 0x40);
                        break;
                    case 7: // 125 kHz
                        write_to_register( REG_LR_TEST2F, 0x40);
                        break;
                    case 8: // 250 kHz
                        write_to_register( REG_LR_TEST2F, 0x40);
                        break;
                }
            } else {
                write_to_register( REG_LR_DETECTOPTIMIZE,
                                  read_register( REG_LR_DETECTOPTIMIZE) | 0x80);
            }

            if (_rf_settings.lora.freq_hop_on == true) {
                write_to_register(REG_LR_IRQFLAGSMASK, RFLR_IRQFLAGS_VALIDHEADER
                                  | RFLR_IRQFLAGS_TXDONE
                                  | RFLR_IRQFLAGS_CADDONE
                                  | RFLR_IRQFLAGS_CADDETECTED);

                // DIO0=RxDone, DIO2=FhssChangeChannel
                write_to_register(REG_DIOMAPPING1, (read_register(REG_DIOMAPPING1)
                                & RFLR_DIOMAPPING1_DIO0_MASK
                                & RFLR_DIOMAPPING1_DIO2_MASK)
                                | RFLR_DIOMAPPING1_DIO0_00
                                | RFLR_DIOMAPPING1_DIO2_00);
            } else {
                write_to_register(REG_LR_IRQFLAGSMASK, RFLR_IRQFLAGS_VALIDHEADER
                                   | RFLR_IRQFLAGS_TXDONE
                                   | RFLR_IRQFLAGS_CADDONE
                                   | RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL
                                   | RFLR_IRQFLAGS_CADDETECTED);

                // DIO0=RxDone
                write_to_register(REG_DIOMAPPING1, (read_register( REG_DIOMAPPING1)
                                & RFLR_DIOMAPPING1_DIO0_MASK)
                                  | RFLR_DIOMAPPING1_DIO0_00);
            }
            write_to_register(REG_LR_FIFORXBASEADDR, 0);
            write_to_register(REG_LR_FIFOADDRPTR, 0);

            break;
    }

    memset(_data_buffer, 0, (size_t) MAX_DATA_BUFFER_SIZE_SX1276);

    _rf_settings.state = RF_RX_RUNNING;

    if (_rf_settings.modem == MODEM_FSK) {
        set_operation_mode(RF_OPMODE_RECEIVER);
        return;
    }

    // If mode is LoRa set mode
    if (_rf_settings.lora.rx_continuous == true) {
        set_operation_mode(RFLR_OPMODE_RECEIVER);
    } else {
        set_operation_mode(RFLR_OPMODE_RECEIVER_SINGLE);
    }
}


/**
 * Perform carrier sensing
 *
 * Checks for a certain time if the RSSI is above a given threshold.
 * This threshold determines if there is already a transmission going on
 * in the channel or not.
 *
 */
bool SX1276_LoRaRadio::perform_carrier_sense(radio_modems_t modem,
                                   uint32_t freq,
                                   int16_t rssi_threshold,
                                   uint32_t max_carrier_sense_time)
{
    bool status = true;
    int16_t rssi = 0;

    set_modem(modem);
    set_channel(freq);
    set_operation_mode(RF_OPMODE_RECEIVER);

    // hold on a bit, radio turn-around time
    wait_ms(1);

    Timer elapsed_time;
    elapsed_time.start();

    // Perform carrier sense for maxCarrierSenseTime
    while (elapsed_time.read_ms() < (int)max_carrier_sense_time) {
        rssi = get_rssi(modem);

        if (rssi > rssi_threshold) {
            status = false;
            break;
        }
    }

    sleep();
    return status;
}

/**
 * TODO: Making sure if this API is valid only for LoRa modulation ?
 *
 * Indicates if the node is part of a private or public network
 */
void SX1276_LoRaRadio::set_public_network(bool enable)
{
    set_modem(MODEM_LORA);

    _rf_settings.lora.public_network = enable;
    if (enable == true) {
        // Change lora modem SyncWord
        write_to_register(REG_LR_SYNCWORD, LORA_MAC_PUBLIC_SYNCWORD);
    } else {
        // Change lora modem SyncWord
        write_to_register(REG_LR_SYNCWORD, LORA_MAC_PRIVATE_SYNCWORD);
    }
}

/**
 * Puts a limit on the size of payload the module can handle
 * By default it is MAX, i.e., 256 bytes
 */
void SX1276_LoRaRadio::set_max_payload_length(radio_modems_t modem, uint8_t max)
{
    set_modem(modem);

    switch (modem) {
        case MODEM_FSK:
            if (_rf_settings.fsk.fix_len == false) {
                write_to_register(REG_PAYLOADLENGTH, max);
            }
            break;
        case MODEM_LORA:
            write_to_register(REG_LR_PAYLOADMAXLENGTH, max);
            break;
    }
}

/**
 * Channel Activity detection (can be done only in LoRa mode)
 *
 * If any activity on the channel is detected, an interrupt is asserted on
 * DIO3. A callback will be generated to the stack/application upon the
 * assertion of DIO3.
 */
void SX1276_LoRaRadio::start_cad()
{
    uint8_t reg_val;

    switch (_rf_settings.modem) {
        case MODEM_FSK:
            break;
        case MODEM_LORA:
            write_to_register(REG_LR_IRQFLAGSMASK,
                              RFLR_IRQFLAGS_RXTIMEOUT |
                              RFLR_IRQFLAGS_RXDONE |
                              RFLR_IRQFLAGS_PAYLOADCRCERROR |
                              RFLR_IRQFLAGS_VALIDHEADER |
                              RFLR_IRQFLAGS_TXDONE |
                              RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL);

            // DIO3=CADDone
            reg_val = read_register(REG_DIOMAPPING1);
            write_to_register(REG_DIOMAPPING1, (reg_val &
                              RFLR_DIOMAPPING1_DIO3_MASK) |
                              RFLR_DIOMAPPING1_DIO3_00);

            set_operation_mode(RFLR_OPMODE_CAD);

            _rf_settings.state = RF_CAD;

            break;
        default:
            break;
    }
}

/**
 * Set transmission in continuous wave mode
 */
void SX1276_LoRaRadio::set_tx_continuous_wave(uint32_t freq, int8_t power,
                                              uint16_t time)
{
    uint8_t reg_val;

    set_channel(freq);
    set_tx_config(MODEM_FSK, power, 0, 0, 4800, 0, 5, false, false, 0, 0, 0, time * 1000);
    reg_val = read_register(REG_PACKETCONFIG2);

    write_to_register( REG_PACKETCONFIG2, (reg_val & RF_PACKETCONFIG2_DATAMODE_MASK ) );
    // Disable radio interrupts
    write_to_register( REG_DIOMAPPING1, RF_DIOMAPPING1_DIO0_11 | RF_DIOMAPPING1_DIO1_11 );
    write_to_register( REG_DIOMAPPING2, RF_DIOMAPPING2_DIO4_10 | RF_DIOMAPPING2_DIO5_10 );

    _rf_settings.state = RF_TX_RUNNING;
    tx_timeout_timer.attach_us(callback(this, &SX1276_LoRaRadio::timeout_irq_isr), time * 1000000);
    set_operation_mode(RF_OPMODE_TRANSMITTER);
}

/*****************************************************************************
 * Private APIs                                                              *
 ****************************************************************************/
#ifdef MBED_CONF_RTOS_PRESENT
/**
 * Thread task handling IRQs
 */
void SX1276_LoRaRadio::rf_irq_task(void)
{
    for (;;) {
        uint32_t flags = ThisThread::flags_wait_any(0x7FFFFFFF);

        lock();
        if (flags & SIG_DIO0) {
            handle_dio0_irq();
        }
        if (flags & SIG_DIO1) {
            handle_dio1_irq();
        }
        if (flags & SIG_DIO2) {
            handle_dio2_irq();
        }
        if (flags & SIG_DIO3) {
            handle_dio3_irq();
        }
        if (flags & SIG_DIO4) {
            handle_dio4_irq();
        }
        if (flags & SIG_DIO5) {
            handle_dio5_irq();
        }
        if (flags & SIG_TIMOUT) {
            handle_timeout_irq();
        }
        unlock();
    }
}
#endif

/**
 * Writes a single byte to a given register
 */
void SX1276_LoRaRadio::write_to_register(uint8_t addr, uint8_t data)
{
    write_to_register(addr, &data, 1);
}

/**
 * Writes multiple bytes to a given register
 */
void SX1276_LoRaRadio::write_to_register(uint8_t addr, uint8_t *data, uint8_t size)
{
    // set chip-select low
    _chip_select = 0;

    // set write command
    _spi.write(addr | SPI_WRITE_CMD);

    // write data
    for (uint8_t i = 0; i < size; i++) {
        _spi.write(data[i]);
    }

    // set chip-select high
    _chip_select = 1;
}

/**
 * Reads the value of a single register
 */
uint8_t SX1276_LoRaRadio::read_register(uint8_t addr)
{
    uint8_t data;
    read_register(addr, &data, 1);
    return data;
}

/**
 * Reads multiple values from a given register
 */
void SX1276_LoRaRadio::read_register(uint8_t addr, uint8_t *buffer, uint8_t size)
{
    // set chip-select low
    _chip_select = 0;

    // set read command
    _spi.write(addr & SPI_READ_CMD);

    // read buffers
    for (uint8_t i = 0; i < size; i++) {
        buffer[i] = _spi.write(0);
    }

    // set chip-select high
    _chip_select = 1;
}

/**
 * Writes to FIIO provided by the chip
 */
void SX1276_LoRaRadio::write_fifo(uint8_t *buffer, uint8_t size)
{
    write_to_register(0, buffer, size);
}

/**
 * Reads from the FIFO provided by the chip
 */
void SX1276_LoRaRadio::read_fifo(uint8_t *buffer, uint8_t size)
{
    read_register(0, buffer, size);
}

/**
 * Sets up operation mode
 */
void SX1276_LoRaRadio::set_operation_mode(uint8_t mode)
{
    if (mode == RF_OPMODE_SLEEP) {
        set_low_power_mode();
    } else {
        set_low_power_mode();
        set_antenna_switch(mode);
    }

    write_to_register(REG_OPMODE, (read_register(REG_OPMODE) & RF_OPMODE_MASK) | mode);
}

/**
 * Sets the modem type to use
 *
 * At initialization FSK is chosen. Later stack or application
 * can choose to change.
 */
void SX1276_LoRaRadio::set_modem(uint8_t modem )
{
    if ((read_register(REG_OPMODE) & RFLR_OPMODE_LONGRANGEMODE_ON) != 0 ) {
        _rf_settings.modem = MODEM_LORA;
    } else {
        _rf_settings.modem = MODEM_FSK;
    }

    if(_rf_settings.modem == modem ) {
        // if the modem is already set
        return;
    }

    _rf_settings.modem = modem;

    switch(_rf_settings.modem)
    {
    default:
    case MODEM_FSK:
        // before changing modem mode, put the module to sleep
        sleep();
        write_to_register(REG_OPMODE, (read_register(REG_OPMODE) & RFLR_OPMODE_LONGRANGEMODE_MASK)
                                      | RFLR_OPMODE_LONGRANGEMODE_OFF);

        // Datasheet Tables 28, 29 DIO mapping
        write_to_register(REG_DIOMAPPING1, 0x00); // sets DIO0-DI03 in default mode
        write_to_register(REG_DIOMAPPING2, 0x30); // bits 4-5 are turned on i.e.,
                                                  //  DIO5 and DIO4=ModeReady
        break;
    case MODEM_LORA:
        sleep();
        write_to_register(REG_OPMODE, (read_register(REG_OPMODE) & RFLR_OPMODE_LONGRANGEMODE_MASK)
                                      | RFLR_OPMODE_LONGRANGEMODE_ON);

        // Datasheet Tables 17 DIO mapping for LoRa
        // set to defaults
        write_to_register(REG_DIOMAPPING1, 0x00); // DIO0 - DIO3 defaults
        write_to_register(REG_DIOMAPPING2, 0x00); // DIO4 - DIO5 defaults

        break;
    }
}

/**
 * Set the radio module variant
 */
void SX1276_LoRaRadio::set_sx1276_variant_type()
{
    if (_rf_ctrls.ant_switch != NC) {
        _ant_switch.input();
        wait_ms(1);
        if (_ant_switch == 1) {
            radio_variant = SX1276MB1LAS;
        } else {
            radio_variant = SX1276MB1MAS;
        }
        _ant_switch.output();
        wait_ms(1);
    } else {
        radio_variant = MBED_CONF_SX1276_LORA_DRIVER_RADIO_VARIANT;
    }
}

/**
 * Sets up frequency for SPI module
 * Reference DataSheet: 4.3 SPI Interface
 */
void SX1276_LoRaRadio::setup_spi()
{
    // SPI bus frequency
    uint32_t spi_freq = SPI_FREQUENCY;

    // Hold chip-select high
    _chip_select = 1;
    _spi.format(8, 0);

#if defined (TARGET_KL25Z)
    //bus-clock frequency is halved -> double the SPI frequency to compensate
    _spi.frequency(spi_freq * 2);
#else
    // otherwise use default SPI frequency which is 8 MHz
    _spi.frequency(spi_freq);
#endif
    // 100 us wait to settle down
    wait(0.1);
}

/**
 * Sets the radio registers to defaults
 */
void SX1276_LoRaRadio::setup_registers()
{
    for (unsigned int i = 0; i < sizeof(radio_reg_init) / sizeof(radio_registers_t); i++) {
        set_modem(radio_reg_init[i].modem);
        write_to_register(radio_reg_init[i].addr, radio_reg_init[i].value);
    }
}

/**
 * Performs the Rx chain calibration for LF and HF bands
 *
 * Must be called just after the reset so all registers are at their
 * default values.
 */
void SX1276_LoRaRadio::rx_chain_calibration(void)
{
    uint8_t regPaConfigInitVal;
    uint32_t initialFreq;

    // Save context
    regPaConfigInitVal = read_register( REG_PACONFIG );
    initialFreq = (float) (((uint32_t) this->read_register(REG_FRFMSB) << 16) |
                            ((uint32_t) this->read_register(REG_FRFMID) << 8 ) |
                            ((uint32_t)this->read_register(REG_FRFLSB))) * (float) FREQ_STEP;

    // Cut the PA just in case, RFO output, power = -1 dBm
    write_to_register(REG_PACONFIG, 0x00);

    // Launch Rx chain calibration for LF band
    write_to_register (REG_IMAGECAL, (read_register(REG_IMAGECAL)
                                       & RF_IMAGECAL_IMAGECAL_MASK)
                                       | RF_IMAGECAL_IMAGECAL_START);
    while((read_register(REG_IMAGECAL) & RF_IMAGECAL_IMAGECAL_RUNNING )
                                      == RF_IMAGECAL_IMAGECAL_RUNNING )
    {
    }

    // Sets a Frequency in HF band
    set_channel(868000000);

    // Launch Rx chain calibration for HF band
    write_to_register (REG_IMAGECAL, (read_register(REG_IMAGECAL)
                                      & RF_IMAGECAL_IMAGECAL_MASK )
                                      | RF_IMAGECAL_IMAGECAL_START );
    while((read_register(REG_IMAGECAL) & RF_IMAGECAL_IMAGECAL_RUNNING )
                                      == RF_IMAGECAL_IMAGECAL_RUNNING )
    {
        // do nothing, just wait while rf image frequency calibration is done
    }

    // Restore context
    write_to_register( REG_PACONFIG, regPaConfigInitVal );
    set_channel(initialFreq);
}

/**
 * Gets FSK bandwidth values
 *
 * Gives either normal bandwidths or bandwidths for
 * AFC (auto frequency correction)
 */
uint8_t SX1276_LoRaRadio::get_fsk_bw_reg_val(uint32_t bandwidth)
{
    uint8_t i;

    for (i = 0; i < (sizeof(fsk_bandwidths) / sizeof(fsk_bw_t)) - 1; i++) {
        if ((bandwidth >= fsk_bandwidths[i].bandwidth)
                && (bandwidth < fsk_bandwidths[i + 1].bandwidth)) {
            return fsk_bandwidths[i].register_value;
        }
    }
    // ERROR: Value not found
    // This should never happen
    while (1);
}

uint8_t SX1276_LoRaRadio::get_pa_conf_reg(uint32_t channel)
{
    if (radio_variant == SX1276UNDEFINED) {
        return RF_PACONFIG_PASELECT_PABOOST;
    } else if (channel > RF_MID_BAND_THRESH) {
        if (radio_variant == SX1276MB1LAS) {
            return RF_PACONFIG_PASELECT_PABOOST;
        } else {
            return RF_PACONFIG_PASELECT_RFO;
        }
    } else {
        return RF_PACONFIG_PASELECT_RFO;
    }
}

/**
 * Sets the transmit power for the module
 */
void SX1276_LoRaRadio::set_rf_tx_power(int8_t power)
{

    uint8_t paConfig = 0;
    uint8_t paDac = 0;

    paConfig = read_register(REG_PACONFIG);
    paDac = read_register(REG_PADAC);

    paConfig = (paConfig & RF_PACONFIG_PASELECT_MASK) | get_pa_conf_reg(_rf_settings.channel);
    paConfig = (paConfig & RF_PACONFIG_MAX_POWER_MASK) | 0x70;

    if ((paConfig & RF_PACONFIG_PASELECT_PABOOST) == RF_PACONFIG_PASELECT_PABOOST) {
        if (power > 17) {
            paDac = (paDac & RF_PADAC_20DBM_MASK) | RF_PADAC_20DBM_ON;
        } else {
            paDac = (paDac & RF_PADAC_20DBM_MASK) | RF_PADAC_20DBM_OFF;
        }
        if ((paDac & RF_PADAC_20DBM_ON) == RF_PADAC_20DBM_ON) {
            if (power < 5) {
                power = 5;
            }
            if (power > 20) {
                power = 20;
            }
            paConfig = (paConfig & RF_PACONFIG_OUTPUTPOWER_MASK)
                    | (uint8_t) ((uint16_t) (power - 5) & 0x0F);
        } else {
            if (power < 2) {
                power = 2;
            }
            if (power > 17) {
                power = 17;
            }
            paConfig = (paConfig & RF_PACONFIG_OUTPUTPOWER_MASK)
                    | (uint8_t) ((uint16_t) (power - 2) & 0x0F);
        }
    } else {
        if (power < -1) {
            power = -1;
        }
        if (power > 14) {
            power = 14;
        }
        paConfig = (paConfig & RF_PACONFIG_OUTPUTPOWER_MASK)
                | (uint8_t) ((uint16_t) (power + 1) & 0x0F);
    }
    write_to_register( REG_PACONFIG, paConfig);
    write_to_register( REG_PADAC, paDac);
}

/**
 * Actual TX - Transmit routine
 *
 * A DIO0 interrupt let the state machine know that a a packet is
 * successfully sent, otherwise a TxTimeout is invoked.
 * TxTimeout should never happen in normal circumstances as the radio should
 * be able to send a packet out in the air no matter what.
 */
void SX1276_LoRaRadio::transmit(uint32_t timeout)
{
    switch (_rf_settings.modem) {

        case MODEM_FSK:
            // DIO0=PacketSent
            // DIO1=FifoEmpty
            // DIO2=FifoFull
            // DIO3=FifoEmpty
            // DIO4=LowBat
            // DIO5=ModeReady
            write_to_register(REG_DIOMAPPING1,(read_register(REG_DIOMAPPING1) &
                              RF_DIOMAPPING1_DIO0_MASK &
                              RF_DIOMAPPING1_DIO1_MASK &
                              RF_DIOMAPPING1_DIO2_MASK) |
                              RF_DIOMAPPING1_DIO1_01);

            write_to_register(REG_DIOMAPPING2, (read_register(REG_DIOMAPPING2) &
                              RF_DIOMAPPING2_DIO4_MASK &
                              RF_DIOMAPPING2_MAP_MASK));
            _rf_settings.fsk_packet_handler.fifo_thresh =
                              read_register(REG_FIFOTHRESH) & 0x3F;

            break;

        case MODEM_LORA:

            if (_rf_settings.lora.freq_hop_on == true) {
                write_to_register(REG_LR_IRQFLAGSMASK,
                                  RFLR_IRQFLAGS_RXTIMEOUT |
                                  RFLR_IRQFLAGS_RXDONE |
                                  RFLR_IRQFLAGS_PAYLOADCRCERROR |
                                  RFLR_IRQFLAGS_VALIDHEADER |
                                  RFLR_IRQFLAGS_CADDONE |
                                  RFLR_IRQFLAGS_CADDETECTED);

                // DIO0=tx_done, DIO2=fhss_change_channel

                write_to_register(REG_DIOMAPPING1, (read_register(REG_DIOMAPPING1) &
                                  RFLR_DIOMAPPING1_DIO0_MASK &
                                  RFLR_DIOMAPPING1_DIO2_MASK) |
                                  RFLR_DIOMAPPING1_DIO0_01 |
                                  RFLR_DIOMAPPING1_DIO2_01);
            } else {
                write_to_register(REG_LR_IRQFLAGSMASK,
                                  RFLR_IRQFLAGS_RXTIMEOUT |
                                  RFLR_IRQFLAGS_RXDONE |
                                  RFLR_IRQFLAGS_PAYLOADCRCERROR |
                                  RFLR_IRQFLAGS_VALIDHEADER |
                                  RFLR_IRQFLAGS_CADDONE |
                                  RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL |
                                  RFLR_IRQFLAGS_CADDETECTED);

                // DIO0=tx_done
                write_to_register(REG_DIOMAPPING1,(read_register(REG_DIOMAPPING1) &
                                  RFLR_DIOMAPPING1_DIO0_MASK) |
                                  RFLR_DIOMAPPING1_DIO0_01);
            }

            break;
    }

    _rf_settings.state = RF_TX_RUNNING;

    tx_timeout_timer.attach_us(callback(this,
                               &SX1276_LoRaRadio::timeout_irq_isr), timeout * 1000);

    set_operation_mode(RF_OPMODE_TRANSMITTER);
}

/**
 * Get RSSI from the module
 */
int16_t SX1276_LoRaRadio::get_rssi(radio_modems_t modem)
{
    int16_t rssi = 0;

    switch (modem) {
        case MODEM_FSK:
            rssi = -(read_register(REG_RSSIVALUE) >> 1);
            break;
        case MODEM_LORA:
            if (_rf_settings.channel > RF_MID_BAND_THRESH) {
                rssi = RSSI_OFFSET_HF + read_register(REG_LR_RSSIVALUE);
            } else {
                rssi = RSSI_OFFSET_LF + read_register(REG_LR_RSSIVALUE);
            }
            break;
        default:
            rssi = -1;
            break;
    }
    return rssi;
}

/**
 * Sets the module in low power mode by disconnecting
 * TX and RX submodules, turning off power amplifier etc.
 */
void SX1276_LoRaRadio::set_low_power_mode()
{

    if (_rf_ctrls.rf_switch_ctl1 != NC) {
        _rf_switch_ctl1 = 0;
    }

    if (_rf_ctrls.rf_switch_ctl2 != NC) {
        _rf_switch_ctl2 = 0;
    }

    if (_rf_ctrls.pwr_amp_ctl != NC) {
        _pwr_amp_ctl = 0;
    }

    if (_rf_ctrls.txctl != NC) {
        _txctl = 0;
    }

    if (_rf_ctrls.rxctl != NC) {
        _rxctl = 0;
    }

    if (_rf_ctrls.ant_switch != NC) {
        _ant_switch = 0;
    }
}

/**
 * Attaches ISRs to interrupt pins
 */
void SX1276_LoRaRadio::setup_interrupts()
{
    _dio0_ctl.rise(callback(this, &SX1276_LoRaRadio::dio0_irq_isr));
    _dio1_ctl.rise(callback(this, &SX1276_LoRaRadio::dio1_irq_isr));
    _dio2_ctl.rise(callback(this, &SX1276_LoRaRadio::dio2_irq_isr));
    _dio3_ctl.rise(callback(this, &SX1276_LoRaRadio::dio3_irq_isr));
    if (_dio4_pin != NC) {
        _dio4_ctl.rise(callback(this, &SX1276_LoRaRadio::dio4_irq_isr));
    }
    if (_dio5_pin != NC) {
        _dio5_ctl.rise(callback(this, &SX1276_LoRaRadio::dio5_irq_isr));
    }
}

/**
 * Sets up radio latch position according to the
 * radio mode
 */
void SX1276_LoRaRadio::set_antenna_switch(uint8_t mode)
{
    // here we got to do ifdef for changing controls
    // as some pins might be NC
    switch (mode) {
        case RFLR_OPMODE_TRANSMITTER:
            if (_rf_ctrls.rf_switch_ctl1 != NC
                && _rf_ctrls.rf_switch_ctl2 != NC) {
                // module is in transmit mode and RF latch switches
                // are connected. Check if power amplifier boost is
                // setup or not
                if ((read_register(REG_PACONFIG) & RF_PACONFIG_PASELECT_PABOOST)
                                       == RF_PACONFIG_PASELECT_PABOOST) {
                    _rf_switch_ctl1 = 1;
                    _rf_switch_ctl2 = 0;
                } else {
                    // power amplifier not selected
                    _rf_switch_ctl1 = 0;
                    _rf_switch_ctl2 = 1;
                }
            }
            if (_rf_ctrls.txctl != NC && _rf_ctrls.rxctl != NC) {
                // module is in transmit mode and tx/rx submodule control
                // pins are connected
                if (_rf_ctrls.pwr_amp_ctl != NC) {
                    if (read_register(REG_PACONFIG) & RF_PACONFIG_PASELECT_PABOOST) {
                        _pwr_amp_ctl = 1;
                        _txctl = 0;
                    } else {
                        _pwr_amp_ctl = 0;
                        _txctl = 1;
                    }
                } else {
                    _txctl = 1;
                }
                _rxctl = 0;
            }
            if (_rf_ctrls.ant_switch != NC){
                _ant_switch = 1;
            }
            break;
        case RFLR_OPMODE_RECEIVER:
        case RFLR_OPMODE_RECEIVER_SINGLE:
        case RFLR_OPMODE_CAD:
            if (_rf_ctrls.rf_switch_ctl1 != NC
                && _rf_ctrls.rf_switch_ctl2 != NC) {
                // radio is in reception or CAD mode and RF latch switches
                // are connected
                _rf_switch_ctl1 = 1;
                _rf_switch_ctl2 = 1;
            }
            if (_rf_ctrls.txctl != NC && _rf_ctrls.rxctl != NC) {
                _txctl = 0;
                _rxctl = 1;
            }
            if (_rf_ctrls.ant_switch != NC) {
                _ant_switch = 0;
            }
            if (_rf_ctrls.pwr_amp_ctl != NC) {
                _pwr_amp_ctl = 0;
            }
            break;
        default:
            // Enforce default case  when any connected control pin is kept low.
            if (_rf_ctrls.rf_switch_ctl1 != NC
                && _rf_ctrls.rf_switch_ctl2 != NC) {
                // radio is in reception or CAD mode and RF latch switches
                // are connected
                _rf_switch_ctl1 = 0;
                _rf_switch_ctl2 = 0;
            }
            if (_rf_ctrls.txctl != NC && _rf_ctrls.rxctl != NC) {
                _txctl = 0;
                _rxctl = 0;
            }
            if (_rf_ctrls.ant_switch != NC) {
                _ant_switch = 0;
            }
            if (_rf_ctrls.pwr_amp_ctl != NC) {
                _pwr_amp_ctl = 0;
            }
            break;
    }
}

/*****************************************************************************
 * Interrupt service routines (ISRs) - set signals to the irq_thread         *
 ****************************************************************************/
void SX1276_LoRaRadio::dio0_irq_isr()
{
#ifdef MBED_CONF_RTOS_PRESENT
   irq_thread.flags_set(SIG_DIO0);
#else
   handle_dio0_irq();
#endif
}

void SX1276_LoRaRadio::dio1_irq_isr()
{
#ifdef MBED_CONF_RTOS_PRESENT
    irq_thread.flags_set(SIG_DIO1);
#else
    handle_dio1_irq();
#endif
}

void SX1276_LoRaRadio::dio2_irq_isr()
{
#ifdef MBED_CONF_RTOS_PRESENT
    irq_thread.flags_set(SIG_DIO2);
#else
    handle_dio2_irq();
#endif
}

void SX1276_LoRaRadio::dio3_irq_isr()
{
#ifdef MBED_CONF_RTOS_PRESENT
    irq_thread.flags_set(SIG_DIO3);
#else
    handle_dio3_irq();
#endif
}

void SX1276_LoRaRadio::dio4_irq_isr()
{
#ifdef MBED_CONF_RTOS_PRESENT
    irq_thread.flags_set(SIG_DIO4);
#else
    handle_dio4_irq();
#endif
}

void SX1276_LoRaRadio::dio5_irq_isr()
{
#ifdef MBED_CONF_RTOS_PRESENT
    irq_thread.flags_set(SIG_DIO5);
#else
    handle_dio5_irq();
#endif
}

// This is not a hardware interrupt
// we invoke it ourselves based upon
// our timers
void SX1276_LoRaRadio::timeout_irq_isr()
{
#ifdef MBED_CONF_RTOS_PRESENT
    irq_thread.flags_set(SIG_TIMOUT);
#else
    handle_timeout_irq();
#endif
}

/******************************************************************************
 * Interrupt Handlers                                                         *
 *****************************************************************************/

void SX1276_LoRaRadio::handle_dio0_irq()
{
    volatile uint8_t irqFlags = 0;

    switch (_rf_settings.state) {
        case RF_RX_RUNNING:
            switch (_rf_settings.modem) {
                case MODEM_FSK:
                    if (_rf_settings.fsk.crc_on == true) {
                        irqFlags = read_register(REG_IRQFLAGS2);
                        if ((irqFlags & RF_IRQFLAGS2_CRCOK)
                                != RF_IRQFLAGS2_CRCOK) {
                            // Clear Irqs
                            write_to_register(REG_IRQFLAGS1, RF_IRQFLAGS1_RSSI |
                                              RF_IRQFLAGS1_PREAMBLEDETECT |
                                              RF_IRQFLAGS1_SYNCADDRESSMATCH);
                            write_to_register(REG_IRQFLAGS2, RF_IRQFLAGS2_FIFOOVERRUN);

                            if (_rf_settings.fsk.rx_continuous == false) {
                                _rf_settings.state = RF_IDLE;
                            } else {
                                // Continuous mode restart Rx chain
                                write_to_register(REG_RXCONFIG,
                                                  read_register(REG_RXCONFIG) |
                                                  RF_RXCONFIG_RESTARTRXWITHOUTPLLLOCK);
                            }

                            if ((_radio_events != NULL)
                                    && (_radio_events->rx_error)) {
                                _radio_events->rx_error();
                            }
                            _rf_settings.fsk_packet_handler.preamble_detected = 0;
                            _rf_settings.fsk_packet_handler.sync_word_detected = 0;
                            _rf_settings.fsk_packet_handler.nb_bytes = 0;
                            _rf_settings.fsk_packet_handler.size = 0;
                            // break from here, a CRC error happened, RX_ERROR
                            // was notified. No need to go any further
                            break;
                        }
                    }

                    // This block was moved from dio2_handler.
                    // We can have a snapshot of RSSI here as at this point it
                    // should be more smoothed out.
                    _rf_settings.fsk_packet_handler.rssi_value =
                            -(read_register(REG_RSSIVALUE) >> 1);

                    _rf_settings.fsk_packet_handler.afc_value =
                            (int32_t) (float) (((uint16_t) read_register(REG_AFCMSB) << 8)
                                    | (uint16_t) read_register( REG_AFCLSB))
                                    * (float) FREQ_STEP;
                    _rf_settings.fsk_packet_handler.rx_gain =
                                          (read_register( REG_LNA) >> 5) & 0x07;

                    // Read received packet size
                    if ((_rf_settings.fsk_packet_handler.size == 0)
                            && (_rf_settings.fsk_packet_handler.nb_bytes == 0)) {
                        if (_rf_settings.fsk.fix_len == false) {
                            read_fifo((uint8_t*) &_rf_settings.fsk_packet_handler.size, 1);
                        } else {
                            _rf_settings.fsk_packet_handler.size = read_register(REG_PAYLOADLENGTH);
                        }
                        read_fifo(_data_buffer + _rf_settings.fsk_packet_handler.nb_bytes,
                                _rf_settings.fsk_packet_handler.size - _rf_settings.fsk_packet_handler.nb_bytes);
                        _rf_settings.fsk_packet_handler.nb_bytes +=
                                (_rf_settings.fsk_packet_handler.size - _rf_settings.fsk_packet_handler.nb_bytes);
                    } else {
                        read_fifo(_data_buffer + _rf_settings.fsk_packet_handler.nb_bytes,
                                _rf_settings.fsk_packet_handler.size - _rf_settings.fsk_packet_handler.nb_bytes);
                        _rf_settings.fsk_packet_handler.nb_bytes +=
                                (_rf_settings.fsk_packet_handler.size - _rf_settings.fsk_packet_handler.nb_bytes);
                    }

                    if (_rf_settings.fsk.rx_continuous == false) {
                        _rf_settings.state = RF_IDLE;
                    } else {
                        // Continuous mode restart Rx chain
                        write_to_register(REG_RXCONFIG, read_register(REG_RXCONFIG)
                                        | RF_RXCONFIG_RESTARTRXWITHOUTPLLLOCK);
                    }

                    if ((_radio_events != NULL) && (_radio_events->rx_done)) {
                        _radio_events->rx_done(
                                _data_buffer,
                                _rf_settings.fsk_packet_handler.size,
                                _rf_settings.fsk_packet_handler.rssi_value, 0);
                    }
                    _rf_settings.fsk_packet_handler.preamble_detected = 0;
                    _rf_settings.fsk_packet_handler.sync_word_detected = 0;
                    _rf_settings.fsk_packet_handler.nb_bytes = 0;
                    _rf_settings.fsk_packet_handler.size = 0;
                    break;

                case MODEM_LORA: {
                    int8_t snr = 0;

                    // Clear Irq
                    write_to_register(REG_LR_IRQFLAGS, RFLR_IRQFLAGS_RXDONE);

                    irqFlags = read_register(REG_LR_IRQFLAGS);
                    if ((irqFlags & RFLR_IRQFLAGS_PAYLOADCRCERROR_MASK)
                            == RFLR_IRQFLAGS_PAYLOADCRCERROR) {
                        // Clear Irq
                        write_to_register( REG_LR_IRQFLAGS, RFLR_IRQFLAGS_PAYLOADCRCERROR);

                        if (_rf_settings.lora.rx_continuous == false) {
                            _rf_settings.state = RF_IDLE;
                        }

                        if ((_radio_events != NULL)
                                && (_radio_events->rx_error)) {
                            _radio_events->rx_error();
                        }
                        break;
                    }

                    _rf_settings.lora_packet_handler.snr_value = read_register(
                            REG_LR_PKTSNRVALUE);
                    if (_rf_settings.lora_packet_handler.snr_value & 0x80) // The SNR sign bit is 1
                            {
                        // Invert and divide by 4
                        snr = ((~_rf_settings.lora_packet_handler.snr_value + 1)
                                & 0xFF) >> 2;
                        snr = -snr;
                    } else {
                        // Divide by 4
                        snr =
                                (_rf_settings.lora_packet_handler.snr_value
                                        & 0xFF) >> 2;
                    }

                    int16_t rssi = read_register( REG_LR_PKTRSSIVALUE);
                    if (snr < 0) {
                        if (_rf_settings.channel > RF_MID_BAND_THRESH) {
                            _rf_settings.lora_packet_handler.rssi_value =
                                    RSSI_OFFSET_HF + rssi + (rssi >> 4) + snr;
                        } else {
                            _rf_settings.lora_packet_handler.rssi_value =
                                    RSSI_OFFSET_LF + rssi + (rssi >> 4) + snr;
                        }
                    } else {
                        if (_rf_settings.channel > RF_MID_BAND_THRESH) {
                            _rf_settings.lora_packet_handler.rssi_value =
                                    RSSI_OFFSET_HF + rssi + (rssi >> 4);
                        } else {
                            _rf_settings.lora_packet_handler.rssi_value =
                                    RSSI_OFFSET_LF + rssi + (rssi >> 4);
                        }
                    }

                    _rf_settings.lora_packet_handler.size = read_register(REG_LR_RXNBBYTES);
                    read_fifo(_data_buffer, _rf_settings.lora_packet_handler.size);

                    if (_rf_settings.lora.rx_continuous == false) {
                        _rf_settings.state = RF_IDLE;
                    }

                    if ((_radio_events != NULL) && (_radio_events->rx_done)) {
                        _radio_events->rx_done(_data_buffer,
                                _rf_settings.lora_packet_handler.size,
                                _rf_settings.lora_packet_handler.rssi_value,
                                _rf_settings.lora_packet_handler.snr_value);
                    }
                }
                    break;
                default:
                    break;
            }
            break;
        case RF_TX_RUNNING:
            tx_timeout_timer.detach();
            // TxDone interrupt
            switch (_rf_settings.modem) {
                case MODEM_LORA:
                    // Clear Irq
                    write_to_register(REG_LR_IRQFLAGS, RFLR_IRQFLAGS_TXDONE);
                    // Intentional fall through
                case MODEM_FSK:
                default:
                    _rf_settings.state = RF_IDLE;
                    if ((_radio_events != NULL)
                            && (_radio_events->tx_done)) {
                        _radio_events->tx_done();
                    }
                    break;
            }
            break;
        default:
            break;
    }
}

void SX1276_LoRaRadio::handle_dio1_irq()
{
    switch (_rf_settings.state) {
        case RF_RX_RUNNING:
            switch (_rf_settings.modem) {
                case MODEM_FSK:
                    // FifoLevel interrupt
                    // Read received packet size
                    if ((_rf_settings.fsk_packet_handler.size == 0)
                            && (_rf_settings.fsk_packet_handler.nb_bytes == 0)) {
                        if (_rf_settings.fsk.fix_len == false) {
                            read_fifo((uint8_t*) &_rf_settings.fsk_packet_handler.size, 1);
                        } else {
                            _rf_settings.fsk_packet_handler.size =
                                    read_register(REG_PAYLOADLENGTH);
                        }
                    }

                    if ((_rf_settings.fsk_packet_handler.size
                            - _rf_settings.fsk_packet_handler.nb_bytes)
                            > _rf_settings.fsk_packet_handler.fifo_thresh) {
                        read_fifo((_data_buffer + _rf_settings.fsk_packet_handler.nb_bytes),
                                _rf_settings.fsk_packet_handler.fifo_thresh);
                        _rf_settings.fsk_packet_handler.nb_bytes +=
                                _rf_settings.fsk_packet_handler.fifo_thresh;
                    } else {
                        read_fifo((_data_buffer + _rf_settings.fsk_packet_handler.nb_bytes),
                                _rf_settings.fsk_packet_handler.size
                                        - _rf_settings.fsk_packet_handler.nb_bytes);
                        _rf_settings.fsk_packet_handler.nb_bytes +=
                                (_rf_settings.fsk_packet_handler.size
                                        - _rf_settings.fsk_packet_handler.nb_bytes);
                    }

                    break;

                case MODEM_LORA:
                    // Clear Irq
                    write_to_register(REG_LR_IRQFLAGS, RFLR_IRQFLAGS_RXTIMEOUT);
                    _rf_settings.state = RF_IDLE;
                    if ((_radio_events != NULL)
                            && (_radio_events->rx_timeout)) {
                        _radio_events->rx_timeout();
                    }
                    break;
                default:
                    break;
            }

            break;

        case RF_TX_RUNNING:
            switch (_rf_settings.modem) {
                case MODEM_FSK:
                    // FifoLevel interrupt
                    if ((_rf_settings.fsk_packet_handler.size
                            - _rf_settings.fsk_packet_handler.nb_bytes)
                            > _rf_settings.fsk_packet_handler.chunk_size) {
                        write_fifo((_data_buffer + _rf_settings.fsk_packet_handler.nb_bytes),
                                _rf_settings.fsk_packet_handler.chunk_size);
                        _rf_settings.fsk_packet_handler.nb_bytes +=
                                _rf_settings.fsk_packet_handler.chunk_size;
                    } else {
                        // Write the last chunk of data
                        write_fifo(_data_buffer + _rf_settings.fsk_packet_handler.nb_bytes,
                                _rf_settings.fsk_packet_handler.size
                                        - _rf_settings.fsk_packet_handler.nb_bytes);
                        _rf_settings.fsk_packet_handler.nb_bytes +=
                                _rf_settings.fsk_packet_handler.size - _rf_settings.fsk_packet_handler.nb_bytes;
                    }

                    break;

                case MODEM_LORA:
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

void SX1276_LoRaRadio::handle_dio2_irq(void)
{
    switch (_rf_settings.state) {
        case RF_RX_RUNNING:
            switch (_rf_settings.modem) {
                case MODEM_FSK:
                    _rf_settings.fsk_packet_handler.preamble_detected = 0;
                    _rf_settings.fsk_packet_handler.sync_word_detected = 0;
                    _rf_settings.fsk_packet_handler.nb_bytes = 0;
                    _rf_settings.fsk_packet_handler.size = 0;

                    // Clear Irqs
                    write_to_register(REG_IRQFLAGS1, RF_IRQFLAGS1_RSSI |
                                      RF_IRQFLAGS1_PREAMBLEDETECT |
                                      RF_IRQFLAGS1_SYNCADDRESSMATCH |
                                      RF_IRQFLAGS1_TIMEOUT);

                    write_to_register( REG_IRQFLAGS2, RF_IRQFLAGS2_FIFOOVERRUN);

                    if (_rf_settings.fsk.rx_continuous == true) {
                        // Continuous mode restart Rx chain
                        write_to_register( REG_RXCONFIG,
                                          read_register(REG_RXCONFIG) |
                                          RF_RXCONFIG_RESTARTRXWITHOUTPLLLOCK);
                    } else {
                        _rf_settings.state = RF_IDLE;
                    }

                    if ((_radio_events != NULL)
                            && (_radio_events->rx_timeout)) {
                        _radio_events->rx_timeout();
                    }

                    break;

                case MODEM_LORA:
                    if (_rf_settings.lora.freq_hop_on == true) {
                        // Clear Irq
                        write_to_register(REG_LR_IRQFLAGS,
                                          RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL);

                        if ((_radio_events != NULL)
                                && (_radio_events->fhss_change_channel)) {
                            _radio_events->fhss_change_channel(
                                    (read_register(REG_LR_HOPCHANNEL)
                                            & RFLR_HOPCHANNEL_CHANNEL_MASK));
                        }
                    }

                    break;

                default:
                    break;
            }

            break;

        case RF_TX_RUNNING:
            switch (_rf_settings.modem) {
                case MODEM_FSK:
                    break;
                case MODEM_LORA:
                    if (_rf_settings.lora.freq_hop_on == true) {
                        // Clear Irq
                        write_to_register(REG_LR_IRQFLAGS,
                                          RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL);

                        if ((_radio_events != NULL)
                                && (_radio_events->fhss_change_channel)) {
                            _radio_events->fhss_change_channel(
                                    (read_register(REG_LR_HOPCHANNEL)
                                            & RFLR_HOPCHANNEL_CHANNEL_MASK));
                        }
                    }
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

void SX1276_LoRaRadio::handle_dio3_irq(void)
{
    switch (_rf_settings.modem) {
        case MODEM_FSK:
            break;
        case MODEM_LORA:
            if ((read_register(REG_LR_IRQFLAGS) & RFLR_IRQFLAGS_CADDETECTED)
                    == RFLR_IRQFLAGS_CADDETECTED) {
                // Clear Irq
                write_to_register(REG_LR_IRQFLAGS,
                        RFLR_IRQFLAGS_CADDETECTED | RFLR_IRQFLAGS_CADDONE);
                if ((_radio_events != NULL)
                        && (_radio_events->cad_done)) {
                    _radio_events->cad_done(true);
                }
            } else {
                // Clear Irq
                write_to_register(REG_LR_IRQFLAGS, RFLR_IRQFLAGS_CADDONE);
                if ((_radio_events != NULL)
                        && (_radio_events->cad_done)) {
                    _radio_events->cad_done(false);
                }
            }
            break;
        default:
            break;
    }
}

void SX1276_LoRaRadio::handle_dio4_irq(void)
{
    // is asserted when a preamble is detected (FSK modem only)
    switch (_rf_settings.modem) {
        case MODEM_FSK: {
            if (_rf_settings.fsk_packet_handler.preamble_detected == 0) {
                _rf_settings.fsk_packet_handler.preamble_detected = 1;
            }
        }
            break;
        case MODEM_LORA:
            break;
        default:
            break;
    }
}

void SX1276_LoRaRadio::handle_dio5_irq()
{
    switch (_rf_settings.modem) {
        case MODEM_FSK:
            break;
        case MODEM_LORA:
            break;
        default:
            break;
    }
}

void SX1276_LoRaRadio::handle_timeout_irq()
{
    tx_timeout_timer.detach();

    if (_rf_settings.state == RF_TX_RUNNING) {
        // Tx timeout shouldn't happen.
        // But it has been observed that when it happens it is a result of a
        // corrupted SPI transfer
        // The workaround is to put the radio in a known state.
        // Thus, we re-initialize it.

        // Initialize radio default values
        set_operation_mode(RF_OPMODE_SLEEP);

        setup_registers();

        set_modem(MODEM_FSK);

        // Restore previous network type setting.
        set_public_network(_rf_settings.lora.public_network);

        _rf_settings.state = RF_IDLE;

        if ((_radio_events != NULL) && (_radio_events->tx_timeout)) {
            _radio_events->tx_timeout();
        }
    }
}
// EOF
