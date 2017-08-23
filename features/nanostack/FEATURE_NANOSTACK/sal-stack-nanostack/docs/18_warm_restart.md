The warm restart feature
========================

This chapter describes the functionality of the warm restart feature. It contains the following sections:

- [_Application configuration_](#application-configuration)
- [_mbed TLS configuration_](#mbed-tls-configuration)

## Application configuration

This section describes the functionalities needed to enable load balancing from an application.

### NVM storage

To use the warm restart feature, your device needs to have non-volatile memory (NVM) available. The security parameters are stored into it, so a secure storage should be used when possible. The application must control the data storing intervals. If you are using a flash drive, you should save scarsely to ensure a long lifetime for it.

### WPAN & PANA configuration 

To understand how to configure WPAN & PANA to receive security parameters, refer to [Network Definition APIs](09_API_network_def.md). Basically, you need to register callback functions for both to receive security paramaeters whenever they change. 

<span class="notes">**Note:** WPAN parameters change very often. Make sure to store them regularly.</span>

To register WPAN callbacks, use the following function:

```
net_nvm_wpan_params_storage_enable
```

To register PANA callbacks, use one of the following functions (based on the type of device):

```
pana_client_nvm_callback_set
pana_server_nvm_callback_set
```

## mbed TLS configuration

This section describes the needed configuration for mbed TLS to use the warm restart feature.

<span class="notes">**Note:** The server side must support mbed TLS session caching for warm restart to work. There is no extra logic needed even if the server does not support session caching (full handshake is performed every time).</span>

Every device initially "cold starts", meaning that a full handshake is performed with server. After the full handshake, the session parameters can be fetched by calling:

```
mbedtls_ssl_get_session (See mbedtls/ssl.h for details)
```

When the SSL session details are known, call the function:

```
mbedtls_ssl_set_session (See mbedtls/ssl.h for details),
```

Finally, to enable "session id" based SSL resume, call:

```
mbedtls_ssl_handshake_step,
```

