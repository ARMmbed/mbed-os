/*
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

#ifndef AT_CELLULAR_SMS_H_
#define AT_CELLULAR_SMS_H_

#include "CellularSMS.h"
#include "AT_CellularBase.h"
#include "Callback.h"
#include <time.h>

namespace mbed {

/**
 *  Class AT_CellularSMS
 *
 *  Class for SMS sending, reading and deleting.
 */
class AT_CellularSMS: public CellularSMS, public AT_CellularBase {

public:
    AT_CellularSMS(ATHandler &atHandler);
    virtual ~AT_CellularSMS();

public:
    // from CellularSMS

    virtual nsapi_error_t initialize(CellularSMSMmode mode,
                                     CellularSMSEncoding encoding = CellularSMSEncoding7Bit);

    virtual nsapi_size_or_error_t send_sms(const char *phone_number, const char *message, int msg_len);

    virtual nsapi_size_or_error_t get_sms(char *buf, uint16_t buf_len, char *phone_num, uint16_t phone_len,
                                          char *time_stamp, uint16_t time_len, int *buf_size);

    virtual void set_sms_callback(Callback<void()> func);

    virtual nsapi_error_t set_cpms(const char *memr, const char *memw, const char *mems);

    virtual nsapi_error_t set_csca(const char *sca, int type);

    virtual nsapi_size_or_error_t set_cscs(const char *chr_set);

    virtual nsapi_error_t delete_all_messages();

    virtual void set_extra_sim_wait_time(int sim_wait_time);

private:

    struct sms_info_t {
        char date[SMS_MAX_TIME_STAMP_SIZE];
        uint16_t msg_index[50]; // can hold up to 50 concatenated msg parts, indexes are in correct order. So max sms size is 50*140 =  7kb
        uint16_t msg_size;
        uint8_t parts;
        uint8_t parts_added;
        uint16_t msg_ref_number;
        struct sms_info_t *next_info;
        sms_info_t() : msg_size(0), parts(1), parts_added(1), msg_ref_number(0), next_info(0) {};
    };

    // application callback function for received sms
    Callback<void()> _cb;
    CellularSMSMmode _mode;
    bool _use_8bit_encoding;
    uint32_t _sim_wait_time;
    uint16_t _sms_message_ref_number;
    sms_info_t *_sms_info;

    // SMS urc's
    void cmt_urc();
    void cmti_urc();

    /** Set command selects the format of messages used with send, list, read and write commands.
     *
     *  @param   msg_format 0 PDU mode, 1 text mode
     *  @return  zero for success
     */
    nsapi_error_t set_cmgf(int msg_format);

    /** Set how receiving of new messages from the network is indicated to the TE.
     *
     *  @return zero for success
     */
    nsapi_error_t set_cnmi();

    /** Set Text Mode Parameters
     *
     * @param fo    See more from 3GPP TS 27.005 for all params.
     * @param vp
     * @param pid
     * @param dcs
     * @return      zero for success
     */
    nsapi_error_t set_csmp(int fo, int vp, int pid, int dcs);

    /** CSDH - Set command controls whether detailed header information is shown in text mode (AT+CMGF=1) result codes.
     *
     *  @param show_header  1 to show detailed header in text mode, 0 for not showing.
     *  @return             zero for success
     */
    nsapi_error_t set_csdh(int show_header);

    /** Delete SMS in the given message position(s) in the storage
     *
     *  @param sms  struct containing index array to delete
     *  @return     zero for success
     */
    nsapi_error_t delete_sms(sms_info_t *sms);

    /**
     * Internal helper methods
     */
    nsapi_error_t list_messages();
    int read_sms_params(char *, char *);
    void free_linked_list();
    void add_info(sms_info_t *info, int index, int part_number);
    int read_udh_from_pdu(const char *pdu, sms_info_t *info, int &part_number, int &padding_bits);
    nsapi_size_or_error_t get_data_from_pdu(const char *pdu, sms_info_t *info, int *part_number,
                                            char *phone_number = NULL, char *msg = NULL);
    nsapi_size_or_error_t read_pdu_payload(const char *pdu, int msg_len, int scheme, char *msg, int padding_bits);
    sms_info_t *get_oldest_sms_index();
    bool create_time(const char *time_string, time_t *time);
    int compare_time_strings(const char *time_string_1, const char *time_string_2);
    char *create_pdu(const char *phone_number, const char *message, uint8_t message_length, uint8_t msg_parts,
                     uint8_t msg_part_number, uint8_t &header_size);
    nsapi_size_or_error_t read_sms_from_index(int msg_index, char *buf, uint16_t len, char *phone_num,
                                              char *time_stamp);
    nsapi_size_or_error_t read_sms(sms_info_t *sms, char *buf, char *phone_num, char *time_stamp);

    /** Packs the given str from ascii to 7bit gsm format and converts it to hex to the given buf.
     *
     *  @param str                   string that is to be converted
     *  @param len                   length of the str buffer
     *  @param buf                   preallocated buffer that holds the converted string in hex format after successful call
     *  @param number_of_padding_bit padding bits needed to keep the octet boundary
     *  @return                      length of buffer buf or zero on failure
     */
    uint16_t pack_7_bit_gsm_and_hex(const char *str, uint16_t len, char *buf, int number_of_padding_bit);

    /** Unpacks the given hex- and 7-bit gsm encoded str to ascii string
     *
     *  @param str          string to convert to ascii string and write to buf
     *  @param len          length of the str divided by two as str is hexencoded
     *  @param buf          preallocated destination buffer
     *  @param padding_bits number of padding bits needed to hold the octet boundary
     *  @param msg_len      Length of the received message, which is coded in str
     *  @return             length of the destination buffer buf
     *
     */
    uint16_t unpack_7_bit_gsm_to_str(const char *str, int len, char *buf, int padding_bits,
                                     int msg_len);
};

} // namespace mbed

#endif // AT_CELLULAR_SMS_H_
