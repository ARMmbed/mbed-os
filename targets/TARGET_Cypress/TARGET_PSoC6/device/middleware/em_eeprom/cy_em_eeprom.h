/*******************************************************************************
* \file cy_em_eeprom.h
* \version 1.0
*
* \brief
*  This file provides the function prototypes and constants for the Emulated
*  EEPROM middleware library.
*
********************************************************************************
* Copyright 2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

/**
 * \mainpage Cypress Em_EEPROM Middleware Library
 *
 * The Emulated EEPROM provides an API that allows creating of an emulated
 * EEPROM in flash that has the ability to do wear leveling and restore
 * corrupted data from a redundant copy. The Emulated EEPROM library is designed
 * to be used with Em_EEPROM component.
 *
 * The Cy_Em_EEPROM API is described in the following sections:
 * - \ref group_em_eeprom_macros
 * - \ref group_em_eeprom_data_structures
 * - \ref group_em_eeprom_enums
 * - \ref group_em_eeprom_functions
 *
 * <b>Features:</b>
 * * EEPROM-Like Non-Volatile Storage
 * * Easy to use read and write API
 * * Optional Wear Leveling
 * * Optional Redundant Data storage
 *
 * \section group_em_eeprom_configuration Configuration Considerations
 *
 * To initialize an Emulated EEPROM on a device different than PSoC 6, just
 * pass the start address of the EEPROM storage to Em_EEPRPOM component start
 * function. E.g.
 *
 *      (void) Em_EEPROM_Init((uint32_t) &emEeprom[0]);
 *
 * The "emEeprom[]" is an array which should be defined in main.c and it should
 * be aligned to the size of the device's flash row. An example of a
 * declaration of such array for MDK and GCC compilers is following:
 *
 *      const uint8_t emEeprom[Em_EEPROM_PHYSICAL_SIZE] __ALIGNED(Em_EEPROM_FLASH_SIZEOF_ROW) = {0u};
 *
 * A similar declaration for the IAR compiler:
 *
 *      #pragma data_alignment = Em_EEPROM_FLASH_SIZEOF_ROW
 *      const uint8_t emEeprom[Em_EEPROM_PHYSICAL_SIZE] = {0u};
 *
 * Note that the user can specify his own name the array.
 *
 * Also note that the Em_EEPROM_PHYSICAL_SIZE is larger than the EEPROM size
 * user entered in the Em_EEPROM component GUI. The Em_EEPROM_PHYSICAL_SIZE
 * also includes the overhead to implement the emulated EEPROM, wear leveling
 * overhead and redundant copy overhead.
 *
 * For PSoC 6 devices the user may not define the array as shown above if one
 * wishes to anticipate less that 32K of Flash. For that purpose just pass
 * zero to the Em_EEPRPOM component start function. The Cy_Em_EEPROM library
 * will automatically reserve the required amount of flash in the region
 * known as work flash. In case if the user needs more than 32 KBytes to
 * implement Emulated EEPROM (Em_EEPROM_PHYSICAL_SIZE > 32K), the user should
 * define an array to reserve the required amount of flash as was described
 * above.
 *
 *
 * \section group_em_eeprom_more_information More Information
 * See the Em_EEPROM Component datasheet.
 *
 *
 * \section group_em_eeprom_MISRA MISRA-C Compliance
 *
 * The Cy_Em_EEPROM library has the following specific deviations:
 *
 * <table class="doxtable">
 *   <tr>
 *     <th>MISRA Rule</th>
 *     <th>Rule Class (Required/Advisory)</th>
 *     <th>Rule Description</th>
 *     <th>Description of Deviation(s)</th>
 *   </tr>
 *   <tr>
 *     <td>11.4</td>
 *     <td>A</td>
 *     <td>The cast should not be performed between a pointer to the object type and
 *         a different pointer to the object type.</td>
 *     <td>The cast from the object type and a different pointer to the object was used
 *         intentionally because of the performance reasons.</td>
 *   </tr>
 *   <tr>
 *     <td>16.7</td>
 *     <td>A</td>
 *     <td>The object addressed by the pointer parameter is not modified and so
 *          the pointer could be of type 'pointer to const'.</td>
 *     <td>The warning is generated because of the pointer dereferencing to
 *         address which makes the MISRA checker think the data is not
 *         modified.</td>
 *   </tr>
 *   <tr>
 *     <td>17.4</td>
 *     <td>R</td>
 *     <td>The array indexing shall be the only allowed form of pointer arithmetic.</td>
 *     <td>The pointer arithmetic used in several places on the Cy_Em_EEPROM
 *         implementation is safe and preferred because it increases the code
 *         flexibility.</td>
 *   </tr>
 *   <tr>
 *     <td>19.7</td>
 *     <td>A</td>
 *     <td>A function shall be used in preference to a function-like macro.</td>
 *     <td>Macro is used because of performance reasons.</td>
 *   </tr>
 * </table>
 *
 * \section group_em_eeprom_changelog Changelog
 * <table class="doxtable">
 *   <tr><th>Version</th><th>Changes</th><th>Reason for Change</th></tr>
 *   <tr>
 *     <td>1.0</td>
 *     <td>Initial Version</td>
 *     <td></td>
 *   </tr>
 * </table>
 *
 * \defgroup group_em_eeprom_macros Emulated EEPROM Macros
 * \brief
 * This section describes Emulated EEPROM Macros.
 *
 * \defgroup group_em_eeprom_data_structures Emulated EEPROM Data Structures
 * \brief
 * Describes data structures defined by Emulated EEPROM.
 *
 * \defgroup group_em_eeprom_enums Emulated EEPROM Return values
 * \brief
 * Describes enumeration types defined by Emulated EEPROM.
 *
 * \defgroup group_em_eeprom_functions Emulated EEPROM Function Prototypes
 * \brief
 * This section describes Emulated EEPROM Function Prototypes.
 *
 */


