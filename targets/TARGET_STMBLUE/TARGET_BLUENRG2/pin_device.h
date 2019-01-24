/*
 * pin_device.h
 *
 *  Created on: 30 ott 2018
 *      Author: Antonio O.
 */

#ifndef MBED_OS_TARGETS_TARGET_STMBLUE_PIN_DEVICE_H_
#define MBED_OS_TARGETS_TARGET_STMBLUE_PIN_DEVICE_H_

//#include "BlueNRG_x_device.h"
#include "BlueNRG1_gpio.h"
#include "BlueNRG1_sysCtrl.h"
#include "PinNames.h"

uint32_t getGpioPin(PinName);

#endif /* MBED_OS_TARGETS_TARGET_STMBLUE_PIN_DEVICE_H_ */
