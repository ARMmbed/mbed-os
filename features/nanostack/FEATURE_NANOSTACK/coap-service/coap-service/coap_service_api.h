/*
 * Copyright (c) 2015-2017, Arm Limited and affiliates.
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

#ifndef COAP_SERVICE_API_H_
#define COAP_SERVICE_API_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>

#include "ns_types.h"
#include "mbed-coap/sn_coap_header.h"
#include "ns_address.h"

/**
 * This interface is used in sending and receiving of CoAP messages to multicast address and receive multiple responses.
 */

// Allowed_methods
#define COAP_SERVICE_ACCESS_ALL_ALLOWED         0x0F
#define COAP_SERVICE_ACCESS_GET_ALLOWED         0x01
#define COAP_SERVICE_ACCESS_PUT_ALLOWED         0x02
#define COAP_SERVICE_ACCESS_POST_ALLOWED        0x04
#define COAP_SERVICE_ACCESS_DELETE_ALLOWED      0x08

// Bits for service options
#define COAP_SERVICE_OPTIONS_NONE  	        	0x00
#define COAP_SERVICE_OPTIONS_VIRTUAL_SOCKET     0x01
#define COAP_SERVICE_OPTIONS_SECURE 	        0x02
#define COAP_SERVICE_OPTIONS_EPHEMERAL_PORT     0x04
/** Coap interface selected as socket interface */
#define COAP_SERVICE_OPTIONS_SELECT_SOCKET_IF   0x08
/** Register to COAP multicast groups */
#define COAP_SERVICE_OPTIONS_MULTICAST_JOIN     0x10
/** Link-layer security bypass option is set*/
#define COAP_SERVICE_OPTIONS_SECURE_BYPASS      0x80

// Bits for request options
#define COAP_REQUEST_OPTIONS_NONE  		        0x00
#define COAP_REQUEST_OPTIONS_ADDRESS_DEFAULT    0x00//!< default is not setting either short or long.
#define COAP_REQUEST_OPTIONS_ADDRESS_LONG       0x01
#define COAP_REQUEST_OPTIONS_ADDRESS_SHORT      0x02
#define COAP_REQUEST_OPTIONS_MULTICAST          0x04 //!< indicates that CoAP library support multicasting
#define COAP_REQUEST_OPTIONS_SECURE_BYPASS      0x08

extern const uint8_t COAP_MULTICAST_ADDR_LINK_LOCAL[16]; //!< ff02::fd, COAP link local multicast address
extern const uint8_t COAP_MULTICAST_ADDR_ADMIN_LOCAL[16]; //!< ff03::fd, COAP admin-local multicast address
extern const uint8_t COAP_MULTICAST_ADDR_SITE_LOCAL[16]; //!> ff05::fd, COAP site-local multicast address

/**
 * \brief Service message response receive callback.
 *
 * Function that handles CoAP service message receiving and parsing
 *
 * \param service_id       Service handle.
 * \param source_address   IPv6 source address.
 * \param source_port      Source port.
 * \param response_ptr     Pointer to CoAP header structure.
 *
 * \return 0 for success / -1 for failure
  */
typedef int coap_service_response_recv(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *response_ptr);

/**
 * \brief CoAP service request callback
 *
 * CoAP service request message receiving and parsing function
 *
 * \param service_id         Id number of the current service.
 * \param source_address     IPv6 source address.
 * \param source_port        Source port.
 * \param request_ptr        Pointer to CoAP header structure.
 *
 * \return -1 = Message ignored, no response will be sent. Transaction will be deleted.
 *          0 = Response is either already sent or will be send. Transaction is not deleted.
 */
typedef int coap_service_request_recv_cb(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *request_ptr);

/**
 * \brief Security service start callback
 *
 * Starts security service handling and fetches device password.
 *
 * \param service_id         Id number of the current service.
 * \param address            Address of sender.
 * \param port               Port of the device.
 * \param pw                 Pointer where to write the ecjpake password.
 * \param pw_len             Pointer where to write length of the ecjpake password.
 *
 * \return 0 for success / -1 for failure
 */
typedef int coap_service_security_start_cb(int8_t service_id, uint8_t address[static 16], uint16_t port, uint8_t* pw, uint8_t *pw_len);

/**
 * \brief CoAP service security done callback
 *
 * CoAP service security done callback function.
 *
 * \param service_id         Id number of the current service.
 * \param address            Address of sender.
 * \param keyblock           Security key (40 bits).
 *
 * \return 0 for success / -1 for failure
 */
