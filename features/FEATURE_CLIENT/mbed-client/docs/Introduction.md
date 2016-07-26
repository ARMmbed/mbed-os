# ARM mbed Client overview

ARM mbed Client is a library that provides the means to connect constrained embedded devices to mbed Device Connector Service, mbed Device Server and to mbed-enabled cloud services from our partners.

The mbed Client high-level APIs allow mbed OS developers to create applications with LWM2M features as described in the [Lightweight Machine to Machine Technical Specification](http://technical.openmobilealliance.org/Technical/technical-information/release-program/current-releases/oma-lightweightm2m-v1-0):

- Manage devices on mbed Device Server.
- Securely communicate with internet services over the industry standard TLS/DTLS.
- Fully control the endpoint and application logic. 

The API is written in C++ to allow quick application development.

## Managing devices on mbed Device Server

mbed Client supports the following three features introduced in the subsequent chapters:

- [Client Registration and Deregistration](client_reg_dereg.md)
- [Device Management and Service Enablement](dev_man_serv_enable.md)
- [Information Reporting](info_reporting.md)

The API also provides an interface to define the application endpoint information. This information will be delivered to mbed Device Server during the registration operation.

First, you need to create an interface for mbed Client:

```
#include "mbed-client/m2minterfacefactory.h"
#include "mbed-client/m2minterface.h"

M2MInterface* interface = M2MInterfaceFactory::create_interface(*this,
                                                  "mbed-endpoint",
                                                  "test",
                                                  3600,
                                                  5684,
                                                  "",
                                                  M2MInterface::UDP,
                                                  M2MInterface::LwIP_IPv4,
                                                  "");
```

### Setting up own random number generator function

To provide a stronger security mechanism, mbed Client requires a random number generator to feed a random number into the underlying SSL library. There is a default PRNG seeded with RTC for security but some platforms do not have RTC, and for some, time value seeded PRNG is not secure enough. 

Now, an application can pass its own RNG implementation to mbed Client as function pointer callback through an API, `set_random_number_callback(random_number_cb callback)`.

Here is an example on how you can use it from an application:

```
#include "mbed-client/m2minterfacefactory.h"
#include "mbed-client/m2minterface.h"

uint32_t get_random_number(void)
{
    uint32_t i = 0;
    printf("\Your application's RNG logic\n");
    return i;
}

_interface->set_random_number_callback(&get_random_number);

```

### Setting up own entropy function for additional secure connectivity 

mbed Client provides an API to add your own entropy source into the underlying SSL library. There is a default entropy source provided by mbed Client. It uses PRNG seeded with RTC for the security but some platforms do not have RTC, and for some, this level of security may not be strong enough. 

Now, an application can pass its own entropy source to mbed Client as function pointer callback through an API, `set_entropy_callback(entropy_cb callback)`.

Here is an example on how you can use it from an application:

```
#include "mbed-client/m2minterfacefactory.h"
#include "mbed-client/m2minterface.h"

entropy_cb ent_cb;

int ent_poll( void *, unsigned char *output, size_t len,
                           size_t *olen )
{
    for(uint16_t i=0; i < len; i++){
        srand(time(NULL));
        output[i] = rand() % 256;
    }
    *olen = len;

    return( 0 );
}

    ent_cb.entropy_source_ptr = ent_poll;
    ent_cb.p_source = NULL;
    ent_cb.threshold = 128;
    ent_cb.strong = 0;

_interface->set_entropy_callback(ent_cb);

```

### Maximum UDP message size

The maximum single UDP message size that mbed Client can receive is 1152 bytes. The actual payload size is 1137 bytes, the header information using the remaining 15 bytes. 

For transferring larger amounts of data, the Blockwise feature must be deployed. When using this feature, mbed Client can handle messages up to 64KB. This feature is disabled by default.

To enable the Blockwise feature, you need to create a `config.json` file in the application level. 

*Example:*
```
{
"coap_max_blockwise_payload_size": 1024
}
```

Acceptable values for the `coap_max_blockwise_payload_size` flag are:
0, 16, 32, 64, 128, 256, 512 and 1024. Value 0 means that the feature is not used.

### CoAP message deduplication

Message duplication is disabled by default. More information about deduplication in the [CoAP specification](https://tools.ietf.org/html/rfc7252#page-24).

To enable message deduplication, you need to create a `config.json` file in the application level.

*Example:*
```
{
"coap_duplication_max_msgs_count": 1
}
```
Recommended values for the `coap_duplication_max_msgs_count` flag are 0 to 6. Value 0 means that the feature is not used. It is not recommended to use higher value than 6, because it increases the memory consumption.

## How to use the API
More information on how to use the API effectively to create and configure Objects, Object Instances and Resources, can be found [here](Howto.md).

## API documentation

The documentation for this API is [available here](https://docs.mbed.com/docs/mbed-client-guide/en/latest/api/annotated.html).

## Example application

We have an example application for

1. [mbed OS](https://github.com/ARMmbed/mbed-client-examples).

2. [Ubuntu](https://github.com/ARMmbed/mbed-client-linux-example).

