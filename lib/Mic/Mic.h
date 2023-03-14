#ifndef MIC_H
#define MIC_H

#include "../common.h"
#include "../Button/Button.h"
#include "../I2S_Driver/I2S.h"
#include "../wav/wav.h"
#include <driver/i2s.h>
#include <FS.h>
#include <SD.h>
#include <SPI.h>

#define SCK  17
#define MISO  18
#define MOSI  16
#define CS  6
#define I2S_MODE I2S_MODE_RX //We are receiving sound, not sending it.
SPIClass spi = SPIClass(HSPI); //Use different SPI line than RF module

void mic_setup();
void sd_setup();
void button_setup();
void Mic_Task(void* p_arg);


#endif