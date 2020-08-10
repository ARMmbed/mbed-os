#[[
 * Copyright (c) 2018, Arm Limited and affiliates
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
]]

# Unit test suite name
set(TEST_SUITE_NAME "lorawan_LoRaPHY")

# Source files
set(unittest-sources
  ../features/lorawan/lorastack/phy/LoRaPHY.cpp
)

# Add test specific include paths
set(unittest-includes ${unittest-includes}
  target_h
  ../features/lorawan/lorastack/phy
)

# Test & stub files
set(unittest-test-sources
  features/lorawan/loraphy/Test_LoRaPHY.cpp
  stubs/LoRaWANTimer_stub.cpp
  stubs/mbed_assert_stub.cpp
)

set(unittest-test-flags
  -DMBED_CONF_LORA_WAKEUP_TIME=5
  -DMBED_CONF_LORA_DUTY_CYCLE_ON_JOIN=true
  -DMBED_CONF_LORA_UPLINK_PREAMBLE_LENGTH=8
  -DMBED_CONF_LORA_TX_MAX_SIZE=255
  -DMBED_CONF_LORA_NB_TRIALS=2
)

