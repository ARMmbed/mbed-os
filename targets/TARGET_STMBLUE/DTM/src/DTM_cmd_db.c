/**
  ******************************************************************************
  * @file    DTM_cmd_db.c
  * @author  Antonio O.
  * @date    21 Dec. 2018
  ******************************************************************************
  */
#include "compiler.h"
#include "bluenrg1_events.h"
#include "osal.h"
#include "miscutil.h"
#include "DTM_cmd_db.h"
#include "bluenrg1_stack.h"

#include <stdint.h>

/** Documentation for C struct Whitelist_Entry_t */
typedef PACKED(struct) packed_Whitelist_Entry_t_s {
  /** Address type.
  * Values:
  - 0x00: Public Device Address
  - 0x01: Random Device Address
  */
  uint8_t Peer_Address_Type;
  /** Public Device Address or Random Device Address of the device
to be added to the white list.
  */
  uint8_t Peer_Address[6];
} packed_Whitelist_Entry_t;
/** Documentation for C struct Bonded_Device_Entry_t */
typedef PACKED(struct) packed_Bonded_Device_Entry_t_s {
  /** Address type.
  * Values:
  - 0x00: Public Device Address
  - 0x01: Random Device Address
  */
  uint8_t Address_Type;
  /** Public Device Address or Random Device Address of the device
to be added to the white list.
  */
  uint8_t Address[6];
} packed_Bonded_Device_Entry_t;
/** Documentation for C struct Whitelist_Identity_Entry_t */
typedef PACKED(struct) packed_Whitelist_Identity_Entry_t_s {
  /** Identity address type.
  * Values:
  - 0x00: Public Identity Address
  - 0x01: Random (static) Identity Address
  */
  uint8_t Peer_Identity_Address_Type;
  /** Public or Random (static) Identity address of the peer device
  */
  uint8_t Peer_Identity_Address[6];
} packed_Whitelist_Identity_Entry_t;
/** Documentation for C union Service_UUID_t */
typedef PACKED(union) packed_Service_UUID_t_s {
  /** 16-bit UUID
  */
  uint16_t Service_UUID_16;
  /** 128-bit UUID
  */
  uint8_t Service_UUID_128[16];
} packed_Service_UUID_t;
/** Documentation for C union Include_UUID_t */
typedef PACKED(union) packed_Include_UUID_t_s {
  /** 16-bit UUID
  */
  uint16_t Include_UUID_16;
  /** 128-bit UUID
  */
  uint8_t Include_UUID_128[16];
} packed_Include_UUID_t;
/** Documentation for C union Char_UUID_t */
typedef PACKED(union) packed_Char_UUID_t_s {
  /** 16-bit UUID
  */
  uint16_t Char_UUID_16;
  /** 128-bit UUID
  */
  uint8_t Char_UUID_128[16];
} packed_Char_UUID_t;
/** Documentation for C union Char_Desc_Uuid_t */
typedef PACKED(union) packed_Char_Desc_Uuid_t_s {
  /** 16-bit UUID
  */
  uint16_t Char_UUID_16;
  /** 128-bit UUID
  */
  uint8_t Char_UUID_128[16];
} packed_Char_Desc_Uuid_t;
/** Documentation for C union UUID_t */
typedef PACKED(union) packed_UUID_t_s {
  /** 16-bit UUID
  */
  uint16_t UUID_16;
  /** 128-bit UUID
  */
  uint8_t UUID_128[16];
} packed_UUID_t;
/** Documentation for C struct Handle_Entry_t */
typedef PACKED(struct) packed_Handle_Entry_t_s {
  /** The handles for which the attribute value has to be read
  */
  uint16_t Handle;
} packed_Handle_Entry_t;
/** Documentation for C struct Handle_Packets_Pair_Entry_t */
typedef PACKED(struct) packed_Handle_Packets_Pair_Entry_t_s {
  /** Connection handle
  */
  uint16_t Connection_Handle;
  /** The number of HCI Data Packets that have been completed (transmitted
or flushed) for the associated Connection_Handle since the previous time
the event was returned.
  */
  uint16_t HC_Num_Of_Completed_Packets;
} packed_Handle_Packets_Pair_Entry_t;
/** Documentation for C struct Attribute_Group_Handle_Pair_t */
typedef PACKED(struct) packed_Attribute_Group_Handle_Pair_t_s {
  /** Found Attribute handle
  */
  uint16_t Found_Attribute_Handle;
  /** Group End handle
  */
  uint16_t Group_End_Handle;
} packed_Attribute_Group_Handle_Pair_t;
/** Documentation for C struct Handle_Item_t */
typedef PACKED(struct) packed_Handle_Item_t_s {
  /**
  */
  uint16_t Handle;
} packed_Handle_Item_t;
/** Documentation for C struct Advertising_Report_t */
typedef PACKED(struct) packed_Advertising_Report_t_s {
  /** Type of advertising report event:
ADV_IND: Connectable undirected advertising',
ADV_DIRECT_IND: Connectable directed advertising,
ADV_SCAN_IND: Scannable undirected advertising,
ADV_NONCONN_IND: Non connectable undirected advertising,
SCAN_RSP: Scan response.
  * Values:
  - 0x00: ADV_IND
  - 0x01: ADV_DIRECT_IND
  - 0x02: ADV_SCAN_IND
  - 0x03: ADV_NONCONN_IND
  - 0x04: SCAN_RSP
  */
  uint8_t Event_Type;
  /** 0x00 Public Device Address
0x01 Random Device Address
0x02 Public Identity Address (Corresponds to Resolved Private Address)
0x03 Random (Static) Identity Address (Corresponds to Resolved Private Address)
  * Values:
  - 0x00: Public Device Address
  - 0x01: Random Device Address
  - 0x02: Public Identity Address
  - 0x03: Random (Static) Identity Address
  */
  uint8_t Address_Type;
  /** Public Device Address, Random Device Address, Public Identity
Address or Random (static) Identity Address of the advertising
device.
  */
  uint8_t Address[6];
  /** Length of the Data[i] field for each device which responded.
  * Values:
  - 0 ... 31
  */
  uint8_t Length_Data;
  /** Length_Data[i] octets of advertising or scan response data formatted
as defined in [Vol 3] Part C, Section 8.
  */
  uint8_t Data[1];
  /** N Size: 1 Octet (signed integer)
Units: dBm
  * Values:
  - 127: RSSI not available
  - -127 ... 20
  */
  uint8_t RSSI;
} packed_Advertising_Report_t;
/** Documentation for C struct Direct_Advertising_Report_t */
typedef PACKED(struct) packed_Direct_Advertising_Report_t_s {
  /** Advertising type
  * Values:
  - 0x01: Connectable directed advertising (ADV_DIRECT_IND)
  */
  uint8_t Event_Type;
  /** 0x00 Public Device Address
0x01 Random Device Address
0x02 Public Identity Address (Corresponds to Resolved Private Address)
0x03 Random (Static) Identity Address (Corresponds to Resolved Private Address)
  * Values:
  - 0x00: Public Device Address
  - 0x01: Random Device Address
  - 0x02: Public Identity Address
  - 0x03: Random (Static) Identity Address
  */
  uint8_t Address_Type;
  /** Public Device Address, Random Device Address, Public Identity
Address or Random (static) Identity Address of the advertising device.
  */
  uint8_t Address[6];
  /** 0x01 Random Device Address
  * Values:
  - 0x01: Random Device Address
  */
  uint8_t Direct_Address_Type;
  /** Random Device Address
  */
  uint8_t Direct_Address[6];
  /** N Size: 1 Octet (signed integer)
Units: dBm
  * Values:
  - 127: RSSI not available
  - -127 ... 20
  */
  uint8_t RSSI;
} packed_Direct_Advertising_Report_t;
typedef PACKED(struct) hci_disconnect_cp0_s {
  uint16_t Connection_Handle;
  uint8_t Reason;
} hci_disconnect_cp0;

typedef PACKED(struct) hci_disconnect_rp0_s {
  uint8_t Status;
} hci_disconnect_rp0;

typedef PACKED(struct) hci_read_remote_version_information_cp0_s {
  uint16_t Connection_Handle;
} hci_read_remote_version_information_cp0;

typedef PACKED(struct) hci_read_remote_version_information_rp0_s {
  uint8_t Status;
} hci_read_remote_version_information_rp0;

typedef PACKED(struct) hci_set_event_mask_cp0_s {
  uint8_t Event_Mask[8];
} hci_set_event_mask_cp0;

typedef PACKED(struct) hci_set_event_mask_rp0_s {
  uint8_t Status;
} hci_set_event_mask_rp0;

typedef PACKED(struct) hci_reset_rp0_s {
  uint8_t Status;
} hci_reset_rp0;

typedef PACKED(struct) hci_read_transmit_power_level_cp0_s {
  uint16_t Connection_Handle;
  uint8_t Type;
} hci_read_transmit_power_level_cp0;

typedef PACKED(struct) hci_read_transmit_power_level_rp0_s {
  uint8_t Status;
  uint16_t Connection_Handle;
  uint8_t Transmit_Power_Level;
} hci_read_transmit_power_level_rp0;

typedef PACKED(struct) hci_read_local_version_information_rp0_s {
  uint8_t Status;
  uint8_t HCI_Version;
  uint16_t HCI_Revision;
  uint8_t LMP_PAL_Version;
  uint16_t Manufacturer_Name;
  uint16_t LMP_PAL_Subversion;
} hci_read_local_version_information_rp0;

typedef PACKED(struct) hci_read_local_supported_commands_rp0_s {
  uint8_t Status;
  uint8_t Supported_Commands[64];
} hci_read_local_supported_commands_rp0;

typedef PACKED(struct) hci_read_local_supported_features_rp0_s {
  uint8_t Status;
  uint8_t LMP_Features[8];
} hci_read_local_supported_features_rp0;

typedef PACKED(struct) hci_read_bd_addr_rp0_s {
  uint8_t Status;
  uint8_t BD_ADDR[6];
} hci_read_bd_addr_rp0;

typedef PACKED(struct) hci_read_rssi_cp0_s {
  uint16_t Connection_Handle;
} hci_read_rssi_cp0;

typedef PACKED(struct) hci_read_rssi_rp0_s {
  uint8_t Status;
  uint16_t Connection_Handle;
  uint8_t RSSI;
} hci_read_rssi_rp0;

typedef PACKED(struct) hci_le_set_event_mask_cp0_s {
  uint8_t LE_Event_Mask[8];
} hci_le_set_event_mask_cp0;

typedef PACKED(struct) hci_le_set_event_mask_rp0_s {
  uint8_t Status;
} hci_le_set_event_mask_rp0;

typedef PACKED(struct) hci_le_read_buffer_size_rp0_s {
  uint8_t Status;
  uint16_t HC_LE_ACL_Data_Packet_Length;
  uint8_t HC_Total_Num_LE_ACL_Data_Packets;
} hci_le_read_buffer_size_rp0;

typedef PACKED(struct) hci_le_read_local_supported_features_rp0_s {
  uint8_t Status;
  uint8_t LE_Features[8];
} hci_le_read_local_supported_features_rp0;

typedef PACKED(struct) hci_le_set_random_address_cp0_s {
  uint8_t Random_Address[6];
} hci_le_set_random_address_cp0;

typedef PACKED(struct) hci_le_set_random_address_rp0_s {
  uint8_t Status;
} hci_le_set_random_address_rp0;

typedef PACKED(struct) hci_le_set_advertising_parameters_cp0_s {
  uint16_t Advertising_Interval_Min;
  uint16_t Advertising_Interval_Max;
  uint8_t Advertising_Type;
  uint8_t Own_Address_Type;
  uint8_t Peer_Address_Type;
  uint8_t Peer_Address[6];
  uint8_t Advertising_Channel_Map;
  uint8_t Advertising_Filter_Policy;
} hci_le_set_advertising_parameters_cp0;

typedef PACKED(struct) hci_le_set_advertising_parameters_rp0_s {
  uint8_t Status;
} hci_le_set_advertising_parameters_rp0;

typedef PACKED(struct) hci_le_read_advertising_channel_tx_power_rp0_s {
  uint8_t Status;
  uint8_t Transmit_Power_Level;
} hci_le_read_advertising_channel_tx_power_rp0;

typedef PACKED(struct) hci_le_set_advertising_data_cp0_s {
  uint8_t Advertising_Data_Length;
  uint8_t Advertising_Data[31];
} hci_le_set_advertising_data_cp0;

typedef PACKED(struct) hci_le_set_advertising_data_rp0_s {
  uint8_t Status;
} hci_le_set_advertising_data_rp0;

typedef PACKED(struct) hci_le_set_scan_response_data_cp0_s {
  uint8_t Scan_Response_Data_Length;
  uint8_t Scan_Response_Data[31];
} hci_le_set_scan_response_data_cp0;

typedef PACKED(struct) hci_le_set_scan_response_data_rp0_s {
  uint8_t Status;
} hci_le_set_scan_response_data_rp0;

typedef PACKED(struct) hci_le_set_advertise_enable_cp0_s {
  uint8_t Advertising_Enable;
} hci_le_set_advertise_enable_cp0;

typedef PACKED(struct) hci_le_set_advertise_enable_rp0_s {
  uint8_t Status;
} hci_le_set_advertise_enable_rp0;

typedef PACKED(struct) hci_le_set_scan_parameters_cp0_s {
  uint8_t LE_Scan_Type;
  uint16_t LE_Scan_Interval;
  uint16_t LE_Scan_Window;
  uint8_t Own_Address_Type;
  uint8_t Scanning_Filter_Policy;
} hci_le_set_scan_parameters_cp0;

typedef PACKED(struct) hci_le_set_scan_parameters_rp0_s {
  uint8_t Status;
} hci_le_set_scan_parameters_rp0;

typedef PACKED(struct) hci_le_set_scan_enable_cp0_s {
  uint8_t LE_Scan_Enable;
  uint8_t Filter_Duplicates;
} hci_le_set_scan_enable_cp0;

typedef PACKED(struct) hci_le_set_scan_enable_rp0_s {
  uint8_t Status;
} hci_le_set_scan_enable_rp0;

typedef PACKED(struct) hci_le_create_connection_cp0_s {
  uint16_t LE_Scan_Interval;
  uint16_t LE_Scan_Window;
  uint8_t Initiator_Filter_Policy;
  uint8_t Peer_Address_Type;
  uint8_t Peer_Address[6];
  uint8_t Own_Address_Type;
  uint16_t Conn_Interval_Min;
  uint16_t Conn_Interval_Max;
  uint16_t Conn_Latency;
  uint16_t Supervision_Timeout;
  uint16_t Minimum_CE_Length;
  uint16_t Maximum_CE_Length;
} hci_le_create_connection_cp0;

typedef PACKED(struct) hci_le_create_connection_rp0_s {
  uint8_t Status;
} hci_le_create_connection_rp0;

typedef PACKED(struct) hci_le_create_connection_cancel_rp0_s {
  uint8_t Status;
} hci_le_create_connection_cancel_rp0;

typedef PACKED(struct) hci_le_read_white_list_size_rp0_s {
  uint8_t Status;
  uint8_t White_List_Size;
} hci_le_read_white_list_size_rp0;

typedef PACKED(struct) hci_le_clear_white_list_rp0_s {
  uint8_t Status;
} hci_le_clear_white_list_rp0;

typedef PACKED(struct) hci_le_add_device_to_white_list_cp0_s {
  uint8_t Address_Type;
  uint8_t Address[6];
} hci_le_add_device_to_white_list_cp0;

typedef PACKED(struct) hci_le_add_device_to_white_list_rp0_s {
  uint8_t Status;
} hci_le_add_device_to_white_list_rp0;

typedef PACKED(struct) hci_le_remove_device_from_white_list_cp0_s {
  uint8_t Address_Type;
  uint8_t Address[6];
} hci_le_remove_device_from_white_list_cp0;

typedef PACKED(struct) hci_le_remove_device_from_white_list_rp0_s {
  uint8_t Status;
} hci_le_remove_device_from_white_list_rp0;

typedef PACKED(struct) hci_le_connection_update_cp0_s {
  uint16_t Connection_Handle;
  uint16_t Conn_Interval_Min;
  uint16_t Conn_Interval_Max;
  uint16_t Conn_Latency;
  uint16_t Supervision_Timeout;
  uint16_t Minimum_CE_Length;
  uint16_t Maximum_CE_Length;
} hci_le_connection_update_cp0;

typedef PACKED(struct) hci_le_connection_update_rp0_s {
  uint8_t Status;
} hci_le_connection_update_rp0;

typedef PACKED(struct) hci_le_set_host_channel_classification_cp0_s {
  uint8_t LE_Channel_Map[5];
} hci_le_set_host_channel_classification_cp0;

typedef PACKED(struct) hci_le_set_host_channel_classification_rp0_s {
  uint8_t Status;
} hci_le_set_host_channel_classification_rp0;

typedef PACKED(struct) hci_le_read_channel_map_cp0_s {
  uint16_t Connection_Handle;
} hci_le_read_channel_map_cp0;

