#include <Wire.h>
#include "bsec.h"

TwoWire I2C_A = TwoWire(0);
TwoWire I2C_B = TwoWire(1);

// Create an object of the class Bsec
Bsec iaqSensor1;
Bsec iaqSensor2;
Bsec iaqSensor3;
Bsec iaqSensor4;

#define I2C_A_SDA_PIN 21
#define I2C_A_SCL_PIN 22
#define I2C_B_SDA_PIN 19
#define I2C_B_SCL_PIN 23

// Helper functions declarations
void errLeds(void);

String output;

bsec_virtual_sensor_t sensorList1[10] = {
  BSEC_OUTPUT_RAW_TEMPERATURE,
  BSEC_OUTPUT_RAW_PRESSURE,
  BSEC_OUTPUT_RAW_HUMIDITY,
  BSEC_OUTPUT_RAW_GAS,
  BSEC_OUTPUT_IAQ,
  BSEC_OUTPUT_STATIC_IAQ,
  BSEC_OUTPUT_CO2_EQUIVALENT,
  BSEC_OUTPUT_BREATH_VOC_EQUIVALENT,
  BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE,
  BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY,
};
bsec_virtual_sensor_t sensorList2[10] = {
  BSEC_OUTPUT_RAW_TEMPERATURE,
  BSEC_OUTPUT_RAW_PRESSURE,
  BSEC_OUTPUT_RAW_HUMIDITY,
  BSEC_OUTPUT_RAW_GAS,
  BSEC_OUTPUT_IAQ,
  BSEC_OUTPUT_STATIC_IAQ,
  BSEC_OUTPUT_CO2_EQUIVALENT,
  BSEC_OUTPUT_BREATH_VOC_EQUIVALENT,
  BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE,
  BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY,
};
bsec_virtual_sensor_t sensorList3[10] = {
  BSEC_OUTPUT_RAW_TEMPERATURE,
  BSEC_OUTPUT_RAW_PRESSURE,
  BSEC_OUTPUT_RAW_HUMIDITY,
  BSEC_OUTPUT_RAW_GAS,
  BSEC_OUTPUT_IAQ,
  BSEC_OUTPUT_STATIC_IAQ,
  BSEC_OUTPUT_CO2_EQUIVALENT,
  BSEC_OUTPUT_BREATH_VOC_EQUIVALENT,
  BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE,
  BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY,
};
bsec_virtual_sensor_t sensorList4[10] = {
  BSEC_OUTPUT_RAW_TEMPERATURE,
  BSEC_OUTPUT_RAW_PRESSURE,
  BSEC_OUTPUT_RAW_HUMIDITY,
  BSEC_OUTPUT_RAW_GAS,
  BSEC_OUTPUT_IAQ,
  BSEC_OUTPUT_STATIC_IAQ,
  BSEC_OUTPUT_CO2_EQUIVALENT,
  BSEC_OUTPUT_BREATH_VOC_EQUIVALENT,
  BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE,
  BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY,
};

void errLeds(void)
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
}

// Helper function definitions
void checkIaqSensorStatus(Bsec *iaqSensor, uint8_t num)
{
  if (iaqSensor->status != BSEC_OK) {
    if (iaqSensor->status < BSEC_OK) {
      output = "Sensor: " + String(num) + " = BSEC error code : " + String(iaqSensor->status);
      Serial.println(output);
      for (;;)
        errLeds(); /* Halt in case of failure */
    } else {
      output = "Sensor: " + String(num) + " = BSEC warning code : " + String(iaqSensor->status);
      Serial.println(output);
    }
  }

  if (iaqSensor->bme680Status != BME680_OK) {
    if (iaqSensor->bme680Status < BME680_OK) {
      output = "Sensor: " + String(num) + " = BME680 error code : " + String(iaqSensor->bme680Status);
      Serial.println(output);
      for (;;)
        errLeds(); /* Halt in case of failure */
    } else {
      output = "Sensor: " + String(num) + " = BME680 warning code : " + String(iaqSensor->bme680Status);
      Serial.println(output);
    }
  }
}

void initBME(Bsec *iaqSensor, bsec_virtual_sensor_t *sensorList, uint8_t num)
{
  output = "\nBSEC " + String(num) + " library version " + String(iaqSensor->version.major) + "." + String(iaqSensor->version.minor) + "." + String(iaqSensor->version.major_bugfix) + "." + String(iaqSensor->version.minor_bugfix);
  Serial.println(output);
  checkIaqSensorStatus(iaqSensor, num);
  iaqSensor->updateSubscription(sensorList, 10, BSEC_SAMPLE_RATE_LP);
  checkIaqSensorStatus(iaqSensor, num);
}

// Function that is printing sensor values
String printIAQ(Bsec *iaqSensor, uint8_t num)
{
  String tabOut = "";
  unsigned long time_trigger = millis();
  if (iaqSensor->run()) { // If new data is available
    output = String(time_trigger);
    output += " Sensor: " + String(num);
    output += " " + String(iaqSensor->rawTemperature);
    output += " " + String(iaqSensor->pressure);
    output += " " + String(iaqSensor->rawHumidity);
    output += " " + String(iaqSensor->gasResistance);
    output += " " + String(iaqSensor->iaq);
    output += " " + String(iaqSensor->iaqAccuracy);
    output += " " + String(iaqSensor->temperature);
    output += " " + String(iaqSensor->humidity);
    output += " " + String(iaqSensor->staticIaq);
    output += " " + String(iaqSensor->co2Equivalent);
    output += " " + String(iaqSensor->breathVocEquivalent);
    //Serial.println(output);

    tabOut += String(num);
    tabOut += " " + String(iaqSensor->rawTemperature);
    tabOut += " " + String(iaqSensor->pressure);
    tabOut += " " + String(iaqSensor->rawHumidity);
    tabOut += " " + String(iaqSensor->gasResistance);
    tabOut += " " + String(iaqSensor->iaq);
    tabOut += " " + String(iaqSensor->iaqAccuracy);
    tabOut += " " + String(iaqSensor->temperature);
    tabOut += " " + String(iaqSensor->humidity);
    tabOut += " " + String(iaqSensor->staticIaq);
    tabOut += " " + String(iaqSensor->co2Equivalent);
    tabOut += " " + String(iaqSensor->breathVocEquivalent);
  } else {
    checkIaqSensorStatus(iaqSensor, num);
  }
  return tabOut;
}
