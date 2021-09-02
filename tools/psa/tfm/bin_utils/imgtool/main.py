#! /usr/bin/env python3
#
# Copyright 2017-2020 Linaro Limited
# Copyright 2019-2021 Arm Limited
#
# SPDX-License-Identifier: Apache-2.0
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

import re
import click
import getpass
import imgtool.keys as keys
import sys
from imgtool import image, imgtool_version
from imgtool.version import decode_version
from .keys import (
    RSAUsageError, ECDSAUsageError, Ed25519UsageError, X25519UsageError)

MIN_PYTHON_VERSION = (3, 6)
if sys.version_info < MIN_PYTHON_VERSION:
    sys.exit("Python %s.%s or newer is required by imgtool."
             % MIN_PYTHON_VERSION)


def gen_rsa2048(keyfile, passwd):
    keys.RSA.generate().export_private(path=keyfile, passwd=passwd)


def gen_rsa3072(keyfile, passwd):
    keys.RSA.generate(key_size=3072).export_private(path=keyfile,
                                                    passwd=passwd)


def gen_ecdsa_p256(keyfile, passwd):
    keys.ECDSA256P1.generate().export_private(keyfile, passwd=passwd)


def gen_ecdsa_p224(keyfile, passwd):
    print("TODO: p-224 not yet implemented")


def gen_ed25519(keyfile, passwd):
    keys.Ed25519.generate().export_private(path=keyfile, passwd=passwd)


def gen_x25519(keyfile, passwd):
    keys.X25519.generate().export_private(path=keyfile, passwd=passwd)


valid_langs = ['c', 'rust']
keygens = {
    'rsa-2048':   gen_rsa2048,
    'rsa-3072':   gen_rsa3072,
    'ecdsa-p256': gen_ecdsa_p256,
    'ecdsa-p224': gen_ecdsa_p224,
    'ed25519':    gen_ed25519,
    'x25519':     gen_x25519,
}


def load_key(keyfile):
    # TODO: better handling of invalid pass-phrase
    key = keys.load(keyfile)
    if key is not None:
        return key
    passwd = getpass.getpass("Enter key passphrase: ").encode('utf-8')
    return keys.load(keyfile, passwd)


def get_password():
    while True:
        passwd = getpass.getpass("Enter key passphrase: ")
        passwd2 = getpass.getpass("Reenter passphrase: ")
        if passwd == passwd2:
            break
        print("Passwords do not match, try again")

    # Password must be bytes, always use UTF-8 for consistent
    # encoding.
    return passwd.encode('utf-8')


@click.option('-p', '--password', is_flag=True,
              help='Prompt for password to protect key')
@click.option('-t', '--type', metavar='type', required=True,
              type=click.Choice(keygens.keys()), prompt=True,
              help='{}'.format('One of: {}'.format(', '.join(keygens.keys()))))
@click.option('-k', '--key', metavar='filename', required=True)
@click.command(help='Generate pub/private keypair')
def keygen(type, key, password):
    password = get_password() if password else None
    keygens[type](key, password)


@click.option('-l', '--lang', metavar='lang', default=valid_langs[0],
              type=click.Choice(valid_langs))
@click.option('-k', '--key', metavar='filename', required=True)
@click.command(help='Dump public key from keypair')
def getpub(key, lang):
    key = load_key(key)
    if key is None:
        print("Invalid passphrase")
    elif lang == 'c':
        key.emit_c_public()
    elif lang == 'rust':
        key.emit_rust_public()
    else:
        raise ValueError("BUG: should never get here!")


@click.option('--minimal', default=False, is_flag=True,
              help='Reduce the size of the dumped private key to include only '
                   'the minimum amount of data required to decrypt. This '
                   'might require changes to the build config. Check the docs!'
              )
@click.option('-k', '--key', metavar='filename', required=True)
@click.command(help='Dump private key from keypair')
def getpriv(key, minimal):
    key = load_key(key)
    if key is None:
        print("Invalid passphrase")
    try:
        key.emit_private(minimal)
    except (RSAUsageError, ECDSAUsageError, Ed25519UsageError,
            X25519UsageError) as e:
        raise click.UsageError(e)


@click.argument('imgfile')
@click.option('-k', '--key', metavar='filename')
@click.command(help="Check that signed image can be verified by given key")
def verify(key, imgfile):
    key = load_key(key) if key else None
    ret, version, digest = image.Image.verify(imgfile, key)
    if ret == image.VerifyResult.OK:
        print("Image was correctly validated")
        print("Image version: {}.{}.{}+{}".format(*version))
        print("Image digest: {}".format(digest.hex()))
        return
    elif ret == image.VerifyResult.INVALID_MAGIC:
        print("Invalid image magic; is this an MCUboot image?")
    elif ret == image.VerifyResult.INVALID_TLV_INFO_MAGIC:
        print("Invalid TLV info magic; is this an MCUboot image?")
    elif ret == image.VerifyResult.INVALID_HASH:
        print("Image has an invalid sha256 digest")
    elif ret == image.VerifyResult.INVALID_SIGNATURE:
        print("No signature found for the given key")
    else:
        print("Unknown return code: {}".format(ret))
    sys.exit(1)


