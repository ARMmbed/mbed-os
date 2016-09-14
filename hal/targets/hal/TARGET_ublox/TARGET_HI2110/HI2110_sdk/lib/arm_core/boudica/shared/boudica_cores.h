/******************************************************************************
 * @brief    NEUL Boudica common definitions
 * Copyright (c) 2015 NEUL LIMITED
******************************************************************************/

#ifndef LIB_COMMON_HEADERS_BOUDICA_CORES_H_
#define LIB_COMMON_HEADERS_BOUDICA_CORES_H_

#if CORE == APPS
#include "boudica_apps.h"
#include "reg_map_apps.h"
#elif CORE == PROTOCOL
#include "boudica_protocol.h"
#include "reg_map_protocol.h"
#elif CORE == SECURITY
#include "boudica_security.h"
#include "reg_map_security.h"
#else
#error 	You must define one core before including bourdica_cores.h
#endif

#include "boudica_common_peripherals.h"
#include "core_cm0.h"
#include "core_cmFunc.h"
#include "core_cmInstr.h"
#include "core_cmSimd.h"

#endif /* LIB_COMMON_HEADERS_BOUDICA_CORES_H_ */
