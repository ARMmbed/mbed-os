/***************************************************************************//**
* \file cy_result.h
*
* \brief
* Basic function result handling. Defines a simple type for conveying
* information about whether something succeeded or details about any issues
* that were detected.
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

/**
* \addtogroup group_result Result Type
* \ingroup group_abstraction
* \{
* Basic function result handling. Defines a simple type for conveying
* information about whether something succeeded or details about any issues
* that were detected.
*
* \defgroup group_result_macros Macros
*/

#pragma once

#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

/**
* \addtogroup group_result_macros
* \{
*/

/** Mask for the bit at position "x" */
#define CY_BIT_MASK(x)                     ((1U << (x)) - 1U)

/** Bit position of the result code */
#define CY_RSLT_CODE_POSITION              (0U)
/** Bit width of the result code */
#define CY_RSLT_CODE_WIDTH                 (16U)
/** Bit position of the result type */
#define CY_RSLT_TYPE_POSITION              (16U)
/** Bit width of the result type */
#define CY_RSLT_TYPE_WIDTH                 (2U)
/** Bit position of the module identifier */
#define CY_RSLT_MODULE_POSITION            (18U)
/** Bit width of the module identifier */
#define CY_RSLT_MODULE_WIDTH               (14U)

/** Mask for the result code */
#define CY_RSLT_CODE_MASK                  CY_BIT_MASK(CY_RSLT_CODE_WIDTH)
/** Mask for the module identifier */
#define CY_RSLT_MODULE_MASK                CY_BIT_MASK(CY_RSLT_MODULE_WIDTH)
/** Mask for the result type */
#define CY_RSLT_TYPE_MASK                  CY_BIT_MASK(CY_RSLT_TYPE_WIDTH)

/** Informational-only result status */
#define CY_RSLT_TYPE_INFO                  (0U)
/** Warning result */
#define CY_RSLT_TYPE_WARNING               (1U)
/** Error result */
#define CY_RSLT_TYPE_ERROR                 (2U)
/** Fatal error result */
#define CY_RSLT_TYPE_FATAL                 (3U)

/** Get the value of the result code field */
#define CY_RSLT_GET_CODE(x)                (((x) >> CY_RSLT_CODE_POSITION) & CY_RSLT_CODE_MASK)
/** Get the value of the result type field */
#define CY_RSLT_GET_TYPE(x)                (((x) >> CY_RSLT_TYPE_POSITION) & CY_RSLT_TYPE_MASK)
/** Get the value of the module identifier field */
#define CY_RSLT_GET_MODULE(x)              (((x) >> CY_RSLT_MODULE_POSITION) & CY_RSLT_MODULE_MASK)


/**** DRIVER Module codes: 0x0000 - 0x00FF ****/
/** Base identifier for peripheral driver library */
#define CY_RSLT_MODULE_DRIVERS_PDL_BASE           (0x0000U)
/** Base identifier for peripheral driver library */
#define CY_RSLT_MODULE_DRIVERS_WHD_BASE           (0x0080U)

/**** ABSTRACTION Module codes: 0x0100 - 0x01FF ****/
/** Base identifier for chip support modules */
#define CY_RSLT_MODULE_ABSTRACTION_HAL_BASE       (0x0100U)
/** Base identifier for board support modules */
#define CY_RSLT_MODULE_ABSTRACTION_BSP            (0x0180U)
/** Base identifier for file system modules */
#define CY_RSLT_MODULE_ABSTRACTION_FS             (0x0181U)
/** Base identifier for resource abstraction modules */
#define CY_RSLT_MODULE_ABSTRACTION_RESOURCE       (0x0182U)
/** Base identifier for rtos abstraction modules */
#define CY_RSLT_MODULE_ABSTRACTION_OS             (0x0183U)
/** Base identifier for environment abstraction modules */
#define CY_RSLT_MODULE_ABSTRACTION_ENV            (0x0184U)

/** Middleware Module codes: 0x0200 - 0x02FF */
#define CY_RSLT_MODULE_MIDDLEWARE_BASE            (0x0200U)



/** Provides the result of an operation as a structured bitfield */
typedef uint32_t cy_rslt_t;

/** Result value indicating success */
#define CY_RSLT_SUCCESS                    ((cy_rslt_t)0x00000000U)

/** Create a result value from the specified type, module, and result code */
#define CY_RSLT_CREATE(type, module, code) \
    ((((module) & CY_RSLT_MODULE_MASK) << CY_RSLT_MODULE_POSITION) | \
    (((code) & CY_RSLT_CODE_MASK) << CY_RSLT_CODE_POSITION) | \
    (((type) & CY_RSLT_TYPE_MASK) << CY_RSLT_TYPE_POSITION))

/** \} group_result_macros */

#ifdef __cplusplus
}
#endif

/** \} group_result */
