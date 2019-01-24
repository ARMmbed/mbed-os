#include "stack_user_cfg.h"

#ifdef SECURE_CONNECTIONS_ENABLED
#if (SECURE_CONNECTIONS_ENABLED == 0)

#include "compiler.h"
#include "bluenrg1_api.h"

#define ERR_UNKNOWN_HCI_COMMAND (0x01)

/* APIs definitions */
tBleStatus aci_gap_passkey_input(uint16_t Connection_Handle, uint8_t Input_Type)
{
  return ERR_UNKNOWN_HCI_COMMAND;
}

tBleStatus aci_gap_numeric_comparison_value_confirm_yesno(uint16_t Connection_Handle, uint8_t Confirm_Yes_No)
{
  return ERR_UNKNOWN_HCI_COMMAND;
}

tBleStatus hci_le_read_local_p256_public_key(void)
{
  return ERR_UNKNOWN_HCI_COMMAND;
}

tBleStatus hci_le_generate_dhkey(uint8_t Remote_P256_Public_Key[64])
{
  return ERR_UNKNOWN_HCI_COMMAND;
}
#endif
#endif
