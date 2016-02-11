/*-----------------------------------------------------------------------------
 * Name:    Device.h
 * Purpose: Include the correct device header file
 *-----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2013 KEIL - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#ifndef __DEVICE_H
#define __DEVICE_H

#if   defined CMSDK_CM0
  #include "CMSDK_CM0.h"                         /* device specific header file */
#elif defined CMSDK_CM0plus
  #include "CMSDK_CM0plus.h"                     /* device specific header file */
#elif defined CMSDK_CM3
  #include "CMSDK_CM3.h"                         /* device specific header file */
#elif defined CMSDK_CM4
  #include "CMSDK_CM4.h"                         /* device specific header file */
#elif defined CMSDK_CM7
  #include "CMSDK_CM7.h"                         /* device specific header file */
#elif defined CMSDK_BEID
  #include "CMSDK_BEID.h"                         /* device specific header file */
#else
  #warning "no appropriate header file found!"
#endif

#endif /* __DEVICE_H */
