# mbed SDK
# SPDX-License-Identifier: Apache-2.0
# Copyright (c) 2011-2013 ARM Limited
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
Utilities for working with region lists.
"""

import hashlib
import struct
import zlib
from time import time
from os.path import splitext, exists, dirname
from os import makedirs
from .config import Config
from .utils import (
    ToolException,
    intelhex_offset,
    integer
)
from intelhex import IntelHex

UPDATE_WHITELIST = (
    "application"
)


def _printihex(ihex):
    import pprint
    pprint.PrettyPrinter().pprint(ihex.todict())


def _real_region_size(region):
    try:
        part = intelhex_offset(region.filename, offset=region.start)
        return (part.maxaddr() - part.minaddr()) + 1
    except AttributeError:
        return region.size


def _fill_header(region_list, current_region):
    """Fill an application header region

    This is done it three steps:
     * Fill the whole region with zeros
     * Fill const, timestamp and size entries with their data
     * Fill the digests using this header as the header region
    """
    region_dict = {r.name: r for r in region_list}
    header = IntelHex()
    header.puts(current_region.start, b'\x00' * current_region.size)
    start = current_region.start
    for member in current_region.filename:
        _, type, subtype, data = member
        if type == "const":
            fmt = {
                "8le": ">B", "16le": "<H", "32le": "<L", "64le": "<Q",
                "8be": "<B", "16be": ">H", "32be": ">L", "64be": ">Q"
            }[subtype]
            header.puts(start, struct.pack(fmt, integer(data, 0)))
        elif type == "timestamp":
            fmt = {"32le": "<L", "64le": "<Q",
                   "32be": ">L", "64be": ">Q"}[subtype]
            header.puts(start, struct.pack(fmt, int(time())))
        elif type == "size":
            fmt = {"32le": "<L", "64le": "<Q",
                   "32be": ">L", "64be": ">Q"}[subtype]
            size = sum(_real_region_size(region_dict[r]) for r in data)
            header.puts(start, struct.pack(fmt, size))
        elif type == "digest":
            if data == "header":
                ih = header[:start]
            else:
                ih = intelhex_offset(
                    region_dict[data].filename,
                    offset=region_dict[data].start
                )
            if subtype.startswith("CRCITT32"):
                fmt = {"CRCITT32be": ">L", "CRCITT32le": "<L"}[subtype]
                crc_val = zlib.crc32(ih.tobinarray()) & 0xffffffff
                header.puts(start, struct.pack(fmt, crc_val))
            elif subtype.startswith("SHA"):
                if subtype == "SHA256":
                    hash = hashlib.sha256()
                elif subtype == "SHA512":
                    hash = hashlib.sha512()
                hash.update(ih.tobinarray())
                header.puts(start, hash.digest())
        start += Config.header_member_size(member)
    return header


def merge_region_list(
        region_list,
        destination,
        notify,
        padding=b'\xFF',
        restrict_size=None
):
    """Merge the region_list into a single image

    Positional Arguments:
    region_list - list of regions, which should contain filenames
    destination - file name to write all regions to
    padding - bytes to fill gaps with
    restrict_size - check to ensure a region fits within the given size
    """
    merged = IntelHex()
    _, format = splitext(destination)
    notify.info("Merging Regions")
    # Merged file list: Keep track of binary/hex files that we have already
    # merged. e.g In some cases, bootloader may be split into multiple parts,
    # but all internally referring to the same bootloader file.
    merged_list = []

    for region in region_list:
        if region.active and not region.filename:
            raise ToolException(
                "Active region has no contents: No file found."
            )
        if isinstance(region.filename, list):
            header_basename, _ = splitext(destination)
            header_filename = header_basename + "_header.hex"
            _fill_header(region_list, region).tofile(
                header_filename, format='hex'
            )
            region = region._replace(filename=header_filename)
        if region.filename and (region.filename not in merged_list):
            notify.info("  Filling region %s with %s" % (
                region.name, region.filename
            ))
            part = intelhex_offset(region.filename, offset=region.start)
            part.start_addr = None
            # Normally, we assume that part.maxddr() can be beyond
            # end of rom. If the size is restricted with config, don't
            # allow this.
            if restrict_size is not None:
                part_size = (part.maxaddr() - part.minaddr()) + 1
                if part_size > region.size:
                    raise ToolException(
                        "Contents of region %s does not fit" % region.name
                    )
            merged_list.append(region.filename)
            merged.merge(part)
        elif region.filename in merged_list:
            notify.info(
                "  Skipping %s as it is merged previously" % (region.name)
            )

    # Hex file can have gaps, so no padding needed. While other formats may
    # need padding. Iterate through segments and pad the gaps.
    if format != ".hex":
        # begin patching from the end of the first segment
        _, begin = merged.segments()[0]
        for start, stop in merged.segments()[1:]:
            pad_size = start - begin
            merged.puts(begin, padding * pad_size)
            begin = stop + 1

    if not exists(dirname(destination)):
        makedirs(dirname(destination))
    notify.info("Space used after regions merged: 0x%x" %
                (merged.maxaddr() - merged.minaddr() + 1))
    merged.tofile(destination, format=format.strip("."))
