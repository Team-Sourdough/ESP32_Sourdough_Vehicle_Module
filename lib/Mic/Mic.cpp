#include "Mic.h"
#include <Arduino.h>

// void mic_setup(){
//     I2S_Init(I2S_MODE, I2S_BITS_PER_SAMPLE_32BIT);

// }
// void sd_setup(){
//   // spi.begin(SCK, MISO, MOSI, CS);
//   if (!SD.begin(CS,spi,80000000)) {
//     Serial.println("Card Mount Failed");
    
//   }
//   while(!SD.begin(CS,spi,80000000)) {
//     Serial.print(".");
//     delay(500);
//   }
// }
void button_setup(){
    //Connect GPIO 2 to Interrupt Service Routine to act as mic
    pinMode(BUTTON_GPIO, INPUT_PULLUP);
    attachInterrupt(BUTTON_GPIO, Button_ISR, FALLING);
}
// void Test_Task(void* p_arg){
//   uint32_t timer = millis();
//   while(1) {
//       if (millis() - timer > 2000) {
//             // Reset the timer
//             Serial.println("Test Task");
//             timer = millis();
//           }
//       vTaskDelay(x10ms);
//   }
// }

void Mic_Task(void* p_arg){
    // //Mic setup
    // const int record_time =  5;  // second

    // char filename[18] = "/Task_Record";
    // filename[12] = record_runs;
    // char ext[] = ".wav";
    // strcat(filename,ext);
    // filename[18] = '0';

    // const int headerSize = 44;
    // const int waveDataSize = record_time * 88000;
    // const int numCommunicationData = 8000;
    // const int numPartWavData = numCommunicationData/4;
    // byte header[headerSize];
    // char communicationData[numCommunicationData];
    // char partWavData[numPartWavData];
    // File file;

    // sd_setup();
    // mic_setup();
    button_setup();
    while(1){         
        // filename[12] = record_runs;
        // CreateWavHeader(header, waveDataSize);

        // Serial.print("Starting recording ");
        // Serial.println(record_runs);
        // if(SD.exists(filename)) {
        //   SD.remove(filename);
        // }
        // if(record_runs == '9') {
        //   record_runs = '0';
        // }
        // file = SD.open(filename, FILE_WRITE);
        // if (!file)  {
        //   Serial.print("File Failed");
        //   return;
        // }
        // file.write(header, headerSize);
        // for (int j = 0; j < waveDataSize/numPartWavData; ++j) {
        //   I2S_Read(communicationData, numCommunicationData);
        //   for (int i = 0; i < numCommunicationData/8; ++i) {
        //     partWavData[2*i] = communicationData[8*i + 2];
        //     partWavData[2*i + 1] = communicationData[8*i + 3];
        //   }
        //   file.write((const byte*)partWavData, numPartWavData);
        // }
        // file.close();
        // Serial.println("Finished Recording.");
        // record_runs++;
        delay(100);
       }
}