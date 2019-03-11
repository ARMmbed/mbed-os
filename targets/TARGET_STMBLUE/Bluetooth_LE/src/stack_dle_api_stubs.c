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

#ifdef CONTROLLER_DATA_LENGTH_EXTENSION_ENABLED
#if (CONTROLLER_DATA_LENGTH_EXTENSION_ENABLED == 0)

#include <compiler.h>
#include "bluenrg1_api.h"

#define ERR_UNKNOWN_HCI_COMMAND (0x01)

/* APIs definitions */
tBleStatus hci_le_set_data_length(uint16_t Connection_Handle, uint16_t TxOctets, uint16_t TxTime)
{
    return ERR_UNKNOWN_HCI_COMMAND;
}

tBleStatus hci_le_read_suggested_default_data_length(uint16_t *SuggestedMaxTxOctets, uint16_t *SuggestedMaxTxTime)
{
    return ERR_UNKNOWN_HCI_COMMAND;
}

tBleStatus hci_le_write_suggested_default_data_length(uint16_t SuggestedMaxTxOctets, uint16_t SuggestedMaxTxTime)
{
    return ERR_UNKNOWN_HCI_COMMAND;
}

tBleStatus hci_le_read_maximum_data_length(uint16_t *supportedMaxTxOctets, uint16_t *supportedMaxTxTime,
                                           uint16_t *supportedMaxRxOctets, uint16_t *supportedMaxRxTime)
{
    return ERR_UNKNOWN_HCI_COMMAND;
}
#endif
#endif

