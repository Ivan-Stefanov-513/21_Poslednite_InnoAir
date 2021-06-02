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
	char message[] = "kurche";

	websocket_send((message_t *) message);
}