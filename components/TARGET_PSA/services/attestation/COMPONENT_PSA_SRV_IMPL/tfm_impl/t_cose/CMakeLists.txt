#-------------------------------------------------------------------------------
# Copyright (c) 2019, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.7)

#Tell cmake where our modules can be found
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../../../cmake)

#Include common stuff to control cmake.
include("Common/BuildSys")

#Start an embedded project.
embedded_project_start(CONFIG "${CMAKE_CURRENT_LIST_DIR}/../../ConfigDefault.cmake")
project(tfm_t_cose LANGUAGES C)
embedded_project_fixup()

#Some project global settings
set(T_COSE_DIR "${CMAKE_CURRENT_LIST_DIR}")

#Append all our source files to global lists.
list(APPEND ALL_SRC_C
	"${T_COSE_DIR}/src/t_cose_sign1_sign.c"
	"${T_COSE_DIR}/src/t_cose_util.c"
	"${T_COSE_DIR}/src/t_cose_psa_crypto.c"
	)

#Setting include directories
embedded_include_directories(PATH ${T_COSE_DIR}/inc ABSOLUTE)

#Specify what we build (for the t_cose, build as an object library)
add_library(${PROJECT_NAME} OBJECT ${ALL_SRC_C})

#Set common compiler flags
config_setting_shared_compiler_flags(${PROJECT_NAME})

embedded_project_end(${PROJECT_NAME})
