/***************************************************************************//**
* \file cy_flash.h
* \version 3.50
*
* Provides the API declarations of the Flash driver.
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

#if !defined(CY_FLASH_H)
#define CY_FLASH_H

/**
* \addtogroup group_flash
* \{
* Internal flash memory programming
*
* The functions and other declarations used in this driver are in cy_flash.h.
* You can include cy_pdl.h to get access to all functions
* and declarations in the PDL.
*
* Flash memory in PSoC devices provides non-volatile storage for user firmware,
* user configuration data, and bulk data storage.
*
* Flash operations are implemented as system calls. System calls are executed
* out of SROM in the privileged mode of operation. Users have no access to read
* or modify the SROM code. The driver API requests the system call by acquiring
* the Inter-processor communication (IPC) and writing the SROM function opcode
* and parameters to its input registers. As a result, an NMI interrupt is invoked
* and the requested SROM function is executed. The operation status is returned
* to the driver context and a release interrupt is triggered.
*
* Writing to flash can take up to 20 milliseconds. During this time,
* the device should not be reset (including XRES pin, software  reset, and
* watchdog) or unexpected changes may be made to portions of the flash.
* Also, the low-voltage detect circuits should be configured to generate an
* interrupt instead of a reset.
*
* A Read while Write violation occurs when a flash Read operation is initiated
* in the same or neighboring (neighboring restriction is applicable just for the
* CY8C6xx6, CY8C6xx7 devices) flash sector where the flash Write, Erase, or
* Program operation is working. This violation may cause a HardFault exception.
* To avoid the Read while Write violation, carefully split the
* Read and Write operation on flash sectors which are not neighboring,
* considering both cores in the multi-processor device. If the flash is divided
* into four equal sectors, you may edit the linker script to place the code
* into neighboring sectors. For example, use sectors number 0 and 1 for code
* and sectors 2 and 3 for data storage.
*
* \section group_flash_configuration Configuration Considerations
*
* \subsection group_flash_config_intro Introduction:
* The PSoC 6 MCU user-programmable Flash consists of:
* - Application flash memory (from 2 to 8 sectors) - 128KB/256KB each.
* - EE emulation flash memory - 32KB.
*
* Write operation may be done as Blocking or Partially Blocking,
* defined as follows:
*
* \subsection group_flash_config_blocking Blocking:
* In this case, the entire Flash block is not available for the duration of the
* Write (&sim;16ms). Therefore, no Flash accesses (from any Bus Master) can
* occur during that time. CPU execution can be performed from SRAM. All
* pre-fetching must be disabled. Code execution from Flash is blocked for the
* Flash Write duration for both cores.
*
* \subsection group_flash_config_block_const Constraints for Blocking Flash operations:
* -# During write to flash, the device should not be reset (including XRES pin,
* software reset, and watchdog), or unexpected changes may be made to portions
* of the flash.
* -# The low-voltage detect circuits should be configured to generate an
*    interrupt instead of a reset.
* -# Flash rite operation is allowed only in one of the following CM4 states:
*     -# CM4 is Active and initialized:<br>
*        call \ref Cy_SysEnableCM4 "Cy_SysEnableCM4(CY_CORTEX_M4_APPL_ADDR)".
*        <b>Note:</b> If desired user may put CM4 core in Deep Sleep any time
*        after calling Cy_SysEnableCM4().
*     -# CM4 is Off and disabled:<br>
*        call Cy_SysDisableCM4(). <b>Note:</b> In this state Debug mode is not
*        supported.
*     .
* -# Flash Write cannot be performed in Ultra Low Power (core voltage 0.9V) mode.
* -# Interrupts must be enabled on both active cores. Do not enter a critical
*    section during flash operation.
* -# For the CY8C6xx6, CY8C6xx7 devices user must guarantee that system pipe
*    interrupts (IPC interrupts 3 and 4) have the highest priority, or
*    at least that pipe interrupts are not interrupted or in a pending state
*    for more than 700 &micro;s.
* -# User must guarantee that during flash write operation no flash read
*    operations are performed by bus masters other than CM0+ and CM4 (DMA and
*    Crypto).
* -# If you do not use the default startup, perform the following steps
*    before any flash write/erase operations:
* \snippet flash/snippet/main.c Flash Initialization
*
* \subsection group_flash_config_rww Partially Blocking:
* This method has a much shorter time window during which Flash accesses are not
* allowed. Application code execution from Flash is blocked for only a part of
* Flash Write duration, for both cores. Blocking duration depends upon the API
* sequence used.
*
* For API sequence Cy_Flash_StartEraseRow() + Cy_Flash_StartProgram() there are
* four block-out regions during which Read is blocked. See <b>Figure 1</b>.
*
* <center>
* <table class="doxtable">
* <caption>Table 1 - Block-out periods (timing values are valid just for the
* CY8C6xx6, CY8C6xx7 devices) </caption>
*   <tr>
*     <th>Block-out</th>
*     <th>Phase</th>
*     <th>Duration</th>
*   </tr>
*   <tr>
*     <td>A</td>
*     <td>The beginning of the Erase operation</td>
*     <td>2ms + 9500 SlowClk cycles</td>
*   </tr>
*   <tr>
*     <td>B</td>
*     <td>The end of the Erase operation</td>
*     <td>0.13ms + 1000 SlowClk cycles</td>
*   </tr>
*   <tr>
*     <td>C</td>
*     <td>The beginning of the Program operation</td>
*     <td>0.8ms + 6000 SlowClk cycles</td>
*   </tr>
*   <tr>
*     <td>D</td>
*     <td>The end of the Program operation</td>
*     <td>0.13ms + 1000 SlowClk cycles</td>
*   </tr>
* </table>
* </center>
*
* This allows both cores to execute for about 80% of Flash Write
* operation - see <b>Figure 1</b>.
* This capability is important for communication protocols that rely on fast
* response.
*
* \image html flash-rww-diagram.png "Figure 1 - Blocking Intervals in Flash Write operation"
*
* For the Cy_Flash_StartWrite() function, the block-out period is different for
* the two cores. The core that initiates Cy_Flash_StartWrite() is blocked for
* two periods:
* - From start of Erase operation (start of A on Figure 1) till the start of
*   Program operation (end of C on Figure 1).
* - During D period on <b>Figure 1</b>.
*
* The core that performs read/execute is blocked identically to the previous
* scenario - see <b>Figure 1</b>.
*
* This allows the core that initiates Cy_Flash_StartWrite() to execute for about
* 20% of Flash Write operation. The other core executes for about 80% of Flash
* Write operation.
*
* Some constraints must be planned for in the Partially Blocking mode which are
* described in detail below.
*
* \subsection group_flash_config_rww_const Constraints for Partially Blocking Flash operations:
* -# During write to flash, the device should not be reset (including XRES pin,
*    software reset, and watchdog) or unexpected changes may be made to portions
*    of the flash.
* -# The low-voltage detect circuits should be configured to generate an
*    interrupt instead of a reset.
* -# During write to flash, application code should not change the clock
*    settings. Use Cy_Flash_IsOperationComplete() to ensure flash write
*    operation is finished.
* -# During write to flash, application code should not start the clock
*    measurements (should not call Cy_SysClk_StartClkMeasurementCounters()).
* -# Flash write operation is allowed only in one of the following CM4 states:
*     -# CM4 is Active and initialized:<br>
*        call \ref Cy_SysEnableCM4 "Cy_SysEnableCM4(CY_CORTEX_M4_APPL_ADDR)".
*        <b>Note:</b> If desired user may put CM4 core in Deep Sleep any time
*        after calling Cy_SysEnableCM4().
*     -# CM4 is Off and disabled:<br>
*        call Cy_SysDisableCM4(). <b>Note:</b> In this state Debug mode is not
*        supported.
*     .
* -# Use the following rules for split by sectors. (In this context, read means
*    read of any bus master: CM0+, CM4, DMA, Crypto, etc.)
*     -# Do not write to and read/execute from the same flash sector at the same
*        time. This is true for all sectors.
*     -# Writing rules in application flash (this restriction is applicable just
* for CY8C6xx6, CY8C6xx7 devices):
*         -# Any bus master can read/execute from UFLASH S0 and/or S1, during
*            flash write to UFLASH S2 or S3.
*         -# Any bus master can read/execute from UFLASH S2 and/or S3, during
*            flash write to UFLASH S0 or S1.
*
*         <b>Suggestion:</b> in case of bootloading, it is recommended to place
*         code for CM4 in either S0 or S1. CM0+ code resides in S0. Write data
*         to S2 and S3 sections.
*     .
* -# Flash Write cannot be performed in Ultra Low Power mode (core voltage 0.9V).
* -# Interrupts must be enabled on both active cores. Do not enter a critical
*    section during flash operation.
* -# For the CY8C6xx6, CY8C6xx7 devices user must guarantee that system pipe
*    interrupts (IPC interrupts 3 and 4) have the highest priority, or at
*    least that pipe interrupts are not interrupted or in a pending state
*    for more than 700 &micro;s.
* -# If you do not use the default startup, perform the following steps
*    before any flash write/erase operations:
* \snippet flash/snippet/main.c Flash Initialization
*
* \subsection group_flash_config_emeeprom EEPROM section use:
* If you plan to use "cy_em_eeprom" section for different purposes for both of
* device cores or use <b>Em_EEPROM Middleware</b> together with flash driver
* write operations you must modify the linker scripts.<br>
* For more information, refer to the <b>Middleware/Cypress Em_EEPROM Middleware
* Library</b> section of the PDL documentation.
*
* \section group_flash_more_information More Information
*
* See the technical reference manual (TRM) for more information about the Flash
* architecture.
*
* \section group_flash_changelog Changelog
*
* <table class="doxtable">
*   <tr><th>Version</th><th style="width: 52%;">Changes</th><th>Reason for Change</th></tr>
*   <tr>
*     <td rowspan="2">3.50</td>
*     <td>Updated attribute usage for the linker section placement.</td>
*     <td>Enhancement based on usability feedback.</td>
*   </tr>
*   <tr>
*     <td>Fixed/documented MISRA 2012 violations.</td>
*     <td>MISRA 2012 compliance.</td>
*   </tr>
*   <tr>
*     <td rowspan="1">3.40</td>
*     <td>Updated Cy_Flash_OperationStatus() to access protected registers.</td>
*     <td>Added PSoC 64 device support.</td>
*   </tr>
*   <tr>
*     <td rowspan="1">3.30.4</td>
*     <td>Improved documentation.</td>
*     <td>User experience enhancement.</td>
*   </tr>
*   <tr>
*     <td rowspan="1">3.30.3</td>
*     <td>Updated documentation to limit devices with the restrictions. Improved calculation of the CY_FLASH_DELAY_CORRECTIVE macro.</td>
*     <td>User experience enhancement.</td>
*   </tr>
*   <tr>
*     <td rowspan="1">3.30.2</td>
*     <td>Updated documentation to limit devices with the neighboring restriction.</td>
*     <td>User experience enhancement.</td>
*   </tr>
*   <tr>
*     <td rowspan="1">3.30.1</td>
*     <td>Used the CY_RAMFUNC_BEGIN and CY_RAMFUNC_END macros that allocate the function in RAM instead of using the CY_SECTION(".cy_ramfunc") macros.</td>
*     <td>Removed the code duplication.</td>
*   </tr>
*   <tr>
*     <td rowspan="3">3.30</td>
*     <td>Moved ipcWaitMessageStc structure to the RAM section called ".cy_sharedmem".</td>
*     <td>Support Secure Boot devices.</td>
*   </tr>
*   <tr>
*     <td>Renamed Function Cy_Flash_StartErase() to Cy_Flash_StartEraseRow().</td>
*     <td>The driver improvements based on the usability feedback.</td>
*   </tr>
*   <tr>
*     <td>Added new API functions \ref Cy_Flash_EraseSector,
*         \ref Cy_Flash_StartEraseSector, \ref Cy_Flash_EraseSubsector,
*         \ref Cy_Flash_StartEraseSubsector </td>
*     <td>The driver improvements based on the usability feedback.</td>
*   </tr>
*   <tr>
*     <td rowspan="3">3.20</td>
*     <td>Flattened the organization of the driver source code into the single source directory and the single include directory.</td>
*     <td>Driver library directory-structure simplification.</td>
*   </tr>
*   <tr>
*     <td>Added new API function \ref Cy_Flash_InitExt</td>
*     <td>The driver improvements based on the usability feedback</td>
*   </tr>
*   <tr>
*     <td>Added register access layer. Use register access macros instead
*         of direct register access using dereferenced pointers.</td>
*     <td>Makes register access device-independent, so that the PDL does
*         not need to be recompiled for each supported part number.</td>
*   </tr>
*   <tr>
*     <td>3.11</td>
*     <td>Updated driver functionality to correctly use the SysClk measurement
*         counters while partially blocking flash operations</td>
*     <td>Added arbiter mechanism for correct usage of the SysClk measurement
*         counters</td>
*   </tr>
*   <tr>
*     <td>3.10</td>
*     <td>Updated Cy_Flash_SendCmd() code to support single core devices.</td>
*     <td>Support new devices</td>
*   </tr>
*   <tr>
*     <td>3.0</td>
*     <td>New function - Cy_Flash_ProgramRow();<br>
*         Updated Cy_Flash_RowChecksum(): changed input parameter to take the
*         <b>row address</b> (rowAddr) instead of the <b>row number</b>
*         (rowNum);<br>
*         Renamed macro for disabling RWW support in driver to
*         <b>CY_FLASH_RWW_DRV_SUPPORT_DISABLED</b>.<br>
*         Updated \ref group_flash_configuration documentation section with
*         flash usage constraints.</td>
*     <td>Improvements made based on usability feedback to use a common
*         interface</td>
*   </tr>
*   <tr>
*     <td rowspan="3">2.0</td>
*     <td>Added non-blocking erase function - Cy_Flash_StartErase().
*         Removed the clear cache function call.</td>
*     <td>The clear cache operation is removed from the blocking Write/Erase
*         function because in this case it is performed by the hardware.
*         Otherwise it is documented that it is the user's responsibility to
*         clear the cache after executing the non-blocking Write/Erase flash
*         operation.</td>
*   </tr>
*   <tr>
*     <td>Added new Cy_Flash_IsOperationComplete() function to check completeness.
*         Obsoleted Cy_Flash_IsWriteComplete(), Cy_Flash_IsProgramComplete(),
*         and Cy_Flash_IsEraseComplete() functions.<br>
*         Added Cy_Flash_GetExternalStatus() function to get unparsed status where
*         flash driver will be used in security applications with other modules
*         as SecureImage.<br>
*         Added Cy_Flash_Init() function to initialize all needed prerequisites
*         for Erase/Write operations.</td>
*     <td>Updated driver design to improve user experience.</td>
*   </tr>
*   <tr>
*     <td>Updated driver implementation to remove MISRA rules deviations.</td>
*     <td>Driver implementation quality improvement.</td>
*   </tr>
*   <tr>
*     <td>1.0</td>
*     <td>Initial version</td>
*     <td></td>
*   </tr>
* </table>
*
* \defgroup group_flash_macros Macros
* \{
*     \defgroup group_flash_general_macros Flash general parameters
*         Provides general information about flash
*
*     \defgroup group_flash_config_macros  Flash configuration
*         Specifies the parameter values passed to SROM API
* \}
* \defgroup group_flash_functions Functions
* \defgroup group_flash_enumerated_types Enumerated Types
*/

