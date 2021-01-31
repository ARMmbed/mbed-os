/***************************************************************************//**
* \file cy_syslib.h
* \version 2.70
*
* Provides an API declaration of the SysLib driver.
*
********************************************************************************
* \copyright
* Copyright 2016-2020 Cypress Semiconductor Corporation
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
* You can include cy_pdl.h to get access to all functions
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
* Use the CY_ASSERT() macro to check expressions that must be true if the
* program is running correctly. It is a convenient way to insert sanity checks.
* The CY_ASSERT() macro is defined in the cy_utils.h file, which is part of the
* <a href="https://github.com/cypresssemiconductorco/core-lib">Cypress Core Library (core-lib)</a>.
* The macro behavior is as follows: if the expression passed
*  to the macro is false, the CPU is halted. \n
*
* Starting from the driver version 2.50, the CY_ASSERT macro was moved
* to the Cypress Core Library (core-lib). Also, the CY_ASSERT implementation
* was changed not to call Cy_SysLib_AssertFailed() function, so user application
* that relied on that should take this change into account. \n
*
*
* The PDL source code uses this assert mechanism extensively. It is recommended
* that you enable asserts when debugging firmware. \n
* <b> Assertion Classes and Levels </b> <br />
* The <a href="https://github.com/cypresssemiconductorco/core-lib">Cypress Core Library</a>
* defines three assert classes, which correspond to different
* kinds of parameters. There is a corresponding assert "level" for each class.
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
* \section group_syslib_changelog Changelog
* <table class="doxtable">
*   <tr><th>Version</th><th>Changes</th><th>Reason for Change</th></tr>
*   <tr>
*     <td rowspan="4">2.70</td>
*     <td>Added new macros CY_SECTION_RAMFUNC_BEGIN, CY_SECTION_RAMFUNC_END,
*         CY_SECTION_SHAREDMEM to enable overriding of the linker section placement.</td>
*     <td>Enhancement based on usability feedback.</td>
*   </tr>
*   <tr>
*     <td>Noted that implementation of CY_ASSERT() was changed back in version 2.50,
*         so that Cy_SysLib_AssertFailed() function is not called and user application
*         may need to be updated.</td>
*     <td>Documentation update.</td>
*   </tr>
*   <tr>
*     <td>Removed the issue related to the malloc() failure to report error for the case when
*         requested allocation size is bigger than the heap size.
*         Refer to the \ref group_system_config_heap_stack_config_gcc section for the more details.
*         Removed empty Known Issues section.
*     <td>Documentation update and clarification.</td>
*   </tr>
*   <tr>
*     <td>Fixed/Documented MISRA 2012 violations.</td>
*     <td>MISRA 2012 compliance.</td>
*   </tr>
*   <tr>
*     <td>2.60.1</td>
*     <td>Updated the Configuration Considerations section with the information that
*         CY_ASSERT() macro is defined in the cy_utils.h file, which is part of the
*         <a href="https://github.com/cypresssemiconductorco/core-lib">Cypress Core Library (core-lib)</a>
*     <td>Documentation update and clarification.</td>
*   </tr>
*   <tr>
*     <td rowspan="2">2.60</td>
*     <td>Updated the following functions for the PSoC 64 devices:
*         \ref Cy_SysLib_ClearFlashCacheAndBuffer, \ref Cy_SysLib_ClearResetReason,
*         \ref Cy_SysLib_SetWaitStates.
*     <td>Added PSoC 64 device support.</td>
*   </tr>
*   <tr>
*     <td>Minor documentation updates.</td>
*     <td>Documentation enhancement.</td>
*   </tr>
*   <tr>
*     <td>2.50.3</td>
*     <td>Add section Known Issues
*     <td>Documentation update and clarification.</td>
*   </tr>
*   <tr>
*     <td>2.50.1</td>
*     <td>Used the core library defines for the message codes forming.
*     <td>Improve PDL code base.</td>
*   </tr>
*   <tr>
*     <td>2.50</td>
*     <td>Moved following macros to the core library:
*         CY_LO8,CY_HI8,CY_LO16,CY_HI16,CY_SWAP_ENDIAN16,CY_SWAP_ENDIAN32,
*         CY_SWAP_ENDIAN64,CY_GET_REG8,CY_SET_REG8,CY_GET_REG16,CY_SET_REG16,
*         CY_GET_REG24,CY_SET_REG24,CY_GET_REG32,CY_SET_REG32,_CLR_SET_FLD32U,
*         CY_REG32_CLR_SET,_CLR_SET_FLD16U,CY_REG16_CLR_SET,_CLR_SET_FLD8U,
*         CY_REG8_CLR_SET,_BOOL2FLD,_FLD2BOOL,CY_SYSLIB_DIV_ROUND,
*         CY_SYSLIB_DIV_ROUNDUP,CY_NOINIT,CY_SECTION,CY_UNUSED,CY_NOINLINE,
*         CY_ALIGN,CY_RAMFUNC_BEGIN,CY_RAMFUNC_END.
*         Use at least version 1.1 of the core library: https://github.com/cypresssemiconductorco/core-lib.
*     <td>Improve PDL code base.</td>
*   </tr>
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
*   <tr>
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
*     <td>Added the following macros: CY_REG32_CLR_SET, _CLR_SET_FLD16U, CY_REG16_CLR_SET, _CLR_SET_FLD8U, CY_REG8_CLR_SET</td>
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
#include "cy_utils.h"
#include "cy_result.h"
#include "cy_device.h"
#include "cy_device_headers.h"

#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#if defined( __ICCARM__ )
    /* Suppress the warning for multiple volatile variables in an expression. */
    /* This is common for driver's code and the usage is not order-dependent. */
    #pragma diag_suppress=Pa082
