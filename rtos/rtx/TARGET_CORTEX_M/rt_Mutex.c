/*----------------------------------------------------------------------------
 *      CMSIS-RTOS  -  RTX
 *----------------------------------------------------------------------------
 *      Name:    RT_MUTEX.C
 *      Purpose: Implements mutex synchronization objects
 *      Rev.:    V4.79
 *----------------------------------------------------------------------------
 *
 * Copyright (c) 1999-2009 KEIL, 2009-2015 ARM Germany GmbH
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  - Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  - Neither the name of ARM  nor the names of its contributors may be used
 *    to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/

#include "rt_TypeDef.h"
#include "RTX_Config.h"
#include "rt_List.h"
#include "rt_Task.h"
#include "rt_Mutex.h"
#include "rt_HAL_CM.h"


/*----------------------------------------------------------------------------
 *      Functions
 *---------------------------------------------------------------------------*/


/*--------------------------- rt_mut_init -----------------------------------*/

void rt_mut_init (OS_ID mutex) {
  /* Initialize a mutex object */
  P_MUCB p_MCB = mutex;

  p_MCB->cb_type = MUCB;
  p_MCB->level   = 0U;
  p_MCB->p_lnk   = NULL;
  p_MCB->owner   = NULL;
  p_MCB->p_mlnk  = NULL;
}


/*--------------------------- rt_mut_delete ---------------------------------*/

#ifdef __CMSIS_RTOS
OS_RESULT rt_mut_delete (OS_ID mutex) {
  /* Delete a mutex object */
  P_MUCB p_MCB = mutex;
  P_TCB  p_TCB;
  P_MUCB p_mlnk;
  U8     prio;

  if (p_MCB->level != 0U) {

    p_TCB = p_MCB->owner;

    /* Remove mutex from task mutex owner list. */
    p_mlnk = p_TCB->p_mlnk;
    if (p_mlnk == p_MCB) {
      p_TCB->p_mlnk = p_MCB->p_mlnk;
    }
    else {
      while (p_mlnk) {
        if (p_mlnk->p_mlnk == p_MCB) {
          p_mlnk->p_mlnk = p_MCB->p_mlnk;
          break;
        }
        p_mlnk = p_mlnk->p_mlnk;
      }
    }

    /* Restore owner task's priority. */
    prio = p_TCB->prio_base;
    p_mlnk = p_TCB->p_mlnk;
    while (p_mlnk) {
      if ((p_mlnk->p_lnk != NULL) && (p_mlnk->p_lnk->prio > prio)) {
        /* A task with higher priority is waiting for mutex. */
        prio = p_mlnk->p_lnk->prio;
      }
      p_mlnk = p_mlnk->p_mlnk;
    }
    if (p_TCB->prio != prio) {
      p_TCB->prio = prio;
      if (p_TCB != os_tsk.run) {
        rt_resort_prio (p_TCB);
      }
    }

  }

  while (p_MCB->p_lnk != NULL) {
    /* A task is waiting for mutex. */
    p_TCB = rt_get_first ((P_XCB)p_MCB);
    rt_ret_val(p_TCB, 0U/*osOK*/);
    rt_rmv_dly(p_TCB);
    p_TCB->state = READY;
    rt_put_prio (&os_rdy, p_TCB);
  }

  if ((os_rdy.p_lnk != NULL) && (os_rdy.p_lnk->prio > os_tsk.run->prio)) {
    /* preempt running task */
    rt_put_prio (&os_rdy, os_tsk.run);
    os_tsk.run->state = READY;
    rt_dispatch (NULL);
  }

  p_MCB->cb_type = 0U;

  return (OS_R_OK);
}
#endif


/*--------------------------- rt_mut_release --------------------------------*/

