/**************************************************************************//**
* \file cy_ble_stack_pvt.h
* \version 2.0
*
* \brief
*  Contains the function prototypes and constants for the HAL section
*  of the BLE component.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#ifndef CY_BLE_CY_BLE_STACK_PVT_H
#define CY_BLE_CY_BLE_STACK_PVT_H

#include "BLE_config.h"
#include "cy_ble_stack.h"
#include "sysclk/cy_sysclk.h"
#include "syspm/cy_syspm.h"

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/***************************************
*   Function Prototypes
***************************************/

/* Mapping functions for stack size optimization */
#if(CY_BLE_MODE_PROFILE)
#if(CY_BLE_SECURE_CONN_FEATURE_ENABLED)
    void Cy_BLE_HAL_EccHeapInit(uint8_t *heapMem, uint8_t numOfConn); 
    void Cy_BLE_HAL_EccHeapDeInit(void);
    uint16_t Cy_BLE_HAL_EccGetFeatureHeapReq(uint8_t numOfConn);
    cy_en_ble_api_result_t Cy_BLE_HAL_EccGenerateSecurityKeypair(uint8_t p_publicKey[], uint8_t p_privateKey[], 
                                                                 uint8_t random[]);
    cy_en_ble_api_result_t Cy_BLE_HAL_EccGenerateDHKey(const uint8_t p_publicKey[], const uint8_t p_privateKey[], 
                                                       uint8_t p_secret[], uint8_t ci);
    cy_en_ble_api_result_t Cy_BLE_HAL_EccValidPublicKey(const uint8_t p_publicKey[]);
    cy_en_ble_api_result_t Cy_BLE_HAL_PairingLocalPublicKeyHandler(void *param);
    cy_en_ble_api_result_t Cy_BLE_HAL_PairingRemoteKeyHandler(void *param);
    cy_en_ble_api_result_t Cy_BLE_HAL_PairingDhkeyHandler(void *param);
    cy_en_ble_api_result_t Cy_BLE_HAL_PairingDhkeyCheckHandler(void *param);
    cy_en_ble_api_result_t Cy_BLE_HAL_PairingKeypressNotificationHandler(void *param);
    cy_en_ble_api_result_t Cy_BLE_HAL_PairingRandHandler(void *param);
    cy_en_ble_api_result_t Cy_BLE_HAL_PairingConfirmHandler(void *param);
    cy_en_ble_api_result_t Cy_BLE_HAL_PairingLrConfirmingHandler(void *param);
    cy_en_ble_api_result_t Cy_BLE_HAL_PairingLrConfirmingHandler(void *param);
    cy_en_ble_api_result_t Cy_BLE_HAL_PairingScTbxDhkeyGenerateComplete(void* param);
    cy_en_ble_api_result_t Cy_BLE_HAL_PairingScTbxGenerateLocalP256PublicKey(uint8_t param);
    cy_en_ble_api_result_t Cy_BLE_HAL_PairingScTbxGenerateDHkey(void  *param1, void  *param2, uint8_t param3);
    cy_en_ble_api_result_t Cy_BLE_HAL_SeSmpScUserPasskeyHandler(void *param1, void *param2);
    cy_en_ble_api_result_t Cy_BLE_HAL_PairingScTbxLocalPubkeyGenerateComplete(void *param);
    void Cy_BLE_HAL_SmpScCmacComplete(uint8_t param);
    void Cy_BLE_HAL_EccPointMult(uint8_t param);