typedef int coap_service_security_done_cb(int8_t service_id, uint8_t address[static 16], uint8_t keyblock[static 40]);

/**
 * \brief Initialise server instance.
 *
 * Initialise Thread services for the registered application.
 *
 * \param interface_id       Informs registered application interface id. This parameter is passed to socket implementation.
 * \param listen_port        Port that Application wants to use for communicate with coap server.
 * \param service_options    Options of the current service.
 * \param *start_ptr         Callback to inform security handling is started and to fetch device password.
 * \param *coap_security_done_cb  Callback to inform security handling is done.
 *
 *  \return service_id / -1 for failure
 */
extern int8_t coap_service_initialize(int8_t interface_id, uint16_t listen_port, uint8_t service_options, coap_service_security_start_cb *start_ptr, coap_service_security_done_cb *coap_security_done_cb);

/**
 * \brief Service delete
 *
 * Removes all data related to this instance
 *
 * \param service_id         Id number of the current service.
 */
extern void coap_service_delete( int8_t service_id );

/**
 * \brief Close secure connection
 *
 * Closes secure connection (if present), but leaves socket open.
 *
 * \param service_id            Id number of the current service.
 * \param destimation_addr_ptr  Connection destination address.
 * \param port                  Connection destination port.
 */
extern void coap_service_close_secure_connection(int8_t service_id, uint8_t destination_addr_ptr[static 16], uint16_t port);

/**
 * \brief Virtual socket sent callback.
 *
 * Sent data to virtual socket.
 *
 * \param service_id                       Id number of the current service.
 * \param destination_addr_ptr             Receiver IPv6 address.
 * \param port                             Receiver port number.
 * \param *data_ptr                        Pointer to the data.
 * \param data_len                         Lenght of the data.
 *
 * \return 0 for success / -1 for failure
  */
typedef int coap_service_virtual_socket_send_cb(int8_t service_id, uint8_t destination_addr_ptr[static 16], uint16_t port, const uint8_t *data_ptr, uint16_t data_len);

/**
 * \brief Virtual socket read.
 *
 * Receive data from virtual socket.
 *
 * \param service_id                       Id number of the current service.
 * \param source_addr_ptr                  Receiver IPv6 address.
 * \param port                             Receiver port number.
 * \param *data_ptr                        Pointer to the data
 * \param data_len                         Lenght of the data
 *
 * \return 0 for success / -1 for failure
  */
extern int16_t coap_service_virtual_socket_recv(int8_t service_id, uint8_t source_addr_ptr[static 16], uint16_t port, uint8_t *data_ptr, uint16_t data_len);

/**
 * \brief Set virtual socket
 *
 * Sets virtual socket for CoAP services.
 *
 * \param service_id         Id number of the current service.
 * \param *send_method_ptr   Callback to coap virtual socket.
 *
 * \return 0 for success / -1 for failure
 */
extern int16_t coap_service_virtual_socket_set_cb(int8_t service_id, coap_service_virtual_socket_send_cb *send_method_ptr);

/**
 * \brief Register unsecure callback methods to CoAP server
 *
 * Register application and informs CoAP services unsecure registery callback function.
 *
 * \param service_id       Id number of the current service.
 * \param *uri             Uri address.
 * \param allowed_method   Informs method that is allowed to use (used defines described above).
 * \param *request_recv_cb CoAP service request receive callback function pointer.
 *
 * \return 0 for success / -1 for failure
 */
extern int8_t coap_service_register_uri(int8_t service_id, const char *uri, uint8_t allowed_method, coap_service_request_recv_cb *request_recv_cb);

/**
 * \brief Unregister unsecure callback methods to CoAP server
 *
 * Register application and informs CoAP services unsecure registery callback function.
 *
 * \param service_id       Id number of the current service.
 * \param *uri             Uri address.
 *
 * \return 0 for success / -1 for failure
 */
extern int8_t coap_service_unregister_uri(int8_t service_id, const char *uri);

/**
 * \brief Sends CoAP service request
 *
 * Build and sends CoAP service request message.
 *
 * \param service_id            Id number of the current service.
 * \param options               Options defined above.
 * \param destination_addr      IPv6 address.
 * \param destination_port      Destination port
 * \param msg_type              Message type can be found from sn_coap_header.
 * \param msg_code              Message code can be found from sn_coap_header.
 * \param *uri                  Uri address.
 * \param cont_type             Content type can be found from sn_coap_header.
 * \param payload_ptr           Pointer to message content.
 * \param payload_len           Lenght of the message.
 * \param *request_response_cb  Callback to inform result of the request.
 *
 * \return msg_id               Id number of the current message.
 */