typedef PACKED(struct) hci_le_read_channel_map_rp0_s {
  uint8_t Status;
  uint16_t Connection_Handle;
  uint8_t LE_Channel_Map[5];
} hci_le_read_channel_map_rp0;

typedef PACKED(struct) hci_le_read_remote_used_features_cp0_s {
  uint16_t Connection_Handle;
} hci_le_read_remote_used_features_cp0;

typedef PACKED(struct) hci_le_read_remote_used_features_rp0_s {
  uint8_t Status;
} hci_le_read_remote_used_features_rp0;

typedef PACKED(struct) hci_le_encrypt_cp0_s {
  uint8_t Key[16];
  uint8_t Plaintext_Data[16];
} hci_le_encrypt_cp0;

typedef PACKED(struct) hci_le_encrypt_rp0_s {
  uint8_t Status;
  uint8_t Encrypted_Data[16];
} hci_le_encrypt_rp0;

typedef PACKED(struct) hci_le_rand_rp0_s {
  uint8_t Status;
  uint8_t Random_Number[8];
} hci_le_rand_rp0;

typedef PACKED(struct) hci_le_start_encryption_cp0_s {
  uint16_t Connection_Handle;
  uint8_t Random_Number[8];
  uint16_t Encrypted_Diversifier;
  uint8_t Long_Term_Key[16];
} hci_le_start_encryption_cp0;

typedef PACKED(struct) hci_le_start_encryption_rp0_s {
  uint8_t Status;
} hci_le_start_encryption_rp0;

typedef PACKED(struct) hci_le_long_term_key_request_reply_cp0_s {
  uint16_t Connection_Handle;
  uint8_t Long_Term_Key[16];
} hci_le_long_term_key_request_reply_cp0;

typedef PACKED(struct) hci_le_long_term_key_request_reply_rp0_s {
  uint8_t Status;
  uint16_t Connection_Handle;
} hci_le_long_term_key_request_reply_rp0;

typedef PACKED(struct) hci_le_long_term_key_requested_negative_reply_cp0_s {
  uint16_t Connection_Handle;
} hci_le_long_term_key_requested_negative_reply_cp0;

typedef PACKED(struct) hci_le_long_term_key_requested_negative_reply_rp0_s {
  uint8_t Status;
  uint16_t Connection_Handle;
} hci_le_long_term_key_requested_negative_reply_rp0;

typedef PACKED(struct) hci_le_read_supported_states_rp0_s {
  uint8_t Status;
  uint8_t LE_States[8];
} hci_le_read_supported_states_rp0;

typedef PACKED(struct) hci_le_receiver_test_cp0_s {
  uint8_t RX_Frequency;
} hci_le_receiver_test_cp0;

typedef PACKED(struct) hci_le_receiver_test_rp0_s {
  uint8_t Status;
} hci_le_receiver_test_rp0;

typedef PACKED(struct) hci_le_transmitter_test_cp0_s {
  uint8_t TX_Frequency;
  uint8_t Length_Of_Test_Data;
  uint8_t Packet_Payload;
} hci_le_transmitter_test_cp0;

typedef PACKED(struct) hci_le_transmitter_test_rp0_s {
  uint8_t Status;
} hci_le_transmitter_test_rp0;

typedef PACKED(struct) hci_le_test_end_rp0_s {
  uint8_t Status;
  uint16_t Number_Of_Packets;
} hci_le_test_end_rp0;

typedef PACKED(struct) hci_le_set_data_length_cp0_s {
  uint16_t Connection_Handle;
  uint16_t TxOctets;
  uint16_t TxTime;
} hci_le_set_data_length_cp0;

typedef PACKED(struct) hci_le_set_data_length_rp0_s {
  uint8_t Status;
  uint16_t Connection_Handle;
} hci_le_set_data_length_rp0;

typedef PACKED(struct) hci_le_read_suggested_default_data_length_rp0_s {
  uint8_t Status;
  uint16_t SuggestedMaxTxOctets;
  uint16_t SuggestedMaxTxTime;
} hci_le_read_suggested_default_data_length_rp0;

typedef PACKED(struct) hci_le_write_suggested_default_data_length_cp0_s {
  uint16_t SuggestedMaxTxOctets;
  uint16_t SuggestedMaxTxTime;
} hci_le_write_suggested_default_data_length_cp0;

typedef PACKED(struct) hci_le_write_suggested_default_data_length_rp0_s {
  uint8_t Status;
} hci_le_write_suggested_default_data_length_rp0;

typedef PACKED(struct) hci_le_read_local_p256_public_key_rp0_s {
  uint8_t Status;
} hci_le_read_local_p256_public_key_rp0;

typedef PACKED(struct) hci_le_generate_dhkey_cp0_s {
  uint8_t Remote_P256_Public_Key[64];
} hci_le_generate_dhkey_cp0;

typedef PACKED(struct) hci_le_generate_dhkey_rp0_s {
  uint8_t Status;
} hci_le_generate_dhkey_rp0;

typedef PACKED(struct) hci_le_add_device_to_resolving_list_cp0_s {
  uint8_t Peer_Identity_Address_Type;
  uint8_t Peer_Identity_Address[6];
  uint8_t Peer_IRK[16];
  uint8_t Local_IRK[16];
} hci_le_add_device_to_resolving_list_cp0;

typedef PACKED(struct) hci_le_add_device_to_resolving_list_rp0_s {
  uint8_t Status;
} hci_le_add_device_to_resolving_list_rp0;

typedef PACKED(struct) hci_le_remove_device_from_resolving_list_cp0_s {
  uint8_t Peer_Identity_Address_Type;
  uint8_t Peer_Identity_Address[6];
} hci_le_remove_device_from_resolving_list_cp0;

typedef PACKED(struct) hci_le_remove_device_from_resolving_list_rp0_s {
  uint8_t Status;
} hci_le_remove_device_from_resolving_list_rp0;

typedef PACKED(struct) hci_le_clear_resolving_list_rp0_s {
  uint8_t Status;
} hci_le_clear_resolving_list_rp0;

typedef PACKED(struct) hci_le_read_resolving_list_size_rp0_s {
  uint8_t Status;
  uint8_t Resolving_List_Size;
} hci_le_read_resolving_list_size_rp0;

typedef PACKED(struct) hci_le_read_peer_resolvable_address_cp0_s {
  uint8_t Peer_Identity_Address_Type;
  uint8_t Peer_Identity_Address[6];
} hci_le_read_peer_resolvable_address_cp0;

typedef PACKED(struct) hci_le_read_peer_resolvable_address_rp0_s {
  uint8_t Status;
  uint8_t Peer_Resolvable_Address[6];
} hci_le_read_peer_resolvable_address_rp0;

typedef PACKED(struct) hci_le_read_local_resolvable_address_cp0_s {
  uint8_t Peer_Identity_Address_Type;
  uint8_t Peer_Identity_Address[6];
} hci_le_read_local_resolvable_address_cp0;

typedef PACKED(struct) hci_le_read_local_resolvable_address_rp0_s {
  uint8_t Status;
  uint8_t Local_Resolvable_Address[6];
} hci_le_read_local_resolvable_address_rp0;

typedef PACKED(struct) hci_le_set_address_resolution_enable_cp0_s {
  uint8_t Address_Resolution_Enable;
} hci_le_set_address_resolution_enable_cp0;

typedef PACKED(struct) hci_le_set_address_resolution_enable_rp0_s {
  uint8_t Status;
} hci_le_set_address_resolution_enable_rp0;

typedef PACKED(struct) hci_le_set_resolvable_private_address_timeout_cp0_s {
  uint16_t RPA_Timeout;
} hci_le_set_resolvable_private_address_timeout_cp0;

typedef PACKED(struct) hci_le_set_resolvable_private_address_timeout_rp0_s {
  uint8_t Status;
} hci_le_set_resolvable_private_address_timeout_rp0;

typedef PACKED(struct) hci_le_read_maximum_data_length_rp0_s {
  uint8_t Status;
  uint16_t supportedMaxTxOctets;
  uint16_t supportedMaxTxTime;
  uint16_t supportedMaxRxOctets;
  uint16_t supportedMaxRxTime;
} hci_le_read_maximum_data_length_rp0;

typedef PACKED(struct) hci_disconnection_complete_event_rp0_s {
  uint8_t Status;
  uint16_t Connection_Handle;
  uint8_t Reason;
} hci_disconnection_complete_event_rp0;

typedef PACKED(struct) hci_encryption_change_event_rp0_s {
  uint8_t Status;
  uint16_t Connection_Handle;
  uint8_t Encryption_Enabled;
} hci_encryption_change_event_rp0;

typedef PACKED(struct) hci_read_remote_version_information_complete_event_rp0_s {
  uint8_t Status;
  uint16_t Connection_Handle;
  uint8_t Version;
  uint16_t Manufacturer_Name;
  uint16_t Subversion;
} hci_read_remote_version_information_complete_event_rp0;

typedef PACKED(struct) hci_hardware_error_event_rp0_s {
  uint8_t Hardware_Code;
} hci_hardware_error_event_rp0;

typedef PACKED(struct) hci_number_of_completed_packets_event_rp0_s {
  uint8_t Number_of_Handles;
  packed_Handle_Packets_Pair_Entry_t Handle_Packets_Pair_Entry[(HCI_MAX_PAYLOAD_SIZE - 1)/sizeof(packed_Handle_Packets_Pair_Entry_t)];
} hci_number_of_completed_packets_event_rp0;

typedef PACKED(struct) hci_data_buffer_overflow_event_rp0_s {
  uint8_t Link_Type;
} hci_data_buffer_overflow_event_rp0;

typedef PACKED(struct) hci_encryption_key_refresh_complete_event_rp0_s {
  uint8_t Status;
  uint16_t Connection_Handle;
} hci_encryption_key_refresh_complete_event_rp0;

typedef PACKED(struct) hci_le_connection_complete_event_rp0_s {
  uint8_t Status;
  uint16_t Connection_Handle;
  uint8_t Role;
  uint8_t Peer_Address_Type;
  uint8_t Peer_Address[6];
  uint16_t Conn_Interval;
  uint16_t Conn_Latency;
  uint16_t Supervision_Timeout;
  uint8_t Master_Clock_Accuracy;
} hci_le_connection_complete_event_rp0;

typedef PACKED(struct) hci_le_advertising_report_event_rp0_s {
  uint8_t Num_Reports;
  packed_Advertising_Report_t Advertising_Report[(HCI_MAX_PAYLOAD_SIZE - 1)/sizeof(packed_Advertising_Report_t)];
} hci_le_advertising_report_event_rp0;

typedef PACKED(struct) hci_le_connection_update_complete_event_rp0_s {
  uint8_t Status;
  uint16_t Connection_Handle;
  uint16_t Conn_Interval;
  uint16_t Conn_Latency;
  uint16_t Supervision_Timeout;
} hci_le_connection_update_complete_event_rp0;

typedef PACKED(struct) hci_le_read_remote_used_features_complete_event_rp0_s {
  uint8_t Status;
  uint16_t Connection_Handle;
  uint8_t LE_Features[8];
} hci_le_read_remote_used_features_complete_event_rp0;

typedef PACKED(struct) hci_le_long_term_key_request_event_rp0_s {
  uint16_t Connection_Handle;
  uint8_t Random_Number[8];
  uint16_t Encrypted_Diversifier;
} hci_le_long_term_key_request_event_rp0;

typedef PACKED(struct) hci_le_data_length_change_event_rp0_s {
  uint16_t Connection_Handle;
  uint16_t MaxTxOctets;
  uint16_t MaxTxTime;
  uint16_t MaxRxOctets;
  uint16_t MaxRxTime;
} hci_le_data_length_change_event_rp0;

typedef PACKED(struct) hci_le_read_local_p256_public_key_complete_event_rp0_s {
  uint8_t Status;
  uint8_t Local_P256_Public_Key[64];
} hci_le_read_local_p256_public_key_complete_event_rp0;

typedef PACKED(struct) hci_le_generate_dhkey_complete_event_rp0_s {
  uint8_t Status;
  uint8_t DHKey[32];
} hci_le_generate_dhkey_complete_event_rp0;

typedef PACKED(struct) hci_le_enhanced_connection_complete_event_rp0_s {
  uint8_t Status;
  uint16_t Connection_Handle;
  uint8_t Role;
  uint8_t Peer_Address_Type;
  uint8_t Peer_Address[6];
  uint8_t Local_Resolvable_Private_Address[6];
  uint8_t Peer_Resolvable_Private_Address[6];
  uint16_t Conn_Interval;
  uint16_t Conn_Latency;
  uint16_t Supervision_Timeout;
  uint8_t Master_Clock_Accuracy;
} hci_le_enhanced_connection_complete_event_rp0;

typedef PACKED(struct) hci_le_direct_advertising_report_event_rp0_s {
  uint8_t Num_Reports;
  packed_Direct_Advertising_Report_t Direct_Advertising_Report[(HCI_MAX_PAYLOAD_SIZE - 1)/sizeof(packed_Direct_Advertising_Report_t)];
} hci_le_direct_advertising_report_event_rp0;

typedef PACKED(struct) aci_hal_write_config_data_cp0_s {
  uint8_t Offset;
  uint8_t Length;
  uint8_t Value[(HCI_MAX_PAYLOAD_SIZE - 2)/sizeof(uint8_t)];
} aci_hal_write_config_data_cp0;

typedef PACKED(struct) aci_hal_write_config_data_rp0_s {
  uint8_t Status;
} aci_hal_write_config_data_rp0;

typedef PACKED(struct) aci_hal_read_config_data_cp0_s {
  uint8_t Offset;
} aci_hal_read_config_data_cp0;

typedef PACKED(struct) aci_hal_read_config_data_rp0_s {
  uint8_t Status;
  uint8_t Data_Length;
  uint8_t Data[(HCI_MAX_PAYLOAD_SIZE - 2)/sizeof(uint8_t)];
} aci_hal_read_config_data_rp0;

const hci_command_table_type hci_command_table[57] = {
  /* wrapper at first position since it is called repeatedly */
  {0xfcff, hci_wrap_BTLE_StackTick_process},
  /* hci_disconnect */
  {0x0406, hci_disconnect_process},
  /* hci_read_remote_version_information */
  {0x041d, hci_read_remote_version_information_process},
  /* hci_set_event_mask */
  {0x0c01, hci_set_event_mask_process},
  /* hci_reset */
  {0x0c03, hci_reset_process},
  /* hci_read_transmit_power_level */
  {0x0c2d, hci_read_transmit_power_level_process},
  /* hci_read_local_version_information */
  {0x1001, hci_read_local_version_information_process},
  /* hci_read_local_supported_commands */
  {0x1002, hci_read_local_supported_commands_process},
  /* hci_read_local_supported_features */
  {0x1003, hci_read_local_supported_features_process},
  /* hci_read_bd_addr */
  {0x1009, hci_read_bd_addr_process},
  /* hci_read_rssi */
  {0x1405, hci_read_rssi_process},
  /* hci_le_set_event_mask */
  {0x2001, hci_le_set_event_mask_process},
  /* hci_le_read_buffer_size */
  {0x2002, hci_le_read_buffer_size_process},
  /* hci_le_read_local_supported_features */
  {0x2003, hci_le_read_local_supported_features_process},
  /* hci_le_set_random_address */
  {0x2005, hci_le_set_random_address_process},
  /* hci_le_set_advertising_parameters */
  {0x2006, hci_le_set_advertising_parameters_process},
  /* hci_le_read_advertising_channel_tx_power */
  {0x2007, hci_le_read_advertising_channel_tx_power_process},
  /* hci_le_set_advertising_data */
  {0x2008, hci_le_set_advertising_data_process},
  /* hci_le_set_scan_response_data */
  {0x2009, hci_le_set_scan_response_data_process},
  /* hci_le_set_advertise_enable */
  {0x200a, hci_le_set_advertise_enable_process},
  /* hci_le_set_scan_parameters */
  {0x200b, hci_le_set_scan_parameters_process},
  /* hci_le_set_scan_enable */
  {0x200c, hci_le_set_scan_enable_process},
  /* hci_le_create_connection */
  {0x200d, hci_le_create_connection_process},
  /* hci_le_create_connection_cancel */
  {0x200e, hci_le_create_connection_cancel_process},
  /* hci_le_read_white_list_size */
  {0x200f, hci_le_read_white_list_size_process},
  /* hci_le_clear_white_list */
  {0x2010, hci_le_clear_white_list_process},
  /* hci_le_add_device_to_white_list */
  {0x2011, hci_le_add_device_to_white_list_process},
  /* hci_le_remove_device_from_white_list */
  {0x2012, hci_le_remove_device_from_white_list_process},
  /* hci_le_connection_update */
  {0x2013, hci_le_connection_update_process},
  /* hci_le_set_host_channel_classification */
  {0x2014, hci_le_set_host_channel_classification_process},
  /* hci_le_read_channel_map */
  {0x2015, hci_le_read_channel_map_process},
  /* hci_le_read_remote_used_features */
  {0x2016, hci_le_read_remote_used_features_process},
  /* hci_le_encrypt */
  {0x2017, hci_le_encrypt_process},
  /* hci_le_rand */
  {0x2018, hci_le_rand_process},
  /* hci_le_start_encryption */
  {0x2019, hci_le_start_encryption_process},
  /* hci_le_long_term_key_request_reply */
  {0x201a, hci_le_long_term_key_request_reply_process},
  /* hci_le_long_term_key_requested_negative_reply */
  {0x201b, hci_le_long_term_key_requested_negative_reply_process},
  /* hci_le_read_supported_states */
  {0x201c, hci_le_read_supported_states_process},
  /* hci_le_receiver_test */
  {0x201d, hci_le_receiver_test_process},
  /* hci_le_transmitter_test */
  {0x201e, hci_le_transmitter_test_process},
  /* hci_le_test_end */
  {0x201f, hci_le_test_end_process},
  /* hci_le_set_data_length */
  {0x2022, hci_le_set_data_length_process},
  /* hci_le_read_suggested_default_data_length */
  {0x2023, hci_le_read_suggested_default_data_length_process},
  /* hci_le_write_suggested_default_data_length */
  {0x2024, hci_le_write_suggested_default_data_length_process},
  /* hci_le_read_local_p256_public_key */
  {0x2025, hci_le_read_local_p256_public_key_process},
  /* hci_le_generate_dhkey */
  {0x2026, hci_le_generate_dhkey_process},
  /* hci_le_add_device_to_resolving_list */
  {0x2027, hci_le_add_device_to_resolving_list_process},
  /* hci_le_remove_device_from_resolving_list */
  {0x2028, hci_le_remove_device_from_resolving_list_process},
  /* hci_le_clear_resolving_list */
  {0x2029, hci_le_clear_resolving_list_process},
  /* hci_le_read_resolving_list_size */
  {0x202a, hci_le_read_resolving_list_size_process},
  /* hci_le_read_peer_resolvable_address */
  {0x202b, hci_le_read_peer_resolvable_address_process},
  /* hci_le_read_local_resolvable_address */
  {0x202c, hci_le_read_local_resolvable_address_process},
  /* hci_le_set_address_resolution_enable */
  {0x202d, hci_le_set_address_resolution_enable_process},
  /* hci_le_set_resolvable_private_address_timeout */
  {0x202e, hci_le_set_resolvable_private_address_timeout_process},
  /* hci_le_read_maximum_data_length */
  {0x202f, hci_le_read_maximum_data_length_process},
  /* aci_hal_write_config_data */
  {0xfc0c, aci_hal_write_config_data_process},
  /* aci_hal_read_config_data */
  {0xfc0d, aci_hal_read_config_data_process}
};

