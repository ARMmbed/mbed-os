/***************************************************************************//**
* \file cy_syslib.h
* \version 2.40.1
*
* Provides an API declaration of the SysLib driver.
*
********************************************************************************
* \copyright
* Copyright 2016-2019 Cypress Semiconductor Corporation
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
* \addtogroup group_syslib
* \{
* The system libraries provide APIs that can be called in the user application
* to handle the timing, logical checking or register.
*
* The functions and other declarations used in this driver are in cy_syslib.h. 
* You can include cy_pdl.h (ModusToolbox only) to get access to all functions 
* and declarations in the PDL. 
*
* The SysLib driver contains a set of different system functions. These functions
* can be called in the application routine. Major features of the system library:
* * Delay functions
* * The register Read/Write macro
* * Assert and Halt
* * Assert Classes and Levels
* * A software reset
* * Reading the reset cause
* * An API to invalidate the flash cache and buffer
* * Data manipulation macro
* * A variable type definition from MISRA-C which specifies signedness
* * Cross compiler compatible attributes
* * Getting a silicon-unique ID API
* * Setting wait states API
* * Resetting the backup domain API
* * APIs to serve Fault handler
*
* \section group_syslib_configuration Configuration Considerations
* <b> Assertion Usage </b> <br />
* Use the CY_ASSERT() macro to check expressions that must be true as long as the
* program is running correctly. It is a convenient way to insert sanity checks.
* The CY_ASSERT() macro is defined in the cy_syslib.h file which is part of
* the PDL library. The behavior of the macro is as follows: if the expression
* passed to the macro is false, output an error message that includes the file
* name and line number, and then halts the CPU. \n
* In case of fault, the CY_ASSERT() macro calls the Cy_SysLib_AssertFailed() function.
* This is a weakly linked function. The default implementation stores the file
* name and line number of the ASSERT into global variables, cy_assertFileName
* and cy_assertLine . It then calls the Cy_SysLib_Halt() function.
* \note Firmware can redefine the Cy_SysLib_AssertFailed() function for custom processing.
* 
* The PDL source code uses this assert mechanism extensively. It is recommended
* that you enable asserts when debugging firmware. \n
* <b> Assertion Classes and Levels </b> <br />
* The PDL defines three assert classes, which correspond to different kinds
* of parameters. There is a corresponding assert "level" for each class.
* <table class="doxtable">
*   <tr><th>Class Macro</th><th>Level Macro</th><th>Type of check</th></tr>
*   <tr>
*     <td>CY_ASSERT_CLASS_1</td>
*     <td>CY_ASSERT_L1</td>
*     <td>A parameter that could change between different PSoC devices
*         (e.g. the number of clock paths)</td>
*   </tr>
*   <tr>
*     <td>CY_ASSERT_CLASS_2</td>
*     <td>CY_ASSERT_L2</td>
*     <td>A parameter that has fixed limits such as a counter period</td>
*   </tr>
*   <tr>
*     <td>CY_ASSERT_CLASS_3</td>
*     <td>CY_ASSERT_L3</td>
*     <td>A parameter that is an enum constant</td>
*   </tr>
* </table>
* Firmware defines which ASSERT class is enabled by defining CY_ASSERT_LEVEL.
* This is a compiler command line argument, similar to how the DEBUG / NDEBUG
* macro is passed. \n 
* Enabling any class also enables any lower-numbered class.
* CY_ASSERT_CLASS_3 is the default level, and it enables asserts for all three
* classes. The following example shows the command-line option to enable all
* the assert levels:
* \code -D CY_ASSERT_LEVEL=CY_ASSERT_CLASS_3 \endcode
* \note The use of special characters, such as spaces, parenthesis, etc. must
* be protected with quotes. 
* 
* After CY_ASSERT_LEVEL is defined, firmware can use
* one of the three level macros to make an assertion. For example, if the
* parameter can vary between devices, firmware uses the L1 macro.
* \code CY_ASSERT_L1(clkPath < SRSS_NUM_CLKPATH); \endcode
* If the parameter has bounds, firmware uses L2.
* \code CY_ASSERT_L2(trim <= CY_CTB_TRIM_VALUE_MAX); \endcode
* If the parameter is an enum, firmware uses L3.
* \code CY_ASSERT_L3(config->LossAction <= CY_SYSCLK_CSV_ERROR_FAULT_RESET); \endcode
* Each check uses the appropriate level macro for the kind of parameter being checked.
* If a particular assert class/level is not enabled, then the assert does nothing.
*
* \section group_syslib_more_information More Information
* Refer to the technical reference manual (TRM).
*
* \section group_syslib_MISRA MISRA-C Compliance
* <table class="doxtable">
*   <tr>
*     <th>MISRA Rule</th>
*     <th>Rule Class (Required/Advisory)</th>
*     <th>Rule Description</th>
*     <th>Description of Deviation(s)</th>
*   </tr>
*   <tr>
*     <td>1.2</td>
*     <td>R</td>
*     <td>No reliance shall be placed on undefined or unspecified behaviour.</td>
*     <td>This specific behavior is explicitly covered in rule 20.1.</td>
*   </tr>
*   <tr>
*     <td>2.1</td>
*     <td>R</td>
*     <td>This function contains a mixture of in-line assembler statements and C statements.</td>
*     <td>This si required by design of the Cy_SysLib_Halt function.</td>
*   </tr>
*   <tr>
*     <td>18.4</td>
*     <td>R</td>
*     <td>Unions shall not be used.</td>
*     <td>The unions are used for CFSR, HFSR and SHCSR Fault Status Registers
*         content access as a word in code and as a structure during debug.</td>
*   </tr>
*   <tr>
*     <td>19.13</td>
*     <td>A</td>
*     <td>The # and ## operators should not be used.</td>
*     <td>The ## preprocessor operator is used in macros to form the field mask.</td>
*   </tr>
*   <tr>
*     <td>20.1</td>
*     <td>R</td>
*     <td>Reserved identifiers, macros and functions in the standard library, shall not be 
*         defined, redefined or undefined.</td>
*     <td>The driver defines the macros with leading underscores 
*         (_CLR_SET_FLD/_BOOL2FLD/_FLD2BOOL) and therefore generates this MISRA violation.</td>
*   </tr>
* </table>
*
* \section group_syslib_changelog Changelog
* <table class="doxtable">
*   <tr><th>Version</th><th>Changes</th><th>Reason for Change</th></tr>
*   <tr>
*     <td>2.40.1</td>
*     <td>Correct the CY_RAMFUNC_BEGIN macro for the IAR compiler.</td>
*     <td>Removed the IAR compiler warning.</td>
*   </tr>
*   <tr>
*     <td>2.40</td>
*     <td>Added new macros CY_SYSLIB_DIV_ROUND and CY_SYSLIB_DIV_ROUNDUP to easy perform integer division with rounding.</td>
*     <td>Improve PDL code base.</td>
*   </tr>
*   <tr>
*     <td rowspan="3">2.30</td>
*     <td>Updated implementation of the Cy_SysLib_AsmInfiniteLoop() function to be compatible with ARMC6.</td>
*     <td>Provided support for the ARM Compiler 6.</td>
*   </tr>
*   <tr>
*     <td>Minor documentation edits.</td>
*     <td>Documentation update and clarification.</td>
*   </tr>
*	<tr>
*     <td>Added new macros CY_RAMFUNC_BEGIN and CY_RAMFUNC_END for convenient placement function in RAM for all supported compilers.</td>
*     <td>Improve user experience.</td>
*   </tr>
*   <tr>
*     <td rowspan="2">2.20</td>
*     <td>Updated implementation of the \ref Cy_SysLib_AssertFailed() function to be available in Release and Debug modes.</td>
*     <td>Provided support for the PDL static library in Release mode.</td>
*   </tr>
*   <tr>
*     <td>Minor documentation edits.</td>
*     <td>Documentation update and clarification.</td>
*   </tr>
*   <tr>
*     <td rowspan="4">2.10</td>
*     <td>Flattened the organization of the driver source code into the single source directory and the single include directory.</td>
*     <td>Driver library directory-structure simplification.</td>
*   </tr>
*   <tr>
*     <td>Added the following macros: \ref CY_REG32_CLR_SET, \ref _CLR_SET_FLD16U, \ref CY_REG16_CLR_SET, \ref _CLR_SET_FLD8U, \ref CY_REG8_CLR_SET</td>
*     <td>Register access simplification.</td>
*   </tr>
*   <tr>
*     <td>Removed the Cy_SysLib_GetNumHfclkResetCause API function.</td>
*     <td>This feature is not supported by SRSS_ver1.</td>
*   </tr>
*   <tr>
*     <td>Added register access layer. Use register access macros instead
*         of direct register access using dereferenced pointers.</td>
*     <td>Makes register access device-independent, so that the PDL does 
*         not need to be recompiled for each supported part number.</td>
*   </tr>
*   <tr>
*     <td>2.0.1</td>
*     <td>Minor documentation edits</td>
*     <td>Documentation update and clarification</td>
*   </tr>
*   <tr>
*     <td rowspan="4"> 2.0</td>
*     <td>
* Added Cy_SysLib_ResetBackupDomain() API implementation. \n
* Added CY_NOINLINE attribute implementation. \n
* Added DIE_YEAR field to 64-bit unique ID return value of Cy_SysLib_GetUniqueId() API. \n
* Added storing of SCB->HFSR, SCB->SHCSR registers and SCB->MMFAR, SCB->BFAR addresses to Fault Handler debug structure. \n
* Optimized Cy_SysLib_SetWaitStates() API implementation.
*     </td>
*     <td>Improvements made based on usability feedback.</td>
*   </tr>
*   <tr>
*     <td>Added Assertion Classes and Levels.</td>
*     <td>For error checking, parameter validation and status returns in the PDL API.</td>
*   </tr>
*   <tr>
*     <td>Applied CY_NOINIT attribute to cy_assertFileName, cy_assertLine, and cy_faultFrame global variables.</td>
*     <td>To store debug information into a non-zero init area for future analysis.</td>
*   </tr>
*   <tr>
*     <td>Removed CY_WEAK attribute implementation.</td>
*     <td>CMSIS __WEAK attribute should be used instead.</td>
*   </tr>
*   <tr>
*     <td>1.0</td>
*     <td>Initial version</td>
*     <td></td>
*   </tr>
* </table>
*
* \defgroup group_syslib_macros Macros
* \defgroup group_syslib_functions Functions
* \defgroup group_syslib_data_structures Data Structures
* \defgroup group_syslib_enumerated_types Enumerated Types
*
*/

