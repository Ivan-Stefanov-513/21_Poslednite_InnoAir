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

void initBME(Bsec *iaqSensor, bsec_virtual_sensor_t *sensorList, uint8_t num)
{
  output = "\nBSEC " + String(num) + " library version " + String(iaqSensor->version.major) + "." + String(iaqSensor->version.minor) + "." + String(iaqSensor->version.major_bugfix) + "." + String(iaqSensor->version.minor_bugfix);
  Serial.println(output);
  checkIaqSensorStatus(iaqSensor, num);
  iaqSensor->updateSubscription(sensorList, 10, BSEC_SAMPLE_RATE_LP);
  checkIaqSensorStatus(iaqSensor, num);
}

// Entry point for the example
void setup(void)
{
  Serial.begin(115200);
  Serial.println("\nBME680 BSEC Test");   // Test line
  I2C_A.begin(I2C_A_SDA_PIN, I2C_A_SCL_PIN, 100000);
  I2C_B.begin(I2C_B_SDA_PIN, I2C_B_SCL_PIN, 100000);

  iaqSensor1.begin(BME680_I2C_ADDR_PRIMARY, I2C_A);   // 0x76 - SDO to GND
  iaqSensor2.begin(BME680_I2C_ADDR_SECONDARY, I2C_A); // 0x77 - SDO floating
  iaqSensor3.begin(BME680_I2C_ADDR_PRIMARY, I2C_B);   // 0x76 - SDO to GND
  iaqSensor4.begin(BME680_I2C_ADDR_SECONDARY, I2C_B); // 0x77 - SDO floating

  initBME(&iaqSensor1, sensorList1, 1);
  initBME(&iaqSensor2, sensorList2, 2);
  //initBME(&iaqSensor3, sensorList3, 3);
  initBME(&iaqSensor4, sensorList4, 4);

  // Print the header
  output = "\nTimestamp [ms], raw temperature [°C], pressure [hPa], raw relative humidity [%], gas [Ohm], IAQ, IAQ accuracy, temperature [°C], relative humidity [%], Static IAQ, CO2 equivalent, breath VOC equivalent";
  Serial.println(output);
}

// Function that is looped forever
void loop(void)
{
  //printIAQ(&iaqSensor1, 1);
  printIAQ(&iaqSensor2, 2);
  //printIAQ(&iaqSensor3, 3);
  printIAQ(&iaqSensor4, 4);
}

// Function that is looped forever
void printIAQ(Bsec *iaqSensor, uint8_t num)
{
  unsigned long time_trigger = millis();
  if (iaqSensor->run()) { // If new data is available
    output = String(time_trigger);
    output += ", Sensor: " + String(num);
    output += ", " + String(iaqSensor->rawTemperature);
    output += ", " + String(iaqSensor->pressure);
    output += ", " + String(iaqSensor->rawHumidity);
    output += ", " + String(iaqSensor->gasResistance);
    output += ", " + String(iaqSensor->iaq);
    output += ", " + String(iaqSensor->iaqAccuracy);
    output += ", " + String(iaqSensor->temperature);
    output += ", " + String(iaqSensor->humidity);
    output += ", " + String(iaqSensor->staticIaq);
    output += ", " + String(iaqSensor->co2Equivalent);
    output += ", " + String(iaqSensor->breathVocEquivalent);
    Serial.println(output);
  } else {
    checkIaqSensorStatus(iaqSensor, num);
  }
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

void errLeds(void)
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
}
