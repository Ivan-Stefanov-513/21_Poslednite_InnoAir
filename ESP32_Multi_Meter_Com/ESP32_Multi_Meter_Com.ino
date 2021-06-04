#include "websocket.h"
#include "Noise.h"

message_t noise_data;

unsigned long previousMillis = 0;
unsigned long currentMillis = 0;
const long interval = 2000;

void setup() {
  Serial.begin(250000);
  pinMode(Mic_Pin, ANALOG);
  analogReadResolution(12);
  analogSetWidth(12);
  analogSetAttenuation(ADC_0db);
  analogSetClockDiv(1);
  delay(100);

  wifi_init();
  websocket_init();
}

void loop() {
  uint32_t spl = noise_measure();
  //Serial.print(spl);  Serial.println(",0,120,");

  currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    Serial.println(spl);

    noise_data.message_type = noise;
    noise_data.message_data.noise = spl;
    noise_data.station_id = 73;

    websocket_send(&noise_data);
  }
}

uint32_t noise_measure(void) {
  // Measure sound level + add sample to buffer
  mic_sample[sample_pos] = analogRead(Mic_Pin);
  sample_pos++;
  if (sample_pos >= SAMPLES) {
    sample_pos = 0;
  }

  // Calculate average - DC bias
  zero = 0;
  for (uint8_t i = 0 ; i < SAMPLES ; i++) {
    zero += mic_sample[i];
  }
  zero /= SAMPLES;

  // Calculate AC RMS - noise level
  noisei = 0;
  for (uint8_t i = 0 ; i < SAMPLES ; i++) {
    noisei += sq(mic_sample[i] - zero);
  }
  noisei /= SAMPLES;
  noisei = sqrt(noisei);

  // Calculate SPL - dB
  noisei *= 100;
  db = (20 / 2) * log10(noisei) + MIC_OFFSET_DB;

  // Microphone and preamp clipping limit
  if (db > MIC_OVERLOAD_DB) {
    db = MIC_OVERLOAD_DB;
  }

  return db;
}
