// Developed on Arduino Uno
#include "gp2y10.h"



  

unsigned long start = 0;



// Readings to average
// Maximum is 20
float getReading() {
  
  unsigned int readings[105];
  unsigned long last = 0;
  int count = 1;
  
  noInterrupts();
  count += 5; // Add 5 more, just to make sure the filter is set up properly
  
  
  for(int i = 0; i < count; i++) {

    // Turn sensor on
    digitalWrite(PIN_LED, PPM_LED_ON);
    start = micros();
    
    // Wait, until we are on time to read
    for(;start+TIME_TOSAMPLE > micros();); 
    //Read sensor data
    last = analogRead( PIN_PPM );  
    for(;start+TIME_HIGH > micros(););
    digitalWrite(PIN_LED, PPM_LED_OFF);
    for(;start+TIME_PERIOD > micros(););
  }

  // Finally - enable interrupts and return averaged value
  interrupts();
  
  return last * 0.8058;
}

void setup() {
  Sharp_gp2y10_init();
  Serial.begin(9600);
  
}

void loop() {
  float milivolts = getReading();
  int filtered = milivolts*100;
  
  Serial.println(milivolts);
}
