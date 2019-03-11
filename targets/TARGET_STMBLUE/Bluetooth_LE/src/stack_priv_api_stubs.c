/*
 * Copyright (c) 2019 STMicroelectronics. All Rights Reserved.
 *
 * The information contained herein is property of STMicroelectronics.
 * Terms and conditions of usage are described in detail in SOFTWARE
 * LICENSE AGREEMENT:
 *
 * (SLA0068 - PRODUCTION LIMITED LICENSE AGREEMENT FOR ST MATERIALS)
 *
 * You may obtain a copy of the License here:
 * LICENSE-SLA0068.txt and at STMicroelectronics website.
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 */

#include "stack_user_cfg.h"

#ifdef CONTROLLER_PRIVACY_ENABLED
#if (CONTROLLER_PRIVACY_ENABLED == 0)

#include <compiler.h>
#include "bluenrg1_api.h"

#define ERR_UNKNOWN_HCI_COMMAND (0x01)

/* APIs definitions */
tBleStatus aci_gap_add_devices_to_resolving_list(uint8_t Num_of_Resolving_list_Entries, Whitelist_Identity_Entry_t Whitelist_Identity_Entry[], uint8_t Clear_Resolving_List)
{
  return ERR_UNKNOWN_HCI_COMMAND;
}

tBleStatus hci_le_remove_device_from_resolving_list(uint8_t Peer_Identity_Address_Type, uint8_t Peer_Identity_Address[6])
{
  return ERR_UNKNOWN_HCI_COMMAND;
}

tBleStatus hci_le_add_device_to_resolving_list(uint8_t Peer_Identity_Address_Type, uint8_t Peer_Identity_Address[6], uint8_t Peer_IRK[16], uint8_t Local_IRK[16])
{
  return ERR_UNKNOWN_HCI_COMMAND;
}

tBleStatus hci_le_set_resolvable_private_address_timeout(uint16_t RpaTimeout)
{
  return ERR_UNKNOWN_HCI_COMMAND;
}

tBleStatus hci_le_set_address_resolution_enable(uint8_t AddressResolutionEnable)
{
  return ERR_UNKNOWN_HCI_COMMAND;
}

tBleStatus hci_le_read_peer_resolvable_address(uint8_t Peer_Identity_Address_Type, uint8_t Peer_Identity_Address[6], uint8_t Peer_Resolvable_Address[6])
{
  return ERR_UNKNOWN_HCI_COMMAND;
}

tBleStatus hci_le_read_local_resolvable_address(uint8_t Peer_Identity_Address_Type, uint8_t Peer_Identity_Address[6], uint8_t Local_Resolvable_Address[6])
{
  return ERR_UNKNOWN_HCI_COMMAND;
}

tBleStatus hci_le_read_resolving_list_size(uint8_t *resolving_List_Size)
{
  return ERR_UNKNOWN_HCI_COMMAND;
}

uint8_t hci_le_clear_resolving_list(void)
{
  return ERR_UNKNOWN_HCI_COMMAND;
}

tBleStatus hci_le_set_privacy_mode(uint8_t Peer_Identity_Address_Type, uint8_t Peer_Identity_Address[6], uint8_t Privacy_Mode)
{
  return ERR_UNKNOWN_HCI_COMMAND;
}
#endif
#endif
