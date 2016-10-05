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
	void (*passed_fptr)(pana_client_nvm_update_process_t),
	uint8_t * nvm_static_buffer
);
```

Parameter|Description
---------|-----------
`passed_fptr`|A pointer to the callback function.
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
       PANA_CLIENT_SESSION_SEQ_UPDATE,
}pana_client_nvm_update_process_t;
```

Parameter|Description
---------|-----------
`PANA_CLIENT_SESSION_UPDATE`|Full update of PANA session information.
`PANA_CLIENT_SESSION_SEQ_UPDATE`|An update of the REQ and RES sequence number of a PANA key pull or push operation.

When a callback takes place with the parameter `PANA_CLIENT_SESSION_UPDATE`, the first 16 bytes in the data buffer are PANA session address and the following 70 bytes PANA session keys. The session address is associated with the node's parent address, so that is most probably required only on star topology networks. On the mesh network, the parent address can change, so storing the session address is unnecessary.

When multiple sessions are supported, the session address identifies each session. All sessions are then stored separately.

An example of a session saving functionality when the session address is not stored:

```
#define PANA_SESSION_ADDRESS_SIZE 16
#define PANA_SESSION_KEY_SIZE 70
void pana_resume_callback(pana_client_nvm_update_process_t event)
{
       if (PANA_CLIENT_SESSION_UPDATE == event) {
             // store now the Session keys, skip the session address
             write_to_storage(nvm_static_buffer+PANA_SESSION_ADDRESS_SIZE, PANA_SESSION_KEY_SIZE);
       }
}
```

To resume the PANA session:

```
int8_t net_pana_client_session_nvm_data_load
(
	uint8_t *data_buffer,
	uint8_t *session_address,
	int8_t interface_id
);
```

Parameter|Description
---------|-----------
`data_buffer`|A pointer to the PANA session keys.
`session_address`|A pointer to the PANA session address, or NULL if no session address information is stored.
`interface_id`|The interface ID for a 6LoWPAN interface.

When you give a `NULL` pointer to `session_address`, the function only restores the PANA keys and can then associate with any parent from the same network. This functionality is required on the mesh topology.

A client can keep track of a single PANA session or multiple PANA sessions using the enumeration below. Single session mode is set by default.

```
typedef enum {
    NET_PANA_SINGLE_SESSION,        
    NET_PANA_MULTI_SESSION,        
} net_pana_session_mode_e;
```

#### Server side API

The server side API differs from the client side so that it stores multiple PANA sessions. This adds some complexity to storing of keys.

The first step is to allocate a buffer that is used to transfer keys to the callback function. The buffer size must be 116 bytes and is symbolically defined as `PANA_SERVER_CLIENT_NVM_SESSION_BUF_SIZE`.

The next step is to set a PANA key update callback that receives the key material:

```
int8_t pana_server_nvm_callback_set
(
	uint16_t (*passed_fptr)(pana_nvm_update_process_t),
	uint8_t * nvm_static_buffer
);
```

