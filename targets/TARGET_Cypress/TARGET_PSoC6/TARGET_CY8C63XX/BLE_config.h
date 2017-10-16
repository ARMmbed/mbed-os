/***************************************************************************//**
* \file CY_BLE_config.h
* \version 2.0
*
* \brief
*  Contains the function prototypes and constants for the BLE Component.
*
********************************************************************************
* \copyright
* Copyright 2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#if !defined(CY_BLE_CONFIG_H)
#define CY_BLE_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

//#include "cyfitter.h"
#include "syslib/cy_syslib.h"


/**
* \addtogroup group_macros
* @{
*/

/* BLE_bless_isr */
#define BLE_bless_isr__INTC_CORTEXM0P_ASSIGNED 1
#define BLE_bless_isr__INTC_CORTEXM0P_MUX 3u
#define BLE_bless_isr__INTC_CORTEXM0P_PRIORITY 3u
#define BLE_bless_isr__INTC_CORTEXM4_ASSIGNED 1
#define BLE_bless_isr__INTC_CORTEXM4_PRIORITY 7u
#define BLE_bless_isr__INTC_NUMBER 24u


/***************************************
* Services Enable defines
***************************************/


    
/***************************************
* API Constants
***************************************/

/** The component operating mode. */
#define CY_BLE_CONFIG_MODE                          (0u)
#define CY_BLE_PROFILE                              (0u)
#define CY_BLE_HCI                                  (1u)

/** The core for the Host. For DUAL core device Controller will be compiled for different core. */
#define CY_BLE_CONFIG_HOST_CORE                     (0u)
/** The core for the Controller in HCI mode. */
#define CY_BLE_CONFIG_HCI_CONTR_CORE                (1u)

#define CY_BLE_CORE_CORTEX_M4                       (0u)
#define CY_BLE_CORE_CORTEX_M0P                      (1u)
#define CY_BLE_CORE_CORTEX_NONE                     (0xffu)

#define CY_BLE_HOST_CORE                            ((CY_BLE_CONFIG_HOST_CORE == CY_BLE_CORE_CORTEX_M4) ? \
                                                      CY_CPU_CORTEX_M4 : CY_CPU_CORTEX_M0P)

#define CY_BLE_HCI_CONTR_CORE                       ((CY_BLE_CONFIG_HCI_CONTR_CORE == CY_BLE_CORE_CORTEX_M4) ? \
                                                      CY_CPU_CORTEX_M4 : CY_CPU_CORTEX_M0P)

#define CY_BLE_CONTR_CORE                           ((CY_BLE_CONFIG_MODE == CY_BLE_HCI) ? CY_BLE_HCI_CONTR_CORE : \
                                                        ((CY_BLE_CONFIG_HOST_CORE == CY_BLE_CORE_CORTEX_M0P) ? \
                                                        CY_CPU_CORTEX_M4 : CY_CPU_CORTEX_M0P))

#define CY_BLE_MODE_PROFILE                         ((CY_BLE_CONFIG_MODE == CY_BLE_PROFILE) && (CY_BLE_HOST_CORE))
#define CY_BLE_MODE_HCI                             ((CY_BLE_CONFIG_MODE == CY_BLE_HCI) && (CY_BLE_HCI_CONTR_CORE))

#define CY_BLE_CONFIG_EXT_PA_ENABLED                ((0u | \
                                                      0u | \
                                                      0u) != 0u)

/* Indicates whether deep sleep mode is used */
#define CY_BLE_CONFIG_USE_DEEP_SLEEP                (1u)

/** Determines the internal stack mode. Used to switch the operation for debugging.
    - ReleaseMode - Host and Controller with software interface.
    - DebugMode - Host and Controller with IPC interface.
    - HostOnly - Host with UART interface.
    - HostContrUart - Host and Controller with a UART interface.
*/
#define CY_BLE_CONFIG_STACK_MODE                    (2u)
#define CY_BLE_CONFIG_STACK_HOST_ONLY               (1u)
#define CY_BLE_CONFIG_STACK_DEBUG                   (2u)
#define CY_BLE_CONFIG_STACK_RELEASE                 (3u)
#define CY_BLE_CONFIG_STACK_DEBUG_UART              (4u)
#define CY_BLE_CONFIG_STACK_HOST_IPC                (5u)

#define CY_BLE_CONFIG_STACK_MODE_CONTR_UART         ((CY_BLE_MODE_HCI) || \
                                                    ((CY_BLE_CONFIG_MODE == CY_BLE_PROFILE) && \
                                                    (CY_BLE_CONFIG_STACK_MODE == CY_BLE_CONFIG_STACK_DEBUG_UART) && \
                                                        CY_BLE_CONTR_CORE)) 

