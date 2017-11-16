/*
 * Copyright (c) 2016-2017, Arm Limited and affiliates.
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
#include "ns_types.h"
#include "ecc.h"

void ecc_init(void)
{
	return;
}
void ecc_library_free_pointer(void *ptr)
{
	return;
}
void ecc_free_memory(void){
	return;
}
int8_t ecc_state_idle_check(void)
{
	return 0;
}
int8_t ecc_calculate_signature(ECDSASignature *signature, MPint *certificate_private_key, void (*cb_ptr)(int8_t, void *))
{
	return 0;
}
int8_t ecc_calculate_verify(ECDSASignature *signature, EllipticPoint *public_key, void (*cb_ptr)(int8_t, void *))
{
	return 0;
}
int8_t ecc_calculate_public_key(MPint *p_key, void (*cb_ptr)(int8_t, void *))
{
	return 0;
}
int8_t ecc_calculate_pre_master_secret(EllipticPoint *public_key, MPint *p_key, void (*cb_ptr)(int8_t, void *))
{
	return 0;
}
int8_t ecc_run(void)
{
	return 0;
}
int8_t ecc_blocking_run_block_counter_set(uint16_t new_block_sequency_length)
{
	return 0;
}
EllipticPoint *ecc_get_elliptic_point(void)
{
	return 0;
}
ECDSASignature *ecc_get_ecdsa_signature(void)
{
	return 0;
}