#include "cy_device.h"
#include <cy_device_headers.h>

#include "cy_syslib.h"

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
#define CY_FLASH_DRV_VERSION_MAJOR       3

/** Driver minor version */
#define CY_FLASH_DRV_VERSION_MINOR       50

#define CY_FLASH_ID               (CY_PDL_DRV_ID(0x14UL))                          /**< FLASH PDL ID */

#define CY_FLASH_ID_INFO          (uint32_t)( CY_FLASH_ID | CY_PDL_STATUS_INFO )   /**< Return prefix for FLASH driver function status codes */
#define CY_FLASH_ID_WARNING       (uint32_t)( CY_FLASH_ID | CY_PDL_STATUS_WARNING) /**< Return prefix for FLASH driver function warning return values */
#define CY_FLASH_ID_ERROR         (uint32_t)( CY_FLASH_ID | CY_PDL_STATUS_ERROR)   /**< Return prefix for FLASH driver function error return values */

/** \} group_flash_macros */


/**
* \addtogroup group_flash_general_macros
* \{
*/

/** Flash row size */
#define CY_FLASH_SIZEOF_ROW                (CPUSS_FLASHC_PA_SIZE * 4u)
/** Long words flash row size */
#define CY_FLASH_SIZEOF_ROW_LONG_UNITS     (CY_FLASH_SIZEOF_ROW / sizeof(uint32_t))

