/*
 * Copyright (c) 2015-2017, Arm Limited and affiliates.
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * Thread internal API to handles the Thread management framework message parsing and building for address resolution.
 * This library is needed only for thread internal operations.
 * This library is stand alone and only supports TMFCOP message parsing and finding functions.
 */
#ifndef THREAD_TMFCOP_LIB_H_
#define THREAD_TMFCOP_LIB_H_

#include "ns_types.h"
#include "thread_meshcop_lib.h" // this uses the same parser functions than meshcop but different message types

/**
 * CoAP address registration TLV definitions
 * Note that these TLV types overlap with meshcop
 * and must not be used in same messages.
*/
#define TMFCOP_TLV_TARGET_EID                              0
#define TMFCOP_TLV_MAC_ADDRESS                             1
#define TMFCOP_TLV_RLOC16                                  2
#define TMFCOP_TLV_ML_EID                                  3
#define TMFCOP_TLV_STATUS                                  4
#define TMFCOP_TLV_ATTACHED_TIME                           5
#define TMFCOP_TLV_LAST_TRANSACTION_TIME                   6
#define TMFCOP_TLV_ROUTER_MASK                             7
#define TMFCOP_TLV_ND_OPTION                               8
#define TMFCOP_TLV_ND_DATA                                 9
#define TMFCOP_TLV_NETWORK_DATA                            10

/* Thread 1.2 defines*/
#define TMFCOP_TLV_TIMEOUT                                 11
#define TMFCOP_TLV_NETWORK_NAME                            12
#define TMFCOP_TLV_SEQUENCE_NUMBER                         13
#define TMFCOP_TLV_IPV6_ADDRESS                            14
#define TMFCOP_TLV_COMMISSIONER_SESSION_ID                 15



/**
 * Write array TLV.
 *
 * \param ptr pointer for array where to write the TLV.
 * \param type Type of TLV.
 * \param length length of the data that is written in TLV.
 * \param data array for TLV value.
 * \return pointer value for writing the next TLV.
 */
#define thread_tmfcop_tlv_data_write(ptr,type,length,data) thread_meshcop_tlv_data_write(ptr,type,length,data)

/**
 * Write header.
 *
 * \param ptr pointer for array where to write the TLV.
 * \param type Type of TLV.
 * \param length length of the data that is written in TLV.
 *
 * \return pointer value for writing the remaining bytes of network data.
 */
#define thread_tmfcop_tlv_data_write_header(ptr,type,length) thread_meshcop_tlv_data_write_header(ptr,type,length)

/**
 * Write 1 byte length TLV.
 *
 * \param ptr pointer for array where to write the TLV.
 * \param type Type of TLV.
 * \param data value.
 * \return pointer value for writing the next TLV.
 */
#define thread_tmfcop_tlv_data_write_uint8(ptr,type,data) thread_meshcop_tlv_data_write_uint8(ptr,type,data)

/**
 * Write 2 byte length TLV.
 *
 * \param ptr pointer for array where to write the TLV.
 * \param type Type of TLV.
 * \param data value.
 * \return pointer value for writing the next TLV.
 */
#define thread_tmfcop_tlv_data_write_uint16(ptr,type,data) thread_meshcop_tlv_data_write_uint16(ptr,type,data)

/**
 * Write 4 byte length TLV.
 *
 * \param ptr pointer for array where to write the TLV.
 * \param type Type of TLV.
 * \param data value.
 * \return pointer value for writing the next TLV.
 */
#define thread_tmfcop_tlv_data_write_uint32(ptr,type,data) thread_meshcop_tlv_data_write_uint32(ptr,type,data)

/**
 * Write 8 byte length TLV.
 *
 * \param ptr pointer for array where to write the TLV.
 * \param type Type of TLV.
 * \param data value.
 * \return pointer value for writing the next TLV.
 */
#define thread_tmfcop_tlv_data_write_uint64(ptr,type,data) thread_meshcop_tlv_data_write_uint64(ptr,type,data)

/**
 * Check if TLV exists in the message.
 *
 * \param ptr Message buffer.
 * \param length Length of the message buffer to validate message.
 * \param type Type of TLV searched.
 *
 * \return true if TLV is found.
 * \return false if TLV does not exist.
 */
#define thread_tmfcop_tlv_exist(ptr, length, type) thread_meshcop_tlv_exist(ptr, length, type)

/**
 * Find TLV from message.
 *
 * \param ptr Message buffer.
 * \param length Length of the message buffer to validate message.
 * \param type Type of TLV searched.
 * \param result_ptr Pointer value is given as result if length is positive. Can be NULL which only searches for the length.
 * \return The length of the TLV data found
 * \return 0 if TLV is empty or no TLV found.
 * \return negative value indicates corrupted message.
 */
#define thread_tmfcop_tlv_find(ptr,length,type,result) thread_meshcop_tlv_find(ptr,length,type,result)

/**
 * Read 1 byte length TLV.
 *
 * \param ptr pointer TLV message array.
 * \param length length of TLV message array.
 * \param type Type of TLV.
 * \param data_ptr pointer to variable where the value is read.
 * \return pointer length of TLV 1 if success. 0 means TLV not found or does not have any data.
 * Any other value indicates that TLV is not as expected.
 */
#define thread_tmfcop_tlv_data_get_uint8(ptr,length,type,data) thread_meshcop_tlv_data_get_uint8(ptr,length,type,data)

/**
 * Read 2 byte length TLV.
 *
 * \param ptr pointer TLV message array.
 * \param length length of TLV message array.
 * \param type Type of TLV.
 * \param data_ptr pointer to variable where the value is read.
 * \return pointer length of TLV 2 if success. 0 means TLV not found or does not have any data.
 * Any other value indicates that TLV is not as expected.
 */
#define thread_tmfcop_tlv_data_get_uint16(ptr,length,type,data) thread_meshcop_tlv_data_get_uint16(ptr,length,type,data)

/**
 * Read 4 byte length TLV.
 *
 * \param ptr pointer TLV message array.
 * \param length length of TLV message array.
 * \param type Type of TLV.
 * \param data_ptr pointer to variable where the value is read.
 * \return pointer length of TLV 4 if success. 0 means TLV not found or does not have any data.
 * Any other value indicates that TLV is not as expected.
 */
#define thread_tmfcop_tlv_data_get_uint32(ptr,length,type,data) thread_meshcop_tlv_data_get_uint32(ptr,length,type,data)

/**
 * Read 8 byte length TLV.
 *
 * \param ptr pointer TLV message array.
 * \param length length of TLV message array.
 * \param type Type of TLV.
 * \param data_ptr pointer to variable where the value is read.
 * \return pointer length of TLV 4 if success. 0 means TLV not found or does not have any data.
 * Any other value indicates that TLV is not as expected.
 */
#define thread_tmfcop_tlv_data_get_uint64(ptr,length,type,data) thread_meshcop_tlv_data_get_uint64(ptr,length,type,data)

#endif /* THREAD_TMFCOP_LIB_H_ */
