/*
 * Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is confidential property of Nordic Semiconductor. The use,
 * copying, transfer or disclosure of such information is prohibited except by express written
 * agreement with Nordic Semiconductor.
 *
 */
 /**
  @addtogroup nrf_soc_api
  @{
  @defgroup nrf_soc_error SoC Library Error Codes
  @{
     
  @brief Error definitions for the SoC library

*/

/* Header guard */
#ifndef NRF_ERROR_SOC_H__
#define NRF_ERROR_SOC_H__

#include "nrf_error.h"

/* Mutex Errors */
#define NRF_ERROR_SOC_MUTEX_ALREADY_TAKEN                 (NRF_ERROR_SOC_BASE_NUM + 0)  ///< Mutex already taken

/* NVIC errors */
#define NRF_ERROR_SOC_NVIC_INTERRUPT_NOT_AVAILABLE        (NRF_ERROR_SOC_BASE_NUM + 1)  ///< NVIC interrupt not available
#define NRF_ERROR_SOC_NVIC_INTERRUPT_PRIORITY_NOT_ALLOWED (NRF_ERROR_SOC_BASE_NUM + 2)  ///< NVIC interrupt priority not allowed
#define NRF_ERROR_SOC_NVIC_SHOULD_NOT_RETURN              (NRF_ERROR_SOC_BASE_NUM + 3)  ///< NVIC should not return

/* Power errors */
#define NRF_ERROR_SOC_POWER_MODE_UNKNOWN                  (NRF_ERROR_SOC_BASE_NUM + 4)  ///< Power mode unknown
#define NRF_ERROR_SOC_POWER_POF_THRESHOLD_UNKNOWN         (NRF_ERROR_SOC_BASE_NUM + 5)  ///< Power POF threshold unknown
#define NRF_ERROR_SOC_POWER_OFF_SHOULD_NOT_RETURN         (NRF_ERROR_SOC_BASE_NUM + 6)  ///< Power off should not return

/* Rand errors */
#define NRF_ERROR_SOC_RAND_NOT_ENOUGH_VALUES              (NRF_ERROR_SOC_BASE_NUM + 7)  ///< RAND not enough values

/* PPI errors */
#define NRF_ERROR_SOC_PPI_INVALID_CHANNEL                 (NRF_ERROR_SOC_BASE_NUM + 8)  ///< Invalid PPI Channel
#define NRF_ERROR_SOC_PPI_INVALID_GROUP                   (NRF_ERROR_SOC_BASE_NUM + 9)  ///< Invalid PPI Group

#endif // NRF_ERROR_SOC_H__
/**
  @}
  @}
*/
