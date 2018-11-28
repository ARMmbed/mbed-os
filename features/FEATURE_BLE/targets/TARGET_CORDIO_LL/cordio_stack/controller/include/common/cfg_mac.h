/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      MAC system configuration.
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

#ifndef CFG_MAC_H
#define CFG_MAC_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*** Baseband ***/

#ifndef BB_CLK_RATE_HZ
#define BB_CLK_RATE_HZ          1000000 /*!< BB clock rate in hertz. */
#endif

/*** Controller HCI ***/

#ifndef CHCI_BUF_TAILROOM
#define CHCI_BUF_TAILROOM       4       /*!< Extra byte allocation required for buffer (e.g., for MIC). */
#endif

#ifdef __cplusplus
};
#endif

#endif /* CFG_MAC_H */
