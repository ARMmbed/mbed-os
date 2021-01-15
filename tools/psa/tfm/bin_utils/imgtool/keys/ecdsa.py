"""
ECDSA key management
"""

from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.primitives.asymmetric import ec
from cryptography.hazmat.primitives.hashes import SHA256

from .general import KeyClass

class ECDSAUsageError(Exception):
    pass

class ECDSA256P1Public(KeyClass):
    def __init__(self, key):
        self.key = key

    def shortname(self):
        return "ecdsa"

    def _unsupported(self, name):
        raise ECDSAUsageError("Operation {} requires private key".format(name))

    def _get_public(self):
        return self.key

    def get_public_bytes(self):
        # The key is embedded into MBUboot in "SubjectPublicKeyInfo" format
        return self._get_public().public_bytes(
                encoding=serialization.Encoding.DER,
                format=serialization.PublicFormat.SubjectPublicKeyInfo)

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
        return "ECDSA256_SHA256"

    def sig_tlv(self):
        return "ECDSA256"

    def sig_len(self):
        # Early versions of MCUboot (< v1.5.0) required ECDSA
        # signatures to be padded to 72 bytes.  Because the DER
        # encoding is done with signed integers, the size of the
        # signature will vary depending on whether the high bit is set
        # in each value.  This padding was done in a
        # not-easily-reversible way (by just adding zeros).
        #
        # The signing code no longer requires this padding, and newer
        # versions of MCUboot don't require it.  But, continue to
        # return the total length so that the padding can be done if
        # requested.
        return 72

    def verify(self, signature, payload):
        # strip possible paddings added during sign
        signature = signature[:signature[1] + 2]
        k = self.key
        if isinstance(self.key, ec.EllipticCurvePrivateKey):
            k = self.key.public_key()
        return k.verify(signature=signature, data=payload,
                        signature_algorithm=ec.ECDSA(SHA256()))


class ECDSA256P1(ECDSA256P1Public):
    """
    Wrapper around an ECDSA private key.
    """

    def __init__(self, key):
        """key should be an instance of EllipticCurvePrivateKey"""
        self.key = key
        self.pad_sig = False

    @staticmethod
    def generate():
        pk = ec.generate_private_key(
                ec.SECP256R1(),
                backend=default_backend())
        return ECDSA256P1(pk)

    def _get_public(self):
        return self.key.public_key()

    def _build_minimal_ecdsa_privkey(self, der):
        '''
        Builds a new DER that only includes the EC private key, removing the
        public key that is added as an "optional" BITSTRING.
        '''
        offset_PUB = 68
        EXCEPTION_TEXT = "Error parsing ecdsa key. Please submit an issue!"
        if der[offset_PUB] != 0xa1:
            raise ECDSAUsageError(EXCEPTION_TEXT)
        len_PUB = der[offset_PUB + 1]
        b = bytearray(der[:-offset_PUB])
        offset_SEQ = 29
        if b[offset_SEQ] != 0x30:
            raise ECDSAUsageError(EXCEPTION_TEXT)
        b[offset_SEQ + 1] -= len_PUB
        offset_OCT_STR = 27
        if b[offset_OCT_STR] != 0x04:
            raise ECDSAUsageError(EXCEPTION_TEXT)
        b[offset_OCT_STR + 1] -= len_PUB
        if b[0] != 0x30 or b[1] != 0x81:
            raise ECDSAUsageError(EXCEPTION_TEXT)
        b[2] -= len_PUB
        return b

    def get_private_bytes(self, minimal):
        priv = self.key.private_bytes(
                encoding=serialization.Encoding.DER,
                format=serialization.PrivateFormat.PKCS8,
                encryption_algorithm=serialization.NoEncryption())
        if minimal:
            priv = self._build_minimal_ecdsa_privkey(priv)
        return priv

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

    def raw_sign(self, payload):
        """Return the actual signature"""
        return self.key.sign(
                data=payload,
                signature_algorithm=ec.ECDSA(SHA256()))

    def sign(self, payload):
        sig = self.raw_sign(payload)
        if self.pad_sig:
            # To make fixed length, pad with one or two zeros.
            sig += b'\000' * (self.sig_len() - len(sig))
            return sig
        else:
            return sig
