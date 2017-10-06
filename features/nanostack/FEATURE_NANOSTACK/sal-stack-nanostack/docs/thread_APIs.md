# Thread APIs

## API Headers

To manage Thread, include the following headers into your application:

```
#include thread_commissioning_api.h
#include thread_management_api.h
```

The subsequent sections describe API functionality for Thread. To understand the ideas behind Thread, read [Introduction to mbed OS Thread](thread_intro.md).

## Thread Commissioning API

Thread Commissioning API is used for enabling the Thread commissioning process.

### Registering a commissioner candidate

If the Border Router or a Commissioner Router interface is up in the network, a Thread commissioner candidate can begin to register with the network. 

To start the registration process:

```
int commissioning_register(uint8_t interface_id);
```
Parameter|Description
---------|-----------
`interface_id`|The interface ID of the commissioner candidate.

<dl>
<dt>Response</dt>
<dd>-1, if a commissioner exists already.</dd>
<dd>-2, if failed to create a commissioner, interface does not exist.</dd>
<dd>0, success.</dd>
</dl>

### Unregistering a commissioner candidate

Unregisters the commissioner candidate from the network.

To start the unregistration:

```
int commissioning_unregister(uint8_t interface_id);
```

Parameter|Description
---------|-----------
`interface_id`|The interface ID of the commissioner.

<dl>
<dt>Response</dt>
<dd>any value other than 0, failure.</dd>
<dd>0, success.</dd>
</dl>

### Petitioning process

After the registration to the network, the commissioner candfidate initiates petitioning to validate itself as the sole commissioner of the network via a Commissioner Router or Border Router (Commissioner Representative) to the Thread Leader Router. 

To initiate the petitioning process:

```
int commissioning_petition_start(int8_t interface_id, char *commissioner_id_ptr, commissioning_status_cb *status_cb_ptr);
```

Parameter|Description
-----------|-----------
`interface_id`|ID of the interface.
`commissioner_id_ptr`|A pointer to the commissioner ID.
`status_cb_ptr`|A pointer to the callback function to receive the state of the commissioning process.

<dl>
<dt>Response</dt>
<dd>any value other than 0, failure.</dd>
<dd>0, success.</dd>
</dl>

### Commissioning state enumeration

Defines current state of the petition made by the commissioner. The following enumeration type definition is used:

```
typedef enum {
	COMMISSIONING_STATE_ACCEPT,
	COMMISSIONING_STATE_PENDING,
	COMMISSIONING_STATE_REJECT,
	COMMISSIONING_STATE_NO_NETWORK
} commissioning_state_e;

```

State|Description
-----------|-----------
`COMMISSIONING_STATE_ACCEPT`|Accepted state, everything went fine.
`COMMISSIONING_STATE_PENDING`|Commissioning request is still pending.
`COMMISSIONING_STATE_REJECT`|Request rejected by the Leader Router.
`COMMISSIONING_STATE_NO_NETWORK`|No Thread network found.

### Petition response callback

This is the callback function that handles the commissioning status response after a petition request is initiated. A pointer to this callback is given as a parameter to the petition initiation function.

```
typedef int (commissioning_status_cb)(int8_t interface_id, commissioning_state_e state);
```

### Petition keep alive messages

The commissioner keeps secure commissioning session via a Commissioner Router or Border Router with the Leader Router using keep alive messages that can be secured and authenticated using DTLS. 

To set the keep alive messages going:

```
int commissioning_petition_keep_alive(int8_t interface_id, commissioning_state_e state);
```

### Adding a joiner device

A device that wants to join the Thread network is typically called a joiner device. Such a device is added to the Thread network through the commissioning process. 

To add a joiner device to the Thread network:

```
int commissioning_device_add(int8_t interface_id, bool short_eui64,
		uint8_t EUI64[8], uint8_t *PSKd_ptr, uint8_t PSKd_len,
		commissioning_joiner_finalisation_cb *joining_device_cb_ptr);
```

Parameter|Description
-----------|-----------
`interface_id`|ID of the Commissioner interface.
`short_eui64`|A boolean determining if we will use a short EUI64 address for Bloom filter generation or not.
`EUI64[8]`|A pointer to the buffer where the EUI64 bit address is stored.
`PSKd_ptr`|A pointer to the pre-shared key of the device.
`PSKd_len`|Length of the pre-shared key. Minimum of 6 uppercase alphanumeric characters long.
`joining_device_cb_ptr`|A pointer to the callback function, to receive the result of the joining process.

<dl>
<dt>Response</dt>
<dd> 0, success.</dd>
<dd>failure otherwise.</dd>
</dl>

### Removing a joiner device

To remove a joiner device from the Thread network:

```
int commissioning_device_delete(int8_t interface_id, uint8_t EUI64[8]);
```

Parameter|Description
-----------|-----------
`interface_id`|ID of the Commissioner interface.
`EUI64[8]`|A pointer to the buffer where the EUI64 bit address is stored.