/* tBleStatus hci_disconnect(uint16_t Connection_Handle,
                          uint8_t Reason);
 */
/* Command len: 2 + 1 */
/* Response len: 1 */
uint16_t hci_disconnect_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{
  /* Input params */
  hci_disconnect_cp0 *cp0 = (hci_disconnect_cp0 *)(buffer_in + 1 + (0));

  int output_size = 1;
  /* Output params */
  uint8_t *status = (uint8_t *) (buffer_out + 3);

  if (buffer_out_max_length < (1 + 6)) { return 0; }
  *status = hci_disconnect(cp0->Connection_Handle /* 2 */,
                           cp0->Reason /* 1 */);
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0F;
  buffer_out[2] = output_size + 3;
  buffer_out[4] = 0x01;
  buffer_out[5] = 0x06;
  buffer_out[6] = 0x04;
  return (output_size + 6);
}
/* tBleStatus hci_read_remote_version_information(uint16_t Connection_Handle);
 */
/* Command len: 2 */
/* Response len: 1 */
uint16_t hci_read_remote_version_information_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{
  /* Input params */
  hci_read_remote_version_information_cp0 *cp0 = (hci_read_remote_version_information_cp0 *)(buffer_in + 1 + (0));

  int output_size = 1;
  /* Output params */
  uint8_t *status = (uint8_t *) (buffer_out + 3);

  *status = hci_read_remote_version_information(cp0->Connection_Handle /* 2 */);
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0F;
  buffer_out[2] = output_size + 3;
  buffer_out[4] = 0x01;
  buffer_out[5] = 0x1d;
  buffer_out[6] = 0x04;
  return (output_size + 6);
}
/* tBleStatus hci_set_event_mask(uint8_t Event_Mask[8]);
 */
/* Command len: 8 */
/* Response len: 1 */
uint16_t hci_set_event_mask_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{
  /* Input params */
  hci_set_event_mask_cp0 *cp0 = (hci_set_event_mask_cp0 *)(buffer_in + 1 + (0));

  int output_size = 1;
  /* Output params */
  uint8_t *status = (uint8_t *) (buffer_out + 6);

  if (buffer_out_max_length < (1 + 6)) { return 0; }
  *status = hci_set_event_mask(cp0->Event_Mask /* 8 */);
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0E;
  buffer_out[2] = output_size + 3;
  buffer_out[3] = 0x01;
  buffer_out[4] = 0x01;
  buffer_out[5] = 0x0c;
  return (output_size + 6);
}
/* tBleStatus hci_reset(void);
 */
/* Command len: 0 */
/* Response len: 1 */
uint16_t hci_reset_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{

  int output_size = 1;
  /* Output params */
  uint8_t *status = (uint8_t *) (buffer_out + 6);

  *status = hci_reset();
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0E;
  buffer_out[2] = output_size + 3;
  buffer_out[3] = 0x01;
  buffer_out[4] = 0x03;
  buffer_out[5] = 0x0c;
  return (output_size + 6);
}
/* tBleStatus hci_read_transmit_power_level(uint16_t Connection_Handle,
                                         uint8_t Type,
                                         uint8_t *Transmit_Power_Level);
 */
/* Command len: 2 + 1 */
/* Response len: 1 + 2 + 1 */
uint16_t hci_read_transmit_power_level_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{
  /* Input params */
  hci_read_transmit_power_level_cp0 *cp0 = (hci_read_transmit_power_level_cp0 *)(buffer_in + 1 + (0));

  int output_size = 1 + 2 + 1;
  /* Output params */
  hci_read_transmit_power_level_rp0 *rp0 = (hci_read_transmit_power_level_rp0 *) (buffer_out + 6);
  uint8_t Transmit_Power_Level = 0;

  if (buffer_out_max_length < (1 + 2 + 1 + 6)) { return 0; }
  rp0->Status = hci_read_transmit_power_level(cp0->Connection_Handle /* 2 */,
                                              cp0->Type /* 1 */,
                                              &Transmit_Power_Level);
  rp0->Connection_Handle = cp0->Connection_Handle;
  rp0->Transmit_Power_Level = Transmit_Power_Level;
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0E;
  buffer_out[2] = output_size + 3;
  buffer_out[3] = 0x01;
  buffer_out[4] = 0x2d;
  buffer_out[5] = 0x0c;
  return (output_size + 6);
}
/* tBleStatus hci_read_local_version_information(uint8_t *HCI_Version,
                                              uint16_t *HCI_Revision,
                                              uint8_t *LMP_PAL_Version,
                                              uint16_t *Manufacturer_Name,
                                              uint16_t *LMP_PAL_Subversion);
 */
/* Command len: 0 */
/* Response len: 1 + 1 + 2 + 1 + 2 + 2 */
uint16_t hci_read_local_version_information_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{

  int output_size = 1 + 1 + 2 + 1 + 2 + 2;
  /* Output params */
  hci_read_local_version_information_rp0 *rp0 = (hci_read_local_version_information_rp0 *) (buffer_out + 6);
  uint8_t HCI_Version = 0;
  uint16_t HCI_Revision = 0;
  uint8_t LMP_PAL_Version = 0;
  uint16_t Manufacturer_Name = 0;
  uint16_t LMP_PAL_Subversion = 0;

  if (buffer_out_max_length < (1 + 1 + 2 + 1 + 2 + 2 + 6)) { return 0; }
  rp0->Status = hci_read_local_version_information(&HCI_Version,
                                                   &HCI_Revision,
                                                   &LMP_PAL_Version,
                                                   &Manufacturer_Name,
                                                   &LMP_PAL_Subversion);
  {  
        PartInfoType partInfo;
        /* get partInfo */
        HAL_GetPartInfo(&partInfo);
        //  H[15:12]= Hardware cut major version
        //  h[11:8]= Hardware cut minor version 
        //  C[7:4]= 0: BlueNRG/BlueNRG-MS, 1: BlueNRG-1; 2: BlueNRG-2;
        //  S[3:0]= Library major version 
        /* Set HCI_Revision fields with information coming from HAL_GetPartInfo() */
        HCI_Revision = HCI_Revision & 0xF;
        HCI_Revision |= ((partInfo.die_major & 0xF) <<12) | ((partInfo.die_cut & 0xF) <<8) | ((partInfo.die_id & 0xF) <<4) ;
  }
  rp0->HCI_Version = HCI_Version;
  rp0->HCI_Revision = HCI_Revision;
  rp0->LMP_PAL_Version = LMP_PAL_Version;
  rp0->Manufacturer_Name = Manufacturer_Name;
  rp0->LMP_PAL_Subversion = LMP_PAL_Subversion;
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0E;
  buffer_out[2] = output_size + 3;
  buffer_out[3] = 0x01;
  buffer_out[4] = 0x01;
  buffer_out[5] = 0x10;
  return (output_size + 6);
}
/* tBleStatus hci_read_local_supported_commands(uint8_t Supported_Commands[64]);
 */
/* Command len: 0 */
/* Response len: 1 + 64 */
uint16_t hci_read_local_supported_commands_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{

  int output_size = 1 + 64;
  /* Output params */
  hci_read_local_supported_commands_rp0 *rp0 = (hci_read_local_supported_commands_rp0 *) (buffer_out + 6);
  uint8_t Supported_Commands[64];

  if (buffer_out_max_length < (1 + 64 + 6)) { return 0; }
  rp0->Status = hci_read_local_supported_commands(Supported_Commands);
  Osal_MemCpy((void *) rp0->Supported_Commands,(const void *) Supported_Commands, 64);
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0E;
  buffer_out[2] = output_size + 3;
  buffer_out[3] = 0x01;
  buffer_out[4] = 0x02;
  buffer_out[5] = 0x10;
  return (output_size + 6);
}
/* tBleStatus hci_read_local_supported_features(uint8_t LMP_Features[8]);
 */
/* Command len: 0 */
/* Response len: 1 + 8 */
uint16_t hci_read_local_supported_features_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{

  int output_size = 1 + 8;
  /* Output params */
  hci_read_local_supported_features_rp0 *rp0 = (hci_read_local_supported_features_rp0 *) (buffer_out + 6);
  uint8_t LMP_Features[8];

  if (buffer_out_max_length < (1 + 8 + 6)) { return 0; }
  rp0->Status = hci_read_local_supported_features(LMP_Features);
  Osal_MemCpy((void *) rp0->LMP_Features,(const void *) LMP_Features, 8);
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0E;
  buffer_out[2] = output_size + 3;
  buffer_out[3] = 0x01;
  buffer_out[4] = 0x03;
  buffer_out[5] = 0x10;
  return (output_size + 6);
}
/* tBleStatus hci_read_bd_addr(uint8_t BD_ADDR[6]);
 */
/* Command len: 0 */
/* Response len: 1 + 6 */
uint16_t hci_read_bd_addr_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{

  int output_size = 1 + 6;
  /* Output params */
  hci_read_bd_addr_rp0 *rp0 = (hci_read_bd_addr_rp0 *) (buffer_out + 6);
  uint8_t BD_ADDR[6];

  if (buffer_out_max_length < (1 + 6 + 6)) { return 0; }
  rp0->Status = hci_read_bd_addr(BD_ADDR);
  Osal_MemCpy((void *) rp0->BD_ADDR,(const void *) BD_ADDR, 6);
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0E;
  buffer_out[2] = output_size + 3;
  buffer_out[3] = 0x01;
  buffer_out[4] = 0x09;
  buffer_out[5] = 0x10;
  return (output_size + 6);
}
/* tBleStatus hci_read_rssi(uint16_t Connection_Handle,
                         uint8_t *RSSI);
 */
/* Command len: 2 */
/* Response len: 1 + 2 + 1 */
uint16_t hci_read_rssi_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{
  /* Input params */
  hci_read_rssi_cp0 *cp0 = (hci_read_rssi_cp0 *)(buffer_in + 1 + (0));

  int output_size = 1 + 2 + 1;
  /* Output params */
  hci_read_rssi_rp0 *rp0 = (hci_read_rssi_rp0 *) (buffer_out + 6);
  uint8_t RSSI = 0;

  if (buffer_out_max_length < (1 + 2 + 1 + 6)) { return 0; }
  rp0->Status = hci_read_rssi(cp0->Connection_Handle /* 2 */,
                              &RSSI);
  rp0->Connection_Handle = cp0->Connection_Handle;
  rp0->RSSI = RSSI;
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0E;
  buffer_out[2] = output_size + 3;
  buffer_out[3] = 0x01;
  buffer_out[4] = 0x05;
  buffer_out[5] = 0x14;
  return (output_size + 6);
}
/* tBleStatus hci_le_set_event_mask(uint8_t LE_Event_Mask[8]);
 */
/* Command len: 8 */
/* Response len: 1 */
uint16_t hci_le_set_event_mask_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{
  /* Input params */
  hci_le_set_event_mask_cp0 *cp0 = (hci_le_set_event_mask_cp0 *)(buffer_in + 1 + (0));

  int output_size = 1;
  /* Output params */
  uint8_t *status = (uint8_t *) (buffer_out + 6);

  if (buffer_out_max_length < (1 + 6)) { return 0; }
  *status = hci_le_set_event_mask(cp0->LE_Event_Mask /* 8 */);
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0E;
  buffer_out[2] = output_size + 3;
  buffer_out[3] = 0x01;
  buffer_out[4] = 0x01;
  buffer_out[5] = 0x20;
  return (output_size + 6);
}
/* tBleStatus hci_le_read_buffer_size(uint16_t *HC_LE_ACL_Data_Packet_Length,
                                   uint8_t *HC_Total_Num_LE_ACL_Data_Packets);
 */
/* Command len: 0 */
/* Response len: 1 + 2 + 1 */
uint16_t hci_le_read_buffer_size_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{

  int output_size = 1 + 2 + 1;
  /* Output params */
  hci_le_read_buffer_size_rp0 *rp0 = (hci_le_read_buffer_size_rp0 *) (buffer_out + 6);
  uint16_t HC_LE_ACL_Data_Packet_Length = 0;
  uint8_t HC_Total_Num_LE_ACL_Data_Packets = 0;

  if (buffer_out_max_length < (1 + 2 + 1 + 6)) { return 0; }
  rp0->Status = hci_le_read_buffer_size(&HC_LE_ACL_Data_Packet_Length,
                                        &HC_Total_Num_LE_ACL_Data_Packets);
  rp0->HC_LE_ACL_Data_Packet_Length = HC_LE_ACL_Data_Packet_Length;
  rp0->HC_Total_Num_LE_ACL_Data_Packets = HC_Total_Num_LE_ACL_Data_Packets;
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0E;
  buffer_out[2] = output_size + 3;
  buffer_out[3] = 0x01;
  buffer_out[4] = 0x02;
  buffer_out[5] = 0x20;
  return (output_size + 6);
}
/* tBleStatus hci_le_read_local_supported_features(uint8_t LE_Features[8]);
 */
/* Command len: 0 */
/* Response len: 1 + 8 */
uint16_t hci_le_read_local_supported_features_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{

  int output_size = 1 + 8;
  /* Output params */
  hci_le_read_local_supported_features_rp0 *rp0 = (hci_le_read_local_supported_features_rp0 *) (buffer_out + 6);
  uint8_t LE_Features[8];

  if (buffer_out_max_length < (1 + 8 + 6)) { return 0; }
  rp0->Status = hci_le_read_local_supported_features(LE_Features);
  Osal_MemCpy((void *) rp0->LE_Features,(const void *) LE_Features, 8);
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0E;
  buffer_out[2] = output_size + 3;
  buffer_out[3] = 0x01;
  buffer_out[4] = 0x03;
  buffer_out[5] = 0x20;
  return (output_size + 6);
}
/* tBleStatus hci_le_set_random_address(uint8_t Random_Address[6]);
 */
/* Command len: 6 */
/* Response len: 1 */
uint16_t hci_le_set_random_address_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{
  /* Input params */
  hci_le_set_random_address_cp0 *cp0 = (hci_le_set_random_address_cp0 *)(buffer_in + 1 + (0));

  int output_size = 1;
  /* Output params */
  uint8_t *status = (uint8_t *) (buffer_out + 6);

  if (buffer_out_max_length < (1 + 6)) { return 0; }
  *status = hci_le_set_random_address(cp0->Random_Address /* 6 */);
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0E;
  buffer_out[2] = output_size + 3;
  buffer_out[3] = 0x01;
  buffer_out[4] = 0x05;
  buffer_out[5] = 0x20;
  return (output_size + 6);
}
/* tBleStatus hci_le_set_advertising_parameters(uint16_t Advertising_Interval_Min,
                                             uint16_t Advertising_Interval_Max,
                                             uint8_t Advertising_Type,
                                             uint8_t Own_Address_Type,
                                             uint8_t Peer_Address_Type,
                                             uint8_t Peer_Address[6],
                                             uint8_t Advertising_Channel_Map,
                                             uint8_t Advertising_Filter_Policy);
 */
