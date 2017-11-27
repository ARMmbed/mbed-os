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

#include "lorawan/system/LoRaWANTimer.h"
#include "lorawan/lorastack/phy/lora_phy_ds.h"
#include "netsocket/LoRaRadio.h"

class LoRaPHY {

public:
    LoRaPHY();
    virtual ~LoRaPHY();

    void set_radio_instance(LoRaRadio& radio);

    void put_radio_to_sleep(void);

    void put_radio_to_standby(void);

    void setup_rx_window(bool is_rx_continuous, uint32_t max_rx_window);

    void setup_tx_cont_wave_mode(uint16_t timeout, uint32_t frequency,
                                          uint8_t power);

    void handle_send(uint8_t *buf, uint8_t size);

    void setup_public_network_mode(bool set);

    uint32_t get_radio_rng();

    /*!
     * \brief The function gets a value of a specific PHY attribute.
     *
     * \param [in] getPhy A pointer to the function parameters.
     *
     * \retval A structure containing the PHY parameter.
     */
    virtual PhyParam_t get_phy_params(GetPhyParams_t* getPhy ) = 0;

    /*!
     * \brief Updates the last TX done parameters of the current channel.
     *
     * \param [in] txDone A pointer to the function parameters.
     */
    virtual void set_band_tx_done(SetBandTxDoneParams_t* txDone ) = 0;

    /*!
     * \brief Initializes the channels masks and the channels.
     *
     * \param [in] type Sets the initialization type.
     */
    virtual void load_defaults(InitType_t type ) = 0;

    /*!
     * \brief Verifies a parameter.
     *
     * \param [in] verify A pointer to the function parameters.
     *
     * \param [in] phyAttribute The attribute for which the verification is needed.
     *
     * \retval True, if the parameter is valid.
     */
   virtual bool verify(VerifyParams_t* verify, PhyAttribute_t phyAttribute ) = 0;

    /*!
     * \brief The function parses the input buffer and sets up the channels of the CF list.
     *
     * \param [in] applyCFList A pointer to the function parameters.
     */
   virtual void apply_cf_list(ApplyCFListParams_t* applyCFList ) = 0;

    /*!
     * \brief Sets a channels mask.
     *
     * \param [in] chanMaskSet A pointer to the function parameters.
     *
     * \retval True, if the channels mask could be set.
     */
    virtual bool set_channel_mask(ChanMaskSetParams_t* chanMaskSet ) = 0;

    /*!
     * \brief Calculates the next datarate to set, when ADR is on or off.
     *
     * \param [in] adrNext A pointer to the function parameters.
     *
     * \param [out] drOut The calculated datarate for the next TX.
     *
     * \param [out] txPowOut The TX power for the next TX.
     *
     * \param [out] adrAckCounter The calculated ADR acknowledgement counter.
     *
     * \retval True, if an ADR request should be performed.
     */
    virtual bool get_next_ADR(AdrNextParams_t* adrNext, int8_t* drOut,
                               int8_t* txPowOut, uint32_t* adrAckCounter ) = 0;

    /*!
     * \brief Configuration of the RX windows.
     *
     * \param [in] rxConfig A pointer to the function parameters.
     *
     * \param [out] datarate The datarate index set.
     *
     * \retval True, if the configuration was applied successfully.
     */
    virtual bool rx_config(RxConfigParams_t* rxConfig, int8_t* datarate ) = 0;

