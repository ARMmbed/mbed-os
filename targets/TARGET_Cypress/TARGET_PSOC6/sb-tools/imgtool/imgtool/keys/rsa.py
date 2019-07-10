"""
RSA Key management
"""

from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.primitives.asymmetric import rsa
from cryptography.hazmat.primitives.asymmetric.padding import PSS, MGF1
from cryptography.hazmat.primitives.hashes import SHA256

from .general import KeyClass

class RSAUsageError(Exception):
    pass

class RSA2048Public(KeyClass):
    """The public key can only do a few operations"""
    def __init__(self, key):
        self.key = key

    def shortname(self):
        return "rsa"

    def _unsupported(self, name):
        raise RSAUsageError("Operation {} requires private key".format(name))

    def _get_public(self):
        return self.key

    def get_public_bytes(self):
        # The key embedded into MCUboot is in PKCS1 format.
        return self._get_public().public_bytes(
                encoding=serialization.Encoding.DER,
                format=serialization.PublicFormat.PKCS1)

    def export_private(self, path, passwd=None):
        self._unsupported('export_private')

    def export_public(self, path):
        """Write the public key to the given file."""
        pem = self._get_public().public_bytes(
                encoding=serialization.Encoding.PEM,
                format=serialization.PublicFormat.SubjectPublicKeyInfo)
        with open(path, 'wb') as f:
            f.write(pem)

    def sig_type(self):
        return "PKCS1_PSS_RSA2048_SHA256"

    def sig_tlv(self):
        return "RSA2048"

    def sig_len(self):
        return 256

class RSA2048(RSA2048Public):
    """
    Wrapper around an 2048-bit RSA key, with imgtool support.
    """

    def __init__(self, key):
        """The key should be a private key from cryptography"""
        self.key = key

    @staticmethod
    def generate():
        pk = rsa.generate_private_key(
                public_exponent=65537,
                key_size=2048,
                backend=default_backend())
        return RSA2048(pk)

    def _get_public(self):
        return self.key.public_key()

    def export_private(self, path, passwd=None):
        """Write the private key to the given file, protecting it with the optional password."""
        if passwd is None:
            enc = serialization.NoEncryption()
        else:
            enc = serialization.BestAvailableEncryption(passwd)
        pem = self.key.private_bytes(
                encoding=serialization.Encoding.PEM,
                format=serialization.PrivateFormat.PKCS8,
                encryption_algorithm=enc)
        with open(path, 'wb') as f:
            f.write(pem)

    def sign(self, payload):
        # The verification code only allows the salt length to be the
        # same as the hash length, 32.
        return self.key.sign(
                data=payload,
                padding=PSS(mgf=MGF1(SHA256()), salt_length=32),
                algorithm=SHA256())
