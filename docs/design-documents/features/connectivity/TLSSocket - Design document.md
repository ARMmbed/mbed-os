# TLSSocket - design document
## Overview and background

This document describes the design of the TLSSocket and DTLSSocket classes, which provide an interface for an Mbed OS user to create TLS connections over a TCP socket or DTLS over a UDP socket.

Mbed OS provides two different implementations for TLS sockets; Mbed TLS based (default) or offloaded TLS socket when used network stack (for example, a cellular modem target driver) supports it.

This class greatly simplifies the use of TLS but limits itself to only one use case. This design limitation is accepted as other users can continue using Mbed TLS API directly. Mbed TLS based TLSSocket also exposes internal Mbed TLS structures allowing use of Mbed TLS API to configure the underlying library.

DTLSSocket shares most of its functionality with TLSSocket (via `TLSSocketWrapper` class), just using UDP instead of TCP as transport socket and handling connection differently.

The high-level goal is to demonstrate that secure connections are not hard to do.

## Design limitations

The following design limitations exist in the current design of TLSSocket:

Mbed TLS based TLSSocket:

* `TLSSocket::connect()` is always blocking.
* Can only use server and client certificates through `set_root_ca_cert()` and `set_client_cert_key()` methods. For other use cases, internal Mbed TLS structures are exposed.
* No PSK mode.

Offloaded vs. Mbed TLS based TLSSocket:

* For offloaded TLS socket, `set_root_ca_cert()` and `set_client_cert_key()` must be called after `TLSSocket::open()` and before `TLSSocket::connect()`.
* Offloaded TLS socket API does not support all Mbed TLS based TLSSocket methods, but common ones (in other words, `open()`, `connect()`, `close()` and setting certficates) use the same API.

# System architecture and high-level design

The Mbed TLS based secure socket consists of four classes:

* `TLSSocketWrapper`, which handles initialization of TLS library, does the TLS handsake and takes any Socket as a transport.
* `TLSSocket`, which inherits TLSSocketWrapper, has TCP socket as a transport and adds `connect(char *hostname)` for initiating the TCP and TLS handshakes at one call.
* `DTLSSocketWrapper`, which inherits from TLSSocketWrapper and adds a few auxiliary functions.
* `DTLSSocket`, which inherits DTLSSocketWrapper, has UDP socket as a transport and adds `connect(char *hostname)` for initiating the UDP and DTLS handshakes at one call.

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
               |______________________________
               |                              |
               |               ,----------------------------.
               |               |DTLSSocketWrapper           |
               |               |----------------------------|
               |               |timer-related functions     |
               |               `----------------------------'
               |                               |
,----------------------------. ,----------------------------.
|TLSSocket                   | |DTLSSocket                  |
|----------------------------| |----------------------------|
|TCPSocket transport         | |UDPSocket transport         |
|----------------------------| |----------------------------|
|int connect(char *hostname);| |int connect(char *hostname);|
`----------------------------' `----------------------------'
```

Offloaded TLS socket:

* `TLSSocket` <br>
    Extends the TCP socket with certicate and key setter methods and internally handles TLS socket control to network stack using offload TLS socket setsockopt() settings.

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

## High-level design goal 1: Abstract Socket API

Mbed TLS based secure socket both uses `Socket` interface as its transport layer and implements it. This makes it transport independent, and there is no direct dependency to the IP stack.

The offloaded secure socket shares the same API as Mbed TLS based TLSSocket as much as possible. This API has no dependency on Mbed TLS APIs.

When TLSSocket implements the Socket API, it can be used instead of TCP connection in any Mbed OS library. For example, the MQTT library is made secure without any code changes: https://github.com/coisme/Mbed-to-Azure-IoT-Hub/tree/new-TLSSocket

The `DTLSSocket` class implements TLS handshake for UDPSocket.

## High-level design goal 2: Only certificate-based authentication

The aim is to first support only certificate-based authentication, so we implement only `set_root_ca_cert()` and `set_client_cert_key()` functions. Later on, different types of authentication methods can be added.

## High-level design goal 3: Support both blocking and nonblocking operations

Because Mbed TLS already works with both socket types, we can create `TLSSocketWrapper`, which handles both types as well.

Functions `set_blocking()` and `set_timeout()` just pass the information for underlying transport socket. Extra handling on the TLS state machine is not required.

## High-level design goal 4: Expose Mbed TLS structures (Mbed TLS secure socket only)

Exposing the Mbed TLS configuration structures allows you to configure the underlying TLS instance using Mbed TLS API. This allows greater usability because TLSSocket is not limited to only one use case.

Also, configuration structures can be shared between sockets, which leads to RAM savings. Then, two or more TLSSockets are used with the same parameters.

## System architecture and component interaction

*Description and diagrams showing overall architecture of how the above-mentioned components/resources interface with each other.*

# Detailed design

*This section provides detailed design on implementation of components/modules mentioned in the high-level design section.*

##  Detailed design for abstract socket API

The Mbed OS [Socket interface](https://github.com/ARMmbed/mbed-os/blob/master/features/netsocket/Socket.h) is an abstract C++ inteface that follows the POSIX socket API.

### Receiving and sending data from Mbed TLS

Mbed TLS based secure socket:

`TLSSocketWrapper` contains static wrappers `ssl_recv()` and `ssl_send()` functions, which are registered to the Mbed TLS library in `mbedtls_ssl_set_bio()` call.

These functions then call the transport socket's `Socket::send()` and `Socket::recv()` calls. Error codes pass through, except `NSAPI_ERROR_WOULD_BLOCK`, which is translated to `MBEDTLS_ERR_SSL_WANT_WRITE` or `MBEDTLS_ERR_SSL_WANT_READ`.

Offloaded secure socket:

When `TLSSocket::connect()` marks a socket as a secure socket in the network stack, the network stack must handle `socket_send()` and `socket_recv()`. In other words, in external modem cases, these methods send secure socket specific send/receive commands to the modem.

### Providing the Socket API

Mbed TLS based secure socket:

```
virtual nsapi_error_t close();
```

This destroys the memory allocated by the TLS library. It also closes the transport socket, unless `TLSSocketWrapper::keep_transport_open()` has been called earlier.

```
virtual nsapi_error_t connect(const SocketAddress &address);
```

This initiates the TCP/UDP connection and continues to the TLS hanshake.

This is currently forced to blocking mode.

```
virtual nsapi_size_or_error_t send(const void *data, nsapi_size_t size);
virtual nsapi_size_or_error_t recv(void *data, nsapi_size_t size);
virtual nsapi_size_or_error_t sendto(const SocketAddress &address, const void *data, nsapi_size_t size);
virtual nsapi_size_or_error_t recvfrom(SocketAddress *address, void *data, nsapi_size_t size);
```

These work as expected, but `SocketAddress` parameters are ignored. TLS connection can't change the peer. Also, `recvfrom()` call doesn't set the peer address.

Mbed TLS error codes `MBEDTLS_ERR_SSL_WANT_READ` and `MBEDTLS_ERR_SSL_WANT_WRITE` are translated to `NSAPI_ERROR_WOULD_BLOCK` before passing to the user.

`MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY` is ignored, and zero is returned to the user (connection closed). Other error codes pass through.

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

These are returning `NSAPI_ERROR_UNSUPPORTED` because you can't set TLS socket to listening mode.

##  Detailed design for certificate-based authentication

TLSSocket API provides the following API to set server certificate. For Mbed TLS secure socket, the `TLSSocketWrapper` class implements this API. You can use either a BASE64 formatted PEM certificate, or binary DER certificates. The latter just assumes `root_ca_pem` or `client_cert_pem` to be standard C string and counts its length and passes to method, which takes just `void*` and `len`.

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

If client authentication is required, the following API allows you to set the client certificate and private key:

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

Certificate is then passed unmodified to the `mbedtls_x509_crt_parse()` function.

##  Detailed design for support of both blocking and nonblocking operations (Mbed TLS secure socket only)

`send()` and `receive()` methods do not need to know whether the underlying socket is in blocking mode because Mbed OS return values are enough to tell that.

Data path when the application sends data to the TLS socket:

1. The application calls `TLSSocketWrapper::send()` function.
1. Which calls the `mbedtls_ssl_write()` function
1. Which calls the `TLSSocketWrapper::ssl_send()`
1. Which calls the transport socket's `Socket::send()`, which either blocks or returns `NSAPI_ERROR_WOULD_BLOCK`.

When this `NSAPI_ERROR_WOULD_BLOCK` is returned:

1. `TLSSocketWrapper::ssl_send()` translates the error to `MBEDTLS_ERR_SSL_WANT_WRITE`.
1. Returned to `mbedtls_ssl_write()`, which returns the same error.
1. `TLSSocketWrapper::send()` gets the `MBEDTLS_ERR_SSL_WANT_WRITE` and translates that to `NSAPI_ERROR_WOULD_BLOCK`.
1. Application gets `NSAPI_ERROR_WOULD_BLOCK` and turns back off.

When transport socket is in blocking mode, it never returns `NSAPI_ERROR_WOULD_BLOCK` and therefore `mbedtls_ssl_write()` never gets `MBEDTLS_ERR_SSL_WANT_WRITE`. Any translation does not happen, but code path stays unchanged.

## Detailed design for exposing Mbed TLS structures (Mbed TLS secure socket only)

TLSSocket exposes the following API to provide access to internal Mbed TLS data structures:

```
mbedtls_x509_crt *get_own_cert();
int set_own_cert(mbedtls_x509_crt *);
mbedtls_x509_crt *get_ca_chain();
void set_ca_chain(mbedtls_x509_crt *);
mbedtls_ssl_config *get_ssl_config();
void set_ssl_config(mbedtls_ssl_config *);
```

This allows sockets to share the same configuration and allow user to adjust the TLS configuration, for example:

```
TLSSocket a;
TLSSocket b;
a.open(<interface>);
b.open(<interface>);
a.set_root_ca_cert(<cert>);
b.set_ssl_config(a.get_ssl_config());
```

## Detailed design for communication between TLSSocket and network stack (offloaded secure socket only)

All TLSSocket control between TLSSocket and the network stack is handled through socket options. The network stack must implement at least the following options:

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

# Usage scenarios and examples

## Scenario 1: Connecting to a secure server

```
NetworkInterface *net = NetworkInterface::get_default_instance();

