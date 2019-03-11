/*
 * Copyright (c) 2019 STMicroelectronics. All Rights Reserved.
 *
 * The information contained herein is property of STMicroelectronics.
 * Terms and conditions of usage are described in detail in SOFTWARE
 * LICENSE AGREEMENT:
 *
 * (SLA0068 - PRODUCTION LIMITED LICENSE AGREEMENT FOR ST MATERIALS)
 *
 * You may obtain a copy of the License here:
 * LICENSE-SLA0068.txt and at STMicroelectronics website.
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 */

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "osal.h"
 
/**
 * Osal_MemCpy
 * 
 */
 
void* Osal_MemCpy(void *dest,const void *src,unsigned int size)
{
    return(memcpy(dest,src,size)); 
}

/**
 * Osal_MemSet
 * 
 */
 
void* Osal_MemSet(void *ptr, int value,unsigned int size)
{
    return(memset(ptr,value,size));
}

/**
 * Osal_MemCmp
 * 
 */
int Osal_MemCmp(void *s1,void *s2,unsigned int size)
{
    return(memcmp(s1,s2,size));
}

