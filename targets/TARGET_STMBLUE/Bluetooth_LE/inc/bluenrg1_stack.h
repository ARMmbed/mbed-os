#ifndef BLUENRG1_STACK_H
#define BLUENRG1_STACK_H
/**
  ******************************************************************************
  * @file    bluenrg1_stack.h
  * @author  AMS - RF Application team
  * @version V2.0.0
  * @date    15-March-2018
  * @brief   Header file for BlueNRG-1,2 BLE stack initialization and virtual timer
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
  * <h2><center>&copy; COPYRIGHT 2017 STMicroelectronics</center></h2>
  ******************************************************************************
  */
#include <stdint.h>
#include <bluenrg1_api.h>
#include <bluenrg1_events.h>

#define MIN(a,b)                        (((a) < (b))? (a) : (b))
#define MAX(a,b)                        (((a) > (b))? (a) : (b))
#define DIV_CEIL(x, y)                  (((x) + (y) - 1) / (y))

/**
 * DEFAULT_ATT_MTU: minimum mtu value that GATT must support.
 * 5.2.1 ATT_MTU, BLUETOOTH SPECIFICATION Version 4.2 [Vol 3, Part G]
 */
#define DEFAULT_ATT_MTU                 (23)

#define MEM_BLOCK_SIZE                  (32)

/**
 * SM_SECURE_CONN_MTU: mtu size needed for Security Manager Channel Configuration
 * Parameters with LE Secure Connections.
 * 3.2 SECURITY MANAGER CHANNEL OVER L2CAP, BLUETOOTH SPECIFICATION Version 4.2
 * [Vol 3, Part H]
 */
#define SM_SECURE_CONN_MTU              (65)

/**
 * DEFAULT_MAX_ATT_MTU: maximum supported ATT_MTU size.
 */
#define DEFAULT_MAX_ATT_MTU             (251 - 4) // (4 bytes for L2CAP header) 

/**
 * DEFAULT_MAX_ATT_SIZE: maximum attribute size.
 */
#define DEFAULT_MAX_ATT_SIZE            (512)

/**
 * PREP_WRITE_X_ATT(max_att): compute how many Prepare Write Request are needed
 * to write a characteristic with size max_att when the used ATT_MTU value is
 * equal to DEFAULT_ATT_MTU (23).
 */
#define PREP_WRITE_X_ATT(max_att)       (DIV_CEIL(max_att, DEFAULT_ATT_MTU - 5U) * 2)

/**
 * MEM_BLOCK_X_MTU(mtu, n_links): compute how many memory blocks are needed to
 * support a maximum number (n_links)of simultaneous connections that use ATT
 * packet with ATT_MTU=mtu.
 * 7.2 FRAGMENTATION AND RECOMBINATION, BLUETOOTH SPECIFICATION Version 4.2
 * [Vol 3, Part A]
 */

#define MEM_BLOCK_X_TX(mtu)             (DIV_CEIL((mtu) + 4U, MEM_BLOCK_SIZE) + 1U)
#define MEM_BLOCK_X_RX(mtu, n_link)     ((DIV_CEIL((mtu) + 4U, MEM_BLOCK_SIZE) + 2U) * (n_link) + 1)
#define MEM_BLOCK_X_MTU(mtu, n_link)    (MEM_BLOCK_X_TX(mtu) + MEM_BLOCK_X_RX(mtu, (n_link)))

/**
 * Minimum number of blocks required for secure connections
 */
#define MBLOCKS_SECURE_CONNECTIONS      (4)

/**
 * MBLOCKS_CALC(pw, mtu, n_link): minimum number of buffers needed by the stack.
 * This is the minimum racomanded value and depends on:
 *  - pw: size of Prepare Write List
 *  - mtu: ATT_MTU size
 *  - n_link: maximum number of simultaneous connections
 */
#define MBLOCKS_CALC(pw, mtu, n_link)   ((pw) + MAX(MEM_BLOCK_X_MTU(mtu, n_link), (MBLOCKS_SECURE_CONNECTIONS)))

/**
 * DEFAULT_PREP_WRITE_LIST_SIZE: default minimum Prepare Write List size.
 */
#define DEFAULT_PREP_WRITE_LIST_SIZE    PREP_WRITE_X_ATT(DEFAULT_MAX_ATT_SIZE)

