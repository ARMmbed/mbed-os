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
#include "libX509_V3.h"

int8_t asn1_parse(uint8_t *data_ptr, uint16_t *len_ptr, uint8_t *type_ptr)
{
	return 0;
}
uint8_t x509_v3_certi_chain_analyze(uint8_t *ptr, uint16_t len, certificate_chain_t *chain_ptr)
{
	return 0;
}
uint8_t x509_v3_certificate_analyze(uint8_t *ptr, uint16_t cer_len, certificate_info_t *cer_info)
{
	return 0;
}
uint8_t x509_v3_parse_subject_public_key(uint8_t *ptr, uint16_t length, certificate_info_t *cer_info)
{
	return 0;
}
uint8_t x509_v3_parse_signature_alg(uint8_t *ptr, uint16_t length)
{
	return 0;
}
uint8_t x509_v3_parse_signature_parameter(uint8_t *src, uint8_t *dst)
{
	return 0;
}
uint8_t x509_v3_certi_pk_get(uint8_t *ptr, certificate_chain_t *chain_ptr)
{
	return 0;
}
uint8_t *x509_v3_certi_subj_get(uint8_t *ptr, uint16_t *len)
{
	return 0;
}
uint8_t *x509_get_key_from_pki(uint8_t *ptr, uint8_t key_type)
{
	return 0;
}
certificate_chain_internal_t *sec_cetificate_chain_get(certficate_get_types type)
{
	return 0;
}
int8_t x509_cetificate_chain_push(certficate_get_types type, certificate_chain_internal_t *chain)
{
	return 0;
}
int8_t sec_cetificate_chain_remove(certficate_get_types type)
{
	return 0;
}
