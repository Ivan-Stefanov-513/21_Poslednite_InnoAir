#define Mic_Pin 35
#define SAMPLES 10

bool refresh = false;

uint16_t mic_sample = 0;
uint16_t samples[10] = {0};
uint8_t position = 0;

uint32_t zero = 0, db = 0, noise = 0, noise_avrg = 0;

#define MIC_OFFSET_DB 6
#define MIC_OVERLOAD_DB 120

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

  mic_sample = analogRead(Mic_Pin);
  //Serial.print(','); Serial.print(mic_sample);

  samples[position] = mic_sample;
  position++;
  if (position >= SAMPLES) {
    position = 0;
    refresh = true;
  }

  if (refresh == true) {
    zero = 0;
    for (uint8_t i = 0 ; i < SAMPLES ; i++) {
      zero += samples[i];
    }
    zero /= SAMPLES;
    for (uint8_t i = 0 ; i < SAMPLES ; i++) {
      noise += sq(samples[i] - zero);
    }
    noise /= SAMPLES;
    noise = sqrt(noise);
    //Serial.print(','); Serial.print(noise);
    noise *= 100;

    db = (20 / 2) * log10(noise) + MIC_OFFSET_DB;
    Serial.print(','); Serial.print(db);

    if (db > MIC_OVERLOAD_DB) {
      db = MIC_OVERLOAD_DB;
      //Serial.println("Mic Overloaded!");
    }

  }

  Serial.println(",0,120,;");
}
