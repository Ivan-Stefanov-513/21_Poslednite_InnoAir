#include "websocket.h"

void test(void);

void setup(void)
{
	// UART initialisation
	Serial.begin(115200);

	// Wi-Fi initialisation
	wifi_init();

	// Secure Websocket initialisation
	websocket_init();

	test();
}

void loop(void)
{

}

void test(void)
{
	message_t message;
	message.message_type = temperature;
	message.message_data.temperature = 27;
	message.station_id = 69;	// Nice

	websocket_send(&message);
}