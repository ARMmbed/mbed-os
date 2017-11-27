/**
 *  @file LoRaPHYEU868.h
 *
 *  @brief Implements LoRaPHY for European 868 MHz band
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
 *
 * License: Revised BSD License, see LICENSE.TXT file include in the project
 *
 * Maintainer: Miguel Luis ( Semtech ), Gregory Cristian ( Semtech ) and Daniel Jaeckle ( STACKFORCE )
 *
 * Copyright (c) 2017, Arm Limited and affiliates.
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef MBED_OS_LORAPHY_EU868_H_
#define MBED_OS_LORAPHY_EU868_H_

#include "LoRaPHY.h"
#include "netsocket/LoRaRadio.h"

/*!
 * LoRaMac maximum number of channels
 */
#define EU868_MAX_NB_CHANNELS                       16

/*!
 * Maximum number of bands
 */
#define EU868_MAX_NB_BANDS                          5

#define EU868_CHANNELS_MASK_SIZE                    1


class LoRaPHYEU868 : public LoRaPHY {

public:
    LoRaPHYEU868();
    virtual ~LoRaPHYEU868();

    /*!
     * \brief The function gets a value of a specific PHY attribute.
     *
     * \param [in] getPhy A pointer to the function parameters.
     *
     * \retval The structure containing the PHY parameter.
     */
    virtual PhyParam_t get_phy_params(GetPhyParams_t* getPhy );

    /*!
     * \brief Updates the last TX done parameters of the current channel.
     *
     * \param [in] txDone A pointer to the function parameters.
     */
    virtual void set_band_tx_done(SetBandTxDoneParams_t* txDone );

    /*!
     * \brief Initializes the channels masks and the channels.
     *
     * \param [in] type Sets the initialization type.
     */
    virtual void load_defaults(InitType_t type );

    /*!
     * \brief Verifies a parameter.
     *
     * \param [in] verify A pointer to the function parameters.
     *
     * \param [in] phyAttribute The attribute to verify.
     *
     * \retval True, if the parameter is valid.
     */
   virtual bool verify(VerifyParams_t* verify, PhyAttribute_t phyAttribute );

    /*!
     * \brief The function parses the input buffer and sets up the channels of the CF list.
     *
     * \param [in] applyCFList A pointer to the function parameters.
     */
   virtual void apply_cf_list(ApplyCFListParams_t* applyCFList );

