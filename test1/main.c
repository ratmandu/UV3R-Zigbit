#include "main.h"

int main(void) {
    // clock_prescale_set(0);
    CLKPR = (1 << CLKPCE); // enable a change to CLKPR
	  CLKPR = 0; // set the CLKDIV to 0 - was 0011b = div by 8 taking 8MHz to 1MHz
    TOPLED_PORT = (1<<TOPLED_PIN);
    TOPLED_DDR |= (1<<TOPLED_PIN);

    rda_init();
    initAFSK();

    rda_setTxRx(0);

    uint8_t sendBuf[MAX_BUF_LEN];
    memset(sendBuf, 0, MAX_BUF_LEN);
    bufCounter = 0;

    // sendBuf[bufCounter++] = 0x7E;
    memcpy(&sendBuf[bufCounter], "QTH1  0", 7);
    bufCounter += 7;
    memcpy(&sendBuf[bufCounter], "KJ4WTR0", 7);
    bufCounter += 7;
    sendBuf[bufCounter++] = 0x03;
    sendBuf[bufCounter++] = 0xF0;
    sendBuf[bufCounter++] = 0x21;
    memcpy(&sendBuf[bufCounter], "3445.20N/08635.49W-TEST", 23);
    bufCounter += 23;

    _delay_ms(1000);
    rda_setFreq(144390);
    _delay_ms(1000);
    uint16_t timerCounter = 30;

    while (1) {
      TOPLED_PORT ^= (1<<TOPLED_PIN);
      if (timerCounter++ >= 30) {
        txAFSK(sendBuf, bufCounter, 50);
        // rda_sendDigital(sendBuf, bufCounter, 100, MARK, SPACE);
        timerCounter = 0;
      }
      _delay_ms(1000);
    }

    return 1;
}
