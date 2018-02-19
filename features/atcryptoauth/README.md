# Atmel Crypto Auth (ATCAECC508A) driver
This is the device driver for Atmel Crypto Auth device ATCAECC508A. It provides:
- Driver for using ATCAECC508A device
- Mbed TLS Opaque keys setup
- Device commissioning sample
- SSL client sample

## About ATCAECC508A
- Locking configuration zone (EEPROM region containing config) is necessary to use the device. 
- The device allows locking data zone(region containing keys and certificates).

## Design considerations 
- This is the first draft of the driver. It is developed for demonstrating ECDSA operations using Opaque keys feature. Hence, only ECDSA sign and verify operations are implemented.
- The device is commissioned with a minimilisted configurations to allows ECDSA operations.
- Hence it is locked using the sample commission app.
- Locking data zone is not required by the driver and it is not a requirement for sample or demo purposes.
- The device is not configured to authenticate driver commands.

## ATCAECC508A configuration
The device configuration should be **locked** before use. To achieve demo requirements it is commissioned with a minimilisted configuration. The commissioning can be done using the sample commission app supplied with the driver. This app commissions following configuration in the device:
- Data slots 0-7 are configured to contain ECC Private keys.
- Slot 8 is configured to store a x509 certificate.
- Slot 9-14 is configured to store ECC Public keys.

## Commission Application.
Commission app is an interactive console app. It prints following options: 
```
Options:
a) Commission ATCAECC508A.
b) Dump config.
c) Generate Private key.
d) Run SSL client example.

Enter choice:
```
Commissioning is done in two steps:
- First, pressing character 'a' configures the device as described above. This step challenges the user to lock the config. On pressing 'y' the configuration is locked. Note: this step is **irreversible**.
- Next, pressing 'c' generated a Private key in slot 0. This key can be referred as *Key Id 0* with the driver API.
***The demo config is done!***

Note: The device allows re-generating Private keys. Any Public key or Certificates generated using same Key Id will be invalidated on re-generating the Private key.

## TODO configuration for SSL client
## command to generate certificate 
../../../mbedtls_cert_write/programs/x509/cert_write.exe subject_key=remote0COM18 issuer_key=remote0COM18 issuer_name=CN=Cert,O=mbed TLS,C=UK authority_identifier=0

# command to run server
../../../mbedtls_cert_write/programs/ssl/ssl_server2.exe server_addr=10.2.203.146 debug_level=4 auth_mode=required crt_file=/c/server/e112111.emea.arm.com_prime256v1.pem key_file=/c/server/e112111.emea.arm.com_prime256v1_priv.pem ca_file=cert.crt  force_ciphersuite=TLS-ECDHE-ECDSA-WITH-AES-128-GCM-SHA256
