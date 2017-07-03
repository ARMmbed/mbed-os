/***************************************************************************//**
* \file cy_syslib.h
* \version 1.0
*
* Provides an API declaration of the syslib driver.
*
********************************************************************************
* \copyright
* Copyright 2016, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

/**
* \defgroup group_syslib System Library (SysLib)
* \{
* The system libraries provide APIs that can be called in the user application
* to handle the timing, logical checking or register.
*
* SysLib driver contains a set of different system functions. These functions
* can be called in the application routine. Some features of the system library:
* * Delay functions
* * Register read/write macro
* * Assert and Halt
* * Software reset
* * Reading a reset cause
* * API to invalidate the flash cache and buffer
* * Data manipulation
* * A variable type definition from MISRA-C specifying signedness
* * Cross compiler compatible attributes
* * Get silicon unique ID API
*
* \section group_syslib_configuration Configuration Considerations
* There are no general SysLib configuration concerns.
*
* \section group_syslib_more_information More Information
* Refer to the technical reference manual (TRM).
*
* \section group_syslib_MISRA MISRA Compliance
* The SysLib driver does not have any specific deviations.
*
* \section group_syslib_changelog Changelog
* <table class="doxtable">
*   <tr><th>Version</th><th>Changes</th><th>Reason for Change</th></tr>
*   <tr>
*     <td>1.0</td>
*     <td>Initial version</td>
*     <td></td>
*   </tr>
* </table>
*
* \defgroup group_syslib_macro Macro
* \defgroup group_syslib_functions Functions
* \defgroup group_syslib_data_structures Data Structures
*
*/

#if !defined(_CY_SYSLIB_H_)
#define _CY_SYSLIB_H_

#if !defined(NDEBUG)
    #include <string.h>
#endif /* NDEBUG */
#include <stdint.h>
#include <stdbool.h>
#include "cy_device_headers.h"

#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#if defined( __ICCARM__ )
    /* Suppress the warning for multiple volatile variables in an expression. */
    /* This is common for drivers code and the usage is not order-dependent. */
    #pragma diag_suppress=Pa082
#endif  /* defined( __ICCARM__ ) */


/**
* \addtogroup group_syslib_data_structures
* \{
* <p>The base types. Acceptable types from MISRA-C specifying signedness and size.
* These types must not be used. The default types should be used instead.
* These types are left for backward compatibility only.</p>
*/

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

/* MISRA rule 6.3 recommends use specific-length typedef for the basic
 * numerical types of signed and unsigned variants of char, float, and double.
 */
typedef char     char_t;    /**< Specific-length typedef for the basic numerical types of char */
typedef float    float32_t; /**< Specific-length typedef for the basic numerical types of float */
typedef double   float64_t; /**< Specific-length typedef for the basic numerical types of double */

/*******************************************************************************
*  Hardware Register Types
*******************************************************************************/

typedef volatile uint8_t  reg8;   /**< Alias to uint8_t  for backward compatibility */
typedef volatile uint16_t reg16;  /**< Alias to uint16_t for backward compatibility */
typedef volatile uint32_t reg32;  /**< Alias to uint32_t for backward compatibility */

/*******************************************************************************
*   General cytypes
*******************************************************************************/
/** ARM 32-bit status value */
typedef uint32_t cy_status;

/** \cond INTERNAL */
/** ARM 32-bit status value for backward compatibility with the UDB components. Do not use it in your code. */
typedef uint32_t cystatus;
/** \endcond */

/** \} group_syslib_data_structures */

/**
* \addtogroup group_syslib_macro
* \{
*/

/******************************************************************************
* Macros
*****************************************************************************/

#define CY_CPU_CORTEX_M0P   (__CORTEX_M == 0)    /**< CM0+ core CPU Code */
#define CY_CPU_CORTEX_M4    (__CORTEX_M == 4)    /**< CM4  core CPU Code */

/** Macro to disable the Fault Handler */
#define CY_ARM_FAULT_DEBUG_DISABLED    (0u)
/** Macro to enable the Fault Handler */
#define CY_ARM_FAULT_DEBUG_ENABLED     (1u)

