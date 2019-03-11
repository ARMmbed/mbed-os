/* Copyright (c) 2009-2019 Arm Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*************************************************************************************************/
/*!
 *  \brief Radio interface file.
 */
/*************************************************************************************************/

#ifndef PAL_RADIO_H
#define PAL_RADIO_H

#include "stack/platform/include/pal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup PAL_RADIO
 *  \{ */

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief      Initialize RF path compensation.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void PalRadioInitPathComp(void);

/*************************************************************************************************/
/*!
 *  \brief      Get the actual Tx power at the antenna (expressed in 1dBm units).
 *
 *  \param      txPwr           Tx power provided by the host (expressed in 1dBm units).
 *  \param      compFlag        Flag to apply Tx path compensation or not.
 *
 *  \return     Actual Tx power at the antenna (expressed in 1dBm units).
 *
 *  Tx path compensation is only used for extended ADV header.
 *  Compensation is not considered when filling in HCI events.
 */
/*************************************************************************************************/
int8_t PalRadioGetActualTxPower(int8_t txPwr, bool_t compFlag);

/*************************************************************************************************/
/*!
 *  \brief      Set RF path compensation.
 *
 *  \param      txPathComp      RF transmit path compensation value (expressed in 0.1dBm units).
 *  \param      rxPathComp      RF receive path compensation value (expressed in 0.1dBm units).
 *
 *  \return     TRUE if successful, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t PalRadioWriteRfPathComp(int16_t txPathComp, int16_t rxPathComp);

/*************************************************************************************************/
/*!
 *  \brief      Get supported transmit power.
 *
 *  \param      pMinTxPwr   Return buffer for minimum transmit power (expressed in 1dBm units).
 *  \param      pMaxTxPwr   Return buffer for maximum transmit power (expressed in 1dBm units).
 *
 *  \return     None.
 */
/*************************************************************************************************/
void PalRadioGetSupTxPower(int8_t *pMinTxPwr, int8_t *pMaxTxPwr);

/*************************************************************************************************/
/*!
 *  \brief      Read RF path compensation.
 *
 *  \param      pTxPathComp Return buffer for RF transmit path compensation value (expressed in 0.1dBm units).
 *  \param      pRxPathComp Return buffer for RF receive path compensation value (expressed in 0.1dBm units).
 *
 *  \return     None.
 */
/*************************************************************************************************/
void PalRadioReadRfPathComp(int16_t *pTxPathComp, int16_t *pRxPathComp);

/*************************************************************************************************/
/*!
 *  \brief      Get receive RF path compensation.
 *
 *  \return     Transmit RF path compensation (in 1-dBm units).
 */
/*************************************************************************************************/
int8_t PalRadioGetRxRfPathComp(void);

/*! \} */    /* PAL_RADIO */

#ifdef __cplusplus
};
#endif

#endif /* PAL_RADIO_H */