/* Command len: 2 + 2 + 1 + 1 + 1 + 6 + 1 + 1 */
/* Response len: 1 */
uint16_t hci_le_set_advertising_parameters_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{
  /* Input params */
  hci_le_set_advertising_parameters_cp0 *cp0 = (hci_le_set_advertising_parameters_cp0 *)(buffer_in + 1 + (0));

  int output_size = 1;
  /* Output params */
  uint8_t *status = (uint8_t *) (buffer_out + 6);

  if (buffer_out_max_length < (1 + 6)) { return 0; }
  *status = hci_le_set_advertising_parameters(cp0->Advertising_Interval_Min /* 2 */,
                                              cp0->Advertising_Interval_Max /* 2 */,
                                              cp0->Advertising_Type /* 1 */,
                                              cp0->Own_Address_Type /* 1 */,
                                              cp0->Peer_Address_Type /* 1 */,
                                              cp0->Peer_Address /* 6 */,
                                              cp0->Advertising_Channel_Map /* 1 */,
                                              cp0->Advertising_Filter_Policy /* 1 */);
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0E;
  buffer_out[2] = output_size + 3;
  buffer_out[3] = 0x01;
  buffer_out[4] = 0x06;
  buffer_out[5] = 0x20;
  return (output_size + 6);
}
/* tBleStatus hci_le_read_advertising_channel_tx_power(uint8_t *Transmit_Power_Level);
 */
/* Command len: 0 */
/* Response len: 1 + 1 */
uint16_t hci_le_read_advertising_channel_tx_power_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{

  int output_size = 1 + 1;
  /* Output params */
  hci_le_read_advertising_channel_tx_power_rp0 *rp0 = (hci_le_read_advertising_channel_tx_power_rp0 *) (buffer_out + 6);
  uint8_t Transmit_Power_Level = 0;

  if (buffer_out_max_length < (1 + 1 + 6)) { return 0; }
  rp0->Status = hci_le_read_advertising_channel_tx_power(&Transmit_Power_Level);
  rp0->Transmit_Power_Level = Transmit_Power_Level;
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0E;
  buffer_out[2] = output_size + 3;
  buffer_out[3] = 0x01;
  buffer_out[4] = 0x07;
  buffer_out[5] = 0x20;
  return (output_size + 6);
}
/* tBleStatus hci_le_set_advertising_data(uint8_t Advertising_Data_Length,
                                       uint8_t Advertising_Data[31]);
 */
/* Command len: 1 + 31 */
/* Response len: 1 */
uint16_t hci_le_set_advertising_data_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{
  /* Input params */
  hci_le_set_advertising_data_cp0 *cp0 = (hci_le_set_advertising_data_cp0 *)(buffer_in + 1 + (0));

  int output_size = 1;
  /* Output params */
  uint8_t *status = (uint8_t *) (buffer_out + 6);

  if (buffer_out_max_length < (1 + 6)) { return 0; }
  *status = hci_le_set_advertising_data(cp0->Advertising_Data_Length /* 1 */,
                                        cp0->Advertising_Data /* 31 */);
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0E;
  buffer_out[2] = output_size + 3;
  buffer_out[3] = 0x01;
  buffer_out[4] = 0x08;
  buffer_out[5] = 0x20;
  return (output_size + 6);
}
/* tBleStatus hci_le_set_scan_response_data(uint8_t Scan_Response_Data_Length,
                                         uint8_t Scan_Response_Data[31]);
 */
/* Command len: 1 + 31 */
/* Response len: 1 */
uint16_t hci_le_set_scan_response_data_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{
  /* Input params */
  hci_le_set_scan_response_data_cp0 *cp0 = (hci_le_set_scan_response_data_cp0 *)(buffer_in + 1 + (0));

  int output_size = 1;
  /* Output params */
  uint8_t *status = (uint8_t *) (buffer_out + 6);

  if (buffer_out_max_length < (1 + 6)) { return 0; }
  *status = hci_le_set_scan_response_data(cp0->Scan_Response_Data_Length /* 1 */,
                                          cp0->Scan_Response_Data /* 31 */);
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0E;
  buffer_out[2] = output_size + 3;
  buffer_out[3] = 0x01;
  buffer_out[4] = 0x09;
  buffer_out[5] = 0x20;
  return (output_size + 6);
}
/* tBleStatus hci_le_set_advertise_enable(uint8_t Advertising_Enable);
 */
/* Command len: 1 */
/* Response len: 1 */
uint16_t hci_le_set_advertise_enable_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{
  /* Input params */
  hci_le_set_advertise_enable_cp0 *cp0 = (hci_le_set_advertise_enable_cp0 *)(buffer_in + 1 + (0));

  int output_size = 1;
  /* Output params */
  uint8_t *status = (uint8_t *) (buffer_out + 6);

  *status = hci_le_set_advertise_enable(cp0->Advertising_Enable /* 1 */);
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0E;
  buffer_out[2] = output_size + 3;
  buffer_out[3] = 0x01;
  buffer_out[4] = 0x0a;
  buffer_out[5] = 0x20;
  return (output_size + 6);
}
/* tBleStatus hci_le_set_scan_parameters(uint8_t LE_Scan_Type,
                                      uint16_t LE_Scan_Interval,
                                      uint16_t LE_Scan_Window,
                                      uint8_t Own_Address_Type,
                                      uint8_t Scanning_Filter_Policy);
 */
/* Command len: 1 + 2 + 2 + 1 + 1 */
/* Response len: 1 */
uint16_t hci_le_set_scan_parameters_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{
  /* Input params */
  hci_le_set_scan_parameters_cp0 *cp0 = (hci_le_set_scan_parameters_cp0 *)(buffer_in + 1 + (0));

  int output_size = 1;
  /* Output params */
  uint8_t *status = (uint8_t *) (buffer_out + 6);

  if (buffer_out_max_length < (1 + 6)) { return 0; }
  *status = hci_le_set_scan_parameters(cp0->LE_Scan_Type /* 1 */,
                                       cp0->LE_Scan_Interval /* 2 */,
                                       cp0->LE_Scan_Window /* 2 */,
                                       cp0->Own_Address_Type /* 1 */,
                                       cp0->Scanning_Filter_Policy /* 1 */);
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0E;
  buffer_out[2] = output_size + 3;
  buffer_out[3] = 0x01;
  buffer_out[4] = 0x0b;
  buffer_out[5] = 0x20;
  return (output_size + 6);
}
/* tBleStatus hci_le_set_scan_enable(uint8_t LE_Scan_Enable,
                                  uint8_t Filter_Duplicates);
 */
/* Command len: 1 + 1 */
/* Response len: 1 */
uint16_t hci_le_set_scan_enable_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{
  /* Input params */
  hci_le_set_scan_enable_cp0 *cp0 = (hci_le_set_scan_enable_cp0 *)(buffer_in + 1 + (0));

  int output_size = 1;
  /* Output params */
  uint8_t *status = (uint8_t *) (buffer_out + 6);

  if (buffer_out_max_length < (1 + 6)) { return 0; }
  *status = hci_le_set_scan_enable(cp0->LE_Scan_Enable /* 1 */,
                                   cp0->Filter_Duplicates /* 1 */);
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0E;
  buffer_out[2] = output_size + 3;
  buffer_out[3] = 0x01;
  buffer_out[4] = 0x0c;
  buffer_out[5] = 0x20;
  return (output_size + 6);
}
/* tBleStatus hci_le_create_connection(uint16_t LE_Scan_Interval,
                                    uint16_t LE_Scan_Window,
                                    uint8_t Initiator_Filter_Policy,
                                    uint8_t Peer_Address_Type,
                                    uint8_t Peer_Address[6],
                                    uint8_t Own_Address_Type,
                                    uint16_t Conn_Interval_Min,
                                    uint16_t Conn_Interval_Max,
                                    uint16_t Conn_Latency,
                                    uint16_t Supervision_Timeout,
                                    uint16_t Minimum_CE_Length,
                                    uint16_t Maximum_CE_Length);
 */
/* Command len: 2 + 2 + 1 + 1 + 6 + 1 + 2 + 2 + 2 + 2 + 2 + 2 */
/* Response len: 1 */
uint16_t hci_le_create_connection_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{
  /* Input params */
  hci_le_create_connection_cp0 *cp0 = (hci_le_create_connection_cp0 *)(buffer_in + 1 + (0));

  int output_size = 1;
  /* Output params */
  uint8_t *status = (uint8_t *) (buffer_out + 3);

  if (buffer_out_max_length < (1 + 6)) { return 0; }
  *status = hci_le_create_connection(cp0->LE_Scan_Interval /* 2 */,
                                     cp0->LE_Scan_Window /* 2 */,
                                     cp0->Initiator_Filter_Policy /* 1 */,
                                     cp0->Peer_Address_Type /* 1 */,
                                     cp0->Peer_Address /* 6 */,
                                     cp0->Own_Address_Type /* 1 */,
                                     cp0->Conn_Interval_Min /* 2 */,
                                     cp0->Conn_Interval_Max /* 2 */,
                                     cp0->Conn_Latency /* 2 */,
                                     cp0->Supervision_Timeout /* 2 */,
                                     cp0->Minimum_CE_Length /* 2 */,
                                     cp0->Maximum_CE_Length /* 2 */);
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0F;
  buffer_out[2] = output_size + 3;
  buffer_out[4] = 0x01;
  buffer_out[5] = 0x0d;
  buffer_out[6] = 0x20;
  return (output_size + 6);
}
/* tBleStatus hci_le_create_connection_cancel(void);
 */
/* Command len: 0 */
/* Response len: 1 */
uint16_t hci_le_create_connection_cancel_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{

  int output_size = 1;
  /* Output params */
  uint8_t *status = (uint8_t *) (buffer_out + 6);

  if (buffer_out_max_length < (1 + 6)) { return 0; }
  *status = hci_le_create_connection_cancel();
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0E;
  buffer_out[2] = output_size + 3;
  buffer_out[3] = 0x01;
  buffer_out[4] = 0x0e;
  buffer_out[5] = 0x20;
  return (output_size + 6);
}
/* tBleStatus hci_le_read_white_list_size(uint8_t *White_List_Size);
 */
/* Command len: 0 */
/* Response len: 1 + 1 */
uint16_t hci_le_read_white_list_size_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{

  int output_size = 1 + 1;
  /* Output params */
  hci_le_read_white_list_size_rp0 *rp0 = (hci_le_read_white_list_size_rp0 *) (buffer_out + 6);
  uint8_t White_List_Size = 0;

  if (buffer_out_max_length < (1 + 1 + 6)) { return 0; }
  rp0->Status = hci_le_read_white_list_size(&White_List_Size);
  rp0->White_List_Size = White_List_Size;
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0E;
  buffer_out[2] = output_size + 3;
  buffer_out[3] = 0x01;
  buffer_out[4] = 0x0f;
  buffer_out[5] = 0x20;
  return (output_size + 6);
}
/* tBleStatus hci_le_clear_white_list(void);
 */
/* Command len: 0 */
/* Response len: 1 */
uint16_t hci_le_clear_white_list_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{

  int output_size = 1;
  /* Output params */
  uint8_t *status = (uint8_t *) (buffer_out + 6);

  if (buffer_out_max_length < (1 + 6)) { return 0; }
  *status = hci_le_clear_white_list();
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0E;
  buffer_out[2] = output_size + 3;
  buffer_out[3] = 0x01;
  buffer_out[4] = 0x10;
  buffer_out[5] = 0x20;
  return (output_size + 6);
}
/* tBleStatus hci_le_add_device_to_white_list(uint8_t Address_Type,
                                           uint8_t Address[6]);
 */
/* Command len: 1 + 6 */
/* Response len: 1 */
uint16_t hci_le_add_device_to_white_list_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{
  /* Input params */
  hci_le_add_device_to_white_list_cp0 *cp0 = (hci_le_add_device_to_white_list_cp0 *)(buffer_in + 1 + (0));

  int output_size = 1;
  /* Output params */
  uint8_t *status = (uint8_t *) (buffer_out + 6);

  if (buffer_out_max_length < (1 + 6)) { return 0; }
  *status = hci_le_add_device_to_white_list(cp0->Address_Type /* 1 */,
                                            cp0->Address /* 6 */);
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0E;
  buffer_out[2] = output_size + 3;
  buffer_out[3] = 0x01;
  buffer_out[4] = 0x11;
  buffer_out[5] = 0x20;
  return (output_size + 6);
}
/* tBleStatus hci_le_remove_device_from_white_list(uint8_t Address_Type,
                                                uint8_t Address[6]);
 */
/* Command len: 1 + 6 */
/* Response len: 1 */
uint16_t hci_le_remove_device_from_white_list_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{
  /* Input params */
  hci_le_remove_device_from_white_list_cp0 *cp0 = (hci_le_remove_device_from_white_list_cp0 *)(buffer_in + 1 + (0));

  int output_size = 1;
  /* Output params */
  uint8_t *status = (uint8_t *) (buffer_out + 6);

  if (buffer_out_max_length < (1 + 6)) { return 0; }
  *status = hci_le_remove_device_from_white_list(cp0->Address_Type /* 1 */,
                                                 cp0->Address /* 6 */);
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0E;
  buffer_out[2] = output_size + 3;
  buffer_out[3] = 0x01;
  buffer_out[4] = 0x12;
  buffer_out[5] = 0x20;
  return (output_size + 6);
}
/* tBleStatus hci_le_connection_update(uint16_t Connection_Handle,
                                    uint16_t Conn_Interval_Min,
                                    uint16_t Conn_Interval_Max,
                                    uint16_t Conn_Latency,
                                    uint16_t Supervision_Timeout,
                                    uint16_t Minimum_CE_Length,
                                    uint16_t Maximum_CE_Length);
 */
/* Command len: 2 + 2 + 2 + 2 + 2 + 2 + 2 */
/* Response len: 1 */
uint16_t hci_le_connection_update_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{
  /* Input params */
  hci_le_connection_update_cp0 *cp0 = (hci_le_connection_update_cp0 *)(buffer_in + 1 + (0));

  int output_size = 1;
  /* Output params */
  uint8_t *status = (uint8_t *) (buffer_out + 3);

  if (buffer_out_max_length < (1 + 6)) { return 0; }
  *status = hci_le_connection_update(cp0->Connection_Handle /* 2 */,
                                     cp0->Conn_Interval_Min /* 2 */,
                                     cp0->Conn_Interval_Max /* 2 */,
                                     cp0->Conn_Latency /* 2 */,
                                     cp0->Supervision_Timeout /* 2 */,
                                     cp0->Minimum_CE_Length /* 2 */,
                                     cp0->Maximum_CE_Length /* 2 */);
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0F;
  buffer_out[2] = output_size + 3;
  buffer_out[4] = 0x01;
  buffer_out[5] = 0x13;
  buffer_out[6] = 0x20;
  return (output_size + 6);
}
/* tBleStatus hci_le_set_host_channel_classification(uint8_t LE_Channel_Map[5]);
 */
/* Command len: 5 */
/* Response len: 1 */
uint16_t hci_le_set_host_channel_classification_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{
  /* Input params */
  hci_le_set_host_channel_classification_cp0 *cp0 = (hci_le_set_host_channel_classification_cp0 *)(buffer_in + 1 + (0));

  int output_size = 1;
  /* Output params */
  uint8_t *status = (uint8_t *) (buffer_out + 6);

  if (buffer_out_max_length < (1 + 6)) { return 0; }
  *status = hci_le_set_host_channel_classification(cp0->LE_Channel_Map /* 5 */);
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0E;
  buffer_out[2] = output_size + 3;
  buffer_out[3] = 0x01;
  buffer_out[4] = 0x14;
  buffer_out[5] = 0x20;
  return (output_size + 6);
}
/* tBleStatus hci_le_read_channel_map(uint16_t Connection_Handle,
                                   uint8_t LE_Channel_Map[5]);
 */
/* Command len: 2 */
/* Response len: 1 + 2 + 5 */
uint16_t hci_le_read_channel_map_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{
  /* Input params */
  hci_le_read_channel_map_cp0 *cp0 = (hci_le_read_channel_map_cp0 *)(buffer_in + 1 + (0));

  int output_size = 1 + 2 + 5;
  /* Output params */
  hci_le_read_channel_map_rp0 *rp0 = (hci_le_read_channel_map_rp0 *) (buffer_out + 6);
  uint8_t LE_Channel_Map[5];

  if (buffer_out_max_length < (1 + 2 + 5 + 6)) { return 0; }
  rp0->Status = hci_le_read_channel_map(cp0->Connection_Handle /* 2 */,
                                        LE_Channel_Map);
  rp0->Connection_Handle = cp0->Connection_Handle;
  Osal_MemCpy((void *) rp0->LE_Channel_Map,(const void *) LE_Channel_Map, 5);
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0E;
  buffer_out[2] = output_size + 3;
  buffer_out[3] = 0x01;
  buffer_out[4] = 0x15;
  buffer_out[5] = 0x20;
  return (output_size + 6);
}
/* tBleStatus hci_le_read_remote_used_features(uint16_t Connection_Handle);
 */