#endif  /* defined( __ICCARM__ ) */

CY_MISRA_DEVIATE_BLOCK_START('MISRA C-2012 Rule 8.6', 3, \
'Coverity does not check the .S assembly files, the definition is a part of syslib assembly source file.');

/**
* \addtogroup group_syslib_macros
* \{
*/

/******************************************************************************
* Macros
*****************************************************************************/

#define CY_CPU_CORTEX_M0P   (__CORTEX_M == 0)    /**< CM0+ core CPU Code */
#define CY_CPU_CORTEX_M4    (__CORTEX_M == 4)    /**< CM4  core CPU Code */

/** The macro to disable the Fault Handler */
#define CY_ARM_FAULT_DEBUG_DISABLED    (0U)
/** The macro to enable the Fault Handler */
#define CY_ARM_FAULT_DEBUG_ENABLED     (1U)

#if !defined(CY_ARM_FAULT_DEBUG)
    /** The macro defines if the Fault Handler is enabled. Enabled by default. */
    #define CY_ARM_FAULT_DEBUG         (CY_ARM_FAULT_DEBUG_ENABLED)
#endif /* CY_ARM_FAULT_DEBUG */

/**
* \defgroup group_syslib_macros_status_codes Status codes
* \{
* Function status type codes
*/
#define CY_PDL_STATUS_CODE_Pos  (CY_RSLT_CODE_POSITION)     /**< The module status code position in the status code */
#define CY_PDL_STATUS_TYPE_Pos  (CY_RSLT_TYPE_POSITION)     /**< The status type position in the status code */
#define CY_PDL_MODULE_ID_Pos    (CY_RSLT_MODULE_POSITION)   /**< The software module ID position in the status code */
#define CY_PDL_STATUS_INFO      ((uint32_t)CY_RSLT_TYPE_INFO << CY_PDL_STATUS_TYPE_Pos)     /**< The information status type */
#define CY_PDL_STATUS_WARNING   ((uint32_t)CY_RSLT_TYPE_WARNING << CY_PDL_STATUS_TYPE_Pos)  /**< The warning status type */
#define CY_PDL_STATUS_ERROR     ((uint32_t)CY_RSLT_TYPE_ERROR << CY_PDL_STATUS_TYPE_Pos)    /**< The error status type */
#define CY_PDL_MODULE_ID_Msk    (CY_RSLT_MODULE_MASK)       /**< The software module ID mask */
/** Get the software PDL module ID */
#define CY_PDL_DRV_ID(id)       ((uint32_t)((uint32_t)((id) & CY_PDL_MODULE_ID_Msk) << CY_PDL_MODULE_ID_Pos))
#define CY_SYSLIB_ID            CY_PDL_DRV_ID(0x11U)     /**< SYSLIB PDL ID */
/** \} group_syslib_macros_status_codes */

/** \} group_syslib_macros */

/**
* \addtogroup group_syslib_enumerated_types
* \{
*/

