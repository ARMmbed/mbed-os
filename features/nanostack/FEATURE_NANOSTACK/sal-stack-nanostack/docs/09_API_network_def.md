Network Definition APIs
======================

This chapter describes the Network API that includes functionalities, such as network tasks, network authentication, and security. It contains the following sections:

- [_API Headers_](#api-headers)
- [_PANA network authentication API_](#pana-network-authentication-api)
- [_General security type definitions_](#general-security-type-definitions)
- [_Ethernet interface bootstrap definition_](#ethernet-interface-bootstrap-definition)
- [_RF 6LoWPAN interface configure definition_](#rf-6lowpan-interface-configure-definition)

## API Headers

To use Network API, include the following headers:

```
#include net_interface.h
#include net_nvm_api.h
```

## PANA network authentication API

This section introduces the PANA network authentication API for server and client mode.

PANA network authentication is an authentication layer for network access. It supports configurable authentication methods and security parameters. The server controls that only devices that passed the network authentication get access to the network and receive security material for network layer encryption. The server also handles the key updates to nodes in a coordinated way.

Supported TLS 1.2 cipher modes for client and server are listed in _Table 3-9_:

**Table 3-9 Supported TLS 1.2 cipher modes**

Cipher mode|Description
-----------|-----------
`NET_TLS_PSK_CIPHER`|PSK authentication.
`NET_TLS_ECC_CIPHER`|ECC authentication (ECC must be enabled).
`NET_TLS_PSK_AND_ECC_CIPHER`|PSK and ECC authentication (if ECC is not enabled, only PSK is used).

In PSK mode, the client and server must use the same PSK key. The PSK key must be loaded to the stack by `arm_tls_add_psk_key()` function. Unused keys can be removed by `arm_tls_remove_psk_key()` function.

In ECC mode, the Network authentication certificate must be loaded to the stack by `arm_network_certificate_chain_set()`.

### Client API

_Table 3-10_ introduces the client functions used.

**Table 3-10 Client functions**

Function|Description
--------|-----------
`arm_pana_client_library_init()`|Initializes the PANA Protocol for client mode with a given setup.
`arm_pana_client_key_pull()`|Manually pulls the key from the server. Only for testing purposes.

To initialize and configure the PANA network client interface, use the following function:

```
int8_t arm_pana_client_library_init
(
	int8_t interface_id,
	net_tls_cipher_e cipher_mode,
	uint32_t psk_key_id
)
```

Parameter|Description
---------|-----------
`interface_id`|The network interface ID.
`cipher_mode`|The TLS 1.2 cipher mode; PSK, ECC or both.
`psk_key_id`|The PSK key ID for the PSK setup.

<dl>
<dt>Return value:</dt>
<dd>0 Success.</dd>
<dd>-1 Unknown network ID or PANA is not supported in this stack.</dd>
<dd>-2 Interface active.</dd>
</dl>

To pull the PANA client key manually for testing purposes:

```
int8_t arm_pana_client_key_pull
(
	int8_t interface_id
)
```

Parameter|Description
---------|-----------
`interface_id`|The network interface ID.

<dl>
<dt>Return value:</dt>
<dd>0 Success.</dd>
<dd>-1 Unknown network ID.</dd>
</dl>

### Server API

_Table 3-11_ introduces the server functions used.

**Table 3-11 Server functions**

Function|Description
--------|-----------
`arm_pana_server_library_init()`|Initializes the PANA server mode and security material.
`arm_pana_server_key_update()`|Starts a key update process and key delivery to all authenticated nodes.
`arm_pana_activate_new_key()`|Triggers new key material quicker after key delivery phase. Only for testing purposes.
`arm_network_key_get()`|Reads PANA server key material. Only for testing purposes.

To initialize and configure the PANA network server interface:

```
int8_t arm_pana_server_library_init
(
	int8_t interface_id,
	net_tls_cipher_e cipher_mode,
	uint8_t *key_material,
	uint32_t time_period_before_activate_key
)
```

Parameter|Description
---------|-----------
`interface_id`|The network interface ID.
`cipher_mode`|The TLS 1.2 cipher mode; PSK, ECC or both.
`key_material`|A pointer to 128-bit key material or `NULL` when the PANA server generates the random key.
`time_period_before_activate_key`|The guard period in seconds after a successful key delivery phase before the key is activated by the server.

<dl>
<dt>Return value:</dt>
<dd>0 Success.</dd>
<dd>-1 Unknown network ID.</dd>
<dd>-2 Interface active.</dd>
</dl>

To trigger a network key update process, use the following function. This function will deliver a new network key to all routers that have registered a GP address with the server. The function call always triggers a new key ID. The key delivery is started in 300ms intervals between the nodes. This function does not cause any traffic if the server does not have any router device sessions.

```
int8_t arm_pana_server_key_update
(
	int8_t interface_id,
	uint8_t * network_key_material
)
```

Parameter|Description
---------|-----------
`interface_id`|The network interface ID.
`network_key_material`|A pointer to new 128-bit key material or a NULL pointer to a randomly generated key.

<dl>
<dt>Return value:</dt>
<dd>0 Key update process is OK.</dd>
<dd>-1 PANA server is not yet initialized.</dd>
<dd>-2 Old key update is still active.</dd>
<dd>-3 Memory allocation fail.</dd>
</dl>

To activate the new key material quicker before a standard timeout, use the following function. This function is only used for testing purposes.

```
int8_t arm_pana_activate_new_key
(
	int8_t interface_id
)
```

Parameter|Description
---------|-----------
`interface_id`|The network interface ID.

<dl>
<dt>Return value:</dt>
<dd>0 Key activate process is OK.</dd>
<dd>-1 No pending key update.</dd>
<dd>-2 PANA server is not initialized or the PANA server API is disabled by this stack.</dd>
</dl>

To read the security key material of the PANA server:

`previous_active_network_key` is only valid when `current_active_key_index` is bigger than 1.

```
int8_t arm_network_key_get
(
	int8_t interface_id, 	ns_keys_t *key
)
```

Parameter|Description
---------|-----------
`key`|A pointer for storing key material information.

<dl>
<dt>Return value:</dt>
<dd>0 Key read is OK.</dd>
<dd>-1 PANA server key material is not available.</dd>
</dl>

This structure defines the network keys used by the `net_network_key_get` function and comprises the following members:

```
typedef struct ns_keys_t
{
	uint8_t previous_active_network_key[16];
	uint8_t previous_active_key_index;
	uint8_t current_active_network_key[16];
	uint8_t current_active_key_index;
} ns_keys_t;
```

Member|Description
------|-----------
`previous_active_network_key`|Contains the previous key. The key is still valid for reception.
`previous_active_key_index`|Contains the key index for the previous key.
`current_active_network_key`|Contains the current key.
`current_active_key_index`|Contains the index for the current key.

### Resume API

The PANA Resume API allows the previously established PANA sessions to be resumed without going through a full authentication phase. This allows smaller low-power devices to join the network faster after the host or router is booted.

The resume functionality depends on the platform side to provide non-volatile storage space that is used for storing PANA session data. The storage is used for keeping the data when the device is powered off or rebooted. The data contains authentication keys so it must be kept secure. If no such secure storage can be provided, ARM recommends not to use the resume functionality because it may leak the authentication keys.

The resume functionality is divided into two parts, the client side API and the server side API. Both sides require the application to set a callback that receives new material when there are updates in the network. The client side callback receives key material immediately after a successful authentication and periodically after each network key update. On the server side, the callback receives an update when a new client joins the network or when network keys are updated.

Security material is basically divided into two parts, session keys and network keys. Each client session has unique authentication keys that are assigned to only one session. After a successful authentication, the client receives network keys that are shared with the whole network. The network keys are used for encryption and decryption for all clients. Therefore, a session resume requires you to store both keys.

#### Client side PANA resume API

To set a callback for storing PANA key material, use the following function. Before setting this callback, the application should allocate a buffer for transferring keys to the callback.

```
int8_t pana_client_nvm_callback_set
(
	pana_client_session_update_cb *nvm_update,
	pana_client_session_get_cb *nvm_get,
	uint8_t * nvm_static_buffer
);
```

Parameter|Description
---------|-----------
`nvm_update`|A pointer to the callback function for session udate and removal.
`nvm_get`|A pointer to the callback function for read session from NVM.
`nvm_static_buffer`|A pointer to the allocated buffer. The required size is 86 bytes.

<dl>
<dt>Return value:</dt>
<dd>0 Initialization is OK.</dd>
<dd>-1 Failure.</dd>
</dl>

The callback function is called whenever the stack acquires new keys. The parameter passed to the callback tells which parts of the keys are updated. This enumeration defines the parameter types:

```
typedef enum pana_client_nvm_update_process_t
{
       PANA_CLIENT_SESSION_UPDATE,
       PANA_CLIENT_SESSION_REMOVE,
}pana_client_nvm_update_process_t;
```

Parameter|Description
---------|-----------
`PANA_CLIENT_SESSION_UPDATE`|Full update of PANA session information.
`PANA_CLIENT_SESSION_REMOVE`|Current session for the given PAN ID can be removed.

When a callback takes place with the parameter `PANA_CLIENT_SESSION_UPDATE`, the first 16 bytes in the data buffer are PANA session address and the following 70 bytes PANA session keys. The session address is associated with the node's parent address, so that is most probably required only on star topology networks. On the mesh network, the parent address can change, so storing the session address is unnecessary.

When multiple sessions are supported, the session address identifies each session. All sessions are then stored separately.

An example of a session saving functionality when the session address is not stored:

```
#define PANA_SESSION_ADDRESS_SIZE 16
#define PANA_SESSION_KEY_SIZE 70
void pana_resume_callback(uint16_t pan_id, pana_client_nvm_update_process_t event)
{
       if (PANA_CLIENT_SESSION_UPDATE == event) {
             // store now the Session keys, skip the session address
             //Discover by given pan-id
             write_to_storage(nvm_static_buffer+PANA_SESSION_ADDRESS_SIZE, PANA_SESSION_KEY_SIZE);
       } else if (PANA_CLIENT_SESSION_UPDATE == event) {
       		//Discover by given pan-id and remove from NVM
       }
}
```

An example for session get callback:

```
bool pana_nvm_read_callback(uint16_t pan_id)
{
       //Discover by given pan-id and remove from NVM
       if (discover_session_from_nvm_by_pan_id(pan_id)) {
       		
       		//Write session to static memory buffer
       		return true;
      }
      return false;
}
```

#### Server side API

The server side API differs from the client side so that it stores multiple PANA sessions. This adds some complexity to storing of keys.

The first step is to allocate a buffer that is used to transfer keys to the callback function. The buffer size must be 116 bytes and is symbolically defined as `PANA_SERVER_CLIENT_NVM_SESSION_BUF_SIZE`.

The next step is to set a PANA key update callback that receives the key material:

```
int8_t pana_server_nvm_callback_set
(
	pana_server_update_cb *update_cb,
	pana_server_session_get_cb *nvm_get,
	pana_server_session_get_by_id_cb *nvm_session_get,
	uint8_t * nvm_static_buffer
);
```

Parameter|Description
---------|-----------
`update_cb`|A pointer to the callback function.
`nvm_get`|A pointer to the callback function to read the client session by session address.
`nvm_session_get`|A pointer to the callback function to read the client session by PANA session ID.
`nvm_static_buffer`|A pointer to the buffer used for storing the PANA keys.

<dl>
<dt>Return value:</dt>
<dd>0 Success.</dd>
<dd>-1 Error.</dd>
</dl>

The following enumeration defines the parameters to the callback:

```
typedef enum pana_nvm_update_process_t
{
       PANA_SERVER_MATERIAL_UPDATE,
       PANA_SERVER_CLIENT_SESSION_UPDATE,
       PANA_SERVER_CLIENT_SESSION_REMOVE_UPDATE,
}pana_nvm_update_process_t;
```

Parameter|Description
---------|-----------
`PANA_SERVER_MATERIAL_UPDATE`|An update of the PANA server security material.
`PANA_SERVER_CLIENT_SESSION_UPDATE`|An update of the PANA client session.
`PANA_SERVER_CLIENT_SESSION_REMOVE_UPDATE`|Removes the PANA client session.

The buffer that is used to transfer data from the PANA process to the storage can contain server keys or client session data. When the server keys are stored, the buffer size is 90 bytes. On the client sessions, it depends on the event that the buffer contains.

When client session data is stored, the buffer is divided into the following parts:

1. Session addresses, 24 bytes, containing the following parts:
	1. 16-bit offset, 2 bytes.
	2. Client session IPv6 address, 16 bytes.
	3. Client port number, 2 bytes.
	4. Pana session Id, 4-bytes
2. PANA client session data, 33 bytes.
3. PANA client session private keys, 59 bytes.

Not all segments are valid by each call. The parameter that is passed to the callback function defines the parts that are valid. _Table 3-12_ defines the segments.

**Table 3-12 Callback parameters**

|Callback parameter|Buffer content|
|------------------|-------------|
|`PANA_SERVER_MATERIAL_UPDATE`|Buffer contains server key material. The size is defined by the symbol<br> `PANA_SERVER_MATERIAL_BUF_SIZE`.|
|`PANA_SERVER_CLIENT_SESSION_UPDATE`|Add a new PANA session or update the previously stored one. All three segments are valid.<br> An offset number is used to determine which session this is. The offset number for new sessions is zero.<br> The callback should return the offset number for the sessions; this information is not used by the stack but is provided back on the following update.|
|`PANA_SERVER_CLIENT_SESSION_SEQ_UPDATE`|Update only the client session data. The first two segments are stored in the buffer.<br>The callback should use the offset field to determine which session data this is.|
|`PANA_SERVER_CLIENT_SESSION_REMOVE_UPDATE`|Remove the previously stored session. Only the first segment is stored in the buffer.<br>The callback should use the offset field to determine which session data this is.|

The following example shows the basic functionality of the PANA server callbacks:

```
uint16_t app_nvm_pana_update(pana_nvm_update_process_t update_type )
{
       uint16_t ret_val = 0;
       uint16_t offset;

       switch(update_type) {
       case PANA_SERVER_MATERIAL_UPDATE:
             store_server_key(nvm_static_buffer, PANA_SERVER_MATERIAL_BUF_SIZE);
             return 0;
       case PANA_SERVER_CLIENT_SESSION_UPDATE:
             // Store a new session, or update full session record.
             offset = *((uint16_t*)nvm_static_buffer);
             if (0 == offset)
                    return store_new_session(nvm_static_buffer);
             else
                    return update_session(offset, nvm_static_buffer);
       case PANA_SERVER_CLIENT_SESSION_SEQ_UPDATE:
             // Update session data, but does not touch keys
             offset = *((uint16_t*)nvm_static_buffer);
             return update_session_data(offset, nvm_static_buffer);
       case PANA_SERVER_CLIENT_SESSION_REMOVE_UPDATE:
              // Remove previously stored session
             offset = *((uint16_t*)nvm_static_buffer);
             return remove_session(offset);
       }
}

bool app_nvm_pana_read_session_by_session_id(uint32_t session_id)
{
       //Discover session by address. Session id  offset is 20 bytes from storaged data
       if (session_read_by_id(session_id) ) {
           //Write session data behind nvm_static_buffer
          return true;
       }
       return false;
}

bool app_nvm_pana_read_session_by_address(uint8_t *linklocal_address)
{
       //Discover session by address session. Address offset is 2 bytes from storaged data
       if (session_read_by_address(linklocal_address) ) {
           //Write session data behind nvm_static_buffer
          return true;
       }
       return false;
}
```
<span class="notes">**Note:** The previous example assumes that the user provides the functions `store_server_key()`, `store_new_session()`, `update_session()`, `update_session_data()` and `remove_session()`.</span>

When the server starts, it uses the following API to restore the previous server keys:

```
int8_t pana_server_restore_from_nvm
(
	uint8_t * nvm_data,
	int8_t interface_id
);
```

Parameter|Description
---------|-----------
`nvm_data`|Full buffer content given to the callback previously.
`interface_id`|The 6LoWPAN interface ID.

<dl>
<dt>Return value:</dt>
<dd>0 Success.</dd>
<dd>-1 Failure.</dd>
</dl>

After restoring the server data, each session previously stored must be restored:

```
int8_t pana_server_nvm_client_session_load
(
	uint8_t *nvm_pointer
);
```

Parameter|Description
---------|-----------
`nvm_pointer`|A pointer to the full PANA session record containing all three defined fields.

<dl>
<dt>Return value:</dt>
<dd>0 Success.</dd>
<dd>-1 Failure.</dd>
</dl>

```
int8_t net_nvm_data_clean
(
	int8_t interface_id
);
```
Parameter|Description
---------|-----------
`interface_id`|The 6LoWPAN interface ID.

<dl>
<dt>Return value:</dt>
<dd>0 Success.</dd>
<dd>-1 Failure.</dd>
</dl>


```
int8_t net_nvm_wpan_params_storage_enable
(
	int8_t interface_id, 
	wpan_params_updated *nvm_update_cb, 
	wpan_params_get *nvm_get_cb
);
```
Parameter|Description
---------|-----------
`interface_id`|The 6LoWPAN interface ID.

<dl>
<dt>Return value:</dt>
<dd>0 Success.</dd>
<dd>-1 Failure.</dd>
<dd>-2 OOM Failure.</dd>
</dl>

```
int8_t net_nvm_wpan_params_storage_reset
(
	int8_t interface_id
);
```
Parameter|Description
---------|-----------
`interface_id`|The 6LoWPAN interface ID.

<dl>
<dt>Return value:</dt>
<dd>0 Success.</dd>
<dd>-1 Failure.</dd>
</dl>

```
int8_t net_nvm_wpan_params_storage_disable
(
	int8_t interface_id
);
```
Parameter|Description
---------|-----------
`interface_id`|The 6LoWPAN interface ID.

<dl>
<dt>Return value:</dt>
<dd>0 Success.</dd>
<dd>-1 Failure.</dd>
</dl>