#define CY_BLE_CONFIG_STACK_MODE_HOST_UART          ((CY_BLE_MODE_PROFILE) && \
                                                    (((CY_BLE_CONFIG_STACK_MODE == CY_BLE_CONFIG_STACK_HOST_ONLY) || \
                                                     (CY_BLE_CONFIG_STACK_MODE == CY_BLE_CONFIG_STACK_DEBUG_UART)) && \
                                                        CY_BLE_HOST_CORE))

#define CY_BLE_CONFIG_STACK_MODE_HOST_IPC           ((CY_BLE_MODE_PROFILE) && \
                                                    (((CY_BLE_CONFIG_STACK_MODE == CY_BLE_CONFIG_STACK_HOST_IPC) || \
                                                     (CY_BLE_CONFIG_STACK_MODE == CY_BLE_CONFIG_STACK_DEBUG)) && \
                                                        CY_BLE_HOST_CORE))

#define CY_BLE_CONFIG_STACK_CONTR_CORE              ((CY_BLE_MODE_HCI) || \
                                                    ((CY_BLE_CONFIG_MODE == CY_BLE_PROFILE) && \
                                                    ((CY_BLE_CONFIG_STACK_MODE == CY_BLE_CONFIG_STACK_RELEASE) && \
                                                        (CY_BLE_HOST_CORE)) || \
                                                    (((CY_BLE_CONFIG_STACK_MODE == CY_BLE_CONFIG_STACK_DEBUG) || \
                                                    (CY_BLE_CONFIG_STACK_MODE == CY_BLE_CONFIG_STACK_DEBUG_UART)) &&\
                                                        (CY_BLE_CONTR_CORE))))

#define CY_BLE_CONFIG_STACK_IPC_CONTR_CORE          ((CY_BLE_CONFIG_MODE == CY_BLE_PROFILE) && \
                                                    (CY_BLE_CONFIG_STACK_MODE == CY_BLE_CONFIG_STACK_DEBUG) && \
                                                    (CY_BLE_CONTR_CORE))

#define CY_BLE_CONFIG_CONTR_CORE                    ((CY_BLE_CONFIG_MODE == CY_BLE_HCI) ? \
                                                        CY_BLE_CONFIG_HCI_CONTR_CORE : \
                                                    ((CY_BLE_CONFIG_STACK_MODE == CY_BLE_CONFIG_STACK_RELEASE) ? \
                                                        CY_BLE_CONFIG_HOST_CORE : \
                                                    (((CY_BLE_CONFIG_STACK_MODE == CY_BLE_CONFIG_STACK_DEBUG) || \
                                                     (CY_BLE_CONFIG_STACK_MODE == CY_BLE_CONFIG_STACK_DEBUG_UART)) ? \
                                                        ((CY_BLE_CONFIG_HOST_CORE == CY_BLE_CORE_CORTEX_M4) ? \
                                                            CY_BLE_CORE_CORTEX_M0P : CY_BLE_CORE_CORTEX_M4) : \
                                                        CY_BLE_CORE_CORTEX_NONE)) )

#define CY_BLE_HOST_CONTR_CORE				        (CY_BLE_MODE_PROFILE || CY_BLE_CONFIG_STACK_CONTR_CORE)	

#if(CY_BLE_CONFIG_STACK_CONTR_CORE) /* Check for the proper BLESS ISR configuration in the DWR  */
    
#if ((CY_BLE_CONFIG_CONTR_CORE == CY_BLE_CORE_CORTEX_M4) && \
    (!defined BLE_bless_isr__INTC_CORTEXM4_ASSIGNED))
    #error Enable BLE_bless_isr interrupt for the ARM CM4 core in the interrupt configuration tab of \
        the Design-Wide Resources (DWR) file (project.cydwr)     
#endif /* (CY_BLE_CONFIG_CONTR_CORE == CY_BLE_CORE_CORTEX_M4) */
#if ((CY_BLE_CONFIG_CONTR_CORE == CY_BLE_CORE_CORTEX_M0P) && \
    (!defined BLE_bless_isr__INTC_CORTEXM0P_ASSIGNED))
    #error Enable BLE_bless_isr interrupt for the ARM CM0+ core in the interrupt configuration tab of \
        the Design-Wide Resources (DWR) file (project.cydwr)     
#endif /* CY_BLE_CONFIG_CONTR_CORE == CY_BLE_CORE_CORTEX_M0P) */

#endif /* (CY_BLE_CONFIG_STACK_CONTR_CORE) */

#if(CY_BLE_CONFIG_STACK_MODE_CONTR_UART) /* Check for the proper UART ISR configuration in the DWR  */

#if((CY_BLE_CONFIG_CONTR_CORE == CY_BLE_CORE_CORTEX_M4) && \
    (!defined BLE_uart_isr__INTC_CORTEXM4_ASSIGNED))
    #error Enable BLE_uart_isr interrupt for the ARM CM4 core in the interrupt configuration tab of \
        the Design-Wide Resources (DWR) file (project.cydwr)     
