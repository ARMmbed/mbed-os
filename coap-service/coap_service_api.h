/*
 *
 * Copyright (c) 2015 ARM. All rights reserved.
 *
 */

#ifndef COAP_SERVICE_API_H_
#define COAP_SERVICE_API_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>

#include <ns_types.h>
#include <sn_coap_header.h>

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
 
// Bits for request options
#define COAP_REQUEST_OPTIONS_NONE  		        0x00
#define COAP_REQUEST_OPTIONS_ADDRESS_DEFAULT    0x00//!< default is not setting either short or long.
#define COAP_REQUEST_OPTIONS_ADDRESS_LONG       0x01
#define COAP_REQUEST_OPTIONS_ADDRESS_SHORT      0x02
#define COAP_REQUEST_OPTIONS_MULTICAST          0x04 //!< indicates that CoAP library support multicasting
#define COAP_REQUEST_OPTIONS_SECURE_BYPASS      0x08


/**
 * \brief Initialise server instance.
 *
 * Initialise CoAP services for the registered application.
 *
 * \param interface_id     Informs registered application interface id. This parameter is passed to socket implementation.
 * \param listen_port      Port that Application wants to use for communicate with coap server.
 * \param service_options  Options of the current service.
 *
 *  \return service_id
 */
int8_t coap_service_initialize(int8_t interface_id, uint16_t listen_port, uint8_t service_options);

/**
 * \brief Service delete
 *
 * Removes all data related to this instance
 * 
 * \param service_id         Id number of the current service.
 */
void coap_service_delete( int8_t service_id );

/**
 * \brief CoAP service request callback
 *
 * CoAP service request message receiving and parsing function
 *
 * \param service_id         Id number of the current service.
 * \param source_address     IPv6 source address.
 * \param source_port        Source port
 * \param request_ptr        Pointer to CoAP header structure.
 *
 * \return Status
 */
typedef int (*coap_service_request_recv_cb)(int8_t service_id, uint8_t source_address[static 16], uint16_t source_port, sn_coap_hdr_s *request_ptr);

/**
 * \brief Security service start callback
 *
 * Starts security service handling.
 *
 * \param service_id         Id number of the current service.
 * \param EUI64              64 bit global identifier
 *
 * \return Status
 */
typedef int (*coap_service_security_start_cb)(int8_t service_id, uint8_t EUI64[static 8]);

/**
 * \brief CoAP service security done callback
 *
 * CoAP service security done callback function.
 *
 * \param service_id         Id number of the current service.
 * \param EUI64              64 bit global identifier
 * \param keyblock           Security key (40 bits)
 *
 * \return Status
 */
typedef int (*coap_service_security_done_cb)(int8_t service_id, uint8_t EUI64[static 8], uint8_t keyblock[static 40]);


/**
 * \brief Security service start callback
 *
 * Starts security service handling.
 *
 * \param EUI64              64 bit global identifier
 * \param PSKd_ptr           Pointer to PSK key.
 * \param PSKd_len           Lenght of PSK key.
 *
 * \return Status
 */
int coap_service_security_key_set(int8_t service_id, uint8_t EUI64[static 8], uint8_t *PSKd_ptr, uint8_t PSKd_len);

/**
 * \brief Service message response receive callback.
 *
 * Function that handles CoAP service message receiving and parsing
 *
 * \param msg_id           Id number of the current message.
 * \param response_ptr     Pointer to CoAP header structure.
 *
 * \return Status
  */
typedef int (*coap_service_response_recv)(uint16_t msg_id, sn_coap_hdr_s *response_ptr);

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
 * \return Status
  */
typedef int (*coap_service_virtual_socket_send_cb)(int8_t service_id, uint8_t destination_addr_ptr[static 16], uint16_t port, uint8_t *data_ptr, uint16_t data_len);

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
 * \return ?
  */
int16_t coap_service_virtual_socket_recv(int8_t service_id, uint8_t source_addr_ptr[static 16], uint16_t port, uint8_t *data_ptr, uint16_t data_len);

/**
 * \brief Set virtual socket
 *
 * Sets virtual socket for CoAP services.
 *
 * \param service_id         Id number of the current service.
 * \param *send_method_ptr   Callback to coap virtual socket.
 *
 * \return TODO
 */
int16_t coap_service_virtual_socket_set_cb(int8_t service_id, coap_service_virtual_socket_send_cb *send_method_ptr);

/**
 * \brief Register unsecure callback methods to CoAP server
 *
 * Register application and informs CoAP services unsecure registery callback function.
 *
 * \param service_id       Id number of the current service.
 * \param *uri             Pointer to uri.
 * \param uri_len          Length of uri.
 * \param port             port that Application wants to use for communicate with coap server.
 * \param allowed_method   Informs method that is allowed to use (used defines described above).
 * \param *request_recv_cb  CoAP service request receive callback function pointer.
 *
 * \return TODO
 */
int16_t coap_service_register_uri(int8_t service_id, uint16_t *uri, uint16_t uri_len, uint16_t port, uint8_t allowed_method, coap_service_request_recv_cb *request_recv_cb);

/**
 * \brief Register secure callback methods to CoAP server.
 *
 * Register application and informs CoAP services secure registery callback functions.
 *
 * \param service_id        Id number of the current service.
 * \param *start_ptr        Callback to inform security handling is started.
 * \param *security_done_cb Callback to inform security handling is done.
 *
 * \return TODO
 *
 * instance id that is used to identify registery.
 */
int16_t coap_service_register_uri_secure_cb_set(int8_t service_id, coap_service_security_start_cb *start_ptr, coap_service_security_done_cb *security_done_cb);

/**
 * \brief Sends CoAP service request
 *
 * Build and sends CoAP service request message.
 *
 * \param service_id            Id number of the current service.
 * \param options               Options defined above.
 * \param addr                  IPv6 address.
 * \param destination_port      Destination port
 * \param request_ptr           Pointer to CoAP header structure.
 * \param *request_response_cb   Callback to inform result of the request.
 *
 * \return msg_id               Id number of the current message.
 */
uint16_t coap_service_request_send(int8_t service_id, uint8_t options, uint8_t addr[static 16], uint16_t destination_port, sn_coap_hdr_s *request_ptr, coap_service_response_recv *request_response_cb);

/**
 * \brief Sends CoAP service response
 *
 * Build and sends CoAP service response message.
 *
 * \param service_id       Id number of the current service.
 * \param msg_id           Message ID number. 
 * \param options          Options defined above.
 * \param response_ptr     Pointer to CoAP header structure.
 *
 * \return TODO
 */
uint16_t coap_service_response_send(int8_t service_id, uint16_t msg_id, uint8_t options, sn_coap_hdr_s *response_ptr);

#ifdef __cplusplus
}
#endif

#endif /* COAP_SERVICE_API_H_ */
