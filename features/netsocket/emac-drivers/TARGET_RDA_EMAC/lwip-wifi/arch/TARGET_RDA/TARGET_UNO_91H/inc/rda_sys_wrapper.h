/* Copyright (c) 2019 Unisoc Communications Inc.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if 1
#ifndef _RDA_SYS_WRAPPER_H_
#define _RDA_SYS_WRAPPER_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Alarm */
/**
 * @brief     : get current time in units of micro second
 * @param[in] :
 * @param[out]:
 * @return    : return time value with uint32 type
 */
extern unsigned long rda_get_cur_time_ms(void);

/**
 * @brief     : create an alarm with given function, return timer handle
 * @param[in] : func(callback)/data(pass to func)/mode(once or periodic)
 * @param[out]:
 * @return    : return timer handle, a pointer to the timer structure, non-zero is valid
 */
extern void * rda_alarm_create_v2(void *func, unsigned int data, unsigned int mode);
extern void * rda_alarm_create(void *func, unsigned int data);

/**
 * @brief     : delete an alarm with given handle, then reset the handle
 * @param[in] : *handle(pointer to the timer structure)
 * @param[out]: **handle(address of the handle variable)
 * @return    :
 */
extern int rda_alarm_delete(void **handle);

/**
 * @brief     : start an alarm, raise a function call after given timeout delay
 * @param[in] : handle(pointer to the timer structure)/timeout(micro second)
 * @param[out]:
 * @return    :
 */
extern int rda_alarm_start(void *handle, unsigned int timeout_ms);

/**
 * @brief     : stop an alarm, will not raise a function call any more
 * @param[in] : handle(pointer to the timer structure)
 * @param[out]:
 * @return    :
 */
extern int rda_alarm_stop(void *handle);


/* Semaphore */
/**
 * @brief     : create a semaphore
 * @param[in] : name and initial valve of semaphore
 * @param[out]:
 * @return    : return ERR or NO_ERR
 */
extern void* rda_sem_create(unsigned int count);

/**
 * @brief     : wait a semaphore
 * @param[in] : name of semaphore
 * @param[out]:
 * @return    : return ERR or NO_ERR
 */
extern int rda_sem_wait(void *sem, unsigned int millisec);

/**
 * @brief     : release a semaphore
 * @param[in] : name of semaphore
 * @param[out]:
 * @return    : return ERR or NO_ERR
 */
extern int rda_sem_release(void *sem);

/**
 * @brief     : delete a semaphore
 * @param[in] : name of semaphore
 * @param[out]:
 * @return    : return ERR or NO_ERR
 */
extern int rda_sem_delete(void *sem);


/* Queue */
/**
 * @brief     : create a message queue
 * @param[in] : size of message queue
 * @param[out]:
 * @return    : return message queue id or NULL if error
 */
extern void* rda_msgQ_create(unsigned int queuesz);

/**
 * @brief     : put a message to queue
 * @param[in] : message queue id, message value and wait time
 * @param[out]:
 * @return    : return ERR or NO_ERR
 */
extern int rda_msg_put(void *msgQId, unsigned int msg, unsigned int millisec);

/**
 * @brief     : get a message from queue
 * @param[in] : message queue id, message value and wait time
 * @param[out]:
 * @return    : return ERR or NO_ERR
 */
extern int rda_msg_get(void *msgQId, unsigned int *value, unsigned int millisec);

/* Mail */
/**
 * @brief     : create a mail
 * @param[in] : mail count/size
 * @param[out]:
 * @return    : return mail handle
 */
void* rda_mail_create(unsigned int msgcnt, unsigned int msgsize);

/**
 * @brief     : get a msg from mail
 * @param[in] : handler name of mail/mail/wait time
 * @param[out]:
 * @return    : return ERR or NO_ERR
 */
int rda_mail_get(void *rdahandle, void *evt, unsigned int wait);

/**
 * @brief     : put a msg to mail
 * @param[in] : handler of mail/mail/wait time
 * @param[out]:
 * @return    : return ERR or NO_ERR
 */

int rda_mail_put(void *rdahandle, void *evt, unsigned int wait);

/* Mutex */
/**
 * @brief     : create a mutex
 * @param[in] :
 * @param[out]:
 * @return    : return ERR or NO_ERR
 */
extern void* rda_mutex_create(void);

/**
 * @brief     : wait a mutex
 * @param[in] : id of mutex and wait time
 * @param[out]:
 * @return    : return ERR or NO_ERR
 */
extern int rda_mutex_wait(void *rdamutex, unsigned int millisec);

/**
 * @brief     : release a mutex
 * @param[in] : id of mutex
 * @param[out]:
 * @return    : return ERR or NO_ERR
 */
extern int rda_mutex_realease(void *rdamutex);

/**
 * @brief     : delete a mutex
 * @param[in] : id of mutex
 * @param[out]:
 * @return    : return ERR or NO_ERR
 */
extern int rda_mutex_delete(void *rdamutex);

/* Thread */
/**
 * @brief     : creat a thread
 * @param[in] : thread name/thread function/thread fuction argument/stacksize/thread priority
 * @param[out]:
 * @return    : return thread id
 */
void* rda_thread_new(const char *pcName, void (*thread)(void *arg), void *arg, int stacksize, int priority);

/**
 * @brief     : delete a thread
 * @param[in] : thread id
 * @param[out]:
 * @return    : return ERR or NO_ERR
 */
int rda_thread_delete(void* id);

/**
 * @brief     : get current thread id
 * @param[in] :
 * @param[out]:
 * @return    : return thread id
 */
void* rda_thread_get_id(void);

#ifdef __cplusplus
}
#endif

#endif /* _RDA_SYS_WRAPPER_H_ */
#endif
