#-------------------------------------------------------------------------------
# Copyright (c) 2018, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

#Definitions to compile the "ipc" module.
#This file assumes it will be included from a project specific cmakefile, and
#will not create a library or executable.
#Inputs:
#	TFM_ROOT_DIR - directory where secure FW sourec is located.
#
#Outputs:
#	Will modify include directories to make the source compile.
#	ALL_SRC_C: C source files to be compiled will be added to this list.
#	  This shall be added to your add_executable or add_library command.
#	ALL_SRC_CXX: C++ source files to be compiled will be added to this list.
#	  This shall be added to your add_executable or add_library command.
#	ALL_SRC_ASM: assembly source files to be compiled will be added to this
#	  list. This shall be added to your add_executable or add_library
#	  command.
#	Include directories will be modified by using the include_directories()
#	  commands as needed.

#Get the current directory where this file is located.
set(SS_IPC_DIR ${CMAKE_CURRENT_LIST_DIR})
if(NOT DEFINED TFM_ROOT_DIR)
	message(FATAL_ERROR
		"Please set TFM_ROOT_DIR before including this file.")
endif()

if (NOT DEFINED TFM_PSA_API)
	message(FATAL_ERROR "Incomplete build configuration: TFM_PSA_API is undefined. ")
elseif (TFM_PSA_API)
	set (SS_IPC_C_SRC "${SS_IPC_DIR}/tfm_svcalls.c"
			"${SS_IPC_DIR}/psa_service.c"
			"${SS_IPC_DIR}/psa_client.c"
			"${SS_IPC_DIR}/tfm_arch_v8m.c"
			"${SS_IPC_DIR}/tfm_thread.c"
			"${SS_IPC_DIR}/tfm_wait.c"
			"${SS_IPC_DIR}/tfm_utils.c"
			"${SS_IPC_DIR}/tfm_message_queue.c"
			"${SS_IPC_DIR}/tfm_pools.c"
			"${SS_IPC_DIR}/tfm_spm.c"
			"${SS_IPC_DIR}/../tfm_core.c"
			"${SS_IPC_DIR}/../tfm_secure_api.c"
			"${SS_IPC_DIR}/../tfm_spm_services.c"
			"${SS_IPC_DIR}/../tfm_handler.c"
			"${SS_IPC_DIR}/../tfm_psa_api_client.c"
			"${SS_IPC_DIR}/../tfm_nspm.c"
			"${SS_IPC_DIR}/../tfm_boot_data.c"
			)
endif()

#Append all our source files to global lists.
list(APPEND ALL_SRC_C ${SS_IPC_C_SRC})
unset(SS_IPC_C_SRC)

#Setting include directories
embedded_include_directories(PATH ${TFM_ROOT_DIR} ABSOLUTE)
embedded_include_directories(PATH ${TFM_ROOT_DIR}/interface/include ABSOLUTE)
embedded_include_directories(PATH ${TFM_ROOT_DIR}/secure_fw/spm ABSOLUTE)
embedded_include_directories(PATH ${TFM_ROOT_DIR}/secure_fw/core ABSOLUTE)
embedded_include_directories(PATH ${TFM_ROOT_DIR}/secure_fw/core/ipc ABSOLUTE)
embedded_include_directories(PATH ${TFM_ROOT_DIR}/secure_fw/core/ipc/include ABSOLUTE)

if(NOT DEFINED PLATFORM_CMAKE_FILE)
	message (FATAL_ERROR "Platform specific CMake is not defined. Please set PLATFORM_CMAKE_FILE.")
elseif(NOT EXISTS ${PLATFORM_CMAKE_FILE})
	message (FATAL_ERROR "Platform specific CMake \"${PLATFORM_CMAKE_FILE}\" file does not exist. Please fix value of PLATFORM_CMAKE_FILE.")
else()
	include(${PLATFORM_CMAKE_FILE})
endif()