#if !defined(CY_SYSLIB_H)
#define CY_SYSLIB_H

#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
*  Data manipulation defines
*******************************************************************************/

/** Get the lower 8 bits of a 16-bit value. */
#define CY_LO8(x)               ((uint8_t) ((x) & 0xFFU))
/** Get the upper 8 bits of a 16-bit value. */
#define CY_HI8(x)               ((uint8_t) ((uint16_t)(x) >> 8U))

/** Get the lower 16 bits of a 32-bit value. */
#define CY_LO16(x)              ((uint16_t) ((x) & 0xFFFFU))
/** Get the upper 16 bits of a 32-bit value. */
#define CY_HI16(x)              ((uint16_t) ((uint32_t)(x) >> 16U))

/** Swap the byte ordering of a 16-bit value */
#define CY_SWAP_ENDIAN16(x)     ((uint16_t)(((x) << 8U) | (((x) >> 8U) & 0x00FFU)))

/** Swap the byte ordering of a 32-bit value */
#define CY_SWAP_ENDIAN32(x)     ((uint32_t)((((x) >> 24U) & 0x000000FFU) | (((x) & 0x00FF0000U) >> 8U) | \
                                (((x) & 0x0000FF00U) << 8U) | ((x) << 24U)))

/** Swap the byte ordering of a 64-bit value */
#define CY_SWAP_ENDIAN64(x)     ((uint64_t) (((uint64_t) CY_SWAP_ENDIAN32((uint32_t)(x)) << 32U) | \
                                CY_SWAP_ENDIAN32((uint32_t)((x) >> 32U))))

