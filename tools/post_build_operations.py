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

"""Post build operation executer script

This script is used to select and execute post-build operation for certain
Mbed targets that require it.
"""
import argparse
import logging
import pathlib
import sys
from enum import Enum

# Ensure all local Python modules are visible to this script
MBED_OS_ROOT = str(pathlib.Path(__file__).absolute().parents[1])
sys.path.insert(0, MBED_OS_ROOT)

from resources import Resources, FileType, FileRef
from targets import (
    ArmMuscaA1Code, ArmMuscaB1Code, ArmMuscaS1Code,
    LPC4088Code, LPCTargetCode, M2351Code, MCU_NRF51Code, MTSCode,
    NCS36510TargetCode, PSOC6Code, RTL8195ACode, TEENSY3_1Code
)


LOG = logging.getLogger(__name__)

LOG_FORMAT = "%(asctime)s - %(levelname)s - %(message)s"


class ReturnCode(Enum):
    """Return codes."""

    SUCCESS = 0
    ERROR = 1
    INVALID_OPTIONS = 2


class ArgumentParserWithDefaultHelp(argparse.ArgumentParser):
    """Subclass that always shows the help message on invalid arguments."""

    def error(self, message):
        """Error handler."""
        sys.stderr.write(f"error: {message}\n")
        self.print_help()
        raise SystemExit(ReturnCode.INVALID_OPTIONS.value)


class PostBuildOpIdError(Exception):
    """An exception for an incorrect post build operation identifier."""


def raise_(ex):
    """Raise an exception.

    This is useful for lambda expressions.
    """
    raise ex


def handle_lpctargetcode_lpc_patch(args):
    """Handle LPCTargetCode.lpc_patch operation."""
    LPCTargetCode.patch_vectortbl(args.binary, LOG)


def handle_lpc4088code_binary_hook(args):
    """Handle LPC4088Code.binary_hook operation."""
    LPC4088Code.pad_binary(args.binary, LOG)


def handle_psoc6code_complete(args):
    """Handle PSOC6Code.complete operation."""
    PSOC6Code.merge_images(
        args.binary, args.elf, LOG, args.m0_img, args.hex
    )


def handle_psoc6code_sign_image(args):
    """Handle PSOC6Code.sign_image operation."""
    PSOC6Code.add_signature_to_image(
        args.build_dir, args.mbed_target, args.policy, args.boot_scheme,
        args.m0_img, args.m4_img, args.binary, args.elf, LOG,
        args.hex
    )


def handle_armmuscas1code_binary_hook(args):
    """Handle ArmMuscaS1Code.binary_hook operation."""
    ArmMuscaS1Code.merge_images(args.secure_image, args.binary)


def handle_armmuscab1code_binary_hook(args):
    """Handle ArmMuscaB1Code.binary_hook operation."""
    ArmMuscaB1Code.merge_images(args.secure_image, args.binary)


def handle_armmuscaa1code_binary_hook(args):
    """Handle ArmMuscaA1Code.binary_hook operation."""
    ArmMuscaA1Code.merge_images(args.secure_image, args.binary)


def handle_teensy3_1code_binary_hook(args):
    """Handle TEENSY3_1Code.binary_hook operation."""
     # This function is referenced by old versions of targets.json and
    # should be kept for backwards compatibility.
    pass


def handle_ncs36510targetcode_ncs36510_addfib(args):
    """Handle NCS36510TargetCode.ncs36510_addfib operation."""
    # This function is referenced by old versions of targets.json and
    # should be kept for backwards compatibility.
    pass


def handle_mtscode_combine_bins_mts_dot(args):
    """Handle MTSCode.combine_bins_mts_dot operation."""
    MTSCode._combine_bins_helper("MTS_MDOT_F411RE", args.binary)


def handle_mtscode_combine_bins_mts_dragonfly(args):
    """Handle MTSCode.combine_bins_mts_dragonfly operation."""
    MTSCode._combine_bins_helper("MTS_DRAGONFLY_F411RE", args.binary)


def handle_mtscode_combine_bins_mtb_mts_dragonfly(args):
    """Handle MTSCode.combine_bins_mtb_mts_dragonfly operation."""
    MTSCode._combine_bins_helper("MTB_MTS_DRAGONFLY", args.binary)


def handle_rtl8195acode_binary_hook(args):
    """Handle RTL8195ACode.binary_hook operation."""
    RTL8195ACode.convert_elf_to_bin(args.toolchain, args.elf, args.binary)


def handle_m2351code_merge_secure(args):
    """Handle M2351Code.merge_secure operation."""
    M2351Code.merge_images(
        LOG, args.secure_image, args.elf, args.hex
    )


def handle_mcu_nrf51code_binary_hook(args):
    """Handle MCU_NRF51Code.binary_hook operation."""
    MCU_NRF51Code.merge_images(
        LOG, args.binary,
        True if args.soft_device else False,
        args.soft_device, args.soft_device_offset,
        True if args.bootloader else False,
        args.bootloader
    )


