#!/usr/bin/env bash

# Copyright (c) 2019, Cypress Semiconductor Corporation, All Rights Reserved
# SPDX-License-Identifier: LicenseRef-PBL
#
# This file and the related binary are licensed under the
# Permissive Binary License, Version 1.0 (the "License");
# you may not use these files except in compliance with the License.
#
# You may obtain a copy of the License here:
# LICENSE-permissive-binary-license-1.0.txt and at
# https://www.mbed.com/licenses/PBL-1.0
#
# See the License for the specific language governing permissions and
# limitations under the License.

# Path to the current script
SCRIPTPATH="$(dirname "$0")"

# Enable error mode and verbose tracing
set -ex

# Path to Elf2Bin tool
# ELF2BIN="C:/Program Files (x86)/GNU Tools ARM Embedded/7 2018-q2-update/bin/arm-none-eabi-objcopy.exe"
[[ -n "$ELF2BIN" ]] || ELF2BIN="${USERPROFILE:-$HOME}/ModusToolbox/tools_2.1/gcc-7.2.1/bin/arm-none-eabi-objcopy.exe"

# Path to Python executable
[[ -n "$PYTHON" ]] || PYTHON=python

# This function converts ELF executable to C file
# Additionally, it generates HEX file
# arg#1: path to ELF file
# arg#2: path to C file
# arg#3: C preprocessor guard macro (optional)
function elf2c()
{
    [[ $# -ge 2 ]] || { echo >&2 "Error: expected 2 or 3 arguments: <path-to-elf> <path-to-c> [c-macro]"; exit 1; }

    local ELF_PATH="$1"
    local HEX_PATH="${ELF_PATH%.*}.hex"
    local BIN_PATH="${ELF_PATH%.*}.bin"
    local C_PATH="$2"
    local C_MACRO="$3"

    file "$ELF_PATH" || { echo >&2 "Error: $ELF_PATH is not found, exiting"; exit 1; }
    "$ELF2BIN" -O ihex "$ELF_PATH" "$HEX_PATH"
    "$ELF2BIN" -O binary "$ELF_PATH" "$BIN_PATH"
    "$PYTHON" "$SCRIPTPATH/bin2c.py" "$BIN_PATH" "$C_PATH" "$C_MACRO"
    echo "Successfully converted $ELF_PATH to $C_PATH"
}

# Check ELF2BIN
"$ELF2BIN" --version || { echo >&2 "Error: $ELF2BIN is not found, exiting"; exit 1; }

# Check python
"$PYTHON" --version || { echo >&2 "Error: python executable is not found, exiting"; exit 1; }

# Convert ELF to C
elf2c $@
