# Copyright (c) 2021 ARM Limited
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
##
# http://www.apache.org/licenses/LICENSE-2.0
###
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

"""Module containing all binary manipulations."""

import os


def substitute_binary(binary):
    """Substitute the binary.

    Remove the directory with the binary parts and rename the temporary
    file to 'binary'
    """
    shutil.rmtree(binary, True)
    os.rename(binary + '.temp', binary)


def lpc_patch_vectortbl(binary, notification):
    """Patch an LPC target vector table in the binary file."""
    from targets.LPC import patch

    notification.debug("LPC Patch: %s" % os.path.split(binary)[1])
    patch(binary)


def lpc4088_add_padding(binary):
    """Add padding to a LPC4088 binary file."""
    outbin = open(binary + ".temp", "wb")
    partf = open(os.path.join(binary, "ER_IROM1"), "rb")

    # Pad the fist part (internal flash) with 0xFF to 512k
    data = partf.read()
    outbin.write(data)
    outbin.write(b'\xFF' * (512*1024 - len(data)))
    partf.close()

    # Read and append the second part (external flash) in chunks of fixed
    # size
    chunksize = 128 * 1024
    partf = open(os.path.join(binary, "ER_IROM2"), "rb")
    while True:
        data = partf.read(chunksize)
        outbin.write(data)
        if len(data) < chunksize:
            break

    partf.close()
    outbin.close()


def lpc4088_pad_binary(binary, notification):
    """Add padding to a LPC4088 binary."""
    if not os.path.isdir(binary):
        # Regular binary file, nothing to do
        lpc_patch_vectortbl(binary, notification)
        return

    lpc4088_add_padding(binary)

    substitute_binary(binary)

    notification.debug(
        "Generated custom binary file (internal flash + SPIFI)"
    )

    lpc_patch_vectortbl(binary, notification)


def mcu_nrf51code_merge_images(
    notification, binary, is_soft_device_merge, soft_device,
    soft_device_offset, is_bootloader_merge, bootloader
):
    """Merge the soft device with the user code."""
    from intelhex import IntelHex

    ih = IntelHex()
    _, ext = os.path.splitext(binary)

    if ext == ".hex":
        ih.loadhex(binary)
    elif ext == ".bin":
        ih.loadbin(binary, soft_device_offset)

    if is_soft_device_merge:
        notification.debug("Merge SoftDevice file %s"
                            % soft_device)
        sdh = IntelHex(sdf)
        sdh.start_addr = None
        ih.merge(sdh)

    if is_bootloader_merge and bootloader:
        notification.debug("Merge BootLoader file %s" % bootloader)
        blh = IntelHex(bootloader)
        blh.start_addr = None
        ih.merge(blh)

    with open(binary.replace(".bin", ".hex"), "w") as fileout:
        ih.write_hex_file(fileout, write_start_addr=False)


def rtl8195a_convert_elf_to_bin(toolchain, elf, binary):
    """"Convert an ELF file to a binary file"""
    from targets.REALTEK_RTL8195AM import rtl8195a_elf2bin

    rtl8195a_elf2bin(toolchain, elf, binary)


def psoc6code_merge_images(
    binary, elf_file, notification, cortex_m0_hex_file=None, hex_file=None
):
    """Merge the ELF file and the binary file."""
    from targets.PSOC6 import complete as psoc6_complete

    psoc6_complete(
        notification.debug,
        elf_file,
        binary,
        cortex_m0_hex_file
    )


def psoc6code_add_signature_to_image(
    build_dir, target_name, policy, boot_scheme, cortex_m0_img_id,
    cortex_m4_img_id, binary, elf_file, notification, hex_file=None,
    cortex_m0_hex_file=None
):
    """Add a signature to the image."""
    from tools.targets.PSOC6 import sign_image as psoc6_sign_image

    if hex_file:
        psoc6_sign_image(
            build_dir,
            hex_file,
            target_name,
            policy,
            notification,
            boot_scheme,
            cortex_m0_img_id,
            cortex_m4_img_id,
            elf_file,
            binary,
            cortex_m0_hex_file
        )


def arm_musca_a1_merge_images(secure_bin, binary):
    """Merge ARM MUSCA A1 secure and non-secure images."""
    from tools.targets.ARM_MUSCA_A1 import musca_tfm_bin
    musca_tfm_bin(binary, secure_bin)


def arm_musca_b1_merge_images(secure_bin, binary):
    """Merge ARM MUSCA B1 secure and non-secure images."""
    from tools.targets.ARM_MUSCA_B1 import musca_tfm_bin
    musca_tfm_bin(binary, secure_bin)


def arm_musca_s1_merge_images(secure_bin, binary):
    """Merge ARM MUSCA S1 secure and non-secure images."""
    from tools.targets.ARM_MUSCA_S1 import musca_tfm_bin
    musca_tfm_bin(binary, secure_bin)


def m2351code_merge_images(notification, secure_image, ns_elf, ns_hex):
    """Merge secure and non-secure images."""
    notification.info("Merging non-secure image with secure image")
    notification.info("Non-secure elf image %s" % ns_elf)
    notification.info("Non-secure hex image %s" % ns_hex)
    notification.info("Finding secure image %s" % secure_image)

    notification.info("Found secure image %s" % s_hex)

    _, ext = os.path.splitext(s_hex)

    if ext != ".hex":
        notification.debug("Secure image %s must be in Intel HEX format" % s_hex)
        return

    if not os.path.isfile(s_hex):
        notification.debug("Secure image %s must be regular file" % s_hex)
        return

    ns_main, ext = os.path.splitext(ns_hex)

    if ext != ".hex":
        notification.debug("Non-secure image %s must be in Intel HEX format" % s_hex)
        return

    if not os.path.isfile(ns_hex):
        notification.debug("Non-secure image %s must be regular file" % s_hex)
        return

    # Keep original non-secure before merge with secure
    ns_nosecure_hex = ns_main + "_no-secure-merge" + ext
    notification.info("Keep no-secure-merge image %s" % ns_nosecure_hex)
    shutil.copy2(ns_hex, ns_nosecure_hex)

    # Merge secure and non-secure and save to non-secure (override it)
    from intelhex import IntelHex

    s_ih = IntelHex()
    s_ih.loadhex(s_hex)
    ns_ih = IntelHex()
    ns_ih.loadhex(ns_hex)
    ns_ih.start_addr = None
    s_ih.merge(ns_ih)
    s_ih.tofile(ns_hex, 'hex')


def mtscode_combine_bins_helper(target_name, binf):
    """Combine bins with the bootloader for a particular target."""
    from paths import TOOLS_BOOTLOADERS

    loader = os.path.join(TOOLS_BOOTLOADERS, target_name, "bootloader.bin")
    target = binf + ".tmp"

    if not os.path.exists(loader):
        print("Can't find bootloader binary: " + loader)
        return

    outbin = open(target, 'w+b')
    part = open(loader, 'rb')
    data = part.read()
    outbin.write(data)
    outbin.write(b'\xFF' * (64*1024 - len(data)))
    part.close()

    part = open(binf, 'rb')
    data = part.read()
    outbin.write(data)
    part.close()

    outbin.seek(0, 0)
    data = outbin.read()
    outbin.seek(0, 1)
    crc = struct.pack('<I', binascii.crc32(data) & 0xFFFFFFFF)
    outbin.write(crc)
    outbin.close()

    os.remove(binf)
    os.rename(target, binf)
