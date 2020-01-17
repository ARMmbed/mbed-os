/**
 ******************************************************************************
 * @file    mbox_def.h
 * @author  MCD Application Team
 * @brief   Mailbox definition
 ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
 */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MBOX_H
#define __MBOX_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32_wpan_common.h"

  /**
   * This file shall be identical between the CPU1 and the CPU2
   */

  /**
   *********************************************************************************
   * TABLES
   *********************************************************************************
   */

  /**
   * Version
   * [0:3]   = Build - 0: Untracked - 15:Released - x: Tracked version
   * [4:7]   = branch - 0: Mass Market - x: ...
   * [8:15]  = Subversion
   * [16:23] = Version minor
   * [24:31] = Version major
   *
   * Memory Size
   * [0:7]   = Flash ( Number of 4k sector)
   * [8:15]  = Reserved ( Shall be set to 0 - may be used as flash extension )
   * [16:23] = SRAM2b ( Number of 1k sector)
   * [24:31] = SRAM2a ( Number of 1k sector)
   */
  typedef PACKED_STRUCT
  {
    uint32_t    Version;
  } MB_SafeBootInfoTable_t;

  typedef PACKED_STRUCT
  {
    uint32_t    Version;
    uint32_t    MemorySize;
    uint32_t    FusInfo;
  } MB_FusInfoTable_t;

  typedef PACKED_STRUCT
  {
    uint32_t    Version;
    uint32_t    MemorySize;
    uint32_t    InfoStack;
    uint32_t    Reserved;
  } MB_WirelessFwInfoTable_t;

  typedef struct
  {
    MB_SafeBootInfoTable_t      SafeBootInfoTable;
    MB_FusInfoTable_t           FusInfoTable;
    MB_WirelessFwInfoTable_t    WirelessFwInfoTable;
  } MB_DeviceInfoTable_t;

  typedef struct
  {
    uint8_t     *pcmd_buffer;
    uint8_t     *pcs_buffer;
    uint8_t     *pevt_queue;
    uint8_t     *phci_acl_data_buffer;
  } MB_BleTable_t;

  typedef struct
  {
    uint8_t   *notack_buffer;
    uint8_t   *clicmdrsp_buffer;
    uint8_t   *otcmdrsp_buffer;
  } MB_ThreadTable_t;

  typedef struct
  {
    uint8_t   *clicmdrsp_buffer;
    uint8_t   *m0cmd_buffer;
  } MB_LldTestsTable_t;

  typedef struct
  {
    uint8_t   *notifM0toM4_buffer;
    uint8_t   *appliCmdM4toM0_buffer;
    uint8_t   *loggingM0toM4_buffer;
  } MB_ZigbeeTable_t;
  /**
   * msg
   * [0:7]   = cmd/evt
   * [8:31] = Reserved
   */
  typedef struct
  {
    uint8_t   *pcmd_buffer;
    uint8_t   *sys_queue;
  } MB_SysTable_t;

  typedef struct
  {
    uint8_t     *spare_ble_buffer;
    uint8_t     *spare_sys_buffer;
    uint8_t     *blepool;
    uint32_t    blepoolsize;
    uint8_t     *pevt_free_buffer_queue;
    uint8_t     *traces_evt_pool;
    uint32_t    tracespoolsize;
  } MB_MemManagerTable_t;

  typedef struct
  {
    uint8_t   *traces_queue;
  } MB_TracesTable_t;

  typedef struct
  {
    uint8_t   *p_cmdrsp_buffer;
    uint8_t   *p_notack_buffer;
    uint8_t   *evt_queue;
  } MB_Mac_802_15_4_t;

  typedef struct
  {
    MB_DeviceInfoTable_t    *p_device_info_table;
    MB_BleTable_t           *p_ble_table;
    MB_ThreadTable_t        *p_thread_table;
    MB_SysTable_t           *p_sys_table;
    MB_MemManagerTable_t    *p_mem_manager_table;
    MB_TracesTable_t        *p_traces_table;
    MB_Mac_802_15_4_t       *p_mac_802_15_4_table;
    MB_ZigbeeTable_t        *p_zigbee_table;
    MB_LldTestsTable_t      *p_lld_tests_table;
} MB_RefTable_t;

#ifdef __cplusplus
}
#endif

/**
 *********************************************************************************
 * IPCC CHANNELS
 *********************************************************************************
 */

