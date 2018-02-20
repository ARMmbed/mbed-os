/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ATCAConfig.h"

ATCAError SlotConfig::EnableGenKey()
{
    if (!IsPrivKey())
        return ATCA_ERR_SLOT_NOT_PRIV_KEY;
    _register = (_register & ~SLOT_CONFIG_WRITE_CONFIG_MASK) | SLOT_CONFIG_GEN_KEY_ENABLE_FLAG;
    return ATCA_SUCCESS;
}

void SlotConfig::SetAsPrivKey()
{
    /* Declare ECC Private key */
    _register |= SLOT_CONFIG_IS_SECRET_MASK;
    /* Disable reads */
    _register &= ~SLOT_CONFIG_ENCRYPT_READ_MASK;
}

void SlotConfig::EnableExtMsgSig()
{
    /* Disable ECDH operations and internal signatures */
    _register &= ~SLOT_CONFIG_READ_KEY_MASK;
    /* Enable signing of arbitrary external messages */
    _register |= SLOT_CONFIG_INT_SIG_FLAG;
    _register |= SLOT_CONFIG_EXT_SIG_FLAG;
}

bool SlotConfig::GenKeyEnabled()
{
    return ( (_register & SLOT_CONFIG_WRITE_CONFIG_MASK) == SLOT_CONFIG_GEN_KEY_ENABLE_FLAG);
}

bool SlotConfig::IsPrivKey()
{
    return (_register & SLOT_CONFIG_IS_SECRET_MASK) == SLOT_CONFIG_IS_SECRET_MASK &&
           (_register & SLOT_CONFIG_ENCRYPT_READ_MASK) == 0;
}


ATCAError KeyConfig::SetECCKeyType()
{
    _register &= ~KEY_CONFIG_KEY_TYPE_MASK;
    _register |= KEY_CONFIG_P256_ECC_KEY;
    return ATCA_SUCCESS;
}

ATCAError KeyConfig::SetNonECCKeyType()
{
    _register &= ~KEY_CONFIG_KEY_TYPE_MASK;
    _register |= KEY_CONFIG_NON_ECC_KEY;
    return ATCA_SUCCESS;
}

ATCAError KeyConfig::EnablePubKeyGen()
{
    if (!IsPrivate())
        return ATCA_ERR_SLOT_NOT_PRIV_KEY;
    _register |= KEY_CONFIG_EN_PUB_KEY_MASK;
    return ATCA_SUCCESS;
}

void KeyConfig::SetPrivate()
{
    _register |= KEY_CONFIG_PRIVATE_KEY_MASK;
}

bool KeyConfig::IsECCKey()
{
    return ( (_register & KEY_CONFIG_P256_ECC_KEY) == KEY_CONFIG_P256_ECC_KEY);
}

bool KeyConfig::PubKeyGenEnabled()
{
    return ( (_register & KEY_CONFIG_EN_PUB_KEY_MASK) == KEY_CONFIG_EN_PUB_KEY_MASK);
}

bool KeyConfig::IsPrivate()
{
    return ( (_register & KEY_CONFIG_PRIVATE_KEY_MASK) == KEY_CONFIG_PRIVATE_KEY_MASK);
}

