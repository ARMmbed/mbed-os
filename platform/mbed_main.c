/* mbed Microcontroller Library
 * Copyright (c) 2006-2016 ARM Limited
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

#include "mbed_rtx.h"
#include "cmsis_os.h"
#include "cmsis_compiler.h"

extern void __libc_init_array (void);
extern int main(int argc, char **argv);

osMutexId_t singleton_mutex_id;
osMutexId_t malloc_mutex_id;
osMutexId_t env_mutex_id;

osThreadAttr_t _main_thread_attr;
char _main_stack[DEFAULT_STACK_SIZE] __ALIGNED(8);
char _main_obj[sizeof(os_thread_t)];

void pre_main(void)
{
  singleton_mutex_id = osMutexNew(NULL);
  malloc_mutex_id = osMutexNew(NULL);
  env_mutex_id = osMutexNew(NULL);

  __libc_init_array();

  main(0, NULL);
}

void mbed_start_main(void)
{
  _main_thread_attr.stack_mem = _main_stack;
  _main_thread_attr.stack_size = sizeof(_main_stack);
  _main_thread_attr.cb_size = sizeof(_main_obj);
  _main_thread_attr.cb_mem = _main_obj;
  _main_thread_attr.priority = osPriorityNormal;
  _main_thread_attr.name = "MAIN";
  osThreadNew((os_thread_func_t)pre_main, NULL, &_main_thread_attr);    // Create application main thread

  osKernelStart();                                                      // Start thread execution
}