/** \} group_flash_general_macros */


/**
* \addtogroup group_flash_enumerated_types
* \{
*/

/** This enum has the return values of the Flash driver */
typedef enum cy_en_flashdrv_status
{
    CY_FLASH_DRV_SUCCESS                  =   0x00UL,  /**< Success */
    CY_FLASH_DRV_INV_PROT                 =   ( CY_FLASH_ID_ERROR + 0x0UL),  /**< Invalid device protection state */
    CY_FLASH_DRV_INVALID_FM_PL            =   ( CY_FLASH_ID_ERROR + 0x1UL),  /**< Invalid flash page latch address */
    CY_FLASH_DRV_INVALID_FLASH_ADDR       =   ( CY_FLASH_ID_ERROR + 0x2UL),  /**< Invalid flash address */
    CY_FLASH_DRV_ROW_PROTECTED            =   ( CY_FLASH_ID_ERROR + 0x3UL),  /**< Row is write protected */
    CY_FLASH_DRV_IPC_BUSY                 =   ( CY_FLASH_ID_ERROR + 0x5UL),  /**< IPC structure is already locked by another process */
    CY_FLASH_DRV_INVALID_INPUT_PARAMETERS =   ( CY_FLASH_ID_ERROR + 0x6UL),  /**< Input parameters passed to Flash API are not valid */
    CY_FLASH_DRV_PL_ROW_COMP_FA           =   ( CY_FLASH_ID_ERROR + 0x22UL), /**< Comparison between Page Latches and FM row failed */
    CY_FLASH_DRV_ERR_UNC                  =   ( CY_FLASH_ID_ERROR + 0xFFUL), /**< Unknown error code. See \ref Cy_Flash_GetExternalStatus() */
    CY_FLASH_DRV_PROGRESS_NO_ERROR        =   ( CY_FLASH_ID_INFO  + 0x0UL),  /**< Command in progress; no error */
    CY_FLASH_DRV_OPERATION_STARTED        =   ( CY_FLASH_ID_INFO  + 0x1UL),  /**< Flash operation is successfully initiated */
    CY_FLASH_DRV_OPCODE_BUSY              =   ( CY_FLASH_ID_INFO  + 0x2UL)   /**< Flash is under operation */
} cy_en_flashdrv_status_t;