    /*
     * RX window precise timing
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
     * \param [in] datarate     The RX window datarate index to be used.
     *
     * \param [in] minRxSymbols The minimum number of symbols required to detect an RX frame.
     *
     * \param [in] rxError      The maximum timing error of the receiver in milliseconds.
     *                          The receiver will turn on in a [-rxError : +rxError] ms
     *                          interval around RxOffset.
     *
     * \param [out] rxConfigParams Returns the updated WindowTimeout and WindowOffset fields.
     *
     */
    virtual void compute_rx_win_params(int8_t datarate,
                                                 uint8_t minRxSymbols,
                                                 uint32_t rxError,
                                                 RxConfigParams_t *rxConfigParams) = 0;
    /*!
     * \brief TX configuration.
     *
     * \param [in] txConfig A pointer to the function parameters.
     *
     * \param [out] txPower The TX power index set.
     *
     * \param [out] txTimeOnAir The time-on-air of the frame.
     *
     * \retval True, if the configuration was applied successfully.
     */
    virtual bool tx_config(TxConfigParams_t* txConfig, int8_t* txPower,
                                TimerTime_t* txTimeOnAir ) = 0;

    /*!
     * \brief The function processes a Link ADR Request.
     *
     * \param [in] linkAdrReq A pointer to the function parameters.
     *
     * \param [out] drOut The datarate applied.
     *
     * \param [out] txPowOut The TX power applied.
     *
     * \param [out] nbRepOut The number of repetitions to apply.
     *
     * \param [out] nbBytesParsed The number of bytes parsed.
     *
     * \retval The status of the operation, according to the LoRaMAC specification.
     */
    virtual uint8_t link_ADR_request(LinkAdrReqParams_t* linkAdrReq,
                                     int8_t* drOut, int8_t* txPowOut,
                                     uint8_t* nbRepOut,
                                     uint8_t* nbBytesParsed ) = 0;

    /*!
     * \brief The function processes a RX Parameter Setup Request.
     *
     * \param [in] rxParamSetupReq A pointer to the function parameters.
     *
     * \retval The status of the operation, according to the LoRaMAC specification.
     */
    virtual uint8_t setup_rx_params(RxParamSetupReqParams_t* rxParamSetupReq ) = 0;

    /*!
     * \brief The function processes a New Channel Request.
     *
     * \param [in] newChannelReq A pointer to the function parameters.
     *
     * \retval The status of the operation, according to the LoRaMAC specification.
     */
    virtual uint8_t request_new_channel(NewChannelReqParams_t* newChannelReq ) = 0;

    /*!
     * \brief The function processes a TX ParamSetup Request.
     *
     * \param [in] txParamSetupReq A pointer to the function parameters.
     *
     * \retval The status of the operation, according to the LoRaMAC specification.
     *         Returns -1, if the functionality is not implemented. In this case, the end node
     *         shall ignore the command.
     */
    virtual int8_t setup_tx_params(TxParamSetupReqParams_t* txParamSetupReq ) = 0;

    /*!
     * \brief The function processes a DlChannel Request.
     *
     * \param [in] dlChannelReq A pointer to the function parameters.
     *
     * \retval The status of the operation, according to the LoRaMAC specification.
     */
    virtual uint8_t dl_channel_request(DlChannelReqParams_t* dlChannelReq ) = 0;

    /*!
     * \brief Alternates the datarate of the channel for the join request.
     *
     * \param [in] alternateDr A pointer to the function parameters.
     *
     * \retval The datarate to apply.
     */
    virtual int8_t get_alternate_DR(AlternateDrParams_t* alternateDr ) = 0;

    /*!
     * \brief Calculates the back-off time.
     *
     * \param [in] calcBackOff A pointer to the function parameters.
     */
    virtual void calculate_backoff(CalcBackOffParams_t* calcBackOff ) = 0;

    /*!
     * \brief Searches and sets the next random available channel.
     *
     * \param [in]  nextChanParams Parameters for the next channel.
     *
     * \param [out] channel The next channel to use for TX.
     *
     * \param [out] time The time to wait for the next transmission according to the duty cycle.
     *
     * \param [out] aggregatedTimeOff Updates the aggregated time off.
     *
     * \retval Function status [1: OK, 0: Unable to find a channel on the current datarate].
     */
    virtual bool set_next_channel(NextChanParams_t* nextChanParams,
                                   uint8_t* channel, TimerTime_t* time,
                                   TimerTime_t* aggregatedTimeOff ) = 0;

