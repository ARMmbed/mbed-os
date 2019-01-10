/**
 * Copyright (c) 2017, Arm Limited and affiliates.
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

#ifndef LORARADIO_H_
#define LORARADIO_H_

/** @addtogroup LoRaWAN
 * Parent class for a LoRa radio driver
 *  @{
 */

#include "platform/Callback.h"
#include "PinNames.h"

/**
 * Structure to hold RF controls for LoRa Radio.
 * SX1276 have an extra control for the crystal (used in DISCO-L072CZ).
 * A subset of these pins may be used by the driver in accordance with the physical
 * implementation.
 */
typedef struct {
    /** TX latch switch pin.
     * Exact operation is implementation specific.
     */
    PinName rf_switch_ctl1;

    /** RX latch switch pin.
     * Exact operation is implementation specific.
     */
    PinName rf_switch_ctl2;

    /** TX control pin for transceiver packaged as a module.
     * Exact operation is implementation specific.
     */
    PinName txctl;

    /** RX control pin for transceiver packaged as a module.
     * Exact operation is implementation specific.
     */
    PinName rxctl;

    /** Transceiver switch pin.
     * Exact operation is implementation specific. One of the polarities of the
     * pin may drive the transceiver in either TX or RX mode.
     */
    PinName ant_switch;

    /** Power amplifier control pin.
     * Exact operation is implementation specific. If defined,
     * controls the operation of an external power amplifier.
     */
    PinName pwr_amp_ctl;

    /** TCXO crystal control pin.
     * Exact operation is implementation specific.
     */
    PinName tcxo;
} rf_ctrls;

/** Radio driver internal state.
 * Helps identify current state of the transceiver.
 */
typedef enum radio_state {
    /** IDLE state.
     * Radio is in idle state.
     */
    RF_IDLE = 0,

    /** RX state.
     * Radio is receiving.
     */
    RF_RX_RUNNING,

    /** TX state.
     * Radio is transmitting.
     */
    RF_TX_RUNNING,

    /** CAD state.
     * Radio is detecting channel activity.
     */
    RF_CAD,
} radio_state_t;

/** Type of modem.
 *  [LORA/FSK]
 */
typedef enum modem_type {
    /** FSK operation mode.
     * Radio is using FSK modulation.
     */
    MODEM_FSK = 0,

    /** LoRa operation mode.
     * Radio is using LoRa modulation.
     */
    MODEM_LORA
} radio_modems_t;

/** FSK modem parameters.
 * Parameters encompassing FSK modulation.
 */
typedef struct radio_fsk_settings {
    /**
     * Transmit power.
     */
    int8_t   power;

    /**
     * Frequency deviation.
     */
    uint32_t f_dev;

    /**
     * Modulation bandwidth.
     */
    uint32_t bandwidth;

    /**
     * Automated frequency correction bandwidth.
     */
    uint32_t bandwidth_afc;

    /**
     * Data rate (SF).
     */
    uint32_t datarate;

    /**
     * Expected preamble length.
     */
    uint16_t preamble_len;

    /**
     * This flag turns on if the TX data size is fixed.
     */
    bool fix_len;

    /**
     * Size of outgoing data.
     */
    uint8_t  payload_len;

    /**
     * Turn CRC on/off.
     */
    bool crc_on;

    /** @deprecated
     * Does not apply to FSK. Will be removed.
     */
    bool iq_inverted;

    /**
     * Turn continuous reception mode (such as Class C mode) on/off.
     */
    bool rx_continuous;

    /**
     * Timeout value in milliseconds (ms) after which the radio driver reports
     * a timeout if the radio was unable to transmit.
     */
    uint32_t tx_timeout;

    /**
     * Timeout value in symbols (symb) after which the radio driver reports a timeout
     * if the radio did not receive a Preamble.
     */
    uint32_t rx_single_timeout;
} radio_fsk_settings_t;

/** FSK packet handle.
 * Contains information about an FSK packet and various metadata.
 */
typedef struct radio_fsk_packet_handler {
    /**
     * Set to true (1) when a Preamble is detected, otherwise false (0).
     */
    uint8_t preamble_detected;

    /**
     * Set to true (1) when a SyncWord is detected, otherwise false (0).
     */
    uint8_t sync_word_detected;

    /**
     * Storage for RSSI value of the received signal.
     */
    int8_t rssi_value;

    /**
     * Automated frequency correction value.
     */
    int32_t afc_value;

    /**
     * LNA gain value (dbm).
     */
    uint8_t rx_gain;

    /**
     * Size of the received data in bytes.
     */
    uint16_t size;

    /**
     * Keeps track of number of bytes already read from the RX FIFO.
     */
    uint16_t nb_bytes;

    /**
     * Stores the FIFO threshold value.
     */
    uint8_t fifo_thresh;

    /**
     * Defines the size of a chunk of outgoing buffer written to
     * the FIFO at a unit time. For example, if the size of the data exceeds the FIFO
     * limit, a certain sized chunk is written to the FIFO. Later, a FIFO-level
     * interrupt enables writing of the remaining data to the FIFO chunk by chunk until
     * transmission is complete.
     */
    uint8_t chunk_size;
} radio_fsk_packet_handler_t;

