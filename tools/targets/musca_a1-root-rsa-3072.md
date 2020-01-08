# Musca-A1 RSA keypair

A default RSA key pair is given to the Musca-A1 target.

Public key was pre-compiled to `targets/TARGET_ARM_SSG/TARGET_MUSCA_A1/TARGET_MUSCA_A1_NS/prebuilt/mcuboot.bin` and private key is in `musca_a1-root-rsa-3072.pem`.

DO NOT use them in production code, they are exclusively for testing!

Private key must be stored in a safe place outside of the repository.

`tools/psa/tfm/bin_utils/imgtool.py` can be used to generate new key pairs.