#if !defined(CY_FLASH_RWW_DRV_SUPPORT_DISABLED)
    /** Flash notification configuration structure */
    typedef struct
    {
        uint8_t  clientID;      /**< Client ID */
        uint8_t  pktType;       /**< Message Type */
        uint16_t intrRelMask;   /**< Mask */
    } cy_stc_flash_notify_t;
#endif /* !defined(CY_FLASH_RWW_DRV_SUPPORT_DISABLED) */

/** \} group_flash_enumerated_types */

/***************************************
* Function Prototypes
***************************************/

/**
* \addtogroup group_flash_functions
* \{
*/
void Cy_Flash_Init(void);
cy_en_flashdrv_status_t Cy_Flash_EraseRow(uint32_t rowAddr);
cy_en_flashdrv_status_t Cy_Flash_StartEraseRow(uint32_t rowAddr);
cy_en_flashdrv_status_t Cy_Flash_EraseSector(uint32_t sectorAddr);
cy_en_flashdrv_status_t Cy_Flash_StartEraseSector(uint32_t sectorAddr);
cy_en_flashdrv_status_t Cy_Flash_EraseSubsector(uint32_t subSectorAddr);
cy_en_flashdrv_status_t Cy_Flash_StartEraseSubsector(uint32_t subSectorAddr);
cy_en_flashdrv_status_t Cy_Flash_ProgramRow(uint32_t rowAddr, const uint32_t* data);
cy_en_flashdrv_status_t Cy_Flash_WriteRow(uint32_t rowAddr, const uint32_t* data);
cy_en_flashdrv_status_t Cy_Flash_StartWrite(uint32_t rowAddr, const uint32_t* data);
cy_en_flashdrv_status_t Cy_Flash_StartProgram(uint32_t rowAddr, const uint32_t* data);
cy_en_flashdrv_status_t Cy_Flash_IsOperationComplete(void);
cy_en_flashdrv_status_t Cy_Flash_RowChecksum(uint32_t rowAddr, uint32_t* checksumPtr);
cy_en_flashdrv_status_t Cy_Flash_CalculateHash(const uint32_t* data, uint32_t numberOfBytes, uint32_t* hashPtr);
uint32_t Cy_Flash_GetExternalStatus(void);

