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
