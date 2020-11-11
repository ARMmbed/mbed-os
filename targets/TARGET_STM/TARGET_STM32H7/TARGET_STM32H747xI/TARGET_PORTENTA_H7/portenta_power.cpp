/*
  Copyright (c) 2019 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
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
  data[0]=0x52;
  data[1]=0x9;
  i2c.write(8 << 1, data, sizeof(data));
  data[0]=0x53;
  data[1]=0xF;
  i2c.write(8 << 1, data, sizeof(data));
  
  // SW2 to 3.3V (SW2_VOLT)
  data[0]=0x3B;
  data[1]=0xF;
  i2c.write(8 << 1, data, sizeof(data));

  // SW1 to 3.0V (SW1_VOLT)
  data[0]=0x35;
  data[1]=0xF;
  i2c.write(8 << 1, data, sizeof(data));
  
}
