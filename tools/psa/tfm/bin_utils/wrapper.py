#! /usr/bin/env python3
#
# -----------------------------------------------------------------------------
# Copyright (c) 2020, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
# -----------------------------------------------------------------------------

import re
import os
import sys
import click

# Add the cwd to the path so that if there is a version of imgtool in there then
# it gets used over the system imgtool. Used so that imgtool from upstream
# mcuboot is preferred over system imgtool
cwd = os.getcwd()
sys.path = [cwd] + sys.path
import imgtool
import imgtool.main

parser_path = os.path.abspath(os.path.join(os.path.dirname(__file__), '../'))
sys.path.append(parser_path)
import macro_parser

sign_bin_size_re = re.compile(r"^\s*RE_SIGN_BIN_SIZE\s*=\s*(.*)")
load_addr_re = re.compile(r"^\s*RE_IMAGE_LOAD_ADDRESS\s*=\s*(.*)")

#This works around Python 2 and Python 3 handling character encodings
#differently. More information about this issue at
#https://click.palletsprojects.com/en/5.x/python3
os.environ['LC_ALL'] = 'C.UTF-8'
os.environ['LANG'] = 'C.UTF-8'

@click.argument('outfile')
@click.argument('infile')
@click.option('-R', '--erased-val', type=click.Choice(['0', '0xff']),
              required=False, help='The value that is read back from erased '
                                   'flash.')
@click.option('-x', '--hex-addr', type=imgtool.main.BasedIntParamType(),
              required=False, help='Adjust address in hex output file.')
@click.option('--save-enctlv', default=False, is_flag=True,
              help='When upgrading, save encrypted key TLVs instead of plain '
                   'keys. Enable when BOOT_SWAP_SAVE_ENCTLV config option '
                   'was set.')
@click.option('-E', '--encrypt', metavar='filename',
              help='Encrypt image using the provided public key')
@click.option('-e', '--endian', type=click.Choice(['little', 'big']),
              default='little', help="Select little or big endian")
@click.option('--overwrite-only', default=False, is_flag=True,
              help='Use overwrite-only instead of swap upgrades')
@click.option('-M', '--max-sectors', type=int,
              help='When padding allow for this amount of sectors (defaults '
                   'to 128)')
@click.option('--confirm', default=False, is_flag=True,
              help='When padding the image, mark it as confirmed')
@click.option('--pad', default=False, is_flag=True,
              help='Pad image to the size determined by --layout, adding '
                   'trailer magic')
@click.option('-l', '--layout', help='The file containing the macros of the '
                                     'slot sizes')
@click.option('--pad-header', default=False, is_flag=True,
              help='Adds --erased-val (defaults to 0xff) --header-size times '
                   'at the beginning of the image')
@click.option('-H', '--header-size',
              callback=imgtool.main.validate_header_size,
              type=imgtool.main.BasedIntParamType(), required=True)
@click.option('-d', '--dependencies', callback=imgtool.main.get_dependencies,
              required=False, help='''Add dependence on another image, format:
                                   "(<image_ID>,<image_version>), ... "''')
@click.option('-s', '--security-counter',
              callback=imgtool.main.validate_security_counter,
              help='Specify the value of security counter. Use the `auto` '
              'keyword to automatically generate it from the image version.')
@click.option('-v', '--version', callback=imgtool.main.validate_version,
              required=True)
@click.option('--align', type=click.Choice(['1', '2', '4', '8']),
              required=True)
@click.option('--public-key-format', type=click.Choice(['hash', 'full']),
              default='hash', help='In what format to add the public key to '
              'the image manifest: full key or hash of the key.')
@click.option('-k', '--key', metavar='filename')
@click.command(help='''Create a signed or unsigned image\n
               INFILE and OUTFILE are parsed as Intel HEX if the params have
               .hex extension, otherwise binary format is used''')
def wrap(key, align, version, header_size, pad_header, layout, pad, confirm,
         max_sectors, overwrite_only, endian, encrypt, infile, outfile,
         dependencies, hex_addr, erased_val, save_enctlv, public_key_format,
         security_counter):

    slot_size = macro_parser.evaluate_macro(layout, sign_bin_size_re, 0, 1)
    load_addr = macro_parser.evaluate_macro(layout, load_addr_re, 0, 1)

    if "_s" in layout:
        boot_record = "SPE"
    elif "_ns" in layout:
        boot_record = "NSPE"
    else:
        boot_record = "NSPE_SPE"

    img = imgtool.image.Image(version=imgtool.version.decode_version(version),
                              header_size=header_size, pad_header=pad_header,
                              pad=pad, confirm=confirm, align=int(align),
                              slot_size=slot_size, max_sectors=max_sectors,
                              overwrite_only=overwrite_only, endian=endian,
                              load_addr=load_addr, erased_val=erased_val,
                              save_enctlv=save_enctlv,
                              security_counter=security_counter)

    img.load(infile)
    key = imgtool.main.load_key(key) if key else None
    enckey = imgtool.main.load_key(encrypt) if encrypt else None
    if enckey and key:
        if (isinstance(key, imgtool.keys.RSA) and
           not isinstance(enckey, imgtool.keys.RSAPublic)):
            # FIXME
            raise click.UsageError("Signing and encryption must use the same "
                                   "type of key")

    img.create(key, public_key_format, enckey, dependencies, boot_record)
    img.save(outfile, hex_addr)


if __name__ == '__main__':
    wrap()
