#!/usr/bin/env python

"""
mbed SDK
Copyright (c) 2017 ARM Limited

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""

from os import path
import hashlib, zlib, struct

'''
define FIRMWARE_HEADER_MAGIC   0x5a51b3d4UL
define FIRMWARE_HEADER_VERSION 1
typedef struct FirmwareHeader {
    uint32_t magic;                         /** Metadata-header specific magic code */
    uint32_t version;                       /** Revision number for this generic metadata header. */
    uint32_t checksum;                      /** A checksum of this header. This field should be considered to be zeroed out for
                                             *  the sake of computing the checksum. */
    uint32_t totalSize;                     /** Total space (in bytes) occupied by the firmware BLOB, including headers and any padding. */
    uint64_t firmwareVersion;               /** Version number for the accompanying firmware. Larger numbers imply more preferred (recent)
                                             *  versions. This defines the selection order when multiple versions are available. */
    uint8_t  firmwareSHA256[SIZEOF_SHA256]; /** A SHA-2 using a block-size of 256-bits of the firmware, including any firmware-padding. */
} FirmwareHeader_t;
'''

# define defaults to go into the metadata header
SIZEOF_SHA256 = 256/8
FIRMWARE_HEADER_MAGIC = 0x5a51b3d4
FIRMWARE_HEADER_VERSION = 1
firmwareVersion = 0
FirmwareHeader_t = "4IQ{}s".format(SIZEOF_SHA256) # 4*unsigned int, 1*unsigned long long, 1*bytes
FIRMWARE_HEADER_SIZE = 56

def create_header(app_blob):
    # calculate the hash of the application
    firmwareSHA256 = hashlib.sha256(app_blob)

    # calculate the total size which is defined as the application size + metadata header
    totalSize = len(app_blob)

    # set checksum to zero and calculate the checksum of the header
    checksum = 0
    FirmwareHeader = struct.pack(FirmwareHeader_t,
                                    FIRMWARE_HEADER_MAGIC,
                                    FIRMWARE_HEADER_VERSION,
                                    checksum,
                                    totalSize,
                                    firmwareVersion,
                                    firmwareSHA256.digest())
    checksum = zlib.crc32(FirmwareHeader) & 0xffffffff

    # Pack the data into a binary blob
    FirmwareHeader = struct.pack(FirmwareHeader_t,
                                    FIRMWARE_HEADER_MAGIC,
                                    FIRMWARE_HEADER_VERSION,
                                    checksum,
                                    totalSize,
                                    firmwareVersion,
                                    firmwareSHA256.digest())

    return FirmwareHeader


if __name__ == '__main__':
    import argparse

    parser = argparse.ArgumentParser(
        description='Combine bootloader with application adding metadata header.')

    def offset_arg(s):
        if s.startswith('0x'):
            return int(s, 16)
        else:
            return int(s)

    # specify arguments
    parser.add_argument('-a', '--app',           type=argparse.FileType('rb'), required=True,
                        help='path to application binary')
    parser.add_argument('-o', '--output',        type=argparse.FileType('wb'), required=True,
                        help='file to write the firmware metadata header to')

    # workaround for http://bugs.python.org/issue9694
    parser._optionals.title = "arguments"

    # get and validate arguments
    args = parser.parse_args()

    # read the contents of bootloader and application binaries into buffers
    app_blob = args.app.read()
    args.app.close()

    header_blob = create_header(app_blob)
    args.output.write(header_blob)

    # close output file
    output_fn = path.abspath(args.output.name)
    args.output.close()

    # print the output file path
    print 'Combined binary:', output_fn