/* Command len: 2 */
/* Response len: 1 */
uint16_t hci_le_read_remote_used_features_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{
  /* Input params */
  hci_le_read_remote_used_features_cp0 *cp0 = (hci_le_read_remote_used_features_cp0 *)(buffer_in + 1 + (0));

  int output_size = 1;
  /* Output params */
  uint8_t *status = (uint8_t *) (buffer_out + 3);

  if (buffer_out_max_length < (1 + 6)) { return 0; }
  *status = hci_le_read_remote_used_features(cp0->Connection_Handle /* 2 */);
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0F;
  buffer_out[2] = output_size + 3;
  buffer_out[4] = 0x01;
  buffer_out[5] = 0x16;
  buffer_out[6] = 0x20;
  return (output_size + 6);
}
/* tBleStatus hci_le_encrypt(uint8_t Key[16],
                          uint8_t Plaintext_Data[16],
                          uint8_t Encrypted_Data[16]);
 */
/* Command len: 16 + 16 */
/* Response len: 1 + 16 */
uint16_t hci_le_encrypt_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{
  /* Input params */
  hci_le_encrypt_cp0 *cp0 = (hci_le_encrypt_cp0 *)(buffer_in + 1 + (0));

  int output_size = 1 + 16;
  /* Output params */
  hci_le_encrypt_rp0 *rp0 = (hci_le_encrypt_rp0 *) (buffer_out + 6);
  uint8_t Encrypted_Data[16];

  if (buffer_out_max_length < (1 + 16 + 6)) { return 0; }
  rp0->Status = hci_le_encrypt(cp0->Key /* 16 */,
                               cp0->Plaintext_Data /* 16 */,
                               Encrypted_Data);
  Osal_MemCpy((void *) rp0->Encrypted_Data,(const void *) Encrypted_Data, 16);
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0E;
  buffer_out[2] = output_size + 3;
  buffer_out[3] = 0x01;
  buffer_out[4] = 0x17;
  buffer_out[5] = 0x20;
  return (output_size + 6);
}
/* tBleStatus hci_le_rand(uint8_t Random_Number[8]);
 */
/* Command len: 0 */
/* Response len: 1 + 8 */
uint16_t hci_le_rand_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{

  int output_size = 1 + 8;
  /* Output params */
  hci_le_rand_rp0 *rp0 = (hci_le_rand_rp0 *) (buffer_out + 6);
  uint8_t Random_Number[8];

  if (buffer_out_max_length < (1 + 8 + 6)) { return 0; }
  rp0->Status = hci_le_rand(Random_Number);
  Osal_MemCpy((void *) rp0->Random_Number,(const void *) Random_Number, 8);
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0E;
  buffer_out[2] = output_size + 3;
  buffer_out[3] = 0x01;
  buffer_out[4] = 0x18;
  buffer_out[5] = 0x20;
  return (output_size + 6);
}
/* tBleStatus hci_le_start_encryption(uint16_t Connection_Handle,
                                   uint8_t Random_Number[8],
                                   uint16_t Encrypted_Diversifier,
                                   uint8_t Long_Term_Key[16]);
 */
/* Command len: 2 + 8 + 2 + 16 */
/* Response len: 1 */
uint16_t hci_le_start_encryption_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{
  /* Input params */
  hci_le_start_encryption_cp0 *cp0 = (hci_le_start_encryption_cp0 *)(buffer_in + 1 + (0));

  int output_size = 1;
  /* Output params */
  uint8_t *status = (uint8_t *) (buffer_out + 3);

  if (buffer_out_max_length < (1 + 6)) { return 0; }
  *status = hci_le_start_encryption(cp0->Connection_Handle /* 2 */,
                                    cp0->Random_Number /* 8 */,
                                    cp0->Encrypted_Diversifier /* 2 */,
                                    cp0->Long_Term_Key /* 16 */);
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0F;
  buffer_out[2] = output_size + 3;
  buffer_out[4] = 0x01;
  buffer_out[5] = 0x19;
  buffer_out[6] = 0x20;
  return (output_size + 6);
}
/* tBleStatus hci_le_long_term_key_request_reply(uint16_t Connection_Handle,
                                              uint8_t Long_Term_Key[16]);
 */
/* Command len: 2 + 16 */
/* Response len: 1 + 2 */
uint16_t hci_le_long_term_key_request_reply_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{
  /* Input params */
  hci_le_long_term_key_request_reply_cp0 *cp0 = (hci_le_long_term_key_request_reply_cp0 *)(buffer_in + 1 + (0));

  int output_size = 1 + 2;
  /* Output params */
  hci_le_long_term_key_request_reply_rp0 *rp0 = (hci_le_long_term_key_request_reply_rp0 *) (buffer_out + 6);

  if (buffer_out_max_length < (1 + 2 + 6)) { return 0; }
  rp0->Status = hci_le_long_term_key_request_reply(cp0->Connection_Handle /* 2 */,
                                                   cp0->Long_Term_Key /* 16 */);
  rp0->Connection_Handle = cp0->Connection_Handle;
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0E;
  buffer_out[2] = output_size + 3;
  buffer_out[3] = 0x01;
  buffer_out[4] = 0x1a;
  buffer_out[5] = 0x20;
  return (output_size + 6);
}
/* tBleStatus hci_le_long_term_key_requested_negative_reply(uint16_t Connection_Handle);
 */
/* Command len: 2 */
/* Response len: 1 + 2 */
uint16_t hci_le_long_term_key_requested_negative_reply_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{
  /* Input params */
  hci_le_long_term_key_requested_negative_reply_cp0 *cp0 = (hci_le_long_term_key_requested_negative_reply_cp0 *)(buffer_in + 1 + (0));

  int output_size = 1 + 2;
  /* Output params */
  hci_le_long_term_key_requested_negative_reply_rp0 *rp0 = (hci_le_long_term_key_requested_negative_reply_rp0 *) (buffer_out + 6);

  if (buffer_out_max_length < (1 + 2 + 6)) { return 0; }
  rp0->Status = hci_le_long_term_key_requested_negative_reply(cp0->Connection_Handle /* 2 */);
  rp0->Connection_Handle = cp0->Connection_Handle;
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0E;
  buffer_out[2] = output_size + 3;
  buffer_out[3] = 0x01;
  buffer_out[4] = 0x1b;
  buffer_out[5] = 0x20;
  return (output_size + 6);
}
/* tBleStatus hci_le_read_supported_states(uint8_t LE_States[8]);
 */
/* Command len: 0 */
/* Response len: 1 + 8 */
uint16_t hci_le_read_supported_states_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{

  int output_size = 1 + 8;
  /* Output params */
  hci_le_read_supported_states_rp0 *rp0 = (hci_le_read_supported_states_rp0 *) (buffer_out + 6);
  uint8_t LE_States[8];

  if (buffer_out_max_length < (1 + 8 + 6)) { return 0; }
  rp0->Status = hci_le_read_supported_states(LE_States);
  Osal_MemCpy((void *) rp0->LE_States,(const void *) LE_States, 8);
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0E;
  buffer_out[2] = output_size + 3;
  buffer_out[3] = 0x01;
  buffer_out[4] = 0x1c;
  buffer_out[5] = 0x20;
  return (output_size + 6);
}
/* tBleStatus hci_le_receiver_test(uint8_t RX_Frequency);
 */
/* Command len: 1 */
/* Response len: 1 */
uint16_t hci_le_receiver_test_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{
  /* Input params */
  hci_le_receiver_test_cp0 *cp0 = (hci_le_receiver_test_cp0 *)(buffer_in + 1 + (0));

  int output_size = 1;
  /* Output params */
  uint8_t *status = (uint8_t *) (buffer_out + 6);

  if (buffer_out_max_length < (1 + 6)) { return 0; }
  *status = hci_le_receiver_test(cp0->RX_Frequency /* 1 */);
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0E;
  buffer_out[2] = output_size + 3;
  buffer_out[3] = 0x01;
  buffer_out[4] = 0x1d;
  buffer_out[5] = 0x20;
  return (output_size + 6);
}
/* tBleStatus hci_le_transmitter_test(uint8_t TX_Frequency,
                                   uint8_t Length_Of_Test_Data,
                                   uint8_t Packet_Payload);
 */
/* Command len: 1 + 1 + 1 */
/* Response len: 1 */
uint16_t hci_le_transmitter_test_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{
  /* Input params */
  hci_le_transmitter_test_cp0 *cp0 = (hci_le_transmitter_test_cp0 *)(buffer_in + 1 + (0));

  int output_size = 1;
  /* Output params */
  uint8_t *status = (uint8_t *) (buffer_out + 6);

  if (buffer_out_max_length < (1 + 6)) { return 0; }
  *status = hci_le_transmitter_test(cp0->TX_Frequency /* 1 */,
                                    cp0->Length_Of_Test_Data /* 1 */,
                                    cp0->Packet_Payload /* 1 */);
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0E;
  buffer_out[2] = output_size + 3;
  buffer_out[3] = 0x01;
  buffer_out[4] = 0x1e;
  buffer_out[5] = 0x20;
  return (output_size + 6);
}
/* tBleStatus hci_le_test_end(uint16_t *Number_Of_Packets);
 */
/* Command len: 0 */
/* Response len: 1 + 2 */
uint16_t hci_le_test_end_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{

  int output_size = 1 + 2;
  /* Output params */
  hci_le_test_end_rp0 *rp0 = (hci_le_test_end_rp0 *) (buffer_out + 6);
  uint16_t Number_Of_Packets = 0;

  if (buffer_out_max_length < (1 + 2 + 6)) { return 0; }
  rp0->Status = hci_le_test_end(&Number_Of_Packets);
  rp0->Number_Of_Packets = Number_Of_Packets;
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0E;
  buffer_out[2] = output_size + 3;
  buffer_out[3] = 0x01;
  buffer_out[4] = 0x1f;
  buffer_out[5] = 0x20;
  return (output_size + 6);
}
/* tBleStatus hci_le_set_data_length(uint16_t Connection_Handle,
                                  uint16_t TxOctets,
                                  uint16_t TxTime);
 */
/* Command len: 2 + 2 + 2 */
/* Response len: 1 + 2 */
uint16_t hci_le_set_data_length_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{
  /* Input params */
  hci_le_set_data_length_cp0 *cp0 = (hci_le_set_data_length_cp0 *)(buffer_in + 1 + (0));

  int output_size = 1 + 2;
  /* Output params */
  hci_le_set_data_length_rp0 *rp0 = (hci_le_set_data_length_rp0 *) (buffer_out + 6);

  if (buffer_out_max_length < (1 + 2 + 6)) { return 0; }
  rp0->Status = hci_le_set_data_length(cp0->Connection_Handle /* 2 */,
                                       cp0->TxOctets /* 2 */,
                                       cp0->TxTime /* 2 */);
  rp0->Connection_Handle = cp0->Connection_Handle;
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0E;
  buffer_out[2] = output_size + 3;
  buffer_out[3] = 0x01;
  buffer_out[4] = 0x22;
  buffer_out[5] = 0x20;
  return (output_size + 6);
}
/* tBleStatus hci_le_read_suggested_default_data_length(uint16_t *SuggestedMaxTxOctets,
                                                     uint16_t *SuggestedMaxTxTime);
 */
/* Command len: 0 */
/* Response len: 1 + 2 + 2 */
uint16_t hci_le_read_suggested_default_data_length_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{

  int output_size = 1 + 2 + 2;
  /* Output params */
  hci_le_read_suggested_default_data_length_rp0 *rp0 = (hci_le_read_suggested_default_data_length_rp0 *) (buffer_out + 6);
  uint16_t SuggestedMaxTxOctets = 0;
  uint16_t SuggestedMaxTxTime = 0;

  if (buffer_out_max_length < (1 + 2 + 2 + 6)) { return 0; }
  rp0->Status = hci_le_read_suggested_default_data_length(&SuggestedMaxTxOctets,
                                                          &SuggestedMaxTxTime);
  rp0->SuggestedMaxTxOctets = SuggestedMaxTxOctets;
  rp0->SuggestedMaxTxTime = SuggestedMaxTxTime;
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0E;
  buffer_out[2] = output_size + 3;
  buffer_out[3] = 0x01;
  buffer_out[4] = 0x23;
  buffer_out[5] = 0x20;
  return (output_size + 6);
}
/* tBleStatus hci_le_write_suggested_default_data_length(uint16_t SuggestedMaxTxOctets,
                                                      uint16_t SuggestedMaxTxTime);
 */
/* Command len: 2 + 2 */
/* Response len: 1 */
uint16_t hci_le_write_suggested_default_data_length_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{
  /* Input params */
  hci_le_write_suggested_default_data_length_cp0 *cp0 = (hci_le_write_suggested_default_data_length_cp0 *)(buffer_in + 1 + (0));

  int output_size = 1;
  /* Output params */
  uint8_t *status = (uint8_t *) (buffer_out + 6);

  if (buffer_out_max_length < (1 + 6)) { return 0; }
  *status = hci_le_write_suggested_default_data_length(cp0->SuggestedMaxTxOctets /* 2 */,
                                                       cp0->SuggestedMaxTxTime /* 2 */);
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0E;
  buffer_out[2] = output_size + 3;
  buffer_out[3] = 0x01;
  buffer_out[4] = 0x24;
  buffer_out[5] = 0x20;
  return (output_size + 6);
}
/* tBleStatus hci_le_read_local_p256_public_key(void);
 */
/* Command len: 0 */
/* Response len: 1 */
uint16_t hci_le_read_local_p256_public_key_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{

  int output_size = 1;
  /* Output params */
  uint8_t *status = (uint8_t *) (buffer_out + 3);

  if (buffer_out_max_length < (1 + 6)) { return 0; }
  *status = hci_le_read_local_p256_public_key();
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0F;
  buffer_out[2] = output_size + 3;
  buffer_out[4] = 0x01;
  buffer_out[5] = 0x25;
  buffer_out[6] = 0x20;
  return (output_size + 6);
}
/* tBleStatus hci_le_generate_dhkey(uint8_t Remote_P256_Public_Key[64]);
 */
/* Command len: 64 */
/* Response len: 1 */
uint16_t hci_le_generate_dhkey_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{
  /* Input params */
  hci_le_generate_dhkey_cp0 *cp0 = (hci_le_generate_dhkey_cp0 *)(buffer_in + 1 + (0));

  int output_size = 1;
  /* Output params */
  uint8_t *status = (uint8_t *) (buffer_out + 3);

  if (buffer_out_max_length < (1 + 6)) { return 0; }
  *status = hci_le_generate_dhkey(cp0->Remote_P256_Public_Key /* 64 */);
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0F;
  buffer_out[2] = output_size + 3;
  buffer_out[4] = 0x01;
  buffer_out[5] = 0x26;
  buffer_out[6] = 0x20;
  return (output_size + 6);
}
/* tBleStatus hci_le_add_device_to_resolving_list(uint8_t Peer_Identity_Address_Type,
                                               uint8_t Peer_Identity_Address[6],
                                               uint8_t Peer_IRK[16],
                                               uint8_t Local_IRK[16]);
 */
/* Command len: 1 + 6 + 16 + 16 */
/* Response len: 1 */
uint16_t hci_le_add_device_to_resolving_list_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{
  /* Input params */
  hci_le_add_device_to_resolving_list_cp0 *cp0 = (hci_le_add_device_to_resolving_list_cp0 *)(buffer_in + 1 + (0));

  int output_size = 1;
  /* Output params */
  uint8_t *status = (uint8_t *) (buffer_out + 6);

  if (buffer_out_max_length < (1 + 6)) { return 0; }
  *status = hci_le_add_device_to_resolving_list(cp0->Peer_Identity_Address_Type /* 1 */,
                                                cp0->Peer_Identity_Address /* 6 */,
                                                cp0->Peer_IRK /* 16 */,
                                                cp0->Local_IRK /* 16 */);
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0E;
  buffer_out[2] = output_size + 3;
  buffer_out[3] = 0x01;
  buffer_out[4] = 0x27;
  buffer_out[5] = 0x20;
  return (output_size + 6);
}
/* tBleStatus hci_le_remove_device_from_resolving_list(uint8_t Peer_Identity_Address_Type,
                                                    uint8_t Peer_Identity_Address[6]);
 */