/*  CPU1                                             CPU2
 *   |             (SYSTEM)                            |
 *   |----HW_IPCC_SYSTEM_CMD_RSP_CHANNEL-------------->|
 *   |                                                 |
 *   |<---HW_IPCC_SYSTEM_EVENT_CHANNEL-----------------|
 *   |                                                 |
 *   |            (ZIGBEE)                             |
 *   |----HW_IPCC_ZIGBEE_CMD_APPLI_CHANNEL------------>|
 *   |                                                 |
 *   |----HW_IPCC_ZIGBEE_CMD_CLI_CHANNEL-------------->|
 *   |                                                 |
 *   |<---HW_IPCC_ZIGBEE_APPLI_NOTIF_ACK_CHANNEL-------|
 *   |                                                 |
 *   |<---HW_IPCC_ZIGBEE_CLI_NOTIF_ACK_CHANNEL---------|
 *   |                                                 |
 *   |             (THREAD)                            |
 *   |----HW_IPCC_THREAD_OT_CMD_RSP_CHANNEL----------->|
 *   |                                                 |
 *   |----HW_IPCC_THREAD_CLI_CMD_CHANNEL-------------->|
 *   |                                                 |
 *   |<---HW_IPCC_THREAD_NOTIFICATION_ACK_CHANNEL------|
 *   |                                                 |
 *   |<---HW_IPCC_THREAD_CLI_NOTIFICATION_ACK_CHANNEL--|
 *   |                                                 |
 *   |             (BLE)                               |
 *   |----HW_IPCC_BLE_CMD_CHANNEL--------------------->|
 *   |                                                 |
 *   |----HW_IPCC_HCI_ACL_DATA_CHANNEL---------------->|
 *   |                                                 |
 *   |<---HW_IPCC_BLE_EVENT_CHANNEL--------------------|
 *   |                                                 |
 *   |             (MAC)                               |
 *   |----HW_IPCC_MAC_802_15_4_CMD_RSP_CHANNEL-------->|
 *   |                                                 |
 *   |<---HW_IPCC_MAC_802_15_4_NOTIFICATION_ACK_CHANNEL|
 *   |                                                 |
 *   |             (BUFFER)                            |
 *   |----HW_IPCC_MM_RELEASE_BUFFER_CHANNE------------>|
 *   |                                                 |
 *   |             (TRACE)                             |
 *   |<----HW_IPCC_TRACES_CHANNEL----------------------|
 *   |                                                 |
 *
 *
 *
 */



/** CPU1 */
#define HW_IPCC_BLE_CMD_CHANNEL                         LL_IPCC_CHANNEL_1
#define HW_IPCC_SYSTEM_CMD_RSP_CHANNEL                  LL_IPCC_CHANNEL_2
#define HW_IPCC_THREAD_OT_CMD_RSP_CHANNEL               LL_IPCC_CHANNEL_3
#define HW_IPCC_ZIGBEE_CMD_APPLI_CHANNEL                LL_IPCC_CHANNEL_3
#define HW_IPCC_MAC_802_15_4_CMD_RSP_CHANNEL            LL_IPCC_CHANNEL_3
#define HW_IPCC_MM_RELEASE_BUFFER_CHANNEL               LL_IPCC_CHANNEL_4
#define HW_IPCC_THREAD_CLI_CMD_CHANNEL                  LL_IPCC_CHANNEL_5
#define HW_IPCC_LLDTESTS_CLI_CMD_CHANNEL                LL_IPCC_CHANNEL_5
#define HW_IPCC_HCI_ACL_DATA_CHANNEL                    LL_IPCC_CHANNEL_6

/** CPU2 */
#define HW_IPCC_BLE_EVENT_CHANNEL                       LL_IPCC_CHANNEL_1
#define HW_IPCC_SYSTEM_EVENT_CHANNEL                    LL_IPCC_CHANNEL_2
#define HW_IPCC_THREAD_NOTIFICATION_ACK_CHANNEL         LL_IPCC_CHANNEL_3
#define HW_IPCC_ZIGBEE_APPLI_NOTIF_ACK_CHANNEL          LL_IPCC_CHANNEL_3
#define HW_IPCC_MAC_802_15_4_NOTIFICATION_ACK_CHANNEL   LL_IPCC_CHANNEL_3
#define HW_IPCC_LLDTESTS_M0_CMD_CHANNEL                 LL_IPCC_CHANNEL_3
#define HW_IPCC_TRACES_CHANNEL                          LL_IPCC_CHANNEL_4
#define HW_IPCC_THREAD_CLI_NOTIFICATION_ACK_CHANNEL     LL_IPCC_CHANNEL_5
#define HW_IPCC_LLDTESTS_CLI_RSP_CHANNEL                LL_IPCC_CHANNEL_5
#define HW_IPCC_ZIGBEE_APPLI_LOGGING_CHANNEL            LL_IPCC_CHANNEL_5
#endif /*__MBOX_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
