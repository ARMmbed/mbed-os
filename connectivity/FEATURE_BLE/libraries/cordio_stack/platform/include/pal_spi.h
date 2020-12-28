/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      SPI driver definition.
 *
 *  Copyright (c) 2019-2020 Packetcraft, Inc.
 *  
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *  
 *      http://www.apache.org/licenses/LICENSE-2.0
 *  
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
/*************************************************************************************************/

#ifndef PAL_SPI_H
#define PAL_SPI_H

#include "pal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup PAL_SPI
 *  \{ */

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Signaling mode. */
typedef enum
{
  PAL_SPI_MODE_SLAVE,                   /*!< Slave mode. */
  PAL_SPI_MODE_MASTER                   /*!< Master mode. */
} PalSpiMode_t;

/*! \brief      Data exchange completion callback. */
typedef void (*PalSpiCompCback)(void);

/*! \brief      Peripheral configuration. */
typedef struct
{
  uint32_t clkRateHz;                   /*!< Clock rate in Hz. */
  PalSpiMode_t mode;                    /*!< Signaling mode. */
  PalSpiCompCback compCback;            /*!< Data exchange completion callback. */
} PalSpiConfig_t;

/*! \brief      Operational states. */
typedef enum
{
  PAL_SPI_STATE_UNINIT = 0,             /*!< Uninitialized state. */
  PAL_SPI_STATE_ERROR  = 0,             /*!< Error state. */
  PAL_SPI_STATE_READY,                  /*!< Ready state. */
  PAL_SPI_STATE_BUSY                    /*!< Data exchange in progress. */
} PalSpiState_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
void PalSpiInit(const PalSpiConfig_t *pCfg);
void PalSpiDeInit(void);

/* Control and Status */
PalSpiState_t PalSpiGetState(void);

/* Data Transfer */
void PalSpiDataExchange(uint8_t *pRdData, uint16_t rdDataLen, const uint8_t *pWrData, uint16_t wrDataLen);

/*! \} */    /* PAL_SPI */

#ifdef __cplusplus
};
#endif

#endif /* PAL_SPI_H */
