/***************************************************************************//**
* \file cy_pra.h
* \version 1.0
*
* \brief The header file of the PRA driver.
*
********************************************************************************
* \copyright
* Copyright 2020 Cypress Semiconductor Corporation
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
* \addtogroup group_pra
* \{
* The Protected Register Access (PRA) driver used to provice access to the
* protected registers to a non-secure application on PSoC 64 devices.
*
* The PRA PDL driver is only intended for the PSoC 64 devices as it provides
* access only to the registers that have PSoC 64 access restrictions. Refer to
* device technical reference manual (TRM) for the list of the protected
* registers.
*
* The driver is not expected to be used directly, instead the PDL functions
* affected by PSoC 64 access restrictions are updated to access registers with
* PRA API.
*
* \section group_pra_more_information More Information
* See the device technical reference manual (TRM).
*
* \section group_pra_MISRA MISRA-C Compliance
* The LVD driver has the following specific deviations:
* <table class="doxtable">
*   <tr>
*     <th>MISRA Rule</th>
*     <th>Rule Class (Required/Advisory)</th>
*     <th>Rule Description</th>
*     <th>Description of Deviation(s)</th>
*   </tr>
*   <tr>
*     <td>13.7</td>
*     <td>R</td>
*     <td>Boolean operations whose results are invariant shall not be permitted.</td>
*     <td>False positive. The Cy_PRA_SendCmd() compiled for Cortex-M4 has a shared
*         variable that is modified by Cortex-M0+ application, but the analysis tool
*         is not aware of this fact.</td>
*   </tr>
*   <tr>
*     <td>14.1</td>
*     <td>R</td>
*     <td>There shall be no unreachable code.</td>
*     <td>False positive. The Cy_PRA_SendCmd() compiled for Cortex-M4 has a shared
*         variable that is modified by Cortex-M0+ application and used in a condition
*         statement, but the analysis tool is not aware of this fact.</td>
*   </tr>
*   <tr>
*     <td>14.7</td>
*     <td>R</td>
*     <td>A function shall have a single point of exit at the end of the function.</td>
*     <td>There are few functions with multiple points of exit implemented to
*         simplify functions design.</td>
*   </tr>
*   <tr>
*     <td>19.13</td>
*     <td>A</td>
*     <td>The # and ## operators should not be used.</td>
*     <td>The ## preprocessor operator is used in macros to form the field mask.</td>
*   </tr>
*   <tr>
*     <td>20.3</td>
*     <td>R</td>
*     <td>The validity of values passed to library functions shall be checked.</td>
*     <td>The additional check to eliminate possibility of accessing beyond array in Cy_PRA_ProcessCmd().</td>
*   </tr>
* </table>
*
* \section group_pra_changelog Changelog
* <table class="doxtable">
*   <tr><th>Version</th><th>Changes</th><th>Reason for Change</th></tr>
*   <tr>
*     <td>1.0</td>
*     <td>Initial version</td>
*     <td></td>
*   </tr>
* </table>
*
* \defgroup group_pra_macros                Macros
* \defgroup group_pra_functions             Functions
* \defgroup group_pra_enums                 Enumerated Types
* \defgroup group_pra_data_structures_cfg   Data Structures
*/

#if !defined(CY_PRA_H)
#define CY_PRA_H

#include <stdint.h>
#include <stdbool.h>
#include "cy_systick.h"
#include "cy_ble_clk.h"
#include "cy_device_headers.h"

#if defined (CY_DEVICE_SECURE) || defined (CY_DOXYGEN)

