#ifndef DTLSSOCKET_H
#define DTLSSOCKET_H

#include "DTLSSocketWrapper.h"
#include "SocketAddress.h"
#include "UDPSocket.h"

// This class requires Mbed TLS SSL/TLS client code
#if defined(MBEDTLS_SSL_CLI_C)

class DTLSSocket : public DTLSSocketWrapper {
public:
    /** Create an uninitialized DTLS socket
     *
     *  Must call open to initialize the socket on a network stack.
     */
    DTLSSocket() : DTLSSocketWrapper(&_udp_socket) {}

    /** Destroy the DTLSSocket and closes the transport.
     */
    virtual ~DTLSSocket();

    /** Create a socket on a network interface
     *
     *  Creates and opens a socket on the network stack of the given
     *  network interface.
     *  If hostname is also given, user is not required to call set_hostname() later.
     *
     *  @param stack    Network stack as target for socket
     *  @param hostname Hostname used for certificate verification
     */
    template <typename S>
    DTLSSocket(S *stack, const char *hostname = NULL) : DTLSSocketWrapper(&_udp_socket, hostname)
    {
        nsapi_error_t ret = _udp_socket.open(stack);
        MBED_ASSERT(ret == NSAPI_ERROR_OK);
    }

    /** Opens a socket
     *
     *  Creates a network socket on the network stack of the given
     *  network interface. Not needed if stack is passed to the
     *  socket's constructor.
     *
     *  @param stack    Network stack as target for socket
     *  @return         0 on success, negative error code on failure
     */
    virtual nsapi_error_t open(NetworkStack *stack) {
        _stack = stack;
        return _udp_socket.open(stack);
    }

    template <typename S>
    nsapi_error_t open(S *stack) {
        return open(nsapi_create_stack(stack));
    }

    using DTLSSocketWrapper::connect;

    /** Connects TCP socket to a remote host
     *
     *  Initiates a connection to a remote server specified by either
     *  a domain name or an IP address and a port.
     *
     *  @param host     Hostname of the remote host
     *  @param port     Port of the remote host
     *  @return         0 on success, negative error code on failure
     */
    nsapi_error_t connect(const char *host, uint16_t port);

private:
    UDPSocket _udp_socket;
    NetworkStack *_stack;
    SocketAddress _remote_address;
};

#endif
#endif