#if !defined(CY_FLASH_RWW_DRV_SUPPORT_DISABLED)
    void Cy_Flash_InitExt(cy_stc_flash_notify_t *ipcWaitMessageAddr);
#endif /* !defined(CY_FLASH_RWW_DRV_SUPPORT_DISABLED) */

/** \} group_flash_functions */

/** \cond INTERNAL */
#if (CY_CPU_CORTEX_M4)
void Cy_Flash_ResumeIrqHandler(void);
#endif

/*******************************************************************************
Backward compatibility macro. The following code is DEPRECATED and must
not be used in new projects
*******************************************************************************/
#define     Cy_Flash_IsWriteComplete(...)    Cy_Flash_IsOperationComplete()
#define     Cy_Flash_IsProgramComplete(...)  Cy_Flash_IsOperationComplete()
#define     Cy_Flash_IsEraseComplete(...)    Cy_Flash_IsOperationComplete()
#define     CY_FLASH_NUMBER_ROWS             (CY_FLASH_SIZE / CY_FLASH_SIZEOF_ROW)
#define     Cy_Flash_StartErase              Cy_Flash_StartEraseRow

/** \endcond */

#if defined(__cplusplus)
}
#endif


#endif /* #if !defined(CY_FLASH_H) */

/** \} group_flash */


/* [] END OF FILE */