/** The SysLib status code structure. */
typedef enum
{
    CY_SYSLIB_SUCCESS       = 0x00UL,    /**< The success status code */
    CY_SYSLIB_BAD_PARAM     = CY_SYSLIB_ID | CY_PDL_STATUS_ERROR | 0x01UL,    /**< The bad parameter status code */
    CY_SYSLIB_TIMEOUT       = CY_SYSLIB_ID | CY_PDL_STATUS_ERROR | 0x02UL,    /**< The time out status code */
    CY_SYSLIB_INVALID_STATE = CY_SYSLIB_ID | CY_PDL_STATUS_ERROR | 0x03UL,    /**< The invalid state status code */
    CY_SYSLIB_UNKNOWN       = CY_SYSLIB_ID | CY_PDL_STATUS_ERROR | 0xFFUL     /**< Unknown status code */
} cy_en_syslib_status_t;

/** \} group_syslib_enumerated_types */
/**
* \addtogroup group_syslib_data_structures
* \{
*/

#if (CY_ARM_FAULT_DEBUG == CY_ARM_FAULT_DEBUG_ENABLED)
    #if (CY_CPU_CORTEX_M4)
        /** Configurable Fault Status Register - CFSR */
        typedef struct
        {
            /** MemManage Fault Status Sub-register - MMFSR */
            uint32_t iaccViol    : 1;  /**< MemManage Fault - The instruction access violation flag */
            uint32_t daccViol    : 1;  /**< MemManage Fault - The data access violation flag */
            uint32_t reserved1   : 1;  /**< Reserved */
            uint32_t mUnstkErr   : 1;  /**< MemManage Fault - Unstacking for a return from exception */
            uint32_t mStkErr     : 1;  /**< MemManage Fault - MemManage fault on stacking for exception entry */
            uint32_t mlspErr     : 1;  /**< MemManage Fault - MemManage fault occurred during floating-point lazy state preservation */
            uint32_t reserved2   : 1;  /**< Reserved */
            uint32_t mmarValid   : 1;  /**< MemManage Fault - The MemManage Address register valid flag */
            /** Bus Fault Status Sub-register - UFSR */
            uint32_t iBusErr     : 1;  /**< Bus Fault - The instruction bus error */
            uint32_t precisErr   : 1;  /**< Bus Fault - The precise Data bus error */
            uint32_t imprecisErr : 1;  /**< Bus Fault - The imprecise data bus error */
            uint32_t unstkErr    : 1;  /**< Bus Fault - Unstacking for an exception return has caused one or more bus faults */
            uint32_t stkErr      : 1;  /**< Bus Fault - Stacking for an exception entry has caused one or more bus faults */
            uint32_t lspErr      : 1;  /**< Bus Fault - A bus fault occurred during the floating-point lazy state */
            uint32_t reserved3   : 1;  /**< Reserved */
            uint32_t bfarValid   : 1;  /**< Bus Fault - The bus fault address register valid flag */
            /** Usage Fault Status Sub-register - UFSR */
            uint32_t undefInstr  : 1;  /**< Usage Fault - An undefined instruction */
            uint32_t invState    : 1;  /**< Usage Fault - The invalid state */
            uint32_t invPC       : 1;  /**< Usage Fault - An invalid PC */
            uint32_t noCP        : 1;  /**< Usage Fault - No coprocessor */
            uint32_t reserved4   : 4;  /**< Reserved */
            uint32_t unaligned   : 1;  /**< Usage Fault - Unaligned access */
            uint32_t divByZero   : 1;  /**< Usage Fault - Divide by zero */
            uint32_t reserved5   : 6;  /**< Reserved */
        } cy_stc_fault_cfsr_t;

        /** Hard Fault Status Register - HFSR */
        typedef struct
        {
            uint32_t reserved1   :  1;   /**< Reserved. */
            uint32_t vectTbl     :  1;   /**< HFSR - Indicates a bus fault on a vector table read during exception processing */
            uint32_t reserved2   : 28;   /**< Reserved. */
            uint32_t forced      :  1;   /**< HFSR - Indicates a forced hard fault */
            uint32_t debugEvt    :  1;   /**< HFSR - Reserved for the debug use.  */
        } cy_stc_fault_hfsr_t;

        /** System Handler Control and State Register - SHCSR */
        typedef struct
        {
            uint32_t memFaultAct    :  1;   /**< SHCSR - The MemManage exception active bit, reads as 1 if the exception is active */
            uint32_t busFaultAct    :  1;   /**< SHCSR - The BusFault exception active bit, reads as 1 if the exception is active */
            uint32_t reserved1      :  1;   /**< Reserved. */
            uint32_t usgFaultAct    :  1;   /**< SHCSR - The UsageFault exception active bit, reads as 1 if the exception is active */
            uint32_t reserved2      :  3;   /**< Reserved. */
            uint32_t svCallAct      :  1;   /**< SHCSR - The SVCall active bit, reads as 1 if the SVC call is active */
            uint32_t monitorAct     :  1;   /**< SHCSR - The debug monitor active bit, reads as 1 if the debug monitor is active */
            uint32_t reserved3      :  1;   /**< Reserved. */
            uint32_t pendSVAct      :  1;   /**< SHCSR - The PendSV exception active bit, reads as 1 if the exception is active */
            uint32_t sysTickAct     :  1;   /**< SHCSR - The SysTick exception active bit, reads as 1 if the exception is active  */
            uint32_t usgFaultPended :  1;   /**< SHCSR - The UsageFault exception pending bit, reads as 1 if the exception is pending */
            uint32_t memFaultPended :  1;   /**< SHCSR - The MemManage exception pending bit, reads as 1 if the exception is pending */
            uint32_t busFaultPended :  1;   /**< SHCSR - The BusFault exception pending bit, reads as 1 if the exception is pending */
            uint32_t svCallPended   :  1;   /**< SHCSR - The SVCall pending bit, reads as 1 if the exception is pending */
            uint32_t memFaultEna    :  1;   /**< SHCSR - The MemManage enable bit, set to 1 to enable */
            uint32_t busFaultEna    :  1;   /**< SHCSR - The BusFault enable bit, set to 1 to enable */
            uint32_t usgFaultEna    :  1;   /**< SHCSR - The UsageFault enable bit, set to 1 to enable */
            uint32_t reserved4      : 13;   /**< Reserved */
        } cy_stc_fault_shcsr_t;
    #endif /* CY_CPU_CORTEX_M4 */

    /** The fault configuration structure. */
    typedef struct
    {
        uint32_t r0;       /**< R0 register content */
        uint32_t r1;       /**< R1 register content */
        uint32_t r2;       /**< R2 register content */
        uint32_t r3;       /**< R3 register content */
        uint32_t r12;      /**< R12 register content */
        uint32_t lr;       /**< LR register content */
        uint32_t pc;       /**< PC register content */
        uint32_t psr;      /**< PSR register content */
        #if (CY_CPU_CORTEX_M4)
            union
            {
                uint32_t cfsrReg;              /**< CFSR register content as a word */
                cy_stc_fault_cfsr_t cfsrBits;  /**< CFSR register content as a structure */
            } cfsr;
            union
            {
                uint32_t hfsrReg;              /**< HFSR register content as a word */
                cy_stc_fault_hfsr_t hfsrBits;  /**< HFSR register content as a structure */
            } hfsr;
            union
            {
                uint32_t shcsrReg;              /**< SHCSR register content as a word */
                cy_stc_fault_shcsr_t shcsrBits; /**< SHCSR register content as a structure */
            } shcsr;
            uint32_t mmfar;                /**< MMFAR register content */
            uint32_t bfar;                 /**< BFAR register content */
            #if ((defined (__FPU_PRESENT) && (__FPU_PRESENT == 1U)) && \
                 (defined (__FPU_USED   ) && (__FPU_USED    == 1U)))
                uint32_t s0;       /**< FPU S0 register content */
                uint32_t s1;       /**< FPU S1 register content */
                uint32_t s2;       /**< FPU S2 register content */
                uint32_t s3;       /**< FPU S3 register content */
                uint32_t s4;       /**< FPU S4 register content */
                uint32_t s5;       /**< FPU S5 register content */
                uint32_t s6;       /**< FPU S6 register content */
                uint32_t s7;       /**< FPU S7 register content */
                uint32_t s8;       /**< FPU S8 register content */
                uint32_t s9;       /**< FPU S9 register content */
                uint32_t s10;      /**< FPU S10 register content */
                uint32_t s11;      /**< FPU S11 register content */
                uint32_t s12;      /**< FPU S12 register content */
                uint32_t s13;      /**< FPU S13 register content */
                uint32_t s14;      /**< FPU S14 register content */
                uint32_t s15;      /**< FPU S15 register content */
                uint32_t fpscr;    /**< FPU FPSCR register content */
            #endif /* __FPU_PRESENT */
        #endif /* CY_CPU_CORTEX_M4 */
    } cy_stc_fault_frame_t;