#if !defined(CY_ARM_FAULT_DEBUG)
    /** Macro defines if the Fault Handler is enabled. Enabled by default. */
    #define CY_ARM_FAULT_DEBUG         (CY_ARM_FAULT_DEBUG_ENABLED)
#endif /* CY_ARM_FAULT_DEBUG */

/**
* \defgroup group_syslib_macro_status_codes Status codes
* \{
* Function status type codes
*/
#define CY_PDL_STATUS_CODE_Pos  (0u)        /**< The module status code position in the status code */
#define CY_PDL_STATUS_TYPE_Pos  (16u)       /**< The status type position in the status code */
#define CY_PDL_MODULE_ID_Pos    (18u)       /**< The software module ID position in the status code */
#define CY_PDL_STATUS_INFO      (0UL << CY_PDL_STATUS_TYPE_Pos)    /**< Information status type */
#define CY_PDL_STATUS_WARNING   (1UL << CY_PDL_STATUS_TYPE_Pos)    /**< Warning status type */
#define CY_PDL_STATUS_ERROR     (2UL << CY_PDL_STATUS_TYPE_Pos)    /**< Error status type */
#define CY_PDL_MODULE_ID_Msk    (0x3FFFu)   /**< The software module ID mask */
/* Get the software PDL module ID */
#define CY_PDL_DRV_ID(id)       ((uint32_t)((uint32_t)((id) & CY_PDL_MODULE_ID_Msk) << CY_PDL_MODULE_ID_Pos))
#define CY_SYSLIB_ID            CY_PDL_DRV_ID(0x11u)     /**< SYSLIB PDL ID */
/** \} group_syslib_macro_status_codes */

/** \} group_syslib_macro */

/**
* \addtogroup group_syslib_data_structures
* \{
*/
/** The SysLib status code structure. */
typedef enum
{
    CY_SYSLIB_SUCCESS       = 0x00UL,    /**< Success status code */
    CY_SYSLIB_BAD_PARAM     = CY_SYSLIB_ID | CY_PDL_STATUS_ERROR | 0x01UL,    /**< Bad parameter status code */
    CY_SYSLIB_TIMEOUT       = CY_SYSLIB_ID | CY_PDL_STATUS_ERROR | 0x02UL,    /**< Time out status code */
    CY_SYSLIB_INVALID_STATE = CY_SYSLIB_ID | CY_PDL_STATUS_ERROR | 0x03UL,    /**< Invalid state status code */
    CY_SYSLIB_UNKNOWN       = CY_SYSLIB_ID | CY_PDL_STATUS_ERROR | 0xFFUL     /**< Unknown status code */
} cy_en_syslib_status_t;

