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
#ifndef M2M_SECURITY_H
#define M2M_SECURITY_H

#include "mbed-client/m2mobject.h"

// FORWARD DECLARATION
class M2MResource;

/**
 *  \brief M2MSecurity.
 *  This class represents an interface for the Security Object model of the LWM2M framework.
 *  It handles the security object instances and all corresponding
 *  resources.
 */

class  M2MSecurity : public M2MObject {

friend class M2MInterfaceFactory;
friend class M2MNsdlInterface;

public:

    /**
     * \brief Enum defining all resources associated with a
     * Security Object in the LWM2M framework.
     */
    typedef enum {
        M2MServerUri,
        BootstrapServer,
        SecurityMode,
        PublicKey,
        ServerPublicKey,
        Secretkey,
        SMSSecurityMode,
        SMSBindingKey,
        SMSBindingSecretKey,
        M2MServerSMSNumber,
        ShortServerID,
        ClientHoldOffTime
    }SecurityResource;

    /**
     * \brief Enum defining the type of a security attribute
     * used by the Security Object.
     */
    typedef enum {
        SecurityNotSet = -1,
        Psk = 0,
        Certificate = 2,
        NoSecurity = 3
    } SecurityModeType;

    /**
     * \brief Enum defining an interface operation that can be
     * handled by the Security Object.
     */
    typedef enum {
        Bootstrap = 0x0,
        M2MServer = 0x1
    } ServerType;

private:

    /**
     * \brief Constructor
     * \param server_type The type of the security object created. Either bootstrap or LWM2M server.
     */
    M2MSecurity(ServerType server_type);

    // Prevents the use of default constructor.
    M2MSecurity();

    // Prevents the use of assignment operator.
    M2MSecurity& operator=( const M2MSecurity& /*other*/ );

    // Prevents the use of copy constructor
    M2MSecurity( const M2MSecurity& /*other*/ );

public:

    /**
     * \brief Destructor
     */
    virtual ~M2MSecurity();

    /**
     * \brief Creates a new resource for the given resource enum.
     * \param rescource With this function, the following resources can be created:
     * ' BootstrapServer', 'SecurityMode', 'SMSSecurityMode',
     * 'M2MServerSMSNumber', 'ShortServerID', 'ClientHoldOffTime'.
     * \param value The value to be set on the resource, in Integer format.
     * \return M2MResource if created successfully, else NULL.
     */
    M2MResource* create_resource(SecurityResource rescource, uint32_t value);

    /**
     * \brief Deletes the resource with the given resource enum.
     * Mandatory resources cannot be deleted.
     * \param resource The resource to be deleted.
     * \return True if deleted, else false.
     */
    bool delete_resource(SecurityResource rescource);

    /**
     * \brief Sets the value of the given resource enum.
     * \param resource With this function, a value can be set for the following resources:
     * 'M2MServerUri', 'SMSBindingKey', 'SMSBindingSecretKey'.
     * \param value The value to be set on the resource, in String format.
     * \return True if successfully set, else false.
     */
    bool set_resource_value(SecurityResource resource,
                            const String &value);

    /**
     * \brief Sets the value of the given resource enum.
     * \param resource With this function, a value can be set for the following resourecs:
     * 'BootstrapServer', 'SecurityMode', 'SMSSecurityMode',
     * 'M2MServerSMSNumber', 'ShortServerID', 'ClientHoldOffTime'.
     * \param value The value to be set on the resource, in Integer format.
     * \return True if successfully set, else false.
     */
    bool set_resource_value(SecurityResource resource,
                            uint32_t value);

    /**
     * \brief Sets the value of the given resource enum.
     * \param resource With this function, a value can be set for the follwing resources:
     * 'PublicKey', 'ServerPublicKey', 'Secretkey'.
     * \param value The value to be set on the resource, in uint8_t format.
     * \param size The size of the buffer value to be set on the resource.
     * \return True if successfully set, else false.
     */
    bool set_resource_value(SecurityResource resource,
                            const uint8_t *value,
                            const uint16_t length);

    /**
     * \brief Returns the value of the given resource enum, in String.
     * \param resource With this function, the following resources can return a value:
     * 'M2MServerUri','SMSBindingKey', 'SMSBindingSecretKey'.
     * \return The value associated with the resource. If the resource is not valid an empty string is returned.
     */
    String resource_value_string(SecurityResource resource) const;

    /**
     * \brief Populates the data buffer and returns the size of the buffer.
     * \param resource With this function, the following resources can return a value:
     * 'PublicKey', 'ServerPublicKey', 'Secretkey'.
     * \param [OUT]data Copy of the data buffer that contains the value. Caller
     * is responsible of freeing this buffer.
     * \return The size of the populated buffer.
     */
    uint32_t resource_value_buffer(SecurityResource resource,
                                   uint8_t *&data) const;

    /**
     * \brief Returns the pointer to the value and size of the buffer.
     * \param resource With this function, the following resources can return a value:
     * 'PublicKey', 'ServerPublicKey', 'Secretkey'.
     * \param [OUT]data Pointer to the data buffer that contains the value.
     * \return The size of the populated buffer.
     */
    uint32_t resource_value_buffer(SecurityResource resource,
                                   const uint8_t *&data) const;

    /**
     * \brief Returns the value of the given resource name, in Integer.
     * \param resource With this function, the following resources can return a value:
     * 'BootstrapServer', 'SecurityMode', 'SMSSecurityMode',
     * 'M2MServerSMSNumber', 'ShortServerID', 'ClientHoldOffTime'.
     * \return The value associated with the resource. If the resource is not valid 0 is returned.
     */
    uint32_t resource_value_int(SecurityResource resource) const;


    /**
     * \brief Returns whether the resource instance with the given resource enum exists or not
     * \param resource Resource enum.
     * \return True if at least one instance exists, else false.
     */
    bool is_resource_present(SecurityResource resource)const;

    /**
     * \brief Returns the total number of resources for a security object.
     * \return The total number of resources.
     */
    uint16_t total_resource_count()const;

    /**
     * \brief Returns the type of the Security Object. It can be either
     * Bootstrap or M2MServer.
     * \return ServerType The type of the Security Object.
     */
    ServerType server_type() const;

private:

    M2MResource* get_resource(SecurityResource resource) const;
    void clear_resources();

private:

    M2MObjectInstance*    _server_instance;
    ServerType            _server_type;

    friend class Test_M2MSecurity;
    friend class Test_M2MInterfaceImpl;
    friend class Test_M2MConnectionSecurityImpl;
    friend class Test_M2MConnectionHandlerPimpl_linux;
    friend class Test_M2MConnectionHandlerPimpl_mbed;
    friend class Test_M2MConnectionSecurityPimpl;
    friend class Test_M2MNsdlInterface;
};

#endif // M2M_SECURITY_H


