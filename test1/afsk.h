#ifndef _AFSK_H_
#define _AFSK_H_

#include <avr/io.h>
#include <string.h>
#include <util/delay.h>
// #include "rda1846.h"

// #define MARK 1.200
// #define SPACE 2.200
#define MARK 4915
#define SPACE 9011

#define BIT_STUFF_LEN 5
#define MAX_BUF_LEN 1024

uint8_t modemBuffer[MAX_BUF_LEN];
uint16_t bufCounter;

int bitStuffCount;
int sending;
uint16_t tone;
uint16_t crc;

void initAFSK();
void txAFSK(unsigned char data[], uint16_t len, uint16_t preamble);
void afskSendByte(unsigned char byte, int stuff);
void flipTone();
void update_crc(uint8_t byte);

#endif