#endif /* (CY_ARM_FAULT_DEBUG == CY_ARM_FAULT_DEBUG_ENABLED) */

/** \} group_syslib_data_structures */

/**
* \addtogroup group_syslib_macros
* \{
*/

/** The driver major version */
#define CY_SYSLIB_DRV_VERSION_MAJOR    2

/** The driver minor version */
#define CY_SYSLIB_DRV_VERSION_MINOR    70

/** Define start of the function placed to the SRAM area by the linker */
#ifndef CY_SECTION_RAMFUNC_BEGIN
#if defined (__ICCARM__)
#define CY_SECTION_RAMFUNC_BEGIN CY_PRAGMA(diag_suppress = Ta023) __ramfunc
#else
#define CY_SECTION_RAMFUNC_BEGIN CY_SECTION(".cy_ramfunc")
#endif
#endif

/** Define end of the function placed to the SRAM area by the linker */
#ifndef CY_SECTION_RAMFUNC_END
#if defined (__ICCARM__)
#define CY_SECTION_RAMFUNC_END CY_PRAGMA(diag_default = Ta023)
#else
#define CY_SECTION_RAMFUNC_END
#endif
#endif

/** Define variable to be placed to the shared SRAM area by the linker */
#ifndef CY_SECTION_SHAREDMEM
#define CY_SECTION_SHAREDMEM CY_SECTION(".cy_sharedmem")
#endif

