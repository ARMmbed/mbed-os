/*
 * BitIO.h
 *
 * Copyright (C) 2017 world-direct.at, MIT License
 *
 * Created:	15.05.2017
 * Author:	Patrick Frischmann
 * EMail:	patrick.frischmann@world-direct.at
 *
 * Description:
 *        Library for simple bit operations
 *
 */ 

#include <inttypes.h>

#ifndef BITIO_H_
#define BITIO_H_

// get byte value
static inline uint8_t BIT_VALUE(uint8_t bit) __attribute__((always_inline));
static inline uint8_t BIT_VALUE(uint8_t bit){
	return (1<<bit);
};

// set bit
static inline void BIT_SET(volatile uint8_t *target, uint8_t bit) __attribute__((always_inline));
static inline void BIT_SET(volatile uint8_t *target, uint8_t bit){
	*target |= (1<<bit);
};

// set clear
static inline void BIT_CLEAR(volatile uint8_t *target, uint8_t bit) __attribute__((always_inline));
static inline void BIT_CLEAR(volatile uint8_t *target, uint8_t bit){
	*target &= ~(1<<bit);
};

// bit toogle
static inline void BIT_TOGGLE(volatile uint8_t *target, uint8_t bit) __attribute__((always_inline));
static inline void BIT_TOGGLE(volatile uint8_t *target, uint8_t bit){
	*target ^= (1<<bit);
};

// set bit by boolean
static inline void BIT_BOOL_SET(volatile uint8_t *target, uint8_t bit, bool enable) __attribute__((always_inline));
static inline void BIT_BOOL_SET(volatile uint8_t *target, uint8_t bit, bool enable){
	if (enable){
		BIT_SET(target, bit);
	}else{
		BIT_CLEAR(target, bit);
	}
};

// check bit
static inline bool BIT_CHECK(volatile uint8_t *target, uint8_t bit) __attribute__((always_inline));
static inline bool BIT_CHECK(volatile uint8_t *target, uint8_t bit){
	return (*target & (1<<bit));
};

#endif /* BITIO_H_ */