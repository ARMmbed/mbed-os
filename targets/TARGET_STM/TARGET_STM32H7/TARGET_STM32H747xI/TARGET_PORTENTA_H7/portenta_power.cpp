/*
  Copyright (c) 2019-2020, Arduino SA
  SPDX-License-Identifier: Apache-2.0

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.

  You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
  either express or implied.

  See the License for the specific language governing permissions and limitations under the License.
*/

/******************************************************************************
   INCLUDE
 ******************************************************************************/

#include "mbed.h"
#include "portenta_power.h"

/******************************************************************************
   PUBLIC MEMBER FUNCTIONS
 ******************************************************************************/
void enableEthPowerSupply(void)
{
    /* Ensure ETH power supply */
    mbed::I2C i2c(PB_7, PB_6);

    char data[2];

    // LDO3 to 1.2V
    data[0] = 0x52;
    data[1] = 0x9;
    i2c.write(8 << 1, data, sizeof(data));
    data[0] = 0x53;
    data[1] = 0xF;
    i2c.write(8 << 1, data, sizeof(data));

    // SW2 to 3.3V (SW2_VOLT)
    data[0] = 0x3B;
    data[1] = 0xF;
    i2c.write(8 << 1, data, sizeof(data));

    // SW1 to 3.0V (SW1_VOLT)
    data[0] = 0x35;
    data[1] = 0xF;
    i2c.write(8 << 1, data, sizeof(data));

}
