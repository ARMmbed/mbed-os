/**
  ******************************************************************************
  * @file    stack_sc_api_stubs.c
  * @author  AMS - RF Application team
  * @version V1.0.0
  * @date    19 February 2018
  * @brief   Stub API for secure connection
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
*/

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
