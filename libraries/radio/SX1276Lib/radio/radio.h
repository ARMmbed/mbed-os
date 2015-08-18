/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    ( C )2014 Semtech

Description: Interface for the radios, contains the main functions that a radio needs, and 5 callback functions

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainers: Miguel Luis, Gregory Cristian and Nicolas Huguenin
*/
#ifndef __RADIO_H__
#define __RADIO_H__

#include "mbed.h"

#include "./enums/enums.h"

/*!
 *    Interface for the radios, contains the main functions that a radio needs, and 5 callback functions
 */
class Radio
{
protected:

    //-------------------------------------------------------------------------
    //                        Callback functions pointers
    //-------------------------------------------------------------------------
    
    /*!
     * @brief  Tx Done callback prototype.
     */
    void ( *txDone )( );

    /*!
     * @brief  Tx Timeout callback prototype.
     */
    void ( *txTimeout ) ( );

    /*!
     * @brief Rx Done callback prototype.
     *
     * @param [IN] payload Received buffer pointer
     * @param [IN] size    Received buffer size
     * @param [IN] rssi    RSSI value computed while receiving the frame [dBm]
     * @param [IN] snr     Raw SNR value given by the radio hardware
     *                     FSK : N/A ( set to 0 )
     *                     LoRa: SNR value in dB
     */
    void ( *rxDone ) ( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr );

    /*!
     * @brief  Rx Timeout callback prototype.
     */
    void ( *rxTimeout ) ( );
    
    /*!
     * @brief Rx Error callback prototype.
     */
    void ( *rxError ) ( );
    
    /*!
     * \brief  FHSS Change Channel callback prototype.
     *
     * \param [IN] CurrentChannel   Index number of the current channel
     */
    void ( *fhssChangeChannel )( uint8_t CurrentChannel );

    /*!
     * @brief CAD Done callback prototype.
     *
     * @param [IN] ChannelDetected    Channel Activity detected during the CAD
     */
    void ( *cadDone ) ( bool channelActivityDetected );
    
public:
    //-------------------------------------------------------------------------
    //                        Constructor
    //-------------------------------------------------------------------------
    /*!
     * @brief Constructor of the radio object, the parameters are the callback functions described in the header.
     * @param [IN]    txDone
     * @param [IN]    txTimeout
     * @param [IN]    rxDone
     * @param [IN]    rxTimeout
     * @param [IN]    rxError
     */
    Radio( void ( *txDone )( ), void ( *txTimeout ) ( ), void ( *rxDone ) ( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr ), 
           void ( *rxTimeout ) ( ), void ( *rxError ) ( ), void ( *fhssChangeChannel ) ( uint8_t channelIndex ), void ( *cadDone ) ( bool channelActivityDetected ) );
    virtual ~Radio( ) {};

    //-------------------------------------------------------------------------
    //                        Pure virtual functions
    //-------------------------------------------------------------------------

    /*!
     * Return current radio status
     *
     * @param status Radio status.[RF_IDLE, RF_RX_RUNNING, RF_TX_RUNNING]
     */
    virtual RadioState GetState( void ) = 0; 

    /*!
     * \brief Configures the radio with the given modem
     *
     * \param [IN] modem Modem to be used [0: FSK, 1: LoRa] 
     */
    virtual void SetModem( ModemType modem ) = 0;

    /*!
     * @brief Sets the channel frequency
     *
     * @param [IN] freq         Channel RF frequency
     */
    virtual void SetChannel( uint32_t freq ) = 0;
    
    /*!
     * @brief Sets the channels configuration
     *
     * @param [IN] modem      Radio modem to be used [0: FSK, 1: LoRa]
     * @param [IN] freq       Channel RF frequency
     * @param [IN] rssiThresh RSSI threshold
     *
     * @retval isFree         [true: Channel is free, false: Channel is not free]
     */
    virtual bool IsChannelFree( ModemType modem, uint32_t freq, int8_t rssiThresh ) = 0;
    
    /*!
     * @brief Generates a 32 bits random value based on the RSSI readings
     *
     * \remark This function sets the radio in LoRa modem mode and disables
     *         all interrupts.
     *         After calling this function either Radio.SetRxConfig or
     *         Radio.SetTxConfig functions must be called.
     *
     * @retval randomValue    32 bits random value
     */
    virtual uint32_t Random( void )= 0;
    