/* Command len: 1 + 6 */
/* Response len: 1 */
uint16_t hci_le_remove_device_from_resolving_list_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{
  /* Input params */
  hci_le_remove_device_from_resolving_list_cp0 *cp0 = (hci_le_remove_device_from_resolving_list_cp0 *)(buffer_in + 1 + (0));

  int output_size = 1;
  /* Output params */
  uint8_t *status = (uint8_t *) (buffer_out + 6);

  if (buffer_out_max_length < (1 + 6)) { return 0; }
  *status = hci_le_remove_device_from_resolving_list(cp0->Peer_Identity_Address_Type /* 1 */,
                                                     cp0->Peer_Identity_Address /* 6 */);
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0E;
  buffer_out[2] = output_size + 3;
  buffer_out[3] = 0x01;
  buffer_out[4] = 0x28;
  buffer_out[5] = 0x20;
  return (output_size + 6);
}
/* tBleStatus hci_le_clear_resolving_list(void);
 */
/* Command len: 0 */
/* Response len: 1 */
uint16_t hci_le_clear_resolving_list_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{

  int output_size = 1;
  /* Output params */
  uint8_t *status = (uint8_t *) (buffer_out + 6);

  if (buffer_out_max_length < (1 + 6)) { return 0; }
  *status = hci_le_clear_resolving_list();
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0E;
  buffer_out[2] = output_size + 3;
  buffer_out[3] = 0x01;
  buffer_out[4] = 0x29;
  buffer_out[5] = 0x20;
  return (output_size + 6);
}
/* tBleStatus hci_le_read_resolving_list_size(uint8_t *Resolving_List_Size);
 */
/* Command len: 0 */
/* Response len: 1 + 1 */
uint16_t hci_le_read_resolving_list_size_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{

  int output_size = 1 + 1;
  /* Output params */
  hci_le_read_resolving_list_size_rp0 *rp0 = (hci_le_read_resolving_list_size_rp0 *) (buffer_out + 6);
  uint8_t Resolving_List_Size = 0;

  if (buffer_out_max_length < (1 + 1 + 6)) { return 0; }
  rp0->Status = hci_le_read_resolving_list_size(&Resolving_List_Size);
  rp0->Resolving_List_Size = Resolving_List_Size;
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0E;
  buffer_out[2] = output_size + 3;
  buffer_out[3] = 0x01;
  buffer_out[4] = 0x2a;
  buffer_out[5] = 0x20;
  return (output_size + 6);
}
/* tBleStatus hci_le_read_peer_resolvable_address(uint8_t Peer_Identity_Address_Type,
                                               uint8_t Peer_Identity_Address[6],
                                               uint8_t Peer_Resolvable_Address[6]);
 */
/* Command len: 1 + 6 */
/* Response len: 1 + 6 */
uint16_t hci_le_read_peer_resolvable_address_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{
  /* Input params */
  hci_le_read_peer_resolvable_address_cp0 *cp0 = (hci_le_read_peer_resolvable_address_cp0 *)(buffer_in + 1 + (0));

  int output_size = 1 + 6;
  /* Output params */
  hci_le_read_peer_resolvable_address_rp0 *rp0 = (hci_le_read_peer_resolvable_address_rp0 *) (buffer_out + 6);
  uint8_t Peer_Resolvable_Address[6];

  if (buffer_out_max_length < (1 + 6 + 6)) { return 0; }
  rp0->Status = hci_le_read_peer_resolvable_address(cp0->Peer_Identity_Address_Type /* 1 */,
                                                    cp0->Peer_Identity_Address /* 6 */,
                                                    Peer_Resolvable_Address);
  Osal_MemCpy((void *) rp0->Peer_Resolvable_Address,(const void *) Peer_Resolvable_Address, 6);
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0E;
  buffer_out[2] = output_size + 3;
  buffer_out[3] = 0x01;
  buffer_out[4] = 0x2b;
  buffer_out[5] = 0x20;
  return (output_size + 6);
}
/* tBleStatus hci_le_read_local_resolvable_address(uint8_t Peer_Identity_Address_Type,
                                                uint8_t Peer_Identity_Address[6],
                                                uint8_t Local_Resolvable_Address[6]);
 */
/* Command len: 1 + 6 */
/* Response len: 1 + 6 */
uint16_t hci_le_read_local_resolvable_address_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{
  /* Input params */
  hci_le_read_local_resolvable_address_cp0 *cp0 = (hci_le_read_local_resolvable_address_cp0 *)(buffer_in + 1 + (0));

  int output_size = 1 + 6;
  /* Output params */
  hci_le_read_local_resolvable_address_rp0 *rp0 = (hci_le_read_local_resolvable_address_rp0 *) (buffer_out + 6);
  uint8_t Local_Resolvable_Address[6];

  if (buffer_out_max_length < (1 + 6 + 6)) { return 0; }
  rp0->Status = hci_le_read_local_resolvable_address(cp0->Peer_Identity_Address_Type /* 1 */,
                                                     cp0->Peer_Identity_Address /* 6 */,
                                                     Local_Resolvable_Address);
  Osal_MemCpy((void *) rp0->Local_Resolvable_Address,(const void *) Local_Resolvable_Address, 6);
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0E;
  buffer_out[2] = output_size + 3;
  buffer_out[3] = 0x01;
  buffer_out[4] = 0x2c;
  buffer_out[5] = 0x20;
  return (output_size + 6);
}
/* tBleStatus hci_le_set_address_resolution_enable(uint8_t Address_Resolution_Enable);
 */
/* Command len: 1 */
/* Response len: 1 */
uint16_t hci_le_set_address_resolution_enable_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{
  /* Input params */
  hci_le_set_address_resolution_enable_cp0 *cp0 = (hci_le_set_address_resolution_enable_cp0 *)(buffer_in + 1 + (0));

  int output_size = 1;
  /* Output params */
  uint8_t *status = (uint8_t *) (buffer_out + 6);

  if (buffer_out_max_length < (1 + 6)) { return 0; }
  *status = hci_le_set_address_resolution_enable(cp0->Address_Resolution_Enable /* 1 */);
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0E;
  buffer_out[2] = output_size + 3;
  buffer_out[3] = 0x01;
  buffer_out[4] = 0x2d;
  buffer_out[5] = 0x20;
  return (output_size + 6);
}
/* tBleStatus hci_le_set_resolvable_private_address_timeout(uint16_t RPA_Timeout);
 */
/* Command len: 2 */
/* Response len: 1 */
uint16_t hci_le_set_resolvable_private_address_timeout_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{
  /* Input params */
  hci_le_set_resolvable_private_address_timeout_cp0 *cp0 = (hci_le_set_resolvable_private_address_timeout_cp0 *)(buffer_in + 1 + (0));

  int output_size = 1;
  /* Output params */
  uint8_t *status = (uint8_t *) (buffer_out + 6);

  if (buffer_out_max_length < (1 + 6)) { return 0; }
  *status = hci_le_set_resolvable_private_address_timeout(cp0->RPA_Timeout /* 2 */);
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0E;
  buffer_out[2] = output_size + 3;
  buffer_out[3] = 0x01;
  buffer_out[4] = 0x2e;
  buffer_out[5] = 0x20;
  return (output_size + 6);
}
/* tBleStatus hci_le_read_maximum_data_length(uint16_t *supportedMaxTxOctets,
                                           uint16_t *supportedMaxTxTime,
                                           uint16_t *supportedMaxRxOctets,
                                           uint16_t *supportedMaxRxTime);
 */
/* Command len: 0 */
/* Response len: 1 + 2 + 2 + 2 + 2 */
uint16_t hci_le_read_maximum_data_length_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{

  int output_size = 1 + 2 + 2 + 2 + 2;
  /* Output params */
  hci_le_read_maximum_data_length_rp0 *rp0 = (hci_le_read_maximum_data_length_rp0 *) (buffer_out + 6);
  uint16_t supportedMaxTxOctets = 0;
  uint16_t supportedMaxTxTime = 0;
  uint16_t supportedMaxRxOctets = 0;
  uint16_t supportedMaxRxTime = 0;

  if (buffer_out_max_length < (1 + 2 + 2 + 2 + 2 + 6)) { return 0; }
  rp0->Status = hci_le_read_maximum_data_length(&supportedMaxTxOctets,
                                                &supportedMaxTxTime,
                                                &supportedMaxRxOctets,
                                                &supportedMaxRxTime);
  rp0->supportedMaxTxOctets = supportedMaxTxOctets;
  rp0->supportedMaxTxTime = supportedMaxTxTime;
  rp0->supportedMaxRxOctets = supportedMaxRxOctets;
  rp0->supportedMaxRxTime = supportedMaxRxTime;
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0E;
  buffer_out[2] = output_size + 3;
  buffer_out[3] = 0x01;
  buffer_out[4] = 0x2f;
  buffer_out[5] = 0x20;
  return (output_size + 6);
}
/* hci_disconnection_complete_event */
/* Event len: 1 + 2 + 1 */
/**
  * @brief The Disconnection Complete event occurs when a connection is terminated.
The status parameter indicates if the disconnection was successful or not. The
reason parameter indicates the reason for the disconnection if the disconnection
was successful. If the disconnection was not successful, the value of the
reason parameter can be ignored by the Host. For example, this can be the
case if the Host has issued the Disconnect command and there was a parameter
error, or the command was not presently allowed, or a Connection_Handle
that didn't correspond to a connection was given.
  * @param Status For standard error codes see Bluetooth specification, Vol. 2, part D. For proprietary error code refer to Error codes section
  * @param Connection_Handle Connection_Handle which was disconnected.
  * Values:
  - 0x0000 ... 0x0EFF
  * @param Reason Reason for disconnection.
  * Values:
  - 0x00: Success
  - 0x01: Unknown HCI Command
  - 0x02: Unknown Connection Identifier
  - 0x03: Hardware Failure
  - 0x04: Page Timeout
  - 0x05: Authentication Failure
  - 0x06: PIN or Key Missing
  - 0x07: Memory Capacity Exceeded
  - 0x08: Connection Timeout
  - 0x09: Connection Limit Exceeded
  - 0x0A: Synchronous Connection Limit to a Device Exceeded
  - 0x0B: ACL Connection Already Exists
  - 0x0C: Command Disallowed
  - 0x0D: Connection Rejected Due To Limited Resources
  - 0x0E: Connection Rejected Due To Security Reasons
  - 0x0F: Connection Rejected due to Unacceptable BD_ADDR
  - 0x10: Connection Accept Timeout Exceeded
  - 0x11: Unsupported Feature Or Parameter Value
  - 0x12: Invalid HCI Command Parameters
  - 0x13: Remote User Terminated Connection
  - 0x14: Remote Device Terminated Connection due to Low Resources
  - 0x15: Remote Device Terminated Connection due to Power Off
  - 0x16: Connection Terminated By Local Host
  - 0x17: Repeated Attempts
  - 0x18: Pairing Not Allowed
  - 0x19: Unknown LMP PDU
  - 0x1A: Unsupported Remote Feature / Unsupported LMP Feature
  - 0x1B: SCO Offset Rejected
  - 0x1C: SCO Interval Rejected
  - 0x1D: SCO Air Mode Rejected
  - 0x1E: Invalid LMP Parameters
  - 0x1F: Unspecified Error
  - 0x20: Unsupported LMP Parameter Value
  - 0x21: Role Change Not Allowed
  - 0x22: LMP Response Timeout / LL Response Timeout
  - 0x23: LMP Error Transaction Collision
  - 0x24: LMP PDU Not Allowed
  - 0x25: Encryption Mode Not Acceptable
  - 0x26: Link Key cannot be Changed
  - 0x27: Requested QoS Not Supported
  - 0x28: Instant Passed
  - 0x29: Pairing With Unit Key Not Supported
  - 0x2A: Different Transaction Collision
  - 0x2C: QoS Unacceptable Parameter
  - 0x2D: QoS Rejected
  - 0x2E: Channel Assessment Not Supported
  - 0x2F: Insufficient Security
  - 0x30: Parameter Out Of Mandatory Range
  - 0x32: Role Switch Pending
  - 0x34: Reserved Slot Violation
  - 0x35: Role Switch Failed
  - 0x36: Extended Inquiry Response Too Large
  - 0x37: Secure Simple Pairing Not Supported by Host
  - 0x38: Host Busy - Pairing
  - 0x39: Connection Rejected due to No Suitable Channel Found
  - 0x3A: Controller Busy
  - 0x3B: Unacceptable Connection Interval
  - 0x3C: Directed Advertising Timeout
  - 0x3D: Connection Terminated Due to MIC Failure
  - 0x3E: Connection Failed to be Established
  - 0x3F: MAC of the 802.11 AMP
  - 0x41: Failed
  - 0x42: Invalid parameters
  - 0x43: Busy
  - 0x44: Invalid length
  - 0x45: Pending
  - 0x46: Not allowed
  - 0x47: GATT error
  - 0x48: Address not resolved
  - 0x50: Invalid CID
  - 0x5A: CSRK not found
  - 0x5B: IRK not found
  - 0x5C: Device not found in DB
  - 0x5D: Security DB full
  - 0x5E: Device not bonded
  - 0x5F: Device in blacklist
  - 0x60: Invalid handle
  - 0x61: Invalid parameter
  - 0x62: Out of handles
  - 0x63: Invalid operation
  - 0x64: Insufficient resources
  - 0x65: Insufficient encryption key size
  - 0x66: Characteristic already exist
  - 0x82: No valid slot
  - 0x83: Short window
  - 0x84: New interval failed
  - 0x85: Too large interval
  - 0x86: Slot length failed
  - 0xFA: Flash read failed
  - 0xFB: Flash write failed
  - 0xFC: Flash erase failed
  * @retval None
*/
void hci_disconnection_complete_event(uint8_t Status,
                                      uint16_t Connection_Handle,
                                      uint8_t Reason)
{
  uint8_t buffer_out[258];
  /* Output params */
  hci_disconnection_complete_event_rp0 *rp0 = (hci_disconnection_complete_event_rp0 *) (buffer_out + 3);
  rp0->Status = Status;
  rp0->Connection_Handle = Connection_Handle;
  rp0->Reason = Reason;
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x05;
  buffer_out[2] = 1 + 2 + 1;
  rcv_callback(buffer_out, buffer_out[2] + 3);
}
/* hci_encryption_change_event */
/* Event len: 1 + 2 + 1 */
/**
  * @brief The Encryption Change event is used to indicate that the change of the encryption
mode has been completed. The Connection_Handle will be a Connection_Handle
for an ACL connection. The Encryption_Enabled event parameter
specifies the new Encryption_Enabled parameter for the Connection_Handle
specified by the Connection_Handle event parameter. This event will occur on
both devices to notify the Hosts when Encryption has changed for the specified
Connection_Handle between two devices. Note: This event shall not be generated
if encryption is paused or resumed; during a role switch, for example.
The meaning of the Encryption_Enabled parameter depends on whether the
Host has indicated support for Secure Connections in the Secure_Connections_Host_Support
parameter. When Secure_Connections_Host_Support is
'disabled' or the Connection_Handle refers to an LE link, the Controller shall
only use Encryption_Enabled values 0x00 (OFF) and 0x01 (ON).
(See Bluetooth Specification v.4.1, Vol. 2, Part E, 7.7.8)
  * @param Status For standard error codes see Bluetooth specification, Vol. 2, part D. For proprietary error code refer to Error codes section
  * @param Connection_Handle Connection handle for which the command is given.
  * Values:
  - 0x0000 ... 0x0EFF
  * @param Encryption_Enabled Link Level Encryption.
  * Values:
  - 0x00: Link Level Encryption OFF
  - 0x01: Link Level Encryption is ON with AES-CCM
  * @retval None
*/
void hci_encryption_change_event(uint8_t Status,
                                 uint16_t Connection_Handle,
                                 uint8_t Encryption_Enabled)
{
  uint8_t buffer_out[258];
  /* Output params */
  hci_encryption_change_event_rp0 *rp0 = (hci_encryption_change_event_rp0 *) (buffer_out + 3);
  rp0->Status = Status;
  rp0->Connection_Handle = Connection_Handle;
  rp0->Encryption_Enabled = Encryption_Enabled;
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x08;
  buffer_out[2] = 1 + 2 + 1;
  rcv_callback(buffer_out, buffer_out[2] + 3);
}
/* hci_read_remote_version_information_complete_event */
/* Event len: 1 + 2 + 1 + 2 + 2 */
/**
  * @brief The Read Remote Version Information Complete event is used to indicate the
completion of the process obtaining the version information of the remote Controller
specified by the Connection_Handle event parameter. The Connection_Handle
shall be for an ACL connection.
The Version event parameter defines the specification version of the LE Controller.
The Manufacturer_Name event parameter indicates the manufacturer
of the remote Controller. The Subversion event parameter is controlled
by the manufacturer and is implementation dependent. The Subversion
event parameter defines the various revisions that each version of the Bluetooth
hardware will go through as design processes change and errors are
fixed. This allows the software to determine what Bluetooth hardware is being
used and, if necessary, to work around various bugs in the hardware.
When the Connection_Handle is associated with an LE-U logical link, the Version
event parameter shall be Link Layer VersNr parameter, the Manufacturer_Name
event parameter shall be the CompId parameter, and the Subversion
event parameter shall be the SubVersNr parameter.
(See Bluetooth Specification v.4.1, Vol. 2, Part E, 7.7.12)
  * @param Status For standard error codes see Bluetooth specification, Vol. 2, part D. For proprietary error code refer to Error codes section
  * @param Connection_Handle Connection handle for which the command is given.
  * Values:
  - 0x0000 ... 0x0EFF
  * @param Version Version of the Current LMP in the remote Controller
  * @param Manufacturer_Name Manufacturer Name of the remote Controller
  * @param Subversion Subversion of the LMP in the remote Controller
  * @retval None
*/
void hci_read_remote_version_information_complete_event(uint8_t Status,
                                                        uint16_t Connection_Handle,
                                                        uint8_t Version,
                                                        uint16_t Manufacturer_Name,
                                                        uint16_t Subversion)
{
  uint8_t buffer_out[258];
  /* Output params */
  hci_read_remote_version_information_complete_event_rp0 *rp0 = (hci_read_remote_version_information_complete_event_rp0 *) (buffer_out + 3);
  rp0->Status = Status;
  rp0->Connection_Handle = Connection_Handle;
  rp0->Version = Version;
  rp0->Manufacturer_Name = Manufacturer_Name;
  rp0->Subversion = Subversion;
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0c;
  buffer_out[2] = 1 + 2 + 1 + 2 + 2;
  rcv_callback(buffer_out, buffer_out[2] + 3);
}
/* hci_hardware_error_event */
/* Event len: 1 */
/**
  * @brief The Hardware Error event is used to indicate some implementation specific type of hardware failure for the controller. This event is used to notify the Host that a hardware failure has occurred in the Controller.
  * @param Hardware_Code Hardware Error Event code.
Error code 0x01 and 0x02 are errors generally caused by hardware issue on the PCB; another possible cause is a slow crystal startup.
In the latter case, the HS_STARTUP_TIME in the device configuration needs to be tuned.
Error code 0x03 indicates an internal error of the protocol stack.
After this event is recommended to force device reset.
  * Values:
  - 0x01: Radio state error
  - 0x02: Timer overrun error
  - 0x03: Internal queue overflow error
  * @retval None
*/
void hci_hardware_error_event(uint8_t Hardware_Code)
{
  uint8_t buffer_out[258];
  /* Output params */
  hci_hardware_error_event_rp0 *rp0 = (hci_hardware_error_event_rp0 *) (buffer_out + 3);
  rp0->Hardware_Code = Hardware_Code;
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x10;
  buffer_out[2] = 1;
  rcv_callback(buffer_out, buffer_out[2] + 3);
}
/* hci_number_of_completed_packets_event */
/* Event len: 1 + rp0->Number_of_Handles * (sizeof(Handle_Packets_Pair_Entry_t)) */
/**
  * @brief 'The Number Of Completed Packets event is used by the Controller to indicate
to the Host how many HCI Data Packets have been completed (transmitted or
flushed) for each Connection_Handle since the previous Number Of Completed
Packets event was sent to the Host. This means that the corresponding
buffer space has been freed in the Controller. Based on this information, and
the HC_Total_Num_ACL_Data_Packets and HC_Total_Num_Synchronous_-
Data_Packets return parameter of the Read_Buffer_Size command, the Host
can determine for which Connection_Handles the following HCI Data Packets
should be sent to the Controller. The Number Of Completed Packets event
must not be sent before the corresponding Connection Complete event. While
the Controller has HCI data packets in its buffer, it must keep sending the Number
Of Completed Packets event to the Host at least periodically, until it finally
reports that all the pending ACL Data Packets have been transmitted or
flushed.
  * @param Number_of_Handles The number of Connection_Handles and Num_HCI_Data_Packets parameters pairs contained in this event
  * @param Handle_Packets_Pair_Entry See @ref Handle_Packets_Pair_Entry_t
  * @retval None
*/
void hci_number_of_completed_packets_event(uint8_t Number_of_Handles,
                                           Handle_Packets_Pair_Entry_t Handle_Packets_Pair_Entry[])
{
  uint8_t buffer_out[258];
  int output_size = 0;
  /* Output params */
  hci_number_of_completed_packets_event_rp0 *rp0 = (hci_number_of_completed_packets_event_rp0 *) (buffer_out + 3);
  rp0->Number_of_Handles = Number_of_Handles;
  {
    int i;
    for (i = 0; i < Number_of_Handles; i++) {
      rp0->Handle_Packets_Pair_Entry[i].Connection_Handle = Handle_Packets_Pair_Entry[i].Connection_Handle;
      output_size += sizeof(uint16_t);
      rp0->Handle_Packets_Pair_Entry[i].HC_Num_Of_Completed_Packets = Handle_Packets_Pair_Entry[i].HC_Num_Of_Completed_Packets;
      output_size += sizeof(uint16_t);
    }
  }
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x13;
  buffer_out[2] = 1 + output_size;
  rcv_callback(buffer_out, buffer_out[2] + 3);
}
/* hci_data_buffer_overflow_event */
/* Event len: 1 */
/**
  * @brief 'This event is used to indicate that the Controller's data buffers have been overflowed.
This can occur if the Host has sent more packets than allowed. The
Link_Type parameter is used to indicate that the overflow was caused by ACL data.
  * @param Link_Type On wich type of channel overflow has occurred.
  * Values:
  - 0x01: ACL Buffer Overflow
  * @retval None
*/
void hci_data_buffer_overflow_event(uint8_t Link_Type)
{
  uint8_t buffer_out[258];
  /* Output params */
  hci_data_buffer_overflow_event_rp0 *rp0 = (hci_data_buffer_overflow_event_rp0 *) (buffer_out + 3);
  rp0->Link_Type = Link_Type;
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x1a;
  buffer_out[2] = 1;
  rcv_callback(buffer_out, buffer_out[2] + 3);
}
/* hci_encryption_key_refresh_complete_event */
/* Event len: 1 + 2 */
/**
  * @brief 'The Encryption Key Refresh Complete event is used to indicate to the Host
that the encryption key was refreshed on the given Connection_Handle any
time encryption is paused and then resumed.
If the Encryption Key Refresh Complete event was generated due to an
encryption pause and resume operation embedded within a change connection
link key procedure, the Encryption Key Refresh Complete event shall be sent
prior to the Change Connection Link Key Complete event.
If the Encryption Key Refresh Complete event was generated due to an
encryption pause and resume operation embedded within a role switch procedure,
the Encryption Key Refresh Complete event shall be sent prior to the
Role Change event.
  * @param Status For standard error codes see Bluetooth specification, Vol. 2, part D. For proprietary error code refer to Error codes section
  * @param Connection_Handle Connection handle for which the command is given.
  * Values:
  - 0x0000 ... 0x0EFF
  * @retval None
*/
void hci_encryption_key_refresh_complete_event(uint8_t Status,
                                               uint16_t Connection_Handle)
{
  uint8_t buffer_out[258];
  /* Output params */
  hci_encryption_key_refresh_complete_event_rp0 *rp0 = (hci_encryption_key_refresh_complete_event_rp0 *) (buffer_out + 3);
  rp0->Status = Status;
  rp0->Connection_Handle = Connection_Handle;
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x30;
  buffer_out[2] = 1 + 2;
  rcv_callback(buffer_out, buffer_out[2] + 3);
}