POST_BUILD_OPERATIONS = {
    "LPCTargetCode.lpc_patch" : handle_lpctargetcode_lpc_patch,
    "LPC4088Code.binary_hook" : handle_lpc4088code_binary_hook,
    "PSOC6Code.complete" : handle_psoc6code_complete,
    "PSOC6Code.sign_image" : handle_psoc6code_sign_image,
    "ArmMuscaS1Code.binary_hook" : handle_armmuscas1code_binary_hook,
    "ArmMuscaB1Code.binary_hook" : handle_armmuscab1code_binary_hook,
    "ArmMuscaA1Code.binary_hook" : handle_armmuscaa1code_binary_hook,
    "TEENSY3_1Code.binary_hook" : handle_teensy3_1code_binary_hook,
    "NCS36510TargetCode.ncs36510_addfib" : handle_ncs36510targetcode_ncs36510_addfib,
    "MTSCode.combine_bins_mts_dot" : handle_mtscode_combine_bins_mts_dot,
    "MTSCode.combine_bins_mts_dragonfly" : handle_mtscode_combine_bins_mts_dragonfly,
    "MTSCode.combine_bins_mtb_mts_dragonfly" : handle_mtscode_combine_bins_mtb_mts_dragonfly,
    "MTSCode.combine_bins_mtb_mts_dragonfly" : handle_mtscode_combine_bins_mtb_mts_dragonfly,
    "RTL8195ACode.binary_hook" : handle_rtl8195acode_binary_hook,
    "M2351Code.merge_secure" : handle_m2351code_merge_secure,
    "MCU_NRF51Code.binary_hook" : handle_mcu_nrf51code_binary_hook,
}


def execute_operation(args):
    """Select and run an operation.

    Throws an exception if an invalid operation-id is provided.
    """
    post_build_operation = POST_BUILD_OPERATIONS.get(
        args.operation_id,
        lambda: raise_(
            PostBuildOpIdError(
                f"operation id `{args.operation_id}` is not supported."
            )
        )
    )

    post_build_operation(args)


def set_log_verbosity(increase_verbosity):
    """Set the verbosity of the log output."""
    log_level = logging.DEBUG if increase_verbosity else logging.INFO

    LOG.setLevel(log_level)
    logging.basicConfig(level=log_level, format=LOG_FORMAT)


def parse_args():
    """Parse the command line args."""
    parser = ArgumentParserWithDefaultHelp(
        description="Perform an operation after the binary has been built.",
        formatter_class=argparse.ArgumentDefaultsHelpFormatter,
    )

    parser.add_argument(
        "operation_id",
        type=str,
        help="the unique identifier of the post-build operation to execute.",
    )

    parser.add_argument(
        "--binary",
        type=str,
        help="the binary file to execute a post-build operation with.",
    )

    parser.add_argument(
        "--bootloader",
        type=str,
        help="the bootloader to merge the binary with.",
    )

    parser.add_argument(
        "--boot-scheme",
        type=str,
        help="Image signing boot scheme.",
    )

    parser.add_argument(
        "--soft-device",
        type=str,
        help="the soft device to merge the binary with.",
    )

    parser.add_argument(
        "--soft-device-offset",
        type=str,
        help="the offset at which to place the soft device.",
    )

    parser.add_argument(
        "--policy",
        type=str,
        help="Image signing policy.",
    )

    parser.add_argument(
        "--secure-image",
        type=str,
        help="the secure binary/hex file to execute a post-build operation with.",
    )

    parser.add_argument(
        "--elf",
        type=str,
        help="the elf file to execute a post-build operation with.",
    )

    parser.add_argument(
        "--hex",
        type=str,
        help="the hex file to execute a post-build operation with.",
    )

    parser.add_argument(
        "--m0-img",
        type=str,
        help="the Cortex-M0 hex/bin file to merge with.",
    )

    parser.add_argument(
        "--m4-img",
        type=str,
        help="the Cortex-M4 hex/bin file to merge with.",
    )

    parser.add_argument(
        "--toolchain",
        type=str,
        help="the toolchain used to build the binary.",
    )

    parser.add_argument(
        "--mbed-target",
        type=str,
        help="a build target for a Mbed-enabled device.",
    )

    parser.add_argument(
        "--build-dir",
        type=str,
        help="the build directory",
    )

    parser.add_argument(
        "-v",
        "--verbose",
        action="store_true",
        help="increase verbosity of status information.",
    )

    parser.set_defaults(func=execute_operation)

    args_namespace = parser.parse_args()

    # We want to fail gracefully, with a consistent
    # help message, in the no argument case.
    # So here's an obligatory hasattr hack.
    if not hasattr(args_namespace, "func"):
        parser.error("No arguments given!")
    else:
        return args_namespace


def run_post_build_operations():
    """Application main algorithm."""
    args = parse_args()

    set_log_verbosity(args.verbose)

    LOG.debug("Starting post-build-operations")
    LOG.debug("Command line arguments: %s", args)

    args.func(args)


def _main():
    """Run post-build-operations."""
    try:
        run_post_build_operations()
    except Exception as error:
        print(error)
        return ReturnCode.ERROR.value
    else:
        return ReturnCode.SUCCESS.value


if __name__ == "__main__":
    sys.exit(_main())