<dl>
<dt>Response</dt>
<dd> 0, success.</dd>
<dd>failure otherwise.</dd>
</dl>

### Joining finalization callback

This is the callback function that provides the handler for the joining process responses.

```
typedef int (commissioning_joiner_finalisation_cb)(int8_t interface_id,
		uint8_t EUI64[8], uint8_t *message_ptr, uint16_t message_len);
```

Parameter|Description
-----------|-----------
`interface_id`|ID of the Commissioner interface.
`EUI64[8]`|A pointer to the buffer where the EUI64 bit address is stored.
`message_ptr`|A pointer to a MeshCoP TLV message buffer. For parsing this message, you can use `thread_meshcop_lib.h` header.
`message_len`|Length of the message.

<dl>
<dt>Response</dt>
<dd> 0, success.</dd>
<dd>failure otherwise.</dd>
</dl>

## Thread Management API

This API enables the user to manage a Thread network configuration. This interface essentially provides the capability to read or write various parameters related to Thread network.

Parameter Name | Description
-----------|-----------
`Network Name`| Read/Write.
`Security Policy`| Read/Write.
`Steering Data`| Read/Write.
`Commissioning Data Time-Stamp`| Read/Write.
`Commissioning Credential, PSKc`| Write only.
`Network Master Key`| Read Only when policy allows.
`Network Key Sequence`| Read Only when policy allows.
`Network Mesh Local ULA`| Read Only.
`Border Router Locator`| Read Only.
`Commissioner Session ID`| Read Only.
`XPANID`| Read Only.
`PANID`| Read Only.
`Channel`|Read Only.

### Registering a Thread Management Interface

To instantiate a Thread Management Interface which can in turn start the Thread Management Session:

```
int thread_management_register(int8_t interface_id);
```

Parameter|Description
-----------|-----------
`interface_id`|ID of the management interface.


<dl>
<dt>Response</dt>
<dd> An integer other than 0 or -1, the <code>instance_id</code>, a handle of the management interface.</dd>
<dd>0, if the stack failed to allocate memory for the management session.</dd>
<dd>-1, if the stack failed to free up memory for the management session.</dd>
</dl>

### Unregister a Thread Management Interface

To remove a Thread Management Interface and therefore remove any management session bound to that interface:

```
int thread_management_register(int8_t interface_id);
```

Parameter|Description
-----------|-----------
`interface_id`|ID of the management interface.

<dl>
<dt>Response</dt>
<dd> 0, success.</dd>
<dd>-1, failure.</dd>
</dl>

### Setting up the Thread network name

To set the name for Thread network:

```
int thread_management_set_network_name(int8_t instance_id, uint8_t *name_ptr, uint8_t name_len, management_set_response_cb *cb_ptr);
```

Parameter|Description
-----------|-----------
`instance_id`|Instance ID of the management session.
`name_ptr`|Pointer to the new name.
`name_len`|Length of the name.
`cb_ptr`|Pointer to the management response callback function.

<dl>
<dt>Response</dt>
<dd> 0, success.</dd>
<dd><0, failure.</dd>
</dl>

### Thread management response callback

To receive the response to the Thread management commands:

```
typedef int (management_set_response_cb)(int8_t interface_id, management_state_e status);
```

Parameter|Description
-----------|-----------
`interface_id`|Interface ID of the management session.
`status`|Result status of the request.

### Thread management states

An enumeration type definition is used to set and read the state of the Thread management session requests.

```
typedef enum {
    MANAGEMENT_STATE_REJECT,
    MANAGEMENT_STATE_PENDING,
    MANAGEMENT_STATE_ACCEPT
} management_state_e;
```

Enumeration|Description
-----------|-----------
`MANAGEMENT_STATE_REJECT`|Thread management suit has rejected the request.
`MANAGEMENT_STATE_PENDING`|Request is still pending.
`MANAGEMENT_STATE_ACCEPT`|Thread management suit has accepted the request.

### Setting up security policy

The function `thread_management_set_security_policy()` is used to regulate the current security policy in a Thread network. Using this function, you can enable and disable external and native commissioning and set the network security key rotation time.

```
int thread_management_set_security_policy(int8_t instance_id, uint8_t options, uint16_t rotation_time, management_set_response_cb *cb_ptr);
```

Parameter|Description
-----------|-----------
`instance_id`|Instance ID of the management session.
`options`|If the bit number `8` is set to 1, external commissioning is restricted. If bit number `7` is set to 1, native commissioning is restricted.
`rotation_time`|Thread key rotation time in hours.
`cb_ptr`|A pointer to `management_set_response_cb` callback function.

<dl>
<dt>Response</dt>
<dd> 0, success.</dd>
<dd><0, failure.</dd>
</dl>

### Data Steering

The function `thread_management_set_steering_data()` steers the way how Thread network allows joiners to join the network. Using this function, you can enable and disable the joining process or filter out the joiners. A Bloom filter is used for the filtering. The length of Bloom filter can be set from 1-16 bytes. The first bit of the first byte (set to `0`)indicates whether to use EUI64 addresses or bottom 24 bits of the EUI64 address (set to `1`).

