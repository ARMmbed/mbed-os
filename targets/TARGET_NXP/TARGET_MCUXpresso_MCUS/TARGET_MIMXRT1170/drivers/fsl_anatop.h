/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FSL_ANATOP_H_
#define _FSL_ANATOP_H_
#endif

#include "fsl_common.h"

/*! @addtogroup anatop */
/*! @{ */

/*! @file */

/*******************************************************************************
 * Configurations
 ******************************************************************************/

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @name Driver version */
/*@{*/
/*! @brief ANATOP driver version 2.0.0. */
#define FSL_ANATOP_DRIVER_VERSION (MAKE_VERSION(2, 0, 0))
/*@}*/

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

void ANATOP_Cm7FbbCfg(void);
void ANATOP_InitWbiasCfg(bool fbb_on, bool rbb_on);
void ANATOP_WbiasPwlvl_1p8(uint32_t pw_lvl_1p8);
void ANATOP_WbiasNwlvl_1p8(uint32_t nw_lvl_1p8);
void ANATOP_WbCfg1p8Cfg(uint32_t wb_cfg_1p8);
void ANATOP_WbPwrSwEn1p8(uint32_t wb_pwr_sw_en_1p8);
void ANATOP_EnableWbias(bool enable);
void ANATOP_TrimLdoLpsrDig(uint32_t target_voltage);

/*!
 * @brief bypass LPSR LDO 
 *
 */
void ANATOP_LdoLpsrAnaBypassOn(void);

/*!
 * @brief Enable PLL LDO
 *
 */
void ANATOP_EnablePllLdo(void);
/* @} */

#if defined(__cplusplus)
}
#endif /* __cplusplus */

/*! @} */

#

