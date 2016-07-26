#!/bin/bash

# Copyright (c) 2015 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
# Licensed under the Apache License, Version 2.0 (the License); you may
# not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an AS IS BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
 

rm -rf coverage
lcov --base-directory ../../ --directory ../../output/gcc_linux/source -c -o testapp.info
lcov --base-directory ../../ --directory ../../output/lwm2m-client-linux/source -c -o lwm2mlinux.info
lcov --base-directory ../../ --directory ../../output/lwm2m-client-mbedtls/source -c -o mbedtls.info
lcov --base-directory ../../../../nsdl-c --directory ../../../../nsdl-c/output/gcc_/source/libCoap/src -c -o libcoap.info
lcov --base-directory ../../../../nsdl-c --directory ../../../../nsdl-c/output/gcc_/source/libNsdl/src -c -o libnsdl.info
genhtml -o coverage -t "lwm2mtestapplication coverage" --num-spaces 4 testapp.info libcoap.info libnsdl.info lwm2mlinux.info mbedtls.info
lcov --base-directory ../../ --directory ../../output/gcc_linux/source -z
lcov --base-directory ../../ --directory ../../output/lwm2m-client-linux/source -z
lcov --base-directory ../../ --directory ../../output/lwm2m-client-mbedtls/source -z
lcov --base-directory ../../../../nsdl-c --directory ../../../../nsdl-c/output/gcc_/source/libCoap/src -z
lcov --base-directory ../../../../nsdl-c --directory ../../../../nsdl-c/output/gcc_/source/libNsdl/src -z