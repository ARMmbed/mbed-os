/*************************************************************************************************/
/*!
 *  \brief      Nordic baseband driver header.
 *
 *  Copyright (c) 2009-2018 ARM Ltd., all rights reserved.
 *  SPDX-License-Identifier: LicenseRef-PBL
 *
 *  This file and the related binary are licensed under the
 *  Permissive Binary License, Version 1.0 (the "License");
 *  you may not use these files except in compliance with the License.
 *
 *  You may obtain a copy of the License here:
 *  LICENSE-permissive-binary-license-1.0.txt and at
 *  https://www.mbed.com/licenses/PBL-1.0
 *
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
/*************************************************************************************************/

#ifndef BB_DRV_NORDIC_H
#define BB_DRV_NORDIC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "bb_api.h"

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief  IRQ callback datatypes. */
typedef void (*bbDrvIrqCback_t)(void);

/*************************************************************************************************/
/*!
 *  \brief      Called to register a protocol's Radio and Timer IRQ callback functions.
 *
 *  \param      protId      Protocol ID.
 *  \param      timerCback  Timer IRQ callback.
 *  \param      radioCback  Timer IRQ callback.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void BbDrvRegisterProtIrq(uint8_t protId, bbDrvIrqCback_t timerCback, bbDrvIrqCback_t radioCback);

#ifdef __cplusplus
};
#endif

#endif /* BB_DRV_NORDIC_H */