#if (CY_ARM_FAULT_DEBUG == CY_ARM_FAULT_DEBUG_ENABLED)
/** The Fault configuration structure. */
typedef struct
{
    uint32_t fault;    /**< Indicates if the fault occurred */
    uint32_t r0;       /**< R0 register content */
    uint32_t r1;       /**< R1 register content */
    uint32_t r2;       /**< R2 register content */
    uint32_t r3;       /**< R3 register content */
    uint32_t r12;      /**< R12 register content */
    uint32_t lr;       /**< LR register content */
    uint32_t pc;       /**< PC register content */
    uint32_t psr;      /**< PSR register content */

    #if (CY_CPU_CORTEX_M4)
        uint32_t cfsr;         /**< CFSR register content */
        uint32_t faultType;    /**< FPU or regular fault type */

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
* \addtogroup group_syslib_macro
* \{
*/

/** Driver major version */
#define CY_SYSLIB_DRV_VERSION_MAJOR    1

/** Driver minor version */
#define CY_SYSLIB_DRV_VERSION_MINOR    0

/** \cond INTERNAL */
/** ARM 32-bit Return error / status codes for backward compatibility.
*  Do not use them in your code.
*/
#define CY_RET_SUCCESS           (0x00u)    /* Successful */
#define CY_RET_BAD_PARAM         (0x01u)    /* One or more invalid parameters */
#define CY_RET_INVALID_OBJECT    (0x02u)    /* Invalid object specified */
#define CY_RET_MEMORY            (0x03u)    /* Memory related failure */
#define CY_RET_LOCKED            (0x04u)    /* Resource lock failure */
#define CY_RET_EMPTY             (0x05u)    /* No more objects available */
#define CY_RET_BAD_DATA          (0x06u)    /* Bad data received (CRC or other error check) */
#define CY_RET_STARTED           (0x07u)    /* Operation started, but not necessarily completed yet */
#define CY_RET_FINISHED          (0x08u)    /* Operation completed */
#define CY_RET_CANCELED          (0x09u)    /* Operation canceled */
#define CY_RET_TIMEOUT           (0x10u)    /* Operation timed out */
#define CY_RET_INVALID_STATE     (0x11u)    /* Operation not setup or is in an improper state */
#define CY_RET_UNKNOWN           ((cy_status) 0xFFFFFFFFu)    /* Unknown failure */

/** ARM 32-bit Return error / status codes for backward compatibility with the UDB components.
*  Do not use them in your code.
*/
#define CYRET_SUCCESS            (0x00u)    /* Successful */
#define CYRET_BAD_PARAM          (0x01u)    /* One or more invalid parameters */
#define CYRET_INVALID_OBJECT     (0x02u)    /* Invalid object specified */
#define CYRET_MEMORY             (0x03u)    /* Memory related failure */
#define CYRET_LOCKED             (0x04u)    /* Resource lock failure */
#define CYRET_EMPTY              (0x05u)    /* No more objects available */
#define CYRET_BAD_DATA           (0x06u)    /* Bad data received (CRC or other error check) */
#define CYRET_STARTED            (0x07u)    /* Operation started, but not necessarily completed yet */
#define CYRET_FINISHED           (0x08u)    /* Operation completed */
#define CYRET_CANCELED           (0x09u)    /* Operation canceled */
#define CYRET_TIMEOUT            (0x10u)    /* Operation timed out */
#define CYRET_INVALID_STATE      (0x11u)    /* Operation not setup or is in an improper state */
#define CYRET_UNKNOWN            ((cystatus) 0xFFFFFFFFu)    /* Unknown failure */
/** \endcond */


/*******************************************************************************
*  Data manipulation defines
*******************************************************************************/

/** Get the lower 8 bits of a 16-bit value. */
#define CY_LO8(x)               ((uint8_t) ((x) & 0xFFu))
/** Get the upper 8 bits of a 16-bit value. */
#define CY_HI8(x)               ((uint8_t) ((uint16_t)(x) >> 8u))

/** Get the lower 16 bits of a 32-bit value. */
#define CY_LO16(x)              ((uint16_t) ((x) & 0xFFFFu))
/** Get the upper 16 bits of a 32-bit value. */
#define CY_HI16(x)              ((uint16_t) ((uint32_t)(x) >> 16u))

/** Swap the byte ordering of a 16-bit value */
#define CY_SWAP_ENDIAN16(x)     ((uint16_t)(((x) << 8u) | (((x) >> 8u) & 0x00FFu)))

/** Swap the byte ordering of a 32-bit value */
#define CY_SWAP_ENDIAN32(x)     ((uint32_t)((((x) >> 24u) & 0x000000FFu) | (((x) & 0x00FF0000u) >> 8u) | \
                                (((x) & 0x0000FF00u) << 8u) | ((x) << 24u)))

/** Swap the byte ordering of a 64-bit value */
#define CY_SWAP_ENDIAN64(x)     ((uint64_t) (((uint64_t) CY_SWAP_ENDIAN32((uint32_t)(x)) << 32u) | \
                                CY_SWAP_ENDIAN32((uint32_t)((x) >> 32u))))