/** LoRa modem parameters.
 * Parameters encompassing LoRa modulation.
 */
typedef struct radio_lora_settings {
    /**
     * Transmit power.
     */
    int8_t power;

    /**
     * Modulation bandwidth.
     */
    uint32_t bandwidth;

    /**
     * Data rate (SF).
     */
    uint32_t datarate;

    /**
     * Turn low data rate optimization on/off.
     */
    bool low_datarate_optimize;

    /**
     * Error correction code rate.
     */
    uint8_t coderate;

    /**
     * Preamble length in symbols.
     */
    uint16_t preamble_len;

    /**
     * Set to true if the outgoing payload length is fixed.
     */
    bool fix_len;

    /**
     * Size of outgoing payload.
     */
    uint8_t payload_len;

    /**
     * Turn CRC on/off.
     */
    bool crc_on;

    /**
     * Turn frequency hopping on/off.
     */
    bool freq_hop_on;

    /**
     * Number of symbols between two frequency hops.
     */
    uint8_t hop_period;

    /**
     * Turn IQ inversion on/off. Usually, the end device sends an IQ inverted
     * signal, and the base stations do not invert. We recommended sending an
     * IQ inverted signal from the device side, so any transmissions from the
     * base stations do not interfere with end device transmission.
     */
    bool iq_inverted;

    /**
     * Turn continuous reception mode (such as in Class C) on/off.
     */
    bool rx_continuous;

    /**
     * Timeout in milliseconds (ms) after which the radio driver reports an error
     * if the radio was unable to transmit.
     */
    uint32_t tx_timeout;

    /**
     * Change the network mode to Public or Private.
     */
    bool public_network;
} radio_lora_settings_t;

/** LoRa packet
 * Contains information about a LoRa packet.
 */
typedef struct radio_lora_packet_handler {
    /**
     * Signal-to-noise ratio of a received packet.
     */
    int8_t snr_value;

    /**
     * RSSI value in dBm for the received packet.
     */
    int8_t rssi_value;

    /**
     * Size of the transmitted or received packet.
     */
    uint8_t size;
} radio_lora_packet_handler_t;

/** Global radio settings.
 * Contains settings for the overall transceiver operation.
 */
typedef struct radio_settings {
    /**
     * Current state of the radio, such as RF_IDLE.
     */
    uint8_t                     state;

    /**
     * Current modem operation, such as MODEM_LORA.
     */
    uint8_t                     modem;

    /**
     * Current channel of operation.
     */
    uint32_t                    channel;

    /**
     * Settings for FSK modem part.
     */
    radio_fsk_settings_t        fsk;

    /**
     * FSK packet and meta data.
     */
    radio_fsk_packet_handler_t  fsk_packet_handler;

    /**
     * Settings for LoRa modem part.
     */
    radio_lora_settings_t       lora;

    /**
     * LoRa packet and metadata.
     */
    radio_lora_packet_handler_t lora_packet_handler;
} radio_settings_t;

/** Reporting functions for upper layers.
 * The radio driver reports various vital events to the upper controlling layers
 * using callback functions provided by the upper layers at the initialization
 * phase.
 */
typedef struct radio_events {
    /**
     * Callback when Transmission is done.
     */
    mbed::Callback<void()> tx_done;

    /**
     * Callback when Transmission is timed out.
     */
    mbed::Callback<void()> tx_timeout;

    /**
     * Rx Done callback prototype.
     *
     *  @param payload Received buffer pointer.
     *  @param size    Received buffer size.
     *  @param rssi    RSSI value computed while receiving the frame [dBm].
     *  @param snr     Raw SNR value given by the radio hardware.
     *                     FSK : N/A (set to 0)
     *                     LoRa: SNR value in dB
     */
    mbed::Callback<void(const uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr)> rx_done;

    /**
     * Callback when Reception is timed out.
     */
    mbed::Callback<void()> rx_timeout;

    /**
     * Callback when Reception ends up in error.
     */
    mbed::Callback<void()> rx_error;

    /**
     * FHSS Change Channel callback prototype.
     *
     *  @param current_channel   The index number of the current channel.
     */
    mbed::Callback<void(uint8_t current_channel)> fhss_change_channel;

    /**
     * CAD Done callback prototype.
     *
     *  @param channel_busy    True, if Channel activity detected.
     */
    mbed::Callback<void(bool channel_busy)> cad_done;
} radio_events_t;

