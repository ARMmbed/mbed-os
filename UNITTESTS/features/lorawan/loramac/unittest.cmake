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
set(TEST_SUITE_NAME "lorawan_LoRaMac")

# Source files
set(unittest-sources
  ../features/lorawan/lorastack/mac/LoRaMac.cpp
)

# Add test specific include paths
set(unittest-includes ${unittest-includes}
  target_h
  ../features/lorawan/lorastack/mac
)

# Test & stub files
set(unittest-test-sources
  features/lorawan/loramac/Test_LoRaMac.cpp
  stubs/LoRaPHY_stub.cpp
  stubs/LoRaWANStack_stub.cpp
  stubs/mbed_assert_stub.cpp
  stubs/LoRaMacCrypto_stub.cpp
  stubs/LoRaMacChannelPlan_stub.cpp
  stubs/LoRaWANTimer_stub.cpp
  stubs/LoRaMacCommand_stub.cpp
  stubs/EventQueue_stub.cpp
  stubs/Mutex_stub.cpp
)

set(unittest-test-flags
  -DMBED_CONF_LORA_ADR_ON=true
  -DMBED_CONF_LORA_PUBLIC_NETWORK=true
  -DMBED_CONF_LORA_NB_TRIALS=2
  -DMBED_CONF_LORA_DOWNLINK_PREAMBLE_LENGTH=5
  -DMBED_CONF_LORA_DUTY_CYCLE_ON=true
  -DMBED_CONF_LORA_MAX_SYS_RX_ERROR=10
  -DMBED_CONF_LORA_TX_MAX_SIZE=255
  -DMBED_CONF_LORA_DEVICE_ADDRESS=0x00000000
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMBED_CONF_LORA_NWKSKEY=\"{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}\"")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMBED_CONF_LORA_NWKSKEY=\"{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}\"")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMBED_CONF_LORA_APPSKEY=\"{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}\"")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMBED_CONF_LORA_APPSKEY=\"{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}\"")