typedef void (* cy_israddress)(void);   /**< Type of ISR callbacks */
#if defined (__ICCARM__)
    typedef union { cy_israddress __fun; void * __ptr; } cy_intvec_elem;
#endif  /* defined (__ICCARM__) */

/* MISRA rule 6.3 recommends using specific-length typedef for the basic
 * numerical types of signed and unsigned variants of char, float, and double.
 */
typedef char     char_t;    /**< Specific-length typedef for the basic numerical types of char */
typedef float    float32_t; /**< Specific-length typedef for the basic numerical types of float */
typedef double   float64_t; /**< Specific-length typedef for the basic numerical types of double */

#if !defined(NDEBUG)
    /** The max size of the file name which stores the ASSERT location */
    #define CY_MAX_FILE_NAME_SIZE  (24U)
    extern CY_NOINIT char_t cy_assertFileName[CY_MAX_FILE_NAME_SIZE];  /**< The assert buffer */
    extern CY_NOINIT uint32_t cy_assertLine;                           /**< The assert line value */
#endif /* NDEBUG */

#if (CY_ARM_FAULT_DEBUG == CY_ARM_FAULT_DEBUG_ENABLED)
    #define CY_R0_Pos             (0U)     /**< The position of the R0  content in a fault structure */
    #define CY_R1_Pos             (1U)     /**< The position of the R1  content in a fault structure */
    #define CY_R2_Pos             (2U)     /**< The position of the R2  content in a fault structure */
    #define CY_R3_Pos             (3U)     /**< The position of the R3  content in a fault structure */
    #define CY_R12_Pos            (4U)     /**< The position of the R12 content in a fault structure */
    #define CY_LR_Pos             (5U)     /**< The position of the LR  content in a fault structure */
    #define CY_PC_Pos             (6U)     /**< The position of the PC  content in a fault structure */
    #define CY_PSR_Pos            (7U)     /**< The position of the PSR content in a fault structure */
    #if (CY_CPU_CORTEX_M4) && ((defined (__FPU_PRESENT) && (__FPU_PRESENT == 1U)) && \
                               (defined (__FPU_USED   ) && (__FPU_USED    == 1U)))
        #define CY_FPSCR_IXC_Msk  (0x00000010U)    /**< The cumulative exception bit for floating-point exceptions */
        #define CY_FPSCR_IDC_Msk  (0x00000080U)    /**< The cumulative exception bit for floating-point exceptions */
        #define CY_S0_Pos         (8U)     /**< The position of the FPU S0 content in a fault structure */
        #define CY_S1_Pos         (9U)     /**< The position of the FPU S1 content in a fault structure */
        #define CY_S2_Pos         (10U)    /**< The position of the FPU S2 content in a fault structure */
        #define CY_S3_Pos         (11U)    /**< The position of the FPU S3 content in a fault structure */
        #define CY_S4_Pos         (12U)    /**< The position of the FPU S4 content in a fault structure */
        #define CY_S5_Pos         (13U)    /**< The position of the FPU S5 content in a fault structure */
        #define CY_S6_Pos         (14U)    /**< The position of the FPU S6 content in a fault structure */
        #define CY_S7_Pos         (15U)    /**< The position of the FPU S7 content in a fault structure */
        #define CY_S8_Pos         (16U)    /**< The position of the FPU S8 content in a fault structure */
        #define CY_S9_Pos         (17U)    /**< The position of the FPU S9 content in a fault structure */
        #define CY_S10_Pos        (18U)    /**< The position of the FPU S10 content in a fault structure */
        #define CY_S11_Pos        (19U)    /**< The position of the FPU S11 content in a fault structure */
        #define CY_S12_Pos        (20U)    /**< The position of the FPU S12 content in a fault structure */
        #define CY_S13_Pos        (21U)    /**< The position of the FPU S13 content in a fault structure */
        #define CY_S14_Pos        (22U)    /**< The position of the FPU S14 content in a fault structure */
        #define CY_S15_Pos        (23U)    /**< The position of the FPU S15 content in a fault structure */
        #define CY_FPSCR_Pos      (24U)    /**< The position of the FPU FPSCR content in a fault structure */
    #endif /* CY_CPU_CORTEX_M4 && __FPU_PRESENT */

    extern CY_NOINIT cy_stc_fault_frame_t cy_faultFrame;    /**< Fault frame structure */
