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
set(TEST_SUITE_NAME "lorawan_LoRaMacCrypto")

# Source files
set(unittest-sources
  ../features/lorawan/lorastack/mac/LoRaMacCrypto.cpp
)

# Add test specific include paths
set(unittest-includes ${unittest-includes}
  target_h
  ../features/lorawan/lorastack/mac
)

# Test & stub files
set(unittest-test-sources
  features/lorawan/loramaccrypto/Test_LoRaMacCrypto.cpp
  stubs/cipher_stub.c
  stubs/aes_stub.c
  stubs/cmac_stub.c
  stubs/mbed_assert_stub.cpp
  ../features/nanostack/coap-service/test/coap-service/unittest/stub/mbedtls_stub.c

)

set(unittest-test-flags
  -DMBED_CONF_LORA_TX_MAX_SIZE=255
  )
