# Copyright 2017 Linaro Limited
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

"""
Cryptographic key management for imgtool.
"""

from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.primitives.asymmetric.rsa import RSAPrivateKey, RSAPublicKey
from cryptography.hazmat.primitives.asymmetric.ec import EllipticCurvePrivateKey, EllipticCurvePublicKey

from .rsa import RSA2048, RSA2048Public, RSAUsageError
from .ecdsa import ECDSA256P1, ECDSA256P1Public, ECDSAUsageError

class PasswordRequired(Exception):
    """Raised to indicate that the key is password protected, but a
    password was not specified."""
    pass

def load(path, passwd=None):
    """Try loading a key from the given path.  Returns None if the password wasn't specified."""
    with open(path, 'rb') as f:
        raw_pem = f.read()
    try:
        pk = serialization.load_pem_private_key(
                raw_pem,
                password=passwd,
                backend=default_backend())
    # Unfortunately, the crypto library raises unhelpful exceptions,
    # so we have to look at the text.
    except TypeError as e:
        msg = str(e)
        if "private key is encrypted" in msg:
            return None
        raise e
    except ValueError:
        # This seems to happen if the key is a public key, let's try
        # loading it as a public key.
        pk = serialization.load_pem_public_key(
                raw_pem,
                backend=default_backend())

    if isinstance(pk, RSAPrivateKey):
        if pk.key_size != 2048:
            raise Exception("Unsupported RSA key size: " + pk.key_size)
        return RSA2048(pk)
    elif isinstance(pk, RSAPublicKey):
        if pk.key_size != 2048:
            raise Exception("Unsupported RSA key size: " + pk.key_size)
        return RSA2048Public(pk)
    elif isinstance(pk, EllipticCurvePrivateKey):
        if pk.curve.name != 'secp256r1':
            raise Exception("Unsupported EC curve: " + pk.curve.name)
        if pk.key_size != 256:
            raise Exception("Unsupported EC size: " + pk.key_size)
        return ECDSA256P1(pk)
    elif isinstance(pk, EllipticCurvePublicKey):
        if pk.curve.name != 'secp256r1':
            raise Exception("Unsupported EC curve: " + pk.curve.name)
        if pk.key_size != 256:
            raise Exception("Unsupported EC size: " + pk.key_size)
        return ECDSA256P1Public(pk)
    else:
        raise Exception("Unknown key type: " + str(type(pk)))
