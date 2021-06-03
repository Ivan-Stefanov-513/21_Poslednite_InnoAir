#define Debug_Pin 23
uint8_t debug = 0;

#define Mic_Pin 35
#define SAMPLES 10

bool refresh = false;

uint16_t mic_sample = 0;
uint16_t samples[10] = {0};
uint8_t position = 0;

int32_t zero = 0, db = 0, noise = 0, noise_avrg = 0, noise_filter = 5;

#define MIC_OFFSET_DB 6

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

    //noise_avrg = (noise_filter * noise + (100 - noise_filter) * noise_avrg)/100;
    //Serial.print(','); Serial.print(noise_avrg);

    db = (20 / 2) * log10(noise) + MIC_OFFSET_DB;
    Serial.print(','); Serial.print(db);
    //noise_avrg = (noise_filter * db + (100 - noise_filter) * noise_avrg) / 100;
    //Serial.print(','); Serial.print(noise_avrg);

    //if(db > MIC_OVERLOAD_DB)

  }

  // Debug code

  Serial.println(",0,100,;");

  if (debug == 0) {
    digitalWrite(Debug_Pin, HIGH);
    debug = 1;
  }
  else {
    digitalWrite(Debug_Pin, LOW);
    debug = 0;
  }
}
