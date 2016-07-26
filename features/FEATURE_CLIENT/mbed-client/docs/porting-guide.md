<span class="notes">**Note:** To port mbed Client, you need to [install yotta and its dependencies](https://github.com/ARMmbed/yotta/blob/master/docs/index.md).</span>

# mbed Client structure and build process

<span class="tips">**Tip:** Before embarking on your own port, you should build the core mbed Client for an existing compilation target to get an understanding of how the mbed Client builds.</span>

mbed Client is structured as a set of modules. Each module declares which other modules it depends on. When you build a module, our build system `yotta` looks at these dependencies and installs the necessary modules before completing the build.

This is also the process to build applications for the mbed Client (including the example application in the release). The application declares dependencies on the mbed Client and when it is built, `yotta` ensures that the modules (and anything that they depend on, recursively) are present before building. 

In general, `yotta` downloads and installs the necessary modules over the internet from the public yotta Registry (which saves published versions of modules) or from a specified source control URL.

# Components of mbed Client

## Component software modules

mbed Client consists of one main component. More major components will be added in future development phases:

 * `mbed-client` is the core mbed Client, providing C++ APIs for the mbed Client.

This module depends on further internal modules:

```
mbed-client x.x.x
|
|_mbed-client-c x.x.x
|   |_mbed-client-libservice x.x.x
|
|_mbed-client-linux x.x.x
```

To list the dependency trees, use the [`yotta list --all` command](http://docs.yottabuild.org/reference/commands.html).

<span class="notes">**Note**: In this case, we have listed the dependencies for the `x86-linux-native` compilation target. Different modules are needed for different compilation targets.</span>

We are using the [mbed Client Linux example](https://github.com/ARMmbed/mbed-client-linux-example) in this document. You can see that it depends directly only on the `mbed-client` and `mbed-client-linux` modules. These modules depend internally on various other modules.

```
mbed-client-linux-example x.x.x
|
|_mbed-client x.x.x
   |
   |_mbed-client-c x.x.x
   |   |_mbed-client-libservice x.x.x
   |
   |_mbed-client-linux x.x.x
```

## Compilation targets

To compile for a target board, you need a [target description](http://docs.yottabuild.org/tutorial/targets.html) that describes how to compile for the target. 

The `mbed-client` module uses the platform name that each target defines to choose which `mbed-client-<platform-name>` module to depend on to provide the platform-specific implementation.

# Porting mbed Client to a different platform

To port mbed Client to a new platform:

 1. [Request for a development repository](#requesting-for-a-development-repository).
 2. [Create a yotta compilation target for your board](#creating-a-yotta-compilation-target).
 3. [Implement the `mbed-client-xxx` module for your target platform](#implementing-mbed-client-xxx).
 4. [Modify the `module.json` of the `mbed-client` module](#modifying-the-json-file-in-the-mbed-client-module).
 5. [Verify that your implementation is correct](#testing-and-verification).

The `yotta` build system is designed for easy reuse of generic modules. If you intend to support multiple platforms that share common features, we recommend moving the common functionality into a separate module and use it for each platform.

## Requesting for a development repository

We provide private git repositories to our partners porting mbed Client. Only the members of the mbed Client team and relevant partner contacts and engineers have access to these repositories.

When you contact `support@mbed.org`, a repository will be created for your module. You also need to provide the target description of your board as follows:

- **`mbed-client-<platform-name>`** is the module that provides the `mbed-client-xxx` implementation for your platform. You may choose to split it into further modules in the future, to enable sharing of code, but we recommend that you implement the port for your first board in this module itself. 

- **`target-<targetname>`** contains the yotta target description of the target you are porting to. This is usually your platform name.

## Creating a yotta compilation target

An example on compiling for linux target can be found in the `yotta_targets` directory of [the example application] (https://github.com/ARMmbed/mbed-client-linux-example).

Please, refer to the [yotta documentation](http://yottadocs.mbed.com/tutorial/targets.html) for setting up your compilation target.

1.To make your target available locally (without publishing it), you use the `yotta link-target` command to link it into the global install targets directory:

```
# in the directory of your target:
yotta link-target
```

2.Use `yotta link-target <targetname>` command to make the globally linked target available when compiling another module.

3.Use the `yotta target <targetname>` command to select your target for the compilation.

## Implementing mbed-client-xxx

Clone your `mbed-client-<your-platform-name>` module and `mbed-client` modules from GitHub.

The `mbed-client-<your-platform-name>` module needs to provide a socket and timer implementation for your target platform. The `mbed-client-xxx` module should include files `m2mconnectionhandler.h`and `m2mtimer.h` from `mbed-client` and implement a corresponding `.cpp` file that points to the platform-specific private implementations of the timer and the socket.

<span class="notes">**Note**: Private implementation classes **must** be named as `M2MConnectionHandlerPimpl` and `M2MTimerPimpl`, because of forward declarations.</span>

An example of mbed-client-platform:

```
|_module.json
|
|_mbed-client-platform
|    |_m2mconnectionhandlerpimpl.h
|    |_m2mtimerpimpl.h
|
|_source
    |_m2mconnectionhandler.cpp
    |_m2mconnectionhandlerpimpl.cpp
    |_m2mtimer.cpp
    |_m2mtimerpimpl.cpp
```

To make your module available to other modules that you want to build, you need to use the [`yotta link`](http://docs.yottabuild.org/reference/commands.html#yotta-link) command to link it to the module where you want to test it out.

For example, to use your local your in-development mbed-client implementation, use the command `yotta link mbed-client-xxx` in the main `mbed-client` module.

```
# in mbed-client, link your module:
yotta link mbed-client-xxx
```

You can also just commit and push your untested code to GitHub, but it is always a good idea to test before committing.

Your `mbed-client-xxx` module must provide a platform-specific implementation for the mbed-client. The APIs that need porting are defined in the `mbed-client-linux` module. The header files contain documentation alongside the declaration of each function, where the function is described along with its parameters and return value.

There are two header files that require porting for your platform:

- `m2mconnectionhandler.h`
- `m2mtimer.h` 

To see how this is done in Linux, check the `mbed-client-linux` module from the mbed [Client Linux Example](https://github.com/ARMmbed/mbed-client-linux-example).

### Implementing the M2MConnectionHandler class for your platform

```
/*
 * Copyright (c) 2015 ARM. All rights reserved.
 */
#ifndef M2M_CONNECTION_HANDLER_H__
#define M2M_CONNECTION_HANDLER_H__

#include "mbed-client/m2mconfig.h"
#include "mbed-client/m2minterface.h"
#include "mbed-client/m2mconnectionobserver.h"
#include "nsdl-c/sn_nsdl.h"

/**
 * \brief M2MConnectionHandler.
 * This class handles the socket connection for the LWM2M Client.
 */

class M2MConnectionHandler {
public:

    /**
     * @enum ConnectionError
     * This enum defines an error that can come from
     * socket read and write operation.
     */
    typedef enum {
        CONNECTION_ERROR_WANTS_READ = -1000,
        CONNECTION_ERROR_WANTS_WRITE = -1001,
        ERROR_NONE = 0,
        SSL_CONNECTION_ERROR,
        SOCKET_READ_ERROR,
        SOCKET_SEND_ERROR,
        SOCKET_ABORT,
        DNS_RESOLVING_ERROR,
        SSL_HANDSHAKE_ERROR
    }ConnectionError;


public:

    /**
    * \brief Constructor
    */
    M2MConnectionHandler(M2MConnectionObserver &observer,
                         M2MConnectionSecurity* sec,
                         M2MInterface::BindingMode mode,
                         M2MInterface::NetworkStack stack);

    /**
    * \brief Destructor
    */
    ~M2MConnectionHandler();

    /**
    * \brief This binds the socket connection.
    * \param listen_port Port to be listened to for an incoming connection.
    * \return True if successful, else false.
    */
    bool bind_connection(const uint16_t listen_port);

    /**
    * \brief This resolves the server address. Output is
    * returned through a callback.
    * \param String The server address.
    * \param uint16_t The server port.
    * \param ServerType The server type to be resolved.
    * \param security The M2MSecurity object that determines which
    * type of secure connection will be used by the socket.
    * \return True if address is valid, else false.
    */
    bool resolve_server_address(const String& server_address,
                                const uint16_t server_port,
                                M2MConnectionObserver::ServerType server_type,
                                const M2MSecurity* security);

    /**
    * \brief Sends data to the connected server.
    * \param data_ptr The data to be sent.
    * \param data_len The length of data to be sent.
    * \param address_ptr The address structure to which the data needs to be sent.
    * \return True if data is sent successfully, else false.
    */
    bool send_data(uint8_t *data_ptr,
                           uint16_t data_len,
                           sn_nsdl_addr_s *address_ptr);

    /**
    * \brief Listens to the incoming data from a remote server.
    * \return True if successful, else false.
    */
    bool start_listening_for_data();

    /**
    * \brief Stops listening to the incoming data.
    */
    void stop_listening();

    /**
     * \brief Sends directly to the socket. This is used by
     * security classes to send the data after it has been encrypted.
     * \param buf Buffer to send.
     * \param len The length of the buffer.
     * \return Number of bytes sent or -1 if failed.
     */
    int send_to_socket(const unsigned char *buf, size_t len);

    /**
     * \brief Receives directly from the socket. This
     * is used by the security classes to receive raw data to be decrypted.
     * \param buf Buffer to send.
     * \param len The length of the buffer.
     * \return Number of bytes read or -1 if failed.
     */
    int receive_from_socket(unsigned char *buf, size_t len);

    /**
    * \brief Closes the open connection.
    */
    void close_connection();

    /**
    * \brief Error handling for DTLS connectivity.
    * \param error Error code from the TLS library.
    */
    void handle_connection_error(int error);

    /**
     * \brief Sets the network interface handler that is used by client to connect
     * to a network over IP..
     * \param handler A network interface handler that is used by client to connect.
     *  This API is optional but provides a mechanism for different platforms to
     * manage usage of underlying network interface by client.
     */
    void set_platform_network_handler(void *handler = NULL);

    /**
    * \brief Claims mutex to prevent thread clashes
    * in multithreaded environment.
    */
    void claim_mutex();

    /**
    * \brief Releases mutex to prevent thread clashes
    * in multithreaded environment.
    */
    void release_mutex();

private:

    M2MConnectionObserver                       &_observer;
    M2MConnectionHandlerPimpl                   *_private_impl;

friend class Test_M2MConnectionHandler;
friend class Test_M2MConnectionHandler_mbed;
friend class Test_M2MConnectionHandler_linux;
friend class M2MConnection_TestObserver;
};

#endif //M2M_CONNECTION_HANDLER_H__

```

Please note that some of these functions are asynchronous in nature and some are expecting a callback from the network. For example, receiving data from a socket needs to be communicated back to `mbed-client` so that the library can act on the data received. The callback comes through the Observer class defined in `M2MConnectionObserver`.

The file `m2mconnectionobserver.h` is present in `mbed-client`. To see how the callback needs to be called, check the implementation in `m2mconnectionhandlerpimpl.cpp` in  `mbed-client-linux`. 

```
/*
 * Copyright (c) 2015 ARM. All rights reserved.
 */
#ifndef M2M_CONNECTION_OBSERVER_H__
#define M2M_CONNECTION_OBSERVER_H__

#include "mbed-client/m2minterface.h"

/**
 * @brief Observer class for informing socket activity to the state machine
 */

class M2MConnectionObserver
{

public :

    /**
      * \enum ServerType, Defines the type of the
      * server that the client wants to use.
      */
    typedef enum {
        Bootstrap,
        LWM2MServer
    }ServerType;

    /**
     * \brief The M2MSocketAddress struct.
     * Unified container for holding socket address data
     * across different platforms.
     */
    struct SocketAddress{
        M2MInterface::NetworkStack  _stack;
        void                        *_address;
        uint8_t                     _length;
        uint16_t                    _port;
    };

    /**
    * \brief Indicates that data is available from socket.
    * \param data The data read from the socket.
    * \param data_size The length of the data read from the socket.
    * \param address The address of the server where the data is coming from.
    */
    virtual void data_available(uint8_t* data,
                                uint16_t data_size,
                                const M2MConnectionObserver::SocketAddress &address) = 0;

    /**
    * \brief Indicates an error occured in socket.
    * \param error_code The error code from socket, it cannot be used any further.
    * \param retry Indicates whether to re-establish connection.
    */
    virtual void socket_error(uint8_t error_code, bool retry = true) = 0;

    /**
    * \brief Indicates that the server address resolving is ready.
    * \param address The resolved socket address.
    * \param server_type The type of the server.
    * \param server_port The port of the resolved server address.
    */
    virtual void address_ready(const M2MConnectionObserver::SocketAddress &address,
                               M2MConnectionObserver::ServerType server_type,
                               const uint16_t server_port) = 0;

    /**
    * \brief Indicates that data has been sent successfully.
    */
    virtual void data_sent() = 0;
};

#endif // M2M_CONNECTION_OBSERVER_H__

```

### Implementing M2MTimer class for your platform

This class provides the periodic timer functionality for your platform.

```
/*
 * Copyright (c) 2015 ARM. All rights reserved.
 */
#ifndef M2M_TIMER_H
#define M2M_TIMER_H

#include <stdint.h>

class M2MTimerObserver;
/**
 * @brief M2MTimerImpl
 * Private implementation class for timer, this can be
 * modified based on the board on which mbed Client needs
 * to be used.
 */
class M2MTimerImpl
{
private:

    // Prevents the use of assignment operator
    M2MTimer& operator=(const M2MTimer& other);

    // Prevents the use of copy constructor
    M2MTimer(const M2MTimer& other);
    
public:

    /**
    * Constructor.
    */
    M2MTimer(M2MTimerObserver& _observer);

    /**
    * Destructor.
    */
    virtual ~M2MTimer();

    /**
     * Starts timer
     * @param interval Timer's interval in milliseconds
    * @param single_shot defines if timer is ticked
    * once or is it restarted everytime timer expires.
    */
    void start_timer(uint64_t interval, bool single_shot = true);

    /**
     * @brief Starts timer in DTLS manner.
     * @param intermediate_interval Intermediate interval to use, must be smaller than tiotal (usually 1/4 of total).
     * @param total_interval Total interval to use, this is the timeout value of a DTLS packet.
     * @param type Type of the timer
     */
    void start_dtls_timer(uint64_t intermediate_interval, uint64_t total_interval, 
                          M2MTimerObserver::Type type = M2MTimerObserver::Dtls);

    /**
    * Stops timer.
    * This cancels the ongoing timer.
    */
    void stop_timer();
    
    /**
     * @brief Checks if the intermediate interval has passed.
     * @return true if interval has passed, false otherwise.
     */
    bool is_intermediate_interval_passed();

    /**
     * @brief Checks if the total interval has passed.
     * @return true if interval has passed, false otherwise.
     */
    bool is_total_interval_passed();

};

#endif // M2M_TIMER_H
```

The timer API functions are asynchronous in nature and whenever a timer event is available, `mbed-client` is notified, so that the library can act on the _timer expired_ signal. The callback is received through an Observer class defined in `M2MTimerObserver` .

The file `m2mtimerobserver.h` is present in `mbed-client`. To see how the callback needs to be called, check the implementation in `m2mtimerimpl.cpp` in `mbed-client-linux`. 

```
/*
 * Copyright (c) 2015 ARM. All rights reserved.
 */
#ifndef M2M_TIMER_OBSERVER_H
#define M2M_TIMER_OBSERVER_H

/**
 *  Observer class for informing the parent class of the timer expiry.
 */
class M2MTimerObserver
{
public:
    /**
      * \enum Defines the types of timer
      * that can be created for mbed Client.
      */
    typedef enum {
        Notdefined,
        Registration,
        NsdlExecution,
        PMinTimer,
        PMaxTimer,
        Dtls,
        QueueSleep,
        RetryTimer
    }Type;

    /**
    * \brief Indicates that the timer has expired.
    * \param type The type of the timer that has expired.
    */
    virtual void timer_expired(M2MTimerObserver::Type type =
                               M2MTimerObserver::Notdefined) = 0;
};

#endif // M2M_TIMER_OBSERVER_H
```

## Modifying the `json` file in the `mbed-client` module

You need to add your target name to `module.json` so that when you set `yt target <platform>`, yotta can resolve the dependency correctly and link the main library with your module.

Two platforms, mbed OS and Linux, are already supported. You just need to add your module support after that.

```
{
  "name": "mbed-client",
  "version": "1.12.0",
  "description": "Mbed Client API",
  "private": true,
  "keywords": [],
  "author": "XXX XXX <xxx.xxx@xxx.com>",
  "homepage": "https://github.com/ARMmbed/mbed-client",
  "licenses": [
    {
      "url": "https://spdx.org/licenses/Apache-2.0",
      "type": "Apache-2.0"
    }
  ],
  "dependencies": {
    "mbed-client-c": "^2.0.0"
  },
  "targetDependencies": {
    "arm": {
      "mbed-client-mbed": "^3.0.0"
    },
    "linux": {
      "mbed-client-linux": "^3.0.0"
    },
    "<your platform as defined in target.json>" : {
      "mbed-client-platform": "<published version , can be done later, first link locally as explained in the steps above>"
    },
  }
}
```

## Testing and verification

You can build your mbed-client port immediately:

```
# use the target we previously made locally available (not necessary if your target has been published):
yotta link-target <yourtargetname>
# build!
yotta build
```

A `helloworld-mbedclient` program will be produced inside the `build/<yourtargetname>/test/` directory. This test application may require some changes to compile and run for your platform. Check for compilation errors. If you find any, fix the test application for your testing. 

Follow the `readme` [instructions](https://github.com/ARMmbed/mbed-client-linux-example) of the `mbed-client-linux` example to see what the test application can do.
