#define Debug_Pin 26
uint8_t debug = 0;

#define Mic_Pin 25

uint16_t mic_sample = 0;

void setup() {
  Serial.begin(115200);
  pinMode(Mic_Pin, INPUT);

  pinMode(Debug_Pin, OUTPUT);
  digitalWrite(Debug_Pin, LOW);
}

void loop() {

  mic_sample = analogRead(Mic_Pin);
  Serial.print(0);
  Serial.print(',');
  Serial.print(4100);
  Serial.print(',');
  Serial.println(mic_sample);

  if(debug == 0) {
    digitalWrite(Debug_Pin, HIGH);
    debug = 1;
  }
  else {
    digitalWrite(Debug_Pin, LOW);
    debug = 0;
  }
}
