/*
 * snwconf.h
 *
 * Created: 20.07.2017 10:22:07
 *  Author: Patrick.Frischmann
 */
#ifndef SNWCONF_H_
#define SNWCONF_H_

#include "PinNames.h"

#define SNWIO_CONF_PIN_RX               Rx0
#define SNWIO_CONF_PIN_TX               Tx0
#define SNWIO_CONF_BAUD                 115200
#define SNWIO_CONF_FRAME_PAUSE_CHARS    0x06
#define SNWIO_CONF_TX_BUFFER_SIZE       512
#define SNWIO_CONF_RX_BUFFER_SIZE       512
#define SNWIO_CONF_RXISR_BUFFER_SIZE    512

#endif /* SNWCONF_H_ */