#else
    void Cy_BLE_HAL_EccHeapInit(uint8_t * heapMem CY_UNUSED, 
                                uint8_t numOfConn CY_UNUSED); 
    void Cy_BLE_HAL_EccHeapDeInit(void);
    uint16_t Cy_BLE_HAL_EccGetFeatureHeapReq(uint8_t numOfConn CY_UNUSED);
    cy_en_ble_api_result_t Cy_BLE_HAL_EccGenerateSecurityKeypair(uint8_t p_publicKey[] CY_UNUSED, 
                                                                 uint8_t p_privateKey[] CY_UNUSED, 
                                                                 uint8_t random[] CY_UNUSED);
    cy_en_ble_api_result_t Cy_BLE_HAL_EccGenerateDHKey(const uint8_t p_publicKey[] CY_UNUSED, 
                                                       const uint8_t p_privateKey[] CY_UNUSED, 
                                                       uint8_t p_secret[] CY_UNUSED, uint8_t ci CY_UNUSED);
    cy_en_ble_api_result_t Cy_BLE_HAL_EccValidPublicKey(const uint8_t p_publicKey[] CY_UNUSED);
    cy_en_ble_api_result_t Cy_BLE_HAL_PairingLocalPublicKeyHandler(void *param CY_UNUSED);
    cy_en_ble_api_result_t Cy_BLE_HAL_PairingRemoteKeyHandler(void *param CY_UNUSED);
    cy_en_ble_api_result_t Cy_BLE_HAL_PairingDhkeyHandler(void *param CY_UNUSED);
    cy_en_ble_api_result_t Cy_BLE_HAL_PairingDhkeyCheckHandler(void *param CY_UNUSED);
    cy_en_ble_api_result_t Cy_BLE_HAL_PairingKeypressNotificationHandler(void *param CY_UNUSED);
    cy_en_ble_api_result_t Cy_BLE_HAL_PairingRandHandler(void *param CY_UNUSED);
    cy_en_ble_api_result_t Cy_BLE_HAL_PairingConfirmHandler(void *param CY_UNUSED);
    cy_en_ble_api_result_t Cy_BLE_HAL_PairingLrConfirmingHandler(void *param CY_UNUSED);
    cy_en_ble_api_result_t Cy_BLE_HAL_PairingScTbxDhkeyGenerateComplete(void* param CY_UNUSED);
    cy_en_ble_api_result_t Cy_BLE_HAL_PairingScTbxGenerateLocalP256PublicKey(uint8_t param CY_UNUSED);
    cy_en_ble_api_result_t Cy_BLE_HAL_PairingScTbxGenerateDHkey(void * param1 CY_UNUSED,
                                                               void * param2 CY_UNUSED,
                                                               uint8_t  param3 CY_UNUSED);
    cy_en_ble_api_result_t Cy_BLE_HAL_SeSmpScUserPasskeyHandler(void *param1 CY_UNUSED,
                                                                void *param2 CY_UNUSED);
    cy_en_ble_api_result_t Cy_BLE_HAL_PairingScTbxLocalPubkeyGenerateComplete(void *param CY_UNUSED);
    void Cy_BLE_HAL_SmpScCmacComplete(uint8_t param CY_UNUSED);
    void Cy_BLE_HAL_EccPointMult(uint8_t param CY_UNUSED);
#endif /* (CY_BLE_SECURE_CONN_FEATURE_ENABLED) */
#endif  /* CY_BLE_MODE_PROFILE */

#if(CY_BLE_SECURE_CONN_FEATURE_ENABLED)
    uint16_t BLE_CMP_FTR_API_lec_hci_handle_read_local_P256_public_key_command(void *param);
    uint16_t BLE_CMP_FTR_API_lec_hci_handle_generate_DHkey_command(void *param);
#else
    uint16_t BLE_CMP_FTR_API_lec_hci_handle_read_local_P256_public_key_command(void *param CY_UNUSED);
    uint16_t BLE_CMP_FTR_API_lec_hci_handle_generate_DHkey_command(void *param CY_UNUSED);
#endif /* CY_BLE_SECURE_CONN_FEATURE_ENABLED*/


/***************************************
*     Stack manager prototypes
***************************************/

/* Extern definitions for BLE stack ISR handler */
extern void Cy_BLE_HAL_BlessInterruptHandler(void);

/* To be used only for HCI_MODE and in Debug mode
* where the logger serial port is enabled
*/
extern void Cy_BLE_HAL_UartTxCompltHandler(void);

/* To be used only for HCI_MODE */
extern void Cy_BLE_HAL_UartRxDataHandler(uint8_t byte);

/* To be used only for Host MODE */
extern void Cy_BLE_HAL_HciTransportHandler(char_t packet, uint16_t packet_len);


/***************************************
*     Macros
***************************************/
#define CY_BLE_INTR_TX_SUCCESS       0x00u
#define CY_BLE_INTR_TX_DONE          0x00u
#define CY_BLE_INTR_TX_OVERFLOW      0x01u

#define CY_BLE_INTR_RX_SUCCESS       0x00u
#define CY_BLE_INTR_RX_NOT_EMPTY     0x00u
#define CY_BLE_INTR_RX_OVERFLOW      0x01u
#define CY_BLE_INTR_RX_FRAME_ERROR   0x02u
#define CY_BLE_INTR_RX_PARITY_ERROR  0x04u


/* Align buffer size value to 4 */
#define CY_BLE_STACK_ALIGN_TO_4(x)       ((((x) & 3u) == 0u) ?    \
                                        (x) :                    \
                                        (((x) - ((x) & 3u)) + 4u))
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CY_BLE_CY_BLE_STACK_PVT_H  */

/* [] END OF FILE */
