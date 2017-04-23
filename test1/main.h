#ifndef _MAIN_H_
#define _MAIN_H_

#include <avr/io.h>
#include <util/delay.h>
#include <avr/power.h>

#include "afsk.h"
//#include "rda1846.h"

#define TOPLED_PORT PORTB
#define TOPLED_DDR  DDRB
#define TOPLED_PIN  PB7

int main(void);

#endif
