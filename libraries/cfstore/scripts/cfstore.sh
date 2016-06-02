#!/bin/bash

# Notes
#
# command for running mbed host test run for a specific binary
#
# 	mbedhtrun -d F: -p COM40:9600 -f "build/frdm-k64f-gcc/test/configuration-store-test-flash.bin"  -C 4 -c shell -m K64F

#
# The following line defines YOTTA_CFG_CONFIG_HARDWARE_MTD_ASYNC_OPS = 0 in yotta_config.h and then builds
#yotta --config='{"config":{"hardware":{"mtd":{"async":{"ops":0}}}}}' build

# The following line defines YOTTA_CFG_CFSTORE_BACKEND_SRAM = 1 in yotta_config.h and then builds
#yotta --config='{"cfstore":{"backend":{"sram":1}}}' build