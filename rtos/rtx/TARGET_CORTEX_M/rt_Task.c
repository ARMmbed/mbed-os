/*----------------------------------------------------------------------------
 *      CMSIS-RTOS  -  RTX
 *----------------------------------------------------------------------------
 *      Name:    RT_TASK.C
 *      Purpose: Task functions and system start up.
 *      Rev.:    V4.80
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
#include "rt_System.h"
#include "rt_Task.h"
#include "rt_List.h"
#include "rt_MemBox.h"
#include "rt_Robin.h"
#include "rt_HAL_CM.h"
#include "rt_OsEventObserver.h"

/*----------------------------------------------------------------------------
 *      Global Variables
 *---------------------------------------------------------------------------*/

/* Running and next task info. */
struct OS_TSK os_tsk;

/* Task Control Blocks of idle demon */
struct OS_TCB os_idle_TCB;


/*----------------------------------------------------------------------------
 *      Local Functions
 *---------------------------------------------------------------------------*/

static OS_TID rt_get_TID (void) {
  U32 tid;

  for (tid = 1U; tid <= os_maxtaskrun; tid++) {
    if (os_active_TCB[tid-1U] == NULL) {
      return ((OS_TID)tid);
    }
  }
  return (0U);
}


/*--------------------------- rt_init_context -------------------------------*/

static void rt_init_context (P_TCB p_TCB, U8 priority, FUNCP task_body) {
  /* Initialize general part of the Task Control Block. */
  p_TCB->cb_type   = TCB;
  p_TCB->state     = READY;
  p_TCB->prio      = priority;
  p_TCB->prio_base = priority;
  p_TCB->p_lnk     = NULL;
  p_TCB->p_rlnk    = NULL;
  p_TCB->p_dlnk    = NULL;
  p_TCB->p_blnk    = NULL;
  p_TCB->p_mlnk    = NULL;
  p_TCB->delta_time    = 0U;
  p_TCB->interval_time = 0U;
  p_TCB->events  = 0U;
  p_TCB->waits   = 0U;
  p_TCB->stack_frame = 0U;

  if (p_TCB->priv_stack == 0U) {
    /* Allocate the memory space for the stack. */
    p_TCB->stack = rt_alloc_box (mp_stk);
  }
  rt_init_stack (p_TCB, task_body);
}


/*--------------------------- rt_switch_req ---------------------------------*/

void rt_switch_req (P_TCB p_new) {
  /* Switch to next task (identified by "p_new"). */
  os_tsk.new_tsk   = p_new;
  p_new->state = RUNNING;
  if (osEventObs && osEventObs->thread_switch) {
    osEventObs->thread_switch(p_new->context);
  }
  DBG_TASK_SWITCH(p_new->task_id);
}


/*--------------------------- rt_dispatch -----------------------------------*/

void rt_dispatch (P_TCB next_TCB) {
  /* Dispatch next task if any identified or dispatch highest ready task    */
  /* "next_TCB" identifies a task to run or has value NULL (=no next task)  */
  if (next_TCB == NULL) {
    /* Running task was blocked: continue with highest ready task */
    next_TCB = rt_get_first (&os_rdy);
    rt_switch_req (next_TCB);
  }
  else {
    /* Check which task continues */
    if (next_TCB->prio > os_tsk.run->prio) {
      /* preempt running task */
      rt_put_rdy_first (os_tsk.run);
      os_tsk.run->state = READY;
      rt_switch_req (next_TCB);
    }
    else {
      /* put next task into ready list, no task switch takes place */
      next_TCB->state = READY;
      rt_put_prio (&os_rdy, next_TCB);
    }
  }
}


/*--------------------------- rt_block --------------------------------------*/

void rt_block (U16 timeout, U8 block_state) {
  /* Block running task and choose next ready task.                         */
  /* "timeout" sets a time-out value or is 0xffff (=no time-out).           */
  /* "block_state" defines the appropriate task state */
  P_TCB next_TCB;

  if (timeout) {
    if (timeout < 0xFFFFU) {
      rt_put_dly (os_tsk.run, timeout);
    }
    os_tsk.run->state = block_state;
    next_TCB = rt_get_first (&os_rdy);
    rt_switch_req (next_TCB);
  }
}


/*--------------------------- rt_tsk_pass -----------------------------------*/

void rt_tsk_pass (void) {
  /* Allow tasks of same priority level to run cooperatively.*/
  P_TCB p_new;

  p_new = rt_get_same_rdy_prio();
  if (p_new != NULL) {
    rt_put_prio ((P_XCB)&os_rdy, os_tsk.run);
    os_tsk.run->state = READY;
    rt_switch_req (p_new);
  }
}


/*--------------------------- rt_tsk_self -----------------------------------*/

OS_TID rt_tsk_self (void) {
  /* Return own task identifier value. */
  if (os_tsk.run == NULL) {
    return (0U);
  }
  return ((OS_TID)os_tsk.run->task_id);
}


/*--------------------------- rt_tsk_prio -----------------------------------*/