#endif /* CY_BLE_CONFIG_CONTR_CORE == CY_BLE_CORE_CORTEX_M4 */
#if((CY_BLE_CONFIG_CONTR_CORE == CY_BLE_CORE_CORTEX_M0P) && \
    (!defined BLE_uart_isr__INTC_CORTEXM0P_ASSIGNED))
    #error Enable BLE_uart_isr interrupt for the ARM CM0+ core in the interrupt configuration tab of \
        the Design-Wide Resources (DWR) file (project.cydwr)     
#endif /* CY_BLE_CONFIG_CONTR_CORE == CY_BLE_CORE_CORTEX_M0P */
    
#endif /* (CY_BLE_CONFIG_STACK_MODE_CONTR_UART) */

#if(CY_BLE_CONFIG_STACK_MODE_HOST_UART) /* Check for the proper HOST UART ISR configuration in the DWR  */

#if((CY_BLE_CONFIG_HOST_CORE == CY_BLE_CORE_CORTEX_M4) && \
    (!defined BLE_host_uart_isr__INTC_CORTEXM4_ASSIGNED))
    #error Enable BLE_host_uart_isr interrupt for the ARM CM4 core in the interrupt configuration tab of \
        the Design-Wide Resources (DWR) file (project.cydwr)     
#endif /* CY_BLE_CONFIG_HOST_CORE == CY_BLE_CORE_CORTEX_M4 */
#if((CY_BLE_CONFIG_HOST_CORE == CY_BLE_CORE_CORTEX_M0P) && \
    (!defined BLE_host_uart_isr__INTC_CORTEXM0P_ASSIGNED))
    #error Enable BLE_host_uart_isr interrupt for the ARM CM0+ core in the interrupt configuration tab of \
        the Design-Wide Resources (DWR) file (project.cydwr)     
#endif /* CY_BLE_CONFIG_HOST_CORE == CY_BLE_CORE_CORTEX_M0P */
    
#endif /* (CY_BLE_CONFIG_STACK_MODE_HOST_UART) */

/** The maximum number of BLE connections */
#define CY_BLE_CONFIG_CONN_COUNT                    (4u)
/** The number of BLE connections */
#define CY_BLE_CONFIG_GATTC_COUNT                   (0x00u)
/** The number of GAP Peripheral configurations */
#define CY_BLE_CONFIG_GAP_PERIPHERAL_COUNT          (0x01u)
/** The number of GAP Broadcaster configurations */
#define CY_BLE_CONFIG_GAP_BROADCASTER_COUNT         (0x00u)
/** The number of GAP Central configurations */
#define CY_BLE_CONFIG_GAP_CENTRAL_COUNT             (0x00u)
/** The number of GAP Observer configurations */
#define CY_BLE_CONFIG_GAP_OBSERVER_COUNT            (0x00u)
/** The number of Security configurations */
#define CY_BLE_CONFIG_AUTH_INFO_COUNT               (0x01u)
/** LL Privacy 1.2 feature */
#define CY_BLE_CONFIG_ENABLE_LL_PRIVACY             (0x01u)
/** LE 2 Mbps feature */
#define CY_BLE_CONFIG_ENABLE_PHY_UPDATE             (0u)
/** Radio power calibration */
#define CY_BLE_CONFIG_TX_POWER_CALIBRATION_ENABLE 	(0u)      
/** The GAP security level */
#define CY_BLE_CONFIG_GAP_SECURITY_LEVEL            (0x00u)
#define CY_BLE_CONFIG_SHARING_MODE                  (0u)
/** The GAP Role */
#define CY_BLE_CONFIG_GAP_ROLE                      (0x01u)
/** The Bonding Requirement */
#define CY_BLE_CONFIG_BONDING_REQUIREMENT           (0x00u)
/** The maximum number of bonded devices to be supported by this device.  */
#define CY_BLE_CONFIG_MAX_BONDED_DEVICES            (16u)
/** The maximum number of peer devices whose addresses should be resolved by this device. */
#define CY_BLE_CONFIG_MAX_RESOLVABLE_DEVICES        (16u)
/** The maximum number of devices that can be added to the whitelist. */
#define CY_BLE_CONFIG_MAX_WHITE_LIST_SIZE           (16u)

#define CY_BLE_SECURITY_CONFIGURATION_0_INDEX   (0x00u)

#define CY_BLE_ADV_PKT_0_INDEX_FLAGS   (0x00u)
#define CY_BLE_PERIPHERAL_CONFIGURATION_0_INDEX   (0x00u)


/* Additional queue depth provided from the customizer  */
#define CY_BLE_CONFIG_ADD_Q_DEPTH_PER_CONN          (0u)