/**
 * Interface for the radios, containing the main functions that a radio needs, and five callback functions.
 */
class LoRaRadio {

public:

    /**
     * Registers radio events with the Mbed LoRaWAN stack and undergoes initialization steps, if any.
     *
     *  @param events Contains driver callback functions.
     */
    virtual void init_radio(radio_events_t *events) = 0;

    /**
     * Resets the radio module.
     */
    virtual void radio_reset() = 0;

    /**
     *  Put the RF module in sleep mode.
     */
    virtual void sleep(void) = 0;

    /**
     *  Sets the radio to standby mode.
     */
    virtual void standby(void) = 0;

    /**
     *  Sets reception parameters.
     *
     *  @param modem         The radio modem [0: FSK, 1: LoRa].
     *  @param bandwidth     Sets the bandwidth.
     *                          FSK : >= 2600 and <= 250000 Hz
     *                          LoRa: [0: 125 kHz, 1: 250 kHz,
     *                                 2: 500 kHz, 3: Reserved]
     *  @param datarate      Sets the datarate.
     *                          FSK : 600..300000 bits/s
     *                          LoRa: [6: 64, 7: 128, 8: 256, 9: 512,
     *                                10: 1024, 11: 2048, 12: 4096  chips]
     *  @param coderate      Sets the coding rate (LoRa only).
     *                          FSK : N/A ( set to 0 )
     *                          LoRa: [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8]
     *  @param bandwidth_afc Sets the AFC bandwidth (FSK only).
     *                          FSK : >= 2600 and <= 250000 Hz
     *                          LoRa: N/A (set to 0)
     *  @param preamble_len  Sets the preamble length (LoRa only).
     *                          FSK : N/A (set to 0)
     *                          LoRa: Length in symbols (the hardware adds four more symbols).
     *  @param symb_timeout  Sets the RxSingle timeout value.
     *                          FSK : Timeout number of bytes
     *                          LoRa: Timeout in symbols
     *  @param fix_len        Fixed length packets [0: variable, 1: fixed].
     *  @param payload_len   Sets the payload length when fixed length is used.
     *  @param crc_on        Enables/disables CRC [0: OFF, 1: ON].
     *  @param freq_hop_on   Enables/disables intra-packet frequency hopping [0: OFF, 1: ON] (LoRa only).
     *  @param hop_period    The number of symbols bewteen each hop (LoRa only).
     *  @param iq_inverted   Inverts the IQ signals (LoRa only).
     *                          FSK : N/A (set to 0)
     *                          LoRa: [0: not inverted, 1: inverted]
     *  @param rx_continuous Sets the reception to continuous mode.
     *                          [false: single mode, true: continuous mode]
     */
    virtual void set_rx_config(radio_modems_t modem, uint32_t bandwidth,
                               uint32_t datarate, uint8_t coderate,
                               uint32_t bandwidth_afc, uint16_t preamble_len,
                               uint16_t symb_timeout, bool fix_len,
                               uint8_t payload_len,
                               bool crc_on, bool freq_hop_on, uint8_t hop_period,
                               bool iq_inverted, bool rx_continuous) = 0;

    /**
     *  Sets the transmission parameters.
     *
     *  @param modem         The radio modem [0: FSK, 1: LoRa].
     *  @param power         Sets the output power [dBm].
     *  @param fdev          Sets the frequency deviation (FSK only).
     *                          FSK : [Hz]
     *                          LoRa: 0
     *  @param bandwidth     Sets the bandwidth (LoRa only).
     *                          FSK : 0
     *                          LoRa: [0: 125 kHz, 1: 250 kHz,
     *                                 2: 500 kHz, 3: Reserved]
     *  @param datarate      Sets the datarate.
     *                          FSK : 600..300000 bits/s
     *                          LoRa: [6: 64, 7: 128, 8: 256, 9: 512,
     *                                10: 1024, 11: 2048, 12: 4096  chips]
     *  @param coderate      Sets the coding rate (LoRa only).
     *                          FSK : N/A ( set to 0 )
     *                          LoRa: [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8]
     *  @param preamble_len  Sets the preamble length.
     *  @param fix_len       Fixed length packets [0: variable, 1: fixed].
     *  @param crc_on        Enables/disables CRC [0: OFF, 1: ON].
     *  @param freq_hop_on   Enables/disables intra-packet frequency hopping [0: OFF, 1: ON] (LoRa only).
     *  @param hop_period    The number of symbols between each hop (LoRa only).
     *  @param iq_inverted   Inverts IQ signals (LoRa only)
     *                          FSK : N/A (set to 0).
     *                          LoRa: [0: not inverted, 1: inverted]
     *  @param timeout       The transmission timeout [ms].
     */
    virtual void set_tx_config(radio_modems_t modem, int8_t power, uint32_t fdev,
                               uint32_t bandwidth, uint32_t datarate,
                               uint8_t coderate, uint16_t preamble_len,
                               bool fix_len, bool crc_on, bool freq_hop_on,
                               uint8_t hop_period, bool iq_inverted, uint32_t timeout) = 0;

