/*
 * snwconf.h
 *
 * Created: 04.07.2017 13:20:46
 *  Author: Guenter.Prossliner
 */


#ifndef SNWCONF_H_
#define SNWCONF_H_

#include "PinNames.h"

#define SNWIO_CONF_PIN_RX               BUS_RxD0
#define SNWIO_CONF_PIN_TX               BUS_TxD0
#define SNWIO_CONF_BAUD                 115200
#define SNWIO_CONF_FRAME_PAUSE_CHARS    0x06
#define SNWIO_CONF_TX_BUFFER_SIZE       512
#define SNWIO_CONF_RX_BUFFER_SIZE       512
#define SNWIO_CONF_RXISR_BUFFER_SIZE    512

#endif /* SNWCONF_H_ */