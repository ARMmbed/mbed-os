/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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

#ifndef ATCACONFIG_H
#define ATCACONFIG_H

#include <stdint.h>
#include "ATCAError.h"

/* Field masks */
#define SLOT_CONFIG_WRITE_CONFIG_MASK   0xF000
#define SLOT_CONFIG_IS_SECRET_MASK      0x0080
#define SLOT_CONFIG_ENCRYPT_READ_MASK   0x0040
#define SLOT_CONFIG_READ_KEY_MASK       0x000F

#define KEY_CONFIG_KEY_TYPE_MASK        0x001C
#define KEY_CONFIG_PRIVATE_KEY_MASK     0x0001
#define KEY_CONFIG_EN_PUB_KEY_MASK      0x0002

/* Configuration flags */
#define SLOT_CONFIG_GEN_KEY_ENABLE_FLAG 0x2000
#define SLOT_CONFIG_INT_SIG_FLAG        0x0002
#define SLOT_CONFIG_EXT_SIG_FLAG        0x0001

#define KEY_CONFIG_P256_ECC_KEY         0x0010
#define KEY_CONFIG_NON_ECC_KEY          0x001C

/** class for validating and preparing slot config for a data zone.
 */
class SlotConfig
{
private:
    uint16_t    _register;
public:
    SlotConfig(uint16_t slot_config)
        : _register(slot_config)
    {}
    SlotConfig()
    {
        Reset();
    }

    void        Reset(){ _register = 0; }
    ATCAError   EnableGenKey();
    void        SetAsPrivKey();
    void        EnableExtMsgSig();
    
    bool        GenKeyEnabled();
    bool        IsPrivKey();
    uint16_t    Get()
    {
        return _register;
    }
};


/** class for validating and preparing key config for a data zone.
 */
class KeyConfig
{
private:
    uint16_t    _register;

public:
    KeyConfig(uint16_t key_config)
        : _register(key_config)
    {}

    KeyConfig()
    {
        Reset();
    }

    void        Reset(){ _register = 0; }
    ATCAError   SetECCKeyType();
    ATCAError   SetNonECCKeyType();
    ATCAError   EnablePubKeyGen();
    void        SetPrivate();

    bool        IsECCKey();
    bool        PubKeyGenEnabled();
    bool        IsPrivate();
    uint16_t    Get()
    {
        return _register;
    }
};

#endif /* ATCACONFIG_H */