OS_RESULT rt_tsk_prio (OS_TID task_id, U8 new_prio) {
  /* Change execution priority of a task to "new_prio". */
  P_TCB p_task;

  if (task_id == 0U) {
    /* Change execution priority of calling task. */
    os_tsk.run->prio      = new_prio;
    os_tsk.run->prio_base = new_prio;
run:if (rt_rdy_prio() > new_prio) {
      rt_put_prio (&os_rdy, os_tsk.run);
      os_tsk.run->state   = READY;
      rt_dispatch (NULL);
    }
    return (OS_R_OK);
  }

  /* Find the task in the "os_active_TCB" array. */
  if ((task_id > os_maxtaskrun) || (os_active_TCB[task_id-1U] == NULL)) {
    /* Task with "task_id" not found or not started. */
    return (OS_R_NOK);
  }
  p_task = os_active_TCB[task_id-1U];
  p_task->prio      = new_prio;
  p_task->prio_base = new_prio;
  if (p_task == os_tsk.run) {
    goto run;
  }
  rt_resort_prio (p_task);
  if (p_task->state == READY) {
    /* Task enqueued in a ready list. */
    p_task = rt_get_first (&os_rdy);
    rt_dispatch (p_task);
  }
  return (OS_R_OK);
}


/*--------------------------- rt_tsk_create ---------------------------------*/

OS_TID rt_tsk_create (FUNCP task, U32 prio_stksz, void *stk, void *argv) {
  /* Start a new task declared with "task". */
  P_TCB task_context;
  U32 i;

  /* Priority 0 is reserved for idle task! */
  if ((prio_stksz & 0xFFU) == 0U) {
    prio_stksz += 1U;
  }
  task_context = rt_alloc_box (mp_tcb);
  if (task_context == NULL) {
    return (0U);
  }
  /* If "size != 0" use a private user provided stack. */
  task_context->stack      = stk;
  task_context->priv_stack = prio_stksz >> 8;

  /* Find a free entry in 'os_active_TCB' table. */
  i = rt_get_TID ();
  if (i == 0U) {
    return (0U);
  }
  task_context->task_id = (U8)i;
  /* Pass parameter 'argv' to 'rt_init_context' */
  task_context->msg = argv;
  task_context->argv = argv;
  /* For 'size == 0' system allocates the user stack from the memory pool. */
  rt_init_context (task_context, (U8)(prio_stksz & 0xFFU), task);

  os_active_TCB[i-1U] = task_context;
  DBG_TASK_NOTIFY(task_context, __TRUE);
  rt_dispatch (task_context);
  return ((OS_TID)i);
}


/*--------------------------- rt_tsk_delete ---------------------------------*/

OS_RESULT rt_tsk_delete (OS_TID task_id) {
  /* Terminate the task identified with "task_id". */
  P_TCB  task_context;
  P_TCB  p_TCB;
  P_MUCB p_MCB, p_MCB0;

  if ((task_id == 0U) || (task_id == os_tsk.run->task_id)) {
    /* Terminate itself. */
    os_tsk.run->state     = INACTIVE;
    os_tsk.run->tsk_stack = rt_get_PSP ();
    rt_stk_check ();
    p_MCB = os_tsk.run->p_mlnk;
    while (p_MCB) {
      /* Release mutexes owned by this task */
      if (p_MCB->p_lnk) {
        /* A task is waiting for mutex. */
        p_TCB = rt_get_first ((P_XCB)p_MCB);
#ifdef __CMSIS_RTOS
        rt_ret_val (p_TCB, 0U/*osOK*/);
#else
        rt_ret_val (p_TCB, OS_R_MUT); 
#endif
        rt_rmv_dly (p_TCB);
        p_TCB->state = READY;
        rt_put_prio (&os_rdy, p_TCB);
        /* A waiting task becomes the owner of this mutex. */
        p_MCB0 = p_MCB->p_mlnk;
        p_MCB->level  = 1U;
        p_MCB->owner  = p_TCB;
        p_MCB->p_mlnk = p_TCB->p_mlnk;
        p_TCB->p_mlnk = p_MCB; 
        p_MCB = p_MCB0;
      }
      else {
        p_MCB0 = p_MCB->p_mlnk;
        p_MCB->level  = 0U;
        p_MCB->owner  = NULL;
        p_MCB->p_mlnk = NULL;
        p_MCB = p_MCB0;
      }
    }
    os_active_TCB[os_tsk.run->task_id-1U] = NULL;
    rt_free_box (mp_stk, os_tsk.run->stack);
    os_tsk.run->stack = NULL;
    DBG_TASK_NOTIFY(os_tsk.run, __FALSE);
    rt_free_box (mp_tcb, os_tsk.run);
    os_tsk.run = NULL;
    rt_dispatch (NULL);
    /* The program should never come to this point. */
  }
  else {
    /* Find the task in the "os_active_TCB" array. */
    if ((task_id > os_maxtaskrun) || (os_active_TCB[task_id-1U] == NULL)) {
      /* Task with "task_id" not found or not started. */
      return (OS_R_NOK);
    }
    task_context = os_active_TCB[task_id-1U];
    rt_rmv_list (task_context);
    rt_rmv_dly (task_context);
    p_MCB = task_context->p_mlnk;
    while (p_MCB) {
      /* Release mutexes owned by this task */
      if (p_MCB->p_lnk) {
        /* A task is waiting for mutex. */
        p_TCB = rt_get_first ((P_XCB)p_MCB);
#ifdef __CMSIS_RTOS
        rt_ret_val (p_TCB, 0U/*osOK*/);
#else
        rt_ret_val (p_TCB, OS_R_MUT); 
#endif
        rt_rmv_dly (p_TCB);
        p_TCB->state = READY;
        rt_put_prio (&os_rdy, p_TCB);
        /* A waiting task becomes the owner of this mutex. */
        p_MCB0 = p_MCB->p_mlnk;
        p_MCB->level  = 1U;
        p_MCB->owner  = p_TCB;
        p_MCB->p_mlnk = p_TCB->p_mlnk;
        p_TCB->p_mlnk = p_MCB; 
        p_MCB = p_MCB0;
      }
      else {
        p_MCB0 = p_MCB->p_mlnk;
        p_MCB->level  = 0U;
        p_MCB->owner  = NULL;
        p_MCB->p_mlnk = NULL;
        p_MCB = p_MCB0;
      }
    }
    os_active_TCB[task_id-1U] = NULL;
    rt_free_box (mp_stk, task_context->stack);
    task_context->stack = NULL;
    DBG_TASK_NOTIFY(task_context, __FALSE);
    rt_free_box (mp_tcb, task_context);
    if (rt_rdy_prio() > os_tsk.run->prio) {
      /* Ready task has higher priority than running task. */
      os_tsk.run->state = READY;
      rt_put_prio (&os_rdy, os_tsk.run);
      rt_dispatch (NULL);
    }
  }
  return (OS_R_OK);
}


