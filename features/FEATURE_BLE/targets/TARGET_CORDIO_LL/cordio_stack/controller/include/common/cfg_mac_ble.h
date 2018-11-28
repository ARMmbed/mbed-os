/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      BLE MAC system configuration.
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

#ifndef CFG_MAC_BLE_H
#define CFG_MAC_BLE_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*** LL ***/

#ifndef LL_API_PARAM_CHECK
#define LL_API_PARAM_CHECK      1       /*!< Check API parameters. */
#endif

#ifndef LL_MAX_CONN
#define LL_MAX_CONN             4       /*!< Absolute maximum number of connections (maximum is 32). */
#endif

#ifndef LL_NUM_ADV_FILT
#define LL_NUM_ADV_FILT         8       /*!< Table size for advertising filter. */
#endif

#ifndef LL_MAX_ADV_SETS
#define LL_MAX_ADV_SETS         6       /*!< Absolute maximum number of advertising sets. */
#endif

#ifndef LL_MAX_PER_SCAN
#define LL_MAX_PER_SCAN         6       /*!< Absolute maximum number of periodic scanners (maximum is 32). */
#endif

#ifndef LL_ENABLE_TESTER
#define LL_ENABLE_TESTER        0       /*!< Enable LL tester extensions. */
#endif

#ifndef LHCI_ENABLE_VS
#define LHCI_ENABLE_VS          1       /*!< Enable vendor specific command processing. */
#endif

/*** Scheduler ***/

#ifndef SCH_RM_MAX_RSVN
#define SCH_RM_MAX_RSVN         (LL_MAX_CONN + LL_MAX_ADV_SETS) /*!< Maximum number of reservations (maximum is 32). */
#endif

/*** Baseband ***/

#ifndef BB_DATA_PLD_MAX_LEN
#define BB_DATA_PLD_MAX_LEN     251     /*!< Maximum data channel payload length (valid range 27 to 251, exclude MIC). */
#endif

#ifndef BB_ADV_PLD_MAX_LEN
#define BB_ADV_PLD_MAX_LEN      255     /*!< Maximum advertising channel payload length (valid range 95 to 255). */
#endif

#ifndef BB_FIXED_DATA_PKT_LEN
#define BB_FIXED_DATA_PKT_LEN   0       /*!< Fixed packet length required for BB data receive buffers (0 to disable). */
#endif

#ifndef BB_FIXED_ADVB_PKT_LEN
#define BB_FIXED_ADVB_PKT_LEN   0       /*!< Fixed packet length required for BB advertisement receive buffers (0 to disable). */
#endif

#ifndef BB_DATA_PDU_TAILROOM
#define BB_DATA_PDU_TAILROOM    4       /*!< Extra byte allocation required for BB operations (i.e. MIC). */
#endif

#ifndef BB_ENABLE_INLINE_ENC_TX
#define BB_ENABLE_INLINE_ENC_TX 0       /*!< Enable inline encryption mode. Not available for all projects. */
#endif

#ifndef BB_ENABLE_INLINE_DEC_RX
#define BB_ENABLE_INLINE_DEC_RX 0       /*!< Enable inline decryption mode. Not available for all projects. */
#endif

#ifndef BB_SYM_PHY_REQ
#define BB_SYM_PHY_REQ          0       /*!< Symmetric PHY required on connections. */
#endif

#ifdef __cplusplus
};
#endif

#endif /* CFG_MAC_BLE_H */