/* MISRA rule 6.3 recommends using specific-length typedef for the basic
 * numerical types of signed and unsigned variants of char, float, and double.
 */
typedef char     char_t;    /**< Specific-length typedef for the basic numerical types of char */
typedef float    float32_t; /**< Specific-length typedef for the basic numerical types of float */
typedef double   float64_t; /**< Specific-length typedef for the basic numerical types of double */


/*******************************************************************************
* Macro Name: CY_GET_REG8(addr)
****************************************************************************//**
*
* Reads the 8-bit value from the specified address. This function can't be
* used to access the Core register, otherwise a fault occurs.
*
* \param addr  The register address.
*
* \return The read value.
*
*******************************************************************************/
#define CY_GET_REG8(addr)           (*((const volatile uint8_t *)(addr)))


/*******************************************************************************
* Macro Name: CY_SET_REG8(addr, value)
****************************************************************************//**
*
* Writes an 8-bit value to the specified address. This function can't be
* used to access the Core register, otherwise a fault occurs.
*
* \param addr  The register address.
*
* \param value The value to write.
*
*******************************************************************************/
#define CY_SET_REG8(addr, value)    (*((volatile uint8_t *)(addr)) = (uint8_t)(value))


/*******************************************************************************
* Macro Name: CY_GET_REG16(addr)
****************************************************************************//**
*
* Reads the 16-bit value from the specified address.
*
* \param addr  The register address.
*
* \return The read value.
*
*******************************************************************************/
#define CY_GET_REG16(addr)          (*((const volatile uint16_t *)(addr)))