/**
 * DEFAULT_MBLOCKS_COUNT: default memory blocks count
 */
#define DEFAULT_MBLOCKS_COUNT           MBLOCKS_CALC(DEFAULT_PREP_WRITE_LIST_SIZE, DEFAULT_MAX_ATT_MTU, NUM_LINKS)

/**
 * A part of the RAM, is dinamically allocated by initilizing all the pointers
 * defined in a global context variable "mem_alloc_ctx_p".
 * This initialization is made in the Dynamic_allocator functions, which
 * assing a portion of RAM given by the external application to the above
 * mentioned "global pointers".
 *
 * The size of this Dynamic RAM is made of 2 main components:
 * - a part that is parameters-dependent (num of links, GATT buffers, ...),
 *   and which value is explicited by the following macro;
 * - a part, that may be considered "fixed", i.e. independent from the above
 *   mentioned parameters.
*/
#define FIXED_BUFFER_SIZE_BYTES (4948+1492+32+4+4-36-8+16)

/**
 * D_LEN_EXT_SIZE: amount of memory needed to support Data Length Extension
 * feature.
 */
#define D_LEN_EXT_SIZE(ENABLED)         (((ENABLED) == 1) ? (436) : (0))

/**
 *
 * This macro returns the amount of memory, in bytes, needed for the storage of GATT database elements
 * and other data structures whose size depends on the number of supported connections.
 *
 * @param NUM_LINKS: Maximum number of simultaneous connections that the device will support. Valid values are from 1 to 8.
 * @param NUM_GATT_ATTRIBUTES: Maximum number of Attributes (i.e. the number of characteristic + the number of characteristic values + the number of descriptors, excluding the services) that can be stored in the GATT database. Note that certain characteristics and relative descriptors are added automatically during device initialization so this parameters should be 9 plus the number of user Attributes
 * @param NUM_GATT_SERVICES: Maximum number of Services that can be stored in the GATT database. Note that the GAP and GATT services are automatically added so this parameter should be 2 plus the number of user services
 * @param ATT_VALUE_ARRAY_SIZE: Size of the storage area for Attribute values.
 * @param MBLOCKS_COUNT: Number of memory blocks allocated for packets.
 * @param D_LEN_EXT_EN: Enable or disable the Extended Packet length feature. Valid values are 0 or 1.
 */
#define TOTAL_BUFFER_SIZE(NUM_LINKS,NUM_GATT_ATTRIBUTES,NUM_GATT_SERVICES,ATT_VALUE_ARRAY_SIZE,MBLOCKS_COUNT,D_LEN_EXT_EN) (\
  FIXED_BUFFER_SIZE_BYTES +                                                     \
  ((((ATT_VALUE_ARRAY_SIZE) - 1) | 3) + 1) +                                    \
  (332 * (NUM_LINKS)) +                                                         \
  (40 * (NUM_GATT_ATTRIBUTES)) +                                                \
  (48 * (NUM_GATT_SERVICES)) +                                                  \
  ((MEM_BLOCK_SIZE + 12) * (MBLOCKS_COUNT)) +                                   \
  D_LEN_EXT_SIZE(D_LEN_EXT_EN)                                                  \
)

/**
*
* This macro returns the amount of memory, in bytes, needed for the storage of non-volatile data for bonded devices
*
* @param FLASH_SEC_DB_SIZE: Size of the database used to store security information for bonded devices. The recommended value for this parameter is 1024.
* @param FLASH_SERVER_DB_SIZE: Size of the database used for service change notification for bonded devices.  The recommended value for this parameter is 1024.
* @note: the total buffer size is rounded-up to the next multiple of on flash sector size, i.e. 2048 bytes
*/
#define TOTAL_FLASH_BUFFER_SIZE(FLASH_SEC_DB_SIZE, FLASH_SERVER_DB_SIZE) (\
  ((((FLASH_SEC_DB_SIZE+FLASH_SERVER_DB_SIZE)-1)|2047)+1)\
  )

/**
* This symbol defines the size of non-volatile device identification data
*/
#define TOTAL_DEVICE_ID_DATA_SIZE (56)

/** @defgroup Master_Sleep_Clock_Accuracy_Definitions   Master Sleep Clock Accuracy Definitions
  * @{
  */