```
int thread_management_set_steering_data(int8_t instance_id, uint8_t *steering_data_ptr, uint8_t steering_data_len, management_set_response_cb *cb_ptr);
```

Parameter|Description
-----------|-----------
`instance_id`|Instance ID of the management session.
`steering_data_ptr`|A pointer to the steering data. If all bits are set to `0`, no joiners will be allowed to join. If all bits are set to 1, any joiner is allowed to join. A mix of 1s and 0s can be defined if you are using a Bloom filter. This filter allows any specific devices to join and prohibits others.
`steering_data_len`|Length of the steering data.
`cb_ptr`|A pointer to `management_set_response_cb` callback function. It can be set to NULL, if no response is required.

<dl>
<dt>Response</dt>
<dd> 0, success.</dd>
<dd><0, failure.</dd>
</dl>

### Thread commissioning time-stamps

To add time-stamps to the Thread commissioning related chunks of data:

```
int thread_management_set_commissioning_data_timestamp(int8_t instance_id, uint64_t time, management_set_response_cb *cb_ptr);
```

Parameter|Description
-----------|-----------
`instance_id`|Instance ID of the management session.
`time`|64 bits in total. Upper 48 bits are is time in seconds. Lower 16 bits are fractional portion of time.
`rotation_time`|Thread key rotation time in hours.
`cb_ptr`|A pointer to `management_set_response_cb` callback function.

<dl>
<dt>Response</dt>
<dd> 0, success.</dd>
<dd><0, failure.</dd>
</dl>

### Setting up commissioning credentials

To set up the security credentials for the Thread network:

```
int thread_management_set_commissioning_credentials(int8_t instance_id, uint8_t PSKc[32], uint8_t PSKc_len, management_set_response_cb *cb_ptr);
```

Parameter|Description
-----------|-----------
`instance_id`|Instance ID of the management session.
`PSKc`|Maximum 32 bytes long pre-shared key used as a network security credential.
`PSKc_len`|Length of the pre-shared key used.
`cb_ptr`|A pointer to `management_set_response_cb` callback function.

<dl>
<dt>Response</dt>
<dd> 0, success.</dd>
<dd><0, failure.</dd>
</dl>

### Collecting Thread management information

#### Fetching Thread Management Information

The function `thread_management_get()` is used to collect Thread management related information from the any device in the Thread network.

Parameter|Description
-----------|-----------
`instance_id`|Instance ID of the management session.
`dst_addr` |Destination address; the address of a remote device from whome it is desirable to fetch management information. If however, the address is not provided, a request is sent to the leader of the network for this purpose. If a native commissioner is used, the request for management information is sent to the border router.
`uri_ptr` |The ASCII string for the URI. This string identifies the CoAP URI for the desired resource. For example, `/c/mg` identifies the management get information resource.
`fields_ptr`|A pointer to management fields; a set of TLVs. A list of such TLVs can be found in `thread_meshcop_lib.h`.
`field_count`|Number of fields in the field pointer array (set of TLVs).
`cb_ptr`|A pointer to `management_get_response_cb` callback function carrying the result of the operation.

<dl>
<dt>Response</dt>
<dd> 0, success.</dd>
<dd><0, failure.</dd>
</dl>

#### Setting up Thread Management Information

The function `thread_management_set()` is used to set up Thread management related information to any device in the Thread network.

Parameter|Description
-----------|-----------
`instance_id`|Instance ID of the management session.
`dst_addr` |Destination address; the address of a remote device where it is desired to set up management information. If however, the address is not provided, a request is sent to the leader of the network for this purpose. If a native commissioner is used, the request for setting up management information is sent to the border router.
`uri_ptr` |The ASCII string for the URI. This string identifies the CoAP URI for the desired resource. For example, `/c/ms` identifies the management set information resource.
`data_ptr`|A pointer to the desired set of TLVs. 
`field_count`|Number of fields in the field pointer array (set of TLVs).
`cb_ptr`|A pointer to `management_get_response_cb` callback function carrying the result of the operation.

<dl>
<dt>Response</dt>
<dd> 0, success.</dd>
<dd><0, failure.</dd>
</dl>

### Thread management get response callback

The callback function `(management_get_response_cb)` is used for receiving the information regarding Thread management from various fields related to the Thread management. Such fields can be parsed using macros defined in `thread_meshcop_lib.h`.

```
typedef int (management_get_response_cb)(int8_t instance_id, management_state_e status, uint8_t *response_message_ptr, uint16_t response_message_len);
```

Parameter|Description
-----------|-----------
`instance_id`|Instance ID of the management session.
`status`|The state of the management interface given by `management_state_e`.
`response_message_ptr`|A pointer to a MeshCoP TLV structure that contains the requested TLVs.
`response_message_len`|Length of the response message.

