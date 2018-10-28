# TLSSocket - Design document
## Overview and Background

This document describes design of TLSSocket class that provides simple interface for Mbed OS
user to create TLS connections over TCP socket.

This class greatly simplifies the usage of TLS but limits itself to only one use case.
This design limitation is accepted as other users can continue using Mbed TLS API
directly. TLSSocket also exposes internal Mbed TLS structures allowing use of Mbed TLS API to configure the underlying library.

High level goal is to demonstrate Mbed OS users that secure connections are not hard to do.

## Design Limitations

Following design limitations exist in the current design of TLSSocket

* `TLSSocket::connect()` is always blocking
* Can only use server and client certificates through `set_root_ca_cert()` and `set_client_cert_key()` methods. For other use cases, internal Mbed TLS structures are exposed.
* No PSK mode

# System Architecture and High-Level Design

Secure socket consist of two classes:

* `TLSocketWrapper` <br>
    Which handles initialization of TLS library and does TLS handsake. Takes any Socket as a
    trasport.
* `TLSSocket` <br>
    Which inherits TLSocketWrapper and has TCP socket as a transport.
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
,----------------------------.
|TLSocketWrapper             |
|----------------------------|
|Socket *transport;          |
|----------------------------|
|TLSocketWrapper(*transport);|
|int set_root_ca_cert(...);  |
|void set_hostname(...);     |
|int do_handshake();         |
`----------------------------'
               |
,----------------------------.
|TLSSocket                   |
|----------------------------|
|TCPSocket transport         |
|----------------------------|
|int connect(char *hostname);|
`----------------------------'
```

## High Level Design Goal 1: Abstract socket API

Secure socket both uses `Socket` interface as its transport layer and implements it. This makes it transport independent and there is no direct dependency to IP stack.

When TLSSocket implements Socket API it is able to be used instead of TCP connection in
any Mbed OS library. For example MQTT library is made secure without any code changes:
https://github.com/coisme/Mbed-to-Azure-IoT-Hub/tree/new-TLSSocket

## High Level Design Goal 2: Only certificate based authentication

Aim is to first support only certificate based authentication, so we implement only `set_root_ca_cert()` and `set_client_cert_key()` functions. For later on, different types of authentication methods can be added.

## High Level Design Goal 3: Support both blocking and non-blocking operations

As the Mbed TLS is already work with both socket types, we are able to `TLSocketWrapper`
that can handle both types as well.

Functions `set_blocking()` and `set_timeout()` just pass the information for underlying
transport socket. Extra handling on the TLS state machine is not required.

## High Level Design Goal 4: Expose Mbed TLS structures

Exposing Mbed TLS configuration structures allows user to configure the underlying TLS instance using Mbed TLS API. This allows greater usability as TLSSocket is not limited to only one use case.

Also configuration structures can be shared between sockets which leads to RAM saving then two or more TLSSockets are used with same parameters.

## System Architecture and Component Interaction
*Description and diagrams showing overall architecture of how the above-mentioned components/resources interface with each other.*

# Detailed Design
*This section provides detailed design on implementation of components/modules mentioned in High Level Design section.*

##  Detailed Design for Abstract socket API

Mbed OS [Socket interface](https://github.com/ARMmbed/mbed-os/blob/master/features/netsocket/Socket.h) is a abstract C++ inteface that follows POSIX socket API.

### Receiving and sending data from Mbed TLS

`TLSSocketWrapper` contains static wrappers `ssl_recv()` and `ssl_send()` functions which are
registered to Mbed TLS library in `mbedtls_ssl_set_bio()` call.

There functions then call the transport socket's `Socket::send()` and `Socket::recv()` calls
respectively. Error coded are passed through, except `NSAPI_ERROR_WOULD_BLOCK` which is translated to `MBEDTLS_ERR_SSL_WANT_WRITE` or `MBEDTLS_ERR_SSL_WANT_READ`.

### Providing Socket API

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

`TLSSocketWrapper` provides following API to set server certificate. You can use either BASE64 formatted PEM certificate, or binary DER certificates. Later form just assumes `root_ca_pem` or `client_cert_pem` to be standard C string and counts its lenght and passes to method which takes just `void*` and `len`.

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

##  Detailed Design for Support both blocking and non-blocking operations

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

## Detailed Design for exposing Mbed TLS structures

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
a.set_root_ca_cert(<cert>);
b.set_ssl_config(a.get_ssl_config());
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

## Reusability

Parts of the state machine are probably relevant when implementing DTLS socket.

TLSSocketWrapper is entirely reusable when doing TLS handshake using any socket type.
It does not have tight bindings to TCP.

## Assumptions

We are assuming that server certificate is given from the application to `TLSSocket::set_root_ca_cert()` interface in a format that is understood by Mbed TLS.

## Deprecations

No deprecations
