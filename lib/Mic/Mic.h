#ifndef MIC_H
#define MIC_H

#include "../common.h"
#include "../Button/Button.h"
#include "../I2S_Driver/I2S.h"
#include "../wav/wav.h"
#include <driver/i2s.h>
#include <FS.h>
#include <SD.h>

#define SCK  17
#define MISO  18
#define MOSI  8
#define CS  5
#define I2S_MODE I2S_MODE_RX
SPIClass spi = SPIClass(SPI);
char record_runs = '0';

void mic_setup();
void sd_setup();
void button_setup();
void Mic_Task(void* p_arg);


#endif