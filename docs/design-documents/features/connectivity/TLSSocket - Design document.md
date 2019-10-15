# TLSSocket - Design document
## Overview and Background

This document describes design of TLSSocket class that provides simple interface for Mbed OS user to create TLS connections over TCP socket.

Mbed OS provides 2 different implementations for TLS socket; Mbed TLS based (default) or offloaded TLS socket when used network stack (e.g. cellular modem target driver) supports it.

This class greatly simplifies the usage of TLS but limits itself to only one use case.
This design limitation is accepted as other users can continue using Mbed TLS API directly. Mbed TLS based TLSSocket also exposes internal Mbed TLS structures allowing use of  Mbed TLS API to configure the underlying library.

High level goal is to demonstrate Mbed OS users that secure connections are not hard to do.

## Design Limitations

Following design limitations exist in the current design of TLSSocket:

Mbed TLS based TLSSocket:
* `TLSSocket::connect()` is always blocking
* Can only use server and client certificates through `set_root_ca_cert()` and `set_client_cert_key()` methods. For other use cases, internal Mbed TLS structures are exposed.
* No PSK mode

Offloaded vs. Mbed TLS based TLSSocket:
* For offloaded TLS socket `set_root_ca_cert()` and `set_client_cert_key()` must be called after `TLSSocket::open()` and before `TLSSocket::connect()`.
* Offloaded TLS socket API does not support all Mbed TLS based TLSSocket methods, but common (i.e. `open()`, `connect()`, `close()` and setting certficates) use the same API.

# System Architecture and High-Level Design

Mbed TLS based secure socket consist of two classes:

* `TLSSocketWrapper` <br>
    Which handles initialization of TLS library and does TLS handsake. Takes any Socket as a
    trasport.
* `TLSSocket` <br>
    Which inherits TLSSocketWrapper and has TCP socket as a transport.
    Adds `connect(char *hostname)`
    for initiating the TCP and TLS handshakes at one call.

```
       ,--------------.
       |Socket        |
       |--------------|
       |--------------|
       |int connect();|
       |int recv();   |
       |int send();   |
       `--------------'
               |
               |
