# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-core
    INTERFACE
        crypto_access_control.hex
        spm_client.hex
        spm_server.hex
        spm_smoke.hex
        tfm.hex
)