/*******************************************************************************
*   Memory model definitions
*******************************************************************************/
#if defined(__ARMCC_VERSION)

    /** To create cross compiler compatible code, use the CY_NOINIT, CY_SECTION, CY_UNUSED, CY_ALIGN
     * attributes at the first place of declaration/definition.
     * For example: CY_NOINIT uint32_t noinitVar;
     */
    #define CY_NOINIT           __attribute__ ((section(".noinit"), zero_init))
    #define CY_SECTION(name)    __attribute__ ((section(name)))
    #define CY_UNUSED           __attribute__ ((unused))

    /* Specifies the minimum alignment (in bytes) for variables of the specified type. */
    #define CY_ALIGN(align)     __ALIGNED(align)
    #define CY_WEAK             __weak

#elif defined (__GNUC__)

    #define CY_NOINIT           __attribute__ ((section(".noinit")))
    #define CY_SECTION(name)    __attribute__ ((section(name)))
    #define CY_UNUSED           __attribute__ ((unused))
    #define CY_ALIGN(align)     __ALIGNED(align)
    #define CY_WEAK             __attribute__ ((weak))

#elif defined (__ICCARM__)

    #define CY_PRAGMA(x)        _Pragma(#x)
    #define CY_NOINIT           __no_init
    #define CY_SECTION(name)    CY_PRAGMA(section = name)
    #define CY_UNUSED
    #define CY_ALIGN(align)     CY_PRAGMA(data_alignment = align)
    #define CY_WEAK             __weak

#else
    #error "Unsupported toolchain"
#endif  /* (__ARMCC_VERSION) */

typedef void (* cy_israddress)(void);   /**< Type of ISR callbacks */

/** \cond INTERNAL */
/** Type of ISR callbacks for backward compatibility with the UDB components. Do not use it in your code. */
typedef void (* cyisraddress)(void);
/** \endcond */

#if defined (__ICCARM__)
    typedef union { cy_israddress __fun; void * __ptr; } cy_intvec_elem;
    /** \cond INTERNAL */
    /** Type of ISR callbacks for backward compatibility with the UDB components. Do not use it in your code. */
    typedef union { cyisraddress __fun; void * __ptr; } intvec_elem;
    /** \endcond */
#endif  /* defined (__ICCARM__) */

#if !defined(NDEBUG)
    /** Max size of file name which stores the ASSERT location */
    #define CY_MAX_FILE_NAME_SIZE  (24u)
    extern char_t cy_assertFileName[CY_MAX_FILE_NAME_SIZE];  /**< Assert buffer */
    extern uint32_t cy_assertLine;                           /**< Assert line value */
#endif /* NDEBUG */

