/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    ( C )2014 Semtech

Description: Actual implementation of a SX1276 radio, inherits Radio

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainers: Miguel Luis, Gregory Cristian and Nicolas Huguenin
*/
#ifndef __SX1276_H__
#define __SX1276_H__

#include "radio.h"
#include "./registers/sx1276Regs-Fsk.h"
#include "./registers/sx1276Regs-LoRa.h"
#include "./typedefs/typedefs.h"

#define XTAL_FREQ                                   32000000
#define FREQ_STEP                                   61.03515625

#define RX_BUFFER_SIZE                              256

#define DEFAULT_TIMEOUT                             200 //usec
#define RSSI_OFFSET                                 -139.0


/*!
 * Constant values need to compute the RSSI value
 */
#define RSSI_OFFSET_LF                              -164.0
#define RSSI_OFFSET_HF                              -157.0

#define RF_MID_BAND_THRESH                          525000000

/*! 
 * Actual implementation of a SX1276 radio, inherits Radio
 */
class SX1276 : public Radio
{
protected:
    /*!
    * SPI Interface
    */
    SPI spi; // mosi, miso, sclk
    DigitalOut nss;

    /*!
     * SX1276 Reset pin
     */
    DigitalInOut reset;

    /*!
     * SX1276 DIO pins
     */
    InterruptIn dio0;
    InterruptIn dio1;
    InterruptIn dio2; 
    InterruptIn dio3;
    InterruptIn dio4;
    DigitalIn dio5;
    
    bool isRadioActive;
    
    uint8_t boardConnected; //1 = SX1276MB1LAS; 0 = SX1276MB1MAS
    
    uint8_t *rxBuffer;
    
    uint8_t previousOpMode;
    
    /*!
     * Hardware DIO IRQ functions
     */
    DioIrqHandler *dioIrq;
    
    /*!
     * Tx and Rx timers
     */
    Timeout txTimeoutTimer;
    Timeout rxTimeoutTimer;
    Timeout rxTimeoutSyncWord;
    
    /*!
     *  rxTx: [1: Tx, 0: Rx]
     */
    uint8_t rxTx;
    
    RadioSettings_t settings;
    
    static const FskBandwidth_t FskBandwidths[] ;
protected:

    /*!
    * Performs the Rx chain calibration for LF and HF bands
    * \remark Must be called just after the reset so all registers are at their
    *         default values
    */
    void RxChainCalibration( void );

public:
    SX1276( void ( *txDone )( ), void ( *txTimeout ) ( ), void ( *rxDone ) ( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr ), 
            void ( *rxTimeout ) ( ), void ( *rxError ) ( ), void ( *fhssChangeChannel ) ( uint8_t channelIndex ), void ( *cadDone ) ( bool channelActivityDetected ),
            PinName mosi, PinName miso, PinName sclk, PinName nss, PinName reset,
            PinName dio0, PinName dio1, PinName dio2, PinName dio3, PinName dio4, PinName dio5 ); 
    SX1276( void ( *txDone )( ), void ( *txTimeout ) ( ), void ( *rxDone ) ( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr ), 
            void ( *rxTimeout ) ( ), void ( *rxError ) ( ), void ( *fhssChangeChannel ) ( uint8_t channelIndex ), void ( *cadDone ) ( bool channelActivityDetected ) );
    virtual ~SX1276( );
    
    //-------------------------------------------------------------------------
    //                        Redefined Radio functions
    //-------------------------------------------------------------------------
    /*!
     * Return current radio status
     *
     * @param status Radio status. [IDLE, RX_RUNNING, TX_RUNNING]
     */
    virtual RadioState GetState( void ); 
    
    /*!
     * @brief Configures the SX1276 with the given modem
     *
     * @param [IN] modem Modem to be used [0: FSK, 1: LoRa] 
     */
    virtual void SetModem( ModemType modem );

    /*!
     * @brief Sets the channel frequency
     *
     * @param [IN] freq         Channel RF frequency
     */
    virtual void SetChannel( uint32_t freq );
    
