#include "Noise.h"

void setup() {
  Serial.begin(250000);
  pinMode(Mic_Pin, ANALOG);
  analogReadResolution(12);
  analogSetWidth(12);
  analogSetAttenuation(ADC_0db);
  analogSetClockDiv(1);
  delay(100);
}

void loop() {
  Serial.print(noise_measure());
  Serial.println(",0,120,");
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
  noise = 0;
  for (uint8_t i = 0 ; i < SAMPLES ; i++) {
    noise += sq(mic_sample[i] - zero);
  }
  noise /= SAMPLES;
  noise = sqrt(noise);

  // Calculate SPL - dB
  noise *= 100;
  db = (20 / 2) * log10(noise) + MIC_OFFSET_DB;

  // Microphone and preamp clipping limit
  if (db > MIC_OVERLOAD_DB) {
    db = MIC_OVERLOAD_DB;
  }

  return db;
}
