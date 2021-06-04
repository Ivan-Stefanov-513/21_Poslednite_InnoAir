#include "websocket.h"
#include "Noise.h"
#include <SoftwareSerial.h>

SoftwareSerial K_30_Serial(18, 19); //Sets up a virtual serial port
//Using pin 18 for Rx and pin 19 for Tx
byte readCO2[] = {0xFE, 0X44, 0X00, 0X08, 0X02, 0X9F, 0X25}; //Command packet to read Co2 (see app note)
byte response[] = {0, 0, 0, 0, 0, 0, 0}; //create an array to store the response
//multiplier for value. default is 1. set to 3 for K-30 3% and 10 for K-33 ICB
int valMultiplier = 1;

message_t noise_data;
message_t co2_data;

unsigned long previousMillis = 0;
unsigned long currentMillis = 0;
const long interval = 1000;

void setup() {
  Serial.begin(250000);
  Serial.println();
  Serial.println("Noise and CO2 Meter");
  pinMode(Mic_Pin, ANALOG);
  analogReadResolution(12);
  analogSetWidth(12);
  analogSetAttenuation(ADC_0db);
  analogSetClockDiv(1);
  K_30_Serial.begin(9600);
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

    sendRequest(readCO2);
    unsigned long valCO2 = getValue(response);

    Serial.print("SPL = ");
    Serial.print(spl);
    Serial.print("dB, CO2 ppm = ");
    Serial.println(valCO2);


    noise_data.message_type = noise;
    noise_data.message_data.noise = spl;
    noise_data.station_id = 73;
    websocket_send(&noise_data);

    co2_data.message_type = co2;
    co2_data.message_data.co2 = (uint32_t)valCO2;
    co2_data.station_id = 73;
    websocket_send(&co2_data);

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


void sendRequest(byte packet[])
{
  while (!K_30_Serial.available()) //keep sending request until we start to get a response
  {
    //Serial.println("waiting for Software.serial port availability");
    K_30_Serial.write(readCO2, 7);
    delay(50);
  }
  int timeout = 0; //set a timeout counter
  while (K_30_Serial.available() < 7 ) //Wait to get a 7 byte response
  {
    timeout++;
    if (timeout > 10) //if it takes too long there was probably an error
    {
      Serial.println("Sensor - Timeout");
      while (K_30_Serial.available()) //flush whatever we have
        K_30_Serial.read();
      break; //exit and try again
    }
    delay(50);
  }
  for (int i = 0; i < 7; i++)
  {
    response[i] = K_30_Serial.read();
  }
}

unsigned long getValue(byte packet[])
{
  int high = packet[3]; //high byte for value is 4th byte in packet in the packet
  int low = packet[4]; //low byte for value is 5th byte in the packet
  unsigned long val = high * 256 + low; //Combine high byte and low byte with this formula to get value
  return val * valMultiplier;
}