    /*!
     * @brief Sets the reception parameters
     *
     * @param [IN] modem        Radio modem to be used [0: FSK, 1: LoRa]
     * @param [IN] bandwidth    Sets the bandwidth
     *                          FSK : >= 2600 and <= 250000 Hz
     *                          LoRa: [0: 125 kHz, 1: 250 kHz,
     *                                 2: 500 kHz, 3: Reserved]
     * @param [IN] datarate     Sets the Datarate
     *                          FSK : 600..300000 bits/s
     *                          LoRa: [6: 64, 7: 128, 8: 256, 9: 512,
     *                                10: 1024, 11: 2048, 12: 4096  chips]
     * @param [IN] coderate     Sets the coding rate ( LoRa only )
     *                          FSK : N/A ( set to 0 )
     *                          LoRa: [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8]
     * @param [IN] bandwidthAfc Sets the AFC Bandwidth ( FSK only )
     *                          FSK : >= 2600 and <= 250000 Hz
     *                          LoRa: N/A ( set to 0 )
     * @param [IN] preambleLen  Sets the Preamble length ( LoRa only )
     *                          FSK : N/A ( set to 0 )
     *                          LoRa: Length in symbols ( the hardware adds 4 more symbols )
     * @param [IN] symbTimeout  Sets the RxSingle timeout value ( LoRa only )
     *                          FSK : N/A ( set to 0 )
     *                          LoRa: timeout in symbols
     * @param [IN] fixLen       Fixed length packets [0: variable, 1: fixed]
     * @param [IN] payloadLen   Sets payload length when fixed lenght is used
     * @param [IN] crcOn        Enables/Disables the CRC [0: OFF, 1: ON]
     * @param [IN] freqHopOn    Enables disables the intra-packet frequency hopping  [0: OFF, 1: ON] (LoRa only)
     * @param [IN] hopPeriod    Number of symbols bewteen each hop (LoRa only)
     * @param [IN] iqInverted   Inverts IQ signals ( LoRa only )
     *                          FSK : N/A ( set to 0 )
     *                          LoRa: [0: not inverted, 1: inverted]
     * @param [IN] rxContinuous Sets the reception in continuous mode
     *                          [false: single mode, true: continuous mode]
     */
    virtual void SetRxConfig ( ModemType modem, uint32_t bandwidth,
                               uint32_t datarate, uint8_t coderate,
                               uint32_t bandwidthAfc, uint16_t preambleLen,
                               uint16_t symbTimeout, bool fixLen,
                               uint8_t payloadLen,
                               bool crcOn, bool freqHopOn, uint8_t hopPeriod,
                               bool iqInverted, bool rxContinuous ) = 0;
    
    /*!
     * @brief Sets the transmission parameters
     *
     * @param [IN] modem        Radio modem to be used [0: FSK, 1: LoRa]
     * @param [IN] power        Sets the output power [dBm]
     * @param [IN] fdev         Sets the frequency deviation ( FSK only )
     *                          FSK : [Hz]
     *                          LoRa: 0
     * @param [IN] bandwidth    Sets the bandwidth ( LoRa only )
     *                          FSK : 0
     *                          LoRa: [0: 125 kHz, 1: 250 kHz,
     *                                 2: 500 kHz, 3: Reserved]
     * @param [IN] datarate     Sets the Datarate
     *                          FSK : 600..300000 bits/s
     *                          LoRa: [6: 64, 7: 128, 8: 256, 9: 512,
     *                                10: 1024, 11: 2048, 12: 4096  chips]
     * @param [IN] coderate     Sets the coding rate ( LoRa only )
     *                          FSK : N/A ( set to 0 )
     *                          LoRa: [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8]
     * @param [IN] preambleLen  Sets the preamble length
     * @param [IN] fixLen       Fixed length packets [0: variable, 1: fixed]
     * @param [IN] crcOn        Enables disables the CRC [0: OFF, 1: ON]
     * @param [IN] freqHopOn    Enables disables the intra-packet frequency hopping  [0: OFF, 1: ON] (LoRa only)
     * @param [IN] hopPeriod    Number of symbols bewteen each hop (LoRa only)
     * @param [IN] iqInverted   Inverts IQ signals ( LoRa only )
     *                          FSK : N/A ( set to 0 )
     *                          LoRa: [0: not inverted, 1: inverted]
     * @param [IN] timeout      Transmission timeout [us]
     */
    virtual void SetTxConfig( ModemType modem, int8_t power, uint32_t fdev,
                              uint32_t bandwidth, uint32_t datarate,
                              uint8_t coderate, uint16_t preambleLen,
                              bool fixLen, bool crcOn, bool freqHopOn,
                              uint8_t hopPeriod, bool iqInverted, uint32_t timeout ) = 0;
    
