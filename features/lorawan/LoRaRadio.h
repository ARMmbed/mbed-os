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

#include "platform/Callback.h"
#include "PinNames.h"

/**
 * Structure to hold RF controls for LoRa Radio.
 * SX1276 have an extra control for the crystal (used in DOSCO-L072CZ)
 */
typedef struct {
    PinName rf_switch_ctl1;
    PinName rf_switch_ctl2;
    PinName txctl;
    PinName rxctl;
    PinName ant_switch;
    PinName pwr_amp_ctl;
    PinName tcxo;
} rf_ctrls;

/** Radio driver internal state.
 * State machine states definition.
 */
typedef enum radio_state {
    RF_IDLE = 0,
    RF_RX_RUNNING,
    RF_TX_RUNNING,
    RF_CAD,
} radio_state_t;

/** Type of the modem.
 *  [LORA/FSK]
 */
typedef enum modem_type {
    MODEM_FSK = 0,
    MODEM_LORA
} radio_modems_t;

/** Radio FSK modem parameters.
 *
 */
typedef struct radio_fsk_settings {
    int8_t   power;
    uint32_t f_dev;
    uint32_t bandwidth;
    uint32_t bandwidth_afc;
    uint32_t datarate;
    uint16_t preamble_len;
    bool     fix_len;
    uint8_t  payload_len;
    bool     crc_on;
    bool     iq_inverted;
    bool     rx_continuous;
    uint32_t tx_timeout;
    uint32_t rx_single_timeout;
} radio_fsk_settings_t;

/** Radio FSK packet handler state.
 *
 */
typedef struct radio_fsk_packet_handler {
    uint8_t  preamble_detected;
    uint8_t  sync_word_detected;
    int8_t   rssi_value;
    int32_t  afc_value;
    uint8_t  rx_gain;
    uint16_t size;
    uint16_t nb_bytes;
    uint8_t  fifo_thresh;
    uint8_t  chunk_size;
} radio_fsk_packet_handler_t;

/** Radio LoRa modem parameters.
 *
 */
typedef struct radio_lora_settings {
    int8_t   power;
    uint32_t bandwidth;
    uint32_t datarate;
    bool     low_datarate_optimize;
    uint8_t  coderate;
    uint16_t preamble_len;
    bool     fix_len;
    uint8_t  payload_len;
    bool     crc_on;
    bool     freq_hop_on;
    uint8_t  hop_period;
    bool     iq_inverted;
    bool     rx_continuous;
    uint32_t tx_timeout;
    bool     public_network;
} radio_lora_settings_t;

/** Radio LoRa packet handler state.
 *
 */
typedef struct radio_lora_packet_handler {
    int8_t  snr_value;
    int8_t  rssi_value;
    uint8_t size;
} radio_lora_packet_handler_t;

/** Radio settings.
 *
 */
typedef struct radio_settings {
    uint8_t                     state;
    uint8_t                     modem;
    uint32_t                    channel;
    radio_fsk_settings_t        fsk;
    radio_fsk_packet_handler_t  fsk_packet_handler;
    radio_lora_settings_t       lora;
    radio_lora_packet_handler_t lora_packet_handler;
} radio_settings_t;

/** Radio driver callback functions.
 *
 */
