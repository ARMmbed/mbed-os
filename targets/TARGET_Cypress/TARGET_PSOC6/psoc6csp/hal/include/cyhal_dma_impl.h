/***************************************************************************//**
* \file cyhal_dma_impl.h
*
* \brief
* Implementation details of Cypress Datawire/DMAC DMA.
*
********************************************************************************
* \copyright
* Copyright 2018-2019 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

#pragma once

#if defined(CY_IP_M4CPUSS_DMAC) || defined(CY_IP_M4CPUSS_DMA)

#if defined(__cplusplus)
extern "C" {
#endif

/** \addtogroup group_hal_psoc6_dma DMA (Direct Memory Access)
 * \ingroup group_hal_psoc6
 * \{
 * DW (DataWire) is one of two DMA hardware implementations for PSOC6. DW is
 * designed for low latency memory to peripheral or peripheral to memory
 * transfers but can also perform memory to memory transfers and peripheral to
 * peripheral transfers.
 *
 * DMAC (Direct Memory Access Controller) is the second of two DMA hardware
 * implementations for PSOC6. DMAC is designed with high memory bandwidth for
 * large memory to memory transfers but can perform peripheral to memory,
 * memory to peripheral, and peripheral to peripheral transfers.
 *
 * Which DMA type is used is dependent on the exact hardware and number of DMA
 * channels already in use. This implementation will attempt to use DMAC first
 * for memory to memory transfers and Datawire otherwise but either type may be
 * used. */

/** Default DMA channel priority */
#define CYHAL_DMA_PRIORITY_DEFAULT    CYHAL_DMA_PRIORITY_LOW
/** High DMA channel priority */
#define CYHAL_DMA_PRIORITY_HIGH       0
/** Medium DMA channel priority */
#define CYHAL_DMA_PRIORITY_MEDIUM     1
/** Low DMA channel priority */
#define CYHAL_DMA_PRIORITY_LOW        3

/** \cond INTERNAL */
/** Hal-Triggers uses bit 8 to denote a one to one trigger, whereas, the PDL
 * SwTrigger function uses bit 5 to denote a one to one trigger. */
#define HAL_TRIGGERS_1TO1_MASK (0x80)
#define PDL_TRIGGERS_1TO1_MASK (0x10)
/** \endcond */

/** \} group_hal_psoc6_dma */

#if defined(__cplusplus)
}
#endif

#endif /* defined(CY_IP_M4CPUSS_DMAC) || defined(CY_IP_M4CPUSS_DMA) */
