# The Client Registration feature

The client uses the Client Registration to register with mbed Device Server, update registration and deregister.

Currently, only one-to-one client-server registration is supported. One-to-many client-server registrations will be supported in an upcoming release.

The Client Registration includes multiple sub-features. Currently supported:

- [Register](#the-register-feature)
- [Update](#the-update-feature)
- [Deregister](#the-deregister-feature)

## The Register feature

This API enables the client registration functionality.

When registering, the client:

* Performs the **Register** operation and provides parameters that mbed Device Server requires to register the client (for example Endpoint Name).

* Maintains the registration and session (for example, it sets the Lifetime and Queue Mode towards mbed Device Server).

* Provides information on the Objects the client supports and existing Object Instances in the client.

### Registering your client

To provide information to mbed Device Server and issue the register command:

**Step 1.** Create an mbed DS Object. This object contains information about mbed Device Server, such as its address and security mode.

```
#include "mbed-client/m2msecurity.h"
 M2MSecurity *security = M2MInterfaceFactory::create_security(M2MSecurity::M2MServer);
        if(security) {
            security->set_resource_value(M2MSecurity::M2MServerUri, LWM2M_SERVER_ADDRESS);
            security->set_resource_value(M2MSecurity::BootstrapServer, 0);
            security->set_resource_value(M2MSecurity::SecurityMode, M2MSecurity::NoSecurity);
        }
```

**Step 2.** mbed Client supports both non-secure and secure mode operations. For secure mode, you need to provide a certificate, private key and server public key through the API.

To create a secure mode operation:

```
#include "mbed-client/m2msecurity.h"
 M2MSecurity *security = M2MInterfaceFactory::create_security(M2MSecurity::M2MServer);
        if(security) {
            security->set_resource_value(M2MSecurity::M2MServerUri, LWM2M_SERVER_ADDRESS);
            security->set_resource_value(M2MSecurity::BootstrapServer, 0);
            security->set_resource_value(M2MSecurity::SecurityMode, M2MSecurity::Certificate);
            security->set_resource_value(M2MSecurity::ServerPublicKey,<SERVER_CERT>,sizeof(<SERVER_CERT>));
            security->set_resource_value(M2MSecurity::PublicKey,<CERT>,sizeof(<CERT>));
            security->set_resource_value(M2MSecurity::Secretkey,<KEY>,sizeof(<KEY>));   
        }
```

**Step 3.** Register all the resources that you would like to monitor or follow using mbed Device Server. To do this, create the resource objects and pass them to the Register API for registration purposes.

For example, if you want to register your OMA LWM2M based Device object, you need to create the object and set the values for mandatory resources as follows:

```
#include "mbed-client/m2mdevice.h"
M2MDevice *device = M2MInterfaceFactory::create_device();
if(device) {
    device->create_resource(M2MDevice::Manufacturer,MANUFACTURER);
	device->create_resource(M2MDevice::DeviceType,TYPE);
	device->create_resource(M2MDevice::ModelNumber,MODEL_NUMBER);
	device->create_resource(M2MDevice::SerialNumber,SERIAL_NUMBER);
    }
```

<span class="notes">**Note:** You can register other resources, including custom resources. Please check the [API documentation](https://docs.mbed.com/docs/mbed-client-guide/en/latest/api/annotated.html) for a detailed description of the M2MObject, M2MObjectInstance and M2MResource classes.</span>

**Step 4.** You have the registration server object and resources that you want to register. Now, you need to call the register API and pass the following objects as parameters:

```
M2MInterface::register_object(M2MSecurity* register_object, M2MObjectList object_list);
```

**Success or failure callback**

Because this is an asynchronous operation, you will receive the result of this operation through a callback defined in `m2minterfaceobserver.h` in your application.

_Success_

If the register operation is successful and the client can register all your resources to mbed DS, your application will receive the following callback:

```
void object_registered(M2MSecurity *server_object, const M2MServer& server)
```

The `M2MSecurity *server_object` specifies to which mbed Device Server instance the client has just registered and `M2MServer &server` contains the data related to mbed Device Server, including the Short ServerID and the client registration period.

_Failure_

If the registration operation fails for some reason, you will receive the following callback:

```
void error(M2MInterface::Error error)
```

You will get more information about the error from the `error` parameter passed with the callback; use it to fix the source of the error.

## The Update feature

Periodically, or in response to events within the client or as initiated by mbed Device Server, the client updates its registration information with mbed Device Server by sending it an **Update** operation.

Normally, the enabler will update the registration automatically, but if you want to renew the registration before that, you can use this API.

To update your registration:

```
M2MInterface::update_registration(M2MSecurity* security_object, const uint32_t lifetime)
```

**Success or failure callback**

_Success_

If the update operation is successful, your application will receive the following callback:

```
void registration_updated(M2MSecurity *const M2MServer& server)
```

The `M2MSecurity *server_object` specifies with which mbed Device Server instance the client has just updated the registration and `M2MServer &server` contains the data related to mbed Device Server, including the Short ServerID and the client registration period.

_Failure_

If the update operation fails for some reason, you will receive the following callback:

```
void error(M2MInterface::Error error)
```

## The Deregister feature

The client can deregister from mbed Device Server when it no longer requires access to the server. When mbed Device Server receives the **Deregister** message it removes the device's registration information from its database. When the client needs mbed Device Server again, it has to register again.

To deregister your endpoint client:

If the endpoint has multiple server registrations, you need to provide the `server_object` of the server from which would like to deregister. If there is only one registration, you can pass `NULL` and the client will deregister from the default registration.

```
M2MInterface::unregister_object(M2MSecurity *object);
```

**Success or failure callback**

Because this is an asynchronous operation, you will receive the result of this operation through a callback defined in `m2minterfaceobserver.h` in your application.

_Success_

If the client is successfully deregistered from mbed Device Server, your application will receive the following callback:

```
void object_unregistered(M2MSecurity *server_object)
```

The `M2MSecurity *server_object` specifies from which mbed Device Server instance the client has just deregistered.

_Failure_

If the deregistration operation fails for some reason, you will receive the following callback:

```
void error(M2MInterface::Error error)
```

You will get more information about the error from the `error` parameter passed with the callback; use it to fix the source of the problem.