#ifdef __cplusplus
extern "C" {
#endif

/***************************************
*        Constants
***************************************/

/** \cond INTERNAL */

#define CY_PRA_REG_INDEX_COUNT           (16U)

#define CY_PRA_MSG_TYPE_REG32_GET        (1U)
#define CY_PRA_MSG_TYPE_REG32_CLR_SET    (2U)
#define CY_PRA_MSG_TYPE_REG32_SET        (3U)
#define CY_PRA_MSG_TYPE_CM0_WAKEUP       (4U)
#define CY_PRA_MSG_TYPE_SYS_CFG_FUNC     (5U)
#define CY_PRA_MSG_TYPE_SECURE_ONLY      (6U)
#define CY_PRA_MSG_TYPE_FUNC_POLICY      (7U)

/* IPC */
#define CY_PRA_IPC_NOTIFY_INTR          (0x1UL << CY_IPC_INTR_PRA)
#define CY_PRA_IPC_CHAN_INTR            (0x1UL << CY_IPC_CHAN_PRA)
#define CY_PRA_IPC_NONE_INTR            (0UL)

/* Registers Index */
#define CY_PRA_INDX_SRSS_PWR_LVD_CTL            (0U)
#define CY_PRA_INDX_SRSS_SRSS_INTR              (1U)
#define CY_PRA_INDX_SRSS_SRSS_INTR_SET          (2U)
#define CY_PRA_INDX_SRSS_SRSS_INTR_MASK         (3U)
#define CY_PRA_INDX_SRSS_SRSS_INTR_CFG          (4U)
#define CY_PRA_INDX_SRSS_CLK_ROOT_SELECT_1      (5U)
/* Do not change index below as it is used in flash loaders */
#define CY_PRA_INDX_SRSS_CLK_ROOT_SELECT_2      (6U)
#define CY_PRA_INDX_SRSS_CLK_ROOT_SELECT_3      (7U)
#define CY_PRA_INDX_SRSS_CLK_ROOT_SELECT_4      (8U)
#define CY_PRA_INDX_SRSS_CLK_ROOT_SELECT_5      (9U)
#define CY_PRA_INDX_SRSS_CLK_ROOT_SELECT_6      (10U)
#define CY_PRA_INDX_FLASHC_FLASH_CMD            (11U)
#define CY_PRA_INDX_SRSS_PWR_HIBERNATE          (12U)
#define CY_PRA_INDX_SRSS_CLK_MFO_CONFIG         (13U)
#define CY_PRA_INDX_SRSS_CLK_MF_SELECT          (14U)
#define CY_PRA_INDX_FLASHC_FM_CTL_BOOKMARK      (15U)


/* Functions Index */
#define CY_PRA_FUNC_INIT_CYCFG_DEVICE           (0U)

#define CY_PRA_CLK_FUNC_ECO_DISABLE             (8U)
#define CY_PRA_CLK_FUNC_FLL_DISABLE             (9U)
#define CY_PRA_CLK_FUNC_PLL_DISABLE             (10U)
#define CY_PRA_CLK_FUNC_ILO_ENABLE              (11U)
#define CY_PRA_CLK_FUNC_ILO_DISABLE             (12U)
#define CY_PRA_CLK_FUNC_ILO_HIBERNATE_ON        (13U)
#define CY_PRA_CLK_FUNC_PILO_ENABLE             (14U)
#define CY_PRA_CLK_FUNC_PILO_DISABLE            (15U)
#define CY_PRA_CLK_FUNC_PILO_SET_TRIM           (16U)
#define CY_PRA_CLK_FUNC_WCO_ENABLE              (17U)
#define CY_PRA_CLK_FUNC_WCO_DISABLE             (18U)
#define CY_PRA_CLK_FUNC_WCO_BYPASS              (19U)
#define CY_PRA_CLK_FUNC_HF_ENABLE               (20U)
#define CY_PRA_CLK_FUNC_HF_DISABLE              (21U)
#define CY_PRA_CLK_FUNC_HF_SET_SOURCE           (22U)
#define CY_PRA_CLK_FUNC_HF_SET_DIVIDER          (23U)
#define CY_PRA_CLK_FUNC_FAST_SET_DIVIDER        (24U)
#define CY_PRA_CLK_FUNC_PERI_SET_DIVIDER        (25U)
#define CY_PRA_CLK_FUNC_LF_SET_SOURCE           (26U)
#define CY_PRA_CLK_FUNC_TIMER_SET_SOURCE        (27U)
#define CY_PRA_CLK_FUNC_TIMER_SET_DIVIDER       (28U)
#define CY_PRA_CLK_FUNC_TIMER_ENABLE            (29U)
#define CY_PRA_CLK_FUNC_TIMER_DISABLE           (30U)
#define CY_PRA_CLK_FUNC_PUMP_SET_SOURCE         (31U)
#define CY_PRA_CLK_FUNC_PUMP_SET_DIVIDER        (32U)
#define CY_PRA_CLK_FUNC_PUMP_ENABLE             (33U)
#define CY_PRA_CLK_FUNC_PUMP_DISABLE            (34U)
#define CY_PRA_CLK_FUNC_BAK_SET_SOURCE          (35U)
#define CY_PRA_CLK_FUNC_ECO_CONFIGURE           (36U)
#define CY_PRA_CLK_FUNC_ECO_ENABLE              (37U)
#define CY_PRA_CLK_FUNC_PATH_SET_SOURCE         (38U)
#define CY_PRA_CLK_FUNC_FLL_MANCONFIG           (39U)
#define CY_PRA_CLK_FUNC_FLL_ENABLE              (40U)
#define CY_PRA_CLK_FUNC_PLL_MANCONFIG           (41U)
#define CY_PRA_CLK_FUNC_PLL_ENABLE              (42U)
#define CY_PRA_CLK_FUNC_SLOW_SET_DIVIDER        (43U)
#define CY_PRA_CLK_FUNC_DS_BEFORE_TRANSITION    (44U)
#define CY_PRA_CLK_FUNC_DS_AFTER_TRANSITION     (45U)
#define CY_PRA_CLK_FUNC_EXT_CLK_SET_FREQUENCY   (46U)

#define CY_PRA_PM_FUNC_HIBERNATE                (102U)
#define CY_PRA_PM_FUNC_CM4_DP_FLAG_SET          (103U)
#define CY_PRA_PM_FUNC_LDO_SET_VOLTAGE          (104U)
#define CY_PRA_PM_FUNC_BUCK_ENABLE              (105U)
#define CY_PRA_PM_FUNC_SET_MIN_CURRENT          (106U)
#define CY_PRA_PM_FUNC_SET_NORMAL_CURRENT       (107U)
#define CY_PRA_PM_FUNC_BUCK_ENABLE_VOLTAGE2     (108U)
#define CY_PRA_PM_FUNC_BUCK_DISABLE_VOLTAGE2    (109U)
#define CY_PRA_PM_FUNC_BUCK_VOLTAGE2_HW_CTRL    (110U)
#define CY_PRA_PM_FUNC_BUCK_SET_VOLTAGE2        (111U)

/** Driver major version */
#define CY_PRA_DRV_VERSION_MAJOR       1

/** Driver minor version */
#define CY_PRA_DRV_VERSION_MINOR       0

/** Protected Register Access driver ID */
#define CY_PRA_ID                       (CY_PDL_DRV_ID(0x46U))

/** \endcond */

/**
* \addtogroup group_pra_enums
* \{
*/
/** The PRA function return value status definitions. */
typedef enum
{
    CY_PRA_STATUS_SUCCESS                           = 0x0U,                                      /**< Returned successful */
    CY_PRA_STATUS_ACCESS_DENIED                     = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xFFFU,
    CY_PRA_STATUS_INVALID_PARAM                     = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xFFEU,
    CY_PRA_STATUS_ERROR_PROCESSING                  = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xFFDU,
    CY_PRA_STATUS_REQUEST_SENT                      = CY_PRA_ID | CY_PDL_STATUS_INFO  | 0xFFCU,
    CY_PRA_STATUS_ERROR_SYSPM_FAIL                  = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xFFBU,
    CY_PRA_STATUS_ERROR_SYSPM_TIMEOUT               = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xFFAU,
    /* Reserve 0xFFFD - 0xFFF0*/

    CY_PRA_STATUS_INVALID_PARAM_ECO                 = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xFEFU,
    CY_PRA_STATUS_INVALID_PARAM_EXTCLK              = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xFEEU,
    CY_PRA_STATUS_INVALID_PARAM_ALTHF               = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xFEDU,
    CY_PRA_STATUS_INVALID_PARAM_ILO                 = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xFECU,
    CY_PRA_STATUS_INVALID_PARAM_PILO                = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xFEBU,
    CY_PRA_STATUS_INVALID_PARAM_WCO                 = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xFEAU,
    /* Reserve for other source clocks 0xFFE9 - 0xFFE0 */

    CY_PRA_STATUS_INVALID_PARAM_PATHMUX0            = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xFDFU,
    CY_PRA_STATUS_INVALID_PARAM_PATHMUX1            = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xFDEU,
    CY_PRA_STATUS_INVALID_PARAM_PATHMUX2            = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xFDDU,
    CY_PRA_STATUS_INVALID_PARAM_PATHMUX3            = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xFDCU,
    CY_PRA_STATUS_INVALID_PARAM_PATHMUX4            = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xFDBU,
    CY_PRA_STATUS_INVALID_PARAM_PATHMUX5            = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xFDAU,
    /* Reserve for other path-mux 0xFFD9 - 0xFFD0 */

    CY_PRA_STATUS_INVALID_PARAM_FLL0                = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xFCFU,
    /* Reserve for other FLLs 0xFFCE - 0xFFC0 */

    CY_PRA_STATUS_INVALID_PARAM_PLL0                = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xFBFU,
    CY_PRA_STATUS_INVALID_PARAM_PLL1                = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xFBEU,
    CY_PRA_STATUS_INVALID_PARAM_PLL_NUM             = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xFBDU,
    /* Reserve for other PLLs 0xFFBD - 0xFFB0 */

    CY_PRA_STATUS_INVALID_PARAM_CLKLF               = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xFAFU,
    /* Reserve for other clocks 0xFFAE - 0xFFA0 */

    CY_PRA_STATUS_INVALID_PARAM_CLKHF0              = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xF9FU,
    CY_PRA_STATUS_INVALID_PARAM_CLKHF1              = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xF9EU,
    CY_PRA_STATUS_INVALID_PARAM_CLKHF2              = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xF9DU,
    CY_PRA_STATUS_INVALID_PARAM_CLKHF3              = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xF9CU,
    CY_PRA_STATUS_INVALID_PARAM_CLKHF4              = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xF9BU,
    CY_PRA_STATUS_INVALID_PARAM_CLKHF5              = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xF9AU,
    /* Reserve for other HF clocks 0xFF99 - 0xFF90 */

    CY_PRA_STATUS_INVALID_PARAM_CLKPUMP             = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xF8FU,
    CY_PRA_STATUS_INVALID_PARAM_CLKBAK              = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xF8EU,
    CY_PRA_STATUS_INVALID_PARAM_CLKFAST             = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xF8DU,
    CY_PRA_STATUS_INVALID_PARAM_CLKPERI             = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xF8CU,
    CY_PRA_STATUS_INVALID_PARAM_CLKSLOW             = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xF8BU,
    CY_PRA_STATUS_INVALID_PARAM_SYSTICK             = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xF8AU,
    CY_PRA_STATUS_INVALID_PARAM_CLKTIMER            = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xF89U,
    /* Reserve for other HF clocks 0xFF88 - 0xFF80 */

    CY_PRA_STATUS_ERROR_PROCESSING_PWR              = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xF6FU,
    /* Reserve 0xFF6E - 0xFF60*/

    CY_PRA_STATUS_ERROR_PROCESSING_ECO              = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xF5FU,
    CY_PRA_STATUS_ERROR_PROCESSING_EXTCLK           = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xF5EU,
    CY_PRA_STATUS_ERROR_PROCESSING_ALTHF            = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xF5DU,
    CY_PRA_STATUS_ERROR_PROCESSING_ILO              = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xF5CU,
    CY_PRA_STATUS_ERROR_PROCESSING_PILO             = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xF5BU,
    CY_PRA_STATUS_ERROR_PROCESSING_WCO              = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xF5AU,
    CY_PRA_STATUS_ERROR_PROCESSING_ECO_ENABLED      = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xF59U,
    /* Reserve for other source clocks 0xF59 - 0xFF50 */

    CY_PRA_STATUS_ERROR_PROCESSING_PATHMUX0         = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xF4FU,
    CY_PRA_STATUS_ERROR_PROCESSING_PATHMUX1         = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xF4EU,
    CY_PRA_STATUS_ERROR_PROCESSING_PATHMUX2         = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xF4DU,
    CY_PRA_STATUS_ERROR_PROCESSING_PATHMUX3         = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xF4CU,
    CY_PRA_STATUS_ERROR_PROCESSING_PATHMUX4         = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xF4BU,
    CY_PRA_STATUS_ERROR_PROCESSING_PATHMUX5         = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xF4AU,
    /* Reserve for other path-mux 0xFF49 - 0xFF40 */

    CY_PRA_STATUS_ERROR_PROCESSING_FLL0             = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xF3FU,
    CY_PRA_STATUS_ERROR_PROCESSING_FLL0_ENABLED     = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xF3EU,
    /* Reserve for other FLLs 0xFF3E - 0xFF30 */

    CY_PRA_STATUS_ERROR_PROCESSING_PLL0             = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xF2FU,
    CY_PRA_STATUS_ERROR_PROCESSING_PLL1             = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xF2EU,
    CY_PRA_STATUS_ERROR_PROCESSING_PLL_ENABLED      = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xF2DU,
    /* Reserve for other PLLs 0xFF2D - 0xFF20 */

    CY_PRA_STATUS_ERROR_PROCESSING_CLKLF            = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xF1FU,
    /* Reserve for other clocks 0xFF1E - 0xFF10 */

    CY_PRA_STATUS_ERROR_PROCESSING_CLKHF0           = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xF0FU,
    CY_PRA_STATUS_ERROR_PROCESSING_CLKHF1           = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xF0EU,
    CY_PRA_STATUS_ERROR_PROCESSING_CLKHF2           = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xF0DU,
    CY_PRA_STATUS_ERROR_PROCESSING_CLKHF3           = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xF0CU,
    CY_PRA_STATUS_ERROR_PROCESSING_CLKHF4           = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xF0BU,
    CY_PRA_STATUS_ERROR_PROCESSING_CLKHF5           = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xF0AU,

    /* Reserve for other HF clocks 0xFF09 - 0xFF00 */
    CY_PRA_STATUS_ERROR_PROCESSING_CLKPUMP          = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xEFFU,
    CY_PRA_STATUS_ERROR_PROCESSING_CLKBAK           = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xEFEU,
    CY_PRA_STATUS_ERROR_PROCESSING_CLKFAST          = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xEFDU,
    CY_PRA_STATUS_ERROR_PROCESSING_CLKPERI          = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xEFCU,
    CY_PRA_STATUS_ERROR_PROCESSING_CLKSLOW          = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xEFBU,
    CY_PRA_STATUS_ERROR_PROCESSING_SYSTICK          = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xEFAU,
    CY_PRA_STATUS_ERROR_PROCESSING_CLKTIMER         = CY_PRA_ID | CY_PDL_STATUS_ERROR | 0xEF9U,
} cy_en_pra_status_t;
/** \} group_pra_enums */


/*******************************************************************************
 * Data Structures
 ******************************************************************************/

/**
* \addtogroup group_pra_data_structures
* \{
*/

/** PRA register access */
typedef struct
{
    volatile uint32_t * addr;           /**< Register address */
    uint32_t writeMask;                 /**< Write mask. Zero grants access, one - no access */
} cy_stc_pra_reg_policy_t;

/** Message used for communication */
typedef struct
{
    uint16_t praCommand;            /**< Message Type */
    uint16_t praIndex;              /**< Register index */
    cy_en_pra_status_t praStatus;   /**< Status */
    uint32_t praData1;              /**< First data word */
    uint32_t praData2;              /**< Second data word */
} cy_stc_pra_msg_t;
/** \} group_pra_data_structures */

/** \cond INTERNAL */
extern cy_stc_pra_reg_policy_t regIndexToAddr[CY_PRA_REG_INDEX_COUNT];
/** \endcond */


/*******************************************************************************
*        Function Prototypes
*******************************************************************************/

/**
* \addtogroup group_pra_functions
* \{
*/
void Cy_PRA_Init(void);

#if (CY_CPU_CORTEX_M0P) || defined (CY_DOXYGEN)
    void Cy_PRA_CloseSrssMain2(void);
    void Cy_PRA_OpenSrssMain2(void);
#endif /* (CY_CPU_CORTEX_M0P) */

#if (CY_CPU_CORTEX_M4) || defined (CY_DOXYGEN)
    cy_en_pra_status_t Cy_PRA_SendCmd(uint16_t cmd, uint16_t regIndex, uint32_t clearMask, uint32_t setMask);

    /** \} group_pra_functions */


    /**
    * \addtogroup group_pra_macros
    * \{
    */

/*******************************************************************************
* Macro Name: CY_PRA_REG32_CLR_SET
****************************************************************************//**
*
* Provides get-clear-modify-write operations with a name field and value and
* writes a resulting value to the 32-bit register.
*
*******************************************************************************/
    #define CY_PRA_REG32_CLR_SET(regIndex, field, value)  \
        (void)Cy_PRA_SendCmd(CY_PRA_MSG_TYPE_REG32_CLR_SET, \
                              (regIndex), \
                              ((uint32_t)(~(field ## _Msk))), \
                              (_VAL2FLD(field, \
                              (value))))


/*******************************************************************************
* Macro Name: CY_PRA_REG32_SET(regIndex, value)
****************************************************************************//**
*
* Writes the 32-bit value to the specified register.
*
* \param regIndex The register address index.
*
* \param value The value to write.
*
*******************************************************************************/
    #define CY_PRA_REG32_SET(regIndex, value)  \
        (void)Cy_PRA_SendCmd(CY_PRA_MSG_TYPE_REG32_SET, (regIndex), (value), 0UL)


/*******************************************************************************
* Macro Name: CY_PRA_REG32_GET(regIndex)
****************************************************************************//**
*
* Reads the 32-bit value from the specified register.
*
* \param regIndex The register address index.
*
* \return The read value.
*
*******************************************************************************/
    #define CY_PRA_REG32_GET(regIndex)  \
        (uint32_t) Cy_PRA_SendCmd(CY_PRA_MSG_TYPE_REG32_GET, (regIndex), 0UL, 0UL)


/*******************************************************************************
* Macro Name: CY_PRA_CM0_WAKEUP()
****************************************************************************//**
*
* A simple request to wake up Cortex-M0+ core.
*
*******************************************************************************/
    #define CY_PRA_CM0_WAKEUP()  \
        (void)Cy_PRA_SendCmd(CY_PRA_MSG_TYPE_CM0_WAKEUP, (uint16_t) 0U, 0UL, 0UL)


/*******************************************************************************
* Macro Name: CY_PRA_FUNCTION_CALL_RETURN_PARAM(msgType, funcIndex, param)
****************************************************************************//**
*
* Calls the specified function with the provided parameter and return the
* execution status.
*
* \param msgType Function type.
*
* \param funcIndex Function reference.
*
* \param param Pointer to the function parameter.
*
* \return Function execution status.
*
*******************************************************************************/
    #define CY_PRA_FUNCTION_CALL_RETURN_PARAM(msgType, funcIndex, param)  \
        Cy_PRA_SendCmd((msgType), (funcIndex), (uint32_t)(param), 0UL)


/*******************************************************************************
* Macro Name: CY_PRA_FUNCTION_CALL_RETURN_VOID(msgType, funcIndex)
****************************************************************************//**
*
* Calls the specified function without parameter and return void.
*
* \param msgType Function type.
*
* \param funcIndex Function reference.
*
* \return Function execution status.
*
*******************************************************************************/
    #define CY_PRA_FUNCTION_CALL_RETURN_VOID(msgType, funcIndex)  \
        Cy_PRA_SendCmd((msgType), (funcIndex), 0UL, 0UL)


/*******************************************************************************
* Macro Name: CY_PRA_FUNCTION_CALL_VOID_PARAM(msgType, funcIndex, param)
****************************************************************************//**
*
* Calls the specified function with the provided parameter and return void.
*
* \param msgType Function type.
*
* \param funcIndex Function reference.
*
* \param param Pointer to the function parameter.
*
*******************************************************************************/
    #define CY_PRA_FUNCTION_CALL_VOID_PARAM(msgType, funcIndex, param)  \
        (void)Cy_PRA_SendCmd((msgType), (funcIndex), (uint32_t)(param), 0UL)


/*******************************************************************************
* Macro Name: CY_PRA_FUNCTION_CALL_VOID_VOID(msgType, funcIndex)
****************************************************************************//**
*
* Calls the specified function without parameter and return void.
*
* \param msgType Function type.
*
* \param funcIndex Function reference.
*
*******************************************************************************/
    #define CY_PRA_FUNCTION_CALL_VOID_VOID(msgType, funcIndex)  \
        (void)Cy_PRA_SendCmd((msgType), (funcIndex), 0UL, 0UL)

/** \} group_pra_macros */

#endif /* (CY_CPU_CORTEX_M4) */

#ifdef __cplusplus
}
#endif

#endif /* (CY_DEVICE_SECURE) */

#endif /* #if !defined(CY_PRA_H) */

/** \} group_pra */

/* [] END OF FILE */