#if !defined(CY_EM_EEPROM_EM_EEPROM_H)
#define CY_EM_EEPROM_EM_EEPROM_H

#include <stddef.h>
#include <stdbool.h>
#include <cy_device_headers.h>
#include "syslib/cy_syslib.h"
#include "flash/cy_flash.h"

/* The C binding of definitions if building with the C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/***************************************
* Conditional Compilation Parameters
***************************************/
#define CY_PSOC6                                    (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC6)


/***************************************
* Data Structure definitions
***************************************/
/**
* \addtogroup group_em_eeprom_data_structures
* \{
*/

/** EEPROM configuration structure */
typedef struct
{
    /** The number of bytes to store in EEPROM */
    uint32_t eepromSize;

    /** The amount of wear leveling from 1 to 10. 1 means no wear leveling
    * is used.
    */
    uint32_t wearLevelingFactor;

    /** If not zero, a redundant copy of the Em_EEPROM is included. */
    uint8_t redundantCopy;

    /** If not zero, a blocking write to flash is used. Otherwise non-blocking
    * write is used. This parameter is used only for PSoC 6.
    */
    uint8_t blockingWrite;

    /** The start address for the EEPROM memory in the user's flash. */
    uint32_t userFlashStartAddr;
} cy_stc_eeprom_config_t;

/** \} group_em_eeprom_data_structures */

/** The EEPROM context data structure. It is used to store the specific
* EEPROM context data.
*/
typedef struct
{
    /** The pointer to the end address of EEPROM including wear leveling overhead
    * and excluding redundant copy overhead.
    */
    uint32_t wlEndAddr;

    /** The number of flash rows allocated for the EEPROM excluding the number of
    * rows allocated for wear leveling and redundant copy overhead.
    */
    uint32_t numberOfRows;

    /** The address of the last written EEPROM row */
    uint32_t lastWrRowAddr;

    /** The number of bytes to store in EEPROM */
    uint32_t eepromSize;

    /** The amount of wear leveling from 1 to 10. 1 means no wear leveling
    * is used.
    */
    uint32_t wearLevelingFactor;

    /** If not zero, a redundant copy of the Em_EEPROM is included. */
    uint8_t redundantCopy;

    /** If not zero, a blocking write to flash is used. Otherwise non-blocking
    * write is used. This parameter is used only for PSoC 6.
    */
    uint8_t blockingWrite;

    /** The start address for the EEPROM memory in the user's flash. */
    uint32_t userFlashStartAddr;
} cy_stc_eeprom_context_t;


