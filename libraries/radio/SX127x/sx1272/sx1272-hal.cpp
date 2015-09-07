/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    ( C )2014 Semtech

Description: -

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainers: Miguel Luis, Gregory Cristian and Nicolas Huguenin
*/
#include "sx1272-hal.h"

const RadioRegisters_t SX1272MB1xAS::RadioRegsInit[] =
{                                                 
    { MODEM_FSK , REG_LNA                , 0x23 },
    { MODEM_FSK , REG_RXCONFIG           , 0x1E },
    { MODEM_FSK , REG_RSSICONFIG         , 0xD2 },
    { MODEM_FSK , REG_PREAMBLEDETECT     , 0xAA },
    { MODEM_FSK , REG_OSC                , 0x07 },
    { MODEM_FSK , REG_SYNCCONFIG         , 0x12 },
    { MODEM_FSK , REG_SYNCVALUE1         , 0xC1 },
    { MODEM_FSK , REG_SYNCVALUE2         , 0x94 },
    { MODEM_FSK , REG_SYNCVALUE3         , 0xC1 },
    { MODEM_FSK , REG_PACKETCONFIG1      , 0x98 },
    { MODEM_FSK , REG_FIFOTHRESH         , 0x8F },
    { MODEM_FSK , REG_IMAGECAL           , 0x02 },
    { MODEM_FSK , REG_DIOMAPPING1        , 0x00 },
    { MODEM_FSK , REG_DIOMAPPING2        , 0x30 },
    { MODEM_LORA, REG_LR_PAYLOADMAXLENGTH, 0x40 },  
};

SX1272MB1xAS::SX1272MB1xAS( void ( *txDone )( ), void ( *txTimeout ) ( ), void ( *rxDone ) ( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr ),
                            void ( *rxTimeout ) ( ), void ( *rxError ) ( ), void ( *fhssChangeChannel ) ( uint8_t channelIndex ), void ( *cadDone ) ( bool ChannelActivityDetected ),
                            PinName mosi, PinName miso, PinName sclk, PinName nss, PinName reset,
                            PinName dio0, PinName dio1, PinName dio2, PinName dio3, PinName dio4, PinName dio5,
                            PinName antSwitch )
                            : SX1272( txDone, txTimeout, rxDone, rxTimeout, rxError, fhssChangeChannel, cadDone, mosi, miso, sclk, nss, reset, dio0, dio1, dio2, dio3, dio4, dio5),
                            antSwitch( antSwitch )
{
    Reset( );
    
    RxChainCalibration( );
    
    IoInit( );
    
    SetOpMode( RF_OPMODE_SLEEP );
    
    IoIrqInit( dioIrq );
    
    RadioRegistersInit( );

    SetModem( MODEM_FSK );

    this->settings.State = IDLE ;
}

//-------------------------------------------------------------------------
//                      Board relative functions
//-------------------------------------------------------------------------
void SX1272MB1xAS::IoInit( void )
{
    AntSwInit( );
    SpiInit( );
}

void SX1272MB1xAS::RadioRegistersInit( ){
    uint8_t i = 0;
    for( i = 0; i < sizeof( RadioRegsInit ) / sizeof( RadioRegisters_t ); i++ )
    {
        SetModem( RadioRegsInit[i].Modem );
        Write( RadioRegsInit[i].Addr, RadioRegsInit[i].Value );
    }    
}

void SX1272MB1xAS::SpiInit( void )
{
    nss = 1;    
    spi.format( 8,0 );   
    uint32_t frequencyToSet = 8000000;
    #if( defined ( TARGET_NUCLEO_L152RE ) ||  defined ( TARGET_LPC11U6X ) )
        spi.frequency( frequencyToSet );
    #elif( defined ( TARGET_KL25Z ) ) //busclock frequency is halved -> double the spi frequency to compensate
        spi.frequency( frequencyToSet * 2 );
    #else
        #warning "Check the board's SPI frequency"
    #endif
    wait(0.1); 
}

void SX1272MB1xAS::IoIrqInit( DioIrqHandler *irqHandlers )
{
    #if( defined ( TARGET_NUCLEO_L152RE ) ||  defined ( TARGET_LPC11U6X ) )
        dio0.mode(PullDown);
        dio1.mode(PullDown);   
        dio2.mode(PullDown);
        dio3.mode(PullDown); 
        dio4.mode(PullDown); 
    #endif
    dio0.rise( this, static_cast< TriggerMB1xASSX1272 > ( irqHandlers[0] ) );
    dio1.rise( this, static_cast< TriggerMB1xASSX1272 > ( irqHandlers[1] ) );
    dio2.rise( this, static_cast< TriggerMB1xASSX1272 > ( irqHandlers[2] ) );
    dio3.rise( this, static_cast< TriggerMB1xASSX1272 > ( irqHandlers[3] ) );
    dio4.rise( this, static_cast< TriggerMB1xASSX1272 > ( irqHandlers[4] ) );
}

void SX1272MB1xAS::IoDeInit( void )
{
    //nothing
}

uint8_t SX1272MB1xAS::GetPaSelect( uint32_t channel )
{
	return RF_PACONFIG_PASELECT_RFO;
}

void SX1272MB1xAS::SetAntSwLowPower( bool status )
{
    if( isRadioActive != status )
    {
        isRadioActive = status;
    
        if( status == false )
        {
            AntSwInit( );
        }
        else
        {
            AntSwDeInit( );
        }
    }
}

void SX1272MB1xAS::AntSwInit( void )
{
    antSwitch = 0;
}

void SX1272MB1xAS::AntSwDeInit( void )
{
    antSwitch = 0;
}

void SX1272MB1xAS::SetAntSw( uint8_t rxTx )
{
    if( this->rxTx == rxTx )
    {
        //no need to go further
        return;
    }

    this->rxTx = rxTx;

    if( rxTx != 0 )
    {
        antSwitch = 1;
    }
    else
    {
        antSwitch = 0;
    }
}

bool SX1272MB1xAS::CheckRfFrequency( uint32_t frequency )
{
    //TODO: Implement check, currently all frequencies are supported
    return true;
}


void SX1272MB1xAS::Reset( void )
{
    reset.output();
    reset = 1;
    wait_ms( 1 );
    reset.input();
    wait_ms( 6 );
}
    
void SX1272MB1xAS::Write( uint8_t addr, uint8_t data )
{
    Write( addr, &data, 1 );
}

uint8_t SX1272MB1xAS::Read( uint8_t addr )
{
    uint8_t data;
    Read( addr, &data, 1 );
    return data;
}

void SX1272MB1xAS::Write( uint8_t addr, uint8_t *buffer, uint8_t size )
{
    uint8_t i;

    nss = 0;
    spi.write( addr | 0x80 );
    for( i = 0; i < size; i++ )
    {
        spi.write( buffer[i] );
    }
    nss = 1;
}

void SX1272MB1xAS::Read( uint8_t addr, uint8_t *buffer, uint8_t size )
{
    uint8_t i;

    nss = 0;
    spi.write( addr & 0x7F );
    for( i = 0; i < size; i++ )
    {
        buffer[i] = spi.write( 0 );
    }
    nss = 1;
}

void SX1272MB1xAS::WriteFifo( uint8_t *buffer, uint8_t size )
{
    Write( 0, buffer, size );
}

void SX1272MB1xAS::ReadFifo( uint8_t *buffer, uint8_t size )
{
    Read( 0, buffer, size );
}
