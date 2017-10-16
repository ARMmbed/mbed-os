/***************************************************************************//**
* \file cy_flash.h
* \version 2.0
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
* \warning If the CM0 core triggers the Flash operation, another core must be in
*          Active mode and interrupts must be enabled in both cores.
*
* \note "Warning[Ta023]: Call to a non __ramfunc function." - The warning may
* appear during the build process while using IAR IDE. The reason - some
* functions in RAM memory that use the __ramfunc keyword, may invoke
* functions located in the ROM memory. You can ignore this warning or
* disable it by adding the --diag_suppress=Ta023 option to the compiler.
*
* \section group_flash_configuration Configuration Considerations
*
* Note that to be able to perform FLASH writes, the VCCD should be more than 0.99 V.
* For more information see the technical reference manual (TRM).
*
* \section group_flash_more_information More Information
*
* See the technical reference manual (TRM) for more information about the Flash architecture.
*
* \section group_flash_MISRA MISRA-C Compliance
*
* <table class="doxtable">
*   <tr>
*     <th>MISRA Rule</th>
*     <th>Rule Class (Required/Advisory)</th>
*     <th>Rule Description</th>
*     <th style="width: 50%;">Description of Deviation(s)</th>
*   </tr>
*   <tr>
*     <td>5.6</td>
*     <td>R</td>
*     <td>No identifier in one name space can have the same spelling as an identifier in another name space, with the
*         exception of structure member and union member names.</td>
*     <td>The "context" is used as a structure/union member; they are a label, tag or ordinary
*         identifier.</td>
*   </tr>
*   <tr>
*     <td>8.7</td>
*     <td>R</td>
*     <td>Objects shall be defined at block scope if they are only accessed
*         from within a single function.</td>
*     <td>The driver defines an internal variable that can be accessed by 
*         any core through the IPC hardware.</td>
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
*   <tr>
*     <td>2.0</td>
*     <td>Added non-blocking erase functions ( Cy_Flash_StartErase() and
*         Cy_Flash_IsEraseComplete() ). Removed the clear cache function
*         call.</td>
*     <td>The clear cache operation is removed from the blocking Write/Erase
*         function because in this case it is performed by the hardware.
*         Otherwise it is documented that it is the user's responsibility to
*         clear the cache after executing the non-blocking Write/Erase flash
*         operation.</td>
*   </tr>
* </table>
*
* \defgroup group_flash_macros Macros
* \defgroup group_flash_functions Functions
* \defgroup group_flash_data_structure Data Structures
* \defgroup group_flash_enumerated_types Enumerated Types
*/

#include <cy_device_headers.h>
#include <ipc/cy_ipc_drv.h>
#include <ipc/cy_ipc_sema.h>
#include <ipc/cy_ipc_pipe.h>
#include <sysclk/cy_sysclk.h>

