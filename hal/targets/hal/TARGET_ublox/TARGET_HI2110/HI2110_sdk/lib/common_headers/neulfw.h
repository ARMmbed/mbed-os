/******************************************************************************
 * @brief    This file should be changed only infrequently and with great care.
             This file should contain the minimum necessary to get the job done.
 * Copyright (c) 2015 NEUL LIMITED
******************************************************************************/

#ifndef NEULFW_H
#define NEULFW_H

/* Create a define specifying the version of C we are compiling against */
#if defined(__STDC__)
#define C89
#if defined(__STDC_VERSION__)
#define C90
#if (__STDC_VERSION__ >= 199409L)
#define C94
#endif
#if (__STDC_VERSION__ >= 199901L)
#define C99
#endif
#endif
#endif

#include "commonc.h"
#include "types.h"
#include "config.h"


#endif /* NEULFW_H */
