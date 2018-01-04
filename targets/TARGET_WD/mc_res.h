/*
 * mc_res.h
 *
 * Created: 09.05.2015 09:22:10
 *  Author: patrick.frischmann
 */

#ifndef __MC_RES_H_
#define __MC_RES_H_

#include "mbed_error.h"

typedef enum {
	MC_RES_NOTIMPL = -2,
	MC_RES_ERROR = -1,
	MC_RES_OK    = 0
} MC_RES;

#define MC_RETURN_ON_ERROR(v) do { int rrRes = (v); if(rrRes==(int)MC_RES_ERROR) return(MC_RES_ERROR); } while (0)
#define MC_ASSERT_ON_ERROR(v, message) do { int rrRes = (v); if(rrRes==(int)MC_RES_ERROR) return(error(message)); } while (0)

#endif // !__MC_RES_H_