#define CY_EM_EEPROM_ID                         (CY_PDL_DRV_ID(0x1BuL))  /**< Em_EEPROM PDL ID */
    /**
    * \addtogroup group_em_eeprom_enums
    * \{
    * Specifies return values meaning.
    */
    /** A prefix for EEPROM function error return-values */
#define CY_EM_EEPROM_ID_ERROR                   (uint32_t)(CY_EM_EEPROM_ID | CY_PDL_STATUS_ERROR)



/***************************************
* Enumerated Types and Parameters
***************************************/

/** EEPROM return enumeration type */
typedef enum
{
    CY_EM_EEPROM_SUCCESS      = 0x00uL,                             /**< The function executed successfully */
    CY_EM_EEPROM_BAD_PARAM    = (CY_EM_EEPROM_ID_ERROR + 1uL),      /**< The input parameter is invalid */
    CY_EM_EEPROM_BAD_CHECKSUM = (CY_EM_EEPROM_ID_ERROR + 2uL),      /**< The data in EEPROM is corrupted */
    CY_EM_EEPROM_BAD_DATA     = (CY_EM_EEPROM_ID_ERROR + 3uL),      /**< Failed to place the EEPROM in flash */
    CY_EM_EEPROM_WRITE_FAIL   = (CY_EM_EEPROM_ID_ERROR + 4uL)       /**< Write to EEPROM failed */
} cy_en_em_eeprom_status_t;

/** \} group_em_eeprom_enums */


/***************************************
*        Function Prototypes
***************************************/

/**
* \addtogroup group_em_eeprom_functions
* \{
*/
cy_en_em_eeprom_status_t Cy_Em_EEPROM_Init(
        cy_stc_eeprom_config_t* config, cy_stc_eeprom_context_t * context);
cy_en_em_eeprom_status_t Cy_Em_EEPROM_Read(
        uint32_t addr, void * eepromData, uint32_t size, cy_stc_eeprom_context_t * context);
cy_en_em_eeprom_status_t Cy_Em_EEPROM_Write(
        uint32_t addr, void * eepromData, uint32_t size, cy_stc_eeprom_context_t * context);
cy_en_em_eeprom_status_t Cy_Em_EEPROM_Erase(
        cy_stc_eeprom_context_t * context);
uint32_t Cy_Em_EEPROM_NumWrites(cy_stc_eeprom_context_t * context);
/** \} group_em_eeprom_functions */


/***************************************
* API Constants
***************************************/
/**
* \addtogroup group_em_eeprom_macros
* \{
*/
/** Library major version */
#define CY_EM_EEPROM_VERSION_MAJOR                   (1)

/** Library minor version */
#define CY_EM_EEPROM_VERSION_MINOR                   (0)

/** Defines the maximum data length that can be stored in one flash row */
#define CY_EM_EEPROM_EEPROM_DATA_LEN                 (CY_EM_EEPROM_FLASH_SIZEOF_ROW / 2u)

/** \} group_em_eeprom_macros */


/***************************************
* Macro definitions
***************************************/
/** \cond INTERNAL */

/* Defines the size of flash row */
#define CY_EM_EEPROM_FLASH_SIZEOF_ROW               (CY_FLASH_SIZEOF_ROW)

/* Device specific flash constants */
#define CY_EM_EEPROM_FLASH_BASE_ADDR                (CY_FLASH_BASE)
#define CY_EM_EEPROM_FLASH_SIZE                     (CY_FLASH_SIZE)
#define CY_EM_EEPROM_WFLASH_BASE_ADDR               (CY_WFLASH_BASE)
#define CY_EM_EEPROM_WFLASH_SIZE                    (CY_WFLASH_SIZE)
#define CY_EM_EEPROM_WFLASH_END_ADDR                (CY_WFLASH_BASE + CY_WFLASH_SIZE)
    /* Checks is the EEPROM is in flash range */