#if defined(__cplusplus)
extern "C" {
#endif

/***************************************
* Macro definitions
***************************************/
/**
* \addtogroup group_flash_macros
* \{
*/

/** Driver major version */
#define CY_FLASH_DRV_VERSION_MAJOR       2

/** Driver minor version */
#define CY_FLASH_DRV_VERSION_MINOR       0

#define CY_FLASH_ID         (CY_PDL_DRV_ID(0x14UL))                          /**< FLASH PDL ID */

#define CY_FLASH_ID_INFO    (uint32_t)( CY_FLASH_ID | CY_PDL_STATUS_INFO )   /**< Return prefix for FLASH driver function status codes */
#define CY_FLASH_ID_WARNING (uint32_t)( CY_FLASH_ID | CY_PDL_STATUS_WARNING) /**< Return prefix for FLASH driver function warning return values */
#define CY_FLASH_ID_ERROR   (uint32_t)( CY_FLASH_ID | CY_PDL_STATUS_ERROR)   /**< Return prefix for FLASH driver function error return values */

/** \cond INTERNAL */
#define CY_FLASH_ENTER_WAIT_LOOP    0xFFU
#define CY_FLASH_IPC_CLIENT_ID      2U
/** Semaphore number reserved for flash driver */
#define CY_FLASH_WAIT_SEMA 0UL
/** \endcond */

/**
* \addtogroup group_flash_config_macros Flash configuration
* \{
* Specifies the parameter values passed to SROM API
*/

/** Set SROM API in blocking mode */
#define CY_FLASH_BLOCKING_MODE             ((0x01UL) << 8UL)
/** Set SROM API in non blocking mode */
#define CY_FLASH_NON_BLOCKING_MODE         (0UL)

/** SROM API opcode for flash write operation */
#define CY_FLASH_OPCODE_WRITE_ROW          ((0x05UL) << 24UL)
/** SROM API opcode for flash program operation */
#define CY_FLASH_OPCODE_PROGRAM_ROW        ((0x06UL) << 24UL)
/** SROM API opcode for row erase operation */
#define CY_FLASH_OPCODE_ERASE_ROW          ((0x1CUL) << 24UL)
/** SROM API opcode for flash checksum operation */
#define CY_FLASH_OPCODE_CHECKSUM           ((0x0BUL) << 24UL)
/** SROM API opcode for flash hash operation */
#define CY_FLASH_OPCODE_HASH               ((0x0DUL) << 24UL)
/** SROM API flash row shift for flash checksum operation */
#define CY_FLASH_OPCODE_CHECKSUM_ROW_SHIFT (8UL)
/** SROM API flash data size parameter for flash write operation */
#define CY_FLASH_CONFIG_DATASIZE           (0x06UL)
/** Data to be programmed to flash is located in SRAM memory region */
#define CY_FLASH_DATA_LOC_SRAM             (0x100UL)
/** SROM API flash verification option for flash write operation */
#define CY_FLASH_CONFIG_VERIFICATION_EN    ((0x01UL) << 16u)

/** \} group_flash_config_macros */


/**
* \addtogroup group_flash_general_macros Flash general parameters
* \{
* Provides general information about flash and IPC
*/

/** Flash row size */
#define CY_FLASH_SIZEOF_ROW                (CPUSS_FLASHC_PA_SIZE * 4u)
/** Number of flash rows */
#define CY_FLASH_NUMBER_ROWS               (CY_FLASH_SIZE / CY_FLASH_SIZEOF_ROW)
/** Long words flash row size */
#define CY_FLASH_SIZEOF_ROW_LONG_UNITS     (CY_FLASH_SIZEOF_ROW / sizeof(uint32_t))
/** IPC channel to be used */
#define CY_FLASH_IPC_STRUCT ((IPC_STRUCT_Type*) &IPC->STRUCT[CY_IPC_CHAN_SYSCALL])
/** IPC notify bit for IPC_STRUCT0 (dedicated to flash operation) */
#define CY_FLASH_IPC_NOTIFY_STRUCT0        (0x1UL << CY_IPC_INTR_SYSCALL1)
/** IPC notify interrupt structure number */
#define CY_FLASH_IPC_INTR_CM0_NOTIFY       (0x1UL << CY_IPC_INTR_FLASH_NOTIFY)

/** \} group_flash_general_macros */

/** \} group_flash_macros */

/**
* \addtogroup group_flash_enumerated_types
* \{
*/

/** This enum has the return values of the Flash driver */
typedef enum
{
    CY_FLASH_DRV_SUCCESS                  =   0x00UL,  /**< Success */
    CY_FLASH_DRV_INV_PROT                 =   ( CY_FLASH_ID_ERROR + 0x0UL),  /**< Invalid device protection state */
    CY_FLASH_DRV_INVALID_FM_PL            =   ( CY_FLASH_ID_ERROR + 0x1UL),  /**< Invalid flash page latch address */
    CY_FLASH_DRV_INVALID_FLASH_ADDR       =   ( CY_FLASH_ID_ERROR + 0x2UL),  /**< Invalid flash address */
    CY_FLASH_DRV_ROW_PROTECTED            =   ( CY_FLASH_ID_ERROR + 0x3UL),  /**< Row is write protected */
    CY_FLASH_DRV_IPC_BUSY                 =   ( CY_FLASH_ID_ERROR + 0x5UL),  /**< IPC structure is already locked by another process */
    CY_FLASH_DRV_INVALID_INPUT_PARAMETERS =   ( CY_FLASH_ID_ERROR + 0x6UL),  /**< Input parameters passed to Flash API are not valid */
    CY_FLASH_DRV_PL_ROW_COMP_FA           =   ( CY_FLASH_ID_ERROR + 0x22UL), /**< Comparison between Page Latches and FM row failed */
    CY_FLASH_DRV_ERR_UNC                  =   ( CY_FLASH_ID_ERROR + 0xFFUL), /**< Unknown error */
    CY_FLASH_DRV_PROGRESS_NO_ERROR        =   ( CY_FLASH_ID_INFO  + 0x0UL),  /**< Command in progress; no error */
    CY_FLASH_DRV_OPERATION_STARTED        =   ( CY_FLASH_ID_INFO  + 0x1UL),  /**< Flash operation is successfully initiated */
    CY_FLASH_DRV_OPCODE_BUSY              =   ( CY_FLASH_ID_INFO  + 0x2UL)   /**< Flash is under operation */
} cy_en_flashdrv_status_t;

/** \} group_flash_enumerated_types */

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
cy_en_flashdrv_status_t Cy_Flash_EraseRow(uint32_t rowAddr);
cy_en_flashdrv_status_t Cy_Flash_WriteRow(uint32_t rowAddr, const uint32_t* data);
cy_en_flashdrv_status_t Cy_Flash_StartWrite(uint32_t rowAddr, const uint32_t* data);
cy_en_flashdrv_status_t Cy_Flash_IsWriteComplete(void);
cy_en_flashdrv_status_t Cy_Flash_StartProgram(uint32_t rowAddr, const uint32_t* data);
cy_en_flashdrv_status_t Cy_Flash_IsProgramComplete(void);
cy_en_flashdrv_status_t Cy_Flash_StartErase(uint32_t rowAddr);
cy_en_flashdrv_status_t Cy_Flash_IsEraseComplete(void);
cy_en_flashdrv_status_t Cy_Flash_RowChecksum(uint32_t rowNum, uint32_t* checksumPtr);
cy_en_flashdrv_status_t Cy_Flash_CalculateHash(const uint32_t* data, uint32_t numberOfBytes, uint32_t* hashPtr);
/** \cond INTERNAL */
typedef cy_en_flashdrv_status_t (*Cy_Flash_Proxy)(cy_stc_flash_context_t *context);

typedef struct _IPC_MSG
{
    uint8_t  clientID;
    uint8_t  pktType;
    uint16_t intrRelMask;
} IPC_MSG;
/** \endcond */

/** \} group_flash_functions */

#if defined(__cplusplus)
}
#endif


#endif /* #if !defined(CY_FLASH_H) */
/** \endcond */


/** \} group_flash */


/* [] END OF FILE */
