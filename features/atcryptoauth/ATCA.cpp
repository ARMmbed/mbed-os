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

#include "ATCA.h"

ATCA * ATCA::instance = NULL;

uint8_t ATCA::sample_config[] = {
    0x01, 0x23, 0x95, 0x11,
    0x00, 0x00, 0x50, 0x00,
    0x5D, 0xCF, 0xD6, 0xCD,
    0xEE, 0xC0, 0x1D, 0x00,
    0xC0, 0x00, 0x55, 0x00,
    0x83, 0x20, 0x83, 0x20,
    0x83, 0x20, 0x83, 0x20,
    0x83, 0x20, 0x83, 0x20,
    0x83, 0x20, 0x83, 0x20,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xAF, 0x8F,
    0xFF, 0xFF, 0xFF, 0xFF,
    0x00, 0x00, 0x00, 0x00,
    0xFF, 0xFF, 0xFF, 0xFF,
    0x00, 0x00, 0x00, 0x00,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0x00, 0x00, 0x55, 0x55,
    0xFF, 0xFF, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x13, 0x00, 0x13, 0x00,
    0x13, 0x00, 0x13, 0x00,
    0x13, 0x00, 0x13, 0x00,
    0x13, 0x00, 0x13, 0x00,
    0x1C, 0x00, 0x10, 0x00,
    0x10, 0x00, 0x10, 0x00,
    0x10, 0x00, 0x10, 0x00,
    0x10, 0x00, 0x1C, 0x00
};

ATCA::ATCA()
    :plt(I2C_SDA, I2C_SCL,
         ATCA_ECC_I2C_FREQUENCY,
         ATCA_ECC_508A_I2C_ADDR),
    device(plt)
{
}

ATCA * ATCA::GetInstance()
{
    if ( instance == NULL )
    {
        instance = new ATCA();
        if ( instance != NULL )
            instance->device.Init();
    }
    return instance;
}

void ATCA::Deinit()
{
    if ( instance != NULL )
        delete instance;
}

ATCAKey * ATCA::GetKeyToken(ATCAKeyID keyId, ATCAError & err)
{
    size_t pk_len = 0;
    uint8_t pk[ATCA_ECC_ECC_PK_LEN];
    err = device.GenPubKey(keyId, pk, sizeof(pk), &pk_len);
    if (err != ATCA_ERR_NO_ERROR )
    {
        return NULL;
    }
    if ( pk_len != ATCA_ECC_ECC_PK_LEN )
    {
        err = ATCA_ERR_DEVICE_ERROR;
        return NULL;
    }

    ATCAKey * key = new ATCAKey( device, keyId, pk );
    if ( key == NULL )
        err = ATCA_ERR_MEM_ALLOC_FAILURE;
    return key;
}

/**
 * ATCA device needs one time (irreversible) locking of the configuration zone. 
 * This function sets up ECC private and public key slots with desired configuration. TBD explain configuration.
 * For production users should come up with their own configuration that is relevant to their application.
 * The configuration is only usable after locking it. This function does not lock the config. Use lock_config with
 * caution.
 */
ATCAError ATCA::Commission( )
{
    // check if the device is locked?
    bool locked;
    ATCAError err = device.CheckConfigLock( &locked );
    if (err != ATCA_ERR_NO_ERROR)
    {
        return ( err );
    }
    if ( locked )
    {
        return( ATCA_ERR_CONFIG_LOCKED );
    }
    // Configure slots 0-7 as ECC Private keys
    for (uint8_t i = 0; i < 8; i++)
    {
        err = device.ConfigPrivKey(i);
        if (err != ATCA_ERR_NO_ERROR)
            return( err );
    }
    // Configure slot 9-14 as ECC Public keys
    for (uint8_t i = 9; i <= 14; i++)
    {
        err = device.ConfigPubKey(i);
        if (err != ATCA_ERR_NO_ERROR)
            return( err );
    }
    // Setup slot 8 for certificate storage.
    err = device.ConfigCertStore();
    if (err != ATCA_ERR_NO_ERROR)
        return( err );

    return( ATCA_ERR_NO_ERROR );
}

ATCAError ATCA::LockConfig()
{
    return device.LockCommand(sample_config, sizeof(sample_config));
}

/**
 * Dump device configuration zone data.
 */
void ATCA::DumpDeviceConfig()
{
    device.DumpConfig();
}



/**
 * Commission ATCA device for testing & demonstration with Mbed OS.
 */
ATCAError ATCA::GenPrivKey(ATCAKeyID keyIdx)
{
    // Create a Private ECC Key on Key Index
    uint8_t pk_temp[ATCA_ECC_ECC_PK_LEN];
    size_t len = 0;
    return device.GenPrivateKey(0, pk_temp, sizeof(pk_temp), &len);
}