    /*!
     * \brief Adds a channel.
     *
     * \param [in] channelAdd A pointer to the function parameters.
     *
     * \retval The status of the operation.
     */
    virtual LoRaMacStatus_t add_channel(ChannelAddParams_t* channelAdd ) = 0;

    /*!
     * \brief Removes a channel.
     *
     * \param [in] channelRemove A pointer to the function parameters.
     *
     * \retval True, if the channel was removed successfully.
     */
    virtual bool remove_channel(ChannelRemoveParams_t* channelRemove ) = 0;

    /*!
     * \brief Sets the radio into continuous wave mode.
     *
     * \param [in] continuousWave A pointer to the function parameters.
     */
    virtual void set_tx_cont_mode(ContinuousWaveParams_t* continuousWave ) = 0;

    /*!
     * \brief Computes new datarate according to the given offset
     *
     * \param [in] downlinkDwellTime The downlink dwell time configuration. 0: No limit, 1: 400ms
     *
     * \param [in] dr The current datarate.
     *
     * \param [in] drOffset The offset to be applied.
     *
     * \retval newDr The computed datarate.
     */
    virtual uint8_t apply_DR_offset(uint8_t downlinkDwellTime, int8_t dr, int8_t drOffset ) = 0;

protected:
    LoRaRadio *_radio;

    typedef struct sRegionCommonLinkAdrParams
    {
        /*!
         * The number of repetitions.
         */
        uint8_t NbRep;
        /*!
         * Datarate.
         */
        int8_t Datarate;
        /*!
         * TX power.
         */
        int8_t TxPower;
        /*!
         * Channels mask control field.
         */
        uint8_t ChMaskCtrl;
        /*!
         * Channels mask field.
         */
        uint16_t ChMask;
    }RegionCommonLinkAdrParams_t;

    typedef struct sRegionCommonLinkAdrReqVerifyParams
    {
        /*!
         * The current status of the AdrLinkRequest.
         */
        uint8_t Status;
        /*!
         * Set to true, if ADR is enabled.
         */
        bool AdrEnabled;
        /*!
         * The datarate the AdrLinkRequest wants to set.
         */
        int8_t Datarate;
        /*!
         * The TX power the AdrLinkRequest wants to set.
         */
        int8_t TxPower;
        /*!
         * The number of repetitions the AdrLinkRequest wants to set.
         */
        uint8_t NbRep;
        /*!
         * The current datarate the node is using.
         */
        int8_t CurrentDatarate;
        /*!
         * The current TX power the node is using.
         */
        int8_t CurrentTxPower;
        /*!
         * The current number of repetitions the node is using.
         */
        int8_t CurrentNbRep;
        /*!
         * The number of channels.
         */
        uint8_t NbChannels;
        /*!
         * A pointer to the first element of the channels mask.
         */
        uint16_t* ChannelsMask;
        /*!
         * The minimum possible datarate.
         */
        int8_t MinDatarate;
        /*!
         * The maximum possible datarate.
         */
        int8_t MaxDatarate;
        /*!
         * A pointer to the channels.
         */
        ChannelParams_t* Channels;
        /*!
         * The minimum possible TX power.
         */
        int8_t MinTxPower;
        /*!
         * The maximum possible TX power.
         */
        int8_t MaxTxPower;
    }RegionCommonLinkAdrReqVerifyParams_t;

    typedef struct sRegionCommonCalcBackOffParams
    {
        /*!
         * A pointer to region specific channels.
         */
        ChannelParams_t* Channels;
        /*!
         * A pointer to region specific bands.
         */
        Band_t* Bands;
        /*!
         * Set to true, if the last uplink was a join request.
         */
        bool LastTxIsJoinRequest;
        /*!
         * Set to true, if the node is joined.
         */
        bool Joined;
        /*!
         * Set to true, if the duty cycle is enabled.
         */
        bool DutyCycleEnabled;
        /*!
         * The current channel.
         */
        uint8_t Channel;
        /*!
         * The elapsed time since initialization.
         */
        TimerTime_t ElapsedTime;
        /*!
         * The time on air of the last TX frame.
         */
        TimerTime_t TxTimeOnAir;
    }RegionCommonCalcBackOffParams_t;