#endif /* (CY_ARM_FAULT_DEBUG == CY_ARM_FAULT_DEBUG_ENABLED) */


/**
* \defgroup group_syslib_macros_assert Assert Classes and Levels
* \{
* Defines for the Assert Classes and Levels
*/

/**
* Class 1 - The highest class, safety-critical functions which rely on parameters that could be
* changed between different PSoC devices
*/
#define CY_ASSERT_CLASS_1           (1U)

/** Class 2 - Functions that have fixed limits such as counter periods (16-bits/32-bits etc.) */
#define CY_ASSERT_CLASS_2           (2U)

/** Class 3 - Functions that accept enums as constant parameters */
#define CY_ASSERT_CLASS_3           (3U)

#ifndef CY_ASSERT_LEVEL
    /** The user-definable assert level from compiler command-line argument (similarly to DEBUG / NDEBUG) */
    #define CY_ASSERT_LEVEL         CY_ASSERT_CLASS_3
#endif /* CY_ASSERT_LEVEL */

#if (CY_ASSERT_LEVEL == CY_ASSERT_CLASS_1)
    #define CY_ASSERT_L1(x)         CY_ASSERT(x)        /**< Assert Level 1 */
    #define CY_ASSERT_L2(x)         do{}while(false)    /**< Assert Level 2 */
    #define CY_ASSERT_L3(x)         do{}while(false)    /**< Assert Level 3 */
#elif (CY_ASSERT_LEVEL == CY_ASSERT_CLASS_2)
    #define CY_ASSERT_L1(x)         CY_ASSERT(x)        /**< Assert Level 1 */
    #define CY_ASSERT_L2(x)         CY_ASSERT(x)        /**< Assert Level 2 */
    #define CY_ASSERT_L3(x)         do{}while(false)    /**< Assert Level 3 */
#else /* Default is Level 3 */
    #define CY_ASSERT_L1(x)         CY_ASSERT(x)        /**< Assert Level 1 */
    #define CY_ASSERT_L2(x)         CY_ASSERT(x)        /**< Assert Level 2 */
    #define CY_ASSERT_L3(x)         CY_ASSERT(x)        /**< Assert Level 3 */
#endif /* CY_ASSERT_LEVEL == CY_ASSERT_CLASS_1 */

/** \} group_syslib_macros_assert */


/******************************************************************************
* Constants
*****************************************************************************/
/** Defines a 32-kHz clock delay */
#define CY_DELAY_MS_OVERFLOW            (0x8000U)

