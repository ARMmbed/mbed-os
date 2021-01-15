"""
RSA Key management
"""

from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.primitives.asymmetric import rsa
from cryptography.hazmat.primitives.asymmetric.padding import PSS, MGF1
from cryptography.hazmat.primitives.hashes import SHA256

from .general import KeyClass


# Sizes that bootutil will recognize
RSA_KEY_SIZES = [2048, 3072]


class RSAUsageError(Exception):
    pass


class RSAPublic(KeyClass):
    """The public key can only do a few operations"""
    def __init__(self, key):
        self.key = key

    def key_size(self):
        return self.key.key_size

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

    def get_private_bytes(self, minimal):
        self._unsupported('get_private_bytes')

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
        return "PKCS1_PSS_RSA{}_SHA256".format(self.key_size())

    def sig_tlv(self):
        return"RSA{}".format(self.key_size())

    def sig_len(self):
        return self.key_size() / 8

    def verify(self, signature, payload):
        k = self.key
        if isinstance(self.key, rsa.RSAPrivateKey):
            k = self.key.public_key()
        return k.verify(signature=signature, data=payload,
                        padding=PSS(mgf=MGF1(SHA256()), salt_length=32),
                        algorithm=SHA256())


class RSA(RSAPublic):
    """
    Wrapper around an RSA key, with imgtool support.
    """

    def __init__(self, key):
        """The key should be a private key from cryptography"""
        self.key = key

    @staticmethod
    def generate(key_size=2048):
        if key_size not in RSA_KEY_SIZES:
            raise RSAUsageError("Key size {} is not supported by MCUboot"
                                .format(key_size))
        pk = rsa.generate_private_key(
                public_exponent=65537,
                key_size=key_size,
                backend=default_backend())
        return RSA(pk)

    def _get_public(self):
        return self.key.public_key()

    def _build_minimal_rsa_privkey(self, der):
        '''
        Builds a new DER that only includes N/E/D/P/Q RSA parameters;
        standard DER private bytes provided by OpenSSL also includes
        CRT params (DP/DQ/QP) which can be removed.
        '''
        OFFSET_N = 7  # N is always located at this offset
        b = bytearray(der)
        off = OFFSET_N
        if b[off + 1] != 0x82:
            raise RSAUsageError("Error parsing N while minimizing")
        len_N = (b[off + 2] << 8) + b[off + 3] + 4
        off += len_N
        if b[off + 1] != 0x03:
            raise RSAUsageError("Error parsing E while minimizing")
        len_E = b[off + 2] + 4
        off += len_E
        if b[off + 1] != 0x82:
            raise RSAUsageError("Error parsing D while minimizing")
        len_D = (b[off + 2] << 8) + b[off + 3] + 4
        off += len_D
        if b[off + 1] != 0x81:
            raise RSAUsageError("Error parsing P while minimizing")
        len_P = b[off + 2] + 3
        off += len_P
        if b[off + 1] != 0x81:
            raise RSAUsageError("Error parsing Q while minimizing")
        len_Q = b[off + 2] + 3
        off += len_Q
        # adjust DER size for removed elements
        b[2] = (off - 4) >> 8
        b[3] = (off - 4) & 0xff
        return b[:off]

    def get_private_bytes(self, minimal):
        priv = self.key.private_bytes(
                encoding=serialization.Encoding.DER,
                format=serialization.PrivateFormat.TraditionalOpenSSL,
                encryption_algorithm=serialization.NoEncryption())
        if minimal:
            priv = self._build_minimal_rsa_privkey(priv)
        return priv

    def export_private(self, path, passwd=None):
        """Write the private key to the given file, protecting it with the
        optional password."""
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
