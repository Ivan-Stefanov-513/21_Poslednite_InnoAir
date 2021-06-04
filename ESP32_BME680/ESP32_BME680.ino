#include "bsec.h"
#include "websocket.h"

#define VLADO_STATION_ID	86

#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22

// Helper functions declarations
void checkIaqSensorStatus(void);
void errLeds(void);

// Create an object of the class Bsec
Bsec iaqSensor;

String output;

// Entry point for the example
void setup(void)
{
	Serial.begin(115200);
	Serial.println("\nBME680 BSEC Test");   // Test line
	Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

	iaqSensor.begin(BME680_I2C_ADDR_PRIMARY, Wire);
	//iaqSensor.begin(BME680_I2C_ADDR_SECONDARY, Wire);
	output = "\nBSEC library version " + String(iaqSensor.version.major) + "." + String(iaqSensor.version.minor) + "." + String(iaqSensor.version.major_bugfix) + "." + String(iaqSensor.version.minor_bugfix);
	Serial.println(output);
	checkIaqSensorStatus();

	bsec_virtual_sensor_t sensorList[10] = {
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

	iaqSensor.updateSubscription(sensorList, 10, BSEC_SAMPLE_RATE_LP);
	checkIaqSensorStatus();

	// Print the header
	output = "Timestamp [ms], raw temperature [°C], pressure [hPa], raw relative humidity [%], gas [Ohm], IAQ, IAQ accuracy, temperature [°C], relative humidity [%], Static IAQ, CO2 equivalent, breath VOC equivalent";
	Serial.println(output);

	// Wi-Fi initialisation
	wifi_init();

	// Secure Websocket initialisation
	websocket_init();
}

// Function that is looped forever
void loop(void)
{
	message_t message =
	{
		.message_type = humidity,
		.message_data = {.humidity = 0},
		.station_id = VLADO_STATION_ID
	};

	unsigned long time_trigger = millis();
	if (iaqSensor.run()) { // If new data is available
		output = String(time_trigger);
		output += ", " + String(iaqSensor.rawTemperature);
		output += ", " + String(iaqSensor.pressure);
		output += ", " + String(iaqSensor.rawHumidity);
		output += ", " + String(iaqSensor.gasResistance);
		output += ", " + String(iaqSensor.iaq);
		output += ", " + String(iaqSensor.iaqAccuracy);
		output += ", " + String(iaqSensor.temperature);
		output += ", " + String(iaqSensor.humidity);
		output += ", " + String(iaqSensor.staticIaq);
		output += ", " + String(iaqSensor.co2Equivalent);
		output += ", " + String(iaqSensor.breathVocEquivalent);
		Serial.println(output);
	} else {
		checkIaqSensorStatus();
	}

	message.message_data.humidity = iaqSensor.humidity;
	websocket_send(&message);
	message.message_type = temperature;
	message.message_data.temperature = iaqSensor.temperature;
	websocket_send(&message);
	message.message_type = pressure;
	message.message_data.pressure = iaqSensor.pressure;
	websocket_send(&message);
	message.message_type = air_quality;
	message.message_data.air_quality = iaqSensor.iaq;
	websocket_send(&message);

	delay(10000);
}

// Helper function definitions
void checkIaqSensorStatus(void)
{
	if (iaqSensor.status != BSEC_OK) {
		if (iaqSensor.status < BSEC_OK) {
			output = "BSEC error code : " + String(iaqSensor.status);
			Serial.println(output);
			for (;;);
				//errLeds(); /* Halt in case of failure */
		} else {
			output = "BSEC warning code : " + String(iaqSensor.status);
			Serial.println(output);
		}
	}

	if (iaqSensor.bme680Status != BME680_OK) {
		if (iaqSensor.bme680Status < BME680_OK) {
			output = "BME680 error code : " + String(iaqSensor.bme680Status);
			Serial.println(output);
			for (;;);
				//errLeds(); /* Halt in case of failure */
		} else {
			output = "BME680 warning code : " + String(iaqSensor.bme680Status);
			Serial.println(output);
		}
	}
}

