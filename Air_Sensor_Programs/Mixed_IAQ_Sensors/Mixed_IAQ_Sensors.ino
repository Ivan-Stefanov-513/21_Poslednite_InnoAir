// SDS011
#include "SdsDustSensor.h"
#define TX2 17;
#define RX2 16;
SdsDustSensor sds(Serial2);

// K-30
#include "K_30_stuff.h"

// BME680
#include "BME680stuff.h"

// WiFi
const char * networkName = "BG_hotspot";//"SA_AP1_2,4GHz";
const char * networkPswd = "bg123456789";
const char * udpAddress = "192.168.43.255";
const int udpPort = 3333;
#include "WiFi_stuff.h"

void setup() {
  // Common
  Serial.begin(115200);
  Serial.println("IAQ Sensor Mish-Mash\n");

  // Wi-Fi setup
  connectToWiFi(networkName, networkPswd);
  WiFi.setTxPower(WIFI_POWER_19_5dBm);
  int power_con = WiFi.getTxPower();
  Serial.print("Tx power: ");
  Serial.println(power_con);
  /*
    // SDS011
    sds.begin();
    Serial.println(sds.queryFirmwareVersion().toString()); // prints firmware version
    Serial.println(sds.setActiveReportingMode().toString()); // ensures sensor is in 'active' reporting mode
    Serial.println(sds.setCustomWorkingPeriod(3).toString()); // sensor sends data every 3 minutes
  */
  // K-30
  for (uint8_t i = 0; i < NUM_K30; i++)
  {
    K_30_Serial_1.begin(9600); //Opens the virtual serial port with a baud of 9600
    K_30_Serial_2.begin(9600); //Opens the virtual serial port with a baud of 9600
  }

  // BME680
  I2C_A.begin(I2C_A_SDA_PIN, I2C_A_SCL_PIN, 100000);
  I2C_B.begin(I2C_B_SDA_PIN, I2C_B_SCL_PIN, 100000);

  //iaqSensor1.begin(BME680_I2C_ADDR_PRIMARY, I2C_A);   // 0x76 - SDO to GND
  iaqSensor2.begin(BME680_I2C_ADDR_SECONDARY, I2C_A); // 0x77 - SDO floating
  //iaqSensor3.begin(BME680_I2C_ADDR_PRIMARY, I2C_B);   // 0x76 - SDO to GND
  iaqSensor4.begin(BME680_I2C_ADDR_SECONDARY, I2C_B); // 0x77 - SDO floating

  //initBME(&iaqSensor1, sensorList1, 1);
  initBME(&iaqSensor2, sensorList2, 2);
  //initBME(&iaqSensor3, sensorList3, 3);
  initBME(&iaqSensor4, sensorList4, 4);
  /*
    // Print the header
    output = "\nTimestamp [ms], raw temperature [°C], pressure [hPa], raw relative humidity [%], gas [Ohm], IAQ, IAQ accuracy, temperature [°C], relative humidity [%], Static IAQ, CO2 equivalent, breath VOC equivalent";
    Serial.println(output);
  */
}

void loop() {
  // Common
  // WiFi
  if (connected)
  {
    output_str = "";

    output_str += K30_readCO2(0);
    output_str += "\t";
    output_str += K30_readCO2(1);
    output_str += "\t";
    output_str += printIAQ(&iaqSensor2, 2);
    output_str += "\t";
    output_str += printIAQ(&iaqSensor4, 4);
    output_str += "\t";

    time_ = esp_timer_get_time() / 1000; // us->ms
    output_str += String((int32_t)time_);
    udp.beginPacket(udpAddress, udpPort);
    udp.print(output_str);
    udp.endPacket();
    Serial.println(output_str);
  }
  else
  {
    connectToWiFi(networkName, networkPswd);
    printIAQ(&iaqSensor2, 2);
    printIAQ(&iaqSensor4, 4);
  }
  delay(3000);
  /*
    // SDS011 - repeat T=5s
    PmResult pm = sds.readPm();
    if (pm.isOk()) {
      Serial.print("PM2.5 = ");
      Serial.print(pm.pm25);
      Serial.print(", PM10 = ");
      Serial.println(pm.pm10);
    }

    // K-30
    Serial.print("K-30 Sensor 1 CO2 ppm = ");
    Serial.print(K30_readCO2(0));
    Serial.print(", Sensor 2 CO2 ppm = ");
    Serial.println(K30_readCO2(1));
    delay(1000);

    // BME680
    //printIAQ(&iaqSensor1, 1);
    printIAQ(&iaqSensor2, 2);
    //printIAQ(&iaqSensor3, 3);
    printIAQ(&iaqSensor4, 4);
  */
}