def validate_version(ctx, param, value):
    try:
        decode_version(value)
        return value
    except ValueError as e:
        raise click.BadParameter("{}".format(e))


def validate_security_counter(ctx, param, value):
    if value is not None:
        if value.lower() == 'auto':
            return 'auto'
        else:
            try:
                return int(value, 0)
            except ValueError:
                raise click.BadParameter(
                    "{} is not a valid integer. Please use code literals "
                    "prefixed with 0b/0B, 0o/0O, or 0x/0X as necessary."
                    .format(value))


def validate_header_size(ctx, param, value):
    min_hdr_size = image.IMAGE_HEADER_SIZE
    if value < min_hdr_size:
        raise click.BadParameter(
            "Minimum value for -H/--header-size is {}".format(min_hdr_size))
    return value


def get_dependencies(ctx, param, value):
    if value is not None:
        versions = []
        images = re.findall(r"\((\d+)", value)
        if len(images) == 0:
            raise click.BadParameter(
                "Image dependency format is invalid: {}".format(value))
        raw_versions = re.findall(r",\s*([0-9.+]+)\)", value)
        if len(images) != len(raw_versions):
            raise click.BadParameter(
                '''There's a mismatch between the number of dependency images
                and versions in: {}'''.format(value))
        for raw_version in raw_versions:
            try:
                versions.append(decode_version(raw_version))
            except ValueError as e:
                raise click.BadParameter("{}".format(e))
        dependencies = dict()
        dependencies[image.DEP_IMAGES_KEY] = images
        dependencies[image.DEP_VERSIONS_KEY] = versions
        return dependencies


class BasedIntParamType(click.ParamType):
    name = 'integer'

    def convert(self, value, param, ctx):
        try:
            return int(value, 0)
        except ValueError:
            self.fail('%s is not a valid integer. Please use code literals '
                      'prefixed with 0b/0B, 0o/0O, or 0x/0X as necessary.'
                      % value, param, ctx)


@click.argument('outfile')
@click.argument('infile')
@click.option('--custom-tlv', required=False, nargs=2, default=[],
              multiple=True, metavar='[tag] [value]',
              help='Custom TLV that will be placed into protected area. '
                   'Add "0x" prefix if the value should be interpreted as an '
                   'integer, otherwise it will be interpreted as a string. '
                   'Specify the option multiple times to add multiple TLVs.')
@click.option('-R', '--erased-val', type=click.Choice(['0', '0xff']),
              required=False,
              help='The value that is read back from erased flash.')
@click.option('-x', '--hex-addr', type=BasedIntParamType(), required=False,
              help='Adjust address in hex output file.')
@click.option('-L', '--load-addr', type=BasedIntParamType(), required=False,
              help='Load address for image when it should run from RAM.')
@click.option('-F', '--rom-fixed', type=BasedIntParamType(), required=False,
              help='Set flash address the image is built for.')
@click.option('--save-enctlv', default=False, is_flag=True,
              help='When upgrading, save encrypted key TLVs instead of plain '
                   'keys. Enable when BOOT_SWAP_SAVE_ENCTLV config option '
                   'was set.')
@click.option('-E', '--encrypt', metavar='filename',
              help='Encrypt image using the provided public key. '
                   '(Not supported in direct-xip or ram-load mode.)')
@click.option('--encrypt-keylen', default='128',
              type=click.Choice(['128','256']),
              help='When encrypting the image using AES, select a 128 bit or '
                   '256 bit key len.')
@click.option('-e', '--endian', type=click.Choice(['little', 'big']),
              default='little', help="Select little or big endian")
@click.option('--overwrite-only', default=False, is_flag=True,
              help='Use overwrite-only instead of swap upgrades')
@click.option('--boot-record', metavar='sw_type', help='Create CBOR encoded '
              'boot record TLV. The sw_type represents the role of the '
              'software component (e.g. CoFM for coprocessor firmware). '
              '[max. 12 characters]')
@click.option('-M', '--max-sectors', type=int,
              help='When padding allow for this amount of sectors (defaults '
                   'to 128)')
@click.option('--confirm', default=False, is_flag=True,
              help='When padding the image, mark it as confirmed (implies '
                   '--pad)')
@click.option('--pad', default=False, is_flag=True,
              help='Pad image to --slot-size bytes, adding trailer magic')
