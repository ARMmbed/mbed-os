#ifndef TEST_ENV_H_
#define TEST_ENV_H_

#include <stdio.h>
#include "mbed.h"

#define NL "\n"

// Test result related notification functions
void led_blink(PinName led, float delay);
void notify_start();
void notify_completion(bool success);

// Test functionality useful during testing
unsigned int testenv_randseed();

#endif