/* hci_le_connection_complete_event */
/* Event len: 1 + 2 + 1 + 1 + 6 + 2 + 2 + 2 + 1 */
/**
  * @brief The LE Connection Complete event indicates to both of the Hosts forming the
connection that a new connection has been created. Upon the creation of the
connection a Connection_Handle shall be assigned by the Controller, and
passed to the Host in this event. If the connection establishment fails this event
shall be provided to the Host that had issued the LE_Create_Connection command.
This event indicates to the Host which issued a LE_Create_Connection
command and received a Command Status event if the connection
establishment failed or was successful.
The Master_Clock_Accuracy parameter is only valid for a slave. On a master,
this parameter shall be set to 0x00.
  * @param Status For standard error codes see Bluetooth specification, Vol. 2, part D. For proprietary error code refer to Error codes section
  * @param Connection_Handle Connection handle to be used to identify the connection with the peer device.
  * Values:
  - 0x0000 ... 0x0EFF
  * @param Role Role of the local device in the connection.
  * Values:
  - 0x00: Master
  - 0x01: Slave
  * @param Peer_Address_Type The address type of the peer device.
  * Values:
  - 0x00: Public Device Address
  - 0x01: Random Device Address
  * @param Peer_Address Public Device Address or Random Device Address of the peer
device
  * @param Conn_Interval Connection interval used on this connection.
Time = N * 1.25 msec
  * Values:
  - 0x0006 (7.50 ms)  ... 0x0C80 (4000.00 ms) 
  * @param Conn_Latency Slave latency for the connection in number of connection events.
  * Values:
  - 0x0000 ... 0x01F3
  * @param Supervision_Timeout Supervision timeout for the LE Link.
It shall be a multiple of 10 ms and larger than (1 + connSlaveLatency) * connInterval * 2.
Time = N * 10 msec.
  * Values:
  - 0x000A (100 ms)  ... 0x0C80 (32000 ms) 
  * @param Master_Clock_Accuracy Master clock accuracy. Only valid for a slave.
  * Values:
  - 0x00: 500 ppm
  - 0x01: 250 ppm
  - 0x02: 150 ppm
  - 0x03: 100 ppm
  - 0x04: 75 ppm
  - 0x05: 50 ppm
  - 0x06: 30 ppm
  - 0x07: 20 ppm
  * @retval None
*/
void hci_le_connection_complete_event(uint8_t Status,
                                      uint16_t Connection_Handle,
                                      uint8_t Role,
                                      uint8_t Peer_Address_Type,
                                      uint8_t Peer_Address[6],
                                      uint16_t Conn_Interval,
                                      uint16_t Conn_Latency,
                                      uint16_t Supervision_Timeout,
                                      uint8_t Master_Clock_Accuracy)
{
  uint8_t buffer_out[258];
  // Output params
  hci_le_connection_complete_event_rp0 *rp0 = (hci_le_connection_complete_event_rp0 *) (buffer_out + 4);
  rp0->Status = Status;
  rp0->Connection_Handle = Connection_Handle;
  rp0->Role = Role;
  rp0->Peer_Address_Type = Peer_Address_Type;
  Osal_MemCpy((void *) rp0->Peer_Address,(const void *) Peer_Address, 6);
  rp0->Conn_Interval = Conn_Interval;
  rp0->Conn_Latency = Conn_Latency;
  rp0->Supervision_Timeout = Supervision_Timeout;
  rp0->Master_Clock_Accuracy = Master_Clock_Accuracy;
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x3E;
  buffer_out[2] = 1 + 2 + 1 + 1 + 6 + 2 + 2 + 2 + 1 + 1;
  buffer_out[3] = 0x01;
  rcv_callback(buffer_out, buffer_out[2] + 3);
}

