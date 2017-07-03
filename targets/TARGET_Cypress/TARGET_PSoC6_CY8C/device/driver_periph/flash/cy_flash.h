/***************************************************************************//**
* \file cy_flash.h
* \version 1.0
*
* Provides the API declarations of the Flash driver.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_FLASH_H)
#define CY_FLASH_H

/**
* \defgroup group_flash Flash System Routine (Flash)
* \{
* Internal flash memory programming
*
* Flash memory in PSoC devices provides non-volatile storage for user firmware,
* user configuration data, and bulk data storage.
*
* Flash operations are implemented as system calls. System calls are executed
* out of SROM in the privileged mode of operation. Users have no access to read
* or modify the SROM code. The driver API requests the system call by acquiring
* the Inter-processor communication (IPC) and writing the SROM function opcode
* and parameters to its input registers. As a result, an NMI interrupt is invoked
* and the requested SROM API is executed. The operation status is returned to the
* driver context and a release interrupt is triggered.
*
* Writing to flash can take up to 20 milliseconds. During this time,
* the device should not be reset (including XRES pin, software  reset, and
* watchdog) or unexpected changes may be made to portions of the flash.
* Also, the low-voltage detect circuits should be configured to generate an
* interrupt instead of a reset.
*
* The Read while Write violation occurs when the flash Read operation is initiated
* in the same flash sector where the flash Write operation is performing. The
* violation leads to the exception generation. To avoid the Read while Write
* violation, the user has to carefully split the Read and Write operation from the
* same flash sector considering both cores in the multi-processor device.
* Use different flash sectors for code and data storage. The flash is divided
* into four equal sectors.
*
* \section group_flash_configuration Configuration Considerations
*
* There are no specific configuration parameters for the flash operations
* outside the driver API input parameters.
*
* \section group_flash_more_information More Information
*
* See the technical reference manual (TRM) for more information about the Flash architecture.
*
* \section group_flash_MISRA MISRA-C Compliance
*
* The Flash driver has the following specific deviations:
*
* <table class="doxtable">
*   <tr>
*     <th>MISRA Rule</th>
*     <th>Rule Class (Required/Advisory)</th>
*     <th>Rule Description</th>
*     <th>Description of Deviation(s)</th>
*   </tr>
*   <tr>
*     <td>19.7</td>
*     <td>A</td>
*     <td>A function shall be used in preference to a function-like macro</td>
*     <td>Macro is used because of performance reasons</td>
*   </tr>
* </table>
*
* \section group_flash_changelog Changelog
*
* <table class="doxtable">
*   <tr><th>Version</th><th>Changes</th><th>Reason for Change</th></tr>
*   <tr>
*     <td>1.0</td>
*     <td>Initial version</td>
*     <td></td>
*   </tr>
* </table>
*
* \defgroup group_flash_macro Macro
* \defgroup group_flash_functions Functions
* \defgroup group_flash_data_structure Data Structures
*/

#include <cy_device_headers.h>
#include <stddef.h>
#include <ipc/cy_ipc_drv.h>

/***************************************
* Macro definitions
***************************************/
/**
* \addtogroup group_flash_macro
* \{
*/

/** Driver major version */
#define CY_FLASH_DRV_VERSION_MAJOR       1

/** Driver minor version */
#define CY_FLASH_DRV_VERSION_MINOR       0

/**
* \defgroup group_flash_returns  Flash return values
* \{
* Specifies return values meaning
*/
#define CY_FLASH_ID                        (CY_PDL_DRV_ID(0x14u))  /**< FLASH PDL ID */

#define CY_FLASH_ID_INFO    (uint32_t)( CY_FLASH_ID | CY_PDL_STATUS_INFO )   /**< Return prefix for FLASH driver function status codes */
#define CY_FLASH_ID_WARNING (uint32_t)( CY_FLASH_ID | CY_PDL_STATUS_WARNING) /**< Return prefix for FLASH driver function warning return values */
#define CY_FLASH_ID_ERROR   (uint32_t)( CY_FLASH_ID | CY_PDL_STATUS_ERROR)   /**< Return prefix for FLASH driver function error return values */

