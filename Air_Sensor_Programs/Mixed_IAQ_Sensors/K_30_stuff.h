#include <SoftwareSerial.h>
#define NUM_K30 2
//SoftwareSerial K_30_Serial[NUM_K30] = {SoftwareSerial(5, 4), SoftwareSerial(7, 6)}; //K-30 1 - Rx pin 5, Tx pin 4, K-30 2 - Rx pin 7, Tx pin 6
SoftwareSerial K_30_Serial_1(12, 14); //K-30 1 - Rx pin 5, Tx pin 4
SoftwareSerial K_30_Serial_2(27, 26); //K-30 2 - Rx pin 7, Tx pin 6

byte request[7] = {0xFE, 0X44, 0X00, 0X08, 0X02, 0X9F, 0X25}; //Command packet to read Co2 (see app note)
byte response[NUM_K30][7] = {0}; //create an array to store the response
int valMultiplier = 1; //default is 1. set to 3 for K-30 3% and 10 for K-33 ICB

void K30_sendRequest(byte *packet, SoftwareSerial& sensorPort, uint8_t num)
{
  while (!sensorPort.available()) //keep sending request until we start to get a response
  {
    /*
      Serial.print("K-30 Sensor: ");
      Serial.print(num+1);
      Serial.println(" serial port unavailable - waiting");
    */
    sensorPort.write(packet, 7);
    delay(50);
  }
  uint8_t timeout = 0; //set a timeout counter
  while (sensorPort.available() < 7 ) //Wait to get a 7 byte response
  {
    timeout++;
    if (timeout > 10) //if it takes too long there was probably an error
    {
      /*
        Serial.print("K-30 Sensor: ");
        Serial.print(num+1);
        Serial.println(" Timeout");
      */
      while (sensorPort.available()) //flush whatever we have
        sensorPort.read();
      break; //exit and try again
      timeout = 0;
    }
    delay(50);
  }
  for (uint8_t i = 0; i < 7; i++)
  {
    response[num][i] = sensorPort.read();
  }
}

unsigned long K30_getValue(byte packet[])
{
  int high = packet[3]; //high byte for value is 4th byte in packet in the packet
  int low = packet[4]; //low byte for value is 5th byte in the packet
  unsigned long val = high * 256 + low; //Combine high byte and low byte with this formula to get value
  return val * valMultiplier;
}

unsigned long K30_readCO2(uint8_t num)
{
  if (num == 0)
  {
    K_30_Serial_1.listen();
    K30_sendRequest(request, K_30_Serial_1, num);
  }
  else if (num == 1)
  {
    K_30_Serial_2.listen();
    K30_sendRequest(request, K_30_Serial_2, num);
  }
  return K30_getValue(response[num]);
}
