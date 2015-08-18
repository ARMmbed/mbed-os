/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    ( C )2014 Semtech

Description: Contains the callbacks for the IRQs and any application related details

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis and Gregory Cristian
*/
#ifndef __MAIN_H__
#define __MAIN_H__

/*
 * Callback functions prototypes
 */
/*!
 * @brief Function to be executed on Radio Tx Done event
 */
void OnTxDone( void );

/*!
 * @brief Function to be executed on Radio Rx Done event
 */
void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr );

/*!
 * @brief Function executed on Radio Tx Timeout event
 */
void OnTxTimeout( void );

/*!
 * @brief Function executed on Radio Rx Timeout event
 */
void OnRxTimeout( void );

/*!
 * @brief Function executed on Radio Rx Error event
 */
void OnRxError( void );

/*!
 * @brief Function executed on Radio Fhss Change Channel event
 */
void OnFhssChangeChannel( uint8_t channelIndex );

/*!
 * @brief Function executed on CAD Done event
 */
void OnCadDone( void );

#endif // __MAIN_H__