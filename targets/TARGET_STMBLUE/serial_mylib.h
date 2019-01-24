/*
 * serial_mylib.h
 *
 *  Created on: 22 gen 2018
 *      Author: Gianluca
 */

#ifndef MBED_OS_TARGETS_TARGET_STMBLUE_SERIAL_MYLIB_H_
#define MBED_OS_TARGETS_TARGET_STMBLUE_SERIAL_MYLIB_H_

#include "Periph_Driver/inc/BlueNRG1_uart.h"

uint32_t retval;

void myputs(const char *str) {
	while (*str){
		while (UART_GetFlagStatus(UART_FLAG_TXFF) == SET);
		UART_SendData((uint16_t)*str ++);
	}
	UART_SendData((uint16_t) '\n');
	UART_SendData((uint16_t) '\r');
}
void myputc(const char c) {
		while (UART_GetFlagStatus(UART_FLAG_TXFF) == SET);
		UART_SendData((uint16_t) c);
}
void reverse(char *s, int len){
	char *j;
	int c;
	j = s + len - 1;
	while(s < j) {
		c = *s;
		*s++ = *j;
		*j-- = c;
	}
}
/* itoa:  convert n to characters in s */
void itoa(int n, char s[])
{
	int i, sign;
	if ((sign = n) < 0)  /* record sign */
		n = -n;          /* make n positive */
	i = 0;
	do {       /* generate digits in reverse order */
		s[i++] = n % 10 + '0';   /* get next digit */
	} while ((n /= 10) > 0);     /* delete it */
	if (sign < 0)
		s[i++] = '-';
	s[i] = '\0';
	reverse(s,i);
}

void printnumb(int n){
	char s[20];
	itoa(n, s);
	myputs(s);
}


#endif /* MBED_OS_TARGETS_TARGET_STMBLUE_SERIAL_MYLIB_H_ */
