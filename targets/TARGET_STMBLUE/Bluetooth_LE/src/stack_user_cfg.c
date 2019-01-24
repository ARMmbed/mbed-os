/**
  ******************************************************************************
  * @file    stack_user_cfg.c
  * @author  AMS - RF Application team
  * @version V1.0.0
  * @date    19 February 2018
  * @brief   BLE stack modular configuration options file
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

       - BLE stack basic configuration: no Controller Privacy, no LE Secure connections, no Master GAP role, no data length extension 
         - User is requested to add the following preprocessor option on project configuration:

           BLE_STACK_CONFIGURATION=BLE_STACK_BASIC_CONFIGURATION

      - BlueNRG-2, BLE stack v2.1 OTA Service Manager BLE stack configuration with extended packet length:

          BLE_STACK_CONFIGURATION=BLE_OTA_BASIC_CONFIGURATION

  - NOTE: default configuration is BLE_STACK_CONFIGURATION = BLE_STACK_BASIC_CONFIGURATION
**/

#ifdef __cplusplus            /////////////antonio
extern "C" {                  /////////////antonio
#endif						  /////////////antonio

#include "bluenrg1_api.h"
#include "hal_types.h"
#include "stack_user_cfg.h"

/* ---------------------------------------------------------------------------------------------------------- */

#ifdef CONTROLLER_PRIVACY_ENABLED
#if (CONTROLLER_PRIVACY_ENABLED == 1)

/* Internal APIs prototypes */
tBleStatus aci_gap_add_devices_to_resolving_list_api(uint8_t Num_of_Resolving_list_Entries, Whitelist_Identity_Entry_t Whitelist_Identity_Entry[], uint8_t Clear_Resolving_List);
tBleStatus hci_le_remove_device_from_resolving_list_api(uint8_t Peer_Identity_Address_Type, uint8_t Peer_Identity_Address[6]);
tBleStatus hci_le_add_device_to_resolving_list_api(uint8_t Peer_Identity_Address_Type, uint8_t Peer_Identity_Address[6], uint8_t Peer_IRK[16], uint8_t Local_IRK[16]);
tBleStatus hci_le_set_resolvable_private_address_timeout_api(uint16_t RpaTimeout);
tBleStatus hci_le_set_address_resolution_enable_api(uint8_t AddressResolutionEnable);
tBleStatus hci_le_read_peer_resolvable_address_api(uint8_t Peer_Identity_Address_Type, uint8_t Peer_Identity_Address[6], uint8_t Peer_Resolvable_Address[6]);
tBleStatus hci_le_read_local_resolvable_address_api(uint8_t Peer_Identity_Address_Type, uint8_t Peer_Identity_Address[6], uint8_t Local_Resolvable_Address[6]);
tBleStatus hci_le_read_resolving_list_size_api(uint8_t *resolving_List_Size);
tBleStatus hci_le_clear_resolving_list_api(void);
tBleStatus hci_le_set_privacy_mode_api(uint8_t Peer_Identity_Address_Type, uint8_t Peer_Identity_Address[6], uint8_t Privacy_Mode);

/* Internal core function prototypes */
void    PRIV_vGeneratePrivateAddress(uint8_t *IRK, uint8_t *PrivateAddress);
uint8_t LL_LE_Set_Resolvable_Private_Address_Timeout(uint32_t RpaTimeout);
uint8_t LL_SetAddressResolutionEnable(uint8_t enable);
uint8_t PRIV_u32ProcessAdvPacket(void *params);
uint8_t PRIV_u8OfflineProcessing(void);
void    PRIV_vGenerate_hci_le_enhanced_connection_complete_event(uint8_t role, uint32_t *peer_addr, uint8_t status, uint16_t connHandle,
																 uint16_t interval, uint16_t latency, uint32_t supertimeout, uint16_t CONN_Sca);

BOOL PRIV_vGenerate_hci_le_direct_advertising_report_event(uint8_t scanFilterPolicy, BOOL adv_direct_add_rpa, uint8_t scantype, uint8_t peer_addr_type, uint8_t* p1, uint8_t rssi, uint8_t length);
tBleStatus GAP_enable_controller_privacy(uint8_t *gapRole, uint8_t *numServiceRec);
tBleStatus LL_Encrypt_Plain_Data(uint8_t *key, uint8_t *textData, uint8_t *textDataOut, BOOL inISR);

/* APIs definitions */
tBleStatus aci_gap_add_devices_to_resolving_list(uint8_t Num_of_Resolving_list_Entries, Whitelist_Identity_Entry_t Whitelist_Identity_Entry[], uint8_t Clear_Resolving_List)
{
    return aci_gap_add_devices_to_resolving_list_api(Num_of_Resolving_list_Entries, Whitelist_Identity_Entry, Clear_Resolving_List);
}

tBleStatus hci_le_remove_device_from_resolving_list(uint8_t Peer_Identity_Address_Type, uint8_t Peer_Identity_Address[6])
{
    return hci_le_remove_device_from_resolving_list_api(Peer_Identity_Address_Type, Peer_Identity_Address);
}

tBleStatus hci_le_add_device_to_resolving_list(uint8_t Peer_Identity_Address_Type, uint8_t Peer_Identity_Address[6], uint8_t Peer_IRK[16], uint8_t Local_IRK[16])
{
    return hci_le_add_device_to_resolving_list_api(Peer_Identity_Address_Type, Peer_Identity_Address, Peer_IRK, Local_IRK);
}

tBleStatus hci_le_set_resolvable_private_address_timeout(uint16_t RpaTimeout)
{
    return hci_le_set_resolvable_private_address_timeout_api(RpaTimeout);
}

tBleStatus hci_le_set_address_resolution_enable(uint8_t AddressResolutionEnable)
{
    return hci_le_set_address_resolution_enable_api(AddressResolutionEnable);
}

tBleStatus hci_le_read_peer_resolvable_address(uint8_t Peer_Identity_Address_Type, uint8_t Peer_Identity_Address[6], uint8_t Peer_Resolvable_Address[6])
{
    return hci_le_read_peer_resolvable_address_api(Peer_Identity_Address_Type, Peer_Identity_Address, Peer_Resolvable_Address);
}

tBleStatus hci_le_read_local_resolvable_address(uint8_t Peer_Identity_Address_Type, uint8_t Peer_Identity_Address[6], uint8_t Local_Resolvable_Address[6])
{
    return hci_le_read_local_resolvable_address_api(Peer_Identity_Address_Type, Peer_Identity_Address, Local_Resolvable_Address);
}

tBleStatus hci_le_read_resolving_list_size(uint8_t *resolving_List_Size)
{
    return hci_le_read_resolving_list_size_api(resolving_List_Size);
}

tBleStatus hci_le_clear_resolving_list(void)
{
    return hci_le_clear_resolving_list_api();
}

tBleStatus hci_le_set_privacy_mode(uint8_t Peer_Identity_Address_Type, uint8_t Peer_Identity_Address[6], uint8_t Privacy_Mode)
{
    return hci_le_set_privacy_mode_api(Peer_Identity_Address_Type, Peer_Identity_Address, Privacy_Mode);
}

/* Internal core function definitions */
void PRIV_vGeneratePrivateAddress_ucfg(uint8_t *IRK, uint8_t *PrivateAddress)
{
    PRIV_vGeneratePrivateAddress(IRK, PrivateAddress);
}

uint8_t LL_LE_Set_Resolvable_Private_Address_Timeout_ucfg(uint32_t RpaTimeout)
{
    return LL_LE_Set_Resolvable_Private_Address_Timeout(RpaTimeout);
}

uint8_t LL_SetAddressResolutionEnable_ucfg(uint8_t enable)
{
    return LL_SetAddressResolutionEnable(enable);
}

uint8_t PRIV_u32ProcessAdvPacket_ucfg(void *params)
{
    return PRIV_u32ProcessAdvPacket(params);
}

uint8_t PRIV_u8OfflineProcessing_ucfg(void)
{
    return PRIV_u8OfflineProcessing();
}

void PRIV_vGenerate_hci_le_enhanced_connection_complete_event_ucfg(uint8_t role,          uint32_t *peer_addr,
																   uint8_t status,        uint16_t connHandle,
																   uint16_t interval,     uint16_t latency,
																   uint32_t supertimeout, uint16_t CONN_Sca)
{
    PRIV_vGenerate_hci_le_enhanced_connection_complete_event(role,
                                                             peer_addr,
                                                             status,
                                                             connHandle,
                                                             interval,
                                                             latency,
                                                             supertimeout,
                                                             CONN_Sca);
}

BOOL PRIV_vGenerate_hci_le_direct_advertising_report_event_ucfg(uint8_t scanFilterPolicy,
                                                                BOOL adv_direct_add_rpa,
                                                                uint8_t scantype,
                                                                uint8_t peer_addr_type,
                                                                uint8_t* p1,
                                                                uint8_t rssi,
                                                                uint8_t length)
{
    return PRIV_vGenerate_hci_le_direct_advertising_report_event(scanFilterPolicy,
                                                                 adv_direct_add_rpa,
                                                                 scantype,
                                                                 peer_addr_type,
                                                                 p1,
                                                                 rssi,
                                                                 length);
}

tBleStatus GAP_enable_controller_privacy_ucfg(uint8_t *gapRole, uint8_t *numServiceRec)
{
    return GAP_enable_controller_privacy(gapRole, numServiceRec);
}

tBleStatus LL_Encrypt_Plain_Data_ucfg(uint8_t *key, uint8_t *textData, uint8_t *textDataOut, BOOL inISR)
{
    return LL_Encrypt_Plain_Data(key, textData, textDataOut, inISR);
}

uint32_t PRIV_controller_privacy_csr_ucfg(void)
{
    return 1U;
}
#endif
#else
#warning CONTROLLER_PRIVACY_ENABLED is not defined
#endif

/* ---------------------------------------------------------------------------------------------------------- */
#ifdef CONTROLLER_MASTER_ENABLED
#if (CONTROLLER_MASTER_ENABLED == 1) 

/* Internal APIs prototypes */
tBleStatus aci_gap_start_auto_connection_establish_proc_api(uint16_t LE_Scan_Interval,
                                                            uint16_t LE_Scan_Window,
                                                            uint8_t Own_Address_Type,
                                                            uint16_t Conn_Interval_Min,
                                                            uint16_t Conn_Interval_Max,
                                                            uint16_t Conn_Latency,
                                                            uint16_t Supervision_Timeout,
                                                            uint16_t Minimum_CE_Length,
                                                            uint16_t Maximum_CE_Length,
                                                            uint8_t Num_of_Whitelist_Entries,
                                                            Whitelist_Entry_t Whitelist_Entry[]);

tBleStatus aci_gap_start_general_connection_establish_proc_api(uint8_t LE_Scan_Type,
                                                               uint16_t LE_Scan_Interval,
                                                               uint16_t LE_Scan_Window,
                                                               uint8_t Own_Address_Type,
                                                               uint8_t Scan_Filter_Policy,
                                                               uint8_t Filter_Duplicates);

tBleStatus aci_gap_start_selective_connection_establish_proc_api(uint8_t LE_Scan_Type,
                                                                 uint16_t LE_Scan_Interval,
                                                                 uint16_t LE_Scan_Window,
                                                                 uint8_t Own_Address_Type,
                                                                 uint8_t Scan_Filter_Policy,
                                                                 uint8_t Filter_Duplicates,
                                                                 uint8_t Num_of_Whitelist_Entries,
                                                                 Whitelist_Entry_t Whitelist_Entry[]);

tBleStatus aci_gap_create_connection_api(uint16_t LE_Scan_Interval,
                                         uint16_t LE_Scan_Window,
                                         uint8_t Peer_Address_Type,
                                         uint8_t Peer_Address[6],
                                         uint8_t Own_Address_Type,
                                         uint16_t Conn_Interval_Min,
                                         uint16_t Conn_Interval_Max,
                                         uint16_t Conn_Latency,
                                         uint16_t Supervision_Timeout,
                                         uint16_t Minimum_CE_Length,
                                         uint16_t Maximum_CE_Length);

tBleStatus aci_gap_start_connection_update_api(uint16_t Connection_Handle,
                                               uint16_t Conn_Interval_Min,
                                               uint16_t Conn_Interval_Max,
                                               uint16_t Conn_Latency,
                                               uint16_t Supervision_Timeout,
                                               uint16_t Minimum_CE_Length,
                                               uint16_t Maximum_CE_Length);

tBleStatus aci_gap_terminate_gap_proc_api(uint8_t Procedure_Code);

tBleStatus aci_gap_start_limited_discovery_proc_api(uint16_t LE_Scan_Interval,
                                                    uint16_t LE_Scan_Window,
                                                    uint8_t Own_Address_Type,
                                                    uint8_t Filter_Duplicates);

tBleStatus aci_gap_start_general_discovery_proc_api(uint16_t LE_Scan_Interval,
                                                    uint16_t LE_Scan_Window,
                                                    uint8_t Own_Address_Type,
                                                    uint8_t Filter_Duplicates);

tBleStatus aci_gap_start_name_discovery_proc_api(uint16_t LE_Scan_Interval,
                                                 uint16_t LE_Scan_Window,
                                                 uint8_t Peer_Address_Type,
                                                 uint8_t Peer_Address[6],
                                                 uint8_t Own_Address_Type,
                                                 uint16_t Conn_Interval_Min,
                                                 uint16_t Conn_Interval_Max,
                                                 uint16_t Conn_Latency,
                                                 uint16_t Supervision_Timeout,
                                                 uint16_t Minimum_CE_Length,
                                                 uint16_t Maximum_CE_Length);

tBleStatus aci_gap_start_observation_proc_api(uint16_t LE_Scan_Interval,
                                              uint16_t LE_Scan_Window,
                                              uint8_t LE_Scan_Type,
                                              uint8_t Own_Address_Type,
                                              uint8_t Filter_Duplicates,
                                              uint8_t scanFilterPolicy);

tBleStatus aci_gap_send_pairing_req_api(uint16_t Connection_Handle, uint8_t Force_Rebond);

tBleStatus aci_l2cap_connection_parameter_update_resp_api(uint16_t Connection_Handle,
                                                          uint16_t Conn_Interval_Min,
                                                          uint16_t Conn_Interval_Max,
                                                          uint16_t Slave_latency,
                                                          uint16_t Timeout_Multiplier,
                                                          uint16_t Minimum_CE_Length,
                                                          uint16_t Maximum_CE_Length,
                                                          uint8_t Identifier,
                                                          uint8_t Accept);

tBleStatus hci_le_create_connection_api(uint16_t LE_Scan_Interval,
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

tBleStatus hci_le_create_connection_cancel_api(void);

tBleStatus hci_le_connection_update_api(uint16_t Connection_Handle,
                                        uint16_t Conn_Interval_Min,
                                        uint16_t Conn_Interval_Max,
                                        uint16_t Conn_Latency,
                                        uint16_t Supervision_Timeout,
                                        uint16_t Minimum_CE_Length,
                                        uint16_t Maximum_CE_Length);

tBleStatus hci_le_set_scan_parameters_api(uint8_t LE_Scan_Type,
                                          uint16_t LE_Scan_Interval,
                                          uint16_t LE_Scan_Window,
                                          uint8_t Own_Address_Type,
                                          uint8_t Scanning_Filter_Policy);

tBleStatus hci_le_set_scan_enable_api(uint8_t LE_Scan_Enable, uint8_t Filter_Duplicates);

tBleStatus hci_le_start_encryption_api(uint16_t Connection_Handle, uint8_t Random_Number[8],
                                       uint16_t Encrypted_Diversifier, uint8_t Long_Term_Key[16]);

tBleStatus hci_le_set_host_channel_classification_api(uint8_t LE_Channel_Map[5]);

/* Internal core function prototypes */
void GAP_DiscProcTimeoutcb(void);
void GAP_LimDiscTimeoutcb(void);
uint32_t cancel_connect_master(uint8_t slaveno);
void isr_event_handler_scan_data(void *params);
void Reclassify_All_Channels(uint8_t slot_no);
tBleStatus LL_Start_Encryption(uint16_t connHandle, uint8_t *randNum, uint8_t *ediv, uint8_t *ltk);
tBleStatus GAP_check_and_set_role(uint8_t *gapRole, uint8_t role);
void GAP_discover_peer_name(void);
void full_state_func_call_init(void);

/* SMP_MASTER functionalities */
tBleStatus smp_MI_Start_Encryption(void *params);
void       smp_Execute_Actions_wrt_Current_State_MI_excerpt(void *params);
tBleStatus smp_Process_Rx_Packet_wrt_Current_State_MI_excerpt(void *params);
/* APIs definitions*/
tBleStatus aci_gap_start_auto_connection_establish_proc(uint16_t LE_Scan_Interval,
                                                        uint16_t LE_Scan_Window,
                                                        uint8_t Own_Address_Type,
                                                        uint16_t Conn_Interval_Min,
                                                        uint16_t Conn_Interval_Max,
                                                        uint16_t Conn_Latency,
                                                        uint16_t Supervision_Timeout,
                                                        uint16_t Minimum_CE_Length,
                                                        uint16_t Maximum_CE_Length,
                                                        uint8_t Num_of_Whitelist_Entries,
                                                        Whitelist_Entry_t Whitelist_Entry[])
{
    return aci_gap_start_auto_connection_establish_proc_api(LE_Scan_Interval,
                                                            LE_Scan_Window,
                                                            Own_Address_Type,
                                                            Conn_Interval_Min,
                                                            Conn_Interval_Max,
                                                            Conn_Latency,
                                                            Supervision_Timeout,
                                                            Minimum_CE_Length,
                                                            Maximum_CE_Length,
                                                            Num_of_Whitelist_Entries,
                                                            Whitelist_Entry);
}

tBleStatus aci_gap_start_general_connection_establish_proc(uint8_t LE_Scan_Type,
                                                           uint16_t LE_Scan_Interval,
                                                           uint16_t LE_Scan_Window,
                                                           uint8_t Own_Address_Type,
                                                           uint8_t Scan_Filter_Policy,
                                                           uint8_t Filter_Duplicates)
{
    return aci_gap_start_general_connection_establish_proc_api(LE_Scan_Type,
                                                               LE_Scan_Interval,
                                                               LE_Scan_Window,
                                                               Own_Address_Type,
                                                               Scan_Filter_Policy,
                                                               Filter_Duplicates);
}

tBleStatus aci_gap_start_selective_connection_establish_proc(uint8_t LE_Scan_Type,
                                                             uint16_t LE_Scan_Interval,
                                                             uint16_t LE_Scan_Window,
                                                             uint8_t Own_Address_Type,
                                                             uint8_t Scan_Filter_Policy,
                                                             uint8_t Filter_Duplicates,
                                                             uint8_t Num_of_Whitelist_Entries,
                                                             Whitelist_Entry_t Whitelist_Entry[])
{
    return aci_gap_start_selective_connection_establish_proc_api(LE_Scan_Type,
                                                                 LE_Scan_Interval,
                                                                 LE_Scan_Window,
                                                                 Own_Address_Type,
                                                                 Scan_Filter_Policy,
                                                                 Filter_Duplicates,
                                                                 Num_of_Whitelist_Entries,
                                                                 Whitelist_Entry);
}

tBleStatus aci_gap_create_connection(uint16_t LE_Scan_Interval,
                                     uint16_t LE_Scan_Window,
                                     uint8_t Peer_Address_Type,
                                     uint8_t Peer_Address[6],
                                     uint8_t Own_Address_Type,
                                     uint16_t Conn_Interval_Min,
                                     uint16_t Conn_Interval_Max,
                                     uint16_t Conn_Latency,
                                     uint16_t Supervision_Timeout,
                                     uint16_t Minimum_CE_Length,
                                     uint16_t Maximum_CE_Length)
{
    return aci_gap_create_connection_api(LE_Scan_Interval,
                                         LE_Scan_Window,
                                         Peer_Address_Type,
                                         Peer_Address,
                                         Own_Address_Type,
                                         Conn_Interval_Min,
                                         Conn_Interval_Max,
                                         Conn_Latency,
                                         Supervision_Timeout,
                                         Minimum_CE_Length,
                                         Maximum_CE_Length);
}

tBleStatus aci_gap_start_connection_update(uint16_t Connection_Handle,
                                           uint16_t Conn_Interval_Min,
                                           uint16_t Conn_Interval_Max,
                                           uint16_t Conn_Latency,
                                           uint16_t Supervision_Timeout,
                                           uint16_t Minimum_CE_Length,
                                           uint16_t Maximum_CE_Length)
{
    return aci_gap_start_connection_update_api(Connection_Handle,
                                               Conn_Interval_Min,
                                               Conn_Interval_Max,
                                               Conn_Latency,
                                               Supervision_Timeout,
                                               Minimum_CE_Length,
                                               Maximum_CE_Length);
}

tBleStatus aci_gap_terminate_gap_proc(uint8_t Procedure_Code)
{
    return aci_gap_terminate_gap_proc_api(Procedure_Code);
}

tBleStatus aci_gap_start_limited_discovery_proc(uint16_t LE_Scan_Interval,
                                                uint16_t LE_Scan_Window,
                                                uint8_t Own_Address_Type,
                                                uint8_t Filter_Duplicates)
{
    return aci_gap_start_limited_discovery_proc_api(LE_Scan_Interval,
                                                    LE_Scan_Window,
                                                    Own_Address_Type,
                                                    Filter_Duplicates);
}

tBleStatus aci_gap_start_general_discovery_proc(uint16_t LE_Scan_Interval,
                                                uint16_t LE_Scan_Window,
                                                uint8_t Own_Address_Type,
                                                uint8_t Filter_Duplicates)
{
    return aci_gap_start_general_discovery_proc_api(LE_Scan_Interval,
                                                    LE_Scan_Window,
                                                    Own_Address_Type,
                                                    Filter_Duplicates);
}

tBleStatus aci_gap_start_name_discovery_proc(uint16_t LE_Scan_Interval,
                                             uint16_t LE_Scan_Window,
                                             uint8_t Peer_Address_Type,
                                             uint8_t Peer_Address[6],
                                             uint8_t Own_Address_Type,
                                             uint16_t Conn_Interval_Min,
                                             uint16_t Conn_Interval_Max,
                                             uint16_t Conn_Latency,
                                             uint16_t Supervision_Timeout,
                                             uint16_t Minimum_CE_Length,
                                             uint16_t Maximum_CE_Length)
{
    return aci_gap_start_name_discovery_proc_api(LE_Scan_Interval,
                                                 LE_Scan_Window,
                                                 Peer_Address_Type,
                                                 Peer_Address,
                                                 Own_Address_Type,
                                                 Conn_Interval_Min,
                                                 Conn_Interval_Max,
                                                 Conn_Latency,
                                                 Supervision_Timeout,
                                                 Minimum_CE_Length,
                                                 Maximum_CE_Length);
}

tBleStatus aci_gap_start_observation_proc(uint16_t LE_Scan_Interval,
                                          uint16_t LE_Scan_Window,
                                          uint8_t LE_Scan_Type,
                                          uint8_t Own_Address_Type,
                                          uint8_t Filter_Duplicates,
                                          uint8_t scanFilterPolicy)
{
    return aci_gap_start_observation_proc_api(LE_Scan_Interval,
                                              LE_Scan_Window,
                                              LE_Scan_Type,
                                              Own_Address_Type,
                                              Filter_Duplicates,
                                              scanFilterPolicy);
}

tBleStatus aci_gap_send_pairing_req(uint16_t Connection_Handle, uint8_t Force_Rebond)
{
    return aci_gap_send_pairing_req_api(Connection_Handle, Force_Rebond);
}

tBleStatus aci_l2cap_connection_parameter_update_resp(uint16_t Connection_Handle,
                                                      uint16_t Conn_Interval_Min,
                                                      uint16_t Conn_Interval_Max,
                                                      uint16_t Slave_latency,
                                                      uint16_t Timeout_Multiplier,
                                                      uint16_t Minimum_CE_Length,
                                                      uint16_t Maximum_CE_Length,
                                                      uint8_t Identifier,
                                                      uint8_t Accept)

{
    return aci_l2cap_connection_parameter_update_resp_api(Connection_Handle,
                                                          Conn_Interval_Min,
                                                          Conn_Interval_Max,
                                                          Slave_latency,
                                                          Timeout_Multiplier,
                                                          Minimum_CE_Length,
                                                          Maximum_CE_Length,
                                                          Identifier,
                                                          Accept);
}

tBleStatus hci_le_create_connection(uint16_t LE_Scan_Interval,
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
                                    uint16_t Maximum_CE_Length)
{
    return hci_le_create_connection_api(LE_Scan_Interval,
                                        LE_Scan_Window,
                                        Initiator_Filter_Policy,
                                        Peer_Address_Type,
                                        Peer_Address,
                                        Own_Address_Type,
                                        Conn_Interval_Min,
                                        Conn_Interval_Max,
                                        Conn_Latency,
                                        Supervision_Timeout,
                                        Minimum_CE_Length,
                                        Maximum_CE_Length);
}

tBleStatus hci_le_create_connection_cancel(void)
{
    return hci_le_create_connection_cancel_api();
}

tBleStatus hci_le_connection_update(uint16_t Connection_Handle,
                                    uint16_t Conn_Interval_Min,
                                    uint16_t Conn_Interval_Max,
                                    uint16_t Conn_Latency,
                                    uint16_t Supervision_Timeout,
                                    uint16_t Minimum_CE_Length,
                                    uint16_t Maximum_CE_Length)
{
    return hci_le_connection_update_api(Connection_Handle,
                                        Conn_Interval_Min,
                                        Conn_Interval_Max,
                                        Conn_Latency,
                                        Supervision_Timeout,
                                        Minimum_CE_Length,
                                        Maximum_CE_Length);
}

tBleStatus hci_le_set_scan_parameters(uint8_t LE_Scan_Type,
                                      uint16_t LE_Scan_Interval,
                                      uint16_t LE_Scan_Window,
                                      uint8_t Own_Address_Type,
                                      uint8_t Scanning_Filter_Policy)
{
    return hci_le_set_scan_parameters_api(LE_Scan_Type,
                                          LE_Scan_Interval,
                                          LE_Scan_Window,
                                          Own_Address_Type,
                                          Scanning_Filter_Policy);
}

tBleStatus hci_le_set_scan_enable(uint8_t LE_Scan_Enable, uint8_t Filter_Duplicates)
{
    return hci_le_set_scan_enable_api(LE_Scan_Enable, Filter_Duplicates);
}

tBleStatus hci_le_start_encryption(uint16_t Connection_Handle,
                                   uint8_t Random_Number[8],
                                   uint16_t Encrypted_Diversifier,
                                   uint8_t Long_Term_Key[16])
{
    return hci_le_start_encryption_api(Connection_Handle,
                                       Random_Number,
                                       Encrypted_Diversifier,
                                       Long_Term_Key);
}

tBleStatus hci_le_set_host_channel_classification(uint8_t LE_Channel_Map[5])
{
    return hci_le_set_host_channel_classification_api(LE_Channel_Map);
}

/* Internal core function definitions */
void GAP_DiscProcTimeoutcb_ucfg(void)
{
    GAP_DiscProcTimeoutcb();
}

void GAP_LimDiscTimeoutcb_ucfg(void)
{
    GAP_LimDiscTimeoutcb();
}

uint32_t cancel_connect_master_ucfg(uint8_t slaveno)
{
    return cancel_connect_master(slaveno);
}

void isr_event_handler_scan_data_ucfg(void *params)
{
    isr_event_handler_scan_data(params);
}

void Reclassify_All_Channels_ucfg(uint8_t slot_no)
{
    Reclassify_All_Channels(slot_no);
}

tBleStatus LL_Start_Encryption_ucfg(uint16_t connHandle, uint8_t *randNum, uint8_t *ediv, uint8_t *ltk)
{
    return LL_Start_Encryption(connHandle, randNum, ediv, ltk);
}

tBleStatus GAP_check_and_set_role_ucfg(uint8_t *gapRole, uint8_t role)
{
    return GAP_check_and_set_role(gapRole, role);
}

void GAP_discover_peer_name_ucfg(void)
{
    GAP_discover_peer_name();
}

void state_func_call_init_csr_ucfg(void)
{
  full_state_func_call_init();
}

uint32_t master_csr_ucfg(void)
{
  return 1U;
}

/* SMP_MASTER functionalities */
tBleStatus smp_MI_Start_Encryption_ucfg(void *params)
{
    return smp_MI_Start_Encryption(params);
}
void       smp_Execute_Actions_wrt_Current_State_MI_excerpt_ucfg(void *params)
{
    smp_Execute_Actions_wrt_Current_State_MI_excerpt(params);
}
tBleStatus smp_Process_Rx_Packet_wrt_Current_State_MI_excerpt_ucfg(void *params)
{
    return smp_Process_Rx_Packet_wrt_Current_State_MI_excerpt(params);
}
#endif
#else
#warning CONTROLLER_MASTER_ENABLED is not defined
#endif


/* ---------------------------------------------------------------------------------------------------------- */
#if ((CONTROLLER_MASTER_ENABLED == 1) || (SECURE_CONNECTIONS_ENABLED == 1))
void       smp_Execute_Actions_wrt_Current_State_sc_MI_excerpt_phase2as2(void *params);
void       smp_Execute_Actions_wrt_Current_State_sc_MI_excerpt_phase2as2_ucfg(void *params)
{
    smp_Execute_Actions_wrt_Current_State_sc_MI_excerpt_phase2as2(params);
}
#endif
    
/* ---------------------------------------------------------------------------------------------------------- */

#ifdef CONTROLLER_DATA_LENGTH_EXTENSION_ENABLED
#if (CONTROLLER_DATA_LENGTH_EXTENSION_ENABLED == 1)

/* Internal APIs prototypes */
tBleStatus hci_le_set_data_length_api(uint16_t Connection_Handle, uint16_t TxOctets, uint16_t TxTime);
tBleStatus hci_le_read_suggested_default_data_length_api(uint16_t *SuggestedMaxTxOctets, uint16_t *SuggestedMaxTxTime);
tBleStatus hci_le_write_suggested_default_data_length_api(uint16_t SuggestedMaxTxOctets, uint16_t SuggestedMaxTxTime);
tBleStatus hci_le_read_maximum_data_length_api(uint16_t *supportedMaxTxOctets, uint16_t *supportedMaxTxTime,
                                               uint16_t *supportedMaxRxOctets, uint16_t *supportedMaxRxTime);
/* Internal core function prototypes */
void Data_Len_Update_Offline_Processing(void);
BOOL Data_Length_Update_Control_ISR(BOOL is_slave_flag, uint8_t opcode_temp, uint8_t *RcvData_temp_p, volatile uint32_t *sent_event_temp_p);
tBleStatus ll_write_supported_data(uint16_t Supported_Max_Tx_Octets, uint16_t Supported_Max_Tx_Time,
                                   uint16_t Supported_Max_Rx_Octets, uint16_t Supported_Max_Rx_Time);

BOOL LL_Data_Length_Change_Event(void *params);
tBleStatus LL_Send_Length_Resp(uint16_t connHandle);

/* APIs definitions */
tBleStatus hci_le_set_data_length(uint16_t Connection_Handle, uint16_t TxOctets, uint16_t TxTime)
{
    return hci_le_set_data_length_api(Connection_Handle, TxOctets, TxTime);
}

tBleStatus hci_le_read_suggested_default_data_length(uint16_t *SuggestedMaxTxOctets, uint16_t *SuggestedMaxTxTime)
{
    return hci_le_read_suggested_default_data_length_api(SuggestedMaxTxOctets, SuggestedMaxTxTime);
}

tBleStatus hci_le_write_suggested_default_data_length(uint16_t SuggestedMaxTxOctets, uint16_t SuggestedMaxTxTime)
{
    return hci_le_write_suggested_default_data_length_api(SuggestedMaxTxOctets, SuggestedMaxTxTime);
}

tBleStatus hci_le_read_maximum_data_length(uint16_t *supportedMaxTxOctets, uint16_t *supportedMaxTxTime,
                                           uint16_t *supportedMaxRxOctets, uint16_t *supportedMaxRxTime)
{
    return hci_le_read_maximum_data_length_api(supportedMaxTxOctets, supportedMaxTxTime,
                                               supportedMaxRxOctets, supportedMaxRxTime);
}

/* Internal core function definitions */
void Data_Len_Update_Offline_Processing_ucfg(void)
{
    Data_Len_Update_Offline_Processing();
}

BOOL Data_Length_Update_Control_ISR_ucfg(BOOL is_slave_flag, uint8_t opcode_temp, uint8_t *RcvData_temp_p, volatile uint32_t *sent_event_temp_p)
{
    return Data_Length_Update_Control_ISR(is_slave_flag, opcode_temp, RcvData_temp_p, sent_event_temp_p);
}

tBleStatus ll_write_supported_data_ucfg(uint16_t Supported_Max_Tx_Octets, uint16_t Supported_Max_Tx_Time,
                                        uint16_t Supported_Max_Rx_Octets, uint16_t Supported_Max_Rx_Time)
{
    return ll_write_supported_data(Supported_Max_Tx_Octets, Supported_Max_Tx_Time,
                                   Supported_Max_Rx_Octets, Supported_Max_Rx_Time);
}

BOOL LL_Data_Length_Change_Event_ucfg(void *params)
{
    return LL_Data_Length_Change_Event(params);
}

tBleStatus LL_Send_Length_Resp_ucfg(uint16_t connHandle)
{
    return LL_Send_Length_Resp(connHandle);
}

uint32_t data_length_extension_csr_ucfg(void)
{
  return 1U;
}
#endif
#else
#warning CONTROLLER_DATA_LENGTH_EXTENSION_ENABLED is not defined
#endif


/* ---------------------------------------------------------------------------------------------------------- */
#ifdef SECURE_CONNECTIONS_ENABLED
#if(SECURE_CONNECTIONS_ENABLED == 1)
/*
* *****************************************************************************
*                         BLE Stack API functions
* *****************************************************************************
*/

/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
 * BLE Stack API functions "_api" declaration
 */
/* ------------------------------------------------ from bluenrg1_api.h ----- */
tBleStatus aci_gap_passkey_input_api(uint16_t Connection_Handle, uint8_t Input_Type);
tBleStatus aci_gap_numeric_comparison_value_confirm_yesno_api(uint16_t Connection_Handle, uint8_t Confirm_Yes_No);  
tBleStatus hci_le_read_local_p256_public_key_api(void);
tBleStatus hci_le_generate_dhkey_api(uint8_t Remote_P256_Public_Key[64]);

/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
 * BLE Stack API functions "_api" wrapping implementation
 */
/* ------------------------------------------------ from bluenrg1_api.h ----- */
tBleStatus aci_gap_passkey_input(uint16_t Connection_Handle, uint8_t Input_Type)
{
    return aci_gap_passkey_input_api(Connection_Handle, Input_Type);
}

tBleStatus aci_gap_numeric_comparison_value_confirm_yesno(uint16_t Connection_Handle, uint8_t Confirm_Yes_No)     
{
    return aci_gap_numeric_comparison_value_confirm_yesno_api(Connection_Handle, Confirm_Yes_No);
}

tBleStatus hci_le_read_local_p256_public_key(void)
{
    return hci_le_read_local_p256_public_key_api();
}

tBleStatus hci_le_generate_dhkey(uint8_t Remote_P256_Public_Key[64])
{
    return hci_le_generate_dhkey_api(Remote_P256_Public_Key);
}

/*
* *****************************************************************************
*                      BLE Stack INTERNAL core functions
* *****************************************************************************
*/
/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
 * BLE Stack INTERNAL core functions' prototype declaration
 */
/* --------------------------------------------------------- from hci.c ----- */
void PKA_Offline_Processing(void);

/* ----------------------------------------------- from smp_scv42_caf.h ----- */
void        smp_sc_init(BOOL useDebugKey);
tBleStatus  smp_sc_continue_on_exclusive_sc_pairing(void *params);
void        smp_sc_check_for_ecc_pk_generation_request(void);
tBleStatus  smp_sc_generate_new_local_oob_data(void);
void        smp_sc_update_statistics_on_pairing_complete(void *params);


/* ----------------------------------------------- from smp_scv42_sap.h ----- */
void        smp_sc_hci_le_read_local_p256_public_key_complete_evt_hndl(void *params);
void        smp_sc_hci_le_generate_dhkey_complete_evt_hndl(void *params);


/* ------------------------------------------------- from securitymgr.h ----- */
tBleStatus smp_sc_MI_PairingResponse_Receive_sc_excerpt(void *params);
void       smp_sc_MI_Pairing_Phase_2AS1_Start_sc_excerpt(void *params);
void       smp_sc_SR_Pairing_Phase_2AS1_Start_sc_excerpt(void *params);

void       smp_sc_MI_PairingConfirm_Send_sc_excerpt(void *params);
tBleStatus smp_sc_MI_PairingRandom_Receive_sc_excerpt1(void *params);
void       smp_sc_MI_PairingRandom_Receive_sc_excerpt2(void *params);

void       smp_sc_SR_PairingConfirm_Receive_sc_excerpt(void *params);
void       smp_sc_SR_PairingRandom_Receive_sc_excerpt(void *params);

BOOL       smp_Execute_Actions_wrt_Current_State_sc_excerpt_phase_1_to_2(void *params);
void       smp_Execute_Actions_wrt_Current_State_sc_excerpt_phase2as2(void *params);
tBleStatus smp_Process_Rx_Packet_wrt_Current_State_sc_excerpt(void *params);
tBleStatus smp_Process_Rx_Packet_Exception_Cases_sc_excerpt(void *params);

/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
 * BLE Stack INTERNAL core functions' "ucfg" wrapping implementation
 */
/* ----------------------------------------------- from hci.c ----- */
void PKA_Offline_Processing_ucfg(void)
{
    PKA_Offline_Processing();
}

/* ----------------------------------------------- from smp_scv42_caf.h ----- */
void        smp_sc_init_ucfg(BOOL useDebugKey)
{
    smp_sc_init(useDebugKey);
}
tBleStatus  smp_sc_continue_on_exclusive_sc_pairing_ucfg(void *params)
{
    return smp_sc_continue_on_exclusive_sc_pairing(params);
}
void        smp_sc_check_for_ecc_pk_generation_request_ucfg(void)
{
    smp_sc_check_for_ecc_pk_generation_request();
}
tBleStatus  smp_sc_generate_new_local_oob_data_ucfg(void)
{
    return smp_sc_generate_new_local_oob_data();
}
void        smp_sc_update_statistics_on_pairing_complete_ucfg(void *params)
{
    smp_sc_update_statistics_on_pairing_complete(params);
}

/* ----------------------------------------------- from smp_scv42_sap.h ----- */
uint32_t secure_connections_csr_ucfg(void)
{
  return 1U;
}

void        smp_sc_hci_le_read_local_p256_public_key_complete_evt_hndl_ucfg(void *params)
{
    smp_sc_hci_le_read_local_p256_public_key_complete_evt_hndl(params);
}
void        smp_sc_hci_le_generate_dhkey_complete_evt_hndl_ucfg(void *params)
{
    smp_sc_hci_le_generate_dhkey_complete_evt_hndl(params);
}

/* ------------------------------------------------- from securitymgr.h ----- */
tBleStatus smp_sc_MI_PairingResponse_Receive_sc_excerpt_ucfg(void *params)
{
    return smp_sc_MI_PairingResponse_Receive_sc_excerpt(params);
}
void       smp_sc_MI_Pairing_Phase_2AS1_Start_sc_excerpt_ucfg(void *params)
{
    smp_sc_MI_Pairing_Phase_2AS1_Start_sc_excerpt(params);
}
void       smp_sc_SR_Pairing_Phase_2AS1_Start_sc_excerpt_ucfg(void *params)
{
    smp_sc_SR_Pairing_Phase_2AS1_Start_sc_excerpt(params);
}

void       smp_sc_MI_PairingConfirm_Send_sc_excerpt_ucfg(void *params)
{
    smp_sc_MI_PairingConfirm_Send_sc_excerpt(params);
}
tBleStatus smp_sc_MI_PairingRandom_Receive_sc_excerpt1_ucfg(void *params)
{
    return smp_sc_MI_PairingRandom_Receive_sc_excerpt1(params);
}
void       smp_sc_MI_PairingRandom_Receive_sc_excerpt2_ucfg(void *params)
{
    smp_sc_MI_PairingRandom_Receive_sc_excerpt2(params);
}

void       smp_sc_SR_PairingConfirm_Receive_sc_excerpt_ucfg(void *params)
{
    smp_sc_SR_PairingConfirm_Receive_sc_excerpt(params);
}
void       smp_sc_SR_PairingRandom_Receive_sc_excerpt_ucfg(void *params)
{
    smp_sc_SR_PairingRandom_Receive_sc_excerpt(params);
}

BOOL       smp_Execute_Actions_wrt_Current_State_sc_excerpt_phase_1_to_2_ucfg(void *params)
{
    return smp_Execute_Actions_wrt_Current_State_sc_excerpt_phase_1_to_2(params);
}
void       smp_Execute_Actions_wrt_Current_State_sc_excerpt_phase2as2_ucfg(void *params)
{
    smp_Execute_Actions_wrt_Current_State_sc_excerpt_phase2as2(params);
}
tBleStatus smp_Process_Rx_Packet_wrt_Current_State_sc_excerpt_ucfg(void *params)
{
    return smp_Process_Rx_Packet_wrt_Current_State_sc_excerpt(params);
}
tBleStatus smp_Process_Rx_Packet_Exception_Cases_sc_excerpt_ucfg(void *params)
{
    return smp_Process_Rx_Packet_Exception_Cases_sc_excerpt(params);
}

#ifdef __cplusplus            /////////////antonio
}                  			  /////////////antonio
#endif						  /////////////antonio

#endif
#else
#warning "SECURE_CONNECTIONS_ENABLED is not defined"
#endif
