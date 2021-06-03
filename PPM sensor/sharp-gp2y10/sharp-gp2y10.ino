// Developed on Arduino Uno

// ========== HAL ========== //
#define PIN_LED   2
#define PIN_PPM   A0

//Time parameters in uS

#define TIME_HIGH     320
#define TIME_TOSAMPLE 280
#define TIME_PERIOD   10000  

unsigned long start = 0;

void Sharp_gp2y10_init()  {
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);
  pinMode(PIN_PPM, INPUT);
}

// Readings to average
// Maximum is 20
unsigned int getAverageReadings(int count) {
  
  unsigned int readings[25];
  unsigned long average = 0;
  
  noInterrupts();
  count += 5; // Add 5 more, just to make sure the filter is set up properly
  
  for(int i = 0; i < count; i++) {

    // Turn sensor on
    digitalWrite(PIN_LED, HIGH);
    start = micros();
    
    // Wait, until we are on time to read
    for(;start+TIME_TOSAMPLE > micros();); 
    //Read sensor data
    readings[i] = analogRead(PIN_PPM); 
    for(;start+TIME_HIGH > micros(););
    digitalWrite(PIN_LED, LOW);
    if(i >= 5)
      average += readings[i];
    for(;start+TIME_PERIOD > micros(););
  }

  // Finally - enable interrupts and return averaged value
  interrupts();
  return average / (count - 5);
}

void setup() {
  Sharp_gp2y10_init();
}

void loop() {
  

}