#define MASTER_SCA_500ppm       0 /*!< 251 ppm to 500 ppm */
#define MASTER_SCA_250ppm       1 /*!< 151 ppm to 250 ppm */
#define MASTER_SCA_150ppm       2 /*!< 101 ppm to 150 ppm */
#define MASTER_SCA_100ppm       3 /*!< 76 ppm to 100 ppm */
#define MASTER_SCA_75ppm        4 /*!< 51 ppm to 75 ppm */
#define MASTER_SCA_50ppm        5 /*!< 31 ppm to 50 ppm */
#define MASTER_SCA_30ppm        6 /*!< 21 ppm to 30 ppm */
#define MASTER_SCA_20ppm        7 /*!< 0 ppm to 20 ppm */
/**
  * @}
  */

#ifdef __cplusplus            /////////////antonio
extern "C" {                  /////////////antonio
#endif						  /////////////antonio

/**
*
* This structure contains low level hardware configuration data for the device
*/
typedef struct {
  uint32_t *hot_ana_config_table;   /**< Set to NULL */
  uint32_t max_conn_event_length;   /**< Maximum duration of the connection event when the device is in Slave mode in units of 625/256 us (~2.44 us) */
  uint16_t slave_sca ;              /**< Sleep clock accuracy in Slave mode (ppm value)*/
  uint8_t master_sca ;              /**< Sleep clock accuracy in Master mode (0 to 7 corresponding to 500, 250, 150, 100, 75, 50, 30, 20 ppm)*/
  uint8_t ls_source;                /**< Source for the 32 kHz slow speed clock: 1: internal RO; 0: external crystal */
  uint16_t hs_startup_time ;        /**< Start up time of the high speed (16 or 32 MHz) crystal oscillator in units of 625/256 us (~2.44 us)*/
} hardware_config_table_t;

/**
*
* This structure contains memory and low level hardware configuration data for the device
*/
typedef struct {
  uint8_t* bleStartFlashAddress;    /**< Start address for the non-volatile memory area allocated according to TOTAL_FLASH_BUFFER_SIZE (Aligned to 2048 bytes flash sector boundary) */
  uint32_t secDbSize;               /**< Size of the database used to store security information for bonded devices (FLASH_SEC_DB_SIZE used in the calculation of TOTAL_FLASH_BUFFER_SIZE). Current SUPPORTED VALUE is 1024 */
  uint32_t serverDbSize;            /**< Size of the database used for service change notification for bonded devices (FLASH_SERVER_DB_SIZE used in the calculation of TOTAL_FLASH_BUFFER_SIZE). Current SUPPORTED VALUE is 1024 */
  uint8_t* stored_device_id_data_p; /**< Address of the const device id data vector (56 bytes, 32bit aligned FLASH area, all elements must be initialized to 0xFF) */
  uint8_t* bleStartRamAddress;      /**< Start address of the RAM buffer for GATT database allocated according to TOTAL_BUFFER_SIZE (32bit aligned RAM area) */
  uint32_t total_buffer_size;       /**< TOTAL_BUFFER_SIZE return value, used to check the MACRO correctness*/
  uint16_t numAttrRecord;           /**< Maximum number of Attributes (i.e. the number of characteristic + the number of characteristic values + the number of descriptors, excluding the services) that can be stored in the GATT database. Note that certain characteristics and relative descriptors are added automatically during device initialization so this parameters should be 9 plus the number of user Attributes (NUM_GATT_ATTRIBUTES used in the calculation of TOTAL_BUFFER_SIZE) */
  uint16_t numAttrServ;             /**< Maximum number of Services that can be stored in the GATT database. Note that the GAP and GATT services are automatically added so this parameter should be 2 plus the number of user services (NUM_GATT_SERVICES used in the calculation of TOTAL_BUFFER_SIZE)*/
  uint16_t attrValueArrSize;        /**< Size of the storage area for Attribute values (ATT_VALUE_ARRAY_SIZE used in the calculation of TOTAL_BUFFER_SIZE)
     This value depends on the number of attributes used by application. In particular the sum of the following quantities (in octets) should be made for each attribute:
     - attribute value length
     - 5, if UUID is 16 bit; 19, if UUID is 128 bit
     - 2, if server configuration descriptor is used
     - 2*numOfLinks, if client configuration descriptor is used
     - 2, if extended properties is used

     The total amount of memory needed is the sum of the above quantities for each attribute.
*/
  uint8_t numOfLinks;               /**< Maximum number of simultaneous connections that the device will support. Valid values are from 1 to 8 (NUM_LINKS used in the calculation of TOTAL_BUFFER_SIZE). */
  uint8_t extended_packet_length_enable; /**unsupported feature (reserved for future use)*/
  uint8_t prWriteListSize;          /**< Prepare Write List size in terms of number of packet with ATT_MTU=23 bytes */
  uint8_t mblockCount;              /**< Number of allocated memory blocks */
  uint16_t attMtu;                   /**< Maximum supported ATT_MTU size */
  hardware_config_table_t hardware_config;  /**< Structure containing low level hardware configuration data for the device */
} BlueNRG_Stack_Initialization_t;


/**
 * @brief This function executes the processing of all Host Stack layers.
 *
 * The BTLE Stack Tick function has to be executed regularly to process incoming Link Layer packets and to process Host Layers procedures. All
 * stack callbacks are called by this function.
 *
 * If Low Speed Ring Oscillator is used instead of the LS Crystal oscillator this function performs also the LS RO calibration and hence must
 * be called at least once at every system wake-up in order to keep the 500 ppm accuracy (at least 500 ppm accuracy is mandatory if acting as a master).
 *
 * No BTLE stack function must be called while the BTLE_StackTick is running. For example, if a BTLE stack function may be called inside an
 * interrupt routine, that interrupt must be disabled during the execution of BTLE_StackTick(). Example (if a stack function may be called inside
 * UART ISR):
 * @code
 * NVIC_DisableIRQ(UART_IRQn);
 * BTLE_StackTick();
 * NVIC_EnableIRQ(UART_IRQn);
 * @endcode

 * @note The API name and parameters are subject to change in future releases.
 * @return None
 */
void BTLE_StackTick(void);


/**
 * @brief The BTLE Stack initialization routine
 *
 * @note The API name and parameters are subject to change in future releases.
 *
 * @param[in]  BlueNRG_Stack_Init_params_p      pointer to the const structure containing memory and low level
 *                                              hardware configuration data for the device
 *
 * @return Value indicating success or error code.
 *
 */
tBleStatus BlueNRG_Stack_Initialization(const BlueNRG_Stack_Initialization_t *BlueNRG_Stack_Init_params_p);


/**
 * @brief Returns the BTLE Stack matching sleep mode
 *
 * @note The API name and parameters are subject to change in future releases.
 *
 * @return
 *  SLEEPMODE_RUNNING       = 0,
 *  SLEEPMODE_CPU_HALT      = 1,
 *  SLEEPMODE_WAKETIMER     = 2,
 *  SLEEPMODE_NOTIMER       = 3,
 */
uint8_t BlueNRG_Stack_Perform_Deep_Sleep_Check(void);

/**
 *
 * @brief Radio ISR routine.
 *
 * This is the base function called for any radio ISR.
 *
 * @return None
 */


void RAL_Isr(void);
/**
 * @brief Starts a one-shot virtual timer for the given relative timeout value expressed in ms
 *
 * @note The API name and parameters are subject to change in future releases.
 *
 * @param[in]  timerNum            The virtual timer number [0..3]
 * @param[in]  msRelTimeout        The relative time, from current time, expressed in ms
 *                                 Note: abs(msRelTimeout) <= 5242879
 *
 * @return 0 if the timerNum virtual timer is valid.
 */
int HAL_VTimerStart_ms(uint8_t timerNum, int32_t msRelTimeout);

/**
 * @brief Stops the one-shot virtual timer specified if found
 *
 * @note The API name and parameters are subject to change in future releases.
 *
 * @param[in]  timerNum            The virtual timer number [0..3]
 *
 * @return None
 */
void HAL_VTimer_Stop(uint8_t timerNum);

/**
 * @brief Virtual timer callback
 *
 * This function will be called on the expiry of a one-shot virtual timer.
 * HAL_VTimerTimeoutCallback will notify the virtual timer number.
 *
 * @note The API name and parameters are subject to change in future releases.
 *
 * @param[in]  timerNum            The virtual timer number [0..3]
 *
 * @return None
 */
void HAL_VTimerTimeoutCallback(uint8_t timerNum);

/**
 * @brief This function return the current reference time expressed in internal time units.
 *
 * The returned value can be used as absolute time parameter where needed in the other
 * HAL_VTimer* APIs
 *
 * @note The API name and parameters are subject to change in future releases.
 *
 * @return  32bit absolute current time expressed in internal time units.
 */
uint32_t HAL_VTimerGetCurrentTime_sysT32(void);

/**
 * @brief This function returns the sum of an absolute time and a signed relative time.
 *
 * @param[in]  sysTime            Absolute time expressed in internal time units.
 * @param[in]  msTime             Signed relative time expressed in ms.
 *                                Note: abs(msTime) <= 5242879
 *
 * @note The API name and parameters are subject to change in future releases.
 *
 * @return  32bit resulting absolute time expressed in internal time units.
 */
uint32_t HAL_VTimerAcc_sysT32_ms(uint32_t sysTime, int32_t msTime);


/**
 * @brief Returns a time difference.
 *
 * This function return the difference between two absolute times: sysTime1-sysTime2.
 * The resulting value is expressed in ms.
 *
 * @param[in]  sysTime1           Absolute time expressed in internal time units.
 * @param[in]  sysTime2           Absolute time expressed in internal time units.
 *
 * @note The API name and parameters are subject to change in future releases.
 * @return  32bit resulting signed relative time expressed in ms.
 */
int32_t HAL_VTimerDiff_ms_sysT32(uint32_t sysTime1, uint32_t sysTime2);

/**
 * @brief Starts a one-shot timer.
 *
 * Starts a one-shot virtual timer for the given absolute timeout value expressed in
 * internal time units.
 *
 * @param[in]  timerNum       The virtual timer number [0..3]
 * @param[in]  time           Absolute time expressed in internal time units.
 *
 * @note The API name and parameters are subject to change in future releases.
 * @return 0 if the timerNum virtual timer is idle.
 */
int HAL_VTimerStart_sysT32(uint8_t timerNum, uint32_t time);


/**
 * @brief Returns the absolute expiry time of a running timer.
 *
 * Returned time is expressed in internal time units.
 *
 * @note The API name and parameters are subject to change in future releases.
 *
 * @param[in]  timerNum       The virtual timer number [0..3]
 * @param[out] sysTime        Absolute time expressed in internal time units.
 *
 * @return 0 if the timerNum virtual timer is running.
 */
int HAL_VTimerExpiry_sysT32(uint8_t timerNum, uint32_t *sysTime);

/**
 * @brief This callback is called by the stack library to indicate the arrival of an ACL Data Packet.
 *
 * @note The API name and parameters are subject to change in future releases.
 *
 * @param[in]  connHandle       Connection handle for which the command is given. Range: 0x0000-0x0EFF (0x0F00 - 0x0FFF Reserved for future use)
 * @param[in]  pb_flag          Packet boundary flag
 * @param[in]  bc_flag          Broadcast flag
 * @param[in]  dataLen          Length of PDU data in octets.
 * @param[in] pduData          PDU data pointer
 *
 * @return void
 */

tBleStatus hci_rx_acl_data_event(uint16_t connHandle, uint8_t  pb_flag, uint8_t  bc_flag, uint16_t  dataLen, uint8_t*  pduData);

/**
 * @brief API used to send HCI ACL Data Packets to exchange data between the Host and Controller.
 *
 * @note The API name is only available in link layer only mode.
 *
 * @param[in]  connHandle       Connection handle for which the command is given. Range: 0x0000-0x0EFF (0x0F00 - 0x0FFF Reserved for future use)
 * @param[in]  pb_flag          Packet boundary flag
 * @param[in]  bc_flag          Broadcast flag
 * @param[in]  dataLen          Length of PDU data in octets.
 * @param[in] pduData          PDU data pointer
 *
 * @return  Error code
 */

tBleStatus hci_tx_acl_data(uint16_t connHandle, uint8_t  pb_flag, uint8_t  bc_flag, uint16_t  dataLen, uint8_t*  pduData);

#ifdef __cplusplus  ////////////antonio
}                   ////////////antonio
#endif              ////////////antonio

#endif // defined( BLUENRG1_STACK_H )

