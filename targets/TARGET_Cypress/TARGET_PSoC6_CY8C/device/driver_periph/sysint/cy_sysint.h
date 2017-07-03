/***************************************************************************//**
* \file cy_sysint.h
* \version 1.0
*
* \brief
* Provides an API declaration of the sysint driver
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

/**
* \defgroup group_sysint System Interrupt (SysInt)
* \{
* The SysInt driver provides an API to configure the device peripheral interrupts.
* It provides a lightweight interface to complement the CMSIS core NVIC API.
* The provided functions are applicable for all cores in a device and they can
* be used to configure and connect device peripheral interrupts to one or more
* cores.
*
* \n
* <b> Initialization </b>
*
* To configure an interrupt, call Cy_SysInt_Init().
* Populate the configuration structure (cy_stc_sysint_t) and pass it as a parameter
* along with the ISR address. This initializes the interrupt and
* instructs the CPU to jump to the specified ISR vector upon a valid trigger.
*
* Populating the interrupt configuration structure differs slightly for the CM0+ core.
* This core supports only up to 32 peripheral interrupt channels. To allow all device
* interrupts to be routable to the NVIC of this core, there exists a 240:1 multiplexer
* at each of the 32 NVIC channels. Hence the configuration structure (cy_stc_sysint_t)
* must specify the device interrupt source (intSrc) and the CM0+ NVIC mux (intrCm0p).
*
* \n
* <b> Enable </b>
*
* After initializing an interrupt, use the CMSIS Core NVIC_EnableIRQ() function to
* enable it. The parameter used in this function call differs between the CM0+ cores
* and non-CM0+ cores due to the previously mentioned architectural differences.
* Given an initialization structure named config, the function should be called as
* follows:
*
* - Non-CM0+ : NVIC_EnableIRQ(config.intrSrc)
* - CM0+ : NVIC_EnableIRQ(config.intrCm0p)
*
* \section group_sysint_section_configuration_considerations Configuration Considerations
*
* For the Cortex-M0+ core, NVIC mux positions 28, 29, 30, and 31 are reserved for
* internal use by Cypress. These should not be used by the application code.
*
* \section group_sysint_more_information More Information
*
* Refer to the technical reference manual (TRM) and the device datasheet.
*
*
* \section group_sysint_MISRA MISRA-C Compliance
*
* The sysint driver does not have any specific deviations.
*
* \section group_sysint_changelog Changelog
* <table class="doxtable">
*   <tr><th>Version</th><th>Changes</th><th>Reason for Change</th></tr>
*   <tr>
*     <td>1.0</td>
*     <td>Initial version</td>
*     <td></td>
*   </tr>
* </table>
*
* \defgroup group_sysint_macro Macro
* \defgroup group_sysint_globals Global variables
* \defgroup group_sysint_functions Functions
* \defgroup group_sysint_data_structures Data structures
* \defgroup group_sysint_enums Enumerated Types
*/


#if !defined(CY_SYSINT_H)
#define CY_SYSINT_H

#include <stddef.h>
#include "syslib/cy_syslib.h"
#include "cy_device_headers.h"

