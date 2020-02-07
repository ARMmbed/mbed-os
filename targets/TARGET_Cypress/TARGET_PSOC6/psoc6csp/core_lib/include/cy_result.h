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
* Copyright 2018-2020 Cypress Semiconductor Corporation
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
 \anchor anchor_general_description 
* Defines a type and related utilities for function result handling. 
* 
* The cy_rslt_t type is a structured bitfield which encodes information
* about result type, the originating module, and a code for the specific 
* error (or warning etc). In order to extract these individual fields from 
* a cy_rslt_t value, the utility macros @ref CY_RSLT_GET_TYPE, @ref CY_RSLT_GET_MODULE, 
* and @ref CY_RSLT_GET_CODE are provided. For example:
* \code
* cy_rslt_t result = cy_hal_do_operation(arg);
* // Will be CY_RSLT_TYPE_INFO, CY_RSLT_TYPE_WARNING, CY_RSLT_TYPE_ERROR, or CY_RSLT_TYPE_FATAL
* uint8_t type = CY_RSLT_GET_TYPE(result)
* // See the "Modules" section for possible values
* uint16_t module_id = CY_RSLT_GET_MODULE(result);
* // Specific error codes are defined by each module
* uint16_t error_code = CY_RSLT_GET_CODE(result);
* \endcode
*/

#if !defined(CY_RESULT_H)
#define CY_RESULT_H

#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

/** 
  * @brief Provides the result of an operation as a structured bitfield. 
  * 
  * See the \ref anchor_general_description "General Description" 
  * for more details on structure and usage.
  */
typedef uint32_t cy_rslt_t;

/** @ref cy_rslt_t return value indicating success */
#define CY_RSLT_SUCCESS                    ((cy_rslt_t)0x00000000U)

/** \cond INTERNAL */
/** Mask for the bit at position "x" */
#define CY_BIT_MASK(x)                     ((1UL << (x)) - 1U)

/** Bit position of the result type */
#define CY_RSLT_TYPE_POSITION              (16U)
/** Bit width of the result type */
#define CY_RSLT_TYPE_WIDTH                 (2U)
/** Bit position of the module identifier */
#define CY_RSLT_MODULE_POSITION            (18U)
/** Bit width of the module identifier */
#define CY_RSLT_MODULE_WIDTH               (14U)
/** Bit position of the result code */
#define CY_RSLT_CODE_POSITION              (0U)
/** Bit width of the result code */
#define CY_RSLT_CODE_WIDTH                 (16U)

/** Mask for the result type */
#define CY_RSLT_TYPE_MASK                  CY_BIT_MASK(CY_RSLT_TYPE_WIDTH)
/** Mask for the module identifier */
#define CY_RSLT_MODULE_MASK                CY_BIT_MASK(CY_RSLT_MODULE_WIDTH)
/** Mask for the result code */
#define CY_RSLT_CODE_MASK                  CY_BIT_MASK(CY_RSLT_CODE_WIDTH)

/** \endcond */

/**
* \{
* @name Fields
* Utility macros for constructing result values and extracting individual fields from existing results.
*/

/** 
  * @brief Get the value of the result type field 
  * @param x the @ref cy_rslt_t value from which to extract the result type
  */
#define CY_RSLT_GET_TYPE(x)                (((x) >> CY_RSLT_TYPE_POSITION) & CY_RSLT_TYPE_MASK)
/** 
  * @brief Get the value of the module identifier field 
  * @param x the @ref cy_rslt_t value from which to extract the module id
  */
#define CY_RSLT_GET_MODULE(x)              (((x) >> CY_RSLT_MODULE_POSITION) & CY_RSLT_MODULE_MASK)
/**
  * @brief Get the value of the result code field
  * @param x the @ref cy_rslt_t value from which to extract the result code
  */
#define CY_RSLT_GET_CODE(x)                (((x) >> CY_RSLT_CODE_POSITION) & CY_RSLT_CODE_MASK)

