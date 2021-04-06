/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FSL_ANATOP_AI_H_
#define _FSL_ANATOP_AI_H_

#include "fsl_common.h"
/*! @addtogroup anatop_ai */
/*! @{ */

/*! @file */

/*! @name Driver version */
/*@{*/
/*! @brief Anatop AI driver version 1.0.0. */
#define FSL_ANATOP_AI_DRIVER_VERSION (MAKE_VERSION(1, 0, 0))
/*@}*/

typedef enum _anatop_ai_itf
{
    kAI_Itf_Ldo = 0,
    kAI_Itf_1g = 1,
    kAI_Itf_Audio = 2,
    kAI_Itf_Video = 3,
    kAI_Itf_400m = 4,
    kAI_Itf_Temp = 5,
    kAI_Itf_Bandgap = 6,
} anatop_ai_itf_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */


/*!
 * @brief AI interface access
 *
 * @param itf AI interface name
 * @param isWrite write enable
 * @param addr address
 * @param wdata data to be set
 *
 */
uint32_t ANATOP_AI_Access(anatop_ai_itf_t itf, bool isWrite, uint8_t addr, uint32_t wdata);

/*!
 * @brief AI interface writing
 *
 * @param itf AI interface name
 * @param addr address
 * @param wdata data to be set
 *
 */
void ANATOP_AI_Write(anatop_ai_itf_t itf, uint8_t addr, uint32_t wdata);

/*!
 * @brief AI interface reading
 *
 * @param itf AI interface name
 * @param addr address
 * @return data read
 *
 */
uint32_t ANATOP_AI_Read(anatop_ai_itf_t itf, uint8_t addr);

/*!
 * @brief AI interface write with mask and shift
 *
 * @param itf AI interface name
 * @param addr address
 * @param wdata data to be written
 * @param mask bit field mask
 * @param shift bit field shift
 *
 */
void ANATOP_AI_WriteWithMaskShift(anatop_ai_itf_t itf, uint8_t addr, uint32_t wdata, uint32_t mask, uint32_t shift);

/* @} */

#if defined(__cplusplus)
}
#endif /* __cplusplus */

/*! @} */

#endif /* _FSL_ANATOP_AI_H_ */