#define CY_EM_EEPROM_IS_IN_FLASH_RANGE(startAddr, endAddr) \
                    (((((startAddr) > CY_EM_EEPROM_FLASH_BASE_ADDR) && ((endAddr) <= CY_EM_EEPROM_FLASH_END_ADDR)) || \
                      (((startAddr) >= CY_EM_EEPROM_WFLASH_BASE_ADDR) && ((endAddr) <= CY_EM_EEPROM_WFLASH_END_ADDR))))

#define CY_EM_EEPROM_FLASH_END_ADDR                 (CY_EM_EEPROM_FLASH_BASE_ADDR + CY_EM_EEPROM_FLASH_SIZE)

/* Defines the length of EEPROM data that can be stored in Em_EEPROM header */
#define CY_EM_EEPROM_HEADER_DATA_LEN                ((CY_EM_EEPROM_FLASH_SIZEOF_ROW / 2u) - 16u)

#define CY_EM_EEPROM_ADDR_IN_RANGE                  (1u)

/* Return CY_EM_EEPROM_ADDR_IN_RANGE if addr exceeded the upper range of
* EEPROM. The wear leveling overhead is included in the range but redundant copy
* is excluded.
*/
#define CY_EM_EEPROM_IS_ADDR_EXCEED_RANGE(addr, endEepromAddr) \
                                    (((addr) >= (endEepromAddr)) ? (0u) : (CY_EM_EEPROM_ADDR_IN_RANGE))

/* Check to see if the specified address is present in the EEPROM */
#define CY_EM_EEPROM_IS_ADDR_IN_RANGE(addr, startEepromAddr, endEepromAddr) \
                                    (((addr) > (startEepromAddr)) ? \
                                    (((addr) < (endEepromAddr)) ? (CY_EM_EEPROM_ADDR_IN_RANGE) : (0u)) : (0u))

/* Check if the EEPROM address locations from startAddr1 to endAddr1
* are crossed with EEPROM address locations from startAddr2 to endAddr2.
*/
#define CY_EM_EEPROM_IS_ADDRESES_CROSSING(startAddr1, endAddr1 , startAddr2, endAddr2) \
                                    (((startAddr1) > (startAddr2)) ? (((startAddr1) >= (endAddr2)) ? (0u) : (1u) ) : \
                                    (((startAddr2) >= (endAddr1)) ? (0u) : (1u)))

/* Return the pointer to the start of the redundant copy of the EEPROM */
#define CY_EM_EEPROM_GET_REDNT_COPY_ADDR_BASE(numRows, wearLeveling, eepromStartAddr) \
                                    ((((numRows) * CY_EM_EEPROM_FLASH_SIZEOF_ROW) * (wearLeveling)) + (eepromStartAddr))

/* Return the number of the row in EM_EEPROM which contains an address defined by
* rowAddr.
 */
#define CY_EM_EEPROM_GET_ACT_ROW_NUM_FROM_ADDR(rowAddr, maxRows, eepromStartAddr) \
                                    ((((rowAddr) - (eepromStartAddr)) / CY_EM_EEPROM_FLASH_SIZEOF_ROW) % (maxRows))


/** Returns the size allocated for the EEPROM excluding wear leveling and
* redundant copy overhead.
*/
#define CY_EM_EEPROM_GET_EEPROM_SIZE(numRows)       ((numRows) * CY_EM_EEPROM_FLASH_SIZEOF_ROW)

/* Check if the given address belongs to the EEPROM address of the row
* specified by rowNum.
*/
#define CY_EM_EEPROM_IS_ADDR_IN_ROW_RANGE(addr, rowNum) \
                                    (((addr) < ((rowNum) * (CY_EM_EEPROM_FLASH_SIZEOF_ROW / 2u))) ? (0u) : \
                                    (((addr) > ((((rowNum) + 1u) * (CY_EM_EEPROM_FLASH_SIZEOF_ROW / 2u)) - 1u)) ? \
                                    (0u) : (1u)))

