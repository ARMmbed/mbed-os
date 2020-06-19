/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FSL_OS_ABSTRACTION_H_
#define _FSL_OS_ABSTRACTION_H_

/*!
 * @addtogroup osa_adapter
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "fsl_os_abstraction_mbed.h"

/*
 * alloc the temporary memory to store the status
 */
#define OSA_SR_ALLOC() uint32_t osaCurrentSr;
/*
 * Enter critical mode
 */
#define OSA_ENTER_CRITICAL() OSA_EnterCritical(&osaCurrentSr)
/*
 * Exit critical mode and retore the previous mode
 */
#define OSA_EXIT_CRITICAL() OSA_ExitCritical(osaCurrentSr)

/*******************************************************************************
 * API
 ******************************************************************************/

/*!
 * @brief Enter critical with nesting mode.
 *
 * @param sr Store current status and return to caller.
 */
void OSA_EnterCritical(uint32_t *sr);

/*!
 * @brief Exit critical with nesting mode.
 *
 * @param sr Previous status to restore.
 */
void OSA_ExitCritical(uint32_t sr);

/*! @}*/
#ifdef __cplusplus
}
#endif
/*! @}*/
#endif