    /*!
     * \brief Calculates the join duty cycle.
     *        This is a generic function and valid for all regions.
     *
     * \param [in] elapsedTime The time elapsed since starting the device.
     *
     * \retval Duty cycle restriction.
     */
    uint16_t get_join_DC( TimerTime_t elapsedTime );

    /*!
     * \brief Verifies, if a value is in a given range.
     *        This is a generic function and valid for all regions.
     *
     * \param [in] value The value to verify, if it is in range.
     *
     * \param [in] min The minimum possible value.
     *
     * \param [in] max The maximum possible value.
     *
     * \retval 1 if the value is in range, otherwise 0.
     */
    uint8_t val_in_range( int8_t value, int8_t min, int8_t max );

    /*!
     * \brief Verifies, if a datarate is available on an active channel.
     *        This is a generic function and valid for all regions.
     *
     * \param [in] nbChannels The number of channels.
     *
     * \param [in] channelsMask The channels mask of the region.
     *
     * \param [in] dr The datarate to verify.
     *
     * \param [in] minDr The minimum datarate.
     *
     * \param [in] maxDr The maximum datarate.
     *
     * \param [in] channels The channels of the region.
     *
     * \retval True if the datarate is supported, false if not.
     */
    bool verify_channel_DR( uint8_t nbChannels, uint16_t* channelsMask, int8_t dr,
                                int8_t minDr, int8_t maxDr, ChannelParams_t* channels );

    /*!
     * \brief Disables a channel in a given channels mask.
     *        This is a generic function and valid for all regions.
     *
     * \param [in] channelsMask The channels mask of the region.
     *
     * \param [in] id The ID of the channels mask to disable.
     *
     * \param [in] maxChannels The maximum number of channels.
     *
     * \retval True if the channel could be disabled, false if not.
     */
    bool disable_channel( uint16_t* channelsMask, uint8_t id, uint8_t maxChannels );

    /*!
     * \brief Counts the number of active channels in a given channels mask.
     *        This is a generic function and valid for all regions.
     *
     * \param [in] channelsMask The channels mask of the region.
     *
     * \param [in] startIdx The start index.
     *
     * \param [in] stopIdx The stop index (the channels of this index will not be counted).
     *
     * \retval The number of active channels.
     */
    uint8_t num_active_channels( uint16_t* channelsMask, uint8_t startIdx, uint8_t stopIdx );

    /*!
     * \brief Copy a channels mask.
     *        This is a generic function and valid for all regions.
     *
     * \param [in] channelsMaskDest The destination channels mask.
     *
     * \param [in] channelsMaskSrc The source channels mask.
     *
     * \param [in] len The index length to copy.
     */
    void copy_channel_mask( uint16_t* channelsMaskDest, uint16_t* channelsMaskSrc, uint8_t len );

    /*!
     * \brief Sets the last TX done property.
     *        This is a generic function and valid for all regions.
     *
     * \param [in] joined Set to true, if the node has joined the network
     *
     * \param [in] band The band to be updated.
     *
     * \param [in] lastTxDone The time of the last TX done.
     */
    void set_last_tx_done( bool joined, Band_t* band, TimerTime_t lastTxDone );

    /*!
     * \brief Updates the time-offs of the bands.
     *        This is a generic function and valid for all regions.
     *
     * \param [in] joined Set to true, if the node has joined the network
     *
     * \param [in] dutyCycle Set to true, if the duty cycle is enabled.
     *
     * \param [in] bands A pointer to the bands.
     *
     * \param [in] nbBands The number of bands available.
     *
     * \retval The time which must be waited to perform the next uplink.
     */
    TimerTime_t update_band_timeoff( bool joined, bool dutyCycle, Band_t* bands, uint8_t nbBands );

