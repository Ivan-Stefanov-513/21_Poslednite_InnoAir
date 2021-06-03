#define Debug_Pin 23
uint8_t debug = 0;

#define Mic_Pin 35

uint16_t mic_sample = 0;
uint16_t samples[10] = {0};
uint8_t position = 0;
bool refresh = false;
uint32_t zero = 0, noise = 0;

#define SAMPLES 10

void setup() {
  Serial.begin(250000);
  pinMode(Mic_Pin, ANALOG);
  analogReadResolution(12);
  analogSetWidth(12);
  analogSetAttenuation(ADC_0db);
  analogSetClockDiv(1);

  pinMode(Debug_Pin, OUTPUT);
  digitalWrite(Debug_Pin, LOW);
}

void loop() {

  mic_sample = analogRead(Mic_Pin);
  /*
  Serial.print(0);
  Serial.print(',');
  Serial.print(4100);
  Serial.print(',');
  Serial.println(mic_sample);
*/
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

    Serial.print(0);
    Serial.print(',');
    Serial.print(4100);
    Serial.print(',');
    Serial.print(zero);
    Serial.print(',');
    Serial.println(noise);
  }

  if (debug == 0) {
    digitalWrite(Debug_Pin, HIGH);
    debug = 1;
  }
  else {
    digitalWrite(Debug_Pin, LOW);
    debug = 0;
  }
}
