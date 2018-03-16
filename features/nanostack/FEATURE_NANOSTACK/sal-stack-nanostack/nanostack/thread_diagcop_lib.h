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


#ifndef NANOSTACK_SOURCE_6LOWPAN_THREAD_THREAD_DIAGCOP_LIB_H_
#define NANOSTACK_SOURCE_6LOWPAN_THREAD_THREAD_DIAGCOP_LIB_H_

#include "ns_types.h"
#include "thread_meshcop_lib.h" // this uses the same parser functions than meshcop but different message types

/**
 * \file thread_diagcop_lib.h
 * \brief CoAP diagnostic TLV.
 * Note that these TLV types overlap with meshcop
 * and must not be used in same messages.
 */

#define DIAGCOP_TLV_EXTENDED_MAC_ADDRESS                     0  /**< Can not reset*/
#define DIAGCOP_TLV_ADDRESS16                                1  /**< Can not reset*/
#define DIAGCOP_TLV_MODE                                     2  /**< Can not reset*/
#define DIAGCOP_TLV_TIMEOUT                                  3  /**< Can not reset*/
#define DIAGCOP_TLV_CONNECTIVITY                             4  /**< Can not reset*/
#define DIAGCOP_TLV_ROUTE64                                  5  /**< Can not reset*/
#define DIAGCOP_TLV_LEADER_DATA                              6  /**< Can not reset*/
#define DIAGCOP_TLV_NETWORK_DATA                             7  /**< Can not reset*/
#define DIAGCOP_TLV_IPV6_ADDRESS_LIST                        8  /**< Can not reset*/
#define DIAGCOP_TLV_MAC_COUNTERS                             9  /**< Can reset*/
#define DIAGCOP_TLV_BATTERY_LEVEL                           14  /**< Can reset*/
#define DIAGCOP_TLV_SUPPLY_VOLTAGE                          15  /**< Can not reset*/
#define DIAGCOP_TLV_CHILD_TABLE                             16  /**< Can not reset*/
#define DIAGCOP_TLV_CHANNEL_PAGES                           17  /**< Can not reset*/
#define DIAGCOP_TLV_TYPE_LIST                               18  /**< List type*/

/**
 * \brief Write array TLV.
 *
 * \param ptr A pointer to array in which to write the TLV.
 * \param type The type of TLV.
 * \param length The length of the data that is written in TLV.
 * \param data The array for TLV value.
 * \return pointer The value for writing the next TLV.
 */
#define thread_diagcop_tlv_data_write(ptr,type,length,data) thread_meshcop_tlv_data_write(ptr,type,length,data)

/**
 * \brief Write 1 byte length TLV.
 *
 * \param ptr A pointer to array in which to write the TLV.
 * \param type The type of TLV.
 * \param data value.
 * \return pointer value for writing the next TLV.
 */
#define thread_diagcop_tlv_data_write_uint8(ptr,type,data) thread_meshcop_tlv_data_write_uint8(ptr,type,data)

/**
 * \brief Write 2 byte length TLV.
 *
 * \param ptr pointer for array where to write the TLV.
 * \param type Type of TLV.
 * \param data value.
 * \return pointer value for writing the next TLV.
 */
#define thread_diagcop_tlv_data_write_uint16(ptr,type,data) thread_meshcop_tlv_data_write_uint16(ptr,type,data)

/**
 * \brief Write 4 byte length TLV.
 *
 * \param ptr pointer for array where to write the TLV.
 * \param type Type of TLV.
 * \param data value.
 * \return pointer value for writing the next TLV.
 */
#define thread_diagcop_tlv_data_write_uint32(ptr,type,data) thread_meshcop_tlv_data_write_uint32(ptr,type,data)

/**
 * \brief Write 8 byte length TLV.
 *
 * \param ptr pointer for array where to write the TLV.
 * \param type Type of TLV.
 * \param data value.
 * \return pointer value for writing the next TLV.
 */
#define thread_diagcop_tlv_data_write_uint64(ptr,type,data) thread_meshcop_tlv_data_write_uint64(ptr,type,data)

/**
 * \brief Find TLV from message.
 *
 * \param ptr Message buffer.
 * \param length Length of the message buffer to validate message.
 * \param type Type of TLV searched.
 * \param result Pointer value is given as result if length is positive. Can be NULL which only searches for the length.
 * \return The length of the TLV data found
 * \return 0 if TLV is empty or no TLV found.
 * \return negative value indicates corrupted message.
 */
#define thread_diagcop_tlv_find(ptr,length,type,result) thread_meshcop_tlv_find(ptr,length,type,result)

/**
 * \brief Read 1 byte length TLV.
 *
 * \param ptr pointer TLV message array.
 * \param length length of TLV message array.
 * \param type Type of TLV.
 * \param data pointer to variable where the value is read.
 * \return pointer length of TLV 1 if success. 0 means TLV not found or does not have any data.
 * Any other value indicates that TLV is not as expected.
 */
#define thread_diagcop_tlv_data_get_uint8(ptr,length,type,data) thread_meshcop_tlv_data_get_uint8(ptr,length,type,data)

/**
 * \brief Read 2 byte length TLV.
 *
 * \param ptr pointer TLV message array.
 * \param length length of TLV message array.
 * \param type Type of TLV.
 * \param data pointer to variable where the value is read.
 * \return pointer length of TLV 2 if success. 0 means TLV not found or does not have any data.
 * Any other value indicates that TLV is not as expected.
 */
#define thread_diagcop_tlv_data_get_uint16(ptr,length,type,data) thread_meshcop_tlv_data_get_uint16(ptr,length,type,data)

/**
 * \brief Read 4 byte length TLV.
 *
 * \param ptr pointer TLV message array.
 * \param length length of TLV message array.
 * \param type Type of TLV.
 * \param data pointer to variable where the value is read.
 * \return pointer length of TLV 4 if success. 0 means TLV not found or does not have any data.
 * Any other value indicates that TLV is not as expected.
 */
#define thread_diagcop_tlv_data_get_uint32(ptr,length,type,data) thread_meshcop_tlv_data_get_uint32(ptr,length,type,data)

/**
 * \brief Read 8 byte length TLV.
 *
 * \param ptr pointer TLV message array.
 * \param length length of TLV message array.
 * \param type Type of TLV.
 * \param data pointer to variable where the value is read.
 * \return pointer length of TLV 4 if success. 0 means TLV not found or does not have any data.
 * Any other value indicates that TLV is not as expected.
 */
#define thread_diagcop_tlv_data_get_uint64(ptr,length,type,data) thread_meshcop_tlv_data_get_uint64(ptr,length,type,data)


#endif /* NANOSTACK_SOURCE_6LOWPAN_THREAD_THREAD_DIAGCOP_LIB_H_ */