/** 
  * @brief Create a new @ref cy_rslt_t value that encodes the specified type, module, and result code.  
  * @param type one of @ref CY_RSLT_TYPE_INFO, @ref CY_RSLT_TYPE_WARNING, 
  *  @ref CY_RSLT_TYPE_ERROR, @ref CY_RSLT_TYPE_FATAL  
  * @param module Identifies the module where this result originated; see @ref anchor_modules "Modules".  
  * @param code a module-defined identifier to identify the specific situation that 
  * this result describes.
  */
#define CY_RSLT_CREATE(type, module, code) \
    ((((module) & CY_RSLT_MODULE_MASK) << CY_RSLT_MODULE_POSITION) | \
    (((code) & CY_RSLT_CODE_MASK) << CY_RSLT_CODE_POSITION) | \
    (((type) & CY_RSLT_TYPE_MASK) << CY_RSLT_TYPE_POSITION))

/** \} fields */

/**
* \{
*@name Result Types
*/

/** @brief The result code is informational-only */
#define CY_RSLT_TYPE_INFO                  (0U)
/** @brief The result code is a warning */
#define CY_RSLT_TYPE_WARNING               (1U)
/** @brief The result code is an error */
#define CY_RSLT_TYPE_ERROR                 (2U)
/** @brief The result code is a fatal error */
#define CY_RSLT_TYPE_FATAL                 (3U)

/** \} severity */

/**
* \{
@name Modules
@anchor anchor_modules
* Defines codes to identify the module from which an error originated.
* For some large libraries, a range of module codes is defined here;
* see the library documentation for values corresonding to individual modules.
*/
/**** DRIVER Module codes: 0x0000 - 0x00FF ****/
/** Base module identifier for peripheral driver library drivers (0x0000 - 0x007F) */
#define CY_RSLT_MODULE_DRIVERS_PDL_BASE             (0x0000U)
/** Base module identifier for wireless host driver library modules (0x0080 - 0x00FF) */
#define CY_RSLT_MODULE_DRIVERS_WHD_BASE             (0x0080U)

/** Base module identifier for HAL drivers (0x0100 - 0x017F) */
#define CY_RSLT_MODULE_ABSTRACTION_HAL_BASE         (0x0100U)
/** Module identifier for board support package */
#define CY_RSLT_MODULE_ABSTRACTION_BSP              (0x0180U)
/** Module identifier for file system abstraction */
#define CY_RSLT_MODULE_ABSTRACTION_FS               (0x0181U)
/** Module identifier for resource abstraction */
#define CY_RSLT_MODULE_ABSTRACTION_RESOURCE         (0x0182U)
/** Module identifier for rtos abstraction */
#define CY_RSLT_MODULE_ABSTRACTION_OS               (0x0183U)
/** Base identifier for environment abstraction modules (0x0184 - 0x01FF) */
#define CY_RSLT_MODULE_ABSTRACTION_ENV              (0x0184U)

/** Base module identifier for Board Libraries (0x01A0 - 0x01BF) */
#define CY_RSLT_MODULE_BOARD_LIB_BASE               (0x01A0U)
/** Module identifier for the Retarget IO Board Library */
#define CY_RSLT_MODULE_BOARD_LIB_RETARGET_IO        (0x1A0U)
/** Module identifier for the RGB LED Board Library */
#define CY_RSLT_MODULE_BOARD_LIB_RGB_LED            (0x01A1U)
/** Module identifier for the Serial Flash Board Library */
#define CY_RSLT_MODULE_BOARD_LIB_SERIAL_FLASH       (0x01A2U)

/** Base module identifier for Shield Board Libraries (0x01C0 - 0x01FF) */
#define CY_RSLT_MODULE_BOARD_SHIELD_BASE            (0x01C0U)
/** Module identifier for Shield Board CY8CKIT-028-EPD */
#define CY_RSLT_MODULE_BOARD_SHIELD_028_EPD         (0x01C0U)
/** Module identifier for Shield Board CY8CKIT-028-TFT */
#define CY_RSLT_MODULE_BOARD_SHIELD_028_TFT         (0x01C1U)


/** Base module identifier for Middleware Libraries (0x0200 - 0x02FF) */
#define CY_RSLT_MODULE_MIDDLEWARE_BASE              (0x0200U)

/** \} modules */

#ifdef __cplusplus
}
#endif

#endif /* CY_RESULT_H */

/** \} group_result */
