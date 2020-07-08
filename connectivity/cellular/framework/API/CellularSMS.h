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

#ifndef CELLULAR_SMS_H_
#define CELLULAR_SMS_H_

#if MBED_CONF_CELLULAR_USE_SMS

#include "Callback.h"
#include "nsapi_types.h"

namespace mbed {

// including trailing '\0'
const uint16_t SMS_MAX_SIZE_WITH_CONCATENATION = 4096 + 1;
const uint16_t SMS_MAX_PHONE_NUMBER_SIZE = 20 + 1;
const uint16_t SMS_MAX_TIME_STAMP_SIZE = 20 + 1;

const uint16_t SMS_MAX_SIZE_8BIT_SINGLE_SMS_SIZE = 140;
const uint16_t SMS_MAX_SIZE_GSM7_SINGLE_SMS_SIZE = 160;

const uint16_t SMS_SIM_WAIT_TIME_MILLISECONDS = 200;

const int SMS_ERROR_MULTIPART_ALL_PARTS_NOT_READ = -5001;

/**
 * @addtogroup cellular
 * @{
 */

/**
 *  Class CellularSMS
 *
 *  An abstract interface for SMS sending, reading and deleting.
 */
class CellularSMS {
protected:
    // friend of CellularDevice so that it's the only way to close/delete this class.
    friend class CellularDevice;

    /**
     * virtual Destructor
     */
    virtual ~CellularSMS() {};

public:
    /* Enumeration for possible SMS modes, PDU and Text */
    enum CellularSMSMmode {
        CellularSMSMmodePDU = 0,
        CellularSMSMmodeText
    };

    enum CellularSMSEncoding {
        CellularSMSEncoding7Bit,
        CellularSMSEncoding8Bit,
    };

    /** Does all the necessary initializations needed for receiving and sending SMS.
     *
     *  @param mode          enumeration for choosing the correct mode: text/pdu
     *  @return              NSAPI_ERROR_OK on success
     *                       NSAPI_ERROR_NO_MEMORY on memory failure
     *                       NSAPI_ERROR_DEVICE_ERROR on other failures
     */
    virtual nsapi_error_t initialize(CellularSMSMmode mode,
                                     CellularSMSEncoding encoding = CellularSMSEncoding::CellularSMSEncoding7Bit) = 0;

    /** Send the SMS with the given parameters
     *
     *  @param phone_number  Phone number where to send SMS
     *  @param message       SMS message content
     *  @param msg_len       Length of the message
     *  @return              On success, length of the sent SMS (positive value)
     *                       NSAPI_ERROR_PARAMETER if invalid parameters
     *                       NSAPI_ERROR_NO_MEMORY on memory failure
     *                       NSAPI_ERROR_DEVICE_ERROR on other failures
     */
    virtual nsapi_size_or_error_t send_sms(const char *phone_number, const char *message, int msg_len) = 0;

    /** Gets the oldest received sms.
     *
     *  @param buf           preallocated buffer for SMS message content
     *  @param buf_len       length of allocated buf
     *  @param phone_num     preallocated buffer for phone number where SMS was sent
     *  @param phone_len     length of allocated phone_num  buffer
     *  @param time_stamp    preallocated buffer for TP-Service Centre Time Stamp (format: yy/MM/dd,hh:mm:ss-+zz). +-zz is timezone.
     *                       The unit of time zone is a quarter of an hour relative to GMT. For example +32 would be GMT+8.
     *  @param time_len      length of allocated time_stamp buffer
     *  @param buf_size      if method return error NSAPI_ERROR_NO_MEMORY because the given buf was not big enough, this
     *                       holds the size which is enough. Otherwise zero.
     *  @return              On success, length of the received SMS, (length of the buf, positive value)
     *                       NSAPI_ERROR_PARAMETER if invalid parameters
     *                       NSAPI_ERROR_NO_MEMORY on memory failure
     *                       SMS_ERROR_MULTIPART_ALL_PARTS_NOT_READ if SMS was multipart but not all parts are present/failed to read.
     *                       -1 if no SMS was found
     *                       NSAPI_ERROR_DEVICE_ERROR on other failures
     */
    virtual nsapi_size_or_error_t get_sms(char *buf, uint16_t buf_len, char *phone_num, uint16_t phone_len,
                                          char *time_stamp, uint16_t time_len, int *buf_size) = 0;

    /** Callback that is called when new SMS is received. SMS can be fetched using method get_sms().
     *
     * @remark      In PDU mode, there can be multipart SMS, and callback is called for every received part.
     *
     * @param func  Callback function that is called when new SMS is received.
     */
    virtual void set_sms_callback(Callback<void()> func) = 0;

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
     *  @return            NSAPI_ERROR_OK on success
     *                     NSAPI_ERROR_DEVICE_ERROR on failure
     */
    virtual nsapi_error_t set_cpms(const char *memr, const char *memw, const char *mems) = 0;

    /** CSCA - set Service Center Address
     *
     *  @param sca        Service Center Address to be used for mobile originated SMS transmissions.
     *  @param type       129 - national numbering scheme, 145 - international numbering scheme (contains the character "+")
     *
     *  @return           NSAPI_ERROR_OK on success
     *                    NSAPI_ERROR_DEVICE_ERROR on failure
     */
    virtual nsapi_error_t set_csca(const char *sca, int type) = 0;

    /** Set command sets the current character set used by the device. "GSM", "IRA",....
     *
     *  @remark Current implementation support only ASCII so choose the correct character set.
     *
     *  @param chr_set   preferred character set list (comma separated). Modem might not support the wanted character set,
     *                   so chr_set list is looped from start until supported set is found. Used character set index is returned.
     *                   See more from 3GPP TS 27.005.
     * @return           Used character set index from the given list in case of success.
     *                   NSAPI_ERROR_DEVICE_ERROR on failure
     */
    virtual nsapi_size_or_error_t set_cscs(const char *chr_set) = 0;

    /** Deletes all messages from the currently set memory/SIM
     *
     *  @return            NSAPI_ERROR_OK on success
     *                     NSAPI_ERROR_DEVICE_ERROR on failure
     */
    virtual nsapi_error_t delete_all_messages() = 0;

    /** Some modems need extra time between AT commands and responses, or there will be error -314, SIM busy.
     *  If SIM busy errors are an issue, this time should be increased. It can also be set to zero to make
     *  operations faster and more energy efficient if no errors will follow. By default, wait time is zero.
     *
     *  @param sim_wait_time
     */
    virtual void set_extra_sim_wait_time(int sim_wait_time) = 0;
};

/**
 * @}
 */

} // namespace mbed

#endif // MBED_CONF_CELLULAR_USE_SMS

#endif // CELLULAR_SMS_H_
