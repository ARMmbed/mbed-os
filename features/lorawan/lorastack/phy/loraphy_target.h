/**
 * Copyright (c) 2017, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
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

#ifndef LORAPHY_TARGET
#define LORAPHY_TARGET

#ifdef MBED_CONF_LORA_PHY
 #if MBED_CONF_LORA_PHY      == 0
  #include "lorawan/lorastack/phy/LoRaPHYEU868.h"
  #define LoRaPHY_region LoRaPHYEU868
 #elif MBED_CONF_LORA_PHY    == 1
  #include "lorawan/lorastack/phy/LoRaPHYAS923.h"
  #define LoRaPHY_region LoRaPHYAS923
 #elif MBED_CONF_LORA_PHY    == 2
  #include "lorawan/lorastack/phy/LoRaPHYAU915.h"
 #define LoRaPHY_region LoRaPHYAU915;
 #elif MBED_CONF_LORA_PHY    == 3
  #include "lorawan/lorastack/phy/LoRaPHYCN470.h"
  #define LoRaPHY_region LoRaPHYCN470
 #elif MBED_CONF_LORA_PHY    == 4
  #include "lorawan/lorastack/phy/LoRaPHYCN779.h"
  #define LoRaPHY_region LoRaPHYCN779
 #elif MBED_CONF_LORA_PHY    == 5
  #include "lorawan/lorastack/phy/LoRaPHYEU433.h"
  #define LoRaPHY_region LoRaPHYEU433
 #elif MBED_CONF_LORA_PHY    == 6
  #include "lorawan/lorastack/phy/LoRaPHYIN865.h"
  #define LoRaPHY_region LoRaPHYIN865
 #elif MBED_CONF_LORA_PHY    == 7
  #include "lorawan/lorastack/phy/LoRaPHYKR920.h"
  #define LoRaPHY_region LoRaPHYKR920
 #elif MBED_CONF_LORA_PHY    == 8
  #include "lorawan/lorastack/phy/LoRaPHYUS915.h"
  #define LoRaPHY_region LoRaPHYUS915
 #elif MBED_CONF_LORA_PHY    == 9
  #include "lorawan/lorastack/phy/LoRaPHYUS915Hybrid.h"
  #define LoRaPHY_region LoRaPHYUS915Hybrid
 #endif //MBED_CONF_LORA_PHY == VALUE
#else
 #error "Must set LoRa PHY layer parameters."
#endif //MBED_CONF_LORA_PHY

#endif // LORAPHY_TARGET