typedef struct radio_events {
    /**
     * Callback when Transmission is done
     */
    mbed::Callback<void()> tx_done;

    /**
     * Callback when Transmission is timed out
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
    mbed::Callback<void(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr)> rx_done;

    /**
     * Callback when Reception is timed out
     */
    mbed::Callback<void()> rx_timeout;

    /**
     * Callback when Reception ends up in error
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
 *    Interface for the radios, contains the main functions that a radio needs, and five callback functions.
 */
class LoRaRadio
{

public:

    /**
     * Registers radio events with the Mbed LoRaWAN stack and undergoes the initialization steps if any.
     *
     *  @param events The structure containing the driver callback functions.
     */
    virtual void init_radio(radio_events_t *events) = 0;

    /**
     * Resets the radio module.
     */
    virtual void radio_reset() = 0;

    /**
     *  Put the RF module in the sleep mode.
     */
    virtual void sleep(void) = 0;

    /**
     *  Sets the radio in the standby mode.
     */
    virtual void standby(void) = 0;

    /**
     *  Sets the reception parameters.
     *
     *  @param modem         The radio modem to be used [0: FSK, 1: LoRa].
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
     *  @param crc_on        Enables/disables the CRC [0: OFF, 1: ON].
     *  @param freq_hop_on   Enables/disables the intra-packet frequency hopping [0: OFF, 1: ON] (LoRa only).
     *  @param hop_period    The number of symbols bewteen each hop (LoRa only).
     *  @param iq_inverted   Inverts the IQ signals (LoRa only).
     *                          FSK : N/A (set to 0)
     *                          LoRa: [0: not inverted, 1: inverted]
     *  @param rx_continuous Sets the reception in continuous mode.
     *                          [false: single mode, true: continuous mode]
     */
    virtual void set_rx_config (radio_modems_t modem, uint32_t bandwidth,
                               uint32_t datarate, uint8_t coderate,
                               uint32_t bandwidth_afc, uint16_t preamble_len,
                               uint16_t symb_timeout, bool fix_len,
                               uint8_t payload_len,
                               bool crc_on, bool freq_hop_on, uint8_t hop_period,
                               bool iq_inverted, bool rx_continuous) = 0;

    /**
     *  Sets the transmission parameters.
     *
     *  @param modem         The radio modem to be used [0: FSK, 1: LoRa].
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
     *  @param crc_on        Enables/disables the CRC [0: OFF, 1: ON].
     *  @param freq_hop_on   Enables/disables the intra-packet frequency hopping [0: OFF, 1: ON] (LoRa only).
     *  @param hop_period    The number of symbols between each hop (LoRa only).
     *  @param iq_inverted   Inverts IQ signals (LoRa only)
     *                          FSK : N/A (set to 0).
     *                          LoRa: [0: not inverted, 1: inverted]
     *  @param timeout       The transmission timeout [us].
     */
    virtual void set_tx_config(radio_modems_t modem, int8_t power, uint32_t fdev,
                              uint32_t bandwidth, uint32_t datarate,
                              uint8_t coderate, uint16_t preamble_len,
                              bool fix_len, bool crc_on, bool freq_hop_on,
                              uint8_t hop_period, bool iq_inverted, uint32_t timeout) = 0;

    /**
     *  Sends the buffer of size
     *
     *  Prepares the packet to be sent and sets the radio in transmission.
     *
     *  @param buffer        A pointer to the buffer.
     *  @param size          The buffer size.
     */
    virtual void send(uint8_t *buffer, uint8_t size) = 0;

    /**
     *  Sets the radio in reception mode for a given time.
     *
     *  If the timeout is set to 0, it essentially puts the receiver in continuous mode and it should
     *  be treated as if in continuous mode. However, an appropriate way to set the receiver in continuous mode is
     *  to use the `set_rx_config()` API.
     *
     *  @param timeout       Reception timeout [ms].
     *
     */
    virtual void receive(uint32_t timeout) = 0;

    /**
     *  Sets the carrier frequency
     *
     *  @param freq          Channel RF frequency.
     */
    virtual void set_channel(uint32_t freq) = 0;

    /**
     *  Generates a 32 bit random value based on the RSSI readings.
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
     *  @param modem         The radio modem to be used [0: FSK, 1: LoRa].
     *  @param max           The maximum payload length in bytes.
     */
    virtual void set_max_payload_length(radio_modems_t modem, uint8_t max) = 0;

    /**
     *  Sets the network to public or private.
     *
     *  Updates the sync byte. Applies to LoRa modem only.
     *
     *  @param enable        If true, it enables a public network.
     */
    virtual void set_public_network(bool enable) = 0;

    /**
     *  Computes the packet time on air for the given payload.
     *
     *  \remark This can only be called once `SetRxConfig` or `SetTxConfig` have been called.
     *
     *  @param modem         The radio modem to be used [0: FSK, 1: LoRa].
     *  @param pkt_len       The packet payload length.
     *  @return              The computed `airTime` for the given packet payload length.
     */
    virtual uint32_t time_on_air(radio_modems_t modem, uint8_t pkt_len) = 0;

    /**
     * Performs carrier sensing.
     *
     * Checks for a certain time if the RSSI is above a given threshold.
     * This threshold determines whether or not there is a transmission going on
     * in the channel already.
     *
     * @param modem                     The type of the radio modem.
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
     *  Sets the radio in CAD mode.
     *
     */
    virtual void start_cad(void) = 0;

    /**
     *  Checks whether the given RF is in range.
     *
     *  @param frequency       The frequency to be checked.
     */
    virtual bool check_rf_frequency(uint32_t frequency) = 0;

    /** Sets the radio in continuous wave transmission mode.
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
     * Releases the exclusive access to this radio.
     */
    virtual void unlock(void) = 0;
};

#endif // LORARADIO_H_