#if (CY_ARM_FAULT_DEBUG == CY_ARM_FAULT_DEBUG_ENABLED)
    #define CY_R0_Pos              (0u)    /**< Position of R0  content in fault structure */
    #define CY_R1_Pos              (1u)    /**< Position of R1  content in fault structure */
    #define CY_R2_Pos              (2u)    /**< Position of R2  content in fault structure */
    #define CY_R3_Pos              (3u)    /**< Position of R3  content in fault structure */
    #define CY_R12_Pos             (4u)    /**< Position of R12 content in fault structure */
    #define CY_LR_Pos              (5u)    /**< Position of LR  content in fault structure */
    #define CY_PC_Pos              (6u)    /**< Position of PC  content in fault structure */
    #define CY_PSR_Pos             (7u)    /**< Position of PSR content in fault structure */
    #if (CY_CPU_CORTEX_M0P)
        #define CY_CM0P_FAULT_TYPE (0xF0F1F2F3u)       /**< Flag for indication CM0P core fault */
    #elif (CY_CPU_CORTEX_M4)
        #define CY_CM4_FAULT_TYPE     (0xF4F5F6F7u)    /**< Flag for indication CM4 core fault */
        #define CY_NON_FPU_FAULT_TYPE (0xA1A2A3A4u)    /**< Flag for indication CM4 non floating point fault type */
        #if ((defined (__FPU_PRESENT) && (__FPU_PRESENT == 1U)) && \
             (defined (__FPU_USED   ) && (__FPU_USED    == 1U)))
            #define CY_FPU_FAULT_TYPE (0xA8A9AAABu)    /**< Flag for indication CM4 core floating point fault type */
            #define CY_FPSCR_IXC_Msk  (0x00000010u)    /**< Cumulative exception bit for floating-point exceptions */
            #define CY_FPSCR_IDC_Msk  (0x00000080u)    /**< Cumulative exception bit for floating-point exceptions */
            #define CY_S0_Pos         (8u)     /**< Position of FPU S0 content in fault structure */
            #define CY_S1_Pos         (9u)     /**< Position of FPU S1 content in fault structure */
            #define CY_S2_Pos         (10u)    /**< Position of FPU S2 content in fault structure */
            #define CY_S3_Pos         (11u)    /**< Position of FPU S3 content in fault structure */
            #define CY_S4_Pos         (12u)    /**< Position of FPU S4 content in fault structure */
            #define CY_S5_Pos         (13u)    /**< Position of FPU S5 content in fault structure */
            #define CY_S6_Pos         (14u)    /**< Position of FPU S6 content in fault structure */
            #define CY_S7_Pos         (15u)    /**< Position of FPU S7 content in fault structure */
            #define CY_S8_Pos         (16u)    /**< Position of FPU S8 content in fault structure */
            #define CY_S9_Pos         (17u)    /**< Position of FPU S9 content in fault structure */
            #define CY_S10_Pos        (18u)    /**< Position of FPU S10 content in fault structure */
            #define CY_S11_Pos        (19u)    /**< Position of FPU S11 content in fault structure */
            #define CY_S12_Pos        (20u)    /**< Position of FPU S12 content in fault structure */
            #define CY_S13_Pos        (21u)    /**< Position of FPU S13 content in fault structure */
            #define CY_S14_Pos        (22u)    /**< Position of FPU S14 content in fault structure */
            #define CY_S15_Pos        (23u)    /**< Position of FPU S15 content in fault structure */
            #define CY_FPSCR_Pos      (24u)    /**< Position of FPU FPSCR content in fault structure */
        #endif /* __FPU_PRESENT */
    #endif /* CY_CPU_CORTEX_M0P */

    extern cy_stc_fault_frame_t cy_faultFrame;    /**< Fault frame structure */
#endif /* (CY_ARM_FAULT_DEBUG == CY_ARM_FAULT_DEBUG_ENABLED) */


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
* Writes an 16-bit value to the specified address.
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
#define CY_GET_REG24(addr)          (uint32_t) ((*((const volatile uint8_t *)(addr))) | \
                                    (uint32_t) ((*((const volatile uint8_t *)(addr) + 1)) << 8u) | \
                                    (uint32_t) ((*((const volatile uint8_t *)(addr) + 2)) << 16u))


/*******************************************************************************
* Macro Name: CY_SET_REG24(addr, value)
****************************************************************************//**
*
* Writes an 24-bit value to the specified address.
*
* \param addr  The register address.
*
* \param value The value to write.
*
*******************************************************************************/
#define CY_SET_REG24(addr, value)   do  \
                                    {   \
                                        (*((volatile uint8_t *) (addr))) = (uint8_t)(value);                \
                                        (*((volatile uint8_t *) (addr) + 1)) = (uint8_t)((value) >> 8u);    \
                                        (*((volatile uint8_t *) (addr) + 2)) = (uint8_t)((value) >> 16u);   \
                                    }   \
                                    while(0u);


/*******************************************************************************
* Macro Name: CY_GET_REG32(addr)
****************************************************************************//**
*
* Reads the 32-bit value from the specified register. The address is the little
* endian order (LSB in lowest address)
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
* Writes a 32-bit value to the specified register. The address is the little
* endian order (LSB in lowest address)
*
* \param addr  The register address.
*
* \param value The value to write.
*
*******************************************************************************/
#define CY_SET_REG32(addr, value)   (*((volatile uint32_t *)(addr)) = (uint32_t)(value))