,-----------------------------.
|TLSSocketWrapper             |
|-----------------------------|
|Socket *transport;           |
|-----------------------------|
|TLSSocketWrapper(*transport);|
|int set_root_ca_cert(...);   |
|void set_hostname(...);      |
|int do_handshake();          |
`-----------------------------'
               |
,----------------------------.
|TLSSocket                   |
|----------------------------|
|TCPSocket transport         |
|----------------------------|
|int connect(char *hostname);|
`----------------------------'
```

Offloaded TLS socket:

* `TLSSocket` <br>
    Extends TCP socket with certicate and key setter methods and internally handles TLS socket control to network stack using offload TLS socket setsockopt() settings.

```
       ,--------------.
       |TCPSocket     |
       |--------------|
       |--------------|
       |int connect();|
       |int recv();   |
       |int send();   |
       `--------------'
               |
               |
,----------------------------.        ,-----------------------------.
|TLSSocket                   |        |NetworkStack implementation  |
|----------------------------|        |-----------------------------|
|----------------------------| -----> |-----------------------------|
|int set_root_ca_cert(...);  |        |nsapi_error_t setsockopt(...)|
`----------------------------'        `-----------------------------'
```

## High Level Design Goal 1: Abstract socket API

Mbed TLS based secure socket both uses `Socket` interface as its transport layer and implements it. This makes it transport independent and there is no direct dependency to IP stack.

Offloaded secure socket shares the same API as Mbed TLS based TLSSocket as much as possible. This API has no dependency to Mbed TLS APIs.

When TLSSocket implements Socket API it is able to be used instead of TCP connection in
any Mbed OS library. For example MQTT library is made secure without any code changes:
https://github.com/coisme/Mbed-to-Azure-IoT-Hub/tree/new-TLSSocket

## High Level Design Goal 2: Only certificate based authentication

Aim is to first support only certificate based authentication, so we implement only `set_root_ca_cert()` and `set_client_cert_key()` functions. For later on, different types of authentication methods can be added.

## High Level Design Goal 3: Support both blocking and non-blocking operations

As the Mbed TLS already worsk with both socket types, we are able to create `TLSSocketWrapper` that can handle both types as well.

Functions `set_blocking()` and `set_timeout()` just pass the information for underlying
transport socket. Extra handling on the TLS state machine is not required.

## High Level Design Goal 4: Expose Mbed TLS structures (Mbed TLS secure socket only)

Exposing Mbed TLS configuration structures allows user to configure the underlying TLS instance using Mbed TLS API. This allows greater usability as TLSSocket is not limited to only one use case.

Also configuration structures can be shared between sockets which leads to RAM saving then two or more TLSSockets are used with same parameters.

## System Architecture and Component Interaction
*Description and diagrams showing overall architecture of how the above-mentioned components/resources interface with each other.*

# Detailed Design
*This section provides detailed design on implementation of components/modules mentioned in High Level Design section.*

##  Detailed Design for Abstract socket API

Mbed OS [Socket interface](https://github.com/ARMmbed/mbed-os/blob/master/features/netsocket/Socket.h) is a abstract C++ inteface that follows POSIX socket API.

### Receiving and sending data from Mbed TLS

Mbed TLS based secure socket:

`TLSSocketWrapper` contains static wrappers `ssl_recv()` and `ssl_send()` functions which are
registered to Mbed TLS library in `mbedtls_ssl_set_bio()` call.

These functions then call the transport socket's `Socket::send()` and `Socket::recv()` calls
respectively. Error coded are passed through, except `NSAPI_ERROR_WOULD_BLOCK` which is translated to `MBEDTLS_ERR_SSL_WANT_WRITE` or `MBEDTLS_ERR_SSL_WANT_READ`.

Offloaded secure socket:

When socket has been marked as secure socket in network stack by `TLSSocket::connect()`, network stack must handle `socket_send()` and `socket_recv()` respectively, i.e. in external modem case these methods send secure socket specific send/receive commands to modem.

### Providing Socket API

Mbed TLS based secure socket:

```
virtual nsapi_error_t close();
```

Destroys the memory allocated by TLS library.
Alternatively also closes the transport socket, unless `TLSSocketWrapper::keep_transport_open()` has been called earlier.


```
virtual nsapi_error_t connect(const SocketAddress &address);
```

Initiates the TCP connection and continues to TLS hanshake.
This is currently forced to blocking mode.

```
virtual nsapi_size_or_error_t send(const void *data, nsapi_size_t size);
virtual nsapi_size_or_error_t recv(void *data, nsapi_size_t size);
virtual nsapi_size_or_error_t sendto(const SocketAddress &address, const void *data, nsapi_size_t size);
virtual nsapi_size_or_error_t recvfrom(SocketAddress *address, void *data, nsapi_size_t size);
```
These work as expected, but `SocketAddress` parameters are ignored. TLS connection cannot
change the peer. Also `recvfrom()` call does not set the peer address.

Mbed TLS error codes `MBEDTLS_ERR_SSL_WANT_READ` and `MBEDTLS_ERR_SSL_WANT_WRITE` are
translated to `NSAPI_ERROR_WOULD_BLOCK` before passing to user.

`MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY` is ignored and zero is returned to user (connection closed). Other error codes are passed through.

```
virtual nsapi_error_t bind(const SocketAddress &address);
virtual void set_blocking(bool blocking);
virtual void set_timeout(int timeout);
virtual void sigio(mbed::Callback<void()> func);
virtual nsapi_error_t setsockopt(int level, int optname, const void *optval, unsigned optlen);
virtual nsapi_error_t getsockopt(int level, int optname, void *optval, unsigned *optlen);
```
These are passed through to transport socket.


```
virtual Socket *accept(nsapi_error_t *error = NULL);
virtual nsapi_error_t listen(int backlog = 1);
```
These are returning `NSAPI_ERROR_UNSUPPORTED` as TLS socket cannot be set to listening mode.

##  Detailed Design for certificate based authentication

TLSSocket API provides following API to set server certificate. For Mbed TLS secure socket, this API is implemented by `TLSSocketWrapper` class. You can use either BASE64 formatted PEM certificate, or binary DER certificates. Later form just assumes `root_ca_pem` or `client_cert_pem` to be standard C string and counts its lenght and passes to method which takes just `void*` and `len`.

```
/** Sets the certification of Root CA.
 *
 * @param root_ca Root CA Certificate in any mbed-TLS supported format.
 * @param len     Length of certificate (including terminating 0 for PEM).
 */
nsapi_error_t TLSSocketWrapper::set_root_ca_cert(const void *root_ca, size_t len);

/** Sets the certification of Root CA.
 *
 * @param root_ca_pem Root CA Certificate in PEM format
 */
nsapi_error_t TLSSocketWrapper::set_root_ca_cert(const char *root_ca_pem);
```

If client authentication is required, following API allows you to set the client certificate and private key:

```
/** Sets client certificate, and client private key.
 *
 * @param client_cert Client certification in any mbed-TLS supported format.
 * @param client_private_key Client private key in PEM format.
 */
nsapi_error_t TLSSocketWrapper::set_client_cert_key(const void *client_cert_pem, size_t client_cert_len,
                                                    const void *client_private_key_pem, size_t client_private_key_len);

/** Sets client certificate, and client private key.
 *
 * @param client_cert_pem Client certification in PEM format.
 * @param client_private_key Client private key in PEM format.
 */
