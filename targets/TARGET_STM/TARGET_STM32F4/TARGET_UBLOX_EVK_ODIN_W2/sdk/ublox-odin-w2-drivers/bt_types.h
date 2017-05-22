/*---------------------------------------------------------------------------
 * Copyright (c) 2016, u-blox Malmö, All Rights Reserved
 * SPDX-License-Identifier: LicenseRef-PBL
 *
 * This file and the related binary are licensed under the
 * Permissive Binary License, Version 1.0 (the "License");
 * you may not use these files except in compliance with the License.
 *
 * You may obtain a copy of the License here:
 * LICENSE-permissive-binary-license-1.0.txt and at
 * https://www.mbed.com/licenses/PBL-1.0
 *
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Component   : 
 * File        : bt_types.h
 *
 * Description : Common Bluetooth types
 *-------------------------------------------------------------------------*/

/**
 * @file bt_types.h
 * @brief Common Bluetooth types
 */

#ifndef _BT_TYPES_H_
#define _BT_TYPES_H_

#include "cb_comdefs.h"

/*===========================================================================
 * DEFINES
 *=========================================================================*/

#define SIZE_OF_BD_ADDR                               (6)
#define SIZE_OF_COD                                   (3)
#define SIZE_OF_LINK_KEY                              (16)
#define SIZE_OF_NAME                                  (248)
#define SIZE_OF_PIN_CODE                              ((cb_uint8)16)
#define SIZE_OF_LAP                                   (3)
#define SIZE_OF_AFH_LMP_HCI_CHANNEL_MAP               (10)
#define CHANNEL_MAP_SIZE                              (5)
#define SIZE_OF_EXT_INQ_RSP                           (240)
#define MIN_PASSKEY_VALUE                             (0)
#define MAX_PASSKEY_VALUE                             (999999)
#define INVALID_CONN_HANDLE                           ((TConnHandle)0xFFFF)
#define MAX_ADV_DATA_LENGTH                           (31)
#define UUID_LENGTH                                   (16)


#define PACKET_TYPE_DM1                                                 (0x0008)
#define PACKET_TYPE_DH1                                                 (0x0010)
#define PACKET_TYPE_DM3                                                 (0x0400)
#define PACKET_TYPE_DH3                                                 (0x0800)
#define PACKET_TYPE_DM5                                                 (0x4000)
#define PACKET_TYPE_DH5                                                 (0x8000)

#define PACKET_TYPE_NO_2_DH1                                            (0x0002)
#define PACKET_TYPE_NO_3_DH1                                            (0x0004)
#define PACKET_TYPE_NO_2_DH3                                            (0x0100)
#define PACKET_TYPE_NO_3_DH3                                            (0x0200)
#define PACKET_TYPE_NO_2_DH5                                            (0x1000)
#define PACKET_TYPE_NO_3_DH5                                            (0x2000)

#define PACKET_TYPE_ALL (PACKET_TYPE_DM1 | PACKET_TYPE_DH1 | PACKET_TYPE_DM3 | PACKET_TYPE_DH3 | PACKET_TYPE_DM5 | PACKET_TYPE_DH5)

/*===========================================================================
 * TYPES
 *=========================================================================*/

typedef cb_int32        int32;
typedef cb_uint32       uint32;
typedef cb_boolean      boolean;
typedef cb_int8         int8;
typedef cb_uint8        uint8;
typedef cb_int16        int16;
typedef cb_uint16       uint16;

typedef cb_uint8       TErrorCode;
typedef cb_uint8       TLinkType;
typedef cb_uint16      TPacketType;
typedef cb_uint16      TConnHandle;

typedef enum
{
  BT_SECURITY_MODE_1 = 1,
  BT_SECURITY_MODE_2,
  BT_SECURITY_MODE_3,
  BT_SECURITY_MODE_4

} TSecurityMode;

typedef enum
{
  BT_SECURITY_LEVEL_0 = 0,
  BT_SECURITY_LEVEL_1,
  BT_SECURITY_LEVEL_2,
  BT_SECURITY_LEVEL_3,
  // Used with security modes 1,2,3 where security level is not applicable
  BT_SECURITY_LEVEL_DUMMY = 5,

} TSecurityLevel;


typedef enum 
{
  BT_MASTER_SLAVE_POLICY_ALWAYS_MASTER     = 0,
  BT_MASTER_SLAVE_POLICY_OTHER_SIDE_DECIDE = 1

} TMasterSlavePolicy;

typedef enum 
{
    BT_TYPE_CLASSIC    = 0,
    BT_TYPE_LOW_ENERGY = 1

} TBluetoothType;

typedef enum
{
    BT_PUBLIC_ADDRESS = 0x00,
    BT_RANDOM_ADDRESS = 0x01,

} TAddressType;

typedef struct
{
  cb_uint8      BdAddress[SIZE_OF_BD_ADDR];
  TAddressType  AddrType;

} TBdAddr;

typedef struct
{
  cb_uint8 Cod[SIZE_OF_COD];

} TCod;

typedef struct
{
  cb_uint8 LinkKey[SIZE_OF_LINK_KEY];

} TLinkKey;

typedef struct
{
  cb_uint8 Name[SIZE_OF_NAME];

} TName;

typedef struct
{
  cb_uint8 PinCode[SIZE_OF_PIN_CODE];

} TPinCode;

typedef cb_uint32 TPasskey;

typedef struct
{
  cb_uint8 Lap[SIZE_OF_LAP];

} TLap;

typedef struct
{
  cb_uint8 Data[SIZE_OF_EXT_INQ_RSP];

} TExtInqRsp;

typedef cb_uint8 TAfhLmpHciChannelMap[SIZE_OF_AFH_LMP_HCI_CHANNEL_MAP];

typedef struct
{
    uint16 channel[CHANNEL_MAP_SIZE];
} TChannelMap;


typedef enum
{
    BT_ADV_TYPE_ADV  = 0x01,
    BT_ADV_TYPE_SCAN = 0x00,
} TAdvDataType;

typedef struct 
{
    TAdvDataType type;
    cb_uint8 length;
    cb_uint8 data[MAX_ADV_DATA_LENGTH];
} TAdvData;

typedef struct
{
    cb_uint16 createConnectionTimeout;
    cb_uint16 connectionIntervalMin;
    cb_uint16 connectionIntervalMax;
    cb_uint16 connectionLatency;
    cb_uint16 linkLossTimeout;
    cb_uint16  scanInterval;
    cb_uint16  scanWindow;
} TAclParamsLe;

#endif /* _BT_TYPES_H */