/** GATT MTU Size */
#define CY_BLE_CONFIG_GATT_MTU                      (0x0017u)

/** GATT Maximum attribute length */
#define CY_BLE_CONFIG_GATT_DB_MAX_VALUE_LEN         (0x000Fu)

#define CY_BLE_GATT_DB_INDEX_COUNT                  (0x000Fu)

/** The number of characteristics supporting a Reliable Write property */
#define CY_BLE_CONFIG_GATT_RELIABLE_CHAR_COUNT      (0x0000u)
/** The total length of characteristics with Reliable Write property */
#define CY_BLE_CONFIG_GATT_RELIABLE_CHAR_LENGTH     (0x0000u)

/** The parameter to enable an application to provide a dynamically allocated buffer for preparing a Write request.  */            
#define CY_BLE_CONFIG_GATT_ENABLE_EXTERNAL_PREP_WRITE_BUFF   (0u)

/** The parameter to enable configuration of the L2CAP logical channels */
#define CY_BLE_CONFIG_L2CAP_ENABLE                  (1u)
    
#if(CY_BLE_CONFIG_L2CAP_ENABLE != 0u)
    /** L2CAP MTU Size */
    #define CY_BLE_CONFIG_L2CAP_MTU                 (23u)
    /** L2CAP MPS Size */
    #define CY_BLE_CONFIG_L2CAP_MPS                 (23u)
    /** Number of L2CAP Logical channels */
    #define CY_BLE_CONFIG_L2CAP_LOGICAL_CHANNEL_COUNT        (1u) 
    /** Number of L2CAP PSMs */
    #define CY_BLE_CONFIG_L2CAP_PSM_COUNT           (1u)
#endif  /* CY_BLE_L2CAP_ENABLE != 0u */


#define CY_BLE_CONFIG_GATT_DB_ATT_VAL_COUNT         (0x07u)

/** Max Tx payload size */
#define CY_BLE_CONFIG_LL_MAX_TX_PAYLOAD_SIZE        (0x1Bu)
/** Max Rx payload size */
#define CY_BLE_CONFIG_LL_MAX_RX_PAYLOAD_SIZE        (0x1Bu)

/** GATT Role */
#define CY_BLE_CONFIG_GATT_ROLE                     (0x01u)
#define CY_BLE_CONFIG_GATT_DB_CCCD_COUNT            (0x02u)

/** Max unique services in the project */
#define CY_BLE_MAX_SRVI                             (0x01u)


/***************************************
* API Constants for BLE services
* (using in CY_BLE_<service>.h)
***************************************/

/* HIDS */
/** The maximum supported count of HID services for the GATT Server role */
#define CY_BLE_CONFIG_HIDSS_SERVICE_COUNT           (0x00u)
/** The maximum supported count of HID reports for the GATT Server role */
#define CY_BLE_CONFIG_HIDSS_REPORT_COUNT            (0x00u)
    
/** The maximum supported count of HID services for the GATT Client role */
#define CY_BLE_CONFIG_HIDSC_SERVICE_COUNT           (0x00u)
/** The maximum supported count of HID reports for the GATT Client role  */
#define CY_BLE_CONFIG_HIDSC_REPORT_COUNT            (0x00u)





/* BAS */
/** The maximum supported count of BAS services for the GATT Server role */
#define CY_BLE_CONFIG_BASS_SERVICE_COUNT            (0x00u)
/** The maximum supported count of BAS reports for the GATT Client role */
#define CY_BLE_CONFIG_BASC_SERVICE_COUNT            (0x00u)



/* ESS */
/** The maximum supported count of ESS characteristics for the GATT Client role */
#define CY_BLE_CONFIG_ES_TOTAL_CHAR_COUNT           (0x00u)

/* AIOS */
/** The maximum supported count of AIOS characteristics for the GATT Client role */
#define CY_BLE_CONFIG_AIO_TOTAL_CHAR_COUNT          (0x00u)

/* CUSTOM */
/** The maximum supported count of Custom services for the GATT Server role */
#define CY_BLE_CONFIG_CUSTOMS_SERVICE_COUNT         (0x00u)
/** The maximum supported count of Custom services for the GATT Client role */
#define CY_BLE_CONFIG_CUSTOMC_SERVICE_COUNT         (0x00u)
/** The maximum supported count of the Custom Service characteristics */
#define CY_BLE_CONFIG_CUSTOM_SERVICE_CHAR_COUNT     (0x00u)
/** The maximum supported count of the Custom Service descriptors in one characteristic */
#define CY_BLE_CONFIG_CUSTOM_SERVICE_CHAR_DESCRIPTORS_COUNT  (0x00u)

/* Below are the indexes and handles of the defined Custom Services and their characteristics */



/** @} group_macros */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CY_BLE_CONFIG_H */

/* [] END OF FILE */
