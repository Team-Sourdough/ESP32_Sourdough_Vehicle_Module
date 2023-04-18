#ifndef MIC_H
#define MIC_H

#include "../common.h"
#include "../Button/Button.h"
#include "../I2S_Driver/I2S.h"
#include "../wav/Wav.h"
#include <driver/i2s.h>
#include <FS.h>
#include <SD.h>
#include <SPI.h>

#define SCK  17
#define MISO  18
#define MOSI  16
#define CS  6
#define I2S_MODE I2S_MODE_RX
SPIClass spi = SPIClass(HSPI);
char record_runs = '0';

void mic_setup();
void sd_setup();
void button_setup();
void Mic_Task(void* p_arg);


#endif