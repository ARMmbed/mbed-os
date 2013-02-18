#ifndef TEST_ENV_H_
#define TEST_ENV_H_

#include <stdio.h>
#include "mbed.h"

#define NL     "\n"

void led_blink(void);

void notify_completion(bool success);

#endif