    /*!
     * \brief Parses the parameter of an LinkAdrRequest.
     *        This is a generic function and valid for all regions.
     *
     * \param [in] payload A pointer to the payload containing the MAC commands. The payload
     *                     must contain the CMD identifier, followed by the parameters.
     *
     * \param [out] parseLinkAdr The function fills the structure with the ADR parameters.
     *
     * \retval The length of the ADR request, if a request was found. Otherwise, the
     *         function returns 0.
     */
    uint8_t parse_link_ADR_req( uint8_t* payload, RegionCommonLinkAdrParams_t* parseLinkAdr );

    /*!
     * \brief Verifies and updates the datarate, the TX power and the number of repetitions
     *        of a LinkAdrRequest. This also depends on the ADR configuration.
     *
     * \param [in] verifyParams A pointer to a structure containing the input parameters.
     *
     * \param [out] dr The updated datarate.
     *
     * \param [out] txPow The updated TX power.
     *
     * \param [out] nbRep The updated number of repetitions.
     *
     * \retval The status according to the LinkAdrRequest definition.
     */
    uint8_t verify_link_ADR_req( RegionCommonLinkAdrReqVerifyParams_t* verifyParams, int8_t* dr, int8_t* txPow, uint8_t* nbRep );

    /*!
     * \brief Computes the symbol time for LoRa modulation.
     *
     * \param [in] phyDr The physical datarate to use.
     *
     * \param [in] bandwidth The bandwidth to use.
     *
     * \retval The symbol time.
     */
    double compute_symb_timeout_lora( uint8_t phyDr, uint32_t bandwidth );

    /*!
     * \brief Computes the symbol time for FSK modulation.
     *
     * \param [in] phyDr The physical datarate to use.
     *
     * \retval The symbol time.
     */
    double compute_symb_timeout_fsk( uint8_t phyDr );

    /*!
     * \brief Computes the RX window timeout and the RX window offset.
     *
     * \param [in] tSymbol The symbol timeout.
     *
     * \param [in] minRxSymbols The minimum required number of symbols to detect an RX frame.
     *
     * \param [in] rxError The system maximum timing error of the receiver in milliseconds
     *                     The receiver will turn on in a [-rxError : +rxError] ms interval around RxOffset.
     *
     * \param [in] wakeUpTime The wakeup time of the system.
     *
     * \param [out] windowTimeout The RX window timeout.
     *
     * \param [out] windowOffset The RX window time offset to be applied to the RX delay.
     */
    void get_rx_window_params( double tSymbol, uint8_t minRxSymbols, uint32_t rxError, uint32_t wakeUpTime, uint32_t* windowTimeout, int32_t* windowOffset );

    /*!
     * \brief Computes the txPower, based on the max EIRP and the antenna gain.
     *
     * \param [in] txPowerIndex The TX power index.
     *
     * \param [in] maxEirp The maximum EIRP.
     *
     * \param [in] antennaGain The antenna gain.
     *
     * \retval The physical TX power.
     */
    int8_t compute_tx_power( int8_t txPowerIndex, float maxEirp, float antennaGain );

    /*!
     * \brief Provides a random number in the range provided.
     *
     * \param [in] min lower boundary
     * \param [in] max upper boundary
     */
    int32_t get_random(int32_t min, int32_t max);

    /*!
     * \brief Calculates the duty cycle for the current band.
     *
     * \param [in] calcBackOffParams A pointer to the input parameters.
     */
    void get_DC_backoff( RegionCommonCalcBackOffParams_t* calcBackOffParams );
};

#endif /* MBED_OS_LORAPHY_BASE_ */
