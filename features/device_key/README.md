## DeviceKey

DeviceKey is a mechanism that implements key derivation from a Root of Trust(RoT) key. The DeviceKey mechanism generates symmetric keys that security features need. You can use these keys for encryption, authentication and more. The DeviceKey API allows key derivation without exposing the actual RoT, to reduce the possibility of accidental exposure of the RoT outside the device.

We have implemented DeviceKey according to NIST SP 800-108, section "KDF in Counter Mode", with AES-CMAC as the pseudorandom function.

### Root of Trust

The RoT key, which DeviceKey uses to derive additional keys, is generated using the hardware random generator if it exists, or using a key injected to the device in the production process.

The characteristics required by this RoT are:

- It must be unique per device.
- It must be difficult to guess.
- It must be at least 128 bits.
- It must be kept secret.

The DeviceKey feature keeps the RoT key in internal storage, using the KVStore component. Internal storage provides protection from external physical attacks to the device.

The RoT is generated at the first use of DeviceKey if the True Random Number Generator(TRNG) is available in the device. If no TRNG is available, you must pass the injected RoT key to the DeviceKey before you call the key derivation API.

### Key derivation API

`generate_derived_key`: This API generates a new key based on a string (salt) the caller provides. The same key is generated for the same salt. Generated keys can be 128 or 256 bits in length.

#### Root of Trust Injection API

`device_inject_root_of_trust`: You must call this API once in the lifecycle of the device, before any call to key derivation, if the device does not support TRNG (`DEVICE_TRNG` is not defined).

#### Using DeviceKey 

DeviceKey is a singleton class, meaning that the system can have only a single instance of it.

To instantiate DeviceKey, you need to call its `get_instance` member function as following:

```c++
    DeviceKey &deviceKey = DeviceKey::get_instance();
```

#### Testing DeviceKey

Run the DeviceKey functionality test with the `mbed` command as follows:

``` 
    ```mbed test -n features-device_key-tests-device_key-functionality```
```