/* CRC-8 constants */
#define CY_EM_EEPROM_CRC8_POLYNOM                   ((uint8_t)(0x31u))
#define CY_EM_EEPROM_CRC8_POLYNOM_LEN               (8u)
#define CY_EM_EEPROM_CRC8_SEED                      (0xFFu)
#define CY_EM_EEPROM_CRC8_XOR_VAL                   ((uint8_t) (0x80u))

#define CY_EM_EEPROM_CALCULATE_CRC8(crc)            \
                  ((CY_EM_EEPROM_CRC8_XOR_VAL == ((crc) & CY_EM_EEPROM_CRC8_XOR_VAL)) ? \
                  ((uint8_t)(((uint8_t)((uint8_t)((crc) << 1u))) ^ CY_EM_EEPROM_CRC8_POLYNOM)) : ((uint8_t)((crc) << 1u)))

/** \endcond */

/**
* \addtogroup group_em_eeprom_macros
* \{
*/

/** Calculate the number of flash rows required to create an Em_EEPROM of
* dataSize.
*/
#define CY_EM_EEPROM_GET_NUM_ROWS_IN_EEPROM(dataSize) \
                                    (((dataSize) / (CY_EM_EEPROM_EEPROM_DATA_LEN)) + \
                                    ((((dataSize) % (CY_EM_EEPROM_EEPROM_DATA_LEN)) != 0u) ? 1U : 0U))

/** Returns the size of flash allocated for EEPROM including wear leveling and
* redundant copy overhead.
*/
#define CY_EM_EEPROM_GET_PHYSICAL_SIZE(dataSize, wearLeveling, redundantCopy) \
                                    (((CY_EM_EEPROM_GET_NUM_ROWS_IN_EEPROM(dataSize) * \
                                    CY_EM_EEPROM_FLASH_SIZEOF_ROW) * \
                                    (wearLeveling)) * (1uL + (redundantCopy)))

/** \} group_em_eeprom_macros */


/******************************************************************************
* Local definitions
*******************************************************************************/
/** \cond INTERNAL */

/* Offsets for 32-bit RAM buffer addressing */
#define CY_EM_EEPROM_EEPROM_DATA_OFFSET_U32         ((CY_EM_EEPROM_FLASH_SIZEOF_ROW / 2u) / 4u)
#define CY_EM_EEPROM_HEADER_ADDR_OFFSET_U32         (1u)
#define CY_EM_EEPROM_HEADER_LEN_OFFSET_U32          (2u)
#define CY_EM_EEPROM_HEADER_DATA_OFFSET_U32         (3u)
#define CY_EM_EEPROM_HEADER_CHECKSUM_OFFSET_U32     (CY_EM_EEPROM_EEPROM_DATA_OFFSET_U32 - 1u)

/* The same offsets as above used for direct memory addressing */
#define CY_EM_EEPROM_EEPROM_DATA_OFFSET             (CY_EM_EEPROM_FLASH_SIZEOF_ROW / 2u)
#define CY_EM_EEPROM_HEADER_ADDR_OFFSET             (4u)
#define CY_EM_EEPROM_HEADER_LEN_OFFSET              (8u)
#define CY_EM_EEPROM_HEADER_DATA_OFFSET             (12u)
#define CY_EM_EEPROM_HEADER_CHECKSUM_OFFSET         (CY_EM_EEPROM_EEPROM_DATA_OFFSET - 4u)

#define CY_EM_EEPROM_U32_DIV                        (4u)

/* Maximum wear leveling value */
#define CY_EM_EEPROM_MAX_WEAR_LEVELING_FACTOR       (10u)

/* Maximum allowed flash row write/erase operation duration */
#define CY_EM_EEPROM_MAX_WRITE_DURATION_MS          (50u)

/** \endcond */


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CY_EM_EEPROM_EM_EEPROM_H */


/* [] END OF FILE */

