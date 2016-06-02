#!/bin/bash
# script to run tests
#
# todo: want to do debug builds too.
# set -x
# set -v
# set -o verbose
# uncomment to debug
# set -v -o xtrace

CFSTORE_timestamp=`date '+%Y_%m_%d_%T' | sed s/:/_/ | sed s/:/_/`
CFSTORE_TEST_REPO_DIR=cfstore_example3_mx2_test_${CFSTORE_timestamp}
CFSTORE_RESULTS_FILE=cfstore_example3_mx_test_log_${CFSTORE_timestamp}.txt
CFSTORE_TOPDIR=`pwd`
CFSTORE_TMP_FILE=/tmp/${timestamp}_temp.txt
CFSTORE_MBED_SETTINGS=mbed_settings.py
# list of error codes
CFSTORE_ERR_BASE_MAIN=0

# main return error code
CFSTORE_RET_VAL=0

# todo: add usage statement.
# ./cfstore_test_mx.sh ex3_k64f_gcc 2>&1 | tee cfstore_test_log.txt


############################################################################### 
# FUNCTION: cfstore_make_mbed_settings_file
#  create a file of the material embedded in this file.
############################################################################## 
cfstore_make_mbed_settings_file()
{
    _err=${CFSTORE_ERR_BASE_MAIN}

    rm -f ${CFSTORE_TMP_FILE}
    cat > ${CFSTORE_TMP_FILE} <<-EOF
"""
mbed SDK
Copyright (c) 2011-2013 ARM Limited
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

from os.path import join, abspath, dirname
import logging

ROOT = abspath(join(dirname(__file__), ".."))

# These default settings have two purposes:
#    1) Give a template for writing local "private_settings.py"
#    2) Give default initialization fields for the "toolchains.py" constructors

##############################################################################
# Build System Settings
##############################################################################
BUILD_DIR = abspath(join(ROOT, "build"))

# ARM
#ARM_PATH = "C:/Program Files/ARM"
ARM_PATH = "C:/mbed_tools/ARMCompiler_5.03_117"

ARM_BIN = join(ARM_PATH, "bin")
ARM_INC = join(ARM_PATH, "include")
ARM_LIB = join(ARM_PATH, "lib")

ARM_CPPLIB = join(ARM_LIB, "cpplib")
MY_ARM_CLIB = join(ARM_PATH, "lib", "microlib")

# GCC ARM
GCC_ARM_PATH = "C:/mbed_tools/GNU_Tools_ARM_Embedded/4.9_2014q4/bin"

# GCC CodeRed
GCC_CR_PATH = "C:/code_red/RedSuite_4.2.0_349/redsuite/Tools/bin"

# IAR
IAR_PATH = "C:/Program Files (x86)/IAR Systems/Embedded Workbench 7.0/arm"

# Goanna static analyser. Please overload it in private_settings.py
GOANNA_PATH = "c:/Program Files (x86)/RedLizards/Goanna Central 3.2.3/bin"

# cppcheck path (command) and output message format
CPPCHECK_CMD = ["cppcheck", "--enable=all"]
CPPCHECK_MSG_FORMAT = ["--template=[{severity}] {file}@{line}: {id}:{message}"]

BUILD_OPTIONS = []

# mbed.org username
MBED_ORG_USER = ""

##############################################################################
# Private Settings
##############################################################################
try:
    # Allow to overwrite the default settings without the need to edit the
    # settings file stored in the repository
    from mbed_settings import *
except ImportError:
    print '[WARNING] Using default settings. Define your settings in the file "./mbed_settings.py"'
	EOF
    mv ${CFSTORE_TMP_FILE} ${CFSTORE_MBED_SETTINGS}
    return ${_err}
}


############################################################################### 
# FUNCTION: cfstore_test_setup
# 
############################################################################## 
cfstore_test_setup()
{
    _err=${CFSTORE_ERR_BASE_MAIN}

	echo "**************************************************"
	echo "*** CONFIGURATION-STORE: CLONING GITHUB REPOS ***"
	echo "**************************************************"
	git clone git@github.com:/armmbed/cfstore_example3_mx2.git ${CFSTORE_TEST_REPO_DIR}
	pushd ${CFSTORE_TEST_REPO_DIR}
	mbed update

	# need to create an mbed_settings.py if one doesnt already exist
	# and populate it with the correct settings for compiler
	cfstore_make_mbed_settings_file
	popd

    return ${_err}
}

############################################################################### 
# FUNCTION: cfstore_ex3_bld_k64f_gcc
# example3: build with toolchain GCC_ARM
############################################################################## 
cfstore_ex3_bld_k64f_gcc()
{
    _err=${CFSTORE_ERR_BASE_MAIN}

	echo "********************************************************************"
	echo "*** CFSTORE_EXAMPLE_MX2: TESTING MORPHEUS EXAMPLE : GCC EXAMPLE3 ***"
	echo "********************************************************************"
	cd ${CFSTORE_TEST_REPO_DIR}
	pwd
	rm -fR .build
	mbed compile --mcu K64F --toolchain GCC_ARM
	cd ..

    return ${_err}
}

############################################################################### 
# FUNCTION: cfstore_ex3_bld_k64f_arm
# 
############################################################################## 
cfstore_ex3_bld_k64f_arm()
{
    _err=${CFSTORE_ERR_BASE_MAIN}

	echo "********************************************************************"
	echo "*** CFSTORE_EXAMPLE_MX2: TESTING MORPHEUS EXAMPLE : ARM EXAMPLE3 ***"
	echo "********************************************************************"
	cd ${CFSTORE_TEST_REPO_DIR}
	pwd
	rm -fR .build
	mbed compile --mcu K64F --toolchain ARM
	cd ..

    return ${_err}
}

############################################################################### 
# FUNCTION: cfstore_ex3_bld_tst_k64f_gcc
# 
############################################################################## 
cfstore_ex3_bld_tst_k64f_gcc()
{
    _err=${CFSTORE_ERR_BASE_MAIN}

	echo "********************************************************************"
	echo "*** CFSTORE_EXAMPLE_MX2: TESTS MORPHEUS EXAMPLE : GCC TESTS      ***"
	echo "********************************************************************"
	cd ${CFSTORE_TEST_REPO_DIR}
	pwd
	rm -fR .build
	mv example3.cpp example3_cpp.txt
	mbed compile --tests --mcu K64F --toolchain GCC_ARM
	mbedgt -VS --test-spec=mbed-os/core/configuration-store/scripts/cfstore_test_spec_k64f_gcc.json
	mv example3_cpp.txt example3.cpp
	cd ..

    return ${_err}
}

############################################################################### 
# FUNCTION: cfstore_ex3_bld_tst_k64f_arm
# 
############################################################################## 
cfstore_ex3_bld_tst_k64f_arm()
{
    _err=${CFSTORE_ERR_BASE_MAIN}

	echo "********************************************************************"
	echo "*** CFSTORE_EXAMPLE_MX2: TESTS MORPHEUS EXAMPLE : ARM TESTS      ***"
	echo "********************************************************************"
	cd ${CFSTORE_TEST_REPO_DIR}
	pwd
	rm -fR .build
	mv example3.cpp example3_cpp.txt
	mbed compile --tests --mcu K64F --toolchain ARM
	mv example3_cpp.txt example3.cpp
	# todo: run the tests
	# mbedgt -VS --test-spec=mbed-os/core/configuration-store/scripts/cfstore_test_spec_k64f_arm.json
	cd ..

    return ${_err}
}


############################################################################### 
# FUNCTION: main
#  main entry point
# 
############################################################################## 
main()
{
	case $1 in 
	    all)
	    	cfstore_test_setup
			cfstore_ex3_bld_k64f_gcc
			cfstore_ex3_bld_k64f_arm
			cfstore_ex3_bld_tst_k64f_gcc
			cfstore_ex3_bld_tst_k64f_arm
		;;
	    ex3_k64f_gcc)
	    	cfstore_test_setup
			cfstore_ex3_bld_k64f_gcc
		;;
	    ex3_k64f_arm)
	    	cfstore_test_setup
			cfstore_ex3_bld_k64f_arm
		;;
	    tests_k64f_gcc)
	    	cfstore_test_setup
			cfstore_ex3_bld_tst_k64f_gcc
		;;
	    tests_k64f_arm)
	    	cfstore_test_setup
			cfstore_ex3_bld_tst_k64f_arm
		;;
	    mbed_settings)
	    	cfstore_make_mbed_settings_file
		;;
	    *)
	    echo "Usage: $0 all|ex3_k64f_gcc|ex3_k64f_arm|tests_k64f_gcc|tests_k64f_arm|mbed_settings";
	esac  
}

main $*
exit $RETVAL