OS_RESULT rt_mut_release (OS_ID mutex) {
  /* Release a mutex object */
  P_MUCB p_MCB = mutex;
  P_TCB  p_TCB;
  P_MUCB p_mlnk;
  U8     prio;

  if ((p_MCB->level == 0U) || (p_MCB->owner != os_tsk.run)) {
    /* Unbalanced mutex release or task is not the owner */
    return (OS_R_NOK);
  }
  if (--p_MCB->level != 0U) {
    return (OS_R_OK);
  }

  /* Remove mutex from task mutex owner list. */
  p_mlnk = os_tsk.run->p_mlnk;
  if (p_mlnk == p_MCB) {
    os_tsk.run->p_mlnk = p_MCB->p_mlnk;
  }
  else {
    while (p_mlnk) {
      if (p_mlnk->p_mlnk == p_MCB) {
        p_mlnk->p_mlnk = p_MCB->p_mlnk;
        break;
      }
      p_mlnk = p_mlnk->p_mlnk;
    }
  }

  /* Restore owner task's priority. */
  prio = os_tsk.run->prio_base;
  p_mlnk = os_tsk.run->p_mlnk;
  while (p_mlnk) {
    if ((p_mlnk->p_lnk != NULL) && (p_mlnk->p_lnk->prio > prio)) {
      /* A task with higher priority is waiting for mutex. */
      prio = p_mlnk->p_lnk->prio;
    }
    p_mlnk = p_mlnk->p_mlnk;
  }
  os_tsk.run->prio = prio;

  if (p_MCB->p_lnk != NULL) {
    /* A task is waiting for mutex. */
    p_TCB = rt_get_first ((P_XCB)p_MCB);
#ifdef __CMSIS_RTOS
    rt_ret_val(p_TCB, 0U/*osOK*/);
#else
    rt_ret_val(p_TCB, OS_R_MUT); 
#endif
    rt_rmv_dly (p_TCB);
    /* A waiting task becomes the owner of this mutex. */
    p_MCB->level  = 1U;
    p_MCB->owner  = p_TCB;
    p_MCB->p_mlnk = p_TCB->p_mlnk;
    p_TCB->p_mlnk = p_MCB; 
    /* Priority inversion, check which task continues. */
    if (os_tsk.run->prio >= rt_rdy_prio()) {
      rt_dispatch (p_TCB);
    }
    else {
      /* Ready task has higher priority than running task. */
      rt_put_prio (&os_rdy, os_tsk.run);
      rt_put_prio (&os_rdy, p_TCB);
      os_tsk.run->state = READY;
      p_TCB->state      = READY;
      rt_dispatch (NULL);
    }
  }
  else {
    /* Check if own priority lowered by priority inversion. */
    if (rt_rdy_prio() > os_tsk.run->prio) {
      rt_put_prio (&os_rdy, os_tsk.run);
      os_tsk.run->state = READY;
      rt_dispatch (NULL);
    }
  }
  return (OS_R_OK);
}


/*--------------------------- rt_mut_wait -----------------------------------*/

OS_RESULT rt_mut_wait (OS_ID mutex, U16 timeout) {
  /* Wait for a mutex, continue when mutex is free. */
  P_MUCB p_MCB = mutex;

  if (p_MCB->level == 0U) {
    p_MCB->owner  = os_tsk.run;
    p_MCB->p_mlnk = os_tsk.run->p_mlnk;
    os_tsk.run->p_mlnk = p_MCB; 
    goto inc;
  }
  if (p_MCB->owner == os_tsk.run) {
    /* OK, running task is the owner of this mutex. */
inc:p_MCB->level++;
    return (OS_R_OK);
  }
  /* Mutex owned by another task, wait until released. */
  if (timeout == 0U) {
    return (OS_R_TMO);
  }
  /* Raise the owner task priority if lower than current priority. */
  /* This priority inversion is called priority inheritance.       */
  if (p_MCB->owner->prio < os_tsk.run->prio) {
    p_MCB->owner->prio = os_tsk.run->prio;
    rt_resort_prio (p_MCB->owner);
  }
  if (p_MCB->p_lnk != NULL) {
    rt_put_prio ((P_XCB)p_MCB, os_tsk.run);
  }
  else {
    p_MCB->p_lnk = os_tsk.run;
    os_tsk.run->p_lnk  = NULL;
    os_tsk.run->p_rlnk = (P_TCB)p_MCB;
  }
  rt_block(timeout, WAIT_MUT);
  return (OS_R_TMO);
}


/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/

