/**
 *  @file LoRaPHY.h
 *
 *  @brief An abstract class providing radio object to children and
 *         provide base for implementing LoRa PHY layer
 *
 *  \code
 *   ______                              _
 *  / _____)             _              | |
 * ( (____  _____ ____ _| |_ _____  ____| |__
 *  \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *  _____) ) ____| | | || |_| ____( (___| | | |
 * (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *   (C)2013 Semtech
 *  ___ _____ _   ___ _  _____ ___  ___  ___ ___
 * / __|_   _/_\ / __| |/ / __/ _ \| _ \/ __| __|
 * \__ \ | |/ _ \ (__| ' <| _| (_) |   / (__| _|
 * |___/ |_/_/ \_\___|_|\_\_| \___/|_|_\\___|___|
 * embedded.connectivity.solutions===============
 *
 * \endcode
 *
 * Description: LoRa PHY layer
 *
 * License: Revised BSD License, see LICENSE.TXT file include in the project
 *
 * Maintainer: Miguel Luis ( Semtech ), Gregory Cristian ( Semtech ) and Daniel Jaeckle ( STACKFORCE )
 *
 * Copyright (c) 2017, Arm Limited and affiliates.
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef MBED_OS_LORAPHY_BASE_
#define MBED_OS_LORAPHY_BASE_

#include "platform/NonCopyable.h"

#include "system/LoRaWANTimer.h"
#include "LoRaRadio.h"
#include "lora_phy_ds.h"

class LoRaPHY : private mbed::NonCopyable<LoRaPHY> {

public:
    virtual ~LoRaPHY();

    /** Stores a reference to Radio object.
     *
     * Application is responsible for constructing a 'LoRaRadio' object
     * which is passed down to the PHY layer.
     *
     * @param radio    a reference to radio driver object
     */
    void set_radio_instance(LoRaRadio& radio);

    /** Puts radio in sleep mode.
     *
     * Requests the radio driver to enter sleep mode.
     */
    void put_radio_to_sleep(void);

    /** Puts radio in standby mode.
     *
     * Requests the radio driver to enter standby mode.
     */
    void put_radio_to_standby(void);

    /** Puts radio in receive mode.
     *
     * Requests the radio driver to enter receive mode for a given time or to
     * enter continuous reception mode.
     *
     * @param is_rx_continuous    if true, sets the radio to enter continuous
     *                            reception mode.
     *
     * @param max_rx_window       duration of receive window
     */
    void setup_rx_window(bool is_rx_continuous, uint32_t max_rx_window);

    /** Delegates MAC layer request to transmit packet.
     *
     * @param buf    a pointer to the data which needs to be transmitted
     *
     * @param size   size of the data in bytes
     */
    void handle_send(uint8_t *buf, uint8_t size);

    /** Enables/Disables public network mode.
     *
     * Public and private LoRaWAN network constitute different preambles and
     * Net IDs. This API isused to tell the radio which network mode is in use.
     *
     * @param set    true or false
     */
    void setup_public_network_mode(bool set);

    /** Provides a random number from radio.
     *
     * Returns a 32-bit random unsigned integer value based upon RSSI
     * measurements.
     *
     * @return    a 32-bit long random number
     *
     */
    uint32_t get_radio_rng();

    /**
     * @brief calculate_backoff Calculates and applies duty cycle back-off time.
     *                          Explicitly updates the band time-off.
     *
     * @param joined                Set to true, if the node has already joined a network, otherwise false.
     * @param last_tx_was_join_req  Set to true, if the last uplink was a join request.
     * @param dc_enabled            Set to true, if the duty cycle is enabled, otherwise false.
     * @param channel               The current channel index.
     * @param elapsed_time          Elapsed time since the start of the node.
     * @param tx_toa                Time-on-air of the last transmission.
     */
    void calculate_backoff(bool joined, bool last_tx_was_join_req, bool dc_enabled, uint8_t channel,
                           lorawan_time_t elapsed_time, lorawan_time_t tx_toa);

    /**
      * Tests if a channel is on or off in the channel mask
      */
    bool mask_bit_test(const uint16_t *mask, unsigned bit);

    /**
      * Tests if a channel is on or off in the channel mask
      */
    void mask_bit_set(uint16_t *mask, unsigned bit);

    /**
      * Tests if a channel is on or off in the channel mask
      */
    void mask_bit_clear(uint16_t *mask, unsigned bit);

    /** Entertain a new channel request MAC command.
     *
     * MAC command subsystem processes the new channel request coming form
     * the network server and then MAC layer asks the PHY layer to entertain
     * the request.
     *
     * @param channel_id The channel ID.
     * @param new_channel A pointer to the new channel's parameters.
     *
     * @return bit mask, according to the LoRaWAN spec 1.0.2.
     */
    virtual uint8_t request_new_channel(int8_t channel_id, channel_params_t* new_channel);

    /** Process PHY layer state after a successful transmission.
     * @brief set_last_tx_done Updates times of the last transmission for the particular channel and
     *                         band upon which last transmission took place.
     * @param channel The channel in use.
     * @param joined Boolean telling if node has joined the network.
     * @param last_tx_done_time The last TX done time.
     */
    virtual void set_last_tx_done(uint8_t channel, bool joined, lorawan_time_t last_tx_done_time);

    /** Enables default channels only.
     *
     * Falls back to a channel mask where only default channels are enabled, all
     * other channels are disabled.
     */
    virtual void restore_default_channels();

    /** Processes the incoming CF-list.
     *
     * Handles the payload containing CF-list and enables channels defined
     * therein.
     *
     * @param payload Payload to process.
     * @param size Size of the payload.
     *
     */
    virtual void apply_cf_list(const uint8_t* payload, uint8_t size);

    /** Calculates the next datarate to set, when ADR is on or off.
     *
     * @param restore_channel_mask    A boolean set restore channel mask in case
     *                                of failure.
     *
     * @param dr_out                  The calculated datarate for the next TX.
     *
     * @param tx_power_out            The TX power for the next TX.
     *
     * @param adr_ack_counter         The calculated ADR acknowledgement counter.
     *
     * @return True, if an ADR request should be performed.
     */
    bool get_next_ADR(bool restore_channel_mask, int8_t& dr_out,
                      int8_t& tx_power_out, uint32_t& adr_ack_counter);

    /** Configure radio reception.
     *
     * @param [in] config    A pointer to the RX configuration.
     *
     * @return True, if the configuration was applied successfully.
     */
    virtual bool rx_config(rx_config_params_t* config);

    /** Computing Receive Windows
     *
     * For more details please consult the following document, chapter 3.1.2.
     * http://www.semtech.com/images/datasheet/SX1272_settings_for_LoRaWAN_v2.0.pdf
     * or
     * http://www.semtech.com/images/datasheet/SX1276_settings_for_LoRaWAN_v2.0.pdf
     *
     *                 Downlink start: T = Tx + 1s (+/- 20 us)
     *                            |
     *             TRxEarly       |        TRxLate
     *                |           |           |
     *                |           |           +---+---+---+---+---+---+---+---+
     *                |           |           |       Latest Rx window        |
     *                |           |           +---+---+---+---+---+---+---+---+
     *                |           |           |
     *                +---+---+---+---+---+---+---+---+
     *                |       Earliest Rx window      |
     *                +---+---+---+---+---+---+---+---+
     *                            |
     *                            +---+---+---+---+---+---+---+---+
     *Downlink preamble 8 symbols |   |   |   |   |   |   |   |   |
     *                            +---+---+---+---+---+---+---+---+
     *
     *                     Worst case Rx window timings
     *
     * TRxLate  = DEFAULT_MIN_RX_SYMBOLS * tSymbol - RADIO_WAKEUP_TIME
     * TRxEarly = 8 - DEFAULT_MIN_RX_SYMBOLS * tSymbol - RxWindowTimeout - RADIO_WAKEUP_TIME
     *
     * TRxLate - TRxEarly = 2 * DEFAULT_SYSTEM_MAX_RX_ERROR
     *
     * RxOffset = ( TRxLate + TRxEarly ) / 2
     *
     * RxWindowTimeout = ( 2 * DEFAULT_MIN_RX_SYMBOLS - 8 ) * tSymbol + 2 * DEFAULT_SYSTEM_MAX_RX_ERROR
     * RxOffset = 4 * tSymbol - RxWindowTimeout / 2 - RADIO_WAKE_UP_TIME
     *
     * The minimum value of RxWindowTimeout must be 5 symbols which implies that the system always tolerates at least an error of 1.5 * tSymbol.
     */
    /*!
     * Computes the RX window timeout and offset.
     *
     * @param [in] datarate         The RX window datarate index to be used.
     *
     * @param [in] min_rx_symbols   The minimum number of symbols required to
     *                              detect an RX frame.
     *
     * @param [in] rx_error         The maximum timing error of the receiver
     *                              in milliseconds. The receiver will turn on
     *                              in a [-rxError : +rxError] ms interval around
     *                              RxOffset.
     *
     * @param [out] rx_conf_params  Pointer to the structure that needs to be
     *                              filled with receive window parameters.
     *
     */
    virtual void compute_rx_win_params(int8_t datarate, uint8_t min_rx_symbols,
                                       uint32_t rx_error,
                                       rx_config_params_t *rx_conf_params);

    /** Configure radio transmission.
     *
     * @param [in]  tx_config    Structure containing tx parameters.
     *
     * @param [out] tx_power     The TX power which will be set.
     *
     * @param [out] tx_toa       The time-on-air of the frame.
     *
     * @return True, if the configuration was applied successfully.
     */
    virtual bool tx_config(tx_config_params_t* tx_config, int8_t* tx_power,
                           lorawan_time_t* tx_toa);

    /** Processes a Link ADR Request.
     *
     * @param [in]  params          A pointer ADR request parameters.
     *
     * @param [out] dr_out          The datarate applied.
     *
     * @param [out] tx_power_out    The TX power applied.
     *
     * @param [out] nb_rep_out      The number of repetitions to apply.
     *
     * @param [out] nb_bytes_parsed The number of bytes parsed.
     *
     * @return The status of the operation, according to the LoRaMAC specification.
     */
    virtual uint8_t link_ADR_request(adr_req_params_t* params,
                                     int8_t* dr_out, int8_t* tx_power_out,
                                     uint8_t* nb_rep_out,
                                     uint8_t* nb_bytes_parsed);

    /** Accept or rejects RxParamSetupReq MAC command
     *
     * The function processes a RX parameter setup request in response to
     * server MAC command for RX setup.
     *
     * @param [in] params    A pointer to rx parameter setup request.
     *
     * @return The status of the operation, according to the LoRaWAN specification.
     */
    virtual uint8_t accept_rx_param_setup_req(rx_param_setup_req_t* params);

    /**
     * @brief accept_tx_param_setup_req Makes decision whether to accept or reject TxParamSetupReq MAC command.
     *
     * @param ul_dwell_time The uplink dwell time.
     * @param dl_dwell_time The downlink dwell time.
     *
     * @return True to let the MAC know that the request is
     *         accepted and MAC can apply TX parameters received
     *         form Network Server. Otherwise false is returned.
     */
    virtual bool accept_tx_param_setup_req(uint8_t ul_dwell_time, uint8_t dl_dwell_time);

    /** Processes a DlChannelReq MAC command.
     *
     * @param channel_id The channel ID to add the frequency.
     * @param rx1_frequency The alternative frequency for the Rx1 window.
     *
     * @return The status of the operation, according to the LoRaWAN specification.
     */
    virtual uint8_t dl_channel_request(uint8_t channel_id, uint32_t rx1_frequency);

    /** Alternates the datarate of the channel for the join request.
     *
     * @param nb_trials    Number of trials to be made on one given data rate.
     *
     * @return             The datarate to apply .
     */
    virtual int8_t get_alternate_DR(uint8_t nb_trials);

    /** Searches and sets the next available channel.
     *
     * If there are multiple channels found available, one of them is selected
     * randomly.
     *
     * @param [in]  nextChanParams Parameters for the next channel.
     *
     * @param [out] channel The next channel to use for TX.
     *
     * @param [out] time The time to wait for the next transmission according to the duty cycle.
     *
     * @param [out] aggregatedTimeOff Updates the aggregated time off.
     *
     * @return Function status [1: OK, 0: Unable to find a channel on the current datarate].
     */
    virtual lorawan_status_t set_next_channel(channel_selection_params_t* nextChanParams,
                                              uint8_t* channel, lorawan_time_t* time,
                                              lorawan_time_t* aggregatedTimeOff);

    /** Adds a channel to the channel list.
     *
     * Verifies the channel parameters and if everything is found legitimate,
     * adds that particular channel to the channel list and updates the channel
     * mask.
     *
     * @param [in] new_channel A pointer to the parameters for the new channel.
     * @param [in] id          Channel ID
     *
     * @return LORAWAN_STATUS_OK if everything goes fine, negative error code
     *         otherwise.
     */
    virtual lorawan_status_t add_channel(const channel_params_t* new_channel, uint8_t id);

    /** Removes a channel from the channel list.
     *
     * @param [in] channel_id Index of the channel to be removed
     *
     * @return True, if the channel was removed successfully.
     */
    virtual bool remove_channel(uint8_t channel_id);

    /** Puts the radio into continuous wave mode.
     *
     * @param [in] continuous_wave   A pointer to the function parameters.
     *
     * @param [in] frequency         Frequency to transmit at
     */
    virtual void set_tx_cont_mode(cw_mode_params_t* continuous_wave,
                                  uint32_t frequency = 0);

    /** Computes new data rate according to the given offset
     *
     * @param [in] dr The current datarate.
     *
     * @param [in] dr_offset The offset to be applied.
     *
     * @return     The computed datarate.
     */
    virtual uint8_t apply_DR_offset(int8_t dr, int8_t dr_offset);

    /**
     * @brief reset_to_default_values resets some parameters to default values
     * @param params Pointer to MAC protocol parameters which will be reset
     * @param init If true, most of the values will be modified
     */
    void reset_to_default_values(loramac_protocol_params *params, bool init = false);

