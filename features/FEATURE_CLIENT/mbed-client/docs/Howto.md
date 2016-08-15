# How to use the mbed Client API

This section explains how to use our C++ API to create and configure different types of Objects, Object Instances and Resources for mbed Client to comply with the OMA LWM2M specifications. These resources can then be communicated to mbed Device Server.

<span class="notes">**Note:** The API complies with the OMA LWM2M specifications.</span>

As per the OMA LWM2M specification:

- The client must have defined Objects, under which it can create Object Instances.
- The client must have created Object Instances under Objects, which will eventually contain Resources.
- The client must have Resources under Object Instances which belong to Objects.
- The client must have created Resources under Object Instance. You can create M2MResource from the M2MObjectInstance class.
- The client must have created Resource Instances under Resources. You can create M2MResourceInstance from the M2MObjectInstance class.


## How to create and configure Objects

With this API, you can create and configure the following Objects:

- [Device Objects](#device-object)
- [Security Objects](#security-object)
- [Custom Objects](#custom-object)

The M2MObject class is derived from the M2MBase class, so all the public methods can be used from the M2MObject and its derived classes.

### Creating OMA-defined Objects

#### Device Object

To create a Device Object:

`static M2MDevice *create_device();`

Because there can be only one instance of M2MDevice, it is a static class and you can delete it as follows:

`M2MDevice::delete_instance();`

Check the [M2MDevice class documentation](https://docs.mbed.com/docs/mbed-client-guide/en/latest/api/classM2MDevice.html) to see how to configure the Device Object. 

#### Security Object

To create a Security Object:

`static M2MSecurity *create_security(M2MSecurity::ServerType server_type);`

You can create a Bootstrap or normal mbed Device Server by passing the appropriate `enum` value.

Check the [M2MSecurity class documentation](https://docs.mbed.com/docs/mbed-client-guide/en/latest/api/classM2MSecurity.html) to see how to configure the Security Object, as well as how to create appropriate Resources and assign values to them.

#### Custom Object

For a Custom Object, you need to pass the name of the Object that you would like to create (for example `Test`); this will create an Object with that name in mbed Device Server.

To create an M2MObject:

`static M2MObject *create_object(const String &name);`

#### Configuring the Object

Once you have created an Object (whether OMA-specific or custom), you can configure various parameters in it to control or modify  communication with mbed Device Server.

```
M2MObject * _object = M2MInterfaceFactory::create_object("Test");
 if(_object) { 
 _object->set_register_uri(true); // The registration message explicitly sends this object path as registered resource such as /Test.
 }
```

Here, we discuss a few of the most important parameters, which you must configure properly to work with the Objects.

##### Setting Operation Mode

To set the operation mode of the Objects so that they can handle `GET`, `PUT`, `POST`, `DELETE` or a combination of these requests coming from mbed Device Server:

```
virtual void set_operation(M2MBase::Operation operation);
_object->set_operation(M2MBase::GET_PUT_POST_ALLOWED); // This defines the REST operations that can be performed on this object.
```

##### Setting Observable Mode

To set the Object to be an observing resource:

`virtual void set_observable(bool observable);` 

By default, all the created objects are non-observable. You can set them to be observable or not observable using this API.

```
_object->set_observable(true); // The object can be observed from server.
_object->set_observable(false); // The object cannot be observed from server.
```

##### Setting CoAP content type

Currently, the only available content type is the OMA TLV type. The OMA TLV type works only for Objects with a numeric value. For example, if you are creating a custom Object it must be of a numeric type such as `100`.

`M2MObject * _object = M2MInterfaceFactory::create_object("100");`

By default, all the numeric objects are assigned the CoAP content type of `99` by mbed Client but if you want your Object to assign any other CoAP content type, for example 120, you can do that by setting the object's CoAP content type as shown below:

```
virtual void set_coap_content_type(const uint8_t content_type);
_object->set_coap_content_type(120);
```

<span class="tips">**Tip:** In future releases, we will introduce support for the JSON content types.</span>

## How to create and configure Object Instances

With this API, you can create and configure the following Object Instances:

- [Device Object Instance](#device-object-instance)
- [Security Object Instance](#security-object-instance)
- [Custom Object Instance](#custom-object-instance)

The `M2MObjectInstance` class is derived from the `M2MBase` class, so all the public methods from `M2MObjectInstance` and its derived classes can be used.

### Creating an OMA-defined Object Instance

#### Device Object Instance

Because there can only be one instance for the Device Object, the Object Instance is automatically created when creating an M2MDevice Object.

#### Security Object Instance

Because there can only be one instance for the Security Object, the Object Instance is automatically created when creating an M2MDevice Object based on a selected server type.

#### Custom Object Instances

Object Instances need IDs. Normally, the IDs start from '0' and increment, 
so the Object Instance structure on the mbed Device Server side would look like this:

`Object/Object Instance ID`

When you create an Object Instance, you therefore need to pass the ID that you would like to create. For example, if you pass `0`, you get an object `/Test/0` on mbed Device Server.

To create an M2M Object Instance:

`M2MObject::create_object_instance(uint16_t instance_id);`

You can pass an Object Instance ID to create appropriate Object Instances. Normally, Object Instances will start from `0` and increment. So the Object Instance structure on the mbed Device Server side would look like this:

`Object/Object Instance ID`

```
M2MObjectInstance * object_instance = _object->create_object_instance(0);
 if(object_instance) { 
     object_instance->set_register_uri(true);  // The registration message explicitly sends this object instance path as registered resource such as /Test/0.
 }
```

### Configuring the Object Instance

When you have created an Object Instance (whether OMA-specific or custom), you can configure various parameters in it to control or modify communication with mbed Device Server.

Here, we present a few of the most important parameters that you must configure properly to work with the object instances.

##### Setting Operation Mode

To set the operation mode of the Objects so that they can handle `GET`, `PUT`, `POST`, `DELETE` or a combination of these requests coming from mbed Device Server:

```
virtual void set_operation(M2MBase::Operation operation);
object_instance->set_operation(M2MBase::GET_PUT_POST_ALLOWED); // This defines the REST operations that can be performed on this object instance.
```

##### Setting Observable Mode

To set the Object Instance to be an observing resource:

`virtual void set_observable(bool observable);`

By default, all the created Object Instances are non-observable. You can set them to be observable or not observable using this API.

```
_object_instance->set_observable(true); // This defines that the Object Instance can be observed from server.
_object_instance->set_observable(false); // This defines that the Object Instance cannot be observed from server.
```

##### Setting CoAP content type

Currently, the only available content type is the OMA TLV type. The OMA TLV type works only for Objects and Object Instances with a numeric value. For example, if you are creating a custom Object and Object Instances it must be of a numeric type such as `100`. 

```
M2MObject * _object = M2MInterfaceFactory::create_object("100");
M2MObjectInstance * object_instance = _object->create_object_instance(0);
```

By default, all numeric objects are assigned the CoAP content type of `99` by mbed Client but if you want your Object Instance to assign any other CoAP content type, for example 120, you can do that by setting the object's CoAP content type as follows:

```
virtual void set_coap_content_type(const uint8_t content_type);
object_instance->set_coap_content_type(120);
```

<span class="tips">**Tip:** In future version, we will introduce support for the JSON content types.</span>

There are additional APIs that provide getter and remove functions for Object Instances in the `M2MObjectInstance` class; [check the API documentation](https://docs.mbed.com/docs/mbed-client-guide/en/latest/api/classM2MObjectInstance.html) for their usage. 

## How to create and configure Resources and Resource Instances

With this API, you can create and configure the following Resources and Resource Instances:

- [Device Object Resources](#device-object-resources)
- [Security Object Resources](#security-object-resources)
- [Custom Resources](#custom-resources)


The `M2MResource` class is derived from the `M2MResourceInstance`, which in turn is derived from the `M2MBase` class, so all the public methods can be used from `M2MResource` or `M2MResourceInstance` and their derived classes.

### Creating OMA-defined Resources

#### Device Object Resources

There are direct APIs to create and set values for the Device Resources. You can create the required Resource and set values based on their data types.

- For Resources that take string values:

`M2MResource* create_resource(DeviceResource resource, const String &value);`

- For Resources that take integer values:

`M2MResource* create_resource(DeviceResource resource, uint32_t value);`

- There are a few Resources that can have multiple instances. To create these Resources:

`M2MResourceInstance* create_resource_instance(DeviceResource resource, uint32_t value,uint16_t instance_id);`

Where `instance_id` is the Resource Instance ID, for example `/3/0/11/0`.

Check the [M2MDevice API documentation](https://docs.mbed.com/docs/mbed-client-guide/en/latest/api/classM2MDevice.html) to find which enums are supported for `integer` or `string` value types.

There are other APIs in the `M2MDevice` class that you can use to set, remove and modify new values for the resources.

#### Security Object Resources

_Mandatory Resources_

Most of the mandatory Resources are created automatically when you create an M2MSecurity Object. You can set their values based on their data types. 

Resources that are automatically created and their accepted data types:

- `SecurityMode`
- `ShortServerID`
- `M2MServerUri`
- `BootstrapServer`
- `PublicKey`
- `ServerPublicKey`
- `Secretkey`

For Resources (`SecurityMode`, `ShortServerID`) that take integer values, you can set the values as follows:

`bool set_resource_value(SecurityResource resource,uint32_t value);`

```
security_object->set_resource_value(M2MSecurity::SecurityMode, 1);
security_object->set_resource_value(M2MSecurity::ShortServerID, 1);
```

For Resources (`M2MServerUri`) that take string values, you can set the values as follows:

`bool set_resource_value(SecurityResource resource,const String &value);`
 
```
security_object->set_resource_value(M2MSecurity::M2MServerUri, "coap://api.connector.mbed.com:5684");
```

For Resources (`PublicKey`, `ServerPublicKey`, `Secretkey`) that take binary values, you can set the values as follows:

`bool set_resource_value(SecurityResource resource,onst uint8_t *value,const uint16_t length);`

```
uint8_t key[] = {"key"};
security_object->set_resource_value(M2MSecurity::PublicKey, key, sizeof(key));
```

_Optional Resources_

Optional Resources as defined in the Security Object:

- `SMSSecurityMode`
- `M2MServerSMSNumber`
- `ClientHoldOffTime`

To create and set values for the optional Resources that take an integer value:

`M2MResource* create_resource(SecurityResource resource, uint32_t value);`

`security_object->create_resource(M2MSecurity::M2MServerSMSNumber, 123542323);`


Check the [M2MSecurity API documentation](https://docs.mbed.com/docs/mbed-client-guide/en/latest/api/classM2MSecurity.html) to find which enums are supported for `integer`, `string` or `uint8_t*` value types.

There are more APIs in the `M2MSecurity` class that you can use to set, remove and modify Resource values.

#### Custom Resources

For Custom Objects, you can create Resources of two types:

- **M2MResource**: a resource with a single instance, for example `/Test/0/Resource`.

- **M2MResourceInstance**: a resource with multiple instances, for example `/Test/0/Resource/0`, `/Test/0/Resource/1`.

For each of these types, the Resource and Resource Instances can be either static or dynamic:

- **Static**: Resource and Resource Instances whose value does not change over time, these are not observable.
- **Dynamic**: Resource and Resource Instances whose value can change. These can be made observable.

**Creating dynamic and static single-instance Resources**

- To create a single-instance Resource with a static value (`/Test/0/Resource`): [see parameters here](https://docs.mbed.com/docs/mbed-client-guide/en/latest/api/classM2MObjectInstance.html#aaa596f731688730d7a883b7f1251a662) 

```
M2MObject * object = M2MInterfaceFactory::create_object("Test");
M2MObjectInstance * object_instance = object->create_object_instance(0);

uint8_t value[] ={"value"};
M2MResource* resource = object_instance->create_static_resource("Resource", "sensor",M2MResourceInstance::INTEGER,value,sizeof(value),false);
```

- To create an observable single-instance Resource (`/Test/0/Resource`) with a dynamic value that can be set later on: [see parameters here](https://docs.mbed.com/docs/mbed-client-guide/en/latest/api/classM2MObjectInstance.html#a9b3f88dc2d28512ea6c3db6f74168c3f)  

```
M2MObject * object = M2MInterfaceFactory::create_object("Test");
M2MObjectInstance * object_instance = object->create_object_instance(0);

uint8_t value[] ={"value"};
M2MResource* resource = object_instance->create_dynamic_resource("Resource", "sensor",M2MResourceInstance::INTEGER,value,sizeof(value), true, false);
```

**Creating dynamic and static Resource Instances**

- To create a Resource Instance (`/Test/0/Resource/0`) with a static value: [see parameters here](https://docs.mbed.com/docs/mbed-client-guide/en/latest/api/classM2MObjectInstance.html#a6acac6e65bfbc8b731ab4afcc805c41b)

```
M2MObject * object = M2MInterfaceFactory::create_object("Test");
M2MObjectInstance * object_instance = object->create_object_instance(0);

uint8_t value[] ={"value"};
M2MResourceInstance* resource_instance = object_instance->create_static_resource_instance("Resource", "sensor",M2MResourceInstance::INTEGER,value,sizeof(value),0);
```


- To create an observable Resource Instance (`/Test/0/Resource/0`) with a dynamic value that can be set later on: [see parameters here](https://docs.mbed.com/docs/mbed-client-guide/en/latest/api/classM2MObjectInstance.html#adcaba046a484282983380edf8a370cfa)  

```
M2MObject * object = M2MInterfaceFactory::create_object("Test");
M2MObjectInstance * object_instance = object->create_object_instance(0);

uint8_t value[] ={"value"};
M2MResource* resource = object_instance->create_dynamic_resource_instance("Resource", "sensor",M2MResourceInstance::INTEGER,value,sizeof(value), true, 0);
```

#### Configuring the Resource and Resource Instance

When you have created a Resource or Resource Instance (whether OMA-specific or custom), you can configure various parameters to control or modify communication with mbed Device Server.

Here, we present a few of the most important parameters that you must configure properly to work with the Resource and Resource Instance.

##### Setting Operation Mode

You can set the Resource or Resource Instance operation mode so that they can handle `GET`, `PUT`, `POST`, `DELETE` or a combination of these requests coming from mbed Device Server.

To set the operation mode:

```
virtual void set_operation(M2MBase::Operation operation);
resource->set_operation(M2MBase::GET_PUT_POST_ALLOWED); // This defines the REST operations that can be performed on this Resource.
resource_instance->set_operation(M2MBase::GET_PUT_POST_ALLOWED); // This defines the REST operations that can be performed on this Resource Instance.
```
##### Setting Observable Mode

To set the Resource or Resource Instance to be an observable resource:

`virtual void set_observable(bool observable);`

By default, all created static Resources or Resource Instances are non-observable. While creating a dynamic Resource or Resource Instance you can specify through API whether they are observable or not. You can change that later as well as follows:

```
resource->set_observable(true); // This defines that the Resource or Resource Instance can be observed from server.
resource->set_observable(false); // This defines that the Resource or Resource Instance cannot be observed from server.
```

##### Setting the value of a dynamic Resource or Resource Instance

You can set the value of a dynamic Resource or Resource Instance so that they can be sent to mbed Device Server using `GET` requests.

To set the values:

```
virtual bool set_value(const uint8_t *value, const uint32_t value_length);
uint8_t value[] = {"value"};
resource->set_value(value,sizeof(value));
```

##### Setting an executable function

For dynamic Resources, you can pass a function pointer to the Resource or Resource Instance. It will be executed when mbed Device Server calls a `POST` method on that resource. The Resource or Resource Instance must support the `POST` operation mode for this feature to work.

To pass the function pointer:

```
virtual void set_execute_function(execute_callback callback);
void execute_function_example(void *) {
// Code
};
resource->set_execute_function(execute_callback(this,&execute_function_example));
```
In case execute callback function is defined as a global function and it's outside of your class scope you can use overloaded set_execute_function:
```
virtual void set_execute_function(execute_callback_2 callback);
static void c_style_function(void *) {
// Code
}
resource->set_execute_function(&c_style_function);
```
There are additional APIs that provide getter and remove functions for Resource and Resource Instances in the `M2MResource` and `M2MResourceInstance` classes. Check [the API documentation](https://docs.mbed.com/docs/mbed-client-guide/en/latest/api/annotated.html) for their usage. 
