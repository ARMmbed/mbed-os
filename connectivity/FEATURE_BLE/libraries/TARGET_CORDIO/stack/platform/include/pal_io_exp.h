/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      I/O expander driver definition.
 *
 *  Copyright (c) 2019 Packetcraft, Inc.
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

#ifndef PAL_IO_EXP_H
#define PAL_IO_EXP_H

#include "pal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup PAL_IO_EXP
 *  \{ */

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Read register completion callback. */
typedef void (*PalIoExpRdRegCompCback_t)(bool_t status, uint8_t portValue);

/*! \brief      Write register completion callback. */
typedef void (*PalIoExpWrRegCompCback_t)(bool_t status);

/*! \brief      Operational states. */
typedef enum
{
  PAL_IO_EXP_STATE_UNINIT = 0,  /*!< Uninitialized state. */
  PAL_IO_EXP_STATE_ERROR  = 0,  /*!< Error state. */
  PAL_IO_EXP_STATE_INIT   = 1,  /*!< Initialized state. */
  PAL_IO_EXP_STATE_READY  = 2,  /*!< Ready state. */
  PAL_IO_EXP_STATE_BUSY   = 3,  /*!< Busy state. */
} PalIoExpState_t;

/*! \brief      Operations. */
typedef enum
{
  PAL_IO_EXP_OP_INPUT          = 0x00,       /*!< Input Port. Read only. */
  PAL_IO_EXP_OP_OUTPUT         = 0x01,       /*!< Output Port. Read/write. */
  PAL_IO_EXP_OP_POL_INV        = 0x02,       /*!< Polarity Inversion. Read/write. */
  PAL_IO_EXP_OP_CONFIG         = 0x03        /*!< Configuration. Read/write. */
} PalIoOp_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
void PalIoExpInit(void);
void PalIoExpDeInit(void);
uint8_t PalIoExpRegisterDevice(uint8_t subAddr);
void  PalIoExpRegisterCback(uint8_t devHandle, PalIoExpRdRegCompCback_t rdCback, PalIoExpWrRegCompCback_t wrCback);

/* Control and Status */
PalIoExpState_t PalIoExpGetState(uint8_t devHandle);

/* Data transfer */
void PalIoExpRead(uint8_t devHandle, PalIoOp_t op);
void PalIoExpWrite(uint8_t devHandle, PalIoOp_t op, uint8_t value);

/*! \} */    /* PAL_IO_EXP */

#ifdef __cplusplus
};
#endif

#endif /* PAL_IO_EXP_H */