nsapi_error_t TLSSocketWrapper::set_client_cert_key(const char *client_cert_pem, const char *client_private_key_pem);
```

Certificate is then passed unmodified to `mbedtls_x509_crt_parse()` function.

##  Detailed Design for Support both blocking and non-blocking operations (Mbed TLS secure socket only)

`send()` and `receive()` methods do not need to know whether underlying socket is in
blocking mode as Mbed OS return values are enough to tell that.

Data path when application send data to TLS socket:

1. Application call `TLSSocketWrapper::send()` function.
1. Which calls `mbedtls_ssl_write()` function
1. Which calls `TLSSocketWrapper::ssl_send()`
1. Which calls transport socket's `Socket::send()` which either blocks or returns `NSAPI_ERROR_WOULD_BLOCK`

When this `NSAPI_ERROR_WOULD_BLOCK` is returned:

1. `TLSSocketWrapper::ssl_send()` translates error to `MBEDTLS_ERR_SSL_WANT_WRITE`
1. returned to `mbedtls_ssl_write()` which return the same error
1. `TLSSocketWrapper::send()` gets the `MBEDTLS_ERR_SSL_WANT_WRITE` and translates that to `NSAPI_ERROR_WOULD_BLOCK`
1. Application gets `NSAPI_ERROR_WOULD_BLOCK` and back off.

When transport socket is in blocking mode, it never returns `NSAPI_ERROR_WOULD_BLOCK` and therefore `mbedtls_ssl_write()` never gets `MBEDTLS_ERR_SSL_WANT_WRITE`, so any translation does not happen, but code path stays unchanged.

## Detailed Design for exposing Mbed TLS structures (Mbed TLS secure socket only)

TLSSocket exposes following API to provide access to internal Mbed TLS data structures:
```
mbedtls_x509_crt *get_own_cert();
int set_own_cert(mbedtls_x509_crt *);
mbedtls_x509_crt *get_ca_chain();
void set_ca_chain(mbedtls_x509_crt *);
mbedtls_ssl_config *get_ssl_config();
void set_ssl_config(mbedtls_ssl_config *);
```

This allows sockets to share same configuration and allow user to fine tune TLS configuration, for example:

```
TLSSocket a;
TLSSocket b;
a.open(<interface>);
b.open(<interface>);
a.set_root_ca_cert(<cert>);
b.set_ssl_config(a.get_ssl_config());
```

## Detailed Design for communication between TLSSocket and network stack (Offloaded secure socket only)

All TLSSocket control between TLSSocket and network stack is handled via socket options. Network stack must implement at least following options:

```
typedef enum nsapi_tlssocket_level {
    NSAPI_TLSSOCKET_LEVEL   = 7099, /*!< TLSSocket option level - see nsapi_tlssocket_option_t for options*/
} nsapi_tlssocket_level_t;

typedef enum nsapi_tlssocket_option {
    NSAPI_TLSSOCKET_SET_HOSTNAME,   /*!< Set host name */
    NSAPI_TLSSOCKET_SET_CACERT,     /*!< Set server CA certificate */
    NSAPI_TLSSOCKET_SET_CLCERT,     /*!< Set client certificate */
    NSAPI_TLSSOCKET_SET_CLKEY,      /*!< Set client key */
    NSAPI_TLSSOCKET_ENABLE          /*!< Enable TLSSocket */
} nsapi_tlssocket_option_t;
```

# Usage Scenarios and Examples

## Scenario 1: Connecting to secure server:

```
NetworkInterface *net = NetworkInterface::get_default_instance();

TLSSocket sock(net);
sock.add_root_CA(...);

sock.connect("my_server.mbed.com", 8080); // Connect & handle TLS handshake

// Now Socket is connected and can be used as any socket
sock.send("hello", 5);
```

Example user: https://github.com/coisme/Mbed-to-Azure-IoT-Hub/blob/new-TLSSocket/MQTTNetwork.h

# Tools and Configuration Changes

No tool changes required

# Other Information

## Selecting Mbed TLS secure socket or offloaded TLS socket

By default system is configured to use Mbed TLS based secure socket as only few network stacks support offloaded TLS socket.
Offloaded TLS socket can be enabled in application json configuration file by setting: `"nsapi.offload-tlssocket": true`

## Reusability

Mbed TLS secure socket:
Parts of the state machine are probably relevant when implementing DTLS socket.

TLSSocketWrapper is entirely reusable when doing TLS handshake using any socket type.
It does not have tight bindings to TCP.

Offloaded secure socket:
Current implementation is for TLS only and does not support DTLS.

## Assumptions

We are assuming that server certificate is given from the application to `TLSSocket::set_root_ca_cert()` interface in a format that is understood by Mbed TLS or, in offloaded TLS socket case, the implementation of TLS socket in network stack.

## Deprecations

No deprecations

## Security

Offloaded TLS socket has one major security difference compared to Mbed TLS based secure socket. In Mbed TLS secure socket data is encrypted already by application processor and therefore all user data between MCU and network interface HW is encrypted. But with offloaded TLS socket, encryption happens in network stack, which can be implemented in network HW resulting in data between MCU and network HW being unencrypted (e.g. in cellular case, UART traffic between MCU and cellular modem).

## Memory consumption

Maybe one of the biggest reason selecting offloaded TLSSocket is memory consumption. In offloaded TLSSocket TLS implementation can be in modem firmware resulting in a significantly smaller application size and RAM consumption if application does not need Mbed TLS functionality for other reasons.
