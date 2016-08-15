# The Device Management and Service Enabler feature

mbed Device Server uses the Device Management and Service Enabler to access Object Instances and Resources available on the client. The interface provides this access through the following operations:

- [Create](#the-create-operation)
- Delete
- [Read](#the-read-operation)
- [Write](#the-write-operation)
- [Write Attributes](#the-write-attributes-operation)
- [Execute](#the-execute-operation)

Currently, support for the Create and Delete actions is limited to Object Instances.

The Device Management and Service Enabler Interface supports the following data types:

- Text: for Resources.
- TLV: for Object and Object Instances.

## The Create operation

There are two types of resources you can create:

- Static: you set the value of the resource once and it does not change during the course of operations.
- Dynamic: the value is expected to change during the course of operations. Therefore, the value is fetched from setter APIs every time the server requests a read operation.

Here is an example of creating a custom static Resource:

```
#include "mbed-client/m2mobject.h"
#include "mbed-client/m2mobjectinstance.h"
#include "mbed-client/m2mresource.h"
_object = M2MInterfaceFactory::create_object("Test");
if(_object) {
    M2MObjectInstance* inst = _object->create_object_instance();
    if(inst) {
    	inst->create_static_resource("S",
        			     "ResourceTest",
                                     STATIC_VALUE,
                                     sizeof(STATIC_VALUE)-1);
```

And here is an example of creating a custom dynamic Resource:

```
#include "mbed-client/m2mobject.h"
#include "mbed-client/m2mobjectinstance.h"
#include "mbed-client/m2mresource.h"
_object = M2MInterfaceFactory::create_object("Test");
if(_object) {
    M2MObjectInstance* inst = _object->create_object_instance();
    if(inst) {
	M2MResource* res = inst->create_dynamic_resource("D","ResourceTest",true);
        char buffer[20];
        int size = sprintf(buffer,"%d",_value);
        res->set_operation(M2MBase::GET_PUT_ALLOWED);
        res->set_value((const uint8_t*)buffer,
                                   (const uint32_t)size);
```

For more information on different resource functionalities, please check the [API documentation](https://docs.mbed.com/docs/mbed-client-guide/en/latest/api/annotated.html) for the M2MObject, M2MObjectInstance and M2MResource classes.

## The Read operation

The Client API allows setting values to Resources, an array of Resource Instances, an Object Instance or all the Object Instances of an Object (TLV format supported). mbed Device Server can then read these values using the **Read** operation.

## The Write operation

The **Write** operation is used to overwrite the value of a Resource, an array of Resource Instances or multiple Resources from an Object Instance. 

Whenever there is a valid `PUT` operation for any of the resources, the application will receive a callback:

```
void value_updated(M2MBase *base, M2MBase::BaseType type) 
```
Where `M2MBase` is the Object whose value has been updated and `M2MBase::BaseType` is the object type.

Check the code snippet below for usage.
```
void value_updated(M2MBase *base, M2MBase::BaseType type) {
        M2MResource* resource = NULL;
        M2MResourceInstance* res_instance = NULL;
        M2MObjectInstance* obj_instance = NULL;
        M2MObject* obj = NULL;
        String object_name = "";
        String resource_name = "";
        uint16_t object_instance_id = 0;
        uint16_t resource_instance_id = 0;
        if(base) {
            switch(base->base_type()) {
                case M2MBase::Object:
                    obj = (M2MObject *)base;
                    object_name = obj->name();
                break;
                case M2MBase::ObjectInstance:
                    obj_instance = (M2MObjectInstance *)base;
                    object_name = obj_instance->name();
                    object_instance_id = obj_instance->instance_id();
                break;
                case M2MBase::Resource: {
                    resource = (M2MResource*)base;
                    object_name = resource->object_name();
                    object_instance_id = resource->object_instance_id();
                    resource_name = resource->name();
                    printf("Value updated, object name %s, object instance id %d, resource name %s\r\n",
                           resource->object_name().c_str(), resource->object_instance_id(), resource->name().c_str());
                }
                break;
                case M2MBase::ResourceInstance: {
                    res_instance = (M2MResourceInstance*)base;
                    object_name = res_instance->object_name();
                    object_instance_id = res_instance->object_instance_id();
                    resource_name = res_instance->name();
                    resource_instance_id = res_instance->instance_id();
                }
                break;
            default:
                break;
            }
        }
    }
```

## The Write Attributes operation

Any readable Resource can have attributes that are considered during the [**Observe** operation](info_reporting.md#the-observe-feature). The following attributes are used:

- Minimum Period (pmin)
- Maximum Period (pmax)
- Greater Than (gt)
- Less Than (lt)
- Step (st)

mbed Device Server sets the endpoint attribute values that are used to determine when the endpoint sends the Resource value to the server.

Check the [LWM2M Specification](http://technical.openmobilealliance.org/Technical/technical-information/release-program/current-releases/oma-lightweightm2m-v1-0) for details of all the possible **Write Attributes** defined for different types of Objects and Resources.

## The Execute operation

mbed Device Server uses the **Execute** operation to perform an action. This operation can only be performed on individual Resources. 

<span class="notes">**Note:** mbed Client returns an error when the **Execute** operation is received for Object Instances or Resource Instances.</span>

If you want to execute a piece of code in your application, you can pass a `POST` request from mbed Device Server.
When your Resource receives the `POST` request, mbed Client parses the payload from the request and wraps it in the `M2MResource::M2MExecuteParameter` class. The class object is passed through the execute callback function.
You can then typecast the `void*` argument into the corresponding `M2MResource::M2MExecuteParameter` and access the passed payload and its length through the given API. M2MResource::M2MExecuteParameter contains also information which object and object instance resource belongs to. Make sure that the payload sent through POST request is in `plain/text` format and that the content-type is set as “plain/text” on the POST request call.

Check the code snippet below for usage.

```
#include "mbed-client/m2mobject.h"
#include "mbed-client/m2mobjectinstance.h"
#include "mbed-client/m2mresource.h"
    static void execute_function_2(void *argument) {
        if(argument) {
            M2MResource::M2MExecuteParameter* param = (M2MResource::M2MExecuteParameter*)argument;
            int payload_length = param->get_argument_value_length();
            uint8_t* payload = param->get_argument_value();
            String object_name = param->get_argument_object_name();
            uint16_t object_instance_id = param->get_argument_object_instance_id();
            String resource_name = param->get_argument_resource_name();
    }
    
    void M2MLWClient::execute_function(void *argument) {
        if(argument) {
            M2MResource::M2MExecuteParameter* param = (M2MResource::M2MExecuteParameter*)argument;
            int payload_length = param->get_argument_value_length();
            uint8_t* payload = param->get_argument_value();
            String object_name = param->get_argument_object_name();
            uint16_t object_instance_id = param->get_argument_object_instance_id();
            String resource_name = param->get_argument_resource_name();
    }

_object = M2MInterfaceFactory::create_object("Test");
if(_object) {
    M2MObjectInstance* inst = _object->create_object_instance();
    if(inst) {
	M2MResource* res = inst->create_dynamic_resource("D","ResourceTest",true);
        char buffer[20];
        int size = sprintf(buffer,"%d",_value);
        res->set_operation(M2MBase::GET_PUT_POST_ALLOWED);
        res->set_value((const uint8_t*)buffer,
                       (const uint32_t)size);
        res->set_execute_function(execute_callback(this,&M2MLWClient::execute_function));
        /* Overloaded function can be used If callback function is not in class scope. 
        res->set_execute_function(&execute_function_2);
        */
```

The POST method allows the client to respond immediately with the updated payload value as a piggyback response. If you want to send the payload response to the server later (because the Execute operation may take longer time on device to complete), you can handle this through the API.

You can set the resource API `void set_delayed_response(bool)` to `true`. In this case, the POST request response to the server is an empty acknowledgment. When your resource is ready with the response, you can simply call `send_delayed_post_response()` on that resource and the client sends the latest resource value to the server as part of a separate response to the POST request. By default, `void set_delayed_response(bool)` is `false`, which means that the POST response returns the resource value as a piggyback payload immediately. 