    /*!
     * @brief Checks if the given RF frequency is supported by the hardware
     *
     * @param [IN] frequency RF frequency to be checked
     * @retval isSupported [true: supported, false: unsupported]
     */
    virtual bool CheckRfFrequency( uint32_t frequency ) = 0;
    
    /*!
     * @brief Computes the packet time on air for the given payload
     *
     * \Remark Can only be called once SetRxConfig or SetTxConfig have been called
     *
     * @param [IN] modem      Radio modem to be used [0: FSK, 1: LoRa]
     * @param [IN] pktLen     Packet payload length
     *
     * @retval airTime        Computed airTime for the given packet payload length
     */
    virtual double TimeOnAir ( ModemType modem, uint8_t pktLen ) = 0;
    
    /*!
     * @brief Sends the buffer of size. Prepares the packet to be sent and sets
     *        the radio in transmission
     *
     * @param [IN]: buffer     Buffer pointer
     * @param [IN]: size       Buffer size
     */
    virtual void Send( uint8_t *buffer, uint8_t size ) = 0;
    
    /*!
     * @brief Sets the radio in sleep mode
     */
    virtual void Sleep( void ) = 0;
    
    /*!
     * @brief Sets the radio in standby mode
     */
    virtual void Standby( void ) = 0;
    
    /*!
     * @brief Sets the radio in CAD mode
     */
    virtual void StartCad( void ) = 0;
    
    /*!
     * @brief Sets the radio in reception mode for the given time
     * @param [IN] timeout Reception timeout [us]
     *                     [0: continuous, others timeout]
     */
    virtual void Rx( uint32_t timeout ) = 0;
    
    /*!
     * @brief Sets the radio in transmission mode for the given time
     * @param [IN] timeout Transmission timeout [us]
     *                     [0: continuous, others timeout]
     */
    virtual void Tx( uint32_t timeout ) = 0;
    
    /*!
     * @brief Reads the current RSSI value
     *
     * @retval rssiValue Current RSSI value in [dBm]
     */
    virtual int16_t GetRssi ( ModemType modem ) = 0;
    
    /*!
     * @brief Writes the radio register at the specified address
     *
     * @param [IN]: addr Register address
     * @param [IN]: data New register value
     */
    virtual void Write ( uint8_t addr, uint8_t data ) = 0;
    
    /*!
     * @brief Reads the radio register at the specified address
     *
     * @param [IN]: addr Register address
     * @retval data Register value
     */
    virtual uint8_t Read ( uint8_t addr ) = 0;
    
    /*!
     * @brief Writes multiple radio registers starting at address
     *
     * @param [IN] addr   First Radio register address
     * @param [IN] buffer Buffer containing the new register's values
     * @param [IN] size   Number of registers to be written
     */
    virtual void Write( uint8_t addr, uint8_t *buffer, uint8_t size ) = 0;
    
    /*!
     * @brief Reads multiple radio registers starting at address
     *
     * @param [IN] addr First Radio register address
     * @param [OUT] buffer Buffer where to copy the registers data
     * @param [IN] size Number of registers to be read
     */
    virtual void Read ( uint8_t addr, uint8_t *buffer, uint8_t size ) = 0;
    
    /*!
     * @brief Writes the buffer contents to the SX1276 FIFO
     *
     * @param [IN] buffer Buffer containing data to be put on the FIFO.
     * @param [IN] size Number of bytes to be written to the FIFO
     */
    virtual void WriteFifo( uint8_t *buffer, uint8_t size ) = 0;

    /*!
     * @brief Reads the contents of the SX1276 FIFO
     *
     * @param [OUT] buffer Buffer where to copy the FIFO read data.
     * @param [IN] size Number of bytes to be read from the FIFO
     */
    virtual void ReadFifo( uint8_t *buffer, uint8_t size ) = 0;
};

#endif // __RADIO_H__

