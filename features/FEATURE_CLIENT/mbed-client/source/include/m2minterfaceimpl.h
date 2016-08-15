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
#ifndef M2M_INTERFACE_IMPL_H
#define M2M_INTERFACE_IMPL_H

#include "mbed-client/m2minterface.h"
#include "mbed-client/m2mserver.h"
#include "mbed-client/m2mconnectionobserver.h"
#include "mbed-client/m2mconnectionsecurity.h"
#include "include/m2mnsdlobserver.h"
#include "mbed-client/m2mtimerobserver.h"

//FORWARD DECLARATION
class M2MNsdlInterface;
class M2MConnectionHandler;
class M2MConnectionSecurity;
class EventData;
class M2MTimer;

/**
 *  @brief M2MInterfaceImpl.
 *  This class implements handling of all mbed Client Interface operations
 *  defined in OMA LWM2M specifications.
 *  This includes Bootstrapping, Client Registration, Device Management &
 *  Service Enablement and Information Reporting.
 */

class  M2MInterfaceImpl : public M2MInterface,
                          public M2MNsdlObserver,
                          public M2MConnectionObserver,
                          public M2MTimerObserver
{
private:
    // Prevents the use of assignment operator by accident.
    M2MInterfaceImpl& operator=( const M2MInterfaceImpl& /*other*/ );

    // Prevents the use of copy constructor by accident
    M2MInterfaceImpl( const M2MInterfaceImpl& /*other*/ );

friend class M2MInterfaceFactory;

private:

    /**
     * @brief Constructor
     * @param observer, Observer to pass the event callbacks for various
     * interface operations.
     * @param endpoint_name, Endpoint name of the client.
     * @param endpoint_type, Endpoint type of the client.
     * @param life_time, Life time of the client in seconds
     * @param listen_port, Listening port for the endpoint, default is 8000.
     * @param domain, Domain of the client.
     * @param mode, Binding mode of the client, default is UDP
     * @param stack, Network Stack to be used for connection, default is LwIP_IPv4
     * @param context_address, Context address default is empty.
     */
    M2MInterfaceImpl(M2MInterfaceObserver& observer,
                     const String &endpoint_name,
                     const String &endpoint_type,
                     const int32_t life_time,
                     const uint16_t listen_port,
                     const String &domain = "",
                     BindingMode mode = M2MInterface::NOT_SET,
                     M2MInterface::NetworkStack stack = M2MInterface::LwIP_IPv4,
                     const String &context_address = "");

public:

    /**
     * @brief Destructor
     */
    virtual ~M2MInterfaceImpl();

    /**
     * @brief Initiates bootstrapping of the client with the provided Bootstrap
     * server information.
     * @param security_object, Security object which contains information
     * required for successful bootstrapping of the client.
     */
    virtual void bootstrap(M2MSecurity *security);

    /**
     * @brief Cancels on going bootstrapping operation of the client. If the client has
     * already successfully bootstrapped then this function deletes existing
     * bootstrap information from the client.
     */
    virtual void cancel_bootstrap();

    /**
     * @brief Initiates registration of the provided Security object to the
     * corresponding LWM2M server.
     * @param security_object, Security object which contains information
     * required for registering to the LWM2M server.
     * If client wants to register to multiple LWM2M servers then it has call
     * this function once for each of LWM2M server object separately.
     * @param object_list, Objects which contains information
     * which the client want to register to the LWM2M server.
     */
    virtual void register_object(M2MSecurity *security_object, const M2MObjectList &object_list);

    /**
     * @brief Updates or refreshes the client's registration on the LWM2M
     * server.
     * @param security_object, Security object from which the device object
     * needs to update registration, if there is only one LWM2M server registered
     * then this parameter can be NULL.
     * @param lifetime, Lifetime for the endpoint client in seconds.
     */
    virtual void update_registration(M2MSecurity *security_object, const uint32_t lifetime = 0);

    /**
     * @brief Unregisters the registered object from the LWM2M server
     * @param security_object, Security object from which the device object
     * needs to be unregistered, if there is only one LWM2M server registered
     * then this parameter can be NULL.
     */
    virtual void unregister_object(M2MSecurity* security = NULL);

    /**
     * @brief Sets the function which will be called indicating client
     * is going to sleep when the Binding mode is selected with Queue mode.
     * @param callback, Function pointer which will be called when client
     * goes to seleep.
     */
    virtual void set_queue_sleep_handler(callback_handler handler);

    /**
     * @brief Sets the network interface handler that is used by client to connect
     * to a network over IP..
     * @param handler A network interface handler that is used by client to connect.
     *  This API is optional but provides a mechanism for different platforms to
     * manage usage of underlying network interface by client.
     */
    virtual void set_platform_network_handler(void *handler = NULL);

/**
     * \brief Sets the function callback that will be called by mbed-client for
     * fetching random number from application for ensuring strong entropy.
     * \param random_callback A function pointer that will be called by mbed-client
     * while performing secure handshake.
     * Function signature should be uint32_t (*random_number_callback)(void);
     */
    virtual void set_random_number_callback(random_number_cb callback);

    /**
     * \brief Sets the function callback that will be called by mbed-client for
     * providing entropy source from application for ensuring strong entropy.
     * \param entropy_callback A function pointer that will be called by mbed-client
     * while performing secure handshake.
     * Function signature , if using mbed-client-mbedtls should be
     * int (*mbedtls_entropy_f_source_ptr)(void *data, unsigned char *output,
     *                                     size_t len, size_t *olen);
     */
    virtual void set_entropy_callback(entropy_cb callback);


protected: // From M2MNsdlObserver

    virtual void coap_message_ready(uint8_t *data_ptr,
                                    uint16_t data_len,
                                    sn_nsdl_addr_s *address_ptr);

    virtual void client_registered(M2MServer *server_object);

    virtual void registration_updated(const M2MServer &server_object);

    virtual void registration_error(uint8_t error_code, bool retry = false);

    virtual void client_unregistered();

    virtual void bootstrap_done(M2MSecurity *security_object);

    virtual void bootstrap_error();

    virtual void coap_data_processed();

    virtual void value_updated(M2MBase *base);

protected: // From M2MConnectionObserver

    virtual void data_available(uint8_t* data,
                                uint16_t data_size,
                                const M2MConnectionObserver::SocketAddress &address);

    virtual void socket_error(uint8_t error_code, bool retry = true);

    virtual void address_ready(const M2MConnectionObserver::SocketAddress &address,
                               M2MConnectionObserver::ServerType server_type,
                               const uint16_t server_port);

    virtual void data_sent();

protected: // from M2MTimerObserver

    virtual void timer_expired(M2MTimerObserver::Type type);


private: // state machine state functions

    /**
    * When the state is Idle.
    */
    void state_idle(EventData* data);

    /**
    * When the client starts bootstrap.
    */
    void state_bootstrap( EventData *data);

    /**
    * When the bootstrap server address is resolved.
    */
    void state_bootstrap_address_resolved( EventData *data);

    /**
    * When the bootstrap resource is created.
    */
    void state_bootstrap_resource_created( EventData *data);

    /**
    * When the server has sent response and bootstrapping is done.
    */
    void state_bootstrapped( EventData *data);

    /**
    * When the client starts register.
    */
    void state_register( EventData *data);

    /**
    * When the server address for register is resolved.
    */
    void state_register_address_resolved( EventData *data);

    /**
    * When the register resource is created.
    */
    void state_register_resource_created( EventData *data);

    /**
    * When the client is registered.
    */
    void state_registered( EventData *data);

    /**
    * When the client is updating registration.
    */
    void state_update_registration( EventData *data);

    /**
    * When the client starts unregister.
    */
    void state_unregister( EventData *data);

    /**
    * When the client has been unregistered.
    */
    void state_unregistered( EventData *data);

    /**
    * When the coap data is been sent through socket.
    */
    void state_sending_coap_data( EventData *data);

    /**
    * When the coap data is sent successfully.
    */
    void state_coap_data_sent( EventData *data);

    /**
    * When the socket is receiving coap data.
    */
    void state_receiving_coap_data( EventData *data);

    /**
    * When the socket has received coap data.
    */
    void state_coap_data_received( EventData *data);

    /**
    * When the coap message is being processed.
    */
    void state_processing_coap_data( EventData *data);

    /**
    * When the coap message has been processed.
    */
    void state_coap_data_processed( EventData *data);

    /**
    * When the client is waiting to receive or send data.
    */
    void state_waiting( EventData *data);

    /**
    * State enumeration order must match the order of state
    * method entries in the state map
    */
    enum E_States {
        STATE_IDLE = 0,
        STATE_BOOTSTRAP,
        STATE_BOOTSTRAP_ADDRESS_RESOLVED,
        STATE_BOOTSTRAP_RESOURCE_CREATED,
        STATE_BOOTSTRAPPED,
        STATE_REGISTER, //5
        STATE_REGISTER_ADDRESS_RESOLVED,
        STATE_REGISTER_RESOURCE_CREATED,
        STATE_REGISTERED,
        STATE_UPDATE_REGISTRATION,
        STATE_UNREGISTER, //10
        STATE_UNREGISTERED,
        STATE_SENDING_COAP_DATA,
        STATE_COAP_DATA_SENT,
        STATE_COAP_DATA_RECEIVED,
        STATE_PROCESSING_COAP_DATA, //15
        STATE_COAP_DATA_PROCESSED,
        STATE_WAITING,
        STATE_MAX_STATES
    };

    /**
     * @brief Redirects the state machine to right function.
     * @param current_state Current state to be set.
     * @param data, Data to be passed to the state function.
     */
    void state_function( uint8_t current_state, EventData* data  );

    /**
     * @brief State Engine maintaining state machine logic.
     */
    void state_engine(void);

    /**
    * External event which can trigger the state machine.
    * @param New State which the state machine should go to.
    * @param data to be passed to the state machine.
    */
    void external_event(uint8_t, EventData* = NULL);

    /**
    * Internal event generated by state machine.
    * @param New State which the state machine should go to.
    * @param data to be passed to the state machine.
    */
    void internal_event(uint8_t, EventData* = NULL);

    enum
    {
        EVENT_IGNORED = 0xFE,
        CANNOT_HAPPEN
    };

    /**
     * Helper method for extracting the IP address part and port from the
     * given server address.
     * @param server_address source url (without "coap" or "coaps" prefix)
     * @param ip_address extracted IP
     * @param port extracted port
     */
    static void process_address(const String& server_address, String& ip_address, uint16_t& port);

private:

    M2MInterfaceObserver        &_observer;
    M2MConnectionHandler        *_connection_handler;
    M2MConnectionSecurity       *_security_connection; // Doesn't own
    M2MNsdlInterface            *_nsdl_interface;
    uint8_t                     _current_state;
    const int                   _max_states;
    bool                        _event_generated;
    EventData                   *_event_data;
    String                      _endpoint_type;
    String                      _domain;
    int32_t                     _life_time;
    BindingMode                 _binding_mode;
    String                      _context_address;
    uint16_t                    _listen_port;
    uint16_t                    _server_port;
    String                      _server_ip_address;
    M2MSecurity                 *_register_server; //TODO: to be the list not owned
    bool                        _event_ignored;
    bool                        _register_ongoing;
    bool                        _update_register_ongoing;
    M2MTimer                    *_queue_sleep_timer;
    M2MTimer                    *_retry_timer;
    M2MTimer                    *_bootstrap_timer;
    callback_handler            _callback_handler;
    M2MSecurity                 *_security;
    uint8_t                     _retry_count;
    bool                        _reconnecting;
    bool                        _retry_timer_expired;

   friend class Test_M2MInterfaceImpl;

};

#define BEGIN_TRANSITION_MAP \
    static const uint8_t TRANSITIONS[] = {\

#define TRANSITION_MAP_ENTRY(entry)\
    entry,

#define END_TRANSITION_MAP(data) \
    0 };\
    external_event(TRANSITIONS[_current_state], data);

#endif //M2M_INTERFACE_IMPL_H


