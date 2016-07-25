/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __M2M_CONNECTION_SECURITY_PIMPL_H__
#define __M2M_CONNECTION_SECURITY_PIMPL_H__

#include "mbed-client/m2mconnectionsecurity.h"
#include "mbed-client/m2mtimerobserver.h"
#include "mbed-client/m2mconstants.h"
#include "mbed-client/m2msecurity.h"

#include "mbedtls/config.h"
#include "mbedtls/platform.h"
#include "mbedtls/debug.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/error.h"
#include "mbedtls/certs.h"
#include "mbedtls/entropy_poll.h"

class M2MTimer;

//TODO: Should we let application to select these or not??
const static int PSK_SUITES[] = {
    MBEDTLS_TLS_PSK_WITH_AES_128_CBC_SHA256,
    MBEDTLS_TLS_PSK_WITH_AES_256_CCM_8,
    MBEDTLS_TLS_PSK_WITH_AES_128_CCM_8,
    0
};


/**
 * @brief The M2MConnectionSecurityPimpl class
 */
class M2MConnectionSecurityPimpl : public M2MTimerObserver {

private:

    // Prevents the use of assignment operator by accident.
    M2MConnectionSecurityPimpl& operator=( const M2MConnectionSecurityPimpl& /*other*/ );
    // Prevents the use of copy constructor by accident
    M2MConnectionSecurityPimpl( const M2MConnectionSecurityPimpl& /*other*/ );

public:

    /**
     * @brief Constructor
     */
    M2MConnectionSecurityPimpl(M2MConnectionSecurity::SecurityMode mode);

    /**
    * @brief Destructor
    */
    virtual ~M2MConnectionSecurityPimpl();

    /**
     * \brief Resets the socket connection states.
     */
    void reset();

    /**
     * \brief Initiatlizes the socket connection states.
     */
    int init(const M2MSecurity *security);

    /**
     * \brief Starts the connection in non-blocking mode.
     * \param connHandler The ConnectionHandler object that maintains the socket.
     * \return Returns the state of the connection. Successful or not.
     */
    int start_connecting_non_blocking(M2MConnectionHandler* connHandler);

    /**
     * \brief Continues connectivity logic for secure connection.
     * \return Returns an error code if any while continuing the connection sequence.
     */
    int continue_connecting();

    /**
     * \brief Connects the client to the server.
     * \param connHandler The ConnectionHandler object that maintains the socket.
     * \return Returns the state of the connection. Successful or not.
     */
    int connect(M2MConnectionHandler* connHandler);

    /**
     * \brief Sends data to the server.
     * \param message The data to be sent.
     * \param len The length of the data.
     * @return Indicates whether the data is sent successfully or not.
     */
    int send_message(unsigned char *message, int len);

    /**
     * \brief Reads the data received from the server.
     * \param message The data to be read.
     * \param len The length of the data.
     * \return Indicates whether the data is read successfully or not.
     */
    int read(unsigned char* buffer, uint16_t len);

    /**
     * \brief Sets the function callback that will be called by mbed-client for
     * fetching random number from application for ensuring strong entropy.
     * \param random_callback A function pointer that will be called by mbed-client
     * while performing secure handshake.
     * Function signature should be uint32_t (*random_number_callback)(void);
     */
    void set_random_number_callback(random_number_cb callback);

    /**
     * \brief Sets the function callback that will be called by mbed-client for
     * providing entropy source from application for ensuring strong entropy.
     * \param entropy_callback A function pointer that will be called by mbed-client
     * while performing secure handshake.
     * Function signature , if using mbed-client-mbedtls should be
     * int (*mbedtls_entropy_f_source_ptr)(void *data, unsigned char *output,
     *                                     size_t len, size_t *olen);
     */
    void set_entropy_callback(entropy_cb callback);

protected: //From M2MTimerObserver

    virtual void timer_expired(M2MTimerObserver::Type type);

private:

    int start_handshake();

private:

    bool                        _init_done;
    mbedtls_ssl_config          _conf;
    mbedtls_ssl_context         _ssl;
    mbedtls_x509_crt            _cacert;
    mbedtls_x509_crt            _owncert;
    mbedtls_pk_context          _pkey;
    mbedtls_ctr_drbg_context    _ctr_drbg;
    mbedtls_entropy_context     _entropy;
    uint32_t                    _flags;
    M2MTimer                    *_timer;
    M2MConnectionSecurity::SecurityMode _sec_mode;

    friend class Test_M2MConnectionSecurityPimpl;
};

#endif //__M2M_CONNECTION_SECURITY_PIMPL_H__