extern uint16_t coap_service_request_send(int8_t service_id, uint8_t options, const uint8_t destination_addr[static 16], uint16_t destination_port, sn_coap_msg_type_e msg_type, sn_coap_msg_code_e msg_code, const char *uri,
        sn_coap_content_format_e cont_type, const uint8_t *payload_ptr, uint16_t payload_len, coap_service_response_recv *request_response_cb);

/**
 * \brief Sends CoAP service response
 *
 * Build and sends CoAP service response message.
 *
 * \param service_id       Id number of the current service.
 * \param options          Options defined above.
 * \param request_ptr      Pointer to CoAP request message header structure.
 * \param message_code     Message code can be found from sn_coap_header.
 * \param content_type     Content type can be found from sn_coap_header.
 * \param payload_ptr      Pointer to message content.
 * \param payload_len      Lenght of the message.
 *
 * \return -1              For failure
 *-         0              For success
 */
extern int8_t coap_service_response_send(int8_t service_id, uint8_t options, sn_coap_hdr_s *request_ptr, sn_coap_msg_code_e message_code, sn_coap_content_format_e content_type, const uint8_t *payload_ptr,uint16_t payload_len);

/**
 * \brief Sends CoAP service response
 *
 * Build and sends CoAP service response message based on CoAP request message id.
 *
 * \param service_id       Id number of the current service.
 * \param options          Options defined above.
 * \param msg_id           Request messages ID.
 * \param msg_type         Message type can be found from sn_coap_header.
 * \param message_code     Message code can be found from sn_coap_header.
 * \param content_type     Content type can be found from sn_coap_header.
 * \param payload_ptr      Pointer to message content.
 * \param payload_len      Lenght of the message.
 *
 * \return -1              For failure
 *-         0              For success
 */
extern int8_t coap_service_response_send_by_msg_id(int8_t service_id, uint8_t options, uint16_t msg_id, sn_coap_msg_code_e message_code, sn_coap_content_format_e content_type, const uint8_t *payload_ptr,uint16_t payload_len);



/**
 * \brief Delete CoAP request transaction
 *
 * Removes pending CoAP transaction from service.
 *
 * \param service_id       Id number of the current service.
 * \param msg_id           Message ID number.
 *
 * \return -1              For failure
 *-         0              For success
 */
extern int8_t coap_service_request_delete(int8_t service_id, uint16_t msg_id);

/**
 * \brief Set DTLS handshake timeout values
 *
 * Configures the DTLS handshake timeout values.
 *
 * \param service_id       Id number of the current service.
 * \param min              Initial timeout value.
 * \param max              Maximum value of timeout.
 *
 * \return -1              For failure
 *-         0              For success
 */
extern int8_t coap_service_set_handshake_timeout(int8_t service_id, uint32_t min, uint32_t max);

/**
 * \brief Set DTLS handshake limit values
 *
 * Configures the limits for DTLS sessions. Values must be > 0.
 *
 * \param handshakes_max        Maximum amount of simultaneous handshakes.
 * \param connections_max       Maximum amount of sessions.
 *
 * \return -1              For failure
 *-         0              For success
 */
extern int8_t coap_service_handshake_limits_set(uint8_t handshakes_max, uint8_t connections_max);

/**
 * \brief Set CoAP duplication message buffer size
 *
 * Configures the CoAP duplication message buffer size.
 *
 * \param service_id       Id number of the current service.
 * \param size             Buffer size (messages).
 *
 * \return -1              For failure
 *-         0              For success
 */
extern int8_t coap_service_set_duplicate_message_buffer(int8_t service_id, uint8_t size);

/**
 * \brief Set DTLS certificates
 *
 * Set DTLS certificates.
 *
 * \param service_id       Id number of the current service.
 * \param cert             Pointer to certificate chain.
 * \param cert_len         Certificate length.
 * \param priv_key         pointer to private key.
 * \param priv_key_len     length of private key.
 *
 * \return -1              For failure
 *-         0              For success
 */

extern int8_t coap_service_certificate_set(int8_t service_id, const unsigned char *cert, uint16_t cert_len, const unsigned char *priv_key, uint8_t priv_key_len);
#ifdef __cplusplus
}
#endif

#endif /* COAP_SERVICE_API_H_ */