/*******************************************************************************
* Macro Name: CY_SET_REG16(addr, value)
****************************************************************************//**
*
* Writes the 16-bit value to the specified address.
*
* \param addr  The register address.
*
* \param value The value to write.
*
*******************************************************************************/
#define CY_SET_REG16(addr, value)   (*((volatile uint16_t *)(addr)) = (uint16_t)(value))


/*******************************************************************************
* Macro Name: CY_GET_REG24(addr)
****************************************************************************//**
*
* Reads the 24-bit value from the specified address.
*
* \param addr  The register address.
*
* \return The read value.
*
*******************************************************************************/
#define CY_GET_REG24(addr)          (((uint32_t) (*((const volatile uint8_t *)(addr)))) | \
                                    (((uint32_t) (*((const volatile uint8_t *)(addr) + 1))) << 8U) | \
                                    (((uint32_t) (*((const volatile uint8_t *)(addr) + 2))) << 16U))                                    


/*******************************************************************************
* Macro Name: CY_SET_REG24(addr, value)
****************************************************************************//**
*
* Writes the 24-bit value to the specified address.
*
* \param addr  The register address.
*
* \param value The value to write.
*
*******************************************************************************/
#define CY_SET_REG24(addr, value)   do  \
                                    {   \
                                        (*((volatile uint8_t *) (addr))) = (uint8_t)(value);                \
                                        (*((volatile uint8_t *) (addr) + 1)) = (uint8_t)((value) >> 8U);    \
                                        (*((volatile uint8_t *) (addr) + 2)) = (uint8_t)((value) >> 16U);   \
                                    }   \
                                    while(0)


/*******************************************************************************
* Macro Name: CY_GET_REG32(addr)
****************************************************************************//**
*
* Reads the 32-bit value from the specified register. The address is the little
* endian order (LSB in lowest address).
*
* \param addr  The register address.
*
* \return The read value.
*
*******************************************************************************/
#define CY_GET_REG32(addr)          (*((const volatile uint32_t *)(addr)))


/*******************************************************************************
* Macro Name: CY_SET_REG32(addr, value)
****************************************************************************//**
*
* Writes the 32-bit value to the specified register. The address is the little
* endian order (LSB in lowest address).
*
* \param addr  The register address.
*
* \param value The value to write.
*
*******************************************************************************/
#define CY_SET_REG32(addr, value)   (*((volatile uint32_t *)(addr)) = (uint32_t)(value))