TLSSocket sock(net);
sock.add_root_CA(...);

sock.connect("my_server.mbed.com", 8080); // Connect & handle TLS handshake

// Now Socket is connected and can be used as any socket
sock.send("hello", 5);
```

Example user: https://github.com/coisme/Mbed-to-Azure-IoT-Hub/blob/new-TLSSocket/MQTTNetwork.h

# Tools and configuration changes

No tool changes required.

# Other information

## Selecting Mbed TLS secure socket or offloaded TLS socket

By default, the system is configured to use the Mbed TLS based secure socket because only a few network stacks support offloaded TLS socket. You can enable the offloaded TLS socket in the application json configuration file by setting: `"nsapi.offload-tlssocket": true`.

## Assumptions

We assume the server certificate is given from the application to `TLSSocket::set_root_ca_cert()` interface in a format that is understood by Mbed TLS or, in the case of the offloaded TLS socket, the implementation of TLS socket in the network stack.

## Deprecations

No deprecations.

## Security

The offloaded TLS socket has one major security difference compared to the Mbed TLS based secure socket. In Mbed TLS secure socket, the application processor has already encrypted the data. Therefore, all user data between MCU and network interface hardware is encrypted. But with offloaded TLS socket, encryption happens in the network stack, which can be implemented in network hardware and result in data between the MCU and network hardware being unencrypted (for example, in cellular, UART traffic between the MCU and cellular modem).

## Memory consumption

One of the main reasons to select the offloaded TLSSocket is memory consumption. In the offloaded TLSSocket, the TLS implementation can be in modem firmware, which results in a significantly smaller application size and RAM consumption if application does not need Mbed TLS functionality for other reasons.