@click.option('-S', '--slot-size', type=BasedIntParamType(), required=True,
              help='Size of the slot. If the slots have different sizes, use '
              'the size of the secondary slot.')
@click.option('--pad-header', default=False, is_flag=True,
              help='Add --header-size zeroed bytes at the beginning of the '
                   'image')
@click.option('-H', '--header-size', callback=validate_header_size,
              type=BasedIntParamType(), required=True)
@click.option('--pad-sig', default=False, is_flag=True,
              help='Add 0-2 bytes of padding to ECDSA signature '
                   '(for mcuboot <1.5)')
@click.option('-d', '--dependencies', callback=get_dependencies,
              required=False, help='''Add dependence on another image, format:
              "(<image_ID>,<image_version>), ... "''')
@click.option('-s', '--security-counter', callback=validate_security_counter,
              help='Specify the value of security counter. Use the `auto` '
              'keyword to automatically generate it from the image version.')
@click.option('-v', '--version', callback=validate_version,  required=True)
@click.option('--align', type=click.Choice(['1', '2', '4', '8']),
              required=True)
@click.option('--public-key-format', type=click.Choice(['hash', 'full']),
              default='hash', help='In what format to add the public key to '
              'the image manifest: full key or hash of the key.')
@click.option('-k', '--key', metavar='filename')
@click.command(help='''Create a signed or unsigned image\n
               INFILE and OUTFILE are parsed as Intel HEX if the params have
               .hex extension, otherwise binary format is used''')
def sign(key, public_key_format, align, version, pad_sig, header_size,
         pad_header, slot_size, pad, confirm, max_sectors, overwrite_only,
         endian, encrypt_keylen, encrypt, infile, outfile, dependencies,
         load_addr, hex_addr, erased_val, save_enctlv, security_counter,
         boot_record, custom_tlv, rom_fixed):

    if confirm:
        # Confirmed but non-padded images don't make much sense, because
        # otherwise there's no trailer area for writing the confirmed status.
        pad = True
    img = image.Image(version=decode_version(version), header_size=header_size,
                      pad_header=pad_header, pad=pad, confirm=confirm,
                      align=int(align), slot_size=slot_size,
                      max_sectors=max_sectors, overwrite_only=overwrite_only,
                      endian=endian, load_addr=load_addr, rom_fixed=rom_fixed,
                      erased_val=erased_val, save_enctlv=save_enctlv,
                      security_counter=security_counter)
    img.load(infile)
    key = load_key(key) if key else None
    enckey = load_key(encrypt) if encrypt else None
    if enckey and key:
        if ((isinstance(key, keys.ECDSA256P1) and
             not isinstance(enckey, keys.ECDSA256P1Public))
                or (isinstance(key, keys.RSA) and
                    not isinstance(enckey, keys.RSAPublic))):
            # FIXME
            raise click.UsageError("Signing and encryption must use the same "
                                   "type of key")

    if pad_sig and hasattr(key, 'pad_sig'):
        key.pad_sig = True

    # Get list of custom protected TLVs from the command-line
    custom_tlvs = {}
    for tlv in custom_tlv:
        tag = int(tlv[0], 0)
        if tag in custom_tlvs:
            raise click.UsageError('Custom TLV %s already exists.' % hex(tag))
        if tag in image.TLV_VALUES.values():
            raise click.UsageError(
                'Custom TLV %s conflicts with predefined TLV.' % hex(tag))

        value = tlv[1]
        if value.startswith('0x'):
            if len(value[2:]) % 2:
                raise click.UsageError('Custom TLV length is odd.')
            custom_tlvs[tag] = bytes.fromhex(value[2:])
        else:
            custom_tlvs[tag] = value.encode('utf-8')

    img.create(key, public_key_format, enckey, dependencies, boot_record,
               custom_tlvs, int(encrypt_keylen))
    img.save(outfile, hex_addr)


class AliasesGroup(click.Group):

    _aliases = {
        "create": "sign",
    }

    def list_commands(self, ctx):
        cmds = [k for k in self.commands]
        aliases = [k for k in self._aliases]
        return sorted(cmds + aliases)

    def get_command(self, ctx, cmd_name):
        rv = click.Group.get_command(self, ctx, cmd_name)
        if rv is not None:
            return rv
        if cmd_name in self._aliases:
            return click.Group.get_command(self, ctx, self._aliases[cmd_name])
        return None


@click.command(help='Print imgtool version information')
def version():
    print(imgtool_version)


@click.command(cls=AliasesGroup,
               context_settings=dict(help_option_names=['-h', '--help']))
def imgtool():
    pass


imgtool.add_command(keygen)
imgtool.add_command(getpub)
imgtool.add_command(getpriv)
imgtool.add_command(verify)
imgtool.add_command(sign)
imgtool.add_command(version)


if __name__ == '__main__':
    imgtool()