/*******************************************************************************
* Macro Name: CY_ASSERT
****************************************************************************//**
*
*  The macro that evaluates the expression and, if it is false (evaluates to 0),
*  the processor is halted. Cy_Halt() is called when the logical expression is
*  false to halt the processor.
*  The ASSERT location of the file name (including path to file) and line number
*  will be stored in a non-zero init area for debugging. They can be accessed
*  by cy_assertFileName and assertLine global variables.
*
* \note This macro is evaluated unless NDEBUG is not defined.
*  If NDEBUG is defined, just empty do while cycle is generated for this macro
*  for the sake of consistency and to avoid MISRA violation.
*  NDEBUG is defined by default for a Release build setting and not defined for
*  a Debug build setting.
*
*  \param x  The logical expression. Asserts if false.
*
*******************************************************************************/
#if !defined(NDEBUG)
    #define CY_ASSERT(x)    do  \
                            {   \
                                if(0u == (uint32_t)(x))                                                 \
                                {                                                                       \
                                    (void) strncpy(cy_assertFileName, __FILE__, CY_MAX_FILE_NAME_SIZE); \
                                    cy_assertLine = (uint32_t)(__LINE__);                               \
                                    Cy_Halt((uint32_t)0u);                                              \
                                }                                                                       \
                            }   \
                            while(0u)
#else
    #define CY_ASSERT(x)    do  \
                            {   \
                            }   \
                            while(0u)
#endif  /* !defined(NDEBUG) */


/*******************************************************************************
* Macro Name: _CLR_SET_FLD32U
****************************************************************************//**
*
*  The macro for setting a register with a name field and value for providing
*  get-clear-modify-write operations.
*
*******************************************************************************/
#define _CLR_SET_FLD32U(reg, field, value) (((reg) & ((uint32_t)(~(field ## _Msk)))) | (_VAL2FLD(field, value)))


/*******************************************************************************
* Macro Name: _BOOL2FLD
****************************************************************************//**
*
*  Returns a field mask if the value is not false.
*  Returns 0, if the value is false.
*
*******************************************************************************/
#define _BOOL2FLD(field, value) ((value != false) ? field ## _Msk : 0UL)


/*******************************************************************************
* Macro Name: _FLD2BOOL
****************************************************************************//**
*
*  Returns true, if the value includes the field mask.
*  Returns false, if the value doesn't include the field mask.
*
*******************************************************************************/
#define _FLD2BOOL(field, value) ((value & field ## _Msk) != 0UL)


/******************************************************************************
* Constants
*****************************************************************************/
/** Defines a 32-kHz clock delay */
#define CY_DELAY_MS_OVERFLOW            (0x8000u)

/**
* \defgroup group_syslib_macro_reset_cause Reset cause
* \{
* Define RESET_CAUSE mask values
*/
/** A basic WatchDog Timer (WDT) reset has occurred since the last power cycle. */
#define CY_SYSLIB_RESET_HWWDT           (0x0001u)
/** The fault logging system requested a reset from its Active logic. */
#define CY_SYSLIB_RESET_ACT_FAULT       (0x0002u)
/** The fault logging system requested a reset from its DeepSleep logic. */
#define CY_SYSLIB_RESET_DPSLP_FAULT     (0x0004u)
/** The clock supervision logic requested a reset due to loss of a watch-crystal clock. */
#define CY_SYSLIB_RESET_CSV_WCO_LOSS    (0x0008u)
/** The CPU requested a system reset through it's SYSRESETREQ. This can be done via a debugger probe or in firmware. */
#define CY_SYSLIB_RESET_SOFT            (0x0010u)
/** The Multi-Counter Watchdog timer #0 reset has occurred since last power cycle. */
#define CY_SYSLIB_RESET_SWWDT0          (0x0020u)
/** The Multi-Counter Watchdog timer #1 reset has occurred since last power cycle. */
#define CY_SYSLIB_RESET_SWWDT1          (0x0040u)
/** The Multi-Counter Watchdog timer #2 reset has occurred since last power cycle. */
#define CY_SYSLIB_RESET_SWWDT2          (0x0080u)
/** The Multi-Counter Watchdog timer #3 reset has occurred since last power cycle. */
#define CY_SYSLIB_RESET_SWWDT3          (0x0100u)
/** The clock supervision logic requested a reset due to loss of a high-frequency clock. */
#define CY_SYSLIB_RESET_HFCLK_LOSS      (0x10000UL)
/** The clock supervision logic requested a reset due to frequency error of a high-frequency clock. */
#define CY_SYSLIB_RESET_HFCLK_ERR       (0x20000UL)