/* hci_le_advertising_report_event */
/* Event len: 1 + rp0->Num_Reports * (sizeof(Advertising_Report_t)) */
/**
  * @brief The LE Advertising Report event indicates that a Bluetooth device or multiple
Bluetooth devices have responded to an active scan or received some information
during a passive scan. The Controller may queue these advertising reports
and send information from multiple devices in one LE Advertising Report event.
  * @param Num_Reports Number of responses in this event.
  * Values:
  - 0x01
  * @param Advertising_Report See @ref Advertising_Report_t
  * @retval None
*/
void hci_le_advertising_report_event(uint8_t Num_Reports,
                                     Advertising_Report_t Advertising_Report[])
{
  uint8_t buffer_out[258];
  int output_size = 0;
  /* Output params */
  hci_le_advertising_report_event_rp0 *rp0 = (hci_le_advertising_report_event_rp0 *) (buffer_out + 4);
  rp0->Num_Reports = Num_Reports;
  {
    int i;
    for (i = 0; i < Num_Reports; i++) {
      hci_le_advertising_report_event_rp0 *rp0_tmp = (hci_le_advertising_report_event_rp0 *) (((uint8_t *) rp0) + output_size);
      rp0_tmp->Advertising_Report[0].Event_Type = Advertising_Report[i].Event_Type;
      output_size += sizeof(uint8_t);
      rp0_tmp->Advertising_Report[0].Address_Type = Advertising_Report[i].Address_Type;
      output_size += sizeof(uint8_t);
      Osal_MemCpy((void *) &rp0_tmp->Advertising_Report[0].Address, (const void *) Advertising_Report[i].Address, 6);
      output_size += 6;
      rp0_tmp->Advertising_Report[0].Length_Data = Advertising_Report[i].Length_Data;
      output_size += sizeof(uint8_t);
      Osal_MemCpy((void *) &rp0_tmp->Advertising_Report[0].Data, (const void *) Advertising_Report[i].Data, Advertising_Report[i].Length_Data);
      output_size += Advertising_Report[i].Length_Data;
      Osal_MemCpy((void *) &rp0_tmp->Advertising_Report[0].Data[Advertising_Report[i].Length_Data], (const void *) &Advertising_Report[i].RSSI, 1);
      output_size += 1;
    }
  }
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x3E;
  buffer_out[2] = 1 + output_size + 1;
  buffer_out[3] = 0x02;
  rcv_callback(buffer_out, buffer_out[2] + 3);
}
/* hci_le_connection_update_complete_event */
/* Event len: 1 + 2 + 2 + 2 + 2 */
/**
  * @brief The LE Connection Update Complete event is used to indicate that the Controller
process to update the connection has completed.
On a slave, if no connection parameters are updated, then this event shall not be issued.
On a master, this event shall be issued if the Connection_Update command was sent.
  * @param Status For standard error codes see Bluetooth specification, Vol. 2, part D. For proprietary error code refer to Error codes section
  * @param Connection_Handle Connection handle to be used to identify the connection with the peer device.
  * Values:
  - 0x0000 ... 0x0EFF
  * @param Conn_Interval Connection interval used on this connection.
Time = N * 1.25 msec
  * Values:
  - 0x0006 (7.50 ms)  ... 0x0C80 (4000.00 ms) 
  * @param Conn_Latency Slave latency for the connection in number of connection events.
  * Values:
  - 0x0000 ... 0x01F3
  * @param Supervision_Timeout Supervision timeout for the LE Link.
It shall be a multiple of 10 ms and larger than (1 + connSlaveLatency) * connInterval * 2.
Time = N * 10 msec.
  * Values:
  - 0x000A (100 ms)  ... 0x0C80 (32000 ms) 
  * @retval None
*/
void hci_le_connection_update_complete_event(uint8_t Status,
                                             uint16_t Connection_Handle,
                                             uint16_t Conn_Interval,
                                             uint16_t Conn_Latency,
                                             uint16_t Supervision_Timeout)
{
  uint8_t buffer_out[258];
  /* Output params */
  hci_le_connection_update_complete_event_rp0 *rp0 = (hci_le_connection_update_complete_event_rp0 *) (buffer_out + 4);
  rp0->Status = Status;
  rp0->Connection_Handle = Connection_Handle;
  rp0->Conn_Interval = Conn_Interval;
  rp0->Conn_Latency = Conn_Latency;
  rp0->Supervision_Timeout = Supervision_Timeout;
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x3E;
  buffer_out[2] = 1 + 2 + 2 + 2 + 2 + 1;
  buffer_out[3] = 0x03;
  rcv_callback(buffer_out, buffer_out[2] + 3);
}
/* hci_le_read_remote_used_features_complete_event */
/* Event len: 1 + 2 + 8 */
/**
  * @brief The LE Read Remote Used Features Complete event is used to indicate the
completion of the process of the Controller obtaining the used features of the
remote Bluetooth device specified by the Connection_Handle event parameter.
  * @param Status For standard error codes see Bluetooth specification, Vol. 2, part D. For proprietary error code refer to Error codes section
  * @param Connection_Handle Connection handle to be used to identify the connection with the peer device.
  * Values:
  - 0x0000 ... 0x0EFF
  * @param LE_Features Bit Mask List of used LE features. For details see LE Link Layer specification.
  * @retval None
*/
void hci_le_read_remote_used_features_complete_event(uint8_t Status,
                                                     uint16_t Connection_Handle,
                                                     uint8_t LE_Features[8])
{
  uint8_t buffer_out[258];
  /* Output params */
  hci_le_read_remote_used_features_complete_event_rp0 *rp0 = (hci_le_read_remote_used_features_complete_event_rp0 *) (buffer_out + 4);
  rp0->Status = Status;
  rp0->Connection_Handle = Connection_Handle;
  Osal_MemCpy((void *) rp0->LE_Features,(const void *) LE_Features, 8);
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x3E;
  buffer_out[2] = 1 + 2 + 8 + 1;
  buffer_out[3] = 0x04;
  rcv_callback(buffer_out, buffer_out[2] + 3);
}
/* hci_le_long_term_key_request_event */
/* Event len: 2 + 8 + 2 */
/**
  * @brief The LE Long Term Key Request event indicates that the master device is
attempting to encrypt or re-encrypt the link and is requesting the Long Term
Key from the Host. (See [Vol 6] Part B, Section 5.1.3).
  * @param Connection_Handle Connection handle to be used to identify the connection with the peer device.
  * Values:
  - 0x0000 ... 0x0EFF
  * @param Random_Number 64-bit random number
  * @param Encrypted_Diversifier 16-bit encrypted diversifier
  * @retval None
*/
void hci_le_long_term_key_request_event(uint16_t Connection_Handle,
                                        uint8_t Random_Number[8],
                                        uint16_t Encrypted_Diversifier)
{
  uint8_t buffer_out[258];
  /* Output params */
  hci_le_long_term_key_request_event_rp0 *rp0 = (hci_le_long_term_key_request_event_rp0 *) (buffer_out + 4);
  rp0->Connection_Handle = Connection_Handle;
  Osal_MemCpy((void *) rp0->Random_Number,(const void *) Random_Number, 8);
  rp0->Encrypted_Diversifier = Encrypted_Diversifier;
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x3E;
  buffer_out[2] = 2 + 8 + 2 + 1;
  buffer_out[3] = 0x05;
  rcv_callback(buffer_out, buffer_out[2] + 3);
}
/* hci_le_data_length_change_event */
/* Event len: 2 + 2 + 2 + 2 + 2 */
/**
  * @brief The LE Data Length Change event notifies the Host of a change to either the maximum Payload length or the maximum transmission time of Data Channel PDUs in either direction. The values reported are the maximum that will actually be used on the connection following the change. 
  * @param Connection_Handle Connection_Handle to be used to identify a connection.
  * @param MaxTxOctets The maximum number of payload octets in a Link Layer Data Channel PDU that the local Controller will send on this connection (connEffectiveMaxTxOctets defined in [Vol 6] Part B, Section 4.5.10).
Range 0x001B-0x00FB (0x0000 - 0x001A and 0x00FC - 0xFFFF Reserved for future use)
  * @param MaxTxTime The maximum time that the local Controller will take to send a Link Layer Data Channel PDU on this connection (connEffectiveMaxTx-Time defined in [Vol 6] Part B, Section 4.5.10).
Range 0x0148-0x0848 (0x0000 - 0x0127 and 0x0849 - 0xFFFF
Reserved for future use)
  * @param MaxRxOctets The maximum number of payload octets in a Link Layer Data Channel PDU that the local controller expects to receive on this connection (connEfectiveMaxRxOctets defined in [Vol 6] Part B, Section 4.5.10).
Range 0x001B-0x00FB (0x0000 - 0x001A and 0x00FC - 0xFFFF Reserved for future use)
  * @param MaxRxTime The maximum time that the local Controller expects to take to receive a Link Layer Data Channel PDU on this connection (connEffectiveMax-RxTime defined in [Vol 6] Part B, Section 4.5.10).
Range 0x0148-0x0848 (0x0000 - 0x0127 and 0x0849 - 0xFFFF Reserved for future use)
  * @retval None
*/
void hci_le_data_length_change_event(uint16_t Connection_Handle,
                                     uint16_t MaxTxOctets,
                                     uint16_t MaxTxTime,
                                     uint16_t MaxRxOctets,
                                     uint16_t MaxRxTime)
{
  uint8_t buffer_out[258];
  /* Output params */
  hci_le_data_length_change_event_rp0 *rp0 = (hci_le_data_length_change_event_rp0 *) (buffer_out + 4);
  rp0->Connection_Handle = Connection_Handle;
  rp0->MaxTxOctets = MaxTxOctets;
  rp0->MaxTxTime = MaxTxTime;
  rp0->MaxRxOctets = MaxRxOctets;
  rp0->MaxRxTime = MaxRxTime;
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x3E;
  buffer_out[2] = 2 + 2 + 2 + 2 + 2 + 1;
  buffer_out[3] = 0x07;
  rcv_callback(buffer_out, buffer_out[2] + 3);
}
/* hci_le_read_local_p256_public_key_complete_event */
/* Event len: 1 + 64 */
/**
  * @brief This event is generated when local P-256 key generation is complete.
  * @param Status For standard error codes see Bluetooth specification, Vol. 2, part D. For proprietary error code refer to Error codes section
  * @param Local_P256_Public_Key Local P-256 public key.
  * @retval None
*/
void hci_le_read_local_p256_public_key_complete_event(uint8_t Status,
                                                      uint8_t Local_P256_Public_Key[64])
{
  uint8_t buffer_out[258];
  /* Output params */
  hci_le_read_local_p256_public_key_complete_event_rp0 *rp0 = (hci_le_read_local_p256_public_key_complete_event_rp0 *) (buffer_out + 4);
  rp0->Status = Status;
  Osal_MemCpy((void *) rp0->Local_P256_Public_Key,(const void *) Local_P256_Public_Key, 64);
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x3E;
  buffer_out[2] = 1 + 64 + 1;
  buffer_out[3] = 0x08;
  rcv_callback(buffer_out, buffer_out[2] + 3);
}
/* hci_le_generate_dhkey_complete_event */
/* Event len: 1 + 32 */
/**
  * @brief This event indicates that LE Diffie Hellman key generation has been completed
by the Controller.
  * @param Status For standard error codes see Bluetooth specification, Vol. 2, part D. For proprietary error code refer to Error codes section
  * @param DHKey Diffie Hellman Key
  * @retval None
*/
void hci_le_generate_dhkey_complete_event(uint8_t Status,
                                          uint8_t DHKey[32])
{
  uint8_t buffer_out[258];
  /* Output params */
  hci_le_generate_dhkey_complete_event_rp0 *rp0 = (hci_le_generate_dhkey_complete_event_rp0 *) (buffer_out + 4);
  rp0->Status = Status;
  Osal_MemCpy((void *) rp0->DHKey,(const void *) DHKey, 32);
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x3E;
  buffer_out[2] = 1 + 32 + 1;
  buffer_out[3] = 0x09;
  rcv_callback(buffer_out, buffer_out[2] + 3);
}
/* hci_le_enhanced_connection_complete_event */
/* Event len: 1 + 2 + 1 + 1 + 6 + 6 + 6 + 2 + 2 + 2 + 1 */
/**
  * @brief The LE Enhanced Connection Complete event indicates to both of the Hosts
forming the connection that a new connection has been created. Upon the
creation of the connection a Connection_Handle shall be assigned by the
Controller, and passed to the Host in this event. If the connection establishment
fails, this event shall be provided to the Host that had issued the
LE_Create_Connection command.
If this event is unmasked and LE Connection Complete event is unmasked,
only the LE Enhanced Connection Complete event is sent when a new
connection has been completed.
This event indicates to the Host that issued a LE_Create_Connection
command and received a Command Status event if the connection
establishment failed or was successful.
The Master_Clock_Accuracy parameter is only valid for a slave. On a master,
this parameter shall be set to 0x00.
  * @param Status For standard error codes see Bluetooth specification, Vol. 2, part D. For proprietary error code refer to Error codes section
  * @param Connection_Handle Connection handle to be used to identify the connection with the peer device.
  * Values:
  - 0x0000 ... 0x0EFF
  * @param Role Role of the local device in the connection.
  * Values:
  - 0x00: Master
  - 0x01: Slave
  * @param Peer_Address_Type 0x00 Public Device Address
0x01 Random Device Address
0x02 Public Identity Address (Corresponds to Resolved Private Address)
0x03 Random (Static) Identity Address (Corresponds to Resolved Private Address)
  * Values:
  - 0x00: Public Device Address
  - 0x01: Random Device Address
  - 0x02: Public Identity Address
  - 0x03: Random (Static) Identity Address
  * @param Peer_Address Public Device Address, Random Device Address, Public Identity
Address or Random (static) Identity Address of the device to be connected.
  * @param Local_Resolvable_Private_Address Resolvable Private Address being used by the local device for this connection.
This is only valid when the Own_Address_Type is set to 0x02 or 0x03. For other Own_Address_Type values,
the Controller shall return all zeros.
  * @param Peer_Resolvable_Private_Address Resolvable Private Address being used by the peer device for this connection.
This is only valid for Peer_Address_Type 0x02 and 0x03. For
other Peer_Address_Type values, the Controller shall return all zeros.
  * @param Conn_Interval Connection interval used on this connection.
Time = N * 1.25 msec
  * Values:
  - 0x0006 (7.50 ms)  ... 0x0C80 (4000.00 ms) 
  * @param Conn_Latency Slave latency for the connection in number of connection events.
  * Values:
  - 0x0000 ... 0x01F3
  * @param Supervision_Timeout Supervision timeout for the LE Link.
It shall be a multiple of 10 ms and larger than (1 + connSlaveLatency) * connInterval * 2.
Time = N * 10 msec.
  * Values:
  - 0x000A (100 ms)  ... 0x0C80 (32000 ms) 
  * @param Master_Clock_Accuracy Master clock accuracy. Only valid for a slave.
  * Values:
  - 0x00: 500 ppm
  - 0x01: 250 ppm
  - 0x02: 150 ppm
  - 0x03: 100 ppm
  - 0x04: 75 ppm
  - 0x05: 50 ppm
  - 0x06: 30 ppm
  - 0x07: 20 ppm
  * @retval None
*/
void hci_le_enhanced_connection_complete_event(uint8_t Status,
                                               uint16_t Connection_Handle,
                                               uint8_t Role,
                                               uint8_t Peer_Address_Type,
                                               uint8_t Peer_Address[6],
                                               uint8_t Local_Resolvable_Private_Address[6],
                                               uint8_t Peer_Resolvable_Private_Address[6],
                                               uint16_t Conn_Interval,
                                               uint16_t Conn_Latency,
                                               uint16_t Supervision_Timeout,
                                               uint8_t Master_Clock_Accuracy)
{
  uint8_t buffer_out[258];
  /* Output params */
  hci_le_enhanced_connection_complete_event_rp0 *rp0 = (hci_le_enhanced_connection_complete_event_rp0 *) (buffer_out + 4);
  rp0->Status = Status;
  rp0->Connection_Handle = Connection_Handle;
  rp0->Role = Role;
  rp0->Peer_Address_Type = Peer_Address_Type;
  Osal_MemCpy((void *) rp0->Peer_Address,(const void *) Peer_Address, 6);
  Osal_MemCpy((void *) rp0->Local_Resolvable_Private_Address,(const void *) Local_Resolvable_Private_Address, 6);
  Osal_MemCpy((void *) rp0->Peer_Resolvable_Private_Address,(const void *) Peer_Resolvable_Private_Address, 6);
  rp0->Conn_Interval = Conn_Interval;
  rp0->Conn_Latency = Conn_Latency;
  rp0->Supervision_Timeout = Supervision_Timeout;
  rp0->Master_Clock_Accuracy = Master_Clock_Accuracy;
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x3E;
  buffer_out[2] = 1 + 2 + 1 + 1 + 6 + 6 + 6 + 2 + 2 + 2 + 1 + 1;
  buffer_out[3] = 0x0a;
  rcv_callback(buffer_out, buffer_out[2] + 3);
}
/* hci_le_direct_advertising_report_event */
/* Event len: 1 + rp0->Num_Reports * (sizeof(Direct_Advertising_Report_t)) */
/**
  * @brief The LE Direct Advertising Report event indicates that directed advertisements
have been received where the advertiser is using a resolvable private address
for the InitA field in the ADV_DIRECT_IND PDU and the
Scanning_Filter_Policy is equal to 0x02 or 0x03, see HCI_LE_Set_Scan_Parameters.
Direct_Address_Type and Direct_Addres is the address the directed
advertisements are being directed to. Address_Type and Address is the
address of the advertiser sending the directed advertisements.
  * @param Num_Reports Number of responses in this event.
  * Values:
  - 0x01
  * @param Direct_Advertising_Report See @ref Direct_Advertising_Report_t
  * @retval None
*/
void hci_le_direct_advertising_report_event(uint8_t Num_Reports,
                                            Direct_Advertising_Report_t Direct_Advertising_Report[])
{
  uint8_t buffer_out[258];
  int output_size = 0;
  /* Output params */
  hci_le_direct_advertising_report_event_rp0 *rp0 = (hci_le_direct_advertising_report_event_rp0 *) (buffer_out + 4);
  rp0->Num_Reports = Num_Reports;
  {
    int i;
    for (i = 0; i < Num_Reports; i++) {
      rp0->Direct_Advertising_Report[i].Event_Type = Direct_Advertising_Report[i].Event_Type;
      output_size += sizeof(uint8_t);
      rp0->Direct_Advertising_Report[i].Address_Type = Direct_Advertising_Report[i].Address_Type;
      output_size += sizeof(uint8_t);
      Osal_MemCpy((void *) &rp0->Direct_Advertising_Report[i].Address, (const void *) Direct_Advertising_Report[i].Address, 6);
      output_size += 6;
      rp0->Direct_Advertising_Report[i].Direct_Address_Type = Direct_Advertising_Report[i].Direct_Address_Type;
      output_size += sizeof(uint8_t);
      Osal_MemCpy((void *) &rp0->Direct_Advertising_Report[i].Direct_Address, (const void *) Direct_Advertising_Report[i].Direct_Address, 6);
      output_size += 6;
      rp0->Direct_Advertising_Report[i].RSSI = Direct_Advertising_Report[i].RSSI;
      output_size += sizeof(uint8_t);
    }
  }
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x3E;
  buffer_out[2] = 1 + output_size + 1;
  buffer_out[3] = 0x0b;
  rcv_callback(buffer_out, buffer_out[2] + 3);
}

/* Command len: 1 + 1 + cp0->Length * (sizeof(uint8_t)) */
/* Response len: 1 */
uint16_t aci_hal_write_config_data_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{
  /* Input params */
  aci_hal_write_config_data_cp0 *cp0 = (aci_hal_write_config_data_cp0 *)(buffer_in + 1 + (0));

  int output_size = 1;
  /* Output params */
  uint8_t *status = (uint8_t *) (buffer_out + 6);

  if (buffer_out_max_length < (1 + 6)) { return 0; }
  *status = aci_hal_write_config_data(cp0->Offset /* 1 */,
                                      cp0->Length /* 1 */,
                                      cp0->Value /* cp0->Length * (sizeof(uint8_t)) */);
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0E;
  buffer_out[2] = output_size + 3;
  buffer_out[3] = 0x01;
  buffer_out[4] = 0x0c;
  buffer_out[5] = 0xfc;
  return (output_size + 6);
}

/* Command len: 1 */
/* Response len: 1 + 1 + rp0->Data_Length * (sizeof(uint8_t)) */
uint16_t aci_hal_read_config_data_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length)
{
  /* Input params */
  aci_hal_read_config_data_cp0 *cp0 = (aci_hal_read_config_data_cp0 *)(buffer_in + 1 + (0));

  int output_size = 0;
  /* Output params */
  aci_hal_read_config_data_rp0 *rp0 = (aci_hal_read_config_data_rp0 *) (buffer_out + 6);
  uint8_t Data_Length = 0;
  uint8_t Data[HCI_MAX_PAYLOAD_SIZE/sizeof(uint8_t)];

  if (buffer_out_max_length < (1 + 6)) { return 0; }
  rp0->Status = aci_hal_read_config_data(cp0->Offset /* 1 */,
                                         &Data_Length,
                                         Data);
  output_size += 1;
  if (buffer_out_max_length < (output_size + (1) + 6)) { return 0; }
  rp0->Data_Length = Data_Length;
  output_size += 1;
  if (buffer_out_max_length < (output_size + (rp0->Data_Length * (sizeof(uint8_t))) + 6)) { return 0; }
  Osal_MemCpy((void *) rp0->Data,(const void *) Data, rp0->Data_Length * (sizeof(uint8_t)));
  output_size += rp0->Data_Length * (sizeof(uint8_t));
  buffer_out[0] = 0x04;
  buffer_out[1] = 0x0E;
  buffer_out[2] = output_size + 3;
  buffer_out[3] = 0x01;
  buffer_out[4] = 0x0d;
  buffer_out[5] = 0xfc;
  return (output_size + 6);
}

tBleStatus hci_rx_acl_data_event(uint16_t connHandle, uint8_t  pb_flag, uint8_t  bc_flag, uint16_t  dataLen, uint8_t*  pduData)
{
  uint8_t buffer_out[251+5];

  buffer_out[0] = 0x02;
  buffer_out[1] = connHandle & 0xFF;
  buffer_out[2] = (connHandle >> 8 & 0x0F) | (pb_flag << 4) | (bc_flag << 6) ;
  Osal_MemCpy(buffer_out+3,&dataLen, 2);
  Osal_MemCpy(buffer_out+5, pduData, dataLen);
  rcv_callback(buffer_out, dataLen+2+2+1);
  return 0;
}

uint16_t hci_wrap_BTLE_StackTick_process(uint8_t *buffer_in, uint16_t buffer_in_length, uint8_t *buffer_out, uint16_t buffer_out_max_length){
	BTLE_StackTick();
	buffer_out[0] = 0x04;
	buffer_out[1] = 0x0F;
	buffer_out[2] = 0x04;
	buffer_out[3] = 0x00;  //00 command success <-
	buffer_out[4] = 0x01;
	buffer_out[5] = 0xFF;
	buffer_out[6] = 0xFC;
	return 7;              //the size of buffer_out
}