    /*!
     * \brief Sets a channels mask.
     *
     * \param [in] chanMaskSet A pointer to the function parameters.
     *
     * \retval True, if the channels mask could be set.
     */
    virtual bool set_channel_mask(ChanMaskSetParams_t* chanMaskSet );

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
                               int8_t* txPowOut, uint32_t* adrAckCounter );

    /*!
     * \brief Configuration of the RX windows.
     *
     * \param [in] rxConfig A pointer to the function parameters.
     *
     * \param [out] datarate The datarate index set.
     *
     * \retval True, if the configuration was applied successfully.
     */
    virtual bool rx_config(RxConfigParams_t* rxConfig, int8_t* datarate );

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
     * The minimum value of RxWindowTimeout must be 5 symbols which implies that the system always tolerates at least an error of 1.5 * tSymbol
     */
    /*!
     * Computes the RX window timeout and offset.
     *
     * \param [in] datarate     The RX window datarate index to be used.
     *
     * \param [in] minRxSymbols The minimum number of symbols required to detect an RX frame.
     *
     * \param [in] rxError      The system maximum timing error of the receiver in milliseconds.
     *                          The receiver will turn on in a [-rxError : +rxError] ms
     *                          interval around RxOffset.
     *
     * \param [out] rxConfigParams Returns the updated WindowTimeout and WindowOffset fields.
     */
    virtual void compute_rx_win_params(int8_t datarate,
                                                 uint8_t minRxSymbols,
                                                 uint32_t rxError,
                                                 RxConfigParams_t *rxConfigParams);

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
                                TimerTime_t* txTimeOnAir );

    /*!
     * \brief The function processes a Link ADR request.
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
                                     uint8_t* nbBytesParsed );

    /*!
     * \brief The function processes a RX parameter setup request.
     *
     * \param [in] rxParamSetupReq A pointer to the function parameters.
     *
     * \retval The status of the operation, according to the LoRaMAC specification.
     */
    virtual uint8_t setup_rx_params(RxParamSetupReqParams_t* rxParamSetupReq );

    /*!
     * \brief The function processes a new channel request.
     *
     * \param [in] newChannelReq A pointer to the function parameters.
     *
     * \retval The status of the operation, according to the LoRaMAC specification.
     */
    virtual uint8_t request_new_channel(NewChannelReqParams_t* newChannelReq );

    /*!
     * \brief The function processes a TX ParamSetup request.
     *
     * \param [in] txParamSetupReq A pointer to the function parameters.
     *
     * \retval The status of the operation, according to the LoRaMAC specification.
     *         Returns -1, if the functionality is not implemented. In this case, the end node
     *         shall ignore the command.
     */
    virtual int8_t setup_tx_params(TxParamSetupReqParams_t* txParamSetupReq );

    /*!
     * \brief The function processes a DlChannel request.
     *
     * \param [in] dlChannelReq A pointer to the function parameters.
     *
     * \retval The status of the operation, according to the LoRaMAC specification.
     */
    virtual uint8_t dl_channel_request(DlChannelReqParams_t* dlChannelReq );

    /*!
     * \brief Alternates the datarate of the channel for the join request.
     *
     * \param [in] alternateDr A pointer to the function parameters.
     *
     * \retval The datarate to apply.
     */
    virtual int8_t get_alternate_DR(AlternateDrParams_t* alternateDr );

    /*!
     * \brief Calculates the back-off time.
     *
     * \param [in] calcBackOff A pointer to the function parameters.
     */
    virtual void calculate_backoff(CalcBackOffParams_t* calcBackOff );

    /*!
     * \brief Searches and sets the next random available channel.
     *
     * \param [in]  nextChanParams The parameters for the next channel.
     *
     * \param [out] channel The next channel to use for TX.
     *
     * \param [out] time The time to wait for the next transmission according to the duty cycle.
     *
     * \param [out] aggregatedTimeOff Updates the aggregated time off.
     *
     * \retval The function status [1: OK, 0: Unable to find a channel on the current datarate].
     */
    virtual bool set_next_channel(NextChanParams_t* nextChanParams,
                                   uint8_t* channel, TimerTime_t* time,
                                   TimerTime_t* aggregatedTimeOff );

    /*!
     * \brief Adds a channel.
     *
     * \param [in] channelAdd A pointer to the function parameters.
     *
     * \retval The status of the operation.
     */
    virtual LoRaMacStatus_t add_channel(ChannelAddParams_t* channelAdd );

    /*!
     * \brief Removes a channel.
     *
     * \param [in] channelRemove A pointer to the function parameters.
     *
     * \retval True, if the channel was removed successfully.
     */
    virtual bool remove_channel(ChannelRemoveParams_t* channelRemove );

    /*!
     * \brief Sets the radio into continuous wave mode.
     *
     * \param [in] continuousWave A pointer to the function parameters.
     */
    virtual void set_tx_cont_mode(ContinuousWaveParams_t* continuousWave );

    /*!
     * \brief Computes a new datarate according to the given offset.
     *
     * \param [in] downlinkDwellTime The downlink dwell time configuration. 0: No limit, 1: 400ms
     *
     * \param [in] dr The current datarate.
     *
     * \param [in] drOffset The offset to be applied.
     *
     * \retval newDr The computed datarate.
     */
    virtual uint8_t apply_DR_offset(uint8_t downlinkDwellTime, int8_t dr, int8_t drOffset );

private:
    uint8_t CountNbOfEnabledChannels( bool joined, uint8_t datarate, uint16_t* channelsMask, ChannelParams_t* channels, Band_t* bands, uint8_t* enabledChannels, uint8_t* delayTx );

    // Global attributes
    /*!
     * LoRaMAC channels
     */
    ChannelParams_t Channels[EU868_MAX_NB_CHANNELS];

    /*!
     * LoRaMac bands
     */
    Band_t Bands[EU868_MAX_NB_BANDS];

    /*!
     * LoRaMac channels mask
     */
    uint16_t ChannelsMask[EU868_CHANNELS_MASK_SIZE];

    /*!
     * LoRaMac channels default mask
     */
    uint16_t ChannelsDefaultMask[EU868_CHANNELS_MASK_SIZE];
};

#endif /* MBED_OS_LORAPHY_EU868_H_ */