public:
    /**
     * @brief get_next_lower_tx_datarate Gets the next lower datarate
     * @param datarate Current TX datarate
     * @return Lower datarate than given one or minimum if lower cannot be found anymore
     */
    int8_t get_next_lower_tx_datarate(int8_t datarate);

    /**
     * @brief get_minimum_rx_datarate Gets the minimum RX datarate supported by a device
     * @return Minimum RX datarate
     */
    uint8_t get_minimum_rx_datarate();

    /**
     * @brief get_minimum_tx_datarate Gets the minimum TX datarate supported by a device
     * @return Minimum TX datarate
     */
    uint8_t get_minimum_tx_datarate();

    /**
     * @brief get_default_tx_datarate Gets the default TX datarate
     * @return default TX datarate
     */
    uint8_t get_default_tx_datarate();

    /**
     * @brief get_default_tx_power Gets the default TX power
     * @return Default TX power
     */
    uint8_t get_default_tx_power();

    /**
     * @brief get_max_payload Gets maximum amount in bytes which device can send
     * @param datarate A datarate to use
     * @param use_repeater If true repeater table is used, otherwise payloads table is used
     * @return Maximum number of bytes for payload
     */
    uint8_t get_max_payload(uint8_t datarate, bool use_repeater = false);

    /**
     * @brief get_maximum_frame_counter_gap Gets maximum frame counter gap
     * @return Maximum frame counter gap
     */
    uint16_t get_maximum_frame_counter_gap();

    /**
     * @brief get_ack_timeout Gets timeout value for ACK to be received
     * @return ACK timeout
     */
    uint32_t get_ack_timeout();

    /**
     * @brief get_default_rx2_frequency Gets default RX2 frequency
     * @return RX2 frequency
     */
    uint32_t get_default_rx2_frequency();

    /**
     * @brief get_default_rx2_datarate Gets default RX2 datarate
     * @return RX2 datarate
     */
    uint8_t get_default_rx2_datarate();

    /**
     * @brief get_channel_mask Gets the channel mask
     * @param get_default If true the default mask is returned, otherwise the current mask is returned
     * @return A channel mask
     */
    uint16_t* get_channel_mask(bool get_default = false);

    /**
     * @brief get_max_nb_channels Gets maximum number of channels supported
     * @return Number of channels
     */
    uint8_t get_max_nb_channels();

    /**
     * @brief get_phy_channels Gets PHY channels
     * @return PHY channels
     */
    channel_params_t* get_phy_channels();

    /**
     * @brief is_custom_channel_plan_supported Checks if custom channel plan is supported
     * @return True if custom channel plan is supported, false otherwise
     */
    bool is_custom_channel_plan_supported();

