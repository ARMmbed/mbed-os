# wolfSSL Port for mbed-os

Tested using Cypress `CY8CKIT-062S2-43012` and the `mbed-os-example-aws-iot-client` example.

https://github.com/cypresssemiconductorco/TARGET_CY8CKIT-062S2-43012
https://github.com/cypresssemiconductorco/mbed-os-example-aws-iot-client

## Overview

The use of wolfSSL is enabled in mbed-os using `USE_WOLFSSL_LIB`.
All configuration options come from `features/wolf/user_settings.h`.

The coding convention is:

```
#ifdef USE_WOLFSSL_LIB
    /* wolfSSL */
    #include "wolfssl/wolfcrypt/settings.h"
    #include "wolfssl/ssl.h"
#else
    /* mbedTLS */
#endif
```

## Porting

The RTC and RNG porting functions are in `mbed-os/features/wolf/wolf_port.c`.
The API's used are RTC `rtc_read` and RNG `trng_get_bytes`.

### Status of mbed-os wolfSSL porting

Complete:

* `features/netsocket/TLSSocket.cpp` (100%)
* `features/netsocket/TLSSocket.h` (100%)
* `features/netsocket/TLSSocketWrapper.cpp` (100%)
* `features/netsocket/TLSSocketWrapper.h` (100%)
* `features/nanostack/nanostack-hal-mbed-cmsis-rtos/arm_hal_random.c` (100%)
* `features/nanostack/sal-stack-nanostack/nanostack/ns_sha256.h` (100%)
* `features/nanostack/sal-stack-nanostack/source/6LoWPAN/ws/ws_pae_controller.c` (100%)
* `features/nanostack/sal-stack-nanostack/source/Security/protocols/sec_prot_lib.c` (100%)
* `features/nanostack/sal-stack-nanostack/source/Service_Libs/CCM_lib/mbedOS/aes_mbedtls_adapter.c` (100%)
* `features/nanostack/sal-stack-nanostack/source/Service_Libs/hmac/hmac_sha1.c` (100%)
* `features/device_key/source/DeviceKey.cpp` (100%)
* `features/lorawan/lorastack/mac/LoRaMacCrypto.cpp` (100%)

Incomplete:

* `features/nanostack/sal-stack-nanostack/source/Security/protocols/tls_sec_prot/tls_sec_prot_lib.c` (0%)
* `features/nanostack/sal-stack-nanostack/source/Service_Libs/nist_aes_kw/nist_aes_kw.c` (0%)
* `features/storage/kvstore/securestore/SecureStore.cpp` (0%)


## Setup

To use wolfSSL you must add the following to your `mbed_app.json`:

```
"macros": ["USE_WOLFSSL_LIB"],
```

Clone wolfSSL into features/wolf/wolfssl

```
cd features/wolf
git clone https://github.com/wolfSSL/wolfssl.git
```

## Building

Clone mbed-os example such as `mbed-os-example-aws-iot-client`

```sh
git clone https://github.com/cypresssemiconductorco/mbed-os-example-aws-iot-client.git
cd mbed-os-example-aws-iot-client

cd publisher
mbed config root .

# modify mbed-os to point to your local mbed-os path
vim mbed-os.lib
# update Wifi settings in mbed_app.json
vim mbed_app.json
# enable wolfSSL in your mbed_app.json by adding
# "macros": ["USE_WOLFSSL_LIB"],
# update AWS settings in aws_config.h
vim aws_config.h

# deploy get resources from Git/local
mbed deploy

# make sure wolfssl library is cloned into features/wolf/wolfssl
cd publisher/mbed-os
cd features/wolf
git clone https://github.com/wolfSSL/wolfssl.git
cd ../..

# build (from root)
cd ../..
mbed compile -t GCC_ARM -m CY8CKIT_062S2_43012

# build debug
mbed compile -t GCC_ARM -m CY8CKIT_062S2_43012 --profile debug
```

Note: If you need to increase stack add these to `mbed_app.json`:

```json
{
    "target_overrides": {
        "*": {
            "rtos.idle-thread-stack-size": 8192,
            "rtos.thread-stack-size": 8192
        }
    }
}
```

## AWS Publish Example Output

```
Connecting to the network using Wifi...
WLAN MAC Address : 10:98:C3:72:C0:A0
WLAN Firmware    : wl0: Oct 31 2019 03:09:53 version 13.10.271.218 (r723132) FWID 01-f298a646
WLAN CLM         : API: 18.2 Data: 9.10.0 Compiler: 1.36.1 ClmImport: 1.34.1 Creation: 2019-10-31 03:00:45 
WHD VERSION      : v1.70.0 : v1.70.0 : GCC 6.3 : 2019-12-02 04:14:53 -0600
Connected to the network successfully. IP address: 192.168.0.146
Connected to AWS endpoint
Published to topic successfully
Published to topic successfully
Published to topic successfully
...
```

## Support

For questions email wolfSSL support at: support@wolfssl.com.