/*******************************************************************************
* Macro Name: _CLR_SET_FLD32U
****************************************************************************//**
*
*  The macro for setting a register with a name field and value for providing
*  get-clear-modify-write operations.
*  Returns a resulting value to be assigned to the register.
*
*******************************************************************************/
#define _CLR_SET_FLD32U(reg, field, value) (((reg) & ((uint32_t)(~(field ## _Msk)))) | (_VAL2FLD(field, value)))


/*******************************************************************************
* Macro Name: CY_REG32_CLR_SET
****************************************************************************//**
*
*  Uses _CLR_SET_FLD32U macro for providing get-clear-modify-write 
*  operations with a name field and value and writes a resulting value 
*  to the 32-bit register.
*
*******************************************************************************/
#define CY_REG32_CLR_SET(reg, field, value) ((reg) = _CLR_SET_FLD32U((reg), field, (value)))


/*******************************************************************************
* Macro Name: _CLR_SET_FLD16U
****************************************************************************//**
*
*  The macro for setting a 16-bit register with a name field and value for providing
*  get-clear-modify-write operations.
*  Returns a resulting value to be assigned to the 16-bit register.
*
*******************************************************************************/
#define _CLR_SET_FLD16U(reg, field, value) ((uint16_t)(((reg) & ((uint16_t)(~(field ## _Msk)))) |   \
                                                       ((uint16_t)_VAL2FLD(field, value))))
                                                       
                                                       
/*******************************************************************************
* Macro Name: CY_REG16_CLR_SET
****************************************************************************//**
*
*  Uses _CLR_SET_FLD16U macro for providing get-clear-modify-write 
*  operations with a name field and value and writes a resulting value 
*  to the 16-bit register.
*
*******************************************************************************/
#define CY_REG16_CLR_SET(reg, field, value) ((reg) = _CLR_SET_FLD16U((reg), field, (value)))


/*******************************************************************************
* Macro Name: _CLR_SET_FLD8U
****************************************************************************//**
*
*  The macro for setting a 8-bit register with a name field and value for providing
*  get-clear-modify-write operations.
*  Returns a resulting value to be assigned to the 8-bit register.
*
*******************************************************************************/
#define _CLR_SET_FLD8U(reg, field, value) ((uint8_t)(((reg) & ((uint8_t)(~(field ## _Msk)))) |  \
                                                     ((uint8_t)_VAL2FLD(field, value))))
                                                     
                                                     
/*******************************************************************************
* Macro Name: CY_REG8_CLR_SET
****************************************************************************//**
*
*  Uses _CLR_SET_FLD8U macro for providing get-clear-modify-write 
*  operations with a name field and value and writes a resulting value 
*  to the 8-bit register.
*
*******************************************************************************/
#define CY_REG8_CLR_SET(reg, field, value) ((reg) = _CLR_SET_FLD8U((reg), field, (value)))


/*******************************************************************************
* Macro Name: _BOOL2FLD
****************************************************************************//**
*
*  Returns a field mask if the value is not false.
*  Returns 0, if the value is false.
*
*******************************************************************************/
#define _BOOL2FLD(field, value) (((value) != false) ? (field ## _Msk) : 0UL)


/*******************************************************************************
* Macro Name: _FLD2BOOL
****************************************************************************//**
*
*  Returns true, if the value includes the field mask.
*  Returns false, if the value doesn't include the field mask.
*
*******************************************************************************/
#define _FLD2BOOL(field, value) (((value) & (field ## _Msk)) != 0UL)


/*******************************************************************************
* Macro Name: CY_SYSLIB_DIV_ROUND
****************************************************************************//**
*
*  Calculates a / b with rounding to the nearest integer,
*  a and b must have the same sign.
*
*******************************************************************************/
#define CY_SYSLIB_DIV_ROUND(a, b) (((a) + ((b) / 2U)) / (b))


/*******************************************************************************
* Macro Name: CY_SYSLIB_DIV_ROUNDUP
****************************************************************************//**
*
*  Calculates a / b with rounding up if remainder != 0,
*  both a and b must be positive.
*
*******************************************************************************/
#define CY_SYSLIB_DIV_ROUNDUP(a, b) ((((a) - 1U) / (b)) + 1U)

