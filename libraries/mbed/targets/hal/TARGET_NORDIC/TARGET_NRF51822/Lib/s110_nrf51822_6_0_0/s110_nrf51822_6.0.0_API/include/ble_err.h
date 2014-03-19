/*
 * Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is confidential property of Nordic Semiconductor. The use,
 * copying, transfer or disclosure of such information is prohibited except by express written
 * agreement with Nordic Semiconductor.
 *
 */
 /**
  @addtogroup BLE_COMMON
  @{
  @addtogroup  nrf_error
  @{
    @ingroup BLE_COMMON
  @}

  @defgroup ble_err General error codes
  @{

  @brief General error code definitions for the BLE API.

  @ingroup BLE_COMMON
*/
#ifndef NRF_BLE_ERR_H__
#define NRF_BLE_ERR_H__

#include "nrf_error.h"

/* @defgroup BLE_ERRORS Error Codes
 * @{ */
#define BLE_ERROR_INVALID_CONN_HANDLE    (NRF_ERROR_STK_BASE_NUM+0x001) /**< Invalid connection handle. */
#define BLE_ERROR_INVALID_ATTR_HANDLE    (NRF_ERROR_STK_BASE_NUM+0x002) /**< Invalid attribute handle. */
#define BLE_ERROR_NO_TX_BUFFERS          (NRF_ERROR_STK_BASE_NUM+0x003) /**< Buffer capacity exceeded. */
/** @} */


/** @defgroup BLE_ERROR_SUBRANGES Module specific error code subranges
 *  @brief Assignment of subranges for module specific error codes.
 *  @note For specific error codes, see ble_<module>.h or ble_error_<module>.h.
 * @{ */
#define NRF_L2CAP_ERR_BASE             (NRF_ERROR_STK_BASE_NUM+0x100) /**< L2CAP specific errors. */
#define NRF_GAP_ERR_BASE               (NRF_ERROR_STK_BASE_NUM+0x200) /**< GAP specific errors. */
#define NRF_GATTC_ERR_BASE             (NRF_ERROR_STK_BASE_NUM+0x300) /**< GATT client specific errors. */
#define NRF_GATTS_ERR_BASE             (NRF_ERROR_STK_BASE_NUM+0x400) /**< GATT server specific errors. */
/** @} */

#endif


/**
  @}
  @}
*/
