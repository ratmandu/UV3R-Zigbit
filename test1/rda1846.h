#ifndef RDA1846_H_
#define RDA1846_H_

#include <avr/io.h>
#include <util/delay.h>

#define RDA_PORT PORTD
#define RDA_DDR  DDRD

#define RDA_CLK  PD0
#define RDA_DIO  PD1
#define RDA_SEN  PD4

#define BAND2M 0x6BE4
#define BAND125M 0x6BC4
#define BAND70CM 0x6B84

enum voiceChannel {
    tx_chan_mic = 0,
    tx_chan_tone = 1,
    tx_chan_gpio1 = 2,
    tx_chan_notx = 3
};

enum channelMode {
    chan_mode_wide = 3,
    chan_mode_narrow = 0,
};

enum bandSelection {
    band_select_2m = 3,
    band_select_125m = 2,
    band_select_70cm = 0
};

enum biasOutputVoltage {
    pabias_101v = 0,
    pabias_105v = 1,
    pabias_109v = 2,
    pabias_118v = 4,
    pabias_134v = 8,
    pabias_168v = 16,
    pabias_245v = 32,
    pabias_313v = 127
};

enum ctcssMode {
    toneMode_disabled = 0,
    toneMode_inner_ctcss_en = 1,
    toneMode_inner_cdcss_en = 2,
    toneMode_outer_ctcss_en = 5,
    toneMode_outer_cdcss_en = 6
};

typedef struct {
    uint8_t address;
    uint16_t data;
} dataFormat_t;

dataFormat_t initData[31] = {
 { 0x30, 0x0001 },
 { 0x30, 0x0004 },
 { 0x04, 0x0FD0 },
 { 0x0B, 0x1A10 },
 { 0x2B, 0x32C8 },
 { 0x2C, 0x1964 },
 { 0x32, 0x627C },
 { 0x33, 0x0AF2 },
 { 0x47, 0x2C2F },
 { 0x4E, 0x293A },
 { 0x54, 0x1D4C },
 { 0x56, 0x0652 },
 { 0x6E, 0x062D },
 { 0x70, 0x1029 },
 { 0x7F, 0x0001 },
 { 0x05, 0x001F },
 { 0x7F, 0x0000 },
 { 0x30, 0x3006 },
// { 0x0A, 0x0404 },        //PA Bias 0000 0100 '00 00' 0000
 { 0x0A, 0x043F },        //PA Bias 0000 0100 '00 00' 0000
 { 0x1F, 0x0000 },        //GPIO selection 0001 1110 1011 1001
 { 0x30, 0x3006 },
 { 0x0F, 0x6be4 },
 { 0x29, 0x0011 }, //145.525
 { 0x2A, 0xC3A8 }, //145.525
 { 0x48, 0x03FF }, //0000 0011 1111 0000
 { 0x49, 0x01b3 }, //0003
 { 0x3C, 0x0958 }, //0000 1001 0101 1000
 { 0x43, 0x1414 },
 { 0x30, 0x3006 },
 { 0x36, 0x1000 },
 { 0x58, 0x001F },
};


typedef struct
{
  short rxFreqM; //The receive mega portion of the freq (BCD)
  short rxFreqK; //The receive kilo portion of the freq (BCD)
  short txFreqM; //The transmit mega portion of the freq (BCD)
  short txFreqK; //The transmit kilo portion of the freq (BCD)
  char offset;
  short rssi; //
  short vssi; //
  short dtmf; //
  short flags; //
  unsigned short ctcss; //
  //unsigned char contrast;
  unsigned char power;
  unsigned char volume;
  unsigned short sqOpen;
  unsigned short sqClose;
  unsigned char txDTMF[6]; //8 different dtmf transmissions
  //unsigned char txFMDev;
  //unsigned char options; //lpf,hpf,emp

  unsigned char transmitting; //TODO, should be in options bitfield

  unsigned short txTime; //The time that we have been transmiting
} radioSettings_t;

radioSettings_t radioSettings;

void rda_init();
void rda_write(uint8_t addr, uint16_t data);
void rda_setFreq(uint32_t freq);
void rda_setBand(uint8_t band);
void rda_setRef();
void rda_setChannelMode(uint8_t channelMode);
void rda_setTxRx(uint8_t tx);
void rda_setSleep(uint8_t sleep);
void rda_setTxVoiceChannel(uint8_t voiceSel);
void rda_setBiasVoltage(uint8_t biasVoltage);
void rda_setCTCSSFreq(uint16_t freq);
void rda_setCTCSSMode(uint8_t tonemode);
void rda_setDataTX(uint8_t tx);
uint32_t rda_read(uint8_t addr);
void rda_toneWait(uint16_t freq, uint16_t wait);

#endif
