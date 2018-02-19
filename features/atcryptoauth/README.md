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
Commission app is part of the driver source. It is gaurded under compile flag ```MBED_CONF_ATCAECC_APP_ENABLE``` and disabled by default in ```mbed_lib.json```. In order to do commissioning mbed-os should be built with this flag enabled.

On start it prints following options: 
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

## Generating certificates
A self signed certificate can be generated using [Modified](https://github.com/ARMmbed/mbedtls/pull/1360) ```cert_write.exe``` application. Steps are as follows:
- Connect ATCAECC508A shield on an Mbed platform.
- Flash Mbed Platform with the sample commissioning application. Reset the board.
- Run modified ```cert_write.exe``` with parameters shown below:

```
mbedtls/programs/x509/cert_write.exe subject_key=remote0COM18 issuer_key=remote0COM18 issuer_name=CN=Cert,O=mbed TLS,C=UK authority_identifier=0 output_file=cert.pem
```
Above command on success generates a certificate in ```cert.pem``` file. This certificate can be inspected with following command:
```
openssl x509 -in cert.pem -noout -text
```

Modified ```cert_write.exe``` parameters ```subject_key``` and ```issuer_key``` are formatted in a special way to identify HW keys. 
The format is ```remote0COM18```. Here:
- ```remote``` indiates a keys accessible via remote service/driver.
- ```0``` indicates the key Id. It can be any key identifier supported by the device.
- ```COM18``` is the serial port to communicate with the ATCAECC508A sample commissioning app.

## SSL Client sample
SSL client sample is provided is the driver source for testing. It can be run from the sample commissioning app by pressing option 'd'.

This sample runs against an SSL server specifically configured to demonstrate use of ECDSA keys. Since an ad hoc server is used for testing. Change in the IP address requires changing the server certificate and client source code. From scratch following thing are required for the test setup:

- Client certificate
- Server CA cert.
- Server certificate signed by CA.
- Server certificate private key.

### Client cert
Client certificate can be obtained using steps expained earlier in this README.

### Server CA cert
In order obtain a CA cert for testing and signing server certificate(s) it is easy to setup local CA authority using ```openssl```. Following steps are executed on an Ubuntu machine.

Creat local CA workspace:
```sh
mkdir CA
cd CA
mkdir newcerts certs crl private requests foreign_keys
touch index.txt
echo "1234" > serial
```

Open ```/usr/lib/ssl/openssl.cnf``` and update section ```[CA_default]```
```
dir             = <path to CA dir>      # Where everything is kept
database        = $dir/CA/index.txt     # database index file.
certificate     = $dir/certs/cacert.pem # The CA certificate
serial          = $dir/CA/serial        # The current serial number
private_key     = $dir/private/cakey.pem# The private key
```

Generate CA private key:
```
openssl ecparam -name prime256v1 -genkey -out private/cakey.pem
```
Remember that this setup is for demonstrating ATCAECC508A that only do ECDSA with ECC NIST P256 curve. For this reason the CA key should also be of type ECC NIST P256 curve.

Generate CA certificate:
```
openssl req -new -x509 -key ./private/cakey.pem -out cacert.pem -days 3650 -set_serial 0
```
Answer appropriately to openssl prompts.

Generate a server key(again an ECC NIST P256 curve):
```
openssl ecparam -name prime256v1 -genkey -out foreign_keys/server_prime256v1_priv.pem
```

Generate server certificate signing request:
```
openssl req -new -key foreign_keys/server_prime256v1_priv.pem -out requests/server_prime256v1.csr
```
Answer appropriately to openssl prompts. Remember to put *Common Name* same as the host name that will be used by the client to connect to the server. It can be the host name that is visible on the network or it could be the server IP address.

Sign server certificate:
```
openssl ca -in requests/server_prime256v1.csr -out certs/server_prime256v1.pem
```

### Running SSL server
```ssl_server2.exe``` can be used with following parameters:
- ```server_addr=``` should be same as entered in the certificate.
- ```auth_mode=required``` to enable client authentication.
- ```crt_file=server_prime256v1.pem``` server certificate.
- ```key_file=server_prime256v1_priv.pem``` server private key.
- ```ca_file=cert.pem``` self signed client certificate.
- ```force_ciphersuite=TLS-ECDHE-ECDSA-WITH-AES-128-GCM-SHA256``` force ECDSA with SHA256.

Full command:
```
mbedtls/programs/ssl/ssl_server2.exe server_addr=<IP Addr> debug_level=4 auth_mode=required crt_file=server_prime256v1.pem key_file=server_prime256v1_priv.pem ca_file=cert.pem  force_ciphersuite=TLS-ECDHE-ECDSA-WITH-AES-128-GCM-SHA256
```

### Modifying SSL Client
SSL Client needs following changes to run successfully with the 