/** This enum has the return values of the Flash driver */
typedef enum
{
    CY_FLASH_DRV_SUCCESS                  =   0x00ul,  /**< Success */
    CY_FLASH_DRV_INV_PROT                 =   ( CY_FLASH_ID_ERROR + 0ul),  /**< Invalid device protection state */
    CY_FLASH_DRV_INVALID_FM_PL            =   ( CY_FLASH_ID_ERROR + 1ul),  /**< Invalid flash page latch address */
    CY_FLASH_DRV_INVALID_FLASH_ADDR       =   ( CY_FLASH_ID_ERROR + 2ul),  /**< Invalid flash address */
    CY_FLASH_DRV_ROW_PROTECTED            =   ( CY_FLASH_ID_ERROR + 3ul),  /**< Row is write protected */
    CY_FLASH_DRV_IPC_BUSY                 =   ( CY_FLASH_ID_ERROR + 5ul),  /**< IPC structure is already locked by another process */
    CY_FLASH_DRV_INVALID_INPUT_PARAMETERS =   ( CY_FLASH_ID_ERROR + 6ul),  /**< Input parameters passed to Flash API are not valid */
    CY_FLASH_DRV_ERR_UNC                  =   ( CY_FLASH_ID_ERROR + 0xFul),/**< Unknown error */
    CY_FLASH_DRV_PROGRESS_NO_ERROR        =   ( CY_FLASH_ID_INFO  + 0ul),  /**< Command in progress; no error */
    CY_FLASH_DRV_OPERATION_STARTED        =   ( CY_FLASH_ID_INFO  + 1ul),  /**< Flash operation is successfully initiated */
    CY_FLASH_DRV_OPCODE_BUSY              =   ( CY_FLASH_ID_INFO  + 2ul),  /**< Flash is under operation */
} cy_en_flashdrv_status_t;

/** \} group_flash_returns */

/** \cond INTERNAL */
/** Command completed with no errors */
#define CY_FLASH_ROMCODE_SUCCESS                   (0xA0000000UL)
/** Invalid device protection state */
#define CY_FLASH_ROMCODE_INVALID_PROTECTION        (0xF0000001UL)
/** Invalid flash page latch address */
#define CY_FLASH_ROMCODE_INVALID_FM_PL             (0xF0000003UL)
/** Invalid flash address */
#define CY_FLASH_ROMCODE_INVALID_FLASH_ADDR        (0xF0000004UL)
/** Row is write protected */
#define CY_FLASH_ROMCODE_ROW_PROTECTED             (0xF0000005UL)
/** Command in progress; no error */
#define CY_FLASH_ROMCODE_IN_PROGRESS_NO_ERROR      (0xA0000009UL)
/** Flash operation is successfully initiated */
#define CY_FLASH_IS_OPERATION_STARTED              (0x00000010UL)
/** Flash is under operation */
#define CY_FLASH_IS_BUSY                           (0x00000040UL)
/** IPC structure is already locked by another process */
#define CY_FLASH_IS_IPC_BUSY                       (0x00000080UL)
/** Input parameters passed to Flash API are not valid */
#define CY_FLASH_IS_INVALID_INPUT_PARAMETERS       (0x00000100UL)

/** Result mask */
#define CY_FLASH_RESULT_MASK               (0xFFFFFFFUL)
/** Error shift */
#define CY_FLASH_ERROR_SHIFT               (28UL)
/** No error */
#define CY_FLASH_ERROR_NO_ERROR            (0xAUL)
/** \endcond */


/**
* \addtogroup group_flash_config_macro Flash configuration
* \{
* Specifies the parameter values passed to SROM API
*/

/** SROM API opcode for flash write operation */
#if (CY_CPU_CORTEX_M0P) && (!defined(CY8C622PSVP) && !defined(CY8C622PSVP_DUAL))
    #define CY_FLASH_OPCODE_WRITE_ROW          (((0x05UL) << 24u) | ((0x01UL) << 8UL))
#else
    #define CY_FLASH_OPCODE_WRITE_ROW          ((0x05UL) << 24u)
#endif