/**
* \defgroup group_syslib_macros_reset_cause Reset cause
* \{
* Define RESET_CAUSE mask values
*/
/** A basic WatchDog Timer (WDT) reset has occurred since the last power cycle. */
#define CY_SYSLIB_RESET_HWWDT           (0x0001U)
/** The fault logging system requested a reset from its Active logic. */
#define CY_SYSLIB_RESET_ACT_FAULT       (0x0002U)
/** The fault logging system requested a reset from its Deep-Sleep logic. */
#define CY_SYSLIB_RESET_DPSLP_FAULT     (0x0004U)
/** The CPU requested a system reset through it's SYSRESETREQ. This can be done via a debugger probe or in firmware. */
#define CY_SYSLIB_RESET_SOFT            (0x0010U)
/** The Multi-Counter Watchdog timer #0 reset has occurred since the last power cycle. */
#define CY_SYSLIB_RESET_SWWDT0          (0x0020U)
/** The Multi-Counter Watchdog timer #1 reset has occurred since the last power cycle. */
#define CY_SYSLIB_RESET_SWWDT1          (0x0040U)
/** The Multi-Counter Watchdog timer #2 reset has occurred since the last power cycle. */
#define CY_SYSLIB_RESET_SWWDT2          (0x0080U)
/** The Multi-Counter Watchdog timer #3 reset has occurred since the last power cycle. */
#define CY_SYSLIB_RESET_SWWDT3          (0x0100U)
/** The reset has occurred on a wakeup from Hibernate power mode. */
#define CY_SYSLIB_RESET_HIB_WAKEUP      (0x40000UL)

/** \} group_syslib_macros_reset_cause */

/** Bit[31:24] Opcode = 0x1B (SoftReset)
 *  Bit[7:1]   Type   = 1    (Only CM4 reset)
 */
#define CY_IPC_DATA_FOR_CM4_SOFT_RESET  (0x1B000002UL)

/**
* \defgroup group_syslib_macros_unique_id Unique ID
* \{
* Unique ID fields positions
*/
#define CY_UNIQUE_ID_DIE_YEAR_Pos       (57U)    /**< The position of the DIE_YEAR  field in the silicon Unique ID */
#define CY_UNIQUE_ID_DIE_MINOR_Pos      (56U)    /**< The position of the DIE_MINOR field in the silicon Unique ID */
#define CY_UNIQUE_ID_DIE_SORT_Pos       (48U)    /**< The position of the DIE_SORT  field in the silicon Unique ID */
#define CY_UNIQUE_ID_DIE_Y_Pos          (40U)    /**< The position of the DIE_Y     field in the silicon Unique ID */
#define CY_UNIQUE_ID_DIE_X_Pos          (32U)    /**< The position of the DIE_X     field in the silicon Unique ID */
#define CY_UNIQUE_ID_DIE_WAFER_Pos      (24U)    /**< The position of the DIE_WAFER field in the silicon Unique ID */
#define CY_UNIQUE_ID_DIE_LOT_2_Pos      (16U)    /**< The position of the DIE_LOT_2 field in the silicon Unique ID */
#define CY_UNIQUE_ID_DIE_LOT_1_Pos      (8U)     /**< The position of the DIE_LOT_1 field in the silicon Unique ID */
#define CY_UNIQUE_ID_DIE_LOT_0_Pos      (0U)     /**< The position of the DIE_LOT_0 field in the silicon Unique ID */

/** \} group_syslib_macros_unique_id */

/** \} group_syslib_macros */

/******************************************************************************
* Function prototypes
******************************************************************************/

/**
* \addtogroup group_syslib_functions
* \{
*/

void Cy_SysLib_Delay(uint32_t milliseconds);
void Cy_SysLib_DelayUs(uint16_t microseconds);
/** Delays for the specified number of cycles.
 *  The function is implemented in the assembler for each supported compiler.
 *  \param cycles  The number of cycles to delay.
 */
void Cy_SysLib_DelayCycles(uint32_t cycles);
__NO_RETURN void Cy_SysLib_Halt(uint32_t reason);
void Cy_SysLib_AssertFailed(const char_t * file, uint32_t line);
void Cy_SysLib_ClearFlashCacheAndBuffer(void);
cy_en_syslib_status_t Cy_SysLib_ResetBackupDomain(void);
uint32_t Cy_SysLib_GetResetReason(void);
void Cy_SysLib_ClearResetReason(void);
uint64_t Cy_SysLib_GetUniqueId(void);
#if (CY_CPU_CORTEX_M0P)
    void Cy_SysLib_SoftResetCM4(void);
#endif /* CY_CPU_CORTEX_M0P */
#if (CY_ARM_FAULT_DEBUG == CY_ARM_FAULT_DEBUG_ENABLED) || defined(CY_DOXYGEN)
    void Cy_SysLib_FaultHandler(uint32_t const *faultStackAddr);
    void Cy_SysLib_ProcessingFault(void);
