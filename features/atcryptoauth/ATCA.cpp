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

ATCAFactory * ATCAFactory::instance = NULL;

ATCAFactory::ATCAFactory()
    :plt(I2C_SDA, I2C_SCL,
         ATCA_ECC_I2C_FREQUENCY,
         ATCA_ECC_508A_I2C_ADDR),
    device(plt)
{
}

ATCAFactory::~ATCAFactory()
{
}

ATCADevice * ATCAFactory::GetDevice( ATCAError & err )
{
    if ( instance == NULL )
    {
        instance = new ATCAFactory();
        if ( instance == NULL )
        {
            err = ATCA_ERR_MEM_ALLOC_FAILURE;
            return NULL;
        }
        err = instance->device.Init();
        if ( err != ATCA_ERR_NO_ERROR )
        {
            delete instance;
            instance = NULL;
            return NULL;
        }
    }
    err = ATCA_ERR_NO_ERROR;
    return &instance->device;
}

void ATCAFactory::Deinit()
{
    if ( instance != NULL )
    {
        delete instance;
        instance = NULL;
    }
}