/** \} group_syslib_macro_reset_cause */

/** Bit[31:24] Opcode = 0x1B (SoftReset)
 *  Bit[7:1]   Type   = 1    (Only CM4 reset)
 */
#define CY_IPC_DATA_FOR_CM4_SOFT_RESET  (0x1B000002UL)

/**
* \defgroup group_syslib_macro_unique_id Unique ID
* \{
* Unique ID fields positions
*/
#define CY_UNIQUE_ID_DIE_MINOR_Pos      (56u)    /**< Position of DIE_MINOR field in the silicon Unique ID */
#define CY_UNIQUE_ID_DIE_SORT_Pos       (48u)    /**< Position of DIE_SORT  field in the silicon Unique ID */
#define CY_UNIQUE_ID_DIE_Y_Pos          (40u)    /**< Position of DIE_Y     field in the silicon Unique ID */
#define CY_UNIQUE_ID_DIE_X_Pos          (32u)    /**< Position of DIE_X     field in the silicon Unique ID */
#define CY_UNIQUE_ID_DIE_WAFER_Pos      (24u)    /**< Position of DIE_WAFER field in the silicon Unique ID */
#define CY_UNIQUE_ID_DIE_LOT_2_Pos      (16u)    /**< Position of DIE_LOT_2 field in the silicon Unique ID */
#define CY_UNIQUE_ID_DIE_LOT_1_Pos      (8u)     /**< Position of DIE_LOT_1 field in the silicon Unique ID */
#define CY_UNIQUE_ID_DIE_LOT_0_Pos      (0u)     /**< Position of DIE_LOT_0 field in the silicon Unique ID */

/** \} group_syslib_macro_unique_id */

/** \} group_syslib_macro */

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
 *  The function is implemented in assembler for each supported compiler.
 *  \param cycles  The number of cycles to delay.
 */
void Cy_SysLib_DelayCycles(uint32_t cycles);
__NO_RETURN void Cy_SysLib_Halt(uint32_t reason);
void Cy_SysLib_ClearFlashCacheAndBuffer(void);
uint32_t Cy_SysLib_GetResetReason(void);
uint32_t Cy_SysLib_GetNumHfclkResetCause(void);
void Cy_SysLib_ClearResetReason(void);
uint64_t Cy_SysLib_GetUniqueId(void);
#if (CY_CPU_CORTEX_M0P)
    void Cy_SysLib_SoftResetCM4(void);
#endif /* CY_CPU_CORTEX_M0P */
#if (CY_ARM_FAULT_DEBUG == CY_ARM_FAULT_DEBUG_ENABLED) || defined(CY_DOXYGEN)
    void Cy_SysLib_FaultHandler(uint32_t const *faultStackAddr);
    CY_WEAK void Cy_SysLib_ProcessingFault(void);
#endif /* (CY_ARM_FAULT_DEBUG == CY_ARM_FAULT_DEBUG_ENABLED) */
void Cy_SysLib_SetWaitStates(bool ulpMode, uint32_t clkHfMHz);


/*******************************************************************************
* Function Name: Cy_SysLib_EnterCriticalSection
****************************************************************************//**
*
*  Cy_SysLib_EnterCriticalSection disables interrupts and returns a value
*  indicating whether interrupts were previously enabled.
*
*  \note Implementation of Cy_SysLib_EnterCriticalSection manipulates the IRQ
*  enable bit with interrupts still enabled.
*
*  \return
*    Returns the current interrupt status. Returns 0 if interrupts were
*    previously enabled or 1 if interrupts were previously disabled.
*
*******************************************************************************/
uint32_t Cy_SysLib_EnterCriticalSection(void);


