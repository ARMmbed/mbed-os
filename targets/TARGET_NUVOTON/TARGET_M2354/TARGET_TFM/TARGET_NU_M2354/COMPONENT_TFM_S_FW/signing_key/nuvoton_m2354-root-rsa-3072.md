# Nuvoton M2354 RSA keypair

A default RSA key pair is given to the Nuvoton M2354 target.

Public key was pre-compiled to `targets/TARGET_NUVOTON/TARGET_M2354/TARGET_TFM/TARGET_NU_M2354/COMPONENT_TFM_S_FW/bl2.bin` and private key is in `nuvoton_m2354-root-rsa-3072.pem` for Secure image and Non-Secure image together.

DO NOT use them in production code, they are exclusively for testing!

Private key must be stored in a safe place outside of the repository.

`tools/psa/tfm/bin_utils/imgtool.py` can be used to generate new key pairs.