typedef uint32_t cy_status;
/** The ARM 32-bit status value for backward compatibility with the UDB components. Do not use it in your code. */
typedef uint32_t cystatus;
typedef uint8_t  uint8;    /**< Alias to uint8_t  for backward compatibility */
typedef uint16_t uint16;   /**< Alias to uint16_t for backward compatibility */
typedef uint32_t uint32;   /**< Alias to uint32_t for backward compatibility */
typedef int8_t   int8;     /**< Alias to int8_t   for backward compatibility */
typedef int16_t  int16;    /**< Alias to int16_t  for backward compatibility */
typedef int32_t  int32;    /**< Alias to int32_t  for backward compatibility */
typedef float    float32;  /**< Alias to float    for backward compatibility */
typedef double   float64;  /**< Alias to double   for backward compatibility */
typedef int64_t  int64;    /**< Alias to int64_t  for backward compatibility */
typedef uint64_t uint64;   /**< Alias to uint64_t for backward compatibility */
/* Signed or unsigned depending on the compiler selection */
typedef char     char8;    /**< Alias to char for backward compatibility */
typedef volatile uint8_t  reg8;   /**< Alias to uint8_t  for backward compatibility */
typedef volatile uint16_t reg16;  /**< Alias to uint16_t for backward compatibility */
typedef volatile uint32_t reg32;  /**< Alias to uint32_t for backward compatibility */

/** The ARM 32-bit Return error / status code for backward compatibility.
*  Do not use them in your code.
*/
#define CY_RET_SUCCESS           (0x00U)    /* Successful */
#define CY_RET_BAD_PARAM         (0x01U)    /* One or more invalid parameters */
#define CY_RET_INVALID_OBJECT    (0x02U)    /* An invalid object specified */
#define CY_RET_MEMORY            (0x03U)    /* A memory-related failure */
#define CY_RET_LOCKED            (0x04U)    /* A resource lock failure */
#define CY_RET_EMPTY             (0x05U)    /* No more objects available */
#define CY_RET_BAD_DATA          (0x06U)    /* Bad data received (CRC or other error check) */
#define CY_RET_STARTED           (0x07U)    /* Operation started, but not necessarily completed yet */
#define CY_RET_FINISHED          (0x08U)    /* Operation is completed */
#define CY_RET_CANCELED          (0x09U)    /* Operation is canceled */
#define CY_RET_TIMEOUT           (0x10U)    /* Operation timed out */
#define CY_RET_INVALID_STATE     (0x11U)    /* Operation is not setup or is in an improper state */
#define CY_RET_UNKNOWN           ((cy_status) 0xFFFFFFFFU)    /* Unknown failure */

/** ARM 32-bit Return error / status codes for backward compatibility with the UDB components.
*  Do not use them in your code.
*/
#define CYRET_SUCCESS            (0x00U)    /* Successful */
#define CYRET_BAD_PARAM          (0x01U)    /* One or more invalid parameters */
#define CYRET_INVALID_OBJECT     (0x02U)    /* An invalid object specified */
#define CYRET_MEMORY             (0x03U)    /* A memory-related failure */
#define CYRET_LOCKED             (0x04U)    /* A resource lock failure */
#define CYRET_EMPTY              (0x05U)    /* No more objects available */
#define CYRET_BAD_DATA           (0x06U)    /* Bad data received (CRC or other error check) */
#define CYRET_STARTED            (0x07U)    /* Operation started, but not necessarily completed yet */
#define CYRET_FINISHED           (0x08U)    /* Operation is completed */
#define CYRET_CANCELED           (0x09U)    /* Operation is canceled */
#define CYRET_TIMEOUT            (0x10U)    /* Operation timed out */
#define CYRET_INVALID_STATE      (0x11U)    /* Operation is not setup or is in an improper state */
#define CYRET_UNKNOWN            ((cystatus) 0xFFFFFFFFU)    /* Unknown failure */

/** \} group_syslib_functions */

#endif /* CY_SYSLIB_H */

/** \} group_syslib */

/* [] END OF FILE */
