# Musca-B1 RSA keypair

A default RSA key pair is given to the Musca-B1 target.

Public keys were pre-compiled to `targets/TARGET_ARM_SSG/TARGET_MUSCA_B1/bl2.bin` and private key is in `musca_b1-root-rsa-3072.pem` for Secure image and `musca_b1-root-rsa-3072_1.pem` for Non-Secure image.

DO NOT use them in production code, they are exclusively for testing!

Private key must be stored in a safe place outside of the repository.

`tools/psa/tfm/bin_utils/imgtool.py` can be used to generate new key pairs.
