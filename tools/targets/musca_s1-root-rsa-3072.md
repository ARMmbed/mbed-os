# Musca-S1 RSA keypair

A default RSA key pair is given to the Musca-S1 target.

Public keys were pre-compiled to `targets/TARGET_ARM_SSG/TARGET_MUSCA_S1/bl2.bin` and private key is in `musca_s1-root-rsa-3072.pem` for Secure image and `musca_s1-root-rsa-3072_1.pem` for Non-Secure image.

DO NOT use them in production code, they are exclusively for testing!

Private key must be stored in a safe place outside of the repository.

`tools/psa/tfm/bin_utils/imgtool.py` can be used to generate new key pairs.
