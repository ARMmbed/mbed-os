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

#define LORA_REGION_EU868           0x10
#define LORA_REGION_AS923           0x11
#define LORA_REGION_AU915           0x12
#define LORA_REGION_CN470           0x13
#define LORA_REGION_CN779           0x14
#define LORA_REGION_EU433           0x15
#define LORA_REGION_IN865           0x16
#define LORA_REGION_KR920           0x17
#define LORA_REGION_US915           0x18
#define LORA_REGION_US915_HYBRID    0x19

//DO NOT USE integer values in mbed_app.json!
//These are defined for backward compatibility and
//Will be DEPRECATED in the future
#define LORA_REGION_0       0x10
#define LORA_REGION_1       0x11
#define LORA_REGION_2       0x12
#define LORA_REGION_3       0x13
#define LORA_REGION_4       0x14
#define LORA_REGION_5       0x15
#define LORA_REGION_6       0x16
#define LORA_REGION_7       0x17
#define LORA_REGION_8       0x18
#define LORA_REGION_9       0x19

//Since 0 would be equal to any undefined value we need to handle this in a other way
#define mbed_lora_concat_(x) LORA_REGION_##x
#define mbed_lora_concat(x) mbed_lora_concat_(x)
#define LORA_REGION mbed_lora_concat(MBED_CONF_LORA_PHY)

 #if LORA_REGION      == LORA_REGION_EU868
  #include "lorawan/lorastack/phy/LoRaPHYEU868.h"
  #define LoRaPHY_region LoRaPHYEU868
 #elif LORA_REGION    == LORA_REGION_AS923
  #include "lorawan/lorastack/phy/LoRaPHYAS923.h"
  #define LoRaPHY_region LoRaPHYAS923
 #elif LORA_REGION    == LORA_REGION_AU915
  #include "lorawan/lorastack/phy/LoRaPHYAU915.h"
 #define LoRaPHY_region LoRaPHYAU915
 #elif LORA_REGION    == LORA_REGION_CN470
  #include "lorawan/lorastack/phy/LoRaPHYCN470.h"
  #define LoRaPHY_region LoRaPHYCN470
 #elif LORA_REGION    == LORA_REGION_CN779
  #include "lorawan/lorastack/phy/LoRaPHYCN779.h"
  #define LoRaPHY_region LoRaPHYCN779
 #elif LORA_REGION    == LORA_REGION_EU433
  #include "lorawan/lorastack/phy/LoRaPHYEU433.h"
  #define LoRaPHY_region LoRaPHYEU433
 #elif LORA_REGION    == LORA_REGION_IN865
  #include "lorawan/lorastack/phy/LoRaPHYIN865.h"
  #define LoRaPHY_region LoRaPHYIN865
 #elif LORA_REGION    == LORA_REGION_KR920
  #include "lorawan/lorastack/phy/LoRaPHYKR920.h"
  #define LoRaPHY_region LoRaPHYKR920
 #elif LORA_REGION    == LORA_REGION_US915
  #include "lorawan/lorastack/phy/LoRaPHYUS915.h"
  #define LoRaPHY_region LoRaPHYUS915
 #elif LORA_REGION    == LORA_REGION_US915_HYBRID
  #include "lorawan/lorastack/phy/LoRaPHYUS915Hybrid.h"
  #define LoRaPHY_region LoRaPHYUS915Hybrid
 #else
  #error "Invalid region configuration, update mbed_app.json with correct MBED_CONF_LORA_PHY value"
 #endif //MBED_CONF_LORA_PHY == VALUE
#else
 #error "Must set LoRa PHY layer parameters."
#endif //MBED_CONF_LORA_PHY

#endif // LORAPHY_TARGET

