/**
 *******************************************************************************
 * @file    tx00_common.h
 * @brief   All common macro and definition for TX00 peripheral drivers
 * @version V2.1.1
 * @date    2014/11/22
 *
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
 *******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TX00_COMMON_H
#define __TX00_COMMON_H

typedef enum {
    SUCCESS = 0U,
    ERROR = 1U
} Result;

typedef enum {
    BUSY = 0U,
    DONE = 1U
} WorkState;

typedef enum {
    DISABLE = 0U,
    ENABLE = 1U
} FunctionalState;
#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

#define IS_POINTER_NOT_NULL(param) ((void*)(param) != (void*)0)

/* 
 * To report the name of the source file and source line number where the
 * assert_param error has occurred, "DEBUG" must be defined. And detailed
 * definition of assert_failed() is needed to be implemented, which can be
 * done, for example, in the main.c file.
 */
#ifdef DEBUG
void assert_failed(char *file, int32_t line);
#define assert_param(expr) ((expr) ? (void)0 : assert_failed((char *)__FILE__, __LINE__))
#else
#define assert_param(expr)
#endif                          /* DEBUG */

#endif                          /* __TX00_COMMON_H */