/** SROM API opcode for flash checksum operation */
#define CY_FLASH_OPCODE_CHECKSUM           ((0x0BUL) << 24u)
/** SROM API opcode for flash hash operation */
#define CY_FLASH_OPCODE_HASH               ((0x0DUL) << 24u)
/** SROM API flash row shift for flash checksum operation */
#define CY_FLASH_OPCODE_CHECKSUM_ROW_SHIFT (8u)
/** SROM API flash data size parameter for flash write operation */
#define CY_FLASH_CONFIG_DATASIZE           (0x06UL)
/** SROM API flash verification option for flash write operation */
#define CY_FLASH_CONFIG_VERIFICATION_EN    ((0x01UL) << 16u)

/** \} group_flash_config_macro */


/**
* \addtogroup group_flash_general_macro Flash general parameters
* \{
* Provides general information about flash and IPC
*/

/** Flash row size */
#define CY_FLASH_SIZEOF_ROW                (CPUSS_FLASHC_PA_SIZE * 4u)
/** Number of flash rows */
#define CY_FLASH_NUMBER_ROWS               (CY_FLASH_SIZE / CY_FLASH_SIZEOF_ROW)
/** Long words flash row size */
#define CY_FLASH_SIZEOF_ROW_LONG_UNITS     (CY_FLASH_SIZEOF_ROW / sizeof(uint32_t))
/** Calculates the flash address for a given row of flash */
#define CY_CALCULATE_FLASH_ADDRESS(rowNum) (CY_FLASH_BASE + ((rowNum) * CY_FLASH_SIZEOF_ROW))
/** IPC channel to be used */
#define CY_FLASH_IPC_STRUCT ((IPC_STRUCT_Type*) &IPC->STRUCT[CY_IPC_CHAN_SYSCALL])
/** IPC notify bit for IPC_STRUCT0 (dedicated to flash operation) */
#define CY_FLASH_IPC_NOTIFY_STRUCT0        (0x1UL)
/** \cond INTERNAL */
#define CY_FLASH_CM4_FLASH_PROXY_ADDR      (0x16001101UL)
/** \endcond */

/** \} group_flash_general_macro */

/** \} group_flash_macro */

#if defined(__cplusplus)
extern "C" {
#endif

/***************************************
* Data Structure definitions
***************************************/

/**
* \addtogroup group_flash_data_structure
* \{
*/

/** Flash driver context */
typedef struct
{
    uint32_t opcode;                    /**< Specifies the code of flash operation */
    uint32_t arg1;                      /**< Specifies the configuration of flash operation */
    uint32_t arg2;                      /**< Specifies the configuration of flash operation */
    uint32_t arg3;                      /**< Specifies the configuration of flash operation */
}cy_stc_flash_context_t;

/** \} group_flash_data_structure */


/***************************************
* Function Prototypes
***************************************/

/**
* \addtogroup group_flash_functions
* \{
*/
cy_en_flashdrv_status_t Cy_Flash_WriteRow(uint32_t rowAddr, const uint32_t* data, cy_stc_flash_context_t* contextPtr);
cy_en_flashdrv_status_t Cy_Flash_StartWrite(uint32_t rowAddr, const uint32_t* data, cy_stc_flash_context_t* contextPtr);
cy_en_flashdrv_status_t Cy_Flash_IsWriteComplete(const cy_stc_flash_context_t* contextPtr);
cy_en_flashdrv_status_t Cy_Flash_RowChecksum (uint32_t rowNum, cy_stc_flash_context_t* contextPtr, uint32_t* cheksumPtr);
cy_en_flashdrv_status_t Cy_Flash_CalculateHash (const uint32_t* data, uint32 numberOfBytes, cy_stc_flash_context_t* contextPtr, uint32_t* hashPtr);
/** \cond INTERNAL */
uint32_t Cy_Flash_BoundsCheck(uint32_t flashAddr);
uint32_t Cy_Flash_PrepeareContext(uint32_t rowAddr, const uint32_t* data, cy_stc_flash_context_t *contextPtr);
cy_en_flashdrv_status_t Cy_Flash_ProcessOpcode(uint32_t opcode);

typedef cy_en_flashdrv_status_t (*Cy_Flash_Proxy)(cy_stc_flash_context_t *contextPtr);
/** \endcond */

/** \} group_flash_functions */

#if defined(__cplusplus)
}
#endif


#endif /* #if !defined(CY_FLASH_H) */
/** \endcond */


/** \} group_flash */


/* [] END OF FILE */
