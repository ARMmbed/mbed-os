 /*
  *  mbedtls_device.h
  *
  *  Copyright (C) 2018, Arm Limited, All Rights Reserved
  *  SPDX-License-Identifier: Apache-2.0
  *
  *  Licensed under the Apache License, Version 2.0 (the "License"); you may
  *  not use this file except in compliance with the License.
  *  You may obtain a copy of the License at
  *
  *  http://www.apache.org/licenses/LICENSE-2.0
  *
  *  Unless required by applicable law or agreed to in writing, software
  *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
  *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  *  See the License for the specific language governing permissions and
  *  limitations under the License.
  *
  */

#ifndef __MBEDTLS_DEVICE__
#define __MBEDTLS_DEVICE__

#define MBEDTLS_PLATFORM_SETUP_TEARDOWN_ALT
#define MBEDTLS_SHA1_ALT
#define MBEDTLS_SHA256_ALT
#define MBEDTLS_CCM_ALT
#define MBEDTLS_ECDSA_VERIFY_ALT
#define MBEDTLS_ECDSA_SIGN_ALT
#define MBEDTLS_ECDSA_GENKEY_ALT
#define MBEDTLS_ECDH_GEN_PUBLIC_ALT
#define MBEDTLS_ECDH_COMPUTE_SHARED_ALT

#endif //__MBEDTLS_DEVICE__
