#include "afsk.h"

void initAFSK() {
  bitStuffCount = 0;
  sending = 0;
  memset(modemBuffer, 0, MAX_BUF_LEN);
  bufCounter = 0;
}

void txAFSK(unsigned char data[], uint16_t len, uint16_t preamble) {
  crc = 0xFFFF;
  sending = 1;
  tone = MARK;

  rda_setDataTX(1);
  _delay_ms(10);

  // send preamble
  for (uint16_t p = 0; p < 30; p++) {
    afskSendByte(0x00, 1);
  }
  for (uint16_t p = 0; p < preamble; p++) {
    afskSendByte(0x7E, 0);
  }

  // send data
  for (uint16_t d = 0; d < len; d++) {
    update_crc(data[d]);
    afskSendByte(data[d], 1);
  }

  // send footer
  uint16_t final_crc = crc;
  afskSendByte(final_crc ^ 0xFF, 1);
  final_crc >>= 8;
  afskSendByte(final_crc ^ 0xFF, 1);
  // send tail
  for (uint16_t t = 0; t < preamble; t++) {
    afskSendByte(0x7e, 0);
  }
  _delay_ms(10);
  rda_setDataTX(0);
  sending = 0;
}

void afskSendFCS(unsigned char byte) {
  int count = 8;
  while (count) {
    if (!(byte & 0x80)) {
      // we have a zero, flip it
      flipTone();
      byte <<= 1;
      count--;
    } else if (bitStuffCount++ >= 5) {
      flipTone();
    } else {
      // we have a one, dont toggle
      byte <<= 1;
      count--;
      bitStuffCount++;
    }

    rda_toneWait(tone, 833);
  }
}

void afskSendByte(unsigned char byte, int stuff) {
  int count = 8;
  // bitStuffCount = 0;
  while (count) {
    if (!(byte & 0x01)) {
      // this is a zero, flip the tone
      flipTone();
      byte >>= 1;
      count--;
    } else if (bitStuffCount++ >= 5 && stuff == 1){
      flipTone();
    } else {
      // current bit is a one, don't toggle
      byte >>= 1;
      count--;
      bitStuffCount++;
    }

    rda_toneWait(tone, 833);
  }
}

void flipTone() {
  bitStuffCount = 0;
  if (tone == MARK)
    tone = SPACE;
  else
    tone = MARK;
}

void update_crc(uint8_t byte) {
  crc ^= byte;
  for (int i = 0; i < 8; i++) {
    if (crc & 0x0001) {
      crc = (crc>>1) ^ 0x8408;
    } else {
      crc >>= 1;
    }
  }
}
