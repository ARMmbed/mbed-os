/**
  ******************************************************************************
  * @file    DTM_config.h 
  * @author  VMA RF Application Team - Antonio O. (Dec. 2018)
  * @version V1.1.0
  * @date    April-2018
  * @brief   Minimal configuration (Antonio O.)
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
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

#ifndef _DTM_CONFIG_H_
#define _DTM_CONFIG_H_
#include "BlueNRG_x_device.h"
#include "bluenrg1_stack.h"
#include "stack_user_cfg.h"

/** This file contains all the information needed to init the BlueNRG-1 stack. 
 * These constants and variables are used from the BlueNRG-1,2 stack to reserve RAM and FLASH 
 * according the application requests.
 * NOTE: the current selected configuration is tuned for supporting up to 8 connections,
 * 8 services, 68 attributes. 
 * MAX ATT_MTU size is set to 220 bytes which is the max allowed value for fitting with the 
 * overall DTM RAM memory requirements for all the supported configurations (SPI and UARTs),  and
 * use the extended packey length for OTA client FW upgrade procedure.
 * This implies to reduce the PREPARE_WRITE_LIST_SIZE value. 
 * User can modify, tune the configuration options according to his specific application requirements. 
 */


/* MAX numberof link for DTM */
#define DTM_NUM_LINK            1                 //8
/* MAX numberof GATT services for DTM */
#define DTM_NUM_GATT_SERVICES   0                 //8
/* MAX number of GAP and GATT attributes for DTM */
#define DTM_NUM_GATT_ATTRIBUTES 0                 //68

/* Number of links needed for the demo: 1
 * Only 1 the default
 */
#define NUM_LINKS               (DTM_NUM_LINK)

/* Number of GATT attributes needed for the DTM */
#define NUM_GATT_ATTRIBUTES     (DTM_NUM_GATT_ATTRIBUTES)

/* Number of GATT services needed for the DTM */
#define NUM_GATT_SERVICES       (DTM_NUM_GATT_SERVICES)

/* Set the number of prepare write request needed to write an att of 512 bytes,
   using MTU default value (23 bytes): value is reduced to 9 (refer to previous added note) */

#define PREPARE_WRITE_LIST_SIZE (0) ////////////////////9  //(DEFAULT_PREP_WRITE_LIST_SIZE)

/* Set supported max value for ATT_MTU */
#define MAX_ATT_MTU              (0)  //220

/* Array size for the attribte value */
#define ATT_VALUE_ARRAY_SIZE    (0) //1344

/* Flash security database size */
#define FLASH_SEC_DB_SIZE       (0x400)

/* Flash server database size */
#define FLASH_SERVER_DB_SIZE    (0x400)

#define OPT_MBLOCKS             (0)


/* Set the number of memory block for packet allocation */
//#define MBLOCKS_COUNT           (MBLOCKS_CALC(PREPARE_WRITE_LIST_SIZE, MAX_ATT_MTU, NUM_LINKS) + OPT_MBLOCKS)
#define MBLOCKS_COUNT 15 //minimal configuration

/* RAM reserved to manage all the data stack according the number of links,
 * number of services, number of attributes and attribute value length
 */
NO_INIT(uint32_t dyn_alloc_a[TOTAL_BUFFER_SIZE(NUM_LINKS,NUM_GATT_ATTRIBUTES,NUM_GATT_SERVICES,ATT_VALUE_ARRAY_SIZE,MBLOCKS_COUNT,CONTROLLER_DATA_LENGTH_EXTENSION_ENABLED)>>2]);

/* FLASH reserved to store all the security database information and
 * and the server database information
 */

NO_INIT_SECTION(uint32_t stacklib_flash_data[TOTAL_FLASH_BUFFER_SIZE(FLASH_SEC_DB_SIZE, FLASH_SERVER_DB_SIZE)>>2], ".noinit.stacklib_flash_data");
/* FLASH reserved to store: security root keys, static random address, public address */


NO_INIT_SECTION(uint8_t stacklib_stored_device_id_data[56], ".noinit.stacklib_stored_device_id_data");


/* Maximum duration of the connection event */
#define MAX_CONN_EVENT_LENGTH 0xFFFFFFFF

/* Sleep clock accuracy: DTM is using internal RO */

/* Sleep clock accuracy in Slave mode */
#define SLAVE_SLEEP_CLOCK_ACCURACY 500

/* Sleep clock accuracy in Master mode */
#define MASTER_SLEEP_CLOCK_ACCURACY MASTER_SCA_500ppm

/* Low Speed Oscillator source */
#if (LS_SOURCE == LS_SOURCE_INTERNAL_RO)
#define LOW_SPEED_SOURCE  1 // Internal RO
#else
#define LOW_SPEED_SOURCE  0 // External 32 KHz
#endif

/* High Speed start up time */
#define HS_STARTUP_TIME 328 // 800 us

/* Radio Config Hot Table */
extern uint8_t hot_table_radio_config[];

/* Low level hardware configuration data for the device */
#define CONFIG_TABLE            \
{                               \
  (uint32_t*)hot_table_radio_config,          \
  MAX_CONN_EVENT_LENGTH,        \
  SLAVE_SLEEP_CLOCK_ACCURACY,   \
  MASTER_SLEEP_CLOCK_ACCURACY,  \
  LOW_SPEED_SOURCE,             \
  HS_STARTUP_TIME               \
}

/* This structure contains memory and low level hardware configuration data for the device */
BlueNRG_Stack_Initialization_t BlueNRG_Stack_Init_params = {
    (uint8_t*)stacklib_flash_data,
    FLASH_SEC_DB_SIZE,
    FLASH_SERVER_DB_SIZE,
    (uint8_t*)stacklib_stored_device_id_data,
    (uint8_t*)dyn_alloc_a,
    TOTAL_BUFFER_SIZE(NUM_LINKS,NUM_GATT_ATTRIBUTES,NUM_GATT_SERVICES,ATT_VALUE_ARRAY_SIZE,MBLOCKS_COUNT,CONTROLLER_DATA_LENGTH_EXTENSION_ENABLED),
    NUM_GATT_ATTRIBUTES,
    NUM_GATT_SERVICES,
    ATT_VALUE_ARRAY_SIZE,
    NUM_LINKS,
    0,
    PREPARE_WRITE_LIST_SIZE,
    MBLOCKS_COUNT,
    MAX_ATT_MTU,
    CONFIG_TABLE,
};

#endif // _SENSORDEMO_CONFIG_H_
