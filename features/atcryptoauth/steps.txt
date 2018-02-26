# command to generate certificate 
../../../mbedtls_cert_write/programs/x509/cert_write.exe subject_key=remote0COM18 issuer_key=remote0COM18 issuer_name=CN=Cert,O=mbed TLS,C=UK authority_identifier=0

# command to run server
../../../mbedtls_cert_write/programs/ssl/ssl_server2.exe server_addr=10.2.203.146 debug_level=4 auth_mode=required crt_file=/c/server/e112111.emea.arm.com_prime256v1.pem key_file=/c/server/e112111.emea.arm.com_prime256v1_priv.pem ca_file=cert.crt  force_ciphersuite=TLS-ECDHE-ECDSA-WITH-AES-128-GCM-SHA256
