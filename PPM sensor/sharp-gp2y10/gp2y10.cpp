#include "gp2y10.h"
#include "Arduino.h"

void Sharp_gp2y10_init()  {
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, PPM_LED_OFF);
  pinMode(PIN_PPM, INPUT);
}
