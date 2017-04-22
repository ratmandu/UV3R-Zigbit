#include "rda1846.h"

void rda_init() {
    RDA_PORT |= (1<<RDA_SEN);
    RDA_DDR  |= (1<<RDA_SEN) | (1<<RDA_CLK) | (1<<RDA_DIO);
    int i;
    for (i=0; i<30; i++) {
        rda_write(initData[i].address, initData[i].data);
    }
}

void rda_write(uint8_t addr, uint16_t data) {
    int i;
    RDA_PORT &= ~(1<<RDA_SEN);
    RDA_PORT &= ~(1<<RDA_CLK);
    uint8_t read = addr & 0x80;

    for (i=8; i; i--) {
        if (addr & 0x80)
            RDA_PORT |= (1<<RDA_DIO);
        else
            RDA_PORT &= ~(1<<RDA_DIO);

        RDA_PORT &= ~(1<<RDA_CLK);
        RDA_PORT |= (1<<RDA_CLK);

        addr <<= 1;
    }

    if (read) {
        return;
    } else {
        for (i = 16; i; i--) {
            RDA_PORT &= ~(1<<RDA_CLK);

            if (data & 0x8000)
                RDA_PORT |= (1<<RDA_DIO);
            else 
                RDA_PORT &= ~(1<<RDA_DIO);

            RDA_PORT &= ~(1<<RDA_CLK);
            RDA_PORT |= (1<<RDA_CLK);

            data <<= 1;
        }
    }

    RDA_PORT |= (1<<RDA_SEN);
    RDA_PORT &= ~(1<<RDA_CLK);
}

void rda_setFreq(uint32_t freq) {

}

void rda_setBand(uint8_t band) {

}

void rda_sendRadioSPI() {
    uint8_t callData[6] = {0x4B, 0x4D, 0x34, 0x4B, 0x45, 0x52};

    rda_write(0x3C, 0x4958);
    rda_write(0x1F, 0xCEB8);
    rda_write(0x30, 0x3046);

    rda_write(0x36, 0);
    rda_write(0x35, 0);

    _delay_ms(500);

    uint16_t clock = 5796;
    uint16_t data = 6492;
    uint8_t callLetter;

    int i,j,k;
    for (k = 0; i<5; i++) {
        for (i = 0; i<6; i++) {
            callLetter = callData[i];       
            for (i=0; i<8; i++) {
                if (callLetter & 0x01) {
                    rda_write(0x35, data);
                } else {
                    rda_write(0x35, 0);
                }
                // clock it
                rda_write(0x36, clock);
                _delay_us(10);
                rda_write(0x36, 0);
                callLetter >>= 1;
            }
        }
    }

    rda_write(0x36, 0);
    rda_write(0x35, 0);
    rda_setTxRx(0);
}

void rda_setRef() {

    //char callSign[6] = {"K", "J", "4", "W", "T", "R"};
    uint8_t callData[7] = {0x7e, 0x96, 0x94, 0x68, 0xae, 0xa8, 0xa4};
    uint8_t dotData[6] = {0x2, 0x8, 0x1E, 0x04, 0x00, 0x05};
    uint8_t ditData[6] = {0x05, 0x07, 0x01, 0x03, 0x01, 0x02};
    // 101 0111 00001 011 1 010

    uint16_t hellData[6] = {0x7992, 0x185E, 0x709E, 0x78de, 0x43d0, 0x7a9a}; 

    rda_write(0x3C, 0x4958);
    rda_write(0x1F, 0xCEB8);
    rda_write(0x30, 0x3046);

    rda_write(0x36, 0);
    _delay_ms(500);
    uint16_t mark = 5796;
    uint16_t space = 6492;
    int i,j,k;
    int ones = 0;
    for (k=0; k<2; k++) {
    for (i = 0; i<6; i++) {
    /*    uint8_t callLetter = callData[i];
        callLetter <<= 1;
        uint16_t tone = mark;
        for (j=0; j<7; j++) {
            if (!callLetter & 0x01) {
                tone = (tone == mark)? space : mark;
                rda_write(0x36, tone);
            } else {
                if (++ones == 5) {
                    _delay_us(8333);
                    tone = (tone == mark)? space : mark;
                    rda_write(0x36, tone);
                    ones = 0;
                }
            }
            callLetter <<= 1;
            _delay_us(8333);
        }*/
        ditData[i] <<= 3;
        dotData[i] <<= 3;
        for (j=5; j; j--) {

            if (ditData[i] & 0x80) {
                rda_write(0x36, mark);
                _delay_ms(180);
            } else if (dotData[i] & 0x80) {
                rda_write(0x36, mark);
                _delay_ms(60);
            } else {
                rda_write(0x36, 0);
            }
            
            rda_write(0x36, 0);

            if (ditData[i] & 0x80 || dotData[i] & 0x80) {
                _delay_ms(60);
            }

            dotData[i] <<= 1;
            ditData[i] <<= 1;
            //_delay_ms(30);
        }
        _delay_ms(120);
        
    }
    }
    rda_write(0x36, 0);
    rda_setTxRx(0);
}


void rda_setChannelMode(uint8_t channelMode) {

}

void rda_setTxRx(uint8_t tx) {
    if (tx) {
        rda_write(0x3C, 0x0958);
        rda_write(0x1F, 0xC500);
        rda_write(0x30, 0x3046);
    } else {
        rda_write(0x1F, 0x0000);
        rda_write(0x30, 0x302E);
    }
}

void rda_setSleep(uint8_t sleep) {

}

void rda_setTxVoiceChannel(uint8_t voiceSel) {

}

void rda_setBiasVoltage(uint8_t biasVoltage) {

}

void rda_setCTCSSFreq(uint16_t freq) {

}

void rda_setCTCSSMode(uint8_t tonemode) {

}

uint32_t rda_read(uint8_t addr) {

}


