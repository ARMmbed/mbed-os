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
#ifndef M2M_CONNECTION_HANDLER_H__
#define M2M_CONNECTION_HANDLER_H__

#include "mbed-client/m2mconnectionobserver.h"
#include "mbed-client/m2mconfig.h"
#include "mbed-client/m2minterface.h"
#include "nsdl-c/sn_nsdl.h"

class M2MConnectionSecurity;
class M2MConnectionHandlerPimpl;

/**
 * \brief M2MConnectionHandler.
 * This class handles the socket connection for the LWM2M Client.
 */

class M2MConnectionHandler {
public:

    /**
     * @enum ConnectionError
     * This enum defines an error that can come from
     * socket read and write operation.
     */
    typedef enum {
        CONNECTION_ERROR_WANTS_READ = -1000,
        CONNECTION_ERROR_WANTS_WRITE = -1001,
        ERROR_NONE = 0,
        SSL_CONNECTION_ERROR,
        SOCKET_READ_ERROR,
        SOCKET_SEND_ERROR,
        SOCKET_ABORT,
        DNS_RESOLVING_ERROR,
        SSL_HANDSHAKE_ERROR
    }ConnectionError;


public:

    /**
    * \brief Constructor
    */
    M2MConnectionHandler(M2MConnectionObserver &observer,
                         M2MConnectionSecurity* sec,
                         M2MInterface::BindingMode mode,
                         M2MInterface::NetworkStack stack);

    /**
    * \brief Destructor
    */
    ~M2MConnectionHandler();

    /**
    * \brief This binds the socket connection.
    * \param listen_port Port to be listened to for an incoming connection.
    * \return True if successful, else false.
    */
    bool bind_connection(const uint16_t listen_port);

    /**
    * \brief This resolves the server address. Output is
    * returned through a callback.
    * \param String The server address.
    * \param uint16_t The server port.
    * \param ServerType The server type to be resolved.
    * \param security The M2MSecurity object that determines which
    * type of secure connection will be used by the socket.
    * \return True if address is valid, else false.
    */
    bool resolve_server_address(const String& server_address,
                                const uint16_t server_port,
                                M2MConnectionObserver::ServerType server_type,
                                const M2MSecurity* security);

    /**
    * \brief Sends data to the connected server.
    * \param data_ptr The data to be sent.
    * \param data_len The length of data to be sent.
    * \param address_ptr The address structure to which the data needs to be sent.
    * \return True if data is sent successfully, else false.
    */
    bool send_data(uint8_t *data_ptr,
                           uint16_t data_len,
                           sn_nsdl_addr_s *address_ptr);

    /**
    * \brief Listens to the incoming data from a remote server.
    * \return True if successful, else false.
    */
    bool start_listening_for_data();

    /**
    * \brief Stops listening to the incoming data.
    */
    void stop_listening();

    /**
     * \brief Sends directly to the socket. This is used by
     * security classes to send the data after it has been encrypted.
     * \param buf Buffer to send.
     * \param len The length of the buffer.
     * \return Number of bytes sent or -1 if failed.
     */
    int send_to_socket(const unsigned char *buf, size_t len);

    /**
     * \brief Receives directly from the socket. This
     * is used by the security classes to receive raw data to be decrypted.
     * \param buf Buffer to send.
     * \param len The length of the buffer.
     * \return Number of bytes read or -1 if failed.
     */
    int receive_from_socket(unsigned char *buf, size_t len);

    /**
    * \brief Closes the open connection.
    */
    void close_connection();

    /**
    * \brief Error handling for DTLS connectivity.
    * \param error Error code from the TLS library.
    */
    void handle_connection_error(int error);

    /**
     * \brief Sets the network interface handler that is used by client to connect
     * to a network over IP..
     * \param handler A network interface handler that is used by client to connect.
     *  This API is optional but provides a mechanism for different platforms to
     * manage usage of underlying network interface by client.
     */
    void set_platform_network_handler(void *handler = NULL);

    /**
    * \brief Claims mutex to prevent thread clashes
    * in multithreaded environment.
    */
    void claim_mutex();

    /**
    * \brief Releases mutex to prevent thread clashes
    * in multithreaded environment.
    */
    void release_mutex();

private:

    M2MConnectionObserver                       &_observer;
    M2MConnectionHandlerPimpl                   *_private_impl;

friend class Test_M2MConnectionHandler;
friend class Test_M2MConnectionHandler_mbed;
friend class Test_M2MConnectionHandler_linux;
friend class M2MConnection_TestObserver;
};

#endif //M2M_CONNECTION_HANDLER_H__