public: //Verifiers

    /**
     * @brief verify_rx_datarate Verifies that given RX datarate is valid
     * @param datarate Datarate to check
     * @return true if given datarate is valid, false otherwise
     */
    bool verify_rx_datarate(uint8_t datarate);

    /**
     * @brief verify_tx_datarate Verifies that given TX datarate is valid
     * @param datarate Datarate to check
     * @param use_default If true validation is done against default value
     * @return true if given datarate is valid, false otherwise
     */
    bool verify_tx_datarate(uint8_t datarate, bool use_default = false);

    /**
     * @brief verify_tx_power Verifies that given TX power is valid
     * @param tx_power Power to check
     * @return True if valid, false otherwise
     */
    bool verify_tx_power(uint8_t tx_power);

    /**
     * @brief verify_duty_cycle Verifies that given cycle is valid
     * @param cycle Cycle to check
     * @return True if valid, false otherwise
     */
    bool verify_duty_cycle(bool cycle);

    /**
     * @brief verify_nb_join_trials Verifies that given number of trials is valid
     * @param nb_join_trials Number to check
     * @return True if valid, false otherwise
     */
    bool verify_nb_join_trials(uint8_t nb_join_trials);

protected:
    LoRaPHY(LoRaWANTimeHandler &lora_time);

    /**
     * Looks up corresponding band for a frequency. Returns -1 if not in any band.
     */
    int lookup_band_for_frequency(uint32_t freq) const;

    /**
     * Verifies, if a frequency is within a given band.
     */
    virtual bool verify_frequency_for_band(uint32_t freq, uint8_t band) const;

    /**
     * Verifies, if a value is in a given range.
     */
    bool val_in_range(int8_t value, int8_t min, int8_t max);

    /**
     * Verifies, if a datarate is available on an active channel.
     */
    bool verify_channel_DR(uint8_t nbChannels, uint16_t* channelsMask, int8_t dr,
                           int8_t minDr, int8_t maxDr, channel_params_t* channels);

    /**
     * Disables a channel in a given channels mask.
     */
    bool disable_channel(uint16_t* channel_mask, uint8_t id, uint8_t max_channels);

    /**
     * Counts number of bits on in a given mask
     */
    uint8_t count_bits(uint16_t mask, uint8_t nb_bits);

    /**
     * Counts the number of active channels in a given channels mask.
     */
    uint8_t num_active_channels(uint16_t* channel_mask, uint8_t start_idx,
                                uint8_t stop_idx);

    /**
     * Copy channel masks.
     */
    void copy_channel_mask(uint16_t* dest_mask, uint16_t* src_mask, uint8_t len);

    /**
     * Updates the time-offs of the bands.
     */
    lorawan_time_t update_band_timeoff(bool joined, bool dutyCycle, band_t* bands,
                                       uint8_t nb_bands);

    /**
     * Parses the parameter of an LinkAdrRequest.
     */
    uint8_t parse_link_ADR_req(const uint8_t* payload, link_adr_params_t* adr_params);

    /**
     * Verifies and updates the datarate, the TX power and the number of repetitions
     * of a LinkAdrRequest.
     */
    uint8_t verify_link_ADR_req(verify_adr_params_t* verify_params, int8_t* dr,
                                int8_t* tx_pow, uint8_t* nb_rep);

    /**
     * Computes the symbol time for LoRa modulation.
     */
    double compute_symb_timeout_lora(uint8_t phy_dr, uint32_t bandwidth );

    /**
     * Computes the symbol time for FSK modulation.
     */
    double compute_symb_timeout_fsk(uint8_t phy_dr);

    /**
     * Computes the RX window timeout and the RX window offset.
     */
    void get_rx_window_params(double t_symbol, uint8_t min_rx_symbols,
                              uint32_t rx_error, uint32_t wakeup_time,
                              uint32_t* window_timeout, int32_t* window_offset);

    /**
     * Computes the txPower, based on the max EIRP and the antenna gain.
     */
    int8_t compute_tx_power(int8_t txPowerIndex, float maxEirp, float antennaGain);

    /**
     * Provides a random number in the range provided.
     */
    int32_t get_random(int32_t min, int32_t max);

    /**
     * Get next lower data rate
     */
    int8_t get_next_lower_dr(int8_t dr, int8_t min_dr);

    /**
     * Get channel bandwidth depending upon data rate table index
     */
    uint8_t get_bandwidth(uint8_t dr_index);

    uint8_t enabled_channel_count(bool joined, uint8_t datarate,
                                  const uint16_t *mask, uint8_t* enabledChannels,
                                  uint8_t* delayTx);

    bool is_datarate_supported(const int8_t datarate) const;

protected:
    LoRaRadio *_radio;
    LoRaWANTimeHandler &_lora_time;
    loraphy_params_t phy_params;
};

#endif /* MBED_OS_LORAPHY_BASE_ */