/*--------------------------- rt_sys_init -----------------------------------*/

#ifdef __CMSIS_RTOS
void rt_sys_init (void) {
#else
void rt_sys_init (FUNCP first_task, U32 prio_stksz, void *stk) {
#endif
  /* Initialize system and start up task declared with "first_task". */
  U32 i;

  DBG_INIT();

  /* Initialize dynamic memory and task TCB pointers to NULL. */
  for (i = 0U; i < os_maxtaskrun; i++) {
    os_active_TCB[i] = NULL;
  }
  rt_init_box (mp_tcb, (U32)mp_tcb_size, sizeof(struct OS_TCB));
  rt_init_box (mp_stk, mp_stk_size, BOX_ALIGN_8 | (U16)(os_stackinfo));
  rt_init_box ((U32 *)m_tmr, (U32)mp_tmr_size, sizeof(struct OS_TMR));

  /* Set up TCB of idle demon */
  os_idle_TCB.task_id    = 255U;
  os_idle_TCB.priv_stack = 0U;
  rt_init_context (&os_idle_TCB, 0U, os_idle_demon);

  /* Set up ready list: initially empty */
  os_rdy.cb_type = HCB;
  os_rdy.p_lnk   = NULL;
  /* Set up delay list: initially empty */
  os_dly.cb_type = HCB;
  os_dly.p_dlnk  = NULL;
  os_dly.p_blnk  = NULL;
  os_dly.delta_time = 0U;

  /* Fix SP and system variables to assume idle task is running */
  /* Transform main program into idle task by assuming idle TCB */
#ifndef __CMSIS_RTOS
  rt_set_PSP (os_idle_TCB.tsk_stack+32U);
#endif
  os_tsk.run = &os_idle_TCB;
  os_tsk.run->state = RUNNING;

  /* Set the current thread to idle, so that on exit from this SVCall we do not
   * de-reference a NULL TCB. */
  rt_switch_req(&os_idle_TCB);

  /* Initialize ps queue */
  os_psq->first = 0U;
  os_psq->last  = 0U;
  os_psq->size  = os_fifo_size;

  rt_init_robin ();

#ifndef __CMSIS_RTOS
  /* Initialize SVC and PendSV */
  rt_svc_init ();

  /* Initialize and start system clock timer */
  os_tick_irqn = os_tick_init ();
  if (os_tick_irqn >= 0) {
    OS_X_INIT((U32)os_tick_irqn);
  }

  /* Start up first user task before entering the endless loop */
  rt_tsk_create (first_task, prio_stksz, stk, NULL);
#endif
}


/*--------------------------- rt_sys_start ----------------------------------*/

#ifdef __CMSIS_RTOS
void rt_sys_start (void) {
  /* Start system */

  /* Initialize SVC and PendSV */
  rt_svc_init ();

  /* Initialize and start system clock timer */
  os_tick_irqn = os_tick_init ();
  if (os_tick_irqn >= 0) {
    OS_X_INIT((U32)os_tick_irqn);
  }
}
#endif

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
