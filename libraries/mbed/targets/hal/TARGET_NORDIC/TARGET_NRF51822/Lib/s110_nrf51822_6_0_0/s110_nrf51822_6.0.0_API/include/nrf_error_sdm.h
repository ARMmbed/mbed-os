/*
 * Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is confidential property of Nordic Semiconductor. The use,
 * copying, transfer or disclosure of such information is prohibited except by express written
 * agreement with Nordic Semiconductor.
 *
 */
 /**
  @addtogroup nrf_sdm_api
  @{
  @defgroup nrf_sdm_error SoftDevice Manager Error Codes
  @{
     
  @brief Error definitions for the SDM API
*/

/* Header guard */
#ifndef NRF_ERROR_SDM_H__
#define NRF_ERROR_SDM_H__

#include "nrf_error.h"

#define NRF_ERROR_SDM_LFCLK_SOURCE_UNKNOWN              (NRF_ERROR_SDM_BASE_NUM + 0)  ///< Unknown lfclk source
#define NRF_ERROR_SDM_INCORRECT_INTERRUPT_CONFIGURATION (NRF_ERROR_SDM_BASE_NUM + 1)  ///< Incorrect interrupt configuration (can be caused by using illegal priority levels, or having enabled SoftDevice interrupts)
#define NRF_ERROR_SDM_INCORRECT_CLENR0                  (NRF_ERROR_SDM_BASE_NUM + 2)  ///< Incorrect CLENR0 (can be caused by erronous SoftDevice flashing)

#endif // NRF_ERROR_SDM_H__

/**
  @}
  @}
*/