    /*!
     * @brief Sets the channels configuration
     *
     * @param [IN] modem      Radio modem to be used [0: FSK, 1: LoRa]
     * @param [IN] freq       Channel RF frequency
     * @param [IN] rssiThresh RSSI threshold
     *
     * @retval isFree         [true: Channel is free, false: Channel is not free]
     */
    virtual bool IsChannelFree( ModemType modem, uint32_t freq, int8_t rssiThresh );
    
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
    virtual uint32_t Random( void );
    
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
                               bool iqInverted, bool rxContinuous );
    
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
                              uint8_t hopPeriod, bool iqInverted, uint32_t timeout );
    
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
    virtual double TimeOnAir ( ModemType modem, uint8_t pktLen );
    
    /*!
     * @brief Sends the buffer of size. Prepares the packet to be sent and sets
     *        the radio in transmission
     *
     * @param [IN]: buffer     Buffer pointer
     * @param [IN]: size       Buffer size
     */
    virtual void Send( uint8_t *buffer, uint8_t size );
    
    /*!
     * @brief Sets the radio in sleep mode
     */
    virtual void Sleep( void );
    
    /*!
     * @brief Sets the radio in standby mode
     */
    virtual void Standby( void );
    
    /*!
     * @brief Sets the radio in reception mode for the given time
     * @param [IN] timeout Reception timeout [us]
     *                     [0: continuous, others timeout]
     */
    virtual void Rx( uint32_t timeout );
    
    /*!
     * @brief Sets the radio in transmission mode for the given time
     * @param [IN] timeout Transmission timeout [us]
     *                     [0: continuous, others timeout]
     */
    virtual void Tx( uint32_t timeout );
    
    /*!
     * @brief Start a Channel Activity Detection
     */
    virtual void StartCad( void );    
    
    /*!
     * @brief Reads the current RSSI value
     *
     * @retval rssiValue Current RSSI value in [dBm]
     */
    virtual int16_t GetRssi ( ModemType modem );
    
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
    /*!
     * @brief Resets the SX1276
     */
    virtual void Reset( void ) = 0;
    
    //-------------------------------------------------------------------------
    //                        Board relative functions
    //-------------------------------------------------------------------------
    
protected:
    /*!
     * @brief Initializes the radio I/Os pins interface
     */
    virtual void IoInit( void ) = 0;
    
    /*!
     *    @brief Initializes the radio registers
     */
    virtual void RadioRegistersInit( ) = 0;
    
    /*!
     * @brief Initializes the radio SPI
     */
    virtual void SpiInit( void ) = 0;
    
    /*!
     * @brief Initializes DIO IRQ handlers
     *
     * @param [IN] irqHandlers Array containing the IRQ callback functions
     */
    virtual void IoIrqInit( DioIrqHandler *irqHandlers ) = 0;

    /*!
     * @brief De-initializes the radio I/Os pins interface. 
     *
     * \remark Useful when going in MCU lowpower modes
     */
    virtual void IoDeInit( void ) = 0;

    /*!
     * @brief Gets the board PA selection configuration
     *
     * @param [IN] channel Channel frequency in Hz
     * @retval PaSelect RegPaConfig PaSelect value
     */
    virtual uint8_t GetPaSelect( uint32_t channel ) = 0;

    /*!
     * @brief Set the RF Switch I/Os pins in Low Power mode
     *
     * @param [IN] status enable or disable
     */
    virtual void SetAntSwLowPower( bool status ) = 0;

    /*!
     * @brief Initializes the RF Switch I/Os pins interface
     */
    virtual void AntSwInit( void ) = 0;

    /*!
     * @brief De-initializes the RF Switch I/Os pins interface 
     *
     * \remark Needed to decrease the power consumption in MCU lowpower modes
     */
    virtual void AntSwDeInit( void ) = 0;

    /*!
     * @brief Controls the antena switch if necessary.
     *
     * \remark see errata note
     *
     * @param [IN] rxTx [1: Tx, 0: Rx]
     */
    virtual void SetAntSw( uint8_t rxTx ) = 0;
    
    /*!
     * @brief Checks if the given RF frequency is supported by the hardware
     *
     * @param [IN] frequency RF frequency to be checked
     * @retval isSupported [true: supported, false: unsupported]
     */
    virtual bool CheckRfFrequency( uint32_t frequency ) = 0;
protected:

    /*!
     * @brief Sets the SX1276 operating mode
     *
     * @param [IN] opMode New operating mode
     */
    virtual void SetOpMode( uint8_t opMode );

    /*
     * SX1276 DIO IRQ callback functions prototype
     */

    /*!
     * @brief DIO 0 IRQ callback
     */
    virtual void OnDio0Irq( void );

    /*!
     * @brief DIO 1 IRQ callback
     */
    virtual void OnDio1Irq( void );

    /*!
     * @brief DIO 2 IRQ callback
     */
    virtual void OnDio2Irq( void );

    /*!
     * @brief DIO 3 IRQ callback
     */
    virtual void OnDio3Irq( void );

    /*!
     * @brief DIO 4 IRQ callback
     */
    virtual void OnDio4Irq( void );

    /*!
     * @brief DIO 5 IRQ callback
     */
    virtual void OnDio5Irq( void );

    /*!
     * @brief Tx & Rx timeout timer callback
     */
    virtual void OnTimeoutIrq( void );
    
    /*!
     * Returns the known FSK bandwidth registers value
     *
     * \param [IN] bandwidth Bandwidth value in Hz
     * \retval regValue Bandwidth register value.
     */
    static uint8_t GetFskBandwidthRegValue( uint32_t bandwidth );
};

#endif //__SX1276_H__