#endif /* (CY_ARM_FAULT_DEBUG == CY_ARM_FAULT_DEBUG_ENABLED) */
void Cy_SysLib_SetWaitStates(bool ulpMode, uint32_t clkHfMHz);


/*******************************************************************************
* Function Name: Cy_SysLib_EnterCriticalSection
****************************************************************************//**
*
*  Cy_SysLib_EnterCriticalSection disables interrupts and returns a value
*  indicating whether the interrupts were previously enabled.
*
*  \return Returns the current interrupt status. Returns 0 if the interrupts
*          were previously enabled or 1 if the interrupts were previously
*          disabled.
*
*  \note Implementation of Cy_SysLib_EnterCriticalSection manipulates the IRQ
*        enable bit with interrupts still enabled.
*
*******************************************************************************/
uint32_t Cy_SysLib_EnterCriticalSection(void);


/*******************************************************************************
* Function Name: Cy_SysLib_ExitCriticalSection
****************************************************************************//**
*
*  Re-enables the interrupts if they were enabled before
*  Cy_SysLib_EnterCriticalSection() was called. The argument should be the value
*  returned from \ref Cy_SysLib_EnterCriticalSection().
*
*  \param savedIntrStatus  Puts the saved interrupts status returned by
*                          the \ref Cy_SysLib_EnterCriticalSection().
*
*******************************************************************************/
void Cy_SysLib_ExitCriticalSection(uint32_t savedIntrStatus);


/** \cond INTERNAL */
#define CY_SYSLIB_DEVICE_REV_0A       (0x21U)  /**< The device TO *A Revision ID */
#define CY_SYSLIB_DEVICE_PSOC6ABLE2   (0x100U) /**< The PSoC6 BLE2 device Family ID */


/*******************************************************************************
* Function Name: Cy_SysLib_GetDeviceRevision
****************************************************************************//**
*
* This function returns a device Revision ID.
*
* \return  A device Revision ID.
*
*******************************************************************************/
__STATIC_INLINE uint8_t Cy_SysLib_GetDeviceRevision(void)
{
    return ((SFLASH_SI_REVISION_ID == 0UL) ? CY_SYSLIB_DEVICE_REV_0A : SFLASH_SI_REVISION_ID);
}


/*******************************************************************************
* Function Name: Cy_SysLib_GetDevice
****************************************************************************//**
*
* This function returns a device Family ID.
*
* \return  A device Family ID.
*
*******************************************************************************/
__STATIC_INLINE uint16_t Cy_SysLib_GetDevice(void)
{
    return ((SFLASH_FAMILY_ID == 0UL) ? CY_SYSLIB_DEVICE_PSOC6ABLE2 : SFLASH_FAMILY_ID);
}


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

/** A type of ISR callbacks for backward compatibility with the UDB components. Do not use it in your code. */
typedef void (* cyisraddress)(void);
#if defined (__ICCARM__)
    /** A type of ISR callbacks for backward compatibility with the UDB components. Do not use it in your code. */
    typedef union { cyisraddress __fun; void * __ptr; } intvec_elem;
#endif  /* defined (__ICCARM__) */

/** The backward compatibility define for the CyDelay() API for the UDB components.
*   Do not use it in your code.
*/
#define CyDelay                   Cy_SysLib_Delay
/** The backward compatibility define for the CyDelayUs() API for the UDB components.
*   Do not use it in your code.
*/
#define CyDelayUs                 Cy_SysLib_DelayUs
/** The backward compatibility define for the CyDelayCycles() API for the UDB components.
*   Do not use it in your code.
*/
#define CyDelayCycles             Cy_SysLib_DelayCycles
/** The backward compatibility define for the CyEnterCriticalSection() API for the UDB components.
*   Do not use it in your code.
*/
#define CyEnterCriticalSection()  ((uint8_t) Cy_SysLib_EnterCriticalSection())
/** The backward compatibility define for the CyExitCriticalSection() API for the UDB components.
*   Do not use it in your code.
*/
#define CyExitCriticalSection(x)  (Cy_SysLib_ExitCriticalSection((uint32_t) (x)))
/** \endcond */

/** \} group_syslib_functions */
CY_MISRA_BLOCK_END('MISRA C-2012 Rule 8.6');

#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */

#endif /* CY_SYSLIB_H */

/** \} group_syslib */

/* [] END OF FILE */
