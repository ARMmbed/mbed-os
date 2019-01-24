/**
  ******************************************************************************
  * @file    stack_user_cfg.h
  * @author  AMS - RF Application team
  * @version V1.0.0
  * @date    19 February 2018
  * @brief   BLE stack modular configuration options header file
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT STMicroelectronics</center></h2>
  ******************************************************************************
* \section BLE_Config BLE stack configuration options  

    - In order to configure the BLE stack v2.1 or later the following options are available:

       - BLE stack full configuration: all available modules are included. 
         - User is requested to add the following preprocessor option on project configuration: 

           BLE_STACK_CONFIGURATION=BLE_STACK_FULL_CONFIGURATION

       - BLE stack basic configuration: no Controller Privacy, no LE Secure Connections, no Master GAP role, no Data Length Extension 
         - User is requested to add the following preprocessor option on project configuration: 

           BLE_STACK_CONFIGURATION=BLE_STACK_BASIC_CONFIGURATION 

       - BLE OTA Service support, BLE stack configuration with Data Length Extension (BlueNRG-2, BLE stack v2.1):

           BLE_STACK_CONFIGURATION=BLE_OTA_BASIC_CONFIGURATION

  - NOTE: default configuration is BLE_STACK_CONFIGURATION = BLE_STACK_BASIC_CONFIGURATION
**/
#ifndef _STACK_USER_CFG_H_
#define _STACK_USER_CFG_H_


/* --------------------- BLE stack configuration options --------------------------------------------------- */

/**
 * @brief BLE stack full configuration: all available modules are included
 */
#define BLE_STACK_FULL_CONFIGURATION  (0U)

/**
 * @brief BLE stack basic configuration: no Controller Privacy, no LE Secure Connections, no Master GAP role, no Data Length Extension 
 */
#define BLE_STACK_BASIC_CONFIGURATION (1U)

/**
 * @brief BLE OTA Service support, BLE stack configuration with Data Length Extension (BlueNRG-2, BLE stack v2.1)
 */
#define BLE_OTA_BASIC_CONFIGURATION   (2U)

/* Select default configuration */
#ifndef BLE_STACK_CONFIGURATION
#error "you shall define basic configuration"
/**
* @brief BLE stack default configuration
*/
#define BLE_STACK_CONFIGURATION        BLE_STACK_FULL_CONFIGURATION
#endif

/* --------------------- BLE stack configuration options and associated module inclusion -------------------- */

/**
 * @brief Full BLE stack configuration
 */
#if (BLE_STACK_CONFIGURATION == BLE_STACK_FULL_CONFIGURATION) 

/* Controller Privacy module: ENABLED */
#define CONTROLLER_PRIVACY_ENABLED               (1U)

/* LE Secure Connections module: ENABLED */
#define SECURE_CONNECTIONS_ENABLED               (1U)

/* Master GAP role module: ENABLED */
#define CONTROLLER_MASTER_ENABLED                (1U)

#ifdef BLUENRG2_DEVICE

 /* Data length extension module: ENABLED */
 #define CONTROLLER_DATA_LENGTH_EXTENSION_ENABLED (1U)

#else /* BlueNRG-1 device doesn't support Data length extension */

 /* Data length extension module: DISABLED */
 #define CONTROLLER_DATA_LENGTH_EXTENSION_ENABLED (0U)

#endif 

/**
 * @brief Basic BLE stack configuration
 */
#elif (BLE_STACK_CONFIGURATION == BLE_STACK_BASIC_CONFIGURATION) 

/* Controller Privacy module: DISABLED */
#define CONTROLLER_PRIVACY_ENABLED               (0U)

/* LE Secure Connections module: DISABLED */
#define SECURE_CONNECTIONS_ENABLED               (0U)

/* Master GAP role module: DISABLED */
#define CONTROLLER_MASTER_ENABLED                (0U)

/* Data length extension module: DISABLED */
#define CONTROLLER_DATA_LENGTH_EXTENSION_ENABLED (0U)

/**
 * @brief BLE OTA Service support, BLE stack configuration with Data Length Extension (BlueNRG-2, BLE stack v2.1)
 */
#elif (BLE_STACK_CONFIGURATION == BLE_OTA_BASIC_CONFIGURATION) 

/* Controller Privacy module: DISABLED */
#define CONTROLLER_PRIVACY_ENABLED               (0U)

/* LE Secure Connections module: DISABLED */
#define SECURE_CONNECTIONS_ENABLED               (0U)

/* Master GAP role module: DISABLED */
#define CONTROLLER_MASTER_ENABLED                (0U)

#ifdef BLUENRG2_DEVICE

 /* Data length extension module: ENABLED */
 #define CONTROLLER_DATA_LENGTH_EXTENSION_ENABLED (1U)

#else /* BlueNRG-1 device doesn't support Data length extension */

 /* Data length extension module: DISABLED */
 #define CONTROLLER_DATA_LENGTH_EXTENSION_ENABLED (0U)

#endif 
#endif
#endif /* _STACK_USER_CFG_H_ */

