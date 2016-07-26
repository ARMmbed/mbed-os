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
#ifndef __M2M_CONNECTION_SECURITY_H__
#define __M2M_CONNECTION_SECURITY_H__

#include "mbed-client/m2mconfig.h"
class M2MConnectionHandler;
class M2MSecurity;
class M2MConnectionSecurityPimpl;

/**
 * \brief M2MConnectionSecurity.
 * This class provides a method to create a secure socket connection
 * to handle connectivity for the mbed Client. It will handle sending, receiving
 * and establishing a secure connection for mbed Client on top of the
 * normal socket connection.
 */

class M2MConnectionSecurity {
public:
    typedef enum {
        NO_SECURITY = 0,
        TLS,
        DTLS
    } SecurityMode;

private:
    // Prevents the use of assignment operator by accident.
    M2MConnectionSecurity& operator=( const M2MConnectionSecurity& /*other*/ );
    // Prevents the use of copy constructor by accident.
    M2MConnectionSecurity( const M2MConnectionSecurity& /*other*/ );

public:
    /**
     * \brief Default Constructor.
     */
    M2MConnectionSecurity(SecurityMode mode);

    /**
     * \brief Default Destructor.
     */
    ~M2MConnectionSecurity();

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
     * \return Indicates whether the data is sent successfully or not.
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
     * \brief Sets the function callback that is called by mbed-client to
     * fetch a random number from an application to ensure strong entropy.
     * \param random_callback A function pointer that is called by mbed-client
     * while performing a secure handshake.
     * The function signature should be uint32_t (*random_number_callback)(void);
     */
    void set_random_number_callback(random_number_cb callback);

    /**
     * \brief Sets the function callback that is called by mbed-client to
     * provide an entropy source from an application to ensure strong entropy.
     * \param entropy_callback A function pointer that is called by mbed-client
     * while performing a secure handshake.
     * Function signature, if using mbed-client-mbedtls, should be
     * int (*mbedtls_entropy_f_source_ptr)(void *data, unsigned char *output,
     *                                     size_t len, size_t *olen);
     */
    void set_entropy_callback(entropy_cb callback);

private:

    M2MConnectionSecurityPimpl* _private_impl;

    friend class Test_M2MConnectionSecurity;
    //friend class Test_M2MConnectionSecurityImpl;
};

#endif //__M2M_CONNECTION_SECURITY_H__
