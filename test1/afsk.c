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

  rda_setDataTX(1);
  _delay_ms(100);

  // send preamble
  for (uint16_t p = 0; p < preamble; p++) {
    afskSendByte(0x7e, 0);
  }

  // send data
  for (uint16_t d = 0; d < len; d++) {
    update_crc(data[d]);
    afskSendByte(data[d], 1);
  }

  // send footer
  uint16_t final_crc = crc;
  afskSendByte(~(final_crc & 0xFF), 0);
  final_crc >>= 8;
  afskSendByte(~(final_crc & 0xFF), 0);
  // send tail
  for (uint16_t t = 0; t < preamble; t++) {
    afskSendByte(0x7e, 0);
  }
  _delay_ms(100);
  rda_setDataTX(0);
  sending = 0;
}

void afskSendByte(unsigned char byte, int stuff) {
  for (unsigned char b = 0; b < 7; b++) {
    if (byte & 0x01) {
      // no tone change
      bitStuffCount++;
      if (bitStuffCount > BIT_STUFF_LEN && stuff == 1) {
        flipTone();
        // rda_toneWait(tone, 833);
      }
      // rda_toneWait(tone, 850);
    } else {
      flipTone();
    }

    rda_toneWait(tone, 850);
    byte >>= 1;
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
  uint8_t bit;
  for (int i = 0; i < 8; i++) {
    bit = byte & 0x01;
    byte >>= 1;

    crc ^= bit;
    if (crc & 1) {
      crc = (crc >> 1) ^ 0x8408;
    } else {
      crc = crc >> 1;
    }
  }
}