Parameter|Description
---------|-----------
`passed_fptr`|A pointer to the callback function.
`nvm_static_buffer`|A pointer to the buffer used to store the PANA keys.

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
       PANA_SERVER_CLIENT_SESSION_SEQ_UPDATE,
       PANA_SERVER_CLIENT_SESSION_REMOVE_UPDATE,
}pana_nvm_update_process_t;
```

Parameter|Description
---------|-----------
`PANA_SERVER_MATERIAL_UPDATE`|An update of the PANA server security material.
`PANA_SERVER_CLIENT_SESSION_UPDATE`|An update of the PANA client session.
`PANA_SERVER_CLIENT_SESSION_SEQ_UPDATE`|A sequence number update of the PANA client session.
`PANA_SERVER_CLIENT_SESSION_REMOVE_UPDATE`|Removes the PANA client session.

The buffer that is used to transfer data from the PANA process to the storage can contain server keys or client session data. When the server keys are stored, the buffer size is 90 bytes. On the client sessions, it depends on the event that the buffer contains.

When client session data is stored, the buffer is divided into the following parts:

1. Session addresses, 20 bytes, containing the following parts:
	1. 16 bytes offset, 2 bytes.
	2. Client session IPv6 address, 16 bytes.
	3. Client port number, 2 bytes.
2. PANA client session data, 33 bytes.
3. PANA client session private keys, 63 bytes.

Not all segments are valid by each call. The parameter that is passed to the callback function defines the parts that are valid. _Table 3-12_ defines the segments.

**Table 3-12 Callback parameters**

|Callback parameter|Buffer content|
|------------------|-------------|
|`PANA_SERVER_MATERIAL_UPDATE`|Buffer contains server key material. The size is defined by the symbol<br> `PANA_SERVER_MATERIAL_BUF_SIZE`.|
|`PANA_SERVER_CLIENT_SESSION_UPDATE`|Add a new PANA session or update the previously stored one. All three segments are valid.<br> An offset number is used to determine which session this is. The offset number for new sessions is zero.<br> The callback should return the offset number for the sessions; this information is not used by the stack but is provided back on the following update.|
|`PANA_SERVER_CLIENT_SESSION_SEQ_UPDATE`|Update only the client session data. The first two segments are stored in the buffer.<br>The callback should use the offset field to determine which session data this is.|
|`PANA_SERVER_CLIENT_SESSION_REMOVE_UPDATE`|Remove the previously stored session. Only the first segment is stored in the buffer.<br>The callback should use the offset field to determine which session data this is.|

The following example shows the basic functionality of the PANA server callback:

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

## General security type definitions

This section introduces general security type definitions.

### Certificate structure

The certificate structure comprises the following members:

```
typedef struct arm_certificate_chain_entry_s
{
	uint8_t chain_length;
	const uint8_t *cert_chain[4];
	uint16_t cert_len[4];
	const uint8_t *key_chain[4];
} arm_certificate_chain_entry_t;
```

Member|Description
------|-----------
`chain_length`|Defines the length of the certificate chain.
`cert_chain`|Defines the pointers to the certificates in chain.
`cert_len`|Defines the certificate lengths.
`key_chain`|Defines the private keys.

### TLS cipher mode structure

This enumeration defines the TLS cipher modes:

```
typedef enum net_tls_cipher_e
{
	NET_TLS_PSK_CIPHER,
	NET_TLS_ECC_CIPHER,
	NET_TLS_PSK_AND_ECC_CIPHER,
} net_tls_cipher_e;
```

Parameter|Description
---------|-----------
`NET_TLS_PSK_CIPHER`|Means that network authentication only supports PSK.
`NET_TLS_ECC_CIPHER`|Means that network authentication only supports ECC.
`NET_TLS_PSK_AND_ECC_CIPHER`|Means that network authentication supports both PSK and ECC.

### TLS PSK info structure

The TLS PSK info structure comprises the following members:

```
typedef struct net_tls_psk_info_s
{
	uint32_t key_id;
	uint8_t key[16];
} net_tls_psk_info_s;
```

Member|Description
------|-----------
`key_id`|Means that a PSK key ID can be `0x01-0xFFFF`. The storage size is intentionally 32 bits.
`key`|Defines a 128-bit PSK key.

The 6LoWPAN stack supports two different chain certificate users:

- Transport Layer Security (TLS).
- Network authentication (PANA, EAP or TLS).

## Ethernet interface bootstrap definition

This section defines the Ethernet interface bootstrap.

### IPv6

This enumeration defines the IPv6 bootstrap:

```
typedef enum net_ipv6_mode_e
{
	NET_IPV6_BOOTSTRAP_STATIC,
	NET_IPV6_BOOTSTRAP_AUTONOMOUS

} net_ipv6_mode_e;
```

Parameter|Description
---------|-----------
`NET_IPV6_BOOTSTRAP_STATIC`|Means that the application defines the IPv6 prefix.
`NET_IPV6_BOOTSTRAP_AUTONOMOUS`|Means that the network defines the IPv6 prefix.

## RF 6LoWPAN interface configure definition

This section defines the RF 6LoWPAN interface configuration.

### typedef enum net_6lowpan_mode_e

This enumeration defines the different 6LoWPAN bootstrap or device modes:

```
typedef enum net_6lowpan_mode_e
{
	NET_6LOWPAN_BORDER_ROUTER,
	NET_6LOWPAN_ROUTER,
	NET_6LOWPAN_HOST,
	NET_6LOWPAN_SLEEPY_HOST
    NET_6LOWPAN_NETWORK_DRIVER, 
    NET_6LOWPAN_SNIFFER 

} net_6lowpan_mode_e;
```

Parameter|Description
---------|-----------
`NET_6LOWPAN_BORDER_ROUTER`|Is a root device for 6LoWPAN ND.
`NET_6LOWPAN_ROUTER`|Is a router device.
`NET_6LOWPAN_HOST`|Is a host device. This is the default setting.
`NET_6LOWPAN_SLEEPY_HOST`|Is a sleepy host device.
`NET_6LOWPAN_NETWORK_DRIVER`|6LoWPAN radio host device only, no bootstrap.
`NET_6LOWPAN_SNIFFER`|Radio sniffer only, no bootstrap.

The `NET_6LOWPAN_SLEEPY_HOST` mode support requires MLE protocol support.

### typedef enum net_6lowpan_mode_extension_e

This enumeration defines the different 6LoWPAN bootstrap extension modes:

```
typedef enum {
    NET_6LOWPAN_ND_WITHOUT_MLE,        
    NET_6LOWPAN_ND_WITH_MLE,           
    NET_6LOWPAN_THREAD,                
    NET_6LOWPAN_ZIGBEE_IP              
} net_6lowpan_mode_extension_e;
```

Parameter|Description
---------|-----------
`NET_6LOWPAN_ND_WITHOUT_MLE`|Starts the 6LoWPAN-ND bootstrap without MLE (Mesh link establishment protocol).
`NET_6LOWPAN_ND_WITH_MLE`|Starts 6LoWPAN-ND with MLE support.
`NET_6LOWPAN_THREAD`|Starts Thread with MLE.
`NET_6LOWPAN_ZIGBEE_IP`|Starts 6LoWPAN ZigBee-IP.

### typedef struct network_driver_setup_s

This structure defines the different 6LoWPAN radio interface setups:

```
typedef struct {
    uint16_t mac_panid;               
    uint16_t mac_short_adr;            
    uint8_t beacon_protocol_id;         
    uint8_t network_id[16];           
    uint8_t beacon_payload_tlv_length; 
    uint8_t *beacon_payload_tlv_ptr;  /**< Optional Steering parameters */
} network_driver_setup_s;
```

Member|Description
------|-----------
`mac_panid`|Link layer PAN ID. Accepted values are `<0xFFFE`.
`mac_short_adr`|Defines the IEEE 802.15.4 Short MAC address. If the value is `<0xFFFE`, it indicates that GP16 addresses are active.
`beacon_protocol_id`|Beacon protocol ID. ZigBee reserves the value 2. 6LoWPAN does not define any value for this, you can use any non-reserved value.
`network_id`|A 16-byte long network ID. Used in the beacon payload.
`beacon_payload_tlv_length`|The length of optional steering parameters.
`beacon_payload_tlv_ptr`|A pointer to the optional steering parameters.

### typedef enum net_6lowpan_gp_address_mode_e

This enumeration defines the different addressing modes for a network interface. This setting is specific to each interface and is only applicable to a 6LoWPAN interface.

```
typedef enum net_6lowpan_gp_address_mode_e
{
	NET_6LOWPAN_GP64_ADDRESS,
	NET_6LOWPAN_GP16_ADDRESS,
	NET_6LOWPAN_MULTI_GP_ADDRESS,
} net_6lowpan_gp_address_mode_e;
```

Parameter|Description
---------|-----------
`NET_6LOWPAN_GP64_ADDRESS`|Means that the interface will only register a GP64 address. This is the default setting.
`NET_6LOWPAN_GP16_ADDRESS`|Means that the interface will only register a GP16 address.
`NET_6LOWPAN_MULTI_GP_ADDRESS`|Means that the interface will register both GP16 and GP64 addresses.

The default address mode is `NET_6LOWPAN_GP64_ADDRESS`.

### typedef enum net_6lowpan_link_layer_sec_mode_e

This enumeration defines the security mode for an interface. This setting is specific to each interface and is only applicable to 6LoWPAN interfaces.

```
typedef enum net_6lowpan_link_layer_sec_mode_e
{
	NET_SEC_MODE_NO_LINK_SECURITY,
	NET_SEC_MODE_PSK_LINK_SECURITY,
	NET_SEC_MODE_PANA_LINK_SECURITY,
} net_6lowpan_link_layer_sec_mode_e;
```

Parameter|Description
---------|-----------
`NET_SEC_MODE_NO_LINK_SECURITY`|Means that the security is disabled at a link layer. This is the default setting.
`NET_SEC_MODE_PSK_LINK_SECURITY`|Means that a PSK key defines the link security.
`NET_SEC_MODE_PANA_LINK_SECURITY`|Means that PANA network authentication defines the link key. The client must call `arm_pana_client_library_init()` and the border router application `arm_pana_server_library_init()` to initialize the PANA protocol. The PANA Network API could be disabled by some of the stack packets and it is also possible that it only supports client mode.

The default setting is `NET_SEC_MODE_NO_SECURITY`.

### typedef struct net_link_layer_psk_security_info_s

This structure defines the PSK security information and comprises the following members:

```
typedef struct net_link_layer_psk_security_info_s
{
	uint8_t key_id;
	uint8_t security_key[16];
} net_link_layer_psk_security_info_s;
```

Member|Description
------|-----------
`key_id`|PSK key ID of a link layer. Can be `0x01-0xFF`.
`security_key`|Defines the 128-bit PSK key of a link layer.

### typedef struct border_router_setup_s

This structure defines the information required to set up a 6LoWPAN border router and comprises the following members:

```
typedef struct border_router_setup_s
{
	uint8_t channel;
	uint16_t mac_panid;
	uint16_t mac_short_adr;
	uint8_t beacon_protocol_id;
	uint8_t network_id[16];
	uint8_t lowpan_nd_prefix[8];
	uint16_t ra_life_time;
	uint32_t abro_version_num;
} border_router_setup_t;
```

Member|Description
------|-----------
`channel`|Defines the channel used in 802.15.4 radio. Supported values are from 1 to 26.
`mac_panid`|Link layer PAN ID. Accepted values are `<0xFFFE`.
`mac_short_adr`|Defines IEEE 802.15.4 Short MAC address. If value is `<0xFFFE`, it indicates that GP16 addresses are active.
`beacon_protocol_id`|Beacon protocol ID. ZigBee reserves the values from `0` to `2`. 6LoWPAN does not define any value for this, you can use any non-reserved value.
`network_id`|A 16-byte long network ID. Used in the beacon payload.
`lowpan_nd_prefix`|Defines the ND default prefix, ABRO, DODAG ID, and GP address.
`ra_life_time`|Defines the ND router lifetime in seconds. ARM recommends value 180+.
`abro_version_num`|Defines the ND ABRO version number (0 when starting a new ND setup).

## General network type definitions

This section defines general network layer types defined by the Network API. 

### typedef enum net_security_t

This enumeration defines the information required to set up an id that represents a network interface and comprises the following parameters:

```
typedef enum net_security_t {
    NW_NO_SECURITY = 0,                       
    NW_SECURITY_LEVEL_MIC32 = 1,               
    NW_SECURITY_LEVEL_MIC64 = 2,             
    NW_SECURITY_LEVEL_MIC128 = 3,             
    NW_SECURITY_LEVEL_ENC = 4,             
    NW_SECURITY_LEVEL_ENC_MIC32 = 5,         
    NW_SECURITY_LEVEL_ENC_MIC64 = 6,          
    NW_SECURITY_LEVEL_ENC_MIC128 = 7 
} net_security_t;

```

Parameter|Description
---------|-----------
`NW_NO_SECURITY`|Disables network level security.
`NW_SECURITY_LEVEL_MIC32`|Enables 32-bit Message Integrity Code (MIC) verification without encoding.
`NW_SECURITY_LEVEL_MIC64`|Enables 64-bit MIC verification without encoding.
`NW_SECURITY_LEVEL_MIC128`|Enables 128-bit Message Integrity Code (MIC) verification without encoding.
`NW_SECURITY_LEVEL_ENC`|Enables AES encoding without MIC.
`NW_SECURITY_LEVEL_ENC_MIC32`|Enables AES encoding with 32-bit MIC.
`NW_SECURITY_LEVEL_ENC_MIC64`|Enables AES encoding with 64-bit MIC.
`NW_SECURITY_LEVEL_ENC_MIC128`|Enables AES encoding with 128-bit MIC.