/*******************************************************************************
* Function Name: Cy_SysLib_ExitCriticalSection
****************************************************************************//**
*
*  Re-enables interrupts if they were enabled before
*  Cy_SysLib_EnterCriticalSection() was called. The argument should be the value
*  returned from \ref Cy_SysLib_EnterCriticalSection().
*
*  \param savedIntrStatus
*   Puts the saved interrupts status returned by
*   the \ref Cy_SysLib_EnterCriticalSection().
*
*******************************************************************************/
void Cy_SysLib_ExitCriticalSection(uint32_t savedIntrStatus);

/** \cond INTERNAL */
/** Backward compatibility define for the CyDelay() API for the UDB components.
*   Do not use it in your code.
*/
#define CyDelay                   (Cy_SysLib_Delay)
/** Backward compatibility define for the CyDelayUs() API for the UDB components.
*   Do not use it in your code.
*/
#define CyDelayUs                 (Cy_SysLib_DelayUs)
/** Backward compatibility define for the CyDelayCycles() API for the UDB components.
*   Do not use it in your code.
*/
#define CyDelayCycles             (Cy_SysLib_DelayCycles)
/** Backward compatibility define for the Cy_Halt() API.
*   Do not use it in your code.
*/
#define Cy_Halt                   (Cy_SysLib_Halt)
/** Backward compatibility define for the Cy_ClearFlashCacheAndBuffer() API.
*   Do not use it in your code.
*/
#define Cy_ClearFlashCacheAndBuffer (Cy_SysLib_ClearFlashCacheAndBuffer)
/** Backward compatibility define for the Cy_SYSLIB_GetResetReason() API.
*   Do not use it in your code.
*/
#define Cy_SYSLIB_GetResetReason  (Cy_SysLib_GetResetReason)
/** Backward compatibility define for the Cy_SYSLIB_GetNumHFCLKResetCause() API.
*   Do not use it in your code.
*/
#define Cy_SYSLIB_GetNumHFCLKResetCause (Cy_SysLib_GetNumHfclkResetCause)
/** Backward compatibility define for the Cy_SYSLIB_ClearResetReason() API.
*   Do not use it in your code.
*/
#define Cy_SYSLIB_ClearResetReason (Cy_SysLib_ClearResetReason)
/** Backward compatibility define for the Cy_SYSLIB_GetUniqueId() API.
*   Do not use it in your code.
*/
#define Cy_SYSLIB_GetUniqueId     (Cy_SysLib_GetUniqueId)
/** Backward compatibility define for the Cy_SYSLIB_SoftResetCM4() API.
*   Do not use it in your code.
*/
#define Cy_SYSLIB_SoftResetCM4    (Cy_SysLib_SoftResetCM4)
#if (CY_ARM_FAULT_DEBUG == CY_ARM_FAULT_DEBUG_ENABLED) || defined(CY_DOXYGEN)
    /** Backward compatibility define for the Cy_SYSLIB_FaultHandler() API.
    *   Do not use it in your code.
    */
    #define Cy_SYSLIB_FaultHandler (Cy_SysLib_FaultHandler)
    /** Backward compatibility define for the Cy_SYSLIB_ProcessingFault() API.
    *   Do not use it in your code.
    */
    #define Cy_SYSLIB_ProcessingFault (Cy_SysLib_ProcessingFault)
#endif /* (CY_ARM_FAULT_DEBUG == CY_ARM_FAULT_DEBUG_ENABLED) */
/** Backward compatibility define for the Cy_SYSLIB_SetWaitStates() API.
*   Do not use it in your code.
*/
#define Cy_SYSLIB_SetWaitStates   (Cy_SysLib_SetWaitStates)
/** Backward compatibility define for the CyEnterCriticalSection() API for the UDB components.
*   Do not use it in your code.
*/
#define CyEnterCriticalSection    (Cy_SysLib_EnterCriticalSection)
/** Backward compatibility define for the CyExitCriticalSection() API for the UDB components.
*   Do not use it in your code.
*/
#define CyExitCriticalSection     (Cy_SysLib_ExitCriticalSection)
/** \endcond */

/** \} group_syslib_functions */

#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */

#endif /* _CY_SYSLIB_H_ */

/** \} group_syslib */

/* [] END OF FILE */
