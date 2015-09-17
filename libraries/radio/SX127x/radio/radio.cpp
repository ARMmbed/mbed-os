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
#include "radio.h"

Radio::Radio( void ( *txDone )( ), void ( *txTimeout ) ( ), void ( *rxDone ) ( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr ), 
              void ( *rxTimeout ) ( ), void ( *rxError ) ( ), void ( *fhssChangeChannel ) ( uint8_t channelIndex ), void ( *cadDone ) ( bool channelActivityDetected ) )
{
    this->txDone = txDone;
    this->txTimeout = txTimeout;
    this->rxDone = rxDone;
    this->rxTimeout = rxTimeout;
    this->rxError = rxError;
    this->fhssChangeChannel = fhssChangeChannel;
    this->cadDone = cadDone;
}