    /**
     *  Sends the packet.
     *
     *  Prepares the packet to be sent and sets the radio to transmission mode.
     *
     *  @param buffer        A pointer to the buffer.
     *  @param size          The buffer size.
     */
    virtual void send(uint8_t *buffer, uint8_t size) = 0;

    /**
     *  Sets the radio to reception mode.
     *
     *  To configure the receiver, use the `set_rx_config()` API.
     */
    virtual void receive(void) = 0;

    /**
     *  Sets the carrier frequency.
     *
     *  @param freq          Channel RF frequency.
     */
    virtual void set_channel(uint32_t freq) = 0;

    /**
     *  Generates a 32 bit random value based on RSSI readings.
     *
     *  \remark This function sets the radio in LoRa modem mode and disables all interrupts.
     *          After calling this function, either `Radio.SetRxConfig` or
     *         `Radio.SetTxConfig` functions must be called.
     *
     *  @return             A 32 bit random value.
     */
    virtual uint32_t random(void) = 0;

    /**
     *  Gets the radio status.
     *
     *  @return              The current radio status.
     */
    virtual uint8_t get_status(void) = 0;

    /**
     *  Sets the maximum payload length.
     *
     *  @param modem         The radio modem [0: FSK, 1: LoRa].
     *  @param max           The maximum payload length in bytes.
     */
    virtual void set_max_payload_length(radio_modems_t modem, uint8_t max) = 0;

    /**
     *  Sets the network to public or private.
     *
     *  Updates the sync byte. Applies to LoRa modem only.
     *
     *  @param enable        If true, enables a public network.
     */
    virtual void set_public_network(bool enable) = 0;

    /**
     *  Computes the packet time on air for the given payload.
     *
     *  \remark This can only be called after `SetRxConfig` or `SetTxConfig`.
     *
     *  @param modem         The radio modem [0: FSK, 1: LoRa].
     *  @param pkt_len       The packet payload length.
     *  @return              The computed `airTime` for the given packet payload length.
     */
    virtual uint32_t time_on_air(radio_modems_t modem, uint8_t pkt_len) = 0;

    /**
     * Performs carrier sensing.
     *
     * Checks for a certain time if the RSSI is above a given threshold.
     * This threshold determines whether or not there is a transmission on
     * the channel already.
     *
     * @param modem                     The type of radio modem.
     * @param freq                      The carrier frequency.
     * @param rssi_threshold            The threshold value of RSSI.
     * @param max_carrier_sense_time    The time set for sensing the channel (ms).
     *
     * @return                          True if there is no active transmission
     *                                  in the channel, otherwise false.
     */
    virtual bool perform_carrier_sense(radio_modems_t modem,
                                       uint32_t freq,
                                       int16_t rssi_threshold,
                                       uint32_t max_carrier_sense_time) = 0;

    /**
     *  Sets the radio to CAD mode.
     *
     */
    virtual void start_cad(void) = 0;

    /**
     *  Checks whether the given RF is in range.
     *
     *  @param frequency       The frequency to check.
     */
    virtual bool check_rf_frequency(uint32_t frequency) = 0;

    /** Sets the radio to continuous wave transmission mode.
     *
     *  @param freq          The RF frequency of the channel.
     *  @param power         The output power [dBm].
     *  @param time          The transmission mode timeout [s].
     */
    virtual void set_tx_continuous_wave(uint32_t freq, int8_t power, uint16_t time) = 0;

    /**
     * Acquires exclusive access to this radio.
     */
    virtual void lock(void) = 0;

    /**
     * Releases exclusive access to this radio.
     */
    virtual void unlock(void) = 0;
};

#endif // LORARADIO_H_
/** @}*/