#if defined(__cplusplus)
extern "C" {
#endif


/***************************************
*       Global Variable
***************************************/

/**
* \addtogroup group_sysint_globals
* \{
*/

extern const cy_israddress __Vectors[]; /**< Vector table in Flash */
extern cy_israddress __ramVectors[]; /**< Relocated vector table in SRAM */

/** \} group_sysint_globals */


/***************************************
*       Global Interrupt
***************************************/

/**
* \addtogroup group_sysint_macro
* \{
*/

/** Driver major version */
#define CY_SYSINT_DRV_VERSION_MAJOR    1

/** Driver minor version */
#define CY_SYSINT_DRV_VERSION_MINOR    0

/** SysInt driver ID */
#define CY_SYSINT_ID CY_PDL_DRV_ID(0x15u)

/** \} group_sysint_macro */


/***************************************
*       Enumeration
***************************************/

/**
* \addtogroup group_sysint_enums
* \{
*/

/**
* SysInt Driver error codes
*/
typedef enum
{
    CY_SYSINT_SUCCESS   = 0x00u,                                      /**< Returned successful */
    CY_SYSINT_BAD_PARAM = CY_SYSINT_ID | CY_PDL_STATUS_ERROR | 0x01u, /**< Bad parameter was passed */
} cy_en_sysint_status_t;

/**
* Enumeration values for Cortex-M0+ NVIC multiplexer selection.
* Deep-sleep wakeup-capability is determined by the CPUSS_CM0_DPSLP_IRQ_NR
* parameter, where the first N number of muxes have the capability to trigger
* deep-sleep interrupts. A deep-sleep capable interrupt source must be
* connected to one of these muxes to be able to trigger in deep-sleep.
*/
typedef enum {
  NvicMux0  = 0,    /**< NVIC Mux for mapping intrSrc to CM0+ NVIC input 0 */
  NvicMux1  = 1,    /**< NVIC Mux for mapping intrSrc to CM0+ NVIC input 1 */
  NvicMux2  = 2,    /**< NVIC Mux for mapping intrSrc to CM0+ NVIC input 2 */
  NvicMux3  = 3,    /**< NVIC Mux for mapping intrSrc to CM0+ NVIC input 3 */
  NvicMux4  = 4,    /**< NVIC Mux for mapping intrSrc to CM0+ NVIC input 4 */
  NvicMux5  = 5,    /**< NVIC Mux for mapping intrSrc to CM0+ NVIC input 5 */
  NvicMux6  = 6,    /**< NVIC Mux for mapping intrSrc to CM0+ NVIC input 6 */
  NvicMux7  = 7,    /**< NVIC Mux for mapping intrSrc to CM0+ NVIC input 7 */
  NvicMux8  = 8,    /**< NVIC Mux for mapping intrSrc to CM0+ NVIC input 8 */
  NvicMux9  = 9,    /**< NVIC Mux for mapping intrSrc to CM0+ NVIC input 9 */
  NvicMux10 = 10,   /**< NVIC Mux for mapping intrSrc to CM0+ NVIC input 10 */
  NvicMux11 = 11,   /**< NVIC Mux for mapping intrSrc to CM0+ NVIC input 11 */
  NvicMux12 = 12,   /**< NVIC Mux for mapping intrSrc to CM0+ NVIC input 12 */
  NvicMux13 = 13,   /**< NVIC Mux for mapping intrSrc to CM0+ NVIC input 13 */
  NvicMux14 = 14,   /**< NVIC Mux for mapping intrSrc to CM0+ NVIC input 14 */
  NvicMux15 = 15,   /**< NVIC Mux for mapping intrSrc to CM0+ NVIC input 15 */
  NvicMux16 = 16,   /**< NVIC Mux for mapping intrSrc to CM0+ NVIC input 16 */
  NvicMux17 = 17,   /**< NVIC Mux for mapping intrSrc to CM0+ NVIC input 17 */
  NvicMux18 = 18,   /**< NVIC Mux for mapping intrSrc to CM0+ NVIC input 18 */
  NvicMux19 = 19,   /**< NVIC Mux for mapping intrSrc to CM0+ NVIC input 19 */
  NvicMux20 = 20,   /**< NVIC Mux for mapping intrSrc to CM0+ NVIC input 20 */
  NvicMux21 = 21,   /**< NVIC Mux for mapping intrSrc to CM0+ NVIC input 21 */
  NvicMux22 = 22,   /**< NVIC Mux for mapping intrSrc to CM0+ NVIC input 22 */
  NvicMux23 = 23,   /**< NVIC Mux for mapping intrSrc to CM0+ NVIC input 23 */
  NvicMux24 = 24,   /**< NVIC Mux for mapping intrSrc to CM0+ NVIC input 24 */
  NvicMux25 = 25,   /**< NVIC Mux for mapping intrSrc to CM0+ NVIC input 25 */
  NvicMux26 = 26,   /**< NVIC Mux for mapping intrSrc to CM0+ NVIC input 26 */
  NvicMux27 = 27,   /**< NVIC Mux for mapping intrSrc to CM0+ NVIC input 27 */
  NvicMux28 = 28,   /**< NVIC Mux for mapping intrSrc to CM0+ NVIC input 28 */
  NvicMux29 = 29,   /**< NVIC Mux for mapping intrSrc to CM0+ NVIC input 29 */
  NvicMux30 = 30,   /**< NVIC Mux for mapping intrSrc to CM0+ NVIC input 30 */
  NvicMux31 = 31,   /**< NVIC Mux for mapping intrSrc to CM0+ NVIC input 31 */
} IRQn_CM0P_Type;

/** \} group_sysint_enums */


/***************************************
*       Configuration Structure
***************************************/

/**
* \addtogroup group_sysint_data_structures
* \{
*/

/**
* Initialization configuration structure for a single interrupt channel
*/
typedef struct {
    IRQn_Type       intrSrc;        /**< Interrupt source */
    IRQn_CM0P_Type  intrCm0p;       /**< (CM0+ only) Maps intrSrc to 32 available CM0+ NVIC inputs */
    uint32_t        intrPriority;   /**< Interrupt priority number (Refer to __NVIC_PRIO_BITS) */
} cy_stc_sysint_t;

/** \} group_sysint_data_structures */


/***************************************
*              Constants
***************************************/

/** \cond INTERNAL */

#define CY_INT_IRQ_BASE            (16u)    /**< Start location of interrupts in the vector table */
#define CY_SYSINT_STATE_MASK       (1ul)    /**< Mask for interrupt state */
#define CY_SYSINT_STIR_MASK        (0xfful) /**< Mask for software trigger interrupt register */
#define CY_SYSINT_CM0P_MUX_MASK    (0xfful) /**< CM0+ NVIC multiplexer mask */
#define CY_SYSINT_CM0P_MUX_SHIFT   (2u)     /**< CM0+ NVIC multiplexer shift */
#define CY_SYSINT_CM0P_MUX_SCALE   (3u)     /**< CM0+ NVIC multiplexer scaling value */

#define CY_SYSINT_CM0P_MUX0        (0u)     /**< CM0+ NVIC multiplexer register 0 */
#define CY_SYSINT_CM0P_MUX1        (1u)     /**< CM0+ NVIC multiplexer register 1 */
#define CY_SYSINT_CM0P_MUX2        (2u)     /**< CM0+ NVIC multiplexer register 2 */
#define CY_SYSINT_CM0P_MUX3        (3u)     /**< CM0+ NVIC multiplexer register 3 */
#define CY_SYSINT_CM0P_MUX4        (4u)     /**< CM0+ NVIC multiplexer register 4 */
#define CY_SYSINT_CM0P_MUX5        (5u)     /**< CM0+ NVIC multiplexer register 5 */
#define CY_SYSINT_CM0P_MUX6        (6u)     /**< CM0+ NVIC multiplexer register 6 */
#define CY_SYSINT_CM0P_MUX7        (7u)     /**< CM0+ NVIC multiplexer register 7 */

#define CY_SYSINT_CM0P_MUX_ERROR   (0xfffffffful)   /**< Invalid CM0+ NVIC multiplexer error code */

/** \endcond */


/***************************************
*       Function Prototypes
***************************************/

/**
* \addtogroup group_sysint_functions
* \{
*/
cy_en_sysint_status_t Cy_SysInt_Init(const cy_stc_sysint_t* config, cy_israddress userIsr);
uint32_t Cy_SysInt_GetState(const cy_stc_sysint_t* config);
cy_israddress Cy_SysInt_SetVector(const cy_stc_sysint_t* config, cy_israddress userIsr);
cy_israddress Cy_SysInt_GetVector(const cy_stc_sysint_t* config);
__STATIC_INLINE void Cy_SysInt_SetIntSourceNMI(const cy_stc_sysint_t* config);
__STATIC_INLINE uint32_t Cy_SysInt_GetIntSourceNMI(void);
#if (CY_CPU_CORTEX_M0P)
    void Cy_SysInt_SetIntSource(const cy_stc_sysint_t* config);
    uint32_t Cy_SysInt_GetIntSource(IRQn_CM0P_Type muxSel);
#else
    __STATIC_INLINE void Cy_SysInt_SoftwareTrig(const cy_stc_sysint_t* config);
#endif


/*******************************************************************************
* Function Name: Cy_SysInt_SetIntSourceNMI
****************************************************************************//**
*
* \brief Sets the interrupt source of NMI.
*
* Setting this value to 240 disconnects the interrupt source from the NMI. The
* interrupt source must be a positive number.
*
* For CM0+, this function sets the interrupt mux output feeding into the
* NVIC as the source for NMI.
*
* \param config
* Interrupt configuration structure
*
*******************************************************************************/
__STATIC_INLINE void Cy_SysInt_SetIntSourceNMI(const cy_stc_sysint_t* config)
{
    #if CY_CPU_CORTEX_M0P
        CPUSS->CM0_NMI_CTL = config->intrCm0p;
    #else
        CPUSS->CM4_NMI_CTL = config->intrSrc;
    #endif
}


/*******************************************************************************
* Function Name: Cy_SysInt_GetIntSourceNMI
****************************************************************************//**
*
* \brief Gets the interrupt source of the NMI.
*
* \return
* Interrupt Source. A value of 240 means that there is no interrupt source
* for the NMI, and an interrupt can be triggered only through software.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Cy_SysInt_GetIntSourceNMI(void)
{
    #if CY_CPU_CORTEX_M0P
        return (CPUSS->CM0_NMI_CTL);
    #else
        return (CPUSS->CM4_NMI_CTL);
    #endif
}


#if (!CY_CPU_CORTEX_M0P) || defined (CY_DOXYGEN)

/*******************************************************************************
* Function Name: Cy_SysInt_SoftwareTrig
****************************************************************************//**
*
* \brief Triggers an interrupt using software (Not applicable for CM0+).
*
* <b>Note</b> Only privileged software can enable unprivileged access to the
* Software Trigger Interrupt Register (STIR).
*
* \param config
* Interrupt configuration structure
*
*******************************************************************************/
__STATIC_INLINE void Cy_SysInt_SoftwareTrig(const cy_stc_sysint_t* config)
{
    NVIC->STIR = config->intrSrc & CY_SYSINT_STIR_MASK;
}

#endif

/** \} group_sysint_functions */

#if defined(__cplusplus)
}
#endif

#endif /* CY_SYSINT_H */

/** \} group_sysint */

/* [] END OF FILE */
