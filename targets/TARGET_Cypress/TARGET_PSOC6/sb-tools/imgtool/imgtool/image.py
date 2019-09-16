# Copyright 2018 Nordic Semiconductor ASA
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
Image signing and management.
"""

from . import version as versmod
from intelhex import IntelHex
import hashlib
import struct
import os.path
import os

IMAGE_MAGIC = 0x96f3b83d
IMAGE_HEADER_SIZE = 32
BIN_EXT = "bin"
INTEL_HEX_EXT = "hex"
DEFAULT_MAX_SECTORS = 128

# Image header flags.
IMAGE_F = {
        'PIC':                   0x0000001,
        'NON_BOOTABLE':          0x0000010, }

TLV_VALUES = {
        'KEYHASH': 0x01,
        'SHA256': 0x10,
        'RSA2048': 0x20,
        'ECDSA224': 0x21,
        'ECDSA256': 0x22, }

TLV_INFO_SIZE = 4
TLV_INFO_MAGIC = 0x6907

boot_magic = bytes([
    0x77, 0xc2, 0x95, 0xf3,
    0x60, 0xd2, 0xef, 0x7f,
    0x35, 0x52, 0x50, 0x0f,
    0x2c, 0xb6, 0x79, 0x80, ])

class TLV():
    def __init__(self):
        self.buf = bytearray()

    def add(self, kind, payload):
        """Add a TLV record.  Kind should be a string found in TLV_VALUES above."""
        buf = struct.pack('<BBH', TLV_VALUES[kind], 0, len(payload))
        self.buf += buf
        self.buf += payload

    def get(self):
        header = struct.pack('<HH', TLV_INFO_MAGIC, TLV_INFO_SIZE + len(self.buf))
        return header + bytes(self.buf)

class Image():
    @classmethod
    def load(cls, path, pad_header=False, **kwargs):
        """Load an image from a given file"""
        ext = os.path.splitext(path)[1][1:].lower()
        if ext == INTEL_HEX_EXT:
            cls = HexImage
        else:
            cls = BinImage

        obj = cls(**kwargs)
        obj.payload, obj.base_addr = obj.load(path)

        # Add the image header if needed.
        if pad_header and obj.header_size > 0:
            if obj.base_addr:
                # Adjust base_addr for new header
                obj.base_addr -= obj.header_size
            obj.payload = (b'\000' * obj.header_size) + obj.payload

        obj.check()
        return obj

    def __init__(self, version=None, header_size=IMAGE_HEADER_SIZE, pad=0,
                 align=1, slot_size=0, max_sectors=DEFAULT_MAX_SECTORS,
                 overwrite_only=False, aes_header_data=None, image_id=1, rollback_counter=0):
        self.version = version or versmod.decode_version("0")
        self.header_size = header_size or IMAGE_HEADER_SIZE
        self.pad = pad
        self.align = align
        self.slot_size = slot_size
        self.max_sectors = max_sectors
        self.overwrite_only = overwrite_only
        self.aes_header_data = aes_header_data
        self.image_id = image_id
        self.rollback_counter = rollback_counter

    def __repr__(self):
        return "<Image version={}, header_size={}, base_addr={}, \
                align={}, slot_size={}, max_sectors={}, overwrite_only={}, \
                format={}, payloadlen=0x{:x}>".format(
                    self.version,
                    self.header_size,
                    self.base_addr if self.base_addr is not None else "N/A",
                    self.align,
                    self.slot_size,
                    self.max_sectors,
                    self.overwrite_only,
                    self.__class__.__name__,
                    len(self.payload))

    def check(self):
        """Perform some sanity checking of the image."""
        # If there is a header requested, make sure that the image
        # starts with all zeros.
        if self.header_size > 0:
            if any(v != 0 for v in self.payload[0:self.header_size]):
                raise Exception("Padding requested, but image does not start with zeros")
        if self.slot_size > 0:
            tsize = self._trailer_size(self.align, self.max_sectors,
                                       self.overwrite_only)
            padding = self.slot_size - (len(self.payload) + tsize)
            if padding < 0:
                msg = "Image size (0x{:x}) + trailer (0x{:x}) exceeds requested size 0x{:x}".format(
                        len(self.payload), tsize, self.slot_size)
                raise Exception(msg)

    def sign(self, key, add_padding = True):
	
        DECRYPT_BLOCK_SIZE = 256	#future AES decription buffer size, is used for align FW image and trailer size

        if add_padding:
            pl_size = len(self.payload)
            pad_len = (DECRYPT_BLOCK_SIZE - pl_size % DECRYPT_BLOCK_SIZE) % DECRYPT_BLOCK_SIZE
            #self.payload += bytearray(os.urandom(pad_len))
            self.payload += bytearray(pad_len)
		
        self.add_header(key)
        if (self.aes_header_data is not None):
            self.add_AES_header()

        tlv = TLV()

        # Note that ecdsa wants to do the hashing itself, which means
        # we get to hash it twice.
        sha = hashlib.sha256()
        sha.update(self.payload)
        digest = sha.digest()

        tlv.add('SHA256', digest)

        if key is not None:
            pub = key.get_public_bytes()
            sha = hashlib.sha256()
            sha.update(pub)
            pubbytes = sha.digest()
            tlv.add('KEYHASH', pubbytes)

            sig = key.sign(bytes(self.payload))
            tlv.add(key.sig_tlv(), sig)

        trailer = tlv.get()
        self.payload += trailer

        if add_padding:		
            trailer_size = len(trailer)
            tr_rem_len = (DECRYPT_BLOCK_SIZE - trailer_size % DECRYPT_BLOCK_SIZE ) % DECRYPT_BLOCK_SIZE
            #self.payload += bytearray(os.urandom(tr_rem_len))
            self.payload += bytearray(tr_rem_len)

    def add_header(self, key):
        """Install the image header.

        The key is needed to know the type of signature, and
        approximate the size of the signature."""

        flags = 0

        fmt = ('<' +
            # type ImageHdr struct {
            'I' +   # Magic uint32
            'I' +   # LoadAddr uint32
            'H' +   # HdrSz uint16
            'B' +   # Image ID uint8
            'B' +   # Rollback monotonic counter value uint8
            'I' +   # ImgSz uint32
            'I' +   # Flags uint32
            'BBHI' + # Vers  ImageVersion
            'I'     # Pad2  uint32
            ) # }
        assert struct.calcsize(fmt) == IMAGE_HEADER_SIZE
        header = struct.pack(fmt,
                IMAGE_MAGIC,
                0, # LoadAddr
                self.header_size,
                self.image_id,
                self.rollback_counter,
                len(self.payload) - self.header_size, # ImageSz
                flags, # Flags
                self.version.major,
                self.version.minor or 0,
                self.version.revision or 0,
                self.version.build or 0,
                0) # Pad2
        self.payload = bytearray(self.payload)
        self.payload[:len(header)] = header
		
    def add_AES_header(self):
        """Install AES header just after main image header
		
		The header contains:
            AES Key, IV (encrypted with AES CBC)
    		salt - random sequence for ECDH KDF
            info - information for ECDH KDF
		"""

        aes_header_bytes = self.aes_header_data #str.encode(self.aes_header_data,'utf-8')
        headerAES = struct.pack( "@%ds" % (len(aes_header_bytes)), aes_header_bytes )		
        self.payload[IMAGE_HEADER_SIZE:IMAGE_HEADER_SIZE + len(headerAES)] = headerAES

	
    def _trailer_size(self, write_size, max_sectors, overwrite_only):
        # NOTE: should already be checked by the argument parser
        if overwrite_only:
            return 8 * 2 + 16
        else:
            if write_size not in set([1, 2, 4, 8]):
                raise Exception("Invalid alignment: {}".format(write_size))
            m = DEFAULT_MAX_SECTORS if max_sectors is None else max_sectors
            return m * 3 * write_size + 8 * 2 + 16

    def pad_to(self, size):
        """Pad the image to the given size, with the given flash alignment."""
        tsize = self._trailer_size(self.align, self.max_sectors,
                                   self.overwrite_only)
        padding = size - (len(self.payload) + tsize)
        pbytes  = b'\xff' * padding
        pbytes += b'\xff' * (tsize - len(boot_magic))
        pbytes += boot_magic
        self.payload += pbytes


class HexImage(Image):

    def load(self, path):
        ih = IntelHex(path)
        return ih.tobinarray(), ih.minaddr()

    def save(self, path):
        h = IntelHex()
        h.frombytes(bytes = self.payload, offset = self.base_addr)
        h.tofile(path, 'hex')

class BinImage(Image):

    def load(self, path):
        with open(path, 'rb') as f:
            return f.read(), None

    def save(self, path):
        with open(path, 'wb') as f:
            f.write(self.payload)
