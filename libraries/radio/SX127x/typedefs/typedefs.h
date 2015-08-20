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
#ifndef __TYPEDEFS_H__
#define __TYPEDEFS_H__

#include "mbed.h"
#include "./enums/enums.h"


class SX1276;
class Radio;
class SX1276MB1xAS;
class SX1272MB1xAS;
/*!
 * Hardware IO IRQ callback function definition
 */
typedef void ( Radio::*DioIrqHandler )( void );

/*!
 * triggers definition
 */
typedef void ( Radio::*Trigger )( void );
typedef void ( SX1276MB1xAS::*TriggerMB1xASSX1276 )( void );
typedef void ( SX1272MB1xAS::*TriggerMB1xASSX1272 )( void );

/*!
 * FSK bandwidth definition
 */
typedef struct
{
    uint32_t bandwidth;
    uint8_t  RegValue;
}FskBandwidth_t;

/*!
 * Radio registers definition
 */
typedef struct
{
    ModemType   Modem;
    uint8_t     Addr;
    uint8_t     Value;
}RadioRegisters_t;

#endif //__TYPEDEFS_H__

