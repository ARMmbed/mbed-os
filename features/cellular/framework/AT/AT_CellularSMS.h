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
class AT_CellularSMS: public CellularSMS, public AT_CellularBase
{

public:
    AT_CellularSMS(ATHandler &atHandler);
    virtual ~AT_CellularSMS();

public:
    // from CellularSMS

    /** Does all the necessary initializations needed for receiving and sending sms.
     *
     *  @param mode          enumeration for choosing the correct mode: text/pdu
     *  @return              zero on success
     */
    virtual nsapi_error_t initialize(CellularSMSMmode mode);

    /** Send the SMS with the given parameters
     *
     *  @param phone_number  Phone number where to send sms
     *  @param message       SMS message content
     *  @param msg_len       Length of the message
     *  @return              possible error code or length of the sent sms
     */
    virtual nsapi_size_or_error_t send_sms(const char* phone_number, const char* message, int msg_len);

    /** Gets the oldest received sms.
     *
     *  @param buf           preallocated buffer for sms message content
     *  @param buf_len       length of allocated buf
     *  @param phone_num     preallocated buffer for phone number where sms was sent
     *  @param phone_len     length of allocated phone_num  buffer
     *  @param time_stamp    preallocated buffer for TP-Service Centre Time Stamp (format: yy/MM/dd,hh:mm:ss-+zz). +-zz is timezone.
     *                       The unit of time zone is a quarter of an hour relative to GMT. For example +32 would be GMT+8.
     *  @param time_len      length of allocated time_stamp buffer
     *  @param buf_size      if method return error NSAPI_ERROR_NO_MEMORY because the given buf was not big enough this will
     *                       hold the size which is enough. Otherwise zero.
     *  @return              possible error code or size of buf. Will return SMS_ERROR_MULTIPART_ALL_PARTS_NOT_READ
     *                       if sms was multipart but not all parts are present/failed to read.
     */
    virtual nsapi_size_or_error_t get_sms(char* buf, uint16_t buf_len, char* phone_num, uint16_t phone_len,
            char* time_stamp, uint16_t time_len, int *buf_size);

    /** Callback which is called when new sms is received. SMS can be fetched via method get_sms().
     *
     *  @remark      In PDU mode there can be multipart sms and callback is called for every received part.
     *
     *  @param func  Callback function which is called when new sms is received.
     */
    virtual void set_sms_callback(Callback<void()> func);

    /** CPMS preferred message storage
     *
     *  @param memr        memory from which messages are read and deleted
     *                     "SM" - SIM SMS memory storage (default)
     *                     "ME" - NVM SMS storage
     *  @param memw        memory to which writing and sending operations are made
     *                     "SM" - SIM SMS memory storage (default)
     *                     "ME" - NVM SMS storage
     *  @param mems        memory to which received SMs are preferred to be stored
     *                     "SM" - SIM SMS memory storage (default)
     *                     "ME" - NVM SMS storage
     *
     * @return             zero for success
     */
    virtual nsapi_error_t set_cpms(const char *memr, const char *memw, const char *mems);

    /** CSCA - set Service Center Address
     *
     *  @param sca        Service Center Address to be used for mobile originated SMS transmissions.
     *  @param type       129 - national numbering scheme, 145 - international numbering scheme (contains the character "+")
     *
     *  @return           zero for success
     */
    virtual nsapi_error_t set_csca(const char *sca, int type);

    /** Set command sets the current character set used by the device. "GSM", "IRA",....
     *
     *  @remark Current implementation support only ASCII so choose the correct character set.
     *
     *  @param chr_set   preferred character set list (comma separated). Modem might not support the wanted character set
     *                   so chr_set list is looped from start until supported set is found. Used character set index is returned.
     *                   See more from 3GPP TS 27.005.
     * @return           Used character set index from the given list in case of success. Otherwise negative errorcode.
     */
    virtual nsapi_size_or_error_t set_cscs(const char *chr_set);

    /** Deletes all messages from the currently set memory/SIM
     *
     *  @return zero for success
     */
    virtual nsapi_error_t delete_all_messages();

    /** Some modems need extra time between AT commands and responses or there will be error -314, SIM busy.
     *  If SIM busy errors are an issue this time should be increased. It can also be set to zero to make
     *  operations faster and more energy efficient if no errors will follow. By default wait time is zero.
     *
     *  @param sim_wait_time
     */
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
        sms_info_t() : msg_size(0), parts(1), parts_added(1), msg_ref_number(0), next_info(0){};
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
    nsapi_error_t delete_sms(sms_info_t* sms);

    /**
     * Internal helper methods
     */
    nsapi_error_t list_messages();
    int read_sms_params(char *, char *);
    void free_linked_list();
    void add_info(sms_info_t* info, int index, int part_number);
    int read_udh_from_pdu(const char* pdu, sms_info_t *info, int &part_number, int &parts, int &padding_bits);
    nsapi_size_or_error_t get_data_from_pdu(const char* pdu, sms_info_t *info, int *part_number,
                                            char *phone_number = NULL, char *msg = NULL);
    nsapi_size_or_error_t read_pdu_payload(const char* pdu, int scheme, char *msg, int padding_bits, bool last_part);
    sms_info_t* get_oldest_sms_index();
    bool create_time(const char* time_string, time_t* time);
    int compare_time_strings(const char* time_string_1, const char* time_string_2);
    char* create_pdu(const char* phone_number, const char* message, uint8_t message_length, uint8_t msg_parts,
                     uint8_t msg_part_number);
    nsapi_size_or_error_t read_sms_from_index(int msg_index, char* buf, uint16_t len, char* phone_num,
                                              char* time_stamp);
    nsapi_size_or_error_t read_sms(sms_info_t* sms, char* buf, char* phone_num, char* time_stamp);

    /** Packs the given str from ascii to 7bit gsm format and converts it to hex to the given buf.
     *
     *  @param str                   string which is to be converted
     *  @param len                   length of the str buffer
     *  @param buf                   preallocated buffer which holds the converted string in hex format after successful call
     *  @param number_of_padding_bit padding bits needed to keep the octet boundary
     *  @return                      length of buffer buf or zero on failure
     */
    uint16_t pack_7_bit_gsm_and_hex(const char* str, uint16_t len, char *buf, int number_of_padding_bit);

    /** Unpacks the given hex- and 7-bit gsm encoded str to ascii string
     *
     *  @param str          string which converted to ascii string to buf
     *  @param len          length of the str divided by two as str is hexencoded
     *  @param buf          preallocated destination buffer
     *  @param padding_bits number of padding bits which were needed to hold the octet boundary
     *  @param last_part    true is last part of the encoded message
     *  @return             length of the destination buffer buf
     *
     */
    uint16_t unpack_7_bit_gsm_to_str(const char* str, int len, char *buf, int padding_bits,
                                         bool last_part);
};

} // namespace mbed

#endif // AT_CELLULAR_SMS_H_
