/*
 * Common.h
 *
 * Created: 16.02.2017 12:16:40
 *  Author: Dima
 */ 


#ifndef COMMON_H_
#define COMMON_H_

//#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>


#define uint unsigned int
#define uchar unsigned char

#define FlagSet(f) flags |= f
#define FlagClear(f) flags &= (~f)
#define IsFlag(f) (flags & f)

#define WritePortBit(port, bit, val) {val ? (port |= (1 << bit)) : (port &= ~(1 << bit));}


#endif /* COMMON_H_ */