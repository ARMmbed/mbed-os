### DeviceKey

DeviceKey is a mechanism that can be used as a root of trust for key derivation, and is one of the most fundamental elements in the implementation of security in a device.
The DeviceKey consists of a secret and unique value per board instance, containing 128 or 256 bits.
There are several possible sources for this data, the preferred one is via this HAL API.
If this HAL API is not implemented and the device supports TRNG, mbedos will generate a DeviceKey the first time it is requested, using the TRNG, and save the value in the internal memory using NVStore.
As a last option, if none of the above is available, the DeviceKey can still be injected from outside the board and saved in NVStore using a dedicated API

##### Defined behavior

- The key should be 16 or 32 bytes long. Any other size is not supported. 
- The function device_key_get_size_in_bytes must return the actual key size in bytes.  
- The function device_key_get_value must return the key value in the provided buffer.
- The function device_key_get_value must return the actual number of bytes written to the buffer in the provided length parameter.
- If the size of the buffer is smaller than the available key, a DEVICEKEY_HAL_BUFFER_TOO_SMALL error code is must be returned.  
- The returned values of the API must be consistent during the life cycle of the device. 

##### Undefined behavior
 - There should not be any undefined behavior

#### Implementing the DeviceKey API

To enable DeviceKey support in Mbed OS, add the `DEVKEY` label in the `device_has` option of the target's section in the `targets.json` file.

#### Testing

The Mbed OS HAL provides a set of conformance tests for DeviceKey. You can use these tests to validate the correctness of your implementation. To run the DeviceKey HAL tests, use the following command:

```
mbed test -t <toolchain> -m <target> -n "tests-mbed_hal-device_key"
```
