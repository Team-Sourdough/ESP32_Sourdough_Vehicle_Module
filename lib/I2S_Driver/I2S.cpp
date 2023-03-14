#include "I2S.h"

void I2S_Init(i2s_mode_t MODE, i2s_bits_per_sample_t BPS)
{
  i2s_config_t i2s_config = {
      .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
      .sample_rate = SAMPLE_RATE,
      .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
      .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
      .communication_format = I2S_COMM_FORMAT_I2S, //I2S_COMM_FORMAT_STAND_I2S - deprecated
      .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
      .dma_buf_count = 4,
      .dma_buf_len = 1024,
      .use_apll = false,
      .tx_desc_auto_clear = false,
      .fixed_mclk = 0};
      //Configure correct pins. DOUT set to -1 since unused.
 i2s_pin_config_t i2s_mic_pins = {
    .bck_io_num = I2S_MIC_SERIAL_CLOCK,
    .ws_io_num = I2S_MIC_LEFT_RIGHT_CLOCK,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_MIC_SERIAL_DATA};

    //Indicate successful setup and configuration
    Serial.println("Installing I2S Driver");
    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    Serial.println("I2S Driver Installed");
    i2s_set_pin(I2S_NUM_0, &i2s_mic_pins);
    Serial.println("I2S Pins Set");
    i2s_set_clk(I2S_NUM_0, SAMPLE_RATE, BPS, I2S_CHANNEL_STEREO);
    Serial.println("I2S Clock Set");
}
size_t bytesre;
int I2S_Read(char *data, int numData)
{
  return i2s_read(I2S_NUM_0, (char *)data, numData, &bytesre, portMAX_DELAY); //Bytesre not neeed since we implicitly know how long to record for.
}
void I2S_Read2(char *data, int numData)
{
  size_t bytes_read = 0;
  i2s_read(I2S_NUM_0, (char *)data, numData, &bytesre, portMAX_DELAY);
  int samples_read = bytesre / sizeof(int32_t);
  // dump the samples out to the serial channel.
  for (int i = 0; i < samples_read; i++)
  {
    Serial.printf("%ld\n", data[i]);
  }
  return;
}
